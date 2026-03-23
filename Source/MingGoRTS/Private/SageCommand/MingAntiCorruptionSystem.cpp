// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingAntiCorruptionSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingAntiCorruptionSystem::UMingAntiCorruptionSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化系統參數
    AuditInterval = 300.0f; // 5分鐘
    MaxWarnings = 10;
    PhenomenonDecayRate = 0.05f;
    
    // 初始化狀態
    bSystemInitialized = false;
    NextWarningID = 1;
    NextAuditID = 1;
    
    // 初始化道德邊界
    MoralBoundaryStatus.MoralIntegrity = 100.0f;
    MoralBoundaryStatus.CorruptionResistance = 100.0f;
    MoralBoundaryStatus.WarningThreshold = 70.0f;
    MoralBoundaryStatus.CriticalThreshold = 30.0f;
    MoralBoundaryStatus.bIsWithinBoundary = true;
    MoralBoundaryStatus.RiskLevel = 0.0f;
}

void UMingAntiCorruptionSystem::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeAntiCorruptionSystem();
    
    UE_LOG(LogTemp, Log, TEXT("防墮機制系統初始化完成"));
}

void UMingAntiCorruptionSystem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized)
    {
        return;
    }
    
    // 更新道德狀態
    UpdateMoralStatus(DeltaTime);
    
    // 處理警告超時
    HandleWarningTimeouts();
}

bool UMingAntiCorruptionSystem::InitializeAntiCorruptionSystem()
{
    if (bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("防墮機制系統已經初始化"));
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("開始初始化防墮機制系統"));
    
    // 清空所有數據
    CurrentCorruptionPhenomena.Empty();
    ActiveWarnings.Empty();
    WarningHistory.Empty();
    AuditHistory.Empty();
    CurrentAudits.Empty();
    ActiveMeasures.Empty();
    
    // 啟動自動檢測定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            AutoDetectionTimer,
            this,
            &UMingAntiCorruptionSystem::AutoDetectCorruptionPhenomena,
            AUTO_DETECTION_INTERVAL,
            true
        );
    }
    
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 防墮機制系統初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  初始道德完整性: %.1f%%"), MoralBoundaryStatus.MoralIntegrity);
    UE_LOG(LogTemp, Log, TEXT("  初始腐敗抵抗力: %.1f%%"), MoralBoundaryStatus.CorruptionResistance);
    
    return true;
}

void UMingAntiCorruptionSystem::ResetAntiCorruptionSystem()
{
    UE_LOG(LogTemp, Log, TEXT("重置防墮機制系統"));
    
    // 停止定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(AutoDetectionTimer);
        GetWorld()->GetTimerManager().ClearTimer(AuditTimer);
    }
    
    // 重置狀態
    CurrentCorruptionPhenomena.Empty();
    ActiveWarnings.Empty();
    AuditHistory.Empty();
    CurrentAudits.Empty();
    ActiveMeasures.Empty();
    
    // 重置道德邊界
    MoralBoundaryStatus = FMoralBoundaryStatus();
    
    // 重置ID
    NextWarningID = 1;
    NextAuditID = 1;
    
    bSystemInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("防墮機制系統已重置"));
}

bool UMingAntiCorruptionSystem::DetectCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType, float Severity, const FString& Evidence)
{
    if (!bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("防墮機制系統未初始化"));
        return false;
    }
    
    // 創建徵象數據
    FCorruptionPhenomenon Phenomenon;
    Phenomenon.PhenomenonType = PhenomenonType;
    Phenomenon.SeverityValue = Severity;
    Phenomenon.Severity = AssessSeverity(Severity);
    Phenomenon.DetectionTime = FDateTime::Now();
    Phenomenon.bIsAcknowledged = false;
    
    // 設置描述
    switch (PhenomenonType)
    {
    case ECorruptionPhenomenon::RationalizationOfEvil:
        Phenomenon.Description = TEXT("將非常手段合理化");
        break;
    case ECorruptionPhenomenon::FollowerDeification:
        Phenomenon.Description = TEXT("將追隨者神聖化");
        break;
    case ECorruptionPhenomenon::OppositionDemonization:
        Phenomenon.Description = TEXT("將反對者妖魔化");
        break;
    case ECorruptionPhenomenon::IAmJusticeSyndrome:
        Phenomenon.Description = TEXT("我即正義徵象");
        break;
    default:
        Phenomenon.Description = TEXT("未知徵象");
        break;
    }
    
    // 添加證據
    if (!Evidence.IsEmpty())
    {
        Phenomenon.Evidence.Add(Evidence);
    }
    
    // 添加到當前徵象
    CurrentCorruptionPhenomena.Add(PhenomenonType, Phenomenon);
    
    // 觸發事件
    TriggerPhenomenonDetectedEvent(Phenomenon);
    
    // 如果嚴重，發布警告
    if (Phenomenon.Severity >= ECorruptionSeverity::Moderate)
    {
        IssueAntiCorruptionWarning(PhenomenonType, Phenomenon.Severity);
    }
    
    // 更新道德完整性
    float IntegrityLoss = Severity * 0.5f;
    UpdateMoralIntegrity(-IntegrityLoss);
    
    UE_LOG(LogTemp, Log, TEXT("檢測到墮落徵象: %s, 嚴重程度: %d, 數值: %.2f"), 
           *Phenomenon.Description, (int32)Phenomenon.Severity, Severity);
    
    return true;
}

TArray<FCorruptionPhenomenon> UMingAntiCorruptionSystem::GetCurrentCorruptionPhenomena() const
{
    TArray<FCorruptionPhenomenon> Phenomena;
    
    for (const auto& Pair : CurrentCorruptionPhenomena)
    {
        Phenomena.Add(Pair.Value);
    }
    
    return Phenomena;
}

FCorruptionPhenomenon UMingAntiCorruptionSystem::GetCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType) const
{
    if (CurrentCorruptionPhenomena.Contains(PhenomenonType))
    {
        return CurrentCorruptionPhenomena[PhenomenonType];
    }
    
    return FCorruptionPhenomenon();
}

float UMingAntiCorruptionSystem::CalculateTotalCorruptionRisk() const
{
    float TotalRisk = 0.0f;
    
    for (const auto& Pair : CurrentCorruptionPhenomena)
    {
        const FCorruptionPhenomenon& Phenomenon = Pair.Value;
        
        // 根據嚴重程度和持續時間計算風險
        float PhenomenonRisk = Phenomenon.SeverityValue;
        
        // 持續時間增加風險
        FDateTime CurrentTime = FDateTime::Now();
        FTimespan Duration = CurrentTime - Phenomenon.DetectionTime;
        float HoursElapsed = Duration.GetTotalHours();
        
        PhenomenonRisk *= (1.0f + HoursElapsed * 0.1f);
        
        TotalRisk += PhenomenonRisk;
    }
    
    // 考慮道德邊界狀態
    float MoralFactor = (100.0f - MoralBoundaryStatus.MoralIntegrity) / 100.0f;
    TotalRisk *= (1.0f + MoralFactor);
    
    return FMath::Clamp(TotalRisk, 0.0f, 100.0f);
}

bool UMingAntiCorruptionSystem::HasCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType) const
{
    return CurrentCorruptionPhenomena.Contains(PhenomenonType);
}

bool UMingAntiCorruptionSystem::ClearCorruptionPhenomenon(ECorruptionPhenomenon PhenomenonType)
{
    if (!CurrentCorruptionPhenomena.Contains(PhenomenonType))
    {
        return false;
    }
    
    // 獲取徵象數據
    FCorruptionPhenomenon Phenomenon = CurrentCorruptionPhenomena[PhenomenonType];
    
    // 移除徵象
    CurrentCorruptionPhenomena.Remove(PhenomenonType);
    
    // 恢復道德完整性
    float IntegrityGain = Phenomenon.SeverityValue * 0.3f;
    UpdateMoralIntegrity(IntegrityGain);
    
    UE_LOG(LogTemp, Log, TEXT("清除墮落徵象: %s"), *Phenomenon.Description);
    
    return true;
}

int32 UMingAntiCorruptionSystem::IssueAntiCorruptionWarning(ECorruptionPhenomenon TargetPhenomenon, ECorruptionSeverity WarningLevel)
{
    if (ActiveWarnings.Num() >= MaxWarnings)
    {
        UE_LOG(LogTemp, Warning, TEXT("警告數量達到上限"));
        return -1;
    }
    
    // 創建警告
    FAntiCorruptionWarning Warning;
    Warning.WarningID = NextWarningID++;
    Warning.TargetPhenomenon = TargetPhenomenon;
    Warning.WarningLevel = WarningLevel;
    Warning.WarningMessage = GenerateWarningMessage(TargetPhenomenon, WarningLevel);
    Warning.IssueTime = FDateTime::Now();
    Warning.RecommendedMeasures = GenerateWarningRecommendations(TargetPhenomenon);
    Warning.bIsUrgent = (WarningLevel >= ECorruptionSeverity::Severe);
    Warning.TimeToCorrect = (WarningLevel == ECorruptionSeverity::Critical) ? 300.0f : 600.0f;
    
    // 添加到活動警告
    ActiveWarnings.Add(Warning);
    WarningHistory.Add(Warning);
    
    // 觸發警告事件
    TriggerWarningIssuedEvent(Warning);
    
    UE_LOG(LogTemp, Log, TEXT("發布防墮警告 ID:%d, 等級:%d, %s"), 
           Warning.WarningID, (int32)WarningLevel, *Warning.WarningMessage);
    
    return Warning.WarningID;
}

TArray<FAntiCorruptionWarning> UMingAntiCorruptionSystem::GetCurrentWarnings() const
{
    return ActiveWarnings;
}

bool UMingAntiCorruptionSystem::ResolveWarning(int32 WarningID)
{
    for (int32 i = 0; i < ActiveWarnings.Num(); ++i)
    {
        if (ActiveWarnings[i].WarningID == WarningID)
        {
            ActiveWarnings.RemoveAt(i);
            UE_LOG(LogTemp, Log, TEXT("解除防墮警告 ID:%d"), WarningID);
            return true;
        }
    }
    
    return false;
}

int32 UMingAntiCorruptionSystem::StartSelfAudit()
{
    if (!bSystemInitialized)
    {
        return -1;
    }
    
    // 創建審計數據
    FSelfAuditData Audit;
    Audit.AuditID = NextAuditID++;
    Audit.Status = ESelfAuditStatus::InProgress;
    Audit.StartTime = FDateTime::Now();
    Audit.OverallHealthScore = MoralBoundaryStatus.MoralIntegrity;
    
    // 添加到當前審計
    CurrentAudits.Add(Audit.AuditID, Audit);
    
    UE_LOG(LogTemp, Log, TEXT("啟動自我審計 ID:%d"), Audit.AuditID);
    
    return Audit.AuditID;
}

FSelfAuditData UMingAntiCorruptionSystem::CompleteSelfAudit(int32 AuditID)
{
    if (!CurrentAudits.Contains(AuditID))
    {
        return FSelfAuditData();
    }
    
    FSelfAuditData& Audit = CurrentAudits[AuditID];
    
    // 設置完成狀態
    Audit.Status = ESelfAuditStatus::Completed;
    Audit.EndTime = FDateTime::Now();
    
    // 檢測當前所有徵象
    for (const auto& Pair : CurrentCorruptionPhenomena)
    {
        Audit.DetectedIssues.Add(Pair.Value);
    }
    
    // 生成建議
    Audit.RecommendedActions = GetRecommendedMeasures(ECorruptionPhenomenon::None);
    
    // 更新健康評分
    Audit.OverallHealthScore = MoralBoundaryStatus.MoralIntegrity;
    
    // 生成報告
    Audit.AuditReport = FString::Printf(TEXT("審計完成。檢測到 %d 個問題，健康評分: %.1f%%"), 
                                        Audit.DetectedIssues.Num(), Audit.OverallHealthScore);
    
    // 移動到歷史
    AuditHistory.Add(Audit);
    CurrentAudits.Remove(AuditID);
    
    // 觸發事件
    OnSelfAuditCompleted.Broadcast(Audit);
    
    UE_LOG(LogTemp, Log, TEXT("完成自我審計 ID:%d - %s"), AuditID, *Audit.AuditReport);
    
    return Audit;
}

TArray<FSelfAuditData> UMingAntiCorruptionSystem::GetAuditHistory() const
{
    return AuditHistory;
}

FMoralBoundaryStatus UMingAntiCorruptionSystem::CheckMoralBoundaryStatus() const
{
    return MoralBoundaryStatus;
}

bool UMingAntiCorruptionSystem::UpdateMoralIntegrity(float Delta)
{
    float PreviousIntegrity = MoralBoundaryStatus.MoralIntegrity;
    
    MoralBoundaryStatus.MoralIntegrity = FMath::Clamp(
        MoralBoundaryStatus.MoralIntegrity + Delta,
        0.0f,
        100.0f
    );
    
    // 檢查是否跨越邊界
    if ((PreviousIntegrity >= MoralBoundaryStatus.WarningThreshold && 
         MoralBoundaryStatus.MoralIntegrity < MoralBoundaryStatus.WarningThreshold) ||
        (PreviousIntegrity >= MoralBoundaryStatus.CriticalThreshold && 
         MoralBoundaryStatus.MoralIntegrity < MoralBoundaryStatus.CriticalThreshold))
    {
        OnMoralBoundaryCrossed.Broadcast(MoralBoundaryStatus.MoralIntegrity, PreviousIntegrity);
    }
    
    // 更新風險等級
    MoralBoundaryStatus.RiskLevel = CalculateMoralRisk();
    
    // 更新邊界狀態
    MoralBoundaryStatus.bIsWithinBoundary = (MoralBoundaryStatus.MoralIntegrity >= MoralBoundaryStatus.CriticalThreshold);
    
    UE_LOG(LogTemp, Log, TEXT("道德完整性更新: %.1f%% -> %.1f%% (變化: %+.1f%%)"), 
           PreviousIntegrity, MoralBoundaryStatus.MoralIntegrity, Delta);
    
    return true;
}

bool UMingAntiCorruptionSystem::StrengthenMoralResistance(float Amount)
{
    MoralBoundaryStatus.CorruptionResistance = FMath::Clamp(
        MoralBoundaryStatus.CorruptionResistance + Amount,
        0.0f,
        100.0f
    );
    
    UE_LOG(LogTemp, Log, TEXT("道德抵抗力強化: %.1f%%"), MoralBoundaryStatus.CorruptionResistance);
    
    return true;
}

bool UMingAntiCorruptionSystem::ApplyAntiCorruptionMeasure(EAntiCorruptionMeasure Measure)
{
    // 創建措施效果
    FAntiCorruptionMeasureEffect Effect;
    Effect.MeasureType = Measure;
    Effect.bIsActive = true;
    
    // 根據措施類型設置參數
    switch (Measure)
    {
    case EAntiCorruptionMeasure::SelfReflection:
        Effect.Effectiveness = 0.3f;
        Effect.Duration = 300.0f;
        Effect.Cost = 10.0f;
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::RationalizationOfEvil);
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::IAmJusticeSyndrome);
        break;
        
    case EAntiCorruptionMeasure::ExternalAudit:
        Effect.Effectiveness = 0.5f;
        Effect.Duration = 600.0f;
        Effect.Cost = 50.0f;
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::FollowerDeification);
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::OppositionDemonization);
        break;
        
    case EAntiCorruptionMeasure::MoralRestraint:
        Effect.Effectiveness = 0.4f;
        Effect.Duration = 900.0f;
        Effect.Cost = 30.0f;
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::PowerObsession);
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::MoralNumbness);
        break;
        
    case EAntiCorruptionMeasure::PowerLimitation:
        Effect.Effectiveness = 0.6f;
        Effect.Duration = 1200.0f;
        Effect.Cost = 40.0f;
        Effect.TargetPhenomena.Add(ECorruptionPhenomenon::PowerObsession);
        break;
        
    default:
        Effect.Effectiveness = 0.2f;
        Effect.Duration = 180.0f;
        Effect.Cost = 5.0f;
        break;
    }
    
    // 添加到活動措施
    ActiveMeasures.Add(Effect);
    
    // 觸發事件
    OnAntiCorruptionMeasureApplied.Broadcast(Measure, Effect.Effectiveness);
    
    // 應用效果
    for (ECorruptionPhenomenon Target : Effect.TargetPhenomena)
    {
        if (CurrentCorruptionPhenomena.Contains(Target))
        {
            float Reduction = Effect.Effectiveness * 0.5f;
            FCorruptionPhenomenon& Phenomenon = CurrentCorruptionPhenomena[Target];
            Phenomenon.SeverityValue = FMath::Max(0.0f, Phenomenon.SeverityValue - Reduction);
            
            if (Phenomenon.SeverityValue <= 0.1f)
            {
                ClearCorruptionPhenomenon(Target);
            }
        }
    }
    
    // 強化道德抵抗
    StrengthenMoralResistance(Effect.Effectiveness * 10.0f);
    
    UE_LOG(LogTemp, Log, TEXT("應用防墮措施: %d, 效果: %.2f, 持續: %.1f秒"), 
           (int32)Measure, Effect.Effectiveness, Effect.Duration);
    
    return true;
}

TArray<EAntiCorruptionMeasure> UMingAntiCorruptionSystem::GetRecommendedMeasures(ECorruptionPhenomenon TargetPhenomenon) const
{
    TArray<EAntiCorruptionMeasure> Recommendations;
    
    // 根據當前徵象推薦措施
    if (TargetPhenomenon != ECorruptionPhenomenon::None)
    {
        switch (TargetPhenomenon)
        {
        case ECorruptionPhenomenon::RationalizationOfEvil:
            Recommendations.Add(EAntiCorruptionMeasure::SelfReflection);
            Recommendations.Add(EAntiCorruptionMeasure::Transparency);
            break;
            
        case ECorruptionPhenomenon::FollowerDeification:
            Recommendations.Add(EAntiCorruptionMeasure::ExternalAudit);
            Recommendations.Add(EAntiCorruptionMeasure::Accountability);
            break;
            
        case ECorruptionPhenomenon::OppositionDemonization:
            Recommendations.Add(EAntiCorruptionMeasure::ExternalAudit);
            Recommendations.Add(EAntiCorruptionMeasure::Transparency);
            break;
            
        case ECorruptionPhenomenon::PowerObsession:
            Recommendations.Add(EAntiCorruptionMeasure::PowerLimitation);
            Recommendations.Add(EAntiCorruptionMeasure::SeparationOfPowers);
            break;
            
        default:
            Recommendations.Add(EAntiCorruptionMeasure::SelfReflection);
            break;
        }
    }
    else
    {
        // 通用推薦
        Recommendations.Add(EAntiCorruptionMeasure::SelfReflection);
        Recommendations.Add(EAntiCorruptionMeasure::Transparency);
    }
    
    return Recommendations;
}

// ============================================================================
// 內部方法實現
// ============================================================================

void UMingAntiCorruptionSystem::AutoDetectCorruptionPhenomena()
{
    // 自動檢測邏輯 - 基於當前遊戲狀態自動檢測
    // 這裡可以根據實際遊戲邏輯進行擴展
    
    // 檢查道德完整性是否過低
    if (MoralBoundaryStatus.MoralIntegrity < MoralBoundaryStatus.WarningThreshold)
    {
        // 可能出現道德麻木
        if (!HasCorruptionPhenomenon(ECorruptionPhenomenon::MoralNumbness))
        {
            DetectCorruptionPhenomenon(ECorruptionPhenomenon::MoralNumbness, 0.3f, TEXT("道德完整性低於警告閾值"));
        }
    }
}

ECorruptionSeverity UMingAntiCorruptionSystem::AssessSeverity(float Value) const
{
    if (Value < 0.2f)
    {
        return ECorruptionSeverity::Mild;
    }
    else if (Value < 0.4f)
    {
        return ECorruptionSeverity::Moderate;
    }
    else if (Value < 0.7f)
    {
        return ECorruptionSeverity::Severe;
    }
    else
    {
        return ECorruptionSeverity::Critical;
    }
}

FString UMingAntiCorruptionSystem::GenerateWarningMessage(ECorruptionPhenomenon PhenomenonType, ECorruptionSeverity Severity) const
{
    FString BaseMessage;
    
    switch (PhenomenonType)
    {
    case ECorruptionPhenomenon::RationalizationOfEvil:
        BaseMessage = TEXT("檢測到將非常手段合理化的徵象");
        break;
    case ECorruptionPhenomenon::FollowerDeification:
        BaseMessage = TEXT("檢測到將追隨者神聖化的徵象");
        break;
    case ECorruptionPhenomenon::OppositionDemonization:
        BaseMessage = TEXT("檢測到將反對者妖魔化的徵象");
        break;
    case ECorruptionPhenomenon::IAmJusticeSyndrome:
        BaseMessage = TEXT("檢測到我即正義徵象");
        break;
    default:
        BaseMessage = TEXT("檢測到未知墮落徵象");
        break;
    }
    
    FString SeverityText;
    switch (Severity)
    {
    case ECorruptionSeverity::Mild:
        SeverityText = TEXT("（輕微）");
        break;
    case ECorruptionSeverity::Moderate:
        SeverityText = TEXT("（中等）");
        break;
    case ECorruptionSeverity::Severe:
        SeverityText = TEXT("（嚴重）");
        break;
    case ECorruptionSeverity::Critical:
        SeverityText = TEXT("（危急）");
        break;
    default:
        SeverityText = TEXT("");
        break;
    }
    
    return BaseMessage + SeverityText + TEXT("，建議立即採取防墮措施。");
}

void UMingAntiCorruptionSystem::UpdateMoralStatus(float DeltaTime)
{
    // 自然衰減徵象
    TArray<ECorruptionPhenomenon> PhenomenaToRemove;
    
    for (auto& Pair : CurrentCorruptionPhenomena)
    {
        FCorruptionPhenomenon& Phenomenon = Pair.Value;
        
        // 增加持續時間
        Phenomenon.Duration += DeltaTime;
        
        // 自然衰減（如果應用了措施）
        if (ActiveMeasures.Num() > 0)
        {
            Phenomenon.SeverityValue *= (1.0f - PhenomenonDecayRate * DeltaTime);
            
            if (Phenomenon.SeverityValue <= 0.1f)
            {
                PhenomenaToRemove.Add(Pair.Key);
            }
        }
    }
    
    // 移除已清除的徵象
    for (ECorruptionPhenomenon PhenomenonType : PhenomenaToRemove)
    {
        ClearCorruptionPhenomenon(PhenomenonType);
    }
    
    // 更新措施持續時間
    TArray<FAntiCorruptionMeasureEffect> MeasuresToRemove;
    
    for (auto& Measure : ActiveMeasures)
    {
        Measure.Duration -= DeltaTime;
        
        if (Measure.Duration <= 0.0f)
        {
            Measure.bIsActive = false;
            MeasuresToRemove.Add(Measure);
        }
    }
    
    // 移除過期的措施
    for (const auto& Measure : MeasuresToRemove)
    {
        ActiveMeasures.Remove(Measure);
    }
    
    // 更新風險等級
    MoralBoundaryStatus.RiskLevel = CalculateMoralRisk();
}

void UMingAntiCorruptionSystem::HandleWarningTimeouts()
{
    FDateTime CurrentTime = FDateTime::Now();
    TArray<int32> WarningsToResolve;
    
    for (const auto& Warning : ActiveWarnings)
    {
        FTimespan Elapsed = CurrentTime - Warning.IssueTime;
        
        if (Elapsed.GetTotalSeconds() > Warning.TimeToCorrect)
        {
            WarningsToResolve.Add(Warning.WarningID);
        }
    }
    
    for (int32 WarningID : WarningsToResolve)
    {
        ResolveWarning(WarningID);
    }
}

void UMingAntiCorruptionSystem::TriggerPhenomenonDetectedEvent(const FCorruptionPhenomenon& Phenomenon)
{
    OnCorruptionPhenomenonDetected.Broadcast(Phenomenon);
}

void UMingAntiCorruptionSystem::TriggerWarningIssuedEvent(const FAntiCorruptionWarning& Warning)
{
    OnAntiCorruptionWarningIssued.Broadcast(Warning);
}

float UMingAntiCorruptionSystem::CalculateMoralRisk() const
{
    float BaseRisk = 0.0f;
    
    // 基於道德完整性
    BaseRisk += (100.0f - MoralBoundaryStatus.MoralIntegrity) * 0.5f;
    
    // 基於腐敗抵抗力
    BaseRisk += (100.0f - MoralBoundaryStatus.CorruptionResistance) * 0.3f;
    
    // 基於當前徵象
    for (const auto& Pair : CurrentCorruptionPhenomena)
    {
        BaseRisk += Pair.Value.SeverityValue * 10.0f;
    }
    
    return FMath::Clamp(BaseRisk, 0.0f, 100.0f);
}

TArray<FString> UMingAntiCorruptionSystem::GetMoralRecommendations() const
{
    TArray<FString> Recommendations;
    
    float RiskLevel = CalculateMoralRisk();
    
    if (RiskLevel > 70.0f)
    {
        Recommendations.Add(TEXT("【危急】立即啟動全面自我審計"));
        Recommendations.Add(TEXT("【危急】暫停所有邪術使用"));
        Recommendations.Add(TEXT("【危急】尋求外部監督和審查"));
    }
    else if (RiskLevel > 50.0f)
    {
        Recommendations.Add(TEXT("【警告】加強自我反思和道德約束"));
        Recommendations.Add(TEXT("【警告】減少逆道策略使用頻率"));
        Recommendations.Add(TEXT("【警告】定期進行道德邊界檢查"));
    }
    else if (RiskLevel > 30.0f)
    {
        Recommendations.Add(TEXT("【注意】保持當前道德實踐"));
        Recommendations.Add(TEXT("【注意】監控潛在風險徵象"));
    }
    else
    {
        Recommendations.Add(TEXT("【良好】繼續保持道德領導"));
    }
    
    return Recommendations;
}
