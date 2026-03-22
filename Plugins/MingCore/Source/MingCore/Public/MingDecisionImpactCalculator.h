#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionImpactCalculator.generated.h"

// 影響類型枚舉
UENUM(BlueprintType)
enum class EMingImpactType : uint8
{
    Immediate,      // 立即影響
    ShortTerm,      // 短期影響 (1-3年)
    MediumTerm,     // 中期影響 (3-10年)
    LongTerm,       // 長期影響 (10年以上)
    Permanent       // 永久影響
};

// 影響範圍枚舉
UENUM(BlueprintType)
enum class EMingImpactScope : uint8
{
    Personal,       // 個人層面
    Regional,       // 地區層面
    National,       // 國家層面
    International,  // 國際層面
    Historical       // 歷史層面
};

// 影響程度枚舉
UENUM(BlueprintType)
enum class EMingImpactMagnitude : uint8
{
    Minimal,        // 極小
    Minor,          // 輕微
    Moderate,       // 中等
    Significant,    // 顯著
    Major,          // 重大
    Catastrophic    // 災難性
};

// 歷史路徑分支
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalPath
{
    GENERATED_BODY()

    // 路徑ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PathID;

    // 路徑名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathName;

    // 路徑描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PathDescription;

    // 路徑標籤
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> PathTags;

    // 路徑權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PathWeight;

    // 是否為主要路徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsMainPath;

    FMingHistoricalPath()
    {
        PathID = -1;
        PathName = TEXT("");
        PathDescription = TEXT("");
        PathTags.Empty();
        PathWeight = 1.0f;
        bIsMainPath = false;
    }
};

// 決策影響詳細信息
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactDetail
{
    GENERATED_BODY()

    // 影響類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactType ImpactType;

    // 影響範圍
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactScope ImpactScope;

    // 影響程度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingImpactMagnitude ImpactMagnitude;

    // 影響描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ImpactDescription;

    // 影響數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ImpactValue;

    // 影響持續時間 (年)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DurationYears;

    // 開始時間 (決策後年數)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 StartYear;

    // 影響的屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> AffectedAttributes;

    // 歷史知識內容
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalKnowledge;

    FMingDecisionImpactDetail()
    {
        ImpactType = EMingImpactType::Immediate;
        ImpactScope = EMingImpactScope::Personal;
        ImpactMagnitude = EMingImpactMagnitude::Moderate;
        ImpactDescription = TEXT("");
        ImpactValue = 0.0f;
        DurationYears = 0;
        StartYear = 0;
        AffectedAttributes.Empty();
        HistoricalKnowledge = TEXT("");
    }
};

// 完整決策影響報告
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionImpactReport
{
    GENERATED_BODY()

    // 決策ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 選擇的選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 計算時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime CalculationTime;

    // 總體影響評分
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallImpactScore;

    // 歷史路徑影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingHistoricalPath> AffectedPaths;

    // 詳細影響列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionImpactDetail> ImpactDetails;

    // 角色聲望變化
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> ReputationChanges;

    // 勢力範圍變化
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> TerritoryChanges;

    // 後續事件觸發條件更新
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> UpdatedEventConditions;

    // 教育內容摘要
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EducationalSummary;

    FMingDecisionImpactReport()
    {
        DecisionID = -1;
        ChosenOptionID = -1;
        CalculationTime = FDateTime::Now();
        OverallImpactScore = 0.0f;
        AffectedPaths.Empty();
        ImpactDetails.Empty();
        ReputationChanges.Empty();
        TerritoryChanges.Empty();
        UpdatedEventConditions.Empty();
        EducationalSummary = TEXT("");
    }
};

// 勢力範圍數據
USTRUCT(BlueprintType)
struct FINGCORE_API FMingTerritoryData
{
    GENERATED_BODY()

    // 勢力名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString FactionName;

    // 控制省份數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledProvinces;

    // 控制城市數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ControlledCities;

    // 軍事實力指數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MilitaryStrength;

    // 經濟實力指數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicStrength;

    // 民眾支持度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PopularSupport;

    FMingTerritoryData()
    {
        FactionName = TEXT("");
        ControlledProvinces = 0;
        ControlledCities = 0;
        MilitaryStrength = 0.0f;
        EconomicStrength = 0.0f;
        PopularSupport = 0.0f;
    }
};

// 影響計算事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnImpactCalculated, const FMingDecisionImpactReport&, Report);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHistoricalPathChanged, const FString&, PathName, float, NewWeight);

/**
 * 決策影響計算器
 * 負責計算和分析歷史決策的各種影響
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingDecisionImpactCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionImpactCalculator();

    // 計算決策影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // 計算歷史路徑影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingHistoricalPath> CalculateHistoricalPathImpact(int32 DecisionID, int32 ChosenOptionID);

    // 計算角色聲望影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, float> CalculateReputationImpact(int32 DecisionID, int32 ChosenOptionID);

    // 計算勢力範圍影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<FString, int32> CalculateTerritoryImpact(int32 DecisionID, int32 ChosenOptionID);

    // 生成教育內容
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FString GenerateEducationalContent(int32 DecisionID, int32 ChosenOptionID);

    // 更新事件觸發條件
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FString> UpdateEventTriggerConditions(int32 DecisionID, int32 ChosenOptionID);

    // 獲取影響評估
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    EMingImpactMagnitude GetImpactMagnitude(float ImpactValue);

    // 獲取影響描述
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    FString GetImpactDescription(EMingImpactMagnitude Magnitude);

    // 模擬長期影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TArray<FMingDecisionImpactDetail> SimulateLongTermImpact(int32 DecisionID, int32 ChosenOptionID, int32 SimulationYears);

    // 比較不同選擇的影響
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    TMap<int32, FMingDecisionImpactReport> CompareDecisionOptions(int32 DecisionID);

    // 獲取當前歷史路徑狀態
    UFUNCTION(BlueprintPure, Category = "Decision Impact")
    TArray<FMingHistoricalPath> GetCurrentHistoricalPaths();

    // 計算決策權重
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    float CalculateDecisionWeight(int32 DecisionID, int32 ChosenOptionID);

    // 生成影響預測
    UFUNCTION(BlueprintCallable, Category = "Decision Impact")
    FMingDecisionImpactReport GenerateImpactPrediction(int32 DecisionID, int32 OptionID);

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnImpactCalculated OnImpactCalculated;

    UPROPERTY(BlueprintAssignable)
    FOnHistoricalPathChanged OnHistoricalPathChanged;

protected:
    // 歷史路徑數據庫
    UPROPERTY()
    TArray<FMingHistoricalPath> HistoricalPaths;

    // 勢力數據
    UPROPERTY()
    TMap<FString, FMingTerritoryData> FactionData;

    // 影響計算規則
    UPROPERTY()
    TMap<EMingDecisionType, float> ImpactWeights;

    // 歷史知識庫
    UPROPERTY()
    TMap<int32, FString> HistoricalKnowledgeBase;

private:
    // 初始化歷史路徑
    void InitializeHistoricalPaths();

    // 初始化勢力數據
    void InitializeFactionData();

    // 初始化影響權重
    void InitializeImpactWeights();

    // 初始化歷史知識庫
    void InitializeHistoricalKnowledge();

    // 計算影響評分
    float CalculateImpactScore(const TArray<FMingDecisionImpactDetail>& Impacts);

    // 更新歷史路徑權重
    void UpdateHistoricalPathWeights(const FMingDecisionImpactReport& Report);

    // 生成影響細節
    TArray<FMingDecisionImpactDetail> GenerateImpactDetails(int32 DecisionID, int32 ChosenOptionID);

    // 驗證影響數據
    bool ValidateImpactData(const FMingDecisionImpactReport& Report) const;
};
