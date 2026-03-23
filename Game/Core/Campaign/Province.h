#pragma once

#include <string>
#include <vector>

namespace MingGoRTS {

// 行省 - 多个定居点的集合
class Province {
public:
    Province(int id, const std::string& name);
    ~Province();
    
    // 基础属性
    int GetId() const { return id_; }
    const std::string& GetName() const { return name_; }
    void SetName(const std::string& name) { name_ = name; }
    
    // 定居点管理
    void AddSettlement(int settlementId);
    void RemoveSettlement(int settlementId);
    const std::vector<int>& GetSettlements() const { return settlementIds_; }
    int GetSettlementCount() const { return settlementIds_.size(); }
    
    // 控制派系
    int GetControllingFaction() const { return controllingFactionId_; }
    void SetControllingFaction(int factionId) { controllingFactionId_ = factionId; }
    
    // 经济汇总
    int GetTotalIncome() const { return totalIncome_; }
    void SetTotalIncome(int income) { totalIncome_ = income; }
    
    // 公共秩序平均
    float GetAveragePublicOrder() const { return averagePublicOrder_; }
    void SetAveragePublicOrder(float order) { averagePublicOrder_ = order; }
    
    // 更新
    void Update(float deltaTime);
    void ProcessEndOfTurn();
    
private:
    int id_;
    std::string name_;
    std::vector<int> settlementIds_;
    int controllingFactionId_;
    int totalIncome_;
    float averagePublicOrder_;
};

} // namespace MingGoRTS
