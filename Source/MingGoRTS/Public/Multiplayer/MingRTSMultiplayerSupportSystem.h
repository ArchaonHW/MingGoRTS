#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSMultiplayerSupportSystem.generated.h"

/**
 * 網絡連接狀態
 */
UENUM(BlueprintType)
enum class EMultiplayerConnectionState: uint8 {
    Offline               UMETA(DisplayName = "離線"),
    Connecting            UMETA(DisplayName = "連接中"),
    Connected             UMETA(DisplayName = "已連接"),
    Authenticating        UMETA(DisplayName = "認證中"),
    Authenticated         UMETA(DisplayName = "已認證"),
    Joining               UMETA(DisplayName = "加入中"),
    InGame                UMETA(DisplayName = "遊戲中"),
    Disconnecting         UMETA(DisplayName = "斷開中"),
    Error                 UMETA(DisplayName = "錯誤")
};

/**
 * 多人遊戲模式
 */
UENUM(BlueprintType)
enum class EMultiplayerGameMode: uint8 {
    Cooperative           UMETA(DisplayName = "合作模式"),
    Competitive           UMETA(DisplayName = "對抗模式"),
    TeamBattle            UMETA(DisplayName = "團隊戰"),
    FreeForAll            UMETA(DisplayName = "混戰"),
    CampaignCoop          UMETA(DisplayName = "戰役合作"),
    Spectator             UMETA(DisplayName = "觀戰模式"),
    Custom                UMETA(DisplayName = "自定義")
};

/**
 * 玩家網絡角色
 */
UENUM(BlueprintType)
enum class ENetworkPlayerRole: uint8 {
    None                  UMETA(DisplayName = "無"),
    Host                  UMETA(DisplayName = "房主"),
    Client                UMETA(DisplayName = "客戶端"),
    ListenServer          UMETA(DisplayName = "監聽服務器"),
    DedicatedServer       UMETA(DisplayName = "專用服務器")
};

/**
 * 網絡玩家信息
 */
USTRUCT(BlueprintType)
struct FNetworkPlayerInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlayerName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ENetworkPlayerRole Role;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Ping;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsReady;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TeamID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString JoinTime;
    
    FNetworkPlayerInfo()
        : Role(ENetworkPlayerRole::None)
        , Ping(0)
        , bIsReady(false)
        , TeamID(0)
    {}
};

/**
 * 多人遊戲房間
 */
USTRUCT(BlueprintType)
struct FMultiplayerRoom
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HostID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMultiplayerGameMode GameMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bHasPassword;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Password;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInProgress;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FNetworkPlayerInfo> Players;
    
    FMultiplayerRoom()
        : GameMode(EMultiplayerGameMode::Cooperative)
        , MaxPlayers(4)
        , CurrentPlayers(0)
        , bHasPassword(false)
        , bIsInProgress(false)
    {}
};

/**
 * 多人遊戲會話配置
 */
USTRUCT(BlueprintType)
struct FMultiplayerSessionConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SessionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMultiplayerGameMode GameMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLan;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShouldAdvertise;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllowJoinInProgress;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TeamCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CustomSettings;
    
    FMultiplayerSessionConfig()
        : GameMode(EMultiplayerGameMode::Cooperative)
        , MaxPlayers(4)
        , bIsLan(false)
        , bShouldAdvertise(true)
        , bAllowJoinInProgress(true)
        , TeamCount(2)
    {}
};

/**
 * 網絡同步數據
 */
USTRUCT(BlueprintType)
struct FNetworkReplicationData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ObjectID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator Rotation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Velocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Health;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHealth;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Team;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString State;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;
    
    FNetworkReplicationData()
        : Health(100)
        , MaxHealth(100)
        , Team(0)
        , Timestamp(0.0f)
    {}
};

/**
 * 多人遊戲支持系統
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSMultiplayerSupportSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSMultiplayerSupportSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void InitializeMultiplayerSystem();
    
    // 連接到服務器
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void ConnectToServer(const FString& ServerAddress, int32 Port);
    
    // 斷開連接
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void Disconnect();
    
    // 創建房間
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    FString CreateRoom(const FMultiplayerSessionConfig& Config);
    
    // 加入房間
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool JoinRoom(const FString& RoomID, const FString& Password);
    
    // 離開房間
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void LeaveRoom();
    
    // 查找房間
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    TArray<FMultiplayerRoom> FindRooms(EMultiplayerGameMode ModeFilter);
    
    // 開始遊戲
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void StartGame();
    
    // 設置玩家準備狀態
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SetPlayerReady(bool bReady);
    
    // 獲取本地玩家信息
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    FNetworkPlayerInfo GetLocalPlayerInfo() const;
    
    // 獲取所有玩家
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    TArray<FNetworkPlayerInfo> GetAllPlayers() const;
    
    // 發送聊天消息
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SendChatMessage(const FString& Message);
    
    // 發送遊戲命令
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SendGameCommand(const FString& CommandType, const TMap<FString, FString>& Parameters);
    
    // 踢出玩家
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void KickPlayer(const FString& PlayerID, const FString& Reason);
    
    // 轉讓房主
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void TransferHost(const FString& NewHostID);
    
    // 設置玩家隊伍
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void SetPlayerTeam(const FString& PlayerID, int32 TeamID);
    
    // 獲取連接狀態
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    EMultiplayerConnectionState GetConnectionState() const;
    
    // 獲取當前房間
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    FMultiplayerRoom GetCurrentRoom() const;
    
    // 檢查是否為房主
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    bool IsHost() const;
    
    // 同步數據到服務器
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    void ReplicateData(const FNetworkReplicationData& Data);
    
    // 獲取網絡延遲
    UFUNCTION(BlueprintCallable, Category = "Multiplayer")
    int32 GetPing() const;
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnected, bool, bSuccess);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDisconnected, const FString&, Reason);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRoomCreated, const FString&, RoomID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, const FNetworkPlayerInfo&, PlayerInfo);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeft, const FString&, PlayerID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarted);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatReceived, const FString&, PlayerID, const FString&, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDataReplicated, const FString&, ObjectID, const FNetworkReplicationData&, Data);
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnConnected OnConnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnDisconnected OnDisconnected;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnRoomCreated OnRoomCreated;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnPlayerJoined OnPlayerJoined;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnPlayerLeft OnPlayerLeft;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnGameStarted OnGameStarted;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnChatReceived OnChatReceived;
    
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnDataReplicated OnDataReplicated;
    
private:
    UPROPERTY()
    EMultiplayerConnectionState ConnectionState;
    
    UPROPERTY()
    FNetworkPlayerInfo LocalPlayer;
    
    UPROPERTY()
    FMultiplayerRoom CurrentRoom;
    
    UPROPERTY()
    TArray<FMultiplayerRoom> AvailableRooms;
    
    UPROPERTY()
    FString ServerAddress;
    
    UPROPERTY()
    int32 ServerPort;
    
    UPROPERTY()
    bool bIsHost;
    
    UPROPERTY()
    int32 CurrentPing;
    
    void InitializeNetworking();
    void ShutdownNetworking();
    void UpdateConnectionState(EMultiplayerConnectionState NewState);
    void HandlePlayerJoined(const FNetworkPlayerInfo& Player);
    void HandlePlayerLeft(const FString& PlayerID);
    void HandleChatMessage(const FString& PlayerID, const FString& Message);
    void HandleGameCommand(const FString& PlayerID, const FString& Command);
    void BroadcastToAllPlayers(const FString& Message);
    void SynchronizeGameState();
    void ProcessReplicationData(const FNetworkReplicationData& Data);
    FString GenerateRoomID();
};
