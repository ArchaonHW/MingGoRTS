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
#include <cstdio>
#include <cstring>
#include <string>

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

    std::printf("\n%s (%d failures)\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
