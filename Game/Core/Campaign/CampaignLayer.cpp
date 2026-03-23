#include "CampaignLayer.h"
#include "Settlement.h"
#include "Province.h"
#include "../Faction/FactionManager.h"
#include "../Engine/GameEngine.h"
#include <queue>
#include <cmath>
#include <iostream>

namespace MingGoRTS {

CampaignLayer::CampaignLayer(GameEngine* engine)
    : engine_(engine)
    , mapWidth_(0)
    , mapHeight_(0)
    , nextArmyId_(1)
    , nextSettlementId_(1)
    , nextProvinceId_(1) {
}

CampaignLayer::~CampaignLayer() {
    Shutdown();
}

bool CampaignLayer::Initialize(int width, int height) {
    mapWidth_ = width;
    mapHeight_ = height;
    
    // 初始化地图
    map_.resize(height);
    for (int y = 0; y < height; ++y) {
        map_[y].resize(width);
        for (int x = 0; x < width; ++x) {
            map_[y][x].position = Vector2D(static_cast<float>(x), static_cast<float>(y));
            // 简单的地形生成逻辑
            if (x < width * 0.2f || x > width * 0.8f) {
                map_[y][x].terrain = TerrainType::Mountains;
            } else if (y < height * 0.3f) {
                map_[y][x].terrain = TerrainType::Forest;
            } else if (y > height * 0.7f) {
                map_[y][x].terrain = TerrainType::Desert;
            } else {
                map_[y][x].terrain = TerrainType::Plains;
            }
        }
    }
    
    std::cout << "[CampaignLayer] Initialized map: " << width << "x" << height << std::endl;
    return true;
}

void CampaignLayer::Shutdown() {
    armies_.clear();
    settlements_.clear();
    provinces_.clear();
    map_.clear();
}

void CampaignLayer::Update(float deltaTime) {
    // 更新军队移动
    for (auto& [id, army] : armies_) {
        if (army->isMoving) {
            Vector2D direction = army->targetPosition - army->position;
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            
            if (distance < 0.1f) {
                army->position = army->targetPosition;
                army->isMoving = false;
                
                // 更新地图标记
                GetTile(static_cast<int>(army->position.x), static_cast<int>(army->position.y))->hasArmy = true;
                GetTile(static_cast<int>(army->position.x), static_cast<int>(army->position.y))->armyFaction = army->factionId;
            } else {
                direction.x /= distance;
                direction.y /= distance;
                float speed = 2.0f * deltaTime;
                army->position.x += direction.x * speed;
                army->position.y += direction.y * speed;
            }
        }
    }
}

void CampaignLayer::ProcessEndOfTurn() {
    // 重置军队移动力
    for (auto& [id, army] : armies_) {
        army->movementPoints = army->maxMovementPoints;
        army->isMoving = false;
    }
    
    // 处理AI回合
    ProcessAIturns();
    
    // 处理经济收入
    auto* factionManager = engine_->GetFactionManager();
    for (const auto& [id, settlement] : settlements_) {
        auto* faction = factionManager->GetFaction(settlement->GetFactionId());
        if (faction) {
            faction->AddMoney(settlement->GetIncome());
        }
    }
}

CampaignTile* CampaignLayer::GetTile(int x, int y) {
    if (x >= 0 && x < mapWidth_ && y >= 0 && y < mapHeight_) {
        return &map_[y][x];
    }
    return nullptr;
}

CampaignTile* CampaignLayer::GetTile(const Vector2D& pos) {
    return GetTile(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

int CampaignLayer::CreateArmy(int factionId, Vector2D position) {
    int id = nextArmyId_++;
    armies_[id] = std::make_unique<CampaignArmy>(id, factionId, position);
    
    CampaignTile* tile = GetTile(position);
    if (tile) {
        tile->hasArmy = true;
        tile->armyFaction = factionId;
    }
    
    return id;
}

void CampaignLayer::DestroyArmy(int armyId) {
    auto it = armies_.find(armyId);
    if (it != armies_.end()) {
        CampaignTile* tile = GetTile(it->second->position);
        if (tile) {
            tile->hasArmy = false;
            tile->armyFaction = -1;
        }
        armies_.erase(it);
    }
}

CampaignArmy* CampaignLayer::GetArmy(int armyId) {
    auto it = armies_.find(armyId);
    if (it != armies_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<CampaignArmy*> CampaignLayer::GetArmiesAt(Vector2D position) {
    std::vector<CampaignArmy*> result;
    for (auto& [id, army] : armies_) {
        if (static_cast<int>(army->position.x) == static_cast<int>(position.x) &&
            static_cast<int>(army->position.y) == static_cast<int>(position.y)) {
            result.push_back(army.get());
        }
    }
    return result;
}

bool CampaignLayer::MoveArmy(int armyId, Vector2D destination) {
    CampaignArmy* army = GetArmy(armyId);
    if (!army || army->movementPoints <= 0) {
        return false;
    }
    
    // 清除旧位置标记
    CampaignTile* oldTile = GetTile(army->position);
    if (oldTile) {
        oldTile->hasArmy = false;
        oldTile->armyFaction = -1;
    }
    
    // 设置新目标
    army->targetPosition = destination;
    army->isMoving = true;
    army->movementPoints--;
    
    return true;
}

bool CampaignLayer::CanAttack(int attackerArmyId, int targetArmyId) {
    CampaignArmy* attacker = GetArmy(attackerArmyId);
    CampaignArmy* target = GetArmy(targetArmyId);
    
    if (!attacker || !target) {
        return false;
    }
    
    // 检查距离
    float distance = attacker->position.Distance(target->position);
    return distance <= 1.5f && attacker->factionId != target->factionId;
}

void CampaignLayer::StartBattle(int attackerArmyId, int defenderArmyId) {
    CampaignArmy* attacker = GetArmy(attackerArmyId);
    CampaignArmy* defender = GetArmy(defenderArmyId);
    
    if (attacker && defender) {
        engine_->EnterBattle(attacker->factionId, defender->factionId, defender->position);
    }
}

void CampaignLayer::ProcessBattleResult(bool attackerWon) {
    // 处理战斗结果对战役地图的影响
    std::cout << "[CampaignLayer] Battle result processed: " << (attackerWon ? "Attacker won" : "Defender won") << std::endl;
}

int CampaignLayer::CreateSettlement(Vector2D position, int factionId, const std::string& name) {
    int id = nextSettlementId_++;
    settlements_[id] = std::make_unique<Settlement>(id, position, factionId, name);
    
    CampaignTile* tile = GetTile(position);
    if (tile) {
        tile->settlementId = id;
        tile->ownerFaction = factionId;
    }
    
    return id;
}

Settlement* CampaignLayer::GetSettlement(int id) {
    auto it = settlements_.find(id);
    if (it != settlements_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Settlement*> CampaignLayer::GetSettlementsByFaction(int factionId) {
    std::vector<Settlement*> result;
    for (auto& [id, settlement] : settlements_) {
        if (settlement->GetFactionId() == factionId) {
            result.push_back(settlement.get());
        }
    }
    return result;
}

int CampaignLayer::CreateProvince(const std::string& name) {
    int id = nextProvinceId_++;
    provinces_[id] = std::make_unique<Province>(id, name);
    return id;
}

Province* CampaignLayer::GetProvince(int id) {
    auto it = provinces_.find(id);
    if (it != provinces_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void CampaignLayer::AssignSettlementToProvince(int settlementId, int provinceId) {
    Settlement* settlement = GetSettlement(settlementId);
    Province* province = GetProvince(provinceId);
    
    if (settlement && province) {
        province->AddSettlement(settlementId);
        settlement->SetProvinceId(provinceId);
    }
}

std::vector<CampaignArmy*> CampaignLayer::GetVisibleArmies(int factionId) {
    std::vector<CampaignArmy*> result;
    for (auto& [id, army] : armies_) {
        // 简化处理：同一派系或相邻格子的军队可见
        if (army->factionId == factionId) {
            result.push_back(army.get());
        }
    }
    return result;
}

void CampaignLayer::ProcessAIturns() {
    auto* factionManager = engine_->GetFactionManager();
    for (auto* faction : factionManager->GetAIFactions()) {
        // 简化的AI逻辑
        std::cout << "[CampaignLayer] Processing AI turn for faction " << faction->GetId() << std::endl;
    }
}

float CampaignLayer::GetMovementCost(TerrainType terrain) {
    switch (terrain) {
        case TerrainType::Plains: return 1.0f;
        case TerrainType::Hills: return 1.5f;
        case TerrainType::Mountains: return 3.0f;
        case TerrainType::Forest: return 2.0f;
        case TerrainType::Desert: return 2.0f;
        case TerrainType::Swamp: return 3.0f;
        case TerrainType::Coast: return 1.0f;
        default: return 1.0f;
    }
}

struct AStarNode {
    Vector2D position;
    float gCost;
    float hCost;
    float fCost() const { return gCost + hCost; }
    Vector2D parent;
};

std::vector<Vector2D> CampaignLayer::FindPath(Vector2D start, Vector2D end) {
    // A* 路径查找简化实现
    std::vector<Vector2D> path;
    path.push_back(start);
    
    // 简化的直线路径
    Vector2D current = start;
    while (current.Distance(end) > 1.0f) {
        Vector2D direction = end - current;
        float dist = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (dist > 0) {
            direction.x /= dist;
            direction.y /= dist;
        }
        
        current.x += direction.x;
        current.y += direction.y;
        path.push_back(current);
    }
    
    path.push_back(end);
    return path;
}

} // namespace MingGoRTS
