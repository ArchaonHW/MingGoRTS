#include "Unit.h"
#include <cmath>
#include <algorithm>

namespace MingGoRTS {

Unit::Unit(int id, int unitTypeId, int factionId, Vector2D position)
    : id_(id)
    , unitTypeId_(unitTypeId)
    , factionId_(factionId)
    , position_(position)
    , destination_(position)
    , rotation_(0.0f)
    , state_(UnitState::Idle)
    , formation_(FormationType::Line)
    , maxHealth_(100)
    , currentHealth_(100)
    , maxSoldiers_(120)
    , currentSoldiers_(120)
    , attackDamage_(25.0f)
    , attackRange_(3.0f)
    , attackSpeed_(1.0f)
    , armor_(5.0f)
    , movementSpeed_(3.0f)
    , visionRange_(50.0f)
    , collisionRadius_(2.0f)
    , baseMorale_(50.0f)
    , currentMorale_(50.0f)
    , moraleRecoveryRate_(2.0f)
    , currentTarget_(nullptr)
    , attackCooldown_(0.0f)
    , chargeBonus_(0.0f)
    , isCharging_(false)
    , isPlayerControlled_(false)
    , hasNightVision_(false)
    , weatherMultiplier_(1.0f) {
}

Unit::~Unit() = default;

void Unit::MoveTo(Vector2D destination) {
    destination_ = destination;
    state_ = UnitState::Moving;
    FacePosition(destination);
}

void Unit::Stop() {
    state_ = UnitState::Idle;
    destination_ = position_;
    isCharging_ = false;
}

void Unit::FacePosition(Vector2D target) {
    float dx = target.x - position_.x;
    float dy = target.y - position_.y;
    rotation_ = std::atan2(dy, dx) * 180.0f / 3.14159f;
}

void Unit::SetFormation(int formationId) {
    formation_ = static_cast<FormationType>(formationId);
}

void Unit::Attack(Unit* target) {
    if (!target || !target->IsAlive()) return;
    
    currentTarget_ = target;
    state_ = UnitState::Attacking;
    
    // 面向目标
    FacePosition(target->GetPosition());
    
    // 检查距离
    float distance = position_.Distance(target->GetPosition());
    if (distance > attackRange_) {
        // 距离太远，先移动
        MoveTo(target->GetPosition());
    }
}

void Unit::TakeDamage(float damage) {
    // 护甲减伤
    float actualDamage = damage * (100.0f / (100.0f + armor_));
    
    // 士气影响
    float moraleFactor = currentMorale_ / 100.0f;
    actualDamage *= (0.8f + 0.4f * moraleFactor);
    
    currentHealth_ -= static_cast<int>(actualDamage);
    
    // 士气下降
    currentMorale_ -= actualDamage * 0.1f;
    
    if (currentHealth_ <= 0 || currentSoldiers_ <= 0) {
        Die();
    }
}

void Unit::ProcessCombat() {
    if (state_ != UnitState::Attacking || !currentTarget_) return;
    
    if (!currentTarget_->IsAlive()) {
        currentTarget_ = nullptr;
        state_ = UnitState::Idle;
        return;
    }
    
    float distance = position_.Distance(currentTarget_->GetPosition());
    
    if (distance <= attackRange_) {
        // 在攻击范围内
        if (attackCooldown_ <= 0.0f) {
            // 执行攻击
            float damage = attackDamage_;
            
            // 冲锋加成
            if (isCharging_) {
                damage *= (1.0f + chargeBonus_);
                isCharging_ = false;
            }
            
            // 编队加成
            switch (formation_) {
                case FormationType::Phalanx:
                    if (unitTypeId_ == 1) {  // 假设1是长矛兵
                        damage *= 1.3f;
                    }
                    break;
                case FormationType::ShieldWall:
                    damage *= 0.9f;
                    break;
                default:
                    break;
            }
            
            currentTarget_->TakeDamage(damage);
            attackCooldown_ = 1.0f / attackSpeed_;
        }
    } else {
        // 追击目标
        MoveTo(currentTarget_->GetPosition());
    }
}

void Unit::UpdateMorale(int nearbyFriends, int nearbyEnemies) {
    // 基础士气恢复
    float moraleChange = moraleRecoveryRate_ * 0.1f;
    
    // 友军加成
    if (nearbyFriends > 0) {
        moraleChange += nearbyFriends * 0.5f;
    }
    
    // 敌军压力
    if (nearbyEnemies > nearbyFriends) {
        moraleChange -= (nearbyEnemies - nearbyFriends) * 1.0f;
    }
    
    // 伤亡影响
    float casualtyRatio = 1.0f - (static_cast<float>(currentSoldiers_) / maxSoldiers_);
    moraleChange -= casualtyRatio * 10.0f;
    
    // 健康状态
    float healthRatio = static_cast<float>(currentHealth_) / maxHealth_;
    moraleChange -= (1.0f - healthRatio) * 5.0f;
    
    currentMorale_ += moraleChange;
    
    // 限制范围
    currentMorale_ = std::max(0.0f, std::min(100.0f, currentMorale_));
    
    // 士气崩溃
    if (currentMorale_ <= 0.0f && state_ != UnitState::Routing) {
        Retreat();
    }
}

void Unit::ApplyMoraleBonus(float bonus) {
    currentMorale_ += bonus;
    currentMorale_ = std::min(100.0f, currentMorale_);
}

void Unit::Retreat() {
    state_ = UnitState::Retreating;
    
    // 撤退方向（远离敌人）
    // 简化处理：向后撤退
    Vector2D retreatDir(-std::cos(rotation_ * 3.14159f / 180.0f), 
                        -std::sin(rotation_ * 3.14159f / 180.0f));
    destination_ = position_ + retreatDir * 50.0f;
}

void Unit::ApplyWeatherPenalty(float multiplier) {
    weatherMultiplier_ = multiplier;
}

void Unit::SetNightVision(bool enabled) {
    hasNightVision_ = enabled;
}

void Unit::ApplyChargeBonus() {
    isCharging_ = true;
    chargeBonus_ = 0.5f;  // 50%伤害加成
}

void Unit::Update(float deltaTime) {
    // 更新移动
    UpdateMovement(deltaTime);
    
    // 更新战斗
    UpdateCombat(deltaTime);
    
    // 更新士气恢复
    UpdateMoraleRecovery(deltaTime);
    
    // 减少攻击冷却
    if (attackCooldown_ > 0.0f) {
        attackCooldown_ -= deltaTime;
    }
    
    // 处理撤退
    if (state_ == UnitState::Retreating) {
        float dist = position_.Distance(destination_);
        if (dist < 1.0f) {
            // 成功撤退出战场
            currentHealth_ = 0;  // 标记为退出战斗
        }
    }
}

void Unit::UpdateMovement(float deltaTime) {
    if (state_ != UnitState::Moving && state_ != UnitState::Retreating) return;
    
    Vector2D direction = destination_ - position_;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance < 0.1f) {
        position_ = destination_;
        if (state_ == UnitState::Moving) {
            state_ = UnitState::Idle;
        }
        return;
    }
    
    // 归一化方向
    direction.x /= distance;
    direction.y /= distance;
    
    // 移动
    float speed = GetMovementSpeed();
    if (state_ == UnitState::Retreating) {
        speed *= 1.5f;  // 撤退速度更快
    }
    
    position_.x += direction.x * speed * deltaTime;
    position_.y += direction.y * speed * deltaTime;
}

void Unit::UpdateCombat(float deltaTime) {
    // 战斗逻辑在 ProcessCombat 中处理
}

void Unit::UpdateMoraleRecovery(float deltaTime) {
    // 基础士气恢复
    if (currentMorale_ < baseMorale_) {
        currentMorale_ += moraleRecoveryRate_ * deltaTime;
        currentMorale_ = std::min(currentMorale_, baseMorale_);
    }
}

float Unit::GetMovementModifier() const {
    float modifier = weatherMultiplier_;
    
    // 编队影响
    switch (formation_) {
        case FormationType::Square:
            modifier *= 0.7f;
            break;
        case FormationType::Phalanx:
            modifier *= 0.8f;
            break;
        case FormationType::Skirmish:
            modifier *= 1.2f;
            break;
        default:
            break;
    }
    
    // 士气影响
    modifier *= (0.5f + 0.5f * (currentMorale_ / 100.0f));
    
    return modifier;
}

float Unit::GetDefenseModifier() const {
    float modifier = 0.0f;
    
    // 编队防御加成
    switch (formation_) {
        case FormationType::ShieldWall:
            modifier += 0.3f;
            break;
        case FormationType::Phalanx:
            modifier += 0.4f;
            break;
        case FormationType::Square:
            modifier += 0.2f;
            break;
        case FormationType::Testudo:
            modifier += 0.5f;
            break;
        default:
            break;
    }
    
    return modifier;
}

void Unit::Die() {
    state_ = UnitState::Dead;
    currentHealth_ = 0;
    currentSoldiers_ = 0;
}

bool Unit::CollidesWith(const Unit& other) const {
    float distance = position_.Distance(other.GetPosition());
    return distance < (collisionRadius_ + other.GetCollisionRadius());
}

} // namespace MingGoRTS
