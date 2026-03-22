#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Decision/MingDecisionConsequenceManager.h"
#include "MingConsequenceConflictResolver.generated.h"

// å¾Œæ?è¡ç?é¡å?
UENUM(BlueprintType)
enum class EMingConsequenceConflictType : uint8
{
    None,
    PoliticalConflict,     // ?¿æ²»å¾Œæ?è¡ç?
    MilitaryConflict,      // è»ä?å¾Œæ?è¡ç?
    EconomicConflict,      // ç¶“æ?å¾Œæ?è¡ç?
    SocialConflict,        // ç¤¾æ?å¾Œæ?è¡ç?
    CrossDomainConflict   // è·¨é??Ÿè?çª?};

// å¾Œæ?è¡ç?
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

// è¡ç?è§?±ºç­–ç•¥
UENUM(BlueprintType)
enum class EMingConflictResolutionStrategy : uint8
{
    PriorityBased,         // ?ºæ–¼?ªå?ç´?    WeightedAverage,      // ? æ?å¹³å?
    DomainOverride,       // ?˜å?è¦†è?
    Conservative,         // ä¿å?ç­–ç•¥
    Aggressive,           // æ¿€?²ç???    Manual               // ?‹å?è§?±º
};

/**
 * å¾Œæ?è¡ç?è§?±º?? * æª¢æ¸¬?Œè§£æ±ºå??‹è?ç®—å™¨ä¹‹é??„å??œè?çª? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingConsequenceConflictResolver : public UObject
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingConsequenceConflictResolver();

    // æª¢æ¸¬å¾Œæ?è¡ç?
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool DetectConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // è§?±ºè¡ç?
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ResolveConflicts(TArray<FMingConsequenceResult>& Results, EMingConflictResolutionStrategy Strategy = EMingConflictResolutionStrategy::PriorityBased);

    // è§?±º?¹å?è¡ç?
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    void ApplyConflictResolution(const FMingConsequenceConflict& Conflict, FMingConsequenceResult& Result);

    // ?²å??¨è–¦è§?±ºç­–ç•¥
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    EMingConflictResolutionStrategy GetRecommendedStrategy(const FMingConsequenceConflict& Conflict);

    // é©—è?è§?±ºçµæ?
    UFUNCTION(BlueprintCallable, Category = "Ming|Conflict")
    bool ValidateResolution(const TArray<FMingConsequenceResult>& Results);

protected:
    // æª¢æ¸¬?¿æ²»è¡ç?
    void DetectPoliticalConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // æª¢æ¸¬è»ä?è¡ç?
    void DetectMilitaryConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // æª¢æ¸¬ç¶“æ?è¡ç?
    void DetectEconomicConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // æª¢æ¸¬ç¤¾æ?è¡ç?
    void DetectSocialConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // æª¢æ¸¬è·¨é??Ÿè?çª?    void DetectCrossDomainConflicts(const TArray<FMingConsequenceResult>& Results, TArray<FMingConsequenceConflict>& OutConflicts);

    // ?ºæ–¼?ªå?ç´šè§£æ±?    void ResolvePriorityBased(TArray<FMingConsequenceResult>& Results);

    // ? æ?å¹³å?è§?±º
    void ResolveWeightedAverage(TArray<FMingConsequenceResult>& Results);

    // ?˜å?è¦†è?è§?±º
    void ResolveDomainOverride(TArray<FMingConsequenceResult>& Results);

    // ä¿å?ç­–ç•¥è§?±º
    void ResolveConservative(TArray<FMingConsequenceResult>& Results);

    // æ¿€?²ç??¥è§£æ±?    void ResolveAggressive(TArray<FMingConsequenceResult>& Results);

    // è¨ˆç?è¡ç??´é?ç¨‹åº¦
    float CalculateConflictSeverity(const FMingConsequenceConflict& Conflict);

    // ?Ÿæ?è§?±º?¸é?
    void GenerateResolutionOptions(const FMingConsequenceConflict& Conflict, TArray<FString>& OutOptions);

    // ?Ÿæ??¨è–¦è§?±º?¹æ?
    void GenerateRecommendedResolution(const FMingConsequenceConflict& Conflict, FString& OutResolution);

private:
    // è¨ˆç??¨å„ª?ˆç?? å?
    TMap<FString, float> CalculatorPriorities;

    // è¡ç?æª¢æ¸¬?¾å€?    static constexpr float CONFLICT_THRESHOLD = 0.3f;

    // ?å??–å„ª?ˆç?
    void InitializePriorities();
};
