#include "FactionManager.h"
#include <iostream>

namespace MingGoRTS {

FactionManager::FactionManager()
    : nextFactionId_(1)
    , playerFactionId_(-1) {
}

FactionManager::~FactionManager() {
    Shutdown();
}

void FactionManager::Initialize() {
    // 创建默认派系
    CreateDefaultFactions();
    std::cout << "[FactionManager] Initialized with " << factions_.size() << " factions" << std::endl;
}

void FactionManager::Shutdown() {
    factions_.clear();
    playerFactionId_ = -1;
}

int FactionManager::CreateFaction(const std::string& name, CultureType culture) {
    int id = nextFactionId_++;
    factions_[id] = std::make_unique<Faction>(id, name, culture);
    
    // 添加政治党派（简化：统治党和反对派）
    PoliticalParty rulingParty;
    rulingParty.id = 1;
    rulingParty.name = "Ruling Party";
    rulingParty.influence = 60.0f;
    rulingParty.gravitas = 50.0f;
    rulingParty.isRuling = true;
    rulingParty.leaderCharacterId = -1;
    
    PoliticalParty oppositionParty;
    oppositionParty.id = 2;
    oppositionParty.name = "Opposition";
    oppositionParty.influence = 40.0f;
    oppositionParty.gravitas = 30.0f;
    oppositionParty.isRuling = false;
    oppositionParty.leaderCharacterId = -1;
    
    factions_[id]->AddParty(rulingParty);
    factions_[id]->AddParty(oppositionParty);
    
    return id;
}

void FactionManager::DestroyFaction(int factionId) {
    auto it = factions_.find(factionId);
    if (it != factions_.end()) {
        // 更新其他派系的关系
        for (auto& [id, faction] : factions_) {
            if (id != factionId) {
                faction->SetRelationship(factionId, 0.0f);
            }
        }
        
        factions_.erase(it);
        
        if (playerFactionId_ == factionId) {
            playerFactionId_ = -1;
        }
    }
}

Faction* FactionManager::GetFaction(int factionId) {
    auto it = factions_.find(factionId);
    if (it != factions_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Faction*> FactionManager::GetAllFactions() {
    std::vector<Faction*> result;
    for (auto& [id, faction] : factions_) {
        result.push_back(faction.get());
    }
    return result;
}

std::vector<Faction*> FactionManager::GetAIFactions() {
    std::vector<Faction*> result;
    for (auto& [id, faction] : factions_) {
        if (faction->IsAI()) {
            result.push_back(faction.get());
        }
    }
    return result;
}

std::vector<Faction*> FactionManager::GetPlayerFactions() {
    std::vector<Faction*> result;
    for (auto& [id, faction] : factions_) {
        if (!faction->IsAI()) {
            result.push_back(faction.get());
        }
    }
    return result;
}

void FactionManager::SetPlayerFaction(int factionId) {
    // 重置旧玩家派系
    if (playerFactionId_ != -1) {
        Faction* oldPlayer = GetFaction(playerFactionId_);
        if (oldPlayer) {
            oldPlayer->SetAI(true);
        }
    }
    
    // 设置新玩家派系
    playerFactionId_ = factionId;
    Faction* newPlayer = GetFaction(factionId);
    if (newPlayer) {
        newPlayer->SetAI(false);
    }
}

Faction* FactionManager::GetPlayerFaction() {
    return GetFaction(playerFactionId_);
}

Faction* FactionManager::GetFactionByName(const std::string& name) {
    for (auto& [id, faction] : factions_) {
        if (faction->GetName() == name) {
            return faction.get();
        }
    }
    return nullptr;
}

std::vector<Faction*> FactionManager::GetFactionsByCulture(CultureType culture) {
    std::vector<Faction*> result;
    for (auto& [id, faction] : factions_) {
        if (faction->GetCulture() == culture) {
            result.push_back(faction.get());
        }
    }
    return result;
}

std::vector<int> FactionManager::GetWarEnemies(int factionId) {
    std::vector<int> result;
    Faction* faction = GetFaction(factionId);
    if (!faction) return result;
    
    for (auto& [id, other] : factions_) {
        if (id != factionId && faction->IsAtWar(id)) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> FactionManager::GetAllies(int factionId) {
    std::vector<int> result;
    Faction* faction = GetFaction(factionId);
    if (!faction) return result;
    
    for (auto& [id, other] : factions_) {
        if (id != factionId && faction->GetRelationship(id) > 50.0f) {
            result.push_back(id);
        }
    }
    return result;
}

bool FactionManager::AreAtWar(int faction1, int faction2) {
    Faction* f1 = GetFaction(faction1);
    if (!f1) return false;
    return f1->IsAtWar(faction2);
}

bool FactionManager::AreAllied(int faction1, int faction2) {
    Faction* f1 = GetFaction(faction1);
    if (!f1) return false;
    return f1->GetRelationship(faction2) > 50.0f;
}

void FactionManager::Update(float deltaTime) {
    for (auto& [id, faction] : factions_) {
        faction->Update(deltaTime);
    }
}

void FactionManager::ProcessEndOfTurn() {
    for (auto& [id, faction] : factions_) {
        faction->ProcessEndOfTurn();
    }
}

void FactionManager::CreateDefaultFactions() {
    CreateRomanFaction();
    CreateBarbarianFaction();
    CreateEasternFaction();
}

void FactionManager::CreateRomanFaction() {
    int id = CreateFaction("Rome", CultureType::Roman);
    Faction* faction = GetFaction(id);
    if (faction) {
        // 罗马特质
        FactionTrait trait;
        trait.name = "Roman Discipline";
        trait.description = "Roman units have high morale and can form testudo formation";
        trait.publicOrderModifier = 0.1f;
        trait.taxModifier = 0.15f;
        trait.armyMaintenanceModifier = 0.0f;
        trait.diplomacyBonus = 5.0f;
        trait.canRecruitMercenaries = true;
        
        faction->AddTrait(trait);
        faction->AddMoney(10000);  // 罗马初始资金较多
        
        // 设置胜利类型
        faction->SetVictoryType(Faction::VictoryType::Military);
    }
}

void FactionManager::CreateBarbarianFaction() {
    int id = CreateFaction("Barbarians", CultureType::Barbarian);
    Faction* faction = GetFaction(id);
    if (faction) {
        // 蛮族特质
        FactionTrait trait;
        trait.name = "Wild Fury";
        trait.description = "Barbarian units have high charge bonus but lower armor";
        trait.publicOrderModifier = -0.05f;
        trait.taxModifier = 0.0f;
        trait.armyMaintenanceModifier = -0.2f;
        trait.diplomacyBonus = -5.0f;
        trait.canRecruitMercenaries = true;
        
        faction->AddTrait(trait);
        faction->AddMoney(3000);
        
        faction->SetVictoryType(Faction::VictoryType::Cultural);
        faction->SetAI(true);
    }
}

void FactionManager::CreateEasternFaction() {
    int id = CreateFaction("Eastern Empire", CultureType::Eastern);
    Faction* faction = GetFaction(id);
    if (faction) {
        // 东方帝国特质
        FactionTrait trait;
        trait.name = "Eastern Wealth";
        trait.description = "High trade income but expensive units";
        trait.publicOrderModifier = 0.05f;
        trait.taxModifier = 0.2f;
        trait.armyMaintenanceModifier = 0.1f;
        trait.diplomacyBonus = 3.0f;
        trait.canRecruitMercenaries = true;
        
        faction->AddTrait(trait);
        faction->AddMoney(8000);
        
        faction->SetVictoryType(Faction::VictoryType::Economic);
        faction->SetAI(true);
    }
}

} // namespace MingGoRTS
