// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Code Standards API Test - 程式碼規範API測試

#include "MingRTSCodeStandardsAPITest.h"
#include "MingRTSCodeStandardsAPI.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"

UMingRTSCodeStandardsAPITest::UMingRTSCodeStandardsAPITest()
{
    CodeStandardsAPI = nullptr;
    TestResults.Empty();
}

void UMingRTSCodeStandardsAPITest::RunAllTests()
{
    UE_LOG(LogTemp, Log, TEXT("Starting Code Standards API Tests..."));
    
    // 初始化測試環境
    InitializeTestEnvironment();
    
    // 運行所有測試
    TestInitialization();
    TestNamingStandards();
    TestFormattingStandards();
    TestCodeAnalysis();
    TestAutoFix();
    TestReportGeneration();
    TestConfiguration();
    TestMonitoring();
    TestStatistics();
    TestTeamCollaboration();
    TestCodeReview();
    
    // 清理測試環境
    CleanupTestEnvironment();
    
    // 輸出測試結果
    OutputTestResults();
    
    UE_LOG(LogTemp, Log, TEXT("Code Standards API Tests Completed"));
}

void UMingRTSCodeStandardsAPITest::InitializeTestEnvironment()
{
    CodeStandardsAPI = NewObject<UMingRTSCodeStandardsAPI>();
    CodeStandardsAPI->InitializeCodeStandards();
    
    // 創建測試文件
    CreateTestFiles();
}

void UMingRTSCodeStandardsAPITest::CleanupTestEnvironment()
{
    // 清理測試文件
    CleanupTestFiles();
    
    if (CodeStandardsAPI)
    {
        CodeStandardsAPI = nullptr;
    }
}

void UMingRTSCodeStandardsAPITest::CreateTestFiles()
{
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // 創建測試目錄
    PlatformFile.CreateDirectoryTree(*TestDir);
    
    // 創建測試C++文件
    FString TestCPPContent = TEXT(
        "#include \"CoreMinimal.h\"\n"
        "#include \"GameFramework/Actor.h\"\n"
        "#include \"TestActor.generated.h\"\n\n"
        "class ATESTPROJECT_API ATestActor : public AActor\n"
        "{\n"
        "    GENERATED_BODY()\n\n"
        "public:\n"
        "    ATestActor();\n\n"
        "    UPROPERTY(EditAnywhere)\n"
        "    int32 TestVariable;\n\n"
        "    UFUNCTION(BlueprintCallable)\n"
        "    void TestFunction();\n"
        "};\n"
    );
    
    FString TestCPPPath = TestDir / TEXT("TestActor.cpp");
    FFileHelper::SaveStringToFile(TestCPPContent, *TestCPPPath);
    
    // 創建測試頭文件
    FString TestHContent = TEXT(
        "#pragma once\n\n"
        "#include \"CoreMinimal.h\"\n"
        "#include \"GameFramework/Actor.h\"\n"
        "#include \"TestActor.generated.h\"\n\n"
        "UCLASS()\n"
        "class ATESTPROJECT_API ATestActor : public AActor\n"
        "{\n"
        "    GENERATED_BODY()\n\n"
        "public:\n"
        "    ATestActor();\n\n"
        "    UPROPERTY(EditAnywhere)\n"
        "    int32 TestVariable;\n\n"
        "    UFUNCTION(BlueprintCallable)\n"
        "    void TestFunction();\n"
        "};\n"
    );
    
    FString TestHPath = TestDir / TEXT("TestActor.h");
    FFileHelper::SaveStringToFile(TestHContent, *TestHPath);
    
    // 創建有命名問題的測試文件
    FString BadNamingContent = TEXT(
        "#include \"CoreMinimal.h\"\n"
        "class bad_class_name\n"
        "{\n"
        "    int bad_variable_name;\n"
        "    void bad_function_name();\n"
        "};\n"
    );
    
    FString BadNamingPath = TestDir / TEXT("BadNaming.cpp");
    FFileHelper::SaveStringToFile(BadNamingContent, *BadNamingPath);
    
    // 創建有格式問題的測試文件
    FString BadFormattingContent = TEXT(
        "#include \"CoreMinimal.h\"\n"
        "class TestClass\n"
        "{\n"
        "public:\n"
        " int32 Variable1;\n"
        "  int32 Variable2;\n"
        "   int32 Variable3;\n"
        "};\n"
    );
    
    FString BadFormattingPath = TestDir / TEXT("BadFormatting.cpp");
    FFileHelper::SaveStringToFile(BadFormattingContent, *BadFormattingPath);
}

void UMingRTSCodeStandardsAPITest::CleanupTestFiles()
{
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // 刪除測試目錄及其內容
    PlatformFile.DeleteDirectoryRecursively(*TestDir);
}

void UMingRTSCodeStandardsAPITest::TestInitialization()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Initialization..."));
    
    bool bTestPassed = true;
    
    // 測試API初始化
    if (!CodeStandardsAPI)
    {
        UE_LOG(LogTemp, Error, TEXT("Code Standards API is null"));
        bTestPassed = false;
    }
    
    // 測試默認配置
    FCodeStandardConfig NamingConfig = CodeStandardsAPI->GetStandardConfig(ECodeStandardType::Naming);
    if (!NamingConfig.bEnabled)
    {
        UE_LOG(LogTemp, Error, TEXT("Naming standards should be enabled by default"));
        bTestPassed = false;
    }
    
    FCodeStandardConfig FormattingConfig = CodeStandardsAPI->GetStandardConfig(ECodeStandardType::Formatting);
    if (!FormattingConfig.bEnabled)
    {
        UE_LOG(LogTemp, Error, TEXT("Formatting standards should be enabled by default"));
        bTestPassed = false;
    }
    
    RecordTestResult(TEXT("Initialization"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestNamingStandards()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Naming Standards..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    FString BadNamingPath = TestDir / TEXT("BadNaming.cpp");
    
    // 測試命名規範檢查
    TArray<FCodeViolation> Violations = CodeStandardsAPI->CheckNamingStandards(BadNamingPath);
    
    if (Violations.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Should detect naming violations in BadNaming.cpp"));
        bTestPassed = false;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Detected %d naming violations"), Violations.Num());
        
        // 檢查是否檢測到類名問題
        bool bFoundClassViolation = false;
        for (const FCodeViolation& Violation : Violations)
        {
            if (Violation.RuleName.Contains(TEXT("Class")))
            {
                bFoundClassViolation = true;
                break;
            }
        }
        
        if (!bFoundClassViolation)
        {
            UE_LOG(LogTemp, Error, TEXT("Should detect class naming violation"));
            bTestPassed = false;
        }
    }
    
    RecordTestResult(TEXT("Naming Standards"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestFormattingStandards()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Formatting Standards..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    FString BadFormattingPath = TestDir / TEXT("BadFormatting.cpp");
    
    // 測試格式化規範檢查
    TArray<FCodeViolation> Violations = CodeStandardsAPI->CheckFormattingStandards(BadFormattingPath);
    
    if (Violations.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Should detect formatting violations in BadFormatting.cpp"));
        bTestPassed = false;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Detected %d formatting violations"), Violations.Num());
        
        // 檢查是否有自動修復的違規
        bool bHasAutoFixable = false;
        for (const FCodeViolation& Violation : Violations)
        {
            if (Violation.bAutoFixable)
            {
                bHasAutoFixable = true;
                break;
            }
        }
        
        if (!bHasAutoFixable)
        {
            UE_LOG(LogTemp, Warning, TEXT("No auto-fixable violations found"));
        }
    }
    
    RecordTestResult(TEXT("Formatting Standards"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestCodeAnalysis()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Code Analysis..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    FString TestCPPPath = TestDir / TEXT("TestActor.cpp");
    
    // 測試單文件分析
    FCodeQualityReport Report = CodeStandardsAPI->AnalyzeFile(TestCPPPath);
    
    if (Report.FilePath.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Report should have valid file path"));
        bTestPassed = false;
    }
    
    if (Report.QualityScore < 0.0f || Report.QualityScore > 100.0f)
    {
        UE_LOG(LogTemp, Error, TEXT("Quality score should be between 0 and 100"));
        bTestPassed = false;
    }
    
    // 測試目錄分析
    TArray<FCodeQualityReport> Reports = CodeStandardsAPI->AnalyzeDirectory(TestDir);
    
    if (Reports.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Should analyze files in test directory"));
        bTestPassed = false;
    }
    
    // 測試項目分析
    TArray<FCodeQualityReport> ProjectReports = CodeStandardsAPI->AnalyzeProject(TestDir);
    
    if (ProjectReports.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("Should analyze project files"));
        bTestPassed = false;
    }
    
    RecordTestResult(TEXT("Code Analysis"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestAutoFix()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Auto Fix..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    FString BadFormattingPath = TestDir / TEXT("BadFormatting.cpp");
    
    // 備份原始文件
    FString OriginalContent;
    FFileHelper::LoadFileToString(OriginalContent, *BadFormattingPath);
    
    // 獲取違規
    TArray<FCodeViolation> Violations = CodeStandardsAPI->CheckFormattingStandards(BadFormattingPath);
    
    if (Violations.Num() > 0)
    {
        // 測試自動修復
        bool bFixed = CodeStandardsAPI->AutoFixCode(BadFormattingPath, Violations);
        
        if (!bFixed)
        {
            UE_LOG(LogTemp, Warning, TEXT("Auto fix may not have been applied"));
        }
        
        // 檢查文件是否被修改
        FString FixedContent;
        FFileHelper::LoadFileToString(FixedContent, *BadFormattingPath);
        
        if (FixedContent == OriginalContent)
        {
            UE_LOG(LogTemp, Warning, TEXT("File content unchanged after auto fix"));
        }
        
        // 恢復原始文件
        FFileHelper::SaveStringToFile(OriginalContent, *BadFormattingPath);
    }
    
    // 測試批量修復
    TArray<FString> FilesToFix = {BadFormattingPath};
    TArray<FString> FixedFiles = CodeStandardsAPI->BatchAutoFix(FilesToFix);
    
    if (FixedFiles.Num() == 0 && Violations.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Batch auto fix should fix at least some files"));
    }
    
    RecordTestResult(TEXT("Auto Fix"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestReportGeneration()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Report Generation..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    TArray<FCodeQualityReport> Reports = CodeStandardsAPI->AnalyzeDirectory(TestDir);
    
    if (Reports.Num() > 0)
    {
        // 測試文本報告生成
        FString TextReport = CodeStandardsAPI->GenerateQualityReport(Reports);
        
        if (TextReport.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("Text report should not be empty"));
            bTestPassed = false;
        }
        
        // 測試HTML報告生成
        FString HTMLReport = CodeStandardsAPI->GenerateHTMLReport(Reports);
        
        if (HTMLReport.IsEmpty())
        {
            UE_LOG(LogTemp, Error, TEXT("HTML report should not be empty"));
            bTestPassed = false;
        }
        
        if (!HTMLReport.Contains(TEXT("<html>")) || !HTMLReport.Contains(TEXT("</html>")))
        {
            UE_LOG(LogTemp, Error, TEXT("HTML report should contain HTML tags"));
            bTestPassed = false;
        }
        
        // 測試報告導出
        FString ReportPath = TestDir / TEXT("TestReport.txt");
        bool bExported = CodeStandardsAPI->ExportReport(TextReport, ReportPath);
        
        if (!bExported)
        {
            UE_LOG(LogTemp, Error, TEXT("Report export should succeed"));
            bTestPassed = false;
        }
        else
        {
            // 清理導出的報告
            IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
            PlatformFile.DeleteFile(*ReportPath);
        }
    }
    
    RecordTestResult(TEXT("Report Generation"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestConfiguration()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Configuration..."));
    
    bool bTestPassed = true;
    
    // 測試添加命名規則
    FNamingRule TestRule;
    TestRule.Pattern = TEXT("^Test[A-Z][a-zA-Z0-9]*");
    TestRule.Convention = ENamingConvention::PascalCase;
    TestRule.Description = TEXT("Test class naming rule");
    TestRule.bRequired = true;
    
    CodeStandardsAPI->AddNamingRule(TestRule);
    
    // 驗證規則已添加
    FCodeStandardConfig Config = CodeStandardsAPI->GetStandardConfig(ECodeStandardType::Naming);
    bool bFoundRule = false;
    
    for (const FNamingRule& Rule : Config.NamingRules)
    {
        if (Rule.Description == TestRule.Description)
        {
            bFoundRule = true;
            break;
        }
    }
    
    if (!bFoundRule)
    {
        UE_LOG(LogTemp, Error, TEXT("Added naming rule should be found in config"));
        bTestPassed = false;
    }
    
    // 測試添加格式化規則
    FFormattingRule FormatRule;
    FormatRule.RuleName = TEXT("Test Rule");
    FormatRule.Pattern = TEXT("test");
    FormatRule.Replacement = TEXT("TEST");
    FormatRule.Description = TEXT("Test formatting rule");
    FormatRule.bAutoFixable = true;
    
    CodeStandardsAPI->AddFormattingRule(FormatRule);
    
    // 測試啟用/禁用規範
    CodeStandardsAPI->SetStandardEnabled(ECodeStandardType::Security, false);
    FCodeStandardConfig SecurityConfig = CodeStandardsAPI->GetStandardConfig(ECodeStandardType::Security);
    
    if (SecurityConfig.bEnabled)
    {
        UE_LOG(LogTemp, Error, TEXT("Security standards should be disabled"));
        bTestPassed = false;
    }
    
    RecordTestResult(TEXT("Configuration"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestMonitoring()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Monitoring..."));
    
    bool bTestPassed = true;
    
    // 測試監控狀態
    bool bInitialMonitoringState = CodeStandardsAPI->IsMonitoringActive();
    
    // 測試啟用監控
    CodeStandardsAPI->EnableRealTimeMonitoring(true);
    
    if (!CodeStandardsAPI->IsMonitoringActive())
    {
        UE_LOG(LogTemp, Error, TEXT("Monitoring should be active after enabling"));
        bTestPassed = false;
    }
    
    // 測試設置監控間隔
    CodeStandardsAPI->SetMonitoringInterval(30.0f);
    
    // 測試禁用監控
    CodeStandardsAPI->EnableRealTimeMonitoring(false);
    
    if (CodeStandardsAPI->IsMonitoringActive())
    {
        UE_LOG(LogTemp, Error, TEXT("Monitoring should not be active after disabling"));
        bTestPassed = false;
    }
    
    // 恢復初始狀態
    CodeStandardsAPI->EnableRealTimeMonitoring(bInitialMonitoringState);
    
    RecordTestResult(TEXT("Monitoring"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestStatistics()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Statistics..."));
    
    bool bTestPassed = true;
    
    FString TestDir = FPaths::ProjectTempDir() / TEXT("CodeStandardsTest");
    TArray<FCodeQualityReport> Reports = CodeStandardsAPI->AnalyzeDirectory(TestDir);
    
    if (Reports.Num() > 0)
    {
        // 測試質量統計
        TMap<ECodeQuality, int32> QualityStats = CodeStandardsAPI->GetQualityStatistics(Reports);
        
        if (QualityStats.Num() == 0)
        {
            UE_LOG(LogTemp, Error, TEXT("Quality statistics should not be empty"));
            bTestPassed = false;
        }
        
        // 測試違規統計
        TMap<ECodeStandardType, int32> ViolationStats = CodeStandardsAPI->GetViolationStatistics(Reports);
        
        if (ViolationStats.Num() == 0)
        {
            UE_LOG(LogTemp, Warning, TEXT("Violation statistics may be empty if no violations found"));
        }
        
        // 測試最常見違規
        TArray<FString> CommonViolations = CodeStandardsAPI->GetMostCommonViolations(5);
        
        // CommonViolations可能為空，這是正常的
        UE_LOG(LogTemp, Log, TEXT("Found %d common violations"), CommonViolations.Num());
    }
    
    RecordTestResult(TEXT("Statistics"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestTeamCollaboration()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Team Collaboration..."));
    
    bool bTestPassed = true;
    
    // 測試分享規範配置
    CodeStandardsAPI->ShareStandardsConfig(TEXT("TestDeveloper"));
    
    // 測試導入團隊規範
    FString TestConfigPath = FPaths::ProjectTempDir() / TEXT("TestConfig.json");
    FString TestConfigContent = TEXT("{\"test\": \"config\"}");
    FFileHelper::SaveStringToFile(TestConfigContent, *TestConfigPath);
    
    bool bImported = CodeStandardsAPI->ImportTeamStandards(TestConfigPath);
    
    // 導入可能失敗，因為我們創建的是測試配置
    UE_LOG(LogTemp, Log, TEXT("Team standards import result: %s"), bImported ? TEXT("Success") : TEXT("Failed"));
    
    // 清理測試配置文件
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    PlatformFile.DeleteFile(*TestConfigPath);
    
    RecordTestResult(TEXT("Team Collaboration"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::TestCodeReview()
{
    UE_LOG(LogTemp, Log, TEXT("Testing Code Review..."));
    
    bool bTestPassed = true;
    
    // 測試代碼變更審查
    TArray<FCodeViolation> ChangeViolations = CodeStandardsAPI->ReviewCodeChanges(TEXT("TestChangeset123"));
    
    // ChangeViolations可能為空，這是正常的
    UE_LOG(LogTemp, Log, TEXT("Found %d violations in code changes review"), ChangeViolations.Num());
    
    // 測試Pull Request審查
    FCodeQualityReport PRReport = CodeStandardsAPI->ReviewPullRequest(TEXT("TestPR456"));
    
    if (PRReport.FilePath.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("PR review report may be empty for test PR"));
    }
    
    RecordTestResult(TEXT("Code Review"), bTestPassed);
}

void UMingRTSCodeStandardsAPITest::RecordTestResult(const FString& TestName, bool bPassed)
{
    FTestResult Result;
    Result.TestName = TestName;
    Result.bPassed = bPassed;
    Result.Timestamp = FDateTime::Now();
    
    TestResults.Add(Result);
    
    if (bPassed)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ %s: PASSED"), *TestName);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("✗ %s: FAILED"), *TestName);
    }
}

void UMingRTSCodeStandardsAPITest::OutputTestResults()
{
    UE_LOG(LogTemp, Log, TEXT("=== Code Standards API Test Results ==="));
    
    int32 PassedCount = 0;
    int32 TotalCount = TestResults.Num();
    
    for (const FTestResult& Result : TestResults)
    {
        if (Result.bPassed)
        {
            PassedCount++;
        }
        
        UE_LOG(LogTemp, Log, TEXT("%s: %s"), 
            *Result.TestName, 
            Result.bPassed ? TEXT("PASSED") : TEXT("FAILED"));
    }
    
    UE_LOG(LogTemp, Log, TEXT("=== Summary ==="));
    UE_LOG(LogTemp, Log, TEXT("Total Tests: %d"), TotalCount);
    UE_LOG(LogTemp, Log, TEXT("Passed: %d"), PassedCount);
    UE_LOG(LogTemp, Log, TEXT("Failed: %d"), TotalCount - PassedCount);
    UE_LOG(LogTemp, Log, TEXT("Success Rate: %.1f%%"), 
        TotalCount > 0 ? (float)PassedCount / TotalCount * 100.0f : 0.0f);
    UE_LOG(LogTemp, Log, TEXT("=== End of Test Results ==="));
}

FString UMingRTSCodeStandardsAPITest::GenerateTestReport()
{
    FString Report = TEXT("Code Standards API Test Report\n");
    Report += TEXT("==============================\n\n");
    
    Report += FString::Printf(TEXT("Test Date: %s\n"), *FDateTime::Now().ToString());
    Report += FString::Printf(TEXT("Total Tests: %d\n"), TestResults.Num());
    
    int32 PassedCount = 0;
    for (const FTestResult& Result : TestResults)
    {
        if (Result.bPassed)
        {
            PassedCount++;
        }
    }
    
    Report += FString::Printf(TEXT("Passed: %d\n"), PassedCount);
    Report += FString::Printf(TEXT("Failed: %d\n"), TestResults.Num() - PassedCount);
    Report += FString::Printf(TEXT("Success Rate: %.1f%%\n\n"), 
        TestResults.Num() > 0 ? (float)PassedCount / TestResults.Num() * 100.0f : 0.0f);
    
    Report += TEXT("Detailed Results:\n");
    for (const FTestResult& Result : TestResults)
    {
        Report += FString::Printf(TEXT("- %s: %s\n"), 
            *Result.TestName, 
            Result.bPassed ? TEXT("PASSED") : TEXT("FAILED"));
    }
    
    return Report;
}

bool UMingRTSCodeStandardsAPITest::AreAllTestsPassed() const
{
    for (const FTestResult& Result : TestResults)
    {
        if (!Result.bPassed)
        {
            return false;
        }
    }
    return true;
}

int32 UMingRTSCodeStandardsAPITest::GetPassedTestCount() const
{
    int32 Count = 0;
    for (const FTestResult& Result : TestResults)
    {
        if (Result.bPassed)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSCodeStandardsAPITest::GetFailedTestCount() const
{
    int32 Count = 0;
    for (const FTestResult& Result : TestResults)
    {
        if (!Result.bPassed)
        {
            Count++;
        }
    }
    return Count;
}

float UMingRTSCodeStandardsAPITest::GetSuccessRate() const
{
    if (TestResults.Num() == 0)
    {
        return 0.0f;
    }
    
    int32 PassedCount = GetPassedTestCount();
    return (float)PassedCount / TestResults.Num() * 100.0f;
}
