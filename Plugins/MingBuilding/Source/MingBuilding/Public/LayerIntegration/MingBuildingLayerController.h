#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LayerIntegration/MingFourLayerTypes.h"
#include "MingBuildingLayerController.generated.h"

UENUM(BlueprintType)
enum class EMingLayerBuildingType : uint8
{
    Headquarters UMETA(DisplayName = "Headquarters"),
    Barracks UMETA(DisplayName = "Barracks"),
    Factory UMETA(DisplayName = "Factory"),
    Armory UMETA(DisplayName = "Armory"),
    Warehouse UMETA(DisplayName = "Warehouse"),
    ResearchLab UMETA(DisplayName = "Research Lab"),
    MedicalStation UMETA(DisplayName = "Medical Station"),
    TrainingGround UMETA(DisplayName = "Training Ground"),
    DefenseTower UMETA(DisplayName = "Defense Tower"),
    Wall UMETA(DisplayName = "Wall"),
    Generator UMETA(DisplayName = "Generator"),
    Farm UMETA(DisplayName = "Farm"),
    Mine UMETA(DisplayName = "Mine"),
    Workshop UMETA(DisplayName = "Workshop")
};

UENUM(BlueprintType)
enum class EMingBuildStatus : uint8
{
    Planned UMETA(DisplayName = "Planned"),
    UnderConstruction UMETA(DisplayName = "Under Construction"),
    Active UMETA(DisplayName = "Active"),
    Damaged UMETA(DisplayName = "Damaged"),
    Destroyed UMETA(DisplayName = "Destroyed"),
    Upgrading UMETA(DisplayName = "Upgrading")
};

USTRUCT(BlueprintType)
struct FMingBuildingInstance
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString BuildingName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EMingBuildingType BuildingType = EMingBuildingType::Barracks;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    EMingBuildStatus Status = EMingBuildStatus::Planned;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 Level = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 MaxLevel = 5;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float Health = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float MaxHealth = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float ConstructionProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    float BuildTime = 60.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TMap<FString, int32> ResourceCosts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 WorkerCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    int32 MaxWorkers = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TArray<FString> ProducingUnits;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    TArray<FString> ResearchingTechs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    bool bIsSelected = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building")
    FString FactionID;

    FMingBuildingInstance()
        : BuildingType(EMingBuildingType::Barracks)
        , Status(EMingBuildStatus::Planned)
        , Level(1)
        , MaxLevel(5)
        , Health(100.0f)
        , MaxHealth(100.0f)
        , ConstructionProgress(0.0f)
        , BuildTime(60.0f)
        , WorkerCount(0)
        , MaxWorkers(10)
        , bIsSelected(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingBaseLayout
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    FString BaseID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    FString BaseName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    FString FactionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    FVector CenterLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    float Radius = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    TArray<FString> BuildingIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    int32 TotalWorkers = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    int32 MaxWorkers = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base")
    bool bIsUnderAttack = false;

    FMingBaseLayout()
        : Radius(1000.0f)
        , TotalWorkers(0)
        , MaxWorkers(100)
        , bIsUnderAttack(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingProductionQueue
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    FString BuildingID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    TArray<FString> QueueItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    float CurrentProgress = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    float ProductionTime = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Production")
    bool bIsPaused = false;

    FMingProductionQueue()
        : CurrentProgress(0.0f)
        , ProductionTime(30.0f)
        , bIsPaused(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingResourceFlow
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Flow")
    TMap<FString, int32> Income;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Flow")
    TMap<FString, int32> Expenses;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Flow")
    TMap<FString, int32> NetFlow;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Flow")
    float Efficiency = 1.0f;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingConstructed, const FMingBuildingInstance&, Building);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingUpgraded, const FMingBuildingInstance&, Building);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBuildingDestroyed, const FString&, BuildingID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProductionCompleted, const FString&, UnitType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBaseAttacked, const FString&, BaseID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingSelected, const FString&, BuildingID, bool, bIsSelected);

/**
 * Building Layer Controller
 * Manages base construction, building management, and resource production
 */
UCLASS(ClassGroup = (Building), Blueprintable)
class MINGBUILDING_API UMingBuildingLayerController : public UObject
{
    GENERATED_BODY()

public:
    UMingBuildingLayerController();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void InitializeBuildingLayer();

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void ShutdownBuildingLayer();

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void ActivateBuildingLayer();

    UFUNCTION(BlueprintCallable, Category = "Building Layer")
    void DeactivateBuildingLayer();

    // Base Management
    UFUNCTION(BlueprintCallable, Category = "Base Management")
    void LoadBase(const FString& BaseID);

    UFUNCTION(BlueprintCallable, Category = "Base Management")
    void CreateNewBase(const FString& BaseName, FVector Location, const FString& FactionID);

    UFUNCTION(BlueprintCallable, Category = "Base Management")
    void ClearBase();

    UFUNCTION(BlueprintCallable, Category = "Base Management")
    bool GetCurrentBase(FMingBaseLayout& OutBase) const;

    UFUNCTION(BlueprintCallable, Category = "Base Management")
    void SetBaseUnderAttack(bool bUnderAttack);

    UFUNCTION(BlueprintCallable, Category = "Base Management")
    void DefendBase();

    // Building Management
    UFUNCTION(BlueprintCallable, Category = "Building Management")
    FString PlaceBuildingBlueprint(EMingBuildingType Type, FVector Location, FRotator Rotation);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void StartConstruction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void CancelConstruction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void CompleteConstruction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void UpgradeBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void DemolishBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void RepairBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    bool GetBuilding(const FString& BuildingID, FMingBuildingInstance& OutBuilding) const;

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    TArray<FMingBuildingInstance> GetAllBuildings() const;

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    TArray<FMingBuildingInstance> GetBuildingsByType(EMingBuildingType Type) const;

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    TArray<FMingBuildingInstance> GetActiveBuildings() const;

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    TArray<FMingBuildingInstance> GetUnderConstructionBuildings() const;

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void SelectBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void DeselectBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Building Management")
    void ClearBuildingSelection();

    UFUNCTION(BlueprintPure, Category = "Building Management")
    TArray<FString> GetSelectedBuildings() const { return SelectedBuildingIDs; }

    // Worker Management
    UFUNCTION(BlueprintCallable, Category = "Workers")
    void AssignWorkers(const FString& BuildingID, int32 WorkerCount);

    UFUNCTION(BlueprintCallable, Category = "Workers")
    void RemoveWorkers(const FString& BuildingID, int32 WorkerCount);

    UFUNCTION(BlueprintCallable, Category = "Workers")
    void SetBuildingPriority(const FString& BuildingID, int32 Priority);

    UFUNCTION(BlueprintCallable, Category = "Workers")
    int32 GetAvailableWorkers() const;

    UFUNCTION(BlueprintCallable, Category = "Workers")
    int32 GetTotalWorkers() const;

    UFUNCTION(BlueprintCallable, Category = "Workers")
    void HireWorkers(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "Workers")
    void ReassignWorkers(const FString& FromBuildingID, const FString& ToBuildingID, int32 Count);

    // Production Management
    UFUNCTION(BlueprintCallable, Category = "Production")
    void QueueUnitProduction(const FString& BuildingID, const FString& UnitType);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void CancelUnitProduction(const FString& BuildingID, int32 QueueIndex);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void PauseProduction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void ResumeProduction(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Production")
    void RushProduction(const FString& BuildingID);

    UFUNCTION(BlueprintPure, Category = "Production")
    FMingProductionQueue GetProductionQueue(const FString& BuildingID) const;

    // Resource Management
    UFUNCTION(BlueprintCallable, Category = "Resources")
    FMingResourceFlow CalculateResourceFlow() const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    int32 GetResourceProduction(const FString& ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    int32 GetResourceConsumption(const FString& ResourceType) const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    float GetPowerProduction() const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    float GetPowerConsumption() const;

    UFUNCTION(BlueprintPure, Category = "Resources")
    bool HasPowerSurplus() const;

    UFUNCTION(BlueprintCallable, Category = "Resources")
    void OptimizeResourceDistribution();

    // Grid & Placement
    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector SnapToGrid(FVector Location, float GridSize = 100.0f);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    bool IsValidBuildLocation(FVector Location, float Radius) const;

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void SetBuildRadius(float Radius);

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void ShowBuildableAreas();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    void HideBuildableAreas();

    UFUNCTION(BlueprintCallable, Category = "Grid")
    FVector FindNearestValidLocation(FVector DesiredLocation, EMingBuildingType Type) const;

    // Camera & View
    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusOnBuilding(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void FocusOnBaseCenter();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetOverviewCamera();

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetFollowCamera(const FString& BuildingID);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ZoomCamera(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void PanCamera(FVector Direction);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void RotateCamera(float Angle);

    // Layer Integration
    UFUNCTION(BlueprintCallable, Category = "Layer Integration")
    void RequestStrategicSupport(const FString& SupportType);

    UFUNCTION(BlueprintCallable, Category = "Layer Integration")
    void SendResourcesToStrategic(TMap<FString, int32> Resources);

    UFUNCTION(BlueprintCallable, Category = "Layer Integration")
    void ReceiveUnitFromTactical(const FString& UnitID);

    UFUNCTION(BlueprintCallable, Category = "Layer Integration")
    void ReturnToStrategic();

    // Utility Functions
    UFUNCTION(BlueprintPure, Category = "Utility")
    int32 GetBuildingLimit() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    int32 GetCurrentBuildingCount() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    float GetBaseDefenseRating() const;

    UFUNCTION(BlueprintPure, Category = "Utility")
    float GetBaseEfficiency() const;

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void ToggleBuildingVisibility(EMingBuildingType Type, bool bVisible);

    UFUNCTION(BlueprintCallable, Category = "Utility")
    void ToggleGridDisplay(bool bShow);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingConstructed OnBuildingConstructed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingUpgraded OnBuildingUpgraded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingDestroyed OnBuildingDestroyed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnProductionCompleted OnProductionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBaseAttacked OnBaseAttacked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnBuildingSelected OnBuildingSelected;

protected:
    UPROPERTY()
    TMap<FString, FMingBuildingInstance> Buildings;

    UPROPERTY()
    FMingBaseLayout CurrentBase;

    UPROPERTY()
    TMap<FString, FMingProductionQueue> ProductionQueues;

    UPROPERTY()
    TArray<FString> SelectedBuildingIDs;

    UPROPERTY()
    bool bIsActive = false;

    UPROPERTY()
    float BuildRadius = 2000.0f;

    UPROPERTY()
    bool bShowGrid = true;

    UPROPERTY()
    int32 NextBuildingID = 1;

    // Internal functions
    void InitializeDefaultBuildings();
    void ProcessConstructionTick(float DeltaTime);
    void ProcessProductionTick(float DeltaTime);
    void UpdateResourceProduction();
    void CheckBuildingHealth();
    
    void OnBuildingCompleted(const FString& BuildingID);
    void OnUnitProductionFinished(const FString& BuildingID, const FString& UnitType);
    void OnBuildingDamaged(const FString& BuildingID, float DamageAmount);
    void OnBaseDefenseTriggered();
    
    bool CanAffordBuilding(const FMingBuildingInstance& Building) const;
    void DeductBuildingCost(const FMingBuildingInstance& Building);
    void RefundBuildingCost(const FMingBuildingInstance& Building);
    
    FString GenerateBuildingID();
    int32 CalculateMaxWorkers(EMingBuildingType Type) const;
    float CalculateBuildTime(EMingBuildingType Type, int32 Level) const;
};
