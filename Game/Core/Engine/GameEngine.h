#pragma once

#include "Types.h"
#include <memory>
#include <vector>

namespace MingGoRTS {

// 前向声明
class CampaignLayer;
class BattleLayer;
class FactionManager;
class EventSystem;

// 游戏引擎 - 核心控制器
class GameEngine {
public:
    GameEngine();
    ~GameEngine();

    // 生命周期
    bool Initialize(const GameConfig& config);
    void Shutdown();
    
    // 主循环
    void Update(float deltaTime);
    void FixedUpdate(float fixedDeltaTime);
    
    // 状态管理
    void SetState(GameState state);
    GameState GetState() const { return currentState_; }
    
    // 层切换
    void EnterCampaign();
    void EnterBattle(int attackerFaction, int defenderFaction, Vector2D location);
    void ExitBattle(bool attackerWon);
    
    // 回合控制
    void EndTurn();
    void NextPhase();
    TurnPhase GetCurrentPhase() const { return currentPhase_; }
    int GetCurrentTurn() const { return currentTurn_; }
    
    // 获取各层
    CampaignLayer* GetCampaignLayer() const { return campaignLayer_.get(); }
    BattleLayer* GetBattleLayer() const { return battleLayer_.get(); }
    FactionManager* GetFactionManager() const { return factionManager_.get(); }
    EventSystem* GetEventSystem() const { return eventSystem_.get(); }
    
    // 配置
    const GameConfig& GetConfig() const { return config_; }
    
    // 事件监听
    void AddEventListener(IGameEventListener* listener);
    void RemoveEventListener(IGameEventListener* listener);

private:
    GameConfig config_;
    GameState currentState_;
    TurnPhase currentPhase_;
    int currentTurn_;
    
    std::unique_ptr<CampaignLayer> campaignLayer_;
    std::unique_ptr<BattleLayer> battleLayer_;
    std::unique_ptr<FactionManager> factionManager_;
    std::unique_ptr<EventSystem> eventSystem_;
    
    std::vector<IGameEventListener*> eventListeners_;
    
    void BroadcastEvent(const GameEvent& event);
    void ProcessTurnLogic();
};

} // namespace MingGoRTS
