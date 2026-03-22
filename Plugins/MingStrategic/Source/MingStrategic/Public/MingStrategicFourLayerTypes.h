#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingFourLayerTypes.generated.h"

// 遊戲層級枚舉
UENUM(BlueprintType)
enum class EMingLayer : uint8
{
    None        UMETA(DisplayName = "None"),
    Personal    UMETA(DisplayName = "Personal"),
    Tactical    UMETA(DisplayName = "Tactical"),
    Strategic   UMETA(DisplayName = "Strategic"),
    Empire      UMETA(DisplayName = "Empire")
};

// 資源類型枚舉
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

// 遊戲事件結構
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

// 遊戲決策結構
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

// 層級衝突結構
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

// 個人層狀態
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

// 建築層狀態
USTRUCT(BlueprintType)
struct FMingBuildingState
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

// 戰術層狀態
USTRUCT(BlueprintType)
struct FMingTacticalState
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
