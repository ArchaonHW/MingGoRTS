#pragma once

#include "../../Core/Engine/Types.h"
#include <string>
#include <vector>
#include <functional>

namespace MingGoRTS {

// 前向声明
class GameEngine;
class CampaignLayer;
class BattleLayer;
class Faction;
class Unit;
class Settlement;

// API 响应结构
struct APIResponse {
    bool success;
    std::string errorMessage;
    std::string data;  // JSON 格式数据
};

// API 命令类型
enum class APICommandType {
    // 游戏状态
    GetGameState,
    SetGameState,
    
    // 战役层
    GetCampaignMap,
    GetTile,
    MoveArmy,
    AttackArmy,
    CreateSettlement,
    GetSettlements,
    
    // 战斗层
    GetBattleState,
    SpawnUnit,
    MoveUnit,
    AttackUnit,
    RetreatUnit,
    
    // 派系
    GetFactions,
    GetFactionDetails,
    DeclareWar,
    MakePeace,
    
    // 回合
    EndTurn,
    NextPhase,
    
    // 经济
    GetMoney,
    BuildBuilding,
    
    // 查询
    GetArmies,
    GetUnits,
    GetProvinces
};

// 游戏 API - Backend 暴露给 Frontend 的接口
class GameAPI {
public:
    GameAPI(GameEngine* engine);
    ~GameAPI();
    
    // 初始化
    void Initialize();
    void Shutdown();
    
    // 游戏状态 API
    APIResponse GetGameState() const;
    APIResponse GetCurrentTurn() const;
    APIResponse GetCurrentPhase() const;
    
    // 战役层 API
    APIResponse GetCampaignMap(int x, int y, int width, int height) const;
    APIResponse GetTile(int x, int y) const;
    APIResponse MoveArmy(int armyId, float x, float y);
    APIResponse AttackArmy(int attackerId, int defenderId);
    APIResponse CreateArmy(int factionId, float x, float y);
    APIResponse GetArmies(int factionId) const;
    APIResponse GetVisibleArmies(int factionId) const;
    
    APIResponse GetSettlements(int factionId) const;
    APIResponse GetSettlement(int settlementId) const;
    APIResponse CreateSettlement(float x, float y, int factionId, const std::string& name);
    APIResponse UpgradeSettlement(int settlementId);
    APIResponse BuildInSettlement(int settlementId, const std::string& buildingType);
    
    // 战斗层 API
    APIResponse GetBattleState() const;
    APIResponse GetUnitsInBattle(int factionId) const;
    APIResponse SpawnUnit(int unitTypeId, int factionId, float x, float y);
    APIResponse MoveUnit(int unitId, float x, float y);
    APIResponse AttackUnit(int attackerId, int targetId);
    APIResponse RotateUnit(int unitId, float angle);
    APIResponse ChangeFormation(int unitId, int formationId);
    APIResponse RetreatUnit(int unitId);
    
    // 派系 API
    APIResponse GetAllFactions() const;
    APIResponse GetFaction(int factionId) const;
    APIResponse GetPlayerFaction() const;
    APIResponse GetFactionRelationship(int faction1, int faction2) const;
    APIResponse DeclareWar(int factionId);
    APIResponse MakePeace(int factionId);
    APIResponse FormAlliance(int factionId);
    
    // 经济 API
    APIResponse GetMoney(int factionId) const;
    APIResponse GetIncome(int factionId) const;
    APIResponse GetFactionStats(int factionId) const;
    
    // 回合控制 API
    APIResponse EndTurn();
    APIResponse NextPhase();
    
    // 高级查询
    APIResponse QuerySettlements(const std::string& filter) const;
    APIResponse QueryArmies(const std::string& filter) const;
    APIResponse QueryUnits(const std::string& filter) const;
    
    // 事件订阅
    void SubscribeToEvent(const std::string& eventType, std::function<void(const GameEvent&)> callback);
    void UnsubscribeFromEvent(const std::string& eventType);

private:
    GameEngine* engine_;
    std::map<std::string, std::vector<std::function<void(const GameEvent&)>>> eventSubscriptions_;
    
    // 辅助方法
    std::string SerializeVector2D(const Vector2D& vec) const;
    Vector2D DeserializeVector2D(const std::string& data) const;
};

} // namespace MingGoRTS
