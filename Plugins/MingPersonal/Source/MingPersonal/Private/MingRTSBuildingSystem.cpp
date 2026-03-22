#include "MingRTSBuildingSystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"
#include "MingRTSEconomicSystem.h"

UMingRTSBuildingSystem::UMingRTSBuildingSystem()
{
    CurrentWorld = nullptr;
    EconomicSystem = nullptr;
    BuildingUpdateInterval = 0.1f; // 每100ms更新
    LastBuildingUpdateTime = 0.0f;
    GridSize = 100.0f; // 100單位網格
    MaxBuildingCount = 100;
    DefaultInfluenceRange = 200.0f;
    ConstructionSpeedMultiplier = 1.0f;
    UpgradeSpeedMultiplier = 1.0f;
    MaintenanceCostMultiplier = 1.0f;
}

void UMingRTSBuildingSystem::InitializeBuildingSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSBuildingSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    LastBuildingUpdateTime = World->GetTimeSeconds();
    
    // 初始化建築模板
    InitializeBuildingTemplates();
    
    // 清理建築網格
    BuildingGrid.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSBuildingSystem initialized successfully"));
}

void UMingRTSBuildingSystem::UpdateBuildingSystem(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    float CurrentTime = CurrentWorld->GetTimeSeconds();
    
    // 定期更新建築
    if (CurrentTime - LastBuildingUpdateTime >= BuildingUpdateInterval)
    {
        // 更新所有建築
        for (TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
        {
            AActor* BuildingActor = BuildingPair.Key;
            FBuildingData& BuildingData = BuildingPair.Value;
            
            if (!BuildingActor || BuildingActor->IsPendingKill())
            {
                continue;
            }
            
            // 根據狀態更新建築
            switch (BuildingData.CurrentState)
            {
                case EBuildingState::UnderConstruction:
                    UpdateBuildingConstruction(BuildingActor, DeltaTime);
                    break;
                    
                case EBuildingState::Upgrading:
                    UpdateBuildingUpgrade(BuildingActor, DeltaTime);
                    break;
                    
                case EBuildingState::Operational:
                    ProcessBuildingProduction(BuildingActor, DeltaTime);
                    ProcessBuildingMaintenance(BuildingActor, DeltaTime);
                    break;
                    
                case EBuildingState::Damaged:
                    ProcessBuildingMaintenance(BuildingActor, DeltaTime);
                    CheckBuildingIntegrity(BuildingActor);
                    break;
                    
                default:
                    break;
            }
            
            // 應用建築效果
            ApplyBuildingEffects(BuildingActor);
        }
        
        // 清理無效建築
        CleanupInvalidBuildings();
        
        // 處理建築交互
        ProcessBuildingInteractions();
        
        LastBuildingUpdateTime = CurrentTime;
    }
}

bool UMingRTSBuildingSystem::ConstructBuilding(EBuildingType BuildingType, FVector Location, FRotator Rotation)
{
    if (!CurrentWorld)
    {
        return false;
    }
    
    // 檢查是否可以建造
    if (!CanPlaceBuilding(BuildingType, Location))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot place building at location: %s"), *Location.ToString());
        return false;
    }
    
    // 檢查資源是否足夠
    if (!HasEnoughResourcesForBuilding(BuildingType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough resources to build: %s"), *GetBuildingName(BuildingType));
        return false;
    }
    
    // 檢查建築依賴
    if (!CheckBuildingDependencies(BuildingType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Building dependencies not met: %s"), *GetBuildingName(BuildingType));
        return false;
    }
    
    // 檢查建築數量限制
    if (BuildingDataMap.Num() >= MaxBuildingCount)
    {
        UE_LOG(LogTemp, Warning, TEXT("Maximum building count reached"));
        return false;
    }
    
    // 消耗資源
    FBuildingCost Cost = GetBuildingCost(BuildingType);
    for (TPair<EResourceType, float>& ResourceCost : Cost.RequiredResources)
    {
        if (EconomicSystem)
        {
            EconomicSystem->ConsumeResource(ResourceCost.Key, ResourceCost.Value);
        }
    }
    
    // 創建建築Actor
    AActor* BuildingActor = CreateBuildingActor(BuildingType, Location, Rotation);
    if (!BuildingActor)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create building actor"));
        return false;
    }
    
    // 初始化建築數據
    FBuildingData BuildingData;
    BuildingData.BuildingType = BuildingType;
    BuildingData.CurrentState = EBuildingState::UnderConstruction;
    BuildingData.CurrentLevel = EBuildingUpgrade::Level1;
    BuildingData.Location = Location;
    BuildingData.Rotation = Rotation;
    BuildingData.ConstructionProgress = 0.0f;
    BuildingData.LastProductionTime = CurrentWorld->GetTimeSeconds();
    
    // 從模板獲取基礎數據
    FBuildingTemplate Template = GetBuildingTemplate(BuildingType);
    BuildingData.MaxHealth = Template.BaseHealth;
    BuildingData.Health = BuildingData.MaxHealth;
    BuildingData.MaxWorkers = Template.MaxWorkerCapacity;
    BuildingData.bIsPowered = !Template.bRequiresPower;
    BuildingData.ProducedResources = Template.ProducedResources;
    BuildingData.ConsumedResources = Template.ConsumedResources;
    BuildingData.ProductionRates = Template.BaseProductionRates;
    
    // 添加到建築映射
    BuildingDataMap.Add(BuildingActor, BuildingData);
    
    // 設置網格位置
    FVector2D GridPosition = GetGridCoordinates(Location);
    SetGridPosition(GridPosition, BuildingActor);
    
    // 觸發建造開始事件
    OnBuildingConstructed.Broadcast(BuildingData);
    
    UE_LOG(LogTemp, Log, TEXT("Started construction of %s at %s"), 
        *GetBuildingName(BuildingType), *Location.ToString());
    
    return true;
}

bool UMingRTSBuildingSystem::DestroyBuilding(AActor* BuildingActor)
{
    if (!BuildingActor || !BuildingDataMap.Contains(BuildingActor))
    {
        return false;
    }
    
    FBuildingData BuildingData = BuildingDataMap[BuildingActor];
    
    // 清除網格位置
    FVector2D GridPosition = GetGridCoordinates(BuildingData.Location);
    ClearGridPosition(GridPosition);
    
    // 從映射中移除
    BuildingDataMap.Remove(BuildingActor);
    
    // 摧毀Actor
    if (CurrentWorld)
    {
        CurrentWorld->DestroyActor(BuildingActor);
    }
    
    // 觸發摧毀事件
    OnBuildingDestroyed.Broadcast(BuildingData);
    
    UE_LOG(LogTemp, Log, TEXT("Destroyed building: %s"), *GetBuildingName(BuildingData.BuildingType));
    
    return true;
}

bool UMingRTSBuildingSystem::UpgradeBuilding(AActor* BuildingActor)
{
    if (!BuildingActor || !BuildingDataMap.Contains(BuildingActor))
    {
        return false;
    }
    
    FBuildingData& BuildingData = BuildingDataMap[BuildingActor];
    
    // 檢查是否可以升級
    if (BuildingData.CurrentState != EBuildingState::Operational)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot upgrade non-operational building"));
        return false;
    }
    
    // 檢查是否已達最高級別
    if (BuildingData.CurrentLevel >= EBuildingUpgrade::Level5)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building already at maximum level"));
        return false;
    }
    
    // 獲取升級成本
    EBuildingUpgrade NextLevel = (EBuildingUpgrade)((int32)BuildingData.CurrentLevel + 1);
    FBuildingCost UpgradeCost = GetBuildingCost(BuildingData.BuildingType, NextLevel);
    
    // 檢查資源是否足夠
    if (!HasEnoughResourcesForBuilding(BuildingData.BuildingType))
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough resources for upgrade"));
        return false;
    }
    
    // 消耗資源
    for (TPair<EResourceType, float>& ResourceCost : UpgradeCost.RequiredResources)
    {
        if (EconomicSystem)
        {
            EconomicSystem->ConsumeResource(ResourceCost.Key, ResourceCost.Value);
        }
    }
    
    // 開始升級
    BuildingData.CurrentState = EBuildingState::Upgrading;
    BuildingData.ConstructionProgress = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Started upgrading %s to level %d"), 
        *GetBuildingName(BuildingData.BuildingType), (int32)NextLevel);
    
    return true;
}

bool UMingRTSBuildingSystem::RepairBuilding(AActor* BuildingActor, float RepairAmount)
{
    if (!BuildingActor || !BuildingDataMap.Contains(BuildingActor))
    {
        return false;
    }
    
    FBuildingData& BuildingData = BuildingDataMap[BuildingActor];
    
    if (BuildingData.Health >= BuildingData.MaxHealth)
    {
        return false;
    }
    
    float OldHealth = BuildingData.Health;
    BuildingData.Health = FMath::Min(BuildingData.Health + RepairAmount, BuildingData.MaxHealth);
    
    // 如果完全修復，改變狀態
    if (BuildingData.Health >= BuildingData.MaxHealth && BuildingData.CurrentState == EBuildingState::Damaged)
    {
        BuildingData.CurrentState = EBuildingState::Operational;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Repaired building: %s (%.1f -> %.1f)"), 
        *GetBuildingName(BuildingData.BuildingType), OldHealth, BuildingData.Health);
    
    return true;
}

void UMingRTSBuildingSystem::DamageBuilding(AActor* BuildingActor, float Damage)
{
    if (!BuildingActor || !BuildingDataMap.Contains(BuildingActor))
    {
        return;
    }
    
    FBuildingData& BuildingData = BuildingDataMap[BuildingActor];
    
    float OldHealth = BuildingData.Health;
    BuildingData.Health = FMath::Max(0.0f, BuildingData.Health - Damage);
    
    // 觸發損壞事件
    OnBuildingDamaged.Broadcast(BuildingData, Damage);
    
    // 檢查是否被摧毀
    if (BuildingData.Health <= 0.0f)
    {
        BuildingData.CurrentState = EBuildingState::Destroyed;
        DestroyBuilding(BuildingActor);
    }
    else if (BuildingData.CurrentState == EBuildingState::Operational)
    {
        BuildingData.CurrentState = EBuildingState::Damaged;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Building damaged: %s (%.1f -> %.1f)"), 
        *GetBuildingName(BuildingData.BuildingType), OldHealth, BuildingData.Health);
}

FBuildingData UMingRTSBuildingSystem::GetBuildingData(AActor* BuildingActor) const
{
    const FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (BuildingData)
    {
        return *BuildingData;
    }
    return FBuildingData();
}

TArray<AActor*> UMingRTSBuildingSystem::GetAllBuildings() const
{
    TArray<AActor*> AllBuildings;
    for (const TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
    {
        if (BuildingPair.Key && !BuildingPair.Key->IsPendingKill())
        {
            AllBuildings.Add(BuildingPair.Key);
        }
    }
    return AllBuildings;
}

TArray<AActor*> UMingRTSBuildingSystem::GetBuildingsByType(EBuildingType BuildingType) const
{
    TArray<AActor*> BuildingsByType;
    for (const TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
    {
        if (BuildingPair.Key && !BuildingPair.Key->IsPendingKill() && 
            BuildingPair.Value.BuildingType == BuildingType)
        {
            BuildingsByType.Add(BuildingPair.Key);
        }
    }
    return BuildingsByType;
}

bool UMingRTSBuildingSystem::CanPlaceBuilding(EBuildingType BuildingType, FVector Location) const
{
    // 檢查網格位置是否被佔用
    FVector2D GridPosition = GetGridCoordinates(Location);
    if (IsGridPositionOccupied(GridPosition))
    {
        return false;
    }
    
    // 檢查地形是否適合建造
    // 這裡可以添加地形檢測邏輯
    
    return true;
}

FBuildingCost UMingRTSBuildingSystem::GetBuildingCost(EBuildingType BuildingType, EBuildingUpgrade Level) const
{
    FBuildingTemplate Template = GetBuildingTemplate(BuildingType);
    FBuildingCost Cost = Template.BaseCost;
    
    // 應用級別倍數
    float LevelMultiplier = CalculateBuildingCostMultiplier(Level);
    
    // 調整成本
    for (TPair<EResourceType, float>& ResourceCost : Cost.RequiredResources)
    {
        ResourceCost.Value *= LevelMultiplier;
    }
    
    Cost.ConstructionTime *= LevelMultiplier;
    Cost.MaintenanceCost *= LevelMultiplier;
    
    return Cost;
}

bool UMingRTSBuildingSystem::HasEnoughResourcesForBuilding(EBuildingType BuildingType) const
{
    if (!EconomicSystem)
    {
        return false;
    }
    
    FBuildingCost Cost = GetBuildingCost(BuildingType);
    
    for (TPair<EResourceType, float>& ResourceCost : Cost.RequiredResources)
    {
        if (!EconomicSystem->HasEnoughResource(ResourceCost.Key, ResourceCost.Value))
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRTSBuildingSystem::SetBuildingWorkers(AActor* BuildingActor, int32 WorkerCount)
{
    if (!BuildingActor || !BuildingDataMap.Contains(BuildingActor))
    {
        return false;
    }
    
    FBuildingData& BuildingData = BuildingDataMap[BuildingActor];
    
    WorkerCount = FMath::Clamp(WorkerCount, 0, BuildingData.MaxWorkers);
    BuildingData.WorkerCount = WorkerCount;
    
    // 更新生產效率
    if (BuildingData.MaxWorkers > 0)
    {
        BuildingData.ProductionEfficiency = (float)WorkerCount / BuildingData.MaxWorkers;
    }
    else
    {
        BuildingData.ProductionEfficiency = 0.0f;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set %d workers for %s (efficiency: %.2f)"), 
        WorkerCount, *GetBuildingName(BuildingData.BuildingType), BuildingData.ProductionEfficiency);
    
    return true;
}

FBuildingTemplate UMingRTSBuildingSystem::GetBuildingTemplate(EBuildingType BuildingType) const
{
    const FBuildingTemplate* Template = BuildingTemplates.Find(BuildingType);
    if (Template)
    {
        return *Template;
    }
    
    // 返回默認模板
    FBuildingTemplate DefaultTemplate;
    DefaultTemplate.BuildingType = BuildingType;
    DefaultTemplate.BuildingName = GetBuildingName(BuildingType);
    DefaultTemplate.Description = GetBuildingDescription(BuildingType);
    return DefaultTemplate;
}

TArray<FBuildingTemplate> UMingRTSBuildingSystem::GetAllBuildingTemplates() const
{
    TArray<FBuildingTemplate> AllTemplates;
    for (const TPair<EBuildingType, FBuildingTemplate>& TemplatePair : BuildingTemplates)
    {
        AllTemplates.Add(TemplatePair.Value);
    }
    return AllTemplates;
}

float UMingRTSBuildingSystem::CalculateBuildingEfficiency(AActor* BuildingActor) const
{
    const FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return 0.0f;
    }
    
    float Efficiency = BuildingData->ProductionEfficiency;
    
    // 應用電力加成
    if (BuildingData->bIsPowered)
    {
        Efficiency *= 1.2f;
    }
    
    // 應用影響加成
    Efficiency *= GetBuildingInfluenceBonus(BuildingActor);
    
    // 應用升級加成
    Efficiency *= CalculateBuildingProductionMultiplier(BuildingData->CurrentLevel);
    
    return FMath::Clamp(Efficiency, 0.0f, 2.0f);
}

TMap<EResourceType, float> UMingRTSBuildingSystem::GetBuildingProductionRates(AActor* BuildingActor) const
{
    TMap<EResourceType, float> ProductionRates;
    
    const FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return ProductionRates;
    }
    
    float Efficiency = CalculateBuildingEfficiency(BuildingActor);
    
    for (TPair<EResourceType, float>& ProductionRate : BuildingData->ProductionRates)
    {
        ProductionRates.Add(ProductionRate.Key, ProductionRate.Value * Efficiency);
    }
    
    return ProductionRates;
}

void UMingRTSBuildingSystem::ProcessBuildingProduction(AActor* BuildingActor, float DeltaTime)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData || !EconomicSystem)
    {
        return;
    }
    
    if (BuildingData->CurrentState != EBuildingState::Operational)
    {
        return;
    }
    
    float CurrentTime = CurrentWorld->GetTimeSeconds();
    float TimeSinceLastProduction = CurrentTime - BuildingData->LastProductionTime;
    
    // 每秒生產一次
    if (TimeSinceLastProduction >= 1.0f)
    {
        TMap<EResourceType, float> ProductionRates = GetBuildingProductionRates(BuildingActor);
        
        // 處理生產
        for (TPair<EResourceType, float>& ProductionRate : ProductionRates)
        {
            if (ProductionRate.Value > 0.0f)
            {
                EconomicSystem->AddResource(ProductionRate.Key, ProductionRate.Value);
                OnResourceProduced.Broadcast(ProductionRate.Key, ProductionRate.Value);
            }
            else if (ProductionRate.Value < 0.0f)
            {
                EconomicSystem->ConsumeResource(ProductionRate.Key, FMath::Abs(ProductionRate.Value));
            }
        }
        
        BuildingData->LastProductionTime = CurrentTime;
    }
}

bool UMingRTSBuildingSystem::CheckBuildingDependencies(EBuildingType BuildingType) const
{
    FBuildingTemplate Template = GetBuildingTemplate(BuildingType);
    
    for (EBuildingType RequiredBuilding : Template.RequiredBuildings)
    {
        TArray<AActor*> RequiredBuildings = GetBuildingsByType(RequiredBuilding);
        bool bHasOperationalBuilding = false;
        
        for (AActor* Building : RequiredBuildings)
        {
            const FBuildingData* BuildingData = BuildingDataMap.Find(Building);
            if (BuildingData && BuildingData->CurrentState == EBuildingState::Operational)
            {
                bHasOperationalBuilding = true;
                break;
            }
        }
        
        if (!bHasOperationalBuilding)
        {
            return false;
        }
    }
    
    return true;
}

float UMingRTSBuildingSystem::GetBuildingInfluenceRange(AActor* BuildingActor) const
{
    const FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return DefaultInfluenceRange;
    }
    
    // 根據建築類型和級別調整影響範圍
    float Range = DefaultInfluenceRange;
    
    switch (BuildingData->BuildingType)
    {
        case EBuildingType::Palace:
            Range *= 2.0f;
            break;
        case EBuildingType::Temple:
            Range *= 1.5f;
            break;
        case EBuildingType::Market:
            Range *= 1.3f;
            break;
        default:
            break;
    }
    
    // 升級增加影響範圍
    Range *= (1.0f + (int32)BuildingData->CurrentLevel * 0.2f);
    
    return Range;
}

TArray<AActor*> UMingRTSBuildingSystem::GetBuildingsInInfluenceRange(AActor* BuildingActor) const
{
    TArray<AActor*> BuildingsInRange;
    
    const FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return BuildingsInRange;
    }
    
    float InfluenceRange = GetBuildingInfluenceRange(BuildingActor);
    
    for (const TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
    {
        if (BuildingPair.Key == BuildingActor || !BuildingPair.Key)
        {
            continue;
        }
        
        float Distance = FVector::Dist(BuildingData->Location, BuildingPair.Value.Location);
        if (Distance <= InfluenceRange)
        {
            BuildingsInRange.Add(BuildingPair.Key);
        }
    }
    
    return BuildingsInRange;
}

void UMingRTSBuildingSystem::InitializeBuildingTemplates()
{
    BuildingTemplates.Empty();
    
    // 房屋
    FBuildingTemplate HouseTemplate;
    HouseTemplate.BuildingType = EBuildingType::House;
    HouseTemplate.BuildingName = TEXT("House");
    HouseTemplate.Description = TEXT("Basic housing for population");
    HouseTemplate.BaseCost.RequiredResources.Add(EResourceType::Wood, 50.0f);
    HouseTemplate.BaseCost.RequiredResources.Add(EResourceType::Stone, 25.0f);
    HouseTemplate.BaseCost.ConstructionTime = 5.0f;
    HouseTemplate.BaseCost.MaintenanceCost = 0.5f;
    HouseTemplate.MaxWorkerCapacity = 0;
    HouseTemplate.BaseHealth = 100.0f;
    HouseTemplate.bRequiresPower = false;
    BuildingTemplates.Add(EBuildingType::House, HouseTemplate);
    
    // 農場
    FBuildingTemplate FarmTemplate;
    FarmTemplate.BuildingType = EBuildingType::Farm;
    FarmTemplate.BuildingName = TEXT("Farm");
    FarmTemplate.Description = TEXT("Produces food for population");
    FarmTemplate.BaseCost.RequiredResources.Add(EResourceType::Wood, 30.0f);
    FarmTemplate.BaseCost.RequiredResources.Add(EResourceType::Gold, 20.0f);
    FarmTemplate.BaseCost.ConstructionTime = 8.0f;
    FarmTemplate.BaseCost.MaintenanceCost = 1.0f;
    FarmTemplate.ProducedResources.Add(EResourceType::Food);
    FarmTemplate.BaseProductionRates.Add(EResourceType::Food, 2.0f);
    FarmTemplate.MaxWorkerCapacity = 3;
    FarmTemplate.BaseHealth = 80.0f;
    FarmTemplate.bRequiresPower = false;
    BuildingTemplates.Add(EBuildingType::Farm, FarmTemplate);
    
    // 礦場
    FBuildingTemplate MineTemplate;
    MineTemplate.BuildingType = EBuildingType::Mine;
    MineTemplate.BuildingName = TEXT("Mine");
    MineTemplate.Description = TEXT("Extracts stone and iron from earth");
    MineTemplate.BaseCost.RequiredResources.Add(EResourceType::Wood, 40.0f);
    MineTemplate.BaseCost.RequiredResources.Add(EResourceType::Gold, 50.0f);
    MineTemplate.BaseCost.ConstructionTime = 12.0f;
    MineTemplate.BaseCost.MaintenanceCost = 2.0f;
    MineTemplate.ProducedResources.Add(EResourceType::Stone);
    MineTemplate.ProducedResources.Add(EResourceType::Iron);
    MineTemplate.BaseProductionRates.Add(EResourceType::Stone, 1.5f);
    MineTemplate.BaseProductionRates.Add(EResourceType::Iron, 0.8f);
    MineTemplate.MaxWorkerCapacity = 5;
    MineTemplate.BaseHealth = 120.0f;
    MineTemplate.bRequiresPower = false;
    BuildingTemplates.Add(EBuildingType::Mine, MineTemplate);
    
    // 市場
    FBuildingTemplate MarketTemplate;
    MarketTemplate.BuildingType = EBuildingType::Market;
    MarketTemplate.BuildingName = TEXT("Market");
    MarketTemplate.Description = TEXT("Facilitates trade and generates gold");
    MarketTemplate.BaseCost.RequiredResources.Add(EResourceType::Wood, 60.0f);
    MarketTemplate.BaseCost.RequiredResources.Add(EResourceType::Stone, 40.0f);
    MarketTemplate.BaseCost.RequiredResources.Add(EResourceType::Gold, 30.0f);
    MarketTemplate.BaseCost.ConstructionTime = 10.0f;
    MarketTemplate.BaseCost.MaintenanceCost = 1.5f;
    MarketTemplate.ProducedResources.Add(EResourceType::Gold);
    MarketTemplate.BaseProductionRates.Add(EResourceType::Gold, 3.0f);
    MarketTemplate.MaxWorkerCapacity = 4;
    MarketTemplate.BaseHealth = 100.0f;
    MarketTemplate.bRequiresPower = false;
    BuildingTemplates.Add(EBuildingType::Market, MarketTemplate);
    
    // 兵營
    FBuildingTemplate BarracksTemplate;
    BarracksTemplate.BuildingType = EBuildingType::Barracks;
    BarracksTemplate.BuildingName = TEXT("Barracks");
    BarracksTemplate.Description = TEXT("Trains military units");
    BarracksTemplate.BaseCost.RequiredResources.Add(EResourceType::Wood, 50.0f);
    BarracksTemplate.BaseCost.RequiredResources.Add(EResourceType::Iron, 30.0f);
    BarracksTemplate.BaseCost.RequiredResources.Add(EResourceType::Gold, 40.0f);
    BarracksTemplate.BaseCost.ConstructionTime = 15.0f;
    BarracksTemplate.BaseCost.MaintenanceCost = 3.0f;
    BarracksTemplate.ConsumedResources.Add(EResourceType::Gold);
    BarracksTemplate.BaseProductionRates.Add(EResourceType::Gold, -1.0f);
    BarracksTemplate.MaxWorkerCapacity = 2;
    BarracksTemplate.BaseHealth = 150.0f;
    BarracksTemplate.bRequiresPower = false;
    BuildingTemplates.Add(EBuildingType::Barracks, BarracksTemplate);
    
    // 可以繼續添加更多建築模板...
}

AActor* UMingRTSBuildingSystem::CreateBuildingActor(EBuildingType BuildingType, FVector Location, FRotator Rotation)
{
    if (!CurrentWorld)
    {
        return nullptr;
    }
    
    // 這裡應該根據建築類型創建對應的Actor類
    // 簡化實現，創建一個基礎Actor
    AActor* BuildingActor = CurrentWorld->SpawnActor<AActor>(AActor::StaticClass(), Location, Rotation);
    
    if (BuildingActor)
    {
        BuildingActor->SetActorLabel(GetBuildingName(BuildingType));
    }
    
    return BuildingActor;
}

void UMingRTSBuildingSystem::UpdateBuildingConstruction(AActor* BuildingActor, float DeltaTime)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 更新建造進度
    FBuildingTemplate Template = GetBuildingTemplate(BuildingData->BuildingType);
    float ConstructionSpeed = Template.BaseCost.ConstructionTime * ConstructionSpeedMultiplier;
    
    BuildingData->ConstructionProgress += DeltaTime / ConstructionSpeed;
    
    // 檢查是否完成建造
    if (BuildingData->ConstructionProgress >= 1.0f)
    {
        BuildingData->ConstructionProgress = 1.0f;
        BuildingData->CurrentState = EBuildingState::Operational;
        
        UE_LOG(LogTemp, Log, TEXT("Construction completed: %s"), *GetBuildingName(BuildingData->BuildingType));
        
        // 更新附近建築
        UpdateNearbyBuildings(BuildingActor);
    }
}

void UMingRTSBuildingSystem::UpdateBuildingUpgrade(AActor* BuildingActor, float DeltaTime)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 更新升級進度
    EBuildingUpgrade NextLevel = (EBuildingUpgrade)((int32)BuildingData->CurrentLevel + 1);
    FBuildingCost UpgradeCost = GetBuildingCost(BuildingData->BuildingType, NextLevel);
    float UpgradeSpeed = UpgradeCost.ConstructionTime * UpgradeSpeedMultiplier;
    
    BuildingData->ConstructionProgress += DeltaTime / UpgradeSpeed;
    
    // 檢查是否完成升級
    if (BuildingData->ConstructionProgress >= 1.0f)
    {
        BuildingData->ConstructionProgress = 0.0f;
        BuildingData->CurrentLevel = NextLevel;
        BuildingData->CurrentState = EBuildingState::Operational;
        
        // 應用升級效果
        ApplyBuildingUpgradeEffects(BuildingActor, NextLevel);
        
        UE_LOG(LogTemp, Log, TEXT("Upgrade completed: %s to level %d"), 
            *GetBuildingName(BuildingData->BuildingType), (int32)NextLevel);
        
        // 觸發升級事件
        OnBuildingUpgraded.Broadcast(*BuildingData, NextLevel);
    }
}

void UMingRTSBuildingSystem::ProcessBuildingMaintenance(AActor* BuildingActor, float DeltaTime)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData || !EconomicSystem)
    {
        return;
    }
    
    FBuildingTemplate Template = GetBuildingTemplate(BuildingData->BuildingType);
    float MaintenanceCost = Template.BaseCost.MaintenanceCost * MaintenanceCostMultiplier;
    
    // 根據效率調整維護成本
    MaintenanceCost *= BuildingData->ProductionEfficiency;
    
    // 消耗維護資源（通常是黃金）
    if (!EconomicSystem->ConsumeResource(EResourceType::Gold, MaintenanceCost * DeltaTime))
    {
        // 如果無法支付維護費用，降低效率
        BuildingData->ProductionEfficiency *= 0.99f;
        
        // 如果效率太低，建築變為損壞狀態
        if (BuildingData->ProductionEfficiency < 0.3f && BuildingData->CurrentState == EBuildingState::Operational)
        {
            BuildingData->CurrentState = EBuildingState::Damaged;
        }
    }
}

void UMingRTSBuildingSystem::CalculateBuildingProduction(AActor* BuildingActor)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 重新計算生產率
    TMap<EResourceType, float> NewProductionRates;
    
    for (TPair<EResourceType, float>& BaseRate : BuildingData->ProductionRates)
    {
        float Efficiency = CalculateBuildingEfficiency(BuildingActor);
        NewProductionRates.Add(BaseRate.Key, BaseRate.Value * Efficiency);
    }
    
    BuildingData->ProductionRates = NewProductionRates;
}

void UMingRTSBuildingSystem::ApplyBuildingEffects(AActor* BuildingActor)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 檢查電力狀態
    CheckBuildingPower(BuildingActor);
    
    // 處理工人效率
    ProcessWorkerEfficiency(BuildingActor);
}

void UMingRTSBuildingSystem::CheckBuildingIntegrity(AActor* BuildingActor)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 如果健康值恢復到一定程度，變為運營狀態
    if (BuildingData->Health >= BuildingData->MaxHealth * 0.8f && BuildingData->CurrentState == EBuildingState::Damaged)
    {
        BuildingData->CurrentState = EBuildingState::Operational;
        UE_LOG(LogTemp, Log, TEXT("Building %s is now operational"), *GetBuildingName(BuildingData->BuildingType));
    }
}

void UMingRTSBuildingSystem::UpdateBuildingGrid()
{
    // 更新建築網格
    BuildingGrid.Empty();
    
    for (const TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
    {
        if (BuildingPair.Key && !BuildingPair.Key->IsPendingKill())
        {
            FVector2D GridPosition = GetGridCoordinates(BuildingPair.Value.Location);
            SetGridPosition(GridPosition, BuildingPair.Key);
        }
    }
}

void UMingRTSBuildingSystem::ProcessBuildingInteractions()
{
    // 處理建築之間的交互
    for (const TPair<TObjectPtr<AActor>, FBuildingData>& BuildingPair : BuildingDataMap)
    {
        AActor* BuildingActor = BuildingPair.Key;
        if (!BuildingActor)
        {
            continue;
        }
        
        // 獲取影響範圍內的建築
        TArray<AActor*> NearbyBuildings = GetBuildingsInInfluenceRange(BuildingActor);
        
        // 處理交互效果
        for (AActor* NearbyBuilding : NearbyBuildings)
        {
            // 這裡可以添加具體的建築交互邏輯
            // 例如市場增加附近農場的效率，神廟增加所有建築的效率等
        }
    }
}

FVector2D UMingRTSBuildingSystem::GetGridCoordinates(FVector Location) const
{
    return FVector2D(
        FMath::RoundToInt(Location.X / GridSize),
        FMath::RoundToInt(Location.Y / GridSize)
    );
}

bool UMingRTSBuildingSystem::IsGridPositionOccupied(FVector2D GridPosition) const
{
    return BuildingGrid.Contains(GridPosition);
}

void UMingRTSBuildingSystem::SetGridPosition(FVector2D GridPosition, AActor* BuildingActor)
{
    BuildingGrid.Add(GridPosition, BuildingActor);
}

void UMingRTSBuildingSystem::ClearGridPosition(FVector2D GridPosition)
{
    BuildingGrid.Remove(GridPosition);
}

float UMingRTSBuildingSystem::CalculateBuildingCostMultiplier(EBuildingUpgrade Level) const
{
    switch (Level)
    {
        case EBuildingUpgrade::None: return 1.0f;
        case EBuildingUpgrade::Level1: return 1.0f;
        case EBuildingUpgrade::Level2: return 1.5f;
        case EBuildingUpgrade::Level3: return 2.5f;
        case EBuildingUpgrade::Level4: return 4.0f;
        case EBuildingUpgrade::Level5: return 6.0f;
        default: return 1.0f;
    }
}

float UMingRTSBuildingSystem::CalculateBuildingProductionMultiplier(EBuildingUpgrade Level) const
{
    switch (Level)
    {
        case EBuildingUpgrade::None: return 1.0f;
        case EBuildingUpgrade::Level1: return 1.0f;
        case EBuildingUpgrade::Level2: return 1.3f;
        case EBuildingUpgrade::Level3: return 1.7f;
        case EBuildingUpgrade::Level4: return 2.2f;
        case EBuildingUpgrade::Level5: return 3.0f;
        default: return 1.0f;
    }
}

void UMingRTSBuildingSystem::ApplyBuildingUpgradeEffects(AActor* BuildingActor, EBuildingUpgrade NewLevel)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 增加最大生命值
    FBuildingTemplate Template = GetBuildingTemplate(BuildingData->BuildingType);
    BuildingData->MaxHealth = Template.BaseHealth * (1.0f + (int32)NewLevel * 0.2f);
    BuildingData->Health = BuildingData->MaxHealth;
    
    // 增加最大工人數量
    BuildingData->MaxWorkers = Template.MaxWorkerCapacity + (int32)NewLevel;
    
    // 重新計算生產率
    CalculateBuildingProduction(BuildingActor);
    
    UE_LOG(LogTemp, Log, TEXT("Applied upgrade effects for %s to level %d"), 
        *GetBuildingName(BuildingData->BuildingType), (int32)NewLevel);
}

void UMingRTSBuildingSystem::CheckBuildingPower(AActor* BuildingActor)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    FBuildingTemplate Template = GetBuildingTemplate(BuildingData->BuildingType);
    
    if (Template.bRequiresPower)
    {
        // 檢查是否有電力供應
        // 這裡可以添加電力系統檢查邏輯
        // 簡化實現，假設總是有電力
        BuildingData->bIsPowered = true;
    }
}

void UMingRTSBuildingSystem::ProcessWorkerEfficiency(AActor* BuildingActor)
{
    FBuildingData* BuildingData = BuildingDataMap.Find(BuildingActor);
    if (!BuildingData)
    {
        return;
    }
    
    // 根據工人數量更新效率
    if (BuildingData->MaxWorkers > 0)
    {
        BuildingData->ProductionEfficiency = (float)BuildingData->WorkerCount / BuildingData->MaxWorkers;
    }
    else
    {
        BuildingData->ProductionEfficiency = 0.0f;
    }
}

float UMingRTSBuildingSystem::GetBuildingInfluenceBonus(AActor* BuildingActor) const
{
    float Bonus = 1.0f;
    
    // 檢查附近建築的影響
    TArray<AActor*> NearbyBuildings = GetBuildingsInInfluenceRange(BuildingActor);
    
    for (AActor* NearbyBuilding : NearbyBuildings)
    {
        const FBuildingData* NearbyBuildingData = BuildingDataMap.Find(NearbyBuilding);
        if (!NearbyBuildingData)
        {
            continue;
        }
        
        // 根據建築類型給予加成
        switch (NearbyBuildingData->BuildingType)
        {
            case EBuildingType::Temple:
                Bonus *= 1.1f; // 神廟增加10%效率
                break;
            case EBuildingType::Market:
                Bonus *= 1.05f; // 市場增加5%效率
                break;
            case EBuildingType::University:
                Bonus *= 1.15f; // 大學增加15%效率
                break;
            default:
                break;
        }
    }
    
    return FMath::Clamp(Bonus, 0.5f, 2.0f);
}

void UMingRTSBuildingSystem::UpdateNearbyBuildings(AActor* BuildingActor)
{
    // 更新附近建築的效率
    TArray<AActor*> NearbyBuildings = GetBuildingsInInfluenceRange(BuildingActor);
    
    for (AActor* NearbyBuilding : NearbyBuildings)
    {
        CalculateBuildingProduction(NearbyBuilding);
    }
}

bool UMingRTSBuildingSystem::ValidateBuildingData(const FBuildingData& BuildingData) const
{
    // 驗證建築數據的有效性
    if (BuildingData.MaxHealth <= 0.0f)
    {
        return false;
    }
    
    if (BuildingData.MaxWorkers < 0)
    {
        return false;
    }
    
    if (BuildingData.WorkerCount < 0 || BuildingData.WorkerCount > BuildingData.MaxWorkers)
    {
        return false;
    }
    
    return true;
}

void UMingRTSBuildingSystem::CleanupInvalidBuildings()
{
    // 清理無效建築
    for (auto It = BuildingDataMap.CreateIterator(); It; ++It)
    {
        if (!It->Key || It->Key->IsPendingKill())
        {
            // 清除網格位置
            FVector2D GridPosition = GetGridCoordinates(It->Value.Location);
            ClearGridPosition(GridPosition);
            
            It.RemoveCurrent();
        }
    }
}

FString UMingRTSBuildingSystem::GetBuildingName(EBuildingType BuildingType) const
{
    switch (BuildingType)
    {
        case EBuildingType::House: return TEXT("House");
        case EBuildingType::Farm: return TEXT("Farm");
        case EBuildingType::Mine: return TEXT("Mine");
        case EBuildingType::LumberMill: return TEXT("Lumber Mill");
        case EBuildingType::Quarry: return TEXT("Quarry");
        case EBuildingType::Market: return TEXT("Market");
        case EBuildingType::Barracks: return TEXT("Barracks");
        case EBuildingType::Temple: return TEXT("Temple");
        case EBuildingType::Palace: return TEXT("Palace");
        case EBuildingType::Wall: return TEXT("Wall");
        case EBuildingType::Tower: return TEXT("Tower");
        case EBuildingType::Workshop: return TEXT("Workshop");
        case EBuildingType::Storage: return TEXT("Storage");
        case EBuildingType::Dock: return TEXT("Dock");
        case EBuildingType::University: return TEXT("University");
        default: return TEXT("Unknown Building");
    }
}

FString UMingRTSBuildingSystem::GetBuildingDescription(EBuildingType BuildingType) const
{
    switch (BuildingType)
    {
        case EBuildingType::House: return TEXT("Basic housing for population");
        case EBuildingType::Farm: return TEXT("Produces food for population");
        case EBuildingType::Mine: return TEXT("Extracts stone and iron from earth");
        case EBuildingType::LumberMill: return TEXT("Processes wood into lumber");
        case EBuildingType::Quarry: return TEXT("Extracts stone from mountains");
        case EBuildingType::Market: return TEXT("Facilitates trade and generates gold");
        case EBuildingType::Barracks: return TEXT("Trains military units");
        case EBuildingType::Temple: return TEXT("Provides religious bonuses");
        case EBuildingType::Palace: return TEXT("Center of government");
        case EBuildingType::Wall: return TEXT("Defensive structure");
        case EBuildingType::Tower: return TEXT("Defensive watchtower");
        case EBuildingType::Workshop: return TEXT("Produces tools and equipment");
        case EBuildingType::Storage: return TEXT("Stores excess resources");
        case EBuildingType::Dock: return TEXT("Facilitates water trade");
        case EBuildingType::University: return TEXT("Research and education center");
        default: return TEXT("Unknown building type");
    }
}
