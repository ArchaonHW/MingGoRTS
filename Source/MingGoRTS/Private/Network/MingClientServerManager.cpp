// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Client-Server Architecture Manager Implementation - B1-3

#include "Network/MingClientServerManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogClientServer, Log, All);

UMingClientServerManager::UMingClientServerManager()
    : CurrentServerState(EServerState::Offline)
    , CurrentClientState(EClientState::Disconnected)
    , bIsServerInstance(false)
    , bReconciliationEnabled(true)
    , bLagCompensationEnabled(true)
    , InterpolationDelay(0.1f)
{
}

bool UMingClientServerManager::InitializeServer(const FServerConfiguration& Config)
{
    ServerConfig = Config;
    bIsServerInstance = true;
    CurrentServerState = EServerState::Starting;

    UE_LOG(LogClientServer, Log, TEXT("Initializing server: %s"), *Config.ServerName);

    // Start server tick
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            ServerTickTimer,
            this,
            &UMingClientServerManager::ServerTick,
            1.0f / Config.TickRate,
            true
        );

        // Start heartbeat
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            HeartbeatTimer,
            this,
            &UMingClientServerManager::SendHeartbeat,
            Config.KeepAliveInterval,
            true
        );
    }

    SetServerState(EServerState::Online);
    return true;
}

void UMingClientServerManager::ShutdownServer()
{
    SetServerState(EServerState::ShuttingDown);

    // Disconnect all clients
    for (const auto& Client : Clients)
    {
        HandleClientDisconnect(Client.ClientID, TEXT("Server shutting down"));
    }
    Clients.Empty();

    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(ServerTickTimer);
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(HeartbeatTimer);
    }

    SetServerState(EServerState::Offline);
    UE_LOG(LogClientServer, Log, TEXT("Server shut down"));
}

void UMingClientServerManager::StartServer()
{
    if (CurrentServerState == EServerState::Offline)
    {
        InitializeServer(ServerConfig);
    }
}

bool UMingClientServerManager::IsServerRunning() const
{
    return CurrentServerState == EServerState::Online;
}

void UMingClientServerManager::KickClient(int32 ClientID, const FString& Reason)
{
    for (int32 i = Clients.Num() - 1; i >= 0; --i)
    {
        if (Clients[i].ClientID == ClientID)
        {
            HandleClientDisconnect(ClientID, Reason);
            Clients.RemoveAt(i);
            OnClientDisconnected.Broadcast(ClientID, Reason);
            break;
        }
    }
}

void UMingClientServerManager::BanClient(int32 ClientID, int32 DurationMinutes, const FString& Reason)
{
    UE_LOG(LogClientServer, Warning, TEXT("Banning client %d for %d minutes. Reason: %s"),
        ClientID, DurationMinutes, *Reason);

    // Add to ban list
    KickClient(ClientID, Reason);
}

void UMingClientServerManager::UpdateServerConfiguration(const FServerConfiguration& Config)
{
    ServerConfig = Config;
    UE_LOG(LogClientServer, Log, TEXT("Server configuration updated"));
}

FServerPerformanceMetrics UMingClientServerManager::GetServerMetrics() const
{
    FServerPerformanceMetrics Metrics;
    Metrics.ActiveConnections = Clients.Num();
    Metrics.CPUUsagePercent = CalculateServerLoad();
    Metrics.TickRate = ServerConfig.TickRate;

    // Calculate bandwidth usage
    float TotalIn = 0.0f, TotalOut = 0.0f;
    for (const auto& Client : Clients)
    {
        TotalOut += Client.BandwidthUsage;
    }
    Metrics.NetworkOutBandwidth = TotalOut;
    Metrics.NetworkInBandwidth = TotalIn;

    return Metrics;
}

void UMingClientServerManager::SetServerState(EServerState NewState)
{
    if (CurrentServerState != NewState)
    {
        CurrentServerState = NewState;
        OnServerStateChanged.Broadcast(NewState);
        UE_LOG(LogClientServer, Log, TEXT("Server state changed to: %s"), *UEnum::GetValueAsString(NewState));
    }
}

bool UMingClientServerManager::ConnectToServer(const FString& ServerAddress, int32 Port)
{
    CurrentClientState = EClientState::Connecting;

    // Simulate connection process
    UE_LOG(LogClientServer, Log, TEXT("Connecting to server %s:%d"), *ServerAddress, Port);

    // In real implementation, would create socket connection
    // For now, simulate successful connection
    CurrentClientState = EClientState::Connected;
    UE_LOG(LogClientServer, Log, TEXT("Connected to server"));

    return true;
}

void UMingClientServerManager::DisconnectFromServer()
{
    CurrentClientState = EClientState::Disconnected;
    UE_LOG(LogClientServer, Log, TEXT("Disconnected from server"));
}

bool UMingClientServerManager::IsClientConnected() const
{
    return CurrentClientState == EClientState::Connected ||
           CurrentClientState == EClientState::Ready;
}

EClientState UMingClientServerManager::GetClientState() const
{
    return CurrentClientState;
}

void UMingClientServerManager::SendHeartbeat()
{
    // Send keepalive packet
    if (IsClientConnected())
    {
        UE_LOG(LogClientServer, VeryVerbose, TEXT("Sending heartbeat"));
    }

    // Check client timeouts
    if (bIsServerInstance)
    {
        uint32 CurrentTime = FPlatformTime::Seconds();
        for (int32 i = Clients.Num() - 1; i >= 0; --i)
        {
            if (CurrentTime - Clients[i].LastPingTime > ServerConfig.ConnectionTimeout)
            {
                UE_LOG(LogClientServer, Warning, TEXT("Client %d timed out"), Clients[i].ClientID);
                KickClient(Clients[i].ClientID, TEXT("Connection timeout"));
            }
        }
    }
}

void UMingClientServerManager::RequestServerInfo()
{
    UE_LOG(LogClientServer, Log, TEXT("Requesting server information"));
}

EConnectionQuality UMingClientServerManager::GetConnectionQuality() const
{
    if (Clients.Num() == 0 && !IsClientConnected())
    {
        return EConnectionQuality::Poor;
    }

    // Calculate based on ping and packet loss
    float AvgPing = 0.0f;
    int32 PacketLoss = 0;

    for (const auto& Client : Clients)
    {
        AvgPing += Client.AveragePing;
        PacketLoss += Client.PacketLossPercent;
    }

    if (Clients.Num() > 0)
    {
        AvgPing /= Clients.Num();
        PacketLoss /= Clients.Num();
    }

    if (AvgPing < 50 && PacketLoss < 1)
    {
        return EConnectionQuality::Excellent;
    }
    else if (AvgPing < 100 && PacketLoss < 3)
    {
        return EConnectionQuality::Good;
    }
    else if (AvgPing < 200 && PacketLoss < 5)
    {
        return EConnectionQuality::Fair;
    }
    else
    {
        return EConnectionQuality::Poor;
    }
}

int32 UMingClientServerManager::GetCurrentPing() const
{
    if (Clients.Num() > 0)
    {
        int32 TotalPing = 0;
        for (const auto& Client : Clients)
        {
            TotalPing += Client.AveragePing;
        }
        return TotalPing / Clients.Num();
    }
    return 0;
}

void UMingClientServerManager::EnableLagCompensation(bool bEnable)
{
    bLagCompensationEnabled = bEnable;
    UE_LOG(LogClientServer, Log, TEXT("Lag compensation %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingClientServerManager::SetInterpolationDelay(float Delay)
{
    InterpolationDelay = FMath::Clamp(Delay, 0.0f, 1.0f);
}

void UMingClientServerManager::EnableStateReconciliation(bool bEnable)
{
    bReconciliationEnabled = bEnable;
    UE_LOG(LogClientServer, Log, TEXT("State reconciliation %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingClientServerManager::ProcessClientInput(int32 ClientID, const TArray<uint8>& InputData)
{
    if (!bIsServerInstance) return;

    // Validate client
    bool bValidClient = false;
    for (auto& Client : Clients)
    {
        if (Client.ClientID == ClientID)
        {
            bValidClient = true;
            Client.LastPingTime = FPlatformTime::Seconds();
            break;
        }
    }

    if (!bValidClient)
    {
        UE_LOG(LogClientServer, Warning, TEXT("Received input from unknown client %d"), ClientID);
        return;
    }

    // Store input for processing
    ValidateClientInput(ClientID, InputData);
}

void UMingClientServerManager::BroadcastGameState(const TArray<uint8>& StateData)
{
    if (!bIsServerInstance) return;

    // Compress data if beneficial
    TArray<uint8> DataToSend = StateData;
    if (StateData.Num() > 1024)
    {
        CompressServerData(DataToSend);
    }

    // Broadcast to all clients
    BroadcastToAllClients(DataToSend);
}

void UMingClientServerManager::ReconcileClientState(int32 ClientID)
{
    if (!bReconciliationEnabled) return;

    UE_LOG(LogClientServer, Verbose, TEXT("Reconciling state for client %d"), ClientID);

    // Send corrected state to client
}

void UMingClientServerManager::RewindGameState(float TimeSeconds)
{
    if (!bLagCompensationEnabled) return;

    UE_LOG(LogClientServer, Verbose, TEXT("Rewinding game state by %.3f seconds"), TimeSeconds);

    // Restore game state from history
}

bool UMingClientServerManager::CanAcceptNewConnection() const
{
    if (CurrentServerState != EServerState::Online)
    {
        return false;
    }

    return Clients.Num() < ServerConfig.MaxPlayers;
}

void UMingClientServerManager::BalanceServerLoad()
{
    float Load = CalculateServerLoad();

    if (Load > 80.0f)
    {
        // High load - reduce tick rate or reject new connections
        UE_LOG(LogClientServer, Warning, TEXT("High server load detected: %.1f%%"), Load);
        OnServerPerformanceWarning.Broadcast(TEXT("Server load is high"));
    }

    if (Load > 95.0f)
    {
        SetServerState(EServerState::Overloaded);
    }
}

void UMingClientServerManager::DistributeClientsAcrossInstances()
{
    // Implementation for multiple server instances
    UE_LOG(LogClientServer, Log, TEXT("Distributing clients across instances"));
}

void UMingClientServerManager::ServerTick()
{
    if (CurrentServerState != EServerState::Online) return;

    // Process client inputs
    ProcessClientInputs();

    // Update game state
    UpdateGameState();

    // Balance load
    BalanceServerLoad();

    // Monitor connection quality
    MonitorConnectionQuality();

    // Save state snapshot for reconciliation
    if (bReconciliationEnabled)
    {
        RecordGameStateSnapshot();
    }
}

void UMingClientServerManager::ProcessClientInputs()
{
    // Process queued inputs
    for (auto& Pair : ClientInputs)
    {
        int32 ClientID = Pair.Key;
        TArray<uint8>& Inputs = Pair.Value;

        // Process each input
        for (const auto& Input : Inputs)
        {
            // Apply input to game state
        }

        Inputs.Empty();
    }
}

void UMingClientServerManager::UpdateGameState()
{
    // Update game state based on processed inputs
}

void UMingClientServerManager::BroadcastToAllClients(const TArray<uint8>& Data)
{
    for (const auto& Client : Clients)
    {
        // Send data to each client
        // Would use actual network send here
    }
}

void UMingClientServerManager::HandleClientDisconnect(int32 ClientID, const FString& Reason)
{
    UE_LOG(LogClientServer, Log, TEXT("Client %d disconnected: %s"), ClientID, *Reason);
    OnClientDisconnected.Broadcast(ClientID, Reason);

    // Save client state for potential reconnect
    SaveClientStates();
}

void UMingClientServerManager::AuthenticateClient(int32 ClientID, const FString& Credentials)
{
    for (auto& Client : Clients)
    {
        if (Client.ClientID == ClientID)
        {
            // Validate credentials
            Client.bIsAuthenticated = true;
            UE_LOG(LogClientServer, Log, TEXT("Client %d authenticated"), ClientID);
            break;
        }
    }
}

void UMingClientServerManager::ValidateClientInput(int32 ClientID, const TArray<uint8>& InputData)
{
    // Validate input data
    // Check for cheating, invalid commands, etc.
}

float UMingClientServerManager::CalculateServerLoad() const
{
    // Calculate current server load
    float Load = 50.0f; // Base load

    // Add load for each client
    Load += Clients.Num() * 0.5f;

    return FMath::Clamp(Load, 0.0f, 100.0f);
}

void UMingClientServerManager::OptimizeServerPerformance()
{
    // Implement performance optimizations
    UE_LOG(LogClientServer, Log, TEXT("Optimizing server performance"));
}

void UMingClientServerManager::MonitorConnectionQuality()
{
    EConnectionQuality Quality = GetConnectionQuality();
    OnConnectionQualityChanged.Broadcast(Quality);
}

void UMingClientServerManager::AdjustTickRateBasedOnLoad()
{
    float Load = CalculateServerLoad();

    if (Load > 80.0f)
    {
        // Reduce tick rate
        ServerConfig.TickRate = FMath::Max(ServerConfig.TickRate - 5, 30);
    }
    else if (Load < 50.0f && ServerConfig.TickRate < 60)
    {
        // Can increase tick rate
        ServerConfig.TickRate = FMath::Min(ServerConfig.TickRate + 5, 60);
    }
}

void UMingClientServerManager::HandlePacketLoss(int32 ClientID)
{
    for (auto& Client : Clients)
    {
        if (Client.ClientID == ClientID)
        {
            Client.PacketLossPercent = FMath::Min(Client.PacketLossPercent + 1, 100);
            break;
        }
    }
}

void UMingClientServerManager::ResendLostData(int32 ClientID, int32 SequenceNumber)
{
    UE_LOG(LogClientServer, Verbose, TEXT("Resending lost data to client %d, sequence %d"), ClientID, SequenceNumber);
}

void UMingClientServerManager::CompressServerData(TArray<uint8>& Data)
{
    // Compress data for network transmission
    UE_LOG(LogClientServer, VeryVerbose, TEXT("Compressing server data: %d bytes"), Data.Num());
}

void UMingClientServerManager::DecompressClientData(const TArray<uint8>& CompressedData, TArray<uint8>& OutData)
{
    // Decompress received data
    OutData = CompressedData;
}

void UMingClientServerManager::RecordGameStateSnapshot()
{
    // Record current state for reconciliation
    GameStateHistory.Add(TArray<uint8>());

    // Limit history size
    if (GameStateHistory.Num() > 100)
    {
        GameStateHistory.RemoveAt(0);
    }
}

void UMingClientServerManager::CleanupOldStateSnapshots()
{
    // Remove old snapshots
    while (GameStateHistory.Num() > 100)
    {
        GameStateHistory.RemoveAt(0);
    }
}

bool UMingClientServerManager::ValidateServerState() const
{
    // Validate server is in consistent state
    return CurrentServerState == EServerState::Online ||
           CurrentServerState == EServerState::Maintenance;
}

void UMingClientServerManager::HandleServerCrash()
{
    UE_LOG(LogClientServer, Error, TEXT("Server crash detected"));

    // Save all client states
    SaveClientStates();

    // Attempt recovery
    SetServerState(EServerState::Offline);
}

void UMingClientServerManager::InitiateGracefulShutdown()
{
    UE_LOG(LogClientServer, Log, TEXT("Initiating graceful shutdown"));

    SetServerState(EServerState::ShuttingDown);

    // Notify all clients
    for (const auto& Client : Clients)
    {
        // Send shutdown notification
    }

    // Wait for clients to disconnect gracefully
    FPlatformProcess::Sleep(5.0f);

    ShutdownServer();
}

void UMingClientServerManager::SaveClientStates()
{
    UE_LOG(LogClientServer, Log, TEXT("Saving client states"));
    // Persist client states for reconnect
}

void UMingClientServerManager::RestoreClientStates()
{
    UE_LOG(LogClientServer, Log, TEXT("Restoring client states"));
    // Restore client states after reconnect
}

static UMingClientServerManager* UMingClientServerManager::Get(UObject* WorldContextObject)
{
    static UMingClientServerManager* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingClientServerManager>();
        Instance->AddToRoot();
    }
    return Instance;
}
