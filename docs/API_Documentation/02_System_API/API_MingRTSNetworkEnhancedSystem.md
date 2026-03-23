# MingRTSNetworkEnhancedSystem 系統文檔 / MingRTSNetworkEnhancedSystem System Documentation

## 概述 / Overview

MingRTSNetworkEnhancedSystem 是一個增強型網絡系統，提供完整的網絡遊戲架構，支持多種網絡類型、連接狀態管理、數據同步和性能優化。

MingRTSNetworkEnhancedSystem is an enhanced networking system providing a complete online gaming architecture, supporting multiple network types, connection state management, data synchronization, and performance optimization.

## 核心功能 / Core Features

### 1. 網絡類型 / Network Types
支持10種網絡類型 / Supports 10 network types：
- **LAN**: 局域網 / Local Area Network - 本地多人遊戲 / Local multiplayer
- **WAN**: 廣域網 / Wide Area Network - 互聯網連接 / Internet connection
- **Internet**: 互聯網 / Internet - 標準在線遊戲 / Standard online gaming
- **Local**: 本地 / Local - 單機模擬網絡 / Single-player simulated network
- **DedicatedServer**: 專用服務器 / Dedicated Server - 專業託管環境 / Professional hosting
- **ListenServer**: 監聽服務器 / Listen Server - 主機端兼任服務器 / Host-as-server
- **PeerToPeer**: 點對點 / Peer to Peer - 玩家直接連接 / Direct player connection
- **Hybrid**: 混合 / Hybrid - 組合模式 / Combined mode
- **Cloud**: 雲端 / Cloud - 雲服務託管 / Cloud hosting
- **Custom**: 自定義 / Custom - 特殊配置 / Special configuration

### 2. 連接狀態 / Connection States
11種連接狀態 / 11 connection states：
- **Disconnected**: 已斷開 / Disconnected
- **Connecting**: 連接中 / Connecting
- **Connected**: 已連接 / Connected
- **Authenticating**: 認證中 / Authenticating
- **Authenticated**: 已認證 / Authenticated
- **Loading**: 加載中 / Loading
- **Ready**: 就緒 / Ready
- **Error**: 錯誤 / Error
- **Timeout**: 超時 / Timeout
- **Kicked**: 被踢出 / Kicked
- **Banned**: 被封禁 / Banned

### 3. 遊戲模式 / Game Modes
11種遊戲模式 / 11 game modes：
- **SinglePlayer**: 單人遊戲 / Single Player
- **Cooperative**: 合作模式 / Cooperative
- **Versus**: 對抗模式 / Versus
- **TeamVersus**: 團隊對抗 / Team Versus
- **FreeForAll**: 混戰模式 / Free For All
- **Campaign**: 戰役模式 / Campaign
- **Skirmish**: 遭遇戰 / Skirmish
- **Tournament**: 錦標賽 / Tournament
- **Survival**: 生存模式 / Survival
- **Sandbox**: 沙盒模式 / Sandbox
- **Custom**: 自定義 / Custom

### 4. 同步模式 / Sync Modes
10種同步模式 / 10 synchronization modes：
- **Full**: 完全同步 / Full Sync - 所有數據 / All data
- **Partial**: 部分同步 / Partial Sync - 選擇性數據 / Selective data
- **Delta**: 增量同步 / Delta Sync - 僅變化數據 / Only changed data
- **Predictive**: 預測同步 / Predictive Sync - 預測性更新 / Predictive updates
- **Deferred**: 延遲同步 / Deferred Sync - 延遲傳輸 / Delayed transmission
- **Event**: 事件同步 / Event Sync - 僅事件 / Events only
- **Periodic**: 定期同步 / Periodic Sync - 定時更新 / Periodic updates
- **OnDemand**: 按需同步 / On-Demand Sync - 請求時更新 / On request
- **Hybrid**: 混合同步 / Hybrid Sync - 組合模式 / Combined mode
- **Custom**: 自定義同步 / Custom Sync

## API 參考 / API Reference

### 系统初始化

```cpp
// 初始化网络系统
void InitializeNetworkSystem();

// 关闭网络系统
void ShutdownNetworkSystem();

// 获取系统状态
bool IsInitialized() const;
```

### 服务器管理

```cpp
// 创建服务器
bool CreateServer(ENetworkType NetworkType, const FServerConfig& Config);

// 启动服务器
bool StartServer(int32 Port, int32 MaxPlayers);

// 停止服务器
void StopServer();

// 关闭服务器
void ShutdownServer();

// 获取服务器信息
FServerInfo GetServerInfo() const;

// 服务器是否运行中
bool IsServerRunning() const;
```

### 客户端连接

```cpp
// 连接到服务器
bool ConnectToServer(const FString& ServerAddress, int32 Port, const FString& Password);

// 断开连接
void Disconnect();

// 重新连接
bool Reconnect();

// 获取连接状态
ENetworkConnectionStatus GetConnectionStatus() const;

// 是否已连接
bool IsConnected() const;

// 获取连接质量
float GetConnectionQuality() const;
```

### 玩家管理

```cpp
// 获取本地玩家ID
FString GetLocalPlayerID() const;

// 获取玩家信息
FNetworkPlayer GetPlayerInfo(const FString& PlayerID) const;

// 获取所有玩家
TArray<FNetworkPlayer> GetAllPlayers() const;

// 获取玩家数量
int32 GetPlayerCount() const;

// 踢出玩家
void KickPlayer(const FString& PlayerID, const FString& Reason);

// 封禁玩家
void BanPlayer(const FString& PlayerID, const FString& Reason, float Duration);

// 设置玩家权限
void SetPlayerAdmin(const FString& PlayerID, bool bIsAdmin);
```

### 会话管理

```cpp
// 创建会话
FString CreateSession(const FSessionConfig& Config);

// 加入会话
bool JoinSession(const FString& SessionID, const FString& Password);

// 离开会话
void LeaveSession();

// 获取当前会话
FNetworkSession GetCurrentSession() const;

// 查找可用会话
TArray<FNetworkSession> FindSessions(const FSessionSearchParams& SearchParams);

// 销毁会话
void DestroySession(const FString& SessionID);
```

### 数据同步

```cpp
// 注册同步对象
void RegisterSyncObject(UObject* Object, ENetworkSyncMode SyncMode);

// 取消注册同步对象
void UnregisterSyncObject(UObject* Object);

// 设置同步属性
void SetReplicatedProperty(UObject* Object, const FString& PropertyName, bool bReplicated);

// 触发同步
void ForceReplication(UObject* Object);

// 设置同步间隔
void SetSyncInterval(UObject* Object, float Interval);
```

### 消息传递

```cpp
// 发送RPC调用
void SendRPC(const FString& TargetPlayerID, const FString& FunctionName, 
    const TArray<FString>& Parameters);

// 广播RPC
void BroadcastRPC(const FString& FunctionName, const TArray<FString>& Parameters);

// 发送可靠消息
void SendReliableMessage(const FString& TargetPlayerID, const FString& Message);

// 发送不可靠消息
void SendUnreliableMessage(const FString& TargetPlayerID, const FString& Message);
```

### 网络监控

```cpp
// 获取网络统计
FNetworkStatistics GetNetworkStatistics() const;

// 获取延迟
float GetLatency() const;

// 获取丢包率
float GetPacketLoss() const;

// 获取带宽使用
float GetBandwidthUsage() const;

// 重置统计
void ResetStatistics();
```

### 延迟补偿

```cpp
// 设置延迟补偿
void SetLagCompensation(bool bEnabled);

// 获取服务器时间
float GetServerTime() const;

// 同步客户端时间
void SyncClientTime();

// 预测玩家位置
FVector PredictPlayerPosition(const FString& PlayerID, float PredictionTime);
```

## 数据结构

### FServerConfig
```cpp
struct FServerConfig
{
    FString ServerName;                 // 服务器名称
    FString GameMode;                   // 游戏模式
    int32 MaxPlayers;                   // 最大玩家数
    FString MapName;                    // 地图名称
    FString Password;                   // 密码
    bool bIsPrivate;                    // 是否私有
    int32 TickRate;                     // 服务器tick率
    TArray<FString> ModList;            // 模组列表
};
```

### FServerInfo
```cpp
struct FServerInfo
{
    FString ServerID;                   // 服务器ID
    FString ServerName;                 // 服务器名称
    FString GameMode;                   // 游戏模式
    int32 CurrentPlayers;               // 当前玩家数
    int32 MaxPlayers;                   // 最大玩家数
    FString MapName;                    // 地图名称
    float Uptime;                       // 运行时间
    float AverageLatency;               // 平均延迟
    float ServerFPS;                    // 服务器帧率
    bool bIsPasswordProtected;          // 是否有密码
    ENetworkType NetworkType;           // 网络类型
};
```

### FNetworkPlayer
```cpp
struct FNetworkPlayer
{
    FString PlayerID;                   // 玩家ID
    FString PlayerName;                 // 玩家名称
    ENetworkConnectionStatus Status;    // 连接状态
    float Latency;                      // 延迟
    int32 Ping;                         // Ping值
    bool bIsAdmin;                      // 是否管理员
    bool bIsReady;                      // 是否就绪
    int32 TeamID;                       // 队伍ID
    FVector Location;                   // 位置
    float JoinTime;                     // 加入时间
};
```

### FNetworkSession
```cpp
struct FNetworkSession
{
    FString SessionID;                  // 会话ID
    FString SessionName;                // 会话名称
    FString HostPlayerID;               // 主机玩家ID
    int32 CurrentPlayers;               // 当前玩家数
    int32 MaxPlayers;                   // 最大玩家数
    EGameMode GameMode;                 // 游戏模式
    ENetworkConnectionStatus Status;    // 会话状态
    FString MapName;                    // 地图名称
    bool bIsPasswordProtected;          // 是否有密码
    int32 Ping;                         // Ping值
};
```

### FNetworkStatistics
```cpp
struct FNetworkStatistics
{
    float TotalBytesSent;               // 发送总字节
    float TotalBytesReceived;           // 接收总字节
    float PacketsSent;                  // 发送包数
    float PacketsReceived;              // 接收包数
    float PacketsLost;                  // 丢包数
    float AverageLatency;               // 平均延迟
    float PeakLatency;                  // 峰值延迟
    float Jitter;                       // 抖动
    float BandwidthIn;                  // 入站带宽
    float BandwidthOut;                 // 出站带宽
    float CompressionRatio;             // 压缩率
};
```

### FSessionConfig
```cpp
struct FSessionConfig
{
    FString SessionName;                // 会话名称
    EGameMode GameMode;                 // 游戏模式
    int32 MaxPlayers;                   // 最大玩家数
    FString MapName;                    // 地图名称
    FString Password;                   // 密码
    ENetworkSyncMode SyncMode;        // 同步模式
    int32 TickRate;                     // Tick率
    bool bAllowJoinInProgress;          // 允许中途加入
};
```

## 事件委托

```cpp
// 玩家连接
FOnPlayerConnected OnPlayerConnected;

// 玩家断开
FOnPlayerDisconnected OnPlayerDisconnected;

// 连接状态改变
FOnConnectionStatusChanged OnConnectionStatusChanged;

// 收到消息
FOnNetworkMessage OnNetworkMessage;

// 数据同步
FOnDataReplicated OnDataReplicated;
```

## 使用示例

### 启动服务器
```cpp
UMingRTSNetworkEnhancedSystem* Network = NewObject<UMingRTSNetworkEnhancedSystem>();
Network->InitializeNetworkSystem();

FServerConfig Config;
Config.ServerName = TEXT("My RTS Server");
Config.GameMode = TEXT("Versus");
Config.MaxPlayers = 8;
Config.MapName = TEXT("Plains");
Config.TickRate = 60;

if (Network->CreateServer(ENetworkType::Internet, Config))
{
    Network->StartServer(7777, 8);
    UE_LOG(LogTemp, Log, TEXT("Server started on port 7777"));
}
```

### 客户端连接
```cpp
// 绑定事件
Network->OnPlayerConnected.AddDynamic(this, &AMyGameMode::OnPlayerJoined);
Network->OnConnectionStatusChanged.AddDynamic(this, &AMyGameMode::OnConnectionChanged);

// 连接到服务器
if (Network->ConnectToServer(TEXT("192.168.1.100"), 7777, TEXT("")))
{
    UE_LOG(LogTemp, Log, TEXT("Connecting to server..."));
}

void AMyGameMode::OnPlayerJoined(const FString& PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player joined: %s"), *PlayerID);
}

void AMyGameMode::OnConnectionChanged(ENetworkConnectionStatus NewStatus)
{
    if (NewStatus == ENetworkConnectionStatus::Connected)
    {
        UE_LOG(LogTemp, Log, TEXT("Connected to server!"));
    }
}
```

### 数据同步
```cpp
// 注册需要同步的对象
MyUnit->SetReplicates(true);
Network->RegisterSyncObject(MyUnit, ENetworkSyncMode::Delta);

// 设置同步间隔
Network->SetSyncInterval(MyUnit, 0.033f);  // 30 times per second

// 强制同步
Network->ForceReplication(MyUnit);
```

### 发送RPC
```cpp
// 发送RPC到特定玩家
Network->SendRPC(TEXT("Player2"), TEXT("TakeDamage"), 
    {TEXT("50"), TEXT("Explosion")});

// 广播RPC到所有玩家
Network->BroadcastRPC(TEXT("PlaySound"), {TEXT("Explosion")});
```

### 监控网络
```cpp
// 获取网络统计
FNetworkStatistics Stats = Network->GetNetworkStatistics();

UE_LOG(LogTemp, Log, TEXT("Network Stats:"));
UE_LOG(LogTemp, Log, TEXT("  Latency: %.2f ms"), Stats.AverageLatency);
UE_LOG(LogTemp, Log, TEXT("  Packet Loss: %.2f%%"), (Stats.PacketsLost / Stats.PacketsSent) * 100.0f);
UE_LOG(LogTemp, Log, TEXT("  Bandwidth: %.2f KB/s"), (Stats.BandwidthIn + Stats.BandwidthOut) / 1024.0f);
```

### 延迟补偿
```cpp
// 启用延迟补偿
Network->SetLagCompensation(true);

// 预测玩家位置进行射击
FVector PredictedPosition = Network->PredictPlayerPosition(TEXT("TargetPlayer"), 0.1f);
FireAt(PredictedPosition);
```

## 性能考虑

- **Tick率**: 建议60 tick/s用于竞技游戏，30 tick/s用于休闲游戏
- **同步频率**: 根据对象重要性调整，玩家单位30-60次/秒，建筑5-10次/秒
- **带宽优化**: 使用增量同步和压缩减少带宽使用
- **延迟补偿**: 对于射击类游戏，启用延迟补偿提高准确性

## 最佳实践

1. **连接恢复**: 实现断线重连机制
2. **错误处理**: 妥善处理连接错误和超时
3. **安全检查**: 服务器端验证所有客户端输入
4. **优化同步**: 只同步必要数据
5. **带宽管理**: 监控和限制带宽使用
6. **作弊防护**: 实施反作弊措施

## 扩展性

系统支持以下扩展：
- 自定义网络协议
- 第三方反作弊集成
- 云服务商集成（AWS、Azure等）
- WebSocket支持
- 专用服务器托管
