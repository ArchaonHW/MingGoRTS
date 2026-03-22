#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCombatSystem.generated.h"

// 目標數量
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSCombatType: uint8 {
    Melee,          // ���
    Ranged,         // 摧毀
    Artillery,      // ??��
    Siege,          // 摧毀
    Naval           // ����
};

UENUM(BlueprintType)
enum class ERTSDamageType: uint8 {
    Physical,       // ???X?�`
    Fire,           // ??�K??�`
    Explosive,      // ??��??�`
    Piercing,       // ���??�`
    Magic,          // �]?X?�`
    Poison          // �r?X?�`
};

UENUM(BlueprintType)
enum class ERTSCombatStance: uint8 {
    Aggressive,     // 摧毀��??
    Defensive,      // ??�m��??
    Neutral,        // ��??��??
    Passive         // �Q??��??
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
);







/**
 * RTS??���t��
 * ��?X???�԰���???X?��摧毀?? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSCombatSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCombatSystem();

    
    void InitializeCombatSystem(UMingRTSUnitManager* InUnitManager);

    // ??����??
    
    void SetUnitCombatStats(AMingGoRTSUnit* Unit, const FRTSCombatStats& CombatStats) {};

    
    FRTSCombatStats GetUnitCombatStats(AMingGoRTSUnit* Unit) const;

    
    void UpdateCombatStats(AMingGoRTSUnit* Unit, float DamageMultiplier = 1.0f, float DefenseMultiplier = 1.0f);

    // ??��摧毀
    
    FRTSCombatResult ExecuteCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    bool CanAttack(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    void StartCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType);

    void StopCombat(AMingGoRTSUnit* Unit);

    bool IsInCombat(AMingGoRTSUnit* Unit) const;

    // ??�`�p??
    
    float CalculateDamage(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float ApplyDamageReduction(float BaseDamage, ERTSDamageType DamageType, float DefenseRating);

    
    bool CheckHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    
    bool CheckCritical(AMingGoRTSUnit* Attacker);

    
    bool CheckDodge(AMingGoRTSUnit* Target);

    
    bool CheckBlock(AMingGoRTSUnit* Target);

    // ??����??
    
    void SetCombatStance(AMingGoRTSUnit* Unit, ERTSCombatStance Stance);

    
    ERTSCombatStance GetCombatStance(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDamageMultiplier(ERTSCombatStance Stance) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDefenseMultiplier(ERTSCombatStance Stance) const;

    // �d?X???
    
    TArray<AMingGoRTSUnit*> GetUnitsInArea(const FVector& Center, float Radius, AMingGoRTSUnit* ExcludedUnit = nullptr);

    
    void ExecuteAreaAttack(AMingGoRTSUnit* Attacker, const FVector& Center, float Radius);

    
    void ExecuteLineAttack(AMingGoRTSUnit* Attacker, const FVector& Start, const FVector& End, float Width);

    
    void AddAbility(AMingGoRTSUnit* Unit, const FRTSAbilityData& Ability) {};

    
    void RemoveAbility(AMingGoRTSUnit* Unit, const FString& AbilityName) {};

    
    bool UseAbility(AMingGoRTSUnit* Unit, const FString& AbilityName, AMingGoRTSUnit* Target = nullptr);

    
    TArray<FRTSAbilityData> GetUnitAbilities(AMingGoRTSUnit* Unit) const;

    
    bool IsAbilityAvailable(AMingGoRTSUnit* Unit, const FString& AbilityName) const;

    
    void ApplyStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName, float Duration, float Power);

    
    void RemoveStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) {};

    
    bool HasStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateStatusEffects(float DeltaTime);

    // ??�����Žվ�
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void BalanceCombatStats();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void AdjustUnitTypeAdvantages();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetDifficultyMultiplier(float Multiplier);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void OptimizeCombatPerformance();

    // ??��??�s
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateCombat(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ProcessCombatQueue();

    // ??����??
    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetActiveCombatCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetTotalDamageDealt() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetUnitsKilled() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ResetCombatStats();

    // �ƥ�
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
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ??��摧毀
    
    TMap<AMingGoRTSUnit*, FRTSCombatStats> UnitCombatStats;

    
    TMap<AMingGoRTSUnit*, ERTSCombatStance> UnitCombatStances;

    
    TMap<AMingGoRTSUnit*, TArray<FRTSAbilityData>> UnitAbilities;

    
    TMap<AMingGoRTSUnit*, TMap<FString, float>> StatusEffects;

    
    TMap<AMingGoRTSUnit*, AMingGoRTSUnit*> CombatPairs;

    // ??���]�m
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

    // ���Žվ�??��
    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float DifficultyMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDamageModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDefenseModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSDamageType, float> DamageTypeEffectiveness;

    // ��?X???
    UPROPERTY()
    int32 TotalDamageDealt;

    UPROPERTY()
    int32 UnitsKilled;

    
    bool bIsInitialized;

    // ??��??��
    void ProcessCombatPair(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, float DeltaTime);
    void ApplyCombatResult(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result) {};
    FString GenerateCombatResultMessage(const FRTSCombatResult& Result) const;
    float GetDamageTypeMultiplier(ERTSDamageType DamageType, ERTSDamageType DefenseType) const;
    void CleanupDeadUnits();
    void UpdateAbilityCooldowns(float DeltaTime);

private:
    // ���U??��
    void NotifyCombatStarted(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType, ERTSDamageType DamageType);
    void NotifyCombatHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result) {};
    void NotifyCombatMissed(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);
    void NotifyUnitKilled(AMingGoRTSUnit* Victim, AMingGoRTSUnit* Killer);
    void NotifyAbilityUsed(AMingGoRTSUnit* Unit, const FString& AbilityName) {};
};

