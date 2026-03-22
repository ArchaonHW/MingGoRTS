# Story 3.6: UI 系統完善指南

## 概述

MingGoRTS UI 系統提供完整的個人層界面管理，包括關係顯示、聲望面板、對話系統和任務界面。

---

## 核心組件架構

```
MingPersonalUIManager (核心管理器)
├── MingMainDashboard (主儀表板)
├── MingRelationshipPanel (關係界面)
├── MingReputationPanel (聲望面板)
├── MingDialoguePanel (對話系統)
├── MingQuestPanel (任務界面)
└── MingAudioPanel (音頻控制)
```

---

## 快速集成

### C++ 集成

```cpp
#include "MingPersonalUIManager.h"
#include "MingRelationshipManager.h"
#include "MingAudioRelationshipManager.h"

// 在 GameMode 中初始化
void AMyGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    // 創建 UI 管理器
    UIManager = NewObject<UMingPersonalUIManager>(this);
    
    // 初始化（需要關係管理器和音頻關係管理器）
    UIManager->InitializePersonalUIManager(
        RelationshipManager, 
        AudioRelationshipManager
    );
    
    // 設置默認 UI 配置
    UIManager->SetDefaultUILayout(EPersonalUILayout::Desktop);
    UIManager->EnableUIAnimations(true);
}
```

### 藍圖集成

```blueprint
BeginPlay
├── Create Widget (MingMainDashboard)
├── Add to Viewport
├── Initialize Personal UI Manager
│   ├── Relationship Manager
│   └── Audio Relationship Manager
└── Set UI Layout (Desktop)
```

---

## 界面詳細說明

### 1. 主儀表板 (MingMainDashboard)

**功能**: 個人系統的中央控制面板

```cpp
// 打開主儀表板
UIManager->OpenMainDashboard();

// 添加自定義小部件
UIManager->AddDashboardWidget(TEXT("CustomWidget"), CustomWidget);

// 刷新儀表板數據
UIManager->RefreshDashboard();
```

**藍圖節點**:
- `Open Main Dashboard`
- `Close Main Dashboard`
- `Refresh Dashboard`
- `Set Dashboard Tab` (Overview, Relationships, Reputation, Quests, Settings)

### 2. 關係面板 (MingRelationshipPanel)

**功能**: 顯示和管理角色關係

```cpp
// 打開關係面板
UIManager->OpenRelationshipPanel();

// 顯示特定角色關係
UIManager->ShowCharacterRelationship(TEXT("Character_001"));

// 刷新關係列表
UIManager->RefreshRelationshipList();
```

**顯示內容**:
- 關係網絡圖
- 關係列表（按類型篩選）
- 關係詳情（點擊查看）
- 關係操作（改善/惡化）

**藍圖節點**:
```blueprint
Open Relationship Panel
├── Filter by Type (Friend, Enemy, Neutral, Family, Business, Political)
├── Sort by (Name, Relationship Value, Recent Interaction)
└── Search (Text Input)
```

### 3. 聲望面板 (MingReputationPanel)

**功能**: 顯示聲望狀態和地區聲望

```cpp
// 顯示聲望面板
UIManager->ShowReputationPanel();

// 切換到特定地區
UIManager->ShowRegionReputation(EMingRegion::Shanghai);

// 顯示聲望詳情
UIManager->ShowReputationDetails(TEXT("Overall"));
```

**顯示內容**:
- 總體聲望評級
- 各地區聲望分布
- 聲望變化歷史
- 聲望影響預覽

### 4. 對話面板 (MingDialoguePanel)

**功能**: 分支對話系統

```cpp
// 啟動對話
UIManager->StartDialogue(TEXT("NPC_001"));

// 顯示對話選項
UIManager->ShowDialogueOptions(Options);

// 設置打字機效果速度
UIManager->SetTypewriterSpeed(50.0f); // 字符/秒

// 結束對話
UIManager->EndDialogue();
```

**對話系統特性**:
- 打字機文字效果
- 分支選項系統
- 語音同步播放
- 角色頭像顯示
- 情感動畫反應

**藍圖使用**:
```blueprint
Start Dialogue
├── Character ID: "NPC_001"
├── Dialogue ID: "QuestStart_01"
└── Context: Quest

On Dialogue Option Selected
├── Option Index: 0
└── Branch to: "QuestAccept"
```

### 5. 任務面板 (MingQuestPanel)

**功能**: 任務管理和追蹤

```cpp
// 打開任務面板
UIManager->OpenQuestPanel();

// 添加任務追蹤
UIManager->TrackQuest(TEXT("Quest_001"));

// 更新任務進度
UIManager->UpdateQuestProgress(TEXT("Quest_001"), 50.0f);

// 完成任務動畫
UIManager->PlayQuestCompleteAnimation(TEXT("Quest_001"));
```

**任務面板功能**:
- 任務列表（過濾、排序、搜索）
- 任務詳情（目標、獎勵、期限）
- 任務追蹤 HUD
- 完成動畫效果

### 6. 音頻面板 (MingAudioPanel)

**功能**: 音頻設置和控制

```cpp
// 打開音頻面板
UIManager->OpenAudioPanel();

// 設置音量
UIManager->SetVolume(EAudioCategory::Music, 0.8f);
UIManager->SetVolume(EAudioCategory::SFX, 1.0f);
UIManager->SetVolume(EAudioCategory::Voice, 0.9f);

// 切換主題
UIManager->SetAudioTheme(EMingMusicTheme::RepublicEra_Diplomatic);
```

---

## UI 配置和自定義

### 佈局配置

```cpp
// 設置 UI 佈局
UIManager->SetUILayout(EPersonalUILayout::Desktop);

// 可選佈局類型
enum class EPersonalUILayout : uint8
{
    Desktop,      // 桌面標準佈局
    Mobile,       // 移動端豎屏
    Tablet,       // 平板橫屏
    Console,      // 主機手柄優化
    VR,           // VR 沉浸模式
    Compact       // 緊湊簡約模式
};
```

### 主題配置

```cpp
// 設置 UI 主題
UIManager->SetUITheme(EUITheme::RepublicEra);

// 自定義顏色
FLinearColor PrimaryColor(0.2f, 0.4f, 0.6f, 1.0f);
FLinearColor SecondaryColor(0.8f, 0.6f, 0.2f, 1.0f);
UIManager->SetCustomColors(PrimaryColor, SecondaryColor);
```

### 動畫設置

```cpp
// 啟用/禁用動畫
UIManager->EnableUIAnimations(true);

// 設置動畫速度
UIManager->SetAnimationSpeed(1.0f); // 正常速度
UIManager->SetAnimationSpeed(0.5f); // 慢速
UIManager->SetAnimationSpeed(2.0f); // 快速
```

---

## 事件系統

### UI 事件綁定

```cpp
// 綁定面板打開事件
UIManager->OnPanelOpened.AddDynamic(this, &AMyGameMode::OnPanelOpened);

// 綁定對話選擇事件
UIManager->OnDialogueOptionSelected.AddDynamic(this, &AMyGameMode::OnDialogueSelected);

// 綁定任務追蹤事件
UIManager->OnQuestTracked.AddDynamic(this, &AMyGameMode::OnQuestTracked);
```

### 事件處理

```cpp
void AMyGameMode::OnPanelOpened(EPersonalUIType PanelType)
{
    // 暫停遊戲（如果是模態面板）
    if (PanelType == EPersonalUIType::DialoguePanel)
    {
        SetGamePaused(true);
    }
    
    // 播放面板打開音效
    AudioSystem->PlaySFX(TEXT("UI_Open"));
}

void AMyGameMode::OnDialogueSelected(int32 OptionIndex, const FString& NextDialogueID)
{
    // 處理對話分支
    ProcessDialogueBranch(NextDialogueID);
    
    // 可能影響關係
    if (OptionIndex == 0) // 友好選項
    {
        RelationshipManager->ImproveRelationship(TEXT("NPC_001"), 5.0f);
    }
}
```

---

## 輔助功能支持

### 智能輔助

```cpp
// 啟用輔助功能
UIManager->EnableAccessibility(true);

// 設置字體大小
UIManager->SetFontSize(EFontSize::Large);

// 高對比度模式
UIManager->EnableHighContrast(true);

// 色盲模式
UIManager->SetColorBlindMode(EColorBlindMode::Deuteranopia);
```

### 輔助功能選項

| 選項 | 說明 |
|------|------|
| 字體大小 | 小 / 中 / 大 / 特大 |
| 高對比度 | 增強文字和背景對比 |
| 色盲模式 | 支持常見色盲類型 |
| 減少動畫 | 禁用非必要動畫 |
| 屏幕閱讀器 | 支持文本朗讀 |

---

## 性能優化

### 加載優化

```cpp
// 延遲加載面板
UIManager->SetLazyLoading(true);

// 預加載常用面板
UIManager->PreloadPanel(EPersonalUIType::MainDashboard);
UIManager->PreloadPanel(EPersonalUIType::RelationshipPanel);

// 設置緩存大小
UIManager->SetPanelCacheSize(5); // 最多緩存5個面板
```

### 渲染優化

```cpp
// 使用虛擬滾動（長列表）
UIManager->EnableVirtualScrolling(true);

// 限制同時顯示的 UI 元素數量
UIManager->SetMaxVisibleElements(100);

// 啟用 UI 剔除
UIManager->EnableUICulling(true);
```

---

## 測試指南

### UI 測試清單

```cpp
void TestUISystem()
{
    // 1. 面板打開測試
    TestPanelOpening();
    
    // 2. 對話系統測試
    TestDialogueSystem();
    
    // 3. 任務系統測試
    TestQuestSystem();
    
    // 4. 輔助功能測試
    TestAccessibility();
    
    // 5. 性能測試
    TestUIPerformance();
}

void TestPanelOpening()
{
    // 測試所有面板能否正常打開和關閉
    for (int32 i = 0; i < static_cast<int32>(EPersonalUIType::MAX); ++i)
    {
        EPersonalUIType Type = static_cast<EPersonalUIType>(i);
        UIManager->OpenPanel(Type);
        check(UIManager->IsPanelOpen(Type));
        
        UIManager->ClosePanel(Type);
        check(!UIManager->IsPanelOpen(Type));
    }
}
```

### 常見問題排查

| 問題 | 可能原因 | 解決方案 |
|------|----------|----------|
| 面板無法打開 | 管理器未初始化 | 檢查 InitializePersonalUIManager 調用 |
| 對話無法顯示 | 對話數據缺失 | 驗證對話配置表 |
| 字體顯示異常 | 字體資產缺失 | 檢查字體路徑配置 |
| 動畫卡頓 | 幀率過低 | 啟用減少動畫模式 |

---

## 藍圖節點參考

### 主要節點

| 節點名稱 | 輸入 | 輸出 | 說明 |
|----------|------|------|------|
| `Initialize Personal UI Manager` | Relationship Manager, Audio Manager | Void | 初始化 |
| `Open Panel` | Panel Type | Void | 打開面板 |
| `Close Panel` | Panel Type | Void | 關閉面板 |
| `Start Dialogue` | Character ID, Dialogue ID | Void | 開始對話 |
| `Show Dialogue Options` | Options Array | Selected Index | 顯示選項 |
| `Track Quest` | Quest ID | Void | 追蹤任務 |
| `Set UI Theme` | Theme | Void | 設置主題 |
| `Enable Accessibility` | Enabled | Void | 輔助功能 |

---

## 示例場景

### 示例 1: 完整的對話流程

```cpp
void StartQuestDialogue(const FString& NPCID)
{
    // 1. 停止遊戲時間
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    
    // 2. 打開對話面板
    UIManager->StartDialogue(NPCID);
    
    // 3. 設置對話內容
    FDialogueContext Context;
    Context.DialogueText = TEXT("你愿意帮我找回失落的宝藏吗？");
    Context.CharacterName = TEXT("老李");
    Context.bShowPortrait = true;
    Context.PortraitPath = TEXT("/Game/Portraits/NPC_LaoLi");
    
    // 4. 設置選項
    TArray<FDialogueOption> Options;
    Options.Add({TEXT("当然愿意！"), TEXT("QuestAccept"), TEXT("Friendly")});
    Options.Add({TEXT("我需要考虑一下。"), TEXT("QuestDelay"), TEXT("Neutral")});
    Options.Add({TEXT("我没兴趣。"), TEXT("QuestReject"), TEXT("Hostile")});
    
    UIManager->ShowDialogueOptions(Options);
    
    // 5. 綁定選擇事件
    UIManager->OnDialogueOptionSelected.AddDynamic(this, &AMyGameMode::HandleQuestResponse);
}

void HandleQuestResponse(int32 SelectedIndex, const FString& BranchID)
{
    if (BranchID == TEXT("QuestAccept"))
    {
        // 接受任務
        QuestManager->AcceptQuest(TEXT("Quest_LostTreasure"));
        
        // 改善關係
        RelationshipManager->ImproveRelationship(TEXT("NPC_LaoLi"), 10.0f);
        
        // 播放正面音效
        AudioSystem->PlaySFX(TEXT("Quest_Accept"));
    }
    else if (BranchID == TEXT("QuestReject"))
    {
        // 惡化關係
        RelationshipManager->DamageRelationship(TEXT("NPC_LaoLi"), 5.0f);
        
        // 播放負面音效
        AudioSystem->PlaySFX(TEXT("Quest_Reject"));
    }
    
    // 恢復遊戲
    UGameplayStatics::SetGamePaused(GetWorld(), false);
}
```

### 示例 2: 聲望界面集成

```cpp
void ShowReputationOverview()
{
    // 1. 獲取聲望數據
    FReputationData OverallRep = ReputationManager->GetOverallReputation();
    TMap<EMingRegion, float> RegionReps = ReputationManager->GetAllRegionReputations();
    
    // 2. 打開聲望面板
    UIManager->ShowReputationPanel();
    
    // 3. 設置數據
    UIManager->SetReputationData(OverallRep, RegionReps);
    
    // 4. 高亮變化
    if (OverallRep.RecentChange != 0.0f)
    {
        UIManager->HighlightReputationChange(OverallRep.RecentChange);
    }
}
```

---

## 文檔版本

- **版本**: 1.0
- **更新日期**: 2026-03-22
- **適用版本**: MingGoRTS Beta
- **關聯系統**: Story 3.5 (音頻), Story 3.7 (保存載入)

---

*本文檔是 Beta 版本集成指南的一部分。詳見 `docs/BETA_INTEGRATION_GUIDE.md`。*
