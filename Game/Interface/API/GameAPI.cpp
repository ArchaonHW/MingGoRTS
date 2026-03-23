#include "GameAPI.h"
#include "../../Core/Engine/GameEngine.h"
#include "../../Core/Campaign/CampaignLayer.h"
#include "../../Core/Battle/BattleLayer.h"
#include "../../Core/Faction/FactionManager.h"
#include "../../Core/Faction/Faction.h"
#include "../../Core/Campaign/Settlement.h"
#include "../../Core/Unit/Unit.h"
#include <sstream>
#include <iomanip>

namespace MingGoRTS {

GameAPI::GameAPI(GameEngine* engine)
    : engine_(engine) {
}

GameAPI::~GameAPI() {
    Shutdown();
}

void GameAPI::Initialize() {
    // 初始化 API
}

void GameAPI::Shutdown() {
    eventSubscriptions_.clear();
}

APIResponse GameAPI::GetGameState() const {
    APIResponse response;
    response.success = true;
    
    std::stringstream ss;
    ss << "{";
    ss << "\"state\":" << static_cast<int>(engine_->GetState()) << ",";
    ss << "\"turn\":" << engine_->GetCurrentTurn() << ",";
    ss << "\"phase\":" << static_cast<int>(engine_->GetCurrentPhase());
    ss << "}";
    
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetCurrentTurn() const {
    APIResponse response;
    response.success = true;
    response.data = std::to_string(engine_->GetCurrentTurn());
    return response;
}

APIResponse GameAPI::GetCurrentPhase() const {
    APIResponse response;
    response.success = true;
    response.data = std::to_string(static_cast<int>(engine_->GetCurrentPhase()));
    return response;
}

APIResponse GameAPI::GetCampaignMap(int x, int y, int width, int height) const {
    APIResponse response;
    response.success = true;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.success = false;
        response.errorMessage = "Campaign layer not available";
        return response;
    }
    
    std::stringstream ss;
    ss << "{";
    ss << "\"x\":" << x << ",";
    ss << "\"y\":" << y << ",";
    ss << "\"width\":" << width << ",";
    ss << "\"height\":" << height << ",";
    ss << "\"tiles\":[";
    
    bool first = true;
    for (int row = y; row < y + height && row < campaign->GetHeight(); ++row) {
        for (int col = x; col < x + width && col < campaign->GetWidth(); ++col) {
            CampaignTile* tile = campaign->GetTile(col, row);
            if (tile) {
                if (!first) ss << ",";
                first = false;
                
                ss << "{";
                ss << "\"x\":" << col << ",";
                ss << "\"y\":" << row << ",";
                ss << "\"terrain\":" << static_cast<int>(tile->terrain) << ",";
                ss << "\"owner\":" << tile->ownerFaction << ",";
                ss << "\"hasArmy\":" << (tile->hasArmy ? "true" : "false") << ",";
                ss << "\"settlement\":" << tile->settlementId;
                ss << "}";
            }
        }
    }
    
    ss << "]}";
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetTile(int x, int y) const {
    APIResponse response;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.success = false;
        response.errorMessage = "Campaign layer not available";
        return response;
    }
    
    CampaignTile* tile = campaign->GetTile(x, y);
    if (!tile) {
        response.success = false;
        response.errorMessage = "Invalid coordinates";
        return response;
    }
    
    std::stringstream ss;
    ss << "{";
    ss << "\"x\":" << x << ",";
    ss << "\"y\":" << y << ",";
    ss << "\"terrain\":" << static_cast<int>(tile->terrain) << ",";
    ss << "\"owner\":" << tile->ownerFaction << ",";
    ss << "\"hasArmy\":" << (tile->hasArmy ? "true" : "false") << ",";
    ss << "\"armyFaction\":" << tile->armyFaction << ",";
    ss << "\"settlement\":" << tile->settlementId;
    ss << "}";
    
    response.success = true;
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::MoveArmy(int armyId, float x, float y) {
    APIResponse response;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.success = false;
        response.errorMessage = "Campaign layer not available";
        return response;
    }
    
    bool success = campaign->MoveArmy(armyId, Vector2D(x, y));
    
    response.success = success;
    if (!success) {
        response.errorMessage = "Failed to move army";
    } else {
        response.data = "Army moved successfully";
    }
    
    return response;
}

APIResponse GameAPI::AttackArmy(int attackerId, int defenderId) {
    APIResponse response;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.success = false;
        response.errorMessage = "Campaign layer not available";
        return response;
    }
    
    if (!campaign->CanAttack(attackerId, defenderId)) {
        response.success = false;
        response.errorMessage = "Cannot attack - too far or same faction";
        return response;
    }
    
    campaign->StartBattle(attackerId, defenderId);
    
    response.success = true;
    response.data = "Battle started";
    return response;
}

APIResponse GameAPI::CreateArmy(int factionId, float x, float y) {
    APIResponse response;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.success = false;
        response.errorMessage = "Campaign layer not available";
        return response;
    }
    
    int armyId = campaign->CreateArmy(factionId, Vector2D(x, y));
    
    response.success = true;
    response.data = std::to_string(armyId);
    return response;
}

APIResponse GameAPI::GetArmies(int factionId) const {
    APIResponse response;
    response.success = true;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.data = "[]";
        return response;
    }
    
    // 简化：返回所有可见军队
    auto armies = campaign->GetVisibleArmies(factionId);
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < armies.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "{";
        ss << "\"id\":" << armies[i]->id << ",";
        ss << "\"faction\":" << armies[i]->factionId << ",";
        ss << "\"x\":" << armies[i]->position.x << ",";
        ss << "\"y\":" << armies[i]->position.y << ",";
        ss << "\"units\":" << armies[i]->unitCount << ",";
        ss << "\"movement\":" << armies[i]->movementPoints;
        ss << "}";
    }
    ss << "]";
    
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetSettlements(int factionId) const {
    APIResponse response;
    response.success = true;
    
    CampaignLayer* campaign = engine_->GetCampaignLayer();
    if (!campaign) {
        response.data = "[]";
        return response;
    }
    
    auto settlements = campaign->GetSettlementsByFaction(factionId);
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < settlements.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "{";
        ss << "\"id\":" << settlements[i]->GetId() << ",";
        ss << "\"name\":\"" << settlements[i]->GetName() << "\",";
        ss << "\"x\":" << settlements[i]->GetPosition().x << ",";
        ss << "\"y\":" << settlements[i]->GetPosition().y << ",";
        ss << "\"population\":" << settlements[i]->GetPopulation() << ",";
        ss << "\"income\":" << settlements[i]->GetIncome() << ",";
        ss << "\"publicOrder\":" << settlements[i]->GetPublicOrder();
        ss << "}";
    }
    ss << "]";
    
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetBattleState() const {
    APIResponse response;
    
    BattleLayer* battle = engine_->GetBattleLayer();
    if (!battle) {
        response.success = false;
        response.errorMessage = "Not in battle";
        return response;
    }
    
    std::stringstream ss;
    ss << "{";
    ss << "\"state\":" << static_cast<int>(battle->GetState()) << ",";
    ss << "\"result\":" << static_cast<int>(battle->GetResult()) << ",";
    ss << "\"attacker\":" << battle->GetAttackerFaction() << ",";
    ss << "\"defender\":" << battle->GetDefenderFaction() << ",";
    ss << "\"time\":" << battle->GetElapsedTime();
    ss << "}";
    
    response.success = true;
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetUnitsInBattle(int factionId) const {
    APIResponse response;
    response.success = true;
    
    BattleLayer* battle = engine_->GetBattleLayer();
    if (!battle) {
        response.data = "[]";
        return response;
    }
    
    auto units = battle->GetUnitsByFaction(factionId);
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < units.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "{";
        ss << "\"id\":" << units[i]->GetId() << ",";
        ss << "\"type\":" << units[i]->GetUnitTypeId() << ",";
        ss << "\"x\":" << units[i]->GetPosition().x << ",";
        ss << "\"y\":" << units[i]->GetPosition().y << ",";
        ss << "\"health\":" << units[i]->GetHealthPercent() << ",";
        ss << "\"morale\":" << units[i]->GetMorale() << ",";
        ss << "\"state\":" << static_cast<int>(units[i]->GetState());
        ss << "}";
    }
    ss << "]";
    
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::MoveUnit(int unitId, float x, float y) {
    APIResponse response;
    
    BattleLayer* battle = engine_->GetBattleLayer();
    if (!battle) {
        response.success = false;
        response.errorMessage = "Not in battle";
        return response;
    }
    
    battle->MoveUnit(unitId, Vector2D(x, y));
    
    response.success = true;
    response.data = "Unit moving";
    return response;
}

APIResponse GameAPI::GetAllFactions() const {
    APIResponse response;
    response.success = true;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.data = "[]";
        return response;
    }
    
    auto factions = fm->GetAllFactions();
    
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < factions.size(); ++i) {
        if (i > 0) ss << ",";
        ss << "{";
        ss << "\"id\":" << factions[i]->GetId() << ",";
        ss << "\"name\":\"" << factions[i]->GetName() << "\",";
        ss << "\"culture\":" << static_cast<int>(factions[i]->GetCulture()) << ",";
        ss << "\"money\":" << factions[i]->GetMoney() << ",";
        ss << "\"settlements\":" << factions[i]->GetSettlementCount() << ",";
        ss << "\"armies\":" << factions[i]->GetArmyCount();
        ss << "}";
    }
    ss << "]";
    
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::GetFaction(int factionId) const {
    APIResponse response;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.success = false;
        response.errorMessage = "Faction manager not available";
        return response;
    }
    
    Faction* faction = fm->GetFaction(factionId);
    if (!faction) {
        response.success = false;
        response.errorMessage = "Faction not found";
        return response;
    }
    
    std::stringstream ss;
    ss << "{";
    ss << "\"id\":" << faction->GetId() << ",";
    ss << "\"name\":\"" << faction->GetName() << "\",";
    ss << "\"culture\":" << static_cast<int>(faction->GetCulture()) << ",";
    ss << "\"money\":" << faction->GetMoney() << ",";
    ss << "\"income\":" << faction->GetIncome() << ",";
    ss << "\"settlements\":" << faction->GetSettlementCount() << ",";
    ss << "\"armies\":" << faction->GetArmyCount() << ",";
    ss << "\"population\":" << faction->GetTotalPopulation() << ",";
    ss << "\"isAI\":" << (faction->IsAI() ? "true" : "false") << ",";
    ss << "\"stability\":" << faction->GetPoliticalStability();
    ss << "}";
    
    response.success = true;
    response.data = ss.str();
    return response;
}

APIResponse GameAPI::EndTurn() {
    APIResponse response;
    
    engine_->EndTurn();
    
    response.success = true;
    response.data = "Turn ended";
    return response;
}

APIResponse GameAPI::NextPhase() {
    APIResponse response;
    
    engine_->NextPhase();
    
    response.success = true;
    response.data = "Phase advanced";
    return response;
}

APIResponse GameAPI::GetMoney(int factionId) const {
    APIResponse response;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.success = false;
        response.errorMessage = "Faction manager not available";
        return response;
    }
    
    Faction* faction = fm->GetFaction(factionId);
    if (!faction) {
        response.success = false;
        response.errorMessage = "Faction not found";
        return response;
    }
    
    response.success = true;
    response.data = std::to_string(faction->GetMoney());
    return response;
}

APIResponse GameAPI::DeclareWar(int factionId) {
    APIResponse response;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.success = false;
        response.errorMessage = "Faction manager not available";
        return response;
    }
    
    Faction* player = fm->GetPlayerFaction();
    Faction* target = fm->GetFaction(factionId);
    
    if (!player || !target) {
        response.success = false;
        response.errorMessage = "Invalid faction";
        return response;
    }
    
    player->DeclareWar(factionId);
    
    response.success = true;
    response.data = "War declared";
    return response;
}

// 其他方法的简化实现
APIResponse GameAPI::GetVisibleArmies(int factionId) const {
    return GetArmies(factionId);
}

APIResponse GameAPI::GetSettlement(int settlementId) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::CreateSettlement(float x, float y, int factionId, const std::string& name) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::UpgradeSettlement(int settlementId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::BuildInSettlement(int settlementId, const std::string& buildingType) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::SpawnUnit(int unitTypeId, int factionId, float x, float y) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::AttackUnit(int attackerId, int targetId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::RotateUnit(int unitId, float angle) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::ChangeFormation(int unitId, int formationId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::RetreatUnit(int unitId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::GetPlayerFaction() const {
    APIResponse response;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.success = false;
        response.errorMessage = "Faction manager not available";
        return response;
    }
    
    Faction* player = fm->GetPlayerFaction();
    if (!player) {
        response.success = false;
        response.errorMessage = "No player faction";
        return response;
    }
    
    return GetFaction(player->GetId());
}

APIResponse GameAPI::GetFactionRelationship(int faction1, int faction2) const {
    APIResponse response;
    
    FactionManager* fm = engine_->GetFactionManager();
    if (!fm) {
        response.success = false;
        response.errorMessage = "Faction manager not available";
        return response;
    }
    
    Faction* f1 = fm->GetFaction(faction1);
    if (!f1) {
        response.success = false;
        response.errorMessage = "Faction not found";
        return response;
    }
    
    response.success = true;
    response.data = std::to_string(f1->GetRelationship(faction2));
    return response;
}

APIResponse GameAPI::MakePeace(int factionId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::FormAlliance(int factionId) {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::GetIncome(int factionId) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::GetFactionStats(int factionId) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::QuerySettlements(const std::string& filter) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::QueryArmies(const std::string& filter) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

APIResponse GameAPI::QueryUnits(const std::string& filter) const {
    APIResponse response;
    response.success = false;
    response.errorMessage = "Not implemented";
    return response;
}

void GameAPI::SubscribeToEvent(const std::string& eventType, std::function<void(const GameEvent&)> callback) {
    eventSubscriptions_[eventType].push_back(callback);
}

void GameAPI::UnsubscribeFromEvent(const std::string& eventType) {
    eventSubscriptions_.erase(eventType);
}

std::string GameAPI::SerializeVector2D(const Vector2D& vec) const {
    return "{" + std::to_string(vec.x) + "," + std::to_string(vec.y) + "}";
}

Vector2D GameAPI::DeserializeVector2D(const std::string& data) const {
    // 简化实现
    return Vector2D(0, 0);
}

} // namespace MingGoRTS
