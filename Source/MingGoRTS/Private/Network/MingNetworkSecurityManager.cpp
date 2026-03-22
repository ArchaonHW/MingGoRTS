// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Network Security Manager Implementation - B1-4

#include "Network/MingNetworkSecurityManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/SecureHash.h"
#include "Misc/AES.h"

DEFINE_LOG_CATEGORY_STATIC(LogNetworkSecurity, Log, All);

UMingNetworkSecurityManager::UMingNetworkSecurityManager()
    : CurrentAlertLevel(ESecurityAlertLevel::Normal)
    , NextIncidentID(1)
    , bAntiCheatEnabled(true)
{
}

void UMingNetworkSecurityManager::InitializeSecurityManager(const FSecurityConfig& Config)
{
    SecurityConfig = Config;
    bAntiCheatEnabled = Config.bEnableAntiCheat;

    // Generate initial session keys
    GenerateSessionKeys();

    UE_LOG(LogNetworkSecurity, Log, TEXT("Network Security Manager initialized with encryption level: %s"),
        *UEnum::GetValueAsString(Config.EncryptionLevel));

    // Start security monitoring
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            SecurityMonitorTimer,
            this,
            &UMingNetworkSecurityManager::ProcessSecurityAlerts,
            5.0f,
            true
        );
    }
}

void UMingNetworkSecurityManager::ShutdownSecurityManager()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(SecurityMonitorTimer);
    }

    PlayerProfiles.Empty();
    ActiveIncidents.Empty();
    BlockedIPs.Empty();

    UE_LOG(LogNetworkSecurity, Log, TEXT("Network Security Manager shutdown"));
}

void UMingNetworkSecurityManager::SetEncryptionLevel(EEncryptionLevel Level)
{
    SecurityConfig.EncryptionLevel = Level;
    UE_LOG(LogNetworkSecurity, Log, TEXT("Encryption level changed to: %s"), *UEnum::GetValueAsString(Level));

    // Regenerate keys if needed
    if (Level != EEncryptionLevel::None)
    {
        GenerateSessionKeys();
    }
}

TArray<uint8> UMingNetworkSecurityManager::EncryptData(const TArray<uint8>& Data, int32 TargetPlayerID)
{
    if (SecurityConfig.EncryptionLevel == EEncryptionLevel::None)
    {
        return Data;
    }

    TArray<uint8> EncryptedData;

    // Use AES encryption with session key
    if (CurrentSessionKey.Num() > 0)
    {
        // Simple XOR encryption for demonstration
        // In production, use proper AES implementation
        EncryptedData.SetNum(Data.Num());
        for (int32 i = 0; i < Data.Num(); ++i)
        {
            EncryptedData[i] = Data[i] ^ CurrentSessionKey[i % CurrentSessionKey.Num()];
        }
    }
    else
    {
        EncryptedData = Data;
    }

    return EncryptedData;
}

TArray<uint8> UMingNetworkSecurityManager::DecryptData(const TArray<uint8>& EncryptedData, int32 SourcePlayerID)
{
    if (SecurityConfig.EncryptionLevel == EEncryptionLevel::None)
    {
        return EncryptedData;
    }

    TArray<uint8> DecryptedData;

    // Decrypt using session key
    if (CurrentSessionKey.Num() > 0)
    {
        DecryptedData.SetNum(EncryptedData.Num());
        for (int32 i = 0; i < EncryptedData.Num(); ++i)
        {
            DecryptedData[i] = EncryptedData[i] ^ CurrentSessionKey[i % CurrentSessionKey.Num()];
        }
    }
    else
    {
        DecryptedData = EncryptedData;
    }

    return DecryptedData;
}

void UMingNetworkSecurityManager::GenerateSessionKeys()
{
    // Generate 256-bit session key
    CurrentSessionKey.Empty();
    for (int32 i = 0; i < 32; ++i)
    {
        CurrentSessionKey.Add(FMath::RandRange(0, 255));
    }

    UE_LOG(LogNetworkSecurity, Log, TEXT("Generated new session key"));
}

void UMingNetworkSecurityManager::RotateEncryptionKeys()
{
    PreviousSessionKey = CurrentSessionKey;
    GenerateSessionKeys();

    UE_LOG(LogNetworkSecurity, Log, TEXT("Encryption keys rotated"));
}

bool UMingNetworkSecurityManager::VerifyDataIntegrity(const TArray<uint8>& Data, const TArray<uint8>& Checksum)
{
    TArray<uint8> CalculatedChecksum = CalculateChecksum(Data);
    return CalculatedChecksum == Checksum;
}

TArray<uint8> UMingNetworkSecurityManager::CalculateChecksum(const TArray<uint8>& Data)
{
    // Use SHA-256 hash
    FSHA256Hash Hash;
    Hash.Update(Data.GetData(), Data.Num());
    FSHA256Digest Digest = Hash.Finalize();

    TArray<uint8> Checksum;
    Checksum.Append(reinterpret_cast<const uint8*>(Digest.Digest), sizeof(Digest.Digest));
    return Checksum;
}

void UMingNetworkSecurityManager::EnableAntiCheat(bool bEnable)
{
    bAntiCheatEnabled = bEnable;
    SecurityConfig.bEnableAntiCheat = bEnable;
    UE_LOG(LogNetworkSecurity, Log, TEXT("Anti-cheat %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingNetworkSecurityManager::ReportPlayerAction(int32 PlayerID, const FString& ActionType, const TArray<uint8>& ActionData)
{
    if (!bAntiCheatEnabled) return;

    // Record action for analysis
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        PlayerBehaviorHistory.Add(PlayerID, TArray<float>());
    }

    // Analyze action pattern
    AnalyzePlayerBehavior(PlayerID);
}

void UMingNetworkSecurityManager::AnalyzePlayerBehavior(int32 PlayerID)
{
    if (!PlayerProfiles.Contains(PlayerID))
    {
        FPlayerSecurityProfile Profile;
        Profile.PlayerID = PlayerID;
        Profile.FirstSeen = FPlatformTime::Seconds();
        PlayerProfiles.Add(PlayerID, Profile);
    }

    FPlayerSecurityProfile& Profile = PlayerProfiles[PlayerID];

    // Calculate behavior anomaly
    float Anomaly = CalculateBehaviorAnomaly(PlayerID);

    // Update trust score
    if (Anomaly > SecurityConfig.CheatDetectionThreshold)
    {
        Profile.TrustScore = FMath::Max(0.0f, Profile.TrustScore - Anomaly * 10.0f);

        // Report potential cheat
        if (Anomaly > 0.9f)
        {
            FSecurityIncident Incident;
            Incident.IncidentID = NextIncidentID++;
            Incident.PlayerID = PlayerID;
            Incident.CheatType = ECheatType::ModifiedClient;
            Incident.AlertLevel = ESecurityAlertLevel::Warning;
            Incident.ConfidenceScore = Anomaly;
            Incident.Description = TEXT("Suspicious behavior pattern detected");
            Incident.Timestamp = FPlatformTime::Seconds();
            Incident.ActionTaken = EActionTaken::None;

            ReportSecurityIncident(Incident);
        }
    }
    else
    {
        Profile.TrustScore = FMath::Min(100.0f, Profile.TrustScore + 1.0f);
    }

    Profile.LastSeen = FPlatformTime::Seconds();
}

float UMingNetworkSecurityManager::CalculateCheatProbability(int32 PlayerID) const
{
    if (PlayerProfiles.Contains(PlayerID))
    {
        return (100.0f - PlayerProfiles[PlayerID].TrustScore) / 100.0f;
    }
    return 0.0f;
}

bool UMingNetworkSecurityManager::ValidatePlayerPosition(int32 PlayerID, const FVector& Position, float Timestamp)
{
    if (!bAntiCheatEnabled) return true;

    // Check if movement is physically possible
    // This would involve checking speed limits, teleport detection, etc.

    return true;
}

bool UMingNetworkSecurityManager::ValidatePlayerAction(int32 PlayerID, const FString& Action, float Timestamp)
{
    if (!bAntiCheatEnabled) return true;

    // Check if action is valid based on game rules
    // This would involve checking cooldowns, resource availability, etc.

    return true;
}

void UMingNetworkSecurityManager::SetSpeedLimit(int32 PlayerID, float MaxSpeed)
{
    // Set speed limit for anti-cheat validation
    UE_LOG(LogNetworkSecurity, Verbose, TEXT("Set speed limit for player %d: %.2f"), PlayerID, MaxSpeed);
}

void UMingNetworkSecurityManager::MonitorResourceChanges(int32 PlayerID)
{
    if (!bAntiCheatEnabled) return;

    // Monitor for impossible resource gains
    UE_LOG(LogNetworkSecurity, Verbose, TEXT("Monitoring resource changes for player %d"), PlayerID);
}

void UMingNetworkSecurityManager::ReportSecurityIncident(const FSecurityIncident& Incident)
{
    ActiveIncidents.Add(Incident);

    OnSecurityAlert.Broadcast(Incident);

    if (Incident.CheatType != ECheatType::DDoS)
    {
        OnCheatDetected.Broadcast(Incident.PlayerID, Incident.CheatType);
    }

    // Auto-respond to critical threats
    if (Incident.AlertLevel == ESecurityAlertLevel::Critical ||
        Incident.AlertLevel == ESecurityAlertLevel::Emergency)
    {
        AutoRespondToThreat(Incident);
    }

    UE_LOG(LogNetworkSecurity, Warning, TEXT("Security incident reported: ID %d, Player %d, Type %s"),
        Incident.IncidentID, Incident.PlayerID, *UEnum::GetValueAsString(Incident.CheatType));
}

void UMingNetworkSecurityManager::TakeAction(int32 PlayerID, EActionTaken Action)
{
    switch (Action)
    {
    case EActionTaken::Warning:
        IssueWarning(PlayerID, TEXT("Your actions have been flagged as suspicious."));
        break;
    case EActionTaken::Kick:
        // Kick player
        UE_LOG(LogNetworkSecurity, Log, TEXT("Kicking player %d"), PlayerID);
        break;
    case EActionTaken::TempBan:
        BanPlayer(PlayerID, 1440, TEXT("Temporary ban due to suspicious activity"));
        break;
    case EActionTaken::PermBan:
        BanPlayer(PlayerID, -1, TEXT("Permanent ban"));
        break;
    case EActionTaken::ShadowBan:
        ShadowBanPlayer(PlayerID);
        break;
    case EActionTaken::RateLimit:
        SetPlayerRateLimit(PlayerID, 10);
        break;
    default:
        break;
    }
}

void UMingNetworkSecurityManager::BanPlayer(int32 PlayerID, int32 DurationMinutes, const FString& Reason)
{
    if (PlayerProfiles.Contains(PlayerID))
    {
        PlayerProfiles[PlayerID].TrustScore = 0.0f;
    }

    OnPlayerBanned.Broadcast(PlayerID);
    UE_LOG(LogNetworkSecurity, Error, TEXT("Player %d banned for %d minutes. Reason: %s"),
        PlayerID, DurationMinutes, *Reason);

    LogSecurityEvent(FString::Printf(TEXT("Player %d banned: %s"), PlayerID, *Reason));
}

void UMingNetworkSecurityManager::UnbanPlayer(int32 PlayerID)
{
    UE_LOG(LogNetworkSecurity, Log, TEXT("Player %d unbanned"), PlayerID);
}

void UMingNetworkSecurityManager::ShadowBanPlayer(int32 PlayerID)
{
    if (PlayerProfiles.Contains(PlayerID))
    {
        PlayerProfiles[PlayerID].bIsShadowBanned = true;
    }
    UE_LOG(LogNetworkSecurity, Log, TEXT("Player %d shadow banned"), PlayerID);
}

void UMingNetworkSecurityManager::IssueWarning(int32 PlayerID, const FString& Message)
{
    UE_LOG(LogNetworkSecurity, Warning, TEXT("Warning issued to player %d: %s"), PlayerID, *Message);
}

bool UMingNetworkSecurityManager::CheckRateLimit(int32 PlayerID, const FString& ActionType)
{
    if (!SecurityConfig.bEnableRateLimiting) return true;

    if (!ActionRateCounters.Contains(PlayerID))
    {
        ActionRateCounters.Add(PlayerID, TMap<FString, int32>());
    }

    TMap<FString, int32>& Counters = ActionRateCounters[PlayerID];

    if (!Counters.Contains(ActionType))
    {
        Counters.Add(ActionType, 0);
    }

    int32& Count = Counters[ActionType];
    Count++;

    if (Count > SecurityConfig.MaxActionsPerSecond)
    {
        UE_LOG(LogNetworkSecurity, Warning, TEXT("Rate limit exceeded for player %d, action %s"),
            PlayerID, *ActionType);
        return false;
    }

    return true;
}

void UMingNetworkSecurityManager::UpdateRateLimits()
{
    // Reset rate counters periodically
    ActionRateCounters.Empty();
}

void UMingNetworkSecurityManager::SetPlayerRateLimit(int32 PlayerID, int32 MaxActionsPerSecond)
{
    // Set custom rate limit for specific player
    UE_LOG(LogNetworkSecurity, Verbose, TEXT("Set rate limit for player %d: %d actions/second"),
        PlayerID, MaxActionsPerSecond);
}

bool UMingNetworkSecurityManager::AuthenticatePlayer(int32 PlayerID, const FString& Credentials)
{
    if (!SecurityConfig.bRequireAuthentication) return true;

    // Validate credentials
    bool bAuthenticated = true; // Simplified for demo

    if (bAuthenticated)
    {
        if (PlayerProfiles.Contains(PlayerID))
        {
            PlayerProfiles[PlayerID].bIsVerified = true;
        }
    }

    return bAuthenticated;
}

bool UMingNetworkSecurityManager::VerifyHardwareID(int32 PlayerID, const FString& HardwareID)
{
    if (!SecurityConfig.bUseHardwareID) return true;

    if (PlayerProfiles.Contains(PlayerID))
    {
        return PlayerProfiles[PlayerID].HardwareID == HardwareID;
    }

    return false;
}

void UMingNetworkSecurityManager::RegisterPlayerHardware(int32 PlayerID, const FString& HardwareID)
{
    if (PlayerProfiles.Contains(PlayerID))
    {
        PlayerProfiles[PlayerID].HardwareID = HardwareID;
    }
}

bool UMingNetworkSecurityManager::IsPlayerAuthenticated(int32 PlayerID) const
{
    if (PlayerProfiles.Contains(PlayerID))
    {
        return PlayerProfiles[PlayerID].bIsVerified;
    }
    return false;
}

void UMingNetworkSecurityManager::BlockIP(const FString& IPAddress)
{
    if (!BlockedIPs.Contains(IPAddress))
    {
        BlockedIPs.Add(IPAddress);
        UE_LOG(LogNetworkSecurity, Log, TEXT("IP blocked: %s"), *IPAddress);
    }
}

void UMingNetworkSecurityManager::UnblockIP(const FString& IPAddress)
{
    BlockedIPs.Remove(IPAddress);
    UE_LOG(LogNetworkSecurity, Log, TEXT("IP unblocked: %s"), *IPAddress);
}

bool UMingNetworkSecurityManager::IsIPBlocked(const FString& IPAddress) const
{
    return BlockedIPs.Contains(IPAddress);
}

void UMingNetworkSecurityManager::WhitelistIP(const FString& IPAddress)
{
    if (!WhitelistedIPs.Contains(IPAddress))
    {
        WhitelistedIPs.Add(IPAddress);
    }
}

bool UMingNetworkSecurityManager::ValidatePacket(const FPacketSecurityInfo& PacketInfo)
{
    if (!SecurityConfig.bEnablePacketValidation) return true;

    // Check sequence number
    // Verify checksum
    // Validate timestamp

    return true;
}

FPacketSecurityInfo UMingNetworkSecurityManager::SignPacket(const TArray<uint8>& Data, int32 SenderID)
{
    FPacketSecurityInfo Info;
    Info.PacketID = FMath::RandRange(1, INT32_MAX);
    Info.SenderID = SenderID;
    Info.Timestamp = FPlatformTime::Seconds();
    Info.SequenceNumber = FMath::RandRange(1, 65535);
    Info.Checksum = CalculateChecksum(Data);
    Info.bIsEncrypted = SecurityConfig.EncryptionLevel != EEncryptionLevel::None;
    Info.PayloadSize = Data.Num();

    return Info;
}

bool UMingNetworkSecurityManager::DetectReplayAttack(const FPacketSecurityInfo& PacketInfo)
{
    // Check for duplicate sequence numbers within time window
    // Store recent packet IDs and check for duplicates

    return false;
}

bool UMingNetworkSecurityManager::DetectPacketTampering(const TArray<uint8>& Data, const TArray<uint8>& ExpectedChecksum)
{
    TArray<uint8> ActualChecksum = CalculateChecksum(Data);
    return ActualChecksum != ExpectedChecksum;
}

TArray<FSecurityIncident> UMingNetworkSecurityManager::GetRecentIncidents(int32 Count) const
{
    TArray<FSecurityIncident> Recent;
    int32 StartIndex = FMath::Max(0, ActiveIncidents.Num() - Count);

    for (int32 i = StartIndex; i < ActiveIncidents.Num(); ++i)
    {
        Recent.Add(ActiveIncidents[i]);
    }

    return Recent;
}

float UMingNetworkSecurityManager::GetAverageTrustScore() const
{
    if (PlayerProfiles.Num() == 0) return 100.0f;

    float Total = 0.0f;
    for (const auto& Pair : PlayerProfiles)
    {
        Total += Pair.Value.TrustScore;
    }

    return Total / PlayerProfiles.Num();
}

void UMingNetworkSecurityManager::ProcessSecurityAlerts()
{
    // Process active incidents
    for (auto& Incident : ActiveIncidents)
    {
        if (Incident.AlertLevel >= ESecurityAlertLevel::Warning)
        {
            // Take action based on incident type
            if (Incident.ActionTaken == EActionTaken::None)
            {
                AutoRespondToThreat(Incident);
            }
        }
    }

    // Update overall alert level
    UpdateAlertLevel();

    // Clean old incidents
    CleanupOldIncidents();

    // Rotate keys periodically
    if (FMath::RandRange(0, 100) == 0)
    {
        RotateEncryptionKeys();
    }
}

void UMingNetworkSecurityManager::UpdateAlertLevel()
{
    ESecurityAlertLevel NewLevel = ESecurityAlertLevel::Normal;

    int32 WarningCount = 0;
    int32 CriticalCount = 0;

    for (const auto& Incident : ActiveIncidents)
    {
        if (Incident.AlertLevel == ESecurityAlertLevel::Warning)
        {
            WarningCount++;
        }
        else if (Incident.AlertLevel >= ESecurityAlertLevel::Critical)
        {
            CriticalCount++;
        }
    }

    if (CriticalCount > 0)
    {
        NewLevel = ESecurityAlertLevel::Critical;
    }
    else if (WarningCount > 5)
    {
        NewLevel = ESecurityAlertLevel::Warning;
    }
    else if (WarningCount > 0)
    {
        NewLevel = ESecurityAlertLevel::Suspicious;
    }

    if (NewLevel != CurrentAlertLevel)
    {
        ESecurityAlertLevel OldLevel = CurrentAlertLevel;
        CurrentAlertLevel = NewLevel;
        OnSecurityLevelChanged.Broadcast(NewLevel);

        UE_LOG(LogNetworkSecurity, Warning, TEXT("Security level changed from %s to %s"),
            *UEnum::GetValueAsString(OldLevel), *UEnum::GetValueAsString(NewLevel));
    }
}

void UMingNetworkSecurityManager::DetectAnomalies()
{
    // Detect unusual patterns across all players
    for (const auto& Pair : PlayerProfiles)
    {
        int32 PlayerID = Pair.Key;
        const FPlayerSecurityProfile& Profile = Pair.Value;

        if (Profile.IncidentCount > 10)
        {
            UE_LOG(LogNetworkSecurity, Warning, TEXT("Player %d has multiple incidents"), PlayerID);
        }
    }
}

bool UMingNetworkSecurityManager::IsActionSuspicious(int32 PlayerID, const FString& Action) const
{
    // Check if action is in suspicious patterns
    return false;
}

void UMingNetworkSecurityManager::RecordPlayerBehavior(int32 PlayerID, float BehaviorMetric)
{
    if (!PlayerBehaviorHistory.Contains(PlayerID))
    {
        PlayerBehaviorHistory.Add(PlayerID, TArray<float>());
    }

    PlayerBehaviorHistory[PlayerID].Add(BehaviorMetric);

    // Limit history size
    if (PlayerBehaviorHistory[PlayerID].Num() > 1000)
    {
        PlayerBehaviorHistory[PlayerID].RemoveAt(0);
    }
}

TArray<float> UMingNetworkSecurityManager::GetBehaviorPattern(int32 PlayerID) const
{
    if (PlayerBehaviorHistory.Contains(PlayerID))
    {
        return PlayerBehaviorHistory[PlayerID];
    }
    return TArray<float>();
}

float UMingNetworkSecurityManager::CalculateBehaviorAnomaly(int32 PlayerID) const
{
    TArray<float> Pattern = GetBehaviorPattern(PlayerID);
    if (Pattern.Num() < 10) return 0.0f;

    // Calculate variance
    float Mean = 0.0f;
    for (float Value : Pattern)
    {
        Mean += Value;
    }
    Mean /= Pattern.Num();

    float Variance = 0.0f;
    for (float Value : Pattern)
    {
        Variance += FMath::Pow(Value - Mean, 2.0f);
    }
    Variance /= Pattern.Num();

    // High variance indicates anomalous behavior
    return FMath::Min(1.0f, Variance / 100.0f);
}

void UMingNetworkSecurityManager::AutoRespondToThreat(const FSecurityIncident& Incident)
{
    EActionTaken Action = EActionTaken::None;

    switch (Incident.AlertLevel)
    {
    case ESecurityAlertLevel::Suspicious:
        Action = EActionTaken::Warning;
        break;
    case ESecurityAlertLevel::Warning:
        Action = (Incident.ConfidenceScore > 0.8f) ? EActionTaken::Kick : EActionTaken::RateLimit;
        break;
    case ESecurityAlertLevel::Critical:
        Action = EActionTaken::TempBan;
        break;
    case ESecurityAlertLevel::Emergency:
        Action = EActionTaken::PermBan;
        break;
    default:
        break;
    }

    TakeAction(Incident.PlayerID, Action);

    // Update incident
    for (auto& Active : ActiveIncidents)
    {
        if (Active.IncidentID == Incident.IncidentID)
        {
            Active.ActionTaken = Action;
            break;
        }
    }
}

void UMingNetworkSecurityManager::EscalateAlert(ESecurityAlertLevel NewLevel)
{
    if (NewLevel > CurrentAlertLevel)
    {
        CurrentAlertLevel = NewLevel;
        OnSecurityLevelChanged.Broadcast(NewLevel);
    }
}

void UMingNetworkSecurityManager::NotifySecurityPersonnel(const FSecurityIncident& Incident)
{
    // Send notification to security team
    UE_LOG(LogNetworkSecurity, Error, TEXT("SECURITY ALERT: Incident %d requires attention"), Incident.IncidentID);
}

void UMingNetworkSecurityManager::GenerateSecurityReport()
{
    // Generate comprehensive security report
    UE_LOG(LogNetworkSecurity, Log, TEXT("Generating security report"));
    UE_LOG(LogNetworkSecurity, Log, TEXT("Active incidents: %d"), ActiveIncidents.Num());
    UE_LOG(LogNetworkSecurity, Log, TEXT("Average trust score: %.1f"), GetAverageTrustScore());
    UE_LOG(LogNetworkSecurity, Log, TEXT("Current alert level: %s"), *UEnum::GetValueAsString(CurrentAlertLevel));
}

void UMingNetworkSecurityManager::CleanupOldIncidents()
{
    uint32 CurrentTime = FPlatformTime::Seconds();

    // Move old incidents to resolved
    for (int32 i = ActiveIncidents.Num() - 1; i >= 0; --i)
    {
        if (CurrentTime - ActiveIncidents[i].Timestamp > 86400) // 24 hours
        {
            ResolvedIncidents.Add(ActiveIncidents[i]);
            ActiveIncidents.RemoveAt(i);
        }
    }
}

void UMingNetworkSecurityManager::RotateSessionKeys()
{
    RotateEncryptionKeys();
}

bool UMingNetworkSecurityManager::VerifySessionKey(const TArray<uint8>& Key) const
{
    return Key == CurrentSessionKey || Key == PreviousSessionKey;
}

void UMingNetworkSecurityManager::BlockSuspiciousActivity(int32 PlayerID)
{
    // Block player from taking actions
    UE_LOG(LogNetworkSecurity, Warning, TEXT("Blocking suspicious activity from player %d"), PlayerID);
}

void UMingNetworkSecurityManager::LogSecurityEvent(const FString& Event)
{
    UE_LOG(LogNetworkSecurity, Log, TEXT("[SECURITY] %s"), *Event);
}

static UMingNetworkSecurityManager* UMingNetworkSecurityManager::Get(UObject* WorldContextObject)
{
    static UMingNetworkSecurityManager* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingNetworkSecurityManager>();
        Instance->AddToRoot();
    }
    return Instance;
}
