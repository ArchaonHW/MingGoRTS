#include "Innovation/MingRTSEcosystemSimulation.h"
#include "Math/UnrealMathUtility.h"

UMingRTSEcosystemSimulation::UMingRTSEcosystemSimulation()
    : CurrentSeason(ESeasonType::Spring)
    , NextZoneId(1)
    , SimulationTime(0.0f)
{
}

void UMingRTSEcosystemSimulation::InitializeEcosystem()
{
    EcosystemZones.Empty();
    ZoneSpecies.Empty();
    CurrentSeason = ESeasonType::Spring;
    NextZoneId = 1;
    SimulationTime = 0.0f;
}

void UMingRTSEcosystemSimulation::ShutdownEcosystem()
{
    EcosystemZones.Empty();
    ZoneSpecies.Empty();
}

int32 UMingRTSEcosystemSimulation::CreateEcosystemZone(EEcosystemZone ZoneType, FVector Center, float Radius)
{
    int32 NewZoneId = NextZoneId++;
    FEcosystemZoneData NewZone;
    NewZone.ZoneType = ZoneType;
    NewZone.ZoneCenter = Center;
    NewZone.ZoneRadius = Radius;
    NewZone.Climate.CurrentSeason = CurrentSeason;

    // 根據生態區類型初始化資源
    switch (ZoneType)
    {
    case EEcosystemZone::Forest:
        NewZone.Resources.Add(EResourceType::Wood, FEcosystemResource());
        NewZone.Resources[EResourceType::Wood].CurrentAmount = 200.0f;
        NewZone.Resources[EResourceType::Wood].MaxCapacity = 500.0f;
        NewZone.CarryingCapacity = 800.0f;
        break;

    case EEcosystemZone::Grassland:
        NewZone.Resources.Add(EResourceType::Food, FEcosystemResource());
        NewZone.Resources[EResourceType::Food].CurrentAmount = 300.0f;
        NewZone.Resources[EResourceType::Food].MaxCapacity = 600.0f;
        NewZone.CarryingCapacity = 1000.0f;
        break;

    case EEcosystemZone::River:
    case EEcosystemZone::Lake:
        NewZone.Resources.Add(EResourceType::Water, FEcosystemResource());
        NewZone.Resources[EResourceType::Water].CurrentAmount = 1000.0f;
        NewZone.Resources[EResourceType::Water].MaxCapacity = 2000.0f;
        NewZone.CarryingCapacity = 500.0f;
        break;

    case EEcosystemZone::Desert:
        NewZone.Resources.Add(EResourceType::Water, FEcosystemResource());
        NewZone.Resources[EResourceType::Water].CurrentAmount = 50.0f;
        NewZone.Resources[EResourceType::Water].MaxCapacity = 100.0f;
        NewZone.Resources[EResourceType::Water].RegenerationRate = 0.2f;
        NewZone.CarryingCapacity = 200.0f;
        break;

    case EEcosystemZone::Mountain:
        NewZone.Resources.Add(EResourceType::Stone, FEcosystemResource());
        NewZone.Resources[EResourceType::Stone].CurrentAmount = 500.0f;
        NewZone.Resources[EResourceType::Stone].MaxCapacity = 1000.0f;
        NewZone.CarryingCapacity = 300.0f;
        break;

    default:
        break;
    }

    // 所有區域都有基礎生物多樣性
    FEcosystemResource Biodiversity;
    Biodiversity.ResourceType = EResourceType::Biodiversity;
    Biodiversity.CurrentAmount = 50.0f;
    Biodiversity.MaxCapacity = 100.0f;
    NewZone.Resources.Add(EResourceType::Biodiversity, Biodiversity);

    EcosystemZones.Add(NewZoneId, NewZone);
    ZoneSpecies.Add(NewZoneId, TArray<FSpeciesPopulation>());

    return NewZoneId;
}

void UMingRTSEcosystemSimulation::RemoveEcosystemZone(int32 ZoneId)
{
    EcosystemZones.Remove(ZoneId);
    ZoneSpecies.Remove(ZoneId);
}

void UMingRTSEcosystemSimulation::UpdateZoneResources(int32 ZoneId, EResourceType ResourceType, float Amount)
{
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        if (FEcosystemResource* Resource = Zone->Resources.Find(ResourceType))
        {
            Resource->CurrentAmount = FMath::Clamp(Resource->CurrentAmount + Amount, 0.0f, Resource->MaxCapacity);
        }
        else if (Amount > 0)
        {
            FEcosystemResource NewResource;
            NewResource.ResourceType = ResourceType;
            NewResource.CurrentAmount = FMath::Min(Amount, 100.0f);
            NewResource.MaxCapacity = 100.0f;
            Zone->Resources.Add(ResourceType, NewResource);
        }
    }
}

FEcosystemZoneData UMingRTSEcosystemSimulation::GetZoneData(int32 ZoneId) const
{
    if (const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        return *Zone;
    }
    return FEcosystemZoneData();
}

TArray<int32> UMingRTSEcosystemSimulation::GetAllZones() const
{
    TArray<int32> Result;
    EcosystemZones.GetKeys(Result);
    return Result;
}

TArray<int32> UMingRTSEcosystemSimulation::GetZonesByType(EEcosystemZone ZoneType) const
{
    TArray<int32> Result;
    for (const auto& Pair : EcosystemZones)
    {
        if (Pair.Value.ZoneType == ZoneType)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

void UMingRTSEcosystemSimulation::SetSeason(ESeasonType NewSeason)
{
    CurrentSeason = NewSeason;
    OnSeasonChanged.Broadcast(NewSeason);

    // 更新所有區域的季節
    for (auto& Pair : EcosystemZones)
    {
        Pair.Value.Climate.CurrentSeason = NewSeason;

        // 根據季節調整溫度
        float BaseTemp = 20.0f;
        switch (NewSeason)
        {
        case ESeasonType::Spring:
            Pair.Value.Climate.Temperature = BaseTemp + FMath::RandRange(-5.0f, 5.0f);
            break;
        case ESeasonType::Summer:
            Pair.Value.Climate.Temperature = BaseTemp + FMath::RandRange(10.0f, 20.0f);
            break;
        case ESeasonType::Autumn:
            Pair.Value.Climate.Temperature = BaseTemp + FMath::RandRange(-5.0f, 5.0f);
            break;
        case ESeasonType::Winter:
            Pair.Value.Climate.Temperature = BaseTemp + FMath::RandRange(-20.0f, -5.0f);
            break;
        default:
            break;
        }
    }
}

void UMingRTSEcosystemSimulation::SetWeather(int32 ZoneId, EWeatherType Weather)
{
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        Zone->Climate.CurrentWeather = Weather;

        // 根據天氣調整資源
        switch (Weather)
        {
        case EWeatherType::Rain:
            if (FEcosystemResource* Water = Zone->Resources.Find(EResourceType::Water))
            {
                Water->CurrentAmount = FMath::Min(Water->CurrentAmount + 20.0f, Water->MaxCapacity);
            }
            if (FEcosystemResource* Food = Zone->Resources.Find(EResourceType::Food))
            {
                Food->CurrentAmount = FMath::Min(Food->CurrentAmount + 5.0f, Food->MaxCapacity);
            }
            break;

        case EWeatherType::Drought:
            if (FEcosystemResource* Water = Zone->Resources.Find(EResourceType::Water))
            {
                Water->CurrentAmount = FMath::Max(Water->CurrentAmount - 30.0f, 0.0f);
            }
            if (FEcosystemResource* Food = Zone->Resources.Find(EResourceType::Food))
            {
                Food->CurrentAmount = FMath::Max(Food->CurrentAmount - 20.0f, 0.0f);
            }
            break;

        case EWeatherType::Snow:
            Zone->Climate.Temperature = FMath::Min(Zone->Climate.Temperature, 0.0f);
            if (FEcosystemResource* Food = Zone->Resources.Find(EResourceType::Food))
            {
                Food->CurrentAmount = FMath::Max(Food->CurrentAmount - 10.0f, 0.0f);
            }
            break;

        case EWeatherType::Storm:
            if (FEcosystemResource* Wood = Zone->Resources.Find(EResourceType::Wood))
            {
                Wood->CurrentAmount = FMath::Max(Wood->CurrentAmount - 10.0f, 0.0f);
            }
            break;

        default:
            break;
        }
    }
}

void UMingRTSEcosystemSimulation::UpdateClimate(int32 ZoneId, float TemperatureDelta, float HumidityDelta)
{
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        Zone->Climate.Temperature += TemperatureDelta;
        Zone->Climate.Humidity = FMath::Clamp(Zone->Climate.Humidity + HumidityDelta, 0.0f, 100.0f);
    }
}

FClimateCondition UMingRTSEcosystemSimulation::GetZoneClimate(int32 ZoneId) const
{
    if (const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        return Zone->Climate;
    }
    return FClimateCondition();
}

void UMingRTSEcosystemSimulation::SimulateWeatherChange()
{
    for (auto& Pair : EcosystemZones)
    {
        // 基於當前天氣和季節計算天氣變化概率
        float ChangeProbability = 0.1f;
        if (FMath::RandRange(0.0f, 1.0f) < ChangeProbability)
        {
            EWeatherType NewWeather = EWeatherType::Clear;

            switch (Pair.Value.Climate.CurrentSeason)
            {
            case ESeasonType::Spring:
                {
                    float Roll = FMath::RandRange(0.0f, 1.0f);
                    if (Roll < 0.5f) NewWeather = EWeatherType::Clear;
                    else if (Roll < 0.7f) NewWeather = EWeatherType::Cloudy;
                    else if (Roll < 0.85f) NewWeather = EWeatherType::Rain;
                    else NewWeather = EWeatherType::Fog;
                }
                break;

            case ESeasonType::Summer:
                {
                    float Roll = FMath::RandRange(0.0f, 1.0f);
                    if (Roll < 0.6f) NewWeather = EWeatherType::Clear;
                    else if (Roll < 0.8f) NewWeather = EWeatherType::Cloudy;
                    else if (Roll < 0.9f) NewWeather = EWeatherType::Rain;
                    else NewWeather = EWeatherType::Storm;
                }
                break;

            case ESeasonType::Autumn:
                {
                    float Roll = FMath::RandRange(0.0f, 1.0f);
                    if (Roll < 0.4f) NewWeather = EWeatherType::Clear;
                    else if (Roll < 0.6f) NewWeather = EWeatherType::Cloudy;
                    else if (Roll < 0.8f) NewWeather = EWeatherType::Rain;
                    else if (Roll < 0.9f) NewWeather = EWeatherType::Fog;
                    else NewWeather = EWeatherType::Snow;
                }
                break;

            case ESeasonType::Winter:
                {
                    float Roll = FMath::RandRange(0.0f, 1.0f);
                    if (Roll < 0.5f) NewWeather = EWeatherType::Clear;
                    else if (Roll < 0.6f) NewWeather = EWeatherType::Cloudy;
                    else if (Roll < 0.85f) NewWeather = EWeatherType::Snow;
                    else NewWeather = EWeatherType::Fog;
                }
                break;

            default:
                break;
            }

            SetWeather(Pair.Key, NewWeather);
        }
    }
}

void UMingRTSEcosystemSimulation::AddResource(int32 ZoneId, EResourceType ResourceType, float Amount)
{
    UpdateZoneResources(ZoneId, ResourceType, Amount);
}

void UMingRTSEcosystemSimulation::ConsumeResource(int32 ZoneId, EResourceType ResourceType, float Amount)
{
    UpdateZoneResources(ZoneId, ResourceType, -Amount);
}

void UMingRTSEcosystemSimulation::RegenerateResources(float DeltaTime)
{
    for (auto& Pair : EcosystemZones)
    {
        for (auto& ResourcePair : Pair.Value.Resources)
        {
            FEcosystemResource& Resource = ResourcePair.Value;

            float RegenRate = CalculateResourceRegeneration(Resource, Pair.Value.ZoneType, Pair.Value.Climate);
            float RegenAmount = RegenRate * DeltaTime;

            Resource.CurrentAmount = FMath::Min(Resource.CurrentAmount + RegenAmount, Resource.MaxCapacity);

            // 消耗資源（自然消耗）
            float Consumption = Resource.ConsumptionRate * DeltaTime;
            Resource.CurrentAmount = FMath::Max(Resource.CurrentAmount - Consumption, 0.0f);

            // 檢查資源耗盡
            if (Resource.CurrentAmount <= 0.0f && Resource.MaxCapacity > 0.0f)
            {
                OnResourceDepleted.Broadcast(Pair.Key, Resource.ResourceType);
            }
        }
    }
}

float UMingRTSEcosystemSimulation::GetResourceAvailability(int32 ZoneId, EResourceType ResourceType) const
{
    if (const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        if (const FEcosystemResource* Resource = Zone->Resources.Find(ResourceType))
        {
            return Resource->CurrentAmount / FMath::Max(Resource->MaxCapacity, 1.0f);
        }
    }
    return 0.0f;
}

float UMingRTSEcosystemSimulation::CalculateResourceBalance(int32 ZoneId) const
{
    const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId);
    if (!Zone)
    {
        return 0.0f;
    }

    float TotalBalance = 0.0f;
    int32 ResourceCount = 0;

    for (const auto& Pair : Zone->Resources)
    {
        const FEcosystemResource& Resource = Pair.Value;
        float Balance = Resource.CurrentAmount / FMath::Max(Resource.MaxCapacity, 1.0f);
        TotalBalance += Balance;
        ResourceCount++;
    }

    return ResourceCount > 0 ? TotalBalance / ResourceCount : 0.0f;
}

void UMingRTSEcosystemSimulation::AddSpecies(int32 ZoneId, const FSpeciesPopulation& Species)
{
    if (TArray<FSpeciesPopulation>* SpeciesList = ZoneSpecies.Find(ZoneId))
    {
        SpeciesList->Add(Species);
    }
}

void UMingRTSEcosystemSimulation::UpdateSpeciesPopulation(int32 ZoneId, const FString& SpeciesName, int32 NewCount)
{
    if (TArray<FSpeciesPopulation>* SpeciesList = ZoneSpecies.Find(ZoneId))
    {
        for (auto& Species : *SpeciesList)
        {
            if (Species.SpeciesName == SpeciesName)
            {
                Species.PopulationCount = FMath::Max(0, NewCount);
                break;
            }
        }
    }
}

void UMingRTSEcosystemSimulation::SimulatePopulationGrowth(float DeltaTime)
{
    for (auto& Pair : ZoneSpecies)
    {
        int32 ZoneId = Pair.Key;
        FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId);
        if (!Zone)
        {
            continue;
        }

        for (auto& Species : Pair.Value)
        {
            // 計算資源可用性對種群的影響
            float ResourceSatisfaction = 1.0f;
            for (EResourceType RequiredResource : Species.RequiredResources)
            {
                float Availability = GetResourceAvailability(ZoneId, RequiredResource);
                ResourceSatisfaction *= Availability;
            }

            // 環境承載量限制
            float CarryingCapacity = Zone->CarryingCapacity;
            float CurrentPop = Zone->CurrentPopulation;
            float CapacityFactor = 1.0f - (CurrentPop / FMath::Max(CarryingCapacity, 1.0f));

            // 污染影響
            float PollutionFactor = 1.0f - (Zone->PollutionLevel * 0.5f);

            // 計算淨增長率
            float NetGrowthRate = (Species.GrowthRate * ResourceSatisfaction * CapacityFactor * PollutionFactor) - Species.DeathRate;

            // 應用增長
            int32 PopulationChange = FMath::RoundToInt(Species.PopulationCount * NetGrowthRate * DeltaTime);
            Species.PopulationCount = FMath::Max(0, Species.PopulationCount + PopulationChange);

            // 更新區域總人口
            Zone->CurrentPopulation = 0;
            for (const auto& S : Pair.Value)
            {
                Zone->CurrentPopulation += S.PopulationCount;
            }
        }
    }
}

TArray<FSpeciesPopulation> UMingRTSEcosystemSimulation::GetZoneSpecies(int32 ZoneId) const
{
    if (const TArray<FSpeciesPopulation>* SpeciesList = ZoneSpecies.Find(ZoneId))
    {
        return *SpeciesList;
    }
    return TArray<FSpeciesPopulation>();
}

void UMingRTSEcosystemSimulation::ApplyPollution(int32 ZoneId, float PollutionAmount)
{
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        Zone->PollutionLevel = FMath::Clamp(Zone->PollutionLevel + PollutionAmount, 0.0f, 1.0f);
        Zone->bIsPolluted = Zone->PollutionLevel > 0.3f;
        Zone->HealthIndex = CalculateZoneHealth(ZoneId);

        if (Zone->HealthIndex < 0.3f)
        {
            OnZoneDegraded.Broadcast(ZoneId, Zone->HealthIndex);
        }

        // 污染可能擴散到鄰近區域
        if (Zone->PollutionLevel > 0.5f && FMath::RandRange(0.0f, 1.0f) < 0.1f)
        {
            SpreadPollution(ZoneId, PollutionAmount * 0.3f);
        }
    }
}

void UMingRTSEcosystemSimulation::CleanUpPollution(int32 ZoneId, float CleanupAmount)
{
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        Zone->PollutionLevel = FMath::Max(0.0f, Zone->PollutionLevel - CleanupAmount);
        Zone->bIsPolluted = Zone->PollutionLevel > 0.3f;
        Zone->HealthIndex = CalculateZoneHealth(ZoneId);
    }
}

void UMingRTSEcosystemSimulation::TriggerEcosystemEvent(const FEcosystemEvent& Event)
{
    for (auto& Pair : EcosystemZones)
    {
        if (Pair.Value.ZoneType == Event.AffectedZone || Event.AffectedZone == EEcosystemZone::None)
        {
            // 應用資源影響
            for (const auto& Impact : Event.ResourceImpacts)
            {
                UpdateZoneResources(Pair.Key, Impact.Key, Impact.Value * Event.ImpactSeverity);
            }

            // 更新健康度
            Pair.Value.HealthIndex = FMath::Max(0.0f, Pair.Value.HealthIndex - Event.ImpactSeverity * 0.2f);

            if (Pair.Value.HealthIndex < 0.3f)
            {
                OnZoneDegraded.Broadcast(Pair.Key, Pair.Value.HealthIndex);
            }
        }
    }
}

void UMingRTSEcosystemSimulation::ApplyHumanActivityImpact(int32 ZoneId, float ActivityIntensity, float Duration)
{
    // 人類活動產生污染
    ApplyPollution(ZoneId, ActivityIntensity * 0.01f * Duration);

    // 消耗資源
    if (FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId))
    {
        for (auto& ResourcePair : Zone->Resources)
        {
            float Consumption = ActivityIntensity * 0.5f * Duration;
            ResourcePair.Value.CurrentAmount = FMath::Max(0.0f, ResourcePair.Value.CurrentAmount - Consumption);
        }
    }
}

float UMingRTSEcosystemSimulation::CalculateZoneHealth(int32 ZoneId) const
{
    const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId);
    if (!Zone)
    {
        return 0.0f;
    }

    // 基於多個因素計算健康度
    float ResourceHealth = CalculateResourceBalance(ZoneId);
    float BiodiversityHealth = GetResourceAvailability(ZoneId, EResourceType::Biodiversity);
    float PollutionHealth = 1.0f - Zone->PollutionLevel;

    // 人口壓力
    float PopulationPressure = 1.0f - FMath::Min(1.0f, Zone->CurrentPopulation / FMath::Max(Zone->CarryingCapacity, 1.0f));

    float Health = (ResourceHealth * 0.3f + BiodiversityHealth * 0.3f + PollutionHealth * 0.25f + PopulationPressure * 0.15f);

    return FMath::Clamp(Health, 0.0f, 1.0f);
}

float UMingRTSEcosystemSimulation::CalculateBiodiversityIndex(int32 ZoneId) const
{
    if (const TArray<FSpeciesPopulation>* SpeciesList = ZoneSpecies.Find(ZoneId))
    {
        // 計算物種豐富度和均勻度
        int32 SpeciesCount = SpeciesList->Num();
        if (SpeciesCount == 0)
        {
            return 0.0f;
        }

        int32 TotalPopulation = 0;
        for (const auto& Species : *SpeciesList)
        {
            TotalPopulation += Species.PopulationCount;
        }

        if (TotalPopulation == 0)
        {
            return 0.0f;
        }

        // 簡化的生物多樣性指數計算
        float Biodiversity = SpeciesCount * FMath::Loge(TotalPopulation / FMath::Max(SpeciesCount, 1));
        return FMath::Clamp(Biodiversity / 100.0f, 0.0f, 1.0f);
    }
    return 0.0f;
}

float UMingRTSEcosystemSimulation::CalculateCarryingCapacity(int32 ZoneId) const
{
    const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId);
    if (!Zone)
    {
        return 0.0f;
    }

    float BaseCapacity = Zone->CarryingCapacity;

    // 資源影響承載量
    float ResourceFactor = CalculateResourceBalance(ZoneId);

    // 氣候影響
    float ClimateFactor = 1.0f;
    if (Zone->Climate.Temperature < -10.0f || Zone->Climate.Temperature > 40.0f)
    {
        ClimateFactor = 0.7f;
    }

    // 污染影響
    float PollutionFactor = 1.0f - Zone->PollutionLevel;

    return BaseCapacity * ResourceFactor * ClimateFactor * PollutionFactor;
}

float UMingRTSEcosystemSimulation::PredictEcosystemCollapseRisk(int32 ZoneId) const
{
    float Health = CalculateZoneHealth(ZoneId);
    float Biodiversity = CalculateBiodiversityIndex(ZoneId);
    const FEcosystemZoneData* Zone = EcosystemZones.Find(ZoneId);

    if (!Zone)
    {
        return 1.0f;
    }

    // 崩潰風險計算
    float CollapseRisk = 0.0f;

    if (Health < 0.3f)
    {
        CollapseRisk += 0.4f;
    }
    if (Biodiversity < 0.2f)
    {
        CollapseRisk += 0.3f;
    }
    if (Zone->PollutionLevel > 0.7f)
    {
        CollapseRisk += 0.2f;
    }
    if (Zone->CurrentPopulation > Zone->CarryingCapacity * 1.2f)
    {
        CollapseRisk += 0.1f;
    }

    return FMath::Clamp(CollapseRisk, 0.0f, 1.0f);
}

void UMingRTSEcosystemSimulation::SimulateEcosystemTick(float DeltaTime)
{
    SimulationTime += DeltaTime;

    // 更新氣候
    UpdateClimateEffects(DeltaTime);

    // 模擬天氣變化
    SimulateWeatherChange();

    // 資源再生
    RegenerateResources(DeltaTime);

    // 種群增長
    SimulatePopulationGrowth(DeltaTime);

    // 物種遷移
    MigrateSpeciesBetweenZones(DeltaTime);

    // 更新所有區域健康度
    for (auto& Pair : EcosystemZones)
    {
        Pair.Value.HealthIndex = CalculateZoneHealth(Pair.Key);
    }
}

void UMingRTSEcosystemSimulation::AdvanceSeason()
{
    ESeasonType NextSeason = CurrentSeason;
    switch (CurrentSeason)
    {
    case ESeasonType::Spring:
        NextSeason = ESeasonType::Summer;
        break;
    case ESeasonType::Summer:
        NextSeason = ESeasonType::Autumn;
        break;
    case ESeasonType::Autumn:
        NextSeason = ESeasonType::Winter;
        break;
    case ESeasonType::Winter:
        NextSeason = ESeasonType::Spring;
        break;
    default:
        break;
    }

    SetSeason(NextSeason);
}

void UMingRTSEcosystemSimulation::ProcessResourceFlow()
{
    // 資源在區域間流動（例如水從河流流向濕地）
    for (const auto& SourcePair : EcosystemZones)
    {
        if (SourcePair.Value.ZoneType == EEcosystemZone::River)
        {
            // 河流向相鄰區域輸送水資源
            for (auto& TargetPair : EcosystemZones)
            {
                if (TargetPair.Key != SourcePair.Key)
                {
                    float Distance = FVector::Distance(SourcePair.Value.ZoneCenter, TargetPair.Value.ZoneCenter);
                    if (Distance < 2000.0f)
                    {
                        float FlowAmount = 5.0f * (1.0f - Distance / 2000.0f);
                        UpdateZoneResources(TargetPair.Key, EResourceType::Water, FlowAmount);
                    }
                }
            }
        }
    }
}

FEcosystemStatistics UMingRTSEcosystemSimulation::GetEcosystemStatistics() const
{
    FEcosystemStatistics Stats;
    Stats.TotalZones = EcosystemZones.Num();

    float TotalHealth = 0.0f;
    float TotalBiodiversity = 0.0f;
    float TotalTemperature = 0.0f;

    for (const auto& Pair : EcosystemZones)
    {
        if (Pair.Value.HealthIndex >= 0.7f)
        {
            Stats.HealthyZones++;
        }
        if (Pair.Value.bIsPolluted)
        {
            Stats.PollutedZones++;
        }

        TotalHealth += Pair.Value.HealthIndex;
        TotalBiodiversity += CalculateBiodiversityIndex(Pair.Key);
        TotalTemperature += Pair.Value.Climate.Temperature;
    }

    if (Stats.TotalZones > 0)
    {
        Stats.AverageHealthIndex = TotalHealth / Stats.TotalZones;
        Stats.TotalBiodiversity = TotalBiodiversity / Stats.TotalZones;
        Stats.AverageTemperature = TotalTemperature / Stats.TotalZones;
    }

    for (const auto& Pair : ZoneSpecies)
    {
        Stats.ActiveSpecies += Pair.Value.Num();
    }

    // 生態系統穩定性基於健康區域比例
    Stats.EcosystemStability = Stats.TotalZones > 0 ? (Stats.HealthyZones / (float)Stats.TotalZones) : 0.0f;

    return Stats;
}

void UMingRTSEcosystemSimulation::GenerateEcosystemReport(const FString& ReportPath)
{
    FEcosystemStatistics Stats = GetEcosystemStatistics();

    FString Report = FString::Printf(
        TEXT("=== 生態環境模擬報告 ===\n")
        TEXT("總區域數: %d\n")
        TEXT("健康區域: %d\n")
        TEXT("污染區域: %d\n")
        TEXT("平均健康指數: %.2f\n")
        TEXT("生物多樣性: %.2f\n")
        TEXT("平均溫度: %.1f°C\n")
        TEXT("活躍物種數: %d\n")
        TEXT("生態系統穩定性: %.2f\n")
        TEXT("當前季節: %d\n")
        TEXT("模擬時間: %.1f\n"),
        Stats.TotalZones,
        Stats.HealthyZones,
        Stats.PollutedZones,
        Stats.AverageHealthIndex,
        Stats.TotalBiodiversity,
        Stats.AverageTemperature,
        Stats.ActiveSpecies,
        Stats.EcosystemStability,
        (int32)CurrentSeason,
        SimulationTime
    );

    UE_LOG(LogTemp, Log, TEXT("%s"), *Report);
}

TArray<int32> UMingRTSEcosystemSimulation::FindDegradedZones() const
{
    TArray<int32> Result;
    for (const auto& Pair : EcosystemZones)
    {
        if (Pair.Value.HealthIndex < 0.3f || Pair.Value.bIsPolluted)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

TArray<int32> UMingRTSEcosystemSimulation::FindResourceRichZones() const
{
    TArray<int32> Result;
    for (const auto& Pair : EcosystemZones)
    {
        float ResourceBalance = CalculateResourceBalance(Pair.Key);
        if (ResourceBalance > 0.7f)
        {
            Result.Add(Pair.Key);
        }
    }
    return Result;
}

float UMingRTSEcosystemSimulation::CalculateResourceRegeneration(const FEcosystemResource& Resource, EEcosystemZone ZoneType, const FClimateCondition& Climate) const
{
    float BaseRegen = Resource.RegenerationRate;

    // 季節影響
    BaseRegen *= GetSeasonalTemperatureMultiplier(Climate.CurrentSeason);

    // 天氣影響
    BaseRegen *= GetWeatherResourceMultiplier(Climate.CurrentWeather, Resource.ResourceType);

    // 生態區類型影響
    switch (ZoneType)
    {
    case EEcosystemZone::Forest:
        if (Resource.ResourceType == EResourceType::Wood) BaseRegen *= 1.5f;
        break;
    case EEcosystemZone::Grassland:
        if (Resource.ResourceType == EResourceType::Food) BaseRegen *= 1.3f;
        break;
    case EEcosystemZone::River:
    case EEcosystemZone::Lake:
        if (Resource.ResourceType == EResourceType::Water) BaseRegen *= 2.0f;
        break;
    default:
        break;
    }

    return BaseRegen;
}

float UMingRTSEcosystemSimulation::CalculatePollutionImpact(float PollutionLevel, EEcosystemZone ZoneType) const
{
    float Impact = PollutionLevel;

    // 不同生態區對污染的抵抗力不同
    switch (ZoneType)
    {
    case EEcosystemZone::Forest:
        Impact *= 0.8f; // 森林有一定淨化能力
        break;
    case EEcosystemZone::Wetland:
        Impact *= 0.6f; // 濕地淨化能力強
        break;
    case EEcosystemZone::Desert:
        Impact *= 1.2f; // 沙漠脆弱
        break;
    default:
        break;
    }

    return Impact;
}

void UMingRTSEcosystemSimulation::SpreadPollution(int32 SourceZoneId, float Amount)
{
    const FEcosystemZoneData* SourceZone = EcosystemZones.Find(SourceZoneId);
    if (!SourceZone)
    {
        return;
    }

    for (auto& Pair : EcosystemZones)
    {
        if (Pair.Key != SourceZoneId)
        {
            float Distance = FVector::Distance(SourceZone->ZoneCenter, Pair.Value.ZoneCenter);
            if (Distance < 1500.0f)
            {
                float SpreadAmount = Amount * (1.0f - Distance / 1500.0f) * 0.5f;
                Pair.Value.PollutionLevel = FMath::Min(1.0f, Pair.Value.PollutionLevel + SpreadAmount);
                Pair.Value.bIsPolluted = Pair.Value.PollutionLevel > 0.3f;
            }
        }
    }
}

void UMingRTSEcosystemSimulation::MigrateSpeciesBetweenZones(float DeltaTime)
{
    // 簡化的物種遷移模型
    for (const auto& SourcePair : ZoneSpecies)
    {
        int32 SourceZoneId = SourcePair.Key;
        const FEcosystemZoneData* SourceZone = EcosystemZones.Find(SourceZoneId);
        if (!SourceZone)
        {
            continue;
        }

        for (auto& Species : const_cast<TArray<FSpeciesPopulation>&>(SourcePair.Value))
        {
            if (Species.MigrationRate > 0.0f && Species.PopulationCount > 10)
            {
                // 尋找更適合的目標區域
                for (const auto& TargetPair : EcosystemZones)
                {
                    if (TargetPair.Key != SourceZoneId)
                    {
                        // 檢查區域是否適合該物種
                        if (TargetPair.Value.ZoneType == Species.PreferredZone)
                        {
                            float Distance = FVector::Distance(SourceZone->ZoneCenter, TargetPair.Value.ZoneCenter);
                            if (Distance < 3000.0f)
                            {
                                // 計算遷移數量
                                int32 Migrants = FMath::RoundToInt(Species.PopulationCount * Species.MigrationRate * DeltaTime * (1.0f - Distance / 3000.0f));
                                if (Migrants > 0)
                                {
                                    Species.PopulationCount -= Migrants;

                                    // 在目標區域增加物種（或創建新種群）
                                    if (TArray<FSpeciesPopulation>* TargetSpecies = ZoneSpecies.Find(TargetPair.Key))
                                    {
                                        bool bFound = false;
                                        for (auto& TargetPop : *TargetSpecies)
                                        {
                                            if (TargetPop.SpeciesName == Species.SpeciesName)
                                            {
                                                TargetPop.PopulationCount += Migrants;
                                                bFound = true;
                                                break;
                                            }
                                        }
                                        if (!bFound)
                                        {
                                            FSpeciesPopulation NewPop = Species;
                                            NewPop.PopulationCount = Migrants;
                                            TargetSpecies->Add(NewPop);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void UMingRTSEcosystemSimulation::UpdateClimateEffects(float DeltaTime)
{
    for (auto& Pair : EcosystemZones)
    {
        FEcosystemZoneData& Zone = Pair.Value;

        // 溫度對資源的影響
        if (Zone.Climate.Temperature > 30.0f)
        {
            // 高溫增加水消耗
            if (FEcosystemResource* Water = Zone.Resources.Find(EResourceType::Water))
            {
                Water->CurrentAmount = FMath::Max(0.0f, Water->CurrentAmount - 2.0f * DeltaTime);
            }
        }
        else if (Zone.Climate.Temperature < 0.0f)
        {
            // 低溫減少食物再生
            if (FEcosystemResource* Food = Zone.Resources.Find(EResourceType::Food))
            {
                Food->RegenerationRate *= 0.5f;
            }
        }
    }
}

float UMingRTSEcosystemSimulation::GetSeasonalTemperatureMultiplier(ESeasonType Season) const
{
    switch (Season)
    {
    case ESeasonType::Spring:
        return 1.2f;
    case ESeasonType::Summer:
        return 1.5f;
    case ESeasonType::Autumn:
        return 1.0f;
    case ESeasonType::Winter:
        return 0.5f;
    default:
        return 1.0f;
    }
}

float UMingRTSEcosystemSimulation::GetWeatherResourceMultiplier(EWeatherType Weather, EResourceType Resource) const
{
    switch (Weather)
    {
    case EWeatherType::Rain:
        if (Resource == EResourceType::Water) return 2.0f;
        if (Resource == EResourceType::Food) return 1.3f;
        break;
    case EWeatherType::Drought:
        if (Resource == EResourceType::Water) return 0.2f;
        if (Resource == EResourceType::Food) return 0.5f;
        break;
    case EWeatherType::Snow:
        if (Resource == EResourceType::Water) return 0.5f;
        break;
    case EWeatherType::Clear:
        if (Resource == EResourceType::Food) return 1.2f;
        break;
    default:
        break;
    }
    return 1.0f;
}
