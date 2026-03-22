# MingRTSNetworkEnhancedSystem 系统文档

## 概述

MingRTSNetworkEnhancedSystem 是一个增强型网络系统，提供完整的网络游戏架构，支持多种网络类型、连接状态管理、数据同步和性能优化。

## 核心功能

### 1. 网络类型
支持10种网络类型：
- **LAN**: 局域网 - 本地多人游戏
- **WAN**: 广域网 - 互联网连接
- **Internet**: 互联网 - 标准在线游戏
- **Local**: 本地 - 单机模拟网络
- **DedicatedServer**: 专用服务器 - 专业托管环境
- **ListenServer**: 监听服务器 - 主机端兼任服务器
- **PeerToPeer**: 点对点 - 玩家直接连接
- **Hybrid**: 混合 - 组合模式
- **Cloud**: 云端 - 云服务托管
- **Custom**: 自定义 - 特殊配置

### 2. 连接状态
11种连接状态：
- **Disconnected**: 已断开
- **Connecting**: 连接中
- **Connected**: 已连接
- **Authenticating**: 认证中
- **Authenticated**: 已认证
- **Loading**: 加载中
- **Ready**: 就绪
- **Error**: 错误
- **Timeout**: 超时
- **Kicked**: 被踢出
- **Banned**: 被封禁

### 3. 游戏模式
11种游戏模式：
- **SinglePlayer**: 单人游戏
- **Cooperative**: 合作模式
- **Versus**: 对抗模式
- **TeamVersus**: 团队对抗
- **FreeForAll**: 混战模式
- **Campaign**: 战役模式
- **Skirmish**: 遭遇战
- **Tournament**: 锦标赛
- **Survival**: 生存模式
- **Sandbox**: 沙盒模式
- **Custom**: 自定义

### 4. 同步模式
10种同步模式：
- **Full**: 完全同步 - 所有数据
- **Partial**: 部分同步 - 选择性数据
- **Delta**: 增量同步 - 仅变化数据
- **Predictive**: 预测同步 - 预测性更新
- **Deferred**: 延迟同步 - 延迟传输
- **Event**: 事件同步 - 仅事件
- **Periodic**: 定期同步 - 定时更新
- **OnDemand**: 按需同步 - 请求时更新
- **Hybrid**: 混合同步 - 组合模式
- **Custom**: 自定义同步

## API 参考

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
