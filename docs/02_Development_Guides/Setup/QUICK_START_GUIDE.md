# MingGoRTS 快速入門指南

## 5分鐘快速開始

### 步驟 1: 複製示例 GameMode

將 `Source/MingGoRTS/Examples/MingGoRTSExampleGameMode.h` 和 `.cpp` 複製到您的專案中。

### 步驟 2: 配置 GameMode

在您的項目設置中使用示例 GameMode：

```cpp
// 在您的 GameConfig.ini 中
[/Script/Engine.WorldSettings]
DefaultGameMode=/Script/MingGoRTS.MingGoRTSExampleGameMode
```

### 步驟 3: 運行遊戲

啟動遊戲後，您將看到所有系統初始化日誌：

```
=== MingGoRTS Example GameMode Initializing ===
Initializing all MingGoRTS systems...
✓ Performance System initialized
✓ Security System initialized
✓ Tutorial System initialized
✓ Replay System initialized
✓ Achievement & Statistics System initialized
✓ Spectator System initialized
✓ Telemetry System initialized
✓ Mod System initialized
✓ Tournament System initialized
✓ Cloud System initialized
✓ VR/AR Support System initialized
✓ Multiplayer System initialized
=== All systems initialized successfully ===
```

---

## 常用功能示例

### 解鎖成就

```cpp
// 當玩家完成某個目標時
AchievementSystem->UnlockAchievement(TEXT("ACH_FIRST_VICTORY"));

// 或者更新進度
AchievementSystem->UpdateAchievementProgress(TEXT("ACH_KILL_100"), 1);
```

### 記錄回放

```cpp
// 開始錄製
ReplaySystem->StartRecording(TEXT("MATCH_001"));

// 記錄事件
TMap<FString, FString> Params;
Params.Add(TEXT("unit_type"), TEXT("cavalry"));
ReplaySystem->RecordGameEvent(EReplayEventType::UnitSpawned, TEXT("UnitSpawn"), Params);

// 結束錄製
FReplayMetadata Metadata = ReplaySystem->StopRecording();
```

### 性能監控

```cpp
// 開始監控
PerformanceSystem->StartPerformanceMonitoring();

// 獲取當前性能
FPerformanceMetrics Metrics = PerformanceSystem->GetCurrentMetrics();
UE_LOG(LogTemp, Log, TEXT("FPS: %.1f"), Metrics.CurrentFPS);

// 自動優化
if (Metrics.CurrentFPS < 30.0f)
{
    PerformanceSystem->AutoOptimize();
}
```

### 多人遊戲

```cpp
// 連接到服務器
MultiplayerSystem->ConnectToServer(TEXT("127.0.0.1"), 7777);

// 創建房間
FMultiplayerSessionConfig Config;
Config.SessionName = TEXT("My Game Room");
Config.GameMode = EMultiplayerGameMode::Cooperative;
Config.MaxPlayers = 4;
FString RoomID = MultiplayerSystem->CreateRoom(Config);

// 發送消息
MultiplayerSystem->SendChatMessage(TEXT("Hello everyone!"));
```

### 創建模組

```cpp
// 創建本地模組
FString ModID = ModSystem->CreateLocalMod(
    TEXT("My Custom Map"),
    EModType::Map,
    TEXT("A custom battle map")
);

// 啟用模組
ModSystem->EnableMod(ModID);

// 上傳到創意工坊
ModSystem->UploadMod(ModID, TEXT("Description here"), {TEXT("map"), TEXT("battle")});
```

---

## Blueprint 使用

所有系統都支持 Blueprint 調用：

1. 在 Blueprint 中獲取 GameMode
2. 調用 `Get[System]System()` 函數獲取系統實例
3. 使用系統提供的 BlueprintCallable 函數

### Blueprint 示例：解鎖成就

```
[Event Begin Play]
    |
    v
[Get Game Mode] -> [Get Achievement System]
    |
    v
[Unlock Achievement]
    Achievement ID: "ACH_FIRST_LOGIN"
```

---

## 事件綁定

### C++ 事件綁定

```cpp
// 在初始化時綁定
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    AchievementSystem->OnAchievementUnlocked.AddDynamic(
        this, &AMyGameMode::OnAchievementEarned
    );
}

// 事件處理函數
void AMyGameMode::OnAchievementEarned(const FString& AchievementID, const FAchievementData& Data)
{
    UE_LOG(LogTemp, Log, TEXT("恭喜！解鎖成就: %s"), *Data.Name.ToString());
}
```

### Blueprint 事件

在 Blueprint 中選擇系統組件，點擊事件圖標 (+) 添加事件節點。

---

## 調試技巧

### 啟用詳細日誌

```cpp
// 在您的代碼中設置日誌級別
UE_LOG(LogTemp, Verbose, TEXT("詳細調試信息"));
UE_LOG(LogTemp, Log, TEXT("普通日誌"));
UE_LOG(LogTemp, Warning, TEXT("警告信息"));
UE_LOG(LogTemp, Error, TEXT("錯誤信息"));
```

### 性能分析

```cpp
// 使用 Unreal Insights 或內置分析器
PerformanceSystem->ExportPerformanceReport(TEXT("C:/PerfReport.txt"));
```

---

## 常見問題

### Q: 系統初始化失敗怎麼辦？
**A:** 檢查日誌輸出，確保所有系統的依賴已正確加載。確認在 `InitGame` 中正確創建系統實例。

### Q: Blueprint 無法找到系統函數？
**A:** 確認函數標記為 `UFUNCTION(BlueprintCallable)`，並且類別設置正確。

### Q: 事件沒有觸發？
**A:** 檢查事件綁定是否在系統初始化之後進行。確認使用 `AddDynamic` 正確綁定。

### Q: 內存使用過高？
**A:** 使用 `PerformanceSystem->ForceGarbageCollection()` 手動觸發垃圾回收，或啟用自動優化。

---

## 下一步

- 閱讀 [API 完整文檔](API_DOCUMENTATION.md)
- 查看 [示例代碼](../Source/MingGoRTS/Examples/)
- 訪問開發者論壇獲取更多幫助

---

## 版本信息

**指南版本**: 1.0.0  
**最後更新**: 2026-03-23  
**適用版本**: MingGoRTS v0.1.0+

---

*祝開發順利！*
