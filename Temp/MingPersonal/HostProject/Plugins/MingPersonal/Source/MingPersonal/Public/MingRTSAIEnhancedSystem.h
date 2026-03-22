#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/ActorComponent.h"
#include "MingRTSAIController.h"
#include "MingRTSCombatSystem.h"
#include "MingRTSCampaignSystem.h"
#include "MingRTSAIEnhancedSystem.generated.h"

UENUM(BlueprintType)
enum class EAIBehaviorPattern : uint8
{
    Rush          UMETA(DisplayName = "Rush"),
    Turtle        UMETA(DisplayName = "Turtle"),
    Boom          UMETA(DisplayName = "Boom"),
    Raid          UMETA(DisplayName = "Raid"),
    Scout         UMETA(DisplayName = "Scout"),
    Expand        UMETA(DisplayName = "Expand"),
    Tech          UMETA(DisplayName = "Tech"),
    Economic      UMETA(DisplayName = "Economic"),
    Defensive     UMETA(DisplayName = "Defensive"),
    Aggressive    UMETA(DisplayName = "Aggressive"),
    Balanced      UMETA(DisplayName = "Balanced"),
    Adaptive      UMETA(DisplayName = "Adaptive")
};

UENUM(BlueprintType)
enum class EAITacticalRole : uint8
{
    Commander      UMETA(DisplayName = "Commander"),
    Assault        UMETA(DisplayName = "Assault"),
    Support        UMETA(DisplayName = "Support"),
    Scout          UMETA(DisplayName = "Scout"),
    Defender       UMETA(DisplayName = "Defender"),
    Harasser      UMETA(DisplayName = "Harasser"),
    Artillery      UMETA(DisplayName = "Artillery"),
    Medic          UMETA(DisplayName = "Medic"),
    Engineer       UMETA(DisplayName = "Engineer"),
    Specialist     UMETA(DisplayName = "Specialist")
};

UENUM(BlueprintType)
enum class EAIMemoryType : uint8
{
    ShortTerm      UMETA(DisplayName = "Short Term"),
    LongTerm       UMETA(DisplayName = "Long Term"),
    Strategic      UMETA(DisplayName = "Strategic"),
    Tactical       UMETA(DisplayName = "Tactical"),
    Social         UMETA(DisplayName = "Social"),
    Emotional      UMETA(DisplayName = "Emotional")
};

UENUM(BlueprintType)
enum class EAIEmotionalState : uint8
{
    Calm           UMETA(DisplayName = "Calm"),
    Confident      UMETA(DisplayName = "Confident"),
    Anxious        UMETA(DisplayName = "Anxious"),
    Aggressive     UMETA(DisplayName = "Aggressive"),
    Cautious       UMETA(DisplayName = "Cautious"),
    Desperate      UMETA(DisplayName = "Desperate"),
    Overwhelmed    UMETA(DisplayName = "Overwhelmed"),
    Focused        UMETA(DisplayName = "Focused"),
    Relaxed        UMETA(DisplayName = "Relaxed"),
    Stressed       UMETA(DisplayName = "Stressed")
};

USTRUCT(BlueprintType)
struct FAIMemory
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    FString MemoryID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    EAIMemoryType MemoryType;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    float CreationTime;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    float LastAccessed;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    float Importance;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    float Confidence;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    TArray<FString> Tags;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    TMap<FString, FString> Data;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    int32 AccessCount;

    UPROPERTY(BlueprintReadOnly, Category = "AI Memory")
    bool bIsActive;

    FAIMemory()
    {
        MemoryID = TEXT("");
        MemoryType = EAIMemoryType::ShortTerm;
        Description = TEXT("");
        CreationTime = 0.0f;
        LastAccessed = 0.0f;
        Importance = 0.5f;
        Confidence = 0.5f;
        AccessCount = 0;
        bIsActive = true;
    }
};

USTRUCT(BlueprintType)
struct FAIPersonality
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    EAIType AIType;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    EAIBehaviorPattern PrimaryPattern;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    EAIBehaviorPattern SecondaryPattern;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    EAITacticalRole PreferredRole;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    EAIEmotionalState CurrentEmotionalState;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    TMap<FString, float> Traits;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Aggression;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Caution;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Creativity;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Discipline;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Adaptability;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float RiskTolerance;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Teamwork;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float Leadership;

    UPROPERTY(BlueprintReadOnly, Category = "AI Personality")
    float LearningRate;

    FAIPersonality()
    {
        AIType = EAIType::Balanced;
        PrimaryPattern = EAIBehaviorPattern::Balanced;
        SecondaryPattern = EAIBehaviorPattern::Adaptive;
        PreferredRole = EAITacticalRole::Commander;
        CurrentEmotionalState = EAIEmotionalState::Calm;
        Aggression = 0.5f;
        Caution = 0.5f;
        Creativity = 0.5f;
        Discipline = 0.5f;
        Adaptability = 0.5f;
        RiskTolerance = 0.5f;
        Teamwork = 0.5f;
        Leadership = 0.5f;
        LearningRate = 0.1f;
    }
};

USTRUCT(BlueprintType)
struct FAITacticalDecision
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    FString DecisionID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    EAIBehaviorPattern Pattern;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    float Priority;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    float SuccessProbability;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    float RiskLevel;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    TArray<FString> RequiredActions;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    TArray<FString> ExpectedOutcomes;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    float ExecutionTime;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    bool bIsExecuting;

    UPROPERTY(BlueprintReadOnly, Category = "AI Tactical Decision")
    float StartTime;

    FAITacticalDecision()
    {
        DecisionID = TEXT("");
        Description = TEXT("");
        Pattern = EAIBehaviorPattern::Balanced;
        Priority = 0.5f;
        SuccessProbability = 0.5f;
        RiskLevel = 0.5f;
        ExecutionTime = 0.0f;
        bIsExecuting = false;
        StartTime = 0.0f;
    }
};

USTRUCT(BlueprintType)
struct FAIStrategy
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    FString StrategyID;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    FString Name;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    FString Description;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    EAIBehaviorPattern Pattern;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    TArray<FAITacticalDecision> TacticalDecisions;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    TMap<FString, float> PhaseTimings;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    TArray<FString> RequiredConditions;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    TArray<FString> SuccessIndicators;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    float OverallPriority;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    bool bIsActive;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    float StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "AI Strategy")
    float CurrentPhase;

    FAIStrategy()
    {
        StrategyID = TEXT("");
        Name = TEXT("");
        Description = TEXT("");
        Pattern = EAIBehaviorPattern::Balanced;
        OverallPriority = 0.5f;
        bIsActive = false;
        StartTime = 0.0f;
        CurrentPhase = 0.0f;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIPersonalityChanged, const FAIPersonality&, Personality);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIMemoryCreated, const FAIMemory&, Memory);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITacticalDecisionMade, const FAITacticalDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIStrategyActivated, const FAIStrategy&, Strategy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIEmotionalStateChanged, EAIEmotionalState, NewState);

/**
 * MingGoRTS 增強AI系統
 * 處理AI個性、記憶、戰術決策、策略規劃等高級AI功能
 */
UCLASS(BlueprintType, Blueprintable, ClassGroup = "MingRTS")
class MINGPERSONAL_API UMingRTSAIEnhancedSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAIEnhancedSystem();

    // 初始化增強AI系統
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void InitializeAIEnhancedSystem(UWorld* World);

    // 更新增強AI系統（每幀調用）
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdateAIEnhancedSystem(float DeltaTime);

    // 創建AI個性
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FString CreateAIPersonality(const FString& PersonalityID, EAIType AIType, EAIBehaviorPattern PrimaryPattern);

    // 獲取AI個性
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FAIPersonality GetAIPersonality(const FString& PersonalityID) const;

    // 更新AI個性
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdateAIPersonality(const FString& PersonalityID, const FAIPersonality& NewPersonality);

    // 創建AI記憶
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FString CreateAIMemory(const FString& PersonalityID, EAIMemoryType MemoryType, const FString& Description);

    // 獲取AI記憶
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FAIMemory GetAIMemory(const FString& MemoryID) const;

    // 檢索AI記憶
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TArray<FAIMemory> RetrieveAIMemories(const FString& PersonalityID, EAIMemoryType MemoryType, const FString& Query) const;

    // 更新記憶重要性
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdateMemoryImportance(const FString& MemoryID, float NewImportance);

    // 衰減記憶
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void DecayMemories(float DeltaTime);

    // 制定戰術決策
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FString MakeTacticalDecision(const FString& PersonalityID, const FString& Context);

    // 獲取戰術決策
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FAITacticalDecision GetTacticalDecision(const FString& DecisionID) const;

    // 執行戰術決策
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ExecuteTacticalDecision(const FString& DecisionID);

    // 評估決策結果
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void EvaluateDecisionResult(const FString& DecisionID, bool bSuccess, const FString& Outcome);

    // 制定AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FString CreateAIStrategy(const FString& PersonalityID, EAIBehaviorPattern Pattern);

    // 獲取AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    FAIStrategy GetAIStrategy(const FString& StrategyID) const;

    // 激活AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ActivateAIStrategy(const FString& StrategyID);

    // 更新AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdateAIStrategy(const FString& StrategyID, float DeltaTime);

    // 學習和適應
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void LearnFromExperience(const FString& PersonalityID, const FString& Experience, float Outcome);

    // 適應玩家策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void AdaptToPlayerStrategy(const FString& PersonalityID, const FString& PlayerStrategy);

    // 更新情感狀態
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdateEmotionalState(const FString& PersonalityID, EAIEmotionalState NewState);

    // 獲取情感狀態
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    EAIEmotionalState GetEmotionalState(const FString& PersonalityID) const;

    // 分析戰場情況
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TMap<FString, float> AnalyzeBattlefield(const FString& PersonalityID);

    // 預測玩家行動
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TArray<FString> PredictPlayerActions(const FString& PersonalityID);

    // 計算威脅評估
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TMap<FString, float> CalculateThreatAssessment(const FString& PersonalityID);

    // 優化資源分配
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TMap<EResourceType, float> OptimizeResourceAllocation(const FString& PersonalityID);

    // 協調AI團隊
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void CoordinateAITeam(const TArray<FString>& TeamMembers);

    // 獲取AI統計
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    TMap<FString, float> GetAIStatistics(const FString& PersonalityID) const;

public:
    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "AI Enhanced System Events")
    FOnAIPersonalityChanged OnAIPersonalityChanged;

    UPROPERTY(BlueprintAssignable, Category = "AI Enhanced System Events")
    FOnAIMemoryCreated OnAIMemoryCreated;

    UPROPERTY(BlueprintAssignable, Category = "AI Enhanced System Events")
    FOnAITacticalDecisionMade OnAITacticalDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "AI Enhanced System Events")
    FOnAIStrategyActivated OnAIStrategyActivated;

    UPROPERTY(BlueprintAssignable, Category = "AI Enhanced System Events")
    FOnAIEmotionalStateChanged OnAIEmotionalStateChanged;

protected:
    // 更新AI個性
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessAIPersonalityUpdates(float DeltaTime);

    // 處理記憶系統
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessMemorySystem(float DeltaTime);

    // 處理戰術決策
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessTacticalDecisions(float DeltaTime);

    // 處理AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessAIStrategies(float DeltaTime);

    // 處理學習系統
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessLearningSystem(float DeltaTime);

    // 處理情感系統
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ProcessEmotionalSystem(float DeltaTime);

    // 計算決策權重
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    float CalculateDecisionWeight(const FAITacticalDecision& Decision, const FAIPersonality& Personality) const;

    // 評估策略效果
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    float EvaluateStrategyEffectiveness(const FAIStrategy& Strategy) const;

    // 更新個性特質
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void UpdatePersonalityTraits(const FString& PersonalityID, const TMap<FString, float>& TraitChanges);

    // 記憶整合
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void ConsolidateMemories(const FString& PersonalityID);

    // 策略切換
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void SwitchStrategy(const FString& PersonalityID, const FString& NewStrategyID);

    // 危機處理
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void HandleCrisisSituation(const FString& PersonalityID);

    // 機會識別
    UFUNCTION(BlueprintCallable, Category = "AI Enhanced System")
    void IdentifyOpportunities(const FString& PersonalityID);

protected:
    // 當前世界引用
    UPROPERTY()
    TObjectPtr<UWorld> CurrentWorld;

    // AI個性映射
    UPROPERTY()
    TMap<FString, FAIPersonality> AIPersonalities;

    // AI記憶映射
    UPROPERTY()
    TMap<FString, FAIMemory> AIMemories;

    // 戰術決策映射
    UPROPERTY()
    TMap<FString, FAITacticalDecision> TacticalDecisions;

    // AI策略映射
    UPROPERTY()
    TMap<FString, FAIStrategy> AIStrategies;

    // 活躍策略映射
    UPROPERTY()
    TMap<FString, FString> ActiveStrategies;

    // AI更新間隔
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    float AIUpdateInterval;

    // 上次AI更新時間
    UPROPERTY()
    float LastAIUpdateTime;

    // 記憶衰減率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    float MemoryDecayRate;

    // 學習率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    float LearningRate;

    // 適應率
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    float AdaptationRate;

    // 最大記憶數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    int32 MaxMemoryCount;

    // 最大決策數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    int32 MaxDecisionCount;

    // 最大策略數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI Enhanced System")
    int32 MaxStrategyCount;

private:
    // 獲取個性名稱
    FString GetPersonalityName(const FString& PersonalityID) const;

    // 獲取記憶摘要
    FString GetMemorySummary(const FAIMemory& Memory) const;

    // 計算記憶相似度
    float CalculateMemorySimilarity(const FAIMemory& Memory1, const FAIMemory& Memory2) const;

    // 生成決策選項
    TArray<FAITacticalDecision> GenerateDecisionOptions(const FString& PersonalityID, const FString& Context) const;

    // 評估決策風險
    float EvaluateDecisionRisk(const FAITacticalDecision& Decision) const;

    // 預測決策結果
    TArray<FString> PredictDecisionOutcome(const FAITacticalDecision& Decision) const;

    // 優化決策執行
    void OptimizeDecisionExecution(const FString& DecisionID);

    // 記憶檢索優化
    TArray<FAIMemory> OptimizeMemoryRetrieval(const FString& PersonalityID, const FString& Query) const;

    // 個性發展
    void DevelopPersonality(const FString& PersonalityID);

    // 情感調節
    void RegulateEmotions(const FString& PersonalityID);

    // 壓力管理
    void ManageStress(const FString& PersonalityID);

    // 動機分析
    TMap<FString, float> AnalyzeMotivations(const FString& PersonalityID) const;

    // 衝突解決
    void ResolveConflicts(const FString& PersonalityID);

    // 目標設定
    void SetAIGoals(const FString& PersonalityID);

    // 績效評估
    float EvaluateAIPerformance(const FString& PersonalityID) const;

    // 策略優化
    void OptimizeStrategies(const FString& PersonalityID);

    // 記憶清理
    void CleanupMemories(const FString& PersonalityID);

    // 決策清理
    void CleanupDecisions(const FString& PersonalityID);

    // 獲取AI模板
    FAIPersonality GetPersonalityTemplate(EAIType AIType) const;

    // 獲取策略模板
    FAIStrategy GetStrategyTemplate(EAIBehaviorPattern Pattern) const;

    // 獲取決策模板
    FAITacticalDecision GetDecisionTemplate(EAIBehaviorPattern Pattern) const;
};
