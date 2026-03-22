#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingPlayerPerformanceTracker.generated.h"

/**
 * ??�a��{摧毀??| Player Performance Data Point
 * �s�x??��?X??X?�a��{摧毀 | Store player performance data at a single time point
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

    /** �p??��??��{??�� 0-100 | Calculate overall performance score 0-100 */
    float CalculatePerformanceScore() const;

    /** ��?X??X??X| Validate data validity */
    bool IsValid() const;
};

/**
 * ??�a��???
 * 摧毀�h???�u摧毀??�H��
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
);

/**
 * ??�a��?�l?X * 摧毀��摧毀����a?X?��?? * 
 * ??��?? * - ???X?��??�l?? * - ??X??X???�l?
 * - ��?��?X??�l?
 * - ??X?目標數量? */
UCLASS()
class MINGTACTICAL_API UMingPlayerPerformanceTracker : public UObject
{
    GENERATED_BODY()

public:
    UMingPlayerPerformanceTracker();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime);

    // ==== �ƥ�l???�f ====

    /** ??X??X?���ƥ� */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitLost(int32 UnitId, int32 UnitType, float UnitValue);

    /** ??X??X?�بƥ� */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitCreated(int32 UnitId, int32 UnitType, float UnitCost);

    /** ??目標數量�ƥ� */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordResourceCollected(float ResourceType, float Amount, float ExpectedRate);

    /** ???��摧毀*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionStarted(const FString& MissionId, float ExpectedDuration);

    /** ???��?��?? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionCompleted(const FString& MissionId, bool bSuccess, float CompletionTime);

    /** ??X摧毀?? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordCombatResult(int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken);

    // ==== ??��?��??�f ====

    /** 摧毀???��??? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    FPlayerPerformanceStats GetCurrentStats() const { return CurrentStats; }

    /** 目標數量?X0-100 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    float GetSkillIndex() const { return CurrentStats.AveragePerformanceScore; }

    /** ???X?�a摧毀�� */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerStrengths() const;

    /** ???X?�a�z???�� */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerWeaknesses() const;

    /** ???X?��?���? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    EDifficultyChangeDirection GetDifficultyRecommendation() const;

    /** ??�_��?�F��??X*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    bool HasSufficientData(int32 MinSamples = 5) const;

    // ==== ??�m??�f ====

    /** ?�m?��摧毀�]摧毀*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetEvaluationInterval(float IntervalSeconds);

    /** ?�m??�u�O?X???�W?? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetMaxDataPoints(int32 MaxPoints);

    /** ??�m摧毀�l??X*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void ResetTracking();

    // ==== ??X?�f ====

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    FString GetDebugString() const;

protected:
    /** �w???���? */
    void EvaluatePerformance();

    /** 摧毀摧毀? */
    float CalculateTrendSlope() const;

    /** ???�i?X*/
    float CalculateVolatility() const;

    /** ��?X摧毀? */
    EPlayerPerformanceRating DetermineRating(float Score) const;

    /** ??X???�ƥ� */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

private:
    bool bIsInitialized;
    float EvaluationInterval;
    float TimeSinceLastEvaluation;
    int32 MaxDataPoints;

    TArray<FPlayerPerformanceDataPoint> DataPoints;
    FPlayerPerformanceStats CurrentStats;

    // ???�l摧毀?
    int32 CurrentTotalUnits;
    int32 CurrentUnitsLost;
    float CurrentResourcesCollected;
    float CurrentExpectedResources;
    TMap<FString, float> ActiveMissions;

    // 摧毀??
    int32 SessionEnemiesKilled;
    int32 SessionAlliesLost;
    float SessionDamageDealt;
    float SessionDamageTaken;

    // ??�v目標數量
    float BestPerformanceScore;
    float WorstPerformanceScore;
};

