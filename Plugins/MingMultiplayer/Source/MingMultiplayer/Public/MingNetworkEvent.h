#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingNetworkEvent.generated.h"

UENUM(BlueprintType)
enum class EMingNetworkEventType : uint8
{
    None UMETA(DisplayName = "None"),
    
    // Connection Events
    PlayerConnected UMETA(DisplayName = "Player Connected"),
    PlayerDisconnected UMETA(DisplayName = "Player Disconnected"),
    PlayerJoined UMETA(DisplayName = "Player Joined"),
    PlayerLeft UMETA(DisplayName = "Player Left"),
    ConnectionLost UMETA(DisplayName = "Connection Lost"),
    ConnectionRestored UMETA(DisplayName = "Connection Restored"),
    
    // Game State Events
    GameStarted UMETA(DisplayName = "Game Started"),
    GameEnded UMETA(DisplayName = "Game Ended"),
    GamePaused UMETA(DisplayName = "Game Paused"),
    GameResumed UMETA(DisplayName = "Game Resumed"),
    MapChanged UMETA(DisplayName = "Map Changed"),
    RoundStarted UMETA(DisplayName = "Round Started"),
    RoundEnded UMETA(DisplayName = "Round Ended"),
    
    // Relationship Events
    RelationshipChanged UMETA(DisplayName = "Relationship Changed"),
    RelationshipLevelUp UMETA(DisplayName = "Relationship Level Up"),
    CharacterMet UMETA(DisplayName = "Character Met"),
    CharacterRelationshipThreshold UMETA(DisplayName = "Relationship Threshold"),
    
    // Reputation Events
    ReputationChanged UMETA(DisplayName = "Reputation Changed"),
    ReputationRankUp UMETA(DisplayName = "Reputation Rank Up"),
    ReputationRankDown UMETA(DisplayName = "Reputation Rank Down"),
    RegionAllegianceChanged UMETA(DisplayName = "Region Allegiance Changed"),
    QuestCompleted UMETA(DisplayName = "Quest Completed"),
    
    // Unit Events
    UnitSpawned UMETA(DisplayName = "Unit Spawned"),
    UnitDestroyed UMETA(DisplayName = "Unit Destroyed"),
    UnitSelected UMETA(DisplayName = "Unit Selected"),
    UnitCommandIssued UMETA(DisplayName = "Unit Command Issued"),
    UnitMoveCompleted UMETA(DisplayName = "Unit Move Completed"),
    UnitCombatStarted UMETA(DisplayName = "Unit Combat Started"),
    UnitCombatEnded UMETA(DisplayName = "Unit Combat Ended"),
    
    // Building Events
    BuildingConstructed UMETA(DisplayName = "Building Constructed"),
    BuildingDestroyed UMETA(DisplayName = "Building Destroyed"),
    BuildingUpgraded UMETA(DisplayName = "Building Upgraded"),
    
    // Resource Events
    ResourceCollected UMETA(DisplayName = "Resource Collected"),
    ResourceSpent UMETA(DisplayName = "Resource Spent"),
    ResourceDepleted UMETA(DisplayName = "Resource Depleted"),
    
    // Combat Events
    BattleStarted UMETA(DisplayName = "Battle Started"),
    BattleEnded UMETA(DisplayName = "Battle Ended"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    
    // Chat Events
    ChatMessageReceived UMETA(DisplayName = "Chat Message Received"),
    SystemMessage UMETA(DisplayName = "System Message"),
    WhisperReceived UMETA(DisplayName = "Whisper Received"),
    
    // Error Events
    NetworkError UMETA(DisplayName = "Network Error"),
    DesyncDetected UMETA(DisplayName = "Desync Detected"),
    CheatDetected UMETA(DisplayName = "Cheat Detected"),
    ServerFull UMETA(DisplayName = "Server Full")
};

USTRUCT(BlueprintType)
struct FMingNetworkEvent
{
    GENERATED_BODY()

    UPROPERTY()
    EMingNetworkEventType EventType;

    UPROPERTY()
    int32 SenderID;

    UPROPERTY()
    int32 TargetID;

    UPROPERTY()
    FString EventData;

    UPROPERTY()
    float Timestamp;

    UPROPERTY()
    FName SourceObject;

    UPROPERTY()
    bool bReliable;

    UPROPERTY()
    int32 Priority;

    UPROPERTY()
    TArray<uint8> BinaryPayload;

    FMingNetworkEvent()
        : EventType(EMingNetworkEventType::None)
        , SenderID(-1)
        , TargetID(-1)
        , Timestamp(0.0f)
        , bReliable(true)
        , Priority(0)
    {}

    bool IsValid() const
    {
        return EventType != EMingNetworkEventType::None && SenderID >= 0;
    }
};

USTRUCT(BlueprintType)
struct FMingNetworkEventHandler
{
    GENERATED_BODY()

    UPROPERTY()
    EMingNetworkEventType EventType;

    UPROPERTY()
    FName HandlerID;

    UPROPERTY()
    int32 Priority;

    UPROPERTY()
    bool bConsumeEvent;

    FMingNetworkEventHandler()
        : EventType(EMingNetworkEventType::None)
        , Priority(0)
        , bConsumeEvent(false)
    {}
};

/**
 * Network Event System
 * Handles routing and processing of multiplayer network events
 */
UCLASS(ClassGroup = (Multiplayer, Events))
class MINGMULTIPLAYER_API UMingNetworkEventSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingNetworkEventSystem(};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void InitializeEventSystem(class UMingNetworkManager* NetworkManager};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void ShutdownEventSystem(};

    // Event Registration
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    FName RegisterEventHandler(EMingNetworkEventType EventType, UObject* Handler, FName FunctionName};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void UnregisterEventHandler(FName HandlerID};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void UnregisterAllHandlers(UObject* Handler};

    // Event Broadcasting
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void BroadcastEvent(const FMingNetworkEvent& Event};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void SendEventToPlayer(int32 PlayerID, const FMingNetworkEvent& Event};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void SendEventToHost(const FMingNetworkEvent& Event};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void MulticastEvent(const FMingNetworkEvent& Event};

    // Convenience Event Creation
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FirePlayerConnectedEvent(int32 PlayerID, const FString& PlayerName};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FirePlayerDisconnectedEvent(int32 PlayerID, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireRelationshipChangedEvent(int32 PlayerID, FName CharacterID, float OldValue, float NewValue};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireReputationChangedEvent(int32 PlayerID, FName RegionID, float OldValue, float NewValue};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireGameStateChangedEvent(const FString& NewState};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireChatMessageEvent(int32 SenderID, const FString& Message, int32 Channel};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireSystemMessageEvent(const FString& Message};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireUnitEvent(EMingNetworkEventType EventType, int32 UnitID, int32 OwnerPlayerID, const FVector& Location};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireCombatEvent(EMingNetworkEventType EventType, int32 AttackerID, int32 DefenderID, float Damage};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FireResourceEvent(EMingNetworkEventType EventType, int32 PlayerID, const FString& ResourceType, int32 Amount};

    // Event Processing
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void ProcessPendingEvents(};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void ClearPendingEvents(};

    UFUNCTION(BlueprintPure, Category = "Network Events")
    int32 GetPendingEventCount() const;

    // Event Validation
    UFUNCTION(BlueprintPure, Category = "Network Events")
    bool ValidateEvent(const FMingNetworkEvent& Event) const;

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FilterEventsByType(EMingNetworkEventType EventType, TArray<FMingNetworkEvent>& OutEvents) const;

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void FilterEventsByPlayer(int32 PlayerID, TArray<FMingNetworkEvent>& OutEvents) const;

    // Event History
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void EnableEventHistory(bool bEnabled};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void ClearEventHistory(};

    UFUNCTION(BlueprintPure, Category = "Network Events")
    TArray<FMingNetworkEvent> GetEventHistory(float SinceTimestamp) const;

    UFUNCTION(BlueprintPure, Category = "Network Events")
    FMingNetworkEvent GetLastEventOfType(EMingNetworkEventType EventType) const;

    // Event Rate Limiting
    UFUNCTION(BlueprintCallable, Category = "Network Events")
    void SetEventRateLimit(EMingNetworkEventType EventType, float MaxEventsPerSecond};

    UFUNCTION(BlueprintCallable, Category = "Network Events")
    bool CheckEventRateLimit(EMingNetworkEventType EventType};

    // Event Statistics
    UFUNCTION(BlueprintPure, Category = "Network Events")
    int32 GetTotalEventsFired() const { return TotalEventsFired; }

    UFUNCTION(BlueprintPure, Category = "Network Events")
    int32 GetTotalEventsProcessed() const { return TotalEventsProcessed; }

    UFUNCTION(BlueprintPure, Category = "Network Events")
    float GetAverageEventLatency() const;

protected:
    UPROPERTY()
    TObjectPtr<UMingNetworkManager> NetworkMgr;

    UPROPERTY()
    TArray<FMingNetworkEvent> PendingEvents;

    UPROPERTY()
    TArray<FMingNetworkEvent> EventHistory;

    UPROPERTY()
    TMap<EMingNetworkEventType, TArray<FMingNetworkEventHandler>> EventHandlers;

    UPROPERTY()
    TMap<FName, TWeakObjectPtr<UObject>> HandlerObjects;

    UPROPERTY()
    TMap<EMingNetworkEventType, float> EventRateLimits;

    UPROPERTY()
    TMap<EMingNetworkEventType, float> LastEventTimestamps;

    UPROPERTY()
    bool bHistoryEnabled;

    UPROPERTY()
    int32 MaxHistorySize;

    UPROPERTY()
    int32 NextHandlerID;

    UPROPERTY()
    int32 TotalEventsFired;

    UPROPERTY()
    int32 TotalEventsProcessed;

    UPROPERTY()
    TArray<float> EventLatencies;

    // Internal Functions
    void ProcessEvent(const FMingNetworkEvent& Event};
    void ExecuteEventHandler(const FMingNetworkEventHandler& Handler, const FMingNetworkEvent& Event};
    void AddToHistory(const FMingNetworkEvent& Event};
    void TrimHistory(};

    FName GenerateHandlerID(};
    bool ShouldProcessEvent(const FMingNetworkEvent& Event) const;
    void SendEventOverNetwork(const FMingNetworkEvent& Event};
    void ReceiveEventFromNetwork(const FMingNetworkEvent& Event};

    // Network Event Handlers
    UFUNCTION()
    void OnPlayerConnected(int32 PlayerID, const FMingPlayerNetworkInfo& PlayerInfo};

    UFUNCTION()
    void OnPlayerDisconnected(int32 PlayerID};

    UFUNCTION()
    void OnNetworkMessageReceived(int32 SenderID, int32 MessageType, const TArray<uint8>& Data};

public:
    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkEventFired, const FMingNetworkEvent&, Event};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNetworkEventProcessed, const FMingNetworkEvent&, Event, bool, bSuccess};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNetworkEventFired OnEventFired;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNetworkEventProcessed OnEventProcessed;
};

