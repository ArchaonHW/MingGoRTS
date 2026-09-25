// GovernanceFieldTest — Epic A 戰場治理追蹤（headless）
// 覆蓋：村莊佔領（A.1）、焚村標記（A.1 burned 狀態位）、
// 護輜抵達/被劫/劫敵輜（A.3/A.4）、墮落 ratchet（A.5）。

#include "Campaign/Governance.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/GovernanceField.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;
using namespace Potato::Campaign;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        ++failures;
    }
}

static int CountOf(const BattleController& b, GovernanceEvent ev) {
    const auto& m = b.GetGovernanceEvents();
    const auto it = m.find(ev);
    return it == m.end() ? 0 : it->second;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== GovernanceField Tests (Epic A) ===\n");

    // ---- [1] 村莊佔領（A.1）----
    printf("\n[1] 村莊佔領\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        battle.CreateSquad("敵軍", 1, Vector2(9, 9), 10);
        battle.BeginExecution();

        GovernanceField field;
        field.Bind({{"village", Vector2(4, 4), 2.0f, ""},
                    {"oil_slick", Vector2(8, 8), 1.0f, ""}},
                   {});
        field.Update(0.1f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageOccupied) == 1,
              "我軍進村 → VillageOccupied×1");
        field.Update(0.1f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageOccupied) == 1,
              "佔領每點只記一次");
        Check(!field.IsBurned(0), "村莊未焚");
        Check(CountOf(battle, GovernanceEvent::VillageBurned) == 0,
              "和平佔領不計焚村");
    }

    // ---- [2] 焚村（A.1 火力波及 → burned 狀態位）----
    printf("\n[2] 焚村\n");
    {
        BattleController battle(10, 10, 1.0f);
        Squad* foe = battle.CreateSquad("敵軍", 1, Vector2(4, 4), 10);
        battle.CreateSquad("我軍", 0, Vector2(9, 9), 10);
        battle.BeginExecution();

        GovernanceField field;
        field.Bind({{"village", Vector2(4, 4), 2.0f, ""}}, {});
        field.Update(0.1f, battle); // 建立員額基線
        foe->ApplyCasualties(3);    // 村內損員 = 戰火波及
        field.Update(0.1f, battle);
        Check(field.IsBurned(0), "村莊 burned 標記");
        Check(CountOf(battle, GovernanceEvent::VillageBurned) == 1,
              "焚村 → VillageBurned×1");
        foe->ApplyCasualties(2);
        field.Update(0.1f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageBurned) == 1,
              "同村只焚一次");
    }

    // ---- [3] 護輜抵達（A.3/A.4）----
    printf("\n[3] 護輜抵達\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("敵軍", 1, Vector2(9, 9), 10); // 遠離路徑
        battle.BeginExecution();

        MapConvoy def;
        def.id = "supply-1";
        def.team = 0;
        def.path = {Vector2(0, 0), Vector2(3, 0)};
        def.speed = 2.0f;
        def.hp = 60;
        def.raidRadius = 1.0f;

        GovernanceField field;
        field.Bind({}, {def});
        field.Update(0.5f, battle); // 走 1 格
        Check(field.GetConvoys()[0].status ==
                  GovernanceField::ConvoyState::Status::Moving,
              "運輸隊移動中");
        Check(std::fabs(field.GetConvoys()[0].pos.x - 1.0f) < 1e-4f,
              "位置沿 waypoint 前進");
        field.Update(1.0f, battle); // 走 2 格 → 抵達
        field.Update(0.1f, battle);
        Check(field.GetConvoys()[0].status ==
                  GovernanceField::ConvoyState::Status::Arrived,
              "運輸隊抵達");
        Check(CountOf(battle, GovernanceEvent::ConvoyProtected) == 1,
              "抵達 → ConvoyProtected×1");
    }

    // ---- [4] 我輜被劫（A.4）----
    printf("\n[4] 我輜被劫\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("敵軍", 1, Vector2(0.5f, 0), 10); // 圈內劫掠
        battle.BeginExecution();

        MapConvoy def;
        def.id = "supply-1";
        def.team = 0;
        def.path = {Vector2(0, 0), Vector2(5, 0)};
        def.speed = 0.5f;
        def.hp = 10; // 低 hp：kRaidDps×dt 兩拍內擊毀
        def.raidRadius = 1.0f;

        GovernanceField field;
        field.Bind({}, {def});
        for (int i = 0; i < 10 &&
                        field.GetConvoys()[0].status ==
                            GovernanceField::ConvoyState::Status::Moving;
             ++i) {
            field.Update(0.2f, battle);
        }
        Check(field.GetConvoys()[0].status ==
                  GovernanceField::ConvoyState::Status::Raided,
              "運輸隊被劫毀");
        Check(CountOf(battle, GovernanceEvent::ConvoyLost) == 1,
              "我輜被劫 → ConvoyLost×1");
        Check(CountOf(battle, GovernanceEvent::ConvoyProtected) == 0,
              "被劫不記護輜");
    }

    // ---- [5] 劫敵輜（A.4：敵全軍士氣打擊）----
    printf("\n[5] 劫敵輜\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(0.5f, 0), 10); // 圈內劫掠
        Squad* foeFar =
            battle.CreateSquad("敵軍", 1, Vector2(9, 9), 10);
        battle.BeginExecution();
        const float morale0 = foeFar->GetMorale();

        MapConvoy def;
        def.id = "enemy-supply";
        def.team = 1;
        def.path = {Vector2(0, 0), Vector2(5, 0)};
        def.speed = 0.5f;
        def.hp = 10;
        def.raidRadius = 1.0f;

        GovernanceField field;
        field.Bind({}, {def});
        for (int i = 0; i < 10 &&
                        field.GetConvoys()[0].status ==
                            GovernanceField::ConvoyState::Status::Moving;
             ++i) {
            field.Update(0.2f, battle);
        }
        Check(field.GetConvoys()[0].status ==
                  GovernanceField::ConvoyState::Status::Raided,
              "敵輜被劫毀");
        Check(CountOf(battle, GovernanceEvent::ConvoyRaided) == 1,
              "劫敵輜 → ConvoyRaided×1");
        Check(foeFar->GetMorale() < morale0, "敵全軍士氣被打擊");
        Check(std::fabs(foeFar->GetMorale() -
                        (morale0 + GovernanceField::kRaidMoraleHit)) <
                  1e-4f,
              "士氣打擊量正確");
    }

    // ---- [6] 墮落 ratchet（A.5）----
    printf("\n[6] 墮落 ratchet\n");
    {
        Governance gov;
        Check(gov.Depravity() == 0.0f, "墮落初始 0");
        std::unordered_map<GovernanceEvent, int> evs;
        evs[GovernanceEvent::Atrocity] = 2;   // +5×2×0.5 = +5
        evs[GovernanceEvent::VillageBurned] = 1; // +2×0.5 = +1
        gov.Accumulate(evs);
        Check(std::fabs(gov.Depravity() - 6.0f) < 1e-4f,
              "墮落累計正確");
        // 善行不減墮落——ratchet 單向
        std::unordered_map<GovernanceEvent, int> good;
        good[GovernanceEvent::SurrenderAccepted] = 10;
        gov.Accumulate(good);
        Check(gov.Depravity() == 6.0f, "墮落只增不減");
    }

    // ---- [7] phase 閘：部署/結算階段 Update 為 no-op ----
    printf("\n[7] 戰鬥 phase 閘控\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10); // 站村內
        battle.CreateSquad("敵軍", 1, Vector2(9, 9), 10);

        MapConvoy def;
        def.id = "supply-1";
        def.team = 0;
        def.path = {Vector2(0, 0), Vector2(3, 0)};
        def.speed = 2.0f;
        def.hp = 60;
        def.raidRadius = 1.0f;

        GovernanceField field;
        field.Bind({{"village", Vector2(4, 4), 2.0f, ""}}, {def});

        // Deployment 階段：佔村/焚村/護輜全部不該動
        field.Update(0.5f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageOccupied) == 0,
              "部署階段佔村不記帳");
        Check(std::fabs(field.GetConvoys()[0].pos.x - 0.0f) < 1e-4f,
              "部署階段運輸隊不動");

        battle.BeginExecution();
        field.Update(0.1f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageOccupied) == 1,
              "進執行階段佔村記帳");
        field.Update(0.5f, battle);
        Check(std::fabs(field.GetConvoys()[0].pos.x - 1.2f) < 1e-4f,
              "執行階段運輸隊前進");
    }

    // ---- [8] radius<=0 互動物停用 ----
    printf("\n[8] radius<=0 停用點\n");
    {
        BattleController battle(10, 10, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        battle.CreateSquad("敵軍", 1, Vector2(9, 9), 10);
        battle.BeginExecution();

        GovernanceField field;
        // 我軍站在 (4,4) 上,但兩點 radius 皆 <=0——永不觸發
        field.Bind({{"village", Vector2(4, 4), 0.0f, ""},
                    {"village", Vector2(4, 4), -1.0f, ""}},
                   {});
        field.Update(0.1f, battle);
        Check(CountOf(battle, GovernanceEvent::VillageOccupied) == 0,
              "radius<=0 互動物不觸發");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
