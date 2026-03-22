#pragma once

#include "CoreMinimal.h"

/**
 * Base interface for all events in the MingGoRTS event system.
 * All custom events must inherit from this interface.
 */
class MINGCORE_API IMingEvent
{
public:
    virtual ~IMingEvent() = default;
    
    /**
     * Get the event type identifier.
     * Used for routing events to appropriate handlers.
     */
    virtual FName GetEventType() const = 0;
    
    /**
     * Get the priority of this event.
     * Higher priority events are processed first.
     */
    virtual int32 GetPriority() const { return 0; }
    
    /**
     * Check if this event should be broadcast to all subscribers.
     */
    virtual bool IsBroadcast() const { return false; }
    
    /**
     * Get the timestamp when this event was created.
     */
    virtual double GetTimestamp() const { return Timestamp; }
    
    /**
     * Mark this event as handled to stop propagation.
     */
    virtual void MarkHandled() { bHandled = true; }
    
    /**
     * Check if this event has been handled.
     */
    virtual bool IsHandled() const { return bHandled; }
    
    /**
     * Get the source layer that triggered this event.
     * Returns: "Strategic", "Tactical", "Personal", "Building", or "Core"
     */
    virtual FString GetSourceLayer() const = 0;
    
protected:
    double Timestamp = 0.0;
    bool bHandled = false;
    
    IMingEvent()
    {
        Timestamp = FPlatformTime::Seconds();
    }
};
