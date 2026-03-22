// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Timeline Manager Implementation

#include "MingRTSTimelineManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingRTSTimelineManager::UMingRTSTimelineManager()
{
    bManagerInitialized = false;
    LastEvolutionTime = 0.0f;
}

void UMingRTSTimelineManager::InitializeTimelineManager()
{
    bManagerInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Timeline Manager initialized"));
}

FString UMingRTSTimelineManager::CreateNewTimeline(const FString& ParentID, const FString& TimelineName, ETimelineBranchType Type)
{
    FString NewTimelineID = GenerateUniqueTimelineID();
    
    FTimelineBranch NewTimeline;
    NewTimeline.BranchID = NewTimelineID;
    NewTimeline.BranchName = TimelineName;
    NewTimeline.BranchType = Type;
    NewTimeline.ParentBranchID = ParentID;
    NewTimeline.CreationTime = FDateTime::Now();
    NewTimeline.bActive = true;
    
    Timelines.Add(NewTimelineID, NewTimeline);
    
    return NewTimelineID;
}

void UMingRTSTimelineManager::SwitchToTimeline(const FString& TimelineID)
{
    if (Timelines.Contains(TimelineID))
    {
        CurrentTimelineID = TimelineID;
        UE_LOG(LogTemp, Log, TEXT("Switched to timeline: %s"), *TimelineID);
    }
}

float UMingRTSTimelineManager::CalculateTimelineSimilarity(const FString& TimelineA, const FString& TimelineB) const
{
    if (!Timelines.Contains(TimelineA) || !Timelines.Contains(TimelineB))
    {
        return 0.0f;
    }
    
    const FTimelineBranch& BranchA = Timelines[TimelineA];
    const FTimelineBranch& BranchB = Timelines[TimelineB];
    
    float EventSimilarity = CalculateEventSequenceSimilarity(BranchA.EventSequence, BranchB.EventSequence);
    float TemporalSimilarity = CalculateTemporalSimilarity(BranchA, BranchB);
    float CausalSimilarity = CalculateCausalSimilarity(BranchA, BranchB);
    
    return (EventSimilarity * 0.4f) + (TemporalSimilarity * 0.3f) + (CausalSimilarity * 0.3f);
}

FString UMingRTSTimelineManager::GenerateUniqueTimelineID() const
{
    return FString::Printf(TEXT("TL_%lld"), FDateTime::Now().GetTicks());
}
