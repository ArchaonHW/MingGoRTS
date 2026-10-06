#include "BattleResources.h"
#include "BattleController.h"

namespace Potato {
namespace Gameplay {

void BattleResources::Setup(BattleController& battle, int team,
                            int intel, int cp) {
    TeamResources& r = teams[team];
    r.intel = intel;
    r.cp = cp;
    r.maxCP = cp;
    battle.SetCommandPoints(team, cp);
}

bool BattleResources::SpendIntel(int team, int amount) {
    auto it = teams.find(team);
    if (it == teams.end() || it->second.intel < amount) {
        return false;
    }
    it->second.intel -= amount;
    return true;
}

int BattleResources::GetIntel(int team) const {
    auto it = teams.find(team);
    return it != teams.end() ? it->second.intel : 0;
}

int BattleResources::GetCP(int team) const {
    auto it = teams.find(team);
    return it != teams.end() ? it->second.cp : 0;
}

void BattleResources::AddIntel(int team, int amount) {
    if (amount > 0) {
        teams[team].intel += amount;
    }
}

void BattleResources::AddCP(BattleController& battle, int team, int amount) {
    if (amount <= 0) {
        return;
    }
    TeamResources& r = teams[team];
    r.cp += amount;
    r.maxCP += amount;
    battle.SetCommandPoints(team, battle.GetCommandPoints(team) + amount);
}

bool BattleResources::RevealEnemyPersonality(int team) {
    if (!SpendIntel(team, 3)) { // 解鎖敵將人格花 3 情報
        return false;
    }
    personalityRevealed[team] = true;
    return true;
}

void BattleResources::AddLoot(int team, int points) {
    if (points > 0) teams[team].loot += points;
}

bool BattleResources::SpendLoot(int team, int points) {
    auto it = teams.find(team);
    if (it == teams.end() || it->second.loot < points) return false;
    it->second.loot -= points;
    return true;
}

int BattleResources::GetLoot(int team) const {
    auto it = teams.find(team);
    return it != teams.end() ? it->second.loot : 0;
}

void BattleResources::ApplyMoraleRule(BattleController& battle,
                                      float threshold, float rate) {
    battle.SetMoraleExecution(threshold, rate);
}

} // namespace Gameplay
} // namespace Potato
