#pragma once

// 戰役治理累加器（C-2/Epic A）：跨章節的民心/秩序帳。
// 戰場事件（GovernanceEvent）在章節邊界由 Accumulate 折進
// 本帳——戰場 delta × 折半率，夾 [0,100]。
// 低秩序觸發動亂：UnrestLevel 0~3 逐級，跨級才發事件不重複。
//
// 分層：Campaign 可含入 Gameplay 型別（GovernanceEvent.h）；
// Gameplay 永不回流依賴 Campaign。

#include "Gameplay/GovernanceEvent.h"
#include "Serialization/JsonParser.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Potato {
namespace Campaign {

class Governance {
public:
    float PopularSupport() const { return popularSupport; }
    float CivilOrder() const { return civilOrder; }
    // 墮落（A.5 ratchet）：只增不減，無恢復機制——GDD 寫死無衰減
    float Depravity() const { return depravity; }

    // 章節邊界：把一場戰鬥的治理事件計數折進戰役帳。
    // 每事件貢獻 = 戰場 delta × kCampaignRate。
    void Accumulate(
        const std::unordered_map<Gameplay::GovernanceEvent, int>&
            battleEvents);

    // 動亂級：秩序 >= UNREST_WARN 為 0；以下每跨一檻 +1，最深 3
    int UnrestLevel() const;
    // 跨級才產生事件字串（回傳並把已報級記下）；平級/下降回傳空
    std::vector<std::string> PollUnrestEvents();

    JsonValue ToJson() const;
    bool FromJson(const JsonValue& j);

    static constexpr float kCampaignRate = 0.5f;   // 戰場→戰役折半率
    static constexpr float kUnrestWarn = 30.0f;    // 秩序 < 此為動亂警訊
    static constexpr float kUnrestRiot = 20.0f;    // < 此為動亂
    static constexpr float kUnrestCollapse = 10.0f; // < 此為崩壞

private:
    float popularSupport = 50.0f; // 戰役民心 0~100
    float civilOrder = 50.0f;     // 戰役秩序 0~100
    float depravity = 0.0f;       // 墮落 0~100（ratchet，只增不減）
    int lastUnrestLevel = 0;      // 已報動亂級（跨級才再發）
};

} // namespace Campaign
} // namespace Potato
