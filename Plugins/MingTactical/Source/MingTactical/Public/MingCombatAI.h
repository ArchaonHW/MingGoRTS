#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameFramework/Actor.h"
#include "MingCombatAI.generated.h"

class AMingTacticalUnit;
class AMingUnitController;
class AMingMultiUnitCoordinator;

/**
 * AI行為狀態
 */
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
 * AI決策類型
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
 * AI戰術類型
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
 * AI難度等級
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
 * AI學習類型
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
 * AI決策節點
 */
USTRUCT(BlueprintType)
struct FAIDecisionNode
{
    GENERATED_BODY()

    // 節點ID
    UPROPERTY(BlueprintReadOnly)
    FString NodeID;

    // 節點名稱
    UPROPERTY(BlueprintReadOnly)
    FString NodeName;

    // 節點類型
    UPROPERTY(BlueprintReadOnly)
    EAIDecisionType DecisionType;

    // 條件表達式
    UPROPERTY(BlueprintReadOnly)
    FString ConditionExpression;

    // 權重
    UPROPERTY(BlueprintReadOnly)
    float Weight;

    // 優先級
    UPROPERTY(BlueprintReadOnly)
    int32 Priority;

    // 子節點
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ChildNodes;

    // 執行次數
    UPROPERTY(BlueprintReadOnly)
    int32 ExecutionCount;

    // 成功次數
    UPROPERTY(BlueprintReadOnly)
    int32 SuccessCount;

    // 失敗次數
    UPROPERTY(BlueprintReadOnly)
    int32 FailureCount;

    // 最後執行時間
    UPROPERTY(BlueprintReadOnly)
    float LastExecutionTime;

    // 節點參數
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> NodeParameters;

    // 是否啟用
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
 * AI戰術分析結果
 */
USTRUCT(BlueprintType)
struct FAITacticalAnalysis
{
    GENERATED_BODY()

    // 分析ID
    UPROPERTY(BlueprintReadOnly)
    FString AnalysisID;

    // 分析時間
    UPROPERTY(BlueprintReadOnly)
    float AnalysisTime;

    // 威脅評估
    UPROPERTY(BlueprintReadOnly)
    float ThreatAssessment;

    // 機會評估
    UPROPERTY(BlueprintReadOnly)
    float OpportunityAssessment;

    // 優勢評估
    UPROPERTY(BlueprintReadOnly)
    float AdvantageAssessment;

    // 劣勢評估
    UPROPERTY(BlueprintReadOnly)
    float DisadvantageAssessment;

    // 戰術建議
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TacticalRecommendations;

    // 風險評估
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RiskAssessments;

    // 機會識別
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> OpportunityIdentifications;

    // 敵方預測
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> EnemyPredictions;

    // 戰術分數
    UPROPERTY(BlueprintReadOnly)
    float TacticalScore;

    // 置信度
    UPROPERTY(BlueprintReadOnly)
    float Confidence;

    // 分析參數
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
 * AI學習數據
 */
USTRUCT(BlueprintType)
struct FAILearningData
{
    GENERATED_BODY()

    // 學習ID
    UPROPERTY(BlueprintReadOnly)
    FString LearningID;

    // 學習類型
    UPROPERTY(BlueprintReadOnly)
    EAILearningType LearningType;

    // 輸入數據
    UPROPERTY(BlueprintReadOnly)
    TArray<float> InputData;

    // 輸出數據
    UPROPERTY(BlueprintReadOnly)
    TArray<float> OutputData;

    // 獎勵值
    UPROPERTY(BlueprintReadOnly)
    float RewardValue;

    // 懲罰值
    UPROPERTY(BlueprintReadOnly)
    float PenaltyValue;

    // 學習時間
    UPROPERTY(BlueprintReadOnly)
    float LearningTime;

    // 學習次數
    UPROPERTY(BlueprintReadOnly)
    int32 LearningCount;

    // 成功率
    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // 收斂值
    UPROPERTY(BlueprintReadOnly)
    float ConvergenceValue;

    // 學習參數
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
 * AI性能指標
 */
USTRUCT(BlueprintType)
struct FAIPerformanceMetrics
{
    GENERATED_BODY()

    // 決策時間
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // 反應時間
    UPROPERTY(BlueprintReadOnly)
    float ReactionTime;

    // 成功率
    UPROPERTY(BlueprintReadOnly)
    float SuccessRate;

    // 效率指數
    UPROPERTY(BlueprintReadOnly)
    float EfficiencyIndex;

    // 適應性指數
    UPROPERTY(BlueprintReadOnly)
    float AdaptabilityIndex;

    // 學習指數
    UPROPERTY(BlueprintReadOnly)
    float LearningIndex;

    // 戰術指數
    UPROPERTY(BlueprintReadOnly)
    float TacticalIndex;

    // 協調指數
    UPROPERTY(BlueprintReadOnly)
    float CoordinationIndex;

    // 創新指數
    UPROPERTY(BlueprintReadOnly)
    float InnovationIndex;

    // 資源使用率
    UPROPERTY(BlueprintReadOnly)
    float ResourceUsage;

    // 計算複雜度
    UPROPERTY(BlueprintReadOnly)
    float ComputationalComplexity;

    // 記憶使用量
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
 * 戰鬥AI系統
 * 管理智能戰鬥決策和行為
 */
UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingCombatAI : public AActor
{
    GENERATED_BODY()

public:
    AMingCombatAI();

    /**
     * 初始化戰鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void InitializeCombatAI();

    /**
     * 關閉戰鬥AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ShutdownCombatAI();

    /**
     * 設置AI難度等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAIDifficultyLevel(EAIDifficultyLevel DifficultyLevel);

    /**
     * 設置AI戰術類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAITacticalType(EAITacticalType TacticalType);

    /**
     * 設置AI學習類型
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void SetAILearningType(EAILearningType LearningType);

    /**
     * 添加受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 移除受控單位
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveControlledUnit(AMingTacticalUnit* Unit);

    /**
     * 獲取所有受控單位
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<AMingTacticalUnit*> GetControlledUnits() const;

    /**
     * 執行AI決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAIDecision(AMingTacticalUnit* Unit);

    /**
     * 執行戰術分析
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis ExecuteTacticalAnalysis(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 執行AI學習
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAILearning(const FAILearningData& LearningData);

    /**
     * 創建決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAIDecisionNode CreateDecisionNode(
        const FString& NodeName,
        EAIDecisionType DecisionType,
        const FString& ConditionExpression,
        float Weight,
        int32 Priority
    );

    /**
     * 添加決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool AddDecisionNode(const FAIDecisionNode& Node);

    /**
     * 移除決策節點
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RemoveDecisionNode(const FString& NodeID);

    /**
     * 獲取決策節點
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIDecisionNode GetDecisionNode(const FString& NodeID) const;

    /**
     * 獲取所有決策節點
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TArray<FAIDecisionNode> GetAllDecisionNodes() const;

    /**
     * 評估戰術局勢
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    FAITacticalAnalysis EvaluateTacticalSituation(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 生成戰術建議
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> GenerateTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units);

    /**
     * 預測敵方行動
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    TArray<FString> PredictEnemyActions(const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 計算威脅等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateThreatLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 計算機會等級
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    float CalculateOpportunityLevel(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& EnemyUnits);

    /**
     * 執行攻擊決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAttackDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行防禦決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteDefenseDecision(AMingTacticalUnit* Unit, const FVector& DefensePosition);

    /**
     * 執行移動決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteMovementDecision(AMingTacticalUnit* Unit, const FVector& TargetPosition);

    /**
     * 執行撤退決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteRetreatDecision(AMingTacticalUnit* Unit, const FVector& RetreatPosition);

    /**
     * 執行側翼決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteFlankDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行伏擊決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAmbushDecision(AMingTacticalUnit* Unit, const TArray<AMingTacticalUnit*>& TargetUnits);

    /**
     * 執行支援決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteSupportDecision(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit);

    /**
     * 執行協調決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteCoordinationDecision(const TArray<AMingTacticalUnit*>& Units, EAIDecisionType DecisionType);

    /**
     * 執行學習決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteLearningDecision(AMingTacticalUnit* Unit, const FAILearningData& LearningData);

    /**
     * 執行適應決策
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool ExecuteAdaptationDecision(AMingTacticalUnit* Unit, const TArray<float>& AdaptationData);

    /**
     * 調整AI難度
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void AdjustAIDifficulty(float PerformanceFactor);

    /**
     * 獲取AI性能指標
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    FAIPerformanceMetrics GetAIPerformanceMetrics() const;

    /**
     * 獲取AI統計數據
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, int32> GetAIStatistics() const;

    /**
     * 獲取AI配置
     */
    UFUNCTION(BlueprintPure, Category = "Combat AI")
    TMap<FString, FString> GetAIConfiguration() const;

    /**
     * 設置AI配置
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool SetAIConfiguration(const TMap<FString, FString>& Configuration);

    /**
     * 重置AI系統
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    void ResetAISystem();

    /**
     * 備份AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool BackupAIData(const FString& BackupPath);

    /**
     * 恢復AI數據
     */
    UFUNCTION(BlueprintCallable, Category = "Combat AI")
    bool RestoreAIData(const FString& BackupPath);

protected:
    // 受控單位列表
    UPROPERTY()
    TArray<AMingTacticalUnit*> ControlledUnits;

    // 決策節點映射
    UPROPERTY()
    TMap<FString, FAIDecisionNode> DecisionNodes;

    // 戰術分析結果
    UPROPERTY()
    TArray<FAITacticalAnalysis> TacticalAnalyses;

    // 學習數據
    UPROPERTY()
    TArray<FAILearningData> LearningData;

    // AI統計數據
    UPROPERTY()
    TMap<FString, int32> AIStatistics;

    // 性能指標
    UPROPERTY()
    FAIPerformanceMetrics PerformanceMetrics;

    // AI配置
    UPROPERTY()
    TMap<FString, FString> AIConfiguration;

    // 單位控制器引用
    UPROPERTY()
    AMingUnitController* UnitController;

    // 多單位協調器引用
    UPROPERTY()
    AMingMultiUnitCoordinator* MultiUnitCoordinator;

    // AI難度等級
    UPROPERTY()
    EAIDifficultyLevel DifficultyLevel;

    // AI戰術類型
    UPROPERTY()
    EAITacticalType TacticalType;

    // AI學習類型
    UPROPERTY()
    EAILearningType LearningType;

    // 是否已初始化
    bool bIsInitialized;

    // Tick更新
    virtual void Tick(float DeltaTime) override;

    // 執行決策樹
    bool ExecuteDecisionTree(AMingTacticalUnit* Unit);

    // 執行行為樹
    bool ExecuteBehaviorTree(AMingTacticalUnit* Unit);

    // 評估決策條件
    bool EvaluateDecisionCondition(const FString& Condition, AMingTacticalUnit* Unit);

    // 計算決策權重
    float CalculateDecisionWeight(const FAIDecisionNode& Node, AMingTacticalUnit* Unit);

    // 選擇最佳決策
    FAIDecisionNode SelectBestDecision(AMingTacticalUnit* Unit);

    // 執行決策節點
    bool ExecuteDecisionNode(const FAIDecisionNode& Node, AMingTacticalUnit* Unit);

    // 更新決策節點統計
    void UpdateDecisionNodeStatistics(const FString& NodeID, bool bSuccess);

    // 計算戰術分數
    float CalculateTacticalScore(const TArray<AMingTacticalUnit*>& Units);

    // 計算威脅評估
    float CalculateThreatAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 計算機會評估
    float CalculateOpportunityAssessment(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 計算優勢評估
    float CalculateAdvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 計算劣勢評估
    float CalculateDisadvantageAssessment(const TArray<AMingTacticalUnit*>& FriendlyUnits, const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 生成戰術建議
    TArray<FString> GenerateTacticalRecommendationsInternal(const TArray<AMingTacticalUnit*>& Units);

    // 預測敵方行動
    TArray<FString> PredictEnemyActionsInternal(const TArray<AMingTacticalUnit*>& EnemyUnits);

    // 執行強化學習
    bool ExecuteReinforcementLearning(const FAILearningData& LearningData);

    // 執行監督學習
    bool ExecuteSupervisedLearning(const FAILearningData& LearningData);

    // 執行無監督學習
    bool ExecuteUnsupervisedLearning(const FAILearningData& LearningData);

    // 執行進化學習
    bool ExecuteEvolutionaryLearning(const FAILearningData& LearningData);

    // 執行神經網絡學習
    bool ExecuteNeuralLearning(const FAILearningData& LearningData);

    // 執行決策樹學習
    bool ExecuteDecisionTreeLearning(const FAILearningData& LearningData);

    // 執行行為樹學習
    bool ExecuteBehaviorTreeLearning(const FAILearningData& LearningData);

    // 執行狀態機學習
    bool ExecuteStateMachineLearning(const FAILearningData& LearningData);

    // 執行模糊學習
    bool ExecuteFuzzyLearning(const FAILearningData& LearningData);

    // 執行遺傳學習
    bool ExecuteGeneticLearning(const FAILearningData& LearningData);

    // 執行群體學習
    bool ExecuteSwarmLearning(const FAILearningData& LearningData);

    // 執行混合學習
    bool ExecuteHybridLearning(const FAILearningData& LearningData);

    // 更新學習數據
    void UpdateLearningData(const FAILearningData& LearningData);

    // 計算學習收斂
    float CalculateLearningConvergence();

    // 優化AI性能
    void OptimizeAIPerformance();

    // 調整決策權重
    void AdjustDecisionWeights();

    // 適應戰術變化
    void AdaptToTacticalChanges();

    // 學習玩家行為
    void LearnPlayerBehavior();

    // 預測玩家意圖
    TArray<FString> PredictPlayerIntent();

    // 生成反制策略
    TArray<FString> GenerateCounterStrategies();

    // 評估AI表現
    float EvaluateAIPerformance();

    // 更新性能指標
    void UpdatePerformanceMetrics();

    // 記錄AI事件
    void LogAIEvent(const FString& EventType, const FString& Details);

    // 生成AI報告
    FString GenerateAIReport() const;

    // 驗證AI配置
    bool ValidateAIConfiguration() const;

    // 獲取決策描述
    FString GetDecisionDescription(EAIDecisionType DecisionType) const;

    // 獲取戰術描述
    FString GetTacticalDescription(EAITacticalType TacticalType) const;

    // 獲取難度描述
    FString GetDifficultyDescription(EAIDifficultyLevel DifficultyLevel) const;

    // 獲取學習描述
    FString GetLearningDescription(EAILearningType LearningType) const;

    // 計算複雜度
    float CalculateComplexity() const;

    // 管理記憶使用
    void ManageMemoryUsage();

    // 優化計算資源
    void OptimizeComputationalResources();

    // 處理AI錯誤
    void HandleAIError(const FString& ErrorType, const FString& ErrorMessage);

    // 恢復AI狀態
    void RecoverAIState();
};
