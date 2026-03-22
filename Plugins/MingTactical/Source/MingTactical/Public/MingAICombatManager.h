#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "MingAICombatManager.generated.h"

// AI行為類�?
UENUM(BlueprintType)
enum class EMingAIBehavior : uint8
{
    Aggressive,         // ?��X    Defensive,          // ?�禦X    Balanced,           // 平衡X    Cautious,           // 謹�X    Reckless,           // 魯莽X    Tactical,           // ?��X    Adaptive            // ?�適?��?
};

// AI決�?權�?
UENUM(BlueprintType)
enum class EMingAIDecisionWeight : uint8
{
    Critical,           // ?�鍵決�?
    High,               // 高優?��?
    Medium,             // 中�X��?�?    Low,                // 低優?��?
    Minimal            // ?�低優?��?
};

// AI?��X��?
UENUM(BlueprintType)
enum class EMingAITacticalObjective : uint8
{
    DestroyEnemy,        // 消�X�人
    CaptureObjective,    // 佔�X��?
    DefendPosition,     // ?��?位置
    FlankEnemy,         // ?�翼?��?
    SupportAllies,       // ?�援?��?
    Retreat,             // ?�退
    Regroup,            // ?��?
    Ambush,             // 伏�?
    HoldGround           // ?��X��
};

// AI威�?評估
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIThreatAssessment
{
    GENERATED_BODY()

    // 威�X��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ThreatUnitID;

    // 威�?等�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatLevel;

    // 威�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatType;

    // 威�?距離
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance;

    // 威�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ThreatDirection;

    // 威�X��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatDuration;

    // 威�X��?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// AI?��?決�?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalDecision
{
    GENERATED_BODY()

    // 決�?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 決�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAITacticalObjective Objective;

    // 決�X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // ?��X��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // 決�?權�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIDecisionWeight DecisionWeight;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // 決�X�數
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> DecisionParameters;

    // ?��?結�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExpectedOutcome;

    // 決�?信�?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    // 決�X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// AI?��X�XUSTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIUnitState
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // ?��?行為
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIBehavior CurrentBehavior;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentTarget;

    // ?��X��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentTargetLocation;

    // 威�?評估
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAIThreatAssessment> ThreatAssessments;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TacticalKnowledge;

    // 經�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatExperience;

    // 學�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningFactor;

    // ?��X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdaptabilityScore;

    // 決�X�卻?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionCooldown;

    // ?�後決策�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime LastDecisionTime;

    FMingAIUnitState()
    {
        UnitID = -1;
        CurrentBehavior = EMingAIBehavior::Balanced;
        CurrentTarget = -1;
        CurrentTargetLocation = FVector::ZeroVector;
        ThreatAssessments.Empty(};
        TacticalKnowledge.Empty(};
        CombatExperience = 0.0f;
        LearningFactor = 1.0f;
        AdaptabilityScore = 0.5f;
        DecisionCooldown = 0.0f;
        LastDecisionTime = FDateTime::Now(};
    }
};

// AI?��X��?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalAnalysis
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AnalysisID;

    // ?�場?�X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector BattlefieldCenter;

    // ?�場?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BattlefieldRadius;

    // ?�方?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // ?�方?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // 中�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeutralUnits;

    // ?�形?�勢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TerrainAdvantages;

    // ?��X��?對�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForceRatio;

    // ?��?建議
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAITacticalDecision> TacticalRecommendations;

    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime AnalysisTime;

    FMingAITacticalAnalysis()
    {
        AnalysisID = -1;
        BattlefieldCenter = FVector::ZeroVector;
        BattlefieldRadius = 1000.0f;
        EnemyUnits.Empty(};
        FriendlyUnits.Empty(};
        NeutralUnits.Empty(};
        TerrainAdvantages.Empty(};
        ForceRatio = 1.0f;
        TacticalRecommendations.Empty(};
        AnalysisTime = FDateTime::Now(};
    }
};

// AI?�鬥事件委�?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDecisionMade, const FMingAITacticalDecision&, Decision};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorChanged, int32, UnitID, EMingAIBehavior, NewBehavior};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIThreatAssessed, const FMingAIThreatAssessment&, Threat};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITacticalAnalysis, const FMingAITacticalAnalysis&, Analysis};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIUnitStateChanged, int32, UnitID, EMingAIBehavior, NewBehavior, const FMingAIUnitState&, State};

/**
 * AI?�鬥管�X * 負責管�X�能AI?�鬥決�X�戰術�X */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingAICombatManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAICombatManager(};

    // ?��X�AI?�鬥系統
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool InitializeAICombatSystem(};

    // 註註?�AI?��?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RegisterAIUnit(int32 UnitID, EMingAIBehavior Behavior};

    // 移除AI?��?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RemoveAIUnit(int32 UnitID};

    // ?�新AI?��X�X    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool UpdateAIUnitState(int32 UnitID, const FMingAIUnitState& State};

    // ?��X�場局X    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalAnalysis AnalyzeBattlefield(const FVector& Center, float Radius};

    // 評估威�?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    TArray<FMingAIThreatAssessment> AssessThreats(int32 UnitID};

    // 製�X��?決�?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalDecision MakeTacticalDecision(int32 UnitID};

    // ?��?AI決�?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool ExecuteAIDecision(const FMingAITacticalDecision& Decision};

    // 設置AI行為模�?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SetAIBehavior(int32 UnitID, EMingAIBehavior Behavior};

    // ?��?AI?��X�X    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    FMingAIUnitState GetAIUnitState(int32 UnitID) const;

    // ?��?AI決�?歷史
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TArray<FMingAITacticalDecision> GetAIDecisionHistory(int32 UnitID) const;

    // 計�?AI?�能等�?
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    float CalculateAIIntelligence(int32 UnitID) const;

    // 學�X�家行為
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LearnFromPlayerBehavior(int32 PlayerUnitID, const FString& Action};

    // ?��?AI策略
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool AdaptAIStrategy(int32 UnitID};

    // ?��?AI統�?
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TMap<EMingAIBehavior, int32> GetAIStatistics() const;

    // 保�?AI?��?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SaveAIData(};

    // 載入AI?��?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LoadAIData(};

    // 清除?�?�AI?��?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    void ClearAllAIUnits(};

    // ========== 並�XAI ?��? (Parallel Processing) ==========
    
    /**
     * 並�X��X�XAI ?��?決�? (ParallelFor)
     * ?�用X100+ ?��X�大規模?�景
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void ProcessAllAIUnitsParallel(float DeltaTime};

    /**
     * ?��?評估威�? (並�X��X
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void AssessThreatsForAllUnitsParallel(};

    /**
     * ?��X��X��?決�? (並�X��X
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    TArray<FMingAITacticalDecision> GenerateDecisionsForAllUnitsParallel(};

    /**
     * 設置並�X��X��?(超�?此數?��X��X�使?�並行�?)
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void SetParallelThreshold(int32 Threshold) { ParallelThreshold = Threshold; }

    /**
     * ?��?並�X��?統�?
     */
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager|Parallel")
    float GetLastParallelProcessingTimeMs() const { return LastParallelProcessingTimeMs; }

    // 事件委�?
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
    // AI?��X�?�數?�庫
    UPROPERTY()
    TMap<int32, FMingAIUnitState> AIUnitStates;

    // AI決�?歷史 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<int32, TArray<FMingAITacticalDecision>> AIDecisionHistory;

    // ?�家行為學�X��?
    UPROPERTY()
    TMap<FString, float> PlayerBehaviorPatterns;

    // AI策略模板 - 注�?：TMap<TArray> 不支XUPROPERTY
    TMap<EMingAIBehavior, TArray<FMingAITacticalDecision>> AIStrategyTemplates;

    // ?��X�場?��?
    UPROPERTY()
    FMingAITacticalAnalysis CurrentBattlefieldAnalysis;

    // ?�否已�?始�?
    UPROPERTY()
    bool bInitialized;

    // 並�X�設�?    int32 ParallelThreshold = 50;  // 超�? 50 ?�單位�X�並行�?
    float LastParallelProcessingTimeMs = 0.0f;
    int32 LastParallelThreadCount = 0;

    // ?��?緒�X��?決�?結�?快�? (並�X��X�使X
    TArray<FMingAITacticalDecision> ParallelDecisionsCache;
    FCriticalSection ParallelCacheLock;

private:
    // 載入?�設AI策略
    void LoadDefaultAIStrategies(};

    // 計�?威�?等�?
    float CalculateThreatLevel(int32 UnitID, int32 ThreatUnitID) const;

    // 評估?��X�勢
    float EvaluateTacticalAdvantage(const TArray<int32>& FriendlyUnits, const TArray<int32>& EnemyUnits) const;

    // ?��X�佳決�?    FMingAITacticalDecision SelectBestDecision(int32 UnitID, const TArray<FMingAITacticalDecision>& Decisions) const;

    // ?�新AI學�?
    void UpdateAILearning(int32 UnitID, const FMingAITacticalDecision& Decision, bool bSuccess};

    // ?��X��?建議
    TArray<FMingAITacticalDecision> GenerateTacticalRecommendations(int32 UnitID, const FMingAITacticalAnalysis& Analysis) const;

    // ?�測?�家行為
    FString PredictPlayerBehavior(int32 PlayerUnitID) const;

    // 計�?決�?信�?�?    float CalculateDecisionConfidence(const FMingAITacticalDecision& Decision) const;

    // 驗�?AI決�?
    bool ValidateAIDecision(const FMingAITacticalDecision& Decision) const;

    // ?�新AI?��X    void UpdateAIAdaptability(int32 UnitID, float AdaptationFactor};

    // ?��?AI決�X�卻
    void ProcessAIDecisionCooldown(int32 UnitID, float DeltaTime};

    // 計�X��X��?機�?
    TArray<int32> FindCoordinatedAttackOpportunities(int32 UnitID) const;

    // 計�X�禦X��
    FVector CalculateDefensivePosition(int32 UnitID) const;

    // ?��X�形?�勢
    TMap<FString, float> AnalyzeTerrainAdvantages(const FVector& Location) const;

    // ?�測?�人移�?
    FVector PredictEnemyMovement(int32 EnemyUnitID) const;

    // 計�X��?機�?
    TArray<int32> CalculateEncirclementOpportunities(int32 UnitID) const;

    // 評估決�?結�?
    float EvaluateDecisionOutcome(const FMingAITacticalDecision& Decision) const;
};

