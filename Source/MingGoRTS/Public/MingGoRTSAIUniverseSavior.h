// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System - ?ØÊ?ÂÆáÂ??üËÉΩÂª∂‰º∏

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSAIWorldSavior.h"
#include "MingGoRTSAIUniverseSavior.generated.h"

// ÂÆáÂ?Â®ÅË?Á¥öÂà• - Ë∂ÖË?‰∏ñÁ?Á¥öÂà•?ÑÂ???UENUM(BlueprintType)
enum class EUniverseThreatLevel : uint8
{
    None            UMETA(DisplayName = "?°Â???),
    Stellar         UMETA(DisplayName = "?ÜÊ?Á¥?),          // ?Æ‰??ÜÊ?Á≥ªÁµ±?óÂΩ±??    Galactic        UMETA(DisplayName = "?ÄÊ≤≥Á?"),          // ?¥ÂÄãÈ?Ê≤≥Á≥ª?óÂΩ±??    Intergalactic   UMETA(DisplayName = "?üÁ≥ª?õÁ?"),       // Â§öÂÄãÊ?Á≥ªÂ?ÂΩ±Èüø
    Cosmic          UMETA(DisplayName = "ÂÆáÂ?Á¥?),          // ÂÆáÂ?ÁµêÊ??óÂ???    Multiverse      UMETA(DisplayName = "Â§öÂ?ÂÆáÂ?Á¥?),      // Â§öÂ?ÂÆáÂ?Á¥öÂà•
    Omniversal      UMETA(DisplayName = "?®ËÉΩÂÆáÂ?Á¥?),      // ÁµÇÊ•µÂ®ÅË?
    MAX             UMETA(Hidden)
};

// ÂÆáÂ?Â®ÅË?È°ûÂ?
UENUM(BlueprintType)
enum class EUniverseThreatType : uint8
{
    Supernova           UMETA(DisplayName = "Ë∂ÖÊñ∞?üÁ???),
    BlackHole           UMETA(DisplayName = "ÈªëÊ??¥Âºµ"),
    GammaRayBurst       UMETA(DisplayName = "‰ºΩÈ¶¨Â∞ÑÁ???),
    DarkMatterAnomaly   UMETA(DisplayName = "?óÁâ©Ë≥™Áï∞Â∏?),
    VoidExpansion       UMETA(DisplayName = "?õÁ©∫?¥Âºµ"),
    RealityDistortion   UMETA(DisplayName = "?æÂØ¶?≠Êõ≤"),
    TimeParadox         UMETA(DisplayName = "?ÇÈ??ñË?"),
    DimensionalCollapse UMETA(DisplayName = "Á∂≠Â∫¶Â¥©Â?"),
    AlienInvasion       UMETA(DisplayName = "Â§ñÊ??•‰æµ"),
    AIApocalypse        UMETA(DisplayName = "AI?´Êó•"),
    CosmicPlague        UMETA(DisplayName = "ÂÆáÂ??üÁñ´"),
    EnergyDepletion     UMETA(DisplayName = "?ΩÈ??ØÁ´≠"),
    EntropyReversal     UMETA(DisplayName = "?µÈÄÜË?"),
    QuantumInstability  UMETA(DisplayName = "?èÂ?‰∏çÁ©©ÂÆ?),
    Singularity         UMETA(DisplayName = "Â•áÈ?‰∫ã‰ª∂"),
    MAX                 UMETA(Hidden)
};

// ÂÆáÂ??ØÊ?Á≠ñÁï•
UENUM(BlueprintType)
enum class EUniverseSaviorStrategy : uint8
{
    StellarEngineering      UMETA(DisplayName = "?ÜÊ?Â∑•Á?"),
    WormholeManipulation    UMETA(DisplayName = "?≤Ê??çÊéß"),
    DimensionalShielding    UMETA(DisplayName = "Á∂≠Â∫¶Ë≠∑Áõæ"),
    TimeManipulation        UMETA(DisplayName = "?ÇÈ??çÊéß"),
    RealityAnchoring        UMETA(DisplayName = "?æÂØ¶?®Â?"),
    QuantumStabilization    UMETA(DisplayName = "?èÂ?Á©©Â?"),
    CosmicDiplomacy         UMETA(DisplayName = "ÂÆáÂ?Â§ñ‰∫§"),
    MultiversalCooperation  UMETA(DisplayName = "Â§öÂ?ÂÆáÂ??à‰?"),
    EnergyRedistribution    UMETA(DisplayName = "?ΩÈ??çÂ???),
    EntropyControl          UMETA(DisplayName = "?µÊéß??),
    GenesisProtocol         UMETA(DisplayName = "?µ‰??îË≠∞"),
    Emergency               UMETA(DisplayName = "Á∑äÊÄ•Â?Ë≠?),
    MAX                     UMETA(Hidden)
};

// ÂÆáÂ??Ä??USTRUCT(BlueprintType)
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

// ÂÆáÂ?Â®ÅË??∏Ê?
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
    float ThreatRadius; // ?âÂπ¥

    UPROPERTY(BlueprintReadOnly)
    float TimeToImpact; // Âπ?
    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    EUniverseSaviorStrategy RecommendedStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FUniverseRegion> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float EnergyRequired; // ÂÆáÂ??ΩÈ??Æ‰?

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

// ÂÆáÂ?Ë°åÂ?Ë®àÂ?
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
    float ExecutionTime; // Âπ?
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

// ÂÆáÂ?Ë≥áÊ??™Â?
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

// ?áÊ??à‰??∏Ê?
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

// ÂÆáÂ??ØÊ?‰ªªÂ?ÁµêÊ?
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

// ÂÆáÂ??ØÊ?Á≥ªÁµ±
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSAIUniverseSavior : public UActorComponent
{
    GENERATED_BODY()

public:
    // ÊßãÈÄ†ÂáΩ??    UMingGoRTSAIUniverseSavior(};

    // ÁµÑ‰ª∂?üÂëΩ?®Ê?
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========== ÂÆáÂ???éßÁ≥ªÁµ± ==========
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

    // ========== Â®ÅË?ÁÆ°Á? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DetectUniverseThreats(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ClassifyCosmicThreat(const FUniverseThreatData& Threat};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void PredictUniversalThreats(float TimeHorizonYears};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AssessThreatEvolution(const FUniverseThreatData& Threat};

    // ========== ?ØÊ?Á≠ñÁï• ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateCosmicStrategy(const FUniverseThreatData& Threat};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateUniverseActionPlan(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteCosmicPlan(const FUniverseActionPlan& Plan};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateMultiversalResponse(};

    // ========== Ë≥áÊ?ÁÆ°Á? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OptimizeCosmicResources(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AllocateGalacticResources(const FString& ResourceType, float Amount, EResourcePriority Priority};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void RedistributeUniversalEnergy(const TArray<FString>& CriticalRegions};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void HarvestStarEnergy(const TArray<FUniverseRegion>& TargetStars};

    // ========== ?áÊ??à‰? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void EstablishCivilizationContact(const FString& CivilizationName};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void NegotiateCoalitionAgreement(const TArray<FString>& Civilizations};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateInterstellarAlliance(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ShareTechnologyWithAllies(EUniverseSaviorStrategy TechType};

    // ========== Á∑äÊÄ•Â?Ë≠?==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateCosmicEmergencyProtocol(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeployGalacticDefenseGrid(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void InitiateGenesisProtocol(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteRealityAnchorSequence(};

    // ========== Â§öÁ∂≠Â∫¶Ê?‰Ω?==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OpenWormholeToThreat(const FVector3d& TargetLocation};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StabilizeLocalDimension(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CreateDimensionalShield(const FVector3d& Center, float Radius};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ManipulateTimeField(float TimeDilationFactor};

    // ========== Á≥ªÁµ±?ßÂà∂ ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateUniverseSaviorMode(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeactivateUniverseSaviorMode(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ResetUniverseSaviorSystem(};

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void UpgradeToOmniversalMode(};

    // ========== ?Ä?ãÊü•Ë©?==========
    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float CalculateUniversalStability() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    int32 GetTotalCivilizationsAtRisk() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float GetCosmicEnergyReserves() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    EUniverseThreatLevel GetCurrentUniversalThreatLevel() const { return CurrentUniverseThreatLevel; }

    // ========== ‰∫ã‰ª∂ÂßîË? ==========
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
    // Á≥ªÁµ±?Ä??    UPROPERTY()
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

    // ?∏Ê?Â≠òÂÑ≤
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

    // ??éßË®àÊ???    FTickerDelegate UniverseMonitoringTicker;
    FDelegateHandle UniverseMonitoringTickerHandle;

    // ?ßÈÉ®?πÊ?
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

    // ËºîÂä©?ΩÊï∏
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

// ‰∫ã‰ª∂ÂßîË?È°ûÂ?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUniverseThreatDetected, const FUniverseThreatData&, Threat};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicPlanGenerated, const FUniverseActionPlan&, Plan};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGalacticResourceOptimized, const FUniverseResourceOptimization&, Resource};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniverseStatusChanged, EUniverseThreatLevel, NewLevel, const FString&, Message};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicMissionCompleted, const FUniverseMissionResult&, Result};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCivilizationContacted, const FCivilizationCooperation&, Civilization};

