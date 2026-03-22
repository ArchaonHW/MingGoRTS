// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/MingRTSResourceGatheringSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

UMingRTSResourceGatheringSystem::UMingRTSResourceGatheringSystem()
{
    InitializeDefaultValues();
}

void UMingRTSResourceGatheringSystem::InitializeResourceSystem()
{
    ResourceNodes.Empty();
    ActiveGatheringTasks.Empty();
    PlayerInventories.Empty();
    GatheringEfficiencyBonuses.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("RTS Resource Gathering System initialized"));
}

void UMingRTSResourceGatheringSystem::InitializeDefaultValues()
{
    // 基础采集速度 (单位/秒)
    BaseGatherRates.Add(ERTSResourceType::Gold, 10.0f);
    BaseGatherRates.Add(ERTSResourceType::Food, 15.0f);
    BaseGatherRates.Add(ERTSResourceType::Wood, 12.0f);
    BaseGatherRates.Add(ERTSResourceType::Stone, 8.0f);
    BaseGatherRates.Add(ERTSResourceType::Iron, 6.0f);
    BaseGatherRates.Add(ERTSResourceType::Coal, 6.0f);
    BaseGatherRates.Add(ERTSResourceType::Silk, 4.0f);
    BaseGatherRates.Add(ERTSResourceType::Tea, 4.0f);
    BaseGatherRates.Add(ERTSResourceType::Porcelain, 2.0f);
    BaseGatherRates.Add(ERTSResourceType::Spices, 2.0f);

    // 资源基础价值
    ResourceBaseValues.Add(ERTSResourceType::Gold, 1.0f);
    ResourceBaseValues.Add(ERTSResourceType::Food, 0.5f);
    ResourceBaseValues.Add(ERTSResourceType::Wood, 0.4f);
    ResourceBaseValues.Add(ERTSResourceType::Stone, 0.6f);
    ResourceBaseValues.Add(ERTSResourceType::Iron, 0.8f);
    ResourceBaseValues.Add(ERTSResourceType::Coal, 0.5f);
    ResourceBaseValues.Add(ERTSResourceType::Silk, 2.0f);
    ResourceBaseValues.Add(ERTSResourceType::Tea, 1.5f);
    ResourceBaseValues.Add(ERTSResourceType::Porcelain, 3.0f);
    ResourceBaseValues.Add(ERTSResourceType::Spices, 4.0f);
}

void UMingRTSResourceGatheringSystem::RegisterResourceNode(const FRTSResourceNode& Node)
{
    ResourceNodes.Add(Node);
    UE_LOG(LogTemp, Log, TEXT("Registered resource node: %s at location %s"),
        *UEnum::GetValueAsString(Node.ResourceType),
        *Node.Location.ToString());
}

void UMingRTSResourceGatheringSystem::UnregisterResourceNode(AActor* NodeActor)
{
    for (int32 i = ResourceNodes.Num() - 1; i >= 0; --i)
    {
        if (ResourceNodes[i].NodeActor == NodeActor)
        {
            ResourceNodes.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Unregistered resource node"));
            break;
        }
    }
}

TArray<FRTSResourceNode> UMingRTSResourceGatheringSystem::GetResourceNodesOfType(ERTSResourceType ResourceType) const
{
    TArray<FRTSResourceNode> Result;
    for (const FRTSResourceNode& Node : ResourceNodes)
    {
        if (Node.ResourceType == ResourceType && !Node.bIsDepleted)
        {
            Result.Add(Node);
        }
    }
    return Result;
}

FRTSResourceNode UMingRTSResourceGatheringSystem::FindNearestResourceNode(const FVector& Location, ERTSResourceType ResourceType) const
{
    FRTSResourceNode NearestNode;
    float MinDistance = FLT_MAX;
    
    for (const FRTSResourceNode& Node : ResourceNodes)
    {
        if (Node.ResourceType == ResourceType && !Node.bIsDepleted)
        {
            float Distance = FVector::Distance(Location, Node.Location);
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                NearestNode = Node;
            }
        }
    }
    
    return NearestNode;
}

void UMingRTSResourceGatheringSystem::StartGathering(AActor* Gatherer, const FRTSResourceNode& TargetNode)
{
    if (!Gatherer || TargetNode.bIsDepleted)
    {
        return;
    }
    
    // 检查是否已在采集
    for (FRTSGatheringTask& Task : ActiveGatheringTasks)
    {
        if (Task.Gatherer == Gatherer)
        {
            Task.TargetNode = TargetNode;
            return;
        }
    }
    
    // 创建新的采集任务
    FRTSGatheringTask NewTask;
    NewTask.Gatherer = Gatherer;
    NewTask.TargetNode = TargetNode;
    NewTask.GatherAmount = 0.0f;
    NewTask.Progress = 0.0f;
    NewTask.bIsComplete = false;
    
    ActiveGatheringTasks.Add(NewTask);
    
    UE_LOG(LogTemp, Log, TEXT("Started gathering %s"), *UEnum::GetValueAsString(TargetNode.ResourceType));
}

void UMingRTSResourceGatheringSystem::StopGathering(AActor* Gatherer)
{
    for (int32 i = ActiveGatheringTasks.Num() - 1; i >= 0; --i)
    {
        if (ActiveGatheringTasks[i].Gatherer == Gatherer)
        {
            ActiveGatheringTasks.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Stopped gathering"));
            break;
        }
    }
}

void UMingRTSResourceGatheringSystem::ProcessGathering(float DeltaTime)
{
    for (FRTSGatheringTask& Task : ActiveGatheringTasks)
    {
        if (Task.bIsComplete || Task.TargetNode.bIsDepleted)
        {
            continue;
        }
        
        // 计算采集速度
        float BaseRate = BaseGatherRates.FindRef(Task.TargetNode.ResourceType);
        float Efficiency = GetGatheringEfficiency(0, Task.TargetNode.ResourceType); // TODO: 获取正确的PlayerID
        float GatherRate = BaseRate * Efficiency * Task.TargetNode.GatherRate;
        
        // 更新进度
        float GatherAmount = GatherRate * DeltaTime;
        Task.GatherAmount += GatherAmount;
        Task.Progress += GatherAmount / 100.0f; // 假设每100单位为一个采集周期
        
        // 检查是否完成一个采集周期
        if (Task.Progress >= 1.0f)
        {
            CompleteGathering(Task);
            Task.Progress = 0.0f;
            Task.GatherAmount = 0.0f;
        }
    }
    
    // 清理无效任务
    for (int32 i = ActiveGatheringTasks.Num() - 1; i >= 0; --i)
    {
        if (!ActiveGatheringTasks[i].Gatherer || ActiveGatheringTasks[i].TargetNode.bIsDepleted)
        {
            ActiveGatheringTasks.RemoveAt(i);
        }
    }
}

void UMingRTSResourceGatheringSystem::CompleteGathering(const FRTSGatheringTask& Task)
{
    // 添加资源到玩家库存
    int32 PlayerID = 0; // TODO: 从Gatherer获取PlayerID
    AddResource(Task.TargetNode.ResourceType, 100.0f, PlayerID);
    
    // 减少资源节点数量
    for (FRTSResourceNode& Node : ResourceNodes)
    {
        if (Node.NodeActor == Task.TargetNode.NodeActor)
        {
            Node.Amount -= 100.0f;
            if (Node.Amount <= 0.0f)
            {
                Node.bIsDepleted = true;
                OnResourceDepleted.Broadcast(Node.NodeActor, Node.ResourceType);
            }
            break;
        }
    }
    
    OnResourceGathered.Broadcast(Task.TargetNode.ResourceType, 100.0f, PlayerID);
    UE_LOG(LogTemp, Log, TEXT("Gathered 100 units of %s"), *UEnum::GetValueAsString(Task.TargetNode.ResourceType));
}

void UMingRTSResourceGatheringSystem::AddResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID)
{
    FRTSPlayerInventory& Inventory = PlayerInventories.FindOrAdd(PlayerID);
    
    float CurrentAmount = Inventory.Resources.FindRef(ResourceType);
    float Capacity = Inventory.ResourceCapacity.FindRef(ResourceType);
    
    float NewAmount = FMath::Min(CurrentAmount + Amount, Capacity);
    Inventory.Resources.Add(ResourceType, NewAmount);
    
    UE_LOG(LogTemp, Log, TEXT("Player %d now has %.1f %s"), PlayerID, NewAmount, *UEnum::GetValueAsString(ResourceType));
}

bool UMingRTSResourceGatheringSystem::ConsumeResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID)
{
    FRTSPlayerInventory& Inventory = PlayerInventories.FindOrAdd(PlayerID);
    
    float CurrentAmount = Inventory.Resources.FindRef(ResourceType);
    
    if (CurrentAmount >= Amount)
    {
        Inventory.Resources.Add(ResourceType, CurrentAmount - Amount);
        return true;
    }
    
    return false;
}

float UMingRTSResourceGatheringSystem::GetResourceAmount(ERTSResourceType ResourceType, int32 PlayerID) const
{
    const FRTSPlayerInventory* Inventory = PlayerInventories.Find(PlayerID);
    if (Inventory)
    {
        return Inventory->Resources.FindRef(ResourceType);
    }
    return 0.0f;
}

float UMingRTSResourceGatheringSystem::GetResourceCapacity(ERTSResourceType ResourceType, int32 PlayerID) const
{
    const FRTSPlayerInventory* Inventory = PlayerInventories.Find(PlayerID);
    if (Inventory)
    {
        return Inventory->ResourceCapacity.FindRef(ResourceType);
    }
    return 0.0f;
}

bool UMingRTSResourceGatheringSystem::HasEnoughResource(ERTSResourceType ResourceType, float Amount, int32 PlayerID) const
{
    return GetResourceAmount(ResourceType, PlayerID) >= Amount;
}

FRTSPlayerInventory UMingRTSResourceGatheringSystem::GetPlayerInventory(int32 PlayerID) const
{
    const FRTSPlayerInventory* Inventory = PlayerInventories.Find(PlayerID);
    if (Inventory)
    {
        return *Inventory;
    }
    return FRTSPlayerInventory();
}

bool UMingRTSResourceGatheringSystem::TradeResources(int32 FromPlayerID, int32 ToPlayerID, ERTSResourceType ResourceType, float Amount)
{
    if (ConsumeResource(ResourceType, Amount, FromPlayerID))
    {
        AddResource(ResourceType, Amount, ToPlayerID);
        OnResourcesTraded.Broadcast(ResourceType, Amount, ToPlayerID);
        UE_LOG(LogTemp, Log, TEXT("Traded %.1f %s from Player %d to Player %d"),
            Amount, *UEnum::GetValueAsString(ResourceType), FromPlayerID, ToPlayerID);
        return true;
    }
    
    return false;
}

float UMingRTSResourceGatheringSystem::CalculateResourceValue(ERTSResourceType ResourceType) const
{
    return ResourceBaseValues.FindRef(ResourceType);
}

void UMingRTSResourceGatheringSystem::ApplyGatheringEfficiencyBonus(int32 PlayerID, ERTSResourceType ResourceType, float BonusMultiplier)
{
    TMap<ERTSResourceType, float>& Bonuses = GatheringEfficiencyBonuses.FindOrAdd(PlayerID);
    Bonuses.Add(ResourceType, BonusMultiplier);
}

float UMingRTSResourceGatheringSystem::GetGatheringEfficiency(int32 PlayerID, ERTSResourceType ResourceType) const
{
    const TMap<ERTSResourceType, float>* Bonuses = GatheringEfficiencyBonuses.Find(PlayerID);
    if (Bonuses)
    {
        return 1.0f + Bonuses->FindRef(ResourceType);
    }
    return 1.0f;
}
