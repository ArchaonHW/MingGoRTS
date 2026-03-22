#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFourLayerTypes.generated.h"

UENUM(BlueprintType)
enum class EMingGameLayer : uint8
{
    None UMETA(DisplayName = "None"),
    Strategic UMETA(DisplayName = "Strategic Layer"),
    Tactical UMETA(DisplayName = "Tactical Layer"),
    Personal UMETA(DisplayName = "Personal Layer"),
    Building UMETA(DisplayName = "Building Layer")
};

UENUM(BlueprintType)
enum class EMingLayerTransitionType : uint8
{
    Instant UMETA(DisplayName = "Instant"),
    Fade UMETA(DisplayName = "Fade"),
    Slide UMETA(DisplayName = "Slide"),
    Zoom UMETA(DisplayName = "Zoom"),
    Cinematic UMETA(DisplayName = "Cinematic")
};

UENUM(BlueprintType)
enum class EMingStrategicAction : uint8
{
    None UMETA(DisplayName = "None"),
    DeclareWar UMETA(DisplayName = "Declare War"),
    FormAlliance UMETA(DisplayName = "Form Alliance"),
    TradeAgreement UMETA(DisplayName = "Trade Agreement"),
    MoveTroops UMETA(DisplayName = "Move Troops"),
    DiplomaticMission UMETA(DisplayName = "Diplomatic Mission"),
    Espionage UMETA(DisplayName = "Espionage"),
    ResourceTrading UMETA(DisplayName = "Resource Trading")
};

UENUM(BlueprintType)
enum class EMingTacticalCommand : uint8
{
    None UMETA(DisplayName = "None"),
    Attack UMETA(DisplayName = "Attack"),
    Defend UMETA(DisplayName = "Defend"),
    Retreat UMETA(DisplayName = "Retreat"),
    Flank UMETA(DisplayName = "Flank"),
    HoldPosition UMETA(DisplayName = "Hold Position"),
    Charge UMETA(DisplayName = "Charge"),
    Ambush UMETA(DisplayName = "Ambush"),
    Support UMETA(DisplayName = "Support"),
    Recon UMETA(DisplayName = "Reconnaissance")
};

USTRUCT(BlueprintType)
struct FMingLayerState
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    EMingGameLayer CurrentLayer = EMingGameLayer::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    EMingGameLayer PreviousLayer = EMingGameLayer::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    float TransitionProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    bool bIsTransitioning = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    EMingLayerTransitionType TransitionType = EMingLayerTransitionType::Fade;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    float TransitionDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    FString LayerContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layer State")
    TMap<FString, FString> LayerData;

    FMingLayerState()
        : CurrentLayer(EMingGameLayer::None)
        , PreviousLayer(EMingGameLayer::None)
        , TransitionProgress(0.0f)
        , bIsTransitioning(false)
        , TransitionType(EMingLayerTransitionType::Fade)
        , TransitionDuration(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingStrategicDecision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    FString DecisionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    FString DecisionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    EMingStrategicAction ActionType = EMingStrategicAction::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    FString TargetFaction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    TArray<FString> InvolvedRegions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    int32 ResourceCost = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    int32 TroopRequirement = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    float ExpectedOutcome = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strategic")
    TArray<FString> Prerequisites;

    FMingStrategicDecision()
        : ActionType(EMingStrategicAction::None)
        , ResourceCost(0)
        , TroopRequirement(0)
        , ExpectedOutcome(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FMingTacticalOrder
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    FString OrderID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    EMingTacticalCommand CommandType = EMingTacticalCommand::None;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    TArray<int32> UnitIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    int32 TargetUnitID = -1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    float Priority = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    bool bAutoExecute = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical")
    float FormationWidth = 100.0f;

    FMingTacticalOrder()
        : CommandType(EMingTacticalCommand::None)
        , TargetUnitID(-1)
        , Priority(1.0f)
        , bAutoExecute(false)
        , FormationWidth(100.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingPersonalAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    FString ActionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    FString ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    FString TargetCharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    FString DialogueID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    bool bIsCombat = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    bool bIsDiplomatic = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Personal")
    TArray<FString> RelatedDecisions;

    FMingPersonalAction()
        : bIsCombat(false)
        , bIsDiplomatic(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingBuildingCommand
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString CommandID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString BuildingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FVector BuildLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 BuildingLevel = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    bool bIsUpgrade = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    bool bIsDemolish = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TArray<FString> ResourceRequirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float BuildTime = 60.0f;

    FMingBuildingCommand()
        : BuildingLevel(1)
        , bIsUpgrade(false)
        , bIsDemolish(false)
        , BuildTime(60.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingLayerSyncData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    TMap<FString, int32> StrategicResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    TMap<FString, int32> TacticalUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    FString PersonalCharacterState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    TMap<FString, FString> BuildingStates;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    float GlobalTime = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync")
    FString CurrentMissionID;

    FMingLayerSyncData()
        : GlobalTime(0.0f)
    {}
};
