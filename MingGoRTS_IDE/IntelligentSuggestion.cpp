/**
 * MingGoRTS IDE Intelligent Suggestion System Implementation
 */

#include "IntelligentSuggestion.h"
#include <algorithm>
#include <iterator>
#include <regex>
#include <sstream>
#include <iostream>
#include <unordered_set>

namespace MingGoRTSIDE {

// ============================================================================
// Intelligent Suggestion System
// ============================================================================

namespace {
// SuggestionType 對應的權重鍵名——id 前綴與 suggestionWeights 共用它，
// 讓 LearnFromFeedback 能從 id 回推類型
const char* SuggestionTypeKey(SuggestionType type) {
    switch (type) {
        case SuggestionType::CodeCompletion: return "code_completion";
        case SuggestionType::Refactoring: return "refactoring";
        case SuggestionType::Optimization: return "optimization";
        case SuggestionType::BugFix: return "bug_fix";
        case SuggestionType::BestPractice: return "best_practice";
        case SuggestionType::Documentation: return "documentation";
        case SuggestionType::TestGeneration: return "test_generation";
        case SuggestionType::Architectural: return "architectural";
    }
    return "code_completion";
}

// 命中位置 → 1-based 行號
int LineOfPosition(const std::string& code, size_t pos) {
    return 1 + static_cast<int>(
        std::count(code.begin(), code.begin() + static_cast<std::ptrdiff_t>(pos), '\n'));
}

// 第一個 regex 命中的 1-based 行號；無命中回 0
int FirstMatchLine(const std::string& code, const std::regex& re) {
    std::smatch m;
    if (std::regex_search(code, m, re)) {
        return LineOfPosition(code, static_cast<size_t>(m.position(0)));
    }
    return 0;
}

// anti-pattern 表：名稱 → 修正建議。FindAntiPatterns 與建議生成共用同一表，
// 名單不再兩處各寫一份（scanf 已在 bannedApis 以更準訊息覆蓋，此處不重複列）
const std::vector<std::pair<std::string, std::pair<std::regex, std::string>>>&
AntiPatternTable() {
    static const std::vector<std::pair<std::string, std::pair<std::regex, std::string>>> t = {
        {"goto",   {std::regex("\\bgoto\\b"),        "Structured control flow is easier to reason about"}},
        {"malloc", {std::regex("\\bmalloc\\s*\\("),  "Use RAII containers or smart pointers instead of malloc"}},
        {"free",   {std::regex("\\bfree\\s*\\("),    "Manual free is fragile — prefer RAII ownership"}},
        {"printf", {std::regex("\\bprintf\\s*\\("),  "Prefer typed output (iostream/std::format)"}},
    };
    return t;
}
} // namespace

IntelligentSuggestionSystem::IntelligentSuggestionSystem()
    : textEmbedding(nullptr)
    , nlpPipeline(nullptr)
    , suggestionModel(nullptr)
    , totalSuggestions(0)
    , acceptedSuggestions(0)
    , nextSuggestionId(0) {
}

IntelligentSuggestionSystem::~IntelligentSuggestionSystem() {
    Shutdown();
}

bool IntelligentSuggestionSystem::Initialize() {
    std::cout << "Initializing Intelligent Suggestion System...\n";

    // Initialize suggestion weights
    {
        std::lock_guard<std::mutex> lk(sharedMutex_);
        suggestionWeights = {
            {"code_completion", 0.8f},
            {"refactoring", 0.6f},
            {"optimization", 0.7f},
            {"bug_fix", 0.9f},
            {"best_practice", 0.5f},
            {"documentation", 0.4f},
            {"test_generation", 0.6f},
            {"architectural", 0.5f}
        };
    }

    // 背景分析 worker（B-1）：分析移出 render thread
    {
        std::lock_guard<std::mutex> lk(queueMutex_);
        workerStop_ = false;
    }
    if (!worker_.joinable()) {
        worker_ = std::thread(&IntelligentSuggestionSystem::WorkerMain, this);
    }

    std::cout << "Intelligent Suggestion System initialized successfully\n";
    return true;
}

void IntelligentSuggestionSystem::Shutdown() {
    std::cout << "Shutting down Intelligent Suggestion System...\n";
    {
        std::lock_guard<std::mutex> lk(queueMutex_);
        workerStop_ = true;
        hasPendingJob_ = false;
    }
    queueCv_.notify_all();
    if (worker_.joinable()) worker_.join();
}

uint64_t IntelligentSuggestionSystem::SubmitAnalysis(
    const std::string& code,
    const std::string& filePath,
    int lineNumber,
    int columnNumber) {

    const uint64_t id = ++nextJobId_;
    {
        std::lock_guard<std::mutex> lk(queueMutex_);
        // 單槽 supersede：直接覆寫未執行的舊 job
        pendingJob_ = {id, code, filePath, lineNumber, columnNumber};
        hasPendingJob_ = true;
    }
    queueCv_.notify_one();
    return id;
}

bool IntelligentSuggestionSystem::PollResult(uint64_t& doneJobId,
                                             std::vector<Suggestion>& out) {
    std::lock_guard<std::mutex> lk(resultMutex_);
    if (doneJobId_ == 0) return false;
    doneJobId = doneJobId_;
    out = std::move(doneSuggestions_);
    doneSuggestions_.clear();
    doneJobId_ = 0;
    return true;
}

void IntelligentSuggestionSystem::WorkerMain() {
    for (;;) {
        AnalysisJob job;
        {
            std::unique_lock<std::mutex> lk(queueMutex_);
            queueCv_.wait(lk, [&] { return workerStop_ || hasPendingJob_; });
            if (workerStop_) return;
            job = pendingJob_;
            hasPendingJob_ = false;
        }

        std::vector<Suggestion> result = GenerateSuggestions(
            job.code, job.filePath, job.line, job.col);

        std::lock_guard<std::mutex> lk(resultMutex_);
        // 執行期間已有更新快照排入 → 這份結果是 stale，直接丟棄
        if (job.id == nextJobId_.load()) {
            doneJobId_ = job.id;
            doneSuggestions_ = std::move(result);
        }
    }
}

std::vector<Suggestion> IntelligentSuggestionSystem::GenerateSuggestions(
    const std::string& code,
    const std::string& filePath,
    int lineNumber,
    int columnNumber) {
    
    std::vector<Suggestion> suggestions;
    
    // Analyze code
    CodeAnalysis analysis = AnalyzeCode(code, filePath);
    
    // Generate different types of suggestions
    std::vector<Suggestion> refactoring = GetRefactoringSuggestions(analysis);
    std::vector<Suggestion> optimization = GetOptimizationSuggestions(analysis);
    std::vector<Suggestion> bugs = DetectBugs(code);
    std::vector<Suggestion> bestPractices = GetBestPracticeRecommendations(analysis);
    std::vector<Suggestion> documentation = GetDocumentationSuggestions(analysis);
    std::vector<Suggestion> tests = GenerateTestSuggestions(analysis);
    std::vector<Suggestion> architectural = GetArchitecturalSuggestions(analysis);
    
    // Combine all suggestions
    suggestions.insert(suggestions.end(), refactoring.begin(), refactoring.end());
    suggestions.insert(suggestions.end(), optimization.begin(), optimization.end());
    suggestions.insert(suggestions.end(), bugs.begin(), bugs.end());
    suggestions.insert(suggestions.end(), bestPractices.begin(), bestPractices.end());
    suggestions.insert(suggestions.end(), documentation.begin(), documentation.end());
    suggestions.insert(suggestions.end(), tests.begin(), tests.end());
    suggestions.insert(suggestions.end(), architectural.begin(), architectural.end());

    // analysis.potentialIssues → BugFix 建議（之前只存進 analysis 不用，
    // 等於掃了卻從不顯示）；命中行用各 pattern 第一次出現的行
    for (const auto& entry : AntiPatternTable()) {
        if (std::find(analysis.potentialIssues.begin(),
                      analysis.potentialIssues.end(),
                      entry.first) == analysis.potentialIssues.end()) {
            continue;
        }
        Suggestion suggestion;
        suggestion.type = SuggestionType::BugFix;
        suggestion.title = "Anti-pattern: " + entry.first;
        suggestion.description = entry.second.second;
        suggestion.code = "// " + entry.first + " removed";
        suggestion.reason = "Legacy C-style construct detected";
        suggestion.confidence = ConfidenceLevel::High;
        suggestion.lineNumber = FirstMatchLine(code, entry.second.first);
        suggestions.push_back(suggestion);
    }

    // Set file location and stable id for all suggestions
    for (auto& suggestion : suggestions) {
        suggestion.id = std::string(SuggestionTypeKey(suggestion.type)) +
                        "_" + std::to_string(nextSuggestionId++);
        suggestion.filePath = filePath;
        // 規則已給命中行號（>0）就保留，否則才蓋請求游標位置——
        // 否則 strcpy 在第 200 行建議卻跳到游標行
        if (suggestion.lineNumber <= 0) suggestion.lineNumber = lineNumber;
        if (suggestion.columnNumber <= 0) suggestion.columnNumber = columnNumber;
        suggestion.confidenceScore = CalculateConfidence(suggestion);
        // 學習權重 → enum：FilterByConfidence 吃的是 enum 而非 score，
        // 不映射的話 LearnFromFeedback 對過濾結果完全無效
        const float s = suggestion.confidenceScore;
        suggestion.confidence =
            s > 0.90f ? ConfidenceLevel::VeryHigh :
            s > 0.75f ? ConfidenceLevel::High :
            s >= 0.50f ? ConfidenceLevel::Medium : ConfidenceLevel::Low;
    }
    
    // Rank and filter
    suggestions = SuggestionRanker::RankByRelevance(suggestions, code);
    suggestions = SuggestionRanker::FilterByConfidence(suggestions, ConfidenceLevel::Medium);
    suggestions = SuggestionRanker::Deduplicate(suggestions);
    
    // Update statistics
    totalSuggestions += suggestions.size();
    
    return suggestions;
}

CodeAnalysis IntelligentSuggestionSystem::AnalyzeCode(const std::string& code, 
                                                       const std::string& filePath) {
    CodeAnalysis analysis;
    analysis.filePath = filePath;
    analysis.codeText = code;
    
    // 函式宣告/定義偵測：<型別...> <名>( ——型別不限內建四種，
    // auto/double/std::/自訂型別皆可，名稱允許 :: 限定。
    // 排除：行首控制/回傳關鍵字（return foo(、for (）、
    // 賦值呼叫點（`int r = foo(` 的 = 在 ( 之前）、註解行。
    static const std::regex funcDeclRegex(
        R"(^\s*(?:[\w:<>&*]+\s+)+([~\w:]+)\s*\()");
    static const std::regex nonDeclLineRegex(
        R"(^\s*(return|if|for|while|switch|catch|else|throw|new|delete|sizeof|co_await|co_return|co_yield)\b)");

    std::istringstream stream(code);
    std::string line;
    while (std::getline(stream, line)) {
        analysis.lineCount++;

        const size_t firstNonSpace = line.find_first_not_of(" \t");
        const bool commentLine = firstNonSpace != std::string::npos &&
            line.compare(firstNonSpace, 2, "//") == 0;

        if (!commentLine && !std::regex_search(line, nonDeclLineRegex)) {
            std::smatch m;
            if (std::regex_search(line, m, funcDeclRegex)) {
                const std::string beforeName = line.substr(0, static_cast<size_t>(m.position(1)));
                const char prev = m.position(1) > 0
                    ? line[static_cast<size_t>(m.position(1)) - 1] : '\0';
                // `x = foo(` 呼叫點（= 在 ( 前）、`a.b c.d(` / `p->f(` 成員呼叫
                if (beforeName.find('=') == std::string::npos &&
                    prev != '.' && prev != '>') {
                    analysis.functions.push_back(line);
                    analysis.functionLines.push_back(analysis.lineCount);
                }
            }
        }

        // Extract classes
        if (line.find("class ") != std::string::npos) {
            analysis.classes.push_back(line);
        }
        
        // Extract imports
        if (line.find("#include") != std::string::npos ||
            line.find("import ") != std::string::npos) {
            analysis.imports.push_back(line);
        }
    }
    
    // Calculate complexity
    analysis.complexityScore = CalculateCyclomaticComplexity(code);
    
    // Find potential issues
    analysis.potentialIssues = FindAntiPatterns(code);
    
    return analysis;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetContextualSuggestions(
    const std::string& context,
    const std::string& currentLine) {
    
    std::vector<Suggestion> suggestions;
    
    // Generate code completion based on context
    std::string completion = GenerateCompletion(currentLine);
    
    if (!completion.empty()) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::CodeCompletion;
        suggestion.title = "Code Completion";
        suggestion.description = "Suggested completion based on context";
        suggestion.code = completion;
        suggestion.reason = "Pattern matching and analysis";
        suggestion.confidence = ConfidenceLevel::High;
        suggestions.push_back(suggestion);
    }
    
    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetRefactoringSuggestions(
    const CodeAnalysis& analysis) {
    
    std::vector<Suggestion> suggestions;
    
    // Check for long functions（掃 codeText——之前誤傳 filePath，函式過長永遠偵測不到）
    auto longFunctions = FindLongFunctions(analysis.codeText, 50);
    for (const auto& func : longFunctions) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::Refactoring;
        suggestion.title = "Extract Method: " + func.first;
        suggestion.description = "Consider splitting this long function into smaller methods";
        suggestion.code = "// Extracted method logic";
        suggestion.reason = "Long functions are harder to maintain and test";
        suggestion.confidence = ConfidenceLevel::High;
        suggestion.lineNumber = func.second;
        suggestions.push_back(suggestion);
    }
    
    // Check for high complexity
    if (analysis.complexityScore > 10) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::Refactoring;
        suggestion.title = "Reduce Complexity";
        suggestion.description = "Consider simplifying this code or extracting sub-methods";
        suggestion.code = "// Simplified logic";
        suggestion.reason = "High cyclomatic complexity indicates complex control flow";
        suggestion.confidence = ConfidenceLevel::Medium;
        suggestions.push_back(suggestion);
    }
    
    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetOptimizationSuggestions(
    const CodeAnalysis& analysis) {

    std::vector<Suggestion> suggestions;
    const std::string& code = analysis.codeText;

    // by-value 傳遞 std:: 容器/字串參數 → const&（參數列表內出現 & 或 * 不算）
    static const std::regex byValueRegex(
        R"(\(\s*(?:const\s+)?std::(?:string|vector|map|unordered_map|set|unordered_set|array|deque)\b[^&*()]*\))");
    {
        std::sregex_iterator it(code.begin(), code.end(), byValueRegex), end;
        if (it != end) {
            const int hits = static_cast<int>(std::distance(it, end));
            Suggestion suggestion;
            suggestion.type = SuggestionType::Optimization;
            suggestion.title = "Pass by const reference";
            suggestion.description = "std:: container/string parameter passed by value copies";
            if (hits > 1) {
                suggestion.description += " (" + std::to_string(hits) + " occurrences)";
            }
            suggestion.code = "void Function(const std::string& name);";
            suggestion.reason = "Const& avoids the copy for read-only parameters";
            suggestion.confidence = ConfidenceLevel::High;
            suggestion.lineNumber = LineOfPosition(code, static_cast<size_t>(it->position()));
            suggestions.push_back(suggestion);
        }
    }

    // std::endl 強迫 flush——一般輸出 '\n' 即可，flush 只在需要時做
    static const std::regex endlRegex(R"(\bstd::endl\b)");
    if (std::regex_search(code, endlRegex)) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::Optimization;
        suggestion.title = "Prefer '\\n' over std::endl";
        suggestion.description = "std::endl flushes the stream on every call";
        suggestion.code = "stream << '\\n';";
        suggestion.reason = "Unnecessary flushes are expensive in loops/throughput paths";
        suggestion.confidence = ConfidenceLevel::Medium;
        suggestion.lineNumber = FirstMatchLine(code, endlRegex);
        suggestions.push_back(suggestion);
    }

    // 迴圈內 push_back → 提示 reserve（粗偵測：檔內同時有迴圈與 push_back）
    static const std::regex loopRegex(R"(\b(?:for|while)\s*\()");
    static const std::regex pushBackRegex(R"(\.\s*push_back\s*\()");
    if (std::regex_search(code, loopRegex) && std::regex_search(code, pushBackRegex)) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::Optimization;
        suggestion.title = "Consider reserve() before loop push_back";
        suggestion.description = "push_back inside a loop may reallocate repeatedly";
        suggestion.code = "vec.reserve(expected);";
        suggestion.reason = "Reserving capacity avoids repeated reallocation";
        suggestion.confidence = ConfidenceLevel::Medium;
        suggestion.lineNumber = FirstMatchLine(code, pushBackRegex);
        suggestions.push_back(suggestion);
    }

    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::DetectBugs(const std::string& code) {
    std::vector<Suggestion> suggestions;

    // Banned C API（與 CI unsafe-api-scan 同清單）——word boundary 防 sscanf
    // 之類前綴誤報；static 快取 regex 編譯結果
    static const std::vector<std::pair<std::regex, std::string>> bannedApis = {
        {std::regex("\\bgets\\s*\\("),    "gets() is unbounded — use fgets"},
        {std::regex("\\bstrcpy\\s*\\("),  "strcpy() can overflow — use strncpy or memcpy with bounds"},
        {std::regex("\\bstrcat\\s*\\("),  "strcat() can overflow — use strncat"},
        {std::regex("\\bsprintf\\s*\\("), "sprintf() can overflow — use snprintf"},
        {std::regex("\\bvsprintf\\s*\\("),"vsprintf() can overflow — use vsnprintf"},
        {std::regex("\\bscanf\\s*\\("),   "scanf() is unbounded — prefer stream input"},
    };
    for (const auto& entry : bannedApis) {
        std::sregex_iterator it(code.begin(), code.end(), entry.first), end;
        if (it == end) continue;
        const int hits = static_cast<int>(std::distance(it, end));
        Suggestion suggestion;
        suggestion.type = SuggestionType::BugFix;
        suggestion.title = "Unsafe C API";
        suggestion.description = entry.second;
        if (hits > 1) {
            suggestion.description += " (" + std::to_string(hits) + " occurrences)";
        }
        suggestion.code = "// Use the bounded alternative";
        suggestion.reason = "Matches CI unsafe-api-scan banned list";
        suggestion.confidence = ConfidenceLevel::VeryHigh;
        suggestion.lineNumber = LineOfPosition(code, static_cast<size_t>(it->position()));
        suggestions.push_back(suggestion);
    }

    // 其他常見 bug 模式
    static const std::vector<std::pair<std::regex, std::string>> bugPatterns = {
        {std::regex("==\\s*NULL"),        "Use nullptr instead of NULL in C++"},
        {std::regex("=\\s*new\\s+\\w"),   "Raw new — prefer std::make_unique/make_shared"},
        {std::regex("\\bwhile\\s*\\(\\s*true\\s*\\)"), "Infinite loop — confirm an exit condition exists"},
    };
    for (const auto& entry : bugPatterns) {
        std::sregex_iterator it(code.begin(), code.end(), entry.first), end;
        if (it == end) continue;
        const int hits = static_cast<int>(std::distance(it, end));
        Suggestion suggestion;
        suggestion.type = SuggestionType::BugFix;
        suggestion.title = "Potential Bug";
        suggestion.description = entry.second;
        if (hits > 1) {
            suggestion.description += " (" + std::to_string(hits) + " occurrences)";
        }
        suggestion.code = "// Fixed code";
        suggestion.reason = "Pattern-matched suspicious construct";
        suggestion.confidence = ConfidenceLevel::High;
        suggestion.lineNumber = LineOfPosition(code, static_cast<size_t>(it->position()));
        suggestions.push_back(suggestion);
    }

    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetBestPracticeRecommendations(
    const CodeAnalysis& analysis) {
    
    std::vector<Suggestion> suggestions;
    
    // Check for missing documentation
    if (analysis.functions.empty() && analysis.lineCount > 10) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::BestPractice;
        suggestion.title = "Add Documentation";
        suggestion.description = "Consider adding comments or documentation for this code";
        suggestion.code = "// TODO: Add documentation";
        suggestion.reason = "Well-documented code is easier to maintain";
        suggestion.confidence = ConfidenceLevel::Low;
        suggestions.push_back(suggestion);
    }
    
    // Check for magic numbers（掃描程式碼本文，不是檔案路徑）
    // static 快取編譯結果——std::regex 建構昂貴，每次分析重建會拖慢 render thread
    static const std::regex magicNumberRegex("\\b\\d{2,}\\b");
    if (std::regex_search(analysis.codeText, magicNumberRegex)) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::BestPractice;
        suggestion.title = "Use Named Constants";
        suggestion.description = "Replace magic numbers with named constants";
        suggestion.code = "const int MAX_SIZE = 100;";
        suggestion.reason = "Named constants improve code readability";
        suggestion.confidence = ConfidenceLevel::Medium;
        suggestion.lineNumber = FirstMatchLine(analysis.codeText, magicNumberRegex);
        suggestions.push_back(suggestion);
    }
    
    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetDocumentationSuggestions(
    const CodeAnalysis& analysis) {
    
    std::vector<Suggestion> suggestions;
    
    // Suggest adding documentation for functions（同 test 建議：
    // 帶函式名避免全被 dedup 成一筆）
    for (size_t i = 0; i < analysis.functions.size(); ++i) {
        const auto& func = analysis.functions[i];
        const std::string name = ExtractFunctionName(func);
        if (name.empty()) continue;
        if (func.find("//") == std::string::npos &&
            func.find("/*") == std::string::npos) {
            Suggestion suggestion;
            suggestion.type = SuggestionType::Documentation;
            suggestion.title = "Add Function Documentation: " + name;
            suggestion.description = "Add a comment documenting this function's purpose";
            suggestion.code = "/**\n * Function description\n * @param params Parameter description\n * @return Return value description\n */";
            suggestion.reason = "Documentation helps other developers understand the code";
            suggestion.confidence = ConfidenceLevel::Low;
            if (i < analysis.functionLines.size()) {
                suggestion.lineNumber = analysis.functionLines[i];
            }
            suggestions.push_back(suggestion);
        }
    }
    
    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GenerateTestSuggestions(
    const CodeAnalysis& analysis) {
    
    std::vector<Suggestion> suggestions;
    
    // Suggest tests for functions——用抽出的函式名做 title，
    // 讓 Deduplicate 不會把不同函式的測試建議全併成一筆；
    // 抽不到名（呼叫點誤判）的列直接略過
    for (size_t i = 0; i < analysis.functions.size(); ++i) {
        const auto& func = analysis.functions[i];
        const std::string name = ExtractFunctionName(func);
        if (name.empty()) continue;
        Suggestion suggestion;
        suggestion.type = SuggestionType::TestGeneration;
        suggestion.title = "Generate Unit Test: " + name;
        suggestion.description = "Generate a unit test for " + name;
        suggestion.code = "// TEST(" + name + ", Cases) { ... }";
        suggestion.reason = "Unit tests ensure code correctness";
        suggestion.confidence = ConfidenceLevel::Medium;
        if (i < analysis.functionLines.size()) {
            suggestion.lineNumber = analysis.functionLines[i];
        }
        suggestions.push_back(suggestion);
    }
    
    return suggestions;
}

std::vector<Suggestion> IntelligentSuggestionSystem::GetArchitecturalSuggestions(
    const CodeAnalysis& analysis) {
    
    std::vector<Suggestion> suggestions;
    
    // Check for single responsibility violations
    if (analysis.functions.size() > 5) {
        Suggestion suggestion;
        suggestion.type = SuggestionType::Architectural;
        suggestion.title = "Consider Splitting Class";
        suggestion.description = "This class has many functions - consider splitting by responsibility";
        suggestion.code = "// Separate classes for different responsibilities";
        suggestion.reason = "Single Responsibility Principle improves maintainability";
        suggestion.confidence = ConfidenceLevel::Medium;
        suggestions.push_back(suggestion);
    }
    
    return suggestions;
}

void IntelligentSuggestionSystem::LearnFromFeedback(const std::string& suggestionId, 
                                                      bool accepted) {
    // id 格式為 "<typeKey>_<counter>"，以最後一個底線切出類型鍵
    const size_t sep = suggestionId.rfind('_');
    const std::string type = (sep == std::string::npos)
        ? suggestionId : suggestionId.substr(0, sep);
    std::lock_guard<std::mutex> lk(sharedMutex_);
    auto it = suggestionWeights.find(type);

    if (accepted) {
        acceptedSuggestions++;
        acceptedSuggestionHistory.push_back(suggestionId);
        if (it != suggestionWeights.end()) {
            it->second = std::min(1.0f, it->second + 0.05f);
        }
    } else if (it != suggestionWeights.end()) {
        // 拒絕回饋：降低該類型權重，下限 0.1 避免永久沉底
        it->second = std::max(0.1f, it->second - 0.05f);
    }
}

float IntelligentSuggestionSystem::GetAcceptanceRate() const {
    if (totalSuggestions == 0) return 0.0f;
    return static_cast<float>(acceptedSuggestions) / static_cast<float>(totalSuggestions);
}

float IntelligentSuggestionSystem::CalculateConfidence(const Suggestion& suggestion) {
    // Base confidence from type weight（worker/UI 共享，需上鎖）
    const std::string typeStr = SuggestionTypeKey(suggestion.type);

    float baseConfidence = 0.5f;
    {
        std::lock_guard<std::mutex> lk(sharedMutex_);
        auto it = suggestionWeights.find(typeStr);
        if (it != suggestionWeights.end()) {
            baseConfidence = it->second;
        }
    }

    // 合併偵測器自評 enum——之前只用權重當 score，回推 enum 會把
    // banned API 的 VeryHigh（權重 0.9 → 0.9 不 >0.90）洗回 High。
    // 用乘性調整不用加性：調整量隨權重縮小，LearnFromFeedback 連續
    // 拒絕後權重壓制仍然生效（0.45×1.1=0.495 仍低於 Medium 門檻）
    float factor = 1.0f;
    switch (suggestion.confidence) {
        case ConfidenceLevel::VeryHigh: factor = 1.10f; break;
        case ConfidenceLevel::High:     factor = 1.05f; break;
        case ConfidenceLevel::Medium:   factor = 1.0f;  break;
        case ConfidenceLevel::Low:      factor = 0.85f; break;
    }
    return std::clamp(baseConfidence * factor, 0.05f, 1.0f);
}

std::string IntelligentSuggestionSystem::ExtractFunctionName(const std::string& line) {
    // 每行都會呼叫——static 快取避免逐行重建 regex
    static const std::regex funcRegex("\\b(\\w+)\\s*\\(");
    std::smatch match;
    if (std::regex_search(line, match, funcRegex)) {
        return match[1].str();
    }
    return "";
}

bool IntelligentSuggestionSystem::IsComplexCode(const std::string& code) {
    int nesting = CalculateNestingDepth(code);
    int complexity = CalculateCyclomaticComplexity(code);
    return nesting > 3 || complexity > 10;
}

std::vector<std::string> IntelligentSuggestionSystem::ExtractKeywords(const std::string& code) {
    std::vector<std::string> keywords;
    std::istringstream stream(code);
    std::string word;
    
    while (stream >> word) {
        if (word.length() > 3) {
            keywords.push_back(word);
        }
    }
    
    return keywords;
}

std::string IntelligentSuggestionSystem::GenerateCompletion(const std::string& prefix) {
    // Simple pattern-based completion
    // In a real implementation, this would use a language model
    
    if (prefix.find("int ") != std::string::npos) {
        return " variableName = 0;";
    } else if (prefix.find("std::string") != std::string::npos) {
        return " variableName;";
    } else if (prefix.find("if (") != std::string::npos) {
        return " {";
    } else if (prefix.find("void ") != std::string::npos) {
        return "() {";
    }
    
    return "";
}

bool IntelligentSuggestionSystem::MatchesPattern(const std::string& code, 
                                                  const std::string& pattern) {
    return code.find(pattern) != std::string::npos;
}

std::vector<std::string> IntelligentSuggestionSystem::FindAntiPatterns(const std::string& code) {
    std::vector<std::string> antiPatterns;

    // 共用 AntiPatternTable——word boundary 防 "freedom"/"printfx" 誤報；
    // scanf 由 bannedApis 以更準訊息覆蓋，此處不重複
    for (const auto& entry : AntiPatternTable()) {
        if (std::regex_search(code, entry.second.first)) {
            antiPatterns.push_back(entry.first);
        }
    }

    return antiPatterns;
}

int IntelligentSuggestionSystem::CalculateCyclomaticComplexity(const std::string& code) {
    // 關鍵字計數——之前用字元計數（'i'/'f'/'w'），識別字會灌水複雜度
    static const std::regex decisionRegex(
        "\\b(if|for|while|case|catch|else\\s+if)\\b");
    int complexity = 1; // Base complexity
    for (std::sregex_iterator it(code.begin(), code.end(), decisionRegex), end;
         it != end; ++it) {
        ++complexity;
    }
    complexity += static_cast<int>(std::count(code.begin(), code.end(), '?')); // ternary
    // && / || 各算一個決策點
    for (size_t pos = code.find("&&"); pos != std::string::npos;
         pos = code.find("&&", pos + 2)) {
        ++complexity;
    }
    for (size_t pos = code.find("||"); pos != std::string::npos;
         pos = code.find("||", pos + 2)) {
        ++complexity;
    }
    return complexity;
}

int IntelligentSuggestionSystem::CalculateNestingDepth(const std::string& code) {
    int maxDepth = 0;
    int currentDepth = 0;
    
    for (char c : code) {
        if (c == '{') {
            currentDepth++;
            maxDepth = std::max(maxDepth, currentDepth);
        } else if (c == '}') {
            currentDepth--;
        }
    }
    
    return maxDepth;
}

std::vector<std::pair<std::string, int>>
IntelligentSuggestionSystem::FindLongFunctions(const std::string& code,
                                               int maxLines) {
    std::vector<std::pair<std::string, int>> longFunctions;

    // 以頂層大括號深度切函式區塊：depth 0→1 的行記為函式頭（含簽名），
    // 回到 depth 0 時若區塊行數超過 maxLines 即回報。非完整 parser——
    // namespace/class 區塊也會被當成函式，但對「過長區塊」提示仍正確。
    std::istringstream stream(code);
    std::string line;
    int depth = 0;
    int blockLines = 0;
    int blockStartLine = 0;
    int lineNo = 0;
    std::string blockHeader;
    while (std::getline(stream, line)) {
        ++lineNo;
        const int depthBefore = depth;
        for (char c : line) {
            if (c == '{') ++depth;
            else if (c == '}') --depth;
        }
        if (depthBefore == 0 && depth > 0) {
            blockHeader = line;
            blockLines = 1;
            blockStartLine = lineNo;
        } else if (depth > 0) {
            ++blockLines;
        }
        if (depth == 0 && !blockHeader.empty()) {
            if (blockLines > maxLines) {
                std::string name = ExtractFunctionName(blockHeader);
                longFunctions.emplace_back(
                    name.empty() ? "unnamed block" : name, blockStartLine);
            }
            blockHeader.clear();
            blockLines = 0;
        }
    }

    return longFunctions;
}

// ============================================================================
// Real-time Suggestion Provider
// ============================================================================

RealTimeSuggestionProvider::RealTimeSuggestionProvider(IntelligentSuggestionSystem* system)
    : suggestionSystem(system)
    , debounceDelay(300) // 300ms default
    , lastSuggestionTime(std::chrono::steady_clock::now()) {
}

std::vector<Suggestion> RealTimeSuggestionProvider::OnTextChanged(
    const std::string& code,
    const std::string& filePath,
    int cursorLine,
    int cursorColumn) {
    
    // Debounce check
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - lastSuggestionTime).count();
    
    if (elapsed < debounceDelay) {
        return {}; // Wait for debounce
    }
    
    lastSuggestionTime = now;
    
    return suggestionSystem->GenerateSuggestions(code, filePath, cursorLine, cursorColumn);
}

std::vector<std::string> RealTimeSuggestionProvider::GetQuickCompletions(
    const std::string& prefix) {
    
    std::vector<std::string> completions;
    
    // Simple keyword-based completions
    std::vector<std::string> commonKeywords = {
        "std::", "void", "int", "float", "bool", "const", "class", "struct"
    };
    
    for (const auto& keyword : commonKeywords) {
        if (keyword.find(prefix) == 0) {
            completions.push_back(keyword);
        }
    }
    
    return completions;
}

void RealTimeSuggestionProvider::SetDebounceDelay(int milliseconds) {
    debounceDelay = milliseconds;
}

// ============================================================================
// Suggestion Ranker
// ============================================================================

std::vector<Suggestion> SuggestionRanker::RankByRelevance(
    const std::vector<Suggestion>& suggestions,
    const std::string& context) {
    
    std::vector<Suggestion> ranked = suggestions;
    
    // Sort by confidence score
    std::sort(ranked.begin(), ranked.end(),
              [](const Suggestion& a, const Suggestion& b) {
                  return a.confidenceScore > b.confidenceScore;
              });
    
    return ranked;
}

std::vector<Suggestion> SuggestionRanker::FilterByConfidence(
    const std::vector<Suggestion>& suggestions,
    ConfidenceLevel minConfidence) {
    
    std::vector<Suggestion> filtered;
    
    for (const auto& suggestion : suggestions) {
        if (suggestion.confidence >= minConfidence) {
            filtered.push_back(suggestion);
        }
    }
    
    return filtered;
}

std::vector<Suggestion> SuggestionRanker::FilterByType(
    const std::vector<Suggestion>& suggestions,
    SuggestionType type) {
    
    std::vector<Suggestion> filtered;
    
    for (const auto& suggestion : suggestions) {
        if (suggestion.type == type) {
            filtered.push_back(suggestion);
        }
    }
    
    return filtered;
}

std::vector<Suggestion> SuggestionRanker::Deduplicate(
    const std::vector<Suggestion>& suggestions) {
    
    std::vector<Suggestion> deduplicated;
    std::unordered_set<std::string> seen;
    
    for (const auto& suggestion : suggestions) {
        std::string key = suggestion.title + suggestion.code;
        if (seen.find(key) == seen.end()) {
            seen.insert(key);
            deduplicated.push_back(suggestion);
        }
    }
    
    return deduplicated;
}

std::vector<Suggestion> SuggestionRanker::SortByLocation(
    const std::vector<Suggestion>& suggestions) {
    
    std::vector<Suggestion> sorted = suggestions;
    
    std::sort(sorted.begin(), sorted.end(),
              [](const Suggestion& a, const Suggestion& b) {
                  if (a.filePath != b.filePath) {
                      return a.filePath < b.filePath;
                  }
                  if (a.lineNumber != b.lineNumber) {
                      return a.lineNumber < b.lineNumber;
                  }
                  return a.columnNumber < b.columnNumber;
              });
    
    return sorted;
}

} // namespace MingGoRTSIDE
