#include "BattlePlan.h"

#include "BattleController.h"
#include "BattleResources.h"
#include "QuantumFog.h"
#include "Squad.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cstdio>

namespace Potato {
namespace Gameplay {

namespace {
// 箭頭尖端確定度：各 fog entity 在 tip 半徑 R 內的「佔有確定度」取最大值。
//   未解析雲：候選質量和——0 質量 = 雲根本沒聲稱尖端，不構成懷疑（跳過）
//   已解析雲：真身在 R 內 → 1.0（坐實）；在 R 外 → 0.0（情報證明落空，
//            這朵雲曾讓尖端有嫌疑——計畫賭輸）
// 沒有任何 entity 對尖端表態（無 fog / 全為未解析且無質量）→ 1.0，
// 空地無疑，行為與加成功能前一致（向後相容）。
// fog 實體皆敵方所見——不按 team 過濾（UncertainEntity::team 是觀測方）。
float TipCertainty(const QuantumFog* fog, const Vector2& tip, float radius) {
    if (!fog) return 1.0f;
    const float r2 = radius * radius;
    float certainty = 0.0f;
    bool anyClaim = false;
    for (size_t e = 0; e < fog->EntityCount(); ++e) {
        const int eid = static_cast<int>(e);
        if (fog->IsRevealed(eid)) {
            anyClaim = true;
            const Vector2 rp = fog->GetRevealedPos(eid);
            const float dx = rp.x - tip.x;
            const float dy = rp.y - tip.y;
            certainty = (std::max)(certainty,
                                   (dx * dx + dy * dy <= r2) ? 1.0f : 0.0f);
            continue;
        }
        float mass = 0.0f;
        for (const auto& cand : fog->GetCloud(eid)) {
            const float dx = cand.first.x - tip.x;
            const float dy = cand.first.y - tip.y;
            if (dx * dx + dy * dy <= r2) {
                mass += static_cast<float>(cand.second);
            }
        }
        if (mass > 0.0f) {
            anyClaim = true;
            certainty = (std::max)(certainty, mass);
        }
    }
    return anyClaim ? certainty : 1.0f;
}
} // namespace

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
    squadArrowIdx.clear(); // 重建綁定——未再指派的小隊不留陳舊映射
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
        // 不疊乘；換一份倍率不同的計畫會先除回舊倍率再乘新倍率。
        // 量子感知：箭頭尖端壓在未解析機率雲上時，加成按確定度縮放
        const float effMul =
            1.0f + (attackMul - 1.0f) *
                       TipCertainty(battle.GetFog(), arrow->to,
                                    uncertaintyRadius);
        const float prevMul = squad->GetPlanAttackMul();
        if (effMul != prevMul) {
            squad->SetDamagePerMember(
                squad->GetDamagePerMember() / prevMul * effMul);
            squad->SetPlanAttackMul(effMul);
        }
        squadArrowIdx[squad] = static_cast<int>(arrow - arrows.data());
        ++assigned;
    }
    appliedTeam = assigned > 0 ? team : appliedTeam;

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

void BattlePlan::SetUncertaintyRadius(float r) {
    if (r > 0.0f) uncertaintyRadius = r; // 非正數忽略（同 SetFogRevealRange 語意）
}

void BattlePlan::UpdateUncertainty(BattleController& battle) const {
    if (appliedTeam < 0 || squadArrowIdx.empty() || attackMul <= 1.0f) {
        return; // 未 Apply / 無綁定 / 無加成——沒有可縮放的東西
    }
    QuantumFog* fog = battle.GetFog();
    for (const auto& kv : squadArrowIdx) {
        Squad* squad = kv.first;
        const int idx = kv.second;
        if (!squad || squad->IsEliminated() || squad->GetTeam() != appliedTeam ||
            idx < 0 || static_cast<size_t>(idx) >= arrows.size()) {
            continue;
        }
        const PlanArrow& arrow = arrows[static_cast<size_t>(idx)];
        const float effMul =
            1.0f + (attackMul - 1.0f) *
                       TipCertainty(fog, arrow.to, uncertaintyRadius);
        // 冪等正規化：除舊乘新——情報沒變時 effMul == prevMul，
        // damagePerMember 完全不動（逐位元一致）
        const float prevMul = squad->GetPlanAttackMul();
        if (effMul != prevMul) {
            squad->SetDamagePerMember(
                squad->GetDamagePerMember() / prevMul * effMul);
            squad->SetPlanAttackMul(effMul);
        }
    }
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
    // 載入的計畫尚未 Apply——清掉舊綁定，否則 UpdateUncertainty
    // 會拿新箭頭 index 去重評舊小隊映射
    squadArrowIdx.clear();
    appliedTeam = -1;
    return true;
}

} // namespace Gameplay
} // namespace Potato
