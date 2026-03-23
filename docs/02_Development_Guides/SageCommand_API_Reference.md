# 至聖者指揮學 API 參考文檔

**版本**: v0.9.0-alpha  
**最後更新**: 2026-03-24  
**適用系統**: MingGoRTS - 民國傭兵RTS

---

## 📚 目錄

1. [系統概覽](#系統概覽)
2. [核心系統API](#核心系統api)
3. [策略系統API](#策略系統api)
4. [防墮機制API](#防墮機制api)
5. [AI適配API](#ai適配api)
6. [集成測試API](#集成測試api)
7. [事件系統](#事件系統)
8. [常見問題](#常見問題)

---

## 🎯 系統概覽

### 系統架構

```
MingSageCharacterSystem
    ├── MingThreePowerSystem (三權系統)
    │   ├── MingMoralAuthority (道權 - 道德監督)
    │   ├── MingStrategyAuthority (策權 - 策略管理)
    │   └── MingMilitaryAuthority (兵權 - 軍事執行)
    ├── MingSixStrategiesSystem (六策系統)
    │   └── MingWuXingRhythmSystem (五行節奏)
    ├── MingAntiCorruptionSystem (防墮機制)
    └── MingAIAdaptationSystem (AI適配)
```

### 核心概念

- **三權指揮**: 道權(40%)、策權(35%)、兵權(25%)
- **正逆六策**: 立國/立制/立人(正)，破局/破結構/不破人(逆)
- **五行節奏**: 木火土金水五階段動態循環
- **防墮機制**: 三大墮落徵象檢測與預防
- **聖者不墮**: 道德邊界管理與自我審計

---

## 🔧 核心系統API

### MingThreePowerSystem

#### 初始化和配置

```cpp
// 創建並初始化系統
UMingThreePowerSystem* ThreePowerSystem = NewObject<UMingThreePowerSystem>();
bool bSuccess = ThreePowerSystem->InitializeThreePowerSystem();

// 檢查系統就緒狀態
bool bReady = ThreePowerSystem->IsSystemReady();
```

#### 權力分配

```cpp
// 獲取當前權力分配
TMap<EPowerType, float> Distribution = ThreePowerSystem->GetCurrentPowerDistribution();

// 重新分配權力
TMap<EPowerType, float> NewDistribution;
NewDistribution.Add(EPowerType::Moral, 0.45f);
NewDistribution.Add(EPowerType::Strategy, 0.35f);
NewDistribution.Add(EPowerType::Military, 0.20f);

bool bDistributed = ThreePowerSystem->DistributePowers(NewDistribution, TEXT("重新配置原因"));

// 轉移特定權力
bool bTransferred = ThreePowerSystem->TransferPower(
    EPowerType::Strategy,    // 從策權
    EPowerType::Military,      // 轉移到兵權
    0.05f,                   // 轉移5%
    TEXT("戰時需求")
);
```

#### 事件監聽

```cpp
// 監聽權力轉移事件
ThreePowerSystem->OnPowerTransferred.AddLambda([](EPowerType From, EPowerType To, float Amount) {
    UE_LOG(LogTemp, Log, TEXT("權力轉移: %d -> %d, 數量: %.2f"), (int32)From, (int32)To, Amount);
});

// 監聽權力平衡事件
ThreePowerSystem->OnBalanceTriggered.AddLambda([](float ImbalanceLevel) {
    UE_LOG(LogTemp, Warning, TEXT("權力失衡檢測，等級: %.2f"), ImbalanceLevel);
});
```

### MingMoralAuthority

#### 墮落徵象檢測

```cpp
// 檢測墮落徵象
bool bDetected = MoralAuthority->DetectCorruptionSign(ECorruptionSign::RationalizationOfEvil);

// 獲取當前徵象列表
TArray<ECorruptionSign> CurrentSigns = MoralAuthority->GetCurrentCorruptionSigns();

// 獲取徵象詳細信息
FCorruptionSignInfo SignInfo = MoralAuthority->GetCorruptionSignInfo(ECorruptionSign::RationalizationOfEvil);
```

#### 道德邊界管理

```cpp
// 檢查道德邊界狀態
EMoralBoundaryStatus BoundaryStatus = MoralAuthority->CheckMoralBoundaryStatus();

// 獲取道德價值
float MoralValue = MoralAuthority->GetCurrentMoralValue();

// 更新道德價值
MoralAuthority->UpdateMoralValue(DeltaValue);
```

---

## ⚔️ 策略系統API

### MingSixStrategiesSystem

#### 策略執行

```cpp
// 創建策略執行上下文
FStrategyExecutionContext Context;
Context.Direction = EStrategyDirection::Righteous;  // 正策
Context.CurrentPhase = EFiveElementPhase::Wood;     // 木階段
Context.EffectIntensity = 1.0f;
Context.InitiatorID = TEXT("Player_001");

// 執行策略
bool bSuccess = StrategySystem->ExecuteStrategy(ESixStrategyType::EstablishNation, Context);

// 停止策略執行
bool bStopped = StrategySystem->StopStrategyExecution(ESixStrategyType::EstablishNation);

// 獲取策略執行狀態
EStrategyExecutionStatus Status = StrategySystem->GetStrategyExecutionStatus(ESixStrategyType::EstablishNation);
```

#### 策略效果管理

```cpp
// 獲取當前效果列表
TArray<FStrategyEffectDetails> CurrentEffects = StrategySystem->GetCurrentEffects();

// 計算效果強度
float EffectIntensity = StrategySystem->CalculateEffectIntensity(
    EStrategyEffectType::MoraleBoost, 
    EFiveElementPhase::Wood
);
```

#### 策略組合

```cpp
// 創建策略組合
TArray<ESixStrategyType> Strategies;
Strategies.Add(ESixStrategyType::EstablishNation);
Strategies.Add(ESixStrategyType::EstablishPeople);

FStrategyCombination Combination = StrategySystem->CreateStrategyCombination(Strategies);

// 激活策略組合
bool bActivated = StrategySystem->ActivateStrategyCombination(Combination);

// 計算協效加成
float SynergyBonus = StrategySystem->CalculateSynergyBonus(Strategies);
```

### MingWuXingRhythmSystem

#### 節奏控制

```cpp
// 初始化系統
WuXingSystem->InitializeWuXingRhythmSystem();

// 啟動五行節奏
bool bStarted = WuXingSystem->StartWuXingRhythm(EWuXingRhythmPattern::Cyclic);

// 停止節奏
bool bStopped = WuXingSystem->StopWuXingRhythm();

// 暫停和恢復
WuXingSystem->PauseWuXingRhythm();
WuXingSystem->ResumeWuXingRhythm();
```

#### 節奏配置

```cpp
// 設置節奏模式
WuXingSystem->SetRhythmPattern(EWuXingRhythmPattern::Strategic, TEXT("戰略調整"));

// 設置節奏強度
WuXingSystem->SetRhythmIntensity(EWuXingRhythmIntensity::Strong);

// 設置五行階段序列
TArray<EFiveElementPhase> Sequence;
Sequence.Add(EFiveElementPhase::Wood);
Sequence.Add(EFiveElementPhase::Fire);
Sequence.Add(EFiveElementPhase::Earth);
Sequence.Add(EFiveElementPhase::Metal);
Sequence.Add(EFiveElementPhase::Water);

WuXingSystem->SetPhaseSequence(Sequence);
```

#### 階段信息

```cpp
// 獲取當前階段
EFiveElementPhase CurrentPhase = WuXingSystem->GetCurrentPhase();

// 獲取階段進度 (0.0 - 1.0)
float PhaseProgress = WuXingSystem->GetPhaseProgress();

// 獲取階段剩餘時間
float RemainingTime = WuXingSystem->GetPhaseRemainingTime();

// 獲取下一階段
EFiveElementPhase NextPhase = WuXingSystem->GetNextPhase();
```

#### 統計和數據

```cpp
// 獲取當前節奏數據
FWuXingRhythmData RhythmData = WuXingSystem->GetCurrentRhythmData();

// 獲取節奏統計
FWuXingRhythmStatistics Statistics = WuXingSystem->GetRhythmStatistics();

// 獲取推薦的下一階段
EFiveElementPhase RecommendedPhase = WuXingSystem->GetRecommendedWuXingPhase(ESixStrategyType::EstablishNation);
```

---

## 🛡️ 防墮機制API

### MingAntiCorruptionSystem

#### 徵象檢測

```cpp
// 檢測墮落徵象
bool bDetected = AntiCorruptionSystem->DetectCorruptionPhenomenon(
    ECorruptionPhenomenon::RationalizationOfEvil,  // 徵象類型
    0.5f,                                         // 嚴重程度 (0.0-1.0)
    TEXT("證據描述")                              // 證據
);

// 獲取當前徵象列表
TArray<FCorruptionPhenomenon> Phenomena = AntiCorruptionSystem->GetCurrentCorruptionPhenomena();

// 檢查特定徵象是否存在
bool bHasPhenomenon = AntiCorruptionSystem->HasCorruptionPhenomenon(ECorruptionPhenomenon::FollowerDeification);

// 清除徵象
bool bCleared = AntiCorruptionSystem->ClearCorruptionPhenomenon(ECorruptionPhenomenon::RationalizationOfEvil);
```

#### 警告系統

```cpp
// 發布防墮警告
int32 WarningID = AntiCorruptionSystem->IssueAntiCorruptionWarning(
    ECorruptionPhenomenon::RationalizationOfEvil,
    ECorruptionSeverity::Moderate
);

// 獲取當前警告列表
TArray<FAntiCorruptionWarning> Warnings = AntiCorruptionSystem->GetCurrentWarnings();

// 解除警告
bool bResolved = AntiCorruptionSystem->ResolveWarning(WarningID);

// 獲取警告詳情
FAntiCorruptionWarning WarningDetails = AntiCorruptionSystem->GetWarningDetails(WarningID);
```

#### 自我審計

```cpp
// 啟動自我審計
int32 AuditID = AntiCorruptionSystem->StartSelfAudit();

// 執行審計檢查
bool bChecked = AntiCorruptionSystem->PerformAuditCheck(AuditID, ECorruptionPhenomenon::RationalizationOfEvil);

// 完成審計
FSelfAuditData AuditData = AntiCorruptionSystem->CompleteSelfAudit(AuditID);

// 獲取審計歷史
TArray<FSelfAuditData> AuditHistory = AntiCorruptionSystem->GetAuditHistory();

// 設置定期審計
bool bScheduled = AntiCorruptionSystem->ScheduleRegularAudit(300.0f); // 每5分鐘
```

#### 道德邊界

```cpp
// 檢查道德邊界狀態
FMoralBoundaryStatus Status = AntiCorruptionSystem->CheckMoralBoundaryStatus();

// 更新道德完整性
bool bUpdated = AntiCorruptionSystem->UpdateMoralIntegrity(10.0f); // 增加10%

// 強化道德抵抗
bool bStrengthened = AntiCorruptionSystem->StrengthenMoralResistance(15.0f);

// 設置道德邊界參數
AntiCorruptionSystem->SetMoralBoundaryParameters(70.0f, 30.0f); // 警告閾值70%, 危急閾值30%
```

#### 防墮措施

```cpp
// 應用防墮措施
bool bApplied = AntiCorruptionSystem->ApplyAntiCorruptionMeasure(EAntiCorruptionMeasure::SelfReflection);

// 獲取措施效果
FAntiCorruptionMeasureEffect MeasureEffect = AntiCorruptionSystem->GetMeasureEffect(EAntiCorruptionMeasure::SelfReflection);

// 獲取推薦措施
TArray<EAntiCorruptionMeasure> Recommendations = AntiCorruptionSystem->GetRecommendedMeasures(ECorruptionPhenomenon::RationalizationOfEvil);

// 計算措施效果
float Effectiveness = AntiCorruptionSystem->CalculateMeasureEffectiveness(
    EAntiCorruptionMeasure::SelfReflection, 
    ECorruptionPhenomenon::RationalizationOfEvil
);
```

---

## 🤖 AI適配API

### MingAIAdaptationSystem

#### 對手分析

```cpp
// 識別對手類型
FAICommanderProfile Profile = AIAdaptationSystem->IdentifyOpponent(TEXT("Player_001"));

// 分析對手行為
FPlayerBehaviorAnalysis Analysis = AIAdaptationSystem->AnalyzeOpponentBehavior(TEXT("Player_001"));

// 獲取對手檔案
FAICommanderProfile ExistingProfile = AIAdaptationSystem->GetOpponentProfile(TEXT("Player_001"));

// 更新對手檔案
bool bUpdated = AIAdaptationSystem->UpdateOpponentProfile(TEXT("Player_001"), NewProfile);

// 檢測對手弱點
TArray<EStrategyEffectType> Weaknesses = AIAdaptationSystem->DetectOpponentWeaknesses(TEXT("Player_001"));
```

#### 玩家行為學習

```cpp
// 記錄玩家行為
bool bRecorded = AIAdaptationSystem->RecordPlayerBehavior(
    TEXT("Player_001"),           // 玩家ID
    ESixStrategyType::EstablishNation, // 使用的策略
    0.8f                          // 結果評分 (0.0-1.0)
);

// 分析玩家模式
FPlayerBehaviorAnalysis Pattern = AIAdaptationSystem->AnalyzePlayerPattern(TEXT("Player_001"));

// 預測玩家下一步
ESixStrategyType PredictedMove = AIAdaptationSystem->PredictPlayerNextMove(TEXT("Player_001"));

// 獲取學習數據
FAdaptationLearningData LearningData = AIAdaptationSystem->GetLearningData(TEXT("Player_001"));
```

#### 適配決策生成

```cpp
// 生成適配決策
FAIAdaptationDecision Decision = AIAdaptationSystem->GenerateAdaptationDecision(
    TEXT("Player_001"),                    // 對手ID
    ESixStrategyType::EstablishNation      // 對手的策略
);

// 選擇最佳反制策略
ESixStrategyType CounterStrategy = AIAdaptationSystem->SelectBestCounterStrategy(
    TEXT("Player_001"), 
    ESixStrategyType::EstablishNation
);

// 評估決策效果
float Effectiveness = AIAdaptationSystem->EvaluateDecisionEffectiveness(Decision);

// 生成決策理由
TArray<FString> Reasoning = AIAdaptationSystem->GenerateDecisionReasoning(Decision);
```

#### 威脅評估

```cpp
// 評估威脅等級
FThreatAssessment Threat = AIAdaptationSystem->AssessThreat(
    TEXT("Player_001"), 
    ESixStrategyType::BreakSituation
);

// 獲取當前威脅列表
TArray<FThreatAssessment> CurrentThreats = AIAdaptationSystem->GetCurrentThreats();

// 獲取最嚴重威脅
FThreatAssessment CriticalThreat = AIAdaptationSystem->GetMostCriticalThreat();

// 生成緩解策略
TArray<FString> Mitigations = AIAdaptationSystem->GenerateMitigationStrategies(Threat);
```

#### AI行為控制

```cpp
// 設置AI決策模式
bool bSet = AIAdaptationSystem->SetAIDecisionMode(EAIDecisionMode::Aggressive);

// 獲取當前決策模式
EAIDecisionMode CurrentMode = AIAdaptationSystem->GetCurrentDecisionMode();

// 執行AI決策
bool bExecuted = AIAdaptationSystem->ExecuteAIDecision(Decision);

// 切換AI類型
bool bSwitched = AIAdaptationSystem->SwitchAIType(EAIOpponentType::AggressiveCommander, TEXT("適應對手侵略性"));

// 獲取AI建議
TArray<FAIAdaptationDecision> Recommendations = AIAdaptationSystem->GetAIRecommendations(3);
```

#### 學習優化

```cpp
// 更新學習數據
bool bUpdated = AIAdaptationSystem->UpdateLearningData(
    TEXT("Player_001"), 
    EAIAdaptationResponse::CounterStrategy, 
    0.85f  // 效果評分
);

// 優化適配策略
bool bOptimized = AIAdaptationSystem->OptimizeAdaptationStrategy(TEXT("Player_001"));

// 清除學習數據
AIAdaptationSystem->ClearLearningData(TEXT("Player_001"));

// 導出學習數據
FString ExportedData = AIAdaptationSystem->ExportLearningData(TEXT("Player_001"));
```

---

## 🧪 集成測試API

### MingSageCommandIntegrationTest

#### 測試管理

```cpp
// 創建測試案例
int32 TestID = IntegrationTest->CreateTestCase(
    TEXT("測試名稱"),
    EIntegrationTestType::ThreePowerIntegration,
    ETestSeverity::Critical
);

// 獲取測試案例
FIntegrationTestCase TestCase = IntegrationTest->GetTestCase(TestID);

// 獲取所有測試案例
TArray<FIntegrationTestCase> AllCases = IntegrationTest->GetAllTestCases();

// 刪除測試案例
bool bRemoved = IntegrationTest->RemoveTestCase(TestID);
```

#### 測試執行

```cpp
// 運行單個測試
FIntegrationTestResultData Result = IntegrationTest->RunSingleTest(TestID);

// 運行測試套件
TArray<int32> TestIDs;
TestIDs.Add(1);
TestIDs.Add(2);
TestIDs.Add(3);
TArray<FIntegrationTestResultData> Results = IntegrationTest->RunTestSuite(TestIDs);

// 運行所有測試
TArray<FIntegrationTestResultData> AllResults = IntegrationTest->RunAllTests();

// 運行特定類型的測試
TArray<FIntegrationTestResultData> TypeResults = IntegrationTest->RunTestsByType(
    EIntegrationTestType::ThreePowerIntegration
);

// 停止/暫停/恢復測試
IntegrationTest->StopCurrentTest();
IntegrationTest->PauseTest();
IntegrationTest->ResumeTest();
```

#### 特定集成測試

```cpp
// 測試三權系統集成
FIntegrationTestResultData ThreePowerResult = IntegrationTest->TestThreePowerIntegration();

// 測試策略執行流程
FIntegrationTestResultData StrategyResult = IntegrationTest->TestStrategyExecutionFlow();

// 測試五行節奏協調
FIntegrationTestResultData WuXingResult = IntegrationTest->TestWuXingRhythmCoordination();

// 測試防墮機制集成
FIntegrationTestResultData AntiCorruptionResult = IntegrationTest->TestAntiCorruptionIntegration();

// 測試AI適配集成
FIntegrationTestResultData AIResult = IntegrationTest->TestAIAdaptationIntegration();

// 測試跨權限通信
FIntegrationTestResultData CommunicationResult = IntegrationTest->TestCrossAuthorityCommunication();

// 測試全系統集成
FIntegrationTestResultData FullIntegrationResult = IntegrationTest->TestFullSystemIntegration();
```

#### 性能測試

```cpp
// 運行性能壓力測試
FPerformanceTestResult PerformanceResult = IntegrationTest->RunPerformanceStressTest(60.0f);

// 測試並發操作
FPerformanceTestResult ConcurrentResult = IntegrationTest->TestConcurrentOperations(100);

// 測試內存使用
float MemoryUsage = IntegrationTest->TestMemoryUsage();

// 測試CPU使用率
float CPUUsage = IntegrationTest->TestCPUUsage(10.0f);

// 生成性能報告
FString PerformanceReport = IntegrationTest->GeneratePerformanceReport();
```

#### 系統狀態檢查

```cpp
// 檢查系統集成狀態
FSystemIntegrationStatus Status = IntegrationTest->CheckSystemIntegrationStatus();

// 驗證系統準備就緒
TArray<FString> RequiredSystems;
RequiredSystems.Add(TEXT("ThreePowerSystem"));
RequiredSystems.Add(TEXT("SixStrategiesSystem"));
bool bReady = IntegrationTest->VerifySystemReadiness(RequiredSystems);

// 獲取系統健康評分
float HealthScore = IntegrationTest->GetSystemHealthScore();

// 診斷系統問題
TArray<FString> Issues = IntegrationTest->DiagnoseSystemIssues();
```

#### 結果分析

```cpp
// 分析測試結果
FString Analysis = IntegrationTest->AnalyzeTestResults(Results);

// 生成測試報告
FString Report = IntegrationTest->GenerateTestReport();

// 導出測試結果
bool bExported = IntegrationTest->ExportTestResults(TEXT("F:/TestResults.json"));

// 獲取測試統計
TMap<EIntegrationTestResult, int32> Statistics = IntegrationTest->GetTestStatistics();
```

---

## 📡 事件系統

### 主要事件委託

#### 三權系統事件

```cpp
// 權力轉移事件
ThreePowerSystem->OnPowerTransferred.AddDynamic(this, &UMyClass::OnPowerTransferred);

// 權力平衡事件
ThreePowerSystem->OnBalanceTriggered.AddDynamic(this, &UMyClass::OnBalanceTriggered);

// 系統就緒事件
ThreePowerSystem->OnSystemReady.AddDynamic(this, &UMyClass::OnSystemReady);
```

#### 策略系統事件

```cpp
// 策略執行開始事件
StrategySystem->OnStrategyExecutionStarted.AddDynamic(this, &UMyClass::OnStrategyStarted);

// 策略執行完成事件
StrategySystem->OnStrategyExecutionCompleted.AddDynamic(this, &UMyClass::OnStrategyCompleted);

// 策略效果應用事件
StrategySystem->OnStrategyEffectApplied.AddDynamic(this, &UMyClass::OnEffectApplied);

// 策略組合激活事件
StrategySystem->OnStrategyCombinationActivated.AddDynamic(this, &UMyClass::OnCombinationActivated);
```

#### 五行節奏事件

```cpp
// 階段開始事件
WuXingSystem->OnWuXingPhaseStarted.AddDynamic(this, &UMyClass::OnPhaseStarted);

// 階段完成事件
WuXingSystem->OnWuXingPhaseCompleted.AddDynamic(this, &UMyClass::OnPhaseCompleted);

// 節奏狀態改變事件
WuXingSystem->OnWuXingRhythmStateChanged.AddDynamic(this, &UMyClass::OnRhythmStateChanged);

// 節奏模式改變事件
WuXingSystem->OnWuXingRhythmPatternChanged.AddDynamic(this, &UMyClass::OnPatternChanged);

// 節奏事件觸發事件
WuXingSystem->OnWuXingRhythmEventTriggered.AddDynamic(this, &UMyClass::OnRhythmEvent);
```

#### 防墮機制事件

```cpp
// 墮落徵象檢測事件
AntiCorruptionSystem->OnCorruptionPhenomenonDetected.AddDynamic(this, &UMyClass::OnPhenomenonDetected);

// 防墮警告發布事件
AntiCorruptionSystem->OnAntiCorruptionWarningIssued.AddDynamic(this, &UMyClass::OnWarningIssued);

// 自我審計完成事件
AntiCorruptionSystem->OnSelfAuditCompleted.AddDynamic(this, &UMyClass::OnAuditCompleted);

// 道德邊界跨越事件
AntiCorruptionSystem->OnMoralBoundaryCrossed.AddDynamic(this, &UMyClass::OnBoundaryCrossed);

// 防墮措施應用事件
AntiCorruptionSystem->OnAntiCorruptionMeasureApplied.AddDynamic(this, &UMyClass::OnMeasureApplied);
```

#### AI適配事件

```cpp
// 對手識別事件
AIAdaptationSystem->OnOpponentIdentified.AddDynamic(this, &UMyClass::OnOpponentIdentified);

// 適配決策事件
AIAdaptationSystem->OnAdaptationDecisionMade.AddDynamic(this, &UMyClass::OnDecisionMade);

// 威脅檢測事件
AIAdaptationSystem->OnThreatDetected.AddDynamic(this, &UMyClass::OnThreatDetected);

// 策略反制事件
AIAdaptationSystem->OnStrategyCountered.AddDynamic(this, &UMyClass::OnStrategyCountered);

// 學習更新事件
AIAdaptationSystem->OnLearningUpdated.AddDynamic(this, &UMyClass::OnLearningUpdated);
```

#### 集成測試事件

```cpp
// 測試開始事件
IntegrationTest->OnIntegrationTestStarted.AddDynamic(this, &UMyClass::OnTestStarted);

// 測試完成事件
IntegrationTest->OnIntegrationTestCompleted.AddDynamic(this, &UMyClass::OnTestCompleted);

// 測試套件完成事件
IntegrationTest->OnIntegrationTestSuiteCompleted.AddDynamic(this, &UMyClass::OnSuiteCompleted);

// 系統集成狀態改變事件
IntegrationTest->OnSystemIntegrationStatusChanged.AddDynamic(this, &UMyClass::OnStatusChanged);
```

---

## ❓ 常見問題

### Q1: 如何正確初始化所有系統？

```cpp
// 推薦的初始化順序
void InitializeAllSystems()
{
    // 1. 三權系統（基礎架構）
    ThreePowerSystem->InitializeThreePowerSystem();
    
    // 2. 策略系統
    SixStrategiesSystem->InitializeSixStrategiesSystem();
    
    // 3. 五行節奏
    WuXingSystem->InitializeWuXingRhythmSystem();
    
    // 4. 防墮和AI系統
    AntiCorruptionSystem->InitializeAntiCorruptionSystem();
    AIAdaptationSystem->InitializeAIAdaptationSystem();
}
```

### Q2: 如何避免策略衝突？

```cpp
// 檢查策略是否衝突
bool bCanExecute = SixStrategiesSystem->ValidateStrategyCombination(ProposedStrategies);

// 獲取活動策略
TArray<ESixStrategyType> ActiveStrategies = SixStrategiesSystem->GetActiveStrategies();

// 確保不超過最大並發數（默認3個）
if (ActiveStrategies.Num() < 3)
{
    // 可以執行新策略
}
```

### Q3: 如何正確處理道德邊界警告？

```cpp
// 監聽道德邊界事件
AntiCorruptionSystem->OnMoralBoundaryCrossed.AddLambda([](float NewIntegrity, float PreviousIntegrity) {
    if (NewIntegrity < 70.0f)
    {
        // 道德完整性低於警告閾值
        UE_LOG(LogTemp, Warning, TEXT("道德邊界警告！當前完整性: %.1f%%"), NewIntegrity);
        
        // 應用防墮措施
        AntiCorruptionSystem->ApplyAntiCorruptionMeasure(EAntiCorruptionMeasure::SelfReflection);
    }
});
```

### Q4: 如何優化性能？

```cpp
// 1. 限制並發策略數量
static const int32 MAX_CONCURRENT_STRATEGIES = 3;

// 2. 定期清理過期數據
AntiCorruptionSystem->ClearOldData(3600.0f); // 清理1小時前的數據

// 3. 使用合適的更新頻率
PrimaryComponentTick.TickInterval = 0.1f; // 10fps更新頻率

// 4. 運行性能測試監控
FPerformanceTestResult Result = IntegrationTest->RunPerformanceStressTest();
if (!Result.bPerformanceAcceptable)
{
    // 性能不達標，需要優化
}
```

### Q5: 如何自定義AI行為？

```cpp
// 創建自定義AI檔案
FAICommanderProfile CustomAI;
CustomAI.OpponentType = EAIOpponentType::SageCommander;
CustomAI.Aggressiveness = 0.3f;
CustomAI.Defensiveness = 0.7f;
CustomAI.Adaptability = 0.8f;
CustomAI.PreferredStrategies.Add(ESixStrategyType::EstablishNation);
CustomAI.PreferredStrategies.Add(ESixStrategyType::EstablishSystem);

// 更新AI檔案
AIAdaptationSystem->UpdateOpponentProfile(TEXT("CustomAI_001"), CustomAI);

// 設置決策模式
AIAdaptationSystem->SetAIDecisionMode(EAIDecisionMode::Defensive);
```

---

## 📞 技術支持

- **專案文檔**: `f:\MingGoRTS\docs\`
- **示例代碼**: `f:\MingGoRTS\Source\MingGoRTS\Examples\`
- **測試報告**: `f:\MingGoRTS\Reports\`
- **開發指南**: `f:\MingGoRTS\docs\02_Development_Guides\`

---

**版權所有**: MingGoRTS Project Team  
**許可證**: MIT License  
**最後更新**: 2026-03-24
