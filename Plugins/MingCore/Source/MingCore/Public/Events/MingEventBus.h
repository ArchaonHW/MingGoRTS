#pragma once

#include "CoreMinimal.h"
#include "MingEvent.h"
#include "MingEventSubscriber.h"

/**
 * Central event bus for routing and distributing events across all layers.
 * This is the core communication mechanism for the modular plugin architecture.
 * 
 * Usage:
 *   // Subscribe to events
 *   FMingEventBus::Get().Subscribe(MySubscriber);
 *   
 *   // Publish an event
 *   auto Event = MakeShared<FMyCustomEvent>(Args...);
 *   FMingEventBus::Get().Publish(Event);
 *   
 *   // Unsubscribe when done
 *   FMingEventBus::Get().Unsubscribe(MySubscriber->GetSubscriberId());
 */
class MINGCORE_API FMingEventBus
{
public:
    /** Get the singleton instance of the event bus. */
    static FMingEventBus& Get();
    
    /** 
     * Subscribe a subscriber to receive events.
     * @param Subscriber - The subscriber to add
     */
    void Subscribe(TSharedPtr<IMingEventSubscriber> Subscriber);
    
    /**
     * Unsubscribe a subscriber by its ID.
     * @param SubscriberId - The ID of the subscriber to remove
     */
    void Unsubscribe(FName SubscriberId);
    
    /**
     * Publish an event to all interested subscribers.
     * @param Event - The event to publish
     */
    void Publish(TSharedPtr<IMingEvent> Event);
    
    /**
     * Publish an event with automatic type detection.
     * Convenience method for publishing typed events.
     * @tparam T - The event type
     * @param Event - The event to publish
     */
    template<typename T>
    void PublishTyped(TSharedPtr<T> Event)
    {
        Publish(StaticCastSharedPtr<IMingEvent>(Event));
    }
    
    /**
     * Process all pending events in the queue.
     * Should be called once per frame (e.g., in Tick).
     */
    void ProcessPendingEvents();
    
    /**
     * Enable or disable event batching for performance.
     * When enabled, events are collected and processed together.
     */
    void SetBatchingEnabled(bool bEnabled) { bBatchingEnabled = bEnabled; }
    
    /**
     * Check if batching is enabled.
     */
    bool IsBatchingEnabled() const { return bBatchingEnabled; }
    
    /**
     * Get the number of pending events in the queue.
     */
    int32 GetPendingEventCount() const { return PendingEvents.Num(); }
    
    /**
     * Clear all pending events without processing them.
     */
    void ClearPendingEvents();
    
    /**
     * Enable or disable async event processing.
     * When enabled, events can be published from any thread.
     */
    void SetAsyncProcessingEnabled(bool bEnabled) { bAsyncProcessingEnabled = bEnabled; }
    
    /**
     * Check if async processing is enabled.
     */
    bool IsAsyncProcessingEnabled() const { return bAsyncProcessingEnabled; }
    
    /**
     * Shutdown the event bus and clean up all subscribers.
     */
    void Shutdown();
    
private:
    FMingEventBus() = default;
    ~FMingEventBus() = default;
    
    FMingEventBus(const FMingEventBus&) = delete;
    FMingEventBus& operator=(const FMingEventBus&) = delete;
    
    /** List of all registered subscribers. */
    TArray<TSharedPtr<IMingEventSubscriber>> Subscribers;
    
    /** Queue of pending events to be processed. */
    TArray<TSharedPtr<IMingEvent>> PendingEvents;
    
    /** Mutex for thread safety when async processing is enabled. */
    mutable FCriticalSection EventQueueLock;
    mutable FCriticalSection SubscribersLock;
    
    /** Whether event batching is enabled. */
    bool bBatchingEnabled = true;
    
    /** Whether async event processing is enabled. */
    bool bAsyncProcessingEnabled = false;
    
    /**
     * Find subscribers interested in this event.
     * @param Event - The event to check
     * @return Array of interested subscribers sorted by priority
     */
    TArray<TSharedPtr<IMingEventSubscriber>> FindInterestedSubscribers(TSharedPtr<IMingEvent> Event};
    
    /**
     * Sort subscribers by priority (higher priority first).
     */
    void SortSubscribersByPriority();
    
    /**
     * Check if a subscriber is interested in a specific event.
     */
    bool IsSubscriberInterested(TSharedPtr<IMingEventSubscriber> Subscriber, TSharedPtr<IMingEvent> Event};
};

