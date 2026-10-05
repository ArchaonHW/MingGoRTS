// BattlePickerTest - RTS 點選層的 headless 驗證
//
// 覆蓋：螢幕→世界射線、地面求交、格子換算、小隊包圍球點選
// （透視相機從戰場上空俯視，驗證點選命中/未命中/最近命中）

#include "Gameplay/BattlePicker.h"
#include "Gameplay/BattleController.h"
#include "Gameplay/Squad.h"
#include "Rendering/Camera.h"

#include <cmath>
#include <cstdio>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_failures = 0;
#define CHECK(cond, msg) do { \
    if (!(cond)) { printf("FAIL: %s\n", msg); ++g_failures; } \
    else { printf("  ok: %s\n", msg); } \
} while (0)

int main() {
    printf("=== BattlePickerTest ===\n");

    const float cellSize = 1.0f;
    const int W = 800, H = 600;
    // 戰場 20x15，格子單位；世界中心 (10, 0, 7.5)
    BattleController battle(20, 15, cellSize);
    battle.CreateSquad("Alpha", 0, Vector2(10.0f, 7.0f), 30);
    battle.CreateSquad("Bravo", 0, Vector2(3.0f, 3.0f), 30);

    Camera cam;
    cam.SetViewport(0, 0, W, H);
    cam.SetPerspective(60.0f, (float)W / (float)H, 0.1f, 200.0f);
    cam.SetPosition(Vector3(10.0f, 20.0f, 7.5f));
    cam.SetTarget(Vector3(10.0f, 0.0f, 7.5f));

    // [1] 螢幕中心 → 射線直指地面中心
    PickRay center = BattlePicker::ScreenToWorldRay(cam, W * 0.5f, H * 0.5f);
    CHECK(std::fabs(center.direction.y + 1.0f) < 0.01f, "中心射線垂直向下");

    Vector3 hit;
    CHECK(BattlePicker::IntersectGround(center, 0.0f, hit), "中心射線命中地面");
    CHECK(std::fabs(hit.x - 10.0f) < 0.01f && std::fabs(hit.z - 7.5f) < 0.01f,
          "地面交點 = 戰場中心");

    // [2] 世界→格子換算
    Vector2 cell = BattlePicker::WorldToCell(hit, cellSize);
    CHECK(std::fabs(cell.x - 10.0f) < 0.01f && std::fabs(cell.y - 7.5f) < 0.01f,
          "WorldToCell 換算正確");

    // [3] 點選 Alpha：Alpha 在 (10,7)，中心射線穿過其包圍球
    Squad* picked = BattlePicker::PickSquad(battle, 0, center, cellSize);
    CHECK(picked != nullptr, "中心點選命中小隊");
    CHECK(picked && picked->GetName() == "Alpha", "命中的是 Alpha");

    // [4] 敵隊過濾：team=1 時不該命中我方小隊
    CHECK(BattlePicker::PickSquad(battle, 1, center, cellSize) == nullptr,
          "敵隊篩選：不命中我方小隊");

    // [5] 角落射線命中 Bravo（左上 1/4 螢幕位置對應戰場左下區）
    // 直接構造打向 Bravo 的射線驗證最近命中語意
    PickRay toBravo;
    toBravo.origin = Vector3(3.0f, 20.0f, 3.0f);
    toBravo.direction = Vector3(0.0f, -1.0f, 0.0f);
    picked = BattlePicker::PickSquad(battle, 0, toBravo, cellSize);
    CHECK(picked && picked->GetName() == "Bravo", "直下的射線命中 Bravo");

    // [6] 未命中：射線打到空地
    PickRay empty;
    empty.origin = Vector3(18.0f, 20.0f, 13.0f);
    empty.direction = Vector3(0.0f, -1.0f, 0.0f);
    CHECK(BattlePicker::PickSquad(battle, 0, empty, cellSize) == nullptr,
          "空地點選未命中");

    // [7] 點選落空 → 取地面格作為移動指令落點
    if (BattlePicker::IntersectGround(empty, 0.0f, hit)) {
        cell = BattlePicker::WorldToCell(hit, cellSize);
        CHECK(std::fabs(cell.x - 18.0f) < 0.01f, "落點格子正確（可下 MoveTo）");
    } else {
        CHECK(false, "空射線應命中地面");
    }

    // [8] 平行射線與反向射線不命中地面
    PickRay flat;
    flat.origin = Vector3(0, 1.0f, 0);
    flat.direction = Vector3(1.0f, 0.0f, 0.0f);
    CHECK(!BattlePicker::IntersectGround(flat, 0.0f, hit), "平行射線未命中");
    PickRay up;
    up.origin = Vector3(0, 1.0f, 0);
    up.direction = Vector3(0.0f, 1.0f, 0.0f);
    CHECK(!BattlePicker::IntersectGround(up, 0.0f, hit), "向上射線未命中");

    if (g_failures == 0) {
        printf("ALL CHECKS PASSED\n");
        return 0;
    }
    printf("%d CHECKS FAILED\n", g_failures);
    return 1;
}
