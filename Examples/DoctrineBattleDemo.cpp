/**
 * DoctrineBattleDemo - 回合規劃 × 即時執行的 doctrine 戰鬥演示
 *
 * 對應設計：玩家（team 0）在部署階段寫好每隊的 doctrine 腳本，
 * 進入即時層後小隊依規則自動作戰；玩家只有 CP 介入點數可微操。
 */

#include "Gameplay/BattleController.h"

#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

int main() {
    printf("=== Doctrine Battle Demo ===\n\n");

    int failures = 0;

    // 規則冷卻（CAP-8）：命中後冷卻期間被略過，冷卻結束恢復
    {
        DoctrineSet cd;
        cd.AddRule({DoctrineTrigger::Always, DoctrineAction::RetreatToRally,
                    0.0f, 0, 2.0f});
        cd.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition,
                    0.0f, 100});

        SquadContext ctx;
        ctx.now = 0.0f;
        if (cd.Evaluate(ctx) != DoctrineAction::RetreatToRally) {
            printf("FAIL: cooldown rule did not fire initially\n");
            failures++;
        }
        ctx.now = 1.0f; // 冷卻中（until 2.0）
        if (cd.Evaluate(ctx) != DoctrineAction::HoldPosition) {
            printf("FAIL: cooling rule was not skipped\n");
            failures++;
        }
        ctx.now = 3.0f; // 冷卻結束
        if (cd.Evaluate(ctx) != DoctrineAction::RetreatToRally) {
            printf("FAIL: rule did not refire after cooldown\n");
            failures++;
        }
    }

    // 20x15 戰場，中央一道牆只留缺口
    BattleController battle(20, 15, 1.0f);
    for (int y = 0; y < 15; ++y) {
        if (y < 6 || y > 8) {
            battle.GetField().SetObstacle(10, y, true);
        }
    }

    battle.SetEventCallback([](const std::string& msg) {
        printf("  %s\n", msg.c_str());
    });

    // ---- 部署階段 ----
    printf("[Deployment]\n");

    battle.SetObjective(0, Vector2(18.0f, 7.0f)); // 玩家目標：攻下右側
    battle.SetObjective(1, Vector2(1.0f, 7.0f));  // 敵方目標：反攻左側
    battle.SetRallyPoint(0, Vector2(1.0f, 7.0f));
    battle.SetRallyPoint(1, Vector2(18.0f, 7.0f));
    battle.SetCommandPoints(0, 2);
    battle.SetCommandPoints(1, 0);

    Squad* alpha = battle.CreateSquad("Alpha", 0, Vector2(2.0f, 4.0f), 30);
    Squad* bravo = battle.CreateSquad("Bravo", 0, Vector2(2.0f, 7.0f), 30);
    Squad* charlie = battle.CreateSquad("Charlie", 0, Vector2(2.0f, 10.0f), 20);

    Squad* defenderA = battle.CreateSquad("Shield", 1, Vector2(16.0f, 7.0f), 35);
    Squad* defenderB = battle.CreateSquad("Spear", 1, Vector2(17.0f, 5.0f), 25);
    (void)defenderA;
    (void)defenderB;

    // Alpha：前排主攻——接敵就打，殘血撤退，預設推進
    {
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::HealthBelow,
                   DoctrineAction::RetreatToRally, 0.3f, 0});
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackNearest, 3.0f, 10});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::AdvanceToObjective, 0.0f, 100});
        battle.AssignDoctrine(alpha, d);
    }

    // Bravo：支援——友軍接戰才投入，否則推進
    {
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::AllyEngaged,
                   DoctrineAction::DefendNearestAlly, 0.0f, 5});
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackWeakest, 4.0f, 10});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::AdvanceToObjective, 0.0f, 100});
        battle.AssignDoctrine(bravo, d);
    }

    // Charlie：預備隊——士氣低先撤，被攻擊才還手，否則原地待命
    {
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::MoraleBelow,
                   DoctrineAction::RetreatToRally, 0.4f, 0});
        d.AddRule({DoctrineTrigger::UnderAttack,
                   DoctrineAction::AttackNearest, 0.0f, 10});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::HoldPosition, 0.0f, 100});
        battle.AssignDoctrine(charlie, d);
    }

    // 敵方：死守型——被靠近或受攻擊才還手
    {
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackNearest, 5.0f, 10});
        d.AddRule({DoctrineTrigger::UnderAttack,
                   DoctrineAction::AttackNearest, 0.0f, 5});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::HoldPosition, 0.0f, 100});
        battle.AssignDoctrine(defenderA, d);
        battle.AssignDoctrine(defenderB, d);
    }

    // ---- 即時執行 ----
    if (!battle.BeginExecution()) {
        printf("ERROR: BeginExecution failed\n");
        return 1;
    }

    printf("\n[Execution]\n");
    float dt = 0.1f;
    int maxTicks = 3000; // 300 秒遊戲時間上限

    bool intervened = false;
    for (int tick = 0; tick < maxTicks; ++tick) {
        battle.Update(dt);

        // 示範 CP 介入：第 300 tick 命令 Charlie 投入戰場
        if (!intervened && tick == 300) {
            intervened = battle.Intervene(
                charlie, SquadOrder::MoveTo, Vector2(10.5f, 7.0f), 60.0f);
        }

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

    // 驗證：戰鬥應在時限內分出勝負，且 CP 介入成功
    if (battle.GetOutcome() == BattleOutcome::Ongoing) {
        printf("FAIL: battle did not resolve\n");
        failures++;
    }
    if (!intervened) {
        printf("FAIL: CP intervention was rejected\n");
        failures++;
    }
    if (battle.GetCommandPoints(0) != 1) {
        printf("FAIL: expected 1 CP remaining, got %d\n",
               battle.GetCommandPoints(0));
        failures++;
    }

    // ---- G-1 士氣連鎖潰逃（TW chain rout）----
    // a 潰逃 → 半徑內友軍吃衝擊 → 低士氣的 near 跟著崩 → 同 tick 繼續
    // 向外傳給 far；敵軍與半徑外的錨點不受影響
    {
        printf("\n-- G-1 chain rout --\n");
        BattleController cb(12, 12, 1.0f);
        cb.SetRoutShock(/*radius=*/5.0f, /*moraleHit=*/0.4f);
        Squad* a    = cb.CreateSquad("潰隊", 0, Vector2(2, 5), 10);
        Squad* near = cb.CreateSquad("近鄰", 0, Vector2(4, 5), 10);
        Squad* far  = cb.CreateSquad("遠哨", 0, Vector2(9, 5), 10);
        Squad* anch = cb.CreateSquad("錨點", 0, Vector2(9, 9), 10);
        Squad* foe  = cb.CreateSquad("敵軍", 1, Vector2(4, 7), 10);
        if (!cb.BeginExecution()) {
            printf("FAIL: chain-rout battle did not start\n");
            failures++;
        }
        near->AdjustMorale(-0.75f);   // 0.25——吃一發衝擊就會崩
        a->AdjustMorale(-1.0f);       // 士氣歸零 → routing
        if (!a->IsRouting()) {
            printf("FAIL: zero morale did not rout\n");
            failures++;
        }
        cb.Update(0.05f);             // 衝擊擴散 + 同 tick 連鎖

        if (!near->IsRouting()) {
            printf("FAIL: chain rout did not break neighbor (morale %.2f)\n",
                   near->GetMorale());
            failures++;
        }
        if (std::fabs(far->GetMorale() - 0.6f) > 0.01f) {
            printf("FAIL: chained shock did not reach far (morale %.2f)\n",
                   far->GetMorale());
            failures++;
        }
        if (far->IsRouting()) {
            printf("FAIL: high-morale squad should hold the chain\n");
            failures++;
        }
        if (std::fabs(anch->GetMorale() - 1.0f) > 0.001f) {
            printf("FAIL: out-of-radius squad affected (%.2f)\n",
                   anch->GetMorale());
            failures++;
        }
        if (std::fabs(foe->GetMorale() - 1.0f) > 0.001f) {
            printf("FAIL: enemy squad affected by friendly rout (%.2f)\n",
                   foe->GetMorale());
            failures++;
        }
    }

    // 關閉（預設）：潰逃不產生衝擊——回歸語義不變
    {
        BattleController nb(10, 10, 1.0f);
        Squad* a = nb.CreateSquad("潰隊", 0, Vector2(2, 5), 10);
        Squad* b = nb.CreateSquad("鄰隊", 0, Vector2(3, 5), 10);
        nb.BeginExecution();
        a->AdjustMorale(-1.0f);
        nb.Update(0.05f);
        if (std::fabs(b->GetMorale() - 1.0f) > 0.001f) {
            printf("FAIL: rout shock fired while disabled (%.2f)\n",
                   b->GetMorale());
            failures++;
        }
    }

    printf(failures == 0 ? "\nALL CHECKS PASSED\n" : "\n%d CHECK(S) FAILED\n",
           failures);
    return failures;
}
