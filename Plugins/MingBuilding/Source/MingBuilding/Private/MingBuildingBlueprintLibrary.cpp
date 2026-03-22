#include "MingBuildingBlueprintLibrary.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"

UMingBuildingManager* UMingBuildingBlueprintLibrary::GetBuildingManager()
{
    // 這裡應該從遊戲實例或單例獲取建築管理器
    // 簡化實作
    static UMingBuildingManager* ManagerInstance = nullptr;
    if (!ManagerInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            ManagerInstance = NewObject<UMingBuildingManager>(World);
            ManagerInstance->Initialize();
        }
    }
    
    return ManagerInstance;
}

void UMingBuildingBlueprintLibrary::InitializeBuildingSystem()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        Manager->Initialize();
        UE_LOG(LogTemp, Log, TEXT("Building system initialized from Blueprint"));
    }
}

FString UMingBuildingBlueprintLibrary::PlaceBuilding(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        UE_LOG(LogTemp, Error, TEXT("Building manager not available"));
        return TEXT("");
    }
    
    FBuildingPlacementResult Result = Manager->PlaceBuilding(BuildingType, Location, Rotation);
    
    if (Result.bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Building placed: %s"), *Result.BuildingID);
        return Result.BuildingID;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to place building: %s"), *Result.ErrorMessage);
        return TEXT("");
    }
}

bool UMingBuildingBlueprintLibrary::StartConstruction(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->StartConstruction(BuildingID);
}

bool UMingBuildingBlueprintLibrary::CancelConstruction(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->CancelConstruction(BuildingID);
}

FString UMingBuildingBlueprintLibrary::AddToConstructionQueue(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TEXT("");
    }
    
    return Manager->AddToConstructionQueue(BuildingType, Location, Rotation);
}

bool UMingBuildingBlueprintLibrary::RemoveFromConstructionQueue(const FString& QueueID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->RemoveFromConstructionQueue(QueueID);
}

AMingBuildingActor* UMingBuildingBlueprintLibrary::GetBuilding(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return nullptr;
    }
    
    return Manager->GetBuilding(BuildingID);
}

TArray<AMingBuildingActor*> UMingBuildingBlueprintLibrary::GetAllBuildings()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TArray<AMingBuildingActor*>();
    }
    
    return Manager->GetAllBuildings();
}

TArray<AMingBuildingActor*> UMingBuildingBlueprintLibrary::GetBuildingsByType(EMingBuildingType BuildingType)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TArray<AMingBuildingActor*>();
    }
    
    return Manager->GetBuildingsByType(BuildingType);
}

TArray<FBuildingQueueItem> UMingBuildingBlueprintLibrary::GetConstructionQueue()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TArray<FBuildingQueueItem>();
    }
    
    return Manager->GetConstructionQueue();
}

FBuildingStatistics UMingBuildingBlueprintLibrary::GetBuildingStatistics()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return FBuildingStatistics();
    }
    
    return Manager->GetBuildingStatistics();
}

bool UMingBuildingBlueprintLibrary::IsValidPlacementLocation(EMingBuildingType BuildingType, const FVector& Location)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->IsValidPlacementLocation(BuildingType, Location);
}

FVector UMingBuildingBlueprintLibrary::GetBuildingGridSize(EMingBuildingType BuildingType)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return FVector(300.0f, 300.0f, 150.0f);
    }
    
    return Manager->GetBuildingGridSize(BuildingType);
}

void UMingBuildingBlueprintLibrary::ShowPlacementPreview(EMingBuildingType BuildingType, const FVector& Location, const FRotator& Rotation)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        Manager->ShowPlacementPreview(BuildingType, Location, Rotation);
    }
}

void UMingBuildingBlueprintLibrary::HidePlacementPreview()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        Manager->HidePlacementPreview();
    }
}

bool UMingBuildingBlueprintLibrary::AssignWorker(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->AssignWorker(BuildingID);
}

bool UMingBuildingBlueprintLibrary::RemoveWorker(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->RemoveWorker(BuildingID);
}

int32 UMingBuildingBlueprintLibrary::GetAvailableWorkers()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return 0;
    }
    
    return Manager->GetAvailableWorkers();
}

bool UMingBuildingBlueprintLibrary::UpgradeBuilding(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->UpgradeBuilding(BuildingID);
}

bool UMingBuildingBlueprintLibrary::DestroyBuilding(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->DestroyBuilding(BuildingID);
}

bool UMingBuildingBlueprintLibrary::RepairBuilding(const FString& BuildingID, int32 RepairAmount)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    return Manager->RepairBuilding(BuildingID, RepairAmount);
}

TArray<AMingBuildingActor*> UMingBuildingBlueprintLibrary::GetBuildingsInRadius(const FVector& Center, float Radius)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TArray<AMingBuildingActor*>();
    }
    
    return Manager->GetBuildingsInRadius(Center, Radius);
}

TArray<AMingBuildingActor*> UMingBuildingBlueprintLibrary::GetSupplyChain(const FString& BuildingID)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return TArray<AMingBuildingActor*>();
    }
    
    return Manager->GetSupplyChain(BuildingID);
}

FMingBuildingCost UMingBuildingBlueprintLibrary::GetBuildingCost(EMingBuildingType BuildingType)
{
    return AMingBuildingActor::GetBuildingCost(BuildingType);
}

bool UMingBuildingBlueprintLibrary::CanAffordBuilding(EMingBuildingType BuildingType)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (!Manager)
    {
        return false;
    }
    
    FMingBuildingCost BuildingCost = AMingBuildingActor::GetBuildingCost(BuildingType);
    
    for (const FMingResourceCost& ResourceCost : BuildingCost.ResourceCosts)
    {
        FString ResourceTypeStr = UEnum::GetValueAsString(ResourceCost.ResourceType);
        if (Manager->GetResourceAmount(ResourceTypeStr) < ResourceCost.Amount)
        {
            return false;
        }
    }
    
    return true;
}

FString UMingBuildingBlueprintLibrary::GetBuildingDescription(EMingBuildingType BuildingType)
{
    switch (BuildingType)
    {
    case EMingBuildingType::CommandCenter:
        return TEXT("指揮中心 - 基地的核心建築，提供基本控制功能");
    case EMingBuildingType::Barracks:
        return TEXT("兵營 - 訓練軍事單位，提供基本防禦能力");
    case EMingBuildingType::Factory:
        return TEXT("工廠 - 生產武器裝備和軍事物資");
    case EMingBuildingType::SupplyDepot:
        return TEXT("補給站 - 存儲和分配資源");
    case EMingBuildingType::ResourceGatherer:
        return TEXT("資源收集器 - 採集和處理原材料");
    case EMingBuildingType::DefenseTower:
        return TEXT("防禦塔 - 提供基地防禦和警戒");
    default:
        return TEXT("未知建築類型");
    }
}

FString UMingBuildingBlueprintLibrary::GetBuildingTypeName(EMingBuildingType BuildingType)
{
    return UEnum::GetValueAsString(BuildingType);
}

FString UMingBuildingBlueprintLibrary::GetBuildingStateName(EMingBuildingState BuildingState)
{
    return UEnum::GetValueAsString(BuildingState);
}

TArray<FString> UMingBuildingBlueprintLibrary::BatchConstructBuildings(const TArray<EMingBuildingType>& BuildingTypes, const TArray<FVector>& Locations, const TArray<FRotator>& Rotations)
{
    TArray<FString> BuildingIDs;
    
    int32 Count = FMath::Min(BuildingTypes.Num(), FMath::Min(Locations.Num(), Rotations.Num()));
    
    for (int32 i = 0; i < Count; ++i)
    {
        FString BuildingID = PlaceBuilding(BuildingTypes[i], Locations[i], Rotations[i]);
        if (!BuildingID.IsEmpty())
        {
            BuildingIDs.Add(BuildingID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch constructed %d buildings"), BuildingIDs.Num());
    return BuildingIDs;
}

TArray<bool> UMingBuildingBlueprintLibrary::BatchUpgradeBuildings(const TArray<FString>& BuildingIDs)
{
    TArray<bool> Results;
    
    for (const FString& BuildingID : BuildingIDs)
    {
        Results.Add(UpgradeBuilding(BuildingID));
    }
    
    return Results;
}

TArray<bool> UMingBuildingBlueprintLibrary::BatchRepairBuildings(const TArray<FString>& BuildingIDs, int32 RepairAmount)
{
    TArray<bool> Results;
    
    for (const FString& BuildingID : BuildingIDs)
    {
        Results.Add(RepairBuilding(BuildingID, RepairAmount));
    }
    
    return Results;
}

bool UMingBuildingBlueprintLibrary::IsBuildingSystemInitialized()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    return Manager != nullptr;
}

FString UMingBuildingBlueprintLibrary::GetBuildingSystemVersion()
{
    return TEXT("1.0.0");
}

void UMingBuildingBlueprintLibrary::ResetBuildingSystem()
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        Manager->Shutdown();
        Manager->Initialize();
        
        UE_LOG(LogTemp, Log, TEXT("Building system reset"));
    }
}

FVector UMingBuildingBlueprintLibrary::GetOptimalPlacementLocation(EMingBuildingType BuildingType, const FVector& ReferencePoint, float SearchRadius)
{
    // 簡化的最優位置算法
    FVector GridSize = GetBuildingGridSize(BuildingType);
    
    // 在搜索半徑內尋找最佳位置
    FVector BestLocation = ReferencePoint;
    float BestScore = -1.0f;
    
    for (float X = -SearchRadius; X <= SearchRadius; X += GridSize.X)
    {
        for (float Y = -SearchRadius; Y <= SearchRadius; Y += GridSize.Y)
        {
            FVector TestLocation = ReferencePoint + FVector(X, Y, 0);
            
            if (IsValidPlacementLocation(BuildingType, TestLocation))
            {
                // 計算位置分數（距離、周圍建築等）
                float DistanceScore = FMath::Clamp(1.0f - (TestLocation - ReferencePoint).Size() / SearchRadius, 0.0f, 1.0f);
                
                if (DistanceScore > BestScore)
                {
                    BestScore = DistanceScore;
                    BestLocation = TestLocation;
                }
            }
        }
    }
    
    return BestLocation;
}

FString UMingBuildingBlueprintLibrary::AutoConstructBuilding(EMingBuildingType BuildingType, const FVector& ReferencePoint)
{
    // 自動找到最佳位置並建造
    FVector OptimalLocation = GetOptimalPlacementLocation(BuildingType, ReferencePoint, 2000.0f);
    FRotator DefaultRotation = FRotator::ZeroRotator;
    
    return PlaceBuilding(BuildingType, OptimalLocation, DefaultRotation);
}

float UMingBuildingBlueprintLibrary::GetBuildingEfficiency(const FString& BuildingID)
{
    AMingBuildingActor* Building = GetBuilding(BuildingID);
    if (!Building)
    {
        return 0.0f;
    }
    
    // 基於工人數量計算效率
    float WorkerRatio = (float)Building->AssignedWorkers / Building->MaxWorkers;
    return FMath::Clamp(WorkerRatio, 0.0f, 1.0f);
}

float UMingBuildingBlueprintLibrary::GetBaseEfficiency()
{
    FBuildingStatistics Stats = GetBuildingStatistics();
    
    if (Stats.TotalBuildings == 0)
    {
        return 0.0f;
    }
    
    float TotalEfficiency = 0.0f;
    TArray<AMingBuildingActor*> AllBuildings = GetAllBuildings();
    
    for (AMingBuildingActor* Building : AllBuildings)
    {
        if (Building)
        {
            TotalEfficiency += GetBuildingEfficiency(Building->BuildingName);
        }
    }
    
    return TotalEfficiency / AllBuildings.Num();
}

TArray<EMingBuildingType> UMingBuildingBlueprintLibrary::GetBuildingRecommendations()
{
    TArray<EMingBuildingType> Recommendations;
    
    FBuildingStatistics Stats = GetBuildingStatistics();
    
    // 基於當前建築狀況推薦
    if (Stats.BuildingCounts.Find(EMingBuildingType::CommandCenter) == nullptr || *Stats.BuildingCounts.Find(EMingBuildingType::CommandCenter) == 0)
    {
        Recommendations.Add(EMingBuildingType::CommandCenter);
    }
    
    if (Stats.BuildingCounts.Find(EMingBuildingType::ResourceGatherer) == nullptr || *Stats.BuildingCounts.Find(EMingBuildingType::ResourceGatherer) < 2)
    {
        Recommendations.Add(EMingBuildingType::ResourceGatherer);
    }
    
    if (Stats.BuildingCounts.Find(EMingBuildingType::Barracks) == nullptr || *Stats.BuildingCounts.Find(EMingBuildingType::Barracks) == 0)
    {
        Recommendations.Add(EMingBuildingType::Barracks);
    }
    
    if (Stats.AvailableWorkers > 5 && Stats.BuildingCounts.Find(EMingBuildingType::Factory) == nullptr)
    {
        Recommendations.Add(EMingBuildingType::Factory);
    }
    
    return Recommendations;
}

void UMingBuildingBlueprintLibrary::SetBuildingSystemParameters(int32 TotalWorkers, int32 MaxConcurrentConstruction)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        Manager->TotalWorkers = TotalWorkers;
        Manager->MaxConcurrentConstruction = MaxConcurrentConstruction;
        
        UE_LOG(LogTemp, Log, TEXT("Building system parameters updated - Workers: %d, Max Concurrent: %d"), 
            TotalWorkers, MaxConcurrentConstruction);
    }
}

void UMingBuildingBlueprintLibrary::GetBuildingSystemParameters(int32& TotalWorkers, int32& MaxConcurrentConstruction)
{
    UMingBuildingManager* Manager = GetBuildingManager();
    if (Manager)
    {
        TotalWorkers = Manager->TotalWorkers;
        MaxConcurrentConstruction = Manager->MaxConcurrentConstruction;
    }
    else
    {
        TotalWorkers = 0;
        MaxConcurrentConstruction = 0;
    }
}
