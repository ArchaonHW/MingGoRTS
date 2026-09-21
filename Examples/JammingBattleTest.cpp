// JammingBattleTest - P-3 擁擠阻塞玩法接線的 headless 驗證
//
// 覆蓋（UNSOLVED_PHYSICS_TO_ENGINE jamming 備選落地）：
//   BattleController::SetJamming — 擁擠群體 crowdFactor 下降、
//   孤隊不受影響、同方向移動的擁擠群真的比孤隊慢、
//   關閉時倍率復位為 1（無回歸）

#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include "MathUtils/JammingModel.h"

#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { ++g_pass; printf("  [PASS] %s\n", name); }
    else    { ++g_fail; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== JammingBattle Test (P-3) ===\n\n");

    Quasi::JammingParams jp; // crit=0.35 jam=0.85 minF=0.05 unitArea=0.15
    // 4 隊 ×20 人於 R=2 圓盤：密度 = 80·0.15/(π·4) ≈ 0.95 ≥ jam

    // [1] 擁擠群 crowdFactor 降到阻塞殘速、孤隊維持自由流
    {
        BattleController battle(24, 16, 1.0f);
        Squad* lone = battle.CreateSquad("孤軍", 0, Vector2(4, 4), 20);
        Squad* packA = battle.CreateSquad("擠A", 0, Vector2(14, 4), 20);
        battle.CreateSquad("擠B", 0, Vector2(14.4f, 4), 20);
        battle.CreateSquad("擠C", 0, Vector2(14, 4.4f), 20);
        battle.CreateSquad("擠D", 0, Vector2(14.4f, 4.4f), 20);
        // 遠方敵隊壓陣，避免 CheckOutcome 單方存活提早結束
        battle.CreateSquad("敵", 1, Vector2(12, 14), 20);

        battle.SetJamming(2.0f, jp);
        Check(battle.IsJammingEnabled(), "SetJamming 啟用");
        Check(battle.BeginExecution(), "開戰");
        battle.Update(0.1f);

        printf("    lone=%.3f pack=%.3f\n", lone->GetCrowdFactor(),
               packA->GetCrowdFactor());
        Check(lone->GetCrowdFactor() == 1.0f, "孤隊 crowdFactor=1 自由流");
        Check(packA->GetCrowdFactor() <= jp.minSpeedFactor + 1e-5f,
              "擁擠群 crowdFactor 降至阻塞殘速");
        Check(packA->GetEffectiveSpeed() < lone->GetEffectiveSpeed(),
              "擁擠群有效移速低於孤隊");
    }

    // [2] 同方向移動：擁擠群保持互擠 → 位移顯著小於孤隊
    {
        BattleController battle(24, 16, 1.0f);
        Squad* lone = battle.CreateSquad("孤軍", 0, Vector2(4, 2), 20);
        Squad* packA = battle.CreateSquad("擠A", 0, Vector2(14, 2), 20);
        Squad* packB = battle.CreateSquad("擠B", 0, Vector2(14.4f, 2), 20);
        Squad* packC = battle.CreateSquad("擠C", 0, Vector2(14, 2.4f), 20);
        battle.CreateSquad("擠D", 0, Vector2(14.4f, 2.4f), 20);
        battle.CreateSquad("敵", 1, Vector2(12, 15), 20);

        battle.SetJamming(2.0f, jp);
        battle.BeginExecution();
        lone->IssueOrder(SquadOrder::MoveTo, Vector2(4, 12));
        packA->IssueOrder(SquadOrder::MoveTo, Vector2(14, 12));
        packB->IssueOrder(SquadOrder::MoveTo, Vector2(14.4f, 12));
        packC->IssueOrder(SquadOrder::MoveTo, Vector2(14, 12.4f));

        const Vector2 loneStart = lone->GetPosition();
        const Vector2 packStart = packA->GetPosition();
        for (int i = 0; i < 60; ++i) battle.Update(0.1f);
        const float loneDist =
            (lone->GetPosition() - loneStart).Length();
        const float packDist =
            (packA->GetPosition() - packStart).Length();
        printf("    60 tick 位移：lone=%.2f pack=%.2f\n", loneDist, packDist);
        Check(packDist < loneDist * 0.7f,
              "擁擠群位移顯著小於孤隊（<70%）");
    }

    // [3] 預設關閉：不設 SetJamming，擁擠群照舊全速
    {
        BattleController battle(24, 16, 1.0f);
        Squad* packA = battle.CreateSquad("擠A", 0, Vector2(14, 4), 20);
        battle.CreateSquad("擠B", 0, Vector2(14.4f, 4), 20);
        battle.CreateSquad("擠C", 0, Vector2(14, 4.4f), 20);
        battle.CreateSquad("敵", 1, Vector2(12, 14), 20);
        battle.BeginExecution();
        battle.Update(0.1f);
        Check(packA->GetCrowdFactor() == 1.0f,
              "未啟用 jamming → crowdFactor 恆 1（無回歸）");
    }

    // [4] 關閉復位：開後再關，倍率回 1
    {
        BattleController battle(24, 16, 1.0f);
        Squad* packA = battle.CreateSquad("擠A", 0, Vector2(14, 4), 20);
        battle.CreateSquad("擠B", 0, Vector2(14.4f, 4), 20);
        battle.CreateSquad("擠C", 0, Vector2(14, 4.4f), 20);
        battle.CreateSquad("擠D", 0, Vector2(14.4f, 4.4f), 20);
        battle.CreateSquad("敵", 1, Vector2(12, 14), 20);
        battle.SetJamming(2.0f, jp);
        battle.BeginExecution();
        battle.Update(0.1f);
        const bool wasJammed = packA->GetCrowdFactor() < 1.0f;
        battle.SetJamming(0.0f, jp);
        Check(wasJammed && packA->GetCrowdFactor() == 1.0f,
              "SetJamming(0) 關閉並復位 crowdFactor");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
