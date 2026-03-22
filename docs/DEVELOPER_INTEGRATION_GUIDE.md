# MingGoRTS 開發者集成指南

## 目標讀者

本指南適合以下開發者：
- 需要將 MingGoRTS 系統集成到現有專案
- 希望自定義和擴展系統功能
- 需要理解系統間的交互關係

---

## 系統依賴圖

```
┌─────────────────────────────────────────────────────────────┐
│                      Core Systems                          │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │  Performance │───▶│   Security   │───▶│   Tutorial   │  │
│  │   System     │    │   System     │    │   System     │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│         │                   │                   │          │
│         ▼                   ▼                   ▼          │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │    Replay    │    │  Achievement │    │   Cloud      │  │
│  │   System     │    │  & Statistics │    │   System     │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│         │                   │                   │          │
│         ▼                   ▼                   ▼          │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │  Spectator   │◄───│  Telemetry   │───▶│   Mod        │  │
│  │   System     │    │   System     │    │   System     │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│                                                             │
│  ┌──────────────┐    ┌──────────────┐    ┌──────────────┐  │
│  │  Tournament  │    │  Multiplayer │    │  VR/AR       │  │
│  │   System     │◄───│   System     │───▶│  Support     │  │
│  └──────────────┘    └──────────────┘    └──────────────┘  │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

**說明**: 
- 箭頭表示數據流向或依賴關係
- 性能系統是所有系統的基礎
- 遙測系統收集所有系統的數據

---

## 集成步驟

### 步驟 1: 添加依賴

在您的 `.Build.cs` 文件中添加：

```csharp
PublicDependencyModuleNames.AddRange(new string[] {
    "MingGoRTS",
    "MingGoRTSEditor" // 如果需要編輯器支持
});
```

### 步驟 2: 包含頭文件

在您的 GameMode 或 GameInstance 中：

```cpp
#include "Tutorial/MingRTSTutorialSystem.h"
#include "Security/MingRTSSecurityManager.h"
#include "Replay/MingRTSReplaySystem.h"
#include "Stats/MingRTSAchievementSystem.h"
#include "Stats/MingRTSStatisticsSystem.h"
#include "Spectator/MingRTSSpectatorSystem.h"
#include "Telemetry/MingRTSTelemetrySystem.h"
#include "Mod/MingRTSModSystem.h"
#include "Tournament/MingRTSTournamentSystem.h"
#include "Cloud/MingRTSCloudSystem.h"
#include "XR/MingRTSVRSupportSystem.h"
#include "Multiplayer/MingRTSMultiplayerSupportSystem.h"
#include "Performance/MingRTSPerformanceSystem.h"
```

### 步驟 3: 初始化系統

推薦在 `InitGame` 中初始化：

```cpp
void AYourGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);
    
    InitializeMingGoRTSSystems();
}

void AYourGameMode::InitializeMingGoRTSSystems()
{
    // 1. 性能系統（最先初始化）
    PerformanceSystem = NewObject<UMingRTSPerformanceSystem>(this);
    PerformanceSystem->InitializePerformanceSystem();
    PerformanceSystem->StartPerformanceMonitoring();
    
    // 2. 安全系統
    SecuritySystem = NewObject<UMingRTSSecurityManager>(this);
    SecuritySystem->InitializeSecuritySystem();
    
    // 3. 教學系統
    TutorialSystem = NewObject<UMingRTSTutorialSystem>(this);
    TutorialSystem->InitializeTutorialSystem();
    
    // ... 其他系統
}
```

### 步驟 4: 綁定事件

在 `StartPlay` 或 `BeginPlay` 中：

```cpp
void AYourGameMode::StartPlay()
{
    Super::StartPlay();
    
    BindSystemEvents();
}

void AYourGameMode::BindSystemEvents()
{
    // 教學事件
    TutorialSystem->OnTutorialStepCompleted.AddDynamic(
        this, &AYourGameMode::HandleTutorialStepCompleted
    );
    
    // 成就事件
    AchievementSystem->OnAchievementUnlocked.AddDynamic(
        this, &AYourGameMode::HandleAchievementUnlocked
    );
    
    // 性能事件
    PerformanceSystem->OnPerformanceLevelChanged.AddDynamic(
        this, &AYourGameMode::HandlePerformanceLevelChanged
    );
    
    // ... 其他事件
}
```

---

## 自定義系統行為

### 擴展教學系統

```cpp
// 創建自定義教學條件
UCLASS()
class UMyCustomTutorialCondition : public UObject
{
    GENERATED_BODY()
    
public:
    UFUNCTION(BlueprintCallable)
    bool CheckCondition(const TMap<FString, FString>& Params);
};

// 在 GameMode 中
void AYourGameMode::RegisterCustomTutorialSteps()
{
    // 註冊自定義教學步驟
    TMap<FString, FString> Params;
    Params.Add(TEXT("custom_param"), TEXT("value"));
    
    TutorialSystem->RegisterTutorialStep(
        TEXT("CUSTOM_STEP"),
        TEXT("自定義教學步驟"),
        TEXT("CustomCondition"),
        Params
    );
}
```

### 自定義成就

```cpp
// 在 GameMode 初始化時加載
void AYourGameMode::LoadCustomAchievements()
{
    TArray<FAchievementData> CustomAchievements;
    
    FAchievementData Achievement1;
    Achievement1.AchievementID = TEXT("ACH_CUSTOM_001");
    Achievement1.Name = FText::FromString(TEXT("自定義成就"));
    Achievement1.Description = FText::FromString(TEXT("完成自定義任務"));
    Achievement1.Rarity = EAchievementRarity::Rare;
    Achievement1.Type = EAchievementType::Custom;
    Achievement1.bHidden = false;
    Achievement1.UnlockCondition = TEXT("CustomCondition");
    
    CustomAchievements.Add(Achievement1);
    
    AchievementSystem->LoadAchievementDefinitions(CustomAchievements);
}
```

---

## 系統間通信

### 教學與成就聯動

```cpp
void AYourGameMode::OnTutorialCompleted(const FString& TutorialID)
{
    // 教學完成時解鎖成就
    if (TutorialID == TEXT("TUTORIAL_BASIC"))
    {
        AchievementSystem->UnlockAchievement(TEXT("ACH_TUTORIAL_COMPLETE"));
    }
}
```

### 性能與回放聯動

```cpp
void AYourGameMode::OnPerformanceWarning(EPerformanceLevel Level)
{
    // 性能問題時記錄到回放
    if (Level == EPerformanceLevel::Critical)
    {
        TMap<FString, FString> Params;
        Params.Add(TEXT("performance_level"), FString::FromInt((int32)Level));
        
        ReplaySystem->RecordGameEvent(
            EReplayEventType::PerformanceWarning,
            TEXT("LowPerformance"),
            Params
        );
    }
}
```

### 多人與統計聯動

```cpp
void AYourGameMode::OnMatchEnd(const FString& WinnerID)
{
    // 比賽結束時記錄統計
    for (const FNetworkPlayerInfo& Player : MultiplayerSystem->GetAllPlayers())
    {
        bool bWon = (Player.PlayerID == WinnerID);
        StatisticsSystem->RecordMatchResult(
            MatchID,
            Player.PlayerID,
            bWon,
            Player.Score
        );
    }
}
```

---

## 配置管理

### 使用配置文件

```ini
; DefaultGame.ini
[/Script/MingGoRTS.MingRTSPerformanceSystem]
TargetFPS=60
MinFPS=30
MaxMemoryMB=4096

[/Script/MingGoRTS.MingRTSTelemetrySystem]
bTelemetryEnabled=true
BatchSize=100
FlushInterval=30.0

[/Script/MingGoRTS.MingRTSAchievementSystem]
bShowUnlockNotifications=true
NotificationDuration=5.0
```

### 運行時配置

```cpp
void AYourGameMode::LoadConfiguration()
{
    // 從配置文件讀取
    int32 TargetFPS = 60;
    GConfig->GetInt(
        TEXT("/Script/MingGoRTS.MingRTSPerformanceSystem"),
        TEXT("TargetFPS"),
        TargetFPS,
        GGameIni
    );
    
    PerformanceSystem->SetTargetFPS(TargetFPS);
}
```

---

## 最佳實踐

### 1. 錯誤處理

```cpp
void AYourGameMode::InitializeSystemsSafely()
{
    // 使用 try-catch 或檢查返回值
    if (!PerformanceSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to create PerformanceSystem"));
        return;
    }
    
    PerformanceSystem->InitializePerformanceSystem();
    
    // 檢查初始化是否成功
    if (PerformanceSystem->GetPerformanceLevel() == EPerformanceLevel::Critical)
    {
        UE_LOG(LogTemp, Warning, TEXT("Performance system initialized with warnings"));
    }
}
```

### 2. 內存管理

```cpp
void AYourGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 正確關閉系統
    if (TelemetrySystem)
    {
        TelemetrySystem->EndSession();
    }
    
    if (ReplaySystem && ReplaySystem->IsRecording())
    {
        ReplaySystem->StopRecording();
    }
    
    if (MultiplayerSystem)
    {
        MultiplayerSystem->Disconnect();
    }
    
    // 釋放對象
    PerformanceSystem = nullptr;
    SecuritySystem = nullptr;
    // ...
    
    Super::EndPlay(EndPlayReason);
}
```

### 3. 性能優化

```cpp
void AYourGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 避免每幀都執行重操作
    static float AccumulatedTime = 0.0f;
    AccumulatedTime += DeltaTime;
    
    if (AccumulatedTime >= 1.0f) // 每秒更新一次
    {
        UpdateSystems();
        AccumulatedTime = 0.0f;
    }
}
```

---

## 故障排除

### 問題 1: 系統初始化順序錯誤

**症狀**: 某些系統無法正常工作  
**解決**: 確保按正確順序初始化：
1. PerformanceSystem
2. SecuritySystem
3. TutorialSystem
4. ReplaySystem
5. Achievement/StatisticsSystem
6. 其他系統

### 問題 2: 事件未觸發

**症狀**: 綁定的事件處理函數未被調用  
**解決**: 
- 檢查是否在系統初始化後綁定
- 確認使用正確的宏：`UFUNCTION()`
- 使用 `AddDynamic` 而不是 `Add`

### 問題 3: 內存洩漏

**症狀**: 長時間運行後內存使用持續增長  
**解決**:
- 確保在 `EndPlay` 中正確關閉系統
- 定期調用 `ForceGarbageCollection()`
- 使用 `FlushUnusedResources()`

### 問題 4: 性能下降

**症狀**: FPS 持續下降  
**解決**:
- 啟用性能監控：`StartPerformanceMonitoring()`
- 使用自動優化：`AutoOptimize()`
- 檢查是否有過多的 Draw Calls
- 降低陰影和效果質量

---

## 高級主題

### 自定義網絡協議

```cpp
void AYourGameMode::SetupCustomMultiplayerProtocol()
{
    // 定義自定義消息類型
    TMap<EVRInputType, FName> CustomMapping;
    CustomMapping.Add(EVRInputType::LeftHand, TEXT("CustomLeftAction"));
    CustomMapping.Add(EVRInputType::RightHand, TEXT("CustomRightAction"));
    
    MultiplayerSystem->SetXRInputMapping(CustomMapping);
}
```

### 數據持久化

```cpp
void AYourGameMode::SaveGameState()
{
    // 保存所有系統狀態
    USaveGame* SaveGameInstance = Cast<USaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
    
    UMySaveGame* MySave = Cast<UMySaveGame>(SaveGameInstance);
    MySave->AchievementData = AchievementSystem->GetPlayerAchievementData(LocalPlayerID);
    MySave->StatisticsData = StatisticsSystem->GetPlayerCareerStats(LocalPlayerID);
    MySave->TutorialProgress = TutorialSystem->GetAllTutorialProgress();
    
    UGameplayStatics::SaveGameToSlot(MySave, TEXT("MySaveSlot"), 0);
}
```

---

## 資源和幫助

- **API 文檔**: [API_DOCUMENTATION.md](API_DOCUMENTATION.md)
- **快速入門**: [QUICK_START_GUIDE.md](QUICK_START_GUIDE.md)
- **示例代碼**: `Source/MingGoRTS/Examples/`
- **社區論壇**: [MingGoRTS Dev Forum](https://example.com/forum)
- **問題報告**: [GitHub Issues](https://github.com/example/minggorts/issues)

---

## 版本歷史

| 版本 | 日期 | 變更 |
|------|------|------|
| 1.0.0 | 2026-03-23 | 初始版本，包含所有15個系統的集成指南 |

---

*本指南將隨著系統更新而持續更新*
