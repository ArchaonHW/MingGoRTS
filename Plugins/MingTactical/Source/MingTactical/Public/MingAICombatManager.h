#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalCombatSystem.h"
#include "MingFormationManager.h"
#include "MingAICombatManager.generated.h"

// AIË°åÁÇ∫È°ûÂ?
UENUM(BlueprintType)
enum class EMingAIBehavior : uint8
{
    Aggressive,         // ?ªÊ???    Defensive,          // ?≤Á¶¶??    Balanced,           // Âπ≥Ë°°??    Cautious,           // Ë¨πÊ???    Reckless,           // È≠ØËéΩ??    Tactical,           // ?∞Ë???    Adaptive            // ?™ÈÅ©?âÂ?
};

// AIÊ±∫Á?Ê¨äÈ?
UENUM(BlueprintType)
enum class EMingAIDecisionWeight : uint8
{
    Critical,           // ?úÈçµÊ±∫Á?
    High,               // È´òÂÑ™?àÁ?
    Medium,             // ‰∏≠Á??™Â?Á¥?    Low,                // ‰ΩéÂÑ™?àÁ?
    Minimal            // ?Ä‰ΩéÂÑ™?àÁ?
};

// AI?∞Ë??ÆÊ?
UENUM(BlueprintType)
enum class EMingAITacticalObjective : uint8
{
    DestroyEnemy,        // Ê∂àÊ??µ‰∫∫
    CaptureObjective,    // ‰ΩîÈ??ÆÊ?
    DefendPosition,     // ?≤Â?‰ΩçÁΩÆ
    FlankEnemy,         // ?¥Áøº?ªÊ?
    SupportAllies,       // ?ØÊè¥?ãË?
    Retreat,             // ?§ÈÄÄ
    Regroup,            // ?çÁ?
    Ambush,             // ‰ºèÊ?
    HoldGround           // ?ÖÂ???ú∞
};

// AIÂ®ÅË?Ë©ï‰º∞
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIThreatAssessment
{
    GENERATED_BODY()

    // Â®ÅË??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ThreatUnitID;

    // Â®ÅË?Á≠âÁ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatLevel;

    // Â®ÅË?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ThreatType;

    // Â®ÅË?Ë∑ùÈõ¢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Distance;

    // Â®ÅË??πÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector ThreatDirection;

    // Â®ÅË??ÅÁ??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ThreatDuration;

    // Â®ÅË??™Â?Á¥?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// AI?∞Ë?Ê±∫Á?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalDecision
{
    GENERATED_BODY()

    // Ê±∫Á?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // Ê±∫Á?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAITacticalObjective Objective;

    // Ê±∫Á??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // ?ÆÊ??Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetUnitID;

    // ?ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    // Ê±∫Á?Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIDecisionWeight DecisionWeight;

    // ?∑Ë??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ExecutionTime;

    // Ê±∫Á??ÉÊï∏
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, FString> DecisionParameters;

    // ?êÊ?ÁµêÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ExpectedOutcome;

    // Ê±∫Á?‰ø°Â?Â∫?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    // Ê±∫Á??ÇÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// AI?Æ‰??Ä??USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAIUnitState
{
    GENERATED_BODY()

    // ?Æ‰?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 UnitID;

    // ?∂Â?Ë°åÁÇ∫
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingAIBehavior CurrentBehavior;

    // ?∂Â??ÆÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentTarget;

    // ?∂Â??ÆÊ?‰ΩçÁΩÆ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector CurrentTargetLocation;

    // Â®ÅË?Ë©ï‰º∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAIThreatAssessment> ThreatAssessments;

    // ?∞Ë??•Ë?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TacticalKnowledge;

    // Á∂ìÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CombatExperience;

    // Â≠∏Á??†Â?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float LearningFactor;

    // ?©Ê??ßÂ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AdaptabilityScore;

    // Ê±∫Á??∑Âçª?ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DecisionCooldown;

    // ?ÄÂæåÊ±∫Á≠ñÊ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// AI?∞Ë??ÜÊ?
USTRUCT(BlueprintType)
struct MINGTACTICAL_API FMingAITacticalAnalysis
{
    GENERATED_BODY()

    // ?ÜÊ?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 AnalysisID;

    // ?∞Â†¥?Ä??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector BattlefieldCenter;

    // ?∞Â†¥?äÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BattlefieldRadius;

    // ?µÊñπ?Æ‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> EnemyUnits;

    // ?ãÊñπ?Æ‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> FriendlyUnits;

    // ‰∏≠Á??Æ‰?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> NeutralUnits;

    // ?∞ÂΩ¢?™Âã¢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> TerrainAdvantages;

    // ?µÊ??õÈ?Â∞çÊ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ForceRatio;

    // ?∞Ë?Âª∫Ë≠∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingAITacticalDecision> TacticalRecommendations;

    // ?ÜÊ??ÇÈ?
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

// AI?∞È¨•‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIDecisionMade, const FMingAITacticalDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorChanged, int32, UnitID, EMingAIBehavior, NewBehavior);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIThreatAssessed, const FMingAIThreatAssessment&, Threat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAITacticalAnalysis, const FMingAITacticalAnalysis&, Analysis);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAIUnitStateChanged, int32, UnitID, EMingAIBehavior, NewBehavior, const FMingAIUnitState&, State);

/**
 * AI?∞È¨•ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??∫ËÉΩAI?∞È¨•Ê±∫Á??åÊà∞Ë°ìÂ??? */
UCLASS(ClassGroup = (Tactical), Blueprintable, BlueprintType)
class MINGTACTICAL_API UMingAICombatManager : public UObject
{
    GENERATED_BODY()

public:
    UMingAICombatManager();

    // ?ùÂ??ñAI?∞È¨•Á≥ªÁµ±
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool InitializeAICombatSystem();

    // Ë®ªË®ª?äAI?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RegisterAIUnit(int32 UnitID, EMingAIBehavior Behavior);

    // ÁßªÈô§AI?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool RemoveAIUnit(int32 UnitID);

    // ?¥Êñ∞AI?Æ‰??Ä??    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool UpdateAIUnitState(int32 UnitID, const FMingAIUnitState& State);

    // ?ÜÊ??∞Â†¥Â±Ä??    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalAnalysis AnalyzeBattlefield(const FVector& Center, float Radius);

    // Ë©ï‰º∞Â®ÅË?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    TArray<FMingAIThreatAssessment> AssessThreats(int32 UnitID);

    // Ë£Ω‰??∞Ë?Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    FMingAITacticalDecision MakeTacticalDecision(int32 UnitID);

    // ?∑Ë?AIÊ±∫Á?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool ExecuteAIDecision(const FMingAITacticalDecision& Decision);

    // Ë®≠ÁΩÆAIË°åÁÇ∫Ê®°Â?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SetAIBehavior(int32 UnitID, EMingAIBehavior Behavior);

    // ?≤Â?AI?Æ‰??Ä??    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    FMingAIUnitState GetAIUnitState(int32 UnitID) const;

    // ?≤Â?AIÊ±∫Á?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TArray<FMingAITacticalDecision> GetAIDecisionHistory(int32 UnitID) const;

    // Ë®àÁ?AI?∫ËÉΩÁ≠âÁ?
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    float CalculateAIIntelligence(int32 UnitID) const;

    // Â≠∏Á??©ÂÆ∂Ë°åÁÇ∫
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LearnFromPlayerBehavior(int32 PlayerUnitID, const FString& Action);

    // ?©Ê?AIÁ≠ñÁï•
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool AdaptAIStrategy(int32 UnitID);

    // ?≤Â?AIÁµ±Ë?
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager")
    TMap<EMingAIBehavior, int32> GetAIStatistics() const;

    // ‰øùÂ?AI?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool SaveAIData();

    // ËºâÂÖ•AI?∏Ê?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    bool LoadAIData();

    // Ê∏ÖÈô§?Ä?âAI?Æ‰?
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager")
    void ClearAllAIUnits();

    // ========== ‰∏¶Ë???AI ?ïÁ? (Parallel Processing) ==========
    
    /**
     * ‰∏¶Ë??ïÁ??Ä??AI ?Æ‰?Ê±∫Á? (ParallelFor)
     * ?©Áî®??100+ ?Æ‰??ÑÂ§ßË¶èÊ®°?¥ÊôØ
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void ProcessAllAIUnitsParallel(float DeltaTime);

    /**
     * ?πÈ?Ë©ï‰º∞Â®ÅË? (‰∏¶Ë??ñÁ???
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void AssessThreatsForAllUnitsParallel();

    /**
     * ?πÈ??üÊ??∞Ë?Ê±∫Á? (‰∏¶Ë??ñÁ???
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    TArray<FMingAITacticalDecision> GenerateDecisionsForAllUnitsParallel();

    /**
     * Ë®≠ÁΩÆ‰∏¶Ë??ïÁ??æÂÄ?(Ë∂ÖÈ?Ê≠§Êï∏?èÁ??Æ‰??ç‰Ωø?®‰∏¶Ë°åÂ?)
     */
    UFUNCTION(BlueprintCallable, Category = "AI Combat Manager|Parallel")
    void SetParallelThreshold(int32 Threshold) { ParallelThreshold = Threshold; }

    /**
     * ?≤Â?‰∏¶Ë??ïÁ?Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "AI Combat Manager|Parallel")
    float GetLastParallelProcessingTimeMs() const { return LastParallelProcessingTimeMs; }

    // ‰∫ã‰ª∂ÂßîË?
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
    // AI?Æ‰??Ä?ãÊï∏?öÂ∫´
    UPROPERTY()
    TMap<int32, FMingAIUnitState> AIUnitStates;

    // AIÊ±∫Á?Ê≠∑Âè≤ - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<int32, TArray<FMingAITacticalDecision>> AIDecisionHistory;

    // ?©ÂÆ∂Ë°åÁÇ∫Â≠∏Á??∏Ê?
    UPROPERTY()
    TMap<FString, float> PlayerBehaviorPatterns;

    // AIÁ≠ñÁï•Ê®°Êùø - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<EMingAIBehavior, TArray<FMingAITacticalDecision>> AIStrategyTemplates;

    // ?∂Â??∞Â†¥?ÜÊ?
    UPROPERTY()
    FMingAITacticalAnalysis CurrentBattlefieldAnalysis;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

    // ‰∏¶Ë??ñË®≠ÂÆ?    int32 ParallelThreshold = 50;  // Ë∂ÖÈ? 50 ?ãÂñÆ‰ΩçÂ??®‰∏¶Ë°åÂ?
    float LastParallelProcessingTimeMs = 0.0f;
    int32 LastParallelThreadCount = 0;

    // ?∑Ë?Á∑íÂ??®Á?Ê±∫Á?ÁµêÊ?Âø´Â? (‰∏¶Ë??ïÁ??Ç‰Ωø??
    TArray<FMingAITacticalDecision> ParallelDecisionsCache;
    FCriticalSection ParallelCacheLock;

private:
    // ËºâÂÖ•?êË®≠AIÁ≠ñÁï•
    void LoadDefaultAIStrategies();

    // Ë®àÁ?Â®ÅË?Á≠âÁ?
    float CalculateThreatLevel(int32 UnitID, int32 ThreatUnitID) const;

    // Ë©ï‰º∞?∞Ë??™Âã¢
    float EvaluateTacticalAdvantage(const TArray<int32>& FriendlyUnits, const TArray<int32>& EnemyUnits) const;

    // ?∏Ê??Ä‰Ω≥Ê±∫Á≠?    FMingAITacticalDecision SelectBestDecision(int32 UnitID, const TArray<FMingAITacticalDecision>& Decisions) const;

    // ?¥Êñ∞AIÂ≠∏Á?
    void UpdateAILearning(int32 UnitID, const FMingAITacticalDecision& Decision, bool bSuccess);

    // ?üÊ??∞Ë?Âª∫Ë≠∞
    TArray<FMingAITacticalDecision> GenerateTacticalRecommendations(int32 UnitID, const FMingAITacticalAnalysis& Analysis) const;

    // ?êÊ∏¨?©ÂÆ∂Ë°åÁÇ∫
    FString PredictPlayerBehavior(int32 PlayerUnitID) const;

    // Ë®àÁ?Ê±∫Á?‰ø°Â?Â∫?    float CalculateDecisionConfidence(const FMingAITacticalDecision& Decision) const;

    // È©óË?AIÊ±∫Á?
    bool ValidateAIDecision(const FMingAITacticalDecision& Decision) const;

    // ?¥Êñ∞AI?©Ê???    void UpdateAIAdaptability(int32 UnitID, float AdaptationFactor);

    // ?ïÁ?AIÊ±∫Á??∑Âçª
    void ProcessAIDecisionCooldown(int32 UnitID, float DeltaTime);

    // Ë®àÁ??îÂ??ªÊ?Ê©üÊ?
    TArray<int32> FindCoordinatedAttackOpportunities(int32 UnitID) const;

    // Ë®àÁ??≤Á¶¶??ú∞
    FVector CalculateDefensivePosition(int32 UnitID) const;

    // ?ÜÊ??∞ÂΩ¢?™Âã¢
    TMap<FString, float> AnalyzeTerrainAdvantages(const FVector& Location) const;

    // ?êÊ∏¨?µ‰∫∫ÁßªÂ?
    FVector PredictEnemyMovement(int32 EnemyUnitID) const;

    // Ë®àÁ??ÖÂ?Ê©üÊ?
    TArray<int32> CalculateEncirclementOpportunities(int32 UnitID) const;

    // Ë©ï‰º∞Ê±∫Á?ÁµêÊ?
    float EvaluateDecisionOutcome(const FMingAITacticalDecision& Decision) const;
};
