#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingWuXingRhythmSystem.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingSixStrategiesSystem;
class UMingStrategyAuthority;

// ============================================================================
// 五行節奏系統枚舉定義
// ============================================================================

/** 節奏狀態 */
UENUM(BlueprintType)
enum class EWuXingRhythmState : uint8
{
    Inactive        UMETA(DisplayName = "非活躍"),
    WarmingUp       UMETA(DisplayName = "預熱中"),
    Active          UMETA(DisplayName = "活躍"),
    Transitioning    UMETA(DisplayName = "轉換中"),
    CoolingDown     UMETA(DisplayName = "冷卻中")
};

/** 節奏模式 */
UENUM(BlueprintType)
enum class EWuXingRhythmPattern : uint8
{
    Linear          UMETA(DisplayName = "線性模式"),
    Cyclic          UMETA(DisplayName = "循環模式"),
    Adaptive        UMETA(DisplayName = "適應模式"),
    Strategic       UMETA(DisplayName = "戰略模式"),
    Emergency       UMETA(DisplayName = "緊急模式")
};

/** 節奏強度 */
UENUM(BlueprintType)
enum class EWuXingRhythmIntensity : uint8
{
    Weak            UMETA(DisplayName = "微弱"),
    Normal          UMETA(DisplayName = "正常"),
    Strong          UMETA(DisplayName = "強烈"),
    Intense         UMETA(DisplayName = "激烈"),
    Overwhelming    UMETA(DisplayName = "壓倒性")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 五行階段配置 */
USTRUCT(BlueprintType)
struct FWuXingPhaseConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    EFiveElementPhase Phase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    float DefaultDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    float MinDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    float MaxDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    float TransitionSpeed;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    TArray<EStrategyEffectType> PreferredEffects;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    TArray<ESixStrategyType> SupportedStrategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "Phase Config")
    FString PhaseDescription;
    
    FWuXingPhaseConfig()
        : Phase(EFiveElementPhase::None)
        , DefaultDuration(120.0f)
        , MinDuration(60.0f)
        , MaxDuration(300.0f)
        , TransitionSpeed(1.0f)
    {}
};

/** 節奏數據 */
USTRUCT(BlueprintType)
struct FWuXingRhythmData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    EWuXingRhythmState State;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    EWuXingRhythmPattern Pattern;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    EWuXingRhythmIntensity Intensity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    EFiveElementPhase PreviousPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    float PhaseProgress;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    float CurrentPhaseDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    float TotalRhythmTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    float RhythmStrength;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    TArray<EFiveElementPhase> PhaseSequence;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Data")
    int32 CurrentCycleIndex;
    
    FWuXingRhythmData()
        : State(EWuXingRhythmState::Inactive)
        , Pattern(EWuXingRhythmPattern::Cyclic)
        , Intensity(EWuXingRhythmIntensity::Normal)
        , CurrentPhase(EFiveElementPhase::None)
        , PreviousPhase(EFiveElementPhase::None)
        , PhaseProgress(0.0f)
        , CurrentPhaseDuration(0.0f)
        , TotalRhythmTime(0.0f)
        , RhythmStrength(0.0f)
        , CurrentCycleIndex(0)
    {}
};

/** 節奏事件 */
USTRUCT(BlueprintType)
struct FWuXingRhythmEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    EFiveElementPhase TriggerPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    FString EventName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    FString EventDescription;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    float EventStrength;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    FDateTime EventTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Event")
    TArray<FString> AffectedSystems;
    
    FWuXingRhythmEvent()
        : TriggerPhase(EFiveElementPhase::None)
        , EventStrength(0.0f)
    {}
};

/** 節奏統計 */
USTRUCT(BlueprintType)
struct FWuXingRhythmStatistics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    TMap<EFiveElementPhase, float> PhaseTimeDistribution;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    TMap<EFiveElementPhase, int32> PhaseTransitionCount;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    float AverageCycleTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    int32 TotalCycles;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    float RhythmEfficiency;
    
    UPROPERTY(BlueprintReadWrite, Category = "Rhythm Statistics")
    TArray<FWuXingRhythmEvent> SignificantEvents;
    
    FWuXingRhythmStatistics()
        : AverageCycleTime(0.0f)
        , TotalCycles(0)
        , RhythmEfficiency(0.0f)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingPhaseStarted,
    EFiveElementPhase, NewPhase,
    const FWuXingRhythmData&, RhythmData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingPhaseCompleted,
    EFiveElementPhase, CompletedPhase,
    float, Duration);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingRhythmStateChanged,
    EWuXingRhythmState, NewState,
    EWuXingRhythmState, PreviousState);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingRhythmPatternChanged,
    EWuXingRhythmPattern, NewPattern,
    const FString&, Reason);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWuXingRhythmEventTriggered,
    const FWuXingRhythmEvent&, RhythmEvent);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 五行節奏系統
 * 負責管理五行階段的節奏控制、轉換邏輯、效果協調
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Wu Xing Rhythm")
class MINGGORTS_API UMingWuXingRhythmSystem : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingWuXingRhythmSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化五行節奏系統 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Rhythm")
    bool InitializeWuXingRhythmSystem();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Rhythm")
    void ResetWuXingRhythmSystem();

    // ============================================================================
    // 節奏控制
    // ============================================================================

    /** 啟動五行節奏 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool StartWuXingRhythm(EWuXingRhythmPattern Pattern = EWuXingRhythmPattern::Cyclic);

    /** 停止五行節奏 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool StopWuXingRhythm();

    /** 暫停五行節奏 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool PauseWuXingRhythm();

    /** 恢復五行節奏 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool ResumeWuXingRhythm();

    /** 設置節奏模式 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool SetRhythmPattern(EWuXingRhythmPattern Pattern, const FString& Reason = TEXT("手動設置"));

    /** 設置節奏強度 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Control")
    bool SetRhythmIntensity(EWuXingRhythmIntensity Intensity);

    // ============================================================================
    // 階段管理
    // ============================================================================

    /** 設置五行階段序列 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    bool SetPhaseSequence(const TArray<EFiveElementPhase>& Sequence);

    /** 強制切換到指定階段 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    bool ForceTransitionToPhase(EFiveElementPhase TargetPhase);

    /** 獲取當前階段 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    EFiveElementPhase GetCurrentPhase() const;

    /** 獲取階段進度 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    float GetPhaseProgress() const;

    /** 獲取階段剩餘時間 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    float GetPhaseRemainingTime() const;

    /** 獲取下一階段 */
    UFUNCTION(BlueprintCallable, Category = "Phase Management")
    EFiveElementPhase GetNextPhase() const;

    // ============================================================================
    // 階段配置
    // ============================================================================

    /** 設置階段配置 */
    UFUNCTION(BlueprintCallable, Category = "Phase Configuration")
    bool SetPhaseConfig(EFiveElementPhase Phase, const FWuXingPhaseConfig& Config);

    /** 獲取階段配置 */
    UFUNCTION(BlueprintCallable, Category = "Phase Configuration")
    FWuXingPhaseConfig GetPhaseConfig(EFiveElementPhase Phase) const;

    /** 獲取所有階段配置 */
    UFUNCTION(BlueprintCallable, Category = "Phase Configuration")
    TArray<FWuXingPhaseConfig> GetAllPhaseConfigs() const;

    /** 重置階段配置到默認值 */
    UFUNCTION(BlueprintCallable, Category = "Phase Configuration")
    void ResetPhaseConfigsToDefault();

    // ============================================================================
    // 節奏數據
    // ============================================================================

    /** 獲取當前節奏數據 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Data")
    FWuXingRhythmData GetCurrentRhythmData() const;

    /** 獲取節奏統計 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Data")
    FWuXingRhythmStatistics GetRhythmStatistics() const;

    /** 獲取節奏強度 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Data")
    float GetRhythmStrength() const;

    /** 計算節奏效率 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Data")
    float CalculateRhythmEfficiency() const;

    /** 獲取推薦的下一階段 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Data")
    EFiveElementPhase GetRecommendedNextPhase() const;

    // ============================================================================
    // 事件系統
    // ============================================================================

    /** 觸發節奏事件 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Events")
    bool TriggerRhythmEvent(const FWuXingRhythmEvent& Event);

    /** 獲取最近事件 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Events")
    TArray<FWuXingRhythmEvent> GetRecentEvents(int32 MaxCount = 10) const;

    /** 清除事件歷史 */
    UFUNCTION(BlueprintCallable, Category = "Rhythm Events")
    void ClearEventHistory();

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 五行階段開始事件 */
    UPROPERTY(BlueprintAssignable, Category = "Wu Xing Rhythm Events")
    FOnWuXingPhaseStarted OnWuXingPhaseStarted;

    /** 五行階段完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Wu Xing Rhythm Events")
    FOnWuXingPhaseCompleted OnWuXingPhaseCompleted;

    /** 節奏狀態改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Wu Xing Rhythm Events")
    FOnWuXingRhythmStateChanged OnWuXingRhythmStateChanged;

    /** 節奏模式改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Wu Xing Rhythm Events")
    FOnWuXingRhythmPatternChanged OnWuXingRhythmPatternChanged;

    /** 節奏事件觸發事件 */
    UPROPERTY(BlueprintAssignable, Category = "Wu Xing Rhythm Events")
    FOnWuXingRhythmEventTriggered OnWuXingRhythmEventTriggered;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 當前節奏數據 */
    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    FWuXingRhythmData CurrentRhythmData;

    /** 階段配置映射 */
    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    TMap<EFiveElementPhase, FWuXingPhaseConfig> PhaseConfigs;

    /** 節奏統計 */
    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    FWuXingRhythmStatistics RhythmStatistics;

    /** 事件歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    TArray<FWuXingRhythmEvent> EventHistory;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Rhythm")
    float DefaultPhaseDuration;

    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Rhythm")
    float RhythmUpdateInterval;

    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Rhythm")
    float MaxEventHistory;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "Wu Xing Rhythm")
    bool bRhythmActive;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 初始化默認階段配置 */
    void InitializeDefaultPhaseConfigs();

    /** 更新節奏狀態 */
    void UpdateRhythmState(float DeltaTime);

    /** 處理階段轉換 */
    void HandlePhaseTransition();

    /** 計算階段持續時間 */
    float CalculatePhaseDuration(EFiveElementPhase Phase) const;

    /** 應用階段效果 */
    void ApplyPhaseEffects(EFiveElementPhase Phase);

    /** 更新節奏統計 */
    void UpdateRhythmStatistics();

    /** 記錄節奏事件 */
    void RecordRhythmEvent(const FWuXingRhythmEvent& Event);

    /** 觸發階段開始事件 */
    void TriggerPhaseStartedEvent(EFiveElementPhase NewPhase);

    /** 觸發階段完成事件 */
    void TriggerPhaseCompletedEvent(EFiveElementPhase CompletedPhase);

    /** 計算節奏強度 */
    float CalculateRhythmStrength() const;

    /** 適應性節奏調整 */
    void AdaptiveRhythmAdjustment();

private:
    /** 定時器句柄 */
    FTimerHandle RhythmUpdateTimer;

    /** 最大事件歷史數量 */
    static constexpr int32 MAX_EVENT_HISTORY_SIZE = 100;

    /** 默認節奏更新間隔 */
    static constexpr float DEFAULT_RHYTHM_UPDATE_INTERVAL = 0.1f;
};
