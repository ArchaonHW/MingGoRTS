#pragma once

#include "CoreMinimal.h"
#include "MingEvent.h"

/**
 * Example event for unit movement in the tactical layer.
 * This demonstrates how to create custom events for the event system.
 */
class MINGCORE_API FUnitMovementEvent : public IMingEvent
{
public:
    FUnitMovementEvent(int32 InUnitId, const FVector& InNewPosition, const FVector& InOldPosition)
        : UnitId(InUnitId)
        , NewPosition(InNewPosition)
        , OldPosition(InOldPosition)
    {
    }
    
    virtual FName GetEventType() const override
    {
        return FName("UnitMovement");
    }
    
    virtual FString GetSourceLayer() const override
    {
        return FString("Tactical");
    }
    
    virtual int32 GetPriority() const override
    {
        return 10; // High priority for movement events
    }
    
    int32 GetUnitId() const { return UnitId; }
    FVector GetNewPosition() const { return NewPosition; }
    FVector GetOldPosition() const { return OldPosition; }
    
private:
    int32 UnitId;
    FVector NewPosition;
    FVector OldPosition;
};

/**
 * Example event for combat engagement.
 */
class MINGCORE_API FCombatEvent : public IMingEvent
{
public:
    FCombatEvent(int32 InAttackerId, int32 InDefenderId, float InDamage)
        : AttackerId(InAttackerId)
        , DefenderId(InDefenderId)
        , Damage(InDamage)
    {
    }
    
    virtual FName GetEventType() const override
    {
        return FName("Combat");
    }
    
    virtual FString GetSourceLayer() const override
    {
        return FString("Tactical");
    }
    
    virtual int32 GetPriority() const override
    {
        return 20; // Higher priority than movement
    }
    
    int32 GetAttackerId() const { return AttackerId; }
    int32 GetDefenderId() const { return DefenderId; }
    float GetDamage() const { return Damage; }
    
private:
    int32 AttackerId;
    int32 DefenderId;
    float Damage;
};

/**
 * Example event for resource changes in the building layer.
 */
class MINGCORE_API FResourceChangedEvent : public IMingEvent
{
public:
    FResourceChangedEvent(FName InResourceType, float InOldAmount, float InNewAmount)
        : ResourceType(InResourceType)
        , OldAmount(InOldAmount)
        , NewAmount(InNewAmount)
    {
    }
    
    virtual FName GetEventType() const override
    {
        return FName("ResourceChanged");
    }
    
    virtual FString GetSourceLayer() const override
    {
        return FString("Building");
    }
    
    virtual int32 GetPriority() const override
    {
        return 5; // Lower priority than combat/movement
    }
    
    FName GetResourceType() const { return ResourceType; }
    float GetOldAmount() const { return OldAmount; }
    float GetNewAmount() const { return NewAmount; }
    float GetChange() const { return NewAmount - OldAmount; }
    
private:
    FName ResourceType;
    float OldAmount;
    float NewAmount;
};

/**
 * Example event for strategic decisions.
 */
class MINGCORE_API FStrategicDecisionEvent : public IMingEvent
{
public:
    FStrategicDecisionEvent(FName InDecisionId, FName InChoice)
        : DecisionId(InDecisionId)
        , Choice(InChoice)
    {
    }
    
    virtual FName GetEventType() const override
    {
        return FName("StrategicDecision");
    }
    
    virtual FString GetSourceLayer() const override
    {
        return FString("Strategic");
    }
    
    virtual int32 GetPriority() const override
    {
        return 30; // Highest priority for strategic decisions
    }
    
    virtual bool IsBroadcast() const override
    {
        return true; // Broadcast to all layers
    }
    
    FName GetDecisionId() const { return DecisionId; }
    FName GetChoice() const { return Choice; }
    
private:
    FName DecisionId;
    FName Choice;
};
