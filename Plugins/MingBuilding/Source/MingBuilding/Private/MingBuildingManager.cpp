#include "MingBuildingManager.h"
#include "MingCoreEventBus.h"

UMingBuildingManager::UMingBuildingManager()
    : bIsInitialized(false)
{
}

void UMingBuildingManager::Initialize()
{
    if (bIsInitialized)
    {
        return;
    }

    // 初始化資源
    ResourceInventory.Add(TEXT("Food"), 0);
    ResourceInventory.Add(TEXT("Money"), 0);
    ResourceInventory.Add(TEXT("Materials"), 0);

    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingBuildingManager initialized - Building system ready"));
}

void UMingBuildingManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    ResourceInventory.Empty();
    bIsInitialized = false;

    UE_LOG(LogTemp, Log, TEXT("MingBuildingManager shutdown"));
}

void UMingBuildingManager::SetupEventSubscriptions()
{
    // 訂閱角色升級事件 (來自個人層)
    // 當角色升級時，可能解鎖新的建築或科技
    UE_LOG(LogTemp, Log, TEXT("BuildingManager event subscriptions setup complete"));
}

void UMingBuildingManager::CleanupEventSubscriptions()
{
    UE_LOG(LogTemp, Log, TEXT("BuildingManager event subscriptions cleaned up"));
}

void UMingBuildingManager::ConstructBuilding(const FString& BuildingType, const FVector& Position)
{
    UE_LOG(LogTemp, Log, TEXT("Constructing %s at position (%f, %f, %f)"),
        *BuildingType, Position.X, Position.Y, Position.Z);
    
    // 檢查資源是否足夠 (簡化檢查)
    if (GetResourceAmount(TEXT("Materials")) < 100)
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient materials for construction"));
        return;
    }
    
    // 消耗資源
    AddResource(TEXT("Materials"), -100);
    
    // 發布建築完成事件
    OnBuildingConstructed(BuildingType);
}

void UMingBuildingManager::UpgradeBuilding(int32 BuildingId)
{
    UE_LOG(LogTemp, Log, TEXT("Upgrading building %d"), BuildingId);
    // TODO: 實現升級邏輯
}

void UMingBuildingManager::AddResource(const FString& ResourceType, int32 Amount)
{
    if (Amount == 0)
    {
        return;
    }

    int32* CurrentAmount = ResourceInventory.Find(ResourceType);
    if (CurrentAmount)
    {
        *CurrentAmount += Amount;
        OnResourceUpdated(ResourceType, *CurrentAmount);
    }
    else
    {
        ResourceInventory.Add(ResourceType, Amount);
        OnResourceUpdated(ResourceType, Amount);
    }
}

int32 UMingBuildingManager::GetResourceAmount(const FString& ResourceType) const
{
    const int32* Amount = ResourceInventory.Find(ResourceType);
    return Amount ? *Amount : 0;
}

void UMingBuildingManager::OnBuildingConstructed(const FString& BuildingType)
{
    UE_LOG(LogTemp, Verbose, TEXT("Building constructed: %s"), *BuildingType);
    
    // 發布資源更新事件通知其他層
    int32 CurrentMaterials = GetResourceAmount(TEXT("Materials"));
    FResourceUpdateEvent Event(TEXT("Materials"), -100, CurrentMaterials);
    IMingCoreEventBus::PublishEvent(Event);
}

void UMingBuildingManager::OnResourceUpdated(const FString& ResourceType, int32 NewAmount)
{
    UE_LOG(LogTemp, Verbose, TEXT("Resource %s updated: %d"), *ResourceType, NewAmount);
}
