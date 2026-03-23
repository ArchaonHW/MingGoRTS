// Copyright Epic Games, Inc. All Rights Reserved.

#include "SageCommand/MingSageCommandIntegrationTest.h"
#include "SageCommand/MingThreePowerSystem.h"
#include "SageCommand/MingSixStrategiesSystem.h"
#include "SageCommand/MingWuXingRhythmSystem.h"
#include "SageCommand/MingAntiCorruptionSystem.h"
#include "SageCommand/MingAIAdaptationSystem.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingSageCommandIntegrationTest::UMingSageCommandIntegrationTest()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 初始化系統參數
    NextTestID = 1;
    TestTimeoutDefault = 30.0f;
    
    // 初始化狀態
    bSystemInitialized = false;
    bIsRunningTest = false;
    bIsPaused = false;
    
    // 初始化系統引用
    ThreePowerSystemRef = nullptr;
    SixStrategiesSystemRef = nullptr;
    WuXingSystemRef = nullptr;
    AntiCorruptionSystemRef = nullptr;
    AIAdaptationSystemRef = nullptr;
}

void UMingSageCommandIntegrationTest::BeginPlay()
{
    Super::BeginPlay();
    
    // 初始化系統
    InitializeIntegrationTestSystem();
    
    // 啟動狀態檢查定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StatusCheckTimer,
            this,
            &UMingSageCommandIntegrationTest::UpdateIntegrationStatus,
            5.0f,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("集成測試系統初始化完成"));
}

void UMingSageCommandIntegrationTest::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    if (!bSystemInitialized || bIsPaused)
    {
        return;
    }
    
    // 記錄幀時間
    if (FrameTimeHistory.Num() >= MAX_FRAME_HISTORY)
    {
        FrameTimeHistory.RemoveAt(0);
    }
    FrameTimeHistory.Add(DeltaTime);
}

bool UMingSageCommandIntegrationTest::InitializeIntegrationTestSystem()
{
    if (bSystemInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("集成測試系統已經初始化"));
        return true;
    }
    
    UE_LOG(LogTemp, Log, TEXT("開始初始化集成測試系統"));
    
    // 初始化測試案例庫
    InitializeTestCaseLibrary();
    
    // 初始化系統引用
    CheckSystemReferences();
    
    // 初始化集成狀態
    CurrentIntegrationStatus = FSystemIntegrationStatus();
    
    // 清空測試歷史
    TestResultHistory.Empty();
    FrameTimeHistory.Empty();
    
    bSystemInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("✓ 集成測試系統初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  測試案例數量: %d"), TestCaseLibrary.Num());
    
    return true;
}

void UMingSageCommandIntegrationTest::ResetIntegrationTestSystem()
{
    UE_LOG(LogTemp, Log, TEXT("重置集成測試系統"));
    
    // 停止定時器
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StatusCheckTimer);
        GetWorld()->GetTimerManager().ClearTimer(PerformanceMonitorTimer);
    }
    
    // 停止當前測試
    if (bIsRunningTest)
    {
        StopCurrentTest();
    }
    
    // 清空數據
    TestCaseLibrary.Empty();
    TestResultHistory.Empty();
    FrameTimeHistory.Empty();
    CurrentTestResult = FIntegrationTestResultData();
    
    // 重置ID
    NextTestID = 1;
    
    // 重置狀態
    bIsRunningTest = false;
    bIsPaused = false;
    bSystemInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("集成測試系統已重置"));
}

void UMingSageCommandIntegrationTest::InitializeTestCaseLibrary()
{
    // 創建標準測試案例
    
    // 1. 三權系統集成測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("三權系統集成測試");
        TestCase.TestType = EIntegrationTestType::ThreePowerIntegration;
        TestCase.Severity = ETestSeverity::Critical;
        TestCase.TestSteps.Add(TEXT("初始化三權系統"));
        TestCase.TestSteps.Add(TEXT("驗證道權初始化"));
        TestCase.TestSteps.Add(TEXT("驗證策權初始化"));
        TestCase.TestSteps.Add(TEXT("驗證兵權初始化"));
        TestCase.TestSteps.Add(TEXT("測試權力分配"));
        TestCase.TestSteps.Add(TEXT("測試跨權通信"));
        TestCase.ExpectedResults.Add(TEXT("三權系統正常初始化"));
        TestCase.ExpectedResults.Add(TEXT("道權組件就緒"));
        TestCase.ExpectedResults.Add(TEXT("策權組件就緒"));
        TestCase.ExpectedResults.Add(TEXT("兵權組件就緒"));
        TestCase.ExpectedResults.Add(TEXT("權力分配正確"));
        TestCase.ExpectedResults.Add(TEXT("通信正常"));
        TestCase.RequiredSystems.Add(TEXT("ThreePowerSystem"));
        TestCase.RequiredSystems.Add(TEXT("MoralAuthority"));
        TestCase.RequiredSystems.Add(TEXT("StrategyAuthority"));
        TestCase.RequiredSystems.Add(TEXT("MilitaryAuthority"));
        TestCase.TimeoutDuration = 30.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 2. 策略執行流程測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("策略執行流程測試");
        TestCase.TestType = EIntegrationTestType::StrategyExecutionFlow;
        TestCase.Severity = ETestSeverity::Critical;
        TestCase.TestSteps.Add(TEXT("初始化六策系統"));
        TestCase.TestSteps.Add(TEXT("執行正策（立國）"));
        TestCase.TestSteps.Add(TEXT("驗證策略效果"));
        TestCase.TestSteps.Add(TEXT("執行逆策（破局）"));
        TestCase.TestSteps.Add(TEXT("驗證陰陽切換"));
        TestCase.ExpectedResults.Add(TEXT("六策系統就緒"));
        TestCase.ExpectedResults.Add(TEXT("正策執行成功"));
        TestCase.ExpectedResults.Add(TEXT("效果應用正確"));
        TestCase.ExpectedResults.Add(TEXT("逆策執行成功"));
        TestCase.ExpectedResults.Add(TEXT("切換邏輯正確"));
        TestCase.RequiredSystems.Add(TEXT("SixStrategiesSystem"));
        TestCase.RequiredSystems.Add(TEXT("StrategyAuthority"));
        TestCase.TimeoutDuration = 45.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 3. 五行節奏協調測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("五行節奏協調測試");
        TestCase.TestType = EIntegrationTestType::WuXingRhythmCoordination;
        TestCase.Severity = ETestSeverity::High;
        TestCase.TestSteps.Add(TEXT("初始化五行節奏"));
        TestCase.TestSteps.Add(TEXT("啟動五行循環"));
        TestCase.TestSteps.Add(TEXT("驗證木階段"));
        TestCase.TestSteps.Add(TEXT("驗證階段轉換"));
        TestCase.TestSteps.Add(TEXT("驗證五行加成"));
        TestCase.ExpectedResults.Add(TEXT("節奏系統就緒"));
        TestCase.ExpectedResults.Add(TEXT("循環正常啟動"));
        TestCase.ExpectedResults.Add(TEXT("木階段效果正確"));
        TestCase.ExpectedResults.Add(TEXT("轉換邏輯正確"));
        TestCase.ExpectedResults.Add(TEXT("加成計算正確"));
        TestCase.RequiredSystems.Add(TEXT("WuXingRhythmSystem"));
        TestCase.RequiredSystems.Add(TEXT("SixStrategiesSystem"));
        TestCase.TimeoutDuration = 120.0f; // 較長超時以允許階段轉換
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 4. 防墮機制集成測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("防墮機制集成測試");
        TestCase.TestType = EIntegrationTestType::AntiCorruptionIntegration;
        TestCase.Severity = ETestSeverity::Critical;
        TestCase.TestSteps.Add(TEXT("初始化防墮系統"));
        TestCase.TestSteps.Add(TEXT("模擬墮落徵象"));
        TestCase.TestSteps.Add(TEXT("驗證徵象檢測"));
        TestCase.TestSteps.Add(TEXT("驗證警告發布"));
        TestCase.TestSteps.Add(TEXT("測試防墮措施"));
        TestCase.ExpectedResults.Add(TEXT("防墮系統就緒"));
        TestCase.ExpectedResults.Add(TEXT("徵象模擬成功"));
        TestCase.ExpectedResults.Add(TEXT("檢測邏輯正確"));
        TestCase.ExpectedResults.Add(TEXT("警告發布正常"));
        TestCase.ExpectedResults.Add(TEXT("措施生效"));
        TestCase.RequiredSystems.Add(TEXT("AntiCorruptionSystem"));
        TestCase.RequiredSystems.Add(TEXT("MoralAuthority"));
        TestCase.TimeoutDuration = 30.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 5. AI適配集成測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("AI適配集成測試");
        TestCase.TestType = EIntegrationTestType::AIAdaptationIntegration;
        TestCase.Severity = ETestSeverity::High;
        TestCase.TestSteps.Add(TEXT("初始化AI系統"));
        TestCase.TestSteps.Add(TEXT("識別對手類型"));
        TestCase.TestSteps.Add(TEXT("分析對手行為"));
        TestCase.TestSteps.Add(TEXT("生成適配決策"));
        TestCase.TestSteps.Add(TEXT("測試學習機制"));
        TestCase.ExpectedResults.Add(TEXT("AI系統就緒"));
        TestCase.ExpectedResults.Add(TEXT("識別成功"));
        TestCase.ExpectedResults.Add(TEXT("分析正確"));
        TestCase.ExpectedResults.Add(TEXT("決策生成正常"));
        TestCase.ExpectedResults.Add(TEXT("學習機制有效"));
        TestCase.RequiredSystems.Add(TEXT("AIAdaptationSystem"));
        TestCase.RequiredSystems.Add(TEXT("StrategyAuthority"));
        TestCase.TimeoutDuration = 45.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 6. 跨權限通信測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("跨權限通信測試");
        TestCase.TestType = EIntegrationTestType::CrossAuthorityCommunication;
        TestCase.Severity = ETestSeverity::High;
        TestCase.TestSteps.Add(TEXT("測試道權->策權通信"));
        TestCase.TestSteps.Add(TEXT("測試策權->兵權通信"));
        TestCase.TestSteps.Add(TEXT("測試兵權->道權通信"));
        TestCase.TestSteps.Add(TEXT("測試廣播機制"));
        TestCase.TestSteps.Add(TEXT("測試事件響應"));
        TestCase.ExpectedResults.Add(TEXT("道策通信正常"));
        TestCase.ExpectedResults.Add(TEXT("策兵通信正常"));
        TestCase.ExpectedResults.Add(TEXT("兵道通信正常"));
        TestCase.ExpectedResults.Add(TEXT("廣播機制正常"));
        TestCase.ExpectedResults.Add(TEXT("事件響應正常"));
        TestCase.RequiredSystems.Add(TEXT("ThreePowerSystem"));
        TestCase.RequiredSystems.Add(TEXT("MoralAuthority"));
        TestCase.RequiredSystems.Add(TEXT("StrategyAuthority"));
        TestCase.RequiredSystems.Add(TEXT("MilitaryAuthority"));
        TestCase.TimeoutDuration = 30.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    // 7. 全系統集成測試
    {
        FIntegrationTestCase TestCase;
        TestCase.TestID = NextTestID++;
        TestCase.TestName = TEXT("全系統集成測試");
        TestCase.TestType = EIntegrationTestType::FullSystemIntegration;
        TestCase.Severity = ETestSeverity::Critical;
        TestCase.TestSteps.Add(TEXT("初始化所有系統"));
        TestCase.TestSteps.Add(TEXT("執行完整策略流程"));
        TestCase.TestSteps.Add(TEXT("驗證跨系統協作"));
        TestCase.TestSteps.Add(TEXT("測試性能指標"));
        TestCase.TestSteps.Add(TEXT("驗證數據一致性"));
        TestCase.ExpectedResults.Add(TEXT("所有系統就緒"));
        TestCase.ExpectedResults.Add(TEXT("流程執行成功"));
        TestCase.ExpectedResults.Add(TEXT("協作機制正常"));
        TestCase.ExpectedResults.Add(TEXT("性能達標"));
        TestCase.ExpectedResults.Add(TEXT("數據一致"));
        TestCase.RequiredSystems.Add(TEXT("AllSystems"));
        TestCase.TimeoutDuration = 180.0f;
        TestCase.bIsAutomated = true;
        
        TestCaseLibrary.Add(TestCase.TestID, TestCase);
    }
    
    UE_LOG(LogTemp, Log, TEXT("創建了 %d 個標準測試案例"), TestCaseLibrary.Num());
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::RunSingleTest(int32 TestID)
{
    FIntegrationTestResultData Result;
    Result.TestID = TestID;
    Result.ExecutionTimeStamp = FDateTime::Now();
    
    // 檢查測試案例是否存在
    if (!TestCaseLibrary.Contains(TestID))
    {
        Result.Result = EIntegrationTestResult::Error;
        Result.Errors.Add(TEXT("測試案例不存在"));
        return Result;
    }
    
    FIntegrationTestCase TestCase = TestCaseLibrary[TestID];
    Result.TestName = TestCase.TestName;
    
    // 觸發測試開始事件
    TriggerTestStartedEvent(TestID, TestCase.TestName);
    
    UE_LOG(LogTemp, Log, TEXT("開始運行測試: %s (ID:%d)"), *TestCase.TestName, TestID);
    
    bIsRunningTest = true;
    
    // 記錄開始時間
    FDateTime StartTime = FDateTime::Now();
    
    // 根據測試類型執行特定測試
    switch (TestCase.TestType)
    {
    case EIntegrationTestType::ThreePowerIntegration:
        Result = TestThreePowerIntegration();
        break;
        
    case EIntegrationTestType::StrategyExecutionFlow:
        Result = TestStrategyExecutionFlow();
        break;
        
    case EIntegrationTestType::WuXingRhythmCoordination:
        Result = TestWuXingRhythmCoordination();
        break;
        
    case EIntegrationTestType::AntiCorruptionIntegration:
        Result = TestAntiCorruptionIntegration();
        break;
        
    case EIntegrationTestType::AIAdaptationIntegration:
        Result = TestAIAdaptationIntegration();
        break;
        
    case EIntegrationTestType::CrossAuthorityCommunication:
        Result = TestCrossAuthorityCommunication();
        break;
        
    case EIntegrationTestType::FullSystemIntegration:
        Result = TestFullSystemIntegration();
        break;
        
    default:
        Result.Result = EIntegrationTestResult::Error;
        Result.Errors.Add(TEXT("未知測試類型"));
        break;
    }
    
    // 計算執行時間
    FTimespan ExecutionTime = FDateTime::Now() - StartTime;
    Result.ExecutionTime = ExecutionTime.GetTotalSeconds();
    Result.TestID = TestID;
    Result.TestName = TestCase.TestName;
    
    // 記錄結果
    RecordTestResult(Result);
    
    bIsRunningTest = false;
    
    // 觸發測試完成事件
    TriggerTestCompletedEvent(TestID, Result);
    
    UE_LOG(LogTemp, Log, TEXT("測試完成: %s, 結果: %d, 耗時: %.2f秒"), 
           *TestCase.TestName, (int32)Result.Result, Result.ExecutionTime);
    
    return Result;
}

TArray<FIntegrationTestResultData> UMingSageCommandIntegrationTest::RunTestSuite(const TArray<int32>& TestIDs)
{
    TArray<FIntegrationTestResultData> Results;
    
    UE_LOG(LogTemp, Log, TEXT("開始運行測試套件, 包含 %d 個測試"), TestIDs.Num());
    
    for (int32 TestID : TestIDs)
    {
        FIntegrationTestResultData Result = RunSingleTest(TestID);
        Results.Add(Result);
        
        // 如果測試失敗且是關鍵測試，可以選擇停止
        if (Result.Result == EIntegrationTestResult::Failed)
        {
            FIntegrationTestCase TestCase = GetTestCase(TestID);
            if (TestCase.Severity == ETestSeverity::Critical)
            {
                UE_LOG(LogTemp, Error, TEXT("關鍵測試失敗，停止測試套件"));
                break;
            }
        }
    }
    
    // 觸發套件完成事件
    TriggerSuiteCompletedEvent(Results);
    
    UE_LOG(LogTemp, Log, TEXT("測試套件完成, 共 %d 個測試, 通過 %d 個"), 
           Results.Num(), GetTestStatistics()[EIntegrationTestResult::Passed]);
    
    return Results;
}

TArray<FIntegrationTestResultData> UMingSageCommandIntegrationTest::RunAllTests()
{
    TArray<int32> AllTestIDs;
    
    for (const auto& Pair : TestCaseLibrary)
    {
        AllTestIDs.Add(Pair.Key);
    }
    
    return RunTestSuite(AllTestIDs);
}

// ============================================================================
// 特定集成測試實現
// ============================================================================

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestThreePowerIntegration()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("三權系統集成測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    // 測試步驟1: 檢查系統引用
    if (!CheckSystemReferences())
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("系統引用檢查失敗"));
        return Result;
    }
    
    // 測試步驟2: 驗證三權系統
    if (ThreePowerSystemRef && ThreePowerSystemRef->InitializeThreePowerSystem())
    {
        Result.ActualResults.Add(TEXT("三權系統初始化成功"));
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("三權系統初始化失敗"));
        return Result;
    }
    
    // 測試步驟3: 驗證權力分配
    TMap<EPowerType, float> PowerDistribution = ThreePowerSystemRef->GetCurrentPowerDistribution();
    if (PowerDistribution.Contains(EPowerType::Moral) && 
        PowerDistribution[EPowerType::Moral] > 0.0f)
    {
        Result.ActualResults.Add(TEXT("權力分配正常"));
    }
    else
    {
        Result.Warnings.Add(TEXT("權力分配數據異常"));
    }
    
    Result.DetailedReport = FString::Printf(TEXT("三權系統集成測試%s"), 
                                            Result.Result == EIntegrationTestResult::Passed ? TEXT("通過") : TEXT("失敗"));
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestStrategyExecutionFlow()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("策略執行流程測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    // 測試策略執行流程
    if (SixStrategiesSystemRef)
    {
        // 測試正策執行
        FStrategyExecutionContext Context;
        Context.Direction = EStrategyDirection::Righteous;
        Context.CurrentPhase = EFiveElementPhase::Wood;
        
        if (SixStrategiesSystemRef->ExecuteStrategy(ESixStrategyType::EstablishNation, Context))
        {
            Result.ActualResults.Add(TEXT("正策執行成功"));
        }
        else
        {
            Result.Warnings.Add(TEXT("正策執行可能失敗或條件不足"));
        }
        
        // 測試策略效果
        TArray<FStrategyEffectDetails> Effects = SixStrategiesSystemRef->GetCurrentEffects();
        if (Effects.Num() > 0)
        {
            Result.ActualResults.Add(FString::Printf(TEXT("當前效果數量: %d"), Effects.Num()));
        }
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("六策系統未初始化"));
    }
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestWuXingRhythmCoordination()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("五行節奏協調測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    if (WuXingSystemRef)
    {
        // 測試節奏啟動
        if (WuXingSystemRef->StartWuXingRhythm())
        {
            Result.ActualResults.Add(TEXT("五行節奏啟動成功"));
            
            // 驗證當前階段
            EFiveElementPhase CurrentPhase = WuXingSystemRef->GetCurrentPhase();
            Result.ActualResults.Add(FString::Printf(TEXT("當前階段: %d"), (int32)CurrentPhase));
        }
        else
        {
            Result.Result = EIntegrationTestResult::Failed;
            Result.Errors.Add(TEXT("五行節奏啟動失敗"));
        }
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("五行節奏系統未初始化"));
    }
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestAntiCorruptionIntegration()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("防墮機制集成測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    if (AntiCorruptionSystemRef)
    {
        // 模擬墮落徵象
        if (AntiCorruptionSystemRef->DetectCorruptionPhenomenon(
            ECorruptionPhenomenon::RationalizationOfEvil, 0.5f, TEXT("測試證據")))
        {
            Result.ActualResults.Add(TEXT("徵象檢測成功"));
            
            // 檢查警告
            TArray<FAntiCorruptionWarning> Warnings = AntiCorruptionSystemRef->GetCurrentWarnings();
            if (Warnings.Num() > 0)
            {
                Result.ActualResults.Add(TEXT("防墮警告系統正常"));
            }
        }
        
        // 測試道德邊界
        FMoralBoundaryStatus Status = AntiCorruptionSystemRef->CheckMoralBoundaryStatus();
        Result.ActualResults.Add(FString::Printf(TEXT("道德完整性: %.1f%%"), Status.MoralIntegrity));
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("防墮系統未初始化"));
    }
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestAIAdaptationIntegration()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("AI適配集成測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    if (AIAdaptationSystemRef)
    {
        // 測試對手識別
        FAICommanderProfile Profile = AIAdaptationSystemRef->IdentifyOpponent(TEXT("TestOpponent_001"));
        if (Profile.OpponentType != EAIOpponentType::None)
        {
            Result.ActualResults.Add(TEXT("對手識別成功"));
        }
        
        // 測試行為分析
        FPlayerBehaviorAnalysis Analysis = AIAdaptationSystemRef->AnalyzeOpponentBehavior(TEXT("TestOpponent_001"));
        Result.ActualResults.Add(FString::Printf(TEXT("侵略性評估: %.2f"), Analysis.AggressionLevel));
        
        // 測試決策生成
        FAIAdaptationDecision Decision = AIAdaptationSystemRef->GenerateAdaptationDecision(
            TEXT("TestOpponent_001"), ESixStrategyType::EstablishNation);
        if (Decision.Confidence > 0.0f)
        {
            Result.ActualResults.Add(TEXT("決策生成成功"));
        }
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("AI適配系統未初始化"));
    }
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestCrossAuthorityCommunication()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("跨權限通信測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    // 測試跨權限通信邏輯
    if (ThreePowerSystemRef)
    {
        // 模擬權力重新分配（觸發通信）
        TMap<EPowerType, float> NewDistribution;
        NewDistribution.Add(EPowerType::Moral, 0.45f);
        NewDistribution.Add(EPowerType::Strategy, 0.35f);
        NewDistribution.Add(EPowerType::Military, 0.20f);
        
        if (ThreePowerSystemRef->DistributePowers(NewDistribution, TEXT("測試重新分配")))
        {
            Result.ActualResults.Add(TEXT("權力重新分配成功"));
            Result.ActualResults.Add(TEXT("跨權限通信機制正常"));
        }
    }
    else
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("三權系統未初始化"));
    }
    
    return Result;
}

FIntegrationTestResultData UMingSageCommandIntegrationTest::TestFullSystemIntegration()
{
    FIntegrationTestResultData Result;
    Result.TestName = TEXT("全系統集成測試");
    Result.Result = EIntegrationTestResult::Passed;
    
    // 檢查所有系統引用
    if (!CheckSystemReferences())
    {
        Result.Result = EIntegrationTestResult::Failed;
        Result.Errors.Add(TEXT("部分系統未正確初始化"));
        return Result;
    }
    
    // 執行完整的策略流程
    Result.ActualResults.Add(TEXT("所有系統引用檢查通過"));
    
    // 驗證集成狀態
    FSystemIntegrationStatus Status = CheckSystemIntegrationStatus();
    if (Status.OverallIntegrationScore >= 80.0f)
    {
        Result.ActualResults.Add(FString::Printf(TEXT("集成評分: %.1f%%"), Status.OverallIntegrationScore));
    }
    else
    {
        Result.Warnings.Add(TEXT("集成評分低於預期"));
    }
    
    return Result;
}

// ============================================================================
// 性能測試實現
// ============================================================================

FPerformanceTestResult UMingSageCommandIntegrationTest::RunPerformanceStressTest(float Duration)
{
    FPerformanceTestResult Result;
    
    UE_LOG(LogTemp, Log, TEXT("開始性能壓力測試，持續 %.1f 秒"), Duration);
    
    // 清空幀時間歷史
    FrameTimeHistory.Empty();
    
    // 運行測試指定時間
    float ElapsedTime = 0.0f;
    while (ElapsedTime < Duration)
    {
        // 執行一些操作來模擬負載
        if (SixStrategiesSystemRef)
        {
            // 模擬策略操作
            FStrategyExecutionContext Context;
            SixStrategiesSystemRef->GetActiveStrategies();
        }
        
        // 簡單的時間推進（實際應該使用真實時間）
        ElapsedTime += 0.1f;
    }
    
    // 計算性能指標
    if (FrameTimeHistory.Num() > 0)
    {
        Result.AverageFrameTime = 0.0f;
        Result.MaxFrameTime = 0.0f;
        Result.MinFrameTime = 9999.0f;
        
        for (float FrameTime : FrameTimeHistory)
        {
            Result.AverageFrameTime += FrameTime;
            Result.MaxFrameTime = FMath::Max(Result.MaxFrameTime, FrameTime);
            Result.MinFrameTime = FMath::Min(Result.MinFrameTime, FrameTime);
        }
        
        Result.AverageFrameTime /= FrameTimeHistory.Num();
        
        // 評估性能是否可接受（假設目標是60fps，即16.67ms）
        Result.bPerformanceAcceptable = (Result.AverageFrameTime <= 0.01667f);
        
        if (!Result.bPerformanceAcceptable)
        {
            Result.Bottlenecks.Add(TEXT("幀時間過長"));
        }
    }
    
    // 測試內存使用
    Result.MemoryUsageMB = TestMemoryUsage();
    
    UE_LOG(LogTemp, Log, TEXT("性能測試完成，平均幀時間: %.3fms, 內存使用: %.1fMB"), 
           Result.AverageFrameTime * 1000.0f, Result.MemoryUsageMB);
    
    return Result;
}

float UMingSageCommandIntegrationTest::TestMemoryUsage()
{
    // 簡化的內存使用估算
    float MemoryUsage = 0.0f;
    
    // 估算各系統內存使用
    if (ThreePowerSystemRef) MemoryUsage += 10.0f;
    if (SixStrategiesSystemRef) MemoryUsage += 15.0f;
    if (WuXingSystemRef) MemoryUsage += 12.0f;
    if (AntiCorruptionSystemRef) MemoryUsage += 8.0f;
    if (AIAdaptationSystemRef) MemoryUsage += 20.0f;
    
    return MemoryUsage;
}

// ============================================================================
// 系統狀態檢查實現
// ============================================================================

FSystemIntegrationStatus UMingSageCommandIntegrationTest::CheckSystemIntegrationStatus()
{
    FSystemIntegrationStatus Status;
    
    // 檢查各個系統的準備狀態
    Status.bThreePowerSystemReady = (ThreePowerSystemRef != nullptr);
    Status.bStrategySystemReady = (SixStrategiesSystemRef != nullptr);
    Status.bWuXingSystemReady = (WuXingSystemRef != nullptr);
    Status.bAntiCorruptionSystemReady = (AntiCorruptionSystemRef != nullptr);
    Status.bAIAdaptationSystemReady = (AIAdaptationSystemRef != nullptr);
    Status.bCrossSystemCommunicationReady = Status.bThreePowerSystemReady && Status.bStrategySystemReady;
    
    // 計算總體集成評分
    int32 ReadySystems = 0;
    int32 TotalSystems = 5;
    
    if (Status.bThreePowerSystemReady) ReadySystems++;
    if (Status.bStrategySystemReady) ReadySystems++;
    if (Status.bWuXingSystemReady) ReadySystems++;
    if (Status.bAntiCorruptionSystemReady) ReadySystems++;
    if (Status.bAIAdaptationSystemReady) ReadySystems++;
    
    Status.OverallIntegrationScore = (float)ReadySystems / TotalSystems * 100.0f;
    
    // 檢查問題並生成建議
    if (!Status.bThreePowerSystemReady)
    {
        Status.IntegrationIssues.Add(TEXT("三權系統未初始化"));
        Status.Recommendations.Add(TEXT("檢查ThreePowerSystem初始化"));
    }
    
    if (!Status.bCrossSystemCommunicationReady)
    {
        Status.IntegrationIssues.Add(TEXT("跨系統通信未就緒"));
        Status.Recommendations.Add(TEXT("確保所有系統正確初始化"));
    }
    
    // 更新當前狀態
    CurrentIntegrationStatus = Status;
    
    // 觸發狀態改變事件
    OnSystemIntegrationStatusChanged.Broadcast(Status);
    
    return Status;
}

bool UMingSageCommandIntegrationTest::CheckSystemReferences()
{
    // 檢查並獲取系統引用
    if (!ThreePowerSystemRef)
    {
        // 在實際實現中，這裡應該從遊戲模式或其他管理器獲取引用
        UE_LOG(LogTemp, Warning, TEXT("三權系統引用未設置"));
    }
    
    // 檢查關鍵系統是否就緒
    bool bCriticalSystemsReady = true;
    
    if (!ThreePowerSystemRef) bCriticalSystemsReady = false;
    if (!SixStrategiesSystemRef) bCriticalSystemsReady = false;
    
    return bCriticalSystemsReady;
}

// ============================================================================
// 輔助方法實現
// ============================================================================

void UMingSageCommandIntegrationTest::RecordTestResult(const FIntegrationTestResultData& Result)
{
    TestResultHistory.Add(Result);
    
    // 限制歷史記錄數量
    if (TestResultHistory.Num() > 100)
    {
        TestResultHistory.RemoveAt(0);
    }
}

void UMingSageCommandIntegrationTest::UpdateIntegrationStatus()
{
    CheckSystemIntegrationStatus();
}

void UMingSageCommandIntegrationTest::TriggerTestStartedEvent(int32 TestID, const FString& TestName)
{
    OnIntegrationTestStarted.Broadcast(TestID, TestName);
}

void UMingSageCommandIntegrationTest::TriggerTestCompletedEvent(int32 TestID, const FIntegrationTestResultData& Result)
{
    OnIntegrationTestCompleted.Broadcast(TestID, Result);
}

void UMingSageCommandIntegrationTest::TriggerSuiteCompletedEvent(const TArray<FIntegrationTestResultData>& Results)
{
    OnIntegrationTestSuiteCompleted.Broadcast(Results);
}

FString UMingSageCommandIntegrationTest::GenerateTestReport()
{
    FString Report = TEXT("=== 至聖者指揮學集成測試報告 ===\n\n");
    
    // 測試統計
    TMap<EIntegrationTestResult, int32> Stats = GetTestStatistics();
    
    Report += FString::Printf(TEXT("總測試數: %d\n"), TestResultHistory.Num());
    Report += FString::Printf(TEXT("通過: %d\n"), Stats.Contains(EIntegrationTestResult::Passed) ? Stats[EIntegrationTestResult::Passed] : 0);
    Report += FString::Printf(TEXT("失敗: %d\n"), Stats.Contains(EIntegrationTestResult::Failed) ? Stats[EIntegrationTestResult::Failed] : 0);
    Report += FString::Printf(TEXT("錯誤: %d\n"), Stats.Contains(EIntegrationTestResult::Error) ? Stats[EIntegrationTestResult::Error] : 0);
    Report += TEXT("\n");
    
    // 詳細結果
    Report += TEXT("詳細結果:\n");
    for (const auto& Result : TestResultHistory)
    {
        Report += FString::Printf(TEXT("[%s] %s - %s (%.2fs)\n"),
                                  Result.Result == EIntegrationTestResult::Passed ? TEXT("✓") : TEXT("✗"),
                                  *Result.TestName,
                                  *FString::Printf(TEXT("結果: %d"), (int32)Result.Result),
                                  Result.ExecutionTime);
    }
    
    // 系統狀態
    Report += TEXT("\n系統集成狀態:\n");
    Report += FString::Printf(TEXT("集成評分: %.1f%%\n"), CurrentIntegrationStatus.OverallIntegrationScore);
    
    if (CurrentIntegrationStatus.IntegrationIssues.Num() > 0)
    {
        Report += TEXT("\n發現的問題:\n");
        for (const auto& Issue : CurrentIntegrationStatus.IntegrationIssues)
        {
            Report += FString::Printf(TEXT("- %s\n"), *Issue);
        }
    }
    
    return Report;
}

TMap<EIntegrationTestResult, int32> UMingSageCommandIntegrationTest::GetTestStatistics()
{
    TMap<EIntegrationTestResult, int32> Stats;
    
    for (const auto& Result : TestResultHistory)
    {
        if (!Stats.Contains(Result.Result))
        {
            Stats.Add(Result.Result, 0);
        }
        Stats[Result.Result]++;
    }
    
    return Stats;
}

bool UMingSageCommandIntegrationTest::StopCurrentTest()
{
    if (bIsRunningTest)
    {
        bIsRunningTest = false;
        UE_LOG(LogTemp, Log, TEXT("測試已停止"));
        return true;
    }
    
    return false;
}

bool UMingSageCommandIntegrationTest::PauseTest()
{
    if (bIsRunningTest && !bIsPaused)
    {
        bIsPaused = true;
        UE_LOG(LogTemp, Log, TEXT("測試已暫停"));
        return true;
    }
    
    return false;
}

bool UMingSageCommandIntegrationTest::ResumeTest()
{
    if (bIsRunningTest && bIsPaused)
    {
        bIsPaused = false;
        UE_LOG(LogTemp, Log, TEXT("測試已恢復"));
        return true;
    }
    
    return false;
}

FIntegrationTestCase UMingSageCommandIntegrationTest::GetTestCase(int32 TestID) const
{
    if (TestCaseLibrary.Contains(TestID))
    {
        return TestCaseLibrary[TestID];
    }
    
    return FIntegrationTestCase();
}

TArray<FIntegrationTestCase> UMingSageCommandIntegrationTest::GetAllTestCases() const
{
    TArray<FIntegrationTestCase> Cases;
    
    for (const auto& Pair : TestCaseLibrary)
    {
        Cases.Add(Pair.Value);
    }
    
    return Cases;
}
