#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingMultiplayerTypes.generated.h"

UENUM(BlueprintType)
enum class EMingNetworkRole: uint8 {
    None UMETA(DisplayName = "None"),
    Client UMETA(DisplayName = "Client"),
    Host UMETA(DisplayName = "Host"),
    DedicatedServer UMETA(DisplayName = "Dedicated Server"),
    ListenServer UMETA(DisplayName = "Listen Server")
};

UENUM(BlueprintType)
enum class EMingConnectionState: uint8 {
    Disconnected UMETA(DisplayName = "Disconnected"),
    Connecting UMETA(DisplayName = "Connecting"),
    Connected UMETA(DisplayName = "Connected"),
    Authenticating UMETA(DisplayName = "Authenticating"),
    Authenticated UMETA(DisplayName = "Authenticated"),
    Ready UMETA(DisplayName = "Ready"),
    Error UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EMingNetworkProtocol: uint8 {
    UDP UMETA(DisplayName = "UDP"),
    TCP UMETA(DisplayName = "TCP"),
    Reliable UDP UMETA(DisplayName = "Reliable UDP"),
    WebSocket UMETA(DisplayName = "WebSocket"),
    QUIC UMETA(DisplayName = "QUIC")
};

UENUM(BlueprintType)
enum class EMingSyncPriority: uint8 {
    Critical UMETA(DisplayName = "Critical"),
    High UMETA(DisplayName = "High"),
    Normal UMETA(DisplayName = "Normal"),
    Low UMETA(DisplayName = "Low"),
    Background UMETA(DisplayName = "Background")
};

UENUM(BlueprintType)
enum class EMingReplicationMode: uint8 {
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

// Cooperative Multiplayer Types
USTRUCT(BlueprintType)
struct FMingCooperativeSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString SessionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    EMingCoopMode CoopMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    int32 MaxPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    EMingDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    bool bAIAssistanceEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    EMingResourceSharingPolicy ResourceSharingPolicy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    TArray<FString> AvailableObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    float SessionTimeoutMinutes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString Password;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    bool bPrivateSession;

    FMingCooperativeSessionInfo()
    {
        SessionID = TEXT("");
        SessionName = TEXT("");
        Description = TEXT("");
        CoopMode = EMingCoopMode::Campaign;
        MaxPlayers = 4;
        Difficulty = EMingDifficulty::Normal;
        bAIAssistanceEnabled = true;
        ResourceSharingPolicy = EMingResourceSharingPolicy::Shared;
        SessionTimeoutMinutes = 120.0f;
        bPrivateSession = false;
    }
};

USTRUCT(BlueprintType)
struct FMingPlayerCooperativeInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    int32 PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString PlayerName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    EMingCoopRole Role;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    bool bReady;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    int32 Score;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    TArray<FString> CompletedObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    TArray<FMingResourceShareRequest> PendingResourceRequests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FVector LastKnownPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    float LastActiveTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    bool bIsLocalPlayer;

    FMingPlayerCooperativeInfo()
    {
        PlayerID = -1;
        PlayerName = TEXT("");
        Role = EMingCoopRole::Member;
        bReady = false;
        Score = 0;
        LastKnownPosition = FVector::ZeroVector;
        LastActiveTime = 0.0f;
        bIsLocalPlayer = false;
    }
};

USTRUCT(BlueprintType)
struct FMingCooperativeObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString ObjectiveID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    EMingObjectiveType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    EMingObjectiveStatus Status;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    int32 RequiredPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    TArray<int32> AssignedPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    float TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    float TimeRemaining;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    TArray<FString> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    TArray<FString> RewardResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    bool bOptional;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Objective")
    int32 ScoreReward;

    FMingCooperativeObjective()
    {
        ObjectiveID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Type = EMingObjectiveType::Capture;
        Status = EMingObjectiveStatus::Active;
        TargetLocation = FVector::ZeroVector;
        RequiredPlayers = 1;
        TimeLimit = 300.0f;
        TimeRemaining = 300.0f;
        bOptional = false;
        ScoreReward = 100;
    }
};

USTRUCT(BlueprintType)
struct FMingResourceShareRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    int32 FromPlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    int32 ToPlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    FString ResourceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    int32 Amount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    FVector PickupLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    float ExpiryTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Resource Share")
    bool bAccepted;

    FMingResourceShareRequest()
    {
        FromPlayerID = -1;
        ToPlayerID = -1;
        ResourceType = TEXT("");
        Amount = 0;
        PickupLocation = FVector::ZeroVector;
        ExpiryTime = 0.0f;
        bAccepted = false;
    }
};

USTRUCT(BlueprintType)
struct FMingTacticalUpdate
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    int32 PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    EMingTacticalUpdateType UpdateType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    FVector Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    FString Message;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    TArray<int32> TargetPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    float Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tactical Update")
    bool bUrgent;

    FMingTacticalUpdate()
    {
        PlayerID = -1;
        UpdateType = EMingTacticalUpdateType::EnemySpotted;
        Location = FVector::ZeroVector;
        Timestamp = 0.0f;
        bUrgent = false;
    }
};

USTRUCT(BlueprintType)
struct FMingCoopProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    float OverallProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    int32 ObjectivesCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    int32 TotalObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    float TimeElapsed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    float TotalTimeAllowed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    TArray<FString> UnlockedAchievements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    int32 ResourcesShared;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progress")
    float TeamworkScore;

    FMingCoopProgress()
    {
        OverallProgress = 0.0f;
        ObjectivesCompleted = 0;
        TotalObjectives = 0;
        TimeElapsed = 0.0f;
        TotalTimeAllowed = 3600.0f;
        ResourcesShared = 0;
        TeamworkScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingCoopScore
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    TMap<int32, int32> PlayerScores;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    int32 TeamScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    float TeamworkBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    float SpeedBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    float EfficiencyBonus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Score")
    int32 TotalScore;

    FMingCoopScore()
    {
        TeamScore = 0;
        TeamworkBonus = 0.0f;
        SpeedBonus = 0.0f;
        EfficiencyBonus = 0.0f;
        TotalScore = 0;
    }
};

UENUM(BlueprintType)
enum class EMingCoopMode: uint8 {
    Campaign     = 0,
    Skirmish     = 1,
    Survival     = 2,
    Defense      = 3,
    Exploration  = 4,
    Custom       = 5
};

UENUM(BlueprintType)
enum class EMingCoopRole: uint8 {
    Leader       = 0,
    Commander    = 1,
    Specialist   = 2,
    Support      = 3,
    Scout        = 4,
    Member       = 5
};

UENUM(BlueprintType)
enum class EMingObjectiveType: uint8 {
    Capture      = 0,
    Defend       = 1,
    Escort       = 2,
    Rescue       = 3,
    Build        = 4,
    Destroy      = 5,
    Collect      = 6,
    Explore      = 7,
    Survive      = 8,
    Custom       = 9
};

UENUM(BlueprintType)
enum class EMingObjectiveStatus: uint8 {
    Active       = 0,
    Completed    = 1,
    Failed       = 2,
    Paused       = 3,
    Upcoming     = 4,
    Cancelled    = 5
};

UENUM(BlueprintType)
enum class EMingResourceSharingPolicy: uint8 {
    Individual   = 0,
    Shared      = 1,
    Requested   = 2,
    Restricted  = 3,
    Custom      = 4
};

UENUM(BlueprintType)
enum class EMingCoopMessageType: uint8 {
    General     = 0,
    Tactical    = 1,
    Request     = 2,
    Alert       = 3,
    Status      = 4,
    Objective   = 5,
    Resource    = 6,
    System      = 7
};

UENUM(BlueprintType)
enum class EMingCoopGameState: uint8 {
    Waiting     = 0,
    Starting    = 1,
    Playing     = 2,
    Paused      = 3,
    Completed   = 4,
    Failed      = 5,
    Ending      = 6
};

UENUM(BlueprintType)
enum class EMingAIAssistanceType: uint8 {
    None        = 0,
    Guidance    = 1,
    Reinforcement = 2,
    Support     = 3,
    Defense     = 4,
    Resources   = 5,
    Full        = 6
};

UENUM(BlueprintType)
enum class EMingAIAssistanceLevel: uint8 {
    Off         = 0,
    Minimal     = 1,
    Normal      = 2,
    High        = 3,
    Maximum     = 4
};

UENUM(BlueprintType)
enum class EMingTacticalUpdateType: uint8 {
    EnemySpotted     = 0,
    ResourceFound    = 1,
    ObjectiveUpdate  = 2,
    DangerAlert      = 3,
    RequestHelp      = 4,
    StrategicPoint    = 5,
    TeamPosition     = 6,
    Custom           = 7
};

UENUM(BlueprintType)
enum class EMingDifficulty: uint8 {
    Easy        = 0,
    Normal      = 1,
    Hard        = 2,
    Expert      = 3,
    Legendary   = 4,
    Custom      = 5
};

// VR/AR Types
USTRUCT(BlueprintType)
struct FMingVRDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    FString DeviceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    FString DeviceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    EMingVRDeviceType DeviceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    bool bConnected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    bool bTracked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    float BatteryLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Device")
    FString FirmwareVersion;

    FMingVRDeviceInfo()
    {
        DeviceID = TEXT("");
        DeviceName = TEXT("");
        DeviceType = EMingVRDeviceType::HeadMounted;
        bConnected = false;
        bTracked = false;
        Position = FVector::ZeroVector;
        Rotation = FRotator::ZeroRotator;
        BatteryLevel = 1.0f;
        FirmwareVersion = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingVRControllerInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    EVRHand Hand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    bool bConnected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    FVector Position;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    FRotator Rotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    bool bTriggerPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    float TriggerValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    bool bGripPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    bool bTouchpadPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    FVector2D TouchpadAxis;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    float HapticIntensity;

    FMingVRControllerInfo()
    {
        Hand = EVRHand::Left;
        bConnected = false;
        Position = FVector::ZeroVector;
        Rotation = FRotator::ZeroRotator;
        bTriggerPressed = false;
        TriggerValue = 0.0f;
        bGripPressed = false;
        bTouchpadPressed = false;
        TouchpadAxis = FVector2D::ZeroVector;
        HapticIntensity = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingVRRoomBounds
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Room")
    TArray<FVector> BoundaryPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Room")
    FVector Center;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Room")
    FVector Size;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Room")
    float Area;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Room")
    bool bValid;

    FMingVRRoomBounds()
    {
        Center = FVector::ZeroVector;
        Size = FVector::ZeroVector;
        Area = 0.0f;
        bValid = false;
    }
};

USTRUCT(BlueprintType)
struct FMingVRInteractionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
    FVector InteractionLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
    FRotator InteractionRotation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
    EVRHand Hand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
    float InteractionStrength;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Interaction")
    float Timestamp;

    FMingVRInteractionData()
    {
        InteractionLocation = FVector::ZeroVector;
        InteractionRotation = FRotator::ZeroRotator;
        Hand = EVRHand::Left;
        InteractionStrength = 0.0f;
        Timestamp = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingVRRenderSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    int32 ResolutionX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    int32 ResolutionY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    float RenderScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    bool bMotionBlurEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    bool bAntiAliasingEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    float TextureQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Render")
    bool bDynamicResolutionEnabled;

    FMingVRRenderSettings()
    {
        ResolutionX = 2160;
        ResolutionY = 1200;
        RenderScale = 1.0f;
        bMotionBlurEnabled = false;
        bAntiAliasingEnabled = true;
        TextureQuality = 1.0f;
        bDynamicResolutionEnabled = true;
    }
};

USTRUCT(BlueprintType)
struct FMingARDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    FString DeviceID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    FString DeviceName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    EMingARDeviceType DeviceType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    bool bARSupported;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    bool bWorldTrackingSupported;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    bool bFaceTrackingSupported;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Device")
    bool bImageTrackingSupported;

    FMingARDeviceInfo()
    {
        DeviceID = TEXT("");
        DeviceName = TEXT("");
        DeviceType = EMingARDeviceType::Smartphone;
        bARSupported = false;
        bWorldTrackingSupported = false;
        bFaceTrackingSupported = false;
        bImageTrackingSupported = false;
    }
};

USTRUCT(BlueprintType)
struct FMingARSessionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    EMingARAlignmentType AlignmentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    EMingARPlaneDetectionMode PlaneDetectionMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bRunInBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bAutoFocusEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    float SessionDuration;

    FMingARSessionInfo()
    {
        SessionID = TEXT("");
        AlignmentType = EMingARAlignmentType::World;
        PlaneDetectionMode = EMingARPlaneDetectionMode::Horizontal;
        bRunInBackground = true;
        bAutoFocusEnabled = true;
        SessionDuration = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingARPlane
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    FString PlaneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    EMingARPlaneType PlaneType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    FVector Center;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    FVector Extents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    FVector Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    TArray<FVector> BoundaryVertices;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    bool bIsUpdated;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Plane")
    float LastUpdateTimestamp;

    FMingARPlane()
    {
        PlaneID = TEXT("");
        PlaneType = EMingARPlaneType::Horizontal;
        Center = FVector::ZeroVector;
        Extents = FVector::ZeroVector;
        Normal = FVector::UpVector;
        bIsUpdated = false;
        LastUpdateTimestamp = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingARAnchor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    FString AnchorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    FTransform Transform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    FString AttachedPlaneID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    bool bIsTracked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    float TrackingQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Anchor")
    FDateTime CreationTime;

    FMingARAnchor()
    {
        AnchorID = TEXT("");
        Transform = FTransform::Identity;
        AttachedPlaneID = TEXT("");
        bIsTracked = false;
        TrackingQuality = 1.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingARInteractionData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    FVector2D ScreenPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    FVector WorldPosition;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    FVector WorldNormal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    FString HitAnchorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    class AActor* HitActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Interaction")
    float Timestamp;

    FMingARInteractionData()
    {
        ScreenPosition = FVector2D::ZeroVector;
        WorldPosition = FVector::ZeroVector;
        WorldNormal = FVector::UpVector;
        HitAnchorID = TEXT("");
        HitActor = nullptr;
        Timestamp = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingARRenderSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    int32 CameraResolutionX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    int32 CameraResolutionY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    float CameraFrameRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    bool bEnvironmentLightingEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    bool bOcclusionEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    EMingARQualityLevel QualityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Render")
    float RenderScale;

    FMingARRenderSettings()
    {
        CameraResolutionX = 1920;
        CameraResolutionY = 1080;
        CameraFrameRate = 60.0f;
        bEnvironmentLightingEnabled = true;
        bOcclusionEnabled = false;
        QualityLevel = EMingARQualityLevel::Medium;
        RenderScale = 1.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingAREnvironmentInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Environment")
    float AmbientLightIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Environment")
    FLinearColor AmbientLightColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Environment")
    float ColorTemperature;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Environment")
    float ReflectionIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Environment")
    bool bEnvironmentTexturingEnabled;

    FMingAREnvironmentInfo()
    {
        AmbientLightIntensity = 1.0f;
        AmbientLightColor = FLinearColor::White;
        ColorTemperature = 6500.0f;
        ReflectionIntensity = 0.5f;
        bEnvironmentTexturingEnabled = true;
    }
};

USTRUCT(BlueprintType)
struct FMingARHistoricalMarker
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FString MarkerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FString LocationName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FString HistoricalInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FString Period;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FVector WorldLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    FString AssociatedAnchorID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    TArray<FString> RelatedImages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Historical")
    TArray<FString> AudioClips;

    FMingARHistoricalMarker()
    {
        MarkerID = TEXT("");
        LocationName = TEXT("");
        HistoricalInfo = TEXT("");
        Period = TEXT("");
        WorldLocation = FVector::ZeroVector;
        AssociatedAnchorID = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FMingARSessionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    EMingARAlignmentType AlignmentType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    EMingARPlaneDetectionMode PlaneDetectionMode;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    bool bAutoFocusEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    bool bRunInBackground;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    EMingARQualityLevel QualityLevel;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    bool bEnvironmentLightingEnabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Config")
    bool bOcclusionEnabled;

    FMingARSessionConfig()
    {
        AlignmentType = EMingARAlignmentType::World;
        PlaneDetectionMode = EMingARPlaneDetectionMode::Horizontal;
        bAutoFocusEnabled = true;
        bRunInBackground = true;
        QualityLevel = EMingARQualityLevel::Medium;
        bEnvironmentLightingEnabled = true;
        bOcclusionEnabled = false;
    }
};

// VR Enums
UENUM(BlueprintType)
enum class EMingVRMode: uint8 {
    Disabled    = 0,
    Seated      = 1,
    Standing    = 2,
    RoomScale   = 3,
    Cinematic   = 4
};

UENUM(BlueprintType)
enum class EMingVRDeviceType: uint8 {
    HeadMounted     = 0,
    Controller      = 1,
    Tracker         = 2,
    BaseStation     = 3,
    Accessory       = 4
};

UENUM(BlueprintType)
enum class EMingVRInteractionType: uint8 {
    None            = 0,
    Trigger         = 1,
    Grip            = 2,
    Touchpad        = 3,
    Gesture         = 4,
    Voice           = 5,
    Gaze            = 6
};

UENUM(BlueprintType)
enum class EMingVRComfortMode: uint8 {
    None            = 0,
    SnapTurning     = 1,
    SmoothTurning   = 2,
    Vignette        = 3,
    Teleport        = 4,
    Hybrid          = 5
};

// AR Enums
UENUM(BlueprintType)
enum class EMingARTrackingState: uint8 {
    NotAvailable    = 0,
    Limited         = 1,
    Normal          = 2,
    ExcessiveMotion = 3,
    InsufficientFeatures = 4
};

UENUM(BlueprintType)
enum class EMingARDeviceType: uint8 {
    Smartphone      = 0,
    Tablet          = 1,
    HeadMounted     = 2,
    SmartGlasses   = 3,
    Drone           = 4
};

UENUM(BlueprintType)
enum class EMingARAlignmentType: uint8 {
    World           = 0,
    Camera          = 1,
    Gravity         = 2
};

UENUM(BlueprintType)
enum class EMingARPlaneDetectionMode: uint8 {
    None            = 0,
    Horizontal      = 1,
    Vertical        = 2,
    Both            = 3
};

UENUM(BlueprintType)
enum class EMingARPlaneType: uint8 {
    None            = 0,
    HorizontalUp    = 1,
    HorizontalDown  = 2,
    Vertical        = 3,
    Arbitrary       = 4
};

UENUM(BlueprintType)
enum class EMingARInteractionType: uint8 {
    None            = 0,
    Touch           = 1,
    Gesture         = 2,
    Voice           = 3,
    Gaze            = 4,
    ObjectPlacement = 5
};

UENUM(BlueprintType)
enum class EMingAROcclusionMode: uint8 {
    None            = 0,
    Environment     = 1,
    People          = 2,
    Both            = 3
};

UENUM(BlueprintType)
enum class EMingARQualityLevel: uint8 {
    Low             = 0,
    Medium          = 1,
    High            = 2,
    Ultra           = 3,
    Automatic       = 4
};

// Educational Types
USTRUCT(BlueprintType)
struct FMingEducationalLesson
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    FString LessonID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    FString Content;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    EMingEducationalDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    EMingEducationalCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    TArray<FString> LearningObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    TArray<FString> RequiredKnowledge;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    TArray<FString> RelatedTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    float EstimatedDuration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    TArray<FMingEducationalQuiz> AssociatedQuizzes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    TArray<FString> MediaAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    float CompletionScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Lesson")
    FDateTime CompletionTime;

    FMingEducationalLesson()
    {
        LessonID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Content = TEXT("");
        Difficulty = EMingEducationalDifficulty::Beginner;
        Category = EMingEducationalCategory::History;
        EstimatedDuration = 30.0f;
        bCompleted = false;
        CompletionScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalQuiz
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    FString QuizID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    TArray<FMingQuizQuestion> Questions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    EMingEducationalDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    int32 TimeLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    float PassingScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    int32 CorrectAnswers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    float TotalScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Quiz")
    FDateTime CompletionTime;

    FMingEducationalQuiz()
    {
        QuizID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Difficulty = EMingEducationalDifficulty::Beginner;
        TimeLimit = 300;
        PassingScore = 70.0f;
        bCompleted = false;
        CorrectAnswers = 0;
        TotalScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingQuizQuestion
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString QuestionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString QuestionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    EMingQuestionType QuestionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    TArray<FString> AnswerOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString CorrectAnswer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString PlayerAnswer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    bool bAnswered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    bool bCorrect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    FString Explanation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz Question")
    TArray<FString> Hints;

    FMingQuizQuestion()
    {
        QuestionID = TEXT("");
        QuestionText = TEXT("");
        QuestionType = EMingQuestionType::MultipleChoice;
        bAnswered = false;
        bCorrect = false;
    }
};

USTRUCT(BlueprintType)
struct FMingHistoricalContent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString FullText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString Period;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString Dynasty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    FString Region;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    TArray<FString> ImportantEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    TArray<FString> RelatedTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    TArray<FString> MediaAssets;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    TArray<FString> Sources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Content")
    EMingEducationalDifficulty Difficulty;

    FMingHistoricalContent()
    {
        ContentID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        FullText = TEXT("");
        Period = TEXT("");
        Dynasty = TEXT("");
        Region = TEXT("");
        Difficulty = EMingEducationalDifficulty::Beginner;
    }
};

USTRUCT(BlueprintType)
struct FMingHistoricalEvent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString EventID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FDateTime EventDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString Period;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    FString Dynasty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    TArray<FString> RelatedEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    bool bTriggered;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    float TriggerTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Event")
    bool bPlayerInfluenced;

    FMingHistoricalEvent()
    {
        EventID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Location = TEXT("");
        Period = TEXT("");
        Dynasty = TEXT("");
        bTriggered = false;
        TriggerTime = 0.0f;
        bPlayerInfluenced = false;
    }
};

USTRUCT(BlueprintType)
struct FMingInteractiveLearningModule
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    FString ModuleID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    EMingInteractiveLearningType LearningType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    TArray<FMingLearningInteraction> Interactions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    TArray<FString> LearningObjectives;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    TArray<FString> RequiredResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    bool bCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    float CompletionScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Module")
    float TimeSpent;

    FMingInteractiveLearningModule()
    {
        ModuleID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        LearningType = EMingInteractiveLearningType::Simulation;
        bCompleted = false;
        CompletionScore = 0.0f;
        TimeSpent = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingLearningInteraction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    FString InteractionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    EMingInteractionType InteractionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    FString Prompt;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    TArray<FString> ResponseOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    FString PlayerResponse;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    bool bCorrect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    FString Feedback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Interaction")
    float Timestamp;

    FMingLearningInteraction()
    {
        InteractionID = TEXT("");
        Prompt = TEXT("");
        PlayerResponse = TEXT("");
        bCorrect = false;
        Feedback = TEXT("");
        Timestamp = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalProgress
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    float OverallProgress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    int32 LessonsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    int32 TotalLessons;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    int32 QuizzesCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    int32 HistoricalEventsExplored;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    float TotalLearningTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    float AverageQuizScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    TArray<FString> MasteredTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    TArray<FString> WeakAreas;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Progress")
    FDateTime LastActivity;

    FMingEducationalProgress()
    {
        OverallProgress = 0.0f;
        LessonsCompleted = 0;
        TotalLessons = 0;
        QuizzesCompleted = 0;
        HistoricalEventsExplored = 0;
        TotalLearningTime = 0.0f;
        AverageQuizScore = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalAchievement
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    FString AchievementID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    EMingAchievementType AchievementType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    FString IconPath;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    TArray<FString> Requirements;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    bool bUnlocked;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    FDateTime UnlockTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    int32 Points;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Achievement")
    bool bHidden;

    FMingEducationalAchievement()
    {
        AchievementID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        AchievementType = EMingAchievementType::Learning;
        IconPath = TEXT("");
        bUnlocked = false;
        Points = 0;
        bHidden = false;
    }
};

USTRUCT(BlueprintType)
struct FMingPlayerLearningProfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    FString PlayerID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    EMingLearningStyle LearningStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    EMingEducationalDifficulty PreferredDifficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    TArray<FString> FavoriteTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    TArray<FString> ChallengingTopics;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    float AverageLearningSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    float RetentionRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    int32 SessionsCompleted;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    float TotalLearningTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    TMap<FString, float> TopicMastery;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning Profile")
    TMap<FString, int32> InteractionCounts;

    FMingPlayerLearningProfile()
    {
        PlayerID = TEXT("");
        LearningStyle = EMingLearningStyle::Visual;
        PreferredDifficulty = EMingEducationalDifficulty::Beginner;
        AverageLearningSpeed = 1.0f;
        RetentionRate = 0.8f;
        SessionsCompleted = 0;
        TotalLearningTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FMingEducationalContent
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    FString ContentID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    EMingEducationalCategory Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    EMingEducationalDifficulty Difficulty;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    FString AdaptedContent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    TArray<FString> PersonalizedHints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    TArray<FString> RecommendedResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Educational Content")
    float AdaptationScore;

    FMingEducationalContent()
    {
        ContentID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        Category = EMingEducationalCategory::History;
        Difficulty = EMingEducationalDifficulty::Beginner;
        AdaptationScore = 0.0f;
    }
};

// Educational Enums
UENUM(BlueprintType)
enum class EMingEducationalMode: uint8 {
    Disabled        = 0,
    Tutorial        = 1,
    GuidedLearning  = 2,
    FreeExploration  = 3,
    Assessment      = 4,
    Custom          = 5
};

UENUM(BlueprintType)
enum class EMingEducationalDifficulty: uint8 {
    Beginner        = 0,
    Intermediate    = 1,
    Advanced        = 2,
    Expert          = 3,
    Adaptive        = 4
};

UENUM(BlueprintType)
enum class EMingEducationalCategory: uint8 {
    History         = 0,
    Culture         = 1,
    Military        = 2,
    Politics        = 3,
    Economics       = 4,
    Geography       = 5,
    Art             = 6,
    Literature      = 7,
    Science         = 8,
    Philosophy      = 9,
    Custom          = 10
};

UENUM(BlueprintType)
enum class EMingQuestionType: uint8 {
    MultipleChoice  = 0,
    TrueFalse       = 1,
    FillInBlank     = 2,
    Essay           = 3,
    Matching        = 4,
    Ordering        = 5,
    Interactive     = 6,
    Custom          = 7
};

UENUM(BlueprintType)
enum class EMingInteractiveLearningType: uint8 {
    Simulation      = 0,
    VirtualTour     = 1,
    RolePlaying     = 2,
    ProblemSolving  = 3,
    CreativeProject = 4,
    Debate          = 5,
    Experiment      = 6,
    Custom          = 7
};

UENUM(BlueprintType)
enum class EMingInteractionType: uint8 {
    Question        = 0,
    Choice          = 1,
    Input           = 2,
    DragDrop        = 3,
    Selection       = 4,
    Gesture         = 5,
    Voice           = 6,
    Custom          = 7
};

UENUM(BlueprintType)
enum class EMingAchievementType: uint8 {
    Learning        = 0,
    Exploration     = 1,
    Completion      = 2,
    Mastery         = 3,
    Speed           = 4,
    Accuracy        = 5,
    Consistency     = 6,
    Collaboration   = 7,
    Custom          = 8
};

UENUM(BlueprintType)
enum class EMingLearningStyle: uint8 {
    Visual          = 0,
    Auditory        = 1,
    Kinesthetic      = 2,
    Reading         = 3,
    Mixed           = 4,
    Adaptive        = 5
};

