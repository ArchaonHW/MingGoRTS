#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCombatSystem.generated.h"

// ?��X��?
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSCombatType : uint8
{
    Melee,          // 近戰
    Ranged,         // ?��?
    Artillery,      // ?�炮
    Siege,          // ?��?
    Naval           // 海戰
};

UENUM(BlueprintType)
enum class ERTSDamageType : uint8
{
    Physical,       // ?��X�害
    Fire,           // ?�焰?�害
    Explosive,      // ?�炸?�害
    Piercing,       // 穿刺?�害
    Magic,          // 魔�X�害
    Poison          // 毒�X�害
};

UENUM(BlueprintType)
enum class ERTSCombatStance : uint8
{
    Aggressive,     // ?��?姿�?
    Defensive,      // ?�禦姿�?
    Neutral,        // 中�?姿�?
    Passive         // 被�?姿�?
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
        ResultMessage = TEXT(""};
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
        AbilityName = TEXT(""};
        Description = TEXT(""};
        Cooldown = 5.0f;
        CurrentCooldown = 0.0f;
        ManaCost = 10.0f;
        Range = 100.0f;
        bIsAvailable = true;
        bIsActive = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCombatStarted, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target, ERTSCombatType, CombatType, ERTSDamageType, DamageType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCombatHit, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target, const FRTSCombatResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCombatMissed, AMingGoRTSUnit*, Attacker, AMingGoRTSUnit*, Target};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitKilled, AMingGoRTSUnit*, Victim, AMingGoRTSUnit*, Killer};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityUsed, AMingGoRTSUnit*, Unit, const FString&, AbilityName};

/**
 * RTS?�鬥系統
 * 管�X�?�戰鬥相?��X�輯?��?�? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSCombatSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCombatSystem(};

    // ?��X    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void InitializeCombatSystem(UMingRTSUnitManager* InUnitManager};

    // ?�鬥統�?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void SetUnitCombatStats(AMingGoRTSUnit* Unit, const FRTSCombatStats& CombatStats) {};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    FRTSCombatStats GetUnitCombatStats(AMingGoRTSUnit* Unit) const;

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void UpdateCombatStats(AMingGoRTSUnit* Unit, float DamageMultiplier = 1.0f, float DefenseMultiplier = 1.0f};

    // ?�鬥?��?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    FRTSCombatResult ExecuteCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target};

    bool CanAttack(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target};

    void StartCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType};

    void StopCombat(AMingGoRTSUnit* Unit};

    bool IsInCombat(AMingGoRTSUnit* Unit) const;

    // ?�害計�?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    float CalculateDamage(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target};

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float ApplyDamageReduction(float BaseDamage, ERTSDamageType DamageType, float DefenseRating};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool CheckHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool CheckCritical(AMingGoRTSUnit* Attacker};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool CheckDodge(AMingGoRTSUnit* Target};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool CheckBlock(AMingGoRTSUnit* Target};

    // ?�鬥姿�?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void SetCombatStance(AMingGoRTSUnit* Unit, ERTSCombatStance Stance};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    ERTSCombatStance GetCombatStance(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDamageMultiplier(ERTSCombatStance Stance) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDefenseMultiplier(ERTSCombatStance Stance) const;

    // 範�X��?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    TArray<AMingGoRTSUnit*> GetUnitsInArea(const FVector& Center, float Radius, AMingGoRTSUnit* ExcludedUnit = nullptr};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void ExecuteAreaAttack(AMingGoRTSUnit* Attacker, const FVector& Center, float Radius};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void ExecuteLineAttack(AMingGoRTSUnit* Attacker, const FVector& Start, const FVector& End, float Width};

    // ?�?�系�?    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void AddAbility(AMingGoRTSUnit* Unit, const FRTSAbilityData& Ability) {};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void RemoveAbility(AMingGoRTSUnit* Unit, const FString& AbilityName) {};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool UseAbility(AMingGoRTSUnit* Unit, const FString& AbilityName, AMingGoRTSUnit* Target = nullptr};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    TArray<FRTSAbilityData> GetUnitAbilities(AMingGoRTSUnit* Unit) const;

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool IsAbilityAvailable(AMingGoRTSUnit* Unit, const FString& AbilityName) const;

    // ?�?��X    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void ApplyStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName, float Duration, float Power};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    void RemoveStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) {};

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X�使XUFUNCTION BlueprintCallable
    bool HasStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateStatusEffects(float DeltaTime};

    // ?�鬥平衡調整
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void BalanceCombatStats(};

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void AdjustUnitTypeAdvantages(};

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetDifficultyMultiplier(float Multiplier};

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void OptimizeCombatPerformance(};

    // ?�鬥?�新
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateCombat(float DeltaTime};

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ProcessCombatQueue(};

    // ?�鬥統�?
    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetActiveCombatCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetTotalDamageDealt() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetUnitsKilled() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ResetCombatStats(};

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

    // ?�鬥?��?
    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X��? UPROPERTY
    TMap<AMingGoRTSUnit*, FRTSCombatStats> UnitCombatStats;

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X��? UPROPERTY
    TMap<AMingGoRTSUnit*, ERTSCombatStance> UnitCombatStances;

    // 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<AMingGoRTSUnit*, TArray<FRTSAbilityData>> UnitAbilities;

    // 注�?：TMap<TMap> 不支XUPROPERTY
    TMap<AMingGoRTSUnit*, TMap<FString, float>> StatusEffects;

    // 注�?：AMingGoRTSUnit ?�主專�?類�?，�X��? UPROPERTY
    TMap<AMingGoRTSUnit*, AMingGoRTSUnit*> CombatPairs;

    // ?�鬥設置
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

    // 平衡調整?�數
    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float DifficultyMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDamageModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDefenseModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSDamageType, float> DamageTypeEffectiveness;

    // 統�X��?
    UPROPERTY()
    int32 TotalDamageDealt;

    UPROPERTY()
    int32 UnitsKilled;

    // ?�X    UPROPERTY()
    bool bIsInitialized;

    // ?�部?�數
    void ProcessCombatPair(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, float DeltaTime};
    void ApplyCombatResult(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result) {};
    FString GenerateCombatResultMessage(const FRTSCombatResult& Result) const;
    float GetDamageTypeMultiplier(ERTSDamageType DamageType, ERTSDamageType DefenseType) const;
    void CleanupDeadUnits(};
    void UpdateAbilityCooldowns(float DeltaTime};

private:
    // 輔助?�數
    void NotifyCombatStarted(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType, ERTSDamageType DamageType};
    void NotifyCombatHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result) {};
    void NotifyCombatMissed(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target};
    void NotifyUnitKilled(AMingGoRTSUnit* Victim, AMingGoRTSUnit* Killer};
    void NotifyAbilityUsed(AMingGoRTSUnit* Unit, const FString& AbilityName) {};
};

