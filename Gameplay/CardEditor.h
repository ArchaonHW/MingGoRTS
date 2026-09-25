#pragma once

// CardEditor —— G-3 敵將編輯器的「可編輯存回」半邊
//
// CardValidator 只能診斷；本類把卡檔載成可變 JsonValue DOM，
// 提供型別化編輯操作，Save() 前跑完整驗證——有 error 即拒存，
// 合法才 tmp+rename 原子寫回（與 CampaignState 存檔同慣例）。
//
// 可編輯面（potato.character_card/1 既有欄位，不擴 schema）：
//   personality.{aggression,discipline,cunning}   夾 0..100
//   signatureDoctrine.{name,trigger,action,threshold}
//   signatureDoctrineId  → assets/doctrine 卡池 id（綁池時驗存在）
//   文字欄 SetText：name/epithet/courtesyName/faction/role/
//                    lore/historicalBattle/art
//   cards[] 槽位：AddCardRule/RemoveCardRule
//
// trigger/action 先過 CardValidator 白名單——未知值 op 直接失敗
// （fail fast，不靠 Save 才炸）。寫出走 WriteJsonValue 排序鍵，
// 產出 deterministic 可 diff。

#include "Gameplay/CardValidator.h"
#include "Serialization/JsonParser.h"

#include <string>

namespace Potato {
namespace Gameplay {

class CardEditor {
public:
    bool Load(const std::string& path);   // 檔案→DOM（記住路徑供 Save()）
    bool Parse(const std::string& json);  // 字串→DOM（Save() 需另給路徑）

    // doctrine 卡池目錄——綁定後 signatureDoctrineId 交叉驗證啟用
    void BindDoctrineDir(const std::string& dir) { doctrineDir_ = dir; }
    void SetAssetsRoot(const std::string& root) { assetsRoot_ = root; }

    // ---- 型別化編輯（失敗回 false，原因在 LastError()，DOM 不動）----
    bool SetPersonality(const std::string& axis, float v);
    bool SetSignature(const std::string& name, const std::string& trigger,
                      const std::string& action, float threshold);
    bool SetSignatureDoctrineId(const std::string& id);
    bool SetText(const std::string& field, const std::string& value);
    bool AddCardRule(const std::string& name, const std::string& trigger,
                     const std::string& action, float threshold,
                     int priority, float cooldown);
    bool RemoveCardRule(int index);

    // 驗證守門：跑 CardValidator 全套（含 doctrine 交叉檢查）
    CardReport Validate() const;

    // 原子寫回（tmp+rename）；Validate 有 error → 拒存回 false
    bool Save();
    bool Save(const std::string& path);

    const std::string& LastError() const { return lastError_; }
    const JsonValue& Root() const { return root_; }
    bool Loaded() const { return loaded_; }

    // 可編輯文字欄白名單（UI 列舉用）
    static const char* const* TextFields();
    static bool IsTextField(const std::string& field);

private:
    bool Fail(std::string msg);

    JsonValue root_;
    bool loaded_ = false;
    std::string path_;
    std::string assetsRoot_;
    std::string doctrineDir_;
    std::string lastError_;
};

} // namespace Gameplay
} // namespace Potato
