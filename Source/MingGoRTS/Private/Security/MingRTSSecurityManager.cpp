#include "Security/MingRTSSecurityManager.h"
#include "Security/MingRTSAntiCheat.h"
#include "Security/MingRTSServerValidation.h"
#include "Misc/DateTime.h"

UMingRTSSecurityManager::UMingRTSSecurityManager()
{
}

void UMingRTSSecurityManager::InitializeSecurityManager()
{
    InitializeSubSystems();
    UE_LOG(LogTemp, Log, TEXT("Security Manager Initialized"));
}

void UMingRTSSecurityManager::InitializeSubSystems()
{
    // 創建並初始化反作弊系統
    AntiCheatSystem = NewObject<UMingRTSAntiCheat>();
    if (AntiCheatSystem)
    {
        AntiCheatSystem->InitializeAntiCheat();
        
        // 綁定反作弊事件
        AntiCheatSystem->OnCheatDetected.AddDynamic(this, &UMingRTSSecurityManager::OnCheatDetectedHandler);
    }
    
    // 創建並初始化服務器驗證系統
    ServerValidation = NewObject<UMingRTSServerValidation>();
    if (ServerValidation)
    {
        ServerValidation->InitializeServerValidation();
        
        // 綁定驗證失敗事件
        ServerValidation->OnValidationFailed.AddDynamic(this, &UMingRTSSecurityManager::OnValidationFailedHandler);
    }
}

UMingRTSAntiCheat* UMingRTSSecurityManager::GetAntiCheatSystem() const
{
    return AntiCheatSystem;
}

UMingRTSServerValidation* UMingRTSSecurityManager::GetServerValidation() const
{
    return ServerValidation;
}

void UMingRTSSecurityManager::StartAllSecuritySystems()
{
    if (AntiCheatSystem)
    {
        AntiCheatSystem->StartMonitoring();
    }
    
    UE_LOG(LogTemp, Log, TEXT("All security systems started"));
}

void UMingRTSSecurityManager::StopAllSecuritySystems()
{
    if (AntiCheatSystem)
    {
        AntiCheatSystem->StopMonitoring();
    }
    
    UE_LOG(LogTemp, Log, TEXT("All security systems stopped"));
}

void UMingRTSSecurityManager::ReportSecurityAlert(ESecurityAlertLevel Level, const FText& Title, const FText& Message)
{
    FSecurityAlert Alert;
    Alert.AlertID = FName(*FString::Printf(TEXT("ALERT_%s"), *FDateTime::Now().ToString()));
    Alert.Level = Level;
    Alert.Title = Title;
    Alert.Message = Message;
    Alert.Timestamp = FDateTime::Now().ToString();
    Alert.bIsAcknowledged = false;
    
    SecurityAlerts.Add(Alert);
    OnSecurityAlert.Broadcast(Alert);
    
    // 根據級別記錄日誌
    switch (Level)
    {
    case ESecurityAlertLevel::Critical:
        UE_LOG(LogTemp, Error, TEXT("[CRITICAL] %s: %s"), *Title.ToString(), *Message.ToString());
        break;
    case ESecurityAlertLevel::High:
        UE_LOG(LogTemp, Error, TEXT("[HIGH] %s: %s"), *Title.ToString(), *Message.ToString());
        break;
    case ESecurityAlertLevel::Medium:
        UE_LOG(LogTemp, Warning, TEXT("[MEDIUM] %s: %s"), *Title.ToString(), *Message.ToString());
        break;
    default:
        UE_LOG(LogTemp, Log, TEXT("[%s] %s: %s"), 
            Level == ESecurityAlertLevel::Low ? TEXT("LOW") : TEXT("INFO"),
            *Title.ToString(), *Message.ToString());
        break;
    }
}

TArray<FSecurityAlert> UMingRTSSecurityManager::GetAllAlerts() const
{
    return SecurityAlerts;
}

TArray<FSecurityAlert> UMingRTSSecurityManager::GetUnacknowledgedAlerts() const
{
    TArray<FSecurityAlert> Unacknowledged;
    
    for (const FSecurityAlert& Alert : SecurityAlerts)
    {
        if (!Alert.bIsAcknowledged)
        {
            Unacknowledged.Add(Alert);
        }
    }
    
    return Unacknowledged;
}

void UMingRTSSecurityManager::AcknowledgeAlert(FName AlertID)
{
    for (FSecurityAlert& Alert : SecurityAlerts)
    {
        if (Alert.AlertID == AlertID)
        {
            Alert.bIsAcknowledged = true;
            UE_LOG(LogTemp, Log, TEXT("Alert acknowledged: %s"), *AlertID.ToString());
            break;
        }
    }
}

void UMingRTSSecurityManager::ClearAlert(FName AlertID)
{
    SecurityAlerts.RemoveAll([AlertID](const FSecurityAlert& Alert) {
        return Alert.AlertID == AlertID;
    });
}

bool UMingRTSSecurityManager::IsPlayerSecure(const FString& PlayerID) const
{
    if (!AntiCheatSystem)
    {
        return true;
    }
    
    // 檢查玩家是否被封禁
    if (AntiCheatSystem->IsPlayerBanned(PlayerID))
    {
        return false;
    }
    
    // 檢查玩家安全評分
    float Score = GetPlayerSecurityScore(PlayerID);
    return Score >= 0.7f;
}

float UMingRTSSecurityManager::GetPlayerSecurityScore(const FString& PlayerID) const
{
    const float* Score = PlayerSecurityScores.Find(PlayerID);
    return Score ? *Score : 1.0f;
}

FString UMingRTSSecurityManager::GenerateSecurityReport() const
{
    FString Report = TEXT("=== MingGoRTS Security Report ===\n");
    Report += FString::Printf(TEXT("Generated: %s\n\n"), *FDateTime::Now().ToString());
    
    // 統計警報
    int32 CriticalCount = 0;
    int32 HighCount = 0;
    int32 MediumCount = 0;
    int32 LowCount = 0;
    int32 InfoCount = 0;
    int32 UnacknowledgedCount = 0;
    
    for (const FSecurityAlert& Alert : SecurityAlerts)
    {
        switch (Alert.Level)
        {
        case ESecurityAlertLevel::Critical: CriticalCount++; break;
        case ESecurityAlertLevel::High: HighCount++; break;
        case ESecurityAlertLevel::Medium: MediumCount++; break;
        case ESecurityAlertLevel::Low: LowCount++; break;
        case ESecurityAlertLevel::Info: InfoCount++; break;
        }
        
        if (!Alert.bIsAcknowledged)
        {
            UnacknowledgedCount++;
        }
    }
    
    Report += TEXT("Alert Summary:\n");
    Report += FString::Printf(TEXT("  Critical: %d\n"), CriticalCount);
    Report += FString::Printf(TEXT("  High: %d\n"), HighCount);
    Report += FString::Printf(TEXT("  Medium: %d\n"), MediumCount);
    Report += FString::Printf(TEXT("  Low: %d\n"), LowCount);
    Report += FString::Printf(TEXT("  Info: %d\n"), InfoCount);
    Report += FString::Printf(TEXT("  Unacknowledged: %d\n\n"), UnacknowledgedCount);
    
    // 系統狀態
    Report += TEXT("System Status:\n");
    Report += FString::Printf(TEXT("  Anti-Cheat: %s\n"), AntiCheatSystem ? TEXT("Active") : TEXT("Inactive"));
    Report += FString::Printf(TEXT("  Server Validation: %s\n\n"), ServerValidation ? TEXT("Active") : TEXT("Inactive"));
    
    // 未確認警報詳情
    if (UnacknowledgedCount > 0)
    {
        Report += TEXT("Unacknowledged Alerts:\n");
        for (const FSecurityAlert& Alert : SecurityAlerts)
        {
            if (!Alert.bIsAcknowledged)
            {
                FString LevelStr;
                switch (Alert.Level)
                {
                case ESecurityAlertLevel::Critical: LevelStr = TEXT("CRITICAL"); break;
                case ESecurityAlertLevel::High: LevelStr = TEXT("HIGH"); break;
                case ESecurityAlertLevel::Medium: LevelStr = TEXT("MEDIUM"); break;
                case ESecurityAlertLevel::Low: LevelStr = TEXT("LOW"); break;
                default: LevelStr = TEXT("INFO"); break;
                }
                
                Report += FString::Printf(TEXT("  [%s] %s - %s\n"), 
                    *LevelStr, *Alert.Title.ToString(), *Alert.Timestamp);
            }
        }
    }
    
    return Report;
}

void UMingRTSSecurityManager::OnCheatDetectedHandler(const FCheatDetectionResult& Detection)
{
    ESecurityAlertLevel Level = ESecurityAlertLevel::Medium;
    
    // 根據作弊類型和置信度確定警報級別
    if (Detection.Confidence == EDetectionConfidence::VeryHigh)
    {
        Level = ESecurityAlertLevel::Critical;
    }
    else if (Detection.Confidence == EDetectionConfidence::High)
    {
        Level = ESecurityAlertLevel::High;
    }
    
    FText Title = FText::Format(FText::FromString(TEXT("Cheat Detected: {0}")), 
        FText::FromString(StaticEnum<ECheatType>()->GetNameStringByValue((int64)Detection.CheatType)));
    
    FText Message = FText::Format(FText::FromString(TEXT("Player: {0}\nDetails: {1}\nConfidence: {2}")),
        FText::FromString(Detection.PlayerID),
        FText::FromString(Detection.DetectionDetails),
        FText::FromString(StaticEnum<EDetectionConfidence>()->GetNameStringByValue((int64)Detection.Confidence)));
    
    ReportSecurityAlert(Level, Title, Message);
    
    // 更新玩家安全評分
    float CurrentScore = GetPlayerSecurityScore(Detection.PlayerID);
    float NewScore = CurrentScore * (1.0f - (Detection.SeverityScore / 100.0f) * 0.5f);
    PlayerSecurityScores.Add(Detection.PlayerID, FMath::Max(NewScore, 0.0f));
}

void UMingRTSSecurityManager::OnValidationFailedHandler(const FString& PlayerID, const FValidationResult& Result)
{
    ESecurityAlertLevel Level = ESecurityAlertLevel::Medium;
    
    if (Result.ConfidenceScore < 0.3f)
    {
        Level = ESecurityAlertLevel::High;
    }
    else if (Result.ConfidenceScore < 0.5f)
    {
        Level = ESecurityAlertLevel::Medium;
    }
    else
    {
        Level = ESecurityAlertLevel::Low;
    }
    
    FText Title = FText::FromString(TEXT("Validation Failed"));
    
    FString ViolationsStr;
    for (const FString& Violation : Result.Violations)
    {
        ViolationsStr += Violation + TEXT("\n");
    }
    
    FText Message = FText::Format(FText::FromString(TEXT("Player: {0}\nViolations:\n{1}")),
        FText::FromString(PlayerID),
        FText::FromString(ViolationsStr));
    
    ReportSecurityAlert(Level, Title, Message);
    
    // 更新玩家安全評分
    float CurrentScore = GetPlayerSecurityScore(PlayerID);
    float NewScore = CurrentScore * Result.ConfidenceScore;
    PlayerSecurityScores.Add(PlayerID, FMath::Max(NewScore, 0.0f));
}
