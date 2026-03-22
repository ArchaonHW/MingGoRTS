#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEventManager.generated.h"

UENUM(BlueprintType)
enum class EMingGameEventType: uint8 {
    // Combat Events
    UnitSpawned UMETA(DisplayName = "Unit Spawned"),
    UnitKilled UMETA(DisplayName = "Unit Killed"),
    UnitDamaged UMETA(DisplayName = "Unit Damaged"),
    UnitHealed UMETA(DisplayName = "Unit Healed"),
    UnitPromoted UMETA(DisplayName = "Unit Promoted"),
    
    // Building Events
    BuildingConstructed UMETA(DisplayName = "Building Constructed"),
    BuildingDestroyed UMETA(DisplayName = "Building Destroyed"),
    BuildingDamaged UMETA(DisplayName = "Building Damaged"),
    BuildingUpgraded UMETA(DisplayName = "Building Upgraded"),
    
    // Resource Events
    ResourceGathered UMETA(DisplayName = "Resource Gathered"),
    ResourceConsumed UMETA(DisplayName = "Resource Consumed"),
    ResourceDepleted UMETA(DisplayName = "Resource Depleted"),
    
    // Mission/Quest Events
    MissionStarted UMETA(DisplayName = "Mission Started"),
    MissionCompleted UMETA(DisplayName = "Mission Completed"),
    MissionFailed UMETA(DisplayName = "Mission Failed"),
    ObjectiveUpdated UMETA(DisplayName = "Objective Updated"),
    
    // Game State Events
    GameStarted UMETA(DisplayName = "Game Started"),
    GamePaused UMETA(DisplayName = "Game Paused"),
    GameResumed UMETA(DisplayName = "Game Resumed"),
    GameEnded UMETA(DisplayName = "Game Ended"),
    Victory UMETA(DisplayName = "Victory"),
    Defeat UMETA(DisplayName = "Defeat"),
    
    // AI/Strategy Events
    AIAlert UMETA(DisplayName = "AI Alert"),
    ReinforcementsCalled UMETA(DisplayName = "Reinforcements Called"),
    TerritoryCaptured UMETA(DisplayName = "Territory Captured"),
    TerritoryLost UMETA(DisplayName = "Territory Lost"),
    
    // Interaction Events
    PlayerJoined UMETA(DisplayName = "Player Joined"),
    PlayerLeft UMETA(DisplayName = "Player Left"),
    AllianceFormed UMETA(DisplayName = "Alliance Formed"),
    AllianceBroken UMETA(DisplayName = "Alliance Broken")
};

USTRUCT(BlueprintType)
struct FMingGameEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EMingGameEventType EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    float Timestamp;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<class UObject> SourceObject;

    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<class UObject> TargetObject;

    UPROPERTY(BlueprintReadOnly)
    FVector Location;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> StringData;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> IntData;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FloatData;

    FMingGameEvent()
        : EventType(EMingGameEventType::UnitSpawned)
        , Timestamp(0.0f)
        , Location(FVector::ZeroVector)
    {}
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnGameEvent, const FMingGameEvent&, Event);

/**
 * Event Manager for MingGoRTS
 * Central system for game event handling and dispatching
 */
UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGCORE_API UMingEventManager : public UObject
{
    GENERATED_BODY()

public:
    UMingEventManager();

    // Singleton access
    UFUNCTION(BlueprintPure, Category = "Event Manager", meta = (WorldContext = "WorldContextObject"))
    static UMingEventManager* Get(UObject* WorldContextObject);

    // Event Publishing
    UFUNCTION(BlueprintCallable, Category = "Events")
    void PublishEvent(const FMingGameEvent& Event) {};

    UFUNCTION(BlueprintCallable, Category = "Events")
    void PublishSimpleEvent(EMingGameEventType Type, UObject* Source = nullptr, UObject* Target = nullptr);

    UFUNCTION(BlueprintCallable, Category = "Events")
    void PublishCombatEvent(EMingGameEventType Type, class AActor* Attacker, class AActor* Target, int32 DamageDealt);

    UFUNCTION(BlueprintCallable, Category = "Events")
    void PublishResourceEvent(EMingGameEventType Type, EMingResourceType Resource, int32 Amount, UObject* Source);

    // Event Subscription
    UFUNCTION(BlueprintCallable, Category = "Events")
    void SubscribeToEvent(EMingGameEventType EventType, const FOnGameEvent& Callback) {};

    UFUNCTION(BlueprintCallable, Category = "Events")
    void UnsubscribeFromEvent(EMingGameEventType EventType, const FOnGameEvent& Callback) {};

    UFUNCTION(BlueprintCallable, Category = "Events")
    void SubscribeToAllEvents(const FOnGameEvent& Callback) {};

    UFUNCTION(BlueprintCallable, Category = "Events")
    void ClearAllSubscriptions();

    // Event History
    UFUNCTION(BlueprintCallable, Category = "History")
    TArray<FMingGameEvent> GetEventHistory(EMingGameEventType EventType, float TimeWindow = -1.0f);

    UFUNCTION(BlueprintCallable, Category = "History")
    TArray<FMingGameEvent> GetAllEventHistory(float TimeWindow = -1.0f);

    UFUNCTION(BlueprintCallable, Category = "History")
    void ClearHistory();

    UFUNCTION(BlueprintPure, Category = "History")
    int32 GetEventCount(EMingGameEventType EventType, float TimeWindow = -1.0f);

    // Event Query
    UFUNCTION(BlueprintPure, Category = "Query")
    bool HasEventOccurred(EMingGameEventType EventType, float TimeWindow = -1.0f);

    UFUNCTION(BlueprintPure, Category = "Query")
    FMingGameEvent GetLastEventOfType(EMingGameEventType EventType);

    UFUNCTION(BlueprintCallable, Category = "Query")
    TArray<FMingGameEvent> GetEventsForObject(UObject* Object, float TimeWindow = -1.0f);

    // Event Processing
    UFUNCTION(BlueprintCallable, Category = "Processing")
    void SetEventBatchingEnabled(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "Processing")
    void SetBatchInterval(float Interval);

    UFUNCTION(BlueprintCallable, Category = "Processing")
    void ProcessPendingEvents();

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetEventTypeName(EMingGameEventType Type);

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FLinearColor GetEventTypeColor(EMingGameEventType Type);

    // Statistics
    UFUNCTION(BlueprintCallable, Category = "Statistics")
    TMap<EMingGameEventType, int32> GetEventStatistics(float TimeWindow = -1.0f);

    UFUNCTION(BlueprintCallable, Category = "Statistics")
    void ExportEventLog(const FString& FilePath) {};

protected:
    // Event storage
    UPROPERTY()
    TArray<FMingGameEvent> EventHistory;

    // Pending events for batching
    UPROPERTY()
    TArray<FMingGameEvent> PendingEvents;

    // Subscribers
    TMap<EMingGameEventType, TArray<FOnGameEvent>> EventSubscribers;
    TArray<FOnGameEvent> GlobalSubscribers;

    // Settings
    UPROPERTY()
    bool bBatchingEnabled;

    UPROPERTY()
    float BatchInterval;

    UPROPERTY()
    float LastBatchProcessTime;

    UPROPERTY()
    int32 MaxHistorySize;

    // Internal functions
    void DispatchEvent(const FMingGameEvent& Event) {};
    void AddToHistory(const FMingGameEvent& Event) {};
    void CleanupOldHistory();
    void ProcessBatch();
    
    UPROPERTY()
    TWeakObjectPtr<UWorld> CachedWorld;
};

