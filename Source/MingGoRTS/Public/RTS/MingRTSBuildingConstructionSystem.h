// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSBuildingConstructionSystem.generated.h"

// 建筑类型
UENUM(BlueprintType)
enum class ERTSBuildingType: uint8 {
    // 基础设施
    Headquarters    UMETA(DisplayName = "Headquarters"),
    Barracks        UMETA(DisplayName = "Barracks"),
    Stable          UMETA(DisplayName = "Stable"),
    Workshop        UMETA(DisplayName = "Workshop"),
    
    // 资源建筑
    Farm            UMETA(DisplayName = "Farm"),
    LumberMill      UMETA(DisplayName = "Lumber Mill"),
    Mine            UMETA(DisplayName = "Mine"),
    Market          UMETA(DisplayName = "Market"),
    
    // 防御建筑
    Wall            UMETA(DisplayName = "Wall"),
    Gate            UMETA(DisplayName = "Gate"),
    Tower           UMETA(DisplayName = "Tower"),
    Fortress        UMETA(DisplayName = "Fortress"),
    
    // 特殊建筑
    Temple          UMETA(DisplayName = "Temple"),
    Academy         UMETA(DisplayName = "Academy"),
    Hospital        UMETA(DisplayName = "Hospital"),
    Blacksmith      UMETA(DisplayName = "Blacksmith")
};

// 建筑状态
UENUM(BlueprintType)
enum class ERTSBuildingState: uint8 {
    Planned         UMETA(DisplayName = "Planned"),
    Constructing    UMETA(DisplayName = "Constructing"),
    Operational     UMETA(DisplayName = "Operational"),
    Upgrading       UMETA(DisplayName = "Upgrading"),
    Damaged         UMETA(DisplayName = "Damaged"),
    Destroyed       UMETA(DisplayName = "Destroyed"),
    Repairing       UMETA(DisplayName = "Repairing")
};

// 建筑成本
USTRUCT(BlueprintType)
struct FRTSBuildingCost
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    TMap<ERTSResourceType, float> ResourceCosts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    float ConstructionTime = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cost")
    int32 WorkerCount = 1;

    FRTSBuildingCost()
    {
        // 默认初始化
    }
};

// 建筑信息
USTRUCT(BlueprintType)
struct FRTSBuildingInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    ERTSBuildingType BuildingType = ERTSBuildingType::Barracks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    ERTSBuildingState BuildingState = ERTSBuildingState::Planned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 MaxLevel = 3;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float Health = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float MaxHealth = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float ConstructionProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    AActor* BuildingActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 OwningPlayerID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FRTSBuildingCost BuildCost;

    // 生产相关
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    bool bCanProduceUnits = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    TArray<FName> ProducibleUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    bool bCanResearchTech = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    TArray<FName> AvailableTechnologies;
};

// 建筑队列项
USTRUCT(BlueprintType)
struct FRTSConstructionQueueItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
    ERTSBuildingType BuildingType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
    float Progress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
    int32 OwningPlayerID = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
    FRTSBuildingCost Cost;
};

UCLASS(ClassGroup = (RTS), meta = (DisplayName = "RTS Building Construction System"))
class MINGRTS_API UMingRTSBuildingConstructionSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBuildingConstructionSystem();

    // 系统初始化
    UFUNCTION(BlueprintCallable, Category = "RTS|Building")
    void InitializeBuildingSystem();

    // 建筑计划与建造
    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Construction")
    bool PlanBuilding(ERTSBuildingType BuildingType, const FVector& Location, int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Construction")
    bool StartConstruction(const FRTSConstructionQueueItem& QueueItem);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Construction")
    void ProcessConstruction(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Construction")
    void CompleteConstruction(FRTSBuildingInfo& BuildingInfo);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Construction")
    void CancelConstruction(const FRTSConstructionQueueItem& QueueItem);

    // 建筑管理
    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Management")
    void RegisterBuilding(const FRTSBuildingInfo& BuildingInfo);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Management")
    void UnregisterBuilding(AActor* BuildingActor);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Management")
    void UpgradeBuilding(AActor* BuildingActor);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Management")
    void RepairBuilding(AActor* BuildingActor, float RepairAmount);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Management")
    void DestroyBuilding(AActor* BuildingActor);

    // 查询建筑
    UFUNCTION(BlueprintPure, Category = "RTS|Building|Query")
    TArray<FRTSBuildingInfo> GetBuildingsOfType(ERTSBuildingType BuildingType, int32 PlayerID = -1) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Building|Query")
    TArray<FRTSBuildingInfo> GetPlayerBuildings(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Building|Query")
    FRTSBuildingInfo GetBuildingInfo(AActor* BuildingActor) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Building|Query")
    bool IsBuildingOperational(AActor* BuildingActor) const;

    // 建筑成本
    UFUNCTION(BlueprintPure, Category = "RTS|Building|Cost")
    FRTSBuildingCost GetBuildingCost(ERTSBuildingType BuildingType, int32 Level = 1) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Building|Cost")
    bool CanAffordBuilding(ERTSBuildingType BuildingType, int32 PlayerID, int32 Level = 1) const;

    // 生产功能
    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Production")
    bool StartUnitProduction(AActor* BuildingActor, FName UnitType);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Production")
    bool StartTechnologyResearch(AActor* BuildingActor, FName TechType);

    UFUNCTION(BlueprintCallable, Category = "RTS|Building|Production")
    void CancelProduction(AActor* BuildingActor);

    // 建筑效果
    UFUNCTION(BlueprintPure, Category = "RTS|Building|Effects")
    float GetBuildingInfluenceRadius(ERTSBuildingType BuildingType, int32 Level) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Building|Effects")
    TArray<ERTSResourceType> GetBuildingProducibleResources(ERTSBuildingType BuildingType) const;

protected:
    UPROPERTY()
    TArray<FRTSBuildingInfo> Buildings;

    UPROPERTY()
    TArray<FRTSConstructionQueueItem> ConstructionQueue;

    UPROPERTY()
    TMap<ERTSBuildingType, FRTSBuildingCost> BuildingCostTemplates;

    UPROPERTY()
    TMap<ERTSBuildingType, TMap<int32, FRTSBuildingCost>> BuildingUpgradeCosts;

    // 内部函数
    void InitializeBuildingTemplates();
    void ProcessConstructionQueue(float DeltaTime);
    void UpdateBuildingEffects();
    void ConsumeConstructionResources(const FRTSConstructionQueueItem& QueueItem, float ProgressDelta);

public:
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingConstructionStarted, FRTSBuildingInfo, BuildingInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingConstructionCompleted, FRTSBuildingInfo, BuildingInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingUpgraded, FRTSBuildingInfo, BuildingInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingDestroyed, FRTSBuildingInfo, BuildingInfo);

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnBuildingConstructionStarted OnBuildingConstructionStarted;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnBuildingConstructionCompleted OnBuildingConstructionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnBuildingUpgraded OnBuildingUpgraded;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnBuildingDestroyed OnBuildingDestroyed;
};
