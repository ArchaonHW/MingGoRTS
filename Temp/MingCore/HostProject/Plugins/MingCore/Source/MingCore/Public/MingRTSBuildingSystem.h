#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSBuildingSystem.generated.h"

class AActor;
class UMingRTSResourceManager;
class UMingRTSUnitManager;

UENUM(BlueprintType)
enum class ERTSBuildingType : uint8
{
    Residential,     // ��X
    Commercial,      // X�~
    Industrial,      // �u�~
    Military,        // �xX
    Agricultural,   // �A�~
    Educational,    // X�|
    Medical,         // X
    Storage,         // X�x
    Power,           // X
    Defense,         // X�m
    Resource,        // ��X
    Research,        // X�s
    Entertainment,   // �TX
    Government,      // X
    Religious,       // �vX
    Custom          // �۩w�q�ؿv
};

UENUM(BlueprintType)
enum class ERTSBuildingState : uint8
{
    Planning,        // �W����
    UnderConstruction, // �س]��
    Operational,     // �B�礤
    Damaged,         // ���l
    Repairing,       // �״_��
    Upgrading,       // �ɯŤ�
    Demolishing,     // ���
    Abandoned,       // �o��
    Destroyed        // ����
};

UENUM(BlueprintType)
enum class ERTSBuildingSize : uint8
{
    Small,           // �pX
    Medium,          // ��X
    Large,           // �jX
    ExtraLarge      // �W�j��
};

USTRUCT(BlueprintType)
struct FRTSBuildingData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    FString BuildingID;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    FString BuildingName;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    ERTSBuildingType BuildingType;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    ERTSBuildingState BuildingState;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    ERTSBuildingSize BuildingSize;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    FVector Location;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    FRotator Rotation;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float Health;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float MaxHealth;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float ConstructionProgress;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float ProductionRate;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float MaintenanceCost;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    int32 WorkerCapacity;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    int32 CurrentWorkers;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    TArray<ERTSResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    TArray<float> ResourceConsumption;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    TArray<ERTSResourceType> ProducedResources;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    TArray<float> ResourceProduction;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float InfluenceRadius;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    float RenderDistance;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    int32 CurrentLOD;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    bool bIsVisible;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    bool bRequiresUpdate;

    UPROPERTY(BlueprintReadOnly, Category = "Building Data")
    FDateTime LastUpdateTime;

    FRTSBuildingData()
    {
        BuildingID = TEXT(""};
        BuildingName = TEXT(""};
        BuildingType = ERTSBuildingType::Residential;
        BuildingSize = ERTSBuildingSize::Small;
        Location = FVector::ZeroVector;
        Rotation = FRotator::ZeroRotator;
        State = ERTSBuildingState::Planned;
        Health = 100.0f;
        MaxHealth = 100.0f;
        ConstructionProgress = 0.0f;
        ProductionRate = 1.0f;
        MaintenanceCost = 1.0f;
        WorkerCapacity = 5;
        CurrentWorkers = 0;
        InfluenceRadius = 300.0f;
        RenderDistance = 1000.0f;
        CurrentLOD = 0;
        bIsVisible = true;
        bRequiresUpdate = true;
        LastUpdateTime = FDateTime::Now(};
        bIsPowered = false;
        bIsConnected = false;
        OwnerID = TEXT(""};
    }
};

// X��X�]�m
USTRUCT(BlueprintType)
struct FBuildingPerformanceSettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    bool bEnableLOD = true;

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    bool bEnableCulling = true;

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    bool bEnableBatching = true;

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    float UpdateFrequency = 60.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    int32 MaxVisibleBuildings = 100;

    UPROPERTY(BlueprintReadWrite, Category = "Performance Settings")
    float CullingDistance = 2000.0f;

    FBuildingPerformanceSettings()
    {
        bEnableLOD = true;
        bEnableCulling = true;
        bEnableBatching = true;
        UpdateFrequency = 60.0f;
        MaxVisibleBuildings = 100;
        CullingDistance = 2000.0f;
    }
};

USTRUCT(BlueprintType)
struct FRTSBuildingTemplate
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    FString TemplateID;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    FString TemplateName;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    ERTSBuildingType BuildingType;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    ERTSBuildingSize BuildingSize;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    float ConstructionCost;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    float ConstructionTime;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    TArray<ERTSResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    TArray<float> ResourceCosts;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    FString BlueprintPath;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Building Template")
    bool bIsAvailable;

    FRTSBuildingTemplate()
    {
        TemplateID = TEXT(""};
        TemplateName = TEXT(""};
        BuildingType = ERTSBuildingType::Residential;
        BuildingSize = ERTSBuildingSize::Small;
        ConstructionCost = 100.0f;
        ConstructionTime = 60.0f;
        BlueprintPath = TEXT(""};
        Description = TEXT(""};
        bIsAvailable = true;
    }
};

USTRUCT(BlueprintType)
struct FRTSBuildingUpgrade
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    FString UpgradeID;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    FString UpgradeName;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    float UpgradeCost;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    float UpgradeTime;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TArray<ERTSResourceType> RequiredResources;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TArray<float> ResourceCosts;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    TArray<FString> Effects;

    UPROPERTY(BlueprintReadOnly, Category = "Building Upgrade")
    bool bIsAvailable;

    FRTSBuildingUpgrade()
    {
        UpgradeID = TEXT(""};
        UpgradeName = TEXT(""};
        Description = TEXT(""};
        UpgradeCost = 50.0f;
        UpgradeTime = 30.0f;
        bIsAvailable = true;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingConstructed, const FString&, BuildingID, ERTSBuildingType, BuildingType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingDestroyed, const FString&, BuildingID, ERTSBuildingType, BuildingType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingStateChanged, const FString&, BuildingID, ERTSBuildingState, NewState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingUpgraded, const FString&, BuildingID, const FString&, UpgradeID};

/**
 * RTS��X�t��
 * ��X�ؿv��X��
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSBuildingSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSBuildingSystem(};

    // X    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void InitializeBuildingSystem(UMingRTSResourceManager* InResourceManager, UMingRTSUnitManager* InUnitManager};

    // ��X��X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    FString CreateBuilding(const FRTSBuildingTemplate& Template, const FVector& Location, const FRotator& Rotation) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool DestroyBuilding(const FString& BuildingID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    FRTSBuildingData GetBuilding(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingData> GetAllBuildings() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingData> GetBuildingsByType(ERTSBuildingType BuildingType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingData> GetBuildingsByState(ERTSBuildingState BuildingState) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingData> GetBuildingsInArea(const FVector& Center, float Radius) const;

    // ��X    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void SetBuildingState(const FString& BuildingID, ERTSBuildingState NewState};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    ERTSBuildingState GetBuildingState(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool IsBuildingOperational(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void StartConstruction(const FString& BuildingID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void StopConstruction(const FString& BuildingID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void CompleteConstruction(const FString& BuildingID) {};

    // ��X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool UpgradeBuilding(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool CanUpgradeBuilding(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingUpgrade> GetAvailableUpgrades(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void AddBuildingUpgrade(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade) {};

    // ��X���@
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void RepairBuilding(const FString& BuildingID, float RepairAmount};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void DamageBuilding(const FString& BuildingID, float DamageAmount};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    float GetBuildingHealth(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    float GetBuildingHealthPercentage(const FString& BuildingID) const;

    // ��X��
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void UpdateBuildingProduction(float DeltaTime};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    float GetBuildingProductionRate(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void SetBuildingProductionRate(const FString& BuildingID, float Rate};

    // ��X�t��X�৹X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void EnhanceBuildingSystem(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OptimizeBuildingLayout(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void ImproveConstructionEfficiency(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void ImplementSmartBuildingManagement(};

    // �u�H��X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool AssignWorkers(const FString& BuildingID, int32 WorkerCount};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void RemoveWorkers(const FString& BuildingID, int32 WorkerCount};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    int32 GetAvailableWorkerCapacity(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool HasEnoughWorkers(const FString& BuildingID) const;

    // ��X�ҪO
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void AddBuildingTemplate(const FRTSBuildingTemplate& Template) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void RemoveBuildingTemplate(const FString& TemplateID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    FRTSBuildingTemplate GetBuildingTemplate(const FString& TemplateID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingTemplate> GetAllBuildingTemplates() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FRTSBuildingTemplate> GetBuildingTemplatesByType(ERTSBuildingType BuildingType) const;

    // ��X�m
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool CanPlaceBuilding(const FRTSBuildingTemplate& Template, const FVector& Location) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool IsLocationOccupied(const FVector& Location, float Radius) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    FVector FindBestLocation(const FRTSBuildingTemplate& Template, const FVector& PreferredLocation) const;

    // ��X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void ConnectBuilding(const FString& BuildingID, const FString& TargetBuildingID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void DisconnectBuilding(const FString& BuildingID, const FString& TargetBuildingID) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    bool IsBuildingConnected(const FString& BuildingID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    TArray<FString> GetConnectedBuildings(const FString& BuildingID) const;

    // ��X��X
    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    int32 GetTotalBuildingCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    int32 GetBuildingCountByType(ERTSBuildingType BuildingType) const;

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    int32 GetOperationalBuildingCount() const;

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    float GetTotalBuildingValue() const;

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    float GetTotalMaintenanceCost() const;

    // ��X�ƥ�
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OnBuildingAttacked(const FString& BuildingID, float Damage};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OnBuildingCaptured(const FString& BuildingID, const FString& NewOwner) {};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OnResourceDepleted(const FString& BuildingID) {};

    // �ƥ�
    UPROPERTY(BlueprintAssignable, Category = "RTS Building Events")
    FOnBuildingConstructed OnBuildingConstructed;

    UPROPERTY(BlueprintAssignable, Category = "RTS Building Events")
    FOnBuildingDestroyed OnBuildingDestroyed;

    UPROPERTY(BlueprintAssignable, Category = "RTS Building Events")
    FOnBuildingStateChanged OnBuildingStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Building Events")
    FOnBuildingUpgraded OnBuildingUpgraded;

protected:
    // �t�Τޥ�
    UPROPERTY()
    TObjectPtr<UMingRTSResourceManager> ResourceManager;

    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;

    // ��X
    UPROPERTY()
    TMap<FString, FRTSBuildingData> Buildings;

    UPROPERTY()
    TMap<FString, FRTSBuildingTemplate> BuildingTemplates;

    // �`X�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FRTSBuildingUpgrade>> BuildingUpgrades;

    // �`X�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FString>> BuildingConnections;

    // �]�m
    UPROPERTY(BlueprintReadWrite, Category = "Building Settings")
    float ConstructionSpeedMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Building Settings")
    float MaintenanceMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Building Settings")
    float RepairSpeedMultiplier = 1.0f;

    UPROPERTY(BlueprintReadWrite, Category = "Building Settings")
    bool bAutoRepair = true;

    UPROPERTY(BlueprintReadWrite, Category = "Building Settings")
    bool bAutoUpgrade = false;

    // ��X��
    UPROPERTY(BlueprintReadWrite, Category = "Enhancement Settings")
    float EfficiencyBonus = 1.2f;

    UPROPERTY(BlueprintReadWrite, Category = "Enhancement Settings")
    float LayoutOptimizationFactor = 1.1f;

    UPROPERTY(BlueprintReadWrite, Category = "Enhancement Settings")
    bool bEnableSmartManagement = true;

    UPROPERTY(BlueprintReadWrite, Category = "Enhancement Settings")
    bool bEnableAutoLayout = true;

    // X    UPROPERTY()
    bool bIsInitialized;

    // X��X��
    void InitializeDefaultTemplates(};
    void ProcessBuildingConstruction(FRTSBuildingData& Building, float DeltaTime};
    void ProcessBuildingProduction(FRTSBuildingData& Building, float DeltaTime};
    void ProcessBuildingMaintenance(FRTSBuildingData& Building, float DeltaTime};
    void UpdateBuildingConnections(const FString& BuildingID) {};
    FString GenerateBuildingID(ERTSBuildingType BuildingType) const;
    FString GetBuildingName(ERTSBuildingType BuildingType) const;
    float GetBuildingSizeMultiplier(ERTSBuildingSize BuildingSize) const;
    bool ValidateBuildingPlacement(const FRTSBuildingTemplate& Template, const FVector& Location) const;
    void ApplyBuildingEffects(const FString& BuildingID, const FRTSBuildingUpgrade& Upgrade) {};
    void CheckBuildingIntegrity(FRTSBuildingData& Building};

    // ��X��X��
    void OptimizeBuildingEfficiency(};
    void CalculateOptimalLayout(};
    void ImproveConstructionSpeed(};
    void ImplementSmartMaintenance(};
    float CalculateBuildingEfficiency(const FString& BuildingID) const;
    void AutoOrganizeBuildings(};
    void OptimizeResourceFlow(};

// X��X
    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OptimizeLargeScaleBuildingManagement(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OptimizeMemoryUsage(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OptimizeRenderingPerformance(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void OptimizeNetworkSynchronization(};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void EnablePerformanceMode(bool bEnable};

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    float GetBuildingSystemPerformance() const;

    UFUNCTION(BlueprintPure, Category = "RTS Building System")
    int32 GetActiveBuildingCount() const;

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void UpdateBuildingLOD(const FString& BuildingID, int32 LODLevel};

    UFUNCTION(BlueprintCallable, Category = "RTS Building System")
    void BatchUpdateBuildings(const TArray<FString>& BuildingIDs};

private:
    // ���UX��
    void NotifyBuildingConstructed(const FString& BuildingID, ERTSBuildingType BuildingType};
    void NotifyBuildingDestroyed(const FString& BuildingID, ERTSBuildingType BuildingType};
    void NotifyBuildingStateChanged(const FString& BuildingID, ERTSBuildingState NewState};
    void NotifyBuildingUpgraded(const FString& BuildingID, const FString& UpgradeID) {};

    // X��X��X��
    void OptimizeBuildingDataStructures(};
    void ImplementBuildingPooling(};
    void OptimizeBuildingUpdates(};
    void ReduceMemoryFootprint(};
    void OptimizeRenderingCalls(};
    void ImplementNetworkDeltaCompression(};
};

