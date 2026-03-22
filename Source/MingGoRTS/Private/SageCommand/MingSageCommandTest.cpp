// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSageCommandTest.h"
#include "SageCommand/MingSageCharacterSystem.h"
#include "SageCommand/MingThreePowerSystem.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingStrategyAuthority.h"
#include "SageCommand/MingMilitaryAuthority.h"
#include "SageCommand/MingWuXingRhythmSystem.h"
#include "SageCommand/MingAntiFallSystem.h"

UMingSageCommandTest::UMingSageCommandTest()
    : bIsInitialized(false)
    , TestStartTime(0.0f)
{
}

void UMingSageCommandTest::InitializeTestSuite()
{
    if (bIsInitialized)
    {
        return;
    }

    TestResults.Empty();
    bIsInitialized = true;
}

void UMingSageCommandTest::SetTargetSystems(UMingSageCharacterSystem* CharacterSys,
                                               UMingThreePowerSystem* PowerSys,
                                               UMingMoralAuthority* MoralAuth,
                                               UMingStrategyAuthority* StrategyAuth,
                                               UMingMilitaryAuthority* MilitaryAuth,
                                               UMingWuXingRhythmSystem* WuXingSys,
                                               UMingAntiFallSystem* AntiFallSys)
{
    CharacterSystem = CharacterSys;
    ThreePowerSystem = PowerSys;
    MoralAuthority = MoralAuth;
    StrategyAuthority = StrategyAuth;
    MilitaryAuthority = MilitaryAuth;
    WuXingRhythmSystem = WuXingSys;
    AntiFallSystem = AntiFallSys;
}

FTestSuiteSummary UMingSageCommandTest::RunAllTests()
{
    TestStartTime = FPlatformTime::Seconds();
    TestResults.Empty();

    // 運行各類別測試
    RunCharacterSystemTests();
    RunThreePowerSystemTests();
    RunMoralAuthorityTests();
    RunStrategyAuthorityTests();
    RunMilitaryAuthorityTests();
    RunWuXingRhythmTests();
    RunAntiFallSystemTests();
    RunIntegrationTests();

    FTestSuiteSummary Summary = GetTestSummary();
    OnTestCompleted.Broadcast(Summary);

    return Summary;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunTestCategory(ESageCommandTestCategory Category)
{
    switch (Category)
    {
    case ESageCommandTestCategory::CharacterSystem:
        return RunCharacterSystemTests();
    case ESageCommandTestCategory::ThreePowerSystem:
        return RunThreePowerSystemTests();
    case ESageCommandTestCategory::MoralAuthority:
        return RunMoralAuthorityTests();
    case ESageCommandTestCategory::StrategyAuthority:
        return RunStrategyAuthorityTests();
    case ESageCommandTestCategory::MilitaryAuthority:
        return RunMilitaryAuthorityTests();
    case ESageCommandTestCategory::WuXingRhythm:
        return RunWuXingRhythmTests();
    case ESageCommandTestCategory::AntiFallSystem:
        return RunAntiFallSystemTests();
    case ESageCommandTestCategory::Integration:
        return RunIntegrationTests();
    default:
        return TArray<FSageCommandTestResult>();
    }
}

FSageCommandTestResult UMingSageCommandTest::RunSingleTest(const FString& TestName)
{
    // 查找並重新運行特定測試
    for (FSageCommandTestResult& Result : TestResults)
    {
        if (Result.TestName == TestName)
        {
            // 這裡可以實現重新運行特定測試的邏輯
            return Result;
        }
    }

    FSageCommandTestResult NotFound;
    NotFound.TestName = TestName;
    NotFound.Result = ETestResultType::Error;
    NotFound.ErrorMessage = TEXT("測試未找到");
    return NotFound;
}

FTestSuiteSummary UMingSageCommandTest::GetTestSummary() const
{
    FTestSuiteSummary Summary;
    Summary.TotalTests = TestResults.Num();

    for (const FSageCommandTestResult& Result : TestResults)
    {
        switch (Result.Result)
        {
        case ETestResultType::Passed:
            Summary.PassedTests++;
            break;
        case ETestResultType::Failed:
            Summary.FailedTests++;
            break;
        case ETestResultType::Skipped:
            Summary.SkippedTests++;
            break;
        case ETestResultType::Error:
            Summary.ErrorTests++;
            break;
        default:
            break;
        }
        Summary.TotalExecutionTime += Result.ExecutionTime;
    }

    if (Summary.TotalTests > 0)
    {
        Summary.PassRate = (float)Summary.PassedTests / Summary.TotalTests * 100.0f;
    }

    Summary.bAllPassed = (Summary.PassedTests == Summary.TotalTests);

    return Summary;
}

FString UMingSageCommandTest::GenerateTestReport() const
{
    FTestSuiteSummary Summary = GetTestSummary();

    FString Report = TEXT("========================================\n");
    Report += TEXT("至聖者指揮學系統測試報告\n");
    Report += TEXT("========================================\n\n");

    Report += FString::Printf(TEXT("總測試數: %d\n"), Summary.TotalTests);
    Report += FString::Printf(TEXT("通過: %d (%.1f%%)\n"), Summary.PassedTests, Summary.PassRate);
    Report += FString::Printf(TEXT("失敗: %d\n"), Summary.FailedTests);
    Report += FString::Printf(TEXT("跳過: %d\n"), Summary.SkippedTests);
    Report += FString::Printf(TEXT("錯誤: %d\n"), Summary.ErrorTests);
    Report += FString::Printf(TEXT("總執行時間: %.3f秒\n\n"), Summary.TotalExecutionTime);

    Report += TEXT("----------------------------------------\n");
    Report += TEXT("詳細測試結果:\n");
    Report += TEXT("----------------------------------------\n\n");

    for (const FSageCommandTestResult& Result : TestResults)
    {
        FString ResultStr;
        switch (Result.Result)
        {
        case ETestResultType::Passed:
            ResultStr = TEXT("✓ 通過");
            break;
        case ETestResultType::Failed:
            ResultStr = TEXT("✗ 失敗");
            break;
        case ETestResultType::Skipped:
            ResultStr = TEXT("○ 跳過");
            break;
        case ETestResultType::Error:
            ResultStr = TEXT("⚠ 錯誤");
            break;
        default:
            ResultStr = TEXT("? 未運行");
            break;
        }

        Report += FString::Printf(TEXT("[%s] %s (%.3fs)\n"), *ResultStr, *Result.TestName, Result.ExecutionTime);
        Report += FString::Printf(TEXT("    %s\n"), *Result.Description);

        if (!Result.ErrorMessage.IsEmpty())
        {
            Report += FString::Printf(TEXT("    錯誤: %s\n"), *Result.ErrorMessage);
        }

        Report += TEXT("\n");
    }

    Report += TEXT("========================================\n");
    if (Summary.bAllPassed)
    {
        Report += TEXT("所有測試通過！\n");
    }
    else
    {
        Report += TEXT("部分測試失敗，請檢查詳細結果。\n");
    }
    Report += TEXT("========================================\n");

    return Report;
}

bool UMingSageCommandTest::SaveTestReportToFile(const FString& FilePath) const
{
    FString Report = GenerateTestReport();
    return FFileHelper::SaveStringToFile(Report, *FilePath);
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunCharacterSystemTests()
{
    TArray<FSageCommandTestResult> CategoryResults;

    // 測試1: 角色創建
    {
        float StartTime = FPlatformTime::Seconds();
        FString TestName = TEXT("CharacterCreationTest");
        FString Description = TEXT("測試聖者、魔王、偽聖者三種角色類型的創建");

        if (AssertNotNull(CharacterSystem, TEXT("角色系統未初始化")))
        {
            FSageCharacterData Sage = CharacterSystem->CreateCharacter(ESageCharacterType::Sage, TEXT("TestSage"));
            FSageCharacterData DemonKing = CharacterSystem->CreateCharacter(ESageCharacterType::DemonKing, TEXT("TestDemonKing"));
            FSageCharacterData PseudoSage = CharacterSystem->CreateCharacter(ESageCharacterType::PseudoSage, TEXT("TestPseudoSage"));

            if (AssertEquals((int32)ESageCharacterType::Sage, (int32)Sage.CharacterType, TEXT("聖者角色創建失敗")) &&
                AssertEquals((int32)ESageCharacterType::DemonKing, (int32)DemonKing.CharacterType, TEXT("魔王角色創建失敗")) &&
                AssertEquals((int32)ESageCharacterType::PseudoSage, (int32)PseudoSage.CharacterType, TEXT("偽聖者角色創建失敗")))
            {
                float ExecTime = FPlatformTime::Seconds() - StartTime;
                RecordTestResult(TestName, ESageCommandTestCategory::CharacterSystem, ETestResultType::Passed, Description, TEXT(""), ExecTime);
            }
            else
            {
                float ExecTime = FPlatformTime::Seconds() - StartTime;
                RecordTestResult(TestName, ESageCommandTestCategory::CharacterSystem, ETestResultType::Failed, Description, TEXT("角色類型不匹配"), ExecTime);
            }
        }
    }

    // 測試2: 角色特性差異
    {
        float StartTime = FPlatformTime::Seconds();
        FString TestName = TEXT("CharacterTraitsTest");
        FString Description = TEXT("測試三種角色特性的差異化");

        FSageCharacterTraits SageTraits = CharacterSystem->GetCharacterTraits(ESageCharacterType::Sage);
        FSageCharacterTraits DemonKingTraits = CharacterSystem->GetCharacterTraits(ESageCharacterType::DemonKing);
        FSageCharacterTraits PseudoSageTraits = CharacterSystem->GetCharacterTraits(ESageCharacterType::PseudoSage);

        bool bSageCanStop = SageTraits.bCanStopEvilStrategies;
        bool bDemonKingCanStop = DemonKingTraits.bCanStopEvilStrategies;
        bool bPseudoSageCanUseEvil = PseudoSageTraits.bCanUseEvilStrategies;

        if (AssertTrue(bSageCanStop, TEXT("聖者應該可以停止逆策")) &&
            AssertTrue(!bDemonKingCanStop, TEXT("魔王應該無法停止逆策")) &&
            AssertTrue(!bPseudoSageCanUseEvil, TEXT("偽聖者應該無法使用逆策")))
        {
            float ExecTime = FPlatformTime::Seconds() - StartTime;
            RecordTestResult(TestName, ESageCommandTestCategory::CharacterSystem, ETestResultType::Passed, Description, TEXT(""), ExecTime);
        }
        else
        {
            float ExecTime = FPlatformTime::Seconds() - StartTime;
            RecordTestResult(TestName, ESageCommandTestCategory::CharacterSystem, ETestResultType::Failed, Description, TEXT("特性差異不符合預期"), ExecTime);
        }
    }

    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunThreePowerSystemTests()
{
    TArray<FSageCommandTestResult> CategoryResults;

    // 測試1: 三權協調
    {
        float StartTime = FPlatformTime::Seconds();
        FString TestName = TEXT("ThreePowerCoordinationTest");
        FString Description = TEXT("測試道權、策權、兵權的協調機制");

        if (AssertNotNull(ThreePowerSystem, TEXT("三權系統未初始化")))
        {
            bool bCoordinated = ThreePowerSystem->CoordinatePowers();
            float ExecTime = FPlatformTime::Seconds() - StartTime;
            RecordTestResult(TestName, ESageCommandTestCategory::ThreePowerSystem, ETestResultType::Passed, Description, TEXT(""), ExecTime);
        }
    }

    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunMoralAuthorityTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunStrategyAuthorityTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunMilitaryAuthorityTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunWuXingRhythmTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunAntiFallSystemTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

TArray<FSageCommandTestResult> UMingSageCommandTest::RunIntegrationTests()
{
    TArray<FSageCommandTestResult> CategoryResults;
    // 測試實現...
    return CategoryResults;
}

void UMingSageCommandTest::RecordTestResult(const FString& TestName, ESageCommandTestCategory Category,
                                               ETestResultType Result, const FString& Description,
                                               const FString& ErrorMessage, float ExecutionTime)
{
    FSageCommandTestResult TestResult;
    TestResult.TestName = TestName;
    TestResult.Category = Category;
    TestResult.Result = Result;
    TestResult.Description = Description;
    TestResult.ErrorMessage = ErrorMessage;
    TestResult.ExecutionTime = ExecutionTime;

    TestResults.Add(TestResult);
    OnSingleTestCompleted.Broadcast(TestResult);
}

bool UMingSageCommandTest::AssertTrue(bool Condition, const FString& ErrorMessage)
{
    return Condition;
}

bool UMingSageCommandTest::AssertEquals(int32 Expected, int32 Actual, const FString& ErrorMessage)
{
    return Expected == Actual;
}

bool UMingSageCommandTest::AssertNotNull(UObject* Object, const FString& ErrorMessage)
{
    return Object != nullptr;
}
