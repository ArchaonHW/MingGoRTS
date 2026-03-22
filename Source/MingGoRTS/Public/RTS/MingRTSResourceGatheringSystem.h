// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSResourceGatheringSystem.generated.h"

// 资源类型 (与Epic 2.x系统对应)
UENUM(BlueprintType)
enum class ERTSResourceType: uint8 {
    Gold            UMETA(DisplayName = "Gold"),
    Food            UMETA(DisplayName = "Food"),
    Wood            UMETA(DisplayName = "Wood"),
    Stone           UMETA(DisplayName = "Stone"),
    Iron            UMETA(DisplayName = "Iron"),
    Coal            UMETA(DisplayName = "Coal"),
    Silk            UMETA(DisplayName = "Silk"),
    Tea             UMETA(DisplayName = "Tea"),
    Porcelain       UMETA(DisplayName = "Porcelain"),
    Spices          UMETA(DisplayName = "Spices")
};

// 采集者类型
UENUM(BlueprintType)
enum class ERTSGathererType: uint8 {
    Worker          UMETA(DisplayName = "Worker"),
    Farmer          UMETA(DisplayName = "Farmer"),
    Lumberjack      UMETA(DisplayName = "Lumberjack"),
    Miner           UMETA(DisplayName = "Miner"),
    Merchant        UMETA(DisplayName = "Merchant")
};

// 资源节点信息
USTRUCT(BlueprintType)
struct FRTSResourceNode
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    ERTSResourceType ResourceType = ERTSResourceType::Wood;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float Amount = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float MaxAmount = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    float GatherRate = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    bool bIsDepleted = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource")
    AActor* NodeActor = nullptr;
};

// 采集任务
USTRUCT(BlueprintType)
struct FRTSGatheringTask
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    AActor* Gatherer = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    FRTSResourceNode TargetNode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    float GatherAmount = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    float Progress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gathering")
    bool bIsComplete = false;
};

// 玩家资源库存
USTRUCT(BlueprintType)
struct FRTSPlayerInventory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TMap<ERTSResourceType, float> Resources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
    TMap<ERTSResourceType, float> ResourceCapacity;

    FRTSPlayerInventory()
    {
        // 初始化资源容量
        ResourceCapacity.Add(ERTSResourceType::Gold, 999999.0f); // 金币无上限
        ResourceCapacity.Add(ERTSResourceType::Food, 5000.0f);
        ResourceCapacity.Add(ERTSResourceType::Wood, 5000.0f);
        ResourceCapacity.Add(ERTSResourceType::Stone, 3000.0f);
        ResourceCapacity.Add(ERTSResourceType::Iron, 2000.0f);
        ResourceCapacity.Add(ERTSResourceType::Coal, 2000.0f);
        ResourceCapacity.Add(ERTSResourceType::Silk, 1000.0f);
        ResourceCapacity.Add(ERTSResourceType::Tea, 1000.0f);
        ResourceCapacity.Add(ERTSResourceType::Porcelain, 500.0f);
        ResourceCapacity.Add(ERTSResourceType::Spices, 500.0f);

        // 初始化资源为0
        for (uint8 i = 0; i < (uint8)ERTSResourceType::Spices + 1; i++)
        {
            Resources.Add((ERTSResourceType)i, 0.0f);
        }
    }
};

UCLASS(ClassGroup = (RTS), meta = (DisplayName = "RTS Resource Gathering System"))
class MINGGORTS_API UMingRTSResourceGatheringSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSResourceGatheringSystem();

    // 系统初始化
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource")
    void InitializeResourceSystem();

    // 资源节点管理
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Nodes")
    void RegisterResourceNode(const FRTSResourceNode& Node);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Nodes")
    void UnregisterResourceNode(AActor* NodeActor);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Nodes")
    TArray<FRTSResourceNode> GetResourceNodesOfType(ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Nodes")
    FRTSResourceNode FindNearestResourceNode(const FVector& Location, ERTSResourceType ResourceType) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Resource|Nodes")
    TArray<FRTSResourceNode> GetAllResourceNodes() const { return ResourceNodes; }

    // 采集管理
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Gathering")
    void StartGathering(AActor* Gatherer, const FRTSResourceNode& TargetNode);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Gathering")
    void StopGathering(AActor* Gatherer);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Gathering")
    void ProcessGathering(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Gathering")
    void CompleteGathering(const FRTSGatheringTask& Task);

    // 资源库存管理
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Inventory")
    void AddResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID = 0);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Inventory")
    bool ConsumeResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID = 0);

    UFUNCTION(BlueprintPure, Category = "RTS|Resource|Inventory")
    float GetResourceAmount(ERTSResourceType ResourceType, int32 PlayerID = 0) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Resource|Inventory")
    float GetResourceCapacity(ERTSResourceType ResourceType, int32 PlayerID = 0) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Resource|Inventory")
    bool HasEnoughResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID = 0) const;

    UFUNCTION(BlueprintPure, Category = "RTS|Resource|Inventory")
    FRTSPlayerInventory GetPlayerInventory(int32 PlayerID = 0) const;

    // 资源交易
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Trade")
    bool TradeResources(int32 FromPlayerID, int32 ToPlayerID, ERTSResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Trade")
    float CalculateResourceValue(ERTSResourceType ResourceType) const;

    // 效率加成
    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Efficiency")
    void ApplyGatheringEfficiencyBonus(int32 PlayerID, ERTSResourceType ResourceType, float BonusMultiplier);

    UFUNCTION(BlueprintCallable, Category = "RTS|Resource|Efficiency")
    float GetGatheringEfficiency(int32 PlayerID, ERTSResourceType ResourceType) const;

protected:
    UPROPERTY()
    TArray<FRTSResourceNode> ResourceNodes;

    UPROPERTY()
    TArray<FRTSGatheringTask> ActiveGatheringTasks;

    UPROPERTY()
    TMap<int32, FRTSPlayerInventory> PlayerInventories;

    UPROPERTY()
    TMap<int32, TMap<ERTSResourceType, float>> GatheringEfficiencyBonuses;

    // 基础采集速度 (单位/秒)
    UPROPERTY(EditDefaultsOnly, Category = "Gathering")
    TMap<ERTSResourceType, float> BaseGatherRates;

    // 资源基础价值
    UPROPERTY(EditDefaultsOnly, Category = "Economy")
    TMap<ERTSResourceType, float> ResourceBaseValues;

    // 内部函数
    void InitializeDefaultValues();
    void RegenerateResourceNodes();
    void UpdateResourceNodes(float DeltaTime);

public:
    // 事件委托
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourceGathered, ERTSResourceType, ResourceType, float, Amount, int32, PlayerID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceDepleted, AActor*, NodeActor, ERTSResourceType, ResourceType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnResourcesTraded, ERTSResourceType, ResourceType, float, Amount, int32, ToPlayerID);

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnResourceGathered OnResourceGathered;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnResourceDepleted OnResourceDepleted;

    UPROPERTY(BlueprintAssignable, Category = "RTS|Events")
    FOnResourcesTraded OnResourcesTraded;
};
