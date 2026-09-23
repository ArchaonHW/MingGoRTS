#pragma once

// EndingPage —— C-5 四手結局生成（戰役結束時的帳目判決頁）
//
// 聚合 CampaignState 全量帳目 → ChapterConventions::EndingInput
// → ResolveEnding 選聲部 → ComposeEnding 產文。
//
// 映射（全部由帳本推導，無真結局標記）：
//   武功（殲滅）  = disposition Slain 數
//   收服          = Subdued/Retired + 無戰處置（見下）
//   無戰章節      = Negotiated/Intimidated/Defected 記錄涵蓋的
//                 相異章節數——議和/懾服/內應倒戈皆「不戰而定」
//   民心/秩序     = Governance 快照
//   墮落          = Depravity ratchet（只增不減，滿 → 對手之筆）
//
// 神話證詞經 myths 參數注入（殼層持有 MythLog；本類不擁有）。

#include "Gameplay/ChapterConventions.h"

#include <string>

namespace Potato {
namespace Campaign {

class CampaignState;

struct EndingResult {
    Gameplay::ChapterConventions::EndingVoice voice;
    std::string text;        // 主文 + 帳本引用 + 神話證詞
    std::string voiceName;   // 聲部中名（霸業/仁政/無字/對手之筆）
    int annihilated = 0;     // 殲滅數（供 UI/測試對帳）
    int subdued = 0;
    int peaceChapters = 0;
};

class EndingPage {
public:
    // myths 可為 nullptr（無神話事件時證詞段為空）
    static EndingResult Generate(const CampaignState& campaign,
                                 const Gameplay::MythLog* myths);
};

} // namespace Campaign
} // namespace Potato
