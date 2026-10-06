#include "PlanningDeck.h"

#include "BattleController.h"
#include "BattlePlanner.h"
#include "Squad.h"

#include <algorithm>

namespace Potato {
namespace Gameplay {

void PlanningDeck::SortDeck(SquadDeck& deck) {
    std::stable_sort(deck.rules.begin(), deck.rules.end(),
                     [](const DoctrineRule& a, const DoctrineRule& b) {
                         return a.priority < b.priority;
                     });
}

void PlanningDeck::Init(const BattleController& battle, int team) {
    decks.clear();
    rationales.clear();
    summary.clear();
    suggestedCP = 0;
    for (const auto& squad : battle.GetSquads()) {
        if (squad->GetTeam() != team) {
            continue;
        }
        SquadDeck deck;
        deck.squad = squad.get();
        if (const DoctrineSet* existing = battle.GetDoctrine(deck.squad)) {
            deck.rules = existing->Rules();
            SortDeck(deck);
        }
        decks.push_back(std::move(deck));
    }
}

void PlanningDeck::LoadPlannerTemplate(const BattlePlanner& planner,
                                       const BattleController& battle,
                                       int team) {
    const BattlePlanner::Plan plan = planner.GeneratePlan(battle, team);
    summary = plan.summary;
    suggestedCP = plan.suggestedCP;
    for (const auto& sp : plan.squadPlans) {
        const int idx = FindDeck(sp.squad);
        if (idx < 0) {
            continue;
        }
        decks[idx].rules = sp.doctrine.Rules();
        SortDeck(decks[idx]);
        rationales[decks[idx].squad] = sp.rationale;
    }
}

int PlanningDeck::FindDeck(const Squad* squad) const {
    for (size_t i = 0; i < decks.size(); ++i) {
        if (decks[i].squad == squad) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

bool PlanningDeck::AddRule(int deckIndex, const DoctrineRule& rule) {
    if (deckIndex < 0 || deckIndex >= SquadCount()) {
        return false;
    }
    SquadDeck& deck = decks[deckIndex];
    if (static_cast<int>(deck.rules.size()) >= slotCap) {
        return false;
    }
    deck.rules.push_back(rule);
    SortDeck(deck);
    return true;
}

bool PlanningDeck::RemoveRule(int deckIndex, int ruleIndex) {
    if (deckIndex < 0 || deckIndex >= SquadCount()) {
        return false;
    }
    SquadDeck& deck = decks[deckIndex];
    if (ruleIndex < 0 || ruleIndex >= static_cast<int>(deck.rules.size())) {
        return false;
    }
    deck.rules.erase(deck.rules.begin() + ruleIndex);
    return true;
}

bool PlanningDeck::SetRule(int deckIndex, int ruleIndex,
                           const DoctrineRule& rule) {
    if (deckIndex < 0 || deckIndex >= SquadCount()) {
        return false;
    }
    SquadDeck& deck = decks[deckIndex];
    if (ruleIndex < 0 || ruleIndex >= static_cast<int>(deck.rules.size())) {
        return false;
    }
    deck.rules[ruleIndex] = rule;
    SortDeck(deck);
    return true;
}

bool PlanningDeck::SwapRules(int deckIndex, int a, int b) {
    if (deckIndex < 0 || deckIndex >= SquadCount() || a == b) {
        return false;
    }
    SquadDeck& deck = decks[deckIndex];
    const int n = static_cast<int>(deck.rules.size());
    if (a < 0 || a >= n || b < 0 || b >= n) {
        return false;
    }
    // 交換 priority 後重排——UI 的「上移/下移」即調整判定順序
    std::swap(deck.rules[a].priority, deck.rules[b].priority);
    SortDeck(deck);
    return true;
}

std::vector<std::string> PlanningDeck::Validate() const {
    std::vector<std::string> warnings;
    for (const auto& deck : decks) {
        const std::string name =
            deck.squad ? deck.squad->GetName() : std::string("?");
        if (deck.rules.empty()) {
            warnings.push_back(std::string(name) +
                               "：卡組為空，只會原地駐守");
            continue;
        }
        bool hasFallback = false;
        for (const auto& rule : deck.rules) {
            if (rule.trigger == DoctrineTrigger::Always) {
                hasFallback = true;
                break;
            }
        }
        if (!hasFallback) {
            warnings.push_back(std::string(name) +
                               "：無 Always 墊底規則，條件全不命中時只會駐守");
        }
    }
    return warnings;
}

void PlanningDeck::Commit(BattleController& battle) const {
    for (const auto& deck : decks) {
        if (!deck.squad) {
            continue;
        }
        DoctrineSet ds;
        for (const auto& rule : deck.rules) {
            ds.AddRule(rule);
        }
        battle.AssignDoctrine(deck.squad, ds);
    }
}

const std::string& PlanningDeck::Rationale(const Squad* squad) const {
    static const std::string kEmpty;
    auto it = rationales.find(squad);
    return it != rationales.end() ? it->second : kEmpty;
}

} // namespace Gameplay
} // namespace Potato
