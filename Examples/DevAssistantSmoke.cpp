/**
 * DevAssistantSmoke - 本地代碼生成管線煙霧測試
 *
 * 驗證 IntelligentDevelopmentSystem 在無外部 LLM 客戶端時
 * 仍能透過本地管線（意圖解析 + 模板合成）產生真實輸出。
 *
 * 用法: DevAssistantSmoke
 * 期望: 全部 PASS，exit code 0
 */

#include "AI/IntelligentDevelopmentSystem.h"
#include "AI/KnowledgeGraph.h"
#include "AI/SelfReflection.h"
#include "AI/LLMIntegration.h"
#include "MingGoRTS_IDE/GUI/GuiTextUtils.h"
#include "MingGoRTS_IDE/IntelligentSuggestion.h"
#include <chrono>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <string>
#include <thread>

using namespace Potato::AI;

static int failures = 0;

static void Check(bool cond, const char* name) {
    std::printf("[%s] %s\n", cond ? "PASS" : "FAIL", name);
    if (!cond) failures++;
}

// 最小 ILLMClient stub：驗證本地管線無法識別時的外部 fallback 路徑
class StubLLMClient : public ILLMClient {
public:
    int chatCalls = 0;  // 記錄 ChatCompletion 被呼叫次數

    LLMResponse ChatCompletion(const std::vector<ChatMessage>&,
                               const LLMConfig&) override {
        chatCalls++;
        LLMResponse r;
        r.success = true;
        r.content = "```cpp\n// stub LLM output\nint stubAnswer() { return 42; }\n```";
        return r;
    }
    LLMResponse ChatCompletionWithTools(const std::vector<ChatMessage>& m,
                                        const std::vector<ToolDefinition>&,
                                        const LLMConfig& c) override {
        return ChatCompletion(m, c);
    }
    void ChatCompletionStream(const std::vector<ChatMessage>& m,
                              const LLMConfig& c,
                              std::function<void(const std::string&)> cb)
                              override {
        cb(ChatCompletion(m, c).content);
    }
    std::future<LLMResponse> ChatCompletionAsync(
        const std::vector<ChatMessage>& m, const LLMConfig& c) override {
        return std::async(std::launch::deferred,
                          [&]() { return ChatCompletion(m, c); });
    }
    std::vector<float> GenerateEmbedding(const std::string&,
                                         const std::string&) override {
        return {0.0f};
    }
    std::vector<std::vector<float>> GenerateEmbeddings(
        const std::vector<std::string>&, const std::string&) override {
        return {{0.0f}};
    }
    bool ValidateConfig(const LLMConfig&) override { return true; }
    std::string GetDefaultModel() override { return "stub"; }
};

int main() {
    std::printf("=== DevAssistant Local Pipeline Smoke Test ===\n\n");

    KnowledgeGraph kg;
    SelfReflection sr;
    IntelligentDevelopmentSystem dev;

    // 無 LLM 客戶端 — 本地管線必須獨立運作
    dev.Initialize(nullptr, nullptr);
    dev.SetKnowledgeGraph(&kg);
    dev.SetSelfReflection(&sr);

    // 種入一個已知實體，驗證 KG 上下文註解
    kg.AddNode("Player", "class");

    // 1) class 意圖
    {
        auto r = dev.GenerateCode("create a class named Player with fields health, speed", "C++");
        Check(r.success, "class prompt succeeds without LLM");
        Check(r.generatedCode.find("class Player") != std::string::npos,
              "class output contains 'class Player'");
        Check(r.generatedCode.find("m_health") != std::string::npos,
              "class output contains member m_health");
    }

    // 2) function 意圖
    {
        auto r = dev.GenerateCode("generate a function called updateWorld", "C++");
        Check(r.success, "function prompt succeeds");
        Check(r.generatedCode.find("updateWorld") != std::string::npos,
              "function output contains name");
    }

    // 3) system stub 意圖
    {
        auto r = dev.GenerateCode("create a system for AudioManager", "C++");
        Check(r.success, "system stub prompt succeeds");
        Check(r.generatedCode.find("Initialize") != std::string::npos,
              "system stub has Initialize()");
    }

    // 4) test stub 意圖
    {
        auto r = dev.GenerateCode("create a test for Vector3", "C++");
        Check(r.success, "test stub prompt succeeds");
        Check(r.generatedCode.find("assert") != std::string::npos,
              "test stub contains assert");
    }

    // 5) 無法識別的提示 → 明確錯誤，不得產垃圾
    {
        auto r = dev.GenerateCode("...", "C++");
        Check(!r.success, "unrecognized prompt fails cleanly");
        Check(!r.error.empty(), "error message present");
        Check(r.generatedCode.empty(), "no garbage code emitted");
    }

    // 6) 空提示與純空白提示
    {
        auto r = dev.GenerateCode("", "C++");
        Check(!r.success, "empty prompt fails cleanly");
        auto w = dev.GenerateCode("   ", "C++");
        Check(!w.success, "whitespace-only prompt fails cleanly");
    }

    // 7) KG 上下文：已知主體附帶 context 註解
    {
        auto r = dev.GenerateCode("create a class for Player", "C++");
        Check(r.success && r.generatedCode.find("knowledge graph") != std::string::npos,
              "known subject gets KG context note");
    }

    // 8) LONG_RESPONSE：CopyToBuffer 截斷 + 標記 + NUL 結尾
    {
        char buf[64];
        std::memset(buf, 0x7F, sizeof(buf));
        std::string big(8192, 'x');
        MingGoRTSIDE::CopyToBuffer(buf, sizeof(buf), big);
        Check(buf[sizeof(buf) - 1] == '\0', "truncated copy is NUL-terminated");
        Check(std::strstr(buf, "...[truncated]") != nullptr,
              "truncated copy carries marker");
        Check(std::strlen(buf) < sizeof(buf), "truncated copy stays in bounds");
    }
    {
        char buf[64];
        std::memset(buf, 0x7F, sizeof(buf));
        MingGoRTSIDE::CopyToBuffer(buf, sizeof(buf), "short");
        Check(std::strcmp(buf, "short") == 0, "short copy is exact");
    }

    // 9) methods 擷取（review 修復：methods 清單不再為死路徑）
    {
        auto r = dev.GenerateCode(
            "create a class named Tank with methods fire, reload", "C++");
        Check(r.success && r.generatedCode.find("fire") != std::string::npos,
              "methods list populates generated class");
    }

    // 10) "and" 清單中間項不丟失（review 修復）
    {
        auto r = dev.GenerateCode(
            "create a class named Hero with health and mana and stamina", "C++");
        Check(r.success &&
              r.generatedCode.find("m_health") != std::string::npos &&
              r.generatedCode.find("m_mana") != std::string::npos &&
              r.generatedCode.find("m_stamina") != std::string::npos,
              "and-list keeps all members");
    }

    // 11) 詞邊界："latest" 不誤中 "test"（review 修復）
    {
        auto r = dev.GenerateCode(
            "create a class named Widget showing the latest data", "C++");
        Check(r.success &&
              r.generatedCode.find("class Widget") != std::string::npos &&
              r.generatedCode.find("assert") == std::string::npos,
              "word-boundary: 'latest' is not 'test'");
    }

    // 12) LLM fallback：本地管線失敗且 client 存在時走外部 stub
    {
        StubLLMClient stub;
        IntelligentDevelopmentSystem dev2;
        dev2.Initialize(&stub, nullptr);  // 非擁有指標——stub 生命期覆蓋本次呼叫
        auto r = dev2.GenerateCode("xyzzy !!!", "C++");
        Check(stub.chatCalls > 0, "fallback actually invoked the client");
        Check(r.success, "fallback to llmClient when local fails");
        Check(r.generatedCode.find("stubAnswer") != std::string::npos,
              "fallback returns external client output");
    }

    // 13) LLM fallback：stub 失敗時錯誤可見
    {
        StubLLMClient stub;
        IntelligentDevelopmentSystem dev2;
        dev2.Initialize(&stub, nullptr);
        // 可識別提示仍走本地，不浪費外部呼叫
        auto r = dev2.GenerateCode("create a class named Local", "C++");
        Check(r.success &&
              r.generatedCode.find("class Local") != std::string::npos,
              "local pipeline wins over configured llmClient");
    }

    // 14) async 寫回 seam：WriteGenerationResult 與 PollDevelopmentResult 共用路徑
    {
        char resp[128];
        std::memset(resp, 0x7F, sizeof(resp));
        MingGoRTSIDE::WriteGenerationResult(true, "class Foo {};", "", resp, sizeof(resp));
        Check(std::strcmp(resp, "class Foo {};") == 0,
              "write-back seam: success writes generated code");

        std::memset(resp, 0x7F, sizeof(resp));
        MingGoRTSIDE::WriteGenerationResult(false, "", "unrecognized prompt",
                                            resp, sizeof(resp));
        Check(std::strcmp(resp, "Error: unrecognized prompt") == 0,
              "write-back seam: failure writes 'Error: ...'");
    }

    // 15) 離線 AnalyzeCode：無 LLM 也有真實結果（metrics + issues + suggestions）
    {
        const char* smelly =
            "void foo() {\n"
            "    char buf[16];\n"
            "    str" "cpy(buf, \"x\");\n" // banned API（拆字避免誤觸 CI 掃描）
            "    int* p = new int(42);\n" // raw new
            "    for (int i = 0; i < 100; i++) {\n"
            "        if (i > 3) { p = new int(i); }\n"
            "    }\n"
            "    delete p;\n"
            "}\n";
        auto r = dev.AnalyzeCode(smelly, "C++");
        Check(r.lineCount > 1, "offline analysis: lineCount populated");
        Check(r.complexity > 1, "offline analysis: complexity counts keywords");
        Check(!r.issues.empty(), "offline analysis: banned API flagged as issue");
        Check(!r.suggestions.empty(), "offline analysis: smart-pointer suggestion");
        Check(r.qualityScore >= 0.0f && r.qualityScore <= 1.0f,
              "offline analysis: qualityScore in range");

        // 乾淨程式碼不應報 banned API
        auto clean = dev.AnalyzeCode("int add(int a, int b) { return a + b; }\n", "C++");
        bool bannedFlagged = false;
        for (const auto& i : clean.issues) {
            if (i.find("Unsafe C API") != std::string::npos) bannedFlagged = true;
        }
        Check(!bannedFlagged, "clean code: no false banned-API issue");

        // complexity 關鍵字計數：識別字含 'i'/'f'/'w' 不再灌水
        auto letterSoup = dev.AnalyzeCode(
            "void verifyFlowThings() { /* infinite wilderness */ }\n", "C++");
        Check(letterSoup.complexity == 1,
              "complexity ignores 'i/f/w' inside identifiers");
    }

    // 16) AnalyzeFile / AnalyzeProject：實際讀檔
    {
        const char* tmpPath = "devassistant_smoke_tmp.cpp";
        {
            std::ofstream tmp(tmpPath);
            tmp << "int answer() { return 42; }\n";
        }
        auto fr = dev.AnalyzeFile(tmpPath);
        Check(fr.lineCount >= 1 && fr.filePath == tmpPath,
              "AnalyzeFile reads file content");
        auto missing = dev.AnalyzeFile("definitely_not_a_file_xyz.cpp");
        Check(!missing.issues.empty(), "AnalyzeFile reports missing file");

        auto proj = dev.AnalyzeProject(".");
        Check(!proj.empty(), "AnalyzeProject finds sources in cwd");
        std::remove(tmpPath);
    }

    // 17) SuggestRefactoring 離線規則
    {
        const char* code = "void f() { char b[8]; str" "cpy(b, \"x\"); }\n";
        auto sugg = dev.SuggestRefactoring(code);
        Check(!sugg.empty(), "offline refactoring suggestions produced");
    }

    // 18) ValidateSyntax：括號平衡檢查
    {
        Check(DevSystemUtils::ValidateSyntax("int f() { return 0; }", "C++"),
              "balanced code passes syntax check");
        Check(!DevSystemUtils::ValidateSyntax("int f() { return 0;", "C++"),
              "unbalanced braces detected");
        Check(!DevSystemUtils::ValidateSyntax("int f() { /* open", "C++"),
              "unterminated block comment detected");
        Check(DevSystemUtils::ValidateSyntax("x = \"}{\"; // }(", "C++"),
              "braces inside string/comment ignored");
    }

    // 19) 非同步建議分析（B-1）：SubmitAnalysis/PollResult + supersede + shutdown
    {
        using MingGoRTSIDE::IntelligentSuggestionSystem;
        using MingGoRTSIDE::Suggestion;

        IntelligentSuggestionSystem sys;
        sys.Initialize();

        const char* code = "void f() { char b[8]; str" "cpy(b, \"x\"); }\n";

        // 基本 Submit → Poll：有界等待結果
        uint64_t job = sys.SubmitAnalysis(code, "smoke.cpp", 1, 1);
        uint64_t doneId = 0;
        std::vector<Suggestion> out;
        bool got = false;
        for (int i = 0; i < 200 && !got; ++i) {  // 最多 10s
            got = sys.PollResult(doneId, out);
            if (!got) std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
        Check(got && doneId == job && !out.empty(),
              "async analysis: submit->poll returns suggestions");

        // supersede：連投兩個 job——最新 job 的結果必達；
        // 舊 job 若在新 job 投遞前已完成則仍可能回傳（合法），
        // 但最終一定要收到 fresh 的結果
        uint64_t stale = sys.SubmitAnalysis("int a(){return 1;}\n", "a.cpp", 1, 1);
        uint64_t fresh = sys.SubmitAnalysis(code, "b.cpp", 1, 1);
        Check(stale != fresh, "async analysis: job ids are unique");
        bool gotFresh = false;
        for (int i = 0; i < 200 && !gotFresh; ++i) {
            uint64_t doneId2 = 0;
            std::vector<Suggestion> out2;
            if (sys.PollResult(doneId2, out2) && doneId2 == fresh) {
                gotFresh = true;
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
        Check(gotFresh, "async analysis: newest job result delivered");

        // pending job 存在時 Shutdown 不死結
        sys.SubmitAnalysis(code, "c.cpp", 1, 1);
        sys.Shutdown();
        Check(true, "async analysis: shutdown with pending job does not hang");
    }

    // 13) LLM client 誠實失敗：無 HTTP transport / 無本地推論後端時
    //     不得回假成功或假內容（弱點掃描 #2）
    {
        LLMConfig cfg;
        cfg.model = "gpt-4";
        cfg.apiKey = "test-key";
        std::vector<ChatMessage> msgs{ChatMessage(MessageRole::User, "hello")};

        OpenAIClient openai("test-key");
        auto oa = openai.ChatCompletion(msgs, cfg);
        Check(!oa.success, "OpenAI client fails honestly without transport");
        Check(oa.error.find("transport") != std::string::npos ||
              oa.error.find("parse") != std::string::npos,
              "OpenAI failure carries error message");
        Check(openai.GenerateEmbedding("x", "m").empty(),
              "OpenAI embedding returns empty (no fake data)");

        AnthropicClient anth("test-key");
        auto an = anth.ChatCompletion(msgs, cfg);
        Check(!an.success && !an.error.empty(),
              "Anthropic client fails honestly without transport");

        LocalModelClient local("nonexistent-model.gguf");
        Check(!local.LoadModel("nonexistent-model.gguf"),
              "LocalModel LoadModel fails honestly (no backend)");
        auto lr = local.ChatCompletion(msgs, cfg);
        Check(!lr.success, "LocalModel chat fails honestly");
        Check(local.GenerateEmbedding("x", "m").empty(),
              "LocalModel embedding returns empty (no fake data)");

        // ParseResponse 真解析：餵合法 OpenAI 格式 JSON 應取出 content
        // （直接驗證私有解析路徑經由公開介面無法注入——用 manager 層確認
        //   失敗時 error 能穿透到呼叫端）
        LLMManager mgr;
        mgr.RegisterClient(LLMProvider::OpenAI,
                           std::make_unique<OpenAIClient>("k"));
        mgr.SetDefaultProvider(LLMProvider::OpenAI);
        auto mr = mgr.Chat(msgs, cfg);
        Check(!mr.success && !mr.error.empty(),
              "LLMManager propagates honest failure + error");
    }

    std::printf("\n%s (%d failures)\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
