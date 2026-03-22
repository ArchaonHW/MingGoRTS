// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Timeline Manager - Manages multiple timeline branches and their evolution

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicHistorySystem.h"
#include "MingRTSTimelineManager.generated.h"

/**
 * Timeline Manager - Manages the creation, evolution, and convergence of timeline branches
 * 
 * This subsystem handles the complex logic of managing multiple parallel timelines,
 * their interactions, and potential convergence points.
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSTimelineManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSTimelineManager();

    // Timeline Management
    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void InitializeTimelineManager();

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    FString CreateNewTimeline(const FString& ParentID, const FString& TimelineName, ETimelineBranchType Type);

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void DeleteTimeline(const FString& TimelineID);

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void MergeTimelines(const FString& SourceID, const FString& TargetID);

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void DivergeTimeline(const FString& TimelineID, const FString& DivergencePoint);

    // Timeline Analysis
    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FTimelineBranch> GetAllTimelines() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FTimelineBranch> GetActiveTimelines() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FTimelineBranch> GetConvergentTimelines() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    float CalculateTimelineSimilarity(const FString& TimelineA, const FString& TimelineB) const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FString> FindConvergencePoints(const FString& TimelineA, const FString& TimelineB) const;

    // Timeline Navigation
    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void SwitchToTimeline(const FString& TimelineID);

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    FString GetCurrentTimeline() const { return CurrentTimelineID; }

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FHistoricalEventData> GetTimelineEvents(const FString& TimelineID) const;

    // Timeline Evolution
    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void EvolveTimelines(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void CheckForConvergenceOpportunities();

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    void ProcessTimelineEvents();

    // Timeline Analytics
    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    int32 GetTimelineCount() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    float GetAverageTimelineLength() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TMap<ETimelineBranchType, int32> GetTimelineTypeDistribution() const;

    UFUNCTION(BlueprintCallable, Category = "Timeline Manager")
    TArray<FString> GetMostDivergentTimelines() const;

protected:
    // Internal timeline management
    void UpdateTimelineStates();
    void ProcessTimelineConvergence();
    void CleanupInactiveTimelines();

    // Timeline similarity calculation
    float CalculateEventSequenceSimilarity(const TArray<FString>& SequenceA, const TArray<FString>& SequenceB) const;
    float CalculateTemporalSimilarity(const FTimelineBranch& TimelineA, const FTimelineBranch& TimelineB) const;
    float CalculateCausalSimilarity(const FTimelineBranch& TimelineA, const FTimelineBranch& TimelineB) const;

    // Convergence detection
    bool ShouldConverge(const FString& TimelineA, const FString& TimelineB) const;
    FString GenerateConvergencePoint(const FString& TimelineA, const FString& TimelineB) const;

private:
    UPROPERTY()
    TMap<FString, FTimelineBranch> Timelines;

    UPROPERTY()
    FString CurrentTimelineID;

    UPROPERTY()
    TArray<FString> ConvergenceQueue;

    UPROPERTY()
    float LastEvolutionTime;

    UPROPERTY()
    bool bManagerInitialized;

    // Helper methods
    FString GenerateUniqueTimelineID() const;
    void ValidateTimelineIntegrity();
    void OptimizeTimelineStorage();
};
