#include "EnemyGeneral.h"
#include "BattleController.h"
#include "Squad.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <fstream>
#include <sstream>

namespace Potato {
namespace Gameplay {

DoctrineTrigger EnemyGeneral::TriggerFromString(const std::string& s) {
    if (s == "HealthBelow") return DoctrineTrigger::HealthBelow;
    if (s == "MoraleBelow") return DoctrineTrigger::MoraleBelow;
    if (s == "EnemyInRange") return DoctrineTrigger::EnemyInRange;
    if (s == "UnderAttack") return DoctrineTrigger::UnderAttack;
    if (s == "Outnumbered") return DoctrineTrigger::Outnumbered;
    if (s == "AllyEngaged") return DoctrineTrigger::AllyEngaged;
    if (s == "ObjectiveReached") return DoctrineTrigger::ObjectiveReached;
    return DoctrineTrigger::Always;
}

DoctrineAction EnemyGeneral::ActionFromString(const std::string& s) {
    if (s == "AttackNearest") return DoctrineAction::AttackNearest;
    if (s == "AttackWeakest") return DoctrineAction::AttackWeakest;
    if (s == "AdvanceToObjective") return DoctrineAction::AdvanceToObjective;
    if (s == "RetreatToRally") return DoctrineAction::RetreatToRally;
    if (s == "DefendNearestAlly") return DoctrineAction::DefendNearestAlly;
    if (s == "Scout") return DoctrineAction::Scout;
    return DoctrineAction::HoldPosition;
}

bool EnemyGeneral::LoadFromFile(const std::string& path) {
    for (const char* prefix : {"", "../", "../../", "../../../"}) {
        std::ifstream f(std::string(prefix) + path);
        if (f) {
            std::ostringstream ss;
            ss << f.rdbuf();
            return LoadFromString(ss.str());
        }
    }
    return false;
}

bool EnemyGeneral::LoadFromString(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) || !root.IsObject()) {
        return false;
    }
    name = root["name"].AsString(name);
    epithet = root["epithet"].AsString();
    artPath = root["art"].AsString();
    faction = root["faction"].AsString();
    rarity = root["rarity"].AsString();
    cardId = root["id"].AsString();

    // G-2：卡可標兵種（unit_class）——ApplyTo 時覆寫全軍小隊
    {
        bool ok = false;
        const UnitClass uc =
            UnitClassFromString(root["unit_class"].AsString(), &ok);
        if (ok) { unitClass = uc; hasUnitClass = true; }
    }

    const JsonValue& p = root["personality"];
    aggression = std::clamp(p["aggression"].AsFloat(aggression), 0.0f, 100.0f);
    discipline = std::clamp(p["discipline"].AsFloat(discipline), 0.0f, 100.0f);
    cunning = std::clamp(p["cunning"].AsFloat(cunning), 0.0f, 100.0f);

    signatureCardId = root["signatureDoctrineId"].AsString();

    cards.clear();
    const JsonValue& sig = root["signatureDoctrine"];
    if (sig.IsObject()) {
        Card c;
        c.name = sig["name"].AsString("signature");
        c.rule = DoctrineRule(TriggerFromString(sig["trigger"].AsString()),
                              ActionFromString(sig["action"].AsString()),
                              sig["threshold"].AsFloat(0.0f), 0);
        cards.push_back(c);
    }
    // 附加卡槽（cards 陣列，最多補到 4 張）
    for (const auto& j : root["cards"].AsArray()) {
        Card c;
        c.name = j["name"].AsString("card");
        c.rule = DoctrineRule(TriggerFromString(j["trigger"].AsString()),
                              ActionFromString(j["action"].AsString()),
                              j["threshold"].AsFloat(0.0f),
                              j["priority"].AsInt(50),
                              j["cooldown"].AsFloat(0.0f));
        cards.push_back(c);
    }
    return true;
}

EnemyGeneral EnemyGeneral::MakeGlock() {
    EnemyGeneral g;
    g.name = "格洛克";
    g.epithet = "斷橋守將";
    g.aggression = 90.0f;
    g.discipline = 40.0f;
    g.cunning = 10.0f;

    // 四張手寫卡：見敵就咬、被圍就退、士氣崩潰早退、預設壓上
    g.cards = {
        {"猛撲",   {DoctrineTrigger::EnemyInRange,
                    DoctrineAction::AttackNearest, 6.0f, 0}},
        {"止損",   {DoctrineTrigger::Outnumbered,
                    DoctrineAction::RetreatToRally, 0.0f, 20}},
        {"潰逃線", {DoctrineTrigger::MoraleBelow,
                    DoctrineAction::RetreatToRally, 0.5f, 30}},
        {"壓上",   {DoctrineTrigger::Always,
                    DoctrineAction::AdvanceToObjective, 0.0f, 100}},
    };
    return g;
}

DoctrineSet EnemyGeneral::BuildDoctrineFor(const Squad& squad,
                                           int rank, int count) const {
    (void)squad;
    DoctrineSet d;

    // 手寫卡優先放（priority 已編排）
    for (const auto& c : cards) {
        d.AddRule(c.rule);
    }

    // 人格補丁：狡詐 > 50 才會「打最弱的」
    if (cunning > 50.0f) {
        d.AddRule({DoctrineTrigger::EnemyInRange,
                   DoctrineAction::AttackWeakest, 4.0f, 15});
    }
    // 高紀律：攻擊卡帶冷卻，避免無腦連打（透過卡片本身 cooldown 表達，
    // 這裡不額外加規則）
    // 低侵略 → 撤退線提高；高侵略 → 降低
    float retreatHp = 0.45f - aggression * 0.003f; // 90→0.18, 40→0.33
    d.AddRule({DoctrineTrigger::HealthBelow,
               DoctrineAction::RetreatToRally, retreatHp, 40});

    // 預設動作：紀律 >= 60 守位等敵；否則往目標推進
    DoctrineAction fallback =
        discipline >= 60.0f ? DoctrineAction::HoldPosition
                            : DoctrineAction::AdvanceToObjective;
    // 只有隊尾（最後一名）且紀律高才死守；其餘仍推進
    if (fallback == DoctrineAction::HoldPosition &&
        !(count > 1 && rank == count - 1)) {
        fallback = DoctrineAction::AdvanceToObjective;
    }
    d.AddRule({DoctrineTrigger::Always, fallback, 0.0f, 100});
    return d;
}

void EnemyGeneral::ApplyTo(BattleController& battle, int team) const {
    // 依兵力排序：最強的帶完整腳本，其餘同套（敵將一套打法打全軍）
    std::vector<Squad*> mine;
    for (const auto& s : battle.GetSquads()) {
        if (s->GetTeam() == team && !s->IsEliminated()) {
            mine.push_back(s.get());
        }
    }
    std::sort(mine.begin(), mine.end(), [](const Squad* a, const Squad* b) {
        return a->GetMembers() > b->GetMembers();
    });
    for (size_t i = 0; i < mine.size(); ++i) {
        if (hasUnitClass) mine[i]->SetUnitClass(unitClass); // G-2
        battle.AssignDoctrine(
            mine[i],
            BuildDoctrineFor(*mine[i], static_cast<int>(i),
                             static_cast<int>(mine.size())));
    }
}

Vector2 EnemyGeneral::FogBiasPoint(Vector2 center, Vector2 enemyDir,
                                   float radius) const {
    const float len = enemyDir.Length();
    if (len < 1e-4f) return center; // 無方向資訊 → 中立
    const Vector2 dir = enemyDir * (1.0f / len);
    const Vector2 perp(-dir.y, dir.x); // 左側翼

    // 侵略:50 中立,±0.8r 沿敵向偏移(90→前推 0.64r)
    // 狡詐:50 中立,±0.4r 側翼偏移(90→+0.32r 左翼)
    const float fwd = (aggression / 100.0f - 0.5f) * 1.6f * radius;
    const float flank = (cunning / 100.0f - 0.5f) * 0.8f * radius;
    return center + dir * fwd + perp * flank;
}

float EnemyGeneral::FogPriorScale() const {
    // 紀律 → 先驗集中度:100→0.6(守位集中),0→1.4(鬆散),50→1.0
    return 1.4f - 0.8f * (discipline / 100.0f);
}

} // namespace Gameplay
} // namespace Potato
