#pragma once

// 戰場治理追蹤器（Epic A：A.1/A.3/A.4）。
// BattleController 只記事件計數，不認識地圖；本類由「有地圖知識的
// 呼叫端」每拍驅動，把地圖互動物/運輸隊轉譯成治理事件：
//
// - 村莊佔領：我軍活隊進駐 village 圈 → VillageOccupied（每點一次）
// - 焚村：小隊在村莊圈內損員（戰火波及近似）→ VillageBurned +
//   burned 標記供渲染層讀 intact/burned（A.1 視覺狀態位）
// - 護輜：convoy 沿 waypoint 前進；抵達 → ConvoyProtected；
//   敵隊入 raidRadius 持續劫掠，hp 歸零 → 我方 ConvoyLost /
//   敵方 ConvoyRaided + 敵全軍士氣打擊（A.4）
//
// 事件一律經 battle.RecordGovernanceEvent 入帳（record-is-truth），
// 本類不直接碰 Campaign 層——章節邊界由呼叫端折帳。

#include "BattleMap.h"
#include "MathUtils/Vector2.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class Squad;

class GovernanceField {
public:
    struct ConvoyState {
        const MapConvoy* def = nullptr;
        Vector2 pos;
        int hp = 0;
        size_t nextWaypoint = 1; // 正前往的 path 索引
        enum class Status { Moving, Arrived, Raided } status =
            Status::Moving;
    };

    // 綁定地圖互動物與運輸隊定義（部署完成後呼叫一次）
    void Bind(const std::vector<MapInteractable>& interactables,
              const std::vector<MapConvoy>& convoys);

    // 每拍驅動：佔領偵測 → 焚村偵測 → 護輜推進/劫掠
    void Update(float dt, BattleController& battle);

    // 渲染層讀取：村莊互動物是否已被戰火波及（intact/burned）
    bool IsBurned(size_t interactableIndex) const;
    const std::vector<ConvoyState>& GetConvoys() const {
        return convoys;
    }

    // 敵輜被劫時對敵全軍的士氣打擊（A.4）
    static constexpr float kRaidMoraleHit = -0.20f;
    // 劫掠傷害速率（hp/秒，敵隊在圈內時）
    static constexpr float kRaidDps = 12.0f;

private:
    void DetectOccupation(BattleController& battle);
    void DetectBurning(BattleController& battle);
    void UpdateConvoys(float dt, BattleController& battle);

    std::vector<MapInteractable> interactables;
    std::unordered_set<size_t> fired;  // 已觸發佔領的互動點
    std::unordered_set<size_t> burned; // 已焚村莊索引
    std::vector<ConvoyState> convoys;
    // 各隊上拍員額——損員位置近似「火力穿過村莊格」
    std::unordered_map<const Squad*, int> prevMembers;
};

} // namespace Gameplay
} // namespace Potato
