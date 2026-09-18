// TutorialScriptTest - Q-9 教學狀態機無頭驗證
// 覆蓋:正序步進、亂序/錯 entity 不推進、Reset 重看、提示文字非空

#include "Gameplay/TutorialScript.h"

#include <cstdio>
#include <cstring>

using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== Tutorial Script Tests ===\n\n");

    // ---- 正序:Observe → Probe → Expire → Done ----
    {
        TutorialScript t(/*A=*/0, /*B=*/1);
        Check(t.Step() == TutorialStep::Observe, "初始步驟 Observe");
        Check(t.StepIndex() == 0, "初始 StepIndex 0");
        Check(TutorialScript::StepCount() == 3, "StepCount 為 3");
        Check(t.TargetEntity() == 0, "步驟1 目標 entity 0");
        Check(std::strlen(t.Hint()) > 0, "步驟1 提示非空");
        Check(std::strlen(t.Nudge()) > 0, "步驟1 nudge 非空");

        Check(t.Advance(TutorialEvent::Observed, 0), "觀測目標推進");
        Check(t.Step() == TutorialStep::Probe, "進入 Probe 步驟");
        Check(t.StepIndex() == 1, "步驟2 StepIndex 1");
        Check(t.TargetEntity() == 1, "步驟2 目標 entity 1");
        Check(std::strlen(t.Hint()) > 0, "步驟2 提示非空");
        Check(std::strlen(t.Nudge()) > 0, "步驟2 nudge 非空");

        Check(t.Advance(TutorialEvent::Probed, 1), "探測目標推進");
        Check(t.Step() == TutorialStep::Expire, "進入 Expire 步驟");
        Check(t.StepIndex() == 2, "步驟3 StepIndex 2");
        Check(t.TargetEntity() == -1, "步驟3 無特定目標");
        Check(std::strlen(t.Hint()) > 0, "步驟3 提示非空");
        Check(std::strlen(t.Nudge()) > 0, "步驟3 nudge 非空");

        Check(t.Advance(TutorialEvent::Expired), "時效到期推進");
        Check(t.IsDone(), "教學完成");
        Check(t.StepIndex() == 3, "Done StepIndex 3");
        Check(t.TargetEntity() == -1, "Done 無特定目標");
        Check(std::strlen(t.Hint()) > 0, "完成提示非空");
        Check(std::strlen(t.Nudge()) == 0, "Done 無 nudge");

        // None 事件永遠是 no-op
        Check(!t.Advance(TutorialEvent::None, 0),
              "None 事件不推進(Done)");
        Check(t.IsDone(), "None 後仍 Done");
    }

    // ---- 亂序/錯誤操作不推進 ----
    {
        TutorialScript t(0, 1);
        Check(!t.Advance(TutorialEvent::Probed, 0),
              "步驟1 收到探測不推進");
        Check(!t.Advance(TutorialEvent::Observed, 1),
              "觀測錯的 entity 不推進");
        Check(!t.Advance(TutorialEvent::Expired),
              "步驟1 收到過期不推進");
        Check(!t.Advance(TutorialEvent::None, 0),
              "步驟1 None 事件不推進");
        Check(t.Step() == TutorialStep::Observe, "仍在步驟1");
        Check(std::strlen(t.Nudge()) > 0, "nudge 提示非空");

        // 步驟2 用強觀測只給 nudge——本步要教弱探測
        t.Advance(TutorialEvent::Observed, 0);
        Check(!t.Advance(TutorialEvent::Observed, 1),
              "步驟2 強觀測不推進(教 Probe)");
        Check(!t.Advance(TutorialEvent::Probed, 0),
              "步驟2 探測錯 entity 不推進");
        Check(t.Step() == TutorialStep::Probe, "仍在步驟2");

        // Done 後事件不再推進
        t.Advance(TutorialEvent::Probed, 1);
        t.Advance(TutorialEvent::Expired);
        Check(!t.Advance(TutorialEvent::Observed, 0),
              "Done 後事件忽略");
    }

    // ---- Reset 重看 ----
    {
        TutorialScript t(0, 1);
        t.Advance(TutorialEvent::Observed, 0);
        t.Advance(TutorialEvent::Probed, 1);
        t.Reset();
        Check(t.Step() == TutorialStep::Observe, "Reset 回步驟1");
        Check(!t.IsDone(), "Reset 後未完成");
        Check(t.Advance(TutorialEvent::Observed, 0), "重看可再走一遍");
    }

    // ---- 自訂 target id ----
    {
        TutorialScript t(5, 7);
        Check(t.TargetEntity() == 5, "自訂目標 A");
        Check(!t.Advance(TutorialEvent::Observed, 0),
              "非目標 id 不推進");
        Check(t.Advance(TutorialEvent::Observed, 5), "目標 id 推進");
        Check(t.TargetEntity() == 7, "自訂目標 B");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
