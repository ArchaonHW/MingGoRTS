#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingCombatAI.generated.h"

class AMingTacticalUnit;
class AMingUnitController;
class AMingMultiUnitCoordinator;

/**
 * AI�欰??X */
UENUM(BlueprintType)
enum class EAIState: uint8 {
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
 * AI�M??��??
 */
UENUM(BlueprintType)
enum class EAIDecisionType: uint8 {
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
 * AI摧毀��??
 */
UENUM(BlueprintType)
enum class EAITacticalType: uint8 {
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
 * AIX??��??
 */
UENUM(BlueprintType)
enum class EAIDifficultyLevel: uint8 {
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
 * AI��??��??
 */
UENUM(BlueprintType)
enum class EAILearningType: uint8 {
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
 * AI�M??�`?? */
USTRUCT(BlueprintType)
struct FAIDecisionNode
{
    GENERATED_BODY()

    // �`�IID
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;

    // �`�I摧毀
    UPROPERTY(BlueprintReadOnly)
    FString NodeName;

    // �`�I?X
    UPROPERTY(BlueprintReadOnly)
    EAIDecisionType DecisionType;

    // �����摧毀
    UPROPERTY(BlueprintReadOnly)
    FString ConditionExpression;

    // �v??
    UPROPERTY(BlueprintReadOnly)
    float Weight;

    // 摧毀??
    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    // �l摧毀
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ChildNodes;

    // 摧毀����
    UPROPERTY(BlueprintReadOnly)
    int32 ExecutionCount;

    // 摧毀����
    UPROPERTY(BlueprintReadOnly)
    int32 SuccessCount;

    // ��??����
    UPROPERTY(BlueprintReadOnly)
    int32 FailureCount;

    // ??�����?X
    UPROPERTY(BlueprintReadOnly)
    float LastExecutionTime;

    // �`�I?X
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> NodeParameters;

    // ??�_??��
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
 * AI目標數量��??
 */
USTRUCT(BlueprintType)
struct FAITacticalAnalysis
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(BlueprintReadOnly)
    FString AnalysisID;

    // 目標數量
    UPROPERTY(BlueprintReadOnly)
    float AnalysisTime;

    // ��??����
    UPROPERTY(BlueprintReadOnly)
    float ThreatAssessment;

    // ��??����
    UPROPERTY(BlueprintReadOnly)
    float OpportunityAssessment;

    // ??�յ���
    UPROPERTY(BlueprintReadOnly)
    float AdvantageAssessment;

    // X??����
    UPROPERTY(BlueprintReadOnly)
    float DisadvantageAssessment;

    // 摧毀��ĳ
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TacticalRecommendations;

    // ���I����
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskAssessments;

    // ��??�ѧO
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> OpportunityIdentifications;

    // ??��??��
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> EnemyPredictions;

    // ???X?��
    UPROPERTY(BlueprintReadOnly)
    float TacticalScore;

    // �m�H??
    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    // ???X?��
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
 * AI��?X???
 */
USTRUCT(BlueprintType)
struct FAILearningData
{
    GENERATED_BODY()

    // ��??ID
    UPROPERTY(BlueprintReadOnly)
    FString LearningID;

    // ��??��??
    UPROPERTY(BlueprintReadOnly)
    EAILearningType LearningType;

    // ��J摧毀
    UPROPERTY(BlueprintReadOnly)
    TArray<float> InputData;

    // ��X摧毀
    UPROPERTY(BlueprintReadOnly)
    TArray<float> OutputData;

    // ??�yX
    UPROPERTY(BlueprintReadOnly)
    float RewardValue;

    // ??�@X
    UPROPERTY(BlueprintReadOnly)
    float PenaltyValue;

    // ��?X???
    UPROPERTY(BlueprintReadOnly)
    float LearningTime;

    // ��??����
    UPROPERTY(BlueprintReadOnly)
    int32 LearningCount;

    // ???X
    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // ???X
    UPROPERTY(BlueprintReadOnly)
    float ConvergenceValue;

    // ��?X?��
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
 * AI??��摧毀
 */
USTRUCT(BlueprintType)
struct FAIPerformanceMetrics
{
    GENERATED_BODY()

    // �M?X???
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // 目標數量
    UPROPERTY(BlueprintReadOnly)
    float ReactionTime;

    // ???X
    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // ???X?��
    UPROPERTY(BlueprintReadOnly)
    float EfficiencyIndex;

    // 故事重要性
    UPROPERTY(BlueprintReadOnly)
    float AdaptabilityIndex;

    // ��?X?��
    UPROPERTY(BlueprintReadOnly)
    float LearningIndex;

    // ???X?��
    UPROPERTY(BlueprintReadOnly)
    float TacticalIndex;

    // ??��??��
    UPROPERTY(BlueprintReadOnly)
    float CoordinationIndex;

    // ??�s??��
    UPROPERTY(BlueprintReadOnly)
    float InnovationIndex;

    // ��??�ϥ�X
    UPROPERTY(BlueprintReadOnly)
    float ResourceUsage;

    // �p??��摧毀
    UPROPERTY(BlueprintReadOnly)
    float ComputationalComplexity;

    // �O�Шϥ�X
    UPROPERTY(BlueprintReadOnly)
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
 * ??��AI�t��
 * ��?X?��??���M?X??X */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingCombatAI : public AActor
{
    GENERATED_BODY()

public:
    AMingCombatAI();

    /**
     * ???X?�԰�AI�t��
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void InitializeCombatAI();

    /**
     * ???X?��AI�t��
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ShutdownCombatAI();

    /**
     * �]�mAIX??��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel);

    /**
     * �]�mAI摧毀��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAITacticalType(EAITacticalType TacticalType);

    /**
     * �]�mAI��??��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAILearningType(EAILearningType LearningType);

    /**
     * �K?X?��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * ����??��摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 目標數量?X?��??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * 摧毀AI�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAIDecision(AMingTacticalUnit* Unit);

    /**
     * 故事選項?
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 摧毀AI��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAILearning(const FAILearningData& LearningData);

    /**
     * ??�بM??�`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    );

    /**
     * �K??�M??�`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddDecisionNode(const FAIDecisionNode& Node);

    /**
     * �����M??�`??     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveDecisionNode(const FString& NodeID);

    /**
     * 摧毀�M??�`??     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIDecisionNode GetDecisionNode(const FString& NodeID) const;

    /**
     * 目標數量�M��摧毀     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<FAIDecisionNode> GetAllDecisionNodes() const;

    /**
     * ����摧毀��X     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 目標數量��ĳ
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * ??��??���??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * �p??��??��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * �p??��??��??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 目標數量�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ???X?�m�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition);

    /**
     * 摧毀��??�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition);

    /**
     * ???X?�h�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition);

    /**
     * ???X?�l�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 摧毀��??�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits);

    /**
     * ???X?���M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * ???X?�ըM??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType);

    /**
     * 摧毀��??�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData);

    /**
     * 目標數量�M??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData);

    /**
     * �վ�AIX??
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void AdjustAIDifficulty(float PerformanceFactor);

    /**
     * 摧毀AI??��摧毀
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIPerformanceMetrics GetAIPerformanceMetrics() const;

    /**
     * 摧毀AI��?X???
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, int32> GetAIStatistics() const;

    /**
     * 摧毀AI??�m
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, FString> GetAIConfiguration() const;

    /**
     * �]�mAI??�m
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool SetAIConfiguration(const TMap<FString, FString>& Configuration);

    /**
     * ??�mAI�t��
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ResetAISystem();

    /**
     * ??��AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool BackupAIData(const FString& BackupPath);

    /**
     * ??�_AI摧毀
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RestoreAIData(const FString& BackupPath);

protected:
    // ??��???X?��
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // �M??�`�I摧毀
    UPROPERTY()
    TMap<FString, FAIDecisionNode> DecisionNodes;

    // 目標數量��??
    UPROPERTY()
    TArray<FAITacticalAnalysis> TacticalAnalyses;

    // ��?X???
    UPROPERTY()
    TArray<FAILearningData> LearningData;

    // AI��?X???
    UPROPERTY()
    TMap<FString, int32> AIStatistics;

    // ??��摧毀
    UPROPERTY()
    FAIPerformanceMetrics PerformanceMetrics;

    // AI??�m
    UPROPERTY()
    TMap<FString, FString> AIConfiguration;

    // ???X?��???X
    UPROPERTY()
    AMingUnitController* UnitController;

    // �h���??�վ��ޥ�
    UPROPERTY()
    AMingMultiUnitCoordinator* MultiUnitCoordinator;

    // AIX??��??
    UPROPERTY()
    EAIDifficultyLevel DifficultyLevel;

    // AI摧毀��??
    UPROPERTY()
    EAITacticalType TacticalType;

    // AI��??��??
    UPROPERTY()
    EAILearningType LearningType;

    // ??�_�w??�l??
    bool bIsInitialized;

    // Tick??�s
    virtual void Tick(float DeltaTime) override;

    // 摧毀�M摧毀
    bool ExecuteDecisionTree(AMingTacticalUnit* Unit);

    // 摧毀�欰??
    bool ExecuteBehaviorTree(AMingTacticalUnit* Unit);

    // ����M??����
    bool EvaluateDecisionCondition(const FString& Condition, AMingTacticalUnit* Unit);

    // �p??�M??�v??
    float CalculateDecisionWeight(const FAIDecisionNode& Node, AMingTacticalUnit* Unit);

    // ???X?�ΨM??
    FAIDecisionNode SelectBestDecision(AMingTacticalUnit* Unit);

    // 摧毀�M??�`??
    bool ExecuteDecisionNode(const FAIDecisionNode& Node, AMingTacticalUnit* Unit);

    // ??�s�M??�`�I��??
    void UpdateDecisionNodeStatistics(const FString& NodeID, bool bSuccess);

    // �p?X??X?��
    float CalculateTacticalScore(const TArray<AMingTacticalUnit*>& Units);

    // �p??��??����
    float CalculateThreatAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // �p??��??����
    float CalculateOpportunityAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // �p?X?�յ���
    float CalculateAdvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    // �p?X??����
    float CalculateDisadvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 目標數量��ĳ
    TArray<FString> GenerateTacticalRecommendationsInternal(const TArray<AMingTacticalUnit*>& Units);

    // ??��??���??
    TArray<FString> PredictEnemyActionsInternal(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 摧毀�j??��??
    bool ExecuteReinforcementLearning(const FAILearningData& LearningData);

    // ???X??��??
    bool ExecuteSupervisedLearning(const FAILearningData& LearningData);

    // ???X?��X摧毀
    bool ExecuteUnsupervisedLearning(const FAILearningData& LearningData);

    // 目標數量��??
    bool ExecuteEvolutionaryLearning(const FAILearningData& LearningData);

    // 摧毀��??�����??
    bool ExecuteNeuralLearning(const FAILearningData& LearningData);

    // 摧毀�M??���??
    bool ExecuteDecisionTreeLearning(const FAILearningData& LearningData);

    // 摧毀�欰���??
    bool ExecuteBehaviorTreeLearning(const FAILearningData& LearningData);

    // 目標數量??��??
    bool ExecuteStateMachineLearning(const FAILearningData& LearningData);

    // 摧毀��??��??
    bool ExecuteFuzzyLearning(const FAILearningData& LearningData);

    // ???X?�Ǿ�??
    bool ExecuteGeneticLearning(const FAILearningData& LearningData);

    // 摧毀�s??��??
    bool ExecuteSwarmLearning(const FAILearningData& LearningData);

    // 摧毀�V??��??
    bool ExecuteHybridLearning(const FAILearningData& LearningData);

    // ??�s��?X???
    void UpdateLearningData(const FAILearningData& LearningData);

    // �p??��?X???
    float CalculateLearningConvergence();

    // 摧毀AI??��
    void OptimizeAIPerformance();

    // �վ�M??�v??
    void AdjustDecisionWeights();

    // 目標數量��??
    void AdaptToTacticalChanges();

    // ��?X?�a�欰
    void LearnPlayerBehavior();

    // ??��??�a摧毀
    TArray<FString> PredictPlayerIntent();

    // ???X?���
    TArray<FString> GenerateCounterStrategies();

    // ����AI��{
    float EvaluateAIPerformance();

    // ??�s??��摧毀
    void UpdatePerformanceMetrics();

    // �O??AI�ƥ�
    void LogAIEvent(const FString& EventType, const FString& Details);

    // 摧毀AI摧毀
    FString GenerateAIReport() const;

    // ��??AI??�m
    bool ValidateAIConfiguration() const;

    // 摧毀�M?X?�z
    FString GetDecisionDescription(EAIDecisionType DecisionType) const;

    // 故事重要性?�z
    FString GetTacticalDescription(EAITacticalType TacticalType) const;

    // 目標數量?�z
    FString GetDifficultyDescription(EAIDifficultyLevel DifficultyLevel) const;

    // 摧毀��?X?�z
    FString GetLearningDescription(EAILearningType LearningType) const;

    // �p??��摧毀
    float CalculateComplexity() const;

    // �޲z�O�Шϥ�
    void ManageMemoryUsage();

    // �u�ƭp��귽
    void OptimizeComputationalResources();

    // �B�zAI���~
    void HandleAIError(const FString& ErrorType, const FString& ErrorMessage);

    // ��_AI���A
    void RecoverAIState();
};

