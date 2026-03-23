#include "EventSystem.h"
#include <algorithm>
#include <iostream>

namespace MingGoRTS {

EventSystem::EventSystem() = default;

EventSystem::~EventSystem() {
    Shutdown();
}

void EventSystem::Initialize() {
    std::cout << "[EventSystem] Initialized" << std::endl;
}

void EventSystem::Shutdown() {
    listeners_.clear();
    typedListeners_.clear();
    callbacks_.clear();
    eventQueue_.clear();
    eventHistory_.clear();
}

void EventSystem::Publish(const Event& event) {
    // 添加到队列
    eventQueue_.push_back(event);
    
    // 按优先级排序
    std::sort(eventQueue_.begin(), eventQueue_.end(), 
        [](const Event& a, const Event& b) {
            return static_cast<int>(a.priority) > static_cast<int>(b.priority);
        });
}

void EventSystem::Publish(EventType type, const std::string& data, int sourceId) {
    Event event;
    event.type = type;
    event.data = data;
    event.sourceId = sourceId;
    event.priority = EventPriority::Normal;
    event.timestamp = 0.0f;  // 将在处理时设置
    
    Publish(event);
}

void EventSystem::PublishImmediate(const Event& event) {
    // 立即处理，不排队
    ProcessEvent(event);
    AddToHistory(event);
}

void EventSystem::Subscribe(IEventListener* listener) {
    if (listener && std::find(listeners_.begin(), listeners_.end(), listener) == listeners_.end()) {
        listeners_.push_back(listener);
    }
}

void EventSystem::Unsubscribe(IEventListener* listener) {
    auto it = std::find(listeners_.begin(), listeners_.end(), listener);
    if (it != listeners_.end()) {
        listeners_.erase(it);
    }
}

void EventSystem::SubscribeToType(EventType type, IEventListener* listener) {
    if (!listener) return;
    
    auto& list = typedListeners_[type];
    if (std::find(list.begin(), list.end(), listener) == list.end()) {
        list.push_back(listener);
    }
}

void EventSystem::UnsubscribeFromType(EventType type, IEventListener* listener) {
    auto it = typedListeners_.find(type);
    if (it != typedListeners_.end()) {
        auto& list = it->second;
        auto lit = std::find(list.begin(), list.end(), listener);
        if (lit != list.end()) {
            list.erase(lit);
        }
    }
}

void EventSystem::SubscribeCallback(EventType type, EventCallback callback) {
    callbacks_[type] = callback;
}

void EventSystem::UnsubscribeCallback(EventType type) {
    callbacks_.erase(type);
}

void EventSystem::Update(float deltaTime) {
    // 处理事件队列
    while (!eventQueue_.empty()) {
        Event event = eventQueue_.front();
        eventQueue_.erase(eventQueue_.begin());
        
        // 更新时间戳
        event.timestamp += deltaTime;
        
        // 处理事件
        ProcessEvent(event);
        
        // 添加到历史
        AddToHistory(event);
    }
}

void EventSystem::ProcessEvent(const Event& event) {
    // 通知通用监听器
    for (auto* listener : listeners_) {
        if (listener && listener->ShouldReceiveEvent(event.type)) {
            listener->OnEvent(event);
        }
    }
    
    // 通知类型特定监听器
    auto it = typedListeners_.find(event.type);
    if (it != typedListeners_.end()) {
        for (auto* listener : it->second) {
            if (listener) {
                listener->OnEvent(event);
            }
        }
    }
    
    // 调用回调
    auto cbIt = callbacks_.find(event.type);
    if (cbIt != callbacks_.end() && cbIt->second) {
        cbIt->second(event);
    }
}

void EventSystem::AddToHistory(const Event& event) {
    eventHistory_.push_back(event);
    TrimHistory();
}

void EventSystem::TrimHistory() {
    while (eventHistory_.size() > MAX_HISTORY_SIZE) {
        eventHistory_.erase(eventHistory_.begin());
    }
}

void EventSystem::ClearQueue() {
    eventQueue_.clear();
}

int EventSystem::GetQueueSize() const {
    return static_cast<int>(eventQueue_.size());
}

std::vector<Event> EventSystem::GetRecentEvents(int count) const {
    std::vector<Event> result;
    int start = static_cast<int>(eventHistory_.size()) - count;
    if (start < 0) start = 0;
    
    for (size_t i = start; i < eventHistory_.size(); ++i) {
        result.push_back(eventHistory_[i]);
    }
    
    return result;
}

std::vector<Event> EventSystem::GetEventsByType(EventType type, int count) const {
    std::vector<Event> result;
    
    for (auto it = eventHistory_.rbegin(); it != eventHistory_.rend() && result.size() < count; ++it) {
        if (it->type == type) {
            result.push_back(*it);
        }
    }
    
    return result;
}

std::vector<Event> EventSystem::GetEventsBySource(int sourceId, int count) const {
    std::vector<Event> result;
    
    for (auto it = eventHistory_.rbegin(); it != eventHistory_.rend() && result.size() < count; ++it) {
        if (it->sourceId == sourceId) {
            result.push_back(*it);
        }
    }
    
    return result;
}

} // namespace MingGoRTS
