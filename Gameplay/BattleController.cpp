#include "BattleController.h"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace Potato {
namespace Gameplay {

BattleController::BattleController(int gridWidth, int gridHeight, float cellSize)
    : field(gridWidth, gridHeight, cellSize)
    , phase(BattlePhase::Deployment)
    , outcome(BattleOutcome::Ongoing)
    , timeScale(1.0f)
    , doctrineTimer(0.0f)
    , elapsed(0.0f) {
}

Squad* BattleController::CreateSquad(const std::string& name, int team,
                                     const Vector2& pos, int members) {
    auto squad = std::make_unique<Squad>(name, team, pos, members);
    Squad* ptr = squad.get();
    squads.push_back(std::move(squad));
    Emit(name + " deployed (team " + std::to_string(team) + ")");
    return ptr;
}

void BattleController::AssignDoctrine(Squad* squad, const DoctrineSet& doctrine) {
    if (phase != BattlePhase::Deployment || !squad) {
        return;
    }
    doctrines[squad] = doctrine;
    Emit(squad->GetName() + " doctrine assigned (" +
         std::to_string(doctrine.Count()) + " rules)");
}

void BattleController::SetObjective(int team, const Vector2& pos) {
    objectives[team] = pos;
}

void BattleController::SetRallyPoint(int team, const Vector2& pos) {
    rallyPoints[team] = pos;
}

int BattleController::TotalMembers(int team) const {
    int total = 0;
    for (const auto& squad : squads) {
        if (squad->GetTeam() == team) {
            total += squad->GetMembers();
        }
    }
    return total;
}

void BattleController::SetCommandPoints(int team, int points) {
    commandPoints[team] = points;
}

int BattleController::GetCommandPoints(int team) const {
    auto it = commandPoints.find(team);
    return it != commandPoints.end() ? it->second : 0;
}

FlowField* BattleController::GetTeamField(int team) {
    auto it = teamFields.find(team);
    if (it != teamFields.end()) {
        return it->second.get();
    }
    // 惰性建立：以地形欄位為底，計算該隊目標的 flow field
    auto objIt = objectives.find(team);
    if (objIt == objectives.end()) {
        return &field;
    }
    auto teamField = std::make_unique<FlowField>(field);
    teamField->Compute(objIt->second);
    FlowField* ptr = teamField.get();
    teamFields[team] = std::move(teamField);
    return ptr;
}

bool BattleController::BeginExecution() {
    if (phase != BattlePhase::Deployment) {
        return false;
    }
    // 每隊目標各自一張 flow field
    for (const auto& kv : objectives) {
        GetTeamField(kv.first);
    }
    phase = BattlePhase::Execution;
    Emit("=== Execution phase begins ===");
    return true;
}

void BattleController::SetTimeScale(float scale) {
    timeScale = std::max(0.0f, scale);
}

bool BattleController::Intervene(Squad* squad, SquadOrder order,
                                 const Vector2& target, float holdSeconds) {
    if (phase != BattlePhase::Execution || !squad || squad->IsEliminated() ||
        squad->IsRouting()) {
        return false;
    }
    int team = squad->GetTeam();
    if (GetCommandPoints(team) <= 0) {
        Emit("Intervene denied: no command points");
        return false;
    }
    commandPoints[team]--;
    squad->IssueOrder(order, target);
    interventionUntil[squad] = holdSeconds;
    Emit("CP intervention on " + squad->GetName());
    return true;
}

bool BattleController::Intervene(Squad* squad, SquadOrder order,
                                 const Squad* target, float holdSeconds) {
    // 非 Engage 的 squad 目標指令退化為位置指令（engageTarget 只對 Engage 有意義）
    if (order != SquadOrder::Engage && target) {
        return Intervene(squad, order, target->GetPosition(), holdSeconds);
    }
    if (!target || target->IsEliminated() || target->IsRouting() ||
        target == squad || (squad && target->GetTeam() == squad->GetTeam())) {
        return false;
    }
    if (phase != BattlePhase::Execution || !squad || squad->IsEliminated() ||
        squad->IsRouting()) {
        return false;
    }
    int team = squad->GetTeam();
    if (GetCommandPoints(team) <= 0) {
        Emit("Intervene denied: no command points");
        return false;
    }
    commandPoints[team]--;
    squad->IssueOrder(order, target);
    interventionUntil[squad] = holdSeconds;
    Emit("CP intervention on " + squad->GetName());
    return true;
}

void BattleController::SetMoraleExecution(float threshold, float rate) {
    moraleExecThreshold = threshold;
    moraleExecRate = rate;
}

void BattleController::Emit(const std::string& msg) {
    if (onEvent) {
        onEvent(msg);
    }
}

void BattleController::Update(float realDt) {
    if (phase != BattlePhase::Execution || outcome != BattleOutcome::Ongoing) {
        return;
    }
    float dt = realDt * timeScale;
    if (dt <= 0.0f) {
        return;
    }
    elapsed += dt;

    // 介入覆寫倒數
    for (auto& kv : interventionUntil) {
        kv.second -= dt;
    }

    UpdateContexts();

    doctrineTimer += dt;
    if (doctrineTimer >= DOCTRINE_INTERVAL) {
        doctrineTimer = 0.0f;
        EvaluateDoctrines();
    }

    for (auto& squad : squads) {
        // 潰逃小隊可能留著陳舊 orderTarget（如敵方 objective），
        // doctrine 又跳過潰逃隊——每 tick 重指集結點，確保往己方撤退
        // （同時修正 ApplyCasualties 潰逃路徑留下的舊目標）
        if (squad->IsRouting()) {
            auto rIt = rallyPoints.find(squad->GetTeam());
            if (rIt != rallyPoints.end()) {
                squad->IssueOrder(SquadOrder::Retreat, rIt->second);
            }
        }
        squad->Update(dt, GetTeamField(squad->GetTeam()));
    }

    ResolveCombat(dt);
    CheckOutcome();
}

SquadContext BattleController::BuildContext(const Squad& squad) const {
    auto it = contexts.find(const_cast<Squad*>(&squad));
    if (it != contexts.end()) {
        return it->second;
    }
    SquadContext ctx;
    ctx.self = &squad;
    ctx.now = elapsed;
    ctx.healthPct = squad.GetHealthPct();
    ctx.moralePct = squad.GetMorale();
    ctx.underAttack = squad.IsUnderAttack();
    return ctx;
}

Squad* BattleController::FindNearestEnemy(const Squad& squad, float maxDist) const {
    Squad* best = nullptr;
    float bestDist = maxDist;
    for (const auto& other : squads) {
        if (other->GetTeam() == squad.GetTeam() || other->IsEliminated() ||
            other->IsRouting()) {
            continue;
        }
        float d = (other->GetPosition() - squad.GetPosition()).Length();
        if (d < bestDist) {
            bestDist = d;
            best = other.get();
        }
    }
    return best;
}

Squad* BattleController::FindWeakestEnemy(const Squad& squad, float maxDist) const {
    Squad* best = nullptr;
    int lowest = 0x7fffffff;
    for (const auto& other : squads) {
        if (other->GetTeam() == squad.GetTeam() || other->IsEliminated() ||
            other->IsRouting()) {
            continue;
        }
        float d = (other->GetPosition() - squad.GetPosition()).Length();
        if (d <= maxDist && other->GetMembers() < lowest) {
            lowest = other->GetMembers();
            best = other.get();
        }
    }
    return best;
}

Squad* BattleController::FindNearestEngagedAlly(const Squad& squad) const {
    Squad* best = nullptr;
    float bestDist = 1e30f;
    for (const auto& other : squads) {
        if (other->GetTeam() != squad.GetTeam() || other.get() == &squad ||
            other->IsEliminated() || !other->IsEngaged()) {
            continue;
        }
        float d = (other->GetPosition() - squad.GetPosition()).Length();
        if (d < bestDist) {
            bestDist = d;
            best = other.get();
        }
    }
    return best;
}

void BattleController::UpdateContexts() {
    for (auto& squad : squads) {
        SquadContext ctx;
        ctx.self = squad.get();
        ctx.now = elapsed;
        ctx.healthPct = squad->GetHealthPct();
        ctx.moralePct = squad->GetMorale();
        ctx.underAttack = squad->IsUnderAttack();

        Squad* nearest = FindNearestEnemy(*squad, 1e30f);
        ctx.nearestEnemyDist = nearest
            ? (nearest->GetPosition() - squad->GetPosition()).Length()
            : -1.0f;

        // 敵眾我寡：交戰範圍內敵方成員總數 > 我方
        int enemyMembers = 0;
        for (const auto& other : squads) {
            if (other->GetTeam() != squad->GetTeam() && !other->IsEliminated() &&
                (other->GetPosition() - squad->GetPosition()).Length() <=
                    squad->GetEngageRange() * 3.0f) {
                enemyMembers += other->GetMembers();
            }
        }
        ctx.outnumbered = enemyMembers > squad->GetMembers();

        ctx.anyAllyEngaged = false;
        for (const auto& other : squads) {
            if (other->GetTeam() == squad->GetTeam() &&
                other.get() != squad.get() && other->IsEngaged()) {
                ctx.anyAllyEngaged = true;
                break;
            }
        }

        auto objIt = objectives.find(squad->GetTeam());
        ctx.objectiveReached =
            objIt != objectives.end() &&
            (objIt->second - squad->GetPosition()).Length() <= OBJECTIVE_RADIUS;

        contexts[squad.get()] = ctx;
    }
}

void BattleController::EvaluateDoctrines() {
    for (auto& squad : squads) {
        if (squad->IsEliminated() || squad->IsRouting()) {
            continue;
        }
        // 玩家 CP 介入覆寫中，跳過 doctrine
        auto iv = interventionUntil.find(squad.get());
        if (iv != interventionUntil.end() && iv->second > 0.0f) {
            continue;
        }

        auto docIt = doctrines.find(squad.get());
        if (docIt == doctrines.end()) {
            continue;
        }
        const SquadContext& ctx = contexts[squad.get()];
        DoctrineAction action = docIt->second.Evaluate(ctx);
        const DoctrineRule* rule = docIt->second.GetMatchedRule();

        // T-6 士氣執行率：士氣崩到門檻以下，命令有機率被抗命（原地不動）
        if (moraleExecThreshold > 0.0f &&
            ctx.moralePct < moraleExecThreshold &&
            action != DoctrineAction::RetreatToRally) { // 撤退永遠執行
            std::uniform_real_distribution<float> roll(0.0f, 1.0f);
            if (roll(execRng) > moraleExecRate) {
                Emit(squad->GetName() + " ignored orders (low morale)");
                continue;
            }
        }

        SquadOrder prevOrder = squad->GetOrder();
        Vector2 prevTarget = squad->GetOrderTarget();
        const Squad* prevEngage = squad->GetEngageTarget();

        switch (action) {
        case DoctrineAction::AttackNearest: {
            Squad* target = FindNearestEnemy(*squad, 1e30f);
            if (target) {
                squad->IssueOrder(SquadOrder::Engage, target);
            }
            break;
        }
        case DoctrineAction::AttackWeakest: {
            Squad* target = FindWeakestEnemy(*squad, 1e30f);
            if (target) {
                squad->IssueOrder(SquadOrder::Engage, target);
            }
            break;
        }
        case DoctrineAction::AdvanceToObjective: {
            auto objIt = objectives.find(squad->GetTeam());
            if (objIt != objectives.end()) {
                GetTeamField(squad->GetTeam()); // 確保場已算好
                squad->IssueOrder(SquadOrder::AttackMove, objIt->second);
            }
            break;
        }
        case DoctrineAction::HoldPosition:
            squad->IssueOrder(SquadOrder::Hold, squad->GetPosition());
            break;
        case DoctrineAction::RetreatToRally: {
            auto rIt = rallyPoints.find(squad->GetTeam());
            Vector2 dest = rIt != rallyPoints.end() ? rIt->second
                                                  : squad->GetPosition();
            squad->IssueOrder(SquadOrder::Retreat, dest);
            break;
        }
        case DoctrineAction::DefendNearestAlly: {
            Squad* ally = FindNearestEngagedAlly(*squad);
            if (ally) {
                squad->IssueOrder(SquadOrder::MoveTo, ally->GetPosition());
            }
            break;
        }
        }

        bool orderChanged =
            squad->GetOrder() != prevOrder ||
            squad->GetEngageTarget() != prevEngage ||
            (squad->GetOrderTarget() - prevTarget).LengthSquared() > 0.01f;

        if (rule && orderChanged && rule->trigger != DoctrineTrigger::Always) {
            std::ostringstream oss;
            oss << squad->GetName() << " doctrine: " << TriggerName(rule->trigger)
                << " -> " << ActionName(rule->action);
            Emit(oss.str());
        }
    }
}

void BattleController::ResolveCombat(float dt) {
    // 每 tick 重置戰鬥旗標，接戰判定再立起來
    for (auto& squad : squads) {
        squad->SetEngaged(false);
        squad->SetUnderAttack(false);
    }

    for (size_t i = 0; i < squads.size(); ++i) {
        Squad* a = squads[i].get();
        if (a->IsEliminated() || a->IsRouting()) {
            continue;
        }
        for (size_t j = i + 1; j < squads.size(); ++j) {
            Squad* b = squads[j].get();
            if (b->IsEliminated() || b->IsRouting() ||
                a->GetTeam() == b->GetTeam()) {
                continue;
            }
            float dist = (a->GetPosition() - b->GetPosition()).Length();
            float range = std::max(a->GetEngageRange(), b->GetEngageRange());
            if (dist <= range) {
                a->SetEngaged(true);
                b->SetEngaged(true);
                a->SetUnderAttack(true);
                b->SetUnderAttack(true);
                damageBuffer[a] += b->GetAttackDPS() * dt; // a 受 b 傷害
                damageBuffer[b] += a->GetAttackDPS() * dt; // b 受 a 傷害
            }
        }
    }

    // 結算傷亡與士氣回復
    for (auto& squad : squads) {
        auto it = damageBuffer.find(squad.get());
        if (it != damageBuffer.end()) {
            int casualties = static_cast<int>(it->second);
            if (casualties > 0) {
                int before = squad->GetMembers();
                squad->ApplyCasualties(casualties);
                it->second -= static_cast<float>(casualties);
                Emit(squad->GetName() + " took " +
                     std::to_string(before - squad->GetMembers()) +
                     " casualties");
                if (squad->IsEliminated()) {
                    Emit(squad->GetName() + " ELIMINATED");
                } else if (squad->IsRouting()) {
                    Emit(squad->GetName() + " is routing!");
                }
            }
        }
        if (!squad->IsEliminated()) {
            squad->RecoverMorale(dt);
        }
    }
}

void BattleController::CheckOutcome() {
    bool alive[2] = {false, false};
    for (const auto& squad : squads) {
        if (!squad->IsEliminated() && !squad->IsRouting()) {
            alive[squad->GetTeam() == 0 ? 0 : 1] = true;
        }
    }
    if (!alive[0] && !alive[1]) {
        outcome = BattleOutcome::Draw;
    } else if (!alive[1]) {
        outcome = BattleOutcome::Victory;
    } else if (!alive[0]) {
        outcome = BattleOutcome::Defeat;
    }

    if (outcome != BattleOutcome::Ongoing) {
        phase = BattlePhase::Resolution;
        Emit("=== Resolution: outcome " +
             std::to_string(static_cast<int>(outcome)) + " ===");
    }
}

} // namespace Gameplay
} // namespace Potato
