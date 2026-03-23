#include "Settlement.h"
#include <algorithm>

namespace MingGoRTS {

Settlement::Settlement(int id, Vector2D position, int factionId, const std::string& name)
    : id_(id)
    , position_(position)
    , factionId_(factionId)
    , name_(name)
    , type_(SettlementType::Village)
    , provinceId_(-1)
    , population_(1000)
    , growthRate_(5)
    , publicOrder_(50.0f)
    , defenseLevel_(1)
    , garrisonSize_(100) {
}

Settlement::~Settlement() = default;

void Settlement::AddBuilding(const Building& building) {
    buildings_.push_back(building);
}

void Settlement::RemoveBuilding(const std::string& buildingName) {
    auto it = std::remove_if(buildings_.begin(), buildings_.end(),
        [&buildingName](const Building& b) { return b.name == buildingName; });
    buildings_.erase(it, buildings_.end());
}

bool Settlement::CanUpgrade() const {
    switch (type_) {
        case SettlementType::Village:
            return population_ >= 3000;
        case SettlementType::Town:
            return population_ >= 10000;
        case SettlementType::City:
            return population_ >= 50000;
        default:
            return false;
    }
}

void Settlement::Upgrade() {
    if (!CanUpgrade()) return;
    
    switch (type_) {
        case SettlementType::Village:
            type_ = SettlementType::Town;
            defenseLevel_ += 2;
            break;
        case SettlementType::Town:
            type_ = SettlementType::City;
            defenseLevel_ += 3;
            break;
        case SettlementType::City:
            type_ = SettlementType::Fortress;
            defenseLevel_ += 5;
            break;
        default:
            break;
    }
}

void Settlement::Update(float deltaTime) {
    // 持续更新逻辑
}

void Settlement::ProcessEndOfTurn() {
    UpdateGrowth();
    
    // 公共秩序自然波动
    publicOrder_ += (50.0f - publicOrder_) * 0.01f;
    
    // 限制范围
    if (publicOrder_ > 100.0f) publicOrder_ = 100.0f;
    if (publicOrder_ < -100.0f) publicOrder_ = -100.0f;
}

int Settlement::CalculateIncome() const {
    int baseIncome = population_ / 100;
    int buildingIncome = 0;
    
    for (const auto& building : buildings_) {
        if (building.isConstructed) {
            buildingIncome += building.incomeBonus;
        }
    }
    
    // 公共秩序影响
    float orderMultiplier = 0.5f + (publicOrder_ / 100.0f) * 0.5f;
    
    return static_cast<int>((baseIncome + buildingIncome) * orderMultiplier);
}

void Settlement::UpdateGrowth() {
    // 基于公共秩序调整增长率
    float orderFactor = publicOrder_ / 100.0f;
    int actualGrowth = static_cast<int>(growthRate_ * (0.5f + orderFactor * 0.5f));
    
    population_ += actualGrowth;
    
    // 不同类型定居点的人口上限
    int maxPop = 0;
    switch (type_) {
        case SettlementType::Village: maxPop = 5000; break;
        case SettlementType::Town: maxPop = 20000; break;
        case SettlementType::City: maxPop = 100000; break;
        case SettlementType::Fortress: maxPop = 50000; break;
        case SettlementType::Port: maxPop = 80000; break;
    }
    
    if (population_ > maxPop) {
        population_ = maxPop;
    }
}

} // namespace MingGoRTS
