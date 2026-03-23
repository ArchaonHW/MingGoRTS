#pragma once

#include "../Engine/Types.h"
#include <string>
#include <vector>

namespace MingGoRTS {

// 定居点类型
enum class SettlementType {
    Village,     // 村庄
    Town,        // 城镇
    City,        // 城市
    Fortress,    // 要塞
    Port         // 港口
};

// 建筑
struct Building {
    std::string name;
    std::string type;
    int level;
    int constructionCost;
    int maintenanceCost;
    int incomeBonus;
    int defenseBonus;
    bool isConstructed;
};

// 定居点 - 战役地图上的据点
class Settlement {
public:
    Settlement(int id, Vector2D position, int factionId, const std::string& name);
    ~Settlement();
    
    // 基础属性
    int GetId() const { return id_; }
    Vector2D GetPosition() const { return position_; }
    int GetFactionId() const { return factionId_; }
    void SetFactionId(int factionId) { factionId_ = factionId; }
    const std::string& GetName() const { return name_; }
    
    // 定居点类型
    SettlementType GetType() const { return type_; }
    void SetType(SettlementType type) { type_ = type; }
    
    // 行省
    int GetProvinceId() const { return provinceId_; }
    void SetProvinceId(int provinceId) { provinceId_ = provinceId; }
    
    // 人口与经济
    int GetPopulation() const { return population_; }
    void SetPopulation(int population) { population_ = population; }
    int GetIncome() const { return CalculateIncome(); }
    int GetGrowthRate() const { return growthRate_; }
    
    // 公共秩序
    float GetPublicOrder() const { return publicOrder_; }
    void ModifyPublicOrder(float delta) { publicOrder_ += delta; }
    
    // 建筑
    void AddBuilding(const Building& building);
    void RemoveBuilding(const std::string& buildingName);
    std::vector<Building>& GetBuildings() { return buildings_; }
    
    // 防御
    int GetDefenseLevel() const { return defenseLevel_; }
    void SetDefenseLevel(int level) { defenseLevel_ = level; }
    int GetGarrisonSize() const { return garrisonSize_; }
    void SetGarrisonSize(int size) { garrisonSize_ = size; }
    
    // 升级
    bool CanUpgrade() const;
    void Upgrade();
    
    // 更新
    void Update(float deltaTime);
    void ProcessEndOfTurn();
    
private:
    int id_;
    Vector2D position_;
    int factionId_;
    std::string name_;
    SettlementType type_;
    int provinceId_;
    
    int population_;
    int growthRate_;
    float publicOrder_;
    
    std::vector<Building> buildings_;
    int defenseLevel_;
    int garrisonSize_;
    
    int CalculateIncome() const;
    void UpdateGrowth();
};

} // namespace MingGoRTS
