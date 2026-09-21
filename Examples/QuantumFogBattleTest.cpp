// QuantumFogBattleTest - Q-1 敵情霧接入戰鬥的無頭驗證
// 覆蓋:情報點觀測塌縮、時效到期回雲、接觸偵查免費揭露、
//       Scout doctrine 向最近未揭露雲的 modal 候選移動

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleResources.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/QuantumFog.h"

#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name, float value = 0.0f,
                  float expected = 0.0f) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s  (got %.4f, expected %.4f)\n", name, value, expected);
    }
}

// 跑 battle 直到條件成立或逾時(秒);回傳是否達成
template <typename Pred>
static bool RunUntil(BattleController& b, float seconds, Pred pred) {
    const float step = 0.1f;
    for (float t = 0.0f; t < seconds; t += step) {
        b.Update(step);
        if (pred()) return true;
    }
    return false;
}

int main() {
    printf("=== Quantum Fog Battle Tests ===\n\n");

    // ---- 情報觀測路徑 ----
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/3, /*cp=*/0);

        QuantumFog fog(/*intelDuration=*/5.0f, /*cost=*/1);
        fog.BindResources(&res);
        battle.BindFog(&fog);

        battle.CreateSquad("偵查隊", 0, Vector2(2, 7), 20);
        Squad* enemy = battle.CreateSquad("敵軍", 1, Vector2(15, 7), 20);
        const int eid = fog.AddEntity("敵軍", /*觀測方=*/0,
            {Vector2(14, 7), Vector2(15, 8), Vector2(16, 7)},
            {0.2, 0.6, 0.2});
        battle.BindFogSquad(enemy, eid);
        battle.BeginExecution();

        // [1] 觀測:扣情報 → 塌縮揭露真實位置
        Check(!fog.IsRevealed(eid), "初始為疊加態");
        Check(fog.Observe(eid, enemy->GetPosition()), "觀測成功");
        Check(res.GetIntel(0) == 2, "觀測扣 1 情報", (float)res.GetIntel(0), 2.0f);
        Check(fog.IsRevealed(eid), "觀測後揭露");
        Vector2 rp = fog.GetRevealedPos(eid);
        Check(std::fabs(rp.x - 15.0f) < 1e-4f && std::fabs(rp.y - 7.0f) < 1e-4f,
              "揭露真實位置", rp.x, 15.0f);
        Check(fog.GetCloud(eid).size() == 1, "塌縮後雲收斂為單點");

        // [2] 重複觀測已揭露者不再扣點
        Check(fog.Observe(eid, enemy->GetPosition()) && res.GetIntel(0) == 2,
              "已揭露重複觀測不扣點");

        // [2b] 再觀測刷新情報時效與真值位置(敵軍移動後重報)
        enemy->IssueOrder(SquadOrder::MoveTo, Vector2(15, 9));
        RunUntil(battle, 4.0f, [&] {
            return (enemy->GetPosition() - Vector2(15, 9)).Length() < 0.3f;
        });
        const Vector2 newTruePos = enemy->GetPosition();
        Check(fog.Observe(eid, newTruePos), "再觀測刷新情報");
        for (int i = 0; i < 30; ++i) battle.Update(0.1f); // 3s < 5s 時效
        Check(fog.IsRevealed(eid), "刷新後時效內仍揭露");
        rp = fog.GetRevealedPos(eid);
        Check((rp - newTruePos).Length() < 0.05f &&
                  std::fabs(rp.y - 7.0f) > 0.5f,
              "揭露位置更新為新真值", rp.y, newTruePos.y);

        // [3] 情報時效到期 → 回疊加態(雙方距離 13 格,不會接觸揭露)
        Check(RunUntil(battle, 8.0f, [&] { return !fog.IsRevealed(eid); }),
              "時效到期回疊加態");
        Check(fog.GetCloud(eid).size() > 1, "回雲後恢復多候選");
    }

    // ---- 接觸偵查 + Scout doctrine ----
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/2, /*cp=*/0);

        QuantumFog fog(5.0f, 1);
        fog.BindResources(&res);
        battle.BindFog(&fog);
        battle.SetFogRevealRange(3.0f);

        Squad* player = battle.CreateSquad("前鋒", 0, Vector2(2, 7), 30);
        Squad* enemy = battle.CreateSquad("伏兵", 1, Vector2(15, 7), 20);
        const int eid = fog.AddEntity("伏兵", 0,
            {Vector2(14, 7), Vector2(15, 8), Vector2(16, 7)},
            {0.2, 0.6, 0.2});
        battle.BindFogSquad(enemy, eid);

        // 戰報計數:持續接觸只能發一次「目擊」,不可每幀洗版
        int spottedEvents = 0;
        battle.SetEventCallback([&](const std::string& e) {
            if (e.find("目擊") != std::string::npos) ++spottedEvents;
        });

        // Scout:Always → 往最近未揭露雲的 modal 候選移動
        DoctrineSet scoutDoc;
        scoutDoc.AddRule({DoctrineTrigger::Always, DoctrineAction::Scout,
                          0.0f, 10});
        battle.AssignDoctrine(player, scoutDoc);
        battle.BeginExecution();

        // [4] Scout 指令:order=AttackMove 且目標是 modal 候選 (15,8)
        Check(RunUntil(battle, 1.0f, [&] {
                  return player->GetOrder() == SquadOrder::AttackMove;
              }),
              "Scout 觸發 AttackMove");
        Vector2 st = player->GetOrderTarget();
        Check(std::fabs(st.x - 15.0f) < 0.5f && std::fabs(st.y - 8.0f) < 0.5f,
              "Scout 目標為 modal 候選格", st.x, 15.0f);

        // [5] 接觸偵查:走進 fogRevealRange 免費揭露,不扣情報
        Check(RunUntil(battle, 30.0f, [&] { return fog.IsRevealed(eid); }),
              "接近後接觸揭露");
        Check(res.GetIntel(0) == 2, "接觸揭露不扣情報",
              (float)res.GetIntel(0), 2.0f);

        // [5b] 維持接觸再跑 3s——目擊戰報只能發一次
        for (int i = 0; i < 30; ++i) battle.Update(0.1f);
        Check(spottedEvents == 1, "持續接觸只發一次目擊戰報",
              (float)spottedEvents, 1.0f);
    }

    // ---- 未綁 fog 時 Scout 退回 Hold(無回歸)----
    {
        BattleController battle(20, 15, 1.0f);
        Squad* lone = battle.CreateSquad("孤軍", 0, Vector2(5, 5), 10);
        DoctrineSet scoutDoc;
        scoutDoc.AddRule({DoctrineTrigger::Always, DoctrineAction::Scout,
                          0.0f, 10});
        battle.AssignDoctrine(lone, scoutDoc);
        battle.BeginExecution();
        // 跑 4 個 doctrine 評估週期,確認不會誤發 AttackMove
        for (int i = 0; i < 10; ++i) battle.Update(0.1f);
        Vector2 lt = lone->GetOrderTarget();
        Check(lone->GetOrder() == SquadOrder::Hold &&
                  std::fabs(lt.x - 5.0f) < 0.5f &&
                  std::fabs(lt.y - 5.0f) < 0.5f,
              "無 fog 時 Scout 退回 Hold 原地");
    }

    // ---- 負面觀測:目視覆蓋候選格但沒人 → 候選消去、雲縮小 ----
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, 2, 0);
        QuantumFog fog(5.0f, 1);
        fog.BindResources(&res);
        battle.BindFog(&fog);
        battle.SetFogRevealRange(2.0f);

        // 玩家小隊站在錯的候選格 (5,3);真身在 (15,12)
        battle.CreateSquad("目視隊", 0, Vector2(5, 3), 10);
        Squad* enemy = battle.CreateSquad("伏軍", 1, Vector2(15, 12), 10);
        const int eid = fog.AddEntity("伏軍", 0,
                                    {Vector2(5, 3), Vector2(15, 12)},
                                    {0.9, 0.1});
        battle.BindFogSquad(enemy, eid);
        battle.BeginExecution();

        Check(fog.GetCloud(eid).size() == 2, "初始雲有 2 候選");
        battle.Update(0.1f);
        // (5,3) 候選被目視覆蓋且真身不在 → 消去;真身格未被覆蓋 → 仍在雲中
        const auto cloud = fog.GetCloud(eid);
        Check(cloud.size() == 1 && !fog.IsRevealed(eid),
              "負面觀測消去空候選格",
              (float)cloud.size(), 1.0f);
        if (cloud.size() == 1) {
            Check(std::fabs(cloud[0].first.x - 15.0f) < 1e-3f &&
                      std::fabs(cloud[0].first.y - 12.0f) < 1e-3f,
                  "殘存候選為真身所在格", cloud[0].first.x, 15.0f);
        }
    }

    // ---- 隱形敵軍不可被介入/自動索敵;綁定驗證;action 字串映射 ----
    {
        BattleController battle(20, 15, 1.0f);
        BattleResources res;
        res.Setup(battle, 0, /*intel=*/2, /*cp=*/2);

        QuantumFog fog(5.0f, 1);
        fog.BindResources(&res);
        battle.BindFog(&fog);
        battle.SetFogRevealRange(2.0f);

        // 相距 4 格 > fogRevealRange:保持隱形;候選格也都不在目視內
        Squad* player = battle.CreateSquad("前鋒", 0, Vector2(10, 5), 20);
        Squad* enemy = battle.CreateSquad("伏兵", 1, Vector2(10, 9), 20);

        // 無效 entityId(-1 / >= EntityCount)的綁定被拒
        battle.BindFogSquad(player, -1);
        battle.BindFogSquad(player, 99);
        Check(battle.GetFogEntityId(player) == -1, "無效 entityId 綁定被拒");

        const int eid = fog.AddEntity("伏兵", 0,
                                    {Vector2(6, 9), Vector2(14, 9)},
                                    {0.5, 0.5});
        battle.BindFogSquad(enemy, eid);

        // 範圍內只有隱形敵軍:EnemyInRange→AttackNearest 不得觸發
        DoctrineSet doc;
        doc.AddRule({DoctrineTrigger::EnemyInRange,
                     DoctrineAction::AttackNearest, 6.0f, 1});
        doc.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition,
                     0.0f, 90});
        battle.AssignDoctrine(player, doc);
        battle.BeginExecution();

        // 隱形目標的 Engage 介入被拒,且不扣 CP
        Check(!battle.Intervene(player, SquadOrder::Engage, enemy),
              "Engage 隱形敵軍被拒");
        Check(battle.GetCommandPoints(0) == 2, "失敗介入不扣 CP",
              (float)battle.GetCommandPoints(0), 2.0f);

        for (int i = 0; i < 10; ++i) battle.Update(0.1f);
        Check(player->GetOrder() == SquadOrder::Hold &&
                  player->GetEngageTarget() == nullptr,
              "只見隱形敵軍時 doctrine 不索敵");

        // 揭露後同一個 Engage 介入成立且扣 CP
        fog.Reveal(eid, enemy->GetPosition());
        Check(battle.Intervene(player, SquadOrder::Engage, enemy),
              "揭露後 Engage 成立");
        Check(battle.GetCommandPoints(0) == 1, "成功介入扣 1 CP",
              (float)battle.GetCommandPoints(0), 1.0f);

        // doctrine 卡 action 字串映射含 Scout
        Check(EnemyGeneral::ActionFromString("Scout") ==
                  DoctrineAction::Scout,
              "ActionFromString 支援 Scout");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
