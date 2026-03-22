#include "Ecosystem/MingEcosystemEnvironmentSystem.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/Engine.h"

UMingEcosystemEnvironmentSystem::UMingEcosystemEnvironmentSystem()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
    , CurrentSeason(ESeasonType::Spring)
    , SeasonProgress(0.0f)
    , DaysPerSeason(30.0f)
    , CurrentWeather(EWeatherType::Clear)
    , WeatherChangeTimer(0.0f)
    , WeatherChangeInterval(300.0f)
    , CurrentHour(12.0f)
    , HoursPerDay(24.0f)
    , CurrentDayPhase(EDayNightCycle::Noon)
    , CurrentClimateZone(EClimateZone::Temperate)
    , TargetWeather(EWeatherType::Clear)
    , WeatherTransitionProgress(1.0f)
    , WeatherTransitionDuration(60.0f)
{
}

void UMingEcosystemEnvironmentSystem::InitializeEnvironmentSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    InitializeSeasonDatabase();
    InitializeClimateDatabase();
    
    CurrentGameTime = 0.0f;
    CurrentSeason = ESeasonType::Spring;
    SeasonProgress = 0.0f;
    CurrentHour = 6.0f;
    CurrentDayPhase = EDayNightCycle::Dawn;
    CurrentWeather = EWeatherType::Clear;
    
    CalculateWeatherParameters();
    UpdateDayNightCycle();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Initialized"));
}

void UMingEcosystemEnvironmentSystem::ShutdownEnvironmentSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    ActiveEvents.Empty();
    SeasonDatabase.Empty();
    ClimateDatabase.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Shutdown"));
}

void UMingEcosystemEnvironmentSystem::InitializeSeasonDatabase()
{
    // Spring
    FSeasonData SpringData;
    SpringData.Season = ESeasonType::Spring;
    SpringData.BaseTemperature = 15.0f;
    SpringData.TemperatureVariation = 8.0f;
    SpringData.AverageHumidity = 65.0f;
    SpringData.PrecipitationProbability = 0.4f;
    SpringData.DayLength = 13.0f;
    SpringData.GrowthRateMultiplier = 1.3f;
    SpringData.ResourceRegenerationRate = 1.2f;
    SpringData.AmbientLightColor = FLinearColor(0.9f, 1.0f, 0.8f, 1.0f);
    SpringData.SunIntensity = 0.9f;
    SeasonDatabase.Add(ESeasonType::Spring, SpringData);
    
    // Summer
    FSeasonData SummerData;
    SummerData.Season = ESeasonType::Summer;
    SummerData.BaseTemperature = 28.0f;
    SummerData.TemperatureVariation = 6.0f;
    SummerData.AverageHumidity = 70.0f;
    SummerData.PrecipitationProbability = 0.5f;
    SummerData.DayLength = 15.0f;
    SummerData.GrowthRateMultiplier = 1.5f;
    SummerData.ResourceRegenerationRate = 1.4f;
    SummerData.AmbientLightColor = FLinearColor(1.0f, 1.0f, 0.9f, 1.0f);
    SummerData.SunIntensity = 1.2f;
    SeasonDatabase.Add(ESeasonType::Summer, SummerData);
    
    // Autumn
    FSeasonData AutumnData;
    AutumnData.Season = ESeasonType::Autumn;
    AutumnData.BaseTemperature = 18.0f;
    AutumnData.TemperatureVariation = 7.0f;
    AutumnData.AverageHumidity = 55.0f;
    AutumnData.PrecipitationProbability = 0.3f;
    AutumnData.DayLength = 11.0f;
    AutumnData.GrowthRateMultiplier = 0.8f;
    AutumnData.ResourceRegenerationRate = 0.9f;
    AutumnData.AmbientLightColor = FLinearColor(1.0f, 0.9f, 0.7f, 1.0f);
    AutumnData.SunIntensity = 0.8f;
    SeasonDatabase.Add(ESeasonType::Autumn, AutumnData);
    
    // Winter
    FSeasonData WinterData;
    WinterData.Season = ESeasonType::Winter;
    WinterData.BaseTemperature = 2.0f;
    WinterData.TemperatureVariation = 10.0f;
    WinterData.AverageHumidity = 45.0f;
    WinterData.PrecipitationProbability = 0.2f;
    WinterData.DayLength = 9.0f;
    WinterData.GrowthRateMultiplier = 0.3f;
    WinterData.ResourceRegenerationRate = 0.5f;
    WinterData.AmbientLightColor = FLinearColor(0.8f, 0.85f, 0.95f, 1.0f);
    WinterData.SunIntensity = 0.6f;
    SeasonDatabase.Add(ESeasonType::Winter, WinterData);
}

void UMingEcosystemEnvironmentSystem::InitializeClimateDatabase()
{
    // Tropical
    FClimateZoneData TropicalData;
    TropicalData.ZoneType = EClimateZone::Tropical;
    TropicalData.MinTemperature = 20.0f;
    TropicalData.MaxTemperature = 35.0f;
    TropicalData.AverageHumidity = 80.0f;
    TropicalData.AvailableSeasons = {ESeasonType::Spring, ESeasonType::Summer};
    TropicalData.CommonWeatherTypes = {EWeatherType::Clear, EWeatherType::Cloudy, EWeatherType::HeavyRain, EWeatherType::Storm};
    TropicalData.Rainfall = 2000.0f;
    TropicalData.NaturalDisasterProbability = 0.15f;
    ClimateDatabase.Add(EClimateZone::Tropical, TropicalData);
    
    // Temperate
    FClimateZoneData TemperateData;
    TemperateData.ZoneType = EClimateZone::Temperate;
    TemperateData.MinTemperature = -10.0f;
    TemperateData.MaxTemperature = 35.0f;
    TemperateData.AverageHumidity = 60.0f;
    TemperateData.AvailableSeasons = {ESeasonType::Spring, ESeasonType::Summer, ESeasonType::Autumn, ESeasonType::Winter};
    TemperateData.CommonWeatherTypes = {EWeatherType::Clear, EWeatherType::Cloudy, EWeatherType::LightRain, EWeatherType::HeavyRain, EWeatherType::Snow};
    TemperateData.Rainfall = 800.0f;
    TemperateData.NaturalDisasterProbability = 0.1f;
    ClimateDatabase.Add(EClimateZone::Temperate, TemperateData);
    
    // Arid
    FClimateZoneData AridData;
    AridData.ZoneType = EClimateZone::Arid;
    AridData.MinTemperature = 5.0f;
    AridData.MaxTemperature = 45.0f;
    AridData.AverageHumidity = 20.0f;
    AridData.AvailableSeasons = {ESeasonType::Summer, ESeasonType::Autumn};
    AridData.CommonWeatherTypes = {EWeatherType::Clear, EWeatherType::Cloudy, EWeatherType::Sandstorm};
    AridData.Rainfall = 100.0f;
    AridData.NaturalDisasterProbability = 0.2f;
    ClimateDatabase.Add(EClimateZone::Arid, AridData);
    
    // Frigid
    FClimateZoneData FrigidData;
    FrigidData.ZoneType = EClimateZone::Frigid;
    FrigidData.MinTemperature = -40.0f;
    FrigidData.MaxTemperature = 10.0f;
    FrigidData.AverageHumidity = 40.0f;
    FrigidData.AvailableSeasons = {ESeasonType::Winter};
    FrigidData.CommonWeatherTypes = {EWeatherType::Clear, EWeatherType::Cloudy, EWeatherType::Snow, EWeatherType::Blizzard};
    FrigidData.Rainfall = 200.0f;
    FrigidData.NaturalDisasterProbability = 0.12f;
    ClimateDatabase.Add(EClimateZone::Frigid, FrigidData);
}

void UMingEcosystemEnvironmentSystem::SetCurrentSeason(ESeasonType NewSeason)
{
    if (CurrentSeason != NewSeason)
    {
        ESeasonType OldSeason = CurrentSeason;
        CurrentSeason = NewSeason;
        SeasonProgress = 0.0f;
        
        OnSeasonChanged.Broadcast(NewSeason, OldSeason);
        
        UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Season changed from %d to %d"), 
            static_cast<int32>(OldSeason), static_cast<int32>(NewSeason));
    }
}

void UMingEcosystemEnvironmentSystem::AdvanceSeason()
{
    ESeasonType NextSeason;
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
        NextSeason = ESeasonType::Spring;
        break;
    }
    
    SetCurrentSeason(NextSeason);
}

FSeasonData UMingEcosystemEnvironmentSystem::GetSeasonData(ESeasonType Season) const
{
    if (SeasonDatabase.Contains(Season))
    {
        return SeasonDatabase[Season];
    }
    return FSeasonData();
}

void UMingEcosystemEnvironmentSystem::SetWeather(EWeatherType NewWeather, float Intensity)
{
    TargetWeather = NewWeather;
    WeatherTransitionProgress = 0.0f;
    
    // TODO: Implement smooth weather transition with interpolation
    // between current and target weather parameters
    
    CurrentWeather = NewWeather;
    CalculateWeatherParameters();
    
    OnWeatherChanged.Broadcast(NewWeather, CurrentWeatherParams);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Weather changed to %d with intensity %.2f"), 
        static_cast<int32>(NewWeather), Intensity);
}

void UMingEcosystemEnvironmentSystem::UpdateWeather(float DeltaTime)
{
    WeatherChangeTimer += DeltaTime;
    
    if (WeatherChangeTimer >= WeatherChangeInterval)
    {
        WeatherChangeTimer = 0.0f;
        GenerateRandomWeather();
    }
    
    // Update weather transition
    if (WeatherTransitionProgress < 1.0f)
    {
        WeatherTransitionProgress += DeltaTime / WeatherTransitionDuration;
        WeatherTransitionProgress = FMath::Clamp(WeatherTransitionProgress, 0.0f, 1.0f);
    }
}

void UMingEcosystemEnvironmentSystem::GenerateRandomWeather()
{
    const FClimateZoneData& ClimateData = GetClimateZoneData();
    
    // Select from common weather types for this climate
    if (ClimateData.CommonWeatherTypes.Num() > 0)
    {
        int32 RandomIndex = FMath::RandRange(0, ClimateData.CommonWeatherTypes.Num() - 1);
        EWeatherType NewWeather = ClimateData.CommonWeatherTypes[RandomIndex];
        
        // Apply season influence
        FSeasonData SeasonData = GetSeasonData(CurrentSeason);
        
        // Adjust probability based on season precipitation probability
        float RandomValue = FMath::FRand();
        if (RandomValue > SeasonData.PrecipitationProbability)
        {
            // Prefer clear weather if no precipitation
            if (FMath::FRand() < 0.7f)
            {
                NewWeather = EWeatherType::Clear;
            }
        }
        
        if (NewWeather != CurrentWeather)
        {
            SetWeather(NewWeather, FMath::FRand() * 0.5f + 0.5f);
        }
    }
}

void UMingEcosystemEnvironmentSystem::CalculateWeatherParameters()
{
    FSeasonData SeasonData = GetSeasonData(CurrentSeason);
    
    // Base temperature from season with some random variation
    float BaseTemp = SeasonData.BaseTemperature;
    float TempVariation = (FMath::FRand() - 0.5f) * 2.0f * SeasonData.TemperatureVariation;
    
    CurrentWeatherParams.Temperature = BaseTemp + TempVariation;
    CurrentWeatherParams.Humidity = SeasonData.AverageHumidity + (FMath::FRand() - 0.5f) * 20.0f;
    CurrentWeatherParams.Humidity = FMath::Clamp(CurrentWeatherParams.Humidity, 0.0f, 100.0f);
    
    // Weather type specific parameters
    switch (CurrentWeather)
    {
    case EWeatherType::Clear:
        CurrentWeatherParams.CloudCover = FMath::FRand() * 0.2f;
        CurrentWeatherParams.WindSpeed = FMath::FRand() * 10.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.0f;
        CurrentWeatherParams.Visibility = 10.0f;
        break;
        
    case EWeatherType::Cloudy:
        CurrentWeatherParams.CloudCover = 0.5f + FMath::FRand() * 0.4f;
        CurrentWeatherParams.WindSpeed = FMath::FRand() * 15.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.0f;
        CurrentWeatherParams.Visibility = 8.0f;
        break;
        
    case EWeatherType::LightRain:
        CurrentWeatherParams.CloudCover = 0.7f + FMath::FRand() * 0.3f;
        CurrentWeatherParams.WindSpeed = 5.0f + FMath::FRand() * 10.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.2f + FMath::FRand() * 0.3f;
        CurrentWeatherParams.Visibility = 6.0f;
        CurrentWeatherParams.Temperature -= 2.0f;
        break;
        
    case EWeatherType::HeavyRain:
        CurrentWeatherParams.CloudCover = 0.9f + FMath::FRand() * 0.1f;
        CurrentWeatherParams.WindSpeed = 10.0f + FMath::FRand() * 20.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.6f + FMath::FRand() * 0.4f;
        CurrentWeatherParams.Visibility = 3.0f;
        CurrentWeatherParams.Temperature -= 5.0f;
        break;
        
    case EWeatherType::Storm:
        CurrentWeatherParams.CloudCover = 1.0f;
        CurrentWeatherParams.WindSpeed = 30.0f + FMath::FRand() * 40.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.8f + FMath::FRand() * 0.2f;
        CurrentWeatherParams.StormIntensity = 0.5f + FMath::FRand() * 0.5f;
        CurrentWeatherParams.Visibility = 1.0f;
        CurrentWeatherParams.Temperature -= 8.0f;
        break;
        
    case EWeatherType::Snow:
        CurrentWeatherParams.CloudCover = 0.8f + FMath::FRand() * 0.2f;
        CurrentWeatherParams.WindSpeed = 5.0f + FMath::FRand() * 15.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.3f + FMath::FRand() * 0.4f;
        CurrentWeatherParams.Visibility = 4.0f;
        CurrentWeatherParams.Temperature = FMath::Min(CurrentWeatherParams.Temperature, 0.0f);
        break;
        
    case EWeatherType::Blizzard:
        CurrentWeatherParams.CloudCover = 1.0f;
        CurrentWeatherParams.WindSpeed = 40.0f + FMath::FRand() * 30.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.7f + FMath::FRand() * 0.3f;
        CurrentWeatherParams.Visibility = 0.5f;
        CurrentWeatherParams.Temperature = FMath::Min(CurrentWeatherParams.Temperature, -10.0f);
        break;
        
    default:
        CurrentWeatherParams.CloudCover = FMath::FRand() * 0.5f;
        CurrentWeatherParams.WindSpeed = FMath::FRand() * 20.0f;
        CurrentWeatherParams.PrecipitationIntensity = 0.0f;
        CurrentWeatherParams.Visibility = 7.0f;
        break;
    }
    
    CurrentWeatherParams.WeatherType = CurrentWeather;
    CurrentWeatherParams.WindDirection = FMath::FRand() * 360.0f;
}

void UMingEcosystemEnvironmentSystem::SetDayTime(float Hour)
{
    CurrentHour = FMath::Fmod(Hour, HoursPerDay);
    if (CurrentHour < 0.0f)
    {
        CurrentHour += HoursPerDay;
    }
    
    UpdateDayNightCycle();
}

void UMingEcosystemEnvironmentSystem::AdvanceTime(float Hours)
{
    float NewHour = CurrentHour + Hours;
    
    // Check if day has advanced
    if (NewHour >= HoursPerDay)
    {
        SeasonProgress += NewHour / HoursPerDay / DaysPerSeason;
        
        if (SeasonProgress >= 1.0f)
        {
            AdvanceSeason();
        }
    }
    
    SetDayTime(NewHour);
}

EDayNightCycle UMingEcosystemEnvironmentSystem::GetCurrentDayPhase() const
{
    return CurrentDayPhase;
}

FDayNightData UMingEcosystemEnvironmentSystem::GetDayNightData() const
{
    FDayNightData Data;
    Data.CurrentPhase = CurrentDayPhase;
    Data.DayProgress = CurrentHour / HoursPerDay;
    Data.SunElevation = CalculateSunIntensity();
    Data.SunAzimuth = CurrentHour / HoursPerDay * 360.0f;
    Data.SkyColor = CalculateSkyColor();
    
    // Calculate light intensity based on time of day
    float NoonDistance = FMath::Abs(CurrentHour - 12.0f) / 12.0f;
    Data.LightIntensity = 1.0f - FMath::Clamp(NoonDistance, 0.0f, 1.0f);
    
    // Adjust for season
    FSeasonData SeasonData = GetSeasonData(CurrentSeason);
    Data.LightIntensity *= SeasonData.SunIntensity;
    
    Data.AmbientColor = SeasonData.AmbientLightColor * Data.LightIntensity;
    
    return Data;
}

void UMingEcosystemEnvironmentSystem::UpdateDayNightCycle()
{
    EDayNightCycle NewPhase;
    
    if (CurrentHour >= 5.0f && CurrentHour < 7.0f)
    {
        NewPhase = EDayNightCycle::Dawn;
    }
    else if (CurrentHour >= 7.0f && CurrentHour < 11.0f)
    {
        NewPhase = EDayNightCycle::Morning;
    }
    else if (CurrentHour >= 11.0f && CurrentHour < 13.0f)
    {
        NewPhase = EDayNightCycle::Noon;
    }
    else if (CurrentHour >= 13.0f && CurrentHour < 17.0f)
    {
        NewPhase = EDayNightCycle::Afternoon;
    }
    else if (CurrentHour >= 17.0f && CurrentHour < 19.0f)
    {
        NewPhase = EDayNightCycle::Dusk;
    }
    else if (CurrentHour >= 19.0f && CurrentHour < 22.0f)
    {
        NewPhase = EDayNightCycle::Evening;
    }
    else if (CurrentHour >= 22.0f || CurrentHour < 1.0f)
    {
        NewPhase = EDayNightCycle::Night;
    }
    else
    {
        NewPhase = EDayNightCycle::Midnight;
    }
    
    if (NewPhase != CurrentDayPhase)
    {
        CurrentDayPhase = NewPhase;
        OnDayNightCycleChanged.Broadcast(CurrentDayPhase);
    }
}

void UMingEcosystemEnvironmentSystem::SetClimateZone(EClimateZone Zone)
{
    CurrentClimateZone = Zone;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Climate zone changed to %d"), 
        static_cast<int32>(Zone));
}

FClimateZoneData UMingEcosystemEnvironmentSystem::GetClimateZoneData() const
{
    if (ClimateDatabase.Contains(CurrentClimateZone))
    {
        return ClimateDatabase[CurrentClimateZone];
    }
    return FClimateZoneData();
}

float UMingEcosystemEnvironmentSystem::GetTemperatureEffectOnGrowth() const
{
    float OptimalTemp = 20.0f;
    float CurrentTemp = CurrentWeatherParams.Temperature;
    float TempDiff = FMath::Abs(CurrentTemp - OptimalTemp);
    
    // Growth rate decreases as temperature moves away from optimal
    float Effect = 1.0f - FMath::Clamp(TempDiff / 30.0f, 0.0f, 1.0f);
    
    // Apply season multiplier
    FSeasonData SeasonData = GetSeasonData(CurrentSeason);
    Effect *= SeasonData.GrowthRateMultiplier;
    
    return Effect;
}

float UMingEcosystemEnvironmentSystem::GetWeatherEffectOnVisibility() const
{
    return CurrentWeatherParams.Visibility / 10.0f;
}

float UMingEcosystemEnvironmentSystem::GetWeatherEffectOnMovement() const
{
    float Effect = 1.0f;
    
    // Movement is reduced in bad weather
    switch (CurrentWeather)
    {
    case EWeatherType::Clear:
        Effect = 1.0f;
        break;
    case EWeatherType::Cloudy:
        Effect = 0.95f;
        break;
    case EWeatherType::LightRain:
        Effect = 0.85f;
        break;
    case EWeatherType::HeavyRain:
        Effect = 0.7f;
        break;
    case EWeatherType::Storm:
        Effect = 0.5f;
        break;
    case EWeatherType::Snow:
        Effect = 0.6f;
        break;
    case EWeatherType::Blizzard:
        Effect = 0.3f;
        break;
    case EWeatherType::Fog:
        Effect = 0.75f;
        break;
    default:
        Effect = 0.8f;
        break;
    }
    
    return Effect;
}

void UMingEcosystemEnvironmentSystem::TriggerEnvironmentEvent(const FString& EventType, const FVector& Location, float Intensity)
{
    FEnvironmentEvent NewEvent;
    NewEvent.EventID = FGuid::NewGuid().ToString();
    NewEvent.EventType = EventType;
    NewEvent.Timestamp = CurrentGameTime;
    NewEvent.Location = Location;
    NewEvent.Intensity = Intensity;
    NewEvent.Duration = 300.0f; // Default 5 minutes
    
    ActiveEvents.Add(NewEvent);
    
    OnEnvironmentEvent.Broadcast(NewEvent);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemEnvironmentSystem: Environment event triggered - %s at %s"), 
        *EventType, *Location.ToString());
}

void UMingEcosystemEnvironmentSystem::ProcessEnvironmentEvents(float DeltaTime)
{
    // Remove expired events
    for (int32 i = ActiveEvents.Num() - 1; i >= 0; --i)
    {
        float EventAge = CurrentGameTime - ActiveEvents[i].Timestamp;
        if (EventAge >= ActiveEvents[i].Duration)
        {
            ActiveEvents.RemoveAt(i);
        }
    }
}

void UMingEcosystemEnvironmentSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    // Update day/night cycle (assuming 1 real second = 1 game minute)
    AdvanceTime(DeltaTime / 60.0f);
    
    // Update weather
    UpdateWeather(DeltaTime);
    
    // Process active events
    ProcessEnvironmentEvents(DeltaTime);
}

FLinearColor UMingEcosystemEnvironmentSystem::CalculateSkyColor() const
{
    FLinearColor BaseSkyColor(0.5f, 0.7f, 1.0f, 1.0f);
    
    // Adjust based on time of day
    float DayBrightness = 1.0f;
    if (CurrentHour < 6.0f || CurrentHour > 18.0f)
    {
        DayBrightness = 0.2f;
    }
    else if (CurrentHour < 8.0f)
    {
        DayBrightness = 0.5f + (CurrentHour - 6.0f) * 0.25f;
    }
    else if (CurrentHour > 16.0f)
    {
        DayBrightness = 1.0f - (CurrentHour - 16.0f) * 0.25f;
    }
    
    // Adjust for weather
    float CloudFactor = 1.0f - (CurrentWeatherParams.CloudCover * 0.5f);
    
    return BaseSkyColor * DayBrightness * CloudFactor;
}

float UMingEcosystemEnvironmentSystem::CalculateSunIntensity() const
{
    // Sun intensity peaks at noon
    float NoonDistance = FMath::Abs(CurrentHour - 12.0f);
    float Intensity = 1.0f - FMath::Clamp(NoonDistance / 12.0f, 0.0f, 1.0f);
    
    // Season adjustment
    FSeasonData SeasonData = GetSeasonData(CurrentSeason);
    Intensity *= SeasonData.SunIntensity;
    
    // Weather adjustment
    Intensity *= (1.0f - CurrentWeatherParams.CloudCover * 0.7f);
    
    return Intensity;
}
