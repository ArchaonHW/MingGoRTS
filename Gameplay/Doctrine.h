#pragma once

#include "Core/CoreTypes.h"
#include "MathUtils/Vector2.h"

#include <vector>
#include <string>

namespace Potato {
namespace Gameplay {

class Squad;

/**
 * Doctrine 觸發條件
 *
 * 對應設計文件「成長綁動詞」：低階 doctrine 只有單條件，
 * 高階解鎖邏輯鏈與跨隊聯動（用 priority 排序模擬）。
 */
enum class DoctrineTrigger {
    Always,             // 永遠成立（墊底規則）
    HealthBelow,        // 自身兵力比例 < threshold
    MoraleBelow,        // 自身士氣 < threshold
    EnemyInRange,       // 最近敵隊距離 < threshold
    UnderAttack,        // 正在受攻擊
    Outnumbered,        // 交戰中且敵方成員總數 > 我方
    AllyEngaged,        // 任一同隊友軍已接戰
    ObjectiveReached    // 已到達目標點
};

/**
 * Doctrine 動作
 */
enum class DoctrineAction {
    AttackNearest,      // 攻擊最近敵隊
    AttackWeakest,      // 攻擊範圍內兵力最少的敵隊
    AdvanceToObjective, // 沿 flow field 往目標推進
    HoldPosition,       // 原地駐守
    RetreatToRally,     // 撤往集結點
    DefendNearestAlly,  // 移向最近的接戰友軍
    Scout               // 偵查：移向最近的未揭露敵情雲（接觸後免費揭露）
};

/**
 * 單條 doctrine 規則：priority 小的先判定，命中即執行
 */
struct DoctrineRule {
    DoctrineTrigger trigger;
    DoctrineAction action;
    float threshold;    // 觸發參數（血量比/士氣/距離，依 trigger 而定）
    int priority;
    float cooldown;              // 命中後冷卻秒數（遊戲時間）；0 = 無冷卻
    mutable float coolingUntil;  // 冷卻截止時間，由 Evaluate 維護

    DoctrineRule(DoctrineTrigger t = DoctrineTrigger::Always,
                 DoctrineAction a = DoctrineAction::HoldPosition,
                 float th = 0.0f, int prio = 100, float cd = 0.0f)
        : trigger(t), action(a), threshold(th), priority(prio),
          cooldown(cd), coolingUntil(0.0f) {
    }
};

/**
 * 每 tick 評估時的戰場上下文（由 BattleController 填寫）
 */
struct SquadContext {
    const Squad* self;
    float now;              // 目前遊戲時間（秒），冷卻判定用
    float healthPct;
    float moralePct;
    float nearestEnemyDist;
    bool underAttack;
    bool outnumbered;
    bool anyAllyEngaged;
    bool objectiveReached;

    SquadContext()
        : self(nullptr)
        , now(0.0f)
        , healthPct(1.0f)
        , moralePct(1.0f)
        , nearestEnemyDist(-1.0f)
        , underAttack(false)
        , outnumbered(false)
        , anyAllyEngaged(false)
        , objectiveReached(false) {
    }
};

/**
 * Doctrine 集合——一隊的戰鬥腳本
 *
 * FF12 Gambit 語義：規則依 priority 排序，由上而下取第一條
 * 觸發成立的規則。規則數量即「指令槽」消耗。
 */
class DoctrineSet {
public:
    void AddRule(const DoctrineRule& rule);
    void Clear() { rules.clear(); }
    size_t Count() const { return rules.size(); }

    // 唯讀存取（PlanningDeck/卡編輯器要列出規則）；依 priority 升序
    const std::vector<DoctrineRule>& Rules() const { return rules; }
    // 移除第 index 條（index 依 Rules() 序）；越界回 false
    bool RemoveRule(size_t index);

    // 評估並回傳命中的 action；沒有任何規則成立時回傳 HoldPosition
    DoctrineAction Evaluate(const SquadContext& ctx) const;
    const DoctrineRule* GetMatchedRule() const { return lastMatched; }

    static bool TriggerMatches(const DoctrineRule& rule, const SquadContext& ctx);

private:
    std::vector<DoctrineRule> rules;
    mutable const DoctrineRule* lastMatched = nullptr;
};

// 顯示用名稱
const char* TriggerName(DoctrineTrigger trigger);
const char* ActionName(DoctrineAction action);

} // namespace Gameplay
} // namespace Potato
