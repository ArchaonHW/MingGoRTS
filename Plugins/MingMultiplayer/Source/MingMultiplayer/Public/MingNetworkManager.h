#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingMultiplayerTypes.h"
#include "MingNetworkManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerConnected, int32, PlayerID, const FMingPlayerNetworkInfo&, PlayerInfo};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDisconnected, int32, PlayerID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConnectionStateChanged, EMingConnectionState, OldState, EMingConnectionState, NewState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNetworkError, int32, ErrorCode, const FString&, ErrorMessage};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionCreated, const FString&, SessionID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionJoined, const FString&, SessionID};

UCLASS(ClassGroup = (Multiplayer, Network))
class MINGMULTIPLAYER_API UMingNetworkManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingNetworkManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Network")
    void InitializeNetwork();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void ShutdownNetwork();

    // Connection Management
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer(const FString& ServerAddress, int32 Port};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void Disconnect();

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool CreateSession(const FMingNetworkSessionInfo& SessionInfo};

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool JoinSession(const FString& SessionID, const FString& Password};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void LeaveSession();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void DiscoverSessions();

    // Server Management
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool StartListenServer(int32 Port, int32 MaxPlayers};

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool StartDedicatedServer(int32 Port};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void StopServer();

    // Player Management
    UFUNCTION(BlueprintCallable, Category = "Network")
    void RegisterPlayer(int32 PlayerID, const FMingPlayerNetworkInfo& PlayerInfo};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void UnregisterPlayer(int32 PlayerID};

    UFUNCTION(BlueprintPure, Category = "Network")
    FMingPlayerNetworkInfo GetPlayerInfo(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "Network")
    TArray<FMingPlayerNetworkInfo> GetAllPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetLocalPlayerID() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsHost() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsConnected() const;

    // Message Sending
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool SendMessage(int32 TargetPlayerID, int32 MessageType, const TArray<uint8>& Data, 
        EMingSyncPriority Priority = EMingSyncPriority::Normal, bool bReliable = true};

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool BroadcastMessage(int32 MessageType, const TArray<uint8>& Data, 
        EMingSyncPriority Priority = EMingSyncPriority::Normal};

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool SendRPC(class UObject* TargetObject, const FString& FunctionName, const TArray<uint8>& Parameters};

    // Network State
    UFUNCTION(BlueprintPure, Category = "Network")
    EMingNetworkRole GetNetworkRole() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    EMingConnectionState GetConnectionState() const;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetConnectionState(EMingConnectionState NewState};

    UFUNCTION(BlueprintPure, Category = "Network")
    FMingNetworkStats GetNetworkStats() const;

    // Latency Management
    UFUNCTION(BlueprintCallable, Category = "Network")
    float GetPlayerPing(int32 PlayerID) const;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetTickRate(int32 TickRate};

    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetTickRate() const;

    // Security
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetServerPassword(const FString& Password};

    UFUNCTION(BlueprintCallable, Category = "Network")
    bool AuthenticatePlayer(int32 PlayerID, const FString& AuthToken};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void KickPlayer(int32 PlayerID, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void BanPlayer(int32 PlayerID, int32 BanDurationMinutes, const FString& Reason};

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerConnected OnPlayerConnected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerDisconnected OnPlayerDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnConnectionStateChanged OnConnectionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnNetworkError OnNetworkError;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionCreated OnSessionCreated;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSessionJoined OnSessionJoined;

protected:
    UPROPERTY()
    EMingNetworkRole CurrentRole;

    UPROPERTY()
    EMingConnectionState CurrentState;

    UPROPERTY()
    int32 LocalPlayerID;

    UPROPERTY()
    TMap<int32, FMingPlayerNetworkInfo> ConnectedPlayers;

    UPROPERTY()
    FMingNetworkSessionInfo CurrentSession;

    UPROPERTY()
    FMingNetworkStats NetworkStats;

    UPROPERTY()
    int32 CurrentTickRate;

    UPROPERTY()
    FString ServerPassword;

    UPROPERTY()
    TSet<int32> BannedPlayers;

    UPROPERTY()
    float LastNetworkUpdateTime;

    UPROPERTY()
    float NetworkUpdateInterval;

    // Internal Functions
    void UpdateNetworkStats(float DeltaTime};
    void ProcessIncomingMessages();
    void HandlePlayerConnection(int32 PlayerID};
    void HandlePlayerDisconnection(int32 PlayerID};
    bool ValidateMessage(const FMingNetworkMessage& Message) const;
    void CompressBatch(FMingSyncBatch& Batch};
    void DecompressBatch(FMingSyncBatch& Batch};
    void SendHeartbeat();
    void CheckConnectionHealth();
    void CleanupDisconnectedPlayers();

    // Delegates
    UFUNCTION()
    void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString};

    UFUNCTION()
    void OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorString};
};

