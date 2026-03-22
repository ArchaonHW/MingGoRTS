#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSEcosystemSimulation.generated.h"

UENUM(BlueprintType)
enum class EEcosystemZone: uint8 {
    None UMETA(DisplayName = "無"),
    Forest UMETA(DisplayName = "森林"),
    Grassland UMETA(DisplayName = "草原"),
    Desert UMETA(DisplayName = "沙漠"),
    Wetland UMETA(DisplayName = "濕地"),
    Mountain UMETA(DisplayName = "山地"),
    River UMETA(DisplayName = "河流"),
    Lake UMETA(DisplayName = "湖泊"),
    Ocean UMETA(DisplayName = "海洋"),
    Tundra UMETA(DisplayName = "凍原"),
    Rainforest UMETA(DisplayName = "雨林"),
    Savannah UMETA(DisplayName = "稀樹草原"),
    Count UMETA(DisplayName = "生態區數量")
};

UENUM(BlueprintType)
enum class ESeasonType: uint8 {
    Spring UMETA(DisplayName = "春季"),
    Summer UMETA(DisplayName = "夏季"),
    Autumn UMETA(DisplayName = "秋季"),
    Winter UMETA(DisplayName = "冬季"),
    Count UMETA(DisplayName = "季節數量")
};

UENUM(BlueprintType)
enum class EWeatherType: uint8 {
    Clear UMETA(DisplayName = "晴朗"),
    Cloudy UMETA(DisplayName = "多雲"),
    Rain UMETA(DisplayName = "下雨"),
    Snow UMETA(DisplayName = "下雪"),
    Storm UMETA(DisplayName = "暴風雨"),
    Fog UMETA(DisplayName = "霧"),
    Drought UMETA(DisplayName = "乾旱"),
    Flood UMETA(DisplayName = "洪水"),
    Count UMETA(DisplayName = "天氣數量")
};

UENUM(BlueprintType)
enum class EResourceType: uint8 {
    Water UMETA(DisplayName = "水資源"),
    Food UMETA(DisplayName = "食物"),
    Wood UMETA(DisplayName = "木材"),
    Stone UMETA(DisplayName = "石材"),
    Metal UMETA(DisplayName = "金屬"),
    Fertility UMETA(DisplayName = "肥沃度"),
    Biodiversity UMETA(DisplayName = "生物多樣性"),
    Count UMETA(DisplayName = "資源數量")
};

USTRUCT(BlueprintType)
struct FEcosystemResource
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EResourceType ResourceType;

    UPROPERTY(BlueprintReadWrite)
    float CurrentAmount;

    UPROPERTY(BlueprintReadWrite)
    float MaxCapacity;

    UPROPERTY(BlueprintReadWrite)
    float RegenerationRate;

    UPROPERTY(BlueprintReadWrite)
    float ConsumptionRate;

    UPROPERTY(BlueprintReadWrite)
    float Quality;

    FEcosystemResource()
        : ResourceType(EResourceType::Water)
        , CurrentAmount(100.0f)
        , MaxCapacity(100.0f)
        , RegenerationRate(1.0f)
        , ConsumptionRate(0.5f)
        , Quality(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FClimateCondition
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    ESeasonType CurrentSeason;

    UPROPERTY(BlueprintReadWrite)
    EWeatherType CurrentWeather;

    UPROPERTY(BlueprintReadWrite)
    float Temperature;

    UPROPERTY(BlueprintReadWrite)
    float Humidity;

    UPROPERTY(BlueprintReadWrite)
    float Precipitation;

    UPROPERTY(BlueprintReadWrite)
    float WindSpeed;

    UPROPERTY(BlueprintReadWrite)
    float DaylightHours;

    FClimateCondition()
        : CurrentSeason(ESeasonType::Spring)
        , CurrentWeather(EWeatherType::Clear)
        , Temperature(20.0f)
        , Humidity(50.0f)
        , Precipitation(0.0f)
        , WindSpeed(5.0f)
        , DaylightHours(12.0f)
    {}
};

USTRUCT(BlueprintType)
struct FEcosystemZoneData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    EEcosystemZone ZoneType;

    UPROPERTY(BlueprintReadWrite)
    FVector ZoneCenter;

    UPROPERTY(BlueprintReadWrite)
    float ZoneRadius;

    UPROPERTY(BlueprintReadWrite)
    TMap<EResourceType, FEcosystemResource> Resources;

    UPROPERTY(BlueprintReadWrite)
    FClimateCondition Climate;

    UPROPERTY(BlueprintReadWrite)
    float CarryingCapacity;

    UPROPERTY(BlueprintReadWrite)
    float CurrentPopulation;

    UPROPERTY(BlueprintReadWrite)
    float HealthIndex;

    UPROPERTY(BlueprintReadWrite)
    bool bIsPolluted;

    UPROPERTY(BlueprintReadWrite)
    float PollutionLevel;

    FEcosystemZoneData()
        : ZoneType(EEcosystemZone::Grassland)
        , ZoneRadius(1000.0f)
        , CarryingCapacity(1000.0f)
        , CurrentPopulation(500.0f)
        , HealthIndex(1.0f)
        , bIsPolluted(false)
        , PollutionLevel(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSpeciesPopulation
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString SpeciesName;

    UPROPERTY(BlueprintReadWrite)
    int32 PopulationCount;

    UPROPERTY(BlueprintReadWrite)
    float GrowthRate;

    UPROPERTY(BlueprintReadWrite)
    float DeathRate;

    UPROPERTY(BlueprintReadWrite)
    float MigrationRate;

    UPROPERTY(BlueprintReadWrite)
    TArray<EResourceType> RequiredResources;

    UPROPERTY(BlueprintReadWrite)
    EEcosystemZone PreferredZone;

    FSpeciesPopulation()
        : PopulationCount(100)
        , GrowthRate(0.05f)
        , DeathRate(0.02f)
        , MigrationRate(0.01f)
        , PreferredZone(EEcosystemZone::Forest)
    {}
};

USTRUCT(BlueprintType)
struct FEcosystemEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString EventName;

    UPROPERTY(BlueprintReadWrite)
    FString EventDescription;

    UPROPERTY(BlueprintReadWrite)
    EEcosystemZone AffectedZone;

    UPROPERTY(BlueprintReadWrite)
    float ImpactSeverity;

    UPROPERTY(BlueprintReadWrite)
    float Duration;

    UPROPERTY(BlueprintReadWrite)
    TMap<EResourceType, float> ResourceImpacts;

    FEcosystemEvent()
        : ImpactSeverity(0.5f)
        , Duration(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FEcosystemStatistics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 TotalZones;

    UPROPERTY(BlueprintReadOnly)
    int32 HealthyZones;

    UPROPERTY(BlueprintReadOnly)
    int32 PollutedZones;

    UPROPERTY(BlueprintReadOnly)
    float AverageHealthIndex;

    UPROPERTY(BlueprintReadOnly)
    float TotalBiodiversity;

    UPROPERTY(BlueprintReadOnly)
    float AverageTemperature;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveSpecies;

    UPROPERTY(BlueprintReadOnly)
    float EcosystemStability;

    FEcosystemStatistics()
        : TotalZones(0)
        , HealthyZones(0)
        , PollutedZones(0)
        , AverageHealthIndex(0.0f)
        , TotalBiodiversity(0)
        , AverageTemperature(0.0f)
        , ActiveSpecies(0)
        , EcosystemStability(0.0f)
    {}
};

/**
 * 生態環境模擬系統
 */
UCLASS(ClassGroup = (MingStrategic), BlueprintType, Blueprintable)
class MINGSTRATEGIC_API UMingRTSEcosystemSimulation : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSEcosystemSimulation();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|System")
    void InitializeEcosystem();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|System")
    void ShutdownEcosystem();

    // 生態區管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Zones")
    int32 CreateEcosystemZone(EEcosystemZone ZoneType, FVector Center, float Radius);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Zones")
    void RemoveEcosystemZone(int32 ZoneId);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Zones")
    void UpdateZoneResources(int32 ZoneId, EResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Zones")
    FEcosystemZoneData GetZoneData(int32 ZoneId) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Zones")
    TArray<int32> GetAllZones() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Zones")
    TArray<int32> GetZonesByType(EEcosystemZone ZoneType) const;

    // 氣候系統
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Climate")
    void SetSeason(ESeasonType NewSeason);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Climate")
    void SetWeather(int32 ZoneId, EWeatherType Weather);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Climate")
    void UpdateClimate(int32 ZoneId, float TemperatureDelta, float HumidityDelta);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Climate")
    FClimateCondition GetZoneClimate(int32 ZoneId) const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Climate")
    void SimulateWeatherChange();

    // 資源管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Resources")
    void AddResource(int32 ZoneId, EResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Resources")
    void ConsumeResource(int32 ZoneId, EResourceType ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Resources")
    void RegenerateResources(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Resources")
    float GetResourceAvailability(int32 ZoneId, EResourceType ResourceType) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Resources")
    float CalculateResourceBalance(int32 ZoneId) const;

    // 物種管理
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Species")
    void AddSpecies(int32 ZoneId, const FSpeciesPopulation& Species);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Species")
    void UpdateSpeciesPopulation(int32 ZoneId, const FString& SpeciesName, int32 NewCount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Species")
    void SimulatePopulationGrowth(float DeltaTime);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Species")
    TArray<FSpeciesPopulation> GetZoneSpecies(int32 ZoneId) const;

    // 環境影響
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Impact")
    void ApplyPollution(int32 ZoneId, float PollutionAmount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Impact")
    void CleanUpPollution(int32 ZoneId, float CleanupAmount);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Impact")
    void TriggerEcosystemEvent(const FEcosystemEvent& Event);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Impact")
    void ApplyHumanActivityImpact(int32 ZoneId, float ActivityIntensity, float Duration);

    // 生態平衡計算
    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Balance")
    float CalculateZoneHealth(int32 ZoneId) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Balance")
    float CalculateBiodiversityIndex(int32 ZoneId) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Balance")
    float CalculateCarryingCapacity(int32 ZoneId) const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Balance")
    float PredictEcosystemCollapseRisk(int32 ZoneId) const;

    // 模擬更新
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Simulation")
    void SimulateEcosystemTick(float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Simulation")
    void AdvanceSeason();

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Simulation")
    void ProcessResourceFlow();

    // 統計與報告
    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Analytics")
    FEcosystemStatistics GetEcosystemStatistics() const;

    UFUNCTION(BlueprintCallable, Category = "MingRTS|Ecosystem|Analytics")
    void GenerateEcosystemReport(const FString& ReportPath);

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Analytics")
    TArray<int32> FindDegradedZones() const;

    UFUNCTION(BlueprintPure, Category = "MingRTS|Ecosystem|Analytics")
    TArray<int32> FindResourceRichZones() const;

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Ecosystem|Events")
    FOnZoneDegraded OnZoneDegraded;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Ecosystem|Events")
    FOnResourceDepleted OnResourceDepleted;

    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Ecosystem|Events")
    FOnSeasonChanged OnSeasonChanged;

private:
    UPROPERTY()
    TMap<int32, FEcosystemZoneData> EcosystemZones;

    UPROPERTY()
    TMap<int32, TArray<FSpeciesPopulation>> ZoneSpecies;

    UPROPERTY()
    ESeasonType CurrentSeason;

    UPROPERTY()
    int32 NextZoneId;

    UPROPERTY()
    float SimulationTime;

    // 內部計算函數
    float CalculateResourceRegeneration(const FEcosystemResource& Resource, EEcosystemZone ZoneType, const FClimateCondition& Climate) const;
    float CalculatePollutionImpact(float PollutionLevel, EEcosystemZone ZoneType) const;
    void SpreadPollution(int32 SourceZoneId, float Amount);
    void MigrateSpeciesBetweenZones(float DeltaTime);
    void UpdateClimateEffects(float DeltaTime);
    float GetSeasonalTemperatureMultiplier(ESeasonType Season) const;
    float GetWeatherResourceMultiplier(EWeatherType Weather, EResourceType Resource) const;
};

// 事件委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnZoneDegraded, int32, ZoneId, float, HealthIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceDepleted, int32, ZoneId, EResourceType, ResourceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, ESeasonType, NewSeason);
