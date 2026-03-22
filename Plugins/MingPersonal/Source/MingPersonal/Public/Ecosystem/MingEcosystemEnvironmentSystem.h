#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEcosystemEnvironmentSystem.generated.h"

UENUM(BlueprintType)
enum class ESeasonType : uint8
{
    Spring      UMETA(DisplayName = "春季"),
    Summer      UMETA(DisplayName = "夏季"),
    Autumn      UMETA(DisplayName = "秋季"),
    Winter      UMETA(DisplayName = "冬季")
};

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear       UMETA(DisplayName = "晴朗"),
    Cloudy      UMETA(DisplayName = "多云"),
    Overcast    UMETA(DisplayName = "阴天"),
    LightRain   UMETA(DisplayName = "小雨"),
    HeavyRain   UMETA(DisplayName = "大雨"),
    Storm       UMETA(DisplayName = "暴风雨"),
    Fog         UMETA(DisplayName = "雾"),
    Snow        UMETA(DisplayName = "雪"),
    Blizzard    UMETA(DisplayName = "暴风雪"),
    Sandstorm   UMETA(DisplayName = "沙尘暴")
};

UENUM(BlueprintType)
enum class EDayNightCycle : uint8
{
    Dawn        UMETA(DisplayName = "黎明"),
    Morning     UMETA(DisplayName = "上午"),
    Noon        UMETA(DisplayName = "中午"),
    Afternoon   UMETA(DisplayName = "下午"),
    Dusk        UMETA(DisplayName = "黄昏"),
    Evening     UMETA(DisplayName = "傍晚"),
    Night       UMETA(DisplayName = "夜晚"),
    Midnight    UMETA(DisplayName = "午夜")
};

UENUM(BlueprintType)
enum class EClimateZone : uint8
{
    Tropical        UMETA(DisplayName = "热带"),
    Subtropical     UMETA(DisplayName = "亚热带"),
    Temperate       UMETA(DisplayName = "温带"),
    ColdTemperate   UMETA(DisplayName = "寒温带"),
    Frigid          UMETA(DisplayName = "寒带"),
    Arid            UMETA(DisplayName = "干旱"),
    Highland        UMETA(DisplayName = "高原")
};

USTRUCT(BlueprintType)
struct FWeatherParameters
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EWeatherType WeatherType;
    
    UPROPERTY(BlueprintReadOnly)
    float Temperature;
    
    UPROPERTY(BlueprintReadOnly)
    float Humidity;
    
    UPROPERTY(BlueprintReadOnly)
    float WindSpeed;
    
    UPROPERTY(BlueprintReadOnly)
    float WindDirection;
    
    UPROPERTY(BlueprintReadOnly)
    float PrecipitationIntensity;
    
    UPROPERTY(BlueprintReadOnly)
    float Visibility;
    
    UPROPERTY(BlueprintReadOnly)
    float CloudCover;
    
    UPROPERTY(BlueprintReadOnly)
    float StormIntensity;
    
    FWeatherParameters()
        : WeatherType(EWeatherType::Clear)
        , Temperature(20.0f)
        , Humidity(50.0f)
        , WindSpeed(0.0f)
        , WindDirection(0.0f)
        , PrecipitationIntensity(0.0f)
        , Visibility(10.0f)
        , CloudCover(0.0f)
        , StormIntensity(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FSeasonData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ESeasonType Season;
    
    UPROPERTY(BlueprintReadOnly)
    float BaseTemperature;
    
    UPROPERTY(BlueprintReadOnly)
    float TemperatureVariation;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageHumidity;
    
    UPROPERTY(BlueprintReadOnly)
    float PrecipitationProbability;
    
    UPROPERTY(BlueprintReadOnly)
    float DayLength;
    
    UPROPERTY(BlueprintReadOnly)
    float GrowthRateMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    float ResourceRegenerationRate;
    
    UPROPERTY(BlueprintReadOnly)
    FLinearColor AmbientLightColor;
    
    UPROPERTY(BlueprintReadOnly)
    float SunIntensity;
    
    FSeasonData()
        : Season(ESeasonType::Spring)
        , BaseTemperature(20.0f)
        , TemperatureVariation(5.0f)
        , AverageHumidity(60.0f)
        , PrecipitationProbability(0.3f)
        , DayLength(12.0f)
        , GrowthRateMultiplier(1.0f)
        , ResourceRegenerationRate(1.0f)
        , AmbientLightColor(FLinearColor(1.0f, 1.0f, 0.9f, 1.0f))
        , SunIntensity(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FDayNightData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EDayNightCycle CurrentPhase;
    
    UPROPERTY(BlueprintReadOnly)
    float DayProgress;
    
    UPROPERTY(BlueprintReadOnly)
    float SunElevation;
    
    UPROPERTY(BlueprintReadOnly)
    float SunAzimuth;
    
    UPROPERTY(BlueprintReadOnly)
    FLinearColor SkyColor;
    
    UPROPERTY(BlueprintReadOnly)
    FLinearColor AmbientColor;
    
    UPROPERTY(BlueprintReadOnly)
    float LightIntensity;
    
    FDayNightData()
        : CurrentPhase(EDayNightCycle::Noon)
        , DayProgress(0.5f)
        , SunElevation(90.0f)
        , SunAzimuth(0.0f)
        , SkyColor(FLinearColor(0.5f, 0.7f, 1.0f, 1.0f))
        , AmbientColor(FLinearColor(0.3f, 0.3f, 0.4f, 1.0f))
        , LightIntensity(1.0f)
    {}
};

USTRUCT(BlueprintType)
struct FClimateZoneData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    EClimateZone ZoneType;
    
    UPROPERTY(BlueprintReadOnly)
    float MinTemperature;
    
    UPROPERTY(BlueprintReadOnly)
    float MaxTemperature;
    
    UPROPERTY(BlueprintReadOnly)
    float AverageHumidity;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<ESeasonType> AvailableSeasons;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<EWeatherType> CommonWeatherTypes;
    
    UPROPERTY(BlueprintReadOnly)
    float Rainfall;
    
    UPROPERTY(BlueprintReadOnly)
    float NaturalDisasterProbability;
    
    FClimateZoneData()
        : ZoneType(EClimateZone::Temperate)
        , MinTemperature(-10.0f)
        , MaxTemperature(35.0f)
        , AverageHumidity(50.0f)
        , Rainfall(800.0f)
        , NaturalDisasterProbability(0.1f)
    {}
};

USTRUCT(BlueprintType)
struct FEnvironmentEvent
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString EventID;
    
    UPROPERTY(BlueprintReadOnly)
    FString EventType;
    
    UPROPERTY(BlueprintReadOnly)
    float Timestamp;
    
    UPROPERTY(BlueprintReadOnly)
    FVector Location;
    
    UPROPERTY(BlueprintReadOnly)
    float Intensity;
    
    UPROPERTY(BlueprintReadOnly)
    float Duration;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> EnvironmentalEffects;
    
    FEnvironmentEvent()
        : Timestamp(0.0f)
        , Location(FVector::ZeroVector)
        , Intensity(1.0f)
        , Duration(60.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSeasonChanged, ESeasonType, NewSeason, ESeasonType, OldSeason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeatherChanged, EWeatherType, NewWeather, FWeatherParameters, WeatherParams);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDayNightCycleChanged, EDayNightCycle, CurrentPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentEvent, FEnvironmentEvent, EventData);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingEcosystemEnvironmentSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingEcosystemEnvironmentSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void InitializeEnvironmentSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Environment")
    void ShutdownEnvironmentSystem();
    
    // Season Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Season")
    void SetCurrentSeason(ESeasonType NewSeason);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Season")
    ESeasonType GetCurrentSeason() const { return CurrentSeason; }
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Season")
    void AdvanceSeason();
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Season")
    FSeasonData GetSeasonData(ESeasonType Season) const;
    
    // Weather Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Weather")
    void SetWeather(EWeatherType NewWeather, float Intensity = 1.0f);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Weather")
    EWeatherType GetCurrentWeather() const { return CurrentWeather; }
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Weather")
    FWeatherParameters GetCurrentWeatherParameters() const { return CurrentWeatherParams; }
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Weather")
    void UpdateWeather(float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Weather")
    void GenerateRandomWeather();
    
    // Day/Night Cycle
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|DayNight")
    void SetDayTime(float Hour);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|DayNight")
    float GetCurrentHour() const { return CurrentHour; }
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|DayNight")
    EDayNightCycle GetCurrentDayPhase() const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|DayNight")
    FDayNightData GetDayNightData() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|DayNight")
    void AdvanceTime(float Hours);
    
    // Climate Zone
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Climate")
    void SetClimateZone(EClimateZone Zone);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Climate")
    EClimateZone GetCurrentClimateZone() const { return CurrentClimateZone; }
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Climate")
    FClimateZoneData GetClimateZoneData() const;
    
    // Environmental Effects
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Effects")
    float GetTemperatureEffectOnGrowth() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Effects")
    float GetWeatherEffectOnVisibility() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Effects")
    float GetWeatherEffectOnMovement() const;
    
    // Event Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void TriggerEnvironmentEvent(const FString& EventType, const FVector& Location, float Intensity);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Events")
    TArray<FEnvironmentEvent> GetActiveEvents() const { return ActiveEvents; }
    
    // Tick
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void Tick(float DeltaTime);
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnSeasonChanged OnSeasonChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnWeatherChanged OnWeatherChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDayNightCycleChanged OnDayNightCycleChanged;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnEnvironmentEvent OnEnvironmentEvent;
    
protected:
    // System state
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float CurrentGameTime;
    
    // Season data
    UPROPERTY()
    ESeasonType CurrentSeason;
    
    UPROPERTY()
    float SeasonProgress;
    
    UPROPERTY()
    float DaysPerSeason;
    
    // Weather data
    UPROPERTY()
    EWeatherType CurrentWeather;
    
    UPROPERTY()
    FWeatherParameters CurrentWeatherParams;
    
    UPROPERTY()
    float WeatherChangeTimer;
    
    UPROPERTY()
    float WeatherChangeInterval;
    
    // Day/Night data
    UPROPERTY()
    float CurrentHour;
    
    UPROPERTY()
    float HoursPerDay;
    
    UPROPERTY()
    EDayNightCycle CurrentDayPhase;
    
    // Climate
    UPROPERTY()
    EClimateZone CurrentClimateZone;
    
    UPROPERTY()
    TMap<ESeasonType, FSeasonData> SeasonDatabase;
    
    UPROPERTY()
    TMap<EClimateZone, FClimateZoneData> ClimateDatabase;
    
    // Events
    UPROPERTY()
    TArray<FEnvironmentEvent> ActiveEvents;
    
    // Private methods
    void InitializeSeasonDatabase();
    void InitializeClimateDatabase();
    void UpdateDayNightCycle();
    void CalculateWeatherParameters();
    void ProcessEnvironmentEvents(float DeltaTime);
    FLinearColor CalculateSkyColor() const;
    float CalculateSunIntensity() const;
    
    // Weather transition
    UPROPERTY()
    EWeatherType TargetWeather;
    
    UPROPERTY()
    float WeatherTransitionProgress;
    
    UPROPERTY()
    float WeatherTransitionDuration;
};
