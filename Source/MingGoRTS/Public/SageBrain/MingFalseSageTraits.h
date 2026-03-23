#pragma once

#include "CoreMinimal.h"
#include "MingSageCharacterSystem.h"
#include "MingFalseSageTraits.generated.h"

USTRUCT(BlueprintType)
struct FDeceptionAbility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString AbilityName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DeceptionPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DetectionResistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TargetTypes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCausesParanoia;

    FDeceptionAbility()
    {
        AbilityName = "";
        DeceptionPower = 0.0f;
        DetectionResistance = 0.0f;
        Duration = 0.0f;
        bCausesParanoia = false;
        Description = "";
    }
};

USTRUCT(BlueprintType)
struct FConspiracyEffect
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ManipulationStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TrustErosion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuspicionSpread;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HiddenInfluence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUntraceable;

    FConspiracyEffect()
    {
        ManipulationStrength = 0.0f;
        TrustErosion = 0.0f;
        SuspicionSpread = 0.0f;
        HiddenInfluence = 0.0f;
        bUntraceable = false;
    }
};

USTRUCT(BlueprintType)
struct FBackstabBonus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SurpriseDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CriticalChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BetrayalEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReputationDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHiddenAttack;

    FBackstabBonus()
    {
        SurpriseDamage = 0.0f;
        CriticalChance = 0.0f;
        BetrayalEffect = 0.0f;
        ReputationDamage = 0.0f;
        bHiddenAttack = false;
    }
};

UCLASS(BlueprintType)
class MINGGORTS_API UMingFalseSageTraits : public UObject
{
    GENERATED_BODY()

public:
    UMingFalseSageTraits();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FSageCharacterTraits GetFalseSageDefaultTraits();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FDeceptionAbility CreateFalsePietyAbility();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FDeceptionAbility CreateConspiracyAbility();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FDeceptionAbility CreateBetrayalAbility();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FConspiracyEffect GetConspiracyEffect();

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static FBackstabBonus GetBackstabBonus();

    UFUNCTION(BlueprintPure, Category = "False Sage Traits")
    static TArray<FString> GetFalseSageSpecialAbilities();

    UFUNCTION(BlueprintPure, Category = "False Sage Traits")
    static bool CanUseFalseSageAbility(const FString& AbilityName, int32 CharacterLevel);

    UFUNCTION(BlueprintPure, Category = "False Sage Traits")
    static float CalculateFalseSageEffectiveness(const FSageCharacterTraits& Traits, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static void ApplyFalseSageLevelUpBonus(FSageCharacterTraits& Traits, int32 NewLevel);

    UFUNCTION(BlueprintPure, Category = "False Sage Traits")
    static FString GetFalseSageAbilityDescription(const FString& AbilityName);

    UFUNCTION(BlueprintPure, Category = "False Sage Traits")
    static TArray<FString> GetFalseSageLevelUpRewards(int32 Level);

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static bool CanDeceiveTarget(float TargetWisdom, float DeceptionPower);

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static float CalculateBackstabDamage(float BaseDamage, float SurpriseFactor);

    UFUNCTION(BlueprintCallable, Category = "False Sage Traits")
    static bool DetectConspiracy(float InvestigationLevel, float ConspiracyComplexity);

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "False Sage Traits")
    void OnFalseSageAbilityUsed(const FString& AbilityName, const FDeceptionAbility& Effect);

    UFUNCTION(BlueprintImplementableEvent, Category = "False Sage Traits")
    void OnConspiracyDiscovered(const FString& ConspiracyName);

private:
    static const TMap<FString, int32> FalseSageAbilityLevelRequirements;
    static const TMap<FString, FDeceptionAbility> FalseSageAbilityEffects;
};
