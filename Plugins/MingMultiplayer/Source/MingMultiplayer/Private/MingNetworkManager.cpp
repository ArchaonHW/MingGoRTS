#include "MingNetworkManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"

DEFINE_STAT(STAT_MingNetworkTick);

UMingNetworkManager::UMingNetworkManager()
    : CurrentRole(EMingNetworkRole::None)
    , CurrentState(EMingConnectionState::Disconnected)
    , LocalPlayerID(-1)
    , CurrentTickRate(30)
    , LastNetworkUpdateTime(0.0f)
    , NetworkUpdateInterval(0.033f)
{
}

void UMingNetworkManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
}

void UMingNetworkManager::Deinitialize()
{
    ShutdownNetwork();
    Super::Deinitialize();
}

void UMingNetworkManager::Tick(float DeltaTime)
{
    SCOPE_CYCLE_COUNTER(STAT_MingNetworkTick);

    if (CurrentState == EMingConnectionState::Connected || 
        CurrentState == EMingConnectionState::Authenticated ||
        CurrentState == EMingConnectionState::Ready)
    {
        // Update network statistics
        UpdateNetworkStats(DeltaTime);
        
        // Process incoming messages
        ProcessIncomingMessages();
        
        // Send periodic heartbeat
        LastNetworkUpdateTime += DeltaTime;
        if (LastNetworkUpdateTime >= NetworkUpdateInterval)
        {
            SendHeartbeat();
            LastNetworkUpdateTime = 0.0f;
        }
        
        // Check connection health
        CheckConnectionHealth();
    }
}

TStatId UMingNetworkManager::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(MingNetworkManager, STATGROUP_Tickables);
}

void UMingNetworkManager::InitializeNetwork()
{
    CurrentState = EMingConnectionState::Disconnected;
    CurrentRole = EMingNetworkRole::None;
    LocalPlayerID = -1;
    ConnectedPlayers.Empty();
    BannedPlayers.Empty();
}

void UMingNetworkManager::ShutdownNetwork()
{
    if (CurrentRole == EMingNetworkRole::Host || 
        CurrentRole == EMingNetworkRole::DedicatedServer ||
        CurrentRole == EMingNetworkRole::ListenServer)
    {
        StopServer();
    }
    else if (CurrentRole == EMingNetworkRole::Client)
    {
        Disconnect();
    }
    
    CurrentState = EMingConnectionState::Disconnected;
    CurrentRole = EMingNetworkRole::None;
}

bool UMingNetworkManager::ConnectToServer(const FString& ServerAddress, int32 Port)
{
    if (CurrentState != EMingConnectionState::Disconnected)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot connect: already in state %d"), static_cast<int32>(CurrentState));
        return false;
    }

    SetConnectionState(EMingConnectionState::Connecting);
    
    // Connection logic would be implemented here
    // This is a placeholder for actual UE network connection
    
    UE_LOG(LogTemp, Log, TEXT("Connecting to server: %s:%d"), *ServerAddress, Port);
    
    // Simulate successful connection for now
    SetConnectionState(EMingConnectionState::Connected);
    CurrentRole = EMingNetworkRole::Client;
    
    return true;
}

void UMingNetworkManager::Disconnect()
{
    if (CurrentState == EMingConnectionState::Disconnected)
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Disconnecting from server"));
    
    // Cleanup
    ConnectedPlayers.Empty();
    
    SetConnectionState(EMingConnectionState::Disconnected);
    CurrentRole = EMingNetworkRole::None;
    LocalPlayerID = -1;
}

bool UMingNetworkManager::CreateSession(const FMingNetworkSessionInfo& SessionInfo)
{
    if (CurrentState != EMingConnectionState::Disconnected)
    {
        return false;
    }

    CurrentSession = SessionInfo;
    CurrentSession.CreationTime = FDateTime::Now();
    
    // Generate unique session ID
    CurrentSession.SessionID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
    
    UE_LOG(LogTemp, Log, TEXT("Created session: %s"), *CurrentSession.SessionID);
    
    OnSessionCreated.Broadcast(CurrentSession.SessionID);
    
    return true;
}

bool UMingNetworkManager::JoinSession(const FString& SessionID, const FString& Password)
{
    if (CurrentState != EMingConnectionState::Disconnected)
    {
        return false;
    }

    // Check password if session is password protected
    if (!CurrentSession.bIsPasswordProtected || ServerPassword == Password)
    {
        CurrentSession.SessionID = SessionID;
        SetConnectionState(EMingConnectionState::Connected);
        
        UE_LOG(LogTemp, Log, TEXT("Joined session: %s"), *SessionID);
        
        OnSessionJoined.Broadcast(SessionID);
        
        return true;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Failed to join session: incorrect password"));
    return false;
}

void UMingNetworkManager::LeaveSession()
{
    if (!CurrentSession.SessionID.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("Leaving session: %s"), *CurrentSession.SessionID);
        
        CurrentSession.SessionID.Empty();
        Disconnect();
    }
}

void UMingNetworkManager::DiscoverSessions()
{
    UE_LOG(LogTemp, Log, TEXT("Discovering network sessions..."));
    
    // Session discovery logic would be implemented here
    // This would typically use UE's OnlineSession system
}

bool UMingNetworkManager::StartListenServer(int32 Port, int32 MaxPlayers)
{
    if (CurrentState != EMingConnectionState::Disconnected)
    {
        return false;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return false;
    }

    // Start listen server
    FURL ListenURL(nullptr, TEXT("/Game/Maps/MainMenu"), TRAVEL_Absolute);
    ListenURL.Port = Port;
    
    FString Error;
    bool bSuccess = World->Listen(ListenURL);
    
    if (bSuccess)
    {
        CurrentRole = EMingNetworkRole::ListenServer;
        LocalPlayerID = 0; // Host is always player 0
        
        CurrentSession.SessionID = FGuid::NewGuid().ToString(EGuidFormats::DigitsWithHyphensLower);
        CurrentSession.MaxPlayers = MaxPlayers;
        CurrentSession.CurrentPlayers = 1;
        CurrentSession.HostName = TEXT("Host");
        
        SetConnectionState(EMingConnectionState::Ready);
        
        UE_LOG(LogTemp, Log, TEXT("Started listen server on port %d"), Port);
        
        return true;
    }
    
    UE_LOG(LogTemp, Error, TEXT("Failed to start listen server: %s"), *Error);
    return false;
}

bool UMingNetworkManager::StartDedicatedServer(int32 Port)
{
    // Dedicated server logic would be implemented here
    CurrentRole = EMingNetworkRole::DedicatedServer;
    
    UE_LOG(LogTemp, Log, TEXT("Started dedicated server on port %d"), Port);
    
    return true;
}

void UMingNetworkManager::StopServer()
{
    if (CurrentRole == EMingNetworkRole::ListenServer || 
        CurrentRole == EMingNetworkRole::DedicatedServer)
    {
        // Notify all clients
        for (const auto& Pair : ConnectedPlayers)
        {
            // Send disconnect notification
        }
        
        UWorld* World = GetWorld();
        if (World)
        {
            World->ServerTravel(TEXT("/Game/Maps/MainMenu"));
        }
        
        CurrentRole = EMingNetworkRole::None;
        SetConnectionState(EMingConnectionState::Disconnected);
        
        UE_LOG(LogTemp, Log, TEXT("Server stopped"));
    }
}

void UMingNetworkManager::RegisterPlayer(int32 PlayerID, const FMingPlayerNetworkInfo& PlayerInfo)
{
    ConnectedPlayers.Add(PlayerID, PlayerInfo);
    CurrentSession.CurrentPlayers = ConnectedPlayers.Num();
    
    UE_LOG(LogTemp, Log, TEXT("Player %d registered: %s"), PlayerID, *PlayerInfo.PlayerName);
    
    OnPlayerConnected.Broadcast(PlayerID, PlayerInfo);
}

void UMingNetworkManager::UnregisterPlayer(int32 PlayerID)
{
    if (ConnectedPlayers.Contains(PlayerID))
    {
        UE_LOG(LogTemp, Log, TEXT("Player %d unregistered"), PlayerID);
        
        OnPlayerDisconnected.Broadcast(PlayerID);
        
        ConnectedPlayers.Remove(PlayerID);
        CurrentSession.CurrentPlayers = ConnectedPlayers.Num();
    }
}

FMingPlayerNetworkInfo UMingNetworkManager::GetPlayerInfo(int32 PlayerID) const
{
    if (ConnectedPlayers.Contains(PlayerID))
    {
        return ConnectedPlayers[PlayerID];
    }
    
    return FMingPlayerNetworkInfo();
}

TArray<FMingPlayerNetworkInfo> UMingNetworkManager::GetAllPlayers() const
{
    TArray<FMingPlayerNetworkInfo> Result;
    ConnectedPlayers.GenerateValueArray(Result);
    return Result;
}

int32 UMingNetworkManager::GetLocalPlayerID() const
{
    return LocalPlayerID;
}

bool UMingNetworkManager::IsHost() const
{
    return CurrentRole == EMingNetworkRole::Host || 
           CurrentRole == EMingNetworkRole::ListenServer ||
           CurrentRole == EMingNetworkRole::DedicatedServer;
}

bool UMingNetworkManager::IsConnected() const
{
    return CurrentState == EMingConnectionState::Connected || 
           CurrentState == EMingConnectionState::Authenticated ||
           CurrentState == EMingConnectionState::Ready;
}

bool UMingNetworkManager::SendMessage(int32 TargetPlayerID, int32 MessageType, const TArray<uint8>& Data,
    EMingSyncPriority Priority, bool bReliable)
{
    if (!IsConnected())
    {
        return false;
    }

    FMingNetworkMessage Message;
    Message.SenderID = LocalPlayerID;
    Message.TargetID = TargetPlayerID;
    Message.MessageType = MessageType;
    Message.Payload = Data;
    Message.Priority = Priority;
    Message.bReliable = bReliable;
    Message.Timestamp = FDateTime::Now();
    
    // Message sending logic would be implemented here
    
    return true;
}

bool UMingNetworkManager::BroadcastMessage(int32 MessageType, const TArray<uint8>& Data, EMingSyncPriority Priority)
{
    if (!IsConnected())
    {
        return false;
    }

    // Broadcast to all connected players
    for (const auto& Pair : ConnectedPlayers)
    {
        SendMessage(Pair.Key, MessageType, Data, Priority, false);
    }
    
    return true;
}

bool UMingNetworkManager::SendRPC(UObject* TargetObject, const FString& FunctionName, const TArray<uint8>& Parameters)
{
    if (!TargetObject || !IsConnected())
    {
        return false;
    }

    // RPC sending logic would be implemented here
    // This would use UE's RPC system
    
    return true;
}

EMingNetworkRole UMingNetworkManager::GetNetworkRole() const
{
    return CurrentRole;
}

EMingConnectionState UMingNetworkManager::GetConnectionState() const
{
    return CurrentState;
}

void UMingNetworkManager::SetConnectionState(EMingConnectionState NewState)
{
    if (CurrentState != NewState)
    {
        EMingConnectionState OldState = CurrentState;
        CurrentState = NewState;
        
        OnConnectionStateChanged.Broadcast(OldState, NewState);
        
        UE_LOG(LogTemp, Log, TEXT("Connection state changed: %d -> %d"), 
            static_cast<int32>(OldState), static_cast<int32>(NewState));
    }
}

FMingNetworkStats UMingNetworkManager::GetNetworkStats() const
{
    return NetworkStats;
}

float UMingNetworkManager::GetPlayerPing(int32 PlayerID) const
{
    if (ConnectedPlayers.Contains(PlayerID))
    {
        return ConnectedPlayers[PlayerID].PingMs;
    }
    
    return -1.0f;
}

void UMingNetworkManager::SetTickRate(int32 TickRate)
{
    CurrentTickRate = FMath::Clamp(TickRate, 10, 60);
    NetworkUpdateInterval = 1.0f / CurrentTickRate;
}

int32 UMingNetworkManager::GetTickRate() const
{
    return CurrentTickRate;
}

void UMingNetworkManager::SetServerPassword(const FString& Password)
{
    ServerPassword = Password;
    CurrentSession.bIsPasswordProtected = !Password.IsEmpty();
}

bool UMingNetworkManager::AuthenticatePlayer(int32 PlayerID, const FString& AuthToken)
{
    // Authentication logic would be implemented here
    // This could verify against a backend service
    
    if (ConnectedPlayers.Contains(PlayerID))
    {
        ConnectedPlayers[PlayerID].ConnectionState = EMingConnectionState::Authenticated;
        return true;
    }
    
    return false;
}

void UMingNetworkManager::KickPlayer(int32 PlayerID, const FString& Reason)
{
    if (ConnectedPlayers.Contains(PlayerID) && IsHost())
    {
        UE_LOG(LogTemp, Log, TEXT("Kicking player %d: %s"), PlayerID, *Reason);
        
        // Send kick notification
        // Disconnect player
        UnregisterPlayer(PlayerID);
    }
}

void UMingNetworkManager::BanPlayer(int32 PlayerID, int32 BanDurationMinutes, const FString& Reason)
{
    if (IsHost())
    {
        BannedPlayers.Add(PlayerID);
        KickPlayer(PlayerID, Reason);
        
        UE_LOG(LogTemp, Log, TEXT("Banning player %d for %d minutes: %s"), 
            PlayerID, BanDurationMinutes, *Reason);
    }
}

void UMingNetworkManager::UpdateNetworkStats(float DeltaTime)
{
    // Update network statistics
    // This would calculate bandwidth, packet loss, etc.
    
    NetworkStats.UptimeSeconds += DeltaTime;
    
    // Calculate average ping
    if (ConnectedPlayers.Num() > 0)
    {
        float TotalPing = 0.0f;
        float MinPing = MAX_FLT;
        float MaxPing = 0.0f;
        
        for (const auto& Pair : ConnectedPlayers)
        {
            float Ping = Pair.Value.PingMs;
            TotalPing += Ping;
            MinPing = FMath::Min(MinPing, Ping);
            MaxPing = FMath::Max(MaxPing, Ping);
        }
        
        NetworkStats.AveragePing = TotalPing / ConnectedPlayers.Num();
        NetworkStats.MinPing = MinPing;
        NetworkStats.MaxPing = MaxPing;
    }
}

void UMingNetworkManager::ProcessIncomingMessages()
{
    // Process incoming network messages
    // This would be called regularly to handle received messages
}

void UMingNetworkManager::HandlePlayerConnection(int32 PlayerID)
{
    if (BannedPlayers.Contains(PlayerID))
    {
        KickPlayer(PlayerID, TEXT("You are banned from this server"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Player %d connected"), PlayerID);
}

void UMingNetworkManager::HandlePlayerDisconnection(int32 PlayerID)
{
    UnregisterPlayer(PlayerID);
}

bool UMingNetworkManager::ValidateMessage(const FMingNetworkMessage& Message) const
{
    // Validate message integrity
    if (Message.SenderID < 0)
    {
        return false;
    }
    
    if (Message.Payload.Num() > 65536) // Max 64KB payload
    {
        return false;
    }
    
    return true;
}

void UMingNetworkManager::CompressBatch(FMingSyncBatch& Batch)
{
    Batch.bCompressed = true;
    // Compression logic would be implemented here
}

void UMingNetworkManager::DecompressBatch(FMingSyncBatch& Batch)
{
    // Decompression logic would be implemented here
    Batch.bCompressed = false;
}

void UMingNetworkManager::SendHeartbeat()
{
    // Send heartbeat to keep connection alive
    if (IsConnected())
    {
        // Heartbeat sending logic would be implemented here
    }
}

void UMingNetworkManager::CheckConnectionHealth()
{
    // Check connection health and detect timeouts
    // Disconnect players that haven't responded in a while
}

void UMingNetworkManager::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, 
    ENetworkFailure::Type FailureType, const FString& ErrorString)
{
    UE_LOG(LogTemp, Error, TEXT("Network failure: %d - %s"), static_cast<int32>(FailureType), *ErrorString);
    
    OnNetworkError.Broadcast(static_cast<int32>(FailureType), ErrorString);
    
    SetConnectionState(EMingConnectionState::Error);
}

void UMingNetworkManager::OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, 
    const FString& ErrorString)
{
    UE_LOG(LogTemp, Error, TEXT("Travel failure: %d - %s"), static_cast<int32>(FailureType), *ErrorString);
    
    OnNetworkError.Broadcast(static_cast<int32>(FailureType), ErrorString);
}
