// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System - 拯救宇宙功能延伸

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSAIWorldSavior.h"
#include "MingGoRTSAIUniverseSavior.generated.h"

// 宇宙威脅級別 - 超越世界級別的威脅
UENUM(BlueprintType)
enum class EUniverseThreatLevel : uint8
{
    None            UMETA(DisplayName = "無威脅"),
    Stellar         UMETA(DisplayName = "恆星級"),          // 單一恆星系統受影響
    Galactic        UMETA(DisplayName = "銀河級"),          // 整個銀河系受影響
    Intergalactic   UMETA(DisplayName = "星系際級"),       // 多個星系受影響
    Cosmic          UMETA(DisplayName = "宇宙級"),          // 宇宙結構受威脅
    Multiverse      UMETA(DisplayName = "多元宇宙級"),      // 多元宇宙級別
    Omniversal      UMETA(DisplayName = "全能宇宙級"),      // 終極威脅
    MAX             UMETA(Hidden)
};

// 宇宙威脅類型
UENUM(BlueprintType)
enum class EUniverseThreatType : uint8
{
    Supernova           UMETA(DisplayName = "超新星爆炸"),
    BlackHole           UMETA(DisplayName = "黑洞擴張"),
    GammaRayBurst       UMETA(DisplayName = "伽馬射線暴"),
    DarkMatterAnomaly   UMETA(DisplayName = "暗物質異常"),
    VoidExpansion       UMETA(DisplayName = "虛空擴張"),
    RealityDistortion   UMETA(DisplayName = "現實扭曲"),
    TimeParadox         UMETA(DisplayName = "時間悖論"),
    DimensionalCollapse UMETA(DisplayName = "維度崩塌"),
    AlienInvasion       UMETA(DisplayName = "外星入侵"),
    AIApocalypse        UMETA(DisplayName = "AI末日"),
    CosmicPlague        UMETA(DisplayName = "宇宙瘟疫"),
    EnergyDepletion     UMETA(DisplayName = "能量枯竭"),
    EntropyReversal     UMETA(DisplayName = "熵逆轉"),
    QuantumInstability  UMETA(DisplayName = "量子不穩定"),
    Singularity         UMETA(DisplayName = "奇點事件"),
    MAX                 UMETA(Hidden)
};

// 宇宙拯救策略
UENUM(BlueprintType)
enum class EUniverseSaviorStrategy : uint8
{
    StellarEngineering      UMETA(DisplayName = "恆星工程"),
    WormholeManipulation    UMETA(DisplayName = "蟲洞操控"),
    DimensionalShielding    UMETA(DisplayName = "維度護盾"),
    TimeManipulation        UMETA(DisplayName = "時間操控"),
    RealityAnchoring        UMETA(DisplayName = "現實錨定"),
    QuantumStabilization    UMETA(DisplayName = "量子穩定"),
    CosmicDiplomacy         UMETA(DisplayName = "宇宙外交"),
    MultiversalCooperation  UMETA(DisplayName = "多元宇宙合作"),
    EnergyRedistribution    UMETA(DisplayName = "能量重分配"),
    EntropyControl          UMETA(DisplayName = "熵控制"),
    GenesisProtocol         UMETA(DisplayName = "創世協議"),
    Emergency               UMETA(DisplayName = "緊急協議"),
    MAX                     UMETA(Hidden)
};

// 宇宙區域
USTRUCT(BlueprintType)
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

// 宇宙威脅數據
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
    float ThreatRadius; // 光年

    UPROPERTY(BlueprintReadOnly)
    float TimeToImpact; // 年

    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    UPROPERTY(BlueprintReadOnly)
    EUniverseSaviorStrategy RecommendedStrategy;

    UPROPERTY(BlueprintReadOnly)
    TArray<FUniverseRegion> AffectedRegions;

    UPROPERTY(BlueprintReadOnly)
    float EnergyRequired; // 宇宙能量單位

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

// 宇宙行動計劃
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
    float ExecutionTime; // 年

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

// 宇宙資源優化
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

// 文明合作數據
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

// 宇宙拯救任務結果
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

// 宇宙拯救系統
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGGORTS_API UMingGoRTSAIUniverseSavior : public UActorComponent
{
    GENERATED_BODY()

public:
    // 構造函數
    UMingGoRTSAIUniverseSavior();

    // 組件生命周期
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ========== 宇宙監控系統 ==========
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

    // ========== 威脅管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DetectUniverseThreats();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ClassifyCosmicThreat(const FUniverseThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void PredictUniversalThreats(float TimeHorizonYears);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AssessThreatEvolution(const FUniverseThreatData& Threat);

    // ========== 拯救策略 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateCosmicStrategy(const FUniverseThreatData& Threat);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void GenerateUniverseActionPlan(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteCosmicPlan(const FUniverseActionPlan& Plan);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateMultiversalResponse();

    // ========== 資源管理 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OptimizeCosmicResources();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void AllocateGalacticResources(const FString& ResourceType, float Amount, EResourcePriority Priority);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void RedistributeUniversalEnergy(const TArray<FString>& CriticalRegions);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void HarvestStarEnergy(const TArray<FUniverseRegion>& TargetStars);

    // ========== 文明合作 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void EstablishCivilizationContact(const FString& CivilizationName);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void NegotiateCoalitionAgreement(const TArray<FString>& Civilizations);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CoordinateInterstellarAlliance();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ShareTechnologyWithAllies(EUniverseSaviorStrategy TechType);

    // ========== 緊急協議 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateCosmicEmergencyProtocol();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeployGalacticDefenseGrid();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void InitiateGenesisProtocol();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ExecuteRealityAnchorSequence();

    // ========== 多維度操作 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void OpenWormholeToThreat(const FVector3d& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void StabilizeLocalDimension();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void CreateDimensionalShield(const FVector3d& Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ManipulateTimeField(float TimeDilationFactor);

    // ========== 系統控制 ==========
    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ActivateUniverseSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void DeactivateUniverseSaviorMode();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void ResetUniverseSaviorSystem();

    UFUNCTION(BlueprintCallable, Category = "Universe Savior")
    void UpgradeToOmniversalMode();

    // ========== 狀態查詢 ==========
    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float CalculateUniversalStability() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    int32 GetTotalCivilizationsAtRisk() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    float GetCosmicEnergyReserves() const;

    UFUNCTION(BlueprintPure, Category = "Universe Savior")
    EUniverseThreatLevel GetCurrentUniversalThreatLevel() const { return CurrentUniverseThreatLevel; }

    // ========== 事件委託 ==========
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
    // 系統狀態
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

    // 數據存儲
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

    // 監控計時器
    FTickerDelegate UniverseMonitoringTicker;
    FDelegateHandle UniverseMonitoringTickerHandle;

    // 內部方法
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

    // 輔助函數
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
};

// 事件委託類型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUniverseThreatDetected, const FUniverseThreatData&, Threat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicPlanGenerated, const FUniverseActionPlan&, Plan);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGalacticResourceOptimized, const FUniverseResourceOptimization&, Resource);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUniverseStatusChanged, EUniverseThreatLevel, NewLevel, const FString&, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCosmicMissionCompleted, const FUniverseMissionResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCivilizationContacted, const FCivilizationCooperation&, Civilization);
