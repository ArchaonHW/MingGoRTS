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

bool BattlePlan::RemoveArrowFor(const std::string& squadName) {
    for (auto it = arrows.begin(); it != arrows.end(); ++it) {
        if (it->squadName == squadName) {
            arrows.erase(it);
            return true;
        }
    }
    return false;
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
        // 計畫加成依 Squad::planAttackMul 正規化：重複 Apply 同倍率
        // 不疊乘；換一份倍率不同的計畫會先除回舊倍率再乘新倍率
        const float prevMul = squad->GetPlanAttackMul();
        if (attackMul != prevMul) {
            squad->SetDamagePerMember(
                squad->GetDamagePerMember() / prevMul * attackMul);
            squad->SetPlanAttackMul(attackMul);
        }
        ++assigned;
    }

    // 集結點寫入本身是冪等的；intel/CP 只入帳一次——重複 Apply
    // 不重複補給；且至少要有一支小隊獲指派才發放（空計畫不白拿加成）
    if (assigned > 0) {
        if (hasRally) {
            battle.SetRallyPoint(team, rallyPoint);
        }
        if (res && !bonusCredited) {
            res->AddIntel(team, bonusIntel);
            res->AddCP(battle, team, bonusCP);
            bonusCredited = true;
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
    // 先全部解析進暫存——任一欄位壞掉就整筆作廢，不動既有計畫
    std::vector<PlanArrow> parsed;
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
        parsed.push_back(arrow);
    }
    Vector2 newRally = rallyPoint;
    const auto& r = root["rally"].AsArray();
    if (r.size() == 2) {
        newRally = Vector2(r[0].AsFloat(), r[1].AsFloat());
    }
    float newMul = root["attack_mul"].AsFloat(1.0f);
    if (newMul <= 0.0f) {
        newMul = 1.0f; // 非正倍率同 SetPlanBonus 收斂到 1.0
    }
    // 全部驗過才落寫
    arrows = std::move(parsed);
    rallyPoint = newRally;
    hasRally = root["has_rally"].AsBool(false); // 缺欄位歸零，不留舊值
    attackMul = newMul;
    bonusIntel = root["bonus_intel"].AsInt(0);
    if (bonusIntel < 0) bonusIntel = 0;
    bonusCP = root["bonus_cp"].AsInt(0);
    if (bonusCP < 0) bonusCP = 0;
    bonusCredited = false; // 載入的計畫尚未入帳
    return true;
}

} // namespace Gameplay
} // namespace Potato
