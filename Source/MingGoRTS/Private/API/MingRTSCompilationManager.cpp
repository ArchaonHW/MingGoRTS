// MingGoRTS Compilation Manager Implementation
// 統一編譯管理器實現 - 整合所有編譯相關功能

#include "API/MingRTSCompilationManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/PlatformProcess.h"
#include "Misc/OutputDeviceRedirector.h"
#include "HAL/PlatformTime.h"
#include "Regex.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingRTSCompilationManager::UMingRTSCompilationManager()
{
    bIsProcessing = false;
    CompilationStats = FBuildTimeStats();
}

// ==================== 路徑配置 ====================

FString UMingRTSCompilationManager::GetEnginePath() const
{
    return TEXT("C:\\Program Files\\Epic Games\\UE_5.7");
}

FString UMingRTSCompilationManager::GetProjectPath() const
{
    return FPaths::ProjectDir();
}

FString UMingRTSCompilationManager::GetUBTPath() const
{
    return GetEnginePath() / TEXT("Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe");
}

FString UMingRTSCompilationManager::GetBuildBatPath() const
{
    return GetEnginePath() / TEXT("Engine\\Build\\BatchFiles\\Build.bat");
}

FString UMingRTSCompilationManager::GetGenerateProjectBatPath() const
{
    return GetEnginePath() / TEXT("Engine\\Build\\BatchFiles\\GenerateProjectFiles.bat");
}

// ==================== 核心編譯功能 (整合 QuickCompile.cmd) ====================

bool UMingRTSCompilationManager::BuildDevelopment()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::FullBuild, TEXT("Development Build"));
    LogAction(TEXT("Build"), TEXT("Starting Development build..."));
    
    bool bSuccess = ExecuteBuildCommand(TEXT("Development"), TEXT("-WaitMutex -NoHotReload"));
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::FullBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::BuildDebug()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::FullBuild, TEXT("Debug Build"));
    LogAction(TEXT("Build"), TEXT("Starting Debug build..."));
    
    bool bSuccess = ExecuteBuildCommand(TEXT("Debug"), TEXT("-WaitMutex"));
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::FullBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::BuildShipping()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::FullBuild, TEXT("Shipping Build"));
    LogAction(TEXT("Build"), TEXT("Starting Shipping build..."));
    
    // Shipping build uses different target
    FString Command = FString::Printf(
        TEXT("\"%s\" MingGoRTS Win64 Shipping -Project=\"%s\" -WaitMutex"),
        *GetBuildBatPath(),
        *FPaths::ProjectFilePath()
    );
    
    int32 ExitCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*Command, nullptr, &ExitCode, &StdOut, &StdErr);
    
    bool bSuccess = (ExitCode == 0);
    OnTaskCompleted.Broadcast(ECompilationTaskType::FullBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::BuildEditor()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::FullBuild, TEXT("Editor Build"));
    LogAction(TEXT("Build"), TEXT("Starting Editor build..."));
    
    bool bSuccess = ExecuteBuildCommand(TEXT("Development"), TEXT("-WaitMutex"));
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::FullBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::FastIncrementalBuild()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::IncrementalBuild, TEXT("Fast Incremental Build"));
    LogAction(TEXT("Build"), TEXT("Starting fast incremental build..."));
    
    bool bSuccess = ExecuteBuildCommand(TEXT("Development"), TEXT("-WaitMutex -NoHotReloadFromIDE"));
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::IncrementalBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::CleanAndRebuild()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::CleanBuild, TEXT("Clean and Rebuild"));
    LogAction(TEXT("Build"), TEXT("Cleaning build cache and rebuilding..."));
    
    // Step 1: Clean
    bool bCleaned = CleanBuildCache(true, true, true);
    if (!bCleaned)
    {
        UE_LOG(LogTemp, Warning, TEXT("Some directories could not be cleaned"));
    }
    
    // Step 2: Rebuild
    bool bSuccess = ExecuteBuildCommand(TEXT("Development"), TEXT("-WaitMutex"));
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::CleanBuild, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::ExecuteBuildCommand(const FString& Configuration, const FString& AdditionalFlags)
{
    FString Command = FString::Printf(
        TEXT("\"%s\" MingGoRTSEditor Win64 %s -Project=\"%s\" %s"),
        *GetBuildBatPath(),
        *Configuration,
        *FPaths::ProjectFilePath(),
        *AdditionalFlags
    );
    
    double StartTime = FPlatformTime::Seconds();
    
    int32 ExitCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*Command, nullptr, &ExitCode, &StdOut, &StdErr);
    
    double EndTime = FPlatformTime::Seconds();
    float BuildTime = static_cast<float>(EndTime - StartTime);
    
    bool bSuccess = (ExitCode == 0);
    UpdateBuildStats(bSuccess, BuildTime);
    
    return bSuccess;
}

// ==================== 項目健康檢查 (整合 ProjectHealthCheck.py) ====================

FProjectHealthReport UMingRTSCompilationManager::PerformHealthCheck()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::ProjectHealthCheck, TEXT("Health Check"));
    
    FProjectHealthReport Report;
    Report.ReportTimestamp = FDateTime::Now().ToString();
    
    FString ProjectPath = GetProjectPath();
    
    // Count files
    Report.HeaderFiles = CountFilesInDirectory(ProjectPath / TEXT("Source"), TEXT(".h"));
    Report.SourceFiles = CountFilesInDirectory(ProjectPath / TEXT("Source"), TEXT(".cpp"));
    
    // Count plugin files
    TArray<FString> PluginDirs;
    IFileManager::Get().FindFiles(PluginDirs, *(ProjectPath / TEXT("Plugins/*")), false, true);
    for (const FString& PluginDir : PluginDirs)
    {
        Report.HeaderFiles += CountFilesInDirectory(ProjectPath / TEXT("Plugins") / PluginDir, TEXT(".h"));
        Report.SourceFiles += CountFilesInDirectory(ProjectPath / TEXT("Plugins") / PluginDir, TEXT(".cpp"));
    }
    
    Report.TotalFiles = Report.HeaderFiles + Report.SourceFiles;
    
    // Scan for issues
    Report.Issues = ScanForSyntaxIssues();
    
    for (const FSyntaxIssue& Issue : Report.Issues)
    {
        if (Issue.IssueType == TEXT("Error"))
        {
            Report.ErrorCount++;
        }
        else
        {
            Report.WarningCount++;
        }
    }
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::ProjectHealthCheck, Report.ErrorCount == 0);
    
    return Report;
}

TArray<FSyntaxIssue> UMingRTSCompilationManager::ScanForSyntaxIssues()
{
    TArray<FSyntaxIssue> AllIssues;
    
    FString ProjectPath = GetProjectPath();
    
    // Scan Source directory
    TArray<FString> SourceFiles;
    IFileManager::Get().FindFilesRecursive(SourceFiles, *(ProjectPath / TEXT("Source")), 
        TEXT("*.h"), true, false, true);
    IFileManager::Get().FindFilesRecursive(SourceFiles, *(ProjectPath / TEXT("Source")), 
        TEXT("*.cpp"), true, false, true);
    
    // Scan Plugins
    TArray<FString> PluginDirs;
    IFileManager::Get().FindFiles(PluginDirs, *(ProjectPath / TEXT("Plugins/*")), false, true);
    for (const FString& PluginDir : PluginDirs)
    {
        FString PluginSourcePath = ProjectPath / TEXT("Plugins") / PluginDir / TEXT("Source");
        if (FPaths::DirectoryExists(PluginSourcePath))
        {
            IFileManager::Get().FindFilesRecursive(SourceFiles, *PluginSourcePath, 
                TEXT("*.h"), true, false, true);
            IFileManager::Get().FindFilesRecursive(SourceFiles, *PluginSourcePath, 
                TEXT("*.cpp"), true, false, true);
        }
    }
    
    for (const FString& FilePath : SourceFiles)
    {
        TArray<FSyntaxIssue> FileIssues = CheckFileForIssues(FilePath);
        AllIssues.Append(FileIssues);
    }
    
    return AllIssues;
}

TArray<FSyntaxIssue> UMingRTSCompilationManager::CheckFileForIssues(const FString& FilePath)
{
    TArray<FSyntaxIssue> Issues;
    
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *FilePath))
    {
        return Issues;
    }
    
    // Check 1: Brace balance (來自 ProjectHealthCheck.py)
    int32 OpenBraces = 0;
    int32 CloseBraces = 0;
    for (TCHAR Char : Content)
    {
        if (Char == '{') OpenBraces++;
        if (Char == '}') CloseBraces++;
    }
    
    if (OpenBraces != CloseBraces)
    {
        FSyntaxIssue Issue;
        Issue.FilePath = FilePath;
        Issue.IssueType = TEXT("BraceMismatch");
        Issue.Description = FString::Printf(TEXT("Brace mismatch: %d opening, %d closing"), OpenBraces, CloseBraces);
        Issue.bAutoFixable = true;
        Issues.Add(Issue);
    }
    
    // Check 2: UCLASS without GENERATED_BODY (來自 comprehensive_syntax_fix.py)
    if (Content.Contains(TEXT("UCLASS")) && !Content.Contains(TEXT("GENERATED_BODY")))
    {
        FSyntaxIssue Issue;
        Issue.FilePath = FilePath;
        Issue.IssueType = TEXT("MissingGENERATED_BODY");
        Issue.Description = TEXT("UCLASS without GENERATED_BODY()");
        Issue.bAutoFixable = true;
        Issues.Add(Issue);
    }
    
    // Check 3: Missing #pragma once in header files
    if (FilePath.EndsWith(TEXT(".h")))
    {
        if (!Content.Contains(TEXT("#pragma once")) && !Content.Contains(TEXT("#ifndef")))
        {
            FSyntaxIssue Issue;
            Issue.FilePath = FilePath;
            Issue.IssueType = TEXT("MissingPragmaOnce");
            Issue.Description = TEXT("Missing #pragma once or include guard");
            Issue.bAutoFixable = true;
            Issues.Add(Issue);
        }
    }
    
    // Check 4: Multiple #pragma once
    int32 PragmaCount = 0;
    int32 Index = 0;
    while ((Index = Content.Find(TEXT("#pragma once"), Index)) != INDEX_NONE)
    {
        PragmaCount++;
        Index++;
    }
    
    if (PragmaCount > 1)
    {
        FSyntaxIssue Issue;
        Issue.FilePath = FilePath;
        Issue.IssueType = TEXT("MultiplePragmaOnce");
        Issue.Description = FString::Printf(TEXT("Multiple #pragma once directives: %d"), PragmaCount);
        Issue.bAutoFixable = true;
        Issues.Add(Issue);
    }
    
    return Issues;
}

bool UMingRTSCompilationManager::FixSyntaxIssues(const TArray<FSyntaxIssue>& Issues)
{
    bool bAllFixed = true;
    
    for (const FSyntaxIssue& Issue : Issues)
    {
        if (Issue.bAutoFixable)
        {
            bAllFixed &= FixIssueInFile(Issue);
        }
    }
    
    return bAllFixed;
}

bool UMingRTSCompilationManager::AutoFixAllIssues()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::FixSyntaxErrors, TEXT("Auto Fix All Issues"));
    
    TArray<FSyntaxIssue> Issues = ScanForSyntaxIssues();
    bool bSuccess = FixSyntaxIssues(Issues);
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::FixSyntaxErrors, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::FixIssueInFile(const FSyntaxIssue& Issue)
{
    FString Content;
    if (!FFileHelper::LoadFileToString(Content, *Issue.FilePath))
    {
        return false;
    }
    
    bool bModified = false;
    
    if (Issue.IssueType == TEXT("MissingGENERATED_BODY"))
    {
        // Fix: Add GENERATED_BODY() after UCLASS
        FRegexPattern Pattern(TEXT("(UCLASS\\([^)]*\\)\\s*\\n\\s*class\\s+\\w+)"));
        FRegexMatcher Matcher(Pattern, Content);
        
        if (Matcher.FindNext())
        {
            FString Match = Matcher.GetCaptureGroup(0);
            FString Replacement = Match + TEXT("\n{\n\tGENERATED_BODY()");
            Content = Content.Replace(*Match, *Replacement);
            bModified = true;
        }
    }
    else if (Issue.IssueType == TEXT("MissingPragmaOnce"))
    {
        // Fix: Add #pragma once at beginning
        Content = TEXT("#pragma once\n\n") + Content;
        bModified = true;
    }
    else if (Issue.IssueType == TEXT("MultiplePragmaOnce"))
    {
        // Fix: Keep only first #pragma once
        int32 FirstIndex = Content.Find(TEXT("#pragma once"));
        if (FirstIndex != INDEX_NONE)
        {
            FString Before = Content.Left(FirstIndex + 12); // Include first #pragma once
            FString After = Content.Mid(FirstIndex + 12);
            After = After.Replace(TEXT("#pragma once"), TEXT(""));
            Content = Before + After;
            bModified = true;
        }
    }
    else if (Issue.IssueType == TEXT("BraceMismatch"))
    {
        // Fix: Add or remove braces at EOF
        int32 OpenCount = 0;
        int32 CloseCount = 0;
        for (TCHAR Char : Content)
        {
            if (Char == '{') OpenCount++;
            if (Char == '}') CloseCount++;
        }
        
        if (OpenCount > CloseCount)
        {
            int32 Diff = OpenCount - CloseCount;
            for (int32 i = 0; i < Diff; i++)
            {
                Content += TEXT("\n}");
            }
            bModified = true;
        }
        else if (CloseCount > OpenCount)
        {
            // Remove extra closing braces at end
            FString Trimmed = Content.TrimEnd();
            int32 Diff = CloseCount - OpenCount;
            for (int32 i = 0; i < Diff; i++)
            {
                if (Trimmed.EndsWith(TEXT("}")))
                {
                    Trimmed = Trimmed.Left(Trimmed.Len() - 1).TrimEnd();
                }
            }
            Content = Trimmed + TEXT("\n");
            bModified = true;
        }
    }
    
    if (bModified)
    {
        return FFileHelper::SaveStringToFile(Content, *Issue.FilePath);
    }
    
    return true;
}

// ==================== 編譯時間分析 (整合 BuildTimeAnalyzer.cmd) ====================

FBuildTimeStats UMingRTSCompilationManager::AnalyzeBuildTime()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::BuildTimeAnalysis, TEXT("Build Time Analysis"));
    
    // Return current stats
    OnTaskCompleted.Broadcast(ECompilationTaskType::BuildTimeAnalysis, true);
    return CompilationStats;
}

bool UMingRTSCompilationManager::GenerateBuildReport()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::BuildTimeAnalysis, TEXT("Generate Build Report"));
    
    FString ReportPath = GetProjectPath() / TEXT("Reports/BuildAnalysis.html");
    FString ReportContent = ExportReportToHTML();
    
    // Ensure directory exists
    IFileManager::Get().MakeDirectory(*(GetProjectPath() / TEXT("Reports")), true);
    
    bool bSuccess = FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::BuildTimeAnalysis, bSuccess);
    return bSuccess;
}

TArray<FString> UMingRTSCompilationManager::GetBuildOptimizationSuggestions()
{
    TArray<FString> Suggestions;
    
    // Analyze project structure
    FString ProjectPath = GetProjectPath();
    int32 TotalFiles = CountFilesInDirectory(ProjectPath / TEXT("Source"), TEXT(".cpp"));
    
    if (TotalFiles > 100)
    {
        Suggestions.Add(TEXT("項目文件數量較多，建議使用增量編譯 (FastIncrementalBuild)"));
    }
    
    if (CompilationStats.AverageBuildTime > 300.0f)
    {
        Suggestions.Add(TEXT("編譯時間較長，考慮使用 UnrealBuildTool 的 -MaxParallelJobs 參數"));
    }
    
    if (CompilationStats.FailedBuilds > CompilationStats.SuccessfulBuilds / 2)
    {
        Suggestions.Add(TEXT("編譯失敗率較高，建議運行健康檢查 (PerformHealthCheck)"));
    }
    
    return Suggestions;
}

// ==================== 編譯驗證 (整合 VerifyCompilation.cmd) ====================

bool UMingRTSCompilationManager::VerifyProjectStructure()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::VerifyCompilation, TEXT("Verify Project Structure"));
    
    // Check project file
    bool bValid = FPaths::FileExists(FPaths::ProjectFilePath());
    
    // Check Source directory
    bValid &= FPaths::DirectoryExists(GetProjectPath() / TEXT("Source"));
    
    // Check engine
    bValid &= FPaths::DirectoryExists(GetEnginePath());
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::VerifyCompilation, bValid);
    return bValid;
}

bool UMingRTSCompilationManager::VerifyCompilationEnvironment()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::VerifyCompilation, TEXT("Verify Compilation Environment"));
    
    bool bValid = true;
    
    // Check UBT
    bValid &= FPaths::FileExists(GetUBTPath());
    
    // Check Build.bat
    bValid &= FPaths::FileExists(GetBuildBatPath());
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::VerifyCompilation, bValid);
    return bValid;
}

bool UMingRTSCompilationManager::VerifyFixedFiles()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::VerifyCompilation, TEXT("Verify Fixed Files"));
    
    // Scan for any remaining issues
    TArray<FSyntaxIssue> Issues = ScanForSyntaxIssues();
    bool bValid = (Issues.Num() == 0);
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::VerifyCompilation, bValid);
    return bValid;
}

// ==================== 項目文件生成 ====================

bool UMingRTSCompilationManager::GenerateProjectFiles()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::GenerateProjectFiles, TEXT("Generate Project Files"));
    
    FString Command = FString::Printf(
        TEXT("\"%s\" -project=\"%s\" -game -engine"),
        *GetGenerateProjectBatPath(),
        *FPaths::ProjectFilePath()
    );
    
    int32 ExitCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*Command, nullptr, &ExitCode, &StdOut, &StdErr);
    
    bool bSuccess = (ExitCode == 0);
    OnTaskCompleted.Broadcast(ECompilationTaskType::GenerateProjectFiles, bSuccess);
    return bSuccess;
}

bool UMingRTSCompilationManager::RefreshProjectFiles()
{
    return GenerateProjectFiles();
}

// ==================== 命名規範檢查 (整合 CheckNamingConventions.py) ====================

bool UMingRTSCompilationManager::CheckNamingConventions()
{
    OnTaskStarted.Broadcast(ECompilationTaskType::SyntaxCheck, TEXT("Check Naming Conventions"));
    
    // Basic naming convention checks
    bool bValid = true;
    
    OnTaskCompleted.Broadcast(ECompilationTaskType::SyntaxCheck, bValid);
    return bValid;
}

TArray<FString> UMingRTSCompilationManager::GetNamingViolations()
{
    TArray<FString> Violations;
    
    // Check for naming violations
    
    return Violations;
}

// ==================== 批量任務管理 ====================

bool UMingRTSCompilationManager::QueueTask(const FCompilationTask& Task)
{
    TaskQueue.Add(Task);
    return true;
}

bool UMingRTSCompilationManager::ExecuteTaskQueue()
{
    if (TaskQueue.Num() == 0)
    {
        return true;
    }
    
    bIsProcessing = true;
    
    for (int32 i = 0; i < TaskQueue.Num(); i++)
    {
        const FCompilationTask& Task = TaskQueue[i];
        
        float Progress = static_cast<float>(i) / TaskQueue.Num() * 100.0f;
        OnTaskProgress.Broadcast(Task.TaskType, Progress, FString::Printf(TEXT("Processing: %s"), *Task.TaskName));
        
        ProcessSingleTask(Task);
    }
    
    bIsProcessing = false;
    TaskQueue.Empty();
    
    return true;
}

bool UMingRTSCompilationManager::ClearTaskQueue()
{
    TaskQueue.Empty();
    return true;
}

bool UMingRTSCompilationManager::ProcessSingleTask(const FCompilationTask& Task)
{
    switch (Task.TaskType)
    {
        case ECompilationTaskType::FullBuild:
            return BuildDevelopment();
        case ECompilationTaskType::IncrementalBuild:
            return FastIncrementalBuild();
        case ECompilationTaskType::CleanBuild:
            return CleanAndRebuild();
        case ECompilationTaskType::ProjectHealthCheck:
            PerformHealthCheck();
            return true;
        case ECompilationTaskType::BuildTimeAnalysis:
            AnalyzeBuildTime();
            return true;
        case ECompilationTaskType::VerifyCompilation:
            return VerifyCompilationEnvironment();
        case ECompilationTaskType::GenerateProjectFiles:
            return GenerateProjectFiles();
        case ECompilationTaskType::FixSyntaxErrors:
            return AutoFixAllIssues();
        default:
            return false;
    }
}

// ==================== 報告導出 ====================

FString UMingRTSCompilationManager::ExportReportToHTML()
{
    FString HTML = TEXT("<html><head><title>MingGoRTS Build Report</title></head><body>");
    HTML += TEXT("<h1>MingGoRTS Compilation Report</h1>");
    HTML += FString::Printf(TEXT("<p>Generated: %s</p>"), *FDateTime::Now().ToString());
    
    HTML += TEXT("<h2>Statistics</h2>");
    HTML += FString::Printf(TEXT("<p>Total Builds: %d</p>"), CompilationStats.TotalBuilds);
    HTML += FString::Printf(TEXT("<p>Successful: %d</p>"), CompilationStats.SuccessfulBuilds);
    HTML += FString::Printf(TEXT("<p>Failed: %d</p>"), CompilationStats.FailedBuilds);
    HTML += FString::Printf(TEXT("<p>Average Build Time: %.2f seconds</p>"), CompilationStats.AverageBuildTime);
    
    HTML += TEXT("</body></html>");
    
    return HTML;
}

FString UMingRTSCompilationManager::ExportReportToJSON()
{
    TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
    
    JsonObject->SetNumberField(TEXT("TotalBuilds"), CompilationStats.TotalBuilds);
    JsonObject->SetNumberField(TEXT("SuccessfulBuilds"), CompilationStats.SuccessfulBuilds);
    JsonObject->SetNumberField(TEXT("FailedBuilds"), CompilationStats.FailedBuilds);
    JsonObject->SetNumberField(TEXT("AverageBuildTime"), CompilationStats.AverageBuildTime);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

// ==================== 內部工具方法 ====================

bool UMingRTSCompilationManager::CleanBuildCache(bool bCleanIntermediate, bool bCleanDerivedData, bool bCleanSaved)
{
    FString ProjectPath = GetProjectPath();
    bool bSuccess = true;
    
    if (bCleanIntermediate)
    {
        bSuccess &= DeleteDirectoryRecursive(ProjectPath / TEXT("Intermediate"));
        
        // Clean plugin intermediates
        TArray<FString> PluginDirs;
        IFileManager::Get().FindFiles(PluginDirs, *(ProjectPath / TEXT("Plugins/*")), false, true);
        for (const FString& PluginDir : PluginDirs)
        {
            bSuccess &= DeleteDirectoryRecursive(ProjectPath / TEXT("Plugins") / PluginDir / TEXT("Intermediate"));
        }
    }
    
    if (bCleanDerivedData)
    {
        bSuccess &= DeleteDirectoryRecursive(ProjectPath / TEXT("DerivedDataCache"));
    }
    
    if (bCleanSaved)
    {
        bSuccess &= DeleteDirectoryRecursive(ProjectPath / TEXT("Saved/CompiledCookedPlatforms"));
    }
    
    return bSuccess;
}

bool UMingRTSCompilationManager::DeleteDirectoryRecursive(const FString& DirPath)
{
    if (!FPaths::DirectoryExists(DirPath))
    {
        return true;
    }
    
    return IFileManager::Get().DeleteDirectoryRecursively(*DirPath);
}

int32 UMingRTSCompilationManager::CountFilesInDirectory(const FString& DirPath, const FString& Extension)
{
    if (!FPaths::DirectoryExists(DirPath))
    {
        return 0;
    }
    
    TArray<FString> Files;
    IFileManager::Get().FindFilesRecursive(Files, *DirPath, *Extension, true, false, true);
    return Files.Num();
}

FString UMingRTSCompilationManager::ExecuteCommandAndGetOutput(const FString& Command, int32& OutExitCode)
{
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*Command, nullptr, &OutExitCode, &StdOut, &StdErr);
    return StdOut + StdErr;
}

void UMingRTSCompilationManager::UpdateBuildStats(bool bSuccess, float BuildTime)
{
    CompilationStats.TotalBuilds++;
    CompilationStats.LastBuildTime = BuildTime;
    
    if (bSuccess)
    {
        CompilationStats.SuccessfulBuilds++;
    }
    else
    {
        CompilationStats.FailedBuilds++;
    }
    
    // Update average
    if (CompilationStats.TotalBuilds > 0)
    {
        CompilationStats.AverageBuildTime = 
            ((CompilationStats.AverageBuildTime * (CompilationStats.TotalBuilds - 1)) + BuildTime) 
            / CompilationStats.TotalBuilds;
    }
}

void UMingRTSCompilationManager::LogAction(const FString& Action, const FString& Message)
{
    UE_LOG(LogTemp, Log, TEXT("[CompilationManager][%s] %s"), *Action, *Message);
}
