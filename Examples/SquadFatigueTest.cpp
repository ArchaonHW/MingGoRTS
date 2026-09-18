// SquadFatigueTest - G-3 疲勞系統無頭測試
// 驗收：stamina 0~1、移動消耗、駐守回復、疲憊移速懲罰生效
#include "Gameplay/Squad.h"
#include <cstdio>
#include <cmath>

using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== Squad Fatigue Tests (G-3) ===\n\n");

    // [1] 初值與範圍
    Squad s("test", 0, Potato::Vector2(0, 0), 10);
    Check(std::fabs(s.GetStamina() - 1.0f) < 1e-6f, "初始 stamina = 1.0");
    Check(!s.IsExhausted(), "初始非疲憊");
    Check(std::fabs(s.GetEffectiveSpeed() - s.GetSpeed()) < 1e-6f,
          "滿體力時 effective speed = base speed");

    // [2] 移動消耗
    s.IssueOrder(SquadOrder::MoveTo, Potato::Vector2(100, 0));
    for (int i = 0; i < 200; ++i) s.Update(0.1f, nullptr); // 20s 行軍
    float afterMarch = s.GetStamina();
    printf("  stamina after 20s march: %.3f\n", afterMarch);
    Check(afterMarch < 1.0f && afterMarch >= 0.0f, "行軍消耗 stamina");
    Check(afterMarch <= 1.0f - 0.03f * 20.0f + 1e-3f, "消耗速率 >= drainMove");

    // [3] 駐守回復
    s.SetStamina(0.5f);
    s.IssueOrder(SquadOrder::Hold, Potato::Vector2(0, 0));
    for (int i = 0; i < 100; ++i) s.Update(0.1f, nullptr); // 10s 駐守
    printf("  stamina after 10s hold: %.3f\n", s.GetStamina());
    Check(s.GetStamina() > 0.5f, "駐守回復 stamina");
    Check(s.GetStamina() <= 1.0f, "stamina 上限 1.0");

    // [4] 疲憊懲罰
    s.SetStamina(0.2f); // 低於預設閾值 0.3
    Check(s.IsExhausted(), "stamina < 閾值 → IsExhausted");
    Check(s.GetEffectiveSpeed() < s.GetSpeed(), "疲憊移速打折");
    Check(std::fabs(s.GetEffectiveSpeed() - s.GetSpeed() * 0.6f) < 1e-6f,
          "疲憊倍率 0.6 生效");

    // [5] 疲憊仍可行軍但更慢（實際位移驗證）
    Squad tired("tired", 0, Potato::Vector2(0, 0), 10);
    tired.SetStamina(0.1f);
    tired.IssueOrder(SquadOrder::MoveTo, Potato::Vector2(50, 0));
    Squad fresh("fresh", 0, Potato::Vector2(0, 0), 10);
    fresh.IssueOrder(SquadOrder::MoveTo, Potato::Vector2(50, 0));
    for (int i = 0; i < 50; ++i) { tired.Update(0.1f, nullptr); fresh.Update(0.1f, nullptr); }
    printf("  tired x=%.2f  fresh x=%.2f\n",
           tired.GetPosition().x, fresh.GetPosition().x);
    Check(tired.GetPosition().x < fresh.GetPosition().x, "疲憊隊伍推進較慢");
    Check(tired.GetPosition().x > 0.0f, "疲憊隊伍仍可移動");

    // [6] 交戰消耗（SetEngaged 模擬接戰）
    Squad combat("combat", 0, Potato::Vector2(0, 0), 10);
    combat.IssueOrder(SquadOrder::Hold, Potato::Vector2(0, 0));
    combat.SetEngaged(true);
    for (int i = 0; i < 100; ++i) combat.Update(0.1f, nullptr);
    printf("  stamina after 10s combat: %.3f\n", combat.GetStamina());
    Check(combat.GetStamina() < 1.0f, "交戰消耗 stamina（駐守不回復）");

    // [7] 可調參數
    Squad custom("custom", 0, Potato::Vector2(0, 0), 10);
    custom.SetStaminaParams(0.5f, 0.5f, 0.0f, 0.9f, 0.5f);
    custom.SetStamina(0.85f);
    Check(custom.IsExhausted(), "自訂閾值生效");
    Check(std::fabs(custom.GetEffectiveSpeed() - custom.GetSpeed() * 0.5f) < 1e-6f,
          "自訂疲憊倍率生效");

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail ? 1 : 0;
}
