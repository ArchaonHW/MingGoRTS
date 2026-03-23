#include "MingSageBrainConsoleAutomation.h"
#include "MingSageBrainBridge.h"
#include "MingSageBrainTestSuite.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/World.h"

// 控制台互動演示類
class MINGGORTS_API FSageBrainConsoleDemo
{
public:
    // 啟動控制台互動系統
    static bool StartConsoleInteraction()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 啟動聖者大腦控制台互動系統 ==="));
        
        // 1. 啟動並初始化自動化系統
        bool bInitialized = UMingSageBrainConsoleAutomation::StartAndInitialize();
        if (!bInitialized)
        {
            UE_LOG(LogTemp, Error, TEXT("自動化系統初始化失敗"));
            return false;
        }
        
        UE_LOG(LogTemp, Log, TEXT("✅ 自動化系統初始化成功"));
        
        // 2. 啟用智能響應
        bool bIntelligentEnabled = UMingSageBrainConsoleAutomation::EnableIntelligentResponse();
        if (bIntelligentEnabled)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 智能響應已啟用"));
        }
        
        // 3. 添加響應規則
        bool bRuleAdded = UMingSageBrainConsoleAutomation::AddResponseRule(
            TEXT("系統檢查"), TEXT("bridge perform_corruption_check"));
        if (bRuleAdded)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 響應規則已添加"));
        }
        
        // 4. 獲取系統狀態
        FString Status = UMingSageBrainConsoleAutomation::GetAutomationStatus();
        UE_LOG(LogTemp, Log, TEXT("📊 系統狀態: %s"), *Status);
        
        UE_LOG(LogTemp, Log, TEXT("=== 控制台互動系統啟動完成 ==="));
        return true;
    }
    
    // 基礎控制台互動演示
    static void BasicConsoleInteraction()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 基礎控制台互動演示 ==="));
        
        // 1. 執行系統狀態檢查
        TArray<FString> StatusCommands = {
            TEXT("status"),
            TEXT("version"),
            TEXT("help")
        };
        
        bool bStatusResult = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(
            StatusCommands, true);
        
        if (bStatusResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 狀態檢查命令執行成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 狀態檢查命令執行失敗"));
        }
        
        // 2. 執行至聖者系統初始化
        bool bInitResult = UMingSageBrainConsoleAutomation::RunSupremeSageInitSequence();
        if (bInitResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 至聖者系統初始化成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 至聖者系統初始化失敗"));
        }
        
        // 3. 獲取執行歷史
        TArray<FString> History = UMingSageBrainConsoleAutomation::GetExecutionHistory();
        UE_LOG(LogTemp, Log, TEXT("📜 執行歷史記錄數: %d"), History.Num());
        
        for (int32 i = 0; i < History.Num() && i < 5; ++i)
        {
            UE_LOG(LogTemp, Log, TEXT("  [%d] %s"), i + 1, *History[i]);
        }
    }
    
    // 高級控制台互動演示
    static void AdvancedConsoleInteraction()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 高級控制台互動演示 ==="));
        
        // 1. 運行五行輪轉序列
        UE_LOG(LogTemp, Log, TEXT("🔄 開始五行輪轉序列..."));
        bool bElementsResult = UMingSageBrainConsoleAutomation::RunFiveElementsSequence();
        if (bElementsResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 五行輪轉序列執行成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 五行輪轉序列執行失敗"));
        }
        
        // 2. 智能決策演示
        UE_LOG(LogTemp, Log, TEXT("🧠 開始智能決策演示..."));
        
        TArray<FString> TestScenarios = {
            TEXT("緊急混亂情況"),
            TEXT("穩定正常情況"),
            TEXT("一般戰略情況")
        };
        
        for (const FString& Scenario : TestScenarios)
        {
            bool bDecisionResult = UMingSageBrainConsoleAutomation::IntelligentSupremeSageDecision(Scenario);
            if (bDecisionResult)
            {
                UE_LOG(LogTemp, Log, TEXT("✅ 情境 '%s' 智能決策成功"), *Scenario);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ 情境 '%s' 智能決策失敗"), *Scenario);
            }
        }
        
        // 3. 運行防墮檢查序列
        UE_LOG(LogTemp, Log, TEXT("🛡️ 開始防墮檢查序列..."));
        bool bCorruptionResult = UMingSageBrainConsoleAutomation::RunCorruptionPreventionSequence();
        if (bCorruptionResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 防墮檢查序列執行成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 防墮檢查序列執行失敗"));
        }
    }
    
    // 工作流管理演示
    static void WorkflowManagementDemo()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 工作流管理演示 ==="));
        
        // 1. 創建自定義工作流
        TArray<FString> CustomWorkflowSteps = {
            TEXT("bridge initialize_supreme_sage_system"),
            TEXT("wait 1"),
            TEXT("bridge enter_five_element_phase Wood \"準備階段\""),
            TEXT("wait 2"),
            TEXT("bridge execute_twelve_strategy EstablishNation \"建立基礎\""),
            TEXT("bridge perform_corruption_check"),
            TEXT("bridge get_sage_command_recommendation")
        };
        
        bool bCreateResult = UMingSageBrainConsoleAutomation::CreateWorkflow(
            TEXT("demo_workflow"), CustomWorkflowSteps);
        
        if (bCreateResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 自定義工作流創建成功"));
            
            // 2. 執行工作流
            bool bExecuteResult = UMingSageBrainConsoleAutomation::ExecuteWorkflow(
                TEXT("demo_workflow"));
            
            if (bExecuteResult)
            {
                UE_LOG(LogTemp, Log, TEXT("✅ 工作流執行成功"));
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("❌ 工作流執行失敗"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 工作流創建失敗"));
        }
        
        // 3. 運行預設工作流
        UE_LOG(LogTemp, Log, TEXT("🔄 運行完整至聖者工作流..."));
        bool bFullWorkflowResult = UMingSageBrainConsoleAutomation::RunSupremeSageFullWorkflow();
        if (bFullWorkflowResult)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 完整至聖者工作流執行成功"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 完整至聖者工作流執行失敗"));
        }
    }
    
    // 定時任務演示
    static void ScheduledTasksDemo()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 定時任務演示 ==="));
        
        // 1. 添加定時任務
        bool bTaskAdded = UMingSageBrainConsoleAutomation::AddScheduledTask(
            TEXT("demo_task"), TEXT("bridge get_sage_command_recommendation"), 10.0f);
        
        if (bTaskAdded)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 定時任務添加成功（10秒後執行）"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 定時任務添加失敗"));
        }
        
        // 2. 添加重複任務
        bool bRepeatTaskAdded = UMingSageBrainConsoleAutomation::AddRepeatingTask(
            TEXT("demo_repeat_task"), TEXT("echo 重複任務執行"), 5.0f, 3);
        
        if (bRepeatTaskAdded)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 重複任務添加成功（每5秒執行，共3次）"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 重複任務添加失敗"));
        }
        
        // 3. 獲取任務列表
        TArray<FString> Tasks = UMingSageBrainConsoleAutomation::GetScheduledTasks();
        UE_LOG(LogTemp, Log, TEXT("📋 當前任務數量: %d"), Tasks.Num());
        
        for (const FString& Task : Tasks)
        {
            UE_LOG(LogTemp, Log, TEXT("  - %s"), *Task);
        }
    }
    
    // 錯誤處理和恢復演示
    static void ErrorHandlingDemo()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 錯誤處理和恢復演示 ==="));
        
        // 1. 設置錯誤重試
        bool bRetrySet = UMingSageBrainConsoleAutomation::SetErrorRetry(3, 1.0f);
        if (bRetrySet)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 錯誤重試設置成功（最多3次，延遲1秒）"));
        }
        
        // 2. 啟用自動恢復
        bool bRecoveryEnabled = UMingSageBrainConsoleAutomation::EnableAutoRecovery();
        if (bRecoveryEnabled)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 自動恢復已啟用"));
        }
        
        // 3. 測試錯誤處理（執行一個可能失敗的命令）
        TArray<FString> TestCommands = {
            TEXT("invalid_command_test"),  // 這個命令可能會失敗
            TEXT("bridge get_sage_command_recommendation")  // 這個命令應該成功
        };
        
        bool bTestResult = UMingSageBrainConsoleAutomation::ExecuteBatchCommands(
            TestCommands, true);
        
        UE_LOG(LogTemp, Log, TEXT("🧪 錯誤處理測試結果: %s"), 
            bTestResult ? TEXT("成功") : TEXT("部分失敗（預期內）"));
    }
    
    // 報告生成演示
    static void ReportGenerationDemo()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 報告生成演示 ==="));
        
        // 1. 導出執行報告
        FString ReportPath = FString::Printf(TEXT("Reports/sage_brain_demo_report_%s.txt"),
            *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
        
        bool bReportExported = UMingSageBrainConsoleAutomation::ExportExecutionReport(ReportPath);
        if (bReportExported)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 執行報告已導出: %s"), *ReportPath);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 執行報告導出失敗"));
        }
        
        // 2. 獲取最終系統狀態
        FString FinalStatus = UMingSageBrainConsoleAutomation::GetAutomationStatus();
        UE_LOG(LogTemp, Log, TEXT("📊 最終系統狀態: %s"), *FinalStatus);
        
        // 3. 獲取最終執行歷史統計
        TArray<FString> FinalHistory = UMingSageBrainConsoleAutomation::GetExecutionHistory();
        UE_LOG(LogTemp, Log, TEXT("📈 總執行歷史記錄數: %d"), FinalHistory.Num());
    }
    
    // 完整控制台互動演示
    static void FullConsoleInteractionDemo()
    {
        UE_LOG(LogTemp, Log, TEXT("🚀 開始完整控制台互動演示"));
        
        // 1. 啟動系統
        if (!StartConsoleInteraction())
        {
            UE_LOG(LogTemp, Error, TEXT("❌ 控制台互動系統啟動失敗，演示終止"));
            return;
        }
        
        // 2. 基礎互動
        BasicConsoleInteraction();
        
        // 3. 高級互動
        AdvancedConsoleInteraction();
        
        // 4. 工作流管理
        WorkflowManagementDemo();
        
        // 5. 定時任務
        ScheduledTasksDemo();
        
        // 6. 錯誤處理
        ErrorHandlingDemo();
        
        // 7. 報告生成
        ReportGenerationDemo();
        
        UE_LOG(LogTemp, Log, TEXT("✅ 完整控制台互動演示完成"));
    }
    
    // 清理和關閉
    static void CleanupAndShutdown()
    {
        UE_LOG(LogTemp, Log, TEXT("=== 清理和關閉系統 ==="));
        
        // 1. 清除執行歷史
        bool bHistoryCleared = UMingSageBrainConsoleAutomation::ClearExecutionHistory();
        if (bHistoryCleared)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 執行歷史已清除"));
        }
        
        // 2. 關閉自動化系統
        bool bShutdown = UMingSageBrainConsoleAutomation::ShutdownAutomationSystem();
        if (bShutdown)
        {
            UE_LOG(LogTemp, Log, TEXT("✅ 自動化系統已關閉"));
        }
        
        UE_LOG(LogTemp, Log, TEXT("=== 系統清理完成 ==="));
    }
};

// 在遊戲中調用的主要函數
void StartSageBrainConsoleInteraction()
{
    // 運行完整演示
    FSageBrainConsoleDemo::FullConsoleInteractionDemo();
}

// 快速啟動控制台互動
void QuickStartConsoleInteraction()
{
    FSageBrainConsoleDemo::StartConsoleInteraction();
}

// 測試控制台互動功能
void TestConsoleInteraction()
{
    UE_LOG(LogTemp, Log, TEXT("=== 測試控制台互動功能 ==="));
    
    // 運行測試套件中的控制台測試
    bool bTestResult = UMingSageBrainTestSuite::TestConsoleInteraction();
    if (bTestResult)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 控制台互動測試通過"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 控制台互動測試失敗"));
    }
    
    // 測試命令執行
    bool bCommandTest = UMingSageBrainTestSuite::TestCommandExecution();
    if (bCommandTest)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 命令執行測試通過"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 命令執行測試失敗"));
    }
    
    // 測試批量命令
    bool bBatchTest = UMingSageBrainTestSuite::TestBatchCommands();
    if (bBatchTest)
    {
        UE_LOG(LogTemp, Log, TEXT("✅ 批量命令測試通過"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("❌ 批量命令測試失敗"));
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== 控制台互動功能測試完成 ==="));
}
