// BattleSceneTest - E4 戰鬥→場景同步驗證(無頭)
// 覆蓋:Squad→SceneNode 綁定、grid→世界座標換算、Sync 每幀更新、
//       全滅自動隱藏、士氣調色、SetUnitMesh 進 render list、新 squad 補掛

#include "Gameplay/BattleController.h"
#include "Gameplay/BattleSceneSync.h"
#include "Gameplay/QuantumFog.h"
#include "Scene/SceneNode.h"
#include "Rendering/SceneRenderer.h"
#include "Rendering/RenderableComponent.h"
#include "Rendering/OpenGLRenderer.h" // Mesh(GL 惰性建立)
#include "Rendering/Camera.h"

#include <cstdio>
#include <cmath>

using namespace Potato;
using namespace Potato::Gameplay;

static int g_pass = 0;
static int g_fail = 0;

static void Check(bool ok, const char* name, float value = 0.0f, float expected = 0.0f) {
    if (ok) {
        g_pass++;
        printf("  [PASS] %s\n", name);
    } else {
        g_fail++;
        printf("  [FAIL] %s  (got %.4f, expected %.4f)\n", name, value, expected);
    }
}

int main() {
    printf("=== Battle Scene Sync Tests ===\n\n");

    const float CELL = 2.0f;
    BattleController battle(20, 15, CELL);
    Squad* blue = battle.CreateSquad("藍軍一隊", 0, Vector2(2, 2), 10);
    Squad* red  = battle.CreateSquad("紅軍一隊", 1, Vector2(17, 12), 10);

    SceneGraph scene;
    auto root = MakeShared<SceneNode>("root");
    scene.SetRootNode(root);

    BattleSceneSync sync;
    sync.Attach(battle, scene, CELL);
    sync.Sync(battle);

    // [1] 兩個 squad 都產生場景節點
    Check(sync.BindingCount() == 2, "兩個 squad 各建一節點",
          static_cast<float>(sync.BindingCount()), 2.0f);

    // [2] grid 座標 × cellSize → 世界座標
    SceneNode* blueNode = sync.GetNodeFor(blue);
    Check(blueNode != nullptr, "藍軍節點存在");
    if (blueNode) {
        Vector3 wp = blueNode->GetWorldPosition();
        Check(std::fabs(wp.x - 4.0f) < 1e-4f && std::fabs(wp.z - 4.0f) < 1e-4f,
              "grid(2,2)*2 = 世界(4,4)", wp.x, 4.0f);
    }

    // [3] Sync 追蹤移動:改 squad 位置後節點跟上
    blue->IssueOrder(SquadOrder::MoveTo, Vector2(8, 8));
    // 直接推 squad(不經 battle.Update,隔離測同步層)
    for (int i = 0; i < 30; ++i) blue->Update(1.0f / 30.0f, nullptr);
    sync.Sync(battle);
    if (blueNode) {
        Vector3 wp = blueNode->GetWorldPosition();
        Check(wp.x > 4.1f, "移動後節點跟進", wp.x, 5.0f);
    }

    // [4] 全滅 → 節點自動隱藏(從視錐收集消失)
    red->ApplyCasualties(99);
    sync.Sync(battle);
    SceneNode* redNode = sync.GetNodeFor(red);
    Check(redNode && !redNode->IsActive(), "全滅 squad 節點隱藏");

    // [5] 士氣映射顏色:滿士氣亮,低士氣暗
    RenderableComponent* rc = blueNode ? blueNode->GetRenderable() : nullptr;
    Check(rc != nullptr, "節點有 renderable");
    if (rc) {
        Vector3 fullColor = rc->color;
        Check(fullColor.z > 0.8f, "藍隊顏色為藍色系", fullColor.z, 0.9f);
    }

    // [6] 補掛:Attach 後新建的 squad 在下次 Sync 自動上場景
    Squad* late = battle.CreateSquad("增援隊", 0, Vector2(5, 5), 8);
    sync.Sync(battle);
    Check(sync.GetNodeFor(late) != nullptr, "新增 squad 自動補掛");

    // [7] 接上渲染管線:設 mesh 後可見節點進 render list
    sync.SetUnitMesh(MakeShared<Mesh>());
    Camera cam;
    cam.SetPosition(Vector3(20, 40, 20));
    cam.SetTarget(Vector3(20, 0, 15));
    cam.SetPerspective(55.0f * 3.14159265f / 180.0f, 16.0f / 9.0f, 0.1f, 200.0f);

    SceneRenderer renderer;
    auto items = renderer.CollectRenderList(scene, cam);
    // 可見的應是藍軍 + 增援隊(紅軍已隱藏);相機俯視戰場中央,兩隊都應在視錐內
    Check(items.size() == 2, "render list = 存活可見小隊數",
          static_cast<float>(items.size()), 2.0f);

    // [8] overlay(A-4):設定環/血條 mesh → 每 binding 長出三個子節點
    sync.SetOverlayMeshes(MakeShared<Mesh>(), MakeShared<Mesh>(),
                          MakeShared<Mesh>());
    sync.Sync(battle);
    auto findChild = [](SceneNode* n, const char* name) -> SceneNode* {
        if (!n) return nullptr;
        for (auto& c : n->GetChildren()) {
            if (c->GetName() == name) return c.get();
        }
        return nullptr;
    };
    SceneNode* blueRing  = findChild(blueNode, "__sel_ring");
    SceneNode* blueBarBg = findChild(blueNode, "__hp_bg");
    SceneNode* blueFill  = findChild(blueNode, "__hp_fill");
    Check(blueRing && blueBarBg && blueFill,
          "小隊節點長出 __sel_ring/__hp_bg/__hp_fill");

    // [9] 選取環只在選中時顯示;全滅小隊血條隱藏
    sync.SetSelectedSquad(blue);
    sync.Sync(battle);
    Check(blueRing && blueRing->IsActive(), "選取後 ring 顯示");
    SceneNode* redBarBg = findChild(redNode, "__hp_bg");
    Check(redBarBg && !redBarBg->IsActive(), "全滅小隊血條隱藏");

    // [10] 血條隨兵力縮短 + 向紅漸變(藍隊 10→5,fill scale.x=barWidth*0.5)
    blue->ApplyCasualties(5);
    sync.Sync(battle);
    if (blueFill) {
        Check(std::fabs(blueFill->GetLocalScale().x - 0.6f) < 0.05f,
              "血量 50% → 血條縮半", blueFill->GetLocalScale().x, 0.6f);
        RenderableComponent* frc = blueFill->GetRenderable();
        Check(frc && frc->color.x > 0.4f, "血條向紅漸變",
              frc ? frc->color.x : 0.0f, 0.5f);
    }

    // [11] SetOverlayMeshes 傳 nullptr 會拆掉既有 overlay 節點
    sync.SetOverlayMeshes(nullptr, nullptr, nullptr, 2.0f, 1.2f);
    Check(findChild(blueNode, "__sel_ring") == nullptr &&
          findChild(blueNode, "__hp_bg") == nullptr &&
          findChild(blueNode, "__hp_fill") == nullptr,
          "傳 nullptr 清除 overlay 節點");

    // [12] Q-1 敵情霧:未揭露的 bound squad 藏真身,機率雲標記掛 __battle_units
    QuantumFog fog(30.0f, 1);
    Squad* ghost = battle.CreateSquad("幻影隊", 1, Vector2(10, 10), 10);
    const int gid = fog.AddEntity("幻影隊", /*觀測方=*/0,
                                  {Vector2(9, 10), Vector2(11, 10)},
                                  {0.5, 0.5});
    battle.BindFog(&fog);
    battle.BindFogSquad(ghost, gid);
    sync.SetFog(&fog);
    sync.Sync(battle);
    SceneNode* ghostNode = sync.GetNodeFor(ghost);
    Check(ghostNode && !ghostNode->IsActive(), "未揭露敵軍節點隱藏");
    SceneNode* units = findChild(root.get(), "__battle_units");
    SceneNode* fogNode = units ? findChild(units, "__fog_e0") : nullptr;
    Check(fogNode && fogNode->IsActive() &&
              fogNode->GetChildren().size() == 2,
          "機率雲標記節點(2 候選)");

    // [13] 揭露 → 真身現、雲標記隱;過期 → 回雲;全滅 → 連雲一起藏
    fog.Reveal(gid, ghost->GetPosition());
    sync.Sync(battle);
    Check(ghostNode && ghostNode->IsActive(), "揭露後節點恢復顯示");
    Check(fogNode && !fogNode->IsActive(), "揭露後雲標記隱藏");
    fog.Update(40.0f); // 情報時效 30s,過期回疊加
    sync.Sync(battle);
    Check(fogNode && fogNode->IsActive(), "時效過期雲標記復活");
    ghost->ApplyCasualties(99);
    sync.Sync(battle);
    Check(fogNode && !fogNode->IsActive(), "全滅後雲標記隱藏");

    sync.Detach();
    Check(sync.BindingCount() == 0, "Detach 清空綁定");

    printf("\n=== 結果: %d PASS, %d FAIL ===\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
