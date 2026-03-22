#include "Security/MingRTSServerValidation.h"
#include "Misc/DateTime.h"

UMingRTSServerValidation::UMingRTSServerValidation()
    : bStrictMode(false)
{
}

void UMingRTSServerValidation::InitializeServerValidation()
{
    UE_LOG(LogTemp, Log, TEXT("Server Validation System Initialized"));
}

FClientCommandValidation UMingRTSServerValidation::ValidateClientCommand(const FString& PlayerID, 
    const FString& CommandType, const TArray<uint8>& CommandData)
{
    FClientCommandValidation Validation;
    Validation.PlayerID = PlayerID;
    Validation.CommandType = CommandType;
    Validation.Timestamp = FDateTime::Now().GetTimeOfDay().GetTotalSeconds();
    
    // 檢查序列號
    int32* LastSeq = LastSequenceNumbers.Find(PlayerID);
    int32 ExpectedSeq = LastSeq ? (*LastSeq + 1) : 1;
    
    // 解析命令數據獲取序列號
    if (CommandData.Num() >= 4)
    {
        Validation.SequenceNumber = FMath::BytesToInt(CommandData.GetData());
    }
    
    // 驗證序列號
    if (Validation.SequenceNumber != ExpectedSeq)
    {
        Validation.bIsValid = false;
        Validation.RejectionReason = FString::Printf(TEXT("Sequence mismatch. Expected %d, got %d"), 
            ExpectedSeq, Validation.SequenceNumber);
        
        LogValidationFailure(PlayerID, FValidationResult());
    }
    else
    {
        // 記錄序列號
        LastSequenceNumbers.Add(PlayerID, Validation.SequenceNumber);
        
        // 執行命令特定驗證
        if (bStrictMode)
        {
            // 獲取玩家當前狀態
            FGameStateSnapshot* State = AuthoritativeStates.Find(PlayerID);
            if (State)
            {
                Validation.bIsValid = IsCommandAllowed(CommandType, *State);
                if (!Validation.bIsValid)
                {
                    Validation.RejectionReason = TEXT("Command not allowed in current state");
                }
            }
        }
    }
    
    // 記錄命令
    RecordClientCommand(PlayerID, Validation.CommandID, CommandType);
    
    return Validation;
}

FValidationResult UMingRTSServerValidation::ValidateGameState(const FGameStateSnapshot& ClientSnapshot)
{
    FValidationResult Result;
    Result.Timestamp = FDateTime::Now().ToString();
    
    // 獲取服務器權威狀態
    FGameStateSnapshot ServerState = GetAuthoritativeState(ClientSnapshot.MatchID, ClientSnapshot.PlayerID);
    
    // 比較各種校驗和
    if (!VerifyStateChecksum(ClientSnapshot.ResourceChecksum, ServerState.ResourceChecksum))
    {
        Result.bIsValid = false;
        Result.Violations.Add(TEXT("Resource checksum mismatch"));
        Result.ConfidenceScore -= 0.2f;
    }
    
    if (!VerifyStateChecksum(ClientSnapshot.UnitChecksum, ServerState.UnitChecksum))
    {
        Result.bIsValid = false;
        Result.Violations.Add(TEXT("Unit checksum mismatch"));
        Result.ConfidenceScore -= 0.2f;
    }
    
    if (!VerifyStateChecksum(ClientSnapshot.BuildingChecksum, ServerState.BuildingChecksum))
    {
        Result.bIsValid = false;
        Result.Violations.Add(TEXT("Building checksum mismatch"));
        Result.ConfidenceScore -= 0.2f;
    }
    
    // 檢查異常值
    TArray<FString> Anomalies = DetectAnomalousState(ClientSnapshot);
    if (Anomalies.Num() > 0)
    {
        Result.bIsValid = false;
        Result.Violations.Append(Anomalies);
        Result.ConfidenceScore -= (Anomalies.Num() * 0.1f);
    }
    
    Result.ConfidenceScore = FMath::Clamp(Result.ConfidenceScore, 0.0f, 1.0f);
    
    if (!Result.bIsValid)
    {
        Result.ValidationMessage = TEXT("State validation failed");
        OnStateMismatch.Broadcast(ClientSnapshot.MatchID, Result);
        LogValidationFailure(ClientSnapshot.PlayerID, Result);
        
        // 如果嚴重不匹配，報告給反作弊系統
        if (Result.ConfidenceScore < 0.5f)
        {
            ReportToAntiCheat(ClientSnapshot.PlayerID, Result);
        }
    }
    
    return Result;
}

bool UMingRTSServerValidation::VerifyStateChecksum(int32 ClientChecksum, int32 ServerChecksum)
{
    // 在嚴格模式下必須完全匹配
    // 在正常模式下允許小差異（考慮網絡延遲導致的狀態差異）
    
    if (bStrictMode)
    {
        return ClientChecksum == ServerChecksum;
    }
    else
    {
        // 允許一定的誤差範圍
        const int32 TOLERANCE = 100;
        return FMath::Abs(ClientChecksum - ServerChecksum) <= TOLERANCE;
    }
}

void UMingRTSServerValidation::RecordClientCommand(const FString& PlayerID, const FString& CommandID, 
    const FString& CommandType)
{
    FClientCommandValidation Record;
    Record.CommandID = CommandID;
    Record.PlayerID = PlayerID;
    Record.CommandType = CommandType;
    Record.Timestamp = FDateTime::Now().GetTimeOfDay().GetTotalSeconds();
    Record.bIsValid = true;
    
    TArray<FClientCommandValidation>* History = CommandHistory.Find(PlayerID);
    if (!History)
    {
        CommandHistory.Add(PlayerID, TArray<FClientCommandValidation>());
        History = CommandHistory.Find(PlayerID);
    }
    
    History->Add(Record);
    
    // 限制歷史記錄大小
    if (History->Num() > 1000)
    {
        History->RemoveAt(0);
    }
}

bool UMingRTSServerValidation::VerifyCommandSequence(const FString& PlayerID, int32 ExpectedSequence)
{
    int32* LastSeq = LastSequenceNumbers.Find(PlayerID);
    if (!LastSeq)
    {
        return ExpectedSequence == 1;
    }
    
    return (*LastSeq + 1) == ExpectedSequence;
}

TArray<FString> UMingRTSServerValidation::DetectAnomalousState(const FGameStateSnapshot& Snapshot)
{
    TArray<FString> Anomalies;
    
    // 檢查資源異常
    if (Snapshot.ResourceChecksum > 1000000)
    {
        Anomalies.Add(TEXT("Abnormally high resource checksum"));
    }
    
    // 檢查單位數量異常
    if (Snapshot.UnitChecksum > 500000)
    {
        Anomalies.Add(TEXT("Abnormally high unit checksum"));
    }
    
    // 檢查遊戲時間異常
    if (Snapshot.GameTime < 0 || Snapshot.GameTime > 86400) // 超過24小時
    {
        Anomalies.Add(TEXT("Abnormal game time"));
    }
    
    return Anomalies;
}

FGameStateSnapshot UMingRTSServerValidation::GetAuthoritativeState(const FString& MatchID, const FString& PlayerID)
{
    FString Key = MatchID + TEXT("_") + PlayerID;
    FGameStateSnapshot* State = AuthoritativeStates.Find(Key);
    
    if (State)
    {
        return *State;
    }
    
    // 創建新的權威狀態
    FGameStateSnapshot NewState;
    NewState.MatchID = MatchID;
    NewState.PlayerID = PlayerID;
    NewState.FrameNumber = 0;
    NewState.GameTime = 0.0f;
    NewState.ResourceChecksum = 0;
    NewState.UnitChecksum = 0;
    NewState.BuildingChecksum = 0;
    NewState.Timestamp = FDateTime::Now().ToString();
    
    return NewState;
}

void UMingRTSServerValidation::ApplyServerCorrection(const FString& PlayerID, const FGameStateSnapshot& CorrectedState)
{
    FString Key = CorrectedState.MatchID + TEXT("_") + PlayerID;
    AuthoritativeStates.Add(Key, CorrectedState);
    
    UE_LOG(LogTemp, Log, TEXT("Applied server correction for player %s"), *PlayerID);
}

void UMingRTSServerValidation::StartSessionValidation(const FString& MatchID)
{
    if (!ActiveSessions.Contains(MatchID))
    {
        ActiveSessions.Add(MatchID);
        UE_LOG(LogTemp, Log, TEXT("Started session validation for match %s"), *MatchID);
    }
}

void UMingRTSServerValidation::EndSessionValidation(const FString& MatchID)
{
    ActiveSessions.Remove(MatchID);
    
    // 清理該會話的數據
    TArray<FString> KeysToRemove;
    for (const auto& Pair : AuthoritativeStates)
    {
        if (Pair.Value.MatchID == MatchID)
        {
            KeysToRemove.Add(Pair.Key);
        }
    }
    
    for (const FString& Key : KeysToRemove)
    {
        AuthoritativeStates.Remove(Key);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Ended session validation for match %s"), *MatchID);
}

void UMingRTSServerValidation::SetStrictValidation(bool bStrict)
{
    bStrictMode = bStrict;
    UE_LOG(LogTemp, Log, TEXT("Server validation strict mode: %s"), bStrict ? TEXT("Enabled") : TEXT("Disabled"));
}

int32 UMingRTSServerValidation::CalculateResourceChecksum(const FString& MatchID, const FString& PlayerID)
{
    // 計算資源數據的校驗和
    // 實際實現需要訪問遊戲狀態
    return 0;
}

int32 UMingRTSServerValidation::CalculateUnitChecksum(const FString& MatchID, const FString& PlayerID)
{
    // 計算單位數據的校驗和
    return 0;
}

int32 UMingRTSServerValidation::CalculateBuildingChecksum(const FString& MatchID, const FString& PlayerID)
{
    // 計算建築數據的校驗和
    return 0;
}

bool UMingRTSServerValidation::IsCommandAllowed(const FString& CommandType, const FGameStateSnapshot& State)
{
    // 檢查命令是否允許在當前狀態下執行
    // 這裡可以添加各種規則檢查
    
    if (CommandType == TEXT("BuildUnit"))
    {
        return State.ResourceChecksum > 0;
    }
    
    if (CommandType == TEXT("Move"))
    {
        return State.UnitChecksum > 0;
    }
    
    return true;
}

bool UMingRTSServerValidation::IsResourceAmountValid(int32 ResourceAmount, const FGameStateSnapshot& State)
{
    // 檢查資源數量是否合理
    const int32 MAX_REASONABLE_RESOURCES = 100000;
    return ResourceAmount >= 0 && ResourceAmount <= MAX_REASONABLE_RESOURCES;
}

bool UMingRTSServerValidation::IsUnitCountValid(int32 UnitCount, const FGameStateSnapshot& State)
{
    // 檢查單位數量是否合理
    const int32 MAX_REASONABLE_UNITS = 500;
    return UnitCount >= 0 && UnitCount <= MAX_REASONABLE_UNITS;
}

bool UMingRTSServerValidation::IsPositionValid(const FVector& Position, const FGameStateSnapshot& State)
{
    // 檢查位置是否在有效範圍內
    const float MAX_MAP_SIZE = 100000.0f;
    return FMath::Abs(Position.X) <= MAX_MAP_SIZE && 
           FMath::Abs(Position.Y) <= MAX_MAP_SIZE && 
           FMath::Abs(Position.Z) <= MAX_MAP_SIZE;
}

void UMingRTSServerValidation::LogValidationFailure(const FString& PlayerID, const FValidationResult& Result)
{
    UE_LOG(LogTemp, Warning, TEXT("Validation failed for player %s: %s"), 
        *PlayerID, *Result.ValidationMessage);
    
    for (const FString& Violation : Result.Violations)
    {
        UE_LOG(LogTemp, Warning, TEXT("  - %s"), *Violation);
    }
}

void UMingRTSServerValidation::ReportToAntiCheat(const FString& PlayerID, const FValidationResult& Result)
{
    // 將嚴重的驗證失敗報告給反作弊系統
    UE_LOG(LogTemp, Error, TEXT("Reporting validation failure to anti-cheat for player %s"), *PlayerID);
}
