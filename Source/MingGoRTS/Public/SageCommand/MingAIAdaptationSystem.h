#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingAIAdaptationSystem.generated.h"

// ============================================================================
// AI適配系統枚舉定義
// ============================================================================

/** AI對手類型 */
UENUM(BlueprintType)
enum class EAIOpponentType : uint8
{
    None                UMETA(DisplayName = "無"),
    SageCommander       UMETA(DisplayName = "聖者指揮官"),
    DemonCommander      UMETA(DisplayName = "魔王指揮官"),
    FalseSageCommander  UMETA(DisplayName = "偽聖者指揮官"),
    BalancedCommander   UMETA(DisplayName = "平衡指揮官"),
    AggressiveCommander UMETA(DisplayName = "侵略指揮官"),
    DefensiveCommander  UMETA(DisplayName = "防禦指揮官"),
    AdaptiveCommander   UMETA(DisplayName = "適應指揮官"),
    ChaoticCommander    UMETA(DisplayName = "混沌指揮官")
};

/** AI決策模式 */
UENUM(BlueprintType)
enum class EAIDecisionMode : uint8
{
    None                UMETA(DisplayName = "無"),
    Strategic           UMETA(DisplayName = "戰略模式"),
    Tactical            UMETA(DisplayName = "戰術模式"),
    Reactive            UMETA(DisplayName = "反應模式"),
    Proactive           UMETA(DisplayName = "主動模式"),
    Defensive           UMETA(DisplayName = "防禦模式"),
    Aggressive          UMETA(DisplayName = "攻擊模式"),
    Economic            UMETA(DisplayName = "經濟模式"),
    Diplomatic          UMETA(DisplayName = "外交模式"),
    Deceptive           UMETA(DisplayName = "欺騙模式")
};

/** AI適配響應類型 */
UENUM(BlueprintType)
enum class EAIAdaptationResponse : uint8
{
    None                UMETA(DisplayName = "無"),
    CounterStrategy     UMETA(DisplayName = "反制策略"),
    MimicStrategy       UMETA(DisplayName = "模仿策略"),
    DisruptStrategy     UMETA(DisplayName = "干擾策略"),
    IgnoreStrategy      UMETA(DisplayName = "無視策略"),
    ExploitWeakness     UMETA(DisplayName = "利用弱點"),
    DefensivePosture    UMETA(DisplayName = "防禦姿態"),
    AggressiveExpansion UMETA(DisplayName = "侵略擴張"),
    EconomicFocus       UMETA(DisplayName = "專注經濟"),
    TechnologicalRush   UMETA(DisplayName = "科技突進")
};

/** AI威脅等級 */
UENUM(BlueprintType)
enum class EAIThreatLevel : uint8
{
    None                UMETA(DisplayName = "無"),
    Minimal             UMETA(DisplayName = "極低"),
    Low                 UMETA(DisplayName = "低"),
    Moderate            UMETA(DisplayName = "中等"),
    High                UMETA(DisplayName = "高"),
    Critical            UMETA(DisplayName = "危急"),
    Existential         UMETA(DisplayName = "生存威脅")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** AI指揮官檔案 */
USTRUCT(BlueprintType)
struct FAICommanderProfile
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    FString CommanderID;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    FString CommanderName;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    EAIOpponentType OpponentType;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    EAIDecisionMode PreferredMode;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float Aggressiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float Defensiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float Adaptability;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float Deceptiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float EconomicFocus;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    float MilitaryFocus;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    TArray<ESixStrategyType> PreferredStrategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Profile")
    TArray<EStrategyEffectType> ExploitableWeaknesses;
    
    FAICommanderProfile()
        : OpponentType(EAIOpponentType::None)
        , PreferredMode(EAIDecisionMode::None)
        , Aggressiveness(0.5f)
        , Defensiveness(0.5f)
        , Adaptability(0.5f)
        , Deceptiveness(0.5f)
        , EconomicFocus(0.5f)
        , MilitaryFocus(0.5f)
    {}
};

/** 玩家行為分析 */
USTRUCT(BlueprintType)
struct FPlayerBehaviorAnalysis
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    FString PlayerID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    float AggressionLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    float StrategyVariety;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    float ResourceEfficiency;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    float MoralConsistency;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    float YinYangBalance;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    TMap<ESixStrategyType, int32> StrategyUsageFrequency;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    TArray<EStrategyEffectType> DetectedWeaknesses;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    TArray<EStrategyEffectType> DetectedStrengths;
    
    UPROPERTY(BlueprintReadWrite, Category = "Player Analysis")
    FDateTime LastUpdateTime;
    
    FPlayerBehaviorAnalysis()
        : AggressionLevel(0.5f)
        , StrategyVariety(0.5f)
        , ResourceEfficiency(0.5f)
        , MoralConsistency(0.5f)
        , YinYangBalance(0.5f)
    {}
};

/** AI適配決策 */
USTRUCT(BlueprintType)
struct FAIAdaptationDecision
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    EAIAdaptationResponse ResponseType;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    ESixStrategyType RecommendedCounterStrategy;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    EAIDecisionMode RecommendedMode;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    float Confidence;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    float ExpectedEffectiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    TArray<FString> Reasoning;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    TArray<FString> TargetVulnerabilities;
    
    UPROPERTY(BlueprintReadWrite, Category = "AI Decision")
    float RiskAssessment;
    
    FAIAdaptationDecision()
        : ResponseType(EAIAdaptationResponse::None)
        , RecommendedCounterStrategy(ESixStrategyType::None)
        , RecommendedMode(EAIDecisionMode::None)
        , Confidence(0.0f)
        , ExpectedEffectiveness(0.0f)
        , RiskAssessment(0.0f)
    {}
};

/** 威脅評估 */
USTRUCT(BlueprintType)
struct FThreatAssessment
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    EAIThreatLevel ThreatLevel;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    FString ThreatSource;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    float ThreatMagnitude;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    TArray<FString> VulnerablePoints;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    TArray<FString> MitigationStrategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    float TimeToImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Threat Assessment")
    bool bIsImmediate;
    
    FThreatAssessment()
        : ThreatLevel(EAIThreatLevel::None)
        , ThreatMagnitude(0.0f)
        , TimeToImpact(0.0f)
        , bIsImmediate(false)
    {}
};

/** 適配學習數據 */
USTRUCT(BlueprintType)
struct FAdaptationLearningData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    int32 EncounterCount;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    float SuccessRate;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    TMap<EAIAdaptationResponse, float> ResponseEffectiveness;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    TArray<ESixStrategyType> EffectiveCounters;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    TArray<ESixStrategyType> IneffectiveCounters;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    FDateTime LastEncounter;
    
    UPROPERTY(BlueprintReadWrite, Category = "Learning Data")
    bool bIsWellUnderstood;
    
    FAdaptationLearningData()
        : EncounterCount(0)
        , SuccessRate(0.5f)
        , bIsWellUnderstood(false)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOpponentIdentified,
    const FAICommanderProfile&, OpponentProfile,
    const FPlayerBehaviorAnalysis&, PlayerAnalysis);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAdaptationDecisionMade,
    const FAIAdaptationDecision&, Decision);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThreatDetected,
    const FThreatAssessment&, Threat);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyCountered,
    ESixStrategyType, PlayerStrategy,
    ESixStrategyType, AICounterStrategy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningUpdated,
    const FAdaptationLearningData&, LearningData);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * AI適配系統
 * 負責智能對手分析、玩家行為學習、適配決策生成、威脅評估
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|AI Adaptation")
class MINGGORTS_API UMingAIAdaptationSystem : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingAIAdaptationSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化AI適配系統 */
    UFUNCTION(BlueprintCallable, Category = "AI Adaptation")
    bool InitializeAIAdaptationSystem();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "AI Adaptation")
    void ResetAIAdaptationSystem();

    // ============================================================================
    // 對手分析
    // ============================================================================

    /** 識別對手類型 */
    UFUNCTION(BlueprintCallable, Category = "Opponent Analysis")
    FAICommanderProfile IdentifyOpponent(const FString& OpponentID);

    /** 分析對手行為 */
    UFUNCTION(BlueprintCallable, Category = "Opponent Analysis")
    FPlayerBehaviorAnalysis AnalyzeOpponentBehavior(const FString& OpponentID);

    /** 獲取對手檔案 */
    UFUNCTION(BlueprintCallable, Category = "Opponent Analysis")
    FAICommanderProfile GetOpponentProfile(const FString& OpponentID) const;

    /** 更新對手檔案 */
    UFUNCTION(BlueprintCallable, Category = "Opponent Analysis")
    bool UpdateOpponentProfile(const FString& OpponentID, const FAICommanderProfile& Profile);

    /** 檢測對手弱點 */
    UFUNCTION(BlueprintCallable, Category = "Opponent Analysis")
    TArray<EStrategyEffectType> DetectOpponentWeaknesses(const FString& OpponentID);

    // ============================================================================
    // 玩家行為學習
    // ============================================================================

    /** 記錄玩家行為 */
    UFUNCTION(BlueprintCallable, Category = "Player Learning")
    bool RecordPlayerBehavior(const FString& PlayerID, ESixStrategyType StrategyUsed, float Outcome);

    /** 分析玩家模式 */
    UFUNCTION(BlueprintCallable, Category = "Player Learning")
    FPlayerBehaviorAnalysis AnalyzePlayerPattern(const FString& PlayerID);

    /** 獲取玩家分析 */
    UFUNCTION(BlueprintCallable, Category = "Player Learning")
    FPlayerBehaviorAnalysis GetPlayerAnalysis(const FString& PlayerID) const;

    /** 預測玩家下一步 */
    UFUNCTION(BlueprintCallable, Category = "Player Learning")
    ESixStrategyType PredictPlayerNextMove(const FString& PlayerID);

    /** 獲取學習數據 */
    UFUNCTION(BlueprintCallable, Category = "Player Learning")
    FAdaptationLearningData GetLearningData(const FString& PlayerID) const;

    // ============================================================================
    // 適配決策生成
    // ============================================================================

    /** 生成適配決策 */
    UFUNCTION(BlueprintCallable, Category = "Adaptation Decision")
    FAIAdaptationDecision GenerateAdaptationDecision(const FString& OpponentID, ESixStrategyType OpponentStrategy);

    /** 選擇最佳反制策略 */
    UFUNCTION(BlueprintCallable, Category = "Adaptation Decision")
    ESixStrategyType SelectBestCounterStrategy(const FString& OpponentID, ESixStrategyType OpponentStrategy);

    /** 評估決策效果 */
    UFUNCTION(BlueprintCallable, Category = "Adaptation Decision")
    float EvaluateDecisionEffectiveness(const FAIAdaptationDecision& Decision) const;

    /** 生成決策建議 */
    UFUNCTION(BlueprintCallable, Category = "Adaptation Decision")
    TArray<FString> GenerateDecisionReasoning(const FAIAdaptationDecision& Decision) const;

    // ============================================================================
    // 威脅評估
    // ============================================================================

    /** 評估威脅等級 */
    UFUNCTION(BlueprintCallable, Category = "Threat Assessment")
    FThreatAssessment AssessThreat(const FString& OpponentID, ESixStrategyType OpponentStrategy);

    /** 獲取當前威脅列表 */
    UFUNCTION(BlueprintCallable, Category = "Threat Assessment")
    TArray<FThreatAssessment> GetCurrentThreats() const;

    /** 獲取最嚴重威脅 */
    UFUNCTION(BlueprintCallable, Category = "Threat Assessment")
    FThreatAssessment GetMostCriticalThreat() const;

    /** 生成緩解策略 */
    UFUNCTION(BlueprintCallable, Category = "Threat Assessment")
    TArray<FString> GenerateMitigationStrategies(const FThreatAssessment& Threat);

    // ============================================================================
    // AI行為控制
    // ============================================================================

    /** 設置AI決策模式 */
    UFUNCTION(BlueprintCallable, Category = "AI Control")
    bool SetAIDecisionMode(EAIDecisionMode Mode);

    /** 獲取當前決策模式 */
    UFUNCTION(BlueprintCallable, Category = "AI Control")
    EAIDecisionMode GetCurrentDecisionMode() const;

    /** 執行AI決策 */
    UFUNCTION(BlueprintCallable, Category = "AI Control")
    bool ExecuteAIDecision(const FAIAdaptationDecision& Decision);

    /** 切換AI類型 */
    UFUNCTION(BlueprintCallable, Category = "AI Control")
    bool SwitchAIType(EAIOpponentType NewType, const FString& Reason = TEXT("自動切換"));

    /** 獲取AI建議 */
    UFUNCTION(BlueprintCallable, Category = "AI Control")
    TArray<FAIAdaptationDecision> GetAIRecommendations(int32 MaxRecommendations = 3);

    // ============================================================================
    // 學習與優化
    // ============================================================================

    /** 更新學習數據 */
    UFUNCTION(BlueprintCallable, Category = "Learning")
    bool UpdateLearningData(const FString& OpponentID, EAIAdaptationResponse Response, float Effectiveness);

    /** 優化適配策略 */
    UFUNCTION(BlueprintCallable, Category = "Learning")
    bool OptimizeAdaptationStrategy(const FString& OpponentID);

    /** 清除學習數據 */
    UFUNCTION(BlueprintCallable, Category = "Learning")
    void ClearLearningData(const FString& OpponentID);

    /** 導出學習數據 */
    UFUNCTION(BlueprintCallable, Category = "Learning")
    FString ExportLearningData(const FString& OpponentID) const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 對手識別事件 */
    UPROPERTY(BlueprintAssignable, Category = "AI Adaptation Events")
    FOnOpponentIdentified OnOpponentIdentified;

    /** 適配決策事件 */
    UPROPERTY(BlueprintAssignable, Category = "AI Adaptation Events")
    FOnAdaptationDecisionMade OnAdaptationDecisionMade;

    /** 威脅檢測事件 */
    UPROPERTY(BlueprintAssignable, Category = "AI Adaptation Events")
    FOnThreatDetected OnThreatDetected;

    /** 策略反制事件 */
    UPROPERTY(BlueprintAssignable, Category = "AI Adaptation Events")
    FOnStrategyCountered OnStrategyCountered;

    /** 學習更新事件 */
    UPROPERTY(BlueprintAssignable, Category = "AI Adaptation Events")
    FOnLearningUpdated OnLearningUpdated;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 對手檔案庫 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    TMap<FString, FAICommanderProfile> OpponentProfiles;

    /** 玩家行為分析庫 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    TMap<FString, FPlayerBehaviorAnalysis> PlayerAnalyses;

    /** 學習數據庫 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    TMap<FString, FAdaptationLearningData> LearningDatabase;

    /** 當前威脅列表 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    TArray<FThreatAssessment> CurrentThreats;

    /** 當前AI檔案 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    FAICommanderProfile CurrentAIProfile;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "AI Adaptation")
    EAIDecisionMode CurrentDecisionMode;

    UPROPERTY(BlueprintReadWrite, Category = "AI Adaptation")
    float LearningRate;

    UPROPERTY(BlueprintReadWrite, Category = "AI Adaptation")
    int32 MinSamplesForPattern;

    UPROPERTY(BlueprintReadWrite, Category = "AI Adaptation")
    float AdaptationConfidenceThreshold;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "AI Adaptation")
    FString CurrentOpponentID;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 分析策略模式 */
    void AnalyzeStrategyPatterns(const FString& PlayerID);

    /** 識別行為趨勢 */
    void IdentifyBehaviorTrends(const FString& PlayerID);

    /** 生成反制策略映射 */
    TMap<ESixStrategyType, ESixStrategyType> GenerateCounterStrategyMap();

    /** 評估策略效果 */
    float EvaluateStrategyEffectiveness(ESixStrategyType Strategy, const FPlayerBehaviorAnalysis& Analysis);

    /** 選擇最佳反制 */
    ESixStrategyType SelectOptimalCounter(const TArray<ESixStrategyType>& Options, const FPlayerBehaviorAnalysis& Analysis);

    /** 更新威脅評估 */
    void UpdateThreatAssessments();

    /** 計算決策置信度 */
    float CalculateDecisionConfidence(const FAIAdaptationDecision& Decision);

    /** 生成決策理由 */
    TArray<FString> GenerateDecisionReasoningInternal(const FAIAdaptationDecision& Decision);

    /** 觸發對手識別事件 */
    void TriggerOpponentIdentifiedEvent(const FAICommanderProfile& Profile, const FPlayerBehaviorAnalysis& Analysis);

    /** 觸發決策事件 */
    void TriggerDecisionMadeEvent(const FAIAdaptationDecision& Decision);

    /** 觸發威脅事件 */
    void TriggerThreatDetectedEvent(const FThreatAssessment& Threat);

    /** 觸發學習更新事件 */
    void TriggerLearningUpdatedEvent(const FAdaptationLearningData& LearningData);

private:
    /** 定時器句柄 */
    FTimerHandle AnalysisTimer;
    FTimerHandle LearningUpdateTimer;

    /** 分析間隔 */
    static constexpr float ANALYSIS_INTERVAL = 10.0f;

    /** 學習更新間隔 */
    static constexpr float LEARNING_UPDATE_INTERVAL = 30.0f;

    /** 最小學習樣本數 */
    static constexpr int32 MIN_LEARNING_SAMPLES = 5;
};
