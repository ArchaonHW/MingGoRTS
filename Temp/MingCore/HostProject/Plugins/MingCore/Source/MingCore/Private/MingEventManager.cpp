#include "MingEventManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"

UMingEventManager::UMingEventManager()
{
    bBatchingEnabled = true;
    BatchInterval = 0.1f;
    LastBatchProcessTime = 0.0f;
    MaxHistorySize = 1000;
}

UMingEventManager* UMingEventManager::Get(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // Use a static map to store managers per world
    static TMap<TWeakObjectPtr<UWorld>, UMingEventManager*> WorldManagers;

    TWeakObjectPtr<UWorld> WorldKey = World;
    UMingEventManager** ExistingManager = WorldManagers.Find(WorldKey);

    if (ExistingManager && *ExistingManager)
    {
        return *ExistingManager;
    }

    // Create new manager
    UMingEventManager* NewManager = NewObject<UMingEventManager>(World);
    NewManager->CachedWorld = World;
    WorldManagers.Add(WorldKey, NewManager);

    return NewManager;
}

void UMingEventManager::PublishEvent(const FMingGameEvent& Event)
{
    FMingGameEvent EventToPublish = Event;

    // Set timestamp if not set
    if (EventToPublish.Timestamp <= 0.0f && CachedWorld.IsValid())
    {
        EventToPublish.Timestamp = CachedWorld->GetTimeSeconds();
    }

    // Generate unique ID if not set
    if (EventToPublish.EventID.IsEmpty())
    {
        EventToPublish.EventID = FGuid::NewGuid().ToString();
    }

    if (bBatchingEnabled)
    {
        PendingEvents.Add(EventToPublish);

        // Check if we should process the batch
        float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;
        if (CurrentTime - LastBatchProcessTime >= BatchInterval)
        {
            ProcessBatch();
        }
    }
    else
    {
        DispatchEvent(EventToPublish);
        AddToHistory(EventToPublish);
    }
}

void UMingEventManager::PublishSimpleEvent(EMingGameEventType Type, UObject* Source, UObject* Target)
{
    FMingGameEvent Event;
    Event.EventType = Type;
    Event.SourceObject = Source;
    Event.TargetObject = Target;

    if (Source)
    {
        Event.Location = Source->GetWorld() && Source->IsA<AActor>() ? 
                         Cast<AActor>(Source)->GetActorLocation() : FVector::ZeroVector;
    }

    PublishEvent(Event);
}

void UMingEventManager::PublishCombatEvent(EMingGameEventType Type, AActor* Attacker, AActor* Target, int32 DamageDealt)
{
    FMingGameEvent Event;
    Event.EventType = Type;
    Event.SourceObject = Attacker;
    Event.TargetObject = Target;
    
    if (Attacker)
    {
        Event.Location = Attacker->GetActorLocation();
    }
    else if (Target)
    {
        Event.Location = Target->GetActorLocation();
    }

    Event.IntData.Add(TEXT("Damage"), DamageDealt);
    
    PublishEvent(Event);
}

void UMingEventManager::PublishResourceEvent(EMingGameEventType Type, EMingResourceType Resource, int32 Amount, UObject* Source)
{
    FMingGameEvent Event;
    Event.EventType = Type;
    Event.SourceObject = Source;

    Event.IntData.Add(TEXT("ResourceType"), (int32)Resource);
    Event.IntData.Add(TEXT("Amount"), Amount);

    PublishEvent(Event);
}

void UMingEventManager::SubscribeToEvent(EMingGameEventType EventType, const FOnGameEvent& Callback)
{
    if (!EventSubscribers.Contains(EventType))
    {
        EventSubscribers.Add(EventType, TArray<FOnGameEvent>());
    }

    EventSubscribers[EventType].Add(Callback);
}

void UMingEventManager::UnsubscribeFromEvent(EMingGameEventType EventType, const FOnGameEvent& Callback)
{
    if (EventSubscribers.Contains(EventType))
    {
        EventSubscribers[EventType].Remove(Callback);
    }
}

void UMingEventManager::SubscribeToAllEvents(const FOnGameEvent& Callback)
{
    GlobalSubscribers.Add(Callback);
}

void UMingEventManager::ClearAllSubscriptions()
{
    EventSubscribers.Empty();
    GlobalSubscribers.Empty();
}

TArray<FMingGameEvent> UMingEventManager::GetEventHistory(EMingGameEventType EventType, float TimeWindow)
{
    TArray<FMingGameEvent> FilteredEvents;
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    for (const FMingGameEvent& Event : EventHistory)
    {
        if (Event.EventType == EventType)
        {
            if (TimeWindow < 0.0f || (CurrentTime - Event.Timestamp) <= TimeWindow)
            {
                FilteredEvents.Add(Event);
            }
        }
    }

    return FilteredEvents;
}

TArray<FMingGameEvent> UMingEventManager::GetAllEventHistory(float TimeWindow)
{
    if (TimeWindow < 0.0f)
    {
        return EventHistory;
    }

    TArray<FMingGameEvent> FilteredEvents;
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    for (const FMingGameEvent& Event : EventHistory)
    {
        if ((CurrentTime - Event.Timestamp) <= TimeWindow)
        {
            FilteredEvents.Add(Event);
        }
    }

    return FilteredEvents;
}

void UMingEventManager::ClearHistory()
{
    EventHistory.Empty();
}

int32 UMingEventManager::GetEventCount(EMingGameEventType EventType, float TimeWindow)
{
    return GetEventHistory(EventType, TimeWindow).Num();
}

bool UMingEventManager::HasEventOccurred(EMingGameEventType EventType, float TimeWindow)
{
    return GetEventCount(EventType, TimeWindow) > 0;
}

FMingGameEvent UMingEventManager::GetLastEventOfType(EMingGameEventType EventType)
{
    TArray<FMingGameEvent> Events = GetEventHistory(EventType);
    
    if (Events.Num() > 0)
    {
        // Return the most recent event
        return Events.Last();
    }

    return FMingGameEvent();
}

TArray<FMingGameEvent> UMingEventManager::GetEventsForObject(UObject* Object, float TimeWindow)
{
    TArray<FMingGameEvent> FilteredEvents;
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    for (const FMingGameEvent& Event : EventHistory)
    {
        bool bMatches = (Event.SourceObject == Object) || (Event.TargetObject == Object);
        
        if (bMatches)
        {
            if (TimeWindow < 0.0f || (CurrentTime - Event.Timestamp) <= TimeWindow)
            {
                FilteredEvents.Add(Event);
            }
        }
    }

    return FilteredEvents;
}

void UMingEventManager::SetEventBatchingEnabled(bool bEnabled)
{
    if (bBatchingEnabled && !bEnabled)
    {
        // Process any pending events before disabling
        ProcessBatch();
    }

    bBatchingEnabled = bEnabled;
}

void UMingEventManager::SetBatchInterval(float Interval)
{
    BatchInterval = FMath::Max(0.01f, Interval);
}

void UMingEventManager::ProcessPendingEvents()
{
    ProcessBatch();
}

void UMingEventManager::ProcessBatch()
{
    if (PendingEvents.Num() == 0)
    {
        return;
    }

    LastBatchProcessTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    // Sort events by timestamp
    PendingEvents.Sort([](const FMingGameEvent& A, const FMingGameEvent& B)
    {
        return A.Timestamp < B.Timestamp;
    });

    // Process all pending events
    for (const FMingGameEvent& Event : PendingEvents)
    {
        DispatchEvent(Event);
        AddToHistory(Event);
    }

    PendingEvents.Empty();

    // Cleanup old history
    CleanupOldHistory();
}

void UMingEventManager::DispatchEvent(const FMingGameEvent& Event)
{
    // Notify type-specific subscribers
    if (EventSubscribers.Contains(Event.EventType))
    {
        for (const FOnGameEvent& Callback : EventSubscribers[Event.EventType])
        {
            if (Callback.IsBound())
            {
                Callback.Execute(Event);
            }
        }
    }

    // Notify global subscribers
    for (const FOnGameEvent& Callback : GlobalSubscribers)
    {
        if (Callback.IsBound())
        {
            Callback.Execute(Event);
        }
    }

    UE_LOG(LogTemp, Verbose, TEXT("Event dispatched: %s"), *GetEventTypeName(Event.EventType));
}

void UMingEventManager::AddToHistory(const FMingGameEvent& Event)
{
    EventHistory.Add(Event);

    // Trim if exceeding max size
    if (EventHistory.Num() > MaxHistorySize)
    {
        EventHistory.RemoveAt(0, EventHistory.Num() - MaxHistorySize);
    }
}

void UMingEventManager::CleanupOldHistory()
{
    // Keep only events from the last 5 minutes by default
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;
    float MaxAge = 300.0f; // 5 minutes

    EventHistory.RemoveAll([CurrentTime, MaxAge](const FMingGameEvent& Event)
    {
        return (CurrentTime - Event.Timestamp) > MaxAge;
    });
}

FString UMingEventManager::GetEventTypeName(EMingGameEventType Type)
{
    switch (Type)
    {
    case EMingGameEventType::UnitSpawned: return TEXT("單位生成");
    case EMingGameEventType::UnitKilled: return TEXT("單位擊殺");
    case EMingGameEventType::UnitDamaged: return TEXT("單位受傷");
    case EMingGameEventType::UnitHealed: return TEXT("單位治療");
    case EMingGameEventType::BuildingConstructed: return TEXT("建築完成");
    case EMingGameEventType::BuildingDestroyed: return TEXT("建築摧毀");
    case EMingGameEventType::ResourceGathered: return TEXT("資源採集");
    case EMingGameEventType::MissionStarted: return TEXT("任務開始");
    case EMingGameEventType::MissionCompleted: return TEXT("任務完成");
    case EMingGameEventType::Victory: return TEXT("勝利");
    case EMingGameEventType::Defeat: return TEXT("失敗");
    default: return TEXT("未知事件");
    }
}

FLinearColor UMingEventManager::GetEventTypeColor(EMingGameEventType Type)
{
    switch (Type)
    {
    case EMingGameEventType::UnitSpawned:
    case EMingGameEventType::BuildingConstructed:
        return FLinearColor(0.0f, 1.0f, 0.0f); // Green

    case EMingGameEventType::UnitKilled:
    case EMingGameEventType::BuildingDestroyed:
    case EMingGameEventType::Defeat:
        return FLinearColor(1.0f, 0.0f, 0.0f); // Red

    case EMingGameEventType::UnitDamaged:
    case EMingGameEventType::BuildingDamaged:
        return FLinearColor(1.0f, 0.5f, 0.0f); // Orange

    case EMingGameEventType::ResourceGathered:
    case EMingGameEventType::ResourceConsumed:
        return FLinearColor(1.0f, 0.8f, 0.0f); // Gold

    case EMingGameEventType::MissionCompleted:
    case EMingGameEventType::Victory:
        return FLinearColor(0.0f, 0.8f, 1.0f); // Cyan

    default:
        return FLinearColor(1.0f, 1.0f, 1.0f); // White
    }
}

TMap<EMingGameEventType, int32> UMingEventManager::GetEventStatistics(float TimeWindow)
{
    TMap<EMingGameEventType, int32> Statistics;
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    for (const FMingGameEvent& Event : EventHistory)
    {
        if (TimeWindow < 0.0f || (CurrentTime - Event.Timestamp) <= TimeWindow)
        {
            int32* Count = Statistics.Find(Event.EventType);
            if (Count)
            {
                (*Count)++;
            }
            else
            {
                Statistics.Add(Event.EventType, 1);
            }
        }
    }

    return Statistics;
}

void UMingEventManager::ExportEventLog(const FString& FilePath)
{
    FString LogContent = TEXT("=== MingGoRTS Event Log ===\n\n");

    for (const FMingGameEvent& Event : EventHistory)
    {
        LogContent += FString::Printf(TEXT("[%s] Type: %s | ID: %s | Source: %s | Target: %s\n"),
            *FString::Printf(TEXT("%.2f"), Event.Timestamp),
            *GetEventTypeName(Event.EventType),
            *Event.EventID,
            Event.SourceObject.IsValid() ? *Event.SourceObject->GetName() : TEXT("None"),
            Event.TargetObject.IsValid() ? *Event.TargetObject->GetName() : TEXT("None")
        );
    }

    FFileHelper::SaveStringToFile(LogContent, *FilePath);
    UE_LOG(LogTemp, Log, TEXT("Event log exported to: %s"), *FilePath);
}
