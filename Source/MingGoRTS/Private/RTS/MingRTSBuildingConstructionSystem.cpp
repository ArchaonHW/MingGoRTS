// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/MingRTSBuildingConstructionSystem.h"
#include "RTS/MingRTSResourceGatheringSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingRTSBuildingConstructionSystem::UMingRTSBuildingConstructionSystem()
{
}

void UMingRTSBuildingConstructionSystem::InitializeBuildingSystem()
{
    Buildings.Empty();
    ConstructionQueue.Empty();
    InitializeBuildingTemplates();
    
    UE_LOG(LogTemp, Log, TEXT("RTS Building Construction System initialized"));
}

void UMingRTSBuildingConstructionSystem::InitializeBuildingTemplates()
{
    // 总部
    FRTSBuildingCost HQCost;
    HQCost.ResourceCosts.Add(ERTSResourceType::Wood, 500.0f);
    HQCost.ResourceCosts.Add(ERTSResourceType::Stone, 300.0f);
    HQCost.ResourceCosts.Add(ERTSResourceType::Gold, 200.0f);
    HQCost.ConstructionTime = 30.0f;
    HQCost.WorkerCount = 5;
    BuildingCostTemplates.Add(ERTSBuildingType::Headquarters, HQCost);

    // 兵营
    FRTSBuildingCost BarracksCost;
    BarracksCost.ResourceCosts.Add(ERTSResourceType::Wood, 300.0f);
    BarracksCost.ResourceCosts.Add(ERTSResourceType::Stone, 100.0f);
    BarracksCost.ConstructionTime = 20.0f;
    BarracksCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Barracks, BarracksCost);

    // 马厩
    FRTSBuildingCost StableCost;
    StableCost.ResourceCosts.Add(ERTSResourceType::Wood, 350.0f);
    StableCost.ResourceCosts.Add(ERTSResourceType::Stone, 150.0f);
    StableCost.ConstructionTime = 25.0f;
    StableCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Stable, StableCost);

    // 农场
    FRTSBuildingCost FarmCost;
    FarmCost.ResourceCosts.Add(ERTSResourceType::Wood, 100.0f);
    FarmCost.ConstructionTime = 10.0f;
    FarmCost.WorkerCount = 2;
    BuildingCostTemplates.Add(ERTSBuildingType::Farm, FarmCost);

    // 伐木场
    FRTSBuildingCost LumberMillCost;
    LumberMillCost.ResourceCosts.Add(ERTSResourceType::Wood, 150.0f);
    LumberMillCost.ResourceCosts.Add(ERTSResourceType::Stone, 50.0f);
    LumberMillCost.ConstructionTime = 15.0f;
    LumberMillCost.WorkerCount = 2;
    BuildingCostTemplates.Add(ERTSBuildingType::LumberMill, LumberMillCost);

    // 矿场
    FRTSBuildingCost MineCost;
    MineCost.ResourceCosts.Add(ERTSResourceType::Wood, 200.0f);
    MineCost.ResourceCosts.Add(ERTSResourceType::Stone, 100.0f);
    MineCost.ConstructionTime = 20.0f;
    MineCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Mine, MineCost);

    // 市场
    FRTSBuildingCost MarketCost;
    MarketCost.ResourceCosts.Add(ERTSResourceType::Wood, 250.0f);
    MarketCost.ResourceCosts.Add(ERTSResourceType::Stone, 100.0f);
    MarketCost.ResourceCosts.Add(ERTSResourceType::Gold, 100.0f);
    MarketCost.ConstructionTime = 20.0f;
    MarketCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Market, MarketCost);

    // 城墙
    FRTSBuildingCost WallCost;
    WallCost.ResourceCosts.Add(ERTSResourceType::Stone, 50.0f);
    WallCost.ConstructionTime = 8.0f;
    WallCost.WorkerCount = 2;
    BuildingCostTemplates.Add(ERTSBuildingType::Wall, WallCost);

    // 箭塔
    FRTSBuildingCost TowerCost;
    TowerCost.ResourceCosts.Add(ERTSResourceType::Wood, 150.0f);
    TowerCost.ResourceCosts.Add(ERTSResourceType::Stone, 200.0f);
    TowerCost.ConstructionTime = 18.0f;
    TowerCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Tower, TowerCost);

    // 铁匠铺
    FRTSBuildingCost BlacksmithCost;
    BlacksmithCost.ResourceCosts.Add(ERTSResourceType::Wood, 200.0f);
    BlacksmithCost.ResourceCosts.Add(ERTSResourceType::Iron, 100.0f);
    BlacksmithCost.ResourceCosts.Add(ERTSResourceType::Coal, 50.0f);
    BlacksmithCost.ConstructionTime = 25.0f;
    BlacksmithCost.WorkerCount = 3;
    BuildingCostTemplates.Add(ERTSBuildingType::Blacksmith, BlacksmithCost);
}

bool UMingRTSBuildingConstructionSystem::PlanBuilding(ERTSBuildingType BuildingType, const FVector& Location, int32 PlayerID)
{
    // 检查是否可以建造
    if (!CanAffordBuilding(BuildingType, PlayerID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Player %d cannot afford building %s"), 
            PlayerID, *UEnum::GetValueAsString(BuildingType));
        return false;
    }
    
    // 添加到建造队列
    FRTSConstructionQueueItem QueueItem;
    QueueItem.BuildingType = BuildingType;
    QueueItem.Location = Location;
    QueueItem.OwningPlayerID = PlayerID;
    QueueItem.Cost = GetBuildingCost(BuildingType);
    QueueItem.Progress = 0.0f;
    
    ConstructionQueue.Add(QueueItem);
    
    UE_LOG(LogTemp, Log, TEXT("Planned building %s for Player %d"), 
        *UEnum::GetValueAsString(BuildingType), PlayerID);
    
    return true;
}

bool UMingRTSBuildingConstructionSystem::StartConstruction(const FRTSConstructionQueueItem& QueueItem)
{
    UE_LOG(LogTemp, Log, TEXT("Started construction of %s"), 
        *UEnum::GetValueAsString(QueueItem.BuildingType));
    return true;
}

void UMingRTSBuildingConstructionSystem::ProcessConstruction(float DeltaTime)
{
    ProcessConstructionQueue(DeltaTime);
    UpdateBuildingEffects();
}

void UMingRTSBuildingConstructionSystem::ProcessConstructionQueue(float DeltaTime)
{
    for (FRTSConstructionQueueItem& QueueItem : ConstructionQueue)
    {
        if (QueueItem.Progress >= 1.0f)
        {
            continue;
        }
        
        // 计算建造进度
        float ProgressRate = 1.0f / QueueItem.Cost.ConstructionTime;
        QueueItem.Progress += ProgressRate * DeltaTime;
        
        // 消耗资源（按进度比例）
        ConsumeConstructionResources(QueueItem, ProgressRate * DeltaTime);
        
        // 检查是否完成
        if (QueueItem.Progress >= 1.0f)
        {
            // 创建建筑信息
            FRTSBuildingInfo BuildingInfo;
            BuildingInfo.BuildingType = QueueItem.BuildingType;
            BuildingInfo.BuildingState = ERTSBuildingState::Operational;
            BuildingInfo.Location = QueueItem.Location;
            BuildingInfo.OwningPlayerID = QueueItem.OwningPlayerID;
            BuildingInfo.Level = 1;
            BuildingInfo.Health = 1000.0f;
            BuildingInfo.MaxHealth = 1000.0f;
            BuildingInfo.ConstructionProgress = 1.0f;
            BuildingInfo.BuildCost = QueueItem.Cost;
            
            // 设置生产功能
            switch (QueueItem.BuildingType)
            {
            case ERTSBuildingType::Barracks:
                BuildingInfo.bCanProduceUnits = true;
                BuildingInfo.ProducibleUnits.Add(FName("Infantry"));
                BuildingInfo.ProducibleUnits.Add(FName("Archer"));
                break;
            case ERTSBuildingType::Stable:
                BuildingInfo.bCanProduceUnits = true;
                BuildingInfo.ProducibleUnits.Add(FName("Cavalry"));
                break;
            case ERTSBuildingType::Workshop:
                BuildingInfo.bCanProduceUnits = true;
                BuildingInfo.ProducibleUnits.Add(FName("Siege"));
                break;
            case ERTSBuildingType::Blacksmith:
                BuildingInfo.bCanResearchTech = true;
                BuildingInfo.AvailableTechnologies.Add(FName("WeaponUpgrade"));
                BuildingInfo.AvailableTechnologies.Add(FName("ArmorUpgrade"));
                break;
            default:
                break;
            }
            
            RegisterBuilding(BuildingInfo);
            OnBuildingConstructionCompleted.Broadcast(BuildingInfo);
            
            UE_LOG(LogTemp, Log, TEXT("Construction of %s completed"), 
                *UEnum::GetValueAsString(QueueItem.BuildingType));
        }
    }
    
    // 移除已完成的建造项目
    for (int32 i = ConstructionQueue.Num() - 1; i >= 0; --i)
    {
        if (ConstructionQueue[i].Progress >= 1.0f)
        {
            ConstructionQueue.RemoveAt(i);
        }
    }
}

void UMingRTSBuildingConstructionSystem::ConsumeConstructionResources(const FRTSConstructionQueueItem& QueueItem, float ProgressDelta)
{
    // 这里应该与资源系统集成，实际消耗资源
    // 简化处理：资源已在PlanBuilding时检查，建造过程中持续消耗
}

void UMingRTSBuildingConstructionSystem::CompleteConstruction(FRTSBuildingInfo& BuildingInfo)
{
    BuildingInfo.BuildingState = ERTSBuildingState::Operational;
    BuildingInfo.ConstructionProgress = 1.0f;
}

void UMingRTSBuildingConstructionSystem::CancelConstruction(const FRTSConstructionQueueItem& QueueItem)
{
    for (int32 i = ConstructionQueue.Num() - 1; i >= 0; --i)
    {
        if (ConstructionQueue[i].BuildingType == QueueItem.BuildingType &&
            ConstructionQueue[i].Location.Equals(QueueItem.Location))
        {
            ConstructionQueue.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Cancelled construction of %s"), 
                *UEnum::GetValueAsString(QueueItem.BuildingType));
            break;
        }
    }
}

void UMingRTSBuildingConstructionSystem::RegisterBuilding(const FRTSBuildingInfo& BuildingInfo)
{
    Buildings.Add(BuildingInfo);
    OnBuildingConstructionStarted.Broadcast(BuildingInfo);
}

void UMingRTSBuildingConstructionSystem::UnregisterBuilding(AActor* BuildingActor)
{
    for (int32 i = Buildings.Num() - 1; i >= 0; --i)
    {
        if (Buildings[i].BuildingActor == BuildingActor)
        {
            OnBuildingDestroyed.Broadcast(Buildings[i]);
            Buildings.RemoveAt(i);
            break;
        }
    }
}

void UMingRTSBuildingConstructionSystem::UpgradeBuilding(AActor* BuildingActor)
{
    for (FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.BuildingActor == BuildingActor)
        {
            if (Building.Level < Building.MaxLevel)
            {
                Building.Level++;
                Building.Health += 500.0f;
                Building.MaxHealth += 500.0f;
                
                // 提升建筑效果
                // TODO: 实现具体的升级效果
                
                OnBuildingUpgraded.Broadcast(Building);
                UE_LOG(LogTemp, Log, TEXT("Building upgraded to Level %d"), Building.Level);
            }
            break;
        }
    }
}

void UMingRTSBuildingConstructionSystem::RepairBuilding(AActor* BuildingActor, float RepairAmount)
{
    for (FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.BuildingActor == BuildingActor)
        {
            Building.Health = FMath::Min(Building.Health + RepairAmount, Building.MaxHealth);
            
            if (Building.Health >= Building.MaxHealth * 0.5f && 
                Building.BuildingState == ERTSBuildingState::Damaged)
            {
                Building.BuildingState = ERTSBuildingState::Operational;
            }
            break;
        }
    }
}

void UMingRTSBuildingConstructionSystem::DestroyBuilding(AActor* BuildingActor)
{
    UnregisterBuilding(BuildingActor);
}

TArray<FRTSBuildingInfo> UMingRTSBuildingConstructionSystem::GetBuildingsOfType(ERTSBuildingType BuildingType, int32 PlayerID) const
{
    TArray<FRTSBuildingInfo> Result;
    for (const FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.BuildingType == BuildingType)
        {
            if (PlayerID == -1 || Building.OwningPlayerID == PlayerID)
            {
                Result.Add(Building);
            }
        }
    }
    return Result;
}

TArray<FRTSBuildingInfo> UMingRTSBuildingConstructionSystem::GetPlayerBuildings(int32 PlayerID) const
{
    TArray<FRTSBuildingInfo> Result;
    for (const FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.OwningPlayerID == PlayerID)
        {
            Result.Add(Building);
        }
    }
    return Result;
}

FRTSBuildingInfo UMingRTSBuildingConstructionSystem::GetBuildingInfo(AActor* BuildingActor) const
{
    for (const FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.BuildingActor == BuildingActor)
        {
            return Building;
        }
    }
    return FRTSBuildingInfo();
}

bool UMingRTSBuildingConstructionSystem::IsBuildingOperational(AActor* BuildingActor) const
{
    FRTSBuildingInfo Info = GetBuildingInfo(BuildingActor);
    return Info.BuildingState == ERTSBuildingState::Operational;
}

FRTSBuildingCost UMingRTSBuildingConstructionSystem::GetBuildingCost(ERTSBuildingType BuildingType, int32 Level) const
{
    if (Level == 1)
    {
        return BuildingCostTemplates.FindRef(BuildingType);
    }
    
    // 计算升级成本
    FRTSBuildingCost BaseCost = BuildingCostTemplates.FindRef(BuildingType);
    FRTSBuildingCost UpgradeCost;
    
    // 每级成本增加50%
    float Multiplier = FMath::Pow(1.5f, Level - 1);
    
    for (const auto& ResourceCost : BaseCost.ResourceCosts)
    {
        UpgradeCost.ResourceCosts.Add(ResourceCost.Key, ResourceCost.Value * Multiplier);
    }
    
    UpgradeCost.ConstructionTime = BaseCost.ConstructionTime * Multiplier;
    UpgradeCost.WorkerCount = BaseCost.WorkerCount;
    
    return UpgradeCost;
}

bool UMingRTSBuildingConstructionSystem::CanAffordBuilding(ERTSBuildingType BuildingType, int32 PlayerID, int32 Level) const
{
    // 这里需要与资源系统集成
    // 简化处理：假设资源足够
    UE_LOG(LogTemp, Log, TEXT("Checking if Player %d can afford %s Level %d"),
        PlayerID, *UEnum::GetValueAsString(BuildingType), Level);
    return true;
}

bool UMingRTSBuildingConstructionSystem::StartUnitProduction(AActor* BuildingActor, FName UnitType)
{
    FRTSBuildingInfo Building = GetBuildingInfo(BuildingActor);
    
    if (!Building.bCanProduceUnits)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building cannot produce units"));
        return false;
    }
    
    if (!Building.ProducibleUnits.Contains(UnitType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Building cannot produce unit type: %s"), *UnitType.ToString());
        return false;
    }
    
    // 开始生产单位
    UE_LOG(LogTemp, Log, TEXT("Started producing unit: %s"), *UnitType.ToString());
    return true;
}

bool UMingRTSBuildingConstructionSystem::StartTechnologyResearch(AActor* BuildingActor, FName TechType)
{
    FRTSBuildingInfo Building = GetBuildingInfo(BuildingActor);
    
    if (!Building.bCanResearchTech)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building cannot research technologies"));
        return false;
    }
    
    if (!Building.AvailableTechnologies.Contains(TechType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Technology not available: %s"), *TechType.ToString());
        return false;
    }
    
    // 开始研究科技
    UE_LOG(LogTemp, Log, TEXT("Started researching technology: %s"), *TechType.ToString());
    return true;
}

void UMingRTSBuildingConstructionSystem::CancelProduction(AActor* BuildingActor)
{
    // 取消建筑的生产/研究任务
    UE_LOG(LogTemp, Log, TEXT("Cancelled production for building"));
}

float UMingRTSBuildingConstructionSystem::GetBuildingInfluenceRadius(ERTSBuildingType BuildingType, int32 Level) const
{
    float BaseRadius = 500.0f;
    
    switch (BuildingType)
    {
    case ERTSBuildingType::Headquarters:
        BaseRadius = 1000.0f;
        break;
    case ERTSBuildingType::Tower:
        BaseRadius = 800.0f;
        break;
    case ERTSBuildingType::Market:
        BaseRadius = 600.0f;
        break;
    default:
        BaseRadius = 500.0f;
        break;
    }
    
    return BaseRadius * (1.0f + (Level - 1) * 0.2f);
}

TArray<ERTSResourceType> UMingRTSBuildingConstructionSystem::GetBuildingProducibleResources(ERTSBuildingType BuildingType) const
{
    TArray<ERTSResourceType> Result;
    
    switch (BuildingType)
    {
    case ERTSBuildingType::Farm:
        Result.Add(ERTSResourceType::Food);
        break;
    case ERTSBuildingType::LumberMill:
        Result.Add(ERTSResourceType::Wood);
        break;
    case ERTSBuildingType::Mine:
        Result.Add(ERTSResourceType::Stone);
        Result.Add(ERTSResourceType::Iron);
        Result.Add(ERTSResourceType::Coal);
        break;
    case ERTSBuildingType::Market:
        Result.Add(ERTSResourceType::Gold);
        Result.Add(ERTSResourceType::Silk);
        Result.Add(ERTSResourceType::Tea);
        Result.Add(ERTSResourceType::Porcelain);
        Result.Add(ERTSResourceType::Spices);
        break;
    default:
        break;
    }
    
    return Result;
}

void UMingRTSBuildingConstructionSystem::UpdateBuildingEffects()
{
    // 更新建筑效果（资源产出、防御加成等）
    for (FRTSBuildingInfo& Building : Buildings)
    {
        if (Building.BuildingState == ERTSBuildingState::Operational)
        {
            // 这里可以实现建筑持续效果的逻辑
            // 例如：市场建筑产生税收、农场产生食物等
        }
    }
}
