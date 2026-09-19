#include "GovernanceField.h"

#include "BattleController.h"
#include "Squad.h"

namespace Potato {
namespace Gameplay {

void GovernanceField::Bind(
    const std::vector<MapInteractable>& newInteractables,
    const std::vector<MapConvoy>& convoyDefs) {
    interactables = newInteractables;
    fired.clear();
    burned.clear();
    prevMembers.clear();
    convoys.clear();
    convoys.reserve(convoyDefs.size());
    for (const auto& d : convoyDefs) {
        ConvoyState s;
        s.def = &d;
        s.pos = d.path.front();
        s.hp = d.hp;
        convoys.push_back(s);
    }
}

void GovernanceField::Update(float dt, BattleController& battle) {
    DetectOccupation(battle);
    DetectBurning(battle);
    UpdateConvoys(dt, battle);
}

bool GovernanceField::IsBurned(size_t i) const {
    return burned.count(i) > 0;
}

void GovernanceField::DetectOccupation(BattleController& battle) {
    for (size_t gi = 0; gi < interactables.size(); ++gi) {
        if (fired.count(gi)) {
            continue;
        }
        const auto& inter = interactables[gi];
        GovernanceEvent gev;
        if (inter.type == "village") {
            gev = GovernanceEvent::VillageOccupied;
        } else if (inter.type == "convoy" ||
                   inter.type == "supply_cache") {
            gev = GovernanceEvent::ConvoyProtected;
        } else {
            continue; // 油漬/落石等其他互動物不計治理
        }
        for (const auto& sq : battle.GetSquads()) {
            if (sq->GetTeam() != 0 || sq->IsEliminated() ||
                sq->IsRouting()) {
                continue;
            }
            if ((sq->GetPosition() - inter.pos).Length() <=
                inter.radius) {
                battle.RecordGovernanceEvent(gev);
                fired.insert(gi);
                break;
            }
        }
    }
}

void GovernanceField::DetectBurning(BattleController& battle) {
    for (const auto& sq : battle.GetSquads()) {
        const int cur = sq->GetMembers();
        auto it = prevMembers.find(sq.get());
        if (it == prevMembers.end()) {
            prevMembers[sq.get()] = cur; // 首拍只建立基線不判焚
            continue;
        }
        if (cur < it->second) {
            // 損員位置在村莊圈內 → 戰火波及，村莊焚毀
            for (size_t gi = 0; gi < interactables.size(); ++gi) {
                const auto& inter = interactables[gi];
                if (inter.type != "village" || burned.count(gi)) {
                    continue;
                }
                if ((sq->GetPosition() - inter.pos).Length() <=
                    inter.radius) {
                    burned.insert(gi);
                    battle.RecordGovernanceEvent(
                        GovernanceEvent::VillageBurned);
                }
            }
        }
        it->second = cur;
    }
}

void GovernanceField::UpdateConvoys(float dt,
                                    BattleController& battle) {
    for (auto& cv : convoys) {
        if (cv.status != ConvoyState::Status::Moving) {
            continue;
        }
        const MapConvoy& def = *cv.def;

        // 敵隊入圈劫掠：扣 hp；歸零 → 依所屬結算
        bool raided = false;
        for (const auto& sq : battle.GetSquads()) {
            if (sq->GetTeam() == def.team || sq->IsEliminated() ||
                sq->IsRouting()) {
                continue;
            }
            if ((sq->GetPosition() - cv.pos).Length() <=
                def.raidRadius) {
                raided = true;
                break;
            }
        }
        if (raided) {
            cv.hp -= static_cast<int>(kRaidDps * dt + 0.5f);
            if (cv.hp <= 0) {
                cv.status = ConvoyState::Status::Raided;
                if (def.team == 0) {
                    battle.RecordGovernanceEvent(
                        GovernanceEvent::ConvoyLost);
                } else {
                    battle.RecordGovernanceEvent(
                        GovernanceEvent::ConvoyRaided);
                    // 敵輜被劫 → 敵全軍士氣打擊（A.4）
                    for (const auto& sq : battle.GetSquads()) {
                        if (sq->GetTeam() == def.team &&
                            !sq->IsEliminated()) {
                            sq->AdjustMorale(kRaidMoraleHit);
                        }
                    }
                }
                continue;
            }
        }

        // 沿 waypoint 前進；抵達終點 → 我方記 ConvoyProtected
        const Vector2& dest = def.path[cv.nextWaypoint];
        const Vector2 step = dest - cv.pos;
        const float dist = step.Length();
        const float move = def.speed * dt;
        if (dist <= move) {
            cv.pos = dest;
            if (++cv.nextWaypoint >= def.path.size()) {
                cv.status = ConvoyState::Status::Arrived;
                if (def.team == 0) {
                    battle.RecordGovernanceEvent(
                        GovernanceEvent::ConvoyProtected);
                }
            }
        } else {
            cv.pos = cv.pos + step.Normalize() * move;
        }
    }
}

} // namespace Gameplay
} // namespace Potato
