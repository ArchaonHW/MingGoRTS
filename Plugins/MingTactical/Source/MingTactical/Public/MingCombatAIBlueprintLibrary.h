#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingCombatAI.h"
#include "MingCombatAIBlueprintLibrary.generated.h"

class AMingTacticalUnit;

/**
 * ?Æ‰??∏Á??ÖË?ÁµêÊ? (?®Êñº TArray<TArray<>> ÂµåÂ?)
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
 * ?∞È¨•AI?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á??∞È¨•AIÁ≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGTACTICAL_API UMingCombatAIBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â??∞È¨•AIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static AMingCombatAI* GetCombatAI();

    /**
     * ?ùÂ??ñÊà∞È¨•AIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void InitializeCombatAI();

    /**
     * ?úÈ??∞È¨•AIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void ShutdownCombatAI();

    /**
     * Ë®≠ÁΩÆAI??∫¶Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel);

    /**
     * Ë®≠ÁΩÆAI?∞Ë?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAITacticalType(EAITacticalType TacticalType);

    /**
     * Ë®≠ÁΩÆAIÂ≠∏Á?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static void SetAILearningType(EAILearningType LearningType);

    /**
     * Ê∑ªÂ??óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * ÁßªÈô§?óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??Ä?âÂ??ßÂñÆ‰Ω?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    static TArray<AMingTacticalUnit*> GetControlledUnits();

    /**
     * ?∑Ë?AIÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAIDecision(AMingTacticalUnit* Unit);

    /**
     * ?∑Ë??∞Ë??ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?∑Ë?AIÂ≠∏Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    static bool ExecuteAILearning(const FAILearningData& LearningData);

    /**
     * ?µÂª∫Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    );

    /**
     * Ê∑ªÂ?Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool AddDecisionNode(const FAIDecisionNode& Node);

    /**
     * ÁßªÈô§Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Decision")
    static bool RemoveDecisionNode(const FString& NodeID);

    /**
     * ?≤Â?Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static FAIDecisionNode GetDecisionNode(const FString& NodeID);

    /**
     * ?≤Â??Ä?âÊ±∫Á≠ñÁ?Èª?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Decision")
    static TArray<FAIDecisionNode> GetAllDecisionNodes();

    /**
     * Ë©ï‰º∞?∞Ë?Â±Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * ?üÊ??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?êÊ∏¨?µÊñπË°åÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Analysis")
    static TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * Ë®àÁ?Â®ÅË?Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * Ë®àÁ?Ê©üÊ?Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * ?∑Ë??ªÊ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ?∑Ë??≤Á¶¶Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition);

    /**
     * ?∑Ë?ÁßªÂ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition);

    /**
     * ?∑Ë??§ÈÄÄÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition);

    /**
     * ?∑Ë??¥ÁøºÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ?∑Ë?‰ºèÊ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits);

    /**
     * ?∑Ë??ØÊè¥Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ?∑Ë??îË™øÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType);

    /**
     * ?∑Ë?Â≠∏Á?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData);

    /**
     * ?∑Ë??©Ê?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Execution")
    static bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData);

    /**
     * Ë™øÊï¥AI??∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Difficulty")
    static void AdjustAIDifficulty(float PerformanceFactor);

    /**
     * ?≤Â?AI?ßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static FAIPerformanceMetrics GetAIPerformanceMetrics();

    /**
     * ?≤Â?AIÁµ±Ë??∏Ê?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Performance")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * ?≤Â?AI?çÁΩÆ
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Configuration")
    static TMap<FString, FString> GetAIConfiguration();

    /**
     * Ë®≠ÁΩÆAI?çÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Configuration")
    static bool SetAIConfiguration(const TMap<FString, FString>& Configuration);

    /**
     * ?≤Â?AI?Ä?ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStateName(EAIState AIState);

    /**
     * ?≤Â?Ê±∫Á?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDecisionTypeName(EAIDecisionType DecisionType);

    /**
     * ?≤Â??∞Ë?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetTacticalTypeName(EAITacticalType TacticalType);

    /**
     * ?≤Â???∫¶Á≠âÁ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetDifficultyLevelName(EAIDifficultyLevel DifficultyLevel);

    /**
     * ?≤Â?Â≠∏Á?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetLearningTypeName(EAILearningType LearningType);

    /**
     * ?µÂª∫Â≠∏Á??∏Ê?
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
     * Ë®≠ÁΩÆÊ±∫Á?ÁØÄÈªûÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode SetDecisionNodeParameter(const FAIDecisionNode& Node, const FString& ParameterName, const FString& ParameterValue);

    /**
     * Ê∑ªÂ?Ê±∫Á?ÁØÄÈªûÂ?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAIDecisionNode AddDecisionNodeChild(const FAIDecisionNode& Node, const FString& ChildNodeID);

    /**
     * ?µÂª∫?∞Ë??ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis CreateTacticalAnalysis(
        float ThreatAssessment,
        float OpportunityAssessment,
        float AdvantageAssessment,
        float DisadvantageAssessment
    );

    /**
     * Ê∑ªÂ??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddTacticalRecommendation(const FAITacticalAnalysis& Analysis, const FString& Recommendation);

    /**
     * Ê∑ªÂ?È¢®Èö™Ë©ï‰º∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddRiskAssessment(const FAITacticalAnalysis& Analysis, const FString& Risk);

    /**
     * Ê∑ªÂ?Ê©üÊ?Ë≠òÂà•
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddOpportunityIdentification(const FAITacticalAnalysis& Analysis, const FString& Opportunity);

    /**
     * Ê∑ªÂ??µÊñπ?êÊ∏¨
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Creation")
    static FAITacticalAnalysis AddEnemyPrediction(const FAITacticalAnalysis& Analysis, const FString& Prediction);

    /**
     * ?πÈ??∑Ë?AIÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAIDecisions(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?πÈ?Ê∑ªÂ??óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchAddControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?πÈ?ÁßªÈô§?óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchRemoveControlledUnits(const TArray<AMingTacticalUnit*>& Units);

    // ?πÈ??∑Ë??∞Ë??ÜÊ? - ‰ΩøÁî® FUnitArrayWrapper ?ÖË?ÁµêÊ?
    static TArray<FAITacticalAnalysis> BatchExecuteTacticalAnalysis(const TArray<FUnitArrayWrapper>& UnitGroups);

    /**
     * ?πÈ??∑Ë?AIÂ≠∏Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Batch")
    static TArray<bool> BatchExecuteAILearning(const TArray<FAILearningData>& LearningDataArray);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsCombatAIInitialized();

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static FString GetCombatAIVersion();

    /**
     * ?≤Â??óÊéß?Æ‰?Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetControlledUnitCount();

    /**
     * ?≤Â?Ê±∫Á?ÁØÄÈªûÁ∏Ω??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetDecisionNodeCount();

    /**
     * ?≤Â?Â≠∏Á??∏Ê?Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetLearningDataCount();

    /**
     * ?≤Â??∞Ë??ÜÊ?Á∏ΩÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static int32 GetTacticalAnalysisCount();

    /**
     * Ê™¢Êü•?Æ‰??ØÂê¶?óÊéß
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsUnitControlled(AMingTacticalUnit* Unit);

    /**
     * Ê™¢Êü•Ê±∫Á?ÁØÄÈªûÊòØ?¶Â???     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool DoesDecisionNodeExist(const FString& NodeID);

    /**
     * Ê™¢Êü•Ê±∫Á?ÁØÄÈªûÊòØ?¶Â???     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static bool IsDecisionNodeEnabled(const FString& NodeID);

    /**
     * ?≤Â?Ê±∫Á?ÁØÄÈªûÂü∑Ë°åÊ¨°??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static int32 GetDecisionNodeExecutionCount(const FString& NodeID);

    /**
     * ?≤Â?Ê±∫Á?ÁØÄÈªûÊ??üÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static float GetDecisionNodeSuccessRate(const FString& NodeID);

    /**
     * Ë®àÁ?AI?ßËÉΩ?ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateAIPerformanceScore();

    /**
     * Ë®àÁ?Ê±∫Á?Ë≥™È??ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateDecisionQualityScore(const FString& NodeID);

    /**
     * Ë®àÁ??∞Ë??ÜÊ??ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateTacticalAnalysisScore(const FAITacticalAnalysis& Analysis);

    /**
     * Ë®àÁ?Â≠∏Á??àÊ??ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Calculation")
    static float CalculateLearningEffectScore(const FAILearningData& LearningData);

    /**
     * ?≤Â??Ä‰Ω≥Ê±∫Á≠ñÁ?Èª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetBestDecisionNode(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??ÄÂ∑ÆÊ±∫Á≠ñÁ?Èª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static FAIDecisionNode GetWorstDecisionNode(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??®Ëñ¶Ê±∫Á?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDecisionType GetRecommendedDecisionType(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??®Ëñ¶?∞Ë?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAITacticalType GetRecommendedTacticalType(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??®Ëñ¶??∫¶Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static EAIDifficultyLevel GetRecommendedDifficultyLevel(float PlayerPerformance);

    /**
     * ?≤Â?AIÂª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetAIRecommendations(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??∞Áï•Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GetStrategicRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ?≤Â??ªÊ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<AMingTacticalUnit*> GetAttackRecommendations(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??≤Á¶¶Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetDefenseRecommendations(AMingTacticalUnit* Unit);

    /**
     * ?≤Â?ÁßªÂ?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetMovementRecommendations(AMingTacticalUnit* Unit);

    /**
     * ?≤Â??§ÈÄÄÂª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FVector> GetRetreatRecommendations(AMingTacticalUnit* Unit);

    /**
     * Ê®°Êì¨AIÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAIDecisions(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * Ê®°Êì¨?∞Ë??ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<FAITacticalAnalysis> SimulateTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units, int32 SimulationCount);

    /**
     * Ê®°Êì¨AIÂ≠∏Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Simulation")
    static TArray<bool> SimulateAILearning(const TArray<FAILearningData>& LearningDataArray, int32 SimulationCount);

    /**
     * ?≤Â?AIÊ≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetAIHistory();

    /**
     * ?≤Â?Ê±∫Á?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetDecisionHistory();

    /**
     * ?≤Â?Â≠∏Á?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetLearningHistory();

    /**
     * ?≤Â??∞Ë?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|History")
    static TArray<FString> GetTacticalHistory();

    /**
     * ?≤Â?AIÁµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetAIStatistics_BPLibrary();

    /**
     * ?≤Â?Ê±∫Á?Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetDecisionStatistics();

    /**
     * ?≤Â?Â≠∏Á?Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetLearningStatistics();

    /**
     * ?≤Â??∞Ë?Áµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, int32> GetTacticalStatistics();

    /**
     * ?≤Â??ßËÉΩÁµ±Ë?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Statistics")
    static TMap<FString, float> GetPerformanceStatistics();

    /**
     * Â∞éÂá∫AI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Export")
    static bool ExportAIData(const FString& FilePath);

    /**
     * Â∞éÂÖ•AI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Import")
    static bool ImportAIData(const FString& FilePath);

    /**
     * ?≤Â?AI?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIReport();

    /**
     * ?≤Â??Æ‰?AI?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateUnitAIReport(AMingTacticalUnit* Unit);

    /**
     * ?≤Â?Ê±∫Á??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateDecisionReport(const FString& NodeID);

    /**
     * ?≤Â??∞Ë??ÜÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateTacticalAnalysisReport(const FAITacticalAnalysis& Analysis);

    /**
     * ?≤Â?Â≠∏Á??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateLearningReport(const FAILearningData& LearningData);

    /**
     * ?≤Â?AI?ÜÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Report")
    static FString GenerateAIAnalysisReport();

    /**
     * ?çÁΩÆAIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ResetAISystem();

    /**
     * Ê∏ÖÈô§?Ä?âÂ??ßÂñÆ‰Ω?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllControlledUnits();

    /**
     * Ê∏ÖÈô§?Ä?âÊ±∫Á≠ñÁ?Èª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllDecisionNodes();

    /**
     * Ê∏ÖÈô§?Ä?âÂ≠∏ÁøíÊï∏??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllLearningData();

    /**
     * Ê∏ÖÈô§?Ä?âÊà∞Ë°ìÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAllTacticalAnalyses();

    /**
     * ?ô‰ªΩAI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool BackupAIData(const FString& BackupPath);

    /**
     * ?¢Âæ©AI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static bool RestoreAIData(const FString& BackupPath);

    /**
     * ?™Â?AI?ßËÉΩ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeAIPerformance();

    /**
     * ?™Â?Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeDecisionNodes();

    /**
     * ?™Â?Â≠∏Á?ÁÆóÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void OptimizeLearningAlgorithms();

    /**
     * Ë™øÊï¥Ê±∫Á?Ê¨äÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdjustDecisionWeights();

    /**
     * ?©Ê??∞Ë?ËÆäÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Optimization")
    static void AdaptToTacticalChanges();

    /**
     * Â≠∏Á??©ÂÆ∂Ë°åÁÇ∫
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static void LearnPlayerBehavior();

    /**
     * ?êÊ∏¨?©ÂÆ∂?èÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> PredictPlayerIntent();

    /**
     * ?üÊ??çÂà∂Á≠ñÁï•
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|AI")
    static TArray<FString> GenerateCounterStrategies();

    /**
     * Ë©ï‰º∞AIË°®Áèæ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static float EvaluateAIPerformance();

    /**
     * ?¥Êñ∞?ßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|Performance")
    static void UpdatePerformanceMetrics();

    /**
     * ?≤Â?AI?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIStatus();

    /**
     * ?≤Â?AIÊ®°Â?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIMode();

    /**
     * ?≤Â?AI?àÊú¨‰ø°ÊÅØ
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|Info")
    static FString GetAIVersionInfo();

    /**
     * Ê™¢Êü•AI?•Â∫∑?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static bool IsAIHealthy();

    /**
     * ?≤Â?AI?ØË™§‰ø°ÊÅØ
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI|System")
    static TArray<FString> GetAIErrorMessages();

    /**
     * Ê∏ÖÈô§AI?ØË™§
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void ClearAIErrors();

    /**
     * ?¢Âæ©AI?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI|System")
    static void RecoverAIState();
};
