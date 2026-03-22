#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Interface.h"
#include "Engine/Engine.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "MingEcologicalEnvironmentSystem.generated.h"

// 生態系統類型枚舉
UENUM(BlueprintType)
enum class EEcosystemType : uint8
{
    Forest = 0,           // 森林生態系統
    Grassland = 1,        // 草原生態系統
    Wetland = 2,          // 濕地生態系統
    Desert = 3,           // 沙漠生態系統
    Ocean = 4,            // 海洋生態系統
    River = 5,            // 河流生態系統
    Mountain = 6,         // 山地生態系統
    Urban = 7,            // 城市生態系統
    Agricultural = 8,     // 農業生態系統
    Mixed = 9             // 混合生態系統
};

// 環境因子類型枚舉
UENUM(BlueprintType)
enum class EEnvironmentalFactor : uint8
{
    Temperature = 0,       // 溫度
    Humidity = 1,          // 濕度
    Precipitation = 2,     // 降水量
    WindSpeed = 3,         // 風速
    Sunlight = 4,          // 陽光
    SoilQuality = 5,       // 土壤質量
    AirQuality = 6,        // 空氣質量
    WaterQuality = 7,       // 水質
    Biodiversity = 8,      // 生物多樣性
    VegetationDensity = 9  // 植被密度
};

// 季節類型枚舉
UENUM(BlueprintType)
enum class ESeason : uint8
{
    Spring = 0,            // 春季
    Summer = 1,            // 夏季
    Autumn = 2,            // 秋季
    Winter = 3             // 冬季
};

// 天氣類型枚舉
UENUM(BlueprintType)
enum class EWeatherType : uint8
{
    Clear = 0,             // 晴朗
    Cloudy = 1,            // 多雲
    Rainy = 2,             // 下雨
    Stormy = 3,            // 暴風雨
    Snowy = 4,             // 下雪
    Foggy = 5,             // 有霧
    Windy = 6,             // 大風
    Extreme = 7            // 極端天氣
};

// 生態平衡狀態枚舉
UENUM(BlueprintType)
enum class EEcologicalBalance : uint8
{
    Perfect = 0,           // 完美平衡
    Good = 1,              // 良好平衡
    Stable = 2,            // 穩定平衡
    Unstable = 3,          // 不穩定平衡
    Degraded = 4,          // 退化生態
    Collapsed = 5          // 生態崩潰
};

// 環境因子數據結構
USTRUCT(BlueprintType)
struct FEnvironmentalFactorData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    EEnvironmentalFactor FactorType = EEnvironmentalFactor::Temperature;

    UPROPERTY(BlueprintReadOnly)
    float CurrentValue = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float OptimalValue = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MinValue = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxValue = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float ChangeRate = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float ImpactWeight = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedSpecies;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdateTime;

    UPROPERTY(BlueprintReadOnly)
    bool bIsCritical = false;
};

// 生態系統數據結構
USTRUCT(BlueprintType)
struct FEcosystemData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EcosystemID;

    UPROPERTY(BlueprintReadOnly)
    EEcosystemType EcosystemType = EEcosystemType::Forest;

    UPROPERTY(BlueprintReadOnly)
    FString EcosystemName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FVector Location = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly)
    float Area = 1000.0f;

    UPROPERTY(BlueprintReadOnly)
    TMap<EEnvironmentalFactor, FEnvironmentalFactorData> EnvironmentalFactors;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> NativeSpecies;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> SpeciesPopulation;

    UPROPERTY(BlueprintReadOnly)
    EEcologicalBalance BalanceState = EEcologicalBalance::Stable;

    UPROPERTY(BlueprintReadOnly)
    float BiodiversityIndex = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float EcosystemHealth = 100.0f;

    UPROPERTY(BlueprintReadOnly)
    ESeason CurrentSeason = ESeason::Spring;

    UPROPERTY(BlueprintReadOnly)
    EWeatherType CurrentWeather = EWeatherType::Clear;

    UPROPERTY(BlueprintReadOnly)
    FDateTime CreationTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastUpdateTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Threats;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConservationEfforts;
};

// 生態事件結構
USTRUCT(BlueprintType)
struct FEcologicalEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString EventID;

    UPROPERTY(BlueprintReadOnly)
    FString EventType;

    UPROPERTY(BlueprintReadOnly)
    FString EventName;

    UPROPERTY(BlueprintReadOnly)
    FString Description;

    UPROPERTY(BlueprintReadOnly)
    FString AffectedEcosystemID;

    UPROPERTY(BlueprintReadOnly)
    float Severity = 1.0f;

    UPROPERTY(BlueprintReadOnly)
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly)
    FDateTime EndTime;

    UPROPERTY(BlueprintReadOnly)
    TMap<EEnvironmentalFactor, float> EnvironmentalImpacts;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedSpecies;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = false;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRecurring = false;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventParameters;
};

// 生態影響評估結構
USTRUCT(BlueprintType)
struct FEcologicalImpactAssessment
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString AssessmentID;

    UPROPERTY(BlueprintReadOnly)
    FString SourceType;

    UPROPERTY(BlueprintReadOnly)
    FString SourceDescription;

    UPROPERTY(BlueprintReadOnly)
    FString TargetEcosystemID;

    UPROPERTY(BlueprintReadOnly)
    TMap<EEnvironmentalFactor, float> FactorImpacts;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> SpeciesImpacts;

    UPROPERTY(BlueprintReadOnly)
    float OverallImpact = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RecoveryTime = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> MitigationStrategies;

    UPROPERTY(BlueprintReadOnly)
    FDateTime AssessmentTime;

    UPROPERTY(BlueprintReadOnly)
    FString RiskLevel;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RecommendedActions;
};

// 資源再生數據結構
USTRUCT(BlueprintType)
struct FResourceRegenerationData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ResourceID;

    UPROPERTY(BlueprintReadOnly)
    FString ResourceType;

    UPROPERTY(BlueprintReadOnly)
    FString ResourceName;

    UPROPERTY(BlueprintReadOnly)
    float CurrentAmount = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float MaxCapacity = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float RegenerationRate = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float ConsumptionRate = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    FDateTime LastRegenerationTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RequiredFactors;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> FactorRequirements;

    UPROPERTY(BlueprintReadOnly)
    bool bIsRegenerating = false;

    UPROPERTY(BlueprintReadOnly)
    float RegenerationEfficiency = 1.0f;
};

// 委託聲明
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcosystemCreated, const FEcosystemData&, Ecosystem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcosystemBalanceChanged, const FEcosystemData&, Ecosystem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnvironmentalFactorChanged, const FEnvironmentalFactorData&, Factor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEcologicalEventOccurred, const FEcologicalEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSeasonChanged, ESeason, NewSeason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeatherChanged, EWeatherType, NewWeather);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBiodiversityChanged, const FString&, EcosystemID, float NewIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceRegenerated, const FString&, ResourceID, float Amount);

/**
 * 生態環境模擬系統接口
 * 提供真實的環境變化模擬、生態系統平衡和資源再生功能
 */
UINTERFACE(BlueprintType)
class UEcologicalEnvironmentSystemInterface : public UInterface
{
    GENERATED_BODY()
};

class IEcologicalEnvironmentSystemInterface
{
    GENERATED_BODY()

public:
    // 初始化系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    bool InitializeEcologicalSystem();

    // 創建生態系統
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    FString CreateEcosystem(EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area);

    // 更新環境因子
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    bool UpdateEnvironmentalFactor(const FString& EcosystemID, EEnvironmentalFactor FactorType, float NewValue);

    // 模擬環境變化
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    bool SimulateEnvironmentalChange(const FString& EcosystemID, float DeltaTime);

    // 觸發生態事件
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    FString TriggerEcologicalEvent(const FString& EcosystemID, const FString& EventType, float Severity);

    // 評估生態影響
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    FEcologicalImpactAssessment AssessEcologicalImpact(const FString& SourceType, const FString& TargetEcosystemID, const TMap<EEnvironmentalFactor, float>& Impacts);

    // 處理資源再生
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    bool ProcessResourceRegeneration(const FString& EcosystemID, float DeltaTime);

    // 更新季節和天氣
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    bool UpdateSeasonAndWeather(const FString& EcosystemID, float DeltaTime);

    // 計算生態平衡
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    EEcologicalBalance CalculateEcologicalBalance(const FString& EcosystemID);

    // 獲取生態系統數據
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    FEcosystemData GetEcosystemData(const FString& EcosystemID) const;

    // 獲取系統統計信息
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Ecological Environment System")
    TMap<FString, float> GetSystemStatistics() const;
};

/**
 * 生態環境模擬系統
 * 實現真實的環境變化模擬、生態系統平衡和資源再生
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = (Innovation))
class MINGRTS_API UMingEcologicalEnvironmentSystem : public UObject, public IEcologicalEnvironmentSystemInterface
{
    GENERATED_BODY()

public:
    UMingEcologicalEnvironmentSystem();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool InitializeEcologicalSystem() override;

    // 創建生態系統
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    FString CreateEcosystem(EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area) override;

    // 更新環境因子
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool UpdateEnvironmentalFactor(const FString& EcosystemID, EEnvironmentalFactor FactorType, float NewValue) override;

    // 模擬環境變化
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool SimulateEnvironmentalChange(const FString& EcosystemID, float DeltaTime) override;

    // 觸發生態事件
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    FString TriggerEcologicalEvent(const FString& EcosystemID, const FString& EventType, float Severity) override;

    // 評估生態影響
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    FEcologicalImpactAssessment AssessEcologicalImpact(const FString& SourceType, const FString& TargetEcosystemID, const TMap<EEnvironmentalFactor, float>& Impacts) override;

    // 處理資源再生
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool ProcessResourceRegeneration(const FString& EcosystemID, float DeltaTime) override;

    // 更新季節和天氣
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool UpdateSeasonAndWeather(const FString& EcosystemID, float DeltaTime) override;

    // 計算生態平衡
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    EEcologicalBalance CalculateEcologicalBalance(const FString& EcosystemID) override;

    // 獲取生態系統數據
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    FEcosystemData GetEcosystemData(const FString& EcosystemID) const override;

    // 獲取系統統計信息
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    TMap<FString, float> GetSystemStatistics() const override;

    // 高級功能
    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    TArray<FEcosystemData> GetAllEcosystems() const;

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    TArray<FEcologicalEvent> GetActiveEvents() const;

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    TArray<FResourceRegenerationData> GetResourceStatus(const FString& EcosystemID) const;

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool ApplyConservationEffort(const FString& EcosystemID, const FString& EffortType, float Effectiveness);

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool RestoreEcosystem(const FString& EcosystemID, float RestorationAmount);

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    float CalculateBiodiversityIndex(const FString& EcosystemID) const;

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool SetSeasonalCycle(const FString& EcosystemID, float SeasonDuration);

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool AddSpecies(const FString& EcosystemID, const FString& SpeciesName, int32 Population);

    UFUNCTION(BlueprintCallable, Category = "Ecological Environment System")
    bool RemoveSpecies(const FString& EcosystemID, const FString& SpeciesName);

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnEcosystemCreated OnEcosystemCreated;

    UPROPERTY(BlueprintAssignable)
    FOnEcosystemBalanceChanged OnEcosystemBalanceChanged;

    UPROPERTY(BlueprintAssignable)
    FOnEnvironmentalFactorChanged OnEnvironmentalFactorChanged;

    UPROPERTY(BlueprintAssignable)
    FOnEcologicalEventOccurred OnEcologicalEventOccurred;

    UPROPERTY(BlueprintAssignable)
    FOnSeasonChanged OnSeasonChanged;

    UPROPERTY(BlueprintAssignable)
    FOnWeatherChanged OnWeatherChanged;

    UPROPERTY(BlueprintAssignable)
    FOnBiodiversityChanged OnBiodiversityChanged;

    UPROPERTY(BlueprintAssignable)
    FOnResourceRegenerated OnResourceRegenerated;

protected:
    // 系統組件
    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    TMap<FString, FEcosystemData> Ecosystems;

    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    TArray<FEcologicalEvent> ActiveEvents;

    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    TMap<FString, FResourceRegenerationData> ResourceData;

    // 配置參數
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    float SimulationSpeed = 1.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    float SeasonDuration = 90.0f; // 天

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    float WeatherChangeProbability = 0.1f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    float BiodiversityThreshold = 0.7f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    float EcosystemHealthThreshold = 50.0f;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    bool bEnableSeasonalChanges = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    bool bEnableWeatherSimulation = true;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Ecological Environment System")
    bool bEnableResourceRegeneration = true;

    // 狀態變數
    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    bool bIsInitialized = false;

    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    int32 ActiveEcosystemCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    float GlobalBiodiversityIndex = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Ecological Environment System")
    EEcologicalBalance GlobalEcologicalBalance = EEcologicalBalance::Stable;

private:
    // 內部方法
    FString GenerateUniqueEcosystemID() const;
    FString GenerateUniqueEventID() const;
    FString GenerateUniqueResourceID() const;

    FEnvironmentalFactorData CreateDefaultFactorData(EEnvironmentalFactor FactorType, EEcosystemType EcosystemType);
    FEcosystemData CreateDefaultEcosystemData(const FString& EcosystemID, EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area);
    FEcologicalEvent CreateDefaultEventData(const FString& EventID, const FString& EventType, const FString& EcosystemID, float Severity);

    void UpdateEnvironmentalFactors(const FString& EcosystemID, float DeltaTime);
    void ProcessEcologicalEvents(float DeltaTime);
    void UpdateGlobalStatistics();

    ESeason CalculateNextSeason(ESeason CurrentSeason);
    EWeatherType CalculateWeather(ESeason CurrentSeason, const TMap<EEnvironmentalFactor, FEnvironmentalFactorData>& Factors);
    float CalculateEcosystemHealth(const FEcosystemData& Ecosystem) const;
    float CalculateSpeciesDiversity(const FEcosystemData& Ecosystem) const;

    void ApplySeasonalEffects(const FString& EcosystemID, ESeason NewSeason);
    void ApplyWeatherEffects(const FString& EcosystemID, EWeatherType NewWeather);
    void ApplyEventEffects(const FString& EcosystemID, const FEcologicalEvent& Event);

    bool ValidateEcosystemCreation(EEcosystemType EcosystemType, const FString& EcosystemName, const FVector& Location, float Area) const;
    bool ValidateEnvironmentalFactorUpdate(const FString& EcosystemID, EEnvironmentalFactor FactorType, float NewValue) const;

    // 定時器
    FTimerHandle SimulationTimerHandle;
    FTimerHandle EventProcessingTimerHandle;
    FTimerHandle ResourceRegenerationTimerHandle;
    FTimerHandle StatisticsUpdateTimerHandle;

    // 統計數據
    UPROPERTY()
    TMap<FString, float> SystemStats;

    UPROPERTY()
    float LastSimulationTime = 0.0f;

    UPROPERTY()
    int32 TotalEventsTriggered = 0;

    UPROPERTY()
    int32 TotalEcosystemsCreated = 0;

    UPROPERTY()
    float TotalResourcesRegenerated = 0.0f;
};
