// NoBattleResolverTest — E-1 無戰章節定義與門檻驗證（headless）
// 覆蓋：no_battle 定義塊解析/降級、CheckOptions 灰顯報表、
// 談判/嚇阻/顛覆三路徑判定、失敗與中計的帳面代價、
// Gather 彙整、分錄 provenance、Record-is-Truth 全留痕。

#include "Campaign/ChapterLibrary.h"
#include "Campaign/NoBattleResolver.h"
#include "Gameplay/EnemyGeneral.h"
#include "Gameplay/GeneralDossier.h"

#include <cstdio>
#include <string>

using namespace Potato::Campaign;
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

static ChapterDef MakeChapter(const char* nb) {
    ChapterDef def;
    std::string j = R"({"schema":"potato.campaign_chapter/1",
        "id":"talkville","name":"議和鄉","arc":0,"chapter":2,
        "map":"maps/duanqiao.json")";
    if (nb) {
        j += ",\"no_battle\":";
        j += nb;
    }
    j += "}";
    def.LoadFromString(j);
    return def;
}

// 建一條有 Civil/Army 淨額的帳本（手寫分錄，不走模板）
static LedgerChain MakeLedger(int civil, int army) {
    LedgerChain chain;
    if (civil > 0) {
        LedgerEntry e;
        e.debit = LedgerAccount::Civil;
        e.credit = LedgerAccount::Supply;
        e.amount = civil;
        e.memo = "民心入帳";
        chain.Append(e);
    }
    if (army > 0) {
        LedgerEntry e;
        e.debit = LedgerAccount::Army;
        e.credit = LedgerAccount::Supply;
        e.amount = army;
        e.memo = "軍威入帳";
        chain.Append(e);
    }
    return chain;
}

static NoBattleContext MakeCtx(int civil, int army, int intel,
                               bool planted = false) {
    NoBattleContext ctx;
    ctx.civilBalance = civil;
    ctx.armyBalance = army;
    ctx.verifiedIntel = intel;
    ctx.plantedUnrefuted = planted;
    ctx.chapterId = "talkville";
    ctx.chapter = 2;
    ctx.generalId = "glock";
    ctx.generalName = "格洛克";
    return ctx;
}

int main() {
    printf("=== No-Battle Resolver Tests (E-1) ===\n");

    // ---- [1] no_battle 定義塊解析 ----
    printf("\n[1] 定義塊解析\n");
    {
        ChapterDef def = MakeChapter(R"({
            "negotiation":{"cost":30},
            "deterrence":{"threshold":60},
            "subversion":{"intel":1}})");
        Check(def.noBattle.Offered(), "三路徑開放");
        Check(def.noBattle.negotiation.enabled &&
                  def.noBattle.negotiation.requirement == 30,
              "談判 cost 解析");
        Check(def.noBattle.deterrence.requirement == 60, "嚇阻門檻解析");
        Check(def.noBattle.subversion.requirement == 1, "顛覆情報數解析");
        Check(def.warnings.empty(), "合法定義無警告");
    }

    // ---- [2] 相容與降級 ----
    printf("\n[2] 相容與降級\n");
    {
        ChapterDef old = MakeChapter(nullptr);
        Check(!old.noBattle.Offered(), "舊檔無 no_battle——純戰鬥章節");

        ChapterDef bad = MakeChapter("42");
        Check(!bad.noBattle.Offered(), "no_battle 非物件→關閉");
        Check(!bad.warnings.empty(), "非物件記警告");

        ChapterDef badOpt =
            MakeChapter(R"({"negotiation":{"cost":"x"},
                "deterrence":{"threshold":0},
                "subversion":{"intel":2}})");
        Check(!badOpt.noBattle.negotiation.enabled &&
                  !badOpt.noBattle.deterrence.enabled,
              "壞門檻路徑關閉");
        Check(badOpt.noBattle.subversion.enabled &&
                  badOpt.noBattle.subversion.requirement == 2,
              "好路徑不受壞路徑拖累");
        Check(badOpt.warnings.size() >= 2, "壞欄逐條記警告");
    }

    // ---- [3] CheckOptions 灰顯可見 ----
    printf("\n[3] 灰顯可見報表\n");
    {
        ChapterDef def = MakeChapter(R"({
            "negotiation":{"cost":30},
            "deterrence":{"threshold":60},
            "subversion":{"intel":1}})");
        NoBattleResolver res;
        // 民心不足、軍威夠、無情報
        auto opts = res.CheckOptions(def, MakeCtx(10, 70, 0));
        Check(opts.size() == 3, "三選項全列出（不隱藏）");
        Check(!opts[0].available && opts[0].current == 10 &&
                  opts[0].threshold == 30,
              "談判灰顯：現值/門檻透明");
        Check(opts[1].available, "嚇阻達標可選");
        Check(!opts[2].available && opts[2].current == 0,
              "顛覆無情報灰顯");
        Check(opts[0].reason.find("民心不足") != std::string::npos,
              "灰顯附人讀原因");
    }

    // ---- [4] 談判：成功消費民心、失敗虛耗 ----
    printf("\n[4] 談判判定\n");
    {
        ChapterDef def = MakeChapter(R"({"negotiation":{"cost":30}})");
        NoBattleResolver res;
        LedgerChain chain = MakeLedger(50, 0);
        CampaignLedger camp;
        NoBattleResult r =
            res.Resolve(NoBattlePath::Negotiation, def, MakeCtx(50, 0, 0),
                        chain, camp);
        Check(r.verdict == NoBattleVerdict::Success, "民心夠→談判成");
        Check(r.spent == 30, "消費=門檻 cost");
        Check(!r.enemyAdvantage, "成功無敵方優勢");
        Check(r.disposition == GeneralDisposition::Negotiated,
              "敵將記 Negotiated");
        Check(chain.Size() == 2, "談判分錄入帳");
        const LedgerEntry& e = chain.Entries().back().entry;
        Check(e.debit == LedgerAccount::Fate &&
                  e.credit == LedgerAccount::Civil && e.amount == 30,
              "分錄 借Fate貸Civil=30");
        Check(e.prov.source == EntrySource::Negotiation &&
                  e.prov.eventId == "talkville:Negotiation",
              "provenance 記出處");
        Check(camp.Find("glock") != nullptr &&
                  camp.Find("glock")->disposition ==
                      GeneralDisposition::Negotiated,
              "處置入戰役帳");

        // 民心不足強行談判 → Failed + 虛耗 + 敵優勢
        LedgerChain poor = MakeLedger(10, 0);
        CampaignLedger camp2;
        NoBattleResult f = res.Resolve(NoBattlePath::Negotiation, def,
                                       MakeCtx(10, 0, 0), poor, camp2);
        Check(f.verdict == NoBattleVerdict::Failed, "民心不足強行→失敗");
        Check(f.enemyAdvantage, "失敗敵獲首波優勢");
        Check(f.spent == 7, "虛耗=cost/4（30/4=7）");
        Check(poor.Size() == 2, "失敗也入帳（Record-is-Truth）");
        Check(poor.Entries().back().entry.credit == LedgerAccount::Civil,
              "失敗分錄貸 Civil");
        Check(camp2.Find("glock") == nullptr, "失敗不記處置");
    }

    // ---- [5] 嚇阻：軍威存在即判定 ----
    printf("\n[5] 嚇阻判定\n");
    {
        ChapterDef def = MakeChapter(R"({"deterrence":{"threshold":60}})");
        NoBattleResolver res;
        LedgerChain chain = MakeLedger(0, 70);
        CampaignLedger camp;
        NoBattleResult r =
            res.Resolve(NoBattlePath::Deterrence, def, MakeCtx(0, 70, 0),
                        chain, camp);
        Check(r.verdict == NoBattleVerdict::Success, "軍威達標→嚇阻成");
        Check(r.disposition == GeneralDisposition::Intimidated,
              "敵將記 Intimidated");
        const LedgerEntry& e = chain.Entries().back().entry;
        Check(e.amount == 1 && e.prov.source == EntrySource::Deterrence,
              "象徵一錄 + 來源嚇阻");
        // 軍威不損：淨額不減（70 入帳 + 象徵借 1 → 71）
        Check(chain.TrialBalance()[static_cast<size_t>(
                  LedgerAccount::Army)] == 71,
              "軍威淨額不減");

        LedgerChain weak = MakeLedger(0, 20);
        CampaignLedger camp2;
        NoBattleResult f = res.Resolve(NoBattlePath::Deterrence, def,
                                       MakeCtx(0, 20, 0), weak, camp2);
        Check(f.verdict == NoBattleVerdict::Failed && f.enemyAdvantage,
              "軍威不足強行→失敗+敵優勢");
        Check(weak.Entries().back().entry.prov.source ==
                  EntrySource::Deterrence,
              "失敗分錄仍記嚇阻來源");
    }

    // ---- [6] 顛覆：情報兌換與中計 ----
    printf("\n[6] 顛覆判定\n");
    {
        ChapterDef def = MakeChapter(R"({"subversion":{"intel":1}})");
        NoBattleResolver res;
        LedgerChain chain;
        CampaignLedger camp;
        NoBattleResult r =
            res.Resolve(NoBattlePath::Subversion, def, MakeCtx(0, 0, 1),
                        chain, camp);
        Check(r.verdict == NoBattleVerdict::Success, "verified 達標→顛覆成");
        Check(r.disposition == GeneralDisposition::Defected,
              "敵將記 Defected");
        Check(chain.Entries().back().entry.debit == LedgerAccount::Army &&
                  chain.Entries().back().entry.credit == LedgerAccount::Fate,
              "顛覆分錄 借Army貸Fate");

        // 中計：verified 達標但有未駁斥 planted
        LedgerChain chain2;
        CampaignLedger camp2;
        NoBattleResult b = res.Resolve(NoBattlePath::Subversion, def,
                                       MakeCtx(0, 0, 1, true), chain2,
                                       camp2);
        Check(b.verdict == NoBattleVerdict::Backfired, "未駁斥假情報→中計");
        Check(b.enemyAdvantage, "中計敵獲首波優勢");
        Check(chain2.Size() == 1, "中計也入帳");
        Check(camp2.Find("glock") == nullptr, "中計不記處置");

        // 情報不足強行 → Failed
        LedgerChain chain3;
        CampaignLedger camp3;
        NoBattleResult f = res.Resolve(NoBattlePath::Subversion, def,
                                       MakeCtx(0, 0, 0), chain3, camp3);
        Check(f.verdict == NoBattleVerdict::Failed && f.enemyAdvantage,
              "情報不足強行→失敗");
    }

    // ---- [7] NotOffered 不寫帳不罰 ----
    printf("\n[7] 未定義路徑\n");
    {
        ChapterDef def = MakeChapter(R"({"negotiation":{"cost":30}})");
        NoBattleResolver res;
        LedgerChain chain = MakeLedger(0, 70);
        CampaignLedger camp;
        NoBattleResult r =
            res.Resolve(NoBattlePath::Deterrence, def, MakeCtx(0, 70, 0),
                        chain, camp);
        Check(r.verdict == NoBattleVerdict::NotOffered, "未定義→NotOffered");
        Check(!r.enemyAdvantage, "NotOffered 無敵方優勢");
        Check(chain.Size() == 1, "NotOffered 不寫帳");
        auto opts = res.CheckOptions(def, MakeCtx(0, 70, 0));
        Check(opts.size() == 1, "報表只列已定義路徑");
    }

    // ---- [8] Gather 彙整 ----
    printf("\n[8] Gather 彙整\n");
    {
        LedgerChain chain = MakeLedger(45, 70);
        GeneralDossier dossier;
        EnemyGeneral g = EnemyGeneral::MakeGlock(); // 格洛克 cunning=10
        dossier.Hear(g);                            // 傳聞未驗證
        NoBattleContext c1 =
            NoBattleResolver::Gather(chain, dossier, g.GetName());
        Check(c1.civilBalance == 45 && c1.armyBalance == 70,
              "Gather 取試算淨額");
        Check(c1.verifiedIntel == 0 && !c1.plantedUnrefuted,
              "未驗證傳聞不算情報");

        dossier.Verify(g); // 觀測驗證→真相
        NoBattleContext c2 =
            NoBattleResolver::Gather(chain, dossier, g.GetName());
        Check(c2.verifiedIntel == 1, "verified 判詞計入");

        // 狡詐敵將放假判詞：cunning≥70 → planted
        EnemyGeneral spy;
        spy.LoadFromString(
            R"({"name":"申不害","personality":{"cunning":80}})");
        GeneralDossier dossier2;
        dossier2.Hear(spy); // planted && !verified → 未駁斥陷阱
        NoBattleContext c3 =
            NoBattleResolver::Gather(chain, dossier2, spy.GetName());
        Check(c3.plantedUnrefuted, "未驗證假判詞=未駁斥陷阱");
        Check(c3.verifiedIntel == 0, "假判詞不算有效情報");

        dossier2.Verify(spy); // 駁斥後不再是陷阱，也不算有效情報
        NoBattleContext c4 =
            NoBattleResolver::Gather(chain, dossier2, spy.GetName());
        Check(!c4.plantedUnrefuted, "已駁斥 planted 不成陷阱");
        Check(c4.verifiedIntel == 0, "已駁斥假判詞仍不算情報");
    }

    // ---- [9] 名稱表與帳面語義 ----
    printf("\n[9] 名稱表\n");
    {
        Check(std::string(NoBattlePathName(NoBattlePath::Subversion)) ==
                  "Subversion",
              "路徑英文名");
        Check(std::string(NoBattlePathNameZh(NoBattlePath::Deterrence)) ==
                  "嚇阻",
              "路徑中文名");
        Check(std::string(NoBattleVerdictName(NoBattleVerdict::Backfired)) ==
                  "Backfired",
              "判定名");
        EntrySource rt = EntrySource::Unknown;
        Check(std::string(SourceName(EntrySource::Deterrence)) ==
                  "Deterrence" &&
                  SourceFromName("Subversion", rt) &&
                  rt == EntrySource::Subversion,
              "EntrySource 新值名稱往返");
        Check(std::string(DispositionName(GeneralDisposition::Intimidated)) ==
                  "Intimidated",
              "Disposition 新值名稱");
    }

    printf("\n=== 結果: %s ===\n", failures == 0 ? "全部 PASS" : "有 FAIL");
    return failures == 0 ? 0 : 1;
}
