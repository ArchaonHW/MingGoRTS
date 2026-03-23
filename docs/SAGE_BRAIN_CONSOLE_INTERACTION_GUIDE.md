# 聖者大腦控制台互動使用指南

## 概述

`MingSageBrainConsoleAutomation` 提供了強大的控制台互動功能，支持命令執行、腳本運行、自動化序列、智能響應等高級功能，讓您能夠與聖者大腦系統進行深度互動。

## 快速開始

### 1. 啟動控制台系統
```cpp
// 基礎啟動
bool bSuccess = UMingSageBrainConsoleAutomation::StartAutomationSystem();

// 啟動並初始化（推薦）
bool bSuccess = UMingSageBrainConsoleAutomation::StartAndInitialize();
```

### 2. 基本命令執行
```cpp
// 執行單個命令
TArray<FString> Commands;
Commands.Add(TEXT("bridge initialize_supreme_sage_system"));
Commands.Add(TEXT("bridge get_sage_command_recommendation"));

bool bResult = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(Commands, true);
```

### 3. 腳本文件執行
```cpp
// 執行PowerShell腳本
bool bResult = UMingSageBrainConsoleAutomation::ExecuteScriptFile(
    TEXT("Tools/scripts/sage_brain_init.ps1"));
```

## 控制台互動功能

### 基礎互動

#### 命令執行
```cpp
// 批量命令執行
TArray<FString> Commands = {
    TEXT("status"),
    TEXT("help"),
    TEXT("version")
};

bool bResult = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(
    Commands, true); // 顯示輸出

// 智能序列執行
bool bResult = UMingSageBrainConsoleAutomation::ExecuteIntelligentSequence(
    TEXT("startup_sequence"));
```

#### 腳本管理
```cpp
// 執行腳本文件
bool bResult = UMingSageBrainConsoleAutomation::ExecuteScriptFile(
    TEXT("path/to/script.ps1"));

// 獲取支持的腳本類型
// 通過查看Tools/api/目錄下的.ps1文件
```

### 高級互動

#### 定時任務
```cpp
// 添加定時任務
bool bResult = UMingSageBrainConsoleAutomation::AddScheduledTask(
    TEXT("daily_check"), 
    TEXT("bridge perform_corruption_check"), 
    3600.0f); // 1小時後執行

// 添加重複任務
bool bResult = UMingSageBrainConsoleAutomation::AddRepeatingTask(
    TEXT("hourly_update"),
    TEXT("bridge get_sage_command_recommendation"),
    3600.0f, // 每小時執行
    -1); // 無限重複

// 獲取任務列表
TArray<FString> Tasks = UMingSageBrainConsoleAutomation::GetScheduledTasks();

// 移除任務
bool bResult = UMingSageBrainConsoleAutomation::RemoveScheduledTask(
    TEXT("daily_check"));
```

#### 智能響應
```cpp
// 啟用智能響應
bool bResult = UMingSageBrainConsoleAutomation::EnableIntelligentResponse();

// 添加響應規則
bool bResult = UMingSageBrainConsoleAutomation::AddResponseRule(
    TEXT("緊急情況"),
    TEXT("bridge execute_twelve_strategy BreakSituation"));

// 移除響應規則
bool bResult = UMingSageBrainConsoleAutomation::RemoveResponseRule(
    TEXT("緊急情況"));

// 禁用智能響應
bool bResult = UMingSageBrainConsoleAutomation::DisableIntelligentResponse();
```

## 至聖者指揮系統互動

### 系統初始化
```cpp
// 運行至聖者初始化序列
bool bResult = UMingSageBrainConsoleAutomation::RunSupremeSageInitSequence();

// 手動初始化
TArray<FString> InitCommands = {
    TEXT("bridge initialize_supreme_sage_system"),
    TEXT("bridge get_sage_command_recommendation"),
    TEXT("bridge perform_corruption_check")
};
UMingSageBrainConsoleAutomation::ExecuteBatchCommands(InitCommands, true);
```

### 五行輪轉互動
```cpp
// 運行完整五行輪轉序列
bool bResult = UMingSageBrainConsoleAutomation::RunFiveElementsSequence();

// 手動控制五行階段
TArray<FString> ElementCommands = {
    TEXT("bridge enter_five_element_phase Wood \"立名階段\""),
    TEXT("wait 2"),
    TEXT("bridge enter_five_element_phase Fire \"造勢階段\""),
    TEXT("wait 2"),
    TEXT("bridge enter_five_element_phase Earth \"收權階段\""),
    TEXT("wait 2"),
    TEXT("bridge enter_five_element_phase Metal \"裁斷階段\""),
    TEXT("wait 2"),
    TEXT("bridge enter_five_element_phase Water \"留白階段\"")
};
UMingSageBrainConsoleAutomation::ExecuteBatchCommands(ElementCommands, true);
```

### 策略執行互動
```cpp
// 運行六伐策略序列
bool bResult = UMingSageBrainConsoleAutomation::RunSixConquestSequence();

// 運行十二策序列
bool bResult = UMingSageBrainConsoleAutomation::RunTwelveStrategiesSequence();

// 手動執行特定策略
TArray<FString> StrategyCommands = {
    TEXT("bridge execute_six_conquest_strategy ConquerMind Righteous \"目標A\""),
    TEXT("bridge execute_twelve_strategy EstablishNation \"建立正當性\"")
};
UMingSageBrainConsoleAutomation::ExecuteBatchCommands(StrategyCommands, true);
```

### 防墮檢查互動
```cpp
// 運行防墮檢查序列
bool bResult = UMingSageBrainConsoleAutomation::RunCorruptionPreventionSequence();

// 手動防墮檢查
TArray<FString> CorruptionCommands = {
    TEXT("bridge perform_corruption_check"),
    TEXT("bridge generate_sage_self_audit_report")
};
UMingSageBrainConsoleAutomation::ExecuteBatchCommands(CorruptionCommands, true);
```

### 智能決策互動
```cpp
// 根據情境智能決策
bool bResult = UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(
    TEXT("緊急混亂情況"));

// 穩定情況決策
bool bResult = UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(
    TEXT("穩定正常情況"));

// 一般情況決策
bool bResult = UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(
    TEXT("一般戰略情況"));
```

## 工作流管理

### 創建工作流
```cpp
// 創建自定義工作流
TArray<FString> WorkflowSteps = {
    TEXT("bridge initialize_supreme_sage_system"),
    TEXT("wait 1"),
    TEXT("bridge enter_five_element_phase Wood \"準備階段\""),
    TEXT("wait 2"),
    TEXT("bridge execute_twelve_strategy EstablishNation \"建立基礎\""),
    TEXT("bridge perform_corruption_check")
};

bool bResult = UMingSageBrainConsoleAutomation::CreateWorkflow(
    TEXT("custom_workflow"), WorkflowSteps);

// 執行工作流
bool bResult = UMingSageBrainConsoleAutomation::ExecuteWorkflow(
    TEXT("custom_workflow"));
```

### 預設工作流
```cpp
// 運行完整至聖者工作流
bool bResult = UMingSageBrainConsoleAutomation::RunSupremeSageFullWorkflow();

// 運行測試序列
bool bResult = UMingSageBrainConsoleAutomation::RunTestSequence();

// 運行演示序列
bool bResult = UMingSageBrainConsoleAutomation::RunDemoSequence();

// 運行開發者序列
bool bResult = UMingSageBrainConsoleAutomation::RunDeveloperSequence();
```

## 監控和狀態

### 系統狀態
```cpp
// 獲取自動化狀態
FString Status = UMingSageBrainConsoleAutomation::GetAutomationStatus();
UE_LOG(LogTemp, Log, TEXT("自動化狀態: %s"), *Status);

// 獲取執行歷史
TArray<FString> History = UMingSageBrainConsoleAutomation::GetExecutionHistory();
for (const FString& Entry : History)
{
    UE_LOG(LogTemp, Log, TEXT("歷史記錄: %s"), *Entry);
}
```

### 報告生成
```cpp
// 導出執行報告
bool bResult = UMingSageBrainConsoleAutomation::ExportExecutionReport(
    TEXT("Reports/sage_brain_execution_report.txt"));

// 清除執行歷史
bool bResult = UMingSageBrainConsoleAutomation::ClearExecutionHistory();
```

## 錯誤處理和恢復

### 錯誤重試
```cpp
// 設置錯誤重試
bool bResult = UMingSageBrainConsoleAutomation::SetErrorRetry(
    3,    // 最大重試次數
    1.0f  // 重試延遲（秒）
);

// 啟用自動恢復
bool bResult = UMingSageBrainConsoleAutomation::EnableAutoRecovery();

// 手動恢復
bool bResult = UMingSageBrainConsoleAutomation::ManualRecovery();
```

## 配置管理

### 保存和加載配置
```cpp
// 保存當前配置
bool bResult = UMingSageBrainConsoleAutomation::SaveConfiguration(
    TEXT("Config/sage_brain_automation_config.json"));

// 加載配置
bool bResult = UMingSageBrainConsoleAutomation::LoadConfiguration(
    TEXT("Config/sage_brain_automation_config.json"));

// 重置為默認配置
bool bResult = UMingSageBrainConsoleAutomation::ResetConfiguration();
```

## 高級功能

### 條件觸發
```cpp
// 設置條件觸發
bool bResult = UMingSageBrainConsoleAutomation::SetConditionalTrigger(
    TEXT("corruption_risk > 0.8"),
    TEXT("bridge generate_sage_self_audit_report()"));

// 啟用事件監聽
bool bResult = UMingSageBrainConsoleAutomation::EnableEventListening();
```

### 系統清理
```cpp
// 關閉自動化系統
bool bResult = UMingSageBrainConsoleAutomation::ShutdownAutomationSystem();

// 清理資源
bool bResult = UMingSageBrainConsoleAutomation::CleanupResources();
```

## 實際使用場景

### 場景1：日常系統檢查
```cpp
// 每日系統健康檢查
void DailySystemCheck()
{
    // 啟動系統
    UMingSageBrainConsoleAutomation::StartAndInitialize();
    
    // 運行健康檢查序列
    TArray<FString> HealthCheck = {
        TEXT("bridge get_sage_command_recommendation"),
        TEXT("bridge perform_corruption_check"),
        TEXT("status"),
        TEXT("performance stats")
    };
    
    UMingSageBrainConsoleAutomation::ExecuteBatchCommands(HealthCheck, true);
    
    // 導出報告
    UMingSageBrainConsoleAutomation::ExportExecutionReport(
        FString::Printf(TEXT("Reports/daily_check_%s.txt"), 
        *FDateTime::Now().ToString(TEXT("%Y%m%d"))));
}
```

### 場景2：緊急情況響應
```cpp
// 緊急情況自動響應
void EmergencyResponse(const FString& Situation)
{
    UE_LOG(LogTemp, Warning, TEXT("檢測到緊急情況: %s"), *Situation);
    
    // 啟用智能響應
    UMingSageBrainConsoleAutomation::EnableIntelligentResponse();
    
    // 智能決策
    UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(Situation);
    
    // 運行防墮檢查
    UMingSageBrainConsoleAutomation::RunCorruptionPreventionSequence();
    
    // 記錄事件
    TArray<FString> EventLog = {
        FString::Printf(TEXT("log_automation_event 緊急響應: %s"), *Situation),
        TEXT("export_execution_report Reports/emergency_response.txt")
    };
    UMingSageBrainConsoleAutomation::ExecuteBatchCommands(EventLog, true);
}
```

### 場景3：策略規劃會話
```cpp
// 策略規劃互動會話
void StrategyPlanningSession()
{
    UE_LOG(LogTemp, Log, TEXT("開始策略規劃會話"));
    
    // 初始化至聖者系統
    UMingSageBrainConsoleAutomation::RunSupremeSageInitSequence();
    
    // 進入適當的五行階段
    UMingSageBrainConsoleAutomation::RunFiveElementsSequence();
    
    // 執行策略分析
    TArray<FString> PlanningCommands = {
        TEXT("bridge execute_twelve_strategy EstablishNation \"分析當前局勢\""),
        TEXT("bridge execute_twelve_strategy EstablishSystem \"建立策略框架\""),
        TEXT("bridge execute_six_conquest_strategy ConquerMind Righteous \"分析對手意圖\""),
        TEXT("bridge get_sage_command_recommendation"),
        TEXT("bridge generate_sage_self_audit_report")
    };
    
    UMingSageBrainConsoleAutomation::ExecuteBatchCommands(PlanningCommands, true);
    
    // 生成策略報告
    UMingSageBrainConsoleAutomation::ExportExecutionReport(
        TEXT("Reports/strategy_planning_session.txt"));
}
```

## 測試和驗證

### 單元測試
```cpp
// 測試控制台互動功能
bool TestConsoleInteraction()
{
    // 測試系統啟動
    bool bStarted = UMingSageBrainConsoleAutomation::StartAndInitialize();
    if (!bStarted) return false;
    
    // 測試命令執行
    TArray<FString> TestCommands = {
        TEXT("echo test"),
        TEXT("status"),
        TEXT("version")
    };
    bool bCommandResult = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(
        TestCommands, false);
    if (!bCommandResult) return false;
    
    // 測試狀態查詢
    FString Status = UMingSageBrainConsoleAutomation::GetAutomationStatus();
    if (Status.IsEmpty()) return false;
    
    UE_LOG(LogTemp, Log, TEXT("控制台互動測試通過"));
    return true;
}
```

### 集成測試
```cpp
// 完整系統集成測試
bool TestFullSystemIntegration()
{
    // 測試自動化系統
    if (!UMingSageBrainConsoleAutomation::StartAutomationSystem()) return false;
    
    // 測試至聖者系統
    if (!UMingSageBrainConsoleAutomation::RunSupremeSageInitSequence()) return false;
    
    // 測試智能決策
    if (!UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(
        TEXT("測試情境"))) return false;
    
    // 測試工作流
    if (!UMingSageBrainConsoleAutomation::RunSupremeSageFullWorkflow()) return false;
    
    UE_LOG(LogTemp, Log, TEXT("完整系統集成測試通過"));
    return true;
}
```

## 最佳實踐

### 1. 系統初始化
- 始終使用 `StartAndInitialize()` 進行完整初始化
- 檢查系統狀態後再執行關鍵操作
- 設置適當的錯誤處理和重試機制

### 2. 命令執行
- 使用批量命令執行提高效率
- 在關鍵操作前檢查系統狀態
- 記錄重要的執行歷史

### 3. 錯誤處理
- 啟用自動恢復機制
- 設置合理的重試次數和延遲
- 監控系統狀態並及時響應

### 4. 性能優化
- 使用智能序列減少冗餘操作
- 合理設置定時任務間隔
- 定期清理執行歷史

### 5. 安全考慮
- 定期執行防墮檢查
- 監控系統異常行為
- 保存重要的配置和狀態

## 故障排除

### 常見問題

#### 1. 系統啟動失敗
```
問題: StartAutomationSystem() 返回 false
解決: 檢查控制台系統是否已運行，使用 StartAndInitialize()
```

#### 2. 命令執行失敗
```
問題: ExecuteBatchCommands() 返回 false
解決: 檢查命令格式，確保系統已正確初始化
```

#### 3. 智能響應不工作
```
問題: 智能響應規則沒有觸發
解決: 確保已調用 EnableIntelligentResponse()，檢查觸發詞匹配
```

#### 4. 工作流執行失敗
```
問題: ExecuteWorkflow() 返回 false
解決: 檢查工作流是否存在，驗證步驟格式正確性
```

## 總結

聖者大腦控制台互動系統提供了完整的功能集，支持：

- **基礎互動**: 命令執行、腳本運行
- **自動化功能**: 定時任務、智能響應、工作流
- **至聖者集成**: 完整的指揮系統互動
- **監控管理**: 狀態查詢、報告生成
- **錯誤處理**: 重試機制、自動恢復

通過合理使用這些功能，您可以實現與聖者大腦系統的高效互動，充分利用其智慧決策和自動化能力。
