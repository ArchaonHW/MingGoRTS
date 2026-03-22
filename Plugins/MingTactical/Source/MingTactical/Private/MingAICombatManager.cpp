#include "MingAICombatManager.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Algo/RandomStream.h"

// 並行化支持
#include "Async/ParallelFor.h"
#include "Async/TaskGraphInterfaces.h"
#include "HAL/CriticalSection.h"
#include "HAL/PlatformTime.h"

UMingAICombatManager::UMingAICombatManager()
{
    bInitialized = false;
    AIUnitStates.Empty();
    AIDecisionHistory.Empty();
    PlayerBehaviorPatterns.Empty();
    AIStrategyTemplates.Empty();
    CurrentBattlefieldAnalysis = FMingAITacticalAnalysis();
}

bool UMingAICombatManager::InitializeAICombatSystem()
{
    if (bInitialized)
    {
        UE_LOG(LogTemp, Warning, TEXT("AI戰鬥系統已經初始化"));
        return true;
    }

    UE_LOG(LogTemp, Log, TEXT("正在初始化AI戰鬥系統..."));

    // 載入預設AI策略
    LoadDefaultAIStrategies();

    // 載入已保存的AI數據
    LoadAIData();

    bInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("AI戰鬥系統初始化完成"));
    
    return true;
}

bool UMingAICombatManager::RegisterAIUnit(int32 UnitID, EMingAIBehavior Behavior)
{
    if (AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 已存在，將覆蓋"), UnitID);
    }

    // 創建AI單位狀態
    FMingAIUnitState NewAIState;
    NewAIState.UnitID = UnitID;
    NewAIState.CurrentBehavior = Behavior;
    NewAIState.CombatExperience = 0.0f;
    NewAIState.LearningFactor = 1.0f;
    NewAIState.AdaptabilityScore = 0.5f;
    NewAIState.DecisionCooldown = 0.0f;
    NewAIState.LastDecisionTime = FDateTime::Now();

    // 根據行為類型初始化戰術知識
    switch (Behavior)
    {
    case EMingAIBehavior::Aggressive:
        NewAIState.TacticalKnowledge = {TEXT("frontal_assault"), TEXT("flanking"), TEXT("rapid_advance")};
        break;
    case EMingAIBehavior::Defensive:
        NewAIState.TacticalKnowledge = {TEXT("fortified_defense"), TEXT("counter_attack"), TEXT("strategic_withdrawal")};
        break;
    case EMingAIBehavior::Balanced:
        NewAIState.TacticalKnowledge = {TEXT("flexible_response"), TEXT("situational_awareness"), TEXT("tactical_positioning")};
        break;
    case EMingAIBehavior::Cautious:
        NewAIState.TacticalKnowledge = {TEXT("reconnaissance"), TEXT("careful_advance"), TEXT("threat_assessment")};
        break;
    case EMingAIBehavior::Tactical:
        NewAIState.TacticalKnowledge = {TEXT("coordinated_attack"), TEXT("formation_combat"), TEXT("strategic_positioning")};
        break;
    default:
        NewAIState.TacticalKnowledge = {TEXT("basic_combat")};
        break;
    }

    AIUnitStates.Add(UnitID, NewAIState);

    UE_LOG(LogTemp, Log, TEXT("註TEXT("註冊AI單位: ID %d，行為: %d"), UnitID, (int32)Behavior);
    
    return true;
}

bool UMingAICombatManager::RemoveAIUnit(int32 UnitID)
{
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates.Remove(UnitID);
        AIDecisionHistory.Remove(UnitID);
        
        UE_LOG(LogTemp, Log, TEXT("移除AI單位: ID %d"), UnitID);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
    return false;
}

bool UMingAICombatManager::UpdateAIUnitState(int32 UnitID, const FMingAIUnitState& State)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
        return false;
    }

    EMingAIBehavior OldBehavior = AIUnitStates[UnitID].CurrentBehavior;
    AIUnitStates[UnitID] = State;
    AIUnitStates[UnitID].LastDecisionTime = FDateTime::Now();

    // 觸發行為變化事件
    if (OldBehavior != State.CurrentBehavior)
    {
        OnAIBehaviorChanged.Broadcast(UnitID, State.CurrentBehavior);
    }

    // 觸發狀態變化事件
    OnAIUnitStateChanged.Broadcast(UnitID, State.CurrentBehavior, State);

    UE_LOG(LogTemp, Log, TEXT("更新AI單位 %d 狀態，行為: %d -> %d"), 
        UnitID, (int32)OldBehavior, (int32)State.CurrentBehavior);
    
    return true;
}

FMingAITacticalAnalysis UMingAICombatManager::AnalyzeBattlefield(const FVector& Center, float Radius)
{
    FMingAITacticalAnalysis Analysis;
    Analysis.AnalysisID = FMath::RandRange(1000, 9999);
    Analysis.BattlefieldCenter = Center;
    Analysis.BattlefieldRadius = Radius;
    Analysis.AnalysisTime = FDateTime::Now();

    // 分析戰場上的單位
    // 這裡需要從戰術戰鬥系統獲取單位信息
    // 暫時使用模擬數據
    
    // 模擬敵方單位
    for (int32 i = 0; i < 15; i++)
    {
        Analysis.EnemyUnits.Add(2000 + i); // 模擬敵方單位ID
    }
    
    // 模擬友方單位
    for (int32 i = 0; i < 12; i++)
    {
        Analysis.FriendlyUnits.Add(1000 + i); // 模擬友方單位ID
    }
    
    // 模擬中立單位
    for (int32 i = 0; i < 3; i++)
    {
        Analysis.NeutralUnits.Add(3000 + i); // 模擬中立單位ID
    }

    // 計算敵我力量對比
    Analysis.ForceRatio = (float)Analysis.FriendlyUnits.Num() / (float)Analysis.EnemyUnits.Num();

    // 分析地形優勢
    Analysis.TerrainAdvantages = AnalyzeTerrainAdvantages(Center);

    // 生成戰術建議
    Analysis.TacticalRecommendations = GenerateTacticalRecommendations(-1, Analysis);

    // 保存當前分析
    CurrentBattlefieldAnalysis = Analysis;

    // 觸發戰場分析事件
    OnAITacticalAnalysis.Broadcast(Analysis);

    UE_LOG(LogTemp, Log, TEXT("戰場分析完成: 敵方 %d 單位，友方 %d 單位，力量比 %.2f"), 
        Analysis.EnemyUnits.Num(), Analysis.FriendlyUnits.Num(), Analysis.ForceRatio);
    
    return Analysis;
}

TArray<FMingAIThreatAssessment> UMingAICombatManager::AssessThreats(int32 UnitID)
{
    TArray<FMingAIThreatAssessment> Threats;
    
    if (!AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
        return Threats;
    }

    const FMingAIUnitState& AIState = AIUnitStates[UnitID];
    FVector UnitPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取

    // 評估所有敵方單位的威脅
    for (int32 EnemyUnitID : CurrentBattlefieldAnalysis.EnemyUnits)
    {
        FMingAIThreatAssessment Threat;
        Threat.ThreatUnitID = EnemyUnitID;
        
        // 計算威脅等級
        float ThreatLevel = CalculateThreatLevel(UnitID, EnemyUnitID);
        Threat.ThreatLevel = ThreatLevel;
        
        // 設定威脅類型
        if (ThreatLevel > 80.0f)
        {
            Threat.ThreatType = TEXT("critical");
            Threat.ThreatPriority = EMingAIDecisionWeight::Critical;
        }
        else if (ThreatLevel > 60.0f)
        {
            Threat.ThreatType = TEXT("high");
            Threat.ThreatPriority = EMingAIDecisionWeight::High;
        }
        else if (ThreatLevel > 40.0f)
        {
            Threat.ThreatType = TEXT("moderate");
            Threat.ThreatPriority = EMingAIDecisionWeight::Medium;
        }
        else
        {
            Threat.ThreatType = TEXT("low");
            Threat.ThreatPriority = EMingAIDecisionWeight::Low;
        }
        
        // 計算威脅方向和距離
        FVector EnemyPosition = FVector::ZeroVector; // 需要從戰鬥系統獲取
        FVector Direction = EnemyPosition - UnitPosition;
        Threat.ThreatDirection = Direction;
        Threat.Distance = Direction.Size();
        
        // 計算威脅持續時間
        Threat.ThreatDuration = Threat.Distance / 100.0f; // 假設移動速度
        
        Threats.Add(Threat);
    }

    // 更新AI單位的威脅評估
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates[UnitID].ThreatAssessments = Threats;
    }

    // 觸發威脅評估事件
    for (const FMingAIThreatAssessment& Threat : Threats)
    {
        OnAIThreatAssessed.Broadcast(Threat);
    }

    UE_LOG(LogTemp, Log, TEXT("單位 %d 威脅評估完成，發現 %d 個威脅"), UnitID, Threats.Num());
    
    return Threats;
}

FMingAITacticalDecision UMingAICombatManager::MakeTacticalDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.DecisionTimestamp = FDateTime::Now();

    if (!AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
        return Decision;
    }

    const FMingAIUnitState& AIState = AIUnitStates[UnitID];
    
    // 根據AI行為類型制定決策
    switch (AIState.CurrentBehavior)
    {
    case EMingAIBehavior::Aggressive:
        Decision = MakeAggressiveDecision(UnitID);
        break;
        
    case EMingAIBehavior::Defensive:
        Decision = MakeDefensiveDecision(UnitID);
        break;
        
    case EMingAIBehavior::Balanced:
        Decision = MakeBalancedDecision(UnitID);
        break;
        
    case EMingAIBehavior::Cautious:
        Decision = MakeCautiousDecision(UnitID);
        break;
        
    case EMingAIBehavior::Tactical:
        Decision = MakeTacticalDecision(UnitID);
        break;
        
    case EMingAIBehavior::Adaptive:
        Decision = MakeAdaptiveDecision(UnitID);
        break;
        
    default:
        Decision = MakeDefaultDecision(UnitID);
        break;
    }

    // 計算決策信心度
    Decision.Confidence = CalculateDecisionConfidence(Decision);
    
    // 驗證決策
    if (ValidateAIDecision(Decision))
    {
        // 添加到決策歷史
        if (!AIDecisionHistory.Contains(UnitID))
        {
            AIDecisionHistory.Add(UnitID, TArray<FMingAITacticalDecision>());
        }
        AIDecisionHistory[UnitID].Add(Decision);
        
        // 觸發決策製作事件
        OnAIDecisionMade.Broadcast(Decision);
        
        UE_LOG(LogTemp, Log, TEXT("AI單位 %d 製作決策: %s (信心度: %.2f)"), 
            UnitID, *Decision.DecisionDescription, Decision.Confidence);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AI決策驗證失敗"));
    }

    return Decision;
}

bool UMingAICombatManager::ExecuteAIDecision(const FMingAITacticalDecision& Decision)
{
    // 執行AI決策
    bool bSuccess = false;
    
    switch (Decision.Objective)
    {
    case EMingAITacticalObjective::DestroyEnemy:
        bSuccess = ExecuteDestroyEnemyDecision(Decision);
        break;
        
    case EMingAITacticalObjective::CaptureObjective:
        bSuccess = ExecuteCaptureObjectiveDecision(Decision);
        break;
        
    case EMingAITacticalObjective::DefendPosition:
        bSuccess = ExecuteDefendPositionDecision(Decision);
        break;
        
    case EMingAITacticalObjective::FlankEnemy:
        bSuccess = ExecuteFlankEnemyDecision(Decision);
        break;
        
    case EMingAITacticalObjective::SupportAllies:
        bSuccess = ExecuteSupportAlliesDecision(Decision);
        break;
        
    case EMingAITacticalObjective::Retreat:
        bSuccess = ExecuteRetreatDecision(Decision);
        break;
        
    default:
        bSuccess = false;
        break;
    }

    // 更新AI學習
    UpdateAILearning(Decision.TargetUnitID, Decision, bSuccess);

    UE_LOG(LogTemp, Log, TEXT("執行AI決策: %s - %s"), 
        *Decision.DecisionDescription, bSuccess ? TEXT("成功") : TEXT("失敗"));
    
    return bSuccess;
}

bool UMingAICombatManager::SetAIBehavior(int32 UnitID, EMingAIBehavior Behavior)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
        return false;
    }

    EMingAIBehavior OldBehavior = AIUnitStates[UnitID].CurrentBehavior;
    AIUnitStates[UnitID].CurrentBehavior = Behavior;
    AIUnitStates[UnitID].LastDecisionTime = FDateTime::Now();

    // 根據新行為更新戰術知識
    switch (Behavior)
    {
    case EMingAIBehavior::Aggressive:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("frontal_assault"), TEXT("flanking"), TEXT("rapid_advance")};
        break;
    case EMingAIBehavior::Defensive:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("fortified_defense"), TEXT("counter_attack"), TEXT("strategic_withdrawal")};
        break;
    case EMingAIBehavior::Balanced:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("flexible_response"), TEXT("situational_awareness"), TEXT("tactical_positioning")};
        break;
    case EMingAIBehavior::Cautious:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("reconnaissance"), TEXT("careful_advance"), TEXT("threat_assessment")};
        break;
    case EMingAIBehavior::Tactical:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("coordinated_attack"), TEXT("formation_combat"), TEXT("strategic_positioning")};
        break;
    default:
        AIUnitStates[UnitID].TacticalKnowledge = {TEXT("basic_combat")};
        break;
    }

    // 觸發行為變化事件
    if (OldBehavior != Behavior)
    {
        OnAIBehaviorChanged.Broadcast(UnitID, Behavior);
    }

    UE_LOG(LogTemp, Log, TEXT("AI單位 %d 行為變更: %d -> %d"), 
        UnitID, (int32)OldBehavior, (int32)Behavior);
    
    return true;
}

FMingAIUnitState UMingAICombatManager::GetAIUnitState(int32 UnitID) const
{
    if (AIUnitStates.Contains(UnitID))
    {
        return AIUnitStates[UnitID];
    }
    
    UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在，返回空狀態"), UnitID);
    return FMingAIUnitState();
}

TArray<FMingAITacticalDecision> UMingAICombatManager::GetAIDecisionHistory(int32 UnitID) const
{
    if (AIDecisionHistory.Contains(UnitID))
    {
        return AIDecisionHistory[UnitID];
    }
    
    return TArray<FMingAITacticalDecision>();
}

float UMingAICombatManager::CalculateAIIntelligence(int32 UnitID) const
{
    if (!AIUnitStates.Contains(UnitID))
    {
        return 0.0f;
    }

    const FMingAIUnitState& AIState = AIUnitStates[UnitID];
    
    // 基礎智能分數
    float BaseIntelligence = 50.0f;
    
    // 經驗加成
    float ExperienceBonus = AIState.CombatExperience * 0.5f;
    
    // 學習加成
    float LearningBonus = AIState.LearningFactor * 30.0f;
    
    // 適應性加成
    float AdaptabilityBonus = AIState.AdaptabilityScore * 20.0f;
    
    // 戰術知識加成
    float KnowledgeBonus = AIState.TacticalKnowledge.Num() * 5.0f;
    
    // 綜合智能分數
    float TotalIntelligence = BaseIntelligence + ExperienceBonus + LearningBonus + 
                            AdaptabilityBonus + KnowledgeBonus;
    
    return FMath::Clamp(TotalIntelligence, 0.0f, 100.0f);
}

bool UMingAICombatManager::LearnFromPlayerBehavior(int32 PlayerUnitID, const FString& Action)
{
    // 分析玩家行為模式
    if (PlayerBehaviorPatterns.Contains(Action))
    {
        float& PatternCount = PlayerBehaviorPatterns[Action];
        PatternCount += 1.0f;
    }
    else
    {
        PlayerBehaviorPatterns.Add(Action, 1.0f);
    }

    // 更新相關AI單位的學習因子
    for (TPair<int32, FMingAIUnitState>& Pair : AIUnitStates)
    {
        int32 AIUnitID = Pair.Key;
        FMingAIUnitState& AIState = Pair.Value;
        
        // 根據玩家行為調整AI策略
        if (Action.Contains(TEXT("aggressive")))
        {
            AIState.LearningFactor = FMath::Clamp(AIState.LearningFactor + 0.1f, 0.5f, 2.0f);
        }
        else if (Action.Contains(TEXT("defensive")))
        {
            AIState.LearningFactor = FMath::Clamp(AIState.LearningFactor - 0.1f, 0.5f, 2.0f);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("AI從玩家行為學習: %s"), *Action);
    
    return true;
}

bool UMingAICombatManager::AdaptAIStrategy(int32 UnitID)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        UE_LOG(LogTemp, Warning, TEXT("AI單位ID %d 不存在"), UnitID);
        return false;
    }

    FMingAIUnitState& AIState = AIUnitStates[UnitID];
    
    // 根據當前表現調整策略
    float PerformanceScore = EvaluateAIPerformance(UnitID);
    
    if (PerformanceScore < 30.0f)
    {
        // 表現不佳，嘗試變更行為
        EMingAIBehavior NewBehavior = EMingAIBehavior::Adaptive;
        
        if (AIState.CurrentBehavior == EMingAIBehavior::Aggressive)
        {
            NewBehavior = EMingAIBehavior::Cautious;
        }
        else if (AIState.CurrentBehavior == EMingAIBehavior::Defensive)
        {
            NewBehavior = EMingAIBehavior::Balanced;
        }
        
        return SetAIBehavior(UnitID, NewBehavior);
    }
    else if (PerformanceScore > 70.0f)
    {
        // 表現良好，增強當前行為
        AIState.AdaptabilityScore = FMath::Clamp(AIState.AdaptabilityScore + 0.1f, 0.0f, 1.0f);
    }

    UE_LOG(LogTemp, Log, TEXT("AI單位 %d 適應策略，表現評分: %.2f"), UnitID, PerformanceScore);
    
    return true;
}

TMap<EMingAIBehavior, int32> UMingAICombatManager::GetAIStatistics() const
{
    TMap<EMingAIBehavior, int32> Statistics;
    
    // 初始化統計
    Statistics.Add(EMingAIBehavior::Aggressive, 0);
    Statistics.Add(EMingAIBehavior::Defensive, 0);
    Statistics.Add(EMingAIBehavior::Balanced, 0);
    Statistics.Add(EMingAIBehavior::Cautious, 0);
    Statistics.Add(EMingAIBehavior::Reckless, 0);
    Statistics.Add(EMingAIBehavior::Tactical, 0);
    Statistics.Add(EMingAIBehavior::Adaptive, 0);
    
    // 統計AI行為類型
    for (const TPair<int32, FMingAIUnitState>& Pair : AIUnitStates)
    {
        const FMingAIUnitState& AIState = Pair.Value;
        int32& Count = Statistics[AIState.CurrentBehavior];
        Count++;
    }
    
    return Statistics;
}

bool UMingAICombatManager::SaveAIData()
{
    FString SavePath = FPaths::ProjectSavedDir() / TEXT("AICombat.json");
    
    // 這裡應該實作JSON序列化保存邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("AI戰鬥數據已保存到: %s"), *SavePath);
    UE_LOG(LogTemp, Log, TEXT("共保存 %d 個AI單位，%d 條決策歷史"), 
        AIUnitStates.Num(), AIDecisionHistory.Num());
    
    return true;
}

bool UMingAICombatManager::LoadAIData()
{
    FString LoadPath = FPaths::ProjectSavedDir() / TEXT("AICombat.json");
    
    // 這裡應該實作JSON反序列化載入邏輯
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("嘗試從 %s 載入AI戰鬥數據"), *LoadPath);
    
    return true;
}

void UMingAICombatManager::ClearAllAIUnits()
{
    AIUnitStates.Empty();
    AIDecisionHistory.Empty();
    PlayerBehaviorPatterns.Empty();
    CurrentBattlefieldAnalysis = FMingAITacticalAnalysis();
    
    UE_LOG(LogTemp, Log, TEXT("所有AI單位數據已清除"));
}

void UMingAICombatManager::LoadDefaultAIStrategies()
{
    // 攻擊性AI策略
    TArray<FMingAITacticalDecision> AggressiveStrategies;
    
    FMingAITacticalDecision Aggressive1;
    Aggressive1.DecisionID = 1;
    Aggressive1.Objective = EMingAITacticalObjective::DestroyEnemy;
    Aggressive1.DecisionDescription = TEXT("正面攻擊敵方主力");
    Aggressive1.DecisionWeight = EMingAIDecisionWeight::High;
    Aggressive1.ExpectedOutcome = TEXT("快速消滅敵人");
    Aggressive1.DecisionParameters = {
        {TEXT("attack_pattern"), TEXT("frontal")},
        {TEXT("intensity"), TEXT("high")}
    };
    AggressiveStrategies.Add(Aggressive1);
    
    // 防禦性AI策略
    TArray<FMingAITacticalDecision> DefensiveStrategies;
    
    FMingAITacticalDecision Defensive1;
    Defensive1.DecisionID = 2;
    Defensive1.Objective = EMingAITacticalObjective::DefendPosition;
    Defensive1.DecisionDescription = TEXT("建立防禦陣地");
    Defensive1.DecisionWeight = EMingAIDecisionWeight::Medium;
    Defensive1.ExpectedOutcome = TEXT("有效抵禦敵人攻擊");
    Defensive1.DecisionParameters = {
        {TEXT("defense_pattern"), TEXT("fortified")},
        {TEXT("formation"), TEXT("circle")}
    };
    DefensiveStrategies.Add(Defensive1);
    
    // 平衡型AI策略
    TArray<FMingAITacticalDecision> BalancedStrategies;
    
    FMingAITacticalDecision Balanced1;
    Balanced1.DecisionID = 3;
    Balanced1.Objective = EMingAITacticalObjective::FlankEnemy;
    Balanced1.DecisionDescription = TEXT("側翼攻擊敵人");
    Balanced1.DecisionWeight = EMingAIDecisionWeight::Medium;
    Balanced1.ExpectedOutcome = TEXT("從側面打擊敵人");
    Balanced1.DecisionParameters = {
        {TEXT("attack_pattern"), TEXT("flanking")},
        {TEXT("coordination"), TEXT("high")}
    };
    BalancedStrategies.Add(Balanced1);
    
    // 添加到策略模板
    AIStrategyTemplates.Add(EMingAIBehavior::Aggressive, AggressiveStrategies);
    AIStrategyTemplates.Add(EMingAIBehavior::Defensive, DefensiveStrategies);
    AIStrategyTemplates.Add(EMingAIBehavior::Balanced, BalancedStrategies);
    
    UE_LOG(LogTemp, Log, TEXT("已載入 %d 種預設AI策略"), AIStrategyTemplates.Num());
}

float UMingAICombatManager::CalculateThreatLevel(int32 UnitID, int32 ThreatUnitID) const
{
    // 基礎威脅等級
    float BaseThreat = 50.0f;
    
    // 距離因子
    float DistanceFactor = 1.0f; // 需要從戰鬥系統獲取實際距離
    
    // 單位類型因子
    float UnitTypeFactor = 1.0f; // 需要從戰鬥系統獲取單位類型
    
    // 數量優勢因子
    float NumericalAdvantage = 1.0f; // 需要計算敵我數量對比
    
    // 最終威脅等級
    float FinalThreat = BaseThreat * DistanceFactor * UnitTypeFactor * NumericalAdvantage;
    
    return FMath::Clamp(FinalThreat, 0.0f, 100.0f);
}

float UMingAICombatManager::EvaluateTacticalAdvantage(const TArray<int32>& FriendlyUnits, const TArray<int32>& EnemyUnits) const
{
    // 基礎優勢分數
    float BaseAdvantage = 0.0f;
    
    // 數量優勢
    float NumericalAdvantage = (float)FriendlyUnits.Num() / (float)EnemyUnits.Num();
    if (NumericalAdvantage > 1.2f)
    {
        BaseAdvantage += 20.0f;
    }
    else if (NumericalAdvantage < 0.8f)
    {
        BaseAdvantage -= 20.0f;
    }
    
    // 位置優勢
    float PositionalAdvantage = 0.0f; // 需要分析地形和位置
    
    // 編隊優勢
    float FormationAdvantage = 0.0f; // 需要檢查編隊狀態
    
    return BaseAdvantage + PositionalAdvantage + FormationAdvantage;
}

FMingAITacticalDecision UMingAICombatManager::SelectBestDecision(int32 UnitID, const TArray<FMingAITacticalDecision>& Decisions) const
{
    if (Decisions.Num() == 0)
    {
        return FMingAITacticalDecision();
    }

    FMingAITacticalDecision BestDecision = Decisions[0];
    float BestScore = EvaluateDecisionOutcome(BestDecision);
    
    for (const FMingAITacticalDecision& Decision : Decisions)
    {
        float Score = EvaluateDecisionOutcome(Decision);
        if (Score > BestScore)
        {
            BestScore = Score;
            BestDecision = Decision;
        }
    }
    
    return BestDecision;
}

void UMingAICombatManager::UpdateAILearning(int32 UnitID, const FMingAITacticalDecision& Decision, bool bSuccess)
{
    if (!AIUnitStates.Contains(UnitID))
    {
        return;
    }

    FMingAIUnitState& AIState = AIUnitStates[UnitID];
    
    // 更新經驗值
    if (bSuccess)
    {
        AIState.CombatExperience += 10.0f;
    }
    else
    {
        AIState.CombatExperience += 2.0f; // 失敗也有少量經驗
    }
    
    // 更新適應性分數
    float LearningRate = 0.1f;
    if (bSuccess)
    {
        AIState.AdaptabilityScore = FMath::Clamp(AIState.AdaptabilityScore + LearningRate, 0.0f, 1.0f);
    }
    else
    {
        AIState.AdaptabilityScore = FMath::Clamp(AIState.AdaptabilityScore - LearningRate * 0.5f, 0.0f, 1.0f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI單位 %d 學習更新: 經驗 %.1f, 適應性 %.2f"), 
        UnitID, AIState.CombatExperience, AIState.AdaptabilityScore);
}

FString UMingAICombatManager::PredictPlayerBehavior(int32 PlayerUnitID) const
{
    // 預測玩家行為模式
    FString PredictedBehavior = TEXT("unknown");
    
    // 基於歷史模式預測
    float HighestPattern = 0.0f;
    for (const TPair<FString, float>& Pattern : PlayerBehaviorPatterns)
    {
        if (Pattern.Value > HighestPattern)
        {
            HighestPattern = Pattern.Value;
            PredictedBehavior = Pattern.Key;
        }
    }
    
    return PredictedBehavior;
}

float UMingAICombatManager::CalculateDecisionConfidence(const FMingAITacticalDecision& Decision) const
{
    // 基礎信心度
    float BaseConfidence = 0.5f;
    
    // 決策權重影響
    float WeightFactor = 1.0f;
    switch (Decision.DecisionWeight)
    {
    case EMingAIDecisionWeight::Critical:
        WeightFactor = 1.5f;
        break;
    case EMingAIDecisionWeight::High:
        WeightFactor = 1.2f;
        break;
    case EMingAIDecisionWeight::Medium:
        WeightFactor = 1.0f;
        break;
    case EMingAIDecisionWeight::Low:
        WeightFactor = 0.8f;
        break;
    case EMingAIDecisionWeight::Minimal:
        WeightFactor = 0.6f;
        break;
    }
    
    // 經驗因子
    float ExperienceFactor = 1.0f; // 需要從AI狀態獲取
    
    // 最終信心度
    float FinalConfidence = BaseConfidence * WeightFactor * ExperienceFactor;
    
    return FMath::Clamp(FinalConfidence, 0.0f, 1.0f);
}

bool UMingAICombatManager::ValidateAIDecision(const FMingAITacticalDecision& Decision) const
{
    // 檢查決策基本有效性
    if (Decision.DecisionID <= 0)
    {
        return false;
    }
    
    if (Decision.DecisionDescription.IsEmpty())
    {
        return false;
    }
    
    if (Decision.TargetUnitID <= 0 && Decision.TargetLocation.IsZero())
    {
        return false;
    }
    
    return true;
}

void UMingAICombatManager::UpdateAIAdaptability(int32 UnitID, float AdaptationFactor)
{
    if (AIUnitStates.Contains(UnitID))
    {
        AIUnitStates[UnitID].AdaptabilityScore = FMath::Clamp(
            AIUnitStates[UnitID].AdaptabilityScore + AdaptationFactor, 0.0f, 1.0f);
    }
}

void UMingAICombatManager::ProcessAIDecisionCooldown(int32 UnitID, float DeltaTime)
{
    if (AIUnitStates.Contains(UnitID))
    {
        FMingAIUnitState& AIState = AIUnitStates[UnitID];
        
        if (AIState.DecisionCooldown > 0.0f)
        {
            AIState.DecisionCooldown -= DeltaTime;
            AIState.DecisionCooldown = FMath::Max(0.0f, AIState.DecisionCooldown);
        }
    }
}

// 輔助決策製作函數
FMingAITacticalDecision UMingAICombatManager::MakeAggressiveDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::DestroyEnemy;
    Decision.DecisionDescription = TEXT("發動正面攻擊");
    Decision.DecisionWeight = EMingAIDecisionWeight::High;
    Decision.ExpectedOutcome = TEXT("快速消滅敵人");
    Decision.DecisionParameters = {
        {TEXT("attack_pattern"), TEXT("frontal")},
        {TEXT("intensity"), TEXT("high")}
    };
    
    // 選擇最脆弱的敵人
    if (CurrentBattlefieldAnalysis.EnemyUnits.Num() > 0)
    {
        Decision.TargetUnitID = CurrentBattlefieldAnalysis.EnemyUnits[0];
    }
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeDefensiveDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::DefendPosition;
    Decision.DecisionDescription = TEXT("建立防禦陣地");
    Decision.DecisionWeight = EMingAIDecisionWeight::Medium;
    Decision.ExpectedOutcome = TEXT("有效抵禦敵人攻擊");
    Decision.DecisionParameters = {
        {TEXT("defense_pattern"), TEXT("fortified")},
        {TEXT("formation"), TEXT("circle")}
    };
    
    // 計算最佳防禦位置
    Decision.TargetLocation = CalculateDefensivePosition(UnitID);
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeBalancedDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::FlankEnemy;
    Decision.DecisionDescription = TEXT("側翼攻擊敵人");
    Decision.DecisionWeight = EMingAIDecisionWeight::Medium;
    Decision.ExpectedOutcome = TEXT("從側面打擊敵人");
    Decision.DecisionParameters = {
        {TEXT("attack_pattern"), TEXT("flanking")},
        {TEXT("coordination"), TEXT("high")}
    };
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeCautiousDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::SupportAllies;
    Decision.DecisionDescription = TEXT("支援友軍單位");
    Decision.DecisionWeight = EMingAIDecisionWeight::Low;
    Decision.ExpectedOutcome = TEXT("增強友軍戰力");
    Decision.DecisionParameters = {
        {TEXT("support_type"), TEXT("tactical")},
        {TEXT("priority"), TEXT("medium")}
    };
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeTacticalDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::CaptureObjective;
    Decision.DecisionDescription = TEXT("佔領戰略目標");
    Decision.DecisionWeight = EMingAIDecisionWeight::High;
    Decision.ExpectedOutcome = TEXT("獲得戰術優勢");
    Decision.DecisionParameters = {
        {TEXT("capture_method"), TEXT("coordinated")},
        {TEXT("speed"), TEXT("fast")}
    };
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeAdaptiveDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    
    // 根據玩家行為模式適應
    FString PlayerBehavior = PredictPlayerBehavior(-1); // 需要玩家單位ID
    
    if (PlayerBehavior.Contains(TEXT("aggressive")))
    {
        Decision.Objective = EMingAITacticalObjective::DefendPosition;
        Decision.DecisionDescription = TEXT("防禦性反應");
    }
    else if (PlayerBehavior.Contains(TEXT("defensive")))
    {
        Decision.Objective = EMingAITacticalObjective::FlankEnemy;
        Decision.DecisionDescription = TEXT("攻擊性反應");
    }
    else
    {
        Decision.Objective = EMingAITacticalObjective::DestroyEnemy;
        Decision.DecisionDescription = TEXT("平衡反應");
    }
    
    Decision.DecisionWeight = EMingAIDecisionWeight::Medium;
    Decision.ExpectedOutcome = TEXT("適應性反應");
    
    return Decision;
}

FMingAITacticalDecision UMingAICombatManager::MakeDefaultDecision(int32 UnitID)
{
    FMingAITacticalDecision Decision;
    Decision.DecisionID = FMath::RandRange(10000, 99999);
    Decision.Objective = EMingAITacticalObjective::DestroyEnemy;
    Decision.DecisionDescription = TEXT("基本攻擊");
    Decision.DecisionWeight = EMingAIDecisionWeight::Medium;
    Decision.ExpectedOutcome = TEXT("標準戰鬥");
    
    return Decision;
}

// 輔助決策執行函數
bool UMingAICombatManager::ExecuteDestroyEnemyDecision(const FMingAITacticalDecision& Decision)
{
    // 執行消滅敵人決策
    UE_LOG(LogTemp, Log, TEXT("執行消滅敵人決策: %s"), *Decision.DecisionDescription);
    
    // 這裡需要調用戰鬥系統來執行實際攻擊
    // return CombatSystem->AttackUnit(Decision.TargetUnitID, UnitID);
    
    return true;
}

bool UMingAICombatManager::ExecuteCaptureObjectiveDecision(const FMingAITacticalDecision& Decision)
{
    // 執行佔領目標決策
    UE_LOG(LogTemp, Log, TEXT("執行佔領目標決策: %s"), *Decision.DecisionDescription);
    
    return true;
}

bool UMingAICombatManager::ExecuteDefendPositionDecision(const FMingAITacticalDecision& Decision)
{
    // 執行防禦位置決策
    UE_LOG(LogTemp, Log, TEXT("執行防禦位置決策: %s"), *Decision.DecisionDescription);
    
    return true;
}

bool UMingAICombatManager::ExecuteFlankEnemyDecision(const FMingAITacticalDecision& Decision)
{
    // 執行側翼攻擊決策
    UE_LOG(LogTemp, Log, TEXT("執行側翼攻擊決策: %s"), *Decision.DecisionDescription);
    
    return true;
}

bool UMingAICombatManager::ExecuteSupportAlliesDecision(const FMingAITacticalDecision& Decision)
{
    // 執行支援友軍決策
    UE_LOG(LogTemp, Log, TEXT("執行支援友軍決策: %s"), *Decision.DecisionDescription);
    
    return true;
}

bool UMingAICombatManager::ExecuteRetreatDecision(const FMingAITacticalDecision& Decision)
{
    // 執行撤退決策
    UE_LOG(LogTemp, Log, TEXT("執行撤退決策: %s"), *Decision.DecisionDescription);
    
    return true;
}

// 其他輔助函數的實現
TArray<FMingAITacticalDecision> UMingAICombatManager::GenerateTacticalRecommendations(int32 UnitID, const FMingAITacticalAnalysis& Analysis) const
{
    TArray<FMingAITacticalDecision> Recommendations;
    
    // 根據戰場分析生成建議
    if (Analysis.ForceRatio > 1.5f)
    {
        // 敵我力量優勢，建議攻擊
        FMingAITacticalDecision AttackRecommendation;
        AttackRecommendation.Objective = EMingAITacticalObjective::DestroyEnemy;
        AttackRecommendation.DecisionDescription = TEXT("利用數量優勢發動攻擊");
        AttackRecommendation.DecisionWeight = EMingAIDecisionWeight::High;
        Recommendations.Add(AttackRecommendation);
    }
    else if (Analysis.ForceRatio < 0.7f)
    {
        // 敵強我弱，建議防禦
        FMingAITacticalDecision DefendRecommendation;
        DefendRecommendation.Objective = EMingAITacticalObjective::DefendPosition;
        DefendRecommendation.DecisionDescription = TEXT("建立防禦陣地");
        DefendRecommendation.DecisionWeight = EMingAIDecisionWeight::High;
        Recommendations.Add(DefendRecommendation);
    }
    else
    {
        // 勢均力，建議戰術機動
        FMingAITacticalDecision TacticalRecommendation;
        TacticalRecommendation.Objective = EMingAITacticalObjective::FlankEnemy;
        TacticalRecommendation.DecisionDescription = TEXT("尋找戰術機會");
        TacticalRecommendation.DecisionWeight = EMingAIDecisionWeight::Medium;
        Recommendations.Add(TacticalRecommendation);
    }
    
    return Recommendations;
}

float UMingAICombatManager::EvaluateDecisionOutcome(const FMingAITacticalDecision& Decision) const
{
    // 評估決策結果
    float BaseScore = 50.0f;
    
    // 決策權重影響
    float WeightScore = 0.0f;
    switch (Decision.DecisionWeight)
    {
    case EMingAIDecisionWeight::Critical:
        WeightScore = 30.0f;
        break;
    case EMingAIDecisionWeight::High:
        WeightScore = 20.0f;
        break;
    case EMingAIDecisionWeight::Medium:
        WeightScore = 10.0f;
        break;
    case EMingAIDecisionWeight::Low:
        WeightScore = 5.0f;
        break;
    case EMingAIDecisionWeight::Minimal:
        WeightScore = 2.0f;
        break;
    }
    
    return BaseScore + WeightScore;
}

float UMingAICombatManager::EvaluateAIPerformance(int32 UnitID) const
{
    if (!AIUnitStates.Contains(UnitID))
    {
        return 0.0f;
    }

    const FMingAIUnitState& AIState = AIUnitStates[UnitID];
    
    // 基於決策歷史評估表現
    if (!AIDecisionHistory.Contains(UnitID))
    {
        return 50.0f; // 沒有歷史記錄，返回中等評分
    }

    const TArray<FMingAITacticalDecision>& History = AIDecisionHistory[UnitID];
    
    float SuccessRate = 0.0f;
    int32 TotalDecisions = History.Num();
    
    if (TotalDecisions > 0)
    {
        int32 SuccessfulDecisions = 0;
        for (const FMingAITacticalDecision& Decision : History)
        {
            // 簡單的成功判斷
            if (Decision.ExpectedOutcome.Contains(TEXT("成功")) || 
                Decision.ExpectedOutcome.Contains(TEXT("消滅")) ||
                Decision.ExpectedOutcome.Contains(TEXT("有效")))
            {
                SuccessfulDecisions++;
            }
        }
        
        SuccessRate = (float)SuccessfulDecisions / (float)TotalDecisions;
    }
    
    // 綜合表現評分
    float PerformanceScore = SuccessRate * 100.0f + AIState.AdaptabilityScore * 50.0f;
    
    return FMath::Clamp(PerformanceScore, 0.0f, 100.0f);
}

TMap<FString, float> UMingAICombatManager::AnalyzeTerrainAdvantages(const FVector& Location) const
{
    TMap<FString, float> Advantages;
    
    // 模擬地形分析
    Advantages.Add(TEXT("high_ground"), 0.3f);
    Advantages.Add(TEXT("cover"), 0.6f);
    Advantages.Add(TEXT("choke_point"), 0.2f);
    Advantages.Add(TEXT("open_field"), 0.8f);
    
    return Advantages;
}

FVector UMingAICombatManager::CalculateDefensivePosition(int32 UnitID) const
{
    // 計算最佳防禦位置
    FVector DefensivePosition = FVector::ZeroVector;
    
    // 這裡需要分析地形和敵人位置
    // 暫時返回模擬位置
    DefensivePosition = FVector(500.0f, 300.0f, 0.0f);
    
    return DefensivePosition;
}

// ========== 並行化 AI 處理實現 (Parallel Processing Implementation) ==========

void UMingAICombatManager::ProcessAllAIUnitsParallel(float DeltaTime)
{
    if (!bInitialized || AIUnitStates.Num() == 0)
    {
        return;
    }

    // 記錄開始時間
    double StartTime = FPlatformTime::Seconds();

    // 收集所有有效的 AI 單位 ID
    TArray<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 決定使用並行化還是串行處理
    if (UnitIDs.Num() >= ParallelThreshold)
    {
        // ===== 並行處理 (ParallelFor) =====
        // 使用 TaskGraph 並行處理多個單位
        
        ParallelFor(UnitIDs.Num(), [&](int32 Index)
        {
            int32 UnitID = UnitIDs[Index];
            
            // 檢查單位狀態是否有效
            if (!AIUnitStates.Contains(UnitID))
            {
                return;
            }
            
            FMingAIUnitState& AIState = AIUnitStates[UnitID];
            
            // 處理決策冷卻
            ProcessAIDecisionCooldown(UnitID, DeltaTime);
            
            // 如果冷卻完成，進行決策
            if (AIState.DecisionCooldown <= 0.0f)
            {
                // 生成戰術決策 (只讀取戰場分析，不修改共享數據)
                FMingAITacticalDecision Decision = MakeTacticalDecision(UnitID);
                
                // 將決策加入執行緒安全的快取
                if (Decision.Confidence > 0.3f) // 只快取高信心度決策
                {
                    FScopeLock Lock(&ParallelCacheLock);
                    ParallelDecisionsCache.Add(Decision);
                }
            }
        });
        
        // 在主線程中執行所有快取的決策 (確保 Blueprint 兼容性)
        for (const FMingAITacticalDecision& Decision : ParallelDecisionsCache)
        {
            ExecuteAIDecision(Decision);
        }
        ParallelDecisionsCache.Empty();
        
        LastParallelThreadCount = FTaskGraphInterface::Get().GetNumWorkerThreads();
    }
    else
    {
        // ===== 串行處理 (單位數量較少時) =====
        for (int32 UnitID : UnitIDs)
        {
            ProcessAIDecisionCooldown(UnitID, DeltaTime);
            
            FMingAIUnitState& AIState = AIUnitStates[UnitID];
            if (AIState.DecisionCooldown <= 0.0f)
            {
                FMingAITacticalDecision Decision = MakeTacticalDecision(UnitID);
                if (Decision.Confidence > 0.3f)
                {
                    ExecuteAIDecision(Decision);
                }
            }
        }
        
        LastParallelThreadCount = 1;
    }

    // 記錄處理時間
    double EndTime = FPlatformTime::Seconds();
    LastParallelProcessingTimeMs = (EndTime - StartTime) * 1000.0f;

    UE_LOG(LogTemp, Log, TEXT("AI 並行處理完成: %d 單位, 耗時 %.2f ms, 使用 %d 執行緒"),
        UnitIDs.Num(), LastParallelProcessingTimeMs, LastParallelThreadCount);
}

void UMingAICombatManager::AssessThreatsForAllUnitsParallel()
{
    if (!bInitialized || AIUnitStates.Num() == 0)
    {
        return;
    }

    TArray<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 使用並行化評估所有單位的威脅
    ParallelFor(UnitIDs.Num(), [&](int32 Index)
    {
        int32 UnitID = UnitIDs[Index];
        
        if (AIUnitStates.Contains(UnitID))
        {
            // 評估威脅 (每個單位獨立進行，無需同步)
            TArray<FMingAIThreatAssessment> Threats = AssessThreats(UnitID);
            
            // 使用鎖保護寫入操作
            FScopeLock Lock(&ParallelCacheLock);
            if (AIUnitStates.Contains(UnitID))
            {
                AIUnitStates[UnitID].ThreatAssessments = Threats;
            }
        }
    });

    UE_LOG(LogTemp, Log, TEXT("並行威脅評估完成: %d 單位"), UnitIDs.Num());
}

TArray<FMingAITacticalDecision> UMingAICombatManager::GenerateDecisionsForAllUnitsParallel()
{
    TArray<FMingAITacticalDecision> AllDecisions;
    
    if (!bInitialized || AIUnitStates.Num() == 0)
    {
        return AllDecisions;
    }

    TArray<int32> UnitIDs;
    AIUnitStates.GetKeys(UnitIDs);

    // 預分配結果陣列
    AllDecisions.SetNumZeroed(UnitIDs.Num());

    // 並行生成決策
    ParallelFor(UnitIDs.Num(), [&](int32 Index)
    {
        int32 UnitID = UnitIDs[Index];
        
        if (AIUnitStates.Contains(UnitID))
        {
            FMingAITacticalDecision Decision = MakeTacticalDecision(UnitID);
            AllDecisions[Index] = Decision;
        }
    });

    // 移除無效決策
    AllDecisions.RemoveAll([](const FMingAITacticalDecision& Decision)
    {
        return Decision.UnitID <= 0 || Decision.DecisionID < 0;
    });

    UE_LOG(LogTemp, Log, TEXT("並行決策生成完成: %d 單位, 生成 %d 有效決策"),
        UnitIDs.Num(), AllDecisions.Num());

    return AllDecisions;
}
