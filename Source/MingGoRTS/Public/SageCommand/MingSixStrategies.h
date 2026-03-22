#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSixStrategies.generated.h"

// 策略類型枚舉
UENUM(BlueprintType)
enum class ESixStrategyType: uint8 {
    HeavenStrategy = 0,    // 天道策略 - 順應天時，利用自然
    EarthStrategy = 1,     // 地道策略 - 利用地形，佔據要地
    HumanStrategy = 2,     // 人道策略 - 收買人心，分化敵人
    HeavenEarthStrategy = 3, // 天地策略 - 天地配合，整體作戰
    HeavenHumanStrategy = 4, // 天人策略 - 天人合一，精神戰
    EarthHumanStrategy = 5  // 地人策略 - 地人協調，持久戰
};

// 策略狀態枚舉
UENUM(BlueprintType)
enum class EStrategyState: uint8 {
    Planning = 0,      // 計劃階段
    Preparing = 1,     // 準備階段
    Executing = 2,     // 執行階段
    Monitoring = 3,    // 監控階段
    Completed = 4,     // 完成階段
    Failed = 5          // 失敗階段
};

// 策略複雜度
UENUM(BlueprintType)
enum class EStrategyComplexity: uint8 {
    Simple = 0,        // 簡單策略
    Moderate = 1,      // 中等複雜度
    Complex = 2,       // 複雜策略
    Master = 3         // 大師級策略
};

// 策略評估結果
USTRUCT(BlueprintType)
struct FStrategyEvaluation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESixStrategyType StrategyType;

    UPROPERTY(BlueprintReadOnly)
    float SuccessProbability = 0.0f;    // 成功概率 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float RiskLevel = 0.0f;              // 風險水平 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float ResourceCost = 0.0f;           // 資源成本 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float TimeRequired = 0.0f;           // 所需時間 (0-100)

    UPROPERTY(BlueprintReadOnly)
    float StrategicValue = 0.0f;         // 戰略價值 (0-100)

    UPROPERTY(BlueprintReadOnly)
    FString Recommendation;               // 建議
};

// 策略執行計劃
USTRUCT(BlueprintType)
struct FStrategyExecutionPlan
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    ESixStrategyType StrategyType;

    UPROPERTY(BlueprintReadOnly)
    EStrategyState CurrentState = EStrategyState::Planning;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ExecutionSteps;       // 執行步驟

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredResources;    // 所需資源

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskFactors;          // 風險因素

    UPROPERTY(BlueprintReadOnly)
    float EstimatedDuration = 0.0f;       // 預計持續時間

    UPROPERTY(BlueprintReadOnly)
    float ProgressPercentage = 0.0f;      // 進度百分比
};

// 六策事件
USTRUCT(BlueprintType)
struct FSixStrategyEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    ESixStrategyType StrategyType;

    UPROPERTY(BlueprintReadOnly)
    EStrategyState EventState;

    UPROPERTY(BlueprintReadOnly)
    float ImpactLevel = 0.0f;            // 影響程度 (0-100)

    UPROPERTY(BlueprintReadOnly)
    FDateTime Timestamp;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyStateChanged, ESixStrategyType, StrategyType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyEvaluated, const FStrategyEvaluation&, Evaluation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategyExecutionStarted, const FStrategyExecutionPlan&, Plan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSixStrategyEventOccurred, const FSixStrategyEvent&, Event);

/**
 * 六策系統
 * 基於六策的戰略決策和執行系統
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (SageCommand))
class MINGRTS_API UMingSixStrategies : public UObject
{
    GENERATED_BODY()

public:
    UMingSixStrategies();

    // 系統初始化和清理
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool Initialize();

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    void Cleanup();

    // 策略規劃
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    FStrategyEvaluation EvaluateStrategy(ESixStrategyType StrategyType, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FStrategyEvaluation> EvaluateAllStrategies(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    ESixStrategyType GetOptimalStrategy(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FString> GetStrategyRecommendations(const FString& Context);

    // 策略執行
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool ExecuteStrategy(ESixStrategyType StrategyType, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    FStrategyExecutionPlan CreateExecutionPlan(ESixStrategyType StrategyType, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool StartStrategyExecution(const FStrategyExecutionPlan& Plan);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool PauseStrategyExecution(ESixStrategyType StrategyType);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool ResumeStrategyExecution(ESixStrategyType StrategyType);

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool CancelStrategyExecution(ESixStrategyType StrategyType);

    // 策略監控
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    EStrategyState GetStrategyState(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    float GetStrategyProgress(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FString> GetActiveStrategies() const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool IsStrategyExecuting(ESixStrategyType StrategyType) const;

    // 策略分析
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<ESixStrategyType> GetCompatibleStrategies(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<ESixStrategyType> GetConflictingStrategies(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    float CalculateStrategySynergy(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    EStrategyComplexity GetStrategyComplexity(ESixStrategyType StrategyType) const;

    // 策略效果
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    float CalculateStrategicAdvantage(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FString> GetStrategyEffects(ESixStrategyType StrategyType) const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FString> GetStrategyRequirements(ESixStrategyType StrategyType) const;

    // 歷史記錄
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    TArray<FSixStrategyEvent> GetStrategyHistory() const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    FSixStrategyEvent GetLastStrategyEvent() const;

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    void ClearStrategyHistory();

    // 系統狀態
    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    bool IsSystemActive() const { return bSystemActive; }

    UFUNCTION(BlueprintCallable, Category = "Six Strategies")
    float GetSystemStability() const { return SystemStability; }

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnStrategyStateChanged OnStrategyStateChanged;

    UPROPERTY(BlueprintAssignable)
    FOnStrategyEvaluated OnStrategyEvaluated;

    UPROPERTY(BlueprintAssignable)
    FOnStrategyExecutionStarted OnStrategyExecutionStarted;

    UPROPERTY(BlueprintAssignable)
    FOnSixStrategyEventOccurred OnSixStrategyEventOccurred;

protected:
    // 系統狀態
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    bool bSystemActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    float SystemStability = 100.0f;

    // 策略狀態映射
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TMap<ESixStrategyType, EStrategyState> StrategyStates;

    // 策略執行計劃
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TMap<ESixStrategyType, FStrategyExecutionPlan> ExecutionPlans;

    // 策略歷史記錄
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TArray<FSixStrategyEvent> StrategyHistory;

    // 當前執行中的策略
    UPROPERTY(BlueprintReadOnly, Category = "Six Strategies")
    TArray<ESixStrategyType> ActiveStrategies;

private:
    // 策略評估算法
    float EvaluateHeavenStrategy(const FString& Context);
    float EvaluateEarthStrategy(const FString& Context);
    float EvaluateHumanStrategy(const FString& Context);
    float EvaluateHeavenEarthStrategy(const FString& Context);
    float EvaluateHeavenHumanStrategy(const FString& Context);
    float EvaluateEarthHumanStrategy(const FString& Context);

    // 風險評估
    float CalculateStrategyRisk(ESixStrategyType StrategyType, const FString& Context);
    float CalculateResourceRequirements(ESixStrategyType StrategyType);
    float CalculateTimeRequirements(ESixStrategyType StrategyType);

    // 策略執行
    bool ExecuteHeavenStrategy(const FString& Context);
    bool ExecuteEarthStrategy(const FString& Context);
    bool ExecuteHumanStrategy(const FString& Context);
    bool ExecuteHeavenEarthStrategy(const FString& Context);
    bool ExecuteHeavenHumanStrategy(const FString& Context);
    bool ExecuteEarthHumanStrategy(const FString& Context);

    // 策略監控
    void UpdateStrategyExecution(float DeltaTime);
    void UpdateStrategyProgress(ESixStrategyType StrategyType, float DeltaTime);
    void CheckStrategyCompletion(ESixStrategyType StrategyType);

    // 策略分析
    TArray<ESixStrategyType> GetSynergisticStrategies(ESixStrategyType StrategyType) const;
    TArray<ESixStrategyType> GetAntagonisticStrategies(ESixStrategyType StrategyType) const;
    float CalculateCompatibilityScore(ESixStrategyType Strategy1, ESixStrategyType Strategy2) const;

    // 事件處理
    void RecordStrategyEvent(const FString& Description, ESixStrategyType StrategyType, EStrategyState State, float Impact);
    void ProcessStrategyEvent(const FSixStrategyEvent& Event);

    // 輔助方法
    FString GetStrategyName(ESixStrategyType StrategyType) const;
    FString GetStateName(EStrategyState State) const;
    FString GetComplexityName(EStrategyComplexity Complexity) const;
    EStrategyComplexity DetermineStrategyComplexity(ESixStrategyType StrategyType) const;

    // 策略效果計算
    TArray<FString> GetHeavenStrategyEffects() const;
    TArray<FString> GetEarthStrategyEffects() const;
    TArray<FString> GetHumanStrategyEffects() const;
    TArray<FString> GetHeavenEarthStrategyEffects() const;
    TArray<FString> GetHeavenHumanStrategyEffects() const;
    TArray<FString> GetEarthHumanStrategyEffects() const;

    // 配置參數
    UPROPERTY(Config)
    float StrategyExecutionSpeed = 1.0f;

    UPROPERTY(Config)
    float RiskAssessmentAccuracy = 0.8f;

    UPROPERTY(Config)
    float SynergyBonusMultiplier = 1.2f;

    // 定時器句柄
    FTimerHandle ExecutionUpdateTimer;
};
