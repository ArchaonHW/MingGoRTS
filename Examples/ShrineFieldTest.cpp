// ShrineFieldTest - D-2 神社與精靈實體無頭測試
// 驗收：佔領觸發待互動、選擇入帳（事件+favor 回呼）、
//       每節點只結算一次、敵隊/潰逃隊不觸發、非 shrine 忽略、
//       缺 spirit 泛稱境靈、視覺態唯讀、schema 相容
#include "Gameplay/BattleController.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/ShrineField.h"
#include "Gameplay/Squad.h"
#include "Campaign/MythLayer.h"

#include <cstdio>
#include <string>
#include <vector>

using namespace Potato;
using namespace Potato::Gameplay;
using Potato::Campaign::MythLayer;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

static MapInteractable MkShrine(const std::string& spirit,
                                float x, float y) {
    MapInteractable it;
    it.type = "shrine";
    it.pos = Vector2(x, y);
    it.radius = 2.0f;
    it.spirit = spirit;
    it.offering = "鮮花素果";
    return it;
}

int main() {
    setvbuf(stdout, nullptr, _IONBF, 0);
    printf("=== ShrineField Tests (D-2) ===\n\n");

    // ---- [1] 佔領觸發：我軍進圈 → Occupied + 發現事件一筆 ----
    printf("[1] 佔領觸發\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        std::vector<std::string> events;
        field.SetEventCallback(
            [&events](const std::string& m) { events.push_back(m); });

        Check(field.GetShrines().size() == 1, "Bind 收一個神社");
        Check(field.GetShrines().at(0).visual == ShrineVisual::Idle,
              "初態 Idle");
        field.Update(battle);
        Check(field.IsPending(0), "進駐後待互動");
        Check(events.size() == 1, "發現事件一筆");
        Check(events.at(0).find("土地公") != std::string::npos,
              "事件含神祇名");
    }

    // ---- [2] 重複佔領不重發 ----
    printf("\n[2] 重複佔領不重發\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        int count = 0;
        field.SetEventCallback(
            [&count](const std::string&) { ++count; });
        field.Update(battle);
        field.Update(battle);
        field.Update(battle);
        Check(count == 1, "發現事件只發一次");
    }

    // ---- [3] 敵隊進圈不觸發 ----
    printf("\n[3] 敵隊不觸發\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("敵軍", 1, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        field.Update(battle);
        Check(!field.IsPending(0), "敵隊不觸發互動");
        Check(field.GetShrines().at(0).visual == ShrineVisual::Idle,
              "仍 Idle");
    }

    // ---- [3b] 潰逃隊進圈不觸發 ----
    printf("\n[3b] 潰逃隊不觸發\n");
    {
        BattleController battle(16, 16, 1.0f);
        Squad* sq = battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        sq->AdjustMorale(-1.0f); // 士氣歸零 → routing
        Check(sq->IsRouting(), "小隊已潰逃");
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        field.Update(battle);
        Check(!field.IsPending(0), "潰逃隊不觸發互動");
    }

    // ---- [4] 選擇入帳：favor 回呼 + 事件字串 ----
    printf("\n[4] 選擇入帳\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("狐仙", 4, 4)});
        std::vector<std::string> events;
        std::vector<std::pair<std::string, float>> favors;
        field.SetEventCallback(
            [&events](const std::string& m) { events.push_back(m); });
        field.SetFavorCallback(
            [&favors](const std::string& s, float d) {
                favors.push_back({s, d});
            });
        field.Update(battle);

        Check(field.ApplyChoice(0, ShrineChoice::Provoke),
              "挑釁受理");
        Check(favors.size() == 1, "favor 回呼一筆");
        Check(favors.at(0).first == "狐仙", "favor 對象正確");
        Check(favors.at(0).second == -ShrineField::kProvokeFavor,
              "挑釁為負 delta");
        Check(events.size() == 2, "發現+選擇共兩筆事件");
        Check(events.at(1).find("挑釁") != std::string::npos,
              "選擇事件含動作名");
        Check(events.at(1).find("狐仙") != std::string::npos,
              "選擇事件含神祇名");
        Check(field.GetShrines().at(0).visual == ShrineVisual::Provoked,
              "視覺態轉 Provoked");
        Check(!field.IsPending(0), "結算後不待互動");
    }

    // ---- [4b] 非法輸入拒絕：OOB 索引 / 非法 enum ----
    printf("\n[4b] 非法輸入拒絕\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        int cb = 0;
        field.SetEventCallback([&cb](const std::string&) { ++cb; });
        field.SetFavorCallback(
            [&cb](const std::string&, float) { ++cb; });
        field.Update(battle); // 發現事件一筆
        Check(field.IsPending(0), "待互動");
        Check(!field.ApplyChoice(99, ShrineChoice::Appease),
              "OOB mapIndex 拒絕");
        Check(!field.ApplyChoice(0, static_cast<ShrineChoice>(99)),
              "非法 enum 拒絕");
        Check(field.IsPending(0), "非法選擇不結算節點");
        Check(cb == 1, "非法選擇不發回呼（僅發現事件）");
    }

    // ---- [5] 未佔領/重複選擇拒絕 ----
    printf("\n[5] 非法選擇拒絕\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(9, 9), 10); // 遠離
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        int cb = 0;
        field.SetEventCallback([&cb](const std::string&) { ++cb; });
        field.SetFavorCallback(
            [&cb](const std::string&, float) { ++cb; });
        field.Update(battle);
        Check(!field.ApplyChoice(0, ShrineChoice::Appease),
              "未佔領拒絕");
        Check(cb == 0, "拒絕不發回呼");

        // 佔領後結算一次，再選拒絕
        battle.CreateSquad("增援", 0, Vector2(4, 4), 10);
        field.Update(battle);
        Check(field.ApplyChoice(0, ShrineChoice::Offer), "獻祭受理");
        Check(!field.ApplyChoice(0, ShrineChoice::Appease),
              "已結算再選拒絕");
        Check(field.GetShrines().at(0).visual == ShrineVisual::Offered,
              "視覺態維持 Offered");
    }

    // ---- [6] 缺 spirit → 境靈泛稱 ----
    printf("\n[6] 缺 spirit 泛稱\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        ShrineField field;
        field.Bind({MkShrine("", 4, 4)});
        std::string lastFavorSpirit;
        float lastDelta = 0.0f;
        field.SetFavorCallback(
            [&](const std::string& s, float d) {
                lastFavorSpirit = s;
                lastDelta = d;
            });
        field.Update(battle);
        Check(field.ApplyChoice(0, ShrineChoice::Appease),
              "無名神社受理");
        Check(lastFavorSpirit == "境靈", "泛稱境靈");
        Check(lastDelta == ShrineField::kAppeaseFavor,
              "安撫為正 delta");
        Check(field.GetShrines().at(0).visual == ShrineVisual::Appeased,
              "視覺態轉 Appeased");
    }

    // ---- [7] 非 shrine 互動物忽略 + schema 相容 ----
    printf("\n[7] 非 shrine 忽略 + schema\n");
    {
        BattleMap map;
        const char* json = R"({
            "format": "potato.battle_map/1",
            "grid": {"width": 16, "height": 16, "cellSize": 1},
            "interactables": [
                {"type": "village", "pos": [2, 2], "radius": 2},
                {"type": "shrine", "pos": [4, 4], "radius": 2,
                 "spirit": "城隍", "offering": "三牲"},
                {"type": "shrine", "pos": [8, 8], "radius": 1}
            ]
        })";
        Check(map.LoadFromString(json), "含新欄位圖載入");
        Check(map.GetInteractables().size() == 3, "三個互動物入圖");
        Check(map.GetInteractables().at(1).spirit == "城隍",
              "spirit 解析");
        Check(map.GetInteractables().at(1).offering == "三牲",
              "offering 解析");
        Check(map.GetInteractables().at(2).spirit.empty(),
              "缺 spirit 為空");

        ShrineField field;
        field.Bind(map.GetInteractables());
        Check(field.GetShrines().size() == 2, "只收 shrine 類");
        Check(field.GetShrines().at(0).mapIndex == 1,
              "mapIndex 對帳原圖索引");
        Check(field.GetShrines().at(0).offering == "三牲",
              "供品態可讀");
        Check(field.GetShrine(1) == &field.GetShrines().at(0),
              "GetShrine(mapIndex) 定址");
        Check(field.GetShrine(0) == nullptr, "非 shrine 索引回空");
    }

    // ---- [8] favor 回呼接線 MythLayer（GodStance C.4 介面）----
    printf("\n[8] 接線 MythLayer favor\n");
    {
        BattleController battle(16, 16, 1.0f);
        battle.CreateSquad("我軍", 0, Vector2(4, 4), 10);
        MythLayer myth;
        ShrineField field;
        field.Bind({MkShrine("土地公", 4, 4)});
        // 呼叫端接線：favor delta → MythLayer（D-1 預留欄位）
        field.SetFavorCallback(
            [&myth](const std::string& s, float d) {
                myth.AdjustFavor(s, d);
            });
        field.Update(battle);
        Check(field.ApplyChoice(0, ShrineChoice::Offer), "獻祭受理");
        Check(myth.Favor("土地公") ==
                  MythLayer::kNeutralFavor + ShrineField::kOfferFavor,
              "favor 寫入 MythLayer");
    }

    printf("\n=== %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
