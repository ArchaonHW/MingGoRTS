#pragma once

#include "CoreMinimal.h"
#include "MingEvent.h"

/**
 * Delegate for handling events.
 * This is a multicast delegate that can bind multiple handlers.
 */
DECLARE_MULTICAST_DELEGATE_OneParam(FMingEventDelegate, TSharedPtr<IMingEvent>);

/**
 * Interface for event subscribers.
 * Any class that wants to receive events must implement this interface.
 */
class MINGCORE_API IMingEventSubscriber
{
public:
    virtual ~IMingEventSubscriber() = default;
    
    /**
     * Get the subscriber ID.
     */
    virtual FName GetSubscriberId() const = 0;
    
    /**
     * Get the layers this subscriber is interested in.
     * Returns array of layer names: "Strategic", "Tactical", "Personal", "Building", "Core"
     */
    virtual TArray<FString> GetInterestedLayers() const = 0;
    
    /**
     * Get the event types this subscriber wants to receive.
     * Empty array means subscribe to all events.
     */
    virtual TArray<FName> GetInterestedEventTypes() const { return TArray<FName>(); }
    
    /**
     * Handle an incoming event.
     * @param Event - The event to handle
     */
    virtual void HandleEvent(TSharedPtr<IMingEvent> Event) = 0;
    
    /**
     * Get the priority of this subscriber.
     * Higher priority subscribers receive events first.
     */
    virtual int32 GetPriority() const { return 0; }
};
