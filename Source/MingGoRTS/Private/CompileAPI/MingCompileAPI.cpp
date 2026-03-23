#include "MingCompileAPI.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/Engine.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "HAL/Process.h"
#include "Async/Async.h"
#include "HAL/ThreadManager.h"

UMingCompileAPI::UMingCompileAPI()
{
    CurrentStatus = EMingCompileStatus::Idle;
    CurrentProcess = nullptr;
    bIsInitialized = false;
    ProjectRoot = FPaths::ProjectDir();
    BuildPath = FPaths::Combine(*ProjectRoot, TEXT("Build"));
    LogPath = FPaths::Combine(*ProjectRoot, TEXT("Logs"));
}

void UMingCompileAPI::InitializeCompileAPI()
{
    if (bIsInitialized)
    {
        WriteLog(TEXT("Compile API already initialized"), TEXT("WARNING"));
        return;
    }

    WriteLog(TEXT("========================================"), TEXT("INFO"));
    WriteLog(TEXT("MingGoRTS Compile API System"), TEXT("INFO"));
    WriteLog(TEXT("========================================"), TEXT("INFO"));

    // Set default configuration
    DefaultConfiguration.CompileMode = EMingCompileMode::Development;
    DefaultConfiguration.Target = EMingCompileTarget::Game;
    DefaultConfiguration.ProjectPath = ProjectRoot;
    DefaultConfiguration.OutputPath = FPaths::Combine(*BuildPath, TEXT("Binaries"));
    DefaultConfiguration.bCleanBuild = false;
    DefaultConfiguration.ParallelJobs = FMath::Clamp(FPlatformMisc::NumberOfCoresIncludingHyperthreads() / 2, 1, 8);
    DefaultConfiguration.AdditionalArguments.Empty();

    // Ensure directories exist
    IFileManager::Get().MakeDirectory(*BuildPath, true);
    IFileManager::Get().MakeDirectory(*LogPath, true);
    IFileManager::Get().MakeDirectory(*DefaultConfiguration.OutputPath, true);

    bIsInitialized = true;
    WriteLog(TEXT("Compile API initialized successfully"), TEXT("SUCCESS"));
}

FMingCompileResult UMingCompileAPI::CompileProject(const FMingCompileConfiguration& Configuration)
{
    WriteLog(TEXT("Starting synchronous compilation"), TEXT("INFO"));
    
    FMingCompileResult Result;
    Result.StartTime = FDateTime::Now();
    CurrentStatus = EMingCompileStatus::Compiling;

    // Broadcast compile started event
    OnCompileStarted.Broadcast(Configuration);

    // Execute compilation
    bool bSuccess = ExecuteCompilation(Configuration, Result);

    Result.EndTime = FDateTime::Now();
    Result.CompileTime = (Result.EndTime - Result.StartTime).GetTotalSeconds();
    CurrentStatus = bSuccess ? EMingCompileStatus::Success : EMingCompileStatus::Failed;

    // Broadcast compile completed event
    OnCompileCompleted.Broadcast(Result);

    WriteLog(FString::Printf(TEXT("Compilation completed in %.2f seconds"), Result.CompileTime), 
              bSuccess ? TEXT("SUCCESS") : TEXT("ERROR"));

    return Result;
}

void UMingCompileAPI::CompileProjectAsync(const FMingCompileConfiguration& Configuration)
{
    WriteLog(TEXT("Starting asynchronous compilation"), TEXT("INFO"));

    // Run compilation in background thread
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalPriority, [this, Configuration]()
    {
        FMingCompileResult Result = CompileProject(Configuration);
        
        // Broadcast result on game thread
        AsyncTask(ENamedThreads::GameThread, [this, Result]()
        {
            OnCompileCompleted.Broadcast(Result);
        });
    });
}

void UMingCompileAPI::CancelCompilation()
{
    if (CurrentStatus == EMingCompileStatus::Compiling && CurrentProcess)
    {
        WriteLog(TEXT("Cancelling compilation"), TEXT("WARNING"));
        
        // Terminate the process
        FPlatformProcess::TerminateProc(*CurrentProcess, true);
        CurrentStatus = EMingCompileStatus::Cancelled;
        
        WriteLog(TEXT("Compilation cancelled"), TEXT("INFO"));
    }
}

EMingCompileStatus UMingCompileAPI::GetCompileStatus()
{
    return CurrentStatus;
}

FMingCompileConfiguration UMingCompileAPI::GetDefaultConfiguration()
{
    return DefaultConfiguration;
}

void UMingCompileAPI::SetDefaultConfiguration(const FMingCompileConfiguration& Configuration)
{
    DefaultConfiguration = Configuration;
    WriteLog(TEXT("Default configuration updated"), TEXT("INFO"));
}

FMingTestResult UMingCompileAPI::RunTests(const FMingTestConfiguration& TestConfig)
{
    WriteLog(TEXT("Starting synchronous test execution"), TEXT("INFO"));
    
    FMingTestResult Result;
    float StartTime = FPlatformTime::Seconds();

    // Broadcast test started event
    OnTestStarted.Broadcast(TestConfig);

    // Execute tests
    bool bSuccess = ExecuteTests(TestConfig, Result);

    Result.TestTime = FPlatformTime::Seconds() - StartTime;

    // Broadcast test completed event
    OnTestCompleted.Broadcast(Result);

    WriteLog(FString::Printf(TEXT("Tests completed in %.2f seconds"), Result.TestTime), 
              bSuccess ? TEXT("SUCCESS") : TEXT("ERROR"));

    return Result;
}

void UMingCompileAPI::RunTestsAsync(const FMingTestConfiguration& TestConfig)
{
    WriteLog(TEXT("Starting asynchronous test execution"), TEXT("INFO"));

    // Run tests in background thread
    AsyncTask(ENamedThreads::AnyBackgroundThreadNormalPriority, [this, TestConfig]()
    {
        FMingTestResult Result = RunTests(TestConfig);
        
        // Broadcast result on game thread
        AsyncTask(ENamedThreads::GameThread, [this, Result]()
        {
            OnTestCompleted.Broadcast(Result);
        });
    });
}

bool UMingCompileAPI::GenerateCompileReport(const FMingCompileResult& Result, const FString& ReportPath)
{
    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingGoRTS Compile Report</title>\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    ReportContent += TEXT(".summary { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".success { color: #27ae60; }\n");
    ReportContent += TEXT(".failure { color: #e74c3c; }\n");
    ReportContent += TEXT(".warning { color: #f39c12; }\n");
    ReportContent += TEXT(".info { color: #3498db; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    ReportContent += TEXT("th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; }\n");
    ReportContent += TEXT(".log { background-color: #f8f9fa; padding: 15px; border-radius: 5px; font-family: monospace; white-space: pre-wrap; max-height: 400px; overflow-y: auto; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>MingGoRTS Compile Report</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>Generated: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += FString::Printf(TEXT("<p>Status: %s</p>\n"), 
        Result.Status == EMingCompileStatus::Success ? TEXT("Success") : 
        Result.Status == EMingCompileStatus::Failed ? TEXT("Failed") : TEXT("Unknown"));
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("<div class=\"summary\">\n");
    ReportContent += TEXT("<h2>Compilation Summary</h2>\n");
    ReportContent += FString::Printf(TEXT("<p><strong>Compile Time:</strong> %.2f seconds</p>\n"), Result.CompileTime);
    ReportContent += FString::Printf(TEXT("<p><strong>Warnings:</strong> <span class=\"warning\">%d</span></p>\n"), Result.WarningCount);
    ReportContent += FString::Printf(TEXT("<p><strong>Errors:</strong> <span class=\"failure\">%d</span></p>\n"), Result.ErrorCount);
    
    if (!Result.ErrorMessage.IsEmpty())
    {
        ReportContent += FString::Printf(TEXT("<p><strong>Error Message:</strong> <span class=\"failure\">%s</span></p>\n"), *Result.ErrorMessage);
    }
    
    ReportContent += FString::Printf(TEXT("<p><strong>Start Time:</strong> %s</p>\n"), *Result.StartTime.ToString());
    ReportContent += FString::Printf(TEXT("<p><strong>End Time:</strong> %s</p>\n"), *Result.EndTime.ToString());
    ReportContent += TEXT("</div>\n");

    if (!Result.OutputLog.IsEmpty())
    {
        ReportContent += TEXT("<div class=\"summary\">\n");
        ReportContent += TEXT("<h2>Compile Log</h2>\n");
        ReportContent += FString::Printf(TEXT("<div class=\"log\">%s</div>\n"), *Result.OutputLog);
        ReportContent += TEXT("</div>\n");
    }

    ReportContent += TEXT("</body>\n</html>");

    // Ensure report directory exists
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!IFileManager::Get().DirectoryExists(*ReportDir))
    {
        IFileManager::Get().MakeDirectory(*ReportDir);
    }

    // Save report
    if (FFileHelper::SaveStringToFile(ReportContent, *ReportPath))
    {
        WriteLog(FString::Printf(TEXT("HTML report generated: %s"), *ReportPath), TEXT("SUCCESS"));
        return true;
    }
    else
    {
        WriteLog(FString::Printf(TEXT("Failed to generate report: %s"), *ReportPath), TEXT("ERROR"));
        return false;
    }
}

void UMingCompileAPI::CleanBuildArtifacts()
{
    WriteLog(TEXT("Cleaning build artifacts"), TEXT("INFO"));

    if (IFileManager::Get().DirectoryExists(*BuildPath))
    {
        IFileManager::Get().DeleteDirectory(*BuildPath, true, true);
        WriteLog(FString::Printf(TEXT("Cleaned build directory: %s"), *BuildPath), TEXT("INFO"));
    }

    // Recreate directories
    IFileManager::Get().MakeDirectory(*BuildPath, true);
    IFileManager::Get().MakeDirectory(*FPaths::Combine(*BuildPath, TEXT("Binaries")), true);
    IFileManager::Get().MakeDirectory(*FPaths::Combine(*BuildPath, TEXT("Intermediate")), true);

    WriteLog(TEXT("Build artifacts cleaned successfully"), TEXT("SUCCESS"));
}

bool UMingCompileAPI::ValidateProjectStructure(const FString& ProjectPath)
{
    WriteLog(FString::Printf(TEXT("Validating project structure: %s"), *ProjectPath), TEXT("INFO"));

    // Check for essential files and directories
    FString ProjectFile = FPaths::Combine(*ProjectPath, TEXT("MingGoRTS.uproject"));
    FString SourceDir = FPaths::Combine(*ProjectPath, TEXT("Source"));
    FString PluginsDir = FPaths::Combine(*ProjectPath, TEXT("Plugins"));
    FString ContentDir = FPaths::Combine(*ProjectPath, TEXT("Content"));

    bool bValid = true;

    if (!FPaths::FileExists(ProjectFile))
    {
        WriteLog(FString::Printf(TEXT("Missing project file: %s"), *ProjectFile), TEXT("ERROR"));
        bValid = false;
    }

    if (!IFileManager::Get().DirectoryExists(*SourceDir))
    {
        WriteLog(FString::Printf(TEXT("Missing source directory: %s"), *SourceDir), TEXT("ERROR"));
        bValid = false;
    }

    if (!IFileManager::Get().DirectoryExists(*PluginsDir))
    {
        WriteLog(FString::Printf(TEXT("Missing plugins directory: %s"), *PluginsDir), TEXT("WARNING"));
    }

    if (!IFileManager::Get().DirectoryExists(*ContentDir))
    {
        WriteLog(FString::Printf(TEXT("Missing content directory: %s"), *ContentDir), TEXT("WARNING"));
    }

    if (bValid)
    {
        WriteLog(TEXT("Project structure validation passed"), TEXT("SUCCESS"));
    }
    else
    {
        WriteLog(TEXT("Project structure validation failed"), TEXT("ERROR"));
    }

    return bValid;
}

bool UMingCompileAPI::ExecuteCompilation(const FMingCompileConfiguration& Configuration, FMingCompileResult& OutResult)
{
    FString BuildCommand = GetBuildCommand(Configuration);
    
    WriteLog(FString::Printf(TEXT("Executing build command: %s"), *BuildCommand), TEXT("INFO"));

    // Create process
    FString WorkingDirectory = Configuration.ProjectPath;
    CurrentProcess = new FProcHandle();
    
    // Execute build command
    void* PipeRead = nullptr;
    void* PipeWrite = nullptr;
    FPlatformProcess::CreatePipe(PipeRead, PipeWrite);
    
    *CurrentProcess = FPlatformProcess::CreateProc(*BuildCommand, nullptr, ECreateProcessFlags::None, nullptr, 
                                              *WorkingDirectory, PipeRead, PipeWrite, nullptr, nullptr, nullptr);

    if (!CurrentProcess->IsValid())
    {
        OutResult.Status = EMingCompileStatus::Failed;
        OutResult.ErrorMessage = TEXT("Failed to create compilation process");
        WriteLog(TEXT("Failed to create compilation process"), TEXT("ERROR"));
        return false;
    }

    // Read output
    FString OutputLog;
    while (CurrentProcess->IsValid())
    {
        FString Line = FPlatformProcess::ReadPipe(PipeRead);
        if (!Line.IsEmpty())
        {
            OutputLog += Line;
            
            // Parse for warnings and errors
            if (Line.Contains(TEXT("warning")))
            {
                OutResult.WarningCount++;
            }
            if (Line.Contains(TEXT("error")))
            {
                OutResult.ErrorCount++;
            }

            // Broadcast progress (simplified)
            OnCompileProgress.Broadcast(0.5f, TEXT("Compiling..."));
        }
        
        FPlatformProcess::Sleep(0.1f);
    }

    // Get exit code
    int32 ExitCode;
    FPlatformProcess::GetProcReturnCode(*CurrentProcess, ExitCode);
    
    // Clean up
    FPlatformProcess::CloseProc(*CurrentProcess);
    FPlatformProcess::ClosePipe(PipeRead, PipeWrite);
    delete CurrentProcess;
    CurrentProcess = nullptr;

    // Parse results
    ParseCompileLog(OutputLog, OutResult);
    OutResult.OutputLog = OutputLog;
    
    if (ExitCode == 0)
    {
        OutResult.Status = EMingCompileStatus::Success;
        return true;
    }
    else
    {
        OutResult.Status = EMingCompileStatus::Failed;
        OutResult.ErrorMessage = FString::Printf(TEXT("Compilation failed with exit code: %d"), ExitCode);
        return false;
    }
}

bool UMingCompileAPI::ExecuteTests(const FMingTestConfiguration& TestConfig, FMingTestResult& OutResult)
{
    FString TestCommand = GetTestCommand(TestConfig);
    
    WriteLog(FString::Printf(TEXT("Executing test command: %s"), *TestCommand), TEXT("INFO"));

    // Create process for tests
    FString WorkingDirectory = ProjectRoot;
    FProcHandle TestProcess;
    
    void* PipeRead = nullptr;
    void* PipeWrite = nullptr;
    FPlatformProcess::CreatePipe(PipeRead, PipeWrite);
    
    TestProcess = FPlatformProcess::CreateProc(*TestCommand, nullptr, ECreateProcessFlags::None, nullptr, 
                                           *WorkingDirectory, PipeRead, PipeWrite, nullptr, nullptr, nullptr);

    if (!TestProcess.IsValid())
    {
        OutResult.bSuccess = false;
        OutResult.TestReport = TEXT("Failed to create test process");
        WriteLog(TEXT("Failed to create test process"), TEXT("ERROR"));
        return false;
    }

    // Read test output
    FString TestOutput;
    float StartTime = FPlatformTime::Seconds();
    
    while (TestProcess.IsValid())
    {
        // Check timeout
        if (FPlatformTime::Seconds() - StartTime > TestConfig.TestTimeout)
        {
            WriteLog(TEXT("Test execution timeout"), TEXT("ERROR"));
            FPlatformProcess::TerminateProc(TestProcess);
            break;
        }

        FString Line = FPlatformProcess::ReadPipe(PipeRead);
        if (!Line.IsEmpty())
        {
            TestOutput += Line;
            OnTestProgress.Broadcast(0.5f, TEXT("Running tests..."));
        }
        
        FPlatformProcess::Sleep(0.1f);
    }

    // Get exit code
    int32 ExitCode;
    FPlatformProcess::GetProcReturnCode(TestProcess, ExitCode);
    
    // Clean up
    FPlatformProcess::CloseProc(TestProcess);
    FPlatformProcess::ClosePipe(PipeRead, PipeWrite);

    // Parse test results
    ParseTestResults(TestOutput, OutResult);
    OutResult.TestReport = TestOutput;
    
    OutResult.bSuccess = (ExitCode == 0);
    return OutResult.bSuccess;
}

void UMingCompileAPI::ParseCompileLog(const FString& LogContent, FMingCompileResult& Result)
{
    // Simple parsing for warnings and errors
    TArray<FString> Lines;
    LogContent.ParseIntoArrayLines(Lines);
    
    Result.WarningCount = 0;
    Result.ErrorCount = 0;
    
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("warning")))
        {
            Result.WarningCount++;
        }
        else if (Line.Contains(TEXT("error")))
        {
            Result.ErrorCount++;
        }
    }
}

void UMingCompileAPI::ParseTestResults(const FString& TestOutput, FMingTestResult& Result)
{
    // Simple parsing for test results
    TArray<FString> Lines;
    TestOutput.ParseIntoArrayLines(Lines);
    
    Result.TotalTests = 0;
    Result.PassedTests = 0;
    Result.FailedTests = 0;
    Result.SkippedTests = 0;
    
    for (const FString& Line : Lines)
    {
        if (Line.Contains(TEXT("PASSED")) || Line.Contains(TEXT("✔")))
        {
            Result.PassedTests++;
            Result.TotalTests++;
        }
        else if (Line.Contains(TEXT("FAILED")) || Line.Contains(TEXT("✗")))
        {
            Result.FailedTests++;
            Result.TotalTests++;
        }
        else if (Line.Contains(TEXT("SKIPPED")))
        {
            Result.SkippedTests++;
            Result.TotalTests++;
        }
    }
}

FString UMingCompileAPI::GetBuildCommand(const FMingCompileConfiguration& Configuration)
{
    FString Command;
    
    // Get Unreal Build Tool path
    FString UBTPath = FPaths::Combine(*FPaths::EngineDir(), TEXT("Build/BatchFiles/Build.bat"));
    
    Command = FString::Printf(TEXT("\"%s\""), *UBTPath);
    
    // Add project file
    FString ProjectFile = FPaths::Combine(*Configuration.ProjectPath, TEXT("MingGoRTS.uproject"));
    Command += FString::Printf(TEXT(" \"%s\""), *ProjectFile);
    
    // Add target
    switch (Configuration.Target)
    {
    case EMingCompileTarget::Editor:
        Command += TEXT(" MingGoRTSEditor");
        break;
    case EMingCompileTarget::Game:
        Command += TEXT(" MingGoRTS");
        break;
    case EMingCompileTarget::Server:
        Command += TEXT(" MingGoRTSServer");
        break;
    }
    
    // Add platform
    Command += TEXT(" Win64");
    
    // Add configuration
    switch (Configuration.CompileMode)
    {
    case EMingCompileMode::Debug:
        Command += TEXT(" Debug");
        break;
    case EMingCompileMode::Development:
        Command += TEXT(" Development");
        break;
    case EMingCompileMode::Shipping:
        Command += TEXT(" Shipping");
        break;
    case EMingCompileMode::Test:
        Command += TEXT(" Test");
        break;
    }
    
    // Add project directory
    Command += FString::Printf(TEXT(" -project=\"%s\""), *Configuration.ProjectPath);
    
    // Add output directory
    if (!Configuration.OutputPath.IsEmpty())
    {
        Command += FString::Printf(TEXT(" -out=\"%s\""), *Configuration.OutputPath);
    }
    
    // Add clean build flag
    if (Configuration.bCleanBuild)
    {
        Command += TEXT(" -clean");
    }
    
    // Add parallel jobs
    Command += FString::Printf(TEXT(" -parallel=%d"), Configuration.ParallelJobs);
    
    // Add additional arguments
    for (const FString& Arg : Configuration.AdditionalArguments)
    {
        Command += FString::Printf(TEXT(" %s"), *Arg);
    }
    
    return Command;
}

FString UMingCompileAPI::GetTestCommand(const FMingTestConfiguration& TestConfig)
{
    FString Command;
    
    // Get test executable path
    FString TestExe = FPaths::Combine(*FPaths::Combine(*BuildPath, TEXT("Binaries/Win64")), 
                                     TEXT("MingGoRTS.exe"));
    
    Command = FString::Printf(TEXT("\"%s\""), *TestExe);
    
    // Add test arguments
    Command += TEXT(" -runtests");
    
    if (TestConfig.bRunUnitTests)
    {
        Command += TEXT(" -unit");
    }
    
    if (TestConfig.bRunIntegrationTests)
    {
        Command += TEXT(" -integration");
    }
    
    if (TestConfig.bRunPerformanceTests)
    {
        Command += TEXT(" -performance");
    }
    
    if (TestConfig.bRunStressTests)
    {
        Command += TEXT(" -stress");
    }
    
    // Add timeout
    Command += FString::Printf(TEXT(" -timeout=%d"), TestConfig.TestTimeout);
    
    // Add output path
    if (!TestConfig.TestOutputPath.IsEmpty())
    {
        Command += FString::Printf(TEXT(" -output=\"%s\""), *TestConfig.TestOutputPath);
    }
    
    return Command;
}

void UMingCompileAPI::WriteLog(const FString& Message, const FString& Level)
{
    FString Timestamp = FDateTime::Now().ToString(TEXT("yyyy-MM-dd HH:mm:ss"));
    FString LogEntry = FString::Printf(TEXT("[%s] [%s] %s"), *Timestamp, *Level, *Message);
    
    // Print to console
    if (Level == TEXT("ERROR"))
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("WARNING"))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
    }
    else if (Level == TEXT("SUCCESS"))
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("%s"), *Message);
    }

    // Write to log file
    FString LogFileName = FString::Printf(TEXT("CompileAPI_%s.log"), *FDateTime::Now().ToString(TEXT("yyyyMMdd_HHmmss")));
    FString LogFilePath = FPaths::Combine(*LogPath, *LogFileName);
    
    // Ensure log directory exists
    if (!IFileManager::Get().DirectoryExists(*LogPath))
    {
        IFileManager::Get().MakeDirectory(*LogPath);
    }
    
    FFileHelper::SaveStringToFile(LogEntry + TEXT("\n"), *LogFilePath, FFileHelper::EEncodingOptions::ForceUTF8);
}
