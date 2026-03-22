// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Client-Server Architecture Manager - B1-3
// Stabilizes and manages client-server communication

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "MingClientServerManager.generated.h"

UENUM(BlueprintType)
enum class EServerState: uint8 {
    Offline = 0, UMETA(DisplayName = "Offline"),
    Starting, UMETA(DisplayName = "Starting"),
    Online, UMETA(DisplayName = "Online"),
    Maintenance, UMETA(DisplayName = "Maintenance"),
    Overloaded, UMETA(DisplayName = "Overloaded"),
    ShuttingDown UMETA(DisplayName = "Shutting Down")
};

UENUM(BlueprintType)
enum class EClientState: uint8 {
    Disconnected = 0, UMETA(DisplayName = "Disconnected"),
    Connecting, UMETA(DisplayName = "Connecting"),
    Connected, UMETA(DisplayName = "Connected"),
    Authenticating, UMETA(DisplayName = "Authenticating"),
    Ready, UMETA(DisplayName = "Ready"),
    DisconnectedByServer, UMETA(DisplayName = "Disconnected By Server"),
    ConnectionLost UMETA(DisplayName = "Connection Lost")
};

UENUM(BlueprintType)
enum class EConnectionQuality: uint8 {
    Excellent = 0, UMETA(DisplayName = "Excellent"),
    Good, UMETA(DisplayName = "Good"),
    Fair, UMETA(DisplayName = "Fair"),
    Poor, UMETA(DisplayName = "Poor"),
    Critical UMETA(DisplayName = "Critical")
};

USTRUCT(BlueprintType)
struct FServerConfiguration
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    int32 MaxPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    int32 MaxSpectators;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    int32 TickRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    int32 NetUpdateFrequency;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    float ConnectionTimeout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    float KeepAliveInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    bool bEnableReconciliation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    bool bUseLagCompensation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    int32 MaxBandwidthPerClient;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    FString ServerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Server Config")
    FString ServerDescription;

    FServerConfiguration()
        : MaxPlayers(64)
        , MaxSpectators(10)
        , TickRate(60)
        , NetUpdateFrequency(30)
        , ConnectionTimeout(30.0f)
        , KeepAliveInterval(1.0f)
        , bEnableReconciliation(true)
        , bUseLagCompensation(true)
        , MaxBandwidthPerClient(2048)
        , ServerName(TEXT("MingGoRTS Server"))
        , ServerDescription(TEXT("Default Server"))
    {}
};

USTRUCT(BlueprintType)
struct FClientConnectionData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ClientID;

    UPROPERTY()
    FString ClientAddress;

    UPROPERTY()
    EClientState ClientState;

    UPROPERTY()
    float ConnectionTime;

    UPROPERTY()
    float LastPingTime;

    UPROPERTY()
    int32 AveragePing;

    UPROPERTY()
    int32 PacketLossPercent;

    UPROPERTY()
    EConnectionQuality ConnectionQuality;

    UPROPERTY()
    int32 BandwidthUsage;

    UPROPERTY()
    bool bIsAuthenticated;

    UPROPERTY()
    FString PlayerName;

    FClientConnectionData()
        : ClientID(0)
        , ClientState(EClientState::Disconnected)
        , ConnectionTime(0.0f)
        , LastPingTime(0.0f)
        , AveragePing(0)
        , PacketLossPercent(0)
        , ConnectionQuality(EConnectionQuality::Good)
        , BandwidthUsage(0)
        , bIsAuthenticated(false)
    {}
};

USTRUCT(BlueprintType)
struct FServerPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY()
    float CPUUsagePercent;

    UPROPERTY()
    float MemoryUsagePercent;

    UPROPERTY()
    float NetworkInBandwidth;

    UPROPERTY()
    float NetworkOutBandwidth;

    UPROPERTY()
    int32 ActiveConnections;

    UPROPERTY()
    float FrameTime;

    UPROPERTY()
    int32 TickRate;

    FServerPerformanceMetrics()
        : CPUUsagePercent(0.0f)
        , MemoryUsagePercent(0.0f)
        , NetworkInBandwidth(0.0f)
        , NetworkOutBandwidth(0.0f)
        , ActiveConnections(0)
        , FrameTime(16.67f)
        , TickRate(60)
    {}
};






/**
 * Client-Server Architecture Manager
 * Manages stable client-server communication
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingClientServerManager : public UObject
{
    GENERATED_BODY()

public:
    UMingClientServerManager(};
    // Server-side functions
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    bool InitializeServer(const FServerConfiguration& Config};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void ShutdownServer(};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void StartServer(};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    bool IsServerRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void KickClient(int32 ClientID, const FString& Reason};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void BanClient(int32 ClientID, int32 DurationMinutes, const FString& Reason};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void UpdateServerConfiguration(const FServerConfiguration& Config};
    UFUNCTION(BlueprintCallable, Category = "Server Management")
    FServerPerformanceMetrics GetServerMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "Server Management")
    void SetServerState(EServerState NewState};
    // Client-side functions
    UFUNCTION(BlueprintCallable, Category = "Client Management")
    bool ConnectToServer(const FString& ServerAddress, int32 Port};
    UFUNCTION(BlueprintCallable, Category = "Client Management")
    void DisconnectFromServer(};
    UFUNCTION(BlueprintCallable, Category = "Client Management")
    bool IsClientConnected() const;

    UFUNCTION(BlueprintCallable, Category = "Client Management")
    EClientState GetClientState() const;

    UFUNCTION(BlueprintCallable, Category = "Client Management")
    void SendHeartbeat(};
    UFUNCTION(BlueprintCallable, Category = "Client Management")
    void RequestServerInfo(};
    // Connection quality management
    UFUNCTION(BlueprintCallable, Category = "Connection Quality")
    EConnectionQuality GetConnectionQuality() const;

    UFUNCTION(BlueprintCallable, Category = "Connection Quality")
    int32 GetCurrentPing() const;

    UFUNCTION(BlueprintCallable, Category = "Connection Quality")
    void EnableLagCompensation(bool bEnable};
    UFUNCTION(BlueprintCallable, Category = "Connection Quality")
    void SetInterpolationDelay(float Delay};
    // State reconciliation
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void EnableStateReconciliation(bool bEnable};
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void ProcessClientInput(int32 ClientID, const TArray<uint8>& InputData};
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void BroadcastGameState(const TArray<uint8>& StateData};
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void ReconcileClientState(int32 ClientID};
    UFUNCTION(BlueprintCallable, Category = "State Management")
    void RewindGameState(float TimeSeconds};
    // Load balancing
    UFUNCTION(BlueprintCallable, Category = "Load Balancing")
    bool CanAcceptNewConnection() const;

    UFUNCTION(BlueprintCallable, Category = "Load Balancing")
    void BalanceServerLoad(};
    UFUNCTION(BlueprintCallable, Category = "Load Balancing")
    void DistributeClientsAcrossInstances(};
    UFUNCTION(BlueprintPure, Category = "Server Info")
    int32 GetConnectedClientCount() const { return Clients.Num(); }

    UFUNCTION(BlueprintPure, Category = "Server Info")
    EServerState GetServerState() const { return CurrentServerState; }

    UFUNCTION(BlueprintPure, Category = "Server Info")
    TArray<FClientConnectionData> GetAllClients() const { return Clients; }

    UPROPERTY(BlueprintAssignable, Category = "Server Events")
    FOnServerStateChanged OnServerStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Server Events")
    FOnClientConnected OnClientConnected;

    UPROPERTY(BlueprintAssignable, Category = "Server Events")
    FOnClientDisconnected OnClientDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Server Events")
    FOnConnectionQualityChanged OnConnectionQualityChanged;

    UPROPERTY(BlueprintAssignable, Category = "Server Events")
    FOnServerPerformanceWarning OnServerPerformanceWarning;

protected:
    UPROPERTY()
    FServerConfiguration ServerConfig;

    UPROPERTY()
    EServerState CurrentServerState;

    UPROPERTY()
    EClientState CurrentClientState;

    UPROPERTY()
    TArray<FClientConnectionData> Clients;

    UPROPERTY()
    FClientConnectionData LocalClientData;

    UPROPERTY()
    bool bIsServerInstance;

    UPROPERTY()
    bool bReconciliationEnabled;

    UPROPERTY()
    bool bLagCompensationEnabled;

    UPROPERTY()
    float InterpolationDelay;

    UPROPERTY()
    TArray<TArray<uint8>> ClientInputs;

    UPROPERTY()
    TArray<TArray<uint8>> GameStateHistory;

    UPROPERTY()
    FTimerHandle ServerTickTimer;

    UPROPERTY()
    FTimerHandle HeartbeatTimer;

    void ServerTick(};
    void ProcessClientInputs(};
    void UpdateGameState(};
    void BroadcastToAllClients(const TArray<uint8>& Data};
    void HandleClientDisconnect(int32 ClientID, const FString& Reason};
    void AuthenticateClient(int32 ClientID, const FString& Credentials};
    void ValidateClientInput(int32 ClientID, const TArray<uint8>& InputData};
    float CalculateServerLoad() const;
    void OptimizeServerPerformance(};
    void MonitorConnectionQuality(};
    void AdjustTickRateBasedOnLoad(};
    void HandlePacketLoss(int32 ClientID};
    void ResendLostData(int32 ClientID, int32 SequenceNumber};
    void CompressServerData(TArray<uint8>& Data};
    void DecompressClientData(const TArray<uint8>& CompressedData, TArray<uint8>& OutData};
    void RecordGameStateSnapshot(};
    void CleanupOldStateSnapshots(};
    bool ValidateServerState() const;
    void HandleServerCrash(};
    void InitiateGracefulShutdown(};
    void SaveClientStates(};
    void RestoreClientStates(};
    static UMingClientServerManager* Get(UObject* WorldContextObject};
};