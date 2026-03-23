#include "SageBrain/MingCorruptionDetectionSystem.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UMingCorruptionDetectionSystem::UMingCorruptionDetectionSystem()
{
    bIsInitialized = false;
    EmergencyThreshold = 80.0f;
    CurrentMaxSeverity = ECorruptionSeverity::None;
    
    InitializeDefaultThresholds();
    InitializeDefaultMeasures();
}

void UMingCorruptionDetectionSystem::InitializeDetectionSystem()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Corruption Detection System already initialized"));
        return;
    }

    // 初始化徵象數據庫
    SignDatabase.Empty();
    DetectionHistory.Empty();
    
    for (uint8 i = static_cast<uint8>(ECorruptionSignDetail::MoralDecay_Light); 
         i <= static_cast<uint8>(ECorruptionSignDetail::GreedGrowth_Severe); ++i)
    {
        ECorruptionSignDetail SignType = static_cast<ECorruptionSignDetail>(i);
        FCorruptionSignData Data;
        Data.SignType = SignType;
        Data.ThresholdValue = GetDetectionThreshold(SignType);
        Data.bIsActive = false;
        
        SignDatabase.Add(SignType, Data);
    }
    
    bIsInitialized = true;
    CurrentMaxSeverity = ECorruptionSeverity::None;
    
    OnDetectionSystemInitialized();
    
    UE_LOG(LogTemp, Log, TEXT("Corruption Detection System initialized successfully"));
}

FDetectionResult UMingCorruptionDetectionSystem::DetectMoralDecay(float CurrentMorality, float HistoricalMorality)
{
    FDetectionResult Result;
    
    if (!bIsInitialized)
    {
        Result.AnalysisReport = TEXT("系統未初始化");
        return Result;
    }

    // 計算道德下降幅度
    float MoralityDrop = FMath::Max(0.0f, HistoricalMorality - CurrentMorality);
    float Threshold = GetDetectionThreshold(ECorruptionSignDetail::MoralDecay_Medium);
    
    if (ValidateDetectionParams(MoralityDrop, Threshold) && MoralityDrop > Threshold)
    {
        Result.bDetected = true;
        Result.ConfidenceLevel = CalculateConfidence(MoralityDrop, Threshold);
        Result.Severity = CalculateSeverity(MoralityDrop, Threshold);
        
        // 確定具體徵象類型
        if (Result.Severity == ECorruptionSeverity::Light)
        {
            Result.DetectedSign = ECorruptionSignDetail::MoralDecay_Light;
        }
        else if (Result.Severity == ECorruptionSeverity::Medium)
        {
            Result.DetectedSign = ECorruptionSignDetail::MoralDecay_Medium;
        }
        else
        {
            Result.DetectedSign = ECorruptionSignDetail::MoralDecay_Severe;
        }
        
        Result.AnalysisReport = GenerateAnalysisReport(Result.DetectedSign, Result.Severity);
        Result.RecommendedActions = GenerateRecommendations(Result.Severity);
        
        // 記錄檢測結果
        RecordDetection(Result);
        
        // 觸發事件
        FCorruptionSignData SignData;
        SignData.SignType = Result.DetectedSign;
        SignData.Severity = Result.Severity;
        SignData.DetectionValue = MoralityDrop;
        SignData.ThresholdValue = Threshold;
        SignData.Description = Result.AnalysisReport;
        SignData.WarningMessage = TEXT("檢測到道德淪喪徵象");
        SignData.TimeDetected = FDateTime::Now().ToUnixTimestamp();
        SignData.bIsActive = true;
        
        UpdateSignStatus(Result.DetectedSign, SignData);
        OnCorruptionSignDetected.Broadcast(SignData);
        
        UE_LOG(LogTemp, Warning, TEXT("Moral decay detected: %s (Severity: %d)"), 
               *Result.AnalysisReport, static_cast<int32>(Result.Severity));
    }
    else
    {
        Result.AnalysisReport = TEXT("未檢測到道德淪喪徵象");
        UE_LOG(LogTemp, Log, TEXT("No moral decay detected"));
    }
    
    return Result;
}

FDetectionResult UMingCorruptionDetectionSystem::DetectPowerAbuse(float PowerUsage, float AuthorizedLimit)
{
    FDetectionResult Result;
    
    if (!bIsInitialized)
    {
        Result.AnalysisReport = TEXT("系統未初始化");
        return Result;
    }

    // 計算權力使用超標程度
    float OverUsage = FMath::Max(0.0f, PowerUsage - AuthorizedLimit);
    float OverUsagePercent = (AuthorizedLimit > 0.0f) ? (OverUsage / AuthorizedLimit) * 100.0f : 0.0f;
    
    float Threshold = GetDetectionThreshold(ECorruptionSignDetail::PowerAbuse_Medium);
    
    if (ValidateDetectionParams(OverUsagePercent, Threshold) && OverUsagePercent > Threshold)
    {
        Result.bDetected = true;
        Result.ConfidenceLevel = CalculateConfidence(OverUsagePercent, Threshold);
        Result.Severity = CalculateSeverity(OverUsagePercent, Threshold);
        
        // 確定具體徵象類型
        if (Result.Severity == ECorruptionSeverity::Light)
        {
            Result.DetectedSign = ECorruptionSignDetail::PowerAbuse_Light;
        }
        else if (Result.Severity == ECorruptionSeverity::Medium)
        {
            Result.DetectedSign = ECorruptionSignDetail::PowerAbuse_Medium;
        }
        else
        {
            Result.DetectedSign = ECorruptionSignDetail::PowerAbuse_Severe;
        }
        
        Result.AnalysisReport = GenerateAnalysisReport(Result.DetectedSign, Result.Severity);
        Result.RecommendedActions = GenerateRecommendations(Result.Severity);
        
        RecordDetection(Result);
        
        FCorruptionSignData SignData;
        SignData.SignType = Result.DetectedSign;
        SignData.Severity = Result.Severity;
        SignData.DetectionValue = OverUsagePercent;
        SignData.ThresholdValue = Threshold;
        SignData.Description = Result.AnalysisReport;
        SignData.WarningMessage = TEXT("檢測到權力濫用徵象");
        SignData.TimeDetected = FDateTime::Now().ToUnixTimestamp();
        SignData.bIsActive = true;
        
        UpdateSignStatus(Result.DetectedSign, SignData);
        OnCorruptionSignDetected.Broadcast(SignData);
        
        UE_LOG(LogTemp, Warning, TEXT("Power abuse detected: %s (Severity: %d)"), 
               *Result.AnalysisReport, static_cast<int32>(Result.Severity));
    }
    else
    {
        Result.AnalysisReport = TEXT("未檢測到權力濫用徵象");
        UE_LOG(LogTemp, Log, TEXT("No power abuse detected"));
    }
    
    return Result;
}

FDetectionResult UMingCorruptionDetectionSystem::DetectGreedGrowth(float ResourceDesire, float ActualNeed)
{
    FDetectionResult Result;
    
    if (!bIsInitialized)
    {
        Result.AnalysisReport = TEXT("系統未初始化");
        return Result;
    }

    // 計算貪慾程度
    float GreedLevel = FMath::Max(0.0f, ResourceDesire - ActualNeed);
    float GreedPercent = (ActualNeed > 0.0f) ? (GreedLevel / ActualNeed) * 100.0f : 0.0f;
    
    float Threshold = GetDetectionThreshold(ECorruptionSignDetail::GreedGrowth_Medium);
    
    if (ValidateDetectionParams(GreedPercent, Threshold) && GreedPercent > Threshold)
    {
        Result.bDetected = true;
        Result.ConfidenceLevel = CalculateConfidence(GreedPercent, Threshold);
        Result.Severity = CalculateSeverity(GreedPercent, Threshold);
        
        // 確定具體徵象類型
        if (Result.Severity == ECorruptionSeverity::Light)
        {
            Result.DetectedSign = ECorruptionSignDetail::GreedGrowth_Light;
        }
        else if (Result.Severity == ECorruptionSeverity::Medium)
        {
            Result.DetectedSign = ECorruptionSignDetail::GreedGrowth_Medium;
        }
        else
        {
            Result.DetectedSign = ECorruptionSignDetail::GreedGrowth_Severe;
        }
        
        Result.AnalysisReport = GenerateAnalysisReport(Result.DetectedSign, Result.Severity);
        Result.RecommendedActions = GenerateRecommendations(Result.Severity);
        
        RecordDetection(Result);
        
        FCorruptionSignData SignData;
        SignData.SignType = Result.DetectedSign;
        SignData.Severity = Result.Severity;
        SignData.DetectionValue = GreedPercent;
        SignData.ThresholdValue = Threshold;
        SignData.Description = Result.AnalysisReport;
        SignData.WarningMessage = TEXT("檢測到貪慾增長徵象");
        SignData.TimeDetected = FDateTime::Now().ToUnixTimestamp();
        SignData.bIsActive = true;
        
        UpdateSignStatus(Result.DetectedSign, SignData);
        OnCorruptionSignDetected.Broadcast(SignData);
        
        UE_LOG(LogTemp, Warning, TEXT("Greed growth detected: %s (Severity: %d)"), 
               *Result.AnalysisReport, static_cast<int32>(Result.Severity));
    }
    else
    {
        Result.AnalysisReport = TEXT("未檢測到貪慾增長徵象");
        UE_LOG(LogTemp, Log, TEXT("No greed growth detected"));
    }
    
    return Result;
}

TArray<FDetectionResult> UMingCorruptionDetectionSystem::PerformFullDetection()
{
    TArray<FDetectionResult> Results;
    
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Detection system not initialized"));
        return Results;
    }

    UE_LOG(LogTemp, Log, TEXT("Performing full corruption detection..."));
    
    // 模擬檢測數據
    // 在實際應用中，這些值應該從遊戲系統獲取
    float CurrentMorality = 70.0f;  // 當前道德值
    float HistoricalMorality = 85.0f;  // 歷史道德值
    float PowerUsage = 120.0f;  // 權力使用
    float AuthorizedLimit = 100.0f;  // 授權限制
    float ResourceDesire = 150.0f;  // 資源渴望
    float ActualNeed = 100.0f;  // 實際需求
    
    // 執行三種檢測
    FDetectionResult MoralResult = DetectMoralDecay(CurrentMorality, HistoricalMorality);
    if (MoralResult.bDetected)
    {
        Results.Add(MoralResult);
    }
    
    FDetectionResult PowerResult = DetectPowerAbuse(PowerUsage, AuthorizedLimit);
    if (PowerResult.bDetected)
    {
        Results.Add(PowerResult);
    }
    
    FDetectionResult GreedResult = DetectGreedGrowth(ResourceDesire, ActualNeed);
    if (GreedResult.bDetected)
    {
        Results.Add(GreedResult);
    }
    
    // 更新最高嚴重程度
    ECorruptionSeverity MaxSeverity = ECorruptionSeverity::None;
    for (const auto& Result : Results)
    {
        if (static_cast<uint8>(Result.Severity) > static_cast<uint8>(MaxSeverity))
        {
            MaxSeverity = Result.Severity;
        }
    }
    
    if (MaxSeverity != CurrentMaxSeverity)
    {
        ECorruptionSeverity OldSeverity = CurrentMaxSeverity;
        CurrentMaxSeverity = MaxSeverity;
        OnSeverityLevelChanged.Broadcast(CurrentMaxSeverity);
        
        UE_LOG(LogTemp, Warning, TEXT("Corruption severity changed from %d to %d"),
               static_cast<int32>(OldSeverity), static_cast<int32>(CurrentMaxSeverity));
    }
    
    OnCorruptionAnalysisCompleted(Results);
    
    UE_LOG(LogTemp, Log, TEXT("Full detection completed. Found %d corruption signs"), Results.Num());
    
    return Results;
}

FCorruptionSignData UMingCorruptionDetectionSystem::GetSignData(ECorruptionSignDetail SignType) const
{
    if (const FCorruptionSignData* Data = SignDatabase.Find(SignType))
    {
        return *Data;
    }
    
    return FCorruptionSignData();
}

void UMingCorruptionDetectionSystem::UpdateDetectionThreshold(ECorruptionSignDetail SignType, float NewThreshold)
{
    if (!bIsInitialized)
    {
        return;
    }

    DetectionThresholds.Add(SignType, NewThreshold);
    
    // 更新數據庫中的閾值
    FCorruptionSignData* Data = SignDatabase.Find(SignType);
    if (Data)
    {
        Data->ThresholdValue = NewThreshold;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Updated detection threshold for sign %d to %.2f"),
           static_cast<int32>(SignType), NewThreshold);
}

bool UMingCorruptionDetectionSystem::ApplyPreventionMeasure(const FPreventionMeasure& Measure)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (Measure.Cost <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid prevention measure cost"));
        return false;
    }

    // 檢查措施是否已經激活
    for (const auto& ExistingMeasure : AvailableMeasures)
    {
        if (ExistingMeasure.MeasureName == Measure.MeasureName && ExistingMeasure.bIsActive)
        {
            UE_LOG(LogTemp, Warning, TEXT("Prevention measure already active: %s"), *Measure.MeasureName);
            return false;
        }
    }
    
    // 激活措施
    FPreventionMeasure NewMeasure = Measure;
    NewMeasure.bIsActive = true;
    AvailableMeasures.Add(NewMeasure);
    
    OnPreventionMeasureTriggered.Broadcast(NewMeasure);
    
    UE_LOG(LogTemp, Log, TEXT("Applied prevention measure: %s (Effectiveness: %.2f)"),
           *NewMeasure.MeasureName, NewMeasure.Effectiveness);
    
    return true;
}

TArray<FPreventionMeasure> UMingCorruptionDetectionSystem::GetRecommendedMeasures(ECorruptionSeverity Severity) const
{
    TArray<FPreventionMeasure> Recommendations;
    
    switch (Severity)
    {
    case ECorruptionSeverity::Light:
        for (const auto& Measure : AvailableMeasures)
        {
            if (Measure.Effectiveness >= 0.3f && Measure.Effectiveness < 0.5f && !Measure.bIsActive)
            {
                Recommendations.Add(Measure);
            }
        }
        break;
        
    case ECorruptionSeverity::Medium:
        for (const auto& Measure : AvailableMeasures)
        {
            if (Measure.Effectiveness >= 0.5f && Measure.Effectiveness < 0.7f && !Measure.bIsActive)
            {
                Recommendations.Add(Measure);
            }
        }
        break;
        
    case ECorruptionSeverity::Severe:
    case ECorruptionSeverity::Critical:
        for (const auto& Measure : AvailableMeasures)
        {
            if (Measure.Effectiveness >= 0.7f && !Measure.bIsActive)
            {
                Recommendations.Add(Measure);
            }
        }
        break;
        
    default:
        break;
    }
    
    return Recommendations;
}

float UMingCorruptionDetectionSystem::CalculateOverallCorruptionRisk() const
{
    if (!bIsInitialized || SignDatabase.Num() == 0)
    {
        return 0.0f;
    }

    float TotalRisk = 0.0f;
    int32 ActiveSigns = 0;
    
    for (const auto& Pair : SignDatabase)
    {
        if (Pair.Value.bIsActive)
        {
            // 根據嚴重程度計算風險值
            float SeverityValue = static_cast<float>(Pair.Value.Severity) * 25.0f;
            TotalRisk += SeverityValue;
            ActiveSigns++;
        }
    }
    
    if (ActiveSigns > 0)
    {
        return FMath::Min(TotalRisk / ActiveSigns, 100.0f);
    }
    
    return 0.0f;
}

bool UMingCorruptionDetectionSystem::RequiresEmergencyIntervention() const
{
    return CalculateOverallCorruptionRisk() >= EmergencyThreshold;
}

// 私有方法實現
void UMingCorruptionDetectionSystem::InitializeDefaultThresholds()
{
    DetectionThresholds.Empty();
    
    // 道德淪喪閾值
    DetectionThresholds.Add(ECorruptionSignDetail::MoralDecay_Light, 10.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::MoralDecay_Medium, 25.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::MoralDecay_Severe, 40.0f);
    
    // 權力濫用閾值
    DetectionThresholds.Add(ECorruptionSignDetail::PowerAbuse_Light, 15.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::PowerAbuse_Medium, 30.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::PowerAbuse_Severe, 50.0f);
    
    // 貪慾增長閾值
    DetectionThresholds.Add(ECorruptionSignDetail::GreedGrowth_Light, 20.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::GreedGrowth_Medium, 40.0f);
    DetectionThresholds.Add(ECorruptionSignDetail::GreedGrowth_Severe, 60.0f);
}

void UMingCorruptionDetectionSystem::InitializeDefaultMeasures()
{
    AvailableMeasures.Empty();
    
    // 輕微措施
    FPreventionMeasure LightMeasure1;
    LightMeasure1.MeasureName = TEXT("道德提醒");
    LightMeasure1.Description = TEXT("定期進行道德教育提醒");
    LightMeasure1.Effectiveness = 0.3f;
    LightMeasure1.Cost = 50.0f;
    LightMeasure1.bIsActive = false;
    AvailableMeasures.Add(LightMeasure1);
    
    FPreventionMeasure LightMeasure2;
    LightMeasure2.MeasureName = TEXT("權力監督");
    LightMeasure2.Description = TEXT("加強對權力使用的監督");
    LightMeasure2.Effectiveness = 0.4f;
    LightMeasure2.Cost = 80.0f;
    LightMeasure2.bIsActive = false;
    AvailableMeasures.Add(LightMeasure2);
    
    // 中等措施
    FPreventionMeasure MediumMeasure1;
    MediumMeasure1.MeasureName = TEXT("制度約束");
    MediumMeasure1.Description = TEXT("建立更嚴格的制度約束機制");
    MediumMeasure1.Effectiveness = 0.6f;
    MediumMeasure1.Cost = 150.0f;
    MediumMeasure1.bIsActive = false;
    AvailableMeasures.Add(MediumMeasure1);
    
    // 嚴重措施
    FPreventionMeasure SevereMeasure1;
    SevereMeasure1.MeasureName = TEXT("全面審查");
    SevereMeasure1.Description = TEXT("對所有決策進行全面道德審查");
    SevereMeasure1.Effectiveness = 0.8f;
    SevereMeasure1.Cost = 300.0f;
    SevereMeasure1.bIsActive = false;
    AvailableMeasures.Add(SevereMeasure1);
    
    FPreventionMeasure SevereMeasure2;
    SevereMeasure2.MeasureName = TEXT("權力制衡");
    SevereMeasure2.Description = TEXT("實施嚴格的權力分立制衡機制");
    SevereMeasure2.Effectiveness = 0.85f;
    SevereMeasure2.Cost = 400.0f;
    SevereMeasure2.bIsActive = false;
    AvailableMeasures.Add(SevereMeasure2);
}

ECorruptionSeverity UMingCorruptionDetectionSystem::CalculateSeverity(float Value, float Threshold) const
{
    if (Value < Threshold * 1.5f)
    {
        return ECorruptionSeverity::Light;
    }
    else if (Value < Threshold * 2.5f)
    {
        return ECorruptionSeverity::Medium;
    }
    else if (Value < Threshold * 4.0f)
    {
        return ECorruptionSeverity::Severe;
    }
    else
    {
        return ECorruptionSeverity::Critical;
    }
}

float UMingCorruptionDetectionSystem::CalculateConfidence(float Value, float Threshold) const
{
    if (Threshold <= 0.0f)
    {
        return 0.0f;
    }
    
    float Ratio = Value / Threshold;
    return FMath::Clamp(Ratio * 0.5f + 0.5f, 0.0f, 1.0f);
}

FString UMingCorruptionDetectionSystem::GenerateAnalysisReport(ECorruptionSignDetail SignType, ECorruptionSeverity Severity) const
{
    FString SignName;
    switch (SignType)
    {
    case ECorruptionSignDetail::MoralDecay_Light:
    case ECorruptionSignDetail::MoralDecay_Medium:
    case ECorruptionSignDetail::MoralDecay_Severe:
        SignName = TEXT("道德淪喪");
        break;
    case ECorruptionSignDetail::PowerAbuse_Light:
    case ECorruptionSignDetail::PowerAbuse_Medium:
    case ECorruptionSignDetail::PowerAbuse_Severe:
        SignName = TEXT("權力濫用");
        break;
    case ECorruptionSignDetail::GreedGrowth_Light:
    case ECorruptionSignDetail::GreedGrowth_Medium:
    case ECorruptionSignDetail::GreedGrowth_Severe:
        SignName = TEXT("貪慾增長");
        break;
    default:
        SignName = TEXT("未知徵象");
        break;
    }
    
    FString SeverityText;
    switch (Severity)
    {
    case ECorruptionSeverity::Light:
        SeverityText = TEXT("輕微");
        break;
    case ECorruptionSeverity::Medium:
        SeverityText = TEXT("中等");
        break;
    case ECorruptionSeverity::Severe:
        SeverityText = TEXT("嚴重");
        break;
    case ECorruptionSeverity::Critical:
        SeverityText = TEXT("危急");
        break;
    default:
        SeverityText = TEXT("未知");
        break;
    }
    
    return FString::Printf(TEXT("檢測到%s徵象，嚴重程度：%s"), *SignName, *SeverityText);
}

TArray<FString> UMingCorruptionDetectionSystem::GenerateRecommendations(ECorruptionSeverity Severity) const
{
    TArray<FString> Recommendations;
    
    switch (Severity)
    {
    case ECorruptionSeverity::Light:
        Recommendations.Add(TEXT("加強道德教育"));
        Recommendations.Add(TEXT("定期自我反省"));
        Recommendations.Add(TEXT("輕度監督機制"));
        break;
        
    case ECorruptionSeverity::Medium:
        Recommendations.Add(TEXT("實施制度約束"));
        Recommendations.Add(TEXT("增加審查頻率"));
        Recommendations.Add(TEXT("權力使用報告"));
        Recommendations.Add(TEXT("道德培訓計畫"));
        break;
        
    case ECorruptionSeverity::Severe:
        Recommendations.Add(TEXT("全面權力制衡"));
        Recommendations.Add(TEXT("強制懺悔程序"));
        Recommendations.Add(TEXT("外部監督介入"));
        Recommendations.Add(TEXT("決策全面審查"));
        Recommendations.Add(TEXT("暫停部分權限"));
        break;
        
    case ECorruptionSeverity::Critical:
        Recommendations.Add(TEXT("緊急干預措施"));
        Recommendations.Add(TEXT("權力完全凍結"));
        Recommendations.Add(TEXT("最高戒律執行"));
        Recommendations.Add(TEXT("徹底調查審查"));
        Recommendations.Add(TEXT("外部接管管理"));
        break;
        
    default:
        Recommendations.Add(TEXT("保持監控"));
        break;
    }
    
    return Recommendations;
}

bool UMingCorruptionDetectionSystem::ValidateDetectionParams(float Value, float Threshold) const
{
    return Value >= 0.0f && Threshold > 0.0f;
}

void UMingCorruptionDetectionSystem::RecordDetection(const FDetectionResult& Result)
{
    DetectionHistory.Add(Result);
    
    // 限制歷史記錄數量
    if (DetectionHistory.Num() > 100)
    {
        DetectionHistory.RemoveAt(0);
    }
}

void UMingCorruptionDetectionSystem::UpdateSignStatus(ECorruptionSignDetail SignType, const FCorruptionSignData& Data)
{
    FCorruptionSignData* ExistingData = SignDatabase.Find(SignType);
    if (ExistingData)
    {
        *ExistingData = Data;
    }
    else
    {
        SignDatabase.Add(SignType, Data);
    }
}

float UMingCorruptionDetectionSystem::GetDetectionThreshold(ECorruptionSignDetail SignType) const
{
    if (const float* Threshold = DetectionThresholds.Find(SignType))
    {
        return *Threshold;
    }
    
    return 50.0f; // 默認閾值
}
