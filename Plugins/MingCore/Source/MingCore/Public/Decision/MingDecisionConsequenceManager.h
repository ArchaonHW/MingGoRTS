#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceManager.generated.h"

// ???X???
class IMingConsequenceCalculator;
class IMingConsequenceApplier;

// 決??上?XUSTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionContext
{
    GENERATED_BODY()

    // 決?X?本信息
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionTitle;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    // 決?X???
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SelectedOptions;

    
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMonth;

    
    FString CurrentRegion;

    
    TArray<FString> InvolvedCharacters;

    
    TArray<FString> PreviousEvents;
};

// ??治後??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingPoliticalConsequence
{
    GENERATED_BODY()

    // 派系????變??
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactionRelationChanges;

    
    float PoliticalStabilityChange;

    // ??治????
    UPROPERTY(BlueprintReadOnly)
    float PoliticalReputationChange;

    // ??部??治壓??
    UPROPERTY(BlueprintReadOnly)
    float InternalPoliticalPressure;
};

// 軍??後??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingMilitaryConsequence
{
    GENERATED_BODY()

    // 軍??實??變??
    UPROPERTY(BlueprintReadOnly)
    int32 ManpowerChange;

    UPROPERTY(BlueprintReadOnly)
    int32 EquipmentChange;

    UPROPERTY(BlueprintReadOnly)
    float MoraleChange;

    
    TArray<FString> WarStatusChanges;

    // 軍?X???變??
    UPROPERTY(BlueprintReadOnly)
    float MilitaryBudgetChange;
};

// 經??後??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEconomicConsequence
{
    GENERATED_BODY()

    // 經??資??變??
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceChanges;

    // 經?X???
    UPROPERTY(BlueprintReadOnly)
    float GDPChange;

    UPROPERTY(BlueprintReadOnly)
    float InflationRateChange;

    UPROPERTY(BlueprintReadOnly)
    float TaxRevenueChange;

    // ????設施變??
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InfrastructureChanges;
};

// 社??後??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSocialConsequence
{
    GENERATED_BODY()

    
    float PublicSupportChange;

    
    float SocialStabilityChange;

    // ??育水平
    UPROPERTY(BlueprintReadOnly)
    float EducationLevelChange;

    // ??共??康
    UPROPERTY(BlueprintReadOnly)
    float PublicHealthChange;

    // 社?X???
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SocialMovements;
};

// 後??結??
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceResult
{
    GENERATED_BODY()

    // 後?X?本信息
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceTitle;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // 後??類?X???
    UPROPERTY(BlueprintReadOnly)
    FMingPoliticalConsequence PoliticalConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingMilitaryConsequence MilitaryConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingEconomicConsequence EconomicConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingSocialConsequence SocialConsequences;

    // 後?X???程度
    UPROPERTY(BlueprintReadOnly)
    float SeverityLevel;

    // 後?X??X???
    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;

    
    bool bIsImmediate;

    UPROPERTY(BlueprintReadOnly)
    bool bIsLongTerm;

    
    TArray<FString> TriggeredEvents;
};

/**
 * 決??後??管?X * 負責計?X??X?決策?X???實現完整??決策?X */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionConsequenceManager : public UObject
{
    GENERATED_BODY()

public:
    // 建????    UMingDecisionConsequenceManager();

    // ???X?管??器
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void Initialize();

    
    void CalculateConsequences(const FMingDecisionContext& DecisionContext, FMingConsequenceResult& OutResult};

    // 後?X?用
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ApplyConsequences(const FMingConsequenceResult& ConsequenceResult};

    // 後?X?覽
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void PreviewConsequences(const FMingDecisionContext& DecisionContext, TArray<FMingConsequenceResult>& OutPreviews};

    // 後?X?銷
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    bool UndoConsequences(const FString& ConsequenceID};

    // ????後??歷史
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    TArray<FMingConsequenceResult> GetConsequenceHistory() const { return ConsequenceHistory; }

    // 清除後??歷史
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ClearConsequenceHistory();

protected:
    // 後??計?X    TArray<TSharedPtr<IMingConsequenceCalculator>> ConsequenceCalculators;

    // 後?X?用X    TArray<TSharedPtr<IMingConsequenceApplier>> ConsequenceAppliers;

    // 後??歷史
    TArray<FMingConsequenceResult> ConsequenceHistory;

    // ???X???算器
    void InitializeCalculators();

    // ???X??X?器
    void InitializeAppliers();

    // ????後??ID
    FString GenerateConsequenceID(const FMingDecisionContext& DecisionContext) const;

    // 計??綜?X???程度
    void CalculateOverallSeverity(FMingConsequenceResult& Result) const;

    // ????後?X?述
    void GenerateConsequenceDescription(FMingConsequenceResult& Result) const;

    // 記??後??計??
    void RecordConsequenceCalculation(const FMingDecisionContext& Context, const FMingConsequenceResult& Result};

    // 驗??後?X??X???    bool CanApplyConsequences(const FMingConsequenceResult& ConsequenceResult) const;

private:
    // ??否已??始??
    bool bInitialized = false;
};

