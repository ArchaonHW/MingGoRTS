#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/Widget.h"
#include "MingAIUIManager.h"
#include "MingBehaviorAnalytics.generated.h"

USTRUCT(BlueprintType)
struct FBehaviorEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Event")
    FString EventType;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Event")
    FString Context;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Event")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Event")
    float Value;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Event")
    TMap<FString, FString> Metadata;

    FBehaviorEvent()
    {
        EventType = TEXT("");
        Context = TEXT("");
        Timestamp = 0.0f;
        Value = 0.0f;
    }

    FBehaviorEvent(const FString& InEventType, const FString& InContext, float InValue = 1.0f)
        : EventType(InEventType), Context(InContext), Value(InValue)
    {
        Timestamp = FDateTime::Now().ToUnixTimestamp();
    }
};

USTRUCT(BlueprintType)
struct FBehaviorPattern
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Pattern")
    FString PatternName;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Pattern")
    float Frequency;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Pattern")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Pattern")
    TArray<FString> Sequence;

    UPROPERTY(BlueprintReadOnly, Category = "Behavior Pattern")
    float AverageInterval;

    FBehaviorPattern()
    {
        PatternName = TEXT("");
        Frequency = 0.0f;
        Confidence = 0.0f;
        AverageInterval = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FUserSession
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    FString SessionID;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    float StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    float EndTime;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    float Duration;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    int32 ActionCount;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    TArray<FBehaviorEvent> Events;

    UPROPERTY(BlueprintReadOnly, Category = "User Session")
    TMap<FString, int32> EventTypeCounts;

    FUserSession()
    {
        SessionID = TEXT("");
        StartTime = 0.0f;
        EndTime = 0.0f;
        Duration = 0.0f;
        ActionCount = 0;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorPatternDetected, const FBehaviorPattern&, Pattern);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionCompleted, const FUserSession&, Session);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAnomalyDetected, const FString&, AnomalyType, float, Severity);

/**
 * 行為分析系統
 * 深度分析用戶行為模式和趨勢
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingBehaviorAnalytics : public UObject
{
    GENERATED_BODY()

public:
    UMingBehaviorAnalytics();

    // 初始化分析系統
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void InitializeAnalytics();

    // 事件記錄
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void RecordEvent(const FString& EventType, const FString& Context, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void RecordEventWithMetadata(const FString& EventType, const FString& Context, 
                                const TMap<FString, FString>& Metadata, float Value = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void StartSession(const FString& SessionID);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void EndSession();

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void RecordUIInteraction(const FString& WidgetType, const FString& Action, const FString& Target);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void RecordGameAction(const FString& ActionType, const FString& Target, float SuccessRate);

    // 模式檢測
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void DetectPatterns();

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<FBehaviorPattern> GetDetectedPatterns() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    FBehaviorPattern GetMostFrequentPattern() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    bool HasPattern(const FString& PatternName) const;

    // 趨勢分析
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<float> GetEventTrend(const FString& EventType, int32 WindowSize = 10) const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    float GetEventFrequency(const FString& EventType) const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    float GetAverageActionInterval(const FString& EventType) const;

    // 異常檢測
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void DetectAnomalies();

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<FString> GetAnomalies() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    float GetAnomalySeverity(const FString& AnomalyType) const;

    // 預測分析
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    FString PredictNextAction();

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    float PredictSessionDuration() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<FString> PredictUserGoals() const;

    // 熱力圖數據
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TMap<FString, float> GenerateHeatmapData(const FString& DataType) const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<FVector2D> GetClickHeatmap() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TMap<EPersonalUIType, float> GetPanelUsageHeatmap() const;

    // 用戶分群
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    FString GetUserSegment() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    TArray<FString> GetSimilarUsers() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    float GetUserEngagementScore() const;

    // 報告生成
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    FString GenerateBehaviorReport() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    FString GenerateSessionSummary() const;

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void ExportAnalyticsData(const FString& FilePath) const;

    // 狀態查詢
    UFUNCTION(BlueprintPure, Category = "Behavior Analytics")
    bool IsRecording() const { return bIsRecording; }

    UFUNCTION(BlueprintPure, Category = "Behavior Analytics")
    int32 GetTotalEventCount() const { return AllEvents.Num(); }

    UFUNCTION(BlueprintPure, Category = "Behavior Analytics")
    FUserSession GetCurrentSession() const { return CurrentSession; }

    UFUNCTION(BlueprintPure, Category = "Behavior Analytics")
    TArray<FUserSession> GetSessionHistory() const { return SessionHistory; }

    // 設置
    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void SetRecordingEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void SetPatternDetectionThreshold(float Threshold);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void SetAnomalyDetectionSensitivity(float Sensitivity);

    UFUNCTION(BlueprintCallable, Category = "Behavior Analytics")
    void ClearAllData();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Analytics Events")
    FOnBehaviorPatternDetected OnBehaviorPatternDetected;

    UPROPERTY(BlueprintAssignable, Category = "Analytics Events")
    FOnSessionCompleted OnSessionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Analytics Events")
    FOnAnomalyDetected OnAnomalyDetected;

protected:
    // 數據存儲
    UPROPERTY()
    TArray<FBehaviorEvent> AllEvents;

    UPROPERTY()
    TArray<FUserSession> SessionHistory;

    UPROPERTY()
    TArray<FBehaviorPattern> DetectedPatterns;

    UPROPERTY()
    TMap<FString, float> EventFrequencies;

    // 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<float>> EventTrends;

    UPROPERTY()
    TMap<FString, float> AnomalyScores;

    // 當前會話
    UPROPERTY()
    FUserSession CurrentSession;

    // 配置
    UPROPERTY()
    bool bIsRecording = true;

    UPROPERTY()
    float PatternDetectionThreshold = 0.7f;

    UPROPERTY()
    float AnomalyDetectionSensitivity = 0.8f;

    UPROPERTY()
    int32 MaxEventsInMemory = 10000;

    UPROPERTY()
    int32 MaxSessionsInHistory = 100;

    // 內部函數
    void ProcessEvent(const FBehaviorEvent& Event);
    void UpdateEventFrequencies();
    void UpdateEventTrends();
    void DetectSequentialPatterns();
    void DetectTemporalPatterns();
    void DetectBehavioralAnomalies();
    float CalculatePatternConfidence(const TArray<FString>& Sequence) const;
    float CalculateAnomalyScore(const FString& EventType, float Value) const;
    void GenerateUserSegments();
    FString ClassifyUserBehavior() const;

    // 機器學習輔助
    TArray<float> ExtractFeatures(const TArray<FBehaviorEvent>& Events) const;
    float CalculateSimilarity(const TArray<float>& Features1, const TArray<float>& Features2) const;
    void UpdatePredictionModels();

private:
    // 輔助函數
    void CleanupOldData();
    void SaveAnalyticsData();
    void LoadAnalyticsData();
    FString GenerateSessionID() const;
};
