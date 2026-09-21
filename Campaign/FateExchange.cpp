#include "Campaign/FateExchange.h"

#include "Campaign/Governance.h"
#include "Campaign/MythLayer.h"

namespace Potato {
namespace Campaign {

namespace {
float Axis(const Governance& g, FateTarget t) {
    return t == FateTarget::PopularSupport ? g.PopularSupport()
                                           : g.CivilOrder();
}
} // namespace

const char* FateTargetName(FateTarget t) {
    switch (t) {
    case FateTarget::PopularSupport: return "民心";
    case FateTarget::CivilOrder:     return "秩序";
    }
    return "未知";
}

void FateExchange::Bind(MythLayer& m, Governance& g) {
    myths = &m;
    gov = &g;
    battleUses = 0; // 重綁=新場域，額度歸零（ShrineField::Bind 慣例）
}

std::vector<FateOption>
FateExchange::Options(const std::string& spirit) const {
    const bool bound = myths && gov;
    const float favor =
        bound ? myths->Favor(spirit) : 0.0f;
    const bool affordable =
        bound && !spirit.empty() && favor >= kFateCost;
    std::string reason;
    if (!bound) {
        reason = "兌換層未綁定";
    } else if (spirit.empty()) {
        reason = "未指定神明";
    } else if (!affordable) {
        reason = "天命不足（需 " + std::to_string((int)kFateCost) + "）";
    } else if (battleUses >= kMaxBattleUses) {
        // 戰中額度盡：affordable 仍表 favor 面，額度走
        // battleUsesLeft + 此 reason 讓 UI 能顯示原因
        reason = "本場兌換次數用盡";
    }
    const bool usesLeft = battleUses < kMaxBattleUses;
    std::vector<FateOption> out;
    for (FateTarget t : {FateTarget::PopularSupport,
                         FateTarget::CivilOrder}) {
        FateOption o{t, kFateCost, kFateGain, affordable, usesLeft,
                     reason};
        // 目標軸已滿：名目可兌但實得為零——灰顯擋下白扣費
        if (o.affordable && bound && Axis(*gov, t) >= 100.0f) {
            o.affordable = false;
            o.reason = "目標已滿，兌換無益";
        }
        out.push_back(o);
    }
    return out;
}

bool FateExchange::Convert(const std::string& spirit,
                           FateTarget target, bool inBattle) {
    if (!myths || !gov || spirit.empty()) return false;
    // 幻影錢包防線：未登錄名字不得以 kNeutralFavor 起算扣費
    if (!myths->HasSpirit(spirit)) return false;
    if (inBattle && battleUses >= kMaxBattleUses) return false;
    if (target != FateTarget::PopularSupport &&
        target != FateTarget::CivilOrder) {
        return false; // 非法 enum：先驗型別再放行，保零副作用
    }
    // !(favor >= cost) 而非 favor < cost——NaN 時兩分支同拒，
    // 與 Options 的 affordable 判定互補一致
    if (!(myths->Favor(spirit) >= kFateCost)) return false;

    // 先變異後派出——回呼可安全再入本層
    const float before = Axis(*gov, target);
    if (before >= 100.0f) return false; // 滿軸兌換實得零，拒絕白扣費
    myths->AdjustFavor(spirit, -kFateCost);
    float after;
    if (target == FateTarget::PopularSupport) {
        gov->AdjustPopularSupport(kFateGain);
        after = gov->PopularSupport();
    } else {
        gov->AdjustCivilOrder(kFateGain);
        after = gov->CivilOrder();
    }
    if (inBattle) ++battleUses;
    if (onEvent) {
        // gain 報實際生效量——軸滿格時 record-is-truth 不虛報
        onEvent({spirit, target, kFateCost, after - before, inBattle});
    }
    return true;
}

} // namespace Campaign
} // namespace Potato
