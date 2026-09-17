#pragma once

#include "MathUtils/Vector2.h"
#include "Quantum/Qudit.h"

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
    explicit QuantumFog(float intelDuration = 30.0f, int intelCostPerObserve = 1);

    void BindResources(BattleResources* res) { this->resources = res; }

    // 註冊一支敵軍的疊加態。priors 長度須等於 candidates；空則均勻。
    int AddEntity(const std::string& name, int team,
                  const std::vector<Vector2>& candidates,
                  const std::vector<double>& priors = {});

    // 藍噪候選格生成：在 suspectedCenter 周圍 radius 內產生至多 count
    // 個最小間距 minSpacing 的候選點（Poisson-disk 散佈，避免機率雲
    // 叢聚）。biasPoint 不為 nullptr 時，先驗機率依與其距離做高斯
    // 遞減（σ = radius/2）；否則以 suspectedCenter 為中心。
    // 產生失敗（候選 < 2）時退回 R2 準隨機圓盤散佈。
    // 回傳 entityId，失敗回 -1。
    int AddEntityCloud(const std::string& name, int team,
                       Vector2 suspectedCenter, float radius, int count,
                       float minSpacing,
                       const Vector2* biasPoint = nullptr);

    // 觀測：扣情報點 → 塌縮到最接近 truePos 的候選 → 揭露真實位置。
    // 情報不足或實體不存在回傳 false。
    bool Observe(int entityId, const Vector2& truePos);

    // 免情報點揭露（物理偵查：小隊目視接觸）。已揭露者僅更新位置。
    bool Reveal(int entityId, const Vector2& truePos);

    // 負面觀測：把某候選格機率歸零並重新歸一化
    // （小隊目視覆蓋該格但沒看到人——「這裡沒有敵軍」也是情報）。
    // 已揭露/實體或索引無效/候選已為零/只剩最後一個非零候選時回 false。
    bool EliminateCandidate(int entityId, int candidateIndex);

    // 純 Born-rule 觀測（無真值來源時）：回傳命中的候選 index，-1 表示失敗
    int ObserveRandom(int entityId);

    void Update(float dt);

    // ---- 查詢 ----
    bool IsRevealed(int entityId) const;
    Vector2 GetRevealedPos(int entityId) const;
    // 機率雲：[(候選位置, 機率)]；已揭露者回傳 [(真實位置, 1.0)]
    std::vector<std::pair<Vector2, double>> GetCloud(int entityId) const;
    const UncertainEntity* GetEntity(int entityId) const;
    size_t EntityCount() const { return entities.size(); }

private:
    void CollapseNear(UncertainEntity& e, const Vector2& truePos);

    std::vector<UncertainEntity> entities;
    BattleResources* resources = nullptr;
    float intelDuration;
    int intelCost;
    uint64_t seedCounter = 1;
};

} // namespace Gameplay
} // namespace Potato
