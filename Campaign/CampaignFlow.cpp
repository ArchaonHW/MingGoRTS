#include "Campaign/CampaignFlow.h"
#include "Gameplay/BattleController.h"
#include <algorithm>
namespace Potato::Campaign {
using namespace Gameplay;
const ChapterDefinition *CampaignFlow::Current(const CampaignState &s) const {
    return library.Find(s.chapter.chapterId);
}
const ChapterChoice *CampaignFlow::SelectedChoice(const CampaignState &s) const {
    const auto *c = Current(s);
    if (!c)
        return nullptr;
    auto it = s.progress.choices.find(c->id);
    if (it == s.progress.choices.end())
        return nullptr;
    for (const auto &v : c->choices)
        if (v.id == it->second)
            return &v;
    return nullptr;
}
bool CampaignFlow::StartNew(CampaignState &s, std::string &error) const {
    if (library.Chapters().empty()) {
        error = "章節尚未載入";
        return false;
    }
    CampaignState n;
    n.progress.initialized = true;
    const auto &c = library.Chapters().front();
    n.AdvanceChapter(c.arc, c.number, c.id);
    n.Camp().DepositLoot(12);
    const char *names[] = {"前鋒", "中軍", "左翼", "後衛", "將軍衛隊"};
    const char *captains[] = {"周鐵槍", "陳守拙", "林燕翼", "石敢當", "衛青川"};
    int members[] = {30, 22, 18, 16, 12};
    BattleController seed(24, 16, 1);
    for (int i = 0; i < 5; ++i) {
        VeteranUnit v;
        v.squadName = names[i];
        v.captainName = captains[i];
        v.members = v.maxMembers = members[i];
        v.relics.push_back(i == 0 ? "斷刃" : "軍中札記");
        n.Camp().EnrollUnit(v);
        auto *sq = seed.CreateSquad(v.squadName, 0, c.friendlyDeployment[i], v.members);
        n.NamedRoster().Enroll(sq, v.captainName, "captain", v.relics.front());
    }
    // 持久名冊純資料化，不保留臨時戰鬥物件指標。
    Roster clean;
    clean.FromJson(n.NamedRoster().ToJson());
    n.NamedRoster() = std::move(clean);
    s = std::move(n);
    error.clear();
    return true;
}
bool CampaignFlow::Validate(const CampaignState &s, std::string &error) const {
    auto fail = [&]() {
        error = "戰役進度與章節資產不一致，拒絕繼續";
        return false;
    };
    if (!s.progress.initialized || !Current(s) || s.Ledger().Chapter() != s.chapter.chapter)
        return fail();
    const auto *c = Current(s);
    if (c->number != s.chapter.chapter || c->arc != s.chapter.arc)
        return fail();
    const auto &all = library.Chapters();
    if (s.progress.completed.size() > all.size())
        return fail();
    for (size_t i = 0; i < s.progress.completed.size(); ++i)
        if (s.progress.completed[i] != all[i].id)
            return fail();
    size_t expected =
        static_cast<size_t>(c->number - 1) + (s.progress.stage == CampaignStage::Briefing ? 0 : 1);
    if (s.progress.completed.size() != expected)
        return fail();
    if (s.progress.stage == CampaignStage::Complete && expected != all.size())
        return fail();
    for (const auto &[id, value] : s.progress.choices) {
        const auto *def = library.Find(id);
        if (!def || def->number > c->number)
            return fail();
        bool found = false;
        for (const auto &v : def->choices)
            if (v.id == value)
                found = true;
        if (!found)
            return fail();
    }
    if (s.progress.outcomes.size() != s.progress.completed.size())
        return fail();
    for (const auto &id : s.progress.completed) {
        auto it = s.progress.outcomes.find(id);
        if (it == s.progress.outcomes.end() ||
            (it->second != "victory" && it->second != "defeat" && it->second != "draw" &&
             it->second != "peace") ||
            !s.progress.choices.count(id))
            return fail();
        if (it->second == "peace" && id != "peace_gate")
            return fail();
    }
    auto value = [](const auto &map, const std::string &id) {
        auto it = map.find(id);
        return it == map.end() ? std::string() : it->second;
    };
    const bool peaceEligible = value(s.progress.choices, "duanqiao") == "mercy" &&
                               value(s.progress.outcomes, "duanqiao") == "victory" &&
                               value(s.progress.choices, "ash_ledger") == "rescue" &&
                               value(s.progress.outcomes, "ash_ledger") == "victory";
    if (value(s.progress.choices, "peace_gate") == "negotiate" && !peaceEligible)
        return fail();
    if (value(s.progress.outcomes, "peace_gate") == "peace" &&
        value(s.progress.choices, "peace_gate") != "negotiate")
        return fail();
    if (value(s.progress.choices, "peace_gate") == "negotiate" &&
        !value(s.progress.outcomes, "peace_gate").empty() &&
        value(s.progress.outcomes, "peace_gate") != "peace")
        return fail();
    // 招募上限為 12，另留傷員接收與七章地方預備隊的空間。
    if (s.Camp().GetUnits().size() > 32)
        return fail();
    for (const auto &u : s.Camp().GetUnits()) {
        if (!library.IsKnownTemplate(u.templateId))
            return fail();
        for (const auto &e : s.NamedRoster().GetEntries())
            if (e.team == 0 && e.squadName == u.squadName && !e.alive)
                return fail();
    }
    if (s.progress.lastPeaceful !=
        (s.progress.stage != CampaignStage::Briefing && s.progress.outcomes.at(c->id) == "peace"))
        return fail();
    if (s.progress.stage != CampaignStage::Briefing) {
        const auto &result = s.progress.outcomes.at(c->id);
        int winner = result == "victory" ? 0 : result == "defeat" ? 1 : -1;
        if (s.progress.lastReport.winnerTeam != winner)
            return fail();
        if (result == "peace" &&
            (!s.progress.lastReport.casualties.empty() || s.progress.lastReport.lootPoints != 0 ||
             !s.progress.lastReport.relics.empty()))
            return fail();
    }
    error.clear();
    return true;
}
bool CampaignFlow::Choose(CampaignState &s, const std::string &id, std::string &error) const {
    if (!Validate(s, error))
        return false;
    if (s.progress.stage != CampaignStage::Briefing) {
        error = "只能在戰前決策";
        return false;
    }
    const auto *c = Current(s);
    if (s.progress.choices.count(c->id)) {
        error = "本章決策已記錄，避免重複付費";
        return false;
    }
    const ChapterChoice *selected = nullptr;
    for (const auto &ch : c->choices)
        if (ch.id == id)
            selected = &ch;
    if (!selected) {
        error = "未知決策";
        return false;
    }
    if (id == "negotiate" && !CanResolvePeacefully(s, error))
        return false;
    CampaignState n = s;
    if (!n.Camp().SpendLoot(selected->cost)) {
        error = "補給不足";
        return false;
    }
    n.Camp().DepositLoot(selected->supply);
    n.progress.choices[c->id] = id;
    s = std::move(n);
    error.clear();
    return true;
}
bool CampaignFlow::CanResolvePeacefully(const CampaignState &s, std::string &reason) const {
    auto value = [](const auto &m, const std::string &id) {
        auto it = m.find(id);
        return it == m.end() ? std::string() : it->second;
    };
    if (!Current(s) || Current(s)->id != "peace_gate" ||
        s.progress.stage != CampaignStage::Briefing) {
        reason = "目前不在第三章戰前";
        return false;
    }
    if (value(s.progress.outcomes, "duanqiao") != "victory" ||
        value(s.progress.choices, "duanqiao") != "mercy") {
        reason = "需要斷橋獲勝並接受投降";
        return false;
    }
    if (value(s.progress.outcomes, "ash_ledger") != "victory" ||
        value(s.progress.choices, "ash_ledger") != "rescue") {
        reason = "需要第二章成功救援後衛";
        return false;
    }
    reason.clear();
    return true;
}
bool CampaignFlow::CompleteBattle(CampaignState &s, const BattleController &b, const Roster &roster,
                                  std::string &error) const {
    if (!Validate(s, error))
        return false;
    if (s.progress.stage != CampaignStage::Briefing || !SelectedChoice(s) ||
        b.GetOutcome() == BattleOutcome::Ongoing) {
        error = "本章尚未決策、戰鬥未結束或已結算";
        return false;
    }
    if (SelectedChoice(s)->id == "negotiate") {
        error = "談判路徑應使用無戰結算";
        return false;
    }
    CampaignState n = s;
    PostBattle pb;
    int winner = b.GetOutcome() == BattleOutcome::Victory  ? 0
                 : b.GetOutcome() == BattleOutcome::Defeat ? 1
                                                           : -1;
    auto report = pb.Settle(b, roster, winner);
    // 全滅者不能再醫治，將全滅損失歸於永久陣亡並保留哀悼名冊。
    for (auto &v : report.casualties)
        if (v.eliminated) {
            v.dead = v.lost;
            v.wounded = 0;
        }
    if (winner != 0) {
        report.lootPoints = 0;
        report.relics.clear();
    }
    n.Camp().Absorb(report, roster, 0);
    n.NamedRoster().MergeHistory(roster);
    RegisterCampRoster(n);
    if (winner == 0)
        n.Camp().DepositLoot(report.lootPoints + SelectedChoice(s)->reward);
    else
        report.lootPoints = 0;
    const auto *c = Current(s);
    GeneralDisposition disposition = GeneralDisposition::Unknown;
    for (const auto &e : roster.GetEntries())
        if (e.team == 1 && e.squadName == c->enemies.front().name) {
            if (!e.alive)
                disposition = GeneralDisposition::Slain;
            else if (winner == 0 && SelectedChoice(s)->id == "mercy")
                disposition = GeneralDisposition::Retired;
            break;
        }
    n.Ledger().RecordDisposition(c->id + "_general", c->enemyGeneralName, c->number, disposition);
    n.progress.lastReport = report;
    n.progress.cumulativeDead += report.TotalDead(0);
    n.progress.completed.push_back(c->id);
    n.progress.outcomes[c->id] = winner == 0 ? "victory" : winner == 1 ? "defeat" : "draw";
    n.progress.stage = CampaignStage::Aftermath;
    n.progress.lastPeaceful = false;
    s = std::move(n);
    error.clear();
    return true;
}
bool CampaignFlow::CompletePeace(CampaignState &s, std::string &error) const {
    if (!Validate(s, error) || !CanResolvePeacefully(s, error))
        return false;
    if (!SelectedChoice(s) || SelectedChoice(s)->id != "negotiate") {
        error = "請先選擇談判";
        return false;
    }
    CampaignState n = s;
    const auto *c = Current(s);
    n.Ledger().RecordDisposition(c->id + "_general", c->enemyGeneralName, c->number,
                                 GeneralDisposition::Subdued);
    n.progress.lastReport = {};
    n.progress.lastPeaceful = true;
    n.progress.completed.push_back(c->id);
    n.progress.outcomes[c->id] = "peace";
    n.progress.stage = CampaignStage::Aftermath;
    s = std::move(n);
    error.clear();
    return true;
}
bool CampaignFlow::Advance(CampaignState &s, std::string &error) const {
    if (!Validate(s, error))
        return false;
    if (s.progress.stage != CampaignStage::Aftermath) {
        error = "目前不是戰後階段";
        return false;
    }
    CampaignState n = s;
    const auto *c = Current(s);
    if (c->number == static_cast<int>(library.Chapters().size()))
        n.progress.stage = CampaignStage::Complete;
    else {
        const auto &next = library.Chapters()[c->number];
        n.AdvanceChapter(next.arc, next.number, next.id);
        n.progress.stage = CampaignStage::Briefing;
        n.progress.lastPeaceful = false;
        // 每章地方補給與新預備隊記帳；不重建、復活舊常備軍。
        n.Camp().DepositLoot(8);
        int strength = 0;
        for (const auto &u : n.Camp().GetUnits())
            strength += u.members;
        if (strength < 24) {
            VeteranUnit reserve;
            reserve.squadName = "地方預備隊 · " + std::to_string(next.number);
            reserve.captainName = "新任軍官 · " + std::to_string(next.number);
            reserve.members = reserve.maxMembers = 24;
            n.Camp().EnrollUnit(reserve);
            BattleController seed(24, 16, 1);
            Roster history;
            history.Enroll(
                seed.CreateSquad(reserve.squadName, 0, next.friendlyDeployment.front(), 24),
                reserve.captainName, "captain", "地方通行札");
            n.NamedRoster().MergeHistory(history);
            n.Ledger().EarnTitle("第" + std::to_string(next.number) + "章地方補给：8；新預備隊：24",
                                 next.number);
        } else
            n.Ledger().EarnTitle("第" + std::to_string(next.number) + "章地方補给：8", next.number);
    }
    s = std::move(n);
    error.clear();
    return true;
}
void CampaignFlow::RegisterCampRoster(CampaignState &state) const {
    BattleController seed(24, 16, 1);
    Roster additions;
    for (const auto &unit : state.Camp().GetUnits()) {
        bool known = false;
        for (const auto &e : state.NamedRoster().GetEntries())
            if (e.team == 0 && e.squadName == unit.squadName)
                known = true;
        if (!known)
            additions.Enroll(seed.CreateSquad(unit.squadName, 0, {2, 2}, 1), unit.captainName,
                             "captain");
    }
    state.NamedRoster().MergeHistory(additions);
}
} // namespace Potato::Campaign
