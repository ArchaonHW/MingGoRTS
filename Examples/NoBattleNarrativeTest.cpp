// NoBattleNarrativeTest — E-4 無戰章節敘事與結算驗證（headless）
// 覆蓋：HistorianReport 和平語域（三路徑句/去向/查帳段在場）、
// ClosingHook 無戰體、ResolveEnding 無戰計入收服側、
// ComposeEnding Mandate 附「凡 N 章不戰而屈」。

#include "Gameplay/ChapterConventions.h"
#include "Gameplay/HistorianReport.h"
#include "Gameplay/Ledger.h"

#include <cstdio>
#include <string>

using namespace Potato::Gameplay;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) {
        printf("  [PASS] %s\n", name);
    } else {
        printf("  [FAIL] %s\n", name);
        ++failures;
    }
}

static bool Has(const std::string& s, const char* sub) {
    return s.find(sub) != std::string::npos;
}

static LedgerChain PeaceLedger() {
    LedgerChain chain;
    chain.Append(LedgerEntry::Negotiation(30, 2, "談判罷兵"));
    return chain;
}

int main() {
    printf("=== No-Battle Narrative Tests (E-4) ===\n");

    // ---- [1] 和平語域：談判 ----
    printf("\n[1] 談判語域\n");
    {
        LedgerChain chain = PeaceLedger();
        HistorianInput in;
        in.battleName = "議和鄉";
        in.peacePathZh = "談判";
        in.peaceGeneral = "格洛克";
        in.ledger = &chain;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "議和鄉之局，兵不血刃"), "和平語域開篇");
        Check(Has(r.text, "遣使入帳") && Has(r.text, "「格洛克」罷兵言和"),
              "談判路徑句");
        Check(Has(r.text, "議和而去，不入陣亡錄"), "去向句（不列陣亡）");
        Check(Has(r.text, "借貸相符"), "查帳段在場（無戰也有帳）");
        Check(Has(r.text, "談判 1"), "來源統計含談判筆");
        Check(Has(r.text, "本報告省略"), "省略計數在場");
        Check(!Has(r.text, "戰歷"), "無戰歷秒數行");
        Check(Has(r.text, "不戰而定"), "戰歷行改寫不戰而定");
    }

    // ---- [2] 和平語域：嚇阻/顛覆 + 泛稱降級 ----
    printf("\n[2] 嚇阻/顛覆語域\n");
    {
        HistorianInput in;
        in.battleName = "威遠";
        in.peacePathZh = "嚇阻";
        in.peaceGeneral = "格洛克";
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "軍威所懾") && Has(r.text, "未交一矢引兵而退"),
              "嚇阻路徑句");
        Check(Has(r.text, "懾服而去，不入陣亡錄"), "嚇阻去向句");

        in.peacePathZh = "顛覆";
        r = ComposeHistorianReport(in);
        Check(Has(r.text, "內應發於敵帳") && Has(r.text, "眾叛親離"),
              "顛覆路徑句");
        Check(Has(r.text, "變節投誠，不入陣亡錄"), "顛覆去向句");

        in.peaceGeneral.clear();
        r = ComposeHistorianReport(in);
        Check(Has(r.text, "「敵將」眾叛親離"), "無敵將名→泛稱降級");
    }

    // ---- [3] 戰鬥語域不迴歸 ----
    printf("\n[3] 戰鬥語域不變\n");
    {
        HistorianInput in;
        in.battleName = "斷橋之役";
        in.outcome = BattleOutcome::Victory;
        in.elapsedSec = 120.0f;
        HistorianReport r = ComposeHistorianReport(in);
        Check(Has(r.text, "斷橋之役，我軍克敵"), "戰鬥語域開篇不變");
        Check(Has(r.text, "戰歷 120 秒"), "戰歷行在場");
        Check(!Has(r.text, "兵不血刃"), "戰鬥語域無和平句式");
    }

    // ---- [4] 章尾鉤無戰體 ----
    printf("\n[4] ClosingHook 無戰體\n");
    {
        ChapterConventions::ClosingContext ctx;
        ctx.chapter = 2;
        ctx.peacePathZh = "談判";
        Check(Has(ChapterConventions::ClosingHook(ctx), "談笑罷兵"),
              "談判鉤句");
        ctx.peacePathZh = "嚇阻";
        Check(Has(ChapterConventions::ClosingHook(ctx), "不戰而退敵"),
              "嚇阻鉤句");
        ctx.peacePathZh = "顛覆";
        Check(Has(ChapterConventions::ClosingHook(ctx), "敵營自潰"),
              "顛覆鉤句");
        ctx.peacePathZh.clear();
        ctx.playerVictory = true;
        Check(Has(ChapterConventions::ClosingHook(ctx), "我軍奏凱"),
              "戰鬥鉤句不變");
        Check(Has(ChapterConventions::ClosingHook(ctx), "且聽下回分解"),
              "下回分解尾綴不變");
    }

    // ---- [5] 結局軌跡：無戰計入收服側 ----
    printf("\n[5] ResolveEnding 無戰軌跡\n");
    {
        ChapterConventions::EndingInput in;
        in.annihilated = 5;
        in.subdued = 3;
        in.popularSupport = 50.f; // 治績未達
        in.civilOrder = 50.f;
        in.corruption = 0.f;
        // annihilated(5) > subdued(3) → 武功顯、治績低 → Conquest
        Check(ChapterConventions::ResolveEnding(in) ==
                  ChapterConventions::EndingVoice::Conquest,
              "無無戰章節→霸業");
        // +3 無戰章節 → subdued 等效 6 > 5 → 武功不顯、治績低 →
        // 皆未達檔取分高者：武功 5+3=8×120=960 vs 治績 100 → 仍霸業？
        // 實際：conquestScore*120*2 = 8*120=960 vs 100 → Conquest 仍。
        // 改用臨界場景：annihilated=5 subdued=3 peace=2 → 5>5 假 → Mandate?
        // heartsHigh 仍需民心秩序≥60——皆低 → 分數決勝：
        // conquest (5+0.5*5)*120=900 vs hearts 100 → Conquest。
        // 讓民心秩序過檻才翻 Mandate：
        in.popularSupport = 70.f;
        in.civilOrder = 70.f;
        in.peaceChapters = 3;
        // annihilated(5) vs subdued 等效 6 → 武功不顯；治績顯 → Mandate
        Check(ChapterConventions::ResolveEnding(in) ==
                  ChapterConventions::EndingVoice::Mandate,
              "無戰章節計入收服側→仁政");
        // 對照：同治績但無無戰章節 → 武功顯+治績顯 → 無字
        in.peaceChapters = 0;
        Check(ChapterConventions::ResolveEnding(in) ==
                  ChapterConventions::EndingVoice::Unwritten,
              "同治績無無戰→兩帳相悖無字");
    }

    // ---- [6] ComposeEnding Mandate 附無戰軌跡句 ----
    printf("\n[6] ComposeEnding 附句\n");
    {
        CampaignLedger ledger;
        ledger.RecordDisposition("glock", "格洛克", 2,
                                 GeneralDisposition::Negotiated);
        ledger.RecordDisposition("spy", "申不害", 4,
                                 GeneralDisposition::Defected);
        MythLog myths;
        std::string text = ChapterConventions::ComposeEnding(
            ChapterConventions::EndingVoice::Mandate, ledger, myths);
        Check(Has(text, "凡 2 章不戰而屈"), "Mandate 附無戰軌跡句");
        Check(Has(text, "民心所歸"), "Mandate 主文不變");

        CampaignLedger empty;
        text = ChapterConventions::ComposeEnding(
            ChapterConventions::EndingVoice::Mandate, empty, myths);
        Check(!Has(text, "不戰而屈，仁者之師"), "無無戰記錄不附句");

        // 陣亡不計入無戰軌跡
        CampaignLedger war;
        war.RecordDisposition("a", "甲", 1, GeneralDisposition::Slain);
        text = ChapterConventions::ComposeEnding(
            ChapterConventions::EndingVoice::Mandate, war, myths);
        Check(!Has(text, "不戰而屈，仁者之師"), "陣亡不算無戰");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
