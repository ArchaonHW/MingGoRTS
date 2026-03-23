#include "Faction.h"
#include <algorithm>

namespace MingGoRTS {

Faction::Faction(int id, const std::string& name, CultureType culture)
    : id_(id)
    , name_(name)
    , culture_(culture)
    , money_(5000)
    , income_(500)
    , politicalStability_(50.0f)
    , victoryType_(VictoryType::Military)
    , isAI_(false)
    , aiDifficulty_(1)
    , settlementCount_(0)
    , armyCount_(0)
    , totalPopulation_(0) {
}

Faction::~Faction() = default;

void Faction::AddTrait(const FactionTrait& trait) {
    traits_.push_back(trait);
}

bool Faction::SpendMoney(int amount) {
    if (money_ >= amount) {
        money_ -= amount;
        return true;
    }
    return false;
}

void Faction::AddParty(const PoliticalParty& party) {
    parties_.push_back(party);
}

PoliticalParty* Faction::GetParty(int partyId) {
    for (auto& party : parties_) {
        if (party.id == partyId) {
            return &party;
        }
    }
    return nullptr;
}

PoliticalParty* Faction::GetRulingParty() {
    for (auto& party : parties_) {
        if (party.isRuling) {
            return &party;
        }
    }
    return nullptr;
}

void Faction::UpdatePoliticalBalance() {
    CalculatePoliticalStability();
    
    // 检查是否有党派影响力过高或过低
    float totalInfluence = 0.0f;
    float maxInfluence = 0.0f;
    float minInfluence = 100.0f;
    
    for (const auto& party : parties_) {
        totalInfluence += party.influence;
        maxInfluence = std::max(maxInfluence, party.influence);
        minInfluence = std::min(minInfluence, party.influence);
    }
    
    // 影响力严重失衡警告
    if (maxInfluence > 70.0f || minInfluence < 10.0f) {
        // 触发政治危机风险
        politicalStability_ -= 5.0f;
    }
}

void Faction::SetRelationship(int otherFactionId, float value) {
    relationships_[otherFactionId] = std::max(-100.0f, std::min(100.0f, value));
}

float Faction::GetRelationship(int otherFactionId) const {
    auto it = relationships_.find(otherFactionId);
    if (it != relationships_.end()) {
        return it->second;
    }
    return 0.0f;  // 中立
}

bool Faction::IsAtWar(int otherFactionId) const {
    return std::find(warEnemies_.begin(), warEnemies_.end(), otherFactionId) != warEnemies_.end();
}

void Faction::DeclareWar(int otherFactionId) {
    if (!IsAtWar(otherFactionId)) {
        warEnemies_.push_back(otherFactionId);
        SetRelationship(otherFactionId, -100.0f);
        
        // 自动断交
        auto allyIt = std::find(allies_.begin(), allies_.end(), otherFactionId);
        if (allyIt != allies_.end()) {
            allies_.erase(allyIt);
        }
    }
}

void Faction::MakePeace(int otherFactionId) {
    auto it = std::find(warEnemies_.begin(), warEnemies_.end(), otherFactionId);
    if (it != warEnemies_.end()) {
        warEnemies_.erase(it);
        SetRelationship(otherFactionId, -50.0f);  // 战后关系仍然紧张
    }
}

void Faction::FormAlliance(int otherFactionId) {
    if (std::find(allies_.begin(), allies_.end(), otherFactionId) == allies_.end()) {
        allies_.push_back(otherFactionId);
        SetRelationship(otherFactionId, 80.0f);
    }
}

void Faction::BreakAlliance(int otherFactionId) {
    auto it = std::find(allies_.begin(), allies_.end(), otherFactionId);
    if (it != allies_.end()) {
        allies_.erase(it);
        SetRelationship(otherFactionId, 20.0f);  // 断交后关系下降
    }
}

bool Faction::CheckVictoryConditions() const {
    switch (victoryType_) {
        case VictoryType::Military:
            // 军事胜利：控制一定比例的定居点和军队
            return settlementCount_ >= 20 && armyCount_ >= 10;
            
        case VictoryType::Economic:
            // 经济胜利：达到一定财富和收入
            return money_ >= 100000 && income_ >= 10000;
            
        case VictoryType::Cultural:
            // 文化胜利：控制特定文化区域
            return settlementCount_ >= 15 && totalPopulation_ >= 500000;
            
        case VictoryType::Diplomatic:
            // 外交胜利：与所有派系保持友好
            for (const auto& [factionId, relation] : relationships_) {
                if (relation < 50.0f) {
                    return false;
                }
            }
            return settlementCount_ >= 10;
            
        case VictoryType::Scientific:
            // 科技胜利：研究完所有科技（简化）
            return settlementCount_ >= 12;
            
        default:
            return false;
    }
}

void Faction::Update(float deltaTime) {
    // 持续更新逻辑
}

void Faction::ProcessEndOfTurn() {
    // 收入
    money_ += income_;
    
    // 军队维护费
    int maintenance = armyCount_ * 100;  // 每支军队100维护费
    money_ -= maintenance;
    
    // 特质影响
    for (const auto& trait : traits_) {
        if (trait.canRecruitMercenaries) {
            // 可以招募雇佣兵的逻辑
        }
    }
    
    // 更新政治
    UpdatePoliticalBalance();
    
    // 政治稳定度自然恢复
    if (politicalStability_ < 50.0f) {
        politicalStability_ += 2.0f;
    } else if (politicalStability_ > 50.0f) {
        politicalStability_ -= 1.0f;
    }
    
    // 限制范围
    politicalStability_ = std::max(0.0f, std::min(100.0f, politicalStability_));
    
    // 更新外交关系（缓慢趋向中立）
    UpdateRelationships();
}

void Faction::UpdateRelationships() {
    for (auto& [factionId, relation] : relationships_) {
        // 战争关系保持不变
        if (IsAtWar(factionId)) {
            continue;
        }
        
        // 非战争关系缓慢趋向0
        if (relation > 0) {
            relation -= 0.5f;
        } else if (relation < 0) {
            relation += 0.5f;
        }
        
        // 限制范围
        relation = std::max(-100.0f, std::min(100.0f, relation));
    }
}

void Faction::CalculatePoliticalStability() {
    if (parties_.empty()) {
        politicalStability_ = 100.0f;
        return;
    }
    
    // 计算影响力分布
    float avgInfluence = 100.0f / parties_.size();
    float variance = 0.0f;
    
    for (const auto& party : parties_) {
        float diff = party.influence - avgInfluence;
        variance += diff * diff;
    }
    
    variance /= parties_.size();
    
    // 方差越大，稳定性越低
    politicalStability_ = 100.0f - (variance / 10.0f);
    
    // 限制范围
    politicalStability_ = std::max(0.0f, std::min(100.0f, politicalStability_));
}

} // namespace MingGoRTS
