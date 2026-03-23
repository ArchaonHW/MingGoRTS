#pragma once

#include "../Engine/Types.h"
#include <string>

namespace MingGoRTS {

// 单位状态
enum class UnitState {
    Idle,
    Moving,
    Attacking,
    Defending,
    Retreating,
    Routing,
    Dead
};

// 单位类型
struct UnitType {
    int id;
    std::string name;
    std::string category;  // Infantry, Cavalry, Archer, etc.
    
    // 基础属性
    int maxHealth;
    int maxSoldiers;       // 士兵数量
    float attackDamage;
    float attackRange;
    float attackSpeed;
    float armor;
    float movementSpeed;
    float visionRange;
    
    // 特殊属性
    float chargeBonus;
    float morale;
    bool canFormShieldWall;
    bool canFormPhalanx;
    bool canFormTestudo;
};

// 编队类型
enum class FormationType {
    Line,           // 线列
    Column,         // 纵队
    Wedge,          // 楔形
    Square,         // 方阵
    Skirmish,       // 散兵
    ShieldWall,     // 盾墙
    Phalanx,        // 方阵（希腊）
    Testudo,        // 龟甲阵
    Loose           // 松散
};

// 单位 - 战斗层的基本战斗单元（模拟Total War的部队单位）
class Unit {
public:
    Unit(int id, int unitTypeId, int factionId, Vector2D position);
    ~Unit();
    
    // 基础属性
    int GetId() const { return id_; }
    int GetUnitTypeId() const { return unitTypeId_; }
    int GetFactionId() const { return factionId_; }
    
    // 位置与移动
    Vector2D GetPosition() const { return position_; }
    void SetPosition(Vector2D position) { position_ = position; }
    void MoveTo(Vector2D destination);
    void Stop();
    bool IsMoving() const { return state_ == UnitState::Moving; }
    
    // 旋转与朝向
    float GetRotation() const { return rotation_; }
    void SetRotation(float angle) { rotation_ = angle; }
    void FacePosition(Vector2D target);
    
    // 编队
    FormationType GetFormation() const { return formation_; }
    void SetFormation(int formationId);
    
    // 战斗
    void Attack(Unit* target);
    void TakeDamage(float damage);
    void ProcessCombat();
    bool IsAlive() const { return currentHealth_ > 0 && currentSoldiers_ > 0; }
    bool CanFight() const { return IsAlive() && state_ != UnitState::Routing && state_ != UnitState::Retreating; }
    float GetHealthPercent() const { return static_cast<float>(currentHealth_) / maxHealth_; }
    int GetSoldierCount() const { return currentSoldiers_; }
    
    // 射程与视野
    float GetAttackRange() const { return attackRange_; }
    float GetVisionRange() const { return visionRange_; }
    
    // 士气
    float GetMorale() const { return currentMorale_; }
    void UpdateMorale(int nearbyFriends, int nearbyEnemies);
    void ApplyMoraleBonus(float bonus);
    void Retreat();
    bool IsRouting() const { return state_ == UnitState::Routing; }
    
    // 属性查询
    float GetAttackDamage() const { return attackDamage_; }
    float GetMovementSpeed() const { return movementSpeed_ * GetMovementModifier(); }
    float GetArmor() const { return armor_; }
    
    // 状态
    UnitState GetState() const { return state_; }
    bool IsPlayerControlled() const { return isPlayerControlled_; }
    void SetPlayerControlled(bool controlled) { isPlayerControlled_ = controlled; }
    
    // 特殊效果
    void ApplyWeatherPenalty(float multiplier);
    void SetNightVision(bool enabled);
    void ApplyChargeBonus();
    
    // 更新
    void Update(float deltaTime);
    
    // 碰撞
    float GetCollisionRadius() const { return collisionRadius_; }
    bool CollidesWith(const Unit& other) const;
    
private:
    int id_;
    int unitTypeId_;
    int factionId_;
    
    Vector2D position_;
    Vector2D destination_;
    float rotation_;
    
    UnitState state_;
    FormationType formation_;
    
    // 单位属性
    int maxHealth_;
    int currentHealth_;
    int maxSoldiers_;
    int currentSoldiers_;
    float attackDamage_;
    float attackRange_;
    float attackSpeed_;
    float armor_;
    float movementSpeed_;
    float visionRange_;
    float collisionRadius_;
    
    // 士气
    float baseMorale_;
    float currentMorale_;
    float moraleRecoveryRate_;
    
    // 战斗
    Unit* currentTarget_;
    float attackCooldown_;
    float chargeBonus_;
    bool isCharging_;
    
    // 控制
    bool isPlayerControlled_;
    bool hasNightVision_;
    float weatherMultiplier_;
    
    // 内部方法
    void UpdateMovement(float deltaTime);
    void UpdateCombat(float deltaTime);
    void UpdateMoraleRecovery(float deltaTime);
    float GetMovementModifier() const;
    float GetDefenseModifier() const;
    void Die();
};

} // namespace MingGoRTS
