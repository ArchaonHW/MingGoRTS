// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageBrain/MingSageBrainLauncher.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Async/Async.h"

UMingSageBrainLauncher::UMingSageBrainLauncher()
    : bSageBrainEnabled(false)
    , bWorkflowRunning(false)
    , BuildMode(TEXT("Development"))
    , bSkipClean(false)
    , bAutoFix(true)
    , bCreateTasks(true)
{
}

bool UMingSageBrainLauncher::EnableSageBrain()
{
    LogLauncher(TEXT("啟用聖者大腦系統..."));

    // 創建編譯優化器
    CompileOptimizer = NewObject<UMingSageBrainCompileOptimizer>();
    if (!CompileOptimizer)
    {
        LogLauncher(TEXT("無法創建編譯優化器"));
        OnSageBrainEnabled.Broadcast(false);
        return false;
    }

    // 綁定編譯優化器事件
    CompileOptimizer->OnSageBrainEnabled.AddDynamic(this, &UMingSageBrainLauncher::OnSageBrainEnabled);
    CompileOptimizer->OnCompilePhaseStarted.AddDynamic(this, &UMingSageBrainLauncher::OnPhaseStarted);
    CompileOptimizer->OnCompilePhaseCompleted.AddDynamic(this, &UMingSageBrainLauncher::OnPhaseCompleted);
    CompileOptimizer->OnCompileOptimizationCompleted.AddDynamic(this, &UMingSageBrainLauncher::OnCompileOptimizationCompleted);
    CompileOptimizer->OnTasksCreatedFromLogs.AddDynamic(this, &UMingSageBrainLauncher::OnTasksCreatedFromLogs);

    // 啟用聖者大腦
    bool bSuccess = CompileOptimizer->EnableSageBrain();
    bSageBrainEnabled = bSuccess;

    if (bSuccess)
    {
        LogLauncher(TEXT("聖者大腦系統已啟用 ✓"));
    }
    else
    {
        LogLauncher(TEXT("聖者大腦系統啟用失敗"));
    }

    OnSageBrainEnabled.Broadcast(bSuccess);
    return bSuccess;
}

void UMingSageBrainLauncher::DisableSageBrain()
{
    LogLauncher(TEXT("關閉聖者大腦系統..."));

    if (CompileOptimizer)
    {
        CompileOptimizer->OnSageBrainEnabled.RemoveAll(this);
        CompileOptimizer->OnCompilePhaseStarted.RemoveAll(this);
        CompileOptimizer->OnCompilePhaseCompleted.RemoveAll(this);
        CompileOptimizer->OnCompileOptimizationCompleted.RemoveAll(this);
        CompileOptimizer->OnTasksCreatedFromLogs.RemoveAll(this);
    }

    CompileOptimizer = nullptr;
    IntegratedAPI = nullptr;
    bSageBrainEnabled = false;
    bWorkflowRunning = false;

    LogLauncher(TEXT("聖者大腦系統已關閉"));
}

bool UMingSageBrainLauncher::RunCompleteWorkflow()
{
    if (bWorkflowRunning)
    {
        LogLauncher(TEXT("工作流程已在執行中"));
        return false;
    }

    bWorkflowRunning = true;
    OnWorkflowStarted.Broadcast();

    LogLauncher(TEXT("=============================================="));
    LogLauncher(TEXT("  🧠 聖者大腦編譯優化工作流程"));
    LogLauncher(TEXT("=============================================="));

    bool bSuccess = true;
    int32 CurrentStep = 0;
    const int32 TotalSteps = 7;

    // 步驟 1: 啟用聖者大腦
    UpdateProgress(++CurrentStep, TotalSteps, TEXT("啟用聖者大腦系統"));
    if (!Step1_EnableSageBrain())
    {
        LogLauncher(TEXT("步驟 1 失敗: 無法啟用聖者大腦"));
        bSuccess = false;
    }

    // 步驟 2: 決定策略
    if (bSuccess)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("分析最佳編譯策略"));
        bSuccess = Step2_DetermineStrategy();
    }

    // 步驟 3: 執行編譯
    if (bSuccess)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("執行優化編譯"));
        bSuccess = Step3_ExecuteCompilation();
    }

    // 步驟 4: 收集日誌
    if (bSuccess || bCreateTasks)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("收集編譯日誌"));
        bSuccess = Step4_CollectLogs() && bSuccess;
    }

    // 步驟 5: 分析日誌
    if (bSuccess || bCreateTasks)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("分析日誌內容"));
        bSuccess = Step5_AnalyzeLogs() && bSuccess;
    }

    // 步驟 6: 建立任務
    if (bCreateTasks && GeneratedTasks.Num() > 0)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("建立任務至需求單系統"));
        bSuccess = Step6_CreateTasks() && bSuccess;
    }
    else
    {
        ++CurrentStep;
    }

    // 步驟 7: 排序任務
    if (GeneratedTasks.Num() > 0)
    {
        UpdateProgress(++CurrentStep, TotalSteps, TEXT("依優先層級排序任務"));
        Step7_SortTasks();
    }
    else
    {
        ++CurrentStep;
    }

    // 生成報告
    Step8_GenerateReport();

    bWorkflowRunning = false;
    UpdateProgress(100, 100, TEXT("工作流程完成"));
    OnWorkflowCompleted.Broadcast(bSuccess, LastReport);

    LogLauncher(TEXT("=============================================="));
    LogLauncher(FString::Printf(TEXT("  結果: %s"), bSuccess ? TEXT("✓ 成功") : TEXT("✗ 失敗")));
    LogLauncher(TEXT("=============================================="));

    return bSuccess;
}

void UMingSageBrainLauncher::RunWorkflowAsync()
{
    Async(EAsyncExecution::Thread, [this]()
    {
        RunCompleteWorkflow();
    });
}

bool UMingSageBrainLauncher::AnalyzeExistingLog(const FString& LogPath)
{
    LogLauncher(TEXT("僅分析模式 - 跳過編譯階段"));

    if (!EnableSageBrain())
    {
        return false;
    }

    LastLogPath = LogPath;
    
    // 直接執行分析步驟
    bool bSuccess = Step5_AnalyzeLogs();
    
    if (bSuccess && bCreateTasks && GeneratedTasks.Num() > 0)
    {
        Step6_CreateTasks();
        Step7_SortTasks();
    }

    Step8_GenerateReport();

    return bSuccess;
}

void UMingSageBrainLauncher::SetBuildMode(const FString& Mode)
{
    BuildMode = Mode;
    LogLauncher(FString::Printf(TEXT("設定編譯模式: %s"), *Mode));
}

// ==================== 工作流程步驟實作 ====================

bool UMingSageBrainLauncher::Step1_EnableSageBrain()
{
    if (bSageBrainEnabled)
    {
        LogLauncher(TEXT("聖者大腦已啟用"));
        return true;
    }
    return EnableSageBrain();
}

bool UMingSageBrainLauncher::Step2_DetermineStrategy()
{
    if (!CompileOptimizer)
    {
        return false;
    }

    ECompileStrategy Strategy = CompileOptimizer->DetermineOptimalStrategy();
    FString StrategyDesc = CompileOptimizer->GetStrategyDescription(Strategy);
    
    LogLauncher(FString::Printf(TEXT("選擇策略: %s"), *StrategyDesc));
    
    // 根據策略設定配置
    switch (Strategy)
    {
    case ECompileStrategy::Fastest:
        bSkipClean = true;
        break;
    case ECompileStrategy::Reliable:
        bSkipClean = false;
        break;
    default:
        bSkipClean = false;
        break;
    }

    return true;
}

bool UMingSageBrainLauncher::Step3_ExecuteCompilation()
{
    if (!CompileOptimizer)
    {
        return false;
    }

    // 生成優化配置
    FCompileOptimizationConfig Config = CompileOptimizer->GenerateOptimizationConfig(
        CompileOptimizer->DetermineOptimalStrategy()
    );
    
    Config.BuildMode = BuildMode;
    Config.bSkipClean = bSkipClean;
    Config.bAutoFix = bAutoFix;

    // 執行編譯
    LastReport = CompileOptimizer->ExecuteOptimizedCompile(Config);

    return LastReport.bOverallSuccess;
}

bool UMingSageBrainLauncher::Step4_CollectLogs()
{
    if (!CompileOptimizer)
    {
        return false;
    }

    FString LogContent = CompileOptimizer->CollectCompileLogs();
    
    // 儲存日誌到檔案
    FString LogDir = FPaths::ProjectSavedDir() / TEXT("Logs");
    FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
    LastLogPath = LogDir / FString::Printf(TEXT("Compile_%s.log"), *Timestamp);
    
    FFileHelper::SaveStringToFile(LogContent, *LastLogPath);
    
    LogLauncher(FString::Printf(TEXT("日誌已儲存至: %s"), *LastLogPath));

    return true;
}

bool UMingSageBrainLauncher::Step5_AnalyzeLogs()
{
    if (!CompileOptimizer)
    {
        return false;
    }

    FString LogContent;
    if (!LastLogPath.IsEmpty() && FPaths::FileExists(LastLogPath))
    {
        FFileHelper::LoadFileToString(LogContent, *LastLogPath);
    }
    else
    {
        LogContent = CompileOptimizer->CollectCompileLogs();
    }

    // 分析日誌
    FMingLogAnalysisResult Analysis = CompileOptimizer->AnalyzeCompileLogs(LogContent);
    
    // 提取任務
    GeneratedTasks = CompileOptimizer->ExtractTasksFromLogs(Analysis);
    
    LogLauncher(FString::Printf(TEXT("分析完成: %d 個錯誤, %d 個警告"), 
        Analysis.ErrorMessages.Num(), Analysis.WarningMessages.Num()));
    LogLauncher(FString::Printf(TEXT("產生 %d 個任務需求單"), GeneratedTasks.Num()));

    return true;
}

bool UMingSageBrainLauncher::Step6_CreateTasks()
{
    if (!CompileOptimizer || GeneratedTasks.Num() == 0)
    {
        return false;
    }

    bool bSuccess = CompileOptimizer->CreateTasksInTicketSystem(GeneratedTasks);
    
    if (bSuccess)
    {
        // 儲存任務清單
        FString TaskDir = FPaths::ProjectSavedDir() / TEXT("TaskAnalysis");
        FString Timestamp = FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S"));
        TaskListPath = TaskDir / FString::Printf(TEXT("CompileTasks_%s.json"), *Timestamp);
        
        LogLauncher(FString::Printf(TEXT("已建立 %d 個任務至需求單系統"), GeneratedTasks.Num()));
        LogLauncher(FString::Printf(TEXT("任務清單儲存於: %s"), *TaskListPath));
    }

    OnTasksCreated.Broadcast(GeneratedTasks.Num());

    return bSuccess;
}

bool UMingSageBrainLauncher::Step7_SortTasks()
{
    if (!CompileOptimizer || GeneratedTasks.Num() == 0)
    {
        return false;
    }

    SortedTasks = CompileOptimizer->SortTasksByPriorityAndImpact(GeneratedTasks);

    // 輸出排序結果
    LogLauncher(TEXT("任務依優先層級排序:"));
    
    TMap<int32, int32> PriorityCounts;
    for (const auto& Task : SortedTasks)
    {
        int32 Priority = static_cast<int32>(Task.Priority);
        if (!PriorityCounts.Contains(Priority))
        {
            PriorityCounts.Add(Priority, 0);
        }
        PriorityCounts[Priority]++;
    }

    // 顯示統計
    for (auto& Pair : PriorityCounts)
    {
        FString PriorityName;
        switch (Pair.Key)
        {
        case 1: PriorityName = TEXT("🔴 Critical"); break;
        case 2: PriorityName = TEXT("🟠 High"); break;
        case 3: PriorityName = TEXT("🟡 Medium"); break;
        case 4: PriorityName = TEXT("🟢 Low"); break;
        default: PriorityName = TEXT("⚪ Lowest"); break;
        }
        
        LogLauncher(FString::Printf(TEXT("  %s: %d 個"), *PriorityName, Pair.Value));
    }

    return true;
}

void UMingSageBrainLauncher::Step8_GenerateReport()
{
    LogLauncher(TEXT("=============================================="));
    LogLauncher(TEXT("  📊 工作流程完成報告"));
    LogLauncher(TEXT("=============================================="));
    
    LogLauncher(FString::Printf(TEXT("編譯結果: %s"), 
        LastReport.bOverallSuccess ? TEXT("✓ 成功") : TEXT("✗ 失敗")));
    LogLauncher(FString::Printf(TEXT("總錯誤數: %d"), LastReport.TotalErrors));
    LogLauncher(FString::Printf(TEXT("總警告數: %d"), LastReport.TotalWarnings));
    LogLauncher(FString::Printf(TEXT("總耗時: %.2f 秒"), LastReport.TotalDuration));
    LogLauncher(FString::Printf(TEXT("產生任務: %d 個"), GeneratedTasks.Num()));

    if (!LastReport.RecommendedNextSteps.IsEmpty())
    {
        LogLauncher(FString::Printf(TEXT("建議: %s"), *LastReport.RecommendedNextSteps));
    }

    LogLauncher(TEXT("=============================================="));
}

// ==================== 輔助函數 ====================

void UMingSageBrainLauncher::LogLauncher(const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[SageBrainLauncher] %s"), *Message);
}

void UMingSageBrainLauncher::UpdateProgress(int32 CurrentStep, int32 TotalSteps, const FString& Message)
{
    int32 Percentage = (CurrentStep * 100) / TotalSteps;
    OnProgressUpdated.Broadcast(Percentage, Message);
    
    LogLauncher(FString::Printf(TEXT("[%d%%] %s"), Percentage, *Message));
}

FString UMingSageBrainLauncher::GetBuildScriptPath() const
{
    return FPaths::ProjectDir() / TEXT("BUILD_OPTIMIZED.cmd");
}

FString UMingSageBrainLauncher::GetProjectPath() const
{
    return FPaths::GetProjectFilePath();
}
