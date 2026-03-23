#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingSageCommandIntegrationTest.generated.h"

// ============================================================================
// 前向聲明
// ============================================================================

class UMingThreePowerSystem;
class UMingMoralAuthority;
class UMingStrategyAuthority;
class UMingMilitaryAuthority;
class UMingSixStrategiesSystem;
class UMingWuXingRhythmSystem;
class UMingAntiCorruptionSystem;
class UMingAIAdaptationSystem;

// ============================================================================
// 集成測試枚舉定義
// ============================================================================

/** 集成測試類型 */
UENUM(BlueprintType)
enum class EIntegrationTestType : uint8
{
    None                        UMETA(DisplayName = "無"),
    ThreePowerIntegration       UMETA(DisplayName = "三權系統集成"),
    StrategyExecutionFlow       UMETA(DisplayName = "策略執行流程"),
    WuXingRhythmCoordination    UMETA(DisplayName = "五行節奏協調"),
    AntiCorruptionIntegration   UMETA(DisplayName = "防墮機制集成"),
    AIAdaptationIntegration     UMETA(DisplayName = "AI適配集成"),
    CrossAuthorityCommunication UMETA(DisplayName = "跨權限通信"),
    FullSystemIntegration       UMETA(DisplayName = "全系統集成"),
    PerformanceStressTest       UMETA(DisplayName = "性能壓力測試"),
    ErrorRecoveryTest           UMETA(DisplayName = "錯誤恢復測試")
};

/** 測試結果狀態 */
UENUM(BlueprintType)
enum class EIntegrationTestResult : uint8
{
    NotRun      UMETA(DisplayName = "未運行"),
    Running     UMETA(DisplayName = "運行中"),
    Passed      UMETA(DisplayName = "通過"),
    Failed      UMETA(DisplayName = "失敗"),
    Partial     UMETA(DisplayName = "部分通過"),
    Error       UMETA(DisplayName = "錯誤"),
    Timeout     UMETA(DisplayName = "超時")
};

/** 測試嚴重程度 */
UENUM(BlueprintType)
enum class ETestSeverity : uint8
{
    Low         UMETA(DisplayName = "低"),
    Medium      UMETA(DisplayName = "中"),
    High        UMETA(DisplayName = "高"),
    Critical    UMETA(DisplayName = "關鍵")
};

// ============================================================================
// 數據結構定義
// ============================================================================

/** 集成測試案例 */
USTRUCT(BlueprintType)
struct FIntegrationTestCase
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    int32 TestID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    FString TestName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    EIntegrationTestType TestType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    ETestSeverity Severity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    TArray<FString> TestSteps;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    TArray<FString> ExpectedResults;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    TArray<FString> RequiredSystems;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    float TimeoutDuration;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Case")
    bool bIsAutomated;
    
    FIntegrationTestCase()
        : TestID(0)
        , TestType(EIntegrationTestType::None)
        , Severity(ETestSeverity::Medium)
        , TimeoutDuration(30.0f)
        , bIsAutomated(true)
    {}
};

/** 集成測試結果 */
USTRUCT(BlueprintType)
struct FIntegrationTestResultData
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    int32 TestID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    EIntegrationTestResult Result;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    FString TestName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    float ExecutionTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    TArray<FString> ActualResults;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    TArray<FString> Errors;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    TArray<FString> Warnings;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    FString DetailedReport;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    FDateTime ExecutionTimeStamp;
    
    UPROPERTY(BlueprintReadWrite, Category = "Test Result")
    float PerformanceMetrics;
    
    FIntegrationTestResultData()
        : TestID(0)
        , Result(EIntegrationTestResult::NotRun)
        , ExecutionTime(0.0f)
        , PerformanceMetrics(0.0f)
    {}
};

/** 系統集成狀態 */
USTRUCT(BlueprintType)
struct FSystemIntegrationStatus
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bThreePowerSystemReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bStrategySystemReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bWuXingSystemReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bAntiCorruptionSystemReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bAIAdaptationSystemReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    bool bCrossSystemCommunicationReady;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    float OverallIntegrationScore;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    TArray<FString> IntegrationIssues;
    
    UPROPERTY(BlueprintReadWrite, Category = "Integration Status")
    TArray<FString> Recommendations;
    
    FSystemIntegrationStatus()
        : bThreePowerSystemReady(false)
        , bStrategySystemReady(false)
        , bWuXingSystemReady(false)
        , bAntiCorruptionSystemReady(false)
        , bAIAdaptationSystemReady(false)
        , bCrossSystemCommunicationReady(false)
        , OverallIntegrationScore(0.0f)
    {}
};

/** 性能測試結果 */
USTRUCT(BlueprintType)
struct FPerformanceTestResult
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float AverageFrameTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float MaxFrameTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float MinFrameTime;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float MemoryUsageMB;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    float CPUUsagePercent;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    int32 ConcurrentOperations;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    bool bPerformanceAcceptable;
    
    UPROPERTY(BlueprintReadWrite, Category = "Performance")
    TArray<FString> Bottlenecks;
    
    FPerformanceTestResult()
        : AverageFrameTime(0.0f)
        , MaxFrameTime(0.0f)
        , MinFrameTime(9999.0f)
        , MemoryUsageMB(0.0f)
        , CPUUsagePercent(0.0f)
        , ConcurrentOperations(0)
        , bPerformanceAcceptable(true)
    {}
};

// ============================================================================
// 委託聲明
// ============================================================================

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntegrationTestStarted,
    int32, TestID,
    const FString&, TestName);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIntegrationTestCompleted,
    int32, TestID,
    const FIntegrationTestResultData&, ResultData);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationTestSuiteCompleted,
    const TArray<FIntegrationTestResultData>&, AllResults);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSystemIntegrationStatusChanged,
    const FSystemIntegrationStatus&, NewStatus);

// ============================================================================
// 主系統類別
// ============================================================================

/**
 * 至聖者指揮學集成測試系統
 * 負責跨系統集成測試、性能測試、系統協調驗證
 */
UCLASS(BlueprintType, Blueprintable, Category = "MingGoRTS|Sage Command|Integration Test")
class MINGGORTS_API UMingSageCommandIntegrationTest : public UObject
{
    GENERATED_BODY()

public:
    // ============================================================================
    // 構造函數與初始化
    // ============================================================================

    UMingSageCommandIntegrationTest();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 初始化集成測試系統 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    bool InitializeIntegrationTestSystem();

    /** 重置測試系統 */
    UFUNCTION(BlueprintCallable, Category = "Integration Test")
    void ResetIntegrationTestSystem();

    // ============================================================================
    // 測試案例管理
    // ============================================================================

    /** 創建測試案例 */
    UFUNCTION(BlueprintCallable, Category = "Test Management")
    int32 CreateTestCase(const FString& TestName, EIntegrationTestType TestType, ETestSeverity Severity);

    /** 刪除測試案例 */
    UFUNCTION(BlueprintCallable, Category = "Test Management")
    bool RemoveTestCase(int32 TestID);

    /** 獲取測試案例 */
    UFUNCTION(BlueprintCallable, Category = "Test Management")
    FIntegrationTestCase GetTestCase(int32 TestID) const;

    /** 獲取所有測試案例 */
    UFUNCTION(BlueprintCallable, Category = "Test Management")
    TArray<FIntegrationTestCase> GetAllTestCases() const;

    /** 更新測試案例 */
    UFUNCTION(BlueprintCallable, Category = "Test Management")
    bool UpdateTestCase(int32 TestID, const FIntegrationTestCase& UpdatedCase);

    // ============================================================================
    // 測試執行
    // ============================================================================

    /** 運行單個測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    FIntegrationTestResultData RunSingleTest(int32 TestID);

    /** 運行測試套件 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    TArray<FIntegrationTestResultData> RunTestSuite(const TArray<int32>& TestIDs);

    /** 運行所有測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    TArray<FIntegrationTestResultData> RunAllTests();

    /** 運行特定類型的測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    TArray<FIntegrationTestResultData> RunTestsByType(EIntegrationTestType TestType);

    /** 停止當前測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    bool StopCurrentTest();

    /** 暫停測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    bool PauseTest();

    /** 恢復測試 */
    UFUNCTION(BlueprintCallable, Category = "Test Execution")
    bool ResumeTest();

    // ============================================================================
    // 特定集成測試
    // ============================================================================

    /** 測試三權系統集成 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestThreePowerIntegration();

    /** 測試策略執行流程 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestStrategyExecutionFlow();

    /** 測試五行節奏協調 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestWuXingRhythmCoordination();

    /** 測試防墮機制集成 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestAntiCorruptionIntegration();

    /** 測試AI適配集成 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestAIAdaptationIntegration();

    /** 測試跨權限通信 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestCrossAuthorityCommunication();

    /** 測試全系統集成 */
    UFUNCTION(BlueprintCallable, Category = "Specific Tests")
    FIntegrationTestResultData TestFullSystemIntegration();

    // ============================================================================
    // 性能測試
    // ============================================================================

    /** 運行性能壓力測試 */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult RunPerformanceStressTest(float Duration = 60.0f);

    /** 測試並發操作性能 */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FPerformanceTestResult TestConcurrentOperations(int32 OperationCount = 100);

    /** 測試內存使用 */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    float TestMemoryUsage();

    /** 測試CPU使用率 */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    float TestCPUUsage(float Duration = 10.0f);

    /** 生成性能報告 */
    UFUNCTION(BlueprintCallable, Category = "Performance Test")
    FString GeneratePerformanceReport();

    // ============================================================================
    // 系統狀態檢查
    // ============================================================================

    /** 檢查系統集成狀態 */
    UFUNCTION(BlueprintCallable, Category = "System Status")
    FSystemIntegrationStatus CheckSystemIntegrationStatus();

    /** 驗證系統準備就緒 */
    UFUNCTION(BlueprintCallable, Category = "System Status")
    bool VerifySystemReadiness(const TArray<FString>& RequiredSystems);

    /** 獲取系統健康評分 */
    UFUNCTION(BlueprintCallable, Category = "System Status")
    float GetSystemHealthScore();

    /** 診斷系統問題 */
    UFUNCTION(BlueprintCallable, Category = "System Status")
    TArray<FString> DiagnoseSystemIssues();

    // ============================================================================
    // 測試結果分析
    // ============================================================================

    /** 分析測試結果 */
    UFUNCTION(BlueprintCallable, Category = "Result Analysis")
    FString AnalyzeTestResults(const TArray<FIntegrationTestResultData>& Results);

    /** 生成測試報告 */
    UFUNCTION(BlueprintCallable, Category = "Result Analysis")
    FString GenerateTestReport();

    /** 導出測試結果 */
    UFUNCTION(BlueprintCallable, Category = "Result Analysis")
    bool ExportTestResults(const FString& FilePath);

    /** 比較測試結果 */
    UFUNCTION(BlueprintCallable, Category = "Result Analysis")
    TArray<FString> CompareTestResults(const FIntegrationTestResultData& Result1, 
                                      const FIntegrationTestResultData& Result2);

    /** 獲取測試統計 */
    UFUNCTION(BlueprintCallable, Category = "Result Analysis")
    TMap<EIntegrationTestResult, int32> GetTestStatistics();

    // ============================================================================
    // 事件委託
    // ============================================================================

    /** 測試開始事件 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Test Events")
    FOnIntegrationTestStarted OnIntegrationTestStarted;

    /** 測試完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Test Events")
    FOnIntegrationTestCompleted OnIntegrationTestCompleted;

    /** 測試套件完成事件 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Test Events")
    FOnIntegrationTestSuiteCompleted OnIntegrationTestSuiteCompleted;

    /** 系統集成狀態改變事件 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Test Events")
    FOnSystemIntegrationStatusChanged OnSystemIntegrationStatusChanged;

protected:
    // ============================================================================
    // 內部狀態
    // ============================================================================

    /** 測試案例庫 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TMap<int32, FIntegrationTestCase> TestCaseLibrary;

    /** 測試結果歷史 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    TArray<FIntegrationTestResultData> TestResultHistory;

    /** 當前測試結果 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FIntegrationTestResultData CurrentTestResult;

    /** 系統引用 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    UMingThreePowerSystem* ThreePowerSystemRef;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    UMingSixStrategiesSystem* SixStrategiesSystemRef;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    UMingWuXingRhythmSystem* WuXingSystemRef;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    UMingAntiCorruptionSystem* AntiCorruptionSystemRef;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    UMingAIAdaptationSystem* AIAdaptationSystemRef;

    /** 系統參數 */
    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    int32 NextTestID;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    bool bIsRunningTest;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    bool bIsPaused;

    UPROPERTY(BlueprintReadWrite, Category = "Integration Test")
    float TestTimeoutDefault;

    /** 系統狀態 */
    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    bool bSystemInitialized;

    UPROPERTY(BlueprintReadOnly, Category = "Integration Test")
    FSystemIntegrationStatus CurrentIntegrationStatus;

    // ============================================================================
    // 內部方法
    // ============================================================================

    /** 初始化測試案例庫 */
    void InitializeTestCaseLibrary();

    /** 執行測試步驟 */
    bool ExecuteTestStep(const FString& Step, const FString& ExpectedResult);

    /** 驗證測試結果 */
    bool VerifyTestResult(const FString& ActualResult, const FString& ExpectedResult);

    /** 記錄測試結果 */
    void RecordTestResult(const FIntegrationTestResultData& Result);

    /** 更新系統集成狀態 */
    void UpdateIntegrationStatus();

    /** 檢查系統引用 */
    bool CheckSystemReferences();

    /** 清理測試環境 */
    void CleanupTestEnvironment();

    /** 觸發測試開始事件 */
    void TriggerTestStartedEvent(int32 TestID, const FString& TestName);

    /** 觸發測試完成事件 */
    void TriggerTestCompletedEvent(int32 TestID, const FIntegrationTestResultData& Result);

    /** 觸發套件完成事件 */
    void TriggerSuiteCompletedEvent(const TArray<FIntegrationTestResultData>& Results);

    /** 計算集成評分 */
    float CalculateIntegrationScore();

private:
    /** 定時器句柄 */
    FTimerHandle StatusCheckTimer;
    FTimerHandle PerformanceMonitorTimer;

    /** 性能監控數據 */
    TArray<float> FrameTimeHistory;
    
    /** 最大歷史記錄數 */
    static constexpr int32 MAX_FRAME_HISTORY = 100;

    /** 默認測試超時 */
    static constexpr float DEFAULT_TEST_TIMEOUT = 30.0f;
};
