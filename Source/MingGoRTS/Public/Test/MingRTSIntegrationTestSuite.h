// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 系統整合測試套件頭文件

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"
#include "MingRTSIntegrationTestSuite.generated.h"

/**
 * 整合測試類型
 */
UENUM(BlueprintType)
enum class EIntegrationTestType : uint8
{
    SystemInitialization     UMETA(DisplayName = "系統初始化"),
    CrossSystemDataFlow      UMETA(DisplayName = "跨系統數據流"),
    EventPropagation         UMETA(DisplayName = "事件傳播"),
    PerformanceStress        UMETA(DisplayName = "性能壓力測試"),
    MemoryLeakDetection      UMETA(DisplayName = "內存洩漏檢測"),
    ConcurrentAccess         UMETA(DisplayName = "並發訪問"),
    ErrorRecovery            UMETA(DisplayName = "錯誤恢復"),
    SaveLoadIntegration      UMETA(DisplayName = "存檔載入整合"),
    NetworkSynchronization   UMETA(DisplayName = "網絡同步"),
    UISystemIntegration      UMETA(DisplayName = "UI系統整合"),
    AudioSystemIntegration     UMETA(DisplayName = "音頻系統整合"),
    AIBehaviorIntegration    UMETA(DisplayName = "AI行為整合"),
    LocalizationIntegration  UMETA(DisplayName = "本地化整合"),
    RegressionTest           UMETA(DisplayName = "回歸測試"),
    EndToEndScenario         UMETA(DisplayName = "端到端場景")
};

/**
 * 測試結果狀態
 */
UENUM(BlueprintType)
enum class EIntegrationTestStatus : uint8
{
    NotStarted    UMETA(DisplayName = "未開始"),
    Running       UMETA(DisplayName = "運行中"),
    Passed        UMETA(DisplayName = "通過"),
    Failed        UMETA(DisplayName = "失敗"),
    Warning       UMETA(DisplayName = "警告"),
    Skipped       UMETA(DisplayName = "已跳過")
};

/**
 * 性能測試指標
 */
USTRUCT(BlueprintType)
struct FPerformanceMetrics
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AverageFrameTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MemoryUsageMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CPUUsagePercent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ObjectCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TextureMemoryMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MeshMemoryMB;
    
    FPerformanceMetrics()
        : AverageFPS(60.0f)
        , MinFPS(60.0f)
        , MaxFPS(60.0f)
        , AverageFrameTime(16.67f)
        , MemoryUsageMB(0.0f)
        , CPUUsagePercent(0.0f)
        , ObjectCount(0)
        , TextureMemoryMB(0)
        , MeshMemoryMB(0)
    {}
};

/**
 * 內存快照
 */
USTRUCT(BlueprintType)
struct FMemorySnapshot
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalVirtualMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 TotalPhysicalMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 AvailableVirtualMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 AvailablePhysicalMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 ProcessMemoryUsage;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime SnapshotTime;
    
    FMemorySnapshot()
        : TotalVirtualMemory(0)
        , TotalPhysicalMemory(0)
        , AvailableVirtualMemory(0)
        , AvailablePhysicalMemory(0)
        , ProcessMemoryUsage(0)
    {}
};

/**
 * 單個測試結果
 */
USTRUCT(BlueprintType)
struct FIntegrationTestResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EIntegrationTestType TestType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EIntegrationTestStatus Status;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString TestName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTimeSeconds;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FPerformanceMetrics PerformanceData;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> ErrorMessages;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> WarningMessages;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> CustomData;
    
    FIntegrationTestResult()
        : TestType(EIntegrationTestType::SystemInitialization)
        , Status(EIntegrationTestStatus::NotStarted)
        , ExecutionTimeSeconds(0.0f)
    {}
};

/**
 * 系統對測試結果
 */
USTRUCT(BlueprintType)
struct FSystemPairTestResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SystemA;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SystemB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCommunicationWorking;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDataTransferWorking;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DataTransferLatency;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CompatibilityIssues;
    
    FSystemPairTestResult()
        : bCommunicationWorking(false)
        , bDataTransferWorking(false)
        , DataTransferLatency(0.0f)
    {}
};

/**
 * 壓力測試配置
 */
USTRUCT(BlueprintType)
struct FStressTestConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ConcurrentUnitCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SimulatedPlayerCount;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TestDurationSeconds;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TargetFPS;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxMemoryUsageMB;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableNetworkSimulation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float NetworkLatencyMs;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableMemoryStress;
    
    FStressTestConfig()
        : ConcurrentUnitCount(100)
        , SimulatedPlayerCount(8)
        , TestDurationSeconds(300.0f)
        , TargetFPS(30.0f)
        , MaxMemoryUsageMB(2048.0f)
        , bEnableNetworkSimulation(false)
        , NetworkLatencyMs(50.0f)
        , bEnableMemoryStress(false)
    {}
};

/**
 * 測試套件配置
 */
USTRUCT(BlueprintType)
struct FIntegrationTestSuiteConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SuiteName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<EIntegrationTestType> EnabledTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FStressTestConfig StressConfig;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bGenerateDetailedReport;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSaveResultsToFile;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OutputPath;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStopOnFirstFailure;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRunMemoryLeakDetection;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRunRegressionTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SystemPairsToTest;
    
    FIntegrationTestSuiteConfig()
        : SuiteName(TEXT("MingGoRTS 整合測試套件"))
        , bGenerateDetailedReport(true)
        , bSaveResultsToFile(true)
        , bStopOnFirstFailure(false)
        , bRunMemoryLeakDetection(true)
        , bRunRegressionTests(true)
    {}
};

/**
 * 完整測試套件結果
 */
USTRUCT(BlueprintType)
struct FIntegrationTestSuiteResult
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString SuiteName;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime StartTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime EndTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalExecutionTime;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FIntegrationTestResult> TestResults;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FSystemPairTestResult> SystemPairResults;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TotalTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PassedTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 FailedTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 WarningTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SkippedTests;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SuccessRate;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMemorySnapshot StartMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FMemorySnapshot EndMemory;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int64 MemoryDelta;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ReportFilePath;
    
    FIntegrationTestSuiteResult()
        : TotalExecutionTime(0.0f)
        , TotalTests(0)
        , PassedTests(0)
        , FailedTests(0)
        , WarningTests(0)
        , SkippedTests(0)
        , SuccessRate(0.0f)
        , MemoryDelta(0)
    {}
};

/**
 * 系統整合測試套件
 * 執行跨系統功能測試、性能壓力測試、內存洩漏檢測
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGGORTS_API UMingRTSIntegrationTestSuite : public UObject
{
    GENERATED_BODY()
    
public:
    /** 初始化測試套件 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    void InitializeTestSuite();
    
    /** 執行完整測試套件 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestSuiteResult RunFullTestSuite(const FIntegrationTestSuiteConfig& Config);
    
    /** 執行單個測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult RunSingleTest(EIntegrationTestType TestType);
    
    /** 執行系統初始化測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestSystemInitialization();
    
    /** 執行跨系統數據流測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestCrossSystemDataFlow();
    
    /** 執行事件傳播測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestEventPropagation();
    
    /** 執行性能壓力測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult RunPerformanceStressTest(const FStressTestConfig& Config);
    
    /** 執行內存洩漏檢測 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult RunMemoryLeakDetection();
    
    /** 執行並發訪問測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestConcurrentAccess();
    
    /** 執行錯誤恢復測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestErrorRecovery();
    
    /** 執行存檔載入整合測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestSaveLoadIntegration();
    
    /** 執行網絡同步測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestNetworkSynchronization();
    
    /** 執行UI系統整合測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestUISystemIntegration();
    
    /** 執行音頻系統整合測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestAudioSystemIntegration();
    
    /** 執行AI行為整合測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestAIBehaviorIntegration();
    
    /** 執行本地化整合測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult TestLocalizationIntegration();
    
    /** 執行回歸測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult RunRegressionTest();
    
    /** 執行端到端場景測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestResult RunEndToEndScenarioTest();
    
    /** 測試系統對之間的通信 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FSystemPairTestResult TestSystemPairCommunication(const FString& SystemA, const FString& SystemB);
    
    /** 生成測試報告 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FString GenerateTestReport(const FIntegrationTestSuiteResult& Result, bool bAsHTML = true);
    
    /** 導出結果到JSON */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    bool ExportResultsToJSON(const FIntegrationTestSuiteResult& Result, const FString& FilePath);
    
    /** 獲取預設測試配置 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FIntegrationTestSuiteConfig GetDefaultTestConfig();
    
    /** 獲取壓力測試配置 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    FStressTestConfig GetDefaultStressConfig();
    
    /** 獲取測試統計 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    TMap<EIntegrationTestStatus, int32> GetTestStatistics(const FIntegrationTestSuiteResult& Result);
    
    /** 獲取失敗的測試 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    TArray<FIntegrationTestResult> GetFailedTests(const FIntegrationTestSuiteResult& Result);
    
    /** 檢查是否所有測試通過 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    bool AreAllTestsPassed(const FIntegrationTestSuiteResult& Result);
    
    /** 計算測試成功率 */
    UFUNCTION(BlueprintCallable, Category = "Integration Testing")
    float CalculateSuccessRate(const FIntegrationTestSuiteResult& Result);
    
    /** 事件：測試套件開始 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Testing Events")
    FOnIntegrationTestStarted OnTestSuiteStarted;
    
    /** 事件：單個測試完成 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Testing Events")
    FOnSingleTestCompleted OnSingleTestCompleted;
    
    /** 事件：發現問題 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Testing Events")
    FOnTestIssueFound OnTestIssueFound;
    
    /** 事件：測試套件完成 */
    UPROPERTY(BlueprintAssignable, Category = "Integration Testing Events")
    FOnTestSuiteCompleted OnTestSuiteCompleted;
    
protected:
    /** 捕獲內存快照 */
    FMemorySnapshot CaptureMemorySnapshot();
    
    /** 比較內存快照 */
    bool CompareMemorySnapshots(const FMemorySnapshot& Before, const FMemorySnapshot& After, int64& OutDelta);
    
    /** 收集性能指標 */
    FPerformanceMetrics CollectPerformanceMetrics(float Duration);
    
    /** 驗證性能指標 */
    bool ValidatePerformanceMetrics(const FPerformanceMetrics& Metrics, const FStressTestConfig& Config);
    
    /** 模擬高負載場景 */
    void SimulateHighLoad(const FStressTestConfig& Config);
    
    /** 清理測試資源 */
    void CleanupTestResources();
    
    /** 生成HTML報告 */
    FString GenerateHTMLReport(const FIntegrationTestSuiteResult& Result);
    
    /** 生成文本報告 */
    FString GenerateTextReport(const FIntegrationTestSuiteResult& Result);
    
    /** 發送測試通知 */
    void SendTestNotification(const FIntegrationTestSuiteResult& Result);
    
    /** 記錄測試開始 */
    void LogTestStart(const FString& TestName);
    
    /** 記錄測試完成 */
    void LogTestComplete(const FString& TestName, EIntegrationTestStatus Status, float Duration);
    
private:
    /** 當前是否正在運行測試 */
    bool bIsRunningTests;
    
    /** 當前測試配置 */
    FIntegrationTestSuiteConfig CurrentConfig;
    
    /** 測試結果緩存 */
    TArray<FIntegrationTestResult> CachedResults;
};

/** 測試開始事件委託 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIntegrationTestStarted, const FString&, SuiteName);

/** 單個測試完成事件委託 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSingleTestCompleted, EIntegrationTestType, TestType, const FIntegrationTestResult&, Result);

/** 測試問題發現事件委託 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTestIssueFound, EIntegrationTestType, TestType, const FString&, IssueMessage);

/** 測試套件完成事件委託 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FIntegrationTestSuiteResult&, Result);
