#include "Network/MingNetworkManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingNetworkManager::UMingNetworkManager()
    : ConnectionState(EMingNetworkConnectionState::Disconnected)
    , NetworkRole(EMingNetworkRole::None)
    , LocalPlayerID(-1)
    , bGameInProgress(false)
    , LastError(EMingNetworkError::None)
{
}

void UMingNetworkManager::Initialize()
{
    ConnectionState = EMingNetworkConnectionState::Disconnected;
    NetworkRole = EMingNetworkRole::None;
    ConnectedPlayers.Empty();
    LocalPlayerID = -1;
    bGameInProgress = false;
    LastError = EMingNetworkError::None;
    LastErrorMessage = TEXT("");

    UE_LOG(LogTemp, Log, TEXT("MingNetworkManager initialized"));
}

void UMingNetworkManager::Shutdown()
{
    if (IsConnected())
    {
        Disconnect();
    }

    UE_LOG(LogTemp, Log, TEXT("MingNetworkManager shutdown"));
}

bool UMingNetworkManager::CreateServer(const FMingNetworkConfig& Config)
{
    if (IsConnected())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create server: already connected"));
        return false;
    }

    CurrentConfig = Config;
    UpdateConnectionState(EMingNetworkConnectionState::Connecting);

    // Simulate server creation
    NetworkRole = EMingNetworkRole::Host;
    LocalPlayerID = 1;

    FMingPlayerNetworkInfo HostPlayer;
    HostPlayer.PlayerID = LocalPlayerID;
    HostPlayer.PlayerName = TEXT("Host");
    HostPlayer.bIsHost = true;
    HostPlayer.bIsReady = false;
    HostPlayer.Ping = 0.0f;
    HostPlayer.IPAddress = TEXT("127.0.0.1");
    HostPlayer.TeamID = 0;
    HostPlayer.bIsSpectator = false;
    HostPlayer.PlayerScore = 0;

    AddPlayer(HostPlayer);
    UpdateConnectionState(EMingNetworkConnectionState::Connected);

    UE_LOG(LogTemp, Log, TEXT("Server created: %s on port %d"), *Config.ServerName, Config.Port);
    return true;
}

void UMingNetworkManager::CloseServer()
{
    if (!IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot close server: not a host"));
        return;
    }

    // Notify all clients
    for (const auto& Player : ConnectedPlayers)
    {
        if (!Player.bIsHost)
        {
            // In real implementation, send disconnect notification
        }
    }

    ConnectedPlayers.Empty();
    UpdateConnectionState(EMingNetworkConnectionState::Disconnected);
    NetworkRole = EMingNetworkRole::None;
    LocalPlayerID = -1;

    UE_LOG(LogTemp, Log, TEXT("Server closed"));
}

bool UMingNetworkManager::ConnectToServer(const FString& ServerAddress, int32 Port, const FString& Password)
{
    if (IsConnected())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot connect: already connected"));
        return false;
    }

    UpdateConnectionState(EMingNetworkConnectionState::Connecting);

    // Validate password if server is private
    if (CurrentConfig.bIsPrivate && !ValidatePassword(Password))
    {
        HandleConnectionFailure(EMingNetworkError::InvalidPassword, TEXT("Invalid password"));
        return false;
    }

    // Simulate connection
    CurrentConfig.Port = Port;
    NetworkRole = EMingNetworkRole::Client;
    LocalPlayerID = GeneratePlayerID();

    // In real implementation, this would use Unreal's net driver
    FTimerHandle ConnectTimer;
    GetWorld()->GetTimerManager().SetTimer(ConnectTimer, [this]()
    {
        HandleConnectionSuccess();
    }, 1.0f, false);

    UE_LOG(LogTemp, Log, TEXT("Connecting to server: %s:%d"), *ServerAddress, Port);
    return true;
}

void UMingNetworkManager::Disconnect()
{
    if (!IsConnected())
    {
        return;
    }

    UpdateConnectionState(EMingNetworkConnectionState::Disconnecting);

    // Clean up player data
    if (IsHost())
    {
        CloseServer();
    }
    else
    {
        // Notify server of disconnection
        RemovePlayer(LocalPlayerID);
    }

    ConnectedPlayers.Empty();
    UpdateConnectionState(EMingNetworkConnectionState::Disconnected);
    NetworkRole = EMingNetworkRole::None;
    LocalPlayerID = -1;

    UE_LOG(LogTemp, Log, TEXT("Disconnected from server"));
}

EMingNetworkConnectionState UMingNetworkManager::GetConnectionState() const
{
    return ConnectionState;
}

bool UMingNetworkManager::IsConnected() const
{
    return ConnectionState == EMingNetworkConnectionState::Connected;
}

bool UMingNetworkManager::IsHost() const
{
    return NetworkRole == EMingNetworkRole::Host;
}

bool UMingNetworkManager::IsClient() const
{
    return NetworkRole == EMingNetworkRole::Client;
}

EMingNetworkRole UMingNetworkManager::GetNetworkRole() const
{
    return NetworkRole;
}

void UMingNetworkManager::SetPlayerReady(bool bReady)
{
    if (!IsConnected())
    {
        return;
    }

    UpdatePlayerReadyState(LocalPlayerID, bReady);

    // Notify other players
    OnPlayerReadyChanged.Broadcast(LocalPlayerID);

    UE_LOG(LogTemp, Log, TEXT("Player %d ready state: %s"), LocalPlayerID, bReady ? TEXT("Ready") : TEXT("Not Ready"));
}

bool UMingNetworkManager::IsPlayerReady(int32 PlayerID) const
{
    for (const auto& Player : ConnectedPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            return Player.bIsReady;
        }
    }
    return false;
}

TArray<FMingPlayerNetworkInfo> UMingNetworkManager::GetAllPlayers() const
{
    return ConnectedPlayers;
}

FMingPlayerNetworkInfo UMingNetworkManager::GetPlayerInfo(int32 PlayerID) const
{
    for (const auto& Player : ConnectedPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            return Player;
        }
    }
    return FMingPlayerNetworkInfo();
}

FMingPlayerNetworkInfo UMingNetworkManager::GetLocalPlayerInfo() const
{
    return GetPlayerInfo(LocalPlayerID);
}

int32 UMingNetworkManager::GetLocalPlayerID() const
{
    return LocalPlayerID;
}

void UMingNetworkManager::KickPlayer(int32 PlayerID, const FString& Reason)
{
    if (!IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot kick player: not a host"));
        return;
    }

    if (PlayerID == LocalPlayerID)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot kick yourself"));
        return;
    }

    // In real implementation, send kick message to player
    RemovePlayer(PlayerID);

    // Broadcast system message
    FMingChatMessage SystemMessage;
    SystemMessage.SenderPlayerID = -1;
    SystemMessage.SenderName = TEXT("System");
    SystemMessage.Message = FString::Printf(TEXT("Player %d was kicked: %s"), PlayerID, *Reason);
    SystemMessage.Timestamp = GetWorld()->GetTimeSeconds();
    SystemMessage.bIsSystemMessage = true;
    SystemMessage.bIsPrivate = false;

    BroadcastChatMessageToAll(SystemMessage);

    UE_LOG(LogTemp, Log, TEXT("Player %d kicked: %s"), PlayerID, *Reason);
}

void UMingNetworkManager::StartGame()
{
    if (!IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start game: not a host"));
        return;
    }

    // Check if all players are ready
    bool bAllReady = true;
    for (const auto& Player : ConnectedPlayers)
    {
        if (!Player.bIsReady && !Player.bIsSpectator)
        {
            bAllReady = false;
            break;
        }
    }

    if (!bAllReady)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot start game: not all players are ready"));
        return;
    }

    bGameInProgress = true;

    // Broadcast game start
    FMingChatMessage SystemMessage;
    SystemMessage.SenderPlayerID = -1;
    SystemMessage.SenderName = TEXT("System");
    SystemMessage.Message = TEXT("Game starting!");
    SystemMessage.Timestamp = GetWorld()->GetTimeSeconds();
    SystemMessage.bIsSystemMessage = true;

    BroadcastChatMessageToAll(SystemMessage);

    OnGameStarted.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Game started"));
}

void UMingNetworkManager::EndGame()
{
    if (!IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot end game: not a host"));
        return;
    }

    bGameInProgress = false;

    // Reset player ready states
    for (auto& Player : ConnectedPlayers)
    {
        Player.bIsReady = false;
    }

    OnGameEnded.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Game ended"));
}

bool UMingNetworkManager::IsGameInProgress() const
{
    return bGameInProgress;
}

void UMingNetworkManager::SendChatMessage(const FString& Message, bool bIsPrivate, int32 TargetPlayerID)
{
    if (!IsConnected())
    {
        return;
    }

    FMingChatMessage ChatMessage;
    ChatMessage.SenderPlayerID = LocalPlayerID;
    ChatMessage.SenderName = GetLocalPlayerInfo().PlayerName;
    ChatMessage.Message = Message;
    ChatMessage.Timestamp = GetWorld()->GetTimeSeconds();
    ChatMessage.bIsSystemMessage = false;
    ChatMessage.bIsPrivate = bIsPrivate;
    ChatMessage.TargetPlayerID = TargetPlayerID;

    if (bIsPrivate && TargetPlayerID >= 0)
    {
        BroadcastChatMessageToPlayer(ChatMessage, TargetPlayerID);
        // Also show to sender
        OnChatMessageReceived.Broadcast(ChatMessage);
    }
    else
    {
        BroadcastChatMessageToAll(ChatMessage);
    }

    UE_LOG(LogTemp, Log, TEXT("Chat message sent: %s"), *Message);
}

bool UMingNetworkManager::PerformHostMigration()
{
    if (!IsConnected() || !IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot perform host migration: not connected or not a client"));
        return false;
    }

    // Find next suitable host
    int32 NewHostID = -1;
    for (const auto& Player : ConnectedPlayers)
    {
        if (!Player.bIsHost && !Player.bIsSpectator)
        {
            NewHostID = Player.PlayerID;
            break;
        }
    }

    if (NewHostID < 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Host migration failed: no suitable new host found"));
        return false;
    }

    // Transfer host privileges
    for (auto& Player : ConnectedPlayers)
    {
        if (Player.PlayerID == NewHostID)
        {
            Player.bIsHost = true;
        }
        else if (Player.PlayerID == LocalPlayerID)
        {
            Player.bIsHost = false;
        }
    }

    if (NewHostID == LocalPlayerID)
    {
        NetworkRole = EMingNetworkRole::Host;
    }

    OnHostMigrated.Broadcast(NewHostID);

    UE_LOG(LogTemp, Log, TEXT("Host migrated to player %d"), NewHostID);
    return true;
}

FMingNetworkStats UMingNetworkManager::GetNetworkStats() const
{
    return NetworkStats;
}

void UMingNetworkManager::GetDetailedNetworkStats(FString& OutStats) const
{
    OutStats = FString::Printf(TEXT(
        "=== Network Statistics ===\n"
        "Connection State: %s\n"
        "Network Role: %s\n"
        "Local Player ID: %d\n"
        "Connected Players: %d\n"
        "Average Ping: %.2f ms\n"
        "Packet Loss: %.2f%%\n"
        "Bytes Sent/sec: %d\n"
        "Bytes Received/sec: %d\n"
        "Server Frame Time: %.2f ms\n"
        "Replicated Actors: %d\n"
        "Game In Progress: %s\n"),
        *UEnum::GetValueAsString(ConnectionState),
        *UEnum::GetValueAsString(NetworkRole),
        LocalPlayerID,
        ConnectedPlayers.Num(),
        NetworkStats.AveragePing,
        NetworkStats.PacketLossRate * 100.0f,
        NetworkStats.BytesSentPerSecond,
        NetworkStats.BytesReceivedPerSecond,
        NetworkStats.ServerFrameTime,
        NetworkStats.ReplicatedActors,
        bGameInProgress ? TEXT("Yes") : TEXT("No")
    );
}

EMingNetworkError UMingNetworkManager::GetLastError() const
{
    return LastError;
}

FString UMingNetworkManager::GetLastErrorMessage() const
{
    return LastErrorMessage;
}

void UMingNetworkManager::SetNetworkConfig(const FMingNetworkConfig& NewConfig)
{
    CurrentConfig = NewConfig;
    UE_LOG(LogTemp, Log, TEXT("Network config updated"));
}

FMingNetworkConfig UMingNetworkManager::GetNetworkConfig() const
{
    return CurrentConfig;
}

// Private helper functions

void UMingNetworkManager::UpdateConnectionState(EMingNetworkConnectionState NewState)
{
    EMingNetworkConnectionState OldState = ConnectionState;
    ConnectionState = NewState;

    if (OldState != NewState)
    {
        OnConnectionStateChanged.Broadcast(NewState);
        OnRep_ConnectionState();
    }
}

void UMingNetworkManager::HandleConnectionSuccess()
{
    UpdateConnectionState(EMingNetworkConnectionState::Connected);

    // Add local player
    FMingPlayerNetworkInfo LocalPlayer;
    LocalPlayer.PlayerID = LocalPlayerID;
    LocalPlayer.PlayerName = TEXT("Player " + FString::FromInt(LocalPlayerID));
    LocalPlayer.bIsHost = IsHost();
    LocalPlayer.bIsReady = false;
    LocalPlayer.Ping = 0.0f;
    LocalPlayer.IPAddress = TEXT("127.0.0.1");
    LocalPlayer.TeamID = 0;
    LocalPlayer.bIsSpectator = false;
    LocalPlayer.PlayerScore = 0;

    AddPlayer(LocalPlayer);

    UE_LOG(LogTemp, Log, TEXT("Connection successful"));
}

void UMingNetworkManager::HandleConnectionFailure(EMingNetworkError Error, const FString& Message)
{
    LastError = Error;
    LastErrorMessage = Message;

    UpdateConnectionState(EMingNetworkConnectionState::Error);

    OnNetworkError.Broadcast(Error);

    UE_LOG(LogTemp, Error, TEXT("Connection failed: %s - %s"), *UEnum::GetValueAsString(Error), *Message);
}

void UMingNetworkManager::UpdateNetworkStats()
{
    // Calculate average ping
    float TotalPing = 0.0f;
    for (const auto& Player : ConnectedPlayers)
    {
        TotalPing += Player.Ping;
    }

    if (ConnectedPlayers.Num() > 0)
    {
        NetworkStats.AveragePing = TotalPing / ConnectedPlayers.Num();
    }

    NetworkStats.ConnectedPlayers = ConnectedPlayers.Num();

    // In real implementation, these would come from Unreal's net driver
    NetworkStats.BytesSentPerSecond = FMath::RandRange(1000, 5000);
    NetworkStats.BytesReceivedPerSecond = FMath::RandRange(1000, 5000);
    NetworkStats.ReplicatedActors = FMath::RandRange(50, 200);
}

void UMingNetworkManager::BroadcastChatMessageToAll(const FMingChatMessage& Message)
{
    OnChatMessageReceived.Broadcast(Message);
}

void UMingNetworkManager::BroadcastChatMessageToPlayer(const FMingChatMessage& Message, int32 TargetPlayerID)
{
    // In real implementation, send to specific player only
    // For now, broadcast to all (would be filtered in UI)
    OnChatMessageReceived.Broadcast(Message);
}

void UMingNetworkManager::AddPlayer(const FMingPlayerNetworkInfo& PlayerInfo)
{
    // Check if player already exists
    for (int32 i = 0; i < ConnectedPlayers.Num(); ++i)
    {
        if (ConnectedPlayers[i].PlayerID == PlayerInfo.PlayerID)
        {
            ConnectedPlayers[i] = PlayerInfo;
            return;
        }
    }

    ConnectedPlayers.Add(PlayerInfo);
    OnPlayerJoined.Broadcast(PlayerInfo);

    UE_LOG(LogTemp, Log, TEXT("Player %d joined"), PlayerInfo.PlayerID);
}

void UMingNetworkManager::RemovePlayer(int32 PlayerID)
{
    for (int32 i = 0; i < ConnectedPlayers.Num(); ++i)
    {
        if (ConnectedPlayers[i].PlayerID == PlayerID)
        {
            ConnectedPlayers.RemoveAt(i);
            OnPlayerLeft.Broadcast(PlayerID);

            UE_LOG(LogTemp, Log, TEXT("Player %d left"), PlayerID);
            return;
        }
    }
}

void UMingNetworkManager::UpdatePlayerReadyState(int32 PlayerID, bool bReady)
{
    for (auto& Player : ConnectedPlayers)
    {
        if (Player.PlayerID == PlayerID)
        {
            Player.bIsReady = bReady;
            return;
        }
    }
}

int32 UMingNetworkManager::GeneratePlayerID()
{
    static int32 NextPlayerID = 1;
    return NextPlayerID++;
}

bool UMingNetworkManager::ValidatePassword(const FString& Password) const
{
    // Simple password validation
    return Password.Equals(CurrentConfig.Password, ESearchCase::CaseSensitive);
}

void UMingNetworkManager::OnRep_ConnectionState()
{
    // Replication callback - could be used for UI updates
}
