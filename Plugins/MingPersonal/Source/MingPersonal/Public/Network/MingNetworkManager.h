#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingNetworkManager.generated.h"

// Forward declarations
class UMingRelationshipReplication;
class UMingReputationReplication;
class UMingPersonalManager;

// Network connection states
UENUM(BlueprintType)
enum class EMingNetworkConnectionState : uint8
{
    Disconnected      UMETA(DisplayName = "Disconnected"),
    Connecting        UMETA(DisplayName = "Connecting"),
    Connected         UMETA(DisplayName = "Connected"),
    Disconnecting     UMETA(DisplayName = "Disconnecting"),
    ConnectionFailed  UMETA(DisplayName = "Connection Failed"),
    Reconnecting      UMETA(DisplayName = "Reconnecting")
};

// Network game modes
UENUM(BlueprintType)
enum class EMingNetworkGameMode : uint8
{
    SinglePlayer      UMETA(DisplayName = "Single Player"),
    Cooperative       UMETA(DisplayName = "Cooperative"),
    Competitive       UMETA(DisplayName = "Competitive"),
    Campaign          UMETA(DisplayName = "Campaign"),
    Sandbox           UMETA(DisplayName = "Sandbox")
};

// Player roles in multiplayer
UENUM(BlueprintType)
enum class EMingPlayerRole : uint8
{
    Host              UMETA(DisplayName = "Host"),
    Client            UMETA(DisplayName = "Client"),
    Observer          UMETA(DisplayName = "Observer"),
    Spectator         UMETA(DisplayName = "Spectator")
};

// Network sync priorities
UENUM(BlueprintType)
enum class EMingSyncPriority : uint8
{
    Critical          UMETA(DisplayName = "Critical"),
    High              UMETA(DisplayName = "High"),
    Normal            UMETA(DisplayName = "Normal"),
    Low               UMETA(DisplayName = "Low"),
    Background        UMETA(DisplayName = "Background")
};

// Multiplayer player data
USTRUCT(BlueprintType)
struct FMingMultiplayerPlayerData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 PlayerID;

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly)
    EMingPlayerRole PlayerRole;

    UPROPERTY(BlueprintReadOnly)
    int32 Ping;

    UPROPERTY(BlueprintReadOnly)
    bool bIsReady;

    UPROPERTY(BlueprintReadOnly)
    bool bIsHost;

    UPROPERTY(BlueprintReadOnly)
    FDateTime JoinTime;

    FMingMultiplayerPlayerData()
        : PlayerID(0)
        , PlayerRole(EMingPlayerRole::Client)
        , Ping(0)
        , bIsReady(false)
        , bIsHost(false)
    {}
};

// Network configuration
USTRUCT(BlueprintType)
struct FMingNetworkConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ServerAddress;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ServerPort;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SyncInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ReconnectTimeout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxReconnectAttempts;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseCompression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseEncryption;

    FMingNetworkConfig()
        : ServerAddress(TEXT("127.0.0.1"))
        , ServerPort(7777)
        , MaxPlayers(8)
        , SyncInterval(0.1f)
        , ReconnectTimeout(30.0f)
        , MaxReconnectAttempts(3)
        , bUseCompression(true)
        , bUseEncryption(false)
    {}
};

// Network statistics
USTRUCT(BlueprintType)
struct FMingNetworkStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 BytesSent;

    UPROPERTY(BlueprintReadOnly)
    int32 BytesReceived;

    UPROPERTY(BlueprintReadOnly)
    int32 PacketsSent;

    UPROPERTY(BlueprintReadOnly)
    int32 PacketsReceived;

    UPROPERTY(BlueprintReadOnly)
    int32 PacketsLost;

    UPROPERTY(BlueprintReadOnly)
    float Latency;

    UPROPERTY(BlueprintReadOnly)
    float PacketLossRate;

    UPROPERTY(BlueprintReadOnly)
    int32 ActiveConnections;

    FMingNetworkStats()
        : BytesSent(0)
        , BytesReceived(0)
        , PacketsSent(0)
        , PacketsReceived(0)
        , PacketsLost(0)
        , Latency(0.0f)
        , PacketLossRate(0.0f)
        , ActiveConnections(0)
    {}
};

// Delegate declarations
declare dynamic multicast delegate(FOnNetworkConnected);
declare dynamic multicast_delegate(FOnNetworkDisconnected, const FString&, Reason);
declare dynamic_multicast_delegate(FOnNetworkConnectionFailed, const FString&, ErrorMessage);
declare dynamic_multicast_delegate(FOnPlayerJoined, int32, PlayerID, const FString&, PlayerName);
declare dynamic_multicast_delegate(FOnPlayerLeft, int32, PlayerID, const FString&, Reason);
declare dynamic_multicast_delegate(FOnNetworkStateChanged, EMingNetworkConnectionState, NewState);
declare dynamic_multicast_delegate(FOnHostMigrated, int32, NewHostPlayerID);

/**
 * 多人遊戲網絡管理器
 * 處理所有網絡連接、同步和多人遊戲邏輯
 */
UCLASS(ClassGroup = (Network), Blueprintable)
class MINGPERSONAL_API UMingNetworkManager : public UObject
{
    GENERATED_BODY()

public:
    UMingNetworkManager();

    // 初始化系統
    UFUNCTION(BlueprintCallable, Category = "Network")
    void InitializeNetworkManager(UMingPersonalManager* InPersonalManager);

    // 關閉系統
    UFUNCTION(BlueprintCallable, Category = "Network")
    void ShutdownNetworkManager();

    UFUNCTION(BlueprintPure, Category = "Network")
    bool IsNetworkInitialized() const { return bIsInitialized; }

    // === 連接管理 ===

    // 創建服務器（作為主機）
    UFUNCTION(BlueprintCallable, Category = "Network|Connection")
    bool CreateServer(const FMingNetworkConfig& Config);

    // 連接到服務器
    UFUNCTION(BlueprintCallable, Category = "Network|Connection")
    bool ConnectToServer(const FString& ServerAddress, int32 Port);

    // 斷開連接
    UFUNCTION(BlueprintCallable, Category = "Network|Connection")
    void DisconnectFromServer();

    // 重新連接
    UFUNCTION(BlueprintCallable, Category = "Network|Connection")
    bool ReconnectToServer();

    // === 狀態查詢 ===

    UFUNCTION(BlueprintPure, Category = "Network|State")
    EMingNetworkConnectionState GetConnectionState() const { return ConnectionState; }

    UFUNCTION(BlueprintPure, Category = "Network|State")
    bool IsConnected() const { return ConnectionState == EMingNetworkConnectionState::Connected; }

    UFUNCTION(BlueprintPure, Category = "Network|State")
    bool IsHost() const;

    UFUNCTION(BlueprintPure, Category = "Network|State")
    int32 GetLocalPlayerID() const { return LocalPlayerID; }

    // === 玩家管理 ===

    UFUNCTION(BlueprintCallable, Category = "Network|Players")
    void SetPlayerReady(bool bReady);

    UFUNCTION(BlueprintPure, Category = "Network|Players")
    TArray<FMingMultiplayerPlayerData> GetAllPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Network|Players")
    FMingMultiplayerPlayerData GetPlayerData(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "Network|Players")
    int32 GetPlayerCount() const;

    // === 遊戲模式 ===

    UFUNCTION(BlueprintCallable, Category = "Network|Game Mode")
    void SetGameMode(EMingNetworkGameMode GameMode);

    UFUNCTION(BlueprintPure, Category = "Network|Game Mode")
    EMingNetworkGameMode GetGameMode() const { return CurrentGameMode; }

    // === 數據同步 ===

    // 請求完整同步
    UFUNCTION(BlueprintCallable, Category = "Network|Sync")
    void RequestFullSync();

    // 啟用/禁用自動同步
    UFUNCTION(BlueprintCallable, Category = "Network|Sync")
    void SetAutoSyncEnabled(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "Network|Sync")
    bool IsAutoSyncEnabled() const { return bAutoSyncEnabled; }

    // 手動觸發同步
    UFUNCTION(BlueprintCallable, Category = "Network|Sync")
    void ForceSync();

    // === 網絡統計 ===

    UFUNCTION(BlueprintPure, Category = "Network|Stats")
    FMingNetworkStats GetNetworkStats() const { return NetworkStats; }

    UFUNCTION(BlueprintCallable, Category = "Network|Stats")
    void ResetNetworkStats();

    // === 事件 ===

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnNetworkConnected OnNetworkConnected;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnNetworkDisconnected OnNetworkDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnNetworkConnectionFailed OnNetworkConnectionFailed;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnPlayerJoined OnPlayerJoined;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnPlayerLeft OnPlayerLeft;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnNetworkStateChanged OnNetworkStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Network|Events")
    FOnHostMigrated OnHostMigrated;

    // === 內部方法（Blueprint不可見） ===

    // 處理連接成功
    void HandleConnectionSuccess();

    // 處理連接失敗
    void HandleConnectionFailed(const FString& ErrorMessage);

    // 處理斷開連接
    void HandleDisconnection(const FString& Reason);

    // 處理玩家加入
    void HandlePlayerJoined(int32 PlayerID, const FString& PlayerName);

    // 處理玩家離開
    void HandlePlayerLeft(int32 PlayerID, const FString& Reason);

    // 處理主機遷移
    void HandleHostMigration(int32 NewHostPlayerID);

    // 更新網絡統計
    void UpdateNetworkStats();

    // 獲取關係複製組件
    UFUNCTION(BlueprintPure, Category = "Network|Replication")
    UMingRelationshipReplication* GetRelationshipReplication() const;

    // 獲取聲望複製組件
    UFUNCTION(BlueprintPure, Category = "Network|Replication")
    UMingReputationReplication* GetReputationReplication() const;

protected:
    // 設置連接狀態
    void SetConnectionState(EMingNetworkConnectionState NewState);

    // 開始重連
    void StartReconnect();

    // 停止重連
    void StopReconnect();

    // 重連計時器回調
    void OnReconnectTimer();

    // 同步計時器回調
    void OnSyncTimer();

    // 初始化複製組件
    void InitializeReplicationComponents();

    // 關閉複製組件
    void ShutdownReplicationComponents();

private:
    // 初始化狀態
    UPROPERTY()
    bool bIsInitialized;

    // 連接狀態
    UPROPERTY()
    EMingNetworkConnectionState ConnectionState;

    // 當前遊戲模式
    UPROPERTY()
    EMingNetworkGameMode CurrentGameMode;

    // 本地玩家ID
    UPROPERTY()
    int32 LocalPlayerID;

    // 網絡配置
    UPROPERTY()
    FMingNetworkConfig NetworkConfig;

    // 網絡統計
    UPROPERTY()
    FMingNetworkStats NetworkStats;

    // 玩家數據
    UPROPERTY()
    TArray<FMingMultiplayerPlayerData> PlayerList;

    // 自動同步設置
    UPROPERTY()
    bool bAutoSyncEnabled;

    // 重連相關
    UPROPERTY()
    int32 CurrentReconnectAttempt;

    UPROPERTY()
    float ReconnectTimer;

    // PersonalManager 引用
    UPROPERTY()
    TObjectPtr<UMingPersonalManager> PersonalManager;

    // 關係複製組件
    UPROPERTY()
    TObjectPtr<UMingRelationshipReplication> RelationshipReplication;

    // 聲望複製組件
    UPROPERTY()
    TObjectPtr<UMingReputationReplication> ReputationReplication;

    // 靜態實例訪問
public:
    UFUNCTION(BlueprintPure, Category = "Network", meta = (WorldContext = "WorldContextObject"))
    static UMingNetworkManager* GetNetworkManager(UObject* WorldContextObject);
};
