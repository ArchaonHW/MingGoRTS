#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSRelationshipNetwork.h"
#include "MingGoRTSRelationshipDynamics.generated.h"

UENUM(BlueprintType)
enum class ERelationshipChangeType : uint8
{
    Improvement       UMETA(DisplayName = "關係改善"),
    Deterioration     UMETA(DisplayName = "關係惡化"),
    Stabilization     UMETA(DisplayName = "關係穩定"),
    Conflict          UMETA(DisplayName = "關係衝突"),
    Resolution        UMETA(DisplayName = "關係解決"),
    Transformation    UMETA(DisplayName = "關係轉變")
};

UENUM(BlueprintType)
enum class EInfluencePropagationType : uint8
{
    Direct            UMETA(DisplayName = "直接影響"),
    Indirect          UMETA(DisplayName = "間接影響"),
    Cascading          UMETA(DisplayName = "級聯影響"),
    Network           UMETA(DisplayName = "網絡影響"),
    Global            UMETA(DisplayName = "全局影響")
};

UENUM(BlueprintType)
enum class ERelationshipStability : uint8
{
    VeryStable        UMETA(DisplayName = "非常穩定"),
    Stable            UMETA(DisplayName = "穩定"),
    Moderate          UMETA(DisplayName = "中等"),
    Unstable          UMETA(DisplayName = "不穩定"),
    VeryUnstable      UMETA(DisplayName = "非常不穩定")
};

USTRUCT(BlueprintType)
struct FRelationshipChange
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString ChangeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString TargetCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    ERelationshipChangeType ChangeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float OldValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float NewValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float ChangeMagnitude;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString ChangeReason;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FString TriggerEventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    FDateTime ChangeTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    float DecayRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    bool bIsPermanent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Relationship Change")
    bool bPropagates;

    FRelationshipChange()
    {
        ChangeID = TEXT("");
        SourceCharacterID = TEXT("");
        TargetCharacterID = TEXT("");
        ChangeType = ERelationshipChangeType::Improvement;
        OldValue = 0.0f;
        NewValue = 0.0f;
        ChangeMagnitude = 0.0f;
        ChangeReason = TEXT("");
        Context = TEXT("");
        TriggerEventID = TEXT("");
        DecayRate = 0.0f;
        bIsPermanent = false;
        bPropagates = true;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipInfluencePropagation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString PropagationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString SourceCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FString InfluenceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    EInfluencePropagationType PropagationType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float InitialInfluence;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float PropagationStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    int32 MaxDepth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    float AttenuationFactor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    TArray<FString> AffectedCharacters;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    TMap<FString, float> InfluenceValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Influence Propagation")
    bool bIsActive;

    FRelationshipInfluencePropagation()
    {
        PropagationID = TEXT("");
        SourceCharacterID = TEXT("");
        InfluenceType = TEXT("");
        PropagationType = EInfluencePropagationType::Direct;
        InitialInfluence = 0.0f;
        PropagationStrength = 1.0f;
        MaxDepth = 3;
        AttenuationFactor = 0.5f;
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
        bIsActive = false;
    }
};

USTRUCT(BlueprintType)
struct FRelationshipStabilityAnalysis
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    FString NetworkID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    ERelationshipStability OverallStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    float StabilityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TMap<FString, float> CharacterStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> UnstableRelationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> CriticalRelationships;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> RiskFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    TArray<FString> StabilizingFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    float PredictedStability;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stability Analysis")
    FDateTime AnalysisTime;

    FRelationshipStabilityAnalysis()
    {
        NetworkID = TEXT("");
        OverallStability = ERelationshipStability::Moderate;
        StabilityScore = 0.0f;
        PredictedStability = 0.0f;
        AnalysisTime = FDateTime::Now();
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRelationshipChanged, const FString&, SourceID, const FString&, TargetID, ERelationshipChangeType, ChangeType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInfluencePropagated, const FString&, PropagationID, const TArray<FString>&, AffectedCharacters);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStabilityAnalysisComplete, const FString&, NetworkID, const FRelationshipStabilityAnalysis&, Analysis);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRelationshipDynamics : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSRelationshipDynamics();

    // 關係動態事件
    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnInfluencePropagated OnInfluencePropagated;

    UPROPERTY(BlueprintAssignable, Category = "Relationship Dynamics")
    FOnStabilityAnalysisComplete OnStabilityAnalysisComplete;

    // 初始化關係動態系統
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void InitializeRelationshipDynamics();

    // 應用關係變化
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool ApplyRelationshipChange(const FString& SourceID, const FString& TargetID, ERelationshipChangeType ChangeType, float ChangeValue, const FString& Reason);

    // 傳播關係影響
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    FString PropagateInfluence(const FString& SourceID, const FString& InfluenceType, float InfluenceValue, EInfluencePropagationType PropagationType, int32 MaxDepth);

    // 分析網絡穩定性
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void AnalyzeNetworkStability(const FString& NetworkID);

    // 獲取關係變化歷史
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FRelationshipChange> GetRelationshipChangeHistory(const FString& SourceID, const FString& TargetID) const;

    // 獲取影響傳播狀態
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FRelationshipInfluencePropagation> GetActivePropagations() const;

    // 獲取穩定性分析結果
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    FRelationshipStabilityAnalysis GetStabilityAnalysis(const FString& NetworkID) const;

    // 預測關係發展趨勢
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<float> PredictRelationshipTrend(const FString& SourceID, const FString& TargetID, int32 TimeSteps) const;

    // 計算關係衰減
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    float CalculateRelationshipDecay(const FString& SourceID, const FString& TargetID, float TimeElapsed) const;

    // 應用時間衰減
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void ApplyTimeDecay(float DeltaTime);

    // 檢查關係衝突
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FString> CheckRelationshipConflicts(const FString& NetworkID) const;

    // 解決關係衝突
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool ResolveRelationshipConflict(const FString& SourceID, const FString& TargetID);

    // 計算網絡動態指標
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TMap<FString, float> CalculateNetworkDynamics(const FString& NetworkID) const;

    // 獲取關係變化建議
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TArray<FString> GetRelationshipChangeSuggestions(const FString& CharacterID) const;

    // 模擬關係發展
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void SimulateRelationshipDevelopment(const FString& NetworkID, int32 SimulationSteps);

    // 獲取關係穩定性預測
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    float PredictRelationshipStability(const FString& SourceID, const FString& TargetID) const;

    // 設置關係衰減參數
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    void SetDecayParameters(float BaseDecayRate, float InfluenceDecayRate);

    // 獲取動態統計
    UFUNCTION(BlueprintPure, Category = "Relationship Dynamics")
    TMap<FString, float> GetDynamicsStatistics() const;

    // 保存動態數據
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool SaveDynamicsData(const FString& SaveSlotName);

    // 載入動態數據
    UFUNCTION(BlueprintCallable, Category = "Relationship Dynamics")
    bool LoadDynamicsData(const FString& SaveSlotName);

protected:
    // 關係變化歷史
    UPROPERTY()
    TArray<FRelationshipChange> RelationshipChanges;

    // 活動的影響傳播
    UPROPERTY()
    TArray<FRelationshipInfluencePropagation> ActivePropagations;

    // 穩定性分析結果
    UPROPERTY()
    TMap<FString, FRelationshipStabilityAnalysis> StabilityAnalyses;

    // 關係網絡系統引用
    UPROPERTY()
    class UMingGoRTSRelationshipNetwork* RelationshipNetwork;

    // 衰減參數
    float BaseDecayRate;
    float InfluenceDecayRate;

    // 是否已初始化
    bool bIsInitialized;

    // 處理關係變化
    void ProcessRelationshipChange(const FRelationshipChange& Change);

    // 執行影響傳播
    void ExecuteInfluencePropagation(FRelationshipInfluencePropagation& Propagation);

    // 計算傳播衰減
    float CalculatePropagationDecay(float InitialValue, int32 Depth, float AttenuationFactor) const;

    // 更新關係穩定性
    void UpdateRelationshipStability(const FString& NetworkID);

    // 檢測關係模式
    void DetectRelationshipPatterns(const FString& NetworkID);

    // 預測網絡變化
    void PredictNetworkChanges(const FString& NetworkID);

    // 計算關係權重
    float CalculateRelationshipWeight(const FString& SourceID, const FString& TargetID) const;

    // 獲取關係變化類型權重
    float GetChangeTypeWeight(ERelationshipChangeType ChangeType) const;

    // 檢查關係閾值
    bool CheckRelationshipThreshold(const FString& SourceID, const FString& TargetID, float Threshold) const;

    // 應用關係閾值
    void ApplyRelationshipThreshold(const FString& SourceID, const FString& TargetID);

    // 計算網絡影響力
    float CalculateNetworkInfluence(const FString& NetworkID) const;

    // 檢測關係連鎖反應
    void DetectRelationshipChainReactions(const FString& NetworkID);

    // 處理連鎖反應
    void ProcessChainReaction(const TArray<FString>& AffectedCharacters);

    // 計算關係恢復力
    float CalculateRelationshipResilience(const FString& SourceID, const FString& TargetID) const;

    // 預測關係恢復時間
    float PredictRelationshipRecoveryTime(const FString& SourceID, const FString& TargetID) const;

    // 生成變化ID
    FString GenerateChangeID(const FString& SourceID, const FString& TargetID) const;

    // 生成傳播ID
    FString GeneratePropagationID(const FString& SourceID, const FString& InfluenceType) const;

    // 清理過期的傳播
    void CleanupExpiredPropagations();

    // 更新統計數據
    void UpdateStatistics();

    // 記錄動態事件
    void RecordDynamicsEvent(const FString& EventType, const FString& Description);

    // 分析關係趨勢
    void AnalyzeRelationshipTrends(const FString& NetworkID);

    // 計算趨勢斜率
    float CalculateTrendSlope(const TArray<float>& Values) const;

    // 預測趨勢方向
    int32 PredictTrendDirection(const TArray<float>& Values) const;

    // 獲取關係變化頻率
    float GetRelationshipChangeFrequency(const FString& SourceID, const FString& TargetID) const;

    // 計算關係變化幅度
    float CalculateRelationshipChangeMagnitude(const FString& SourceID, const FString& TargetID) const;

    // 檢查關係平衡
    bool CheckRelationshipBalance(const FString& NetworkID) const;

    // 恢復關係平衡
    void RestoreRelationshipBalance(const FString& NetworkID);

    // 計算網絡平衡度
    float CalculateNetworkBalance(const FString& NetworkID) const;

    // 檢測關係異常
    TArray<FString> DetectRelationshipAnomalies(const FString& NetworkID) const;

    // 處理關係異常
    void HandleRelationshipAnomaly(const FString& SourceID, const FString& TargetID);

    // 計算異常分數
    float CalculateAnomalyScore(const FString& SourceID, const FString& TargetID) const;

    // 預測關係崩潰風險
    float PredictRelationshipCollapseRisk(const FString& SourceID, const FString& TargetID) const;

    // 計算網絡韌性指數
    float CalculateNetworkResilienceIndex(const FString& NetworkID) const;

    // 檢測網絡脆弱點
    TArray<FString> DetectNetworkVulnerabilities(const FString& NetworkID) const;

    // 計算關係傳染風險
    float CalculateRelationshipContagionRisk(const FString& NetworkID) const;

    // 模擬關係傳播
    void SimulateRelationshipContagion(const FString& NetworkID, const FString& SourceID, float ContagionStrength);

    // 計算關係隔離效果
    float CalculateRelationshipIsolationEffect(const FString& NetworkID, const FString& CharacterID) const;

    // 預測關係演化路徑
    TArray<FString> PredictRelationshipEvolutionPath(const FString& SourceID, const FString& TargetID) const;

    // 計算關係演化概率
    TMap<ERelationshipType, float> CalculateRelationshipEvolutionProbabilities(const FString& SourceID, const FString& TargetID) const;

    // 生成關係發展報告
    FString GenerateRelationshipDevelopmentReport(const FString& NetworkID) const;
};
