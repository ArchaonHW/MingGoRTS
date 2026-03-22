#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/Callback.h"
#include "Containers/Ticker.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingFourLayerManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLayerChanged, EMingGameLayer, NewLayer, EMingGameLayer, PreviousLayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLayerTransitionStarted, EMingGameLayer, TargetLayer);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLayerTransitionCompleted, EMingGameLayer, TargetLayer, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStrategicDecisionMade, const FMingStrategicDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTacticalOrderIssued, const FMingTacticalOrder&, Order);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPersonalActionTriggered, const FMingPersonalAction&, Action);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingCommandExecuted, const FMingBuildingCommand&, Command);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLayerDataSynced, const FMingLayerSyncData&, SyncData);

/**
 * Four Layer Strategy Integration Manager
 * Coordinates between Strategic, Tactical, Personal, and Building layers
 * Handles layer switching, data synchronization, and cross-layer communication
 */
UCLASS(ClassGroup = (Core), Blueprintable)
class MINGCORE_API UMingFourLayerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingFourLayerManager();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Four Layer Manager")
    void InitializeFourLayerSystem();

    UFUNCTION(BlueprintCallable, Category = "Four Layer Manager")
    void ShutdownFourLayerSystem();

    // Layer Management
    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    void SwitchToLayer(EMingGameLayer TargetLayer, EMingLayerTransitionType TransitionType = EMingLayerTransitionType::Fade);

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    void QuickSwitchToLayer(EMingGameLayer TargetLayer);

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    bool CanSwitchToLayer(EMingGameLayer TargetLayer) const;

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    EMingGameLayer GetCurrentLayer() const { return CurrentState.CurrentLayer; }

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    EMingGameLayer GetPreviousLayer() const { return CurrentState.PreviousLayer; }

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    bool IsInLayer(EMingGameLayer Layer) const { return CurrentState.CurrentLayer == Layer; }

    UFUNCTION(BlueprintPure, Category = "Layer Management")
    bool IsTransitioning() const { return CurrentState.bIsTransitioning; }

    UFUNCTION(BlueprintPure, Category = "Layer Management")
    float GetTransitionProgress() const { return CurrentState.TransitionProgress; }

    UFUNCTION(BlueprintCallable, Category = "Layer Management")
    void AbortTransition();

    // Strategic Layer Interface
    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void MakeStrategicDecision(const FMingStrategicDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void InitiateWar(const FString& TargetFaction, const TArray<FString>& TargetRegions);

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void FormAlliance(const FString& AllyFaction, int32 DurationDays);

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void TradeResources(const FString& TradePartner, TMap<FString, int32> ResourcesToTrade);

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void DeployTroops(const FString& FromRegion, const FString& ToRegion, int32 TroopCount);

    UFUNCTION(BlueprintCallable, Category = "Strategic Layer")
    void InitiateDiplomaticMission(const FString& TargetFaction, EMingStrategicAction MissionType);

    // Tactical Layer Interface
    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void IssueTacticalOrder(const FMingTacticalOrder& Order);

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void CommandAttack(const TArray<int32>& UnitIDs, const FVector& TargetLocation, int32 TargetEnemyID = -1);

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void CommandDefend(const TArray<int32>& UnitIDs, const FVector& DefendLocation);

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void CommandRetreat(const TArray<int32>& UnitIDs);

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void SetFormation(const TArray<int32>& UnitIDs, const FString& FormationType, float Width);

    UFUNCTION(BlueprintCallable, Category = "Tactical Layer")
    void ActivateTacticalAbility(const FString& AbilityID, const TArray<int32>& UnitIDs);

    // Personal Layer Interface
    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void TriggerPersonalAction(const FMingPersonalAction& Action);

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void StartCharacterDialogue(const FString& CharacterID, const FString& DialogueID);

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void InitiateCombatEncounter(const FString& EnemyCharacterID, const FString& LocationID);

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void EnterDiplomaticNegotiation(const FString& TargetCharacterID, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void MakePersonalChoice(const FString& ChoiceID, const FString& ConsequenceID);

    UFUNCTION(BlueprintCallable, Category = "Personal Layer")
    void ReturnToTacticalFromPersonal();

    // Building Layer Interface
    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void ExecuteBuildingCommand(const FMingBuildingCommand& Command);

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void QueueBuildingConstruction(const FString& BuildingType, FVector Location, int32 Level = 1);

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void UpgradeBuilding(const FString& BuildingID, int32 TargetLevel);

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void DemolishBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void SetProductionPriority(const FString& BuildingID, const FString& ProductionType, int32 Priority);

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void AllocateWorkers(const FString& BuildingID, int32 WorkerCount);

    // Cross-Layer Integration
    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    void SyncLayerData();

    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    void PropagateStrategicToTactical(const FMingStrategicDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    void PropagateTacticalToStrategic(const FMingTacticalOrder& Order, bool bVictory);

    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    void PropagatePersonalToTactical(const FMingPersonalAction& Action);

    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    void PropagateBuildingToStrategic(const FMingBuildingCommand& Command);

    UFUNCTION(BlueprintCallable, Category = "Cross-Layer")
    FMingLayerSyncData GetCurrentSyncData() const { return CurrentSyncData; }

    // Layer State Management
    UFUNCTION(BlueprintCallable, Category = "Layer State")
    void SaveLayerState();

    UFUNCTION(BlueprintCallable, Category = "Layer State")
    void RestoreLayerState(const FMingLayerState& State);

    UFUNCTION(BlueprintCallable, Category = "Layer State")
    void ClearLayerContext();

    UFUNCTION(BlueprintCallable, Category = "Layer State")
    void SetLayerContext(const FString& Context);

    UFUNCTION(BlueprintPure, Category = "Layer State")
    FString GetLayerContext() const { return CurrentState.LayerContext; }

    // Quick Access Functions
    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void EnterStrategicMap();

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void EnterTacticalBattle(const FString& BattleID);

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void EnterPersonalMode(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Quick Access")
    void EnterBuildingMode(const FString& BaseID);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLayerChanged OnLayerChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLayerTransitionStarted OnLayerTransitionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLayerTransitionCompleted OnLayerTransitionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnStrategicDecisionMade OnStrategicDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnTacticalOrderIssued OnTacticalOrderIssued;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPersonalActionTriggered OnPersonalActionTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingCommandExecuted OnBuildingCommandExecuted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnLayerDataSynced OnLayerDataSynced;

protected:
    UPROPERTY()
    FMingLayerState CurrentState;

    UPROPERTY()
    FMingLayerSyncData CurrentSyncData;

    UPROPERTY()
    TArray<FMingLayerState> StateHistory;

    UPROPERTY()
    int32 MaxStateHistory = 10;

    // Layer transition handling
    FTickerDelegate TransitionTicker;

    FDelegateHandle TransitionTickerHandle;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal functions
    void StartLayerTransition(EMingGameLayer TargetLayer, EMingLayerTransitionType TransitionType);
    bool UpdateLayerTransition(float DeltaTime);
    void CompleteLayerTransition();
    void CancelLayerTransition();

    void InitializeStrategicLayer();
    void InitializeTacticalLayer();
    void InitializePersonalLayer();
    void InitializeBuildingLayer();

    void ShutdownStrategicLayer();
    void ShutdownTacticalLayer();
    void ShutdownPersonalLayer();
    void ShutdownBuildingLayer();

    void ActivateLayer(EMingGameLayer Layer);
    void DeactivateLayer(EMingGameLayer Layer);

    bool ValidateStrategicDecision(const FMingStrategicDecision& Decision) const;
    bool ValidateTacticalOrder(const FMingTacticalOrder& Order) const;
    bool ValidatePersonalAction(const FMingPersonalAction& Action) const;
    bool ValidateBuildingCommand(const FMingBuildingCommand& Command) const;

    void RecordStateHistory();
    void NotifyLayerChanged();
    void SyncToAllLayers();

    UWorld* GetWorld() const;
};
