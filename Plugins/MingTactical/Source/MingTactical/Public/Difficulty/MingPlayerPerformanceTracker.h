#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingPlayerPerformanceTracker.generated.h"

/**
 * ??家表現??????| Player Performance Data Point
 * 存儲??個?X??X?家表現???? | Store player performance data at a single time point
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

    /** 計??綜??表現??數 0-100 | Calculate overall performance score 0-100 */
    float CalculatePerformanceScore() const;

    /** 驗?X??X??X| Validate data validity */
    bool IsValid() const;
};

/**
 * ??家表???
 * ????多???据??????信息
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

    void UpdateFromDataPoints(const TArray<FPlayerPerformanceDataPoint>& DataPoints};
};

/**
 * ??家表?追?X * ????控????估玩家?X?水?? * 
 * ??能?? * - ???X?失??追?? * - ??X??X???追?
 * - 任?完?X??追?
 * - ??X????X???? */
UCLASS()
class MINGTACTICAL_API UMingPlayerPerformanceTracker : public UObject
{
    GENERATED_BODY()

public:
    UMingPlayerPerformanceTracker();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime};

    // ==== 事件追???口 ====

    /** ??X??X?失事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitLost(int32 UnitId, int32 UnitType, float UnitValue};

    /** ??X??X?建事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitCreated(int32 UnitId, int32 UnitType, float UnitCost};

    /** ?????X???事件 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordResourceCollected(float ResourceType, float Amount, float ExpectedRate};

    /** ???任????*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionStarted(const FString& MissionId, float ExpectedDuration};

    /** ???任?完?? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionCompleted(const FString& MissionId, bool bSuccess, float CompletionTime};

    /** ??X?????? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordCombatResult(int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken};

    // ==== ??能?估??口 ====

    /** ???????表??? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    FPlayerPerformanceStats GetCurrentStats() const { return CurrentStats; }

    /** ???X????X0-100 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    float GetSkillIndex() const { return CurrentStats.AveragePerformanceScore; }

    /** ???X?家????表 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerStrengths() const;

    /** ???X?家弱???表 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerWeaknesses() const;

    /** ???X?度?整建? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    EDifficultyChangeDirection GetDifficultyRecommendation() const;

    /** ??否累?了足??X*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    bool HasSufficientData(int32 MinSamples = 5) const;

    // ==== ??置??口 ====

    /** ?置?估????（????*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetEvaluationInterval(float IntervalSeconds};

    /** ?置??据保?X???上?? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetMaxDataPoints(int32 MaxPoints};

    /** ??置????追??X*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void ResetTracking();

    // ==== ??X?口 ====

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    FString GetDebugString() const;

protected:
    /** 定???估表? */
    void EvaluatePerformance();

    /** ????????? */
    float CalculateTrendSlope() const;

    /** ???波?X*/
    float CalculateVolatility() const;

    /** 确?X????? */
    EPlayerPerformanceRating DetermineRating(float Score) const;

    /** ??X???事件 */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

private:
    bool bIsInitialized;
    float EvaluationInterval;
    float TimeSinceLastEvaluation;
    int32 MaxDataPoints;

    TArray<FPlayerPerformanceDataPoint> DataPoints;
    FPlayerPerformanceStats CurrentStats;

    // ???追?????    int32 CurrentTotalUnits;
    int32 CurrentUnitsLost;
    float CurrentResourcesCollected;
    float CurrentExpectedResources;
    TMap<FString, float> ActiveMissions;

    // ??????
    int32 SessionEnemiesKilled;
    int32 SessionAlliesLost;
    float SessionDamageDealt;
    float SessionDamageTaken;

    // ??史???X???    float BestPerformanceScore;
    float WorstPerformanceScore;
};

