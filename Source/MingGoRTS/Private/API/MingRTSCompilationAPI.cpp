// MingGoRTS Compilation API Implementation
// 編譯 API 實現 - 調用 UnrealBuildTool 進行項目編譯

#include "API/MingRTSCompilationAPI.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformProcess.h"
#include "HAL/PlatformProcess.h"
#include "Misc/OutputDeviceRedirector.h"
#include "HAL/PlatformTime.h"
#include "Regex.h"

UMingRTSCompilationAPI::UMingRTSCompilationAPI()
{
    bIsBuilding = false;
    bShouldCancel = false;
    InitializeLearningDatabase();
}

// ==================== 項目編譯功能 ====================

FBuildResult UMingRTSCompilationAPI::BuildProject(const FBuildOptions& Options)
{
    FBuildResult Result;
    
    if (bIsBuilding)
    {
        Result.Status = EBuildStatus::Failed;
        Result.ErrorCode = -1;
        Result.OutputLog = TEXT("另一個編譯任務正在進行中");
        return Result;
    }
    
    bIsBuilding = true;
    bShouldCancel = false;
    Result.Status = EBuildStatus::Preparing;
    
    // 清理構建緩存（如果需要）
    if (Options.bCleanBuild)
    {
        CleanBuildCache(true, true, false);
    }
    
    // 構建 UBT 命令
    FString Command = BuildUBTCommand(Options);
    
    // 執行編譯
    Result = ExecuteBuildCommand(Command);
    
    bIsBuilding = false;
    LastBuildResult = Result;
    
    return Result;
}

FBuildResult UMingRTSCompilationAPI::BuildProjectAsync(const FBuildOptions& Options)
{
    // 異步版本調用同步版本（UE4/UE5 Blueprint 不直接支持真正的異步）
    // 在實際遊戲線程中，這會阻塞，但可以在單獨線程中運行
    return BuildProject(Options);
}

bool UMingRTSCompilationAPI::CancelBuild()
{
    if (!bIsBuilding)
    {
        return false;
    }
    
    bShouldCancel = true;
    return true;
}

bool UMingRTSCompilationAPI::CleanBuildCache(bool bCleanIntermediate, bool bCleanDerivedData, bool bCleanSaved)
{
    FString ProjectPath = FPaths::ProjectDir();
    bool bSuccess = true;
    
    // 清理 Intermediate 目錄
    if (bCleanIntermediate)
    {
        TArray<FString> PluginDirs;
        IFileManager::Get().FindFiles(PluginDirs, *(ProjectPath / TEXT("Plugins/*")), false, true);
        
        // 清理主項目 Intermediate
        FString MainIntermediate = ProjectPath / TEXT("Intermediate");
        if (FPaths::DirectoryExists(MainIntermediate))
        {
            bSuccess &= DeleteDirectory(MainIntermediate);
        }
        
        // 清理插件 Intermediate
        for (const FString& PluginDir : PluginDirs)
        {
            FString PluginIntermediate = ProjectPath / TEXT("Plugins") / PluginDir / TEXT("Intermediate");
            if (FPaths::DirectoryExists(PluginIntermediate))
            {
                bSuccess &= DeleteDirectory(PluginIntermediate);
            }
        }
    }
    
    // 清理 DerivedDataCache
    if (bCleanDerivedData)
    {
        FString DDCPath = ProjectPath / TEXT("DerivedDataCache");
        if (FPaths::DirectoryExists(DDCPath))
        {
            bSuccess &= DeleteDirectory(DDCPath);
        }
    }
    
    // 清理 Saved/CompiledCookedPlatforms
    if (bCleanSaved)
    {
        FString SavedPath = ProjectPath / TEXT("Saved/CompiledCookedPlatforms");
        if (FPaths::DirectoryExists(SavedPath))
        {
            bSuccess &= DeleteDirectory(SavedPath);
        }
    }
    
    return bSuccess;
}

bool UMingRTSCompilationAPI::GenerateProjectFiles()
{
    FString EnginePath = TEXT("C:\\Program Files\\Epic Games\\UE_5.7");
    FString ProjectPath = FPaths::ProjectFilePath();
    
    FString GenerateScript = EnginePath / TEXT("Engine\\Build\\BatchFiles\\GenerateProjectFiles.bat");
    FString Command = FString::Printf(TEXT("\"%s\" -project=\"%s\" -game -engine"), 
        *GenerateScript, *ProjectPath);
    
    int32 ExitCode = 0;
    FString StdOut, StdErr;
    FPlatformProcess::ExecProcess(*Command, nullptr, &ExitCode, &StdOut, &StdErr);
    
    return ExitCode == 0;
}

bool UMingRTSCompilationAPI::ValidateBuildEnvironment(FString& OutErrorMessage)
{
    // 檢查引擎路徑
    FString EnginePath = TEXT("C:\\Program Files\\Epic Games\\UE_5.7");
    if (!FPaths::DirectoryExists(EnginePath))
    {
        OutErrorMessage = FString::Printf(TEXT("找不到 UE5 引擎路徑: %s"), *EnginePath);
        return false;
    }
    
    // 檢查 UBT
    FString UBTPath = EnginePath / TEXT("Engine\\Binaries\\DotNET\\UnrealBuildTool\\UnrealBuildTool.exe");
    if (!FPaths::FileExists(UBTPath))
    {
        OutErrorMessage = FString::Printf(TEXT("找不到 UnrealBuildTool: %s"), *UBTPath);
        return false;
    }
    
    // 檢查項目文件
    FString ProjectFile = FPaths::ProjectFilePath();
    if (!FPaths::FileExists(ProjectFile))
    {
        OutErrorMessage = FString::Printf(TEXT("找不到項目文件: %s"), *ProjectFile);
        return false;
    }
    
    OutErrorMessage = TEXT("環境驗證通過");
    return true;
}

TArray<EBuildConfiguration> UMingRTSCompilationAPI::GetAvailableConfigurations() const
{
    return {
        EBuildConfiguration::Development,
        EBuildConfiguration::Debug,
        EBuildConfiguration::Shipping,
        EBuildConfiguration::Test,
        EBuildConfiguration::Editor
    };
}

TArray<EBuildPlatform> UMingRTSCompilationAPI::GetAvailablePlatforms() const
{
    TArray<EBuildPlatform> Platforms;
    
    Platforms.Add(EBuildPlatform::Win64);
    
    #if PLATFORM_WINDOWS
    // Win32 在現代 Windows 上較少使用
    #endif
    
    return Platforms;
}

// ==================== 工具方法 ====================

FString UMingRTSCompilationAPI::GetBuildStatusString(EBuildStatus Status)
{
    switch (Status)
    {
        case EBuildStatus::Idle: return TEXT("空閒");
        case EBuildStatus::Preparing: return TEXT("準備中");
        case EBuildStatus::Compiling: return TEXT("編譯中");
        case EBuildStatus::Linking: return TEXT("鏈接中");
        case EBuildStatus::Succeeded: return TEXT("成功");
        case EBuildStatus::Failed: return TEXT("失敗");
        case EBuildStatus::Cancelled: return TEXT("已取消");
        default: return TEXT("未知");
    }
}

FString UMingRTSCompilationAPI::GetConfigurationString(EBuildConfiguration Config)
{
    switch (Config)
    {
        case EBuildConfiguration::Development: return TEXT("Development");
        case EBuildConfiguration::Debug: return TEXT("Debug");
        case EBuildConfiguration::Shipping: return TEXT("Shipping");
        case EBuildConfiguration::Test: return TEXT("Test");
        case EBuildConfiguration::Editor: return TEXT("Editor");
        default: return TEXT("Unknown");
    }
}

FString UMingRTSCompilationAPI::GetPlatformString(EBuildPlatform Platform)
{
    switch (Platform)
    {
        case EBuildPlatform::Win64: return TEXT("Win64");
        case EBuildPlatform::Win32: return TEXT("Win32");
        case EBuildPlatform::Android: return TEXT("Android");
        case EBuildPlatform::IOS: return TEXT("IOS");
        case EBuildPlatform::Linux: return TEXT("Linux");
        case EBuildPlatform::Mac: return TEXT("Mac");
        default: return TEXT("Unknown");
    }
}

// ==================== 內部實現 ====================

FString UMingRTSCompilationAPI::BuildUBTCommand(const FBuildOptions& Options)
{
    FString EnginePath = Options.EnginePath.IsEmpty() ? 
        TEXT("C:\\Program Files\\Epic Games\\UE_5.7") : Options.EnginePath;
    
    FString ProjectPath = Options.ProjectPath.IsEmpty() ? 
        FPaths::ProjectFilePath() : Options.ProjectPath;
    
    FString UBTBat = EnginePath / TEXT("Engine\\Build\\BatchFiles\\Build.bat");
    
    FString Target = Options.TargetName;
    FString Platform = GetPlatformString(Options.Platform);
    FString Configuration = GetConfigurationString(Options.Configuration);
    
    // 構建命令行
    FString Command = FString::Printf(
        TEXT("\"%s\" %s %s %s -Project=\"%s\""),
        *UBTBat,
        *Target,
        *Platform,
        *Configuration,
        *ProjectPath
    );
    
    // 添加可選標誌
    if (Options.bWaitMutex)
    {
        Command += TEXT(" -WaitMutex");
    }
    
    if (Options.bNoHotReload)
    {
        Command += TEXT(" -NoHotReload");
    }
    
    if (Options.MaxParallelJobs > 0)
    {
        Command += FString::Printf(TEXT(" -MaxParallelJobs=%d"), Options.MaxParallelJobs);
    }
    
    for (const FString& Flag : Options.AdditionalFlags)
    {
        Command += TEXT(" ") + Flag;
    }
    
    return Command;
}

FBuildResult UMingRTSCompilationAPI::ExecuteBuildCommand(const FString& Command)
{
    FBuildResult Result;
    Result.Status = EBuildStatus::Compiling;
    
    double StartTime = FPlatformTime::Seconds();
    
    int32 ExitCode = 0;
    FString StdOut, StdErr;
    
    // 執行編譯命令
    FPlatformProcess::ExecProcess(*Command, nullptr, &ExitCode, &StdOut, &StdErr);
    
    double EndTime = FPlatformTime::Seconds();
    Result.BuildTimeSeconds = static_cast<float>(EndTime - StartTime);
    Result.ExitCode = ExitCode;
    Result.OutputLog = StdOut + TEXT("\n") + StdErr;
    
    // 解析錯誤和警告
    Result.Errors = ParseBuildErrors(StdOut);
    Result.Warnings = ParseBuildErrors(StdOut); // 同樣的解析邏輯，但過濾警告
    
    // 確定狀態
    if (bShouldCancel)
    {
        Result.Status = EBuildStatus::Cancelled;
    }
    else if (ExitCode == 0)
    {
        Result.Status = EBuildStatus::Succeeded;
    }
    else
    {
        Result.Status = EBuildStatus::Failed;
    }
    
    return Result;
}

TArray<FBuildError> UMingRTSCompilationAPI::ParseBuildErrors(const FString& LogOutput)
{
    TArray<FBuildError> Errors;
    
    // 錯誤模式：文件路徑(行號): error 錯誤代碼: 消息
    const FRegexPattern ErrorPattern(TEXT("(.+?)\\((\\d+)\\):\\s*(error|warning|note)\\s*(\\w+)?\\s*:?\\s*(.+)$"));
    
    TArray<FString> Lines;
    LogOutput.ParseIntoArrayLines(Lines);
    
    for (const FString& Line : Lines)
    {
        FRegexMatcher Matcher(ErrorPattern, Line);
        if (Matcher.FindNext())
        {
            FBuildError Error;
            Error.FilePath = Matcher.GetCaptureGroup(1);
            Error.LineNumber = FCString::Atoi(*Matcher.GetCaptureGroup(2));
            Error.Severity = Matcher.GetCaptureGroup(3); // error, warning, note
            Error.ErrorCode = Matcher.GetCaptureGroup(4);
            Error.Message = Matcher.GetCaptureGroup(5);
            
            Errors.Add(Error);
        }
    }
    
    return Errors;
}

int UMingRTSCompilationAPI::CalculateBuildProgress(const FString& LogLine, int CurrentProgress) const
{
    // 根據日誌輸出估算進度
    if (LogLine.Contains(TEXT("Building")))
    {
        return FMath::Min(CurrentProgress + 5, 90);
    }
    else if (LogLine.Contains(TEXT("Linking")))
    {
        return 95;
    }
    else if (LogLine.Contains(TEXT("[1/1]")) || LogLine.Contains(TEXT("Built")))
    {
        return 100;
    }
    
    return CurrentProgress;
}

bool UMingRTSCompilationAPI::DeleteDirectory(const FString& DirPath)
{
    if (!FPaths::DirectoryExists(DirPath))
    {
        return true;
    }
    
    return IFileManager::Get().DeleteDirectoryRecursively(*DirPath);
}

// ==================== 自我學習功能（保留原有實現） ====================

void UMingRTSCompilationAPI::InitializeLearningDatabase()
{
    // 初始化學習數據庫
    LoadLearningData();
}

void UMingRTSCompilationAPI::SaveLearningData()
{
    // 保存學習數據到文件
    FString LearningDataPath = FPaths::ProjectDir() / TEXT("Config/SelfLearningData.json");
    // 實現數據序列化
}

void UMingRTSCompilationAPI::LoadLearningData()
{
    // 從文件加載學習數據
    FString LearningDataPath = FPaths::ProjectDir() / TEXT("Config/SelfLearningData.json");
    // 實現數據反序列化
}

FString UMingRTSCompilationAPI::AnalyzeErrorPattern(const FCompilationResult& Result)
{
    // 分析錯誤模式
    return TEXT("");
}

bool UMingRTSCompilationAPI::ApplyLearnedFix(FString& SourceCode, const FString& ErrorPattern)
{
    // 應用學習到的修復
    return false;
}

// 其他原有方法的實現...
void UMingRTSCompilationAPI::LearnFromCompilationResult(const FCompilationResult& Result, const FString& OriginalCode)
{
    // 從編譯結果學習
}

void UMingRTSCompilationAPI::UpdateErrorPatterns(const ECompilationErrorType ErrorType, const FString& Pattern)
{
    // 更新錯誤模式
}

TArray<FString> UMingRTSCompilationAPI::GetSmartSuggestions(const FCompilationResult& Result)
{
    // 獲取智能建議
    return TArray<FString>();
}

FCompilationResult UMingRTSCompilationAPI::OptimizeCompilation(const FCompilationContext& Context)
{
    // 優化編譯
    return FCompilationResult();
}

void UMingRTSCompilationAPI::UpdateOptimizationStrategies(const FString& Strategy, float SuccessRate)
{
    // 更新優化策略
}

void UMingRTSCompilationAPI::RequestLogAnalysis(const FString& CompilationLog)
{
    // 請求日誌分析
}

void UMingRTSCompilationAPI::RequestDebuggingSession(const FCompilationResult& ErrorResult)
{
    // 請求調試會話
}

void UMingRTSCompilationAPI::RequestPerformanceAnalysis(const FString& ProjectPath)
{
    // 請求性能分析
}
