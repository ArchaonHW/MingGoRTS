#include "Squad.h"
#include "FlowField.h"

#include <algorithm>
#include <cmath>

namespace Potato {
namespace Gameplay {

Squad::Squad(const std::string& squadName, int teamId,
             const Vector2& pos, int memberCount)
    : name(squadName)
    , team(teamId)
    , position(pos)
    , members(memberCount)
    , maxMembers(memberCount)
    , morale(1.0f)
    , routing(false)
    , engaged(false)
    , underAttack(false)
    , speed(2.0f)
    , engageRange(1.5f)
    , damagePerMember(0.05f)
    , stamina(1.0f)
    , staminaDrainMove(0.03f)
    , staminaDrainCombat(0.05f)
    , staminaRegen(0.06f)
    , exhaustedThreshold(0.3f)
    , exhaustedSpeedMul(0.6f)
    , order(SquadOrder::Hold)
    , orderTarget(pos)
    , engageTarget(nullptr) {
}

float Squad::GetEffectiveSpeed() const {
    float s = speed * (stamina < exhaustedThreshold ? exhaustedSpeedMul : 1.0f);
    if (chargeTimer > 0.0f) {
        s *= 1.5f; // G-8 帶隊突擊
    }
    return s * crowdFactor; // P-3 擁擠懲罰（預設 1.0）
}

void Squad::StartCharge(float seconds) {
    if (seconds > 0.0f) {
        chargeTimer = seconds;
    }
}

void Squad::SetStaminaParams(float drainMove, float drainCombat,
                             float regen, float threshold, float penaltyMul) {
    staminaDrainMove = drainMove;
    staminaDrainCombat = drainCombat;
    staminaRegen = regen;
    exhaustedThreshold = threshold;
    exhaustedSpeedMul = penaltyMul;
}

float Squad::GetAttackDPS() const {
    // 戰力隨成員數遞減；潰逃中無法攻擊
    if (routing || members <= 0) {
        return 0.0f;
    }
    float dps = damagePerMember * members;
    if (chargeTimer > 0.0f) {
        dps *= 1.5f; // G-8 帶隊突擊
    }
    return dps;
}

void Squad::IssueOrder(SquadOrder newOrder, const Vector2& target) {
    order = newOrder;
    orderTarget = target;
    engageTarget = nullptr;
}

void Squad::IssueOrder(SquadOrder newOrder, const Squad* target) {
    order = newOrder;
    engageTarget = target;
    if (target) {
        orderTarget = target->GetPosition();
    }
}

void Squad::ApplyCasualties(int count) {
    if (count <= 0 || members <= 0) {
        return;
    }
    int actual = std::min(count, members);
    members -= actual;

    // 傷亡比例打擊士氣；被殲滅必定潰逃
    float lossRatio = static_cast<float>(actual) / maxMembers;
    morale = std::max(0.0f, morale - lossRatio * 1.5f);

    if (members <= 0 || morale <= 0.0f) {
        routing = members > 0; // 全滅不算潰逃，算殲滅
        order = SquadOrder::Retreat;
    }
}

void Squad::AdjustMorale(float delta) {
    if (members <= 0) {
        return;
    }
    morale = std::clamp(morale + delta, 0.0f, 1.0f);
    // 士氣歸零 → 潰逃（與傷亡歸零同型；潰逃為終態，不復歸）
    if (!routing && morale <= 0.0f) {
        routing = true;
        order = SquadOrder::Retreat;
    }
}

void Squad::RecoverMorale(float dt) {
    if (!engaged && !routing) {
        morale = std::min(1.0f, morale + 0.02f * dt);
    }
}

void Squad::Update(float dt, const FlowField* field) {
    if (members <= 0) {
        return;
    }

    if (chargeTimer > 0.0f) {
        chargeTimer = std::max(0.0f, chargeTimer - dt);
    }

    if (routing) {
        // 潰逃：不聽指揮，往己方邊緣移動（orderTarget 由 controller 設為集結點）
        MoveToward(orderTarget, dt * 1.2f, field);
        stamina = std::max(0.0f, stamina - staminaDrainMove * dt);
        return;
    }

    bool moved = false;
    switch (order) {
    case SquadOrder::Hold:
        break;
    case SquadOrder::MoveTo:
    case SquadOrder::AttackMove:
    case SquadOrder::Retreat:
        moved = MoveToward(orderTarget, dt, field);
        break;
    case SquadOrder::Engage:
        if (engageTarget && !engageTarget->IsEliminated()) {
            moved = MoveToward(engageTarget->GetPosition(), dt, field);
        } else {
            order = SquadOrder::Hold;
        }
        break;
    }

    // 疲勞：移動與交戰消耗，駐守且未接戰時回復
    if (moved) {
        stamina -= staminaDrainMove * dt;
    }
    if (engaged) {
        stamina -= staminaDrainCombat * dt;
    }
    if (!moved && !engaged) {
        stamina += staminaRegen * dt;
    }
    stamina = std::clamp(stamina, 0.0f, 1.0f);
}

bool Squad::MoveToward(const Vector2& dest, float dt, const FlowField* field) {
    Vector2 toDest = dest - position;
    float dist = toDest.Length();

    // 已到達目標附近就停下
    if (dist < 0.5f) {
        return false;
    }

    Vector2 dir(0.0f, 0.0f);
    // flow field 只導向它自己的 goal；dest 不是場的目標時直線走
    // （Engage 追敵、Retreat 回集結點等目標與 objective 不同）
    if (field && field->HasGoal() &&
        (field->GetGoal() - dest).Length() <= field->GetCellSize()) {
        dir = field->GetDirection(position);
    }
    if (dir.LengthSquared() < 0.0001f) {
        dir = toDest.Normalize();
    }

    Vector2 step = dir * (GetEffectiveSpeed() * dt);
    if (step.Length() > dist) {
        position = dest;
    } else {
        position = position + step;
    }
    return true;
}

const char* UnitClassName(UnitClass cls) {
    switch (cls) {
    case UnitClass::Infantry: return "infantry";
    case UnitClass::Archer:   return "archer";
    case UnitClass::Cavalry:  return "cavalry";
    }
    return "infantry";
}

UnitClass UnitClassFromString(const std::string& s, bool* ok) {
    if (ok) *ok = true;
    if (s == "cavalry" || s == "cav") return UnitClass::Cavalry;
    if (s == "archer" || s == "arch") return UnitClass::Archer;
    if (s == "infantry" || s == "inf") return UnitClass::Infantry;
    if (ok) *ok = false;
    return UnitClass::Infantry;
}

} // namespace Gameplay
} // namespace Potato
