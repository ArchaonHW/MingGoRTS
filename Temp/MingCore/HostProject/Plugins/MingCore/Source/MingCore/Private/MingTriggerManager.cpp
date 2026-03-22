#include "MingTriggerManager.h"
#include "Engine/World.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"
#include "MingResourceSystem.h"
#include "MingBuildingActor.h"

UMingTriggerManager::UMingTriggerManager()
{
}

UMingTriggerManager* UMingTriggerManager::Get(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // Use static map for world-specific managers
    static TMap<TWeakObjectPtr<UWorld>, UMingTriggerManager*> WorldManagers;

    TWeakObjectPtr<UWorld> WorldKey = World;
    UMingTriggerManager** ExistingManager = WorldManagers.Find(WorldKey);

    if (ExistingManager && *ExistingManager)
    {
        return *ExistingManager;
    }

    // Create new manager
    UMingTriggerManager* NewManager = NewObject<UMingTriggerManager>(World);
    NewManager->CachedWorld = World;
    NewManager->EventManager = UMingEventManager::Get(WorldContextObject);

    // Subscribe to game events
    if (NewManager->EventManager.IsValid())
    {
        FOnGameEvent EventCallback;
        EventCallback.BindUObject(NewManager, &UMingTriggerManager::OnGameEvent);
        NewManager->EventManager->SubscribeToAllEvents(EventCallback);
    }

    WorldManagers.Add(WorldKey, NewManager);
    return NewManager;
}

FString UMingTriggerManager::RegisterTrigger(const FMingGameTrigger& Trigger)
{
    FMingGameTrigger NewTrigger = Trigger;
    
    // Generate ID if not provided
    if (NewTrigger.TriggerID.IsEmpty())
    {
        NewTrigger.TriggerID = GenerateTriggerID();
    }

    RegisteredTriggers.Add(NewTrigger.TriggerID, NewTrigger);

    UE_LOG(LogTemp, Log, TEXT("Registered trigger: %s (ID: %s)"), 
           *NewTrigger.TriggerName, *NewTrigger.TriggerID);

    return NewTrigger.TriggerID;
}

void UMingTriggerManager::UnregisterTrigger(const FString& TriggerID)
{
    if (RegisteredTriggers.Contains(TriggerID))
    {
        RegisteredTriggers.Remove(TriggerID);
        UE_LOG(LogTemp, Log, TEXT("Unregistered trigger: %s"), *TriggerID);
    }
}

void UMingTriggerManager::ActivateTrigger(const FString& TriggerID)
{
    FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    if (Trigger)
    {
        Trigger->bIsActive = true;
        UE_LOG(LogTemp, Log, TEXT("Activated trigger: %s"), *Trigger->TriggerName);
    }
}

void UMingTriggerManager::DeactivateTrigger(const FString& TriggerID)
{
    FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    if (Trigger)
    {
        Trigger->bIsActive = false;
        UE_LOG(LogTemp, Log, TEXT("Deactivated trigger: %s"), *Trigger->TriggerName);
    }
}

void UMingTriggerManager::ResetTrigger(const FString& TriggerID)
{
    FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    if (Trigger)
    {
        Trigger->bTriggered = false;
        Trigger->TriggerCount = 0;
        Trigger->LastTriggerTime = -1.0f;
        UE_LOG(LogTemp, Log, TEXT("Reset trigger: %s"), *Trigger->TriggerName);
    }
}

bool UMingTriggerManager::IsTriggerActive(const FString& TriggerID) const
{
    const FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    return Trigger ? Trigger->bIsActive : false;
}

bool UMingTriggerManager::HasTriggerFired(const FString& TriggerID) const
{
    const FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    return Trigger ? Trigger->bTriggered : false;
}

int32 UMingTriggerManager::GetTriggerCount(const FString& TriggerID) const
{
    const FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    return Trigger ? Trigger->TriggerCount : 0;
}

TArray<FMingGameTrigger> UMingTriggerManager::GetAllTriggers() const
{
    TArray<FMingGameTrigger> Result;
    for (const auto& Pair : RegisteredTriggers)
    {
        Result.Add(Pair.Value);
    }
    return Result;
}

FMingGameTrigger UMingTriggerManager::GetTrigger(const FString& TriggerID) const
{
    const FMingGameTrigger* Trigger = RegisteredTriggers.Find(TriggerID);
    return Trigger ? *Trigger : FMingGameTrigger();
}

void UMingTriggerManager::UpdateTriggers(float DeltaTime)
{
    float CurrentTime = CachedWorld.IsValid() ? CachedWorld->GetTimeSeconds() : 0.0f;

    for (auto& Pair : RegisteredTriggers)
    {
        FMingGameTrigger& Trigger = Pair.Value;

        // Skip if not active
        if (!Trigger.bIsActive)
        {
            continue;
        }

        // Skip if one-time only and already triggered
        if (Trigger.bOneTimeOnly && Trigger.bTriggered)
        {
            continue;
        }

        // Check cooldown
        if (Trigger.LastTriggerTime > 0.0f && 
            (CurrentTime - Trigger.LastTriggerTime) < Trigger.Cooldown)
        {
            continue;
        }

        // Check max trigger count
        if (Trigger.MaxTriggerCount > 0 && Trigger.TriggerCount >= Trigger.MaxTriggerCount)
        {
            continue;
        }

        // Evaluate trigger conditions
        if (EvaluateTrigger(Trigger))
        {
            // Mark as triggered
            Trigger.bTriggered = true;
            Trigger.TriggerCount++;
            Trigger.LastTriggerTime = CurrentTime;

            // Execute actions
            ExecuteActions(Trigger);

            // Broadcast
            OnTriggerActivated.Broadcast(Trigger);

            UE_LOG(LogTemp, Log, TEXT("Trigger activated: %s"), *Trigger.TriggerName);

            // Deactivate if not repeatable and one-time
            if (!Trigger.bRepeatable && Trigger.bOneTimeOnly)
            {
                Trigger.bIsActive = false;
            }
        }
    }
}

void UMingTriggerManager::OnGameEvent(const FMingGameEvent& Event)
{
    // Event-driven trigger evaluation
    for (auto& Pair : RegisteredTriggers)
    {
        FMingGameTrigger& Trigger = Pair.Value;

        if (!Trigger.bIsActive)
        {
            continue;
        }

        // Check if any condition depends on this event type
        for (const FMingTriggerCondition& Condition : Trigger.Conditions)
        {
            if (Condition.ConditionType == EMingTriggerConditionType::EventOccurred &&
                Condition.EventType == Event.EventType)
            {
                // Re-evaluate this trigger on next update
                break;
            }
        }
    }
}

FString UMingTriggerManager::CreateVictoryTrigger(const FString& TriggerName, const TArray<FMingTriggerCondition>& Conditions)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;
    Trigger.Conditions = Conditions;
    Trigger.bOneTimeOnly = true;
    Trigger.bRepeatable = false;

    // Add victory action
    FMingTriggerAction Action;
    Action.ActionType = TEXT("Victory");
    Action.Message = TEXT("Mission Accomplished!");
    Trigger.Actions.Add(Action);

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateDefeatTrigger(const FString& TriggerName, const TArray<FMingTriggerCondition>& Conditions)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;
    Trigger.Conditions = Conditions;
    Trigger.bOneTimeOnly = true;
    Trigger.bRepeatable = false;

    FMingTriggerAction Action;
    Action.ActionType = TEXT("Defeat");
    Action.Message = TEXT("Mission Failed!");
    Trigger.Actions.Add(Action);

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateResourceGoalTrigger(const FString& TriggerName, EMingResourceType Resource, int32 TargetAmount)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;
    
    FMingTriggerCondition Condition;
    Condition.ConditionType = EMingTriggerConditionType::ResourceThreshold;
    Condition.ResourceType = Resource;
    Condition.Comparison = EMingTriggerComparison::GreaterOrEqual;
    Condition.IntValue = TargetAmount;
    Trigger.Conditions.Add(Condition);

    Trigger.bOneTimeOnly = true;

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateUnitCountTrigger(const FString& TriggerName, int32 MinUnitCount)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;

    FMingTriggerCondition Condition;
    Condition.ConditionType = EMingTriggerConditionType::UnitCount;
    Condition.Comparison = EMingTriggerComparison::GreaterOrEqual;
    Condition.IntValue = MinUnitCount;
    Trigger.Conditions.Add(Condition);

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateBuildingTrigger(const FString& TriggerName, EMingBuildingType BuildingType)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;

    FMingTriggerCondition Condition;
    Condition.ConditionType = EMingTriggerConditionType::BuildingConstructed;
    Condition.StringValue = UEnum::GetValueAsString(BuildingType);
    Trigger.Conditions.Add(Condition);

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateTimeLimitTrigger(const FString& TriggerName, float TimeLimitSeconds)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;

    FMingTriggerCondition Condition;
    Condition.ConditionType = EMingTriggerConditionType::TimeElapsed;
    Condition.Comparison = EMingTriggerComparison::GreaterOrEqual;
    Condition.FloatValue = TimeLimitSeconds;
    Trigger.Conditions.Add(Condition);

    return RegisterTrigger(Trigger);
}

FString UMingTriggerManager::CreateAreaTrigger(const FString& TriggerName, const FVector& Location, float Radius, int32 RequiredUnits)
{
    FMingGameTrigger Trigger;
    Trigger.TriggerName = TriggerName;

    FMingTriggerCondition Condition;
    Condition.ConditionType = EMingTriggerConditionType::AreaEntered;
    Condition.TargetLocation = Location;
    Condition.Radius = Radius;
    Condition.IntValue = RequiredUnits;
    Condition.Comparison = EMingTriggerComparison::GreaterOrEqual;
    Trigger.Conditions.Add(Condition);

    return RegisterTrigger(Trigger);
}

bool UMingTriggerManager::EvaluateTrigger(const FMingGameTrigger& Trigger)
{
    // All conditions must be met (AND logic by default)
    for (const FMingTriggerCondition& Condition : Trigger.Conditions)
    {
        if (!EvaluateConditionInternal(Condition))
        {
            return false;
        }
    }

    return true;
}

bool UMingTriggerManager::EvaluateConditionInternal(const FMingTriggerCondition& Condition)
{
    switch (Condition.ConditionType)
    {
    case EMingTriggerConditionType::CompoundAND:
        return EvaluateCompoundAND(Condition);

    case EMingTriggerConditionType::CompoundOR:
        return EvaluateCompoundOR(Condition);

    case EMingTriggerConditionType::ResourceThreshold:
        return EvaluateResourceThreshold(Condition);

    case EMingTriggerConditionType::UnitCount:
        return EvaluateUnitCount(Condition);

    case EMingTriggerConditionType::BuildingConstructed:
        return EvaluateBuildingConstructed(Condition);

    case EMingTriggerConditionType::TimeElapsed:
        return EvaluateTimeElapsed(Condition);

    case EMingTriggerConditionType::AreaEntered:
        return EvaluateAreaEntered(Condition);

    case EMingTriggerConditionType::EventOccurred:
        if (EventManager.IsValid())
        {
            return EventManager->HasEventOccurred(Condition.EventType, 1.0f);
        }
        return false;

    default:
        return false;
    }
}

bool UMingTriggerManager::EvaluateCompoundAND(const FMingTriggerCondition& Condition)
{
    for (const FMingTriggerCondition& SubCondition : Condition.SubConditions)
    {
        if (!EvaluateConditionInternal(SubCondition))
        {
            return false;
        }
    }
    return Condition.SubConditions.Num() > 0;
}

bool UMingTriggerManager::EvaluateCompoundOR(const FMingTriggerCondition& Condition)
{
    for (const FMingTriggerCondition& SubCondition : Condition.SubConditions)
    {
        if (EvaluateConditionInternal(SubCondition))
        {
            return true;
        }
    }
    return false;
}

bool UMingTriggerManager::EvaluateResourceThreshold(const FMingTriggerCondition& Condition)
{
    // This would need integration with the resource system
    // For now, placeholder implementation
    return false;
}

bool UMingTriggerManager::EvaluateUnitCount(const FMingTriggerCondition& Condition)
{
    if (!CachedWorld.IsValid())
    {
        return false;
    }

    // Count units (this is a simplified implementation)
    int32 UnitCount = 0;
    
    // Would iterate through all tactical units in the world
    // Placeholder: return based on condition
    return CompareValues(UnitCount, Condition.IntValue, Condition.Comparison);
}

bool UMingTriggerManager::EvaluateBuildingConstructed(const FMingTriggerCondition& Condition)
{
    // Check if specific building type has been constructed
    if (EventManager.IsValid())
    {
        return EventManager->HasEventOccurred(EMingGameEventType::BuildingConstructed, -1.0f);
    }
    return false;
}

bool UMingTriggerManager::EvaluateTimeElapsed(const FMingTriggerCondition& Condition)
{
    if (!CachedWorld.IsValid())
    {
        return false;
    }

    AGameStateBase* GameState = CachedWorld->GetGameState<AGameStateBase>();
    if (!GameState)
    {
        return false;
    }

    float ElapsedTime = GameState->GetServerWorldTimeSeconds();
    return CompareValues(ElapsedTime, Condition.FloatValue, Condition.Comparison);
}

bool UMingTriggerManager::EvaluateAreaEntered(const FMingTriggerCondition& Condition)
{
    // Check if required number of units are in the specified area
    // This would need spatial queries
    return false;
}

bool UMingTriggerManager::EvaluateCondition(const FMingTriggerCondition& Condition, UObject* WorldContext)
{
    UMingTriggerManager* Manager = Get(WorldContext);
    if (Manager)
    {
        return Manager->EvaluateConditionInternal(Condition);
    }
    return false;
}

void UMingTriggerManager::ExecuteActions(const FMingGameTrigger& Trigger)
{
    for (const FMingTriggerAction& Action : Trigger.Actions)
    {
        ExecuteAction(Action);
    }
}

void UMingTriggerManager::ExecuteAction(const FMingTriggerAction& Action)
{
    UE_LOG(LogTemp, Log, TEXT("Executing action: %s"), *Action.ActionType);

    if (Action.ActionType == TEXT("Spawn"))
    {
        // Spawn actor at location
        if (CachedWorld.IsValid() && Action.SpawnClass)
        {
            FActorSpawnParameters Params;
            CachedWorld->SpawnActor<AActor>(Action.SpawnClass, Action.SpawnLocation, FRotator::ZeroRotator, Params);
        }
    }
    else if (Action.ActionType == TEXT("Victory"))
    {
        UE_LOG(LogTemp, Log, TEXT("Victory! %s"), *Action.Message);
        // Trigger victory condition
    }
    else if (Action.ActionType == TEXT("Defeat"))
    {
        UE_LOG(LogTemp, Log, TEXT("Defeat! %s"), *Action.Message);
        // Trigger defeat condition
    }
    else if (Action.ActionType == TEXT("Message"))
    {
        UE_LOG(LogTemp, Log, TEXT("Message: %s"), *Action.Message);
        // Display message to player
    }
}

bool UMingTriggerManager::CompareValues(int32 Value1, int32 Value2, EMingTriggerComparison Comparison)
{
    switch (Comparison)
    {
    case EMingTriggerComparison::Equal: return Value1 == Value2;
    case EMingTriggerComparison::GreaterThan: return Value1 > Value2;
    case EMingTriggerComparison::LessThan: return Value1 < Value2;
    case EMingTriggerComparison::GreaterOrEqual: return Value1 >= Value2;
    case EMingTriggerComparison::LessOrEqual: return Value1 <= Value2;
    case EMingTriggerComparison::NotEqual: return Value1 != Value2;
    default: return false;
    }
}

bool UMingTriggerManager::CompareValues(float Value1, float Value2, EMingTriggerComparison Comparison)
{
    const float Tolerance = 0.001f;

    switch (Comparison)
    {
    case EMingTriggerComparison::Equal: return FMath::IsNearlyEqual(Value1, Value2, Tolerance);
    case EMingTriggerComparison::GreaterThan: return Value1 > (Value2 + Tolerance);
    case EMingTriggerComparison::LessThan: return Value1 < (Value2 - Tolerance);
    case EMingTriggerComparison::GreaterOrEqual: return Value1 >= (Value2 - Tolerance);
    case EMingTriggerComparison::LessOrEqual: return Value1 <= (Value2 + Tolerance);
    case EMingTriggerComparison::NotEqual: return !FMath::IsNearlyEqual(Value1, Value2, Tolerance);
    default: return false;
    }
}

void UMingTriggerManager::ClearAllTriggers()
{
    RegisteredTriggers.Empty();
    UE_LOG(LogTemp, Log, TEXT("Cleared all triggers"));
}

int32 UMingTriggerManager::GetActiveTriggerCount() const
{
    int32 Count = 0;
    for (const auto& Pair : RegisteredTriggers)
    {
        if (Pair.Value.bIsActive)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingTriggerManager::GetTotalTriggerCount() const
{
    return RegisteredTriggers.Num();
}

FString UMingTriggerManager::ExportTriggersToJson()
{
    // Simple JSON export
    FString Json = TEXT("{\n");
    Json += TEXT("  \"triggers\": [\n");

    int32 Index = 0;
    for (const auto& Pair : RegisteredTriggers)
    {
        const FMingGameTrigger& Trigger = Pair.Value;
        Json += TEXT("    {\n");
        Json += FString::Printf(TEXT("      \"id\": \"%s\",\n"), *Trigger.TriggerID);
        Json += FString::Printf(TEXT("      \"name\": \"%s\",\n"), *Trigger.TriggerName);
        Json += FString::Printf(TEXT("      \"active\": %s,\n"), Trigger.bIsActive ? TEXT("true") : TEXT("false"));
        Json += FString::Printf(TEXT("      \"triggered\": %s\n"), Trigger.bTriggered ? TEXT("true") : TEXT("false"));
        Json += TEXT("    }");
        
        if (++Index < RegisteredTriggers.Num())
        {
            Json += TEXT(",");
        }
        Json += TEXT("\n");
    }

    Json += TEXT("  ]\n");
    Json += TEXT("}\n");

    return Json;
}

void UMingTriggerManager::ImportTriggersFromJson(const FString& JsonString)
{
    // This would parse JSON and recreate triggers
    UE_LOG(LogTemp, Log, TEXT("Importing triggers from JSON"));
}

FString UMingTriggerManager::GenerateTriggerID()
{
    return FGuid::NewGuid().ToString();
}
