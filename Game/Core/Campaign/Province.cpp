#include "Province.h"
#include <algorithm>

namespace MingGoRTS {

Province::Province(int id, const std::string& name)
    : id_(id)
    , name_(name)
    , controllingFactionId_(-1)
    , totalIncome_(0)
    , averagePublicOrder_(50.0f) {
}

Province::~Province() = default;

void Province::AddSettlement(int settlementId) {
    if (std::find(settlementIds_.begin(), settlementIds_.end(), settlementId) == settlementIds_.end()) {
        settlementIds_.push_back(settlementId);
    }
}

void Province::RemoveSettlement(int settlementId) {
    auto it = std::find(settlementIds_.begin(), settlementIds_.end(), settlementId);
    if (it != settlementIds_.end()) {
        settlementIds_.erase(it);
    }
}

void Province::Update(float deltaTime) {
    // 持续更新逻辑
}

void Province::ProcessEndOfTurn() {
    // 处理回合结束逻辑
    // 更新经济汇总等
}

} // namespace MingGoRTS
