#include "Network/MingNetworkManager.h"
#include "Network/MingRelationshipReplication.h"
#include "Network/MingReputationReplication.h"
#include "MingPersonalManager.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

UMingNetworkManager::UMingNetworkManager()
    : bIsInitialized(false)
    , ConnectionState(EMingNetworkConnectionState::Disconnected)
    , CurrentGameMode(EMingNetworkGameMode::SinglePlayer)
    , LocalPlayerID(0)
    , bAutoSyncEnabled(true)
    , CurrentReconnectAttempt(0)
    , ReconnectTimer(0.0f)
    , PersonalManager(nullptr)
    , RelationshipReplication(nullptr)
    , ReputationReplication(nullptr)
{
}

void UMingNetworkManager::InitializeNetworkManager(UMingPersonalManager* InPersonalManager)
    {
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("NetworkManager already initialized"));
        return;
    }

    PersonalManager = InPersonalManager;

    // 初始化複製組件
    InitializeReplicationComponents();

    bIsInitialized = true;
    SetConnectionState(EMingNetworkConnectionState::Disconnected);

    UE_LOG(LogTemp, Log, TEXT("NetworkManager initialized"));
}

void UMingNetworkManager::ShutdownNetworkManager()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 斷開連接
    if (IsConnected())
    {
        DisconnectFromServer();
    }

    // 關閉複製組件
    ShutdownReplicationComponents();

    bIsInitialized = false;
    UE_LOG(LogTemp, Log, TEXT("NetworkManager shutdown"));
}

bool UMingNetworkManager::CreateServer(const FMingNetworkConfig& Config)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("NetworkManager not initialized"));
        return false;
    }

    if (IsConnected())
    {
        UE_LOG(LogTemp, Warning, TEXT("Already connected to a server"));
        return false;
    }

    NetworkConfig = Config;
    CurrentGameMode = EMingNetworkGameMode::Cooperative;

    // 設置為主機
    LocalPlayerID = 1;

    // 添加自己到玩家列表
    FMingMultiplayerPlayerData HostData;
    HostData.PlayerID = LocalPlayerID;
    HostData.PlayerName = TEXT("Host");
    HostData.PlayerRole = EMingPlayerRole::Host;
    HostData.bIsHost = true;
    HostData.bIsReady = true;
    HostData.JoinTime = FDateTime::Now();
    PlayerList.Add(HostData);

    SetConnectionState(EMingNetworkConnectionState::Connected);

    UE_LOG(LogTemp, Log, TEXT("Server created on port %d, max players: %d"), 
        NetworkConfig.ServerPort, NetworkConfig.MaxPlayers);

    OnNetworkConnected.Broadcast();
    return true;
}

bool UMingNetworkManager::ConnectToServer(const FString& ServerAddress, int32 Port)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("NetworkManager not initialized"));
        return false;
    }

    if (IsConnected())
    {
        UE_LOG(LogTemp, Warning, TEXT("Already connected to a server"));
        return false;
    }

    SetConnectionState(EMingNetworkConnectionState::Connecting);

    NetworkConfig.ServerAddress = ServerAddress;
    NetworkConfig.ServerPort = Port;

    // 模擬連接過程（實際實現會使用UE的網絡功能）
    // 這裡我們假設連接成功

    // 生成隨機玩家ID（實際應該由服務器分配）
    LocalPlayerID = FMath::RandRange(2, 1000);

    // 添加自己到玩家列表
    FMingMultiplayerPlayerData ClientData;
    ClientData.PlayerID = LocalPlayerID;
    ClientData.PlayerName = FString::Printf(TEXT("Player %d"), LocalPlayerID);
    ClientData.PlayerRole = EMingPlayerRole::Client;
    ClientData.bIsHost = false;
    ClientData.bIsReady = false;
    ClientData.JoinTime = FDateTime::Now();
    PlayerList.Add(ClientData);

    SetConnectionState(EMingNetworkConnectionState::Connected);

    UE_LOG(LogTemp, Log, TEXT("Connected to server %s:%d as player %d"), 
        *ServerAddress, Port, LocalPlayerID);

    // 請求完整同步
    RequestFullSync();

    OnNetworkConnected.Broadcast();
    return true;
}

void UMingNetworkManager::DisconnectFromServer()
{
    if (!IsConnected())
    {
        return;
    }

    SetConnectionState(EMingNetworkConnectionState::Disconnecting);

    // 清理玩家列表
    PlayerList.Empty();
    LocalPlayerID = 0;

    SetConnectionState(EMingNetworkConnectionState::Disconnected);

    UE_LOG(LogTemp, Log, TEXT("Disconnected from server"));

    OnNetworkDisconnected.Broadcast(TEXT("Manual disconnect"));
}

bool UMingNetworkManager::ReconnectToServer()
{
    if (IsConnected())
    {
        UE_LOG(LogTemp, Warning, TEXT("Already connected"));
        return true;
    }

    if (NetworkConfig.ServerAddress.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("No server address configured for reconnect"));
        return false;
    }

    SetConnectionState(EMingNetworkConnectionState::Reconnecting);

    UE_LOG(LogTemp, Log, TEXT("Attempting to reconnect... (Attempt %d/%d)"), 
        CurrentReconnectAttempt + 1, NetworkConfig.MaxReconnectAttempts);

    return ConnectToServer(NetworkConfig.ServerAddress, NetworkConfig.ServerPort);
}

bool UMingNetworkManager::IsHost() const
{
    if (!IsConnected())
    {
        return false;
    }

    for (const FMingMultiplayerPlayerData& Player : PlayerList)
    {
        if (Player.PlayerID == LocalPlayerID)
        {
            return Player.bIsHost;
        }
    }

    return false;
}

void UMingNetworkManager::SetPlayerReady(bool bReady)
{
    if (!IsConnected())
    {
        return;
    }

    for (FMingMultiplayerPlayerData& Player : PlayerList)
    {
        if (Player.PlayerID == LocalPlayerID)
        {
            Player.bIsReady = bReady;
            UE_LOG(LogTemp, Log, TEXT("Player %d ready state: %s"), 
                LocalPlayerID, bReady ? TEXT("Ready") : TEXT("Not Ready"));
            break;
        }
    }

    // 同步到其他玩家
    if (RelationshipReplication)
    {
        RelationshipReplication->SyncPlayerReadyState(LocalPlayerID, bReady);
    }
}

TArray<FMingMultiplayerPlayerData> UMingNetworkManager::GetAllPlayers() const
{
    return PlayerList;
}

FMingMultiplayerPlayerData UMingNetworkManager::GetPlayerData(int32 PlayerID) const
{
    for (const FMingMultiplayerPlayerData& Player : PlayerList)
    {
        if (Player.PlayerID == PlayerID)
        {
            return Player;
        }
    }

    return FMingMultiplayerPlayerData();
}

int32 UMingNetworkManager::GetPlayerCount() const
{
    return PlayerList.Num();
}

void UMingNetworkManager::SetGameMode(EMingNetworkGameMode GameMode)
{
    if (!IsHost())
    {
        UE_LOG(LogTemp, Warning, TEXT("Only host can change game mode"));
        return;
    }

    CurrentGameMode = GameMode;

    UE_LOG(LogTemp, Log, TEXT("Game mode changed to %d"), static_cast<int32>(GameMode));

    // 同步到其他玩家
    if (RelationshipReplication)
    {
        RelationshipReplication->SyncGameMode(GameMode);
    }
}

void UMingNetworkManager::RequestFullSync()
{
    if (!IsConnected())
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Requesting full sync from server"));

    // 觸發關係數據同步
    if (RelationshipReplication)
    {
        RelationshipReplication->RequestFullSync();
    }

    // 觸發聲望數據同步
    if (ReputationReplication)
    {
        ReputationReplication->RequestFullSync();
    }
}

void UMingNetworkManager::SetAutoSyncEnabled(bool bEnabled)
{
    bAutoSyncEnabled = bEnabled;

    UE_LOG(LogTemp, Log, TEXT("Auto sync %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingNetworkManager::ForceSync()
{
    if (!IsConnected())
    {
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Forcing manual sync"));

    // 手動觸發同步
    if (RelationshipReplication)
    {
        RelationshipReplication->ForceSync();
    }

    if (ReputationReplication)
    {
        ReputationReplication->ForceSync();
    }
}

void UMingNetworkManager::ResetNetworkStats()
{
    NetworkStats = FMingNetworkStats();
    UE_LOG(LogTemp, Log, TEXT("Network stats reset"));
}

void UMingNetworkManager::HandleConnectionSuccess()
{
    UE_LOG(LogTemp, Log, TEXT("Network connection successful"));
    SetConnectionState(EMingNetworkConnectionState::Connected);
    CurrentReconnectAttempt = 0;
    OnNetworkConnected.Broadcast();
}

void UMingNetworkManager::HandleConnectionFailed(const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("Network connection failed: %s"), *ErrorMessage);
    SetConnectionState(EMingNetworkConnectionState::ConnectionFailed);

    // 嘗試重連
    if (CurrentReconnectAttempt < NetworkConfig.MaxReconnectAttempts)
    {
        CurrentReconnectAttempt++;
        StartReconnect();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Max reconnection attempts reached"));
        OnNetworkConnectionFailed.Broadcast(ErrorMessage);
    }
}

void UMingNetworkManager::HandleDisconnection(const FString& Reason)
{
    UE_LOG(LogTemp, Warning, TEXT("Network disconnected: %s"), *Reason);

    if (ConnectionState != EMingNetworkConnectionState::Disconnecting)
    {
        SetConnectionState(EMingNetworkConnectionState::Disconnected);

        // 嘗試自動重連
        if (NetworkConfig.MaxReconnectAttempts > 0 && CurrentReconnectAttempt < NetworkConfig.MaxReconnectAttempts)
        {
            CurrentReconnectAttempt++;
            StartReconnect();
        }
        else
        {
            OnNetworkDisconnected.Broadcast(Reason);
        }
    }
}

void UMingNetworkManager::HandlePlayerJoined(int32 PlayerID, const FString& PlayerName)
    {
    UE_LOG(LogTemp, Log, TEXT("Player joined: %s (ID: %d)"), *PlayerName, PlayerID);

    // 檢查玩家是否已存在
    for (const FMingMultiplayerPlayerData& ExistingPlayer : PlayerList)
    {
        if (ExistingPlayer.PlayerID == PlayerID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Player %d already exists"), PlayerID);
            return;
        }
    }

    FMingMultiplayerPlayerData NewPlayer;
    NewPlayer.PlayerID = PlayerID;
    NewPlayer.PlayerName = PlayerName;
    NewPlayer.PlayerRole = EMingPlayerRole::Client;
    NewPlayer.bIsHost = false;
    NewPlayer.bIsReady = false;
    NewPlayer.JoinTime = FDateTime::Now();
    PlayerList.Add(NewPlayer);

    OnPlayerJoined.Broadcast(PlayerID, PlayerName);
}

void UMingNetworkManager::HandlePlayerLeft(int32 PlayerID, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("Player left: %d, Reason: %s"), PlayerID, *Reason);

    // 從玩家列表移除
    for (int32 i = 0; i < PlayerList.Num(); i++)
    {
        if (PlayerList[i].PlayerID == PlayerID)
        {
            PlayerList.RemoveAt(i);
            break;
        }
    }

    OnPlayerLeft.Broadcast(PlayerID, Reason);
}

void UMingNetworkManager::HandleHostMigration(int32 NewHostPlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Host migrated to player %d"), NewHostPlayerID);

    // 更新主機狀態
    for (FMingMultiplayerPlayerData& Player : PlayerList)
    {
        Player.bIsHost = (Player.PlayerID == NewHostPlayerID);
        if (Player.bIsHost)
        {
            Player.PlayerRole = EMingPlayerRole::Host;
        }
    }

    OnHostMigrated.Broadcast(NewHostPlayerID);
}

void UMingNetworkManager::UpdateNetworkStats()
{
    // 更新網絡統計數據
    // 實際實現會從UE的網絡層獲取數據

    // 計算包丟失率
    if (NetworkStats.PacketsSent > 0)
    {
        NetworkStats.PacketLossRate = static_cast<float>(NetworkStats.PacketsLost) / NetworkStats.PacketsSent;
    }

    // 更新活動連接數
    NetworkStats.ActiveConnections = PlayerList.Num();
}

void UMingNetworkManager::SetConnectionState(EMingNetworkConnectionState NewState)
{
    if (ConnectionState != NewState)
    {
        EMingNetworkConnectionState OldState = ConnectionState;
        ConnectionState = NewState;

        UE_LOG(LogTemp, Log, TEXT("Network state changed from %d to %d"), 
            static_cast<int32>(OldState), static_cast<int32>(NewState));

        OnNetworkStateChanged.Broadcast(NewState);
    }
}

void UMingNetworkManager::StartReconnect()
{
    if (ConnectionState == EMingNetworkConnectionState::Reconnecting)
    {
        return;
    }

    SetConnectionState(EMingNetworkConnectionState::Reconnecting);
    ReconnectTimer = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Starting reconnection process..."));

    // 延遲後嘗試重連
    FTimerHandle ReconnectTimerHandle;
    GetWorld()->GetTimerManager().SetTimer(ReconnectTimerHandle, this, 
        &UMingNetworkManager::OnReconnectTimer, 1.0f, false);
}

void UMingNetworkManager::StopReconnect()
{
    CurrentReconnectAttempt = 0;
    ReconnectTimer = 0.0f;
    UE_LOG(LogTemp, Log, TEXT("Reconnection stopped"));
}

void UMingNetworkManager::OnReconnectTimer()
{
    ReconnectTimer += 1.0f;

    if (ReconnectTimer >= NetworkConfig.ReconnectTimeout)
    {
        UE_LOG(LogTemp, Warning, TEXT("Reconnect timeout"));
        StopReconnect();
        return;
    }

    // 嘗試重連
    if (ReconnectToServer())
    {
        UE_LOG(LogTemp, Log, TEXT("Reconnection successful"));
        StopReconnect();
    }
}

void UMingNetworkManager::OnSyncTimer()
{
    if (!IsConnected() || !bAutoSyncEnabled)
    {
        return;
    }

    // 自動同步數據
    ForceSync();
}

void UMingNetworkManager::InitializeReplicationComponents()
{
    // 創建關係複製組件
    RelationshipReplication = NewObject<UMingRelationshipReplication>(this);
    if (RelationshipReplication)
    {
        RelationshipReplication->InitializeReplication(this, PersonalManager);
    }

    // 創建聲望複製組件
    ReputationReplication = NewObject<UMingReputationReplication>(this);
    if (ReputationReplication)
    {
        ReputationReplication->InitializeReplication(this, PersonalManager);
    }
}

void UMingNetworkManager::ShutdownReplicationComponents()
{
    if (RelationshipReplication)
    {
        RelationshipReplication->ShutdownReplication();
        RelationshipReplication = nullptr;
    }

    if (ReputationReplication)
    {
        ReputationReplication->ShutdownReplication();
        ReputationReplication = nullptr;
    }
}

UMingRelationshipReplication* UMingNetworkManager::GetRelationshipReplication() const
{
    return RelationshipReplication;
}

UMingReputationReplication* UMingNetworkManager::GetReputationReplication() const
{
    return ReputationReplication;
}

UMingNetworkManager* UMingNetworkManager::GetNetworkManager(UObject* WorldContextObject)
{
    if (!WorldContextObject)
    {
        return nullptr;
    }

    // 從遊戲實例獲取網絡管理器
    // 實際實現需要根據項目架構調整
    UWorld* World = WorldContextObject->GetWorld();
    if (!World)
    {
        return nullptr;
    }

    // 這裡應該從GameInstance或其他全局管理器獲取
    // 暫時返回nullptr，實際項目需要實現
    return nullptr;
}
