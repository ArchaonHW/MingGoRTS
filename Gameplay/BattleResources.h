#pragma once

#include "Core/CoreTypes.h"

#include <unordered_map>

namespace Potato {
namespace Gameplay {

class BattleController;

/**
 * 戰鬥資源（BattleResources）——T-6 情報/CP/士氣執行率
 *
 * 回合層的核心資源三件套：
 *   intel  —— 情報點。戰前花費解鎖敵方資訊（敵將人格、卡組傾向）；
 *            與指令槽互斥取捨（設計：情報↔指令槽 trade-off）
 *   cp     —— 指揮點數。即時層介入用，由 BattleController 持有；
 *            此處管理額度並同步進 controller
 *   morale —— 每小隊士氣在 Squad 裡；本類提供「士氣 < 30% →
 *            doctrine 執行率 70%」的全域規則設定（接到 controller）
 */
class BattleResources {
public:
    struct TeamResources {
        int intel = 0;
        int cp = 0;
        int maxCP = 0;
    };

    // 設定某隊資源並把 CP 同步進 controller
    void Setup(BattleController& battle, int team, int intel, int cp);

    // 花情報：回傳 false = 情報不足
    bool SpendIntel(int team, int amount);
    int GetIntel(int team) const;
    int GetCP(int team) const;

    // 情報消費：解鎖敵方人格三軸的可視化（回傳是否成功解鎖）
    bool RevealEnemyPersonality(int team);

    // 把士氣執行率規則灌進 controller：
    // 小隊士氣 < threshold 時 doctrine 動作只有 rate 機率執行
    // （預設 < 0.30 → 70%，對應設計「士氣 < 30 → 執行率 70%」）
    static void ApplyMoraleRule(BattleController& battle,
                                float threshold = 0.30f,
                                float rate = 0.70f);

private:
    std::unordered_map<int, TeamResources> teams;
    std::unordered_map<int, bool> personalityRevealed;
};

} // namespace Gameplay
} // namespace Potato
