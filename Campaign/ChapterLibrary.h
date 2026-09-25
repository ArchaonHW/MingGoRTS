#pragma once

// 章節定義庫（C-1）：assets/campaign/*.json 的內容側定義，
// 與執行期 ChapterState 分工——定義檔描述「這章是什麼」，
// ChapterState 記「玩家打到哪」。
//
// JSON schema: potato.campaign_chapter/1
// {
//   "schema": "potato.campaign_chapter/1",
//   "id": "duanqiao",
//   "name": "斷橋",
//   "arc": 0,                 // 0=軍閥 1=北伐 2=抗戰 3=內戰
//   "chapter": 1,             // 弧內章節序
//   "map": "maps/duanqiao.json",   // 相對 assets/ 的地圖路徑
//   "enemy_deck": "",         // 敵將牌組 id（可空，內容後補）
//   "next": ""                // 預設下一章 id（可空=弧內順序或終章）
// }
//
// 錯誤處理（比照 SquadTemplateLibrary 慣例）：
// - schema 不符 / JSON 壞 → LoadFromFile/LoadFromString 回 false，
//   失敗時物件先重置為預設值，不產生半成品
// - 缺 id/name → 仍解析但記 warnings；欄位型別錯（數字/布林當字串）
//   也強轉並記警告
// - arc 越界（<0 或 >3）→ 夾回 [0,3] + 警告；chapter 夾 [1,INT_MAX]+警告
// - LoadDir 壞檔跳過不中止（筆數見 LastSkipped）；同 id keep-latest 除重，
//   空 id 不參與除重
// - 指標生命週期：Find/Sorted 回傳值指向庫內部，
//   Add/LoadDir 可能使舊指標懸空

#include "Serialization/JsonParser.h"

#include <string>
#include <vector>

namespace Potato {
namespace Campaign {

// 無戰選項定義（E-1）：章節層級的單一路徑設定。
// requirement 語義隨路徑而異：
//   談判 negotiation = 民心消費額（Civil 淨額 ≥ 此值開放，成功即支出）
//   嚇阻 deterrence  = 軍威門檻（Army 淨額 ≥ 此值開放，不消耗）
//   顛覆 subversion  = 敵將 verified 判詞條數（情報帳 ≥ 此值開放）
struct NoBattleOptionDef {
    bool enabled = false;   // 定義塊是否開放此路徑
    int requirement = 0;    // 開放門檻（>0 才視為有效定義）
};

// 無戰定義塊：章節定義 "no_battle" 欄位解析結果。
// 三路徑獨立開關——章節可只開談判不開顛覆。
struct NoBattleDef {
    NoBattleOptionDef negotiation;
    NoBattleOptionDef deterrence;
    NoBattleOptionDef subversion;

    bool Offered() const {
        return negotiation.enabled || deterrence.enabled ||
               subversion.enabled;
    }
};

// 神話入侵定義塊（D-4）：章節定義 "incursion" 欄位。
// seepage = 觸發所需滲透等級序數（0-3）；kind 見 MythIncursion。
struct IncursionDef {
    bool enabled = false;
    int seepage = 0;        // 滲透等級門檻（>0 才有效；Quiet 章不入侵）
    std::string kind;       // "ghost_legion" | "fox_rumor"
};

struct ChapterDef {
    static constexpr const char* kSchema = "potato.campaign_chapter/1";

    std::string id;
    std::string name;
    int arc = 0;
    int chapter = 1;
    std::string map;        // 相對 assets/ 的地圖路徑
    std::string enemyDeck;  // 敵將牌組 id（可空）
    std::string next;       // 預設下一章 id（可空）
    NoBattleDef noBattle;   // 無戰路徑定義（可選欄位，全預設=純戰鬥章節）
    IncursionDef incursion; // 神話入侵定義（可選欄位，預設=不入侵）
    // G-5 教學章節："tutor": true → 正典四步；或
    // {"tutor": {"steps": ["pick_card","deploy",...]}} 自訂序列
    // （須為正典序的子序列，見 TutorialFlow::CanonicalSteps）
    bool tutor = false;
    std::vector<std::string> tutorSteps;

    std::vector<std::string> warnings; // 降級記錄（機器可讀）

    bool LoadFromFile(const std::string& path);
    bool LoadFromString(const std::string& json);
};

class ChapterLibrary {
public:
    // 掃描目錄載入全部合法章節定義；回傳成功解析的檔案筆數
    // （keep-latest 覆蓋也計入）。重複呼叫不累積複本——同 id 以新載入者覆蓋。
    size_t LoadDir(const std::string& dir);

    // 上次 LoadDir 跳過的檔案數（打不開/壞 JSON/schema 不符）
    size_t LastSkipped() const { return lastSkipped; }

    // 直接加一筆（測試/程式化定義用）；同 id 不除重，Find 取先載入者
    void Add(const ChapterDef& def) { defs.push_back(def); }
    void Clear() { defs.clear(); }
    size_t Size() const { return defs.size(); }

    // 【生命週期注意】回傳指標指向 vector 內部，Add/LoadDir
    // 重新配置後舊指標懸空，取用後勿跨變動沿用。
    const ChapterDef* Find(const std::string& id) const;

    // 依 (arc, chapter) 排序回傳——章節序瀏覽用
    std::vector<const ChapterDef*> Sorted() const;

private:
    std::vector<ChapterDef> defs;
    size_t lastSkipped = 0;
};

} // namespace Campaign
} // namespace Potato
