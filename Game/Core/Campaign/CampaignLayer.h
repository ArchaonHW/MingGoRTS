#pragma once

#include "../Engine/Types.h"
#include <vector>
#include <memory>
#include <map>

namespace MingGoRTS {

// 前向声明
class GameEngine;
class Faction;
class Settlement;
class Province;

// 战役地图格子类型
enum class TerrainType {
    Plains,      // 平原
    Hills,       // 丘陵
    Mountains,   // 山地
    Forest,      // 森林
    Desert,      // 沙漠
    Swamp,       // 沼泽
    Water,       // 水域
    Coast        // 海岸
};

// 战役地图格子
struct CampaignTile {
    Vector2D position;
    TerrainType terrain;
    int ownerFaction;  // -1 表示无主
    int settlementId;  // -1 表示无定居点
    bool hasArmy;
    int armyFaction;
    
    CampaignTile() : terrain(TerrainType::Plains), ownerFaction(-1), 
                     settlementId(-1), hasArmy(false), armyFaction(-1) {}
};

// 军队在战役地图上的表示
struct CampaignArmy {
    int id;
    int factionId;
    Vector2D position;
    int unitCount;
    int movementPoints;
    int maxMovementPoints;
    bool isMoving;
    Vector2D targetPosition;
    
    CampaignArmy(int id, int faction, Vector2D pos) 
        : id(id), factionId(faction), position(pos), unitCount(0),
          movementPoints(10), maxMovementPoints(10), isMoving(false) {}
};

// 战役层 - 回合制策略层 (Total War Grand Campaign 模式)
class CampaignLayer {
public:
    explicit CampaignLayer(GameEngine* engine);
    ~CampaignLayer();
    
    // 初始化
    bool Initialize(int width, int height);
    void Shutdown();
    
    // 更新
    void Update(float deltaTime);
    void ProcessEndOfTurn();
    
    // 地图操作
    CampaignTile* GetTile(int x, int y);
    CampaignTile* GetTile(const Vector2D& pos);
    std::vector<Vector2D> GetPath(const Vector2D& start, const Vector2D& end);
    
    // 军队操作
    int CreateArmy(int factionId, Vector2D position);
    void DestroyArmy(int armyId);
    CampaignArmy* GetArmy(int armyId);
    std::vector<CampaignArmy*> GetArmiesAt(Vector2D position);
    bool MoveArmy(int armyId, Vector2D destination);
    
    // 战斗触发
    bool CanAttack(int attackerArmyId, int targetArmyId);
    void StartBattle(int attackerArmyId, int defenderArmyId);
    void ProcessBattleResult(bool attackerWon);
    
    // 定居点
    int CreateSettlement(Vector2D position, int factionId, const std::string& name);
    Settlement* GetSettlement(int id);
    std::vector<Settlement*> GetSettlementsByFaction(int factionId);
    
    // 行省
    int CreateProvince(const std::string& name);
    Province* GetProvince(int id);
    void AssignSettlementToProvince(int settlementId, int provinceId);
    
    // 查询
    int GetWidth() const { return mapWidth_; }
    int GetHeight() const { return mapHeight_; }
    std::vector<CampaignArmy*> GetVisibleArmies(int factionId);
    
private:
    GameEngine* engine_;
    int mapWidth_;
    int mapHeight_;
    
    std::vector<std::vector<CampaignTile>> map_;
    std::map<int, std::unique_ptr<CampaignArmy>> armies_;
    std::map<int, std::unique_ptr<Settlement>> settlements_;
    std::map<int, std::unique_ptr<Province>> provinces_;
    
    int nextArmyId_;
    int nextSettlementId_;
    int nextProvinceId_;
    
    // AI处理
    void ProcessAIturns();
    
    // 路径查找 (A*算法)
    float GetMovementCost(TerrainType terrain);
    std::vector<Vector2D> FindPath(Vector2D start, Vector2D end);
};

} // namespace MingGoRTS
