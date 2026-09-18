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
    const auto has = [&](const char* s) {
        return event.find(s) != std::string::npos;
    };
    // 結局標記先擋——衛隊 ELIMINATED 已入 Elimination，
    // "general slain!" 是同一事件的結果宣告，重計會虛增中性殲滅
    if (has("general slain")) {
        return LedgerClass::Omitted;
    }
    if (has("CP intervention")) {
        return LedgerClass::Intervention;
    }
    if (has(" ELIMINATED")) {
        return LedgerClass::Elimination;
    }
    // Rout 只吃「潰逃！」本身與 is routing；
    // 「目睹友軍潰逃」「潰逃歸附」是擴散/歸附流水帳，不算新潰逃
    if (has("潰逃！") || has("is routing")) {
        return LedgerClass::Rout;
    }
    // Reveal：接觸目擊「被我軍目擊」＋ fog 情報動作
    // （「目睹友軍潰逃」不含「被我軍」，不會誤入此類）
    if (has("被我軍目擊") || has("揭露") || has("探測") ||
        has("fog:observe") || has("fog:reveal") ||
        has("fog:probe") || has("fog:eliminate")) {
        return LedgerClass::Reveal;
    }
    return LedgerClass::Omitted;
}

// ELIMINATED 事件的隊別歸屬：事件格式固定為 "<squadName> ELIMINATED"，
// 截尾後精確比對 squadName——子字串比對會讓「前鋒」誤吞「前鋒二隊」
int TeamOfEliminated(const std::string& event, const Roster* roster) {
    if (!roster) {
        return -1;
    }
    static const std::string kSuffix = " ELIMINATED";
    std::string name = event;
    if (name.size() >= kSuffix.size() &&
        name.compare(name.size() - kSuffix.size(), kSuffix.size(),
                     kSuffix) == 0) {
        name.resize(name.size() - kSuffix.size());
    }
    for (const RosterEntry& e : roster->GetEntries()) {
        if (!e.squadName.empty() && e.squadName == name) {
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
        // 收尾全形逗號是 3-byte UTF-8（EF BC 8C）——不能只改最後一 byte
        t.resize(t.size() - 3);
        t += "。";
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

    // N-3 帳本外洩判詞：敵方針對我軍慣用 trigger 時留痕，
    // 置於名冊句之後、省略計數之前（審計欄位恆為全文最後一段）。
    // 措辭與 RivalDeck::WarningLine 一致——同一判詞兩處書寫
    if (!in.counteredHabit.empty()) {
        t += "彼之陣法，似針對我軍慣用「" + in.counteredHabit + "」";
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
