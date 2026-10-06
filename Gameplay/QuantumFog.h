#pragma once

#include "MathUtils/Vector2.h"
#include "Quantum/Qudit.h"

#include <functional>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleResources;

/**
 * 不確定實體：一支敵軍在玩家視角中的疊加態
 *
 * 玩家看到的不是單一位置，而是候選位置上的機率雲。
 * 花費情報點「觀測」→ 態塌縮 → 顯示真實位置（情報有時效，
 * 過期後重新回到疊加態——量子退相干的反向運用）。
 */
struct UncertainEntity {
    std::string name;
    int team = 1;
    std::vector<Vector2> candidates;   // 候選位置（格子座標）
    std::vector<double> priors;        // 各候選的先驗機率（人格/地形推得）
    Quantum::Qudit state;              // 疊加態（|a_i|^2 = 機率）
    bool revealed = false;
    Vector2 revealedPos;               // 塌縮後的真實位置
    float revealTimer = 0.0f;          // 情報時效倒數（秒）
    std::vector<double> phases;        // Q-7 候選相位（空 = 無相位資訊）
    double lastObserveAt = -1.0;       // 上次情報觀測的 fog 時鐘（干涉判定用）

    UncertainEntity(int dim, uint64_t seed)
        : state(dim, seed) {}
};

/**
 * 量子敵情霧
 *
 * 把「寫作戰鬥」的資訊不確定性量化成疊加態：
 *   - 未觀測：敵軍以機率雲形式存在於多個候選位置
 *   - 觀測（花情報點）：態塌縮，揭露真實位置
 *   - 時效：情報過期後機率雲重新擴散（Diffuse 回先驗）
 *
 * 渲染層讀 GetCloud() 畫機率雲；邏輯層用 IsRevealed()/GetRevealedPos()
 * 決定 UI 是否顯示確切數值。
 */
class QuantumFog {
public:
    // intelCostPerObserve：每次觀測的情報點花費
    // intelDuration：情報時效（秒），0 = 永久揭露
    // intelCostPerProbe：弱觀測花費（Q-3），須 < intelCostPerObserve
    explicit QuantumFog(float intelDuration = 30.0f, int intelCostPerObserve = 1,
                        int intelCostPerProbe = 1);

    void BindResources(BattleResources* res) { this->resources = res; }

    // Q-6：觀測/塌縮/情報過期等態變化發事件（BattleController 接到
    // Emit → BattleRecorder 可錄）。回放時間軸因此能重現雲變化。
    void SetEventCallback(std::function<void(const std::string&)> cb) {
        onEvent = std::move(cb);
    }

    // 註冊一支敵軍的疊加態。priors 長度須等於 candidates；空則均勻。
    int AddEntity(const std::string& name, int team,
                  const std::vector<Vector2>& candidates,
                  const std::vector<double>& priors = {});

    // 藍噪候選格生成：在 suspectedCenter 周圍 radius 內產生至多 count
    // 個最小間距 minSpacing 的候選點（Poisson-disk 散佈，避免機率雲
    // 叢聚）。biasPoint 不為 nullptr 時，先驗機率依與其距離做高斯
    // 遞減（σ = radius/2）；否則以 suspectedCenter 為中心。
    // 產生失敗（候選 < 2）時退回 R2 準隨機圓盤散佈。
    // priorScale（Q-4）：先驗高斯的 σ 縮放（= radius/2·scale），
    // <1 更集中、>1 更鬆散；<=0 視為 1.0。
    // 回傳 entityId，失敗回 -1。
    int AddEntityCloud(const std::string& name, int team,
                       Vector2 suspectedCenter, float radius, int count,
                       float minSpacing,
                       const Vector2* biasPoint = nullptr,
                       float priorScale = 1.0f);

    // 觀測：扣情報點 → 塌縮到最接近 truePos 的候選 → 揭露真實位置。
    // 情報不足或實體不存在回傳 false。
    bool Observe(int entityId, const Vector2& truePos);

    // 免情報點揭露（物理偵查：小隊目視接觸）。已揭露者僅更新位置。
    bool Reveal(int entityId, const Vector2& truePos);

    // 負面觀測：把某候選格機率歸零並重新歸一化
    // （小隊目視覆蓋該格但沒看到人——「這裡沒有敵軍」也是情報）。
    // 已揭露/實體或索引無效/候選已為零/只剩最後一個非零候選時回 false。
    bool EliminateCandidate(int entityId, int candidateIndex);

    // 弱觀測（Q-3）：扣 probeCost 情報，機率雲向 truePos 近端重加權
    // 收縮但不塌縮（revealed 不變）。strength∈(0,1] 愈強收縮愈多；
    // 情報不足/已揭露（回 true 不扣點）/strength<=0/無效 id 處理見實作。
    bool Probe(int entityId, const Vector2& truePos, float strength);

    // 純 Born-rule 觀測（無真值來源時）：回傳命中的候選 index，-1 表示失敗
    int ObserveRandom(int entityId);

    // 糾纏（Q-2）：兩實體疊加態相關——一方塌縮時，未揭露的另一方
    // 機率向「同向候選」（雲質心相對偏移最近者）集中 0.7，但不揭露。
    // 1:1、雙向對稱；a==b/無效 id/任一方已糾纏回 false。
    bool Entangle(int a, int b);
    // 糾纏對象的 entityId；-1 = 未糾纏或無效
    int EntangledPartner(int entityId) const;

    // ---- Q-7 相位干涉（實驗,預設關閉） ----
    // 注入候選相位（長度不足補 0、超出截斷）；同 tick 對同一實體
    // 二次觀測時,新目標候選與當前主峰的相位差 cos<0 → 觀測被拒,
    // cos>=0 → 刷新並加成時效。flag 關閉時行為與 Q-1 完全一致。
    bool SetEntityPhases(int entityId, const std::vector<double>& phases);
    void SetInterferenceEnabled(bool on) { interference = on; }
    bool InterferenceEnabled() const { return interference; }

    void Update(float dt);

    // ---- Q-6 疊加態存檔（potato.quantum_fog/1）----
    // 含 candidates/priors/振幅(re,im)/revealed/timer/糾纏表/seedCounter
    bool SaveToFile(const std::string& path) const;
    bool LoadFromFile(const std::string& path);

    // ---- 查詢 ----
    bool IsRevealed(int entityId) const;
    Vector2 GetRevealedPos(int entityId) const;
    // 機率雲：[(候選位置, 機率)]；已揭露者回傳 [(真實位置, 1.0)]
    std::vector<std::pair<Vector2, double>> GetCloud(int entityId) const;
    const UncertainEntity* GetEntity(int entityId) const;
    size_t EntityCount() const { return entities.size(); }

private:
    // 回傳塌縮到的候選 index
    int CollapseNear(UncertainEntity& e, const Vector2& truePos);
    // 糾纏傳遞：entityId 塌縮到 candIdx 時,集中其未揭露糾纏對象
    void PropagateEntanglement(int entityId, int candIdx);
    int NearestCandidate(const UncertainEntity& e,
                         const Vector2& pos) const;
    int ModalCandidate(const UncertainEntity& e) const;

    struct EntangleLink {
        int a, b;
        std::vector<int> a2b; // A 候選 i → B 相關候選
        std::vector<int> b2a;
    };

    void Emit(const std::string& msg) const {
        if (onEvent) onEvent(msg);
    }

    std::vector<UncertainEntity> entities;
    std::vector<EntangleLink> links;
    BattleResources* resources = nullptr;
    std::function<void(const std::string&)> onEvent;
    float intelDuration;
    int intelCost;
    int probeCost;
    double fogTime = 0.0;      // Q-7 干涉判定用的 fog 時鐘
    bool interference = false; // Q-7 實驗開關(預設關)
    uint64_t seedCounter = 1;
};

} // namespace Gameplay
} // namespace Potato
