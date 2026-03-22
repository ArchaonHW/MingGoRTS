#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSAdvancedAI.generated.h"

UENUM(BlueprintType)
enum class ERTSAIBehaviorPattern: uint8 {
    Rush        UMETA(DisplayName = "Rush"),
    Turtle      UMETA(DisplayName = "Turtle"),
    Boom        UMETA(DisplayName = "Boom"),
    Raid        UMETA(DisplayName = "Raid"),
    Scout       UMETA(DisplayName = "Scout"),
    Expand      UMETA(DisplayName = "Expand"),
    Tech        UMETA(DisplayName = "Tech"),
    Economic    UMETA(DisplayName = "Economic"),
    Defensive   UMETA(DisplayName = "Defensive"),
    Aggressive  UMETA(DisplayName = "Aggressive"),
    Balanced    UMETA(DisplayName = "Balanced"),
    Adaptive    UMETA(DisplayName = "Adaptive")
};

UENUM(BlueprintType)
enum class ERTSAITacticalRole: uint8 {
    Commander   UMETA(DisplayName = "Commander"),
    Assault     UMETA(DisplayName = "Assault"),
    Support     UMETA(DisplayName = "Support"),
    Scout       UMETA(DisplayName = "Scout"),
    Defender    UMETA(DisplayName = "Defender"),
    Harasser    UMETA(DisplayName = "Harasser"),
    Artillery   UMETA(DisplayName = "Artillery"),
    Medic       UMETA(DisplayName = "Medic"),
    Engineer    UMETA(DisplayName = "Engineer"),
    Specialist  UMETA(DisplayName = "Specialist")
};

UENUM(BlueprintType)
enum class ERTSAIMemoryType: uint8 {
    ShortTerm   UMETA(DisplayName = "Short Term"),
    LongTerm    UMETA(DisplayName = "Long Term"),
    Strategic   UMETA(DisplayName = "Strategic"),
    Tactical    UMETA(DisplayName = "Tactical"),
    Social      UMETA(DisplayName = "Social"),
    Emotional   UMETA(DisplayName = "Emotional")
};

UENUM(BlueprintType)
enum class ERTSAIEmotionalState: uint8 {
    Calm        UMETA(DisplayName = "Calm"),
    Confident   UMETA(DisplayName = "Confident"),
    Anxious     UMETA(DisplayName = "Anxious"),
    Aggressive  UMETA(DisplayName = "Aggressive"),
    Cautious    UMETA(DisplayName = "Cautious"),
    Desperate   UMETA(DisplayName = "Desperate"),
    Stressed    UMETA(DisplayName = "Stressed"),
    Focused     UMETA(DisplayName = "Focused"),
    Relaxed     UMETA(DisplayName = "Relaxed"),
    Pressured   UMETA(DisplayName = "Pressured")
};

USTRUCT(BlueprintType)
struct FRTSAIPersonality
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Aggression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Defensiveness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Expansionism;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float EconomicFocus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TechFocus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float RiskTolerance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Adaptability;

    FRTSAIPersonality()
        : Aggression(0.5f)
        , Defensiveness(0.5f)
        , Expansionism(0.5f)
        , EconomicFocus(0.5f)
        , TechFocus(0.5f)
        , RiskTolerance(0.5f)
        , Adaptability(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FRTSAIMemory
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString MemoryID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EventDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERTSAIMemoryType MemoryType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Importance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Location;

    FRTSAIMemory()
        : MemoryType(ERTSAIMemoryType::ShortTerm)
        , Importance(0.5f)
        , Timestamp(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FRTSTacticalDecision
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ActionType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Priority;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Confidence;

    FRTSTacticalDecision()
        : Priority(0.5f)
        , Confidence(0.5f)
    {}
};

USTRUCT(BlueprintType)
struct FRTSAIStrategyPlan
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString PlanID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ERTSAIBehaviorPattern StrategyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FRTSTacticalDecision> Decisions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float CurrentPhase;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TotalPhases;

    FRTSAIStrategyPlan()
        : StrategyType(ERTSAIBehaviorPattern::Balanced)
        , CurrentPhase(0.0f)
        , TotalPhases(1.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIBehaviorChanged, FString, AIControllerID, ERTSAIBehaviorPattern, NewBehavior);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAIEmotionalStateChanged, FString, AIControllerID, ERTSAIEmotionalState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAITacticalDecisionMade, FString, AIControllerID, FRTSTacticalDecision, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAIAdaptationOccurred, FString, AIControllerID);

UCLASS(ClassGroup=(RTS), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSAdvancedAI : public UObject
{
    GENERATED_BODY()

public:
    UMingRTSAdvancedAI();

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void InitializeAdvancedAI();

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void SetAIBehaviorPattern(const FString& AIControllerID, ERTSAIBehaviorPattern NewPattern);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    ERTSAIBehaviorPattern GetAIBehaviorPattern(const FString& AIControllerID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void SetAITacticalRole(const FString& AIControllerID, ERTSAITacticalRole NewRole);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void AssignAIPersonality(const FString& AIControllerID, const FRTSAIPersonality& Personality);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    FRTSAIPersonality GetAIPersonality(const FString& AIControllerID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void AddAIMemory(const FString& AIControllerID, const FRTSAIMemory& Memory);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    TArray<FRTSAIMemory> GetAIMemories(const FString& AIControllerID, ERTSAIMemoryType MemoryType) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void UpdateAIEmotionalState(const FString& AIControllerID, ERTSAIEmotionalState NewState);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    ERTSAIEmotionalState GetAIEmotionalState(const FString& AIControllerID) const;

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    FRTSTacticalDecision MakeTacticalDecision(const FString& AIControllerID);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void ExecuteTacticalDecision(const FString& AIControllerID, const FRTSTacticalDecision& Decision);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void CreateStrategyPlan(const FString& AIControllerID, ERTSAIBehaviorPattern StrategyType);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void ExecuteStrategyPlan(const FString& AIControllerID);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void AdaptToPlayerStrategy(const FString& AIControllerID, const FString& PlayerID);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void LearnFromMatch(const FString& AIControllerID, bool bWon, const FString& MatchData);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void SetTeamCooperation(const FString& AIControllerID, int32 TeamID, bool bEnableCooperation);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    void ProcessTeamCoordination(int32 TeamID);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    float EvaluateThreatLevel(const FString& AIControllerID, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "RTS Advanced AI")
    TArray<FRTSTacticalDecision> GenerateTacticalOptions(const FString& AIControllerID);

    UPROPERTY(BlueprintAssignable, Category = "RTS Advanced AI Events")
    FOnAIBehaviorChanged OnAIBehaviorChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Advanced AI Events")
    FOnAIEmotionalStateChanged OnAIEmotionalStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "RTS Advanced AI Events")
    FOnAITacticalDecisionMade OnAITacticalDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "RTS Advanced AI Events")
    FOnAIAdaptationOccurred OnAIAdaptationOccurred;

protected:
    UPROPERTY()
    TMap<FString, ERTSAIBehaviorPattern> AIBehaviorPatterns;

    UPROPERTY()
    TMap<FString, ERTSAITacticalRole> AITacticalRoles;

    UPROPERTY()
    TMap<FString, FRTSAIPersonality> AIPersonalities;

    UPROPERTY()
    TMap<FString, TArray<FRTSAIMemory>> AIMemories;

    UPROPERTY()
    TMap<FString, ERTSAIEmotionalState> AIEmotionalStates;

    UPROPERTY()
    TMap<FString, FRTSAIStrategyPlan> AIStrategyPlans;

    UPROPERTY()
    TMap<int32, TArray<FString>> TeamMembers;

    UFUNCTION()
    ERTSAIBehaviorPattern AnalyzePlayerPattern(const FString& PlayerID) const;

    UFUNCTION()
    float CalculateDecisionConfidence(const FString& AIControllerID, const FRTSTacticalDecision& Decision) const;

    UFUNCTION()
    void UpdateAIPersonalityFromExperience(const FString& AIControllerID, bool bSuccess);

    UFUNCTION()
    void CleanupOldMemories(const FString& AIControllerID);

    UFUNCTION()
    ERTSAIEmotionalState DetermineEmotionalResponse(const FString& AIControllerID, float ThreatLevel, float ResourceLevel);
};
