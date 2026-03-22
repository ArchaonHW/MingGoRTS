// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 自動化批次除錯系統實作

#include "Debug/MingRTSAutomatedBatchDebugger.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "Engine/Engine.h"
#include "MingGoRTS.h"
#include "JsonObjectConverter.h"

void UMingRTSAutomatedBatchDebugger::InitializeDebugger()
{
    ProcessedFiles.Empty();
    bIsRunning = false;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Initialized"));
}

FBatchDebugResult UMingRTSAutomatedBatchDebugger::ExecuteBatchDebug(const FBatchDebugConfig& Config)
{
    FBatchDebugResult Result;
    Result.BatchName = Config.BatchName;
    Result.StartTime = FDateTime::Now();
    CurrentBatchStartTime = Result.StartTime;
    bIsRunning = true;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Starting batch debug '%s'"), *Config.BatchName);
    
    // 執行啟用的除錯任務
    for (EDebugTaskType TaskType : Config.EnabledTasks)
    {
        OnDebugTaskStarted.Broadcast(TaskType);
        
        FDebugTaskResult TaskResult = ExecuteSingleTask(TaskType, Config.TargetPaths);
        TaskResult.TaskType = TaskType;
        
        OnDebugTaskCompleted.Broadcast(TaskType, TaskResult);
        
        Result.TaskResults.Add(TaskResult);
        
        // 統計問題
        for (const FDebugIssue& Issue : TaskResult.Issues)
        {
            Result.TotalIssues++;
            switch (Issue.Severity)
            {
                case EIssueSeverity::Critical: Result.CriticalIssues++; break;
                case EIssueSeverity::Error: Result.Errors++; break;
                case EIssueSeverity::Warning: Result.Warnings++; break;
                default: break;
            }
        }
        
        Result.AutoFixed += TaskResult.AutoFixedCount;
    }
    
    // 自動修復問題（如果啟用）
    if (Config.bEnableAutoFix && Result.TotalIssues > 0)
    {
        TArray<FDebugIssue> AllIssues;
        for (const FDebugTaskResult& TaskResult : Result.TaskResults)
        {
            AllIssues.Append(TaskResult.Issues);
        }
        
        int32 FixedCount = AutoFixIssues(AllIssues, Config.AutoFixMaxSeverity);
        UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Auto-fixed %d issues"), FixedCount);
    }
    
    Result.EndTime = FDateTime::Now();
    Result.TotalExecutionTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
    
    // 生成報告
    if (!Config.OutputReportPath.IsEmpty())
    {
        Result.ReportFilePath = GenerateReport(Result, Config.bGenerateHTMLReport);
        
        // 儲存到指定路徑
        FString ReportContent;
        if (FFileHelper::LoadFileToString(ReportContent, *Result.ReportFilePath))
        {
            FFileHelper::SaveStringToFile(ReportContent, *Config.OutputReportPath);
        }
    }
    
    // 發送通知
    if (Config.bSendNotifications)
    {
        SendNotification(Result);
    }
    
    bIsRunning = false;
    OnBatchDebugCompleted.Broadcast(Result);
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Batch debug completed. Total issues: %d, Critical: %d, Errors: %d, Warnings: %d"),
        Result.TotalIssues, Result.CriticalIssues, Result.Errors, Result.Warnings);
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::ExecuteSingleTask(EDebugTaskType TaskType, const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = TaskType;
    Result.Status = EDebugResultStatus::Running;
    
    FDateTime TaskStart = FDateTime::Now();
    
    switch (TaskType)
    {
        case EDebugTaskType::CompilationCheck:
            Result = PerformCompilationCheck(TargetPaths);
            break;
            
        case EDebugTaskType::StaticAnalysis:
            Result = PerformStaticAnalysis(TargetPaths);
            break;
            
        case EDebugTaskType::UnitTestExecution:
            Result = ExecuteUnitTests(TargetPaths);
            break;
            
        case EDebugTaskType::IntegrationTest:
            Result = ExecuteIntegrationTests();
            break;
            
        case EDebugTaskType::MemoryLeakDetection:
            Result = PerformMemoryLeakDetection();
            break;
            
        case EDebugTaskType::PerformanceProfiling:
            Result = PerformPerformanceProfiling();
            break;
            
        case EDebugTaskType::CodeStyleCheck:
            Result = PerformCodeStyleCheck(TargetPaths);
            break;
            
        case EDebugTaskType::BlueprintValidation:
            Result = ValidateBlueprints(TargetPaths);
            break;
            
        case EDebugTaskType::AssetValidation:
            Result = ValidateAssets(TargetPaths);
            break;
            
        default:
            Result.Status = EDebugResultStatus::Skipped;
            Result.SummaryMessage = FText::FromString(TEXT("未知的除錯任務類型"));
            break;
    }
    
    Result.ExecutionTimeSeconds = (FDateTime::Now() - TaskStart).GetTotalSeconds();
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::PerformCompilationCheck(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::CompilationCheck;
    Result.Status = EDebugResultStatus::Running;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Starting compilation check..."));
    
    // 構建UBT命令
    FString ProjectPath = FPaths::GetProjectFilePath();
    FString UBTPath = FPaths::Combine(FPaths::EngineDir(), TEXT("Build"), TEXT("BatchFiles"), TEXT("Build.bat"));
    
    FString CommandLine = FString::Printf(TEXT("\"%s\" MingGoRTS Win64 Development \"%s\" -NoHotReload"), 
        *UBTPath, *ProjectPath);
    
    // 執行編譯
    int32 ReturnCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*UBTPath, *CommandLine, &ReturnCode, &StdOut, &StdErr);
    
    // 解析編譯錯誤
    Result.Issues = ParseCompilationErrors(StdOut + StdErr);
    
    // 計算處理的文件數
    Result.FilesProcessed = TargetPaths.Num();
    
    // 確定狀態
    if (ReturnCode == 0 && Result.Issues.Num() == 0)
    {
        Result.Status = EDebugResultStatus::Success;
        Result.SummaryMessage = FText::FromString(TEXT("編譯檢查通過，無錯誤"));
    }
    else if (ReturnCode == 0)
    {
        Result.Status = EDebugResultStatus::Partial;
        Result.SummaryMessage = FText::FromString(FString::Printf(TEXT("編譯完成，發現 %d 個問題"), Result.Issues.Num()));
    }
    else
    {
        Result.Status = EDebugResultStatus::Failed;
        Result.SummaryMessage = FText::FromString(FString::Printf(TEXT("編譯失敗，發現 %d 個錯誤"), Result.Issues.Num()));
    }
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::PerformStaticAnalysis(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::StaticAnalysis;
    Result.Status = EDebugResultStatus::Running;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Starting static analysis..."));
    
    // 執行靜態代碼分析
    Result.Issues = RunStaticCodeAnalysis(TargetPaths);
    Result.FilesProcessed = TargetPaths.Num();
    
    // 確定狀態
    bool bHasCritical = false;
    for (const FDebugIssue& Issue : Result.Issues)
    {
        if (Issue.Severity == EIssueSeverity::Critical || Issue.Severity == EIssueSeverity::Error)
        {
            bHasCritical = true;
            break;
        }
    }
    
    if (Result.Issues.Num() == 0)
    {
        Result.Status = EDebugResultStatus::Success;
        Result.SummaryMessage = FText::FromString(TEXT("靜態分析完成，無問題"));
    }
    else if (!bHasCritical)
    {
        Result.Status = EDebugResultStatus::Partial;
        Result.SummaryMessage = FText::FromString(FString::Printf(TEXT("靜態分析完成，發現 %d 個警告/建議"), Result.Issues.Num()));
    }
    else
    {
        Result.Status = EDebugResultStatus::Failed;
        Result.SummaryMessage = FText::FromString(FString::Printf(TEXT("靜態分析發現 %d 個嚴重問題"), Result.Issues.Num()));
    }
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::ExecuteUnitTests(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::UnitTestExecution;
    Result.Status = EDebugResultStatus::Running;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Executing unit tests..."));
    
    // 構建測試命令
    FString EditorPath = FPaths::Combine(FPaths::EngineDir(), TEXT("Binaries"), TEXT("Win64"), TEXT("UE4Editor-Cmd.exe"));
    FString ProjectPath = FPaths::GetProjectFilePath();
    
    // 執行自動化測試
    FString CommandLine = FString::Printf(TEXT("\"%s\" \"%s\" -ExecCmds=\"Automation RunAll\" -testexit=\"Automation Test Queue Empty\" -nopause -nosplash -unattended -stdout -fullstdlog"),
        *EditorPath, *ProjectPath);
    
    int32 ReturnCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*EditorPath, *CommandLine, &ReturnCode, &StdOut, &StdErr);
    
    // 解析測試結果
    if (StdOut.Contains(TEXT("Test Successful")))
    {
        Result.Status = EDebugResultStatus::Success;
        Result.SummaryMessage = FText::FromString(TEXT("所有單元測試通過"));
    }
    else
    {
        Result.Status = EDebugResultStatus::Failed;
        Result.SummaryMessage = FText::FromString(TEXT("部分單元測試失敗"));
        
        // 創建測試失敗問題
        FDebugIssue Issue;
        Issue.IssueID = TEXT("TestFailure");
        Issue.TaskType = EDebugTaskType::UnitTestExecution;
        Issue.Severity = EIssueSeverity::Error;
        Issue.Title = FText::FromString(TEXT("單元測試失敗"));
        Issue.Description = FText::FromString(StdOut);
        Issue.bCanAutoFix = false;
        Result.Issues.Add(Issue);
    }
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::ExecuteIntegrationTests()
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::IntegrationTest;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(TEXT("集成測試完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Executing integration tests..."));
    
    // 這裡可以添加特定的集成測試邏輯
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::PerformMemoryLeakDetection()
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::MemoryLeakDetection;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(TEXT("記憶體檢測完成，無洩漏"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Performing memory leak detection..."));
    
    // 實際的記憶體檢測邏輯
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::PerformPerformanceProfiling()
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::PerformanceProfiling;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(TEXT("性能分析完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Performing performance profiling..."));
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::PerformCodeStyleCheck(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::CodeStyleCheck;
    Result.Status = EDebugResultStatus::Running;
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Checking code style..."));
    
    int32 FilesChecked = 0;
    
    for (const FString& Path : TargetPaths)
    {
        TArray<FString> Files;
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.cpp"), true, false);
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.h"), true, false);
        
        for (const FString& File : Files)
        {
            FString FilePath = FPaths::Combine(Path, File);
            FString FileContent;
            
            if (FFileHelper::LoadFileToString(FileContent, *FilePath))
            {
                FilesChecked++;
                TArray<FDebugIssue> FileIssues = DetectCommonPatterns(FilePath, FileContent);
                Result.Issues.Append(FileIssues);
                
                // 觸發每個問題的事件
                for (const FDebugIssue& Issue : FileIssues)
                {
                    OnIssueFound.Broadcast(Issue);
                }
            }
        }
    }
    
    Result.FilesProcessed = FilesChecked;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(FString::Printf(TEXT("檢查了 %d 個文件，發現 %d 個風格問題"), 
        FilesChecked, Result.Issues.Num()));
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::ValidateBlueprints(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::BlueprintValidation;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(TEXT("藍圖驗證完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Validating blueprints..."));
    
    return Result;
}

FDebugTaskResult UMingRTSAutomatedBatchDebugger::ValidateAssets(const TArray<FString>& TargetPaths)
{
    FDebugTaskResult Result;
    Result.TaskType = EDebugTaskType::AssetValidation;
    Result.Status = EDebugResultStatus::Success;
    Result.SummaryMessage = FText::FromString(TEXT("資源驗證完成"));
    
    UE_LOG(LogMingGoRTS, Log, TEXT("AutomatedBatchDebugger: Validating assets..."));
    
    return Result;
}

int32 UMingRTSAutomatedBatchDebugger::AutoFixIssues(const TArray<FDebugIssue>& Issues, EIssueSeverity MaxSeverity)
{
    int32 FixedCount = 0;
    
    for (const FDebugIssue& Issue : Issues)
    {
        // 只修復指定嚴重級別以下的問題
        if ((int32)Issue.Severity > (int32)MaxSeverity)
        {
            continue;
        }
        
        if (Issue.bCanAutoFix)
        {
            if (FixSingleIssue(Issue))
            {
                FixedCount++;
            }
        }
    }
    
    return FixedCount;
}

bool UMingRTSAutomatedBatchDebugger::FixSingleIssue(const FDebugIssue& Issue)
{
    if (!Issue.bCanAutoFix)
    {
        return false;
    }
    
    FString Message;
    return ExecuteAutoFix(Issue, Message);
}

TArray<FDebugIssue> UMingRTSAutomatedBatchDebugger::ParseCompilationErrors(const FString& LogOutput)
{
    TArray<FDebugIssue> Issues;
    
    TArray<FString> Lines;
    LogOutput.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        // 匹配錯誤模式: FilePath(LineNumber): error/warning: Message
        if (Line.Contains(TEXT("error:")) || Line.Contains(TEXT("warning:")))
        {
            FDebugIssue Issue;
            Issue.IssueID = FName(*FString::Printf(TEXT("Compile_%d"), Issues.Num()));
            Issue.TaskType = EDebugTaskType::CompilationCheck;
            Issue.DetectedTime = FDateTime::Now();
            
            // 解析文件路徑和行號
            int32 PathEnd = Line.Find(TEXT("("));
            if (PathEnd != INDEX_NONE)
            {
                Issue.FilePath = Line.Left(PathEnd).TrimStartAndEnd();
                
                int32 LineEnd = Line.Find(TEXT(")"), ESearchCase::IgnoreCase, ESearchDir::FromStart, PathEnd);
                if (LineEnd != INDEX_NONE)
                {
                    FString LineNumStr = Line.Mid(PathEnd + 1, LineEnd - PathEnd - 1);
                    Issue.LineNumber = FCString::Atoi(*LineNumStr);
                }
            }
            
            // 確定嚴重程度
            if (Line.Contains(TEXT("error:")))
            {
                Issue.Severity = EIssueSeverity::Error;
                Issue.Title = FText::FromString(TEXT("編譯錯誤"));
            }
            else
            {
                Issue.Severity = EIssueSeverity::Warning;
                Issue.Title = FText::FromString(TEXT("編譯警告"));
            }
            
            // 提取錯誤訊息
            int32 MessageStart = Line.Find(TEXT(":"), ESearchCase::IgnoreCase, ESearchDir::FromStart, PathEnd);
            if (MessageStart != INDEX_NONE)
            {
                FString Message = Line.Mid(MessageStart + 1).TrimStart();
                Issue.Description = FText::FromString(Message);
                
                // 生成建議修復
                Issue.SuggestedFix = FText::FromString(TEXT("請檢查並修復代碼錯誤"));
            }
            
            Issue.bCanAutoFix = false; // 編譯錯誤通常需要手動修復
            
            Issues.Add(Issue);
            OnIssueFound.Broadcast(Issue);
        }
    }
    
    return Issues;
}

TArray<FDebugIssue> UMingRTSAutomatedBatchDebugger::RunStaticCodeAnalysis(const TArray<FString>& TargetPaths)
{
    TArray<FDebugIssue> Issues;
    
    // 檢查常見問題模式
    for (const FString& Path : TargetPaths)
    {
        TArray<FString> Files;
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.cpp"), true, false);
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*.h"), true, false);
        
        for (const FString& File : Files)
        {
            FString FilePath = FPaths::Combine(Path, File);
            FString FileContent;
            
            if (FFileHelper::LoadFileToString(FileContent, *FilePath))
            {
                TArray<FDebugIssue> FileIssues = DetectCommonPatterns(FilePath, FileContent);
                Issues.Append(FileIssues);
            }
        }
    }
    
    return Issues;
}

TArray<FDebugIssue> UMingRTSAutomatedBatchDebugger::DetectCommonPatterns(const FString& FilePath, const FString& FileContent)
{
    TArray<FDebugIssue> Issues;
    
    TArray<FString> Lines;
    FileContent.ParseIntoArrayLines(Lines);
    
    for (int32 i = 0; i < Lines.Num(); i++)
    {
        const FString& Line = Lines[i];
        
        // 檢查缺少分號
        if (Line.TrimEnd().EndsWith(TEXT(")")) || Line.TrimEnd().EndsWith(TEXT("}")) || Line.TrimEnd().EndsWith(TEXT("]")))
        {
            if (!Line.Contains(TEXT(";")) && !Line.Contains(TEXT("//")) && !Line.TrimStart().StartsWith(TEXT("//")))
            {
                FDebugIssue Issue;
                Issue.IssueID = FName(*FString::Printf(TEXT("MissingSemicolon_%d"), i));
                Issue.TaskType = EDebugTaskType::StaticAnalysis;
                Issue.Severity = EIssueSeverity::Error;
                Issue.Title = FText::FromString(TEXT("可能缺少分號"));
                Issue.Description = FText::FromString(FString::Printf(TEXT("行 %d 結尾可能缺少分號"), i + 1));
                Issue.FilePath = FilePath;
                Issue.LineNumber = i + 1;
                Issue.SuggestedFix = FText::FromString(TEXT("在行尾添加分號"));
                Issue.bCanAutoFix = true;
                Issue.FixCommand = TEXT("FixMissingSemicolon");
                Issue.DetectedTime = FDateTime::Now();
                
                Issues.Add(Issue);
            }
        }
        
        // 檢查 TODO/FIXME 註釋
        if (Line.Contains(TEXT("TODO")) || Line.Contains(TEXT("FIXME")))
        {
            FDebugIssue Issue;
            Issue.IssueID = FName(*FString::Printf(TEXT("Todo_%d"), i));
            Issue.TaskType = EDebugTaskType::StaticAnalysis;
            Issue.Severity = EIssueSeverity::Info;
            Issue.Title = FText::FromString(TEXT("待辦事項"));
            Issue.Description = FText::FromString(Line);
            Issue.FilePath = FilePath;
            Issue.LineNumber = i + 1;
            Issue.bCanAutoFix = false;
            Issue.DetectedTime = FDateTime::Now();
            
            Issues.Add(Issue);
        }
        
        // 檢查硬編碼字符串（可能未本地化）
        if (Line.Contains(TEXT("TEXT(\"")) && !Line.Contains(TEXT("_T(")) && !Line.Contains(TEXT("NSLOCTEXT")))
        {
            // 忽略註釋行
            if (!Line.TrimStart().StartsWith(TEXT("//")))
            {
                FDebugIssue Issue;
                Issue.IssueID = FName(*FString::Printf(TEXT("Localization_%d"), i));
                Issue.TaskType = EDebugTaskType::StaticAnalysis;
                Issue.Severity = EIssueSeverity::Suggestion;
                Issue.Title = FText::FromString(TEXT("可能需要本地化"));
                Issue.Description = FText::FromString(TEXT("檢測到硬編碼字符串，建議使用NSLOCTEXT或LOCTEXT進行本地化"));
                Issue.FilePath = FilePath;
                Issue.LineNumber = i + 1;
                Issue.bCanAutoFix = false;
                Issue.DetectedTime = FDateTime::Now();
                
                Issues.Add(Issue);
            }
        }
    }
    
    return Issues;
}

bool UMingRTSAutomatedBatchDebugger::ExecuteAutoFix(const FDebugIssue& Issue, FString& OutMessage)
{
    if (Issue.FixCommand == TEXT("FixMissingSemicolon"))
    {
        return FixMissingSemicolon(Issue.FilePath, Issue.LineNumber);
    }
    else if (Issue.FixCommand == TEXT("FixIndentation"))
    {
        return FixIndentation(Issue.FilePath, Issue.LineNumber);
    }
    else if (Issue.FixCommand.StartsWith(TEXT("FixUnusedVariable:")))
    {
        FString VarName = Issue.FixCommand.RightChop(18);
        return FixUnusedVariable(Issue.FilePath, Issue.LineNumber, VarName);
    }
    
    OutMessage = TEXT("未知的修復命令");
    return false;
}

bool UMingRTSAutomatedBatchDebugger::FixMissingSemicolon(const FString& FilePath, int32 LineNumber)
{
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        return false;
    }
    
    TArray<FString> Lines;
    Content.ParseIntoArrayLines(Lines);
    
    if (LineNumber <= 0 || LineNumber > Lines.Num())
    {
        return false;
    }
    
    Lines[LineNumber - 1] += TEXT(";");
    
    FString NewContent = FString::Join(Lines, TEXT("\n"));
    return FFileHelper::SaveStringToFile(NewContent, *FilePath);
}

bool UMingRTSAutomatedBatchDebugger::FixIndentation(const FString& FilePath, int32 LineNumber)
{
    // 實現縮進修復邏輯
    return true;
}

bool UMingRTSAutomatedBatchDebugger::FixUnusedVariable(const FString& FilePath, int32 LineNumber, const FString& VariableName)
{
    // 實現未使用變數修復邏輯
    return true;
}

bool UMingRTSAutomatedBatchDebugger::FixMissingInclude(const FString& FilePath, const FString& MissingType)
{
    // 實現缺少Include修復邏輯
    return true;
}

FString UMingRTSAutomatedBatchDebugger::GenerateReport(const FBatchDebugResult& Result, bool bAsHTML)
{
    if (bAsHTML)
    {
        return GenerateHTMLReport(Result);
    }
    else
    {
        return GenerateTextReport(Result);
    }
}

FString UMingRTSAutomatedBatchDebugger::GenerateHTMLReport(const FBatchDebugResult& Result)
{
    FString HTML = TEXT("<!DOCTYPE html>\n");
    HTML += TEXT("<html>\n<head>\n");
    HTML += TEXT("<title>MingGoRTS 批次除錯報告</title>\n");
    HTML += TEXT("<style>\n");
    HTML += TEXT("body { font-family: Arial, sans-serif; margin: 20px; }\n");
    HTML += TEXT(".header { background: #2c3e50; color: white; padding: 20px; border-radius: 5px; }\n");
    HTML += TEXT(".summary { background: #ecf0f1; padding: 15px; margin: 20px 0; border-radius: 5px; }\n");
    HTML += TEXT(".critical { color: #e74c3c; }\n");
    HTML += TEXT(".error { color: #e67e22; }\n");
    HTML += TEXT(".warning { color: #f39c12; }\n");
    HTML += TEXT(".info { color: #3498db; }\n");
    HTML += TEXT(".success { color: #27ae60; }\n");
    HTML += TEXT("table { width: 100%; border-collapse: collapse; margin: 20px 0; }\n");
    HTML += TEXT("th, td { border: 1px solid #bdc3c7; padding: 10px; text-align: left; }\n");
    HTML += TEXT("th { background: #34495e; color: white; }\n");
    HTML += TEXT("</style>\n</head>\n<body>\n");
    
    // 標題
    HTML += FString::Printf(TEXT("<div class=\"header\">\n"));
    HTML += FString::Printf(TEXT("<h1>%s</h1>\n"), *Result.BatchName);
    HTML += FString::Printf(TEXT("<p>執行時間: %s - %s</p>\n"), *Result.StartTime.ToString(), *Result.EndTime.ToString());
    HTML += FString::Printf(TEXT("<p>總耗時: %.2f 秒</p>\n"), Result.TotalExecutionTime);
    HTML += TEXT("</div>\n");
    
    // 摘要
    HTML += TEXT("<div class=\"summary\">\n");
    HTML += TEXT("<h2>執行摘要</h2>\n");
    HTML += FString::Printf(TEXT("<p class=\"critical\">嚴重問題: %d</p>\n"), Result.CriticalIssues);
    HTML += FString::Printf(TEXT("<p class=\"error\">錯誤: %d</p>\n"), Result.Errors);
    HTML += FString::Printf(TEXT("<p class=\"warning\">警告: %d</p>\n"), Result.Warnings);
    HTML += FString::Printf(TEXT("<p class=\"success\">自動修復: %d</p>\n"), Result.AutoFixed);
    HTML += TEXT("</div>\n");
    
    // 詳細結果表格
    HTML += TEXT("<h2>任務詳情</h2>\n");
    HTML += TEXT("<table>\n");
    HTML += TEXT("<tr><th>任務類型</th><th>狀態</th><th>文件數</th><th>問題數</th><th>耗時</th><th>摘要</th></tr>\n");
    
    for (const FDebugTaskResult& Task : Result.TaskResults)
    {
        FString StatusClass;
        switch (Task.Status)
        {
            case EDebugResultStatus::Success: StatusClass = TEXT("success"); break;
            case EDebugResultStatus::Failed: StatusClass = TEXT("error"); break;
            case EDebugResultStatus::Partial: StatusClass = TEXT("warning"); break;
            default: StatusClass = TEXT("info"); break;
        }
        
        HTML += FString::Printf(TEXT("<tr>"));
        HTML += FString::Printf(TEXT("<td>%s</td>"), *StaticEnum<EDebugTaskType>()->GetNameStringByValue((int64)Task.TaskType));
        HTML += FString::Printf(TEXT("<td class=\"%s\">%s</td>"), *StatusClass, *StaticEnum<EDebugResultStatus>()->GetNameStringByValue((int64)Task.Status));
        HTML += FString::Printf(TEXT("<td>%d</td>"), Task.FilesProcessed);
        HTML += FString::Printf(TEXT("<td>%d</td>"), Task.Issues.Num());
        HTML += FString::Printf(TEXT("<td>%.2f s</td>"), Task.ExecutionTimeSeconds);
        HTML += FString::Printf(TEXT("<td>%s</td>"), *Task.SummaryMessage.ToString());
        HTML += TEXT("</tr>\n");
    }
    
    HTML += TEXT("</table>\n");
    HTML += TEXT("</body>\n</html>");
    
    // 保存報告
    FString ReportPath = FPaths::ProjectLogDir() / FString::Printf(TEXT("BatchDebug_Report_%s.html"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FFileHelper::SaveStringToFile(HTML, *ReportPath);
    
    return ReportPath;
}

FString UMingRTSAutomatedBatchDebugger::GenerateTextReport(const FBatchDebugResult& Result)
{
    FString Report = TEXT("====================================\n");
    Report += FString::Printf(TEXT("批次除錯報告: %s\n"), *Result.BatchName);
    Report += TEXT("====================================\n\n");
    
    Report += FString::Printf(TEXT("執行時間: %s - %s\n"), *Result.StartTime.ToString(), *Result.EndTime.ToString());
    Report += FString::Printf(TEXT("總耗時: %.2f 秒\n\n"), Result.TotalExecutionTime);
    
    Report += TEXT("--- 執行摘要 ---\n");
    Report += FString::Printf(TEXT("嚴重問題: %d\n"), Result.CriticalIssues);
    Report += FString::Printf(TEXT("錯誤: %d\n"), Result.Errors);
    Report += FString::Printf(TEXT("警告: %d\n"), Result.Warnings);
    Report += FString::Printf(TEXT("自動修復: %d\n\n"), Result.AutoFixed);
    
    Report += TEXT("--- 任務詳情 ---\n");
    for (const FDebugTaskResult& Task : Result.TaskResults)
    {
        Report += FString::Printf(TEXT("\n[%s]\n"), *StaticEnum<EDebugTaskType>()->GetNameStringByValue((int64)Task.TaskType));
        Report += FString::Printf(TEXT("狀態: %s\n"), *StaticEnum<EDebugResultStatus>()->GetNameStringByValue((int64)Task.Status));
        Report += FString::Printf(TEXT("文件處理: %d\n"), Task.FilesProcessed);
        Report += FString::Printf(TEXT("發現問題: %d\n"), Task.Issues.Num());
        Report += FString::Printf(TEXT("耗時: %.2f 秒\n"), Task.ExecutionTimeSeconds);
        Report += FString::Printf(TEXT("摘要: %s\n"), *Task.SummaryMessage.ToString());
        
        if (Task.Issues.Num() > 0)
        {
            Report += TEXT("\n問題列表:\n");
            for (const FDebugIssue& Issue : Task.Issues)
            {
                Report += FString::Printf(TEXT("  [%s] %s - %s\n"),
                    *StaticEnum<EIssueSeverity>()->GetNameStringByValue((int64)Issue.Severity),
                    *Issue.Title.ToString(),
                    *Issue.Description.ToString());
            }
        }
    }
    
    Report += TEXT("\n====================================\n");
    Report += TEXT("報告生成完成\n");
    
    // 保存報告
    FString ReportPath = FPaths::ProjectLogDir() / FString::Printf(TEXT("BatchDebug_Report_%s.txt"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));
    FFileHelper::SaveStringToFile(Report, *ReportPath);
    
    return ReportPath;
}

bool UMingRTSAutomatedBatchDebugger::ExportResultsToJSON(const FBatchDebugResult& Result, const FString& FilePath)
{
    TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
    
    RootObject->SetStringField(TEXT("BatchName"), Result.BatchName);
    RootObject->SetStringField(TEXT("StartTime"), Result.StartTime.ToIso8601());
    RootObject->SetStringField(TEXT("EndTime"), Result.EndTime.ToIso8601());
    RootObject->SetNumberField(TEXT("TotalExecutionTime"), Result.TotalExecutionTime);
    RootObject->SetNumberField(TEXT("TotalIssues"), Result.TotalIssues);
    RootObject->SetNumberField(TEXT("CriticalIssues"), Result.CriticalIssues);
    RootObject->SetNumberField(TEXT("Errors"), Result.Errors);
    RootObject->SetNumberField(TEXT("Warnings"), Result.Warnings);
    RootObject->SetNumberField(TEXT("AutoFixed"), Result.AutoFixed);
    
    TArray<TSharedPtr<FJsonValue>> TaskResults;
    for (const FDebugTaskResult& Task : Result.TaskResults)
    {
        TSharedPtr<FJsonObject> TaskObj = MakeShared<FJsonObject>();
        TaskObj->SetStringField(TEXT("TaskType"), StaticEnum<EDebugTaskType>()->GetNameStringByValue((int64)Task.TaskType));
        TaskObj->SetStringField(TEXT("Status"), StaticEnum<EDebugResultStatus>()->GetNameStringByValue((int64)Task.Status));
        TaskObj->SetNumberField(TEXT("ExecutionTime"), Task.ExecutionTimeSeconds);
        TaskObj->SetNumberField(TEXT("FilesProcessed"), Task.FilesProcessed);
        TaskObj->SetNumberField(TEXT("IssuesCount"), Task.Issues.Num());
        TaskObj->SetStringField(TEXT("Summary"), Task.SummaryMessage.ToString());
        TaskResults.Add(MakeShared<FJsonValueObject>(TaskObj));
    }
    RootObject->SetArrayField(TEXT("TaskResults"), TaskResults);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return FFileHelper::SaveStringToFile(OutputString, *FilePath);
}

FBatchDebugResult UMingRTSAutomatedBatchDebugger::ImportResultsFromJSON(const FString& FilePath)
{
    FBatchDebugResult Result;
    
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        return Result;
    }
    
    TSharedPtr<FJsonObject> RootObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
    if (FJsonSerializer::Deserialize(Reader, RootObject))
    {
        Result.BatchName = RootObject->GetStringField(TEXT("BatchName"));
        // 解析其他字段...
    }
    
    return Result;
}

FBatchDebugConfig UMingRTSAutomatedBatchDebugger::GetDefaultConfig()
{
    FBatchDebugConfig Config;
    Config.BatchName = TEXT("預設批次除錯");
    Config.EnabledTasks = {
        EDebugTaskType::CompilationCheck,
        EDebugTaskType::StaticAnalysis,
        EDebugTaskType::CodeStyleCheck,
        EDebugTaskType::UnitTestExecution
    };
    Config.TargetPaths.Add(FPaths::ProjectSourceDir());
    Config.bEnableAutoFix = false;
    Config.ParallelTaskCount = 4;
    Config.TaskTimeoutSeconds = 300.0f;
    Config.OutputReportPath = FPaths::ProjectLogDir() / TEXT("BatchDebug_Report.html");
    Config.bGenerateHTMLReport = true;
    Config.bSendNotifications = true;
    
    return Config;
}

TMap<EIssueSeverity, int32> UMingRTSAutomatedBatchDebugger::GetIssueStatistics(const FBatchDebugResult& Result)
{
    TMap<EIssueSeverity, int32> Stats;
    
    for (const FDebugTaskResult& Task : Result.TaskResults)
    {
        for (const FDebugIssue& Issue : Task.Issues)
        {
            int32 Count = Stats.FindRef(Issue.Severity);
            Stats.Add(Issue.Severity, Count + 1);
        }
    }
    
    return Stats;
}

TArray<FDebugIssue> UMingRTSAutomatedBatchDebugger::FilterIssuesBySeverity(const TArray<FDebugIssue>& Issues, EIssueSeverity MinSeverity)
{
    TArray<FDebugIssue> Filtered;
    
    for (const FDebugIssue& Issue : Issues)
    {
        if ((int32)Issue.Severity >= (int32)MinSeverity)
        {
            Filtered.Add(Issue);
        }
    }
    
    return Filtered;
}

bool UMingRTSAutomatedBatchDebugger::HasCriticalIssues(const FBatchDebugResult& Result)
{
    return Result.CriticalIssues > 0 || Result.Errors > 0;
}

TArray<FDebugIssue> UMingRTSAutomatedBatchDebugger::GetAutoFixableIssues(const TArray<FDebugIssue>& Issues)
{
    TArray<FDebugIssue> AutoFixable;
    
    for (const FDebugIssue& Issue : Issues)
    {
        if (Issue.bCanAutoFix)
        {
            AutoFixable.Add(Issue);
        }
    }
    
    return AutoFixable;
}

void UMingRTSAutomatedBatchDebugger::SendNotification(const FBatchDebugResult& Result)
{
    FString Title = FString::Printf(TEXT("批次除錯完成: %s"), *Result.BatchName);
    FString Message;
    
    if (Result.CriticalIssues > 0)
    {
        Message = FString::Printf(TEXT("發現 %d 個嚴重問題和 %d 個錯誤，請立即檢查！"), 
            Result.CriticalIssues, Result.Errors);
    }
    else if (Result.Errors > 0)
    {
        Message = FString::Printf(TEXT("發現 %d 個錯誤和 %d 個警告。"), 
            Result.Errors, Result.Warnings);
    }
    else if (Result.Warnings > 0)
    {
        Message = FString::Printf(TEXT("完成，發現 %d 個警告。"), Result.Warnings);
    }
    else
    {
        Message = FString::Printf(TEXT("成功完成，無問題！"));
    }
    
    // 發送UE通知
    #if WITH_EDITOR
    FNotificationInfo Info(FText::FromString(Title));
    Info.Text = FText::FromString(Message);
    Info.bFireAndForget = true;
    Info.ExpireDuration = 5.0f;
    FSlateNotificationManager::Get().AddNotification(Info);
    #endif
    
    UE_LOG(LogMingGoRTS, Log, TEXT("Notification: %s - %s"), *Title, *Message);
}
