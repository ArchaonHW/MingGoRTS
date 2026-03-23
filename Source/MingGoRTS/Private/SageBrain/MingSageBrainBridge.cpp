#include "MingSageBrainBridge.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/DateTime.h"
#include "Engine/World.h"

UMingSageBrainBridge::UMingSageBrainBridge()
    : bIsInitialized(false)
{
    UE_LOG(LogTemp, Log, TEXT("MingSageBrainBridge constructor called"));
}

bool UMingSageBrainBridge::InitializeBridge(UMingSageBrainCoreSystem* InSageBrainSystem)
{
    if (bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sage Brain Bridge already initialized"));
        return true;
    }

    if (!InSageBrainSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid SageBrainSystem"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Initializing MingSageBrainBridge..."));

    SageBrainSystem = InSageBrainSystem;

    // 驗證系統狀態
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("System validation failed"));
        return false;
    }

    // 確保聖者大腦系統已初始化
    if (!SageBrainSystem->IsInitialized())
    {
        if (!SageBrainSystem->InitializeSageBrain())
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to initialize Sage Brain system"));
            return false;
        }
    }

    // 綁定聖者大腦事件
    SageBrainSystem->OnConsciousnessEvolved.AddDynamic(this, &UMingSageBrainBridge::OnConsciousnessEvolved);
    SageBrainSystem->OnThoughtProcessed.AddDynamic(this, &UMingSageBrainBridge::OnSageBrainDecision);
    SageBrainSystem->OnPhilosophyAnalyzed.AddDynamic(this, &UMingSageBrainBridge::OnEnhancedContentGenerated);

    bIsInitialized = true;
    
    LogBridgeActivity(TEXT("Bridge initialized successfully"));
    UE_LOG(LogTemp, Log, TEXT("MingSageBrainBridge initialized successfully"));
    
    return true;
}

void UMingSageBrainBridge::ShutdownBridge()
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("Sage Brain Bridge not initialized"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("Shutting down MingSageBrainBridge..."));

    // 解綁事件
    if (SageBrainSystem)
    {
        SageBrainSystem->OnConsciousnessEvolved.RemoveDynamic(this, &UMingSageBrainBridge::OnConsciousnessEvolved);
        SageBrainSystem->OnThoughtProcessed.RemoveDynamic(this, &UMingSageBrainBridge::OnSageBrainDecision);
        SageBrainSystem->OnPhilosophyAnalyzed.RemoveDynamic(this, &UMingSageBrainBridge::OnEnhancedContentGenerated);
    }

    // 解綁啟動器事件
    if (SageBrainLauncher)
    {
        SageBrainLauncher->OnWorkflowCompleted.RemoveDynamic(this, &UMingSageBrainBridge::OnLauncherWorkflowCompleted);
        SageBrainLauncher->OnProgressUpdated.RemoveDynamic(this, &UMingSageBrainBridge::OnLauncherProgressUpdated);
    }

    SageBrainSystem = nullptr;
    SageBrainLauncher = nullptr;
    bIsInitialized = false;

    LogBridgeActivity(TEXT("Bridge shutdown complete"));
    UE_LOG(LogTemp, Log, TEXT("MingSageBrainBridge shutdown complete"));
}

FString UMingSageBrainBridge::GenerateSageBrainMusic(const FString& Prompt, ESageBrainThinkingLayer ThinkingLayer)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for music generation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Generating Sage Brain music with thinking layer %d: %s"), (int32)ThinkingLayer, *Prompt);

    // 使用聖者大腦增強提示詞
    FString EnhancedPrompt = GenerateEnhancedPrompt(Prompt, ThinkingLayer, ESageBrainPhilosophy::Taoism);
    
    // 進行思考處理
    FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(EnhancedPrompt, ThinkingLayer);
    
    // 使用增強後的提示詞生成音樂
    FString MusicPath = PythonAPI->GenerateMusic(ThoughtResult.Content, 180.0f);
    
    if (!MusicPath.IsEmpty())
    {
        LogBridgeActivity(FString::Printf(TEXT("Generated Sage Brain music: %s"), *MusicPath));
        OnEnhancedContentGenerated.Broadcast(TEXT("Music"), MusicPath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to generate Sage Brain music"));
    }

    return MusicPath;
}

FString UMingSageBrainBridge::GenerateSageBrainScene(const FString& Description, ESageBrainPhilosophy Philosophy)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for scene generation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Generating Sage Brain scene with philosophy %d: %s"), (int32)Philosophy, *Description);

    // 使用哲學系統增強描述
    FString EnhancedDescription = EnhanceWithPhilosophy(Description, Philosophy);
    
    // 進行哲學分析
    FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(EnhancedDescription, Philosophy);
    
    // 使用增強後的描述生成場景
    FString ScenePath = PythonAPI->GenerateScene(PhilosophyAnalysis.Analysis);
    
    if (!ScenePath.IsEmpty())
    {
        LogBridgeActivity(FString::Printf(TEXT("Generated Sage Brain scene: %s"), *ScenePath));
        OnEnhancedContentGenerated.Broadcast(TEXT("Scene"), ScenePath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to generate Sage Brain scene"));
    }

    return ScenePath;
}

FString UMingSageBrainBridge::GenerateSageBrainAssets(const FString& AssetType, ESageBrainLearningAlgorithm Algorithm)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for asset generation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Generating Sage Brain assets with algorithm %d: %s"), (int32)Algorithm, *AssetType);

    // 使用學習算法增強資產生成
    FString EnhancedAssetType = EnhanceWithLearning(AssetType, Algorithm);
    
    // 訓練學習算法
    FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(Algorithm, EnhancedAssetType);
    
    // 使用學習結果生成資產
    FString AssetPath = PythonAPI->GenerateAsset(EnhancedAssetType, LearningResult.LearningData);
    
    if (!AssetPath.IsEmpty())
    {
        LogBridgeActivity(FString::Printf(TEXT("Generated Sage Brain assets: %s"), *AssetPath));
        OnEnhancedContentGenerated.Broadcast(TEXT("Asset"), AssetPath);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to generate Sage Brain assets"));
    }

    return AssetPath;
}

FString UMingSageBrainBridge::MakeStrategicDecision(const FString& Context)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for strategic decision"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Making strategic decision: %s"), *Context);

    // 使用戰略思考層次
    FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(Context, ESageBrainThinkingLayer::Strategic);
    
    // 使用哲學系統分析
    FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
        ThoughtResult.Content, 
        ESageBrainPhilosophy::MilitaryStrategy
    );
    
    // 生成決策
    FString Decision = FString::Printf(
        TEXT("戰略決策分析:\n思考結果: %s\n哲學分析: %s\n建議: %s"),
        *ThoughtResult.Content,
        *PhilosophyAnalysis.Analysis,
        *SageBrainSystem->GenerateSelfReflection()
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Made strategic decision: %s"), *Decision));
    OnSageBrainDecision.Broadcast(Context, Decision);
    
    return Decision;
}

FString UMingSageBrainBridge::OptimizeGameplay(const FString& CurrentState)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for gameplay optimization"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Optimizing gameplay: %s"), *CurrentState);

    // 使用深度思考體系
    TArray<FSageBrainThoughtResult> DeepThoughtResults = SageBrainSystem->ProcessDeepThought(CurrentState);
    
    // 使用學習算法優化
    FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(
        ESageBrainLearningAlgorithm::EnsembleLearning,
        CurrentState
    );
    
    // 生成優化建議
    FString Optimization = TEXT("遊戲優化建議:\n");
    for (const FSageBrainThoughtResult& Result : DeepThoughtResults)
    {
        Optimization += FString::Printf(TEXT("[%s] %s\n"), 
            *UEnum::GetDisplayValueAsText(Result.Layer).ToString(),
            *Result.Content);
    }
    
    Optimization += FString::Printf(TEXT("學習算法建議: %s\n"), *LearningResult.LearningData);
    Optimization += FString::Printf(TEXT("意識反思: %s"), *SageBrainSystem->GenerateSelfReflection());
    
    LogBridgeActivity(FString::Printf(TEXT("Optimized gameplay: %s"), *Optimization));
    OnSageBrainDecision.Broadcast(CurrentState, Optimization);
    
    return Optimization;
}

FString UMingSageBrainBridge::PredictPlayerBehavior(const FString& PlayerData)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for player behavior prediction"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Predicting player behavior: %s"), *PlayerData);

    // 使用預測思考層次
    FSageBrainThoughtResult PredictiveThought = SageBrainSystem->ProcessThought(PlayerData, ESageBrainThinkingLayer::Predictive);
    
    // 使用量子思考層次
    FSageBrainThoughtResult QuantumThought = SageBrainSystem->ProcessThought(PlayerData, ESageBrainThinkingLayer::Quantum);
    
    // 使用深度學習算法
    FSageBrainLearningResult DeepLearningResult = SageBrainSystem->TrainAlgorithm(
        ESageBrainLearningAlgorithm::DeepLearning,
        PlayerData
    );
    
    // 生成預測
    FString Prediction = FString::Printf(
        TEXT("玩家行為預測:\n預測分析: %s\n量子分析: %s\n深度學習: %s\n信心度: %.2f"),
        *PredictiveThought.Content,
        *QuantumThought.Content,
        *DeepLearningResult.LearningData,
        (PredictiveThought.Confidence + QuantumThought.Confidence + DeepLearningResult.Accuracy) / 3.0f
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Predicted player behavior: %s"), *Prediction));
    OnSageBrainDecision.Broadcast(PlayerData, Prediction);
    
    return Prediction;
}

FString UMingSageBrainBridge::GenerateCulturalContent(const FString& Theme, ESageBrainPhilosophy Philosophy)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for cultural content generation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Generating cultural content with philosophy %d: %s"), (int32)Philosophy, *Theme);

    // 使用文化思考層次
    FSageBrainThoughtResult CulturalThought = SageBrainSystem->ProcessThought(Theme, ESageBrainThinkingLayer::Cultural);
    
    // 使用歷史思考層次
    FSageBrainThoughtResult HistoricalThought = SageBrainSystem->ProcessThought(Theme, ESageBrainThinkingLayer::Historical);
    
    // 使用哲學系統分析
    FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(Theme, Philosophy);
    
    // 生成文化內容
    FString CulturalContent = FString::Printf(
        TEXT("文化內容生成:\n主題: %s\n文化分析: %s\n歷史分析: %s\n哲學分析: %s\n關鍵原則: %s"),
        *Theme,
        *CulturalThought.Content,
        *HistoricalThought.Content,
        *PhilosophyAnalysis.Analysis,
        *FString::Join(TEXT(", "), PhilosophyAnalysis.KeyPrinciples)
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Generated cultural content: %s"), *CulturalContent));
    OnEnhancedContentGenerated.Broadcast(TEXT("Cultural"), CulturalContent);
    
    return CulturalContent;
}

FString UMingSageBrainBridge::CreateHistoricalScenario(const FString& Period, ESageBrainThinkingLayer ThinkingLayer)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for historical scenario creation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Creating historical scenario with thinking layer %d: %s"), (int32)ThinkingLayer, *Period);

    // 使用歷史思考層次
    FSageBrainThoughtResult HistoricalThought = SageBrainSystem->ProcessThought(Period, ESageBrainThinkingLayer::Historical);
    
    // 使用指定思考層次
    FSageBrainThoughtResult LayerThought = SageBrainSystem->ProcessThought(Period, ThinkingLayer);
    
    // 使用儒家哲學分析
    FSageBrainPhilosophyAnalysis ConfucianAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(Period, ESageBrainPhilosophy::Confucianism);
    
    // 生成歷史場景
    FString Scenario = FString::Printf(
        TEXT("歷史場景創建:\n時期: %s\n歷史分析: %s\n% s分析: %s\n儒家分析: %s\n場景描述: %s"),
        *Period,
        *HistoricalThought.Content,
        *UEnum::GetDisplayValueAsText(ThinkingLayer).ToString(),
        *LayerThought.Content,
        *ConfucianAnalysis.Analysis,
        *SageBrainSystem->GenerateSelfReflection()
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Created historical scenario: %s"), *Scenario));
    OnEnhancedContentGenerated.Broadcast(TEXT("Historical"), Scenario);
    
    return Scenario;
}

FString UMingSageBrainBridge::DesignPhilosophicalQuest(const FString& QuestType)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for philosophical quest design"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Designing philosophical quest: %s"), *QuestType);

    // 使用哲學思考層次
    FSageBrainThoughtResult PhilosophicalThought = SageBrainSystem->ProcessThought(QuestType, ESageBrainThinkingLayer::Philosophical);
    
    // 使用創新思考層次
    FSageBrainThoughtResult InnovativeThought = SageBrainSystem->ProcessThought(QuestType, ESageBrainThinkingLayer::Innovative);
    
    // 使用佛家哲學分析
    FSageBrainPhilosophyAnalysis BuddhistAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(QuestType, ESageBrainPhilosophy::Buddhism);
    
    // 使用道家哲學分析
    FSageBrainPhilosophyAnalysis TaoistAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(QuestType, ESageBrainPhilosophy::Taoism);
    
    // 生成哲學任務
    FString Quest = FString::Printf(
        TEXT("哲學任務設計:\n類型: %s\n哲學思考: %s\n創新思考: %s\n佛家分析: %s\n道家分析: %s\n任務目標: %s\n道德考驗: %s"),
        *QuestType,
        *PhilosophicalThought.Content,
        *InnovativeThought.Content,
        *BuddhistAnalysis.Analysis,
        *TaoistAnalysis.Analysis,
        *SageBrainSystem->GenerateSelfReflection(),
        *FString::Join(TEXT(", "), BuddhistAnalysis.KeyPrinciples)
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Designed philosophical quest: %s"), *Quest));
    OnEnhancedContentGenerated.Broadcast(TEXT("Quest"), Quest);
    
    return Quest;
}

bool UMingSageBrainBridge::AutomateAssetGeneration()
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for asset automation"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Automating asset generation..."));

    bool bSuccess = true;

    // 自動生成音樂
    FString MusicResult = GenerateSageBrainMusic(TEXT("明朝背景音樂"), ESageBrainThinkingLayer::Cultural);
    if (MusicResult.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to automate music generation"));
        bSuccess = false;
    }

    // 自動生成場景
    FString SceneResult = GenerateSageBrainScene(TEXT("明朝戰場"), ESageBrainPhilosophy::MilitaryStrategy);
    if (SceneResult.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to automate scene generation"));
        bSuccess = false;
    }

    // 自動生成資產
    FString AssetResult = GenerateSageBrainAssets(TEXT("明朝武器"), ESageBrainLearningAlgorithm::DeepLearning);
    if (AssetResult.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to automate asset generation"));
        bSuccess = false;
    }

    // 優化學習性能
    if (!SageBrainSystem->OptimizeLearningPerformance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to optimize learning performance"));
        bSuccess = false;
    }

    LogBridgeActivity(FString::Printf(TEXT("Asset automation %s"), bSuccess ? TEXT("successful") : TEXT("failed")));
    return bSuccess;
}

bool UMingSageBrainBridge::OptimizeSystemPerformance()
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for performance optimization"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Optimizing system performance..."));

    bool bSuccess = true;

    // 優化學習算法性能
    if (!SageBrainSystem->OptimizeLearningPerformance())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to optimize learning performance"));
        bSuccess = false;
    }

    // 更新意識狀態
    FSageBrainConsciousnessState ConsciousnessState = SageBrainSystem->UpdateConsciousnessState();
    if (ConsciousnessState.Awareness < 0.5f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Consciousness state low after optimization"));
        bSuccess = false;
    }

    // 進行自我評估
    if (!SageBrainSystem->PerformSelfAssessment())
    {
        UE_LOG(LogTemp, Warning, TEXT("Self-assessment failed during optimization"));
        bSuccess = false;
    }

    LogBridgeActivity(FString::Printf(TEXT("System performance optimization %s"), bSuccess ? TEXT("successful") : TEXT("failed")));
    return bSuccess;
}

bool UMingSageBrainBridge::AutoBalanceGameplay()
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for gameplay auto-balance"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Auto-balancing gameplay..."));

    bool bSuccess = true;

    // 分析當前遊戲狀態
    FString GameState = TEXT("當前遊戲平衡狀態分析");
    FString Optimization = OptimizeGameplay(GameState);
    
    if (Optimization.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to generate gameplay optimization"));
        bSuccess = false;
    }

    // 使用兵家哲學進行平衡分析
    FSageBrainPhilosophyAnalysis MilitaryAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
        TEXT("遊戲平衡性"), 
        ESageBrainPhilosophy::MilitaryStrategy
    );
    
    if (MilitaryAnalysis.Relevance < 0.7f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Military philosophy analysis relevance low"));
        bSuccess = false;
    }

    // 使用法家思想進行規則分析
    FSageBrainPhilosophyAnalysis LegalistAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(
        TEXT("遊戲規則"), 
        ESageBrainPhilosophy::Legalism
    );
    
    if (LegalistAnalysis.Relevance < 0.7f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Legalist philosophy analysis relevance low"));
        bSuccess = false;
    }

    LogBridgeActivity(FString::Printf(TEXT("Gameplay auto-balance %s"), bSuccess ? TEXT("successful") : TEXT("failed")));
    return bSuccess;
}

FString UMingSageBrainBridge::AnalyzeGameMetrics(const FString& MetricsData)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for metrics analysis"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Analyzing game metrics..."));

    // 使用戰略思考分析
    FSageBrainThoughtResult StrategicAnalysis = SageBrainSystem->ProcessThought(MetricsData, ESageBrainThinkingLayer::Strategic);
    
    // 使用預測思考分析
    FSageBrainThoughtResult PredictiveAnalysis = SageBrainSystem->ProcessThought(MetricsData, ESageBrainThinkingLayer::Predictive);
    
    // 使用深度學習分析
    FSageBrainLearningResult DeepLearningAnalysis = SageBrainSystem->TrainAlgorithm(
        ESageBrainLearningAlgorithm::DeepLearning,
        MetricsData
    );
    
    // 生成分析報告
    FString Analysis = FString::Printf(
        TEXT("遊戲指標分析報告:\n戰略分析: %s\n預測分析: %s\n深度學習分析: %s\n學習性能: %.2f\n學習準確度: %.2f\n綜合評估: %s"),
        *StrategicAnalysis.Content,
        *PredictiveAnalysis.Content,
        *DeepLearningAnalysis.LearningData,
        DeepLearningAnalysis.Performance,
        DeepLearningAnalysis.Accuracy,
        *SageBrainSystem->GenerateSelfReflection()
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Analyzed game metrics: %s"), *Analysis));
    return Analysis;
}

FString UMingSageBrainBridge::GenerateInsights(const FString& InputData)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for insights generation"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Generating insights..."));

    // 使用深度思考體系
    TArray<FSageBrainThoughtResult> DeepThoughtResults = SageBrainSystem->ProcessDeepThought(InputData);
    
    // 使用集成學習算法
    FSageBrainLearningResult EnsembleLearningResult = SageBrainSystem->TrainAlgorithm(
        ESageBrainLearningAlgorithm::EnsembleLearning,
        InputData
    );
    
    // 生成洞察
    FString Insights = TEXT("深度洞察分析:\n");
    for (const FSageBrainThoughtResult& Result : DeepThoughtResults)
    {
        Insights += FString::Printf(TEXT("[%s] %s (信心度: %.2f)\n"), 
            *UEnum::GetDisplayValueAsText(Result.Layer).ToString(),
            *Result.Content,
            Result.Confidence);
    }
    
    Insights += FString::Printf(TEXT("\n集成學習洞察: %s\n"), *EnsembleLearningResult.LearningData);
    Insights += FString::Printf(TEXT("系統反思: %s\n"), *SageBrainSystem->GenerateSelfReflection());
    Insights += FString::Printf(TEXT("意識狀態: %s"), *UEnum::GetDisplayValueAsText(SageBrainSystem->GetCurrentConsciousnessState()).ToString());
    
    LogBridgeActivity(FString::Printf(TEXT("Generated insights: %s"), *Insights));
    return Insights;
}

FString UMingSageBrainBridge::PredictTrends(const FString& HistoricalData)
{
    if (!ValidateSystems())
    {
        UE_LOG(LogTemp, Error, TEXT("Systems not validated for trend prediction"));
        return TEXT("");
    }

    UE_LOG(LogTemp, Log, TEXT("Predicting trends..."));

    // 使用預測思考層次
    FSageBrainThoughtResult PredictiveThought = SageBrainSystem->ProcessThought(HistoricalData, ESageBrainThinkingLayer::Predictive);
    
    // 使用量子思考層次
    FSageBrainThoughtResult QuantumThought = SageBrainSystem->ProcessThought(HistoricalData, ESageBrainThinkingLayer::Quantum);
    
    // 使用超越思考層次
    FSageBrainThoughtResult TranscendentalThought = SageBrainSystem->ProcessThought(HistoricalData, ESageBrainThinkingLayer::Transcendental);
    
    // 使用量子學習算法
    FSageBrainLearningResult QuantumLearningResult = SageBrainSystem->TrainAlgorithm(
        ESageBrainLearningAlgorithm::QuantumLearning,
        HistoricalData
    );
    
    // 生成趨勢預測
    FString Trends = FString::Printf(
        TEXT("趨勢預測分析:\n預測分析: %s\n量子分析: %s\n超越分析: %s\n量子學習: %s\n預測信心度: %.2f\n量子信心度: %.2f\n超越信心度: %.2f\n學習準確度: %.2f\n綜合預測: %s"),
        *PredictiveThought.Content,
        *QuantumThought.Content,
        *TranscendentalThought.Content,
        *QuantumLearningResult.LearningData,
        PredictiveThought.Confidence,
        QuantumThought.Confidence,
        TranscendentalThought.Confidence,
        QuantumLearningResult.Accuracy,
        *SageBrainSystem->GenerateSelfReflection()
    );
    
    LogBridgeActivity(FString::Printf(TEXT("Predicted trends: %s"), *Trends));
    return Trends;
}

FString UMingSageBrainBridge::GetBridgeStatus() const
{
    if (!bIsInitialized)
    {
        return TEXT("橋樑未初始化");
    }

    FString Status = TEXT("聖者大腦橋樑狀態:\n");
    Status += FString::Printf(TEXT("初始化: 是\n"));
    Status += FString::Printf(TEXT("聖者大腦系統: %s\n"), SageBrainSystem && SageBrainSystem->IsInitialized() ? TEXT("已初始化") : TEXT("未初始化"));
    Status += FString::Printf(TEXT("Python API: %s\n"), PythonAPI ? TEXT("已連接") : TEXT("未連接"));
    Status += FString::Printf(TEXT("當前意識狀態: %s\n"), *UEnum::GetDisplayValueAsText(SageBrainSystem->GetCurrentConsciousnessState()).ToString());
    Status += FString::Printf(TEXT("系統反思: %s"), *SageBrainSystem->GenerateSelfReflection());
    
    return Status;
}

ESageBrainConsciousness UMingSageBrainBridge::GetConsciousnessLevel() const
{
    if (!ValidateSystems())
    {
        return ESageBrainConsciousness::Dormant;
    }

    return SageBrainSystem->GetCurrentConsciousnessState();
}

// 私有輔助函數實現

FString UMingSageBrainBridge::EnhanceWithThinking(const FString& BaseContent, ESageBrainThinkingLayer Layer)
{
    if (!SageBrainSystem)
    {
        return BaseContent;
    }

    FSageBrainThoughtResult ThoughtResult = SageBrainSystem->ProcessThought(BaseContent, Layer);
    return ThoughtResult.Content;
}

FString UMingSageBrainBridge::EnhanceWithPhilosophy(const FString& BaseContent, ESageBrainPhilosophy Philosophy)
{
    if (!SageBrainSystem)
    {
        return BaseContent;
    }

    FSageBrainPhilosophyAnalysis PhilosophyAnalysis = SageBrainSystem->AnalyzeWithPhilosophy(BaseContent, Philosophy);
    return PhilosophyAnalysis.Analysis;
}

FString UMingSageBrainBridge::EnhanceWithLearning(const FString& BaseContent, ESageBrainLearningAlgorithm Algorithm)
{
    if (!SageBrainSystem)
    {
        return BaseContent;
    }

    FSageBrainLearningResult LearningResult = SageBrainSystem->TrainAlgorithm(Algorithm, BaseContent);
    return LearningResult.LearningData;
}

FString UMingSageBrainBridge::EnhanceWithConsciousness(const FString& BaseContent)
{
    if (!SageBrainSystem)
    {
        return BaseContent;
    }

    FString Reflection = SageBrainSystem->GenerateSelfReflection();
    return FString::Printf(TEXT("%s\n意識反思: %s"), *BaseContent, *Reflection);
}

bool UMingSageBrainBridge::ValidateSystems() const
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("Bridge not initialized"));
        return false;
    }

    if (!SageBrainSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("SageBrainSystem is null"));
        return false;
    }

    if (!PythonAPI)
    {
        UE_LOG(LogTemp, Error, TEXT("PythonAPI is null"));
        return false;
    }

    if (!SageBrainSystem->IsInitialized())
    {
        UE_LOG(LogTemp, Error, TEXT("SageBrainSystem not initialized"));
        return false;
    }

    return true;
}

FString UMingSageBrainBridge::GenerateEnhancedPrompt(const FString& BasePrompt, ESageBrainThinkingLayer Layer, ESageBrainPhilosophy Philosophy)
{
    FString EnhancedPrompt = BasePrompt;
    
    // 添加思考層次指示
    EnhancedPrompt += FString::Printf(TEXT("\n\n請從%s的角度進行思考"), *UEnum::GetDisplayValueAsText(Layer).ToString());
    
    // 添加哲學系統指示
    EnhancedPrompt += FString::Printf(TEXT("\n\n請融入%s的哲學思想"), *UEnum::GetDisplayValueAsText(Philosophy).ToString());
    
    // 添加意識狀態指示
    if (SageBrainSystem)
    {
        EnhancedPrompt += FString::Printf(TEXT("\n\n當前意識狀態: %s"), 
            *UEnum::GetDisplayValueAsText(SageBrainSystem->GetCurrentConsciousnessState()).ToString());
    }
    
    return EnhancedPrompt;
}

void UMingSageBrainBridge::LogBridgeActivity(const FString& Activity) const
{
    UE_LOG(LogTemp, Log, TEXT("SageBrainBridge: %s"), *Activity);
}

// ==================== 編譯優化工作流程實作 ====================

bool UMingSageBrainBridge::RunCompileOptimizationWorkflow()
{
    UE_LOG(LogTemp, Log, TEXT("啟動聖者大腦編譯優化工作流程..."));

    if (!SageBrainLauncher)
    {
        UE_LOG(LogTemp, Error, TEXT("聖者大腦啟動器未初始化，請先呼叫 EnableCompileOptimizer()"));
        return false;
    }

    // 綁定啟動器事件
    SageBrainLauncher->OnWorkflowCompleted.AddDynamic(this, &UMingSageBrainBridge::OnLauncherWorkflowCompleted);
    SageBrainLauncher->OnProgressUpdated.AddDynamic(this, &UMingSageBrainBridge::OnLauncherProgressUpdated);

    // 執行工作流程
    bool bSuccess = SageBrainLauncher->RunCompleteWorkflow();

    LogBridgeActivity(FString::Printf(TEXT("編譯優化工作流程 %s"), bSuccess ? TEXT("執行成功") : TEXT("執行失敗")));

    return bSuccess;
}

bool UMingSageBrainBridge::EnableCompileOptimizer()
{
    UE_LOG(LogTemp, Log, TEXT("啟用聖者大腦編譯優化器..."));

    if (SageBrainLauncher)
    {
        UE_LOG(LogTemp, Log, TEXT("編譯優化器已啟用"));
        return true;
    }

    // 創建啟動器
    SageBrainLauncher = NewObject<UMingSageBrainLauncher>(this);
    if (!SageBrainLauncher)
    {
        UE_LOG(LogTemp, Error, TEXT("無法創建聖者大腦啟動器"));
        return false;
    }

    // 啟用聖者大腦系統
    if (!SageBrainLauncher->EnableSageBrain())
    {
        UE_LOG(LogTemp, Error, TEXT("無法啟用聖者大腦系統"));
        SageBrainLauncher = nullptr;
        return false;
    }

    // 預設配置
    SageBrainLauncher->SetBuildMode(TEXT("Development"));
    SageBrainLauncher->SetAutoFix(true);
    SageBrainLauncher->SetCreateTasks(true);

    UE_LOG(LogTemp, Log, TEXT("聖者大腦編譯優化器已啟用 ✓"));
    LogBridgeActivity(TEXT("編譯優化器已啟用"));

    return true;
}

bool UMingSageBrainBridge::IsCompileOptimizerEnabled() const
{
    return SageBrainLauncher != nullptr && SageBrainLauncher->IsSageBrainEnabled();
}

void UMingSageBrainBridge::SetBuildMode(const FString& Mode)
{
    if (SageBrainLauncher)
    {
        SageBrainLauncher->SetBuildMode(Mode);
        UE_LOG(LogTemp, Log, TEXT("設定編譯模式: %s"), *Mode);
    }
}

void UMingSageBrainBridge::SetSkipClean(bool bSkip)
{
    if (SageBrainLauncher)
    {
        SageBrainLauncher->SetSkipClean(bSkip);
        UE_LOG(LogTemp, Log, TEXT("設定跳過清理: %s"), bSkip ? TEXT("是") : TEXT("否"));
    }
}

void UMingSageBrainBridge::SetAutoFix(bool bAuto)
{
    if (SageBrainLauncher)
    {
        SageBrainLauncher->SetAutoFix(bAuto);
        UE_LOG(LogTemp, Log, TEXT("設定自動修復: %s"), bAuto ? TEXT("是") : TEXT("否"));
    }
}

void UMingSageBrainBridge::SetCreateTasks(bool bCreate)
{
    if (SageBrainLauncher)
    {
        SageBrainLauncher->SetCreateTasks(bCreate);
        UE_LOG(LogTemp, Log, TEXT("設定建立任務: %s"), bCreate ? TEXT("是") : TEXT("否"));
    }
}

TArray<FMingTaskRequirement> UMingSageBrainBridge::GetGeneratedTasks() const
{
    if (SageBrainLauncher)
    {
        return SageBrainLauncher->GetSortedTasks();
    }
    return TArray<FMingTaskRequirement>();
}

FCompileOptimizationReport UMingSageBrainBridge::GetLastCompileReport() const
{
    if (SageBrainLauncher)
    {
        return SageBrainLauncher->GetLastReport();
    }
    return FCompileOptimizationReport();
}

void UMingSageBrainBridge::OnLauncherWorkflowCompleted(bool bSuccess, const FCompileOptimizationReport& Report)
{
    UE_LOG(LogTemp, Log, TEXT("編譯優化工作流程完成 - 成功: %s"), bSuccess ? TEXT("是") : TEXT("否"));
    UE_LOG(LogTemp, Log, TEXT("總錯誤: %d, 總警告: %d, 耗時: %.2f 秒"),
        Report.TotalErrors, Report.TotalWarnings, Report.TotalDuration);

    // 廣播事件
    OnCompileOptimizationCompleted.Broadcast(Report);
}

void UMingSageBrainBridge::OnLauncherProgressUpdated(int32 Percentage, const FString& Message)
{
    // 轉發進度更新事件
    OnCompileProgressUpdated.Broadcast(Percentage, Message);
}

// ==================== 自我思考與學習功能實作 ====================

bool UMingSageBrainBridge::EnableSelfThinking(ESelfThinkingMode Mode, float IntervalSeconds)
{
    UE_LOG(LogTemp, Log, TEXT("啟用聖者大腦自我思考功能..."));

    if (!SelfLearningSystem)
    {
        // 創建自我學習系統
        SelfLearningSystem = NewObject<UMingSageBrainSelfLearningSystem>(this);
        if (!SelfLearningSystem)
        {
            UE_LOG(LogTemp, Error, TEXT("無法創建自我學習系統"));
            return false;
        }

        // 初始化自我學習系統
        if (!SelfLearningSystem->InitializeSelfLearningSystem(SageBrainSystem))
        {
            UE_LOG(LogTemp, Error, TEXT("無法初始化自我學習系統"));
            SelfLearningSystem = nullptr;
            return false;
        }
    }

    // 啟動自我思考循環
    bool bSuccess = SelfLearningSystem->StartSelfThinkingLoop(Mode, IntervalSeconds);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("自我思考已啟用 - 模式: %s, 間隔: %.1f秒 ✓"),
            *UEnum::GetDisplayValueAsText(Mode).ToString(), IntervalSeconds);
        LogBridgeActivity(TEXT("自我思考功能已啟用"));
    }

    return bSuccess;
}

void UMingSageBrainBridge::DisableSelfThinking()
{
    if (SelfLearningSystem)
    {
        SelfLearningSystem->StopSelfThinkingLoop();
        UE_LOG(LogTemp, Log, TEXT("自我思考已停用"));
        LogBridgeActivity(TEXT("自我思考功能已停用"));
    }
}

bool UMingSageBrainBridge::IsSelfThinkingActive() const
{
    return SelfLearningSystem && SelfLearningSystem->IsSelfThinkingActive();
}

FSelfThinkingRecord UMingSageBrainBridge::TriggerSelfThinking(const FString& Context, ESelfThinkingMode Mode)
{
    if (!SelfLearningSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("自我學習系統未初始化"));
        return FSelfThinkingRecord();
    }

    FSelfThinkingRecord Record = SelfLearningSystem->PerformSelfThinking(Context, Mode);

    UE_LOG(LogTemp, Log, TEXT("觸發自我思考 - 上下文: %s, 深度: %.2f"), *Context, Record.DepthScore);

    return Record;
}

bool UMingSageBrainBridge::EnableContinuousLearning(float CheckIntervalSeconds)
{
    UE_LOG(LogTemp, Log, TEXT("啟用聖者大腦持續學習功能..."));

    if (!SelfLearningSystem)
    {
        // 創建並初始化自我學習系統
        SelfLearningSystem = NewObject<UMingSageBrainSelfLearningSystem>(this);
        if (!SelfLearningSystem)
        {
            UE_LOG(LogTemp, Error, TEXT("無法創建自我學習系統"));
            return false;
        }

        if (!SelfLearningSystem->InitializeSelfLearningSystem(SageBrainSystem))
        {
            UE_LOG(LogTemp, Error, TEXT("無法初始化自我學習系統"));
            SelfLearningSystem = nullptr;
            return false;
        }
    }

    // 啟動持續學習
    bool bSuccess = SelfLearningSystem->StartContinuousLearning(CheckIntervalSeconds);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("持續學習已啟用 - 檢查間隔: %.1f秒 ✓"), CheckIntervalSeconds);
        LogBridgeActivity(TEXT("持續學習功能已啟用"));
    }

    return bSuccess;
}

void UMingSageBrainBridge::DisableContinuousLearning()
{
    if (SelfLearningSystem)
    {
        SelfLearningSystem->StopContinuousLearning();
        UE_LOG(LogTemp, Log, TEXT("持續學習已停用"));
        LogBridgeActivity(TEXT("持續學習功能已停用"));
    }
}

bool UMingSageBrainBridge::IsContinuousLearningActive() const
{
    return SelfLearningSystem && SelfLearningSystem->IsSelfLearningActive();
}

TArray<FSelfImprovementSuggestion> UMingSageBrainBridge::GetSelfImprovementSuggestions()
{
    if (!SelfLearningSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("自我學習系統未初始化，創建並初始化..."));

        SelfLearningSystem = NewObject<UMingSageBrainSelfLearningSystem>(this);
        if (!SelfLearningSystem || !SelfLearningSystem->InitializeSelfLearningSystem(SageBrainSystem))
        {
            UE_LOG(LogTemp, Error, TEXT("無法初始化自我學習系統"));
            return TArray<FSelfImprovementSuggestion>();
        }
    }

    // 進行自我評估
    TMap<FString, float> Assessment = SelfLearningSystem->PerformSelfAssessment();

    // 生成改進建議
    TArray<FSelfImprovementSuggestion> Suggestions = SelfLearningSystem->GenerateSelfImprovementSuggestions();

    UE_LOG(LogTemp, Log, TEXT("生成 %d 個自我改進建議"), Suggestions.Num());

    return Suggestions;
}

bool UMingSageBrainBridge::ImplementSelfImprovement(const FString& SuggestionID)
{
    if (!SelfLearningSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("自我學習系統未初始化"));
        return false;
    }

    // 評估建議
    SelfLearningSystem->EvaluateImprovementSuggestion(SuggestionID);

    // 實施建議
    bool bSuccess = SelfLearningSystem->ImplementImprovementSuggestion(SuggestionID);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("改進建議 %s 已實施"), *SuggestionID);
        LogBridgeActivity(FString::Printf(TEXT("實施改進建議: %s"), *SuggestionID));
    }

    return bSuccess;
}

FString UMingSageBrainBridge::GetSelfLearningStatus() const
{
    if (!SelfLearningSystem)
    {
        return TEXT("自我學習系統未初始化");
    }

    return SelfLearningSystem->GetSelfLearningStatus();
}

// ==================== 至聖者指揮系統整合 ====================

bool UMingSageBrainBridge::InitializeSupremeSageSystem()
{
    UE_LOG(LogTemp, Log, TEXT("初始化至聖者指揮系統..."));

    if (!SupremeSageSystem)
    {
        SupremeSageSystem = NewObject<UMingSupremeSageCommandSystem>(this);
        if (!SupremeSageSystem)
        {
            UE_LOG(LogTemp, Error, TEXT("無法創建至聖者指揮系統"));
            return false;
        }
    }

    if (!SupremeSageSystem->InitializeSystem())
    {
        UE_LOG(LogTemp, Error, TEXT("無法初始化至聖者指揮系統"));
        SupremeSageSystem = nullptr;
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("✓ 至聖者指揮系統初始化完成"));
    UE_LOG(LogTemp, Log, TEXT("  - 三權模型：道權/策權/兵權"));
    UE_LOG(LogTemp, Log, TEXT("  - 五行輪轉：木火土金水"));
    UE_LOG(LogTemp, Log, TEXT("  - 六伐策略：伐心/伐氣/伐勢/伐命/伐影/伐無"));
    UE_LOG(LogTemp, Log, TEXT("  - 防墮機制：三大徵象監測"));

    LogBridgeActivity(TEXT("至聖者指揮系統已初始化"));

    return true;
}

bool UMingSageBrainBridge::EnterFiveElementPhase(EFiveElementPhase Phase, const FPhaseContext& Context)
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return false;
    }

    bool bSuccess = SupremeSageSystem->EnterPhase(Phase, Context);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("進入%s階段: %s"),
            *UEnum::GetDisplayValueAsText(Phase).ToString(),
            *Context.PhaseName);
        LogBridgeActivity(FString::Printf(TEXT("進入%s階段"), *UEnum::GetDisplayValueAsText(Phase).ToString()));
    }

    return bSuccess;
}

bool UMingSageBrainBridge::ExecuteSixConquestStrategy(ESixConquestStrategy Strategy, EStrategyApproach Approach, const FConquestTarget& Target)
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return false;
    }

    FConquestResult Result = SupremeSageSystem->ExecuteSixConquest(Strategy, Approach, Target);

    UE_LOG(LogTemp, Log, TEXT("執行%s: %s - %s"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString(),
        *UEnum::GetDisplayValueAsText(Approach).ToString(),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"));

    LogBridgeActivity(FString::Printf(TEXT("執行%s"), *UEnum::GetDisplayValueAsText(Strategy).ToString()));

    return Result.bSuccess;
}

bool UMingSageBrainBridge::ExecuteTwelveStrategy(ETwelveStrategies Strategy, const FStrategyContext& Context)
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return false;
    }

    FStrategyResult Result = SupremeSageSystem->ExecuteTwelveStrategy(Strategy, Context);

    UE_LOG(LogTemp, Log, TEXT("執行十二策『%s』: %s"),
        *UEnum::GetDisplayValueAsText(Strategy).ToString(),
        Result.bSuccess ? TEXT("成功") : TEXT("失敗"));

    LogBridgeActivity(FString::Printf(TEXT("執行十二策: %s"), *UEnum::GetDisplayValueAsText(Strategy).ToString()));

    return Result.bSuccess;
}

FCorruptionCheckResult UMingSageBrainBridge::PerformCorruptionCheck()
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return FCorruptionCheckResult();
    }

    FCorruptionCheckResult Result = SupremeSageSystem->PerformCorruptionCheck();

    UE_LOG(LogTemp, Log, TEXT("墮落檢查結果:"));
    UE_LOG(LogTemp, Log, TEXT("  整體風險: %.2f"), Result.OverallCorruptionRisk);
    UE_LOG(LogTemp, Log, TEXT("  需要交接: %s"), Result.bShouldTransferCommand ? TEXT("是") : TEXT("否"));

    if (Result.OverallCorruptionRisk > 0.5f)
    {
        UE_LOG(LogTemp, Warning, TEXT("⚠ 檢測到墮落風險，建議執行防墮措施"));
    }

    return Result;
}

FCommandRecommendation UMingSageBrainBridge::GetSageCommandRecommendation() const
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return FCommandRecommendation();
    }

    FCommandRecommendation Recommendation = SupremeSageSystem->GetCommandRecommendation();

    UE_LOG(LogTemp, Log, TEXT("至聖者指揮建議:"));
    UE_LOG(LogTemp, Log, TEXT("  推薦權限: %s"), *UEnum::GetDisplayValueAsText(Recommendation.RecommendedAuthority).ToString());
    UE_LOG(LogTemp, Log, TEXT("  推薦階段: %s"), *UEnum::GetDisplayValueAsText(Recommendation.RecommendedPhase).ToString());
    UE_LOG(LogTemp, Log, TEXT("  信心指數: %.2f"), Recommendation.ConfidenceLevel);

    return Recommendation;
}

bool UMingSageBrainBridge::SwitchAuthority(ESupremeAuthorityType AuthorityType)
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return false;
    }

    bool bSuccess = SupremeSageSystem->SwitchToAuthority(AuthorityType);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("切換至%s主導"), *UEnum::GetDisplayValueAsText(AuthorityType).ToString());
        LogBridgeActivity(FString::Printf(TEXT("切換至%s"), *UEnum::GetDisplayValueAsText(AuthorityType).ToString()));
    }

    return bSuccess;
}

bool UMingSageBrainBridge::DeployForce(EForceType ForceType, const FDeploymentConfig& Config)
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return false;
    }

    bool bSuccess = SupremeSageSystem->DeployForce(ForceType, Config);

    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("部署%s: %s"),
            *UEnum::GetDisplayValueAsText(ForceType).ToString(),
            *Config.DeploymentName);
        LogBridgeActivity(FString::Printf(TEXT("部署%s"), *UEnum::GetDisplayValueAsText(ForceType).ToString()));
    }

    return bSuccess;
}

FSelfAuditReport UMingSageBrainBridge::GenerateSageSelfAuditReport()
{
    if (!SupremeSageSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("至聖者指揮系統未初始化"));
        return FSelfAuditReport();
    }

    FSelfAuditReport Report = SupremeSageSystem->GenerateSelfAuditReport();

    UE_LOG(LogTemp, Log, TEXT("=== 至聖者自我審核報告 ==="));
    UE_LOG(LogTemp, Log, TEXT("總命令數: %d"), Report.TotalCommandsIssued);
    UE_LOG(LogTemp, Log, TEXT("邪兵使用: %d"), Report.EvilForceUsages);
    UE_LOG(LogTemp, Log, TEXT("平均決策品質: %.2f"), Report.AverageDecisionQuality);
    UE_LOG(LogTemp, Log, TEXT("墮落風險: %.2f"), Report.CorruptionStatus.OverallCorruptionRisk);

    if (!Report.Recommendations.IsEmpty())
    {
        UE_LOG(LogTemp, Log, TEXT("改進建議:\n%s"), *Report.Recommendations);
    }

    LogBridgeActivity(TEXT("生成至聖者自我審核報告"));

    return Report;
}
