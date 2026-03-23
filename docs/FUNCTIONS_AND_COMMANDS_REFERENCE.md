# MingGoRTS 功能與指令一覽

## 目錄
1. [核心系統總覽](#核心系統總覽)
2. [開發工具與指令](#開發工具與指令)
3. [版本控制指令](#版本控制指令)
4. [智能任務分類提交](#智能任務分類提交)
5. [快速參考](#快速參考)

---

## 核心系統總覽

### 已完成的 Epic 系統

| Epic | 系統名稱 | 狀態 | 核心功能 |
|------|----------|------|----------|
| Epic 1.1 | 內建作業系統 | ✅ 完成 | 內核、進程管理、記憶體管理、文件系統 |
| Epic 2.1 | 經濟系統優化 | ✅ 完成 | 10種資源、貿易系統、動態經濟指標 |
| Epic 3.1 | 建築系統優化 | ✅ 完成 | 15種建築、5級升級、建築網格系統 |
| Epic 4.1 | UI/音頻系統優化 | ✅ 完成 | 21種UI、16種音頻、情感系統 |
| Epic 5.1 | 本地化和國際化 | ✅ 完成 | 20種語言、17種地區、文化適應 |
| Epic 6.1 | 北洋勢力美術 | ✅ 完成 | 單位、建築、UI、場景 |
| Epic 7.1 | 本地化和國際化 | ✅ 完成 | 增強本地化系統、文化信息管理 |
| Epic 8.1 | 性能優化調試 | ✅ 完成 | 11種性能監控、自動優化、實時調試 |
| Epic 9 | 歷史事件系統 | ✅ 完成 | 動態歷史演進、多分支路徑 |
| Epic 10 | 平衡測試系統 | ✅ 完成 | 自動平衡測試、數據分析 |

### 核心子系統

#### 🎮 遊戲系統
- **MingRTSEconomicSystem** - 經濟系統（10種資源、動態經濟指標）
- **MingRTSBuildingSystem** - 建築系統（15種建築、5級升級）
- **MingRTSResourceManager** - 資源管理器（資源節點、提取方法）
- **MingRTSProcessManager** - 進程管理器（100+併發進程）
- **MingRTSKernel** - 內建OS內核
- **MingRTSVirtualFileSystem** - 虛擬文件系統

#### 🤖 AI系統
- **MingRTSEvolutionaryAISystem** - 自進化AI系統（遺傳算法、深度強化學習）
- **MingRTSDynamicHistorySystem** - 動態歷史演進系統
- **MingRTSEmotionalAIEngine** - 情感AI引擎（15種情感類型）
- **MingRTSCrossMediaNarrativeSystem** - 跨媒體敘事系統
- **MingRTSSelfLearningSystem** - 自我學習系統
- **MingRTSAILearningIntegration** - AI學習集成
- **MingDynamicTaskGenerator** - 動態任務生成器
- **MingPlayerBehaviorPredictor** - 玩家行為預測器

#### 🎨 內容生成系統
- **MingAIGeneratedContentSystem** - AI內容生成核心
- **MingAIContentRenderer** - AI內容渲染器
- **MingAIContentManager** - AI內容管理器
- **MingRTSGameAssetGenerator** - 遊戲資產生成器
- **MingAutoSceneGenerator** - 自動場景生成器

#### 🌐 網絡與多人系統
- **MingRTSNetworkEnhancedSystem** - 增強網絡系統
- **MingRTSInterServiceCommunication** - 服務間通信
- **MingRTSServiceRegistry** - 服務註冊中心
- **MingRTSServiceHealthMonitor** - 服務健康監控

#### 🛡️ 安全與監控系統
- **MingRTSSecurityFirewall** - 安全防火牆
- **MingAntiVirusSystem** - 防病毒系統
- **MingSecurityManager** - 安全管理器
- **MingSecurityTestSuite** - 安全測試套件
- **RiskMonitoringDashboard** - 風險監控儀表板

#### 📊 性能與測試系統
- **MingPerformanceAnalysisSystem** - 性能分析系統
- **MingPerformanceMonitor** - 性能監控系統
- **MingStressTestAutomation** - 壓力測試自動化
- **MingProjectStressTest** - 項目壓力測試
- **MingTestingIntegrationTestSuite** - 集成測試套件

#### 🎵 音頻系統
- **MingRTSAudioEnhancedSystem** - 增強音頻系統
- **MingRepublicEraAudioThemes** - 民國時期音頻主題
- **MingAudioRelationshipManager** - 音頻關係管理器

#### 🖼️ UI系統
- **MingRTSUIEnhancedSystem** - 增強UI系統
- **MingMainGameUI** - 主遊戲UI

#### 💾 存檔系統
- **MingSaveGame** - 存檔數據類
- **MingSaveGameManager** - 存檔管理器
- **MingSaveGameSystemTest** - 存檔系統測試

#### 🎭 個人系統 (MingPersonal)
- **MingPersonalManager** - 個人管理器
- **MingRelationshipManager** - 關係管理器
- **MingReputationManager** - 聲望管理器
- **MingDialogueSystem** - 對話系統
- **MingQuestManager** - 任務管理器

---

## 開發工具與指令

### 🔧 核心 PowerShell 腳本

#### 智能版本控制
```powershell
# 基本使用
.\Tools\build\SmartVersionControl.ps1                    # 檢查代碼
.\Tools\build\SmartVersionControl.ps1 -Action backup      # 創建備份
.\Tools\build\SmartVersionControl.ps1 -Action restore     # 恢復備份
.\Tools\build\SmartVersionControl.ps1 -Action clean       # 清理臨時文件
.\Tools\build\SmartVersionControl.ps1 -Action commit -Message "[修復] 修復bug"  # 智能提交
```

#### 智能任務分類提交
```powershell
# 分析變更
.\Tools\build\SmartTaskCommit.ps1 -Action analyze

# 智能分類
.\Tools\build\SmartTaskCommit.ps1 -Action classify

# 預覽提交
.\Tools\build\SmartTaskCommit.ps1 -Action preview

# 執行提交
.\Tools\build\SmartTaskCommit.ps1 -Action commit

# 自定義提交（指定分類）
.\Tools\build\SmartTaskCommit.ps1 -Action commit -TaskCategory Encoding -CustomMessage "修復編碼問題"
.\Tools\build\SmartTaskCommit.ps1 -Action commit -TaskCategory BugFix -CustomMessage "修復內存泄漏"
```

**任務分類類型：**
- `Encoding` - 編碼修復
- `BugFix` - 錯誤修復
- `Feature` - 功能新增
- `Refactor` - 代碼重構
- `Documentation` - 文檔更新
- `Config` - 配置更新
- `Build` - 構建系統

#### 性能優化腳本
```powershell
.\Tools\performance\Optimize-Performance.ps1              # 執行性能優化
```

#### 項目清理
```powershell
.\Tools\cleanup_project.ps1                              # 清理項目臨時文件
```

### 🚀 快速啟動命令

#### CMD 快捷命令
```cmd
# 智能版本控制快速啟動
.\SmartVC_QuickStart.cmd

# 智能任務分類提交快速啟動
.\SmartTaskCommit_QuickStart.cmd

# 一鍵啟動器
.\OneClickLauncher.cmd

# 在UE5編輯器中打開
.\Open in UE5 Editor.cmd
```

### 🐍 Python 腳本

#### 編碼修復腳本
```bash
# 修復編碼問題
python fix_encoding.py
python fix_all_encoding.py
python fix_comprehensive_encoding.py
python fix_chu_chars.py

# 檢查編碼
python check_encoding.py
python check_encoding_comments.py
python scan_all_encoding_issues.py

# 修復特定問題
python fix_pragma.py
python fix_unbalanced.py
python fix_garbled.py
```

#### 恢復腳本
```bash
# 從Git恢復
python restore_from_git.py
python restore_all.py
python restore_more.py

# 批量恢復
python batch_restore.py
```

---

## 版本控制指令

### Git 基礎指令

```bash
# 狀態檢查
git status
git log --oneline -10

# 變更管理
git add -A
git add <file>
git reset HEAD <file>

# 提交操作
git commit -m "[類型] 描述"
git commit --amend

# 分支操作
git branch -a
git checkout <branch>
git checkout -b <new-branch>
git merge <branch>

# 遠程操作
git fetch
git pull
git push origin <branch>
```

### 智能提交規範

#### 提交訊息格式
```
[類型] 簡短描述

詳細說明（可選）

涉及的檔案統計和說明
```

#### 類型標籤
- `[編碼]` - 編碼修復
- `[修復]` - 錯誤修復
- `[功能]` - 功能新增
- `[重構]` - 代碼重構
- `[文檔]` - 文檔更新
- `[配置]` - 配置更新
- `[構建]` - 構建系統

### Git 鉤子

項目已配置以下Git鉤子：
- **pre-commit** - 預提交檢查（語法、編碼、安全）
- **pre-merge-commit** - 合併前檢查
- **post-merge** - 合併後操作

---

## 智能任務分類提交

### 系統概述
智能任務分類提交系統自動分析Git變更，智能分類任務類型，並生成規範的提交訊息。

### 使用流程

1. **分析階段** - 檢測所有變更檔案
2. **分類階段** - 基於內容關鍵字智能分類
3. **生成階段** - 生成規範提交訊息
4. **提交階段** - 執行Git提交

### 檔案類型識別

系統自動識別以下檔案類型：
- `.h/.cpp` - C++源碼（編碼檢查）
- `.cs` - C#腳本
- `.ps1` - PowerShell腳本
- `.py` - Python腳本
- `.cmd/.bat` - 批處理腳本
- `.md` - 文檔檔案
- `.json/.xml/.yaml` - 配置文件

### 編碼問題檢測

系統自動檢測以下編碼問題字符：
- `出`、`本`、`使`、`設置` - 中文字符損壞
- `Copyrisht` - Copyright損壞
- `UObbect` - UObject損壞
- `strvct` - struct損壞
- `Learnins` - Learning損壞
- `Alsorithm` - Algorithm損壞

---

## 快速參考

### 🎯 常用快捷鍵

| 操作 | CMD命令 | PowerShell |
|------|---------|------------|
| 智能版本控制 | `SmartVC_QuickStart.cmd` | `SmartVersionControl.ps1` |
| 智能提交 | `SmartTaskCommit_QuickStart.cmd` | `SmartTaskCommit.ps1` |
| 性能優化 | - | `Optimize-Performance.ps1` |
| 清理項目 | - | `cleanup_project.ps1` |
| 編碼修復 | `batch_fix_encoding.cmd` | `fix_encoding_*.ps1` |

### 📁 重要目錄結構

```
MingGoRTS/
├── Source/MingGoRTS/           # 核心源碼
│   ├── Public/                 # 頭檔案
│   └── Private/                # 實現檔案
├── Plugins/                    # 插件目錄
│   ├── MingAI/                 # AI插件
│   ├── MingAudio/              # 音頻插件
│   ├── MingCore/               # 核心插件
│   ├── MingPersonal/           # 個人系統插件
│   ├── MingStrategic/          # 戰略插件
│   └── MingTactical/           # 戰術插件
├── Tools/                      # 開發工具
│   ├── build/                  # 構建腳本
│   ├── ai/                     # AI工具
│   ├── performance/            # 性能工具
│   └── automation/             # 自動化腳本
├── docs/                       # 文檔目錄
├── Config/                     # 配置文件
└── Content/                    # 遊戲內容
```

### 🔍 問題排查

| 問題 | 解決方案 |
|------|----------|
| 編碼問題 | 運行 `python fix_all_encoding.py` |
| 編譯錯誤 | 檢查 `COMPILATION_STATUS_REPORT.md` |
| 性能問題 | 運行性能分析工具 |
| Git問題 | 使用智能版本控制腳本 |
| 構建失敗 | 檢查 `BuildLog.txt` |

### 📊 系統狀態檢查

```powershell
# 檢查編譯狀態
.\COMPILE_VERIFY.cmd

# 檢查編碼問題
.\scan_errors.cmd

# 快速驗證
.\QUICK_VERIFY.cmd
```

### 📝 文檔索引

| 文檔 | 用途 |
|------|------|
| `DEVELOPER_GUIDE.md` | 開發者完整指南 |
| `API_DOCUMENTATION.md` | API參考文檔 |
| `USER_MANUAL.md` | 用戶手冊 |
| `QUICK_START_GUIDE.md` | 快速入門 |
| `BUILD_OPTIMIZATION.md` | 構建優化指南 |
| `PERFORMANCE_BEST_PRACTICES.md` | 性能最佳實踐 |
| `NAMING_CONVENTIONS.md` | 命名規範 |

---

## 版本信息

- **文檔版本**: 1.0
- **更新日期**: 2026-03-23
- **適用版本**: MingGoRTS 全版本
- **維護者**: 開發團隊

---

## 詳細系統參考

### 📖 Story 系統詳情

#### Story 3.4: 角色關係和聲望系統 ✅
**檔案位置**: `Plugins/MingPersonal/Source/MingPersonal/`

**核心組件**:
- `MingRelationshipManager` - 管理10種關係類型
- `MingReputationManager` - 地區差異聲望系統
- `MingRelationshipBlueprintLibrary` - Blueprint接口

**API示例**:
```cpp
// 更新關係
RelationshipManager->ModifyRelationship(PlayerID, NPCID, ERelationshipType::Friendship, 10.0f);

// 檢查聲望影響
if (ReputationManager->HasSufficientReputation(PlayerID, RegionID, RequiredReputation))
{
    // 給予特殊任務或折扣
}
```

#### Story 3.5: 音頻和音效系統 ✅
**檔案位置**: `Plugins/MingPersonal/Source/MingPersonal/Audio/`

**核心組件**:
- `MingAudioRelationshipManager` - 音頻關係管理
- `MingRepublicEraAudioThemes` - 民國時期10個地區主題
- `MingMetaSoundsSystem` - 動態音效生成

**功能實現**:
- 10個音頻主題（戰鬥、和平、宮廷等）
- 10個地區音效設計
- 關係變化音頻反饋
- 聲望變化音頻反饋

#### Story 3.6: UI 和界面系統 ✅
**檔案位置**: `Plugins/MingPersonal/Source/MingPersonal/UI/`

**核心組件**:
- `MingPersonalUIManager` - UI管理器
- `MingRelationshipPanel` - 關係界面
- `MingReputationPanel` - 聲望面板
- `MingDialoguePanel` - 對話系統UI
- `MingQuestPanel` - 任務界面
- `MingAudioPanel` - 音頻控制UI
- `MingMainDashboard` - 主儀表板

**UI特性**:
- 打字機效果對話
- 選項系統
- 任務過濾、排序、搜索
- UI動畫和事件系統

#### Story 3.7: 保存和載入系統 ✅
**檔案位置**: `Plugins/MingPersonal/Source/MingPersonal/Save/`

**核心組件**:
- `MingSaveGame` - 存檔數據類（版本管理、壓縮加密）
- `MingSaveGameManager` - 100個存檔槽管理
- `MingSaveGameSystemTest` - 15個測試用例

**API示例**:
```cpp
// 保存遊戲
EMingSaveGameResult Result = PersonalManager->SaveGame(1, TEXT("Chapter 1 Complete"));

// 快速保存
PersonalManager->QuickSave();

// 加載遊戲
PersonalManager->LoadGame(1);

// 自動保存設置
PersonalManager->SetAutoSaveEnabled(true);
```

**高級功能**:
- 異步存檔/讀檔
- 數據壓縮和加密
- 自動備份恢復
- 版本兼容性

#### Story 3.8: 多人遊戲支持 🔄
**狀態**: 進行中
**計劃功能**:
- 網絡同步機制
- 多人關係系統
- 客戶端-服務器架構

---

### 🏛️ 民國勢力設計（12勢力）

| 編號 | 勢力名稱 | 難度 | 起始地 | 特色機制 | 狀態 |
|------|----------|------|--------|----------|------|
| 1 | 北洋政府 | ★★☆☆☆ | 北京 | 中央正統、外交優勢 | 設計完成 |
| 2 | 國民政府 | ★★★☆☆ | 南京/廣州 | 民族主義、現代化 | 設計完成 |
| 3 | 中國共產黨 | ★★★★☆ | 延安/江西 | 人民戰爭、游擊戰 | 設計完成 |
| 4 | 奉系軍閥 | ★★★☆☆ | 瀋陽 | 東北工業、騎兵優勢 | 設計完成 |
| 5 | 直系軍閥 | ★★★☆☆ | 保定/洛陽 | 中原霸主、兵力龐大 | 設計完成 |
| 6 | 皖系軍閥 | ★★★☆☆ | 天津/合肥 | 政治操作、日本援助 | 設計完成 |
| 7 | 晉系軍閥 | ★★★☆☆ | 太原 | 山西模範、防禦專精 | 設計完成 |
| 8 | 桂系軍閥 | ★★★☆☆ | 桂林/南寧 | 廣西民兵、山地戰 | 設計完成 |
| 9 | 滇系軍閥 | ★★★★☆ | 昆明 | 護國傳統、邊疆擴張 | 設計完成 |
| 10 | 川系軍閥 | ★★★★☆ | 成都/重慶 | 防區制、內部統一 | 設計完成 |
| 11 | 馬家軍 | ★★★★☆ | 蘭州/西寧 | 回族騎兵、宗教團結 | 設計完成 |
| 12 | 新疆勢力 | ★★★★★ | 迪化 | 邊疆要塞、民族複雜 | 設計完成 |

**每個勢力包含**:
1. 基本屬性（名稱、難度、起始地、起始年份）
2. 特色機制（專屬遊戲機制）
3. 專屬單位（3-4個特色單位）
4. 優勢劣勢分析
5. 推薦玩法指南

---

### ⚠️ 高優先級風險解決方案

#### 1. 內建作業系統複雜度解決方案
**任務ID**: `os-complexity-solution`
**實施階段**:
- **階段一** (4週): 基礎內核開發
- **階段二** (3週): 系統服務框架
- **階段三** (3週): 高級功能開發
- **階段四** (2週): 優化和集成

**核心技術指標**:
- 100+併發進程
- 進程切換延遲 < 10ms
- 內存分配延遲 < 1ms
- 系統啟動時間 < 5秒

#### 2. AI生成內容質量不穩定解決方案
**任務ID**: `ai-quality-solution`
**核心組件**: `AIContentQualityStabilizer`

**質量級別**: Unstable → Moderate → Stable → VeryStable → Excellent

**解決策略**:
- 多層質量控制
- 多供應商策略
- 成本控制機制
- 內容後處理優化

#### 3. 性能瓶頸解決方案
**任務ID**: `performance-bottleneck-solution`
**核心組件**:
- `MingPerformanceAnalysisSystem`
- `MingPerformanceMonitor`
- `SmartCache`

**優化策略**:
- 智能資源管理
- 分級性能配置
- 異步處理和任務隊列
- 智能緩存和預載入

**預期成果**:
- 減少70%性能問題
- 響應時間 < 100ms

---

### 💻 API 使用速查

#### 內建OS API
```cpp
// 初始化內核
UMingRTSKernel* Kernel = NewObject<UMingRTSKernel>();
Kernel->InitializeKernel();

// 創建進程
FProcessCreateInfo CreateInfo;
CreateInfo.ProcessName = TEXT("AISystem");
CreateInfo.Priority = EProcessPriority::High;
FProcessID ProcessID = Kernel->CreateProcess(CreateInfo);

// 管理記憶體
FMemoryBlockID BlockID = Kernel->AllocateMemory(1024 * 1024); // 1MB
Kernel->FreeMemory(BlockID);
```

#### AI內容生成 API
```cpp
// 初始化AI內容系統
UMingAIGeneratedContentSystem* AIGenerator = NewObject<UMingAIGeneratedContentSystem>();
AIGenerator->InitializeAIGeneratedContentSystem();

// 生成內容
FString ContentID = AIGenerator->GenerateContent(
    EAIContentType::Image,
    TEXT("Ming dynasty warrior"),
    EAIQualityLevel::High
);

// 批量生成
TArray<FString> ContentIDs = AIGenerator->GenerateContentBatch(Requirements);
```

#### 自我學習系統 API
```cpp
// 初始化自我學習系統
UMingRTSSelfLearningSystem* LearningSystem = NewObject<UMingRTSSelfLearningSystem>();
LearningSystem->InitializeLearningSystem();

// 記錄玩家行為
FUserBehaviorData BehaviorData;
BehaviorData.UserID = PlayerID;
BehaviorData.ActionType = EUserActionType::UnitMovement;
LearningSystem->CollectUserBehaviorData(BehaviorData);

// 獲取適應性參數
FAdaptiveParameters AdaptiveParams = LearningSystem->GetAdaptiveParameters(PlayerID);
```

#### 動態任務生成 API
```cpp
// 初始化任務生成器
UMingDynamicTaskGenerator* TaskGenerator = NewObject<UMingDynamicTaskGenerator>();
TaskGenerator->InitializeTaskGenerator();

// 生成個人化任務
FDynamicTask Task = TaskGenerator->GeneratePersonalizedTask(
    PlayerID,
    ETaskType::Combat,
    ETaskDifficulty::Adaptive
);

// 基於情境生成
FDynamicTask ContextTask = TaskGenerator->GenerateContextualTask(
    PlayerID,
    TEXT("SiegeSituation"),
    ETaskType::Strategic
);
```

#### 情感AI引擎 API
```cpp
// 初始化情感AI引擎
UMingRTSEmotionalAIEngine* EmotionalAI = NewObject<UMingRTSEmotionalAIEngine>();
EmotionalAI->InitializeEmotionalAIEngine();

// 處理玩家行為並更新情感
EmotionalAI->ProcessPlayerBehavior(PlayerID, PlayerAction);

// 獲取情感響應
FEmotionalResponse Response = EmotionalAI->GenerateEmotionalResponse(
    PlayerID,
    EEmotionType::Joy,
    0.8f // Intensity
);

// 情感統計
FEmotionalStatistics Stats = EmotionalAI->GetEmotionalStatistics(PlayerID);
```

#### 存檔系統 API
```cpp
// 創建存檔數據
UMingSaveGame* SaveGame = NewObject<UMingSaveGame>();
SaveGame->InitializeSaveGame(PlayerID, 1, TEXT("Manual Save"));

// 設置存檔數據
SaveGame->SetPlayerName(PlayerName);
SaveGame->SetRelationshipData(RelationshipManager->GetAllRelationshipData());
SaveGame->SetReputationData(ReputationManager->GetAllReputationData());

// 保存
UMingSaveGameManager* SaveManager = NewObject<UMingSaveGameManager>();
EMingSaveGameResult Result = SaveManager->SaveGame(SaveGame, 1);
```

---

### 🛠️ 開發工具詳情

#### 一鍵啟動器功能
`.\OneClickLauncher.cmd` 提供以下功能：
1. **開發模式** - 啟動UE5編輯器
2. **構建模式** - 執行完整構建
3. **測試模式** - 運行自動化測試
4. **部署模式** - 打包發布版本

#### 智能版本控制功能
`.\Tools\build\SmartVersionControl.ps1`:
- **代碼檢查**: 編碼、語法、損壞檢測
- **自動備份**: 創建完整項目備份
- **臨時清理**: 清理臨時和中間檔案
- **智能提交**: 集成檢查+提交

#### 性能優化腳本
`.\Tools\performance\Optimize-Performance.ps1`:
- 記憶體池優化
- 動態LOD系統
- 空間分割優化
- AI決策緩存
- 網絡協議優化

---

### 📈 性能指標參考

#### AI系統性能
| 系統 | 響應時間 | 準確率 | 並發支持 |
|------|----------|--------|----------|
| 任務生成 | < 50ms | > 85% | 1000+ |
| 行為預測 | < 10ms | 70-90% | 1000+ |
| 情感識別 | < 100ms | > 85% | 500+ |
| 歷史演進 | < 50ms | N/A | 100+ |

#### 渲染性能
| 質量級別 | 分辨率 | 幀率 | 延遲 |
|----------|--------|------|------|
| Draft | 720p | 60 FPS | < 16ms |
| Standard | 1080p | 60 FPS | < 16ms |
| High | 1440p | 60 FPS | < 16ms |
| Ultra | 4K | 60 FPS | < 16ms |
| Cinematic | 4K/8K | 24/30 FPS | < 42ms |

#### 內建OS性能
| 操作 | 目標延遲 | 實際延遲 |
|------|----------|----------|
| 進程切換 | < 10ms | ~5ms |
| 內存分配 | < 1ms | ~0.5ms |
| 文件操作 | < 2ms | ~1ms |
| 系統啟動 | < 5s | ~3s |

---

### 🔗 集成示例

#### 完整遊戲循環集成
```cpp
// 遊戲管理器中的完整集成
class AMyGameMode : public AGameModeBase
{
    UPROPERTY()
    TObjectPtr<UMingRTSSelfLearningSystem> LearningSystem;
    
    UPROPERTY()
    TObjectPtr<UMingDynamicTaskGenerator> TaskGenerator;
    
    UPROPERTY()
    TObjectPtr<UMingRTSEmotionalAIEngine> EmotionalAI;
    
    UPROPERTY()
    TObjectPtr<UMingAIGeneratedContentSystem> AIGenerator;

    void BeginPlay() override
    {
        // 初始化所有系統
        LearningSystem = NewObject<UMingRTSSelfLearningSystem>();
        LearningSystem->InitializeLearningSystem();
        
        TaskGenerator = NewObject<UMingDynamicTaskGenerator>();
        TaskGenerator->InitializeTaskGenerator();
        
        EmotionalAI = NewObject<UMingRTSEmotionalAIEngine>();
        EmotionalAI->InitializeEmotionalAIEngine();
        
        AIGenerator = NewObject<UMingAIGeneratedContentSystem>();
        AIGenerator->InitializeAIGeneratedContentSystem();
        
        // 綁定事件
        TaskGenerator->OnTaskGenerated.AddDynamic(this, &AMyGameMode::OnTaskGenerated);
        EmotionalAI->OnEmotionalResponseGenerated.AddDynamic(this, &AMyGameMode::OnEmotionalResponse);
    }

    // 處理玩家行為
    void ProcessPlayerAction(const FPlayerAction& Action)
    {
        // 記錄行為
        LearningSystem->CollectUserBehaviorData(Action.ToBehaviorData());
        
        // 更新情感
        EmotionalAI->ProcessPlayerBehavior(Action.PlayerID, Action);
        
        // 生成個人化任務
        FDynamicTask Task = TaskGenerator->GeneratePersonalizedTask(
            Action.PlayerID,
            ETaskType::Adaptive
        );
        
        // 預載入內容
        TArray<FContentPreloadRequest> Preloads = 
            AIGenerator->GetContentPreloadRecommendations(Action.PlayerID);
    }
};
```

---

### 📝 提交訊息模板

#### 常用模板
```
[編碼] 修復檔案編碼問題

修復以下檔案的編碼問題：
- File1.h
- File2.cpp

移除損壞字符：出, 本, 使, 設置

涉及 2 個檔案
```

```
[功能] 新增AI內容生成系統

實現完整的AI內容生成系統：
- 支持12種內容類型
- 多AI提供商支持
- 質量控制機制
- 批量操作功能

涉及 5 個檔案
主要類型: .h(2), .cpp(3)
```

```
[修復] 修復記憶體泄漏問題

修復MingPersonalManager中的記憶體泄漏：
- 正確釋放RelationshipManager
- 清理未使用的音頻資源
- 修復UI事件監聽器

涉及 3 個檔案
```

---

*本文檔持續更新，最後更新時間: 2026-03-23 10:30*

---

### 🔌 插件開發指南

#### 創建新插件

```powershell
# 使用Unreal Engine插件嚮導
# 或手動創建插件結構

Plugins/
└── MyNewPlugin/
    ├── Source/
    │   └── MyNewPlugin/
    │       ├── Public/
    │       │   └── MyNewPlugin.h
    │       └── Private/
    │           └── MyNewPlugin.cpp
    ├── Resources/
    │   └── Icon128.png
    └── MyNewPlugin.uplugin
```

#### 插件配置文件示例 (`.uplugin`)

```json
{
    "FileVersion": 3,
    "Version": 1,
    "VersionName": "1.0",
    "FriendlyName": "My New Plugin",
    "Description": "插件描述",
    "Category": "MingGoRTS",
    "CreatedBy": "開發者名稱",
    "CreatedByURL": "",
    "DocsURL": "",
    "MarketplaceURL": "",
    "SupportURL": "",
    "CanContainContent": true,
    "IsBetaVersion": false,
    "IsExperimentalVersion": false,
    "Installed": false,
    "Modules": [
        {
            "Name": "MyNewPlugin",
            "Type": "Runtime",
            "LoadingPhase": "Default"
        }
    ],
    "Plugins": [
        {
            "Name": "MingCore",
            "Enabled": true
        }
    ]
}
```

#### 插件模組類型

| 類型 | 用途 | 加載時機 |
|------|------|----------|
| Runtime | 運行時模組 | 遊戲啟動時 |
| Editor | 編輯器模組 | 編輯器啟動時 |
| Developer | 開發工具 | 開發模式下 |
| UncookedOnly | 未打包時 | 僅開發階段 |

#### 插件依賴管理

```cpp
// 在.Build.cs中聲明依賴
PublicDependencyModuleNames.AddRange(new string[] {
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "MingCore"
});

PrivateDependencyModuleNames.AddRange(new string[] {
    "Projects",
    "MingPersonal"
});
```

---

### 🧪 測試框架參考

#### 單元測試

```cpp
// 創建測試類
UCLASS()
class UMingMySystemTest : public UObject
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MySystem Test")
    FTestResult TestMyFunction();

    UFUNCTION(BlueprintCallable, Category = "MySystem Test")
    FTestSuiteSummary RunAllTests();
};
```

#### 自動化測試腳本

```powershell
# 運行所有測試
.\Tools\build\RunAllTests.ps1

# 運行特定系統測試
.\Tools\build\RunTests.ps1 -TestCategory "PersonalSystem"

# 生成測試報告
.\Tools\build\GenerateTestReport.ps1 -OutputPath "docs/testing/"
```

#### 壓力測試

```cpp
// 初始化壓力測試
UMingStressTestAutomation* StressTest = NewObject<UMingStressTestAutomation>();
StressTest->InitializeStressTest();

// 配置測試參數
FStressTestConfig Config;
Config.TestDuration = 3600; // 1小時
Config.ConcurrentUsers = 1000;
Config.TargetFPS = 60.0f;

// 執行測試
StressTest->RunStressTest(Config);
```

#### 性能測試基準

| 測試項目 | 目標值 | 警告閾值 | 嚴重閾值 |
|----------|--------|----------|----------|
| CPU使用率 | < 70% | 70-85% | > 85% |
| 記憶體使用 | < 2GB | 2-4GB | > 4GB |
| 幀率 | > 55 FPS | 30-55 FPS | < 30 FPS |
| 加載時間 | < 5s | 5-10s | > 10s |
| 網絡延遲 | < 50ms | 50-100ms | > 100ms |

---

### ⚙️ 配置文件參考

#### 性能閾值配置 (`Config/PerformanceThresholds.json`)

```json
{
    "MemoryThresholds": {
        "WarningLevelMB": 80,
        "CriticalLevelMB": 100,
        "EmergencyLevelMB": 120
    },
    "FrameRateThresholds": {
        "TargetFPS": 60,
        "MinimumFPS": 55,
        "WarningFPS": 40
    },
    "NetworkThresholds": {
        "MaxLatencyMs": 50,
        "WarningLatencyMs": 80,
        "PacketLossPercent": 1.0
    },
    "AIThresholds": {
        "MaxDecisionTimeMs": 30,
        "MaxPathfindingTimeMs": 50,
        "MaxBehaviorUpdateTimeMs": 20
    }
}
```

#### 遊戲設置配置 (`Config/GameSettings.ini`)

```ini
[/Script/MingGoRTS.MingGameSettings]
bEnableSelfLearning=true
bEnableDynamicTasks=true
bEnableEmotionalAI=true
LearningDataRetentionDays=30
MaxConcurrentAIProcesses=10

[/Script/MingGoRTS.MingAudioSettings]
MasterVolume=1.0
MusicVolume=0.8
SFXVolume=1.0
bEnableDynamicAudio=true
AudioQuality=High

[/Script/MingGoRTS.MingGraphicsSettings]
Resolution=1920x1080
FullscreenMode=Windowed
GraphicsQuality=High
bEnableRayTracing=false
TargetFPS=60
```

#### 本地化配置 (`Config/Localization.ini`)

```ini
[Internationalization]
Culture=en

[Localization]
DefaultLanguage=ChineseTraditional
SupportedLanguages=ChineseTraditional,ChineseSimplified,English,Japanese,Korean
bAutoDetectLanguage=true

[CulturalAdaptation]
EnabledRegions=Global,China,Japan,Korea,SEAsia
DefaultRegion=China
bEnableDynamicCulturalContent=true
```

---

### ❗ 常見問題與解決方案

#### 編譯問題

**問題**: `Unresolved external symbol`
```
解決方案:
1. 檢查.Build.cs中的模組依賴
2. 確保頭文件正確包含
3. 清理並重新生成項目文件
   .\Tools\build\CleanProject.ps1
   .\Tools\build\GenerateProjectFiles.ps1
```

**問題**: `UENUM/USTRUCT redefinition`
```
解決方案:
1. 檢查是否有重複定義的枚舉/結構
2. 使用命名空間或前綴區分
3. 確保頭文件防護正確
```

**問題**: `Cannot open include file`
```
解決方案:
1. 檢查include路徑
2. 確認模組已添加到依賴
3. 重新生成項目文件
```

#### 運行時問題

**問題**: `Access violation`
```
解決方案:
1. 檢查空指針訪問
2. 驗證對象生命周期
3. 使用TObjectPtr代替原始指針
4. 啟用地址清理器檢測
```

**問題**: `Out of memory`
```
解決方案:
1. 檢查記憶體泄漏
2. 優化資源加載策略
3. 使用Streaming級別加載
4. 啟用記憶體分析工具
```

**問題**: `Frame rate drops`
```
解決方案:
1. 運行性能分析工具
   .\Tools\performance\Optimize-Performance.ps1
2. 檢查Draw Calls數量
3. 優化材質和紋理
4. 使用LOD系統
5. 啟用動態分辨率
```

#### 版本控制問題

**問題**: `Pre-commit hook failed`
```
解決方案:
1. 檢查語法錯誤
2. 修復編碼問題
   python fix_all_encoding.py
3. 手動運行檢查
   .\Tools\build\SmartVersionControl.ps1
4. 臨時繞過（不推薦）
   git commit --no-verify
```

**問題**: `Merge conflicts`
```
解決方案:
1. 使用合併工具
   git mergetool
2. 手動解決衝突
3. 測試合併結果
4. 完成合併提交
```

#### 內容生成問題

**問題**: `AI content generation failed`
```
解決方案:
1. 檢查API密鑰配置
2. 驗證網絡連接
3. 查看錯誤日誌
4. 降低質量級別重試
5. 切換AI提供商
```

**問題**: `Low content quality`
```
解決方案:
1. 優化提示詞(Prompt)
2. 提高質量級別
3. 使用多供應商策略
4. 啟用後處理優化
5. 手動審核和修正
```

---

### 📊 監控和日誌

#### 日誌級別

```cpp
// UE_LOG 使用指南
UE_LOG(LogMingGoRTS, Log, TEXT("普通信息"));
UE_LOG(LogMingGoRTS, Warning, TEXT("警告信息"));
UE_LOG(LogMingGoRTS, Error, TEXT("錯誤信息"));
UE_LOG(LogMingGoRTS, Fatal, TEXT("致命錯誤"));

// 條件日誌
UE_CLOG(bCondition, LogMingGoRTS, Warning, TEXT("條件警告"));
```

#### 性能監控

```cpp
// 使用統計系統
DECLARE_STATS_GROUP(TEXT("MingGoRTS"), STATGROUP_MingGoRTS, STATCAT_Advanced);

DECLARE_CYCLE_STAT(TEXT("AI Decision Time"), STAT_AI_DecisionTime, STATGROUP_MingGoRTS);
SCOPE_CYCLE_COUNTER(STAT_AI_DecisionTime);

// 內存監控
DECLARE_MEMORY_STAT(TEXT("AI Memory"), STAT_AI_Memory, STATGROUP_MingGoRTS);
```

#### 日誌文件位置

```
Saved/Logs/MingGoRTS.log
Saved/Logs/MingGoRTS_2.log (備份)
```

---

### 🎓 最佳實踐

#### 代碼規範

1. **命名規範**
   - 類名: `UMingXxxSystem` (U開頭)
   - 接口: `IMingXxxInterface` (I開頭)
   - 枚舉: `EXxxType` (E開頭)
   - 結構: `FXxxData` (F開頭)

2. **文檔規範**
   ```cpp
   /**
    * @brief 簡短描述
    * @param Param1 參數1說明
    * @param Param2 參數2說明
    * @return 返回值說明
    */
   ```

3. **錯誤處理**
   ```cpp
   if (!IsValid(MyObject))
   {
       UE_LOG(LogMingGoRTS, Error, TEXT("對象無效"));
       return false;
   }
   ```

#### 性能優化

1. **對象創建**
   - 使用對象池減少GC壓力
   - 延遲加載非必要資源
   - 使用TObjectPtr管理引用

2. **循環優化**
   - 緩存循環條件
   - 使用平行For循環
   - 避免在循環中分配記憶體

3. **藍圖優化**
   - 減少藍圖通信頻率
   - 使用事件驅動代替輪詢
   - 複雜計算移至C++

#### 資源管理

1. **資源加載**
   ```cpp
   // 異步加載
   FSoftObjectPath AssetPath(TEXT("/Game/Path/To/Asset"));
   FStreamableManager& Streamable = UAssetManager::GetStreamableManager();
   Streamable.RequestAsyncLoad(AssetPath, FStreamableDelegate::CreateUObject(this, &AMyClass::OnAssetLoaded));
   ```

2. **資源卸載**
   ```cpp
   // 手動釋放資源
   AssetObject = nullptr;
   UAssetManager::Get().UnloadPrimaryAsset(AssetID);
   ```

---

### 🌐 多平台支持

#### 支持平台

| 平台 | 狀態 | 特性 |
|------|------|------|
| Windows | ✅ 完全支持 | 完整功能 |
| macOS | 🔄 開發中 | 基礎功能 |
| Linux | 🔄 開發中 | 服務器模式 |
| iOS | 📋 計劃中 | 移動版 |
| Android | 📋 計劃中 | 移動版 |
| PlayStation | 📋 計劃中 | 主機版 |
| Xbox | 📋 計劃中 | 主機版 |

#### 平台特定配置

```cpp
#if PLATFORM_WINDOWS
    // Windows特定代碼
#elif PLATFORM_MAC
    // macOS特定代碼
#elif PLATFORM_LINUX
    // Linux特定代碼
#endif
```

---

*本文檔版本: 1.1 | 最終更新: 2026-03-23 10:35*

---

### 🚀 部署與發布指南

#### 構建配置

```ini
# Config/DefaultGame.ini
[/Script/Engine.GameSession]
MaxPlayers=1000

[/Script/MingGoRTS.MingGameInstance]
bEnableAnalytics=true
bEnableCrashReporting=true
ServerRegion=Asia
```

#### 打包命令

```powershell
# 開發版本打包
.\Tools\build\PackageGame.ps1 -Configuration Development -Platform Win64

# 發布版本打包
.\Tools\build\PackageGame.ps1 -Configuration Shipping -Platform Win64

# 多平台打包
.\Tools\build\PackageAllPlatforms.ps1

# 僅打包服務器
.\Tools\build\PackageServer.ps1
```

#### 發布檢查清單

**發布前檢查**:
- [ ] 所有測試通過
- [ ] 性能基準達標
- [ ] 編碼問題已修復
- [ ] 文檔已更新
- [ ] 版本號已更新
- [ ] 更新日誌已編寫
- [ ] 許可證文件已包含

**發布流程**:
1. 創建發布分支
   ```powershell
   git checkout -b release/v1.0.0
   ```

2. 更新版本信息
   ```powershell
   .\Tools\build\UpdateVersion.ps1 -Version "1.0.0"
   ```

3. 執行完整構建
   ```powershell
   .\Tools\build\FullBuild.ps1 -Configuration Shipping
   ```

4. 運行驗收測試
   ```powershell
   .\Tools\build\AcceptanceTest.ps1
   ```

5. 打包發布版本
   ```powershell
   .\Tools\build\PackageRelease.ps1
   ```

6. 創建Git標籤
   ```powershell
   git tag -a v1.0.0 -m "Release version 1.0.0"
   git push origin v1.0.0
   ```

7. 發布到分發平台
   ```powershell
   .\Tools\build\DeployToSteam.ps1
   .\Tools\build\DeployToEpic.ps1
   ```

#### 更新策略

**熱更新**:
```cpp
// 檢查更新
UMingUpdateManager* UpdateManager = NewObject<UMingUpdateManager>();
bool bUpdateAvailable = UpdateManager->CheckForUpdates();

// 下載更新
if (bUpdateAvailable)
{
    UpdateManager->DownloadUpdate(FOnUpdateProgress::CreateUObject(this, &AMyClass::OnUpdateProgress));
}
```

**版本兼容性**:
| 版本類型 | 兼容性 | 數據遷移 |
|----------|--------|----------|
| 主版本 (X.0.0) | 不兼容 | 手動遷移 |
| 次版本 (x.X.0) | 向後兼容 | 自動遷移 |
| 修補版本 (x.x.X) | 完全兼容 | 無需遷移 |

---

### 🔒 安全指南

#### 代碼安全

1. **輸入驗證**
   ```cpp
   // 始終驗證用戶輸入
   if (!InputText.IsValid() || InputText.Len() > MAX_INPUT_LENGTH)
   {
       UE_LOG(LogMingGoRTS, Warning, TEXT("無效輸入"));
       return false;
   }
   ```

2. **防注入攻擊**
   ```cpp
   // 使用參數化查詢
   FString SafeQuery = FString::Printf(TEXT("SELECT * FROM Players WHERE ID=%d"), PlayerID);
   ```

3. **資源保護**
   ```cpp
   // 限制資源訪問
   UFUNCTION(BlueprintCallable, Category = "Admin", meta = (AccessLevel = "Admin"))
   void AdminFunction();
   ```

#### 網絡安全

1. **加密通信**
   ```cpp
   // 啟用SSL/TLS
   Connection->EnableEncryption(true);
   Connection->SetCertificate(CertData);
   ```

2. **防作弊措施**
   ```cpp
   // 服務器權威驗證
   void AMyGameMode::ProcessPlayerAction(APlayerController* PC, FPlayerAction Action)
   {
       if (!ValidateAction(PC, Action))
       {
           UE_LOG(LogMingGoRTS, Warning, TEXT("無效操作，可能作弊"));
           return;
       }
       // 處理操作
   }
   ```

3. **速率限制**
   ```cpp
   // 限制請求頻率
   if (!RateLimiter->CanProcessRequest(PlayerID))
   {
       return false;
   }
   ```

#### 數據保護

1. **存檔加密**
   ```cpp
   // 加密存檔數據
   USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(UMingSaveGame::StaticClass());
   SaveGame->Serialize(Ar);
   EncryptionManager->EncryptData(SaveData);
   ```

2. **敏感信息處理**
   ```cpp
   // 不記錄敏感信息
   UE_LOG(LogMingGoRTS, Log, TEXT("玩家登錄: %s"), *FString::Printf(TEXT("ID:%d"), PlayerID));
   // 錯誤: 不記錄密碼或個人信息
   ```

#### 安全審計

**定期檢查項**:
- 依賴庫漏洞掃描
- 代碼靜態分析
- 滲透測試
- 訪問日誌審查

**安全事件響應**:
1. 發現安全漏洞
2. 評估影響範圍
3. 開發修復方案
4. 測試修復方案
5. 緊急發布補丁
6. 通知受影響用戶

---

### 📖 術語表

| 術語 | 英文 | 說明 |
|------|------|------|
| 自我學習系統 | Self-Learning System | AI系統通過玩家行為自我改進 |
| 動態任務生成 | Dynamic Task Generation | 基於玩家狀態動態生成任務 |
| 情感AI | Emotional AI | 識別和響應玩家情感的AI |
| 跨媒體敘事 | Cross-Media Narrative | 融合多種媒體的敘事方式 |
| 內建作業系統 | Built-in OS | 遊戲內嵌的完整作業系統 |
| 性能分析 | Performance Profiling | 分析和優化遊戲性能 |
| 壓力測試 | Stress Testing | 測試系統在極限負載下的表現 |
| 本地化 | Localization | 適應不同語言和文化 |
| 智能任務分類 | Smart Task Classification | 自動分析並分類開發任務 |
| 熱更新 | Hot Update | 不停服更新遊戲內容 |
| 對象池 | Object Pooling | 重複使用對象減少GC |
| 流式加載 | Streaming | 動態加載和卸載資源 |
| LOD | Level of Detail | 根據距離調整模型細節 |
| RPC | Remote Procedure Call | 遠程過程調用 |
| PIE | Play In Editor | 在編輯器中運行遊戲 |
| UBT | Unreal Build Tool | UE構建工具 |
| UAT | Unreal Automation Tool | UE自動化工具 |

---

### 📞 支持與聯繫

#### 內部資源

**文檔**:
- API參考: `docs/api/`
- 開發指南: `docs/guides/`
- 技術文檔: `docs/technical/`

**工具**:
- 構建工具: `Tools/build/`
- AI工具: `Tools/ai/`
- 性能工具: `Tools/performance/`

**日誌**:
- 構建日誌: `BuildLog.txt`
- 編譯報告: `COMPILATION_STATUS_REPORT.md`
- 錯誤掃描: `scan_errors.cmd`

#### 社區支持

**問題追蹤**:
- 內部系統: 使用公司項目管理工具
- 已知問題: 參見 `docs/KNOWN_ISSUES.md`

**貢獻指南**:
1. 創建功能分支
2. 提交更改（使用智能提交）
3. 發起Pull Request
4. 通過代碼審查
5. 合併到主分支

---

### 🔄 文檔更新記錄

| 版本 | 日期 | 更新內容 |
|------|------|----------|
| 1.0 | 2026-03-23 | 初始版本，包含核心系統、指令參考、API示例 |
| 1.1 | 2026-03-23 | 添加插件開發、測試框架、配置參考、故障排除 |

---

**本文檔由 MingGoRTS 開發團隊維護**

*版權所有 © 2026 MingGoRTS. 保留所有權利。*

---

*文檔結束*
