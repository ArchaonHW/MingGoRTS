#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceManager.generated.h"

// ?å??²æ?
class IMingConsequenceCalculator;
class IMingConsequenceApplier;

// æ±ºç?ä¸Šä???USTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionContext
{
    GENERATED_BODY()

    // æ±ºç??ºæœ¬ä¿¡æ¯
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionTitle;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    // æ±ºç??¸é?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SelectedOptions;

    // ?‚é?ä¸Šä???    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMonth;

    // ?°ç?ä¸Šä???    UPROPERTY(BlueprintReadOnly)
    FString CurrentRegion;

    // è§’è‰²ä¸Šä???    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedCharacters;

    // æ­·å²ä¸Šä???    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreviousEvents;
};

// ?¿æ²»å¾Œæ?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingPoliticalConsequence
{
    GENERATED_BODY()

    // æ´¾ç³»?œä?è®Šå?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactionRelationChanges;

    // ?¿æ²»ç©©å???    UPROPERTY(BlueprintReadOnly)
    float PoliticalStabilityChange;

    // ?¿æ²»?²æ?
    UPROPERTY(BlueprintReadOnly)
    float PoliticalReputationChange;

    // ?§éƒ¨?¿æ²»å£“å?
    UPROPERTY(BlueprintReadOnly)
    float InternalPoliticalPressure;
};

// è»ä?å¾Œæ?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingMilitaryConsequence
{
    GENERATED_BODY()

    // è»ä?å¯¦å?è®Šå?
    UPROPERTY(BlueprintReadOnly)
    int32 ManpowerChange;

    UPROPERTY(BlueprintReadOnly)
    int32 EquipmentChange;

    UPROPERTY(BlueprintReadOnly)
    float MoraleChange;

    // ?°çˆ­?€?‹è???    UPROPERTY(BlueprintReadOnly)
    TArray<FString> WarStatusChanges;

    // è»ä??ç?è®Šå?
    UPROPERTY(BlueprintReadOnly)
    float MilitaryBudgetChange;
};

// ç¶“æ?å¾Œæ?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEconomicConsequence
{
    GENERATED_BODY()

    // ç¶“æ?è³‡æ?è®Šå?
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceChanges;

    // ç¶“æ??‡æ?
    UPROPERTY(BlueprintReadOnly)
    float GDPChange;

    UPROPERTY(BlueprintReadOnly)
    float InflationRateChange;

    UPROPERTY(BlueprintReadOnly)
    float TaxRevenueChange;

    // ?ºç?è¨­æ–½è®Šå?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InfrastructureChanges;
};

// ç¤¾æ?å¾Œæ?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSocialConsequence
{
    GENERATED_BODY()

    // æ°‘çœ¾?¯æ?åº?    UPROPERTY(BlueprintReadOnly)
    float PublicSupportChange;

    // ç¤¾æ?ç©©å???    UPROPERTY(BlueprintReadOnly)
    float SocialStabilityChange;

    // ?™è‚²æ°´å¹³
    UPROPERTY(BlueprintReadOnly)
    float EducationLevelChange;

    // ?¬å…±?¥åº·
    UPROPERTY(BlueprintReadOnly)
    float PublicHealthChange;

    // ç¤¾æ??‹å?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SocialMovements;
};

// å¾Œæ?çµæ?
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceResult
{
    GENERATED_BODY()

    // å¾Œæ??ºæœ¬ä¿¡æ¯
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceTitle;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // å¾Œæ?é¡å??†é?
    UPROPERTY(BlueprintReadOnly)
    FMingPoliticalConsequence PoliticalConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingMilitaryConsequence MilitaryConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingEconomicConsequence EconomicConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingSocialConsequence SocialConsequences;

    // å¾Œæ??´é?ç¨‹åº¦
    UPROPERTY(BlueprintReadOnly)
    float SeverityLevel;

    // å¾Œæ??ç??‚é?
    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;

    // å¾Œæ??¯è???    UPROPERTY(BlueprintReadOnly)
    bool bIsImmediate;

    UPROPERTY(BlueprintReadOnly)
    bool bIsLongTerm;

    // å¾Œæ?è§¸ç™¼?„ä?ä»?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggeredEvents;
};

/**
 * æ±ºç?å¾Œæ?ç®¡ç??? * è² è²¬è¨ˆç??Œæ??¨æ±ºç­–å??œï?å¯¦ç¾å®Œæ•´?„æ±ºç­–é??? */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionConsequenceManager : public UObject
{
    GENERATED_BODY()

public:
    // å»ºæ?å­?    UMingDecisionConsequenceManager();

    // ?å??–ç®¡?†å™¨
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void Initialize();

    // å¾Œæ?è¨ˆç?ä¸»ä???    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateConsequences(const FMingDecisionContext& DecisionContext, FMingConsequenceResult& OutResult);

    // å¾Œæ??‰ç”¨
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ApplyConsequences(const FMingConsequenceResult& ConsequenceResult);

    // å¾Œæ??è¦½
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void PreviewConsequences(const FMingDecisionContext& DecisionContext, TArray<FMingConsequenceResult>& OutPreviews);

    // å¾Œæ??¤éŠ·
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    bool UndoConsequences(const FString& ConsequenceID);

    // ?²å?å¾Œæ?æ­·å²
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    TArray<FMingConsequenceResult> GetConsequenceHistory() const { return ConsequenceHistory; }

    // æ¸…é™¤å¾Œæ?æ­·å²
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ClearConsequenceHistory();

protected:
    // å¾Œæ?è¨ˆç???    TArray<TSharedPtr<IMingConsequenceCalculator>> ConsequenceCalculators;

    // å¾Œæ??‰ç”¨??    TArray<TSharedPtr<IMingConsequenceApplier>> ConsequenceAppliers;

    // å¾Œæ?æ­·å²
    TArray<FMingConsequenceResult> ConsequenceHistory;

    // ?å??–è?ç®—å™¨
    void InitializeCalculators();

    // ?å??–æ??¨å™¨
    void InitializeAppliers();

    // ?Ÿæ?å¾Œæ?ID
    FString GenerateConsequenceID(const FMingDecisionContext& DecisionContext) const;

    // è¨ˆç?ç¶œå??´é?ç¨‹åº¦
    void CalculateOverallSeverity(FMingConsequenceResult& Result) const;

    // ?Ÿæ?å¾Œæ??è¿°
    void GenerateConsequenceDescription(FMingConsequenceResult& Result) const;

    // è¨˜é?å¾Œæ?è¨ˆç?
    void RecordConsequenceCalculation(const FMingDecisionContext& Context, const FMingConsequenceResult& Result);

    // é©—è?å¾Œæ??¯æ??¨æ€?    bool CanApplyConsequences(const FMingConsequenceResult& ConsequenceResult) const;

private:
    // ?¯å¦å·²å?å§‹å?
    bool bInitialized = false;
};
