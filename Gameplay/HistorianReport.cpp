#include "HistorianReport.h"

#include <algorithm>
#include <cstdio>

namespace Potato {
namespace Gameplay {

namespace {

// 事件帳目分類：戰報只書寫「值得入史」的事件，其餘記入省略計數。
// 判定吃事件字串的子字串——與 BattleController::Emit 的用語耦合，
// 事件用語改動時此表需同步（見 spec-n1 §事件分類）。
enum class LedgerClass {
    Rout,          // 潰逃 / 恐慌擴散
    Elimination,   // 全隊殲滅 / 將軍陣亡
    Reveal,        // 接觸目擊 / 情報揭露 / 探測
    Intervention,  // CP 強令
    Omitted        // 階段標記、部署噪音、傷害流水帳等
};

LedgerClass ClassifyEvent(const std::string& event) {
    if (event.find("CP intervention") != std::string::npos) {
        return LedgerClass::Intervention;
    }
    if (event.find("ELIMINATED") != std::string::npos ||
        event.find("slain") != std::string::npos) {
        return LedgerClass::Elimination;
    }
    if (event.find("潰逃") != std::string::npos ||
        event.find("is routing") != std::string::npos) {
        return LedgerClass::Rout;
    }
    if (event.find("目擊") != std::string::npos ||
        event.find("揭露") != std::string::npos ||
        event.find("探測") != std::string::npos) {
        return LedgerClass::Reveal;
    }
    return LedgerClass::Omitted;
}

// ELIMINATED 事件的隊別歸屬：事件字串含 squadName → 查 roster 隊號；
// 查無（roster 為空或未收編）→ 回 -1（中性）
int TeamOfEliminated(const std::string& event, const Roster* roster) {
    if (!roster) {
        return -1;
    }
    for (const RosterEntry& e : roster->GetEntries()) {
        if (!e.squadName.empty() &&
            event.find(e.squadName) != std::string::npos) {
            return e.team;
        }
    }
    return -1;
}

const char* OutcomeLine(BattleOutcome o) {
    switch (o) {
    case BattleOutcome::Victory: return "我軍克敵，奪南岸而還。";
    case BattleOutcome::Defeat:  return "我軍失利，退守北岸。";
    case BattleOutcome::Draw:    return "兩軍相持，鳴金收兵。";
    default:                     return "戰事未決，筆錄至此。";
    }
}

void AppendCount(std::string& out, int count, const char* fmt) {
    if (count <= 0) {
        return;
    }
    char buf[64];
    std::snprintf(buf, sizeof(buf), fmt, count);
    out += buf;
}

} // namespace

HistorianReport ComposeHistorianReport(const HistorianInput& in) {
    HistorianReport r;

    // ---- 事件流分類計數 ----
    if (in.recorder) {
        for (const BattleRecorder::Record& rec : in.recorder->GetRecords()) {
            switch (ClassifyEvent(rec.event)) {
            case LedgerClass::Intervention:
                ++r.interventionCount;
                ++r.narratedCount;
                break;
            case LedgerClass::Reveal:
                ++r.revealCount;
                ++r.narratedCount;
                break;
            case LedgerClass::Rout:
                ++r.routCount;
                ++r.narratedCount;
                break;
            case LedgerClass::Elimination: {
                const int team = TeamOfEliminated(rec.event, in.roster);
                if (team == in.playerTeam) {
                    ++r.playerLosses;
                } else if (team == in.enemyTeam) {
                    ++r.enemyLosses;
                } else {
                    ++r.neutralLosses;
                }
                ++r.narratedCount;
                break;
            }
            case LedgerClass::Omitted:
                break;
            }
        }
        const int total = static_cast<int>(in.recorder->GetRecords().size());
        r.omittedCount = std::max(0, total - r.narratedCount);
    }

    // ---- 片段組裝（史官體）----
    std::string& t = r.text;
    t += "史官曰：" + in.battleName + "，";
    t += OutcomeLine(in.outcome);

    {
        char buf[64];
        std::snprintf(buf, sizeof(buf), "戰歷 %.0f 秒。", in.elapsedSec);
        t += buf;
    }

    // 戰況摘錄：只列非零項，全零則整段不現
    if (r.interventionCount + r.revealCount + r.routCount +
            r.playerLosses + r.enemyLosses + r.neutralLosses >
        0) {
        t += "是役也，";
        AppendCount(t, r.interventionCount, "強令介入 %d 次，");
        AppendCount(t, r.revealCount, "斥候揭敵 %d 處，");
        AppendCount(t, r.routCount, "潰逃 %d 起，");
        AppendCount(t, r.enemyLosses, "斬敵 %d 隊，");
        AppendCount(t, r.playerLosses, "我軍覆 %d 隊，");
        AppendCount(t, r.neutralLosses, "殲滅 %d 隊，");
        t.back() = '。'; // 收尾逗號改句號
    }

    // 名冊句：殉國（含遺物）→ 得全 → 斬敵
    if (in.roster) {
        for (const RosterEntry& e : in.roster->GetEntries()) {
            if (e.team == in.playerTeam && !e.alive) {
                t += "「" + e.name + "」隊長殉國";
                if (!e.relic.empty()) {
                    t += "，遺「" + e.relic + "」";
                }
                t += "。";
            }
        }
        for (const RosterEntry& e : in.roster->GetEntries()) {
            if (e.team == in.playerTeam && e.alive) {
                t += "「" + e.name + "」得全。";
            }
        }
        for (const RosterEntry& e : in.roster->GetEntries()) {
            if (e.team == in.enemyTeam && !e.alive) {
                t += "斬敵「" + e.name + "」。";
            }
        }
    }

    // 審計欄位：省略計數永遠在場——帳目不全是規則不是疏漏
    {
        char buf[48];
        std::snprintf(buf, sizeof(buf), "本報告省略 %d 項。", r.omittedCount);
        t += buf;
    }

    return r;
}

} // namespace Gameplay
} // namespace Potato
