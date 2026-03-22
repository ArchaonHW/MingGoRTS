// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MingSystemEventBus.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"

UMingSystemEventBus::UMingSystemEventBus()
	: bQueueEnabled(true)
	, MaxQueueSize(1000)
	, MaxHistorySize(1000)
{
}

void UMingSystemEventBus::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Start event processing timer
	if (GetGameInstance())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UMingSystemEventBus::OnProcessingTick);
		GetGameInstance()->GetTimerManager().SetTimer(ProcessingTimerHandle, TimerDelegate, 0.016f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("System Event Bus initialized"));
}

void UMingSystemEventBus::Deinitialize()
{
	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(ProcessingTimerHandle);
	}

	ClearAllSubscriptions();

	Super::Deinitialize();
}

void UMingSystemEventBus::PublishEvent(const FSystemEvent& Event)
{
	FSystemEvent MutableEvent = Event;
	MutableEvent.Timestamp = FPlatformTime::Seconds();

	if (bQueueEnabled && ShouldQueueEvent(MutableEvent))
	{
		FScopeLock Lock(&EventLock);
		
		if (PendingEvents.Num() >= MaxQueueSize)
		{
			// Remove oldest event
			PendingEvents.RemoveAt(0);
		}
		
		PendingEvents.Add(MutableEvent);
	}
	else
	{
		ProcessEvent(MutableEvent);
	}
}

FGuid UMingSystemEventBus::Subscribe(ESystemEventType EventType, const FSystemEventHandler& Handler)
{
	FGuid SubscriptionId = FGuid::NewGuid();
	
	FScopeLock Lock(&EventLock);
	
	TMap<FGuid, FSystemEventHandler>& Handlers = Subscribers.FindOrAdd(EventType);
	Handlers.Add(SubscriptionId, Handler);
	
	UE_LOG(LogTemp, Verbose, TEXT("Subscribed to event type %s with ID %s"),
		*UEnum::GetValueAsString(EventType), *SubscriptionId.ToString());
	
	return SubscriptionId;
}

void UMingSystemEventBus::Unsubscribe(const FGuid& SubscriptionId)
{
	FScopeLock Lock(&EventLock);
	
	for (auto& Pair : Subscribers)
	{
		Pair.Value.Remove(SubscriptionId);
	}
}

void UMingSystemEventBus::SubscribeToAll(const FSystemEventHandler& Handler)
{
	FScopeLock Lock(&EventLock);
	GlobalSubscribers.Add(Handler);
}

void UMingSystemEventBus::ClearSubscriptions(ESystemEventType EventType)
{
	FScopeLock Lock(&EventLock);
	Subscribers.Remove(EventType);
}

void UMingSystemEventBus::ClearAllSubscriptions()
{
	FScopeLock Lock(&EventLock);
	Subscribers.Empty();
	GlobalSubscribers.Empty();
}

void UMingSystemEventBus::ProcessPendingEvents()
{
	TArray<FSystemEvent> EventsToProcess;
	
	{
		FScopeLock Lock(&EventLock);
		EventsToProcess = PendingEvents;
		PendingEvents.Empty();
	}
	
	for (const auto& Event : EventsToProcess)
	{
		ProcessEvent(Event);
	}
}

void UMingSystemEventBus::SetEventQueueEnabled(bool bEnabled)
{
	bQueueEnabled = bEnabled;
	
	if (!bEnabled)
	{
		// Process any remaining queued events
		ProcessPendingEvents();
	}
}

void UMingSystemEventBus::SetMaxQueueSize(int32 MaxSize)
{
	MaxQueueSize = FMath::Max(1, MaxSize);
	
	FScopeLock Lock(&EventLock);
	while (PendingEvents.Num() > MaxQueueSize)
	{
		PendingEvents.RemoveAt(0);
	}
}

int32 UMingSystemEventBus::GetPendingEventCount() const
{
	return PendingEvents.Num();
}

TArray<FSystemEvent> UMingSystemEventBus::GetEventHistory(int32 Limit) const
{
	TArray<FSystemEvent> Result;
	
	Limit = FMath::Clamp(Limit, 0, EventHistory.Num());
	
	for (int32 i = EventHistory.Num() - Limit; i < EventHistory.Num(); ++i)
	{
		if (i >= 0)
		{
			Result.Add(EventHistory[i]);
		}
	}
	
	return Result;
}

FString UMingSystemEventBus::GetEventBusStatistics() const
{
	FString Stats;
	Stats += TEXT("=== System Event Bus Statistics ===\n\n");
	
	Stats += FString::Printf(TEXT("Pending Events: %d/%d\n"), PendingEvents.Num(), MaxQueueSize);
	Stats += FString::Printf(TEXT("Event History: %d/%d\n"), EventHistory.Num(), MaxHistorySize);
	Stats += FString::Printf(TEXT("Queue Enabled: %s\n\n"), bQueueEnabled ? TEXT("Yes") : TEXT("No"));
	
	Stats += TEXT("Active Subscriptions:\n");
	for (const auto& Pair : Subscribers)
	{
		Stats += FString::Printf(TEXT("  %s: %d handlers\n"),
			*UEnum::GetValueAsString(Pair.Key), Pair.Value.Num());
	}
	
	Stats += FString::Printf(TEXT("\nGlobal Subscribers: %d\n"), GlobalSubscribers.Num());
	
	return Stats;
}

void UMingSystemEventBus::ProcessEvent(const FSystemEvent& Event)
{
	// Add to history
	AddToHistory(Event);
	
	// Broadcast to global subscribers
	OnAnyEvent.Broadcast(Event);
	
	for (const auto& Handler : GlobalSubscribers)
	{
		if (Handler.IsBound())
		{
			Handler.Execute(Event);
		}
	}
	
	// Broadcast to specific subscribers
	const TMap<FGuid, FSystemEventHandler>* Handlers = Subscribers.Find(Event.EventType);
	if (Handlers)
	{
		for (const auto& Pair : *Handlers)
		{
			if (Pair.Value.IsBound())
			{
				Pair.Value.Execute(Event);
			}
		}
	}
	
	UE_LOG(LogTemp, Verbose, TEXT("Processed event: %s from %s"),
		*Event.EventName, *Event.Sender);
}

void UMingSystemEventBus::AddToHistory(const FSystemEvent& Event)
{
	EventHistory.Add(Event);
	
	while (EventHistory.Num() > MaxHistorySize)
	{
		EventHistory.RemoveAt(0);
	}
}

void UMingSystemEventBus::OnProcessingTick()
{
	if (bQueueEnabled && PendingEvents.Num() > 0)
	{
		ProcessPendingEvents();
	}
}

bool UMingSystemEventBus::ShouldQueueEvent(const FSystemEvent& Event) const
{
	// Critical events should not be queued
	return Event.Priority != ESystemEventPriority::Critical;
}
