/**
 * 至聖者指揮學系統使用示例
 * 
 * 本文件展示了如何使用至聖者指揮學系統的各個組件。
 * 包括三權系統、策略系統、防墮機制、AI適配等。
 */

#include "SageCommand/MingSageCommandIntegrationTest.h"
#include "SageCommand/MingThreePowerSystem.h"
#include "SageCommand/MingMoralAuthority.h"
#include "SageCommand/MingStrategyAuthority.h"
#include "SageCommand/MingMilitaryAuthority.h"
#include "SageCommand/MingSixStrategiesSystem.h"
#include "SageCommand/MingWuXingRhythmSystem.h"
#include "SageCommand/MingAntiCorruptionSystem.h"
#include "SageCommand/MingAIAdaptationSystem.h"

// ============================================================================
// 示例1: 基礎系統初始化和設置
// ============================================================================

void Example_BasicSystemSetup(UWorld* World)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例1: 基礎系統初始化和設置 ==="));
    
    // 1. 創建三權系統
    UMingThreePowerSystem* ThreePowerSystem = NewObject<UMingThreePowerSystem>();
    ThreePowerSystem->InitializeThreePowerSystem();
    
    // 2. 創建六策系統
    UMingSixStrategiesSystem* SixStrategiesSystem = NewObject<UMingSixStrategiesSystem>();
    SixStrategiesSystem->InitializeSixStrategiesSystem();
    
    // 3. 創建五行節奏系統
    UMingWuXingRhythmSystem* WuXingSystem = NewObject<UMingWuXingRhythmSystem>();
    WuXingSystem->InitializeWuXingRhythmSystem();
    
    // 4. 創建防墮系統
    UMingAntiCorruptionSystem* AntiCorruptionSystem = NewObject<UMingAntiCorruptionSystem>();
    AntiCorruptionSystem->InitializeAntiCorruptionSystem();
    
    // 5. 創建AI適配系統
    UMingAIAdaptationSystem* AIAdaptationSystem = NewObject<UMingAIAdaptationSystem>();
    AIAdaptationSystem->InitializeAIAdaptationSystem();
    
    UE_LOG(LogTemp, Log, TEXT("✓ 所有基礎系統初始化完成"));
}

// ============================================================================
// 示例2: 三權系統的使用
// ============================================================================

void Example_ThreePowerSystemUsage(UMingThreePowerSystem* ThreePowerSystem)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例2: 三權系統的使用 ==="));
    
    if (!ThreePowerSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("三權系統未初始化"));
        return;
    }
    
    // 1. 獲取當前權力分配
    TMap<EPowerType, float> CurrentDistribution = ThreePowerSystem->GetCurrentPowerDistribution();
    UE_LOG(LogTemp, Log, TEXT("當前權力分配:"));
    UE_LOG(LogTemp, Log, TEXT("  道權(Moral): %.1f%%"), CurrentDistribution[EPowerType::Moral] * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("  策權(Strategy): %.1f%%"), CurrentDistribution[EPowerType::Strategy] * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("  兵權(Military): %.1f%%"), CurrentDistribution[EPowerType::Military] * 100.0f);
    
    // 2. 重新分配權力（例：聖者模式 - 道權為主）
    TMap<EPowerType, float> SageDistribution;
    SageDistribution.Add(EPowerType::Moral, 0.45f);     // 45% 道權
    SageDistribution.Add(EPowerType::Strategy, 0.35f); // 35% 策權
    SageDistribution.Add(EPowerType::Military, 0.20f);  // 20% 兵權
    
    bool bSuccess = ThreePowerSystem->DistributePowers(SageDistribution, TEXT("聖者模式配置"));
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 權力重新分配成功（聖者模式）"));
    }
    
    // 3. 監聽權力轉移事件
    ThreePowerSystem->OnPowerTransferred.AddLambda([](EPowerType From, EPowerType To, float Amount) {
        UE_LOG(LogTemp, Log, TEXT("權力轉移: %d -> %d, 數量: %.2f"), (int32)From, (int32)To, Amount);
    });
    
    // 4. 轉移部分權力
    ThreePowerSystem->TransferPower(EPowerType::Strategy, EPowerType::Military, 0.05f, TEXT("戰時需求"));
}

// ============================================================================
// 示例3: 正逆六策的執行
// ============================================================================

void Example_StrategyExecution(UMingSixStrategiesSystem* StrategySystem, UMingWuXingRhythmSystem* WuXingSystem)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例3: 正逆六策的執行 ==="));
    
    if (!StrategySystem || !WuXingSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("系統未初始化"));
        return;
    }
    
    // 1. 啟動五行節奏
    WuXingSystem->StartWuXingRhythm(EWuXingRhythmPattern::Cyclic);
    EFiveElementPhase CurrentPhase = WuXingSystem->GetCurrentPhase();
    UE_LOG(LogTemp, Log, TEXT("當前五行階段: %d"), (int32)CurrentPhase);
    
    // 2. 執行正策 - 立國（在木階段效果最佳）
    FStrategyExecutionContext EstablishNationContext;
    EstablishNationContext.Direction = EStrategyDirection::Righteous;
    EstablishNationContext.CurrentPhase = CurrentPhase;
    EstablishNationContext.EffectIntensity = 1.0f;
    EstablishNationContext.InitiatorID = TEXT("Player_001");
    
    bool bSuccess = StrategySystem->ExecuteStrategy(ESixStrategyType::EstablishNation, EstablishNationContext);
    if (bSuccess)
    {
        UE_LOG(LogTemp, Log, TEXT("✓ 正策[立國]執行成功"));
        UE_LOG(LogTemp, Log, TEXT("  效果提升: 士氣+30%%, 徵兵+20%%"));
    }
    
    // 3. 執行正策 - 立制
    FStrategyExecutionContext EstablishSystemContext;
    EstablishSystemContext.Direction = EStrategyDirection::Righteous;
    EstablishSystemContext.CurrentPhase = CurrentPhase;
    
    StrategySystem->ExecuteStrategy(ESixStrategyType::EstablishSystem, EstablishSystemContext);
    UE_LOG(LogTemp, Log, TEXT("✓ 正策[立制]執行成功"));
    UE_LOG(LogTemp, Log, TEXT("  效果提升: 經濟效率+25%%"));
    
    // 4. 切換到逆策（需要陰陽切換）
    // 注意：從正策切換到逆策會影響道德價值
    FStrategyExecutionContext BreakSituationContext;
    BreakSituationContext.Direction = EStrategyDirection::Reverse;
    BreakSituationContext.CurrentPhase = CurrentPhase;
    
    StrategySystem->ExecuteStrategy(ESixStrategyType::BreakSituation, BreakSituationContext);
    UE_LOG(LogTemp, Log, TEXT("✓ 逆策[破局]執行成功"));
    UE_LOG(LogTemp, Log, TEXT("  警告: 信譽下降，道德價值-20"));
    
    // 5. 獲取當前活動策略
    TArray<ESixStrategyType> ActiveStrategies = StrategySystem->GetActiveStrategies();
    UE_LOG(LogTemp, Log, TEXT("當前活動策略數量: %d"), ActiveStrategies.Num());
}

// ============================================================================
// 示例4: 防墮機制的使用
// ============================================================================

void Example_AntiCorruptionSystem(UMingAntiCorruptionSystem* AntiCorruptionSystem)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例4: 防墮機制的使用 ==="));
    
    if (!AntiCorruptionSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("防墮系統未初始化"));
        return;
    }
    
    // 1. 檢查當前道德邊界狀態
    FMoralBoundaryStatus Status = AntiCorruptionSystem->CheckMoralBoundaryStatus();
    UE_LOG(LogTemp, Log, TEXT("當前道德狀態:"));
    UE_LOG(LogTemp, Log, TEXT("  道德完整性: %.1f%%"), Status.MoralIntegrity);
    UE_LOG(LogTemp, Log, TEXT("  腐敗抵抗力: %.1f%%"), Status.CorruptionResistance);
    UE_LOG(LogTemp, Log, TEXT("  風險等級: %.1f%%"), Status.RiskLevel);
    
    // 2. 檢測墮落徵象（例：合理化邪術）
    // 當使用過多逆策時，可能會觸發此徵象
    AntiCorruptionSystem->DetectCorruptionPhenomenon(
        ECorruptionPhenomenon::RationalizationOfEvil, 
        0.4f, // 嚴重程度
        TEXT("連續使用逆策，試圖合理化非常手段")
    );
    
    // 3. 檢查是否有警告
    TArray<FAntiCorruptionWarning> Warnings = AntiCorruptionSystem->GetCurrentWarnings();
    if (Warnings.Num() > 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("檢測到 %d 個防墮警告!"), Warnings.Num());
        for (const auto& Warning : Warnings)
        {
            UE_LOG(LogTemp, Warning, TEXT("  [%d] %s"), Warning.WarningID, *Warning.WarningMessage);
        }
    }
    
    // 4. 應用防墮措施
    AntiCorruptionSystem->ApplyAntiCorruptionMeasure(EAntiCorruptionMeasure::SelfReflection);
    UE_LOG(LogTemp, Log, TEXT("✓ 應用防墮措施: 自我反思"));
    
    // 5. 啟動自我審計
    int32 AuditID = AntiCorruptionSystem->StartSelfAudit();
    UE_LOG(LogTemp, Log, TEXT("✓ 啟動自我審計 ID:%d"), AuditID);
    
    // 6. 完成審計並獲取報告
    FSelfAuditData AuditData = AntiCorruptionSystem->CompleteSelfAudit(AuditID);
    UE_LOG(LogTemp, Log, TEXT("審計報告: %s"), *AuditData.AuditReport);
    
    // 7. 獲取道德建議
    TArray<FString> Recommendations = AntiCorruptionSystem->GetMoralRecommendations();
    UE_LOG(LogTemp, Log, TEXT("道德建議:"));
    for (const auto& Rec : Recommendations)
    {
        UE_LOG(LogTemp, Log, TEXT("  - %s"), *Rec);
    }
}

// ============================================================================
// 示例5: AI適配系統的使用
// ============================================================================

void Example_AIAdaptationSystem(UMingAIAdaptationSystem* AIAdaptationSystem)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例5: AI適配系統的使用 ==="));
    
    if (!AIAdaptationSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("AI適配系統未初始化"));
        return;
    }
    
    // 1. 識別對手
    FString OpponentID = TEXT("Player_Opponent_001");
    FAICommanderProfile OpponentProfile = AIAdaptationSystem->IdentifyOpponent(OpponentID);
    UE_LOG(LogTemp, Log, TEXT("識別對手: %s"), *OpponentProfile.CommanderName);
    UE_LOG(LogTemp, Log, TEXT("  類型: %d"), (int32)OpponentProfile.OpponentType);
    UE_LOG(LogTemp, Log, TEXT("  侵略性: %.2f"), OpponentProfile.Aggressiveness);
    
    // 2. 分析對手行為
    FPlayerBehaviorAnalysis BehaviorAnalysis = AIAdaptationSystem->AnalyzeOpponentBehavior(OpponentID);
    UE_LOG(LogTemp, Log, TEXT("對手行為分析:"));
    UE_LOG(LogTemp, Log, TEXT("  侵略水平: %.2f"), BehaviorAnalysis.AggressionLevel);
    UE_LOG(LogTemp, Log, TEXT("  策略多樣性: %.2f"), BehaviorAnalysis.StrategyVariety);
    UE_LOG(LogTemp, Log, TEXT("  陰陽平衡: %.2f"), BehaviorAnalysis.YinYangBalance);
    
    // 3. 預測對手下一步
    ESixStrategyType PredictedStrategy = AIAdaptationSystem->PredictPlayerNextMove(OpponentID);
    if (PredictedStrategy != ESixStrategyType::None)
    {
        UE_LOG(LogTemp, Log, TEXT("預測對手策略: %d"), (int32)PredictedStrategy);
    }
    
    // 4. 生成適配決策
    FAIAdaptationDecision Decision = AIAdaptationSystem->GenerateAdaptationDecision(OpponentID, PredictedStrategy);
    UE_LOG(LogTemp, Log, TEXT("AI決策:"));
    UE_LOG(LogTemp, Log, TEXT("  響應類型: %d"), (int32)Decision.ResponseType);
    UE_LOG(LogTemp, Log, TEXT("  推薦策略: %d"), (int32)Decision.RecommendedCounterStrategy);
    UE_LOG(LogTemp, Log, TEXT("  置信度: %.2f"), Decision.Confidence);
    UE_LOG(LogTemp, Log, TEXT("  預期效果: %.2f"), Decision.ExpectedEffectiveness);
    
    // 5. 評估威脅
    FThreatAssessment Threat = AIAdaptationSystem->AssessThreat(OpponentID, PredictedStrategy);
    UE_LOG(LogTemp, Log, TEXT("威脅評估:"));
    UE_LOG(LogTemp, Log, TEXT("  威脅等級: %d"), (int32)Threat.ThreatLevel);
    UE_LOG(LogTemp, Log, TEXT("  威脅強度: %.2f"), Threat.ThreatMagnitude);
    
    if (Threat.VulnerablePoints.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("  發現脆弱點:"));
        for (const auto& Point : Threat.VulnerablePoints)
        {
            UE_LOG(LogTemp, Log, TEXT("    - %s"), *Point);
        }
    }
    
    // 6. 執行AI決策
    AIAdaptationSystem->ExecuteAIDecision(Decision);
    UE_LOG(LogTemp, Log, TEXT("✓ AI決策已執行"));
    
    // 7. 記錄學習數據
    AIAdaptationSystem->RecordPlayerBehavior(OpponentID, PredictedStrategy, 0.8f);
    AIAdaptationSystem->UpdateLearningData(OpponentID, Decision.ResponseType, Decision.ExpectedEffectiveness);
}

// ============================================================================
// 示例6: 五行節奏的使用
// ============================================================================

void Example_WuXingRhythmSystem(UMingWuXingRhythmSystem* WuXingSystem)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例6: 五行節奏的使用 ==="));
    
    if (!WuXingSystem)
    {
        UE_LOG(LogTemp, Error, TEXT("五行節奏系統未初始化"));
        return;
    }
    
    // 1. 設置五行序列
    TArray<EFiveElementPhase> CustomSequence;
    CustomSequence.Add(EFiveElementPhase::Wood);
    CustomSequence.Add(EFiveElementPhase::Fire);
    CustomSequence.Add(EFiveElementPhase::Earth);
    CustomSequence.Add(EFiveElementPhase::Metal);
    CustomSequence.Add(EFiveElementPhase::Water);
    
    WuXingSystem->SetPhaseSequence(CustomSequence);
    UE_LOG(LogTemp, Log, TEXT("✓ 設置五行序列: 木->火->土->金->水"));
    
    // 2. 啟動節奏（戰略模式）
    WuXingSystem->StartWuXingRhythm(EWuXingRhythmPattern::Strategic);
    WuXingSystem->SetRhythmIntensity(EWuXingRhythmIntensity::Strong);
    UE_LOG(LogTemp, Log, TEXT("✓ 啟動五行節奏（戰略模式，強烈強度）"));
    
    // 3. 監聽節奏事件
    WuXingSystem->OnWuXingPhaseStarted.AddLambda([](EFiveElementPhase NewPhase, const FWuXingRhythmData& RhythmData) {
        UE_LOG(LogTemp, Log, TEXT("新階段開始: %d, 強度: %.2f"), (int32)NewPhase, RhythmData.RhythmStrength);
    });
    
    // 4. 獲取當前節奏數據
    FWuXingRhythmData RhythmData = WuXingSystem->GetCurrentRhythmData();
    UE_LOG(LogTemp, Log, TEXT("當前節奏狀態:"));
    UE_LOG(LogTemp, Log, TEXT("  階段: %d"), (int32)RhythmData.CurrentPhase);
    UE_LOG(LogTemp, Log, TEXT("  進度: %.1f%%"), RhythmData.PhaseProgress * 100.0f);
    UE_LOG(LogTemp, Log, TEXT("  強度: %.2f"), RhythmData.RhythmStrength);
    
    // 5. 獲取節奏統計
    FWuXingRhythmStatistics Statistics = WuXingSystem->GetRhythmStatistics();
    UE_LOG(LogTemp, Log, TEXT("節奏統計:"));
    UE_LOG(LogTemp, Log, TEXT("  完成循環數: %d"), Statistics.TotalCycles);
    UE_LOG(LogTemp, Log, TEXT("  平均循環時間: %.1f秒"), Statistics.AverageCycleTime);
    UE_LOG(LogTemp, Log, TEXT("  節奏效率: %.1f%%"), Statistics.RhythmEfficiency * 100.0f);
}

// ============================================================================
// 示例7: 集成測試的使用
// ============================================================================

void Example_IntegrationTesting(UMingSageCommandIntegrationTest* IntegrationTest)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例7: 集成測試的使用 ==="));
    
    if (!IntegrationTest)
    {
        UE_LOG(LogTemp, Error, TEXT("集成測試系統未初始化"));
        return;
    }
    
    // 1. 初始化測試系統
    IntegrationTest->InitializeIntegrationTestSystem();
    
    // 2. 檢查系統集成狀態
    FSystemIntegrationStatus Status = IntegrationTest->CheckSystemIntegrationStatus();
    UE_LOG(LogTemp, Log, TEXT("系統集成狀態:"));
    UE_LOG(LogTemp, Log, TEXT("  總體評分: %.1f%%"), Status.OverallIntegrationScore);
    UE_LOG(LogTemp, Log, TEXT("  三權系統: %s"), Status.bThreePowerSystemReady ? TEXT("✓") : TEXT("✗"));
    UE_LOG(LogTemp, Log, TEXT("  策略系統: %s"), Status.bStrategySystemReady ? TEXT("✓") : TEXT("✗"));
    UE_LOG(LogTemp, Log, TEXT("  五行系統: %s"), Status.bWuXingSystemReady ? TEXT("✓") : TEXT("✗"));
    UE_LOG(LogTemp, Log, TEXT("  防墮系統: %s"), Status.bAntiCorruptionSystemReady ? TEXT("✓") : TEXT("✗"));
    UE_LOG(LogTemp, Log, TEXT("  AI適配: %s"), Status.bAIAdaptationSystemReady ? TEXT("✓") : TEXT("✗"));
    
    // 3. 運行特定類型的測試
    UE_LOG(LogTemp, Log, TEXT("\n運行三權系統集成測試..."));
    FIntegrationTestResultData ThreePowerResult = IntegrationTest->TestThreePowerIntegration();
    UE_LOG(LogTemp, Log, TEXT("結果: %s (%.2f秒)"), 
           ThreePowerResult.Result == EIntegrationTestResult::Passed ? TEXT("✓ 通過") : TEXT("✗ 失敗"),
           ThreePowerResult.ExecutionTime);
    
    // 4. 運行所有測試
    UE_LOG(LogTemp, Log, TEXT("\n運行完整測試套件..."));
    TArray<FIntegrationTestResultData> AllResults = IntegrationTest->RunAllTests();
    
    // 5. 生成測試報告
    FString Report = IntegrationTest->GenerateTestReport();
    UE_LOG(LogTemp, Log, TEXT("\n%s"), *Report);
    
    // 6. 運行性能測試
    UE_LOG(LogTemp, Log, TEXT("\n運行性能壓力測試..."));
    FPerformanceTestResult PerformanceResult = IntegrationTest->RunPerformanceStressTest(30.0f);
    UE_LOG(LogTemp, Log, TEXT("性能測試結果:"));
    UE_LOG(LogTemp, Log, TEXT("  平均幀時間: %.3fms"), PerformanceResult.AverageFrameTime * 1000.0f);
    UE_LOG(LogTemp, Log, TEXT("  最大幀時間: %.3fms"), PerformanceResult.MaxFrameTime * 1000.0f);
    UE_LOG(LogTemp, Log, TEXT("  內存使用: %.1fMB"), PerformanceResult.MemoryUsageMB);
    UE_LOG(LogTemp, Log, TEXT("  性能評估: %s"), PerformanceResult.bPerformanceAcceptable ? TEXT("✓ 可接受") : TEXT("✗ 需要優化"));
}

// ============================================================================
// 示例8: 完整遊戲場景模擬
// ============================================================================

void Example_CompleteGameScenario(UWorld* World)
{
    UE_LOG(LogTemp, Log, TEXT("=== 示例8: 完整遊戲場景模擬 ==="));
    
    // 1. 初始化所有系統
    Example_BasicSystemSetup(World);
    
    // 2. 設置聖者指揮官（玩家）
    UMingThreePowerSystem* PlayerThreePower = NewObject<UMingThreePowerSystem>();
    PlayerThreePower->InitializeThreePowerSystem();
    
    // 設置為聖者模式
    TMap<EPowerType, float> SagePowers;
    SagePowers.Add(EPowerType::Moral, 0.45f);
    SagePowers.Add(EPowerType::Strategy, 0.35f);
    SagePowers.Add(EPowerType::Military, 0.20f);
    PlayerThreePower->DistributePowers(SagePowers, TEXT("聖者初始化"));
    
    // 3. 啟動五行節奏
    UMingWuXingRhythmSystem* WuXingSystem = NewObject<UMingWuXingRhythmSystem>();
    WuXingSystem->InitializeWuXingRhythmSystem();
    WuXingSystem->StartWuXingRhythm();
    
    // 4. 遊戲回合開始
    UE_LOG(LogTemp, Log, TEXT("\n========== 遊戲回合開始 =========="));
    
    // 當前處於木階段（立名正當性）
    UE_LOG(LogTemp, Log, TEXT("當前五行階段: 木（春/立名）"));
    UE_LOG(LogTemp, Log, TEXT("階段效果: 正策效果+20%%, 徵兵速度+30%%"));
    
    // 5. 執行策略組合
    UMingSixStrategiesSystem* StrategySystem = NewObject<UMingSixStrategiesSystem>();
    StrategySystem->InitializeSixStrategiesSystem();
    
    // 執行正策組合：立國 + 立制
    FStrategyExecutionContext Context;
    Context.Direction = EStrategyDirection::Righteous;
    Context.CurrentPhase = EFiveElementPhase::Wood;
    
    StrategySystem->ExecuteStrategy(ESixStrategyType::EstablishNation, Context);
    StrategySystem->ExecuteStrategy(ESixStrategyType::EstablishSystem, Context);
    
    UE_LOG(LogTemp, Log, TEXT("✓ 執行正策組合: 立國 + 立制"));
    UE_LOG(LogTemp, Log, TEXT("  協效加成: +15%%"));
    
    // 6. 監控防墮機制
    UMingAntiCorruptionSystem* AntiCorruptionSystem = NewObject<UMingAntiCorruptionSystem>();
    AntiCorruptionSystem->InitializeAntiCorruptionSystem();
    
    FMoralBoundaryStatus MoralStatus = AntiCorruptionSystem->CheckMoralBoundaryStatus();
    UE_LOG(LogTemp, Log, TEXT("\n道德狀態監控:"));
    UE_LOG(LogTemp, Log, TEXT("  道德完整性: %.1f%%"), MoralStatus.MoralIntegrity);
    UE_LOG(LogTemp, Log, TEXT("  狀態評估: %s"), MoralStatus.bIsWithinBoundary ? TEXT("✓ 在邊界內") : TEXT("⚠ 接近邊界"));
    
    // 7. AI對手適配
    UMingAIAdaptationSystem* AIAdaptationSystem = NewObject<UMingAIAdaptationSystem>();
    AIAdaptationSystem->InitializeAIAdaptationSystem();
    
    // 識別玩家為聖者類型
    FAICommanderProfile PlayerProfile = AIAdaptationSystem->IdentifyOpponent(TEXT("Player_Sage"));
    UE_LOG(LogTemp, Log, TEXT("\nAI對手分析:"));
    UE_LOG(LogTemp, Log, TEXT("  識別類型: 聖者指揮官"));
    UE_LOG(LogTemp, Log, TEXT("  預測策略: 正策為主"));
    
    // AI生成反制策略
    FAIAdaptationDecision AIDecision = AIAdaptationSystem->GenerateAdaptationDecision(
        TEXT("Player_Sage"), ESixStrategyType::EstablishNation);
    UE_LOG(LogTemp, Log, TEXT("  AI決策: %d（破結構）"), (int32)AIDecision.RecommendedCounterStrategy);
    
    // 8. 回合結束
    UE_LOG(LogTemp, Log, TEXT("\n========== 遊戲回合結束 =========="));
    
    // 獲取統計
    FWuXingRhythmStatistics RhythmStats = WuXingSystem->GetRhythmStatistics();
    UE_LOG(LogTemp, Log, TEXT("回合統計:"));
    UE_LOG(LogTemp, Log, TEXT("  節奏循環: %d"), RhythmStats.TotalCycles);
    UE_LOG(LogTemp, Log, TEXT("  節奏效率: %.1f%%"), RhythmStats.RhythmEfficiency * 100.0f);
    
    UE_LOG(LogTemp, Log, TEXT("\n✓ 遊戲場景模擬完成"));
}

// ============================================================================
// 使用建議和最佳實踐
// ============================================================================

/*
最佳實踐:

1. 系統初始化順序:
   - 先初始化三權系統（基礎架構）
   - 然後初始化策略系統
   - 接著初始化五行節奏
   - 最後初始化防墮和AI系統

2. 權力分配建議:
   - 聖者: 道權45%, 策權35%, 兵權20%
   - 魔王: 道權20%, 策權30%, 兵權50%
   - 偽聖者: 道權35%, 策權40%, 兵權25%

3. 策略使用建議:
   - 木階段: 優先使用立國、立人
   - 火階段: 優先使用立制、破局
   - 土階段: 優先使用立制、破結構
   - 金階段: 優先使用破局、破結構
   - 水階段: 優先使用不破人

4. 防墮機制:
   - 定期進行自我審計
   - 監控道德完整性不低於70%
   - 及時應用防墮措施
   - 保持陰陽平衡

5. 性能優化:
   - 避免同時執行過多策略（建議最多3個）
   - 合理使用策略組合獲得協效
   - 監控系統內存使用
   - 定期清理過期數據

6. 調試建議:
   - 使用集成測試系統驗證功能
   - 啟用詳細日誌追蹤問題
   - 使用性能監控識別瓶頸
   - 定期運行完整測試套件
*/
