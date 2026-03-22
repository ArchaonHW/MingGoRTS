#include "Innovation/MingEcologicalEnvironmentSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"
#include "Math/UnrealMathUtility.h"

UMingEcologicalEnvironmentSystem::UMingEcologicalEnvironmentSystem()
{
    SimulationSpeed = 1.0f;
    SeasonDuration = 90.0f;
    WeatherChangeProbability = 0.1f;
    BiodiversityThreshold = 0.7f;
    EcosystemHealthThreshold = 50.0f;
    bEnableSeasonalChanges = true;
    bEnableWeatherSimulation = true;
    bEnableResourceRegeneration = true;
    
    bIsInitialized = false;
    ActiveEcosystemCount = 0;
    GlobalBiodiversityIndex = 0.0f;
    GlobalEcologicalBalance = EEcologicalBalance::Stable;
    
    LastSimulationTime = 0.0f;
    TotalEventsTriggered = 0;
    TotalEcosystemsCreated = 0;
    TotalResourcesRegenerated = 0.0f;
}

bool UMingEcologicalEnvironmentSystem::InitializeEcologicalSystem()
{
    if (bIsInitialized)
    {
        return true;
    }
    
    // 初始化統計數據
    SystemStats.Empty();
    SystemStats.Add(TEXT("ActiveEcosystems"), 0.0f);
    SystemStats.Add(TEXT("GlobalBiodiversity"), 0.0f);
    SystemStats.Add(TEXT("ActiveEvents"), 0.0f);
    SystemStats.Add(TEXT("TotalResources"), 0.0f);
    SystemStats.Add(TEXT("AverageHealth"), 100.0f);
    SystemStats.Add(TEXT("SimulationSpeed"), SimulationSpeed);
    
    bIsInitialized = true;
    
    // 設置定時器
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(
            SimulationTimerHandle,
            this,
            &UMingEcologicalEnvironmentSystem::UpdateEnvironmentalFactors,
            1.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            EventProcessingTimerHandle,
            this,
            &UMingEcologicalEnvironmentSystem::ProcessEcologicalEvents,
            0.5f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            ResourceRegenerationTimerHandle,
            [this]()
            {
                for (const auto& EcosystemPair : Ecosystems)
                {
                    ProcessResourceRegeneration(EcosystemPair.Key, 1.0f);
                }
            },
            2.0f,
            true
        );
        
        World->GetTimerManager().SetTimer(
            StatisticsUpdateTimerHandle,
            this,
            &UMingEcologicalEnvironmentSystem::UpdateGlobalStatistics,
            5.0f,
            true
        );
    }
    
    return true;
}

FString UMingEcologicalEnvironmentSystem::CreateEcosystem(EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area)
{
    if (!bIsInitialized)
    {
        return FString();
    }
    
    if (!ValidateEcosystemCreation(EcosystemType, EcosystemName, Location, Area))
    {
        return FString();
    }
    
    FString EcosystemID = GenerateUniqueEcosystemID();
    FEcosystemData Ecosystem = CreateDefaultEcosystemData(EcosystemID, EcosystemType, EcosystemName, Location, Area);
    
    Ecosystems.Add(EcosystemID, Ecosystem);
    ActiveEcosystemCount++;
    TotalEcosystemsCreated++;
    
    // 初始化資源數據
    if (bEnableResourceRegeneration)
    {
        FResourceRegenerationData WaterResource;
        WaterResource.ResourceID = GenerateUniqueResourceID();
        WaterResource.ResourceType = TEXT("Water");
        WaterResource.ResourceName = TEXT("Fresh Water");
        WaterResource.CurrentAmount = 1000.0f;
        WaterResource.MaxCapacity = 2000.0f;
        WaterResource.RegenerationRate = 10.0f;
        WaterResource.ConsumptionRate = 5.0f;
        WaterResource.RegenerationEfficiency = 1.0f;
        
        ResourceData.Add(WaterResource.ResourceID, WaterResource);
    }
    
    // 更新統計
    SystemStats[TEXT("ActiveEcosystems")] = static_cast<float>(ActiveEcosystemCount);
    
    // 觸發事件
    OnEcosystemCreated.Broadcast(Ecosystem);
    
    return EcosystemID;
}

bool UMingEcologicalEnvironmentSystem::UpdateEnvironmentalFactor(const FString& EcosystemID, EEnvironmentalFactor FactorType, float NewValue)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return false;
    }
    
    if (!ValidateEnvironmentalFactorUpdate(EcosystemID, FactorType, NewValue))
    {
        return false;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    FEnvironmentalFactorData& Factor = Ecosystem.EnvironmentalFactors[FactorType];
    
    Factor.CurrentValue = FMath::Clamp(NewValue, Factor.MinValue, Factor.MaxValue);
    Factor.LastUpdateTime = FDateTime::Now();
    
    // 檢查是否為關鍵狀態
    Factor.bIsCritical = FMath::Abs(Factor.CurrentValue - Factor.OptimalValue) > (Factor.MaxValue - Factor.MinValue) * 0.3f;
    
    // 觸發事件
    OnEnvironmentalFactorChanged.Broadcast(Factor);
    
    return true;
}

bool UMingEcologicalEnvironmentSystem::SimulateEnvironmentalChange(const FString& EcosystemID, float DeltaTime)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return false;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 模擬環境因子的自然變化
    for (auto& FactorPair : Ecosystem.EnvironmentalFactors)
    {
        FEnvironmentalFactorData& Factor = FactorPair.Value;
        
        // 添加隨機波動
        float RandomChange = FMath::RandRange(-1.0f, 1.0f) * Factor.ChangeRate * DeltaTime * SimulationSpeed;
        float NewValue = Factor.CurrentValue + RandomChange;
        
        // 應用季節性影響
        if (bEnableSeasonalChanges)
        {
            float SeasonalEffect = CalculateSeasonalEffect(Factor.FactorType, Ecosystem.CurrentSeason);
            NewValue += SeasonalEffect * DeltaTime * SimulationSpeed;
        }
        
        UpdateEnvironmentalFactor(EcosystemID, Factor.FactorType, NewValue);
    }
    
    // 更新季節和天氣
    if (bEnableSeasonalChanges)
    {
        UpdateSeasonAndWeather(EcosystemID, DeltaTime);
    }
    
    // 處理資源再生
    if (bEnableResourceRegeneration)
    {
        ProcessResourceRegeneration(EcosystemID, DeltaTime);
    }
    
    // 計算生態平衡
    EEcologicalBalance NewBalance = CalculateEcologicalBalance(EcosystemID);
    if (NewBalance != Ecosystem.BalanceState)
    {
        Ecosystem.BalanceState = NewBalance;
        OnEcosystemBalanceChanged.Broadcast(Ecosystem);
    }
    
    return true;
}

FString UMingEcologicalEnvironmentSystem::TriggerEcologicalEvent(const FString& EcosystemID, const FString& EventType, float Severity)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return FString();
    }
    
    FString EventID = GenerateUniqueEventID();
    FEcologicalEvent Event = CreateDefaultEventData(EventID, EventType, EcosystemID, Severity);
    
    Event.bIsActive = true;
    Event.StartTime = FDateTime::Now();
    Event.EndTime = Event.StartTime + FTimespan::FromSeconds(Event.Duration);
    
    ActiveEvents.Add(Event);
    TotalEventsTriggered++;
    
    // 應用事件效果
    ApplyEventEffects(EcosystemID, Event);
    
    // 更新統計
    SystemStats[TEXT("ActiveEvents")] = static_cast<float>(ActiveEvents.Num());
    
    // 觸發事件
    OnEcologicalEventOccurred.Broadcast(Event);
    
    return EventID;
}

FEcologicalImpactAssessment UMingEcologicalEnvironmentSystem::AssessEcologicalImpact(const FString& SourceType, const FString& TargetEcosystemID, const TMap<EEnvironmentalFactor, float>& Impacts)
{
    FEcologicalImpactAssessment Assessment;
    Assessment.AssessmentID = GenerateUniqueEventID();
    Assessment.SourceType = SourceType;
    Assessment.SourceDescription = FString::Printf(TEXT("Impact assessment for %s"), *SourceType);
    Assessment.TargetEcosystemID = TargetEcosystemID;
    Assessment.FactorImpacts = Impacts;
    Assessment.AssessmentTime = FDateTime::Now();
    
    if (Ecosystems.Contains(TargetEcosystemID))
    {
        const FEcosystemData& Ecosystem = Ecosystems[TargetEcosystemID];
        
        // 計算物種影響
        for (const auto& SpeciesPair : Ecosystem.SpeciesPopulation)
        {
            float Impact = 0.0f;
            for (const auto& ImpactPair : Impacts)
            {
                Impact += FMath::Abs(ImpactPair.Value);
            }
            Assessment.SpeciesImpacts.Add(SpeciesPair.Key, Impact);
        }
        
        // 計算總體影響
        for (const auto& ImpactPair : Impacts)
        {
            Assessment.OverallImpact += FMath::Abs(ImpactPair.Value);
        }
        
        // 計算恢復時間
        Assessment.RecoveryTime = Assessment.OverallImpact * 10.0f;
        
        // 確定風險等級
        if (Assessment.OverallImpact < 10.0f)
        {
            Assessment.RiskLevel = TEXT("Low");
        }
        else if (Assessment.OverallImpact < 30.0f)
        {
            Assessment.RiskLevel = TEXT("Medium");
        }
        else
        {
            Assessment.RiskLevel = TEXT("High");
        }
        
        // 生成緩解策略
        Assessment.MitigationStrategies.Add(TEXT("Reduce pollution sources"));
        Assessment.MitigationStrategies.Add(TEXT("Restore natural habitats"));
        Assessment.MitigationStrategies.Add(TEXT("Implement conservation measures"));
        
        // 生成建議行動
        Assessment.RecommendedActions.Add(TEXT("Monitor environmental factors closely"));
        Assessment.RecommendedActions.Add(TEXT("Implement emergency response plan"));
        Assessment.RecommendedActions.Add(TEXT("Engage local community in conservation"));
    }
    
    return Assessment;
}

bool UMingEcologicalEnvironmentSystem::ProcessResourceRegeneration(const FString& EcosystemID, float DeltaTime)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return false;
    }
    
    const FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    for (auto& ResourcePair : ResourceData)
    {
        FResourceRegenerationData& Resource = ResourcePair.Value;
        
        if (!Resource.bIsRegenerating)
        {
            continue;
        }
        
        // 計算再生量
        float RegenerationAmount = Resource.RegenerationRate * DeltaTime * SimulationSpeed * Resource.RegenerationEfficiency;
        
        // 檢查環境因子要求
        bool bRequirementsMet = true;
        for (const auto& RequirementPair : Resource.FactorRequirements)
        {
            FString FactorName = RequirementPair.Key;
            float RequiredValue = RequirementPair.Value;
            
            // 簡化的因子檢查
            if (Ecosystem.EnvironmentalFactors.Num() > 0)
            {
                const FEnvironmentalFactorData& Factor = Ecosystem.EnvironmentalFactors[EEnvironmentalFactor::Temperature];
                if (FMath::Abs(Factor.CurrentValue - RequiredValue) > 10.0f)
                {
                    bRequirementsMet = false;
                    break;
                }
            }
        }
        
        if (bRequirementsMet)
        {
            Resource.CurrentAmount = FMath::Clamp(
                Resource.CurrentAmount + RegenerationAmount,
                0.0f,
                Resource.MaxCapacity
            );
            
            TotalResourcesRegenerated += RegenerationAmount;
            
            // 觸發事件
            OnResourceRegenerated.Broadcast(Resource.ResourceID, RegenerationAmount);
        }
    }
    
    return true;
}

bool UMingEcologicalEnvironmentSystem::UpdateSeasonAndWeather(const FString& EcosystemID, float DeltaTime)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return false;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 更新季節（簡化實現）
    static float SeasonAccumulator = 0.0f;
    SeasonAccumulator += DeltaTime * SimulationSpeed;
    
    if (SeasonAccumulator >= SeasonDuration)
    {
        SeasonAccumulator = 0.0f;
        ESeason NewSeason = CalculateNextSeason(Ecosystem.CurrentSeason);
        
        if (NewSeason != Ecosystem.CurrentSeason)
        {
            Ecosystem.CurrentSeason = NewSeason;
            ApplySeasonalEffects(EcosystemID, NewSeason);
            OnSeasonChanged.Broadcast(NewSeason);
        }
    }
    
    // 更新天氣
    if (bEnableWeatherSimulation && FMath::RandRange(0.0f, 1.0f) < WeatherChangeProbability * DeltaTime)
    {
        EWeatherType NewWeather = CalculateWeather(Ecosystem.CurrentSeason, Ecosystem.EnvironmentalFactors);
        
        if (NewWeather != Ecosystem.CurrentWeather)
        {
            Ecosystem.CurrentWeather = NewWeather;
            ApplyWeatherEffects(EcosystemID, NewWeather);
            OnWeatherChanged.Broadcast(NewWeather);
        }
    }
    
    return true;
}

EEcologicalBalance UMingEcologicalEnvironmentSystem::CalculateEcologicalBalance(const FString& EcosystemID)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return EEcologicalBalance::Collapsed;
    }
    
    const FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 計算生態系統健康度
    float HealthScore = CalculateEcosystemHealth(Ecosystem);
    
    // 計算生物多樣性指數
    float BiodiversityScore = CalculateBiodiversityIndex(EcosystemID);
    
    // 計算綜合平衡分數
    float BalanceScore = (HealthScore + BiodiversityScore * 100.0f) / 2.0f;
    
    // 確定平衡狀態
    if (BalanceScore >= 90.0f)
    {
        return EEcologicalBalance::Perfect;
    }
    else if (BalanceScore >= 75.0f)
    {
        return EEcologicalBalance::Good;
    }
    else if (BalanceScore >= 60.0f)
    {
        return EEcologicalBalance::Stable;
    }
    else if (BalanceScore >= 40.0f)
    {
        return EEcologicalBalance::Unstable;
    }
    else if (BalanceScore >= 20.0f)
    {
        return EEcologicalBalance::Degraded;
    }
    else
    {
        return EEcologicalBalance::Collapsed;
    }
}

FEcosystemData UMingEcologicalEnvironmentSystem::GetEcosystemData(const FString& EcosystemID) const
{
    if (Ecosystems.Contains(EcosystemID))
    {
        return Ecosystems[EcosystemID];
    }
    
    return FEcosystemData();
}

TMap<FString, float> UMingEcologicalEnvironmentSystem::GetSystemStatistics() const
{
    return SystemStats;
}

// 私有方法實現
FString UMingEcologicalEnvironmentSystem::GenerateUniqueEcosystemID() const
{
    return FString::Printf(TEXT("Ecosystem_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingEcologicalEnvironmentSystem::GenerateUniqueEventID() const
{
    return FString::Printf(TEXT("Event_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FString UMingEcologicalEnvironmentSystem::GenerateUniqueResourceID() const
{
    return FString::Printf(TEXT("Resource_%s_%d"), *FDateTime::Now().ToString(), FMath::RandRange(1000, 9999));
}

FEnvironmentalFactorData UMingEcologicalEnvironmentSystem::CreateDefaultFactorData(EEnvironmentalFactor FactorType, EEcosystemType EcosystemType)
{
    FEnvironmentalFactorData Factor;
    Factor.FactorType = FactorType;
    Factor.LastUpdateTime = FDateTime::Now();
    Factor.ChangeRate = 1.0f;
    Factor.ImpactWeight = 1.0f;
    Factor.bIsCritical = false;
    
    // 根據因子類型設置默認值
    switch (FactorType)
    {
    case EEnvironmentalFactor::Temperature:
        Factor.CurrentValue = 20.0f;
        Factor.OptimalValue = 22.0f;
        Factor.MinValue = -10.0f;
        Factor.MaxValue = 45.0f;
        break;
    case EEnvironmentalFactor::Humidity:
        Factor.CurrentValue = 60.0f;
        Factor.OptimalValue = 65.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 100.0f;
        break;
    case EEnvironmentalFactor::Precipitation:
        Factor.CurrentValue = 50.0f;
        Factor.OptimalValue = 60.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 200.0f;
        break;
    case EEnvironmentalFactor::Sunlight:
        Factor.CurrentValue = 70.0f;
        Factor.OptimalValue = 75.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 100.0f;
        break;
    case EEnvironmentalFactor::SoilQuality:
        Factor.CurrentValue = 80.0f;
        Factor.OptimalValue = 85.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 100.0f;
        break;
    case EEnvironmentalFactor::Biodiversity:
        Factor.CurrentValue = 70.0f;
        Factor.OptimalValue = 80.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 100.0f;
        break;
    default:
        Factor.CurrentValue = 50.0f;
        Factor.OptimalValue = 50.0f;
        Factor.MinValue = 0.0f;
        Factor.MaxValue = 100.0f;
        break;
    }
    
    return Factor;
}

FEcosystemData UMingEcologicalEnvironmentSystem::CreateDefaultEcosystemData(const FString& EcosystemID, EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area)
{
    FEcosystemData Ecosystem;
    Ecosystem.EcosystemID = EcosystemID;
    Ecosystem.EcosystemType = EcosystemType;
    Ecosystem.EcosystemName = EcosystemName;
    Ecosystem.Description = FString::Printf(TEXT("%s ecosystem"), *EcosystemName);
    Ecosystem.Location = Location;
    Ecosystem.Area = Area;
    Ecosystem.BalanceState = EEcologicalBalance::Stable;
    Ecosystem.BiodiversityIndex = 0.7f;
    Ecosystem.EcosystemHealth = 100.0f;
    Ecosystem.CurrentSeason = ESeason::Spring;
    Ecosystem.CurrentWeather = EWeatherType::Clear;
    Ecosystem.CreationTime = FDateTime::Now();
    Ecosystem.LastUpdateTime = FDateTime::Now();
    
    // 初始化環境因子
    for (int32 i = 0; i <= static_cast<int32>(EEnvironmentalFactor::VegetationDensity); ++i)
    {
        EEnvironmentalFactor FactorType = static_cast<EEnvironmentalFactor>(i);
        Ecosystem.EnvironmentalFactors.Add(FactorType, CreateDefaultFactorData(FactorType, EcosystemType));
    }
    
    // 添加默認物種
    Ecosystem.NativeSpecies.Add(TEXT("Oak Tree"));
    Ecosystem.NativeSpecies.Add(TEXT("Pine Tree"));
    Ecosystem.NativeSpecies.Add(TEXT("Deer"));
    Ecosystem.NativeSpecies.Add(TEXT("Rabbit"));
    Ecosystem.NativeSpecies.Add(TEXT("Bird"));
    
    // 設置物種數量
    for (const FString& Species : Ecosystem.NativeSpecies)
    {
        Ecosystem.SpeciesPopulation.Add(Species, FMath::RandRange(50, 200));
    }
    
    return Ecosystem;
}

FEcologicalEvent UMingEcologicalEnvironmentSystem::CreateDefaultEventData(const FString& EventID, const FString& EventType, const FString& EcosystemID, float Severity)
{
    FEcologicalEvent Event;
    Event.EventID = EventID;
    Event.EventType = EventType;
    Event.EventName = FString::Printf(TEXT("%s Event"), *EventType);
    Event.Description = FString::Printf(TEXT("A %s event occurred in the ecosystem"), *EventType);
    Event.AffectedEcosystemID = EcosystemID;
    Event.Severity = Severity;
    Event.Duration = Severity * 60.0f; // 嚴重程度影響持續時間
    Event.bIsActive = true;
    Event.bIsRecurring = false;
    
    return Event;
}

void UMingEcologicalEnvironmentSystem::UpdateEnvironmentalFactors(const FString& EcosystemID, float DeltaTime)
{
    SimulateEnvironmentalChange(EcosystemID, DeltaTime);
}

void UMingEcologicalEnvironmentSystem::ProcessEcologicalEvents(float DeltaTime)
{
    FDateTime CurrentTime = FDateTime::Now();
    
    for (int32 i = ActiveEvents.Num() - 1; i >= 0; i--)
    {
        FEcologicalEvent& Event = ActiveEvents[i];
        
        if (CurrentTime >= Event.EndTime)
        {
            Event.bIsActive = false;
            ActiveEvents.RemoveAt(i);
        }
    }
    
    // 更新統計
    SystemStats[TEXT("ActiveEvents")] = static_cast<float>(ActiveEvents.Num());
}

void UMingEcologicalEnvironmentSystem::UpdateGlobalStatistics()
{
    // 計算全局生物多樣性指數
    float TotalBiodiversity = 0.0f;
    float TotalHealth = 0.0f;
    
    for (const auto& EcosystemPair : Ecosystems)
    {
        const FEcosystemData& Ecosystem = EcosystemPair.Value;
        TotalBiodiversity += Ecosystem.BiodiversityIndex;
        TotalHealth += Ecosystem.EcosystemHealth;
    }
    
    if (Ecosystems.Num() > 0)
    {
        GlobalBiodiversityIndex = TotalBiodiversity / Ecosystems.Num();
        SystemStats[TEXT("GlobalBiodiversity")] = GlobalBiodiversityIndex;
        SystemStats[TEXT("AverageHealth")] = TotalHealth / Ecosystems.Num();
    }
    
    SystemStats[TEXT("TotalResources")] = TotalResourcesRegenerated;
}

ESeason UMingEcologicalEnvironmentSystem::CalculateNextSeason(ESeason CurrentSeason)
{
    switch (CurrentSeason)
    {
    case ESeason::Spring:
        return ESeason::Summer;
    case ESeason::Summer:
        return ESeason::Autumn;
    case ESeason::Autumn:
        return ESeason::Winter;
    case ESeason::Winter:
        return ESeason::Spring;
    default:
        return ESeason::Spring;
    }
}

EWeatherType UMingEcologicalEnvironmentSystem::CalculateWeather(ESeason CurrentSeason, const TMap<EEnvironmentalFactor, FEnvironmentalFactorData>& Factors)
{
    // 簡化的天氣計算
    float Random = FMath::RandRange(0.0f, 1.0f);
    
    if (CurrentSeason == ESeason::Winter)
    {
        return Random < 0.6f ? EWeatherType::Snowy : EWeatherType::Cloudy;
    }
    else if (CurrentSeason == ESeason::Summer)
    {
        return Random < 0.7f ? EWeatherType::Clear : EWeatherType::Cloudy;
    }
    else
    {
        return Random < 0.5f ? EWeatherType::Clear : EWeatherType::Cloudy;
    }
}

float UMingEcologicalEnvironmentSystem::CalculateEcosystemHealth(const FEcosystemData& Ecosystem) const
{
    float HealthScore = 0.0f;
    int32 FactorCount = 0;
    
    for (const auto& FactorPair : Ecosystem.EnvironmentalFactors)
    {
        const FEnvironmentalFactorData& Factor = FactorPair.Value;
        float Deviation = FMath::Abs(Factor.CurrentValue - Factor.OptimalValue);
        float MaxDeviation = Factor.MaxValue - Factor.MinValue;
        float FactorHealth = 1.0f - (Deviation / MaxDeviation);
        
        HealthScore += FactorHealth;
        FactorCount++;
    }
    
    return FactorCount > 0 ? (HealthScore / FactorCount) * 100.0f : 0.0f;
}

float UMingEcologicalEnvironmentSystem::CalculateSpeciesDiversity(const FEcosystemData& Ecosystem) const
{
    if (Ecosystem.SpeciesPopulation.Num() == 0)
    {
        return 0.0f;
    }
    
    int32 TotalPopulation = 0;
    for (const auto& SpeciesPair : Ecosystem.SpeciesPopulation)
    {
        TotalPopulation += SpeciesPair.Value;
    }
    
    if (TotalPopulation == 0)
    {
        return 0.0f;
    }
    
    float DiversityIndex = 0.0f;
    for (const auto& SpeciesPair : Ecosystem.SpeciesPopulation)
    {
        float Proportion = static_cast<float>(SpeciesPair.Value) / TotalPopulation;
        DiversityIndex -= Proportion * FMath::Loge(Proportion);
    }
    
    return DiversityIndex;
}

void UMingEcologicalEnvironmentSystem::ApplySeasonalEffects(const FString& EcosystemID, ESeason NewSeason)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 根據季節調整環境因子
    switch (NewSeason)
    {
    case ESeason::Spring:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Temperature, 18.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 70.0f);
        break;
    case ESeason::Summer:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Temperature, 28.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Sunlight, 85.0f);
        break;
    case ESeason::Autumn:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Temperature, 15.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 40.0f);
        break;
    case ESeason::Winter:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Temperature, 5.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 20.0f);
        break;
    }
}

void UMingEcologicalEnvironmentSystem::ApplyWeatherEffects(const FString& EcosystemID, EWeatherType NewWeather)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 根據天氣調整環境因子
    switch (NewWeather)
    {
    case EWeatherType::Rainy:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Humidity, 85.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 80.0f);
        break;
    case EWeatherType::Stormy:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::WindSpeed, 25.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 90.0f);
        break;
    case EWeatherType::Snowy:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Temperature, -2.0f);
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Precipitation, 30.0f);
        break;
    case EWeatherType::Clear:
        UpdateEnvironmentalFactor(EcosystemID, EEnvironmentalFactor::Sunlight, 90.0f);
        break;
    }
}

void UMingEcologicalEnvironmentSystem::ApplyEventEffects(const FString& EcosystemID, const FEcologicalEvent& Event)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return;
    }
    
    FEcosystemData& Ecosystem = Ecosystems[EcosystemID];
    
    // 應用事件的環境影響
    for (const auto& ImpactPair : Event.EnvironmentalImpacts)
    {
        EEnvironmentalFactor FactorType = ImpactPair.Key;
        float Impact = ImpactPair.Value * Event.Severity;
        
        if (Ecosystem.EnvironmentalFactors.Contains(FactorType))
        {
            float CurrentValue = Ecosystem.EnvironmentalFactors[FactorType].CurrentValue;
            UpdateEnvironmentalFactor(EcosystemID, FactorType, CurrentValue + Impact);
        }
    }
    
    // 影響物種數量
    for (const FString& Species : Event.AffectedSpecies)
    {
        if (Ecosystem.SpeciesPopulation.Contains(Species))
        {
            int32 CurrentPopulation = Ecosystem.SpeciesPopulation[Species];
            int32 NewPopulation = FMath::Max(0, 
                static_cast<int32>(CurrentPopulation * (1.0f - Event.Severity * 0.1f)));
            Ecosystem.SpeciesPopulation[Species] = NewPopulation;
        }
    }
}

bool UMingEcologicalEnvironmentSystem::ValidateEcosystemCreation(EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area) const
{
    return !EcosystemName.IsEmpty() && Area > 0.0f;
}

bool UMingEcologicalEnvironmentSystem::ValidateEnvironmentalFactorUpdate(const FString& EcosystemID, EEnvironmentalFactor FactorType, float NewValue) const
{
    return Ecosystems.Contains(EcosystemID);
}

float UMingEcologicalEnvironmentSystem::CalculateSeasonalEffect(EEnvironmentalFactor FactorType, ESeason Season) const
{
    // 簡化的季節性影響計算
    switch (FactorType)
    {
    case EEnvironmentalFactor::Temperature:
        switch (Season)
        {
        case ESeason::Spring: return 0.1f;
        case ESeason::Summer: return 0.2f;
        case ESeason::Autumn: return -0.1f;
        case ESeason::Winter: return -0.2f;
        }
        break;
    case EEnvironmentalFactor::Precipitation:
        switch (Season)
        {
        case ESeason::Spring: return 0.3f;
        case ESeason::Summer: return -0.1f;
        case ESeason::Autumn: return 0.1f;
        case ESeason::Winter: return -0.2f;
        }
        break;
    }
    
    return 0.0f;
}
