# MingGoRTS 技術文檔

## 概述

MingGoRTS 是基於 Unreal Engine 5 開發的即時戰略遊戲，融合明朝歷史教育元素。本技術文檔詳細介紹了遊戲的系統架構、核心技術實現、性能優化和開發指南。

## 目錄

- [系統架構](#系統架構)
- [核心系統](#核心系統)
- [插件系統](#插件系統)
- [數據結構](#數據結構)
- [性能優化](#性能優化)
- [網絡架構](#網絡架構)
- [AI 系統](#ai-系統)
- [渲染系統](#渲染系統)
- [音頻系統](#音頻系統)
- [本地化系統](#本地化系統)
- [測試框架](#測試框架)
- [部署和發布](#部署和發布)

---

## 系統架構

### 整體架構

MingGoRTS 採用模組化架構設計，基於 UE5 的插件系統和子系統框架：

```
MingGoRTS 架構
├── Core Systems (核心系統)
│   ├── Game Instance Manager
│   ├── World Manager
│   └── Save/Load System
├── Plugin Systems (插件系統)
│   ├── MingCore (核心插件)
│   ├── MingRTS (RTS系統)
│   ├── MingPersonal (個人系統)
│   ├── MingMultiplayer (多人系統)
│   ├── MingAI (AI系統)
│   ├── MingAudio (音頻系統)
│   ├── MingBuilding (建築系統)
│   └── MingAchievements (成就系統)
└── Content Systems (內容系統)
    ├── Levels (關卡)
    ├── Blueprints (藍圖)
    ├── Materials (材質)
    └── Assets (資產)
```

### 設計原則

#### 1. 模組化設計
- 每個插件職責單一，接口清晰
- 插件間通過統一的接口通信
- 支持熱插拔和獨立開發

#### 2. 事件驅動架構
- 基於 UE5 的事件系統
- 異步處理提高性能
- 解耦組件間的依賴關係

#### 3. 數據驅動
- 配置數據與邏輯分離
- 支持運行時數據修改
- 便於內容更新和版本控制

#### 4. 可擴展性
- 預留擴展接口
- 支持第三方插件
- 向後兼容保證

---

## 核心系統

### Game Instance Manager

#### 功能概述
Game Instance Manager 是遊戲的核心管理器，負責：
- 遊戲狀態管理
- 全局數據存儲
- 子系統協調
- 事件分發

#### 關鍵類
```cpp
// Game Instance 主類
UCLASS()
class MINGGORTS_API UMingGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // 子系統管理
    UPROPERTY()
    TObjectPtr<UMingGameInstanceManager> GameInstanceManager;
    
    // 初始化和清理
    virtual void Init() override;
    virtual void Shutdown() override;
    
    // 遊戲狀態管理
    UFUNCTION(BlueprintCallable)
    void SetGameState(EMingGameState NewState);
    
    UFUNCTION(BlueprintCallable)
    EMingGameState GetGameState() const;
};

// Game Instance Manager
UCLASS()
class MINGGORTS_API UMingGameInstanceManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 子系統註冊
    void RegisterSubsystem(TSubclassOf<UGameInstanceSubsystem> SubsystemClass);
    
    // 事件管理
    void BroadcastGameEvent(EMingGameEventType EventType, const FMingGameEventData& EventData);
    
    // 數據管理
    UFUNCTION(BlueprintCallable)
    void SetGlobalData(const FString& Key, const FString& Value);
    
    UFUNCTION(BlueprintCallable)
    FString GetGlobalData(const FString& Key) const;
};
```

#### 狀態機
```cpp
// 遊戲狀態枚舉
UENUM(BlueprintType)
enum class EMingGameState : uint8
{
    None,
    Initializing,
    MainMenu,
    Loading,
    Playing,
    Paused,
    GameOver,
    Exiting
};

// 狀態轉換邏輯
void UMingGameInstanceManager::SetGameState(EMingGameState NewState)
{
    if (CurrentState == NewState) return;
    
    // 狀態離開事件
    OnGameStateLeft.Broadcast(CurrentState);
    
    EMingGameState PreviousState = CurrentState;
    CurrentState = NewState;
    
    // 狀態進入事件
    OnGameStateEntered.Broadcast(NewState, PreviousState);
    
    // 廣播狀態變化
    OnGameStateChanged.Broadcast(NewState, PreviousState);
}
```

### World Manager

#### 功能概述
World Manager 負責遊戲世界的管理：
- 關卡加載和卸載
- 世界對象管理
- 物理世界設置
- 環境系統控制

#### 關鍵實現
```cpp
UCLASS()
class MINGGORTS_API UMingWorldManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 關卡管理
    UFUNCTION(BlueprintCallable)
    bool LoadLevel(const FString& LevelName, bool bAsync = true);
    
    UFUNCTION(BlueprintCallable)
    void UnloadLevel(const FString& LevelName);
    
    // 世界對象管理
    UFUNCTION(BlueprintCallable)
    void SpawnWorldObject(TSubclassOf<AActor> ActorClass, const FVector& Location, const FRotator& Rotation);
    
    UFUNCTION(BlueprintCallable)
    void DestroyWorldObject(AActor* Actor);
    
    // 環境系統
    UFUNCTION(BlueprintCallable)
    void SetTimeOfDay(float TimeInHours);
    
    UFUNCTION(BlueprintCallable)
    void SetWeatherConditions(EMingWeatherType WeatherType);
};
```

---

## 插件系統

### MingCore 插件

#### 插件結構
```
MingCore/
├── Source/
│   └── MingCore/
│       ├── Public/
│       │   ├── MingCore.h
│       │   ├── MingCoreTypes.h
│       │   ├── MingCoreFunctions.h
│       │   └── MingCoreInterfaces.h
│       └── Private/
│           ├── MingCore.cpp
│           ├── MingCoreFunctions.cpp
│           └── MingCoreManager.cpp
├── Resources/
│   └── MingCore.uplugin
└── Config/
    └── MingCore.ini
```

#### 核心功能
- **基礎數據結構** - 定義通用的數據類型
- **工具函數** - 提供常用的工具函數
- **接口定義** - 定義系統間通信接口
- **配置管理** - 統一的配置管理系統

### MingRTS 插件

#### RTS 核心系統
```cpp
// RTS 系統管理器
UCLASS()
class MINGRTS_API UMingRTSSystemManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 單位管理
    UPROPERTY()
    TObjectPtr<UMingRTSUnitManager> UnitManager;
    
    // 戰鬥系統
    UPROPERTY()
    TObjectPtr<UMingRTSCombatSystem> CombatSystem;
    
    // 資源管理
    UPROPERTY()
    TObjectPtr<UMingRTSResourceManager> ResourceManager;
    
    // 建築系統
    UPROPERTY()
    TObjectPtr<UMingRTSBuildingSystem> BuildingSystem;
    
    // 經濟系統
    UPROPERTY()
    TObjectPtr<UMingRTSEconomicSystem> EconomicSystem;
};
```

#### 單位系統
```cpp
// 單位基類
UCLASS()
class MINGRTS_API AMingRTSUnit : public ACharacter
{
    GENERATED_BODY()

public:
    // 單位屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    FMingUnitStats UnitStats;
    
    // 單位狀態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Unit")
    EMingUnitState UnitState;
    
    // 移動系統
    UFUNCTION(BlueprintCallable, Category = "Unit")
    void MoveToLocation(const FVector& TargetLocation);
    
    // 攻擊系統
    UFUNCTION(BlueprintCallable, Category = "Unit")
    void AttackTarget(AActor* Target);
    
    // 技能系統
    UFUNCTION(BlueprintCallable, Category = "Unit")
    void ActivateAbility(int32 AbilityIndex);
};

// 單位管理器
UCLASS()
class MINGRTS_API UMingRTSUnitManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 單位註冊
    UFUNCTION(BlueprintCallable, Category = "Unit Manager")
    void RegisterUnit(AMingRTSUnit* Unit);
    
    // 單位查詢
    UFUNCTION(BlueprintCallable, Category = "Unit Manager")
    TArray<AMingRTSUnit*> GetUnitsInArea(const FVector& Center, float Radius);
    
    // 單位選擇
    UFUNCTION(BlueprintCallable, Category = "Unit Manager")
    void SelectUnits(const TArray<AMingRTSUnit*>& Units);
    
    // 隊形管理
    UFUNCTION(BlueprintCallable, Category = "Unit Manager")
    void SetFormation(EMingFormationType FormationType);
};
```

### MingPersonal 插件

#### 個人系統架構
```cpp
// 個人系統管理器
UCLASS()
class MINGPERSONAL_API UMingPersonalManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 關係系統
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;
    
    // 聲望系統
    UPROPERTY()
    TObjectPtr<UMingReputationManager> ReputationManager;
    
    // 對話系統
    UPROPERTY()
    TObjectPtr<UMingDialogueManager> DialogueManager;
    
    // 任務系統
    UPROPERTY()
    TObjectPtr<UMingQuestManager> QuestManager;
    
    // UI 系統
    UPROPERTY()
    TObjectPtr<UMingPersonalUIManager> UIManager;
    
    // 音頻系統
    UPROPERTY()
    TObjectPtr<UMingAudioRelationshipManager> AudioManager;
};
```

---

## 數據結構

### 核心數據類型

#### 基礎結構
```cpp
// 基礎向量結構
USTRUCT(BlueprintType)
struct MINGCORE_API FMingVector2D
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float X;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Y;

    FMingVector2D() : X(0.0f), Y(0.0f) {}
    FMingVector2D(float InX, float InY) : X(InX), Y(InY) {}
};

// 基礎顏色結構
USTRUCT(BlueprintType)
struct MINGCORE_API FMingColor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float R;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float G;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float B;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float A;

    FMingColor() : R(1.0f), G(1.0f), B(1.0f), A(1.0f) {}
};
```

#### RTS 數據結構
```cpp
// 單位統計數據
USTRUCT(BlueprintType)
struct MINGRTS_API FMingUnitStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DefensePower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisionRange;
};

// 資源數據
USTRUCT(BlueprintType)
struct MINGRTS_API FMingResourceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingResourceType, int32> Resources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStorage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProductionRate;

    int32 GetResource(EMingResourceType ResourceType) const
    {
        return Resources.FindRef(ResourceType);
    }

    void AddResource(EMingResourceType ResourceType, int32 Amount)
    {
        int32 Current = Resources.FindRef(ResourceType);
        Resources.Add(ResourceType, FMath::Clamp(Current + Amount, 0, MaxStorage));
    }
};
```

#### 個人系統數據結構
```cpp
// 關係數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingRelationshipData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingRelationshipType, float> RelationshipValues;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LastInteractionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SharedMemories;

    float GetRelationshipValue(EMingRelationshipType Type) const
    {
        return RelationshipValues.FindRef(Type);
    }

    void SetRelationshipValue(EMingRelationshipType Type, float Value)
    {
        RelationshipValues.Add(Type, FMath::Clamp(Value, -100.0f, 100.0f));
    }
};

// 聲望數據
USTRUCT(BlueprintType)
struct MINGPERSONAL_API FMingReputationData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<EMingFaction, float> FactionReputation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> RegionReputation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float GlobalReputation;

    float GetReputation(EMingFaction Faction) const
    {
        return FactionReputation.FindRef(Faction);
    }

    void ModifyReputation(EMingFaction Faction, float Amount)
    {
        float Current = FactionReputation.FindRef(Faction);
        FactionReputation.Add(Faction, FMath::Clamp(Current + Amount, -100.0f, 100.0f));
    }
};
```

---

## 性能優化

### 內存管理

#### 對象池系統
```cpp
// 對象池基類
template<typename T>
class MINGCORE_API TMingObjectPool
{
private:
    TArray<T*> AvailableObjects;
    TArray<T*> UsedObjects;
    TSubclassOf<T> ObjectClass;
    int32 MaxPoolSize;

public:
    TMingObjectPool(TSubclassOf<T> InObjectClass, int32 InMaxPoolSize = 100)
        : ObjectClass(InObjectClass), MaxPoolSize(InMaxPoolSize)
    {
    }

    T* GetObject()
    {
        if (AvailableObjects.Num() > 0)
        {
            T* Object = AvailableObjects.Pop();
            UsedObjects.Add(Object);
            return Object;
        }
        else if (UsedObjects.Num() < MaxPoolSize)
        {
            T* Object = NewObject<T>(GetTransientPackage(), ObjectClass);
            UsedObjects.Add(Object);
            return Object;
        }
        return nullptr;
    }

    void ReturnObject(T* Object)
    {
        if (UsedObjects.Remove(Object) > 0)
        {
            // 重置對象狀態
            if (Object->GetClass()->ImplementsInterface(UMingPoolableInterface::StaticClass()))
            {
                IMingPoolableInterface::Execute_ResetObject(Object);
            }
            AvailableObjects.Add(Object);
        }
    }

    void ClearPool()
    {
        AvailableObjects.Empty();
        UsedObjects.Empty();
    }
};
```

#### 智能垃圾回收
```cpp
// 垃圾回收管理器
UCLASS()
class MINGCORE_API UMingGarbageCollector : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<UObject*, float> ObjectLastAccessTime;
    float CleanupInterval;
    float ObjectLifetime;

public:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override
    {
        Super::Initialize(Collection);
        
        CleanupInterval = 60.0f; // 每分鐘清理一次
        ObjectLifetime = 300.0f;  // 5分鐘未使用則清理
        
        GetWorld()->GetTimerManager().SetTimer(
            CleanupTimer,
            this,
            &UMingGarbageCollector::PerformCleanup,
            CleanupInterval,
            true
        );
    }

    void MarkObjectAccess(UObject* Object)
    {
        ObjectLastAccessTime.Add(Object, GetWorld()->GetTimeSeconds());
    }

    void PerformCleanup()
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        TArray<UObject*> ObjectsToRemove;

        for (auto& Pair : ObjectLastAccessTime)
        {
            if (CurrentTime - Pair.Value > ObjectLifetime)
            {
                ObjectsToRemove.Add(Pair.Key);
            }
        }

        for (UObject* Object : ObjectsToRemove)
        {
            ObjectLastAccessTime.Remove(Object);
            if (Object && !Object->IsPendingKill())
            {
                Object->MarkAsGarbage();
            }
        }
    }
};
```

### 渲染優化

#### LOD 系統
```cpp
// LOD 管理器
UCLASS()
class MINGRTS_API UMingLODManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 設置單位 LOD
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void SetUnitLOD(AMingRTSUnit* Unit, int32 LODLevel);

    // 計算最佳 LOD 級別
    UFUNCTION(BlueprintCallable, Category = "LOD")
    int32 CalculateOptimalLOD(const FVector& ViewLocation, const FVector& ObjectLocation, float ObjectSize);

    // 批量更新 LOD
    UFUNCTION(BlueprintCallable, Category = "LOD")
    void UpdateBatchLOD(const TArray<AMingRTSUnit*>& Units, const FVector& ViewLocation);
};

int32 UMingLODManager::CalculateOptimalLOD(const FVector& ViewLocation, const FVector& ObjectLocation, float ObjectSize)
{
    float Distance = FVector::Distance(ViewLocation, ObjectLocation);
    
    if (Distance < 500.0f)
    {
        return 0; // 高質量
    }
    else if (Distance < 1500.0f)
    {
        return 1; // 中等質量
    }
    else if (Distance < 3000.0f)
    {
        return 2; // 低質量
    }
    else
    {
        return 3; // 最低質量
    }
}
```

#### 實例化渲染
```cpp
// 實例化渲染組件
UCLASS(ClassGroup = (Rendering), meta = (BlueprintSpawnableComponent))
class MINGRTS_API UMingInstancedRenderComponent : public USceneComponent
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TArray<FTransform> InstanceTransforms;

    UPROPERTY()
    UMaterialInterface* InstanceMaterial;

    UPROPERTY()
    UStaticMesh* InstanceMesh;

public:
    // 添加實例
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void AddInstance(const FTransform& Transform);

    // 移除實例
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void RemoveInstance(int32 InstanceIndex);

    // 更新實例數據
    UFUNCTION(BlueprintCallable, Category = "Instanced Rendering")
    void UpdateInstanceData();

    // 渲染所有實例
    virtual void GetUsedMaterials(TArray<UMaterialInterface*>& OutMaterials) const override;
};
```

---

## 網絡架構

### 客戶端-服務器模型

#### 網絡管理器
```cpp
// 網絡管理器
UCLASS()
class MINGMULTIPLAYER_API UMingNetworkManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 連接服務器
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer(const FString& ServerAddress, int32 Port);

    // 斷開連接
    UFUNCTION(BlueprintCallable, Category = "Network")
    void DisconnectFromServer();

    // 發送數據
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SendDataToServer(const FMingNetworkData& Data);

    // 廣播數據
    UFUNCTION(BlueprintCallable, Category = "Network")
    void BroadcastData(const FMingNetworkData& Data);

    // 網絡事件
    UPROPERTY(BlueprintAssignable)
    FMingNetworkEvent OnConnected;

    UPROPERTY(BlueprintAssignable)
    FMingNetworkEvent OnDisconnected;

    UPROPERTY(BlueprintAssignable)
    FMingNetworkDataEvent OnDataReceived;
};
```

#### 數據同步
```cpp
// 網絡數據結構
USTRUCT(BlueprintType)
struct MINGMULTIPLAYER_API FMingNetworkData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DataType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<uint8> DataBytes;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SenderID;
};

// 同步管理器
UCLASS()
class MINGMULTIPLAYER_API UMingSyncManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, FMingNetworkData> SyncData;
    float LastSyncTime;

public:
    // 註冊同步數據
    UFUNCTION(BlueprintCallable, Category = "Sync")
    void RegisterSyncData(const FString& DataID, const FMingNetworkData& Data);

    // 獲取同步數據
    UFUNCTION(BlueprintCallable, Category = "Sync")
    FMingNetworkData GetSyncData(const FString& DataID);

    // 執行同步
    UFUNCTION(BlueprintCallable, Category = "Sync")
    void PerformSync();

    // 檢查同步狀態
    UFUNCTION(BlueprintCallable, Category = "Sync")
    bool IsDataSynced(const FString& DataID);
};
```

---

## AI 系統

### 行為樹系統

#### AI 控制器
```cpp
// 增強 AI 控制器
UCLASS()
class MINGAI_API UMingEnhancedAIController : public AAIController
{
    GENERATED_BODY()

protected:
    // 行為樹組件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UBehaviorTree> BehaviorTree;

    // AI 感知組件
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    TObjectPtr<UAIPerceptionComponent> AIPerception;

    // AI 配置
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FMingAIConfig AIConfig;

public:
    // 執行 AI 行為
    UFUNCTION(BlueprintCallable, Category = "AI")
    void ExecuteAIBehavior();

    // 設置 AI 目標
    UFUNCTION(BlueprintCallable, Category = "AI")
    void SetAITarget(AActor* Target);

    // 更新 AI 狀態
    UFUNCTION(BlueprintCallable, Category = "AI")
    void UpdateAIState();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 感知處理
    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
```

#### 任務系統
```cpp
// AI 任務節點
UCLASS()
class MINGAI_API UMingAITaskNode : public UBTTaskNode
{
    GENERATED_BODY()

public:
    // 執行任務
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    // 任務描述
    virtual FString GetStaticDescription() const override;

protected:
    // 任務邏輯
    virtual EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) { return EBTNodeResult::Succeeded; }

    // 檢查任務條件
    virtual bool CheckTaskConditions(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) { return true; }
};

// 具體任務實現
UCLASS()
class MINGAI_API UMingMoveToTask : public UMingAITaskNode
{
    GENERATED_BODY()

protected:
    virtual EBTNodeResult::Type PerformTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float AcceptableRadius = 50.0f;
};
```

---

## 渲染系統

### 自定義渲染

#### 渲染管理器
```cpp
// 渲染管理器
UCLASS()
class MINGCORE_API UMingRenderManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, UMaterialInterface*> CustomMaterials;
    TMap<FString, UTexture2D*> RenderTargets;

public:
    // 註冊自定義材質
    UFUNCTION(BlueprintCallable, Category = "Render")
    void RegisterCustomMaterial(const FString& MaterialID, UMaterialInterface* Material);

    // 創建渲染目標
    UFUNCTION(BlueprintCallable, Category = "Render")
    UTexture2D* CreateRenderTarget(const FString& TargetID, int32 Width, int32 Height);

    // 執行自定義渲染
    UFUNCTION(BlueprintCallable, Category = "Render")
    void ExecuteCustomRender(const FString& RenderPassID);

    // 後處理效果
    UFUNCTION(BlueprintCallable, Category = "Render")
    void ApplyPostProcessEffect(const FString& EffectName, float Intensity);
};
```

#### 着色器系統
```cpp
// 自定義着色器
class MINGCORE_API FMingCustomShader : public FGlobalShader
{
    DECLARE_SHADER_TYPE(FMingCustomShader, Global);

public:
    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return IsFeatureLevelSupported(Parameters.Platform, ERHIFeatureLevel::SM5);
    }

    static void ModifyCompilationEnvironment(const FGlobalShaderPermutationParameters& Parameters, FShaderCompilerEnvironment& OutEnvironment)
    {
        OutEnvironment.SetDefine(TEXT("CUSTOM_SHADER"), 1);
    }

    FMingCustomShader() {}
    FMingCustomShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {
        // 綁定參數
        TextureParam.Bind(Initializer.ParameterMap, TEXT("CustomTexture"));
        TimeParam.Bind(Initializer.ParameterMap, TEXT("Time"));
    }

    void SetParameters(FRHICommandListImmediate& RHICmdList, FTextureRHIParamRef Texture, float Time)
    {
        SetTextureParameter(RHICmdList, GetPixelShader(), TextureParam, Texture);
        SetShaderValue(RHICmdList, GetPixelShader(), TimeParam, Time);
    }

private:
    FShaderResourceParameter TextureParam;
    FShaderParameter TimeParam;
};
```

---

## 音頻系統

### 音頻管理

#### 音頻管理器
```cpp
// 音頻管理器
UCLASS()
class MINGAUDIO_API UMingAudioManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TObjectPtr<UAudioComponent> BackgroundMusicComponent;

    UPROPERTY()
    TMap<FString, USoundBase*> SoundEffects;

    UPROPERTY()
    TMap<FString, USoundCue*> AudioCues;

public:
    // 播放背景音樂
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayBackgroundMusic(USoundBase* Music, float Volume = 1.0f);

    // 播放音效
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlaySoundEffect(const FString& EffectID, const FVector& Location = FVector::ZeroVector);

    // 播放音頻提示
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void PlayAudioCue(const FString& CueID);

    // 設置音頻參數
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetAudioParameters(float MasterVolume, float MusicVolume, float SFXVolume);

    // 音頻淡入淡出
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void FadeAudio(float TargetVolume, float FadeTime);
};
```

#### 3D 音頻系統
```cpp
// 3D 音頻組件
UCLASS(ClassGroup = (Audio), meta = (BlueprintSpawnableComponent))
class MINGAUDIO_API UMing3DAudioComponent : public UAudioComponent
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3D Audio")
    float MaxDistance = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3D Audio")
    float ReferenceDistance = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3D Audio")
    float AttenuationOverride = 1.0f;

public:
    // 設置 3D 音頻參數
    UFUNCTION(BlueprintCallable, Category = "3D Audio")
    void Set3DAudioParameters(float InMaxDistance, float InReferenceDistance, float InAttenuation);

    // 更新音頻位置
    UFUNCTION(BlueprintCallable, Category = "3D Audio")
    void UpdateAudioPosition(const FVector& NewPosition);

    // 計算音頻衰減
    UFUNCTION(BlueprintCallable, Category = "3D Audio")
    float CalculateAttenuation(float Distance) const;
};
```

---

## 本地化系統

### 多語言支持

#### 本地化管理器
```cpp
// 本地化管理器
UCLASS()
class MINGLOCALIZATION_API UMingLocalizationManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    TMap<FString, TMap<FString, FString>> LocalizationData;
    FString CurrentLanguage;

public:
    // 加載語言包
    UFUNCTION(BlueprintCallable, Category = "Localization")
    bool LoadLanguagePack(const FString& LanguageCode);

    // 獲取本地化文本
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString GetLocalizedText(const FString& Key);

    // 設置當前語言
    UFUNCTION(BlueprintCallable, Category = "Localization")
    void SetCurrentLanguage(const FString& LanguageCode);

    // 獲取支持語言列表
    UFUNCTION(BlueprintCallable, Category = "Localization")
    TArray<FString> GetSupportedLanguages();

    // 格式化本地化文本
    UFUNCTION(BlueprintCallable, Category = "Localization")
    FString FormatLocalizedText(const FString& Key, const TArray<FString>& Parameters);
};
```

#### 文本資源
```cpp
// 文本資源結構
USTRUCT(BlueprintType)
struct MINGLOCALIZATION_API FMingTextResource
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Key;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> Translations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Context;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsPluralizable;

    FString GetTranslation(const FString& LanguageCode) const
    {
        return Translations.FindRef(LanguageCode);
    }
};
```

---

## 測試框架

### 自動化測試

#### 測試基類
```cpp
// 測試基類
UCLASS()
class MINGCORE_API UMingTestBase : public UObject
{
    GENERATED_BODY()

protected:
    // 測試結果
    UPROPERTY()
    TArray<FMingTestResult> TestResults;

    // 測試設置
    UFUNCTION(BlueprintCallable, Category = "Test")
    virtual void SetupTest();

    // 測試清理
    UFUNCTION(BlueprintCallable, Category = "Test")
    virtual void CleanupTest();

    // 斷言函數
    UFUNCTION(BlueprintCallable, Category = "Test")
    void AssertTrue(bool Condition, const FString& Message = "");

    UFUNCTION(BlueprintCallable, Category = "Test")
    void AssertFalse(bool Condition, const FString& Message = "");

    UFUNCTION(BlueprintCallable, Category = "Test")
    void AssertEqual(float A, float B, float Tolerance = 0.001f, const FString& Message = "");

    // 運行測試
    UFUNCTION(BlueprintCallable, Category = "Test")
    virtual void RunTest();

public:
    // 獲取測試結果
    UFUNCTION(BlueprintCallable, Category = "Test")
    TArray<FMingTestResult> GetTestResults() const { return TestResults; }
};

// 測試結果結構
USTRUCT(BlueprintType)
struct MINGCORE_API FMingTestResult
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bPassed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;
};
```

#### 測試套件
```cpp
// 測試套件
UCLASS()
class MINGCORE_API UMingTestSuite : public UMingTestBase
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TArray<TObjectPtr<UMingTestBase>> TestCases;

public:
    // 添加測試用例
    UFUNCTION(BlueprintCallable, Category = "Test Suite")
    void AddTestCase(UMingTestBase* TestCase);

    // 運行所有測試
    UFUNCTION(BlueprintCallable, Category = "Test Suite")
    void RunAllTests();

    // 生成測試報告
    UFUNCTION(BlueprintCallable, Category = "Test Suite")
    FString GenerateTestReport();

    // 獲取測試統計
    UFUNCTION(BlueprintCallable, Category = "Test Suite")
    FMingTestStatistics GetTestStatistics();
};
```

---

## 部署和發布

### 構建配置

#### 構建腳本
```powershell
# 構建腳本示例
param(
    [string]$BuildConfiguration = "Development",
    [string]$TargetPlatform = "Win64",
    [string]$ProjectPath = "C:\HW\MingGoRTS"
)

# 設置環境變量
$env:UE_PATH = "C:\Program Files\Epic Games\UE_5.4"
$env:PROJECT_PATH = $ProjectPath

# 執行構建
Write-Host "開始構建 MingGoRTS..."
Write-Host "配置: $BuildConfiguration"
Write-Host "平台: $TargetPlatform"

# 調用 UE 構建工具
& "$env:UE_PATH\Engine\Build\BatchFiles\Build.bat" `
    MingGoRTSEditor `
    $TargetPlatform `
    $BuildConfiguration `
    -project="$env:PROJECT_PATH\MingGoRTS.uproject" `
    -waitmutex

if ($LASTEXITCODE -eq 0) {
    Write-Host "構建成功完成!"
} else {
    Write-Host "構建失敗!"
    exit $LASTEXITCODE
}
```

#### 打包配置
```json
{
    "BuildConfiguration": "Shipping",
    "TargetPlatforms": ["Win64", "Android", "iOS"],
    "IncludeDebugSymbols": false,
    "CompressAssets": true,
    "GenerateChunks": true,
    "AdditionalOptions": {
        "EnableCrashReporter": true,
        "EnableTelemetry": false,
        "SkipCooking": false,
        "SkipPak": false
    }
}
```

### 發布流程

#### 版本管理
```cpp
// 版本管理器
UCLASS()
class MINGCORE_API UMingVersionManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

private:
    UPROPERTY()
    FMingVersionInfo CurrentVersion;

public:
    // 獲取當前版本
    UFUNCTION(BlueprintCallable, Category = "Version")
    FMingVersionInfo GetCurrentVersion() const { return CurrentVersion; }

    // 檢查版本兼容性
    UFUNCTION(BlueprintCallable, Category = "Version")
    bool IsVersionCompatible(const FMingVersionInfo& OtherVersion) const;

    // 更新版本信息
    UFUNCTION(BlueprintCallable, Category = "Version")
    void UpdateVersionInfo(const FMingVersionInfo& NewVersion);

    // 生成版本報告
    UFUNCTION(BlueprintCallable, Category = "Version")
    FString GenerateVersionReport() const;
};

// 版本信息結構
USTRUCT(BlueprintType)
struct MINGCORE_API FMingVersionInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MajorVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinorVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PatchVersion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BuildNumber;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime BuildDate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString BranchName;

    FString GetVersionString() const
    {
        return FString::Printf(TEXT("%d.%d.%d-%s"), MajorVersion, MinorVersion, PatchVersion, *BuildNumber);
    }
};
```

---

## 總結

MingGoRTS 的技術架構採用了現代化的設計理念，通過模組化插件系統、事件驅動架構和數據驅動設計，實現了高度可擴展和可維護的遊戲框架。

### 主要技術特點

1. **模組化設計** - 基於 UE5 插件系統的模組化架構
2. **性能優化** - 對象池、智能垃圾回收、LOD 系統等優化技術
3. **網絡支持** - 完整的客戶端-服務器網絡架構
4. **AI 系統** - 基於行為樹的智能 AI 系統
5. **渲染系統** - 自定義着色器和渲染管線
6. **音頻系統** - 3D 音頻和動態音頻管理
7. **本地化** - 完整的多語言支持系統
8. **測試框架** - 自動化測試和質量保證

### 未來擴展

技術架構為未來的功能擴展提供了堅實的基礎：
- VR/AR 支持的進一步優化
- 雲端計算集成
- AI 系統的增強
- 跨平台支持擴展
- 性能進一步優化

這個技術架構確保了 MingGoRTS 能夠滿足當前的功能需求，同時為未來的技術發展和功能擴展提供了靈活的支持。

---

*本文檔最後更新：2026年3月23日*
