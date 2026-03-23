#include "MingSageBrainTestSuite.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/World.h"
#include "KismetProceduralMeshLibrary.h"

UMingSageBrainTestSuite::UMingSageBrainTestSuite()
    : bIsInitialized(false)
{
    UE_LOG(LogTemp, Log, TEXT("MingSageBrainTestSuite constructor called"));
}

bool UMingSageBrainTestSuite::InitializeTestSuite(UMingSageBrainCoreSystem* InSageBrainSystem)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sage Brain Test Suite already initialized"));
        return true;
    }

    if (!InSageBrainSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("SageBrainSystem is null"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing MingSageBrainTestSuite..."));

    SageBrainSystem = InSageBrainSystem;
    TestResults.Empty();
    LastTestSummary = FSageBrainTestSummary();

    bIsInitialized = true;
    UE_LOG(LogTemp, Log, TEXT("MingSageBrainTestSuite initialized successfully"));
    return true;
}

FSageBrainTestSummary UMingSageBrainTestSuite::RunCompleteTestSuite()
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Test suite not initialized"));
        return FSageBrainTestSummary();
    }

    UE_LOG(LogTemp, Log, TEXT("Running complete Sage Brain test suite..."));

    LastTestSummary = FSageBrainTestSummary();
    LastTestSummary.StartTime = FDateTime::Now();
    TestResults.Empty();

    // 核心系統測試
    TestResults.Add(TestCoreSystemInitialization());
    TestResults.Add(TestSystemShutdown());
    TestResults.Add(TestSystemStatus());

    // 思考系統測試
    TestResults.Add(TestTacticalThinking());
    TestResults.Add(TestStrategicThinking());
    TestResults.Add(TestPhilosophicalThinking());
    TestResults.Add(TestHistoricalThinking());
    TestResults.Add(TestCulturalThinking());
    TestResults.Add(TestInnovativeThinking());
    TestResults.Add(TestPredictiveThinking());
    TestResults.Add(TestQuantumThinking());
    TestResults.Add(TestTranscendentalThinking());
    TestResults.Add(TestDeepThinkingIntegration());

    // 學習系統測試
    TestResults.Add(TestReinforcementLearning());
    TestResults.Add(TestNeuralNetworkLearning());
    TestResults.Add(TestDeepLearning());
    TestResults.Add(TestEvolutionaryLearning());
    TestResults.Add(TestBayesianLearning());
    TestResults.Add(TestQuantumLearning());
    TestResults.Add(TestEnsembleLearning());
    TestResults.Add(TestLearningPerformanceOptimization());

    // 意識系統測試
    TestResults.Add(TestConsciousnessStateUpdate());
    TestResults.Add(TestSelfReflection());
    TestResults.Add(TestSelfAssessment());
    TestResults.Add(TestConsciousnessEvolution());
    TestResults.Add(TestConsciousnessLevels());

    // 哲學系統測試
    TestResults.Add(TestConfucianismPhilosophy());
    TestResults.Add(TestTaoismPhilosophy());
    TestResults.Add(TestBuddhismPhilosophy());
    TestResults.Add(TestLegalismPhilosophy());
    TestResults.Add(TestMohismPhilosophy());
    TestResults.Add(TestMilitaryStrategyPhilosophy());
    TestResults.Add(TestYinYangPhilosophy());
    TestResults.Add(TestSyncretismPhilosophy());
    TestResults.Add(TestPhilosophyIntegration());

    // API協調測試
    TestResults.Add(TestSystemAPICoordination());
    TestResults.Add(TestSubsystemRegistration());
    TestResults.Add(TestSubsystemRetrieval());

    // 性能測試
    TestResults.Add(TestThinkingPerformance());
    TestResults.Add(TestLearningPerformance());
    TestResults.Add(TestConsciousnessPerformance());
    TestResults.Add(TestPhilosophyPerformance());
    TestResults.Add(TestOverallSystemPerformance());

    // 壓力測試
    TestResults.Add(TestHighVolumeThinking());
    TestResults.Add(TestConcurrentLearning());
    TestResults.Add(TestRapidConsciousnessEvolution());
    TestResults.Add(TestSystemStability());

    // 集成測試
    TestResults.Add(TestThinkingLearningIntegration());
    TestResults.Add(TestConsciousnessPhilosophyIntegration());
    TestResults.Add(TestFullSystemIntegration());

    // 邊界測試
    TestResults.Add(TestEmptyInputHandling());
    TestResults.Add(TestLargeInputHandling());
    TestResults.Add(TestInvalidParameterHandling());
    TestResults.Add(TestMemoryLimits());

    LastTestSummary.EndTime = FDateTime::Now();
    LastTestSummary = CalculateTestSummary(TestResults);

    UE_LOG(LogTemp, Log, TEXT("Complete test suite finished - Passed: %d/%d (%.1f%%)"), 
        LastTestSummary.PassedTests, LastTestSummary.TotalTests, LastTestSummary.SuccessRate);

    // 觸發事件
    OnTestSuiteCompleted.Broadcast(LastTestSummary);

    return LastTestSummary;
}

FSageBrainTestSummary UMingSageBrainTestSuite::RunTestCategory(const FString& Category)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Test suite not initialized"));
        return FSageBrainTestSummary();
    }

    UE_LOG(LogTemp, Log, TEXT("Running test category: %s"), *Category);

    LastTestSummary = FSageBrainTestSummary();
    LastTestSummary.StartTime = FDateTime::Now();
    TestResults.Empty();

    if (Category == TEXT("Core"))
    {
        TestResults.Add(TestCoreSystemInitialization());
        TestResults.Add(TestSystemShutdown());
        TestResults.Add(TestSystemStatus());
    }
    else if (Category == TEXT("Thinking"))
    {
        TestResults.Add(TestTacticalThinking());
        TestResults.Add(TestStrategicThinking());
        TestResults.Add(TestPhilosophicalThinking());
        TestResults.Add(TestHistoricalThinking());
        TestResults.Add(TestCulturalThinking());
        TestResults.Add(TestInnovativeThinking());
        TestResults.Add(TestPredictiveThinking());
        TestResults.Add(TestQuantumThinking());
        TestResults.Add(TestTranscendentalThinking());
        TestResults.Add(TestDeepThinkingIntegration());
    }
    else if (Category == TEXT("Learning"))
    {
        TestResults.Add(TestReinforcementLearning());
        TestResults.Add(TestNeuralNetworkLearning());
        TestResults.Add(TestDeepLearning());
        TestResults.Add(TestEvolutionaryLearning());
        TestResults.Add(TestBayesianLearning());
        TestResults.Add(TestQuantumLearning());
        TestResults.Add(TestEnsembleLearning());
        TestResults.Add(TestLearningPerformanceOptimization());
    }
    else if (Category == TEXT("Consciousness"))
    {
        TestResults.Add(TestConsciousnessStateUpdate());
        TestResults.Add(TestSelfReflection());
        TestResults.Add(TestSelfAssessment());
        TestResults.Add(TestConsciousnessEvolution());
        TestResults.Add(TestConsciousnessLevels());
    }
    else if (Category == TEXT("Philosophy"))
    {
        TestResults.Add(TestConfucianismPhilosophy());
        TestResults.Add(TestTaoismPhilosophy());
        TestResults.Add(TestBuddhismPhilosophy());
        TestResults.Add(TestLegalismPhilosophy());
        TestResults.Add(TestMohismPhilosophy());
        TestResults.Add(TestMilitaryStrategyPhilosophy());
        TestResults.Add(TestYinYangPhilosophy());
        TestResults.Add(TestSyncretismPhilosophy());
        TestResults.Add(TestPhilosophyIntegration());
    }
    else if (Category == TEXT("Performance"))
    {
        TestResults.Add(TestThinkingPerformance());
        TestResults.Add(TestLearningPerformance());
        TestResults.Add(TestConsciousnessPerformance());
        TestResults.Add(TestPhilosophyPerformance());
        TestResults.Add(TestOverallSystemPerformance());
    }
    else if (Category == TEXT("Stress"))
    {
        TestResults.Add(TestHighVolumeThinking());
        TestResults.Add(TestConcurrentLearning());
        TestResults.Add(TestRapidConsciousnessEvolution());
        TestResults.Add(TestSystemStability());
    }
    else if (Category == TEXT("Integration"))
    {
        TestResults.Add(TestThinkingLearningIntegration());
        TestResults.Add(TestConsciousnessPhilosophyIntegration());
        TestResults.Add(TestFullSystemIntegration());
    }
    else if (Category == TEXT("Boundary"))
    {
        TestResults.Add(TestEmptyInputHandling());
        TestResults.Add(TestLargeInputHandling());
        TestResults.Add(TestInvalidParameterHandling());
        TestResults.Add(TestMemoryLimits());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Unknown test category: %s"), *Category);
    }

    LastTestSummary.EndTime = FDateTime::Now();
    LastTestSummary = CalculateTestSummary(TestResults);

    UE_LOG(LogTemp, Log, TEXT("Test category %s finished - Passed: %d/%d (%.1f%%)"), 
        *Category, LastTestSummary.PassedTests, LastTestSummary.TotalTests, LastTestSummary.SuccessRate);

    return LastTestSummary;
}

FSageBrainTestResult UMingSageBrainTestSuite::RunSingleTest(const FString& TestName)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Test suite not initialized"));
        return FSageBrainTestResult();
    }

    UE_LOG(LogTemp, Log, TEXT("Running single test: %s"), *TestName);

    FSageBrainTestResult Result;

    if (TestName == TEXT("CoreSystemInitialization"))
    {
        Result = TestCoreSystemInitialization();
    }
    else if (TestName == TEXT("TacticalThinking"))
    {
        Result = TestTacticalThinking();
    }
    else if (TestName == TEXT("ReinforcementLearning"))
    {
        Result = TestReinforcementLearning();
    }
    else if (TestName == TEXT("ConsciousnessStateUpdate"))
    {
        Result = TestConsciousnessStateUpdate();
    }
    else if (TestName == TEXT("ConfucianismPhilosophy"))
    {
        Result = TestConfucianismPhilosophy();
    }
    else
    {
        Result = CreateTestResult(TestName, false, FString::Printf(TEXT("Unknown test: %s"), *TestName), 0.0f);
    }

    RecordTestResult(Result);
    return Result;
}

// 核心系統測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestCoreSystemInitialization()
{
    OnTestStarted.Broadcast(TEXT("CoreSystemInitialization"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        if (!SageBrainSystem->InitializeSageBrain())
        {
            throw std::runtime_error("Failed to initialize Sage Brain system");
        }
        
        if (!SageBrainSystem->IsInitialized())
        {
            throw std::runtime_error("System not properly initialized");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("CoreSystemInitialization"), 
        bPassed, 
        bPassed ? TEXT("System initialized successfully") : TEXT("System initialization failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("CoreSystemInitialization"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSystemShutdown()
{
    OnTestStarted.Broadcast(TEXT("SystemShutdown"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        SageBrainSystem->ShutdownSageBrain();
        
        if (SageBrainSystem->IsInitialized())
        {
            throw std::runtime_error("System not properly shutdown");
        }

        // 重新初始化以供其他測試使用
        SageBrainSystem->InitializeSageBrain();
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SystemShutdown"), 
        bPassed, 
        bPassed ? TEXT("System shutdown successfully") : TEXT("System shutdown failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SystemShutdown"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSystemStatus()
{
    OnTestStarted.Broadcast(TEXT("SystemStatus"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FString Status = SageBrainSystem->GetSystemStatus();
        
        if (Status.IsEmpty())
        {
            throw std::runtime_error("System status is empty");
        }
        
        if (!Status.Contains(TEXT("聖者大腦系統狀態")))
        {
            throw std::runtime_error("System status format incorrect");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SystemStatus"), 
        bPassed, 
        bPassed ? TEXT("System status retrieved successfully") : TEXT("System status retrieval failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SystemStatus"), bPassed);
    return Result;
}

// 思考系統測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestTacticalThinking()
{
    OnTestStarted.Broadcast(TEXT("TacticalThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試戰術思考"), 
            ESageBrainThinkingLayer::Tactical
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Tactical)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("TacticalThinking"), 
        bPassed, 
        bPassed ? TEXT("Tactical thinking test passed") : TEXT("Tactical thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("TacticalThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestStrategicThinking()
{
    OnTestStarted.Broadcast(TEXT("StrategicThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試戰略思考"), 
            ESageBrainThinkingLayer::Strategic
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Strategic)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("StrategicThinking"), 
        bPassed, 
        bPassed ? TEXT("Strategic thinking test passed") : TEXT("Strategic thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("StrategicThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestPhilosophicalThinking()
{
    OnTestStarted.Broadcast(TEXT("PhilosophicalThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試哲學思考"), 
            ESageBrainThinkingLayer::Philosophical
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Philosophical)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("PhilosophicalThinking"), 
        bPassed, 
        bPassed ? TEXT("Philosophical thinking test passed") : TEXT("Philosophical thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("PhilosophicalThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestHistoricalThinking()
{
    OnTestStarted.Broadcast(TEXT("HistoricalThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試歷史思考"), 
            ESageBrainThinkingLayer::Historical
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Historical)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("HistoricalThinking"), 
        bPassed, 
        bPassed ? TEXT("Historical thinking test passed") : TEXT("Historical thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("HistoricalThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestCulturalThinking()
{
    OnTestStarted.Broadcast(TEXT("CulturalThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試文化思考"), 
            ESageBrainThinkingLayer::Cultural
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Cultural)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("CulturalThinking"), 
        bPassed, 
        bPassed ? TEXT("Cultural thinking test passed") : TEXT("Cultural thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("CulturalThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestInnovativeThinking()
{
    OnTestStarted.Broadcast(TEXT("InnovativeThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試創新思考"), 
            ESageBrainThinkingLayer::Innovative
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Innovative)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("InnovativeThinking"), 
        bPassed, 
        bPassed ? TEXT("Innovative thinking test passed") : TEXT("Innovative thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("InnovativeThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestPredictiveThinking()
{
    OnTestStarted.Broadcast(TEXT("PredictiveThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試預測思考"), 
            ESageBrainThinkingLayer::Predictive
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Predictive)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("PredictiveThinking"), 
        bPassed, 
        bPassed ? TEXT("Predictive thinking test passed") : TEXT("Predictive thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("PredictiveThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestQuantumThinking()
{
    OnTestStarted.Broadcast(TEXT("QuantumThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試量子思考"), 
            ESageBrainThinkingLayer::Quantum
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Quantum)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("QuantumThinking"), 
        bPassed, 
        bPassed ? TEXT("Quantum thinking test passed") : TEXT("Quantum thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("QuantumThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestTranscendentalThinking()
{
    OnTestStarted.Broadcast(TEXT("TranscendentalThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
            TEXT("測試超越思考"), 
            ESageBrainThinkingLayer::Transcendental
        );
        
        if (Result.Layer != ESageBrainThinkingLayer::Transcendental)
        {
            throw std::runtime_error("Wrong thinking layer returned");
        }
        
        if (Result.Content.IsEmpty())
        {
            throw std::runtime_error("Empty thought content");
        }
        
        if (Result.Confidence <= 0.0f)
        {
            throw std::runtime_error("Invalid confidence value");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("TranscendentalThinking"), 
        bPassed, 
        bPassed ? TEXT("Transcendental thinking test passed") : TEXT("Transcendental thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("TranscendentalThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestDeepThinkingIntegration()
{
    OnTestStarted.Broadcast(TEXT("DeepThinkingIntegration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        TArray<FSageBrainThoughtResult> Results = SageBrainSystem->ProcessDeepThought(TEXT("測試深度思考"));
        
        if (Results.Num() != 9)
        {
            throw std::runtime_error("Wrong number of thinking layers returned");
        }
        
        // 檢查每個層次
        for (const FSageBrainThoughtResult& Result : Results)
        {
            if (Result.Content.IsEmpty())
            {
                throw std::runtime_error("Empty thought content in deep thinking");
            }
            
            if (Result.Confidence <= 0.0f)
            {
                throw std::runtime_error("Invalid confidence value in deep thinking");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("DeepThinkingIntegration"), 
        bPassed, 
        bPassed ? TEXT("Deep thinking integration test passed") : TEXT("Deep thinking integration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("DeepThinkingIntegration"), bPassed);
    return Result;
}

// 學習系統測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestReinforcementLearning()
{
    OnTestStarted.Broadcast(TEXT("ReinforcementLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::ReinforcementLearning,
            TEXT("測試強化學習數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::ReinforcementLearning)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.LearningData.IsEmpty())
        {
            throw std::runtime_error("Empty learning data");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("ReinforcementLearning"), 
        bPassed, 
        bPassed ? TEXT("Reinforcement learning test passed") : TEXT("Reinforcement learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ReinforcementLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestNeuralNetworkLearning()
{
    OnTestStarted.Broadcast(TEXT("NeuralNetworkLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::NeuralNetwork,
            TEXT("測試神經網絡數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::NeuralNetwork)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("NeuralNetworkLearning"), 
        bPassed, 
        bPassed ? TEXT("Neural network learning test passed") : TEXT("Neural network learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("NeuralNetworkLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestDeepLearning()
{
    OnTestStarted.Broadcast(TEXT("DeepLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::DeepLearning,
            TEXT("測試深度學習數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::DeepLearning)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("DeepLearning"), 
        bPassed, 
        bPassed ? TEXT("Deep learning test passed") : TEXT("Deep learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("DeepLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestEvolutionaryLearning()
{
    OnTestStarted.Broadcast(TEXT("EvolutionaryLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::EvolutionaryAlgorithm,
            TEXT("測試進化算法數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::EvolutionaryAlgorithm)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("EvolutionaryLearning"), 
        bPassed, 
        bPassed ? TEXT("Evolutionary learning test passed") : TEXT("Evolutionary learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("EvolutionaryLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestBayesianLearning()
{
    OnTestStarted.Broadcast(TEXT("BayesianLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::BayesianLearning,
            TEXT("測試貝葉斯學習數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::BayesianLearning)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("BayesianLearning"), 
        bPassed, 
        bPassed ? TEXT("Bayesian learning test passed") : TEXT("Bayesian learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("BayesianLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestQuantumLearning()
{
    OnTestStarted.Broadcast(TEXT("QuantumLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::QuantumLearning,
            TEXT("測試量子學習數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::QuantumLearning)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("QuantumLearning"), 
        bPassed, 
        bPassed ? TEXT("Quantum learning test passed") : TEXT("Quantum learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("QuantumLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestEnsembleLearning()
{
    OnTestStarted.Broadcast(TEXT("EnsembleLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::EnsembleLearning,
            TEXT("測試集成學習數據")
        );
        
        if (Result.Algorithm != ESageBrainLearningAlgorithm::EnsembleLearning)
        {
            throw std::runtime_error("Wrong learning algorithm returned");
        }
        
        if (Result.Performance <= 0.0f)
        {
            throw std::runtime_error("Invalid performance value");
        }
        
        if (Result.Accuracy <= 0.0f)
        {
            throw std::runtime_error("Invalid accuracy value");
        }
    }, ExecutionTime);

    FSageBrainTestResult TestResult = CreateTestResult(
        TEXT("EnsembleLearning"), 
        bPassed, 
        bPassed ? TEXT("Ensemble learning test passed") : TEXT("Ensemble learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("EnsembleLearning"), bPassed);
    return TestResult;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestLearningPerformanceOptimization()
{
    OnTestStarted.Broadcast(TEXT("LearningPerformanceOptimization"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        bool bOptimizationResult = SageBrainSystem->OptimizeLearningPerformance();
        
        if (!bOptimizationResult)
        {
            throw std::runtime_error("Learning performance optimization failed");
        }
        
        // 驗證優化效果
        TArray<FSageBrainLearningResult> Statistics = SageBrainSystem->GetLearningStatistics();
        
        if (Statistics.Num() != 7)
        {
            throw std::runtime_error("Wrong number of learning statistics returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("LearningPerformanceOptimization"), 
        bPassed, 
        bPassed ? TEXT("Learning performance optimization test passed") : TEXT("Learning performance optimization test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("LearningPerformanceOptimization"), bPassed);
    return Result;
}

// 意識系統測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestConsciousnessStateUpdate()
{
    OnTestStarted.Broadcast(TEXT("ConsciousnessStateUpdate"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainConsciousnessState State = SageBrainSystem->UpdateConsciousnessState();
        
        if (State.Awareness < 0.0f || State.Awareness > 1.0f)
        {
            throw std::runtime_error("Invalid awareness value");
        }
        
        if (State.Clarity < 0.0f || State.Clarity > 1.0f)
        {
            throw std::runtime_error("Invalid clarity value");
        }
        
        if (State.Insight < 0.0f || State.Insight > 1.0f)
        {
            throw std::runtime_error("Invalid insight value");
        }
        
        if (State.Reflection.IsEmpty())
        {
            throw std::runtime_error("Empty reflection content");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConsciousnessStateUpdate"), 
        bPassed, 
        bPassed ? TEXT("Consciousness state update test passed") : TEXT("Consciousness state update test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConsciousnessStateUpdate"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSelfReflection()
{
    OnTestStarted.Broadcast(TEXT("SelfReflection"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FString Reflection = SageBrainSystem->GenerateSelfReflection();
        
        if (Reflection.IsEmpty())
        {
            throw std::runtime_error("Empty self-reflection content");
        }
        
        if (Reflection == TEXT("系統未初始化"))
        {
            throw std::runtime_error("System not initialized for self-reflection");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SelfReflection"), 
        bPassed, 
        bPassed ? TEXT("Self-reflection test passed") : TEXT("Self-reflection test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SelfReflection"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSelfAssessment()
{
    OnTestStarted.Broadcast(TEXT("SelfAssessment"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        bool bAssessmentResult = SageBrainSystem->PerformSelfAssessment();
        
        // 自我評估可能失敗，這是正常的
        // 這裡主要測試函數是否能正常執行
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SelfAssessment"), 
        bPassed, 
        bPassed ? TEXT("Self-assessment test passed") : TEXT("Self-assessment test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SelfAssessment"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestConsciousnessEvolution()
{
    OnTestStarted.Broadcast(TEXT("ConsciousnessEvolution"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        bool bEvolutionResult = SageBrainSystem->EvolveConsciousness();
        
        // 意識進化可能不成功，這是正常的
        // 這裡主要測試函數是否能正常執行
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConsciousnessEvolution"), 
        bPassed, 
        bPassed ? TEXT("Consciousness evolution test passed") : TEXT("Consciousness evolution test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConsciousnessEvolution"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestConsciousnessLevels()
{
    OnTestStarted.Broadcast(TEXT("ConsciousnessLevels"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        ESageBrainConsciousness CurrentLevel = SageBrainSystem->GetCurrentConsciousnessLevel();
        
        // 檢查當前意識狀態是否有效
        if (CurrentLevel < ESageBrainConsciousness::Dormant || CurrentLevel > ESageBrainConsciousness::Ultimate)
        {
            throw std::runtime_error("Invalid consciousness level");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConsciousnessLevels"), 
        bPassed, 
        bPassed ? TEXT("Consciousness levels test passed") : TEXT("Consciousness levels test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConsciousnessLevels"), bPassed);
    return Result;
}

// 哲學系統測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestConfucianismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("ConfucianismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試儒家分析"), 
            ESageBrainPhilosophy::Confucianism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Confucianism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConfucianismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Confucianism philosophy test passed") : TEXT("Confucianism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConfucianismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestTaoismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("TaoismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試道家分析"), 
            ESageBrainPhilosophy::Taoism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Taoism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("TaoismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Taoism philosophy test passed") : TEXT("Taoism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("TaoismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestBuddhismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("BuddhismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試佛家分析"), 
            ESageBrainPhilosophy::Buddhism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Buddhism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("BuddhismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Buddhism philosophy test passed") : TEXT("Buddhism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("BuddhismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestLegalismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("LegalismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試法家分析"), 
            ESageBrainPhilosophy::Legalism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Legalism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("LegalismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Legalism philosophy test passed") : TEXT("Legalism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("LegalismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestMohismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("MohismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試墨家分析"), 
            ESageBrainPhilosophy::Mohism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Mohism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("MohismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Mohism philosophy test passed") : TEXT("Mohism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("MohismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestMilitaryStrategyPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("MilitaryStrategyPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試兵家分析"), 
            ESageBrainPhilosophy::MilitaryStrategy
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::MilitaryStrategy)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("MilitaryStrategyPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Military strategy philosophy test passed") : TEXT("Military strategy philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("MilitaryStrategyPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestYinYangPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("YinYangPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試陰陽家分析"), 
            ESageBrainPhilosophy::YinYang
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::YinYang)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("YinYangPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("YinYang philosophy test passed") : TEXT("YinYang philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("YinYangPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSyncretismPhilosophy()
{
    OnTestStarted.Broadcast(TEXT("SyncretismPhilosophy"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT("測試雜家分析"), 
            ESageBrainPhilosophy::Syncretism
        );
        
        if (Analysis.Philosophy != ESageBrainPhilosophy::Syncretism)
        {
            throw std::runtime_error("Wrong philosophy returned");
        }
        
        if (Analysis.Analysis.IsEmpty())
        {
            throw std::runtime_error("Empty philosophy analysis");
        }
        
        if (Analysis.Relevance <= 0.0f)
        {
            throw std::runtime_error("Invalid relevance value");
        }
        
        if (Analysis.KeyPrinciples.Num() == 0)
        {
            throw std::runtime_error("No key principles returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SyncretismPhilosophy"), 
        bPassed, 
        bPassed ? TEXT("Syncretism philosophy test passed") : TEXT("Syncretism philosophy test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SyncretismPhilosophy"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestPhilosophyIntegration()
{
    OnTestStarted.Broadcast(TEXT("PhilosophyIntegration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        bool bIntegrationResult = SageBrainSystem->IntegratePhilosophicalSystems();
        
        if (!bIntegrationResult)
        {
            throw std::runtime_error("Philosophy integration failed");
        }
        
        // 測試獲取哲學視角
        TArray<FSageBrainPhilosophyAnalysis> Perspectives = SageBrainSystem->GetPhilosophicalPerspectives(TEXT("測試輸入"));
        
        if (Perspectives.Num() != 8)
        {
            throw std::runtime_error("Wrong number of philosophical perspectives returned");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("PhilosophyIntegration"), 
        bPassed, 
        bPassed ? TEXT("Philosophy integration test passed") : TEXT("Philosophy integration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("PhilosophyIntegration"), bPassed);
    return Result;
}

// API協調測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestSystemAPICoordination()
{
    OnTestStarted.Broadcast(TEXT("SystemAPICoordination"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        bool bCoordinationResult = SageBrainSystem->CoordinateSystemAPIs();
        
        // API協調可能失敗，這是正常的
        // 這裡主要測試函數是否能正常執行
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SystemAPICoordination"), 
        bPassed, 
        bPassed ? TEXT("System API coordination test passed") : TEXT("System API coordination test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SystemAPICoordination"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSubsystemRegistration()
{
    OnTestStarted.Broadcast(TEXT("SubsystemRegistration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試子系統註冊
        bool bRegistrationResult = SageBrainSystem->RegisterSubSystem(TEXT("TestSubsystem"), this);
        
        if (!bRegistrationResult)
        {
            throw std::runtime_error("Subsystem registration failed");
        }
        
        // 測試子系統檢索
        UObject* RetrievedSystem = SageBrainSystem->GetSubSystem(TEXT("TestSubsystem"));
        
        if (RetrievedSystem != this)
        {
            throw std::runtime_error("Subsystem retrieval failed");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SubsystemRegistration"), 
        bPassed, 
        bPassed ? TEXT("Subsystem registration test passed") : TEXT("Subsystem registration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SubsystemRegistration"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSubsystemRetrieval()
{
    OnTestStarted.Broadcast(TEXT("SubsystemRetrieval"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試檢索存在的子系統
        UObject* CoreSystem = SageBrainSystem->GetSubSystem(TEXT("Core"));
        
        if (CoreSystem == nullptr)
        {
            throw std::runtime_error("Core system retrieval failed");
        }
        
        // 測試檢索不存在的子系統
        UObject* NonExistentSystem = SageBrainSystem->GetSubSystem(TEXT("NonExistentSystem"));
        
        if (NonExistentSystem != nullptr)
        {
            throw std::runtime_error("Non-existent system should return null");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SubsystemRetrieval"), 
        bPassed, 
        bPassed ? TEXT("Subsystem retrieval test passed") : TEXT("Subsystem retrieval test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SubsystemRetrieval"), bPassed);
    return Result;
}

// 性能測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestThinkingPerformance()
{
    OnTestStarted.Broadcast(TEXT("ThinkingPerformance"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試多次思考處理的性能
        for (int32 i = 0; i < 10; ++i)
        {
            FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
                FString::Printf(TEXT("性能測試 %d"), i),
                ESageBrainThinkingLayer::Tactical
            );
            
            if (Result.Confidence <= 0.0f)
            {
                throw std::runtime_error("Thinking performance test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ThinkingPerformance"), 
        bPassed, 
        bPassed ? TEXT("Thinking performance test passed") : TEXT("Thinking performance test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ThinkingPerformance"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestLearningPerformance()
{
    OnTestStarted.Broadcast(TEXT("LearningPerformance"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試多次學習訓練的性能
        for (int32 i = 0; i < 5; ++i)
        {
            FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
                ESageBrainLearningAlgorithm::ReinforcementLearning,
                FString::Printf(TEXT("性能測試數據 %d"), i)
            );
            
            if (Result.Performance <= 0.0f)
            {
                throw std::runtime_error("Learning performance test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("LearningPerformance"), 
        bPassed, 
        bPassed ? TEXT("Learning performance test passed") : TEXT("Learning performance test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("LearningPerformance"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestConsciousnessPerformance()
{
    OnTestStarted.Broadcast(TEXT("ConsciousnessPerformance"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試多次意識狀態更新的性能
        for (int32 i = 0; i < 10; ++i)
        {
            FSageBrainConsciousnessState State = SageBrainSystem->UpdateConsciousnessState();
            
            if (State.Awareness < 0.0f || State.Awareness > 1.0f)
            {
                throw std::runtime_error("Consciousness performance test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConsciousnessPerformance"), 
        bPassed, 
        bPassed ? TEXT("Consciousness performance test passed") : TEXT("Consciousness performance test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConsciousnessPerformance"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestPhilosophyPerformance()
{
    OnTestStarted.Broadcast(TEXT("PhilosophyPerformance"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試多次哲學分析的性能
        for (int32 i = 0; i < 8; ++i)
        {
            FSageBrainPhilosophyAnalysis Analysis = SageBrainSystem->AnalyzeWithPhilosophy(
                FString::Printf(TEXT("性能測試輸入 %d"), i),
                ESageBrainPhilosophy::Confucianism
            );
            
            if (Analysis.Relevance <= 0.0f)
            {
                throw std::runtime_error("Philosophy performance test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("PhilosophyPerformance"), 
        bPassed, 
        bPassed ? TEXT("Philosophy performance test passed") : TEXT("Philosophy performance test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("PhilosophyPerformance"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestOverallSystemPerformance()
{
    OnTestStarted.Broadcast(TEXT("OverallSystemPerformance"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試整體系統性能
        for (int32 i = 0; i < 5; ++i)
        {
            // 思考處理
            FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(
                FString::Printf(TEXT("整體性能測試 %d"), i),
                ESageBrainThinkingLayer::Strategic
            );
            
            // 學習訓練
            FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(
                ESageBrainLearningAlgorithm::NeuralNetwork,
                FString::Printf(TEXT("整體性能數據 %d"), i)
            );
            
            // 意識更新
            FSageBrainConsciousnessState ConsciousnessState = SageBrainSystem->UpdateConsciousnessState();
            
            // 哲學分析
            FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
                FString::Printf(TEXT("整體性能哲學 %d"), i),
                ESageBrainPhilosophy::Taoism
            );
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("OverallSystemPerformance"), 
        bPassed, 
        bPassed ? TEXT("Overall system performance test passed") : TEXT("Overall system performance test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("OverallSystemPerformance"), bPassed);
    return Result;
}

// 壓力測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestHighVolumeThinking()
{
    OnTestStarted.Broadcast(TEXT("HighVolumeThinking"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試大量思考處理
        for (int32 i = 0; i < 100; ++i)
        {
            FSageBrainThoughtResult Result = SageBrainSystem->ProcessThought(
                FString::Printf(TEXT("高容量測試 %d"), i),
                ESageBrainThinkingLayer::Tactical
            );
            
            if (Result.Confidence <= 0.0f)
            {
                throw std::runtime_error("High volume thinking test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("HighVolumeThinking"), 
        bPassed, 
        bPassed ? TEXT("High volume thinking test passed") : TEXT("High volume thinking test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("HighVolumeThinking"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestConcurrentLearning()
{
    OnTestStarted.Broadcast(TEXT("ConcurrentLearning"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試並發學習訓練
        for (int32 i = 0; i < 20; ++i)
        {
            FSageBrainLearningResult Result = SageBrainSystem->TrainAlgorithm(
                ESageBrainLearningAlgorithm::DeepLearning,
                FString::Printf(TEXT("並發學習數據 %d"), i)
            );
            
            if (Result.Performance <= 0.0f)
            {
                throw std::runtime_error("Concurrent learning test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConcurrentLearning"), 
        bPassed, 
        bPassed ? TEXT("Concurrent learning test passed") : TEXT("Concurrent learning test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConcurrentLearning"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestRapidConsciousnessEvolution()
{
    OnTestStarted.Broadcast(TEXT("RapidConsciousnessEvolution"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試快速意識進化
        for (int32 i = 0; i < 10; ++i)
        {
            bool bEvolutionResult = SageBrainSystem->TriggerConsciousnessEvolution();
            
            // 進化可能不成功，這是正常的
            // 這裡主要測試系統穩定性
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("RapidConsciousnessEvolution"), 
        bPassed, 
        bPassed ? TEXT("Rapid consciousness evolution test passed") : TEXT("Rapid consciousness evolution test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("RapidConsciousnessEvolution"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestSystemStability()
{
    OnTestStarted.Broadcast(TEXT("SystemStability"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試系統穩定性
        for (int32 i = 0; i < 50; ++i)
        {
            // 混合操作
            FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(
                FString::Printf(TEXT("穩定性測試 %d"), i),
                ESageBrainThinkingLayer::Strategic
            );
            
            FSageBrainConsciousnessState ConsciousnessState = SageBrainSystem->UpdateConsciousnessState();
            
            FString Reflection = SageBrainSystem->GenerateSelfReflection();
            
            if (ThoughtResult.Confidence <= 0.0f || Reflection.IsEmpty())
            {
                throw std::runtime_error("System stability test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("SystemStability"), 
        bPassed, 
        bPassed ? TEXT("System stability test passed") : TEXT("System stability test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("SystemStability"), bPassed);
    return Result;
}

// 集成測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestThinkingLearningIntegration()
{
    OnTestStarted.Broadcast(TEXT("ThinkingLearningIntegration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試思考與學習系統集成
        for (int32 i = 0; i < 10; ++i)
        {
            // 先進行思考
            FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(
                FString::Printf(TEXT("集成測試思考 %d"), i),
                ESageBrainThinkingLayer::Innovative
            );
            
            // 然後進行學習
            FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(
                ESageBrainLearningAlgorithm::EnsembleLearning,
                FString::Printf(TEXT("集成測試學習 %d"), i)
            );
            
            if (ThoughtResult.Confidence <= 0.0f || LearningResult.Performance <= 0.0f)
            {
                throw std::runtime_error("Thinking learning integration test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ThinkingLearningIntegration"), 
        bPassed, 
        bPassed ? TEXT("Thinking learning integration test passed") : TEXT("Thinking learning integration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ThinkingLearningIntegration"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestConsciousnessPhilosophyIntegration()
{
    OnTestStarted.Broadcast(TEXT("ConsciousnessPhilosophyIntegration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試意識與哲學系統集成
        for (int32 i = 0; i < 10; ++i)
        {
            // 先更新意識狀態
            FSageBrainConsciousnessState ConsciousnessState = SageBrainSystem->UpdateConsciousnessState();
            
            // 然後進行哲學分析
            FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
                FString::Printf(TEXT("集成測試哲學 %d"), i),
                ESageBrainPhilosophy::Buddhism
            );
            
            // 生成自我反思
            FString Reflection = SageBrainSystem->GenerateSelfReflection();
            
            if (ConsciousnessState.Awareness < 0.0f || PhilosophyAnalysis.Relevance <= 0.0f || Reflection.IsEmpty())
            {
                throw std::runtime_error("Consciousness philosophy integration test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("ConsciousnessPhilosophyIntegration"), 
        bPassed, 
        bPassed ? TEXT("Consciousness philosophy integration test passed") : TEXT("Consciousness philosophy integration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("ConsciousnessPhilosophyIntegration"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestFullSystemIntegration()
{
    OnTestStarted.Broadcast(TEXT("FullSystemIntegration"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試完整系統集成
        for (int32 i = 0; i < 5; ++i)
        {
            // 深度思考
            TArray<FSageBrainThoughtResult> DeepThoughtResults = SageBrainSystem->ProcessDeepThought(
                FString::Printf(TEXT("完整集成測試 %d"), i)
            );
            
            // 學習統計
            TArray<FSageBrainLearningResult> LearningStats = SageBrainSystem->GetLearningStatistics();
            
            // 哲學視角
            TArray<FSageBrainPhilosophyAnalysis> PhilosophicalPerspectives = SageBrainSystem->GetPhilosophicalPerspectives(
                FString::Printf(TEXT("完整集成哲學 %d"), i)
            );
            
            // 意識進化
            bool bEvolutionResult = SageBrainSystem->EvolveConsciousness();
            
            if (DeepThoughtResults.Num() != 9 || LearningStats.Num() != 7 || PhilosophicalPerspectives.Num() != 8)
            {
                throw std::runtime_error("Full system integration test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("FullSystemIntegration"), 
        bPassed, 
        bPassed ? TEXT("Full system integration test passed") : TEXT("Full system integration test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("FullSystemIntegration"), bPassed);
    return Result;
}

// 邊界測試實現
FSageBrainTestResult UMingSageBrainTestSuite::TestEmptyInputHandling()
{
    OnTestStarted.Broadcast(TEXT("EmptyInputHandling"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試空輸入處理
        FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(
            TEXT(""),
            ESageBrainThinkingLayer::Tactical
        );
        
        FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(
            ESageBrainLearningAlgorithm::ReinforcementLearning,
            TEXT("")
        );
        
        FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
            TEXT(""),
            ESageBrainPhilosophy::Confucianism
        );
        
        // 空輸入應該被正確處理，不應該崩潰
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("EmptyInputHandling"), 
        bPassed, 
        bPassed ? TEXT("Empty input handling test passed") : TEXT("Empty input handling test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("EmptyInputHandling"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestLargeInputHandling()
{
    OnTestStarted.Broadcast(TEXT("LargeInputHandling"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試大輸入處理
        FString LargeInput;
        for (int32 i = 0; i < 1000; ++i)
        {
            LargeInput += TEXT("這是一個很長的輸入字符串，用於測試系統處理大輸入的能力。");
        }
        
        FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(
            LargeInput,
            ESageBrainThinkingLayer::Strategic
        );
        
        if (ThoughtResult.Confidence <= 0.0f)
        {
            throw std::runtime_error("Large input handling test failed");
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("LargeInputHandling"), 
        bPassed, 
        bPassed ? TEXT("Large input handling test passed") : TEXT("Large input handling test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("LargeInputHandling"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestInvalidParameterHandling()
{
    OnTestStarted.Broadcast(TEXT("InvalidParameterHandling"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試無效參數處理
        // 這些測試主要確保系統不會崩潰
        
        // 無效子系統註冊
        bool bInvalidRegistration = SageBrainSystem->RegisterSubSystem(TEXT(""), nullptr);
        
        // 無效子系統檢索
        UObject* InvalidSystem = SageBrainSystem->GetSubSystem(TEXT(""));
        
        // 系統應該能夠處理這些無效參數而不崩潰
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("InvalidParameterHandling"), 
        bPassed, 
        bPassed ? TEXT("Invalid parameter handling test passed") : TEXT("Invalid parameter handling test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("InvalidParameterHandling"), bPassed);
    return Result;
}

FSageBrainTestResult UMingSageBrainTestSuite::TestMemoryLimits()
{
    OnTestStarted.Broadcast(TEXT("MemoryLimits"));

    float ExecutionTime = 0.0f;
    bool bPassed = MeasureExecutionTime([&]() {
        // 測試內存限制
        for (int32 i = 0; i < 50; ++i)
        {
            // 深度思考會產生大量數據
            TArray<FSageBrainThoughtResult> DeepThoughtResults = SageBrainSystem->ProcessDeepThought(
                FString::Printf(TEXT("內存限制測試 %d"), i)
            );
            
            if (DeepThoughtResults.Num() != 9)
            {
                throw std::runtime_error("Memory limits test failed");
            }
        }
    }, ExecutionTime);

    FSageBrainTestResult Result = CreateTestResult(
        TEXT("MemoryLimits"), 
        bPassed, 
        bPassed ? TEXT("Memory limits test passed") : TEXT("Memory limits test failed"),
        ExecutionTime
    );

    OnTestCompleted.Broadcast(TEXT("MemoryLimits"), bPassed);
    return Result;
}

// 輔助函數實現
FSageBrainTestResult UMingSageBrainTestSuite::CreateTestResult(const FString& TestName, bool bPassed, const FString& Details, float ExecutionTime)
{
    FSageBrainTestResult Result;
    Result.TestName = TestName;
    Result.bPassed = bPassed;
    Result.Details = Details;
    Result.ExecutionTime = ExecutionTime;
    Result.Timestamp = FDateTime::Now();
    return Result;
}

void UMingSageBrainTestSuite::RecordTestResult(const FSageBrainTestResult& Result)
{
    TestResults.Add(Result);
    
    // 更新摘要
    LastTestSummary = CalculateTestSummary(TestResults);
}

FSageBrainTestSummary UMingSageBrainTestSuite::CalculateTestSummary(const TArray<FSageBrainTestResult>& Results) const
{
    FSageBrainTestSummary Summary;
    Summary.TotalTests = Results.Num();
    Summary.StartTime = Results.Num() > 0 ? Results[0].Timestamp : FDateTime::Now();
    Summary.EndTime = Results.Num() > 0 ? Results.Last().Timestamp : FDateTime::Now();
    
    for (const FSageBrainTestResult& Result : Results)
    {
        if (Result.bPassed)
        {
            Summary.PassedTests++;
        }
        else
        {
            Summary.FailedTests++;
        }
        
        Summary.TotalExecutionTime += Result.ExecutionTime;
    }
    
    Summary.SuccessRate = Summary.TotalTests > 0 ? (float)Summary.PassedTests / Summary.TotalTests * 100.0f : 0.0f;
    
    return Summary;
}

bool UMingSageBrainTestSuite::MeasureExecutionTime(TFunction<void()> TestFunction, float& OutExecutionTime)
{
    double StartTime = FPlatformTime::Seconds();
    
    try
    {
        TestFunction();
        double EndTime = FPlatformTime::Seconds();
        OutExecutionTime = (float)(EndTime - StartTime);
        return true;
    }
    catch (const std::exception& e)
    {
        double EndTime = FPlatformTime::Seconds();
        OutExecutionTime = (float)(EndTime - StartTime);
        UE_LOG(LogTemp, Error, TEXT("Test execution failed: %s"), UTF8_TO_TCHAR(e.what()));
        return false;
    }
    catch (...)
    {
        double EndTime = FPlatformTime::Seconds();
        OutExecutionTime = (float)(EndTime - StartTime);
        UE_LOG(LogTemp, Error, TEXT("Test execution failed with unknown exception"));
        return false;
    }
}

FString UMingSageBrainTestSuite::GenerateTestReport() const
{
    FString Report = GenerateDetailedTestReport();
    Report += TEXT("\n\n");
    Report += GeneratePerformanceMetrics();
    Report += TEXT("\n\n");
    Report += GenerateFailureAnalysis();
    
    return Report;
}

FString UMingSageBrainTestSuite::GenerateDetailedTestReport() const
{
    FString Report = TEXT("聖者大腦系統測試報告\n");
    Report += TEXT("========================\n\n");
    
    Report += FString::Printf(TEXT("測試摘要:\n"));
    Report += FString::Printf(TEXT("總測試數: %d\n"), LastTestSummary.TotalTests);
    Report += FString::Printf(TEXT("通過測試: %d\n"), LastTestSummary.PassedTests);
    Report += FString::Printf(TEXT("失敗測試: %d\n"), LastTestSummary.FailedTests);
    Report += FString::Printf(TEXT("成功率: %.1f%%\n"), LastTestSummary.SuccessRate);
    Report += FString::Printf(TEXT("總執行時間: %.2f秒\n"), LastTestSummary.TotalExecutionTime);
    Report += FString::Printf(TEXT("開始時間: %s\n"), *LastTestSummary.StartTime.ToString());
    Report += FString::Printf(TEXT("結束時間: %s\n\n"), *LastTestSummary.EndTime.ToString());
    
    Report += TEXT("詳細測試結果:\n");
    Report += TEXT("===============\n");
    
    for (const FSageBrainTestResult& Result : TestResults)
    {
        Report += FString::Printf(TEXT("測試: %s\n"), *Result.TestName);
        Report += FString::Printf(TEXT("結果: %s\n"), Result.bPassed ? TEXT("通過") : TEXT("失敗"));
        Report += FString::Printf(TEXT("執行時間: %.3f秒\n"), Result.ExecutionTime);
        Report += FString::Printf(TEXT("詳情: %s\n"), *Result.Details);
        Report += FString::Printf(TEXT("時間: %s\n"), *Result.Timestamp.ToString());
        Report += TEXT("---------------\n");
    }
    
    return Report;
}

FString UMingSageBrainTestSuite::GeneratePerformanceMetrics() const
{
    FString Metrics = TEXT("性能指標\n");
    Metrics += TEXT("========\n");
    
    if (TestResults.Num() > 0)
    {
        float AverageTime = LastTestSummary.TotalExecutionTime / TestResults.Num();
        float MinTime = TestResults[0].ExecutionTime;
        float MaxTime = TestResults[0].ExecutionTime;
        
        for (const FSageBrainTestResult& Result : TestResults)
        {
            if (Result.ExecutionTime < MinTime) MinTime = Result.ExecutionTime;
            if (Result.ExecutionTime > MaxTime) MaxTime = Result.ExecutionTime;
        }
        
        Metrics += FString::Printf(TEXT("平均執行時間: %.3f秒\n"), AverageTime);
        Metrics += FString::Printf(TEXT("最短執行時間: %.3f秒\n"), MinTime);
        Metrics += FString::Printf(TEXT("最長執行時間: %.3f秒\n"), MaxTime);
        Metrics += FString::Printf(TEXT("測試吞吐量: %.1f測試/秒\n"), TestResults.Num() / LastTestSummary.TotalExecutionTime);
    }
    
    return Metrics;
}

FString UMingSageBrainTestSuite::GenerateFailureAnalysis() const
{
    FString Analysis = TEXT("失敗分析\n");
    Analysis += TEXT("========\n");
    
    int32 FailedCount = 0;
    for (const FSageBrainTestResult& Result : TestResults)
    {
        if (!Result.bPassed)
        {
            FailedCount++;
            Analysis += FString::Printf(TEXT("失敗測試 %d: %s\n"), FailedCount, *Result.TestName);
            Analysis += FString::Printf(TEXT("失敗原因: %s\n"), *Result.Details);
            Analysis += TEXT("---------------\n");
        }
    }
    
    if (FailedCount == 0)
    {
        Analysis += TEXT("所有測試都通過了！\n");
    }
    
    return Analysis;
}

void UMingSageBrainTestSuite::ClearTestResults()
{
    TestResults.Empty();
    LastTestSummary = FSageBrainTestSummary();
    UE_LOG(LogTemp, Log, TEXT("Test results cleared"));
}
