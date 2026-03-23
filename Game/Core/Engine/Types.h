#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace MingGoRTS {

// 游戏层类型枚举
enum class GameLayer {
    None,
    Campaign,    // 战役层 - 回合制策略
    Battle       // 战斗层 - 即时战术
};

// 游戏状态
enum class GameState {
    Menu,
    Campaign,
    Battle,
    Loading,
    Paused
};

// 回合阶段
enum class TurnPhase {
    Start,
    PlayerTurn,
    AIProcessing,
    End
};

// 基础事件结构
struct GameEvent {
    std::string type;
    std::string data;
    float timestamp;
};

// 事件监听器类型
using EventCallback = std::function<void(const GameEvent&)>;

class IGameEventListener {
public:
    virtual ~IGameEventListener() = default;
    virtual void OnEvent(const GameEvent& event) = 0;
};

// 向量/位置结构
struct Vector2D {
    float x;
    float y;
    
    Vector2D(float x = 0, float y = 0) : x(x), y(y) {}
    
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    float Distance(const Vector2D& other) const {
        float dx = x - other.x;
        float dy = y - other.y;
        return std::sqrt(dx * dx + dy * dy);
    }
};

// 游戏配置
struct GameConfig {
    int maxTurns = 500;
    int campaignMapWidth = 256;
    int campaignMapHeight = 256;
    int battleMapWidth = 1024;
    int battleMapHeight = 1024;
    float battleSpeed = 1.0f;
    bool autoSave = true;
    std::string savePath = "Saves/";
};

} // namespace MingGoRTS
