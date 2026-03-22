#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMultiplayerTypes.generated.h"

UENUM(BlueprintType)
enum class EMingNetworkRole : uint8
{
    None UMETA(DisplayName = "None"),
    Client UMETA(DisplayName = "Client"),
    Host UMETA(DisplayName = "Host"),
    DedicatedServer UMETA(DisplayName = "Dedicated Server"),
    ListenServer UMETA(DisplayName = "Listen Server")
};

UENUM(BlueprintType)
enum class EMingConnectionState : uint8
{
    Disconnected UMETA(DisplayName = "Disconnected"),
    Connecting UMETA(DisplayName = "Connecting"),
    Connected UMETA(DisplayName = "Connected"),
    Authenticating UMETA(DisplayName = "Authenticating"),
    Authenticated UMETA(DisplayName = "Authenticated"),
    Ready UMETA(DisplayName = "Ready"),
    Error UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EMingNetworkProtocol : uint8
{
    UDP UMETA(DisplayName = "UDP"),
    TCP UMETA(DisplayName = "TCP"),
    Reliable UDP UMETA(DisplayName = "Reliable UDP"),
    WebSocket UMETA(DisplayName = "WebSocket"),
    QUIC UMETA(DisplayName = "QUIC")
};

UENUM(BlueprintType)
enum class EMingSyncPriority : uint8
{
    Critical UMETA(DisplayName = "Critical"),
    High UMETA(DisplayName = "High"),
    Normal UMETA(DisplayName = "Normal"),
    Low UMETA(DisplayName = "Low"),
    Background UMETA(DisplayName = "Background")
};

UENUM(BlueprintType)
enum class EMingReplicationMode : uint8
{
    Always UMETA(DisplayName = "Always"),
    OwnerOnly UMETA(DisplayName = "Owner Only"),
    ServerOnly UMETA(DisplayName = "Server Only"),
    Proximity UMETA(DisplayName = "Proximity"),
    Manual UMETA(DisplayName = "Manual")
};

USTRUCT(BlueprintType)
struct FMingPlayerNetworkInfo
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PlayerID;

    UPROPERTY()
    FString PlayerName;

    UPROPERTY()
    FString UniqueNetId;

    UPROPERTY()
    EMingNetworkRole NetworkRole;

    UPROPERTY()
    EMingConnectionState ConnectionState;

    UPROPERTY()
    float PingMs;

    UPROPERTY()
    float PacketLossPercent;

    UPROPERTY()
    int32 BytesReceivedPerSecond;

    UPROPERTY()
    int32 BytesSentPerSecond;

    UPROPERTY()
    FDateTime ConnectionTime;

    UPROPERTY()
    bool bIsHost;

    UPROPERTY()
    bool bIsAdmin;

    FMingPlayerNetworkInfo()
        : PlayerID(-1)
        , NetworkRole(EMingNetworkRole::None)
        , ConnectionState(EMingConnectionState::Disconnected)
        , PingMs(0.0f)
        , PacketLossPercent(0.0f)
        , BytesReceivedPerSecond(0)
        , BytesSentPerSecond(0)
        , bIsHost(false)
        , bIsAdmin(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingNetworkSessionInfo
{
    GENERATED_BODY()

    UPROPERTY()
    FString SessionID;

    UPROPERTY()
    FString SessionName;

    UPROPERTY()
    FString HostName;

    UPROPERTY()
    int32 MaxPlayers;

    UPROPERTY()
    int32 CurrentPlayers;

    UPROPERTY()
    bool bIsPasswordProtected;

    UPROPERTY()
    bool bIsLanOnly;

    UPROPERTY()
    int32 MapIndex;

    UPROPERTY()
    FString GameMode;

    UPROPERTY()
    FDateTime CreationTime;

    UPROPERTY()
    int32 TickRate;

    FMingNetworkSessionInfo()
        : MaxPlayers(8)
        , CurrentPlayers(0)
        , bIsPasswordProtected(false)
        , bIsLanOnly(false)
        , MapIndex(0)
        , TickRate(30)
    {}
};

USTRUCT(BlueprintType)
struct FMingNetworkMessage
{
    GENERATED_BODY()

    UPROPERTY()
    int32 MessageType;

    UPROPERTY()
    int32 SenderID;

    UPROPERTY()
    int32 TargetID;

    UPROPERTY()
    TArray<uint8> Payload;

    UPROPERTY()
    EMingSyncPriority Priority;

    UPROPERTY()
    bool bReliable;

    UPROPERTY()
    bool bOrdered;

    UPROPERTY()
    FDateTime Timestamp;

    UPROPERTY()
    int32 SequenceNumber;

    FMingNetworkMessage()
        : MessageType(0)
        , SenderID(-1)
        , TargetID(-1)
        , Priority(EMingSyncPriority::Normal)
        , bReliable(true)
        , bOrdered(false)
        , SequenceNumber(0)
    {}
};

USTRUCT(BlueprintType)
struct FMingNetworkStats
{
    GENERATED_BODY()

    UPROPERTY()
    float AveragePing;

    UPROPERTY()
    float MinPing;

    UPROPERTY()
    float MaxPing;

    UPROPERTY()
    float PacketLossRate;

    UPROPERTY()
    int32 TotalBytesReceived;

    UPROPERTY()
    int32 TotalBytesSent;

    UPROPERTY()
    int32 MessagesPerSecond;

    UPROPERTY()
    int32 FailedConnections;

    UPROPERTY()
    float UptimeSeconds;

    FMingNetworkStats()
        : AveragePing(0.0f)
        , MinPing(0.0f)
        , MaxPing(0.0f)
        , PacketLossRate(0.0f)
        , TotalBytesReceived(0)
        , TotalBytesSent(0)
        , MessagesPerSecond(0)
        , FailedConnections(0)
        , UptimeSeconds(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingSyncBatch
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BatchID;

    UPROPERTY()
    TArray<FMingNetworkMessage> Messages;

    UPROPERTY()
    float Timestamp;

    UPROPERTY()
    bool bCompressed;

    UPROPERTY()
    int32 OriginalSize;

    UPROPERTY()
    int32 CompressedSize;

    FMingSyncBatch()
        : BatchID(0)
        , Timestamp(0.0f)
        , bCompressed(false)
        , OriginalSize(0)
        , CompressedSize(0)
    {}
};

