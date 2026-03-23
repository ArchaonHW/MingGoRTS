#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingStrategyAuthority.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingThreePowerSystem;

// ============================================================================
// 策略系統枚舉定義
// ============================================================================

/** 策略方向 */
UENUM(BlueprintType)
enum class EStrategyDirection : uint8
{
    Righteous    UMETA(DisplayName = "正道"),
    Reverse      UMETA(DisplayName = "逆道"),
    Hybrid       UMETA(DisplayName = "混合")
};

/** 五行階段 */
UENUM(BlueprintType)
enum class EFiveElementPhase : uint8
{
    Wood    UMETA(DisplayName = "木 - 立名正當性"),
    Fire    UMETA(DisplayName = "火 - 造勢加速"),
    Earth   UMETA(DisplayName = "土 - 收權止亂"),
    Metal   UMETA(DisplayName = "金 - 裁斷清除"),
    Water   UMETA(DisplayName = "水 - 留白不動"),
    None    UMETA(DisplayName = "無")
};

/** 策略推薦類型 */
UENUM(BlueprintType)
enum class EStrategyRecommendationType : uint8
{
    Offensive     UMETA(DisplayName = "攻勢策略"),
    Defensive     UMETA(DisplayName = "守勢策略"),
    Economic      UMETA(DisplayName = "經濟策略"),
    Diplomatic    UMETA(DisplayName = "外交策略"),
    Adaptive      UMETA(DisplayName = "適應性策略")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 策略數據 */
USTRUCT(BlueprintType)
struct FStrategyData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    ESixStrategyType StrategyType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    EStrategyDirection Direction;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    EFiveElementPhase ElementPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    float CooldownDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    float Duration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    float EffectIntensity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    int32 MoralValueImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy")
    FString Description;
    
    FStrategyData()
        : StrategyType(ESixStrategyType::None)
        , Direction(EStrategyDirection::Righteous)
        , ElementPhase(EFiveElementPhase::None)
        , CooldownDuration(0.0f)
        , Duration(0.0f)
        , EffectIntensity(1.0f)
        , MoralValueImpact(0)
    {}
};

/** 策略效果 */
USTRUCT(BlueprintType)
struct FStrategyEffect
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float MoraleImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float RecruitmentSpeedImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float EconomicEfficiencyImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float LeaderGrowthImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float EnemyConfusionImpact;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float ReputationImpact;
    
    FStrategyEffect()
        : MoraleImpact(0.0f)
        , RecruitmentSpeedImpact(0.0f)
        , EconomicEfficiencyImpact(0.0f)
        , LeaderGrowthImpact(0.0f)
        , EnemyConfusionImpact(0.0f)
        , ReputationImpact(0.0f)
    {}
};

/** 陰陽切換決策數據 */
USTRUCT(BlueprintType)
struct FYinYangSwitchDecision
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    EStrategyDirection CurrentDirection;
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    EStrategyDirection RecommendedDirection;
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    float SwitchConfidence;
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    TArray<FString> Reasoning;
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Yin Yang")
    EFiveElementPhase RecommendedPhase;
    
    FYinYangSwitchDecision()
        : CurrentDirection(EStrategyDirection::Righteous)
        , RecommendedDirection(EStrategyDirection::Righteous)
        , SwitchConfidence(0.0f)
        , CurrentPhase(EFiveElementPhase::None)
        , RecommendedPhase(EFiveElementPhase::None)
    {}
};

/** 五行節奏數據 */
USTRUCT(BlueprintType)
struct FWuXingRhythmData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    float PhaseProgress;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    float PhaseDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    TArray<EFiveElementPhase> PhaseSequence;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    bool bIsTransitioning;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing")
    float TransitionProgress;
    
    FWuXingRhythmData()
        : CurrentPhase(EFiveElementPhase::None)
        , PhaseProgress(0.0f)
        , PhaseDuration(0.0f)
        , bIsTransitioning(false)
        , TransitionProgress(0.0f)
    {}
};

/** 策略推薦數據 */
USTRUCT(BlueprintType)
struct FStrategyRecommendation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    EStrategyRecommendationType RecommendationType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    ESixStrategyType RecommendedStrategy;
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    float Confidence;
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Justification;
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    FStrategyEffect ExpectedEffects;
    
    UPROPERTY(BlueprintReadWrite, Category = "Recommendation")
    TArray<FString> Risks;
    
    FStrategyRecommendation()
        : RecommendationType(EStrategyRecommendationType::Adaptive)
        , RecommendedStrategy(ESixStrategyType::None)
        , Confidence(0.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyDirectionChanged,
    EStrategyDirection, NewDirection,
    EStrategyDirection, PreviousDirection);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingPhaseChanged,
    EFiveElementPhase, NewPhase,
    EFiveElementPhase, PreviousPhase);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyRecommendationGenerated,
    const FStrategyRecommendation&, Recommendation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyExecuted,
    ESixStrategyType, StrategyType,
    bool bSuccess);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 策略權威系統
 * 負責正逆策略管理、陰陽切換決策、五行節奏控制、策略推薦系統
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Strategy Authority")
class MINGGORTS_API UMingStrategyAuthority : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingStrategyAuthority();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化策略權威系統 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    bool InitializeStrategyAuthority();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Authority")
    void ResetStrategyAuthority();

    // ============================================================================
    // 正逆策略管理
    // ============================================================================

    /** 執行策略 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    bool ExecuteStrategy(ESixStrategyType StrategyType);

    /** 獲取可用策略列表 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    TArray<ESixStrategyType> GetAvailableStrategies() const;

    /** 檢查策略是否可用 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    bool IsStrategyAvailable(ESixStrategyType StrategyType) const;

    /** 獲取策略數據 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    FStrategyData GetStrategyData(ESixStrategyType StrategyType) const;

    /** 獲取策略效果 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    FStrategyEffect GetStrategyEffect(ESixStrategyType StrategyType) const;

    /** 設置策略參數 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Management")
    void SetStrategyParameters(ESixStrategyType StrategyType, const FStrategyData& Data);

    // ============================================================================
    // 陰陽切換決策
    // ============================================================================

    /** 切換策略方向 */
    UFUNCTION(BlueprintCallable, Category = "Yin Yang")
    bool SwitchStrategyDirection(EStrategyDirection NewDirection);

    /** 獲取當前策略方向 */
    UFUNCTION(BlueprintCallable, Category = "Yin Yang")
    EStrategyDirection GetCurrentDirection() const;

    /** 分析是否需要切換方向 */
    UFUNCTION(BlueprintCallable, Category = "Yin Yang")
    FYinYangSwitchDecision AnalyzeDirectionSwitch() const;

    /** 強制切換到推薦方向 */
    UFUNCTION(BlueprintCallable, Category = "Yin Yang")
    bool ForceSwitchToRecommendedDirection();

    /** 獲取連續使用統計 */
    UFUNCTION(BlueprintCallable, Category = "Yin Yang")
    int32 GetConsecutiveDirectionUses(EStrategyDirection Direction) const;

    // ============================================================================
    // 五行節奏控制
    // ============================================================================

    /** 設置五行節奏 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    bool SetWuXingRhythm(const TArray<EFiveElementPhase>& PhaseSequence);

    /** 獲取當前五行階段 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    EFiveElementPhase GetCurrentWuXingPhase() const;

    /** 切換到下一五行階段 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    bool TransitionToNextWuXingPhase();

    /** 獲取五行節奏數據 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    FWuXingRhythmData GetWuXingRhythmData() const;

    /** 檢查五行相生相剋關係 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    float GetWuXingRelationship(EFiveElementPhase Phase1, EFiveElementPhase Phase2) const;

    /** 計算五行加成 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing")
    float CalculateWuXingBonus(EFiveElementPhase CurrentPhase, ESixStrategyType StrategyType) const;

    // ============================================================================
    // 策略推薦系統
    // ============================================================================

    /** 生成策略推薦 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Recommendation")
    FStrategyRecommendation GenerateStrategyRecommendation(EStrategyRecommendationType RecommendationType);

    /** 獲取推薦列表 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Recommendation")
    TArray<FStrategyRecommendation> GetRecommendations() const;

    /** 設置推薦參數 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Recommendation")
    void SetRecommendationParameters(float ConfidenceThreshold, int32 MaxRecommendations);

    /** 應用策略推薦 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Recommendation")
    bool ApplyRecommendation(const FStrategyRecommendation& Recommendation);

    /** 評估推薦質量 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Recommendation")
    float EvaluateRecommendationQuality(const FStrategyRecommendation& Recommendation) const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 策略方向改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority Events")
    FOnStrategyDirectionChanged OnStrategyDirectionChanged;

    /** 五行階段改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority Events")
    FOnWuXingPhaseChanged OnWuXingPhaseChanged;

    /** 策略推薦生成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority Events")
    FOnStrategyRecommendationGenerated OnStrategyRecommendationGenerated;

    /** 策略執行事件 */
    UPROPERTY(BlueprintAssignable, Category = "Strategy Authority Events")
    FOnStrategyExecuted OnStrategyExecuted;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 當前策略方向 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    EStrategyDirection CurrentDirection;

    /** 當前五行階段 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    EFiveElementPhase CurrentWuXingPhase;

    /** 策略映射表 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    TMap<ESixStrategyType, FStrategyData> StrategyMap;

    /** 策略效果映射表 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    TMap<ESixStrategyType, FStrategyEffect> StrategyEffectMap;

    /** 五行節奏數據 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    FWuXingRhythmData WuXingRhythm;

    /** 策略推薦列表 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    TArray<FStrategyRecommendation> Recommendations;

    /** 連續使用統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    int32 ConsecutiveRighteousUses;

    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    int32 ConsecutiveEvilUses;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Authority")
    float RecommendationConfidenceThreshold;

    UPROPERTY(BlueprintReadWrite, Category = "Strategy Authority")
    int32 MaxRecommendations;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Strategy Authority")
    bool bIsInitialized;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 初始化默認策略 */
    void InitializeDefaultStrategies();

    /** 更新五行節奏 */
    void UpdateWuXingRhythm(float DeltaTime);

    /** 分析當前局勢 */
    void AnalyzeCurrentSituation();

    /** 生成基於局勢的推薦 */
    void GenerateSituationBasedRecommendations();

    /** 計算策略切換成本 */
    float CalculateStrategySwitchCost(EStrategyDirection From, EStrategyDirection To) const;

    /** 驗證策略執行條件 */
    bool ValidateStrategyExecution(ESixStrategyType StrategyType) const;

    /** 應用策略效果 */
    void ApplyStrategyEffects(ESixStrategyType StrategyType);

    /** 更新連續使用統計 */
    void UpdateConsecutiveUsageStats(EStrategyDirection Direction);

    /** 觸發策略方向改變事件 */
    void TriggerDirectionChangedEvent(EStrategyDirection NewDirection);

    /** 觸發五行階段改變事件 */
    void TriggerPhaseChangedEvent(EFiveElementPhase NewPhase);

private:
    /** 定時器句柄 */
    FTimerHandle StrategyUpdateTimer;

    /** 最大連續使用次數 */
    static constexpr int32 MAX_CONSECUTIVE_USES = 5;

    /** 策略更新間隔 */
    static constexpr float STRATEGY_UPDATE_INTERVAL = 1.0f;
};
