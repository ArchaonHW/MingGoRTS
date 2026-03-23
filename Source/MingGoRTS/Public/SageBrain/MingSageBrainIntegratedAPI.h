#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "MingSageBrainCoreSystem.h"
#include "MingRTSAssetGenerationAPI.h"
#include "MingRTSAudioGenerationAPI.h"
#include "API/MingRTSCppReferenceIntegration.h"
#include "MingSageBrainIntegratedAPI.generated.h"

/**
 * 聖者大腦整合API系統
 * 將所有API功能整合到聖者大腦中，提供統一的智能接口
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainIntegratedAPI : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainIntegratedAPI();

    // 初始化整合API系統
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Integrated API")
    bool InitializeIntegratedAPI();

    // 關閉整合API系統
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Integrated API")
    void ShutdownIntegratedAPI();

    // 獲取系統狀態
    UFUNCTION(BlueprintCallable, Category = "Sage Brain Integrated API")
    FString GetIntegratedSystemStatus() const;

    // ========================================
    // 聖者大腦增強的音樂生成API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Music")
    FString GenerateSageBrainMusic(const FString& Prompt, int32 Duration = 10, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Cultural, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Taoism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Music")
    FString GeneratePhilosophicalMusic(const FString& Philosophy, ESageBrainPhilosophy PhilosophyType = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Music")
    FString GenerateHistoricalMusic(const FString& HistoricalPeriod, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Historical);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Music")
    FString GenerateStrategicMusic(const FString& BattleContext, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Strategic);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Music")
    TArray<FString> GenerateCompleteMusicSuite(const FString& GameTheme);

    // ========================================
    // 聖者大腦增強的資產生成API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateSageBrainScene(const FString& Description, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Cultural);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateSageBrainCharacter(const FString& CharacterDescription, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateSageBrainBuilding(const FString& BuildingDescription, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateSageBrainEnvironment(const FString& EnvironmentDescription, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Cultural);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateSageBrainAssetBatch(const TArray<FAssetRequirement>& Requirements, ESageBrainLearningAlgorithm Algorithm = ESageBrainLearningAlgorithm::DeepLearning);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Assets")
    FString GenerateEpicAssetsWithSageBrain(const FString& EpicID, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Strategic);

    // ========================================
    // 聖者大腦增強的技能系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Skills")
    bool UnlockSageBrainSkill(const FString& SkillID, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Skills")
    bool TrainSkillWithSageBrain(const FString& SkillID, int32 Amount, ESageBrainLearningAlgorithm Algorithm = ESageBrainLearningAlgorithm::ReinforcementLearning);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Skills")
    float CalculateSageBrainSkillEffect(const FString& SkillID, const FString& EffectType, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Tactical);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Skills")
    bool ChoosePhilosophicalSpecialization(const FString& SpecializationPath, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Skills")
    TArray<FString> GetRecommendedSkills(const FString& PlayerProfile);

    // ========================================
    // 聖者大腦增強的場景生成API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Scene")
    FString GenerateSageBrainScene(const FString& Description, const FString& SceneType = TEXT("battle"), ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Cultural);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Scene")
    TArray<FString> GetSageBrainRecommendedScenes(const FString& Context, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Scene")
    TArray<FString> GenerateSageBrainSceneVariants(const FString& BaseDescription, int32 VariantCount = 3, ESageBrainLearningAlgorithm Algorithm = ESageBrainLearningAlgorithm::EvolutionaryAlgorithm);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Scene")
    FString GenerateHistoricalSceneWithSageBrain(const FString& HistoricalPeriod, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Historical);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Scene")
    FString GeneratePhilosophicalScene(const FString& PhilosophicalConcept, ESageBrainPhilosophy Philosophy);

    // ========================================
    // 聖者大腦增強的測試系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunSageBrainSystemTests();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunSageBrainSkillTests();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunSageBrainRelationshipTests();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunSageBrainDialogueTests();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunSageBrainHistoricalTests();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    bool RunCompleteSageBrainTestSuite();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Testing")
    FString GenerateTestReport();

    // ========================================
    // 聖者大腦增強的工具函數API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    bool FixEncodingWithSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    bool CheckProjectHealthWithSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    bool OptimizePerformanceWithSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    FString GenerateProjectInsights();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    FString PredictProjectTrends();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Utilities")
    bool AutoOptimizeProject();

    // ========================================
    // 聖者大腦高級決策API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Decision")
    FString MakeStrategicGameDecision(const FString& GameContext, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Strategic);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Decision")
    FString OptimizeGameBalance(const FString& CurrentBalance, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Legalism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Decision")
    FString PredictPlayerBehavior(const FString& PlayerData, ESageBrainLearningAlgorithm Algorithm = ESageBrainLearningAlgorithm::DeepLearning);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Decision")
    FString GenerateGameDesignInsights(const FString& DesignProblem);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Decision")
    FString CreateContentStrategy(const FString& ContentGoals);

    // ========================================
    // 聖者大腦文化內容生成API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Culture")
    FString GenerateCulturalContent(const FString& Theme, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Culture")
    FString CreateHistoricalScenario(const FString& Period, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Historical);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Culture")
    FString DesignPhilosophicalQuest(const FString& QuestType, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Buddhism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Culture")
    FString GenerateCulturalDialogue(const FString& DialogueContext, ESageBrainPhilosophy Philosophy = ESageBrainPhilosophy::Confucianism);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Culture")
    TArray<FString> GenerateCulturalAssetPack(const FString& CulturalTheme);

    // ========================================
    // 聖者大腦自動化系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutomateAssetGeneration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutomateMusicGeneration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutomateSceneGeneration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutomateContentCreation();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool OptimizeSystemPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutoBalanceGameplay();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Automation")
    bool AutoGenerateContent();

    // ========================================
    // 聖者大腦分析系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Analysis")
    FString AnalyzeGameMetrics(const FString& MetricsData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Analysis")
    FString GenerateDeepInsights(const FString& InputData, ESageBrainThinkingLayer ThinkingLayer = ESageBrainThinkingLayer::Philosophical);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Analysis")
    FString PredictGameTrends(const FString& HistoricalData, ESageBrainLearningAlgorithm Algorithm = ESageBrainLearningAlgorithm::QuantumLearning);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Analysis")
    FString AnalyzePlayerBehavior(const FString& PlayerData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Analysis")
    FString GenerateContentAnalysis(const FString& ContentData);

    // ========================================
    // 聖者大腦意識狀態管理API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    ESageBrainConsciousness GetCurrentConsciousnessLevel();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    bool EvolveConsciousness();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    FString GenerateSelfReflection();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    bool PerformSelfAssessment();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Consciousness")
    FString GetConsciousnessReport();

    // ========================================
    // 聖者大腦哲學系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    FSageBrainPhilosophyAnalysis AnalyzeWithPhilosophy(const FString& Input, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    TArray<FSageBrainPhilosophyAnalysis> GetPhilosophicalPerspectives(const FString& Input);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    FString ApplyPhilosophicalPrinciples(const FString& Problem, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    bool IntegratePhilosophicalSystems();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Philosophy")
    FString GeneratePhilosophicalGuidance(ESageBrainPhilosophy Philosophy);

    // ========================================
    // 聖者大腦學習系統API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    FSageBrainLearningResult TrainLearningAlgorithm(ESageBrainLearningAlgorithm Algorithm, const FString& TrainingData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    bool OptimizeLearningPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    TArray<FSageBrainLearningResult> GetLearningStatistics();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    FString GenerateLearningInsights();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Learning")
    bool ApplyLearningToGameplay();

    // ========================================
    // 聖者大腦 C++ Reference 整合API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    bool LoadCppReferenceToSageBrain();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    FString GenerateCodeWithSageBrainKnowledge(const FString& Topic, const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    TArray<FString> GetSageBrainCppTopics(const FString& CategoryName) const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    FString ExplainCppConceptWithSageBrain(const FString& Concept);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    FString CreatePhilosophicalCodeExample(ESageBrainPhilosophy Philosophy, const FString& CppTopic);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|CppReference")
    bool IntegrateCppReferenceIntoLearning();

    // ========================================
    // 狀態查詢API
    // ========================================

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    bool IsInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    FString GetSageBrainStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    FString GetAPIStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    TMap<FString, FString> GetAllSystemStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Status")
    bool ValidateAllSystems();

    // ========================================
    // 事件委託
    // ========================================

    // 聖者大腦事件
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSageBrainDecision, const FString&, Decision);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsciousnessEvolved, ESageBrainConsciousness, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnEnhancedContentGenerated, const FString&, ContentType, const FString&, Content);

    // 整合API事件
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnIntegratedAPIInitialized, bool, bSuccess, const FString&, Message);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSageBrainMusicGenerated, const FString&, MusicPath, const FString&, Philosophy);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSageBrainAssetGenerated, const FString&, AssetPath, const FString&, AssetType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParam(FOnSageBrainDecisionMade, const FString&, Context, const FString&, Decision);

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnSageBrainDecision OnSageBrainDecision;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnConsciousnessEvolved OnConsciousnessEvolved;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnEnhancedContentGenerated OnEnhancedContentGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnIntegratedAPIInitialized OnIntegratedAPIInitialized;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnSageBrainMusicGenerated OnSageBrainMusicGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnSageBrainAssetGenerated OnSageBrainAssetGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Events")
    FOnSageBrainDecisionMade OnSageBrainDecisionMade;

private:
    bool bIsInitialized;

    // 核心系統組件
    UPROPERTY()
    TObjectPtr<UMingSageBrainCoreSystem> SageBrainSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSAssetGenerationAPI> AssetGenerationAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSAudioGenerationAPI> AudioGenerationAPI;

    UPROPERTY()
    TObjectPtr<UMingRTSCppReferenceIntegration> CppReferenceIntegration;

    // 內部輔助函數
    bool InitializeAllSubSystems();
    bool ValidateSubSystems();
    void BindEvents();
    void UnbindEvents();

    // 聖者大腦增強輔助函數
    FString EnhanceWithSageBrain(const FString& BaseContent, ESageBrainThinkingLayer Layer, ESageBrainPhilosophy Philosophy);
    FString GenerateSageBrainPrompt(const FString& BasePrompt, ESageBrainThinkingLayer Layer, ESageBrainPhilosophy Philosophy);
    bool ApplySageBrainOptimization(const FString& SystemName);

    // 內容生成輔助函數
    FString GenerateMusicWithPhilosophy(const FString& Prompt, ESageBrainPhilosophy Philosophy);
    FString GenerateAssetWithThinking(const FString& Description, ESageBrainThinkingLayer Layer);
    FString GenerateSceneWithLearning(const FString& Description, ESageBrainLearningAlgorithm Algorithm);

    // 系統協調輔助函數
    bool CoordinateAPIs();
    FString GenerateSystemReport();
    bool PerformSystemHealthCheck();

    // 日誌和調試
    void LogIntegratedAPIActivity(const FString& Activity) const;
    void LogSageBrainActivity(const FString& Activity) const;
};
