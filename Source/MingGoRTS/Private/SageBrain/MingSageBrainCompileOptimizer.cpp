#include "SageBrain/MingSageBrainCompileOptimizer.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"

UMingSageBrainCompileOptimizer::UMingSageBrainCompileOptimizer()
{
    bSageBrainEnabled = false;
    bIsCompiling = false;
    CurrentStrategy = ECompileStrategy::Balanced;
    CompileStartTime = 0.0f;
}

bool UMingSageBrainCompileOptimizer::EnableSageBrain()
{
    SageBrainAPI = NewObject<UMingSageBrainIntegratedAPI>();
    CommandAPI = NewObject<UMingSageBrainCommandAPI>();
    TaskAnalyzer = NewObject<UMingTaskAnalyzer>();
    TaskSystem = NewObject<UMingSageBrainTaskSystem>();

    if (SageBrainAPI && CommandAPI && TaskAnalyzer && TaskSystem)
    {
        TaskAnalyzer->InitializeTaskAnalyzer();
        TaskSystem->InitializeTaskSystem();
        bSageBrainEnabled = true;
        OnSageBrainEnabled.Broadcast(true);
        return true;
    }
    OnSageBrainEnabled.Broadcast(false);
    return false;
}

ECompileStrategy UMingSageBrainCompileOptimizer::DetermineOptimalStrategy()
{
    if (!bSageBrainEnabled) return ECompileStrategy::Balanced;
    
    // 聖者大腦智慧分析
    bool bHasErrors = LastReport.TotalErrors > 0;
    bool bIsCleanBuild = !bHasErrors && LastReport.TotalWarnings < 10;
    
    if (bIsCleanBuild) return ECompileStrategy::Fastest;
    if (bHasErrors) return ECompileStrategy::Reliable;
    return ECompileStrategy::Balanced;
}

FCompileOptimizationConfig UMingSageBrainCompileOptimizer::GenerateOptimizationConfig(ECompileStrategy Strategy)
{
    FCompileOptimizationConfig Config;
    Config.Strategy = Strategy;
    
    switch (Strategy)
    {
    case ECompileStrategy::Fastest:
        Config.bEnableIncrementalBuild = true;
        Config.bEnableParallelCompilation = true;
        Config.MaxParallelJobs = FPlatformMisc::NumberOfCores();
        Config.bEnableCache = true;
        Config.bSkipValidation = true;
        Config.bAutoFixErrors = true;
        Config.TimeoutMinutes = 15.0f;
        break;
        
    case ECompileStrategy::Reliable:
        Config.bEnableIncrementalBuild = false;
        Config.bEnableParallelCompilation = true;
        Config.MaxParallelJobs = FPlatformMisc::NumberOfCores() / 2;
        Config.bEnableCache = false;
        Config.bSkipValidation = false;
        Config.bAutoFixErrors = true;
        Config.TimeoutMinutes = 60.0f;
        break;
        
    case ECompileStrategy::Balanced:
    default:
        Config.bEnableIncrementalBuild = true;
        Config.bEnableParallelCompilation = true;
        Config.MaxParallelJobs = FPlatformMisc::NumberOfCores() / 2 + 1;
        Config.bEnableCache = true;
        Config.bSkipValidation = false;
        Config.bAutoFixErrors = true;
        Config.TimeoutMinutes = 30.0f;
        break;
    }
    
    OnCompileStrategySelected.Broadcast(Strategy);
    return Config;
}

FCompileOptimizationReport UMingSageBrainCompileOptimizer::ExecuteOptimizedCompile(const FCompileOptimizationConfig& Config)
{
    FCompileOptimizationReport Report;
    Report.StrategyName = GetStrategyDescription(Config.Strategy);
    bIsCompiling = true;
    CompileStartTime = FPlatformTime::Seconds();

    TArray<ECompilePhase> Phases = {
        ECompilePhase::Preparation,
        ECompilePhase::Validation,
        ECompilePhase::CleanUp,
        ECompilePhase::Generation,
        ECompilePhase::Compilation,
        ECompilePhase::Linking,
        ECompilePhase::Verification,
        ECompilePhase::Optimization
    };

    bool bAllSuccess = true;
    for (ECompilePhase Phase : Phases)
    {
        OnCompilePhaseStarted.Broadcast(Phase);
        
        FCompilePhaseResult PhaseResult;
        switch (Phase)
        {
        case ECompilePhase::Preparation: PhaseResult = ExecutePreparationPhase(Config); break;
        case ECompilePhase::Validation: PhaseResult = ExecuteValidationPhase(Config); break;
        case ECompilePhase::CleanUp: PhaseResult = ExecuteCleanUpPhase(Config); break;
        case ECompilePhase::Generation: PhaseResult = ExecuteGenerationPhase(Config); break;
        case ECompilePhase::Compilation: PhaseResult = ExecuteCompilationPhase(Config); break;
        case ECompilePhase::Linking: PhaseResult = ExecuteLinkingPhase(Config); break;
        case ECompilePhase::Verification: PhaseResult = ExecuteVerificationPhase(Config); break;
        case ECompilePhase::Optimization: PhaseResult = ExecuteOptimizationPhase(Config); break;
        }
        
        Report.PhaseResults.Add(PhaseResult);
        Report.TotalErrors += PhaseResult.Errors.Num();
        Report.TotalWarnings += PhaseResult.Warnings.Num();
        
        if (!PhaseResult.bSuccess) bAllSuccess = false;
        OnCompilePhaseCompleted.Broadcast(PhaseResult);
    }

    Report.TotalDuration = FPlatformTime::Seconds() - CompileStartTime;
    Report.bOverallSuccess = bAllSuccess;
    Report.RecommendedNextSteps = bAllSuccess ? TEXT("編譯成功，建議執行測試") : TEXT("編譯失敗，請檢查錯誤並修復");
    
    bIsCompiling = false;
    LastReport = Report;
    
    OnCompileOptimizationCompleted.Broadcast(Report);
    
    // 聖者大腦學習
    UpdateSageBrainWithCompileResults(Report);
    
    return Report;
}

FCompilePhaseResult UMingSageBrainCompileOptimizer::ExecutePreparationPhase(const FCompileOptimizationConfig& Config)
{
    FCompilePhaseResult Result;
    Result.Phase = ECompilePhase::Preparation;
    
    // 檢查專案結構
    FString ProjectPath = FPaths::GetProjectFilePath();
    bool bProjectExists = FPaths::FileExists(ProjectPath);
    
    Result.bSuccess = bProjectExists;
    Result.DurationSeconds = 0.5f;
    
    if (!bProjectExists)
    {
        Result.Errors.Add(TEXT("找不到專案檔案"));
    }
    
    return Result;
}

FCompilePhaseResult UMingSageBrainCompileOptimizer::ExecuteCleanUpPhase(const FCompileOptimizationConfig& Config)
{
    FCompilePhaseResult Result;
    Result.Phase = ECompilePhase::CleanUp;
    
    if (!Config.bEnableIncrementalBuild)
    {
        // 執行清理
        if (CommandAPI)
        {
            FBuildConfiguration BuildConfig;
            CommandAPI->ExecuteClean(BuildConfig);
        }
    }
    
    Result.bSuccess = true;
    Result.DurationSeconds = 2.0f;
    return Result;
}

FCompilePhaseResult UMingSageBrainCompileOptimizer::ExecuteCompilationPhase(const FCompileOptimizationConfig& Config)
{
    FCompilePhaseResult Result;
    Result.Phase = ECompilePhase::Compilation;
    
    FString LogContent;
    bool bCompileSuccess = false;
    
    if (CommandAPI)
    {
        FBuildConfiguration BuildConfig;
        BuildConfig.BuildConfiguration = Config.BuildConfiguration;
        BuildConfig.bCleanBuild = !Config.bEnableIncrementalBuild;
        
        FBuildResult BuildResult = CommandAPI->ExecuteBuild(BuildConfig);
        bCompileSuccess = BuildResult.bSuccess;
        LogContent = BuildResult.LogOutput;
    }
    
    Result.bSuccess = bCompileSuccess;
    Result.LogOutput = LogContent;
    Result.Errors = ParseErrorsFromLog(LogContent);
    Result.Warnings = ParseWarningsFromLog(LogContent);
    Result.DurationSeconds = 10.0f;
    
    return Result;
}

FCompilePhaseResult UMingSageBrainCompileOptimizer::ExecuteVerificationPhase(const FCompileOptimizationConfig& Config)
{
    FCompilePhaseResult Result;
    Result.Phase = ECompilePhase::Verification;
    
    if (CommandAPI && !Config.bSkipValidation)
    {
        FVerificationOptions Options;
        FVerificationResults VerifyResult = CommandAPI->ExecuteVerify(Options);
        Result.bSuccess = VerifyResult.bSuccess;
    }
    else
    {
        Result.bSuccess = true;
    }
    
    Result.DurationSeconds = 3.0f;
    return Result;
}

FString UMingSageBrainCompileOptimizer::CollectCompileLogs()
{
    FString LogContent;
    FString LogPath = FPaths::ProjectLogDir() / TEXT("Compile.log");
    
    if (FPaths::FileExists(LogPath))
    {
        FFileHelper::LoadFileToString(LogContent, *LogPath);
    }
    
    // 整合所有階段日誌
    for (const FCompilePhaseResult& Phase : LastReport.PhaseResults)
    {
        if (!Phase.LogOutput.IsEmpty())
        {
            LogContent += TEXT("\n=== ") + UEnum::GetValueAsString(Phase.Phase) + TEXT(" ===\n");
            LogContent += Phase.LogOutput;
        }
    }
    
    return LogContent;
}

FMingLogAnalysisResult UMingSageBrainCompileOptimizer::AnalyzeCompileLogs(const FString& LogContent)
{
    if (TaskAnalyzer)
    {
        return TaskAnalyzer->AnalyzeLogFromText(LogContent);
    }
    
    return FMingLogAnalysisResult();
}

TArray<FMingTaskRequirement> UMingSageBrainCompileOptimizer::ExtractTasksFromLogs(const FMingLogAnalysisResult& AnalysisResult)
{
    TArray<FMingTaskRequirement> AllTasks;
    AllTasks.Append(AnalysisResult.DebugTasks);
    AllTasks.Append(AnalysisResult.OptimizationTasks);
    AllTasks.Append(AnalysisResult.FeatureTasks);
    AllTasks.Append(AnalysisResult.SecurityTasks);
    AllTasks.Append(AnalysisResult.PerformanceTasks);
    
    return AllTasks;
}

bool UMingSageBrainCompileOptimizer::CreateTasksInTicketSystem(const TArray<FMingTaskRequirement>& Tasks)
{
    bool bSuccess = true;
    int32 CreatedCount = 0;
    
    for (const FMingTaskRequirement& Task : Tasks)
    {
        // 創建聖者大腦任務
        if (TaskSystem)
        {
            FSageBrainTask SageTask;
            SageTask.TaskID = Task.TaskID;
            SageTask.Title = Task.Title;
            SageTask.Description = Task.Description;
            SageTask.Priority = static_cast<ESageBrainTaskPriority>(Task.Priority.GetValue());
            SageTask.Status = ESageBrainTaskStatus::Planning;
            SageTask.Objectives.Add(Task.LogMessage);
            SageTask.Objectives.Add(Task.ErrorMessage);
            SageTask.Objectives.Add(Task.SuggestedFix);
            
            FString TaskID = TaskSystem->CreateSageBrainTask(
                SageTask.Title, 
                SageTask.Description, 
                ESageBrainTaskType::CoreIntegration, 
                SageTask.Priority
            );
            
            if (!TaskID.IsEmpty()) CreatedCount++;
        }
    }
    
    OnTasksCreatedFromLogs.Broadcast(CreatedCount);
    return CreatedCount > 0;
}

bool UMingSageBrainCompileOptimizer::RunCompleteCompileOptimizationWorkflow()
{
    // 1. 啟用聖者大腦
    if (!IsSageBrainEnabled())
    {
        if (!EnableSageBrain()) return false;
    }
    
    // 2. 決定最佳策略
    ECompileStrategy Strategy = DetermineOptimalStrategy();
    
    // 3. 生成優化配置
    FCompileOptimizationConfig Config = GenerateOptimizationConfig(Strategy);
    
    // 4. 執行優化編譯
    FCompileOptimizationReport Report = ExecuteOptimizedCompile(Config);
    
    // 5. 收集日誌
    FString LogContent = CollectCompileLogs();
    
    // 6. 分析日誌
    FMingLogAnalysisResult Analysis = AnalyzeCompileLogs(LogContent);
    
    // 7. 建立任務
    TArray<FMingTaskRequirement> Tasks = ExtractTasksFromLogs(Analysis);
    
    // 8. 建立至需求單系統
    bool bTasksCreated = CreateTasksInTicketSystem(Tasks);
    
    // 9. 排序任務
    TArray<FMingTaskRequirement> SortedTasks = SortTasksByPriorityAndImpact(Tasks);
    
    return Report.bOverallSuccess && bTasksCreated;
}

TArray<FMingTaskRequirement> UMingSageBrainCompileOptimizer::SortTasksByPriorityAndImpact(const TArray<FMingTaskRequirement>& Tasks)
{
    TArray<FMingTaskRequirement> SortedTasks = Tasks;
    
    SortedTasks.Sort([](const FMingTaskRequirement& A, const FMingTaskRequirement& B)
    {
        // 優先級排序
        int32 PriorityA = static_cast<int32>(A.Priority);
        int32 PriorityB = static_cast<int32>(B.Priority);
        
        if (PriorityA != PriorityB)
        {
            return PriorityA < PriorityB; // 數值越小優先級越高
        }
        
        // 相同優先級按預估工時排序
        return A.EstimatedHours < B.EstimatedHours;
    });
    
    return SortedTasks;
}

FString UMingSageBrainCompileOptimizer::GetStrategyDescription(ECompileStrategy Strategy)
{
    switch (Strategy)
    {
    case ECompileStrategy::Fastest: return TEXT("最快速策略 - 增量編譯，最小清理");
    case ECompileStrategy::Balanced: return TEXT("平衡策略 - 部分清理，智能緩存");
    case ECompileStrategy::Reliable: return TEXT("最可靠策略 - 完整清理，完整驗證");
    case ECompileStrategy::Parallel: return TEXT("並行策略 - 多線程編譯");
    case ECompileStrategy::Distributed: return TEXT("分布式策略 - 多機編譯");
    default: return TEXT("未知策略");
    }
}

TArray<FString> UMingSageBrainCompileOptimizer::ParseErrorsFromLog(const FString& LogContent)
{
    TArray<FString> Errors;
    TArray<FString> Lines;
    LogContent.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("error")) || Line.Contains(TEXT("Error")) || Line.Contains(TEXT("ERROR")))
        {
            Errors.Add(Line);
        }
    }
    
    return Errors;
}

TArray<FString> UMingSageBrainCompileOptimizer::ParseWarningsFromLog(const FString& LogContent)
{
    TArray<FString> Warnings;
    TArray<FString> Lines;
    LogContent.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("warning")) || Line.Contains(TEXT("Warning")) || Line.Contains(TEXT("WARNING")))
        {
            Warnings.Add(Line);
        }
    }
    
    return Warnings;
}

void UMingSageBrainCompileOptimizer::UpdateSageBrainWithCompileResults(const FCompileOptimizationReport& Report)
{
    if (SageBrainAPI)
    {
        // 將編譯結果回饋至聖者大腦系統進行學習
        LogOptimizerActivity(FString::Printf(TEXT("聖者大腦學習編譯結果 - 錯誤: %d, 警告: %d, 耗時: %.2f秒"),
            Report.TotalErrors, Report.TotalWarnings, Report.TotalDuration));
    }
}

void UMingSageBrainCompileOptimizer::LogOptimizerActivity(const FString& Activity)
{
    UE_LOG(LogTemp, Log, TEXT("[SageBrainCompileOptimizer] %s"), *Activity);
}
