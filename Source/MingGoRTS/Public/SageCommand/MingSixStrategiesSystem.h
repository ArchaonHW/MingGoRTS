#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingSixStrategiesSystem.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingStrategyAuthority;
class UMingMilitaryAuthority;

// ============================================================================
// 六策系統枚舉定義
// ============================================================================

/** 策略執行狀態 */
UENUM(BlueprintType)
enum class EStrategyExecutionStatus : uint8
{
    None                UMETA(DisplayName = "無"),
    Preparing           UMETA(DisplayName = "準備中"),
    Executing           UMETA(DisplayName = "執行中"),
    Sustaining          UMETA(DisplayName = "維持中"),
    Completed           UMETA(DisplayName = "已完成"),
    Failed              UMETA(DisplayName = "失敗"),
    Cancelled           UMETA(DisplayName = "已取消")
};

/** 策略效果類型 */
UENUM(BlueprintType)
enum class EStrategyEffectType : uint8
{
    None                UMETA(DisplayName = "無"),
    MoraleBoost         UMETA(DisplayName = "士氣提升"),
    EconomicGrowth      UMETA(DisplayName = "經濟增長"),
    MilitaryAdvantage   UMETA(DisplayName = "軍事優勢"),
    DiplomaticInfluence  UMETA(DisplayName = "外交影響"),
    PsychologicalWarfare UMETA(DisplayName = "心理戰"),
    StrategicPositioning UMETA(DisplayName = "戰略定位"),
    ResourceAcquisition  UMETA(DisplayName = "資源獲取")
};

/** 五行相生相剋關係 */
UENUM(BlueprintType)
enum class EWuXingRelationship : uint8
{
    None                UMETA(DisplayName = "無"),
    Generating          UMETA(DisplayName = "相生"),
    Overcoming          UMETA(DisplayName = "相剋"),
    Generated           UMETA(DisplayName = "被生"),
    Overcome            UMETA(DisplayName = "被剋")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 策略執行上下文 */
USTRUCT(BlueprintType)
struct FStrategyExecutionContext
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    ESixStrategyType StrategyType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    EStrategyDirection Direction;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    EFiveElementPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    float ExecutionProgress;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    float EffectIntensity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    FDateTime StartTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    FDateTime ExpectedEndTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    TArray<FString> TargetIDs;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Context")
    FString InitiatorID;
    
    FStrategyExecutionContext()
        : StrategyType(ESixStrategyType::None)
        , Direction(EStrategyDirection::Righteous)
        , CurrentPhase(EFiveElementPhase::None)
        , ExecutionProgress(0.0f)
        , EffectIntensity(1.0f)
    {}
};

/** 策略效果詳細數據 */
USTRUCT(BlueprintType)
struct FStrategyEffectDetails
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    EStrategyEffectType EffectType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float BaseValue;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float CurrentValue;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float Duration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    float DecayRate;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    bool bIsPermanent;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Effect")
    TArray<FString> AffectedTargets;
    
    FStrategyEffectDetails()
        : EffectType(EStrategyEffectType::None)
        , BaseValue(0.0f)
        , CurrentValue(0.0f)
        , Duration(0.0f)
        , DecayRate(0.0f)
        , bIsPermanent(false)
    {}
};

/** 策略組合數據 */
USTRUCT(BlueprintType)
struct FStrategyCombination
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Combination")
    TArray<ESixStrategyType> Strategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Combination")
    float SynergyBonus;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Combination")
    float RiskMultiplier;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Combination")
    EFiveElementPhase OptimalPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Strategy Combination")
    TArray<EStrategyEffectType> CombinedEffects;
    
    FStrategyCombination()
        : SynergyBonus(0.0f)
        , RiskMultiplier(1.0f)
        , OptimalPhase(EFiveElementPhase::None)
    {}
};

/** 五行策略映射 */
USTRUCT(BlueprintType)
struct FWuXingStrategyMapping
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Mapping")
    EFiveElementPhase ElementPhase;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Mapping")
    TArray<ESixStrategyType> SupportedStrategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Mapping")
    float ElementBonus;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Mapping")
    EWuXingRelationship RelationshipWithNext;
    
    UPROPERTY(BlueprintReadWrite, Category = "Wu Xing Mapping")
    EWuXingRelationship RelationshipWithPrevious;
    
    FWuXingStrategyMapping()
        : ElementPhase(EFiveElementPhase::None)
        , ElementBonus(1.0f)
        , RelationshipWithNext(EWuXingRelationship::None)
        , RelationshipWithPrevious(EWuXingRelationship::None)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyExecutionStarted,
    ESixStrategyType, StrategyType,
    const FStrategyExecutionContext&, Context);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStrategyExecutionCompleted,
    ESixStrategyType, StrategyType,
    bool bSuccess);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStrategyEffectApplied,
    EStrategyEffectType, EffectType,
    float, EffectValue,
    const TArray<FString>& Targets);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWuXingPhaseTransition,
    EFiveElementPhase, FromPhase,
    EFiveElementPhase, ToPhase);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyCombinationActivated,
    const FStrategyCombination&, Combination);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 六策系統管理器
 * 負責正逆六策的執行、效果管理、五行協調、策略組合
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Six Strategies")
class MINGGORTS_API UMingSixStrategiesSystem : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingSixStrategiesSystem();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化六策系統 */
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool InitializeSixStrategiesSystem();

    /** 重置系統到初始狀態 */
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    void ResetSixStrategiesSystem();

    // ============================================================================
    // 策略執行管理
    // ============================================================================

    /** 執行策略 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Execution")
    bool ExecuteStrategy(ESixStrategyType StrategyType, const FStrategyExecutionContext& Context);

    /** 停止策略執行 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Execution")
    bool StopStrategyExecution(ESixStrategyType StrategyType);

    /** 獲取策略執行狀態 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Execution")
    EStrategyExecutionStatus GetStrategyExecutionStatus(ESixStrategyType StrategyType) const;

    /** 獲取活動策略列表 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Execution")
    TArray<ESixStrategyType> GetActiveStrategies() const;

    /** 更新策略執行進度 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Execution")
    void UpdateStrategyProgress(ESixStrategyType StrategyType, float DeltaProgress);

    // ============================================================================
    // 策略效果管理
    // ============================================================================

    /** 應用策略效果 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Effects")
    bool ApplyStrategyEffect(EStrategyEffectType EffectType, float Value, const TArray<FString>& Targets);

    /** 移除策略效果 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Effects")
    bool RemoveStrategyEffect(EStrategyEffectType EffectType, const TArray<FString>& Targets);

    /** 獲取當前效果列表 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Effects")
    TArray<FStrategyEffectDetails> GetCurrentEffects() const;

    /** 計算策略效果強度 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Effects")
    float CalculateEffectIntensity(EStrategyEffectType EffectType, EFiveElementPhase CurrentPhase) const;

    /** 更新效果衰減 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Effects")
    void UpdateEffectDecay(float DeltaTime);

    // ============================================================================
    // 五行策略協調
    // ============================================================================

    /** 設置五行策略映射 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Coordination")
    bool SetWuXingStrategyMapping(const TArray<FWuXingStrategyMapping>& Mappings);

    /** 獲取當前五行加成 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Coordination")
    float GetWuXingBonus(ESixStrategyType StrategyType, EFiveElementPhase CurrentPhase) const;

    /** 檢查五行相生相剋關係 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Coordination")
    EWuXingRelationship CheckWuXingRelationship(EFiveElementPhase Phase1, EFiveElementPhase Phase2) const;

    /** 計算五行協調加成 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Coordination")
    float CalculateWuXingCoordinationBonus(const TArray<ESixStrategyType>& Strategies, EFiveElementPhase CurrentPhase) const;

    /** 獲取推薦的五行階段 */
    UFUNCTION(BlueprintCallable, Category = "Wu Xing Coordination")
    EFiveElementPhase GetRecommendedWuXingPhase(ESixStrategyType StrategyType) const;

    // ============================================================================
    // 策略組合系統
    // ============================================================================

    /** 創建策略組合 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Combination")
    FStrategyCombination CreateStrategyCombination(const TArray<ESixStrategyType>& Strategies);

    /** 激活策略組合 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Combination")
    bool ActivateStrategyCombination(const FStrategyCombination& Combination);

    /** 獲取可用組合 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Combination")
    TArray<FStrategyCombination> GetAvailableCombinations() const;

    /** 計算組合協效加成 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Combination")
    float CalculateSynergyBonus(const TArray<ESixStrategyType>& Strategies) const;

    /** 驗證組合有效性 */
    UFUNCTION(BlueprintCallable, Category = "Strategy Combination")
    bool ValidateStrategyCombination(const TArray<ESixStrategyType>& Strategies) const;

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 策略執行開始事件 */
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies Events")
    FOnStrategyExecutionStarted OnStrategyExecutionStarted;

    /** 策略執行完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies Events")
    FOnStrategyExecutionCompleted OnStrategyExecutionCompleted;

    /** 策略效果應用事件 */
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies Events")
    FOnStrategyEffectApplied OnStrategyEffectApplied;

    /** 五行階段轉換事件 */
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies Events")
    FOnWuXingPhaseTransition OnWuXingPhaseTransition;

    /** 策略組合激活事件 */
    UPROPERTY(BlueprintAssignable, Category = "Six Strategies Events")
    FOnStrategyCombinationActivated OnStrategyCombinationActivated;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 活動策略執行上下文 */
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TMap<ESixStrategyType, FStrategyExecutionContext> ActiveStrategies;

    /** 當前策略效果 */
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TArray<FStrategyEffectDetails> CurrentEffects;

    /** 五行策略映射 */
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TArray<FWuXingStrategyMapping> WuXingMappings;

    /** 活動策略組合 */
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TArray<FStrategyCombination> ActiveCombinations;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Six Strategies")
    float DefaultEffectDecayRate;

    UPROPERTY(BlueprintReadWrite, Category = "Six Strategies")
    float MaxSynergyBonus;

    UPROPERTY(BlueprintReadWrite, Category = "Six Strategies")
    float WuXingCoordinationMultiplier;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    bool bSystemInitialized;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 初始化五行映射 */
    void InitializeWuXingMappings();

    /** 更新策略執行 */
    void UpdateStrategyExecutions(float DeltaTime);

    /** 處理策略完成 */
    void HandleStrategyCompletion(ESixStrategyType StrategyType, bool bSuccess);

    /** 計算策略基礎效果 */
    float CalculateBaseStrategyEffect(ESixStrategyType StrategyType) const;

    /** 應用五行加成到效果 */
    float ApplyWuXingBonusToEffect(float BaseEffect, EFiveElementPhase Phase) const;

    /** 檢查策略衝突 */
    bool CheckStrategyConflict(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const;

    /** 解析策略組合 */
    void ResolveStrategyCombination(const FStrategyCombination& Combination);

    /** 觸發策略事件 */
    void TriggerStrategyEvent(ESixStrategyType StrategyType, const FString& EventType);

private:
    /** 定時器句柄 */
    FTimerHandle StrategyUpdateTimer;

    /** 最大同時執行策略數 */
    static constexpr int32 MAX_CONCURRENT_STRATEGIES = 3;

    /** 策略更新間隔 */
    static constexpr float STRATEGY_UPDATE_INTERVAL = 0.5f;
};
