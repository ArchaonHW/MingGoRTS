#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCombatSystem.generated.h"

// 前向聲明
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSCombatType : uint8
{
    Melee,          // 近戰
    Ranged,         // 遠程
    Artillery,      // 火炮
    Siege,          // 攻城
    Naval           // 海戰
};

UENUM(BlueprintType)
enum class ERTSDamageType : uint8
{
    Physical,       // 物理傷害
    Fire,           // 火焰傷害
    Explosive,      // 爆炸傷害
    Piercing,       // 穿刺傷害
    Magic,          // 魔法傷害
    Poison          // 毒素傷害
};

UENUM(BlueprintType)
enum class ERTSCombatStance : uint8
{
    Aggressive,     // 攻擊姿態
    Defensive,      // 防禦姿態
    Neutral,        // 中立姿態
    Passive         // 被動姿態
};

USTRUCT(BlueprintType)
struct FRTSCombatStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float AttackDamage;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float DefenseRating;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float AttackSpeed;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float AttackRange;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float Accuracy;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float CriticalChance;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float CriticalMultiplier;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float DodgeChance;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    float BlockChance;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    ERTSCombatType CombatType;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Stats")
    ERTSDamageType DamageType;

    FRTSCombatStats()
    {
        AttackDamage = 20.0f;
        DefenseRating = 10.0f;
        AttackSpeed = 1.0f;
        AttackRange = 100.0f;
        Accuracy = 0.8f;
        CriticalChance = 0.1f;
        CriticalMultiplier = 2.0f;
        DodgeChance = 0.1f;
        BlockChance = 0.15f;
        CombatType = ERTSCombatType::Melee;
        DamageType = ERTSDamageType::Physical;
    }
};

USTRUCT(BlueprintType)
struct FRTSCombatResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    bool bHit;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    bool bCritical;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    bool bDodged;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    bool bBlocked;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    float DamageDealt;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    float DamageBlocked;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    ERTSDamageType DamageType;

    UPROPERTY(BlueprintReadOnly, Category = "Combat Result")
    FString ResultMessage;

    FRTSCombatResult()
    {
        bHit = false;
        bCritical = false;
        bDodged = false;
        bBlocked = false;
        DamageDealt = 0.0f;
        DamageBlocked = 0.0f;
        DamageType = ERTSDamageType::Physical;
        ResultMessage = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FRTSAbilityData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    FString AbilityName;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    float Cooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    float CurrentCooldown;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    float ManaCost;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    float Range;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    bool bIsAvailable;

    UPROPERTY(BlueprintReadOnly, Category = "Ability Data")
    bool bIsActive;

    FRTSAbilityData()
    {
        AbilityName = TEXT("");
        Description = TEXT("");
        Cooldown = 5.0f;
        CurrentCooldown = 0.0f;
        ManaCost = 10.0f;
        Range = 100.0f;
        bIsAvailable = true;
        bIsActive = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCombatStarted, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target, ERTSCombatType, CombatType, ERTSDamageType, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatHit, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target, const FRTSCombatResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatMissed, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitKilled, AMingGoRTSUnit*, Victim, AMingGoRTSUnit*, Killer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityUsed, AMingGoRTSUnit*, Unit, const FString&, AbilityName);

/**
 * RTS戰鬥系統
 * 管理所有戰鬥相關的邏輯和計算
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSCombatSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCombatSystem();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void InitializeCombatSystem(UMingRTSUnitManager* InUnitManager);

    // 戰鬥統計
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetUnitCombatStats(AMingGoRTSUnit* Unit, const FRTSCombatStats& CombatStats);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    FRTSCombatStats GetUnitCombatStats(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateCombatStats(AMingGoRTSUnit* Unit, float DamageMultiplier = 1.0f, float DefenseMultiplier = 1.0f);

    // 戰鬥執行
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    FRTSCombatResult ExecuteCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool CanAttack(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void StartCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void StopCombat(AMingGoRTSUnit* Unit);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool IsInCombat(AMingGoRTSUnit* Unit) const;

    // 傷害計算
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float CalculateDamage(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float ApplyDamageReduction(float BaseDamage, ERTSDamageType DamageType, float DefenseRating);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool CheckHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool CheckCritical(AMingGoRTSUnit* Attacker);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool CheckDodge(AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    bool CheckBlock(AMingGoRTSUnit* Target);

    // 戰鬥姿態
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetCombatStance(AMingGoRTSUnit* Unit, ERTSCombatStance Stance);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    ERTSCombatStance GetCombatStance(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDamageMultiplier(ERTSCombatStance Stance) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDefenseMultiplier(ERTSCombatStance Stance) const;

    // 範圍攻擊
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    TArray<AMingGoRTSUnit*> GetUnitsInArea(const FVector& Center, float Radius, AMingGoRTSUnit* ExcludedUnit = nullptr);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ExecuteAreaAttack(AMingGoRTSUnit* Attacker, const FVector& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ExecuteLineAttack(AMingGoRTSUnit* Attacker, const FVector& Start, const FVector& End, float Width);

    // 技能系統
    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    void AddAbility(AMingGoRTSUnit* Unit, const FRTSAbilityData& Ability);

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    void RemoveAbility(AMingGoRTSUnit* Unit, const FString& AbilityName);

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    bool UseAbility(AMingGoRTSUnit* Unit, const FString& AbilityName, AMingGoRTSUnit* Target = nullptr);

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    TArray<FRTSAbilityData> GetUnitAbilities(AMingGoRTSUnit* Unit) const;

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    bool IsAbilityAvailable(AMingGoRTSUnit* Unit, const FString& AbilityName) const;

    // 狀態效果
    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    void ApplyStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName, float Duration, float Power);

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    void RemoveStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName);

    // 注意：AMingGoRTSUnit 是主專案類型，不能使用 UFUNCTION BlueprintCallable
    bool HasStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateStatusEffects(float DeltaTime);

    // 戰鬥平衡調整
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void BalanceCombatStats();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void AdjustUnitTypeAdvantages();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetDifficultyMultiplier(float Multiplier);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void OptimizeCombatPerformance();

    // 戰鬥更新
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateCombat(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ProcessCombatQueue();

    // 戰鬥統計
    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetActiveCombatCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetTotalDamageDealt() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetUnitsKilled() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ResetCombatStats();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "RTS Combat Events")
    FOnCombatStarted OnCombatStarted;

    UPROPERTY(BlueprintAssignable, Category = "RTS Combat Events")
    FOnCombatHit OnCombatHit;

    UPROPERTY(BlueprintAssignable, Category = "RTS Combat Events")
    FOnCombatMissed OnCombatMissed;

    UPROPERTY(BlueprintAssignable, Category = "RTS Combat Events")
    FOnUnitKilled OnUnitKilled;

    UPROPERTY(BlueprintAssignable, Category = "RTS Combat Events")
    FOnAbilityUsed OnAbilityUsed;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // 戰鬥數據
    UPROPERTY()
    TMap<AMingGoRTSUnit*, FRTSCombatStats> UnitCombatStats;

    UPROPERTY()
    TMap<AMingGoRTSUnit*, ERTSCombatStance> UnitCombatStances;

    // 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<AMingGoRTSUnit*, TArray<FRTSAbilityData>> UnitAbilities;

    // 注意：TMap<TMap> 不支持 UPROPERTY
    TMap<AMingGoRTSUnit*, TMap<FString, float>> StatusEffects;

    UPROPERTY()
    TMap<AMingGoRTSUnit*, AMingGoRTSUnit*> CombatPairs;

    // 戰鬥設置
    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    float BaseDamageMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    float BaseDefenseMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    float CriticalHitBonus = 1.5f;

    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    bool bEnableCriticalHits = true;

    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    bool bEnableDodging = true;

    UPROPERTY(BlueprintReadWrite, Category = "Combat Settings")
    bool bEnableBlocking = true;

    // 平衡調整參數
    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float DifficultyMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDamageModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDefenseModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSDamageType, float> DamageTypeEffectiveness;

    // 統計數據
    UPROPERTY()
    int32 TotalDamageDealt;

    UPROPERTY()
    int32 UnitsKilled;

    // 狀態
    UPROPERTY()
    bool bIsInitialized;

    // 內部函數
    void ProcessCombatPair(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, float DeltaTime);
    void ApplyCombatResult(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result);
    FString GenerateCombatResultMessage(const FRTSCombatResult& Result) const;
    float GetDamageTypeMultiplier(ERTSDamageType DamageType, ERTSDamageType DefenseType) const;
    void CleanupDeadUnits();
    void UpdateAbilityCooldowns(float DeltaTime);

private:
    // 輔助函數
    void NotifyCombatStarted(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType, ERTSDamageType DamageType);
    void NotifyCombatHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result);
    void NotifyCombatMissed(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);
    void NotifyUnitKilled(AMingGoRTSUnit* Victim, AMingGoRTSUnit* Killer);
    void NotifyAbilityUsed(AMingGoRTSUnit* Unit, const FString& AbilityName);
};
