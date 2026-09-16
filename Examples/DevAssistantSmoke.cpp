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

    // 6) 空提示
    {
        auto r = dev.GenerateCode("   ", "C++");
        Check(!r.success, "empty prompt fails cleanly");
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

    std::printf("\n%s (%d failures)\n", failures == 0 ? "ALL PASS" : "FAILURES", failures);
    return failures == 0 ? 0 : 1;
}
