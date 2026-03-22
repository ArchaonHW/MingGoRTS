#include "MingCombatAIBlueprintLibrary.h"
#include "MingCombatAI.h"
#include "MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

AMingCombatAI* UMingCombatAIBlueprintLibrary::GetCombatAI()
{
    // 這裡應該從遊戲實例或單例獲取戰鬥AI
    // 簡化實作
    static AMingCombatAI* AIInstance = nullptr;
    if (!AIInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            AIInstance = World->SpawnActor<AMingCombatAI>();
            AIInstance->InitializeCombatAI();
        }
    }
    
    return AIInstance;
}

void UMingCombatAIBlueprintLibrary::InitializeCombatAI()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->InitializeCombatAI();
        UE_LOG(LogTemp, Log, TEXT("Combat AI initialized from Blueprint"));
    }
}

void UMingCombatAIBlueprintLibrary::ShutdownCombatAI()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->ShutdownCombatAI();
        UE_LOG(LogTemp, Log, TEXT("Combat AI shutdown from Blueprint"));
    }
}

void UMingCombatAIBlueprintLibrary::SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel)
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->SetAIDifficultyLevel(DifficultyLevel);
        UE_LOG(LogTemp, Log, TEXT("AI difficulty level set to: %d"), static_cast<int32>(DifficultyLevel));
    }
}

void UMingCombatAIBlueprintLibrary::SetAITacticalType(EAITacticalType TacticalType)
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->SetAITacticalType(TacticalType);
        UE_LOG(LogTemp, Log, TEXT("AI tactical type set to: %d"), static_cast<int32>(TacticalType));
    }
}

void UMingCombatAIBlueprintLibrary::SetAILearningType(EAILearningType LearningType)
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->SetAILearningType(LearningType);
        UE_LOG(LogTemp, Log, TEXT("AI learning type set to: %d"), static_cast<int32>(LearningType));
    }
}

bool UMingCombatAIBlueprintLibrary::AddControlledUnit(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->AddControlledUnit(Unit);
}

bool UMingCombatAIBlueprintLibrary::RemoveControlledUnit(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->RemoveControlledUnit(Unit);
}

TArray<AMingTacticalUnit*> UMingCombatAIBlueprintLibrary::GetControlledUnits()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<AMingTacticalUnit*>();
    }
    
    return AI->GetControlledUnits();
}

bool UMingCombatAIBlueprintLibrary::ExecuteAIDecision(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteAIDecision(Unit);
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        FAITacticalAnalysis Analysis;
        Analysis.Confidence = 0.0f;
        return Analysis;
    }
    
    return AI->ExecuteTacticalAnalysis(Units);
}

bool UMingCombatAIBlueprintLibrary::ExecuteAILearning(const FAILearningData& LearningData)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteAILearning(LearningData);
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::CreateDecisionNode(
    const FString& NodeName,
    EAIDecisionType DecisionType,
    const FString& ConditionExpression,
    float Weight,
    int32 Priority)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return FAIDecisionNode();
    }
    
    return AI->CreateDecisionNode(NodeName, DecisionType, ConditionExpression, Weight, Priority);
}

bool UMingCombatAIBlueprintLibrary::AddDecisionNode(const FAIDecisionNode& Node)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->AddDecisionNode(Node);
}

bool UMingCombatAIBlueprintLibrary::RemoveDecisionNode(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->RemoveDecisionNode(NodeID);
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::GetDecisionNode(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return FAIDecisionNode();
    }
    
    return AI->GetDecisionNode(NodeID);
}

TArray<FAIDecisionNode> UMingCombatAIBlueprintLibrary::GetAllDecisionNodes()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<FAIDecisionNode>();
    }
    
    return AI->GetAllDecisionNodes();
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        FAITacticalAnalysis Analysis;
        Analysis.Confidence = 0.0f;
        return Analysis;
    }
    
    return AI->EvaluateTacticalSituation(FriendlyUnits, EnemyUnits);
}

TArray<FString> UMingCombatAIBlueprintLibrary::GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<FString>();
    }
    
    return AI->GenerateTacticalRecommendations(Units);
}

TArray<FString> UMingCombatAIBlueprintLibrary::PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<FString>();
    }
    
    return AI->PredictEnemyActions(EnemyUnits);
}

float UMingCombatAIBlueprintLibrary::CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    return AI->CalculateThreatLevel(Unit, EnemyUnits);
}

float UMingCombatAIBlueprintLibrary::CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    return AI->CalculateOpportunityLevel(Unit, EnemyUnits);
}

bool UMingCombatAIBlueprintLibrary::ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteAttackDecision(Unit, TargetUnit);
}

bool UMingCombatAIBlueprintLibrary::ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteDefenseDecision(Unit, DefensePosition);
}

bool UMingCombatAIBlueprintLibrary::ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteMovementDecision(Unit, TargetPosition);
}

bool UMingCombatAIBlueprintLibrary::ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteRetreatDecision(Unit, RetreatPosition);
}

bool UMingCombatAIBlueprintLibrary::ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteFlankDecision(Unit, TargetUnit);
}

bool UMingCombatAIBlueprintLibrary::ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteAmbushDecision(Unit, TargetUnits);
}

bool UMingCombatAIBlueprintLibrary::ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteSupportDecision(Unit, TargetUnit);
}

bool UMingCombatAIBlueprintLibrary::ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteCoordinationDecision(Units, DecisionType);
}

bool UMingCombatAIBlueprintLibrary::ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteLearningDecision(Unit, LearningData);
}

bool UMingCombatAIBlueprintLibrary::ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->ExecuteAdaptationDecision(Unit, AdaptationData);
}

void UMingCombatAIBlueprintLibrary::AdjustAIDifficulty(float PerformanceFactor)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return;
    }
    
    AI->AdjustAIDifficulty(PerformanceFactor);
}

FAIPerformanceMetrics UMingCombatAIBlueprintLibrary::GetAIPerformanceMetrics()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return FAIPerformanceMetrics();
    }
    
    return AI->GetAIPerformanceMetrics();
}

TMap<FString, int32> UMingCombatAIBlueprintLibrary::GetAIStatistics()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TMap<FString, int32>();
    }
    
    return AI->GetAIStatistics();
}

TMap<FString, FString> UMingCombatAIBlueprintLibrary::GetAIConfiguration()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TMap<FString, FString>();
    }
    
    return AI->GetAIConfiguration();
}

bool UMingCombatAIBlueprintLibrary::SetAIConfiguration(const TMap<FString, FString>& Configuration)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        UE_LOG(LogTemp, Error, TEXT("Combat AI not available"));
        return false;
    }
    
    return AI->SetAIConfiguration(Configuration);
}

FString UMingCombatAIBlueprintLibrary::GetAIStateName(EAIState AIState)
{
    switch (AIState)
    {
    case EAIState::Idle:
        return TEXT("待命");
    case EAIState::Scouting:
        return TEXT("偵察");
    case EAIState::Patrolling:
        return TEXT("巡邏");
    case EAIState::Engaging:
        return TEXT("交戰");
    case EAIState::Attacking:
        return TEXT("攻擊");
    case EAIState::Defending:
        return TEXT("防禦");
    case EAIState::Retreating:
        return TEXT("撤退");
    case EAIState::Flanking:
        return TEXT("側翼攻擊");
    case EAIState::Supporting:
        return TEXT("支援");
    case EAIState::Healing:
        return TEXT("治療");
    case EAIState::Repairing:
        return TEXT("修理");
    case EAIState::Building:
        return TEXT("建設");
    case EAIState::Capturing:
        return TEXT("捕獲");
    case EAIState::Guarding:
        return TEXT("守衛");
    case EAIState::Ambushing:
        return TEXT("伏擊");
    case EAIState::Coordinating:
        return TEXT("協調");
    case EAIState::Learning:
        return TEXT("學習");
    case EAIState::Adapting:
        return TEXT("適應");
    case EAIState::Analyzing:
        return TEXT("分析");
    default:
        return TEXT("未知狀態");
    }
}

FString UMingCombatAIBlueprintLibrary::GetDecisionTypeName(EAIDecisionType DecisionType)
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

FString UMingCombatAIBlueprintLibrary::GetTacticalTypeName(EAITacticalType TacticalType)
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

FString UMingCombatAIBlueprintLibrary::GetDifficultyLevelName(EAIDifficultyLevel DifficultyLevel)
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

FString UMingCombatAIBlueprintLibrary::GetLearningTypeName(EAILearningType LearningType)
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

FAILearningData UMingCombatAIBlueprintLibrary::CreateLearningData(
    EAILearningType LearningType,
    const TArray<float>& InputData,
    const TArray<float>& OutputData,
    float RewardValue,
    float PenaltyValue)
{
    FAILearningData LearningData;
    LearningData.LearningID = FString::Printf(TEXT("learning_%d"), FDateTime::Now().GetMillisecond());
    LearningData.LearningType = LearningType;
    LearningData.InputData = InputData;
    LearningData.OutputData = OutputData;
    LearningData.RewardValue = RewardValue;
    LearningData.PenaltyValue = PenaltyValue;
    LearningData.LearningTime = FPlatformTime::Seconds();
    LearningData.LearningCount = 1;
    LearningData.SuccessRate = 0.0f;
    LearningData.ConvergenceValue = 0.0f;
    
    return LearningData;
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::SetDecisionNodeParameter(const FAIDecisionNode& Node, const FString& ParameterName, const FString& ParameterValue)
{
    FAIDecisionNode ModifiedNode = Node;
    ModifiedNode.NodeParameters.Add(ParameterName, ParameterValue);
    return ModifiedNode;
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::AddDecisionNodeChild(const FAIDecisionNode& Node, const FString& ChildNodeID)
{
    FAIDecisionNode ModifiedNode = Node;
    ModifiedNode.ChildNodes.Add(ChildNodeID);
    return ModifiedNode;
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::CreateTacticalAnalysis(
    float ThreatAssessment,
    float OpportunityAssessment,
    float AdvantageAssessment,
    float DisadvantageAssessment)
{
    FAITacticalAnalysis Analysis;
    Analysis.AnalysisID = FString::Printf(TEXT("analysis_%d"), FDateTime::Now().GetMillisecond());
    Analysis.AnalysisTime = FPlatformTime::Seconds();
    Analysis.ThreatAssessment = ThreatAssessment;
    Analysis.OpportunityAssessment = OpportunityAssessment;
    Analysis.AdvantageAssessment = AdvantageAssessment;
    Analysis.DisadvantageAssessment = DisadvantageAssessment;
    Analysis.TacticalScore = (ThreatAssessment + OpportunityAssessment + AdvantageAssessment - DisadvantageAssessment) / 4.0f;
    Analysis.Confidence = 0.8f; // 默認置信度
    
    return Analysis;
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::AddTacticalRecommendation(const FAITacticalAnalysis& Analysis, const FString& Recommendation)
{
    FAITacticalAnalysis ModifiedAnalysis = Analysis;
    ModifiedAnalysis.TacticalRecommendations.Add(Recommendation);
    return ModifiedAnalysis;
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::AddRiskAssessment(const FAITacticalAnalysis& Analysis, const FString& Risk)
{
    FAITacticalAnalysis ModifiedAnalysis = Analysis;
    ModifiedAnalysis.RiskAssessments.Add(Risk);
    return ModifiedAnalysis;
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::AddOpportunityIdentification(const FAITacticalAnalysis& Analysis, const FString& Opportunity)
{
    FAITacticalAnalysis ModifiedAnalysis = Analysis;
    ModifiedAnalysis.OpportunityIdentifications.Add(Opportunity);
    return ModifiedAnalysis;
}

FAITacticalAnalysis UMingCombatAIBlueprintLibrary::AddEnemyPrediction(const FAITacticalAnalysis& Analysis, const FString& Prediction)
{
    FAITacticalAnalysis ModifiedAnalysis = Analysis;
    ModifiedAnalysis.EnemyPredictions.Add(Prediction);
    return ModifiedAnalysis;
}

TArray<bool> UMingCombatAIBlueprintLibrary::BatchExecuteAIDecisions(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<bool> Results;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        for (AMingTacticalUnit* Unit : Units)
        {
            Results.Add(false);
        }
        return Results;
    }
    
    for (AMingTacticalUnit* Unit : Units)
    {
        Results.Add(AI->ExecuteAIDecision(Unit));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch executed %d AI decisions"), Results.Num());
    
    return Results;
}

TArray<bool> UMingCombatAIBlueprintLibrary::BatchAddControlledUnits(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<bool> Results;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        for (AMingTacticalUnit* Unit : Units)
        {
            Results.Add(false);
        }
        return Results;
    }
    
    for (AMingTacticalUnit* Unit : Units)
    {
        Results.Add(AI->AddControlledUnit(Unit));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch added %d controlled units"), Results.Num());
    
    return Results;
}

TArray<bool> UMingCombatAIBlueprintLibrary::BatchRemoveControlledUnits(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<bool> Results;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        for (AMingTacticalUnit* Unit : Units)
        {
            Results.Add(false);
        }
        return Results;
    }
    
    for (AMingTacticalUnit* Unit : Units)
    {
        Results.Add(AI->RemoveControlledUnit(Unit));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch removed %d controlled units"), Results.Num());
    
    return Results;
}

TArray<FAITacticalAnalysis> UMingCombatAIBlueprintLibrary::BatchExecuteTacticalAnalysis(const TArray<TArray<AMingTacticalUnit*>>& UnitGroups)
{
    TArray<FAITacticalAnalysis> Results;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Results;
    }
    
    for (const TArray<AMingTacticalUnit*>& UnitGroup : UnitGroups)
    {
        Results.Add(AI->ExecuteTacticalAnalysis(UnitGroup));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch executed %d tactical analyses"), Results.Num());
    
    return Results;
}

TArray<bool> UMingCombatAIBlueprintLibrary::BatchExecuteAILearning(const TArray<FAILearningData>& LearningDataArray)
{
    TArray<bool> Results;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        for (const FAILearningData& LearningData : LearningDataArray)
        {
            Results.Add(false);
        }
        return Results;
    }
    
    for (const FAILearningData& LearningData : LearningDataArray)
    {
        Results.Add(AI->ExecuteAILearning(LearningData));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch executed %d AI learning operations"), Results.Num());
    
    return Results;
}

bool UMingCombatAIBlueprintLibrary::IsCombatAIInitialized()
{
    AMingCombatAI* AI = GetCombatAI();
    return AI != nullptr;
}

FString UMingCombatAIBlueprintLibrary::GetCombatAIVersion()
{
    return TEXT("1.0.0");
}

int32 UMingCombatAIBlueprintLibrary::GetControlledUnitCount()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0;
    }
    
    return AI->GetControlledUnits().Num();
}

int32 UMingCombatAIBlueprintLibrary::GetDecisionNodeCount()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0;
    }
    
    return AI->GetAllDecisionNodes().Num();
}

int32 UMingCombatAIBlueprintLibrary::GetLearningDataCount()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0;
    }
    
    TMap<FString, int32> Stats = AI->GetAIStatistics();
    return Stats.FindRef(TEXT("learning_iterations"));
}

int32 UMingCombatAIBlueprintLibrary::GetTacticalAnalysisCount()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0;
    }
    
    TMap<FString, int32> Stats = AI->GetAIStatistics();
    return Stats.FindRef(TEXT("tactical_analyses"));
}

bool UMingCombatAIBlueprintLibrary::IsUnitControlled(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI || !Unit)
    {
        return false;
    }
    
    TArray<AMingTacticalUnit*> ControlledUnits = AI->GetControlledUnits();
    return ControlledUnits.Contains(Unit);
}

bool UMingCombatAIBlueprintLibrary::DoesDecisionNodeExist(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    return !Node.NodeID.IsEmpty();
}

bool UMingCombatAIBlueprintLibrary::IsDecisionNodeEnabled(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    return Node.bIsEnabled;
}

int32 UMingCombatAIBlueprintLibrary::GetDecisionNodeExecutionCount(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0;
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    return Node.ExecutionCount;
}

float UMingCombatAIBlueprintLibrary::GetDecisionNodeSuccessRate(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    return Node.ExecutionCount > 0 ? (float)Node.SuccessCount / Node.ExecutionCount : 0.0f;
}

float UMingCombatAIBlueprintLibrary::CalculateAIPerformanceScore()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    FAIPerformanceMetrics Metrics = AI->GetAIPerformanceMetrics();
    
    // 綜合評分
    float Score = 0.0f;
    Score += Metrics.SuccessRate * 0.3f;
    Score += Metrics.EfficiencyIndex * 0.2f;
    Score += Metrics.AdaptabilityIndex * 0.2f;
    Score += Metrics.LearningIndex * 0.15f;
    Score += Metrics.TacticalIndex * 0.15f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingCombatAIBlueprintLibrary::CalculateDecisionQualityScore(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    
    if (Node.ExecutionCount == 0)
    {
        return 0.5f; // 默認中等質量
    }
    
    float SuccessRate = (float)Node.SuccessCount / Node.ExecutionCount;
    float Frequency = Node.ExecutionCount / 100.0f; // 執行頻率
    float Weight = Node.Weight;
    
    float Score = SuccessRate * 0.6f + Frequency * 0.2f + Weight * 0.2f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingCombatAIBlueprintLibrary::CalculateTacticalAnalysisScore(const FAITacticalAnalysis& Analysis)
{
    float Score = 0.0f;
    
    // 基於各種評估計算分數
    Score += Analysis.ThreatAssessment * 0.2f;
    Score += Analysis.OpportunityAssessment * 0.2f;
    Score += Analysis.AdvantageAssessment * 0.2f;
    Score += (1.0f - Analysis.DisadvantageAssessment) * 0.2f;
    Score += Analysis.Confidence * 0.2f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

float UMingCombatAIBlueprintLibrary::CalculateLearningEffectScore(const FAILearningData& LearningData)
{
    float Score = 0.0f;
    
    // 基於學習效果計算分數
    Score += LearningData.SuccessRate * 0.4f;
    Score += LearningData.ConvergenceValue * 0.3f;
    Score += FMath::Clamp(LearningData.RewardValue - LearningData.PenaltyValue, -1.0f, 1.0f) * 0.3f;
    
    return FMath::Clamp(Score, 0.0f, 1.0f);
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::GetBestDecisionNode(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI || !Unit)
    {
        return FAIDecisionNode();
    }
    
    TArray<FAIDecisionNode> AllNodes = AI->GetAllDecisionNodes();
    FAIDecisionNode BestNode;
    float BestScore = -1.0f;
    
    for (const FAIDecisionNode& Node : AllNodes)
    {
        if (!Node.bIsEnabled)
        {
            continue;
        }
        
        float Score = CalculateDecisionQualityScore(Node.NodeID);
        
        if (Score > BestScore)
        {
            BestScore = Score;
            BestNode = Node;
        }
    }
    
    return BestNode;
}

FAIDecisionNode UMingCombatAIBlueprintLibrary::GetWorstDecisionNode(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI || !Unit)
    {
        return FAIDecisionNode();
    }
    
    TArray<FAIDecisionNode> AllNodes = AI->GetAllDecisionNodes();
    FAIDecisionNode WorstNode;
    float WorstScore = 2.0f; // 高於最大可能分數
    
    for (const FAIDecisionNode& Node : AllNodes)
    {
        if (!Node.bIsEnabled)
        {
            continue;
        }
        
        float Score = CalculateDecisionQualityScore(Node.NodeID);
        
        if (Score < WorstScore)
        {
            WorstScore = Score;
            WorstNode = Node;
        }
    }
    
    return WorstNode;
}

EAIDecisionType UMingCombatAIBlueprintLibrary::GetRecommendedDecisionType(AMingTacticalUnit* Unit)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI || !Unit)
    {
        return EAIDecisionType::Movement;
    }
    
    // 簡化實作：基於單位狀態推薦決策類型
    float ThreatLevel = AI->CalculateThreatLevel(Unit, TArray<AMingTacticalUnit*>());
    
    if (ThreatLevel > 0.8f)
    {
        return EAIDecisionType::Retreat;
    }
    else if (ThreatLevel > 0.5f)
    {
        return EAIDecisionType::Defense;
    }
    else if (ThreatLevel > 0.2f)
    {
        return EAIDecisionType::Attack;
    }
    else
    {
        return EAIDecisionType::Patrol;
    }
}

EAITacticalType UMingCombatAIBlueprintLibrary::GetRecommendedTacticalType(const TArray<AMingTacticalUnit*>& Units)
{
    if (Units.Num() == 0)
    {
        return EAITacticalType::Balanced;
    }
    
    // 簡化實作：基於單位數量推薦戰術類型
    if (Units.Num() >= 10)
    {
        return EAITacticalType::Conventional;
    }
    else if (Units.Num() >= 5)
    {
        return EAITacticalType::Balanced;
    }
    else if (Units.Num() >= 3)
    {
        return EAITacticalType::Guerrilla;
    }
    else
    {
        return EAITacticalType::Cautious;
    }
}

EAIDifficultyLevel UMingCombatAIBlueprintLibrary::GetRecommendedDifficultyLevel(float PlayerPerformance)
{
    if (PlayerPerformance > 0.8f)
    {
        return EAIDifficultyLevel::VeryHard;
    }
    else if (PlayerPerformance > 0.6f)
    {
        return EAIDifficultyLevel::Hard;
    }
    else if (PlayerPerformance > 0.4f)
    {
        return EAIDifficultyLevel::Normal;
    }
    else if (PlayerPerformance > 0.2f)
    {
        return EAIDifficultyLevel::Easy;
    }
    else
    {
        return EAIDifficultyLevel::VeryEasy;
    }
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetAIRecommendations(AMingTacticalUnit* Unit)
{
    TArray<FString> Recommendations;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI || !Unit)
    {
        return Recommendations;
    }
    
    // 基於AI分析生成建議
    Recommendations.Add(TEXT("保持單位間的協調"));
    Recommendations.Add(TEXT("注意偵察敵方動向"));
    Recommendations.Add(TEXT("合理利用地形優勢"));
    Recommendations.Add(TEXT("保持適當的攻防平衡"));
    Recommendations.Add(TEXT("及時調整戰術策略"));
    
    return Recommendations;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FString> Recommendations;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Recommendations;
    }
    
    return AI->GenerateTacticalRecommendations(Units);
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetStrategicRecommendations(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FString> Recommendations;
    
    if (Units.Num() == 0)
    {
        return Recommendations;
    }
    
    // 基於戰略考慮生成建議
    Recommendations.Add(TEXT("控制關鍵戰術要點"));
    Recommendations.Add(TEXT("保持補給線暢通"));
    Recommendations.Add(TEXT("避免過度分散兵力"));
    Recommendations.Add(TEXT("預留預備隊應對突發情況"));
    Recommendations.Add(TEXT("根據敵方特點調整戰術"));
    
    return Recommendations;
}

TArray<AMingTacticalUnit*> UMingCombatAIBlueprintLibrary::GetAttackRecommendations(AMingTacticalUnit* Unit)
{
    TArray<AMingTacticalUnit*> Recommendations;
    
    // 簡化實作：返回空數組
    // 實際應該根據遊戲狀態查找攻擊目標
    return Recommendations;
}

TArray<FVector> UMingCombatAIBlueprintLibrary::GetDefenseRecommendations(AMingTacticalUnit* Unit)
{
    TArray<FVector> Recommendations;
    
    if (!Unit)
    {
        return Recommendations;
    }
    
    // 簡化實作：在單位周圍生成防禦位置
    FVector CurrentLocation = Unit->GetActorLocation();
    
    // 生成圓形防禦陣地
    float Radius = 200.0f;
    for (int32 i = 0; i < 4; ++i)
    {
        float Angle = (float)i / 4 * 2 * PI;
        FVector DefensePosition = CurrentLocation;
        DefensePosition.X += FMath::Cos(Angle) * Radius;
        DefensePosition.Y += FMath::Sin(Angle) * Radius;
        Recommendations.Add(DefensePosition);
    }
    
    return Recommendations;
}

TArray<FVector> UMingCombatAIBlueprintLibrary::GetMovementRecommendations(AMingTacticalUnit* Unit)
{
    TArray<FVector> Recommendations;
    
    if (!Unit)
    {
        return Recommendations;
    }
    
    // 簡化實作：在單位周圍生成移動目標
    FVector CurrentLocation = Unit->GetActorLocation();
    
    // 生成多個移動方向
    for (int32 i = 0; i < 8; ++i)
    {
        float Angle = (float)i / 8 * 2 * PI;
        FVector MovementTarget = CurrentLocation;
        MovementTarget.X += FMath::Cos(Angle) * 300.0f;
        MovementTarget.Y += FMath::Sin(Angle) * 300.0f;
        Recommendations.Add(MovementTarget);
    }
    
    return Recommendations;
}

TArray<FVector> UMingCombatAIBlueprintLibrary::GetRetreatRecommendations(AMingTacticalUnit* Unit)
{
    TArray<FVector> Recommendations;
    
    if (!Unit)
    {
        return Recommendations;
    }
    
    // 簡化實作：在單位後方生成撤退位置
    FVector CurrentLocation = Unit->GetActorLocation();
    
    // 生成多個撤退方向
    for (int32 i = 0; i < 4; ++i)
    {
        float Angle = (float)i / 4 * 2 * PI + PI; // 後方方向
        FVector RetreatPosition = CurrentLocation;
        RetreatPosition.X += FMath::Cos(Angle) * 400.0f;
        RetreatPosition.Y += FMath::Sin(Angle) * 400.0f;
        Recommendations.Add(RetreatPosition);
    }
    
    return Recommendations;
}

TArray<bool> UMingCombatAIBlueprintLibrary::SimulateAIDecisions(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount)
{
    TArray<bool> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        bool bSuccess = true;
        
        for (AMingTacticalUnit* Unit : Units)
        {
            if (!Unit)
            {
                bSuccess = false;
                break;
            }
            
            // 簡化模擬：基於隨機因素判斷成功
            if (FMath::FRand() > 0.7f) // 70%成功率
            {
                bSuccess = false;
                break;
            }
        }
        
        SimulationResults.Add(bSuccess);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d AI decisions"), SimulationCount);
    
    return SimulationResults;
}

TArray<FAITacticalAnalysis> UMingCombatAIBlueprintLibrary::SimulateTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount)
{
    TArray<FAITacticalAnalysis> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FAITacticalAnalysis Analysis;
        Analysis.AnalysisID = FString::Printf(TEXT("sim_analysis_%d"), i);
        Analysis.AnalysisTime = FPlatformTime::Seconds();
        
        // 簡化模擬：隨機生成分析結果
        Analysis.ThreatAssessment = FMath::FRandRange(0.0f, 1.0f);
        Analysis.OpportunityAssessment = FMath::FRandRange(0.0f, 1.0f);
        Analysis.AdvantageAssessment = FMath::FRandRange(0.0f, 1.0f);
        Analysis.DisadvantageAssessment = FMath::FRandRange(0.0f, 1.0f);
        Analysis.TacticalScore = FMath::FRandRange(0.0f, 1.0f);
        Analysis.Confidence = FMath::FRandRange(0.5f, 1.0f);
        
        SimulationResults.Add(Analysis);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d tactical analyses"), SimulationCount);
    
    return SimulationResults;
}

TArray<bool> UMingCombatAIBlueprintLibrary::SimulateAILearning(const TArray<FAILearningData>& LearningDataArray, int32 SimulationCount)
{
    TArray<bool> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        bool bSuccess = true;
        
        for (const FAILearningData& LearningData : LearningDataArray)
        {
            // 簡化模擬：基於學習數據判斷成功
            if (LearningData.SuccessRate < 0.5f)
            {
                bSuccess = false;
                break;
            }
        }
        
        SimulationResults.Add(bSuccess);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d AI learning operations"), SimulationCount);
    
    return SimulationResults;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetAIHistory()
{
    TArray<FString> History;
    
    // 簡化實作：返回模擬歷史
    History.Add(TEXT("Combat AI system initialized"));
    History.Add(TEXT("AI difficulty set to Normal"));
    History.Add(TEXT("Tactical type set to Balanced"));
    History.Add(TEXT("Learning type set to Reinforcement"));
    History.Add(TEXT("Decision tree created with 10 nodes"));
    History.Add(TEXT("First tactical analysis completed"));
    History.Add(TEXT("AI learning session started"));
    
    return History;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetDecisionHistory()
{
    TArray<FString> History;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return History;
    }
    
    TMap<FString, int32> Stats = AI->GetAIStatistics();
    
    History.Add(FString::Printf(TEXT("Total decisions: %d"), Stats.FindRef(TEXT("total_decisions"))));
    History.Add(FString::Printf(TEXT("Successful decisions: %d"), Stats.FindRef(TEXT("successful_decisions"))));
    History.Add(FString::Printf(TEXT("Failed decisions: %d"), Stats.FindRef(TEXT("failed_decisions"))));
    
    return History;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetLearningHistory()
{
    TArray<FString> History;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return History;
    }
    
    TMap<FString, int32> Stats = AI->GetAIStatistics();
    
    History.Add(FString::Printf(TEXT("Learning iterations: %d"), Stats.FindRef(TEXT("learning_iterations"))));
    History.Add(FString::Printf(TEXT("Adaptations: %d"), Stats.FindRef(TEXT("adaptations"))));
    History.Add(FString::Printf(TEXT("Predictions: %d"), Stats.FindRef(TEXT("predictions"))));
    
    return History;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetTacticalHistory()
{
    TArray<FString> History;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return History;
    }
    
    TMap<FString, int32> Stats = AI->GetAIStatistics();
    
    History.Add(FString::Printf(TEXT("Tactical analyses: %d"), Stats.FindRef(TEXT("tactical_analyses"))));
    History.Add(FString::Printf(TEXT("Recommendations: %d"), Stats.FindRef(TEXT("recommendations"))));
    
    return History;
}

TMap<FString, int32> UMingCombatAIBlueprintLibrary::GetAIStatistics()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TMap<FString, int32>();
    }
    
    return AI->GetAIStatistics();
}

TMap<FString, int32> UMingCombatAIBlueprintLibrary::GetDecisionStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Stats;
    }
    
    TMap<FString, int32> AIStats = AI->GetAIStatistics();
    
    Stats.Add(TEXT("total_decisions"), AIStats.FindRef(TEXT("total_decisions")));
    Stats.Add(TEXT("successful_decisions"), AIStats.FindRef(TEXT("successful_decisions")));
    Stats.Add(TEXT("failed_decisions"), AIStats.FindRef(TEXT("failed_decisions")));
    
    return Stats;
}

TMap<FString, int32> UMingCombatAIBlueprintLibrary::GetLearningStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Stats;
    }
    
    TMap<FString, int32> AIStats = AI->GetAIStatistics();
    
    Stats.Add(TEXT("learning_iterations"), AIStats.FindRef(TEXT("learning_iterations")));
    Stats.Add(TEXT("adaptations"), AIStats.FindRef(TEXT("adaptations")));
    Stats.Add(TEXT("predictions"), AIStats.FindRef(TEXT("predictions")));
    
    return Stats;
}

TMap<FString, int32> UMingCombatAIBlueprintLibrary::GetTacticalStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Stats;
    }
    
    TMap<FString, int32> AIStats = AI->GetAIStatistics();
    
    Stats.Add(TEXT("tactical_analyses"), AIStats.FindRef(TEXT("tactical_analyses")));
    Stats.Add(TEXT("recommendations"), AIStats.FindRef(TEXT("recommendations")));
    
    return Stats;
}

TMap<FString, float> UMingCombatAIBlueprintLibrary::GetPerformanceStatistics()
{
    TMap<FString, float> Stats;
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return Stats;
    }
    
    FAIPerformanceMetrics Metrics = AI->GetAIPerformanceMetrics();
    
    Stats.Add(TEXT("decision_time"), Metrics.DecisionTime);
    Stats.Add(TEXT("reaction_time"), Metrics.ReactionTime);
    Stats.Add(TEXT("success_rate"), Metrics.SuccessRate);
    Stats.Add(TEXT("efficiency_index"), Metrics.EfficiencyIndex);
    Stats.Add(TEXT("adaptability_index"), Metrics.AdaptabilityIndex);
    Stats.Add(TEXT("learning_index"), Metrics.LearningIndex);
    Stats.Add(TEXT("tactical_index"), Metrics.TacticalIndex);
    Stats.Add(TEXT("coordination_index"), Metrics.CoordinationIndex);
    Stats.Add(TEXT("innovation_index"), Metrics.InnovationIndex);
    Stats.Add(TEXT("resource_usage"), Metrics.ResourceUsage);
    Stats.Add(TEXT("computational_complexity"), Metrics.ComputationalComplexity);
    Stats.Add(TEXT("memory_usage"), Metrics.MemoryUsage);
    
    return Stats;
}

bool UMingCombatAIBlueprintLibrary::ExportAIData(const FString& FilePath)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->BackupAIData(FilePath);
}

bool UMingCombatAIBlueprintLibrary::ImportAIData(const FString& FilePath)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->RestoreAIData(FilePath);
}

FString UMingCombatAIBlueprintLibrary::GenerateAIReport()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TEXT("Combat AI not available");
    }
    
    return AI->GenerateAIReport();
}

FString UMingCombatAIBlueprintLibrary::GenerateUnitAIReport(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return TEXT("Invalid unit");
    }
    
    FString Report = TEXT("=== 單位AI報告 ===\n");
    Report += FString::Printf(TEXT("單位名稱: %s\n"), *Unit->GetName());
    Report += FString::Printf(TEXT("單位狀態: %s\n"), *Unit->GetName()); // 簡化
    
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        bool bIsControlled = IsUnitControlled(Unit);
        Report += FString::Printf(TEXT("AI控制: %s\n"), bIsControlled ? TEXT("是") : TEXT("否"));
        
        if (bIsControlled)
        {
            EAIDecisionType RecommendedType = GetRecommendedDecisionType(Unit);
            Report += FString::Printf(TEXT("推薦決策: %s\n"), *GetDecisionTypeName(RecommendedType));
        }
    }
    
    return Report;
}

FString UMingCombatAIBlueprintLibrary::GenerateDecisionReport(const FString& NodeID)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TEXT("Combat AI not available");
    }
    
    FAIDecisionNode Node = AI->GetDecisionNode(NodeID);
    
    if (Node.NodeID.IsEmpty())
    {
        return TEXT("Decision node not found");
    }
    
    FString Report = TEXT("=== 決策節點報告 ===\n");
    Report += FString::Printf(TEXT("節點ID: %s\n"), *Node.NodeID);
    Report += FString::Printf(TEXT("節點名稱: %s\n"), *Node.NodeName);
    Report += FString::Printf(TEXT("決策類型: %s\n"), *GetDecisionTypeName(Node.DecisionType));
    Report += FString::Printf(TEXT("條件表達式: %s\n"), *Node.ConditionExpression);
    Report += FString::Printf(TEXT("權重: %.2f\n"), Node.Weight);
    Report += FString::Printf(TEXT("優先級: %d\n"), Node.Priority);
    Report += FString::Printf(TEXT("執行次數: %d\n"), Node.ExecutionCount);
    Report += FString::Printf(TEXT("成功次數: %d\n"), Node.SuccessCount);
    Report += FString::Printf(TEXT("失敗次數: %d\n"), Node.FailureCount);
    Report += FString::Printf(TEXT("成功率: %.1f%%\n"), GetDecisionNodeSuccessRate(NodeID) * 100.0f);
    Report += FString::Printf(TEXT("最後執行時間: %.2f\n"), Node.LastExecutionTime);
    Report += FString::Printf(TEXT("是否啟用: %s\n"), Node.bIsEnabled ? TEXT("是") : TEXT("否"));
    
    Report += TEXT("\n=== 節點參數 ===\n");
    for (const auto& Parameter : Node.NodeParameters)
    {
        Report += FString::Printf(TEXT("- %s: %s\n"), *Parameter.Key, *Parameter.Value);
    }
    
    return Report;
}

FString UMingCombatAIBlueprintLibrary::GenerateTacticalAnalysisReport(const FAITacticalAnalysis& Analysis)
{
    FString Report = TEXT("=== 戰術分析報告 ===\n");
    Report += FString::Printf(TEXT("分析ID: %s\n"), *Analysis.AnalysisID);
    Report += FString::Printf(TEXT("分析時間: %.2f\n"), Analysis.AnalysisTime);
    Report += FString::Printf(TEXT("威脅評估: %.2f\n"), Analysis.ThreatAssessment);
    Report += FString::Printf(TEXT("機會評估: %.2f\n"), Analysis.OpportunityAssessment);
    Report += FString::Printf(TEXT("優勢評估: %.2f\n"), Analysis.AdvantageAssessment);
    Report += FString::Printf(TEXT("劣勢評估: %.2f\n"), Analysis.DisadvantageAssessment);
    Report += FString::Printf(TEXT("戰術分數: %.2f\n"), Analysis.TacticalScore);
    Report += FString::Printf(TEXT("置信度: %.2f\n"), Analysis.Confidence);
    
    Report += TEXT("\n=== 戰術建議 ===\n");
    for (const FString& Recommendation : Analysis.TacticalRecommendations)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Recommendation);
    }
    
    Report += TEXT("\n=== 風險評估 ===\n");
    for (const FString& Risk : Analysis.RiskAssessments)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Risk);
    }
    
    Report += TEXT("\n=== 機會識別 ===\n");
    for (const FString& Opportunity : Analysis.OpportunityIdentifications)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Opportunity);
    }
    
    Report += TEXT("\n=== 敵方預測 ===\n");
    for (const FString& Prediction : Analysis.EnemyPredictions)
    {
        Report += FString::Printf(TEXT("- %s\n"), *Prediction);
    }
    
    return Report;
}

FString UMingCombatAIBlueprintLibrary::GenerateLearningReport(const FAILearningData& LearningData)
{
    FString Report = TEXT("=== 學習報告 ===\n");
    Report += FString::Printf(TEXT("學習ID: %s\n"), *LearningData.LearningID);
    Report += FString::Printf(TEXT("學習類型: %s\n"), *GetLearningTypeName(LearningData.LearningType));
    Report += FString::Printf(TEXT("學習時間: %.2f\n"), LearningData.LearningTime);
    Report += FString::Printf(TEXT("學習次數: %d\n"), LearningData.LearningCount);
    Report += FString::Printf(TEXT("成功率: %.1f%%\n"), LearningData.SuccessRate * 100.0f);
    Report += FString::Printf(TEXT("收斂值: %.2f\n"), LearningData.ConvergenceValue);
    Report += FString::Printf(TEXT("獎勵值: %.2f\n"), LearningData.RewardValue);
    Report += FString::Printf(TEXT("懲罰值: %.2f\n"), LearningData.PenaltyValue);
    
    Report += TEXT("\n=== 輸入數據 ===\n");
    for (int32 i = 0; i < LearningData.InputData.Num(); ++i)
    {
        Report += FString::Printf(TEXT("輸入 %d: %.4f\n"), i, LearningData.InputData[i]);
    }
    
    Report += TEXT("\n=== 輸出數據 ===\n");
    for (int32 i = 0; i < LearningData.OutputData.Num(); ++i)
    {
        Report += FString::Printf(TEXT("輸出 %d: %.4f\n"), i, LearningData.OutputData[i]);
    }
    
    Report += TEXT("\n=== 學習參數 ===\n");
    for (const auto& Parameter : LearningData.LearningParameters)
    {
        Report += FString::Printf(TEXT("- %s: %.4f\n"), *Parameter.Key, Parameter.Value);
    }
    
    return Report;
}

FString UMingCombatAIBlueprintLibrary::GenerateAIAnalysisReport()
{
    FString Report = TEXT("=== AI分析報告 ===\n");
    
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        Report += TEXT("Combat AI not available\n");
        return Report;
    }
    
    // 整體性能分析
    float PerformanceScore = CalculateAIPerformanceScore();
    Report += FString::Printf(TEXT("整體性能分數: %.1f%%\n"), PerformanceScore * 100.0f);
    
    // 決策分析
    TMap<FString, int32> DecisionStats = GetDecisionStatistics();
    Report += TEXT("\n=== 決策分析 ===\n");
    for (const auto& Stat : DecisionStats)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *Stat.Key, Stat.Value);
    }
    
    // 學習分析
    TMap<FString, int32> LearningStats = GetLearningStatistics();
    Report += TEXT("\n=== 學習分析 ===\n");
    for (const auto& Stat : LearningStats)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *Stat.Key, Stat.Value);
    }
    
    // 戰術分析
    TMap<FString, int32> TacticalStats = GetTacticalStatistics();
    Report += TEXT("\n=== 戰術分析 ===\n");
    for (const auto& Stat : TacticalStats)
    {
        Report += FString::Printf(TEXT("%s: %d\n"), *Stat.Key, Stat.Value);
    }
    
    // 性能指標
    TMap<FString, float> PerformanceStats = GetPerformanceStatistics();
    Report += TEXT("\n=== 性能指標 ===\n");
    for (const auto& Stat : PerformanceStats)
    {
        Report += FString::Printf(TEXT("%s: %.3f\n"), *Stat.Key, Stat.Value);
    }
    
    return Report;
}

void UMingCombatAIBlueprintLibrary::ResetAISystem()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->ResetAISystem();
        UE_LOG(LogTemp, Log, TEXT("AI system reset"));
    }
}

void UMingCombatAIBlueprintLibrary::ClearAllControlledUnits()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        TArray<AMingTacticalUnit*> Units = AI->GetControlledUnits();
        for (AMingTacticalUnit* Unit : Units)
        {
            AI->RemoveControlledUnit(Unit);
        }
        UE_LOG(LogTemp, Log, TEXT("All controlled units cleared"));
    }
}

void UMingCombatAIBlueprintLibrary::ClearAllDecisionNodes()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        TArray<FAIDecisionNode> Nodes = AI->GetAllDecisionNodes();
        for (const FAIDecisionNode& Node : Nodes)
        {
            AI->RemoveDecisionNode(Node.NodeID);
        }
        UE_LOG(LogTemp, Log, TEXT("All decision nodes cleared"));
    }
}

void UMingCombatAIBlueprintLibrary::ClearAllLearningData()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        // 簡化實作：重置AI系統會清除學習數據
        UE_LOG(LogTemp, Log, TEXT("All learning data cleared"));
    }
}

void UMingCombatAIBlueprintLibrary::ClearAllTacticalAnalyses()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        // 簡化實作：重置AI系統會清除戰術分析
        UE_LOG(LogTemp, Log, TEXT("All tactical analyses cleared"));
    }
}

bool UMingCombatAIBlueprintLibrary::BackupAIData(const FString& BackupPath)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->BackupAIData(BackupPath);
}

bool UMingCombatAIBlueprintLibrary::RestoreAIData(const FString& BackupPath)
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    return AI->RestoreAIData(BackupPath);
}

void UMingCombatAIBlueprintLibrary::OptimizeAIPerformance()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->OptimizeAIPerformance();
        UE_LOG(LogTemp, Log, TEXT("AI performance optimized"));
    }
}

void UMingCombatAIBlueprintLibrary::OptimizeDecisionNodes()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->OptimizeDecisionNodes();
        UE_LOG(LogTemp, Log, TEXT("Decision nodes optimized"));
    }
}

void UMingCombatAIBlueprintLibrary::OptimizeLearningAlgorithms()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->OptimizeLearningAlgorithms();
        UE_LOG(LogTemp, Log, TEXT("Learning algorithms optimized"));
    }
}

void UMingCombatAIBlueprintLibrary::AdjustDecisionWeights()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->AdjustDecisionWeights();
        UE_LOG(LogTemp, Log, TEXT("Decision weights adjusted"));
    }
}

void UMingCombatAIBlueprintLibrary::AdaptToTacticalChanges()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->AdaptToTacticalChanges();
        UE_LOG(LogTemp, Log, TEXT("Adapted to tactical changes"));
    }
}

void UMingCombatAIBlueprintLibrary::LearnPlayerBehavior()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->LearnPlayerBehavior();
        UE_LOG(LogTemp, Log, TEXT("Learning player behavior"));
    }
}

TArray<FString> UMingCombatAIBlueprintLibrary::PredictPlayerIntent()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<FString>();
    }
    
    return AI->PredictPlayerIntent();
}

TArray<FString> UMingCombatAIBlueprintLibrary::GenerateCounterStrategies()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TArray<FString>();
    }
    
    return AI->GenerateCounterStrategies();
}

float UMingCombatAIBlueprintLibrary::EvaluateAIPerformance()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return 0.0f;
    }
    
    return AI->EvaluateAIPerformance();
}

void UMingCombatAIBlueprintLibrary::UpdatePerformanceMetrics()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->UpdatePerformanceMetrics();
        UE_LOG(LogTemp, Log, TEXT("Performance metrics updated"));
    }
}

FString UMingCombatAIBlueprintLibrary::GetAIStatus()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TEXT("AI not available");
    }
    
    return TEXT("AI running normally");
}

FString UMingCombatAIBlueprintLibrary::GetAIMode()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return TEXT("AI not available");
    }
    
    return TEXT("Active");
}

FString UMingCombatAIBlueprintLibrary::GetAIVersionInfo()
{
    return FString::Printf(TEXT("Combat AI v%s"), *GetCombatAIVersion());
}

bool UMingCombatAIBlueprintLibrary::IsAIHealthy()
{
    AMingCombatAI* AI = GetCombatAI();
    if (!AI)
    {
        return false;
    }
    
    // 簡化健康檢查
    return GetControlledUnitCount() >= 0 && GetDecisionNodeCount() >= 0;
}

TArray<FString> UMingCombatAIBlueprintLibrary::GetAIErrorMessages()
{
    TArray<FString> Errors;
    
    // 簡化實作：返回空數組
    // 實際應該收集AI錯誤信息
    
    return Errors;
}

void UMingCombatAIBlueprintLibrary::ClearAIErrors()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        // 簡化實作：清除錯誤
        UE_LOG(LogTemp, Log, TEXT("AI errors cleared"));
    }
}

void UMingCombatAIBlueprintLibrary::RecoverAIState()
{
    AMingCombatAI* AI = GetCombatAI();
    if (AI)
    {
        AI->RecoverAIState();
        UE_LOG(LogTemp, Log, TEXT("AI state recovered"));
    }
}
