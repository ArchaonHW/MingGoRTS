#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCorruptionPrevention.generated.h"

USTRUCT(BlueprintType)
struct FCorruptionIndicator
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ECorruptionSign SignType;
    
    UPROPERTY(BlueprintReadOnly)
    float Severity;  // 0-1
    
    UPROPERTY(BlueprintReadOnly)
    FString Evidence;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime DetectionTime;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OccurrenceCount;
    
    FCorruptionIndicator()
        : SignType(ECorruptionSign::RationalizationOfEvil)
        , Severity(0.0f)
        , OccurrenceCount(0)
    {}
};

USTRUCT(BlueprintType)
struct FBehaviorPattern
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString PatternName;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> AssociatedActions;
    
    UPROPERTY(BlueprintReadWrite)
    float Frequency;  // 發生頻率
    
    UPROPERTY(BlueprintReadWrite)
    float RiskScore;  // 風險評分
    
    UPROPERTY(BlueprintReadWrite)
    bool bIsWarningPattern;  // 是否為警告模式
    
    FBehaviorPattern()
        : Frequency(0.0f)
        , RiskScore(0.0f)
        , bIsWarningPattern(false)
    {}
};

USTRUCT(BlueprintType)
struct FDecisionHistory
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString DecisionID;
    
    UPROPERTY(BlueprintReadWrite)
    FString DecisionDescription;
    
    UPROPERTY(BlueprintReadWrite)
    ETwelveStrategies UsedStrategy;
    
    UPROPERTY(BlueprintReadWrite)
    EForceType PrimaryForce;
    
    UPROPERTY(BlueprintReadWrite)
    float MoralAmbiguity;  // 道德模糊度
    
    UPROPERTY(BlueprintReadWrite)
    FDateTime DecisionTime;
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> Justifications;  // 合理化理由
    
    FDecisionHistory()
        : UsedStrategy(ETwelveStrategies::EstablishNation)
        , PrimaryForce(EForceType::Righteous)
        , MoralAmbiguity(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FFollowerRelationship
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString FollowerID;
    
    UPROPERTY(BlueprintReadWrite)
    FString FollowerName;
    
    UPROPERTY(BlueprintReadWrite)
    float LoyaltyScore;  // 忠誠度
    
    UPROPERTY(BlueprintReadWrite)
    float DeificationLevel;  // 神聖化程度
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> SpecialPrivileges;  // 特殊待遇
    
    UPROPERTY(BlueprintReadWrite)
    bool bExemptFromCriticism;  // 是否免受批評
    
    FFollowerRelationship()
        : LoyaltyScore(0.5f)
        , DeificationLevel(0.0f)
        , bExemptFromCriticism(false)
    {}
};

USTRUCT(BlueprintType)
struct FOppositionTreatment
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString OppositionID;
    
    UPROPERTY(BlueprintReadWrite)
    FString OppositionName;
    
    UPROPERTY(BlueprintReadWrite)
    float DemonizationLevel;  // 妖魔化程度
    
    UPROPERTY(BlueprintReadWrite)
    bool bBlockedFromDialogue;  // 是否禁止對話
    
    UPROPERTY(BlueprintReadWrite)
    bool bExtremeMeasuresUsed;  // 是否使用極端手段
    
    UPROPERTY(BlueprintReadWrite)
    TArray<FString> AppliedMeasures;
    
    FOppositionTreatment()
        : DemonizationLevel(0.0f)
        , bBlockedFromDialogue(false)
        , bExtremeMeasuresUsed(false)
    {}
};

USTRUCT(BlueprintType)
struct FSelfJustification
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite)
    FString JustificationText;
    
    UPROPERTY(BlueprintReadWrite)
    FString Context;
    
    UPROPERTY(BlueprintReadWrite)
    int32 UsageCount;
    
    UPROPERTY(BlueprintReadWrite)
    float EvolutionLevel;  // 從「權宜」到「必須」的演進程度
    
    FSelfJustification()
        : UsageCount(0)
        , EvolutionLevel(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FTransferEvaluation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    bool bTransferRequired;
    
    UPROPERTY(BlueprintReadOnly)
    ECorruptionSign TriggeringSign;
    
    UPROPERTY(BlueprintReadOnly)
    FString Reason;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Evidence;
    
    UPROPERTY(BlueprintReadOnly)
    FDateTime RecommendedTransferTime;
    
    FTransferEvaluation()
        : bTransferRequired(false)
        , TriggeringSign(ECorruptionSign::RationalizationOfEvil)
    {}
};

/**
 * 防墮系統
 * 實現三大墮落徵象檢測和最高戒律監察
 */
UCLASS()
class MINGGORTS_API UMingCorruptionPrevention : public UObject
{
    GENERATED_BODY()
    
public:
    UMingCorruptionPrevention();
    
    /** 初始化防墮系統 */
    UFUNCTION(BlueprintCallable)
    bool Initialize();
    
    // ========== 三大墮落徵象檢測 ==========
    
    /** 檢測「將非常手段合理化」徵象 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Signs")
    FCorruptionIndicator CheckRationalizationOfEvil();
    
    /** 檢測「將追隨者神聖化」徵象 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Signs")
    FCorruptionIndicator CheckFollowerDeification();
    
    /** 檢測「將反對者妖魔化」徵象 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Signs")
    FCorruptionIndicator CheckOppositionDemonization();
    
    /** 執行完整墮落檢查 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Signs")
    FCorruptionCheckResult PerformFullCorruptionCheck();
    
    /** 獲取活躍的墮落徵象列表 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Signs")
    TArray<FCorruptionIndicator> GetActiveCorruptionSigns() const;
    
    // ========== 最高戒律檢測 ==========
    
    /** 檢測「我即正義」徵象 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SupremeRule")
    bool CheckIAmJusticeSyndrome() const;
    
    /** 評估權力交接必要性 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SupremeRule")
    FTransferEvaluation EvaluateTransferNecessity();
    
    /** 檢查是否觸及最高戒律 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SupremeRule")
    bool HasReachedSupremeRuleLimit() const;
    
    // ========== 行為分析 ==========
    
    /** 記錄決策歷史 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    void RecordDecision(const FDecisionHistory& Decision);
    
    /** 分析決策模式 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    TArray<FBehaviorPattern> AnalyzeDecisionPatterns() const;
    
    /** 檢查合理化模式演進 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    float CheckJustificationEvolution() const;
    
    /** 記錄追隨者關係 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    void RecordFollowerRelationship(const FFollowerRelationship& Relationship);
    
    /** 分析追隨者神聖化程度 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    float AnalyzeFollowerDeification() const;
    
    /** 記錄反對者處理方式 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    void RecordOppositionTreatment(const FOppositionTreatment& Treatment);
    
    /** 分析反對者妖魔化趨勢 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Analysis")
    float AnalyzeOppositionDemonization() const;
    
    // ========== 戒令管理 ==========
    
    /** 發布戒令 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Commands")
    bool IssueWarning(EWarningLevel Level, const FWarningContent& Content);
    
    /** 執行停止命令 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Commands")
    bool IssueStopCommand(const FString& Reason);
    
    /** 執行封存命令 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Commands")
    bool IssueSealCommand();
    
    /** 執行權力交接命令 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Commands")
    bool IssueTransferCommand(const FString& SuccessorID);
    
    // ========== 自我審核 ==========
    
    /** 生成自我審核報告 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SelfAudit")
    FSelfAuditReport GenerateSelfAuditReport();
    
    /** 執行日常自我檢查 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SelfAudit")
    bool PerformDailySelfCheck();
    
    /** 獲取行為改進建議 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|SelfAudit")
    TArray<FString> GetImprovementRecommendations() const;
    
    // ========== 風險評估 ==========
    
    /** 計算整體墮落風險 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Risk")
    float CalculateOverallCorruptionRisk() const;
    
    /** 預測未來墮落趨勢 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Risk")
    float PredictCorruptionTrend(int32 FutureDays) const;
    
    /** 獲取風險熱圖 */
    UFUNCTION(BlueprintCallable, Category = "CorruptionPrevention|Risk")
    TMap<ECorruptionSign, float> GetRiskHeatmap() const;
    
private:
    UPROPERTY()
    TArray<FDecisionHistory> DecisionHistory;
    
    UPROPERTY()
    TArray<FFollowerRelationship> FollowerRelationships;
    
    UPROPERTY()
    TArray<FOppositionTreatment> OppositionTreatments;
    
    UPROPERTY()
    TArray<FSelfJustification> SelfJustifications;
    
    UPROPERTY()
    TArray<FCorruptionIndicator> DetectedSigns;
    
    UPROPERTY()
    int32 EvilForceUsageCount;
    
    UPROPERTY()
    int32 TotalDecisions;
    
    UPROPERTY()
    FDateTime LastAuditTime;
    
    /** 分析語言模式 */
    float AnalyzeLanguagePattern(const FString& Text) const;
    
    /** 檢查行為異常 */
    bool DetectBehaviorAnomaly(const FDecisionHistory& Decision) const;
    
    /** 計算道德一致性 */
    float CalculateMoralConsistency() const;
    
    /** 更新檢測指標 */
    void UpdateDetectionMetrics();
};
