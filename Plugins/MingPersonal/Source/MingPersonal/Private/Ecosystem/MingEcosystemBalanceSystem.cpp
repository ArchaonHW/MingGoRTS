#include "Ecosystem/MingEcosystemBalanceSystem.h"
#include "Math/UnrealMathUtility.h"

UMingEcosystemBalanceSystem::UMingEcosystemBalanceSystem()
    : bIsInitialized(false)
{
}

void UMingEcosystemBalanceSystem::InitializeBalanceSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    InitializeDefaultConfigs();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Initialized"));
}

void UMingEcosystemBalanceSystem::ShutdownBalanceSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    EcosystemZones.Empty();
    ZoneInteractions.Empty();
    ZoneNutrientCycles.Empty();
    DefaultSpeciesConfigs.Empty();
    DefaultResourceConfigs.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Shutdown"));
}

void UMingEcosystemBalanceSystem::InitializeDefaultConfigs()
{
    // Initialize default species configurations
    // Trees
    FSpeciesPopulation TreeConfig;
    TreeConfig.Species = ESpeciesType::Flora_Tree;
    TreeConfig.SpeciesName = TEXT("树木");
    TreeConfig.OptimalPopulation = 200;
    TreeConfig.CarryingCapacity = 500;
    TreeConfig.BirthRate = 0.02f;
    TreeConfig.DeathRate = 0.01f;
    TreeConfig.GrowthRate = 0.01f;
    TreeConfig.FoodRequirement = 0.0f;
    TreeConfig.WaterRequirement = 5.0f;
    TreeConfig.SpaceRequirement = 10.0f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Flora_Tree, TreeConfig);
    
    // Grass
    FSpeciesPopulation GrassConfig;
    GrassConfig.Species = ESpeciesType::Flora_Grass;
    GrassConfig.SpeciesName = TEXT("草类");
    GrassConfig.OptimalPopulation = 1000;
    GrassConfig.CarryingCapacity = 5000;
    GrassConfig.BirthRate = 0.1f;
    GrassConfig.DeathRate = 0.05f;
    GrassConfig.GrowthRate = 0.05f;
    GrassConfig.FoodRequirement = 0.0f;
    GrassConfig.WaterRequirement = 1.0f;
    GrassConfig.SpaceRequirement = 0.5f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Flora_Grass, GrassConfig);
    
    // Crops
    FSpeciesPopulation CropConfig;
    CropConfig.Species = ESpeciesType::Flora_Crop;
    CropConfig.SpeciesName = TEXT("作物");
    CropConfig.OptimalPopulation = 500;
    CropConfig.CarryingCapacity = 1000;
    CropConfig.BirthRate = 0.0f; // Manual planting
    CropConfig.DeathRate = 0.02f;
    CropConfig.GrowthRate = 0.03f;
    CropConfig.FoodRequirement = 0.0f;
    CropConfig.WaterRequirement = 3.0f;
    CropConfig.SpaceRequirement = 2.0f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Flora_Crop, CropConfig);
    
    // Herbivores
    FSpeciesPopulation HerbivoreConfig;
    HerbivoreConfig.Species = ESpeciesType::Fauna_Herbivore;
    HerbivoreConfig.SpeciesName = TEXT("草食动物");
    HerbivoreConfig.OptimalPopulation = 100;
    HerbivoreConfig.CarryingCapacity = 300;
    HerbivoreConfig.BirthRate = 0.05f;
    HerbivoreConfig.DeathRate = 0.02f;
    HerbivoreConfig.GrowthRate = 0.03f;
    HerbivoreConfig.FoodRequirement = 10.0f;
    HerbivoreConfig.WaterRequirement = 3.0f;
    HerbivoreConfig.SpaceRequirement = 20.0f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Fauna_Herbivore, HerbivoreConfig);
    
    // Carnivores
    FSpeciesPopulation CarnivoreConfig;
    CarnivoreConfig.Species = ESpeciesType::Fauna_Carnivore;
    CarnivoreConfig.SpeciesName = TEXT("肉食动物");
    CarnivoreConfig.OptimalPopulation = 20;
    CarnivoreConfig.CarryingCapacity = 50;
    CarnivoreConfig.BirthRate = 0.03f;
    CarnivoreConfig.DeathRate = 0.01f;
    CarnivoreConfig.GrowthRate = 0.02f;
    CarnivoreConfig.FoodRequirement = 5.0f;
    CarnivoreConfig.WaterRequirement = 2.0f;
    CarnivoreConfig.SpaceRequirement = 50.0f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Fauna_Carnivore, CarnivoreConfig);
    
    // Omnivores
    FSpeciesPopulation OmnivoreConfig;
    OmnivoreConfig.Species = ESpeciesType::Fauna_Omnivore;
    OmnivoreConfig.SpeciesName = TEXT("杂食动物");
    OmnivoreConfig.OptimalPopulation = 50;
    OmnivoreConfig.CarryingCapacity = 150;
    OmnivoreConfig.BirthRate = 0.04f;
    OmnivoreConfig.DeathRate = 0.015f;
    OmnivoreConfig.GrowthRate = 0.025f;
    OmnivoreConfig.FoodRequirement = 7.0f;
    OmnivoreConfig.WaterRequirement = 2.5f;
    OmnivoreConfig.SpaceRequirement = 30.0f;
    DefaultSpeciesConfigs.Add(ESpeciesType::Fauna_Omnivore, OmnivoreConfig);
    
    // Initialize default resource configurations
    // Water
    FResourcePool WaterConfig;
    WaterConfig.Resource = EResourceType::Water;
    WaterConfig.MaximumCapacity = 10000.0f;
    WaterConfig.NaturalRegenerationRate = 50.0f;
    WaterConfig.CurrentRegenerationRate = 50.0f;
    WaterConfig.Quality = 1.0f;
    DefaultResourceConfigs.Add(EResourceType::Water, WaterConfig);
    
    // Nitrogen
    FResourcePool NitrogenConfig;
    NitrogenConfig.Resource = EResourceType::Nitrogen;
    NitrogenConfig.MaximumCapacity = 5000.0f;
    NitrogenConfig.NaturalRegenerationRate = 10.0f;
    NitrogenConfig.CurrentRegenerationRate = 10.0f;
    NitrogenConfig.Quality = 1.0f;
    DefaultResourceConfigs.Add(EResourceType::Nitrogen, NitrogenConfig);
    
    // Sunlight
    FResourcePool SunlightConfig;
    SunlightConfig.Resource = EResourceType::Sunlight;
    SunlightConfig.MaximumCapacity = 1000000.0f; // Essentially infinite
    SunlightConfig.NaturalRegenerationRate = 1000.0f;
    SunlightConfig.CurrentRegenerationRate = 1000.0f;
    SunlightConfig.Quality = 1.0f;
    DefaultResourceConfigs.Add(EResourceType::Sunlight, SunlightConfig);
    
    // Organic Matter
    FResourcePool OrganicConfig;
    OrganicConfig.Resource = EResourceType::OrganicMatter;
    OrganicConfig.MaximumCapacity = 10000.0f;
    OrganicConfig.NaturalRegenerationRate = 20.0f;
    OrganicConfig.CurrentRegenerationRate = 20.0f;
    OrganicConfig.Quality = 1.0f;
    DefaultResourceConfigs.Add(EResourceType::OrganicMatter, OrganicConfig);
}

FString UMingEcosystemBalanceSystem::CreateEcosystemZone(const FVector& Center, float Radius)
{
    FString ZoneID = FGuid::NewGuid().ToString();
    
    FEcosystemZone NewZone;
    NewZone.ZoneID = ZoneID;
    NewZone.CenterLocation = Center;
    NewZone.Radius = Radius;
    
    // Initialize with default resources
    for (const auto& ResourcePair : DefaultResourceConfigs)
    {
        FResourcePool Pool = ResourcePair.Value;
        Pool.CurrentAmount = Pool.MaximumCapacity * 0.8f; // Start at 80%
        NewZone.ResourcePools.Add(ResourcePair.Key, Pool);
    }
    
    EcosystemZones.Add(ZoneID, NewZone);
    
    // Initialize interactions array
    ZoneInteractions.Add(ZoneID, TArray<FSpeciesInteraction>());
    
    // Initialize nutrient cycles
    TMap<EResourceType, FNutrientCycle> NutrientCycles;
    
    FNutrientCycle NitrogenCycle;
    NitrogenCycle.Nutrient = EResourceType::Nitrogen;
    NutrientCycles.Add(EResourceType::Nitrogen, NitrogenCycle);
    
    FNutrientCycle PhosphorusCycle;
    PhosphorusCycle.Nutrient = EResourceType::Phosphorus;
    NutrientCycles.Add(EResourceType::Phosphorus, PhosphorusCycle);
    
    FNutrientCycle CarbonCycle;
    CarbonCycle.Nutrient = EResourceType::Carbon;
    NutrientCycles.Add(EResourceType::Carbon, CarbonCycle);
    
    ZoneNutrientCycles.Add(ZoneID, NutrientCycles);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Created ecosystem zone %s"), *ZoneID);
    
    return ZoneID;
}

void UMingEcosystemBalanceSystem::RemoveEcosystemZone(const FString& ZoneID)
{
    EcosystemZones.Remove(ZoneID);
    ZoneInteractions.Remove(ZoneID);
    ZoneNutrientCycles.Remove(ZoneID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Removed ecosystem zone %s"), *ZoneID);
}

FEcosystemZone UMingEcosystemBalanceSystem::GetZoneData(const FString& ZoneID) const
{
    if (EcosystemZones.Contains(ZoneID))
    {
        return EcosystemZones[ZoneID];
    }
    return FEcosystemZone();
}

TArray<FString> UMingEcosystemBalanceSystem::GetAllZoneIDs() const
{
    TArray<FString> ZoneIDs;
    EcosystemZones.GetKeys(ZoneIDs);
    return ZoneIDs;
}

void UMingEcosystemBalanceSystem::AddSpeciesToZone(const FString& ZoneID, ESpeciesType Species, int32 InitialPopulation)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (!DefaultSpeciesConfigs.Contains(Species))
    {
        return;
    }
    
    FSpeciesPopulation Pop = DefaultSpeciesConfigs[Species];
    Pop.CurrentPopulation = FMath::Clamp(InitialPopulation, 0, Pop.CarryingCapacity);
    
    Zone.SpeciesPopulations.Add(Species, Pop);
    
    OnSpeciesPopulationChanged.Broadcast(Species, Pop.CurrentPopulation);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Added species %d to zone %s with population %d"),
        static_cast<int32>(Species), *ZoneID, InitialPopulation);
}

void UMingEcosystemBalanceSystem::RemoveSpeciesFromZone(const FString& ZoneID, ESpeciesType Species)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    Zone.SpeciesPopulations.Remove(Species);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Removed species %d from zone %s"),
        static_cast<int32>(Species), *ZoneID);
}

void UMingEcosystemBalanceSystem::ModifySpeciesPopulation(const FString& ZoneID, ESpeciesType Species, int32 Delta)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.SpeciesPopulations.Contains(Species))
    {
        FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
        int32 NewPopulation = FMath::Clamp(Pop.CurrentPopulation + Delta, 0, Pop.CarryingCapacity);
        Pop.CurrentPopulation = NewPopulation;
        
        OnSpeciesPopulationChanged.Broadcast(Species, NewPopulation);
    }
}

FSpeciesPopulation UMingEcosystemBalanceSystem::GetSpeciesPopulation(const FString& ZoneID, ESpeciesType Species) const
{
    if (EcosystemZones.Contains(ZoneID))
    {
        const FEcosystemZone& Zone = EcosystemZones[ZoneID];
        if (Zone.SpeciesPopulations.Contains(Species))
        {
            return Zone.SpeciesPopulations[Species];
        }
    }
    return FSpeciesPopulation();
}

void UMingEcosystemBalanceSystem::SetSpeciesParameters(const FString& ZoneID, ESpeciesType Species, const FSpeciesPopulation& Params)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.SpeciesPopulations.Contains(Species))
    {
        Zone.SpeciesPopulations[Species] = Params;
    }
}

void UMingEcosystemBalanceSystem::AddResourceToZone(const FString& ZoneID, EResourceType Resource, float InitialAmount)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    FResourcePool Pool;
    if (DefaultResourceConfigs.Contains(Resource))
    {
        Pool = DefaultResourceConfigs[Resource];
    }
    Pool.Resource = Resource;
    Pool.CurrentAmount = FMath::Clamp(InitialAmount, 0.0f, Pool.MaximumCapacity);
    
    Zone.ResourcePools.Add(Resource, Pool);
    
    OnResourceLevelChanged.Broadcast(Resource, Pool.CurrentAmount);
}

void UMingEcosystemBalanceSystem::ConsumeResource(const FString& ZoneID, EResourceType Resource, float Amount)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.ResourcePools.Contains(Resource))
    {
        FResourcePool& Pool = Zone.ResourcePools[Resource];
        Pool.CurrentAmount = FMath::Max(0.0f, Pool.CurrentAmount - Amount);
        Pool.ConsumptionRate += Amount;
        
        OnResourceLevelChanged.Broadcast(Resource, Pool.CurrentAmount);
    }
}

void UMingEcosystemBalanceSystem::RegenerateResource(const FString& ZoneID, EResourceType Resource, float Amount)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.ResourcePools.Contains(Resource))
    {
        FResourcePool& Pool = Zone.ResourcePools[Resource];
        Pool.CurrentAmount = FMath::Min(Pool.MaximumCapacity, Pool.CurrentAmount + Amount);
        
        OnResourceLevelChanged.Broadcast(Resource, Pool.CurrentAmount);
    }
}

FResourcePool UMingEcosystemBalanceSystem::GetResourcePool(const FString& ZoneID, EResourceType Resource) const
{
    if (EcosystemZones.Contains(ZoneID))
    {
        const FEcosystemZone& Zone = EcosystemZones[ZoneID];
        if (Zone.ResourcePools.Contains(Resource))
        {
            return Zone.ResourcePools[Resource];
        }
    }
    return FResourcePool();
}

void UMingEcosystemBalanceSystem::SetResourceParameters(const FString& ZoneID, EResourceType Resource, const FResourcePool& Params)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    Zone.ResourcePools.Add(Resource, Params);
}

void UMingEcosystemBalanceSystem::CalculateEcosystemBalance(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Calculate various indices
    Zone.BiodiversityIndex = CalculateBiodiversityIndex(ZoneID);
    Zone.StabilityIndex = CalculateStabilityIndex(ZoneID);
    Zone.ProductivityIndex = CalculateProductivityIndex(ZoneID);
    
    // Update health state
    UpdateZoneHealthState(ZoneID);
    
    // Check for disruptions
    CheckForBalanceDisruption(ZoneID);
}

FEcosystemBalanceMetrics UMingEcosystemBalanceSystem::GetBalanceMetrics(const FString& ZoneID) const
{
    FEcosystemBalanceMetrics Metrics;
    
    if (!EcosystemZones.Contains(ZoneID))
    {
        return Metrics;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Overall health based on multiple factors
    Metrics.OverallHealth = (Zone.BiodiversityIndex + Zone.StabilityIndex + Zone.ProductivityIndex) / 3.0f;
    
    // Biodiversity score
    Metrics.BiodiversityScore = Zone.BiodiversityIndex;
    
    // Resource sustainability
    float ResourceHealth = 0.0f;
    int32 ResourceCount = 0;
    for (const auto& ResourcePair : Zone.ResourcePools)
    {
        float Ratio = ResourcePair.Value.CurrentAmount / ResourcePair.Value.MaximumCapacity;
        ResourceHealth += Ratio;
        ResourceCount++;
    }
    if (ResourceCount > 0)
    {
        Metrics.ResourceSustainability = ResourceHealth / ResourceCount;
    }
    
    // Population stability
    float PopStability = 0.0f;
    int32 PopCount = 0;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        const FSpeciesPopulation& Pop = PopPair.Value;
        float Ratio = static_cast<float>(Pop.CurrentPopulation) / Pop.OptimalPopulation;
        float Deviation = FMath::Abs(1.0f - Ratio);
        PopStability += 1.0f - FMath::Clamp(Deviation, 0.0f, 1.0f);
        PopCount++;
    }
    if (PopCount > 0)
    {
        Metrics.PopulationStability = PopStability / PopCount;
    }
    
    // Nutrient cycle health
    if (ZoneNutrientCycles.Contains(ZoneID))
    {
        const auto& Nutrients = ZoneNutrientCycles[ZoneID];
        float CycleHealth = 0.0f;
        int32 CycleCount = 0;
        for (const auto& NutrientPair : Nutrients)
        {
            CycleHealth += NutrientPair.Value.CycleEfficiency;
            CycleCount++;
        }
        if (CycleCount > 0)
        {
            Metrics.NitrogenCycleHealth = CycleHealth / CycleCount;
        }
    }
    
    // Predator-prey balance
    int32 PredatorCount = 0;
    int32 PreyCount = 0;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        ESpeciesType Species = PopPair.Key;
        int32 Population = PopPair.Value.CurrentPopulation;
        
        if (Species == ESpeciesType::Fauna_Carnivore)
        {
            PredatorCount += Population;
        }
        else if (Species == ESpeciesType::Fauna_Herbivore || Species == ESpeciesType::Fauna_Omnivore)
        {
            PreyCount += Population;
        }
    }
    
    if (PredatorCount > 0 && PreyCount > 0)
    {
        float Ratio = static_cast<float>(PredatorCount) / PreyCount;
        // Ideal ratio is around 1:10 to 1:20
        Metrics.PredatorPreyBalance = 1.0f - FMath::Clamp(FMath::Abs(Ratio - 0.1f) * 10.0f, 0.0f, 1.0f);
    }
    else if (PredatorCount == 0 && PreyCount > 0)
    {
        Metrics.PredatorPreyBalance = 0.5f; // No predators, but prey exists
    }
    else
    {
        Metrics.PredatorPreyBalance = 0.0f;
    }
    
    // Producer-consumer ratio
    int32 ProducerCount = 0;
    int32 ConsumerCount = 0;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        ESpeciesType Species = PopPair.Key;
        int32 Population = PopPair.Value.CurrentPopulation;
        
        if (Species == ESpeciesType::Flora_Tree || Species == ESpeciesType::Flora_Grass ||
            Species == ESpeciesType::Flora_Crop || Species == ESpeciesType::Flora_Aquatic)
        {
            ProducerCount += Population;
        }
        else
        {
            ConsumerCount += Population;
        }
    }
    
    if (ProducerCount > 0 && ConsumerCount > 0)
    {
        float Ratio = static_cast<float>(ConsumerCount) / ProducerCount;
        // Ideal ratio varies but generally consumers should be less than producers
        Metrics.ProducerConsumerRatio = 1.0f - FMath::Clamp(FMath::Abs(Ratio - 0.3f) * 3.0f, 0.0f, 1.0f);
    }
    else if (ProducerCount > 0 && ConsumerCount == 0)
    {
        Metrics.ProducerConsumerRatio = 0.7f; // Producers only
    }
    else
    {
        Metrics.ProducerConsumerRatio = 0.0f;
    }
    
    return Metrics;
}

EcosystemHealthState UMingEcosystemBalanceSystem::GetZoneHealthState(const FString& ZoneID) const
{
    if (EcosystemZones.Contains(ZoneID))
    {
        return EcosystemZones[ZoneID].HealthState;
    }
    return EcosystemHealthState::Collapsed;
}

float UMingEcosystemBalanceSystem::CalculateBiodiversityIndex(const FString& ZoneID)
{
    return CalculateShannonDiversity(ZoneID);
}

float UMingEcosystemBalanceSystem::CalculateStabilityIndex(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Stability is based on:
    // 1. Resource availability (30%)
    // 2. Population balance (40%)
    // 3. Species diversity (30%)
    
    float ResourceScore = 0.0f;
    for (const auto& ResourcePair : Zone.ResourcePools)
    {
        float Ratio = ResourcePair.Value.CurrentAmount / ResourcePair.Value.MaximumCapacity;
        ResourceScore += Ratio;
    }
    ResourceScore /= FMath::Max(1, Zone.ResourcePools.Num());
    
    float PopulationScore = 0.0f;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        const FSpeciesPopulation& Pop = PopPair.Value;
        float Ratio = static_cast<float>(Pop.CurrentPopulation) / Pop.OptimalPopulation;
        float OptimalRange = 1.0f - FMath::Clamp(FMath::Abs(1.0f - Ratio) * 2.0f, 0.0f, 1.0f);
        PopulationScore += OptimalRange;
    }
    PopulationScore /= FMath::Max(1, Zone.SpeciesPopulations.Num());
    
    float DiversityScore = CalculateBiodiversityIndex(ZoneID);
    
    float Stability = ResourceScore * 0.3f + PopulationScore * 0.4f + DiversityScore * 0.3f;
    
    FEcosystemZone& MutableZone = EcosystemZones[ZoneID];
    MutableZone.StabilityIndex = Stability;
    
    return Stability;
}

float UMingEcosystemBalanceSystem::CalculateProductivityIndex(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Productivity is based on:
    // 1. Producer population density (50%)
    // 2. Resource availability (30%)
    // 3. Growth rates (20%)
    
    float ProducerScore = 0.0f;
    float TotalGrowthRate = 0.0f;
    int32 ProducerCount = 0;
    
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        ESpeciesType Species = PopPair.Key;
        const FSpeciesPopulation& Pop = PopPair.Value;
        
        if (Species == ESpeciesType::Flora_Tree || Species == ESpeciesType::Flora_Grass ||
            Species == ESpeciesType::Flora_Crop || Species == ESpeciesType::Flora_Aquatic)
        {
            float Density = static_cast<float>(Pop.CurrentPopulation) / Pop.CarryingCapacity;
            ProducerScore += Density;
            TotalGrowthRate += Pop.GrowthRate;
            ProducerCount++;
        }
    }
    
    if (ProducerCount > 0)
    {
        ProducerScore /= ProducerCount;
        TotalGrowthRate /= ProducerCount;
    }
    
    float ResourceScore = 0.0f;
    for (const auto& ResourcePair : Zone.ResourcePools)
    {
        float Ratio = ResourcePair.Value.CurrentAmount / ResourcePair.Value.MaximumCapacity;
        ResourceScore += Ratio;
    }
    ResourceScore /= FMath::Max(1, Zone.ResourcePools.Num());
    
    float Productivity = ProducerScore * 0.5f + ResourceScore * 0.3f + TotalGrowthRate * 10.0f * 0.2f;
    
    FEcosystemZone& MutableZone = EcosystemZones[ZoneID];
    MutableZone.ProductivityIndex = Productivity;
    
    return Productivity;
}

void UMingEcosystemBalanceSystem::SimulateNutrientCycle(const FString& ZoneID, EResourceType Nutrient, float DeltaTime)
{
    if (!ZoneNutrientCycles.Contains(ZoneID))
    {
        return;
    }
    
    auto& NutrientCycles = ZoneNutrientCycles[ZoneID];
    
    if (!NutrientCycles.Contains(Nutrient))
    {
        return;
    }
    
    FNutrientCycle& Cycle = NutrientCycles[Nutrient];
    
    // Simulate nutrient cycle dynamics
    // 1. Decomposition increases available nutrients
    float DecompositionRate = 0.05f;
    float Decomposed = Cycle.InDecomposition * DecompositionRate * DeltaTime;
    Cycle.AvailableInSoil += Decomposed;
    Cycle.InDecomposition -= Decomposed;
    
    // 2. Plants absorb nutrients
    float AbsorptionRate = 0.1f;
    if (EcosystemZones.Contains(ZoneID))
    {
        float PlantBiomass = 0.0f;
        for (const auto& PopPair : EcosystemZones[ZoneID].SpeciesPopulations)
        {
            ESpeciesType Species = PopPair.Key;
            if (Species == ESpeciesType::Flora_Tree || Species == ESpeciesType::Flora_Grass ||
                Species == ESpeciesType::Flora_Crop)
            {
                PlantBiomass += PopPair.Value.CurrentPopulation;
            }
        }
        
        float Absorbed = FMath::Min(Cycle.AvailableInSoil * AbsorptionRate * DeltaTime, 
                                     PlantBiomass * 0.01f);
        Cycle.AvailableInSoil -= Absorbed;
        Cycle.InBiomass += Absorbed;
    }
    
    // 3. Leaching
    float LeachingRate = 0.01f;
    float Leached = Cycle.AvailableInSoil * LeachingRate * DeltaTime;
    Cycle.AvailableInSoil -= Leached;
    Cycle.Leached += Leached;
    
    // 4. Atmospheric exchange (for some nutrients)
    if (Nutrient == EResourceType::Nitrogen || Nutrient == EResourceType::Carbon)
    {
        float AtmosphericExchange = FMath::Sin(FPlatformTime::Seconds()) * 0.1f * DeltaTime;
        Cycle.Atmospheric += AtmosphericExchange;
        Cycle.AvailableInSoil += AtmosphericExchange * 0.1f;
    }
    
    // Update cycle efficiency
    float TotalNutrients = Cycle.AvailableInSoil + Cycle.InBiomass + 
                          Cycle.InDecomposition + Cycle.Atmospheric;
    if (TotalNutrients > 0)
    {
        // Efficiency is higher when nutrients are actively cycling
        float ActiveCycle = (Cycle.InBiomass + Cycle.AvailableInSoil) / TotalNutrients;
        Cycle.CycleEfficiency = ActiveCycle;
    }
}

FNutrientCycle UMingEcosystemBalanceSystem::GetNutrientCycleState(const FString& ZoneID, EResourceType Nutrient) const
{
    if (ZoneNutrientCycles.Contains(ZoneID))
    {
        const auto& NutrientCycles = ZoneNutrientCycles[ZoneID];
        if (NutrientCycles.Contains(Nutrient))
        {
            return NutrientCycles[Nutrient];
        }
    }
    return FNutrientCycle();
}

void UMingEcosystemBalanceSystem::OptimizeNutrientCycles(const FString& ZoneID)
{
    // TODO: Implement nutrient cycle optimization logic
    // This could involve adding specific species to improve nutrient cycling
    // or adjusting environmental parameters
}

void UMingEcosystemBalanceSystem::AddSpeciesInteraction(const FString& ZoneID, ESpeciesType Source, ESpeciesType Target, float Strength, bool bIsPredation)
{
    if (!ZoneInteractions.Contains(ZoneID))
    {
        return;
    }
    
    FSpeciesInteraction Interaction;
    Interaction.SourceSpecies = Source;
    Interaction.TargetSpecies = Target;
    Interaction.InteractionStrength = Strength;
    Interaction.bIsPredation = bIsPredation;
    Interaction.bIsCompetition = !bIsPredation;
    
    ZoneInteractions[ZoneID].Add(Interaction);
}

void UMingEcosystemBalanceSystem::ProcessSpeciesInteractions(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID) || !ZoneInteractions.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    const TArray<FSpeciesInteraction>& Interactions = ZoneInteractions[ZoneID];
    
    for (const FSpeciesInteraction& Interaction : Interactions)
    {
        if (!Zone.SpeciesPopulations.Contains(Interaction.SourceSpecies) ||
            !Zone.SpeciesPopulations.Contains(Interaction.TargetSpecies))
        {
            continue;
        }
        
        FSpeciesPopulation& SourcePop = Zone.SpeciesPopulations[Interaction.SourceSpecies];
        FSpeciesPopulation& TargetPop = Zone.SpeciesPopulations[Interaction.TargetSpecies];
        
        if (Interaction.bIsPredation)
        {
            // Predation: predator population grows, prey population declines
            int32 PredationAmount = FMath::RoundToInt(Interaction.InteractionStrength * SourcePop.CurrentPopulation * 0.1f);
            PredationAmount = FMath::Min(PredationAmount, TargetPop.CurrentPopulation / 2);
            
            TargetPop.CurrentPopulation -= PredationAmount;
            
            // Predator benefits from successful hunting
            float Benefit = PredationAmount * 0.5f;
            SourcePop.CurrentPopulation = FMath::Min(
                SourcePop.CurrentPopulation + FMath::RoundToInt(Benefit),
                SourcePop.CarryingCapacity);
        }
        else if (Interaction.bIsCompetition)
        {
            // Competition: both populations are negatively affected
            float CompetitionEffect = Interaction.InteractionStrength * 0.05f;
            int32 SourceDecline = FMath::RoundToInt(SourcePop.CurrentPopulation * CompetitionEffect);
            int32 TargetDecline = FMath::RoundToInt(TargetPop.CurrentPopulation * CompetitionEffect);
            
            SourcePop.CurrentPopulation -= SourceDecline;
            TargetPop.CurrentPopulation -= TargetDecline;
        }
    }
}

TArray<FSpeciesInteraction> UMingEcosystemBalanceSystem::GetSpeciesInteractions(const FString& ZoneID) const
{
    if (ZoneInteractions.Contains(ZoneID))
    {
        return ZoneInteractions[ZoneID];
    }
    return TArray<FSpeciesInteraction>();
}

void UMingEcosystemBalanceSystem::SimulatePopulationGrowth(const FString& ZoneID, ESpeciesType Species, float DeltaTime)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (!Zone.SpeciesPopulations.Contains(Species))
    {
        return;
    }
    
    FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
    
    // Calculate growth based on multiple factors
    float CompetitionFactor = CalculateCompetitionFactor(ZoneID, Species);
    float ResourceAvailability = CalculateResourceAvailability(ZoneID, Species);
    float CarryingCapacityFactor = 1.0f - (static_cast<float>(Pop.CurrentPopulation) / Pop.CarryingCapacity);
    
    // Logistic growth model with environmental factors
    float EffectiveGrowthRate = Pop.GrowthRate * Pop.HealthFactor * CompetitionFactor * 
                              ResourceAvailability * CarryingCapacityFactor;
    
    int32 Growth = FMath::RoundToInt(Pop.CurrentPopulation * EffectiveGrowthRate * DeltaTime);
    int32 Deaths = FMath::RoundToInt(Pop.CurrentPopulation * Pop.DeathRate * DeltaTime);
    
    int32 NewPopulation = Pop.CurrentPopulation + Growth - Deaths;
    NewPopulation = FMath::Clamp(NewPopulation, 0, Pop.CarryingCapacity);
    
    Pop.CurrentPopulation = NewPopulation;
    
    OnSpeciesPopulationChanged.Broadcast(Species, NewPopulation);
}

void UMingEcosystemBalanceSystem::SimulatePopulationDynamics(const FString& ZoneID, float DeltaTime)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Process interactions first
    ProcessSpeciesInteractions(ZoneID);
    
    // Then simulate growth for each species
    TArray<ESpeciesType> SpeciesList;
    Zone.SpeciesPopulations.GetKeys(SpeciesList);
    
    for (ESpeciesType Species : SpeciesList)
    {
        SimulatePopulationGrowth(ZoneID, Species, DeltaTime);
    }
    
    // Update ecosystem metrics
    CalculateEcosystemBalance(ZoneID);
}

int32 UMingEcosystemBalanceSystem::CalculateOptimalPopulation(const FString& ZoneID, ESpeciesType Species) const
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 0;
    }
    
    if (!DefaultSpeciesConfigs.Contains(Species))
    {
        return 0;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    const FSpeciesPopulation& DefaultConfig = DefaultSpeciesConfigs[Species];
    
    // Calculate optimal population based on available resources
    float ResourceFactor = CalculateResourceAvailability(ZoneID, Species);
    float SpaceFactor = Zone.Radius * Zone.Radius * PI / DefaultConfig.SpaceRequirement / 1000000.0f;
    
    float OptimalPop = DefaultConfig.OptimalPopulation * FMath::Min(ResourceFactor, SpaceFactor);
    
    return FMath::RoundToInt(OptimalPop);
}

void UMingEcosystemBalanceSystem::ApplyTemperatureEffect(const FString& ZoneID, float Temperature)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    Zone.Temperature = Temperature;
    
    // Apply effects to species based on temperature
    for (auto& PopPair : Zone.SpeciesPopulations)
    {
        FSpeciesPopulation& Pop = PopPair.Value;
        
        // Optimal temperature is around 20-25°C for most species
        float OptimalTemp = 22.0f;
        float TempDiff = FMath::Abs(Temperature - OptimalTemp);
        
        if (TempDiff > 15.0f)
        {
            Pop.HealthFactor = FMath::Max(0.3f, 1.0f - (TempDiff - 15.0f) * 0.02f);
        }
        else
        {
            Pop.HealthFactor = 1.0f;
        }
    }
}

void UMingEcosystemBalanceSystem::ApplyHumidityEffect(const FString& ZoneID, float Humidity)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    Zone.Humidity = Humidity;
    
    // Water availability affects all species
    if (Zone.ResourcePools.Contains(EResourceType::Water))
    {
        FResourcePool& WaterPool = Zone.ResourcePools[EResourceType::Water];
        float HumidityEffect = Humidity / 100.0f;
        WaterPool.CurrentRegenerationRate = WaterPool.NaturalRegenerationRate * HumidityEffect;
    }
}

void UMingEcosystemBalanceSystem::ApplyPollutionEffect(const FString& ZoneID, float PollutionAmount)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Apply pollution to resources
    for (auto& ResourcePair : Zone.ResourcePools)
    {
        FResourcePool& Pool = ResourcePair.Value;
        Pool.PollutionLevel = FMath::Min(1.0f, Pool.PollutionLevel + PollutionAmount);
        Pool.Quality = 1.0f - Pool.PollutionLevel;
        Pool.CurrentRegenerationRate = Pool.NaturalRegenerationRate * Pool.Quality;
    }
    
    // Pollution reduces species health
    for (auto& PopPair : Zone.SpeciesPopulations)
    {
        FSpeciesPopulation& Pop = PopPair.Value;
        Pop.HealthFactor = FMath::Max(0.1f, Pop.HealthFactor - PollutionAmount * 0.5f);
    }
}

void UMingEcosystemBalanceSystem::RestoreEcosystem(const FString& ZoneID, float RestorationRate)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Reduce pollution
    for (auto& ResourcePair : Zone.ResourcePools)
    {
        FResourcePool& Pool = ResourcePair.Value;
        Pool.PollutionLevel = FMath::Max(0.0f, Pool.PollutionLevel - RestorationRate * 0.01f);
        Pool.Quality = 1.0f - Pool.PollutionLevel;
        Pool.CurrentRegenerationRate = Pool.NaturalRegenerationRate * Pool.Quality;
        
        // Restore resource levels
        Pool.CurrentAmount = FMath::Min(Pool.MaximumCapacity, 
                                       Pool.CurrentAmount + Pool.CurrentRegenerationRate * RestorationRate);
    }
    
    // Restore species health
    for (auto& PopPair : Zone.SpeciesPopulations)
    {
        FSpeciesPopulation& Pop = PopPair.Value;
        Pop.HealthFactor = FMath::Min(1.0f, Pop.HealthFactor + RestorationRate * 0.01f);
    }
    
    // Update soil fertility
    Zone.SoilFertility = FMath::Min(1.0f, Zone.SoilFertility + RestorationRate * 0.005f);
}

void UMingEcosystemBalanceSystem::InitiateRebalancing(const FString& ZoneID)
{
    BalancePredatorPreyRatio(ZoneID);
    OptimizeNutrientCycles(ZoneID);
    CalculateEcosystemBalance(ZoneID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemBalanceSystem: Initiated rebalancing for zone %s"), *ZoneID);
}

void UMingEcosystemBalanceSystem::IntroduceKeystoneSpecies(const FString& ZoneID, ESpeciesType Species)
{
    // Keystone species help maintain ecosystem balance
    AddSpeciesToZone(ZoneID, Species, 20);
    
    // Set keystone species parameters (higher impact on ecosystem)
    if (EcosystemZones.Contains(ZoneID))
    {
        FEcosystemZone& Zone = EcosystemZones[ZoneID];
        if (Zone.SpeciesPopulations.Contains(Species))
        {
            FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
            Pop.HealthFactor = 1.2f; // Keystone species are more resilient
        }
    }
}

void UMingEcosystemBalanceSystem::RemoveInvasiveSpecies(const FString& ZoneID, ESpeciesType Species)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.SpeciesPopulations.Contains(Species))
    {
        // Gradually reduce population
        FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
        Pop.CurrentPopulation = FMath::RoundToInt(Pop.CurrentPopulation * 0.9f);
        
        if (Pop.CurrentPopulation < 5)
        {
            Zone.SpeciesPopulations.Remove(Species);
        }
    }
}

void UMingEcosystemBalanceSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // Process each ecosystem zone
    TArray<FString> ZoneIDs;
    EcosystemZones.GetKeys(ZoneIDs);
    
    for (const FString& ZoneID : ZoneIDs)
    {
        // Simulate population dynamics
        SimulatePopulationDynamics(ZoneID, DeltaTime);
        
        // Simulate nutrient cycles
        if (ZoneNutrientCycles.Contains(ZoneID))
        {
            auto& NutrientCycles = ZoneNutrientCycles[ZoneID];
            for (auto& NutrientPair : NutrientCycles)
            {
                SimulateNutrientCycle(ZoneID, NutrientPair.Key, DeltaTime);
            }
        }
        
        // Update resource regeneration
        UpdateResourceRegenerationRates(ZoneID);
    }
}

void UMingEcosystemBalanceSystem::UpdateZoneHealthState(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    FEcosystemBalanceMetrics Metrics = GetBalanceMetrics(ZoneID);
    
    // Determine health state based on overall metrics
    if (Metrics.OverallHealth >= 0.8f)
    {
        Zone.HealthState = EcosystemHealthState::Thriving;
    }
    else if (Metrics.OverallHealth >= 0.6f)
    {
        Zone.HealthState = EcosystemHealthState::Healthy;
    }
    else if (Metrics.OverallHealth >= 0.4f)
    {
        Zone.HealthState = EcosystemHealthState::Stable;
    }
    else if (Metrics.OverallHealth >= 0.3f)
    {
        Zone.HealthState = EcosystemHealthState::Stressed;
    }
    else if (Metrics.OverallHealth >= 0.2f)
    {
        Zone.HealthState = EcosystemHealthState::Degraded;
    }
    else if (Metrics.OverallHealth >= 0.1f)
    {
        Zone.HealthState = EcosystemHealthState::Critical;
    }
    else
    {
        Zone.HealthState = EcosystemHealthState::Collapsed;
    }
    
    if (Metrics.OverallHealth != Zone.StabilityIndex) // Avoid spam
    {
        OnEcosystemHealthChanged.Broadcast(Zone.HealthState, Metrics.OverallHealth);
    }
}

float UMingEcosystemBalanceSystem::CalculateCompetitionFactor(const FString& ZoneID, ESpeciesType Species) const
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 1.0f;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    float TotalSpaceRequired = 0.0f;
    float AvailableSpace = Zone.Radius * Zone.Radius * PI;
    
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        const FSpeciesPopulation& Pop = PopPair.Value;
        TotalSpaceRequired += Pop.CurrentPopulation * Pop.SpaceRequirement;
    }
    
    float SpaceRatio = TotalSpaceRequired / AvailableSpace;
    
    // Competition factor decreases as space becomes limited
    return FMath::Max(0.1f, 1.0f - SpaceRatio * 0.5f);
}

float UMingEcosystemBalanceSystem::CalculateResourceAvailability(const FString& ZoneID, ESpeciesType Species) const
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (!Zone.SpeciesPopulations.Contains(Species))
    {
        return 0.0f;
    }
    
    const FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
    
    float Availability = 1.0f;
    
    // Check water availability
    if (Zone.ResourcePools.Contains(EResourceType::Water))
    {
        const FResourcePool& Water = Zone.ResourcePools[EResourceType::Water];
        float WaterNeeded = Pop.CurrentPopulation * Pop.WaterRequirement;
        float WaterAvailable = Water.CurrentAmount;
        float WaterFactor = FMath::Min(1.0f, WaterAvailable / WaterNeeded);
        Availability *= WaterFactor;
    }
    
    // Check nutrient availability for plants
    if (Species == ESpeciesType::Flora_Tree || Species == ESpeciesType::Flora_Grass ||
        Species == ESpeciesType::Flora_Crop)
    {
        if (Zone.ResourcePools.Contains(EResourceType::Nitrogen))
        {
            const FResourcePool& Nitrogen = Zone.ResourcePools[EResourceType::Nitrogen];
            Availability *= Nitrogen.CurrentAmount / Nitrogen.MaximumCapacity;
        }
    }
    
    return Availability;
}

void UMingEcosystemBalanceSystem::ApplyCarryingCapacityLimit(const FString& ZoneID, ESpeciesType Species)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    if (Zone.SpeciesPopulations.Contains(Species))
    {
        FSpeciesPopulation& Pop = Zone.SpeciesPopulations[Species];
        Pop.CurrentPopulation = FMath::Clamp(Pop.CurrentPopulation, 0, Pop.CarryingCapacity);
    }
}

void UMingEcosystemBalanceSystem::UpdateResourceRegenerationRates(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    for (auto& ResourcePair : Zone.ResourcePools)
    {
        FResourcePool& Pool = ResourcePair.Value;
        
        // Regenerate resources based on current rate
        float Regeneration = Pool.CurrentRegenerationRate * 0.016f; // Assuming 60 FPS tick
        Pool.CurrentAmount = FMath::Min(Pool.MaximumCapacity, Pool.CurrentAmount + Regeneration);
        
        // Decay consumption rate over time
        Pool.ConsumptionRate *= 0.99f;
    }
}

void UMingEcosystemBalanceSystem::CheckForBalanceDisruption(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    FEcosystemBalanceMetrics Metrics = GetBalanceMetrics(ZoneID);
    
    // Check for various disruption conditions
    if (Metrics.PredatorPreyBalance < 0.2f)
    {
        OnBalanceDisrupted.Broadcast(TEXT("捕食者与猎物比例失衡"));
    }
    else if (Metrics.ProducerConsumerRatio < 0.2f)
    {
        OnBalanceDisrupted.Broadcast(TEXT("生产者与消费者比例失衡"));
    }
    else if (Metrics.ResourceSustainability < 0.3f)
    {
        OnBalanceDisrupted.Broadcast(TEXT("资源可持续性危机"));
    }
    else if (Zone.BiodiversityIndex < 0.2f)
    {
        OnBalanceDisrupted.Broadcast(TEXT("生物多样性不足"));
    }
}

float UMingEcosystemBalanceSystem::CalculateShannonDiversity(const FString& ZoneID) const
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return 0.0f;
    }
    
    const FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    int32 TotalPopulation = 0;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        TotalPopulation += PopPair.Value.CurrentPopulation;
    }
    
    if (TotalPopulation == 0)
    {
        return 0.0f;
    }
    
    float ShannonIndex = 0.0f;
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        float Proportion = static_cast<float>(PopPair.Value.CurrentPopulation) / TotalPopulation;
        if (Proportion > 0)
        {
            ShannonIndex -= Proportion * FMath::Log2(Proportion);
        }
    }
    
    // Normalize to 0-1 range (assuming max diversity is log2 of number of species types)
    int32 NumSpeciesTypes = static_cast<int32>(ESpeciesType::Decomposer) + 1;
    float MaxDiversity = FMath::Log2(static_cast<float>(NumSpeciesTypes));
    
    return FMath::Clamp(ShannonIndex / MaxDiversity, 0.0f, 1.0f);
}

void UMingEcosystemBalanceSystem::BalancePredatorPreyRatio(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    int32 PredatorCount = 0;
    int32 PreyCount = 0;
    
    for (const auto& PopPair : Zone.SpeciesPopulations)
    {
        ESpeciesType Species = PopPair.Key;
        int32 Population = PopPair.Value.CurrentPopulation;
        
        if (Species == ESpeciesType::Fauna_Carnivore)
        {
            PredatorCount += Population;
        }
        else if (Species == ESpeciesType::Fauna_Herbivore || Species == ESpeciesType::Fauna_Omnivore)
        {
            PreyCount += Population;
        }
    }
    
    // Ideal predator:prey ratio is approximately 1:10 to 1:20
    float CurrentRatio = (PreyCount > 0) ? static_cast<float>(PredatorCount) / PreyCount : 0.0f;
    float TargetRatio = 0.1f; // 1:10 ratio
    
    if (CurrentRatio > TargetRatio * 2.0f)
    {
        // Too many predators, reduce them
        for (auto& PopPair : Zone.SpeciesPopulations)
        {
            if (PopPair.Key == ESpeciesType::Fauna_Carnivore)
            {
                PopPair.Value.CurrentPopulation = FMath::RoundToInt(PopPair.Value.CurrentPopulation * 0.9f);
            }
        }
    }
    else if (CurrentRatio < TargetRatio * 0.5f && PredatorCount > 0)
    {
        // Too few predators, increase them
        for (auto& PopPair : Zone.SpeciesPopulations)
        {
            if (PopPair.Key == ESpeciesType::Fauna_Carnivore)
            {
                PopPair.Value.CurrentPopulation = FMath::Min(
                    PopPair.Value.CarryingCapacity,
                    FMath::RoundToInt(PopPair.Value.CurrentPopulation * 1.1f));
            }
        }
    }
}

void UMingEcosystemBalanceSystem::UpdateSoilFertility(const FString& ZoneID)
{
    if (!EcosystemZones.Contains(ZoneID))
    {
        return;
    }
    
    FEcosystemZone& Zone = EcosystemZones[ZoneID];
    
    // Soil fertility is affected by organic matter and nutrient cycles
    if (Zone.ResourcePools.Contains(EResourceType::OrganicMatter))
    {
        float OrganicMatter = Zone.ResourcePools[EResourceType::OrganicMatter].CurrentAmount;
        float MaxOrganic = Zone.ResourcePools[EResourceType::OrganicMatter].MaximumCapacity;
        
        Zone.SoilFertility = FMath::Clamp(OrganicMatter / MaxOrganic, 0.0f, 1.0f);
    }
}
