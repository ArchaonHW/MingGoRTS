#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector3.h"

#include <unordered_map>
#include <vector>

namespace Potato {

class SceneGraph;
class SceneNode;
class Mesh;
class RenderableComponent;
namespace Quasi { class TurbulenceField; class GustField; }

namespace Gameplay {

class BattleController;
class Squad;
class QuantumFog;

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

    // ---- 敵情霧(Q-1)----
    // 綁定後:未揭露的 bound squad 節點隱藏,改在 parentNode 下顯示
    // 機率雲標記(__fog_e<id> 節點,每候選一子節點,scale∝機率)。
    void SetFog(const QuantumFog* fog);
    void SetFogMarkerMesh(SharedPtr<Mesh> mesh);
    // P-1 湍流漂移：雲標記位置疊加無散度微擾（場連續→平滑不回跳）。
    // strength 是世界單位偏移量級；0（預設）= 行為與無場一致。
    // field 生命週期由呼叫端持有；nullptr 關閉。
    void SetFogDrift(const Quasi::TurbulenceField* field, float strength);
    // P-4 間歇陣風（可選）：漂移量再乘 GustField::Intensity——
    // 雲漂移忽強忽弱而非恆速。nullptr（預設）= 不調製。
    void SetFogDriftGust(const Quasi::GustField* gust);

    // D-5 滲透視效（UX-DR5，唯讀消費——呼叫端從 SeepageStage 取值注入，
    // sync 不寫滲透狀態）：
    // tint——敵情雲底色 RGB 加算偏移（等級1 霧色偏移）；
    // jitterAmp——雲標記定域正弦微移（等級2 物件微移；無亂數、
    //   相位依候選索引，回放可重現）；
    // shadowOffset——雲標記 +x 向靜態錯位（陰影錯位感）。
    // 全零（預設）= 行為與無滲透一致。
    void SetSeepageFX(const Vector3& tint, float jitterAmp,
                      float shadowOffset);

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
    SceneNode* EnsureFogNode(int entityId);
    static Vector3 TeamColor(int team);

    std::vector<Binding> bindings;
    std::unordered_map<int, SharedPtr<SceneNode>> fogNodes;
    SharedPtr<SceneNode> parentNode;
    SceneGraph* scene = nullptr;
    SharedPtr<Mesh> unitMesh;
    SharedPtr<Mesh> ringMesh, barBgMesh, barFillMesh;
    const QuantumFog* fog = nullptr;
    SharedPtr<Mesh> fogMarkerMesh;
    const Quasi::TurbulenceField* fogDrift = nullptr;
    float fogDriftStrength = 0.0f;
    const Quasi::GustField* fogGust = nullptr; // P-4 陣風調製
    Vector3 seepageTint;     // D-5 滲透霧色偏移
    float seepageJitter = 0.0f; // D-5 微移幅度
    float seepageShadow = 0.0f; // D-5 陰影錯位
    const Squad* selected = nullptr;
    float barY = 2.0f;
    float barWidth = 1.2f;
    float cellSize = 1.0f;
};

} // namespace Gameplay
} // namespace Potato
