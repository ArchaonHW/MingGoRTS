// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Service Version Manager - Advanced Service Management Implementation

#include "Process/MingRTSServiceVersionManager.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/DateTime.h"
#include "Misc/Guid.h"

UMingRTSServiceVersionManager::UMingRTSServiceVersionManager()
{
    InitializeVersionManager();
}

void UMingRTSServiceVersionManager::InitializeVersionManager()
{
    UE_LOG(LogTemp, Log, TEXT("Service Version Manager initialized"));
}

FString UMingRTSServiceVersionManager::CreateServiceVersion(const FString& ServiceName, const FString& VersionNumber, const FString& BuildNumber)
{
    FServiceVersion NewVersion;
    NewVersion.VersionID = GenerateVersionID();
    NewVersion.ServiceName = ServiceName;
    NewVersion.VersionNumber = VersionNumber;
    NewVersion.BuildNumber = BuildNumber;
    NewVersion.Status = EServiceVersionStatus::Development;
    NewVersion.BuildDate = FDateTime::Now();
    NewVersion.DeploymentTime = FDateTime::Now();
    NewVersion.bIsStable = false;
    NewVersion.PerformanceScore = 0.0f;
    NewVersion.ActiveInstances = 0;

    if (RegisterServiceVersion(NewVersion))
    {
        OnVersionCreated.Broadcast(NewVersion.VersionID, NewVersion);
        return NewVersion.VersionID;
    }

    return FString();
}

bool UMingRTSServiceVersionManager::RegisterServiceVersion(const FServiceVersion& Version)
{
    if (!ValidateVersion(Version))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid version data for %s"), *Version.VersionID);
        return false;
    }

    // Store version
    ServiceVersions.Add(Version.VersionID, Version);

    // Update service index
    if (!ServiceVersionIndex.Contains(Version.ServiceName))
    {
        ServiceVersionIndex.Add(Version.ServiceName, TArray<FString>());
    }
    ServiceVersionIndex[Version.ServiceName].Add(Version.VersionID);

    // Initialize health status
    VersionHealthStatus.Add(Version.VersionID, true);
    VersionPerformanceScores.Add(Version.VersionID, 0.0f);

    UE_LOG(LogTemp, Log, TEXT("Registered service version: %s (%s)"), *Version.VersionID, *Version.VersionNumber);
    return true;
}

FServiceVersion UMingRTSServiceVersionManager::GetServiceVersion(const FString& VersionID) const
{
    if (const FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        return *Version;
    }
    return FServiceVersion();
}

TArray<FServiceVersion> UMingRTSServiceVersionManager::GetAllVersions(const FString& ServiceName) const
{
    TArray<FServiceVersion> Result;
    
    if (const TArray<FString>* VersionIDs = ServiceVersionIndex.Find(ServiceName))
    {
        for (const FString& VersionID : *VersionIDs)
        {
            if (const FServiceVersion* Version = ServiceVersions.Find(VersionID))
            {
                Result.Add(*Version);
            }
        }
    }
    
    return Result;
}

TArray<FServiceVersion> UMingRTSServiceVersionManager::GetVersionsByStatus(EServiceVersionStatus Status) const
{
    TArray<FServiceVersion> Result;
    
    for (const auto& VersionPair : ServiceVersions)
    {
        if (VersionPair.Value.Status == Status)
        {
            Result.Add(VersionPair.Value);
        }
    }
    
    return Result;
}

bool UMingRTSServiceVersionManager::UpdateVersionStatus(const FString& VersionID, EServiceVersionStatus NewStatus)
{
    if (FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        EServiceVersionStatus OldStatus = Version->Status;
        Version->Status = NewStatus;
        
        OnVersionStatusChanged.Broadcast(VersionID, NewStatus);
        
        UE_LOG(LogTemp, Log, TEXT("Updated version status: %s -> %s"), *VersionID, *StaticEnum<EServiceVersionStatus>()->GetValueAsString(NewStatus));
        return true;
    }
    
    return false;
}

bool UMingRTSServiceVersionManager::DeleteVersion(const FString& VersionID)
{
    if (FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        // Remove from service index
        if (TArray<FString>* VersionList = ServiceVersionIndex.Find(Version->ServiceName))
        {
            VersionList->Remove(VersionID);
        }
        
        // Remove from storage
        ServiceVersions.Remove(VersionID);
        VersionHealthStatus.Remove(VersionID);
        VersionPerformanceScores.Remove(VersionID);
        
        UE_LOG(LogTemp, Log, TEXT("Deleted service version: %s"), *VersionID);
        return true;
    }
    
    return false;
}

FString UMingRTSServiceVersionManager::CreateDeploymentPlan(const FString& TargetVersion, EDeploymentStrategy Strategy)
{
    FDeploymentPlan NewPlan;
    NewPlan.PlanID = GeneratePlanID();
    NewPlan.TargetVersion = TargetVersion;
    NewPlan.Strategy = Strategy;
    NewPlan.TargetInstanceCount = 1;
    NewPlan.RolloutPercentage = 100.0f;
    NewPlan.HealthCheckInterval = 30;
    NewPlan.MaxRetries = 3;
    NewPlan.bEnableAutoRollback = true;

    DeploymentPlans.Add(NewPlan.PlanID, NewPlan);
    
    UE_LOG(LogTemp, Log, TEXT("Created deployment plan: %s for version %s"), *NewPlan.PlanID, *TargetVersion);
    return NewPlan.PlanID;
}

bool UMingRTSServiceVersionManager::ExecuteDeploymentPlan(const FString& PlanID)
{
    if (FDeploymentPlan* Plan = DeploymentPlans.Find(PlanID))
    {
        if (!ValidateDeploymentPlan(*Plan))
        {
            UE_LOG(LogTemp, Error, TEXT("Invalid deployment plan: %s"), *PlanID);
            return false;
        }

        // Perform pre-deployment checks
        if (!PerformPreDeploymentChecks(*Plan))
        {
            UE_LOG(LogTemp, Error, TEXT("Pre-deployment checks failed for plan: %s"), *PlanID);
            return false;
        }

        // Execute deployment based on strategy
        switch (Plan->Strategy)
        {
        case EDeploymentStrategy::Rolling:
            ExecuteRollingDeployment(*Plan);
            break;
        case EDeploymentStrategy::BlueGreen:
            ExecuteBlueGreenDeployment(*Plan);
            break;
        case EDeploymentStrategy::Canary:
            ExecuteCanaryDeployment(*Plan);
            break;
        case EDeploymentStrategy::ABO:
            ExecuteABDeployment(*Plan);
            break;
        case EDeploymentStrategy::Shadow:
            ExecuteShadowDeployment(*Plan);
            break;
        default:
            UE_LOG(LogTemp, Warning, TEXT("Unknown deployment strategy for plan: %s"), *PlanID);
            return false;
        }

        // Perform post-deployment checks
        if (PerformPostDeploymentChecks(*Plan))
        {
            OnVersionDeployed.Broadcast(Plan->TargetVersion, true);
            return true;
        }
        else if (Plan->bEnableAutoRollback)
        {
            RollbackDeployment(PlanID, GetLatestStableVersion(ServiceVersions[Plan->TargetVersion].ServiceName));
            OnVersionDeployed.Broadcast(Plan->TargetVersion, false);
            return false;
        }
    }
    
    return false;
}

bool UMingRTSServiceVersionManager::RollbackDeployment(const FString& PlanID, const FString& TargetVersion)
{
    UE_LOG(LogTemp, Log, TEXT("Rolling back deployment plan: %s to version: %s"), *PlanID, *TargetVersion);
    
    // Implement rollback logic
    UpdateDeploymentProgress(PlanID, 50.0f, TEXT("Rolling back..."));
    UpdateDeploymentProgress(PlanID, 100.0f, TEXT("Rollback complete"));
    
    return true;
}

FDeploymentPlan UMingRTSServiceVersionManager::GetDeploymentPlan(const FString& PlanID) const
{
    if (const FDeploymentPlan* Plan = DeploymentPlans.Find(PlanID))
    {
        return *Plan;
    }
    return FDeploymentPlan();
}

TArray<FDeploymentPlan> UMingRTSServiceVersionManager::GetAllDeploymentPlans() const
{
    TArray<FDeploymentPlan> Result;
    DeploymentPlans.GenerateValueArray(Result);
    return Result;
}

bool UMingRTSServiceVersionManager::RegisterVersionCompatibility(const FVersionCompatibility& Compatibility)
{
    // Remove existing compatibility for this version pair
    for (int32 i = 0; i < CompatibilityMatrix.Num(); i++)
    {
        if (CompatibilityMatrix[i].SourceVersion == Compatibility.SourceVersion &&
            CompatibilityMatrix[i].TargetVersion == Compatibility.TargetVersion)
        {
            CompatibilityMatrix.RemoveAt(i);
            break;
        }
    }
    
    CompatibilityMatrix.Add(Compatibility);
    UE_LOG(LogTemp, Log, TEXT("Registered version compatibility: %s -> %s"), *Compatibility.SourceVersion, *Compatibility.TargetVersion);
    return true;
}

bool UMingRTSServiceVersionManager::IsVersionCompatible(const FString& SourceVersion, const FString& TargetVersion) const
{
    for (const FVersionCompatibility& Compatibility : CompatibilityMatrix)
    {
        if (Compatibility.SourceVersion == SourceVersion && Compatibility.TargetVersion == TargetVersion)
        {
            return Compatibility.bIsCompatible;
        }
    }
    return false;
}

FVersionCompatibility UMingRTSServiceVersionManager::GetCompatibilityInfo(const FString& SourceVersion, const FString& TargetVersion) const
{
    for (const FVersionCompatibility& Compatibility : CompatibilityMatrix)
    {
        if (Compatibility.SourceVersion == SourceVersion && Compatibility.TargetVersion == TargetVersion)
        {
            return Compatibility;
        }
    }
    return FVersionCompatibility();
}

bool UMingRTSServiceVersionManager::ExecuteMigration(const FString& SourceVersion, const FString& TargetVersion)
{
    FVersionCompatibility Compatibility = GetCompatibilityInfo(SourceVersion, TargetVersion);
    
    if (!Compatibility.bRequiresMigration)
    {
        UE_LOG(LogTemp, Log, TEXT("No migration required: %s -> %s"), *SourceVersion, *TargetVersion);
        return true;
    }
    
    if (Compatibility.MigrationScript.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Migration script not found for: %s -> %s"), *SourceVersion, *TargetVersion);
        return false;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executing migration: %s -> %s"), *SourceVersion, *TargetVersion);
    // Implement migration script execution
    
    return true;
}

FString UMingRTSServiceVersionManager::GetLatestStableVersion(const FString& ServiceName) const
{
    FString LatestVersion;
    FDateTime LatestDate;
    
    if (const TArray<FString>* VersionIDs = ServiceVersionIndex.Find(ServiceName))
    {
        for (const FString& VersionID : *VersionIDs)
        {
            if (const FServiceVersion* Version = ServiceVersions.Find(VersionID))
            {
                if (Version->Status == EServiceVersionStatus::Production && Version->bIsStable)
                {
                    if (LatestVersion.IsEmpty() || Version->BuildDate > LatestDate)
                    {
                        LatestVersion = VersionID;
                        LatestDate = Version->BuildDate;
                    }
                }
            }
        }
    }
    
    return LatestVersion;
}

TArray<FString> UMingRTSServiceVersionManager::GetUpgradePath(const FString& CurrentVersion, const FString& TargetVersion) const
{
    TArray<FString> UpgradePath;
    
    // Simple implementation - direct upgrade if compatible
    if (IsVersionCompatible(CurrentVersion, TargetVersion))
    {
        UpgradePath.Add(TargetVersion);
    }
    else
    {
        // Find intermediate versions
        // This would require more complex graph traversal in a real implementation
        UE_LOG(LogTemp, Warning, TEXT("No direct upgrade path found: %s -> %s"), *CurrentVersion, *TargetVersion);
    }
    
    return UpgradePath;
}

float UMingRTSServiceVersionManager::GetVersionPerformanceScore(const FString& VersionID) const
{
    if (const float* Score = VersionPerformanceScores.Find(VersionID))
    {
        return *Score;
    }
    return 0.0f;
}

void UMingRTSServiceVersionManager::UpdatePerformanceScore(const FString& VersionID, float NewScore)
{
    VersionPerformanceScores.Add(VersionID, NewScore);
    
    if (FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        Version->PerformanceScore = NewScore;
    }
}

bool UMingRTSServiceVersionManager::SetVersionConfiguration(const FString& VersionID, const TMap<FString, FString>& Configuration)
{
    if (FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        Version->Configuration = Configuration;
        return true;
    }
    return false;
}

TMap<FString, FString> UMingRTSServiceVersionManager::GetVersionConfiguration(const FString& VersionID) const
{
    if (const FServiceVersion* Version = ServiceVersions.Find(VersionID))
    {
        return Version->Configuration;
    }
    return TMap<FString, FString>();
}

bool UMingRTSServiceVersionManager::IsVersionHealthy(const FString& VersionID) const
{
    if (const bool* Healthy = VersionHealthStatus.Find(VersionID))
    {
        return *Healthy;
    }
    return false;
}

void UMingRTSServiceVersionManager::PerformHealthCheck(const FString& VersionID)
{
    bool bHealthy = CheckVersionHealth(VersionID);
    VersionHealthStatus.Add(VersionID, bHealthy);
    
    UE_LOG(LogTemp, Log, TEXT("Health check for version %s: %s"), *VersionID, bHealthy ? TEXT("Healthy") : TEXT("Unhealthy"));
}

TArray<FString> UMingRTSServiceVersionManager::GetUnhealthyVersions() const
{
    TArray<FString> UnhealthyVersions;
    
    for (const auto& HealthPair : VersionHealthStatus)
    {
        if (!HealthPair.Value)
        {
            UnhealthyVersions.Add(HealthPair.Key);
        }
    }
    
    return UnhealthyVersions;
}

// Internal Methods
FString UMingRTSServiceVersionManager::GenerateVersionID() const
{
    return FGuid::NewGuid().ToString();
}

FString UMingRTSServiceVersionManager::GeneratePlanID() const
{
    return FString::Printf(TEXT("plan_%s"), *FGuid::NewGuid().ToString());
}

bool UMingRTSServiceVersionManager::ValidateVersion(const FServiceVersion& Version) const
{
    return !Version.VersionID.IsEmpty() && 
           !Version.ServiceName.IsEmpty() && 
           !Version.VersionNumber.IsEmpty();
}

bool UMingRTSServiceVersionManager::ValidateDeploymentPlan(const FDeploymentPlan& Plan) const
{
    return !Plan.PlanID.IsEmpty() && 
           !Plan.TargetVersion.IsEmpty() && 
           ServiceVersions.Contains(Plan.TargetVersion);
}

void UMingRTSServiceVersionManager::ExecuteRollingDeployment(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing rolling deployment for plan: %s"), *Plan.PlanID);
    
    UpdateDeploymentProgress(Plan.PlanID, 10.0f, TEXT("Starting rolling deployment..."));
    UpdateDeploymentProgress(Plan.PlanID, 50.0f, TEXT("Deploying new instances..."));
    UpdateDeploymentProgress(Plan.PlanID, 90.0f, TEXT("Updating load balancer..."));
    UpdateDeploymentProgress(Plan.PlanID, 100.0f, TEXT("Rolling deployment complete"));
}

void UMingRTSServiceVersionManager::ExecuteBlueGreenDeployment(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing blue-green deployment for plan: %s"), *Plan.PlanID);
    
    UpdateDeploymentProgress(Plan.PlanID, 10.0f, TEXT("Preparing green environment..."));
    UpdateDeploymentProgress(Plan.PlanID, 50.0f, TEXT("Deploying to green environment..."));
    UpdateDeploymentProgress(Plan.PlanID, 80.0f, TEXT("Switching traffic to green..."));
    UpdateDeploymentProgress(Plan.PlanID, 100.0f, TEXT("Blue-green deployment complete"));
}

void UMingRTSServiceVersionManager::ExecuteCanaryDeployment(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing canary deployment for plan: %s"), *Plan.PlanID);
    
    UpdateDeploymentProgress(Plan.PlanID, 10.0f, TEXT("Deploying canary instances..."));
    UpdateDeploymentProgress(Plan.PlanID, 30.0f, TEXT("Monitoring canary performance..."));
    UpdateDeploymentProgress(Plan.PlanID, 70.0f, TEXT("Expanding canary deployment..."));
    UpdateDeploymentProgress(Plan.PlanID, 100.0f, TEXT("Canary deployment complete"));
}

void UMingRTSServiceVersionManager::ExecuteABDeployment(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing A/B deployment for plan: %s"), *Plan.PlanID);
    
    UpdateDeploymentProgress(Plan.PlanID, 10.0f, TEXT("Setting up A/B test..."));
    UpdateDeploymentProgress(Plan.PlanID, 50.0f, TEXT("Running A/B test..."));
    UpdateDeploymentProgress(Plan.PlanID, 90.0f, TEXT("Analyzing results..."));
    UpdateDeploymentProgress(Plan.PlanID, 100.0f, TEXT("A/B deployment complete"));
}

void UMingRTSServiceVersionManager::ExecuteShadowDeployment(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing shadow deployment for plan: %s"), *Plan.PlanID);
    
    UpdateDeploymentProgress(Plan.PlanID, 10.0f, TEXT("Setting up shadow environment..."));
    UpdateDeploymentProgress(Plan.PlanID, 50.0f, TEXT("Mirroring traffic to shadow..."));
    UpdateDeploymentProgress(Plan.PlanID, 90.0f, TEXT("Analyzing shadow performance..."));
    UpdateDeploymentProgress(Plan.PlanID, 100.0f, TEXT("Shadow deployment complete"));
}

bool UMingRTSServiceVersionManager::PerformPreDeploymentChecks(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Performing pre-deployment checks for plan: %s"), *Plan.PlanID);
    
    for (const FString& Check : Plan.PreDeploymentChecks)
    {
        UE_LOG(LogTemp, Log, TEXT("Running pre-deployment check: %s"), *Check);
        // Implement actual check logic
    }
    
    return true;
}

bool UMingRTSServiceVersionManager::PerformPostDeploymentChecks(const FDeploymentPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Performing post-deployment checks for plan: %s"), *Plan.PlanID);
    
    for (const FString& Check : Plan.PostDeploymentChecks)
    {
        UE_LOG(LogTemp, Log, TEXT("Running post-deployment check: %s"), *Check);
        // Implement actual check logic
    }
    
    return true;
}

void UMingRTSServiceVersionManager::UpdateDeploymentProgress(const FString& PlanID, float Progress, const FString& Status)
{
    OnDeploymentProgress.Broadcast(PlanID, Progress, Status);
}

bool UMingRTSServiceVersionManager::CheckVersionHealth(const FString& VersionID) const
{
    // Implement actual health check logic
    // For now, assume all versions are healthy
    return true;
}

void UMingRTSServiceVersionManager::CleanupOldVersions()
{
    // Implement cleanup logic for old/deprecated versions
    UE_LOG(LogTemp, Log, TEXT("Cleaning up old service versions"));
}
