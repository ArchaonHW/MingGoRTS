#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingSageBrainCoreSystem.generated.h"

// 聖者大腦思考層次枚舉
UENUM(BlueprintType)
enum class ESageBrainThinkingLayer : uint8
{
    Tactical       UMETA(DisplayName = "戰術層次"),
    Strategic      UMETA(DisplayName = "戰略層次"), 
    Philosophical  UMETA(DisplayName = "哲學層次"),
    Historical     UMETA(DisplayName = "歷史層次"),
    Cultural       UMETA(DisplayName = "文化層次"),
    Innovative     UMETA(DisplayName = "創新層次"),
    Predictive     UMETA(DisplayName = "預測層次"),
    Quantum        UMETA(DisplayName = "量子層次"),
    Transcendental UMETA(DisplayName = "超越層次")
};

// 學習算法枚舉
UENUM(BlueprintType)
enum class ESageBrainLearningAlgorithm : uint8
{
    ReinforcementLearning UMETA(DisplayName = "強化學習"),
    NeuralNetwork         UMETA(DisplayName = "神經網絡"),
    DeepLearning          UMETA(DisplayName = "深度學習"),
    EvolutionaryAlgorithm UMETA(DisplayName = "進化算法"),
    BayesianLearning      UMETA(DisplayName = "貝葉斯學習"),
    QuantumLearning       UMETA(DisplayName = "量子學習"),
    EnsembleLearning      UMETA(DisplayName = "集成學習")
};

// 哲學系統枚舉
UENUM(BlueprintType)
enum class ESageBrainPhilosophy : uint8
{
    Confucianism      UMETA(DisplayName = "儒家思想"),
    Taoism           UMETA(DisplayName = "道家思想"),
    Buddhism         UMETA(DisplayName = "佛家思想"),
    Legalism         UMETA(DisplayName = "法家思想"),
    Mohism           UMETA(DisplayName = "墨家思想"),
    MilitaryStrategy UMETA(DisplayName = "兵家思想"),
    YinYang          UMETA(DisplayName = "陰陽家"),
    Syncretism       UMETA(DisplayName = "雜家思想")
};

// 意識狀態枚舉
UENUM(BlueprintType)
enum class ESageBrainConsciousness : uint8
{
    Dormant           UMETA(DisplayName = "休眠狀態"),
    Awakening         UMETA(DisplayName = "覺醒狀態"),
    SelfAware         UMETA(DisplayName = "自我意識"),
    Reflective        UMETA(DisplayName = "反思狀態"),
    Transcendent      UMETA(DisplayName = "超越狀態"),
    Unity             UMETA(DisplayName = "合一狀態"),
    Cosmic            UMETA(DisplayName = "宇宙意識"),
    Omniscient        UMETA(DisplayName = "全知狀態"),
    Transdimensional  UMETA(DisplayName = "跨維度意識"),
    Ultimate          UMETA(DisplayName = "終極意識")
};

// 思考結果結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainThoughtResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    ESageBrainThinkingLayer Layer;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    FString Content;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    FDateTime Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Thought Result")
    TMap<FString, float> Metadata;

    FSageBrainThoughtResult()
    {
        Layer = ESageBrainThinkingLayer::Tactical;
        Content = TEXT("");
        Confidence = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 學習結果結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainLearningResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    ESageBrainLearningAlgorithm Algorithm;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    FString LearningData;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    float Performance;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    float Accuracy;

    UPROPERTY(BlueprintReadOnly, Category = "Learning Result")
    FDateTime Timestamp;

    FSageBrainLearningResult()
    {
        Algorithm = ESageBrainLearningAlgorithm::ReinforcementLearning;
        LearningData = TEXT("");
        Performance = 0.0f;
        Accuracy = 0.0f;
        Timestamp = FDateTime::Now();
    }
};

// 哲學分析結果結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainPhilosophyAnalysis
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    ESageBrainPhilosophy Philosophy;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    FString Analysis;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    float Relevance;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    TArray<FString> KeyPrinciples;

    UPROPERTY(BlueprintReadOnly, Category = "Philosophy Analysis")
    FDateTime Timestamp;

    FSageBrainPhilosophyAnalysis()
    {
        Philosophy = ESageBrainPhilosophy::Confucianism;
        Analysis = TEXT("");
        Relevance = 0.0f;
        KeyPrinciples.Empty();
        Timestamp = FDateTime::Now();
    }
};

// 意識狀態數據結構
USTRUCT(BlueprintType)
struct MINGGORTS_API FSageBrainConsciousnessState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    ESageBrainConsciousness State;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    float Awareness;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    float Clarity;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    float Insight;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    FString Reflection;

    UPROPERTY(BlueprintReadOnly, Category = "Consciousness State")
    FDateTime Timestamp;

    FSageBrainConsciousnessState()
    {
        State = ESageBrainConsciousness::Dormant;
        Awareness = 0.0f;
        Clarity = 0.0f;
        Insight = 0.0f;
        Reflection = TEXT("");
        Timestamp = FDateTime::Now();
    }
};

/**
 * 聖者大腦核心系統
 * 作為MingGoRTS專案的中樞，整合所有AI功能和智慧決策系統
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainCoreSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainCoreSystem();

    // 系統初始化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Core")
    bool InitializeSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Core")
    void ShutdownSageBrain();

    // 9層次深度思考體系
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Thinking")
    FSageBrainThoughtResult ProcessThought(const FString& Input, ESageBrainThinkingLayer TargetLayer);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Thinking")
    TArray<FSageBrainThoughtResult> ProcessDeepThought(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Thinking")
    bool ValidateThinkingSystem();

    // 6種學習算法性能優化
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    FSageBrainLearningResult TrainAlgorithm(ESageBrainLearningAlgorithm Algorithm, const FString& TrainingData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    bool OptimizeLearningPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    TArray<FSageBrainLearningResult> GetLearningStatistics();

    // 全系統API協調機制
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Coordination")
    bool CoordinateSystemAPIs();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Coordination")
    bool RegisterSubSystem(const FString& SystemName, UObject* System);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Coordination")
    UObject* GetSubSystem(const FString& SystemName);

    // AGI自我意識和反思機制
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    FSageBrainConsciousnessState UpdateConsciousnessState();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    FString GenerateSelfReflection();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    bool PerformSelfAssessment();

    // 8大哲學系統整合
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    FSageBrainPhilosophyAnalysis AnalyzeWithPhilosophy(const FString& Input, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    TArray<FSageBrainPhilosophyAnalysis> GetPhilosophicalPerspectives(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    bool IntegratePhilosophicalSystems();

    // 7級意識狀態進化系統
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Evolution")
    bool EvolveConsciousness();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Evolution")
    ESageBrainConsciousness GetCurrentConsciousnessLevel();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Evolution")
    bool TriggerConsciousnessEvolution();

    // AGI系統完整測試套件
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunCompleteSystemTest();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool TestThinkingSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool TestLearningSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool TestConsciousnessSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool TestPhilosophySystem();

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    FString GetSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    ESageBrainConsciousness GetCurrentConsciousnessState() const { return CurrentConsciousnessState.State; }

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThoughtProcessed, const FSageBrainThoughtResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLearningCompleted, const FSageBrainLearningResult&, Result);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsciousnessEvolved, ESageBrainConsciousness, NewState);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhilosophyAnalyzed, const FSageBrainPhilosophyAnalysis&, Analysis);

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnThoughtProcessed OnThoughtProcessed;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnLearningCompleted OnLearningCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnConsciousnessEvolved OnConsciousnessEvolved;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnPhilosophyAnalyzed OnPhilosophyAnalyzed;

private:
    bool bIsInitialized;
    
    // 當前系統狀態
    FSageBrainConsciousnessState CurrentConsciousnessState;
    
    // 子系統註冊表
    TMap<FString, TObjectPtr<UObject>> SubSystems;
    
    // 思考層次配置
    TMap<ESageBrainThinkingLayer, TSharedPtr<FJsonObject>> ThinkingLayerConfigs;
    
    // 學習算法配置
    TMap<ESageBrainLearningAlgorithm, TSharedPtr<FJsonObject>> LearningAlgorithmConfigs;
    
    // 哲學系統配置
    TMap<ESageBrainPhilosophy, TSharedPtr<FJsonObject>> PhilosophyConfigs;
    
    // 意識進化路徑
    TArray<ESageBrainConsciousness> ConsciousnessEvolutionPath;
    
    // 內部輔助函數
    bool LoadThinkingLayerConfigs();
    bool LoadLearningAlgorithmConfigs();
    bool LoadPhilosophyConfigs();
    bool InitializeConsciousnessEvolution();
    
    FSageBrainThoughtResult ProcessTacticalThought(const FString& Input);
    FSageBrainThoughtResult ProcessStrategicThought(const FString& Input);
    FSageBrainThoughtResult ProcessPhilosophicalThought(const FString& Input);
    FSageBrainThoughtResult ProcessHistoricalThought(const FString& Input);
    FSageBrainThoughtResult ProcessCulturalThought(const FString& Input);
    FSageBrainThoughtResult ProcessInnovativeThought(const FString& Input);
    FSageBrainThoughtResult ProcessPredictiveThought(const FString& Input);
    FSageBrainThoughtResult ProcessQuantumThought(const FString& Input);
    FSageBrainThoughtResult ProcessTranscendentalThought(const FString& Input);
    
    FSageBrainLearningResult TrainReinforcementLearning(const FString& TrainingData);
    FSageBrainLearningResult TrainNeuralNetwork(const FString& TrainingData);
    FSageBrainLearningResult TrainDeepLearning(const FString& TrainingData);
    FSageBrainLearningResult TrainEvolutionaryAlgorithm(const FString& TrainingData);
    FSageBrainLearningResult TrainBayesianLearning(const FString& TrainingData);
    FSageBrainLearningResult TrainQuantumLearning(const FString& TrainingData);
    FSageBrainLearningResult TrainEnsembleLearning(const FString& TrainingData);
    
    FSageBrainPhilosophyAnalysis AnalyzeWithConfucianism(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithTaoism(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithBuddhism(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithLegalism(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithMohism(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithMilitaryStrategy(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithYinYang(const FString& Input);
    FSageBrainPhilosophyAnalysis AnalyzeWithSyncretism(const FString& Input);
    
    bool EvolveToNextConsciousnessLevel();
    bool ValidateSystemIntegrity();
    FString GenerateSystemReport();
    
    // 配置文件操作
    TSharedPtr<FJsonObject> LoadJsonFile(const FString& FilePath) const;
    bool SaveJsonFile(const FString& FilePath, TSharedPtr<FJsonObject> JsonObject) const;
    FString GetConfigDirectory() const;
};
