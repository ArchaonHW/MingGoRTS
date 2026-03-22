// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System - ????�t?X?�ੵ��

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSAIWorldSavior.h"
#include "MingGoRTSAIUniverseSavior.generated.h"

// �t??��??�ŧO - �W??�@??�ŧO???XUENUM(BlueprintType)
enum class EUniverseThreatLevel: uint8 {
    None            UMETA(DisplayName = "???X),
    Stellar         UMETA(DisplayName = "??????),          // ???X???�t��??�vX
    Galactic        UMETA(DisplayName = "??�e??"),          // ??��??�e�t??�vX
    Intergalactic   UMETA(DisplayName = "??�t????"),       // �h��??�t??�v�T
    Cosmic          UMETA(DisplayName = "�t????),          // �t??��?X??X
    Multiverse      UMETA(DisplayName = "�h??�t????),      // �h??�t??�ŧO
    Omniversal      UMETA(DisplayName = "??��t????),      // �׷���??
    MAX             UMETA(Hidden)
};

// �t??��??��??
UENUM(BlueprintType)
enum class EUniverseThreatType: uint8 {
    Supernova           UMETA(DisplayName = "�W�s???X),
    BlackHole           UMETA(DisplayName = "��?X?�i"),
    GammaRayBurst       UMETA(DisplayName = "�����g?X),
    DarkMatterAnomaly   UMETA(DisplayName = "??���貧??),
    VoidExpansion       UMETA(DisplayName = "??��??�i"),
    RealityDistortion   UMETA(DisplayName = "??��??��"),
    TimeParadox         UMETA(DisplayName = "???X???"),
    DimensionalCollapse UMETA(DisplayName = "���ױY??"),
    AlienInvasion       UMETA(DisplayName = "�~?X?�I"),
    AIApocalypse        UMETA(DisplayName = "AI??��"),
    CosmicPlague        UMETA(DisplayName = "�t?X?��"),
    EnergyDepletion     UMETA(DisplayName = "???X?��"),
    EntropyReversal     UMETA(DisplayName = "??�f??"),
    QuantumInstability  UMETA(DisplayName = "????��í??),
    Singularity         UMETA(DisplayName = "�_??�ƥ�"),
    MAX                 UMETA(Hidden)
};

// �t?X???����
UENUM(BlueprintType)
enum class EUniverseSaviorStrategy: uint8 {
    StellarEngineering      UMETA(DisplayName = "????�u??"),
    WormholeManipulation    UMETA(DisplayName = "???X?��"),
    DimensionalShielding    UMETA(DisplayName = "�����@��"),
    TimeManipulation        UMETA(DisplayName = "???X?��"),
    RealityAnchoring        UMETA(DisplayName = "??��????"),
    QuantumStabilization    UMETA(DisplayName = "????í??"),
    CosmicDiplomacy         UMETA(DisplayName = "�t??�~��"),
    MultiversalCooperation  UMETA(DisplayName = "�h??�t?X???"),
    EnergyRedistribution    UMETA(DisplayName = "???X??X),
    EntropyControl          UMETA(DisplayName = "??��X),
    GenesisProtocol         UMETA(DisplayName = "???X?ĳ"),
    Emergency               UMETA(DisplayName = "���????),
    MAX                     UMETA(Hidden)
};

// �t?X?XUSTRUCT(BlueprintType)
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

// �t??��?X???
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
    float ThreatRadius; // ??�~

    UPROPERTY(BlueprintReadOnly)
    float TimeToImpact; // ??
    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    EUniverseSaviorStrategy RecommendedStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FUniverseRegion> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float EnergyRequired; // �t?X??X???

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

// �t??��??�p??
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
    float ExecutionTime; // ??
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

// �t??��?X???
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

// ???X??X???
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

// �t?X???��??��??
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

// �t?X???�t��
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSAIUniverseSavior : public UActorComponent
{
    GENERATED_BODY()

public:
    // �c�y��X
    UMingGoRTSAIUniverseSavior();

    // �ե�??�R????
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========== �t?X??�t�� ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StartUniverseMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StopUniverseMonitoring();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ScanForCosmicThreats();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void MonitorGalacticStability();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AnalyzeMultiversalAnomalies();

    // ========== ��??��?? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DetectUniverseThreats();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ClassifyCosmicThreat(const FUniverseThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void PredictUniversalThreats(float TimeHorizonYears);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AssessThreatEvolution(const FUniverseThreatData& Threat);

    // ========== ????���� ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateCosmicStrategy(const FUniverseThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateUniverseActionPlan(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteCosmicPlan(const FUniverseActionPlan& Plan);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateMultiversalResponse();

    // ========== ��??��?? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OptimizeCosmicResources();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AllocateGalacticResources(const FString& ResourceType, float Amount, EResourcePriority Priority);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void RedistributeUniversalEnergy(const TArray<FString>& CriticalRegions);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void HarvestStarEnergy(const TArray<FUniverseRegion>& TargetStars);

    // ========== ???X??? ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void EstablishCivilizationContact(const FString& CivilizationName);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void NegotiateCoalitionAgreement(const TArray<FString>& Civilizations);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateInterstellarAlliance();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ShareTechnologyWithAllies(EUniverseSaviorStrategy TechType);

    // ========== ���????==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateCosmicEmergencyProtocol();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeployGalacticDefenseGrid();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void InitiateGenesisProtocol();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteRealityAnchorSequence();

    // ========== �h����????==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OpenWormholeToThreat(const FVector3d& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StabilizeLocalDimension();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CreateDimensionalShield(const FVector3d& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ManipulateTimeField(float TimeDilationFactor);

    // ========== �t��??�� ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateUniverseSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeactivateUniverseSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ResetUniverseSaviorSystem();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void UpgradeToOmniversalMode();

    // ========== ????�d??==========
    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float CalculateUniversalStability() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    int32 GetTotalCivilizationsAtRisk() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float GetCosmicEnergyReserves() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    EUniverseThreatLevel GetCurrentUniversalThreatLevel() const { return CurrentUniverseThreatLevel; }

    // ========== �ƥ�e?? ==========
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
    // �t��??X
    UPROPERTY()
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

    // ????�s�x
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

    // X??�p?X
    FTickerDelegate UniverseMonitoringTicker;
    FDelegateHandle UniverseMonitoringTickerHandle;

    // ??��????
    void ProcessCosmicThreats();
    void UpdateUniverseStatus();
    void OptimizeCosmicResourceAllocation();
    void AnalyzeUniversalSituation();
    void CoordinateGalacticResponse();
    void UpdateCosmicAIParameters();
    void GenerateEmergencyCosmicPlans();
    void DeployCosmicEmergencyResources();
    void NotifyUniverseThreat(const FUniverseThreatData& Threat);
    void NotifyCosmicPlanGenerated(const FUniverseActionPlan& Plan);
    void NotifyGalacticResourceOptimized(const FUniverseResourceOptimization& Resource);
    void NotifyUniverseStatusChange(EUniverseThreatLevel NewLevel, const FString& Message);
    void NotifyCosmicMissionCompleted(const FUniverseMissionResult& Result);
    void NotifyCivilizationContacted(const FCivilizationCooperation& Civilization);

    // ���U??��
    FString GenerateCosmicThreatName();
    FString GenerateCosmicThreatDescription();
    EUniverseSaviorStrategy GenerateRecommendedCosmicStrategy(EUniverseThreatLevel ThreatLevel);
    float CalculateCosmicSuccessRate(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy);
    float EstimateCosmicExecutionTime(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy);
    void GenerateCosmicActionSteps(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy);
    void CalculateCosmicRequiredResources(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat);
    void AssessCosmicPotentialRisks(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat);
    void AnalyzeCosmicResourceStatus();
    void OptimizeCosmicAllocationStrategy();
    void UpdateCosmicResourceStatus();
    FString GenerateCosmicAllocationStrategy(EResourcePriority Priority);
    float CalculateOptimalCosmicAllocation(const FString& Need);
    void UpdateCosmicStrategyBasedOnPattern(const FString& Pattern);
    void UpdateCosmicAIEffectiveness(float Effectiveness);
    float CalculateCosmicGoalProgress(const FString& Goal);
    FString GenerateCosmicStatusMessage(EUniverseThreatLevel Level);
    bool OnUniverseMonitoringTick(float DeltaTime);
);

// �ƥ�e??��??







