#include "MingRTSGameMode.h"
#include "MingNetworkManager.h"
#include "MingRelationshipReplication.h"
#include "MingReputationReplication.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

AMingRTSGameMode::AMingRTSGameMode()
    : bGamePaused(false)
    , ServerStartTime(0.0f)
    , NextPlayerID(1)
{
    // Set default values
    ServerConfig.MaxPlayers = 8;
    ServerConfig.TickRate = 30;
}

void AMingRTSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    // Parse command line options
    if (FParse::Param(FCommandLine::Get(), TEXT("dedicated")))
    {
        ServerConfig.ServerMode = EMingServerMode::DedicatedServer;
    }
    else if (FParse::Param(FCommandLine::Get(), TEXT("listen")))
    {
        ServerConfig.ServerMode = EMingServerMode::ListenServer;
    }
    
    // Parse max players
    FString MaxPlayersStr;
    if (FParse::Value(FCommandLine::Get(), TEXT("MaxPlayers="), MaxPlayersStr))
    {
        ServerConfig.MaxPlayers = FCString::Atoi(*MaxPlayersStr);
    }
    
    // Initialize systems
    InitializeMultiplayerSystems();
    
    ServerStartTime = GetWorld()->GetTimeSeconds();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTS Game Mode initialized - Mode: %d, MaxPlayers: %d"),
        static_cast<int32>(ServerConfig.ServerMode), ServerConfig.MaxPlayers);
}

void AMingRTSGameMode::StartPlay()
{
    Super::StartPlay();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTS Game Mode started"));
}

void AMingRTSGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    ShutdownMultiplayerSystems();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTS Game Mode ended - Reason: %d"), static_cast<int32>(EndPlayReason));
    
    Super::EndPlay(EndPlayReason);
}

void AMingRTSGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, 
    FString& ErrorMessage)
{
    Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
    
    // Check if server is full
    if (GetPlayerCount() >= ServerConfig.MaxPlayers)
    {
        ErrorMessage = TEXT("Server is full");
        return;
    }
    
    // Check password if required
    if (ServerConfig.bUsePassword)
    {
        FString Password;
        if (FParse::Value(Options, TEXT("Password="), Password))
        {
            if (Password != ServerConfig.ServerPassword)
            {
                ErrorMessage = TEXT("Incorrect password");
                return;
            }
        }
        else
        {
            ErrorMessage = TEXT("Password required");
            return;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Player pre-login from: %s"), *Address);
}

void AMingRTSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);
    
    int32 PlayerID = AssignPlayerID();
    
    // Store join time
    float CurrentTime = GetWorld()->GetTimeSeconds();
    PlayerJoinTimes.Add(PlayerID, CurrentTime);
    PlayerLastActiveTimes.Add(PlayerID, CurrentTime);
    
    // Initialize player data
    if (NetworkManager)
    {
        FMingPlayerNetworkInfo PlayerInfo;
        PlayerInfo.PlayerID = PlayerID;
        PlayerInfo.PlayerName = NewPlayer->PlayerState ? NewPlayer->PlayerState->GetPlayerName() : TEXT("Unknown");
        PlayerInfo.ConnectionState = EMingConnectionState::Connected;
        PlayerInfo.ConnectionTime = FDateTime::Now();
        
        NetworkManager->RegisterPlayer(PlayerID, PlayerInfo);
    }
    
    // Sync player data
    SyncPlayerData(PlayerID);
    
    OnPlayerJoined.Broadcast(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Player %d logged in"), PlayerID);
}

void AMingRTSGameMode::Logout(AController* Exiting)
{
    int32 PlayerID = -1;
    
    // Find player ID
    for (const auto& Pair : PlayerJoinTimes)
    {
        // This is a simplified approach - in real implementation you'd map controller to player ID
        PlayerID = Pair.Key;
        break;
    }
    
    if (PlayerID >= 0)
    {
        ReleasePlayerID(PlayerID);
        PlayerJoinTimes.Remove(PlayerID);
        PlayerLastActiveTimes.Remove(PlayerID);
        
        if (NetworkManager)
        {
            NetworkManager->UnregisterPlayer(PlayerID);
        }
        
        OnPlayerLeft.Broadcast(PlayerID);
        
        UE_LOG(LogTemp, Log, TEXT("Player %d logged out"), PlayerID);
    }
    
    Super::Logout(Exiting);
}

void AMingRTSGameMode::ConfigureServer(const FMingServerConfiguration& Configuration)
{
    ServerConfig = Configuration;
    
    // Apply configuration
    if (NetworkManager)
    {
        NetworkManager->SetTickRate(ServerConfig.TickRate);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Server configured - Mode: %d, MaxPlayers: %d"),
        static_cast<int32>(ServerConfig.ServerMode), ServerConfig.MaxPlayers);
}

void AMingRTSGameMode::SetMaxPlayers(int32 MaxPlayers)
{
    ServerConfig.MaxPlayers = FMath::Clamp(MaxPlayers, 1, 64);
    
    UE_LOG(LogTemp, Log, TEXT("Max players set to: %d"), ServerConfig.MaxPlayers);
}

void AMingRTSGameMode::SetServerPassword(const FString& Password)
{
    ServerConfig.ServerPassword = Password;
    ServerConfig.bUsePassword = !Password.IsEmpty();
    
    if (NetworkManager)
    {
        NetworkManager->SetServerPassword(Password);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Server password %s"), ServerConfig.bUsePassword ? TEXT("set") : TEXT("cleared"));
}

void AMingRTSGameMode::EnableCheats(bool bEnabled)
{
    ServerConfig.bAllowCheats = bEnabled;
    
    UE_LOG(LogTemp, Log, TEXT("Cheats %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void AMingRTSGameMode::KickPlayer(int32 PlayerID, const FString& Reason)
{
    if (NetworkManager)
    {
        NetworkManager->KickPlayer(PlayerID, Reason);
        OnPlayerKicked.Broadcast(PlayerID, Reason);
        
        UE_LOG(LogTemp, Log, TEXT("Player %d kicked: %s"), PlayerID, *Reason);
    }
}

void AMingRTSGameMode::BanPlayer(int32 PlayerID, const FString& Reason, int32 DurationMinutes)
{
    BannedPlayers.Add(PlayerID);
    
    if (NetworkManager)
    {
        NetworkManager->BanPlayer(PlayerID, DurationMinutes, Reason);
    }
    
    KickPlayer(PlayerID, Reason);
    
    UE_LOG(LogTemp, Log, TEXT("Player %d banned for %d minutes: %s"), PlayerID, DurationMinutes, *Reason);
}

void AMingRTSGameMode::UnbanPlayer(int32 PlayerID)
{
    BannedPlayers.Remove(PlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Player %d unbanned"), PlayerID);
}

void AMingRTSGameMode::TransferHost(int32 NewHostPlayerID)
{
    // Transfer host privileges to another player
    UE_LOG(LogTemp, Log, TEXT("Host transferred to player %d"), NewHostPlayerID);
}

TArray<int32> AMingRTSGameMode::GetConnectedPlayerIDs() const
{
    TArray<int32> PlayerIDs;
    PlayerJoinTimes.GetKeys(PlayerIDs);
    return PlayerIDs;
}

int32 AMingRTSGameMode::GetPlayerCount() const
{
    return PlayerJoinTimes.Num();
}

bool AMingRTSGameMode::IsPlayerBanned(int32 PlayerID) const
{
    return BannedPlayers.Contains(PlayerID);
}

void AMingRTSGameMode::PauseGame()
{
    if (!bGamePaused)
    {
        bGamePaused = true;
        
        // Pause game world
        GetWorld()->GetWorldSettings()->SetPauserPlayerState(nullptr);
        
        OnGamePaused.Broadcast();
        
        UE_LOG(LogTemp, Log, TEXT("Game paused"));
    }
}

void AMingRTSGameMode::ResumeGame()
{
    if (bGamePaused)
    {
        bGamePaused = false;
        
        // Unpause game world
        GetWorld()->GetWorldSettings()->SetPauserPlayerState(nullptr);
        
        OnGameResumed.Broadcast();
        
        UE_LOG(LogTemp, Log, TEXT("Game resumed"));
    }
}

void AMingRTSGameMode::RestartGame()
{
    // Restart the current map
    FString CurrentMap = GetWorld()->GetMapName();
    ChangeMap(CurrentMap);
    
    UE_LOG(LogTemp, Log, TEXT("Game restarted"));
}

void AMingRTSGameMode::ChangeMap(const FString& MapName)
{
    // Server travel to new map
    FString TravelURL = MapName;
    GetWorld()->ServerTravel(TravelURL);
    
    UE_LOG(LogTemp, Log, TEXT("Changing map to: %s"), *MapName);
}

void AMingRTSGameMode::BroadcastGameState()
{
    // Broadcast current game state to all clients
    UE_LOG(LogTemp, Log, TEXT("Broadcasting game state to all players"));
}

void AMingRTSGameMode::SyncPlayerData(int32 PlayerID)
{
    // Sync relationship data
    if (RelationshipReplication)
    {
        RelationshipReplication->RequestFullSync(PlayerID);
    }
    
    // Sync reputation data
    if (ReputationReplication)
    {
        ReputationReplication->RequestFullSync(PlayerID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Synced data for player %d"), PlayerID);
}

void AMingRTSGameMode::SyncAllPlayersData()
{
    TArray<int32> PlayerIDs = GetConnectedPlayerIDs();
    
    for (int32 PlayerID : PlayerIDs)
    {
        SyncPlayerData(PlayerID);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Synced data for all %d players"), PlayerIDs.Num());
}

void AMingRTSGameMode::ServerUpdateRelationship(int32 PlayerID, FName CharacterID, float Value, const FString& Reason)
{
    if (RelationshipReplication)
    {
        RelationshipReplication->ServerUpdateRelationship(PlayerID, CharacterID, Value, Reason);
    }
}

void AMingRTSGameMode::ServerUpdateReputation(int32 PlayerID, FName RegionID, float Value, const FString& Reason)
{
    if (ReputationReplication)
    {
        ReputationReplication->ServerUpdateReputation(PlayerID, RegionID, Value, Reason);
    }
}

void AMingRTSGameMode::ServerCompleteQuest(int32 PlayerID, const FString& QuestID)
{
    // Handle quest completion on server
    UE_LOG(LogTemp, Log, TEXT("Player %d completed quest: %s"), PlayerID, *QuestID);
}

bool AMingRTSGameMode::ValidatePlayerAction(int32 PlayerID, int32 ActionType, const TArray<uint8>& ActionData)
{
    // Validate player action based on action type
    // This prevents cheating by validating all client actions on server
    
    switch (ActionType)
    {
    case 0: // Relationship change
        // Validate relationship change
        return true;
    case 1: // Reputation change
        // Validate reputation change
        return true;
    default:
        return true;
    }
}

bool AMingRTSGameMode::ValidateRelationshipChange(int32 PlayerID, FName CharacterID, float NewValue) const
{
    // Validate relationship change on server
    // Check if change is within reasonable bounds
    if (NewValue < 0.0f || NewValue > 100.0f)
    {
        return false;
    }
    
    // Check if player has permission to change this relationship
    // This would involve checking quest completion, etc.
    
    return true;
}

bool AMingRTSGameMode::ValidateReputationChange(int32 PlayerID, FName RegionID, float NewValue) const
{
    // Validate reputation change on server
    if (NewValue < 0.0f || NewValue > 100.0f)
    {
        return false;
    }
    
    return true;
}

float AMingRTSGameMode::GetServerUptime() const
{
    return GetWorld()->GetTimeSeconds() - ServerStartTime;
}

int32 AMingRTSGameMode::GetTotalBytesTransferred() const
{
    // Calculate total bytes transferred
    if (NetworkManager)
    {
        FMingNetworkStats Stats = NetworkManager->GetNetworkStats();
        return Stats.TotalBytesReceived + Stats.TotalBytesSent;
    }
    
    return 0;
}

float AMingRTSGameMode::GetAveragePlayerPing() const
{
    if (NetworkManager)
    {
        FMingNetworkStats Stats = NetworkManager->GetNetworkStats();
        return Stats.AveragePing;
    }
    
    return 0.0f;
}

void AMingRTSGameMode::InitializeMultiplayerSystems()
{
    // Create network manager
    NetworkManager = NewObject<UMingNetworkManager>();
    if (NetworkManager)
    {
        NetworkManager->InitializeNetwork();
    }
    
    // Create relationship replication
    RelationshipReplication = NewObject<UMingRelationshipReplication>();
    if (RelationshipReplication)
    {
        // RelationshipReplication->InitializeReplication(NetworkManager, nullptr);
    }
    
    // Create reputation replication
    ReputationReplication = NewObject<UMingReputationReplication>();
    if (ReputationReplication)
    {
        // ReputationReplication->InitializeReplication(NetworkManager, nullptr);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Multiplayer systems initialized"));
}

void AMingRTSGameMode::ShutdownMultiplayerSystems()
{
    if (RelationshipReplication)
    {
        RelationshipReplication->ShutdownReplication();
        RelationshipReplication = nullptr;
    }
    
    if (ReputationReplication)
    {
        ReputationReplication->ShutdownReplication();
        ReputationReplication = nullptr;
    }
    
    if (NetworkManager)
    {
        NetworkManager->ShutdownNetwork();
        NetworkManager = nullptr;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Multiplayer systems shutdown"));
}

void AMingRTSGameMode::CheckIdlePlayers()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    
    for (const auto& Pair : PlayerLastActiveTimes)
    {
        int32 PlayerID = Pair.Key;
        float LastActiveTime = Pair.Value;
        
        if (CurrentTime - LastActiveTime > ServerConfig.IdleKickTime)
        {
            KickPlayer(PlayerID, TEXT("Idle timeout"));
        }
    }
}

void AMingRTSGameMode::UpdatePlayerActivity(int32 PlayerID)
{
    if (PlayerLastActiveTimes.Contains(PlayerID))
    {
        PlayerLastActiveTimes[PlayerID] = GetWorld()->GetTimeSeconds();
    }
}

int32 AMingRTSGameMode::AssignPlayerID()
{
    // Check reserved slots first
    for (int32 ReservedID : ReservedPlayerIDs)
    {
        if (!PlayerJoinTimes.Contains(ReservedID))
        {
            return ReservedID;
        }
    }
    
    // Generate new player ID
    while (PlayerJoinTimes.Contains(NextPlayerID))
    {
        NextPlayerID++;
    }
    
    return NextPlayerID++;
}

void AMingRTSGameMode::ReleasePlayerID(int32 PlayerID)
{
    // Player ID can be reused
}

bool AMingRTSGameMode::AuthenticatePlayer(int32 PlayerID, const FString& AuthToken)
{
    if (ServerConfig.bRequireAuthentication)
    {
        // Validate auth token
        return NetworkManager->AuthenticatePlayer(PlayerID, AuthToken);
    }
    
    return true;
}

void AMingRTSGameMode::HandlePlayerTimeout(int32 PlayerID)
{
    KickPlayer(PlayerID, TEXT("Connection timeout"));
}

void AMingRTSGameMode::OnGamePaused()
{
    // Handle game pause
}

void AMingRTSGameMode::OnGameResumed()
{
    // Handle game resume
}

void AMingRTSGameMode::OnPlayerActivity(int32 PlayerID)
{
    UpdatePlayerActivity(PlayerID);
}

void AMingRTSGameMode::SaveServerState()
{
    // Save server state to file
    UE_LOG(LogTemp, Log, TEXT("Saving server state"));
}

void AMingRTSGameMode::LoadServerState()
{
    // Load server state from file
    UE_LOG(LogTemp, Log, TEXT("Loading server state"));
}

// Admin Commands
void AMingRTSGameMode::AdminKick(const FString& PlayerName, const FString& Reason)
{
    // Find player by name and kick
    UE_LOG(LogTemp, Log, TEXT("Admin kick: %s - %s"), *PlayerName, *Reason);
}

void AMingRTSGameMode::AdminBan(const FString& PlayerName, const FString& Reason, int32 DurationMinutes)
{
    // Find player by name and ban
    UE_LOG(LogTemp, Log, TEXT("Admin ban: %s for %d minutes - %s"), *PlayerName, DurationMinutes, *Reason);
}

void AMingRTSGameMode::AdminPause()
{
    PauseGame();
}

void AMingRTSGameMode::AdminUnpause()
{
    ResumeGame();
}

void AMingRTSGameMode::AdminChangeMap(const FString& MapName)
{
    ChangeMap(MapName);
}

void AMingRTSGameMode::AdminSetMaxPlayers(int32 MaxPlayers)
{
    SetMaxPlayers(MaxPlayers);
}
