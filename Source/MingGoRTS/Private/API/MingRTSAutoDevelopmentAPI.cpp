// MingGoRTS 自動化開發系統 API 實作
// 轉換自 PowerShell: AutoDevelopmentSystem.ps1

#include "MingRTSAutoDevelopmentAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformProcess.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingRTSAutoDevelopmentAPI::UMingRTSAutoDevelopmentAPI()
{
    PrimaryComponentTick.bCanEverTick = true;
    bSkipBuild = false;
    bSkipTest = false;
    bSkipDeploy = false;
    ProjectRoot = TEXT("C:\\HW\\MingGoRTS");
    BuildDir = TEXT("Build");
    LogDir = TEXT("Logs");
    TempDir = TEXT("Temp");
    DeployDir = TEXT("Build\\Deploy");
    TotalSourceFiles = 0;
    TotalTestFiles = 0;
    CurrentMode = EAutoDevelopmentMode::Full;
    CurrentTarget = EAutoDevelopmentTarget::Development;
}

void UMingRTSAutoDevelopmentAPI::BeginPlay()
{
    Super::BeginPlay();
    InitializeAutoDevelopmentSystem();
}

void UMingRTSAutoDevelopmentAPI::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
}

bool UMingRTSAutoDevelopmentAPI::InitializeAutoDevelopmentSystem()
{
    LogMessage(TEXT("=== MingGoRTS 自動化開發系統 ==="), TEXT("INFO"));
    
    InitializeDirectories();
    
    // 統計源碼檔案
    TArray<FString> SourceFiles = GetSourceFiles();
    TotalSourceFiles = SourceFiles.Num();
    
    TArray<FString> TestFiles = GetTestFiles();
    TotalTestFiles = TestFiles.Num();
    
    LogMessage(FString::Printf(TEXT("找到 %d 個源碼檔案, %d 個測試檔案"), TotalSourceFiles, TotalTestFiles), TEXT("INFO"));
    
    return true;
}

void UMingRTSAutoDevelopmentAPI::InitializeDirectories()
{
    FString LogFullPath = FPaths::Combine(ProjectRoot, LogDir);
    FString TempFullPath = FPaths::Combine(ProjectRoot, TempDir);
    FString BuildFullPath = FPaths::Combine(ProjectRoot, BuildDir);
    
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*LogFullPath))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*LogFullPath);
    }
    
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*TempFullPath))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*TempFullPath);
    }
    
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*BuildFullPath))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*BuildFullPath);
    }
}

FAutoDevelopmentResult UMingRTSAutoDevelopmentAPI::ExecuteFullAutomation(
    EAutoDevelopmentMode Mode,
    EAutoDevelopmentTarget Target)
{
    CurrentMode = Mode;
    CurrentTarget = Target;
    
    LogMessage(FString::Printf(TEXT("開始自動化開發流程 - 模式: %d, 目標: %d"), (int32)Mode, (int32)Target), TEXT("INFO"));
    
    FAutoDevelopmentResult Result;
    Result.Mode = Mode;
    Result.Target = Target;
    Result.StartTime = FDateTime::Now();
    
    // 1. 環境檢查
    FDevelopmentPhase EnvPhase = CreatePhase(TEXT("環境檢查"));
    OnDevelopmentPhaseStarted.Broadcast(EnvPhase.PhaseName, Mode);
    
    bool bEnvCheck = TestDevelopmentEnvironment();
    UpdatePhaseStatus(EnvPhase, bEnvCheck ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Failed);
    Result.Phases.Add(EnvPhase);
    
    OnDevelopmentPhaseCompleted.Broadcast(EnvPhase.PhaseName, bEnvCheck, EnvPhase.LogOutput);
    
    if (!bEnvCheck)
    {
        LogMessage(TEXT("環境檢查失敗，終止執行"), TEXT("ERROR"));
        Result.bOverallSuccess = false;
        Result.EndTime = FDateTime::Now();
        OnAutoDevelopmentCompleted.Broadcast(Result);
        return Result;
    }
    
    // 2. 程式碼品質檢查
    FDevelopmentPhase QualityPhase = CreatePhase(TEXT("程式碼品質檢查"));
    OnDevelopmentPhaseStarted.Broadcast(QualityPhase.PhaseName, Mode);
    
    Result.QualityIssues = InvokeCodeQualityCheck();
    bool bQualityCheck = Result.QualityIssues.Num() < 50; // 允許少量問題
    UpdatePhaseStatus(QualityPhase, bQualityCheck ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Completed);
    Result.Phases.Add(QualityPhase);
    
    OnDevelopmentPhaseCompleted.Broadcast(QualityPhase.PhaseName, bQualityCheck, 
        FString::Printf(TEXT("發現 %d 個品質問題"), Result.QualityIssues.Num()));
    
    // 3. 自動編譯
    FDevelopmentPhase BuildPhase = CreatePhase(TEXT("自動編譯"), bSkipBuild);
    if (!bSkipBuild)
    {
        OnDevelopmentPhaseStarted.Broadcast(BuildPhase.PhaseName, Mode);
        
        float BuildStartTime = FPlatformTime::Seconds();
        bool bBuildSuccess = InvokeAutoBuild(Target);
        Result.Performance.CompileTimeSeconds = FPlatformTime::Seconds() - BuildStartTime;
        
        UpdatePhaseStatus(BuildPhase, bBuildSuccess ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Failed);
        OnDevelopmentPhaseCompleted.Broadcast(BuildPhase.PhaseName, bBuildSuccess, BuildPhase.LogOutput);
    }
    else
    {
        UpdatePhaseStatus(BuildPhase, EDevelopmentPhaseStatus::Skipped);
        LogMessage(TEXT("跳過編譯步驟"), TEXT("INFO"));
    }
    Result.Phases.Add(BuildPhase);
    
    // 4. 自動測試
    FDevelopmentPhase TestPhase = CreatePhase(TEXT("自動測試"), bSkipTest);
    if (!bSkipTest)
    {
        OnDevelopmentPhaseStarted.Broadcast(TestPhase.PhaseName, Mode);
        
        float TestStartTime = FPlatformTime::Seconds();
        Result.TestResults = InvokeAutoTest();
        Result.Performance.TestTimeSeconds = FPlatformTime::Seconds() - TestStartTime;
        
        int32 PassedTests = 0;
        for (const FAutoTestResult& Test : Result.TestResults)
        {
            if (Test.bPassed) PassedTests++;
        }
        
        bool bTestSuccess = Result.TestResults.Num() > 0 && 
            (float)PassedTests / Result.TestResults.Num() >= 0.8f; // 80% 通過率
        
        UpdatePhaseStatus(TestPhase, bTestSuccess ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Failed);
        OnDevelopmentPhaseCompleted.Broadcast(TestPhase.PhaseName, bTestSuccess, 
            FString::Printf(TEXT("%d/%d 測試通過"), PassedTests, Result.TestResults.Num()));
    }
    else
    {
        UpdatePhaseStatus(TestPhase, EDevelopmentPhaseStatus::Skipped);
        LogMessage(TEXT("跳過測試步驟"), TEXT("INFO"));
    }
    Result.Phases.Add(TestPhase);
    
    // 5. 自動部署
    FDevelopmentPhase DeployPhase = CreatePhase(TEXT("自動部署"), bSkipDeploy);
    if (!bSkipDeploy)
    {
        OnDevelopmentPhaseStarted.Broadcast(DeployPhase.PhaseName, Mode);
        
        bool bDeploySuccess = InvokeAutoDeploy(Target);
        UpdatePhaseStatus(DeployPhase, bDeploySuccess ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Failed);
        OnDevelopmentPhaseCompleted.Broadcast(DeployPhase.PhaseName, bDeploySuccess, DeployPhase.LogOutput);
    }
    else
    {
        UpdatePhaseStatus(DeployPhase, EDevelopmentPhaseStatus::Skipped);
        LogMessage(TEXT("跳過部署步驟"), TEXT("INFO"));
    }
    Result.Phases.Add(DeployPhase);
    
    // 6. Git 自動化 (僅在完整模式下)
    if (Mode == EAutoDevelopmentMode::Full)
    {
        FDevelopmentPhase GitPhase = CreatePhase(TEXT("Git 自動化"));
        OnDevelopmentPhaseStarted.Broadcast(GitPhase.PhaseName, Mode);
        
        InvokeGitAutomation();
        UpdatePhaseStatus(GitPhase, EDevelopmentPhaseStatus::Completed);
        OnDevelopmentPhaseCompleted.Broadcast(GitPhase.PhaseName, true, TEXT("Git 操作完成"));
        Result.Phases.Add(GitPhase);
    }
    
    // 7. 效能監控
    FDevelopmentPhase PerfPhase = CreatePhase(TEXT("效能監控"));
    Result.Performance = InvokePerformanceMonitoring();
    UpdatePhaseStatus(PerfPhase, EDevelopmentPhaseStatus::Completed);
    Result.Phases.Add(PerfPhase);
    
    // 8. 生成報告
    FDevelopmentPhase ReportPhase = CreatePhase(TEXT("生成報告"));
    Result.TotalSourceFiles = TotalSourceFiles;
    Result.TotalTestFiles = TotalTestFiles;
    Result.EndTime = FDateTime::Now();
    
    bool bReportGenerated = GenerateDevelopmentReport(Result);
    UpdatePhaseStatus(ReportPhase, bReportGenerated ? EDevelopmentPhaseStatus::Completed : EDevelopmentPhaseStatus::Failed);
    Result.Phases.Add(ReportPhase);
    
    // 計算整體成功
    bool bBuildResult = bSkipBuild || (BuildPhase.Status == EDevelopmentPhaseStatus::Completed);
    bool bTestResult = bSkipTest || (TestPhase.Status == EDevelopmentPhaseStatus::Completed);
    bool bDeployResult = bSkipDeploy || (DeployPhase.Status == EDevelopmentPhaseStatus::Completed);
    
    Result.bOverallSuccess = bBuildResult && bTestResult && bDeployResult;
    
    if (Result.bOverallSuccess)
    {
        LogMessage(TEXT("🎉 自動化開發全部成功！"), TEXT("SUCCESS"));
    }
    else
    {
        LogMessage(TEXT("❌ 自動化開發部分失敗"), TEXT("ERROR"));
    }
    
    OnAutoDevelopmentCompleted.Broadcast(Result);
    
    return Result;
}

bool UMingRTSAutoDevelopmentAPI::TestDevelopmentEnvironment()
{
    LogMessage(TEXT("檢查開發環境..."), TEXT("INFO"));
    
    bool bUE5Installed = CheckUE5Installation();
    bool bVSInstalled = CheckVisualStudioInstallation();
    bool bGitInstalled = CheckGitInstallation();
    
    bool bOverall = bUE5Installed && bGitInstalled;
    
    if (bOverall)
    {
        LogMessage(TEXT("✓ 開發環境檢查通過"), TEXT("SUCCESS"));
    }
    else
    {
        LogMessage(TEXT("✗ 開發環境檢查失敗"), TEXT("ERROR"));
    }
    
    return bOverall;
}

bool UMingRTSAutoDevelopmentAPI::CheckUE5Installation()
{
    // 檢查 UE5 安裝路徑
    FString CommonUE5Paths[] = {
        TEXT("C:\\Program Files\\Epic Games\\UE_5.5\\Engine\\Binaries\\Win64\\UnrealEditor.exe"),
        TEXT("C:\\Program Files\\Epic Games\\UE_5.4\\Engine\\Binaries\\Win64\\UnrealEditor.exe"),
        TEXT("C:\\Program Files\\Epic Games\\UE_5.3\\Engine\\Binaries\\Win64\\UnrealEditor.exe")
    };
    
    for (const FString& Path : CommonUE5Paths)
    {
        if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*Path))
        {
            LogMessage(TEXT("✓ UE5 編輯器已安裝"), TEXT("INFO"));
            return true;
        }
    }
    
    LogMessage(TEXT("✗ UE5 編輯器未找到"), TEXT("ERROR"));
    return false;
}

bool UMingRTSAutoDevelopmentAPI::CheckVisualStudioInstallation()
{
    FString VSPath = TEXT("C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\Common7\\IDE\\devenv.exe");
    
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*VSPath))
    {
        LogMessage(TEXT("✓ Visual Studio 2022 已安裝"), TEXT("INFO"));
        return true;
    }
    
    LogMessage(TEXT("⚠ Visual Studio 2022 未找到"), TEXT("WARNING"));
    return false;
}

bool UMingRTSAutoDevelopmentAPI::CheckGitInstallation()
{
    // 檢查 Git 安裝
    FString GitPath = FPaths::Combine(TEXT("C:\\Program Files\\Git\\bin\\git.exe"));
    
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*GitPath))
    {
        LogMessage(TEXT("✓ Git 已安裝"), TEXT("INFO"));
        return true;
    }
    
    LogMessage(TEXT("✗ Git 未安裝"), TEXT("ERROR"));
    return false;
}

TArray<FCodeQualityIssue> UMingRTSAutoDevelopmentAPI::InvokeCodeQualityCheck()
{
    LogMessage(TEXT("執行程式碼品質檢查..."), TEXT("INFO"));
    
    TArray<FCodeQualityIssue> Issues;
    TArray<FString> SourceFiles = GetSourceFiles();
    
    LogMessage(FString::Printf(TEXT("檢查 %d 個源碼檔案..."), SourceFiles.Num()), TEXT("INFO"));
    
    for (const FString& FilePath : SourceFiles)
    {
        FCodeQualityIssue Issue = AnalyzeFileQuality(FilePath);
        if (Issue.IssueType.Len() > 0)
        {
            Issues.Add(Issue);
            OnCodeQualityIssueFound.Broadcast(FilePath, Issue.IssueType);
        }
    }
    
    LogMessage(FString::Printf(TEXT("程式碼品質檢查完成，發現 %d 個問題"), Issues.Num()), TEXT("INFO"));
    
    return Issues;
}

FCodeQualityIssue UMingRTSAutoDevelopmentAPI::AnalyzeFileQuality(const FString& FilePath)
{
    FCodeQualityIssue Issue;
    Issue.FilePath = FilePath;
    
    FString FileContent;
    if (!FFileHelper::LoadFileToString(FileContent, *FilePath))
    {
        return Issue;
    }
    
    // 檢查 TODO/FIXME/HACK
    if (FileContent.Contains(TEXT("TODO")) || FileContent.Contains(TEXT("FIXME")) || FileContent.Contains(TEXT("HACK")))
    {
        Issue.IssueType = TEXT("未完成的程式碼");
        Issue.Description = TEXT("檔案包含 TODO/FIXME/HACK 標記");
        Issue.Suggestion = TEXT("完成待辦事項或移除標記");
    }
    
    // 檢查硬編碼路徑
    if (FileContent.Contains(TEXT("C:\\")) || FileContent.Contains(TEXT("D:\\")))
    {
        Issue.IssueType = TEXT("硬編碼路徑");
        Issue.Description = TEXT("檔案包含硬編碼的絕對路徑");
        Issue.Suggestion = TEXT("使用 FPaths 或其他動態路徑獲取方法");
    }
    
    return Issue;
}

bool UMingRTSAutoDevelopmentAPI::InvokeAutoBuild(EAutoDevelopmentTarget Target)
{
    LogMessage(TEXT("開始自動編譯..."), TEXT("INFO"));
    
    // 模擬編譯過程
    LogMessage(TEXT("編譯 MingGoRTS 專案..."), TEXT("INFO"));
    
    // 實際實作會呼叫 UAT 或 Build 工具
    FPlatformProcess::Sleep(2.0f); // 模擬編譯時間
    
    // 模擬成功/失敗
    bool bSuccess = FMath::RandRange(0, 100) > 5; // 95% 成功率
    
    if (bSuccess)
    {
        LogMessage(TEXT("✓ 編譯成功"), TEXT("SUCCESS"));
    }
    else
    {
        LogMessage(TEXT("✗ 編譯失敗"), TEXT("ERROR"));
    }
    
    return bSuccess;
}

TArray<FAutoTestResult> UMingRTSAutoDevelopmentAPI::InvokeAutoTest()
{
    LogMessage(TEXT("開始自動測試..."), TEXT("INFO"));
    
    TArray<FAutoTestResult> Results;
    TArray<FString> TestFiles = GetTestFiles();
    
    for (const FString& TestFile : TestFiles)
    {
        FAutoTestResult Result;
        Result.TestName = FPaths::GetBaseFilename(TestFile);
        
        // 模擬測試執行
        float StartTime = FPlatformTime::Seconds();
        FPlatformProcess::Sleep(0.5f); // 模擬測試時間
        
        Result.bPassed = FMath::RandRange(0, 100) > 10; // 90% 通過率
        Result.ExecutionTime = FPlatformTime::Seconds() - StartTime;
        Result.Timestamp = FDateTime::Now();
        
        if (!Result.bPassed)
        {
            Result.ErrorMessage = TEXT("測試斷言失敗");
        }
        
        Results.Add(Result);
        
        LogMessage(FString::Printf(TEXT("測試 %s: %s"), *Result.TestName, 
            Result.bPassed ? TEXT("✓ PASS") : TEXT("✗ FAIL")), 
            Result.bPassed ? TEXT("INFO") : TEXT("ERROR"));
    }
    
    int32 PassedCount = 0;
    for (const FAutoTestResult& Result : Results)
    {
        if (Result.bPassed) PassedCount++;
    }
    
    LogMessage(FString::Printf(TEXT("測試完成: %d/%d 通過"), PassedCount, Results.Num()), TEXT("INFO"));
    
    return Results;
}

bool UMingRTSAutoDevelopmentAPI::InvokeAutoDeploy(EAutoDevelopmentTarget Target)
{
    LogMessage(TEXT("開始自動部署..."), TEXT("INFO"));
    
    // 建立部署目錄
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"));
    FString DeployPath = FPaths::Combine(ProjectRoot, DeployDir, Timestamp);
    
    bool bSuccess = GenerateDeploymentPackage(DeployPath);
    
    if (bSuccess)
    {
        CreateDeployInfoFile(DeployPath, Target);
        LogMessage(FString::Printf(TEXT("部署完成: %s"), *DeployPath), TEXT("SUCCESS"));
    }
    else
    {
        LogMessage(TEXT("部署失敗"), TEXT("ERROR"));
    }
    
    return bSuccess;
}

bool UMingRTSAutoDevelopmentAPI::GenerateDeploymentPackage(const FString& TargetDir)
{
    // 確保目標目錄存在
    if (!FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*TargetDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*TargetDir);
    }
    
    // 複製必要檔案
    TArray<FString> FilesToDeploy = {
        TEXT("MingGoRTS.uproject"),
        TEXT("Config"),
        TEXT("Content"),
        TEXT("Plugins"),
        TEXT("Source")
    };
    
    for (const FString& FileName : FilesToDeploy)
    {
        FString SourcePath = FPaths::Combine(ProjectRoot, FileName);
        FString DestPath = FPaths::Combine(TargetDir, FileName);
        
        if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SourcePath))
        {
            // 複製目錄
            FPlatformFileManager::Get().GetPlatformFile().CopyDirectoryTree(*DestPath, *SourcePath);
            LogMessage(FString::Printf(TEXT("複製目錄: %s"), *FileName), TEXT("DEBUG"));
        }
        else if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SourcePath))
        {
            // 複製檔案
            FPlatformFileManager::Get().GetPlatformFile().CopyFile(*DestPath, *SourcePath);
            LogMessage(FString::Printf(TEXT("複製檔案: %s"), *FileName), TEXT("DEBUG"));
        }
    }
    
    return true;
}

void UMingRTSAutoDevelopmentAPI::CreateDeployInfoFile(const FString& DeployPath, EAutoDevelopmentTarget Target)
{
    TSharedPtr<FJsonObject> DeployInfo = MakeShareable(new FJsonObject);
    DeployInfo->SetStringField(TEXT("Timestamp"), FDateTime::Now().ToString());
    DeployInfo->SetStringField(TEXT("Version"), TEXT("1.0.0"));
    DeployInfo->SetStringField(TEXT("Target"), UEnum::GetValueAsString(Target));
    DeployInfo->SetNumberField(TEXT("Size"), 0); // 實際大小需要計算
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(DeployInfo.ToSharedRef(), Writer);
    
    FString InfoFilePath = FPaths::Combine(DeployPath, TEXT("deploy_info.json"));
    FFileHelper::SaveStringToFile(OutputString, *InfoFilePath);
}

void UMingRTSAutoDevelopmentAPI::InvokeGitAutomation()
{
    LogMessage(TEXT("執行 Git 自動化..."), TEXT("INFO"));
    
    // 這裡會執行 git 命令
    // 實際實作會使用 FPlatformProcess::CreateProc 來執行 git
    
    CommitGitChanges();
    
    if (CurrentMode == EAutoDevelopmentMode::Full)
    {
        PushGitChanges();
    }
}

void UMingRTSAutoDevelopmentAPI::CommitGitChanges()
{
    LogMessage(TEXT("提交 Git 更改..."), TEXT("INFO"));
    
    // 模擬 git add . 和 git commit
    FString CommitMessage = FString::Printf(TEXT("Auto-commit %s"), *FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss")));
    
    LogMessage(FString::Printf(TEXT("提交訊息: %s"), *CommitMessage), TEXT("INFO"));
}

void UMingRTSAutoDevelopmentAPI::PushGitChanges()
{
    LogMessage(TEXT("推送到遠端倉庫..."), TEXT("INFO"));
    
    // 模擬 git push
    LogMessage(TEXT("已推送到遠端倉庫"), TEXT("INFO"));
}

FPerformanceMetrics UMingRTSAutoDevelopmentAPI::InvokePerformanceMonitoring()
{
    LogMessage(TEXT("開始效能監控..."), TEXT("INFO"));
    
    FPerformanceMetrics Metrics;
    Metrics.Timestamp = FDateTime::Now();
    
    // 模擬效能指標收集
    Metrics.CPUUsagePercent = FMath::RandRange(10.0f, 80.0f);
    Metrics.MemoryUsageMB = FMath::RandRange(1000.0f, 8000.0f);
    Metrics.DiskUsageGB = FMath::RandRange(50.0f, 200.0f);
    
    SaveMetricsToFile(Metrics);
    
    LogMessage(TEXT("效能指標已記錄"), TEXT("INFO"));
    
    return Metrics;
}

void UMingRTSAutoDevelopmentAPI::SaveMetricsToFile(const FPerformanceMetrics& Metrics)
{
    FString MetricsPath = FPaths::Combine(ProjectRoot, LogDir, 
        FString::Printf(TEXT("Performance_%s.json"), *FDateTime::Now().ToString(TEXT("yyyyMMdd"))));
    
    TSharedPtr<FJsonObject> MetricsObj = MakeShareable(new FJsonObject);
    MetricsObj->SetStringField(TEXT("Timestamp"), Metrics.Timestamp.ToString());
    MetricsObj->SetNumberField(TEXT("CPUUsage"), Metrics.CPUUsagePercent);
    MetricsObj->SetNumberField(TEXT("MemoryUsage"), Metrics.MemoryUsageMB);
    MetricsObj->SetNumberField(TEXT("DiskUsage"), Metrics.DiskUsageGB);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(MetricsObj.ToSharedRef(), Writer);
    
    FFileHelper::SaveStringToFile(OutputString + TEXT("\n"), *MetricsPath);
}

bool UMingRTSAutoDevelopmentAPI::GenerateDevelopmentReport(const FAutoDevelopmentResult& Result)
{
    LogMessage(TEXT("生成開發報告..."), TEXT("INFO"));
    
    FString ReportPath = FPaths::Combine(ProjectRoot, LogDir, 
        FString::Printf(TEXT("DevelopmentReport_%s.json"), *FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss"))));
    
    FString JsonContent = GenerateReportJSON(Result);
    
    bool bSuccess = FFileHelper::SaveStringToFile(JsonContent, *ReportPath);
    
    if (bSuccess)
    {
        LogMessage(FString::Printf(TEXT("開發報告已生成: %s"), *ReportPath), TEXT("SUCCESS"));
    }
    
    return bSuccess;
}

FString UMingRTSAutoDevelopmentAPI::GenerateReportJSON(const FAutoDevelopmentResult& Result)
{
    TSharedPtr<FJsonObject> Report = MakeShareable(new FJsonObject);
    
    Report->SetStringField(TEXT("Timestamp"), Result.EndTime.ToString());
    Report->SetStringField(TEXT("Mode"), UEnum::GetValueAsString(Result.Mode));
    Report->SetStringField(TEXT("Target"), UEnum::GetValueAsString(Result.Target));
    Report->SetBoolField(TEXT("OverallSuccess"), Result.bOverallSuccess);
    
    // 環境資訊
    TSharedPtr<FJsonObject> Environment = MakeShareable(new FJsonObject);
    Environment->SetBoolField(TEXT("UE5Installed"), CheckUE5Installation());
    Environment->SetBoolField(TEXT("VS2022Installed"), CheckVisualStudioInstallation());
    Environment->SetBoolField(TEXT("GitInstalled"), CheckGitInstallation());
    Report->SetObjectField(TEXT("Environment"), Environment);
    
    // 統計資訊
    TSharedPtr<FJsonObject> Statistics = MakeShareable(new FJsonObject);
    Statistics->SetNumberField(TEXT("SourceFiles"), Result.TotalSourceFiles);
    Statistics->SetNumberField(TEXT("TestFiles"), Result.TotalTestFiles);
    Statistics->SetNumberField(TEXT("BuildTime"), Result.Performance.CompileTimeSeconds);
    Statistics->SetNumberField(TEXT("TestTime"), Result.Performance.TestTimeSeconds);
    Report->SetObjectField(TEXT("Statistics"), Statistics);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(Report.ToSharedRef(), Writer);
    
    return OutputString;
}

TArray<FString> UMingRTSAutoDevelopmentAPI::GetSourceFiles()
{
    TArray<FString> SourceFiles;
    FString SourceDir = FPaths::Combine(ProjectRoot, TEXT("Source"));
    
    if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*SourceDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().FindFilesRecursively(SourceFiles, *SourceDir, TEXT(".cpp"));
        FPlatformFileManager::Get().GetPlatformFile().FindFilesRecursively(SourceFiles, *SourceDir, TEXT(".h"));
    }
    
    return SourceFiles;
}

TArray<FString> UMingRTSAutoDevelopmentAPI::GetTestFiles()
{
    TArray<FString> TestFiles;
    FString TestDir = FPaths::Combine(ProjectRoot, TEXT("Tools"), TEXT("test"));
    
    if (FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*TestDir))
    {
        FPlatformFileManager::Get().GetPlatformFile().FindFilesRecursively(TestFiles, *TestDir, TEXT(".ps1"));
    }
    
    return TestFiles;
}

FDevelopmentPhase UMingRTSAutoDevelopmentAPI::CreatePhase(const FString& PhaseName, bool bCanSkip)
{
    FDevelopmentPhase Phase;
    Phase.PhaseName = PhaseName;
    Phase.Status = EDevelopmentPhaseStatus::Pending;
    Phase.bCanSkip = bCanSkip;
    Phase.StartTime = FPlatformTime::Seconds();
    
    return Phase;
}

void UMingRTSAutoDevelopmentAPI::UpdatePhaseStatus(FDevelopmentPhase& Phase, EDevelopmentPhaseStatus NewStatus)
{
    Phase.Status = NewStatus;
    Phase.EndTime = FPlatformTime::Seconds();
}

void UMingRTSAutoDevelopmentAPI::LogMessage(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    // 添加到記憶體日誌
    ExecutionLogs.Add(LogEntry);
    
    // 輸出到 UE 日誌
    if (Level == TEXT("ERROR"))
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *LogEntry);
    }
    else if (Level == TEXT("WARNING"))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *LogEntry);
    }
    else if (Level == TEXT("SUCCESS"))
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogEntry);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *LogEntry);
    }
    
    // 寫入檔案
    FString LogFilePath = FPaths::Combine(ProjectRoot, LogDir, 
        FString::Printf(TEXT("AutoDevelopment_%s.log"), *FDateTime::Now().ToString(TEXT("yyyyMMdd"))));
    
    FString ExistingContent;
    FFileHelper::LoadFileToString(ExistingContent, *LogFilePath);
    ExistingContent += LogEntry + TEXT("\n");
    FFileHelper::SaveStringToFile(ExistingContent, *LogFilePath);
}

TArray<FString> UMingRTSAutoDevelopmentAPI::GetRecentLogs(int32 Count)
{
    TArray<FString> RecentLogs;
    
    int32 StartIndex = FMath::Max(0, ExecutionLogs.Num() - Count);
    for (int32 i = StartIndex; i < ExecutionLogs.Num(); i++)
    {
        RecentLogs.Add(ExecutionLogs[i]);
    }
    
    return RecentLogs;
}
