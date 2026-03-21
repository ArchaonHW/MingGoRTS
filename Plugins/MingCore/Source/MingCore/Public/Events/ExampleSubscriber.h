#pragma once

#include "CoreMinimal.h"
#include "MingEventSubscriber.h"
#include "MingEvent.h"

/**
 * Example subscriber that demonstrates how to implement IMingEventSubscriber.
 * This subscriber listens to all tactical layer events.
 */
class MINGCORE_API FExampleTacticalSubscriber : public IMingEventSubscriber
{
public:
    FExampleTacticalSubscriber();
    
    // IMingEventSubscriber interface
    virtual FName GetSubscriberId() const override { return FName("ExampleTactical"); }
    virtual TArray<FString> GetInterestedLayers() const override;
    virtual TArray<FName> GetInterestedEventTypes() const override;
    virtual void HandleEvent(TSharedPtr<IMingEvent> Event) override;
    virtual int32 GetPriority() const override { return 10; }
    
    // Statistics for testing
    int32 GetHandledEventCount() const { return HandledEventCount; }
    void ResetStatistics() { HandledEventCount = 0; }
    
private:
    int32 HandledEventCount = 0;
    
    void HandleUnitMovement(TSharedPtr<IMingEvent> Event);
    void HandleCombat(TSharedPtr<IMingEvent> Event);
};
