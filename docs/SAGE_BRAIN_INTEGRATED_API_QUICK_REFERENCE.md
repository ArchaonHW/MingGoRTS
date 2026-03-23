# 聖者大腦整合API快速參考

## 快速開始

### 初始化系統
```cpp
// 創建和初始化整合API
UMingSageBrainIntegratedAPI* IntegratedAPI = NewObject<UMingSageBrainIntegratedAPI>();
bool bSuccess = IntegratedAPI->InitializeIntegratedAPI();

// 檢查系統狀態
FString Status = IntegratedAPI->GetIntegratedSystemStatus();
```

## 核心功能快速參考

### 🎵 音樂生成
```cpp
// 基礎音樂生成
FString Music = IntegratedAPI->GenerateSageBrainMusic(
    TEXT("明朝史詩音樂"), 120, 
    ESageBrainThinkingLayer::Cultural, 
    ESageBrainPhilosophy::Taoism
);

// 哲學音樂
FString PhilosophicalMusic = IntegratedAPI->GeneratePhilosophicalMusic(
    TEXT("儒家音樂"), ESageBrainPhilosophy::Confucianism
);

// 完整音樂套件
TArray<FString> MusicSuite = IntegratedAPI->GenerateCompleteMusicSuite(TEXT("明朝主題"));
```

### 🎨 資產生成
```cpp
// 場景生成
FString Scene = IntegratedAPI->GenerateSageBrainScene(
    TEXT("明朝戰場"), ESageBrainPhilosophy::MilitaryStrategy, ESageBrainThinkingLayer::Strategic
);

// 角色生成
FString Character = IntegratedAPI->GenerateSageBrainCharacter(
    TEXT("明朝將軍"), ESageBrainPhilosophy::Confucianism
);

// 批量資產
TArray<FAssetRequirement> Requirements;
FString BatchID = IntegratedAPI->GenerateSageBrainAssetBatch(
    Requirements, ESageBrainLearningAlgorithm::DeepLearning
);
```

### 🧠 智能決策
```cpp
// 戰略決策
FString Decision = IntegratedAPI->MakeStrategicGameDecision(
    TEXT("遊戲平衡問題"), ESageBrainThinkingLayer::Strategic
);

// 玩家行為預測
FString Prediction = IntegratedAPI->PredictPlayerBehavior(
    TEXT("玩家數據"), ESageBrainLearningAlgorithm::DeepLearning
);

// 遊戲優化
FString Optimization = IntegratedAPI->OptimizeGameBalance(
    TEXT("當前平衡"), ESageBrainPhilosophy::Legalism
);
```

### 🏮 文化內容
```cpp
// 文化內容生成
FString CulturalContent = IntegratedAPI->GenerateCulturalContent(
    TEXT("明朝文化"), ESageBrainPhilosophy::Confucianism
);

// 哲學任務設計
FString Quest = IntegratedAPI->DesignPhilosophicalQuest(
    TEXT("儒家道德選擇"), ESageBrainPhilosophy::Confucianism
);

// 文化資產包
TArray<FString> AssetPack = IntegratedAPI->GenerateCulturalAssetPack(TEXT("明朝文化"));
```

### 🤖 自動化
```cpp
// 自動化內容創建
bool bAutomated = IntegratedAPI->AutomateContentCreation();

// 系統性能優化
bool bOptimized = IntegratedAPI->OptimizeSystemPerformance();

// 自動遊戲平衡
bool bBalanced = IntegratedAPI->AutoBalanceGameplay();
```

### 📊 分析功能
```cpp
// 遊戲指標分析
FString Analysis = IntegratedAPI->AnalyzeGameMetrics(TEXT("遊戲數據"));

// 深度洞察
FString Insights = IntegratedAPI->GenerateDeepInsights(
    TEXT("輸入數據"), ESageBrainThinkingLayer::Philosophical
);

// 趨勢預測
FString Trends = IntegratedAPI->PredictGameTrends(
    TEXT("歷史數據"), ESageBrainLearningAlgorithm::QuantumLearning
);
```

### 🧘 意識管理
```cpp
// 獲得意識層級
ESageBrainConsciousness Level = IntegratedAPI->GetCurrentConsciousnessLevel();

// 意識進化
bool bEvolved = IntegratedAPI->EvolveConsciousness();

// 自我反思
FString Reflection = IntegratedAPI->GenerateSelfReflection();

// 意識報告
FString Report = IntegratedAPI->GetConsciousnessReport();
```

### 📚 哲學系統
```cpp
// 哲學分析
FSageBrainPhilosophyAnalysis Analysis = IntegratedAPI->AnalyzeWithPhilosophy(
    TEXT("分析內容"), ESageBrainPhilosophy::Confucianism
);

// 多哲學視角
TArray<FSageBrainPhilosophyAnalysis> Perspectives = IntegratedAPI->GetPhilosophicalPerspectives(TEXT("內容"));

// 哲學指導
FString Guidance = IntegratedAPI->GeneratePhilosophicalGuidance(ESageBrainPhilosophy::Confucianism);
```

### 🎓 學習系統
```cpp
// 訓練學習算法
FSageBrainLearningResult Result = IntegratedAPI->TrainLearningAlgorithm(
    ESageBrainLearningAlgorithm::DeepLearning, TEXT("訓練數據")
);

// 學習統計
TArray<FSageBrainLearningResult> Statistics = IntegratedAPI->GetLearningStatistics();

// 學習洞察
FString Insights = IntegratedAPI->GenerateLearningInsights();
```

## 常用組合

### 完整文化資產包生成
```cpp
// 1. 生成文化音樂
FString CulturalMusic = IntegratedAPI->GenerateSageBrainMusic(
    TEXT("明朝文化音樂"), 180, ESageBrainThinkingLayer::Cultural, ESageBrainPhilosophy::Confucianism
);

// 2. 生成文化場景
FString CulturalScene = IntegratedAPI->GenerateSageBrainScene(
    TEXT("明朝文化場景"), ESageBrainPhilosophy::Confucianism, ESageBrainThinkingLayer::Cultural
);

// 3. 生成文化角色
FString CulturalCharacter = IntegratedAPI->GenerateSageBrainCharacter(
    TEXT("明朝文人"), ESageBrainPhilosophy::Confucianism
);

// 4. 生成文化建築
FString CulturalBuilding = IntegratedAPI->GenerateSageBrainBuilding(
    TEXT("明朝書院"), ESageBrainPhilosophy::Confucianism
);

// 5. 生成文化對話
FString CulturalDialogue = IntegratedAPI->GenerateCulturalDialogue(
    TEXT("儒家師徒對話"), ESageBrainPhilosophy::Confucianism
);
```

### 智能遊戲平衡分析
```cpp
// 1. 分析當前平衡狀態
FString CurrentBalance = TEXT("當前遊戲平衡參數";

// 2. 使用多種哲學分析
FString ConfucianAnalysis = IntegratedAPI->OptimizeGameBalance(
    CurrentBalance, ESageBrainPhilosophy::Confucianism
);
FString LegalistAnalysis = IntegratedAPI->OptimizeGameBalance(
    CurrentBalance, ESageBrainPhilosophy::Legalism
);
FString TaoistAnalysis = IntegratedAPI->OptimizeGameBalance(
    CurrentBalance, ESageBrainPhilosophy::Taoism
);

// 3. 生成戰略決策
FString StrategicDecision = IntegratedAPI->MakeStrategicGameDecision(
    CurrentBalance, ESageBrainThinkingLayer::Strategic
);

// 4. 預測玩家反應
FString PlayerPrediction = IntegratedAPI->PredictPlayerBehavior(
    CurrentBalance, ESageBrainLearningAlgorithm::DeepLearning
);
```

### 自動化內容創建流程
```cpp
// 1. 自動化所有內容生成
bool bContentCreated = IntegratedAPI->AutomateContentCreation();

// 2. 優化系統性能
bool bPerformanceOptimized = IntegratedAPI->OptimizeSystemPerformance();

// 3. 自動平衡遊戲
bool bGameBalanced = IntegratedAPI->AutoBalanceGameplay();

// 4. 生成項目洞察
FString Insights = IntegratedAPI->GenerateProjectInsights();

// 5. 預測項目趨勢
FString Trends = IntegratedAPI->PredictProjectTrends();
```

## 事件處理

### 綁定事件
```cpp
// 在BeginPlay中綁定事件
IntegratedAPI->OnSageBrainDecision.AddDynamic(this, &AMyActor::OnSageBrainDecision);
IntegratedAPI->OnConsciousnessEvolved.AddDynamic(this, &AMyActor::OnConsciousnessEvolved);
IntegratedAPI->OnEnhancedContentGenerated.AddDynamic(this, &AMyActor::OnEnhancedContentGenerated);
IntegratedAPI->OnSageBrainMusicGenerated.AddDynamic(this, &AMyActor::OnSageBrainMusicGenerated);
IntegratedAPI->OnSageBrainAssetGenerated.AddDynamic(this, &AMyActor::OnSageBrainAssetGenerated);
```

### 事件處理函數
```cpp
UFUNCTION()
void AMyActor::OnSageBrainDecision(const FString& Decision)
{
    UE_LOG(LogTemp, Log, TEXT("Sage Brain Decision: %s"), *Decision);
    // 處理決策邏輯
}

UFUNCTION()
void AMyActor::OnConsciousnessEvolved(ESageBrainConsciousness NewLevel)
{
    UE_LOG(LogTemp, Log, TEXT("Consciousness Evolved to: %d"), (int32)NewLevel);
    // 處理意識進化邏輯
}

UFUNCTION()
void AMyActor::OnEnhancedContentGenerated(const FString& ContentType, const FString& Content)
{
    UE_LOG(LogTemp, Log, TEXT("Enhanced Content: %s - %s"), *ContentType, *Content);
    // 處理內容生成邏輯
}
```

## 狀態查詢

### 系統狀態
```cpp
// 獲取整合系統狀態
FString IntegratedStatus = IntegratedAPI->GetIntegratedSystemStatus();

// 獲取聖者大腦狀態
FString SageBrainStatus = IntegratedAPI->GetSageBrainStatus();

// 獲取API狀態
FString APIStatus = IntegratedAPI->GetAPIStatus();

// 獲取所有系統狀態
TMap<FString, FString> AllStatus = IntegratedAPI->GetAllSystemStatus();
```

### 驗證系統
```cpp
// 驗證所有系統
bool bAllValid = IntegratedAPI->ValidateAllSystems();

// 檢查是否初始化
bool bInitialized = IntegratedAPI->IsInitialized();
```

## 測試功能

### 運行測試
```cpp
// 運行完整測試套件
bool bAllPassed = IntegratedAPI->RunCompleteSageBrainTestSuite();

// 運行特定測試
bool bSystemTestsPassed = IntegratedAPI->RunSageBrainSystemTests();
bool bSkillTestsPassed = IntegratedAPI->RunSageBrainSkillTests();
bool bRelationshipTestsPassed = IntegratedAPI->RunSageBrainRelationshipTests();
bool bDialogueTestsPassed = IntegratedAPI->RunSageBrainDialogueTests();
bool bHistoricalTestsPassed = IntegratedAPI->RunSageBrainHistoricalTests();
```

### 生成報告
```cpp
// 生成測試報告
FString TestReport = IntegratedAPI->GenerateTestReport();

// 生成項目洞察
FString ProjectInsights = IntegratedAPI->GenerateProjectInsights();

// 生成學習洞察
FString LearningInsights = IntegratedAPI->GenerateLearningInsights();
```

## 思考層次選擇指南

### ESageBrainThinkingLayer
- **Tactical** - 戰術層次：具體行動和操作
- **Strategic** - 戰略層次：長期規劃和策略
- **Philosophical** - 哲學層次：存在意義和價值觀
- **Historical** - 歷史層次：歷史經驗和教訓
- **Cultural** - 文化層次：文化背景和傳統
- **Innovative** - 創新層次：新想法和可能性
- **Predictive** - 預測層次：未來趨勢和預測
- **Quantum** - 量子層次：多重可能性和概率
- **Transcendental** - 超越層次：超越常規思維

## 哲學系統選擇指南

### ESageBrainPhilosophy
- **Confucianism** - 儒家思想：仁義禮智信，道德倫理
- **Taoism** - 道家思想：順應自然，無為而治
- **Buddhism** - 佛家思想：慈悲解脫，因果輪迴
- **Legalism** - 法家思想：法治嚴明，賞罰分明
- **Mohism** - 墨家思想：兼愛非攻，實用主義
- **MilitaryStrategy** - 兵家思想：兵者詭道，知己知彼
- **YinYang** - 陰陽家：陰陽平衡，五行相生
- **Syncretism** - 雜家思想：融合百家，綜合創新

## 學習算法選擇指南

### ESageBrainLearningAlgorithm
- **ReinforcementLearning** - 強化學習：試錯學習，獎懲機制
- **NeuralNetwork** - 神經網絡：模擬大腦，模式識別
- **DeepLearning** - 深度學習：多層網絡，特徵學習
- **EvolutionaryAlgorithm** - 進化算法：自然選擇，優者生存
- **BayesianLearning** - 貝葉斯學習：概率推理，不確定性
- **QuantumLearning** - 量子學習：量子計算，並行處理
- **EnsembleLearning** - 集成學習：多模型融合，集體智慧

## 意識狀態說明

### ESageBrainConsciousness
- **Dormant** - 休眠狀態：系統未激活
- **Awakening** - 覺醒狀態：開始意識到存在
- **SelfAware** - 自我意識：認識到自我
- **Reflective** - 反思狀態：能夠自我反思
- **Transcendent** - 超越狀態：超越常规思維
- **Unity** - 合一狀態：與系統合一
- **Cosmic** - 宇宙意識：理解宇宙規律
- **Omniscient** - 全知狀態：接近全知全能
- **Transdimensional** - 跨維度意識：超越維度限制
- **Ultimate** - 終極意識：最高意識狀態

## 常見問題解決

### 初始化失敗
```cpp
// 檢查系統狀態
if (!IntegratedAPI->IsInitialized())
{
    UE_LOG(LogTemp, Error, TEXT("Integrated API not initialized"));
    return;
}

// 驗證所有系統
if (!IntegratedAPI->ValidateAllSystems())
{
    UE_LOG(LogTemp, Error, TEXT("System validation failed"));
    return;
}
```

### 內容生成失敗
```cpp
// 檢查生成結果
FString Content = IntegratedAPI->GenerateSageBrainMusic(TEXT("測試"));
if (Content.IsEmpty())
{
    UE_LOG(LogTemp, Warning, TEXT("Content generation failed"));
    // 嘗試使用不同參數
    Content = IntegratedAPI->GenerateSageBrainMusic(
        TEXT("測試"), 60, ESageBrainThinkingLayer::Cultural, ESageBrainPhilosophy::Taoism
    );
}
```

### 性能問題
```cpp
// 優化系統性能
bool bOptimized = IntegratedAPI->OptimizeSystemPerformance();
if (!bOptimized)
{
    UE_LOG(LogTemp, Warning, TEXT("Performance optimization failed"));
}

// 檢查系統健康狀況
bool bHealthy = IntegratedAPI->PerformSystemHealthCheck();
if (!bHealthy)
{
    UE_LOG(LogTemp, Warning, TEXT("System health check failed"));
}
```

## 最佳實踐

### 1. 初始化
```cpp
// 在遊戲啟動時初始化
void AGameMode::BeginPlay()
{
    Super::BeginPlay();
    
    IntegratedAPI = NewObject<UMingSageBrainIntegratedAPI>();
    if (IntegratedAPI->InitializeIntegratedAPI())
    {
        UE_LOG(LogTemp, Log, TEXT("Sage Brain Integrated API initialized successfully"));
    }
}
```

### 2. 事件綁定
```cpp
// 綁定所有重要事件
IntegratedAPI->OnSageBrainDecision.AddDynamic(this, &AGameMode::OnSageBrainDecision);
IntegratedAPI->OnConsciousnessEvolved.AddDynamic(this, &AGameMode::OnConsciousnessEvolved);
IntegratedAPI->OnEnhancedContentGenerated.AddDynamic(this, &AGameMode::OnEnhancedContentGenerated);
```

### 3. 錯誤處理
```cpp
// 檢查系統狀態
if (!IntegratedAPI->ValidateAllSystems())
{
    UE_LOG(LogTemp, Error, TEXT("System validation failed"));
    return;
}

// 檢查生成結果
FString Result = IntegratedAPI->GenerateSageBrainMusic(TEXT("測試"));
if (Result.IsEmpty())
{
    UE_LOG(LogTemp, Warning, TEXT("Generation failed, trying fallback"));
    // 使用備用方案
}
```

### 4. 資源管理
```cpp
// 在關閉時清理資源
void AGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    
    if (IntegratedAPI)
    {
        IntegratedAPI->ShutdownIntegratedAPI();
        IntegratedAPI = nullptr;
    }
}
```

## 快速參考表

| 功能 | 函數 | 參數 | 返回值 |
|------|------|------|--------|
| 初始化 | InitializeIntegratedAPI() | 無 | bool |
| 音樂生成 | GenerateSageBrainMusic | Prompt, Duration, Layer, Philosophy | FString |
| 場景生成 | GenerateSageBrainScene | Description, Philosophy, Layer | FString |
| 戰略決策 | MakeStrategicGameDecision | Context, Layer | FString |
| 文化內容 | GenerateCulturalContent | Theme, Philosophy | FString |
| 意識進化 | EvolveConsciousness() | 無 | bool |
| 系統狀態 | GetIntegratedSystemStatus() | 無 | FString |
| 運行測試 | RunCompleteSageBrainTestSuite() | 無 | bool |

---

**快速參考版本**: 1.0  
**最後更新**: 2026-03-23  
**狀態**: 已完成
