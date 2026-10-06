// AI Agent 介面無頭煙霧測試
// 驗證：初始化建立預設 agent 編制、協作已啟用、ProcessRequest 走真實
// 分派路徑（非 "AI response to:" 佔位）、assistant hook 注入生效、
// 空請求安全、Shutdown 乾淨。
#include "MingGoRTS_IDE/IDECore.h"
#include "MingGoRTS_IDE/AIIntegration.h"

#include <iostream>
#include <string>

using namespace MingGoRTSIDE;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool cond, const char* name) {
    if (cond) {
        ++g_pass;
        std::cout << "  [PASS] " << name << "\n";
    } else {
        ++g_fail;
        std::cout << "  [FAIL] " << name << "\n";
    }
}

int main() {
    std::cout << "=== AIAgentSmoke ===\n";

    AIAgentInterface iface;

    Check(iface.Initialize(), "Initialize succeeds");
    Check(iface.IsAvailable(), "IsAvailable after init");
    Check(iface.GetActiveAgentCount() >= 4,
          "default agent roster created (>=4)");
    Check(iface.GetAgentManager() != nullptr, "agent manager exists");
    Check(iface.GetContextManager() != nullptr, "context manager exists");
    Check(iface.GetAgentManager()->IsCollaborationEnabled(),
          "collaboration enabled");

    // [1] 佔位符號已移除：回應不再只是回聲
    std::string r1 = iface.ProcessRequest("generate a squad movement function");
    Check(!r1.empty(), "code-gen request returns non-empty");
    Check(r1.find("AI response to:") == std::string::npos,
          "response is not the old placeholder");
    Check(r1.find("[CodeAssistant]") != std::string::npos,
          "code intent routed to CodeAssistant");

    // [2] 意圖分派
    std::string r2 = iface.ProcessRequest("design a level with a bridge");
    Check(r2.find("[Designer]") != std::string::npos,
          "level intent routed to Designer");

    std::string r3 = iface.ProcessRequest("build is failing to compile");
    Check(r3.find("[BuildKeeper]") != std::string::npos,
          "build intent routed to BuildKeeper");

    // [3] 空請求安全（不崩潰、回傳字串）
    std::string r4 = iface.ProcessRequest("");
    Check(true, "empty request does not crash");
    (void)r4;

    // [4] assistant hook 注入：GUI 層後端優先於規則式 agent，且帶意圖標籤
    std::string lastHookInput;
    iface.SetAssistantHook([&lastHookInput](const std::string& q) -> std::string {
        lastHookInput = q;
        return "HOOKED:" + q;
    });
    std::string r5 = iface.ProcessRequest("how do I add a doctrine rule");
    Check(r5.find("HOOKED:") != std::string::npos,
          "assistant hook intercepts request");
    Check(lastHookInput.rfind("chat: ", 0) == 0,
          "general question tagged 'chat:'");

    iface.ProcessRequest("design a level");
    Check(lastHookInput.rfind("design: ", 0) == 0,
          "level intent tagged 'design:'");

    iface.ProcessRequest("build keeps failing");
    Check(lastHookInput.rfind("build: ", 0) == 0,
          "build intent tagged 'build:'");

    // 直接呼叫 manager 方法同樣走 backend（SetAssistantHook 已轉發）
    std::string direct = iface.GetAgentManager()->DesignLevel(
        "arena map", SimplifiedAI::Context{});
    Check(direct.find("HOOKED:design: ") != std::string::npos,
          "direct manager call routed through backend");

    // hook 回空 → 退回 agent
    iface.SetAssistantHook([](const std::string&) -> std::string {
        return std::string();
    });
    std::string r6 = iface.ProcessRequest("hello");
    Check(!r6.empty() && r6.find("HOOKED:") == std::string::npos,
          "empty hook result falls back to agent");

    iface.Shutdown();
    Check(true, "Shutdown clean");

    std::cout << "=== " << g_pass << " PASS, " << g_fail << " FAIL ===\n";
    return g_fail == 0 ? 0 : 1;
}
