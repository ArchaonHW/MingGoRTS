#include "Campaign/Governance.h"

#include "Campaign/JsonWriter.h"

#include <algorithm>
#include <cmath>

namespace Potato {
namespace Campaign {

void Governance::Accumulate(
    const std::unordered_map<Gameplay::GovernanceEvent, int>&
        battleEvents) {
    for (const auto& [ev, count] : battleEvents) {
        if (count <= 0) continue;
        const Gameplay::GovernanceDelta d =
            Gameplay::DeltaOf(ev);
        popularSupport = std::clamp(
            popularSupport + d.support * kCampaignRate * count,
            0.0f, 100.0f);
        civilOrder = std::clamp(
            civilOrder + d.order * kCampaignRate * count,
            0.0f, 100.0f);
        // 墮落 ratchet：只增不減——負值貢獻直接忽略，不給贖回通道
        if (d.depravity > 0.0f) {
            depravity = std::clamp(
                depravity + d.depravity * kCampaignRate * count,
                0.0f, 100.0f);
        }
    }
}

int Governance::UnrestLevel() const {
    if (civilOrder < kUnrestCollapse) return 3;
    if (civilOrder < kUnrestRiot) return 2;
    if (civilOrder < kUnrestWarn) return 1;
    return 0;
}

std::vector<std::string> Governance::PollUnrestEvents() {
    std::vector<std::string> out;
    const int level = UnrestLevel();
    // 只在向上跨級時發事件；回落再升不會重發同一級
    for (int l = lastUnrestLevel + 1; l <= level; ++l) {
        switch (l) {
        case 1:
            out.push_back("動亂警訊：境內秩序漸弛，民心浮動");
            break;
        case 2:
            out.push_back("動亂爆發：盜賊蜂起，郡縣告急");
            break;
        case 3:
            out.push_back("秩序崩壞：四方板蕩，號令不出都門");
            break;
        default:
            break;
        }
    }
    if (level > lastUnrestLevel) {
        lastUnrestLevel = level;
    } else if (level < lastUnrestLevel) {
        // 秩序回升：已報級跟著下調，之後再惡化會重新報
        lastUnrestLevel = level;
    }
    return out;
}

JsonValue Governance::ToJson() const {
    JsonValue o;
    o.type = JsonValue::Type::Object;
    o.objectValue["popular_support"] = JsonValue::Number(popularSupport);
    o.objectValue["civil_order"] = JsonValue::Number(civilOrder);
    o.objectValue["last_unrest_level"] = JsonValue::Number(lastUnrestLevel);
    return o;
}

bool Governance::FromJson(const JsonValue& j) {
    if (!j.IsObject()) {
        return false;
    }
    popularSupport = std::clamp(
        static_cast<float>(j["popular_support"].AsNumber(50.0)),
        0.0f, 100.0f);
    civilOrder = std::clamp(
        static_cast<float>(j["civil_order"].AsNumber(50.0)),
        0.0f, 100.0f);
    lastUnrestLevel = std::clamp(j["last_unrest_level"].AsInt(0), 0, 3);
    return true;
}

} // namespace Campaign
} // namespace Potato
