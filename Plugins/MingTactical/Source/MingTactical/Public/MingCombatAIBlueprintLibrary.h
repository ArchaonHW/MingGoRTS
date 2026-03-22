#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingCombatAI.h"
#include "MingCombatAIBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * 故事重要性摧毀? (?? TArray<TArray<>> O??)
 */
USTRUCT(BlueprintType)
struct FCombatAIUnitArrayWrapper
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    TArray<AMingTacticalUnit*> Units;

    FCombatAIUnitArrayWrapper() {}
};

/**
 * ??AI目標數量 * 故事重要性摧毀X?AIt??
 */
UCLASS()
class MINGTACTICAL_API UMingCombatAIBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ???X?AIt
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static AMingCombatAI* GetCombatAI();

    /**
     * ???X?԰AIt
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void InitializeCombatAI();

    /**
     * ???X?AIt
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void ShutdownCombatAI();

    /**
     * ]mAIX摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel);

    /**
     * ]mAI摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAITacticalType(EAITacticalType TacticalType);

    /**
     * ]mAI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAILearningType(EAILearningType LearningType);

    /**
     * K?X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 目標數量?X???     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * 摧毀AIM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAIDecision(AMingTacticalUnit* Unit);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀AI??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAILearning(const FAILearningData& LearningData);

    /**
     * ??بM??`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    );

    /**
     * K??M??`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool AddDecisionNode(const FAIDecisionNode& Node);

    /**
     * M??`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool RemoveDecisionNode(const FString& NodeID);

    /**
     * 摧毀M??`??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static FAIDecisionNode GetDecisionNode(const FString& NodeID);

    /**
     * 目標數量M摧毀     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static TArray<FAIDecisionNode> GetAllDecisionNodes();

    /**
     * 摧毀X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * p摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * p摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 目標數量M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ???X?mM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition);

    /**
     * 摧毀??M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition);

    /**
     * ???X?hM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition);

    /**
     * ???X?lM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 摧毀??M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits);

    /**
     * ???X?M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ???X?ըM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType);

    /**
     * 摧毀??M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData);

    /**
     * 目標數量M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData);

    /**
     * վAIX??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Difficulty")
    static void AdjustAIDifficulty(float PerformanceFactor);

    /**
     * 摧毀AI摧毀??
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static FAIPerformanceMetrics GetAIPerformanceMetrics();

    /**
     * 摧毀AI?X???
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * 摧毀AI??m
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Configuration")
    static TMap<FString, FString> GetAIConfiguration();

    /**
     * ]mAI??m
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Configuration")
    static bool SetAIConfiguration(const TMap<FString, FString>& Configuration);

    /**
     * 摧毀AI摧毀摧毀     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStateName(EAIState AIState);

    /**
     * 摧毀M???X?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDecisionTypeName(EAIDecisionType DecisionType);

    /**
     * 目標數量?X?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetTacticalTypeName(EAITacticalType TacticalType);

    /**
     * 目標數量X?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDifficultyLevelName(EAIDifficultyLevel DifficultyLevel);

    /**
     * 摧毀???X?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetLearningTypeName(EAILearningType LearningType);

    /**
     * ??ؾ?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAILearningData CreateLearningData(
        EAILearningType LearningType,
        const TArray<float>& InputData,
        const TArray<float>& OutputData,
        float RewardValue,
        float PenaltyValue
    );

    /**
     * ]mM??`I?X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode SetDecisionNodeParameter(const FAIDecisionNode& Node, const FString& ParameterName, const FString& ParameterValue);

    /**
     * K??M??`I??`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode AddDecisionNodeChild(const FAIDecisionNode& Node, const FString& ChildNodeID);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis CreateTacticalAnalysis(
        float ThreatAssessment,
        float OpportunityAssessment,
        float AdvantageAssessment,
        float DisadvantageAssessment
    );

    /**
     * K?X???ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddTacticalRecommendation(const FAITacticalAnalysis& Analysis, const FString& Recommendation);

    /**
     * K??I
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddRiskAssessment(const FAITacticalAnalysis& Analysis, const FString& Risk);

    /**
     * K摧毀ѧO
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddOpportunityIdentification(const FAITacticalAnalysis& Analysis, const FString& Opportunity);

    /**
     * K?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddEnemyPrediction(const FAITacticalAnalysis& Analysis, const FString& Prediction);

    /**
     * 目標數量AIM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAIDecisions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀K?X摧毀?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchAddControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchRemoveControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    // 故事選項X??? - ϥ FUnitArrayWrapper 摧毀??
    static TArray<FAITacticalAnalysis> BatchExecuteTacticalAnalysis(const TArray<FUnitArrayWrapper>& UnitGroups);

    /**
     * 目標數量AI??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAILearning(const TArray<FAILearningData>& LearningDataArray);

    /**
     * 摧毀t??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsCombatAIInitialized();

    /**
     * 摧毀t??
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static FString GetCombatAIVersion();

    /**
     * 目標數量??`
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetControlledUnitCount();

    /**
     * 摧毀M??`I`X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetDecisionNodeCount();

    /**
     * ??目標數量`
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetLearningDataCount();

    /**
     * 故事選項?`
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetTacticalAnalysisCount();

    /**
     * ˬd???X?_??
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * ˬdM??`IO???X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool DoesDecisionNodeExist(const FString& NodeID);

    /**
     * ˬdM??`IO???X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsDecisionNodeEnabled(const FString& NodeID);

    /**
     * 摧毀M??`I榸X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static int32 GetDecisionNodeExecutionCount(const FString& NodeID);

    /**
     * 摧毀M??`I?X???
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static float GetDecisionNodeSuccessRate(const FString& NodeID);

    /**
     * p??AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateAIPerformanceScore();

    /**
     * p??M???X?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateDecisionQualityScore(const FString& NodeID);

    /**
     * p?X??X??X?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateTacticalAnalysisScore(const FAITacticalAnalysis& Analysis);

    /**
     * p故事重要性?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateLearningEffectScore(const FAILearningData& LearningData);

    /**
     * ???X?ΨM摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetBestDecisionNode(AMingTacticalUnit* Unit);

    /**
     * ???X?tM摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetWorstDecisionNode(AMingTacticalUnit* Unit);

    /**
     * ???X?˨M摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDecisionType GetRecommendedDecisionType(AMingTacticalUnit* Unit);

    /**
     * 目標數量摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAITacticalType GetRecommendedTacticalType(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ???X?X摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDifficultyLevel GetRecommendedDifficultyLevel(float PlayerPerformance);

    /**
     * 摧毀AIĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetAIRecommendations(AMingTacticalUnit* Unit);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ???X?ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetStrategicRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 目標數量ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<AMingTacticalUnit*> GetAttackRecommendations(AMingTacticalUnit* Unit);

    /**
     * ???X?mĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetDefenseRecommendations(AMingTacticalUnit* Unit);

    /**
     * 摧毀??ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetMovementRecommendations(AMingTacticalUnit* Unit);

    /**
     * ???X?hĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetRetreatRecommendations(AMingTacticalUnit* Unit);

    /**
     * AIM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAIDecisions(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<FAITacticalAnalysis> SimulateTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * AI??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAILearning(const TArray<FAILearningData>& LearningDataArray, int32 SimulationCount);

    /**
     * 摧毀AIv
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetAIHistory();

    /**
     * 摧毀M??v
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetDecisionHistory();

    /**
     * 摧毀??v
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetLearningHistory();

    /**
     * 目標數量v
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetTacticalHistory();

    /**
     * 摧毀AI??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * 摧毀M摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetDecisionStatistics();

    /**
     * 摧毀摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetLearningStatistics();

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetTacticalStatistics();

    /**
     * 目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * ɥXAI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Export")
    static bool ExportAIData(const FString& FilePath);

    /**
     * ɤJAI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Import")
    static bool ImportAIData(const FString& FilePath);

    /**
     * 摧毀AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIReport();

    /**
     * 目標數量AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateUnitAIReport(AMingTacticalUnit* Unit);

    /**
     * 摧毀M?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateDecisionReport(const FString& NodeID);

    /**
     * 故事選項X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateTacticalAnalysisReport(const FAITacticalAnalysis& Analysis);

    /**
     * ??目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateLearningReport(const FAILearningData& LearningData);

    /**
     * 摧毀AI目標數量
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIAnalysisReport();

    /**
     * ??mAIt
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ResetAISystem();

    /**
     * M??目標數量     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllControlledUnits();

    /**
     * M摧毀M摧毀     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllDecisionNodes();

    /**
     * M摧毀ǲ߼X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllLearningData();

    /**
     * M摧毀ԳN?X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllTacticalAnalyses();

    /**
     * ??AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool BackupAIData(const FString& BackupPath);

    /**
     * ??_AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool RestoreAIData(const FString& BackupPath);

    /**
     * 摧毀AI??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeAIPerformance();

    /**
     * 摧毀M??`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeDecisionNodes();

    /**
     * 摧毀摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeLearningAlgorithms();

    /**
     * վM??v??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdjustDecisionWeights();

    /**
     * 目標數量??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdaptToTacticalChanges();

    /**
     * ?X?a欰
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static void LearnPlayerBehavior();

    /**
     * 摧毀a摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> PredictPlayerIntent();

    /**
     * ???X?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GenerateCounterStrategies();

    /**
     * AI{
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static float EvaluateAIPerformance();

    /**
     * ??s摧毀??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static void UpdatePerformanceMetrics();

    /**
     * 摧毀AI??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStatus();

    /**
     * 摧毀AI??
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIMode();

    /**
     * 摧毀AI??H
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIVersionInfo();

    /**
     * ˬdAI??d??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsAIHealthy();

    /**
     * 摧毀AI??~H
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static TArray<FString> GetAIErrorMessages();

    /**
     * MAI??~
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAIErrors();

    /**
     * ??_AI??X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void RecoverAIState();
};

