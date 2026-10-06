#pragma once

#include "Core/CoreTypes.h"
#include "Doctrine.h"

#include <string>
#include <unordered_map>

namespace Potato {
namespace Gameplay {

class BattleController;

/**
 * 對手軍師反制牌組（N-3，narrative-design「ledger leaks」）
 *
 * 對手軍師會讀你的戰報：每場戰鬥玩家最慣用的 trigger 類型被記下，
 * 下一場他帶來針對性反制牌組——「彼之陣法，似針對我軍慣用」。
 *
 * 資料流：
 *   BattleController::TriggerUsage（本場命中統計）
 *     → RecordBattle 累積跨場
 *     → MostUsedTrigger → BuildCounterDeck 產 DoctrineSet
 *   ToJson/FromJson 持久化（掛 RefitCamp/meta 層存檔）
 *
 * 反制表（trigger 習慣 → 敵方牌組風格）：
 *   Always/推進型   → 據守誘敵（EnemyInRange→AttackWeakest + 墊底駐守）
 *   HealthBelow     → 追擊壓迫（EnemyInRange→AttackNearest + 打最弱）
 *   EnemyInRange    → 拒戰拉扯（敵近即退 + 友軍互援）
 *   MoraleBelow     → 士氣壓制（全線強攻逼崩）
 *   UnderAttack     → 佯攻調動（接戰才動 + 馳援）
 *   Outnumbered     → 正面決戰（全線強攻）
 *   AllyEngaged     → 分割各個擊破（攻最近 + 馳援反制）
 *   ObjectiveReached→ 目標區埋伏（到位固守 + 反打）
 */
class RivalDeck {
public:
    // 累積一場戰鬥的 trigger 命中分佈
    void RecordBattle(const BattleController& battle, int playerTeam);
    void RecordUsage(DoctrineTrigger trigger, int count);

    // 玩家最慣用的 trigger；無資料回 false
    bool MostUsedTrigger(DoctrineTrigger& out) const;
    int UsageCount(DoctrineTrigger trigger) const;
    int TotalUsage() const;

    // 依慣用 trigger 產出反制 DoctrineSet；無資料回空牌組
    DoctrineSet BuildCounterDeck() const;

    // 戰報判詞行（無資料回「無足判斷」）
    std::string WarningLine() const;

    // potato.rival_deck/1 序列化（跨場持續）
    std::string ToJson() const;
    bool FromJson(const std::string& json);

private:
    std::unordered_map<DoctrineTrigger, int> usage;
};

} // namespace Gameplay
} // namespace Potato
