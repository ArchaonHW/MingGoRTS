#pragma once

// 無戰章節判定器（E-1，epics Story E.1/E.2/E.3）——
// 「至聖者無戰」從結局評語升格為戰前可選路徑：
//   談判（民心消費）/ 嚇阻（軍威存在即判定）/ 顛覆（情報兌換內應）。
//
// CheckOptions 產生灰顯可見的選項報表——門檻與現值全列出，
// 未達標路徑 available=false 但不隱藏；UI 可自行決定鎖不鎖，
// 強行嘗試由 Resolve 判為 Failed（代價明確：敵獲首波優勢）。
//
// Resolve 把每次判定寫進 LedgerChain（Record-is-Truth——
// 成功、失敗、中計皆留分錄），成功路徑另記敵將處置。
//
// 分層：本模組在 Campaign 層，讀 Gameplay 帳本/判詞檔案與
// ChapterDef 定義塊；不依賴 BattleController——無戰判定
// 跳過戰鬥狀態機，enemyAdvantage 旗標交戰鬥層兌現首波優勢。

#include "Gameplay/CampaignLedger.h"
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/Ledger.h"

#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

struct ChapterDef;

enum class NoBattlePath {
    Negotiation, // 談判——民心消費換罷兵
    Deterrence,  // 嚇阻——軍威存在即判定
    Subversion,  // 顛覆——verified 情報兌換內應
};

enum class NoBattleVerdict {
    NotOffered, // 章節未定義此路徑——不寫帳、無代價
    Success,    // 門檻通過——跳過戰鬥直進結算
    Failed,     // 強行嘗試未過門檻——進戰鬥、敵首波優勢
    Backfired,  // 顛覆中計（帳本含未駁斥假情報）——進戰鬥、敵首波優勢
};

const char* NoBattlePathName(NoBattlePath p);
const char* NoBattlePathNameZh(NoBattlePath p);
const char* NoBattleVerdictName(NoBattleVerdict v);

// 判定輸入快照：由呼叫端從既有系統匯入（resolver 無狀態）。
// Gather() 是標準匯入法——帳本試算淨額 + 判詞檔案計數。
struct NoBattleContext {
    int civilBalance = 0;          // LedgerChain 試算 Civil 淨額（民心）
    int armyBalance = 0;           // Army 淨額（軍威）
    int verifiedIntel = 0;         // 該敵將 verified && !planted 判詞數
    bool plantedUnrefuted = false; // 該敵將 planted && !verified 判詞存在
    std::string chapterId;         // provenance eventId 前綴
    int chapter = 0;               // 分錄章節序
    std::string generalId;         // 敵將處置記錄 id
    std::string generalName;       // 敵將名（處置記錄/分錄備忘）
};

// 灰顯可見報表：章節定義的每條路徑一筆——門檻與現值皆透明
struct NoBattleOptionInfo {
    NoBattlePath path;
    bool available = false; // 現值達門檻（offered 恆真——未定義者不列）
    int current = 0;        // 現值（民心/軍威/情報條數）
    int threshold = 0;      // 門檻（cost/threshold/intel）
    std::string reason;     // 人讀說明（UI 灰顯 tooltip 用）
};

struct NoBattleResult {
    NoBattlePath path;
    NoBattleVerdict verdict = NoBattleVerdict::NotOffered;
    bool enemyAdvantage = false; // 失敗/中計 → 敵首波優勢
    int spent = 0;               // 實際消費量（談判=民心，餘為象徵一錄）
    Gameplay::GeneralDisposition disposition =
        Gameplay::GeneralDisposition::Unknown; // 成功時記下的敵將處置
    std::string summary;                       // 一句話結果（戰報/UI 用）
};

class NoBattleResolver {
public:
    // 彙整輸入：帳本 Civil/Army 試算淨額 + 該敵將判詞檔案計數。
    // verifiedIntel = verified && !planted 條數；
    // plantedUnrefuted = planted && !verified（未驗證的假情報才是陷阱，
    // 驗證過的 planted 已駁斥揭曉）。
    static NoBattleContext Gather(
        const Gameplay::LedgerChain& ledger,
        const Gameplay::GeneralDossier& dossier,
        const std::string& generalName);

    // 灰顯可見：章節定義開放的每條路徑產一筆報表；未定義者不列
    std::vector<NoBattleOptionInfo> CheckOptions(
        const ChapterDef& def, const NoBattleContext& ctx) const;

    // 判定並入帳：Success/Failed/Backfired 皆寫分錄進 ledger，
    // Success 另記敵將處置進 campaign。NotOffered 不寫任何東西。
    // 分錄 provenance：source=路徑對應 EntrySource、
    // tick=章節序、eventId="<chapterId>:<path>"——判定出處可查。
    NoBattleResult Resolve(NoBattlePath path, const ChapterDef& def,
                           const NoBattleContext& ctx,
                           Gameplay::LedgerChain& ledger,
                           Gameplay::CampaignLedger& campaign) const;
};

} // namespace Campaign
} // namespace Potato
