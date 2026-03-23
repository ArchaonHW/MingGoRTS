#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingSageBrainCoreSystem.h"
#include "MingSageBrainLauncher.h"
#include "MingSageBrainSelfLearningSystem.h"
#include "MingSupremeSageCommandSystem.h"
#include "MingThreeAuthoritiesManager.h"
#include "MingFiveElementsRotation.h"
#include "MingCorruptionPrevention.h"
#include "MingSageBrainBridge.generated.h"

/**
 * 聖者大腦與Python API集成橋樑
 * 將聖者大腦系統與現有的Python API系統集成
 */
UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingSageBrainBridge : public UObject
{
    GENERATED_BODY()

public:
    UMingSageBrainBridge();

    // 初始化橋樑
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge")
    bool InitializeBridge(UMingSageBrainCoreSystem* SageBrainSystem);

    // 關閉橋樑
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge")
    void ShutdownBridge();

    // 聖者大腦增強的Python API功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Enhanced")
    FString GenerateSageBrainMusic(const FString& Prompt, ESageBrainThinkingLayer ThinkingLayer);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Enhanced")
    FString GenerateSageBrainScene(const FString& Description, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Enhanced")
    FString GenerateSageBrainAssets(const FString& AssetType, ESageBrainLearningAlgorithm Algorithm);

    // 智能決策支持
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Decision")
    FString MakeStrategicDecision(const FString& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Decision")
    FString OptimizeGameplay(const FString& CurrentState);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Decision")
    FString PredictPlayerBehavior(const FString& PlayerData);

    // 文化內容生成
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Culture")
    FString GenerateCulturalContent(const FString& Theme, ESageBrainPhilosophy Philosophy);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Culture")
    FString CreateHistoricalScenario(const FString& Period, ESageBrainThinkingLayer ThinkingLayer);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Culture")
    FString DesignPhilosophicalQuest(const FString& QuestType);

    // 自動化系統管理
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Automation")
    bool AutomateAssetGeneration();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Automation")
    bool OptimizeSystemPerformance();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Automation")
    bool AutoBalanceGameplay();

    // 高級分析功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Analysis")
    FString AnalyzeGameMetrics(const FString& MetricsData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Analysis")
    FString GenerateInsights(const FString& InputData);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Analysis")
    FString PredictTrends(const FString& HistoricalData);

    // 狀態查詢
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Status")
    bool IsBridgeInitialized() const { return bIsInitialized; }

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Status")
    FString GetBridgeStatus() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Status")
    ESageBrainConsciousness GetConsciousnessLevel() const;

    // 編譯優化工作流程
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    bool RunCompileOptimizationWorkflow();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    bool EnableCompileOptimizer();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    bool IsCompileOptimizerEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    void SetBuildMode(const FString& Mode);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    void SetSkipClean(bool bSkip);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    void SetAutoFix(bool bAuto);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    void SetCreateTasks(bool bCreate);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    TArray<FMingTaskRequirement> GetGeneratedTasks() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Compile")
    FCompileOptimizationReport GetLastCompileReport() const;

    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSageBrainDecision, const FString&, Context, const FString&, Decision);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConsciousnessEvolved, ESageBrainConsciousness, NewLevel);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEnhancedContentGenerated, const FString&, ContentType, const FString&, Content);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompileOptimizationCompleted, const FCompileOptimizationReport&, Report);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCompileProgressUpdated, int32, Percentage, const FString&, Message);

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Bridge|Events")
    FOnSageBrainDecision OnSageBrainDecision;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Bridge|Events")
    FOnConsciousnessEvolved OnConsciousnessEvolved;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Bridge|Events")
    FOnEnhancedContentGenerated OnEnhancedContentGenerated;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Bridge|Events")
    FOnCompileOptimizationCompleted OnCompileOptimizationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Sage Brain|Bridge|Events")
    FOnCompileProgressUpdated OnCompileProgressUpdated;

    // 自我思考與學習功能
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    bool EnableSelfThinking(ESelfThinkingMode Mode, float IntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    void DisableSelfThinking();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    bool IsSelfThinkingActive() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    FSelfThinkingRecord TriggerSelfThinking(const FString& Context, ESelfThinkingMode Mode);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    bool EnableContinuousLearning(float CheckIntervalSeconds);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    void DisableContinuousLearning();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    bool IsContinuousLearningActive() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    TArray<FSelfImprovementSuggestion> GetSelfImprovementSuggestions();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    bool ImplementSelfImprovement(const FString& SuggestionID);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Self Learning")
    FString GetSelfLearningStatus() const;

private:
    bool bIsInitialized;
    
    UPROPERTY()
    TObjectPtr<UMingSageBrainCoreSystem> SageBrainSystem;

    UPROPERTY()
    TObjectPtr<UMingSageBrainLauncher> SageBrainLauncher;

    UPROPERTY()
    TObjectPtr<UMingSageBrainSelfLearningSystem> SelfLearningSystem;

    UPROPERTY()
    TObjectPtr<UMingSupremeSageCommandSystem> SupremeSageSystem;

    // ========== 至聖者指揮系統接口 ==========
public:
    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool InitializeSupremeSageSystem();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool EnterFiveElementPhase(EFiveElementPhase Phase, const FPhaseContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool ExecuteSixConquestStrategy(ESixConquestStrategy Strategy, EStrategyApproach Approach, const FConquestTarget& Target);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool ExecuteTwelveStrategy(ETwelveStrategies Strategy, const FStrategyContext& Context);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    FCorruptionCheckResult PerformCorruptionCheck();

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    FCommandRecommendation GetSageCommandRecommendation() const;

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool SwitchAuthority(ESupremeAuthorityType AuthorityType);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    bool DeployForce(EForceType ForceType, const FDeploymentConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Sage Brain|Bridge|Supreme Sage")
    FSelfAuditReport GenerateSageSelfAuditReport();

    // 內部輔助函數
    FString EnhanceWithThinking(const FString& BaseContent, ESageBrainThinkingLayer Layer);
    FString EnhanceWithPhilosophy(const FString& BaseContent, ESageBrainPhilosophy Philosophy);
    FString EnhanceWithLearning(const FString& BaseContent, ESageBrainLearningAlgorithm Algorithm);
    FString EnhanceWithConsciousness(const FString& BaseContent);
    
    bool ValidateSystems() const;
    FString GenerateEnhancedPrompt(const FString& BasePrompt, ESageBrainThinkingLayer Layer, ESageBrainPhilosophy Philosophy);
    void LogBridgeActivity(const FString& Activity) const;

    // 編譯優化事件處理
    UFUNCTION()
    void OnLauncherWorkflowCompleted(bool bSuccess, const FCompileOptimizationReport& Report);

    UFUNCTION()
    void OnLauncherProgressUpdated(int32 Percentage, const FString& Message);
};
