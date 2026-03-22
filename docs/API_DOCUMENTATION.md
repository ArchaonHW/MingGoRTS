# MingGoRTS API 文檔 / MingGoRTS API Documentation

## 📚 概述 / Overview

MingGoRTS 是一個基於 Unreal Engine 5 開發的即時戰略遊戲，設定在中國民國時期。本文檔提供了完整的 API 參考，幫助開發者理解和使用遊戲的各個系統。

MingGoRTS is a real-time strategy game developed with Unreal Engine 5, set in the Republican era of Chinese history. This document provides a complete API reference to help developers understand and use the game's various systems.

---

## 🏗️ 核心架構 / Core Architecture

### 插件結構 / Plugin Structure

```
MingGoRTS/
├── Plugins/
│   ├── MingCore/           # 核心系統 / Core Systems
│   ├── MingPersonal/        # 個人系統 / Personal Systems
│   ├── MingTactical/        # 戰術系統 / Tactical Systems
│   ├── MingStrategic/       # 策略系統 / Strategic Systems
│   ├── MingBuilding/        # 建築系統 / Building Systems
│   ├── MingAudio/           # 音頻系統 / Audio Systems
│   └── MingAchievements/    # 成就系統 / Achievement Systems
```

---

## 🎮 核心系統 API / Core Systems API

### 1. RTS 戰鬥系統 / RTS Combat System

#### MingRTSUnitSelector - 單位選擇器 / Unit Selector

```cpp
/**
 * RTS 單位選擇器
 * 提供多種單位選擇方式和智能過濾功能
 * 
 * RTS Unit Selector
 * Provides various unit selection methods and intelligent filtering
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSUnitSelector : public UObject
{
    GENERATED_BODY()

public:
    // 選擇類型枚舉 / Selection Type Enum
    UENUM(BlueprintType)
    enum class ESelectionType : uint8
    {
        Single,      // 單選 / Single Selection
        Multiple,    // 多選 / Multiple Selection
        Box,         // 框選 / Box Selection
        Group        // 分組 / Group Selection
    };

    // 選擇單位 / Select Units
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    void SelectUnits(const TArray<AActor*>& Units, ESelectionType SelectionType);
    
    // 獲取已選單位 / Get Selected Units
    UFUNCTION(BlueprintPure, Category = "Unit Selection")
    TArray<AActor*> GetSelectedUnits() const { return SelectedUnits; }
    
    // 清除選擇 / Clear Selection
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    void ClearSelection();
    
    // 添加選擇過濾器 / Add Selection Filter
    UFUNCTION(BlueprintCallable, Category = "Unit Selection")
    void AddSelectionFilter(TFunction<bool(AActor*)> Filter);

private:
    UPROPERTY()
    TArray<AActor*> SelectedUnits;
    
    UPROPERTY()
    TArray<TFunction<bool(AActor*)>> SelectionFilters;
};
```

#### MingRTSPathfinder - 路徑規劃器 / Path Planner

```cpp
/**
 * RTS 路徑規劃器
 * 支持多種路徑尋找算法和動態避障
 * 
 * RTS Path Planner
 * Supports multiple pathfinding algorithms and dynamic obstacle avoidance
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSPathfinder : public UObject
{
    GENERATED_BODY()

public:
    // 路徑算法枚舉 / Path Algorithm Enum
    UENUM(BlueprintType)
    enum class EPathAlgorithm : uint8
    {
        AStar,       // A* 算法 / A* Algorithm
        Dijkstra,    // Dijkstra 算法 / Dijkstra Algorithm
        Custom       // 自定義算法 / Custom Algorithm
    };

    // 尋找路徑 / Find Path
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    TArray<FVector> FindPath(const FVector& Start, const FVector& End, EPathAlgorithm Algorithm = EPathAlgorithm::AStar);
    
    // 設置避障 / Set Obstacle Avoidance
    UFUNCTION(BlueprintCallable, Category = "Pathfinding")
    void SetObstacleAvoidance(bool bEnabled);
    
    // 獲取路徑長度 / Get Path Length
    UFUNCTION(BlueprintPure, Category = "Pathfinding")
    float GetPathLength(const TArray<FVector>& Path) const;

private:
    UPROPERTY()
    bool bObstacleAvoidanceEnabled;
    
    // A* 算法實現 / A* Algorithm Implementation
    TArray<FVector> FindPathAStar(const FVector& Start, const FVector& End);
    
    // Dijkstra 算法實現 / Dijkstra Algorithm Implementation
    TArray<FVector> FindPathDijkstra(const FVector& Start, const FVector& End);
};
```

#### MingRTSCombatSystem - 戰鬥系統 / Combat System

```cpp
/**
 * RTS 戰鬥系統
 * 處理單位戰鬥、傷害計算和狀態效果
 * 
 * RTS Combat System
 * Handles unit combat, damage calculation, and status effects
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSCombatSystem : public UObject
{
    GENERATED_BODY()

public:
    // 傷害類型枚舉 / Damage Type Enum
    UENUM(BlueprintType)
    enum class EDamageType : uint8
    {
        Physical,    // 物理傷害 / Physical Damage
        Fire,        // 火焰傷害 / Fire Damage
        Ice,         // 冰霜傷害 / Ice Damage
        Lightning,   // 電擊傷害 / Lightning Damage
        Poison,      // 毒素傷害 / Poison Damage
        Holy,        // 神聖傷害 / Holy Damage
        Dark         // 黑暗傷害 / Dark Damage
    };

    // 執行攻擊 / Execute Attack
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ExecuteAttack(AActor* Attacker, AActor* Target, float DamageAmount, EDamageType DamageType);
    
    // 計算傷害 / Calculate Damage
    UFUNCTION(BlueprintPure, Category = "Combat")
    float CalculateDamage(AActor* Attacker, AActor* Target, float BaseDamage, EDamageType DamageType);
    
    // 應用狀態效果 / Apply Status Effect
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void ApplyStatusEffect(AActor* Target, const FString& EffectName, float Duration);
    
    // 檢查暴擊 / Check Critical Hit
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsCriticalHit(AActor* Attacker, AActor* Target) const;

private:
    UPROPERTY()
    TMap<EDamageType, float> DamageMultipliers;
    
    UPROPERTY()
    TMap<FString, float> StatusEffectDurations;
};
```

### 2. 經濟系統 / Economic System

#### MingRTSEconomicSystem - 經濟管理器 / Economic Manager

```cpp
/**
 * RTS 經濟系統
 * 管理資源生產、貿易和經濟指標
 * 
 * RTS Economic System
 * Manages resource production, trade, and economic indicators
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSEconomicSystem : public UObject
{
    GENERATED_BODY()

public:
    // 資源類型枚舉 / Resource Type Enum
    UENUM(BlueprintType)
    enum class EResourceType : uint8
    {
        Gold,        // 黃金 / Gold
        Food,        // 食物 / Food
        Wood,        // 木材 / Wood
        Stone,       // 石頭 / Stone
        Iron,        // 鐵礦 / Iron Ore
        Coal,        // 煤炭 / Coal
        Silk,        // 絲綢 / Silk
        Tea,         // 茶葉 / Tea
        Porcelain,   // 瓷器 / Porcelain
        Spice        // 香料 / Spice
    };

    // 經濟狀態枚舉 / Economic State Enum
    UENUM(BlueprintType)
    enum class EEconomicState : uint8
    {
        Prosperous,  // 繁榮 / Prosperous
        Stable,      // 穩定 / Stable
        Struggling,  // 掙扎 / Struggling
        Crisis,      // 危機 / Crisis
        Depression,  // 蕭條 / Depression
        Recovery     // 復甦 / Recovery
    };

    // 獲取資源數量 / Get Resource Amount
    UFUNCTION(BlueprintPure, Category = "Economy")
    float GetResourceAmount(EResourceType ResourceType) const;
    
    // 添加資源 / Add Resource
    UFUNCTION(BlueprintCallable, Category = "Economy")
    void AddResource(EResourceType ResourceType, float Amount);
    
    // 消耗資源 / Consume Resource
    UFUNCTION(BlueprintCallable, Category = "Economy")
    bool ConsumeResource(EResourceType ResourceType, float Amount);
    
    // 獲取經濟狀態 / Get Economic State
    UFUNCTION(BlueprintPure, Category = "Economy")
    EEconomicState GetEconomicState() const;
    
    // 計算 GDP / Calculate GDP
    UFUNCTION(BlueprintPure, Category = "Economy")
    float CalculateGDP() const;

private:
    UPROPERTY()
    TMap<EResourceType, float> Resources;
    
    UPROPERTY()
    EEconomicState CurrentEconomicState;
    
    UPROPERTY()
    float GDP;
    
    UPROPERTY()
    float InflationRate;
    
    UPROPERTY()
    float UnemploymentRate;
};
```

#### MingRTSBuildingSystem - 建築系統 / Building System

```cpp
/**
 * RTS 建築系統
 * 管理建築建設、升級和維護
 * 
 * RTS Building System
 * Manages building construction, upgrades, and maintenance
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSBuildingSystem : public UObject
{
    GENERATED_BODY()

public:
    // 建築類型枚舉 / Building Type Enum
    UENUM(BlueprintType)
    enum class EBuildingType : uint8
    {
        Residential,  // 住宅 / Residential
        Farm,         // 農場 / Farm
        Mine,         // 礦場 / Mine
        Market,       // 市場 / Market
        Barracks,     // 兵營 / Barracks
        Temple,       // 神廟 / Temple
        University,   // 大學 / University
        Factory,      // 工廠 / Factory
        Warehouse,    // 倉庫 / Warehouse
        Wall,         // 牆壁 / Wall
        Tower,        // 塔樓 / Tower
        Palace,       // 宮殿 / Palace
        Port,         // 港口 / Port
        Bridge,       // 橋樑 / Bridge
        Monument      // 紀念碑 / Monument
    };

    // 建築狀態枚舉 / Building State Enum
    UENUM(BlueprintType)
    enum class EBuildingState : uint8
    {
        Planned,     // 規劃 / Planned
        Building,    // 建設 / Building
        Operational, // 運營 / Operational
        Upgrading,   // 升級 / Upgrading
        Damaged,     // 損壞 / Damaged
        Destroyed,   // 摧毀 / Destroyed
        Abandoned    // 廢棄 / Abandoned
    };

    // 建造建築 / Build Building
    UFUNCTION(BlueprintCallable, Category = "Building")
    AActor* BuildBuilding(EBuildingType BuildingType, const FVector& Location);
    
    // 升級建築 / Upgrade Building
    UFUNCTION(BlueprintCallable, Category = "Building")
    bool UpgradeBuilding(AActor* Building);
    
    // 維護建築 / Maintain Building
    UFUNCTION(BlueprintCallable, Category = "Building")
    void MaintainBuilding(AActor* Building);
    
    // 獲取建築狀態 / Get Building State
    UFUNCTION(BlueprintPure, Category = "Building")
    EBuildingState GetBuildingState(AActor* Building) const;
    
    // 獲取建築等級 / Get Building Level
    UFUNCTION(BlueprintPure, Category = "Building")
    int32 GetBuildingLevel(AActor* Building) const;

private:
    UPROPERTY()
    TArray<AActor*> Buildings;
    
    UPROPERTY()
    TMap<EBuildingType, float> BuildingCosts;
    
    UPROPERTY()
    TMap<EBuildingType, float> BuildingTimes;
};
```

### 3. AI 系統 / AI System

#### MingRTSAIController - AI 控制器 / AI Controller

```cpp
/**
 * RTS AI 控制器
 * 實現智能 AI 行為和決策系統
 * 
 * RTS AI Controller
 * Implements intelligent AI behavior and decision-making systems
 */
UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API AMingRTSAIController : public AAIController
{
    GENERATED_BODY()

public:
    // AI 類型枚舉 / AI Type Enum
    UENUM(BlueprintType)
    enum class EAIType : uint8
    {
        Aggressive,  // 攻擊型 / Aggressive
        Defensive,   // 防禦型 / Defensive
        Balanced,    // 平衡型 / Balanced
        Economic,    // 經濟型 / Economic
        Rush,        // 快攻型 / Rush
        Turtle       // 龜縮型 / Turtle
    };

    // AI 狀態枚舉 / AI State Enum
    UENUM(BlueprintType)
    enum class EAIState : uint8
    {
        Idle,        // 空閒 / Idle
        Moving,      // 移動 / Moving
        Attacking,   // 攻擊 / Attacking
        Defending,   // 防禦 / Defending
        Building,    // 建設 / Building
        Gathering,   // 採集 / Gathering
        Retreating   // 撤退 / Retreating
    };

    // 設置 AI 類型 / Set AI Type
    UFUNCTION(BlueprintCallable, Category = "AI")
    void SetAIType(EAIType NewAIType);
    
    // 獲取 AI 狀態 / Get AI State
    UFUNCTION(BlueprintPure, Category = "AI")
    EAIState GetAIState() const;
    
    // 執行 AI 決策 / Execute AI Decision
    UFUNCTION(BlueprintCallable, Category = "AI")
    void ExecuteAIDecision();
    
    // 評估威脅 / Assess Threat
    UFUNCTION(BlueprintPure, Category = "AI")
    float AssessThreat(AActor* Target) const;
    
    // 選擇目標 / Select Target
    UFUNCTION(BlueprintCallable, Category = "AI")
    AActor* SelectTarget();

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    EAIType AIType;
    
    UPROPERTY()
    EAIState CurrentAIState;
    
    UPROPERTY()
    TArray<AActor*> KnownEnemies;
    
    UPROPERTY()
    TArray<AActor*> KnownAllies;
    
    // AI 決策邏輯 / AI Decision Logic
    void MakeStrategicDecision();
    void MakeTacticalDecision();
    void MakeEconomicDecision();
    
    // 威脅評估 / Threat Assessment
    void UpdateThreatAssessment();
    float CalculateThreatLevel(AActor* Actor) const;
};
```

---

## 🎨 個人系統 API / Personal Systems API

### 1. 關係系統 / Relationship System

#### MingRelationshipManager - 關係管理器 / Relationship Manager

```cpp
/**
 * 角色關係管理器
 * 管理角色間的關係網絡和聲望系統
 * 
 * Character Relationship Manager
 * Manages relationship networks and reputation systems between characters
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    // 關係類型枚舉 / Relationship Type Enum
    UENUM(BlueprintType)
    enum class ERelationshipType : uint8
    {
        Family,      // 家人 / Family
        Friend,      // 朋友 / Friend
        Ally,        // 同盟 / Ally
        Enemy,       // 敵人 / Enemy
        Rival,       // 競爭者 / Rival
        Mentor,      // 導師 / Mentor
        Student,     // 學生 / Student
        Colleague,   // 同事 / Colleague
        Neighbor,    // 鄰居 / Neighbor
        Stranger     // 陌生人 / Stranger
    };

    // 聲望等級枚舉 / Reputation Level Enum
    UENUM(BlueprintType)
    enum class EReputationLevel : uint8
    {
        Hated,       // 痛恨 / Hated
        Disliked,    // 不喜歡 / Disliked
        Neutral,     // 中立 / Neutral
        Liked,       // 喜歡 / Liked
        Respected,   // 尊重 / Respected
        Revered,     // 敬畏 / Revered
        Legendary    // 傳奇 / Legendary
    };

    // 設置關係 / Set Relationship
    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void SetRelationship(const FString& CharacterA, const FString& CharacterB, ERelationshipType Type, float Value);
    
    // 獲取關係值 / Get Relationship Value
    UFUNCTION(BlueprintPure, Category = "Relationship")
    float GetRelationshipValue(const FString& CharacterA, const FString& CharacterB) const;
    
    // 獲取聲望等級 / Get Reputation Level
    UFUNCTION(BlueprintPure, Category = "Relationship")
    EReputationLevel GetReputationLevel(const FString& Character, const FString& Region = TEXT("")) const;
    
    // 更新關係 / Update Relationship
    UFUNCTION(BlueprintCallable, Category = "Relationship")
    void UpdateRelationship(const FString& CharacterA, const FString& CharacterB, float DeltaValue);
    
    // 獲取所有關係 / Get All Relationships
    UFUNCTION(BlueprintPure, Category = "Relationship")
    TArray<FString> GetAllRelationships(const FString& Character) const;

private:
    UPROPERTY()
    TMap<FString, TMap<FString, float>> Relationships;
    
    UPROPERTY()
    TMap<FString, TMap<FString, EReputationLevel>> Reputations;
    
    // 關係衰減 / Relationship Decay
    void ApplyRelationshipDecay(float DeltaTime);
    
    // 聲望計算 / Reputation Calculation
    void CalculateReputation(const FString& Character);
};
```

### 2. 音頻系統 / Audio System

#### MingAudioRelationshipManager - 音頻關係管理器 / Audio Relationship Manager

```cpp
/**
 * 音頻關係管理器
 * 將關係變化轉化為音頻反饋
 * 
 * Audio Relationship Manager
 * Converts relationship changes into audio feedback
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingAudioRelationshipManager : public UObject
{
    GENERATED_BODY()

public:
    // 音頻事件類型 / Audio Event Type
    UENUM(BlueprintType)
    enum class EAudioEventType : uint8
    {
        RelationshipChange,    // 關係變化 / Relationship Change
        ReputationChange,       // 聲望變化 / Reputation Change
        DialogueStart,          // 對話開始 / Dialogue Start
        DialogueEnd,            // 對話結束 / Dialogue End
        QuestComplete,          // 任務完成 / Quest Complete
        BattleStart,            // 戰鬥開始 / Battle Start
        BattleEnd,              // 戰鬥結束 / Battle End
        LevelUp                 // 升級 / Level Up
    };

    // 播放關係音頻 / Play Relationship Audio
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayRelationshipAudio(const FString& CharacterA, const FString& CharacterB, float RelationshipValue);
    
    // 播放聲望音頻 / Play Reputation Audio
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayReputationAudio(const FString& Character, EReputationLevel ReputationLevel);
    
    // 播放音頻事件 / Play Audio Event
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void PlayAudioEvent(EAudioEventType EventType, const FString& Character = TEXT(""));
    
    // 設置音頻主題 / Set Audio Theme
    UFUNCTION(BlueprintCallable, Category = "Audio Relationship")
    void SetAudioTheme(const FString& Region);

private:
    UPROPERTY()
    TMap<EReputationLevel, USoundBase*> ReputationSounds;
    
    UPROPERTY()
    TMap<EAudioEventType, USoundBase*> EventSounds;
    
    UPROPERTY()
    FString CurrentAudioTheme;
    
    // 音頻邏輯 / Audio Logic
    USoundBase* SelectRelationshipSound(float RelationshipValue);
    USoundBase* SelectReputationSound(EReputationLevel ReputationLevel);
    void ApplyAudioEffects(USoundBase* Sound);
};
```

---

## 🌐 網絡系統 API / Network Systems API

### 1. 網絡管理 / Network Management

#### MingRTSNetworkEnhancedSystem - 增強網絡系統 / Enhanced Network System

```cpp
/**
 * 增強網絡系統
 * 提供完整的多人遊戲網絡支持
 * 
 * Enhanced Network System
 * Provides complete multiplayer game network support
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRTSNetworkEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    // 網絡類型枚舉 / Network Type Enum
    UENUM(BlueprintType)
    enum class ENetworkType : uint8
    {
        LAN,             // 局域網 / LAN
        WAN,             // 廣域網 / WAN
        Internet,        // 互聯網 / Internet
        Local,           // 本地 / Local
        DedicatedServer, // 專用服務器 / Dedicated Server
        ListenServer,    // 監聽服務器 / Listen Server
        PeerToPeer,      // 點對點 / Peer to Peer
        Hybrid,          // 混合 / Hybrid
        Cloud,           // 雲端 / Cloud
        Custom           // 自定義 / Custom
    };

    // 連接狀態枚舉 / Connection State Enum
    UENUM(BlueprintType)
    enum class EConnectionState : uint8
    {
        Disconnected,    // 斷開 / Disconnected
        Connecting,     // 連接中 / Connecting
        Connected,      // 已連接 / Connected
        Authenticating,  // 認證中 / Authenticating
        Authenticated,   // 已認證 / Authenticated
        Loading,         // 載入中 / Loading
        Ready,           // 就緒 / Ready
        Error,           // 錯誤 / Error
        Timeout,         // 超時 / Timeout
        Kicked,          // 被踢出 / Kicked
        Banned           // 被封禁 / Banned
    };

    // 連接到服務器 / Connect to Server
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool ConnectToServer(const FString& IPAddress, int32 Port);
    
    // 創建服務器 / Create Server
    UFUNCTION(BlueprintCallable, Category = "Network")
    bool CreateServer(int32 Port, int32 MaxPlayers);
    
    // 斷開連接 / Disconnect
    UFUNCTION(BlueprintCallable, Category = "Network")
    void Disconnect();
    
    // 獲取連接狀態 / Get Connection State
    UFUNCTION(BlueprintPure, Category = "Network")
    EConnectionState GetConnectionState() const;
    
    // 發送數據 / Send Data
    UFUNCTION(BlueprintCallable, Category = "Network")
    void SendData(const TArray<uint8>& Data);

private:
    UPROPERTY()
    EConnectionState CurrentConnectionState;
    
    UPROPERTY()
    FString ServerIPAddress;
    
    UPROPERTY()
    int32 ServerPort;
    
    UPROPERTY()
    int32 MaxPlayers;
    
    // 網絡邏輯 / Network Logic
    void HandleNetworkMessage(const TArray<uint8>& Data);
    void ProcessIncomingData();
    void MaintainConnection();
};
```

---

## 💾 保存系統 API / Save System API

### 1. 保存載入 / Save Load

#### MingRTSSaveLoadEnhancedSystem - 增強保存載入系統 / Enhanced Save Load System

```cpp
/**
 * 增強保存載入系統
 * 提供多格式、多位置的數據持久化
 * 
 * Enhanced Save Load System
 * Provides multi-format, multi-location data persistence
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingRTSSaveLoadEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    // 數據類型枚舉 / Data Type Enum
    UENUM(BlueprintType)
    enum class ESaveDataType : uint8
    {
        GameState,       // 遊戲狀態 / Game State
        PlayerData,      // 玩家數據 / Player Data
        WorldData,       // 世界數據 / World Data
        Inventory,       // 庫存 / Inventory
        Quests,          // 任務 / Quests
        Relationships,   // 關係 / Relationships
        Campaign,        // 戰役 / Campaign
        Settings,        // 設置 / Settings
        Statistics,      // 統計 / Statistics
        Custom,          // 自定義 / Custom
        All              // 全部 / All
    };

    // 保存格式枚舉 / Save Format Enum
    UENUM(BlueprintType)
    enum class ESaveFormat : uint8
    {
        Binary,      // 二進制 / Binary
        JSON,        // JSON / JSON
        XML,         // XML / XML
        CSV,         // CSV / CSV
        Text,        // 文本 / Text
        Compressed,  // 壓縮 / Compressed
        Encrypted,   // 加密 / Encrypted
        Cloud,       // 雲端 / Cloud
        Database,    // 數據庫 / Database
        Custom       // 自定義 / Custom
    };

    // 保存數據 / Save Data
    UFUNCTION(BlueprintCallable, Category = "Save Load")
    bool SaveData(ESaveDataType DataType, const FString& SlotName, ESaveFormat Format = ESaveFormat::Binary);
    
    // 載入數據 / Load Data
    UFUNCTION(BlueprintCallable, Category = "Save Load")
    bool LoadData(ESaveDataType DataType, const FString& SlotName);
    
    // 刪除存檔 / Delete Save
    UFUNCTION(BlueprintCallable, Category = "Save Load")
    bool DeleteSave(const FString& SlotName);
    
    // 獲取存檔列表 / Get Save List
    UFUNCTION(BlueprintPure, Category = "Save Load")
    TArray<FString> GetSaveList() const;
    
    // 驗證存檔 / Validate Save
    UFUNCTION(BlueprintPure, Category = "Save Load")
    bool ValidateSave(const FString& SlotName) const;

private:
    UPROPERTY()
    TMap<FString, TArray<uint8>> SaveData;
    
    UPROPERTY()
    TMap<FString, FDateTime> SaveTimestamps;
    
    // 數據處理 / Data Processing
    TArray<uint8> SerializeData(ESaveDataType DataType);
    bool DeserializeData(const TArray<uint8>& Data, ESaveDataType DataType);
    TArray<uint8> CompressData(const TArray<uint8>& Data);
    TArray<uint8> EncryptData(const TArray<uint8>& Data);
};
```

---

## 🎯 使用示例 / Usage Examples

### 基本單位選擇 / Basic Unit Selection

```cpp
// 創建單位選擇器 / Create Unit Selector
UMingRTSUnitSelector* UnitSelector = NewObject<UMingRTSUnitSelector>();

// 選擇多個單位 / Select Multiple Units
TArray<AActor*> UnitsToSelect;
UnitsToSelect.Add(Unit1);
UnitsToSelect.Add(Unit2);
UnitsToSelect.Add(Unit3);

UnitSelector->SelectUnits(UnitsToSelect, ESelectionType::Multiple);

// 獲取已選單位 / Get Selected Units
TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
```

### 經濟系統操作 / Economic System Operations

```cpp
// 創建經濟系統 / Create Economic System
UMingRTSEconomicSystem* EconomicSystem = NewObject<UMingRTSEconomicSystem>();

// 添加資源 / Add Resources
EconomicSystem->AddResource(EResourceType::Gold, 1000.0f);
EconomicSystem->AddResource(EResourceType::Food, 500.0f);

// 消耗資源 / Consume Resources
bool bSuccess = EconomicSystem->ConsumeResource(EResourceType::Gold, 200.0f);

// 獲取經濟狀態 / Get Economic State
EEconomicState CurrentState = EconomicSystem->GetEconomicState();
```

### AI 控制器設置 / AI Controller Setup

```cpp
// 創建 AI 控制器 / Create AI Controller
AMingRTSAIController* AIController = GetWorld()->SpawnActor<AMingRTSAIController>();

// 設置 AI 類型 / Set AI Type
AIController->SetAIType(EAIType::Balanced);

// 執行 AI 決策 / Execute AI Decision
AIController->ExecuteAIDecision();

// 獲取 AI 狀態 / Get AI State
EAIState CurrentState = AIController->GetAIState();
```

---

## 🔧 最佳實踐 / Best Practices

### 性能優化 / Performance Optimization

1. **使用對象池** - Use object pools for frequently created objects
2. **避免頻繁分配** - Avoid frequent allocations in hot paths
3. **批量操作** - Batch operations when possible
4. **異步處理** - Use async processing for heavy operations

### 內存管理 / Memory Management

1. **智能指針** - Use smart pointers (TSharedPtr, TWeakPtr)
2. **避免循環引用** - Avoid circular references
3. **及時清理** - Clean up resources promptly
4. **監控內存使用** - Monitor memory usage

### 錯誤處理 / Error Handling

1. **輸入驗證** - Validate all inputs
2. **優雅降級** - Graceful degradation on errors
3. **日誌記錄** - Log all errors and warnings
4. **恢復機制** - Implement recovery mechanisms

---

## 📚 參考資料 / References

### Unreal Engine 文檔 / Unreal Engine Documentation
- [Unreal Engine API Reference](https://docs.unrealengine.com/)
- [Blueprint API Reference](https://docs.unrealengine.com/en-US/programming-and-scripting/blueprints/)
- [C++ Programming Guide](https://docs.unrealengine.com/en-US/programming-and-scripting/cpp-in-unreal-engine/)

### MingGoRTS 特定文檔 / MingGoRTS Specific Documentation
- [系統架構指南](SYSTEM_ARCHITECTURE_GUIDE.md)
- [性能優化指南](PERFORMANCE_OPTIMIZATION_GUIDE.md)
- [網絡編程指南](NETWORK_PROGRAMMING_GUIDE.md)

---

## 📞 支持與反饋 / Support and Feedback

如果您在使用 API 過程中遇到問題，請通過以下方式聯繫我們：

If you encounter issues while using the API, please contact us through:

- **郵件 / Email**: api@minggorts.com
- **GitHub**: [MingGoRTS Repository](https://github.com/minggorts/api)
- **Discord**: [MingGoRTS Discord](https://discord.gg/minggorts)
- **論壇 / Forum**: [MingGoRTS Forum](https://forum.minggorts.com)

---

*最後更新 / Last Updated: 2026年3月22日*  
*版本 / Version: v1.0.0*  
*語言 / Language: 繁體中文 / English*
