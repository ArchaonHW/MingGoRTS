#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingCombatAI.h"
#include "MingCombatAIBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ???X??X???結?? (??於 TArray<TArray<>> 嵌??)
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
 * ??鬥AI???X?數?? * ???X??X?調???X?鬥AI系統??能
 */
UCLASS()
class MINGTACTICAL_API UMingCombatAIBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ???X?鬥AI系統
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static AMingCombatAI* GetCombatAI();

    /**
     * ???X?戰鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void InitializeCombatAI();

    /**
     * ???X?鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void ShutdownCombatAI();

    /**
     * 設置AIX??等??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel};

    /**
     * 設置AI????類??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAITacticalType(EAITacticalType TacticalType};

    /**
     * 設置AI學??類??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAILearningType(EAILearningType LearningType};

    /**
     * 添?X?控????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool AddControlledUnit(AMingTacticalUnit* Unit};

    /**
     * 移除??控????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ???X????X?單??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * ????AI決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAIDecision(AMingTacticalUnit* Unit};

    /**
     * ???X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ????AI學??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAILearning(const FAILearningData& LearningData};

    /**
     * ??建決??節??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    };

    /**
     * 添??決??節??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool AddDecisionNode(const FAIDecisionNode& Node};

    /**
     * 移除決??節??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool RemoveDecisionNode(const FString& NodeID};

    /**
     * ????決??節??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static FAIDecisionNode GetDecisionNode(const FString& NodeID};

    /**
     * ???X???決策????     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static TArray<FAIDecisionNode> GetAllDecisionNodes();

    /**
     * 評估????局X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ??測??方行??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * 計??威??等??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * 計??機??等??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * ???X???決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ???X?禦決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition};

    /**
     * ????移??決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition};

    /**
     * ???X?退決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition};

    /**
     * ???X?翼決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ????伏??決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits};

    /**
     * ???X?援決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ???X?調決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType};

    /**
     * ????學??決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData};

    /**
     * ???X???決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData};

    /**
     * 調整AIX??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Difficulty")
    static void AdjustAIDifficulty(float PerformanceFactor};

    /**
     * ????AI??能????
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static FAIPerformanceMetrics GetAIPerformanceMetrics();

    /**
     * ????AI統?X???
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * ????AI??置
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Configuration")
    static TMap<FString, FString> GetAIConfiguration();

    /**
     * 設置AI??置
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Configuration")
    static bool SetAIConfiguration(const TMap<FString, FString>& Configuration};

    /**
     * ????AI????????     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStateName(EAIState AIState};

    /**
     * ????決??類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDecisionTypeName(EAIDecisionType DecisionType};

    /**
     * ???X???類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetTacticalTypeName(EAITacticalType TacticalType};

    /**
     * ???X??等?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDifficultyLevelName(EAIDifficultyLevel DifficultyLevel};

    /**
     * ????學??類?X?稱
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetLearningTypeName(EAILearningType LearningType};

    /**
     * ??建學?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAILearningData CreateLearningData(
        EAILearningType LearningType,
        const TArray<float>& InputData,
        const TArray<float>& OutputData,
        float RewardValue,
        float PenaltyValue
    };

    /**
     * 設置決??節點?X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode SetDecisionNodeParameter(const FAIDecisionNode& Node, const FString& ParameterName, const FString& ParameterValue};

    /**
     * 添??決??節點??節??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode AddDecisionNodeChild(const FAIDecisionNode& Node, const FString& ChildNodeID};

    /**
     * ??建???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis CreateTacticalAnalysis(
        float ThreatAssessment,
        float OpportunityAssessment,
        float AdvantageAssessment,
        float DisadvantageAssessment
    };

    /**
     * 添?X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddTacticalRecommendation(const FAITacticalAnalysis& Analysis, const FString& Recommendation};

    /**
     * 添??風險評估
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddRiskAssessment(const FAITacticalAnalysis& Analysis, const FString& Risk};

    /**
     * 添??機??識別
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddOpportunityIdentification(const FAITacticalAnalysis& Analysis, const FString& Opportunity};

    /**
     * 添?X?方??測
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddEnemyPrediction(const FAITacticalAnalysis& Analysis, const FString& Prediction};

    /**
     * ???X???AI決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAIDecisions(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ????添?X?控????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchAddControlledUnits(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ????移除??控????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchRemoveControlledUnits(const TArray<AMingTacticalUnit*>& Units};

    // ???X??X??X??? - 使用 FUnitArrayWrapper ????結??
    static TArray<FAITacticalAnalysis> BatchExecuteTacticalAnalysis(const TArray<FUnitArrayWrapper>& UnitGroups};

    /**
     * ???X???AI學??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAILearning(const TArray<FAILearningData>& LearningDataArray};

    /**
     * ????系統??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsCombatAIInitialized();

    /**
     * ????系統??本
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static FString GetCombatAIVersion();

    /**
     * ???X?控????總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetControlledUnitCount();

    /**
     * ????決??節點總X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetDecisionNodeCount();

    /**
     * ????學?X???總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetLearningDataCount();

    /**
     * ???X??X???總數
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetTacticalAnalysisCount();

    /**
     * 檢查???X?否??控
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit};

    /**
     * 檢查決??節點是???X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool DoesDecisionNodeExist(const FString& NodeID};

    /**
     * 檢查決??節點是???X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsDecisionNodeEnabled(const FString& NodeID};

    /**
     * ????決??節點執行次X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static int32 GetDecisionNodeExecutionCount(const FString& NodeID};

    /**
     * ????決??節點?X???
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static float GetDecisionNodeSuccessRate(const FString& NodeID};

    /**
     * 計??AI??能??數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateAIPerformanceScore();

    /**
     * 計??決??質?X?數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateDecisionQualityScore(const FString& NodeID};

    /**
     * 計?X??X??X?數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateTacticalAnalysisScore(const FAITacticalAnalysis& Analysis};

    /**
     * 計??學?X??X?數
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateLearningEffectScore(const FAILearningData& LearningData};

    /**
     * ???X?佳決策????     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetBestDecisionNode(AMingTacticalUnit* Unit};

    /**
     * ???X?差決策????     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetWorstDecisionNode(AMingTacticalUnit* Unit};

    /**
     * ???X?薦決??類??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDecisionType GetRecommendedDecisionType(AMingTacticalUnit* Unit};

    /**
     * ???X?薦????類??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAITacticalType GetRecommendedTacticalType(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X?薦X??等??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDifficultyLevel GetRecommendedDifficultyLevel(float PlayerPerformance};

    /**
     * ????AI建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetAIRecommendations(AMingTacticalUnit* Unit};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X?略建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetStrategicRecommendations(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ???X???建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<AMingTacticalUnit*> GetAttackRecommendations(AMingTacticalUnit* Unit};

    /**
     * ???X?禦建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetDefenseRecommendations(AMingTacticalUnit* Unit};

    /**
     * ????移??建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetMovementRecommendations(AMingTacticalUnit* Unit};

    /**
     * ???X?退建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetRetreatRecommendations(AMingTacticalUnit* Unit};

    /**
     * 模擬AI決??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAIDecisions(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount};

    /**
     * 模擬???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<FAITacticalAnalysis> SimulateTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount};

    /**
     * 模擬AI學??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAILearning(const TArray<FAILearningData>& LearningDataArray, int32 SimulationCount};

    /**
     * ????AI歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetAIHistory();

    /**
     * ????決??歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetDecisionHistory();

    /**
     * ????學??歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetLearningHistory();

    /**
     * ???X???歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetTacticalHistory();

    /**
     * ????AI統??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * ????決??統??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetDecisionStatistics();

    /**
     * ????學??統??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetLearningStatistics();

    /**
     * ???X???統??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetTacticalStatistics();

    /**
     * ???X?能統??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * 導出AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Export")
    static bool ExportAIData(const FString& FilePath};

    /**
     * 導入AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Import")
    static bool ImportAIData(const FString& FilePath};

    /**
     * ????AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIReport();

    /**
     * ???X???AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateUnitAIReport(AMingTacticalUnit* Unit};

    /**
     * ????決?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateDecisionReport(const FString& NodeID};

    /**
     * ???X??X??X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateTacticalAnalysisReport(const FAITacticalAnalysis& Analysis};

    /**
     * ????學?X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateLearningReport(const FAILearningData& LearningData};

    /**
     * ????AI???X???
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIAnalysisReport();

    /**
     * ??置AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ResetAISystem();

    /**
     * 清除?????X?單??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllControlledUnits();

    /**
     * 清除????決策????     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllDecisionNodes();

    /**
     * 清除????學習數X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllLearningData();

    /**
     * 清除????戰術?X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllTacticalAnalyses();

    /**
     * ??份AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool BackupAIData(const FString& BackupPath};

    /**
     * ??復AI????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool RestoreAIData(const FString& BackupPath};

    /**
     * ????AI??能
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeAIPerformance();

    /**
     * ????決??節??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeDecisionNodes();

    /**
     * ????學??算??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeLearningAlgorithms();

    /**
     * 調整決??權??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdjustDecisionWeights();

    /**
     * ???X???變??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdaptToTacticalChanges();

    /**
     * 學?X?家行為
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static void LearnPlayerBehavior();

    /**
     * ??測??家????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> PredictPlayerIntent();

    /**
     * ???X?制策略
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GenerateCounterStrategies();

    /**
     * 評估AI表現
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static float EvaluateAIPerformance();

    /**
     * ??新??能????
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static void UpdatePerformanceMetrics();

    /**
     * ????AI??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStatus();

    /**
     * ????AI模??
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIMode();

    /**
     * ????AI??本信息
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIVersionInfo();

    /**
     * 檢查AI??康??X     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsAIHealthy();

    /**
     * ????AI??誤信息
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static TArray<FString> GetAIErrorMessages();

    /**
     * 清除AI??誤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAIErrors();

    /**
     * ??復AI??X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void RecoverAIState();
};

