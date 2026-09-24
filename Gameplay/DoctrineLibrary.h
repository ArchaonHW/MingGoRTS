#pragma once

// DoctrineLibrary —— G-1 doctrine 卡池：assets/doctrine/*.json
// 以 potato.doctrine/1 單卡 schema 資料驅動，「寫 doctrine」成為
// 純內容工作，不需重新編譯。
//
// 檔案格式（每檔一卡）：
//   {"schema":"potato.doctrine/1","id":"vanguard_push",
//    "name":"前鋒令","flavor":"……","axis":"combat",
//    "cost":1,
//    "rules":[{"trigger":"EnemyInRange","action":"AttackNearest",
//              "threshold":8,"priority":10,"cooldown":0}],
//    "modifiers":[{"key":"nobattle.negotiation","value":0.1}]}
//
// 語義：
// - axis ∈ combat|nobattle|myth——無戰軸/神話軸卡以 modifiers
//   掛 E/D epic 掛鉤點（HookKeys() 註冊表），戰鬥軸以 rules 為主
// - 卡至少要有一條 rule 或一個 modifier，空卡拒收
// - 壞檔/壞卡跳過並記 Warnings()，不中止其他載入；同 id 後載覆蓋
// - modifiers 的未知 hook key 記警告但保留（向後相容新掛鉤）

#include "Gameplay/Doctrine.h"

#include <map>
#include <set>
#include <string>
#include <vector>

namespace Potato {
namespace Gameplay {

struct DoctrineModifier {
    std::string key;
    float value = 0.0f;
};

struct DoctrineCard {
    std::string id;
    std::string name;
    std::string flavor;
    std::string axis;                      // combat|nobattle|myth
    int cost = 0;                          // 指令槽成本
    std::vector<DoctrineRule> rules;
    std::vector<DoctrineModifier> modifiers;

    // 戰鬥規則注入既有 DoctrineSet（走 AddRule 保 priority 序）
    void AppendRules(DoctrineSet& set) const;
};

class DoctrineLibrary {
public:
    int LoadDir(const std::string& dir);   // 掃 *.json，回傳入池卡數
    int LoadFile(const std::string& path);
    void Clear();

    const DoctrineCard* Find(const std::string& id) const;
    const std::vector<DoctrineCard>& Cards() const { return cards; }
    std::vector<const DoctrineCard*> Axis(const std::string& axis) const;

    // 掛鉤點消費 API：全池或指定 id 子集的 modifier 加總
    float TotalModifier(const std::string& key) const;
    float TotalModifier(const std::set<std::string>& ids,
                        const std::string& key) const;

    const std::vector<std::string>& Warnings() const { return warnings; }
    size_t Size() const { return cards.size(); }

    // 合法值表（編輯器/驗證器列舉用）
    static const char* const* AxisNames();   // 終止於 nullptr
    static const char* const* HookKeys();    // E/D epic 掛鉤點註冊表
    static bool IsAxis(const std::string& axis);
    static bool IsHookKey(const std::string& key);

private:
    std::vector<DoctrineCard> cards;
    std::map<std::string, size_t> byId;
    std::vector<std::string> warnings;
};

} // namespace Gameplay
} // namespace Potato
