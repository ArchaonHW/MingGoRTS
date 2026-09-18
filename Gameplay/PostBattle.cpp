#include "PostBattle.h"
#include "BattleController.h"
#include "BattleRecorder.h"
#include "BattleResources.h"
#include "Roster.h"
#include "Squad.h"

#include <algorithm>
#include <cmath>

namespace Potato {
namespace Gameplay {

int PostBattleReport::TotalLost(int team) const {
    int n = 0;
    for (const auto& c : casualties) if (c.team == team) n += c.lost;
    return n;
}
int PostBattleReport::TotalWounded(int team) const {
    int n = 0;
    for (const auto& c : casualties) if (c.team == team) n += c.wounded;
    return n;
}
int PostBattleReport::TotalDead(int team) const {
    int n = 0;
    for (const auto& c : casualties) if (c.team == team) n += c.dead;
    return n;
}

void PostBattle::SetWoundedRatio(float r) {
    woundedRatio = std::clamp(r, 0.0f, 1.0f);
}

PostBattleReport PostBattle::Settle(const BattleController& battle,
                                    const Roster& roster,
                                    int winnerTeam) const {
    PostBattleReport report;
    report.winnerTeam = winnerTeam;

    // 傷亡分類：lost 拆傷兵/陣亡
    for (const auto& s : battle.GetSquads()) {
        SquadCasualty c;
        c.squadName = s->GetName();
        c.team = s->GetTeam();
        c.lost = s->GetMaxMembers() - s->GetMembers();
        if (c.lost < 0) c.lost = 0;
        c.eliminated = s->IsEliminated();
        c.wounded = static_cast<int>(std::lround(c.lost * woundedRatio));
        c.dead = c.lost - c.wounded;
        if (c.lost > 0 || c.eliminated) {
            report.casualties.push_back(c);
        }
        // 殲滅敵隊 → 戰利品
        if (winnerTeam >= 0 && s->GetTeam() != winnerTeam &&
            s->IsEliminated()) {
            report.lootPoints += lootPerEliminated;
        }
    }

    // 陣亡敵方隊長的遺物列入拾獲清單
    if (winnerTeam >= 0) {
        for (const auto& e : roster.GetEntries()) {
            if (e.team != winnerTeam && !e.alive && !e.relic.empty()) {
                report.relics.push_back(e.relic);
            }
        }
    }
    return report;
}

void PostBattle::ApplyLoot(BattleResources& res, int team,
                           const PostBattleReport& report) const {
    if (team == report.winnerTeam) {
        res.AddLoot(team, report.lootPoints);
    }
}

void PostBattle::RecordTo(const PostBattleReport& report,
                          BattleRecorder& recorder, float t) const {
    for (const auto& c : report.casualties) {
        recorder.AddRecord(t,
            "戰後結算 " + c.squadName + " 損失 " +
            std::to_string(c.lost) + "（傷兵 " +
            std::to_string(c.wounded) + " / 陣亡 " +
            std::to_string(c.dead) + ")" +
            (c.eliminated ? "（全滅）" : ""));
    }
    for (const auto& relic : report.relics) {
        recorder.AddRecord(t, "拾獲遺物：" + relic);
    }
    if (report.lootPoints > 0) {
        recorder.AddRecord(t, "戰利品入帳 +" +
                           std::to_string(report.lootPoints));
    }
}

} // namespace Gameplay
} // namespace Potato
