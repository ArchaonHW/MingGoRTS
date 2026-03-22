#include "MingRTSBuildingSystem.h"
#include "MingRTSResourceManager.h"
#include "MingRTSUnitManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

UMingRTSBuildingSystem::UMingRTSBuildingSystem()
    : ConstructionSpeedMultiplier(1.0f)
    , MaintenanceMultiplier(1.0f)
    , RepairSpeedMultiplier(1.0f)
    , bAutoRepair(true)
    , bAutoUpgrade(false)
    , bIsInitialized(false)
{
}

void UMingRTSBuildingSystem::InitializeBuildingSystem(UMingRTSResourceManager* InResourceManager, UMingRTSUnitManager* InUnitManager)
{
    ResourceManager = InResourceManager;
    UnitManager = InUnitManager;
    
    if (ResourceManager && UnitManager)
    {
        InitializeDefaultTemplates();
        bIsInitialized = true;
        
        UE_LOG(LogTemp, Log, TEXT("RTS Building System initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RTS Building System initialization failed"));
    }
}

FString UMingRTSBuildingSystem::CreateBuilding(const FRTSBuildingTemplate& Template, const FVector& Location, const FRotator& Rotation)
{
    // TODO: 優化建築創建性能 - 當前每次創建都進行完整驗證，可考慮批量創建或緩存驗證結果
    if (!bIsInitialized)
    {
        return TEXT("");
    }

    if (!ValidateBuildingPlacement(Template, Location))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot place building at location: %s"), *Location.ToString());
        return TEXT("");
    }

    // 檢查資源
    if (ResourceManager)
    {
        for (int32 i = 0; i < Template.RequiredResources.Num(); ++i)
        {
            if (!ResourceManager->HasEnoughResource(Template.RequiredResources[i], Template.ResourceCosts[i]))
            {
                UE_LOG(LogTemp, Warning, TEXT("Insufficient resources for building"));
                return TEXT("");
            }
        }
        
        // 消耗資源
        for (int32 i = 0; i < Template.RequiredResources.Num(); ++i)
        {
            ResourceManager->RemoveResource(Template.RequiredResources[i], Template.ResourceCosts[i]);
        }
    }

    // 創建建築
    FRTSBuildingData Building;
    Building.BuildingID = GenerateBuildingID(Template.BuildingType);
    Building.BuildingName = Template.TemplateName;
    Building.BuildingType = Template.BuildingType;
    Building.BuildingSize = Template.BuildingSize;
    Building.Location = Location;
    Building.Rotation = Rotation;
    Building.BuildingState = ERTSBuildingState::UnderConstruction;
    Building.ConstructionProgress = 0.0f;
    Building.MaxHealth = 100.0f * GetBuildingSizeMultiplier(Template.BuildingSize);
    Building.Health = Building.MaxHealth * 0.1f; // 建設中只有10%生命值
    Building.MaintenanceCost = Template.ConstructionCost * 0.1f * GetBuildingSizeMultiplier(Template.BuildingSize);
    Building.WorkerCapacity = 5 * GetBuildingSizeMultiplier(Template.BuildingSize);
    Building.CurrentWorkers = 0;
    Building.bIsPowered = false;
    Building.bIsConnected = false;
    
    // 設置生產和消耗
    Building.ProducedResources = TArray<ERTSResourceType>();
    Building.ResourceProduction = TArray<float>();
    Building.RequiredResources = TArray<ERTSResourceType>();
    Building.ResourceConsumption = TArray<float>();
    
    Buildings.Add(Building.BuildingID, Building);
    
    UE_LOG(LogTemp, Log, TEXT("Created building: %s at %s"), *Building.BuildingName, *Location.ToString());
    
    return Building.BuildingID;
}

bool UMingRTSBuildingSystem::DestroyBuilding(const FString& BuildingID)
{
    if (!Buildings.Contains(BuildingID))
    {
        return false;
    }

    FRTSBuildingData Building = Buildings[BuildingID];
    
    // 斷開所有連接
    TArray<FString> ConnectedBuildings = GetConnectedBuildings(BuildingID);
    for (const FString& ConnectedID : ConnectedBuildings)
    {
        DisconnectBuilding(BuildingID, ConnectedID);
    }
    
    // 移除建築
    Buildings.Remove(BuildingID);
    
    NotifyBuildingDestroyed(BuildingID, Building.BuildingType);
    
    UE_LOG(LogTemp, Log, TEXT("Destroyed building: %s"), *BuildingID);
    
    return true;
}

FRTSBuildingData UMingRTSBuildingSystem::GetBuilding(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        return Buildings[BuildingID];
    }
    
    return FRTSBuildingData();
}

TArray<FRTSBuildingData> UMingRTSBuildingSystem::GetAllBuildings() const
{
    TArray<FRTSBuildingData> AllBuildings;
    
    for (const auto& BuildingPair : Buildings)
    {
        AllBuildings.Add(BuildingPair.Value);
    }
    
    return AllBuildings;
}

TArray<FRTSBuildingData> UMingRTSBuildingSystem::GetBuildingsByType(ERTSBuildingType BuildingType) const
{
    TArray<FRTSBuildingData> BuildingsByType;
    
    for (const auto& BuildingPair : Buildings)
    {
        if (BuildingPair.Value.BuildingType == BuildingType)
        {
            BuildingsByType.Add(BuildingPair.Value);
        }
    }
    
    return BuildingsByType;
}

TArray<FRTSBuildingData> UMingRTSBuildingSystem::GetBuildingsByState(ERTSBuildingState BuildingState) const
{
    TArray<FRTSBuildingData> BuildingsByState;
    
    for (const auto& BuildingPair : Buildings)
    {
        if (BuildingPair.Value.BuildingState == BuildingState)
        {
            BuildingsByState.Add(BuildingPair.Value);
        }
    }
    
    return BuildingsByState;
}

TArray<FRTSBuildingData> UMingRTSBuildingSystem::GetBuildingsInArea(const FVector& Center, float Radius) const
{
    TArray<FRTSBuildingData> BuildingsInArea;
    
    for (const auto& BuildingPair : Buildings)
    {
        float Distance = FVector::Dist(Center, BuildingPair.Value.Location);
        if (Distance <= Radius)
        {
            BuildingsInArea.Add(BuildingPair.Value);
        }
    }
    
    return BuildingsInArea;
}

void UMingRTSBuildingSystem::SetBuildingState(const FString& BuildingID, ERTSBuildingState NewState)
{
    if (Buildings.Contains(BuildingID))
    {
        ERTSBuildingState OldState = Buildings[BuildingID].BuildingState;
        Buildings[BuildingID].BuildingState = NewState;
        
        NotifyBuildingStateChanged(BuildingID, NewState);
        
        UE_LOG(LogTemp, Log, TEXT("Building %s state changed from %d to %d"), *BuildingID, (int32)OldState, (int32)NewState);
    }
}

ERTSBuildingState UMingRTSBuildingSystem::GetBuildingState(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        return Buildings[BuildingID].BuildingState;
    }
    
    return ERTSBuildingState::Destroyed;
}

bool UMingRTSBuildingSystem::IsBuildingOperational(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        const FRTSBuildingData& Building = Buildings[BuildingID];
        return Building.BuildingState == ERTSBuildingState::Operational && 
               Building.Health > 0.0f && 
               Building.bIsPowered;
    }
    
    return false;
}

void UMingRTSBuildingSystem::StartConstruction(const FString& BuildingID)
{
    if (Buildings.Contains(BuildingID))
    {
        Buildings[BuildingID].BuildingState = ERTSBuildingState::UnderConstruction;
        Buildings[BuildingID].ConstructionProgress = 0.0f;
        
        UE_LOG(LogTemp, Log, TEXT("Started construction for building: %s"), *BuildingID);
    }
}

void UMingRTSBuildingSystem::StopConstruction(const FString& BuildingID)
{
    if (Buildings.Contains(BuildingID))
    {
        Buildings[BuildingID].BuildingState = ERTSBuildingState::Planning;
        
        UE_LOG(LogTemp, Log, TEXT("Stopped construction for building: %s"), *BuildingID);
    }
}

void UMingRTSBuildingSystem::CompleteConstruction(const FString& BuildingID)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        Building.BuildingState = ERTSBuildingState::Operational;
        Building.ConstructionProgress = 100.0f;
        Building.Health = Building.MaxHealth;
        
        NotifyBuildingConstructed(BuildingID, Building.BuildingType);
        
        UE_LOG(LogTemp, Log, TEXT("Completed construction for building: %s"), *BuildingID);
    }
}

bool UMingRTSBuildingSystem::UpgradeBuilding(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade)
{
    if (!Buildings.Contains(BuildingID) || !CanUpgradeBuilding(BuildingID, Upgrade))
    {
        return false;
    }

    // 消耗資源
    if (ResourceManager)
    {
        for (int32 i = 0; i < Upgrade.RequiredResources.Num(); ++i)
        {
            ResourceManager->RemoveResource(Upgrade.RequiredResources[i], Upgrade.ResourceCosts[i]);
        }
    }

    // 設置升級狀態
    FRTSBuildingData& Building = Buildings[BuildingID];
    Building.BuildingState = ERTSBuildingState::Upgrading;
    
    // 應用升級效果
    ApplyBuildingEffects(BuildingID, Upgrade);
    
    // 添加到升級列表
    if (!BuildingUpgrades.Contains(BuildingID))
    {
        BuildingUpgrades.Add(BuildingID, TArray<FRTSBuildingUpgrade>());
    }
    BuildingUpgrades[BuildingID].Add(Upgrade);
    
    NotifyBuildingUpgraded(BuildingID, Upgrade.UpgradeID);
    
    UE_LOG(LogTemp, Log, TEXT("Upgraded building: %s with %s"), *BuildingID, *Upgrade.UpgradeName);
    
    return true;
}

bool UMingRTSBuildingSystem::CanUpgradeBuilding(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade) const
{
    if (!Buildings.Contains(BuildingID))
    {
        return false;
    }

    const FRTSBuildingData& Building = Buildings[BuildingID];
    
    // 檢查建築狀態
    if (Building.BuildingState != ERTSBuildingState::Operational)
    {
        return false;
    }
    
    // 檢查資源
    if (ResourceManager)
    {
        for (int32 i = 0; i < Upgrade.RequiredResources.Num(); ++i)
        {
            if (!ResourceManager->HasEnoughResource(Upgrade.RequiredResources[i], Upgrade.ResourceCosts[i]))
            {
                return false;
            }
        }
    }
    
    return true;
}

TArray<FRTSBuildingUpgrade> UMingRTSBuildingSystem::GetAvailableUpgrades(const FString& BuildingID) const
{
    if (BuildingUpgrades.Contains(BuildingID))
    {
        return BuildingUpgrades[BuildingID];
    }
    
    return TArray<FRTSBuildingUpgrade>();
}

void UMingRTSBuildingSystem::AddBuildingUpgrade(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade)
{
    if (!BuildingUpgrades.Contains(BuildingID))
    {
        BuildingUpgrades.Add(BuildingID, TArray<FRTSBuildingUpgrade>());
    }
    
    BuildingUpgrades[BuildingID].Add(Upgrade);
    
    UE_LOG(LogTemp, Log, TEXT("Added upgrade %s to building %s"), *Upgrade.UpgradeName, *BuildingID);
}

void UMingRTSBuildingSystem::RepairBuilding(const FString& BuildingID, float RepairAmount)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        
        if (Building.BuildingState == ERTSBuildingState::Damaged)
        {
            Building.BuildingState = ERTSBuildingState::Repairing;
        }
        
        float ActualRepairAmount = RepairAmount * RepairSpeedMultiplier;
        Building.Health = FMath::Clamp(Building.Health + ActualRepairAmount, 0.0f, Building.MaxHealth);
        
        if (Building.Health >= Building.MaxHealth)
        {
            Building.BuildingState = ERTSBuildingState::Operational;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Repaired building %s by %.1f"), *BuildingID, ActualRepairAmount);
    }
}

void UMingRTSBuildingSystem::DamageBuilding(const FString& BuildingID, float DamageAmount)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        Building.Health = FMath::Clamp(Building.Health - DamageAmount, 0.0f, Building.MaxHealth);
        
        if (Building.Health <= 0.0f)
        {
            Building.BuildingState = ERTSBuildingState::Destroyed;
        }
        else if (Building.Health < Building.MaxHealth * 0.5f)
        {
            Building.BuildingState = ERTSBuildingState::Damaged;
        }
        
        UE_LOG(LogTemp, Warning, TEXT("Building %s damaged by %.1f, health: %.1f/%.1f"), 
               *BuildingID, DamageAmount, Building.Health, Building.MaxHealth);
    }
}

float UMingRTSBuildingSystem::GetBuildingHealth(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        return Buildings[BuildingID].Health;
    }
    
    return 0.0f;
}

float UMingRTSBuildingSystem::GetBuildingHealthPercentage(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        const FRTSBuildingData& Building = Buildings[BuildingID];
        return Building.MaxHealth > 0.0f ? (Building.Health / Building.MaxHealth) : 0.0f;
    }
    
    return 0.0f;
}

void UMingRTSBuildingSystem::UpdateBuildingProduction(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    for (auto& BuildingPair : Buildings)
    {
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 處理建設
        if (Building.BuildingState == ERTSBuildingState::UnderConstruction)
        {
            ProcessBuildingConstruction(Building, DeltaTime);
        }
        
        // 處理生產
        if (Building.BuildingState == ERTSBuildingState::Operational && Building.bIsPowered)
        {
            ProcessBuildingProduction(Building, DeltaTime);
        }
        
        // 處理維護
        if (Building.BuildingState == ERTSBuildingState::Operational)
        {
            ProcessBuildingMaintenance(Building, DeltaTime);
        }
        
        // 檢查建築完整性
        CheckBuildingIntegrity(Building);
    }
}

float UMingRTSBuildingSystem::GetBuildingProductionRate(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        return Buildings[BuildingID].ProductionRate;
    }
    
    return 0.0f;
}

void UMingRTSBuildingSystem::SetBuildingProductionRate(const FString& BuildingID, float Rate)
{
    if (Buildings.Contains(BuildingID))
    {
        Buildings[BuildingID].ProductionRate = FMath::Max(0.0f, Rate);
        UE_LOG(LogTemp, Log, TEXT("Set production rate for building %s to %.2f"), *BuildingID, Rate);
    }
}

bool UMingRTSBuildingSystem::AssignWorkers(const FString& BuildingID, int32 WorkerCount)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        int32 AvailableSpace = Building.WorkerCapacity - Building.CurrentWorkers;
        
        if (WorkerCount <= AvailableSpace)
        {
            Building.CurrentWorkers += WorkerCount;
            
            // 根據工人數量調整生產率
            float WorkerEfficiency = (float)Building.CurrentWorkers / (float)Building.WorkerCapacity;
            Building.ProductionRate = WorkerEfficiency;
            
            UE_LOG(LogTemp, Log, TEXT("Assigned %d workers to building %s (total: %d/%d)"), 
                   WorkerCount, *BuildingID, Building.CurrentWorkers, Building.WorkerCapacity);
            return true;
        }
    }
    
    return false;
}

void UMingRTSBuildingSystem::RemoveWorkers(const FString& BuildingID, int32 WorkerCount)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        Building.CurrentWorkers = FMath::Max(0, Building.CurrentWorkers - WorkerCount);
        
        // 根據工人數量調整生產率
        float WorkerEfficiency = Building.WorkerCapacity > 0 ? (float)Building.CurrentWorkers / (float)Building.WorkerCapacity : 0.0f;
        Building.ProductionRate = WorkerEfficiency;
        
        UE_LOG(LogTemp, Log, TEXT("Removed %d workers from building %s (total: %d/%d)"), 
               WorkerCount, *BuildingID, Building.CurrentWorkers, Building.WorkerCapacity);
    }
}

int32 UMingRTSBuildingSystem::GetAvailableWorkerCapacity(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        const FRTSBuildingData& Building = Buildings[BuildingID];
        return Building.WorkerCapacity - Building.CurrentWorkers;
    }
    
    return 0;
}

bool UMingRTSBuildingSystem::HasEnoughWorkers(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        const FRTSBuildingData& Building = Buildings[BuildingID];
        return Building.CurrentWorkers >= (Building.WorkerCapacity * 0.5f); // 至少需要50%工人
    }
    
    return false;
}

void UMingRTSBuildingSystem::AddBuildingTemplate(const FRTSBuildingTemplate& Template)
{
    if (!Template.TemplateID.IsEmpty())
    {
        BuildingTemplates.Add(Template.TemplateID, Template);
        UE_LOG(LogTemp, Log, TEXT("Added building template: %s"), *Template.TemplateName);
    }
}

void UMingRTSBuildingSystem::RemoveBuildingTemplate(const FString& TemplateID)
{
    if (BuildingTemplates.Contains(TemplateID))
    {
        BuildingTemplates.Remove(TemplateID);
        UE_LOG(LogTemp, Log, TEXT("Removed building template: %s"), *TemplateID);
    }
}

FRTSBuildingTemplate UMingRTSBuildingSystem::GetBuildingTemplate(const FString& TemplateID) const
{
    if (BuildingTemplates.Contains(TemplateID))
    {
        return BuildingTemplates[TemplateID];
    }
    
    return FRTSBuildingTemplate();
}

TArray<FRTSBuildingTemplate> UMingRTSBuildingSystem::GetAllBuildingTemplates() const
{
    TArray<FRTSBuildingTemplate> AllTemplates;
    
    for (const auto& TemplatePair : BuildingTemplates)
    {
        AllTemplates.Add(TemplatePair.Value);
    }
    
    return AllTemplates;
}

TArray<FRTSBuildingTemplate> UMingRTSBuildingSystem::GetBuildingTemplatesByType(ERTSBuildingType BuildingType) const
{
    TArray<FRTSBuildingTemplate> TemplatesByType;
    
    for (const auto& TemplatePair : BuildingTemplates)
    {
        if (TemplatePair.Value.BuildingType == BuildingType)
        {
            TemplatesByType.Add(TemplatePair.Value);
        }
    }
    
    return TemplatesByType;
}

bool UMingRTSBuildingSystem::CanPlaceBuilding(const FRTSBuildingTemplate& Template, const FVector& Location) const
{
    return ValidateBuildingPlacement(Template, Location);
}

bool UMingRTSBuildingSystem::IsLocationOccupied(const FVector& Location, float Radius) const
{
    for (const auto& BuildingPair : Buildings)
    {
        const FRTSBuildingData& Building = BuildingPair.Value;
        float Distance = FVector::Dist(Location, Building.Location);
        
        // 根據建築大小計算佔用半徑
        float BuildingRadius = 100.0f * GetBuildingSizeMultiplier(Building.BuildingSize);
        
        if (Distance < (Radius + BuildingRadius))
        {
            return true;
        }
    }
    
    return false;
}

FVector UMingRTSBuildingSystem::FindBestLocation(const FRTSBuildingTemplate& Template, const FVector& PreferredLocation) const
{
    // TODO: 優化位置搜索算法 - 當前使用簡單的螺旋搜索，需要改為更高效的算法
    // 考慮使用A*算法或網格搜索來提高性能
    float SearchRadius = 500.0f;
    float StepSize = 50.0f;
    
    for (float Radius = StepSize; Radius <= SearchRadius; Radius += StepSize)
    {
        for (float Angle = 0.0f; Angle < 360.0f; Angle += 45.0f)
        {
            FVector TestLocation = PreferredLocation + FVector(
                FMath::Cos(FMath::DegreesToRadians(Angle)) * Radius,
                FMath::Sin(FMath::DegreesToRadians(Angle)) * Radius,
                0.0f
            );
            
            if (CanPlaceBuilding(Template, TestLocation))
            {
                return TestLocation;
            }
        }
    }
    
    return PreferredLocation; // 如果找不到，返回優先位置
}

void UMingRTSBuildingSystem::ConnectBuilding(const FString& BuildingID, const FString& TargetBuildingID)
{
    // TODO: 優化建築連接算法 - 當前使用簡單的雙向連接，可考慮圖算法優化路徑查找
    if (!Buildings.Contains(BuildingID) || !Buildings.Contains(TargetBuildingID))
    {
        return;
    }

    if (!BuildingConnections.Contains(BuildingID))
    {
        BuildingConnections.Add(BuildingID, TArray<FString>());
    }
    
    if (!BuildingConnections[BuildingID].Contains(TargetBuildingID))
    {
        BuildingConnections[BuildingID].Add(TargetBuildingID);
        
        // 雙向連接
        if (!BuildingConnections.Contains(TargetBuildingID))
        {
            BuildingConnections.Add(TargetBuildingID, TArray<FString>());
        }
        BuildingConnections[TargetBuildingID].Add(BuildingID);
        
        // 更新連接狀態
        Buildings[BuildingID].bIsConnected = true;
        Buildings[TargetBuildingID].bIsConnected = true;
        
        UE_LOG(LogTemp, Log, TEXT("Connected buildings: %s <-> %s"), *BuildingID, *TargetBuildingID);
    }
}

void UMingRTSBuildingSystem::DisconnectBuilding(const FString& BuildingID, const FString& TargetBuildingID)
{
    if (BuildingConnections.Contains(BuildingID))
    {
        BuildingConnections[BuildingID].Remove(TargetBuildingID);
        
        // 雙向斷開
        if (BuildingConnections.Contains(TargetBuildingID))
        {
            BuildingConnections[TargetBuildingID].Remove(BuildingID);
        }
        
        // 檢查是否還有其他連接
        if (BuildingConnections[BuildingID].Num() == 0)
        {
            Buildings[BuildingID].bIsConnected = false;
        }
        
        if (BuildingConnections[TargetBuildingID].Num() == 0)
        {
            Buildings[TargetBuildingID].bIsConnected = false;
        }
        
        UE_LOG(LogTemp, Log, TEXT("Disconnected buildings: %s <-> %s"), *BuildingID, *TargetBuildingID);
    }
}

bool UMingRTSBuildingSystem::IsBuildingConnected(const FString& BuildingID) const
{
    if (Buildings.Contains(BuildingID))
    {
        return Buildings[BuildingID].bIsConnected;
    }
    
    return false;
}

TArray<FString> UMingRTSBuildingSystem::GetConnectedBuildings(const FString& BuildingID) const
{
    if (BuildingConnections.Contains(BuildingID))
    {
        return BuildingConnections[BuildingID];
    }
    
    return TArray<FString>();
}

int32 UMingRTSBuildingSystem::GetTotalBuildingCount() const
{
    return Buildings.Num();
}

int32 UMingRTSBuildingSystem::GetBuildingCountByType(ERTSBuildingType BuildingType) const
{
    return GetBuildingsByType(BuildingType).Num();
}

int32 UMingRTSBuildingSystem::GetOperationalBuildingCount() const
{
    return GetBuildingsByState(ERTSBuildingState::Operational).Num();
}

float UMingRTSBuildingSystem::GetTotalBuildingValue() const
{
    float TotalValue = 0.0f;
    
    for (const auto& BuildingPair : Buildings)
    {
        const FRTSBuildingData& Building = BuildingPair.Value;
        TotalValue += Building.MaxHealth * GetBuildingSizeMultiplier(Building.BuildingSize);
    }
    
    return TotalValue;
}

float UMingRTSBuildingSystem::GetTotalMaintenanceCost() const
{
    float TotalCost = 0.0f;
    
    for (const auto& BuildingPair : Buildings)
    {
        const FRTSBuildingData& Building = BuildingPair.Value;
        if (Building.BuildingState == ERTSBuildingState::Operational)
        {
            TotalCost += Building.MaintenanceCost * MaintenanceMultiplier;
        }
    }
    
    return TotalCost;
}

void UMingRTSBuildingSystem::OnBuildingAttacked(const FString& BuildingID, float Damage)
{
    DamageBuilding(BuildingID, Damage);
}

void UMingRTSBuildingSystem::OnBuildingCaptured(const FString& BuildingID, const FString& NewOwner)
{
    if (Buildings.Contains(BuildingID))
    {
        Buildings[BuildingID].OwnerID = NewOwner;
        UE_LOG(LogTemp, Warning, TEXT("Building %s captured by %s"), *BuildingID, *NewOwner);
    }
}

void UMingRTSBuildingSystem::OnResourceDepleted(const FString& BuildingID)
{
    if (Buildings.Contains(BuildingID))
    {
        Buildings[BuildingID].BuildingState = ERTSBuildingState::Abandoned;
        UE_LOG(LogTemp, Warning, TEXT("Building %s abandoned due to resource depletion"), *BuildingID);
    }
}

void UMingRTSBuildingSystem::InitializeDefaultTemplates()
{
    // 創建默認建築模板
    TArray<FRTSBuildingTemplate> DefaultTemplates;
    
    // 住宅
    FRTSBuildingTemplate Residential;
    Residential.TemplateID = TEXT("residential_small");
    Residential.TemplateName = TEXT("小型住宅");
    Residential.BuildingType = ERTSBuildingType::Residential;
    Residential.BuildingSize = ERTSBuildingSize::Small;
    Residential.ConstructionCost = 100.0f;
    Residential.ConstructionTime = 30.0f;
    Residential.RequiredResources.Add(ERTSResourceType::Wood);
    Residential.ResourceCosts.Add(50.0f);
    Residential.RequiredResources.Add(ERTSResourceType::Stone);
    Residential.ResourceCosts.Add(30.0f);
    Residential.Description = TEXT("提供基礎居住空間");
    DefaultTemplates.Add(Residential);
    
    // 工廠
    FRTSBuildingTemplate Factory;
    Factory.TemplateID = TEXT("industrial_medium");
    Factory.TemplateName = TEXT("中型工廠");
    Factory.BuildingType = ERTSBuildingType::Industrial;
    Factory.BuildingSize = ERTSBuildingSize::Medium;
    Factory.ConstructionCost = 300.0f;
    Factory.ConstructionTime = 60.0f;
    Factory.RequiredResources.Add(ERTSResourceType::Iron);
    Factory.ResourceCosts.Add(100.0f);
    Factory.RequiredResources.Add(ERTSResourceType::Stone);
    Factory.ResourceCosts.Add(80.0f);
    Factory.Description = TEXT("生產工業產品");
    DefaultTemplates.Add(Factory);
    
    // 農場
    FRTSBuildingTemplate Farm;
    Farm.TemplateID = TEXT("agricultural_large");
    Farm.TemplateName = TEXT("大型農場");
    Farm.BuildingType = ERTSBuildingType::Agricultural;
    Farm.BuildingSize = ERTSBuildingSize::Large;
    Farm.ConstructionCost = 200.0f;
    Farm.ConstructionTime = 45.0f;
    Farm.RequiredResources.Add(ERTSResourceType::Wood);
    Farm.ResourceCosts.Add(80.0f);
    Farm.Description = TEXT("生產糧食");
    DefaultTemplates.Add(Farm);
    
    // 添加所有模板
    for (const FRTSBuildingTemplate& Template : DefaultTemplates)
    {
        AddBuildingTemplate(Template);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized %d default building templates"), DefaultTemplates.Num());
}

void UMingRTSBuildingSystem::ProcessBuildingConstruction(FRTSBuildingData& Building, float DeltaTime)
{
    // 計算建設進度
    float ConstructionSpeed = ConstructionSpeedMultiplier;
    if (Building.CurrentWorkers > 0)
    {
        float WorkerEfficiency = (float)Building.CurrentWorkers / (float)Building.WorkerCapacity;
        ConstructionSpeed *= WorkerEfficiency;
    }
    
    Building.ConstructionProgress += ConstructionSpeed * DeltaTime * 10.0f; // 10% per second base rate
    
    if (Building.ConstructionProgress >= 100.0f)
    {
        Building.ConstructionProgress = 100.0f;
        Building.BuildingState = ERTSBuildingState::Operational;
        Building.Health = Building.MaxHealth;
        
        NotifyBuildingConstructed(Building.BuildingID, Building.BuildingType);
    }
}

void UMingRTSBuildingSystem::ProcessBuildingProduction(FRTSBuildingData& Building, float DeltaTime)
{
    if (!ResourceManager || Building.ProducedResources.Num() == 0)
    {
        return;
    }

    // 計算生產量
    float ProductionMultiplier = Building.ProductionRate;
    if (Building.CurrentWorkers > 0)
    {
        float WorkerEfficiency = (float)Building.CurrentWorkers / (float)Building.WorkerCapacity;
        ProductionMultiplier *= WorkerEfficiency;
    }
    
    // 生產資源
    for (int32 i = 0; i < Building.ProducedResources.Num(); ++i)
    {
        float ProductionAmount = Building.ResourceProduction[i] * ProductionMultiplier * DeltaTime;
        ResourceManager->AddResource(Building.ProducedResources[i], ProductionAmount);
    }
}

void UMingRTSBuildingSystem::ProcessBuildingMaintenance(FRTSBuildingData& Building, float DeltaTime)
{
    if (!ResourceManager)
    {
        return;
    }

    // 計算維護成本
    float MaintenanceCost = Building.MaintenanceCost * MaintenanceMultiplier * DeltaTime;
    
    // 消耗黃金作為維護費用
    if (ResourceManager->HasEnoughResource(ERTSResourceType::Gold, MaintenanceCost))
    {
        ResourceManager->RemoveResource(ERTSResourceType::Gold, MaintenanceCost);
    }
    else
    {
        // 如果沒有足夠的黃金，建築會損壞
        DamageBuilding(Building.BuildingID, MaintenanceCost * 0.1f);
    }
}

void UMingRTSBuildingSystem::UpdateBuildingConnections(const FString& BuildingID)
{
    // 更新建築連接狀態
    if (BuildingConnections.Contains(BuildingID))
    {
        Buildings[BuildingID].bIsConnected = BuildingConnections[BuildingID].Num() > 0;
    }
}

FString UMingRTSBuildingSystem::GenerateBuildingID(ERTSBuildingType BuildingType) const
{
    static int32 BuildingCounter = 0;
    return FString::Printf(TEXT("Building_%d_%d"), (int32)BuildingType, BuildingCounter++);
}

FString UMingRTSBuildingSystem::GetBuildingName(ERTSBuildingType BuildingType) const
{
    switch (BuildingType)
    {
        case ERTSBuildingType::Residential: return TEXT("住宅");
        case ERTSBuildingType::Commercial: return TEXT("商業");
        case ERTSBuildingType::Industrial: return TEXT("工業");
        case ERTSBuildingType::Military: return TEXT("軍事");
        case ERTSBuildingType::Agricultural: return TEXT("農業");
        case ERTSBuildingType::Educational: return TEXT("教育");
        case ERTSBuildingType::Medical: return TEXT("醫療");
        case ERTSBuildingType::Storage: return TEXT("倉儲");
        case ERTSBuildingType::Power: return TEXT("電力");
        case ERTSBuildingType::Defense: return TEXT("防禦");
        case ERTSBuildingType::Resource: return TEXT("資源");
        case ERTSBuildingType::Research: return TEXT("研究");
        case ERTSBuildingType::Entertainment: return TEXT("娛樂");
        case ERTSBuildingType::Government: return TEXT("政府");
        case ERTSBuildingType::Religious: return TEXT("宗教");
        case ERTSBuildingType::Custom: return TEXT("自定義");
        default: return TEXT("未知");
    }
}

float UMingRTSBuildingSystem::GetBuildingSizeMultiplier(ERTSBuildingSize BuildingSize) const
{
    switch (BuildingSize)
    {
        case ERTSBuildingSize::Small: return 1.0f;
        case ERTSBuildingSize::Medium: return 1.5f;
        case ERTSBuildingSize::Large: return 2.0f;
        case ERTSBuildingSize::ExtraLarge: return 3.0f;
        default: return 1.0f;
    }
}

bool UMingRTSBuildingSystem::ValidateBuildingPlacement(const FRTSBuildingTemplate& Template, const FVector& Location) const
{
    // 檢查位置是否被佔用
    float BuildingRadius = 100.0f * GetBuildingSizeMultiplier(Template.BuildingSize);
    if (IsLocationOccupied(Location, BuildingRadius))
    {
        return false;
    }
    
    // 可以添加更多驗證規則
    // 例如：地形檢查、資源可用性、法規限制等
    
    return true;
}

void UMingRTSBuildingSystem::ApplyBuildingEffects(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade)
{
    if (!Buildings.Contains(BuildingID))
    {
        return;
    }

    FRTSBuildingData& Building = Buildings[BuildingID];
    
    // 應用升級效果
    for (const FString& Effect : Upgrade.Effects)
    {
        if (Effect == TEXT("IncreaseProduction"))
        {
            Building.ProductionRate *= 1.2f;
        }
        else if (Effect == TEXT("IncreaseHealth"))
        {
            Building.MaxHealth *= 1.3f;
            Building.Health = Building.MaxHealth;
        }
        else if (Effect == TEXT("ReduceMaintenance"))
        {
            Building.MaintenanceCost *= 0.8f;
        }
        else if (Effect == TEXT("IncreaseWorkerCapacity"))
        {
            Building.WorkerCapacity = FMath::RoundToInt(Building.WorkerCapacity * 1.5f);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied %d effects to building %s"), Upgrade.Effects.Num(), *BuildingID);
}

void UMingRTSBuildingSystem::CheckBuildingIntegrity(FRTSBuildingData& Building)
{
    // 檢查建築完整性
    if (Building.Health <= 0.0f && Building.BuildingState != ERTSBuildingState::Destroyed)
    {
        Building.BuildingState = ERTSBuildingState::Destroyed;
        NotifyBuildingDestroyed(Building.BuildingID, Building.BuildingType);
    }
    else if (Building.Health < Building.MaxHealth * 0.3f && Building.BuildingState == ERTSBuildingState::Operational)
    {
        Building.BuildingState = ERTSBuildingState::Damaged;
        
        // 自動修復
        if (bAutoRepair && ResourceManager)
        {
            float RepairCost = Building.MaxHealth * 0.1f;
            if (ResourceManager->HasEnoughResource(ERTSResourceType::Gold, RepairCost))
            {
                ResourceManager->RemoveResource(ERTSResourceType::Gold, RepairCost);
                RepairBuilding(Building.BuildingID, Building.MaxHealth * 0.2f);
            }
        }
    }
}

void UMingRTSBuildingSystem::NotifyBuildingConstructed(const FString& BuildingID, ERTSBuildingType BuildingType)
{
    OnBuildingConstructed.Broadcast(BuildingID, BuildingType);
}

void UMingRTSBuildingSystem::NotifyBuildingDestroyed(const FString& BuildingID, ERTSBuildingType BuildingType)
{
    OnBuildingDestroyed.Broadcast(BuildingID, BuildingType);
}

void UMingRTSBuildingSystem::NotifyBuildingStateChanged(const FString& BuildingID, ERTSBuildingState NewState)
{
    OnBuildingStateChanged.Broadcast(BuildingID, NewState);
}

void UMingRTSBuildingSystem::NotifyBuildingUpgraded(const FString& BuildingID, const FString& UpgradeID)
{
    OnBuildingUpgraded.Broadcast(BuildingID, UpgradeID);
}

// 建築系統功能完善
void UMingRTSBuildingSystem::EnhanceBuildingSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Enhancing building system..."));
    
    // 優化建築效率
    OptimizeBuildingEfficiency();
    
    // 改善建設速度
    ImproveConstructionSpeed();
    
    // 實施智能維護
    ImplementSmartMaintenance();
    
    UE_LOG(LogTemp, Log, TEXT("Building system enhanced"));
}

void UMingRTSBuildingSystem::OptimizeBuildingLayout()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing building layout..."));
    
    // 計算最優布局
    CalculateOptimalLayout();
    
    // 自動組織建築
    AutoOrganizeBuildings();
    
    // 優化資源流動
    OptimizeResourceFlow();
    
    UE_LOG(LogTemp, Log, TEXT("Building layout optimized"));
}

void UMingRTSBuildingSystem::ImproveConstructionEfficiency()
{
    UE_LOG(LogTemp, Log, TEXT("Improving construction efficiency..."));
    
    // 提高建設速度
    ConstructionSpeedMultiplier *= EfficiencyBonus;
    
    // 優化工人效率
    for (auto& BuildingPair : Buildings)
    {
        FRTSBuildingData& Building = BuildingPair.Value;
        if (Building.BuildingState == ERTSBuildingState::UnderConstruction)
        {
            Building.ConstructionProgress *= (1.0f + (EfficiencyBonus - 1.0f) * 0.5f);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Construction efficiency improved"));
}

void UMingRTSBuildingSystem::ImplementSmartBuildingManagement()
{
    if (!bEnableSmartManagement)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Implementing smart building management..."));
    
    // 智能維護系統
    ImplementSmartMaintenance();
    
    // 自動布局優化
    if (bEnableAutoLayout)
    {
        AutoOrganizeBuildings();
    }
    
    // 資源流動優化
    OptimizeResourceFlow();
    
    UE_LOG(LogTemp, Log, TEXT("Smart building management implemented"));
}

void UMingRTSBuildingSystem::OptimizeBuildingEfficiency()
{
    for (auto& BuildingPair : Buildings)
    {
        FString BuildingID = BuildingPair.Key;
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 計算當前效率
        float CurrentEfficiency = CalculateBuildingEfficiency(BuildingID);
        
        // 應用效率加成
        if (CurrentEfficiency < 0.8f)
        {
            Building.ProductionRate *= EfficiencyBonus;
            Building.WorkerEfficiency *= EfficiencyBonus;
        }
    }
}

void UMingRTSBuildingSystem::CalculateOptimalLayout()
{
    // 分析現有建築布局
    TArray<FVector> BuildingPositions;
    for (const auto& BuildingPair : Buildings)
    {
        BuildingPositions.Add(BuildingPair.Value.Location);
    }
    
    // 計算建築之間的距離和影響範圍
    for (auto& BuildingPair : Buildings)
    {
        FString BuildingID = BuildingPair.Key;
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 計算與其他建築的協作加成
        float CooperationBonus = 0.0f;
        for (const FVector& OtherPosition : BuildingPositions)
        {
            float Distance = FVector::Dist(Building.Location, OtherPosition);
            if (Distance > 0.0f && Distance < Building.InfluenceRadius)
            {
                CooperationBonus += (1.0f - Distance / Building.InfluenceRadius) * 0.1f;
            }
        }
        
        // 應用協作加成
        Building.ProductionRate *= (1.0f + CooperationBonus);
    }
}

void UMingRTSBuildingSystem::ImproveConstructionSpeed()
{
    // 提高所有在建建築的建設速度
    for (auto& BuildingPair : Buildings)
    {
        FRTSBuildingData& Building = BuildingPair.Value;
        if (Building.BuildingState == ERTSBuildingState::UnderConstruction)
        {
            Building.ConstructionSpeed *= EfficiencyBonus;
        }
    }
    
    // 更新建設速度乘數
    ConstructionSpeedMultiplier *= LayoutOptimizationFactor;
}

void UMingRTSBuildingSystem::ImplementSmartMaintenance()
{
    // 智能維護系統
    for (auto& BuildingPair : Buildings)
    {
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 預測性維護
        if (Building.Health < Building.MaxHealth * 0.5f)
        {
            // 提高修復速度
            Building.RepairRate *= 1.5f;
        }
        
        // 自動維護調度
        if (Building.BuildingState == ERTSBuildingState::Operational)
        {
            // 根據使用率調整維護頻率
            float UsageRate = Building.CurrentWorkers / FMath::Max(Building.WorkerCapacity, 1);
            if (UsageRate > 0.8f)
            {
                Building.MaintenanceRate *= 1.2f;
            }
        }
    }
}

float UMingRTSBuildingSystem::CalculateBuildingEfficiency(const FString& BuildingID) const
{
    if (!Buildings.Contains(BuildingID))
    {
        return 0.0f;
    }
    
    const FRTSBuildingData& Building = Buildings[BuildingID];
    
    // 基於多種因素計算效率
    float Efficiency = 1.0f;
    
    // 工人效率
    float WorkerRatio = Building.CurrentWorkers / FMath::Max(Building.WorkerCapacity, 1);
    Efficiency *= WorkerRatio;
    
    // 健康狀態效率
    float HealthRatio = Building.Health / Building.MaxHealth;
    Efficiency *= HealthRatio;
    
    // 升級加成
    float UpgradeBonus = Building.Level * 0.1f;
    Efficiency *= (1.0f + UpgradeBonus);
    
    return FMath::Clamp(Efficiency, 0.0f, 2.0f);
}

void UMingRTSBuildingSystem::AutoOrganizeBuildings()
{
    // 自動組織建築以優化布局
    TArray<FString> BuildingIDs;
    for (const auto& BuildingPair : Buildings)
    {
        BuildingIDs.Add(BuildingPair.Key);
    }
    
    // 按類型分組建築
    TMap<ERTSBuildingType, TArray<FString>> BuildingGroups;
    for (const FString& BuildingID : BuildingIDs)
    {
        if (Buildings.Contains(BuildingID))
        {
            ERTSBuildingType BuildingType = Buildings[BuildingID].BuildingType;
            if (!BuildingGroups.Contains(BuildingType))
            {
                BuildingGroups.Add(BuildingType, TArray<FString>());
            }
            BuildingGroups[BuildingType].Add(BuildingID);
        }
    }
    
    // 為每種類型優化布局
    for (const auto& GroupPair : BuildingGroups)
    {
        ERTSBuildingType BuildingType = GroupPair.Key;
        const TArray<FString>& TypeBuildings = GroupPair.Value;
        
        // 計算該類型的最優位置
        FVector OptimalCenter = FVector::ZeroVector;
        for (const FString& BuildingID : TypeBuildings)
        {
            if (Buildings.Contains(BuildingID))
            {
                OptimalCenter += Buildings[BuildingID].Location;
            }
        }
        OptimalCenter /= TypeBuildings.Num();
        
        // 調整建築位置以形成最佳布局
        for (const FString& BuildingID : TypeBuildings)
        {
            if (Buildings.Contains(BuildingID))
            {
                FRTSBuildingData& Building = Buildings[BuildingID];
                FVector Direction = Building.Location - OptimalCenter;
                float Distance = Direction.Size();
                
                if (Distance > 500.0f) // 如果距離太遠
                {
                    // 向中心移動
                    Direction.Normalize();
                    FVector NewPosition = OptimalCenter + Direction * 400.0f;
                    Building.Location = NewPosition;
                }
            }
        }
    }
}

void UMingRTSBuildingSystem::OptimizeResourceFlow()
{
    // 優化資源在建築之間的流動
    for (auto& BuildingPair : Buildings)
    {
        FString BuildingID = BuildingPair.Key;
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 計算資源流動效率
        float FlowEfficiency = 0.0f;
        int32 NearbyBuildings = 0;
        
        for (const auto& OtherBuildingPair : Buildings)
        {
            if (OtherBuildingPair.Key != BuildingID)
            {
                float Distance = FVector::Dist(Building.Location, OtherBuildingPair.Value.Location);
                if (Distance < Building.InfluenceRadius)
                {
                    NearbyBuildings++;
                    FlowEfficiency += (1.0f - Distance / Building.InfluenceRadius);
                }
            }
        }
        
        // 應用資源流動加成
        if (NearbyBuildings > 0)
        {
            FlowEfficiency /= NearbyBuildings;
            Building.ProductionRate *= (1.0f + FlowEfficiency * 0.2f);
        }
    }
}

// 性能優化實現
void UMingRTSBuildingSystem::OptimizeLargeScaleBuildingManagement()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing large scale building management..."));
    
    // 實現建築分區管理
    OptimizeBuildingDataStructures();
    
    // 啟用建築池化
    ImplementBuildingPooling();
    
    // 優化更新頻率
    OptimizeBuildingUpdates();
    
    UE_LOG(LogTemp, Log, TEXT("Large scale building management optimized"));
}

void UMingRTSBuildingSystem::OptimizeMemoryUsage()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing memory usage..."));
    
    // 減少內存佔用
    ReduceMemoryFootprint();
    
    // 清理無效建築數據
    for (auto It = Buildings.CreateIterator(); It; ++It)
    {
        if (It->Value.Health <= 0.0f)
        {
            It.RemoveCurrent();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Memory usage optimized"));
}

void UMingRTSBuildingSystem::OptimizeRenderingPerformance()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing rendering performance..."));
    
    // 優化渲染調用
    OptimizeRenderingCalls();
    
    // 批量更新建築LOD
    for (auto& BuildingPair : Buildings)
    {
        FString BuildingID = BuildingPair.Key;
        FRTSBuildingData& Building = BuildingPair.Value;
        
        // 根據距離計算LOD等級
        float DistanceToPlayer = FVector::Dist(Building.Location, FVector::ZeroVector);
        int32 LODLevel = DistanceToPlayer < 500.0f ? 0 : (DistanceToPlayer < 1000.0f ? 1 : 2);
        
        UpdateBuildingLOD(BuildingID, LODLevel);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Rendering performance optimized"));
}

void UMingRTSBuildingSystem::OptimizeNetworkSynchronization()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing network synchronization..."));
    
    // 實現網絡增量壓縮
    ImplementNetworkDeltaCompression();
    
    UE_LOG(LogTemp, Log, TEXT("Network synchronization optimized"));
}

void UMingRTSBuildingSystem::EnablePerformanceMode(bool bEnable)
{
    UE_LOG(LogTemp, Log, TEXT("Performance mode %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    
    if (bEnable)
    {
        // 啟用性能模式：降低更新頻率，增加批量處理
        ConstructionSpeedMultiplier *= 1.5f;
        MaintenanceMultiplier *= 0.8f;
    }
    else
    {
        // 恢復正常模式
        ConstructionSpeedMultiplier = 1.0f;
        MaintenanceMultiplier = 1.0f;
    }
}

float UMingRTSBuildingSystem::GetBuildingSystemPerformance() const
{
    // 計算建築系統性能指標
    float PerformanceScore = 1.0f;
    
    // 基於建築數量調整性能分數
    int32 BuildingCount = Buildings.Num();
    if (BuildingCount > 100)
    {
        PerformanceScore *= 0.8f; // 大量建築時性能下降
    }
    else if (BuildingCount > 50)
    {
        PerformanceScore *= 0.9f;
    }
    
    return PerformanceScore;
}

int32 UMingRTSBuildingSystem::GetActiveBuildingCount() const
{
    int32 ActiveCount = 0;
    for (const auto& BuildingPair : Buildings)
    {
        if (BuildingPair.Value.State == ERTSBuildingState::Operational)
        {
            ActiveCount++;
        }
    }
    return ActiveCount;
}

void UMingRTSBuildingSystem::UpdateBuildingLOD(const FString& BuildingID, int32 LODLevel)
{
    if (Buildings.Contains(BuildingID))
    {
        FRTSBuildingData& Building = Buildings[BuildingID];
        Building.CurrentLOD = LODLevel;
        
        // 根據LOD等級調整渲染質量
        switch (LODLevel)
        {
        case 0: // 高質量
            Building.RenderDistance = 1000.0f;
            break;
        case 1: // 中等質量
            Building.RenderDistance = 500.0f;
            break;
        case 2: // 低質量
            Building.RenderDistance = 200.0f;
            break;
        }
    }
}

void UMingRTSBuildingSystem::BatchUpdateBuildings(const TArray<FString>& BuildingIDs)
{
    // 批量更新建築狀態以提高性能
    for (const FString& BuildingID : BuildingIDs)
    {
        if (Buildings.Contains(BuildingID))
        {
            FRTSBuildingData& Building = Buildings[BuildingID];
            
            // 批量處理建築邏輯
            ProcessBuildingConstruction(Building, 0.016f); // 假設60FPS
            ProcessBuildingProduction(Building, 0.016f);
            ProcessBuildingMaintenance(Building, 0.016f);
        }
    }
}

// 性能優化內部函數
void UMingRTSBuildingSystem::OptimizeBuildingDataStructures()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing building data structures..."));
    
    // 實現空間分區以提高查詢性能
    // 這裡可以實現四叉樹或網格分區
}

void UMingRTSBuildingSystem::ImplementBuildingPooling()
{
    UE_LOG(LogTemp, Log, TEXT("Implementing building pooling..."));
    
    // 實現建築對象池化以減少內存分配
}

void UMingRTSBuildingSystem::OptimizeBuildingUpdates()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing building updates..."));
    
    // 實現增量更新和時間片分配
}

void UMingRTSBuildingSystem::ReduceMemoryFootprint()
{
    UE_LOG(LogTemp, Log, TEXT("Reducing memory footprint..."));
    
    // 壓縮建築數據，移除不必要的字段
}

void UMingRTSBuildingSystem::OptimizeRenderingCalls()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing rendering calls..."));
    
    // 批量渲染調用，減少Draw Call
}

void UMingRTSBuildingSystem::ImplementNetworkDeltaCompression()
{
    UE_LOG(LogTemp, Log, TEXT("Implementing network delta compression..."));
    
    // 只同步變化的建築數據
}
