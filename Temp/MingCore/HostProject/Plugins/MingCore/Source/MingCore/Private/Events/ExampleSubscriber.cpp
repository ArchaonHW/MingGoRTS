#include "Events/ExampleSubscriber.h"
#include "Events/ExampleEvents.h"

FExampleTacticalSubscriber::FExampleTacticalSubscriber()
    : HandledEventCount(0)
{
}

TArray<FString> FExampleTacticalSubscriber::GetInterestedLayers() const
{
    TArray<FString> Layers;
    Layers.Add(TEXT("Tactical"));
    return Layers;
}

TArray<FName> FExampleTacticalSubscriber::GetInterestedEventTypes() const
{
    TArray<FName> EventTypes;
    // Empty array means subscribe to all event types from the Tactical layer
    return EventTypes;
}

void FExampleTacticalSubscriber::HandleEvent(TSharedPtr<IMingEvent> Event)
{
    if (!Event.IsValid())
    {
        return;
    }
    
    FName EventType = Event->GetEventType();
    
    if (EventType == FName("UnitMovement"))
    {
        HandleUnitMovement(Event);
    }
    else if (EventType == FName("Combat"))
    {
        HandleCombat(Event);
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("FExampleTacticalSubscriber: Received unhandled event type %s"), *EventType.ToString());
    }
    
    HandledEventCount++;
}

void FExampleTacticalSubscriber::HandleUnitMovement(TSharedPtr<IMingEvent> Event)
{
    UE_LOG(LogTemp, Log, TEXT("FExampleTacticalSubscriber: Handling unit movement event"));
    
    // In a real implementation, you would cast to FUnitMovementEvent and access the data
    // auto MovementEvent = StaticCastSharedPtr<FUnitMovementEvent>(Event);
    // if (MovementEvent.IsValid())
    // {
    //     int32 UnitId = MovementEvent->GetUnitId();
    //     FVector NewPos = MovementEvent->GetNewPosition();
    //     // Process movement...
    // }
}

void FExampleTacticalSubscriber::HandleCombat(TSharedPtr<IMingEvent> Event)
{
    UE_LOG(LogTemp, Log, TEXT("FExampleTacticalSubscriber: Handling combat event"));
    
    // In a real implementation, you would cast to FCombatEvent and access the data
    // auto CombatEvent = StaticCastSharedPtr<FCombatEvent>(Event);
    // if (CombatEvent.IsValid())
    // {
    //     int32 AttackerId = CombatEvent->GetAttackerId();
    //     int32 DefenderId = CombatEvent->GetDefenderId();
    //     float Damage = CombatEvent->GetDamage();
    //     // Process combat...
    // }
}
