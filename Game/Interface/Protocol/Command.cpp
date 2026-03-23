#include "Command.h"
#include <sstream>
#include <algorithm>

namespace MingGoRTS {

Command::Command() : type_(CommandType::Unknown) {
}

Command::Command(CommandType type) : type_(type) {
}

Command::~Command() = default;

void Command::AddParameter(const std::string& name, const std::string& value) {
    // 检查是否已存在
    for (auto& param : parameters_) {
        if (param.name == name) {
            param.value = value;
            return;
        }
    }
    parameters_.push_back(CommandParameter(name, value));
}

void Command::AddParameter(const std::string& name, int value) {
    AddParameter(name, std::to_string(value));
}

void Command::AddParameter(const std::string& name, float value) {
    AddParameter(name, std::to_string(value));
}

void Command::AddParameter(const std::string& name, bool value) {
    AddParameter(name, value ? "true" : "false");
}

std::string Command::GetParameter(const std::string& name) const {
    for (const auto& param : parameters_) {
        if (param.name == name) {
            return param.value;
        }
    }
    return "";
}

int Command::GetIntParameter(const std::string& name, int defaultValue) const {
    std::string value = GetParameter(name);
    if (value.empty()) return defaultValue;
    try {
        return std::stoi(value);
    } catch (...) {
        return defaultValue;
    }
}

float Command::GetFloatParameter(const std::string& name, float defaultValue) const {
    std::string value = GetParameter(name);
    if (value.empty()) return defaultValue;
    try {
        return std::stof(value);
    } catch (...) {
        return defaultValue;
    }
}

bool Command::GetBoolParameter(const std::string& name, bool defaultValue) const {
    std::string value = GetParameter(name);
    if (value.empty()) return defaultValue;
    return value == "true" || value == "1" || value == "yes";
}

bool Command::HasParameter(const std::string& name) const {
    for (const auto& param : parameters_) {
        if (param.name == name) {
            return true;
        }
    }
    return false;
}

std::string Command::Serialize() const {
    std::stringstream ss;
    ss << CommandTypeToString(type_) << "|";
    
    for (size_t i = 0; i < parameters_.size(); ++i) {
        if (i > 0) ss << "&";
        ss << parameters_[i].name << "=" << parameters_[i].value;
    }
    
    return ss.str();
}

bool Command::Deserialize(const std::string& data) {
    size_t pos = data.find('|');
    if (pos == std::string::npos) {
        type_ = ParseCommandType(data);
        return true;
    }
    
    std::string typeStr = data.substr(0, pos);
    type_ = ParseCommandType(typeStr);
    
    std::string params = data.substr(pos + 1);
    parameters_.clear();
    
    size_t start = 0;
    while (start < params.length()) {
        size_t end = params.find('&', start);
        if (end == std::string::npos) end = params.length();
        
        std::string param = params.substr(start, end - start);
        size_t eqPos = param.find('=');
        
        if (eqPos != std::string::npos) {
            std::string name = param.substr(0, eqPos);
            std::string value = param.substr(eqPos + 1);
            parameters_.push_back(CommandParameter(name, value));
        }
        
        start = end + 1;
    }
    
    return true;
}

CommandType Command::ParseCommandType(const std::string& typeStr) {
    if (typeStr == "StartGame") return CommandType::StartGame;
    if (typeStr == "LoadGame") return CommandType::LoadGame;
    if (typeStr == "SaveGame") return CommandType::SaveGame;
    if (typeStr == "ExitGame") return CommandType::ExitGame;
    if (typeStr == "Pause") return CommandType::Pause;
    if (typeStr == "Resume") return CommandType::Resume;
    if (typeStr == "SelectArmy") return CommandType::SelectArmy;
    if (typeStr == "MoveArmy") return CommandType::MoveArmy;
    if (typeStr == "AttackTarget") return CommandType::AttackTarget;
    if (typeStr == "SiegeSettlement") return CommandType::SiegeSettlement;
    if (typeStr == "EnterSettlement") return CommandType::EnterSettlement;
    if (typeStr == "RecruitUnits") return CommandType::RecruitUnits;
    if (typeStr == "ConstructBuilding") return CommandType::ConstructBuilding;
    if (typeStr == "SelectUnit") return CommandType::SelectUnit;
    if (typeStr == "MoveUnit") return CommandType::MoveUnit;
    if (typeStr == "AttackUnit") return CommandType::AttackUnit;
    if (typeStr == "RotateUnit") return CommandType::RotateUnit;
    if (typeStr == "ChangeFormation") return CommandType::ChangeFormation;
    if (typeStr == "Retreat") return CommandType::Retreat;
    if (typeStr == "TogglePause") return CommandType::TogglePause;
    if (typeStr == "OpenDiplomacy") return CommandType::OpenDiplomacy;
    if (typeStr == "DeclareWar") return CommandType::DeclareWar;
    if (typeStr == "ProposePeace") return CommandType::ProposePeace;
    if (typeStr == "ProposeAlliance") return CommandType::ProposeAlliance;
    if (typeStr == "OpenTrade") return CommandType::OpenTrade;
    if (typeStr == "EndTurn") return CommandType::EndTurn;
    if (typeStr == "NextPhase") return CommandType::NextPhase;
    if (typeStr == "OpenMenu") return CommandType::OpenMenu;
    if (typeStr == "CloseMenu") return CommandType::CloseMenu;
    if (typeStr == "ShowHelp") return CommandType::ShowHelp;
    if (typeStr == "ToggleOverlay") return CommandType::ToggleOverlay;
    return CommandType::Unknown;
}

std::string Command::CommandTypeToString(CommandType type) {
    switch (type) {
        case CommandType::StartGame: return "StartGame";
        case CommandType::LoadGame: return "LoadGame";
        case CommandType::SaveGame: return "SaveGame";
        case CommandType::ExitGame: return "ExitGame";
        case CommandType::Pause: return "Pause";
        case CommandType::Resume: return "Resume";
        case CommandType::SelectArmy: return "SelectArmy";
        case CommandType::MoveArmy: return "MoveArmy";
        case CommandType::AttackTarget: return "AttackTarget";
        case CommandType::SiegeSettlement: return "SiegeSettlement";
        case CommandType::EnterSettlement: return "EnterSettlement";
        case CommandType::RecruitUnits: return "RecruitUnits";
        case CommandType::ConstructBuilding: return "ConstructBuilding";
        case CommandType::SelectUnit: return "SelectUnit";
        case CommandType::MoveUnit: return "MoveUnit";
        case CommandType::AttackUnit: return "AttackUnit";
        case CommandType::RotateUnit: return "RotateUnit";
        case CommandType::ChangeFormation: return "ChangeFormation";
        case CommandType::Retreat: return "Retreat";
        case CommandType::TogglePause: return "TogglePause";
        case CommandType::OpenDiplomacy: return "OpenDiplomacy";
        case CommandType::DeclareWar: return "DeclareWar";
        case CommandType::ProposePeace: return "ProposePeace";
        case CommandType::ProposeAlliance: return "ProposeAlliance";
        case CommandType::OpenTrade: return "OpenTrade";
        case CommandType::EndTurn: return "EndTurn";
        case CommandType::NextPhase: return "NextPhase";
        case CommandType::OpenMenu: return "OpenMenu";
        case CommandType::CloseMenu: return "CloseMenu";
        case CommandType::ShowHelp: return "ShowHelp";
        case CommandType::ToggleOverlay: return "ToggleOverlay";
        default: return "Unknown";
    }
}

} // namespace MingGoRTS
