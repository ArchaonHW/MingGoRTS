#include "HistorianReport.h"
#include "NarrativePack.h"

#include "Gameplay/Ledger.h"

#include <algorithm>
#include <cstdio>
#include <map>

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
    // E-4 和平語域：無戰章節不經戰鬥狀態機，但照樣書寫——
    // 路徑句 + 敵將去向（不入陣亡錄），查帳段與省略計數同規。
    const bool peace = !in.peacePathZh.empty();
    std::string& t = r.text;
    const std::string g =
        in.peaceGeneral.empty() ? "敵將" : in.peaceGeneral;
    // C-2：pack 在場時碎片取資料檔，查無 id 回退內建字串
    const std::map<std::string, std::string> pairs = {
        {"{battle}", in.battleName}, {"{g}", g},
        {"{habit}", in.counteredHabit}};
    auto Frag = [&](const char* id, const char* fallback) {
        if (in.pack) {
            return in.pack->Frag(id, fallback, pairs);
        }
        // 無 pack：回退字串照樣代換佔位符，行為與 pack 命中一致
        std::string s = fallback;
        for (const auto& [k, v] : pairs) {
            size_t p = 0;
            while ((p = s.find(k, p)) != std::string::npos) {
                s.replace(p, k.size(), v);
                p += v.size();
            }
        }
        return s;
    };
    if (peace) {
        t += Frag("historian.peace.open",
                  "史官曰：{battle}之局，兵不血刃。");
        if (in.peacePathZh == "談判") {
            t += Frag("historian.peace.negotiation",
                      "遣使入帳，以民心曉之，「{g}」罷兵言和——"
                      "議和而去，不入陣亡錄。");
        } else if (in.peacePathZh == "嚇阻") {
            t += Frag("historian.peace.deterrence",
                      "陳師疆埸，軍威所懾，「{g}」未交一矢引兵而退——"
                      "懾服而去，不入陣亡錄。");
        } else if (in.peacePathZh == "顛覆") {
            t += Frag("historian.peace.subversion",
                      "內應發於敵帳，「{g}」眾叛親離——"
                      "變節投誠，不入陣亡錄。");
        } else {
            t += Frag("historian.peace.generic",
                      "「{g}」偃旗息鼓——不入陣亡錄。");
        }
        t += Frag("historian.peace.closing", "不戰而定。");
    } else {
        t += Frag("historian.open", "史官曰：{battle}，");
        if (!in.pack) {
            t += OutcomeLine(in.outcome);
        } else {
            const char* oid = "historian.outcome.ongoing";
            switch (in.outcome) {
            case BattleOutcome::Victory: oid = "historian.outcome.victory"; break;
            case BattleOutcome::Defeat:  oid = "historian.outcome.defeat";  break;
            case BattleOutcome::Draw:    oid = "historian.outcome.draw";    break;
            default: break;
            }
            t += Frag(oid, OutcomeLine(in.outcome));
        }

        {
            char buf[64];
            std::snprintf(buf, sizeof(buf), "戰歷 %.0f 秒。",
                          in.elapsedSec);
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
    }

    // 名冊句：殉國（含遺物）→ 得全 → 斬敵（和平語域無戰可錄，略過）
    if (!peace && in.roster) {
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
        t += Frag("historian.countered",
                  "彼之陣法，似針對我軍慣用「{habit}」");
    }

    // L-3 查帳段：帳簿在場才書。斷鏈 → 帳目遭篡；借貸不成立 →
    // 墨跡未乾；平衡 → 借貸相符並附非零帳戶淨額。審計欄位仍恆居末。
    if (in.ledger && in.ledger->Size() > 0) {
        const int broken = in.ledger->Verify();
        const int unsound = in.ledger->SoundnessViolation();
        char buf[160];
        if (broken >= 0) {
            std::snprintf(buf, sizeof(buf),
                          "本章記帳 %d 筆，第 %d 筆墨跡未乾——帳目遭篡，史官存疑。",
                          static_cast<int>(in.ledger->Size()), broken + 1);
            t += buf;
        } else if (unsound >= 0) {
            std::snprintf(buf, sizeof(buf),
                          "本章記帳 %d 筆，第 %d 筆借貸不符——墨跡未乾，史官存疑。",
                          static_cast<int>(in.ledger->Size()), unsound + 1);
            t += buf;
        } else {
            std::snprintf(buf, sizeof(buf), "本章記帳 %d 筆，借貸相符",
                          static_cast<int>(in.ledger->Size()));
            t += buf;
            // L-7 產生軌跡統計：各來源系統記了幾筆（失考照列——
            // 無軌跡本身就是可查證的事實）
            {
                std::array<int, static_cast<size_t>(EntrySource::Count)>
                    srcCount{};
                for (const auto& ch : in.ledger->Entries()) {
                    const EntrySource s = ch.entry.prov.source;
                    if (s < EntrySource::Count) {
                        srcCount[static_cast<size_t>(s)]++;
                    }
                }
                std::string srcText;
                char sbuf[32];
                for (int i = 0;
                     i < static_cast<int>(EntrySource::Count); ++i) {
                    const int n = srcCount[static_cast<size_t>(i)];
                    if (n == 0) {
                        continue;
                    }
                    std::snprintf(sbuf, sizeof(sbuf), "%s%s %d",
                                  srcText.empty() ? "——" : "、",
                                  SourceNameZh(static_cast<EntrySource>(i)),
                                  n);
                    srcText += sbuf;
                }
                t += srcText;
            }
            const auto net = in.ledger->TrialBalance();
            std::string details;
            char dbuf[64];
            for (int i = 0; i < static_cast<int>(LedgerAccount::Count); ++i) {
                const int n = net[static_cast<size_t>(i)];
                if (n == 0) {
                    continue;
                }
                std::snprintf(dbuf, sizeof(dbuf), "，%s%s %d",
                              AccountNameZh(static_cast<LedgerAccount>(i)),
                              n > 0 ? "增" : "損", n > 0 ? n : -n);
                details += dbuf;
            }
            t += details;
            t += "。";
        }
        // L-6 登錄字號：帳簿的 rootHash 派生短碼——平衡/存疑皆帶，
        // 玩家可貼出此字號供揭露引用（帳變則字號變）
        t += "登錄字號 " + in.ledger->RegistryId() + "。";
        // L-8 確信結論：seeded 抽驗複算後史官方具結（或不具結）——
        // 相符 / 存疑 / 拒絕三態，對應 ISSA 5000 確信光譜
        if (in.assuranceSample > 0) {
            const LedgerAssurance as =
                AssureLedger(*in.ledger, in.assuranceSeed,
                             in.assuranceSample);
            char abuf[192];
            if (as.verdict == AssuranceVerdict::Adverse) {
                const int at = as.brokenAt >= 0 ? as.brokenAt
                                                : as.sampleBad;
                std::snprintf(
                    abuf, sizeof(abuf),
                    "鏈斷於第 %d 筆——史官拒絕具結，本章帳目不予採信。",
                    at + 1);
            } else if (as.verdict == AssuranceVerdict::Qualified) {
                std::string why;
                if (as.unsoundAt >= 0) {
                    char w[48];
                    std::snprintf(w, sizeof(w), "第 %d 筆借貸不成立",
                                  as.unsoundAt + 1);
                    why = w;
                }
                if (as.suspectCount > 0) {
                    char w[48];
                    std::snprintf(w, sizeof(w), "%s疑帳 %d 筆",
                                  why.empty() ? "有" : "、另有",
                                  as.suspectCount);
                    why += w;
                }
                std::snprintf(abuf, sizeof(abuf),
                              "抽驗 %d 筆皆符，然%s——史官存疑，不予具結。",
                              as.sampled, why.c_str());
            } else {
                std::snprintf(
                    abuf, sizeof(abuf),
                    "抽驗 %d 筆皆符，鏈環相續——史官具結：帳目相符。",
                    as.sampled);
            }
            t += abuf;
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
