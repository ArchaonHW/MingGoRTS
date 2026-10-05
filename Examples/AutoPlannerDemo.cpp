/**
 * AutoPlannerDemo - 智能規劃演示
 *
 * 只佈署小隊與目標，doctrine 全部由 BattlePlanner 自動生成：
 * 兵力比決定戰略姿態，隊內強度排序決定角色，附可解釋的 rationale。
 */

#include "Gameplay/BattleController.h"
#include "Gameplay/BattlePlanner.h"

#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

int main() {
    printf("=== Auto Planner Demo ===\n\n");

    // 20x15 戰場，中央牆留缺口（與 DoctrineBattleDemo 相同地形）
    BattleController battle(20, 15, 1.0f);
    for (int y = 0; y < 15; ++y) {
        if (y < 6 || y > 8) {
            battle.GetField().SetObstacle(10, y, true);
        }
    }

    battle.SetObjective(0, Vector2(18.0f, 7.0f));
    battle.SetObjective(1, Vector2(1.0f, 7.0f));
    battle.SetRallyPoint(0, Vector2(1.0f, 7.0f));
    battle.SetRallyPoint(1, Vector2(18.0f, 7.0f));

    // 玩家方三隊（一強一中一弱）、敵方兩隊（一大一小）
    battle.CreateSquad("Alpha",   0, Vector2(2.0f, 4.0f),  30);
    battle.CreateSquad("Bravo",   0, Vector2(2.0f, 7.0f),  30);
    battle.CreateSquad("Charlie", 0, Vector2(2.0f, 10.0f), 20);
    battle.CreateSquad("Shield",  1, Vector2(16.0f, 7.0f), 35);
    battle.CreateSquad("Spear",   1, Vector2(17.0f, 5.0f), 25);

    // ---- 智能規劃 ----
    printf("[Planning]\n");
    BattlePlanner planner;

    for (int team = 0; team <= 1; ++team) {
        BattlePlanner::Plan plan = planner.GeneratePlan(battle, team);
        printf("  %s\n", plan.summary.c_str());
        for (const auto& sp : plan.squadPlans) {
            printf("    %-8s %s\n", sp.squad->GetName().c_str(),
                   sp.rationale.c_str());
        }
        planner.ApplyPlan(battle, plan);
    }
    printf("\n");

    // ---- 即時執行 ----
    if (!battle.BeginExecution()) {
        printf("ERROR: BeginExecution failed\n");
        return 1;
    }

    battle.SetEventCallback([](const std::string& msg) {
        printf("  %s\n", msg.c_str());
    });

    printf("[Execution]\n");
    float dt = 0.1f;
    for (int tick = 0; tick < 3000; ++tick) {
        battle.Update(dt);
        if (battle.GetPhase() == BattlePhase::Resolution) {
            break;
        }
    }

    // ---- 戰後 ----
    printf("\n[Resolution]\n");
    switch (battle.GetOutcome()) {
    case BattleOutcome::Victory: printf("Outcome: VICTORY\n"); break;
    case BattleOutcome::Defeat:  printf("Outcome: DEFEAT\n"); break;
    case BattleOutcome::Draw:    printf("Outcome: DRAW\n"); break;
    default:                     printf("Outcome: ONGOING (timeout)\n"); break;
    }

    for (const auto& squad : battle.GetSquads()) {
        printf("  %-8s team=%d members=%d/%d morale=%.2f%s\n",
               squad->GetName().c_str(), squad->GetTeam(),
               squad->GetMembers(), squad->GetMaxMembers(),
               squad->GetMorale(),
               squad->IsRouting() ? " [routing]" : "");
    }

    // 驗證：自動規劃的戰鬥應在時限內分出勝負，
    // 且規劃器有實際產出 doctrine（CP > 0 表示 ApplyPlan 生效）
    int failures = 0;
    if (battle.GetOutcome() == BattleOutcome::Ongoing) {
        printf("FAIL: battle did not resolve\n");
        failures++;
    }
    if (battle.GetCommandPoints(0) <= 0 || battle.GetCommandPoints(1) <= 0) {
        printf("FAIL: planner did not assign command points\n");
        failures++;
    }

    printf(failures == 0 ? "\nALL CHECKS PASSED\n" : "\n%d CHECK(S) FAILED\n",
           failures);
    return failures;
}
