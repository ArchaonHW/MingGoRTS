#pragma once

#include "../../Core/Engine/Types.h"
#include <vector>
#include <map>
#include <functional>
#include <string>

namespace MingGoRTS {

// 事件优先级
enum class EventPriority {
    Low,
    Normal,
    High,
    Critical
};

// 具体事件类型
enum class EventType {
    // 游戏状态
    GameStarted,
    GameEnded,
    StateChanged,
    
    // 回合
    TurnStarted,
    TurnEnded,
    PhaseChanged,
    
    // 战役
    ArmyCreated,
    ArmyMoved,
    ArmyDestroyed,
    SettlementCreated,
    SettlementCaptured,
    SettlementUpgraded,
    
    // 战斗
    BattleStarted,
    BattleEnded,
    UnitSpawned,
    UnitMoved,
    UnitAttacked,
    UnitDestroyed,
    UnitRetreated,
    
    // 派系
    FactionCreated,
    FactionDestroyed,
    WarDeclared,
    PeaceMade,
    AllianceFormed,
    MoneyChanged,
    
    // 输入
    InputCommand,
    UIAction,
    
    // 系统
    Error,
    Warning,
    Info
};

// 事件结构
struct Event {
    EventType type;
    EventPriority priority;
    float timestamp;
    int sourceId;  // 事件源（如派系ID）
    std::string data;  // JSON格式数据
    
    Event() : type(EventType::Info), priority(EventPriority::Normal), 
              timestamp(0.0f), sourceId(-1) {}
};

// 事件监听器接口
class IEventListener {
public:
    virtual ~IEventListener() = default;
    virtual void OnEvent(const Event& event) = 0;
    virtual bool ShouldReceiveEvent(EventType type) const { return true; }
};

// 事件系统 - Backend 与 Frontend 通信的桥梁
class EventSystem {
public:
    EventSystem();
    ~EventSystem();
    
    // 初始化
    void Initialize();
    void Shutdown();
    
    // 事件发布
    void Publish(const Event& event);
    void Publish(EventType type, const std::string& data, int sourceId = -1);
    void PublishImmediate(const Event& event);  // 立即处理，不排队
    
    // 事件订阅
    void Subscribe(IEventListener* listener);
    void Unsubscribe(IEventListener* listener);
    void SubscribeToType(EventType type, IEventListener* listener);
    void UnsubscribeFromType(EventType type, IEventListener* listener);
    
    // 回调订阅（函数对象）
    using EventCallback = std::function<void(const Event&)>;
    void SubscribeCallback(EventType type, EventCallback callback);
    void UnsubscribeCallback(EventType type);
    
    // 更新（处理事件队列）
    void Update(float deltaTime);
    
    // 队列管理
    void ClearQueue();
    int GetQueueSize() const;
    
    // 事件历史
    std::vector<Event> GetRecentEvents(int count) const;
    std::vector<Event> GetEventsByType(EventType type, int count) const;
    std::vector<Event> GetEventsBySource(int sourceId, int count) const;

private:
    std::vector<Event> eventQueue_;
    std::vector<Event> eventHistory_;
    std::vector<IEventListener*> listeners_;
    std::map<EventType, std::vector<IEventListener*>> typedListeners_;
    std::map<EventType, EventCallback> callbacks_;
    
    static const int MAX_HISTORY_SIZE = 1000;
    
    void ProcessEvent(const Event& event);
    void AddToHistory(const Event& event);
    void TrimHistory();
};

} // namespace MingGoRTS
