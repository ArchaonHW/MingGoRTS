#pragma once

#include "../Engine/Types.h"
#include <string>
#include <vector>
#include <map>

namespace MingGoRTS {

// 文化类型 - 类似 Total War 的文化分组
enum class CultureType {
    Roman,
    Barbarian,
    Eastern,
    Greek,
    Egyptian,
    Carthaginian,
    Numidian,
    Celtic,
    Germanic,
    Custom
};

// 派系特质
struct FactionTrait {
    std::string name;
    std::string description;
    float publicOrderModifier;
    float taxModifier;
    float armyMaintenanceModifier;
    float diplomacyBonus;
    bool canRecruitMercenaries;
};

// 政治党派 - 参考 Rome II 政治系统
struct PoliticalParty {
    int id;
    std::string name;
    float influence;  // 影响力 0-100
    float gravitas;   // 威望
    bool isRuling;
    int leaderCharacterId;
    std::vector<int> memberIds;
};

// 派系 - 战役层的主要参与者
class Faction {
public:
    Faction(int id, const std::string& name, CultureType culture);
    ~Faction();
    
    // 基础属性
    int GetId() const { return id_; }
    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }
    
    // 文化
    CultureType GetCulture() const { return culture_; }
    void SetCulture(CultureType culture) { culture_ = culture; }
    
    // 特质
    void AddTrait(const FactionTrait& trait);
    const std::vector<FactionTrait>& GetTraits() const { return traits_; }
    
    // 经济
    int GetMoney() const { return money_; }
    void AddMoney(int amount) { money_ += amount; }
    bool SpendMoney(int amount);
    int GetIncome() const { return income_; }
    void SetIncome(int income) { income_ = income; }
    
    // 政治
    void AddParty(const PoliticalParty& party);
    PoliticalParty* GetParty(int partyId);
    PoliticalParty* GetRulingParty();
    void UpdatePoliticalBalance();
    float GetPoliticalStability() const { return politicalStability_; }
    
    // 外交
    void SetRelationship(int otherFactionId, float value);  // -100 到 100
    float GetRelationship(int otherFactionId) const;
    bool IsAtWar(int otherFactionId) const;
    void DeclareWar(int otherFactionId);
    void MakePeace(int otherFactionId);
    void FormAlliance(int otherFactionId);
    void BreakAlliance(int otherFactionId);
    
    // 胜利条件
    enum class VictoryType {
        Military,      // 军事胜利
        Economic,      // 经济胜利
        Cultural,      // 文化胜利
        Diplomatic,    // 外交胜利
        Scientific     // 科技胜利
    };
    
    void SetVictoryType(VictoryType type) { victoryType_ = type; }
    VictoryType GetVictoryType() const { return victoryType_; }
    bool CheckVictoryConditions() const;
    
    // AI
    bool IsAI() const { return isAI_; }
    void SetAI(bool isAI) { isAI_ = isAI; }
    int GetAIDifficulty() const { return aiDifficulty_; }
    void SetAIDifficulty(int difficulty) { aiDifficulty_ = difficulty; }
    
    // 更新
    void Update(float deltaTime);
    void ProcessEndOfTurn();
    
    // 统计
    int GetSettlementCount() const { return settlementCount_; }
    void SetSettlementCount(int count) { settlementCount_ = count; }
    int GetArmyCount() const { return armyCount_; }
    void SetArmyCount(int count) { armyCount_ = count; }
    int GetTotalPopulation() const { return totalPopulation_; }
    void SetTotalPopulation(int pop) { totalPopulation_ = pop; }
    
private:
    int id_;
    std::string name_;
    CultureType culture_;
    
    std::vector<FactionTrait> traits_;
    
    int money_;
    int income_;
    
    std::vector<PoliticalParty> parties_;
    float politicalStability_;
    
    std::map<int, float> relationships_;  // factionId -> relationship value
    std::vector<int> warEnemies_;
    std::vector<int> allies_;
    
    VictoryType victoryType_;
    
    bool isAI_;
    int aiDifficulty_;
    
    int settlementCount_;
    int armyCount_;
    int totalPopulation_;
    
    void UpdateRelationships();
    void CalculatePoliticalStability();
};

} // namespace MingGoRTS
