#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceManager.generated.h"

// 目標數量
class IMingConsequenceCalculator;
class IMingConsequenceApplier;

// �M??�W?XUSTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionContext
{
    GENERATED_BODY()

    // �M?X?���H��
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionTitle;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    // �M?X???
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SelectedOptions;

    
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMonth;

    
    FString CurrentRegion;

    
    TArray<FString> InvolvedCharacters;

    
    TArray<FString> PreviousEvents;
};

// ??�v��??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingPoliticalConsequence
{
    GENERATED_BODY()

    // ���t摧毀��??
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactionRelationChanges;

    
    float PoliticalStabilityChange;

    // ??�v摧毀
    UPROPERTY(BlueprintReadOnly)
    float PoliticalReputationChange;

    // ??��??�v��??
    UPROPERTY(BlueprintReadOnly)
    float InternalPoliticalPressure;
};

// �x??��??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingMilitaryConsequence
{
    GENERATED_BODY()

    // �x??��??��??
    UPROPERTY(BlueprintReadOnly)
    int32 ManpowerChange;

    UPROPERTY(BlueprintReadOnly)
    int32 EquipmentChange;

    UPROPERTY(BlueprintReadOnly)
    float MoraleChange;

    
    TArray<FString> WarStatusChanges;

    // �x?X???��??
    UPROPERTY(BlueprintReadOnly)
    float MilitaryBudgetChange;
};

// �g??��??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEconomicConsequence
{
    GENERATED_BODY()

    // �g??��??��??
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceChanges;

    // �g?X???
    UPROPERTY(BlueprintReadOnly)
    float GDPChange;

    UPROPERTY(BlueprintReadOnly)
    float InflationRateChange;

    UPROPERTY(BlueprintReadOnly)
    float TaxRevenueChange;

    // 摧毀�]�I��??
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InfrastructureChanges;
};

// ��??��??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSocialConsequence
{
    GENERATED_BODY()

    
    float PublicSupportChange;

    
    float SocialStabilityChange;

    // ??�|���
    UPROPERTY(BlueprintReadOnly)
    float EducationLevelChange;

    // ??�@??�d
    UPROPERTY(BlueprintReadOnly)
    float PublicHealthChange;

    // ��?X???
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SocialMovements;
};

// ��??��??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceResult
{
    GENERATED_BODY()

    // ��?X?���H��
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceTitle;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // ��??��?X???
    UPROPERTY(BlueprintReadOnly)
    FMingPoliticalConsequence PoliticalConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingMilitaryConsequence MilitaryConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingEconomicConsequence EconomicConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingSocialConsequence SocialConsequences;

    // ��?X???�{��
    UPROPERTY(BlueprintReadOnly)
    float SeverityLevel;

    // ��?X??X???
    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;

    
    bool bIsImmediate;

    UPROPERTY(BlueprintReadOnly)
    bool bIsLongTerm;

    
    TArray<FString> TriggeredEvents;
};

/**
 * �M??��??��?X * �t�d�p?X??X?�M��?X???��{����??�M��?X */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionConsequenceManager : public UObject
{
    GENERATED_BODY()

public:
    // ��摧毀
    UMingDecisionConsequenceManager();

    // ???X?��??��
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void Initialize();

    
    void CalculateConsequences(const FMingDecisionContext& DecisionContext, FMingConsequenceResult& OutResult);

    // ��?X?��
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ApplyConsequences(const FMingConsequenceResult& ConsequenceResult);

    // ��?X?��
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void PreviewConsequences(const FMingDecisionContext& DecisionContext, TArray<FMingConsequenceResult>& OutPreviews);

    // ��?X?�P
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    bool UndoConsequences(const FString& ConsequenceID);

    // 摧毀��??���v
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    TArray<FMingConsequenceResult> GetConsequenceHistory() const { return ConsequenceHistory; }

    // �M����??���v
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ClearConsequenceHistory();

protected:
    // ��??�p?X
    TArray<TSharedPtr<IMingConsequenceCalculator>> ConsequenceCalculators;

    // ��?X?��X
    TArray<TSharedPtr<IMingConsequenceApplier>> ConsequenceAppliers;

    // ��??���v
    TArray<FMingConsequenceResult> ConsequenceHistory;

    // 目標數量�⾹
    void InitializeCalculators();

    // 故事重要性?��
    void InitializeAppliers();

    // 摧毀��??ID
    FString GenerateConsequenceID(const FMingDecisionContext& DecisionContext) const;

    // �p??��?X???�{��
    void CalculateOverallSeverity(FMingConsequenceResult& Result) const;

    // 摧毀��?X?�z
    void GenerateConsequenceDescription(FMingConsequenceResult& Result) const;

    // �O??��??�p??
    void RecordConsequenceCalculation(const FMingDecisionContext& Context, const FMingConsequenceResult& Result);

    // ��??��?X??X???
    bool CanApplyConsequences(const FMingConsequenceResult& ConsequenceResult) const;

private:
    // ??�_�w??�l??
    bool bInitialized = false;
};

