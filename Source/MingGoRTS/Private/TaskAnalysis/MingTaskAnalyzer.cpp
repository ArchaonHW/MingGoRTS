#include "MingTaskAnalyzer.h"
#include "MingCompileAPI.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingTaskAnalyzer::UMingTaskAnalyzer()
{
    CompileAPI = nullptr;
    TaskIDCounter = 1;
}

void UMingTaskAnalyzer::InitializeTaskAnalyzer()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing MingTaskAnalyzer..."));

    // 創建編譯 API 實例
    CompileAPI = NewObject<UMingCompileAPI>();
    CompileAPI->InitializeCompileAPI();

    // 初始化分析規則
    ErrorPatterns = {
        TEXT("error"),
        TEXT("Error"),
        TEXT("ERROR"),
        TEXT("fatal error"),
        TEXT("Fatal Error"),
        TEXT("FATAL ERROR"),
        TEXT("cannot open"),
        TEXT("undefined reference"),
        TEXT("unresolved external"),
        TEXT("syntax error"),
        TEXT("compilation failed")
    };

    WarningPatterns = {
        TEXT("warning"),
        TEXT("Warning"),
        TEXT("WARNING"),
        TEXT("deprecated"),
        TEXT("unused"),
        TEXT("unreferenced"),
        TEXT("potentially uninitialized"),
        TEXT("implicit conversion"),
        TEXT("narrowing conversion"),
        TEXT("signed/unsigned mismatch")
    };

    PerformancePatterns = {
        TEXT("performance"),
        TEXT("slow"),
        TEXT("bottleneck"),
        TEXT("inefficient"),
        TEXT("memory leak"),
        TEXT("memory usage"),
        TEXT("cpu usage"),
        TEXT("optimization"),
        TEXT("cache miss"),
        TEXT("frame drop")
    };

    SecurityPatterns = {
        TEXT("security"),
        TEXT("vulnerability"),
        TEXT("buffer overflow"),
        TEXT("stack overflow"),
        TEXT("memory corruption"),
        TEXT("access violation"),
        TEXT("privilege escalation"),
        TEXT("injection"),
        TEXT("cross-site scripting"),
        TEXT("SQL injection")
    };

    UE_LOG(LogTemp, Log, TEXT("MingTaskAnalyzer initialized successfully"));
}

FMingCompileResult UMingTaskAnalyzer::CompileProjectAndGenerateLog(const FMingCompileConfiguration& Config)
{
    UE_LOG(LogTemp, Log, TEXT("Starting project compilation for task analysis..."));

    if (!CompileAPI)
    {
        InitializeTaskAnalyzer();
    }

    // 配置編譯參數以獲取詳細日誌
    FMingCompileConfiguration DetailedConfig = Config;
    DetailedConfig.AdditionalArguments.Add(TEXT("-logcommands"));
    DetailedConfig.AdditionalArguments.Add(TEXT("-log"));
    DetailedConfig.AdditionalArguments.Add(TEXT("-verbose"));
    DetailedConfig.AdditionalArguments.Add(TEXT("-showincludes"));

    // 執行編譯
    FMingCompileResult Result = CompileAPI->CompileProject(DetailedConfig);

    // 保存詳細日誌到文件
    FString LogPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Logs/CompileLog_TaskAnalysis.txt"));
    if (!Result.OutputLog.IsEmpty())
    {
        FFileHelper::SaveStringToFile(Result.OutputLog, *LogPath);
        UE_LOG(LogTemp, Log, TEXT("Compile log saved to: %s"), *LogPath);
    }

    return Result;
}

FMingLogAnalysisResult UMingTaskAnalyzer::AnalyzeCompileLog(const FString& LogPath)
{
    UE_LOG(LogTemp, Log, TEXT("Analyzing compile log: %s"), *LogPath);

    // 觸發分析開始事件
    OnLogAnalysisStarted.Broadcast(LogPath);

    FMingLogAnalysisResult Result;
    Result.AnalysisTime = FDateTime::Now();

    // 讀取日誌文件
    FString LogContent;
    if (!FFileHelper::LoadFileToString(LogContent, *LogPath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load log file: %s"), *LogPath);
        return Result;
    }

    // 分析日誌內容
    Result = AnalyzeLogFromText(LogContent);

    // 觸發分析完成事件
    OnLogAnalysisCompleted.Broadcast(Result);

    UE_LOG(LogTemp, Log, TEXT("Log analysis completed. Found %d errors, %d warnings"), 
        Result.TotalErrors, Result.TotalWarnings);

    return Result;
}

FMingLogAnalysisResult UMingTaskAnalyzer::AnalyzeLogFromText(const FString& LogText)
{
    FMingLogAnalysisResult Result;
    Result.AnalysisTime = FDateTime::Now();

    // 將日誌按行分割
    TArray<FString> LogLines;
    LogText.ParseIntoArrayLines(LogLines);

    float TotalLines = LogLines.Num();
    float ProcessedLines = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("Processing %d log lines..."), LogLines.Num());

    for (int32 i = 0; i < LogLines.Num(); i++)
    {
        const FString& Line = LogLines[i];
        ProcessedLines++;

        // 更新進度
        float Progress = ProcessedLines / TotalLines;
        if (i % 100 == 0) // 每100行更新一次進度
        {
            OnLogAnalysisProgress.Broadcast(Progress, FString::Printf(TEXT("Processing line %d/%d"), i, LogLines.Num()));
        }

        // 分析每一行
        FMingTaskRequirement Task;
        bool bTaskCreated = false;

        // 檢查錯誤
        if (IsErrorLine(Line))
        {
            Task = ParseCompileError(Line);
            Task.TaskType = EMingTaskType::Debug;
            bTaskCreated = true;
            Result.TotalErrors++;
        }
        // 檢查警告
        else if (IsWarningLine(Line))
        {
            Task = ParseCompileWarning(Line);
            Task.TaskType = EMingTaskType::Debug;
            bTaskCreated = true;
            Result.TotalWarnings++;
        }
        // 檢查性能問題
        else if (IsPerformanceLine(Line))
        {
            Task = ParsePerformanceIssue(Line);
            Task.TaskType = EMingTaskType::Performance;
            bTaskCreated = true;
        }
        // 檢查安全問題
        else if (IsSecurityLine(Line))
        {
            Task = ParseSecurityIssue(Line);
            Task.TaskType = EMingTaskType::Security;
            bTaskCreated = true;
        }

        if (bTaskCreated)
        {
            // 設定優先級
            Task.Priority = DetermineTaskPriority(Task);
            
            // 估算時間
            Task.EstimatedHours = EstimateTaskHours(Task);
            
            // 生成建議修復方案
            Task.SuggestedFix = GenerateSuggestedFix(Task);
            
            // 添加到對應的任務列表
            switch (Task.TaskType)
            {
                case EMingTaskType::Debug:
                    Result.DebugTasks.Add(Task);
                    break;
                case EMingTaskType::Optimization:
                    Result.OptimizationTasks.Add(Task);
                    break;
                case EMingTaskType::Performance:
                    Result.PerformanceTasks.Add(Task);
                    break;
                case EMingTaskType::Security:
                    Result.SecurityTasks.Add(Task);
                    break;
                case EMingTaskType::Feature:
                    Result.FeatureTasks.Add(Task);
                    break;
            }
            
            Result.TotalMessages++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Log analysis completed. Total tasks created: %d"), Result.TotalMessages);
    return Result;
}

bool UMingTaskAnalyzer::GenerateTaskReport(const FMingLogAnalysisResult& AnalysisResult, const FString& ReportPath)
{
    UE_LOG(LogTemp, Log, TEXT("Generating task report: %s"), *ReportPath);

    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS 任務分析報告</title>\n");
    ReportContent += TEXT("<meta charset=\"UTF-8\">\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: 'Microsoft JhengHei', Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); color: white; padding: 30px; border-radius: 10px; margin-bottom: 30px; text-align: center; }\n");
    ReportContent += TEXT(".summary { background: white; padding: 25px; border-radius: 10px; margin-bottom: 25px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".task-section { background: white; padding: 25px; border-radius: 10px; margin-bottom: 25px; box-shadow: 0 4px 6px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".critical { border-left: 5px solid #e74c3c; }\n");
    ReportContent += TEXT(".high { border-left: 5px solid #f39c12; }\n");
    ReportContent += TEXT(".medium { border-left: 5px solid #3498db; }\n");
    ReportContent += TEXT(".low { border-left: 5px solid #95a5a6; }\n");
    ReportContent += TEXT(".priority-critical { color: #e74c3c; font-weight: bold; }\n");
    ReportContent += TEXT(".priority-high { color: #f39c12; font-weight: bold; }\n");
    ReportContent += TEXT(".priority-medium { color: #3498db; }\n");
    ReportContent += TEXT(".priority-low { color: #95a5a6; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 15px; }\n");
    ReportContent += TEXT("th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; font-weight: bold; }\n");
    ReportContent += TEXT("tr:hover { background-color: #f8f9fa; }\n");
    ReportContent += TEXT(".stats { display: flex; justify-content: space-around; margin: 20px 0; }\n");
    ReportContent += TEXT(".stat-box { text-align: center; padding: 15px; background: #f8f9fa; border-radius: 8px; min-width: 120px; }\n");
    ReportContent += TEXT(".stat-number { font-size: 2em; font-weight: bold; color: #2c3e50; }\n");
    ReportContent += TEXT(".stat-label { color: #7f8c8d; margin-top: 5px; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    // 頁眉
    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>MingGoRTS 任務分析報告</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>生成時間: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += TEXT("</div>\n");

    // 統計摘要
    ReportContent += TEXT("<div class=\"summary\">\n");
    ReportContent += TEXT("<h2>📊 統計摘要</h2>\n");
    ReportContent += TEXT("<div class=\"stats\">\n");
    ReportContent += FString::Printf(TEXT("<div class=\"stat-box\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">總任務數</div></div>\n"), AnalysisResult.TotalMessages);
    ReportContent += FString::Printf(TEXT("<div class=\"stat-box\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">錯誤數</div></div>\n"), AnalysisResult.TotalErrors);
    ReportContent += FString::Printf(TEXT("<div class=\"stat-box\"><div class=\"stat-number\">%d</div><div class=\"stat-label\">警告數</div></div>\n"), AnalysisResult.TotalWarnings);
    ReportContent += FString::Printf(TEXT("<div class=\"stat-box\"><div class=\"stat-number\">%.1f</div><div class=\"stat-label\">預估工時</div></div>\n"), CalculateTotalEstimatedHours(AnalysisResult));
    ReportContent += TEXT("</div>\n");
    ReportContent += TEXT("</div>\n");

    // 關鍵任務
    TArray<FMingTaskRequirement> CriticalTasks = GetCriticalTasks(AnalysisResult);
    if (CriticalTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"task-section critical\">\n");
        ReportContent += TEXT("<h2>🚨 關鍵任務 (立即處理)</h2>\n");
        ReportContent += GenerateTaskTable(CriticalTasks);
        ReportContent += TEXT("</div>\n");
    }

    // 除錯任務
    if (AnalysisResult.DebugTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"task-section\">\n");
        ReportContent += FString::Printf(TEXT("<h2>🐛 除錯任務 (%d 個)</h2>\n"), AnalysisResult.DebugTasks.Num());
        ReportContent += GenerateTaskTable(AnalysisResult.DebugTasks);
        ReportContent += TEXT("</div>\n");
    }

    // 優化任務
    if (AnalysisResult.OptimizationTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"task-section\">\n");
        ReportContent += FString::Printf(TEXT("<h2>⚡ 優化任務 (%d 個)</h2>\n"), AnalysisResult.OptimizationTasks.Num());
        ReportContent += GenerateTaskTable(AnalysisResult.OptimizationTasks);
        ReportContent += TEXT("</div>\n");
    }

    // 性能任務
    if (AnalysisResult.PerformanceTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"task-section\">\n");
        ReportContent += FString::Printf(TEXT("<h2>🚀 性能任務 (%d 個)</h2>\n"), AnalysisResult.PerformanceTasks.Num());
        ReportContent += GenerateTaskTable(AnalysisResult.PerformanceTasks);
        ReportContent += TEXT("</div>\n");
    }

    // 安全任務
    if (AnalysisResult.SecurityTasks.Num() > 0)
    {
        ReportContent += TEXT("<div class=\"task-section\">\n");
        ReportContent += FString::Printf(TEXT("<h2>🔒 安全任務 (%d 個)</h2>\n"), AnalysisResult.SecurityTasks.Num());
        ReportContent += GenerateTaskTable(AnalysisResult.SecurityTasks);
        ReportContent += TEXT("</div>\n");
    }

    ReportContent += TEXT("</body>\n</html>");

    // 確保報告目錄存在
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!IFileManager::Get().DirectoryExists(*ReportDir))
    {
        IFileManager::Get().MakeDirectory(*ReportDir);
    }

    // 保存報告
    bool bSuccess = FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("Task report generated successfully: %s"), *ReportPath);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to save task report: %s"), *ReportPath);
    }

    return bSuccess;
}

TArray<FMingTaskRequirement> UMingTaskAnalyzer::SortTasksByPriority(const TArray<FMingTaskRequirement>& Tasks)
{
    TArray<FMingTaskRequirement> SortedTasks = Tasks;
    
    SortedTasks.Sort([](const FMingTaskRequirement& A, const FMingTaskRequirement& B)
    {
        // 優先級排序: Critical > High > Medium > Low > Deferred
        if (A.Priority != B.Priority)
        {
            return static_cast<int32>(A.Priority) < static_cast<int32>(B.Priority);
        }
        
        // 相同優先級按創建時間排序
        return A.CreatedTime < B.CreatedTime;
    });
    
    return SortedTasks;
}

TArray<FMingTaskRequirement> UMingTaskAnalyzer::GetCriticalTasks(const FMingLogAnalysisResult& AnalysisResult)
{
    TArray<FMingTaskRequirement> CriticalTasks;
    
    // 收集所有關鍵任務
    CriticalTasks.Append(GetTasksByPriority(AnalysisResult.DebugTasks, EMingTaskPriority::Critical));
    CriticalTasks.Append(GetTasksByPriority(AnalysisResult.OptimizationTasks, EMingTaskPriority::Critical));
    CriticalTasks.Append(GetTasksByPriority(AnalysisResult.PerformanceTasks, EMingTaskPriority::Critical));
    CriticalTasks.Append(GetTasksByPriority(AnalysisResult.SecurityTasks, EMingTaskPriority::Critical));
    CriticalTasks.Append(GetTasksByPriority(AnalysisResult.FeatureTasks, EMingTaskPriority::Critical));
    
    return SortTasksByPriority(CriticalTasks);
}

TArray<FMingTaskRequirement> UMingTaskAnalyzer::GetHighPriorityTasks(const FMingLogAnalysisResult& AnalysisResult)
{
    TArray<FMingTaskRequirement> HighPriorityTasks;
    
    // 收集所有高優先級任務
    HighPriorityTasks.Append(GetTasksByPriority(AnalysisResult.DebugTasks, EMingTaskPriority::High));
    HighPriorityTasks.Append(GetTasksByPriority(AnalysisResult.OptimizationTasks, EMingTaskPriority::High));
    HighPriorityTasks.Append(GetTasksByPriority(AnalysisResult.PerformanceTasks, EMingTaskPriority::High));
    HighPriorityTasks.Append(GetTasksByPriority(AnalysisResult.SecurityTasks, EMingTaskPriority::High));
    HighPriorityTasks.Append(GetTasksByPriority(AnalysisResult.FeatureTasks, EMingTaskPriority::High));
    
    return SortTasksByPriority(HighPriorityTasks);
}

TMap<EMingTaskType, int32> UMingTaskAnalyzer::CalculateTaskStatistics(const FMingLogAnalysisResult& AnalysisResult)
{
    TMap<EMingTaskType, int32> Statistics;
    
    Statistics.Add(EMingTaskType::Debug, AnalysisResult.DebugTasks.Num());
    Statistics.Add(EMingTaskType::Optimization, AnalysisResult.OptimizationTasks.Num());
    Statistics.Add(EMingTaskType::Performance, AnalysisResult.PerformanceTasks.Num());
    Statistics.Add(EMingTaskType::Security, AnalysisResult.SecurityTasks.Num());
    Statistics.Add(EMingTaskType::Feature, AnalysisResult.FeatureTasks.Num());
    
    return Statistics;
}

FString UMingTaskAnalyzer::ExportTasksToJSON(const FMingLogAnalysisResult& AnalysisResult)
{
    TSharedPtr<FJsonObject> RootObject = MakeShareable(new FJsonObject);
    
    // 添加統計信息
    RootObject->SetNumberField(TEXT("TotalTasks"), AnalysisResult.TotalMessages);
    RootObject->SetNumberField(TEXT("TotalErrors"), AnalysisResult.TotalErrors);
    RootObject->SetNumberField(TEXT("TotalWarnings"), AnalysisResult.TotalWarnings);
    RootObject->SetStringField(TEXT("AnalysisTime"), AnalysisResult.AnalysisTime.ToString());
    
    // 添加任務數組
    RootObject->SetArrayField(TEXT("DebugTasks"), TaskArrayToJSONArray(AnalysisResult.DebugTasks));
    RootObject->SetArrayField(TEXT("OptimizationTasks"), TaskArrayToJSONArray(AnalysisResult.OptimizationTasks));
    RootObject->SetArrayField(TEXT("PerformanceTasks"), TaskArrayToJSONArray(AnalysisResult.PerformanceTasks));
    RootObject->SetArrayField(TEXT("SecurityTasks"), TaskArrayToJSONArray(AnalysisResult.SecurityTasks));
    RootObject->SetArrayField(TEXT("FeatureTasks"), TaskArrayToJSONArray(AnalysisResult.FeatureTasks));
    
    // 序列化為字符串
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer);
    
    return OutputString;
}

FString UMingTaskAnalyzer::ExportTasksToCSV(const FMingLogAnalysisResult& AnalysisResult)
{
    FString CSVContent = TEXT("TaskID,Title,Type,Priority,Status,SourceFile,LineNumber,Description,EstimatedHours,CreatedTime\n");
    
    // 輸出所有任務
    TArray<FMingTaskRequirement> AllTasks;
    AllTasks.Append(AnalysisResult.DebugTasks);
    AllTasks.Append(AnalysisResult.OptimizationTasks);
    AllTasks.Append(AnalysisResult.PerformanceTasks);
    AllTasks.Append(AnalysisResult.SecurityTasks);
    AllTasks.Append(AnalysisResult.FeatureTasks);
    
    for (const FMingTaskRequirement& Task : AllTasks)
    {
        CSVContent += FString::Printf(TEXT("%s,%s,%s,%s,%s,%s,%d,\"%s\",%.1f,%s\n"),
            *Task.TaskID,
            *Task.Title,
            *TaskTypeToString(Task.TaskType),
            *TaskPriorityToString(Task.Priority),
            *TaskStatusToString(Task.Status),
            *Task.SourceFile,
            Task.LineNumber,
            *Task.Description.ReplaceCharWithEscapedChar(),
            Task.EstimatedHours,
            *Task.CreatedTime.ToString()
        );
    }
    
    return CSVContent;
}

// 私有輔助函數實現

bool UMingTaskAnalyzer::IsErrorLine(const FString& Line)
{
    for (const FString& Pattern : ErrorPatterns)
    {
        if (Line.Contains(Pattern))
        {
            return true;
        }
    }
    return false;
}

bool UMingTaskAnalyzer::IsWarningLine(const FString& Line)
{
    for (const FString& Pattern : WarningPatterns)
    {
        if (Line.Contains(Pattern))
        {
            return true;
        }
    }
    return false;
}

bool UMingTaskAnalyzer::IsPerformanceLine(const FString& Line)
{
    for (const FString& Pattern : PerformancePatterns)
    {
        if (Line.Contains(Pattern))
        {
            return true;
        }
    }
    return false;
}

bool UMingTaskAnalyzer::IsSecurityLine(const FString& Line)
{
    for (const FString& Pattern : SecurityPatterns)
    {
        if (Line.Contains(Pattern))
        {
            return true;
        }
    }
    return false;
}

FMingTaskRequirement UMingTaskAnalyzer::ParseCompileError(const FString& ErrorLine)
{
    FMingTaskRequirement Task;
    Task.TaskID = FString::Printf(TEXT("ERR_%04d"), TaskIDCounter++);
    Task.Title = TEXT("編譯錯誤");
    Task.Description = ErrorLine;
    Task.LogMessage = ErrorLine;
    Task.ErrorMessage = ErrorLine;
    Task.Status = EMingTaskStatus::New;
    
    // 解析文件名和行號
    ParseFileAndLine(ErrorLine, Task.SourceFile, Task.LineNumber);
    
    return Task;
}

FMingTaskRequirement UMingTaskAnalyzer::ParseCompileWarning(const FString& WarningLine)
{
    FMingTaskRequirement Task;
    Task.TaskID = FString::Printf(TEXT("WARN_%04d"), TaskIDCounter++);
    Task.Title = TEXT("編譯警告");
    Task.Description = WarningLine;
    Task.LogMessage = WarningLine;
    Task.Status = EMingTaskStatus::New;
    
    // 解析文件名和行號
    ParseFileAndLine(WarningLine, Task.SourceFile, Task.LineNumber);
    
    return Task;
}

FMingTaskRequirement UMingTaskAnalyzer::ParsePerformanceIssue(const FString& PerformanceLine)
{
    FMingTaskRequirement Task;
    Task.TaskID = FString::Printf(TEXT("PERF_%04d"), TaskIDCounter++);
    Task.Title = TEXT("性能問題");
    Task.Description = PerformanceLine;
    Task.LogMessage = PerformanceLine;
    Task.Status = EMingTaskStatus::New;
    
    return Task;
}

FMingTaskRequirement UMingTaskAnalyzer::ParseSecurityIssue(const FString& SecurityLine)
{
    FMingTaskRequirement Task;
    Task.TaskID = FString::Printf(TEXT("SEC_%04d"), TaskIDCounter++);
    Task.Title = TEXT("安全問題");
    Task.Description = SecurityLine;
    Task.LogMessage = SecurityLine;
    Task.Status = EMingTaskStatus::New;
    
    return Task;
}

EMingTaskPriority UMingTaskAnalyzer::DetermineTaskPriority(const FMingTaskRequirement& Task)
{
    // 根據任務類型和內容確定優先級
    if (Task.TaskType == EMingTaskType::Security)
    {
        return EMingTaskPriority::Critical;
    }
    
    if (Task.TaskType == EMingTaskType::Debug && Task.LogMessage.Contains(TEXT("error")))
    {
        return EMingTaskPriority::Critical;
    }
    
    if (Task.TaskType == EMingTaskType::Performance)
    {
        return EMingTaskPriority::High;
    }
    
    if (Task.TaskType == EMingTaskType::Debug && Task.LogMessage.Contains(TEXT("warning")))
    {
        return EMingTaskPriority::Medium;
    }
    
    return EMingTaskPriority::Medium;
}

float UMingTaskAnalyzer::EstimateTaskHours(const FMingTaskRequirement& Task)
{
    // 根據任務類型和複雜度估算時間
    switch (Task.TaskType)
    {
        case EMingTaskType::Debug:
            return Task.LogMessage.Contains(TEXT("error")) ? 2.0f : 1.0f;
        case EMingTaskType::Performance:
            return 4.0f;
        case EMingTaskType::Security:
            return 8.0f;
        case EMingTaskType::Optimization:
            return 3.0f;
        case EMingTaskType::Feature:
            return 16.0f;
        default:
            return 2.0f;
    }
}

FString UMingTaskAnalyzer::GenerateSuggestedFix(const FMingTaskRequirement& Task)
{
    // 根據錯誤類型生成建議修復方案
    if (Task.LogMessage.Contains(TEXT("undefined reference")))
    {
        return TEXT("檢查函數定義和聲明，確保所有必要的庫已鏈接");
    }
    
    if (Task.LogMessage.Contains(TEXT("syntax error")))
    {
        return TEXT("檢查語法錯誤，特別是括號、分號和關鍵字");
    }
    
    if (Task.LogMessage.Contains(TEXT("cannot open")))
    {
        return TEXT("檢查文件路徑和文件是否存在");
    }
    
    if (Task.LogMessage.Contains(TEXT("warning")))
    {
        return TEXT("檢查警告信息並修復潛在問題");
    }
    
    return TEXT("需要進一步分析以確定修復方案");
}

void UMingTaskAnalyzer::ParseFileAndLine(const FString& LogLine, FString& OutFile, int32& OutLine)
{
    // 簡單的文件和行號解析 (可以根據實際日誌格式調整)
    TArray<FString> Parts;
    LogLine.ParseIntoArray(Parts, TEXT(" "));
    
    for (const FString& Part : Parts)
    {
        if (Part.Contains(TEXT("(")) && Part.Contains(TEXT(")")))
        {
            FString FilePart = Part.Left(Part.Find(TEXT("(")));
            FString LinePart = Part.Mid(Part.Find(TEXT("(")) + 1);
            LinePart = LinePart.Left(LinePart.Find(TEXT(")")));
            
            OutFile = FilePart;
            OutLine = FCString::Atoi(*LinePart);
            break;
        }
    }
}

FString UMingTaskAnalyzer::GenerateTaskTable(const TArray<FMingTaskRequirement>& Tasks)
{
    FString TableContent = TEXT("<table>\n");
    TableContent += TEXT("<thead><tr><th>任務ID</th><th>標題</th><th>優先級</th><th>狀態</th><th>文件</th><th>行號</th><th>預估工時</th><th>描述</th></tr></thead>\n");
    TableContent += TEXT("<tbody>\n");
    
    for (const FMingTaskRequirement& Task : Tasks)
    {
        FString PriorityClass = FString::Printf(TEXT("priority-%s"), *TaskPriorityToString(Task.Priority).ToLower());
        TableContent += FString::Printf(TEXT("<tr><td>%s</td><td>%s</td><td class=\"%s\">%s</td><td>%s</td><td>%s</td><td>%d</td><td>%.1fh</td><td>%s</td></tr>\n"),
            *Task.TaskID,
            *Task.Title,
            *PriorityClass,
            *TaskPriorityToString(Task.Priority),
            *TaskStatusToString(Task.Status),
            *Task.SourceFile,
            Task.LineNumber,
            Task.EstimatedHours,
            *Task.Description.Left(100) // 限制描述長度
        );
    }
    
    TableContent += TEXT("</tbody>\n</table>\n");
    return TableContent;
}

float UMingTaskAnalyzer::CalculateTotalEstimatedHours(const FMingLogAnalysisResult& AnalysisResult)
{
    float TotalHours = 0.0f;
    
    for (const FMingTaskRequirement& Task : AnalysisResult.DebugTasks)
        TotalHours += Task.EstimatedHours;
    
    for (const FMingTaskRequirement& Task : AnalysisResult.OptimizationTasks)
        TotalHours += Task.EstimatedHours;
    
    for (const FMingTaskRequirement& Task : AnalysisResult.PerformanceTasks)
        TotalHours += Task.EstimatedHours;
    
    for (const FMingTaskRequirement& Task : AnalysisResult.SecurityTasks)
        TotalHours += Task.EstimatedHours;
    
    for (const FMingTaskRequirement& Task : AnalysisResult.FeatureTasks)
        TotalHours += Task.EstimatedHours;
    
    return TotalHours;
}

// 字符串轉換輔助函數
FString UMingTaskAnalyzer::TaskTypeToString(EMingTaskType Type)
{
    switch (Type)
    {
        case EMingTaskType::Debug: return TEXT("除錯");
        case EMingTaskType::Optimization: return TEXT("優化");
        case EMingTaskType::Performance: return TEXT("性能");
        case EMingTaskType::Security: return TEXT("安全");
        case EMingTaskType::Feature: return TEXT("功能");
        default: return TEXT("未知");
    }
}

FString UMingTaskAnalyzer::TaskPriorityToString(EMingTaskPriority Priority)
{
    switch (Priority)
    {
        case EMingTaskPriority::Critical: return TEXT("關鍵");
        case EMingTaskPriority::High: return TEXT("高");
        case EMingTaskPriority::Medium: return TEXT("中");
        case EMingTaskPriority::Low: return TEXT("低");
        case EMingTaskPriority::Deferred: return TEXT("延遲");
        default: return TEXT("未知");
    }
}

FString UMingTaskAnalyzer::TaskStatusToString(EMingTaskStatus Status)
{
    switch (Status)
    {
        case EMingTaskStatus::New: return TEXT("新建");
        case EMingTaskStatus::InProgress: return TEXT("進行中");
        case EMingTaskStatus::Completed: return TEXT("已完成");
        case EMingTaskStatus::Blocked: return TEXT("阻塞");
        case EMingTaskStatus::Deferred: return TEXT("延遲");
        default: return TEXT("未知");
    }
}

TArray<FMingTaskRequirement> UMingTaskAnalyzer::GetTasksByPriority(const TArray<FMingTaskRequirement>& Tasks, EMingTaskPriority Priority)
{
    TArray<FMingTaskRequirement> FilteredTasks;
    
    for (const FMingTaskRequirement& Task : Tasks)
    {
        if (Task.Priority == Priority)
        {
            FilteredTasks.Add(Task);
        }
    }
    
    return FilteredTasks;
}

TArray<TSharedPtr<FJsonValue>> UMingTaskAnalyzer::TaskArrayToJSONArray(const TArray<FMingTaskRequirement>& Tasks)
{
    TArray<TSharedPtr<FJsonValue>> JsonArray;
    
    for (const FMingTaskRequirement& Task : Tasks)
    {
        TSharedPtr<FJsonObject> TaskObject = MakeShareable(new FJsonObject);
        TaskObject->SetStringField(TEXT("TaskID"), Task.TaskID);
        TaskObject->SetStringField(TEXT("Title"), Task.Title);
        TaskObject->SetStringField(TEXT("Description"), Task.Description);
        TaskObject->SetNumberField(TEXT("TaskType"), static_cast<int32>(Task.TaskType));
        TaskObject->SetNumberField(TEXT("Priority"), static_cast<int32>(Task.Priority));
        TaskObject->SetNumberField(TEXT("Status"), static_cast<int32>(Task.Status));
        TaskObject->SetStringField(TEXT("SourceFile"), Task.SourceFile);
        TaskObject->SetNumberField(TEXT("LineNumber"), Task.LineNumber);
        TaskObject->SetStringField(TEXT("LogMessage"), Task.LogMessage);
        TaskObject->SetStringField(TEXT("ErrorMessage"), Task.ErrorMessage);
        TaskObject->SetStringField(TEXT("SuggestedFix"), Task.SuggestedFix);
        TaskObject->SetNumberField(TEXT("EstimatedHours"), Task.EstimatedHours);
        TaskObject->SetStringField(TEXT("CreatedTime"), Task.CreatedTime.ToString());
        TaskObject->SetStringField(TEXT("DueTime"), Task.DueTime.ToString());
        
        JsonArray.Add(MakeShareable(new FJsonValueObject(TaskObject)));
    }
    
    return JsonArray;
}
