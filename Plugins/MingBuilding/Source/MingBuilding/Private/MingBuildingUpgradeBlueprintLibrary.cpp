#include "MingBuildingUpgradeBlueprintLibrary.h"
#include "MingBuildingActor.h"
#include "MingTechTree.h"

int32 UMingBuildingUpgradeBlueprintLibrary::GetBuildingUpgradeLevel(AMingBuildingActor* Building)
{
    if (Building)
    {
        return Building->GetCurrentUpgradeLevel();
    }
    return 0;
}

bool UMingBuildingUpgradeBlueprintLibrary::IsBuildingUpgrading(AMingBuildingActor* Building)
{
    if (Building)
    {
        return Building->IsUpgrading();
    }
    return false;
}

float UMingBuildingUpgradeBlueprintLibrary::GetBuildingUpgradeProgress(AMingBuildingActor* Building)
{
    if (Building)
    {
        return Building->GetUpgradeProgress();
    }
    return 0.0f;
}

bool UMingBuildingUpgradeBlueprintLibrary::StartBuildingUpgrade(AMingBuildingActor* Building, const FString& UpgradeID)
{
    if (Building)
    {
        Building->StartUpgrade(UpgradeID);
        return true;
    }
    return false;
}

void UMingBuildingUpgradeBlueprintLibrary::CancelBuildingUpgrade(AMingBuildingActor* Building)
{
    if (Building)
    {
        Building->CancelUpgrade();
    }
}

TArray<FMingBuildingUpgrade> UMingBuildingUpgradeBlueprintLibrary::GetAvailableUpgrades(EMingBuildingType BuildingType, UMingTechTree* TechTree)
{
    TArray<FMingBuildingUpgrade> AvailableUpgrades;
    
    if (TechTree)
    {
        AvailableUpgrades = TechTree->GetAvailableUpgradesForBuilding(BuildingType);
    }
    
    return AvailableUpgrades;
}

bool UMingBuildingUpgradeBlueprintLibrary::CanUpgradeBuilding(
    AMingBuildingActor* Building, 
    const FString& UpgradeID, 
    UMingTechTree* TechTree,
    const TMap<EMingResourceType, int32>& AvailableResources)
{
    if (!Building || !TechTree)
    {
        return false;
    }
    
    return TechTree->CanUpgradeBuilding(Building->BuildingType, UpgradeID, AvailableResources);
}

TArray<FMingResourceCost> UMingBuildingUpgradeBlueprintLibrary::GetUpgradeCost(const FString& UpgradeID, UMingTechTree* TechTree)
{
    TArray<FMingResourceCost> Costs;
    
    if (TechTree)
    {
        FMingBuildingUpgrade Upgrade = TechTree->GetBuildingUpgrade(UpgradeID);
        Costs = Upgrade.UpgradeCost;
    }
    
    return Costs;
}

float UMingBuildingUpgradeBlueprintLibrary::GetUpgradeTime(const FString& UpgradeID, UMingTechTree* TechTree)
{
    if (TechTree)
    {
        FMingBuildingUpgrade Upgrade = TechTree->GetBuildingUpgrade(UpgradeID);
        return Upgrade.UpgradeTime;
    }
    return 0.0f;
}

void UMingBuildingUpgradeBlueprintLibrary::ApplyUpgradeToBuilding(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData)
{
    if (Building)
    {
        Building->ApplyUpgrade(UpgradeData);
    }
}

int32 UMingBuildingUpgradeBlueprintLibrary::GetUpgradedHealth(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData)
{
    if (Building)
    {
        float HealthMultiplier = FMath::Pow(UpgradeData.HealthMultiplier, UpgradeData.CurrentLevel);
        return FMath::RoundToInt(Building->MaxHealth * HealthMultiplier);
    }
    return 0;
}

float UMingBuildingUpgradeBlueprintLibrary::GetUpgradedProduction(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData)
{
    if (Building && Building->ResourceProducer)
    {
        float ProductionMultiplier = FMath::Pow(UpgradeData.ProductionMultiplier, UpgradeData.CurrentLevel);
        // Get base production from ResourceProducer and apply multiplier
        return ProductionMultiplier;
    }
    return 1.0f;
}

float UMingBuildingUpgradeBlueprintLibrary::GetUpgradedDefense(AMingBuildingActor* Building, const FMingBuildingUpgrade& UpgradeData)
{
    if (Building)
    {
        float DefenseMultiplier = FMath::Pow(UpgradeData.DefenseMultiplier, UpgradeData.CurrentLevel);
        return DefenseMultiplier;
    }
    return 1.0f;
}
