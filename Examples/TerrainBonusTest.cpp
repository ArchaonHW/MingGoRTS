// TerrainBonusTest - G-4 地形加成無頭測試
// 驗收：FlowField 地形層、BattleMap type 欄位解析（缺省 Plain 零回歸）、
//       ResolveCombat 高地攻方 ×1.25 / 森林守方 ×0.7
#include "Gameplay/FlowField.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include <cstdio>
#include <string>

using namespace Potato::Gameplay;
using Potato::Vector2;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { g_pass++; printf("  [PASS] %s\n", name); }
    else    { g_fail++; printf("  [FAIL] %s\n", name); }
}

// 打一場固定時長的 1v1，回傳 {A 剩員, B 剩員}
static void Duel(const BattleMap* map, int& aLeft, int& bLeft) {
    BattleController battle(20, 20, 1.0f);
    if (map) {
        map->ApplyToField(battle.GetField());
    }
    Squad* a = battle.CreateSquad("A", 0, Vector2(5, 5), 20);
    Squad* b = battle.CreateSquad("B", 1, Vector2(6, 5), 20);
    if (!battle.BeginExecution()) {
        aLeft = bLeft = -1;
        return;
    }
    for (int i = 0; i < 100; ++i) { // 10 秒交戰
        battle.Update(0.1f);
    }
    aLeft = a->GetMembers();
    bLeft = b->GetMembers();
}

int main() {
    printf("=== Terrain Bonus Tests (G-4) ===\n\n");

    // [1] FlowField 地形層基礎
    printf("[1] FlowField 地形層\n");
    FlowField field(20, 20, 1.0f);
    Check(field.GetTerrain(5, 5) == TerrainType::Plain, "缺省全 Plain");
    field.SetTerrain(5, 5, TerrainType::Highland);
    field.SetTerrain(7, 3, TerrainType::Forest);
    Check(field.GetTerrain(5, 5) == TerrainType::Highland, "SetTerrain Highland");
    Check(field.GetTerrain(7, 3) == TerrainType::Forest, "SetTerrain Forest");
    Check(field.GetTerrain(-1, 0) == TerrainType::Plain, "越界查詢回 Plain");
    Check(field.TerrainAt(Vector2(5.4f, 5.9f)) == TerrainType::Highland,
          "TerrainAt worldPos 命中");
    Check(field.TerrainAt(Vector2(99, 99)) == TerrainType::Plain,
          "TerrainAt 界外回 Plain");

    // [2] BattleMap type 欄位解析
    printf("\n[2] BattleMap 地形類型解析\n");
    const char* mapJson = R"({
        "name": "terrain_test",
        "grid": {"width": 20, "height": 20, "cellSize": 1.0},
        "terrain": [
            {"rect": [5,5,1,1], "cost": 1.0, "type": "highland", "note": "丘"},
            {"rect": [6,5,1,1], "cost": 1.0, "type": "forest",   "note": "林"},
            {"rect": [8,8,2,2], "cost": 2.0, "type": "mud",      "note": "泥"},
            {"rect": [3,3,1,1], "cost": 1.0,                     "note": "無type"}
        ]
    })";
    BattleMap map;
    Check(map.LoadFromString(mapJson), "地圖 JSON 載入");
    FlowField f2(20, 20, 1.0f);
    map.ApplyToField(f2);
    Check(f2.GetTerrain(5, 5) == TerrainType::Highland, "patch highland 寫入");
    Check(f2.GetTerrain(6, 5) == TerrainType::Forest, "patch forest 寫入");
    Check(f2.GetTerrain(8, 8) == TerrainType::Mud, "patch mud 寫入");
    Check(f2.GetTerrain(9, 9) == TerrainType::Mud, "mud rect 範圍內");
    Check(f2.GetTerrain(3, 3) == TerrainType::Plain, "缺省 type = Plain");

    // [3] 高地攻方 ×1.25：A 在高地、B 在平地 → B 損失大於 A
    printf("\n[3] 高地攻方加成\n");
    const char* highlandMap = R"({
        "name": "h", "grid": {"width": 20, "height": 20, "cellSize": 1.0},
        "terrain": [{"rect": [5,5,1,1], "type": "highland"}]
    })";
    BattleMap hm;
    hm.LoadFromString(highlandMap);
    int baseA, baseB, hA, hB;
    Duel(nullptr, baseA, baseB);
    Duel(&hm, hA, hB);
    printf("  平地對稱: A=%d B=%d | A在高地: A=%d B=%d\n",
           baseA, baseB, hA, hB);
    Check(baseA == baseB, "平地對稱 1v1 損失相等");
    Check(hB < baseB, "高地攻方造成更多傷亡");
    // 高地不直接減攻方受傷，但殺得快 → B 的 DPS 提早衰減，
    // A 受傷只會持平或更少，不可能更多
    Check(hA >= baseA, "高地不讓攻方受更多傷");

    // [4] 森林守方 ×0.7：B 在森林 → B 損失小於平地
    printf("\n[4] 森林守方減傷\n");
    const char* forestMap = R"({
        "name": "f", "grid": {"width": 20, "height": 20, "cellSize": 1.0},
        "terrain": [{"rect": [6,5,1,1], "type": "forest"}]
    })";
    BattleMap fm;
    fm.LoadFromString(forestMap);
    int fA, fB;
    Duel(&fm, fA, fB);
    printf("  B在森林: A=%d B=%d (基準 B=%d)\n", fA, fB, baseB);
    Check(fB > baseB, "森林守方剩員更多（受傷減少）");
    // 森林不直接影響守方輸出，但守方活更久 → 輸出窗口更長，
    // A 剩員只會持平或更少
    Check(fA <= baseA, "森林不削弱守方輸出");

    printf("\n=== %d passed, %d failed ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
