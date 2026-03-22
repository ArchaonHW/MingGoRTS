// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System - ?��?宇�X�能延伸

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSAIWorldSavior.h"
#include "MingGoRTSAIUniverseSavior.generated.h"

// 宇�?威�?級別 - 超�?世�?級別?��XUENUM(BlueprintType)
enum class EUniverseThreatLevel : uint8
{
    None            UMETA(DisplayName = "?��X),
    Stellar         UMETA(DisplayName = "?��?�?),          // ?��X��?系統?�影X    Galactic        UMETA(DisplayName = "?�河�?"),          // ?�個�?河系?�影X    Intergalactic   UMETA(DisplayName = "?�系?��?"),       // 多個�?系�?影響
    Cosmic          UMETA(DisplayName = "宇�?�?),          // 宇�?結�X��X    Multiverse      UMETA(DisplayName = "多�?宇�?�?),      // 多�?宇�?級別
    Omniversal      UMETA(DisplayName = "?�能宇�?�?),      // 終極威�?
    MAX             UMETA(Hidden)
};

// 宇�?威�?類�?
UENUM(BlueprintType)
enum class EUniverseThreatType : uint8
{
    Supernova           UMETA(DisplayName = "超新?��X),
    BlackHole           UMETA(DisplayName = "黑�X�張"),
    GammaRayBurst       UMETA(DisplayName = "伽馬射�X),
    DarkMatterAnomaly   UMETA(DisplayName = "?�物質異�?),
    VoidExpansion       UMETA(DisplayName = "?�空?�張"),
    RealityDistortion   UMETA(DisplayName = "?�實?�曲"),
    TimeParadox         UMETA(DisplayName = "?��X��?"),
    DimensionalCollapse UMETA(DisplayName = "維度崩�?"),
    AlienInvasion       UMETA(DisplayName = "外�X�侵"),
    AIApocalypse        UMETA(DisplayName = "AI?�日"),
    CosmicPlague        UMETA(DisplayName = "宇�X�疫"),
    EnergyDepletion     UMETA(DisplayName = "?��X�竭"),
    EntropyReversal     UMETA(DisplayName = "?�逆�?"),
    QuantumInstability  UMETA(DisplayName = "?��?不穩�?),
    Singularity         UMETA(DisplayName = "奇�?事件"),
    MAX                 UMETA(Hidden)
};

// 宇�X��?策略
UENUM(BlueprintType)
enum class EUniverseSaviorStrategy : uint8
{
    StellarEngineering      UMETA(DisplayName = "?��?工�?"),
    WormholeManipulation    UMETA(DisplayName = "?��X�控"),
    DimensionalShielding    UMETA(DisplayName = "維度護盾"),
    TimeManipulation        UMETA(DisplayName = "?��X�控"),
    RealityAnchoring        UMETA(DisplayName = "?�實?��?"),
    QuantumStabilization    UMETA(DisplayName = "?��?穩�?"),
    CosmicDiplomacy         UMETA(DisplayName = "宇�?外交"),
    MultiversalCooperation  UMETA(DisplayName = "多�?宇�X��?"),
    EnergyRedistribution    UMETA(DisplayName = "?��X��X),
    EntropyControl          UMETA(DisplayName = "?�控X),
    GenesisProtocol         UMETA(DisplayName = "?��X�議"),
    Emergency               UMETA(DisplayName = "緊急�?�?),
    MAX                     UMETA(Hidden)
};

// 宇�X�XUSTRUCT(BlueprintType)
struct FUniverseRegion
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString RegionName;

    UPROPERTY(BlueprintReadOnly)
    FVector3d GalacticCoordinates;

    UPROPERTY(BlueprintReadOnly)
    int32 StarCount;

    UPROPERTY(BlueprintReadOnly)
    float ThreatLevel;

    UPROPERTY(BlueprintReadOnly)
    bool bIsStable;

    FUniverseRegion()
        : StarCount(0)
        , ThreatLevel(0.0f)
        , bIsStable(true)
    {}
};

// 宇�?威�X��?
USTRUCT(BlueprintType)
struct FUniverseThreatData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ThreatName;

    UPROPERTY(BlueprintReadOnly)
    EUniverseThreatType ThreatType;

    UPROPERTY(BlueprintReadOnly)
    EUniverseThreatLevel ThreatLevel;

    UPROPERTY(BlueprintReadOnly)
    FString ThreatDescription;

    UPROPERTY(BlueprintReadOnly)
    FVector3d ThreatLocation;

    UPROPERTY(BlueprintReadOnly)
    float ThreatRadius; // ?�年

    UPROPERTY(BlueprintReadOnly)
    float TimeToImpact; // �?
    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    EUniverseSaviorStrategy RecommendedStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FUniverseRegion> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float EnergyRequired; // 宇�X��X��?

    UPROPERTY(BlueprintReadOnly)
    int32 CivilizationsAtRisk;

    FUniverseThreatData()
        : ThreatType(EUniverseThreatType::Supernova)
        , ThreatLevel(EUniverseThreatLevel::Stellar)
        , ThreatRadius(0.0f)
        , TimeToImpact(0.0f)
        , Confidence(0.0f)
        , EnergyRequired(0.0f)
        , CivilizationsAtRisk(0)
    {}
};

// 宇�?行�?計�?
USTRUCT(BlueprintType)
struct FUniverseActionPlan
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlanName;

    UPROPERTY(BlueprintReadOnly)
    EUniverseSaviorStrategy Strategy;

    UPROPERTY(BlueprintReadOnly)
    float EstimatedSuccessRate;

    UPROPERTY(BlueprintReadOnly)
    float ExecutionTime; // �?
    UPROPERTY(BlueprintReadOnly)
    float EnergyCost;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ActionSteps;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredTechnologies;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> PotentialRisks;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MitigationStrategies;

    UPROPERTY(BlueprintReadOnly)
    int32 CivilizationsRequired;

    FUniverseActionPlan()
        : EstimatedSuccessRate(0.0f)
        , ExecutionTime(0.0f)
        , EnergyCost(0.0f)
        , CivilizationsRequired(0)
    {}
};

// 宇�?資�X��?
USTRUCT(BlueprintType)
struct FUniverseResourceOptimization
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ResourceType;

    UPROPERTY(BlueprintReadOnly)
    float CurrentAmount;

    UPROPERTY(BlueprintReadOnly)
    float RequiredAmount;

    UPROPERTY(BlueprintReadOnly)
    float Efficiency;

    UPROPERTY(BlueprintReadOnly)
    EResourcePriority Priority;

    UPROPERTY(BlueprintReadOnly)
    FString AllocationStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FUniverseRegion> SourceRegions;

    FUniverseResourceOptimization()
        : CurrentAmount(0.0f)
        , RequiredAmount(0.0f)
        , Efficiency(0.0f)
        , Priority(EResourcePriority::Medium)
    {}
};

// ?��X��X��?
USTRUCT(BlueprintType)
struct FCivilizationCooperation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString CivilizationName;

    UPROPERTY(BlueprintReadOnly)
    int32 TechLevel;

    UPROPERTY(BlueprintReadOnly)
    float ContributionLevel;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCooperating;

    UPROPERTY(BlueprintReadOnly)
    TArray<EUniverseSaviorStrategy> Capabilities;

    FCivilizationCooperation()
        : TechLevel(1)
        , ContributionLevel(0.0f)
        , bIsCooperating(false)
    {}
};

// 宇�X��?任�?結�?
USTRUCT(BlueprintType)
struct FUniverseMissionResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    UPROPERTY(BlueprintReadOnly)
    FString MissionName;

    UPROPERTY(BlueprintReadOnly)
    int32 CivilizationsSaved;

    UPROPERTY(BlueprintReadOnly)
    int32 StarSystemsSaved;

    UPROPERTY(BlueprintReadOnly)
    float EnergyExpended;

    UPROPERTY(BlueprintReadOnly)
    float TimeTaken;

    UPROPERTY(BlueprintReadOnly)
    FString AfterActionReport;

    FUniverseMissionResult()
        : bSuccess(false)
        , CivilizationsSaved(0)
        , StarSystemsSaved(0)
        , EnergyExpended(0.0f)
        , TimeTaken(0.0f)
    {}
};

// 宇�X��?系統
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSAIUniverseSavior : public UActorComponent
{
    GENERATED_BODY()

public:
    // 構造函X    UMingGoRTSAIUniverseSavior(};

    // 組件?�命?��?
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========== 宇�X��系統 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StartUniverseMonitoring(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StopUniverseMonitoring(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ScanForCosmicThreats(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void MonitorGalacticStability(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AnalyzeMultiversalAnomalies(};

    // ========== 威�?管�? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DetectUniverseThreats(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ClassifyCosmicThreat(const FUniverseThreatData& Threat};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void PredictUniversalThreats(float TimeHorizonYears};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AssessThreatEvolution(const FUniverseThreatData& Threat};

    // ========== ?��?策略 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateCosmicStrategy(const FUniverseThreatData& Threat};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateUniverseActionPlan(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteCosmicPlan(const FUniverseActionPlan& Plan};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateMultiversalResponse(};

    // ========== 資�?管�? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OptimizeCosmicResources(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AllocateGalacticResources(const FString& ResourceType, float Amount, EResourcePriority Priority};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void RedistributeUniversalEnergy(const TArray<FString>& CriticalRegions};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void HarvestStarEnergy(const TArray<FUniverseRegion>& TargetStars};

    // ========== ?��X��? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void EstablishCivilizationContact(const FString& CivilizationName};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void NegotiateCoalitionAgreement(const TArray<FString>& Civilizations};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateInterstellarAlliance(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ShareTechnologyWithAllies(EUniverseSaviorStrategy TechType};

    // ========== 緊急�?�?==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateCosmicEmergencyProtocol(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeployGalacticDefenseGrid(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void InitiateGenesisProtocol(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteRealityAnchorSequence(};

    // ========== 多維度�?�?==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OpenWormholeToThreat(const FVector3d& TargetLocation};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StabilizeLocalDimension(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CreateDimensionalShield(const FVector3d& Center, float Radius};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ManipulateTimeField(float TimeDilationFactor};

    // ========== 系統?�制 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateUniverseSaviorMode(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeactivateUniverseSaviorMode(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ResetUniverseSaviorSystem(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void UpgradeToOmniversalMode(};

    // ========== ?�?�查�?==========
    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float CalculateUniversalStability() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    int32 GetTotalCivilizationsAtRisk() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float GetCosmicEnergyReserves() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    EUniverseThreatLevel GetCurrentUniversalThreatLevel() const { return CurrentUniverseThreatLevel; }

    // ========== 事件委�? ==========
    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnUniverseThreatDetected OnUniverseThreatDetected;

    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnCosmicPlanGenerated OnCosmicPlanGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnGalacticResourceOptimized OnGalacticResourceOptimized;

    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnUniverseStatusChanged OnUniverseStatusChanged;

    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnCosmicMissionCompleted OnCosmicMissionCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Universe Savior Events")
    FOnCivilizationContacted OnCivilizationContacted;

private:
    // 系統?�X    UPROPERTY()
    bool bUniverseSaviorActive;

    UPROPERTY()
    bool bCosmicEmergencyActive;

    UPROPERTY()
    bool bUniverseMonitoringActive;

    UPROPERTY()
    bool bOmniversalModeActive;

    UPROPERTY()
    EUniverseThreatLevel CurrentUniverseThreatLevel;

    UPROPERTY()
    float CosmicAIEffectiveness;

    UPROPERTY()
    float UniverseMonitoringInterval;

    UPROPERTY()
    float LastCosmicMonitoringTime;

    UPROPERTY()
    float CosmicEnergyReserves;

    // ?��?存儲
    UPROPERTY()
    TArray<FUniverseThreatData> DetectedCosmicThreats;

    UPROPERTY()
    TArray<FUniverseThreatData> PredictedCosmicThreats;

    UPROPERTY()
    TArray<FUniverseActionPlan> AvailableCosmicPlans;

    UPROPERTY()
    TArray<FUniverseResourceOptimization> CosmicResourceStatus;

    UPROPERTY()
    TArray<FCivilizationCooperation> AlliedCivilizations;

    UPROPERTY()
    TArray<FString> CosmicRecommendations;

    UPROPERTY()
    TArray<FString> UniversalStrategicGoals;

    // X��計�X    FTickerDelegate UniverseMonitoringTicker;
    FDelegateHandle UniverseMonitoringTickerHandle;

    // ?�部?��?
    void ProcessCosmicThreats(};
    void UpdateUniverseStatus(};
    void OptimizeCosmicResourceAllocation(};
    void AnalyzeUniversalSituation(};
    void CoordinateGalacticResponse(};
    void UpdateCosmicAIParameters(};
    void GenerateEmergencyCosmicPlans(};
    void DeployCosmicEmergencyResources(};
    void NotifyUniverseThreat(const FUniverseThreatData& Threat};
    void NotifyCosmicPlanGenerated(const FUniverseActionPlan& Plan};
    void NotifyGalacticResourceOptimized(const FUniverseResourceOptimization& Resource};
    void NotifyUniverseStatusChange(EUniverseThreatLevel NewLevel, const FString& Message};
    void NotifyCosmicMissionCompleted(const FUniverseMissionResult& Result};
    void NotifyCivilizationContacted(const FCivilizationCooperation& Civilization};

    // 輔助?�數
    FString GenerateCosmicThreatName(};
    FString GenerateCosmicThreatDescription(};
    EUniverseSaviorStrategy GenerateRecommendedCosmicStrategy(EUniverseThreatLevel ThreatLevel};
    float CalculateCosmicSuccessRate(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy};
    float EstimateCosmicExecutionTime(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy};
    void GenerateCosmicActionSteps(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy};
    void CalculateCosmicRequiredResources(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat};
    void AssessCosmicPotentialRisks(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat};
    void AnalyzeCosmicResourceStatus(};
    void OptimizeCosmicAllocationStrategy(};
    void UpdateCosmicResourceStatus(};
    FString GenerateCosmicAllocationStrategy(EResourcePriority Priority};
    float CalculateOptimalCosmicAllocation(const FString& Need};
    void UpdateCosmicStrategyBasedOnPattern(const FString& Pattern};
    void UpdateCosmicAIEffectiveness(float Effectiveness};
    float CalculateCosmicGoalProgress(const FString& Goal};
    FString GenerateCosmicStatusMessage(EUniverseThreatLevel Level};
    bool OnUniverseMonitoringTick(float DeltaTime};
};

// 事件委�?類�?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUniverseThreatDetected, const FUniverseThreatData&, Threat};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicPlanGenerated, const FUniverseActionPlan&, Plan};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGalacticResourceOptimized, const FUniverseResourceOptimization&, Resource};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniverseStatusChanged, EUniverseThreatLevel, NewLevel, const FString&, Message};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicMissionCompleted, const FUniverseMissionResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCivilizationContacted, const FCivilizationCooperation&, Civilization};

