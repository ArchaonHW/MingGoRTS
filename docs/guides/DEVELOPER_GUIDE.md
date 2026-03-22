# MingGoRTS 開發者指南

## 概述

本指南為 MingGoRTS 開發者提供完整的開發指導，包括環境設置、系統集成、最佳實踐和常見問題解決方案。

## 目錄

- [環境設置](#環境設置)
- [項目結構](#項目結構)
- [快速開始](#快速開始)
- [系統集成](#系統集成)
- [最佳實踐](#最佳實踐)
- [調試技巧](#調試技巧)
- [性能優化](#性能優化)
- [常見問題](#常見問題)
- [新系統集成](#新系統集成)

---

## 環境設置

### 系統要求

#### 最低要求
- **操作系統**: Windows 10 64-bit 或更新版本
- **處理器**: Intel Core i5 或 AMD Ryzen 5
- **內存**: 16GB RAM
- **顯卡**: NVIDIA GTX 1060 或 AMD RX 580
- **存儲**: 50GB 可用空間
- **網絡**: 寬帶互聯網連接

#### 推薦配置
- **操作系統**: Windows 11 64-bit
- **處理器**: Intel Core i7 或 AMD Ryzen 7
- **內存**: 32GB RAM
- **顯卡**: NVIDIA RTX 3070 或 AMD RX 6700 XT
- **存儲**: 100GB SSD
- **網絡**: 高速寬帶連接

### 開發工具安裝

#### 1. Unreal Engine 5
```bash
# 下載並安裝 Unreal Engine 5.3
# 從 Epic Games Launcher 安裝
# 或從源碼編譯
```

#### 2. Visual Studio 2022
```bash
# 安裝 Visual Studio 2022 Community 或更高版本
# 確保安裝以下組件：
# - Game development with C++
# - Windows 10/11 SDK
# - .NET Framework 4.7.2 或更高
```

#### 3. Git 版本控制
```bash
# 安裝 Git for Windows
git --version

# 配置 Git
git config --global user.name "Your Name"
git config --global user.email "your.email@example.com"
```

#### 4. Python 3.9+
```bash
# 安裝 Python 3.9 或更高版本
python --version

# 安裝必要包
pip install unreal-engine
pip install python-ue4
```

### 項目克隆和設置

```bash
# 克隆項目
git clone https://github.com/MingGoRTS/MingGoRTS.git
cd MingGoRTS

# 初始化子模塊
git submodule update --init --recursive

# 生成項目文件
# 在 Windows 上
GenerateProjectFiles.bat

# 在其他平台上
./GenerateProjectFiles.sh
```

---

## 項目結構

### 根目錄結構

```
MingGoRTS/
├── Source/                          # 主要源代碼
│   ├── MingGoRTS/                  # 主遊戲模組
│   │   ├── Public/                  # 公共頭文件
│   │   └── Private/                 # 私有實現文件
│   └── MingGoRTS.Target.cs         # 項目目標文件
├── Plugins/                         # 插件目錄
│   ├── MingCore/                    # 核心系統插件
│   ├── MingMultiplayer/             # 多人遊戲插件
│   ├── MingPersonal/                # 個人系統插件
│   ├── MingEducational/             # 教育系統插件
│   ├── MingAI/                     # AI 系統插件
│   └── MingTools/                  # 開發工具插件
├── Content/                         # 遊戲內容
│   ├── Blueprints/                  # Blueprint 資產
│   ├── Materials/                  # 材質資產
│   ├── Models/                     # 3D 模型
│   ├── Audio/                      # 音頻資產
│   └── UI/                         # UI 資產
├── Config/                          # 配置文件
├── Build/                           # 構建輸出
├── DerivedDataCache/                 # 派生數據緩存
├── Saved/                           # 保存的數據
├── docs/                            # 文檔
└── Tools/                           # 開發工具
```

### 插件結構

每個插件都遵循統一的結構：

```
PluginName/
├── Source/PluginName/
│   ├── Public/                      # 公共 API
│   │   ├── PluginName.h            # 主插件頭文件
│   │   ├── Subsystems/             # 子系統頭文件
│   │   └── Types/                 # 數據類型
│   └── Private/                     # 私有實現
│       ├── PluginName.cpp          # 主插件實現
│       ├── Subsystems/             # 子系統實現
│       └── Tests/                  # 測試文件
├── Content/                         # 插件內容
├── Config/                          # 插件配置
└── PluginName.uplugin             # 插件描述文件
```

---

## 快速開始

### 創建第一個子系統

#### 1. 創建頭文件

```cpp
// MySubsystem.h
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MySubsystem.generated.h"

UCLASS()
class MINGGORTS_API UMySubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // 初始化子系統
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    
    // 清理子系統
    virtual void Deinitialize() override;
    
    // 每幀更新
    virtual void Tick(float DeltaTime) override;
    
    // 自定義功能
    UFUNCTION(BlueprintCallable, Category = "My Subsystem")
    void DoSomething();
    
protected:
    // 子系統狀態
    UPROPERTY()
    bool bIsInitialized = false;
    
    // 配置數據
    UPROPERTY()
    FString ConfigurationData;
};
```

#### 2. 創建實現文件

```cpp
// MySubsystem.cpp
#include "MySubsystem.h"
#include "Engine/GameInstance.h"

void UMySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogTemp, Log, TEXT("MySubsystem initializing..."));
    
    // 初始化邏輯
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MySubsystem initialized successfully"));
}

void UMySubsystem::Deinitialize()
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Log, TEXT("MySubsystem deinitializing..."));
        
        // 清理邏輯
        bIsInitialized = false;
        
        UE_LOG(LogTemp, Log, TEXT("MySubsystem deinitialized successfully"));
    }
    
    Super::Deinitialize();
}

void UMySubsystem::Tick(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }
    
    // 每幀更新邏輯
    Super::Tick(DeltaTime);
}

void UMySubsystem::DoSomething()
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("MySubsystem not initialized"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Doing something..."));
    
    // 實現功能邏輯
}
```

#### 3. 註冊子系統

在插件的 `.Build.cs` 文件中添加：

```csharp
PublicDependencyModuleNames.AddRange(new string[] 
{
    "MySubsystem"
});
```

### 創建第一個遊戲模式

```cpp
// MyGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameMode.generated.h"

UCLASS()
class MINGGORTS_API AMyGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMyGameMode();
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
    // 初始化遊戲系統
    void InitializeGameSystems();
    
    // 處理玩家連接
    UFUNCTION()
    void OnPlayerConnected(APlayerController* PlayerController);
    
    // 處理玩家斷開連接
    UFUNCTION()
    void OnPlayerDisconnected(APlayerController* PlayerController);
};
```

```cpp
// MyGameMode.cpp
#include "MyGameMode.h"
#include "MingCore/Subsystems/MingConfigurationManager.h"
#include "MingMultiplayer/Subsystems/MingNetworkManager.h"

AMyGameMode::AMyGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogTemp, Log, TEXT("MyGameMode starting..."));
    
    // 初始化遊戲系統
    InitializeGameSystems();
}

void AMyGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 遊戲邏輯更新
}

void AMyGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogTemp, Log, TEXT("MyGameMode ending..."));
    
    Super::EndPlay(EndPlayReason);
}

void AMyGameMode::InitializeGameSystems()
{
    // 獲取子系統
    UGameInstance* GameInstance = GetGameInstance();
    
    if (GameInstance)
    {
        // 初始化配置管理器
        UMingConfigurationManager* ConfigManager = GameInstance->GetSubsystem<UMingConfigurationManager>();
        if (ConfigManager)
        {
            ConfigManager->InitializeConfiguration();
        }
        
        // 初始化網絡管理器
        UMingNetworkManager* NetworkManager = GameInstance->GetSubsystem<UMingNetworkManager>();
        if (NetworkManager)
        {
            NetworkManager->InitializeNetwork();
            
            // 綁定事件
            NetworkManager->OnPlayerConnected.AddDynamic(this, &AMyGameMode::OnPlayerConnected);
            NetworkManager->OnPlayerDisconnected.AddDynamic(this, &AMyGameMode::OnPlayerDisconnected);
        }
    }
}

void AMyGameMode::OnPlayerConnected(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        UE_LOG(LogTemp, Log, TEXT("Player connected: %s"), *PlayerController->GetName());
        
        // 處理玩家連接邏輯
    }
}

void AMyGameMode::OnPlayerDisconnected(APlayerController* PlayerController)
{
    if (PlayerController)
    {
        UE_LOG(LogTemp, Log, TEXT("Player disconnected: %s"), *PlayerController->GetName());
        
        // 處理玩家斷開邏輯
    }
}
```

---

## 系統集成

### 核心系統集成

#### 配置管理器集成

```cpp
// 在任何需要配置的類中集成
class AMyActor : public AActor
{
protected:
    UPROPERTY()
    TObjectPtr<UMingConfigurationManager> ConfigManager;
    
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // 獲取配置管理器
        ConfigManager = GetGameInstance()->GetSubsystem<UMingConfigurationManager>();
        
        if (ConfigManager)
        {
            // 讀取配置
            FString GameMode = ConfigManager->GetConfigurationValue(TEXT("GameMode"), TEXT("Default"));
            int32 MaxPlayers = ConfigManager->GetIntConfiguration(TEXT("MaxPlayers"), 4);
            bool bEnableVR = ConfigManager->GetBoolConfiguration(TEXT("EnableVR"), false);
            
            // 應用配置
            ApplyGameSettings(GameMode, MaxPlayers, bEnableVR);
        }
    }
    
    UFUNCTION()
    void ApplyGameSettings(const FString& GameMode, int32 MaxPlayers, bool bEnableVR)
    {
        // 應用遊戲設置邏輯
        UE_LOG(LogTemp, Log, TEXT("Applying settings: Mode=%s, MaxPlayers=%d, VR=%s"), 
            *GameMode, MaxPlayers, bEnableVR ? TEXT("true") : TEXT("false"));
    }
};
```

#### 事件系統集成

```cpp
// 創建自定義事件管理器
class UMyEventManager : public UObject
{
    GENERATED_BODY()

public:
    // 聲明自定義事件
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMyCustomEvent, const FString&, EventData, int32, Priority);
    
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMyCustomEvent OnMyCustomEvent;
    
    // 觸發事件
    UFUNCTION(BlueprintCallable, Category = "Events")
    void TriggerCustomEvent(const FString& EventData, int32 Priority = 0);
    
    // 註冊事件監聽器
    UFUNCTION(BlueprintCallable, Category = "Events")
    void RegisterEventListener(UObject* Listener);
    
    // 註銷事件監聽器
    UFUNCTION(BlueprintCallable, Category = "Events")
    void UnregisterEventListener(UObject* Listener);
};
```

### 多人遊戲集成

#### 網絡系統集成

```cpp
// 網絡遊戲模式
class AMyNetworkGameMode : public AGameModeBase
{
protected:
    UPROPERTY()
    TObjectPtr<UMingNetworkManager> NetworkManager;
    
    UPROPERTY()
    TObjectPtr<UMingCooperativeManager> CooperativeManager;
    
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // 初始化網絡系統
        InitializeNetworkSystems();
    }
    
    UFUNCTION()
    void InitializeNetworkSystems()
    {
        UGameInstance* GameInstance = GetGameInstance();
        
        if (GameInstance)
        {
            // 獲取網絡管理器
            NetworkManager = GameInstance->GetSubsystem<UMingNetworkManager>();
            CooperativeManager = GameInstance->GetSubsystem<UMingCooperativeManager>();
            
            if (NetworkManager && CooperativeManager)
            {
                // 初始化網絡
                NetworkManager->InitializeNetwork();
                
                // 綁定網絡事件
                NetworkManager->OnPlayerConnected.AddDynamic(this, &AMyNetworkGameMode::OnPlayerConnected);
                NetworkManager->OnPlayerDisconnected.AddDynamic(this, &AMyNetworkGameMode::OnPlayerDisconnected);
                
                // 創建合作會話
                FMingCooperativeSessionInfo SessionInfo;
                SessionInfo.SessionName = TEXT("Coop Game");
                SessionInfo.MaxPlayers = 4;
                SessionInfo.bIsPrivate = false;
                
                CooperativeManager->CreateCooperativeSession(SessionInfo);
            }
        }
    }
    
    UFUNCTION()
    void OnPlayerConnected(int32 PlayerID)
    {
        UE_LOG(LogTemp, Log, TEXT("Network player %d connected"), PlayerID);
        
        // 處理網絡玩家連接
        HandleNetworkPlayerConnection(PlayerID);
    }
    
    UFUNCTION()
    void OnPlayerDisconnected(int32 PlayerID)
    {
        UE_LOG(LogTemp, Log, TEXT("Network player %d disconnected"), PlayerID);
        
        // 處理網絡玩家斷開
        HandleNetworkPlayerDisconnection(PlayerID);
    }
};
```

### 教育系統集成

#### 教育模式集成

```cpp
// 教育遊戲模式
class AMyEducationalGameMode : public AGameModeBase
{
protected:
    UPROPERTY()
    TObjectPtr<UMingEducationalManager> EducationalManager;
    
    UPROPERTY()
    TObjectPtr<UMingVRManager> VRManager;
    
    UPROPERTY()
    TObjectPtr<UMingARManager> ARManager;
    
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        
        // 初始化教育系統
        InitializeEducationalSystems();
    }
    
    UFUNCTION()
    void InitializeEducationalSystems()
    {
        UGameInstance* GameInstance = GetGameInstance();
        
        if (GameInstance)
        {
            // 獲取教育系統
            EducationalManager = GameInstance->GetSubsystem<UMingEducationalManager>();
            VRManager = GameInstance->GetSubsystem<UMingVRManager>();
            ARManager = GameInstance->GetSubsystem<UMingARManager>();
            
            if (EducationalManager)
            {
                // 啟用教育模式
                EducationalManager->EnableEducationalMode(EMingEducationalMode::GuidedLearning);
                EducationalManager->SetEducationalDifficulty(EMingEducationalDifficulty::Adaptive);
                
                // 綁定教育事件
                EducationalManager->OnLessonStarted.AddDynamic(this, &AMyEducationalGameMode::OnLessonStarted);
                EducationalManager->OnLessonCompleted.AddDynamic(this, &AMyEducationalGameMode::OnLessonCompleted);
                
                // 開始初始課程
                StartInitialLesson();
            }
            
            // 檢查 VR/AR 可用性
            if (VRManager && VRManager->IsVRAvailable())
            {
                VRManager->SetVRMode(EMingVRMode::RoomScale);
            }
            
            if (ARManager && ARManager->IsARAvailable())
            {
                FMingARSessionConfig ARConfig;
                ARConfig.bAREducationalMode = true;
                ARManager->StartARSession(ARConfig);
            }
        }
    }
    
    UFUNCTION()
    void StartInitialLesson()
    {
        if (EducationalManager)
        {
            // 獲取可用課程
            TArray<FMingEducationalLesson> AvailableLessons = EducationalManager->GetAvailableLessons();
            
            if (AvailableLessons.Num() > 0)
            {
                // 開始第一個課程
                EducationalManager->StartLesson(AvailableLessons[0].LessonID);
            }
        }
    }
    
    UFUNCTION()
    void OnLessonStarted(const FMingEducationalLesson& Lesson)
    {
        UE_LOG(LogTemp, Log, TEXT("Educational lesson started: %s"), *Lesson.Title);
        
        // 處理課程開始邏輯
        HandleLessonStart(Lesson);
    }
    
    UFUNCTION()
    void OnLessonCompleted(const FMingEducationalLesson& Lesson)
    {
        UE_LOG(LogTemp, Log, TEXT("Educational lesson completed: %s"), *Lesson.Title);
        
        // 處理課程完成邏輯
        HandleLessonCompletion(Lesson);
        
        // 開始下一個課程
        StartNextLesson();
    }
};
```

---

## 最佳實踐

### 代碼規範

#### C++ 編碼標準

```cpp
// 1. 使用統一的頭文件保護
#pragma once

// 2. 包含必要的頭文件
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyClass.generated.h"

// 3. 類聲明
UCLASS(ClassGroup = (MyGroup), BlueprintType, Blueprintable)
class MINGGORTS_API UMyClass : public UObject
{
    GENERATED_BODY()

public:
    // 構造函數
    UMyClass();

    // 虛擬析構函數
    virtual ~UMyClass() override;

    // Blueprint 可調用函數
    UFUNCTION(BlueprintCallable, Category = "My Class")
    void MyFunction(const FString& Parameter);

    // Blueprint 純函數
    UFUNCTION(BlueprintPure, Category = "My Class")
    FString GetMyValue() const;

protected:
    // 受保護的成員變量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Class")
    FString MyValue;

private:
    // 私有成員變量
    int32 InternalCounter;
};
```

#### Blueprint 最佳實踐

```cpp
// 1. 使用適當的函數修飾符
UFUNCTION(BlueprintCallable, Category = "My Category")
void MyCallableFunction();

UFUNCTION(BlueprintPure, Category = "My Category")
int32 MyPureFunction();

UFUNCTION(BlueprintImplementableEvent, Category = "My Category")
void MyBlueprintEvent();

// 2. 使用適當的屬性修飾符
UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My Category")
FString MyProperty;

UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "My Category")
int32 ReadOnlyProperty;

UPROPERTY(BlueprintAssignable, Category = "My Category")
FMyDelegate MyDelegate;
```

### 性能優化

#### 內存管理

```cpp
// 1. 使用智能指針
TSharedPtr<FMyData> DataPtr = MakeShared<FMyData>();

// 2. 使用弱指針避免循環引用
TWeakObjectPtr<UMyObject> WeakPtr = MyObject;

// 3. 及時釋放資源
virtual void BeginDestroy() override
{
    // 清理資源
    MyTexture = nullptr;
    MyMesh = nullptr;
    
    Super::BeginDestroy();
}
```

#### 渲染優化

```cpp
// 1. 使用對象池
class UMyObjectPool : public UObject
{
private:
    TArray<TObjectPtr<UMyPooledObject>> AvailableObjects;
    
public:
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    UMyPooledObject* GetPooledObject()
    {
        if (AvailableObjects.Num() > 0)
        {
            UMyPooledObject* Object = AvailableObjects.Last();
            AvailableObjects.Pop();
            Object->Activate();
            return Object;
        }
        return nullptr;
    }
    
    UFUNCTION(BlueprintCallable, Category = "Object Pool")
    void ReturnToPool(UMyPooledObject* Object)
    {
        if (Object)
        {
            Object->Deactivate();
            AvailableObjects.Add(Object);
        }
    }
};
```

### 錯誤處理

#### 異常處理

```cpp
// 1. 使用 UE_LOG 進行日誌記錄
void MyFunction()
{
    if (!ValidateInput())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid input in MyFunction"));
        return;
    }
    
    try
    {
        // 執行可能失敗的操作
        PerformRiskyOperation();
    }
    catch (const std::exception& e)
    {
        UE_LOG(LogTemp, Error, TEXT("Exception in MyFunction: %s"), UTF8_TO_TCHAR(e.what()));
    }
}

// 2. 使用斷言進行調試
void MyDebugFunction()
{
    check(MyPointer != nullptr);
    ensure(MyArray.Num() > 0);
    verify(MyCondition == true);
}
```

---

## 調試技巧

### 日誌記錄

```cpp
// 1. 使用不同的日誌級別
UE_LOG(LogTemp, Log, TEXT("This is a log message"));
UE_LOG(LogTemp, Warning, TEXT("This is a warning message"));
UE_LOG(LogTemp, Error, TEXT("This is an error message"));

// 2. 使用自定義日誌類別
DEFINE_LOG_CATEGORY_STATIC(MyLog, Log, All);

UE_LOG(MyLog, Log, TEXT("Custom log message"));
UE_LOG(MyLog, Warning, TEXT("Custom warning message"));
```

### 調試工具

```cpp
// 1. 使用 GEngine 添加螢幕調試信息
if (GEngine)
{
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Debug Message"));
}

// 2. 使用 DrawDebug 系列函數
DrawDebugSphere(GetWorld(), Location, Radius, 12, FColor::Red, false, 5.0f);
DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.0f);
DrawDebugBox(GetWorld(), Center, Extent, FColor::Blue, false, 5.0f);
```

### 性能分析

```cpp
// 1. 使用計時器
SCOPE_CYCLE_COUNTER(STAT_MyFunctionTime);

void MyFunction()
{
    // 函數實現
}

// 2. 使用 CPU 時間統計
DECLARE_CYCLE_STAT(TEXT("My Function Time"), STAT_MyFunctionTime);

// 3. 使用內存統計
DECLARE_MEMORY_STAT(TEXT("My Memory Usage"), STAT_MyMemoryUsage);
```

---

## 性能優化

### 多線程

```cpp
// 1. 使用 AsyncTask
AsyncTask(ENamedThreads::AnyBackgroundThread, [this]()
{
    // 後台線程執行
    BackgroundOperation();
    
    // 回到主線程
    AsyncTask(ENamedThreads::GameThread, [this]()
    {
        // 主線程執行
        UpdateGameThread();
    });
});

// 2. 使用 FRunnable
class FMyBackgroundTask : public FRunnable
{
public:
    virtual uint32 Run() override
    {
        // 後台任務實現
        return 0;
    }
    
    virtual void Stop() override
    {
        // 停止任務
    }
};
```

### 資源管理

```cpp
// 1. 使用資源流
class UMyResourceManager : public UObject
{
private:
    TMap<FString, TWeakObjectPtr<UObject>> LoadedResources;
    
public:
    template<typename T>
    T* LoadResource(const FString& ResourcePath)
    {
        if (LoadedResources.Contains(ResourcePath))
        {
            return Cast<T>(LoadedResources[ResourcePath].Get());
        }
        
        T* Resource = LoadObject<T>(nullptr, *ResourcePath);
        if (Resource)
        {
            LoadedResources.Add(ResourcePath, Resource);
        }
        
        return Resource;
    }
    
    void UnloadResource(const FString& ResourcePath)
    {
        if (LoadedResources.Contains(ResourcePath))
        {
            LoadedResources.Remove(ResourcePath);
        }
    }
};
```

---

## 常見問題

### 編譯問題

#### Q: 編譯時出現 "unresolved external symbol" 錯誤
**A**: 檢查以下項目：
1. 確保所有必要的模塊在 `.Build.cs` 文件中聲明
2. 檢查頭文件包含路徑是否正確
3. 確保生成的 `.generated.h` 文件是最新的

#### Q: 鏈接錯誤 "cannot open file 'libxxx.lib'"
**A**: 解決方案：
1. 檢查第三方庫路徑配置
2. 確保庫文件版本兼容
3. 檢查平台特定的庫配置

### 運行時問題

#### Q: 遊戲啟動時崩潰
**A**: 調試步驟：
1. 檢查日誌文件中的錯誤信息
2. 使用調試器逐步執行
3. 檢查 BeginPlay 中的初始化代碼
4. 驗證資源加載是否成功

#### Q: 網絡連接失敗
**A**: 檢查項目：
1. 防火牆設置
2. 網絡端口是否開放
3. NAT 類型是否支持
4. 服務器狀態是否正常

### 性能問題

#### Q: 幀率過低
**A**: 優化建議：
1. 使用 Stat 命令查看性能瓶頸
2. 檢查 DrawCall 數量
3. 優化材質和著色器
4. 使用對象池減少 GC 壓力

#### Q: 內存泄漏
**A**: 檢查方法：
1. 使用內存分析工具
2. 檢查 UObject 引用計數
3. 確保及時釋放資源
4. 使用智能指針避免內存泄漏

---

## 進階主題

### 插件開發

```cpp
// 1. 創建插件描述文件
{
    "FileVersion": 3,
    "Version": 1,
    "VersionName": "1.0.0",
    "FriendlyName": "My Plugin",
    "Description": "A sample plugin for MingGoRTS",
    "Category": "Game",
    "CreatedBy": "Your Name",
    "CreatedByURL": "https://yourwebsite.com",
    "DocsURL": "https://docs.yourwebsite.com",
    "MarketplaceURL": "",
    "SupportURL": "",
    "CanContainContent": true,
    "IsBetaVersion": false,
    "IsExperimentalVersion": false,
    "Installed": false,
    "Modules": [
        {
            "Name": "MyPlugin",
            "Type": "Runtime",
            "LoadingPhase": "Default",
            "SupportedTargetPlatforms": [
                "Win64",
                "Mac",
                "Linux"
            ]
        }
    ]
}
```

### 自動化測試

```cpp
// 1. 創建測試類
IMPLEMENT_SIMPLE_AUTOMATION_TEST(FMySubsystemTest, "MingGoRTS.MySubsystem.Basic")

bool FMySubsystemTest::RunTest(const FString& Parameters)
{
    // 測試實現
    UMySubsystem* Subsystem = GetGameInstance()->GetSubsystem<UMySubsystem>();
    
    if (!Subsystem)
    {
        AddError(TEXT("Subsystem not found"));
        return false;
    }
    
    // 執行測試邏輯
    bool TestResult = Subsystem->DoSomething();
    
    if (!TestResult)
    {
        AddError(TEXT("DoSomething returned false"));
        return false;
    }
    
    AddWarning(TEXT("Test passed"));
    return true;
}
```

---

## 新系統集成

### AI內容質量控制系統

#### 系統概述
AI內容質量控制系統提供多供應商AI內容生成和質量管理功能。

#### 集成步驟

1. **初始化系統**
```cpp
// 在遊戲模式中初始化
UMingAIContentQualityController* QualityController = NewObject<UMingAIContentQualityController>();
QualityController->InitializeAIContentQualityController();
```

2. **配置供應商**
```cpp
// 配置AI供應商
FProviderConfig Config;
Config.APIKey = TEXT("your-api-key");
Config.EndpointURL = TEXT("https://api.example.com");
Config.MaxRequestsPerMinute = 60;

QualityController->ConfigureProvider(EAIProvider::StableDiffusion, Config);
```

3. **生成內容**
```cpp
// 生成內容並進行質量控制
FContentGenerationRequest Request;
Request.ContentType = EContentType::Image;
Request.Description = TEXT("Ming dynasty warrior");
Request.QualityLevel = EQualityLevel::High;

QualityController->GenerateContentWithQualityControl(Request);
```

#### 最佳實踐
- 使用異步操作避免阻塞主線程
- 實施預算控制避免超額使用
- 監控供應商性能和可靠性
- 實施內容緩存提高效率

### 多人遊戲系統

#### 系統概述
多人遊戲系統提供完整的網絡同步和多人關係管理功能。

#### 集成步驟

1. **初始化多人系統**
```cpp
UMingMultiplayerSystem* MultiplayerSystem = NewObject<UMingMultiplayerSystem>();
MultiplayerSystem->InitializeMultiplayerSystem();

// 綁定事件
MultiplayerSystem->OnPlayerConnected.AddDynamic(this, &AMyGameMode::OnPlayerConnected);
MultiplayerSystem->OnPlayerDisconnected.AddDynamic(this, &AMyGameMode::OnPlayerDisconnected);
```

2. **創建會話**
```cpp
FSessionSettings Settings;
Settings.SessionName = TEXT("MingGoRTS Game");
Settings.MaxPlayers = 8;
Settings.NetworkMode = ENetworkMode::Internet;

MultiplayerSystem->CreateSession(Settings);
```

3. **數據同步**
```cpp
// 同步玩家位置
FSynchronizationData SyncData;
SyncData.DataType = TEXT("PlayerPosition");
SyncData.TargetPlayerID = PlayerID;
// 設置數據內容

MultiplayerSystem->SynchronizeData(SyncData);

// 同步關係數據
FRelationshipSyncData RelationshipData;
RelationshipData.PlayerID = PlayerID;
RelationshipData.TargetPlayerID = TargetID;
RelationshipData.RelationshipValue = 75.0f;

MultiplayerSystem->SynchronizeRelationship(RelationshipData);
```

#### 最佳實踐
- 使用適當的同步類型（實時、增量、事件驅動）
- 實施網絡優化策略
- 處理網絡中斷和重連
- 監控網絡性能和質量

### 技術債務管理系統

#### 系統概述
技術債務管理系統提供代碼質量監控、技術債務追蹤和自動化優化功能。

#### 集成步驟

1. **初始化系統**
```cpp
UMingTechnicalDebtManager* DebtManager = NewObject<UMingTechnicalDebtManager>();
DebtManager->InitializeTechnicalDebtManager();

// 綁定事件
DebtManager->OnTechnicalDebtIdentified.AddDynamic(this, &AMyGameMode::OnTechnicalDebtIdentified);
```

2. **自動分析**
```cpp
// 啟用自動分析
DebtManager->AutoIdentifyTechnicalDebt();

// 分析特定文件
DebtManager->AnalyzeCodeQuality(TEXT("Source/GameCode.cpp"));

// 分析性能
DebtManager->AnalyzePerformance();
```

3. **優化計劃**
```cpp
// 創建優化計劃
TArray<FString> TargetFiles = {TEXT("File1.cpp"), TEXT("File2.cpp")};
DebtManager->CreateOptimizationPlan(TargetFiles, EOptimizationType::CodeRefactoring);

// 執行優化計劃
FString PlanID = TEXT("plan-123");
DebtManager->ExecuteOptimizationPlan(PlanID);
```

#### 最佳實踐
- 定期運行代碼質量分析
- 監控技術債務趨勢
- 優先處理高優先級技術債務
- 使用自動化工具提高效率

### 系統間集成

#### 統一初始化
```cpp
// 在遊戲模式中統一初始化所有系統
void AMingGameMode::InitializeSystems()
{
    // 初始化AI內容質量控制
    AIQualityController = NewObject<UMingAIContentQualityController>();
    AIQualityController->InitializeAIContentQualityController();
    
    // 初始化多人系統
    MultiplayerSystem = NewObject<UMingMultiplayerSystem>();
    MultiplayerSystem->InitializeMultiplayerSystem();
    
    // 初始化技術債務管理
    TechnicalDebtManager = NewObject<UMingTechnicalDebtManager>();
    TechnicalDebtManager->InitializeTechnicalDebtManager();
    
    // 設置系統間通信
    SetupSystemCommunication();
}
```

#### 事件協調
```cpp
void AMingGameMode::SetupSystemCommunication()
{
    // AI內容生成完成後通知多人系統
    AIQualityController->OnContentQualityChecked.AddDynamic(this, &AMingGameMode::OnContentQualityChecked);
    
    // 多人系統連接變化時通知技術債務系統
    MultiplayerSystem->OnPlayerConnected.AddDynamic(this, &AMingGameMode::OnPlayerConnected);
    
    // 技術債務系統發現問題時通知其他系統
    TechnicalDebtManager->OnTechnicalDebtIdentified.AddDynamic(this, &AMingGameMode::OnTechnicalDebtIdentified);
}
```

---

## 社區資源

### 官方資源
- **文檔網站**: https://docs.minggorts.com
- **API 參考**: https://api.minggorts.com
- **論壇**: https://forum.minggorts.com
- **Discord**: https://discord.gg/minggorts

### 第三方工具
- **Unreal Engine 文檔**: https://docs.unrealengine.com
- **Blueprint 工具**: https://blueprintue.com
- **性能分析工具**: Unreal Insights, RenderDoc

### 學習資源
- **Unreal Engine 學習路徑**: https://learn.unrealengine.com
- **C++ 編程指南**: https://docs.unrealengine.com/en-US/Programming/index.html
- **Blueprint 開發指南**: https://docs.unrealengine.com/en-US/Blueprints/index.html

---

## 貢獻指南

如果您想為 MingGoRTS 項目做出貢獻：

### 代碼貢獻
1. Fork 項目倉庫
2. 創建功能分支
3. 提交您的更改
4. 創建 Pull Request
5. 等待代碼審查

### 文檔貢獻
1. 改進現有文檔
2. 添加新的示例
3. 報告文檔問題
4. 提交文檔更新

### 錯誤報告
1. 使用 GitHub Issues
2. 提供詳細的重現步驟
3. 包含系統信息
4. 添加相關日誌

---

*本指南最後更新：2026年3月23日*
