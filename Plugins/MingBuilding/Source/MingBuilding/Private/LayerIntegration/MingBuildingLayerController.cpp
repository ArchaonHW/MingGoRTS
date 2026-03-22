#include "LayerIntegration/MingBuildingLayerController.h"

UMingBuildingLayerController::UMingBuildingLayerController()
    : BuildRadius(2000.0f)
    , bShowGrid(true)
    , NextBuildingID(1)
    , bIsActive(false)
{
}

void UMingBuildingLayerController::InitializeBuildingLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing Building Layer Controller"));
    
    // Initialize default base
    CurrentBase.BaseID = FGuid::NewGuid().ToString();
    CurrentBase.BaseName = TEXT("Main Base");
    CurrentBase.CenterLocation = FVector::ZeroVector;
    CurrentBase.Radius = BuildRadius;
    CurrentBase.TotalWorkers = 0;
    CurrentBase.MaxWorkers = 100;
    
    // Initialize default buildings
    InitializeDefaultBuildings();
    
    UE_LOG(LogTemp, Log, TEXT("Building Layer initialized with base: %s"), *CurrentBase.BaseName);
}

void UMingBuildingLayerController::ShutdownBuildingLayer()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down Building Layer Controller"));
    
    // Clear all data
    Buildings.Empty();
    ProductionQueues.Empty();
    SelectedBuildingIDs.Empty();
    
    bIsActive = false;
}

void UMingBuildingLayerController::ActivateBuildingLayer()
{
    bIsActive = true;
    UE_LOG(LogTemp, Log, TEXT("Building Layer activated"));
}

void UMingBuildingLayerController::DeactivateBuildingLayer()
{
    bIsActive = false;
    UE_LOG(LogTemp, Log, TEXT("Building Layer deactivated"));
}

// Base Management
void UMingBuildingLayerController::LoadBase(const FString& BaseID)
{
    UE_LOG(LogTemp, Log, TEXT("Loading base: %s"), *BaseID);
    CurrentBase.BaseID = BaseID;
    // In real implementation, would load from save data
}

void UMingBuildingLayerController::CreateNewBase(const FString& BaseName, FVector Location, const FString& FactionID)
{
    CurrentBase.BaseID = FGuid::NewGuid().ToString();
    CurrentBase.BaseName = BaseName;
    CurrentBase.CenterLocation = Location;
    CurrentBase.FactionID = FactionID;
    CurrentBase.BuildingIDs.Empty();
    CurrentBase.TotalWorkers = 0;
    
    Buildings.Empty();
    ProductionQueues.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Created new base: %s at %s"), *BaseName, *Location.ToString());
}

void UMingBuildingLayerController::ClearBase()
{
    Buildings.Empty();
    ProductionQueues.Empty();
    SelectedBuildingIDs.Empty();
    CurrentBase.BuildingIDs.Empty();
    CurrentBase.TotalWorkers = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Base cleared"));
}

bool UMingBuildingLayerController::GetCurrentBase(FMingBaseLayout& OutBase) const
{
    OutBase = CurrentBase;
    return !CurrentBase.BaseID.IsEmpty();
}

void UMingBuildingLayerController::SetBaseUnderAttack(bool bUnderAttack)
{
    CurrentBase.bIsUnderAttack = bUnderAttack;
    
    if (bUnderAttack)
    {
        OnBaseAttacked.Broadcast(CurrentBase.BaseID);
        UE_LOG(LogTemp, Warning, TEXT("Base %s is under attack!"), *CurrentBase.BaseName);
    }
}

void UMingBuildingLayerController::DefendBase()
{
    // Activate all defense structures
    for (auto& Pair : Buildings)
    {
        if (Pair.Value.BuildingType == EMingBuildingType::DefenseTower)
        {
            UE_LOG(LogTemp, Log, TEXT("Defense tower %s activated"), *Pair.Value.BuildingName);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Base defense activated"));
}

// Building Management
FString UMingBuildingLayerController::PlaceBuildingBlueprint(EMingBuildingType Type, FVector Location, FRotator Rotation)
{
    FString BuildingID = GenerateBuildingID();
    
    FMingBuildingInstance Building;
    Building.BuildingID = BuildingID;
    Building.BuildingName = GetBuildingTypeName(Type);
    Building.BuildingType = Type;
    Building.Status = EMingBuildStatus::Planned;
    Building.Location = SnapToGrid(Location);
    Building.Rotation = Rotation;
    Building.Level = 1;
    Building.MaxLevel = 5;
    Building.Health = 10.0f; // Blueprint has minimal health
    Building.MaxHealth = 100.0f;
    Building.BuildTime = CalculateBuildTime(Type, 1);
    Building.MaxWorkers = CalculateMaxWorkers(Type);
    Building.FactionID = CurrentBase.FactionID;
    
    // Set resource costs
    SetBuildingResourceCosts(Building);
    
    Buildings.Add(BuildingID, Building);
    CurrentBase.BuildingIDs.Add(BuildingID);
    
    UE_LOG(LogTemp, Log, TEXT("Placed building blueprint: %s (%s)"), *BuildingID, *Building.BuildingName);
    
    return BuildingID;
}

void UMingBuildingLayerController::StartConstruction(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    if (!CanAffordBuilding(*Building))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot afford building %s"), *BuildingID);
        return;
    }
    
    DeductBuildingCost(*Building);
    
    Building->Status = EMingBuildStatus::UnderConstruction;
    Building->ConstructionProgress = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Started construction: %s"), *Building->BuildingName);
}

void UMingBuildingLayerController::CancelConstruction(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    if (Building->Status == EMingBuildStatus::UnderConstruction)
    {
        RefundBuildingCost(*Building);
        Building->Status = EMingBuildStatus::Planned;
        UE_LOG(LogTemp, Log, TEXT("Construction cancelled: %s"), *Building->BuildingName);
    }
}

void UMingBuildingLayerController::CompleteConstruction(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    Building->Status = EMingBuildStatus::Active;
    Building->ConstructionProgress = 1.0f;
    Building->Health = Building->MaxHealth;
    
    // Initialize production queue for production buildings
    if (IsProductionBuilding(Building->BuildingType))
    {
        FMingProductionQueue Queue;
        Queue.BuildingID = BuildingID;
        ProductionQueues.Add(BuildingID, Queue);
    }
    
    OnBuildingConstructed.Broadcast(*Building);
    
    UE_LOG(LogTemp, Log, TEXT("Construction completed: %s"), *Building->BuildingName);
}

void UMingBuildingLayerController::UpgradeBuilding(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    if (Building->Level >= Building->MaxLevel)
    {
        UE_LOG(LogTemp, Warning, TEXT("Building %s is already at max level"), *BuildingID);
        return;
    }
    
    // Check if can afford upgrade
    int32 UpgradeCost = Building->Level * 100;
    // In real implementation, would check faction resources
    
    Building->Status = EMingBuildStatus::Upgrading;
    Building->Level++;
    Building->MaxHealth += 50.0f;
    Building->Health = Building->MaxHealth;
    Building->MaxWorkers += 2;
    
    Building->Status = EMingBuildStatus::Active;
    
    OnBuildingUpgraded.Broadcast(*Building);
    
    UE_LOG(LogTemp, Log, TEXT("Building upgraded to level %d: %s"), Building->Level, *Building->BuildingName);
}

void UMingBuildingLayerController::DemolishBuilding(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    // Refund some resources
    RefundBuildingCost(*Building);
    
    UE_LOG(LogTemp, Log, TEXT("Building demolished: %s"), *Building->BuildingName);
    
    OnBuildingDestroyed.Broadcast(BuildingID);
    
    Buildings.Remove(BuildingID);
    ProductionQueues.Remove(BuildingID);
    CurrentBase.BuildingIDs.Remove(BuildingID);
    SelectedBuildingIDs.Remove(BuildingID);
}

void UMingBuildingLayerController::RepairBuilding(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    if (Building->Status == EMingBuildStatus::Destroyed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot repair destroyed building %s"), *BuildingID);
        return;
    }
    
    Building->Health = Building->MaxHealth;
    
    if (Building->Status == EMingBuildStatus::Damaged)
    {
        Building->Status = EMingBuildStatus::Active;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Building repaired: %s"), *Building->BuildingName);
}

bool UMingBuildingLayerController::GetBuilding(const FString& BuildingID, FMingBuildingInstance& OutBuilding) const
{
    const FMingBuildingInstance* Found = Buildings.Find(BuildingID);
    if (Found)
    {
        OutBuilding = *Found;
        return true;
    }
    return false;
}

TArray<FMingBuildingInstance> UMingBuildingLayerController::GetAllBuildings() const
{
    TArray<FMingBuildingInstance> Result;
    Buildings.GenerateValueArray(Result);
    return Result;
}

TArray<FMingBuildingInstance> UMingBuildingLayerController::GetBuildingsByType(EMingBuildingType Type) const
{
    TArray<FMingBuildingInstance> Result;
    
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.BuildingType == Type)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FMingBuildingInstance> UMingBuildingLayerController::GetActiveBuildings() const
{
    TArray<FMingBuildingInstance> Result;
    
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.Status == EMingBuildStatus::Active)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

TArray<FMingBuildingInstance> UMingBuildingLayerController::GetUnderConstructionBuildings() const
{
    TArray<FMingBuildingInstance> Result;
    
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.Status == EMingBuildStatus::UnderConstruction)
        {
            Result.Add(Pair.Value);
        }
    }
    
    return Result;
}

void UMingBuildingLayerController::SelectBuilding(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    Building->bIsSelected = true;
    SelectedBuildingIDs.AddUnique(BuildingID);
    
    OnBuildingSelected.Broadcast(BuildingID, true);
}

void UMingBuildingLayerController::DeselectBuilding(const FString& BuildingID)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (Building)
    {
        Building->bIsSelected = false;
    }
    
    SelectedBuildingIDs.Remove(BuildingID);
    OnBuildingSelected.Broadcast(BuildingID, false);
}

void UMingBuildingLayerController::ClearBuildingSelection()
{
    for (const FString& ID : SelectedBuildingIDs)
    {
        FMingBuildingInstance* Building = Buildings.Find(ID);
        if (Building)
        {
            Building->bIsSelected = false;
        }
    }
    
    SelectedBuildingIDs.Empty();
}

// Worker Management
void UMingBuildingLayerController::AssignWorkers(const FString& BuildingID, int32 WorkerCount)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    int32 AvailableWorkers = GetAvailableWorkers();
    int32 CanAssign = FMath::Min(WorkerCount, AvailableWorkers);
    CanAssign = FMath::Min(CanAssign, Building->MaxWorkers - Building->WorkerCount);
    
    Building->WorkerCount += CanAssign;
    
    UE_LOG(LogTemp, Log, TEXT("Assigned %d workers to %s. Total: %d/%d"), 
        CanAssign, *Building->BuildingName, Building->WorkerCount, Building->MaxWorkers);
}

void UMingBuildingLayerController::RemoveWorkers(const FString& BuildingID, int32 WorkerCount)
{
    FMingBuildingInstance* Building = Buildings.Find(BuildingID);
    if (!Building)
    {
        return;
    }
    
    int32 CanRemove = FMath::Min(WorkerCount, Building->WorkerCount);
    Building->WorkerCount -= CanRemove;
    
    UE_LOG(LogTemp, Log, TEXT("Removed %d workers from %s. Remaining: %d"), 
        CanRemove, *Building->BuildingName, Building->WorkerCount);
}

void UMingBuildingLayerController::SetBuildingPriority(const FString& BuildingID, int32 Priority)
{
    UE_LOG(LogTemp, Log, TEXT("Set building %s priority to %d"), *BuildingID, Priority);
}

int32 UMingBuildingLayerController::GetAvailableWorkers() const
{
    int32 AssignedWorkers = 0;
    for (const auto& Pair : Buildings)
    {
        AssignedWorkers += Pair.Value.WorkerCount;
    }
    
    return CurrentBase.MaxWorkers - AssignedWorkers;
}

int32 UMingBuildingLayerController::GetTotalWorkers() const
{
    int32 Total = 0;
    for (const auto& Pair : Buildings)
    {
        Total += Pair.Value.WorkerCount;
    }
    return Total;
}

void UMingBuildingLayerController::HireWorkers(int32 Count)
{
    CurrentBase.MaxWorkers += Count;
    UE_LOG(LogTemp, Log, TEXT("Hired %d workers. Max workers now: %d"), Count, CurrentBase.MaxWorkers);
}

void UMingBuildingLayerController::ReassignWorkers(const FString& FromBuildingID, const FString& ToBuildingID, int32 Count)
{
    RemoveWorkers(FromBuildingID, Count);
    AssignWorkers(ToBuildingID, Count);
}

// Production Management
void UMingBuildingLayerController::QueueUnitProduction(const FString& BuildingID, const FString& UnitType)
{
    FMingProductionQueue* Queue = ProductionQueues.Find(BuildingID);
    if (!Queue)
    {
        return;
    }
    
    Queue->QueueItems.Add(UnitType);
    
    UE_LOG(LogTemp, Log, TEXT("Queued %s production in building %s"), *UnitType, *BuildingID);
}

void UMingBuildingLayerController::CancelUnitProduction(const FString& BuildingID, int32 QueueIndex)
{
    FMingProductionQueue* Queue = ProductionQueues.Find(BuildingID);
    if (!Queue || QueueIndex < 0 || QueueIndex >= Queue->QueueItems.Num())
    {
        return;
    }
    
    Queue->QueueItems.RemoveAt(QueueIndex);
    
    UE_LOG(LogTemp, Log, TEXT("Cancelled production at index %d in building %s"), QueueIndex, *BuildingID);
}

void UMingBuildingLayerController::PauseProduction(const FString& BuildingID)
{
    FMingProductionQueue* Queue = ProductionQueues.Find(BuildingID);
    if (Queue)
    {
        Queue->bIsPaused = true;
        UE_LOG(LogTemp, Log, TEXT("Production paused in building %s"), *BuildingID);
    }
}

void UMingBuildingLayerController::ResumeProduction(const FString& BuildingID)
{
    FMingProductionQueue* Queue = ProductionQueues.Find(BuildingID);
    if (Queue)
    {
        Queue->bIsPaused = false;
        UE_LOG(LogTemp, Log, TEXT("Production resumed in building %s"), *BuildingID);
    }
}

void UMingBuildingLayerController::RushProduction(const FString& BuildingID)
{
    FMingProductionQueue* Queue = ProductionQueues.Find(BuildingID);
    if (!Queue)
    {
        return;
    }
    
    // In real implementation, would spend resources to rush
    Queue->CurrentProgress = Queue->ProductionTime;
    
    UE_LOG(LogTemp, Log, TEXT("Production rushed in building %s"), *BuildingID);
}

FMingProductionQueue UMingBuildingLayerController::GetProductionQueue(const FString& BuildingID) const
{
    const FMingProductionQueue* Found = ProductionQueues.Find(BuildingID);
    if (Found)
    {
        return *Found;
    }
    return FMingProductionQueue();
}

// Resource Management
FMingResourceFlow UMingBuildingLayerController::CalculateResourceFlow() const
{
    FMingResourceFlow Flow;
    Flow.Efficiency = GetBaseEfficiency();
    
    // Calculate income from resource buildings
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.Status == EMingBuildStatus::Active)
        {
            switch (Pair.Value.BuildingType)
            {
            case EMingBuildingType::Farm:
                Flow.Income.FindOrAdd(TEXT("Food")) += 10 * Pair.Value.WorkerCount;
                break;
            case EMingBuildingType::Mine:
                Flow.Income.FindOrAdd(TEXT("Minerals")) += 5 * Pair.Value.WorkerCount;
                break;
            case EMingBuildingType::Generator:
                Flow.Income.FindOrAdd(TEXT("Power")) += 20;
                break;
            default:
                break;
            }
            
            // Power consumption
            if (Pair.Value.BuildingType != EMingBuildingType::Generator)
            {
                Flow.Expenses.FindOrAdd(TEXT("Power")) += 2;
            }
        }
    }
    
    // Calculate net flow
    for (const auto& Pair : Flow.Income)
    {
        Flow.NetFlow.Add(Pair.Key, Pair.Value - Flow.Expenses.FindRef(Pair.Key));
    }
    
    return Flow;
}

int32 UMingBuildingLayerController::GetResourceProduction(const FString& ResourceType) const
{
    FMingResourceFlow Flow = CalculateResourceFlow();
    return Flow.Income.FindRef(ResourceType);
}

int32 UMingBuildingLayerController::GetResourceConsumption(const FString& ResourceType) const
{
    FMingResourceFlow Flow = CalculateResourceFlow();
    return Flow.Expenses.FindRef(ResourceType);
}

float UMingBuildingLayerController::GetPowerProduction() const
{
    return GetResourceProduction(TEXT("Power"));
}

float UMingBuildingLayerController::GetPowerConsumption() const
{
    return GetResourceConsumption(TEXT("Power"));
}

bool UMingBuildingLayerController::HasPowerSurplus() const
{
    return GetPowerProduction() > GetPowerConsumption();
}

void UMingBuildingLayerController::OptimizeResourceDistribution()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing resource distribution"));
    
    // In real implementation, would redistribute workers for optimal efficiency
}

// Grid & Placement
FVector UMingBuildingLayerController::SnapToGrid(FVector Location, float GridSize)
{
    return FVector(
        FMath::RoundToFloat(Location.X / GridSize) * GridSize,
        FMath::RoundToFloat(Location.Y / GridSize) * GridSize,
        Location.Z
    );
}

bool UMingBuildingLayerController::IsValidBuildLocation(FVector Location, float Radius) const
{
    // Check if within base radius
    if (FVector::Dist(Location, CurrentBase.CenterLocation) > CurrentBase.Radius)
    {
        return false;
    }
    
    // Check for collision with existing buildings
    for (const auto& Pair : Buildings)
    {
        if (FVector::Dist(Location, Pair.Value.Location) < Radius + 50.0f)
        {
            return false;
        }
    }
    
    return true;
}

void UMingBuildingLayerController::SetBuildRadius(float Radius)
{
    BuildRadius = FMath::Clamp(Radius, 500.0f, 5000.0f);
    CurrentBase.Radius = BuildRadius;
}

void UMingBuildingLayerController::ShowBuildableAreas()
{
    bShowGrid = true;
    UE_LOG(LogTemp, Log, TEXT("Showing buildable areas"));
}

void UMingBuildingLayerController::HideBuildableAreas()
{
    bShowGrid = false;
    UE_LOG(LogTemp, Log, TEXT("Hiding buildable areas"));
}

FVector UMingBuildingLayerController::FindNearestValidLocation(FVector DesiredLocation, EMingBuildingType Type) const
{
    FVector BestLocation = DesiredLocation;
    float BestDistance = 0.0f;
    
    // Search in spiral pattern
    for (float Radius = 0.0f; Radius < CurrentBase.Radius; Radius += 100.0f)
    {
        for (int32 Angle = 0; Angle < 360; Angle += 45)
        {
            float Rad = FMath::DegreesToRadians(Angle);
            FVector TestLocation = CurrentBase.CenterLocation + FVector(
                FMath::Cos(Rad) * Radius,
                FMath::Sin(Rad) * Radius,
                0
            );
            
            if (IsValidBuildLocation(TestLocation, 50.0f))
            {
                float Dist = FVector::Dist(TestLocation, DesiredLocation);
                if (BestDistance == 0.0f || Dist < BestDistance)
                {
                    BestDistance = Dist;
                    BestLocation = TestLocation;
                }
            }
        }
        
        if (BestDistance > 0.0f && BestDistance < 200.0f)
        {
            break;
        }
    }
    
    return SnapToGrid(BestLocation);
}

// Camera & View
void UMingBuildingLayerController::FocusOnBuilding(const FString& BuildingID)
{
    FMingBuildingInstance Building;
    if (GetBuilding(BuildingID, Building))
    {
        UE_LOG(LogTemp, Log, TEXT("Camera focused on building: %s"), *Building.BuildingName);
    }
}

void UMingBuildingLayerController::FocusOnBaseCenter()
{
    UE_LOG(LogTemp, Log, TEXT("Camera focused on base center: %s"), *CurrentBase.BaseName);
}

void UMingBuildingLayerController::SetOverviewCamera()
{
    UE_LOG(LogTemp, Log, TEXT("Camera set to overview mode"));
}

void UMingBuildingLayerController::SetFollowCamera(const FString& BuildingID)
{
    UE_LOG(LogTemp, Log, TEXT("Camera following building: %s"), *BuildingID);
}

void UMingBuildingLayerController::ZoomCamera(float Amount)
{
    UE_LOG(LogTemp, Log, TEXT("Camera zoom: %f"), Amount);
}

void UMingBuildingLayerController::PanCamera(FVector Direction)
{
    UE_LOG(LogTemp, Log, TEXT("Camera pan: %s"), *Direction.ToString());
}

void UMingBuildingLayerController::RotateCamera(float Angle)
{
    UE_LOG(LogTemp, Log, TEXT("Camera rotate: %f"), Angle);
}

// Layer Integration
void UMingBuildingLayerController::RequestStrategicSupport(const FString& SupportType)
{
    UE_LOG(LogTemp, Log, TEXT("Requesting strategic support: %s"), *SupportType);
}

void UMingBuildingLayerController::SendResourcesToStrategic(TMap<FString, int32> Resources)
{
    UE_LOG(LogTemp, Log, TEXT("Sending resources to strategic layer"));
    
    for (const auto& Pair : Resources)
    {
        UE_LOG(LogTemp, Log, TEXT("  %s: %d"), *Pair.Key, Pair.Value);
    }
}

void UMingBuildingLayerController::ReceiveUnitFromTactical(const FString& UnitID)
{
    UE_LOG(LogTemp, Log, TEXT("Received unit from tactical layer: %s"), *UnitID);
}

void UMingBuildingLayerController::ReturnToStrategic()
{
    UE_LOG(LogTemp, Log, TEXT("Returning to strategic layer"));
}

// Utility Functions
int32 UMingBuildingLayerController::GetBuildingLimit() const
{
    return 50; // Configurable limit
}

int32 UMingBuildingLayerController::GetCurrentBuildingCount() const
{
    return Buildings.Num();
}

float UMingBuildingLayerController::GetBaseDefenseRating() const
{
    float Defense = 0.0f;
    
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.BuildingType == EMingBuildingType::DefenseTower && 
            Pair.Value.Status == EMingBuildStatus::Active)
        {
            Defense += 10.0f * Pair.Value.Level;
        }
        else if (Pair.Value.BuildingType == EMingBuildingType::Wall &&
                 Pair.Value.Status == EMingBuildStatus::Active)
        {
            Defense += 5.0f * Pair.Value.Level;
        }
    }
    
    return Defense;
}

float UMingBuildingLayerController::GetBaseEfficiency() const
{
    float TotalEfficiency = 0.0f;
    int32 ActiveCount = 0;
    
    for (const auto& Pair : Buildings)
    {
        if (Pair.Value.Status == EMingBuildStatus::Active)
        {
            // Efficiency based on worker count vs max workers
            float BuildingEfficiency = static_cast<float>(Pair.Value.WorkerCount) / Pair.Value.MaxWorkers;
            TotalEfficiency += BuildingEfficiency;
            ActiveCount++;
        }
    }
    
    if (ActiveCount == 0)
    {
        return 1.0f;
    }
    
    return TotalEfficiency / ActiveCount;
}

void UMingBuildingLayerController::ToggleBuildingVisibility(EMingBuildingType Type, bool bVisible)
{
    UE_LOG(LogTemp, Log, TEXT("Toggled %s visibility to %s"), 
        *GetBuildingTypeName(Type), bVisible ? TEXT("visible") : TEXT("hidden"));
}

void UMingBuildingLayerController::ToggleGridDisplay(bool bShow)
{
    bShowGrid = bShow;
}

// Internal functions
void UMingBuildingLayerController::InitializeDefaultBuildings()
{
    // Headquarters
    FString HQID = PlaceBuildingBlueprint(EMingBuildingType::Headquarters, FVector(0, 0, 0), FRotator::ZeroRotator);
    FMingBuildingInstance* HQ = Buildings.Find(HQID);
    if (HQ)
    {
        HQ->Status = EMingBuildStatus::Active;
        HQ->Health = HQ->MaxHealth;
        HQ->Level = 3;
    }
    
    // Barracks
    FString BarracksID = PlaceBuildingBlueprint(EMingBuildingType::Barracks, FVector(200, 0, 0), FRotator::ZeroRotator);
    FMingBuildingInstance* Barracks = Buildings.Find(BarracksID);
    if (Barracks)
    {
        Barracks->Status = EMingBuildStatus::Active;
        Barracks->Health = Barracks->MaxHealth;
    }
    
    // Factory
    FString FactoryID = PlaceBuildingBlueprint(EMingBuildingType::Factory, FVector(-200, 100, 0), FRotator::ZeroRotator);
    FMingBuildingInstance* Factory = Buildings.Find(FactoryID);
    if (Factory)
    {
        Factory->Status = EMingBuildStatus::Active;
        Factory->Health = Factory->MaxHealth;
    }
    
    // Warehouse
    FString WarehouseID = PlaceBuildingBlueprint(EMingBuildingType::Warehouse, FVector(0, 200, 0), FRotator::ZeroRotator);
    FMingBuildingInstance* Warehouse = Buildings.Find(WarehouseID);
    if (Warehouse)
    {
        Warehouse->Status = EMingBuildStatus::Active;
        Warehouse->Health = Warehouse->MaxHealth;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Initialized %d default buildings"), Buildings.Num());
}

FString UMingBuildingLayerController::GenerateBuildingID()
{
    return FString::Printf(TEXT("Building_%d"), NextBuildingID++);
}

FString UMingBuildingLayerController::GetBuildingTypeName(EMingBuildingType Type) const
{
    switch (Type)
    {
    case EMingBuildingType::Headquarters: return TEXT("Headquarters");
    case EMingBuildingType::Barracks: return TEXT("Barracks");
    case EMingBuildingType::Factory: return TEXT("Factory");
    case EMingBuildingType::Armory: return TEXT("Armory");
    case EMingBuildingType::Warehouse: return TEXT("Warehouse");
    case EMingBuildingType::ResearchLab: return TEXT("Research Lab");
    case EMingBuildingType::MedicalStation: return TEXT("Medical Station");
    case EMingBuildingType::TrainingGround: return TEXT("Training Ground");
    case EMingBuildingType::DefenseTower: return TEXT("Defense Tower");
    case EMingBuildingType::Wall: return TEXT("Wall");
    case EMingBuildingType::Generator: return TEXT("Generator");
    case EMingBuildingType::Farm: return TEXT("Farm");
    case EMingBuildingType::Mine: return TEXT("Mine");
    case EMingBuildingType::Workshop: return TEXT("Workshop");
    default: return TEXT("Unknown");
    }
}

void UMingBuildingLayerController::SetBuildingResourceCosts(FMingBuildingInstance& Building)
{
    // Set default costs based on building type
    switch (Building.BuildingType)
    {
    case EMingBuildingType::Barracks:
        Building.ResourceCosts.Add(TEXT("Minerals"), 100);
        Building.ResourceCosts.Add(TEXT("Power"), 10);
        break;
    case EMingBuildingType::Factory:
        Building.ResourceCosts.Add(TEXT("Minerals"), 200);
        Building.ResourceCosts.Add(TEXT("Power"), 20);
        break;
    case EMingBuildingType::DefenseTower:
        Building.ResourceCosts.Add(TEXT("Minerals"), 150);
        Building.ResourceCosts.Add(TEXT("Power"), 5);
        break;
    case EMingBuildingType::Farm:
        Building.ResourceCosts.Add(TEXT("Minerals"), 50);
        break;
    default:
        Building.ResourceCosts.Add(TEXT("Minerals"), 100);
        break;
    }
}

bool UMingBuildingLayerController::CanAffordBuilding(const FMingBuildingInstance& Building) const
{
    // In real implementation, would check faction resources
    return true;
}

void UMingBuildingLayerController::DeductBuildingCost(const FMingBuildingInstance& Building)
{
    // In real implementation, would deduct from faction resources
    UE_LOG(LogTemp, Log, TEXT("Deducted building costs for %s"), *Building.BuildingName);
}

void UMingBuildingLayerController::RefundBuildingCost(const FMingBuildingInstance& Building)
{
    // In real implementation, would refund to faction resources (partial refund)
    UE_LOG(LogTemp, Log, TEXT("Refunded building costs for %s"), *Building.BuildingName);
}

int32 UMingBuildingLayerController::CalculateMaxWorkers(EMingBuildingType Type) const
{
    switch (Type)
    {
    case EMingBuildingType::Headquarters: return 20;
    case EMingBuildingType::Factory: return 15;
    case EMingBuildingType::Mine: return 10;
    case EMingBuildingType::Farm: return 8;
    default: return 5;
    }
}

float UMingBuildingLayerController::CalculateBuildTime(EMingBuildingType Type, int32 Level) const
{
    float BaseTime = 60.0f;
    
    switch (Type)
    {
    case EMingBuildingType::Headquarters: BaseTime = 300.0f; break;
    case EMingBuildingType::Factory: BaseTime = 180.0f; break;
    case EMingBuildingType::Barracks: BaseTime = 120.0f; break;
    default: BaseTime = 60.0f; break;
    }
    
    return BaseTime * Level;
}

bool UMingBuildingLayerController::IsProductionBuilding(EMingBuildingType Type) const
{
    return Type == EMingBuildingType::Barracks ||
           Type == EMingBuildingType::Factory ||
           Type == EMingBuildingType::Workshop ||
           Type == EMingBuildingType::ResearchLab;
}
