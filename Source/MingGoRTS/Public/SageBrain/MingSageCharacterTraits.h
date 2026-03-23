#pragma once

#include "CoreMinimal.h"
#include "MingSageCharacterSystem.h"
#include "MingSageCharacterTraits.generated.h"

USTRUCT(BlueprintType)
struct FSageAbilityEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EffectMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TargetTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    FSageAbilityEffect()
    {
        AbilityName = "";
        EffectMagnitude = 0.0f;
        Duration = 0.0f;
        Description = "";
    }
};

USTRUCT(BlueprintType)
struct FSageMoralInfluence
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoraleBoost;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LoyaltyIncrease;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ConversionChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 InfluenceRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAffectsEnemies;

    FSageMoralInfluence()
    {
        MoraleBoost = 0.0f;
        LoyaltyIncrease = 0.0f;
        ConversionChance = 0.0f;
        InfluenceRadius = 0;
        bAffectsEnemies = false;
    }
};

UCLASS(BlueprintType)
class MINGGORTS_API UMingSageTraits : public UObject
{
    GENERATED_BODY()

public:
    UMingSageTraits();

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static FSageCharacterTraits GetSageDefaultTraits();

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static FSageAbilityEffect CreateMoralAuraEffect();

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static FSageAbilityEffect CreateWisdomBlessingEffect();

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static FSageAbilityEffect CreateHolyJudgmentEffect();

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static FSageMoralInfluence GetSageMoralInfluence();

    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    static TArray<FString> GetSageSpecialAbilities();

    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    static bool CanUseSageAbility(const FString& AbilityName, int32 CharacterLevel);

    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    static float CalculateSageEffectiveness(const FSageCharacterTraits& Traits, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Traits")
    static void ApplySageLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel);

    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    static FString GetSageAbilityDescription(const FString& AbilityName);

    UFUNCTION(BlueprintPure, Category = "Sage Traits")
    static TArray<FString> GetSageLevelUpRewards(int32 Level);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Sage Traits")
    void OnSageAbilityUsed(const FString& AbilityName, const FSageAbilityEffect& Effect);

private:
    static const TMap<FString, int32> SageAbilityLevelRequirements;
    static const TMap<FString, FSageAbilityEffect> SageAbilityEffects;
};
