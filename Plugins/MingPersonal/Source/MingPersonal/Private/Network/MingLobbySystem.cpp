#include "Network/MingLobbySystem.h"
#include "Network/MingNetworkManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingLobbySystem::UMingLobbySystem()
    : NetworkManager(nullptr)
    , bInLobby(false)
    , bIsHost(false)
    , bCountdownActive(false)
    , CountdownRemaining(0)
{
}

void UMingLobbySystem::Initialize(UMingNetworkManager* InNetworkManager)
{
    NetworkManager = InNetworkManager;
    bInLobby = false;
    bIsHost = false;
    LobbyPlayers.Empty();
    ChatHistory.Empty();
    Teams.Empty();
    CountdownRemaining = 0;
    bCountdownActive = false;

    UE_LOG(LogTemp, Log, TEXT("MingLobbySystem initialized"));
}

void UMingLobbySystem::Shutdown()
{
    if (bInLobby)
    {
        LeaveLobby();
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
    }

    UE_LOG(LogTemp, Log, TEXT("MingLobbySystem shutdown"));
}

bool UMingLobbySystem::CreateLobby(const FMingLobbySettings& Settings)
{
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create lobby: NetworkManager not initialized"));
        return false;
    }

    if (bInLobby)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create lobby: already in a lobby"));
        return false;
    }

    if (!ValidateSettings(Settings))
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot create lobby: invalid settings"));
        return false;
    }

    CurrentSettings = Settings;
    GenerateLobbyID();
    bInLobby = true;
    bIsHost = true;

    // Add host player
    FMingLobbyPlayer HostPlayer;
    HostPlayer.PlayerID = NetworkManager->GetLocalPlayerID();
    HostPlayer.PlayerName = TEXT("Host");
    HostPlayer.TeamID = 0;
    HostPlayer.PlayerState = EMingLobbyPlayerState::Connected;
    HostPlayer.bIsHost = true;
    HostPlayer.SelectedFaction = CurrentSettings.AvailableFactions.Num() > 0 ? CurrentSettings.AvailableFactions[0] : TEXT("Ming");
    LobbyPlayers.Add(HostPlayer);

    // Initialize teams
    UpdateTeams();

    // Send system message
    SendSystemMessage(TEXT("Lobby created. Waiting for players..."));

    UE_LOG(LogTemp, Log, TEXT("Lobby created: %s (ID: %s)"), *CurrentSettings.LobbyName, *LobbyID);
    return true;
}

void UMingLobbySystem::CloseLobby()
{
    if (!bInLobby || !bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot close lobby: not in lobby or not host"));
        return;
    }

    SendSystemMessage(TEXT("Lobby is closing..."));

    // Notify all players
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsHost)
        {
            // In real implementation, notify player
        }
    }

    LobbyPlayers.Empty();
    ChatHistory.Empty();
    Teams.Empty();
    bInLobby = false;
    bIsHost = false;

    UE_LOG(LogTemp, Log, TEXT("Lobby closed"));
}

bool UMingLobbySystem::JoinLobby(const FString& InLobbyID, const FString& Password)
{
    if (!NetworkManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot join lobby: NetworkManager not initialized"));
        return false;
    }

    if (bInLobby)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot join lobby: already in a lobby"));
        return false;
    }

    // In real implementation, validate password and connect to server
    LobbyID = InLobbyID;
    bInLobby = true;
    bIsHost = false;

    // Add local player
    FMingLobbyPlayer LocalPlayer;
    LocalPlayer.PlayerID = NetworkManager->GetLocalPlayerID();
    LocalPlayer.PlayerName = FString::Printf(TEXT("Player %d"), LocalPlayer.PlayerID);
    LocalPlayer.TeamID = 0;
    LocalPlayer.PlayerState = EMingLobbyPlayerState::Connected;
    LocalPlayer.bIsHost = false;
    LocalPlayer.SelectedFaction = TEXT("Ming");
    LobbyPlayers.Add(LocalPlayer);

    UpdateTeams();

    SendSystemMessage(FString::Printf(TEXT("Player %d joined the lobby"), LocalPlayer.PlayerID));

    UE_LOG(LogTemp, Log, TEXT("Joined lobby: %s"), *LobbyID);
    return true;
}

void UMingLobbySystem::LeaveLobby()
{
    if (!bInLobby)
    {
        return;
    }

    SendSystemMessage(TEXT("A player left the lobby"));

    LobbyPlayers.Empty();
    ChatHistory.Empty();
    Teams.Empty();
    bInLobby = false;
    bIsHost = false;

    // Cancel countdown if active
    if (bCountdownActive)
    {
        CancelGameCountdown();
    }

    UE_LOG(LogTemp, Log, TEXT("Left lobby"));
}

bool UMingLobbySystem::IsInLobby() const
{
    return bInLobby;
}

TArray<FMingLobbyPlayer> UMingLobbySystem::GetLobbyPlayers() const
{
    return LobbyPlayers;
}

FMingLobbyPlayer UMingLobbySystem::GetLocalPlayer() const
{
    if (NetworkManager)
    {
        int32 LocalID = NetworkManager->GetLocalPlayerID();
        for (const auto& Player : LobbyPlayers)
        {
            if (Player.PlayerID == LocalID)
            {
                return Player;
            }
        }
    }
    return FMingLobbyPlayer();
}

void UMingLobbySystem::SetPlayerReady(bool bReady)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    int32 LocalID = NetworkManager->GetLocalPlayerID();
    FMingLobbyPlayer* Player = GetPlayerByID(LocalID);
    
    if (Player)
    {
        Player->PlayerState = bReady ? EMingLobbyPlayerState::Ready : EMingLobbyPlayerState::Connected;
        
        FString StatusMsg = FString::Printf(TEXT("Player %d is %s"), 
            LocalID, bReady ? TEXT("ready") : TEXT("not ready"));
        SendSystemMessage(StatusMsg);

        OnPlayerStateChanged.Broadcast(LocalID);

        UE_LOG(LogTemp, Log, TEXT("Player %d ready state: %s"), LocalID, bReady ? TEXT("Ready") : TEXT("Not Ready"));
    }
}

void UMingLobbySystem::SetPlayerTeam(int32 TeamID)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    int32 LocalID = NetworkManager->GetLocalPlayerID();
    FMingLobbyPlayer* Player = GetPlayerByID(LocalID);
    
    if (Player)
    {
        // Remove from old team
        RemovePlayerFromTeam(LocalID);
        
        // Add to new team
        Player->TeamID = TeamID;
        AddPlayerToTeam(LocalID, TeamID);

        FString TeamMsg = FString::Printf(TEXT("Player %d moved to Team %d"), LocalID, TeamID);
        SendSystemMessage(TeamMsg);

        OnPlayerTeamChanged.Broadcast(LocalID);

        UE_LOG(LogTemp, Log, TEXT("Player %d set to team %d"), LocalID, TeamID);
    }
}

void UMingLobbySystem::SetPlayerFaction(const FString& FactionName)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    int32 LocalID = NetworkManager->GetLocalPlayerID();
    FMingLobbyPlayer* Player = GetPlayerByID(LocalID);
    
    if (Player)
    {
        // Validate faction
        if (CurrentSettings.AvailableFactions.Contains(FactionName))
        {
            Player->SelectedFaction = FactionName;
            UE_LOG(LogTemp, Log, TEXT("Player %d selected faction: %s"), LocalID, *FactionName);
        }
    }
}

void UMingLobbySystem::SetPlayerColor(const FString& ColorName)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    int32 LocalID = NetworkManager->GetLocalPlayerID();
    FMingLobbyPlayer* Player = GetPlayerByID(LocalID);
    
    if (Player)
    {
        Player->SelectedColor = ColorName;
        UE_LOG(LogTemp, Log, TEXT("Player %d selected color: %s"), LocalID, *ColorName);
    }
}

void UMingLobbySystem::SetPlayerSpectator(bool bSpectator)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    int32 LocalID = NetworkManager->GetLocalPlayerID();
    FMingLobbyPlayer* Player = GetPlayerByID(LocalID);
    
    if (Player)
    {
        Player->bIsSpectator = bSpectator;
        
        FString SpecMsg = FString::Printf(TEXT("Player %d is now a %s"), 
            LocalID, bSpectator ? TEXT("spectator") : TEXT("player"));
        SendSystemMessage(SpecMsg);

        UE_LOG(LogTemp, Log, TEXT("Player %d spectator state: %s"), LocalID, bSpectator ? TEXT("Yes") : TEXT("No"));
    }
}

void UMingLobbySystem::KickPlayerFromLobby(int32 PlayerID, const FString& Reason)
{
    if (!bInLobby || !bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot kick player: not host"));
        return;
    }

    if (NetworkManager && PlayerID == NetworkManager->GetLocalPlayerID())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot kick yourself"));
        return;
    }

    FMingLobbyPlayer* Player = GetPlayerByID(PlayerID);
    if (Player)
    {
        FString KickMsg = FString::Printf(TEXT("Player %d was kicked: %s"), PlayerID, *Reason);
        SendSystemMessage(KickMsg);

        RemovePlayerFromTeam(PlayerID);
        
        for (int32 i = 0; i < LobbyPlayers.Num(); ++i)
        {
            if (LobbyPlayers[i].PlayerID == PlayerID)
            {
                OnPlayerLeft.Broadcast(PlayerID);
                LobbyPlayers.RemoveAt(i);
                break;
            }
        }

        UE_LOG(LogTemp, Log, TEXT("Player %d kicked: %s"), PlayerID, *Reason);
    }
}

void UMingLobbySystem::UpdateLobbySettings(const FMingLobbySettings& NewSettings)
{
    if (!bInLobby || !bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot update settings: not host"));
        return;
    }

    if (!ValidateSettings(NewSettings))
    {
        UE_LOG(LogTemp, Error, TEXT("Cannot update settings: invalid settings"));
        return;
    }

    CurrentSettings = NewSettings;
    
    // Update teams if number changed
    UpdateTeams();

    SendSystemMessage(TEXT("Lobby settings updated"));
    OnSettingsChanged.Broadcast(CurrentSettings);

    UE_LOG(LogTemp, Log, TEXT("Lobby settings updated"));
}

FMingLobbySettings UMingLobbySystem::GetLobbySettings() const
{
    return CurrentSettings;
}

void UMingLobbySystem::SetGameMode(EMingLobbyGameMode NewMode)
{
    if (!bInLobby || !bIsHost)
    {
        return;
    }

    CurrentSettings.GameMode = NewMode;
    UpdateLobbySettings(CurrentSettings);

    UE_LOG(LogTemp, Log, TEXT("Game mode changed to: %s"), *UEnum::GetValueAsString(NewMode));
}

void UMingLobbySystem::SetMap(const FString& MapName)
{
    if (!bInLobby || !bIsHost)
    {
        return;
    }

    CurrentSettings.MapName = MapName;
    UpdateLobbySettings(CurrentSettings);

    UE_LOG(LogTemp, Log, TEXT("Map changed to: %s"), *MapName);
}

void UMingLobbySystem::SetMaxPlayers(int32 MaxPlayers)
{
    if (!bInLobby || !bIsHost)
    {
        return;
    }

    CurrentSettings.MaxPlayers = FMath::Clamp(MaxPlayers, 2, 16);
    UpdateLobbySettings(CurrentSettings);

    UE_LOG(LogTemp, Log, TEXT("Max players set to: %d"), CurrentSettings.MaxPlayers);
}

void UMingLobbySystem::SetNumTeams(int32 NumTeams)
{
    if (!bInLobby || !bIsHost)
    {
        return;
    }

    CurrentSettings.NumTeams = FMath::Clamp(NumTeams, 1, 4);
    UpdateLobbySettings(CurrentSettings);

    UE_LOG(LogTemp, Log, TEXT("Number of teams set to: %d"), CurrentSettings.NumTeams);
}

void UMingLobbySystem::AutoBalanceTeams()
{
    if (!bInLobby || !bIsHost)
    {
        return;
    }

    if (!CurrentSettings.bAutoBalanceTeams)
    {
        return;
    }

    // Simple round-robin team assignment
    int32 TeamIndex = 0;
    for (auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator)
        {
            Player.TeamID = TeamIndex % CurrentSettings.NumTeams;
            TeamIndex++;
        }
    }

    UpdateTeams();

    SendSystemMessage(TEXT("Teams auto-balanced"));
    UE_LOG(LogTemp, Log, TEXT("Teams auto-balanced"));
}

TArray<FMingLobbyTeam> UMingLobbySystem::GetTeams() const
{
    return Teams;
}

int32 UMingLobbySystem::GetTeamPlayerCount(int32 TeamID) const
{
    for (const auto& Team : Teams)
    {
        if (Team.TeamID == TeamID)
        {
            return Team.PlayerIDs.Num();
        }
    }
    return 0;
}

void UMingLobbySystem::StartGameCountdown(int32 CountdownSeconds)
{
    if (!bInLobby || !bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start countdown: not host"));
        return;
    }

    if (!CanStartGame())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start countdown: not all conditions met"));
        return;
    }

    CountdownRemaining = CountdownSeconds;
    bCountdownActive = true;

    SendSystemMessage(FString::Printf(TEXT("Game starting in %d seconds..."), CountdownSeconds));

    // Start timer
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UMingLobbySystem::UpdateCountdown, 1.0f, true);
    }

    OnGameStarting.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Game countdown started: %d seconds"), CountdownSeconds);
}

void UMingLobbySystem::CancelGameCountdown()
{
    if (!bCountdownActive)
    {
        return;
    }

    bCountdownActive = false;
    CountdownRemaining = 0;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
    }

    SendSystemMessage(TEXT("Game start cancelled"));

    UE_LOG(LogTemp, Log, TEXT("Game countdown cancelled"));
}

bool UMingLobbySystem::IsCountdownActive() const
{
    return bCountdownActive;
}

int32 UMingLobbySystem::GetCountdownRemaining() const
{
    return CountdownRemaining;
}

void UMingLobbySystem::StartGame()
{
    if (!bInLobby || !bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start game: not host"));
        return;
    }

    if (!CanStartGame())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start game: not all conditions met"));
        return;
    }

    // Cancel countdown if still active
    if (bCountdownActive)
    {
        CancelGameCountdown();
    }

    // Update player states
    for (auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator)
        {
            Player.PlayerState = EMingLobbyPlayerState::InGame;
        }
    }

    SendSystemMessage(TEXT("Game started!"));
    OnGameStarted.Broadcast();

    // Notify network manager to start game
    if (NetworkManager)
    {
        NetworkManager->StartGame();
    }

    UE_LOG(LogTemp, Log, TEXT("Game started"));
}

bool UMingLobbySystem::CanStartGame() const
{
    if (LobbyPlayers.Num() < 1)
    {
        return false;
    }

    // Check if at least 2 non-spectator players
    int32 ActivePlayers = 0;
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator)
        {
            ActivePlayers++;
        }
    }

    if (ActivePlayers < 2)
    {
        return false;
    }

    // Check if all active players are ready
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator && Player.PlayerState != EMingLobbyPlayerState::Ready)
        {
            return false;
        }
    }

    // Check team configuration
    if (!IsValidTeamConfiguration())
    {
        return false;
    }

    return true;
}

void UMingLobbySystem::SendLobbyChatMessage(const FString& Message)
{
    if (!bInLobby || !NetworkManager)
    {
        return;
    }

    FMingLobbyChatMessage ChatMsg;
    ChatMsg.SenderPlayerID = NetworkManager->GetLocalPlayerID();
    ChatMsg.SenderName = GetLocalPlayer().PlayerName;
    ChatMsg.Message = Message;
    ChatMsg.Timestamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    ChatMsg.bIsSystemMessage = false;

    ChatHistory.Add(ChatMsg);
    BroadcastChatMessage(ChatMsg);

    UE_LOG(LogTemp, Log, TEXT("Lobby chat: %s: %s"), *ChatMsg.SenderName, *Message);
}

TArray<FMingLobbyChatMessage> UMingLobbySystem::GetChatHistory() const
{
    return ChatHistory;
}

FString UMingLobbySystem::GetLobbyID() const
{
    return LobbyID;
}

bool UMingLobbySystem::IsLobbyHost() const
{
    return bIsHost;
}

int32 UMingLobbySystem::GetPlayerCount() const
{
    return LobbyPlayers.Num();
}

int32 UMingLobbySystem::GetReadyPlayerCount() const
{
    int32 ReadyCount = 0;
    for (const auto& Player : LobbyPlayers)
    {
        if (Player.PlayerState == EMingLobbyPlayerState::Ready || Player.PlayerState == EMingLobbyPlayerState::InGame)
        {
            ReadyCount++;
        }
    }
    return ReadyCount;
}

void UMingLobbySystem::GetLobbyInfo(FString& OutInfo) const
{
    OutInfo = FString::Printf(TEXT(
        "=== Lobby Information ===\n"
        "Lobby ID: %s\n"
        "Name: %s\n"
        "Host: %s\n"
        "Players: %d/%d\n"
        "Ready: %d\n"
        "Game Mode: %s\n"
        "Map: %s\n"
        "Teams: %d\n"),
        *LobbyID,
        *CurrentSettings.LobbyName,
        bIsHost ? TEXT("Yes") : TEXT("No"),
        GetPlayerCount(),
        CurrentSettings.MaxPlayers,
        GetReadyPlayerCount(),
        *UEnum::GetValueAsString(CurrentSettings.GameMode),
        *CurrentSettings.MapName,
        CurrentSettings.NumTeams
    );
}

bool UMingLobbySystem::IsPlayerReady(int32 PlayerID) const
{
    for (const auto& Player : LobbyPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            return Player.PlayerState == EMingLobbyPlayerState::Ready || Player.PlayerState == EMingLobbyPlayerState::InGame;
        }
    }
    return false;
}

bool UMingLobbySystem::AreAllPlayersReady() const
{
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator && Player.PlayerState != EMingLobbyPlayerState::Ready)
        {
            return false;
        }
    }
    return LobbyPlayers.Num() > 0;
}

bool UMingLobbySystem::IsValidTeamConfiguration() const
{
    // Check if teams are balanced enough
    TArray<int32> TeamCounts;
    TeamCounts.SetNum(CurrentSettings.NumTeams);
    
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator && Player.TeamID >= 0 && Player.TeamID < CurrentSettings.NumTeams)
        {
            TeamCounts[Player.TeamID]++;
        }
    }

    // Check if all teams have at least one player
    for (int32 Count : TeamCounts)
    {
        if (Count == 0)
        {
            return false;
        }
    }

    // Check balance (difference should not be more than 1)
    int32 MinCount = INT32_MAX;
    int32 MaxCount = 0;
    for (int32 Count : TeamCounts)
    {
        MinCount = FMath::Min(MinCount, Count);
        MaxCount = FMath::Max(MaxCount, Count);
    }

    return (MaxCount - MinCount) <= 1;
}

// Private helper functions

void UMingLobbySystem::GenerateLobbyID()
{
    // Generate a simple lobby ID
    FDateTime Now = FDateTime::Now();
    LobbyID = FString::Printf(TEXT("LOBBY_%s_%d"), 
        *Now.ToString(TEXT("%Y%m%d%H%M%S")),
        FMath::RandRange(1000, 9999));
}

void UMingLobbySystem::UpdateTeams()
{
    Teams.Empty();
    
    for (int32 i = 0; i < CurrentSettings.NumTeams; ++i)
    {
        FMingLobbyTeam Team;
        Team.TeamID = i;
        Team.TeamName = FString::Printf(TEXT("Team %d"), i + 1);
        
        // Assign colors
        TArray<FString> Colors = { TEXT("Red"), TEXT("Blue"), TEXT("Green"), TEXT("Yellow") };
        if (i < Colors.Num())
        {
            Team.TeamColor = Colors[i];
        }
        
        Teams.Add(Team);
    }

    // Populate teams with players
    for (const auto& Player : LobbyPlayers)
    {
        if (!Player.bIsSpectator && Player.TeamID >= 0 && Player.TeamID < Teams.Num())
        {
            Teams[Player.TeamID].PlayerIDs.Add(Player.PlayerID);
        }
    }

    // Update full status
    int32 MaxPerTeam = CurrentSettings.MaxPlayers / CurrentSettings.NumTeams;
    for (auto& Team : Teams)
    {
        Team.bIsFull = Team.PlayerIDs.Num() >= MaxPerTeam;
    }
}

void UMingLobbySystem::BroadcastSettingsChanged()
{
    OnSettingsChanged.Broadcast(CurrentSettings);
}

void UMingLobbySystem::BroadcastChatMessage(const FMingLobbyChatMessage& Message)
{
    OnChatMessageReceived.Broadcast(Message);
}

void UMingLobbySystem::UpdateCountdown()
{
    if (!bCountdownActive)
    {
        return;
    }

    CountdownRemaining--;

    OnCountdownChanged.Broadcast(CountdownRemaining);

    if (CountdownRemaining <= 0)
    {
        OnCountdownFinished();
    }
    else if (CountdownRemaining <= 5)
    {
        // Announce final seconds
        SendSystemMessage(FString::Printf(TEXT("%d..."), CountdownRemaining));
    }
}

void UMingLobbySystem::OnCountdownFinished()
{
    bCountdownActive = false;
    
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
    }

    StartGame();
}

FMingLobbyPlayer* UMingLobbySystem::GetPlayerByID(int32 PlayerID)
{
    for (auto& Player : LobbyPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            return &Player;
        }
    }
    return nullptr;
}

const FMingLobbyPlayer* UMingLobbySystem::GetPlayerByID(int32 PlayerID) const
{
    for (const auto& Player : LobbyPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            return &Player;
        }
    }
    return nullptr;
}

void UMingLobbySystem::AddPlayerToTeam(int32 PlayerID, int32 TeamID)
{
    if (TeamID >= 0 && TeamID < Teams.Num())
    {
        if (!Teams[TeamID].PlayerIDs.Contains(PlayerID))
        {
            Teams[TeamID].PlayerIDs.Add(PlayerID);
        }
        UpdateTeams();
    }
}

void UMingLobbySystem::RemovePlayerFromTeam(int32 PlayerID)
{
    for (auto& Team : Teams)
    {
        Team.PlayerIDs.Remove(PlayerID);
    }
    UpdateTeams();
}

void UMingLobbySystem::UpdatePlayerState(int32 PlayerID, EMingLobbyPlayerState NewState)
{
    FMingLobbyPlayer* Player = GetPlayerByID(PlayerID);
    if (Player)
    {
        Player->PlayerState = NewState;
        OnPlayerStateChanged.Broadcast(PlayerID);
    }
}

bool UMingLobbySystem::ValidateSettings(const FMingLobbySettings& Settings) const
{
    if (Settings.MaxPlayers < 2 || Settings.MaxPlayers > 16)
    {
        return false;
    }

    if (Settings.NumTeams < 1 || Settings.NumTeams > 4)
    {
        return false;
    }

    if (Settings.MapName.IsEmpty())
    {
        return false;
    }

    if (Settings.GameSpeed <= 0.0f || Settings.GameSpeed > 3.0f)
    {
        return false;
    }

    return true;
}

void UMingLobbySystem::SendSystemMessage(const FString& Message)
{
    FMingLobbyChatMessage SystemMsg;
    SystemMsg.SenderPlayerID = -1;
    SystemMsg.SenderName = TEXT("System");
    SystemMsg.Message = Message;
    SystemMsg.Timestamp = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.0f;
    SystemMsg.bIsSystemMessage = true;

    ChatHistory.Add(SystemMsg);
    BroadcastChatMessage(SystemMsg);
}
