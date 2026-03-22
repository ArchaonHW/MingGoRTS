#include "MingResourceSystem.h"
#include "MingResourceProducer.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"

UMingResourceSystem::UMingResourceSystem()
{
    // Initialize default capacities
    ResourceCapacityLimits.Add(EMingResourceType::Food, 1000);
    ResourceCapacityLimits.Add(EMingResourceType::Money, 50000);
    ResourceCapacityLimits.Add(EMingResourceType::Materials, 2000);
    ResourceCapacityLimits.Add(EMingResourceType::Fuel, 1000);
    ResourceCapacityLimits.Add(EMingResourceType::Ammo, 500);
    ResourceCapacityLimits.Add(EMingResourceType::Manpower, 500);
}

void UMingResourceSystem::InitializeDefaultResources()
{
    // Starting resources for a new game
    ResourceStorage.Add(EMingResourceType::Food, 200);
    ResourceStorage.Add(EMingResourceType::Money, 1000);
    ResourceStorage.Add(EMingResourceType::Materials, 100);
    ResourceStorage.Add(EMingResourceType::Fuel, 50);
    ResourceStorage.Add(EMingResourceType::Ammo, 100);
    ResourceStorage.Add(EMingResourceType::Manpower, 50);

    UE_LOG(LogTemp, Log, TEXT("Resource system initialized with default resources"));
}

void UMingResourceSystem::AddResource(EMingResourceType Type, int32 Amount)
{
    if (Amount <= 0)
    {
        return;
    }

    int32 CurrentAmount = GetResourceAmount(Type);
    int32 NewAmount = CurrentAmount + Amount;

    // Check capacity limit
    int32 Capacity = GetResourceCapacity(Type);
    if (NewAmount > Capacity)
    {
        NewAmount = Capacity;
        UE_LOG(LogTemp, Warning, TEXT("Resource %s reached capacity limit (%d)"), 
               *GetResourceName(Type), Capacity);
    }

    ResourceStorage.Add(Type, NewAmount);
    
    // Notify change
    NotifyResourceChange(Type, NewAmount);

    UE_LOG(LogTemp, Verbose, TEXT("Added %d %s. Total: %d"), 
           Amount, *GetResourceName(Type), NewAmount);
}

bool UMingResourceSystem::ConsumeResource(EMingResourceType Type, int32 Amount)
{
    if (Amount <= 0)
    {
        return true;
    }

    int32 CurrentAmount = GetResourceAmount(Type);
    if (CurrentAmount < Amount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient %s. Required: %d, Available: %d"),
               *GetResourceName(Type), Amount, CurrentAmount);
        
        OnResourceInsufficient.Broadcast(Type);
        return false;
    }

    int32 NewAmount = CurrentAmount - Amount;
    ResourceStorage.Add(Type, NewAmount);
    
    // Notify change
    NotifyResourceChange(Type, NewAmount);

    UE_LOG(LogTemp, Verbose, TEXT("Consumed %d %s. Remaining: %d"),
           Amount, *GetResourceName(Type), NewAmount);

    return true;
}

bool UMingResourceSystem::HasEnoughResources(const TArray<FMingResourceCost>& Costs) const
{
    for (const FMingResourceCost& Cost : Costs)
    {
        if (GetResourceAmount(Cost.ResourceType) < Cost.Cost)
        {
            return false;
        }
    }
    return true;
}

bool UMingResourceSystem::ConsumeResources(const TArray<FMingResourceCost>& Costs)
{
    // First check if we have enough
    if (!HasEnoughResources(Costs))
    {
        // Find which resource is insufficient
        for (const FMingResourceCost& Cost : Costs)
        {
            if (GetResourceAmount(Cost.ResourceType) < Cost.Cost)
            {
                OnResourceInsufficient.Broadcast(Cost.ResourceType);
            }
        }
        return false;
    }

    // Consume all resources
    for (const FMingResourceCost& Cost : Costs)
    {
        ConsumeResource(Cost.ResourceType, Cost.Cost);
    }

    return true;
}

int32 UMingResourceSystem::GetResourceAmount(EMingResourceType Type) const
{
    const int32* Amount = ResourceStorage.Find(Type);
    return Amount ? *Amount : 0;
}

int32 UMingResourceSystem::GetResourceCapacity(EMingResourceType Type) const
{
    const int32* Capacity = ResourceCapacityLimits.Find(Type);
    return Capacity ? *Capacity : 999999; // No limit if not set
}

float UMingResourceSystem::GetResourcePercentage(EMingResourceType Type) const
{
    int32 Amount = GetResourceAmount(Type);
    int32 Capacity = GetResourceCapacity(Type);
    
    if (Capacity <= 0)
    {
        return 0.0f;
    }
    
    return FMath::Clamp((float)Amount / (float)Capacity, 0.0f, 1.0f);
}

void UMingResourceSystem::RegisterResourceProducer(UMingResourceProducer* Producer)
{
    if (Producer && !ResourceProducers.Contains(Producer))
    {
        ResourceProducers.Add(Producer);
        UE_LOG(LogTemp, Log, TEXT("Registered resource producer: %s"), *Producer->GetName());
    }
}

void UMingResourceSystem::UnregisterResourceProducer(UMingResourceProducer* Producer)
{
    if (Producer)
    {
        ResourceProducers.Remove(Producer);
        UE_LOG(LogTemp, Log, TEXT("Unregistered resource producer: %s"), *Producer->GetName());
    }
}

void UMingResourceSystem::AddProduction(const FMingResourceProduction& Production)
{
    // Remove existing production of same type
    RemoveProduction(Production.ResourceType);
    
    ActiveProductions.Add(Production);
    UE_LOG(LogTemp, Log, TEXT("Added production: %s at %.1f per %.1f seconds"),
           *GetResourceName(Production.ResourceType), Production.ProductionRate, Production.ProductionInterval);
}

void UMingResourceSystem::RemoveProduction(EMingResourceType Type)
{
    for (int32 i = ActiveProductions.Num() - 1; i >= 0; --i)
    {
        if (ActiveProductions[i].ResourceType == Type)
        {
            ActiveProductions.RemoveAt(i);
        }
    }
}

void UMingResourceSystem::UpdateProduction(float DeltaTime)
{
    // Update registered producers
    for (auto& WeakProducer : ResourceProducers)
    {
        if (WeakProducer.IsValid())
        {
            WeakProducer->UpdateProduction(DeltaTime);
        }
    }

    // Process active productions
    ProcessProduction(DeltaTime);
}

void UMingResourceSystem::ProcessProduction(float DeltaTime)
{
    for (FMingResourceProduction& Production : ActiveProductions)
    {
        Production.TimeSinceLastProduction += DeltaTime;
        
        if (Production.TimeSinceLastProduction >= Production.ProductionInterval)
        {
            // Calculate how much to produce
            int32 ProductionCycles = FMath::FloorToInt(Production.TimeSinceLastProduction / Production.ProductionInterval);
            int32 AmountToProduce = FMath::FloorToInt(ProductionCycles * Production.ProductionRate * Production.ProductionInterval);
            
            if (AmountToProduce > 0)
            {
                AddResource(Production.ResourceType, AmountToProduce);
                OnResourceProduced.Broadcast(Production.ResourceType, AmountToProduce);
                
                Production.TimeSinceLastProduction -= ProductionCycles * Production.ProductionInterval;
            }
        }
    }
}

void UMingResourceSystem::ClampResourcesToCapacity()
{
    for (auto& Pair : ResourceStorage)
    {
        EMingResourceType Type = Pair.Key;
        int32 Capacity = GetResourceCapacity(Type);
        
        if (Pair.Value > Capacity)
        {
            Pair.Value = Capacity;
        }
    }
}

void UMingResourceSystem::NotifyResourceChange(EMingResourceType Type, int32 NewAmount)
{
    OnResourceChanged.Broadcast(Type, NewAmount);
}

FString UMingResourceSystem::GetResourceName(EMingResourceType Type)
{
    switch (Type)
    {
    case EMingResourceType::Food:
        return TEXT("糧食");
    case EMingResourceType::Money:
        return TEXT("資金");
    case EMingResourceType::Materials:
        return TEXT("原材料");
    case EMingResourceType::Fuel:
        return TEXT("燃料");
    case EMingResourceType::Ammo:
        return TEXT("彈藥");
    case EMingResourceType::Manpower:
        return TEXT("人力");
    default:
        return TEXT("未知資源");
    }
}

FLinearColor UMingResourceSystem::GetResourceColor(EMingResourceType Type)
{
    switch (Type)
    {
    case EMingResourceType::Food:
        return FLinearColor(0.3f, 0.8f, 0.3f); // Green
    case EMingResourceType::Money:
        return FLinearColor(1.0f, 0.8f, 0.0f); // Gold
    case EMingResourceType::Materials:
        return FLinearColor(0.6f, 0.4f, 0.2f); // Brown
    case EMingResourceType::Fuel:
        return FLinearColor(0.2f, 0.3f, 0.8f); // Blue
    case EMingResourceType::Ammo:
        return FLinearColor(0.8f, 0.2f, 0.2f); // Red
    case EMingResourceType::Manpower:
        return FLinearColor(0.8f, 0.6f, 0.4f); // Beige
    default:
        return FLinearColor::White;
    }
}

TMap<EMingResourceType, int32> UMingResourceSystem::SerializeResources() const
{
    return ResourceStorage;
}

void UMingResourceSystem::DeserializeResources(const TMap<EMingResourceType, int32>& SavedResources)
{
    ResourceStorage = SavedResources;
    ClampResourcesToCapacity();
    
    UE_LOG(LogTemp, Log, TEXT("Deserialized %d resource types"), ResourceStorage.Num());
}
