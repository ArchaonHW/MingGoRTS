#include "Gameplay/BattleCommandEnv.h"

#include "Gameplay/Doctrine.h"

#include <algorithm>
#include <random>

namespace Potato {
namespace Gameplay {

using Potato::AI::Action;
using Potato::AI::State;
using Potato::AI::StepResult;

int BattleCommandEnv::TotalMembers(const std::vector<Squad*>& v) {
    int n = 0;
    for (const Squad* s : v) {
        if (!s->IsEliminated()) n += s->GetMembers();
    }
    return n;
}

float BattleCommandEnv::OwnEngagedFraction() const {
    float engaged = 0.0f;
    int n = 0;
    for (const Squad* s : ownSquads) {
        if (s->IsEliminated()) continue;
        ++n;
        if (s->IsEngaged()) engaged += 1.0f;
    }
    return n > 0 ? engaged / n : 0.0f;
}

float BattleCommandEnv::MeanDistToObjective() const {
    const Vector2 obj = battle->GetObjective(0);
    float sum = 0.0f;
    int n = 0;
    for (const Squad* s : ownSquads) {
        if (s->IsEliminated()) continue;
        sum += (s->GetPosition() - obj).Length();
        ++n;
    }
    return n > 0 ? sum / n : 0.0f;
}

State BattleCommandEnv::Reset(unsigned seed) {
    epSeed = seed;
    std::mt19937 rng(seed);
    auto jitter = [&](float base, float range) {
        return base +
               std::uniform_real_distribution<float>(-range, range)(rng);
    };

    battle = std::make_unique<BattleController>(20, 15, 1.0f);
    // 中央牆留缺口（y 6~8）——同斷橋地形
    for (int y = 0; y < 15; ++y) {
        if (y < 6 || y > 8) {
            battle->GetField().SetObstacle(10, y, true);
        }
    }
    battle->SetObjective(0, Vector2(18, 7));
    battle->SetRallyPoint(0, Vector2(1, 7));
    battle->SetRallyPoint(1, Vector2(18, 7));
    battle->SetCommandPoints(0, 60); // 指揮官預算
    battle->SetCommandPoints(1, 0);  // 守軍全交 doctrine

    // 攻方四隊：站位/人數隨 seed 抖動;只有墊底 Hold——無指揮不動
    const char* atkNames[] = {"前鋒", "左翼", "右翼", "預備隊"};
    const Vector2 atkPos[] = {{3, 4}, {2, 7}, {3, 10}, {5, 7}};
    ownSquads.clear();
    for (int i = 0; i < 4; ++i) {
        Vector2 p(jitter(atkPos[i].x, 1.0f), jitter(atkPos[i].y, 1.0f));
        p.x = std::clamp(p.x, 0.5f, 9.0f);
        p.y = std::clamp(p.y, 0.5f, 14.5f);
        Squad* s = battle->CreateSquad(atkNames[i], 0, p,
                                       10 + (int)(rng() % 6));
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition,
                   0.0f, 100});
        battle->AssignDoctrine(s, d);
        ownSquads.push_back(s);
    }
    // 守方四隊：VideoDataDemo 式守備 doctrine
    const char* defNames[] = {"橋頭守軍", "北段守軍", "南段守軍", "城內守軍"};
    const Vector2 defPos[] = {{12, 7}, {11, 3}, {11, 11}, {14, 7}};
    enemySquads.clear();
    for (int i = 0; i < 4; ++i) {
        Vector2 p(jitter(defPos[i].x, 0.8f), jitter(defPos[i].y, 1.0f));
        p.x = std::clamp(p.x, 10.5f, 19.5f);
        p.y = std::clamp(p.y, 0.5f, 14.5f);
        // 守軍刻意較弱（8~11 vs 攻方 10~15）——主動求戰明顯有利,
        // DQN 才不會收斂到「龜縮換平手」的懦夫最優解
        Squad* s = battle->CreateSquad(defNames[i], 1, p,
                                       8 + (int)(rng() % 4));
        DoctrineSet d;
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackNearest, 4.0f, 10});
        d.AddRule({DoctrineTrigger::AllyEngaged,
                   DoctrineAction::DefendNearestAlly, 0.0f, 50});
        d.AddRule({DoctrineTrigger::Always, DoctrineAction::HoldPosition,
                   0.0f, 100});
        battle->AssignDoctrine(s, d);
        enemySquads.push_back(s);
    }

    snap.ownStart = TotalMembers(ownSquads);
    snap.enemyStart = TotalMembers(enemySquads);
    prevOwnLost = 0;
    prevEnemyLost = 0;
    prevProgress = MeanDistToObjective();
    snap.lastAction = 0;
    snap.elapsed = 0.0f;
    battle->BeginExecution();
    return FeaturizeCommandState(*battle, ownSquads, enemySquads, snap,
                                 maxTime);
}

StepResult BattleCommandEnv::Step(const Action& action) {
    ApplyCommandAction(*battle, action.id, ownSquads, enemySquads,
                       kHoldSeconds);
    snap.lastAction = action.id;

    StepResult out;
    for (int i = 0; i < kTicksPerDecision; ++i) {
        battle->Update(kDt);
        snap.elapsed += kDt;
        if (battle->GetPhase() == BattlePhase::Resolution) {
            break;
        }
    }

    // ---- reward shaping ----
    const int ownLost = snap.ownStart - TotalMembers(ownSquads);
    const int enemyLost = snap.enemyStart - TotalMembers(enemySquads);
    float r = 0.06f * (float)((enemyLost - prevEnemyLost) -
                              (ownLost - prevOwnLost)); // 交換比
    const float progress = MeanDistToObjective();
    r += 0.08f * (prevProgress - progress);  // 壓向目標
    r += 0.03f * OwnEngagedFraction();       // 接戰即時獎勵
    r -= 0.004f;                             // 每步微懲（催促）
    prevOwnLost = ownLost;
    prevEnemyLost = enemyLost;
    prevProgress = progress;

    out.done = (battle->GetOutcome() != BattleOutcome::Ongoing) ||
               (snap.elapsed >= maxTime);
    if (battle->GetOutcome() == BattleOutcome::Victory) {
        r += 1.5f;
    } else if (battle->GetOutcome() == BattleOutcome::Defeat) {
        r -= 1.5f;
    } else if (out.done) {
        r -= 0.6f; // 超時平手＝沒打到東西,明顯差過積極求戰
    }

    out.nextState = FeaturizeCommandState(*battle, ownSquads, enemySquads,
                                          snap, maxTime);
    out.reward = r;
    return out;
}

std::vector<Action> BattleCommandEnv::GetActionSpace() const {
    std::vector<Action> v;
    for (int i = 0; i < kCommandActionCount; ++i) {
        v.emplace_back(i, CommandActionName(i));
    }
    return v;
}

} // namespace Gameplay
} // namespace Potato
