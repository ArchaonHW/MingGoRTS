#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingSageBrainCoreSystem.h"
#include "MingSageBrainTestSuite.generated.h"

// 測試結果結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FString TestName;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    bool bPassed;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FString Details;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FDateTime Timestamp;

    FSageBrainTestResult()
    {
        TestName = TEXT("");
        bPassed = false;
        Details = TEXT("");
        ExecutionTime = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 測試套件摘要
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainTestSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 TotalTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 PassedTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 FailedTests;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    float SuccessRate;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    float TotalExecutionTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    FDateTime EndTime;

    FSageBrainTestSummary()
    {
        TotalTests = 0;
        PassedTests = 0;
        FailedTests = 0;
        SuccessRate = 0.0f;
        TotalExecutionTime = 0.0f;
        StartTime = FDateTime::Now();
        EndTime = FDateTime::Now();
    }
};

/**
 * 聖者大腦完整測試套件
 * 提供全面的系統測試功能
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainTestSuite : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainTestSuite();

    // 初始化測試套件
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool InitializeTestSuite(UMingSageBrainCoreSystem* SageBrainSystem);

    // 運行完整測試套件
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FSageBrainTestSummary RunCompleteTestSuite();

    // 運行特定測試類別
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FSageBrainTestSummary RunTestCategory(const FString& Category);

    // 運行單個測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FSageBrainTestResult RunSingleTest(const FString& TestName);

    // 核心系統測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Core")
    FSageBrainTestResult TestCoreSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Core")
    FSageBrainTestResult TestSystemShutdown();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Core")
    FSageBrainTestResult TestSystemStatus();

    // 思考系統測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestTacticalThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestStrategicThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestPhilosophicalThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestHistoricalThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestCulturalThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestInnovativeThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestPredictiveThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestQuantumThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestTranscendentalThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Thinking")
    FSageBrainTestResult TestDeepThinkingIntegration();

    // 學習系統測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestReinforcementLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestNeuralNetworkLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestDeepLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestEvolutionaryLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestBayesianLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestQuantumLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestEnsembleLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Learning")
    FSageBrainTestResult TestLearningPerformanceOptimization();

    // 意識系統測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Consciousness")
    FSageBrainTestResult TestConsciousnessStateUpdate();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Consciousness")
    FSageBrainTestResult TestSelfReflection();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Consciousness")
    FSageBrainTestResult TestSelfAssessment();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Consciousness")
    FSageBrainTestResult TestConsciousnessEvolution();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Consciousness")
    FSageBrainTestResult TestConsciousnessLevels();

    // 哲學系統測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestConfucianismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestTaoismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestBuddhismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestLegalismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestMohismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestMilitaryStrategyPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestYinYangPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestSyncretismPhilosophy();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Philosophy")
    FSageBrainTestResult TestPhilosophyIntegration();

    // API協調測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Coordination")
    FSageBrainTestResult TestSystemAPICoordination();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Coordination")
    FSageBrainTestResult TestSubsystemRegistration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Coordination")
    FSageBrainTestResult TestSubsystemRetrieval();

    // 性能測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Performance")
    FSageBrainTestResult TestThinkingPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Performance")
    FSageBrainTestResult TestLearningPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Performance")
    FSageBrainTestResult TestConsciousnessPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Performance")
    FSageBrainTestResult TestPhilosophyPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Performance")
    FSageBrainTestResult TestOverallSystemPerformance();

    // 壓力測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Stress")
    FSageBrainTestResult TestHighVolumeThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Stress")
    FSageBrainTestResult TestConcurrentLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Stress")
    FSageBrainTestResult TestRapidConsciousnessEvolution();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Stress")
    FSageBrainTestResult TestSystemStability();

    // 集成測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Integration")
    FSageBrainTestResult TestThinkingLearningIntegration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Integration")
    FSageBrainTestResult TestConsciousnessPhilosophyIntegration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Integration")
    FSageBrainTestResult TestFullSystemIntegration();

    // 邊界測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Boundary")
    FSageBrainTestResult TestEmptyInputHandling();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Boundary")
    FSageBrainTestResult TestLargeInputHandling();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Boundary")
    FSageBrainTestResult TestInvalidParameterHandling();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing|Boundary")
    FSageBrainTestResult TestMemoryLimits();

    // 獲取測試結果
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    TArray<FSageBrainTestResult> GetAllTestResults() const { return TestResults; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FSageBrainTestSummary GetLastTestSummary() const { return LastTestSummary; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FString GenerateTestReport() const;

    // 清理測試結果
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    void ClearTestResults();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestStarted, const FString&, TestName);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnTestCompleted, const FString&, TestName, bool, bPassed);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, const FSageBrainTestSummary&, Summary);

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Testing|Events")
    FOnTestStarted OnTestStarted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Testing|Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Testing|Events")
    FOnTestSuiteCompleted OnTestSuiteCompleted;

private:
    UPROPERTY()
    TObjectPtr<UMingSageBrainCoreSystem> SageBrainSystem;

    TArray<FSageBrainTestResult> TestResults;
    FSageBrainTestSummary LastTestSummary;

    bool bIsInitialized;

    // 內部輔助函數
    FSageBrainTestResult CreateTestResult(const FString& TestName, bool bPassed, const FString& Details, float ExecutionTime);
    void RecordTestResult(const FSageBrainTestResult& Result);
    FSageBrainTestSummary CalculateTestSummary(const TArray<FSageBrainTestResult>& Results) const;
    
    // 測試輔助函數
    bool MeasureExecutionTime(TFunction<void()> TestFunction, float& OutExecutionTime);
    FString GenerateDetailedTestReport() const;
    FString GeneratePerformanceMetrics() const;
    FString GenerateFailureAnalysis() const;
};
