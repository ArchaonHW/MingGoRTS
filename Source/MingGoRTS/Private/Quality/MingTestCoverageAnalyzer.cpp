// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Test Coverage Analyzer Implementation - B3-2

#include "Quality/MingTestCoverageAnalyzer.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogTestCoverage, Log, All);

UMingTestCoverageAnalyzer::UMingTestCoverageAnalyzer()
    : bIsMonitoring(false)
{
}

void UMingTestCoverageAnalyzer::InitializeAnalyzer()
{
    UE_LOG(LogTestCoverage, Log, TEXT("Test Coverage Analyzer initialized"));

    // Set default coverage targets
    FCoverageTarget LineCoverage;
    LineCoverage.Type = ECoverageType::Line;
    LineCoverage.TargetPercent = 85.0f;
    LineCoverage.MinimumPercent = 70.0f;
    CoverageTargets.Add(ECoverageType::Line, LineCoverage);

    FCoverageTarget BranchCoverage;
    BranchCoverage.Type = ECoverageType::Branch;
    BranchCoverage.TargetPercent = 80.0f;
    BranchCoverage.MinimumPercent = 65.0f;
    CoverageTargets.Add(ECoverageType::Branch, BranchCoverage);
}

void UMingTestCoverageAnalyzer::ShutdownAnalyzer()
{
    StopContinuousMonitoring();
    UE_LOG(LogTestCoverage, Log, TEXT("Test Coverage Analyzer shutdown"));
}

void UMingTestCoverageAnalyzer::SetCoverageTarget(const FCoverageTarget& Target)
{
    CoverageTargets.Add(Target.Type, Target);
    UE_LOG(LogTestCoverage, Log, TEXT("Set coverage target for %s: %.1f%% (min: %.1f%%)"),
        *UEnum::GetValueAsString(Target.Type), Target.TargetPercent, Target.MinimumPercent);
}

FCoverageData UMingTestCoverageAnalyzer::AnalyzeFileCoverage(const FString& FilePath, ECoverageType Type)
{
    FCoverageData Data;
    Data.FilePath = FilePath;
    Data.Type = Type;

    UE_LOG(LogTestCoverage, Log, TEXT("Analyzing coverage for %s"), *FilePath);

    // Simulate coverage analysis
    Data.TotalLines = 100;
    Data.CoveredLines = FMath::RandRange(70, 95);
    Data.PartialLines = FMath::RandRange(0, 10);
    Data.UncoveredLines = Data.TotalLines - Data.CoveredLines - Data.PartialLines;
    Data.CoveragePercent = static_cast<float>(Data.CoveredLines) / Data.TotalLines * 100.0f;

    // Simulate covered/uncovered line numbers
    for (int32 i = 1; i <= Data.TotalLines; ++i)
    {
        if (i <= Data.CoveredLines)
        {
            Data.CoveredLineNumbers.Add(i);
        }
        else if (i > Data.CoveredLines + Data.PartialLines)
        {
            Data.UncoveredLineNumbers.Add(i);
        }
    }

    UpdateCoverageData(Data);

    UE_LOG(LogTestCoverage, Log, TEXT("Coverage for %s: %.1f%%"), *FilePath, Data.CoveragePercent);

    return Data;
}

TArray<FCoverageData> UMingTestCoverageAnalyzer::AnalyzeProjectCoverage(const FString& ProjectPath)
{
    TArray<FCoverageData> AllCoverage;

    // Find all source files
    TArray<FString> SourceFiles;
    IFileManager::Get().FindFilesRecursive(SourceFiles, *ProjectPath, TEXT("*.cpp"), true, false);

    UE_LOG(LogTestCoverage, Log, TEXT("Analyzing project coverage: %s (%d files)"), *ProjectPath, SourceFiles.Num());

    for (const auto& File : SourceFiles)
    {
        FCoverageData LineData = AnalyzeFileCoverage(File, ECoverageType::Line);
        FCoverageData BranchData = AnalyzeFileCoverage(File, ECoverageType::Branch);
        FCoverageData FunctionData = AnalyzeFileCoverage(File, ECoverageType::Function);

        AllCoverage.Add(LineData);
        AllCoverage.Add(BranchData);
        AllCoverage.Add(FunctionData);
    }

    CheckCoverageTargets();

    return AllCoverage;
}

float UMingTestCoverageAnalyzer::CalculateCoveragePercent(const FCoverageData& Data) const
{
    if (Data.TotalLines == 0)
    {
        return 0.0f;
    }

    return static_cast<float>(Data.CoveredLines) / Data.TotalLines * 100.0f;
}

bool UMingTestCoverageAnalyzer::RunTests(const FTestSuiteConfig& Config)
{
    UE_LOG(LogTestCoverage, Log, TEXT("Running test suite: %s"), *Config.SuiteName);

    TArray<FString> Tests = DiscoverTests(Config);

    UE_LOG(LogTestCoverage, Log, TEXT("Discovered %d tests"), Tests.Num());

    for (const auto& Test : Tests)
    {
        for (const auto& Type : Config.IncludedTypes)
        {
            RunTest(Test, Type);
        }
    }

    // Check if all tests passed
    for (const auto& Result : TestResults)
    {
        if (Result.Status == ETestStatus::Failed || Result.Status == ETestStatus::Error)
        {
            if (Config.bFailFast)
            {
                return false;
            }
        }
    }

    return true;
}

bool UMingTestCoverageAnalyzer::RunTest(const FString& TestName, ETestType Type)
{
    UE_LOG(LogTestCoverage, Verbose, TEXT("Running test: %s (%s)"), *TestName, *UEnum::GetValueAsString(Type));

    FTestResult Result;
    Result.TestID = FName(*TestName);
    Result.TestName = TestName;
    Result.Type = Type;
    Result.Status = ETestStatus::Running;
    Result.Timestamp = FPlatformTime::Seconds();

    // Simulate test execution
    bool bPassed = FMath::RandBool();

    Result.Duration = FMath::RandRange(0.1f, 5.0f);
    Result.Status = bPassed ? ETestStatus::Passed : ETestStatus::Failed;
    Result.Assertions = FMath::RandRange(1, 20);
    Result.FailedAssertions = bPassed ? 0 : FMath::RandRange(1, 5);

    if (!bPassed)
    {
        Result.ErrorMessage = TEXT("Test assertion failed");
        Result.StackTrace = TEXT("at line 42");
    }

    StoreTestResult(Result);
    OnTestCompleted.Broadcast(Result);

    return bPassed;
}

TArray<FTestResult> UMingTestCoverageAnalyzer::GetTestResults() const
{
    return TestResults;
}

TArray<FTestResult> UMingTestCoverageAnalyzer::GetFailedTests() const
{
    TArray<FTestResult> Failed;

    for (const auto& Result : TestResults)
    {
        if (Result.Status == ETestStatus::Failed || Result.Status == ETestStatus::Error || Result.Status == ETestStatus::Timeout)
        {
            Failed.Add(Result);
        }
    }

    return Failed;
}

TArray<FTestResult> UMingTestCoverageAnalyzer::GetTestsByType(ETestType Type) const
{
    TArray<FTestResult> Filtered;

    for (const auto& Result : TestResults)
    {
        if (Result.Type == Type)
        {
            Filtered.Add(Result);
        }
    }

    return Filtered;
}

FCoverageReport UMingTestCoverageAnalyzer::GenerateCoverageReport()
{
    FCoverageReport Report;
    Report.ReportID = GenerateReportID();
    Report.GeneratedTime = FPlatformTime::Seconds();
    Report.FileCoverage = CoverageHistory;

    // Calculate overall coverage
    TMap<ECoverageType, float> TypeCoverage;
    TMap<ECoverageType, int32> TypeCount;

    for (const auto& Data : CoverageHistory)
    {
        if (!TypeCoverage.Contains(Data.Type))
        {
            TypeCoverage.Add(Data.Type, 0.0f);
            TypeCount.Add(Data.Type, 0);
        }

        TypeCoverage[Data.Type] += Data.CoveragePercent;
        TypeCount[Data.Type]++;
    }

    for (const auto& Pair : TypeCoverage)
    {
        float Average = TypeCount[Pair.Key] > 0 ? Pair.Value / TypeCount[Pair.Key] : 0.0f;
        Report.OverallCoverage.Add(Pair.Key, Average);
    }

    Report.TotalFiles = CoverageHistory.Num();

    for (const auto& Data : CoverageHistory)
    {
        if (CoverageTargets.Contains(Data.Type))
        {
            const FCoverageTarget& Target = CoverageTargets[Data.Type];

            if (Data.CoveragePercent >= Target.TargetPercent)
            {
                Report.FilesMeetingTarget++;
            }
            else if (Data.CoveragePercent < Target.MinimumPercent)
            {
                Report.FilesBelowMinimum++;
                Report.HighRiskFiles.Add(Data.FilePath);
            }
        }
    }

    GenerateRecommendations(Report);

    OnReportGenerated.Broadcast(Report);

    UE_LOG(LogTestCoverage, Log, TEXT("Generated coverage report: %s"), *Report.ReportID);

    return Report;
}

void UMingTestCoverageAnalyzer::ExportCoverageReport(const FString& FilePath)
{
    FCoverageReport Report = GenerateCoverageReport();

    FString Content = TEXT("# Test Coverage Report\n\n");
    Content += FString::Printf(TEXT("**Report ID:** %s\n\n"), *Report.ReportID);
    Content += FString::Printf(TEXT("**Generated:** %s\n\n"), *FDateTime::Now().ToString());

    Content += TEXT("## Overall Coverage\n\n");
    Content += TEXT("| Type | Coverage |\n");
    Content += TEXT("|------|----------|\n");

    for (const auto& Pair : Report.OverallCoverage)
    {
        Content += FString::Printf(TEXT("| %s | %.1f%% |\n"), *UEnum::GetValueAsString(Pair.Key), Pair.Value);
    }

    Content += TEXT("\n## File Coverage\n\n");
    Content += TEXT("| File | Type | Coverage | Status |\n");
    Content += TEXT("|------|------|----------|--------|\n");

    for (const auto& Data : Report.FileCoverage)
    {
        FString Status = TEXT("✓");
        if (CoverageTargets.Contains(Data.Type))
        {
            const FCoverageTarget& Target = CoverageTargets[Data.Type];
            if (Data.CoveragePercent < Target.MinimumPercent)
            {
                Status = TEXT("✗");
            }
            else if (Data.CoveragePercent < Target.TargetPercent)
            {
                Status = TEXT("⚠");
            }
        }

        Content += FString::Printf(TEXT("| %s | %s | %.1f%% | %s |\n"),
            *FPaths::GetCleanFilename(Data.FilePath),
            *UEnum::GetValueAsString(Data.Type),
            Data.CoveragePercent,
            *Status);
    }

    Content += TEXT("\n## High Risk Files\n\n");
    for (const auto& File : Report.HighRiskFiles)
    {
        Content += FString::Printf(TEXT("- %s\n"), *File);
    }

    Content += TEXT("\n## Recommendations\n\n");
    for (const auto& Rec : Report.Recommendations)
    {
        Content += FString::Printf(TEXT("- %s\n"), *Rec);
    }

    FFileHelper::SaveStringToFile(Content, *FilePath);

    UE_LOG(LogTestCoverage, Log, TEXT("Exported coverage report to: %s"), *FilePath);
}

void UMingTestCoverageAnalyzer::ExportTestResults(const FString& FilePath)
{
    FString Content = TEXT("# Test Results\n\n");
    Content += FString::Printf(TEXT("**Total Tests:** %d\n\n"), TestResults.Num());

    int32 Passed = GetPassedTestsCount();
    int32 Failed = GetFailedTestsCount();

    Content += FString::Printf(TEXT("**Passed:** %d\n\n"), Passed);
    Content += FString::Printf(TEXT("**Failed:** %d\n\n"), Failed);

    Content += TEXT("## Detailed Results\n\n");
    Content += TEXT("| Test | Type | Status | Duration |\n");
    Content += TEXT("|------|------|--------|----------|\n");

    for (const auto& Result : TestResults)
    {
        Content += FString::Printf(TEXT("| %s | %s | %s | %.2fs |\n"),
            *Result.TestName,
            *UEnum::GetValueAsString(Result.Type),
            *UEnum::GetValueAsString(Result.Status),
            Result.Duration);
    }

    FFileHelper::SaveStringToFile(Content, *FilePath);

    UE_LOG(LogTestCoverage, Log, TEXT("Exported test results to: %s"), *FilePath);
}

TArray<FString> UMingTestCoverageAnalyzer::IdentifyUntestedCode()
{
    TArray<FString> Untested;

    for (const auto& Data : CoverageHistory)
    {
        if (Data.CoveragePercent < 10.0f)
        {
            Untested.Add(Data.FilePath);
        }
    }

    UE_LOG(LogTestCoverage, Log, TEXT("Identified %d untested files"), Untested.Num());

    return Untested;
}

TArray<FString> UMingTestCoverageAnalyzer::IdentifyHighRiskAreas()
{
    TArray<FString> HighRisk;

    for (const auto& Data : CoverageHistory)
    {
        if (CoverageTargets.Contains(Data.Type))
        {
            const FCoverageTarget& Target = CoverageTargets[Data.Type];
            if (Data.CoveragePercent < Target.MinimumPercent)
            {
                HighRisk.Add(Data.FilePath);
            }
        }
    }

    UE_LOG(LogTestCoverage, Log, TEXT("Identified %d high-risk files"), HighRisk.Num());

    return HighRisk;
}

TArray<FString> UMingTestCoverageAnalyzer::SuggestTestsForFile(const FString& FilePath)
{
    TArray<FString> Suggestions;

    Suggestions.Add(FString::Printf(TEXT("Add unit test for %s"), *FPaths::GetCleanFilename(FilePath)));
    Suggestions.Add(TEXT("Test boundary conditions"));
    Suggestions.Add(TEXT("Test error handling paths"));
    Suggestions.Add(TEXT("Add integration tests"));

    return Suggestions;
}

bool UMingTestCoverageAnalyzer::IsCoverageTargetMet(ECoverageType Type) const
{
    if (!CoverageTargets.Contains(Type))
    {
        return false;
    }

    const FCoverageTarget& Target = CoverageTargets[Type];
    float CurrentCoverage = GetCurrentCoverage(Type);

    return CurrentCoverage >= Target.TargetPercent;
}

float UMingTestCoverageAnalyzer::GetCurrentCoverage(ECoverageType Type) const
{
    float Total = 0.0f;
    int32 Count = 0;

    for (const auto& Data : CoverageHistory)
    {
        if (Data.Type == Type)
        {
            Total += Data.CoveragePercent;
            Count++;
        }
    }

    return Count > 0 ? Total / Count : 0.0f;
}

void UMingTestCoverageAnalyzer::StartContinuousMonitoring()
{
    if (bIsMonitoring)
    {
        return;
    }

    bIsMonitoring = true;

    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            MonitoringTimer,
            this,
            &UMingTestCoverageAnalyzer::PerformMonitoringCheck,
            300.0f, // 5 minutes
            true
        );

        UE_LOG(LogTestCoverage, Log, TEXT("Started continuous monitoring"));
    }
}

void UMingTestCoverageAnalyzer::StopContinuousMonitoring()
{
    if (!bIsMonitoring)
    {
        return;
    }

    bIsMonitoring = false;

    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(MonitoringTimer);
    }

    UE_LOG(LogTestCoverage, Log, TEXT("Stopped continuous monitoring"));
}

void UMingTestCoverageAnalyzer::SetCoverageThreshold(ECoverageType Type, float Target, float Minimum)
{
    FCoverageTarget CoverageTarget;
    CoverageTarget.Type = Type;
    CoverageTarget.TargetPercent = Target;
    CoverageTarget.MinimumPercent = Minimum;

    SetCoverageTarget(CoverageTarget);
}

void UMingTestCoverageAnalyzer::GenerateCoverageBadge()
{
    // Generate a badge for README display
    float Overall = GetCurrentCoverage(ECoverageType::Line);

    FString Color = TEXT("red");
    if (Overall >= 80.0f) Color = TEXT("brightgreen");
    else if (Overall >= 70.0f) Color = TEXT("green");
    else if (Overall >= 60.0f) Color = TEXT("yellow");
    else if (Overall >= 50.0f) Color = TEXT("orange");

    UE_LOG(LogTestCoverage, Log, TEXT("Generated coverage badge: %.1f%% (%s)"), Overall, *Color);
}

int32 UMingTestCoverageAnalyzer::GetPassedTestsCount() const
{
    int32 Count = 0;
    for (const auto& Result : TestResults)
    {
        if (Result.Status == ETestStatus::Passed)
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingTestCoverageAnalyzer::GetFailedTestsCount() const
{
    int32 Count = 0;
    for (const auto& Result : TestResults)
    {
        if (Result.Status == ETestStatus::Failed || Result.Status == ETestStatus::Error)
        {
            Count++;
        }
    }
    return Count;
}

void UMingTestCoverageAnalyzer::StartMonitoring()
{
    // Timer is set in StartContinuousMonitoring
}

void UMingTestCoverageAnalyzer::StopMonitoring()
{
    StopContinuousMonitoring();
}

void UMingTestCoverageAnalyzer::PerformMonitoringCheck()
{
    UE_LOG(LogTestCoverage, Verbose, TEXT("Performing monitoring check"));

    // Check for coverage changes
    CheckCoverageTargets();
}

FCoverageData UMingTestCoverageAnalyzer::ParseCoverageData(const FString& RawData, ECoverageType Type)
{
    FCoverageData Data;
    Data.Type = Type;

    // Would parse actual coverage data from tools like gcov, lcov, etc.
    return Data;
}

void UMingTestCoverageAnalyzer::UpdateCoverageData(const FCoverageData& NewData)
{
    // Remove old data for this file and type
    for (int32 i = CoverageHistory.Num() - 1; i >= 0; --i)
    {
        if (CoverageHistory[i].FilePath == NewData.FilePath && CoverageHistory[i].Type == NewData.Type)
        {
            CoverageHistory.RemoveAt(i);
        }
    }

    CoverageHistory.Add(NewData);
    OnCoverageDataUpdated.Broadcast(NewData);
}

void UMingTestCoverageAnalyzer::CheckCoverageTargets()
{
    for (const auto& Pair : CoverageTargets)
    {
        ECoverageType Type = Pair.Key;
        const FCoverageTarget& Target = Pair.Value;

        float Current = GetCurrentCoverage(Type);

        if (Current >= Target.TargetPercent)
        {
            OnCoverageTargetMet.Broadcast(Type);
        }
        else if (Current < Target.MinimumPercent)
        {
            OnCoverageBelowTarget.Broadcast(Type, Current);
        }
    }
}

bool UMingTestCoverageAnalyzer::ExecuteTest(const FString& TestName, ETestType Type)
{
    return RunTest(TestName, Type);
}

TArray<FString> UMingTestCoverageAnalyzer::DiscoverTests(const FTestSuiteConfig& Config)
{
    TArray<FString> Tests;

    for (const auto& Path : Config.TestPaths)
    {
        TArray<FString> Files;
        IFileManager::Get().FindFilesRecursive(Files, *Path, TEXT("*Test.cpp"), true, false);

        for (const auto& File : Files)
        {
            FString TestName = FPaths::GetBaseFilename(File);
            Tests.Add(TestName);
        }
    }

    return Tests;
}

FString UMingTestCoverageAnalyzer::GenerateReportID()
{
    return FString::Printf(TEXT("CR-%d-%d"), static_cast<int32>(FPlatformTime::Seconds()), FMath::RandRange(1000, 9999));
}

void UMingTestCoverageAnalyzer::StoreTestResult(const FTestResult& Result)
{
    // Remove existing result for this test
    for (int32 i = TestResults.Num() - 1; i >= 0; --i)
    {
        if (TestResults[i].TestID == Result.TestID && TestResults[i].Type == Result.Type)
        {
            TestResults.RemoveAt(i);
        }
    }

    TestResults.Add(Result);
}

void UMingTestCoverageAnalyzer::LogCoverageActivity(const FString& Activity)
{
    UE_LOG(LogTestCoverage, Log, TEXT("[COVERAGE] %s"), *Activity);
}

float UMingTestCoverageAnalyzer::CalculateWeightedCoverage(const TArray<FCoverageData>& Data)
{
    float Total = 0.0f;
    float Weight = 0.0f;

    for (const auto& D : Data)
    {
        float W = D.TotalLines;
        Total += D.CoveragePercent * W;
        Weight += W;
    }

    return Weight > 0.0f ? Total / Weight : 0.0f;
}

TArray<FString> UMingTestCoverageAnalyzer::FindCriticalUncoveredLines(const FCoverageData& Data)
{
    TArray<FString> CriticalLines;

    // Would identify critical uncovered code paths
    for (int32 Line : Data.UncoveredLineNumbers)
    {
        CriticalLines.Add(FString::Printf(TEXT("Line %d"), Line));
    }

    return CriticalLines;
}

void UMingTestCoverageAnalyzer::GenerateRecommendations(FCoverageReport& Report)
{
    Report.Recommendations.Empty();

    if (Report.FilesBelowMinimum > 0)
    {
        Report.Recommendations.Add(FString::Printf(TEXT("%d files are below minimum coverage threshold"), Report.FilesBelowMinimum));
        Report.Recommendations.Add(TEXT("Focus testing efforts on high-risk files"));
    }

    if (!IsCoverageTargetMet(ECoverageType::Line))
    {
        float Current = GetCurrentCoverage(ECoverageType::Line);
        float Target = CoverageTargets.Contains(ECoverageType::Line) ? CoverageTargets[ECoverageType::Line].TargetPercent : 85.0f;
        Report.Recommendations.Add(FString::Printf(TEXT("Increase line coverage from %.1f%% to %.1f%%"), Current, Target));
    }

    if (!IsCoverageTargetMet(ECoverageType::Branch))
    {
        Report.Recommendations.Add(TEXT("Add tests for branch coverage, especially edge cases"));
    }

    if (Report.HighRiskFiles.Num() > 0)
    {
        Report.Recommendations.Add(TEXT("Prioritize testing for high-risk files"));
    }
}

static UMingTestCoverageAnalyzer* UMingTestCoverageAnalyzer::Get(UObject* WorldContextObject)
{
    static UMingTestCoverageAnalyzer* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingTestCoverageAnalyzer>();
        Instance->AddToRoot();
    }
    return Instance;
}
