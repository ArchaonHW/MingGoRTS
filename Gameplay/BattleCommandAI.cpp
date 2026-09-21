#include "Gameplay/BattleCommandAI.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

using Potato::AI::State;

const char* CommandActionName(int id) {
    static const char* names[] = {"wait", "advance", "engage", "hold",
                                  "retreat"};
    if (id < 0 || id >= kCommandActionCount) return "unknown";
    return names[id];
}

static int TotalMembers(const std::vector<Squad*>& v) {
    int n = 0;
    for (const Squad* s : v) {
        if (!s->IsEliminated()) n += s->GetMembers();
    }
    return n;
}

static const Squad* NearestEnemy(const std::vector<Squad*>& enemy,
                                 const Squad* self) {
    const Squad* best = nullptr;
    float bd = 1e30f;
    for (const Squad* e : enemy) {
        if (e->IsEliminated() || e->IsRouting()) continue;
        float d = (e->GetPosition() - self->GetPosition()).Length();
        if (d < bd) { bd = d; best = e; }
    }
    return best;
}

static float MeanDistToObjective(const std::vector<Squad*>& own,
                                 const Vector2& obj) {
    float sum = 0.0f;
    int n = 0;
    for (const Squad* s : own) {
        if (s->IsEliminated()) continue;
        sum += (s->GetPosition() - obj).Length();
        ++n;
    }
    return n > 0 ? sum / n : 0.0f;
}

State FeaturizeCommandState(const BattleController& battle,
                            const std::vector<Squad*>& own,
                            const std::vector<Squad*>& enemy,
                            const CommandSnapshot& snap, float maxTime) {
    State s;
    s.features.assign(kCommandStateSize, 0.0f);
    s.timestamp = (uint64_t)(snap.elapsed * 1000.0f);

    const int ownAlive = TotalMembers(own);
    const int enemyAlive = TotalMembers(enemy);
    float moraleSum = 0, eMoraleSum = 0, staminaSum = 0;
    float engaged = 0, routing = 0, eEngaged = 0;
    float nearestSum = 0;
    int aliveSquads = 0, eAliveSquads = 0;
    for (const Squad* sq : own) {
        if (sq->IsEliminated()) continue;
        ++aliveSquads;
        moraleSum += sq->GetMorale();
        staminaSum += sq->GetStamina();
        if (sq->IsEngaged()) engaged += 1.0f;
        if (sq->IsRouting()) routing += 1.0f;
        const Squad* ne = NearestEnemy(enemy, sq);
        nearestSum += ne ? (ne->GetPosition() - sq->GetPosition()).Length()
                         : 20.0f;
    }
    for (const Squad* sq : enemy) {
        if (sq->IsEliminated()) continue;
        ++eAliveSquads;
        eMoraleSum += sq->GetMorale();
        if (sq->IsEngaged()) eEngaged += 1.0f;
    }
    const float as = (float)std::max(1, aliveSquads);
    const float es = (float)std::max(1, eAliveSquads);

    s.features[0] = snap.ownStart > 0 ? (float)ownAlive / snap.ownStart : 0.0f;
    s.features[1] =
        snap.enemyStart > 0 ? (float)enemyAlive / snap.enemyStart : 0.0f;
    s.features[2] = moraleSum / as;
    s.features[3] = eMoraleSum / es;
    s.features[4] = engaged / as;
    s.features[5] = routing / as;
    s.features[6] =
        MeanDistToObjective(own, battle.GetObjective(0)) / 20.0f;
    s.features[7] = (nearestSum / as) / 20.0f;
    s.features[8] = (float)battle.GetCommandPoints(0) / 60.0f;
    s.features[9] = snap.elapsed / maxTime;
    s.features[10] = enemyAlive > ownAlive ? 1.0f : 0.0f;
    s.features[11] =
        std::clamp((float)((snap.enemyStart - enemyAlive) -
                           (snap.ownStart - ownAlive)) / 60.0f,
                   -1.0f, 1.0f);
    s.features[12] = eEngaged / es;
    s.features[13] = staminaSum / as;
    s.features[14] = (float)snap.lastAction / 4.0f;
    return s;
}

void ApplyCommandAction(BattleController& battle, int actionId,
                        const std::vector<Squad*>& own,
                        const std::vector<Squad*>& enemy,
                        float holdSeconds) {
    const Vector2 obj = battle.GetObjective(0);
    const Vector2 rally = battle.GetRallyPoint(0);
    for (Squad* s : own) {
        if (s->IsEliminated() || s->IsRouting()) continue;
        switch (actionId) {
        case 1:
            battle.Intervene(s, SquadOrder::AttackMove, obj, holdSeconds);
            break;
        case 2: {
            const Squad* t = NearestEnemy(enemy, s);
            if (t) battle.Intervene(s, SquadOrder::Engage, t, holdSeconds);
            break;
        }
        case 3:
            battle.Intervene(s, SquadOrder::Hold, s->GetPosition(),
                             holdSeconds);
            break;
        case 4:
            battle.Intervene(s, SquadOrder::Retreat, rally, holdSeconds);
            break;
        default:
            break; // wait / 未知 id
        }
    }
}

// ---- BattleCommanderAI ----

BattleCommanderAI::BattleCommanderAI()
    : agent(kCommandStateSize, kCommandActionCount) {}

bool BattleCommanderAI::LoadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in) {
        loaded = false;
        return false;
    }
    std::stringstream buf;
    buf << in.rdbuf();
    loaded = agent.Deserialize(buf.str());
    return loaded;
}

void BattleCommanderAI::Reset(const BattleController& battle, int team_,
                              float maxTimeSec) {
    team = team_;
    maxTime = maxTimeSec > 0.0f ? maxTimeSec : 60.0f;
    own.clear();
    enemy.clear();
    for (const auto& sq : battle.GetSquads()) {
        if (sq->GetTeam() == team)
            own.push_back(sq.get());
        else
            enemy.push_back(sq.get());
    }
    snap.ownStart = TotalMembers(own);
    snap.enemyStart = TotalMembers(enemy);
    snap.elapsed = battle.GetElapsed();
    snap.lastAction = 0;
    decisionAcc = 0.0f;
}

void BattleCommanderAI::Update(BattleController& battle, float dt) {
    if (!loaded) return;
    if (battle.GetPhase() != BattlePhase::Execution) return;
    if (own.empty() && enemy.empty()) Reset(battle, team, maxTime);

    // 歷時以戰鬥時鐘為準（暫停/子彈時間下與 env 語義一致）
    snap.elapsed = battle.GetElapsed();
    decisionAcc += dt;
    if (decisionAcc < kCommandTickDt * kCommandTicksPerDecision) return;
    decisionAcc =
        std::fmod(decisionAcc, kCommandTickDt * kCommandTicksPerDecision);

    State s = FeaturizeCommandState(battle, own, enemy, snap, maxTime);
    int id = agent.SelectAction(s, /*explore=*/false).id;
    ApplyCommandAction(battle, id, own, enemy, kCommandHoldSeconds);
    snap.lastAction = id;
}

} // namespace Gameplay
} // namespace Potato
