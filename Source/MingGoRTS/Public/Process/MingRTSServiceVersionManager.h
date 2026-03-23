// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Service Version Manager - Advanced Service Management
// Provides comprehensive service version management and deployment

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "Containers/Array.h"
#include "MingRTSServiceVersionManager.generated.h"

UENUM(BlueprintType)
enum class EServiceVersionStatus : uint8 {
    Development     UMETA(DisplayName = "Development"),
    Testing         UMETA(DisplayName = "Testing"),
    Staging         UMETA(DisplayName = "Staging"),
    Production      UMETA(DisplayName = "Production"),
    Deprecated      UMETA(DisplayName = "Deprecated"),
    Retired         UMETA(DisplayName = "Retired")
};

UENUM(BlueprintType)
enum class EDeploymentStrategy : uint8 {
    Rolling         UMETA(DisplayName = "Rolling Update"),
    BlueGreen        UMETA(DisplayName = "Blue-Green"),
    Canary          UMETA(DisplayName = "Canary"),
    ABO             UMETA(DisplayName = "A/B Testing"),
    Shadow          UMETA(DisplayName = "Shadow"),
    Custom          UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FServiceVersion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString VersionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString VersionNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString ServiceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    EServiceVersionStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString BuildNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FDateTime BuildDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString GitCommit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    TArray<FString> Dependencies;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    TMap<FString, FString> Configuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    bool bIsStable;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    float PerformanceScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    int32 ActiveInstances;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Service Version")
    FDateTime DeploymentTime;

    FServiceVersion()
        : Status(EServiceVersionStatus::Development)
        , bIsStable(false)
        , PerformanceScore(0.0f)
        , ActiveInstances(0)
    {}
};

USTRUCT(BlueprintType)
struct FDeploymentPlan
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    FString PlanID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    FString TargetVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    EDeploymentStrategy Strategy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    int32 TargetInstanceCount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    float RolloutPercentage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    int32 HealthCheckInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    int32 MaxRetries;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    bool bEnableAutoRollback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    TArray<FString> PreDeploymentChecks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    TArray<FString> PostDeploymentChecks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deployment Plan")
    TMap<FString, FString> DeploymentParameters;

    FDeploymentPlan()
        : Strategy(EDeploymentStrategy::Rolling)
        , TargetInstanceCount(1)
        , RolloutPercentage(100.0f)
        , HealthCheckInterval(30)
        , MaxRetries(3)
        , bEnableAutoRollback(true)
    {}
};

USTRUCT(BlueprintType)
struct FVersionCompatibility
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    FString SourceVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    FString TargetVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    bool bIsCompatible;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    bool bRequiresMigration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    FString MigrationScript;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    TArray<FString> BreakingChanges;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Compatibility")
    TArray<FString> NewFeatures;

    FVersionCompatibility()
        : bIsCompatible(true)
        , bRequiresMigration(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVersionCreated, const FString&, VersionID, const FServiceVersion&, Version);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVersionDeployed, const FString&, VersionID, bool, bSuccess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVersionStatusChanged, const FString&, VersionID, EServiceVersionStatus, NewStatus);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDeploymentProgress, const FString&, PlanID, float, Progress, const FString&, Status);

/**
 * Service Version Manager
 * Manages service versions, deployments, and compatibility
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingRTSServiceVersionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSServiceVersionManager();

    // Version Management
    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    FString CreateServiceVersion(const FString& ServiceName, const FString& VersionNumber, const FString& BuildNumber);

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool RegisterServiceVersion(const FServiceVersion& Version);

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    FServiceVersion GetServiceVersion(const FString& VersionID) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TArray<FServiceVersion> GetAllVersions(const FString& ServiceName) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TArray<FServiceVersion> GetVersionsByStatus(EServiceVersionStatus Status) const;

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool UpdateVersionStatus(const FString& VersionID, EServiceVersionStatus NewStatus);

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool DeleteVersion(const FString& VersionID);

    // Deployment Management
    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    FString CreateDeploymentPlan(const FString& TargetVersion, EDeploymentStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool ExecuteDeploymentPlan(const FString& PlanID);

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool RollbackDeployment(const FString& PlanID, const FString& TargetVersion);

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    FDeploymentPlan GetDeploymentPlan(const FString& PlanID) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TArray<FDeploymentPlan> GetAllDeploymentPlans() const;

    // Compatibility Management
    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool RegisterVersionCompatibility(const FVersionCompatibility& Compatibility);

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    bool IsVersionCompatible(const FString& SourceVersion, const FString& TargetVersion) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    FVersionCompatibility GetCompatibilityInfo(const FString& SourceVersion, const FString& TargetVersion) const;

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool ExecuteMigration(const FString& SourceVersion, const FString& TargetVersion);

    // Version Analysis
    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    FString GetLatestStableVersion(const FString& ServiceName) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TArray<FString> GetUpgradePath(const FString& CurrentVersion, const FString& TargetVersion) const;

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    float GetVersionPerformanceScore(const FString& VersionID) const;

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    void UpdatePerformanceScore(const FString& VersionID, float NewScore);

    // Configuration Management
    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    bool SetVersionConfiguration(const FString& VersionID, const TMap<FString, FString>& Configuration);

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TMap<FString, FString> GetVersionConfiguration(const FString& VersionID) const;

    // Health and Monitoring
    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    bool IsVersionHealthy(const FString& VersionID) const;

    UFUNCTION(BlueprintCallable, Category = "Service Version Manager")
    void PerformHealthCheck(const FString& VersionID);

    UFUNCTION(BlueprintPure, Category = "Service Version Manager")
    TArray<FString> GetUnhealthyVersions() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Service Version Manager Events")
    FOnVersionCreated OnVersionCreated;

    UPROPERTY(BlueprintAssignable, Category = "Service Version Manager Events")
    FOnVersionDeployed OnVersionDeployed;

    UPROPERTY(BlueprintAssignable, Category = "Service Version Manager Events")
    FOnVersionStatusChanged OnVersionStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "Service Version Manager Events")
    FOnDeploymentProgress OnDeploymentProgress;

protected:
    // Version Storage
    UPROPERTY()
    TMap<FString, FServiceVersion> ServiceVersions;

    UPROPERTY()
    TMap<FString, TArray<FString>> ServiceVersionIndex;

    // Deployment Storage
    UPROPERTY()
    TMap<FString, FDeploymentPlan> DeploymentPlans;

    // Compatibility Storage
    UPROPERTY()
    TArray<FVersionCompatibility> CompatibilityMatrix;

    // Runtime State
    UPROPERTY()
    TMap<FString, bool> VersionHealthStatus;

    UPROPERTY()
    TMap<FString, float> VersionPerformanceScores;

    // Internal Methods
    void InitializeVersionManager();
    FString GenerateVersionID() const;
    FString GeneratePlanID() const;
    bool ValidateVersion(const FServiceVersion& Version) const;
    bool ValidateDeploymentPlan(const FDeploymentPlan& Plan) const;
    void ExecuteRollingDeployment(const FDeploymentPlan& Plan);
    void ExecuteBlueGreenDeployment(const FDeploymentPlan& Plan);
    void ExecuteCanaryDeployment(const FDeploymentPlan& Plan);
    void ExecuteABDeployment(const FDeploymentPlan& Plan);
    void ExecuteShadowDeployment(const FDeploymentPlan& Plan);
    bool PerformPreDeploymentChecks(const FDeploymentPlan& Plan);
    bool PerformPostDeploymentChecks(const FDeploymentPlan& Plan);
    void UpdateDeploymentProgress(const FString& PlanID, float Progress, const FString& Status);
    bool CheckVersionHealth(const FString& VersionID) const;
    void CleanupOldVersions();
};
