#include "GameEngine.h"
#include "../Campaign/CampaignLayer.h"
#include "../Battle/BattleLayer.h"
#include "../Faction/FactionManager.h"
#include "../../Interface/Events/EventSystem.h"
#include <iostream>

namespace MingGoRTS {

GameEngine::GameEngine()
    : currentState_(GameState::Menu)
    , currentPhase_(TurnPhase::Start)
    , currentTurn_(1) {
}

GameEngine::~GameEngine() {
    Shutdown();
}

bool GameEngine::Initialize(const GameConfig& config) {
    config_ = config;
    
    // 初始化事件系统
    eventSystem_ = std::make_unique<EventSystem>();
    
    // 初始化派系管理器
    factionManager_ = std::make_unique<FactionManager>();
    factionManager_->Initialize();
    
    // 初始化战役层
    campaignLayer_ = std::make_unique<CampaignLayer>(this);
    if (!campaignLayer_->Initialize(config.campaignMapWidth, config.campaignMapHeight)) {
        return false;
    }
    
    // 初始化战斗层（延迟初始化，进入战斗时才完全准备）
    battleLayer_ = std::make_unique<BattleLayer>(this);
    
    std::cout << "[GameEngine] Initialized successfully" << std::endl;
    return true;
}

void GameEngine::Shutdown() {
    if (battleLayer_) {
        battleLayer_->Shutdown();
        battleLayer_.reset();
    }
    
    if (campaignLayer_) {
        campaignLayer_->Shutdown();
        campaignLayer_.reset();
    }
    
    if (factionManager_) {
        factionManager_->Shutdown();
        factionManager_.reset();
    }
    
    eventListeners_.clear();
    
    std::cout << "[GameEngine] Shutdown complete" << std::endl;
}

void GameEngine::Update(float deltaTime) {
    switch (currentState_) {
        case GameState::Campaign:
            if (campaignLayer_) {
                campaignLayer_->Update(deltaTime);
            }
            break;
            
        case GameState::Battle:
            if (battleLayer_) {
                battleLayer_->Update(deltaTime);
            }
            break;
            
        default:
            break;
    }
}

void GameEngine::FixedUpdate(float fixedDeltaTime) {
    if (currentState_ == GameState::Battle && battleLayer_) {
        battleLayer_->FixedUpdate(fixedDeltaTime);
    }
}

void GameEngine::SetState(GameState state) {
    GameState oldState = currentState_;
    currentState_ = state;
    
    // 广播状态变化事件
    GameEvent event;
    event.type = "StateChanged";
    event.data = std::to_string(static_cast<int>(oldState)) + "->" + 
                 std::to_string(static_cast<int>(state));
    BroadcastEvent(event);
}

void GameEngine::EnterCampaign() {
    SetState(GameState::Campaign);
    currentPhase_ = TurnPhase::Start;
    
    GameEvent event;
    event.type = "EnterCampaign";
    event.data = "Turn " + std::to_string(currentTurn_);
    BroadcastEvent(event);
}

void GameEngine::EnterBattle(int attackerFaction, int defenderFaction, Vector2D location) {
    SetState(GameState::Battle);
    
    // 准备战斗层
    if (battleLayer_) {
        battleLayer_->InitializeBattle(attackerFaction, defenderFaction, location);
    }
    
    GameEvent event;
    event.type = "EnterBattle";
    event.data = "Faction " + std::to_string(attackerFaction) + " vs " + 
                 std::to_string(defenderFaction);
    BroadcastEvent(event);
}

void GameEngine::ExitBattle(bool attackerWon) {
    // 处理战斗结果
    if (campaignLayer_) {
        campaignLayer_->ProcessBattleResult(attackerWon);
    }
    
    SetState(GameState::Campaign);
    
    GameEvent event;
    event.type = "ExitBattle";
    event.data = attackerWon ? "AttackerWon" : "DefenderWon";
    BroadcastEvent(event);
}

void GameEngine::EndTurn() {
    ProcessTurnLogic();
    currentTurn_++;
    currentPhase_ = TurnPhase::Start;
    
    GameEvent event;
    event.type = "TurnEnded";
    event.data = std::to_string(currentTurn_);
    BroadcastEvent(event);
}

void GameEngine::NextPhase() {
    switch (currentPhase_) {
        case TurnPhase::Start:
            currentPhase_ = TurnPhase::PlayerTurn;
            break;
        case TurnPhase::PlayerTurn:
            currentPhase_ = TurnPhase::AIProcessing;
            break;
        case TurnPhase::AIProcessing:
            currentPhase_ = TurnPhase::End;
            break;
        case TurnPhase::End:
            EndTurn();
            return;
    }
    
    GameEvent event;
    event.type = "PhaseChanged";
    event.data = std::to_string(static_cast<int>(currentPhase_));
    BroadcastEvent(event);
}

void GameEngine::AddEventListener(IGameEventListener* listener) {
    if (listener && std::find(eventListeners_.begin(), eventListeners_.end(), listener) 
        == eventListeners_.end()) {
        eventListeners_.push_back(listener);
    }
}

void GameEngine::RemoveEventListener(IGameEventListener* listener) {
    auto it = std::find(eventListeners_.begin(), eventListeners_.end(), listener);
    if (it != eventListeners_.end()) {
        eventListeners_.erase(it);
    }
}

void GameEngine::BroadcastEvent(const GameEvent& event) {
    for (auto* listener : eventListeners_) {
        if (listener) {
            listener->OnEvent(event);
        }
    }
}

void GameEngine::ProcessTurnLogic() {
    // 处理回合结束逻辑
    if (campaignLayer_) {
        campaignLayer_->ProcessEndOfTurn();
    }
}

} // namespace MingGoRTS
