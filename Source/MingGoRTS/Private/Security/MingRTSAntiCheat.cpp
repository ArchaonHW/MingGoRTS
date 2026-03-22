#include "Security/MingRTSAntiCheat.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Misc/DateTime.h"

UMingRTSAntiCheat::UMingRTSAntiCheat()
    : bIsMonitoring(false)
{
}

void UMingRTSAntiCheat::InitializeAntiCheat()
{
    UE_LOG(LogTemp, Log, TEXT("Anti-Cheat System Initialized"));
}

void UMingRTSAntiCheat::StartMonitoring()
{
    if (!bIsMonitoring)
    {
        bIsMonitoring = true;
        StartBackgroundChecks();
        UE_LOG(LogTemp, Log, TEXT("Anti-Cheat monitoring started"));
    }
}

void UMingRTSAntiCheat::StopMonitoring()
{
    if (bIsMonitoring)
    {
        bIsMonitoring = false;
        StopBackgroundChecks();
        UE_LOG(LogTemp, Log, TEXT("Anti-Cheat monitoring stopped"));
    }
}

void UMingRTSAntiCheat::ReportSuspiciousBehavior(const FString& PlayerID, ECheatType Type, const FString& Details)
{
    FCheatDetectionResult Result;
    Result.PlayerID = PlayerID;
    Result.CheatType = Type;
    Result.DetectionDetails = Details;
    Result.Confidence = EDetectionConfidence::Medium;
    Result.SeverityScore = 50.0f;
    Result.Timestamp = FDateTime::Now().ToString();
    
    ProcessDetection(Result);
}

bool UMingRTSAntiCheat::DetectMemoryModification(const FString& PlayerID)
{
    // 檢查關鍵遊戲數據的校驗和
    // 這裡是示例實現，實際應該有更複雜的檢測邏輯
    
    UE_LOG(LogTemp, Verbose, TEXT("Memory check for player: %s"), *PlayerID);
    return false; // 暫時返回未檢測到
}

bool UMingRTSAntiCheat::DetectSpeedHack(const FString& PlayerID, float CurrentSpeed)
{
    // 檢測速度是否異常
    const float MAX_NORMAL_SPEED = 1.5f; // 正常最大速度倍數
    
    if (CurrentSpeed > MAX_NORMAL_SPEED)
    {
        FCheatDetectionResult Result;
        Result.PlayerID = PlayerID;
        Result.CheatType = ECheatType::SpeedHack;
        Result.DetectionDetails = FString::Printf(TEXT("Abnormal speed detected: %.2fx"), CurrentSpeed);
        Result.Confidence = EDetectionConfidence::High;
        Result.SeverityScore = 80.0f;
        Result.Timestamp = FDateTime::Now().ToString();
        
        ProcessDetection(Result);
        return true;
    }
    
    return false;
}

bool UMingRTSAntiCheat::DetectMacroUsage(const FString& PlayerID, const TArray<float>& ActionTimings)
{
    if (ActionTimings.Num() < 10)
    {
        return false;
    }
    
    // 檢測操作間隔是否過於規律 (機器人特徵)
    float Variance = 0.0f;
    float Mean = 0.0f;
    
    for (float Timing : ActionTimings)
    {
        Mean += Timing;
    }
    Mean /= ActionTimings.Num();
    
    for (float Timing : ActionTimings)
    {
        Variance += FMath::Pow(Timing - Mean, 2);
    }
    Variance /= ActionTimings.Num();
    
    // 如果方差非常小，可能是腳本
    const float MACRO_THRESHOLD = 0.001f;
    if (Variance < MACRO_THRESHOLD)
    {
        FCheatDetectionResult Result;
        Result.PlayerID = PlayerID;
        Result.CheatType = ECheatType::MacroScript;
        Result.DetectionDetails = FString::Printf(TEXT("Suspicious timing consistency detected. Variance: %.6f"), Variance);
        Result.Confidence = EDetectionConfidence::High;
        Result.SeverityScore = 75.0f;
        Result.Timestamp = FDateTime::Now().ToString();
        
        ProcessDetection(Result);
        return true;
    }
    
    return false;
}

void UMingRTSAntiCheat::AnalyzePlayerBehavior(const FString& PlayerID)
{
    FPlayerBehaviorRecord* Record = PlayerBehaviorMap.Find(PlayerID);
    if (!Record)
    {
        FPlayerBehaviorRecord NewRecord;
        NewRecord.PlayerID = PlayerID;
        PlayerBehaviorMap.Add(PlayerID, NewRecord);
        Record = PlayerBehaviorMap.Find(PlayerID);
    }
    
    // 計算行為異常分數
    float AnomalyScore = CalculateBehaviorAnomalyScore(*Record);
    
    if (AnomalyScore > 0.8f)
    {
        FCheatDetectionResult Result;
        Result.PlayerID = PlayerID;
        Result.CheatType = ECheatType::AimBot;
        Result.DetectionDetails = FString::Printf(TEXT("Behavior anomaly detected. Score: %.2f"), AnomalyScore);
        Result.Confidence = EDetectionConfidence::Medium;
        Result.SeverityScore = AnomalyScore * 100.0f;
        Result.Timestamp = FDateTime::Now().ToString();
        
        ProcessDetection(Result);
    }
}

void UMingRTSAntiCheat::ProcessDetection(const FCheatDetectionResult& Result)
{
    // 記錄檢測
    DetectionHistory.Add(Result);
    LogDetection(Result);
    
    // 通知服務器
    NotifyServer(Result);
    
    // 廣播事件
    OnCheatDetected.Broadcast(Result);
    
    // 根據置信度和嚴重程度決定處罰
    if (Result.Confidence >= EDetectionConfidence::High && Result.SeverityScore >= 70.0f)
    {
        EPenaltyLevel Penalty = DeterminePenaltyLevel(Result);
        ApplyPenalty(Result.PlayerID, Penalty, Result.DetectionDetails);
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Cheat detected: Player %s, Type %d, Confidence %d"),
        *Result.PlayerID, (int32)Result.CheatType, (int32)Result.Confidence);
}

void UMingRTSAntiCheat::ApplyPenalty(const FString& PlayerID, EPenaltyLevel Penalty, const FString& Reason)
{
    ActivePenalties.Add(PlayerID, Penalty);
    
    FText ReasonText = FText::FromString(Reason);
    OnPlayerPenalized.Broadcast(PlayerID, Penalty, ReasonText);
    
    switch (Penalty)
    {
    case EPenaltyLevel::Warning:
        UE_LOG(LogTemp, Warning, TEXT("Player %s received warning: %s"), *PlayerID, *Reason);
        break;
        
    case EPenaltyLevel::Restriction:
        UE_LOG(LogTemp, Warning, TEXT("Player %s restricted: %s"), *PlayerID, *Reason);
        break;
        
    case EPenaltyLevel::Suspension:
        UE_LOG(LogTemp, Error, TEXT("Player %s suspended: %s"), *PlayerID, *Reason);
        break;
        
    case EPenaltyLevel::PermanentBan:
        BannedPlayers.AddUnique(PlayerID);
        OnPlayerBanned.Broadcast(PlayerID);
        UE_LOG(LogTemp, Error, TEXT("Player %s permanently banned: %s"), *PlayerID, *Reason);
        break;
        
    case EPenaltyLevel::LegalAction:
        BannedPlayers.AddUnique(PlayerID);
        UE_LOG(LogTemp, Fatal, TEXT("Player %s flagged for legal action: %s"), *PlayerID, *Reason);
        break;
    }
}

TArray<FCheatDetectionResult> UMingRTSAntiCheat::GetPlayerViolationHistory(const FString& PlayerID) const
{
    TArray<FCheatDetectionResult> Results;
    
    for (const FCheatDetectionResult& Detection : DetectionHistory)
    {
        if (Detection.PlayerID == PlayerID)
        {
            Results.Add(Detection);
        }
    }
    
    return Results;
}

bool UMingRTSAntiCheat::IsPlayerBanned(const FString& PlayerID) const
{
    return BannedPlayers.Contains(PlayerID);
}

void UMingRTSAntiCheat::UnbanPlayer(const FString& PlayerID, const FString& AdminID, const FString& Reason)
{
    BannedPlayers.Remove(PlayerID);
    ActivePenalties.Remove(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Player %s unbanned by admin %s. Reason: %s"),
        *PlayerID, *AdminID, *Reason);
}

void UMingRTSAntiCheat::UploadDetectionReport(const FCheatDetectionResult& Result)
{
    // 上傳到反作弊服務器
    UE_LOG(LogTemp, Log, TEXT("Uploading detection report for player %s"), *Result.PlayerID);
}

void UMingRTSAntiCheat::StartBackgroundChecks()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimer(MonitoringTimerHandle, this, &UMingRTSAntiCheat::PerformPeriodicChecks, 5.0f, true);
    }
}

void UMingRTSAntiCheat::StopBackgroundChecks()
{
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(MonitoringTimerHandle);
    }
}

void UMingRTSAntiCheat::PerformPeriodicChecks()
{
    if (!bIsMonitoring)
    {
        return;
    }
    
    // 執行各種檢測
    ValidateGameFiles();
    CheckForKnownCheats();
    VerifyProcessIntegrity();
    
    // 對在線玩家進行行為分析
    for (const auto& Pair : PlayerBehaviorMap)
    {
        AnalyzePlayerBehavior(Pair.Key);
    }
}

bool UMingRTSAntiCheat::ValidateGameFiles()
{
    // 驗證遊戲文件完整性
    return true;
}

bool UMingRTSAntiCheat::CheckForKnownCheats()
{
    // 檢查已知的作弊進程或文件
    return true;
}

bool UMingRTSAntiCheat::VerifyProcessIntegrity()
{
    // 驗證遊戲進程完整性
    return true;
}

float UMingRTSAntiCheat::CalculateBehaviorAnomalyScore(const FPlayerBehaviorRecord& Record)
{
    float Score = 0.0f;
    
    // APM異常高
    if (Record.AverageAPM > 400.0f)
    {
        Score += 0.3f;
    }
    
    // APM方差異常小 (過於穩定)
    if (Record.ActionVariance < 5.0f && Record.AverageAPM > 200.0f)
    {
        Score += 0.3f;
    }
    
    // 操作時間一致性異常
    if (Record.TimingConsistency > 0.95f)
    {
        Score += 0.2f;
    }
    
    // 多次可疑操作
    if (Record.SuspiciousActions > 5)
    {
        Score += 0.2f;
    }
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

EPenaltyLevel UMingRTSAntiCheat::DeterminePenaltyLevel(const FCheatDetectionResult& Detection)
{
    // 根據違規歷史和檢測結果決定處罰等級
    TArray<FCheatDetectionResult> History = GetPlayerViolationHistory(Detection.PlayerID);
    int32 ViolationCount = History.Num();
    
    if (Detection.CheatType == ECheatType::Injection || Detection.CheatType == ECheatType::MemoryModification)
    {
        return EPenaltyLevel::PermanentBan;
    }
    
    if (Detection.SeverityScore >= 90.0f && Detection.Confidence >= EDetectionConfidence::VeryHigh)
    {
        return ViolationCount > 2 ? EPenaltyLevel::PermanentBan : EPenaltyLevel::Suspension;
    }
    
    if (Detection.SeverityScore >= 70.0f && Detection.Confidence >= EDetectionConfidence::High)
    {
        if (ViolationCount == 0)
        {
            return EPenaltyLevel::Warning;
        }
        else if (ViolationCount == 1)
        {
            return EPenaltyLevel::Restriction;
        }
        else
        {
            return EPenaltyLevel::Suspension;
        }
    }
    
    return EPenaltyLevel::Warning;
}

void UMingRTSAntiCheat::LogDetection(const FCheatDetectionResult& Result)
{
    // 寫入日誌文件
    FString LogEntry = FString::Printf(TEXT("[%s] Player: %s, Type: %d, Confidence: %d, Severity: %.1f, Details: %s"),
        *Result.Timestamp, *Result.PlayerID, (int32)Result.CheatType, (int32)Result.Confidence,
        Result.SeverityScore, *Result.DetectionDetails);
    
    UE_LOG(LogTemp, Warning, TEXT("%s"), *LogEntry);
}

void UMingRTSAntiCheat::NotifyServer(const FCheatDetectionResult& Result)
{
    // 向服務器發送檢測通知
    UploadDetectionReport(Result);
}
