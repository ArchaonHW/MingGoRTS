#include "MingBuildingActor.h"
#include "MingResourceProducer.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AMingBuildingActor::AMingBuildingActor()
{
    PrimaryActorTick.bCanEverTick = true;

    // Set default building properties
    BuildingType = EMingBuildingType::CommandCenter;
    BuildingName = TEXT("Base Building");
    BuildingState = EMingBuildingState::Planned;
    
    CurrentHealth = 1000;
    MaxHealth = 1000;
    ConstructionProgress = 0.0f;
    ConstructionSpeed = 10.0f;
    bIsUnderConstruction = false;
    AssignedWorkers = 0;
    MaxWorkers = 5;
    
    bIsOperational = false;
    bProvidesSupply = false;
    SupplyCapacity = 0;

    // Create components
    ConstructionZone = CreateDefaultSubobject<UBoxComponent>(TEXT("ConstructionZone"));
    RootComponent = ConstructionZone;
    ConstructionZone->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));

    BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BuildingMesh"));
    BuildingMesh->SetupAttachment(RootComponent);
    BuildingMesh->SetVisibility(false); // Hidden until construction starts

    ResourceProducer = CreateDefaultSubobject<UMingResourceProducer>(TEXT("ResourceProducer"));
}

void AMingBuildingActor::BeginPlay()
{
    Super::BeginPlay();

    // Try to find resource system
    if (UWorld* World = GetWorld())
    {
        // This is a simplified approach - in a real implementation you'd have a proper way to access the resource system
        // through the game mode or a game instance subsystem
    }

    UE_LOG(LogTemp, Log, TEXT("Building %s initialized"), *BuildingName);
}

void AMingBuildingActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bIsUnderConstruction)
    {
        UpdateConstruction(DeltaTime);
    }
}

void AMingBuildingActor::StartConstruction()
{
    if (bIsUnderConstruction || BuildingState == EMingBuildingState::Active)
    {
        return;
    }

    bIsUnderConstruction = true;
    BuildingState = EMingBuildingState::Constructing;
    ConstructionProgress = 0.0f;

    // Show construction mesh
    BuildingMesh->SetVisibility(true);
    
    // Apply semi-transparent or construction material effect
    UpdateVisualState();

    OnConstructionStarted.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Started construction of %s"), *BuildingName);
}

void AMingBuildingActor::UpdateConstruction(float DeltaTime)
{
    if (!bIsUnderConstruction)
    {
        return;
    }

    // Calculate construction speed based on assigned workers
    float WorkerMultiplier = 1.0f + (AssignedWorkers * 0.2f); // Each worker adds 20% speed
    float ProgressDelta = (ConstructionSpeed * WorkerMultiplier * DeltaTime) / BuildingCost.BuildTime;

    ConstructionProgress += ProgressDelta;
    ConstructionProgress = FMath::Clamp(ConstructionProgress, 0.0f, 1.0f);

    // Broadcast progress update
    OnConstructionProgress.Broadcast(ConstructionProgress);

    // Check if construction is complete
    if (ConstructionProgress >= 1.0f)
    {
        CompleteConstruction();
    }
}

void AMingBuildingActor::CompleteConstruction()
{
    bIsUnderConstruction = false;
    ConstructionProgress = 1.0f;
    BuildingState = EMingBuildingState::Active;
    bIsOperational = true;

    // Set full health
    CurrentHealth = MaxHealth;

    // Update visual state
    UpdateVisualState();

    // Enable resource production if available
    if (ResourceProducer)
    {
        EnableResourceProduction();
    }

    OnConstructionCompleted.Broadcast();
    OnBuildingActivated.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Construction of %s completed"), *BuildingName);
}

void AMingBuildingActor::CancelConstruction()
{
    if (!bIsUnderConstruction)
    {
        return;
    }

    // Refund resources (partial refund based on progress)
    float RefundRatio = 0.5f * ConstructionProgress;
    // In a real implementation, this would add resources back to the resource system

    bIsUnderConstruction = false;
    ConstructionProgress = 0.0f;
    BuildingState = EMingBuildingState::Planned;
    AssignedWorkers = 0;

    // Hide mesh
    BuildingMesh->SetVisibility(false);

    UE_LOG(LogTemp, Log, TEXT("Construction of %s cancelled"), *BuildingName);
}

void AMingBuildingActor::AssignWorker()
{
    if (AssignedWorkers < MaxWorkers && bIsUnderConstruction)
    {
        AssignedWorkers++;
        UE_LOG(LogTemp, Verbose, TEXT("Worker assigned to %s (%d/%d)"), 
               *BuildingName, AssignedWorkers, MaxWorkers);
    }
}

void AMingBuildingActor::RemoveWorker()
{
    if (AssignedWorkers > 0)
    {
        AssignedWorkers--;
        UE_LOG(LogTemp, Verbose, TEXT("Worker removed from %s (%d/%d)"),
               *BuildingName, AssignedWorkers, MaxWorkers);
    }
}

void AMingBuildingActor::TakeDamage(int32 DamageAmount)
{
    if (DamageAmount <= 0 || BuildingState == EMingBuildingState::Destroyed)
    {
        return;
    }

    CurrentHealth -= DamageAmount;
    
    if (CurrentHealth <= 0)
    {
        CurrentHealth = 0;
        DestroyBuilding();
    }
    else
    {
        // Check if building is damaged
        if (CurrentHealth < MaxHealth * 0.5f)
        {
            BuildingState = EMingBuildingState::Damaged;
        }

        OnBuildingDamaged.Broadcast(CurrentHealth, MaxHealth);
        UpdateVisualState();
    }

    UE_LOG(LogTemp, Log, TEXT("Building %s took %d damage (%d/%d HP)"),
           *BuildingName, DamageAmount, CurrentHealth, MaxHealth);
}

void AMingBuildingActor::Repair(int32 RepairAmount)
{
    if (RepairAmount <= 0 || BuildingState == EMingBuildingState::Destroyed)
    {
        return;
    }

    CurrentHealth += RepairAmount;
    CurrentHealth = FMath::Min(CurrentHealth, MaxHealth);

    // Check if building is no longer damaged
    if (CurrentHealth >= MaxHealth * 0.5f && BuildingState == EMingBuildingState::Damaged)
    {
        BuildingState = EMingBuildingState::Active;
    }

    UpdateVisualState();

    UE_LOG(LogTemp, Log, TEXT("Building %s repaired %d HP (%d/%d)"),
           *BuildingName, RepairAmount, CurrentHealth, MaxHealth);
}

float AMingBuildingActor::GetHealthPercentage() const
{
    if (MaxHealth <= 0)
    {
        return 0.0f;
    }

    return FMath::Clamp((float)CurrentHealth / (float)MaxHealth, 0.0f, 1.0f);
}

void AMingBuildingActor::DestroyBuilding()
{
    BuildingState = EMingBuildingState::Destroyed;
    bIsOperational = false;
    
    // Disable resource production
    DisableResourceProduction();

    OnBuildingDestroyed.Broadcast();

    // In a real implementation, you might want to:
    // - Play destruction effects
    // - Leave ruins
    // - Remove the actor after a delay
    // For now, just hide the mesh
    BuildingMesh->SetVisibility(false);

    UE_LOG(LogTemp, Log, TEXT("Building %s destroyed"), *BuildingName);
}

void AMingBuildingActor::EnableResourceProduction()
{
    if (ResourceProducer && bIsOperational)
    {
        ResourceProducer->StartProduction();
        UE_LOG(LogTemp, Log, TEXT("Resource production enabled for %s"), *BuildingName);
    }
}

void AMingBuildingActor::DisableResourceProduction()
{
    if (ResourceProducer)
    {
        ResourceProducer->StopProduction();
        UE_LOG(LogTemp, Log, TEXT("Resource production disabled for %s"), *BuildingName);
    }
}

void AMingBuildingActor::UpdateVisualState()
{
    // This would update the visual appearance based on building state
    // For example:
    // - Change material during construction
    // - Show damage visual when health is low
    // - Play effects when operational

    if (!BuildingMesh)
    {
        return;
    }

    switch (BuildingState)
    {
    case EMingBuildingState::Constructing:
        // Set construction material (semi-transparent, scaffolding effect)
        // This is a placeholder - in real implementation, you'd set specific materials
        break;

    case EMingBuildingState::Active:
        // Set normal operational material
        break;

    case EMingBuildingState::Damaged:
        // Set damaged material (cracks, smoke, fire effects)
        break;

    case EMingBuildingState::Destroyed:
        // Hide or show ruins
        BuildingMesh->SetVisibility(false);
        break;

    default:
        break;
    }
}

void AMingBuildingActor::OnConstructionComplete_Internal()
{
    // Internal handling for construction completion
    // This is called by CompleteConstruction()
}

void AMingBuildingActor::OnDestroyed_Internal()
{
    // Internal handling for building destruction
    // This is called by DestroyBuilding()
}

FMingBuildingCost AMingBuildingActor::GetBuildingCost(EMingBuildingType Type)
{
    FMingBuildingCost Cost;
    Cost.BuildingType = Type;

    switch (Type)
    {
    case EMingBuildingType::CommandCenter:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 500});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 200});
        Cost.BuildTime = 30.0f;
        Cost.MaxHealth = 2000;
        break;

    case EMingBuildingType::Barracks:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 300});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 150});
        Cost.BuildTime = 20.0f;
        Cost.MaxHealth = 1500;
        break;

    case EMingBuildingType::Factory:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 400});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 250});
        Cost.BuildTime = 25.0f;
        Cost.MaxHealth = 1800;
        break;

    case EMingBuildingType::SupplyDepot:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 150});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 100});
        Cost.BuildTime = 15.0f;
        Cost.MaxHealth = 1000;
        break;

    case EMingBuildingType::ResourceGatherer:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 200});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 100});
        Cost.BuildTime = 15.0f;
        Cost.MaxHealth = 800;
        break;

    case EMingBuildingType::DefenseTower:
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Money, 350});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Materials, 200});
        Cost.ResourceCosts.Add(FMingResourceCost{EMingResourceType::Ammo, 50});
        Cost.BuildTime = 25.0f;
        Cost.MaxHealth = 1200;
        break;

    default:
        break;
    }

    return Cost;
}

bool AMingBuildingActor::CanAffordBuilding(EMingBuildingType Type, const TMap<EMingResourceType, int32>& AvailableResources)
{
    FMingBuildingCost Cost = GetBuildingCost(Type);

    for (const FMingResourceCost& ResourceCost : Cost.ResourceCosts)
    {
        const int32* AvailableAmount = AvailableResources.Find(ResourceCost.ResourceType);
        if (!AvailableAmount || *AvailableAmount < ResourceCost.Cost)
        {
            return false;
        }
    }

    return true;
}
