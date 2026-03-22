// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingSystemEventBus.generated.h"

UENUM(BlueprintType)
enum class ESystemEventType: uint8 {
	GameStarted,
	GamePaused,
	GameResumed,
	GameEnded,
	LevelLoaded,
	LevelUnloaded,
	PlayerJoined,
	PlayerLeft,
	PlayerAction,
	UnitCreated,
	UnitDestroyed,
	UnitSelected,
	UnitMoved,
	CombatStarted,
	CombatEnded,
	ResourceChanged,
	BuildingConstructed,
	BuildingDestroyed,
	ResearchCompleted,
	DiplomacyChanged,
	SaveGame,
	LoadGame,
	SettingsChanged,
	AudioEvent,
	UIEvent,
	NetworkEvent,
	AIEvent,
	DatabaseEvent,
	LocalizationEvent,
	PerformanceEvent,
	Error,
	Custom
};

UENUM(BlueprintType)
enum class ESystemEventPriority: uint8 {
	Critical,
	High,
	Normal,
	Low,
	Background
};

USTRUCT(BlueprintType)
struct FSystemEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	FGuid EventId;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	ESystemEventType EventType;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	FString EventName;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	FString Sender;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	TMap<FString, FString> StringData;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	TMap<FString, int32> IntData;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	TMap<FString, float> FloatData;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	ESystemEventPriority Priority;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	float Timestamp;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	bool bPropagated;

	FSystemEvent()
		: EventId(FGuid::NewGuid())
		, EventType(ESystemEventType::Custom)
		, Priority(ESystemEventPriority::Normal)
		, Timestamp(0.0f)
		, bPropagated(false)
	{
	}

	static FSystemEvent Create(ESystemEventType Type, const FString& Name, const FString& SenderName)
	{
		FSystemEvent Event;
		Event.EventType = Type;
		Event.EventName = Name;
		Event.Sender = SenderName;
		Event.Timestamp = FPlatformTime::Seconds();
		return Event;
	}

	FSystemEvent& WithString(const FString& Key, const FString& Value)
	{
		StringData.Add(Key, Value);
		return *this;
	}

	FSystemEvent& WithInt(const FString& Key, int32 Value)
	{
		IntData.Add(Key, Value);
		return *this;
	}

	FSystemEvent& WithFloat(const FString& Key, float Value)
	{
		FloatData.Add(Key, Value);
		return *this;
	}
};

USTRUCT(BlueprintType)
struct FEventSubscription
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	FGuid SubscriptionId;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	ESystemEventType EventType;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	FString Filter;

	UPROPERTY(BlueprintReadOnly, Category = "EventBus")
	bool bAsync;

	FEventSubscription()
		: EventType(ESystemEventType::Custom)
		, bAsync(false)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemEvent, const FSystemEvent&, Event);
DECLARE_DYNAMIC_DELEGATE_OneParam(FSystemEventHandler, const FSystemEvent&, Event);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingSystemEventBus : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingSystemEventBus();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void PublishEvent(const FSystemEvent& Event);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	FGuid Subscribe(ESystemEventType EventType, const FSystemEventHandler& Handler);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void Unsubscribe(const FGuid& SubscriptionId);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void SubscribeToAll(const FSystemEventHandler& Handler);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void ClearSubscriptions(ESystemEventType EventType);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void ClearAllSubscriptions();

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void ProcessPendingEvents();

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void SetEventQueueEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	void SetMaxQueueSize(int32 MaxSize);

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	int32 GetPendingEventCount() const;

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	TArray<FSystemEvent> GetEventHistory(int32 Limit = 100) const;

	UFUNCTION(BlueprintCallable, Category = "EventBus")
	FString GetEventBusStatistics() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "EventBus|Global")
	FOnSystemEvent OnAnyEvent;

protected:
	UPROPERTY()
	TMap<ESystemEventType, TMap<FGuid, FSystemEventHandler>> Subscribers;

	UPROPERTY()
	TArray<FSystemEventHandler> GlobalSubscribers;

	UPROPERTY()
	TArray<FSystemEvent> PendingEvents;

	UPROPERTY()
	TArray<FSystemEvent> EventHistory;

	UPROPERTY()
	bool bQueueEnabled;

	UPROPERTY()
	int32 MaxQueueSize;

	UPROPERTY()
	int32 MaxHistorySize;

	UPROPERTY()
	FTimerHandle ProcessingTimerHandle;

	UPROPERTY()
	FCriticalSection EventLock;

protected:
	void ProcessEvent(const FSystemEvent& Event);
	void AddToHistory(const FSystemEvent& Event);
	void OnProcessingTick();
	bool ShouldQueueEvent(const FSystemEvent& Event) const;
};
