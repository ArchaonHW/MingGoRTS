#include "BattlePlanner.h"
#include "BattleController.h"
#include "Squad.h"

#include <algorithm>
#include <sstream>

namespace Potato {
namespace Gameplay {

const char* BattlePlanner::PostureName(Posture p) {
    switch (p) {
    case Posture::Aggressive: return "Aggressive";
    case Posture::Balanced:   return "Balanced";
    case Posture::Defensive:  return "Defensive";
    }
    return "?";
}

const char* BattlePlanner::RoleName(SquadRole r) {
    switch (r) {
    case SquadRole::Assault: return "Assault";
    case SquadRole::Support: return "Support";
    case SquadRole::Reserve: return "Reserve";
    }
    return "?";
}

BattlePlanner::SquadRole BattlePlanner::ClassifyRole(size_t rank,
                                                   size_t count) const {
    // 排名 0 = 最強。單隊 → Assault；兩隊 → Assault + Support；
    // 三隊以上 → 前 1/3 Assault、中 1/3 Support、尾端 Reserve
    if (count <= 1) {
        return SquadRole::Assault;
    }
    if (count == 2) {
        return rank == 0 ? SquadRole::Assault : SquadRole::Support;
    }
    float frac = static_cast<float>(rank) / count;
    if (frac < 1.0f / 3.0f) {
        return SquadRole::Assault;
    }
    return frac < 2.0f / 3.0f ? SquadRole::Support : SquadRole::Reserve;
}

DoctrineSet BattlePlanner::BuildDoctrine(SquadRole role,
                                         Posture posture) const {
    DoctrineSet d;
    // 撤退線：劣勢方更早撤（保存兵力），優勢方敢打到底
    float retreatHp = posture == Posture::Defensive  ? 0.40f
                    : posture == Posture::Aggressive ? 0.20f
                                                     : 0.30f;

    switch (role) {
    case SquadRole::Assault:
        d.AddRule({DoctrineTrigger::HealthBelow,
                   DoctrineAction::RetreatToRally, retreatHp, 0});
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackNearest, 3.0f, 10});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::AdvanceToObjective, 0.0f, 100});
        break;

    case SquadRole::Support:
        d.AddRule({DoctrineTrigger::HealthBelow,
                   DoctrineAction::RetreatToRally, retreatHp, 0});
        d.AddRule({DoctrineTrigger::AllyEngaged,
                   DoctrineAction::DefendNearestAlly, 0.0f, 5});
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackWeakest, 4.0f, 10});
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::AdvanceToObjective, 0.0f, 100});
        break;

    case SquadRole::Reserve:
        d.AddRule({DoctrineTrigger::MoraleBelow,
                   DoctrineAction::RetreatToRally, 0.4f, 0});
        d.AddRule({DoctrineTrigger::UnderAttack,
                   DoctrineAction::AttackNearest, 0.0f, 10});
        if (posture == Posture::Aggressive) {
            // 優勢方預備隊也跟進壓上
            d.AddRule({DoctrineTrigger::AllyEngaged,
                       DoctrineAction::DefendNearestAlly, 0.0f, 50});
        }
        d.AddRule({DoctrineTrigger::Always,
                   DoctrineAction::HoldPosition, 0.0f, 100});
        break;
    }
    return d;
}

BattlePlanner::Plan BattlePlanner::GeneratePlan(
    const BattleController& battle, int team) const {

    Plan plan;
    plan.team = team;

    // ---- 兵力比 → 戰略姿態 ----
    int mine = battle.TotalMembers(team);
    int enemy = battle.TotalMembers(team == 0 ? 1 : 0);
    float ratio = enemy > 0 ? static_cast<float>(mine) / enemy : 10.0f;

    if (ratio >= 1.3f) {
        plan.posture = Posture::Aggressive;
    } else if (ratio <= 0.7f) {
        plan.posture = Posture::Defensive;
    }

    // ---- 目標可達性分析（用戰場 flow field 檢查）----
    bool reachable = true;
    float pathCost = 0.0f;
    if (battle.HasObjective(team)) {
        FlowField probe = battle.GetField();
        if (probe.Compute(battle.GetObjective(team))) {
            // 找該隊最強小隊的位置驗證路徑
            for (const auto& s : battle.GetSquads()) {
                if (s->GetTeam() == team) {
                    float c = probe.GetIntegrationCost(s->GetPosition());
                    if (c < 0.0f) {
                        reachable = false;
                    } else {
                        pathCost = std::max(pathCost, c);
                    }
                }
            }
        } else {
            reachable = false;
        }
    }

    // ---- 小隊強度排序 → 角色分配 ----
    std::vector<const Squad*> mySquads;
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() == team && !s->IsEliminated()) {
            mySquads.push_back(s.get());
        }
    }
    std::sort(mySquads.begin(), mySquads.end(),
              [](const Squad* a, const Squad* b) {
                  return a->GetMembers() > b->GetMembers();
              });

    int reserves = 0;
    for (size_t i = 0; i < mySquads.size(); ++i) {
        const Squad* s = mySquads[i];
        SquadPlan sp;
        sp.squad = s;
        sp.role = ClassifyRole(i, mySquads.size());
        sp.doctrine = BuildDoctrine(sp.role, plan.posture);
        if (sp.role == SquadRole::Reserve) {
            reserves++;
        }

        std::ostringstream rs;
        rs << "rank " << (i + 1) << "/" << mySquads.size()
           << " (" << s->GetMembers() << " men) -> " << RoleName(sp.role);
        if (!reachable) {
            rs << "; WARNING: objective unreachable";
        }
        sp.rationale = rs.str();
        plan.squadPlans.push_back(sp);
    }

    // CP 建議：預備隊 + 劣勢方需要更多微操
    plan.suggestedCP = reserves + (plan.posture == Posture::Defensive ? 2 : 1);

    std::ostringstream ss;
    ss << "team " << team << ": " << mine << " vs " << enemy
       << " (ratio " << ratio << ") -> " << PostureName(plan.posture)
       << "; " << mySquads.size() << " squads, " << reserves
       << " reserve, CP " << plan.suggestedCP
       << (reachable ? "" : "; objective UNREACHABLE");
    plan.summary = ss.str();
    return plan;
}

void BattlePlanner::ApplyPlan(BattleController& battle,
                              const Plan& plan) const {
    for (const auto& sp : plan.squadPlans) {
        // const_cast 安全：plan 裡的指標本來就指向 battle 持有的小隊
        battle.AssignDoctrine(const_cast<Squad*>(sp.squad), sp.doctrine);
    }
    battle.SetCommandPoints(plan.team, plan.suggestedCP);
}

} // namespace Gameplay
} // namespace Potato
