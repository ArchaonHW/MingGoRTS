#include "BattleLayer.h"
#include "Unit.h"
#include "Formation.h"
#include "../Engine/GameEngine.h"
#include <iostream>
#include <cmath>

namespace MingGoRTS {

BattleLayer::BattleLayer(GameEngine* engine)
    : engine_(engine)
    , state_(BattleState::Setup)
    , result_(BattleResult::Ongoing)
    , attackerFactionId_(-1)
    , defenderFactionId_(-1)
    , elapsedTime_(0.0f)
    , battleTimeLimit_(1800.0f)  // 30分钟
    , battlefieldWidth_(100)
    , battlefieldHeight_(100)
    , nextUnitId_(1)
    , currentWeather_("Clear")
    , timeOfDay_(12.0f) {
}

BattleLayer::~BattleLayer() {
    Shutdown();
}

void BattleLayer::InitializeBattle(int attackerFaction, int defenderFaction, Vector2D location) {
    attackerFactionId_ = attackerFaction;
    defenderFactionId_ = defenderFaction;
    battleLocation_ = location;
    
    InitializeBattlefield();
    
    state_ = BattleState::Deployment;
    result_ = BattleResult::Ongoing;
    elapsedTime_ = 0.0f;
    
    factionCasualties_[attackerFaction] = 0;
    factionCasualties_[defenderFaction] = 0;
    
    std::cout << "[BattleLayer] Battle initialized: Faction " << attackerFaction 
              << " vs Faction " << defenderFaction << std::endl;
}

void BattleLayer::Shutdown() {
    units_.clear();
    battlefield_.clear();
    state_ = BattleState::Ended;
}

void BattleLayer::InitializeBattlefield() {
    battlefield_.resize(battlefieldHeight_);
    for (int y = 0; y < battlefieldHeight_; ++y) {
        battlefield_[y].resize(battlefieldWidth_);
        for (int x = 0; x < battlefieldWidth_; ++x) {
            battlefield_[y][x].position = Vector2D(static_cast<float>(x), static_cast<float>(y));
            
            // 简单的地形生成
            float noise = std::sin(x * 0.1f) * std::cos(y * 0.1f);
            if (noise > 0.7f) {
                battlefield_[y][x].terrain = BattleTerrainType::Elevated;
                battlefield_[y][x].height = noise * 10.0f;
                battlefield_[y][x].defenseModifier = 0.2f;
            } else if (noise < -0.7f) {
                battlefield_[y][x].terrain = BattleTerrainType::Water;
                battlefield_[y][x].movementModifier = 0.0f;
            } else if (std::abs(noise) < 0.2f) {
                battlefield_[y][x].terrain = BattleTerrainType::Rough;
                battlefield_[y][x].movementModifier = 0.7f;
            } else {
                battlefield_[y][x].terrain = BattleTerrainType::Clear;
            }
        }
    }
}

void BattleLayer::Update(float deltaTime) {
    if (state_ != BattleState::Active) return;
    
    elapsedTime_ += deltaTime;
    
    // 检查时间限制
    if (elapsedTime_ >= battleTimeLimit_) {
        // 时间到，判定防守方胜利
        EndBattle(BattleResult::DefenderVictory);
        return;
    }
    
    // 更新所有单位
    for (auto& [id, unit] : units_) {
        unit->Update(deltaTime);
    }
    
    // 更新格子占用状态
    UpdateTileOccupancy();
    
    // 检查胜利条件
    CheckVictoryConditions();
}

void BattleLayer::FixedUpdate(float fixedDeltaTime) {
    if (state_ != BattleState::Active) return;
    
    // 处理AI逻辑
    ProcessUnitAI(fixedDeltaTime);
    
    // 处理战斗
    ProcessCombat();
    
    // 处理士气
    ProcessMorale();
}

void BattleLayer::StartBattle() {
    state_ = BattleState::Active;
    std::cout << "[BattleLayer] Battle started!" << std::endl;
}

void BattleLayer::PauseBattle() {
    if (state_ == BattleState::Active) {
        state_ = BattleState::Paused;
    }
}

void BattleLayer::ResumeBattle() {
    if (state_ == BattleState::Paused) {
        state_ = BattleState::Active;
    }
}

void BattleLayer::EndBattle(BattleResult result) {
    result_ = result;
    state_ = BattleState::Ended;
    
    std::cout << "[BattleLayer] Battle ended: ";
    switch (result) {
        case BattleResult::AttackerVictory:
            std::cout << "Attacker Victory" << std::endl;
            break;
        case BattleResult::DefenderVictory:
            std::cout << "Defender Victory" << std::endl;
            break;
        case BattleResult::Draw:
            std::cout << "Draw" << std::endl;
            break;
        case BattleResult::Retreat:
            std::cout << "Retreat" << std::endl;
            break;
        default:
            break;
    }
}

int BattleLayer::SpawnUnit(int unitTypeId, int factionId, Vector2D position) {
    int id = nextUnitId_++;
    units_[id] = std::make_unique<Unit>(id, unitTypeId, factionId, position);
    
    // 设置初始朝向（攻击方朝右，防守方朝左）
    if (factionId == attackerFactionId_) {
        units_[id]->SetRotation(90.0f);
    } else {
        units_[id]->SetRotation(270.0f);
    }
    
    return id;
}

void BattleLayer::DestroyUnit(int unitId) {
    auto it = units_.find(unitId);
    if (it != units_.end()) {
        // 记录伤亡
        int factionId = it->second->GetFactionId();
        factionCasualties_[factionId]++;
        
        // 清除格子占用
        BattleTile* tile = GetTile(it->second->GetPosition());
        if (tile) {
            tile->isOccupied = false;
            tile->unitId = -1;
        }
        
        units_.erase(it);
    }
}

Unit* BattleLayer::GetUnit(int unitId) {
    auto it = units_.find(unitId);
    if (it != units_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::vector<Unit*> BattleLayer::GetUnitsByFaction(int factionId) {
    std::vector<Unit*> result;
    for (auto& [id, unit] : units_) {
        if (unit->GetFactionId() == factionId) {
            result.push_back(unit.get());
        }
    }
    return result;
}

std::vector<Unit*> BattleLayer::GetUnitsInArea(Vector2D center, float radius) {
    std::vector<Unit*> result;
    for (auto& [id, unit] : units_) {
        if (unit->GetPosition().Distance(center) <= radius) {
            result.push_back(unit.get());
        }
    }
    return result;
}

void BattleLayer::MoveUnit(int unitId, Vector2D destination) {
    Unit* unit = GetUnit(unitId);
    if (unit && unit->IsAlive()) {
        unit->MoveTo(destination);
    }
}

void BattleLayer::AttackUnit(int attackerId, int targetId) {
    Unit* attacker = GetUnit(attackerId);
    Unit* target = GetUnit(targetId);
    
    if (attacker && target && attacker->IsAlive() && target->IsAlive()) {
        if (attacker->GetFactionId() != target->GetFactionId()) {
            attacker->Attack(target);
        }
    }
}

void BattleLayer::ChangeFormation(int unitId, int formationId) {
    Unit* unit = GetUnit(unitId);
    if (unit) {
        unit->SetFormation(formationId);
    }
}

void BattleLayer::RotateUnit(int unitId, float angle) {
    Unit* unit = GetUnit(unitId);
    if (unit) {
        unit->SetRotation(angle);
    }
}

void BattleLayer::RetreatUnit(int unitId) {
    Unit* unit = GetUnit(unitId);
    if (unit) {
        unit->Retreat();
    }
}

BattleTile* BattleLayer::GetTile(int x, int y) {
    if (x >= 0 && x < battlefieldWidth_ && y >= 0 && y < battlefieldHeight_) {
        return &battlefield_[y][x];
    }
    return nullptr;
}

BattleTile* BattleLayer::GetTile(const Vector2D& pos) {
    return GetTile(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

float BattleLayer::GetDistance(Vector2D a, Vector2D b) {
    return a.Distance(b);
}

bool BattleLayer::IsLineOfSightClear(Vector2D start, Vector2D end) {
    // 简化的视线检测
    float dist = start.Distance(end);
    Vector2D direction = end - start;
    direction.x /= dist;
    direction.y /= dist;
    
    Vector2D current = start;
    float step = 1.0f;
    
    while (current.Distance(start) < dist) {
        current.x += direction.x * step;
        current.y += direction.y * step;
        
        BattleTile* tile = GetTile(current);
        if (tile && tile->terrain == BattleTerrainType::Obstacle) {
            return false;
        }
    }
    
    return true;
}

void BattleLayer::CheckVictoryConditions() {
    bool attackerDefeated = IsFactionDefeated(attackerFactionId_);
    bool defenderDefeated = IsFactionDefeated(defenderFactionId_);
    
    if (attackerDefeated && defenderDefeated) {
        EndBattle(BattleResult::Draw);
    } else if (attackerDefeated) {
        EndBattle(BattleResult::DefenderVictory);
    } else if (defenderDefeated) {
        EndBattle(BattleResult::AttackerVictory);
    }
}

bool BattleLayer::IsFactionDefeated(int factionId) {
    auto units = GetUnitsByFaction(factionId);
    
    // 检查是否还有战斗能力
    for (auto* unit : units) {
        if (unit->IsAlive() && unit->CanFight()) {
            return false;
        }
    }
    
    return true;
}

int BattleLayer::GetUnitCount(int factionId) {
    return static_cast<int>(GetUnitsByFaction(factionId).size());
}

int BattleLayer::GetCasualties(int factionId) {
    auto it = factionCasualties_.find(factionId);
    if (it != factionCasualties_.end()) {
        return it->second;
    }
    return 0;
}

void BattleLayer::SetWeather(const std::string& weather) {
    currentWeather_ = weather;
    
    // 天气影响
    if (weather == "Rain") {
        // 雨天降低移动速度和士气
        for (auto& [id, unit] : units_) {
            unit->ApplyWeatherPenalty(0.8f);
        }
    }
}

void BattleLayer::SetTimeOfDay(float time) {
    timeOfDay_ = time;
    
    // 夜晚影响视线
    if (time < 6.0f || time > 18.0f) {
        for (auto& [id, unit] : units_) {
            unit->SetNightVision(true);
        }
    }
}

void BattleLayer::ProcessUnitAI(float deltaTime) {
    // 简化的AI处理
    for (auto& [id, unit] : units_) {
        if (unit->IsAlive() && !unit->IsPlayerControlled()) {
            // AI决策逻辑
            auto nearbyEnemies = GetUnitsInArea(unit->GetPosition(), 50.0f);
            
            for (auto* enemy : nearbyEnemies) {
                if (enemy->GetFactionId() != unit->GetFactionId() && enemy->IsAlive()) {
                    float dist = unit->GetPosition().Distance(enemy->GetPosition());
                    
                    if (dist <= unit->GetAttackRange()) {
                        // 在攻击范围内，攻击
                        unit->Attack(enemy);
                    } else if (dist <= unit->GetVisionRange()) {
                        // 在视野内，移动接近
                        unit->MoveTo(enemy->GetPosition());
                    }
                    
                    break;  // 只处理第一个发现的敌人
                }
            }
        }
    }
}

void BattleLayer::ProcessCombat() {
    // 处理所有单位的战斗
    for (auto& [id, unit] : units_) {
        if (unit->IsAlive()) {
            unit->ProcessCombat();
        }
    }
}

void BattleLayer::ProcessMorale() {
    // 处理士气系统
    for (auto& [id, unit] : units_) {
        if (unit->IsAlive()) {
            // 检查友军和敌军数量
            auto nearbyFriends = GetUnitsInArea(unit->GetPosition(), 30.0f);
            auto nearbyEnemies = GetUnitsInArea(unit->GetPosition(), 30.0f);
            
            int friendCount = 0;
            int enemyCount = 0;
            
            for (auto* u : nearbyFriends) {
                if (u->GetFactionId() == unit->GetFactionId() && u->IsAlive()) {
                    friendCount++;
                } else if (u->GetFactionId() != unit->GetFactionId() && u->IsAlive()) {
                    enemyCount++;
                }
            }
            
            unit->UpdateMorale(friendCount, enemyCount);
            
            // 士气过低单位溃逃
            if (unit->GetMorale() <= 0.0f) {
                unit->Retreat();
            }
        }
    }
}

void BattleLayer::UpdateTileOccupancy() {
    // 清空所有格子
    for (int y = 0; y < battlefieldHeight_; ++y) {
        for (int x = 0; x < battlefieldWidth_; ++x) {
            battlefield_[y][x].isOccupied = false;
            battlefield_[y][x].unitId = -1;
        }
    }
    
    // 重新标记
    for (auto& [id, unit] : units_) {
        if (unit->IsAlive()) {
            Vector2D pos = unit->GetPosition();
            BattleTile* tile = GetTile(pos);
            if (tile) {
                tile->isOccupied = true;
                tile->unitId = id;
            }
        }
    }
}

std::vector<Vector2D> BattleLayer::FindBattlePath(Vector2D start, Vector2D end, int unitId) {
    // 简化的路径查找
    std::vector<Vector2D> path;
    path.push_back(start);
    path.push_back(end);
    return path;
}

bool BattleLayer::IsTileWalkable(int x, int y, int unitId) {
    if (x < 0 || x >= battlefieldWidth_ || y < 0 || y >= battlefieldHeight_) {
        return false;
    }
    
    BattleTile* tile = GetTile(x, y);
    if (!tile) return false;
    
    // 水域不可行走
    if (tile->terrain == BattleTerrainType::Water) {
        return false;
    }
    
    // 检查是否被其他单位占用
    if (tile->isOccupied && tile->unitId != unitId) {
        return false;
    }
    
    return true;
}

} // namespace MingGoRTS
