#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingRTSTestingAPI.generated.h"

/**
 * 測試系統API
 * 專門負責各種測試套件和驗證功能 (從PythonAPI分配而來)
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSTestingAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSTestingAPI();

    // 初始化測試系統
    UFUNCTION(BlueprintCallable, Category = "Testing API")
    bool InitializeTestingSystem();

    // 技能系統測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Skill System")
    FTestSuiteSummary RunSkillSystemTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Skill System")
    FTestResult TestSkillInitialization();

    UFUNCTION(BlueprintCallable, Category = "Testing|Skill System")
    FTestResult TestSkillUnlocking();

    UFUNCTION(BlueprintCallable, Category = "Testing|Skill System")
    FTestResult TestSkillExperience();

    UFUNCTION(BlueprintCallable, Category = "Testing|Skill System")
    FTestResult TestSkillSpecialization();

    // 關係系統測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Relationship System")
    FTestSuiteSummary RunRelationshipTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Relationship System")
    FTestResult TestRelationshipCreation();

    UFUNCTION(BlueprintCallable, Category = "Testing|Relationship System")
    FTestResult TestRelationshipModification();

    UFUNCTION(BlueprintCallable, Category = "Testing|Relationship System")
    FTestResult TestRelationshipDecay();

    UFUNCTION(BlueprintCallable, Category = "Testing|Relationship System")
    FTestResult TestReputationSystem();

    // 對話系統測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Dialogue System")
    FTestSuiteSummary RunDialogueSystemTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Dialogue System")
    FTestResult TestDialogueGeneration();

    UFUNCTION(BlueprintCallable, Category = "Testing|Dialogue System")
    FTestResult TestDialogueChoices();

    UFUNCTION(BlueprintCallable, Category = "Testing|Dialogue System")
    FTestResult TestDialogueEffects();

    // 歷史角色扮演測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Historical Roleplay")
    FTestSuiteSummary RunHistoricalRoleplayTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Historical Roleplay")
    FTestResult TestCharacterCreation();

    UFUNCTION(BlueprintCallable, Category = "Testing|Historical Roleplay")
    FTestResult TestHistoricalAccuracy();

    UFUNCTION(BlueprintCallable, Category = "Testing|Historical Roleplay")
    FTestResult TestRoleplayScenarios();

    // 音頻系統測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Audio System")
    FTestSuiteSummary RunAudioSystemTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Audio System")
    FTestResult TestMusicGeneration();

    UFUNCTION(BlueprintCallable, Category = "Testing|Audio System")
    FTestResult TestSoundEffects();

    UFUNCTION(BlueprintCallable, Category = "Testing|Audio System")
    FTestResult TestAudioIntegration();

    // 場景生成測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Scene Generation")
    FTestSuiteSummary RunSceneGenerationTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Scene Generation")
    FTestResult TestSceneCreation();

    UFUNCTION(BlueprintCallable, Category = "Testing|Scene Generation")
    FTestResult TestSceneTemplates();

    UFUNCTION(BlueprintCallable, Category = "Testing|Scene Generation")
    FTestResult TestSceneOptimization();

    // 資產生成測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Asset Generation")
    FTestSuiteSummary RunAssetGenerationTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Asset Generation")
    FTestResult TestEpicAssetGeneration();

    UFUNCTION(BlueprintCallable, Category = "Testing|Asset Generation")
    FTestResult TestGameAssetGeneration();

    UFUNCTION(BlueprintCallable, Category = "Testing|Asset Generation")
    FTestResult TestAssetQuality();

    // 系統集成測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Integration")
    FTestSuiteSummary RunIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Integration")
    FTestResult TestAPISystemIntegration();

    UFUNCTION(BlueprintCallable, Category = "Testing|Integration")
    FTestResult TestCrossSystemCommunication();

    UFUNCTION(BlueprintCallable, Category = "Testing|Integration")
    FTestResult TestDataConsistency();

    // 性能測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Performance")
    FTestSuiteSummary RunPerformanceTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Performance")
    FTestResult TestSystemPerformance();

    UFUNCTION(BlueprintCallable, Category = "Testing|Performance")
    FTestResult TestMemoryUsage();

    UFUNCTION(BlueprintCallable, Category = "Testing|Performance")
    FTestResult TestLoadBearing();

    // 壓力測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Stress")
    FTestSuiteSummary RunStressTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Stress")
    FTestResult TestHighVolumeOperations();

    UFUNCTION(BlueprintCallable, Category = "Testing|Stress")
    FTestResult TestLongRunningStability();

    UFUNCTION(BlueprintCallable, Category = "Testing|Stress")
    FTestResult TestResourceLimits();

    // 兼容性測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Compatibility")
    FTestSuiteSummary RunCompatibilityTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Compatibility")
    FTestResult TestVersionCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Testing|Compatibility")
    FTestResult TestPlatformCompatibility();

    UFUNCTION(BlueprintCallable, Category = "Testing|Compatibility")
    FTestResult TestAPICompatibility();

    // 自動化測試
    UFUNCTION(BlueprintCallable, Category = "Testing|Automation")
    bool RunAllAutomatedTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Automation")
    bool ScheduleAutomatedTestRun(const FDateTime& ScheduledTime);

    UFUNCTION(BlueprintCallable, Category = "Testing|Automation")
    bool RunContinuousIntegrationTests();

    UFUNCTION(BlueprintCallable, Category = "Testing|Automation")
    TArray<FTestSuiteSummary> GetRecentTestResults();

    // 測試報告
    UFUNCTION(BlueprintCallable, Category = "Testing|Reporting")
    bool GenerateTestReport(const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "Testing|Reporting")
    bool GenerateDetailedTestReport(const FString& OutputPath);

    UFUNCTION(BlueprintCallable, Category = "Testing|Reporting")
    FString GetTestSummary();

    UFUNCTION(BlueprintCallable, Category = "Testing|Reporting")
    TArray<FTestSuiteSummary> GetAllTestSummaries();

    // 測試配置
    UFUNCTION(BlueprintCallable, Category = "Testing|Configuration")
    bool LoadTestConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Testing|Configuration")
    bool SaveTestConfiguration(const FString& ConfigPath);

    UFUNCTION(BlueprintCallable, Category = "Testing|Configuration")
    void SetTestParameters(const FTestParameters& Parameters);

    UFUNCTION(BlueprintCallable, Category = "Testing|Configuration")
    FTestParameters GetTestParameters();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestStarted, const FString&, TestName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnTestCompleted, const FString&, TestName, const FTestResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FTestSuiteSummary&, Summary);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnTestFailed, const FString&, TestName, const FString&, ErrorMessage);

    UPROPERTY(BlueprintAssignable, Category = "Testing|Events")
    FOnTestStarted OnTestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Testing|Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Testing|Events")
    FOnTestSuiteCompleted OnTestSuiteCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Testing|Events")
    FOnTestFailed OnTestFailed;

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Testing|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Testing|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Testing|Status")
    TArray<FString> GetAvailableTestSuites() const;

    UFUNCTION(BlueprintCallable, Category = "Testing|Status")
    bool IsTestRunning() const;

private:
    bool bIsInitialized;
    
    // 測試配置
    FTestParameters TestParameters;
    
    // 測試結果歷史
    TArray<FTestSuiteSummary> TestHistory;
    
    // 當前運行的測試
    FString CurrentRunningTest;
    bool bTestRunning;

    // 內部輔助函數
    bool LoadTestConfiguration();
    bool InitializeTestEnvironment();
    void CleanupTestEnvironment();
    
    FTestResult RunIndividualTest(const FString& TestName, TFunction<bool()> TestFunction);
    void LogTestResult(const FTestResult& Result);
    void RecordTestSuite(const FTestSuiteSummary& Summary);
    
    // 測試斷言輔助函數
    bool AssertTrue(bool Condition, const FString& Message, FTestResult& Result);
    bool AssertEquals(const FString& Expected, const FString& Actual, const FString& Message, FTestResult& Result);
    bool AssertNotNull(const UObject* Object, const FString& Message, FTestResult& Result);
    bool AssertFileExists(const FString& FilePath, const FString& Message, FTestResult& Result);
    
    // 測試環境設置
    bool SetupTestEnvironment(const FString& TestType);
    void CleanupTestEnvironment(const FString& TestType);
    
    // 數據持久化
    TSharedPtr<FJsonObject> SaveTestResults() const;
    bool LoadTestResults(TSharedPtr<FJsonObject> TestResultsData);
};

// 測試結果狀態枚舉
UENUM(BlueprintType)
enum class ETestResult : uint8
{
    NotRun      UMETA(DisplayName = "Not Run"),
    Passed      UMETA(DisplayName = "Passed"),
    Failed      UMETA(DisplayName = "Failed"),
    Skipped     UMETA(DisplayName = "Skipped"),
    Error       UMETA(DisplayName = "Error")
};

// 測試類型枚舉
UENUM(BlueprintType)
enum class ETestType : uint8
{
    Unit        UMETA(DisplayName = "Unit"),
    Integration UMETA(DisplayName = "Integration"),
    Performance UMETA(DisplayName = "Performance"),
    Stress      UMETA(DisplayName = "Stress"),
    Compatibility UMETA(DisplayName = "Compatibility"),
    Automation  UMETA(DisplayName = "Automation")
};

// 測試優先級枚舉
UENUM(BlueprintType)
enum class ETestPriority : uint8
{
    Critical    UMETA(DisplayName = "Critical"),
    High        UMETA(DisplayName = "High"),
    Medium      UMETA(DisplayName = "Medium"),
    Low         UMETA(DisplayName = "Low")
};

// 測試結果結構
USTRUCT(BlueprintType)
struct FTEST_RESULT
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FString TestName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    ETestResult Result;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FString ErrorMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    float ExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    int32 AssertionsRun;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    ETestType TestType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    ETestPriority Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Result")
    TArray<FString> Tags;

    FTEST_RESULT()
    {
        TestName = TEXT("");
        Result = ETestResult::NotRun;
        ErrorMessage = TEXT("");
        ExecutionTime = 0.0f;
        AssertionsRun = 0;
        TestType = ETestType::Unit;
        Priority = ETestPriority::Medium;
    }
};

// 測試套件摘要結構
USTRUCT(BlueprintType)
struct FTEST_SUITE_SUMMARY
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    FString SuiteName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    TArray<FTestResult> TestResults;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    int32 TotalTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    int32 PassedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    int32 FailedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    int32 SkippedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    int32 ErrorTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    float TotalExecutionTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    float SuccessRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Suite Summary")
    FDateTime EndTime;

    FTEST_SUITE_SUMMARY()
    {
        SuiteName = TEXT("");
        TotalTests = 0;
        PassedTests = 0;
        FailedTests = 0;
        SkippedTests = 0;
        ErrorTests = 0;
        TotalExecutionTime = 0.0f;
        SuccessRate = 0.0f;
    }
};

// 測試參數結構
USTRUCT(BlueprintType)
struct FTEST_PARAMETERS
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    bool bEnableVerboseLogging;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    bool bStopOnFirstFailure;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    bool bGenerateDetailedReports;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    float TestTimeout;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    int32 MaxConcurrentTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    TArray<FString> TestCategories;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    TArray<FString> ExcludedTests;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test Parameters")
    FString OutputDirectory;

    FTEST_PARAMETERS()
    {
        bEnableVerboseLogging = false;
        bStopOnFirstFailure = false;
        bGenerateDetailedReports = true;
        TestTimeout = 300.0f;
        MaxConcurrentTests = 1;
        OutputDirectory = TEXT("");
    }
};
