#include "LayerIntegration/MingFourLayerManager.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

UMingFourLayerManager::UMingFourLayerManager()
{
    CurrentState.CurrentLayer = EMingGameLayer::None;
    CurrentState.PreviousLayer = EMingGameLayer::None;
}

void UMingFourLayerManager::InitializeFourLayerSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Four Layer Strategy System"));
    
    // Initialize all layers
    InitializeStrategicLayer();
    InitializeTacticalLayer();
    InitializePersonalLayer();
    InitializeBuildingLayer();
    
    // Set default layer
    CurrentState.CurrentLayer = EMingGameLayer::Strategic;
    CurrentState.PreviousLayer = EMingGameLayer::None;
    
    // Initialize sync data
    CurrentSyncData.GlobalTime = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Four Layer System initialized. Default layer: Strategic"));
}

void UMingFourLayerManager::ShutdownFourLayerSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Four Layer System"));
    
    // Stop any ongoing transition
    if (CurrentState.bIsTransitioning)
    {
        CancelLayerTransition();
    }
    
    // Shutdown all layers
    ShutdownStrategicLayer();
    ShutdownTacticalLayer();
    ShutdownPersonalLayer();
    ShutdownBuildingLayer();
    
    // Clear state history
    StateHistory.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Four Layer System shutdown complete"));
}

// Layer Management
void UMingFourLayerManager::SwitchToLayer(EMingGameLayer TargetLayer, EMingLayerTransitionType TransitionType)
{
    if (TargetLayer == CurrentState.CurrentLayer)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already in layer %d"), (int32)TargetLayer);
        return;
    }
    
    if (!CanSwitchToLayer(TargetLayer))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot switch to layer %d from current layer %d"), 
            (int32)TargetLayer, (int32)CurrentState.CurrentLayer);
        return;
    }
    
    // Record current state before switching
    RecordStateHistory();
    
    // Start transition
    StartLayerTransition(TargetLayer, TransitionType);
}

void UMingFourLayerManager::QuickSwitchToLayer(EMingGameLayer TargetLayer)
{
    SwitchToLayer(TargetLayer, EMingLayerTransitionType::Instant);
}

bool UMingFourLayerManager::CanSwitchToLayer(EMingGameLayer TargetLayer) const
{
    if (TargetLayer == EMingGameLayer::None)
    {
        return false;
    }
    
    // Define allowed transitions
    switch (CurrentState.CurrentLayer)
    {
    case EMingGameLayer::None:
        return true; // Can switch to any layer from None
        
    case EMingGameLayer::Strategic:
        // From strategic, can go to tactical, building, or personal
        return TargetLayer == EMingGameLayer::Tactical || 
               TargetLayer == EMingGameLayer::Building || 
               TargetLayer == EMingGameLayer::Personal;
        
    case EMingGameLayer::Tactical:
        // From tactical, can go to strategic or personal
        return TargetLayer == EMingGameLayer::Strategic || 
               TargetLayer == EMingGameLayer::Personal;
        
    case EMingGameLayer::Personal:
        // From personal, can go to tactical or strategic
        return TargetLayer == EMingGameLayer::Tactical || 
               TargetLayer == EMingGameLayer::Strategic;
        
    case EMingGameLayer::Building:
        // From building, can go to strategic
        return TargetLayer == EMingGameLayer::Strategic;
        
    default:
        return false;
    }
}

void UMingFourLayerManager::AbortTransition()
{
    if (CurrentState.bIsTransitioning)
    {
        CancelLayerTransition();
        UE_LOG(LogTemp, Log, TEXT("Layer transition aborted"));
    }
}

// Layer transition implementation
void UMingFourLayerManager::StartLayerTransition(EMingGameLayer TargetLayer, EMingLayerTransitionType TransitionType)
{
    CurrentState.PreviousLayer = CurrentState.CurrentLayer;
    CurrentState.bIsTransitioning = true;
    CurrentState.TransitionType = TransitionType;
    CurrentState.TransitionProgress = 0.0f;
    
    // Set transition duration based on type
    switch (TransitionType)
    {
    case EMingLayerTransitionType::Instant:
        CurrentState.TransitionDuration = 0.0f;
        break;
    case EMingLayerTransitionType::Fade:
        CurrentState.TransitionDuration = 0.5f;
        break;
    case EMingLayerTransitionType::Slide:
        CurrentState.TransitionDuration = 0.8f;
        break;
    case EMingLayerTransitionType::Zoom:
        CurrentState.TransitionDuration = 1.0f;
        break;
    case EMingLayerTransitionType::Cinematic:
        CurrentState.TransitionDuration = 2.0f;
        break;
    default:
        CurrentState.TransitionDuration = 0.5f;
        break;
    }
    
    // Deactivate current layer
    DeactivateLayer(CurrentState.CurrentLayer);
    
    // Broadcast transition started
    OnLayerTransitionStarted.Broadcast(TargetLayer);
    
    // If instant transition, complete immediately
    if (TransitionType == EMingLayerTransitionType::Instant || CurrentState.TransitionDuration <= 0.0f)
    {
        CurrentState.CurrentLayer = TargetLayer;
        CompleteLayerTransition();
    }
    else
    {
        // Start transition ticker
        TransitionTicker = FTickerDelegate::CreateUObject(this, &UMingFourLayerManager::UpdateLayerTransition);
        TransitionTickerHandle = FTSTicker::GetCoreTicker().AddTicker(TransitionTicker);
    }
}

bool UMingFourLayerManager::UpdateLayerTransition(float DeltaTime)
{
    if (!CurrentState.bIsTransitioning)
    {
        return false;
    }
    
    CurrentState.TransitionProgress += DeltaTime / CurrentState.TransitionDuration;
    
    if (CurrentState.TransitionProgress >= 1.0f)
    {
        CurrentState.TransitionProgress = 1.0f;
        CompleteLayerTransition();
        return false; // Stop ticker
    }
    
    return true; // Continue ticker
}

void UMingFourLayerManager::CompleteLayerTransition()
{
    CurrentState.bIsTransitioning = false;
    
    // Activate new layer
    ActivateLayer(CurrentState.CurrentLayer);
    
    // Sync data to all layers
    SyncLayerData();
    
    // Broadcast events
    OnLayerTransitionCompleted.Broadcast(CurrentState.CurrentLayer, true);
    NotifyLayerChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Layer transition completed. Now in layer: %d"), (int32)CurrentState.CurrentLayer);
    
    // Remove ticker
    if (TransitionTickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TransitionTickerHandle);
        TransitionTickerHandle.Reset();
    }
}

void UMingFourLayerManager::CancelLayerTransition()
{
    CurrentState.bIsTransitioning = false;
    CurrentState.TransitionProgress = 0.0f;
    
    // Revert to previous layer if needed
    if (CurrentState.PreviousLayer != EMingGameLayer::None)
    {
        CurrentState.CurrentLayer = CurrentState.PreviousLayer;
        ActivateLayer(CurrentState.CurrentLayer);
    }
    
    // Remove ticker
    if (TransitionTickerHandle.IsValid())
    {
        FTSTicker::GetCoreTicker().RemoveTicker(TransitionTickerHandle);
        TransitionTickerHandle.Reset();
    }
    
    OnLayerTransitionCompleted.Broadcast(CurrentState.CurrentLayer, false);
}

// Strategic Layer Interface
void UMingFourLayerManager::MakeStrategicDecision(const FMingStrategicDecision& Decision)
{
    if (!ValidateStrategicDecision(Decision))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid strategic decision"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Making strategic decision: %s"), *Decision.DecisionName);
    
    // Execute decision logic
    switch (Decision.ActionType)
    {
    case EMingStrategicAction::DeclareWar:
        UE_LOG(LogTemp, Log, TEXT("Declaring war on %s"), *Decision.TargetFaction);
        break;
        
    case EMingStrategicAction::FormAlliance:
        UE_LOG(LogTemp, Log, TEXT("Forming alliance with %s"), *Decision.TargetFaction);
        break;
        
    case EMingStrategicAction::TradeAgreement:
        UE_LOG(LogTemp, Log, TEXT("Establishing trade agreement with %s"), *Decision.TargetFaction);
        break;
        
    case EMingStrategicAction::MoveTroops:
        UE_LOG(LogTemp, Log, TEXT("Moving %d troops"), Decision.TroopRequirement);
        break;
        
    default:
        UE_LOG(LogTemp, Log, TEXT("Executing strategic action type: %d"), (int32)Decision.ActionType);
        break;
    }
    
    // Broadcast event
    OnStrategicDecisionMade.Broadcast(Decision);
    
    // Sync to tactical layer if needed
    if (Decision.TroopRequirement > 0)
    {
        PropagateStrategicToTactical(Decision);
    }
}

void UMingFourLayerManager::InitiateWar(const FString& TargetFaction, const TArray<FString>& TargetRegions)
{
    FMingStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.DecisionName = FString::Printf(TEXT("Declare War on %s"), *TargetFaction);
    Decision.ActionType = EMingStrategicAction::DeclareWar;
    Decision.TargetFaction = TargetFaction;
    Decision.InvolvedRegions = TargetRegions;
    
    MakeStrategicDecision(Decision);
}

void UMingFourLayerManager::FormAlliance(const FString& AllyFaction, int32 DurationDays)
{
    FMingStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.DecisionName = FString::Printf(TEXT("Alliance with %s"), *AllyFaction);
    Decision.ActionType = EMingStrategicAction::FormAlliance;
    Decision.TargetFaction = AllyFaction;
    
    MakeStrategicDecision(Decision);
}

void UMingFourLayerManager::TradeResources(const FString& TradePartner, TMap<FString, int32> ResourcesToTrade)
{
    FMingStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.DecisionName = FString::Printf(TEXT("Trade with %s"), *TradePartner);
    Decision.ActionType = EMingStrategicAction::ResourceTrading;
    Decision.TargetFaction = TradePartner;
    
    // Calculate total resource value
    int32 TotalValue = 0;
    for (const auto& Pair : ResourcesToTrade)
    {
        TotalValue += Pair.Value;
    }
    Decision.ResourceCost = TotalValue;
    
    MakeStrategicDecision(Decision);
}

void UMingFourLayerManager::DeployTroops(const FString& FromRegion, const FString& ToRegion, int32 TroopCount)
{
    FMingStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.DecisionName = FString::Printf(TEXT("Deploy troops from %s to %s"), *FromRegion, *ToRegion);
    Decision.ActionType = EMingStrategicAction::MoveTroops;
    Decision.TroopRequirement = TroopCount;
    Decision.InvolvedRegions = { FromRegion, ToRegion };
    
    MakeStrategicDecision(Decision);
}

void UMingFourLayerManager::InitiateDiplomaticMission(const FString& TargetFaction, EMingStrategicAction MissionType)
{
    FMingStrategicDecision Decision;
    Decision.DecisionID = FGuid::NewGuid().ToString();
    Decision.DecisionName = FString::Printf(TEXT("Diplomatic mission to %s"), *TargetFaction);
    Decision.ActionType = MissionType;
    Decision.TargetFaction = TargetFaction;
    
    MakeStrategicDecision(Decision);
}

// Tactical Layer Interface
void UMingFourLayerManager::IssueTacticalOrder(const FMingTacticalOrder& Order)
{
    if (!ValidateTacticalOrder(Order))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid tactical order"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Issuing tactical order: %s"), *Order.OrderID);
    
    // Broadcast event
    OnTacticalOrderIssued.Broadcast(Order);
    
    // Auto-execute if enabled
    if (Order.bAutoExecute)
    {
        UE_LOG(LogTemp, Log, TEXT("Auto-executing order"));
    }
}

void UMingFourLayerManager::CommandAttack(const TArray<int32>& UnitIDs, const FVector& TargetLocation, int32 TargetEnemyID)
{
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::Attack;
    Order.UnitIDs = UnitIDs;
    Order.TargetLocation = TargetLocation;
    Order.TargetUnitID = TargetEnemyID;
    Order.Priority = 2.0f;
    
    IssueTacticalOrder(Order);
}

void UMingFourLayerManager::CommandDefend(const TArray<int32>& UnitIDs, const FVector& DefendLocation)
{
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::Defend;
    Order.UnitIDs = UnitIDs;
    Order.TargetLocation = DefendLocation;
    Order.Priority = 1.5f;
    
    IssueTacticalOrder(Order);
}

void UMingFourLayerManager::CommandRetreat(const TArray<int32>& UnitIDs)
{
    FMingTacticalOrder Order;
    Order.OrderID = FGuid::NewGuid().ToString();
    Order.CommandType = EMingTacticalCommand::Retreat;
    Order.UnitIDs = UnitIDs;
    Order.Priority = 3.0f;
    
    IssueTacticalOrder(Order);
}

void UMingFourLayerManager::SetFormation(const TArray<int32>& UnitIDs, const FString& FormationType, float Width)
{
    UE_LOG(LogTemp, Log, TEXT("Setting formation: %s for %d units"), *FormationType, UnitIDs.Num());
    // Formation logic would be implemented here
}

void UMingFourLayerManager::ActivateTacticalAbility(const FString& AbilityID, const TArray<int32>& UnitIDs)
{
    UE_LOG(LogTemp, Log, TEXT("Activating ability: %s for %d units"), *AbilityID, UnitIDs.Num());
    // Ability activation logic would be implemented here
}

// Personal Layer Interface
void UMingFourLayerManager::TriggerPersonalAction(const FMingPersonalAction& Action)
{
    if (!ValidatePersonalAction(Action))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid personal action"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Triggering personal action: %s"), *Action.ActionName);
    
    // Broadcast event
    OnPersonalActionTriggered.Broadcast(Action);
    
    // If combat action, may need to switch layers
    if (Action.bIsCombat)
    {
        UE_LOG(LogTemp, Log, TEXT("Combat action - preparing tactical layer"));
    }
}

void UMingFourLayerManager::StartCharacterDialogue(const FString& CharacterID, const FString& DialogueID)
{
    FMingPersonalAction Action;
    Action.ActionID = FGuid::NewGuid().ToString();
    Action.ActionName = TEXT("Dialogue");
    Action.CharacterID = CharacterID;
    Action.DialogueID = DialogueID;
    Action.bIsDiplomatic = true;
    
    TriggerPersonalAction(Action);
}

void UMingFourLayerManager::InitiateCombatEncounter(const FString& EnemyCharacterID, const FString& LocationID)
{
    FMingPersonalAction Action;
    Action.ActionID = FGuid::NewGuid().ToString();
    Action.ActionName = TEXT("Combat Encounter");
    Action.TargetCharacterID = EnemyCharacterID;
    Action.bIsCombat = true;
    
    TriggerPersonalAction(Action);
    
    // Switch to tactical for combat
    SwitchToLayer(EMingGameLayer::Tactical, EMingLayerTransitionType::Cinematic);
}

void UMingFourLayerManager::EnterDiplomaticNegotiation(const FString& TargetCharacterID, const FString& Context)
{
    FMingPersonalAction Action;
    Action.ActionID = FGuid::NewGuid().ToString();
    Action.ActionName = TEXT("Diplomatic Negotiation");
    Action.TargetCharacterID = TargetCharacterID;
    Action.bIsDiplomatic = true;
    
    TriggerPersonalAction(Action);
}

void UMingFourLayerManager::MakePersonalChoice(const FString& ChoiceID, const FString& ConsequenceID)
{
    UE_LOG(LogTemp, Log, TEXT("Making personal choice: %s with consequence: %s"), *ChoiceID, *ConsequenceID);
    // Choice logic would be implemented here
}

void UMingFourLayerManager::ReturnToTacticalFromPersonal()
{
    if (CurrentState.CurrentLayer == EMingGameLayer::Personal)
    {
        SwitchToLayer(EMingGameLayer::Tactical, EMingLayerTransitionType::Fade);
    }
}

// Building Layer Interface
void UMingFourLayerManager::ExecuteBuildingCommand(const FMingBuildingCommand& Command)
{
    if (!ValidateBuildingCommand(Command))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid building command"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing building command: %s"), *Command.CommandID);
    
    // Broadcast event
    OnBuildingCommandExecuted.Broadcast(Command);
    
    // Sync to strategic layer
    PropagateBuildingToStrategic(Command);
}

void UMingFourLayerManager::QueueBuildingConstruction(const FString& BuildingType, FVector Location, int32 Level)
{
    FMingBuildingCommand Command;
    Command.CommandID = FGuid::NewGuid().ToString();
    Command.BuildingType = BuildingType;
    Command.BuildLocation = Location;
    Command.BuildingLevel = Level;
    
    ExecuteBuildingCommand(Command);
}

void UMingFourLayerManager::UpgradeBuilding(const FString& BuildingID, int32 TargetLevel)
{
    FMingBuildingCommand Command;
    Command.CommandID = FGuid::NewGuid().ToString();
    Command.BuildingType = BuildingID;
    Command.BuildingLevel = TargetLevel;
    Command.bIsUpgrade = true;
    
    ExecuteBuildingCommand(Command);
}

void UMingFourLayerManager::DemolishBuilding(const FString& BuildingID)
{
    FMingBuildingCommand Command;
    Command.CommandID = FGuid::NewGuid().ToString();
    Command.BuildingType = BuildingID;
    Command.bIsDemolish = true;
    
    ExecuteBuildingCommand(Command);
}

void UMingFourLayerManager::SetProductionPriority(const FString& BuildingID, const FString& ProductionType, int32 Priority)
{
    UE_LOG(LogTemp, Log, TEXT("Setting production priority for %s: %s = %d"), *BuildingID, *ProductionType, Priority);
    // Production priority logic would be implemented here
}

void UMingFourLayerManager::AllocateWorkers(const FString& BuildingID, int32 WorkerCount)
{
    UE_LOG(LogTemp, Log, TEXT("Allocating %d workers to %s"), WorkerCount, *BuildingID);
    // Worker allocation logic would be implemented here
}

// Cross-Layer Integration
void UMingFourLayerManager::SyncLayerData()
{
    UE_LOG(LogTemp, Log, TEXT("Synchronizing layer data"));
    
    // Update sync data from all layers
    // This would gather data from each layer's managers
    
    CurrentSyncData.GlobalTime += 1.0f; // Increment time
    
    // Broadcast sync event
    OnLayerDataSynced.Broadcast(CurrentSyncData);
    
    // Notify all layers
    SyncToAllLayers();
}

void UMingFourLayerManager::PropagateStrategicToTactical(const FMingStrategicDecision& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("Propagating strategic decision to tactical layer"));
    
    // Convert strategic decisions to tactical implications
    if (Decision.TroopRequirement > 0)
    {
        // Update available units in tactical layer
        CurrentSyncData.TacticalUnits.Add(TEXT("AvailableUnits"), Decision.TroopRequirement);
    }
}

void UMingFourLayerManager::PropagateTacticalToStrategic(const FMingTacticalOrder& Order, bool bVictory)
{
    UE_LOG(LogTemp, Log, TEXT("Propagating tactical result to strategic layer. Victory: %s"), bVictory ? TEXT("Yes") : TEXT("No"));
    
    // Update strategic state based on tactical outcome
    if (bVictory)
    {
        // Gain territory, resources, etc.
    }
    else
    {
        // Lose resources, retreat, etc.
    }
}

void UMingFourLayerManager::PropagatePersonalToTactical(const FMingPersonalAction& Action)
{
    UE_LOG(LogTemp, Log, TEXT("Propagating personal action to tactical layer"));
    
    if (Action.bIsCombat)
    {
        // Set up combat encounter in tactical layer
    }
}

void UMingFourLayerManager::PropagateBuildingToStrategic(const FMingBuildingCommand& Command)
{
    UE_LOG(LogTemp, Log, TEXT("Propagating building command to strategic layer"));
    
    // Update strategic resources based on building changes
    if (!Command.bIsDemolish)
    {
        // New or upgraded building affects resource production
    }
}

// Layer State Management
void UMingFourLayerManager::SaveLayerState()
{
    StateHistory.Add(CurrentState);
    
    // Limit history size
    if (StateHistory.Num() > MaxStateHistory)
    {
        StateHistory.RemoveAt(0);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Layer state saved. History size: %d"), StateHistory.Num());
}

void UMingFourLayerManager::RestoreLayerState(const FMingLayerState& State)
{
    CurrentState = State;
    UE_LOG(LogTemp, Log, TEXT("Layer state restored to layer: %d"), (int32)CurrentState.CurrentLayer);
}

void UMingFourLayerManager::ClearLayerContext()
{
    CurrentState.LayerContext.Empty();
    UE_LOG(LogTemp, Log, TEXT("Layer context cleared"));
}

void UMingFourLayerManager::SetLayerContext(const FString& Context)
{
    CurrentState.LayerContext = Context;
    UE_LOG(LogTemp, Log, TEXT("Layer context set: %s"), *Context);
}

// Quick Access Functions
void UMingFourLayerManager::EnterStrategicMap()
{
    SwitchToLayer(EMingGameLayer::Strategic, EMingLayerTransitionType::Zoom);
}

void UMingFourLayerManager::EnterTacticalBattle(const FString& BattleID)
{
    SetLayerContext(FString::Printf(TEXT("Battle:%s"), *BattleID));
    SwitchToLayer(EMingGameLayer::Tactical, EMingLayerTransitionType::Cinematic);
}

void UMingFourLayerManager::EnterPersonalMode(const FString& CharacterID)
{
    SetLayerContext(FString::Printf(TEXT("Character:%s"), *CharacterID));
    SwitchToLayer(EMingGameLayer::Personal, EMingLayerTransitionType::Fade);
}

void UMingFourLayerManager::EnterBuildingMode(const FString& BaseID)
{
    SetLayerContext(FString::Printf(TEXT("Base:%s"), *BaseID));
    SwitchToLayer(EMingGameLayer::Building, EMingLayerTransitionType::Slide);
}

// Internal Layer Management
void UMingFourLayerManager::InitializeStrategicLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Strategic Layer"));
    // Strategic layer initialization would connect to MingStrategic plugin
}

void UMingFourLayerManager::InitializeTacticalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Tactical Layer"));
    // Tactical layer initialization would connect to MingTactical plugin
}

void UMingFourLayerManager::InitializePersonalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Personal Layer"));
    // Personal layer initialization would connect to MingPersonal plugin
}

void UMingFourLayerManager::InitializeBuildingLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Building Layer"));
    // Building layer initialization would connect to MingBuilding plugin
}

void UMingFourLayerManager::ShutdownStrategicLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Strategic Layer"));
}

void UMingFourLayerManager::ShutdownTacticalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Tactical Layer"));
}

void UMingFourLayerManager::ShutdownPersonalLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Personal Layer"));
}

void UMingFourLayerManager::ShutdownBuildingLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Building Layer"));
}

void UMingFourLayerManager::ActivateLayer(EMingGameLayer Layer)
{
    UE_LOG(LogTemp, Log, TEXT("Activating layer: %d"), (int32)Layer);
    // Layer-specific activation logic
}

void UMingFourLayerManager::DeactivateLayer(EMingGameLayer Layer)
{
    UE_LOG(LogTemp, Log, TEXT("Deactivating layer: %d"), (int32)Layer);
    // Layer-specific deactivation logic
}

// Validation Functions
bool UMingFourLayerManager::ValidateStrategicDecision(const FMingStrategicDecision& Decision) const
{
    return !Decision.DecisionID.IsEmpty() && Decision.ActionType != EMingStrategicAction::None;
}

bool UMingFourLayerManager::ValidateTacticalOrder(const FMingTacticalOrder& Order) const
{
    return !Order.OrderID.IsEmpty() && Order.CommandType != EMingTacticalCommand::None && Order.UnitIDs.Num() > 0;
}

bool UMingFourLayerManager::ValidatePersonalAction(const FMingPersonalAction& Action) const
{
    return !Action.ActionID.IsEmpty() && !Action.CharacterID.IsEmpty();
}

bool UMingFourLayerManager::ValidateBuildingCommand(const FMingBuildingCommand& Command) const
{
    return !Command.CommandID.IsEmpty() && !Command.BuildingType.IsEmpty();
}

void UMingFourLayerManager::RecordStateHistory()
{
    SaveLayerState();
}

void UMingFourLayerManager::NotifyLayerChanged()
{
    OnLayerChanged.Broadcast(CurrentState.CurrentLayer, CurrentState.PreviousLayer);
}

void UMingFourLayerManager::SyncToAllLayers()
{
    // This would notify all layer managers to sync their data
    UE_LOG(LogTemp, Log, TEXT("Syncing to all layers"));
}

UWorld* UMingFourLayerManager::GetWorld() const
{
    if (WorldContext.IsValid())
    {
        return WorldContext.Get();
    }
    
    if (GEngine)
    {
        for (const FWorldContext& Context : GEngine->GetWorldContexts())
        {
            if (Context.World() && (Context.WorldType == EWorldType::Game || Context.WorldType == EWorldType::PIE))
            {
                return Context.World();
            }
        }
    }
    
    return nullptr;
}
