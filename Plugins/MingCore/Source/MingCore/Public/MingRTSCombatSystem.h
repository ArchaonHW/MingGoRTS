#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSCombatSystem.generated.h"

// ?çÂ??≤Ê?
class AMingGoRTSUnit;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSCombatType : uint8
{
    Melee,          // ËøëÊà∞
    Ranged,         // ?†Á?
    Artillery,      // ?´ÁÇÆ
    Siege,          // ?ªÂ?
    Naval           // Êµ∑Êà∞
};

UENUM(BlueprintType)
enum class ERTSDamageType : uint8
{
    Physical,       // ?©Á??∑ÂÆ≥
    Fire,           // ?´ÁÑ∞?∑ÂÆ≥
    Explosive,      // ?ÜÁÇ∏?∑ÂÆ≥
    Piercing,       // Á©øÂà∫?∑ÂÆ≥
    Magic,          // È≠îÊ??∑ÂÆ≥
    Poison          // ÊØíÁ??∑ÂÆ≥
};

UENUM(BlueprintType)
enum class ERTSCombatStance : uint8
{
    Aggressive,     // ?ªÊ?ÂßøÊ?
    Defensive,      // ?≤Á¶¶ÂßøÊ?
    Neutral,        // ‰∏≠Á?ÂßøÊ?
    Passive         // Ë¢´Â?ÂßøÊ?
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
 * RTS?∞È¨•Á≥ªÁµ±
 * ÁÆ°Á??Ä?âÊà∞È¨•Áõ∏?úÁ??èËºØ?åË?ÁÆ? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSCombatSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSCombatSystem();

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void InitializeCombatSystem(UMingRTSUnitManager* InUnitManager);

    // ?∞È¨•Áµ±Ë?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void SetUnitCombatStats(AMingGoRTSUnit* Unit, const FRTSCombatStats& CombatStats);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    FRTSCombatStats GetUnitCombatStats(AMingGoRTSUnit* Unit) const;

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void UpdateCombatStats(AMingGoRTSUnit* Unit, float DamageMultiplier = 1.0f, float DefenseMultiplier = 1.0f);

    // ?∞È¨•?∑Ë?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    FRTSCombatResult ExecuteCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    bool CanAttack(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    void StartCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType);

    void StopCombat(AMingGoRTSUnit* Unit);

    bool IsInCombat(AMingGoRTSUnit* Unit) const;

    // ?∑ÂÆ≥Ë®àÁ?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    float CalculateDamage(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float ApplyDamageReduction(float BaseDamage, ERTSDamageType DamageType, float DefenseRating);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool CheckHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool CheckCritical(AMingGoRTSUnit* Attacker);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool CheckDodge(AMingGoRTSUnit* Target);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool CheckBlock(AMingGoRTSUnit* Target);

    // ?∞È¨•ÂßøÊ?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void SetCombatStance(AMingGoRTSUnit* Unit, ERTSCombatStance Stance);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    ERTSCombatStance GetCombatStance(AMingGoRTSUnit* Unit) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDamageMultiplier(ERTSCombatStance Stance) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    float GetStanceDefenseMultiplier(ERTSCombatStance Stance) const;

    // ÁØÑÂ??ªÊ?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    TArray<AMingGoRTSUnit*> GetUnitsInArea(const FVector& Center, float Radius, AMingGoRTSUnit* ExcludedUnit = nullptr);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void ExecuteAreaAttack(AMingGoRTSUnit* Attacker, const FVector& Center, float Radius);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void ExecuteLineAttack(AMingGoRTSUnit* Attacker, const FVector& Start, const FVector& End, float Width);

    // ?Ä?ΩÁ≥ªÁµ?    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void AddAbility(AMingGoRTSUnit* Unit, const FRTSAbilityData& Ability);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void RemoveAbility(AMingGoRTSUnit* Unit, const FString& AbilityName);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool UseAbility(AMingGoRTSUnit* Unit, const FString& AbilityName, AMingGoRTSUnit* Target = nullptr);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    TArray<FRTSAbilityData> GetUnitAbilities(AMingGoRTSUnit* Unit) const;

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool IsAbilityAvailable(AMingGoRTSUnit* Unit, const FString& AbilityName) const;

    // ?Ä?ãÊ???    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void ApplyStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName, float Duration, float Power);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    void RemoveStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName);

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??Ω‰Ωø??UFUNCTION BlueprintCallable
    bool HasStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateStatusEffects(float DeltaTime);

    // ?∞È¨•Âπ≥Ë°°Ë™øÊï¥
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void BalanceCombatStats();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void AdjustUnitTypeAdvantages();

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void SetDifficultyMultiplier(float Multiplier);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void OptimizeCombatPerformance();

    // ?∞È¨•?¥Êñ∞
    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void UpdateCombat(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ProcessCombatQueue();

    // ?∞È¨•Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetActiveCombatCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetTotalDamageDealt() const;

    UFUNCTION(BlueprintPure, Category = "RTS Combat System")
    int32 GetUnitsKilled() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Combat System")
    void ResetCombatStats();

    // ‰∫ã‰ª∂
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
    // Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ?∞È¨•?∏Ê?
    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
    TMap<AMingGoRTSUnit*, FRTSCombatStats> UnitCombatStats;

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
    TMap<AMingGoRTSUnit*, ERTSCombatStance> UnitCombatStances;

    // Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<AMingGoRTSUnit*, TArray<FRTSAbilityData>> UnitAbilities;

    // Ê≥®Ê?ÔºöTMap<TMap> ‰∏çÊîØ??UPROPERTY
    TMap<AMingGoRTSUnit*, TMap<FString, float>> StatusEffects;

    // Ê≥®Ê?ÔºöAMingGoRTSUnit ?Ø‰∏ªÂ∞àÊ?È°ûÂ?Ôºå‰??©Â? UPROPERTY
    TMap<AMingGoRTSUnit*, AMingGoRTSUnit*> CombatPairs;

    // ?∞È¨•Ë®≠ÁΩÆ
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

    // Âπ≥Ë°°Ë™øÊï¥?ÉÊï∏
    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    float DifficultyMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDamageModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSUnitType, float> UnitTypeDefenseModifiers;

    UPROPERTY(BlueprintReadWrite, Category = "Balance Settings")
    TMap<ERTSDamageType, float> DamageTypeEffectiveness;

    // Áµ±Ë??∏Ê?
    UPROPERTY()
    int32 TotalDamageDealt;

    UPROPERTY()
    int32 UnitsKilled;

    // ?Ä??    UPROPERTY()
    bool bIsInitialized;

    // ?ßÈÉ®?ΩÊï∏
    void ProcessCombatPair(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, float DeltaTime);
    void ApplyCombatResult(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result);
    FString GenerateCombatResultMessage(const FRTSCombatResult& Result) const;
    float GetDamageTypeMultiplier(ERTSDamageType DamageType, ERTSDamageType DefenseType) const;
    void CleanupDeadUnits();
    void UpdateAbilityCooldowns(float DeltaTime);

private:
    // ËºîÂä©?ΩÊï∏
    void NotifyCombatStarted(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType, ERTSDamageType DamageType);
    void NotifyCombatHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result);
    void NotifyCombatMissed(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target);
    void NotifyUnitKilled(AMingGoRTSUnit* Victim, AMingGoRTSUnit* Killer);
    void NotifyAbilityUsed(AMingGoRTSUnit* Unit, const FString& AbilityName);
};
