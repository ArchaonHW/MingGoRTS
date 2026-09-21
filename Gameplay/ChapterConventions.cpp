#include "ChapterConventions.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

// ---- 題詞韻庫 ----
// 演義體詩問：主題嵌句在前、章問在後。theme 為空時用通用問句。
static const char* kVerseFrames[] = {
    "滾滾長江東逝水，%s——且看誰人先落筆？",
    "青山依舊在，%s——這一回勝負怎生寫？",
    "天下大勢分合久，%s——是筆錄人，還是人錄筆？",
    "成敗轉頭空，%s——帳上一筆，是誰的功過？",
    "浪花淘盡英雄，%s——可有人記得無名者？",
};
static const char* kVerseFallback = "古今多少事";

std::string ChapterConventions::OpeningVerse(int chapter,
                                             const std::string& theme) {
    if (chapter < 1) chapter = 1;
    const size_t n = sizeof(kVerseFrames) / sizeof(kVerseFrames[0]);
    const char* frame = kVerseFrames[(size_t)(chapter - 1) % n];
    const std::string t = theme.empty() ? kVerseFallback : theme;
    char buf[256];
    std::snprintf(buf, sizeof(buf), frame, t.c_str());
    return buf;
}

// ---- 敵將入場判詞 ----
std::string
ChapterConventions::EntranceJudgment(const GeneralDossier& dossier,
                                     const std::string& generalName) {
    const HearsayEntry* e = dossier.Find(generalName);
    if (!e) {
        return "敵將" + generalName + "——虛實未聞，帳上無此判詞。";
    }
    if (e->verified) {
        // 已驗證：直書判詞；有詐者 dossier 已在 verdict 附「前判有詐」
        return "敵將" + generalName + "——" + e->verdict;
    }
    return "據報敵將" + generalName + "——" + e->verdict;
}

// ---- 章尾「欲知後事」鉤 ----
std::string
ChapterConventions::ClosingHook(const ClosingContext& ctx) {
    std::string hook;
    if (!ctx.peacePathZh.empty()) {
        // E-4 無戰收場：主語換無戰體——不戰之局不入勝負
        if (ctx.peacePathZh == "談判") {
            hook = "談笑罷兵";
        } else if (ctx.peacePathZh == "嚇阻") {
            hook = "不戰而退敵";
        } else if (ctx.peacePathZh == "顛覆") {
            hook = "敵營自潰";
        } else {
            hook = "不戰而定";
        }
    } else if (ctx.draw) {
        hook = "勝負未分，兩軍各自收兵";
    } else if (ctx.playerVictory) {
        hook = "我軍奏凱";
    } else {
        hook = "我軍折戟";
    }
    if (ctx.mythEventCount > 0) {
        hook += "，祠中異象已錄";
    }
    if (ctx.rivalCountered) {
        hook += "，敵帳軍師似已窺破我軍慣性";
    }
    hook += "——欲知";
    hook += ctx.nextForeshadow.empty() ? "後事如何" : ctx.nextForeshadow;
    hook += "，且聽下回分解。";
    return hook;
}

// ---- 結局四聲部 ----
ChapterConventions::EndingVoice
ChapterConventions::ResolveEnding(const EndingInput& in) {
    if (in.corruption >= kFallenCorruption) {
        return EndingVoice::Fallen;
    }
    // E-4：無戰章節計入收服側——不戰而勝是收服的最高形式
    const int subdued = in.subdued + in.peaceChapters;
    const bool conquestHigh = in.annihilated > subdued;
    const bool heartsHigh = in.popularSupport >= kHeartsThreshold &&
                            in.civilOrder >= kHeartsThreshold;
    if (conquestHigh && heartsHigh) {
        return EndingVoice::Unwritten; // 兩帳相悖
    }
    if (conquestHigh) return EndingVoice::Conquest;
    if (heartsHigh) return EndingVoice::Mandate;
    // 皆未達檻：兩分數取高者——筆總得落下
    const float conquestScore =
        (float)in.annihilated + 0.5f * (float)subdued;
    const float heartsScore = in.popularSupport + in.civilOrder;
    return conquestScore * kHeartsThreshold * 2.0f >= heartsScore
               ? EndingVoice::Conquest
               : EndingVoice::Mandate;
}

const char* ChapterConventions::VoiceName(EndingVoice v) {
    switch (v) {
    case EndingVoice::Conquest: return "霸業";
    case EndingVoice::Mandate: return "仁政";
    case EndingVoice::Unwritten: return "無字";
    case EndingVoice::Fallen: return "對手之筆";
    }
    return "?";
}

std::string
ChapterConventions::ComposeEnding(EndingVoice voice,
                                  const CampaignLedger& ledger,
                                  const MythLog& myths) {
    std::string text;
    switch (voice) {
    case EndingVoice::Conquest:
        text = "史曰：兵鋒所至，群雄束手。";
        break;
    case EndingVoice::Mandate: {
        text = "史曰：不戰而屈，民心所歸。";
        // E-4 無戰軌跡：帳本處置記錄數無戰章節——
        // Negotiated/Intimidated/Defected 皆不戰之勝
        int peaceN = 0;
        for (const auto& rec : ledger.Records()) {
            if (rec.disposition == GeneralDisposition::Negotiated ||
                rec.disposition == GeneralDisposition::Intimidated ||
                rec.disposition == GeneralDisposition::Defected) {
                ++peaceN;
            }
        }
        if (peaceN > 0) {
            char buf[64];
            std::snprintf(buf, sizeof(buf),
                          "凡 %d 章不戰而屈，仁者之師也。", peaceN);
            text += "\n";
            text += buf;
        }
        break;
    }
    case EndingVoice::Unwritten:
        // 史官擱筆：武功與民心俱高，兩帳相悖，冊頁留白——
        // 只剩累積記錄自身的聲音。
        break;
    case EndingVoice::Fallen:
        text = "（此頁非史官之筆）敵手代筆曰：其筆鋒，已是我鋒。";
        break;
    }
    if (!text.empty()) {
        text += "\n";
    }
    for (const std::string& c : ledger.EndingCitations()) {
        text += c;
        text += "\n";
    }
    for (const std::string& t : myths.TestimonyLines()) {
        text += t;
        text += "\n";
    }
    return text;
}

} // namespace Gameplay
} // namespace Potato
