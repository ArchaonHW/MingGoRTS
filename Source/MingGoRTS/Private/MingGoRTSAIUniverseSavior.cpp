// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic X: Universe Savior System - 拯救宇宙功能延伸

#include "MingGoRTSAIUniverseSavior.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Math/UnrealMathUtility.h"

UMingGoRTSAIUniverseSavior::UMingGoRTSAIUniverseSavior()
    : bUniverseSaviorActive(false)
    , bCosmicEmergencyActive(false)
    , bUniverseMonitoringActive(false)
    , bOmniversalModeActive(false)
    , CurrentUniverseThreatLevel(EUniverseThreatLevel::None)
    , CosmicAIEffectiveness(0.85f)
    , UniverseMonitoringInterval(10.0f)
    , LastCosmicMonitoringTime(0.0f)
    , CosmicEnergyReserves(1000000.0f)
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化宇宙監控計時器
    UniverseMonitoringTicker = FTickerDelegate::CreateUObject(this, &UMingGoRTSAIUniverseSavior::OnUniverseMonitoringTick);
}

void UMingGoRTSAIUniverseSavior::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("AI Universe Savior System Initialized - Ready to save the universe!"));
    
    // 自動啟動宇宙監控
    StartUniverseMonitoring();
}

void UMingGoRTSAIUniverseSavior::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (bUniverseSaviorActive)
    {
        // 持續監控宇宙狀態
        if (bUniverseMonitoringActive)
        {
            ProcessCosmicThreats();
            UpdateUniverseStatus();
        }
        
        // 優化宇宙資源分配
        if (FMath::FRand() < 0.005f) // 0.5%機率每幀執行
        {
            OptimizeCosmicResourceAllocation();
        }
        
        // 監控銀河系穩定性
        if (FMath::FRand() < 0.001f) // 0.1%機率
        {
            MonitorGalacticStability();
        }
    }
}

void UMingGoRTSAIUniverseSavior::StartUniverseMonitoring()
{
    if (bUniverseMonitoringActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Universe monitoring is already active"));
        return;
    }

    bUniverseMonitoringActive = true;
    
    // 啟動監控計時器
    if (!UniverseMonitoringTickerHandle.IsValid())
    {
        UniverseMonitoringTickerHandle = FTicker::GetCoreTicker().AddTicker(UniverseMonitoringTicker, UniverseMonitoringInterval);
    }

    UE_LOG(LogTemp, Log, TEXT("Started universe monitoring across all dimensions"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("🌌 AI Universe Savior: Cosmic Monitoring Started"));
    }
}

void UMingGoRTSAIUniverseSavior::StopUniverseMonitoring()
{
    bUniverseMonitoringActive = false;
    
    if (UniverseMonitoringTickerHandle.IsValid())
    {
        FTicker::GetCoreTicker().RemoveTicker(UniverseMonitoringTickerHandle);
        UniverseMonitoringTickerHandle.Reset();
    }

    UE_LOG(LogTemp, Log, TEXT("Stopped universe monitoring"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, TEXT("🌌 AI Universe Savior: Cosmic Monitoring Stopped"));
    }
}

void UMingGoRTSAIUniverseSavior::ScanForCosmicThreats()
{
    UE_LOG(LogTemp, Log, TEXT("Scanning for cosmic threats across galaxies..."));
    
    // 模擬宇宙級威脅檢測
    if (FMath::FRand() < 0.2f) // 20%機率檢測到宇宙威脅
    {
        FUniverseThreatData NewThreat;
        NewThreat.ThreatName = GenerateCosmicThreatName();
        NewThreat.ThreatType = static_cast<EUniverseThreatType>(FMath::RandRange(0, 14));
        NewThreat.ThreatLevel = static_cast<EUniverseThreatLevel>(FMath::RandRange(1, 6));
        NewThreat.ThreatDescription = GenerateCosmicThreatDescription();
        NewThreat.ThreatLocation = FVector3d(FMath::FRand() * 100000, FMath::FRand() * 100000, FMath::FRand() * 100000);
        NewThreat.ThreatRadius = FMath::FRandRange(100, 10000); // 光年
        NewThreat.TimeToImpact = FMath::FRandRange(100, 10000); // 年
        NewThreat.Confidence = FMath::FRandRange(0.5, 1.0);
        NewThreat.RecommendedStrategy = GenerateRecommendedCosmicStrategy(NewThreat.ThreatLevel);
        NewThreat.EnergyRequired = FMath::FRandRange(10000, 1000000);
        NewThreat.CivilizationsAtRisk = FMath::RandRange(1, 1000);
        
        // 生成受影響區域
        int32 AffectedRegionCount = FMath::RandRange(1, 10);
        for (int32 i = 0; i < AffectedRegionCount; ++i)
        {
            FUniverseRegion Region;
            Region.RegionName = FString::Printf(TEXT("Galaxy Sector %d"), i);
            Region.GalacticCoordinates = FVector3d(FMath::FRand() * 1000, FMath::FRand() * 1000, FMath::FRand() * 1000);
            Region.StarCount = FMath::RandRange(1000000, 1000000000);
            Region.ThreatLevel = FMath::FRandRange(0.0f, 1.0f);
            Region.bIsStable = Region.ThreatLevel < 0.5f;
            NewThreat.AffectedRegions.Add(Region);
        }
        
        DetectedCosmicThreats.Add(NewThreat);
        
        NotifyUniverseThreat(NewThreat);
        
        UE_LOG(LogTemp, Warning, TEXT("COSMIC THREAT DETECTED: %s (Level: %d, Civilizations at Risk: %d)"), 
            *NewThreat.ThreatName, (int32)NewThreat.ThreatLevel, NewThreat.CivilizationsAtRisk);
    }
}

void UMingGoRTSAIUniverseSavior::DetectUniverseThreats()
{
    UE_LOG(LogTemp, Log, TEXT("Detecting universe-level threats..."));
    
    // 檢測多種類型的宇宙威脅
    TArray<EUniverseThreatType> ThreatTypes = {
        EUniverseThreatType::Supernova,
        EUniverseThreatType::BlackHole,
        EUniverseThreatType::GammaRayBurst,
        EUniverseThreatType::DarkMatterAnomaly,
        EUniverseThreatType::VoidExpansion
    };
    
    for (EUniverseThreatType ThreatType : ThreatTypes)
    {
        if (FMath::FRand() < 0.1f) // 10%機率每種類型
        {
            FUniverseThreatData Threat;
            Threat.ThreatType = ThreatType;
            Threat.ThreatLevel = static_cast<EUniverseThreatLevel>(FMath::RandRange(2, 5));
            
            switch (ThreatType)
            {
            case EUniverseThreatType::Supernova:
                Threat.ThreatName = TEXT("Type-II Supernova Chain Reaction");
                Threat.ThreatDescription = TEXT("Multiple stars approaching supernova phase simultaneously");
                break;
            case EUniverseThreatType::BlackHole:
                Threat.ThreatName = TEXT("Rogue Supermassive Black Hole");
                Threat.ThreatDescription = TEXT("Displaced black hole consuming stellar systems");
                break;
            case EUniverseThreatType::GammaRayBurst:
                Threat.ThreatName = TEXT("Directed Gamma Ray Burst");
                Threat.ThreatDescription = TEXT("High-energy burst threatening multiple star systems");
                break;
            case EUniverseThreatType::DarkMatterAnomaly:
                Threat.ThreatName = TEXT("Dark Matter Density Fluctuation");
                Threat.ThreatDescription = TEXT("Unusual dark matter concentration affecting gravity");
                break;
            case EUniverseThreatType::VoidExpansion:
                Threat.ThreatName = TEXT("Accelerated Void Expansion");
                Threat.ThreatDescription = TEXT("Cosmic void expanding at dangerous rate");
                break;
            default:
                break;
            }
            
            DetectedCosmicThreats.Add(Threat);
            NotifyUniverseThreat(Threat);
        }
    }
}

void UMingGoRTSAIUniverseSavior::GenerateCosmicStrategy(const FUniverseThreatData& Threat)
{
    UE_LOG(LogTemp, Log, TEXT("Generating cosmic savior strategy for: %s"), *Threat.ThreatName);
    
    EUniverseSaviorStrategy Strategy = Threat.RecommendedStrategy;
    
    // 根據威脅類型調整策略
    switch (Threat.ThreatType)
    {
    case EUniverseThreatType::Supernova:
        Strategy = EUniverseSaviorStrategy::StellarEngineering;
        break;
    case EUniverseThreatType::BlackHole:
        Strategy = EUniverseSaviorStrategy::WormholeManipulation;
        break;
    case EUniverseThreatType::GammaRayBurst:
        Strategy = EUniverseSaviorStrategy::DimensionalShielding;
        break;
    case EUniverseThreatType::DarkMatterAnomaly:
        Strategy = EUniverseSaviorStrategy::QuantumStabilization;
        break;
    case EUniverseThreatType::TimeParadox:
        Strategy = EUniverseSaviorStrategy::TimeManipulation;
        break;
    case EUniverseThreatType::RealityDistortion:
        Strategy = EUniverseSaviorStrategy::RealityAnchoring;
        break;
    default:
        break;
    }
    
    GenerateUniverseActionPlan(Threat, Strategy);
}

void UMingGoRTSAIUniverseSavior::GenerateUniverseActionPlan(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy)
{
    FUniverseActionPlan NewPlan;
    NewPlan.PlanName = FString::Printf(TEXT("Cosmic Response Plan for %s"), *Threat.ThreatName);
    NewPlan.Strategy = Strategy;
    NewPlan.EstimatedSuccessRate = CalculateCosmicSuccessRate(Threat, Strategy);
    NewPlan.ExecutionTime = EstimateCosmicExecutionTime(Threat, Strategy);
    NewPlan.EnergyCost = Threat.EnergyRequired;
    NewPlan.CivilizationsRequired = FMath::RandRange(1, 100);
    
    // 生成行動步驟
    GenerateCosmicActionSteps(NewPlan, Threat, Strategy);
    
    // 計算所需技術
    NewPlan.RequiredTechnologies = {
        TEXT("Quantum Field Manipulation"),
        TEXT("Gravitational Control"),
        TEXT("Dimensional Engineering"),
        TEXT("Cosmic Energy Harvesting")
    };
    
    // 評估潛在風險
    AssessCosmicPotentialRisks(NewPlan, Threat);
    
    AvailableCosmicPlans.Add(NewPlan);
    
    NotifyCosmicPlanGenerated(NewPlan);
    
    UE_LOG(LogTemp, Log, TEXT("Generated cosmic action plan: %s (Success Rate: %.1f%%, Energy Cost: %.0f)"), 
        *NewPlan.PlanName, NewPlan.EstimatedSuccessRate * 100, NewPlan.EnergyCost);
}

void UMingGoRTSAIUniverseSavior::ExecuteCosmicPlan(const FUniverseActionPlan& Plan)
{
    UE_LOG(LogTemp, Log, TEXT("Executing cosmic plan: %s"), *Plan.PlanName);
    
    // 模擬執行過程
    bool bSuccess = FMath::FRand() < Plan.EstimatedSuccessRate;
    
    FUniverseMissionResult Result;
    Result.bSuccess = bSuccess;
    Result.MissionName = Plan.PlanName;
    Result.CivilizationsSaved = bSuccess ? FMath::RandRange(10, 1000) : FMath::RandRange(0, 100);
    Result.StarSystemsSaved = bSuccess ? FMath::RandRange(100, 10000) : FMath::RandRange(0, 1000);
    Result.EnergyExpended = Plan.EnergyCost;
    Result.TimeTaken = Plan.ExecutionTime;
    Result.AfterActionReport = bSuccess ? 
        TEXT("Mission completed successfully. Threat neutralized.") : 
        TEXT("Mission failed. Partial evacuation only.");
    
    // 更新AI效果評分
    if (bSuccess)
    {
        CosmicAIEffectiveness = FMath::Clamp(CosmicAIEffectiveness + 0.03f, 0.0f, 1.0f);
        CosmicEnergyReserves -= Plan.EnergyCost;
    }
    else
    {
        CosmicAIEffectiveness = FMath::Clamp(CosmicAIEffectiveness - 0.02f, 0.0f, 1.0f);
    }
    
    NotifyCosmicMissionCompleted(Result);
    
    if (GEngine)
    {
        FColor MessageColor = bSuccess ? FColor::Green : FColor::Red;
        FString Message = FString::Printf(TEXT("🌌 Cosmic Mission %s: %s\nCivilizations Saved: %d, Star Systems: %d"),
            bSuccess ? TEXT("SUCCESS") : TEXT("FAILED"),
            *Plan.PlanName,
            Result.CivilizationsSaved,
            Result.StarSystemsSaved);
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, MessageColor, Message);
    }
}

void UMingGoRTSAIUniverseSavior::OptimizeCosmicResources()
{
    UE_LOG(LogTemp, Log, TEXT("Optimizing cosmic resources across the universe..."));
    
    AnalyzeCosmicResourceStatus();
    OptimizeCosmicAllocationStrategy();
    UpdateCosmicResourceStatus();
    
    // 能量儲備恢復
    CosmicEnergyReserves += FMath::FRandRange(1000, 10000);
}

void UMingGoRTSAIUniverseSavior::AllocateGalacticResources(const FString& ResourceType, float Amount, EResourcePriority Priority)
{
    FUniverseResourceOptimization ResourceData;
    ResourceData.ResourceType = ResourceType;
    ResourceData.CurrentAmount = Amount;
    ResourceData.Priority = Priority;
    ResourceData.Efficiency = FMath::FRandRange(0.7, 1.0);
    ResourceData.AllocationStrategy = GenerateCosmicAllocationStrategy(Priority);
    
    // 添加來源區域
    for (int32 i = 0; i < 3; ++i)
    {
        FUniverseRegion Source;
        Source.RegionName = FString::Printf(TEXT("Resource Sector %d"), i);
        Source.GalacticCoordinates = FVector3d(FMath::FRand() * 100, FMath::FRand() * 100, FMath::FRand() * 100);
        Source.StarCount = FMath::RandRange(1000000, 5000000);
        ResourceData.SourceRegions.Add(Source);
    }
    
    CosmicResourceStatus.Add(ResourceData);
    
    NotifyGalacticResourceOptimized(ResourceData);
    
    UE_LOG(LogTemp, Log, TEXT("Allocated %.2f cosmic units of %s (Priority: %d)"), Amount, *ResourceType, (int32)Priority);
}

void UMingGoRTSAIUniverseSavior::EstablishCivilizationContact(const FString& CivilizationName)
{
    UE_LOG(LogTemp, Log, TEXT("Establishing contact with civilization: %s"), *CivilizationName);
    
    FCivilizationCooperation NewCivilization;
    NewCivilization.CivilizationName = CivilizationName;
    NewCivilization.TechLevel = FMath::RandRange(1, 10);
    NewCivilization.ContributionLevel = FMath::FRandRange(0.0f, 1.0f);
    NewCivilization.bIsCooperating = FMath::FRand() < 0.7f; // 70%機率願意合作
    
    // 隨機分配能力
    TArray<EUniverseSaviorStrategy> AllCapabilities = {
        EUniverseSaviorStrategy::StellarEngineering,
        EUniverseSaviorStrategy::QuantumStabilization,
        EUniverseSaviorStrategy::DimensionalShielding,
        EUniverseSaviorStrategy::EnergyRedistribution
    };
    
    int32 CapabilityCount = FMath::RandRange(1, 4);
    for (int32 i = 0; i < CapabilityCount; ++i)
    {
        NewCivilization.Capabilities.Add(AllCapabilities[FMath::RandRange(0, AllCapabilities.Num() - 1)]);
    }
    
    AlliedCivilizations.Add(NewCivilization);
    
    NotifyCivilizationContacted(NewCivilization);
    
    if (GEngine)
    {
        FString Message = FString::Printf(TEXT("🌌 Contact established with %s (Tech Level: %d, Cooperation: %s)"),
            *CivilizationName,
            NewCivilization.TechLevel,
            NewCivilization.bIsCooperating ? TEXT("YES") : TEXT("NO"));
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, Message);
    }
}

void UMingGoRTSAIUniverseSavior::ActivateCosmicEmergencyProtocol()
{
    if (bCosmicEmergencyActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cosmic emergency protocol already active"));
        return;
    }
    
    bCosmicEmergencyActive = true;
    
    UE_LOG(LogTemp, Warning, TEXT("🚨 COSMIC EMERGENCY PROTOCOL ACTIVATED 🚨"));
    
    // 啟動所有可用文明
    for (FCivilizationCooperation& Civ : AlliedCivilizations)
    {
        Civ.bIsCooperating = true;
        Civ.ContributionLevel = 1.0f;
    }
    
    // 部署緊急資源
    DeployCosmicEmergencyResources();
    
    // 生成緊急計劃
    GenerateEmergencyCosmicPlans();
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, 
            TEXT("🚨 COSMIC EMERGENCY PROTOCOL ACTIVATED 🚨\nAll civilizations mobilized for universal defense"));
    }
}

void UMingGoRTSAIUniverseSavior::DeployCosmicEmergencyResources()
{
    UE_LOG(LogTemp, Log, TEXT("Deploying cosmic emergency resources..."));
    
    TArray<FString> EmergencyResources = {
        TEXT("Quantum Stabilization Arrays"),
        TEXT("Gravitational Anchor Networks"),
        TEXT("Dimensional Barrier Generators"),
        TEXT("Cosmic Energy Collectors"),
        TEXT("Stellar Control Stations"),
        TEXT("Wormhole Transit Gates"),
        TEXT("Reality Anchor Matrices"),
        TEXT("Time Dilation Buffers")
    };
    
    for (const FString& Resource : EmergencyResources)
    {
        AllocateGalacticResources(Resource, 10000.0f, EResourcePriority::Critical);
    }
}

void UMingGoRTSAIUniverseSavior::InitiateGenesisProtocol()
{
    UE_LOG(LogTemp, Log, TEXT("Initiating Genesis Protocol - Universal restoration sequence..."));
    
    // 創世協議：在最極端情況下重建宇宙
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Purple, 
            TEXT("✨ GENESIS PROTOCOL INITIATED ✨\nBeginning universal restoration sequence..."));
    }
    
    // 重置宇宙參數
    CosmicEnergyReserves = 10000000.0f;
    CosmicAIEffectiveness = 1.0f;
    
    // 清除所有威脅
    DetectedCosmicThreats.Empty();
    PredictedCosmicThreats.Empty();
    
    UE_LOG(LogTemp, Warning, TEXT("Genesis Protocol complete. Universe restored to baseline state."));
}

void UMingGoRTSAIUniverseSavior::OpenWormholeToThreat(const FVector3d& TargetLocation)
{
    UE_LOG(LogTemp, Log, TEXT("Opening wormhole to cosmic threat at coordinates: %s"), *TargetLocation.ToString());
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Cyan, 
            FString::Printf(TEXT("🌀 Wormhole opened to threat location: %s"), *TargetLocation.ToString()));
    }
}

void UMingGoRTSAIUniverseSavior::CreateDimensionalShield(const FVector3d& Center, float Radius)
{
    UE_LOG(LogTemp, Log, TEXT("Creating dimensional shield at %s with radius %.2f light-years"), 
        *Center.ToString(), Radius);
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Turquoise, 
            FString::Printf(TEXT("🛡️ Dimensional Shield deployed (Radius: %.0f ly)"), Radius));
    }
}

void UMingGoRTSAIUniverseSavior::ActivateUniverseSaviorMode()
{
    if (bUniverseSaviorActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Universe Savior mode already active"));
        return;
    }
    
    bUniverseSaviorActive = true;
    
    UE_LOG(LogTemp, Log, TEXT("🌌 AI UNIVERSE SAVIOR MODE ACTIVATED 🌌"));
    
    StartUniverseMonitoring();
    OptimizeCosmicResources();
    
    // 建立文明聯繫
    EstablishCivilizationContact(TEXT("Federation of United Worlds"));
    EstablishCivilizationContact(TEXT("Andromeda Collective"));
    EstablishCivilizationContact(TEXT("Galactic Alliance"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, 
            TEXT("🌌 AI UNIVERSE SAVIOR MODE ACTIVATED 🌌\nMonitoring all cosmic threats across dimensions"));
    }
}

void UMingGoRTSAIUniverseSavior::DeactivateUniverseSaviorMode()
{
    bUniverseSaviorActive = false;
    bCosmicEmergencyActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Universe Savior mode deactivated"));
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Gray, TEXT("🌌 Universe Savior Mode Deactivated"));
    }
}

void UMingGoRTSAIUniverseSavior::ResetUniverseSaviorSystem()
{
    UE_LOG(LogTemp, Log, TEXT("Resetting Universe Savior system..."));
    
    DetectedCosmicThreats.Empty();
    PredictedCosmicThreats.Empty();
    AvailableCosmicPlans.Empty();
    CosmicResourceStatus.Empty();
    AlliedCivilizations.Empty();
    CosmicRecommendations.Empty();
    UniversalStrategicGoals.Empty();
    
    CurrentUniverseThreatLevel = EUniverseThreatLevel::None;
    CosmicAIEffectiveness = 0.85f;
    CosmicEnergyReserves = 1000000.0f;
    bCosmicEmergencyActive = false;
    bOmniversalModeActive = false;
    
    UE_LOG(LogTemp, Log, TEXT("Universe Savior system reset completed"));
}

void UMingGoRTSAIUniverseSavior::UpgradeToOmniversalMode()
{
    bOmniversalModeActive = true;
    
    UE_LOG(LogTemp, Warning, TEXT("⚡ OMNIVERSAL MODE ACTIVATED ⚡"));
    UE_LOG(LogTemp, Warning, TEXT("System capabilities expanded to multiversal scale"));
    
    // 增加能量儲備
    CosmicEnergyReserves *= 10.0f;
    CosmicAIEffectiveness = 1.0f;
    
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Purple, 
            TEXT("⚡ OMNIVERSAL MODE ACTIVATED ⚡\nExtended to multiversal scale operations"));
    }
}

float UMingGoRTSAIUniverseSavior::CalculateUniversalStability() const
{
    float Stability = 1.0f;
    
    // 根據威脅級別計算穩定性
    for (const FUniverseThreatData& Threat : DetectedCosmicThreats)
    {
        float ThreatImpact = (float)Threat.ThreatLevel / 7.0f * Threat.Confidence;
        Stability -= ThreatImpact;
    }
    
    // 根據文明合作調整
    float CooperationBonus = 0.0f;
    for (const FCivilizationCooperation& Civ : AlliedCivilizations)
    {
        if (Civ.bIsCooperating)
        {
            CooperationBonus += 0.05f;
        }
    }
    
    Stability += CooperationBonus;
    
    return FMath::Clamp(Stability, 0.0f, 1.0f);
}

int32 UMingGoRTSAIUniverseSavior::GetTotalCivilizationsAtRisk() const
{
    int32 TotalAtRisk = 0;
    
    for (const FUniverseThreatData& Threat : DetectedCosmicThreats)
    {
        TotalAtRisk += Threat.CivilizationsAtRisk;
    }
    
    return TotalAtRisk;
}

float UMingGoRTSAIUniverseSavior::GetCosmicEnergyReserves() const
{
    return CosmicEnergyReserves;
}

// Private helper implementations
void UMingGoRTSAIUniverseSavior::ProcessCosmicThreats()
{
    for (const FUniverseThreatData& Threat : DetectedCosmicThreats)
    {
        if (Threat.TimeToImpact <= 10.0f) // 10年內即將發生
        {
            ActivateCosmicEmergencyProtocol();
            break;
        }
    }
}

void UMingGoRTSAIUniverseSavior::UpdateUniverseStatus()
{
    EUniverseThreatLevel NewThreatLevel = EUniverseThreatLevel::None;
    
    for (const FUniverseThreatData& Threat : DetectedCosmicThreats)
    {
        if (Threat.ThreatLevel > NewThreatLevel)
        {
            NewThreatLevel = Threat.ThreatLevel;
        }
    }
    
    if (NewThreatLevel != CurrentUniverseThreatLevel)
    {
        CurrentUniverseThreatLevel = NewThreatLevel;
        NotifyUniverseStatusChange(CurrentUniverseThreatLevel, GenerateCosmicStatusMessage(CurrentUniverseThreatLevel));
    }
}

void UMingGoRTSAIUniverseSavior::OptimizeCosmicResourceAllocation()
{
    for (FUniverseResourceOptimization& Resource : CosmicResourceStatus)
    {
        if (Resource.Priority == EResourcePriority::Critical)
        {
            Resource.Efficiency = FMath::Clamp(Resource.Efficiency + 0.01f, 0.0f, 1.0f);
        }
    }
}

void UMingGoRTSAIUniverseSavior::AnalyzeUniversalSituation()
{
    float UniversalStability = CalculateUniversalStability();
    
    if (UniversalStability < 0.2f)
    {
        ActivateCosmicEmergencyProtocol();
    }
    else if (UniversalStability < 0.5f)
    {
        // 生成警告建議
        CosmicRecommendations.Add(TEXT("Increase cosmic monitoring frequency"));
        CosmicRecommendations.Add(TEXT("Mobilize allied civilizations"));
    }
}

void UMingGoRTSAIUniverseSavior::CoordinateGalacticResponse()
{
    UE_LOG(LogTemp, Log, TEXT("Coordinating intergalactic response efforts..."));
    
    // 協調所有合作文明
    for (const FCivilizationCooperation& Civ : AlliedCivilizations)
    {
        if (Civ.bIsCooperating)
        {
            UE_LOG(LogTemp, Log, TEXT("Coordinating with %s for cosmic defense"), *Civ.CivilizationName);
        }
    }
}

void UMingGoRTSAIUniverseSavior::GenerateEmergencyCosmicPlans()
{
    for (const FUniverseThreatData& Threat : DetectedCosmicThreats)
    {
        if (Threat.ThreatLevel >= EUniverseThreatLevel::Cosmic)
        {
            GenerateUniverseActionPlan(Threat, EUniverseSaviorStrategy::Emergency);
            GenerateUniverseActionPlan(Threat, EUniverseSaviorStrategy::GenesisProtocol);
        }
    }
}

FString UMingGoRTSAIUniverseSavior::GenerateCosmicThreatName()
{
    TArray<FString> ThreatNames = {
        TEXT("Stellar Cascade Failure"),
        TEXT("Gravitational Singularity"),
        TEXT("Quantum Vacuum Decay"),
        TEXT("Dark Energy Surge"),
        TEXT("Dimensional Breach"),
        TEXT("Cosmic String Oscillation"),
        TEXT("Antimatter Contamination"),
        TEXT("Temporal Rift Expansion"),
        TEXT("Wormhole Network Collapse"),
        TEXT("Entropy Reversal Event"),
        TEXT("Planck Scale Instability"),
        TEXT("Multiversal Phase Shift"),
        TEXT("Cosmic Ray Tsunami"),
        TEXT("Stellar Engine Malfunction"),
        TEXT("Galactic Core Eruption")
    };
    
    return ThreatNames[FMath::RandRange(0, ThreatNames.Num() - 1)];
}

FString UMingGoRTSAIUniverseSavior::GenerateCosmicThreatDescription()
{
    TArray<FString> Descriptions = {
        TEXT("Catastrophic cosmic event requiring immediate universal intervention"),
        TEXT("Multi-galactic threat with potential universe-ending consequences"),
        TEXT("Fundamental physics anomaly threatening cosmic stability"),
        TEXT("Large-scale dimensional instability affecting multiple star systems"),
        TEXT("Accelerating cosmic phenomenon with exponential growth pattern")
    };
    
    return Descriptions[FMath::RandRange(0, Descriptions.Num() - 1)];
}

EUniverseSaviorStrategy UMingGoRTSAIUniverseSavior::GenerateRecommendedCosmicStrategy(EUniverseThreatLevel ThreatLevel)
{
    switch (ThreatLevel)
    {
    case EUniverseThreatLevel::None:
        return EUniverseSaviorStrategy::StellarEngineering;
    case EUniverseThreatLevel::Stellar:
        return EUniverseSaviorStrategy::StellarEngineering;
    case EUniverseThreatLevel::Galactic:
        return EUniverseSaviorStrategy::WormholeManipulation;
    case EUniverseThreatLevel::Intergalactic:
        return EUniverseSaviorStrategy::DimensionalShielding;
    case EUniverseThreatLevel::Cosmic:
        return EUniverseSaviorStrategy::QuantumStabilization;
    case EUniverseThreatLevel::Multiverse:
        return EUniverseSaviorStrategy::MultiversalCooperation;
    case EUniverseThreatLevel::Omniversal:
        return EUniverseSaviorStrategy::GenesisProtocol;
    default:
        return EUniverseSaviorStrategy::StellarEngineering;
    }
}

float UMingGoRTSAIUniverseSavior::CalculateCosmicSuccessRate(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy)
{
    float BaseRate = 0.6f;
    
    // 根據威脅級別調整
    BaseRate -= (float)Threat.ThreatLevel * 0.08f;
    
    // 根據策略調整
    switch (Strategy)
    {
    case EUniverseSaviorStrategy::GenesisProtocol:
        BaseRate += 0.3f;
        break;
    case EUniverseSaviorStrategy::MultiversalCooperation:
        BaseRate += 0.25f;
        break;
    case EUniverseSaviorStrategy::QuantumStabilization:
        BaseRate += 0.2f;
        break;
    case EUniverseSaviorStrategy::DimensionalShielding:
        BaseRate += 0.15f;
        break;
    default:
        break;
    }
    
    return FMath::Clamp(BaseRate, 0.1f, 0.95f);
}

float UMingGoRTSAIUniverseSavior::EstimateCosmicExecutionTime(const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy)
{
    float BaseTime = 100.0f; // 基礎時間（年）
    
    // 根據威脅級別調整
    BaseTime *= (1.0f + (float)Threat.ThreatLevel * 0.3f);
    
    // 根據策略調整
    switch (Strategy)
    {
    case EUniverseSaviorStrategy::GenesisProtocol:
        BaseTime *= 3.0f;
        break;
    case EUniverseSaviorStrategy::TimeManipulation:
        BaseTime *= 0.5f;
        break;
    case EUniverseSaviorStrategy::WormholeManipulation:
        BaseTime *= 0.7f;
        break;
    default:
        break;
    }
    
    return BaseTime;
}

void UMingGoRTSAIUniverseSavior::GenerateCosmicActionSteps(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat, EUniverseSaviorStrategy Strategy)
{
    Plan.ActionSteps.Empty();
    
    switch (Strategy)
    {
    case EUniverseSaviorStrategy::StellarEngineering:
        Plan.ActionSteps.Add(TEXT("Deploy stellar control arrays"));
        Plan.ActionSteps.Add(TEXT("Stabilize affected star systems"));
        Plan.ActionSteps.Add(TEXT("Redirect energy flows"));
        break;
    case EUniverseSaviorStrategy::WormholeManipulation:
        Plan.ActionSteps.Add(TEXT("Calculate wormhole coordinates"));
        Plan.ActionSteps.Add(TEXT("Open transit corridors"));
        Plan.ActionSteps.Add(TEXT("Evacuate threatened populations"));
        break;
    case EUniverseSaviorStrategy::DimensionalShielding:
        Plan.ActionSteps.Add(TEXT("Generate dimensional barriers"));
        Plan.ActionSteps.Add(TEXT("Anchor local space-time"));
        Plan.ActionSteps.Add(TEXT("Isolate threat containment zones"));
        break;
    case EUniverseSaviorStrategy::QuantumStabilization:
        Plan.ActionSteps.Add(TEXT("Deploy quantum stabilizers"));
        Plan.ActionSteps.Add(TEXT("Repair fundamental constants"));
        Plan.ActionSteps.Add(TEXT("Normalize field fluctuations"));
        break;
    case EUniverseSaviorStrategy::GenesisProtocol:
        Plan.ActionSteps.Add(TEXT("Initialize universal backup sequence"));
        Plan.ActionSteps.Add(TEXT("Preserve essential cosmic structures"));
        Plan.ActionSteps.Add(TEXT("Execute controlled universal reset"));
        break;
    default:
        Plan.ActionSteps.Add(TEXT("Assess cosmic situation"));
        Plan.ActionSteps.Add(TEXT("Deploy cosmic resources"));
        Plan.ActionSteps.Add(TEXT("Monitor universal progress"));
        break;
    }
}

void UMingGoRTSAIUniverseSavior::AssessCosmicPotentialRisks(FUniverseActionPlan& Plan, const FUniverseThreatData& Threat)
{
    Plan.PotentialRisks.Empty();
    Plan.MitigationStrategies.Empty();
    
    Plan.PotentialRisks.Add(TEXT("Energy depletion"));
    Plan.PotentialRisks.Add(TEXT("Civilization withdrawal"));
    Plan.PotentialRisks.Add(TEXT("Unexpected cosmic escalation"));
    Plan.PotentialRisks.Add(TEXT("Dimensional instability"));
    
    Plan.MitigationStrategies.Add(TEXT("Establish backup energy reserves"));
    Plan.MitigationStrategies.Add(TEXT("Negotiate binding cooperation treaties"));
    Plan.MitigationStrategies.Add(TEXT("Prepare contingency protocols"));
    Plan.MitigationStrategies.Add(TEXT("Deploy dimensional anchor networks"));
}

void UMingGoRTSAIUniverseSavior::AnalyzeCosmicResourceStatus()
{
    for (FUniverseResourceOptimization& Resource : CosmicResourceStatus)
    {
        Resource.Efficiency = FMath::FRandRange(0.7, 1.0);
    }
}

void UMingGoRTSAIUniverseSavior::OptimizeCosmicAllocationStrategy()
{
    for (FUniverseResourceOptimization& Resource : CosmicResourceStatus)
    {
        if (Resource.Priority == EResourcePriority::Critical)
        {
            Resource.AllocationStrategy = TEXT("Priority cosmic allocation with multiversal reserves");
        }
    }
}

FString UMingGoRTSAIUniverseSavior::GenerateCosmicAllocationStrategy(EResourcePriority Priority)
{
    switch (Priority)
    {
    case EResourcePriority::Critical:
        return TEXT("Immediate cosmic allocation with priority override");
    case EResourcePriority::High:
        return TEXT("High priority galactic allocation");
    case EResourcePriority::Medium:
        return TEXT("Standard universal allocation procedure");
    case EResourcePriority::Low:
        return TEXT("Low priority stellar allocation");
    case EResourcePriority::Optional:
        return TEXT("Optional allocation if cosmic resources available");
    default:
        return TEXT("Standard cosmic allocation");
    }
}

FString UMingGoRTSAIUniverseSavior::GenerateCosmicStatusMessage(EUniverseThreatLevel Level)
{
    switch (Level)
    {
    case EUniverseThreatLevel::None:
        return TEXT("🌌 Universal Status: Stable - All systems nominal");
    case EUniverseThreatLevel::Stellar:
        return TEXT("⭐ Universal Status: Stellar Alert - Localized threats detected");
    case EUniverseThreatLevel::Galactic:
        return TEXT("🌠 Universal Status: Galactic Warning - Sector threats active");
    case EUniverseThreatLevel::Intergalactic:
        return TEXT("⚠️ Universal Status: Intergalactic Crisis - Multi-sector involvement");
    case EUniverseThreatLevel::Cosmic:
        return TEXT("🔴 Universal Status: Cosmic Emergency - Universal threat detected");
    case EUniverseThreatLevel::Multiverse:
        return TEXT("💥 Universal Status: Multiversal Catastrophe - Multi-dimensional threat");
    case EUniverseThreatLevel::Omniversal:
        return TEXT("☠️ Universal Status: OMNIVERSAL EXTINCTION - Reality itself at risk");
    default:
        return TEXT("🌌 Universal Status: Unknown");
    }
}

bool UMingGoRTSAIUniverseSavior::OnUniverseMonitoringTick(float DeltaTime)
{
    if (!bUniverseMonitoringActive)
    {
        return false;
    }
    
    ScanForCosmicThreats();
    AnalyzeUniversalSituation();
    
    return bUniverseMonitoringActive;
}

// Notification methods
void UMingGoRTSAIUniverseSavior::NotifyUniverseThreat(const FUniverseThreatData& Threat)
{
    OnUniverseThreatDetected.Broadcast(Threat);
}

void UMingGoRTSAIUniverseSavior::NotifyCosmicPlanGenerated(const FUniverseActionPlan& Plan)
{
    OnCosmicPlanGenerated.Broadcast(Plan);
}

void UMingGoRTSAIUniverseSavior::NotifyGalacticResourceOptimized(const FUniverseResourceOptimization& Resource)
{
    OnGalacticResourceOptimized.Broadcast(Resource);
}

void UMingGoRTSAIUniverseSavior::NotifyUniverseStatusChange(EUniverseThreatLevel NewLevel, const FString& Message)
{
    OnUniverseStatusChanged.Broadcast(NewLevel, Message);
}

void UMingGoRTSAIUniverseSavior::NotifyCosmicMissionCompleted(const FUniverseMissionResult& Result)
{
    OnCosmicMissionCompleted.Broadcast(Result);
}

void UMingGoRTSAIUniverseSavior::NotifyCivilizationContacted(const FCivilizationCooperation& Civilization)
{
    OnCivilizationContacted.Broadcast(Civilization);
}
