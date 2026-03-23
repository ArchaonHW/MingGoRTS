#pragma once

#include "Faction.h"
#include <vector>
#include <memory>
#include <map>

namespace MingGoRTS {

// 派系管理器 - 管理所有派系
class FactionManager {
public:
    FactionManager();
    ~FactionManager();
    
    // 初始化与关闭
    void Initialize();
    void Shutdown();
    
    // 派系管理
    int CreateFaction(const std::string& name, CultureType culture);
    void DestroyFaction(int factionId);
    Faction* GetFaction(int factionId);
    std::vector<Faction*> GetAllFactions();
    std::vector<Faction*> GetAIFactions();
    std::vector<Faction*> GetPlayerFactions();
    
    // 玩家控制
    void SetPlayerFaction(int factionId);
    Faction* GetPlayerFaction();
    
    // 查询
    int GetFactionCount() const { return static_cast<int>(factions_.size()); }
    Faction* GetFactionByName(const std::string& name);
    std::vector<Faction*> GetFactionsByCulture(CultureType culture);
    
    // 外交关系查询
    std::vector<int> GetWarEnemies(int factionId);
    std::vector<int> GetAllies(int factionId);
    bool AreAtWar(int faction1, int faction2);
    bool AreAllied(int faction1, int faction2);
    
    // 更新
    void Update(float deltaTime);
    void ProcessEndOfTurn();
    
    // 预设派系创建
    void CreateDefaultFactions();
    void CreateRomanFaction();
    void CreateBarbarianFaction();
    void CreateEasternFaction();
    
private:
    std::map<int, std::unique_ptr<Faction>> factions_;
    int nextFactionId_;
    int playerFactionId_;
};

} // namespace MingGoRTS
