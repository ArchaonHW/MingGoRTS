#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTwelveStrategiesExecutor.generated.h"

USTRUCT(BlueprintType)
struct FEstablishNationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString NationName;
    
    UPROPERTY(BlueprintReadWrite)
    FString RighteousFlag;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> CoreValues;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TargetPopulations;
    
    UPROPERTY(BlueprintReadWrite)
    float LegitimacyBuildingRate;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUsePublicCampaign;
    
    FEstablishNationConfig()
        : LegitimacyBuildingRate(0.5f)
        , bUsePublicCampaign(true)
    {}
};

USTRUCT(BlueprintType)
struct FEstablishSystemConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> RulesToEstablish;
    
    UPROPERTY(BlueprintReadWrite)
    bool bCreateHierarchy;
    
    UPROPERTY(BlueprintReadWrite)
    bool bDefineRewardsPunishments;
    
    UPROPERTY(BlueprintReadWrite)
    bool bAssignResponsibilities;
    
    UPROPERTY(BlueprintReadWrite)
    float SystemStabilityTarget;
    
    FEstablishSystemConfig()
        : bCreateHierarchy(true)
        , bDefineRewardsPunishments(true)
        , bAssignResponsibilities(true)
        , SystemStabilityTarget(0.8f)
    {}
};

USTRUCT(BlueprintType)
struct FEstablishPeopleConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> KeyPositions;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TalentCriteria;
    
    UPROPERTY(BlueprintReadWrite)
    bool bBuildRoleModels;
    
    UPROPERTY(BlueprintReadWrite)
    bool bCreateMentorshipSystem;
    
    UPROPERTY(BlueprintReadWrite)
    float PeopleQualityTarget;
    
    FEstablishPeopleConfig()
        : bBuildRoleModels(true)
        , bCreateMentorshipSystem(true)
        , PeopleQualityTarget(0.7f)
    {}
};

USTRUCT(BlueprintType)
struct FBreakSituationConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString TargetSituation;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> BreakingPoints;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseSurprise;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUseAsymmetry;
    
    UPROPERTY(BlueprintReadWrite)
    float SpeedRequirement;
    
    UPROPERTY(BlueprintReadWrite)
    float CovertLevel;
    
    FBreakSituationConfig()
        : bUseSurprise(true)
        , bUseAsymmetry(true)
        , SpeedRequirement(0.8f)
        , CovertLevel(0.6f)
    {}
};

USTRUCT(BlueprintType)
struct FBreakStructureConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString TargetStructure;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> CriticalConnections;
    
    UPROPERTY(BlueprintReadWrite)
    bool bSeverRelationships;
    
    UPROPERTY(BlueprintReadWrite)
    bool bUndermineFoundations;
    
    UPROPERTY(BlueprintReadWrite)
    float StructuralDisruptionTarget;
    
    FBreakStructureConfig()
        : bSeverRelationships(true)
        , bUndermineFoundations(true)
        , StructuralDisruptionTarget(0.7f)
    {}
};

USTRUCT(BlueprintType)
struct FBreakNotPersonConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TargetsToPreserve;
    
    UPROPERTY(BlueprintReadWrite)
    bool bLeaveFormRemoveSubstance;
    
    UPROPERTY(BlueprintReadWrite)
    bool bRedirectRatherThanDestroy;
    
    UPROPERTY(BlueprintReadWrite)
    float PreservationRate;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> ReplacementMechanisms;
    
    FBreakNotPersonConfig()
        : bLeaveFormRemoveSubstance(true)
        , bRedirectRatherThanDestroy(true)
        , PreservationRate(0.8f)
    {}
};

USTRUCT(BlueprintType)
struct FStrategyExecutionContext
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    ETwelveStrategies Strategy;
    
    UPROPERTY(BlueprintReadWrite)
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite)
    float UrgencyLevel;
    
    UPROPERTY(BlueprintReadWrite)
    float ResourceAvailability;
    
    UPROPERTY(BlueprintReadWrite)
    float RiskTolerance;
    
    UPROPERTY(BlueprintReadWrite)
    TMap<FString, float> EnvironmentalFactors;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Constraints;
    
    FStrategyExecutionContext()
        : Strategy(ETwelveStrategies::EstablishNation)
        , CurrentPhase(EFiveElementPhase::None)
        , UrgencyLevel(0.5f)
        , ResourceAvailability(0.5f)
        , RiskTolerance(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FStrategyCompatibility
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ETwelveStrategies Strategy;
    
    UPROPERTY(BlueprintReadOnly)
    float PhaseCompatibility;  // 與當前階段的相容性
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceCompatibility;  // 資源匹配度
    
    UPROPERTY(BlueprintReadOnly)
    float RiskCompatibility;  // 風險匹配度
    
    UPROPERTY(BlueprintReadOnly)
    float OverallScore;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> CompatibilityReasons;
    
    FStrategyCompatibility()
        : Strategy(ETwelveStrategies::EstablishNation)
        , PhaseCompatibility(0.0f)
        , ResourceCompatibility(0.0f)
        , RiskCompatibility(0.0f)
        , OverallScore(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FStrategyExecutionReport
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ETwelveStrategies ExecutedStrategy;
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    float StrategicGain;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceCost;
    
    UPROPERTY(BlueprintReadOnly)
    float TimeCost;
    
    UPROPERTY(BlueprintReadOnly)
    float MoralImpact;
    
    UPROPERTY(BlueprintReadOnly)
    float ReputationImpact;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnintendedConsequences;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RecommendedFollowUps;
    
    FStrategyExecutionReport()
        : ExecutedStrategy(ETwelveStrategies::EstablishNation)
        , bSuccess(false)
        , StrategicGain(0.0f)
        , ResourceCost(0.0f)
        , TimeCost(0.0f)
        , MoralImpact(0.0f)
        , ReputationImpact(0.0f)
    {}
};

/**
 * 十二策執行器
 * 實現正六策（立國、立制、立人）與逆六策（破局、破結構、不破人）
 */
UCLASS()
class MINGGORTS_API UMingTwelveStrategiesExecutor : public UObject
{
    GENERATED_BODY()
    
public:
    UMingTwelveStrategiesExecutor();
    
    /** 初始化執行器 */
    UFUNCTION(BlueprintCallable)
    bool Initialize();
    
    // ========== 正六策執行 ==========
    
    /** 立國 - 伐心正道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Righteous")
    FStrategyResult ExecuteEstablishNation(
        const FEstablishNationConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    /** 立制 - 伐勢伐命正道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Righteous")
    FStrategyResult ExecuteEstablishSystem(
        const FEstablishSystemConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    /** 立人 - 伐氣伐影伐無正道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Righteous")
    FStrategyResult ExecuteEstablishPeople(
        const FEstablishPeopleConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    // ========== 逆六策執行 ==========
    
    /** 破局 - 伐勢伐氣逆道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reverse")
    FStrategyResult ExecuteBreakSituation(
        const FBreakSituationConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    /** 破結構 - 伐命伐影逆道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reverse")
    FStrategyResult ExecuteBreakStructure(
        const FBreakStructureConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    /** 不破人 - 伐心伐無逆道 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reverse")
    FStrategyResult ExecuteBreakNotPerson(
        const FBreakNotPersonConfig& Config,
        const FStrategyExecutionContext& Context
    );
    
    // ========== 通用執行接口 ==========
    
    /** 執行指定策略 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies")
    FStrategyResult ExecuteStrategy(
        ETwelveStrategies Strategy,
        const FStrategyContext& Context
    );
    
    /** 根據情境自動選擇策略 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies")
    FStrategyResult ExecuteAutoSelectedStrategy(const FStrategyExecutionContext& Context);
    
    // ========== 策略規劃與評估 ==========
    
    /** 評估策略相容性 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Planning")
    FStrategyCompatibility EvaluateStrategyCompatibility(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 獲取所有策略相容性排序 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Planning")
    TArray<FStrategyCompatibility> RankStrategiesByCompatibility(
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 獲取當前階段推薦的策略 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Planning")
    TArray<ETwelveStrategies> GetRecommendedStrategiesForPhase(
        EFiveElementPhase Phase,
        const FSituationAssessment& Assessment
    ) const;
    
    /** 檢查策略使用條件 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Planning")
    bool CheckStrategyPrerequisites(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 預測策略效果 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Planning")
    FStrategyExecutionReport PredictStrategyOutcome(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    // ========== 策略組合與序列 ==========
    
    /** 創建正策組合（立國→立制→立人） */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Combination")
    TArray<FStrategyResult> ExecuteRighteousSequence(
        const FEstablishNationConfig& NationConfig,
        const FEstablishSystemConfig& SystemConfig,
        const FEstablishPeopleConfig& PeopleConfig
    );
    
    /** 創建逆策組合（破局→破結構→不破人） */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Combination")
    TArray<FStrategyResult> ExecuteReverseSequence(
        const FBreakSituationConfig& SituationConfig,
        const FBreakStructureConfig& StructureConfig,
        const FBreakNotPersonConfig& NotPersonConfig
    );
    
    /** 創建正逆混合序列 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Combination")
    TArray<FStrategyResult> ExecuteMixedSequence(
        const TArray<ETwelveStrategies>& StrategySequence,
        const TArray<FStrategyContext>& Contexts
    );
    
    // ========== 策略風險評估 ==========
    
    /** 計算策略風險 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Risk")
    float CalculateStrategyRisk(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 評估策略反噬風險 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Risk")
    float AssessBackfireRisk(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 獲取策略道德邊界 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Risk")
    TArray<FString> GetStrategyMoralBoundaries(ETwelveStrategies Strategy) const;
    
    // ========== 策略執行報告 ==========
    
    /** 生成執行報告 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reporting")
    FStrategyExecutionReport GenerateExecutionReport(
        const FStrategyResult& Result,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 獲取執行歷史 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reporting")
    TArray<FStrategyExecutionReport> GetExecutionHistory() const;
    
    /** 分析執行模式 */
    UFUNCTION(BlueprintCallable, Category = "TwelveStrategies|Reporting")
    TMap<ETwelveStrategies, float> AnalyzeExecutionPatterns() const;
    
private:
    UPROPERTY()
    TArray<FStrategyExecutionReport> ExecutionHistory;
    
    UPROPERTY()
    TMap<ETwelveStrategies, int32> StrategyUsageCounts;
    
    UPROPERTY()
    TMap<ETwelveStrategies, float> AverageSuccessRates;
    
    UPROPERTY()
    int32 RighteousStrategyCount;
    
    UPROPERTY()
    int32 ReverseStrategyCount;
    
    /** 計算立國效果 */
    float CalculateEstablishNationEffect(
        const FEstablishNationConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 計算立制效果 */
    float CalculateEstablishSystemEffect(
        const FEstablishSystemConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 計算立人效果 */
    float CalculateEstablishPeopleEffect(
        const FEstablishPeopleConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 計算破局效果 */
    float CalculateBreakSituationEffect(
        const FBreakSituationConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 計算破結構效果 */
    float CalculateBreakStructureEffect(
        const FBreakStructureConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 計算不破人效果 */
    float CalculateBreakNotPersonEffect(
        const FBreakNotPersonConfig& Config,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 檢查正策使用條件 */
    bool CheckRighteousPrerequisites(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 檢查逆策啟動條件 */
    bool CheckReverseActivationConditions(
        ETwelveStrategies Strategy,
        const FStrategyExecutionContext& Context
    ) const;
    
    /** 更新使用統計 */
    void UpdateUsageStatistics(ETwelveStrategies Strategy, bool bSuccess);
};
