#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDynamicTaskGenerator.h"
#include "MingPlayerBehaviorPredictor.h"
#include "MingAITaskIntegration.generated.h"

USTRUCT(BlueprintType)
struct FTaskGenerationRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bForcePersonalization;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Priority;
};

USTRUCT(BlueprintType)
struct FBehaviorAnalysisRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AnalysisWindowMinutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIncludePredictions;
};

USTRUCT(BlueprintType)
struct FTaskBehaviorInsight
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FDynamicTask> RecommendedTasks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPredictionResult> BehaviorPredictions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FContentPreloadRequest> PreloadRequests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OverallEngagementScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString InsightSummary;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTaskGenerated, const FDynamicTask&, GeneratedTask);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBehaviorPredicted, const FPredictionResult&, Prediction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInsightGenerated, const FTaskBehaviorInsight&, Insight);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingAITaskIntegration : public UObject
{
    GENERATED_BODY()

public:
    UMingAITaskIntegration();

    // Initialize AI task integration system
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    void InitializeAITaskIntegration();

    // Process task generation request
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    FDynamicTask ProcessTaskGenerationRequest(const FTaskGenerationRequest& Request);

    // Process behavior analysis request
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    TArray<FPredictionResult> ProcessBehaviorAnalysisRequest(const FBehaviorAnalysisRequest& Request);

    // Get comprehensive player insight
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    FTaskBehaviorInsight GetPlayerInsight(const FString& PlayerID);

    // Record player action for behavior tracking
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    void RecordPlayerAction(const FString& PlayerID, const FPlayerAction& Action);

    // Get adaptive task recommendations
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    TArray<FDynamicTask> GetAdaptiveTaskRecommendations(const FString& PlayerID, int32 MaxTasks = 3);

    // Update player behavior profile
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    void UpdatePlayerBehaviorProfile(const FString& PlayerID, const FPlayerBehaviorProfile& Profile);

    // Get content preload recommendations
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    TArray<FContentPreloadRequest> GetContentPreloadRecommendations(const FString& PlayerID);

    // Analyze task completion impact
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    void AnalyzeTaskCompletionImpact(const FString& PlayerID, const FDynamicTask& CompletedTask, float CompletionTime);

    // Get real-time behavior predictions
    UFUNCTION(BlueprintCallable, Category = "AI Task Integration")
    TArray<FPredictionResult> GetRealTimePredictions(const FString& PlayerID, float TimeWindowMinutes = 10.0f);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "AI Task Integration")
    FOnTaskGenerated OnTaskGenerated;

    UPROPERTY(BlueprintAssignable, Category = "AI Task Integration")
    FOnBehaviorPredicted OnBehaviorPredicted;

    UPROPERTY(BlueprintAssignable, Category = "AI Task Integration")
    FOnInsightGenerated OnInsightGenerated;

protected:
    // Core AI systems
    UPROPERTY(Category = "AI Task Integration")
    TObjectPtr<UMingDynamicTaskGenerator> TaskGenerator;

    UPROPERTY(Category = "AI Task Integration")
    TObjectPtr<UMingPlayerBehaviorPredictor> BehaviorPredictor;

    // Configuration parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Task Integration")
    float TaskPersonalizationThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Task Integration")
    float PredictionConfidenceThreshold;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Task Integration")
    int32 MaxInsightHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Task Integration")
    bool bEnableRealTimePrediction;

private:
    // Internal helper functions
    FPlayerBehaviorProfile CreateDefaultProfile(const FString& PlayerID);
    float CalculateEngagementScore(const FString& PlayerID);
    FString GenerateInsightSummary(const FTaskBehaviorInsight& Insight);
    void ProcessRealTimeUpdates(const FString& PlayerID);
    void CacheInsight(const FString& PlayerID, const FTaskBehaviorInsight& Insight);
    FTaskBehaviorInsight GetCachedInsight(const FString& PlayerID);

    // Cache system
    UPROPERTY(Category = "AI Task Integration")
    TMap<FString, FTaskBehaviorInsight> InsightCache;

    UPROPERTY(Category = "AI Task Integration")
    TMap<FString, FDateTime> LastInsightUpdate;
};

