# 多人模式規劃

## 基於 Unreal Engine Replication 的多人架構

### 1. 網絡架構設計

#### 1.1 服務器-客戶端架構
```cpp
// 遊戲模式定義
UENUM(BlueprintType)
enum class EGameMode : uint8
{
    SinglePlayer,
    HostMultiplayer,
    JoinMultiplayer
};

// 自定義遊戲模式
class MINGGORTS_API AMingGoRTSGameMode : public AGameModeBase
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    EGameMode CurrentGameMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
    int32 MaxPlayers;
    
    virtual void BeginPlay() override;
    virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FString& Address, const FUniqueNetId& UniqueId, FString& ErrorMessage) override;
};
```

#### 1.2 網絡參數設置
```cpp
// 網絡配置
USTRUCT(BlueprintType)
struct FNetworkSettings
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Port = 7777;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxConnections = 8;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseLAN = false;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ServerTickRate = 30.0f;
};

class MINGGORTS_API AMingGoRTSGameSession : public AGameSession
{
public:
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool HostGame(const FString& GameName, const FNetworkSettings& Settings);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool JoinGame(const FString& IPAddress, int32 Port);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    void FindGames();
};
```

### 2. 同步機制設計

#### 2.1 單位同步
```cpp
// 網絡單位類
class MINGGORTS_API ARTSNetworkUnit : public ARTSUnit
{
    UPROPERTY(ReplicatedUsing = OnRep_Health)
    float Health;
    
    UPROPERTY(ReplicatedUsing = OnRep_Position)
    FVector_NetQuantize100 NetworkPosition;
    
    UPROPERTY(ReplicatedUsing = OnRep_UnitState)
    EUnitState NetworkUnitState;
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerMoveUnit(const FVector& TargetLocation);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastUnitMoved(const FString& UnitID, const FVector& NewPosition);
    
    UFUNCTION(Client, Reliable)
    void ClientUnitSelected(const FString& UnitID, bool bSelected);
    
protected:
    UFUNCTION()
    void OnRep_Health();
    
    UFUNCTION()
    void OnRep_Position();
    
    UFUNCTION()
    void OnRep_UnitState();
};
```

#### 2.2 戰鬥同步
```cpp
// 戰鬥事件同步
USTRUCT(BlueprintType)
struct FDamageInfo
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SourceUnitID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TargetUnitID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Damage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector HitLocation;
};

class MINGGORTS_API ARTSCombatManager : public AActor
{
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerProcessAttack(const FDamageInfo& DamageInfo);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastUnitDamaged(const FDamageInfo& DamageInfo);
    
    UFUNCTION(Client, Reliable)
    void ClientPlayHitEffect(const FVector& HitLocation);
};
```

### 3. 資源同步

#### 3.1 資源管理器同步
```cpp
class MINGGORTS_API ARTSNetworkResourceManager : public UResourceManager
{
    UPROPERTY(ReplicatedUsing = OnRep_PlayerResources)
    TMap<EResourceType, int32> PlayerResources;
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerAddResources(EResourceType ResourceType, int32 Amount);
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerConsumeResources(const TArray<FResourceAmount>& Resources);
    
    UFUNCTION(Client, Reliable)
    void ClientUpdateResources(const TMap<EResourceType, int32>& NewResources);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastResourceChanged(EResourceType ResourceType, int32 NewAmount);
    
protected:
    UFUNCTION()
    void OnRep_PlayerResources();
};
```

### 4. 遊戲狀態同步

#### 4.1 遊戲時鐘同步
```cpp
class MINGGORTS_API ARTSGameClock : public AActor
{
    UPROPERTY(ReplicatedUsing = OnRep_GameTime)
    float NetworkGameTime;
    
    UPROPERTY(ReplicatedUsing = OnRep_GameSpeed)
    float NetworkGameSpeed;
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerSetGameSpeed(float NewSpeed);
    
    UFUNCTION(NetMulticast, Reliable)
    void MulticastGameSpeedChanged(float NewSpeed);
    
    UFUNCTION(Client, Reliable)
    void ClientSyncGameTime(float ServerTime);
    
protected:
    UFUNCTION()
    void OnRep_GameTime();
    
    UFUNCTION()
    void OnRep_GameSpeed();
};
```

### 5. 房間和匹配系統

#### 5.1 房間管理
```cpp
USTRUCT(BlueprintType)
struct FGameRoomInfo
{
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString RoomName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HostName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString GameMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MapName;
};

class MINGGORTS_API URoomManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Room")
    bool CreateRoom(const FGameRoomInfo& RoomInfo);
    
    UFUNCTION(BlueprintCallable, Category = "Room")
    TArray<FGameRoomInfo> GetAvailableRooms();
    
    UFUNCTION(BlueprintCallable, Category = "Room")
    bool JoinRoom(const FString& RoomID);
    
    UFUNCTION(BlueprintCallable, Category = "Room")
    void LeaveRoom();
};
```

### 6. 網絡優化

#### 6.1 數據壓縮
```cpp
// 網絡數據壓縮
class MINGGORTS_API UNetworkCompression
{
public:
    static TArray<uint8> CompressData(const TArray<uint8>& InputData);
    static TArray<uint8> DecompressData(const TArray<uint8>& CompressedData);
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ShouldCompressData(int32 DataSize);
    
private:
    static int32 CompressionThreshold = 1024; // 1KB
};
```

#### 6.2 帶寬管理
```cpp
// 網絡帶寶監控
class MINGGORTS_API UNetworkMonitor : public UGameInstanceSubsystem
{
public:
    UFUNCTION(BlueprintCallable, Category = "Network")
    float GetCurrentBandwidthUsage();
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    float GetCurrentLatency();
    
    UFUNCTION(BlueprintCallable, Category = "Network")
    ENetworkQuality GetNetworkQuality();
    
    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float AverageLatency;
    
    UPROPERTY(BlueprintReadOnly, Category = "Network")
    float PacketLossRate;
};

UENUM(BlueprintType)
enum class ENetworkQuality : uint8
{
    Excellent,
    Good,
    Fair,
    Poor
};
```

### 7. 安全性考慮

#### 7.1 反作弊機制
```cpp
class MINGGORTS_API UAntiCheatManager : public UGameInstanceSubsystem
{
public:
    UFUNCTION(Server, Reliable, BlueprintCallable)
    void ServerValidatePlayerAction(APlayerController* Player, const FString& Action);
    
    UFUNCTION(Server, Reliable, BlueprintCallable)
    bool ServerCheckResourceLegality(APlayerController* Player, EResourceType ResourceType, int32 Amount);
    
    UFUNCTION(Client, Reliable)
    void ClientNotifySuspiciousActivity(const FString& Reason);
    
private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-Cheat")
    bool bEnableAntiCheat = true;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anti-Cheat")
    float MaxResourceChangePerSecond = 100.0f;
};
```

### 8. 實現時間表

#### 階段 1: 基礎網絡 (4週)
- 遊戲模式重構支持多人
- 基礎單位同步
- 簡單房間系統

#### 階段 2: 完整同步 (4週)
- 戰鬥系統同步
- 資源管理同步
- 遊戲時鐘同步

#### 階段 3: 高級功能 (4週)
- 房間匹配系統
- 網絡優化
- 反作弊機制

#### 階段 4: 性能優化 (2週)
- 帶寬管理
- 服務器性能優化
- 客戶端預測

### 9. 技術要求

#### 9.1 服務器要求
- CPU: 4核心以上
- 記憶: 8GB以上
- 帶寬: 100Mbps以上
- 並發玩家: 支持8-16人

#### 9.2 客戶端要求
- 網絡延遲: < 200ms
- 下載速度: > 10Mbps
- 穩定幀率: > 30 FPS
- 記憶使用: < 2GB
