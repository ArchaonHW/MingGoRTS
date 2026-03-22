#include "Events/MingEventBus.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformTime.h"
#include "Logging/LogMacros.h"

FMingEventBus& FMingEventBus::Get()
{
    static FMingEventBus Instance;
    return Instance;
}

void FMingEventBus::Subscribe(TSharedPtr<IMingEventSubscriber> Subscriber)
{
    if (!Subscriber.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FMingEventBus::Subscribe: Invalid subscriber"));
        return;
    }
    
    FScopeLock Lock(&SubscribersLock);
    
    // Check if subscriber already exists
    FName SubscriberId = Subscriber->GetSubscriberId();
    for (const auto& ExistingSubscriber : Subscribers)
    {
        if (ExistingSubscriber.IsValid() && ExistingSubscriber->GetSubscriberId() == SubscriberId)
        {
            UE_LOG(LogTemp, Warning, TEXT("FMingEventBus::Subscribe: Subscriber %s already registered"), *SubscriberId.ToString());
            return;
        }
    }
    
    Subscribers.Add(Subscriber);
    SortSubscribersByPriority();
    
    UE_LOG(LogTemp, Log, TEXT("FMingEventBus::Subscribe: Added subscriber %s"), *SubscriberId.ToString());
}

void FMingEventBus::Unsubscribe(FName SubscriberId)
{
    FScopeLock Lock(&SubscribersLock);
    
    int32 RemovedCount = Subscribers.RemoveAll([&](const TSharedPtr<IMingEventSubscriber>& Subscriber)
    {
        return Subscriber.IsValid() && Subscriber->GetSubscriberId() == SubscriberId;
    });
    
    if (RemovedCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("FMingEventBus::Unsubscribe: Removed subscriber %s"), *SubscriberId.ToString());
    }
}

void FMingEventBus::Publish(TSharedPtr<IMingEvent> Event)
{
    if (!Event.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("FMingEventBus::Publish: Invalid event"));
        return;
    }
    
    if (bAsyncProcessingEnabled)
    {
        FScopeLock Lock(&EventQueueLock);
        PendingEvents.Add(Event);
    }
    else
    {
        // Process immediately if not batching
        if (!bBatchingEnabled)
        {
            auto InterestedSubscribers = FindInterestedSubscribers(Event);
            for (const auto& Subscriber : InterestedSubscribers)
            {
                if (Event->IsHandled())
                {
                    break;
                }
                
                if (Subscriber.IsValid())
                {
                    Subscriber->HandleEvent(Event);
                }
            }
        }
        else
        {
            PendingEvents.Add(Event);
        }
    }
}

void FMingEventBus::ProcessPendingEvents()
{
    TArray<TSharedPtr<IMingEvent>> EventsToProcess;
    
    {
        FScopeLock Lock(&EventQueueLock);
        EventsToProcess = MoveTemp(PendingEvents);
        PendingEvents.Empty();
    }
    
    // Sort events by priority (higher priority first)
    EventsToProcess.Sort([](const TSharedPtr<IMingEvent>& A, const TSharedPtr<IMingEvent>& B)
    {
        if (!A.IsValid() || !B.IsValid())
        {
            return false;
        }
        return A->GetPriority() > B->GetPriority();
    });
    
    for (const auto& Event : EventsToProcess)
    {
        if (!Event.IsValid() || Event->IsHandled())
        {
            continue;
        }
        
        auto InterestedSubscribers = FindInterestedSubscribers(Event);
        for (const auto& Subscriber : InterestedSubscribers)
        {
            if (Event->IsHandled())
            {
                break;
            }
            
            if (Subscriber.IsValid())
            {
                Subscriber->HandleEvent(Event);
            }
        }
    }
}

void FMingEventBus::ClearPendingEvents()
{
    FScopeLock Lock(&EventQueueLock);
    PendingEvents.Empty();
    UE_LOG(LogTemp, Log, TEXT("FMingEventBus::ClearPendingEvents: Cleared all pending events"));
}

void FMingEventBus::Shutdown()
{
    ClearPendingEvents();
    
    {
        FScopeLock Lock(&SubscribersLock);
        Subscribers.Empty();
    }
    
    UE_LOG(LogTemp, Log, TEXT("FMingEventBus::Shutdown: Event bus shutdown complete"));
}

TArray<TSharedPtr<IMingEventSubscriber>> FMingEventBus::FindInterestedSubscribers(TSharedPtr<IMingEvent> Event)
{
    TArray<TSharedPtr<IMingEventSubscriber>> InterestedSubscribers;
    
    if (!Event.IsValid())
    {
        return InterestedSubscribers;
    }
    
    FScopeLock Lock(&SubscribersLock);
    
    for (const auto& Subscriber : Subscribers)
    {
        if (Subscriber.IsValid() && IsSubscriberInterested(Subscriber, Event))
        {
            InterestedSubscribers.Add(Subscriber);
        }
    }
    
    return InterestedSubscribers;
}

void FMingEventBus::SortSubscribersByPriority()
{
    Subscribers.Sort([](const TSharedPtr<IMingEventSubscriber>& A, const TSharedPtr<IMingEventSubscriber>& B)
    {
        if (!A.IsValid() || !B.IsValid())
        {
            return false;
        }
        return A->GetPriority() > B->GetPriority();
    });
}

bool FMingEventBus::IsSubscriberInterested(TSharedPtr<IMingEventSubscriber> Subscriber, TSharedPtr<IMingEvent> Event)
{
    if (!Subscriber.IsValid() || !Event.IsValid())
    {
        return false;
    }
    
    // Check if subscriber is interested in this layer
    TArray<FString> InterestedLayers = Subscriber->GetInterestedLayers();
    FString SourceLayer = Event->GetSourceLayer();
    
    if (InterestedLayers.Num() > 0 && !InterestedLayers.Contains(SourceLayer))
    {
        return false;
    }
    
    // Check if subscriber is interested in this event type
    TArray<FName> InterestedEventTypes = Subscriber->GetInterestedEventTypes();
    if (InterestedEventTypes.Num() > 0 && !InterestedEventTypes.Contains(Event->GetEventType()))
    {
        return false;
    }
    
    return true;
}
