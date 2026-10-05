#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector3.h"

#include <vector>

namespace Potato {

class SceneGraph;
class SceneNode;
class Mesh;
class RenderableComponent;

namespace Gameplay {

class BattleController;
class Squad;

/**
 * 戰鬥→場景同步器(E4:Gameplay 接上場景圖/渲染管線)
 *
 * Attach 為每個 Squad 建立 SceneNode + RenderableComponent(隊伍配色),
 * Sync 每幀把 squad 位置(grid × cellSize)與狀態寫回節點:
 *   - 位置 → localPosition(XZ 平面)
 *   - 全滅 → SetActive(false)(自動從視錐收集/渲染中消失)
 *   - 潰逃/低士氣 → 顏色壓暗
 * mesh 由呼叫方提供(SetUnitMesh),不設時節點仍可被遍歷但不產生 draw item。
 */
class BattleSceneSync {
public:
    BattleSceneSync() = default;
    ~BattleSceneSync() { Detach(); }

    // 為 battle 現有全部 squad 建立場景節點,掛在 scene 根節點下
    void Attach(BattleController& battle, SceneGraph& scene, float cellSize);

    // 每幀呼叫:同步位置/存亡/士氣到節點;新 squad(Attach 後建立的)自動補掛
    void Sync(BattleController& battle);

    // 設定共用單位 mesh(可為 nullptr 清除)
    void SetUnitMesh(SharedPtr<Mesh> mesh);

    void Detach();

    size_t BindingCount() const { return bindings.size(); }
    SceneNode* GetNodeFor(const Squad* squad) const;

private:
    struct Binding {
        Squad* squad;
        SharedPtr<SceneNode> node;
        SharedPtr<RenderableComponent> renderable;
    };

    void CreateBinding(Squad* squad);
    static Vector3 TeamColor(int team);

    std::vector<Binding> bindings;
    SharedPtr<SceneNode> parentNode;
    SceneGraph* scene = nullptr;
    SharedPtr<Mesh> unitMesh;
    float cellSize = 1.0f;
};

} // namespace Gameplay
} // namespace Potato
