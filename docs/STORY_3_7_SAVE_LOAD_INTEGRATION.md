# Story 3.7: 保存和載入系統完善指南

## 概述

MingGoRTS 保存和載入系統（MingRTSSaveLoadEnhancedSystem）提供完整的遊戲進度管理，支持多種數據類型、格式和存儲位置。

---

## 系統架構

```
MingRTSSaveLoadEnhancedSystem (核心管理器)
├── 11種保存數據類型
│   ├── GameState (遊戲狀態)
│   ├── PlayerData (玩家數據)
│   ├── WorldData (世界數據)
│   ├── Inventory (庫存)
│   ├── Quests (任務)
│   ├── Relationships (關係)
│   ├── Campaign (戰役)
│   ├── Settings (設置)
│   ├── Statistics (統計)
│   ├── Custom (自定義)
│   └── All (全部)
├── 10種保存格式
│   ├── Binary (二進制)
│   ├── JSON
│   ├── XML
│   ├── CSV
│   ├── Text
│   ├── Compressed (壓縮)
│   ├── Encrypted (加密)
│   ├── Cloud (雲端)
│   ├── Database (數據庫)
│   └── Custom (自定義)
└── 10種保存位置
    ├── Local (本地)
    ├── Cloud (雲端)
    ├── Network (網絡)
    ├── USB
    ├── Memory (內存)
    ├── Cache (緩存)
    ├── Temp (臨時)
    ├── Backup (備份)
    ├── Archive (存檔)
    └── Custom (自定義)
```

---

## 快速集成

### C++ 集成

```cpp
#include "SaveLoad/MingRTSSaveLoadEnhancedSystem.h"

// 在 GameInstance 中初始化
void UMyGameInstance::Init()
{
    Super::Init();
    
    // 創建保存系統
    SaveSystem = NewObject<UMingRTSSaveLoadEnhancedSystem>(this);
    SaveSystem->InitializeSaveSystem();
    
    // 配置保存設置
    SaveSystem->SetDefaultSaveType(ERTSSaveDataType::All);
    SaveSystem->SetDefaultSaveFormat(ERTSSaveFormat::Binary);
    SaveSystem->SetDefaultSaveLocation(ERTSSaveLocation::Local);
    
    // 啟用自動保存
    SaveSystem->SetAutoSaveEnabled(true);
    SaveSystem->SetAutoSaveInterval(300.0f); // 5分鐘
    
    // 設置最大存檔數量
    SaveSystem->SetMaxSaveSlots(20);
    
    // 啟用備份
    SaveSystem->SetBackupEnabled(true);
    SaveSystem->SetBackupCount(3);
}

void UMyGameInstance::Shutdown()
{
    // 關閉前自動保存
    if (SaveSystem)
    {
        SaveSystem->AutoSave(TEXT("ShutdownAutoSave"));
        SaveSystem->ShutdownSaveSystem();
    }
    
    Super::Shutdown();
}
```

### 藍圖集成

```blueprint
Event Init
├── Create Save System
├── Initialize Save System
├── Set Auto Save Enabled (true)
├── Set Auto Save Interval (300)
└── Set Max Save Slots (20)

Event Shutdown
├── Auto Save (Name: "ShutdownAutoSave")
└── Shutdown Save System
```

---

## 核心功能使用

### 1. 基本保存操作

```cpp
// 創建保存數據
FRTSSaveGameData SaveData;
SaveData.SaveName = TEXT("ManualSave_001");
SaveData.SaveType = ERTSSaveDataType::All;
SaveData.SaveFormat = ERTSSaveFormat::Binary;
SaveData.SaveLocation = ERTSSaveLocation::Local;
SaveData.Description = TEXT("Chapter 2 - Battle of Shanghai");

// 添加截圖（可選）
SaveData.Screenshot = CaptureScreenshot();

// 執行保存
bool bSuccess = SaveSystem->SaveGame(SaveData);

if (bSuccess)
{
    UE_LOG(LogTemp, Log, TEXT("Game saved successfully: %s"), *SaveData.SaveName);
}
```

### 2. 基本載入操作

```cpp
// 檢查存檔是否存在
if (SaveSystem->HasSaveGame(TEXT("ManualSave_001")))
{
    // 載入遊戲
    bool bSuccess = SaveSystem->LoadGame(TEXT("ManualSave_001"));
    
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Game loaded successfully"));
        
        // 恢復遊戲狀態
        RestoreGameState();
    }
}
```

### 3. 自動保存

```cpp
// 手動觸發自動保存
SaveSystem->AutoSave();

// 自定義名稱的自動保存
SaveSystem->AutoSave(TEXT("AutoSave_Chapter2"));

// 獲取自動保存設置
bool bAutoSaveEnabled = SaveSystem->IsAutoSaveEnabled();
float AutoSaveInterval = SaveSystem->GetAutoSaveInterval();
```

### 4. 存檔管理

```cpp
// 獲取所有存檔列表
TArray<FRTSSaveGameInfo> SaveList = SaveSystem->GetSaveList();

// 按時間排序
SaveList.Sort([](const FRTSSaveGameInfo& A, const FRTSSaveGameInfo& B)
{
    return A.SaveTime > B.SaveTime;
});

// 刪除存檔
SaveSystem->DeleteSave(TEXT("OldSave_001"));

// 重命名存檔
SaveSystem->RenameSave(TEXT("OldName"), TEXT("NewName"));

// 複製存檔
SaveSystem->CopySave(TEXT("Save_001"), TEXT("Save_001_Backup"));
```

---

## 數據類型詳細說明

### GameState (遊戲狀態)

```cpp
FRTSSaveGameData SaveData;
SaveData.SaveType = ERTSSaveDataType::GameState;

// 包含數據：
// - 遊戲時間
// - 當前地圖
// - 遊戲難度
// - 當前章節
// - 全局變量
```

### PlayerData (玩家數據)

```cpp
FRTSSaveGameData SaveData;
SaveData.SaveType = ERTSSaveDataType::PlayerData;

// 包含數據：
// - 玩家位置
// - 玩家屬性（生命值、經驗等）
// - 裝備狀態
// - 技能等級
```

### Relationships (關係數據)

```cpp
FRTSSaveGameData SaveData;
SaveData.SaveType = ERTSSaveDataType::Relationships;

// 包含數據：
// - 所有角色關係
// - 關係歷史記錄
// - 聲望數據
// - 互動記錄
```

### Quests (任務數據)

```cpp
FRTSSaveGameData SaveData;
SaveData.SaveType = ERTSSaveDataType::Quests;

// 包含數據：
// - 進行中任務
// - 已完成任務
// - 任務進度
// - 任務獎勵領取狀態
```

---

## 多格式支持

### 二進制格式（推薦）

```cpp
// 特點：體積小、速度快、不可讀
FRTSSaveGameData SaveData;
SaveData.SaveFormat = ERTSSaveFormat::Binary;

// 優勢：
// - 最小的文件大小
// - 最快的讀寫速度
// - 自動數據壓縮
```

### JSON 格式（調試用）

```cpp
// 特點：人類可讀、易於調試
FRTSSaveGameData SaveData;
SaveData.SaveFormat = ERTSSaveFormat::JSON;

// 優勢：
// - 可手動編輯
// - 便於開發調試
// - 跨平台兼容性
```

### 壓縮格式

```cpp
// 特點：大文件優化
FRTSSaveGameData SaveData;
SaveData.SaveFormat = ERTSSaveFormat::Compressed;

// 自動壓縮算法：
// - GZip 壓縮
// - 可配置壓縮級別
```

### 加密格式

```cpp
// 特點：數據安全
FRTSSaveGameData SaveData;
SaveData.SaveFormat = ERTSSaveFormat::Encrypted;

// 配置加密：
SaveSystem->SetEncryptionKey(TEXT("YourSecretKey"));
SaveSystem->SetEncryptionAlgorithm(EEncryptionAlgorithm::AES256);
```

---

## 雲端保存

### 配置雲端保存

```cpp
// 啟用雲端保存
SaveSystem->SetCloudSaveEnabled(true);

// 配置雲端提供商
SaveSystem->SetCloudProvider(ECloudProvider::Steam);
// 或
SaveSystem->SetCloudProvider(ECloudProvider::Epic);
// 或
SaveSystem->SetCloudProvider(ECloudProvider::Custom);

// 設置雲端同步模式
SaveSystem->SetCloudSyncMode(ECloudSyncMode::Automatic);
// 選項：Automatic, Manual, OnExit, OnDemand
```

### 雲端操作

```cpp
// 上傳存檔到雲端
SaveSystem->UploadToCloud(TEXT("Save_001"));

// 從雲端下載存檔
SaveSystem->DownloadFromCloud(TEXT("Save_001"));

// 獲取雲端存檔列表
TArray<FRTSSaveGameInfo> CloudSaves = SaveSystem->GetCloudSaveList();

// 刪除雲端存檔
SaveSystem->DeleteFromCloud(TEXT("Save_001"));

// 檢查雲端同步狀態
ECloudSyncStatus SyncStatus = SaveSystem->GetCloudSyncStatus(TEXT("Save_001"));
```

---

## 進階功能

### 1. 保存槽位管理

```cpp
// 創建保存槽位
SaveSystem->CreateSaveSlot(TEXT("Slot1"), TEXT("主線存檔"));
SaveSystem->CreateSaveSlot(TEXT("Slot2"), TEXT("支線存檔"));
SaveSystem->CreateSaveSlot(TEXT("Slot3"), TEXT("測試存檔"));

// 獲取槽位信息
FRTSSaveSlotInfo SlotInfo = SaveSystem->GetSaveSlotInfo(TEXT("Slot1"));

// 獲取槽位中的所有存檔
TArray<FRTSSaveGameInfo> SlotSaves = SaveSystem->GetSavesInSlot(TEXT("Slot1"));

// 刪除槽位（及其中所有存檔）
SaveSystem->DeleteSaveSlot(TEXT("Slot3"));
```

### 2. 存檔備份和恢復

```cpp
// 手動創建備份
SaveSystem->CreateBackup(TEXT("Save_001"), TEXT("BeforeBossFight"));

// 設置自動備份
SaveSystem->SetBackupEnabled(true);
SaveSystem->SetBackupInterval(600.0f); // 10分鐘
SaveSystem->SetMaxBackups(5);

// 獲取備份列表
TArray<FRTSSaveGameInfo> Backups = SaveSystem->GetBackupList(TEXT("Save_001"));

// 從備份恢復
SaveSystem->RestoreFromBackup(TEXT("Save_001"), TEXT("BeforeBossFight"));

// 清理舊備份
SaveSystem->CleanupOldBackups(TEXT("Save_001"), 3); // 保留最近3個
```

### 3. 數據驗證和修復

```cpp
// 驗證存檔完整性
bool bValid = SaveSystem->ValidateSave(TEXT("Save_001"));

if (!bValid)
{
    // 嘗試修復
    bool bRepaired = SaveSystem->RepairSave(TEXT("Save_001"));
    
    if (!bRepaired)
    {
        // 從備份恢復
        SaveSystem->RestoreFromBackup(TEXT("Save_001"), TEXT("AutoBackup"));
    }
}

// 掃描所有存檔
TArray<FString> CorruptSaves = SaveSystem->ScanForCorruptSaves();
```

### 4. 導入/導出

```cpp
// 導出存檔為通用格式
SaveSystem->ExportSave(TEXT("Save_001"), TEXT("/Export/Save_001.json"), ERTSSaveFormat::JSON);

// 導入外部存檔
SaveSystem->ImportSave(TEXT("/Import/ExternalSave.json"), TEXT("ImportedSave"));

// 批量導出
TArray<FString> SavesToExport = {TEXT("Save_001"), TEXT("Save_002")};
SaveSystem->BatchExport(SavesToExport, TEXT("/Export/Backup/"));
```

---

## 事件系統

### 綁定保存事件

```cpp
// 綁定保存完成事件
SaveSystem->OnSaveCompleted.AddDynamic(this, &UMyGameInstance::OnSaveCompleted);

// 綁定載入完成事件
SaveSystem->OnLoadCompleted.AddDynamic(this, &UMyGameInstance::OnLoadCompleted);

// 綁定自動保存事件
SaveSystem->OnAutoSaveTriggered.AddDynamic(this, &UMyGameInstance::OnAutoSave);

// 綁定保存失敗事件
SaveSystem->OnSaveFailed.AddDynamic(this, &UMyGameInstance::OnSaveFailed);
```

### 事件處理

```cpp
void UMyGameInstance::OnSaveCompleted(const FString& SaveName, ERTSSaveDataType SaveType)
{
    // 顯示保存成功提示
    ShowNotification(FString::Printf(TEXT("遊戲已保存: %s"), *SaveName));
    
    // 播放保存成功音效
    AudioSystem->PlaySFX(TEXT("Save_Success"));
}

void UMyGameInstance::OnAutoSave(float TimeSinceLastSave)
{
    // 顯示自動保存提示（非侵入式）
    ShowAutoSaveIndicator();
    
    UE_LOG(LogTemp, Log, TEXT("Auto save triggered. Time since last: %.1f seconds"), TimeSinceLastSave);
}

void UMyGameInstance::OnSaveFailed(const FString& SaveName, const FString& ErrorMessage)
{
    // 顯示錯誤提示
    ShowErrorNotification(FString::Printf(TEXT("保存失敗: %s"), *ErrorMessage));
    
    // 嘗試備用保存位置
    SaveSystem->SetDefaultSaveLocation(ERTSSaveLocation::Temp);
    SaveSystem->AutoSave(SaveName + TEXT("_Backup"));
}
```

---

## 性能優化

### 異步保存

```cpp
// 異步保存（不阻塞遊戲）
SaveSystem->SaveGameAsync(SaveData, 
    FOnSaveCompleted::CreateLambda([](const FString& SaveName)
    {
        UE_LOG(LogTemp, Log, TEXT("Async save completed: %s"), *SaveName);
    })
);

// 檢查異步操作狀態
bool bSaving = SaveSystem->IsAsyncSaveInProgress();

// 等待異步保存完成
SaveSystem->WaitForAsyncSave(5.0f); // 最多等待5秒
```

### 增量保存

```cpp
// 只保存變化的數據
FRTSSaveGameData IncrementalSave;
IncrementalSave.SaveType = ERTSSaveDataType::Custom;
IncrementalSave.bIncremental = true;
IncrementalSave.ChangedDataTypes = {
    ERTSSaveDataType::PlayerData,
    ERTSSaveDataType::Quests
};

SaveSystem->SaveGame(IncrementalSave);
```

### 壓縮設置

```cpp
// 配置壓縮
SaveSystem->SetCompressionEnabled(true);
SaveSystem->SetCompressionLevel(ECompressionLevel::Medium);
// 選項：None, Low, Medium, High, Maximum
```

---

## 藍圖節點參考

### 主要節點

| 節點名稱 | 輸入 | 輸出 | 說明 |
|----------|------|------|------|
| `Initialize Save System` | Void | Void | 初始化系統 |
| `Save Game` | Save Name, Save Type, Format | Success | 保存遊戲 |
| `Load Game` | Save Name | Success | 載入遊戲 |
| `Auto Save` | Save Name (可選) | Void | 自動保存 |
| `Delete Save` | Save Name | Success | 刪除存檔 |
| `Has Save Game` | Save Name | Boolean | 檢查存檔 |
| `Get Save List` | Void | Save Info Array | 獲取存檔列表 |
| `Set Auto Save Enabled` | Enabled | Void | 啟用/禁用 |
| `Set Auto Save Interval` | Interval (秒) | Void | 設置間隔 |
| `Validate Save` | Save Name | Valid | 驗證存檔 |
| `Export Save` | Save Name, Path | Success | 導出存檔 |
| `Import Save` | Path, New Name | Success | 導入存檔 |

---

## 測試指南

### 保存系統測試

```cpp
void TestSaveSystem()
{
    // 1. 基本保存測試
    TestBasicSave();
    
    // 2. 載入測試
    TestLoad();
    
    // 3. 自動保存測試
    TestAutoSave();
    
    // 4. 多格式測試
    TestMultipleFormats();
    
    // 5. 數據完整性測試
    TestDataIntegrity();
    
    // 6. 備份恢復測試
    TestBackupAndRestore();
}

void TestBasicSave()
{
    // 創建測試數據
    FRTSSaveGameData TestData;
    TestData.SaveName = TEXT("TestSave");
    TestData.SaveType = ERTSSaveDataType::All;
    
    // 執行保存
    bool bSaved = SaveSystem->SaveGame(TestData);
    check(bSaved);
    
    // 驗證文件存在
    check(SaveSystem->HasSaveGame(TEXT("TestSave")));
}

void TestDataIntegrity()
{
    // 保存前記錄數據
    int32 HealthBefore = PlayerCharacter->GetHealth();
    FVector LocationBefore = PlayerCharacter->GetActorLocation();
    
    // 保存
    SaveSystem->SaveGame(SaveData);
    
    // 修改數據
    PlayerCharacter->SetHealth(0);
    PlayerCharacter->SetActorLocation(FVector::ZeroVector);
    
    // 載入
    SaveSystem->LoadGame(TEXT("TestSave"));
    
    // 驗證數據恢復
    check(PlayerCharacter->GetHealth() == HealthBefore);
    check(PlayerCharacter->GetActorLocation().Equals(LocationBefore));
}
```

### 性能測試

```cpp
void TestSavePerformance()
{
    // 測試保存速度
    double StartTime = FPlatformTime::Seconds();
    
    SaveSystem->SaveGame(LargeSaveData);
    
    double SaveTime = FPlatformTime::Seconds() - StartTime;
    UE_LOG(LogTemp, Log, TEXT("Save time: %.3f seconds"), SaveTime);
    
    // 測試載入速度
    StartTime = FPlatformTime::Seconds();
    
    SaveSystem->LoadGame(TEXT("LargeSave"));
    
    double LoadTime = FPlatformTime::Seconds() - StartTime;
    UE_LOG(LogTemp, Log, TEXT("Load time: %.3f seconds"), LoadTime);
    
    // 驗證性能目標
    check(SaveTime < 1.0f); // 保存 < 1秒
    check(LoadTime < 2.0f); // 載入 < 2秒
}
```

---

## 常見問題排查

| 問題 | 可能原因 | 解決方案 |
|------|----------|----------|
| 保存失敗 | 磁盤空間不足 | 檢查磁盤空間，清理舊存檔 |
| 載入失敗 | 存檔損壞 | 使用 RepairSave 或 RestoreFromBackup |
| 自動保存不工作 | 未啟用或間隔過長 | 檢查 SetAutoSaveEnabled 和間隔設置 |
| 存檔列表為空 | 路徑錯誤 | 驗證 SaveLocation 設置 |
| 雲端同步失敗 | 網絡或認證問題 | 檢查網絡和雲端配置 |
| 保存文件過大 | 數據包含過多內容 | 使用增量保存或壓縮 |

---

## 示例場景

### 示例 1: 關卡切換保存

```cpp
void AMyGameMode::OnLevelTransition(const FString& TargetLevel)
{
    // 創建檢查點保存
    FRTSSaveGameData Checkpoint;
    Checkpoint.SaveName = FString::Printf(TEXT("Checkpoint_%s"), *TargetLevel);
    Checkpoint.SaveType = ERTSSaveDataType::All;
    Checkpoint.Description = FString::Printf(TEXT("Before entering %s"), *TargetLevel);
    Checkpoint.bIsCheckpoint = true;
    
    // 保存
    SaveSystem->SaveGame(Checkpoint);
    
    // 限制檢查點數量
    TArray<FRTSSaveGameInfo> Checkpoints = SaveSystem->GetSavesByType(ERTSSaveDataType::All);
    Checkpoints.RemoveAll([](const FRTSSaveGameInfo& Info) {
        return !Info.bIsCheckpoint;
    });
    
    // 刪除舊檢查點（保留最近5個）
    if (Checkpoints.Num() > 5)
    {
        for (int32 i = 5; i < Checkpoints.Num(); ++i)
        {
            SaveSystem->DeleteSave(Checkpoints[i].SaveName);
        }
    }
    
    // 切換關卡
    UGameplayStatics::OpenLevel(GetWorld(), FName(*TargetLevel));
}
```

### 示例 2: 關係數據持久化

```cpp
void AMyGameMode::SaveRelationshipData()
{
    // 只保存關係數據（快速）
    FRTSSaveGameData RelSave;
    RelSave.SaveName = TEXT("RelationshipData");
    RelSave.SaveType = ERTSSaveDataType::Relationships;
    RelSave.SaveFormat = ERTSSaveFormat::Binary;
    
    // 保存
    SaveSystem->SaveGame(RelSave);
}

void AMyGameMode::LoadRelationshipData()
{
    if (SaveSystem->HasSaveGame(TEXT("RelationshipData")))
    {
        SaveSystem->LoadGame(TEXT("RelationshipData"));
        
        // 刷新 UI
        UIManager->RefreshRelationshipPanel();
    }
}
```

### 示例 3: 多存檔槽位 UI

```cpp
void UMySaveMenuWidget::RefreshSaveSlots()
{
    // 清空現有列表
    SaveSlotContainer->ClearChildren();
    
    // 獲取所有槽位
    TArray<FRTSSaveSlotInfo> Slots = SaveSystem->GetAllSaveSlots();
    
    for (const FRTSSaveSlotInfo& Slot : Slots)
    {
        // 創建槽位 UI
        USaveSlotWidget* SlotWidget = CreateWidget<USaveSlotWidget>(this, SaveSlotWidgetClass);
        
        // 設置槽位信息
        SlotWidget->SetSlotName(Slot.SlotName);
        SlotWidget->SetSlotDescription(Slot.Description);
        
        // 獲取最新存檔
        TArray<FRTSSaveGameInfo> SlotSaves = SaveSystem->GetSavesInSlot(Slot.SlotName);
        if (SlotSaves.Num() > 0)
        {
            SlotWidget->SetLastSaveTime(SlotSaves[0].SaveTime);
            SlotWidget->SetSaveCount(SlotSaves.Num());
            
            if (SlotSaves[0].Screenshot)
            {
                SlotWidget->SetScreenshot(SlotSaves[0].Screenshot);
            }
        }
        
        // 添加到容器
        SaveSlotContainer->AddChild(SlotWidget);
    }
}
```

---

## 文檔版本

- **版本**: 1.0
- **更新日期**: 2026-03-22
- **適用版本**: MingGoRTS Beta
- **關聯系統**: Story 3.5 (音頻), Story 3.6 (UI)

---

*本文檔是 Beta 版本集成指南的一部分。詳見 `docs/BETA_INTEGRATION_GUIDE.md`。*
