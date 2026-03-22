#include "MingCombatAI.h"
#include "MingUnitController.h"
#include "MingMultiUnitCoordinator.h"
#include "MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

AMingCombatAI::AMingCombatAI()
    : bIsInitialized(false)
    , UnitController(nullptr)
    , MultiUnitCoordinator(nullptr)
    , DifficultyLevel(EAIDifficultyLevel::Normal)
    , TacticalType(EAITacticalType::Balanced)
    , LearningType(EAILearningType::Reinforcement)
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 預分配容量
    ControlledUnits.Reserve(100);
    DecisionNodes.Reserve(200);
    TacticalAnalyses.Reserve(50);
    LearningData.Reserve(100);
    AIStatistics.Reserve(50);
    AIConfiguration.Reserve(30);
}

void AMingCombatAI::InitializeCombatAI()
{
    if (bIsInitialized)
    {
        return;
    }

    // 獲取單位控制器和多單位協調器
    UWorld* World = GetWorld();
    if (World)
    {
        // 簡化實作：假設組件已存在
        // UnitController = World->GetSubsystem<AMingUnitController>();
        // MultiUnitCoordinator = World->GetSubsystem<AMingMultiUnitCoordinator>();
    }

    // 初始化AI配置
    AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(1.0f));
    AIConfiguration.Add(TEXT("learning_rate"), FString::SanitizeFloat(0.1f));
    AIConfiguration.Add(TEXT("adaptation_rate"), FString::SanitizeFloat(0.05f));
    AIConfiguration.Add(TEXT("max_decision_depth"), FString::FromInt(10));
    AIConfiguration.Add(TEXT("performance_threshold"), FString::SanitizeFloat(0.7f));
    
    // 初始化統計數據
    AIStatistics.Add(TEXT("total_decisions"), 0);
    AIStatistics.Add(TEXT("successful_decisions"), 0);
    AIStatistics.Add(TEXT("failed_decisions"), 0);
    AIStatistics.Add(TEXT("learning_iterations"), 0);
    AIStatistics.Add(TEXT("tactical_analyses"), 0);
    AIStatistics.Add(TEXT("adaptations"), 0);
    AIStatistics.Add(TEXT("predictions"), 0);
    AIStatistics.Add(TEXT("recommendations"), 0);
    
    // 初始化性能指標
    PerformanceMetrics.DecisionTime = 0.1f;
    PerformanceMetrics.ReactionTime = 0.2f;
    PerformanceMetrics.SuccessRate = 0.5f;
    PerformanceMetrics.EfficiencyIndex = 0.6f;
    PerformanceMetrics.AdaptabilityIndex = 0.4f;
    PerformanceMetrics.LearningIndex = 0.3f;
    PerformanceMetrics.TacticalIndex = 0.5f;
    PerformanceMetrics.CoordinationIndex = 0.4f;
    PerformanceMetrics.InnovationIndex = 0.2f;
    PerformanceMetrics.ResourceUsage = 0.3f;
    PerformanceMetrics.ComputationalComplexity = 0.5f;
    PerformanceMetrics.MemoryUsage = 0.2f;
    
    // 創建基礎決策節點
    CreateBasicDecisionNodes();
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingCombatAI initialized"));
}

void AMingCombatAI::ShutdownCombatAI()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 清理所有單位
    ControlledUnits.Empty();
    DecisionNodes.Empty();
    TacticalAnalyses.Empty();
    LearningData.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingCombatAI shutdown"));
}

void AMingCombatAI::SetAIDifficultyLevel(EAIDifficultyLevel NewDifficultyLevel)
{
    DifficultyLevel = NewDifficultyLevel;
    
    // 根據難度調整AI配置
    switch (DifficultyLevel)
    {
    case EAIDifficultyLevel::VeryEasy:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(0.5f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.5f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(0.6f));
        break;
    case EAIDifficultyLevel::Easy:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(0.7f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.4f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(0.7f));
        break;
    case EAIDifficultyLevel::Normal:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(1.0f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.3f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(0.8f));
        break;
    case EAIDifficultyLevel::Hard:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(1.3f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.2f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(0.9f));
        break;
    case EAIDifficultyLevel::VeryHard:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(1.5f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.15f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(0.95f));
        break;
    case EAIDifficultyLevel::Insane:
        AIConfiguration.Add(TEXT("decision_frequency"), FString::SanitizeFloat(2.0f));
        AIConfiguration.Add(TEXT("reaction_time"), FString::SanitizeFloat(0.1f));
        AIConfiguration.Add(TEXT("accuracy"), FString::SanitizeFloat(1.0f));
        break;
    case EAIDifficultyLevel::Adaptive:
    case EAIDifficultyLevel::Dynamic:
    case EAIDifficultyLevel::Learning:
        // 動態調整
        break;
    default:
        break;
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI difficulty level set to: %s"), *GetDifficultyDescription(DifficultyLevel));
}

void AMingCombatAI::SetAITacticalType(EAITacticalType NewTacticalType)
{
    TacticalType = NewTacticalType;
    
    // 根據戰術類型調整決策權重
    AdjustTacticalWeights();
    
    UE_LOG(LogTemp, Log, TEXT("AI tactical type set to: %s"), *GetTacticalDescription(TacticalType));
}

void AMingCombatAI::SetAILearningType(EAILearningType NewLearningType)
{
    LearningType = NewLearningType;
    
    // 根據學習類型調整學習參數
    AdjustLearningParameters();
    
    UE_LOG(LogTemp, Log, TEXT("AI learning type set to: %s"), *GetLearningDescription(LearningType));
}

bool AMingCombatAI::AddControlledUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        UE_LOG(LogTemp, Error, TEXT("CombatAI not initialized or invalid unit"));
        return false;
    }

    if (ControlledUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit already controlled by AI"));
        return false;
    }

    // 添加到受控列表
    ControlledUnits.Add(Unit);
    
    // 更新統計
    int32* Count = AIStatistics.Find(TEXT("controlled_units"));
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        AIStatistics.Add(TEXT("controlled_units"), 1);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit added to AI control: %s"), *Unit->GetName());
    
    return true;
}

bool AMingCombatAI::RemoveControlledUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        return false;
    }

    if (!ControlledUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit not controlled by AI"));
        return false;
    }

    // 從受控列表移除
    ControlledUnits.Remove(Unit);
    
    // 更新統計
    int32* Count = AIStatistics.Find(TEXT("controlled_units"));
    if (Count && *Count > 0)
    {
        (*Count)--;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit removed from AI control: %s"), *Unit->GetName());
    
    return true;
}

TArray<AMingTacticalUnit*> AMingCombatAI::GetControlledUnits() const
{
    return ControlledUnits;
}

bool AMingCombatAI::ExecuteAIDecision(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        return false;
    }

    if (!ControlledUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit not controlled by AI: %s"), *Unit->GetName());
        return false;
    }

    float StartTime = FPlatformTime::Seconds();
    
    // 執行決策樹
    bool bSuccess = ExecuteDecisionTree(Unit);
    
    // 執行行為樹
    if (bSuccess)
    {
        bSuccess = ExecuteBehaviorTree(Unit);
    }
    
    // 更新性能指標
    float EndTime = FPlatformTime::Seconds();
    PerformanceMetrics.DecisionTime = EndTime - StartTime;
    
    // 更新統計
    int32* TotalDecisions = AIStatistics.Find(TEXT("total_decisions"));
    if (TotalDecisions)
    {
        (*TotalDecisions)++;
    }
    
    if (bSuccess)
    {
        int32* SuccessfulDecisions = AIStatistics.Find(TEXT("successful_decisions"));
        if (SuccessfulDecisions)
        {
            (*SuccessfulDecisions)++;
        }
    }
    else
    {
        int32* FailedDecisions = AIStatistics.Find(TEXT("failed_decisions"));
        if (FailedDecisions)
        {
            (*FailedDecisions)++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI decision executed for unit %s: %s"), 
        *Unit->GetName(), bSuccess ? TEXT("Success") : TEXT("Failed"));
    
    return bSuccess;
}

FAITacticalAnalysis AMingCombatAI::ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units)
{
    FAITacticalAnalysis Analysis;
    Analysis.AnalysisID = FString::Printf(TEXT("analysis_%d"), FDateTime::Now().GetMillisecond());
    Analysis.AnalysisTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized || Units.Num() == 0)
    {
        Analysis.Confidence = 0.0f;
        return Analysis;
    }

    UE_LOG(LogTemp, Log, TEXT("Executing tactical analysis for %d units"), Units.Num());

    // 計算各種評估
    Analysis.ThreatAssessment = CalculateThreatAssessment(Units);
    Analysis.OpportunityAssessment = CalculateOpportunityAssessment(Units);
    Analysis.AdvantageAssessment = CalculateAdvantageAssessment(Units, TArray<AMingTacticalUnit*>());
    Analysis.DisadvantageAssessment = CalculateDisadvantageAssessment(Units, TArray<AMingTacticalUnit*>());
    
    // 生成戰術建議
    Analysis.TacticalRecommendations = GenerateTacticalRecommendationsInternal(Units);
    
    // 評估風險
    Analysis.RiskAssessments = AssessRisks(Units);
    
    // 識別機會
    Analysis.OpportunityIdentifications = IdentifyOpportunities(Units);
    
    // 預測敵方行動
    Analysis.EnemyPredictions = PredictEnemyActionsInternal(Units);
    
    // 計算戰術分數
    Analysis.TacticalScore = CalculateTacticalScore(Units);
    
    // 計算置信度
    Analysis.Confidence = CalculateAnalysisConfidence(Units);
    
    // 添加到分析列表
    TacticalAnalyses.Add(Analysis);
    
    // 更新統計
    int32* Count = AIStatistics.Find(TEXT("tactical_analyses"));
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        AIStatistics.Add(TEXT("tactical_analyses"), 1);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Tactical analysis completed: Score=%.2f, Confidence=%.2f"), 
        Analysis.TacticalScore, Analysis.Confidence);
    
    return Analysis;
}

bool AMingCombatAI::ExecuteAILearning(const FAILearningData& LearningData)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("CombatAI not initialized"));
        return false;
    }

    UE_LOG(LogTemp, Log, TEXT("Executing AI learning: %s"), *GetLearningDescription(LearningData.LearningType));

    bool bSuccess = false;
    
    // 根據學習類型執行相應的學習算法
    switch (LearningData.LearningType)
    {
    case EAILearningType::Reinforcement:
        bSuccess = ExecuteReinforcementLearning(LearningData);
        break;
    case EAILearningType::Supervised:
        bSuccess = ExecuteSupervisedLearning(LearningData);
        break;
    case EAILearningType::Unsupervised:
        bSuccess = ExecuteUnsupervisedLearning(LearningData);
        break;
    case EAILearningType::Evolutionary:
        bSuccess = ExecuteEvolutionaryLearning(LearningData);
        break;
    case EAILearningType::Neural:
        bSuccess = ExecuteNeuralLearning(LearningData);
        break;
    case EAILearningType::DecisionTree:
        bSuccess = ExecuteDecisionTreeLearning(LearningData);
        break;
    case EAILearningType::BehaviorTree:
        bSuccess = ExecuteBehaviorTreeLearning(LearningData);
        break;
    case EAILearningType::StateMachine:
        bSuccess = ExecuteStateMachineLearning(LearningData);
        break;
    case EAILearningType::Fuzzy:
        bSuccess = ExecuteFuzzyLearning(LearningData);
        break;
    case EAILearningType::Genetic:
        bSuccess = ExecuteGeneticLearning(LearningData);
        break;
    case EAILearningType::Swarm:
        bSuccess = ExecuteSwarmLearning(LearningData);
        break;
    case EAILearningType::Hybrid:
        bSuccess = ExecuteHybridLearning(LearningData);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown learning type: %d"), static_cast<int32>(LearningData.LearningType));
        break;
    }
    
    if (bSuccess)
    {
        // 更新學習數據
        UpdateLearningData(LearningData);
        
        // 更新統計
        int32* Count = AIStatistics.Find(TEXT("learning_iterations"));
        if (Count)
        {
            (*Count)++;
        }
        else
        {
            AIStatistics.Add(TEXT("learning_iterations"), 1);
        }
        
        // 計算收斂
        float Convergence = CalculateLearningConvergence();
        PerformanceMetrics.LearningIndex = Convergence;
        
        UE_LOG(LogTemp, Log, TEXT("AI learning completed successfully. Convergence: %.2f"), Convergence);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("AI learning failed"));
    }
    
    return bSuccess;
}

FAIDecisionNode AMingCombatAI::CreateDecisionNode(
    const FString& NodeName,
    EAIDecisionType DecisionType,
    const FString& ConditionExpression,
    float Weight,
    int32 Priority)
{
    FAIDecisionNode Node;
    Node.NodeID = FString::Printf(TEXT("node_%s_%d"), *NodeName, FDateTime::Now().GetMillisecond());
    Node.NodeName = NodeName;
    Node.DecisionType = DecisionType;
    Node.ConditionExpression = ConditionExpression;
    Node.Weight = Weight;
    Node.Priority = Priority;
    Node.ExecutionCount = 0;
    Node.SuccessCount = 0;
    Node.FailureCount = 0;
    Node.LastExecutionTime = 0.0f;
    Node.bIsEnabled = true;
    
    // 設置節點參數
    Node.NodeParameters.Add(TEXT("decision_type"), UEnum::GetValueAsString(DecisionType));
    Node.NodeParameters.Add(TEXT("weight"), FString::SanitizeFloat(Weight));
    Node.NodeParameters.Add(TEXT("priority"), FString::FromInt(Priority));
    
    return Node;
}

bool AMingCombatAI::AddDecisionNode(const FAIDecisionNode& Node)
{
    if (!bIsInitialized)
    {
        UE_LOG(LogTemp, Error, TEXT("CombatAI not initialized"));
        return false;
    }

    if (Node.NodeID.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid decision node ID"));
        return false;
    }

    // 添加到決策節點映射
    DecisionNodes.Add(Node.NodeID, Node);
    
    UE_LOG(LogTemp, Log, TEXT("Decision node added: %s"), *Node.NodeName);
    
    return true;
}

bool AMingCombatAI::RemoveDecisionNode(const FString& NodeID)
{
    if (!bIsInitialized)
    {
        return false;
    }

    if (!DecisionNodes.Contains(NodeID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Decision node not found: %s"), *NodeID);
        return false;
    }

    // 從決策節點映射移除
    DecisionNodes.Remove(NodeID);
    
    UE_LOG(LogTemp, Log, TEXT("Decision node removed: %s"), *NodeID);
    
    return true;
}

FAIDecisionNode AMingCombatAI::GetDecisionNode(const FString& NodeID) const
{
    const FAIDecisionNode* Node = DecisionNodes.Find(NodeID);
    return Node ? *Node : FAIDecisionNode();
}

TArray<FAIDecisionNode> AMingCombatAI::GetAllDecisionNodes() const
{
    TArray<FAIDecisionNode> Nodes;
    
    for (const auto& NodePair : DecisionNodes)
    {
        Nodes.Add(NodePair.Value);
    }
    
    return Nodes;
}

FAITacticalAnalysis AMingCombatAI::EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    FAITacticalAnalysis Analysis;
    Analysis.AnalysisID = FString::Printf(TEXT("situation_%d"), FDateTime::Now().GetMillisecond());
    Analysis.AnalysisTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        Analysis.Confidence = 0.0f;
        return Analysis;
    }

    UE_LOG(LogTemp, Log, TEXT("Evaluating tactical situation: %d friendly, %d enemy"), 
        FriendlyUnits.Num(), EnemyUnits.Num());

    // 計算各種評估
    Analysis.ThreatAssessment = CalculateThreatAssessment(EnemyUnits);
    Analysis.OpportunityAssessment = CalculateOpportunityAssessment(EnemyUnits);
    Analysis.AdvantageAssessment = CalculateAdvantageAssessment(FriendlyUnits, EnemyUnits);
    Analysis.DisadvantageAssessment = CalculateDisadvantageAssessment(FriendlyUnits, EnemyUnits);
    
    // 生成戰術建議
    TArray<AMingTacticalUnit*> AllUnits = FriendlyUnits;
    AllUnits.Append(EnemyUnits);
    Analysis.TacticalRecommendations = GenerateTacticalRecommendationsInternal(AllUnits);
    
    // 評估風險
    Analysis.RiskAssessments = AssessRisks(FriendlyUnits);
    
    // 識別機會
    Analysis.OpportunityIdentifications = IdentifyOpportunities(FriendlyUnits);
    
    // 預測敵方行動
    Analysis.EnemyPredictions = PredictEnemyActionsInternal(EnemyUnits);
    
    // 計算戰術分數
    Analysis.TacticalScore = CalculateTacticalScore(FriendlyUnits);
    
    // 計算置信度
    Analysis.Confidence = CalculateAnalysisConfidence(FriendlyUnits);
    
    // 添加到分析列表
    TacticalAnalyses.Add(Analysis);
    
    // 更新統計
    int32* Count = AIStatistics.Find(TEXT("tactical_analyses"));
    if (Count)
    {
        (*Count)++;
    }
    else
    {
        AIStatistics.Add(TEXT("tactical_analyses"), 1);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Tactical situation evaluation completed: Score=%.2f, Confidence=%.2f"), 
        Analysis.TacticalScore, Analysis.Confidence);
    
    return Analysis;
}

TArray<FString> AMingCombatAI::GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units)
{
    if (!bIsInitialized || Units.Num() == 0)
    {
        return TArray<FString>();
    }

    return GenerateTacticalRecommendationsInternal(Units);
}

TArray<FString> AMingCombatAI::PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (!bIsInitialized || EnemyUnits.Num() == 0)
    {
        return TArray<FString>();
    }

    return PredictEnemyActionsInternal(EnemyUnits);
}

float AMingCombatAI::CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (!Unit || EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }

    float ThreatLevel = 0.0f;
    
    // 基於敵方單位數量
    ThreatLevel += EnemyUnits.Num() * 0.1f;
    
    // 基於敵方單位距離
    for (AMingTacticalUnit* EnemyUnit : EnemyUnits)
    {
        if (EnemyUnit)
        {
            float Distance = FVector::Dist(Unit->GetActorLocation(), EnemyUnit->GetActorLocation());
            if (Distance < 1000.0f) // 1000單位威脅範圍
            {
                ThreatLevel += (1.0f - Distance / 1000.0f) * 0.2f;
            }
        }
    }
    
    // 基於單位狀態
    // 簡化實作：這裡應該檢查單位的生命值、彈藥等
    
    return FMath::Clamp(ThreatLevel, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (!Unit || EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }

    float OpportunityLevel = 0.0f;
    
    // 基於敵方單位弱點
    for (AMingTacticalUnit* EnemyUnit : EnemyUnits)
    {
        if (EnemyUnit)
        {
            float Distance = FVector::Dist(Unit->GetActorLocation(), EnemyUnit->GetActorLocation());
            
            // 在攻擊範圍內的敵方單位
            if (Distance < 500.0f) // 500單位攻擊範圍
            {
                OpportunityLevel += 0.3f;
            }
            
            // 低生命值的敵方單位
            // 簡化實作：這裡應該檢查敵方單位的生命值
            OpportunityLevel += 0.2f;
        }
    }
    
    return FMath::Clamp(OpportunityLevel, 0.0f, 1.0f);
}

bool AMingCombatAI::ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    if (!Unit || !TargetUnit)
    {
        return false;
    }

    // 檢查攻擊範圍
    float Distance = FVector::Dist(Unit->GetActorLocation(), TargetUnit->GetActorLocation());
    if (Distance > 500.0f) // 簡化攻擊範圍
    {
        // 需要移動到攻擊範圍
        return ExecuteMovementDecision(Unit, TargetUnit->GetActorLocation());
    }
    
    // 執行攻擊
    if (UnitController)
    {
        // 簡化實作：這裡應該調用單位控制器的攻擊功能
        UE_LOG(LogTemp, Log, TEXT("AI executing attack: %s -> %s"), *Unit->GetName(), *TargetUnit->GetName());
        return true;
    }
    
    return false;
}

bool AMingCombatAI::ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition)
{
    if (!Unit)
    {
        return false;
    }

    // 檢查是否在防禦位置
    float Distance = FVector::Dist(Unit->GetActorLocation(), DefensePosition);
    if (Distance > 100.0f) // 100單位防禦範圍
    {
        // 移動到防禦位置
        return ExecuteMovementDecision(Unit, DefensePosition);
    }
    
    // 執行防禦
    if (UnitController)
    {
        // 簡化實作：這裡應該調用單位控制器的防禦功能
        UE_LOG(LogTemp, Log, TEXT("AI executing defense: %s at %s"), *Unit->GetName(), *DefensePosition.ToString());
        return true;
    }
    
    return false;
}

bool AMingCombatAI::ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition)
{
    if (!Unit)
    {
        return false;
    }

    // 執行移動
    if (UnitController)
    {
        // 簡化實作：這裡應該調用單位控制器的移動功能
        UE_LOG(LogTemp, Log, TEXT("AI executing movement: %s -> %s"), *Unit->GetName(), *TargetPosition.ToString());
        
        // 直接設置位置（簡化實作）
        Unit->SetActorLocation(TargetPosition);
        return true;
    }
    
    return false;
}

bool AMingCombatAI::ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition)
{
    if (!Unit)
    {
        return false;
    }

    // 執行撤退
    if (UnitController)
    {
        // 簡化實作：這裡應該調用單位控制器的撤退功能
        UE_LOG(LogTemp, Log, TEXT("AI executing retreat: %s -> %s"), *Unit->GetName(), *RetreatPosition.ToString());
        
        // 直接設置位置（簡化實作）
        Unit->SetActorLocation(RetreatPosition);
        return true;
    }
    
    return false;
}

bool AMingCombatAI::ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    if (!Unit || !TargetUnit)
    {
        return false;
    }

    // 計算側翼位置
    FVector TargetLocation = TargetUnit->GetActorLocation();
    FVector CurrentLocation = Unit->GetActorLocation();
    FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
    
    // 計算側翼方向（垂直於攻擊方向）
    FVector FlankDirection = FVector(Direction.Y, -Direction.X, 0.0f).GetSafeNormal();
    FVector FlankPosition = TargetLocation + FlankDirection * 300.0f; // 300單位側翼距離
    
    // 執行側翼移動
    return ExecuteMovementDecision(Unit, FlankPosition);
}

bool AMingCombatAI::ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits)
{
    if (!Unit || TargetUnits.Num() == 0)
    {
        return false;
    }

    // 計算伏擊位置
    FVector AmbushPosition = FVector::ZeroVector;
    for (AMingTacticalUnit* TargetUnit : TargetUnits)
    {
        if (TargetUnit)
        {
            AmbushPosition += TargetUnit->GetActorLocation();
        }
    }
    
    if (TargetUnits.Num() > 0)
    {
        AmbushPosition /= TargetUnits.Num();
        
        // 在目標附近設置伏擊位置
        FVector RandomOffset = FVector(FMath::FRandRange(-200.0f, 200.0f), FMath::FRandRange(-200.0f, 200.0f), 0.0f);
        AmbushPosition += RandomOffset;
        
        // 執行伏擊移動
        return ExecuteMovementDecision(Unit, AmbushPosition);
    }
    
    return false;
}

bool AMingCombatAI::ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    if (!Unit || !TargetUnit)
    {
        return false;
    }

    // 計算支援位置
    FVector TargetLocation = TargetUnit->GetActorLocation();
    FVector SupportPosition = TargetLocation + FVector(100.0f, 100.0f, 0.0f); // 100單位支援距離
    
    // 執行支援移動
    return ExecuteMovementDecision(Unit, SupportPosition);
}

bool AMingCombatAI::ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType)
{
    if (Units.Num() == 0)
    {
        return false;
    }

    bool bSuccess = true;
    
    for (AMingTacticalUnit* Unit : Units)
    {
        if (!Unit)
        {
            bSuccess = false;
            continue;
        }
        
        // 根據決策類型執行相應的協調決策
        switch (DecisionType)
        {
        case EAIDecisionType::Attack:
            // 簡化實作：攻擊最近的敵方單位
            if (ControlledUnits.Num() > 0)
            {
                bSuccess &= ExecuteAttackDecision(Unit, ControlledUnits[0]);
            }
            break;
        case EAIDecisionType::Defense:
            // 防禦當前位置
            bSuccess &= ExecuteDefenseDecision(Unit, Unit->GetActorLocation());
            break;
        case EAIDecisionType::Movement:
            // 移動到隊形中心
            bSuccess &= ExecuteMovementDecision(Unit, CalculateFormationCenter(Units));
            break;
        case EAIDecisionType::Retreat:
            // 撤退到安全位置
            bSuccess &= ExecuteRetreatDecision(Unit, CalculateRetreatPosition(Units));
            break;
        default:
            bSuccess &= ExecuteAIDecision(Unit);
            break;
        }
    }
    
    return bSuccess;
}

bool AMingCombatAI::ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData)
{
    if (!Unit)
    {
        return false;
    }

    // 執行學習決策
    bool bSuccess = ExecuteAILearning(LearningData);
    
    if (bSuccess)
    {
        // 根據學習結果調整決策
        AdjustDecisionBasedOnLearning(Unit, LearningData);
    }
    
    return bSuccess;
}

bool AMingCombatAI::ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData)
{
    if (!Unit || AdaptationData.Num() == 0)
    {
        return false;
    }

    // 執行適應決策
    AdaptToTacticalChanges();
    
    // 根據適應數據調整行為
    AdjustBehaviorBasedOnAdaptation(Unit, AdaptationData);
    
    return true;
}

void AMingCombatAI::AdjustAIDifficulty(float PerformanceFactor)
{
    if (!bIsInitialized)
    {
        return;
    }

    // 根據性能因子調整難度
    if (PerformanceFactor > 0.8f && DifficultyLevel < EAIDifficultyLevel::Insane)
    {
        // 提升難度
        switch (DifficultyLevel)
        {
        case EAIDifficultyLevel::VeryEasy:
            SetAIDifficultyLevel(EAIDifficultyLevel::Easy);
            break;
        case EAIDifficultyLevel::Easy:
            SetAIDifficultyLevel(EAIDifficultyLevel::Normal);
            break;
        case EAIDifficultyLevel::Normal:
            SetAIDifficultyLevel(EAIDifficultyLevel::Hard);
            break;
        case EAIDifficultyLevel::Hard:
            SetAIDifficultyLevel(EAIDifficultyLevel::VeryHard);
            break;
        case EAIDifficultyLevel::VeryHard:
            SetAIDifficultyLevel(EAIDifficultyLevel::Insane);
            break;
        default:
            break;
        }
    }
    else if (PerformanceFactor < 0.3f && DifficultyLevel > EAIDifficultyLevel::VeryEasy)
    {
        // 降低難度
        switch (DifficultyLevel)
        {
        case EAIDifficultyLevel::Insane:
            SetAIDifficultyLevel(EAIDifficultyLevel::VeryHard);
            break;
        case EAIDifficultyLevel::VeryHard:
            SetAIDifficultyLevel(EAIDifficultyLevel::Hard);
            break;
        case EAIDifficultyLevel::Hard:
            SetAIDifficultyLevel(EAIDifficultyLevel::Normal);
            break;
        case EAIDifficultyLevel::Normal:
            SetAIDifficultyLevel(EAIDifficultyLevel::Easy);
            break;
        case EAIDifficultyLevel::Easy:
            SetAIDifficultyLevel(EAIDifficultyLevel::VeryEasy);
            break;
        default:
            break;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI difficulty adjusted based on performance factor: %.2f"), PerformanceFactor);
}

FAIPerformanceMetrics AMingCombatAI::GetAIPerformanceMetrics() const
{
    return PerformanceMetrics;
}

TMap<FString, int32> AMingCombatAI::GetAIStatistics() const
{
    return AIStatistics;
}

TMap<FString, FString> AMingCombatAI::GetAIConfiguration() const
{
    return AIConfiguration;
}

bool AMingCombatAI::SetAIConfiguration(const TMap<FString, FString>& Configuration)
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 驗證配置
    if (!ValidateAIConfiguration())
    {
        UE_LOG(LogTemp, Error, TEXT("Invalid AI configuration"));
        return false;
    }

    // 設置配置
    AIConfiguration = Configuration;
    
    // 根據配置調整AI行為
    AdjustAIBehaviorBasedOnConfiguration();
    
    UE_LOG(LogTemp, Log, TEXT("AI configuration updated"));
    
    return true;
}

void AMingCombatAI::ResetAISystem()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 清理所有數據
    ControlledUnits.Empty();
    DecisionNodes.Empty();
    TacticalAnalyses.Empty();
    LearningData.Empty();
    
    // 重置統計
    AIStatistics.Empty();
    
    // 重置性能指標
    PerformanceMetrics = FAIPerformanceMetrics();
    
    // 重新初始化
    InitializeCombatAI();
    
    UE_LOG(LogTemp, Log, TEXT("AI system reset"));
}

bool AMingCombatAI::BackupAIData(const FString& BackupPath)
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 簡化實作：記錄備份操作
    UE_LOG(LogTemp, Log, TEXT("AI data would be backed up to: %s"), *BackupPath);
    return true;
}

bool AMingCombatAI::RestoreAIData(const FString& BackupPath)
{
    if (!bIsInitialized)
    {
        return false;
    }

    // 簡化實作：記錄恢復操作
    UE_LOG(LogTemp, Log, TEXT("AI data would be restored from: %s"), *BackupPath);
    return true;
}

// === 內部方法實現 ===

void AMingCombatAI::CreateBasicDecisionNodes()
{
    // 創建基礎決策節點
    FAIDecisionNode AttackNode = CreateDecisionNode(TEXT("Attack"), EAIDecisionType::Attack, TEXT("threat > 0.5"), 1.0f, 1);
    AddDecisionNode(AttackNode);
    
    FAIDecisionNode DefenseNode = CreateDecisionNode(TEXT("Defense"), EAIDecisionType::Defense, TEXT("threat > 0.7"), 0.8f, 2);
    AddDecisionNode(DefenseNode);
    
    FAIDecisionNode MovementNode = CreateDecisionNode(TEXT("Movement"), EAIDecisionType::Movement, TEXT("opportunity > 0.3"), 0.6f, 3);
    AddDecisionNode(MovementNode);
    
    FAIDecisionNode RetreatNode = CreateDecisionNode(TEXT("Retreat"), EAIDecisionType::Retreat, TEXT("health < 0.3"), 0.9f, 1);
    AddDecisionNode(RetreatNode);
    
    FAIDecisionNode SupportNode = CreateDecisionNode(TEXT("Support"), EAIDecisionType::Support, TEXT("ally_needs_help"), 0.7f, 2);
    AddDecisionNode(SupportNode);
    
    FAIDecisionNode FlankNode = CreateDecisionNode(TEXT("Flank"), EAIDecisionType::Flank, TEXT("enemy_flankable"), 0.8f, 2);
    AddDecisionNode(FlankNode);
    
    FAIDecisionNode AmbushNode = CreateDecisionNode(TEXT("Ambush"), EAIDecisionType::Ambush, TEXT("enemy_unaware"), 0.9f, 1);
    AddDecisionNode(AmbushNode);
    
    FAIDecisionNode ScoutNode = CreateDecisionNode(TEXT("Scout"), EAIDecisionType::Scout, TEXT("area_unknown"), 0.5f, 4);
    AddDecisionNode(ScoutNode);
    
    FAIDecisionNode PatrolNode = CreateDecisionNode(TEXT("Patrol"), EAIDecisionType::Patrol, TEXT("no_immediate_threat"), 0.3f, 5);
    AddDecisionNode(PatrolNode);
    
    FAIDecisionNode GuardNode = CreateDecisionNode(TEXT("Guard"), EAIDecisionType::Guard, TEXT("strategic_point"), 0.6f, 3);
    AddDecisionNode(GuardNode);
}

bool AMingCombatAI::ExecuteDecisionTree(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    // 選擇最佳決策
    FAIDecisionNode BestDecision = SelectBestDecision(Unit);
    
    if (BestDecision.NodeID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("No suitable decision found for unit: %s"), *Unit->GetName());
        return false;
    }
    
    // 執行決策節點
    bool bSuccess = ExecuteDecisionNode(BestDecision, Unit);
    
    // 更新統計
    UpdateDecisionNodeStatistics(BestDecision.NodeID, bSuccess);
    
    return bSuccess;
}

bool AMingCombatAI::ExecuteBehaviorTree(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    // 簡化實作：執行基本行為樹
    // 1. 檢查生存狀態
    if (!IsUnitAlive(Unit))
    {
        return false;
    }
    
    // 2. 檢查威脅
    float ThreatLevel = CalculateThreatLevel(Unit, GetEnemyUnits(Unit));
    
    // 3. 根據威脅級別執行相應行為
    if (ThreatLevel > 0.8f)
    {
        // 高威脅：撤退或防禦
        return ExecuteRetreatDecision(Unit, CalculateRetreatPosition({Unit}));
    }
    else if (ThreatLevel > 0.5f)
    {
        // 中威脅：攻擊或防禦
        AMingTacticalUnit* Target = FindBestTarget(Unit);
        if (Target)
        {
            return ExecuteAttackDecision(Unit, Target);
        }
        else
        {
            return ExecuteDefenseDecision(Unit, Unit->GetActorLocation());
        }
    }
    else if (ThreatLevel > 0.2f)
    {
        // 低威脅：巡邏或偵察
        return ExecutePatrolDecision(Unit);
    }
    else
    {
        // 無威脅：守衛或待命
        return ExecuteGuardDecision(Unit);
    }
}

bool AMingCombatAI::EvaluateDecisionCondition(const FString& Condition, AMingTacticalUnit* Unit)
{
    if (!Unit || Condition.IsEmpty())
    {
        return false;
    }

    // 簡化實作：解析條件表達式
    if (Condition.Contains(TEXT("threat")))
    {
        float ThreatLevel = CalculateThreatLevel(Unit, GetEnemyUnits(Unit));
        return ThreatLevel > 0.5f;
    }
    else if (Condition.Contains(TEXT("opportunity")))
    {
        float OpportunityLevel = CalculateOpportunityLevel(Unit, GetEnemyUnits(Unit));
        return OpportunityLevel > 0.3f;
    }
    else if (Condition.Contains(TEXT("health")))
    {
        // 簡化：假設健康狀態
        return true;
    }
    else if (Condition.Contains(TEXT("ally_needs_help")))
    {
        // 簡化：檢查友軍是否需要幫助
        return false;
    }
    else if (Condition.Contains(TEXT("enemy_flankable")))
    {
        // 簡化：檢查敵方是否可以被側翼攻擊
        return true;
    }
    else if (Condition.Contains(TEXT("enemy_unaware")))
    {
        // 簡化：檢查敵方是否沒有察覺
        return false;
    }
    else if (Condition.Contains(TEXT("area_unknown")))
    {
        // 簡化：檢查區域是否未知
        return false;
    }
    else if (Condition.Contains(TEXT("no_immediate_threat")))
    {
        float ThreatLevel = CalculateThreatLevel(Unit, GetEnemyUnits(Unit));
        return ThreatLevel < 0.2f;
    }
    else if (Condition.Contains(TEXT("strategic_point")))
    {
        // 簡化：檢查是否在戰略要點
        return true;
    }
    
    return false;
}

float AMingCombatAI::CalculateDecisionWeight(const FAIDecisionNode& Node, AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return 0.0f;
    }

    float Weight = Node.Weight;
    
    // 根據成功率調整權重
    if (Node.ExecutionCount > 0)
    {
        float SuccessRate = (float)Node.SuccessCount / Node.ExecutionCount;
        Weight *= (0.5f + SuccessRate); // 成功率越高權重越大
    }
    
    // 根據戰術類型調整權重
    switch (TacticalType)
    {
    case EAITacticalType::Aggressive:
        if (Node.DecisionType == EAIDecisionType::Attack || Node.DecisionType == EAIDecisionType::Flank)
        {
            Weight *= 1.5f;
        }
        break;
    case EAITacticalType::Defensive:
        if (Node.DecisionType == EAIDecisionType::Defense || Node.DecisionType == EAIDecisionType::Retreat)
        {
            Weight *= 1.5f;
        }
        break;
    case EAITacticalType::Balanced:
        // 保持原始權重
        break;
    case EAITacticalType::Cautious:
        if (Node.DecisionType == EAIDecisionType::Retreat || Node.DecisionType == EAIDecisionType::Defense)
        {
            Weight *= 1.3f;
        }
        break;
    case EAITacticalType::Opportunistic:
        if (Node.DecisionType == EAIDecisionType::Attack || Node.DecisionType == EAIDecisionType::Ambush)
        {
            Weight *= 1.4f;
        }
        break;
    default:
        break;
    }
    
    return Weight;
}

FAIDecisionNode AMingCombatAI::SelectBestDecision(AMingTacticalUnit* Unit)
{
    FAIDecisionNode BestNode;
    float BestWeight = -1.0f;
    
    for (const auto& NodePair : DecisionNodes)
    {
        const FAIDecisionNode& Node = NodePair.Value;
        
        if (!Node.bIsEnabled)
        {
            continue;
        }
        
        // 檢查條件
        if (!EvaluateDecisionCondition(Node.ConditionExpression, Unit))
        {
            continue;
        }
        
        // 計算權重
        float Weight = CalculateDecisionWeight(Node, Unit);
        
        // 考慮優先級
        Weight *= (1.0f + Node.Priority * 0.1f);
        
        if (Weight > BestWeight)
        {
            BestWeight = Weight;
            BestNode = Node;
        }
    }
    
    return BestNode;
}

bool AMingCombatAI::ExecuteDecisionNode(const FAIDecisionNode& Node, AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    bool bSuccess = false;
    
    // 根據決策類型執行相應操作
    switch (Node.DecisionType)
    {
    case EAIDecisionType::Attack:
        {
            AMingTacticalUnit* Target = FindBestTarget(Unit);
            if (Target)
            {
                bSuccess = ExecuteAttackDecision(Unit, Target);
            }
        }
        break;
    case EAIDecisionType::Defense:
        bSuccess = ExecuteDefenseDecision(Unit, Unit->GetActorLocation());
        break;
    case EAIDecisionType::Movement:
        {
            FVector TargetPosition = CalculateBestMovementPosition(Unit);
            bSuccess = ExecuteMovementDecision(Unit, TargetPosition);
        }
        break;
    case EAIDecisionType::Retreat:
        {
            FVector RetreatPosition = CalculateRetreatPosition({Unit});
            bSuccess = ExecuteRetreatDecision(Unit, RetreatPosition);
        }
        break;
    case EAIDecisionType::Support:
        {
            AMingTacticalUnit* AllyNeedingHelp = FindAllyNeedingHelp(Unit);
            if (AllyNeedingHelp)
            {
                bSuccess = ExecuteSupportDecision(Unit, AllyNeedingHelp);
            }
        }
        break;
    case EAIDecisionType::Flank:
        {
            AMingTacticalUnit* Target = FindBestTarget(Unit);
            if (Target)
            {
                bSuccess = ExecuteFlankDecision(Unit, Target);
            }
        }
        break;
    case EAIDecisionType::Ambush:
        {
            TArray<AMingTacticalUnit*> Enemies = GetEnemyUnits(Unit);
            if (Enemies.Num() > 0)
            {
                bSuccess = ExecuteAmbushDecision(Unit, Enemies);
            }
        }
        break;
    case EAIDecisionType::Scout:
        bSuccess = ExecuteScoutDecision(Unit);
        break;
    case EAIDecisionType::Patrol:
        bSuccess = ExecutePatrolDecision(Unit);
        break;
    case EAIDecisionType::Guard:
        bSuccess = ExecuteGuardDecision(Unit);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown decision type: %d"), static_cast<int32>(Node.DecisionType));
        break;
    }
    
    // 更新執行時間
    FAIDecisionNode* MutableNode = DecisionNodes.Find(Node.NodeID);
    if (MutableNode)
    {
        MutableNode->LastExecutionTime = FPlatformTime::Seconds();
        MutableNode->ExecutionCount++;
    }
    
    return bSuccess;
}

void AMingCombatAI::UpdateDecisionNodeStatistics(const FString& NodeID, bool bSuccess)
{
    FAIDecisionNode* Node = DecisionNodes.Find(NodeID);
    if (Node)
    {
        if (bSuccess)
        {
            Node->SuccessCount++;
        }
        else
        {
            Node->FailureCount++;
        }
    }
}

float AMingCombatAI::CalculateTacticalScore(const TArray<AMingTacticalUnit*>& Units)
{
    if (Units.Num() == 0)
    {
        return 0.0f;
    }

    float Score = 0.0f;
    
    // 基於單位數量
    Score += Units.Num() * 0.1f;
    
    // 基於單位分布
    float Spread = CalculateUnitSpread(Units);
    Score += (1.0f - Spread) * 0.2f; // 分布越集中分數越高
    
    // 基於單位健康狀態
    float AverageHealth = CalculateAverageHealth(Units);
    Score += AverageHealth * 0.3f;
    
    // 基於戰術位置
    float TacticalPositionScore = CalculateTacticalPositionScore(Units);
    Score += TacticalPositionScore * 0.4f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateThreatAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }

    float ThreatAssessment = 0.0f;
    
    // 基於敵方單位數量
    ThreatAssessment += EnemyUnits.Num() * 0.1f;
    
    // 基於敵方單位類型
    for (AMingTacticalUnit* EnemyUnit : EnemyUnits)
    {
        if (EnemyUnit)
        {
            // 簡化實作：基於單位類型評估威脅
            ThreatAssessment += 0.1f;
        }
    }
    
    return FMath::Clamp(ThreatAssessment, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateOpportunityAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }

    float OpportunityAssessment = 0.0f;
    
    // 基於敵方單位弱點
    for (AMingTacticalUnit* EnemyUnit : EnemyUnits)
    {
        if (EnemyUnit)
        {
            // 簡化實作：檢查敵方單位是否有弱點
            OpportunityAssessment += 0.1f;
        }
    }
    
    return FMath::Clamp(OpportunityAssessment, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateAdvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    if (FriendlyUnits.Num() == 0)
    {
        return 0.0f;
    }

    float AdvantageAssessment = 0.0f;
    
    // 基於單位數量對比
    float UnitRatio = (float)FriendlyUnits.Num() / (EnemyUnits.Num() + 1);
    AdvantageAssessment += FMath::Clamp(UnitRatio - 1.0f, -1.0f, 1.0f) * 0.3f;
    
    // 基於單位質量對比
    float QualityRatio = CalculateUnitQualityRatio(FriendlyUnits, EnemyUnits);
    AdvantageAssessment += QualityRatio * 0.4f;
    
    // 基於位置優勢
    float PositionAdvantage = CalculatePositionAdvantage(FriendlyUnits, EnemyUnits);
    AdvantageAssessment += PositionAdvantage * 0.3f;
    
    return FMath::Clamp(AdvantageAssessment, -1.0f, 1.0f);
}

float AMingCombatAI::CalculateDisadvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    // 劣勢評估是優勢評估的負值
    return -CalculateAdvantageAssessment(FriendlyUnits, EnemyUnits);
}

TArray<FString> AMingCombatAI::GenerateTacticalRecommendationsInternal(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FString> Recommendations;
    
    if (Units.Num() == 0)
    {
        return Recommendations;
    }
    
    // 基於當前局勢生成建議
    float ThreatLevel = CalculateThreatAssessment(GetEnemyUnits(Units[0]));
    
    if (ThreatLevel > 0.8f)
    {
        Recommendations.Add(TEXT("建議採取防禦姿態"));
        Recommendations.Add(TEXT("考慮戰術性撤退"));
        Recommendations.Add(TEXT("尋求友軍支援"));
    }
    else if (ThreatLevel > 0.5f)
    {
        Recommendations.Add(TEXT("保持警戒狀態"));
        Recommendations.Add(TEXT("準備應對威脅"));
        Recommendations.Add(TEXT("加強防禦陣地"));
    }
    else if (ThreatLevel > 0.2f)
    {
        Recommendations.Add(TEXT("可以主動出擊"));
        Recommendations.Add(TEXT("擴大偵察範圍"));
        Recommendations.Add(TEXT("佔領戰略要點"));
    }
    else
    {
        Recommendations.Add(TEXT("保持常規巡邏"));
        Recommendations.Add(TEXT("加強戰術訓練"));
        Recommendations.Add(TEXT("準備應對突發情況"));
    }
    
    // 基於戰術類型生成建議
    switch (TacticalType)
    {
    case EAITacticalType::Aggressive:
        Recommendations.Add(TEXT("尋找攻擊機會"));
        Recommendations.Add(TEXT("保持主動姿態"));
        break;
    case EAITacticalType::Defensive:
        Recommendations.Add(TEXT("鞏固防禦陣地"));
        Recommendations.Add(TEXT("保持謹慎態度"));
        break;
    case EAITacticalType::Balanced:
        Recommendations.Add(TEXT("平衡攻防態度"));
        Recommendations.Add(TEXT("靈活應對局勢"));
        break;
    default:
        break;
    }
    
    return Recommendations;
}

TArray<FString> AMingCombatAI::PredictEnemyActionsInternal(const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    TArray<FString> Predictions;
    
    if (EnemyUnits.Num() == 0)
    {
        return Predictions;
    }
    
    // 基於敵方單位行為預測
    for (AMingTacticalUnit* EnemyUnit : EnemyUnits)
    {
        if (EnemyUnit)
        {
            // 簡化實作：基於單位類型預測行為
            Predictions.Add(FString::Printf(TEXT("%s 可能會發動攻擊"), *EnemyUnit->GetName()));
            Predictions.Add(FString::Printf(TEXT("%s 可能會尋求支援"), *EnemyUnit->GetName()));
        }
    }
    
    // 基於歷史數據預測
    if (LearningData.Num() > 0)
    {
        Predictions.Add(TEXT("基於歷史數據，敵方可能採取協調攻擊"));
        Predictions.Add(TEXT("敵方可能會嘗試側翼包抄"));
    }
    
    return Predictions;
}

bool AMingCombatAI::ExecuteReinforcementLearning(const FAILearningData& LearningData)
{
    // 簡化實作：強化學習
    UE_LOG(LogTemp, Log, TEXT("Executing reinforcement learning"));
    
    // 更新Q值（簡化）
    for (int32 i = 0; i < LearningData.InputData.Num() && i < LearningData.OutputData.Num(); ++i)
    {
        float QValue = LearningData.OutputData[i];
        float Reward = LearningData.RewardValue;
        float Penalty = LearningData.PenaltyValue;
        
        // Q學習公式：Q(s,a) = Q(s,a) + α * (r + γ * max(Q(s',a')) - Q(s,a))
        float LearningRate = 0.1f;
        float DiscountFactor = 0.9f;
        QValue += LearningRate * (Reward - Penalty + DiscountFactor * QValue - QValue);
        
        UE_LOG(LogTemp, Verbose, TEXT("Updated Q-value: %.2f"), QValue);
    }
    
    return true;
}

bool AMingCombatAI::ExecuteSupervisedLearning(const FAILearningData& LearningData)
{
    // 簡化實作：監督學習
    UE_LOG(LogTemp, Log, TEXT("Executing supervised learning"));
    
    // 訓练簡單的分類器
    for (int32 i = 0; i < LearningData.InputData.Num() && i < LearningData.OutputData.Num(); ++i)
    {
        float Input = LearningData.InputData[i];
        float ExpectedOutput = LearningData.OutputData[i];
        
        // 簡化的梯度下降
        float LearningRate = 0.01f;
        float PredictedOutput = Input * 0.5f; // 簡化預測
        float Error = ExpectedOutput - PredictedOutput;
        float WeightUpdate = LearningRate * Error * Input;
        
        UE_LOG(LogTemp, Verbose, TEXT("Weight update: %.4f"), WeightUpdate);
    }
    
    return true;
}

bool AMingCombatAI::ExecuteUnsupervisedLearning(const FAILearningData& LearningData)
{
    // 簡化實作：無監督學習
    UE_LOG(LogTemp, Log, TEXT("Executing unsupervised learning"));
    
    // K-means聚類（簡化）
    TArray<float> ClusterCenters;
    ClusterCenters.Add(0.0f);
    ClusterCenters.Add(0.5f);
    ClusterCenters.Add(1.0f);
    
    for (float DataPoint : LearningData.InputData)
    {
        // 找到最近的聚類中心
        float MinDistance = FLT_MAX;
        int32 BestCluster = 0;
        
        for (int32 i = 0; i < ClusterCenters.Num(); ++i)
        {
            float Distance = FMath::Abs(DataPoint - ClusterCenters[i]);
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                BestCluster = i;
            }
        }
        
        // 更新聚類中心
        ClusterCenters[BestCluster] = (ClusterCenters[BestCluster] + DataPoint) / 2.0f;
    }
    
    return true;
}

bool AMingCombatAI::ExecuteEvolutionaryLearning(const FAILearningData& LearningData)
{
    // 簡化實作：進化學習
    UE_LOG(LogTemp, Log, TEXT("Executing evolutionary learning"));
    
    // 遺傳算法（簡化）
    TArray<float> Population;
    for (int32 i = 0; i < 10; ++i)
    {
        Population.Add(FMath::FRandRange(0.0f, 1.0f));
    }
    
    // 選擇、交叉、變異
    for (int32 Generation = 0; Generation < 10; ++Generation)
    {
        // 選擇最優個體
        Population.Sort([](const float& A, const float& B) { return A > B; });
        
        // 交叉
        for (int32 i = 0; i < 5; ++i)
        {
            Population[i + 5] = (Population[i] + Population[i + 1]) / 2.0f;
        }
        
        // 變異
        for (int32 i = 5; i < 10; ++i)
        {
            Population[i] += FMath::FRandRange(-0.1f, 0.1f);
            Population[i] = FMath::Clamp(Population[i], 0.0f, 1.0f);
        }
    }
    
    return true;
}

bool AMingCombatAI::ExecuteNeuralLearning(const FAILearningData& LearningData)
{
    // 簡化實作：神經網絡學習
    UE_LOG(LogTemp, Log, TEXT("Executing neural learning"));
    
    // 簡單的感知器
    TArray<float> Weights;
    for (int32 i = 0; i < LearningData.InputData.Num(); ++i)
    {
        Weights.Add(FMath::FRandRange(-1.0f, 1.0f));
    }
    
    // 前向傳播
    for (int32 Epoch = 0; Epoch < 100; ++Epoch)
    {
        for (int32 i = 0; i < LearningData.InputData.Num() && i < LearningData.OutputData.Num(); ++i)
        {
            float Input = LearningData.InputData[i];
            float ExpectedOutput = LearningData.OutputData[i];
            
            // 計算輸出
            float NetInput = 0.0f;
            for (int32 j = 0; j < Weights.Num(); ++j)
            {
                NetInput += Weights[j] * Input;
            }
            float Output = 1.0f / (1.0f + FMath::Exp(-NetInput)); // Sigmoid
            
            // 計算誤差
            float Error = ExpectedOutput - Output;
            
            // 反向傳播
            float LearningRate = 0.1f;
            for (int32 j = 0; j < Weights.Num(); ++j)
            {
                Weights[j] += LearningRate * Error * Input * Output * (1.0f - Output);
            }
        }
    }
    
    return true;
}

bool AMingCombatAI::ExecuteDecisionTreeLearning(const FAILearningData& LearningData)
{
    // 簡化實作：決策樹學習
    UE_LOG(LogTemp, Log, TEXT("Executing decision tree learning"));
    
    // ID3算法（簡化）
    // 計算信息增益
    float Entropy = 0.0f;
    for (float Output : LearningData.OutputData)
    {
        if (Output > 0.5f)
        {
            Entropy -= 0.5f * FMath::Loge(0.5f);
        }
        else
        {
            Entropy -= 0.5f * FMath::Loge(0.5f);
        }
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Entropy: %.4f"), Entropy);
    
    return true;
}

bool AMingCombatAI::ExecuteBehaviorTreeLearning(const FAILearningData& LearningData)
{
    // 簡化實作：行為樹學習
    UE_LOG(LogTemp, Log, TEXT("Executing behavior tree learning"));
    
    // 行為樹節點優化
    for (const auto& NodePair : DecisionNodes)
    {
        FAIDecisionNode& Node = DecisionNodes[NodePair.Key];
        
        // 基於成功率調整權重
        if (Node.ExecutionCount > 0)
        {
            float SuccessRate = (float)Node.SuccessCount / Node.ExecutionCount;
            Node.Weight = SuccessRate;
        }
    }
    
    return true;
}

bool AMingCombatAI::ExecuteStateMachineLearning(const FAILearningData& LearningData)
{
    // 簡化實作：狀態機學習
    UE_LOG(LogTemp, Log, TEXT("Executing state machine learning"));
    
    // 馬爾可夫鏈學習
    TMap<FString, TMap<FString, float>> TransitionProbabilities;
    
    // 計算轉移概率
    for (int32 i = 0; i < LearningData.InputData.Num() - 1; ++i)
    {
        FString CurrentState = FString::FromInt(FMath::RoundToInt(LearningData.InputData[i]));
        FString NextState = FString::FromInt(FMath::RoundToInt(LearningData.InputData[i + 1]));
        
        TMap<FString, float>& Transitions = TransitionProbabilities.FindOrAdd(CurrentState);
        Transitions.FindOrAdd(NextState, 0.0f)++;
        Transitions[NextState] += 1.0f;
    }
    
    // 正規化概率
    for (auto& TransitionPair : TransitionProbabilities)
    {
        float Total = 0.0f;
        for (const auto& ProbPair : TransitionPair.Value)
        {
            Total += ProbPair.Value;
        }
        
        for (auto& ProbPair : TransitionPair.Value)
        {
            ProbPair.Value /= Total;
        }
    }
    
    return true;
}

bool AMingCombatAI::ExecuteFuzzyLearning(const FAILearningData& LearningData)
{
    // 簡化實作：模糊學習
    UE_LOG(LogTemp, Log, TEXT("Executing fuzzy learning"));
    
    // 模糊邏輯規則
    TArray<float> MembershipFunctions;
    MembershipFunctions.Add(0.0f); // 低
    MembershipFunctions.Add(0.5f); // 中
    MembershipFunctions.Add(1.0f); // 高
    
    for (float DataPoint : LearningData.InputData)
    {
        // 計算隸屬度
        TArray<float> Memberships;
        for (float Center : MembershipFunctions)
        {
            float Membership = FMath::Exp(-FMath::Square(DataPoint - Center) / 0.1f);
            Memberships.Add(Membership);
        }
        
        // 模糊推理
        float FuzzyOutput = 0.0f;
        for (int32 i = 0; i < Memberships.Num(); ++i)
        {
            FuzzyOutput += Memberships[i] * MembershipFunctions[i];
        }
        
        UE_LOG(LogTemp, Verbose, TEXT("Fuzzy output: %.2f"), FuzzyOutput);
    }
    
    return true;
}

bool AMingCombatAI::ExecuteGeneticLearning(const FAILearningData& LearningData)
{
    // 簡化實作：遺傳學習
    return ExecuteEvolutionaryLearning(LearningData);
}

bool AMingCombatAI::ExecuteSwarmLearning(const FAILearningData& LearningData)
{
    // 簡化實作：群體學習
    UE_LOG(LogTemp, Log, TEXT("Executing swarm learning"));
    
    // 粒子群優化
    TArray<float> Particles;
    TArray<float> Velocities;
    TArray<float> BestPositions;
    
    for (int32 i = 0; i < 20; ++i)
    {
        Particles.Add(FMath::FRandRange(0.0f, 1.0f));
        Velocities.Add(FMath::FRandRange(-0.1f, 0.1f));
        BestPositions.Add(Particles[i]);
    }
    
    float GlobalBest = 0.0f;
    
    for (int32 Iteration = 0; Iteration < 50; ++Iteration)
    {
        for (int32 i = 0; i < Particles.Num(); ++i)
        {
            // 評估適應度
            float Fitness = EvaluateFitness(Particles[i]);
            
            // 更新個體最佳位置
            if (Fitness > EvaluateFitness(BestPositions[i]))
            {
                BestPositions[i] = Particles[i];
            }
            
            // 更新全局最佳位置
            if (Fitness > GlobalBest)
            {
                GlobalBest = Fitness;
            }
            
            // 更新速度和位置
            float InertiaWeight = 0.7f;
            float CognitiveWeight = 1.5f;
            float SocialWeight = 1.5f;
            
            Velocities[i] = InertiaWeight * Velocities[i] + 
                           CognitiveWeight * FMath::FRand() * (BestPositions[i] - Particles[i]) +
                           SocialWeight * FMath::FRand() * (GlobalBest - Particles[i]);
            
            Particles[i] += Velocities[i];
            Particles[i] = FMath::Clamp(Particles[i], 0.0f, 1.0f);
        }
    }
    
    return true;
}

bool AMingCombatAI::ExecuteHybridLearning(const FAILearningData& LearningData)
{
    // 簡化實作：混合學習
    UE_LOG(LogTemp, Log, TEXT("Executing hybrid learning"));
    
    // 結合多種學習方法
    bool bSuccess = true;
    
    bSuccess &= ExecuteReinforcementLearning(LearningData);
    bSuccess &= ExecuteSupervisedLearning(LearningData);
    bSuccess &= ExecuteUnsupervisedLearning(LearningData);
    
    return bSuccess;
}

void AMingCombatAI::UpdateLearningData(const FAILearningData& LearningData)
{
    // 添加到學習數據列表
    LearningData.Add(LearningData);
    
    // 限制學習數據數量
    if (LearningData.Num() > 1000)
    {
        LearningData.RemoveAt(0);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Learning data updated. Total samples: %d"), LearningData.Num());
}

float AMingCombatAI::CalculateLearningConvergence()
{
    if (LearningData.Num() < 10)
    {
        return 0.0f;
    }
    
    // 計算最近10次學習的平均成功率
    float TotalSuccess = 0.0f;
    int32 Count = 0;
    
    for (int32 i = LearningData.Num() - 10; i < LearningData.Num(); ++i)
    {
        const FAILearningData& Data = LearningData[i];
        if (Data.LearningCount > 0)
        {
            TotalSuccess += Data.SuccessRate;
            Count++;
        }
    }
    
    return Count > 0 ? TotalSuccess / Count : 0.0f;
}

void AMingCombatAI::OptimizeAIPerformance()
{
    // 優化決策節點
    OptimizeDecisionNodes();
    
    // 優化學習算法
    OptimizeLearningAlgorithms();
    
    // 優化記憶使用
    ManageMemoryUsage();
    
    UE_LOG(LogTemp, Log, TEXT("AI performance optimized"));
}

void AMingCombatAI::AdjustDecisionWeights()
{
    // 基於成功率調整決策權重
    for (auto& NodePair : DecisionNodes)
    {
        FAIDecisionNode& Node = NodePair.Value;
        
        if (Node.ExecutionCount > 0)
        {
            float SuccessRate = (float)Node.SuccessCount / Node.ExecutionCount;
            
            // 調整權重
            if (SuccessRate > 0.7f)
            {
                Node.Weight *= 1.1f; // 成功率高，增加權重
            }
            else if (SuccessRate < 0.3f)
            {
                Node.Weight *= 0.9f; // 成功率低，減少權重
            }
            
            Node.Weight = FMath::Clamp(Node.Weight, 0.1f, 2.0f);
        }
    }
}

void AMingCombatAI::AdaptToTacticalChanges()
{
    // 適應戰術變化
    float CurrentPerformance = EvaluateAIPerformance();
    
    if (CurrentPerformance < 0.5f)
    {
        // 性能不佳，調整戰術類型
        switch (TacticalType)
        {
        case EAITacticalType::Aggressive:
            SetAITacticalType(EAITacticalType::Balanced);
            break;
        case EAITacticalType::Balanced:
            SetAITacticalType(EAITacticalType::Defensive);
            break;
        case EAITacticalType::Defensive:
            SetAITacticalType(EAITacticalType::Cautious);
            break;
        default:
            break;
        }
    }
    else if (CurrentPerformance > 0.8f)
    {
        // 性能良好，可以更積極
        switch (TacticalType)
        {
        case EAITacticalType::Cautious:
            SetAITacticalType(EAITacticalType::Defensive);
            break;
        case EAITacticalType::Defensive:
            SetAITacticalType(EAITacticalType::Balanced);
            break;
        case EAITacticalType::Balanced:
            SetAITacticalType(EAITacticalType::Aggressive);
            break;
        default:
            break;
        }
    }
}

void AMingCombatAI::LearnPlayerBehavior()
{
    // 學習玩家行為模式
    // 簡化實作：記錄玩家決策模式
    
    UE_LOG(LogTemp, Log, TEXT("Learning player behavior patterns"));
}

TArray<FString> AMingCombatAI::PredictPlayerIntent()
{
    TArray<FString> Predictions;
    
    // 基於學習數據預測玩家意圖
    if (LearningData.Num() > 0)
    {
        Predictions.Add(TEXT("玩家可能準備發動攻擊"));
        Predictions.Add(TEXT("玩家可能會試試側翼包抄"));
        Predictions.Add(TEXT("玩家可能會尋求戰術優勢"));
    }
    
    return Predictions;
}

TArray<FString> AMingCombatAI::GenerateCounterStrategies()
{
    TArray<FString> Strategies;
    
    // 生成反制策略
    Strategies.Add(TEXT("加強防禦陣地"));
    Strategies.Add(TEXT("設置反伏擊陷阱"));
    Strategies.Add(TEXT("準備應對側翼攻擊"));
    Strategies.Add(TEXT("保持機動預備隊"));
    Strategies.Add(TEXT("監控關鍵戰術點"));
    
    return Strategies;
}

float AMingCombatAI::EvaluateAIPerformance()
{
    // 評估AI整體性能
    float Performance = 0.0f;
    
    // 基於成功率
    int32 TotalDecisions = AIStatistics.FindRef(TEXT("total_decisions"));
    int32 SuccessfulDecisions = AIStatistics.FindRef(TEXT("successful_decisions"));
    
    if (TotalDecisions > 0)
    {
        Performance += (float)SuccessfulDecisions / TotalDecisions * 0.4f;
    }
    
    // 基於學習效果
    float LearningConvergence = CalculateLearningConvergence();
    Performance += LearningConvergence * 0.3f;
    
    // 基於適應性
    Performance += PerformanceMetrics.AdaptabilityIndex * 0.2f;
    
    // 基於戰術表現
    Performance += PerformanceMetrics.TacticalIndex * 0.1f;
    
    return FMath::Clamp(Performance, 0.0f, 1.0f);
}

void AMingCombatAI::UpdatePerformanceMetrics()
{
    // 更新性能指標
    PerformanceMetrics.SuccessRate = EvaluateAIPerformance();
    PerformanceMetrics.AdaptabilityIndex = CalculateAdaptabilityIndex();
    PerformanceMetrics.LearningIndex = CalculateLearningConvergence();
    PerformanceMetrics.TacticalIndex = CalculateTacticalIndex();
    PerformanceMetrics.CoordinationIndex = CalculateCoordinationIndex();
    PerformanceMetrics.InnovationIndex = CalculateInnovationIndex();
    PerformanceMetrics.ResourceUsage = CalculateResourceUsage();
    PerformanceMetrics.ComputationalComplexity = CalculateComplexity();
    PerformanceMetrics.MemoryUsage = CalculateMemoryUsage();
}

void AMingCombatAI::LogAIEvent(const FString& EventType, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[CombatAI] %s: %s"), *EventType, *Details);
}

FString AMingCombatAI::GenerateAIReport() const
{
    FString Report = TEXT("=== 戰鬥AI報告 ===\n");
    
    Report += FString::Printf(TEXT("AI難度: %s\n"), *GetDifficultyDescription(DifficultyLevel));
    Report += FString::Printf(TEXT("戰術類型: %s\n"), *GetTacticalDescription(TacticalType));
    Report += FString::Printf(TEXT("學習類型: %s\n"), *GetLearningDescription(LearningType));
    Report += FString::Printf(TEXT("受控單位數量: %d\n"), ControlledUnits.Num());
    Report += FString::Printf(TEXT("決策節點數量: %d\n"), DecisionNodes.Num());
    Report += FString::Printf(TEXT("學習數據數量: %d\n"), LearningData.Num());
    
    Report += TEXT("\n=== AI統計 ===\n");
    for (const auto& StatPair : AIStatistics)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *StatPair.Key, StatPair.Value);
    }
    
    Report += TEXT("\n=== 性能指標 ===\n");
    Report += FString::Printf(TEXT("決策時間: %.3f秒\n"), PerformanceMetrics.DecisionTime);
    Report += FString::Printf(TEXT("反應時間: %.3f秒\n"), PerformanceMetrics.ReactionTime);
    Report += FString::Printf(TEXT("成功率: %.1f%%\n"), PerformanceMetrics.SuccessRate * 100.0f);
    Report += FString::Printf(TEXT("效率指數: %.1f%%\n"), PerformanceMetrics.EfficiencyIndex * 100.0f);
    Report += FString::Printf(TEXT("適應性指數: %.1f%%\n"), PerformanceMetrics.AdaptabilityIndex * 100.0f);
    Report += FString::Printf(TEXT("學習指數: %.1f%%\n"), PerformanceMetrics.LearningIndex * 100.0f);
    Report += FString::Printf(TEXT("戰術指數: %.1f%%\n"), PerformanceMetrics.TacticalIndex * 100.0f);
    Report += FString::Printf(TEXT("協調指數: %.1f%%\n"), PerformanceMetrics.CoordinationIndex * 100.0f);
    Report += FString::Printf(TEXT("創新指數: %.1f%%\n"), PerformanceMetrics.InnovationIndex * 100.0f);
    Report += FString::Printf(TEXT("資源使用率: %.1f%%\n"), PerformanceMetrics.ResourceUsage * 100.0f);
    Report += FString::Printf(TEXT("計算複雜度: %.1f%%\n"), PerformanceMetrics.ComputationalComplexity * 100.0f);
    Report += FString::Printf(TEXT("記憶使用量: %.1f%%\n"), PerformanceMetrics.MemoryUsage * 100.0f);
    
    return Report;
}

bool AMingCombatAI::ValidateAIConfiguration() const
{
    // 驗證AI配置
    if (AIConfiguration.Num() == 0)
    {
        return false;
    }
    
    // 檢查必要配置項
    if (!AIConfiguration.Contains(TEXT("decision_frequency")) ||
        !AIConfiguration.Contains(TEXT("learning_rate")) ||
        !AIConfiguration.Contains(TEXT("adaptation_rate")))
    {
        return false;
    }
    
    return true;
}

FString AMingCombatAI::GetDecisionDescription(EAIDecisionType DecisionType) const
{
    switch (DecisionType)
    {
    case EAIDecisionType::Movement:
        return TEXT("移動");
    case EAIDecisionType::Attack:
        return TEXT("攻擊");
    case EAIDecisionType::Defense:
        return TEXT("防禦");
    case EAIDecisionType::Support:
        return TEXT("支援");
    case EAIDecisionType::Retreat:
        return TEXT("撤退");
    case EAIDecisionType::Flank:
        return TEXT("側翼攻擊");
    case EAIDecisionType::Ambush:
        return TEXT("伏擊");
    case EAIDecisionType::Capture:
        return TEXT("捕獲");
    case EAIDecisionType::Build:
        return TEXT("建設");
    case EAIDecisionType::Repair:
        return TEXT("修理");
    case EAIDecisionType::Heal:
        return TEXT("治療");
    case EAIDecisionType::Scout:
        return TEXT("偵察");
    case EAIDecisionType::Patrol:
        return TEXT("巡邏");
    case EAIDecisionType::Guard:
        return TEXT("守衛");
    case EAIDecisionType::Coordinate:
        return TEXT("協調");
    case EAIDecisionType::Learn:
        return TEXT("學習");
    case EAIDecisionType::Adapt:
        return TEXT("適應");
    default:
        return TEXT("未知決策");
    }
}

FString AMingCombatAI::GetTacticalDescription(EAITacticalType TacticalType) const
{
    switch (TacticalType)
    {
    case EAITacticalType::Aggressive:
        return TEXT("攻擊型");
    case EAITacticalType::Defensive:
        return TEXT("防禦型");
    case EAITacticalType::Balanced:
        return TEXT("平衡型");
    case EAITacticalType::Cautious:
        return TEXT("謹慎型");
    case EAITacticalType::Opportunistic:
        return TEXT("機會型");
    case EAITacticalType::Adaptive:
        return TEXT("適應型");
    case EAITacticalType::Reactive:
        return TEXT("反應型");
    case EAITacticalType::Proactive:
        return TEXT("主動型");
    case EAITacticalType::Strategic:
        return TEXT("戰略型");
    case EAITacticalType::Tactical:
        return TEXT("戰術型");
    case EAITacticalType::Guerrilla:
        return TEXT("游擊型");
    case EAITacticalType::Conventional:
        return TEXT("常規型");
    case EAITacticalType::Blitzkrieg:
        return TEXT("閃電戰");
    case EAITacticalType::Attrition:
        return TEXT("消耗戰");
    case EAITacticalType::Maneuver:
        return TEXT("機動戰");
    case EAITacticalType::Siege:
        return TEXT("圍攻戰");
    case EAITacticalType::Raiding:
        return TEXT("突襲戰");
    case EAITacticalType::Skirmish:
        return TEXT("遭遇戰");
    default:
        return TEXT("未知戰術");
    }
}

FString AMingCombatAI::GetDifficultyDescription(EAIDifficultyLevel DifficultyLevel) const
{
    switch (DifficultyLevel)
    {
    case EAIDifficultyLevel::VeryEasy:
        return TEXT("非常簡單");
    case EAIDifficultyLevel::Easy:
        return TEXT("簡單");
    case EAIDifficultyLevel::Normal:
        return TEXT("普通");
    case EAIDifficultyLevel::Hard:
        return TEXT("困難");
    case EAIDifficultyLevel::VeryHard:
        return TEXT("非常困難");
    case EAIDifficultyLevel::Insane:
        return TEXT("瘋狂");
    case EAIDifficultyLevel::Custom:
        return TEXT("自定義");
    case EAIDifficultyLevel::Adaptive:
        return TEXT("自適應");
    case EAIDifficultyLevel::Dynamic:
        return TEXT("動態");
    case EAIDifficultyLevel::Learning:
        return TEXT("學習型");
    default:
        return TEXT("未知難度");
    }
}

FString AMingCombatAI::GetLearningDescription(EAILearningType LearningType) const
{
    switch (LearningType)
    {
    case EAILearningType::Reinforcement:
        return TEXT("強化學習");
    case EAILearningType::Supervised:
        return TEXT("監督學習");
    case EAILearningType::Unsupervised:
        return TEXT("無監督學習");
    case EAILearningType::Evolutionary:
        return TEXT("進化學習");
    case EAILearningType::Neural:
        return TEXT("神經網絡學習");
    case EAILearningType::DecisionTree:
        return TEXT("決策樹學習");
    case EAILearningType::BehaviorTree:
        return TEXT("行為樹學習");
    case EAILearningType::StateMachine:
        return TEXT("狀態機學習");
    case EAILearningType::Fuzzy:
        return TEXT("模糊學習");
    case EAILearningType::Genetic:
        return TEXT("遺傳學習");
    case EAILearningType::Swarm:
        return TEXT("群體學習");
    case EAILearningType::Hybrid:
        return TEXT("混合學習");
    default:
        return TEXT("未知學習");
    }
}

// === 輔助方法實現 ===

float AMingCombatAI::CalculateComplexity() const
{
    float Complexity = 0.0f;
    
    // 基於決策節點數量
    Complexity += DecisionNodes.Num() * 0.01f;
    
    // 基於學習數據數量
    Complexity += LearningData.Num() * 0.001f;
    
    // 基於受控單位數量
    Complexity += ControlledUnits.Num() * 0.005f;
    
    return FMath::Clamp(Complexity, 0.0f, 1.0f);
}

void AMingCombatAI::ManageMemoryUsage()
{
    // 管理記憶使用
    if (LearningData.Num() > 1000)
    {
        // 移除最舊的學習數據
        LearningData.RemoveAt(0, LearningData.Num() - 1000);
    }
    
    if (TacticalAnalyses.Num() > 100)
    {
        // 移除最舊的戰術分析
        TacticalAnalyses.RemoveAt(0, TacticalAnalyses.Num() - 100);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Memory usage managed"));
}

void AMingCombatAI::OptimizeComputationalResources()
{
    // 優化計算資源
    // 移除不活躍的決策節點
    TArray<FString> NodesToRemove;
    
    for (const auto& NodePair : DecisionNodes)
    {
        const FAIDecisionNode& Node = NodePair.Value;
        
        if (Node.ExecutionCount == 0 || Node.SuccessRate < 0.1f)
        {
            NodesToRemove.Add(NodePair.Key);
        }
    }
    
    for (const FString& NodeID : NodesToRemove)
    {
        DecisionNodes.Remove(NodeID);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Computational resources optimized"));
}

void AMingCombatAI::HandleAIError(const FString& ErrorType, const FString& ErrorMessage)
{
    UE_LOG(LogTemp, Error, TEXT("[AI Error] %s: %s"), *ErrorType, *ErrorMessage);
    
    // 嘗試恢復
    RecoverAIState();
}

void AMingCombatAI::RecoverAIState()
{
    // 恢復AI狀態
    UE_LOG(LogTemp, Log, TEXT("Recovering AI state"));
    
    // 重置性能指標
    PerformanceMetrics = FAIPerformanceMetrics();
    
    // 重新初始化決策節點
    CreateBasicDecisionNodes();
}

void AMingCombatAI::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsInitialized)
    {
        return;
    }

    // 為所有受控單位執行AI決策
    for (AMingTacticalUnit* Unit : ControlledUnits)
    {
        if (Unit && IsUnitAlive(Unit))
        {
            ExecuteAIDecision(Unit);
        }
    }

    // 定期更新性能指標
    static float LastUpdateTime = 0.0f;
    float CurrentTime = FPlatformTime::Seconds();
    
    if (CurrentTime - LastUpdateTime > 1.0f) // 每秒更新一次
    {
        UpdatePerformanceMetrics();
        LastUpdateTime = CurrentTime;
    }
    
    // 定期優化AI性能
    static float LastOptimizationTime = 0.0f;
    
    if (CurrentTime - LastOptimizationTime > 10.0f) // 每10秒優化一次
    {
        OptimizeAIPerformance();
        LastOptimizationTime = CurrentTime;
    }
}

// === 輔助方法實現（續） ===

bool AMingCombatAI::IsUnitAlive(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return false;
    }
    
    // 簡化實作：檢查單位是否有效
    return Unit->IsValidLowLevel();
}

TArray<AMingTacticalUnit*> AMingCombatAI::GetEnemyUnits(AMingTacticalUnit* Unit) const
{
    TArray<AMingTacticalUnit*> EnemyUnits;
    
    // 簡化實作：返回空數組
    // 實際應該根據遊戲狀態查找敵方單位
    return EnemyUnits;
}

AMingTacticalUnit* AMingCombatAI::FindBestTarget(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return nullptr;
    }
    
    TArray<AMingTacticalUnit*> EnemyUnits = GetEnemyUnits(Unit);
    
    if (EnemyUnits.Num() == 0)
    {
        return nullptr;
    }
    
    // 簡化實作：返回第一個敵方單位
    return EnemyUnits[0];
}

AMingTacticalUnit* AMingCombatAI::FindAllyNeedingHelp(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return nullptr;
    }
    
    // 簡化實作：返回空指針
    // 實際應該根據友軍狀態查找需要幫助的單位
    return nullptr;
}

FVector AMingCombatAI::CalculateBestMovementPosition(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return FVector::ZeroVector;
    }
    
    // 簡化實作：返回當前位置附近的隨機位置
    FVector CurrentLocation = Unit->GetActorLocation();
    FVector RandomOffset = FVector(FMath::FRandRange(-200.0f, 200.0f), FMath::FRandRange(-200.0f, 200.0f), 0.0f);
    return CurrentLocation + RandomOffset;
}

FVector AMingCombatAI::CalculateRetreatPosition(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return FVector::ZeroVector;
    }
    
    // 計算單位中心
    FVector Center = FVector::ZeroVector;
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Center += Unit->GetActorLocation();
        }
    }
    
    if (Units.Num() > 0)
    {
        Center /= Units.Num();
    }
    
    // 在中心後方生成撤退位置
    FVector RetreatPosition = Center;
    RetreatPosition.Y -= 500.0f; // 向後撤退500單位
    
    return RetreatPosition;
}

FVector AMingCombatAI::CalculateFormationCenter(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return FVector::ZeroVector;
    }
    
    FVector Center = FVector::ZeroVector;
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Center += Unit->GetActorLocation();
        }
    }
    
    return Units.Num() > 0 ? Center / Units.Num() : FVector::ZeroVector;
}

bool AMingCombatAI::ExecuteScoutDecision(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    // 執行偵察決策
    FVector ScoutPosition = CalculateBestMovementPosition(Unit);
    return ExecuteMovementDecision(Unit, ScoutPosition);
}

bool AMingCombatAI::ExecutePatrolDecision(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    // 執行巡邏決策
    FVector PatrolPosition = CalculateBestMovementPosition(Unit);
    return ExecuteMovementDecision(Unit, PatrolPosition);
}

bool AMingCombatAI::ExecuteGuardDecision(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return false;
    }

    // 執行守衛決策
    return ExecuteDefenseDecision(Unit, Unit->GetActorLocation());
}

float AMingCombatAI::CalculateUnitSpread(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() < 2)
    {
        return 0.0f;
    }
    
    float TotalDistance = 0.0f;
    int32 PairCount = 0;
    
    for (int32 i = 0; i < Units.Num(); ++i)
    {
        for (int32 j = i + 1; j < Units.Num(); ++j)
        {
            if (Units[i] && Units[j])
            {
                TotalDistance += FVector::Dist(Units[i]->GetActorLocation(), Units[j]->GetActorLocation());
                PairCount++;
            }
        }
    }
    
    return PairCount > 0 ? TotalDistance / PairCount : 0.0f;
}

float AMingCombatAI::CalculateAverageHealth(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return 0.0f;
    }
    
    float TotalHealth = 0.0f;
    int32 ValidUnits = 0;
    
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            // 簡化實作：假設健康值為1.0
            TotalHealth += 1.0f;
            ValidUnits++;
        }
    }
    
    return ValidUnits > 0 ? TotalHealth / ValidUnits : 0.0f;
}

float AMingCombatAI::CalculateTacticalPositionScore(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return 0.0f;
    }
    
    // 簡化實作：基於位置計算戰術分數
    float Score = 0.0f;
    
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            FVector Location = Unit->GetActorLocation();
            
            // 基於位置評估戰術價值
            if (Location.X > 0.0f && Location.Y > 0.0f)
            {
                Score += 0.1f; // 第一象限有戰術優勢
            }
            
            if (FMath::Abs(Location.X) < 500.0f && FMath::Abs(Location.Y) < 500.0f)
            {
                Score += 0.2f; // 中心位置有戰術價值
            }
        }
    }
    
    return FMath::Clamp(Score / Units.Num(), 0.0f, 1.0f);
}

float AMingCombatAI::CalculateUnitQualityRatio(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits) const
{
    if (FriendlyUnits.Num() == 0 || EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }
    
    float FriendlyQuality = 0.0f;
    float EnemyQuality = 0.0f;
    
    // 簡化實作：基於單位質量計算比率
    for (AMingTacticalUnit* Unit : FriendlyUnits)
    {
        if (Unit)
        {
            FriendlyQuality += 1.0f; // 簡化質量評估
        }
    }
    
    for (AMingTacticalUnit* Unit : EnemyUnits)
    {
        if (Unit)
        {
            EnemyQuality += 1.0f; // 簡化質量評估
        }
    }
    
    return (FriendlyQuality / FriendlyUnits.Num()) / (EnemyQuality / EnemyUnits.Num()) - 1.0f;
}

float AMingCombatAI::CalculatePositionAdvantage(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits) const
{
    if (FriendlyUnits.Num() == 0 || EnemyUnits.Num() == 0)
    {
        return 0.0f;
    }
    
    float Advantage = 0.0f;
    
    // 計算位置優勢
    FVector FriendlyCenter = CalculateFormationCenter(FriendlyUnits);
    FVector EnemyCenter = CalculateFormationCenter(EnemyUnits);
    
    float Distance = FVector::Dist(FriendlyCenter, EnemyCenter);
    
    // 距離適中時有優勢
    if (Distance > 300.0f && Distance < 800.0f)
    {
        Advantage += 0.3f;
    }
    
    return FMath::Clamp(Advantage, -1.0f, 1.0f);
}

TArray<FString> AMingCombatAI::AssessRisks(const TArray<AMingTacticalUnit*>& Units) const
{
    TArray<FString> Risks;
    
    if (Units.Num() == 0)
    {
        return Risks;
    }
    
    // 評估風險
    Risks.Add(TEXT("單位分散可能被各個擊破"));
    Risks.Add(TEXT("缺乏掩護可能受到遠程攻擊"));
    Risks.Add(TEXT("位置暴露可能被偵察到"));
    
    if (Units.Num() < 3)
    {
        Risks.Add(TEXT("單位數量不足，戰鬥力有限"));
    }
    
    return Risks;
}

TArray<FString> AMingCombatAI::IdentifyOpportunities(const TArray<AMingTacticalUnit*>& Units) const
{
    TArray<FString> Opportunities;
    
    if (Units.Num() == 0)
    {
        return Opportunities;
    }
    
    // 識別機會
    Opportunities.Add(TEXT("可以發動協調攻擊"));
    Opportunities.Add(TEXT("可以設置伏擊陣地"));
    Opportunities.Add(TEXT("可以佔領戰略要點"));
    
    if (Units.Num() >= 5)
    {
        Opportunities.Add(TEXT("可以執行包圍戰術"));
    }
    
    return Opportunities;
}

float AMingCombatAI::CalculateAnalysisConfidence(const TArray<AMingTacticalUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return 0.0f;
    }
    
    float Confidence = 0.0f;
    
    // 基於單位數量
    Confidence += FMath::Clamp((float)Units.Num() / 10.0f, 0.0f, 0.3f);
    
    // 基於數據質量
    Confidence += 0.4f; // 簡化：假設數據質量良好
    
    // 基於AI性能
    Confidence += PerformanceMetrics.TacticalIndex * 0.3f;
    
    return FMath::Clamp(Confidence, 0.0f, 1.0f);
}

void AMingCombatAI::AdjustTacticalWeights()
{
    // 根據戰術類型調整決策權重
    for (auto& NodePair : DecisionNodes)
    {
        FAIDecisionNode& Node = NodePair.Value;
        
        switch (TacticalType)
        {
        case EAITacticalType::Aggressive:
            if (Node.DecisionType == EAIDecisionType::Attack || Node.DecisionType == EAIDecisionType::Flank)
            {
                Node.Weight *= 1.5f;
            }
            else if (Node.DecisionType == EAIDecisionType::Defense || Node.DecisionType == EAIDecisionType::Retreat)
            {
                Node.Weight *= 0.7f;
            }
            break;
        case EAITacticalType::Defensive:
            if (Node.DecisionType == EAIDecisionType::Defense || Node.DecisionType == EAIDecisionType::Retreat)
            {
                Node.Weight *= 1.5f;
            }
            else if (Node.DecisionType == EAIDecisionType::Attack || Node.DecisionType == EAIDecisionType::Flank)
            {
                Node.Weight *= 0.7f;
            }
            break;
        case EAITacticalType::Balanced:
            // 保持原始權重
            break;
        case EAITacticalType::Cautious:
            if (Node.DecisionType == EAIDecisionType::Retreat || Node.DecisionType == EAIDecisionType::Defense)
            {
                Node.Weight *= 1.3f;
            }
            else if (Node.DecisionType == EAIDecisionType::Attack)
            {
                Node.Weight *= 0.8f;
            }
            break;
        case EAITacticalType::Opportunistic:
            if (Node.DecisionType == EAIDecisionType::Attack || Node.DecisionType == EAIDecisionType::Ambush)
            {
                Node.Weight *= 1.4f;
            }
            break;
        default:
            break;
        }
        
        Node.Weight = FMath::Clamp(Node.Weight, 0.1f, 2.0f);
    }
}

void AMingCombatAI::AdjustLearningParameters()
{
    // 根據學習類型調整學習參數
    switch (LearningType)
    {
    case EAILearningType::Reinforcement:
        AIConfiguration.Add(TEXT("learning_rate"), FString::SanitizeFloat(0.1f));
        AIConfiguration.Add(TEXT("discount_factor"), FString::SanitizeFloat(0.9f));
        break;
    case EAILearningType::Supervised:
        AIConfiguration.Add(TEXT("learning_rate"), FString::SanitizeFloat(0.01f));
        AIConfiguration.Add(TEXT("epochs"), FString::FromInt(100));
        break;
    case EAILearningType::Neural:
        AIConfiguration.Add(TEXT("learning_rate"), FString::SanitizeFloat(0.001f));
        AIConfiguration.Add(TEXT("hidden_layers"), FString::FromInt(3));
        break;
    default:
        AIConfiguration.Add(TEXT("learning_rate"), FString::SanitizeFloat(0.1f));
        break;
    }
}

void AMingCombatAI::AdjustAIBehaviorBasedOnConfiguration()
{
    // 根據配置調整AI行為
    FString* DecisionFreq = AIConfiguration.Find(TEXT("decision_frequency"));
    if (DecisionFreq)
    {
        float Frequency = FCString::Atof(**DecisionFreq);
        // 調整決策頻率
    }
    
    FString* LearningRate = AIConfiguration.Find(TEXT("learning_rate"));
    if (LearningRate)
    {
        float Rate = FCString::Atof(**LearningRate);
        // 調整學習率
    }
}

void AMingCombatAI::AdjustDecisionBasedOnLearning(AMingTacticalUnit* Unit, const FAILearningData& LearningData)
{
    if (!Unit)
    {
        return;
    }
    
    // 根據學習數據調整決策
    if (LearningData.SuccessRate > 0.8f)
    {
        // 成功率高，增加該類型決策的權重
        for (auto& NodePair : DecisionNodes)
        {
            FAIDecisionNode& Node = NodePair.Value;
            Node.Weight *= 1.1f;
            Node.Weight = FMath::Clamp(Node.Weight, 0.1f, 2.0f);
        }
    }
    else if (LearningData.SuccessRate < 0.3f)
    {
        // 成功率低，減少該類型決策的權重
        for (auto& NodePair : DecisionNodes)
        {
            FAIDecisionNode& Node = NodePair.Value;
            Node.Weight *= 0.9f;
            Node.Weight = FMath::Clamp(Node.Weight, 0.1f, 2.0f);
        }
    }
}

void AMingCombatAI::AdjustBehaviorBasedOnAdaptation(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData)
{
    if (!Unit || AdaptationData.Num() == 0)
    {
        return;
    }
    
    // 根據適應數據調整行為
    float AdaptationLevel = 0.0f;
    for (float Data : AdaptationData)
    {
        AdaptationLevel += Data;
    }
    AdaptationLevel /= AdaptationData.Num();
    
    if (AdaptationLevel > 0.7f)
    {
        // 高適應性，可以更積極
        SetAITacticalType(EAITacticalType::Aggressive);
    }
    else if (AdaptationLevel < 0.3f)
    {
        // 低適應性，需要更謹慎
        SetAITacticalType(EAITacticalType::Cautious);
    }
}

void AMingCombatAI::OptimizeDecisionNodes()
{
    // 優化決策節點
    TArray<FString> NodesToRemove;
    
    for (const auto& NodePair : DecisionNodes)
    {
        const FAIDecisionNode& Node = NodePair.Value;
        
        // 移除不活躍或效果差的節點
        if (Node.ExecutionCount == 0 || (Node.ExecutionCount > 10 && Node.SuccessRate < 0.1f))
        {
            NodesToRemove.Add(NodePair.Key);
        }
    }
    
    for (const FString& NodeID : NodesToRemove)
    {
        DecisionNodes.Remove(NodeID);
    }
}

void AMingCombatAI::OptimizeLearningAlgorithms()
{
    // 優化學習算法
    if (LearningData.Num() > 500)
    {
        // 保留最近的500個學習樣本
        LearningData.RemoveAt(0, LearningData.Num() - 500);
    }
}

float AMingCombatAI::CalculateAdaptabilityIndex() const
{
    float Index = 0.0f;
    
    // 基於適應次數
    int32 Adaptations = AIStatistics.FindRef(TEXT("adaptations"));
    Index += FMath::Clamp((float)Adaptations / 100.0f, 0.0f, 0.4f);
    
    // 基於戰術類型變化
    Index += 0.3f; // 簡化：假設戰術類型有變化
    
    // 基於學習效果
    Index += PerformanceMetrics.LearningIndex * 0.3f;
    
    return FMath::Clamp(Index, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateTacticalIndex() const
{
    float Index = 0.0f;
    
    // 基於戰術分析質量
    if (TacticalAnalyses.Num() > 0)
    {
        float TotalConfidence = 0.0f;
        for (const FAITacticalAnalysis& Analysis : TacticalAnalyses)
        {
            TotalConfidence += Analysis.Confidence;
        }
        Index += TotalConfidence / TacticalAnalyses.Num() * 0.4f;
    }
    
    // 基於決策質量
    int32 TotalDecisions = AIStatistics.FindRef(TEXT("total_decisions"));
    int32 SuccessfulDecisions = AIStatistics.FindRef(TEXT("successful_decisions"));
    
    if (TotalDecisions > 0)
    {
        Index += (float)SuccessfulDecisions / TotalDecisions * 0.3f;
    }
    
    // 基於協調效果
    Index += PerformanceMetrics.CoordinationIndex * 0.3f;
    
    return FMath::Clamp(Index, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateCoordinationIndex() const
{
    float Index = 0.0f;
    
    // 基於多單位協調效果
    if (MultiUnitCoordinator)
    {
        // 簡化實作：假設協調效果良好
        Index += 0.6f;
    }
    
    // 基於單位間協作
    Index += 0.4f; // 簡化：假設協作效果良好
    
    return FMath::Clamp(Index, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateInnovationIndex() const
{
    float Index = 0.0f;
    
    // 基於新決策嘗試
    int32 TotalDecisions = AIStatistics.FindRef(TEXT("total_decisions"));
    Index += FMath::Clamp((float)TotalDecisions / 1000.0f, 0.0f, 0.5f);
    
    // 基於學習創新
    Index += PerformanceMetrics.LearningIndex * 0.3f;
    
    // 基於戰術多樣性
    Index += 0.2f; // 簡化：假設戰術多樣性良好
    
    return FMath::Clamp(Index, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateResourceUsage() const
{
    float Usage = 0.0f;
    
    // 基於計算複雜度
    Usage += PerformanceMetrics.ComputationalComplexity * 0.4f;
    
    // 基於記憶使用
    Usage += PerformanceMetrics.MemoryUsage * 0.3f;
    
    // 基於單位數量
    Usage += FMath::Clamp((float)ControlledUnits.Num() / 100.0f, 0.0f, 0.3f);
    
    return FMath::Clamp(Usage, 0.0f, 1.0f);
}

float AMingCombatAI::CalculateMemoryUsage() const
{
    float Usage = 0.0f;
    
    // 基於數據結構大小
    Usage += DecisionNodes.Num() * 0.001f;
    Usage += LearningData.Num() * 0.0001f;
    Usage += TacticalAnalyses.Num() * 0.0002f;
    Usage += ControlledUnits.Num() * 0.0001f;
    
    return FMath::Clamp(Usage, 0.0f, 1.0f);
}

float AMingCombatAI::EvaluateFitness(float Value) const
{
    // 簡化的適應度函數
    return FMath::Clamp(Value, 0.0f, 1.0f);
}
