#include "Doctrine.h"
#include "Squad.h"
#include "Serialization/JsonParser.h"

#include <algorithm>
#include <cstdio>

namespace Potato {
namespace Gameplay {

void DoctrineSet::AddRule(const DoctrineRule& rule) {
    rules.push_back(rule);
    std::sort(rules.begin(), rules.end(),
              [](const DoctrineRule& a, const DoctrineRule& b) {
                  return a.priority < b.priority;
              });
}

bool DoctrineSet::RemoveRule(size_t index) {
    if (index >= rules.size()) {
        return false;
    }
    rules.erase(rules.begin() + static_cast<std::ptrdiff_t>(index));
    return true;
}

bool DoctrineSet::TriggerMatches(const DoctrineRule& rule, const SquadContext& ctx) {
    switch (rule.trigger) {
    case DoctrineTrigger::Always:
        return true;
    case DoctrineTrigger::HealthBelow:
        return ctx.healthPct < rule.threshold;
    case DoctrineTrigger::MoraleBelow:
        return ctx.moralePct < rule.threshold;
    case DoctrineTrigger::EnemyInRange:
        return ctx.nearestEnemyDist >= 0.0f &&
               ctx.nearestEnemyDist < rule.threshold;
    case DoctrineTrigger::UnderAttack:
        return ctx.underAttack;
    case DoctrineTrigger::Outnumbered:
        return ctx.outnumbered;
    case DoctrineTrigger::AllyEngaged:
        return ctx.anyAllyEngaged;
    case DoctrineTrigger::ObjectiveReached:
        return ctx.objectiveReached;
    }
    return false;
}

DoctrineAction DoctrineSet::Evaluate(const SquadContext& ctx) const {
    lastMatched = nullptr;
    for (const auto& rule : rules) {
        if (rule.coolingUntil > ctx.now) {
            continue; // 冷卻中，本輪略過
        }
        if (TriggerMatches(rule, ctx)) {
            rule.coolingUntil = ctx.now + rule.cooldown;
            lastMatched = &rule;
            return rule.action;
        }
    }
    return DoctrineAction::HoldPosition;
}

const char* TriggerName(DoctrineTrigger trigger) {
    switch (trigger) {
    case DoctrineTrigger::Always: return "Always";
    case DoctrineTrigger::HealthBelow: return "HealthBelow";
    case DoctrineTrigger::MoraleBelow: return "MoraleBelow";
    case DoctrineTrigger::EnemyInRange: return "EnemyInRange";
    case DoctrineTrigger::UnderAttack: return "UnderAttack";
    case DoctrineTrigger::Outnumbered: return "Outnumbered";
    case DoctrineTrigger::AllyEngaged: return "AllyEngaged";
    case DoctrineTrigger::ObjectiveReached: return "ObjectiveReached";
    }
    return "Unknown";
}

const char* ActionName(DoctrineAction action) {
    switch (action) {
    case DoctrineAction::AttackNearest: return "AttackNearest";
    case DoctrineAction::AttackWeakest: return "AttackWeakest";
    case DoctrineAction::AdvanceToObjective: return "AdvanceToObjective";
    case DoctrineAction::HoldPosition: return "HoldPosition";
    case DoctrineAction::RetreatToRally: return "RetreatToRally";
    case DoctrineAction::DefendNearestAlly: return "DefendNearestAlly";
    case DoctrineAction::Scout: return "Scout";
    }
    return "Unknown";
}

bool TriggerFromName(const std::string& name, DoctrineTrigger& out) {
    static const DoctrineTrigger all[] = {
        DoctrineTrigger::Always,          DoctrineTrigger::HealthBelow,
        DoctrineTrigger::MoraleBelow,     DoctrineTrigger::EnemyInRange,
        DoctrineTrigger::UnderAttack,     DoctrineTrigger::Outnumbered,
        DoctrineTrigger::AllyEngaged,     DoctrineTrigger::ObjectiveReached,
    };
    for (DoctrineTrigger t : all) {
        if (name == TriggerName(t)) {
            out = t;
            return true;
        }
    }
    return false;
}

bool ActionFromName(const std::string& name, DoctrineAction& out) {
    static const DoctrineAction all[] = {
        DoctrineAction::AttackNearest,   DoctrineAction::AttackWeakest,
        DoctrineAction::AdvanceToObjective, DoctrineAction::HoldPosition,
        DoctrineAction::RetreatToRally,  DoctrineAction::DefendNearestAlly,
        DoctrineAction::Scout,
    };
    for (DoctrineAction a : all) {
        if (name == ActionName(a)) {
            out = a;
            return true;
        }
    }
    return false;
}

std::string DoctrineSet::ToJson() const {
    std::string out = "{\"schema\":\"potato.doctrine_set/1\",\"rules\":[";
    for (size_t i = 0; i < rules.size(); ++i) {
        const DoctrineRule& r = rules[i];
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "{\"trigger\":\"%s\",\"action\":\"%s\","
                      "\"threshold\":%.4f,\"priority\":%d,\"cooldown\":%.2f}%s",
                      TriggerName(r.trigger), ActionName(r.action),
                      r.threshold, r.priority, r.cooldown,
                      i + 1 < rules.size() ? "," : "");
        out += buf;
    }
    out += "]}";
    return out;
}

bool DoctrineSet::FromJson(const std::string& json) {
    JsonValue root;
    if (!JsonValue::ParseOk(json, root) ||
        root["schema"].AsString() != "potato.doctrine_set/1") {
        return false;
    }
    std::vector<DoctrineRule> parsed;
    for (const JsonValue& r : root["rules"].AsArray()) {
        DoctrineRule rule;
        if (!TriggerFromName(r["trigger"].AsString(), rule.trigger) ||
            !ActionFromName(r["action"].AsString(), rule.action)) {
            return false; // 不認識的觸發/動作 → 整組作廢
        }
        rule.threshold = r["threshold"].AsFloat(0.0f);
        rule.priority = r["priority"].AsInt(100);
        rule.cooldown = r["cooldown"].AsFloat(0.0f);
        parsed.push_back(rule);
    }
    rules.clear();
    for (const DoctrineRule& r : parsed) {
        AddRule(r); // 走 AddRule 保 priority 排序
    }
    return true;
}

} // namespace Gameplay
} // namespace Potato
