#pragma once

#include "../Engine/Types.h"
#include <vector>
#include <memory>
#include <map>

namespace MingGoRTS {

// 前向声明
class GameEngine;
class Unit;
class Formation;

// 战斗状态
enum class BattleState {
    Setup,       // 准备阶段
    Deployment,  // 部署阶段
    Active,      // 进行中
    Paused,      // 暂停
    Ended        // 结束
};

// 战斗结果
enum class BattleResult {
    Ongoing,
    AttackerVictory,
    DefenderVictory,
    Draw,
    Retreat
};

// 战场格子类型
enum class BattleTerrainType {
    Clear,       // 平地
    Rough,       // 崎岖
    Elevated,    // 高地
    Obstacle,    // 障碍
    Water        // 水域
};

// 战斗战场格子
struct BattleTile {
    Vector2D position;
    BattleTerrainType terrain;
    float height;
    bool isOccupied;
    int unitId;  // -1 表示无单位
    float movementModifier;
    float defenseModifier;
    
    BattleTile() : terrain(BattleTerrainType::Clear), height(0.0f), 
                   isOccupied(false), unitId(-1), 
                   movementModifier(1.0f), defenseModifier(0.0f) {}
};

// 战斗层 - 即时战术层 (Total War Battle Mode)
class BattleLayer {
public:
    explicit BattleLayer(GameEngine* engine);
    ~BattleLayer();
    
    // 生命周期
    void InitializeBattle(int attackerFaction, int defenderFaction, Vector2D location);
    void Shutdown();
    
    // 更新
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    
    // 战斗控制
    void StartBattle();
    void PauseBattle();
    void ResumeBattle();
    void EndBattle(BattleResult result);
    
    // 单位管理
    int SpawnUnit(int unitTypeId, int factionId, Vector2D position);
    void DestroyUnit(int unitId);
    Unit* GetUnit(int unitId);
    std::vector<Unit*> GetUnitsByFaction(int factionId);
    std::vector<Unit*> GetUnitsInArea(Vector2D center, float radius);
    
    // 单位命令
    void MoveUnit(int unitId, Vector2D destination);
    void AttackUnit(int attackerId, int targetId);
    void ChangeFormation(int unitId, int formationId);
    void RotateUnit(int unitId, float angle);
    void RetreatUnit(int unitId);
    
    // 战场查询
    BattleTile* GetTile(int x, int y);
    BattleTile* GetTile(const Vector2D& pos);
    float GetDistance(Vector2D a, Vector2D b);
    bool IsLineOfSightClear(Vector2D start, Vector2D end);
    
    // 战斗信息
    BattleState GetState() const { return state_; }
    BattleResult GetResult() const { return result_; }
    int GetAttackerFaction() const { return attackerFactionId_; }
    int GetDefenderFaction() const { return defenderFactionId_; }
    float GetElapsedTime() const { return elapsedTime_; }
    
    // 胜负判定
    void CheckVictoryConditions();
    bool IsFactionDefeated(int factionId);
    int GetUnitCount(int factionId);
    int GetCasualties(int factionId);
    
    // 天气与环境
    void SetWeather(const std::string& weather);
    void SetTimeOfDay(float time);  // 0-24小时
    
private:
    GameEngine* engine_;
    BattleState state_;
    BattleResult result_;
    
    int attackerFactionId_;
    int defenderFactionId_;
    Vector2D battleLocation_;
    
    float elapsedTime_;
    float battleTimeLimit_;  // 战斗时间限制
    
    int battlefieldWidth_;
    int battlefieldHeight_;
    std::vector<std::vector<BattleTile>> battlefield_;
    
    std::map<int, std::unique_ptr<Unit>> units_;
    int nextUnitId_;
    
    std::map<int, int> factionCasualties_;
    std::string currentWeather_;
    float timeOfDay_;
    
    // 内部方法
    void InitializeBattlefield();
    void ProcessUnitAI(float deltaTime);
    void ProcessCombat();
    void ProcessMorale();
    void UpdateTileOccupancy();
    
    // 路径查找
    std::vector<Vector2D> FindBattlePath(Vector2D start, Vector2D end, int unitId);
    bool IsTileWalkable(int x, int y, int unitId);
};

} // namespace MingGoRTS
