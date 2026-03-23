#pragma once

#include "CoreMinimal.h"
#include "MingMemoryOptimization.h"
#include "MingSageBrainOptimizedTypes.generated.h"

// ============================================================================
// 優化的聖者大腦類型
// 使用記憶體優化技術減少記憶體佔用
// ============================================================================

/**
 * 優化的思考結果結構
 * 使用 FMingOptimizedString 減少字串記憶體佔用
 * 限制元數據大小
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainThoughtResult_Optimized
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    ESageBrainThinkingLayer Layer;

    // 使用優化字串 (減少 ~70% 字串記憶體佔用)
    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    FMingOptimizedString Content;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    FDateTime Timestamp;

    // 使用固定大小映射 (限制最多32個元數據項)
    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    TMingFixedMap<FMingOptimizedString, float, 32> Metadata;

    FSageBrainThoughtResult_Optimized()
    {
        Layer = ESageBrainThinkingLayer::Tactical;
        Confidence = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

/**
 * 優化的學習結果結構
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainLearningResult_Optimized
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    ESageBrainLearningAlgorithm Algorithm;

    // 優化字串
    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    FMingOptimizedString LearningData;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    float Performance;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    float Accuracy;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    FDateTime Timestamp;

    // 使用位元封裝存儲額外數據
    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    uint32 PackedData;

    FSageBrainLearningResult_Optimized()
    {
        Algorithm = ESageBrainLearningAlgorithm::ReinforcementLearning;
        Performance = 0.0f;
        Accuracy = 0.0f;
        Timestamp = FDateTime::Now();
        PackedData = 0;
    }
};

/**
 * 優化的哲學分析結果
 * 使用固定大小陣列代替動態 TArray
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainPhilosophyAnalysis_Optimized
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    ESageBrainPhilosophy Philosophy;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    FMingOptimizedString Analysis;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    float Relevance;

    // 限制關鍵原則數量 (最多16個)
    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    TMingFixedArray<FMingOptimizedString, 16> KeyPrinciples;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    FDateTime Timestamp;

    FSageBrainPhilosophyAnalysis_Optimized()
    {
        Philosophy = ESageBrainPhilosophy::Confucianism;
        Relevance = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

/**
 * 優化的意識狀態結構
 * 使用位元封裝減少記憶體佔用
 */
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainConsciousnessState_Optimized
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    ESageBrainConsciousness State;

    // 打包為 uint8 以節省記憶體
    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    uint8 Awareness_Packed;  // 0-255 映射到 0.0-1.0

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    uint8 Clarity_Packed;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    uint8 Insight_Packed;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    FMingOptimizedString Reflection;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    FDateTime Timestamp;

    FSageBrainConsciousnessState_Optimized()
    {
        State = ESageBrainConsciousness::Dormant;
        Awareness_Packed = 0;
        Clarity_Packed = 0;
        Insight_Packed = 0;
        Timestamp = FDateTime::Now();
    }

    // 輔助函數：打包/解包浮點數
    float GetAwareness() const { return Awareness_Packed / 255.0f; }
    void SetAwareness(float Value) { Awareness_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetClarity() const { return Clarity_Packed / 255.0f; }
    void SetClarity(float Value) { Clarity_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetInsight() const { return Insight_Packed / 255.0f; }
    void SetInsight(float Value) { Insight_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

// ============================================================================
// 至聖者指揮系統優化類型
// ============================================================================

/**
 * 優化的階段上下文
 */
USTRUCT(BlueprintType)
struct FPhaseContext_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString PhaseName;
    
    UPROPERTY(BlueprintReadWrite)
    float DurationEstimate;
    
    UPROPERTY(BlueprintReadWrite)
    EForceType PrimaryForce;
    
    // 限制目標數量 (最多16個)
    UPROPERTY(BlueprintReadWrite)
    TMingFixedArray<FMingOptimizedString, 16> Objectives;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime ExpectedEndTime;
    
    FPhaseContext_Optimized()
        : DurationEstimate(0.0f)
        , PrimaryForce(EForceType::Righteous)
    {}
};

/**
 * 優化的三權狀態
 */
USTRUCT(BlueprintType)
struct FAuthorityStatus_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESupremeAuthorityType AuthorityType;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    // 打包為 uint8
    UPROPERTY(BlueprintReadOnly)
    uint8 HealthLevel_Packed;  // 0-255
    
    UPROPERTY(BlueprintReadOnly)
    uint8 CorruptionRisk_Packed;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString LastAction;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime LastCheckTime;
    
    FAuthorityStatus_Optimized()
        : AuthorityType(ESupremeAuthorityType::None)
        , bIsActive(false)
        , HealthLevel_Packed(255)
        , CorruptionRisk_Packed(0)
    {}
    
    float GetHealthLevel() const { return HealthLevel_Packed / 255.0f; }
    void SetHealthLevel(float Value) { HealthLevel_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
    
    float GetCorruptionRisk() const { return CorruptionRisk_Packed / 255.0f; }
    void SetCorruptionRisk(float Value) { CorruptionRisk_Packed = FMath::Clamp(FMath::RoundToInt(Value * 255), 0, 255); }
};

/**
 * 優化的部署配置
 */
USTRUCT(BlueprintType)
struct FDeploymentConfig_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString DeploymentName;
    
    UPROPERTY(BlueprintReadWrite)
    float ResourceAllocation;
    
    UPROPERTY(BlueprintReadWrite)
    float Duration;
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsCovert;
    
    // 限制目標ID數量 (最多32個)
    UPROPERTY(BlueprintReadWrite)
    TMingFixedArray<FMingOptimizedString, 32> TargetIDs;
    
    FDeploymentConfig_Optimized()
        : ResourceAllocation(0.0f)
        , Duration(0.0f)
        , bIsCovert(false)
    {}
};

/**
 * 優化的征服目標
 */
USTRUCT(BlueprintType)
struct FConquestTarget_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString TargetID;
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString TargetName;
    
    // 限制弱點映射大小 (最多16個弱點)
    UPROPERTY(BlueprintReadWrite)
    TMingFixedMap<FMingOptimizedString, float, 16> WeaknessMap;
    
    UPROPERTY(BlueprintReadWrite)
    float ResistanceLevel;
    
    UPROPERTY(BlueprintReadWrite)
    float StrategicValue;
    
    FConquestTarget_Optimized()
        : ResistanceLevel(0.5f)
        , StrategicValue(0.5f)
    {}
};

/**
 * 優化的策略上下文
 */
USTRUCT(BlueprintType)
struct FStrategyContext_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString StrategyName;
    
    UPROPERTY(BlueprintReadWrite)
    EFiveElementPhase CurrentPhase;
    
    // 限制情境因素 (最多16個)
    UPROPERTY(BlueprintReadWrite)
    TMingFixedMap<FMingOptimizedString, float, 16> SituationFactors;
    
    // 限制可用資源 (最多16個)
    UPROPERTY(BlueprintReadWrite)
    TMingFixedArray<FMingOptimizedString, 16> AvailableResources;
    
    UPROPERTY(BlueprintReadWrite)
    float UrgencyLevel;
    
    FStrategyContext_Optimized()
        : CurrentPhase(EFiveElementPhase::None)
        , UrgencyLevel(0.5f)
    {}
};

/**
 * 優化的征服結果
 */
USTRUCT(BlueprintType)
struct FConquestResult_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    float Effectiveness;
    
    UPROPERTY(BlueprintReadOnly)
    float MoralCost;
    
    UPROPERTY(BlueprintReadOnly)
    float ReputationImpact;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString OutcomeDescription;
    
    // 限制副作用數量 (最多16個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedArray<FMingOptimizedString, 16> SideEffects;
    
    FConquestResult_Optimized()
        : bSuccess(false)
        , Effectiveness(0.0f)
        , MoralCost(0.0f)
        , ReputationImpact(0.0f)
    {}
};

/**
 * 優化的策略結果
 */
USTRUCT(BlueprintType)
struct FStrategyResult_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    ETwelveStrategies ExecutedStrategy;
    
    UPROPERTY(BlueprintReadOnly)
    float StrategicGain;
    
    UPROPERTY(BlueprintReadOnly)
    float RiskLevel;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString ResultDescription;
    
    FStrategyResult_Optimized()
        : bSuccess(false)
        , ExecutedStrategy(ETwelveStrategies::EstablishNation)
        , StrategicGain(0.0f)
        , RiskLevel(0.0f)
    {}
};

/**
 * 優化的警告內容
 */
USTRUCT(BlueprintType)
struct FWarningContent_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    EWarningLevel Level;
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString Title;
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString Message;
    
    UPROPERTY(BlueprintReadWrite)
    FMingOptimizedString RecommendedAction;
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime IssueTime;
    
    FWarningContent_Optimized()
        : Level(EWarningLevel::Notice)
    {}
};

/**
 * 優化的自我審核報告
 */
USTRUCT(BlueprintType)
struct FSelfAuditReport_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime AuditTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 TotalCommandsIssued;
    
    UPROPERTY(BlueprintReadOnly)
    int32 EvilForceUsages;
    
    UPROPERTY(BlueprintReadOnly)
    int32 PhaseTransitions;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageDecisionQuality;
    
    // 限制關鍵決策數量 (最多32個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedArray<FMingOptimizedString, 32> CriticalDecisions;
    
    UPROPERTY(BlueprintReadOnly)
    FCorruptionCheckResult CorruptionStatus;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString Recommendations;
    
    FSelfAuditReport_Optimized()
        : TotalCommandsIssued(0)
        , EvilForceUsages(0)
        , PhaseTransitions(0)
        , AverageDecisionQuality(0.0f)
    {}
};

/**
 * 優化的指揮決策建議
 */
USTRUCT(BlueprintType)
struct FCommandRecommendation_Optimized
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESupremeAuthorityType RecommendedAuthority;
    
    UPROPERTY(BlueprintReadOnly)
    EFiveElementPhase RecommendedPhase;
    
    UPROPERTY(BlueprintReadOnly)
    EForceType RecommendedForceType;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString RecommendationDescription;
    
    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;
    
    // 限制推理數量 (最多16個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedArray<FMingOptimizedString, 16> Reasoning;
    
    FCommandRecommendation_Optimized()
        : RecommendedAuthority(ESupremeAuthorityType::None)
        , RecommendedPhase(EFiveElementPhase::None)
        , RecommendedForceType(EForceType::Righteous)
        , ConfidenceLevel(0.0f)
    {}
};

// ============================================================================
// 優化的 API 類型
// ============================================================================

/**
 * 輕量級 API 回應結構
 * 用於減少 API 調用的記憶體開銷
 */
USTRUCT(BlueprintType)
struct FLightweightAPIResponse
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;
    
    UPROPERTY(BlueprintReadOnly)
    int32 ErrorCode;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString Message;
    
    // 使用位元封裝存儲額外數據
    UPROPERTY(BlueprintReadOnly)
    uint32 PackedResponseData;
    
    // 限制回應數據 (最多8個鍵值對)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedMap<FMingOptimizedString, FMingOptimizedString, 8> ResponseData;
    
    FLightweightAPIResponse()
        : bSuccess(false)
        , ErrorCode(0)
        , PackedResponseData(0)
    {}
};

/**
 * 批次 API 請求結構
 * 用於合併多個請求以減少記憶體分配
 */
USTRUCT(BlueprintType)
struct FBatchAPIRequest
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString RequestID;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString APIName;
    
    // 限制參數數量 (最多16個)
    UPROPERTY(BlueprintReadOnly)
    TMingFixedMap<FMingOptimizedString, FMingOptimizedString, 16> Parameters;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime RequestTime;
    
    FBatchAPIRequest()
    {
        RequestTime = FDateTime::Now();
    }
};

// ============================================================================
// 記憶體統計結構
// ============================================================================

/**
 * 結構記憶體使用報告
 */
USTRUCT(BlueprintType)
struct FStructMemoryReport
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString StructName;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OriginalSize;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OptimizedSize;
    
    UPROPERTY(BlueprintReadOnly)
    float MemoryReductionPercent;
    
    UPROPERTY(BlueprintReadOnly)
    FMingOptimizedString OptimizationNotes;
    
    float CalculateReduction() const
    {
        if (OriginalSize > 0)
        {
            return ((OriginalSize - OptimizedSize) / (float)OriginalSize) * 100.0f;
        }
        return 0.0f;
    }
};
