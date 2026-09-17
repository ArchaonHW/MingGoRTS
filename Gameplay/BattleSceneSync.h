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

    // ---- overlay(A-4):選取環 + 血條 ----
    // 三個 mesh 皆以「寬 1 單位、原點居中」製作,由 sync 負責版面:
    //   ring    — 平放環,選取時顯示於小隊腳下(y≈0.22,高過橋板/水面)
    //   barBg   — 血條底(常駐深色),掛小隊節點上方 barY 處,寬 barWidth
    //   barFill — 血條量條,sync 依 healthPct 縮放 scale.x 並向左對齊,
    //             顏色綠→紅漸變;小隊潰逃或全滅時整條隱藏
    // 傳 nullptr 清除對應 overlay;重設 mesh 或 barY/barWidth 會更新既有節點。
    void SetOverlayMeshes(SharedPtr<Mesh> ring, SharedPtr<Mesh> barBg,
                          SharedPtr<Mesh> barFill, float barY = 2.0f,
                          float barWidth = 1.2f);

    // 選取的小隊(顯示選取環);nullptr 取消選取
    void SetSelectedSquad(const Squad* squad);
    const Squad* GetSelectedSquad() const { return selected; }

    void Detach();

    size_t BindingCount() const { return bindings.size(); }
    SceneNode* GetNodeFor(const Squad* squad) const;

private:
    struct Binding {
        Squad* squad;
        SharedPtr<SceneNode> node;
        SharedPtr<RenderableComponent> renderable;
        // overlay 子節點(SetOverlayMeshes 後建立,命名 "__sel_ring" 等)
        SharedPtr<SceneNode> ringNode;
        SharedPtr<SceneNode> barBgNode;
        SharedPtr<SceneNode> barFillNode;
        SharedPtr<RenderableComponent> barFillRc;
    };

    void CreateBinding(Squad* squad);
    void EnsureOverlay(Binding& b);
    static Vector3 TeamColor(int team);

    std::vector<Binding> bindings;
    SharedPtr<SceneNode> parentNode;
    SceneGraph* scene = nullptr;
    SharedPtr<Mesh> unitMesh;
    SharedPtr<Mesh> ringMesh, barBgMesh, barFillMesh;
    const Squad* selected = nullptr;
    float barY = 2.0f;
    float barWidth = 1.2f;
    float cellSize = 1.0f;
};

} // namespace Gameplay
} // namespace Potato
