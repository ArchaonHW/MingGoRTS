#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceConflictResolver.generated.h"

// ??X??X??X
UENUM(BlueprintType)
enum class EMingConsequenceConflictType : uint8
{
    None,
    PoliticalConflict,     // X?v??X??X
    MilitaryConflict,      // ?xX??X??X
    EconomicConflict,      // ?gX??X??X
    SocialConflict,        // ??X??X??X
    CrossDomainConflict,   // ??????
};

// ??X??X
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceConflict
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ConflictID;

    UPROPERTY(BlueprintReadOnly)
    EMingConsequenceConflictType ConflictType;

    UPROPERTY(BlueprintReadOnly)
    FString ConflictDescription;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConflictingCalculators;

    UPROPERTY(BlueprintReadOnly)
    float ConflictSeverity;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ResolutionOptions;

    UPROPERTY(BlueprintReadOnly)
    FString RecommendedResolution;
};

// ??X????
UENUM(BlueprintType)
enum class EMingConflictResolutionStrategy : uint8
{
    PriorityBased,         // X??X    WeightedAverage,      // X??X
    DomainOverride,       // X??X
    Conservative,         // ?OX????
    Aggressive,           // ?EX    Manual               // X
};

/**
 * ??X??X * ???X??MX????X */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceConflictResolver : public UObject
{
    GENERATED_BODY()

public:
    // ??X    UMingConsequenceConflictResolver();

    // ?????X??X
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool DetectConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // X??X
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ResolveConflicts(TArray<FMingConsequenceResult>& Results, EMingConflictResolutionStrategy Strategy = EMingConflictResolutionStrategy::PriorityBased};

    // X??X
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ApplyConflictResolution(const FMingConsequenceConflict& Conflict, FMingConsequenceResult& Result};

    // X??X????
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    EMingConflictResolutionStrategy GetRecommendedStrategy(const FMingConsequenceConflict& Conflict};

    // ??X??X
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool ValidateResolution(const TArray<FMingConsequenceResult>& Results};

protected:
    // ???X?v??X
    void DetectPoliticalConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // ????xX??X
    void DetectMilitaryConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // ????gX??X
    void DetectEconomicConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // ?????X??X
    void DetectSocialConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // ?????X    void DetectCrossDomainConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts};

    // X??X???X    void ResolvePriorityBased(TArray<FMingConsequenceResult>& Results};

    // X??X
    void ResolveWeightedAverage(TArray<FMingConsequenceResult>& Results};

    // X??X
    void ResolveDomainOverride(TArray<FMingConsequenceResult>& Results};

    // ?OX????X
    void ResolveConservative(TArray<FMingConsequenceResult>& Results};

    // ?EX??X    void ResolveAggressive(TArray<FMingConsequenceResult>& Results};

    // ?pX??X?{??
    float CalculateConflictSeverity(const FMingConsequenceConflict& Conflict};

    // X
    void GenerateResolutionOptions(const FMingConsequenceConflict& Conflict, TArray<FString>& OutOptions};

    // X??X
    void GenerateRecommendedResolution(const FMingConsequenceConflict& Conflict, FString& OutResolution};

private:
    // ?pX?uX
    TMap<FString, float> CalculatorPriorities;

    // ??X???X    static constexpr float CONFLICT_THRESHOLD = 0.3f;

    // X?uX
    void InitializePriorities();
};

