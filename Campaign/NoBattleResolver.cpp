#include "Campaign/NoBattleResolver.h"

#include "Campaign/ChapterLibrary.h"

namespace Potato {
namespace Campaign {

const char* NoBattlePathName(NoBattlePath p) {
    switch (p) {
    case NoBattlePath::Negotiation: return "Negotiation";
    case NoBattlePath::Deterrence:  return "Deterrence";
    case NoBattlePath::Subversion:  return "Subversion";
    }
    return "Negotiation";
}

const char* NoBattlePathNameZh(NoBattlePath p) {
    switch (p) {
    case NoBattlePath::Negotiation: return "談判";
    case NoBattlePath::Deterrence:  return "嚇阻";
    case NoBattlePath::Subversion:  return "顛覆";
    }
    return "談判";
}

const char* NoBattleVerdictName(NoBattleVerdict v) {
    switch (v) {
    case NoBattleVerdict::NotOffered: return "NotOffered";
    case NoBattleVerdict::Success:    return "Success";
    case NoBattleVerdict::Failed:     return "Failed";
    case NoBattleVerdict::Backfired:  return "Backfired";
    }
    return "NotOffered";
}

NoBattleContext NoBattleResolver::Gather(
    const Gameplay::LedgerChain& ledger,
    const Gameplay::GeneralDossier& dossier,
    const std::string& generalName) {
    NoBattleContext ctx;
    const auto& tb = ledger.TrialBalance();
    ctx.civilBalance = tb[static_cast<size_t>(Gameplay::LedgerAccount::Civil)];
    ctx.armyBalance = tb[static_cast<size_t>(Gameplay::LedgerAccount::Army)];
    for (const auto& e : dossier.Entries()) {
        if (e.generalName != generalName) {
            continue;
        }
        if (e.verified && !e.planted && !e.consumed) {
            ++ctx.verifiedIntel; // E-3：已兌換的判詞不再計入可用情報
        } else if (e.planted && !e.verified) {
            ctx.plantedUnrefuted = true; // 未驗證的假情報——陷阱仍埋著
        }
    }
    return ctx;
}

std::vector<NoBattleOptionInfo> NoBattleResolver::CheckOptions(
    const ChapterDef& def, const NoBattleContext& ctx) const {
    std::vector<NoBattleOptionInfo> out;
    auto push = [&](NoBattlePath path, const NoBattleOptionDef& opt,
                    int current, const char* ok, const char* shortfall) {
        if (!opt.enabled) {
            return; // 未定義路徑不列入報表
        }
        NoBattleOptionInfo info;
        info.path = path;
        info.current = current;
        info.threshold = opt.requirement;
        info.available = opt.requirement > 0 && current >= opt.requirement;
        info.reason = info.available
                          ? ok
                          : std::string(shortfall) + "（現值 " +
                                std::to_string(current) + "／需 " +
                                std::to_string(opt.requirement) + "）";
        out.push_back(std::move(info));
    };
    push(NoBattlePath::Negotiation, def.noBattle.negotiation,
         ctx.civilBalance, "民心可換罷兵", "民心不足");
    push(NoBattlePath::Deterrence, def.noBattle.deterrence,
         ctx.armyBalance, "軍威足以懾敵", "軍威不足");
    push(NoBattlePath::Subversion, def.noBattle.subversion,
         ctx.verifiedIntel, "內應可策", "情報不足");
    return out;
}

NoBattleResult NoBattleResolver::Resolve(NoBattlePath path,
                                         const ChapterDef& def,
                                         const NoBattleContext& ctx,
                                         Gameplay::LedgerChain& ledger,
                                         Gameplay::CampaignLedger& campaign)
    const {
    using Gameplay::EntrySource;
    using Gameplay::GeneralDisposition;
    using Gameplay::LedgerAccount;
    using Gameplay::LedgerEntry;

    NoBattleResult r;
    r.path = path;

    const NoBattleOptionDef* opt = nullptr;
    int current = 0;
    switch (path) {
    case NoBattlePath::Negotiation:
        opt = &def.noBattle.negotiation;
        current = ctx.civilBalance;
        break;
    case NoBattlePath::Deterrence:
        opt = &def.noBattle.deterrence;
        current = ctx.armyBalance;
        break;
    case NoBattlePath::Subversion:
        opt = &def.noBattle.subversion;
        current = ctx.verifiedIntel;
        break;
    }
    if (!opt || !opt->enabled || opt->requirement <= 0) {
        r.summary = std::string("本章未開") + NoBattlePathNameZh(path) +
                    "之路";
        return r; // NotOffered：不寫帳、無代價
    }

    const EntrySource src =
        path == NoBattlePath::Negotiation  ? EntrySource::Negotiation
        : path == NoBattlePath::Deterrence ? EntrySource::Deterrence
                                           : EntrySource::Subversion;
    const std::string evId = ctx.chapterId + ":" + NoBattlePathName(path);
    auto record = [&](LedgerAccount debit, LedgerAccount credit, int amount,
                      const std::string& memo) {
        LedgerEntry e;
        e.debit = debit;
        e.credit = credit;
        e.amount = amount;
        e.chapter = ctx.chapter;
        e.memo = memo;
        e.prov.source = src;
        e.prov.tick = ctx.chapter;
        e.prov.eventId = evId;
        ledger.Append(e);
    };

    const bool met = current >= opt->requirement;
    const char* who =
        ctx.generalName.empty() ? "敵將" : ctx.generalName.c_str();
    const std::string tag =
        "「" + std::string(who) + "」";

    if (met && path == NoBattlePath::Subversion && ctx.plantedUnrefuted) {
        // 顛覆中計：verified 情報達標但帳本有未駁斥的假情報——
        // 敵設局候我，情報資產反成陷阱（E.3）
        record(LedgerAccount::Supply, LedgerAccount::Civil, 1,
               "顛覆中計——假情報反噬" + tag);
        r.verdict = NoBattleVerdict::Backfired;
        r.enemyAdvantage = true;
        r.spent = 1;
        r.summary = "顛覆中計——敵設局候我，" + tag + "反客為主";
        return r;
    }

    if (!met) {
        // 強行嘗試未過門檻：失敗進戰鬥且敵獲首波優勢（代價明確）
        int cost = 1;
        LedgerAccount debit = LedgerAccount::Supply;
        LedgerAccount credit = LedgerAccount::Civil;
        std::string memo;
        if (path == NoBattlePath::Negotiation) {
            cost = opt->requirement / 4;
            if (cost < 1) {
                cost = 1;
            }
            memo = "談判破裂——民心虛耗" + tag;
        } else if (path == NoBattlePath::Deterrence) {
            credit = LedgerAccount::Army;
            memo = "虛張被識破——軍威折損" + tag;
        } else {
            memo = "情報不足強行顛覆——事敗" + tag;
        }
        record(debit, credit, cost, memo);
        r.verdict = NoBattleVerdict::Failed;
        r.enemyAdvantage = true;
        r.spent = cost;
        r.summary = std::string(NoBattlePathNameZh(path)) + "事敗——" + tag +
                    "已有備，首波陷敵";
        return r;
    }

    // 門檻通過——跳過戰鬥狀態機直進結算
    switch (path) {
    case NoBattlePath::Negotiation:
        // 民心消費：以人和感天罷兵（借 Fate 貸 Civil）
        record(LedgerAccount::Fate, LedgerAccount::Civil, opt->requirement,
               "談判罷兵——民心通於天命" + tag);
        r.spent = opt->requirement;
        r.disposition = GeneralDisposition::Negotiated;
        r.summary = "談判成——" + tag + "罷兵言和";
        break;
    case NoBattlePath::Deterrence:
        // 軍威存在即判定，不消耗（象徵一錄：借 Army 貸 Martial）
        record(LedgerAccount::Army, LedgerAccount::Martial, 1,
               "嚇阻退兵——軍威不損" + tag);
        r.spent = 1;
        r.disposition = GeneralDisposition::Intimidated;
        r.summary = "嚇阻成——" + tag + "聞風退兵";
        break;
    case NoBattlePath::Subversion:
        // 情報兌換內應（借 Army 貸 Fate——天命所歸敵陣自潰）
        record(LedgerAccount::Army, LedgerAccount::Fate, 1,
               "內應倒戈——敵陣自潰" + tag);
        r.spent = 1;
        r.disposition = GeneralDisposition::Defected;
        r.summary = "顛覆成——" + tag + "帳下內應倒戈";
        break;
    }
    r.verdict = NoBattleVerdict::Success;
    campaign.RecordDisposition(ctx.generalId, ctx.generalName, ctx.chapter,
                               r.disposition);
    return r;
}

} // namespace Campaign
} // namespace Potato
