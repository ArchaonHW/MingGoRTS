#include "Innovation/MingEcologicalEnvironmentSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Math/UnrealMathUtility.h"
#include "Misc/DateTime.h"

UMingEcologicalEnvironmentSystem::UMingEcologicalEnvironmentSystem()
{
    SimulationSpeed = 1.0f;
    SeasonDuration = 90.0f; // 90 days per season
    bEnableWeatherSystem = true;
    bEnableClimateChange = true;
    bEnableResourceRegeneration = true;
    bEnableEcosystemBalance = true;
    CurrentSeason = ESeason::Spring;
    CurrentWeather = EWeatherType::Sunny;
    Temperature = 20.0f;
    Humidity = 50.0f;
    Precipitation = 0.0f;
    WindSpeed = 5.0f;
}

void UMingEcologicalEnvironmentSystem::InitializeEnvironmentSystem()
{
    // Initialize system state
    bSystemInitialized = true;
    LastUpdateTime = FDateTime::Now();
    
    // Clear existing data
    Ecosystems.Empty();
    EnvironmentalZones.Empty();
    Resources.Empty();
    ClimateData.Empty();
    
    // Create default ecosystem
    CreateDefaultEcosystem();
    
    // Initialize climate system
    InitializeClimateSystem();
    
    UE_LOG(LogTemp, Log, TEXT("Ecological Environment System initialized"));
}

void UMingEcologicalEnvironmentSystem::ShutdownEnvironmentSystem()
{
    bSystemInitialized = false;
    
    // Clear all data
    Ecosystems.Empty();
    EnvironmentalZones.Empty();
    Resources.Empty();
    ClimateData.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Ecological Environment System shutdown"));
}

void UMingEcologicalEnvironmentSystem::CreateEcosystem(const FEcosystem& Ecosystem)
{
    // Validate ecosystem
    FEcosystem ValidatedEcosystem = Ecosystem;
    ValidateEcosystem(ValidatedEcosystem);
    
    // Add ecosystem with unique ID
    if (ValidatedEcosystem.EcosystemID.IsEmpty())
    {
        ValidatedEcosystem.EcosystemID = FString::Printf(TEXT("Ecosystem_%d"), Ecosystems.Num());
    }
    
    Ecosystems.Add(ValidatedEcosystem.EcosystemID, ValidatedEcosystem);
    
    // Create environmental zones for this ecosystem
    CreateEnvironmentalZones(ValidatedEcosystem);
    
    UE_LOG(LogTemp, Log, TEXT("Created ecosystem: %s"), *ValidatedEcosystem.EcosystemID);
}

void UMingEcologicalEnvironmentSystem::RemoveEcosystem(const FString& EcosystemID)
{
    if (Ecosystems.Contains(EcosystemID))
    {
        // Remove associated environmental zones
        EnvironmentalZones.RemoveAll([&](const FEnvironmentalZone& Zone) {
            return Zone.EcosystemID == EcosystemID;
        });
        
        // Remove ecosystem
        Ecosystems.Remove(EcosystemID);
        
        UE_LOG(LogTemp, Log, TEXT("Removed ecosystem: %s"), *EcosystemID);
    }
}

FEcosystem UMingEcologicalEnvironmentSystem::GetEcosystem(const FString& EcosystemID) const
{
    if (Ecosystems.Contains(EcosystemID))
    {
        return Ecosystems[EcosystemID];
    }
    
    return FEcosystem();
}

TArray<FEcosystem> UMingEcologicalEnvironmentSystem::GetAllEcosystems() const
{
    TArray<FEcosystem> AllEcosystems;
    
    for (const auto& EcosystemPair : Ecosystems)
    {
        AllEcosystems.Add(EcosystemPair.Value);
    }
    
    return AllEcosystems;
}

void UMingEcologicalEnvironmentSystem::CreateEnvironmentalZones(const FEcosystem& Ecosystem)
{
    // Create zones based on ecosystem type
    TArray<EZoneType> ZoneTypes = GetZoneTypesForEcosystem(Ecosystem.Type);
    
    for (EZoneType ZoneType : ZoneTypes)
    {
        FEnvironmentalZone Zone;
        Zone.ZoneID = FString::Printf(TEXT("%s_Zone_%d"), *Ecosystem.EcosystemID, EnvironmentalZones.Num());
        Zone.EcosystemID = Ecosystem.EcosystemID;
        Zone.Type = ZoneType;
        Zone.Size = FMath::RandRange(100, 1000); // Random size in square kilometers
        Zone.Temperature = CalculateZoneTemperature(ZoneType);
        Zone.Humidity = CalculateZoneHumidity(ZoneType);
        Zone.Fertility = CalculateZoneFertility(ZoneType);
        Zone.Biodiversity = CalculateZoneBiodiversity(ZoneType);
        Zone.ResourceDensity = CalculateZoneResourceDensity(ZoneType);
        
        EnvironmentalZones.Add(Zone);
    }
}

void UMingEcologicalEnvironmentSystem::UpdateEnvironmentalConditions(float DeltaTime)
{
    if (!bSystemInitialized)
    {
        return;
    }
    
    // Update weather
    UpdateWeather(DeltaTime);
    
    // Update season
    UpdateSeason(DeltaTime);
    
    // Update climate
    if (bEnableClimateChange)
    {
        UpdateClimate(DeltaTime);
    }
    
    // Update ecosystem conditions
    UpdateEcosystemConditions(DeltaTime);
    
    // Update resource regeneration
    if (bEnableResourceRegeneration)
    {
        UpdateResourceRegeneration(DeltaTime);
    }
    
    // Update ecosystem balance
    if (bEnableEcosystemBalance)
    {
        UpdateEcosystemBalance(DeltaTime);
    }
    
    LastUpdateTime = FDateTime::Now();
}

void UMingEcologicalEnvironmentSystem::SetWeather(EWeatherType NewWeather)
{
    CurrentWeather = NewWeather;
    
    // Update environmental parameters based on weather
    switch (NewWeather)
    {
        case EWeatherType::Sunny:
            Temperature += 2.0f;
            Humidity -= 10.0f;
            Precipitation = 0.0f;
            break;
        case EWeatherType::Cloudy:
            Temperature -= 1.0f;
            Humidity += 5.0f;
            Precipitation = 0.0f;
            break;
        case EWeatherType::Rainy:
            Temperature -= 3.0f;
            Humidity += 20.0f;
            Precipitation = FMath::RandRange(5.0f, 25.0f);
            break;
        case EWeatherType::Stormy:
            Temperature -= 5.0f;
            Humidity += 15.0f;
            Precipitation = FMath::RandRange(20.0f, 50.0f);
            WindSpeed += 15.0f;
            break;
        case EWeatherType::Snowy:
            Temperature -= 8.0f;
            Humidity += 10.0f;
            Precipitation = FMath::RandRange(2.0f, 15.0f);
            break;
        case EWeatherType::Foggy:
            Temperature -= 2.0f;
            Humidity += 25.0f;
            Precipitation = 0.0f;
            WindSpeed -= 5.0f;
            break;
    }
    
    // Clamp values
    Temperature = FMath::Clamp(Temperature, -30.0f, 50.0f);
    Humidity = FMath::Clamp(Humidity, 0.0f, 100.0f);
    WindSpeed = FMath::Clamp(WindSpeed, 0.0f, 100.0f);
    
    // Broadcast weather change
    OnWeatherChanged.Broadcast(CurrentWeather);
    
    UE_LOG(LogTemp, Log, TEXT("Weather changed to: %s"), *UEnum::GetValueAsString(NewWeather));
}

void UMingEcologicalEnvironmentSystem::SetSeason(ESeason NewSeason)
{
    CurrentSeason = NewSeason;
    
    // Update environmental parameters based on season
    switch (NewSeason)
    {
        case ESeason::Spring:
            Temperature = 15.0f;
            Humidity = 60.0f;
            Precipitation = 10.0f;
            break;
        case ESeason::Summer:
            Temperature = 28.0f;
            Humidity = 40.0f;
            Precipitation = 5.0f;
            break;
        case ESeason::Autumn:
            Temperature = 12.0f;
            Humidity = 65.0f;
            Precipitation = 15.0f;
            break;
        case ESeason::Winter:
            Temperature = -2.0f;
            Humidity = 50.0f;
            Precipitation = 8.0f;
            break;
    }
    
    // Broadcast season change
    OnSeasonChanged.Broadcast(CurrentSeason);
    
    UE_LOG(LogTemp, Log, TEXT("Season changed to: %s"), *UEnum::GetValueAsString(NewSeason));
}

void UMingEcologicalEnvironmentSystem::AddEnvironmentalResource(const FEnvironmentalResource& Resource)
{
    // Validate resource
    FEnvironmentalResource ValidatedResource = Resource;
    ValidateResource(ValidatedResource);
    
    // Add resource with unique ID
    if (ValidatedResource.ResourceID.IsEmpty())
    {
        ValidatedResource.ResourceID = FString::Printf(TEXT("Resource_%d"), Resources.Num());
    }
    
    Resources.Add(ValidatedResource.ResourceID, ValidatedResource);
    
    UE_LOG(LogTemp, Log, TEXT("Added environmental resource: %s"), *ValidatedResource.ResourceID);
}

void UMingEcologicalEnvironmentSystem::RemoveResource(const FString& ResourceID)
{
    if (Resources.Contains(ResourceID))
    {
        Resources.Remove(ResourceID);
        UE_LOG(LogTemp, Log, TEXT("Removed environmental resource: %s"), *ResourceID);
    }
}

FEnvironmentalResource UMingEcologicalEnvironmentSystem::GetResource(const FString& ResourceID) const
{
    if (Resources.Contains(ResourceID))
    {
        return Resources[ResourceID];
    }
    
    return FEnvironmentalResource();
}

TArray<FEnvironmentalResource> UMingEcologicalEnvironmentSystem::GetResourcesInZone(const FString& ZoneID) const
{
    TArray<FEnvironmentalResource> ZoneResources;
    
    for (const auto& ResourcePair : Resources)
    {
        const FEnvironmentalResource& Resource = ResourcePair.Value;
        
        if (Resource.ZoneID == ZoneID)
        {
            ZoneResources.Add(Resource);
        }
    }
    
    return ZoneResources;
}

void UMingEcologicalEnvironmentSystem::SimulateClimateChange(float DeltaTime)
{
    if (!bEnableClimateChange)
    {
        return;
    }
    
    // Simulate gradual climate change
    float ClimateChangeRate = 0.001f; // Very slow change
    
    // Global warming effect
    Temperature += ClimateChangeRate * DeltaTime;
    
    // Increased extreme weather events
    if (FMath::FRand() < 0.001f) // Small chance of extreme weather
    {
        EWeatherType ExtremeWeather = GetExtremeWeatherType();
        SetWeather(ExtremeWeather);
    }
    
    // Update climate data
    UpdateClimateData();
    
    UE_LOG(LogTemp, VeryVerbose, TEXT("Climate change simulation: Temperature = %.2f"), Temperature);
}

void UMingEcologicalEnvironmentSystem::ProcessEnvironmentalEvent(const FEnvironmentalEvent& Event)
{
    // Apply event effects
    for (const auto& EffectPair : Event.ZoneEffects)
    {
        const FString& ZoneID = EffectPair.Key;
        const FEnvironmentalEffect& Effect = EffectPair.Value;
        
        ApplyEnvironmentalEffect(ZoneID, Effect);
    }
    
    // Apply resource effects
    for (const auto& ResourceEffect : Event.ResourceEffects)
    {
        if (Resources.Contains(ResourceEffect.ResourceID))
        {
            FEnvironmentalResource& Resource = Resources[ResourceEffect.ResourceID];
            Resource.Quantity *= ResourceEffect.QuantityMultiplier;
            Resource.Quality *= ResourceEffect.QualityMultiplier;
            
            // Clamp values
            Resource.Quantity = FMath::Max(0.0f, Resource.Quantity);
            Resource.Quality = FMath::Clamp(Resource.Quality, 0.0f, 1.0f);
        }
    }
    
    // Broadcast event
    OnEnvironmentalEventOccurred.Broadcast(Event);
    
    UE_LOG(LogTemp, Log, TEXT("Processed environmental event: %s"), *Event.EventID);
}

FEnvironmentalMetrics UMingEcologicalEnvironmentSystem::GetEnvironmentalMetrics() const
{
    FEnvironmentalMetrics Metrics;
    
    // Calculate overall metrics
    Metrics.TotalEcosystems = Ecosystems.Num();
    Metrics.TotalZones = EnvironmentalZones.Num();
    Metrics.TotalResources = Resources.Num();
    Metrics.AverageTemperature = CalculateAverageTemperature();
    Metrics.AverageHumidity = CalculateAverageHumidity();
    Metrics.TotalBiodiversity = CalculateTotalBiodiversity();
    Metrics.EcosystemHealth = CalculateEcosystemHealth();
    metrics.ClimateStability = CalculateClimateStability();
    metrics.ResourceSustainability = CalculateResourceSustainability();
    
    return Metrics;
}

void UMingEcologicalEnvironmentSystem::RestoreEcosystem(const FString& EcosystemID)
{
    if (Ecosystems.Contains(EcosystemID))
    {
        FEcosystem& Ecosystem = Ecosystems[EcosystemID];
        
        // Reset ecosystem health
        Ecosystem.Health = 1.0f;
        Ecosystem.Balance = 1.0f;
        
        // Restore resources in this ecosystem
        for (auto& ResourcePair : Resources)
        {
            FEnvironmentalResource& Resource = ResourcePair.Value;
            
            // Find zones belonging to this ecosystem
            for (const FEnvironmentalZone& Zone : EnvironmentalZones)
            {
                if (Zone.EcosystemID == EcosystemID && Resource.ZoneID == Zone.ZoneID)
                {
                    // Restore resource to original quantity
                    Resource.Quantity = Resource.OriginalQuantity;
                    Resource.Quality = 1.0f;
                    break;
                }
            }
        }
        
        UE_LOG(LogTemp, Log, TEXT("Restored ecosystem: %s"), *EcosystemID);
    }
}

void UMingEcologicalEnvironmentSystem::BalanceEcosystem(const FString& EcosystemID)
{
    if (Ecosystems.Contains(EcosystemID))
    {
        FEcosystem& Ecosystem = Ecosystems[EcosystemID];
        
        // Calculate current balance
        float CurrentBalance = CalculateEcosystemBalance(EcosystemID);
        
        // Apply balancing measures
        if (CurrentBalance < 0.5f)
        {
            // Ecosystem is unbalanced, apply corrective measures
            ApplyBalancingMeasures(EcosystemID);
        }
        
        Ecosystem.Balance = FMath::Clamp(CurrentBalance, 0.0f, 1.0f);
        
        UE_LOG(LogTemp, Log, TEXT("Balanced ecosystem: %s (Balance: %.2f)"), *EcosystemID, Ecosystem.Balance);
    }
}

// Private helper functions

void UMingEcologicalEnvironmentSystem::CreateDefaultEcosystem()
{
    FEcosystem DefaultEcosystem;
    DefaultEcosystem.EcosystemID = TEXT("DefaultEcosystem");
    DefaultEcosystem.Name = TEXT("Default Ecosystem");
    DefaultEcosystem.Type = EEcosystemType::Forest;
    DefaultEcosystem.Health = 1.0f;
    DefaultEcosystem.Balance = 1.0f;
    DefaultEcosystem.Biodiversity = 0.8f;
    DefaultEcosystem.Resilience = 0.7f;
    
    Ecosystems.Add(DefaultEcosystem.EcosystemID, DefaultEcosystem);
    
    // Create zones for default ecosystem
    CreateEnvironmentalZones(DefaultEcosystem);
}

void UMingEcologicalEnvironmentSystem::InitializeClimateSystem()
{
    // Initialize climate data
    FClimateData InitialData;
    InitialData.Timestamp = FDateTime::Now();
    InitialData.Temperature = Temperature;
    InitialData.Humidity = Humidity;
    InitialData.Precipitation = Precipitation;
    InitialData.WindSpeed = WindSpeed;
    InitialData.Season = CurrentSeason;
    InitialData.Weather = CurrentWeather;
    
    ClimateData.Add(InitialData);
}

void UMingEcologicalEnvironmentSystem::ValidateEcosystem(FEcosystem& Ecosystem)
{
    // Clamp values
    Ecosystem.Health = FMath::Clamp(Ecosystem.Health, 0.0f, 1.0f);
    Ecosystem.Balance = FMath::Clamp(Ecosystem.Balance, 0.0f, 1.0f);
    Ecosystem.Biodiversity = FMath::Clamp(Ecosystem.Biodiversity, 0.0f, 1.0f);
    Ecosystem.Resilience = FMath::Clamp(Ecosystem.Resilience, 0.0f, 1.0f);
}

void UMingEcologicalEnvironmentSystem::ValidateResource(FEnvironmentalResource& Resource)
{
    // Clamp values
    Resource.Quantity = FMath::Max(0.0f, Resource.Quantity);
    Resource.Quality = FMath::Clamp(Resource.Quality, 0.0f, 1.0f);
    Resource.RegenerationRate = FMath::Max(0.0f, Resource.RegenerationRate);
}

TArray<EZoneType> UMingEcologicalEnvironmentSystem::GetZoneTypesForEcosystem(EEcosystemType EcosystemType) const
{
    TArray<EZoneType> ZoneTypes;
    
    switch (EcosystemType)
    {
        case EEcosystemType::Forest:
            ZoneTypes = {EZoneType::Forest, EZoneType::Grassland, EZoneType::Wetland};
            break;
        case EEcosystemType::Desert:
            ZoneTypes = {EZoneType::Desert, EZoneType::Mountain};
            break;
        case EEcosystemType::Ocean:
            ZoneTypes = {EZoneType::Ocean, EZoneType::Coastal};
            break;
        case EEcosystemType::Grassland:
            ZoneTypes = {EZoneType::Grassland, EZoneType::Wetland};
            break;
        case EEcosystemType::Tundra:
            ZoneTypes = {EZoneType::Tundra, EZoneType::Mountain};
            break;
        case EEcosystemType::Urban:
            ZoneTypes = {EZoneType::Urban, EZoneType::Industrial};
            break;
        default:
            ZoneTypes = {EZoneType::Forest};
            break;
    }
    
    return ZoneTypes;
}

float UMingEcologicalEnvironmentSystem::CalculateZoneTemperature(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::Forest: return 18.0f;
        case EZoneType::Desert: return 35.0f;
        case EZoneType::Ocean: return 22.0f;
        case EZoneType::Grassland: return 20.0f;
        case EZoneType::Mountain: return 10.0f;
        case EZoneType::Wetland: return 25.0f;
        case EZoneType::Tundra: return -5.0f;
        case EZoneType::Urban: return 24.0f;
        case EZoneType::Industrial: return 26.0f;
        case EZoneType::Coastal: return 21.0f;
        default: return 20.0f;
    }
}

float UMingEcologicalEnvironmentSystem::CalculateZoneHumidity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::Forest: return 70.0f;
        case EZoneType::Desert: return 15.0f;
        case EZoneType::Ocean: return 85.0f;
        case EZoneType::Grassland: return 45.0f;
        case EZoneType::Mountain: return 55.0f;
        case EZoneType::Wetland: return 90.0f;
        case EZoneType::Tundra: return 40.0f;
        case EZoneType::Urban: return 50.0f;
        case EZoneType::Industrial: return 35.0f;
        case EZoneType::Coastal: return 75.0f;
        default: return 50.0f;
    }
}

float UMingEcologicalEnvironmentSystem::CalculateZoneFertility(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::Forest: return 0.8f;
        case EZoneType::Desert: return 0.1f;
        case EZoneType::Ocean: return 0.3f;
        case EZoneType::Grassland: return 0.7f;
        case EZoneType::Mountain: return 0.2f;
        case EZoneType::Wetland: return 0.9f;
        case EZoneType::Tundra: return 0.1f;
        case EZoneType::Urban: return 0.4f;
        case EZoneType::Industrial: return 0.2f;
        case EZoneType::Coastal: return 0.6f;
        default: return 0.5f;
    }
}

float UMingEcologicalEnvironmentSystem::CalculateZoneBiodiversity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::Forest: return 0.9f;
        case EZoneType::Desert: return 0.3f;
        case EZoneType::Ocean: return 0.8f;
        case EZoneType::Grassland: return 0.6f;
        case EZoneType::Mountain: return 0.5f;
        case EZoneType::Wetland: return 0.8f;
        case EZoneType::Tundra: return 0.2f;
        case EZoneType::Urban: return 0.1f;
        case EZoneType::Industrial: return 0.05f;
        case EZoneType::Coastal: return 0.7f;
        default: return 0.5f;
    }
}

float UMingEcologicalEnvironmentSystem::CalculateZoneResourceDensity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::Forest: return 0.7f;
        case EZoneType::Desert: return 0.2f;
        case EZoneType::Ocean: return 0.4f;
        case EZoneType::Grassland: return 0.6f;
        case EZoneType::Mountain: return 0.3f;
        case EZoneType::Wetland: return 0.5f;
        case EZoneType::Tundra: return 0.1f;
        case EZoneType::Urban: return 0.8f;
        case EZoneType::Industrial: return 0.9f;
        case EZoneType::Coastal: return 0.6f;
        default: return 0.5f;
    }
}

void UMingEcologicalEnvironmentSystem::UpdateWeather(float DeltaTime)
{
    if (!bEnableWeatherSystem)
    {
        return;
    }
    
    // Random weather changes
    if (FMath::FRand() < 0.01f) // 1% chance per tick
    {
        TArray<EWeatherType> PossibleWeather = {
            EWeatherType::Sunny, EWeatherType::Cloudy, EWeatherType::Rainy,
            EWeatherType::Stormy, EWeatherType::Snowy, EWeatherType::Foggy
        };
        
        EWeatherType NewWeather = PossibleWeather[FMath::RandRange(0, PossibleWeather.Num() - 1)];
        SetWeather(NewWeather);
    }
    
    // Gradual weather changes
    Temperature += FMath::RandRange(-0.1f, 0.1f) * DeltaTime;
    Humidity += FMath::RandRange(-0.5f, 0.5f) * DeltaTime;
    WindSpeed += FMath::RandRange(-0.2f, 0.2f) * DeltaTime;
    
    // Clamp values
    Temperature = FMath::Clamp(Temperature, -30.0f, 50.0f);
    Humidity = FMath::Clamp(Humidity, 0.0f, 100.0f);
    WindSpeed = FMath::Clamp(WindSpeed, 0.0f, 100.0f);
}

void UMingEcologicalEnvironmentSystem::UpdateSeason(float DeltaTime)
{
    static float SeasonTimer = 0.0f;
    SeasonTimer += DeltaTime * SimulationSpeed;
    
    if (SeasonTimer >= SeasonDuration)
    {
        // Change to next season
        int32 CurrentSeasonValue = static_cast<int32>(CurrentSeason);
        CurrentSeasonValue = (CurrentSeasonValue + 1) % 4;
        CurrentSeason = static_cast<ESeason>(CurrentSeasonValue);
        
        SetSeason(CurrentSeason);
        SeasonTimer = 0.0f;
    }
}

void UMingEcologicalEnvironmentSystem::UpdateClimate(float DeltaTime)
{
    // Update climate data
    FClimateData NewData;
    NewData.Timestamp = FDateTime::Now();
    NewData.Temperature = Temperature;
    NewData.Humidity = Humidity;
    NewData.Precipitation = Precipitation;
    NewData.WindSpeed = WindSpeed;
    NewData.Season = CurrentSeason;
    NewData.Weather = CurrentWeather;
    
    ClimateData.Add(NewData);
    
    // Limit climate data history
    if (ClimateData.Num() > 1000)
    {
        ClimateData.RemoveAt(0);
    }
}

void UMingEcologicalEnvironmentSystem::UpdateEcosystemConditions(float DeltaTime)
{
    for (auto& EcosystemPair : Ecosystems)
    {
        FEcosystem& Ecosystem = EcosystemPair.Value;
        
        // Natural ecosystem changes
        Ecosystem.Health += FMath::RandRange(-0.001f, 0.001f) * DeltaTime;
        Ecosystem.Balance += FMath::RandRange(-0.001f, 0.001f) * DeltaTime;
        
        // Clamp values
        Ecosystem.Health = FMath::Clamp(Ecosystem.Health, 0.0f, 1.0f);
        Ecosystem.Balance = FMath::Clamp(Ecosystem.Balance, 0.0f, 1.0f);
        
        // Apply environmental effects
        ApplyEnvironmentalEffectsToEcosystem(Ecosystem);
    }
}

void UMingEcologicalEnvironmentSystem::UpdateResourceRegeneration(float DeltaTime)
{
    for (auto& ResourcePair : Resources)
    {
        FEnvironmentalResource& Resource = ResourcePair.Value;
        
        // Regenerate resources
        if (Resource.Quantity < Resource.OriginalQuantity)
        {
            Resource.Quantity += Resource.RegenerationRate * DeltaTime;
            Resource.Quantity = FMath::Min(Resource.Quantity, Resource.OriginalQuantity);
        }
        
        // Quality changes
        Resource.Quality += FMath::RandRange(-0.0001f, 0.0001f) * DeltaTime;
        Resource.Quality = FMath::Clamp(Resource.Quality, 0.0f, 1.0f);
    }
}

void UMingEcologicalEnvironmentSystem::UpdateEcosystemBalance(float DeltaTime)
{
    for (auto& EcosystemPair : Ecosystems)
    {
        const FString& EcosystemID = EcosystemPair.Key;
        float CurrentBalance = CalculateEcosystemBalance(EcosystemID);
        
        // Auto-balance if needed
        if (CurrentBalance < 0.3f)
        {
            ApplyBalancingMeasures(EcosystemID);
        }
    }
}

void UMingEcologicalEnvironmentSystem::ApplyEnvironmentalEffectsToEcosystem(FEcosystem& Ecosystem)
{
    // Apply weather effects
    switch (CurrentWeather)
    {
        case EWeatherType::Stormy:
            Ecosystem.Health -= 0.01f;
            Ecosystem.Balance -= 0.02f;
            break;
        case EWeatherType::Sunny:
            Ecosystem.Health += 0.005f;
            break;
        case EWeatherType::Rainy:
            Ecosystem.Health += 0.01f;
            Ecosystem.Balance += 0.005f;
            break;
    }
    
    // Apply seasonal effects
    switch (CurrentSeason)
    {
        case ESeason::Spring:
            Ecosystem.Health += 0.01f;
            Ecosystem.Balance += 0.005f;
            break;
        case ESeason::Winter:
            Ecosystem.Health -= 0.005f;
            break;
    }
}

void UMingEcologicalEnvironmentSystem::ApplyEnvironmentalEffect(const FString& ZoneID, const FEnvironmentalEffect& Effect)
{
    // Find and update zone
    for (FEnvironmentalZone& Zone : EnvironmentalZones)
    {
        if (Zone.ZoneID == ZoneID)
        {
            Zone.Temperature += Effect.TemperatureChange;
            Zone.Humidity += Effect.HumidityChange;
            Zone.Fertility += Effect.FertilityChange;
            Zone.Biodiversity += Effect.BiodiversityChange;
            Zone.ResourceDensity += Effect.ResourceDensityChange;
            
            // Clamp values
            Zone.Temperature = FMath::Clamp(Zone.Temperature, -50.0f, 60.0f);
            Zone.Humidity = FMath::Clamp(Zone.Humidity, 0.0f, 100.0f);
            Zone.Fertility = FMath::Clamp(Zone.Fertility, 0.0f, 1.0f);
            Zone.Biodiversity = FMath::Clamp(Zone.Biodiversity, 0.0f, 1.0f);
            Zone.ResourceDensity = FMath::Clamp(Zone.ResourceDensity, 0.0f, 1.0f);
            
            break;
        }
    }
}

EWeatherType UMingEcologicalEnvironmentSystem::GetExtremeWeatherType() const
{
    TArray<EWeatherType> ExtremeWeather = {
        EWeatherType::Stormy, EWeatherType::Snowy
    };
    
    return ExtremeWeather[FMath::RandRange(0, ExtremeWeather.Num() - 1)];
}

void UMingEcologicalEnvironmentSystem::UpdateClimateData()
{
    // Climate data is updated in UpdateClimate function
    // This function can be used for additional climate analysis
}

float UMingEcologicalEnvironmentSystem::CalculateAverageTemperature() const
{
    if (EnvironmentalZones.Num() == 0)
    {
        return Temperature;
    }
    
    float TotalTemperature = 0.0f;
    for (const FEnvironmentalZone& Zone : EnvironmentalZones)
    {
        TotalTemperature += Zone.Temperature;
    }
    
    return TotalTemperature / EnvironmentalZones.Num();
}

float UMingEcologicalEnvironmentSystem::CalculateAverageHumidity() const
{
    if (EnvironmentalZones.Num() == 0)
    {
        return Humidity;
    }
    
    float TotalHumidity = 0.0f;
    for (const FEnvironmentalZone& Zone : EnvironmentalZones)
    {
        TotalHumidity += Zone.Humidity;
    }
    
    return TotalHumidity / EnvironmentalZones.Num();
}

float UMingEcologicalEnvironmentSystem::CalculateTotalBiodiversity() const
{
    float TotalBiodiversity = 0.0f;
    
    for (const FEnvironmentalZone& Zone : EnvironmentalZones)
    {
        TotalBiodiversity += Zone.Biodiversity * Zone.Size;
    }
    
    return TotalBiodiversity;
}

float UMingEcologicalEnvironmentSystem::CalculateEcosystemHealth() const
{
    if (Ecosystems.Num() == 0)
    {
        return 0.0f;
    }
    
    float TotalHealth = 0.0f;
    for (const auto& EcosystemPair : Ecosystems)
    {
        TotalHealth += EcosystemPair.Value.Health;
    }
    
    return TotalHealth / Ecosystems.Num();
}

float UMingEcologicalEnvironmentSystem::CalculateClimateStability() const
{
    if (ClimateData.Num() < 2)
    {
        return 1.0f;
    }
    
    // Calculate temperature variance
    float TotalVariance = 0.0f;
    float MeanTemperature = 0.0f;
    
    for (const FClimateData& Data : ClimateData)
    {
        MeanTemperature += Data.Temperature;
    }
    
    MeanTemperature /= ClimateData.Num();
    
    for (const FClimateData& Data : ClimateData)
    {
        TotalVariance += FMath::Square(Data.Temperature - MeanTemperature);
    }
    
    float Variance = TotalVariance / ClimateData.Num();
    
    // Convert variance to stability (lower variance = higher stability)
    float Stability = 1.0f - FMath::Clamp(Variance / 100.0f, 0.0f, 1.0f);
    
    return Stability;
}

float UMingEcologicalEnvironmentSystem::CalculateResourceSustainability() const
{
    if (Resources.Num() == 0)
    {
        return 1.0f;
    }
    
    float TotalSustainability = 0.0f;
    
    for (const auto& ResourcePair : Resources)
    {
        const FEnvironmentalResource& Resource = ResourcePair.Value;
        
        // Calculate sustainability based on current vs original quantity
        float ResourceSustainability = Resource.Quantity / Resource.OriginalQuantity;
        ResourceSustainability *= Resource.Quality; // Factor in quality
        
        TotalSustainability += ResourceSustainability;
    }
    
    return TotalSustainability / Resources.Num();
}

float UMingEcologicalEnvironmentSystem::CalculateEcosystemBalance(const FString& EcosystemID) const
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return 0.0f;
    }
    
    const FEcosystem& Ecosystem = Ecosystems[EcosystemID];
    
    // Calculate balance based on multiple factors
    float HealthFactor = Ecosystem.Health;
    float BiodiversityFactor = Ecosystem.Biodiversity;
    float ResilienceFactor = Ecosystem.Resilience;
    
    // Check zone balance
    float ZoneBalance = 0.0f;
    int32 ZoneCount = 0;
    
    for (const FEnvironmentalZone& Zone : EnvironmentalZones)
    {
        if (Zone.EcosystemID == EcosystemID)
        {
            ZoneBalance += (Zone.Fertility + Zone.Biodiversity + Zone.ResourceDensity) / 3.0f;
            ZoneCount++;
        }
    }
    
    if (ZoneCount > 0)
    {
        ZoneBalance /= ZoneCount;
    }
    
    return (HealthFactor + BiodiversityFactor + ResilienceFactor + ZoneBalance) / 4.0f;
}

void UMingEcologicalEnvironmentSystem::ApplyBalancingMeasures(const FString& EcosystemID)
{
    if (!Ecosystems.Contains(EcosystemID))
    {
        return;
    }
    
    FEcosystem& Ecosystem = Ecosystems[EcosystemID];
    
    // Restore health
    Ecosystem.Health = FMath::Min(Ecosystem.Health + 0.1f, 1.0f);
    
    // Improve balance
    Ecosystem.Balance = FMath::Min(Ecosystem.Balance + 0.05f, 1.0f);
    
    // Boost resource regeneration in affected zones
    for (auto& ResourcePair : Resources)
    {
        FEnvironmentalResource& Resource = ResourcePair.Value;
        
        for (const FEnvironmentalZone& Zone : EnvironmentalZones)
        {
            if (Zone.EcosystemID == EcosystemID && Resource.ZoneID == Zone.ZoneID)
            {
                Resource.RegenerationRate *= 1.5f; // Boost regeneration
                break;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied balancing measures to ecosystem: %s"), *EcosystemID);
}
