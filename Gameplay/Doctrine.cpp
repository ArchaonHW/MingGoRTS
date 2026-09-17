#include "Doctrine.h"
#include "Squad.h"

#include <algorithm>

namespace Potato {
namespace Gameplay {

void DoctrineSet::AddRule(const DoctrineRule& rule) {
    rules.push_back(rule);
    std::sort(rules.begin(), rules.end(),
              [](const DoctrineRule& a, const DoctrineRule& b) {
                  return a.priority < b.priority;
              });
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

} // namespace Gameplay
} // namespace Potato
