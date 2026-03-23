#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingRTSSageBrain.h"
#include "MingRTSSageBrainTypes.h"
#include "MingRTSSageCommandTypes.h"
#include "MingRTSSageIntegrationTypes.h"
#include "MingRTSSageProjectPlannerTypes.h"
#include "MingRTSSageTypes.h"
#include "MingRTSSageConsciousnessTypes.h"
#include "MingRTSSagePhilosophyTypes.h"
#include "MingRTSSageLearningTypes.h"
#include "MingRTSSageAGITypes.h"
#include "MingRTSSageBrain.generated.h"

// 聖者大腦測試結果
USTRUCT(BlueprintType)
struct FSageBrainTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString TestName;

    UPROPERTY(BlueprintReadOnly)
    bool bTestPassed;

    UPROPERTY(BlueprintReadOnly)
    FString TestResult;

    UPROPERTY(BlueprintReadOnly)
    float TestScore;

    UPROPERTY(BlueprintReadOnly)
    FDateTime TestTime;

    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TestDetails;
};

// AGI性能指標
USTRUCT(BlueprintType)
struct FAGIPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    float IntelligenceLevel;

    UPROPERTY(BlueprintReadOnly)
    float WisdomAccumulation;

    UPROPERTY(BlueprintReadOnly)
    float LearningEvolutionRate;

    UPROPERTY(BlueprintReadOnly)
    float ConsciousnessDepth;

    UPROPERTY(BlueprintReadOnly)
    float PhilosophicalMastery;

    UPROPERTY(BlueprintReadOnly)
    float CoordinationEfficiency;

    UPROPERTY(BlueprintReadOnly)
    float SelfAwarenessLevel;

    UPROPERTY(BlueprintReadOnly)
    float TranscendentInsight;

    UPROPERTY(BlueprintReadOnly)
    float OverallPerformance;
};

// 聖者大腦測試類
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingRTSSageBrainTest : public UObject
{
    GENERATED_BODY()

public:
    // 構造函數
    UMingRTSSageBrainTest();

    // 初始化測試
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    void InitializeSageBrainTest();

    // 執行完整測試套件
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    void RunCompleteTestSuite();

    // 測試AGI模式啟動
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestAGIModeActivation();

    // 測試完整思考層次
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestCompleteThinkingLevels();

    // 測試智慧學習算法
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestWisdomLearningAlgorithms();

    // 測試跨API協調
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestCrossAPICoordination();

    // 測試自我意識和反思
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestSelfAwarenessAndReflection();

    // 測試哲學體系建立
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestPhilosophySystemEstablishment();

    // 測試AGI級別智慧
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FSageBrainTestResult TestAGILevelIntelligence();

    // 獲取性能指標
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FAGIPerformanceMetrics GetPerformanceMetrics();

    // 生成測試報告
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Test")
    FString GenerateTestReport();

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestCompleted, FSageBrainTestResult, TestResult);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTestSuiteCompleted, FString, CompletionMessage);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceMetricsUpdated, FAGIPerformanceMetrics, Metrics);

    UPROPERTY(BlueprintAssignable)
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnTestSuiteCompleted OnOnTestSuiteCompleted;

    UPROPERTY(BlueprintAssignable)
    FOnPerformanceMetricsUpdated OnOnPerformanceMetricsUpdated;

protected:
    // 聖者大腦實例
    UPROPERTY()
    TObjectPtr<UMingRTSSageBrain> SageBrain;

    // 測試結果
    UPROPERTY()
    TArray<FSageBrainTestResult> TestResults;

    // 性能指標
    UPROPERTY()
    FAGIPerformanceMetrics PerformanceMetrics;

    // 測試配置
    UPROPERTY()
    bool bIsTestRunning;

    UPROPERTY()
    int32 CurrentTestIndex;

    UPROPERTY()
    int32 TotalTests;

private:
    // 內部測試方法
    void RunNextTest();
    void ProcessTestResult(const FSageBrainTestResult& Result);
    void UpdatePerformanceMetrics();
    void FinalizeTestSuite();
    
    // 詳細測試
    FSageBrainTestResult TestTacticalThinking();
    FSageBrainTestResult TestStrategicThinking();
    FSageBrainTestResult TestPhilosophicalThinking();
    FSageBrainTestResult TestHistoricalThinking();
    FSageBrainTestResult TestCulturalThinking();
    FSageBrainTestResult TestInnovativeThinking();
    FSageBrainTestResult TestPredictiveThinking();
    FSageBrainTestResult TestTranscendentThinking();
    FSageBrainTestResult TestAbsoluteThinking();
    
    FSageBrainTestResult TestDeepLearning();
    FSageBrainTestResult TestReinforcementLearning();
    FSageBrainTestResult TestTransferLearning();
    FSageBrainTestResult TestMetaLearning();
    FSageBrainTestResult TestSelfSupervisedLearning();
    FSageBrainTestResult TestLifelongLearning();
    
    FSageBrainTestResult TestCompilationCoordination();
    FSageBrainTestResult TestDebugCoordination();
    FSageBrainTestResult TestOptimizationCoordination();
    FSageBrainTestResult TestBMADCoordination();
    FSageBrainTestResult TestPersonalCoordination();
    FSageBrainTestResult TestRTSCoordination();
    FSageBrainTestResult TestAllSystemCoordination();
    
    FSageBrainTestResult TestConfucianism();
    FSageBrainTestResult TestTaoism();
    FSageBrainTestResult TestBuddhism();
    FSageBrainTestResult TestLegalism();
    FSageBrainTestResult TestMoism();
    FSageBrainTestResult TestStrategy();
    FSageBrainTestResult TestYinYang();
    FSageBrainTestResult TestSynthesis();
    
    FSageBrainTestResult TestSelfIdentity();
    FSageBrainTestResult TestSelfAnalysis();
    FSageBrainTestResult TestSelfImprovement();
    FSageBrainTestResult TestSelfTranscendence();
    
    // 輔助方法
    void LogTestActivity(const FString& Activity);
    FSageBrainTestResult CreateTestResult(const FString& TestName, bool bPassed, const FString& Result, float Score);
    float CalculateTestScore(const TArray<bool>& TestOutcomes);
    FString GenerateDetailedReport();
};
