#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "MingAICombatManager.generated.h"

// AI行為類型
UENUM(BlueprintType)
enum class EMingAIBehavior : uint8
{
    Aggressive,         // 攻擊性
    Defensive,          // 防禦性
    Balanced,           // 平衡型
    Cautious,           // 謹慎型
    Reckless,           // 魯莽型
    Tactical,           // 戰術型
    Adaptive            // 自適應型
};

// AI決策權重
UENUM(BlueprintType)
enum class EMingAIDecisionWeight : uint8
{
    Critical,           // 關鍵決策
    High,               // 高優先級
    Medium,             // 中等優先級
    Low,                // 低優先級
    Minimal            // 最低優先級
};

// AI戰術目標
UENUM(BlueprintType)
enum class EMingAITacticalObjective : uint8
{
    DestroyEnemy,        // 消滅敵人
    CaptureObjective,    // 佔領目標
    DefendPosition,     // 防守位置
    FlankEnemy,         // 側翼攻擊
    SupportAllies,       // 支援友軍
    Retreat,             // 撤退
    Regroup,            // 重組
    Ambush,             // 伏擊
    HoldGround           // 堅守陣地
};

// AI威脅評估
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIThreatAssessment
{
    GENERATED_BODY()

    // 威脅單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ThreatUnitID;

    // 威脅等級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatLevel;

    // 威脅類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatType;

    // 威脅距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance;

    // 威脅方向
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ThreatDirection;

    // 威脅持續時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatDuration;

    // 威脅優先級
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIDecisionWeight ThreatPriority;

    FMingAIThreatAssessment()
    {
        ThreatUnitID = -1;
        ThreatLevel = 0.0f;
        ThreatType = TEXT("");
        Distance = 0.0f;
        ThreatDirection = FVector::ZeroVector;
        ThreatDuration = 0.0f;
        ThreatPriority = EMingAIDecisionWeight::Low;
    }
};

// AI戰術決策
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalDecision
{
    GENERATED_BODY()

    // 決策ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 決策類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAITacticalObjective Objective;

    // 決策描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // 目標單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // 決策權重
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIDecisionWeight DecisionWeight;

    // 執行時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // 決策參數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> DecisionParameters;

    // 預期結果
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExpectedOutcome;

    // 決策信心度
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    // 決策時間戳
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTimestamp;

    FMingAITacticalDecision()
    {
        DecisionID = -1;
        Objective = EMingAITacticalObjective::DestroyEnemy;
        DecisionDescription = TEXT("");
        TargetUnitID = -1;
        TargetLocation = FVector::ZeroVector;
        DecisionWeight = EMingAIDecisionWeight::Medium;
        ExecutionTime = 0.0f;
        DecisionParameters.Empty();
        ExpectedOutcome = TEXT("");
        Confidence = 0.5f;
        DecisionTimestamp = FDateTime::Now();
    }
};

// AI單位狀態
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIUnitState
{
    GENERATED_BODY()

    // 單位ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // 當前行為
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIBehavior CurrentBehavior;

    // 當前目標
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentTarget;

    // 當前目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentTargetLocation;

    // 威脅評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAIThreatAssessment> ThreatAssessments;

    // 戰術知識
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TacticalKnowledge;

    // 經驗值
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatExperience;

    // 學習因子
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningFactor;

    // 適應性分數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdaptabilityScore;

    // 決策冷卻時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionCooldown;

    // 最後決策時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastDecisionTime;

    FMingAIUnitState()
    {
        UnitID = -1;
        CurrentBehavior = EMingAIBehavior::Balanced;
        CurrentTarget = -1;
        CurrentTargetLocation = FVector::ZeroVector;
        ThreatAssessments.Empty();
        TacticalKnowledge.Empty();
        CombatExperience = 0.0f;
        LearningFactor = 1.0f;
        AdaptabilityScore = 0.5f;
        DecisionCooldown = 0.0f;
        LastDecisionTime = FDateTime::Now();
    }
};

// AI戰術分析
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalAnalysis
{
    GENERATED_BODY()

    // 分析ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AnalysisID;

    // 戰場區域
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector BattlefieldCenter;

    // 戰場半徑
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BattlefieldRadius;

    // 敵方單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // 友方單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // 中立單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeutralUnits;

    // 地形優勢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TerrainAdvantages;

    // 敵我力量對比
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForceRatio;

    // 戰術建議
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAITacticalDecision> TacticalRecommendations;

    // 分析時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AnalysisTime;

    FMingAITacticalAnalysis()
    {
        AnalysisID = -1;
        BattlefieldCenter = FVector::ZeroVector;
        BattlefieldRadius = 1000.0f;
        EnemyUnits.Empty();
        FriendlyUnits.Empty();
        NeutralUnits.Empty();
        TerrainAdvantages.Empty();
        ForceRatio = 1.0f;
        TacticalRecommendations.Empty();
        AnalysisTime = FDateTime::Now();
    }
};

// AI戰鬥事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDecisionMade, const FMingAITacticalDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorChanged, int32, UnitID, EMingAIBehavior, NewBehavior);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIThreatAssessed, const FMingAIThreatAssessment&, Threat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITacticalAnalysis, const FMingAITacticalAnalysis&, Analysis);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIUnitStateChanged, int32, UnitID, EMingAIBehavior, NewBehavior, const FMingAIUnitState&, State);

/**
 * AI戰鬥管理器
 * 負責管理智能AI戰鬥決策和戰術分析
 */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingAICombatManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAICombatManager();

    // 初始化AI戰鬥系統
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool InitializeAICombatSystem();

    // 註註冊AI單位
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RegisterAIUnit(int32 UnitID, EMingAIBehavior Behavior);

    // 移除AI單位
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RemoveAIUnit(int32 UnitID);

    // 更新AI單位狀態
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool UpdateAIUnitState(int32 UnitID, const FMingAIUnitState& State);

    // 分析戰場局勢
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalAnalysis AnalyzeBattlefield(const FVector& Center, float Radius);

    // 評估威脅
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    TArray<FMingAIThreatAssessment> AssessThreats(int32 UnitID);

    // 製作戰術決策
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalDecision MakeTacticalDecision(int32 UnitID);

    // 執行AI決策
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool ExecuteAIDecision(const FMingAITacticalDecision& Decision);

    // 設置AI行為模式
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SetAIBehavior(int32 UnitID, EMingAIBehavior Behavior);

    // 獲取AI單位狀態
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    FMingAIUnitState GetAIUnitState(int32 UnitID) const;

    // 獲取AI決策歷史
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TArray<FMingAITacticalDecision> GetAIDecisionHistory(int32 UnitID) const;

    // 計算AI智能等級
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    float CalculateAIIntelligence(int32 UnitID) const;

    // 學習玩家行為
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LearnFromPlayerBehavior(int32 PlayerUnitID, const FString& Action);

    // 適應AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool AdaptAIStrategy(int32 UnitID);

    // 獲取AI統計
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TMap<EMingAIBehavior, int32> GetAIStatistics() const;

    // 保存AI數據
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SaveAIData();

    // 載入AI數據
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LoadAIData();

    // 清除所有AI單位
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    void ClearAllAIUnits();

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnAIDecisionMade OnAIDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnAIBehaviorChanged OnAIBehaviorChanged;

    UPROPERTY(BlueprintAssignable)
    FOnAIThreatAssessed OnAIThreatAssessed;

    UPROPERTY(BlueprintAssignable)
    FOnAITacticalAnalysis OnAITacticalAnalysis;

    UPROPERTY(BlueprintAssignable)
    FOnAIUnitStateChanged OnAIUnitStateChanged;

protected:
    // AI單位狀態數據庫
    UPROPERTY()
    TMap<int32, FMingAIUnitState> AIUnitStates;

    // AI決策歷史
    UPROPERTY()
    TMap<int32, TArray<FMingAITacticalDecision>> AIDecisionHistory;

    // 玩家行為學習數據
    UPROPERTY()
    TMap<FString, float> PlayerBehaviorPatterns;

    // AI策略模板
    UPROPERTY()
    TMap<EMingAIBehavior, TArray<FMingAITacticalDecision>> AIStrategyTemplates;

    // 當前戰場分析
    UPROPERTY()
    FMingAITacticalAnalysis CurrentBattlefieldAnalysis;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設AI策略
    void LoadDefaultAIStrategies();

    // 計算威脅等級
    float CalculateThreatLevel(int32 UnitID, int32 ThreatUnitID) const;

    // 評估戰術優勢
    float EvaluateTacticalAdvantage(const TArray<int32>& FriendlyUnits, const TArray<int32>& EnemyUnits) const;

    // 選擇最佳決策
    FMingAITacticalDecision SelectBestDecision(int32 UnitID, const TArray<FMingAITacticalDecision>& Decisions) const;

    // 更新AI學習
    void UpdateAILearning(int32 UnitID, const FMingAITacticalDecision& Decision, bool bSuccess);

    // 生成戰術建議
    TArray<FMingAITacticalDecision> GenerateTacticalRecommendations(int32 UnitID, const FMingAITacticalAnalysis& Analysis) const;

    // 預測玩家行為
    FString PredictPlayerBehavior(int32 PlayerUnitID) const;

    // 計算決策信心度
    float CalculateDecisionConfidence(const FMingAITacticalDecision& Decision) const;

    // 驗證AI決策
    bool ValidateAIDecision(const FMingAITacticalDecision& Decision) const;

    // 更新AI適應性
    void UpdateAIAdaptability(int32 UnitID, float AdaptationFactor);

    // 處理AI決策冷卻
    void ProcessAIDecisionCooldown(int32 UnitID, float DeltaTime);

    // 計算協同攻擊機會
    TArray<int32> FindCoordinatedAttackOpportunities(int32 UnitID) const;

    // 計算防禦陣地
    FVector CalculateDefensivePosition(int32 UnitID) const;

    // 分析地形優勢
    TMap<FString, float> AnalyzeTerrainAdvantages(const FVector& Location) const;

    // 預測敵人移動
    FVector PredictEnemyMovement(int32 EnemyUnitID) const;

    // 計算包圍機會
    TArray<int32> CalculateEncirclementOpportunities(int32 UnitID) const;

    // 評估決策結果
    float EvaluateDecisionOutcome(const FMingAITacticalDecision& Decision) const;
};
