# MingGoRTS API 參考文檔

## 概述

本文檔提供了 MingGoRTS 所有公共 API 的完整參考，包括核心系統、多人遊戲、教育系統、沉浸式體驗和 AI 系統的詳細 API 說明。

## 目錄

- [核心系統 API](#核心系統-api)
- [多人遊戲 API](#多人遊戲-api)
- [教育系統 API](#教育系統-api)
- [沉浸式系統 API](#沉浸式系統-api)
- [AI 系統 API](#ai-系統-api)
- [數據結構](#數據結構)
- [枚舉類型](#枚舉類型)
- [事件系統](#事件系統)

---

## 核心系統 API

### MingConfigurationManager

配置管理器負責管理遊戲的全局配置和設置。

#### 初始化和配置

```cpp
// 初始化配置管理器
UFUNCTION(BlueprintCallable, Category = "Configuration")
bool InitializeConfiguration();

// 載入配置文件
UFUNCTION(BlueprintCallable, Category = "Configuration")
bool LoadConfiguration(const FString& ConfigPath);

// 保存配置文件
UFUNCTION(BlueprintCallable, Category = "Configuration")
bool SaveConfiguration(const FString& ConfigPath);

// 重置為默認配置
UFUNCTION(BlueprintCallable, Category = "Configuration")
void ResetToDefaults();
```

#### 配置獲取和設置

```cpp
// 獲取配置值
UFUNCTION(BlueprintPure, Category = "Configuration")
FString GetConfigurationValue(const FString& Key, const FString& DefaultValue = "");

// 設置配置值
UFUNCTION(BlueprintCallable, Category = "Configuration")
void SetConfigurationValue(const FString& Key, const FString& Value);

// 獲取整數配置
UFUNCTION(BlueprintPure, Category = "Configuration")
int32 GetIntConfiguration(const FString& Key, int32 DefaultValue = 0);

// 設置整數配置
UFUNCTION(BlueprintCallable, Category = "Configuration")
void SetIntConfiguration(const FString& Key, int32 Value);

// 獲取浮點數配置
UFUNCTION(BlueprintPure, Category = "Configuration")
float GetFloatConfiguration(const FString& Key, float DefaultValue = 0.0f);

// 設置浮點數配置
UFUNCTION(BlueprintCallable, Category = "Configuration")
void SetFloatConfiguration(const FString& Key, float Value);

// 獲取布爾配置
UFUNCTION(BlueprintPure, Category = "Configuration")
bool GetBoolConfiguration(const FString& Key, bool DefaultValue = false);

// 設置布爾配置
UFUNCTION(BlueprintCallable, Category = "Configuration")
void SetBoolConfiguration(const FString& Key, bool Value);
```

#### 配置監控

```cpp
// 配置變化事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfigurationChanged, const FString&, Key, const FString&, Value);

UPROPERTY(BlueprintAssignable, Category = "Events")
FOnConfigurationChanged OnConfigurationChanged;

// 開始監控配置變化
UFUNCTION(BlueprintCallable, Category = "Configuration")
void StartConfigurationMonitoring();

// 停止監控配置變化
UFUNCTION(BlueprintCallable, Category = "Configuration")
void StopConfigurationMonitoring();
```

---

## 多人遊戲 API

### MingNetworkManager

網絡管理器負責處理遊戲的網絡連接和通信。

#### 網絡初始化

```cpp
// 初始化網絡系統
UFUNCTION(BlueprintCallable, Category = "Network")
bool InitializeNetwork();

// 關閉網絡系統
UFUNCTION(BlueprintCallable, Category = "Network")
void ShutdownNetwork();

// 檢查網絡是否可用
UFUNCTION(BlueprintPure, Category = "Network")
bool IsNetworkAvailable();
```

#### 會話管理

```cpp
// 創建會話
UFUNCTION(BlueprintCallable, Category = "Network")
bool CreateSession(const FString& SessionName, int32 MaxPlayers = 4);

// 加入會話
UFUNCTION(BlueprintCallable, Category = "Network")
bool JoinSession(const FString& SessionID);

// 離開會話
UFUNCTION(BlueprintCallable, Category = "Network")
void LeaveSession();

// 獲取當前會話信息
UFUNCTION(BlueprintPure, Category = "Network")
FMingNetworkSessionInfo GetCurrentSession();

// 搜索可用會話
UFUNCTION(BlueprintCallable, Category = "Network")
TArray<FMingNetworkSessionInfo> FindSessions();
```

#### 玩家管理

```cpp
// 註冊玩家
UFUNCTION(BlueprintCallable, Category = "Network")
bool RegisterPlayer(const FMingPlayerNetworkInfo& PlayerInfo);

// 註銷玩家
UFUNCTION(BlueprintCallable, Category = "Network")
void UnregisterPlayer(int32 PlayerID);

// 獲取玩家信息
UFUNCTION(BlueprintPure, Category = "Network")
FMingPlayerNetworkInfo GetPlayerInfo(int32 PlayerID);

// 獲取所有玩家
UFUNCTION(BlueprintPure, Category = "Network")
TArray<FMingPlayerNetworkInfo> GetAllPlayers();
```

#### 消息傳遞

```cpp
// 發送消息給特定玩家
UFUNCTION(BlueprintCallable, Category = "Network")
bool SendMessageToPlayer(int32 PlayerID, const FMingNetworkMessage& Message);

// 廣播消息給所有玩家
UFUNCTION(BlueprintCallable, Category = "Network")
bool BroadcastMessage(const FMingNetworkMessage& Message);

// 發送消息給會話
UFUNCTION(BlueprintCallable, Category = "Network")
bool SendMessageToSession(const FMingNetworkMessage& Message);
```

### MingCooperativeManager

合作管理器負責處理多人合作遊戲的邏輯。

#### 合作會話管理

```cpp
// 創建合作會話
UFUNCTION(BlueprintCallable, Category = "Cooperative")
bool CreateCooperativeSession(const FMingCooperativeSessionInfo& SessionInfo);

// 加入合作會話
UFUNCTION(BlueprintCallable, Category = "Cooperative")
bool JoinCooperativeSession(const FString& SessionID);

// 離開合作會話
UFUNCTION(BlueprintCallable, Category = "Cooperative")
void LeaveCooperativeSession();

// 獲取當前合作會話
UFUNCTION(BlueprintPure, Category = "Cooperative")
FMingCooperativeSessionInfo GetCurrentCooperativeSession();
```

#### 目標和任務管理

```cpp
// 創建合作目標
UFUNCTION(BlueprintCallable, Category = "Cooperative")
bool CreateCooperativeObjective(const FMingCooperativeObjective& Objective);

// 更新目標進度
UFUNCTION(BlueprintCallable, Category = "Cooperative")
bool UpdateObjectiveProgress(const FString& ObjectiveID, float Progress);

// 完成目標
UFUNCTION(BlueprintCallable, Category = "Cooperative")
bool CompleteObjective(const FString& ObjectiveID);

// 獲取所有目標
UFUNCTION(BlueprintPure, Category = "Cooperative")
TArray<FMingCooperativeObjective> GetCooperativeObjectives();
```

---

## 教育系統 API

### MingEducationalManager

教育管理器負責管理遊戲的教育模式和學習內容。

#### 教育模式管理

```cpp
// 啟用教育模式
UFUNCTION(BlueprintCallable, Category = "Educational")
bool EnableEducationalMode(EMingEducationalMode Mode);

// 禁用教育模式
UFUNCTION(BlueprintCallable, Category = "Educational")
void DisableEducationalMode();

// 獲取當前教育模式
UFUNCTION(BlueprintPure, Category = "Educational")
EMingEducationalMode GetCurrentEducationalMode();

// 設置教育難度
UFUNCTION(BlueprintCallable, Category = "Educational")
void SetEducationalDifficulty(EMingEducationalDifficulty Difficulty);

// 獲取教育難度
UFUNCTION(BlueprintPure, Category = "Educational")
EMingEducationalDifficulty GetEducationalDifficulty();
```

#### 課程管理

```cpp
// 開始課程
UFUNCTION(BlueprintCallable, Category = "Educational")
bool StartLesson(const FString& LessonID);

// 停止課程
UFUNCTION(BlueprintCallable, Category = "Educational")
void StopLesson();

// 暫停課程
UFUNCTION(BlueprintCallable, Category = "Educational")
void PauseLesson();

// 恢復課程
UFUNCTION(BlueprintCallable, Category = "Educational")
void ResumeLesson();

// 獲取當前課程
UFUNCTION(BlueprintPure, Category = "Educational")
FMingEducationalLesson GetCurrentLesson();

// 獲取可用課程列表
UFUNCTION(BlueprintPure, Category = "Educational")
TArray<FMingEducationalLesson> GetAvailableLessons();
```

#### 測驗系統

```cpp
// 開始測驗
UFUNCTION(BlueprintCallable, Category = "Educational")
bool StartQuiz(const FString& QuizID);

// 提交測驗答案
UFUNCTION(BlueprintCallable, Category = "Educational")
bool SubmitQuizAnswer(const FString& QuestionID, const FString& Answer);

// 完成測驗
UFUNCTION(BlueprintCallable, Category = "Educational")
void CompleteQuiz();

// 獲取測驗結果
UFUNCTION(BlueprintPure, Category = "Educational")
FMingEducationalQuiz GetQuizResults();

// 獲取測驗進度
UFUNCTION(BlueprintPure, Category = "Educational")
float GetQuizProgress();
```

#### 歷史內容

```cpp
// 觸發歷史事件
UFUNCTION(BlueprintCallable, Category = "Educational")
bool TriggerHistoricalEvent(const FString& EventID);

// 獲取歷史內容
UFUNCTION(BlueprintPure, Category = "Educational")
FMingHistoricalContent GetHistoricalContent(const FString& ContentID);

// 獲取歷史時間線
UFUNCTION(BlueprintPure, Category = "Educational")
TArray<FMingHistoricalEvent> GetHistoricalTimeline();

// 顯示歷史時間線
UFUNCTION(BlueprintCallable, Category = "Educational")
void ShowHistoricalTimeline();

// 隱藏歷史時間線
UFUNCTION(BlueprintCallable, Category = "Educational")
void HideHistoricalTimeline();
```

---

## 沉浸式系統 API

### MingVRManager

VR 管理器負責處理虛擬現實功能。

#### VR 系統管理

```cpp
// 初始化 VR 系統
UFUNCTION(BlueprintCallable, Category = "VR")
bool InitializeVRSystem();

// 關閉 VR 系統
UFUNCTION(BlueprintCallable, Category = "VR")
void ShutdownVRSystem();

// 檢查 VR 是否可用
UFUNCTION(BlueprintPure, Category = "VR")
bool IsVRAvailable();

// 檢查 VR 是否啟用
UFUNCTION(BlueprintPure, Category = "VR")
bool IsVREnabled();
```

#### VR 模式控制

```cpp
// 設置 VR 模式
UFUNCTION(BlueprintCallable, Category = "VR")
bool SetVRMode(EMingVRMode VRMode);

// 獲取當前 VR 模式
UFUNCTION(BlueprintPure, Category = "VR")
EMingVRMode GetCurrentVRMode();

// 啟用座席模式
UFUNCTION(BlueprintCallable, Category = "VR")
void EnableSeatedMode();

// 啟用站立模式
UFUNCTION(BlueprintCallable, Category = "VR")
void EnableStandingMode();

// 啟用房間規模模式
UFUNCTION(BlueprintCallable, Category = "VR")
void EnableRoomScale();
```

#### VR 設備管理

```cpp
// 獲取連接的 VR 設備
UFUNCTION(BlueprintPure, Category = "VR")
TArray<FMingVRDeviceInfo> GetConnectedVRDevices();

// 獲取主要 VR 設備
UFUNCTION(BlueprintPure, Category = "VR")
FMingVRDeviceInfo GetPrimaryVRDevice();

// 獲取 VR 控制器信息
UFUNCTION(BlueprintPure, Category = "VR")
TArray<FMingVRControllerInfo> GetVRControllers();
```

### MingARManager

AR 管理器負責處理增強現實功能。

#### AR 系統管理

```cpp
// 初始化 AR 系統
UFUNCTION(BlueprintCallable, Category = "AR")
bool InitializeARSystem();

// 關閉 AR 系統
UFUNCTION(BlueprintCallable, Category = "AR")
void ShutdownARSystem();

// 檢查 AR 是否可用
UFUNCTION(BlueprintPure, Category = "AR")
bool IsARAvailable();

// 檢查 AR 是否啟用
UFUNCTION(BlueprintPure, Category = "AR")
bool IsAREnabled();
```

#### AR 會話管理

```cpp
// 開始 AR 會話
UFUNCTION(BlueprintCallable, Category = "AR")
bool StartARSession(const FMingARSessionConfig& SessionConfig);

// 停止 AR 會話
UFUNCTION(BlueprintCallable, Category = "AR")
void StopARSession();

// 暫停 AR 會話
UFUNCTION(BlueprintCallable, Category = "AR")
void PauseARSession();

// 恢復 AR 會話
UFUNCTION(BlueprintCallable, Category = "AR")
void ResumeARSession();

// 獲取當前 AR 會話
UFUNCTION(BlueprintPure, Category = "AR")
FMingARSession GetCurrentARSession();
```

#### AR 內容管理

```cpp
// 在位置放置 AR 內容
UFUNCTION(BlueprintCallable, Category = "AR")
bool PlaceARContentAtLocation(const FString& ContentID, const FVector3d& Location, const FVector3d& Scale);

// 移除 AR 內容
UFUNCTION(BlueprintCallable, Category = "AR")
bool RemoveARContent(const FString& ContentID);

// 獲取放置的 AR 內容
UFUNCTION(BlueprintPure, Category = "AR")
TArray<FMingARContent> GetPlacedARContent();

// 創建 AR 錨點
UFUNCTION(BlueprintCallable, Category = "AR")
bool CreateARAnchor(const FString& AnchorID, const FVector3d& Position);
```

---

## AI 系統 API

### MingDynamicTaskGenerator

動態任務生成器負責根據玩家行為生成個人化任務。

#### 任務生成

```cpp
// 生成動態任務
UFUNCTION(BlueprintCallable, Category = "AI")
FDynamicTask GenerateDynamicTask(const FTaskGenerationRequest& Request);

// 生成個人化任務
UFUNCTION(BlueprintCallable, Category = "AI")
FDynamicTask GeneratePersonalizedTask(const FString& PlayerID, const FString& Context);

// 生成情境任務
UFUNCTION(BlueprintCallable, Category = "AI")
FDynamicTask GenerateContextualTask(const FString& Context, EMingTaskType TaskType);

// 獲取推薦任務
UFUNCTION(BlueprintPure, Category = "AI")
TArray<FDynamicTask> GetRecommendedTasks(const FString& PlayerID);
```

#### 任務管理

```cpp
// 分配任務給玩家
UFUNCTION(BlueprintCallable, Category = "AI")
bool AssignTaskToPlayer(const FString& TaskID, const FString& PlayerID);

// 更新任務進度
UFUNCTION(BlueprintCallable, Category = "AI")
bool UpdateTaskProgress(const FString& TaskID, float Progress);

// 完成任務
UFUNCTION(BlueprintCallable, Category = "AI")
bool CompleteTask(const FString& TaskID);

// 取消任務
UFUNCTION(BlueprintCallable, Category = "AI")
bool CancelTask(const FString& TaskID);

// 獲取玩家任務
UFUNCTION(BlueprintPure, Category = "AI")
TArray<FDynamicTask> GetPlayerTasks(const FString& PlayerID);
```

### MingPlayerBehaviorPredictor

玩家行為預測器負責分析和預測玩家行為。

#### 行為分析

```cpp
// 記錄玩家行為
UFUNCTION(BlueprintCallable, Category = "AI")
void RecordPlayerAction(const FString& PlayerID, const FPlayerAction& Action);

// 分析玩家行為模式
UFUNCTION(BlueprintCallable, Category = "AI")
FPlayerBehaviorAnalysis AnalyzePlayerBehavior(const FString& PlayerID);

// 獲取玩家洞察
UFUNCTION(BlueprintPure, Category = "AI")
FTaskBehaviorInsight GetPlayerInsight(const FString& PlayerID);

// 獲取行為統計
UFUNCTION(BlueprintPure, Category = "AI")
FPlayerBehaviorStats GetBehaviorStats(const FString& PlayerID);
```

#### 行為預測

```cpp
// 預測玩家下一步行為
UFUNCTION(BlueprintCallable, Category = "AI")
TArray<FPredictionResult> PredictPlayerBehavior(const FString& PlayerID, float TimeWindow);

// 預測玩家動作
UFUNCTION(BlueprintCallable, Category = "AI")
FPredictionResult PredictPlayerAction(const FString& PlayerID, const FString& Context);

// 獲取預測信心
UFUNCTION(BlueprintPure, Category = "AI")
float GetPredictionConfidence(const FString& PlayerID);

// 獲取實時預測
UFUNCTION(BlueprintPure, Category = "AI")
TArray<FPredictionResult> GetRealTimePredictions(const FString& PlayerID, float Duration);
```

---

## 數據結構

### 核心數據結構

```cpp
// 網絡玩家信息
USTRUCT(BlueprintType)
struct FMingPlayerNetworkInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    int32 PlayerID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString PlayerName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    FString PlayerIP;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    int32 Ping;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Info")
    bool bIsHost;
};

// 網絡會話信息
USTRUCT(BlueprintType)
struct FMingNetworkSessionInfo
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString SessionID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    FString SessionName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    int32 MaxPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    int32 CurrentPlayers;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Session Info")
    bool bIsPrivate;
};

// 網絡消息
USTRUCT(BlueprintType)
struct FMingNetworkMessage
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FString MessageID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FString MessageType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FString MessageData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message")
    FDateTime Timestamp;
};
```

### 教育數據結構

```cpp
// 教育課程
USTRUCT(BlueprintType)
struct FMingEducationalLesson
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    FString LessonID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    FString Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    FString Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    EMingEducationalCategory Category;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    EMingEducationalDifficulty Difficulty;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    TArray<FString> Objectives;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lesson")
    TArray<FString> MediaAssets;
};

// 教育測驗
USTRUCT(BlueprintType)
struct FMingEducationalQuiz
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    FString QuizID;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    FString Title;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    TArray<FMingQuizQuestion> Questions;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    float PassingScore;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Quiz")
    float TimeLimit;
};
```

---

## 枚舉類型

### 核心枚舉

```cpp
// 網絡角色
UENUM(BlueprintType)
enum class EMingNetworkRole : uint8
{
    None        = 0,
    Host        = 1,
    Client      = 2,
    Server      = 3,
    Peer        = 4
};

// 連接狀態
UENUM(BlueprintType)
enum class EMingConnectionState : uint8
{
    Disconnected   = 0,
    Connecting     = 1,
    Connected     = 2,
    Reconnecting  = 3,
    Failed        = 4
};
```

### 教育枚舉

```cpp
// 教育模式
UENUM(BlueprintType)
enum class EMingEducationalMode : uint8
{
    Disabled        = 0,
    Tutorial        = 1,
    GuidedLearning  = 2,
    FreeExploration = 3,
    Assessment     = 4,
    Custom          = 5
};

// 教育難度
UENUM(BlueprintType)
enum class EMingEducationalDifficulty : uint8
{
    Beginner    = 0,
    Intermediate = 1,
    Advanced    = 2,
    Expert      = 3,
    Adaptive    = 4
};

// 教育類別
UENUM(BlueprintType)
enum class EMingEducationalCategory : uint8
{
    History     = 0,
    Culture     = 1,
    Military    = 2,
    Politics    = 3,
    Economics   = 4,
    Geography   = 5,
    Art         = 6,
    Literature  = 7,
    Science     = 8,
    Philosophy  = 9,
    Custom      = 10
};
```

### VR/AR 枚舉

```cpp
// VR 模式
UENUM(BlueprintType)
enum class EMingVRMode : uint8
{
    Disabled    = 0,
    Seated      = 1,
    Standing    = 2,
    RoomScale   = 3,
    Cinematic   = 4,
    Custom      = 5
};

// VR 設備類型
UENUM(BlueprintType)
enum class EMingVRDeviceType : uint8
{
    Unknown    = 0,
    Oculus     = 1,
    Vive       = 2,
    Index      = 3,
    Quest      = 4,
    Custom     = 5
};

// AR 追蹤狀態
UENUM(BlueprintType)
enum class EMingARTrackingState : uint8
{
    NotAvailable   = 0,
    Limited        = 1,
    Normal         = 2,
    Excessive      = 3,
    Relocalizing    = 4,
    Custom         = 5
};
```

---

## 事件系統

### 核心事件

```cpp
// 網絡事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerConnected, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerDisconnected, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionCreated, const FString&, SessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionJoined, const FString&, SessionID);

// 配置事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfigurationChanged, const FString&, Key, const FString&, Value);
```

### 教育事件

```cpp
// 教育事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEducationalModeToggled, bool, bEnabled);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLessonStarted, const FMingEducationalLesson&, Lesson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLessonCompleted, const FMingEducationalLesson&, Lesson);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuizCompleted, const FMingEducationalQuiz&, Quiz);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHistoricalEventTriggered, const FMingHistoricalEvent&, Event);
```

### VR/AR 事件

```cpp
// VR 事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceConnected, const FMingVRDeviceInfo&, Device);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRModeChanged, EMingVRMode, OldMode, EMingVRMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRControllerConnected, const FMingVRControllerInfo&, Controller);

// AR 事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARSessionStarted, const FMingARSession&, Session);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARSessionEnded, const FString&, SessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARContentPlaced, const FString&, ContentID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARAnchorCreated, const FString&, AnchorID);
```

---

## 使用示例

### 基礎設置

```cpp
// 在遊戲模式中初始化系統
class AMingGameMode : public AGameModeBase
{
protected:
    UPROPERTY()
    TObjectPtr<UMingNetworkManager> NetworkManager;
    
    UPROPERTY()
    TObjectPtr<UMingEducationalManager> EducationalManager;
    
    UPROPERTY()
    TObjectPtr<UMingVRManager> VRManager;
    
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // 獲取子系統
        NetworkManager = GetGameInstance()->GetSubsystem<UMingNetworkManager>();
        EducationalManager = GetGameInstance()->GetSubsystem<UMingEducationalManager>();
        VRManager = GetGameInstance()->GetSubsystem<UMingVRManager>();
        
        // 綁定事件
        NetworkManager->OnPlayerConnected.AddDynamic(this, &AMingGameMode::OnPlayerConnected);
        EducationalManager->OnLessonStarted.AddDynamic(this, &AMingGameMode::OnLessonStarted);
        VRManager->OnVRDeviceConnected.AddDynamic(this, &AMingGameMode::OnVRDeviceConnected);
    }
    
    UFUNCTION()
    void OnPlayerConnected(int32 PlayerID)
    {
        UE_LOG(LogTemp, Log, TEXT("Player %d connected"), PlayerID);
    }
    
    UFUNCTION()
    void OnLessonStarted(const FMingEducationalLesson& Lesson)
    {
        UE_LOG(LogTemp, Log, TEXT("Lesson started: %s"), *Lesson.Title);
    }
    
    UFUNCTION()
    void OnVRDeviceConnected(const FMingVRDeviceInfo& Device)
    {
        UE_LOG(LogTemp, Log, TEXT("VR device connected: %s"), *Device.DeviceName);
    }
};
```

### 高級功能

```cpp
// 創建個人化教育體驗
UFUNCTION(BlueprintCallable, Category = "Educational")
void CreatePersonalizedEducationalExperience()
{
    // 獲取子系統
    UMingEducationalManager* EduManager = GetGameInstance()->GetSubsystem<UMingEducationalManager>();
    UMingVRManager* VRManager = GetGameInstance()->GetSubsystem<UMingVRManager>();
    
    // 啟用教育模式
    EduManager->EnableEducationalMode(EMingEducationalMode::GuidedLearning);
    EduManager->SetEducationalDifficulty(EMingEducationalDifficulty::Adaptive);
    
    // 如果 VR 可用，啟用 VR 模式
    if (VRManager->IsVRAvailable())
    {
        VRManager->SetVRMode(EMingVRMode::RoomScale);
    }
    
    // 開始個人化課程
    FString PlayerID = GetPlayerID();
    TArray<FMingEducationalLesson> AvailableLessons = EduManager->GetAvailableLessons();
    
    if (AvailableLessons.Num() > 0)
    {
        EduManager->StartLesson(AvailableLessons[0].LessonID);
    }
}
```

---

## 版本歷史

- **v1.0.0** - 初始 API 文檔
- **v1.1.0** - 添加教育系統 API
- **v1.2.0** - 添加 VR/AR 系統 API
- **v1.3.0** - 添加 AI 系統 API
- **v1.4.0** - 完整 API 覆蓋和優化

---

## 貢獻指南

如果您想為 API 文檔做出貢獻：

1. **報告問題** - 在 GitHub 上創建 issue
2. **提交改進** - 創建 pull request
3. **文檔標準** - 遵循現有文檔格式
4. **測試驗證** - 確保代碼示例正確

---

## 聯繫方式

- **技術支持** - support@minggorts.com
- **文檔反饋** - docs@minggorts.com
- **開發者社區** - https://community.minggorts.com

---

*本文檔最後更新：2026年3月23日*
