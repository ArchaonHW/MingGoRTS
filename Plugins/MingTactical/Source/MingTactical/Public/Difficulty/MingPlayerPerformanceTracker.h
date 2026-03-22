#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingPlayerPerformanceTracker.generated.h"

/**
 * 玩家表現數據點 | Player Performance Data Point
 * 存儲單個時間點玩家表現數據 | Store player performance data at a single time point
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FPlayerPerformanceDataPoint
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float Timestamp = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float UnitLossRate = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float ResourceEfficiency = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float MissionCompletionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float ExpectedCompletionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 UnitsLost = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    int32 TotalUnits = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float ResourcesCollected = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float ExpectedResourceCollection = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance")
    float CombatEffectiveness = 0.0f;

    FPlayerPerformanceDataPoint() = default;
    explicit FPlayerPerformanceDataPoint(float InTimestamp)
        : Timestamp(InTimestamp)
    {}

    /** 計算綜合表現分數 0-100 | Calculate overall performance score 0-100 */
    float CalculatePerformanceScore() const;

    /** 驗證數據有效性 | Validate data validity */
    bool IsValid() const;
};

/**
 * 玩家表现统计
 * 聚合多个数据点的统计信息
 */
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FPlayerPerformanceStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float AveragePerformanceScore = 50.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float TrendSlope = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float Volatility = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    int32 SampleCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    EPlayerPerformanceRating OverallRating = EPlayerPerformanceRating::Average;

    UPROPERTY(BlueprintReadOnly, Category = "Stats")
    float RecommendationScore = 0.0f;

    void UpdateFromDataPoints(const TArray<FPlayerPerformanceDataPoint>& DataPoints);
};

/**
 * 玩家表现追踪器
 * 实时监控和评估玩家技能水平
 * 
 * 功能：
 * - 单位损失率追踪
 * - 资源收集效率追踪
 * - 任务完成时间追踪
 * - 综合技能指数计算
 */
UCLASS()
class MINGTACTICAL_API UMingPlayerPerformanceTracker : public UObject
{
    GENERATED_BODY()

public:
    UMingPlayerPerformanceTracker();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime);

    // ==== 事件追踪接口 ====

    /** 记录单位损失事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitLost(int32 UnitId, int32 UnitType, float UnitValue);

    /** 记录单位创建事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitCreated(int32 UnitId, int32 UnitType, float UnitCost);

    /** 记录资源收集事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordResourceCollected(float ResourceType, float Amount, float ExpectedRate);

    /** 记录任务开始 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionStarted(const FString& MissionId, float ExpectedDuration);

    /** 记录任务完成 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionCompleted(const FString& MissionId, bool bSuccess, float CompletionTime);

    /** 记录战斗结果 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordCombatResult(int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken);

    // ==== 性能评估接口 ====

    /** 获取当前表现统计 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    FPlayerPerformanceStats GetCurrentStats() const { return CurrentStats; }

    /** 获取技能指数 0-100 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    float GetSkillIndex() const { return CurrentStats.AveragePerformanceScore; }

    /** 获取玩家强项列表 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerStrengths() const;

    /** 获取玩家弱项列表 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerWeaknesses() const;

    /** 获取难度调整建议 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    EDifficultyChangeDirection GetDifficultyRecommendation() const;

    /** 是否累积了足够数据 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    bool HasSufficientData(int32 MinSamples = 5) const;

    // ==== 配置接口 ====

    /** 设置评估间隔（秒） */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetEvaluationInterval(float IntervalSeconds);

    /** 设置数据保留数量上限 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetMaxDataPoints(int32 MaxPoints);

    /** 重置所有追踪数据 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void ResetTracking();

    // ==== 调试接口 ====

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    FString GetDebugString() const;

protected:
    /** 定期评估表现 */
    void EvaluatePerformance();

    /** 计算趋势斜率 */
    float CalculateTrendSlope() const;

    /** 计算波动性 */
    float CalculateVolatility() const;

    /** 确定总体评级 */
    EPlayerPerformanceRating DetermineRating(float Score) const;

    /** 订阅核心事件 */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

private:
    bool bIsInitialized;
    float EvaluationInterval;
    float TimeSinceLastEvaluation;
    int32 MaxDataPoints;

    TArray<FPlayerPerformanceDataPoint> DataPoints;
    FPlayerPerformanceStats CurrentStats;

    // 当前追踪状态
    int32 CurrentTotalUnits;
    int32 CurrentUnitsLost;
    float CurrentResourcesCollected;
    float CurrentExpectedResources;
    TMap<FString, float> ActiveMissions;

    // 战斗统计
    int32 SessionEnemiesKilled;
    int32 SessionAlliesLost;
    float SessionDamageDealt;
    float SessionDamageTaken;

    // 历史最佳/最差
    float BestPerformanceScore;
    float WorstPerformanceScore;
};
