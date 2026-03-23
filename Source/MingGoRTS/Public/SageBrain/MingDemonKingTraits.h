#pragma once

#include "CoreMinimal.h"
#include "MingSageCharacterSystem.h"
#include "MingDemonKingTraits.generated.h"

USTRUCT(BlueprintType)
struct FDemonAbilityEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FearMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DamageBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TargetTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCausesTerror;

    FDemonAbilityEffect()
    {
        AbilityName = "";
        FearMagnitude = 0.0f;
        DamageBonus = 0.0f;
        Duration = 0.0f;
        bCausesTerror = false;
        Description = "";
    }
};

USTRUCT(BlueprintType)
struct FDominanceAura
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuppressionRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EnemyMoraleDrain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AllyCompliance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RebellionSuppression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAffectsAllUnits;

    FDominanceAura()
    {
        SuppressionRadius = 0.0f;
        EnemyMoraleDrain = 0.0f;
        AllyCompliance = 0.0f;
        RebellionSuppression = 0.0f;
        bAffectsAllUnits = false;
    }
};

USTRUCT(BlueprintType)
struct FCorruptionEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CorruptionStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConversionChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LoyaltyDecay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PropagationSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPermanent;

    FCorruptionEffect()
    {
        CorruptionStrength = 0.0f;
        ConversionChance = 0.0f;
        LoyaltyDecay = 0.0f;
        PropagationSpeed = 0.0f;
        bPermanent = false;
    }
};

UCLASS(BlueprintType)
class MINGGORTS_API UMingDemonKingTraits : public UObject
{
    GENERATED_BODY()

public:
    UMingDemonKingTraits();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FSageCharacterTraits GetDemonKingDefaultTraits();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FDemonAbilityEffect CreateTerrorAuraEffect();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FDemonAbilityEffect CreateDarkCommandEffect();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FDemonAbilityEffect CreateDestructionOrderEffect();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FDominanceAura GetDemonDominanceAura();

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static FCorruptionEffect GetCorruptionEffect();

    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    static TArray<FString> GetDemonKingSpecialAbilities();

    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    static bool CanUseDemonAbility(const FString& AbilityName, int32 CharacterLevel);

    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    static float CalculateDemonEffectiveness(const FSageCharacterTraits& Traits, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static void ApplyDemonKingLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel);

    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    static FString GetDemonAbilityDescription(const FString& AbilityName);

    UFUNCTION(BlueprintPure, Category = "Demon Traits")
    static TArray<FString> GetDemonKingLevelUpRewards(int32 Level);

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static bool CanCorruptTarget(float TargetMoralAuthority, float DemonPower);

    UFUNCTION(BlueprintCallable, Category = "Demon Traits")
    static float CalculateFearDamage(float BaseDamage, float FearLevel);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnDemonAbilityUsed(const FString& AbilityName, const FDemonAbilityEffect& Effect);

    UFUNCTION(BlueprintImplementableEvent, Category = "Demon Traits")
    void OnCorruptionSpread(int32 AffectedUnits);

private:
    static const TMap<FString, int32> DemonAbilityLevelRequirements;
    static const TMap<FString, FDemonAbilityEffect> DemonAbilityEffects;
};
