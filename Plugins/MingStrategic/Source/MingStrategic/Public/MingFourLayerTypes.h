#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingFourLayerTypes.generated.h"

// Forward declarations
UENUM(BlueprintType)
enum class EMingLayer : uint8;

// Layer type enum
UENUM(BlueprintType)
enum class EMingLayer : uint8
{
    None        UMETA(DisplayName = "None"),
    Personal    UMETA(DisplayName = "Personal"),
    Tactical    UMETA(DisplayName = "Tactical"),
    Strategic   UMETA(DisplayName = "Strategic"),
    Empire      UMETA(DisplayName = "Empire")
};

// Game event structure
USTRUCT(BlueprintType)
struct FMingGameEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString EventID;

    UPROPERTY(BlueprintReadWrite)
    FString EventName;

    UPROPERTY(BlueprintReadWrite)
    EMingLayer SourceLayer;

    UPROPERTY(BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FString> EventData;

    FMingGameEvent()
        : SourceLayer(EMingLayer::None)
        , Timestamp(0.0f)
    {}
};

// Game decision structure
USTRUCT(BlueprintType)
struct FMingGameDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString DecisionID;

    UPROPERTY(BlueprintReadWrite)
    FString DecisionName;

    UPROPERTY(BlueprintReadWrite)
    EMingLayer SourceLayer;

    UPROPERTY(BlueprintReadWrite)
    EMingLayer TargetLayer;

    UPROPERTY(BlueprintReadWrite)
    float Priority;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FString> DecisionData;

    FMingGameDecision()
        : SourceLayer(EMingLayer::None)
        , TargetLayer(EMingLayer::None)
        , Priority(1.0f)
    {}
};

// Layer conflict structure
USTRUCT(BlueprintType)
struct FMingLayerConflict
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString ConflictID;

    UPROPERTY(BlueprintReadWrite)
    EMingLayer LayerA;

    UPROPERTY(BlueprintReadWrite)
    EMingLayer LayerB;

    UPROPERTY(BlueprintReadWrite)
    FString ConflictDescription;

    UPROPERTY(BlueprintReadWrite)
    float Severity;

    FMingLayerConflict()
        : LayerA(EMingLayer::None)
        , LayerB(EMingLayer::None)
        , Severity(0.5f)
    {}
};

// Personal layer state
USTRUCT(BlueprintType)
struct FMingPersonalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString CharacterID;

    UPROPERTY(BlueprintReadWrite)
    FString CharacterName;

    UPROPERTY(BlueprintReadWrite)
    int32 Level;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    UPROPERTY(BlueprintReadWrite)
    float Experience;

    FMingPersonalState()
        : Level(1)
        , Health(100.0f)
        , Experience(0.0f)
    {}
};

// Building layer state
USTRUCT(BlueprintType)
struct FMingBuildingState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString BuildingID;

    UPROPERTY(BlueprintReadWrite)
    FString BuildingName;

    UPROPERTY(BlueprintReadWrite)
    int32 BuildingType;

    UPROPERTY(BlueprintReadWrite)
    int32 Level;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FMingBuildingState()
        : BuildingType(0)
        , Level(1)
        , Health(100.0f)
        , bIsActive(true)
    {}
};

// Strategic layer state
USTRUCT(BlueprintType)
struct FMingStrategicState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString StrategyID;

    UPROPERTY(BlueprintReadWrite)
    FString StrategyName;

    UPROPERTY(BlueprintReadWrite)
    int32 Priority;

    UPROPERTY(BlueprintReadWrite)
    float Progress;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FMingStrategicState()
        : Priority(1)
        , Progress(0.0f)
        , bIsActive(true)
    {}
};

// Tactical unit state enum
UENUM(BlueprintType)
enum class EMingTacticalUnitState : uint8
{
    Idle        UMETA(DisplayName = "Idle"),
    Moving      UMETA(DisplayName = "Moving"),
    Attacking   UMETA(DisplayName = "Attacking"),
    Defending   UMETA(DisplayName = "Defending"),
    Patrolling  UMETA(DisplayName = "Patrolling"),
    Retreating  UMETA(DisplayName = "Retreating"),
    Stunned     UMETA(DisplayName = "Stunned"),
    Dead        UMETA(DisplayName = "Dead")
};

// Strategic tactical unit
USTRUCT(BlueprintType)
struct FMingStratTacticalUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString UnitID;

    UPROPERTY(BlueprintReadWrite)
    FString UnitName;

    UPROPERTY(BlueprintReadWrite)
    int32 UnitType;

    UPROPERTY(BlueprintReadWrite)
    int32 Count;

    UPROPERTY(BlueprintReadWrite)
    FVector Position;

    UPROPERTY(BlueprintReadWrite)
    EMingTacticalUnitState State;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    UPROPERTY(BlueprintReadWrite)
    float Morale;

    FMingStratTacticalUnit()
        : UnitType(0)
        , Count(1)
        , Position(FVector::ZeroVector)
        , State(EMingTacticalUnitState::Idle)
        , Health(100.0f)
        , Morale(100.0f)
    {}
};

// Strategic tactical order
USTRUCT(BlueprintType)
struct FMingStratTacticalOrder
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString OrderID;

    UPROPERTY(BlueprintReadWrite)
    FString OrderName;

    UPROPERTY(BlueprintReadWrite)
    int32 OrderType;

    UPROPERTY(BlueprintReadWrite)
    TArray<FString> TargetUnitIDs;

    UPROPERTY(BlueprintReadWrite)
    FVector TargetPosition;

    UPROPERTY(BlueprintReadWrite)
    float Priority;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FMingStratTacticalOrder()
        : OrderType(0)
        , TargetPosition(FVector::ZeroVector)
        , Priority(1.0f)
        , bIsActive(true)
    {}
};

// Resource type enum
UENUM(BlueprintType)
enum class EMingResourceType : uint8
{
    None        UMETA(DisplayName = "None"),
    Gold        UMETA(DisplayName = "Gold"),
    Food        UMETA(DisplayName = "Food"),
    Wood        UMETA(DisplayName = "Wood"),
    Stone       UMETA(DisplayName = "Stone"),
    Iron        UMETA(DisplayName = "Iron"),
    Population  UMETA(DisplayName = "Population"),
    Influence   UMETA(DisplayName = "Influence")
};
