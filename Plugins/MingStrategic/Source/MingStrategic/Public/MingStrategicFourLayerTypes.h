#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingStrategicFourLayerTypes.generated.h"

// ?戲層X
UENUM(BlueprintType)
enum class EMingLayer : uint8
{
    None, UMETA(DisplayName = "None"),
    Personal, UMETA(DisplayName = "Personal"),
    Tactical, UMETA(DisplayName = "Tactical"),
    Strategic, UMETA(DisplayName = "Strategic"),
    Empire, UMETA(DisplayName = "Empire")
};

// 資?類X
UENUM(BlueprintType)
enum class EMingStrategicResourceType : uint8
{
    None, UMETA(DisplayName = "None"),
    Gold, UMETA(DisplayName = "Gold"),
    Food, UMETA(DisplayName = "Food"),
    Wood, UMETA(DisplayName = "Wood"),
    Stone, UMETA(DisplayName = "Stone"),
    Iron, UMETA(DisplayName = "Iron"),
    Population, UMETA(DisplayName = "Population"),
    Influence, UMETA(DisplayName = "Influence")
};

// ?戲事件結?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicFourLayerEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString EventID;

    UPROPERTY(BlueprintReadWrite)
    FString EventName;

    UPROPERTY(BlueprintReadWrite)
    EMingGameLayer SourceLayer;

    UPROPERTY(BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(BlueprintReadWrite)
    TMap<FString, FString> EventData;

    FMingStrategicFourLayerEvent()
        : SourceLayer(EMingGameLayer::None)
        , Timestamp(0.0f)
    {}
};

// ?略層XUSTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStrategicState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString FactionID;

    UPROPERTY(BlueprintReadWrite)
    FString FactionName;

    UPROPERTY(BlueprintReadWrite)
    int32 TerritoryCount;

    UPROPERTY(BlueprintReadWrite)
    float Power;

    UPROPERTY(BlueprintReadWrite)
    float Economy;

    FMingStrategicState()
        : TerritoryCount(0)
        , Power(50.0f)
        , Economy(50.0f)
    {}
};

// ?人層XUSTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingPersonalState
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

// 建?層XUSTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingBuildingState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString BuildingID;

    UPROPERTY(BlueprintReadWrite)
    FString BuildingName;

    UPROPERTY(BlueprintReadWrite)
    EMingBuildingType BuildingType;

    UPROPERTY(BlueprintReadWrite)
    int32 Level;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    UPROPERTY(BlueprintReadWrite)
    bool bIsActive;

    FMingBuildingState()
        : BuildingType(EMingBuildingType::CommandCenter)
        , Level(1)
        , Health(100.0f)
        , bIsActive(true)
    {}
};

// X層XUSTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingTacticalState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString UnitID;

    UPROPERTY(BlueprintReadWrite)
    FString UnitName;

    UPROPERTY(BlueprintReadWrite)
    int32 UnitCount;

    UPROPERTY(BlueprintReadWrite)
    FVector Position;

    UPROPERTY(BlueprintReadWrite)
    float Morale;

    FMingTacticalState()
        : UnitCount(1)
        , Position(FVector::ZeroVector)
        , Morale(100.0f)
    {}
};

// XUENUM(BlueprintType)
enum class EMingTacticalUnitState : uint8
{
    Idle, UMETA(DisplayName = "Idle"),
    Moving, UMETA(DisplayName = "Moving"),
    Attacking, UMETA(DisplayName = "Attacking"),
    Defending, UMETA(DisplayName = "Defending"),
    Retreating, UMETA(DisplayName = "Retreating"),
    Patrolling, UMETA(DisplayName = "Patrolling"),
    Engaged, UMETA(DisplayName = "Engaged"),
    Disabled, UMETA(DisplayName = "Disabled")
};

// X結?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalUnit
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString UnitID;

    UPROPERTY(BlueprintReadWrite)
    FString UnitName;

    UPROPERTY(BlueprintReadWrite)
    EMingTacticalUnitState UnitState;

    UPROPERTY(BlueprintReadWrite)
    FVector Position;

    UPROPERTY(BlueprintReadWrite)
    float Health;

    FMingStratTacticalUnit()
        : UnitState(EMingTacticalUnitState::Idle)
        , Position(FVector::ZeroVector)
        , Health(100.0f)
    {}
};

// X令結?
USTRUCT(BlueprintType)
struct MINGSTRATEGIC_API FMingStratTacticalOrder
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString OrderID;

    UPROPERTY(BlueprintReadWrite)
    FString OrderName;

    UPROPERTY(BlueprintReadWrite)
    EMingTacticalCommand CommandType;

    UPROPERTY(BlueprintReadWrite)
    FVector TargetPosition;

    UPROPERTY(BlueprintReadWrite)
    float Priority;

    FMingStratTacticalOrder()
        : CommandType(EMingTacticalCommand::None)
        , TargetPosition(FVector::ZeroVector)
        , Priority(1.0f)
    {}
};
};

