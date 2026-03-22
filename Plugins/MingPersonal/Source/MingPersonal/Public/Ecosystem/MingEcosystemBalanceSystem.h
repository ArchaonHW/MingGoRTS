#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEcosystemBalanceSystem.generated.h"

UENUM(BlueprintType)
enum class ESpeciesType : uint8
{
    Flora_Tree          UMETA(DisplayName = "树木"),
    Flora_Grass         UMETA(DisplayName = "草类"),
    Flora_Crop          UMETA(DisplayName = "作物"),
    Flora_Aquatic       UMETA(DisplayName = "水生植物"),
    Fauna_Herbivore     UMETA(DisplayName = "草食动物"),
    Fauna_Carnivore     UMETA(DisplayName = "肉食动物"),
    Fauna_Omnivore      UMETA(DisplayName = "杂食动物"),
    Fauna_Aquatic       UMETA(DisplayName = "水生动物"),
    Microorganism       UMETA(DisplayName = "微生物"),
    Decomposer          UMETA(DisplayName = "分解者")
};

UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Water               UMETA(DisplayName = "水"),
    Nitrogen            UMETA(DisplayName = "氮"),
    Phosphorus          UMETA(DisplayName = "磷"),
    Potassium           UMETA(DisplayName = "钾"),
    Carbon              UMETA(DisplayName = "碳"),
    Oxygen              UMETA(DisplayName = "氧"),
    Sunlight            UMETA(DisplayName = "阳光"),
    OrganicMatter       UMETA(DisplayName = "有机物"),
    Minerals            UMETA(DisplayName = "矿物质")
};

UENUM(BlueprintType)
enum class EcosystemHealthState : uint8
{
    Thriving            UMETA(DisplayName = "繁荣"),
    Healthy             UMETA(DisplayName = "健康"),
    Stable              UMETA(DisplayName = "稳定"),
    Stressed            UMETA(DisplayName = "压力"),
    Degraded            UMETA(DisplayName = "退化"),
    Critical            UMETA(DisplayName = "危急"),
    Collapsed           UMETA(DisplayName = "崩溃")
};

USTRUCT(BlueprintType)
struct FSpeciesPopulation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESpeciesType Species;
    
    UPROPERTY(BlueprintReadOnly)
    FString SpeciesName;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentPopulation;
    
    UPROPERTY(BlueprintReadOnly)
    int32 OptimalPopulation;
    
    UPROPERTY(BlueprintReadOnly)
    int32 CarryingCapacity;
    
    UPROPERTY(BlueprintReadOnly)
    float BirthRate;
    
    UPROPERTY(BlueprintReadOnly)
    float DeathRate;
    
    UPROPERTY(BlueprintReadOnly)
    float GrowthRate;
    
    UPROPERTY(BlueprintReadOnly)
    float HealthFactor;
    
    UPROPERTY(BlueprintReadOnly)
    float FoodRequirement;
    
    UPROPERTY(BlueprintReadOnly)
    float WaterRequirement;
    
    UPROPERTY(BlueprintReadOnly)
    float SpaceRequirement;
    
    FSpeciesPopulation()
        : Species(ESpeciesType::Flora_Tree)
        , CurrentPopulation(100)
        , OptimalPopulation(500)
        , CarryingCapacity(1000)
        , BirthRate(0.1f)
        , DeathRate(0.05f)
        , GrowthRate(0.05f)
        , HealthFactor(1.0f)
        , FoodRequirement(1.0f)
        , WaterRequirement(1.0f)
        , SpaceRequirement(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FResourcePool
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EResourceType Resource;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentAmount;
    
    UPROPERTY(BlueprintReadOnly)
    float MaximumCapacity;
    
    UPROPERTY(BlueprintReadOnly)
    float NaturalRegenerationRate;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentRegenerationRate;
    
    UPROPERTY(BlueprintReadOnly)
    float ConsumptionRate;
    
    UPROPERTY(BlueprintReadOnly)
    float PollutionLevel;
    
    UPROPERTY(BlueprintReadOnly)
    float Quality;
    
    FResourcePool()
        : Resource(EResourceType::Water)
        , CurrentAmount(1000.0f)
        , MaximumCapacity(10000.0f)
        , NaturalRegenerationRate(10.0f)
        , CurrentRegenerationRate(10.0f)
        , ConsumptionRate(5.0f)
        , PollutionLevel(0.0f)
        , Quality(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FEcosystemZone
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    FVector CenterLocation;
    
    UPROPERTY(BlueprintReadOnly)
    float Radius;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<ESpeciesType, FSpeciesPopulation> SpeciesPopulations;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<EResourceType, FResourcePool> ResourcePools;
    
    UPROPERTY(BlueprintReadOnly)
    EcosystemHealthState HealthState;
    
    UPROPERTY(BlueprintReadOnly)
    float BiodiversityIndex;
    
    UPROPERTY(BlueprintReadOnly)
    float StabilityIndex;
    
    UPROPERTY(BlueprintReadOnly)
    float ProductivityIndex;
    
    UPROPERTY(BlueprintReadOnly)
    float Temperature;
    
    UPROPERTY(BlueprintReadOnly)
    float Humidity;
    
    UPROPERTY(BlueprintReadOnly)
    float SoilFertility;
    
    FEcosystemZone()
        : Radius(1000.0f)
        , HealthState(EcosystemHealthState::Healthy)
        , BiodiversityIndex(0.5f)
        , StabilityIndex(0.5f)
        , ProductivityIndex(0.5f)
        , Temperature(20.0f)
        , Humidity(50.0f)
        , SoilFertility(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FSpeciesInteraction
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESpeciesType SourceSpecies;
    
    UPROPERTY(BlueprintReadOnly)
    ESpeciesType TargetSpecies;
    
    UPROPERTY(BlueprintReadOnly)
    float InteractionStrength;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsPredation;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsCompetition;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsSymbiosis;
    
    UPROPERTY(BlueprintReadOnly)
    float EffectOnSource;
    
    UPROPERTY(BlueprintReadOnly)
    float EffectOnTarget;
    
    FSpeciesInteraction()
        : SourceSpecies(ESpeciesType::Flora_Tree)
        , TargetSpecies(ESpeciesType::Fauna_Herbivore)
        , InteractionStrength(0.5f)
        , bIsPredation(false)
        , bIsCompetition(false)
        , bIsSymbiosis(false)
        , EffectOnSource(0.0f)
        , EffectOnTarget(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FEcosystemBalanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    float OverallHealth;
    
    UPROPERTY(BlueprintReadOnly)
    float BiodiversityScore;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceSustainability;
    
    UPROPERTY(BlueprintReadOnly)
    float PopulationStability;
    
    UPROPERTY(BlueprintReadOnly)
    float CarbonBalance;
    
    UPROPERTY(BlueprintReadOnly)
    float WaterCycleHealth;
    
    UPROPERTY(BlueprintReadOnly)
    float NitrogenCycleHealth;
    
    UPROPERTY(BlueprintReadOnly)
    float OxygenBalance;
    
    UPROPERTY(BlueprintReadOnly)
    float PredatorPreyBalance;
    
    UPROPERTY(BlueprintReadOnly)
    float ProducerConsumerRatio;
    
    FEcosystemBalanceMetrics()
        : OverallHealth(0.5f)
        , BiodiversityScore(0.5f)
        , ResourceSustainability(0.5f)
        , PopulationStability(0.5f)
        , CarbonBalance(0.5f)
        , WaterCycleHealth(0.5f)
        , NitrogenCycleHealth(0.5f)
        , OxygenBalance(0.5f)
        , PredatorPreyBalance(0.5f)
        , ProducerConsumerRatio(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FNutrientCycle
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EResourceType Nutrient;
    
    UPROPERTY(BlueprintReadOnly)
    float AvailableInSoil;
    
    UPROPERTY(BlueprintReadOnly)
    float InBiomass;
    
    UPROPERTY(BlueprintReadOnly)
    float InDecomposition;
    
    UPROPERTY(BlueprintReadOnly)
    float Leached;
    
    UPROPERTY(BlueprintReadOnly)
    float Atmospheric;
    
    UPROPERTY(BlueprintReadOnly)
    float CycleEfficiency;
    
    FNutrientCycle()
        : Nutrient(EResourceType::Nitrogen)
        , AvailableInSoil(100.0f)
        , InBiomass(50.0f)
        , InDecomposition(20.0f)
        , Leached(5.0f)
        , Atmospheric(10.0f)
        , CycleEfficiency(0.7f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSpeciesPopulationChanged, ESpeciesType, Species, int32, NewPopulation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceLevelChanged, EResourceType, Resource, float, NewLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEcosystemHealthChanged, EcosystemHealthState, NewState, float, HealthScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBalanceDisrupted, FString, DisruptionCause);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingEcosystemBalanceSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingEcosystemBalanceSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    void InitializeBalanceSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    void ShutdownBalanceSystem();
    
    // Zone Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Zone")
    FString CreateEcosystemZone(const FVector& Center, float Radius);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Zone")
    void RemoveEcosystemZone(const FString& ZoneID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Zone")
    FEcosystemZone GetZoneData(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Zone")
    TArray<FString> GetAllZoneIDs() const;
    
    // Species Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Species")
    void AddSpeciesToZone(const FString& ZoneID, ESpeciesType Species, int32 InitialPopulation);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Species")
    void RemoveSpeciesFromZone(const FString& ZoneID, ESpeciesType Species);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Species")
    void ModifySpeciesPopulation(const FString& ZoneID, ESpeciesType Species, int32 Delta);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Species")
    FSpeciesPopulation GetSpeciesPopulation(const FString& ZoneID, ESpeciesType Species) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Species")
    void SetSpeciesParameters(const FString& ZoneID, ESpeciesType Species, const FSpeciesPopulation& Params);
    
    // Resource Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void AddResourceToZone(const FString& ZoneID, EResourceType Resource, float InitialAmount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void ConsumeResource(const FString& ZoneID, EResourceType Resource, float Amount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void RegenerateResource(const FString& ZoneID, EResourceType Resource, float Amount);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Resource")
    FResourcePool GetResourcePool(const FString& ZoneID, EResourceType Resource) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Resource")
    void SetResourceParameters(const FString& ZoneID, EResourceType Resource, const FResourcePool& Params);
    
    // Balance Calculations
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    void CalculateEcosystemBalance(const FString& ZoneID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Balance")
    FEcosystemBalanceMetrics GetBalanceMetrics(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Balance")
    EcosystemHealthState GetZoneHealthState(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    float CalculateBiodiversityIndex(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    float CalculateStabilityIndex(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Balance")
    float CalculateProductivityIndex(const FString& ZoneID);
    
    // Nutrient Cycles
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Nutrients")
    void SimulateNutrientCycle(const FString& ZoneID, EResourceType Nutrient, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Nutrients")
    FNutrientCycle GetNutrientCycleState(const FString& ZoneID, EResourceType Nutrient) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Nutrients")
    void OptimizeNutrientCycles(const FString& ZoneID);
    
    // Species Interactions
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Interactions")
    void AddSpeciesInteraction(const FString& ZoneID, ESpeciesType Source, ESpeciesType Target, float Strength, bool bIsPredation);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Interactions")
    void ProcessSpeciesInteractions(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Interactions")
    TArray<FSpeciesInteraction> GetSpeciesInteractions(const FString& ZoneID) const;
    
    // Population Dynamics
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Population")
    void SimulatePopulationGrowth(const FString& ZoneID, ESpeciesType Species, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Population")
    void SimulatePopulationDynamics(const FString& ZoneID, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Population")
    int32 CalculateOptimalPopulation(const FString& ZoneID, ESpeciesType Species) const;
    
    // Environmental Effects
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void ApplyTemperatureEffect(const FString& ZoneID, float Temperature);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void ApplyHumidityEffect(const FString& ZoneID, float Humidity);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void ApplyPollutionEffect(const FString& ZoneID, float PollutionAmount);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void RestoreEcosystem(const FString& ZoneID, float RestorationRate);
    
    // Balance Restoration
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Restoration")
    void InitiateRebalancing(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Restoration")
    void IntroduceKeystoneSpecies(const FString& ZoneID, ESpeciesType Species);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Restoration")
    void RemoveInvasiveSpecies(const FString& ZoneID, ESpeciesType Species);
    
    // Tick
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void Tick(float DeltaTime);
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnSpeciesPopulationChanged OnSpeciesPopulationChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnResourceLevelChanged OnResourceLevelChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnEcosystemHealthChanged OnEcosystemHealthChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnBalanceDisrupted OnBalanceDisrupted;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    TMap<FString, FEcosystemZone> EcosystemZones;
    
    UPROPERTY()
    TMap<FString, TArray<FSpeciesInteraction>> ZoneInteractions;
    
    UPROPERTY()
    TMap<FString, TMap<EResourceType, FNutrientCycle>> ZoneNutrientCycles;
    
    // Default configurations
    UPROPERTY()
    TMap<ESpeciesType, FSpeciesPopulation> DefaultSpeciesConfigs;
    
    UPROPERTY()
    TMap<EResourceType, FResourcePool> DefaultResourceConfigs;
    
    // Private methods
    void InitializeDefaultConfigs();
    void UpdateZoneHealthState(const FString& ZoneID);
    float CalculateCompetitionFactor(const FString& ZoneID, ESpeciesType Species) const;
    float CalculateResourceAvailability(const FString& ZoneID, ESpeciesType Species) const;
    void ApplyCarryingCapacityLimit(const FString& ZoneID, ESpeciesType Species);
    void UpdateResourceRegenerationRates(const FString& ZoneID);
    void CheckForBalanceDisruption(const FString& ZoneID);
    float CalculateShannonDiversity(const FString& ZoneID) const;
    void BalancePredatorPreyRatio(const FString& ZoneID);
    void UpdateSoilFertility(const FString& ZoneID);
};
