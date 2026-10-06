/**
 * DuanqiaoDemo - 斷橋原型：T-4~T-8 整合驗證
 *
 * 一場完整三拍：
 *   部署（回合層）：載入斷橋地圖 → 玩家方 BattlePlanner 自動規劃、
 *                   敵方格洛克手寫腳本 → 名冊登記隊長 → 花情報解鎖敵情
 *   即時層：doctrine 自動執行 + 士氣執行率（<30% → 70%）+ 事件錄製
 *   戰後：回放存檔 + 名冊陣亡紀錄存檔
 */

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/BattlePlanner.h"
#include "Gameplay/BattleRecorder.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/Roster.h"

#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

int main() {
    printf("=== Duanqiao (斷橋) Prototype ===\n\n");
    int failures = 0;

    // ---- T-4：載入斷橋地圖 ----
    BattleMap map;
    if (!map.LoadFromFile("assets/maps/duanqiao.json")) {
        printf("FAIL: cannot load duanqiao.json\n");
        return 1;
    }
    printf("[Map] %s  %dx%d cell=%.1f  zones=%zu fords=%zu pins=%zu\n",
           map.GetName().c_str(), map.GetGridWidth(), map.GetGridHeight(),
           map.GetCellSize(), map.GetZones().size(), map.GetFords().size(),
           map.GetPins().size());

    BattleController battle(map.GetGridWidth(), map.GetGridHeight(),
                            map.GetCellSize());
    map.ApplyToField(battle.GetField());

    // 部署區合法性檢查
    if (!map.IsInDeployZone(0, Vector2(12.0f, 2.0f)) ||
        map.IsInDeployZone(0, Vector2(12.0f, 12.0f))) {
        printf("FAIL: deploy zone check wrong\n");
        failures++;
    }
    const MapPin* bridge = map.FindPin("斷橋");
    if (!bridge) {
        printf("FAIL: bridge pin missing\n");
        failures++;
    }

    // ---- 部署：我方 4 隊（北岸）、敵方格洛克 4 隊（南岸）----
    const MapPin* northRally = map.FindPin("北岸集結點");
    const MapPin* southCamp = map.FindPin("南岸敵營");

    Squad* alpha   = battle.CreateSquad("前鋒", 0, Vector2(8.0f, 3.0f),  30);
    Squad* bravo   = battle.CreateSquad("中軍", 0, Vector2(12.0f, 2.5f), 30);
    Squad* charlie = battle.CreateSquad("左翼", 0, Vector2(16.0f, 3.0f), 25);
    Squad* delta   = battle.CreateSquad("後衛", 0, Vector2(12.0f, 4.5f), 15);

    battle.CreateSquad("格洛克親衛", 1, Vector2(12.0f, 12.0f), 35);
    battle.CreateSquad("蠻兵隊",     1, Vector2(9.0f, 12.5f),  25);
    battle.CreateSquad("掠奪隊",     1, Vector2(15.0f, 12.5f), 25);
    battle.CreateSquad("守橋隊",     1, Vector2(12.0f, 10.5f), 20);

    battle.SetObjective(0, southCamp->pos);   // 攻下南岸敵營
    battle.SetObjective(1, northRally->pos);  // 敵方反撲北岸集結點
    battle.SetRallyPoint(0, northRally->pos);
    battle.SetRallyPoint(1, southCamp->pos);

    // ---- T-5：敵將格洛克的人格腳本 ----
    EnemyGeneral glock = EnemyGeneral::MakeGlock();
    printf("[Enemy] %s「%s」 侵略%.0f 紀律%.0f 狡詐%.0f\n",
           glock.GetName().c_str(), glock.GetEpithet().c_str(),
           glock.GetAggression(), glock.GetDiscipline(), glock.GetCunning());
    glock.ApplyTo(battle, 1);

    // ---- 我方：智能規劃 ----
    BattlePlanner planner;
    BattlePlanner::Plan plan = planner.GeneratePlan(battle, 0);
    planner.ApplyPlan(battle, plan);
    printf("[Plan] %s\n", plan.summary.c_str());
    for (const auto& sp : plan.squadPlans) {
        printf("    %-6s %s\n", sp.squad->GetName().c_str(),
               sp.rationale.c_str());
    }

    // ---- T-6：情報/CP/士氣執行率 ----
    BattleResources res;
    res.Setup(battle, 0, 10, 3); // 規格：10 情報 / 3 CP
    res.Setup(battle, 1, 10, 3);
    if (res.RevealEnemyPersonality(0)) {
        printf("[Intel] 花 3 情報解鎖敵將人格（剩 %d）\n",
               res.GetIntel(0));
    } else {
        printf("FAIL: intel spend rejected\n");
        failures++;
    }
    BattleResources::ApplyMoraleRule(battle); // <30% → 70% 執行率

    // ---- T-8：名冊登記（隊長級）----
    Roster roster;
    roster.Enroll(alpha,   "老周", "captain", "斷刃");
    roster.Enroll(bravo,   "石敢當", "captain", "家書");
    roster.Enroll(charlie, "小陳", "captain", "銅哨");
    roster.Enroll(delta,   "何伯", "veteran", "菸袋");
    roster.Enroll(battle.GetSquads()[4].get(), "格洛克", "general",
                  "斷橋鑰匙");

    // ---- T-7：事件錄製（先設列印回調，Attach 會把它當下游包進來）----
    battle.SetEventCallback([](const std::string& msg) {
        printf("  %s\n", msg.c_str());
    });
    BattleRecorder recorder;
    recorder.Attach(battle);

    if (!battle.BeginExecution()) {
        printf("ERROR: BeginExecution failed\n");
        return 1;
    }

    printf("\n[Execution]\n");
    float dt = 0.1f;
    for (int tick = 0; tick < 4000; ++tick) {
        battle.Update(dt);
        roster.Update(battle);
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
    default:                     printf("Outcome: ONGOING\n"); break;
    }

    printf("名冊：%zu 在世 / %zu 陣亡\n", roster.AliveCount(),
           roster.DeadCount());
    for (const auto& e : roster.GetEntries()) {
        if (!e.alive) {
            printf("  ✝ %s（%s，%s）t=%.1f 遺物：%s\n", e.name.c_str(),
                   e.squadName.c_str(), e.rank.c_str(), e.deathTime,
                   e.relic.c_str());
        }
    }

    // ---- 存檔驗證 ----
    bool replayOk = recorder.SaveToFile("duanqiao_replay.json") &&
                    recorder.Count() > 0;
    bool rosterOk = roster.SaveToFile("duanqiao_roster.json");

    BattleRecorder loaded;
    if (!loaded.LoadFromFile("duanqiao_replay.json") ||
        loaded.Count() != recorder.Count()) {
        replayOk = false;
    }

    if (battle.GetOutcome() == BattleOutcome::Ongoing) {
        printf("FAIL: battle did not resolve\n");
        failures++;
    }
    if (!replayOk) { printf("FAIL: replay save/load\n"); failures++; }
    if (!rosterOk) { printf("FAIL: roster save\n"); failures++; }
    if (recorder.Count() < 5) {
        printf("FAIL: too few events recorded (%zu)\n", recorder.Count());
        failures++;
    }

    printf("\n錄製事件 %zu 筆 → duanqiao_replay.json\n", recorder.Count());
    printf("名冊 → duanqiao_roster.json\n");
    printf(failures == 0 ? "\nALL CHECKS PASSED\n" : "\n%d CHECK(S) FAILED\n",
           failures);
    return failures;
}
