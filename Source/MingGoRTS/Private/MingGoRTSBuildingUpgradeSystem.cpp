#include "MingGoRTSBuildingUpgradeSystem.h"
#include "TimerManager.h"

UMingGoRTSBuildingUpgradeSystem::UMingGoRTSBuildingUpgradeSystem()
    : bInitialized(false)
{
}

void UMingGoRTSBuildingUpgradeSystem::InitializeUpgradeSystem()
{
    if (bInitialized)
    {
        return;
    }

    UpgradePaths.Empty();
    ActiveUpgrades.Empty();
    BuildingLevels.Empty();
    UnlockedPaths.Empty();

    bInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("Building Upgrade System initialized"));
}

void UMingGoRTSBuildingUpgradeSystem::RegisterUpgradePath(const FBuildingUpgradePath& UpgradePath)
{
    if (!bInitialized)
    {
        return;
    }

    UpgradePaths.Add(UpgradePath.PathID, UpgradePath);
    UE_LOG(LogTemp, Log, TEXT("Registered upgrade path: %s"), *UpgradePath.PathID);
}

bool UMingGoRTSBuildingUpgradeSystem::StartBuildingUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel)
{
    if (!bInitialized)
    {
        return false;
    }

    if (!UpgradePaths.Contains(PathID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Upgrade path not found: %s"), *PathID);
        return false;
    }

    if (ActiveUpgrades.Contains(BuildingID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Building already has active upgrade: %s"), *BuildingID);
        return false;
    }

    if (!CanUpgrade(BuildingID, PathID, TargetLevel))
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot upgrade building %s to level %d"), *BuildingID, TargetLevel);
        return false;
    }

    TMap<EMingUpgradeResourceType, float> Cost = GetUpgradeCost(BuildingID, PathID, TargetLevel);
    if (!ConsumeResources(Cost))
    {
        UE_LOG(LogTemp, Warning, TEXT("Insufficient resources for upgrade"));
        return false;
    }

    FBuildingUpgradeProgress Progress;
    Progress.BuildingID = BuildingID;
    Progress.PathID = PathID;
    Progress.CurrentLevel = GetBuildingCurrentLevel(BuildingID);
    Progress.TargetLevel = TargetLevel;
    Progress.Progress = 0.0f;
    Progress.TimeRemaining = GetUpgradeTime(BuildingID, PathID, TargetLevel);

    ActiveUpgrades.Add(BuildingID, Progress);

    OnUpgradeStarted.Broadcast(BuildingID, PathID);

    UE_LOG(LogTemp, Log, TEXT("Started upgrade for building %s, path %s, target level %d"), 
        *BuildingID, *PathID, TargetLevel);

    return true;
}

bool UMingGoRTSBuildingUpgradeSystem::PauseUpgrade(const FString& BuildingID)
{
    if (!ActiveUpgrades.Contains(BuildingID))
    {
        return false;
    }

    OnUpgradePaused.Broadcast(BuildingID, TEXT("User paused"));
    UE_LOG(LogTemp, Log, TEXT("Paused upgrade for building: %s"), *BuildingID);
    return true;
}

bool UMingGoRTSBuildingUpgradeSystem::ResumeUpgrade(const FString& BuildingID)
{
    if (!ActiveUpgrades.Contains(BuildingID))
    {
        return false;
    }

    OnUpgradeResumed.Broadcast(BuildingID, TEXT("User resumed"));
    UE_LOG(LogTemp, Log, TEXT("Resumed upgrade for building: %s"), *BuildingID);
    return true;
}

bool UMingGoRTSBuildingUpgradeSystem::CancelUpgrade(const FString& BuildingID)
{
    if (!ActiveUpgrades.Contains(BuildingID))
    {
        return false;
    }

    ActiveUpgrades.Remove(BuildingID);
    OnUpgradeFailed.Broadcast(BuildingID, TEXT("Cancelled by user"));

    UE_LOG(LogTemp, Log, TEXT("Cancelled upgrade for building: %s"), *BuildingID);
    return true;
}

bool UMingGoRTSBuildingUpgradeSystem::InstantCompleteUpgrade(const FString& BuildingID)
{
    if (!ActiveUpgrades.Contains(BuildingID))
    {
        return false;
    }

    CompleteUpgrade(BuildingID);
    return true;
}

FBuildingUpgradeProgress UMingGoRTSBuildingUpgradeSystem::GetUpgradeProgress(const FString& BuildingID) const
{
    if (ActiveUpgrades.Contains(BuildingID))
    {
        return ActiveUpgrades[BuildingID];
    }

    return FBuildingUpgradeProgress();
}

TArray<FBuildingUpgradePath> UMingGoRTSBuildingUpgradeSystem::GetAllUpgradePaths() const
{
    TArray<FBuildingUpgradePath> Result;
    UpgradePaths.GenerateValueArray(Result);
    return Result;
}

TArray<FBuildingUpgradePath> UMingGoRTSBuildingUpgradeSystem::GetUpgradePathsByType(EMingBuildingType BuildingType) const
{
    TArray<FBuildingUpgradePath> Result;
    for (const auto& Pair : UpgradePaths)
    {
        if (Pair.Value.BuildingType == BuildingType)
        {
            Result.Add(Pair.Value);
        }
    }
    return Result;
}

int32 UMingGoRTSBuildingUpgradeSystem::GetBuildingCurrentLevel(const FString& BuildingID) const
{
    if (BuildingLevels.Contains(BuildingID))
    {
        return BuildingLevels[BuildingID];
    }
    return 1;
}

FBuildingUpgradeStatistics UMingGoRTSBuildingUpgradeSystem::GetUpgradeStatistics() const
{
    FBuildingUpgradeStatistics Stats;
    Stats.TotalUpgrades = BuildingLevels.Num();
    Stats.SuccessRate = 1.0f;
    Stats.AverageUpgradeTime = 0.0f;
    return Stats;
}

bool UMingGoRTSBuildingUpgradeSystem::CanUpgrade(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const
{
    if (!UpgradePaths.Contains(PathID))
    {
        return false;
    }

    const FBuildingUpgradePath& Path = UpgradePaths[PathID];
    if (TargetLevel > Path.MaxLevel)
    {
        return false;
    }

    int32 CurrentLevel = GetBuildingCurrentLevel(BuildingID);
    if (TargetLevel <= CurrentLevel)
    {
        return false;
    }

    return true;
}

TMap<EMingUpgradeResourceType, float> UMingGoRTSBuildingUpgradeSystem::GetUpgradeCost(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const
{
    TMap<EMingUpgradeResourceType, float> Cost;
    Cost.Add(EMingUpgradeResourceType::Gold, 100.0f * TargetLevel);
    Cost.Add(EMingUpgradeResourceType::Wood, 50.0f * TargetLevel);
    return Cost;
}

float UMingGoRTSBuildingUpgradeSystem::GetUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const
{
    return 10.0f * TargetLevel;
}

TArray<FBuildingUpgradeEffect> UMingGoRTSBuildingUpgradeSystem::GetUpgradeEffects(const FString& PathID, int32 Level) const
{
    TArray<FBuildingUpgradeEffect> Effects;

    FBuildingUpgradeEffect Effect;
    Effect.EffectName = TEXT("Health Boost");
    Effect.EffectType = EBuildingUpgradeEffectType::HealthBoost;
    Effect.Value = 10.0f * Level;
    Effects.Add(Effect);

    return Effects;
}

void UMingGoRTSBuildingUpgradeSystem::UnlockUpgradePath(const FString& PathID, EMingBuildingType BuildingType)
{
    UnlockedPaths.Add(PathID);
    OnUpgradePathUnlocked.Broadcast(PathID, BuildingType);
    UE_LOG(LogTemp, Log, TEXT("Unlocked upgrade path: %s"), *PathID);
}

bool UMingGoRTSBuildingUpgradeSystem::IsUpgradePathUnlocked(const FString& PathID) const
{
    return UnlockedPaths.Contains(PathID);
}

void UMingGoRTSBuildingUpgradeSystem::UpdateUpgradeProgress(float DeltaTime)
{
    TArray<FString> CompletedBuildings;

    for (auto& Pair : ActiveUpgrades)
    {
        FBuildingUpgradeProgress& Progress = Pair.Value;
        Progress.TimeRemaining -= DeltaTime;

        float TotalTime = GetUpgradeTime(Progress.BuildingID, Progress.PathID, Progress.TargetLevel);
        Progress.Progress = 1.0f - (Progress.TimeRemaining / TotalTime);

        OnUpgradeProgress.Broadcast(Progress.BuildingID, Progress.Progress, Progress.TimeRemaining);

        if (Progress.TimeRemaining <= 0.0f)
        {
            CompletedBuildings.Add(Progress.BuildingID);
        }
    }

    for (const FString& BuildingID : CompletedBuildings)
    {
        CompleteUpgrade(BuildingID);
    }
}

void UMingGoRTSBuildingUpgradeSystem::CompleteUpgrade(const FString& BuildingID)
{
    if (!ActiveUpgrades.Contains(BuildingID))
    {
        return;
    }

    FBuildingUpgradeProgress& Progress = ActiveUpgrades[BuildingID];
    BuildingLevels.Add(BuildingID, Progress.TargetLevel);

    TArray<FBuildingUpgradeEffect> Effects = GetUpgradeEffects(Progress.PathID, Progress.TargetLevel);
    ApplyUpgradeEffects(BuildingID, Effects);

    ActiveUpgrades.Remove(BuildingID);
    OnUpgradeCompleted.Broadcast(BuildingID, Effects);

    UE_LOG(LogTemp, Log, TEXT("Completed upgrade for building: %s"), *BuildingID);
}

void UMingGoRTSBuildingUpgradeSystem::ApplyUpgradeEffects(const FString& BuildingID, const TArray<FBuildingUpgradeEffect>& Effects)
{
    for (const FBuildingUpgradeEffect& Effect : Effects)
    {
        UE_LOG(LogTemp, Log, TEXT("Applied effect %s to building %s"), *Effect.EffectName, *BuildingID);
    }
}

float UMingGoRTSBuildingUpgradeSystem::CalculateActualUpgradeTime(const FString& BuildingID, const FString& PathID, int32 TargetLevel) const
{
    return GetUpgradeTime(BuildingID, PathID, TargetLevel);
}

bool UMingGoRTSBuildingUpgradeSystem::HasEnoughResources(const TMap<EMingUpgradeResourceType, float>& Cost) const
{
    return true;
}

bool UMingGoRTSBuildingUpgradeSystem::ConsumeResources(const TMap<EMingUpgradeResourceType, float>& Cost)
{
    return true;
}
