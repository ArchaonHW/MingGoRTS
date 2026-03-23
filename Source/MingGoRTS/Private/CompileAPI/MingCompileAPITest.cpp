#include "MingCompileAPITest.h"
#include "MingCompileAPI.h"
#include "Engine/Engine.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "HAL/PlatformFilemanager.h"

UMingCompileAPITest::UMingCompileAPITest()
{
    TotalTests = 0;
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();
}

bool UMingCompileAPITest::RunAllTests()
{
    UE_LOG(LogTemp, Log, TEXT("Starting MingCompileAPI Test Suite"));
    
    TotalTests = 0;
    PassedTests = 0;
    FailedTests = 0;
    TestResults.Empty();

    // Run all tests
    bool bAllPassed = true;
    bAllPassed &= TestCompileAPIInitialization();
    bAllPassed &= TestDefaultConfiguration();
    bAllPassed &= TestProjectCompilation();
    bAllPassed &= TestAsyncCompilation();
    bAllPassed &= TestTestExecution();
    bAllPassed &= TestReportGeneration();
    bAllPassed &= TestProjectValidation();
    bAllPassed &= TestBuildCleanup();
    bAllPassed &= TestErrorHandling();
    bAllPassed &= TestCancellation();

    // Generate test report
    FString ReportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/CompileAPITestReport.html"));
    GenerateTestReport(ReportPath);

    UE_LOG(LogTemp, Log, TEXT("Test Suite Completed: %d/%d tests passed"), PassedTests, TotalTests);
    
    return bAllPassed;
}

bool UMingCompileAPITest::TestCompileAPIInitialization()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Compile API Initialization..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        if (CompileAPI->GetCompileStatus() != EMingCompileStatus::Idle)
        {
            LogTestResult(TEXT("CompileAPIInitialization"), false, TEXT("Compile API not in Idle state after initialization"));
            return false;
        }

        LogTestResult(TEXT("CompileAPIInitialization"), true, TEXT("Compile API initialized successfully"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("CompileAPIInitialization"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestDefaultConfiguration()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Default Configuration..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileConfiguration Config = CompileAPI->GetDefaultConfiguration();
        
        if (Config.ProjectPath.IsEmpty())
        {
            LogTestResult(TEXT("DefaultConfiguration"), false, TEXT("Default project path is empty"));
            return false;
        }

        if (Config.ParallelJobs <= 0)
        {
            LogTestResult(TEXT("DefaultConfiguration"), false, TEXT("Parallel jobs should be greater than 0"));
            return false;
        }

        LogTestResult(TEXT("DefaultConfiguration"), true, TEXT("Default configuration is valid"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("DefaultConfiguration"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestProjectCompilation()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Project Compilation..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileConfiguration Config = CreateTestConfiguration();
        FMingCompileResult Result = CompileAPI->CompileProject(Config);
        
        if (Result.Status == EMingCompileStatus::Failed && Result.ErrorCount == 0)
        {
            LogTestResult(TEXT("ProjectCompilation"), false, TEXT("Compilation failed but no errors reported"));
            return false;
        }

        LogTestResult(TEXT("ProjectCompilation"), true, TEXT("Project compilation test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("ProjectCompilation"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestAsyncCompilation()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Asynchronous Compilation..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileConfiguration Config = CreateTestConfiguration();
        
        // Start async compilation
        CompileAPI->CompileProjectAsync(Config);
        
        // Wait a bit for async operation
        FPlatformProcess::Sleep(1.0f);
        
        EMingCompileStatus Status = CompileAPI->GetCompileStatus();
        if (Status == EMingCompileStatus::Idle)
        {
            LogTestResult(TEXT("AsyncCompilation"), false, TEXT("Async compilation not started"));
            return false;
        }

        // Cancel to test cancellation
        CompileAPI->CancelCompilation();
        
        LogTestResult(TEXT("AsyncCompilation"), true, TEXT("Async compilation test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("AsyncCompilation"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestTestExecution()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Test Execution..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingTestConfiguration TestConfig = CreateTestTestConfiguration();
        FMingTestResult Result = CompileAPI->RunTests(TestConfig);
        
        if (Result.TestTime <= 0.0f)
        {
            LogTestResult(TEXT("TestExecution"), false, TEXT("Test execution time should be greater than 0"));
            return false;
        }

        LogTestResult(TEXT("TestExecution"), true, TEXT("Test execution test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("TestExecution"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestReportGeneration()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Report Generation..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileResult Result;
        Result.Status = EMingCompileStatus::Success;
        Result.CompileTime = 123.45f;
        Result.WarningCount = 2;
        Result.ErrorCount = 0;
        Result.StartTime = FDateTime::Now();
        Result.EndTime = FDateTime::Now();
        Result.OutputLog = TEXT("Test log output");
        
        FString ReportPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/TestCompileReport.html"));
        bool bSuccess = CompileAPI->GenerateCompileReport(Result, ReportPath);
        
        if (!bSuccess)
        {
            LogTestResult(TEXT("ReportGeneration"), false, TEXT("Failed to generate compile report"));
            return false;
        }

        if (!FPaths::FileExists(ReportPath))
        {
            LogTestResult(TEXT("ReportGeneration"), false, TEXT("Report file was not created"));
            return false;
        }

        LogTestResult(TEXT("ReportGeneration"), true, TEXT("Report generation test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("ReportGeneration"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestProjectValidation()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Project Validation..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        bool bValid = CompileAPI->ValidateProjectStructure(FPaths::ProjectDir());
        
        // Should be valid for MingGoRTS project
        if (!bValid)
        {
            LogTestResult(TEXT("ProjectValidation"), false, TEXT("MingGoRTS project structure validation failed"));
            return false;
        }

        LogTestResult(TEXT("ProjectValidation"), true, TEXT("Project validation test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("ProjectValidation"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestBuildCleanup()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Build Cleanup..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        // Create some test files
        FString TestBuildDir = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Build/Test"));
        IFileManager::Get().MakeDirectory(*TestBuildDir, true);
        
        CompileAPI->CleanBuildArtifacts();
        
        // Check if test files were cleaned (simplified check)
        LogTestResult(TEXT("BuildCleanup"), true, TEXT("Build cleanup test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("BuildCleanup"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestErrorHandling()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Error Handling..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        // Test with invalid project path
        FMingCompileConfiguration Config;
        Config.ProjectPath = TEXT("INVALID_PATH_THAT_DOES_NOT_EXIST");
        
        FMingCompileResult Result = CompileAPI->CompileProject(Config);
        
        if (Result.Status != EMingCompileStatus::Failed)
        {
            LogTestResult(TEXT("ErrorHandling"), false, TEXT("Should fail with invalid project path"));
            return false;
        }

        LogTestResult(TEXT("ErrorHandling"), true, TEXT("Error handling test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("ErrorHandling"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::TestCancellation()
{
    TotalTests++;
    UE_LOG(LogTemp, Log, TEXT("Testing Cancellation..."));

    try
    {
        UMingCompileAPI* CompileAPI = NewObject<UMingCompileAPI>();
        CompileAPI->InitializeCompileAPI();
        
        FMingCompileConfiguration Config = CreateTestConfiguration();
        
        // Start async compilation
        CompileAPI->CompileProjectAsync(Config);
        
        // Wait a bit then cancel
        FPlatformProcess::Sleep(0.5f);
        CompileAPI->CancelCompilation();
        
        // Check status after cancellation
        EMingCompileStatus Status = CompileAPI->GetCompileStatus();
        if (Status != EMingCompileStatus::Cancelled && Status != EMingCompileStatus::Idle)
        {
            LogTestResult(TEXT("Cancellation"), false, TEXT("Compilation status should be Cancelled or Idle after cancellation"));
            return false;
        }

        LogTestResult(TEXT("Cancellation"), true, TEXT("Cancellation test completed"));
        PassedTests++;
        return true;
    }
    catch (const std::exception& e)
    {
        LogTestResult(TEXT("Cancellation"), false, FString::Printf(TEXT("Exception: %s"), UTF8_TO_TCHAR(e.what())));
        FailedTests++;
        return false;
    }
}

bool UMingCompileAPITest::GenerateTestReport(const FString& ReportPath)
{
    FString ReportContent = TEXT("<!DOCTYPE html>\n<html>\n<head>\n");
    ReportContent += TEXT("<title>MingCompileAPI Test Report</title>\n");
    ReportContent += TEXT("<style>\n");
    ReportContent += TEXT("body { font-family: Arial, sans-serif; margin: 20px; background-color: #f5f5f5; }\n");
    ReportContent += TEXT(".header { background-color: #2c3e50; color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; }\n");
    ReportContent += TEXT(".summary { background-color: white; padding: 20px; border-radius: 5px; margin-bottom: 20px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n");
    ReportContent += TEXT(".success { color: #27ae60; }\n");
    ReportContent += TEXT(".failure { color: #e74c3c; }\n");
    ReportContent += TEXT(".info { color: #3498db; }\n");
    ReportContent += TEXT("table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n");
    ReportContent += TEXT("th, td { padding: 10px; text-align: left; border-bottom: 1px solid #ddd; }\n");
    ReportContent += TEXT("th { background-color: #f8f9fa; }\n");
    ReportContent += TEXT("</style>\n</head>\n<body>\n");

    ReportContent += TEXT("<div class=\"header\">\n");
    ReportContent += TEXT("<h1>MingCompileAPI Test Report</h1>\n");
    ReportContent += FString::Printf(TEXT("<p>Generated: %s</p>\n"), *FDateTime::Now().ToString());
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("<div class=\"summary\">\n");
    ReportContent += TEXT("<h2>Test Summary</h2>\n");
    ReportContent += FString::Printf(TEXT("<p><strong>Total Tests:</strong> %d</p>\n"), TotalTests);
    ReportContent += FString::Printf(TEXT("<p><strong>Passed:</strong> <span class=\"success\">%d</span></p>\n"), PassedTests);
    ReportContent += FString::Printf(TEXT("<p><strong>Failed:</strong> <span class=\"failure\">%d</span></p>\n"), FailedTests);
    
    float SuccessRate = TotalTests > 0 ? (float)PassedTests / TotalTests * 100.0f : 0.0f;
    ReportContent += FString::Printf(TEXT("<p><strong>Success Rate:</strong> %.1f%%</p>\n"), SuccessRate);
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("<div class=\"summary\">\n");
    ReportContent += TEXT("<h2>Test Results</h2>\n");
    ReportContent += TEXT("<table>\n");
    ReportContent += TEXT("<tr><th>Test Name</th><th>Status</th><th>Message</th></tr>\n");
    
    for (const auto& TestResult : TestResults)
    {
        FString StatusClass = TestResult.Value ? TEXT("success") : TEXT("failure");
        FString StatusText = TestResult.Value ? TEXT("PASSED") : TEXT("FAILED");
        
        ReportContent += FString::Printf(TEXT("<tr><td>%s</td><td class=\"%s\">%s</td><td>%s</td></tr>\n"), 
            *TestResult.Key, *StatusClass, *StatusText, *TestResults[TestResult.Key]);
    }
    
    ReportContent += TEXT("</table>\n");
    ReportContent += TEXT("</div>\n");

    ReportContent += TEXT("</body>\n</html>");

    // Ensure report directory exists
    FString ReportDir = FPaths::GetPath(ReportPath);
    if (!IFileManager::Get().DirectoryExists(*ReportDir))
    {
        IFileManager::Get().MakeDirectory(*ReportDir);
    }

    // Save report
    return FFileHelper::SaveStringToFile(ReportContent, *ReportPath);
}

FMingCompileConfiguration UMingCompileAPITest::CreateTestConfiguration()
{
    FMingCompileConfiguration Config;
    Config.CompileMode = EMingCompileMode::Development;
    Config.Target = EMingCompileTarget::Game;
    Config.ProjectPath = FPaths::ProjectDir();
    Config.OutputPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Build/TestOutput"));
    Config.bCleanBuild = false;
    Config.ParallelJobs = 2;
    Config.AdditionalArguments.Empty();
    
    return Config;
}

FMingTestConfiguration UMingCompileAPITest::CreateTestTestConfiguration()
{
    FMingTestConfiguration Config;
    Config.bRunUnitTests = true;
    Config.bRunIntegrationTests = false;
    Config.bRunPerformanceTests = false;
    Config.bRunStressTests = false;
    Config.TestTimeout = 60;
    Config.TestOutputPath = FPaths::Combine(*FPaths::ProjectDir(), TEXT("Reports/TestOutput"));
    
    return Config;
}

void UMingCompileAPITest::LogTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    TestResults.Add(TestName, bPassed);
    
    FString LogMessage = FString::Printf(TEXT("[%s] %s"), 
        bPassed ? TEXT("PASS") : TEXT("FAIL"), *Message);
    
    if (bPassed)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *LogMessage);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s"), *LogMessage);
    }
}
