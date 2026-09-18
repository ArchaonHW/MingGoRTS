#include "BattlePlan.h"

#include "BattleController.h"
#include "BattleResources.h"
#include "Squad.h"
#include "Serialization/JsonParser.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

void BattlePlan::AddArrow(const std::string& squadName, const Vector2& from,
                          const Vector2& to, int priority) {
    PlanArrow a;
    a.squadName = squadName;
    a.from = from;
    a.to = to;
    a.priority = priority;
    arrows.push_back(a);
}

void BattlePlan::SetRallyPoint(const Vector2& p) {
    rallyPoint = p;
    hasRally = true;
}

void BattlePlan::SetPlanBonus(float mul, int intel, int cp) {
    attackMul = mul > 0.0f ? mul : 1.0f;
    bonusIntel = intel > 0 ? intel : 0;
    bonusCP = cp > 0 ? cp : 0;
}

DoctrineSet BattlePlan::ArrowToDoctrine(const PlanArrow& arrow) const {
    DoctrineSet set;
    // 保命優先：兵力剩三成以下撤往集結點（15s 冷卻避免反覆橫跳）
    set.AddRule(DoctrineRule(DoctrineTrigger::HealthBelow,
                             DoctrineAction::RetreatToRally, 0.30f,
                             /*prio=*/10, /*cd=*/15.0f));
    // 到位後就地接敵
    set.AddRule(DoctrineRule(DoctrineTrigger::ObjectiveReached,
                             DoctrineAction::AttackNearest, 0.0f,
                             /*prio=*/20));
    // 主軸：沿箭頭推進
    set.AddRule(DoctrineRule(DoctrineTrigger::Always,
                             DoctrineAction::AdvanceToObjective, 0.0f,
                             arrow.priority));
    return set;
}

int BattlePlan::Apply(BattleController& battle, BattleResources* res,
                      int team) const {
    const PlanArrow* generic = nullptr;
    for (const PlanArrow& a : arrows) {
        if (a.squadName.empty()) {
            generic = &a;
            break;
        }
    }

    int assigned = 0;
    for (const auto& sp : battle.GetSquads()) {
        Squad* squad = sp.get();
        if (squad->GetTeam() != team) {
            continue;
        }
        const PlanArrow* arrow = nullptr;
        for (const PlanArrow& a : arrows) {
            if (a.squadName == squad->GetName()) {
                arrow = &a;
                break;
            }
        }
        if (!arrow) {
            arrow = generic;
        }
        if (!arrow) {
            continue; // 計畫沒有它的份——保持既有 doctrine
        }
        battle.AssignDoctrine(squad, ArrowToDoctrine(*arrow));
        battle.SetSquadObjective(squad, arrow->to);
        if (attackMul != 1.0f) {
            squad->SetDamagePerMember(squad->GetDamagePerMember() * attackMul);
        }
        ++assigned;
    }

    if (assigned > 0) {
        if (hasRally) {
            battle.SetRallyPoint(team, rallyPoint);
        }
        if (res) {
            res->AddIntel(team, bonusIntel);
            res->AddCP(battle, team, bonusCP);
        }
    }
    return assigned;
}

std::string BattlePlan::ToJson() const {
    std::string out = "{\"schema\":\"potato.battle_plan/1\",\"arrows\":[";
    for (size_t i = 0; i < arrows.size(); ++i) {
        const PlanArrow& a = arrows[i];
        char buf[320];
        std::snprintf(buf, sizeof(buf),
                      "{\"squad\":\"%s\",\"from\":[%.3f,%.3f],"
                      "\"to\":[%.3f,%.3f],\"priority\":%d}%s",
                      a.squadName.c_str(), a.from.x, a.from.y,
                      a.to.x, a.to.y, a.priority,
                      i + 1 < arrows.size() ? "," : "");
        out += buf;
    }
    char tail[256];
    std::snprintf(tail, sizeof(tail),
                  "],\"rally\":[%.3f,%.3f],\"has_rally\":%s,"
                  "\"attack_mul\":%.4f,\"bonus_intel\":%d,"
                  "\"bonus_cp\":%d}",
                  rallyPoint.x, rallyPoint.y, hasRally ? "true" : "false",
                  attackMul, bonusIntel, bonusCP);
    out += tail;
    return out;
}

bool BattlePlan::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.battle_plan/1") {
        return false;
    }
    arrows.clear();
    for (const JsonValue& a : root["arrows"].AsArray()) {
        const auto& f = a["from"].AsArray();
        const auto& t = a["to"].AsArray();
        if (f.size() != 2 || t.size() != 2) {
            return false;
        }
        PlanArrow arrow;
        arrow.squadName = a["squad"].AsString();
        arrow.from = Vector2(f[0].AsFloat(), f[1].AsFloat());
        arrow.to = Vector2(t[0].AsFloat(), t[1].AsFloat());
        arrow.priority = a["priority"].AsInt(30);
        arrows.push_back(arrow);
    }
    const auto& r = root["rally"].AsArray();
    if (r.size() == 2) {
        rallyPoint = Vector2(r[0].AsFloat(), r[1].AsFloat());
    }
    hasRally = root["has_rally"].AsBool(false);
    attackMul = root["attack_mul"].AsFloat(1.0f);
    if (attackMul <= 0.0f) {
        attackMul = 1.0f;
    }
    bonusIntel = root["bonus_intel"].AsInt(0);
    bonusCP = root["bonus_cp"].AsInt(0);
    return true;
}

} // namespace Gameplay
} // namespace Potato
