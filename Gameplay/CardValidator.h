#pragma once

#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

/**
 * 角色卡驗證器（CardValidator）——G-3 敵將編輯器的 headless 半邊
 *
 * 掃 potato.character_card/1 卡檔給出結構化問題清單：
 *   error —— 載入後語義必錯（壞 JSON、schema 不符、缺 id/name、
 *            未知 trigger/action/unit_class——載入器會靜默降級，
 *            不擋就變成 Always/HoldPosition 隱形卡）
 *   warn  —— 有預設值可降級（人格軸缺漏→50、越界→clamp、
 *            缺 signature/立繪檔、未知 rarity）
 *
 * assetsRoot 給空字串時跳過 art 檔案存在性檢查（單元測試用）；
 * doctrineDir 給空字串時跳過 doctrine 卡池交叉引用檢查。
 */
struct CardIssue {
    bool error = false;   // true=拒存級；false=警告
    std::string field;    // "signatureDoctrine.trigger" 之類路徑
    std::string message;
};

struct CardReport {
    std::string path;
    std::string cardId;
    std::vector<CardIssue> issues;
    bool Ok() const;      // 無 error 即通過（warn 不擋）
};

// --fix 寫回結果：warn 級問題自動修，error 級拒存
struct FixResult {
    bool writable = false;          // 修正後無 error → 可寫回
    bool changed = false;           // 修正造成內容異動
    std::string json;               // 修正後內容（排序鍵、最短 roundtrip）
    CardReport report;              // 修正後再驗的結果（應全為 warn/空）
    std::vector<std::string> fixes; // 逐項修正記錄（審計）
};

class CardValidator {
public:
    // 單卡驗證（json 內容 + 顯示用路徑標籤）
    // doctrineDir 非空時加掃 assets/doctrine 卡池：
    //   signatureDoctrineId / cards[].doctrineId 不在池內 → error
    //   （跨檔引用壞了掠奪解鎖會靜默失敗——G-2 的 signatureDoctrineId）
    static CardReport ValidateString(const std::string& json,
                                     const std::string& pathLabel,
                                     const std::string& assetsRoot = "",
                                     const std::string& doctrineDir = "");
    static CardReport ValidateFile(const std::string& path,
                                   const std::string& assetsRoot = "",
                                   const std::string& doctrineDir = "");

    // 遞迴掃 *.json；壞檔不中止其他卡。另補跨卡檢查（id 重複）
    static std::vector<CardReport> ValidateDir(
        const std::string& dir, const std::string& assetsRoot = "",
        const std::string& doctrineDir = "");

    // G-3 寫回半邊：warn 級問題自動修正後重驗證——
    //   人格塊缺漏→補 50、軸越界→clamp、非數字→50
    //   threshold/cooldown 負值→0、缺 signatureDoctrine→補駐守卡
    // error 級（壞 JSON/缺 id/name/未知 trigger/action/unit_class/
    // 未知 doctrine id）不修——writable=false，內容判斷留給人。
    // 稀有度/立繪屬內容語義，不自動改寫。
    static FixResult FixCard(const std::string& json);

    // 合法值表（供編輯器 UI 列舉與測試對帳）
    static const char* const* TriggerNames();   // 以 nullptr 結尾
    static const char* const* ActionNames();
    static const char* const* RarityNames();
    static const char* const* UnitClassNames();
};

} // namespace Gameplay
} // namespace Potato
