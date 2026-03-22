#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "MingAICombatManager.generated.h"

// AI�欰��??
UENUM(BlueprintType)
enum class EMingAIBehavior: uint8 {
    Aggressive,         // ???X
    Defensive,          // ??�mX
    Balanced,           // ����X
    Cautious,           // ��?X
    Reckless,           // �|��X
    Tactical,           // ???X
    Adaptive            // ??�A摧毀
};

// AI�M??�v??
UENUM(BlueprintType)
enum class EMingAIDecisionWeight: uint8 {
    Critical,           // ??��M??
    High,               // ���u摧毀
    Medium,             // ��?X摧毀?
    Low,                // �C�u摧毀
    Minimal            // ??�C�u摧毀
};

// AI目標數量
UENUM(BlueprintType)
enum class EMingAITacticalObjective: uint8 {
    DestroyEnemy,        // ��?X?�H
    CaptureObjective,    // ��?X???
    DefendPosition,     // 摧毀��m
    FlankEnemy,         // ??�l摧毀
    SupportAllies,       // ??��摧毀
    Retreat,             // ??�h
    Regroup,            // 摧毀
    Ambush,             // ��??
    HoldGround           // ???X??
};

// AI��??����
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIThreatAssessment
{
    GENERATED_BODY()

    // ��?X???ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ThreatUnitID;

    // ��??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatLevel;

    // ��??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatType;

    // ��??�Z��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance;

    // ��?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ThreatDirection;

    // ��?X??X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatDuration;

    // ��?X摧毀?
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

// AI摧毀�M??
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalDecision
{
    GENERATED_BODY()

    // �M??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // �M??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAITacticalObjective Objective;

    // �M?X?�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // 目標數量ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // �M??�v??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIDecisionWeight DecisionWeight;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // �M?X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> DecisionParameters;

    // 摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExpectedOutcome;

    // �M??�H摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    // �M?X??X
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

// AI???X?XUSTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIUnitState
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // 摧毀�欰
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIBehavior CurrentBehavior;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentTarget;

    // 目標數量��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentTargetLocation;

    // ��??����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAIThreatAssessment> ThreatAssessments;

    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TacticalKnowledge;

    // �g?X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatExperience;

    // ��?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningFactor;

    // 故事重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdaptabilityScore;

    // �M?X?�o摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionCooldown;

    // ??��M��?X
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

// AI目標數量
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalAnalysis
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AnalysisID;

    // ??��??X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector BattlefieldCenter;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BattlefieldRadius;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // ??��摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // ��?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeutralUnits;

    // ??��??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TerrainAdvantages;

    // 目標數量��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForceRatio;

    // 摧毀��ĳ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAITacticalDecision> TacticalRecommendations;

    // 目標數量
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

// AI??���ƥ�e??






/**
 * AI??����?X * �t�d��?X?��AI??���M?X?�ԳN?X */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingAICombatManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAICombatManager();

    // ???X?AI??���t��
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool InitializeAICombatSystem();

    // ����??AI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RegisterAIUnit(int32 UnitID, EMingAIBehavior Behavior);

    // ����AI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RemoveAIUnit(int32 UnitID);

    // ??�sAI???X?X
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool UpdateAIUnitState(int32 UnitID, const FMingAIUnitState& State);

    // ???X?����X
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalAnalysis AnalyzeBattlefield(const FVector& Center, float Radius);

    // �����??
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    TArray<FMingAIThreatAssessment> AssessThreats(int32 UnitID);

    // �s?X???�M??
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalDecision MakeTacticalDecision(int32 UnitID);

    // 摧毀AI�M??
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool ExecuteAIDecision(const FMingAITacticalDecision& Decision);

    // �]�mAI�欰��??
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SetAIBehavior(int32 UnitID, EMingAIBehavior Behavior);

    // 摧毀AI???X?X
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    FMingAIUnitState GetAIUnitState(int32 UnitID) const;

    // 摧毀AI�M??���v
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TArray<FMingAITacticalDecision> GetAIDecisionHistory(int32 UnitID) const;

    // �p??AI??�൥??
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    float CalculateAIIntelligence(int32 UnitID) const;

    // ��?X?�a�欰
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LearnFromPlayerBehavior(int32 PlayerUnitID, const FString& Action);

    // 摧毀AI����
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool AdaptAIStrategy(int32 UnitID);

    // 摧毀AI��??
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TMap<EMingAIBehavior, int32> GetAIStatistics() const;

    // �O??AI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SaveAIData();

    // ���JAI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LoadAIData();

    // �M��摧毀AI摧毀
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    void ClearAllAIUnits();

    // ========== ��?XAI 摧毀 (Parallel Processing) ==========
    
    /**
     * ��?X??X?XAI 摧毀�M?? (ParallelFor)
     * ??��X100+ ???X?�j�W��??��
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void ProcessAllAIUnitsParallel(float DeltaTime);

    /**
     * 摧毀�����?? (��?X??X
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void AssessThreatsForAllUnitsParallel();

    /**
     * 故事選項?�M?? (��?X??X
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    TArray<FMingAITacticalDecision> GenerateDecisionsForAllUnitsParallel();

    /**
     * �]�m��?X??X???(�W??����故事重要性?��??�æ�??)
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void SetParallelThreshold(int32 Threshold) { ParallelThreshold = Threshold; }

    /**
     * 摧毀��?X???��??
     */
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager|Parallel")
    float GetLastParallelProcessingTimeMs() const { return LastParallelProcessingTimeMs; }

    // �ƥ�e??
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
    // AI目標數量��??�w
    UPROPERTY()
    TMap<int32, FMingAIUnitState> AIUnitStates;

    // AI�M??���v - �`??�GTMap<TArray> ����XUPROPERTY
    TMap<int32, TArray<FMingAITacticalDecision>> AIDecisionHistory;

    // ??�a�欰��?X???
    UPROPERTY()
    TMap<FString, float> PlayerBehaviorPatterns;

    // AI�����ҪO - �`??�GTMap<TArray> ����XUPROPERTY
    TMap<EMingAIBehavior, TArray<FMingAITacticalDecision>> AIStrategyTemplates;

    // ???X?��摧毀
    UPROPERTY()
    FMingAITacticalAnalysis CurrentBattlefieldAnalysis;

    // ??�_�w??�l??
    UPROPERTY()
    bool bInitialized;

    // ��?X?�]??
    int32 ParallelThreshold = 50;  // �W?? 50 ??���?X?�æ�??
    float LastParallelProcessingTimeMs = 0.0f;
    int32 LastParallelThreadCount = 0;

    // 摧毀��?X???�M??��??��?? (��?X??X?��X
    TArray<FMingAITacticalDecision> ParallelDecisionsCache;
    FCriticalSection ParallelCacheLock;

private:
    // ���J??�]AI����
    void LoadDefaultAIStrategies();

    // �p??��??��??
    float CalculateThreatLevel(int32 UnitID, int32 ThreatUnitID) const;

    // ����???X?��
    float EvaluateTacticalAdvantage(const TArray<int32>& FriendlyUnits, const TArray<int32>& EnemyUnits) const;

    // ???X?�ΨM??
    FMingAITacticalDecision SelectBestDecision(int32 UnitID, const TArray<FMingAITacticalDecision>& Decisions) const;

    // ??�sAI��??
    void UpdateAILearning(int32 UnitID, const FMingAITacticalDecision& Decision, bool bSuccess);

    // 目標數量��ĳ
    TArray<FMingAITacticalDecision> GenerateTacticalRecommendations(int32 UnitID, const FMingAITacticalAnalysis& Analysis) const;

    // ??��??�a�欰
    FString PredictPlayerBehavior(int32 PlayerUnitID) const;

    // �p??�M??�H摧毀
    float CalculateDecisionConfidence(const FMingAITacticalDecision& Decision) const;

    // ��??AI�M??
    bool ValidateAIDecision(const FMingAITacticalDecision& Decision) const;

    // ??�sAI???X
    void UpdateAIAdaptability(int32 UnitID, float AdaptationFactor);

    // 摧毀AI�M?X?�o
    void ProcessAIDecisionCooldown(int32 UnitID, float DeltaTime);

    // �p?X??X???��??
    TArray<int32> FindCoordinatedAttackOpportunities(int32 UnitID) const;

    // �p?X?�mX??
    FVector CalculateDefensivePosition(int32 UnitID) const;

    // ???X?��??��
    TMap<FString, float> AnalyzeTerrainAdvantages(const FVector& Location) const;

    // ??��??�H��??
    FVector PredictEnemyMovement(int32 EnemyUnitID) const;

    // �p?X???��??
    TArray<int32> CalculateEncirclementOpportunities(int32 UnitID) const;

    // ����M??��??
    float EvaluateDecisionOutcome(const FMingAITacticalDecision& Decision) const;
};

