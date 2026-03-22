#include "MingBlockchainMiningSystem.h"
#include "MingBlockchainWallet.h"
#include "MingBlockchainTransactionManager.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UMingBlockchainMiningSystem::UMingBlockchainMiningSystem()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    CurrentDifficulty = EMiningDifficulty::Medium;
    bMiningEnabled = true;
    CurrentHashRate = 0.0f;
    MiningEfficiencyBonus = 1.0f;
    BaseRewardAmount = 10.0f;
    ProgressUpdateInterval = 1.0f;
    MaxConcurrentSessions = 1;
    TimeSinceLastProgressUpdate = 0.0f;

    BlockchainWallet = nullptr;
    TransactionManager = nullptr;
}

void UMingBlockchainMiningSystem::BeginPlay()
{
    Super::BeginPlay();
    InitializeMiningConfiguration();
}

void UMingBlockchainMiningSystem::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Stop all active sessions
    for (FMiningSession& Session : ActiveSessions)
    {
        StopMiningSession(Session.SessionID);
    }
    
    Super::EndPlay(EndPlayReason);
}

void UMingBlockchainMiningSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bMiningEnabled && ActiveSessions.Num() > 0)
    {
        ProcessMiningTick(DeltaTime);
    }
}

void UMingBlockchainMiningSystem::InitializeMiningSystem(UMingBlockchainWallet* Wallet, UMingBlockchainTransactionManager* InTransactionManager)
{
    BlockchainWallet = Wallet;
    TransactionManager = InTransactionManager;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Initialized with wallet and transaction manager"));
}

FString UMingBlockchainMiningSystem::StartMiningSession(const FString& PlayerID, EMiningAlgorithm Algorithm, EMiningDifficulty Difficulty)
{
    if (!bMiningEnabled)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainMiningSystem: Mining is disabled"));
        return TEXT("");
    }

    if (ActiveSessions.Num() >= MaxConcurrentSessions)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingBlockchainMiningSystem: Maximum concurrent sessions reached"));
        return TEXT("");
    }

    if (!BlockchainWallet)
    {
        UE_LOG(LogTemp, Error, TEXT("MingBlockchainMiningSystem: Blockchain wallet is not initialized"));
        return TEXT("");
    }

    FMiningSession NewSession;
    NewSession.SessionID = GenerateSessionID();
    NewSession.PlayerID = PlayerID;
    NewSession.Algorithm = Algorithm;
    NewSession.Difficulty = Difficulty;
    NewSession.StartTime = GetWorld()->GetTimeSeconds();
    NewSession.ElapsedTime = 0.0f;
    NewSession.HashRate = CalculateBaseHashRate(Algorithm) * MiningEfficiencyBonus;
    NewSession.AttemptsCount = 0;
    NewSession.IsActive = true;
    NewSession.Progress = 0.0f;

    ActiveSessions.Add(NewSession);
    CurrentSession = NewSession;

    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Started mining session %s for player %s"), 
           *NewSession.SessionID, *PlayerID);

    OnMiningSessionStarted.Broadcast(NewSession);

    return NewSession.SessionID;
}

bool UMingBlockchainMiningSystem::StopMiningSession(const FString& SessionID)
{
    for (int32 i = 0; i < ActiveSessions.Num(); ++i)
    {
        if (ActiveSessions[i].SessionID == SessionID)
        {
            FMiningSession& Session = ActiveSessions[i];
            Session.IsActive = false;
            Session.ElapsedTime = GetWorld()->GetTimeSeconds() - Session.StartTime;

            // Process final reward if progress is sufficient
            if (Session.Progress >= 100.0f)
            {
                ProcessMiningReward(Session);
                UpdateMiningStatistics(Session.PlayerID, Session, true);
            }
            else
            {
                UpdateMiningStatistics(Session.PlayerID, Session, false);
            }

            OnMiningSessionCompleted.Broadcast(Session);
            ActiveSessions.RemoveAt(i);

            UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Stopped mining session %s"), *SessionID);
            return true;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("MingBlockchainMiningSystem: Session %s not found"), *SessionID);
    return false;
}

FMiningSession UMingBlockchainMiningSystem::GetCurrentMiningSession() const
{
    return CurrentSession;
}

TArray<FMiningSession> UMingBlockchainMiningSystem::GetActiveMiningSessions() const
{
    return ActiveSessions;
}

FMiningReward UMingBlockchainMiningSystem::CalculateMiningReward(const FMiningSession& Session)
{
    FMiningReward Reward;
    
    // Base reward calculation
    float BaseValue = BaseRewardAmount * GetDifficultyMultiplier(Session.Difficulty) * GetAlgorithmMultiplier(Session.Algorithm);
    
    // Apply efficiency bonus
    BaseValue *= MiningEfficiencyBonus;
    
    // Apply time-based bonus (longer sessions get slightly more rewards)
    float TimeBonus = FMath::Clamp(Session.ElapsedTime / 300.0f, 1.0f, 2.0f); // Max 2x bonus for 5+ minutes
    BaseValue *= TimeBonus;

    // Determine reward type based on algorithm and difficulty
    if (Session.Difficulty >= EMiningDifficulty::Hard)
    {
        Reward.RewardType = EMiningRewardType::NFT;
        Reward.AssetName = TEXT("Ming Dynasty Artifact");
        Reward.Amount = 1;
    }
    else
    {
        Reward.RewardType = EMiningRewardType::Token;
        Reward.AssetName = TEXT("MingCoin");
        Reward.Amount = FMath::RoundToInt(BaseValue);
    }

    Reward.Value = BaseValue;
    Reward.RewardID = FGuid::NewGuid().ToString();
    Reward.TokenContract = GetTokenContractForReward(Reward.RewardType);
    Reward.Metadata = GenerateRewardMetadata(Session);

    return Reward;
}

FMiningStatistics UMingBlockchainMiningSystem::GetMiningStatistics(const FString& PlayerID) const
{
    if (const FMiningStatistics* Stats = PlayerStatistics.Find(PlayerID))
    {
        return *Stats;
    }
    
    return FMiningStatistics();
}

void UMingBlockchainMiningSystem::SetMiningDifficulty(EMiningDifficulty NewDifficulty)
{
    CurrentDifficulty = NewDifficulty;
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Mining difficulty set to %d"), (int32)NewDifficulty);
}

EMiningDifficulty UMingBlockchainMiningSystem::GetMiningDifficulty() const
{
    return CurrentDifficulty;
}

bool UMingBlockchainMiningSystem::IsMiningEnabled() const
{
    return bMiningEnabled;
}

void UMingBlockchainMiningSystem::SetMiningEnabled(bool bEnabled)
{
    bMiningEnabled = bEnabled;
    
    if (!bEnabled)
    {
        // Stop all active sessions
        for (const FMiningSession& Session : ActiveSessions)
        {
            StopMiningSession(Session.SessionID);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Mining %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

float UMingBlockchainMiningSystem::GetCurrentHashRate() const
{
    return CurrentHashRate;
}

float UMingBlockchainMiningSystem::EstimateMiningTime() const
{
    if (CurrentHashRate <= 0.0f)
    {
        return 0.0f;
    }

    float DifficultyMultiplier = GetDifficultyMultiplier(CurrentDifficulty);
    float BaseTime = 60.0f; // Base time in seconds for medium difficulty
    
    return BaseTime * DifficultyMultiplier / (CurrentHashRate * MiningEfficiencyBonus);
}

void UMingBlockchainMiningSystem::ProcessMiningTick(float DeltaTime)
{
    TimeSinceLastProgressUpdate += DeltaTime;

    for (FMiningSession& Session : ActiveSessions)
    {
        if (!Session.IsActive)
        {
            continue;
        }

        Session.ElapsedTime += DeltaTime;
        Session.AttemptsCount++;

        // Calculate progress
        float ProgressIncrement = CalculateMiningProgress(Session, DeltaTime);
        Session.Progress = FMath::Clamp(Session.Progress + ProgressIncrement, 0.0f, 100.0f);

        // Check if mining is complete
        if (Session.Progress >= 100.0f)
        {
            ProcessMiningReward(Session);
            UpdateMiningStatistics(Session.PlayerID, Session, true);
            OnMiningSessionCompleted.Broadcast(Session);
            Session.IsActive = false;
        }
    }

    // Update progress notifications
    if (TimeSinceLastProgressUpdate >= ProgressUpdateInterval)
    {
        for (const FMiningSession& Session : ActiveSessions)
        {
            if (Session.IsActive)
            {
                OnMiningProgressUpdated.Broadcast(Session.SessionID, Session.Progress);
            }
        }
        TimeSinceLastProgressUpdate = 0.0f;
    }

    // Clean up completed sessions
    CleanupCompletedSessions();
}

bool UMingBlockchainMiningSystem::ValidateMiningConfiguration() const
{
    return BlockchainWallet != nullptr && 
           TransactionManager != nullptr && 
           BaseRewardAmount > 0.0f &&
           MaxConcurrentSessions > 0;
}

TArray<EMiningAlgorithm> UMingBlockchainMiningSystem::GetSupportedAlgorithms() const
{
    TArray<EMiningAlgorithm> Algorithms;
    Algorithms.Add(EMiningAlgorithm::ProofOfWork);
    Algorithms.Add(EMiningAlgorithm::ProofOfStake);
    Algorithms.Add(EMiningAlgorithm::Hybrid);
    return Algorithms;
}

float UMingBlockchainMiningSystem::GetMiningEfficiencyBonus() const
{
    return MiningEfficiencyBonus;
}

void UMingBlockchainMiningSystem::SetMiningEfficiencyBonus(float Bonus)
{
    MiningEfficiencyBonus = FMath::Max(0.1f, Bonus);
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Mining efficiency bonus set to %f"), MiningEfficiencyBonus);
}

void UMingBlockchainMiningSystem::InitializeMiningConfiguration()
{
    // Load configuration from game settings or use defaults
    // This could be extended to load from config files
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Configuration initialized"));
}

FString UMingBlockchainMiningSystem::GenerateSessionID() const
{
    return FGuid::NewGuid().ToString();
}

float UMingBlockchainMiningSystem::CalculateMiningProgress(const FMiningSession& Session, float DeltaTime) const
{
    float BaseProgressRate = Session.HashRate / 1000.0f; // Convert to progress rate
    float DifficultyMultiplier = 1.0f / GetDifficultyMultiplier(Session.Difficulty);
    
    return BaseProgressRate * DeltaTime * DifficultyMultiplier;
}

bool UMingBlockchainMiningSystem::SimulateMiningHash(const FMiningSession& Session) const
{
    // Simulate hash calculation success based on difficulty and hash rate
    float SuccessChance = Session.HashRate / (Session.HashRate + GetDifficultyMultiplier(Session.Difficulty) * 100);
    return UKismetMathLibrary::RandomBoolWithWeight(SuccessChance);
}

void UMingBlockchainMiningSystem::UpdateMiningStatistics(const FString& PlayerID, const FMiningSession& Session, bool bSuccess)
{
    FMiningStatistics& Stats = PlayerStatistics.FindOrAdd(PlayerID);
    
    Stats.TotalMiningTime += Session.ElapsedTime;
    
    if (bSuccess)
    {
        Stats.TotalBlocksMined++;
        Stats.SuccessfulMines++;
        
        FMiningReward Reward = CalculateMiningReward(Session);
        Stats.TotalEarnings += Reward.Value;
    }
    else
    {
        Stats.FailedMines++;
    }
    
    // Calculate success rate
    int32 TotalMines = Stats.SuccessfulMines + Stats.FailedMines;
    Stats.SuccessRate = TotalMines > 0 ? (float)Stats.SuccessfulMines / TotalMines : 0.0f;
    
    // Calculate average hash rate
    Stats.AverageHashRate = Stats.TotalMiningTime > 0.0f ? 
        (Stats.AverageHashRate * (Stats.TotalMiningTime - Session.ElapsedTime) + Session.HashRate * Session.ElapsedTime) / Stats.TotalMiningTime :
        Session.HashRate;
}

void UMingBlockchainMiningSystem::ProcessMiningReward(const FMiningSession& Session)
{
    FMiningReward Reward = CalculateMiningReward(Session);
    
    // Send reward to wallet
    if (BlockchainWallet)
    {
        BlockchainWallet->ReceiveReward(Reward);
    }
    
    // Create transaction record
    if (TransactionManager)
    {
        TransactionManager->CreateRewardTransaction(Session.PlayerID, Reward);
    }
    
    OnMiningRewardEarned.Broadcast(Reward);
    
    UE_LOG(LogTemp, Log, TEXT("MingBlockchainMiningSystem: Processed reward %s for player %s"), 
           *Reward.AssetName, *Session.PlayerID);
}

float UMingBlockchainMiningSystem::GetDifficultyMultiplier(EMiningDifficulty Difficulty) const
{
    switch (Difficulty)
    {
        case EMiningDifficulty::Easy:   return 0.5f;
        case EMiningDifficulty::Medium: return 1.0f;
        case EMiningDifficulty::Hard:   return 2.0f;
        case EMiningDifficulty::Expert: return 5.0f;
        case EMiningDifficulty::Master: return 10.0f;
        default: return 1.0f;
    }
}

float UMingBlockchainMiningSystem::GetAlgorithmMultiplier(EMiningAlgorithm Algorithm) const
{
    switch (Algorithm)
    {
        case EMiningAlgorithm::ProofOfWork:  return 1.0f;
        case EMiningAlgorithm::ProofOfStake: return 1.2f;
        case EMiningAlgorithm::Hybrid:        return 1.5f;
        case EMiningAlgorithm::Custom:        return 2.0f;
        default: return 1.0f;
    }
}

void UMingBlockchainMiningSystem::CleanupCompletedSessions()
{
    ActiveSessions.RemoveAll([](const FMiningSession& Session) {
        return !Session.IsActive;
    });
}

bool UMingBlockchainMiningSystem::ValidateMiningSession(const FMiningSession& Session) const
{
    return !Session.SessionID.IsEmpty() && 
           !Session.PlayerID.IsEmpty() && 
           Session.StartTime > 0.0f &&
           Session.HashRate > 0.0f;
}

float UMingBlockchainMiningSystem::CalculateBaseHashRate(EMiningAlgorithm Algorithm) const
{
    // Base hash rates in MH/s
    switch (Algorithm)
    {
        case EMiningAlgorithm::ProofOfWork:  return 10.0f;
        case EMiningAlgorithm::ProofOfStake: return 15.0f;
        case EMiningAlgorithm::Hybrid:        return 20.0f;
        case EMiningAlgorithm::Custom:        return 25.0f;
        default: return 10.0f;
    }
}

FString UMingBlockchainMiningSystem::GetTokenContractForReward(EMiningRewardType RewardType) const
{
    switch (RewardType)
    {
        case EMiningRewardType::Token:    return TEXT("0x1234567890abcdef1234567890abcdef12345678");
        case EMiningRewardType::NFT:      return TEXT("0xabcdef1234567890abcdef1234567890abcdef12");
        default: return TEXT("");
    }
}

FString UMingBlockchainMiningSystem::GenerateRewardMetadata(const FMiningSession& Session) const
{
    FString Metadata = TEXT("{");
    Metadata += TEXT("\"session_id\":\"") + Session.SessionID + TEXT("\",");
    Metadata += TEXT("\"player_id\":\"") + Session.PlayerID + TEXT("\",");
    Metadata += TEXT("\"algorithm\":") + FString::FromInt((int32)Session.Algorithm) + TEXT(",");
    Metadata += TEXT("\"difficulty\":") + FString::FromInt((int32)Session.Difficulty) + TEXT(",");
    Metadata += TEXT("\"duration\":") + FString::SanitizeFloat(Session.ElapsedTime) + TEXT(",");
    Metadata += TEXT("\"hash_rate\":") + FString::SanitizeFloat(Session.HashRate) + TEXT(",");
    Metadata += TEXT("\"attempts\":") + FString::FromInt(Session.AttemptsCount);
    Metadata += TEXT("}");
    
    return Metadata;
}
