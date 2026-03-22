#include "MingResourceProducer.h"
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

UMingResourceProducer::UMingResourceProducer()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.5f; // Update every 0.5 seconds

    ProducedResource = EMingResourceType::Money;
    ProductionRate = 1.0f;
    ProductionInterval = 1.0f;
    bAutoStartProduction = true;
    
    bIsProducing = false;
    TimeSinceLastProduction = 0.0f;
    TotalProduced = 0;
    
    bConsumeResourcesToOperate = false;
}

void UMingResourceProducer::BeginPlay()
{
    Super::BeginPlay();

    // Find resource system
    if (UWorld* World = GetWorld())
    {
        // Try to find resource system from game mode
        if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(World))
        {
            // This assumes the game mode has a resource system component
            // In a real implementation, you'd have a proper way to access the resource system
        }
    }

    // Auto-start production if configured
    if (bAutoStartProduction)
    {
        StartProduction();
    }

    UE_LOG(LogTemp, Log, TEXT("Resource producer initialized for %s"), *GetResourceName(ProducedResource));
}

void UMingResourceProducer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsProducing)
    {
        UpdateProduction(DeltaTime);
    }
}

void UMingResourceProducer::StartProduction()
{
    if (bIsProducing)
    {
        return;
    }

    bIsProducing = true;
    TimeSinceLastProduction = 0.0f;

    OnProductionStarted.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Started producing %s at %.1f per %.1f seconds"),
           *GetResourceName(ProducedResource), ProductionRate, ProductionInterval);
}

void UMingResourceProducer::StopProduction()
{
    if (!bIsProducing)
    {
        return;
    }

    bIsProducing = false;
    TimeSinceLastProduction = 0.0f;

    OnProductionStopped.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Stopped producing %s"), *GetResourceName(ProducedResource));
}

void UMingResourceProducer::PauseProduction()
{
    if (bIsProducing)
    {
        bIsProducing = false;
        UE_LOG(LogTemp, Log, TEXT("Paused producing %s"), *GetResourceName(ProducedResource));
    }
}

void UMingResourceProducer::ResumeProduction()
{
    if (!bIsProducing && TotalProduced > 0) // Was previously producing
    {
        bIsProducing = true;
        UE_LOG(LogTemp, Log, TEXT("Resumed producing %s"), *GetResourceName(ProducedResource));
    }
}

void UMingResourceProducer::SetProductionRate(float NewRate)
{
    ProductionRate = FMath::Max(0.0f, NewRate);
    UE_LOG(LogTemp, Log, TEXT("Production rate for %s changed to %.1f"),
           *GetResourceName(ProducedResource), ProductionRate);
}

bool UMingResourceProducer::IsOperational() const
{
    if (!bIsProducing)
    {
        return false;
    }

    if (bConsumeResourcesToOperate && OperatingCosts.Num() > 0)
    {
        return CheckOperatingCosts();
    }

    return true;
}

float UMingResourceProducer::GetProductionProgress() const
{
    if (ProductionInterval <= 0.0f)
    {
        return 1.0f;
    }

    return FMath::Clamp(TimeSinceLastProduction / ProductionInterval, 0.0f, 1.0f);
}

void UMingResourceProducer::UpdateProduction(float DeltaTime)
{
    // Check operating costs first
    if (bConsumeResourcesToOperate && OperatingCosts.Num() > 0)
    {
        if (!CheckOperatingCosts())
        {
            OnInsufficientResources.Broadcast();
            
            // Optionally pause production when resources are insufficient
            UE_LOG(LogTemp, Warning, TEXT("Insufficient resources to operate %s producer"),
                   *GetResourceName(ProducedResource));
            return;
        }

        // Consume operating costs
        ConsumeOperatingCosts();
    }

    // Accumulate production time
    TimeSinceLastProduction += DeltaTime;

    // Check if we should produce
    if (TimeSinceLastProduction >= ProductionInterval)
    {
        ProduceResource();
    }
}

void UMingResourceProducer::ProduceResource()
{
    // Calculate production amount
    int32 ProductionCycles = FMath::FloorToInt(TimeSinceLastProduction / ProductionInterval);
    int32 AmountToProduce = FMath::FloorToInt(ProductionCycles * ProductionRate * ProductionInterval);

    if (AmountToProduce > 0)
    {
        // In a real implementation, this would add resources to the resource system
        // For now, just track the total
        TotalProduced += AmountToProduce;

        // Broadcast production event
        OnProductionTick.Broadcast(ProducedResource, AmountToProduce);

        // Reset timer
        TimeSinceLastProduction -= ProductionCycles * ProductionInterval;

        UE_LOG(LogTemp, Verbose, TEXT("Produced %d %s (Total: %d)"),
               AmountToProduce, *GetResourceName(ProducedResource), TotalProduced);
    }
}

bool UMingResourceProducer::CheckOperatingCosts()
{
    if (!ResourceSystem.IsValid())
    {
        return OperatingCosts.Num() == 0; // No costs means always operational
    }

    return ResourceSystem->HasEnoughResources(OperatingCosts);
}

void UMingResourceProducer::ConsumeOperatingCosts()
{
    if (ResourceSystem.IsValid() && OperatingCosts.Num() > 0)
    {
        ResourceSystem->ConsumeResources(OperatingCosts);
    }
}

void UMingResourceProducer::OnResourceSystemAvailable()
{
    // This would be called when the resource system becomes available
    // In a real implementation, you'd properly link to the resource system
}

FString UMingResourceProducer::GetResourceName(EMingResourceType Type)
{
    return UMingResourceSystem::GetResourceName(Type);
}
