#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Difficulty/EDifficultyLevel.h"
#include "MingPlayerPerformanceTracker.generated.h"

/**
 * ?©å®¶è¡¨ç¾?¸æ?é»?| Player Performance Data Point
 * å­˜å„²?®å€‹æ??“é??©å®¶è¡¨ç¾?¸æ? | Store player performance data at a single time point
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

    /** è¨ˆç?ç¶œå?è¡¨ç¾?†æ•¸ 0-100 | Calculate overall performance score 0-100 */
    float CalculatePerformanceScore() const;

    /** é©—è??¸æ??‰æ???| Validate data validity */
    bool IsValid() const;
};

/**
 * ?©å®¶è¡¨ç°ç»Ÿè®¡
 * ?šå?å¤šä¸ª?°æ®?¹ç?ç»Ÿè®¡ä¿¡æ¯
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
 * ?©å®¶è¡¨ç°è¿½è¸ª?? * å®æ—¶?‘æ§?Œè?ä¼°ç©å®¶æ??½æ°´å¹? * 
 * ?Ÿèƒ½ï¼? * - ?•ä??Ÿå¤±?‡è¿½è¸? * - èµ„æ??¶é??ˆç?è¿½è¸ª
 * - ä»»åŠ¡å®Œæ??¶é—´è¿½è¸ª
 * - ç»¼å??€?½æ??°è®¡ç®? */
UCLASS()
class MINGTACTICAL_API UMingPlayerPerformanceTracker : public UObject
{
    GENERATED_BODY()

public:
    UMingPlayerPerformanceTracker();

    virtual void Initialize();
    virtual void Shutdown();
    void Tick(float DeltaTime);

    // ==== äº‹ä»¶è¿½è¸ª?¥å£ ====

    /** è®°å??•ä??Ÿå¤±äº‹ä»¶ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitLost(int32 UnitId, int32 UnitType, float UnitValue);

    /** è®°å??•ä??›å»ºäº‹ä»¶ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordUnitCreated(int32 UnitId, int32 UnitType, float UnitCost);

    /** è®°å?èµ„æ??¶é?äº‹ä»¶ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordResourceCollected(float ResourceType, float Amount, float ExpectedRate);

    /** è®°å?ä»»åŠ¡å¼€å§?*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionStarted(const FString& MissionId, float ExpectedDuration);

    /** è®°å?ä»»åŠ¡å®Œæ? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordMissionCompleted(const FString& MissionId, bool bSuccess, float CompletionTime);

    /** è®°å??˜æ?ç»“æ? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void RecordCombatResult(int32 EnemiesKilled, int32 AlliesLost, float DamageDealt, float DamageTaken);

    // ==== ?§èƒ½è¯„ä¼°?¥å£ ====

    /** ?·å?å½“å?è¡¨ç°ç»Ÿè®¡ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    FPlayerPerformanceStats GetCurrentStats() const { return CurrentStats; }

    /** ?·å??€?½æ???0-100 */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    float GetSkillIndex() const { return CurrentStats.AveragePerformanceScore; }

    /** ?·å??©å®¶å¼ºé¡¹?—è¡¨ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerStrengths() const;

    /** ?·å??©å®¶å¼±é¡¹?—è¡¨ */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    TArray<FString> GetPlayerWeaknesses() const;

    /** ?·å??¾åº¦è°ƒæ•´å»ºè®® */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    EDifficultyChangeDirection GetDifficultyRecommendation() const;

    /** ?¯å¦ç´¯ç§¯äº†è¶³å¤Ÿæ•°??*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    bool HasSufficientData(int32 MinSamples = 5) const;

    // ==== ?ç½®?¥å£ ====

    /** è®¾ç½®è¯„ä¼°?´é?ï¼ˆç?ï¼?*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetEvaluationInterval(float IntervalSeconds);

    /** è®¾ç½®?°æ®ä¿ç??°é?ä¸Šé? */
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void SetMaxDataPoints(int32 MaxPoints);

    /** ?ç½®?€?‰è¿½è¸ªæ•°??*/
    UFUNCTION(BlueprintCallable, Category = "Performance Tracking")
    void ResetTracking();

    // ==== è°ƒè??¥å£ ====

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    void PrintDebugInfo();

    UFUNCTION(BlueprintCallable, Category = "Performance Tracking|Debug")
    FString GetDebugString() const;

protected:
    /** å®šæ?è¯„ä¼°è¡¨ç° */
    void EvaluatePerformance();

    /** è®¡ç?è¶‹åŠ¿?œç? */
    float CalculateTrendSlope() const;

    /** è®¡ç?æ³¢åŠ¨??*/
    float CalculateVolatility() const;

    /** ç¡®å??»ä?è¯„çº§ */
    EPlayerPerformanceRating DetermineRating(float Score) const;

    /** è®¢é??¸å?äº‹ä»¶ */
    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

private:
    bool bIsInitialized;
    float EvaluationInterval;
    float TimeSinceLastEvaluation;
    int32 MaxDataPoints;

    TArray<FPlayerPerformanceDataPoint> DataPoints;
    FPlayerPerformanceStats CurrentStats;

    // å½“å?è¿½è¸ª?¶æ€?    int32 CurrentTotalUnits;
    int32 CurrentUnitsLost;
    float CurrentResourcesCollected;
    float CurrentExpectedResources;
    TMap<FString, float> ActiveMissions;

    // ?˜æ?ç»Ÿè®¡
    int32 SessionEnemiesKilled;
    int32 SessionAlliesLost;
    float SessionDamageDealt;
    float SessionDamageTaken;

    // ?†å²?€ä½??€å·?    float BestPerformanceScore;
    float WorstPerformanceScore;
};
