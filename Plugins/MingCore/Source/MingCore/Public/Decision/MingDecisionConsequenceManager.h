#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceManager.generated.h"

// 前向聲明
class IMingConsequenceCalculator;
class IMingConsequenceApplier;

// 決策上下文
USTRUCT(BlueprintType)
struct MINGCORE_API FMingDecisionContext
{
    GENERATED_BODY()

    // 決策基本信息
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionTitle;

    UPROPERTY(BlueprintReadOnly)
    FString DecisionDescription;

    // 決策選項
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SelectedOptions;

    // 時間上下文
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentMonth;

    // 地理上下文
    UPROPERTY(BlueprintReadOnly)
    FString CurrentRegion;

    // 角色上下文
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InvolvedCharacters;

    // 歷史上下文
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PreviousEvents;
};

// 政治後果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingPoliticalConsequence
{
    GENERATED_BODY()

    // 派系關係變化
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactionRelationChanges;

    // 政治穩定性
    UPROPERTY(BlueprintReadOnly)
    float PoliticalStabilityChange;

    // 政治聲望
    UPROPERTY(BlueprintReadOnly)
    float PoliticalReputationChange;

    // 內部政治壓力
    UPROPERTY(BlueprintReadOnly)
    float InternalPoliticalPressure;
};

// 軍事後果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingMilitaryConsequence
{
    GENERATED_BODY()

    // 軍事實力變化
    UPROPERTY(BlueprintReadOnly)
    int32 ManpowerChange;

    UPROPERTY(BlueprintReadOnly)
    int32 EquipmentChange;

    UPROPERTY(BlueprintReadOnly)
    float MoraleChange;

    // 戰爭狀態變化
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> WarStatusChanges;

    // 軍事預算變化
    UPROPERTY(BlueprintReadOnly)
    float MilitaryBudgetChange;
};

// 經濟後果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingEconomicConsequence
{
    GENERATED_BODY()

    // 經濟資源變化
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceChanges;

    // 經濟指標
    UPROPERTY(BlueprintReadOnly)
    float GDPChange;

    UPROPERTY(BlueprintReadOnly)
    float InflationRateChange;

    UPROPERTY(BlueprintReadOnly)
    float TaxRevenueChange;

    // 基礎設施變化
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> InfrastructureChanges;
};

// 社會後果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingSocialConsequence
{
    GENERATED_BODY()

    // 民眾支持度
    UPROPERTY(BlueprintReadOnly)
    float PublicSupportChange;

    // 社會穩定性
    UPROPERTY(BlueprintReadOnly)
    float SocialStabilityChange;

    // 教育水平
    UPROPERTY(BlueprintReadOnly)
    float EducationLevelChange;

    // 公共健康
    UPROPERTY(BlueprintReadOnly)
    float PublicHealthChange;

    // 社會運動
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SocialMovements;
};

// 後果結果
USTRUCT(BlueprintType)
struct MINGCORE_API FMingConsequenceResult
{
    GENERATED_BODY()

    // 後果基本信息
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceTitle;

    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // 後果類型分類
    UPROPERTY(BlueprintReadOnly)
    FMingPoliticalConsequence PoliticalConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingMilitaryConsequence MilitaryConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingEconomicConsequence EconomicConsequences;

    UPROPERTY(BlueprintReadOnly)
    FMingSocialConsequence SocialConsequences;

    // 後果嚴重程度
    UPROPERTY(BlueprintReadOnly)
    float SeverityLevel;

    // 後果持續時間
    UPROPERTY(BlueprintReadOnly)
    int32 DurationMonths;

    // 後果可見性
    UPROPERTY(BlueprintReadOnly)
    bool bIsImmediate;

    UPROPERTY(BlueprintReadOnly)
    bool bIsLongTerm;

    // 後果觸發的事件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggeredEvents;
};

/**
 * 決策後果管理器
 * 負責計算和應用決策後果，實現完整的決策閉環
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingDecisionConsequenceManager : public UObject
{
    GENERATED_BODY()

public:
    // 建構子
    UMingDecisionConsequenceManager();

    // 初始化管理器
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void Initialize();

    // 後果計算主介面
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void CalculateConsequences(const FMingDecisionContext& DecisionContext, FMingConsequenceResult& OutResult);

    // 後果應用
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ApplyConsequences(const FMingConsequenceResult& ConsequenceResult);

    // 後果預覽
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void PreviewConsequences(const FMingDecisionContext& DecisionContext, TArray<FMingConsequenceResult>& OutPreviews);

    // 後果撤銷
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    bool UndoConsequences(const FString& ConsequenceID);

    // 獲取後果歷史
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    TArray<FMingConsequenceResult> GetConsequenceHistory() const { return ConsequenceHistory; }

    // 清除後果歷史
    UFUNCTION(BlueprintCallable, Category = "Ming|Decision")
    void ClearConsequenceHistory();

protected:
    // 後果計算器
    TArray<TSharedPtr<IMingConsequenceCalculator>> ConsequenceCalculators;

    // 後果應用器
    TArray<TSharedPtr<IMingConsequenceApplier>> ConsequenceAppliers;

    // 後果歷史
    TArray<FMingConsequenceResult> ConsequenceHistory;

    // 初始化計算器
    void InitializeCalculators();

    // 初始化應用器
    void InitializeAppliers();

    // 生成後果ID
    FString GenerateConsequenceID(const FMingDecisionContext& DecisionContext) const;

    // 計算綜合嚴重程度
    void CalculateOverallSeverity(FMingConsequenceResult& Result) const;

    // 生成後果描述
    void GenerateConsequenceDescription(FMingConsequenceResult& Result) const;

    // 記錄後果計算
    void RecordConsequenceCalculation(const FMingDecisionContext& Context, const FMingConsequenceResult& Result);

    // 驗證後果可應用性
    bool CanApplyConsequences(const FMingConsequenceResult& ConsequenceResult) const;

private:
    // 是否已初始化
    bool bInitialized = false;
};
