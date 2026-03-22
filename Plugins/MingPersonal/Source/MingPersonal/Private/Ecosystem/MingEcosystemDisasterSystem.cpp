#include "Ecosystem/MingEcosystemDisasterSystem.h"
#include "Math/UnrealMathUtility.h"

UMingEcosystemDisasterSystem::UMingEcosystemDisasterSystem()
    : bIsInitialized(false)
    , CurrentGameTime(0.0f)
{
}

void UMingEcosystemDisasterSystem::InitializeDisasterSystem()
{
    if (bIsInitialized)
    {
        return;
    }
    
    InitializeDisasterTemplates();
    
    CurrentGameTime = 0.0f;
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemDisasterSystem: Initialized"));
}

void UMingEcosystemDisasterSystem::ShutdownDisasterSystem()
{
    if (!bIsInitialized)
    {
        return;
    }
    
    ActiveDisasters.Empty();
    DisasterHistory.Empty();
    MitigationMeasures.Empty();
    ZoneEnvironmentalConditions.Empty();
    ZoneImpacts.Empty();
    RecoveryProgress.Empty();
    DisasterTemplates.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemDisasterSystem: Shutdown"));
}

void UMingEcosystemDisasterSystem::InitializeDisasterTemplates()
{
    // Drought template
    FDisasterParameters DroughtParams;
    DroughtParams.DisasterType = ENaturalDisasterType::Drought;
    DroughtParams.Severity = EDisasterSeverity::Moderate;
    DroughtParams.BaseProbability = 0.05f;
    DroughtParams.MaxIntensity = 1.0f;
    DroughtParams.Duration = 3600.0f; // 1 hour
    DroughtParams.AffectedRadius = 5000.0f;
    DroughtParams.DamageMultiplier = 0.3f;
    DroughtParams.WarningTime = 300.0f; // 5 minutes
    DroughtParams.bHasWarning = true;
    DroughtParams.RecoveryTime = 1800.0f; // 30 minutes
    DisasterTemplates.Add(ENaturalDisasterType::Drought, DroughtParams);
    
    // Flood template
    FDisasterParameters FloodParams;
    FloodParams.DisasterType = ENaturalDisasterType::Flood;
    FloodParams.Severity = EDisasterSeverity::Severe;
    FloodParams.BaseProbability = 0.03f;
    FloodParams.MaxIntensity = 1.0f;
    FloodParams.Duration = 1800.0f; // 30 minutes
    FloodParams.AffectedRadius = 3000.0f;
    FloodParams.DamageMultiplier = 0.6f;
    FloodParams.WarningTime = 180.0f; // 3 minutes
    FloodParams.bHasWarning = true;
    FloodParams.RecoveryTime = 2400.0f; // 40 minutes
    DisasterTemplates.Add(ENaturalDisasterType::Flood, FloodParams);
    
    // Earthquake template
    FDisasterParameters EarthquakeParams;
    EarthquakeParams.DisasterType = ENaturalDisasterType::Earthquake;
    EarthquakeParams.Severity = EDisasterSeverity::Severe;
    EarthquakeParams.BaseProbability = 0.01f;
    EarthquakeParams.MaxIntensity = 1.0f;
    EarthquakeParams.Duration = 60.0f; // 1 minute
    EarthquakeParams.AffectedRadius = 10000.0f;
    EarthquakeParams.DamageMultiplier = 0.8f;
    EarthquakeParams.WarningTime = 30.0f; // 30 seconds
    EarthquakeParams.bHasWarning = false; // Hard to predict
    EarthquakeParams.RecoveryTime = 3600.0f; // 1 hour
    DisasterTemplates.Add(ENaturalDisasterType::Earthquake, EarthquakeParams);
    
    // Wildfire template
    FDisasterParameters FireParams;
    FireParams.DisasterType = ENaturalDisasterType::Wildfire;
    FireParams.Severity = EDisasterSeverity::Catastrophic;
    FireParams.BaseProbability = 0.02f;
    FireParams.MaxIntensity = 1.0f;
    FireParams.Duration = 7200.0f; // 2 hours
    FireParams.AffectedRadius = 8000.0f;
    FireParams.DamageMultiplier = 0.9f;
    FireParams.WarningTime = 120.0f; // 2 minutes
    FireParams.bHasWarning = true;
    FireParams.RecoveryTime = 7200.0f; // 2 hours
    DisasterTemplates.Add(ENaturalDisasterType::Wildfire, FireParams);
    
    // Hurricane template
    FDisasterParameters HurricaneParams;
    HurricaneParams.DisasterType = ENaturalDisasterType::Hurricane;
    HurricaneParams.Severity = EDisasterSeverity::Catastrophic;
    HurricaneParams.BaseProbability = 0.005f;
    HurricaneParams.MaxIntensity = 1.0f;
    HurricaneParams.Duration = 5400.0f; // 1.5 hours
    HurricaneParams.AffectedRadius = 15000.0f;
    HurricaneParams.DamageMultiplier = 1.0f;
    HurricaneParams.WarningTime = 1800.0f; // 30 minutes
    HurricaneParams.bHasWarning = true;
    HurricaneParams.RecoveryTime = 10800.0f; // 3 hours
    DisasterTemplates.Add(ENaturalDisasterType::Hurricane, HurricaneParams);
    
    // Tornado template
    FDisasterParameters TornadoParams;
    TornadoParams.DisasterType = ENaturalDisasterType::Tornado;
    TornadoParams.Severity = EDisasterSeverity::Severe;
    TornadoParams.BaseProbability = 0.008f;
    TornadoParams.MaxIntensity = 1.0f;
    TornadoParams.Duration = 600.0f; // 10 minutes
    TornadoParams.AffectedRadius = 2000.0f;
    TornadoParams.DamageMultiplier = 0.85f;
    TornadoParams.WarningTime = 60.0f; // 1 minute
    TornadoParams.bHasWarning = true;
    TornadoParams.RecoveryTime = 3600.0f; // 1 hour
    DisasterTemplates.Add(ENaturalDisasterType::Tornado, TornadoParams);
    
    // Tsunami template
    FDisasterParameters TsunamiParams;
    TsunamiParams.DisasterType = ENaturalDisasterType::Tsunami;
    TsunamiParams.Severity = EDisasterSeverity::Catastrophic;
    TsunamiParams.BaseProbability = 0.002f;
    TsunamiParams.MaxIntensity = 1.0f;
    TsunamiParams.Duration = 1800.0f; // 30 minutes
    TsunamiParams.AffectedRadius = 12000.0f;
    TsunamiParams.DamageMultiplier = 1.0f;
    TsunamiParams.WarningTime = 600.0f; // 10 minutes
    TsunamiParams.bHasWarning = true;
    TsunamiParams.RecoveryTime = 7200.0f; // 2 hours
    DisasterTemplates.Add(ENaturalDisasterType::Tsunami, TsunamiParams);
    
    // Volcanic eruption template
    FDisasterParameters VolcanoParams;
    VolcanoParams.DisasterType = ENaturalDisasterType::VolcanicEruption;
    VolcanoParams.Severity = EDisasterSeverity::Apocalyptic;
    VolcanoParams.BaseProbability = 0.001f;
    VolcanoParams.MaxIntensity = 1.0f;
    VolcanoParams.Duration = 10800.0f; // 3 hours
    VolcanoParams.AffectedRadius = 20000.0f;
    VolcanoParams.DamageMultiplier = 1.2f;
    VolcanoParams.WarningTime = 3600.0f; // 1 hour
    VolcanoParams.bHasWarning = true;
    VolcanoParams.RecoveryTime = 14400.0f; // 4 hours
    DisasterTemplates.Add(ENaturalDisasterType::VolcanicEruption, VolcanoParams);
    
    // Landslide template
    FDisasterParameters LandslideParams;
    LandslideParams.DisasterType = ENaturalDisasterType::Landslide;
    LandslideParams.Severity = EDisasterSeverity::Moderate;
    LandslideParams.BaseProbability = 0.015f;
    LandslideParams.MaxIntensity = 1.0f;
    LandslideParams.Duration = 300.0f; // 5 minutes
    LandslideParams.AffectedRadius = 1500.0f;
    LandslideParams.DamageMultiplier = 0.5f;
    LandslideParams.WarningTime = 60.0f; // 1 minute
    LandslideParams.bHasWarning = true;
    LandslideParams.RecoveryTime = 3600.0f; // 1 hour
    DisasterTemplates.Add(ENaturalDisasterType::Landslide, LandslideParams);
    
    // Plague template
    FDisasterParameters PlagueParams;
    PlagueParams.DisasterType = ENaturalDisasterType::Plague;
    PlagueParams.Severity = EDisasterSeverity::Severe;
    PlagueParams.BaseProbability = 0.008f;
    PlagueParams.MaxIntensity = 1.0f;
    PlagueParams.Duration = 7200.0f; // 2 hours
    PlagueParams.AffectedRadius = 10000.0f;
    PlagueParams.DamageMultiplier = 0.4f;
    PlagueParams.WarningTime = 600.0f; // 10 minutes
    PlagueParams.bHasWarning = true;
    PlagueParams.RecoveryTime = 5400.0f; // 1.5 hours
    DisasterTemplates.Add(ENaturalDisasterType::Plague, PlagueParams);
    
    // Pest infestation template
    FDisasterParameters PestParams;
    PestParams.DisasterType = ENaturalDisasterType::PestInfestation;
    PestParams.Severity = EDisasterSeverity::Moderate;
    PestParams.BaseProbability = 0.04f;
    PestParams.MaxIntensity = 1.0f;
    PestParams.Duration = 3600.0f; // 1 hour
    PestParams.AffectedRadius = 6000.0f;
    PestParams.DamageMultiplier = 0.35f;
    PestParams.WarningTime = 300.0f; // 5 minutes
    PestParams.bHasWarning = true;
    PestParams.RecoveryTime = 1800.0f; // 30 minutes
    DisasterTemplates.Add(ENaturalDisasterType::PestInfestation, PestParams);
    
    // Blizzard template
    FDisasterParameters BlizzardParams;
    BlizzardParams.DisasterType = ENaturalDisasterType::Blizzard;
    BlizzardParams.Severity = EDisasterSeverity::Moderate;
    BlizzardParams.BaseProbability = 0.025f;
    BlizzardParams.MaxIntensity = 1.0f;
    BlizzardParams.Duration = 2700.0f; // 45 minutes
    BlizzardParams.AffectedRadius = 7000.0f;
    BlizzardParams.DamageMultiplier = 0.4f;
    BlizzardParams.WarningTime = 240.0f; // 4 minutes
    BlizzardParams.bHasWarning = true;
    BlizzardParams.RecoveryTime = 3600.0f; // 1 hour
    DisasterTemplates.Add(ENaturalDisasterType::Blizzard, BlizzardParams);
    
    // Heat wave template
    FDisasterParameters HeatWaveParams;
    HeatWaveParams.DisasterType = ENaturalDisasterType::HeatWave;
    HeatWaveParams.Severity = EDisasterSeverity::Moderate;
    HeatWaveParams.BaseProbability = 0.03f;
    HeatWaveParams.MaxIntensity = 1.0f;
    HeatWaveParams.Duration = 5400.0f; // 1.5 hours
    HeatWaveParams.AffectedRadius = 8000.0f;
    HeatWaveParams.DamageMultiplier = 0.25f;
    HeatWaveParams.WarningTime = 600.0f; // 10 minutes
    HeatWaveParams.bHasWarning = true;
    HeatWaveParams.RecoveryTime = 1800.0f; // 30 minutes
    DisasterTemplates.Add(ENaturalDisasterType::HeatWave, HeatWaveParams);
    
    // Cold wave template
    FDisasterParameters ColdWaveParams;
    ColdWaveParams.DisasterType = ENaturalDisasterType::ColdWave;
    ColdWaveParams.Severity = EDisasterSeverity::Moderate;
    ColdWaveParams.BaseProbability = 0.025f;
    ColdWaveParams.MaxIntensity = 1.0f;
    ColdWaveParams.Duration = 3600.0f; // 1 hour
    ColdWaveParams.AffectedRadius = 9000.0f;
    ColdWaveParams.DamageMultiplier = 0.3f;
    ColdWaveParams.WarningTime = 300.0f; // 5 minutes
    ColdWaveParams.bHasWarning = true;
    ColdWaveParams.RecoveryTime = 2400.0f; // 40 minutes
    DisasterTemplates.Add(ENaturalDisasterType::ColdWave, ColdWaveParams);
}

FString UMingEcosystemDisasterSystem::TriggerNaturalDisaster(ENaturalDisasterType Type, EDisasterSeverity Severity, const FVector& Location)
{
    FString DisasterID = FGuid::NewGuid().ToString();
    
    FActiveDisaster NewDisaster;
    NewDisaster.DisasterID = DisasterID;
    
    // Copy template parameters
    if (DisasterTemplates.Contains(Type))
    {
        NewDisaster.Parameters = DisasterTemplates[Type];
    }
    
    // Apply severity modifications
    NewDisaster.Parameters.DisasterType = Type;
    NewDisaster.Parameters.Severity = Severity;
    NewDisaster.Parameters.CurrentPhase = EDisasterPhase::Building;
    
    // Modify based on severity
    switch (Severity)
    {
    case EDisasterSeverity::Minor:
        NewDisaster.Parameters.DamageMultiplier *= 0.3f;
        NewDisaster.Parameters.AffectedRadius *= 0.5f;
        NewDisaster.Parameters.Duration *= 0.5f;
        break;
    case EDisasterSeverity::Moderate:
        NewDisaster.Parameters.DamageMultiplier *= 0.6f;
        break;
    case EDisasterSeverity::Severe:
        NewDisaster.Parameters.DamageMultiplier *= 1.0f;
        break;
    case EDisasterSeverity::Catastrophic:
        NewDisaster.Parameters.DamageMultiplier *= 1.5f;
        NewDisaster.Parameters.AffectedRadius *= 1.5f;
        break;
    case EDisasterSeverity::Apocalyptic:
        NewDisaster.Parameters.DamageMultiplier *= 2.0f;
        NewDisaster.Parameters.AffectedRadius *= 2.0f;
        NewDisaster.Parameters.Duration *= 1.5f;
        break;
    }
    
    NewDisaster.Epicenter = Location;
    NewDisaster.StartTime = CurrentGameTime;
    NewDisaster.CurrentTime = 0.0f;
    NewDisaster.Progress = 0.0f;
    NewDisaster.Parameters.CurrentIntensity = 0.0f;
    
    ActiveDisasters.Add(DisasterID, NewDisaster);
    
    OnDisasterStarted.Broadcast(NewDisaster);
    
    // Generate warning if applicable
    if (NewDisaster.Parameters.bHasWarning)
    {
        GenerateDisasterWarning(NewDisaster);
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemDisasterSystem: Disaster %s triggered - Type: %d, Severity: %d"),
        *DisasterID, static_cast<int32>(Type), static_cast<int32>(Severity));
    
    return DisasterID;
}

void UMingEcosystemDisasterSystem::EndDisaster(const FString& DisasterID)
{
    if (!ActiveDisasters.Contains(DisasterID))
    {
        return;
    }
    
    FActiveDisaster Disaster = ActiveDisasters[DisasterID];
    Disaster.Parameters.CurrentPhase = EDisasterPhase::Complete;
    
    // Archive the disaster
    ArchiveDisaster(Disaster);
    
    OnDisasterEnded.Broadcast(Disaster);
    
    ActiveDisasters.Remove(DisasterID);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemDisasterSystem: Disaster %s ended"), *DisasterID);
}

FActiveDisaster UMingEcosystemDisasterSystem::GetActiveDisaster(const FString& DisasterID) const
{
    if (ActiveDisasters.Contains(DisasterID))
    {
        return ActiveDisasters[DisasterID];
    }
    return FActiveDisaster();
}

TArray<FString> UMingEcosystemDisasterSystem::GetAllActiveDisasters() const
{
    TArray<FString> DisasterIDs;
    ActiveDisasters.GetKeys(DisasterIDs);
    return DisasterIDs;
}

void UMingEcosystemDisasterSystem::ModifyDisasterIntensity(const FString& DisasterID, float NewIntensity)
{
    if (!ActiveDisasters.Contains(DisasterID))
    {
        return;
    }
    
    FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
    Disaster.Parameters.CurrentIntensity = FMath::Clamp(NewIntensity, 0.0f, Disaster.Parameters.MaxIntensity);
    
    OnDisasterProgressed.Broadcast(Disaster);
}

void UMingEcosystemDisasterSystem::SimulateDisasterProgression(const FString& DisasterID, float DeltaTime)
{
    if (!ActiveDisasters.Contains(DisasterID))
    {
        return;
    }
    
    FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
    
    // Update current time
    Disaster.CurrentTime += DeltaTime;
    
    // Calculate progress
    Disaster.Progress = FMath::Clamp(Disaster.CurrentTime / Disaster.Parameters.Duration, 0.0f, 1.0f);
    
    // Update phase
    UpdateDisasterPhase(Disaster);
    
    // Calculate intensity based on phase and progress
    float TargetIntensity = 0.0f;
    switch (Disaster.Parameters.CurrentPhase)
    {
    case EDisasterPhase::Building:
        TargetIntensity = Disaster.Progress * Disaster.Parameters.MaxIntensity;
        break;
    case EDisasterPhase::Active:
    case EDisasterPhase::Peak:
        TargetIntensity = Disaster.Parameters.MaxIntensity;
        break;
    case EDisasterPhase::Declining:
        TargetIntensity = (1.0f - Disaster.Progress) * Disaster.Parameters.MaxIntensity;
        break;
    default:
        TargetIntensity = 0.0f;
        break;
    }
    
    Disaster.Parameters.CurrentIntensity = TargetIntensity;
    
    // Calculate damage
    CalculateDisasterDamage(Disaster);
    
    // Affect nearby zones
    AffectNearbyZones(Disaster);
    
    // Apply mitigation effects
    ProcessMitigationEffects(Disaster);
    
    OnDisasterProgressed.Broadcast(Disaster);
}

void UMingEcosystemDisasterSystem::ProcessDisasterEffects(const FString& DisasterID)
{
    if (!ActiveDisasters.Contains(DisasterID))
    {
        return;
    }
    
    const FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
    
    // Process effects based on disaster type
    switch (Disaster.Parameters.DisasterType)
    {
    case ENaturalDisasterType::Drought:
        // TODO: Reduce water resources, increase fire risk
        break;
    case ENaturalDisasterType::Flood:
        // TODO: Damage to buildings, infrastructure
        break;
    case ENaturalDisasterType::Earthquake:
        // TODO: Structural damage, landslide triggers
        break;
    case ENaturalDisasterType::Wildfire:
        // TODO: Destroy vegetation, spread to nearby areas
        break;
    default:
        break;
    }
}

FDisasterImpact UMingEcosystemDisasterSystem::CalculateDisasterImpact(const FString& DisasterID, const FString& ZoneID)
{
    FDisasterImpact Impact;
    
    if (!ActiveDisasters.Contains(DisasterID))
    {
        return Impact;
    }
    
    const FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
    
    Impact.ZoneID = ZoneID;
    Impact.DisasterType = Disaster.Parameters.DisasterType;
    Impact.DamageSeverity = Disaster.Parameters.CurrentIntensity * Disaster.Parameters.DamageMultiplier;
    
    // Calculate species casualties
    float BaseCasualtyRate = Impact.DamageSeverity * 0.1f;
    // TODO: Get zone population data and calculate casualties
    
    // Calculate resource depletion
    switch (Disaster.Parameters.DisasterType)
    {
    case ENaturalDisasterType::Drought:
        Impact.ResourceDepletion.Add(EResourceType::Water, Impact.DamageSeverity * 0.5f);
        break;
    case ENaturalDisasterType::Wildfire:
        Impact.ResourceDepletion.Add(EResourceType::OrganicMatter, Impact.DamageSeverity * 0.7f);
        break;
    default:
        break;
    }
    
    // Infrastructure damage
    Impact.InfrastructureDamage = Impact.DamageSeverity * 0.3f;
    
    // Recovery time
    Impact.EstimatedRecoveryTime = Disaster.Parameters.RecoveryTime * (1.0f + Impact.DamageSeverity);
    
    return Impact;
}

TArray<FDisasterPrediction> UMingEcosystemDisasterSystem::GeneratePredictions(const FString& ZoneID)
{
    TArray<FDisasterPrediction> Predictions;
    
    // Check for environmental triggers
    CheckEnvironmentalTriggers(ZoneID);
    
    // Generate predictions for each disaster type
    for (const auto& Template : DisasterTemplates)
    {
        float Probability = CalculateDisasterProbability(Template.Key, ZoneID);
        
        if (Probability > 0.1f) // Only include meaningful predictions
        {
            FDisasterPrediction Prediction;
            Prediction.PredictedDisaster = Template.Key;
            Prediction.Probability = Probability;
            Prediction.ExpectedTime = CurrentGameTime + FMath::FRand() * 3600.0f; // Within next hour
            Prediction.PredictedSeverity = EDisasterSeverity::Moderate;
            Prediction.ConfidenceLevel = FMath::Clamp(Probability * 2.0f, 0.0f, 1.0f);
            
            // Generate warning message
            switch (Template.Key)
            {
            case ENaturalDisasterType::Drought:
                Prediction.WarningMessage = TEXT("环境监测显示干旱风险增加");
                Prediction.ContributingFactors.Add(TEXT("降水不足"));
                Prediction.ContributingFactors.Add(TEXT("高温持续"));
                break;
            case ENaturalDisasterType::Flood:
                Prediction.WarningMessage = TEXT("洪水预警：水位上升");
                Prediction.ContributingFactors.Add(TEXT("持续降雨"));
                Prediction.ContributingFactors.Add(TEXT("河流水位上涨"));
                break;
            case ENaturalDisasterType::Wildfire:
                Prediction.WarningMessage = TEXT("火灾风险升高");
                Prediction.ContributingFactors.Add(TEXT("干旱条件"));
                Prediction.ContributingFactors.Add(TEXT("风速较高"));
                break;
            default:
                Prediction.WarningMessage = TEXT("自然灾害风险增加");
                break;
            }
            
            Predictions.Add(Prediction);
        }
    }
    
    return Predictions;
}

float UMingEcosystemDisasterSystem::CalculateDisasterProbability(ENaturalDisasterType Type, const FString& ZoneID)
{
    float BaseProbability = 0.0f;
    
    if (DisasterTemplates.Contains(Type))
    {
        BaseProbability = DisasterTemplates[Type].BaseProbability;
    }
    
    // Modify based on environmental conditions
    if (ZoneEnvironmentalConditions.Contains(ZoneID))
    {
        const auto& Conditions = ZoneEnvironmentalConditions[ZoneID];
        
        switch (Type)
        {
        case ENaturalDisasterType::Drought:
            if (Conditions.Contains(TEXT("Humidity")))
            {
                float Humidity = Conditions[TEXT("Humidity")];
                if (Humidity < 30.0f)
                {
                    BaseProbability *= 3.0f;
                }
            }
            if (Conditions.Contains(TEXT("Temperature")))
            {
                float Temp = Conditions[TEXT("Temperature")];
                if (Temp > 35.0f)
                {
                    BaseProbability *= 2.0f;
                }
            }
            break;
            
        case ENaturalDisasterType::Flood:
            if (Conditions.Contains(TEXT("Humidity")))
            {
                float Humidity = Conditions[TEXT("Humidity")];
                if (Humidity > 80.0f)
                {
                    BaseProbability *= 2.5f;
                }
            }
            break;
            
        case ENaturalDisasterType::Wildfire:
            if (Conditions.Contains(TEXT("Temperature")))
            {
                float Temp = Conditions[TEXT("Temperature")];
                if (Temp > 30.0f)
                {
                    BaseProbability *= 2.0f;
                }
            }
            if (Conditions.Contains(TEXT("Humidity")))
            {
                float Humidity = Conditions[TEXT("Humidity")];
                if (Humidity < 40.0f)
                {
                    BaseProbability *= 2.0f;
                }
            }
            break;
            
        default:
            break;
        }
    }
    
    return FMath::Clamp(BaseProbability, 0.0f, 1.0f);
}

FClimateRiskAssessment UMingEcosystemDisasterSystem::AssessClimateRisk(const FString& ZoneID)
{
    FClimateRiskAssessment Assessment;
    Assessment.ZoneID = ZoneID;
    
    // Calculate individual risks
    Assessment.DroughtRisk = CalculateDisasterProbability(ENaturalDisasterType::Drought, ZoneID);
    Assessment.FloodRisk = CalculateDisasterProbability(ENaturalDisasterType::Flood, ZoneID);
    Assessment.EarthquakeRisk = CalculateDisasterProbability(ENaturalDisasterType::Earthquake, ZoneID);
    Assessment.FireRisk = CalculateDisasterProbability(ENaturalDisasterType::Wildfire, ZoneID);
    Assessment.StormRisk = CalculateDisasterProbability(ENaturalDisasterType::Hurricane, ZoneID);
    
    // Calculate overall risk
    Assessment.OverallRiskLevel = (Assessment.DroughtRisk + Assessment.FloodRisk + 
                                  Assessment.EarthquakeRisk + Assessment.FireRisk + 
                                  Assessment.StormRisk) / 5.0f;
    
    // Identify risk factors
    if (Assessment.DroughtRisk > 0.3f)
    {
        Assessment.RiskFactors.Add(TEXT("干旱风险"));
    }
    if (Assessment.FloodRisk > 0.3f)
    {
        Assessment.RiskFactors.Add(TEXT("洪水风险"));
    }
    if (Assessment.EarthquakeRisk > 0.2f)
    {
        Assessment.RiskFactors.Add(TEXT("地震风险"));
    }
    if (Assessment.FireRisk > 0.3f)
    {
        Assessment.RiskFactors.Add(TEXT("火灾风险"));
    }
    
    Assessment.AssessmentDate = FDateTime::Now().ToString();
    
    return Assessment;
}

void UMingEcosystemDisasterSystem::IssueEarlyWarning(const FDisasterPrediction& Prediction)
{
    float TimeUntil = Prediction.ExpectedTime - CurrentGameTime;
    
    OnDisasterWarning.Broadcast(Prediction, TimeUntil);
    
    UE_LOG(LogTemp, Warning, TEXT("MingEcosystemDisasterSystem: Early warning issued - %s in %.0f seconds"),
        *Prediction.WarningMessage, TimeUntil);
}

void UMingEcosystemDisasterSystem::CheckEnvironmentalTriggers(const FString& ZoneID)
{
    UpdateEnvironmentalConditions(ZoneID, 0.0f);
    EvaluateRiskFactors(ZoneID);
}

void UMingEcosystemDisasterSystem::RegisterEnvironmentalCondition(const FString& ZoneID, const FString& Condition, float Value)
{
    if (!ZoneEnvironmentalConditions.Contains(ZoneID))
    {
        ZoneEnvironmentalConditions.Add(ZoneID, TMap<FString, float>());
    }
    
    ZoneEnvironmentalConditions[ZoneID].Add(Condition, Value);
}

void UMingEcosystemDisasterSystem::EvaluateDisasterTriggers(const FString& ZoneID)
{
    // Check for automatic disaster triggers based on conditions
    TArray<FDisasterPrediction> Predictions = GeneratePredictions(ZoneID);
    
    for (const FDisasterPrediction& Prediction : Predictions)
    {
        if (Prediction.Probability > 0.7f && Prediction.ExpectedTime - CurrentGameTime < 60.0f)
        {
            // High probability and imminent - auto-trigger
            TriggerNaturalDisaster(Prediction.PredictedDisaster, Prediction.PredictedSeverity, FVector::ZeroVector);
        }
    }
}

FString UMingEcosystemDisasterSystem::AddMitigationMeasure(ENaturalDisasterType TargetDisaster, const FString& Type, const FVector& Location)
{
    FString MitigationID = FGuid::NewGuid().ToString();
    
    FDisasterMitigation Mitigation;
    Mitigation.MitigationID = MitigationID;
    Mitigation.TargetDisaster = TargetDisaster;
    Mitigation.MitigationType = Type;
    Mitigation.Location = Location;
    
    // Set effectiveness based on type
    if (Type == TEXT("Early Warning System"))
    {
        Mitigation.Effectiveness = 0.3f;
        Mitigation.Cost = 5000.0f;
    }
    else if (Type == TEXT("Flood Barrier"))
    {
        Mitigation.Effectiveness = 0.6f;
        Mitigation.Cost = 10000.0f;
    }
    else if (Type == TEXT("Firebreak"))
    {
        Mitigation.Effectiveness = 0.5f;
        Mitigation.Cost = 3000.0f;
    }
    else if (Type == TEXT("Earthquake Resistant Buildings"))
    {
        Mitigation.Effectiveness = 0.7f;
        Mitigation.Cost = 15000.0f;
    }
    else
    {
        Mitigation.Effectiveness = 0.4f;
        Mitigation.Cost = 5000.0f;
    }
    
    MitigationMeasures.Add(MitigationID, Mitigation);
    
    return MitigationID;
}

void UMingEcosystemDisasterSystem::ActivateMitigation(const FString& MitigationID)
{
    if (MitigationMeasures.Contains(MitigationID))
    {
        MitigationMeasures[MitigationID].bIsActive = true;
    }
}

void UMingEcosystemDisasterSystem::DeactivateMitigation(const FString& MitigationID)
{
    if (MitigationMeasures.Contains(MitigationID))
    {
        MitigationMeasures[MitigationID].bIsActive = false;
    }
}

float UMingEcosystemDisasterSystem::CalculateMitigationEffectiveness(const FString& MitigationID, const FString& DisasterID)
{
    if (!MitigationMeasures.Contains(MitigationID) || !ActiveDisasters.Contains(DisasterID))
    {
        return 0.0f;
    }
    
    const FDisasterMitigation& Mitigation = MitigationMeasures[MitigationID];
    
    if (!Mitigation.bIsActive)
    {
        return 0.0f;
    }
    
    // Check if disaster type matches
    const FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
    if (Mitigation.TargetDisaster != Disaster.Parameters.DisasterType)
    {
        return 0.0f;
    }
    
    // Calculate distance-based effectiveness
    float Distance = CalculateDistanceToDisaster(Mitigation.Location, Disaster);
    float DistanceFactor = FMath::Max(0.0f, 1.0f - (Distance / Mitigation.CoverageRadius));
    
    return Mitigation.Effectiveness * DistanceFactor;
}

TArray<FDisasterMitigation> UMingEcosystemDisasterSystem::GetActiveMitigations(const FString& ZoneID) const
{
    TArray<FDisasterMitigation> ActiveMitigations;
    
    for (const auto& MitigationPair : MitigationMeasures)
    {
        if (MitigationPair.Value.bIsActive)
        {
            ActiveMitigations.Add(MitigationPair.Value);
        }
    }
    
    return ActiveMitigations;
}

void UMingEcosystemDisasterSystem::StartRecovery(const FString& ZoneID, const FString& DisasterID)
{
    FString Key = ZoneID + TEXT("_") + DisasterID;
    RecoveryProgress.Add(Key, 0.0f);
    
    UE_LOG(LogTemp, Log, TEXT("MingEcosystemDisasterSystem: Started recovery for zone %s from disaster %s"),
        *ZoneID, *DisasterID);
}

void UMingEcosystemDisasterSystem::ProcessRecovery(const FString& ZoneID, float DeltaTime)
{
    // Process recovery for all active recoveries in this zone
    TArray<FString> KeysToRemove;
    
    for (auto& ProgressPair : RecoveryProgress)
    {
        if (ProgressPair.Key.StartsWith(ZoneID))
        {
            ProgressPair.Value += DeltaTime / 3600.0f; // Assume 1 hour recovery time
            
            float Progress = FMath::Clamp(ProgressPair.Value, 0.0f, 1.0f);
            OnRecoveryProgress.Broadcast(ZoneID, Progress * 100.0f);
            
            if (Progress >= 1.0f)
            {
                KeysToRemove.Add(ProgressPair.Key);
            }
        }
    }
    
    for (const FString& Key : KeysToRemove)
    {
        RecoveryProgress.Remove(Key);
    }
}

float UMingEcosystemDisasterSystem::GetRecoveryProgress(const FString& ZoneID) const
{
    float TotalProgress = 0.0f;
    int32 Count = 0;
    
    for (const auto& ProgressPair : RecoveryProgress)
    {
        if (ProgressPair.Key.StartsWith(ZoneID))
        {
            TotalProgress += ProgressPair.Value;
            Count++;
        }
    }
    
    return Count > 0 ? TotalProgress / Count : 0.0f;
}

void UMingEcosystemDisasterSystem::AccelerateRecovery(const FString& ZoneID, float AccelerationFactor)
{
    for (auto& ProgressPair : RecoveryProgress)
    {
        if (ProgressPair.Key.StartsWith(ZoneID))
        {
            ProgressPair.Value *= AccelerationFactor;
        }
    }
}

void UMingEcosystemDisasterSystem::ApplyDamageToZone(const FString& ZoneID, const FDisasterImpact& Impact)
{
    ZoneImpacts.Add(ZoneID, Impact);
    OnDisasterImpact.Broadcast(Impact);
}

void UMingEcosystemDisasterSystem::ApplySpeciesDamage(const FString& ZoneID, ESpeciesType Species, int32 Casualties)
{
    if (ZoneImpacts.Contains(ZoneID))
    {
        FDisasterImpact& Impact = ZoneImpacts[ZoneID];
        Impact.SpeciesCasualties.Add(Species, Casualties);
    }
}

void UMingEcosystemDisasterSystem::ApplyResourceDamage(const FString& ZoneID, EResourceType Resource, float Depletion)
{
    if (ZoneImpacts.Contains(ZoneID))
    {
        FDisasterImpact& Impact = ZoneImpacts[ZoneID];
        Impact.ResourceDepletion.Add(Resource, Depletion);
    }
}

void UMingEcosystemDisasterSystem::ApplyInfrastructureDamage(const FString& ZoneID, float DamageAmount)
{
    if (ZoneImpacts.Contains(ZoneID))
    {
        FDisasterImpact& Impact = ZoneImpacts[ZoneID];
        Impact.InfrastructureDamage += DamageAmount;
    }
}

void UMingEcosystemDisasterSystem::CreateEarthquake(const FVector& Epicenter, float Magnitude, float Depth)
{
    EDisasterSeverity Severity = EDisasterSeverity::Moderate;
    if (Magnitude < 4.0f)
    {
        Severity = EDisasterSeverity::Minor;
    }
    else if (Magnitude < 6.0f)
    {
        Severity = EDisasterSeverity::Moderate;
    }
    else if (Magnitude < 7.0f)
    {
        Severity = EDisasterSeverity::Severe;
    }
    else if (Magnitude < 8.0f)
    {
        Severity = EDisasterSeverity::Catastrophic;
    }
    else
    {
        Severity = EDisasterSeverity::Apocalyptic;
    }
    
    FString DisasterID = TriggerNaturalDisaster(ENaturalDisasterType::Earthquake, Severity, Epicenter);
    
    // Modify earthquake-specific parameters
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.AffectedRadius = FMath::Min(50000.0f, 10000.0f * (Magnitude / 5.0f));
        Disaster.Parameters.DamageMultiplier = Magnitude / 5.0f;
    }
}

void UMingEcosystemDisasterSystem::CreateFlood(const FVector& Epicenter, float WaterLevel, float Duration)
{
    EDisasterSeverity Severity = EDisasterSeverity::Moderate;
    if (WaterLevel < 1.0f)
    {
        Severity = EDisasterSeverity::Minor;
    }
    else if (WaterLevel < 3.0f)
    {
        Severity = EDisasterSeverity::Moderate;
    }
    else if (WaterLevel < 5.0f)
    {
        Severity = EDisasterSeverity::Severe;
    }
    else if (WaterLevel < 8.0f)
    {
        Severity = EDisasterSeverity::Catastrophic;
    }
    else
    {
        Severity = EDisasterSeverity::Apocalyptic;
    }
    
    FString DisasterID = TriggerNaturalDisaster(ENaturalDisasterType::Flood, Severity, Epicenter);
    
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.Duration = Duration;
        Disaster.Parameters.AffectedRadius = WaterLevel * 1000.0f;
    }
}

void UMingEcosystemDisasterSystem::CreateDrought(const FString& ZoneID, float Severity, float Duration)
{
    EDisasterSeverity DisasterSeverity = EDisasterSeverity::Moderate;
    if (Severity < 0.3f)
    {
        DisasterSeverity = EDisasterSeverity::Minor;
    }
    else if (Severity < 0.5f)
    {
        DisasterSeverity = EDisasterSeverity::Moderate;
    }
    else if (Severity < 0.7f)
    {
        DisasterSeverity = EDisasterSeverity::Severe;
    }
    else if (Severity < 0.9f)
    {
        DisasterSeverity = EDisasterSeverity::Catastrophic;
    }
    else
    {
        DisasterSeverity = EDisasterSeverity::Apocalyptic;
    }
    
    FString DisasterID = TriggerNaturalDisaster(ENaturalDisasterType::Drought, DisasterSeverity, FVector::ZeroVector);
    
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.Duration = Duration;
    }
}

void UMingEcosystemDisasterSystem::CreateWildfire(const FVector& Epicenter, float SpreadRate, float Intensity)
{
    EDisasterSeverity Severity = EDisasterSeverity::Moderate;
    if (Intensity < 0.3f)
    {
        Severity = EDisasterSeverity::Minor;
    }
    else if (Intensity < 0.5f)
    {
        Severity = EDisasterSeverity::Moderate;
    }
    else if (Intensity < 0.7f)
    {
        Severity = EDisasterSeverity::Severe;
    }
    else if (Intensity < 0.9f)
    {
        Severity = EDisasterSeverity::Catastrophic;
    }
    else
    {
        Severity = EDisasterSeverity::Apocalyptic;
    }
    
    FString DisasterID = TriggerNaturalDisaster(ENaturalDisasterType::Wildfire, Severity, Epicenter);
    
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.AffectedRadius = SpreadRate * 5000.0f;
        Disaster.Parameters.DamageMultiplier = Intensity;
    }
}

void UMingEcosystemDisasterSystem::CreateStorm(const FVector& Epicenter, float WindSpeed, float Precipitation)
{
    EDisasterSeverity Severity = EDisasterSeverity::Moderate;
    if (WindSpeed < 50.0f)
    {
        Severity = EDisasterSeverity::Minor;
    }
    else if (WindSpeed < 100.0f)
    {
        Severity = EDisasterSeverity::Moderate;
    }
    else if (WindSpeed < 150.0f)
    {
        Severity = EDisasterSeverity::Severe;
    }
    else if (WindSpeed < 200.0f)
    {
        Severity = EDisasterSeverity::Catastrophic;
    }
    else
    {
        Severity = EDisasterSeverity::Apocalyptic;
    }
    
    // Determine disaster type based on conditions
    ENaturalDisasterType StormType = ENaturalDisasterType::Hurricane;
    if (WindSpeed > 200.0f && FMath::FRand() < 0.3f)
    {
        StormType = ENaturalDisasterType::Tornado;
    }
    
    FString DisasterID = TriggerNaturalDisaster(StormType, Severity, Epicenter);
    
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.AffectedRadius = WindSpeed * 50.0f;
    }
}

float UMingEcosystemDisasterSystem::GetTotalDamageCost() const
{
    float TotalCost = 0.0f;
    
    for (const auto& Disaster : DisasterHistory)
    {
        TotalCost += Disaster.EconomicLoss;
    }
    
    for (const auto& Disaster : ActiveDisasters)
    {
        TotalCost += Disaster.Value.EconomicLoss;
    }
    
    return TotalCost;
}

int32 UMingEcosystemDisasterSystem::GetTotalCasualties() const
{
    int32 TotalCasualties = 0;
    
    for (const auto& Disaster : DisasterHistory)
    {
        TotalCasualties += Disaster.AffectedPopulation;
    }
    
    for (const auto& Disaster : ActiveDisasters)
    {
        TotalCasualties += Disaster.Value.AffectedPopulation;
    }
    
    return TotalCasualties;
}

void UMingEcosystemDisasterSystem::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    CurrentGameTime += DeltaTime;
    
    // Process active disasters
    TArray<FString> DisastersToRemove;
    
    for (auto& DisasterPair : ActiveDisasters)
    {
        FString DisasterID = DisasterPair.Key;
        SimulateDisasterProgression(DisasterID, DeltaTime);
        
        // Check if disaster should end
        if (IsDisasterExpired(DisasterPair.Value))
        {
            DisastersToRemove.Add(DisasterID);
        }
    }
    
    // End expired disasters
    for (const FString& DisasterID : DisastersToRemove)
    {
        EndDisaster(DisasterID);
    }
    
    // Process recovery for all zones with impacts
    TArray<FString> ZoneIDs;
    ZoneImpacts.GetKeys(ZoneIDs);
    for (const FString& ZoneID : ZoneIDs)
    {
        ProcessRecovery(ZoneID, DeltaTime);
    }
}

void UMingEcosystemDisasterSystem::UpdateDisasterPhase(FActiveDisaster& Disaster)
{
    float PhaseProgress = Disaster.Progress;
    
    if (PhaseProgress < 0.1f)
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Building;
    }
    else if (PhaseProgress < 0.3f)
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Active;
    }
    else if (PhaseProgress < 0.6f)
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Peak;
    }
    else if (PhaseProgress < 0.9f)
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Declining;
    }
    else if (PhaseProgress < 1.0f)
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Aftermath;
    }
    else
    {
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Complete;
    }
}

void UMingEcosystemDisasterSystem::CalculateDisasterDamage(FActiveDisaster& Disaster)
{
    // Calculate economic loss based on intensity, duration, and affected area
    float AreaFactor = FMath::PI * FMath::Square(Disaster.Parameters.AffectedRadius / 1000.0f);
    float IntensityFactor = Disaster.Parameters.CurrentIntensity * Disaster.Parameters.DamageMultiplier;
    float DurationFactor = FMath::Min(Disaster.CurrentTime / 3600.0f, 10.0f); // Cap at 10 hours
    
    Disaster.EconomicLoss = AreaFactor * IntensityFactor * DurationFactor * 1000.0f;
    Disaster.TotalDamage = IntensityFactor * 100.0f;
    
    // Affected population estimate
    Disaster.AffectedPopulation = FMath::RoundToInt(AreaFactor * IntensityFactor * 100.0f);
}

void UMingEcosystemDisasterSystem::AffectNearbyZones(FActiveDisaster& Disaster)
{
    // TODO: Get zones from ecosystem system and check distance
    // For now, this is a placeholder for zone interaction logic
}

float UMingEcosystemDisasterSystem::CalculateDistanceToDisaster(const FVector& Location, const FActiveDisaster& Disaster) const
{
    return FVector::Distance(Location, Disaster.Epicenter);
}

float UMingEcosystemDisasterSystem::CalculateDamageAtDistance(float Distance, const FActiveDisaster& Disaster) const
{
    float NormalizedDistance = Distance / Disaster.Parameters.AffectedRadius;
    
    // Damage decreases with distance using inverse square law
    float DamageFactor = FMath::Max(0.0f, 1.0f - FMath::Square(NormalizedDistance));
    
    return DamageFactor * Disaster.Parameters.CurrentIntensity * Disaster.Parameters.DamageMultiplier;
}

void UMingEcosystemDisasterSystem::InitializeDisasterParameters(FDisasterParameters& Params)
{
    // Set default values
    Params.CurrentPhase = EDisasterPhase::Dormant;
    Params.CurrentIntensity = 0.0f;
    Params.EnvironmentalConditions.Empty();
}

void UMingEcosystemDisasterSystem::ApplyEnvironmentalModifiers(FDisasterParameters& Params, const FString& ZoneID)
{
    if (!ZoneEnvironmentalConditions.Contains(ZoneID))
    {
        return;
    }
    
    const auto& Conditions = ZoneEnvironmentalConditions[ZoneID];
    
    // Apply modifiers based on disaster type
    switch (Params.DisasterType)
    {
    case ENaturalDisasterType::Wildfire:
        if (Conditions.Contains(TEXT("Temperature")))
        {
            float Temp = Conditions[TEXT("Temperature")];
            if (Temp > 35.0f)
            {
                Params.MaxIntensity *= 1.2f;
            }
        }
        break;
        
    case ENaturalDisasterType::Flood:
        if (Conditions.Contains(TEXT("Humidity")))
        {
            float Humidity = Conditions[TEXT("Humidity")];
            if (Humidity > 90.0f)
            {
                Params.Duration *= 1.5f;
            }
        }
        break;
        
    default:
        break;
    }
}

bool UMingEcosystemDisasterSystem::ShouldTriggerWarning(const FActiveDisaster& Disaster) const
{
    return Disaster.Parameters.bHasWarning && 
           Disaster.Parameters.WarningTime > 0.0f &&
           Disaster.Parameters.CurrentPhase == EDisasterPhase::Building;
}

void UMingEcosystemDisasterSystem::GenerateDisasterWarning(const FActiveDisaster& Disaster)
{
    FDisasterPrediction Prediction;
    Prediction.PredictedDisaster = Disaster.Parameters.DisasterType;
    Prediction.Probability = 1.0f;
    Prediction.ExpectedTime = Disaster.StartTime + Disaster.Parameters.WarningTime;
    Prediction.PredictedLocation = Disaster.Epicenter;
    Prediction.PredictedSeverity = Disaster.Parameters.Severity;
    Prediction.ConfidenceLevel = 0.9f;
    
    switch (Disaster.Parameters.DisasterType)
    {
    case ENaturalDisasterType::Earthquake:
        Prediction.WarningMessage = TEXT("地震预警：预计有感地震即将发生");
        break;
    case ENaturalDisasterType::Flood:
        Prediction.WarningMessage = TEXT("洪水预警：水位即将达到危险水平");
        break;
    case ENaturalDisasterType::Wildfire:
        Prediction.WarningMessage = TEXT("火灾预警：火险等级极高");
        break;
    case ENaturalDisasterType::Hurricane:
        Prediction.WarningMessage = TEXT("飓风预警：强风暴即将来袭");
        break;
    default:
        Prediction.WarningMessage = TEXT("自然灾害预警：请做好防护准备");
        break;
    }
    
    IssueEarlyWarning(Prediction);
}

void UMingEcosystemDisasterSystem::CompleteDisaster(const FString& DisasterID)
{
    if (ActiveDisasters.Contains(DisasterID))
    {
        FActiveDisaster& Disaster = ActiveDisasters[DisasterID];
        Disaster.Parameters.CurrentPhase = EDisasterPhase::Complete;
        EndDisaster(DisasterID);
    }
}

void UMingEcosystemDisasterSystem::ArchiveDisaster(const FActiveDisaster& Disaster)
{
    DisasterHistory.Add(Disaster);
}

bool UMingEcosystemDisasterSystem::IsDisasterExpired(const FActiveDisaster& Disaster) const
{
    return Disaster.CurrentTime >= Disaster.Parameters.Duration ||
           Disaster.Parameters.CurrentPhase == EDisasterPhase::Complete;
}

void UMingEcosystemDisasterSystem::ProcessMitigationEffects(FActiveDisaster& Disaster)
{
    float TotalMitigation = 0.0f;
    
    for (const auto& MitigationPair : MitigationMeasures)
    {
        float Effectiveness = CalculateMitigationEffectiveness(MitigationPair.Key, Disaster.DisasterID);
        TotalMitigation += Effectiveness;
    }
    
    // Cap total mitigation at 80%
    TotalMitigation = FMath::Min(TotalMitigation, 0.8f);
    
    // Apply mitigation to disaster
    Disaster.Parameters.DamageMultiplier *= (1.0f - TotalMitigation);
}

float UMingEcosystemDisasterSystem::GetDisasterDuration(ENaturalDisasterType Type) const
{
    if (DisasterTemplates.Contains(Type))
    {
        return DisasterTemplates[Type].Duration;
    }
    return 1800.0f;
}

float UMingEcosystemDisasterSystem::GetDisasterRadius(ENaturalDisasterType Type, EDisasterSeverity Severity) const
{
    if (DisasterTemplates.Contains(Type))
    {
        float BaseRadius = DisasterTemplates[Type].AffectedRadius;
        
        switch (Severity)
        {
        case EDisasterSeverity::Minor:
            return BaseRadius * 0.5f;
        case EDisasterSeverity::Moderate:
            return BaseRadius;
        case EDisasterSeverity::Severe:
            return BaseRadius * 1.2f;
        case EDisasterSeverity::Catastrophic:
            return BaseRadius * 1.5f;
        case EDisasterSeverity::Apocalyptic:
            return BaseRadius * 2.0f;
        default:
            return BaseRadius;
        }
    }
    return 1000.0f;
}

float UMingEcosystemDisasterSystem::GetDisasterProbability(ENaturalDisasterType Type, const FString& ZoneID) const
{
    float BaseProb = 0.0f;
    
    if (DisasterTemplates.Contains(Type))
    {
        BaseProb = DisasterTemplates[Type].BaseProbability;
    }
    
    // TODO: Modify based on zone characteristics
    
    return BaseProb;
}

void UMingEcosystemDisasterSystem::UpdateEnvironmentalConditions(const FString& ZoneID, float DeltaTime)
{
    // Update or initialize environmental conditions
    if (!ZoneEnvironmentalConditions.Contains(ZoneID))
    {
        ZoneEnvironmentalConditions.Add(ZoneID, TMap<FString, float>());
    }
    
    auto& Conditions = ZoneEnvironmentalConditions[ZoneID];
    
    // Set some default conditions if not present
    if (!Conditions.Contains(TEXT("Temperature")))
    {
        Conditions.Add(TEXT("Temperature"), 20.0f);
    }
    if (!Conditions.Contains(TEXT("Humidity")))
    {
        Conditions.Add(TEXT("Humidity"), 50.0f);
    }
    if (!Conditions.Contains(TEXT("WindSpeed")))
    {
        Conditions.Add(TEXT("WindSpeed"), 5.0f);
    }
    if (!Conditions.Contains(TEXT("Pressure")))
    {
        Conditions.Add(TEXT("Pressure"), 1013.0f);
    }
    if (!Conditions.Contains(TEXT("Rainfall")))
    {
        Conditions.Add(TEXT("Rainfall"), 0.0f);
    }
}

void UMingEcosystemDisasterSystem::EvaluateRiskFactors(const FString& ZoneID)
{
    if (!ZoneEnvironmentalConditions.Contains(ZoneID))
    {
        return;
    }
    
    // Evaluate risk based on current conditions
    const auto& Conditions = ZoneEnvironmentalConditions[ZoneID];
    
    // Check for drought risk
    if (Conditions.Contains(TEXT("Humidity")) && Conditions.Contains(TEXT("Temperature")))
    {
        float Humidity = Conditions[TEXT("Humidity")];
        float Temp = Conditions[TEXT("Temperature")];
        
        if (Humidity < 30.0f && Temp > 30.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("MingEcosystemDisasterSystem: Drought risk detected in zone %s"), *ZoneID);
        }
    }
    
    // Check for flood risk
    if (Conditions.Contains(TEXT("Humidity")) && Conditions.Contains(TEXT("Rainfall")))
    {
        float Humidity = Conditions[TEXT("Humidity")];
        float Rainfall = Conditions[TEXT("Rainfall")];
        
        if (Humidity > 85.0f && Rainfall > 50.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("MingEcosystemDisasterSystem: Flood risk detected in zone %s"), *ZoneID);
        }
    }
    
    // Check for fire risk
    if (Conditions.Contains(TEXT("Temperature")) && Conditions.Contains(TEXT("Humidity")))
    {
        float Temp = Conditions[TEXT("Temperature")];
        float Humidity = Conditions[TEXT("Humidity")];
        
        if (Temp > 35.0f && Humidity < 40.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("MingEcosystemDisasterSystem: Fire risk detected in zone %s"), *ZoneID);
        }
    }
}
