// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Dynamic History System Implementation

#include "MingRTSDynamicHistorySystem.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Paths.h"

UMingRTSDynamicHistorySystem::UMingRTSDynamicHistorySystem()
{
    SystemConfig = FHistorySystemConfig();
    CurrentBranchID = TEXT("MAIN");
}

void UMingRTSDynamicHistorySystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogTemp, Log, TEXT("Initializing Dynamic History System..."));
    
    // Initialize system state
    bSystemInitialized = false;
    HistoricalEvents.Empty();
    TimelineBranches.Empty();
    CausalityNetwork.Empty();
    EventCausalityMap.Empty();
    
    // Load saved history state if exists
    LoadHistoryState();
    
    // Create main timeline branch if not exists
    if (!TimelineBranches.Contains(CurrentBranchID))
    {
        FTimelineBranch MainBranch;
        MainBranch.BranchID = CurrentBranchID;
        MainBranch.BranchName = TEXT("Main Timeline");
        MainBranch.BranchType = ETimelineBranchType::Main;
        MainBranch.CreationTime = FDateTime::Now();
        MainBranch.bActive = true;
        TimelineBranches.Add(CurrentBranchID, MainBranch);
    }
    
    // Start periodic event generation
    if (SystemConfig.EventGenerationFrequency > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            EventGenerationTimer,
            this,
            &UMingRTSDynamicHistorySystem::GeneratePeriodicEvents,
            SystemConfig.EventGenerationFrequency,
            true
        );
    }
    
    // Start causality calculation
    if (SystemConfig.CausalityCalculationInterval > 0.0f)
    {
        GetWorld()->GetTimerManager().SetTimer(
            CausalityCalculationTimer,
            this,
            &UMingRTSDynamicHistorySystem::CalculateCausality,
            SystemConfig.CausalityCalculationInterval,
            true
        );
    }
    
    // Start narrative generation
    if (SystemConfig.bEnableAutoNarrative)
    {
        GetWorld()->GetTimerManager().SetTimer(
            NarrativeGenerationTimer,
            this,
            &UMingRTSDynamicHistorySystem::GenerateAutoNarrative,
            120.0f, // Every 2 minutes
            true
        );
    }
    
    bSystemInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Dynamic History System initialized successfully"));
}

void UMingRTSDynamicHistorySystem::Deinitialize()
{
    Super::Deinitialize();
    
    UE_LOG(LogTemp, Log, TEXT("Deinitializing Dynamic History System..."));
    
    // Clear all timers
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(EventGenerationTimer);
        GetWorld()->GetTimerManager().ClearTimer(CausalityCalculationTimer);
        GetWorld()->GetTimerManager().ClearTimer(NarrativeGenerationTimer);
    }
    
    // Save history state
    SaveHistoryState();
    
    // Clear data
    HistoricalEvents.Empty();
    TimelineBranches.Empty();
    CausalityNetwork.Empty();
    EventCausalityMap.Empty();
    
    bSystemInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("Dynamic History System deinitialized"));
}

void UMingRTSDynamicHistorySystem::InitializeDynamicHistorySystem()
{
    if (!bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Dynamic History System not initialized"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Dynamic History System ready for operation"));
}

void UMingRTSDynamicHistorySystem::ShutdownDynamicHistorySystem()
{
    Deinitialize();
}

FString UMingRTSDynamicHistorySystem::GenerateHistoricalEvent(const FHistoricalEventData& EventData)
{
    if (!bSystemInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot generate event: System not initialized"));
        return FString();
    }
    
    // Create event with unique ID
    FHistoricalEventData NewEvent = EventData;
    NewEvent.EventID = GenerateUniqueEventID();
    NewEvent.Timestamp = FDateTime::Now();
    
    // Validate event
    if (!IsValidEvent(NewEvent))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid event data provided"));
        return FString();
    }
    
    // Add to events collection
    HistoricalEvents.Add(NewEvent.EventID, NewEvent);
    
    // Add to current timeline branch
    if (TimelineBranches.Contains(CurrentBranchID))
    {
        TimelineBranches[CurrentBranchID].EventSequence.Add(NewEvent.EventID);
    }
    
    // Broadcast event generation
    OnHistoricalEventGenerated.Broadcast(NewEvent);
    
    UE_LOG(LogTemp, Log, TEXT("Generated historical event: %s"), *NewEvent.EventName);
    
    return NewEvent.EventID;
}

FHistoricalEventData UMingRTSDynamicHistorySystem::GetHistoricalEvent(const FString& EventID) const
{
    if (HistoricalEvents.Contains(EventID))
    {
        return HistoricalEvents[EventID];
    }
    
    return FHistoricalEventData();
}

TArray<FHistoricalEventData> UMingRTSDynamicHistorySystem::GetEventsInTimeRange(const FDateTime& StartTime, const FDateTime& EndTime) const
{
    TArray<FHistoricalEventData> EventsInRange;
    
    for (const auto& EventPair : HistoricalEvents)
    {
        const FHistoricalEventData& Event = EventPair.Value;
        if (Event.Timestamp >= StartTime && Event.Timestamp <= EndTime)
        {
            EventsInRange.Add(Event);
        }
    }
    
    return EventsInRange;
}

TArray<FHistoricalEventData> UMingRTSDynamicHistorySystem::GetEventsByType(EHistoricalEventType EventType) const
{
    TArray<FHistoricalEventData> EventsByType;
    
    for (const auto& EventPair : HistoricalEvents)
    {
        const FHistoricalEventData& Event = EventPair.Value;
        if (Event.EventType == EventType)
        {
            EventsByType.Add(Event);
        }
    }
    
    return EventsByType;
}

TArray<FHistoricalEventData> UMingRTSDynamicHistorySystem::GetEventsByImportance(EHistoricalImportance Importance) const
{
    TArray<FHistoricalEventData> EventsByImportance;
    
    for (const auto& EventPair : HistoricalEvents)
    {
        const FHistoricalEventData& Event = EventPair.Value;
        if (Event.Importance == Importance)
        {
            EventsByImportance.Add(Event);
        }
    }
    
    return EventsByImportance;
}

void UMingRTSDynamicHistorySystem::RecordPlayerDecision(const FString& EventID, const FString& Decision)
{
    if (HistoricalEvents.Contains(EventID))
    {
        FHistoricalEventData& Event = HistoricalEvents[EventID];
        Event.PlayerDecision = Decision;
        Event.bPlayerTriggered = true;
        
        // Check if this decision should create a new timeline branch
        CheckForBranchPoints();
        
        UE_LOG(LogTemp, Log, TEXT("Recorded player decision for event %s: %s"), *EventID, *Decision);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Event ID %s not found for player decision"), *EventID);
    }
}

FString UMingRTSDynamicHistorySystem::CreateTimelineBranch(const FString& ParentBranchID, const FString& BranchName, ETimelineBranchType BranchType)
{
    if (!TimelineBranches.Contains(ParentBranchID))
    {
        UE_LOG(LogTemp, Error, TEXT("Parent branch %s not found"), *ParentBranchID);
        return FString();
    }
    
    if (TimelineBranches.Num() >= SystemConfig.MaxActiveBranches)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum number of active branches reached"));
        return FString();
    }
    
    // Create new branch
    FTimelineBranch NewBranch;
    NewBranch.BranchID = GenerateUniqueBranchID();
    NewBranch.BranchName = BranchName;
    NewBranch.BranchType = BranchType;
    NewBranch.ParentBranchID = ParentBranchID;
    NewBranch.CreationTime = FDateTime::Now();
    NewBranch.bActive = true;
    
    // Copy event sequence from parent
    const FTimelineBranch& ParentBranch = TimelineBranches[ParentBranchID];
    NewBranch.EventSequence = ParentBranch.EventSequence;
    
    // Add to branches collection
    TimelineBranches.Add(NewBranch.BranchID, NewBranch);
    
    // Broadcast branch creation
    OnTimelineBranchCreated.Broadcast(NewBranch);
    
    UE_LOG(LogTemp, Log, TEXT("Created timeline branch: %s from parent %s"), *BranchName, *ParentBranchID);
    
    return NewBranch.BranchID;
}

FTimelineBranch UMingRTSDynamicHistorySystem::GetTimelineBranch(const FString& BranchID) const
{
    if (TimelineBranches.Contains(BranchID))
    {
        return TimelineBranches[BranchID];
    }
    
    return FTimelineBranch();
}

TArray<FTimelineBranch> UMingRTSDynamicHistorySystem::GetAllActiveBranches() const
{
    TArray<FTimelineBranch> ActiveBranches;
    
    for (const auto& BranchPair : TimelineBranches)
    {
        const FTimelineBranch& Branch = BranchPair.Value;
        if (Branch.bActive)
        {
            ActiveBranches.Add(Branch);
        }
    }
    
    return ActiveBranches;
}

void UMingRTSDynamicHistorySystem::MergeTimelineBranches(const FString& SourceBranchID, const FString& TargetBranchID)
{
    if (!TimelineBranches.Contains(SourceBranchID) || !TimelineBranches.Contains(TargetBranchID))
    {
        UE_LOG(LogTemp, Error, TEXT("Source or target branch not found"));
        return;
    }
    
    FTimelineBranch& SourceBranch = TimelineBranches[SourceBranchID];
    FTimelineBranch& TargetBranch = TimelineBranches[TargetBranchID];
    
    // Merge event sequences
    for (const FString& EventID : SourceBranch.EventSequence)
    {
        if (!TargetBranch.EventSequence.Contains(EventID))
        {
            TargetBranch.EventSequence.Add(EventID);
        }
    }
    
    // Deactivate source branch
    SourceBranch.bActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Merged timeline branch %s into %s"), *SourceBranchID, *TargetBranchID);
}

void UMingRTSDynamicHistorySystem::CalculateCausality()
{
    CalculateEventCausality();
    UpdateCausalityNetwork();
}

TArray<FCausalityRelationship> UMingRTSDynamicHistorySystem::GetCausalRelationships(const FString& EventID) const
{
    TArray<FCausalityRelationship> Relationships;
    
    for (const FCausalityRelationship& Relationship : CausalityNetwork)
    {
        if (Relationship.SourceEventID == EventID || Relationship.TargetEventID == EventID)
        {
            Relationships.Add(Relationship);
        }
    }
    
    return Relationships;
}

TArray<FHistoricalEventData> UMingRTSDynamicHistorySystem::GetCausedEvents(const FString& EventID) const
{
    TArray<FHistoricalEventData> CausedEvents;
    
    if (EventCausalityMap.Contains(EventID))
    {
        const TArray<FString>& CausedEventIDs = EventCausalityMap[EventID];
        for (const FString& CausedEventID : CausedEventIDs)
        {
            if (HistoricalEvents.Contains(CausedEventID))
            {
                CausedEvents.Add(HistoricalEvents[CausedEventID]);
            }
        }
    }
    
    return CausedEvents;
}

TArray<FHistoricalEventData> UMingRTSDynamicHistorySystem::GetCausingEvents(const FString& EventID) const
{
    TArray<FHistoricalEventData> CausingEvents;
    
    for (const auto& CausalityPair : EventCausalityMap)
    {
        const TArray<FString>& CausedEventIDs = CausalityPair.Value;
        if (CausedEventIDs.Contains(EventID))
        {
            const FString& CausingEventID = CausalityPair.Key;
            if (HistoricalEvents.Contains(CausingEventID))
            {
                CausingEvents.Add(HistoricalEvents[CausingEventID]);
            }
        }
    }
    
    return CausingEvents;
}

FString UMingRTSDynamicHistorySystem::GenerateHistoricalNarrative(const FString& BranchID, const FDateTime& StartTime, const FDateTime& EndTime)
{
    FString Narrative;
    
    if (!TimelineBranches.Contains(BranchID))
    {
        UE_LOG(LogTemp, Error, TEXT("Branch %s not found"), *BranchID);
        return Narrative;
    }
    
    const FTimelineBranch& Branch = TimelineBranches[BranchID];
    TArray<FHistoricalEventData> BranchEvents;
    
    // Collect events in time range for this branch
    for (const FString& EventID : Branch.EventSequence)
    {
        if (HistoricalEvents.Contains(EventID))
        {
            const FHistoricalEventData& Event = HistoricalEvents[EventID];
            if (Event.Timestamp >= StartTime && Event.Timestamp <= EndTime)
            {
                BranchEvents.Add(Event);
            }
        }
    }
    
    // Sort events by timestamp
    BranchEvents.Sort([](const FHistoricalEventData& A, const FHistoricalEventData& B)
    {
        return A.Timestamp < B.Timestamp;
    });
    
    // Generate narrative
    Narrative = ConnectEventNarratives(BranchEvents);
    
    // Broadcast narrative generation
    OnNarrativeGenerated.Broadcast(Narrative);
    
    return Narrative;
}

FString UMingRTSDynamicHistorySystem::GenerateEventSummary(const FHistoricalEventData& Event) const
{
    FString Summary = FString::Printf(
        TEXT("[%s] %s - %s\n"),
        *Event.Timestamp.ToString(),
        *Event.EventName,
        *Event.Description
    );
    
    if (Event.bPlayerTriggered)
    {
        Summary += FString::Printf(TEXT("Player Decision: %s\n"), *Event.PlayerDecision);
    }
    
    if (Event.ImpactValues.Num() > 0)
    {
        Summary += TEXT("Impacts:\n");
        for (const auto& ImpactPair : Event.ImpactValues)
        {
            Summary += FString::Printf(TEXT("  %s: %.2f\n"), *ImpactPair.Key, ImpactPair.Value);
        }
    }
    
    return Summary;
}

FString UMingRTSDynamicHistorySystem::GenerateTimelineSummary(const FString& BranchID) const
{
    FString Summary;
    
    if (!TimelineBranches.Contains(BranchID))
    {
        UE_LOG(LogTemp, Error, TEXT("Branch %s not found"), *BranchID);
        return Summary;
    }
    
    const FTimelineBranch& Branch = TimelineBranches[BranchID];
    Summary += FString::Printf(TEXT("Timeline: %s\n"), *Branch.BranchName);
    Summary += FString::Printf(TEXT("Branch Type: %d\n"), (int32)Branch.BranchType);
    Summary += FString::Printf(TEXT("Total Events: %d\n"), Branch.EventSequence.Num());
    Summary += FString::Printf(TEXT("Created: %s\n"), *Branch.CreationTime.ToString());
    
    if (Branch.EventSequence.Num() > 0)
    {
        Summary += TEXT("\nRecent Events:\n");
        int32 DisplayCount = FMath::Min(5, Branch.EventSequence.Num());
        for (int32 i = Branch.EventSequence.Num() - DisplayCount; i < Branch.EventSequence.Num(); i++)
        {
            const FString& EventID = Branch.EventSequence[i];
            if (HistoricalEvents.Contains(EventID))
            {
                const FHistoricalEventData& Event = HistoricalEvents[EventID];
                Summary += FString::Printf(TEXT("  %s: %s\n"), *Event.Timestamp.ToString(), *Event.EventName);
            }
        }
    }
    
    return Summary;
}

void UMingRTSDynamicHistorySystem::SetHistorySystemConfig(const FHistorySystemConfig& Config)
{
    SystemConfig = Config;
    
    // Restart timers with new configuration
    if (GetWorld() && bSystemInitialized)
    {
        GetWorld()->GetTimerManager().ClearTimer(EventGenerationTimer);
        GetWorld()->GetTimerManager().ClearTimer(CausalityCalculationTimer);
        GetWorld()->GetTimerManager().ClearTimer(NarrativeGenerationTimer);
        
        if (SystemConfig.EventGenerationFrequency > 0.0f)
        {
            GetWorld()->GetTimerManager().SetTimer(
                EventGenerationTimer,
                this,
                &UMingRTSDynamicHistorySystem::GeneratePeriodicEvents,
                SystemConfig.EventGenerationFrequency,
                true
            );
        }
        
        if (SystemConfig.CausalityCalculationInterval > 0.0f)
        {
            GetWorld()->GetTimerManager().SetTimer(
                CausalityCalculationTimer,
                this,
                &UMingRTSDynamicHistorySystem::CalculateCausality,
                SystemConfig.CausalityCalculationInterval,
                true
            );
        }
        
        if (SystemConfig.bEnableAutoNarrative)
        {
            GetWorld()->GetTimerManager().SetTimer(
                NarrativeGenerationTimer,
                this,
                &UMingRTSDynamicHistorySystem::GenerateAutoNarrative,
                120.0f,
                true
            );
        }
    }
}

int32 UMingRTSDynamicHistorySystem::GetTotalEventCount() const
{
    return HistoricalEvents.Num();
}

int32 UMingRTSDynamicHistorySystem::GetBranchCount() const
{
    return TimelineBranches.Num();
}

TMap<EHistoricalEventType, int32> UMingRTSDynamicHistorySystem::GetEventTypeDistribution() const
{
    TMap<EHistoricalEventType, int32> Distribution;
    
    for (const auto& EventPair : HistoricalEvents)
    {
        const FHistoricalEventData& Event = EventPair.Value;
        int32 Count = Distribution.Contains(Event.EventType) ? Distribution[Event.EventType] : 0;
        Distribution.Add(Event.EventType, Count + 1);
    }
    
    return Distribution;
}

float UMingRTSDynamicHistorySystem::GetTimelineDivergence() const
{
    if (TimelineBranches.Num() <= 1)
    {
        return 0.0f;
    }
    
    // Calculate divergence based on branch differences
    float TotalDivergence = 0.0f;
    int32 ComparisonCount = 0;
    
    TArray<FString> BranchIDs;
    TimelineBranches.GetKeys(BranchIDs);
    
    for (int32 i = 0; i < BranchIDs.Num(); i++)
    {
        for (int32 j = i + 1; j < BranchIDs.Num(); j++)
        {
            const FTimelineBranch& BranchA = TimelineBranches[BranchIDs[i]];
            const FTimelineBranch& BranchB = TimelineBranches[BranchIDs[j]];
            
            // Calculate event sequence difference
            int32 CommonEvents = 0;
            for (const FString& EventID : BranchA.EventSequence)
            {
                if (BranchB.EventSequence.Contains(EventID))
                {
                    CommonEvents++;
                }
            }
            
            int32 MaxEvents = FMath::Max(BranchA.EventSequence.Num(), BranchB.EventSequence.Num());
            if (MaxEvents > 0)
            {
                float Divergence = 1.0f - (float)CommonEvents / (float)MaxEvents;
                TotalDivergence += Divergence;
                ComparisonCount++;
            }
        }
    }
    
    return ComparisonCount > 0 ? TotalDivergence / (float)ComparisonCount : 0.0f;
}

// Private implementation methods

void UMingRTSDynamicHistorySystem::GeneratePeriodicEvents()
{
    if (!bSystemInitialized)
    {
        return;
    }
    
    // Generate contextual events based on current game state
    GenerateContextualEvents();
    
    // Generate player-influenced events if enabled
    if (SystemConfig.bEnablePlayerInfluence)
    {
        GeneratePlayerInfluencedEvents();
    }
}

void UMingRTSDynamicHistorySystem::GeneratePlayerInfluencedEvents()
{
    // This would integrate with the AI task generation system
    // to create events based on player behavior patterns
    
    FHistoricalEventData PlayerEvent;
    PlayerEvent.EventType = EHistoricalEventType::Personal;
    PlayerEvent.Importance = EHistoricalImportance::Moderate;
    PlayerEvent.bPlayerTriggered = true;
    
    // Generate event based on recent player actions
    // This is a simplified implementation
    PlayerEvent.EventName = TEXT("Player Activity Detected");
    PlayerEvent.Description = TEXT("Player actions are influencing the course of history");
    
    GenerateHistoricalEvent(PlayerEvent);
}

void UMingRTSDynamicHistorySystem::GenerateContextualEvents()
{
    // Generate events based on current world state
    // This would integrate with game systems to create relevant events
    
    FHistoricalEventData ContextualEvent;
    ContextualEvent.EventType = EHistoricalEventType::Political;
    ContextualEvent.Importance = EHistoricalImportance::Moderate;
    ContextualEvent.EventName = TEXT("Political Development");
    ContextualEvent.Description = TEXT("The political landscape is evolving");
    
    GenerateHistoricalEvent(ContextualEvent);
}

void UMingRTSDynamicHistorySystem::CalculateEventCausality()
{
    // Clear existing causality map
    EventCausalityMap.Empty();
    
    // Calculate causality between events
    TArray<FString> EventIDs;
    HistoricalEvents.GetKeys(EventIDs);
    
    for (int32 i = 0; i < EventIDs.Num(); i++)
    {
        for (int32 j = i + 1; j < EventIDs.Num(); j++)
        {
            const FString& SourceEventID = EventIDs[i];
            const FString& TargetEventID = EventIDs[j];
            
            if (HistoricalEvents.Contains(SourceEventID) && HistoricalEvents.Contains(TargetEventID))
            {
                const FHistoricalEventData& SourceEvent = HistoricalEvents[SourceEventID];
                const FHistoricalEventData& TargetEvent = HistoricalEvents[TargetEventID];
                
                // Calculate causal strength
                float Strength = CalculateCausalStrength(SourceEvent, TargetEvent);
                
                if (Strength > 0.3f) // Threshold for causal relationship
                {
                    FCausalityRelationship Relationship;
                    Relationship.SourceEventID = SourceEventID;
                    Relationship.TargetEventID = TargetEventID;
                    Relationship.CausalityType = ECausalityType::Direct;
                    Relationship.Strength = Strength;
                    Relationship.Probability = FMath::Clamp(Strength, 0.0f, 1.0f);
                    Relationship.Description = FString::Printf(TEXT("Event %s influenced event %s"), *SourceEvent.EventName, *TargetEvent.EventName);
                    
                    CausalityNetwork.Add(Relationship);
                    
                    // Update causality map
                    if (!EventCausalityMap.Contains(SourceEventID))
                    {
                        EventCausalityMap.Add(SourceEventID, TArray<FString>());
                    }
                    EventCausalityMap[SourceEventID].Add(TargetEventID);
                    
                    // Broadcast causality calculation
                    OnCausalityCalculated.Broadcast(Relationship, Strength);
                }
            }
        }
    }
}

float UMingRTSDynamicHistorySystem::CalculateCausalStrength(const FHistoricalEventData& SourceEvent, const FHistoricalEventData& TargetEvent) const
{
    float Strength = 0.0f;
    
    // Time proximity factor
    FTimespan TimeDifference = TargetEvent.Timestamp - SourceEvent.Timestamp;
    float TimeFactor = FMath::Clamp(1.0f - (TimeDifference.GetTotalDays() / 30.0f), 0.0f, 1.0f);
    
    // Location proximity factor
    float LocationDistance = FVector2D::Distance(SourceEvent.Location, TargetEvent.Location);
    float LocationFactor = FMath::Clamp(1.0f - (LocationDistance / 1000.0f), 0.0f, 1.0f);
    
    // Event type compatibility
    float TypeFactor = 0.5f; // Default
    if (SourceEvent.EventType == EHistoricalEventType::Political && TargetEvent.EventType == EHistoricalEventType::Social)
    {
        TypeFactor = 0.8f;
    }
    else if (SourceEvent.EventType == EHistoricalEventType::Military && TargetEvent.EventType == EHistoricalEventType::Political)
    {
        TypeFactor = 0.9f;
    }
    else if (SourceEvent.EventType == EHistoricalEventType::Economic && TargetEvent.EventType == EHistoricalEventType::Social)
    {
        TypeFactor = 0.7f;
    }
    
    // Player influence factor
    float PlayerFactor = 1.0f;
    if (SourceEvent.bPlayerTriggered)
    {
        PlayerFactor = SystemConfig.PlayerInfluenceWeight;
    }
    
    // Combine factors
    Strength = (TimeFactor * 0.4f) + (LocationFactor * 0.3f) + (TypeFactor * 0.2f) + (PlayerFactor * 0.1f);
    
    return Strength;
}

void UMingRTSDynamicHistorySystem::UpdateCausalityNetwork()
{
    // This method would update the causality network based on new events
    // and changing conditions. For now, it's a placeholder.
}

void UMingRTSDynamicHistorySystem::GenerateAutoNarrative()
{
    if (!TimelineBranches.Contains(CurrentBranchID))
    {
        return;
    }
    
    // Generate narrative for the current timeline branch
    FDateTime Now = FDateTime::Now();
    FDateTime StartTime = Now - FTimespan::FromDays(7); // Last 7 days
    
    FString Narrative = GenerateHistoricalNarrative(CurrentBranchID, StartTime, Now);
    
    UE_LOG(LogTemp, Log, TEXT("Generated auto-narrative: %s"), *Narrative);
}

FString UMingRTSDynamicHistorySystem::FormatEventNarrative(const FHistoricalEventData& Event) const
{
    FString Narrative = FString::Printf(
        TEXT("在%s，%s發生了。%s"),
        *Event.Timestamp.ToString(),
        *Event.EventName,
        *Event.Description
    );
    
    if (Event.bPlayerTriggered)
    {
        Narrative += FString::Printf(TEXT("玩家的決定是：%s。"), *Event.PlayerDecision);
    }
    
    return Narrative;
}

FString UMingRTSDynamicHistorySystem::ConnectEventNarratives(const TArray<FHistoricalEventData>& Events) const
{
    FString ConnectedNarrative;
    
    for (int32 i = 0; i < Events.Num(); i++)
    {
        ConnectedNarrative += FormatEventNarrative(Events[i]);
        
        if (i < Events.Num() - 1)
        {
            ConnectedNarrative += TEXT("隨後，");
        }
    }
    
    return ConnectedNarrative;
}

void UMingRTSDynamicHistorySystem::CheckForBranchPoints()
{
    // This method would check if current conditions warrant creating a new timeline branch
    // For now, it's a simplified implementation
    
    if (TimelineBranches.Num() < SystemConfig.MaxActiveBranches)
    {
        // Random chance to create a branch (simplified)
        if (FMath::RandRange(0, 100) < 5) // 5% chance
        {
            FString NewBranchID = CreateTimelineBranch(
                CurrentBranchID,
                TEXT("Alternate Timeline"),
                ETimelineBranchType::Alternate
            );
            
            if (!NewBranchID.IsEmpty())
            {
                UE_LOG(LogTemp, Log, TEXT("Automatically created new timeline branch: %s"), *NewBranchID);
            }
        }
    }
}

FString UMingRTSDynamicHistorySystem::GenerateUniqueEventID() const
{
    return FString::Printf(TEXT("EVENT_%lld"), FDateTime::Now().GetTicks());
}

FString UMingRTSDynamicHistorySystem::GenerateUniqueBranchID() const
{
    return FString::Printf(TEXT("BRANCH_%lld"), FDateTime::Now().GetTicks());
}

bool UMingRTSDynamicHistorySystem::IsValidEvent(const FHistoricalEventData& Event) const
{
    return !Event.EventName.IsEmpty() && !Event.Description.IsEmpty();
}

void UMingRTSDynamicHistorySystem::SaveHistoryState()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("HistoryState.json");
    
    // Create save data structure
    TSharedPtr<FJsonObject> SaveData = MakeShareable(new FJsonObject);
    
    // Save events
    TArray<TSharedPtr<FJsonValue>> EventArray;
    for (const auto& EventPair : HistoricalEvents)
    {
        TSharedPtr<FJsonObject> EventObj = MakeShareable(new FJsonObject);
        EventObj->SetStringField(TEXT("EventID"), EventPair.Key);
        EventObj->SetStringField(TEXT("EventName"), EventPair.Value.EventName);
        EventObj->SetStringField(TEXT("Description"), EventPair.Value.Description);
        EventObj->SetNumberField(TEXT("EventType"), (int32)EventPair.Value.EventType);
        EventObj->SetNumberField(TEXT("Importance"), (int32)EventPair.Value.Importance);
        EventObj->SetStringField(TEXT("Timestamp"), EventPair.Value.Timestamp.ToString());
        EventArray.Add(MakeShareable(new FJsonValueObject(EventObj)));
    }
    SaveData->SetArrayField(TEXT("Events"), EventArray);
    
    // Save branches
    TArray<TSharedPtr<FJsonValue>> BranchArray;
    for (const auto& BranchPair : TimelineBranches)
    {
        TSharedPtr<FJsonObject> BranchObj = MakeShareable(new FJsonObject);
        BranchObj->SetStringField(TEXT("BranchID"), BranchPair.Key);
        BranchObj->SetStringField(TEXT("BranchName"), BranchPair.Value.BranchName);
        BranchObj->SetNumberField(TEXT("BranchType"), (int32)BranchPair.Value.BranchType);
        BranchArray.Add(MakeShareable(new FJsonValueObject(BranchObj)));
    }
    SaveData->SetArrayField(TEXT("Branches"), BranchArray);
    
    // Write to file
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(SaveData.ToSharedRef(), Writer);
    
    if (!FFileHelper::SaveStringToFile(OutputString, *SavePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save history state to %s"), *SavePath);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("History state saved to %s"), *SavePath);
    }
}

void UMingRTSDynamicHistorySystem::LoadHistoryState()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("HistoryState.json");
    
    if (!FFileHelper::FileExists(*LoadPath))
    {
        UE_LOG(LogTemp, Log, TEXT("No saved history state found, starting fresh"));
        return;
    }
    
    FString LoadString;
    if (!FFileHelper::LoadFileToString(LoadString, *LoadPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load history state from %s"), *LoadPath);
        return;
    }
    
    TSharedPtr<FJsonObject> LoadData;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(LoadString);
    
    if (!FJsonSerializer::Deserialize(Reader, LoadData) || !LoadData.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse history state JSON"));
        return;
    }
    
    // Load events
    const TArray<TSharedPtr<FJsonValue>>* EventArray;
    if (LoadData->TryGetArrayField(TEXT("Events"), EventArray))
    {
        for (const TSharedPtr<FJsonValue>& EventValue : *EventArray)
        {
            TSharedPtr<FJsonObject> EventObj = EventValue->AsObject();
            if (EventObj.IsValid())
            {
                FHistoricalEventData Event;
                FString EventID;
                if (EventObj->TryGetStringField(TEXT("EventID"), EventID))
                {
                    Event.EventID = EventID;
                    EventObj->TryGetStringField(TEXT("EventName"), Event.EventName);
                    Event.Obj->TryGetStringField(TEXT("Description"), Event.Description);
                    
                    int32 EventTypeInt;
                    if (EventObj->TryGetNumberField(TEXT("EventType"), EventTypeInt))
                    {
                        Event.EventType = (EHistoricalEventType)EventTypeInt;
                    }
                    
                    int32 ImportanceInt;
                    if (EventObj->TryGetNumberField(TEXT("Importance"), ImportanceInt))
                    {
                        Event.Importance = (EHistoricalImportance)ImportanceInt;
                    }
                    
                    FString TimestampStr;
                    if (EventObj->TryGetStringField(TEXT("Timestamp"), TimestampStr))
                    {
                        Event.Timestamp = FDateTime::Parse(TimestampStr);
                    }
                    
                    HistoricalEvents.Add(EventID, Event);
                }
            }
        }
    }
    
    // Load branches
    const TArray<TSharedPtr<FJsonValue>>* BranchArray;
    if (LoadData->TryGetArrayField(TEXT("Branches"), BranchArray))
    {
        for (const TSharedPtr<FJsonValue>& BranchValue : *BranchArray)
        {
            TSharedPtr<FJsonObject> BranchObj = BranchValue->AsObject();
            if (BranchObj.IsValid())
            {
                FTimelineBranch Branch;
                FString BranchID;
                if (BranchObj->TryGetStringField(TEXT("BranchID"), BranchID))
                {
                    Branch.BranchID = BranchID;
                    BranchObj->TryGetStringField(TEXT("BranchName"), Branch.BranchName);
                    
                    int32 BranchTypeInt;
                    if (BranchObj->TryGetNumberField(TEXT("BranchType"), BranchTypeInt))
                    {
                        Branch.BranchType = (ETimelineBranchType)BranchTypeInt;
                    }
                    
                    TimelineBranches.Add(BranchID, Branch);
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("History state loaded successfully"));
}
