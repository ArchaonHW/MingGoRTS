#include "MingCooperativeManager.h"
#include "MingNetworkManager.h"
#include "Engine/GameInstance.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

UMingCooperativeManager::UMingCooperativeManager()
    : CurrentGameState(EMingCoopGameState::Waiting)
    , ResourceSharingPolicy(EMingResourceSharingPolicy::Shared)
    , AIAssistanceLevel(EMingAIAssistanceLevel::Normal)
    , bSessionActive(false)
    , LastUpdateTime(0.0f)
    , UpdateInterval(0.1f)
{
}

void UMingCooperativeManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogTemp, Log, TEXT("MingCooperativeManager initialized"));
}

void UMingCooperativeManager::Deinitialize()
{
    if (bSessionActive)
    {
        EndCooperativeSession(TEXT("System shutdown"));
    }
    
    Super::Deinitialize();
}

void UMingCooperativeManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (bSessionActive)
    {
        UpdateCooperativeSession(DeltaTime);
    }
}

TStatId UMingCooperativeManager::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UMingCooperativeManager, STATGROUP_GameThread);
}

bool UMingCooperativeManager::CreateCooperativeSession(const FMingCooperativeSessionInfo& SessionInfo)
{
    if (bSessionActive)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cooperative session already active"));
        return false;
    }
    
    if (!ValidateSessionInfo(SessionInfo))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid session info"));
        return false;
    }
    
    CurrentSession = SessionInfo;
    CurrentSession.SessionID = GenerateSessionID();
    bSessionActive = true;
    
    CurrentGameState = EMingCoopGameState::Waiting;
    InitializeDefaultObjectives();
    
    OnCooperativeSessionStarted.Broadcast(CurrentSession.SessionID, CurrentSession);
    
    UE_LOG(LogTemp, Log, TEXT("Cooperative session created: %s"), *CurrentSession.SessionID);
    return true;
}

bool UMingCooperativeManager::JoinCooperativeSession(const FString& SessionID, const FMingPlayerCooperativeInfo& PlayerInfo)
{
    if (!ValidatePlayerInfo(PlayerInfo))
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid player info"));
        return false;
    }
    
    CurrentSession.SessionID = SessionID;
    bSessionActive = true;
    
    CooperativePlayers.Add(PlayerInfo.PlayerID, PlayerInfo);
    
    OnPlayerJoinedCooperative.Broadcast(SessionID, PlayerInfo.PlayerID, PlayerInfo);
    
    UE_LOG(LogTemp, Log, TEXT("Player %d joined cooperative session %s"), PlayerInfo.PlayerID, *SessionID);
    return true;
}

void UMingCooperativeManager::LeaveCooperativeSession()
{
    if (!bSessionActive)
    {
        return;
    }
    
    // Get local player ID and broadcast leaving event
    for (const auto& PlayerPair : CooperativePlayers)
    {
        if (PlayerPair.Value.bIsLocalPlayer)
        {
            OnPlayerLeftCooperative.Broadcast(CurrentSession.SessionID, PlayerPair.Key);
            CooperativePlayers.Remove(PlayerPair.Key);
            break;
        }
    }
    
    if (CooperativePlayers.Num() == 0)
    {
        EndCooperativeSession(TEXT("All players left"));
    }
}

void UMingCooperativeManager::EndCooperativeSession(const FString& Reason)
{
    if (!bSessionActive)
    {
        return;
    }
    
    FString SessionID = CurrentSession.SessionID;
    OnCooperativeSessionEnded.Broadcast(SessionID, Reason);
    
    CleanupSession();
    
    UE_LOG(LogTemp, Log, TEXT("Cooperative session ended: %s, Reason: %s"), *SessionID, *Reason);
}

bool UMingCooperativeManager::IsInCooperativeSession() const
{
    return bSessionActive;
}

FString UMingCooperativeManager::GetCurrentSessionID() const
{
    return CurrentSession.SessionID;
}

FMingCooperativeSessionInfo UMingCooperativeManager::GetCurrentSessionInfo() const
{
    return CurrentSession;
}

void UMingCooperativeManager::UpdatePlayerRole(int32 PlayerID, EMingCoopRole NewRole)
{
    if (FMingPlayerCooperativeInfo* PlayerInfo = CooperativePlayers.Find(PlayerID))
    {
        PlayerInfo->Role = NewRole;
        BroadcastSessionState();
    }
}

void UMingCooperativeManager::SetPlayerReady(int32 PlayerID, bool bReady)
{
    if (FMingPlayerCooperativeInfo* PlayerInfo = CooperativePlayers.Find(PlayerID))
    {
        PlayerInfo->bReady = bReady;
        
        if (AreAllPlayersReady() && CurrentGameState == EMingCoopGameState::Waiting)
        {
            SetCoopGameState(EMingCoopGameState::Starting);
        }
    }
}

bool UMingCooperativeManager::AreAllPlayersReady() const
{
    for (const auto& PlayerPair : CooperativePlayers)
    {
        if (!PlayerPair.Value.bReady)
        {
            return false;
        }
    }
    return CooperativePlayers.Num() > 0;
}

TArray<FMingPlayerCooperativeInfo> UMingCooperativeManager::GetCooperativePlayers() const
{
    TArray<FMingPlayerCooperativeInfo> Players;
    CooperativePlayers.GenerateValueArray(Players);
    return Players;
}

FMingPlayerCooperativeInfo UMingCooperativeManager::GetPlayerCooperativeInfo(int32 PlayerID) const
{
    if (const FMingPlayerCooperativeInfo* PlayerInfo = CooperativePlayers.Find(PlayerID))
    {
        return *PlayerInfo;
    }
    return FMingPlayerCooperativeInfo();
}

void UMingCooperativeManager::AddObjective(const FMingCooperativeObjective& Objective)
{
    ActiveObjectives.Add(Objective);
    BroadcastSessionState();
}

void UMingCooperativeManager::CompleteObjective(const FString& ObjectiveID)
{
    for (int32 i = 0; i < ActiveObjectives.Num(); ++i)
    {
        if (ActiveObjectives[i].ObjectiveID == ObjectiveID)
        {
            FMingCooperativeObjective CompletedObjective = ActiveObjectives[i];
            CompletedObjective.Status = EMingObjectiveStatus::Completed;
            
            CompletedObjectives.Add(CompletedObjective);
            ActiveObjectives.RemoveAt(i);
            
            // Award scores to assigned players
            for (int32 PlayerID : CompletedObjective.AssignedPlayers)
            {
                UpdatePlayerScore(PlayerID, CompletedObjective.ScoreReward, 
                    FString::Printf(TEXT("Objective completed: %s"), *CompletedObjective.Title));
            }
            
            OnCooperativeObjectiveCompleted.Broadcast(CurrentSession.SessionID, CompletedObjective);
            CheckObjectives();
            break;
        }
    }
}

void UMingCooperativeManager::FailObjective(const FString& ObjectiveID, const FString& Reason)
{
    for (int32 i = 0; i < ActiveObjectives.Num(); ++i)
    {
        if (ActiveObjectives[i].ObjectiveID == ObjectiveID)
        {
            FMingCooperativeObjective FailedObjective = ActiveObjectives[i];
            FailedObjective.Status = EMingObjectiveStatus::Failed;
            
            CompletedObjectives.Add(FailedObjective);
            ActiveObjectives.RemoveAt(i);
            
            OnCooperativeObjectiveFailed.Broadcast(CurrentSession.SessionID, FailedObjective);
            CheckObjectives();
            break;
        }
    }
}

TArray<FMingCooperativeObjective> UMingCooperativeManager::GetActiveObjectives() const
{
    return ActiveObjectives;
}

TArray<FMingCooperativeObjective> UMingCooperativeManager::GetCompletedObjectives() const
{
    return CompletedObjectives;
}

bool UMingCooperativeManager::ShareResource(int32 TargetPlayerID, const FMingResourceShareRequest& ResourceRequest)
{
    if (ResourceSharingPolicy == EMingResourceSharingPolicy::Individual)
    {
        return false;
    }
    
    // Process the resource share request
    ProcessResourceShare(ResourceRequest);
    
    return true;
}

void UMingCooperativeManager::ProcessResourceShare(const FMingResourceShareRequest& ResourceRequest)
{
    // Add to pending requests for target player
    if (FMingPlayerCooperativeInfo* PlayerInfo = CooperativePlayers.Find(ResourceRequest.ToPlayerID))
    {
        PlayerInfo->PendingResourceRequests.Add(ResourceRequest);
        SessionProgress.ResourcesShared++;
    }
}

void UMingCooperativeManager::SetResourceSharingPolicy(EMingResourceSharingPolicy Policy)
{
    ResourceSharingPolicy = Policy;
    BroadcastSessionState();
}

EMingResourceSharingPolicy UMingCooperativeManager::GetResourceSharingPolicy() const
{
    return ResourceSharingPolicy;
}

void UMingCooperativeManager::SendCooperativeMessage(const FString& Message, EMingCoopMessageType MessageType)
{
    FMingTacticalUpdate Update;
    Update.Message = Message;
    Update.Timestamp = GetWorld()->GetTimeSeconds();
    
    // Broadcast to all players
    for (const auto& PlayerPair : CooperativePlayers)
    {
        Update.TargetPlayers.Add(PlayerPair.Key);
    }
    
    // Send via network manager
    if (UMingNetworkManager* NetworkManager = GetGameInstance()->GetSubsystem<UMingNetworkManager>())
    {
        TArray<uint8> MessageData;
        // Serialize message data
        NetworkManager->BroadcastMessage(0, MessageData);
    }
}

void UMingCooperativeManager::SendPlayerPing(int32 TargetPlayerID, const FVector& Location)
{
    FMingTacticalUpdate Update;
    Update.PlayerID = TargetPlayerID;
    Update.UpdateType = EMingTacticalUpdateType::TeamPosition;
    Update.Location = Location;
    Update.Timestamp = GetWorld()->GetTimeSeconds();
    Update.TargetPlayers.Add(TargetPlayerID);
    
    BroadcastTacticalUpdate(Update);
}

void UMingCooperativeManager::BroadcastTacticalUpdate(const FMingTacticalUpdate& Update)
{
    // Send tactical update to relevant players
    if (UMingNetworkManager* NetworkManager = GetGameInstance()->GetSubsystem<UMingNetworkManager>())
    {
        TArray<uint8> UpdateData;
        // Serialize update data
        NetworkManager->BroadcastMessage(1, UpdateData);
    }
}

void UMingCooperativeManager::SetCoopGameState(EMingCoopGameState NewState)
{
    if (CurrentGameState != NewState)
    {
        EMingCoopGameState OldState = CurrentGameState;
        CurrentGameState = NewState;
        
        OnCoopGameStateChanged.Broadcast(CurrentSession.SessionID, NewState);
        
        // Handle state transitions
        switch (NewState)
        {
        case EMingCoopGameState::Starting:
            // Start game after a short delay
            GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
            {
                SetCoopGameState(EMingCoopGameState::Playing);
            });
            break;
            
        case EMingCoopGameState::Playing:
            SessionProgress.TimeElapsed = 0.0f;
            break;
            
        case EMingCoopGameState::Completed:
            CalculateSessionScores();
            break;
        }
    }
}

EMingCoopGameState UMingCooperativeManager::GetCoopGameState() const
{
    return CurrentGameState;
}

void UMingCooperativeManager::PauseCooperativeSession()
{
    if (CurrentGameState == EMingCoopGameState::Playing)
    {
        SetCoopGameState(EMingCoopGameState::Paused);
    }
}

void UMingCooperativeManager::ResumeCooperativeSession()
{
    if (CurrentGameState == EMingCoopGameState::Paused)
    {
        SetCoopGameState(EMingCoopGameState::Playing);
    }
}

void UMingCooperativeManager::UpdatePlayerScore(int32 PlayerID, int32 ScoreChange, const FString& Reason)
{
    if (FMingPlayerCooperativeInfo* PlayerInfo = CooperativePlayers.Find(PlayerID))
    {
        PlayerInfo->Score += ScoreChange;
        SessionScores.PlayerScores.FindOrAdd(PlayerID) = PlayerInfo->Score;
        
        OnCoopScoreUpdated.Broadcast(CurrentSession.SessionID, PlayerID, PlayerInfo->Score, Reason);
    }
}

void UMingCooperativeManager::AwardSharedAchievement(const FString& AchievementID)
{
    SessionProgress.UnlockedAchievements.Add(AchievementID);
    
    // Award bonus score to all players
    for (const auto& PlayerPair : CooperativePlayers)
    {
        UpdatePlayerScore(PlayerPair.Key, 50, FString::Printf(TEXT("Shared achievement: %s"), *AchievementID));
    }
}

FMingCoopProgress UMingCooperativeManager::GetCooperativeProgress() const
{
    return SessionProgress;
}

FMingCoopScore UMingCooperativeManager::GetCooperativeScores() const
{
    return SessionScores;
}

void UMingCooperativeManager::RequestAIAssistance(EMingAIAssistanceType AssistanceType, const FVector& Location)
{
    if (!CurrentSession.bAIAssistanceEnabled)
    {
        return;
    }
    
    // Process AI assistance request based on type and level
    switch (AssistanceType)
    {
    case EMingAIAssistanceType::Guidance:
        // Provide tactical hints
        break;
    case EMingAIAssistanceType::Reinforcement:
        // Send AI units to help
        break;
    case EMingAIAssistanceType::Support:
        // Provide resources or healing
        break;
    case EMingAIAssistanceType::Defense:
        // Deploy defensive AI units
        break;
    case EMingAIAssistanceType::Resources:
        // Provide resource bonus
        break;
    }
}

void UMingCooperativeManager::SetAIAssistanceLevel(EMingAIAssistanceLevel Level)
{
    AIAssistanceLevel = Level;
}

EMingAIAssistanceLevel UMingCooperativeManager::GetAIAssistanceLevel() const
{
    return AIAssistanceLevel;
}

void UMingCooperativeManager::UpdateCooperativeSession(float DeltaTime)
{
    LastUpdateTime += DeltaTime;
    
    if (LastUpdateTime >= UpdateInterval)
    {
        // Update session progress
        if (CurrentGameState == EMingCoopGameState::Playing)
        {
            SessionProgress.TimeElapsed += LastUpdateTime;
        }
        
        // Update objectives
        for (FMingCooperativeObjective& Objective : ActiveObjectives)
        {
            if (Objective.TimeLimit > 0.0f)
            {
                Objective.TimeRemaining -= LastUpdateTime;
                if (Objective.TimeRemaining <= 0.0f)
                {
                    FailObjective(Objective.ObjectiveID, TEXT("Time expired"));
                }
            }
        }
        
        // Process resource sharing
        ProcessResourceSharing();
        
        // Update AI assistance
        UpdateAIAssistance();
        
        // Validate session integrity
        ValidateCooperativeIntegrity();
        
        LastUpdateTime = 0.0f;
    }
}

void UMingCooperativeManager::ProcessResourceSharing()
{
    // Process pending resource requests
    for (auto& PlayerPair : CooperativePlayers)
    {
        TArray<FMingResourceShareRequest> ExpiredRequests;
        
        for (FMingResourceShareRequest& Request : PlayerPair.Value.PendingResourceRequests)
        {
            if (GetWorld()->GetTimeSeconds() >= Request.ExpiryTime)
            {
                ExpiredRequests.Add(Request);
            }
        }
        
        // Remove expired requests
        for (const FMingResourceShareRequest& ExpiredRequest : ExpiredRequests)
        {
            PlayerPair.Value.PendingResourceRequests.Remove(ExpiredRequest);
        }
    }
}

void UMingCooperativeManager::UpdateAIAssistance()
{
    // Update AI assistance based on current level and game state
    if (CurrentSession.bAIAssistanceEnabled && CurrentGameState == EMingCoopGameState::Playing)
    {
        // AI assistance logic here
    }
}

void UMingCooperativeManager::CheckObjectives()
{
    // Check if all required objectives are completed
    bool bAllRequiredCompleted = true;
    int32 TotalObjectives = ActiveObjectives.Num() + CompletedObjectives.Num();
    int32 CompletedRequired = 0;
    
    for (const FMingCooperativeObjective& Objective : CompletedObjectives)
    {
        if (!Objective.bOptional && Objective.Status == EMingObjectiveStatus::Completed)
        {
            CompletedRequired++;
        }
    }
    
    for (const FMingCooperativeObjective& Objective : ActiveObjectives)
    {
        if (!Objective.bOptional)
        {
            bAllRequiredCompleted = false;
            break;
        }
    }
    
    // Update progress
    SessionProgress.TotalObjectives = TotalObjectives;
    SessionProgress.ObjectivesCompleted = CompletedObjectives.Num();
    SessionProgress.OverallProgress = (float)SessionProgress.ObjectivesCompleted / (float)TotalObjectives;
    
    // Check win/lose conditions
    if (bAllRequiredCompleted && ActiveObjectives.Num() == 0)
    {
        SetCoopGameState(EMingCoopGameState::Completed);
    }
    else if (SessionProgress.TimeElapsed >= CurrentSession.SessionTimeoutMinutes * 60.0f)
    {
        SetCoopGameState(EMingCoopGameState::Failed);
    }
}

void UMingCooperativeManager::SyncCooperativeState()
{
    BroadcastSessionState();
}

void UMingCooperativeManager::BroadcastSessionState()
{
    // Broadcast current session state to all players
    if (UMingNetworkManager* NetworkManager = GetGameInstance()->GetSubsystem<UMingNetworkManager>())
    {
        TArray<uint8> StateData;
        // Serialize session state
        NetworkManager->BroadcastMessage(2, StateData);
    }
}

void UMingCooperativeManager::HandlePlayerDisconnection(int32 PlayerID)
{
    CooperativePlayers.Remove(PlayerID);
    
    if (CooperativePlayers.Num() == 0)
    {
        EndCooperativeSession(TEXT("All players disconnected"));
    }
}

void UMingCooperativeManager::ValidateCooperativeIntegrity()
{
    // Check for disconnected players and sync issues
    for (auto It = CooperativePlayers.CreateIterator(); It; ++It)
    {
        // Remove inactive players
        if (GetWorld()->GetTimeSeconds() - It->Value.LastActiveTime > 300.0f) // 5 minutes timeout
        {
            HandlePlayerDisconnection(It->Key);
        }
    }
}

void UMingCooperativeManager::CalculateSessionScores()
{
    // Calculate final scores including bonuses
    SessionScores.TeamScore = 0;
    
    for (const auto& PlayerPair : SessionScores.PlayerScores)
    {
        SessionScores.TeamScore += PlayerPair.Value;
    }
    
    // Calculate teamwork bonus based on resources shared and cooperation
    SessionScores.TeamworkBonus = SessionProgress.ResourcesShared * 10.0f;
    
    // Calculate speed bonus based on completion time
    float TimeRatio = SessionProgress.TimeElapsed / (CurrentSession.SessionTimeoutMinutes * 60.0f);
    SessionScores.SpeedBonus = (1.0f - TimeRatio) * 500.0f;
    
    // Calculate efficiency bonus
    SessionScores.EfficiencyBonus = SessionProgress.TeamworkScore * 20.0f;
    
    // Calculate total score
    SessionScores.TotalScore = SessionScores.TeamScore + 
        (int32)SessionScores.TeamworkBonus + 
        (int32)SessionScores.SpeedBonus + 
        (int32)SessionScores.EfficiencyBonus;
}

void UMingCooperativeManager::GenerateSessionReport()
{
    // Generate detailed session report for analytics and player feedback
    CalculateSessionScores();
}

bool UMingCooperativeManager::ValidateSessionInfo(const FMingCooperativeSessionInfo& SessionInfo) const
{
    return !SessionInfo.SessionName.IsEmpty() && 
           SessionInfo.MaxPlayers > 0 && 
           SessionInfo.MaxPlayers <= 8 &&
           SessionInfo.SessionTimeoutMinutes > 0.0f;
}

bool UMingCooperativeManager::ValidatePlayerInfo(const FMingPlayerCooperativeInfo& PlayerInfo) const
{
    return !PlayerInfo.PlayerName.IsEmpty() && 
           PlayerInfo.PlayerID >= 0;
}

FString UMingCooperativeManager::GenerateSessionID() const
{
    return FString::Printf(TEXT("COOP_%lld"), FDateTime::Now().GetTicks());
}

void UMingCooperativeManager::InitializeDefaultObjectives()
{
    // Initialize default objectives based on session mode
    if (CurrentSession.CoopMode == EMingCoopMode::Campaign)
    {
        // Add campaign-specific objectives
    }
    else if (CurrentSession.CoopMode == EMingCoopMode::Survival)
    {
        // Add survival objectives
    }
}

void UMingCooperativeManager::CleanupSession()
{
    bSessionActive = false;
    CurrentSession = FMingCooperativeSessionInfo();
    CooperativePlayers.Empty();
    ActiveObjectives.Empty();
    CompletedObjectives.Empty();
    CurrentGameState = EMingCoopGameState::Waiting;
    SessionProgress = FMingCoopProgress();
    SessionScores = FMingCoopScore();
}

void UMingCooperativeManager::OnNetworkMessageReceived(const FMingNetworkMessage& Message)
{
    // Handle incoming network messages
}

void UMingCooperativeManager::OnPlayerNetworkStateChanged(int32 PlayerID, EMingConnectionState NewState)
{
    if (NewState == EMingConnectionState::Disconnected)
    {
        HandlePlayerDisconnection(PlayerID);
    }
}
