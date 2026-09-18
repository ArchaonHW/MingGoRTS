/**
 * MingGoRTS IDE Intelligent Suggestion System
 * Provides intelligent code suggestions and recommendations
 */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

namespace Potato {
namespace AI {
    class TextEmbedding;
    class NLPPipeline;
    class NeuralNetwork;
}
}

namespace MingGoRTSIDE {

/**
 * Suggestion types
 */
enum class SuggestionType {
    CodeCompletion,      // Code completion suggestions
    Refactoring,         // Refactoring recommendations
    Optimization,        // Performance optimization
    BugFix,             // Bug detection and fixes
    BestPractice,        // Best practice recommendations
    Documentation,       // Documentation suggestions
    TestGeneration,      // Test generation
    Architectural        // Architectural suggestions
};

/**
 * Confidence level
 */
enum class ConfidenceLevel {
    Low,      // < 50%
    Medium,   // 50-75%
    High,     // 75-90%
    VeryHigh  // > 90%
};

/**
 * Suggestion
 */
struct Suggestion {
    std::string id;
    SuggestionType type;
    std::string title;
    std::string description;
    std::string code;
    std::string reason;
    ConfidenceLevel confidence;
    float confidenceScore;
    std::string filePath;
    int lineNumber;
    int columnNumber;
    
    Suggestion()
        : type(SuggestionType::CodeCompletion)
        , confidence(ConfidenceLevel::Medium)
        , confidenceScore(0.5f)
        , lineNumber(0)
        , columnNumber(0) {}
};

/**
 * Code analysis result
 */
struct CodeAnalysis {
    std::string filePath;
    std::string codeText;  // 保留原文供後續規則掃描（如 magic number 檢查）
    std::vector<std::string> functions;
    std::vector<int> functionLines;  // 與 functions 平行的 1-based 行號
    std::vector<std::string> classes;
    std::vector<std::string> variables;
    std::vector<std::string> imports;
    int lineCount;
    int complexityScore;
    std::vector<std::string> potentialIssues;
    
    CodeAnalysis()
        : lineCount(0)
        , complexityScore(0) {}
};

/**
 * Intelligent Suggestion System
 */
class IntelligentSuggestionSystem {
public:
    IntelligentSuggestionSystem();
    ~IntelligentSuggestionSystem();
    
    // Initialization
    bool Initialize();
    void Shutdown();
    
    // Main suggestion generation（同步入口——測試與工具仍用；UI 請走非同步）
    std::vector<Suggestion> GenerateSuggestions(const std::string& code,
                                                const std::string& filePath,
                                                int lineNumber,
                                                int columnNumber);

    // 非同步分析：投遞快照到背景 worker，回傳 job id。
    // 單槽 supersede——新 job 覆寫未執行的舊 job，執行中 job 完成時
    // 若已有更新 job 排入則結果直接丟棄（stale），不積壓不追舊。
    uint64_t SubmitAnalysis(const std::string& code,
                            const std::string& filePath,
                            int lineNumber,
                            int columnNumber);

    // UI 執行緒每幀輪詢：有完成結果則搬出並回 true。
    // 呼叫端比對 doneJobId 是否為自己最後投遞的 id，過期結果自行丟棄。
    bool PollResult(uint64_t& doneJobId, std::vector<Suggestion>& out);
    
    // Code analysis
    CodeAnalysis AnalyzeCode(const std::string& code, const std::string& filePath);
    
    // Context-aware suggestions
    std::vector<Suggestion> GetContextualSuggestions(const std::string& context,
                                                      const std::string& currentLine);
    
    // Refactoring suggestions
    std::vector<Suggestion> GetRefactoringSuggestions(const CodeAnalysis& analysis);
    
    // Optimization suggestions
    std::vector<Suggestion> GetOptimizationSuggestions(const CodeAnalysis& analysis);
    
    // Bug detection
    std::vector<Suggestion> DetectBugs(const std::string& code);
    
    // Best practice recommendations
    std::vector<Suggestion> GetBestPracticeRecommendations(const CodeAnalysis& analysis);
    
    // Documentation suggestions
    std::vector<Suggestion> GetDocumentationSuggestions(const CodeAnalysis& analysis);
    
    // Test generation
    std::vector<Suggestion> GenerateTestSuggestions(const CodeAnalysis& analysis);
    
    // Architectural suggestions
    std::vector<Suggestion> GetArchitecturalSuggestions(const CodeAnalysis& analysis);
    
    // Learning from user feedback
    void LearnFromFeedback(const std::string& suggestionId, bool accepted);
    
    // Statistics
    size_t GetTotalSuggestions() const { return totalSuggestions; }
    size_t GetAcceptedSuggestions() const { return acceptedSuggestions; }
    float GetAcceptanceRate() const;
    
private:
    // AI modules
    Potato::AI::TextEmbedding* textEmbedding;
    Potato::AI::NLPPipeline* nlpPipeline;
    Potato::AI::NeuralNetwork* suggestionModel;
    
    // Learning data
    std::unordered_map<std::string, float> suggestionWeights;
    std::vector<std::string> acceptedSuggestionHistory;
    
    // Statistics（atomic：worker 與 UI 執行緒都會碰）
    std::atomic<size_t> totalSuggestions;
    std::atomic<size_t> acceptedSuggestions;
    std::atomic<size_t> nextSuggestionId;  // 遞增序號，供 LearnFromFeedback 回推類型

    // ---- 非同步 worker（SubmitAnalysis/PollResult）----
    struct AnalysisJob {
        uint64_t id = 0;
        std::string code;
        std::string filePath;
        int line = 0;
        int col = 0;
    };
    std::thread worker_;
    std::mutex queueMutex_;
    std::condition_variable queueCv_;
    AnalysisJob pendingJob_;
    bool hasPendingJob_ = false;
    bool workerStop_ = false;
    std::mutex resultMutex_;
    uint64_t doneJobId_ = 0;
    std::vector<Suggestion> doneSuggestions_;
    std::atomic<uint64_t> nextJobId_{0};
    // suggestionWeights / acceptedSuggestionHistory 由 worker 讀、UI 寫
    std::mutex sharedMutex_;

    void WorkerMain();
    
    // Helper methods
    float CalculateConfidence(const Suggestion& suggestion);
    std::string ExtractFunctionName(const std::string& line);
    bool IsComplexCode(const std::string& code);
    std::vector<std::string> ExtractKeywords(const std::string& code);
    std::string GenerateCompletion(const std::string& prefix);
    
    // Pattern matching
    bool MatchesPattern(const std::string& code, const std::string& pattern);
    std::vector<std::string> FindAntiPatterns(const std::string& code);
    
    // Code metrics
    int CalculateCyclomaticComplexity(const std::string& code);
    int CalculateNestingDepth(const std::string& code);
    // 回傳（函式名, 起始行號 1-based）——建議要指到區塊開頭不是游標行
    std::vector<std::pair<std::string, int>> FindLongFunctions(const std::string& code,
                                                               int maxLines = 50);
};

/**
 * Real-time suggestion provider
 */
class RealTimeSuggestionProvider {
public:
    RealTimeSuggestionProvider(IntelligentSuggestionSystem* system);
    
    // Trigger suggestions on typing
    std::vector<Suggestion> OnTextChanged(const std::string& code,
                                          const std::string& filePath,
                                          int cursorLine,
                                          int cursorColumn);
    
    // Get quick suggestions (for inline display)
    std::vector<std::string> GetQuickCompletions(const std::string& prefix);
    
    // Debounce handling
    void SetDebounceDelay(int milliseconds);
    
private:
    IntelligentSuggestionSystem* suggestionSystem;
    int debounceDelay;
    std::chrono::steady_clock::time_point lastSuggestionTime;
};

/**
 * Suggestion ranking and filtering
 */
class SuggestionRanker {
public:
    // Rank suggestions by relevance
    static std::vector<Suggestion> RankByRelevance(const std::vector<Suggestion>& suggestions,
                                                   const std::string& context);
    
    // Filter by confidence
    static std::vector<Suggestion> FilterByConfidence(const std::vector<Suggestion>& suggestions,
                                                     ConfidenceLevel minConfidence);
    
    // Filter by type
    static std::vector<Suggestion> FilterByType(const std::vector<Suggestion>& suggestions,
                                                SuggestionType type);
    
    // Deduplicate suggestions
    static std::vector<Suggestion> Deduplicate(const std::vector<Suggestion>& suggestions);
    
    // Sort by file location
    static std::vector<Suggestion> SortByLocation(const std::vector<Suggestion>& suggestions);
};

} // namespace MingGoRTSIDE
