#include "MingBuildingManager.h"
#include "MingCoreEventBus.h"
#include "Engine/World.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UMingBuildingManager::UMingBuildingManager()
    : bIsInitialized(false)
    , TotalWorkers(20)
    , MaxConcurrentConstruction(3)
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

    // 預分配容量
    Buildings.Reserve(100);
    ConstructionQueue.Reserve(20);

    SetupEventSubscriptions();
    bIsInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("MingBuildingManager initialized - Building system ready"));
    UE_LOG(LogTemp, Log, TEXT("Total Workers: %d, Max Concurrent Construction: %d"), 
        TotalWorkers, MaxConcurrentConstruction);
}

void UMingBuildingManager::Shutdown()
{
    if (!bIsInitialized)
    {
        return;
    }

    CleanupEventSubscriptions();
    CleanupPlacementPreview();
    
    Buildings.Empty();
    ConstructionQueue.Empty();
    BuildingIDMap.Empty();
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

// === 建築放置和建造 ===

FBuildingPlacementResult UMingBuildingManager::PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    FBuildingPlacementResult Result;
    
    if (!bIsInitialized)
    {
        Result.ErrorMessage = TEXT("BuildingManager not initialized");
        return Result;
    }

    // 檢查放置位置是否有效
    if (!IsValidPlacementLocation(BuildingType, Location))
    {
        Result.ErrorMessage = TEXT("Invalid placement location");
        return Result;
    }

    // 檢查資源是否足夠
    if (!HasEnoughResources(BuildingType))
    {
        Result.ErrorMessage = TEXT("Insufficient resources");
        return Result;
    }

    // 生成建築ID
    FString BuildingID = GenerateBuildingID();
    
    // 創建建築Actor
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
    if (!World)
    {
        Result.ErrorMessage = TEXT("Failed to get world context");
        return Result;
    }

    // 這裡簡化處理，實際應該根據BuildingType創建對應的建築
    AMingBuildingActor* NewBuilding = World->SpawnActor<AMingBuildingActor>(Location, Rotation);
    if (!NewBuilding)
    {
        Result.ErrorMessage = TEXT("Failed to spawn building");
        return Result;
    }

    // 設置建築屬性
    NewBuilding->BuildingType = BuildingType;
    NewBuilding->BuildingName = FString::Printf(TEXT("Building_%s"), *UEnum::GetValueAsString(BuildingType));
    
    // 添加到管理列表
    Buildings.Add(NewBuilding);
    BuildingIDMap.Add(BuildingID, NewBuilding);

    // 消耗資源
    if (!ConsumeConstructionResources(BuildingType))
    {
        // 如果資源消耗失敗，清理建築
        NewBuilding->Destroy();
        Buildings.Remove(NewBuilding);
        BuildingIDMap.Remove(BuildingID);
        
        Result.ErrorMessage = TEXT("Failed to consume resources");
        return Result;
    }

    Result.bSuccess = true;
    Result.PlacementLocation = Location;
    Result.BuildingID = BuildingID;

    UE_LOG(LogTemp, Log, TEXT("Building placed successfully: %s at (%f, %f, %f)"), 
        *BuildingID, Location.X, Location.Y, Location.Z);

    return Result;
}

bool UMingBuildingManager::StartConstruction(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        UE_LOG(LogTemp, Error, TEXT("Building not found: %s"), *BuildingID);
        return false;
    }

    if (Building->BuildingState != EMingBuildingState::Planned)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building %s is not in planned state"), *BuildingID);
        return false;
    }

    // 檢查是否有可用的工人
    if (GetAvailableWorkers() <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No available workers for construction"));
        return false;
    }

    // 分配工人並開始建造
    Building->StartConstruction();
    Building->AssignWorker();

    CurrentConstructionID = BuildingID;
    OnBuildingConstructionStarted(BuildingID);

    UE_LOG(LogTemp, Log, TEXT("Construction started for building: %s"), *BuildingID);
    return true;
}

bool UMingBuildingManager::CancelConstruction(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        UE_LOG(LogTemp, Error, TEXT("Building not found: %s"), *BuildingID);
        return false;
    }

    if (Building->BuildingState != EMingBuildingState::Constructing)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building %s is not under construction"), *BuildingID);
        return false;
    }

    // 取消建造並返還資源
    Building->CancelConstruction();
    RefundConstructionResources(Building->BuildingType);

    if (CurrentConstructionID == BuildingID)
    {
        CurrentConstructionID = TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Construction cancelled for building: %s"), *BuildingID);
    return true;
}

FString UMingBuildingManager::AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    FBuildingQueueItem QueueItem;
    QueueItem.BuildingType = BuildingType;
    QueueItem.Position = Location;
    QueueItem.Rotation = Rotation;
    QueueItem.QueueID = GenerateQueueID();
    QueueItem.Progress = 0.0f;
    QueueItem.bIsConstructing = false;

    // 計算預計完成時間
    FMingBuildingCost BuildingCost = AMingBuildingActor::GetBuildingCost(BuildingType);
    QueueItem.EstimatedCompletionTime = BuildingCost.BuildTime;

    ConstructionQueue.Add(QueueItem);

    UE_LOG(LogTemp, Log, TEXT("Added to construction queue: %s"), *QueueItem.QueueID);
    
    // 如果當前沒有在建項目，開始下一個
    if (CurrentConstructionID.IsEmpty())
    {
        StartNextConstruction();
    }

    return QueueItem.QueueID;
}

bool UMingBuildingManager::RemoveFromConstructionQueue(const FString& QueueID)
{
    for (int32 i = 0; i < ConstructionQueue.Num(); ++i)
    {
        if (ConstructionQueue[i].QueueID == QueueID)
        {
            // 如果正在建造，不能移除
            if (ConstructionQueue[i].bIsConstructing)
            {
                UE_LOG(LogTemp, Warning, TEXT("Cannot remove constructing item from queue: %s"), *QueueID);
                return false;
            }

            ConstructionQueue.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("Removed from construction queue: %s"), *QueueID);
            return true;
        }
    }

    return false;
}

// === 建築管理 ===

AMingBuildingActor* UMingBuildingManager::GetBuilding(const FString& BuildingID) const
{
    TObjectPtr<AMingBuildingActor>* BuildingPtr = BuildingIDMap.Find(BuildingID);
    return BuildingPtr ? *BuildingPtr : nullptr;
}

TArray<AMingBuildingActor*> UMingBuildingManager::GetAllBuildings() const
{
    return Buildings;
}

TArray<AMingBuildingActor*> UMingBuildingManager::GetBuildingsByType(EMingBuildingType BuildingType) const
{
    TArray<AMingBuildingActor*> Result;
    
    for (AMingBuildingActor* Building : Buildings)
    {
        if (Building && Building->BuildingType == BuildingType)
        {
            Result.Add(Building);
        }
    }
    
    return Result;
}

TArray<FBuildingQueueItem> UMingBuildingManager::GetConstructionQueue() const
{
    return ConstructionQueue;
}

FBuildingStatistics UMingBuildingManager::GetBuildingStatistics() const
{
    FBuildingStatistics Stats;
    
    Stats.TotalBuildings = Buildings.Num();
    Stats.TotalWorkers = TotalWorkers;
    Stats.QueueLength = ConstructionQueue.Num();
    
    // 計算各類型建築數量
    for (AMingBuildingActor* Building : Buildings)
    {
        if (!Building) continue;
        
        // 統計狀態
        switch (Building->BuildingState)
        {
        case EMingBuildingState::Active:
            Stats.ActiveBuildings++;
            break;
        case EMingBuildingState::Constructing:
            Stats.ConstructingBuildings++;
            break;
        case EMingBuildingState::Damaged:
            Stats.DamagedBuildings++;
            break;
        default:
            break;
        }
        
        // 統計類型
        int32* Count = Stats.BuildingCounts.Find(Building->BuildingType);
        if (Count)
        {
            (*Count)++;
        }
        else
        {
            Stats.BuildingCounts.Add(Building->BuildingType, 1);
        }
        
        // 統計工人
        Stats.AvailableWorkers += Building->MaxWorkers - Building->AssignedWorkers;
    }
    
    return Stats;
}

// === 建築放置驗證 ===

bool UMingBuildingManager::IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location) const
{
    // 檢查碰撞
    if (!CheckBuildingCollision(BuildingType, Location))
    {
        return false;
    }
    
    // 檢查地形適應性
    if (!IsTerrainSuitable(BuildingType, Location))
    {
        return false;
    }
    
    return true;
}

FVector UMingBuildingManager::GetBuildingGridSize(EMingBuildingType BuildingType) const
{
    // 根據建築類型返回網格大小
    switch (BuildingType)
    {
    case EMingBuildingType::CommandCenter:
        return FVector(400.0f, 400.0f, 200.0f);
    case EMingBuildingType::Barracks:
        return FVector(300.0f, 300.0f, 150.0f);
    case EMingBuildingType::Factory:
        return FVector(350.0f, 350.0f, 180.0f);
    case EMingBuildingType::SupplyDepot:
        return FVector(250.0f, 250.0f, 120.0f);
    case EMingBuildingType::ResourceGatherer:
        return FVector(200.0f, 200.0f, 100.0f);
    case EMingBuildingType::DefenseTower:
        return FVector(150.0f, 150.0f, 200.0f);
    default:
        return FVector(300.0f, 300.0f, 150.0f);
    }
}

void UMingBuildingManager::ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    // 清理之前的預覽
    CleanupPlacementPreview();
    
    UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
    if (!World) return;
    
    // 創建預覽網格
    PlacementPreview = World->SpawnActor<UStaticMeshActor>(Location, Rotation);
    if (PlacementPreview)
    {
        UStaticMeshComponent* MeshComp = PlacementPreview->GetStaticMeshComponent();
        if (MeshComp)
        {
            // 設置半透明材質
            MeshComp->SetVisibility(true);
            MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        }
    }
}

void UMingBuildingManager::HidePlacementPreview()
{
    CleanupPlacementPreview();
}

// === 工人管理 ===

bool UMingBuildingManager::AssignWorker(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return false;
    }
    
    if (GetAvailableWorkers() <= 0)
    {
        return false;
    }
    
    Building->AssignWorker();
    return true;
}

bool UMingBuildingManager::RemoveWorker(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return false;
    }
    
    Building->RemoveWorker();
    return true;
}

int32 UMingBuildingManager::GetAvailableWorkers() const
{
    int32 AssignedWorkers = 0;
    
    for (AMingBuildingActor* Building : Buildings)
    {
        if (Building)
        {
            AssignedWorkers += Building->AssignedWorkers;
        }
    }
    
    return TotalWorkers - AssignedWorkers;
}

// === 建築升級和維護 ===

bool UMingBuildingManager::UpgradeBuilding(const FString& BuildingID)
{
    UE_LOG(LogTemp, Log, TEXT("Upgrading building %s"), *BuildingID);
    // TODO: 實現升級邏輯
    return true;
}

bool UMingBuildingManager::DestroyBuilding(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return false;
    }
    
    Building->DestroyBuilding();
    
    // 從管理列表移除
    Buildings.Remove(Building);
    BuildingIDMap.Remove(BuildingID);
    
    OnBuildingDestroyed(BuildingID);
    
    UE_LOG(LogTemp, Log, TEXT("Building destroyed: %s"), *BuildingID);
    return true;
}

bool UMingBuildingManager::RepairBuilding(const FString& BuildingID, int32 RepairAmount)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return false;
    }
    
    Building->Repair(RepairAmount);
    return true;
}

// === 建築查詢 ===

TArray<AMingBuildingActor*> UMingBuildingManager::GetBuildingsInRadius(const FVector& Center, float Radius) const
{
    TArray<AMingBuildingActor*> Result;
    
    for (AMingBuildingActor* Building : Buildings)
    {
        if (Building && FVector::Dist(Building->GetActorLocation(), Center) <= Radius)
        {
            Result.Add(Building);
        }
    }
    
    return Result;
}

TArray<AMingBuildingActor*> UMingBuildingManager::GetSupplyChain(const FString& BuildingID) const
{
    // 簡化實現：返回附近的供應建築
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return TArray<AMingBuildingActor*>();
    }
    
    return GetBuildingsInRadius(Building->GetActorLocation(), 1000.0f);
}

// === 舊版介面 (兼容性) ===

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

// === 資源管理 ===

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

// === 事件處理 ===

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

// === 新增的內部方法實現 ===

void UMingBuildingManager::ProcessConstructionQueue(float DeltaTime)
{
    // 處理建造隊列
    if (CurrentConstructionID.IsEmpty() && ConstructionQueue.Num() > 0)
    {
        StartNextConstruction();
    }
}

void UMingBuildingManager::StartNextConstruction()
{
    for (FBuildingQueueItem& QueueItem : ConstructionQueue)
    {
        if (!QueueItem.bIsConstructing)
        {
            // 放置建築並開始建造
            FBuildingPlacementResult Result = PlaceBuilding(QueueItem.BuildingType, QueueItem.Position, QueueItem.Rotation);
            if (Result.bSuccess)
            {
                QueueItem.bIsConstructing = true;
                StartConstruction(Result.BuildingID);
                break;
            }
        }
    }
}

FString UMingBuildingManager::GenerateBuildingID()
{
    return FString::Printf(TEXT("Building_%d_%d"), 
        FDateTime::Now().GetMillisecond(), 
        FMath::RandRange(1000, 9999));
}

FString UMingBuildingManager::GenerateQueueID()
{
    return FString::Printf(TEXT("Queue_%d_%d"), 
        FDateTime::Now().GetMillisecond(), 
        FMath::RandRange(1000, 9999));
}

bool UMingBuildingManager::HasEnoughResources(EMingBuildingType BuildingType) const
{
    FMingBuildingCost BuildingCost = AMingBuildingActor::GetBuildingCost(BuildingType);
    
    for (const FMingResourceCost& ResourceCost : BuildingCost.ResourceCosts)
    {
        if (GetResourceAmount(UEnum::GetValueAsString(ResourceCost.ResourceType)) < ResourceCost.Amount)
        {
            return false;
        }
    }
    
    return true;
}

bool UMingBuildingManager::ConsumeConstructionResources(EMingBuildingType BuildingType)
{
    FMingBuildingCost BuildingCost = AMingBuildingActor::GetBuildingCost(BuildingType);
    
    for (const FMingResourceCost& ResourceCost : BuildingCost.ResourceCosts)
    {
        FString ResourceTypeStr = UEnum::GetValueAsString(ResourceCost.ResourceType);
        int32 CurrentAmount = GetResourceAmount(ResourceTypeStr);
        
        if (CurrentAmount < ResourceCost.Amount)
        {
            return false;
        }
        
        AddResource(ResourceTypeStr, -ResourceCost.Amount);
    }
    
    return true;
}

void UMingBuildingManager::RefundConstructionResources(EMingBuildingType BuildingType)
{
    FMingBuildingCost BuildingCost = AMingBuildingActor::GetBuildingCost(BuildingType);
    
    for (const FMingResourceCost& ResourceCost : BuildingCost.ResourceCosts)
    {
        FString ResourceTypeStr = UEnum::GetValueAsString(ResourceCost.ResourceType);
        AddResource(ResourceTypeStr, ResourceCost.Amount);
    }
}

void UMingBuildingManager::UpdateBuildingStatistics()
{
    // 更新統計信息
    FBuildingStatistics Stats = GetBuildingStatistics();
    
    UE_LOG(LogTemp, Verbose, TEXT("Building Stats - Total: %d, Active: %d, Constructing: %d"),
        Stats.TotalBuildings, Stats.ActiveBuildings, Stats.ConstructingBuildings);
}

void UMingBuildingManager::OnBuildingConstructionStarted(const FString& BuildingID)
{
    UE_LOG(LogTemp, Log, TEXT("Construction started: %s"), *BuildingID);
}

void UMingBuildingManager::OnBuildingConstructionCompleted(const FString& BuildingID)
{
    UE_LOG(LogTemp, Log, TEXT("Construction completed: %s"), *BuildingID);
    
    CurrentConstructionID = TEXT("");
    StartNextConstruction();
}

void UMingBuildingManager::OnBuildingDestroyed(const FString& BuildingID)
{
    UE_LOG(LogTemp, Log, TEXT("Building destroyed: %s"), *BuildingID);
}

void UMingBuildingManager::SaveBuildingData()
{
    // TODO: 實現建築數據保存
}

void UMingBuildingManager::LoadBuildingData()
{
    // TODO: 實現建築數據加載
}

void UMingBuildingManager::CleanupPlacementPreview()
{
    if (PlacementPreview)
    {
        PlacementPreview->Destroy();
        PlacementPreview = nullptr;
    }
}

bool UMingBuildingManager::CheckBuildingCollision(EMingBuildingType BuildingType, const FVector& Location) const
{
    FVector GridSize = GetBuildingGridSize(BuildingType);
    FBox BuildingBox = FBox::BuildAABB(Location - GridSize * 0.5f, Location + GridSize * 0.5f);
    
    // 檢查與現有建築的碰撞
    for (AMingBuildingActor* Building : Buildings)
    {
        if (!Building) continue;
        
        FVector OtherGridSize = GetBuildingGridSize(Building->BuildingType);
        FBox OtherBox = FBox::BuildAABB(
            Building->GetActorLocation() - OtherGridSize * 0.5f,
            Building->GetActorLocation() + OtherGridSize * 0.5f
        );
        
        if (BuildingBox.Intersect(OtherBox))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingBuildingManager::IsTerrainSuitable(EMingBuildingType BuildingType, const FVector& Location) const
{
    // 簡化實現：假設所有地形都適合
    // 實際實作應該檢查地形類型、坡度等
    return true;
}

float UMingBuildingManager::CalculateBuildingEfficiency(AMingBuildingActor* Building) const
{
    if (!Building) return 0.0f;
    
    // 基於工人數量計算效率
    float WorkerRatio = (float)Building->AssignedWorkers / Building->MaxWorkers;
    return FMath::Clamp(WorkerRatio, 0.0f, 1.0f);
}

void UMingBuildingManager::UpdateSupplyChains()
{
    // TODO: 實現供應鏈更新邏輯
}

void UMingBuildingManager::ProcessBuildingUpgrades(float DeltaTime)
{
    // TODO: 實現建築升級處理邏輯
}
