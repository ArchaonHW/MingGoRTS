#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingNetworkManager.generated.h"

// Forward declarations
class UMingPersonalManager;
class UMingLobbySystem;
class UMingRelationshipReplication;
class UMingReputationReplication;

// Network connection state
UENUM(BlueprintType)
enum class EMingNetworkConnectionState : uint8
{
    Disconnected      UMETA(DisplayName = "Disconnected"),
    Connecting        UMETA(DisplayName = "Connecting"),
    Connected         UMETA(DisplayName = "Connected"),
    Disconnecting     UMETA(DisplayName = "Disconnecting"),
    Reconnecting      UMETA(DisplayName = "Reconnecting"),
    Error             UMETA(DisplayName = "Error")
};

// Network role
UENUM(BlueprintType)
enum class EMingNetworkRole : uint8
{
    None              UMETA(DisplayName = "None"),
    Host              UMETA(DisplayName = "Host"),
    Client            UMETA(DisplayName = "Client"),
    ListenServer      UMETA(DisplayName = "Listen Server")
};

// Network configuration
USTRUCT(BlueprintType)
struct FMingNetworkConfig
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    FString ServerName;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    int32 Port;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    FString MapName;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    bool bUseLAN;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    bool bIsPrivate;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    FString Password;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    FString GameMode;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    int32 TickRate;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    bool bEnableVoiceChat;

    UPROPERTY(BlueprintReadWrite, Category = "Network")
    bool bAllowJoinInProgress;

    FMingNetworkConfig()
        : ServerName(TEXT("MingGoRTS Server"))
        , MaxPlayers(4)
        , Port(7777)
        , MapName(TEXT("MainMap"))
        , bUseLAN(false)
        , bIsPrivate(false)
        , Password(TEXT(""))
        , GameMode(TEXT("MingRTSGameMode"))
        , TickRate(30)
        , bEnableVoiceChat(true)
        , bAllowJoinInProgress(true)
    {}
};

// Player network info
USTRUCT(BlueprintType)
struct FMingPlayerNetworkInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 PlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    bool bIsHost;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    bool bIsReady;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float Ping;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    FString IPAddress;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 TeamID;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    bool bIsSpectator;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 PlayerScore;

    FMingPlayerNetworkInfo()
        : PlayerID(-1)
        , PlayerName(TEXT(""))
        , bIsHost(false)
        , bIsReady(false)
        , Ping(0.0f)
        , IPAddress(TEXT(""))
        , TeamID(0)
        , bIsSpectator(false)
        , PlayerScore(0)
    {}
};

// Network statistics
USTRUCT(BlueprintType)
struct FMingNetworkStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float AveragePing;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float PacketLossRate;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 BytesSentPerSecond;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 BytesReceivedPerSecond;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 ConnectedPlayers;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float ServerFrameTime;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 ReplicatedActors;

    FMingNetworkStats()
        : AveragePing(0.0f)
        , PacketLossRate(0.0f)
        , BytesSentPerSecond(0)
        , BytesReceivedPerSecond(0)
        , ConnectedPlayers(0)
        , ServerFrameTime(0.0f)
        , ReplicatedActors(0)
    {}
};

// Chat message
USTRUCT(BlueprintType)
struct FMingChatMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 SenderPlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    FString SenderName;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    bool bIsSystemMessage;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    bool bIsPrivate;

    UPROPERTY(BlueprintReadOnly, Category = "Network")
    int32 TargetPlayerID;

    FMingChatMessage()
        : SenderPlayerID(-1)
        , SenderName(TEXT(""))
        , Message(TEXT(""))
        , Timestamp(0.0f)
        , bIsSystemMessage(false)
        , bIsPrivate(false)
        , TargetPlayerID(-1)
    {}
};

// Network error types
UENUM(BlueprintType)
enum class EMingNetworkError : uint8
{
    None                      UMETA(DisplayName = "None"),
    ConnectionFailed          UMETA(DisplayName = "Connection Failed"),
    ConnectionLost            UMETA(DisplayName = "Connection Lost"),
    ServerFull                UMETA(DisplayName = "Server Full"),
    InvalidPassword           UMETA(DisplayName = "Invalid Password"),
    VersionMismatch           UMETA(DisplayName = "Version Mismatch"),
    Timeout                   UMETA(DisplayName = "Timeout"),
    HostMigrationFailed       UMETA(DisplayName = "Host Migration Failed"),
    NatPunchthroughFailed     UMETA(DisplayName = "NAT Punchthrough Failed"),
    ServerShutdown            UMETA(DisplayName = "Server Shutdown"),
    Kicked                    UMETA(DisplayName = "Kicked"),
    Banned                    UMETA(DisplayName = "Banned"),
    GenericError              UMETA(DisplayName = "Generic Error")
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkConnectionStateChanged, EMingNetworkConnectionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, FMingPlayerNetworkInfo, PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeft, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerReadyChanged, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatMessageReceived, FMingChatMessage, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkError, EMingNetworkError, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHostMigrated, int32, NewHostPlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameEnded);

/**
 * Á∂≤Áµ°ÁÆ°Á??? * ?ïÁ?Â§ö‰∫∫?äÊà≤?ÑÁ∂≤Áµ°ÈÄ?é•?ÅÁé©ÂÆ∂ÁÆ°?ÜÂ??∏Ê??åÊ≠•
 */
UCLASS(ClassGroup = (MingGoRTS), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingNetworkManager : public UObject
{
    GENERATED_BODY()

public:
    UMingNetworkManager();

    // Initialize/Shutdown
    UFUNCTION(BlueprintCallable, Category = "Network")
    void Initialize();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void Shutdown();

    // Server creation
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool CreateServer(const FMingNetworkConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Network")
    void CloseServer();

    // Client connection
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer(const FString& ServerAddress, int32 Port, const FString& Password = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Network")
    void Disconnect();

    // Connection state
    UFUNCTION(BlueprintPure, Category = "Network")
    EMingNetworkConnectionState GetConnectionState() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsConnected() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsHost() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsClient() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    EMingNetworkRole GetNetworkRole() const;

    // Player management
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetPlayerReady(bool bReady);

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsPlayerReady(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "Network")
    TArray<FMingPlayerNetworkInfo> GetAllPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    FMingPlayerNetworkInfo GetPlayerInfo(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "Network")
    FMingPlayerNetworkInfo GetLocalPlayerInfo() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    int32 GetLocalPlayerID() const;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void KickPlayer(int32 PlayerID, const FString& Reason);

    // Game control
    UFUNCTION(BlueprintCallable, Category = "Network")
    void StartGame();

    UFUNCTION(BlueprintCallable, Category = "Network")
    void EndGame();

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsGameInProgress() const;

    // Chat system
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SendChatMessage(const FString& Message, bool bIsPrivate = false, int32 TargetPlayerID = -1);

    // Host migration
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool PerformHostMigration();

    // Network stats
    UFUNCTION(BlueprintPure, Category = "Network")
    FMingNetworkStats GetNetworkStats() const;

    UFUNCTION(BlueprintCallable, Category = "Network")
    void GetDetailedNetworkStats(FString& OutStats) const;

    // Error handling
    UFUNCTION(BlueprintPure, Category = "Network")
    EMingNetworkError GetLastError() const;

    UFUNCTION(BlueprintPure, Category = "Network")
    FString GetLastErrorMessage() const;

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetNetworkConfig(const FMingNetworkConfig& NewConfig);

    UFUNCTION(BlueprintPure, Category = "Network")
    FMingNetworkConfig GetNetworkConfig() const;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnNetworkConnectionStateChanged OnConnectionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnPlayerJoined OnPlayerJoined;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnPlayerLeft OnPlayerLeft;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnPlayerReadyChanged OnPlayerReadyChanged;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnChatMessageReceived OnChatMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnNetworkError OnNetworkError;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnHostMigrated OnHostMigrated;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnGameStarted OnGameStarted;

    UPROPERTY(BlueprintAssignable, Category = "Network Events")
    FOnGameEnded OnGameEnded;

private:
    // Network state
    UPROPERTY()
    EMingNetworkConnectionState ConnectionState;

    UPROPERTY()
    EMingNetworkRole NetworkRole;

    UPROPERTY()
    FMingNetworkConfig CurrentConfig;

    UPROPERTY()
    TArray<FMingPlayerNetworkInfo> ConnectedPlayers;

    UPROPERTY()
    int32 LocalPlayerID;

    UPROPERTY()
    bool bGameInProgress;

    UPROPERTY()
    EMingNetworkError LastError;

    UPROPERTY()
    FString LastErrorMessage;

    UPROPERTY()
    FMingNetworkStats NetworkStats;

    // Internal functions
    void UpdateConnectionState(EMingNetworkConnectionState NewState);
    void HandleConnectionSuccess();
    void HandleConnectionFailure(EMingNetworkError Error, const FString& Message);
    void UpdateNetworkStats();
    void BroadcastChatMessageToAll(const FMingChatMessage& Message);
    void BroadcastChatMessageToPlayer(const FMingChatMessage& Message, int32 TargetPlayerID);
    void AddPlayer(const FMingPlayerNetworkInfo& PlayerInfo);
    void RemovePlayer(int32 PlayerID);
    void UpdatePlayerReadyState(int32 PlayerID, bool bReady);
    int32 GeneratePlayerID();
    bool ValidatePassword(const FString& Password) const;
    void OnRep_ConnectionState();
};
