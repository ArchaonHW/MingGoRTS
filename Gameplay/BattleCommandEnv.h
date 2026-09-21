#pragma once

// BattleCommandEnv — 把 BattleController 包成 RLEnvironment（訓練側）。
//
// 場景（同斷橋地形）：team 0 攻方四隊，doctrine 只有 Always→HoldPosition
// ——沒有指揮官介入就原地站崗到超時。RL agent 扮演攻方指揮官：每 1 秒
// 決策一次，從 5 個全軍姿態動作選一個（動作/特徵契約見
// BattleCommandAI.h，與 BattleCommanderAI 共用）。
// 學習目標：在 60 CP 預算內學會「推進→接戰→殲敵」。

#include "Gameplay/BattleCommandAI.h"
#include "AI/ReinforcementLearning.h"

#include <memory>
#include <vector>

namespace Potato {
namespace Gameplay {

class BattleCommandEnv : public Potato::AI::RLEnvironment {
public:
    explicit BattleCommandEnv(unsigned seed) { Reset(seed); }

    // 指定 seed 重建戰場（評估用同一組 seed 比較才公平）
    Potato::AI::State Reset(unsigned seed);
    Potato::AI::State Reset() override { return Reset(epSeed); }

    Potato::AI::StepResult Step(const Potato::AI::Action& action) override;
    std::vector<Potato::AI::Action> GetActionSpace() const override;
    int GetStateSize() const override { return kCommandStateSize; }

    // ---- 評估/檢查用 ----
    const BattleController& Battle() const { return *battle; }
    BattleOutcome GetOutcome() const { return battle->GetOutcome(); }
    int OwnLost() const { return snap.ownStart - TotalMembers(ownSquads); }
    int EnemyLost() const {
        return snap.enemyStart - TotalMembers(enemySquads);
    }
    int GetLastAction() const { return snap.lastAction; }

private:
    static constexpr float kDt = kCommandTickDt;
    static constexpr int kTicksPerDecision = kCommandTicksPerDecision;
    static constexpr float kHoldSeconds = kCommandHoldSeconds;
    const float maxTime = 60.0f;

    static int TotalMembers(const std::vector<Squad*>& v);
    float OwnEngagedFraction() const;
    float MeanDistToObjective() const;

    std::unique_ptr<BattleController> battle;
    std::vector<Squad*> ownSquads;
    std::vector<Squad*> enemySquads;
    unsigned epSeed = 0;
    CommandSnapshot snap;
    int prevOwnLost = 0, prevEnemyLost = 0;
    float prevProgress = 0.0f;
};

} // namespace Gameplay
} // namespace Potato
