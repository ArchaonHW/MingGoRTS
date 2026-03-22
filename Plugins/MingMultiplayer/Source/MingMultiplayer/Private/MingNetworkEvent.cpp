#include "MingNetworkEvent.h"
#include "MingNetworkManager.h"
#include "Engine/Engine.h"

UMingNetworkEventSystem::UMingNetworkEventSystem()
    : bHistoryEnabled(true)
    , MaxHistorySize(1000)
    , NextHandlerID(0)
    , TotalEventsFired(0)
    , TotalEventsProcessed(0)
{
}

void UMingNetworkEventSystem::InitializeEventSystem(UMingNetworkManager* NetworkManager)
{
    NetworkMgr = NetworkManager;
    
    if (NetworkMgr)
    {
        NetworkMgr->OnPlayerConnected.AddDynamic(this, &UMingNetworkEventSystem::OnPlayerConnected);
        NetworkMgr->OnPlayerDisconnected.AddDynamic(this, &UMingNetworkEventSystem::OnPlayerDisconnected);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Network event system initialized"));
}

void UMingNetworkEventSystem::ShutdownEventSystem()
{
    if (NetworkMgr)
    {
        NetworkMgr->OnPlayerConnected.RemoveAll(this);
        NetworkMgr->OnPlayerDisconnected.RemoveAll(this);
    }
    
    PendingEvents.Empty();
    EventHistory.Empty();
    EventHandlers.Empty();
    HandlerObjects.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Network event system shutdown"));
}

FName UMingNetworkEventSystem::RegisterEventHandler(EMingNetworkEventType EventType, UObject* Handler, FName FunctionName)
{
    if (!Handler)
    {
        return NAME_None;
    }
    
    FName HandlerID = GenerateHandlerID();
    
    FMingNetworkEventHandler EventHandler;
    EventHandler.EventType = EventType;
    EventHandler.HandlerID = HandlerID;
    EventHandler.Priority = 0;
    EventHandler.bConsumeEvent = false;
    
    if (!EventHandlers.Contains(EventType))
    {
        EventHandlers.Add(EventType, TArray<FMingNetworkEventHandler>());
    }
    
    EventHandlers[EventType].Add(EventHandler);
    HandlerObjects.Add(HandlerID, Handler);
    
    UE_LOG(LogTemp, Log, TEXT("Registered event handler %s for event type %d"), 
        *HandlerID.ToString(), static_cast<int32>(EventType));
    
    return HandlerID;
}

void UMingNetworkEventSystem::UnregisterEventHandler(FName HandlerID)
{
    // Remove from handler objects
    HandlerObjects.Remove(HandlerID);
    
    // Remove from event handlers
    for (auto& Pair : EventHandlers)
    {
        Pair.Value.RemoveAll([HandlerID](const FMingNetworkEventHandler& Handler) {
            return Handler.HandlerID == HandlerID;
        });
    }
}

void UMingNetworkEventSystem::UnregisterAllHandlers(UObject* Handler)
{
    // Find all handler IDs for this object
    TArray<FName> HandlerIDs;
    for (const auto& Pair : HandlerObjects)
    {
        if (Pair.Value.Get() == Handler)
        {
            HandlerIDs.Add(Pair.Key);
        }
    }
    
    // Unregister each handler
    for (FName HandlerID : HandlerIDs)
    {
        UnregisterEventHandler(HandlerID);
    }
}

void UMingNetworkEventSystem::BroadcastEvent(const FMingNetworkEvent& Event)
{
    if (!ValidateEvent(Event))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid network event rejected"));
        return;
    }
    
    // Check rate limit
    if (!CheckEventRateLimit(Event.EventType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Event rate limit exceeded for type %d"), static_cast<int32>(Event.EventType));
        return;
    }
    
    // Add to pending events for processing
    FMingNetworkEvent EventCopy = Event;
    EventCopy.Timestamp = GetWorld()->GetTimeSeconds();
    
    PendingEvents.Add(EventCopy);
    TotalEventsFired++;
    
    // Add to history
    if (bHistoryEnabled)
    {
        AddToHistory(EventCopy);
    }
    
    // Send over network if needed
    if (Event.SenderID != NetworkMgr->GetLocalPlayerID())
    {
        SendEventOverNetwork(EventCopy);
    }
    
    OnEventFired.Broadcast(EventCopy);
    
    UE_LOG(LogTemp, Log, TEXT("Broadcasted event type %d"), static_cast<int32>(Event.EventType));
}

void UMingNetworkEventSystem::SendEventToPlayer(int32 PlayerID, const FMingNetworkEvent& Event)
{
    FMingNetworkEvent TargetedEvent = Event;
    TargetedEvent.TargetID = PlayerID;
    TargetedEvent.bReliable = true;
    
    BroadcastEvent(TargetedEvent);
}

void UMingNetworkEventSystem::SendEventToHost(const FMingNetworkEvent& Event)
{
    if (NetworkMgr && NetworkMgr->IsConnected())
    {
        FMingNetworkEvent HostEvent = Event;
        HostEvent.TargetID = 0; // Host is always player 0
        HostEvent.bReliable = true;
        
        BroadcastEvent(HostEvent);
    }
}

void UMingNetworkEventSystem::MulticastEvent(const FMingNetworkEvent& Event)
{
    if (NetworkMgr)
    {
        FMingNetworkEvent MulticastEvent = Event;
        MulticastEvent.TargetID = -1; // -1 means all players
        
        BroadcastEvent(MulticastEvent);
    }
}

void UMingNetworkEventSystem::FirePlayerConnectedEvent(int32 PlayerID, const FString& PlayerName)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::PlayerConnected;
    Event.SenderID = PlayerID;
    Event.EventData = PlayerName;
    Event.bReliable = true;
    Event.Priority = 10;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FirePlayerDisconnectedEvent(int32 PlayerID, const FString& Reason)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::PlayerDisconnected;
    Event.SenderID = PlayerID;
    Event.EventData = Reason;
    Event.bReliable = true;
    Event.Priority = 10;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireRelationshipChangedEvent(int32 PlayerID, FName CharacterID, float OldValue, float NewValue)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::RelationshipChanged;
    Event.SenderID = PlayerID;
    Event.SourceObject = CharacterID;
    Event.EventData = FString::Printf(TEXT("%.2f,%.2f"), OldValue, NewValue);
    Event.bReliable = true;
    Event.Priority = 5;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireReputationChangedEvent(int32 PlayerID, FName RegionID, float OldValue, float NewValue)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::ReputationChanged;
    Event.SenderID = PlayerID;
    Event.SourceObject = RegionID;
    Event.EventData = FString::Printf(TEXT("%.2f,%.2f"), OldValue, NewValue);
    Event.bReliable = true;
    Event.Priority = 5;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireGameStateChangedEvent(const FString& NewState)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::GameStarted;
    Event.SenderID = NetworkMgr ? NetworkMgr->GetLocalPlayerID() : 0;
    Event.EventData = NewState;
    Event.bReliable = true;
    Event.Priority = 8;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireChatMessageEvent(int32 SenderID, const FString& Message, int32 Channel)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::ChatMessageReceived;
    Event.SenderID = SenderID;
    Event.EventData = FString::Printf(TEXT("%d|%s"), Channel, *Message);
    Event.bReliable = false;
    Event.Priority = 1;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireSystemMessageEvent(const FString& Message)
{
    FMingNetworkEvent Event;
    Event.EventType = EMingNetworkEventType::SystemMessage;
    Event.SenderID = -1; // System
    Event.EventData = Message;
    Event.bReliable = true;
    Event.Priority = 7;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireUnitEvent(EMingNetworkEventType EventType, int32 UnitID, int32 OwnerPlayerID, const FVector& Location)
{
    FMingNetworkEvent Event;
    Event.EventType = EventType;
    Event.SenderID = OwnerPlayerID;
    Event.EventData = FString::Printf(TEXT("%d|%f|%f|%f"), UnitID, Location.X, Location.Y, Location.Z);
    Event.bReliable = false;
    Event.Priority = 3;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireCombatEvent(EMingNetworkEventType EventType, int32 AttackerID, int32 DefenderID, float Damage)
{
    FMingNetworkEvent Event;
    Event.EventType = EventType;
    Event.EventData = FString::Printf(TEXT("%d|%d|%.2f"), AttackerID, DefenderID, Damage);
    Event.bReliable = true;
    Event.Priority = 6;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::FireResourceEvent(EMingNetworkEventType EventType, int32 PlayerID, const FString& ResourceType, int32 Amount)
{
    FMingNetworkEvent Event;
    Event.EventType = EventType;
    Event.SenderID = PlayerID;
    Event.EventData = FString::Printf(TEXT("%s|%d"), *ResourceType, Amount);
    Event.bReliable = true;
    Event.Priority = 4;
    
    BroadcastEvent(Event);
}

void UMingNetworkEventSystem::ProcessPendingEvents()
{
    // Process all pending events
    for (FMingNetworkEvent& Event : PendingEvents)
    {
        ProcessEvent(Event);
    }
    
    PendingEvents.Empty();
}

void UMingNetworkEventSystem::ClearPendingEvents()
{
    PendingEvents.Empty();
}

int32 UMingNetworkEventSystem::GetPendingEventCount() const
{
    return PendingEvents.Num();
}

bool UMingNetworkEventSystem::ValidateEvent(const FMingNetworkEvent& Event) const
{
    if (Event.EventType == EMingNetworkEventType::None)
    {
        return false;
    }
    
    if (Event.SenderID < -1) // -1 is valid for system events
    {
        return false;
    }
    
    return true;
}

void UMingNetworkEventSystem::FilterEventsByType(EMingNetworkEventType EventType, TArray<FMingNetworkEvent>& OutEvents) const
{
    OutEvents.Empty();
    
    for (const FMingNetworkEvent& Event : EventHistory)
    {
        if (Event.EventType == EventType)
        {
            OutEvents.Add(Event);
        }
    }
}

void UMingNetworkEventSystem::FilterEventsByPlayer(int32 PlayerID, TArray<FMingNetworkEvent>& OutEvents) const
{
    OutEvents.Empty();
    
    for (const FMingNetworkEvent& Event : EventHistory)
    {
        if (Event.SenderID == PlayerID || Event.TargetID == PlayerID)
        {
            OutEvents.Add(Event);
        }
    }
}

void UMingNetworkEventSystem::EnableEventHistory(bool bEnabled)
{
    bHistoryEnabled = bEnabled;
    
    if (!bEnabled)
    {
        EventHistory.Empty();
    }
}

void UMingNetworkEventSystem::ClearEventHistory()
{
    EventHistory.Empty();
}

TArray<FMingNetworkEvent> UMingNetworkEventSystem::GetEventHistory(float SinceTimestamp) const
{
    TArray<FMingNetworkEvent> Result;
    
    for (const FMingNetworkEvent& Event : EventHistory)
    {
        if (Event.Timestamp >= SinceTimestamp)
        {
            Result.Add(Event);
        }
    }
    
    return Result;
}

FMingNetworkEvent UMingNetworkEventSystem::GetLastEventOfType(EMingNetworkEventType EventType) const
{
    for (int32 i = EventHistory.Num() - 1; i >= 0; i--)
    {
        if (EventHistory[i].EventType == EventType)
        {
            return EventHistory[i];
        }
    }
    
    return FMingNetworkEvent();
}

void UMingNetworkEventSystem::SetEventRateLimit(EMingNetworkEventType EventType, float MaxEventsPerSecond)
{
    EventRateLimits.Add(EventType, MaxEventsPerSecond);
}

bool UMingNetworkEventSystem::CheckEventRateLimit(EMingNetworkEventType EventType)
{
    if (!EventRateLimits.Contains(EventType))
    {
        return true;
    }
    
    float MaxRate = EventRateLimits[EventType];
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    if (LastEventTimestamps.Contains(EventType))
    {
        float LastTime = LastEventTimestamps[EventType];
        float TimeSinceLast = CurrentTime - LastTime;
        
        if (TimeSinceLast < (1.0f / MaxRate))
        {
            return false;
        }
    }
    
    LastEventTimestamps.Add(EventType, CurrentTime);
    return true;
}

float UMingNetworkEventSystem::GetAverageEventLatency() const
{
    if (EventLatencies.Num() == 0)
    {
        return 0.0f;
    }
    
    float Total = 0.0f;
    for (float Latency : EventLatencies)
    {
        Total += Latency;
    }
    
    return Total / EventLatencies.Num();
}

void UMingNetworkEventSystem::ProcessEvent(const FMingNetworkEvent& Event)
{
    // Execute registered handlers
    if (EventHandlers.Contains(Event.EventType))
    {
        for (const FMingNetworkEventHandler& Handler : EventHandlers[Event.EventType])
        {
            ExecuteEventHandler(Handler, Event);
            
            if (Handler.bConsumeEvent)
            {
                break;
            }
        }
    }
    
    TotalEventsProcessed++;
    OnEventProcessed.Broadcast(Event, true);
}

void UMingNetworkEventSystem::ExecuteEventHandler(const FMingNetworkEventHandler& Handler, const FMingNetworkEvent& Event)
{
    TWeakObjectPtr<UObject> HandlerObj = HandlerObjects[Handler.HandlerID];
    
    if (HandlerObj.IsValid())
    {
        // Call the handler function on the object
        // This would use reflection to call the function by name
        UE_LOG(LogTemp, Log, TEXT("Executing handler %s for event type %d"),
            *Handler.HandlerID.ToString(), static_cast<int32>(Event.EventType));
    }
}

void UMingNetworkEventSystem::AddToHistory(const FMingNetworkEvent& Event)
{
    EventHistory.Add(Event);
    TrimHistory();
}

void UMingNetworkEventSystem::TrimHistory()
{
    while (EventHistory.Num() > MaxHistorySize)
    {
        EventHistory.RemoveAt(0);
    }
}

FName UMingNetworkEventSystem::GenerateHandlerID()
{
    return FName(*FString::Printf(TEXT("Handler_%d"), NextHandlerID++));
}

bool UMingNetworkEventSystem::ShouldProcessEvent(const FMingNetworkEvent& Event) const
{
    // Check if event is targeted at us
    if (Event.TargetID >= 0 && Event.TargetID != NetworkMgr->GetLocalPlayerID())
    {
        return false;
    }
    
    return true;
}

void UMingNetworkEventSystem::SendEventOverNetwork(const FMingNetworkEvent& Event)
{
    if (!NetworkMgr)
    {
        return;
    }
    
    // Serialize event
    TArray<uint8> SerializedEvent;
    FMemoryWriter Writer(SerializedEvent);
    Writer << const_cast<FMingNetworkEvent&>(Event);
    
    // Determine sync priority based on event type
    EMingSyncPriority Priority = EMingSyncPriority::Normal;
    if (Event.Priority >= 8)
    {
        Priority = EMingSyncPriority::Critical;
    }
    else if (Event.Priority >= 5)
    {
        Priority = EMingSyncPriority::High;
    }
    
    // Send over network
    if (Event.TargetID >= 0)
    {
        NetworkMgr->SendMessage(Event.TargetID, 4, SerializedEvent, Priority, Event.bReliable);
    }
    else
    {
        NetworkMgr->BroadcastMessage(4, SerializedEvent, Priority);
    }
}

void UMingNetworkEventSystem::ReceiveEventFromNetwork(const FMingNetworkEvent& Event)
{
    if (ShouldProcessEvent(Event))
    {
        ProcessEvent(Event);
    }
}

void UMingNetworkEventSystem::OnPlayerConnected(int32 PlayerID, const FMingPlayerNetworkInfo& PlayerInfo)
{
    FirePlayerConnectedEvent(PlayerID, PlayerInfo.PlayerName);
}

void UMingNetworkEventSystem::OnPlayerDisconnected(int32 PlayerID)
{
    FirePlayerDisconnectedEvent(PlayerID, TEXT("Player left the game"));
}

void UMingNetworkEventSystem::OnNetworkMessageReceived(int32 SenderID, int32 MessageType, const TArray<uint8>& Data)
{
    if (MessageType == 4) // Event message type
    {
        // Deserialize event
        FMemoryReader Reader(Data);
        FMingNetworkEvent Event;
        Reader << Event;
        
        ReceiveEventFromNetwork(Event);
    }
}
