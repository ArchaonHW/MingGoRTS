#include "RivalDeck.h"

#include "BattleController.h"
#include "Serialization/JsonParser.h"

#include <cstdio>

namespace Potato {
namespace Gameplay {

void RivalDeck::RecordUsage(DoctrineTrigger trigger, int count) {
    if (count > 0) {
        usage[trigger] += count;
    }
}

void RivalDeck::RecordBattle(const BattleController& battle,
                             int playerTeam) {
    for (const auto& [trigger, count] : battle.TriggerUsage(playerTeam)) {
        usage[trigger] += count;
    }
}

int RivalDeck::UsageCount(DoctrineTrigger trigger) const {
    auto it = usage.find(trigger);
    return it != usage.end() ? it->second : 0;
}

int RivalDeck::TotalUsage() const {
    int total = 0;
    for (const auto& [t, c] : usage) {
        total += c;
    }
    return total;
}

bool RivalDeck::MostUsedTrigger(DoctrineTrigger& out) const {
    if (usage.empty()) {
        return false;
    }
    int best = -1;
    for (const auto& [t, c] : usage) {
        if (c > best) {
            best = c;
            out = t;
        }
    }
    return true;
}

DoctrineSet RivalDeck::BuildCounterDeck() const {
    DoctrineTrigger habit;
    DoctrineSet deck;
    if (!MostUsedTrigger(habit)) {
        return deck; // 無情資 → 空牌組
    }
    using DT = DoctrineTrigger;
    using DA = DoctrineAction;
    switch (habit) {
    case DT::Always: // 慣推進 → 據守誘敵，近距點殺
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::AttackWeakest,
                                  3.0f, 20));
        deck.AddRule(DoctrineRule(DT::Always, DA::HoldPosition, 0.0f, 90));
        break;
    case DT::HealthBelow: // 慣撤退 → 追擊壓迫
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::AttackNearest,
                                  4.0f, 20));
        deck.AddRule(DoctrineRule(DT::Always, DA::AttackWeakest, 0.0f, 90));
        break;
    case DT::EnemyInRange: // 慣接戰反應 → 拒戰拉扯
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::RetreatToRally,
                                  2.0f, 20));
        deck.AddRule(DoctrineRule(DT::AllyEngaged, DA::DefendNearestAlly,
                                  0.0f, 50));
        deck.AddRule(DoctrineRule(DT::Always, DA::HoldPosition, 0.0f, 90));
        break;
    case DT::MoraleBelow: // 慣士氣控管 → 士氣壓制全線強攻
        deck.AddRule(DoctrineRule(DT::Always, DA::AttackNearest, 0.0f,
                                  90));
        break;
    case DT::UnderAttack: // 慣挨打反應 → 佯攻調動
        deck.AddRule(DoctrineRule(DT::AllyEngaged, DA::DefendNearestAlly,
                                  0.0f, 30));
        deck.AddRule(DoctrineRule(DT::Always, DA::AdvanceToObjective,
                                  0.0f, 90));
        break;
    case DT::Outnumbered: // 慣以少敵多判斷 → 正面決戰
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::AttackNearest,
                                  5.0f, 20));
        deck.AddRule(DoctrineRule(DT::Always, DA::AttackNearest, 0.0f,
                                  90));
        break;
    case DT::AllyEngaged: // 慣友軍連動 → 分割各個擊破
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::AttackWeakest,
                                  4.0f, 20));
        deck.AddRule(DoctrineRule(DT::Always, DA::AttackNearest, 0.0f,
                                  90));
        break;
    case DT::ObjectiveReached: // 慣搶點 → 目標區埋伏
        deck.AddRule(DoctrineRule(DT::ObjectiveReached, DA::HoldPosition,
                                  0.0f, 20));
        deck.AddRule(DoctrineRule(DT::EnemyInRange, DA::AttackNearest,
                                  3.0f, 40));
        deck.AddRule(DoctrineRule(DT::Always, DA::HoldPosition, 0.0f,
                                  90));
        break;
    }
    return deck;
}

std::string RivalDeck::WarningLine() const {
    DoctrineTrigger habit;
    if (!MostUsedTrigger(habit)) {
        return "彼之陣法未明——無足判斷";
    }
    std::string line = "彼之陣法，似針對我軍慣用「";
    line += TriggerName(habit);
    line += "」";
    return line;
}

std::string RivalDeck::ToJson() const {
    std::string out =
        "{\"schema\":\"potato.rival_deck/1\",\"usage\":{";
    bool first = true;
    for (const auto& [t, c] : usage) {
        if (!first) {
            out += ",";
        }
        first = false;
        char buf[96];
        std::snprintf(buf, sizeof(buf), "\"%s\":%d", TriggerName(t), c);
        out += buf;
    }
    out += "}}";
    return out;
}

bool RivalDeck::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.rival_deck/1") {
        return false;
    }
    usage.clear();
    const JsonValue& u = root["usage"];
    for (const auto& kv : u.objectValue) {
        DoctrineTrigger t;
        if (TriggerFromName(kv.first, t)) {
            usage[t] = kv.second.AsInt(0);
        }
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
