#pragma once

#include "Core/CoreTypes.h"
#include "Doctrine.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleController;
class BattlePlanner;
class Squad;

/**
 * 回合層牌組模型（T-9）
 *
 * 對應「寫作戰鬥」循環的第一拍：玩家在即時執行前編排每支小隊的
 * doctrine 卡槽。PlanningDeck 持有可編輯的規則副本（與
 * BattleController 內的 doctrine 脫鉤），「開戰」時 Commit 寫回。
 *
 * 設計重點：
 * - 卡槽上限 SlotCap（設計上 4→7 成長，先留掛點）
 * - 規則依 priority 升序，UI 的上移/下移即交換 priority
 * - LoadPlannerTemplate 一鍵載入 AI 參謀起手牌組，保留 rationale
 *   供 UI 顯示「為何這樣規劃」
 * - Validate 回傳警告而非錯誤——空卡組、無 Always 墊底都是
 *   合法但通常不理想的組態
 */
class PlanningDeck {
public:
    struct SquadDeck {
        Squad* squad = nullptr;
        std::vector<DoctrineRule> rules; // priority 升序
    };

    // 收集 team 的小隊；已指派過的 doctrine 讀回為編輯起點
    void Init(const BattleController& battle, int team);

    // 一鍵載入 AI 參謀模板（GeneratePlan 的 doctrine + rationale）
    void LoadPlannerTemplate(const BattlePlanner& planner,
                             const BattleController& battle, int team);

    int SquadCount() const { return static_cast<int>(decks.size()); }
    SquadDeck& Deck(int index) { return decks[index]; }
    const SquadDeck& Deck(int index) const { return decks[index]; }
    // squad → deck 索引；找不到回 -1
    int FindDeck(const Squad* squad) const;

    // ---- 編輯操作（全部回傳是否成功）----
    bool AddRule(int deckIndex, const DoctrineRule& rule); // 滿槽回 false
    bool RemoveRule(int deckIndex, int ruleIndex);
    bool SetRule(int deckIndex, int ruleIndex,
                 const DoctrineRule& rule);               // 改後重排
    bool SwapRules(int deckIndex, int a, int b);          // 交換 priority

    int SlotCap() const { return slotCap; }
    void SetSlotCap(int cap) { slotCap = cap > 0 ? cap : 1; }

    // 組態警告清單（空卡組 / 無 Always 墊底…），可空 = 無警告
    std::vector<std::string> Validate() const;

    // 把編輯結果寫回 BattleController（AssignDoctrine）
    void Commit(BattleController& battle) const;

    // AI 參謀理由（LoadPlannerTemplate 後有效；無則回空字串）
    const std::string& Rationale(const Squad* squad) const;
    const std::string& Summary() const { return summary; }
    int SuggestedCP() const { return suggestedCP; }

private:
    static void SortDeck(SquadDeck& deck);

    std::vector<SquadDeck> decks;
    std::unordered_map<const Squad*, std::string> rationales;
    std::string summary;
    int suggestedCP = 0;
    int slotCap = 7; // 成長線掛點：低階 4，高階 7
};

} // namespace Gameplay
} // namespace Potato
