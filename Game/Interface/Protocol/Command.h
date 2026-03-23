#pragma once

#include <string>
#include <vector>
#include <map>

namespace MingGoRTS {

// 命令类型 - 前端向后端发送的指令
enum class CommandType {
    // 游戏控制
    StartGame,
    LoadGame,
    SaveGame,
    ExitGame,
    Pause,
    Resume,
    
    // 战役层
    SelectArmy,
    MoveArmy,
    AttackTarget,
    SiegeSettlement,
    EnterSettlement,
    RecruitUnits,
    ConstructBuilding,
    
    // 战斗层
    SelectUnit,
    MoveUnit,
    AttackUnit,
    RotateUnit,
    ChangeFormation,
    Retreat,
    TogglePause,
    
    // 派系
    OpenDiplomacy,
    DeclareWar,
    ProposePeace,
    ProposeAlliance,
    OpenTrade,
    
    // 回合
    EndTurn,
    NextPhase,
    
    // UI
    OpenMenu,
    CloseMenu,
    ShowHelp,
    ToggleOverlay,
    
    // 系统
    Unknown
};

// 命令参数
struct CommandParameter {
    std::string name;
    std::string value;
    
    CommandParameter() = default;
    CommandParameter(const std::string& n, const std::string& v) 
        : name(n), value(v) {}
};

// 命令 - 前端向后端发送的指令封装
class Command {
public:
    Command();
    explicit Command(CommandType type);
    ~Command();
    
    // 基础属性
    CommandType GetType() const { return type_; }
    void SetType(CommandType type) { type_ = type; }
    
    // 参数
    void AddParameter(const std::string& name, const std::string& value);
    void AddParameter(const std::string& name, int value);
    void AddParameter(const std::string& name, float value);
    void AddParameter(const std::string& name, bool value);
    
    std::string GetParameter(const std::string& name) const;
    int GetIntParameter(const std::string& name, int defaultValue = 0) const;
    float GetFloatParameter(const std::string& name, float defaultValue = 0.0f) const;
    bool GetBoolParameter(const std::string& name, bool defaultValue = false) const;
    bool HasParameter(const std::string& name) const;
    
    const std::vector<CommandParameter>& GetParameters() const { return parameters_; }
    
    // 序列化
    std::string Serialize() const;
    bool Deserialize(const std::string& data);
    
    // 辅助方法
    static CommandType ParseCommandType(const std::string& typeStr);
    static std::string CommandTypeToString(CommandType type);

private:
    CommandType type_;
    std::vector<CommandParameter> parameters_;
};

} // namespace MingGoRTS
