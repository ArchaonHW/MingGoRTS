#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingCombatAI.generated.h"

class AMingTacticalUnit;
class AMingUnitController;
class AMingMultiUnitCoordinator;

/**
 * AIË°åÁÇ∫?Ä?? */
UENUM(BlueprintType)
enum class EAIState : uint8
{
    Idle                UMETA(DisplayName = "Idle"),
    Scouting            UMETA(DisplayName = "Scouting"),
    Patrolling          UMETA(DisplayName = "Patrolling"),
    Engaging            UMETA(DisplayName = "Engaging"),
    Attacking           UMETA(DisplayName = "Attacking"),
    Defending           UMETA(DisplayName = "Defending"),
    Retreating           UMETA(DisplayName = "Retreating"),
    Flanking            UMETA(DisplayName = "Flanking"),
    Supporting          UMETA(DisplayName = "Supporting"),
    Healing             UMETA(DisplayName = "Healing"),
    Repairing           UMETA(DisplayName = "Repairing"),
    Building            UMETA(DisplayName = "Building"),
    Capturing           UMETA(DisplayName = "Capturing"),
    Guarding            UMETA(DisplayName = "Guarding"),
    Ambushing           UMETA(DisplayName = "Ambushing"),
    Coordinating        UMETA(DisplayName = "Coordinating"),
    Learning            UMETA(DisplayName = "Learning"),
    Adapting            UMETA(DisplayName = "Adapting"),
    Analyzing           UMETA(DisplayName = "Analyzing")
};

/**
 * AIÊ±∫Á?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EAIDecisionType : uint8
{
    Movement            UMETA(DisplayName = "Movement"),
    Attack              UMETA(DisplayName = "Attack"),
    Defense             UMETA(DisplayName = "Defense"),
    Support             UMETA(DisplayName = "Support"),
    Retreat             UMETA(DisplayName = "Retreat"),
    Flank               UMETA(DisplayName = "Flank"),
    Ambush              UMETA(DisplayName = "Ambush"),
    Capture             UMETA(DisplayName = "Capture"),
    Build               UMETA(DisplayName = "Build"),
    Repair              UMETA(DisplayName = "Repair"),
    Heal                UMETA(DisplayName = "Heal"),
    Scout               UMETA(DisplayName = "Scout"),
    Patrol              UMETA(DisplayName = "Patrol"),
    Guard               UMETA(DisplayName = "Guard"),
    Coordinate          UMETA(DisplayName = "Coordinate"),
    Learn               UMETA(DisplayName = "Learn"),
    Adapt               UMETA(DisplayName = "Adapt")
};

/**
 * AI?∞Ë?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EAITacticalType : uint8
{
    Aggressive          UMETA(DisplayName = "Aggressive"),
    Defensive           UMETA(DisplayName = "Defensive"),
    Balanced            UMETA(DisplayName = "Balanced"),
    Cautious            UMETA(DisplayName = "Cautious"),
    Opportunistic       UMETA(DisplayName = "Opportunistic"),
    Adaptive            UMETA(DisplayName = "Adaptive"),
    Reactive            UMETA(DisplayName = "Reactive"),
    Proactive           UMETA(DisplayName = "Proactive"),
    Strategic           UMETA(DisplayName = "Strategic"),
    Tactical            UMETA(DisplayName = "Tactical"),
    Guerrilla           UMETA(DisplayName = "Guerrilla"),
    Conventional        UMETA(DisplayName = "Conventional"),
    Blitzkrieg          UMETA(DisplayName = "Blitzkrieg"),
    Attrition           UMETA(DisplayName = "Attrition"),
    Maneuver            UMETA(DisplayName = "Maneuver"),
    Siege               UMETA(DisplayName = "Siege"),
    Raiding             UMETA(DisplayName = "Raiding"),
    Skirmish            UMETA(DisplayName = "Skirmish")
};

/**
 * AI??∫¶Á≠âÁ?
 */
UENUM(BlueprintType)
enum class EAIDifficultyLevel : uint8
{
    VeryEasy            UMETA(DisplayName = "Very Easy"),
    Easy                UMETA(DisplayName = "Easy"),
    Normal              UMETA(DisplayName = "Normal"),
    Hard                UMETA(DisplayName = "Hard"),
    VeryHard            UMETA(DisplayName = "Very Hard"),
    Insane              UMETA(DisplayName = "Insane"),
    Custom              UMETA(DisplayName = "Custom"),
    Adaptive            UMETA(DisplayName = "Adaptive"),
    Dynamic             UMETA(DisplayName = "Dynamic"),
    Learning            UMETA(DisplayName = "Learning")
};

/**
 * AIÂ≠∏Á?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EAILearningType : uint8
{
    Reinforcement       UMETA(DisplayName = "Reinforcement"),
    Supervised          UMETA(DisplayName = "Supervised"),
    Unsupervised        UMETA(DisplayName = "Unsupervised"),
    Evolutionary         UMETA(DisplayName = "Evolutionary"),
    Neural              UMETA(DisplayName = "Neural"),
    DecisionTree        UMETA(DisplayName = "Decision Tree"),
    BehaviorTree        UMETA(DisplayName = "Behavior Tree"),
    StateMachine        UMETA(DisplayName = "State Machine"),
    Fuzzy               UMETA(DisplayName = "Fuzzy"),
    Genetic             UMETA(DisplayName = "Genetic"),
    Swarm               UMETA(DisplayName = "Swarm"),
    Hybrid              UMETA(DisplayName = "Hybrid")
};

/**
 * AIÊ±∫Á?ÁØÄÈª? */
USTRUCT(BlueprintType)
struct FAIDecisionNode
{
    GENERATED_BODY()

    // ÁØÄÈªûID
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;

    // ÁØÄÈªûÂ?Á®?    UPROPERTY(BlueprintReadOnly)
    FString NodeName;

    // ÁØÄÈªûÈ???    UPROPERTY(BlueprintReadOnly)
    EAIDecisionType DecisionType;

    // Ê¢ù‰ª∂Ë°®È?Âº?    UPROPERTY(BlueprintReadOnly)
    FString ConditionExpression;

    // Ê¨äÈ?
    UPROPERTY(BlueprintReadOnly)
    float Weight;

    // ?™Â?Á¥?    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    // Â≠êÁ?Èª?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ChildNodes;

    // ?∑Ë?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 ExecutionCount;

    // ?êÂ?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 SuccessCount;

    // Â§±Ê?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 FailureCount;

    // ?ÄÂæåÂü∑Ë°åÊ???    UPROPERTY(BlueprintReadOnly)
    float LastExecutionTime;

    // ÁØÄÈªûÂ???    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> NodeParameters;

    // ?ØÂê¶?üÁî®
    UPROPERTY(BlueprintReadOnly)
    bool bIsEnabled;

    FAIDecisionNode()
        : NodeID(TEXT(""))
        , NodeName(TEXT(""))
        , DecisionType(EAIDecisionType::Movement)
        , ConditionExpression(TEXT(""))
        , Weight(1.0f)
        , Priority(1)
        , ExecutionCount(0)
        , SuccessCount(0)
        , FailureCount(0)
        , LastExecutionTime(0.0f)
        , bIsEnabled(true)
    {}
};

/**
 * AI?∞Ë??ÜÊ?ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FAITacticalAnalysis
{
    GENERATED_BODY()

    // ?ÜÊ?ID
    UPROPERTY(BlueprintReadOnly)
    FString AnalysisID;

    // ?ÜÊ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float AnalysisTime;

    // Â®ÅË?Ë©ï‰º∞
    UPROPERTY(BlueprintReadOnly)
    float ThreatAssessment;

    // Ê©üÊ?Ë©ï‰º∞
    UPROPERTY(BlueprintReadOnly)
    float OpportunityAssessment;

    // ?™Âã¢Ë©ï‰º∞
    UPROPERTY(BlueprintReadOnly)
    float AdvantageAssessment;

    // ??ã¢Ë©ï‰º∞
    UPROPERTY(BlueprintReadOnly)
    float DisadvantageAssessment;

    // ?∞Ë?Âª∫Ë≠∞
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TacticalRecommendations;

    // È¢®Èö™Ë©ï‰º∞
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskAssessments;

    // Ê©üÊ?Ë≠òÂà•
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> OpportunityIdentifications;

    // ?µÊñπ?êÊ∏¨
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> EnemyPredictions;

    // ?∞Ë??ÜÊï∏
    UPROPERTY(BlueprintReadOnly)
    float TacticalScore;

    // ÁΩÆ‰ø°Â∫?    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    // ?ÜÊ??ÉÊï∏
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> AnalysisParameters;

    FAITacticalAnalysis()
        : AnalysisID(TEXT(""))
        , AnalysisTime(0.0f)
        , ThreatAssessment(0.0f)
        , OpportunityAssessment(0.0f)
        , AdvantageAssessment(0.0f)
        , DisadvantageAssessment(0.0f)
        , TacticalScore(0.0f)
        , Confidence(0.0f)
    {}
};

/**
 * AIÂ≠∏Á??∏Ê?
 */
USTRUCT(BlueprintType)
struct FAILearningData
{
    GENERATED_BODY()

    // Â≠∏Á?ID
    UPROPERTY(BlueprintReadOnly)
    FString LearningID;

    // Â≠∏Á?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EAILearningType LearningType;

    // Ëº∏ÂÖ•?∏Ê?
    UPROPERTY(BlueprintReadOnly)
    TArray<float> InputData;

    // Ëº∏Âá∫?∏Ê?
    UPROPERTY(BlueprintReadOnly)
    TArray<float> OutputData;

    // ?éÂãµ??    UPROPERTY(BlueprintReadOnly)
    float RewardValue;

    // ?≤ÁΩ∞??    UPROPERTY(BlueprintReadOnly)
    float PenaltyValue;

    // Â≠∏Á??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float LearningTime;

    // Â≠∏Á?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 LearningCount;

    // ?êÂ???    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // ?∂Ê???    UPROPERTY(BlueprintReadOnly)
    float ConvergenceValue;

    // Â≠∏Á??ÉÊï∏
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> LearningParameters;

    FAILearningData()
        : LearningID(TEXT(""))
        , LearningType(EAILearningType::Reinforcement)
        , RewardValue(0.0f)
        , PenaltyValue(0.0f)
        , LearningTime(0.0f)
        , LearningCount(0)
        , SuccessRate(0.0f)
        , ConvergenceValue(0.0f)
    {}
};

/**
 * AI?ßËÉΩ?áÊ?
 */
USTRUCT(BlueprintType)
struct FAIPerformanceMetrics
{
    GENERATED_BODY()

    // Ê±∫Á??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // ?çÊ??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float ReactionTime;

    // ?êÂ???    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // ?àÁ??áÊï∏
    UPROPERTY(BlueprintReadOnly)
    float EfficiencyIndex;

    // ?©Ê??ßÊ???    UPROPERTY(BlueprintReadOnly)
    float AdaptabilityIndex;

    // Â≠∏Á??áÊï∏
    UPROPERTY(BlueprintReadOnly)
    float LearningIndex;

    // ?∞Ë??áÊï∏
    UPROPERTY(BlueprintReadOnly)
    float TacticalIndex;

    // ?îË™ø?áÊï∏
    UPROPERTY(BlueprintReadOnly)
    float CoordinationIndex;

    // ?µÊñ∞?áÊï∏
    UPROPERTY(BlueprintReadOnly)
    float InnovationIndex;

    // Ë≥áÊ?‰ΩøÁî®??    UPROPERTY(BlueprintReadOnly)
    float ResourceUsage;

    // Ë®àÁ?Ë§áÈ?Â∫?    UPROPERTY(BlueprintReadOnly)
    float ComputationalComplexity;

    // Ë®òÊÜ∂‰ΩøÁî®??    UPROPERTY(BlueprintReadOnly)
    float MemoryUsage;

    FAIPerformanceMetrics()
        : DecisionTime(0.0f)
        , ReactionTime(0.0f)
        , SuccessRate(0.0f)
        , EfficiencyIndex(0.0f)
        , AdaptabilityIndex(0.0f)
        , LearningIndex(0.0f)
        , TacticalIndex(0.0f)
        , CoordinationIndex(0.0f)
        , InnovationIndex(0.0f)
        , ResourceUsage(0.0f)
        , ComputationalComplexity(0.0f)
        , MemoryUsage(0.0f)
    {}
};

/**
 * ?∞È¨•AIÁ≥ªÁµ±
 * ÁÆ°Á??∫ËÉΩ?∞È¨•Ê±∫Á??åË??? */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingCombatAI : public AActor
{
    GENERATED_BODY()

public:
    AMingCombatAI(};

    /**
     * ?ùÂ??ñÊà∞È¨•AIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void InitializeCombatAI(};

    /**
     * ?úÈ??∞È¨•AIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ShutdownCombatAI(};

    /**
     * Ë®≠ÁΩÆAI??∫¶Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel};

    /**
     * Ë®≠ÁΩÆAI?∞Ë?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAITacticalType(EAITacticalType TacticalType};

    /**
     * Ë®≠ÁΩÆAIÂ≠∏Á?È°ûÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAILearningType(EAILearningType LearningType};

    /**
     * Ê∑ªÂ??óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ÁßªÈô§?óÊéß?Æ‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit};

    /**
     * ?≤Â??Ä?âÂ??ßÂñÆ‰Ω?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * ?∑Ë?AIÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAIDecision(AMingTacticalUnit* Unit};

    /**
     * ?∑Ë??∞Ë??ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?∑Ë?AIÂ≠∏Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAILearning(const FAILearningData& LearningData};

    /**
     * ?µÂª∫Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    };

    /**
     * Ê∑ªÂ?Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddDecisionNode(const FAIDecisionNode& Node};

    /**
     * ÁßªÈô§Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveDecisionNode(const FString& NodeID};

    /**
     * ?≤Â?Ê±∫Á?ÁØÄÈª?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIDecisionNode GetDecisionNode(const FString& NodeID) const;

    /**
     * ?≤Â??Ä?âÊ±∫Á≠ñÁ?Èª?     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<FAIDecisionNode> GetAllDecisionNodes() const;

    /**
     * Ë©ï‰º∞?∞Ë?Â±Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * ?üÊ??∞Ë?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units};

    /**
     * ?êÊ∏¨?µÊñπË°åÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * Ë®àÁ?Â®ÅË?Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * Ë®àÁ?Ê©üÊ?Á≠âÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits};

    /**
     * ?∑Ë??ªÊ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ?∑Ë??≤Á¶¶Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition};

    /**
     * ?∑Ë?ÁßªÂ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition};

    /**
     * ?∑Ë??§ÈÄÄÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition};

    /**
     * ?∑Ë??¥ÁøºÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ?∑Ë?‰ºèÊ?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits};

    /**
     * ?∑Ë??ØÊè¥Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit};

    /**
     * ?∑Ë??îË™øÊ±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType};

    /**
     * ?∑Ë?Â≠∏Á?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData};

    /**
     * ?∑Ë??©Ê?Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData};

    /**
     * Ë™øÊï¥AI??∫¶
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void AdjustAIDifficulty(float PerformanceFactor};

    /**
     * ?≤Â?AI?ßËÉΩ?áÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIPerformanceMetrics GetAIPerformanceMetrics() const;

    /**
     * ?≤Â?AIÁµ±Ë??∏Ê?
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, int32> GetAIStatistics() const;

    /**
     * ?≤Â?AI?çÁΩÆ
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, FString> GetAIConfiguration() const;

    /**
     * Ë®≠ÁΩÆAI?çÁΩÆ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool SetAIConfiguration(const TMap<FString, FString>& Configuration};

    /**
     * ?çÁΩÆAIÁ≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ResetAISystem(};

    /**
     * ?ô‰ªΩAI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool BackupAIData(const FString& BackupPath};

    /**
     * ?¢Âæ©AI?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RestoreAIData(const FString& BackupPath};

protected:
    // ?óÊéß?Æ‰??óË°®
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // Ê±∫Á?ÁØÄÈªûÊ?Â∞?    UPROPERTY()
    TMap<FString, FAIDecisionNode> DecisionNodes;

    // ?∞Ë??ÜÊ?ÁµêÊ?
    UPROPERTY()
    TArray<FAITacticalAnalysis> TacticalAnalyses;

    // Â≠∏Á??∏Ê?
    UPROPERTY()
    TArray<FAILearningData> LearningData;

    // AIÁµ±Ë??∏Ê?
    UPROPERTY()
    TMap<FString, int32> AIStatistics;

    // ?ßËÉΩ?áÊ?
    UPROPERTY()
    FAIPerformanceMetrics PerformanceMetrics;

    // AI?çÁΩÆ
    UPROPERTY()
    TMap<FString, FString> AIConfiguration;

    // ?Æ‰??ßÂà∂?®Â???    UPROPERTY()
    AMingUnitController* UnitController;

    // Â§öÂñÆ‰ΩçÂ?Ë™øÂô®ÂºïÁî®
    UPROPERTY()
    AMingMultiUnitCoordinator* MultiUnitCoordinator;

    // AI??∫¶Á≠âÁ?
    UPROPERTY()
    EAIDifficultyLevel DifficultyLevel;

    // AI?∞Ë?È°ûÂ?
    UPROPERTY()
    EAITacticalType TacticalType;

    // AIÂ≠∏Á?È°ûÂ?
    UPROPERTY()
    EAILearningType LearningType;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // Tick?¥Êñ∞
    virtual void Tick(float DeltaTime) override;

    // ?∑Ë?Ê±∫Á?Ê®?    bool ExecuteDecisionTree(AMingTacticalUnit* Unit};

    // ?∑Ë?Ë°åÁÇ∫Ê®?    bool ExecuteBehaviorTree(AMingTacticalUnit* Unit};

    // Ë©ï‰º∞Ê±∫Á?Ê¢ù‰ª∂
    bool EvaluateDecisionCondition(const FString& Condition, AMingTacticalUnit* Unit};

    // Ë®àÁ?Ê±∫Á?Ê¨äÈ?
    float CalculateDecisionWeight(const FAIDecisionNode& Node, AMingTacticalUnit* Unit};

    // ?∏Ê??Ä‰Ω≥Ê±∫Á≠?    FAIDecisionNode SelectBestDecision(AMingTacticalUnit* Unit};

    // ?∑Ë?Ê±∫Á?ÁØÄÈª?    bool ExecuteDecisionNode(const FAIDecisionNode& Node, AMingTacticalUnit* Unit};

    // ?¥Êñ∞Ê±∫Á?ÁØÄÈªûÁµ±Ë®?    void UpdateDecisionNodeStatistics(const FString& NodeID, bool bSuccess};

    // Ë®àÁ??∞Ë??ÜÊï∏
    float CalculateTacticalScore(const TArray<AMingTacticalUnit*>& Units};

    // Ë®àÁ?Â®ÅË?Ë©ï‰º∞
    float CalculateThreatAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits};

    // Ë®àÁ?Ê©üÊ?Ë©ï‰º∞
    float CalculateOpportunityAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits};

    // Ë®àÁ??™Âã¢Ë©ï‰º∞
    float CalculateAdvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits};

    // Ë®àÁ???ã¢Ë©ï‰º∞
    float CalculateDisadvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits};

    // ?üÊ??∞Ë?Âª∫Ë≠∞
    TArray<FString> GenerateTacticalRecommendationsInternal(const TArray<AMingTacticalUnit*>& Units};

    // ?êÊ∏¨?µÊñπË°åÂ?
    TArray<FString> PredictEnemyActionsInternal(const TArray<AMingTacticalUnit*>& EnemyUnits};

    // ?∑Ë?Âº∑Â?Â≠∏Á?
    bool ExecuteReinforcementLearning(const FAILearningData& LearningData};

    // ?∑Ë???ù£Â≠∏Á?
    bool ExecuteSupervisedLearning(const FAILearningData& LearningData};

    // ?∑Ë??°Áõ£??≠∏Áø?    bool ExecuteUnsupervisedLearning(const FAILearningData& LearningData};

    // ?∑Ë??≤Â?Â≠∏Á?
    bool ExecuteEvolutionaryLearning(const FAILearningData& LearningData};

    // ?∑Ë?Á•ûÁ?Á∂≤Áµ°Â≠∏Á?
    bool ExecuteNeuralLearning(const FAILearningData& LearningData};

    // ?∑Ë?Ê±∫Á?Ê®πÂ≠∏Áø?    bool ExecuteDecisionTreeLearning(const FAILearningData& LearningData};

    // ?∑Ë?Ë°åÁÇ∫Ê®πÂ≠∏Áø?    bool ExecuteBehaviorTreeLearning(const FAILearningData& LearningData};

    // ?∑Ë??Ä?ãÊ?Â≠∏Á?
    bool ExecuteStateMachineLearning(const FAILearningData& LearningData};

    // ?∑Ë?Ê®°Á?Â≠∏Á?
    bool ExecuteFuzzyLearning(const FAILearningData& LearningData};

    // ?∑Ë??∫ÂÇ≥Â≠∏Á?
    bool ExecuteGeneticLearning(const FAILearningData& LearningData};

    // ?∑Ë?Áæ§È?Â≠∏Á?
    bool ExecuteSwarmLearning(const FAILearningData& LearningData};

    // ?∑Ë?Ê∑∑Â?Â≠∏Á?
    bool ExecuteHybridLearning(const FAILearningData& LearningData};

    // ?¥Êñ∞Â≠∏Á??∏Ê?
    void UpdateLearningData(const FAILearningData& LearningData};

    // Ë®àÁ?Â≠∏Á??∂Ê?
    float CalculateLearningConvergence(};

    // ?™Â?AI?ßËÉΩ
    void OptimizeAIPerformance(};

    // Ë™øÊï¥Ê±∫Á?Ê¨äÈ?
    void AdjustDecisionWeights(};

    // ?©Ê??∞Ë?ËÆäÂ?
    void AdaptToTacticalChanges(};

    // Â≠∏Á??©ÂÆ∂Ë°åÁÇ∫
    void LearnPlayerBehavior(};

    // ?êÊ∏¨?©ÂÆ∂?èÂ?
    TArray<FString> PredictPlayerIntent(};

    // ?üÊ??çÂà∂Á≠ñÁï•
    TArray<FString> GenerateCounterStrategies(};

    // Ë©ï‰º∞AIË°®Áèæ
    float EvaluateAIPerformance(};

    // ?¥Êñ∞?ßËÉΩ?áÊ?
    void UpdatePerformanceMetrics(};

    // Ë®òÈ?AI‰∫ã‰ª∂
    void LogAIEvent(const FString& EventType, const FString& Details};

    // ?üÊ?AI?±Â?
    FString GenerateAIReport() const;

    // È©óË?AI?çÁΩÆ
    bool ValidateAIConfiguration() const;

    // ?≤Â?Ê±∫Á??èËø∞
    FString GetDecisionDescription(EAIDecisionType DecisionType) const;

    // ?≤Â??∞Ë??èËø∞
    FString GetTacticalDescription(EAITacticalType TacticalType) const;

    // ?≤Â???∫¶?èËø∞
    FString GetDifficultyDescription(EAIDifficultyLevel DifficultyLevel) const;

    // ?≤Â?Â≠∏Á??èËø∞
    FString GetLearningDescription(EAILearningType LearningType) const;

    // Ë®àÁ?Ë§áÈ?Â∫?    float CalculateComplexity() const;

    // ÁÆ°Á?Ë®òÊÜ∂‰ΩøÁî®
    void ManageMemoryUsage(};

    // ?™Â?Ë®àÁ?Ë≥áÊ?
    void OptimizeComputationalResources(};

    // ?ïÁ?AI?ØË™§
    void HandleAIError(const FString& ErrorType, const FString& ErrorMessage};

    // ?¢Âæ©AI?Ä??    void RecoverAIState(};
};

