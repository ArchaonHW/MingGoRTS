#pragma once

#include "Core/CoreTypes.h"
#include "Gameplay/CampaignLedger.h"
#include "Gameplay/GeneralDossier.h"
#include "Gameplay/MythLog.h"

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * 章節慣例文字層（N-6，narrative-design §Structure Type）
 *
 * 演義章回慣例入 UI：
 *  - 章開題詞：一句詩問，預告本章之問
 *  - 敵將入場判詞：一句人格判斷——聽聞態（接 N-2 GeneralDossier，
 *    已驗證的檔案附「前判有詐」等揭曉語）
 *  - 章尾「欲知後事」鉤：依本章結果生成懸念
 *  - 結局四聲部：霸業 / 仁政 / 無字 / 對手之筆（Fallen）
 *
 * 寫死規則（依 narrative-design §The Ending — Written, Not Chosen）：
 *  - 結局由累積記錄生成，不提供選項
 *  - 墮落滿 → 對手之筆（Fallen）：敵手之筆寫你
 *  - 武功與民心俱高 → 無字：史官無法調和兩帳，冊頁留白
 *  - 武功顯而治績低 → 霸業：編年武功冊語體
 *  - 治績顯而武功收斂 → 仁政：同一支筆的受命一統變體
 *  - 皆未達門檻 → 兩分數取高者歸檔（筆總得落下）
 */
class ChapterConventions {
public:
    // ---- 章開題詞 ----
    // 一句詩問：章序定韻腳、theme 嵌主題；theme 為空用通用問句
    static std::string OpeningVerse(int chapter,
                                    const std::string& theme);

    // ---- 敵將入場判詞 ----
    // 有檔案 → 聽聞體「據報……」；已驗證 → 直書；無檔案 → 「虛實未聞」
    static std::string EntranceJudgment(const GeneralDossier& dossier,
                                        const std::string& generalName);

    // ---- 章尾「欲知後事」鉤 ----
    struct ClosingContext {
        int chapter = 1;                    // 本章序
        bool playerVictory = false;         // 本章勝敗
        bool draw = false;                  // 平手
        std::string nextForeshadow;         // 下回伏筆詞（如「敵帳中那封回信」）
        bool rivalCountered = false;        // N-3：敵方已針對我軍慣性
        int mythEventCount = 0;             // 本章新增神話事件數
        std::string peacePathZh;            // E-4：無戰收場路徑（談判/嚇阻/顛覆），空=戰鬥章節
    };
    static std::string ClosingHook(const ClosingContext& ctx);

    // ---- 結局四聲部 ----
    enum class EndingVoice {
        Conquest,   // 霸業：武功顯、治績低——史官之筆，武功冊語體
        Mandate,    // 仁政：治績顯、武功收斂——同筆受命一統變體
        Unwritten,  // 無字：武功與民心俱高——兩帳相悖，冊頁留白
        Fallen,     // 對手之筆：墮落滿——敵手之筆寫你
    };

    struct EndingInput {
        int annihilated = 0;        // 累計殲滅敵隊數（武功）
        int subdued = 0;            // 累計收服/潰逃敵隊數（武功的另一種寫法）
        float popularSupport = 0.f; // 民心 0~100（治績）
        float civilOrder = 0.f;     // 秩序 0~100（治績）
        float corruption = 0.f;     // 墮落 0~100
        int peaceChapters = 0;      // E-4：無戰而定的章節數——計入收服側
    };

    // 門檻（寫死，與 SageCommand 治平判定同源）
    static constexpr float kFallenCorruption = 100.0f;
    static constexpr float kHeartsThreshold = 60.0f;   // 民心/秩序俱 ≥ 此為治績顯
    // 武功顯 = 殲滅數 > 收服數（以殺止殺的戰法為主）

    // 四聲部判定：墮落 → 俱高無字 → 單高歸檔 → 皆低取分高者
    static EndingVoice ResolveEnding(const EndingInput& in);

    // 結局文生成：聲部主文 + 帳本引用（處置/稱號）+ 神話證詞
    // 無字：主文為空（史官擱筆），只剩累積記錄自身的聲音
    static std::string ComposeEnding(EndingVoice voice,
                                     const CampaignLedger& ledger,
                                     const MythLog& myths);

    static const char* VoiceName(EndingVoice v);
};

} // namespace Gameplay
} // namespace Potato
