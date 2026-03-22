#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingEcosystemDisasterSystem.generated.h"

UENUM(BlueprintType)
enum class ENaturalDisasterType: uint8 {
    Drought              UMETA(DisplayName = "干旱"),
    Flood                UMETA(DisplayName = "洪水"),
    Earthquake           UMETA(DisplayName = "地震"),
    Wildfire             UMETA(DisplayName = "野火"),
    Hurricane            UMETA(DisplayName = "飓风"),
    Tornado              UMETA(DisplayName = "龙卷风"),
    Tsunami              UMETA(DisplayName = "海啸"),
    VolcanicEruption     UMETA(DisplayName = "火山爆发"),
    Landslide            UMETA(DisplayName = "山体滑坡"),
    Avalanche            UMETA(DisplayName = "雪崩"),
    Plague               UMETA(DisplayName = "瘟疫"),
    PestInfestation      UMETA(DisplayName = "虫害"),
    Blizzard             UMETA(DisplayName = "暴风雪"),
    HeatWave             UMETA(DisplayName = "热浪"),
    ColdWave             UMETA(DisplayName = "寒潮")
};

UENUM(BlueprintType)
enum class EDisasterSeverity: uint8 {
    Minor           UMETA(DisplayName = "轻微"),
    Moderate        UMETA(DisplayName = "中等"),
    Severe          UMETA(DisplayName = "严重"),
    Catastrophic    UMETA(DisplayName = "灾难性"),
    Apocalyptic     UMETA(DisplayName = "毁灭性")
};

UENUM(BlueprintType)
enum class EDisasterPhase: uint8 {
    Dormant         UMETA(DisplayName = "休眠"),
    Building        UMETA(DisplayName = "酝酿"),
    Active          UMETA(DisplayName = "活跃"),
    Peak            UMETA(DisplayName = "巅峰"),
    Declining       UMETA(DisplayName = "消退"),
    Aftermath       UMETA(DisplayName = "善后"),
    Complete        UMETA(DisplayName = "结束")
};

USTRUCT(BlueprintType)
struct FDisasterParameters
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ENaturalDisasterType DisasterType;
    
    UPROPERTY(BlueprintReadOnly)
    EDisasterSeverity Severity;
    
    UPROPERTY(BlueprintReadOnly)
    EDisasterPhase CurrentPhase;
    
    UPROPERTY(BlueprintReadOnly)
    float BaseProbability;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentIntensity;
    
    UPROPERTY(BlueprintReadOnly)
    float MaxIntensity;
    
    UPROPERTY(BlueprintReadOnly)
    float Duration;
    
    UPROPERTY(BlueprintReadOnly)
    float AffectedRadius;
    
    UPROPERTY(BlueprintReadOnly)
    float DamageMultiplier;
    
    UPROPERTY(BlueprintReadOnly)
    float WarningTime;
    
    UPROPERTY(BlueprintReadOnly)
    bool bHasWarning;
    
    UPROPERTY(BlueprintReadOnly)
    float RecoveryTime;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> EnvironmentalConditions;
    
    FDisasterParameters()
        : DisasterType(ENaturalDisasterType::Drought)
        , Severity(EDisasterSeverity::Minor)
        , CurrentPhase(EDisasterPhase::Dormant)
        , BaseProbability(0.01f)
        , CurrentIntensity(0.0f)
        , MaxIntensity(1.0f)
        , Duration(300.0f)
        , AffectedRadius(1000.0f)
        , DamageMultiplier(1.0f)
        , WarningTime(60.0f)
        , bHasWarning(false)
        , RecoveryTime(600.0f)
    {}
};

USTRUCT(BlueprintType)
struct FActiveDisaster
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString DisasterID;
    
    UPROPERTY(BlueprintReadOnly)
    FDisasterParameters Parameters;
    
    UPROPERTY(BlueprintReadOnly)
    FVector Epicenter;
    
    UPROPERTY(BlueprintReadOnly)
    float StartTime;
    
    UPROPERTY(BlueprintReadOnly)
    float CurrentTime;
    
    UPROPERTY(BlueprintReadOnly)
    float Progress;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedZones;
    
    UPROPERTY(BlueprintReadOnly)
    float TotalDamage;
    
    UPROPERTY(BlueprintReadOnly)
    int32 AffectedPopulation;
    
    UPROPERTY(BlueprintReadOnly)
    float EconomicLoss;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsPlayerInvolved;
    
    FActiveDisaster()
        : Epicenter(FVector::ZeroVector)
        , StartTime(0.0f)
        , CurrentTime(0.0f)
        , Progress(0.0f)
        , TotalDamage(0.0f)
        , AffectedPopulation(0)
        , EconomicLoss(0.0f)
        , bIsPlayerInvolved(false)
    {}
};

USTRUCT(BlueprintType)
struct FDisasterImpact
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    ENaturalDisasterType DisasterType;
    
    UPROPERTY(BlueprintReadOnly)
    float DamageSeverity;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<ESpeciesType, int32> SpeciesCasualties;
    
    UPROPERTY(BlueprintReadOnly)
    TMap<EResourceType, float> ResourceDepletion;
    
    UPROPERTY(BlueprintReadOnly)
    float InfrastructureDamage;
    
    UPROPERTY(BlueprintReadOnly)
    float RecoveryProgress;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsRecovering;
    
    UPROPERTY(BlueprintReadOnly)
    float EstimatedRecoveryTime;
    
    FDisasterImpact()
        : DisasterType(ENaturalDisasterType::Drought)
        , DamageSeverity(0.0f)
        , InfrastructureDamage(0.0f)
        , RecoveryProgress(0.0f)
        , bIsRecovering(false)
        , EstimatedRecoveryTime(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FDisasterPrediction
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    ENaturalDisasterType PredictedDisaster;
    
    UPROPERTY(BlueprintReadOnly)
    float Probability;
    
    UPROPERTY(BlueprintReadOnly)
    float ExpectedTime;
    
    UPROPERTY(BlueprintReadOnly)
    FVector PredictedLocation;
    
    UPROPERTY(BlueprintReadOnly)
    EDisasterSeverity PredictedSeverity;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ContributingFactors;
    
    UPROPERTY(BlueprintReadOnly)
    FString WarningMessage;
    
    UPROPERTY(BlueprintReadOnly)
    float ConfidenceLevel;
    
    FDisasterPrediction()
        : PredictedDisaster(ENaturalDisasterType::Drought)
        , Probability(0.0f)
        , ExpectedTime(0.0f)
        , PredictedLocation(FVector::ZeroVector)
        , PredictedSeverity(EDisasterSeverity::Minor)
        , ConfidenceLevel(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FDisasterMitigation
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString MitigationID;
    
    UPROPERTY(BlueprintReadOnly)
    ENaturalDisasterType TargetDisaster;
    
    UPROPERTY(BlueprintReadOnly)
    FString MitigationType;
    
    UPROPERTY(BlueprintReadOnly)
    float Effectiveness;
    
    UPROPERTY(BlueprintReadOnly)
    float Cost;
    
    UPROPERTY(BlueprintReadOnly)
    bool bIsActive;
    
    UPROPERTY(BlueprintReadOnly)
    float CoverageRadius;
    
    UPROPERTY(BlueprintReadOnly)
    FVector Location;
    
    FDisasterMitigation()
        : TargetDisaster(ENaturalDisasterType::Drought)
        , Effectiveness(0.5f)
        , Cost(1000.0f)
        , bIsActive(false)
        , CoverageRadius(500.0f)
        , Location(FVector::ZeroVector)
    {}
};

USTRUCT(BlueprintType)
struct FClimateRiskAssessment
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    FString ZoneID;
    
    UPROPERTY(BlueprintReadOnly)
    float DroughtRisk;
    
    UPROPERTY(BlueprintReadOnly)
    float FloodRisk;
    
    UPROPERTY(BlueprintReadOnly)
    float EarthquakeRisk;
    
    UPROPERTY(BlueprintReadOnly)
    float FireRisk;
    
    UPROPERTY(BlueprintReadOnly)
    float StormRisk;
    
    UPROPERTY(BlueprintReadOnly)
    float OverallRiskLevel;
    
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskFactors;
    
    UPROPERTY(BlueprintReadOnly)
    FString AssessmentDate;
    
    FClimateRiskAssessment()
        : DroughtRisk(0.0f)
        , FloodRisk(0.0f)
        , EarthquakeRisk(0.0f)
        , FireRisk(0.0f)
        , StormRisk(0.0f)
        , OverallRiskLevel(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisasterStarted, FActiveDisaster, Disaster);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisasterProgressed, FActiveDisaster, Disaster);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisasterEnded, FActiveDisaster, Disaster);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDisasterWarning, FDisasterPrediction, Prediction, float, TimeUntil);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisasterImpact, FDisasterImpact, Impact);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRecoveryProgress, FString, ZoneID, float, RecoveryPercentage);

UCLASS(ClassGroup = (MingPersonal), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingEcosystemDisasterSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingEcosystemDisasterSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void InitializeDisasterSystem();
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void ShutdownDisasterSystem();
    
    // Disaster Management
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    FString TriggerNaturalDisaster(ENaturalDisasterType Type, EDisasterSeverity Severity, const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void EndDisaster(const FString& DisasterID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Disaster")
    FActiveDisaster GetActiveDisaster(const FString& DisasterID) const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Disaster")
    TArray<FString> GetAllActiveDisasters() const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void ModifyDisasterIntensity(const FString& DisasterID, float NewIntensity);
    
    // Disaster Simulation
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void SimulateDisasterProgression(const FString& DisasterID, float DeltaTime);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    void ProcessDisasterEffects(const FString& DisasterID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Disaster")
    FDisasterImpact CalculateDisasterImpact(const FString& DisasterID, const FString& ZoneID);
    
    // Prediction System
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Prediction")
    TArray<FDisasterPrediction> GeneratePredictions(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Prediction")
    float CalculateDisasterProbability(ENaturalDisasterType Type, const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Prediction")
    FClimateRiskAssessment AssessClimateRisk(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Prediction")
    void IssueEarlyWarning(const FDisasterPrediction& Prediction);
    
    // Environmental Triggers
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Triggers")
    void CheckEnvironmentalTriggers(const FString& ZoneID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Triggers")
    void RegisterEnvironmentalCondition(const FString& ZoneID, const FString& Condition, float Value);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Triggers")
    void EvaluateDisasterTriggers(const FString& ZoneID);
    
    // Mitigation Measures
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Mitigation")
    FString AddMitigationMeasure(ENaturalDisasterType TargetDisaster, const FString& Type, const FVector& Location);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Mitigation")
    void ActivateMitigation(const FString& MitigationID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Mitigation")
    void DeactivateMitigation(const FString& MitigationID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Mitigation")
    float CalculateMitigationEffectiveness(const FString& MitigationID, const FString& DisasterID);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Mitigation")
    TArray<FDisasterMitigation> GetActiveMitigations(const FString& ZoneID) const;
    
    // Recovery System
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Recovery")
    void StartRecovery(const FString& ZoneID, const FString& DisasterID);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Recovery")
    void ProcessRecovery(const FString& ZoneID, float DeltaTime);
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Recovery")
    float GetRecoveryProgress(const FString& ZoneID) const;
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Recovery")
    void AccelerateRecovery(const FString& ZoneID, float AccelerationFactor);
    
    // Damage Functions
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Damage")
    void ApplyDamageToZone(const FString& ZoneID, const FDisasterImpact& Impact);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Damage")
    void ApplySpeciesDamage(const FString& ZoneID, ESpeciesType Species, int32 Casualties);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Damage")
    void ApplyResourceDamage(const FString& ZoneID, EResourceType Resource, float Depletion);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Damage")
    void ApplyInfrastructureDamage(const FString& ZoneID, float DamageAmount);
    
    // Event Functions
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void CreateEarthquake(const FVector& Epicenter, float Magnitude, float Depth);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void CreateFlood(const FVector& Epicenter, float WaterLevel, float Duration);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void CreateDrought(const FString& ZoneID, float Severity, float Duration);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void CreateWildfire(const FVector& Epicenter, float SpreadRate, float Intensity);
    
    UFUNCTION(BlueprintCallable, Category = "Ecosystem|Events")
    void CreateStorm(const FVector& Epicenter, float WindSpeed, float Precipitation);
    
    // Statistics
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    int32 GetTotalDisasterCount() const { return DisasterHistory.Num(); }
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    float GetTotalDamageCost() const;
    
    UFUNCTION(BlueprintPure, Category = "Ecosystem|Statistics")
    int32 GetTotalCasualties() const;
    
    // Tick
    UFUNCTION(BlueprintCallable, Category = "Ecosystem")
    void Tick(float DeltaTime);
    
    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDisasterStarted OnDisasterStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDisasterProgressed OnDisasterProgressed;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDisasterEnded OnDisasterEnded;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDisasterWarning OnDisasterWarning;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnDisasterImpact OnDisasterImpact;
    
    UPROPERTY(BlueprintAssignable, Category = "Ecosystem|Events")
    FOnRecoveryProgress OnRecoveryProgress;
    
protected:
    UPROPERTY()
    bool bIsInitialized;
    
    UPROPERTY()
    float CurrentGameTime;
    
    UPROPERTY()
    TMap<FString, FActiveDisaster> ActiveDisasters;
    
    UPROPERTY()
    TArray<FActiveDisaster> DisasterHistory;
    
    UPROPERTY()
    TMap<FString, FDisasterMitigation> MitigationMeasures;
    
    UPROPERTY()
    TMap<FString, TMap<FString, float>> ZoneEnvironmentalConditions;
    
    UPROPERTY()
    TMap<FString, FDisasterImpact> ZoneImpacts;
    
    UPROPERTY()
    TMap<FString, float> RecoveryProgress;
    
    UPROPERTY()
    TMap<ENaturalDisasterType, FDisasterParameters> DisasterTemplates;
    
    // Private methods
    void InitializeDisasterTemplates();
    void UpdateDisasterPhase(FActiveDisaster& Disaster);
    void CalculateDisasterDamage(FActiveDisaster& Disaster);
    void AffectNearbyZones(FActiveDisaster& Disaster);
    float CalculateDistanceToDisaster(const FVector& Location, const FActiveDisaster& Disaster) const;
    float CalculateDamageAtDistance(float Distance, const FActiveDisaster& Disaster) const;
    void InitializeDisasterParameters(FDisasterParameters& Params);
    void ApplyEnvironmentalModifiers(FDisasterParameters& Params, const FString& ZoneID);
    bool ShouldTriggerWarning(const FActiveDisaster& Disaster) const;
    void GenerateDisasterWarning(const FActiveDisaster& Disaster);
    void CompleteDisaster(const FString& DisasterID);
    void ArchiveDisaster(const FActiveDisaster& Disaster);
    bool IsDisasterExpired(const FActiveDisaster& Disaster) const;
    void ProcessMitigationEffects(FActiveDisaster& Disaster);
    float GetDisasterDuration(ENaturalDisasterType Type) const;
    float GetDisasterRadius(ENaturalDisasterType Type, EDisasterSeverity Severity) const;
    float GetDisasterProbability(ENaturalDisasterType Type, const FString& ZoneID) const;
    void UpdateEnvironmentalConditions(const FString& ZoneID, float DeltaTime);
    void EvaluateRiskFactors(const FString& ZoneID);
};
