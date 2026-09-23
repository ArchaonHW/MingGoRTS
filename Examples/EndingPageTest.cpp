// EndingPageTest — C-5 四手結局生成無頭測試。
// 覆蓋：四聲部判定經帳目聚合、無字矛盾、墮落對手之筆、
// 無戰章節計數（相異章節去重）、證詞引用、空 MythLog 安全。
#include "Campaign/EndingPage.h"
#include "Campaign/CampaignState.h"
#include "Gameplay/CampaignLedger.h"
#include "Gameplay/GovernanceEvent.h"
#include "Gameplay/MythLog.h"

#include <cstdio>
#include <string>
#include <unordered_map>

using Potato::Campaign::CampaignState;
using Potato::Campaign::EndingPage;
using Potato::Gameplay::ChapterConventions;
using Potato::Gameplay::GeneralDisposition;
using Potato::Gameplay::MythLog;
using EV = ChapterConventions::EndingVoice;

static int failures = 0;
static void Check(bool cond, const char* name) {
    if (cond) { std::printf("[PASS] %s\n", name); return; }
    std::printf("[FAIL] %s\n", name);
    ++failures;
}

int main() {
    // [1] 霸業：Slain 多於收服、治理低
    {
        CampaignState cs;
        cs.Ledger().RecordDisposition("g1", "甲將", 1,
                                      GeneralDisposition::Slain);
        cs.Ledger().RecordDisposition("g2", "乙將", 2,
                                      GeneralDisposition::Slain);
        cs.Ledger().RecordDisposition("g3", "丙將", 3,
                                      GeneralDisposition::Subdued);
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.voice == EV::Conquest, "slain>subdued -> Conquest");
        Check(r.annihilated == 2 && r.subdued == 1,
              "disposition counts");
        Check(r.voiceName == "霸業", "voice name zh");
    }
    // [2] 無字：武功與民心俱高 → 空白頁
    {
        CampaignState cs;
        cs.Ledger().RecordDisposition("g1", "甲將", 1,
                                      GeneralDisposition::Slain);
        cs.Gov().AdjustPopularSupport(80.0f);
        cs.Gov().AdjustCivilOrder(80.0f);
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.voice == EV::Unwritten, "both high -> Unwritten");
    }
    // [3] 對手之筆：墮落滿（Atrocity ratchet 只增不減）
    {
        CampaignState cs;
        std::unordered_map<Potato::Gameplay::GovernanceEvent, int> ev;
        ev[Potato::Gameplay::GovernanceEvent::Atrocity] = 40;
        cs.Gov().Accumulate(ev); // 5×0.5×40 = 100
        Check(cs.Gov().Depravity() >= 100.0f, "depravity ratchet");
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.voice == EV::Fallen, "depravity max -> Fallen");
    }
    // [4] 仁政：收服為主 + 治績顯
    {
        CampaignState cs;
        cs.Ledger().RecordDisposition("g1", "甲將", 1,
                                      GeneralDisposition::Subdued);
        cs.Ledger().RecordDisposition("g2", "乙將", 2,
                                      GeneralDisposition::Retired);
        cs.Gov().AdjustPopularSupport(70.0f);
        cs.Gov().AdjustCivilOrder(70.0f);
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.voice == EV::Mandate, "subdued+gov -> Mandate");
    }
    // [5] 無戰處置計入收服 + peaceChapters 相異章節去重
    {
        CampaignState cs;
        cs.Ledger().RecordDisposition("g1", "甲將", 2,
                                      GeneralDisposition::Negotiated);
        cs.Ledger().RecordDisposition("g2", "乙將", 2,
                                      GeneralDisposition::Intimidated);
        cs.Ledger().RecordDisposition("g3", "丙將", 4,
                                      GeneralDisposition::Defected);
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.subdued == 3, "peace dispositions count subdued");
        Check(r.peaceChapters == 2, "peace chapters dedup by chapter");
    }
    // [6] 結局文含帳本引用（處置/稱號）
    {
        CampaignState cs;
        cs.Ledger().RecordDisposition("g1", "格洛克", 1,
                                      GeneralDisposition::Slain);
        auto r = EndingPage::Generate(cs, nullptr);
        Check(r.text.find("格洛克") != std::string::npos,
              "ending cites general name");
    }
    // [7] 神話證詞經 MythLog 注入
    {
        CampaignState cs;
        MythLog myths;
        myths.Record("斷橋祠", "狐仙", "第一章", "神像流淚");
        auto r = EndingPage::Generate(cs, &myths);
        Check(r.text.find("狐仙") != std::string::npos,
              "myth testimony in ending text");
    }
    std::printf("%s\n", failures == 0 ? "ALL PASS" : "FAILURES");
    return failures == 0 ? 0 : 1;
}
