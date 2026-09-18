// MapGenTest - G-10 程序戰場生成驗證（無頭）
//
// 覆蓋：同 seed 逐位一致、異 seed 可辨差異、BattleMap loader 接受、
//       河道/渡口通行性、載入後可打一場（時間推進且有交戰或位移）。

#include "Gameplay/MapGenerator.h"
#include "Gameplay/BattleMap.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Doctrine.h"
#include "Gameplay/Squad.h"

#include <cmath>
#include <cstdio>
#include <string>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0, g_fail = 0;
static void Check(bool ok, const char* name) {
    if (ok) { ++g_pass; printf("  [PASS] %s\n", name); }
    else    { ++g_fail; printf("  [FAIL] %s\n", name); }
}

int main() {
    printf("=== MapGen Test (G-10) ===\n\n");

    // [1] 同 seed 逐位一致
    const std::string a1 = MapGenerator::GenerateJson(42);
    const std::string a2 = MapGenerator::GenerateJson(42);
    Check(a1 == a2, "同 seed 產出逐位一致");

    // [2] 異 seed 產出差異
    const std::string b1 = MapGenerator::GenerateJson(7);
    Check(a1 != b1, "異 seed 產出可辨差異");

    // [3] BattleMap loader 接受
    BattleMap map;
    Check(map.LoadFromString(a1), "產出 JSON 通過 BattleMap loader");
    Check(map.GetGridWidth() == 24 && map.GetGridHeight() == 16,
          "網格尺寸正確");
    Check(!map.GetFords().empty(), "有渡口");
    Check(map.FindPin("北岸集結點") != nullptr &&
          map.FindPin("南岸敵營") != nullptr,
          "集結點/敵營圖釘存在");

    // [4] 河道擋路、渡口可通行
    BattleController battle(map.GetGridWidth(), map.GetGridHeight(),
                            map.GetCellSize());
    map.ApplyToField(battle.GetField());
    {
        const MapZone& ford = map.GetFords().front();
        const int fx = ford.rect.x + ford.rect.w / 2;
        const int fy = ford.rect.y + ford.rect.h / 2;
        Check(!battle.GetField().IsBlocked(fx, fy), "渡口格可通行");
        // 渡口旁的河道格應擋路（找一格非渡口的水格）
        bool foundWater = false;
        for (int x = 0; x < map.GetGridWidth() && !foundWater; ++x) {
            if (x >= ford.rect.x && x < ford.rect.x + ford.rect.w)
                continue;
            if (battle.GetField().IsBlocked(x, fy)) foundWater = true;
        }
        Check(foundWater, "河道其餘格擋路");
    }

    // [5] 可打一場：雙方部署 → doctrine → 時間推進且有行動
    {
        const MapPin* rally = map.FindPin("北岸集結點");
        const MapPin* camp = map.FindPin("南岸敵營");
        Squad* t0a = battle.CreateSquad("前鋒", 0, Vector2(8, 2), 20);
        Squad* t0b = battle.CreateSquad("中軍", 0, Vector2(12, 2), 20);
        Squad* t1a = battle.CreateSquad("敵前鋒", 1, Vector2(8, 13), 20);
        battle.CreateSquad("敵中軍", 1, Vector2(12, 13), 20);
        if (camp)  battle.SetObjective(0, camp->pos);
        if (rally) battle.SetObjective(1, rally->pos);
        if (rally) battle.SetRallyPoint(0, rally->pos);
        if (camp)  battle.SetRallyPoint(1, camp->pos);

        DoctrineSet rush;
        rush.AddRule({DoctrineTrigger::Always,
                      DoctrineAction::AdvanceToObjective, 0.0f, 1});
        for (Squad* s : {t0a, t0b, t1a})
            battle.AssignDoctrine(s, rush);
        Check(battle.BeginExecution(), "程序地圖可開戰");

        const Vector2 start = t0a->GetPosition();
        for (int i = 0; i < 600; ++i) battle.Update(0.1f);
        const bool acted =
            (t0a->GetPosition() - start).Length() > 0.5f ||
            t0a->IsEngaged() || battle.GetOutcome() != BattleOutcome::Ongoing;
        Check(battle.GetElapsed() > 0.0f, "戰鬥時間推進");
        Check(acted, "小隊有行動（位移/交戰/分出勝負）");
    }

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
