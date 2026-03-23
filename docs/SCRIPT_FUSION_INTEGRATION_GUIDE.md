# MingGoRTS 腳本融合整合指南

## 概述

MingGoRTS 腳本融合系統是一個統一的API平台，將所有現有的PowerShell腳本整合到智慧化的API功能中，提供自動化、智能化的腳本管理和執行能力。

## 🎯 核心功能

### 腳本發現和分類
- **自動發現**: 掃描Tools目錄下的所有.ps1文件
- **智能分類**: 根據路徑和內容自動分類腳本
- **元數據提取**: 自動提取腳本參數、依賴關係和執行時間估計
- **實時更新**: 支持腳本註冊表的動態刷新

### 腳本融合任務管理
- **任務創建**: 支持創建複雜的腳本融合任務
- **多種執行模式**: 順序、並行、管道、條件、事件驅動、智慧執行
- **優先級管理**: 關鍵、高、中、低、背景五個優先級級別
- **依賴關係處理**: 自動處理腳本間的依賴關係

### 預設融合配置
- **構建配置**: 完整的構建和編譯流程
- **自動化配置**: 開發自動化工作流程
- **優化配置**: 系統性能和優化流程
- **測試配置**: 綜合測試套件
- **部署配置**: 生產環境部署流程

### 智慧融合功能
- **智能建議**: 基於目標自動建議最優腳本組合
- **執行優化**: 自動優化腳本執行順序和模式
- **預測分析**: 基於上下文預測所需腳本
- **學習適應**: 從執行結果中學習並改進

## 📁 腳本分類

### 構建腳本 (Build Scripts)
- **功能**: 編譯、構建、語法檢查
- **數量**: 19個腳本
- **關鍵腳本**:
  - `SelfLearningCompile.ps1` - 自我學習編譯
  - `AutoCompileDebugger.ps1` - 自動編譯調試
  - `ComprehensiveSyntaxFix.ps1` - 綜合語法修復
  - `IntegrationTestRunner.ps1` - 集成測試運行器

### 自動化腳本 (Automation Scripts)
- **功能**: 開發自動化、持續集成
- **數量**: 7個腳本
- **關鍵腳本**:
  - `AutoDevelopmentSystem.ps1` - 自動開發系統
  - `ContinuousIntegration.ps1` - 持續集成
  - `AutoGenerateAndDeploy.ps1` - 自動生成和部署

### AI腳本 (AI Scripts)
- **功能**: AI內容生成、資產創建
- **數量**: 5個腳本
- **關鍵腳本**:
  - `auto_generate.ps1` - 自動生成
  - `epic_assets_generate.ps1` - 史詩資產生成
  - `generate_music.ps1` - 音樂生成

### API腳本 (API Scripts)
- **功能**: API增強、整合
- **數量**: 1個腳本
- **關鍵腳本**:
  - `ImplementAPIEnhancements.ps1` - API增強實施

### 優化腳本 (Optimization Scripts)
- **功能**: 性能優化、資源管理
- **數量**: 5個腳本
- **關鍵腳本**:
  - `IntelligentRecommendationGenerator.ps1` - 智能推薦生成器
  - `ExecuteThreePhaseDevelopment.ps1` - 三階段開發執行

### 除錯腳本 (Debug Scripts)
- **功能**: 測試、除錯、監控
- **數量**: 4個腳本
- **關鍵腳本**:
  - `IntegrationTestRunner.ps1` - 集成測試運行器
  - `RiskMonitoring.ps1` - 風險監控

### 安全腳本 (Security Scripts)
- **功能**: 安全掃描、漏洞檢測
- **數量**: 1個腳本
- **關鍵腳本**:
  - `VulnerabilityScanner.ps1` - 漏洞掃描器

### 維護腳本 (Maintenance Scripts)
- **功能**: 系統維護、環境設置
- **數量**: 5個腳本
- **關鍵腳本**:
  - `SetupUE5Environment.ps1` - UE5環境設置
  - `FixGarbledText.ps1` - 亂碼修復

## 🚀 使用方法

### 基礎API使用

```cpp
// 初始化腳本融合API
UMingRTSScriptFusionAPI* ScriptFusion = NewObject<UMingRTSScriptFusionAPI>();
ScriptFusion->InitializeScriptFusionSystem();

// 發現所有腳本
ScriptFusion->DiscoverAllScripts();

// 獲取特定類別的腳本
TArray<FString> BuildScripts = ScriptFusion->GetScriptsByCategory(EScriptCategory::Build);

// 創建融合任務
FScriptFusionTask Task;
Task.TaskName = TEXT("Complete Build Process");
Task.FusionType = EScriptFusionType::BuildScripts;
Task.ScriptPaths = BuildScripts;
Task.ExecutionMode = EFusionMode::Pipeline;
Task.Priority = EScriptPriority::High;

FString TaskID = ScriptFusion->CreateFusionTask(Task);

// 執行任務
ScriptFusion->ExecuteFusionTask(TaskID);
```

### 預設配置使用

```cpp
// 創建並執行構建配置
FScriptFusionProfile BuildProfile = ScriptFusion->CreateBuildProfile();
ScriptFusion->ExecuteProfile(BuildProfile.ProfileID);

// 創建並執行自動化配置
FScriptFusionProfile AutomationProfile = ScriptFusion->CreateAutomationProfile();
ScriptFusion->ExecuteProfile(AutomationProfile.ProfileID);

// 創建並執行測試配置
FScriptFusionProfile TestingProfile = ScriptFusion->CreateTestingProfile();
ScriptFusion->ExecuteProfile(TestingProfile.ProfileID);
```

### 智慧融合使用

```cpp
// 獲得智能建議
FScriptFusionTask SuggestedTask = ScriptFusion->SuggestOptimalFusion(TEXT("complete build and test"));
FString SuggestedTaskID = ScriptFusion->CreateFusionTask(SuggestedTask);

// 啟用智慧融合
ScriptFusion->EnableIntelligentFusion(true);

// 優化任務執行
ScriptFusion->OptimizeTaskExecution(SuggestedTaskID);

// 預測所需腳本
TArray<FString> PredictedScripts = ScriptFusion->PredictRequiredScripts(TEXT("deployment preparation"));
```

### 事件處理

```cpp
// 綁定事件處理
ScriptFusion->OnTaskStarted.AddDynamic(this, &AMyGameMode::OnTaskStarted);
ScriptFusion->OnTaskCompleted.AddDynamic(this, &AMyGameMode::OnTaskCompleted);
ScriptFusion->OnTaskFailed.AddDynamic(this, &AMyGameMode::OnTaskFailed);

// 處理事件
UFUNCTION()
void AMyGameMode::OnTaskStarted(const FString& TaskID, const FScriptFusionTask& Task)
{
    UE_LOG(LogTemp, Log, TEXT("Task started: %s"), *Task.TaskName);
}

UFUNCTION()
void AMyGameMode::OnTaskCompleted(const FString& TaskID, const FScriptFusionResult& Result)
{
    if (Result.bSuccess) {
        UE_LOG(LogTemp, Log, TEXT("Task completed successfully: %s"), *TaskID);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("Task completed with errors: %s"), *TaskID);
    }
}
```

## 🛠️ PowerShell整合腳本

### 基礎使用

```powershell
# 執行所有腳本（順序模式）
.\IntegrateAllScripts.ps1 -Category "All" -Mode "Sequential"

# 執行特定類別腳本（並行模式）
.\IntegrateAllScripts.ps1 -Category "Build" -Mode "Parallel"

# 執行測試腳本（管道模式）
.\IntegrateAllScripts.ps1 -Category "Debug" -Mode "Pipeline"

# 乾運行（不實際執行）
.\IntegrateAllScripts.ps1 -Category "All" -Mode "Sequential" -DryRun
```

### 高級使用

```powershell
# 執行AI腳本並生成詳細報告
.\IntegrateAllScripts.ps1 -Category "AI" -Mode "Parallel" -Verbose

# 執行構建腳本（包含錯誤處理）
try {
    .\IntegrateAllScripts.ps1 -Category "Build" -Mode "Pipeline"
    Write-Host "Build completed successfully" -ForegroundColor Green
} catch {
    Write-Host "Build failed: $($_.Exception.Message)" -ForegroundColor Red
}

# 批量執行多個類別
$Categories = @("Build", "Automation", "Testing")
foreach ($Cat in $Categories) {
    Write-Host "Executing category: $Cat" -ForegroundColor Yellow
    .\IntegrateAllScripts.ps1 -Category $Cat -Mode "Sequential"
}
```

## 📊 執行模式

### 順序執行 (Sequential)
- **特點**: 腳本按順序執行，失敗時停止
- **適用**: 有依賴關係的腳本鏈
- **優點**: 簡單可控，易於除錯
- **缺點**: 執行時間較長

### 並行執行 (Parallel)
- **特點**: 腳本同時執行，最多4個並行任務
- **適用**: 獨立的腳本集合
- **優點**: 執行速度快，資源利用率高
- **缺點**: 錯誤處理複雜

### 管道執行 (Pipeline)
- **特點**: 前一個腳本的輸出作為後一個的輸入
- **適用**: 數據處理管道
- **優點**: 數據流暢處理
- **缺點**: 依賴關係強

### 智慧執行 (Intelligent)
- **特點**: 自動優化執行順序和模式
- **適用**: 複雜的腳本組合
- **優點**: 最高效率
- **缺點**: 需要學習和適應

## 🔧 配置選項

### 系統配置

```cpp
// 設置最大並行任務數
ScriptFusion->SetMaxParallelTasks(8);

// 啟用詳細日誌
ScriptFusion->EnableDetailedLogging(true);

// 啟用自動重試
ScriptFusion->EnableAutoRetry(true);
ScriptFusion->SetMaxRetryAttempts(3);

// 啟用智慧融合
ScriptFusion->EnableIntelligentFusion(true);
```

### 任務配置

```cpp
FScriptFusionTask Task;
Task.TaskName = TEXT("Custom Task");
Task.FusionType = EScriptFusionType::AllScripts;
Task.ExecutionMode = EFusionMode::Intelligent;
Task.Priority = EScriptPriority::High;
Task.bEnabled = true;

// 添加參數
Task.Parameters.Add(TEXT("Environment"), TEXT("Development"));
Task.Parameters.Add(TEXT("LogLevel"), TEXT("Verbose"));

// 添加依賴
Task.Dependencies.Add(TEXT("SetupEnvironment"));
Task.Dependencies.Add(TEXT("ValidateInputs"));

// 設置調度
Task.Schedule = TEXT("0 2 * * *"); // 每天凌晨2點
```

## 📈 監控和報告

### 實時監控

```cpp
// 獲取運行中的任務
TArray<FString> RunningTasks = ScriptFusion->GetRunningTasks();

// 獲取任務結果
FScriptFusionResult Result = ScriptFusion->GetTaskResult(TaskID);

// 獲取執行歷史
TArray<FScriptFusionResult> History = ScriptFusion->GetExecutionHistory(
    FDateTime::Now() - FTimespan::FromDays(7), 
    FDateTime::Now()
);

// 生成執行報告
ScriptFusion->GenerateExecutionReport(TEXT("C:/Reports/ExecutionReport.html"));
```

### PowerShell報告

```powershell
# 執行並生成HTML報告
.\IntegrateAllScripts.ps1 -Category "All" -Mode "Sequential" -Verbose

# 報告位置: C:\HW\MingGoRTS\Reports\ScriptIntegrationReport_YYYYMMDD_HHMMSS.html

# 查看日誌
Get-Content "C:\HW\MingGoRTS\Logs\ScriptIntegration_YYYYMMDD_HHMMSS.log"
```

## 🎯 最佳實踐

### 1. 腳本組織
- **命名規範**: 使用一致的命名規範
- **分類清晰**: 確保腳本放在正確的目錄
- **文檔完整**: 添加詳細的註釋和說明
- **版本控制**: 使用版本控制管理腳本變更

### 2. 任務設計
- **粒度適中**: 避免過大或過小的任務
- **依賴明確**: 清楚定義腳本間的依賴關係
- **錯誤處理**: 添加適當的錯誤處理邏輯
- **超時控制**: 設置合理的執行超時

### 3. 性能優化
- **並行化**: 對獨立腳本使用並行執行
- **緩存利用**: 利用腳本執行結果緩存
- **資源管理**: 合理管理系統資源
- **監控調整**: 根據監控數據調整配置

### 4. 安全考慮
- **權限控制**: 確保腳本有適當的執行權限
- **輸入驗證**: 驗證腳本參數和輸入
- **日誌審計**: 記錄所有腳本執行活動
- **錯誤隔離**: 隔離腳本錯誤避免影響系統

## 🔍 故障排除

### 常見問題

#### 腳本執行失敗
```
問題: 腳本執行返回錯誤
解決: 檢查腳本路徑、權限和依賴關係
```

#### 並行執行衝突
```
問題: 並行執行時出現資源衝突
解決: 減少並行任務數量或使用順序執行
```

#### 管道執行中斷
```
問題: 管道執行在某個腳本處中斷
解決: 檢查腳本輸出格式和依賴關係
```

#### 智慧建議不準確
```
問題: 智慧建議的腳本組合不適用
解決: 提供更具體的目標描述或手動調整
```

### 調試技巧

#### 啟用詳細日誌
```cpp
ScriptFusion->EnableDetailedLogging(true);
```

#### 乾運行測試
```powershell
.\IntegrateAllScripts.ps1 -Category "All" -Mode "Sequential" -DryRun
```

#### 單獨測試腳本
```powershell
# 測試單個腳本
.\Tools\build\SelfLearningCompile.ps1

# 檢查腳本語法
Get-Command .\Tools\build\SelfLearningCompile.ps1 -Syntax
```

## 🚀 未來擴展

### 計劃功能
- **Web界面**: 基於Web的腳本管理界面
- **分布式執行**: 支持多機分布式腳本執行
- **機器學習**: 基於ML的腳本推薦和優化
- **容器化**: 支持容器化腳本執行環境

### 技術改進
- **實時監控**: 更豐富的實時監控功能
- **自動化測試**: 腳本自動化測試框架
- **版本管理**: 集成版本控制系統
- **雲端同步**: 雲端腳本同步和備份

## 📝 總結

MingGoRTS 腳本融合系統提供了一個強大而靈活的平台，將所有現有腳本統一整合到智慧化的API功能中。通過自動發現、智能分類、多種執行模式和智慧優化，系統顯著提升了腳本管理和執行的效率。

### 主要優勢
- **統一管理**: 所有腳本集中管理和執行
- **智能化**: 自動優化和建議功能
- **多樣化**: 支持多種執行模式和配置
- **可擴展**: 易於添加新腳本和功能
- **監控完善**: 詳細的監控和報告功能

### 適用場景
- **開發自動化**: 完整的開發流程自動化
- **持續集成**: CI/CD管道的自動化
- **系統維護**: 定期系統維護和優化
- **部署流程**: 自動化的部署和發布
- **測試執行**: 綜合測試套件的執行

通過使用腳本融合系統，開發團隊可以大幅提升工作效率，降低人為錯誤，實現真正的DevOps自動化。

---

**文檔版本**: 1.0  
**最後更新**: 2026-03-23  
**作者**: MingGoRTS開發團隊  
**審核狀態**: 已完成
