#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingWinLossCondition.generated.h"

/**
 * ?ùË?ÁµêÊ?È°ûÂ?
 */
UENUM(BlueprintType)
enum class EWinLossResult : uint8
{
    Victory             UMETA(DisplayName = "Victory"),
    Defeat              UMETA(DisplayName = "Defeat"),
    Draw                UMETA(DisplayName = "Draw"),
    Stalemate           UMETA(DisplayName = "Stalemate"),
    InProgress          UMETA(DisplayName = "In Progress"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * ?ùË?Ê¢ù‰ª∂È°ûÂ?
 */
UENUM(BlueprintType)
enum class EWinLossConditionType : uint8
{
    Military             UMETA(DisplayName = "Military"),
    Political            UMETA(DisplayName = "Political"),
    Economic             UMETA(DisplayName = "Economic"),
    Territorial          UMETA(DisplayName = "Territorial"),
    TimeBased            UMETA(DisplayName = "Time Based"),
    ScoreBased           UMETA(DisplayName = "Score Based"),
    Custom               UMETA(DisplayName = "Custom")
};

/**
 * ?ùË??™Â?Á¥? */
UENUM(BlueprintType)
enum class EWinLossPriority : uint8
{
    Critical            UMETA(DisplayName = "Critical"),
    High                UMETA(DisplayName = "High"),
    Medium              UMETA(DisplayName = "Medium"),
    Low                 UMETA(DisplayName = "Low"),
    Background          UMETA(DisplayName = "Background")
};

/**
 * ?ùË?Ê¢ù‰ª∂?Ä?? */
UENUM(BlueprintType)
enum class EWinLossConditionState : uint8
{
    Inactive            UMETA(DisplayName = "Inactive"),
    Active              UMETA(DisplayName = "Active"),
    Satisfied           UMETA(DisplayName = "Satisfied"),
    Failed              UMETA(DisplayName = "Failed"),
    Disabled            UMETA(DisplayName = "Disabled")
};

/**
 * ?ùË?Ê¢ù‰ª∂?ÉÊï∏
 */
USTRUCT(BlueprintType)
struct FWinLossConditionParameter
{
    GENERATED_BODY()

    // ?ÉÊï∏?çÁ®±
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterName;

    // ?ÉÊï∏??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterValue;

    // ?ÉÊï∏È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterType; // "int", "float", "bool", "string"

    // ÊØîË??ç‰?Á¨?    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ComparisonOperator; // ">", "<", "==", ">=", "<=", "contains"

    // ?ØÂê¶ÂøÖÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    bool bIsRequired;

    // Ê¨äÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    float Weight;

    FWinLossConditionParameter()
        : ParameterName(TEXT(""))
        , ParameterValue(TEXT(""))
        , ParameterType(TEXT("float"))
        , ComparisonOperator(TEXT("=="))
        , bIsRequired(true)
        , Weight(1.0f)
    {}
};

/**
 * ?ùË?Ê¢ù‰ª∂ÂÆöÁæ©
 */
USTRUCT(BlueprintType)
struct FWinLossCondition
{
    GENERATED_BODY()

    // Ê¢ù‰ª∂ID
    UPROPERTY(BlueprintReadOnly)
    FString ConditionID;

    // Ê¢ù‰ª∂?çÁ®±
    UPROPERTY(BlueprintReadOnly)
    FString ConditionName;

    // Ê¢ù‰ª∂?èËø∞
    UPROPERTY(BlueprintReadOnly)
    FString ConditionDescription;

    // Ê¢ù‰ª∂È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionType ConditionType;

    // ?ùË?ÁµêÊ?È°ûÂ?
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult ResultType;

    // ?™Â?Á¥?    UPROPERTY(BlueprintReadOnly)
    EWinLossPriority Priority;

    // Ê¢ù‰ª∂?ÉÊï∏
    UPROPERTY(BlueprintReadOnly)
    TArray<FWinLossConditionParameter> Parameters;

    // Ê¢ù‰ª∂?Ä??    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionState State;

    // ?ØÂê¶?∫È??µÊ?‰ª?    UPROPERTY(BlueprintReadOnly)
    bool bIsCriticalCondition;

    // Ê™¢Êü•?ìÈ? (Áß?
    UPROPERTY(BlueprintReadOnly)
    float CheckInterval;

    // ‰∏äÊ¨°Ê™¢Êü•?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float LastCheckTime;

    // ÊªøË∂≥Â∫¶Â???(0-1)
    UPROPERTY(BlueprintReadOnly)
    float SatisfactionScore;

    // Ê¢ù‰ª∂Ê®ôÁ±§
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConditionTags;

    // ?∏È?‰∫ã‰ª∂ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedEvents;

    FWinLossCondition()
        : ConditionID(TEXT(""))
        , ConditionName(TEXT(""))
        , ConditionDescription(TEXT(""))
        , ConditionType(EWinLossConditionType::Military)
        , ResultType(EWinLossResult::Unknown)
        , Priority(EWinLossPriority::Medium)
        , State(EWinLossConditionState::Inactive)
        , bIsCriticalCondition(false)
        , CheckInterval(1.0f)
        , LastCheckTime(0.0f)
        , SatisfactionScore(0.0f)
    {}
};

/**
 * ?ùË?Ë©ï‰º∞ÁµêÊ?
 */
USTRUCT(BlueprintType)
struct FWinLossEvaluationResult
{
    GENERATED_BODY()

    // Ë©ï‰º∞ÁµêÊ?
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult Result;

    // Á∏ΩÂ?
    UPROPERTY(BlueprintReadOnly)
    float TotalScore;

    // ?ùÂà©?ÜÊï∏
    UPROPERTY(BlueprintReadOnly)
    float VictoryScore;

    // Â§±Ê??ÜÊï∏
    UPROPERTY(BlueprintReadOnly)
    float DefeatScore;

    // Âπ≥Â??ÜÊï∏
    UPROPERTY(BlueprintReadOnly)
    float DrawScore;

    // ÊªøË∂≥?ÑÊ?‰ª?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SatisfiedConditions;

    // ?™ÊªøË∂≥Á?Ê¢ù‰ª∂
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnsatisfiedConditions;

    // ?úÈçµÊ¢ù‰ª∂?Ä??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EWinLossConditionState> CriticalConditionStates;

    // Ë©ï‰º∞?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float EvaluationTime;

    // Ë©≥‰º∞Ë©≥Ê?
    UPROPERTY(BlueprintReadOnly)
    FString EvaluationDetails;

    FWinLossEvaluationResult()
        : Result(EWinLossResult::Unknown)
        , TotalScore(0.0f)
        , VictoryScore(0.0f)
        , DefeatScore(0.0f)
        , DrawScore(0.0f)
        , EvaluationTime(0.0f)
        , EvaluationDetails(TEXT(""))
    {}
};

/**
 * ?ùË?Áµ±Ë??∏Ê?
 */
USTRUCT(BlueprintType)
struct FWinLossStatistics
{
    GENERATED_BODY()

    // Á∏ΩË?‰º∞Ê¨°??    UPROPERTY(BlueprintReadOnly)
    int32 TotalEvaluations;

    // ?ùÂà©Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 VictoryCount;

    // Â§±Ê?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 DefeatCount;

    // Âπ≥Â?Ê¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 DrawCount;

    // ?ùÁ?
    UPROPERTY(BlueprintReadOnly)
    float WinRate;

    // Âπ≥Â?Ë©ï‰º∞?ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float AverageEvaluationTime;

    // Ê¢ù‰ª∂ÊªøË∂≥??    UPROPERTY(BlueprintReadOnly)
    float ConditionSatisfactionRate;

    // ?úÈçµÊ¢ù‰ª∂Ëß∏ÁôºÊ¨°Êï∏
    UPROPERTY(BlueprintReadOnly)
    int32 CriticalConditionTriggers;

    FWinLossStatistics()
        : TotalEvaluations(0)
        , VictoryCount(0)
        , DefeatCount(0)
        , DrawCount(0)
        , WinRate(0.0f)
        , AverageEvaluationTime(0.0f)
        , ConditionSatisfactionRate(0.0f)
        , CriticalConditionTriggers(0)
    {}
};

/**
 * Ê∞ëÂ??ÇÊ??ùË?Ê¢ù‰ª∂Á≥ªÁµ±
 * ÁÆ°Á??Ä?âÂ?Ë≤†Ê?‰ª∂Á?Ê™¢Ê∏¨?ÅË?‰º∞Â??öÁü•
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingWinLossCondition : public UObject
{
    GENERATED_BODY()

public:
    UMingWinLossCondition(};

    /**
     * ?ùÂ??ñÂ?Ë≤†Ê?‰ª∂Á≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Initialize(};

    /**
     * ?úÈ??ùË?Ê¢ù‰ª∂Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Shutdown(};

    /**
     * Ë®ªÂ??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool RegisterWinLossCondition(const FWinLossCondition& Condition};

    /**
     * ?ñÊ?Ë®ªÂ??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool UnregisterWinLossCondition(const FString& ConditionID};

    /**
     * Ë©ï‰º∞?ùË??ÄÊ≥?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult EvaluateWinLoss(};

    /**
     * Ê™¢Êü•?ÆÂÄãÊ?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool CheckCondition(const FString& ConditionID};

    /**
     * ?≤Â??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossCondition GetWinLossCondition(const FString& ConditionID) const;

    /**
     * ?≤Â??Ä?âÂ?Ë≤†Ê?‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetAllWinLossConditions() const;

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÊ?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType) const;

    /**
     * ?≤Â??úÈçµÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetCriticalConditions() const;

    /**
     * ?≤Â?Â∑≤ÊªøË∂≥Á?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetSatisfiedConditions() const;

    /**
     * ?≤Â??™ÊªøË∂≥Á?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetUnsatisfiedConditions() const;

    /**
     * Ë®≠ÁΩÆ?äÊà≤?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void SetGameState(const TMap<FString, FString>& GameState};

    /**
     * ?≤Â??äÊà≤?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TMap<FString, FString> GetGameState() const;

    /**
     * ?¥Êñ∞?äÊà≤?Ä?ãÂÄ?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void UpdateGameStateValue(const FString& Key, const FString& Value};

    /**
     * ?≤Â??ùË?Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossStatistics GetWinLossStatistics() const;

    /**
     * ?çÁΩÆÁµ±Ë??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void ResetStatistics(};

    /**
     * Âº∑Âà∂Ë©ï‰º∞ (?®ÊñºÊ∏¨Ë©¶)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult ForceEvaluation(};

    /**
     * ?≤Â??∂Â??ùË?ÁµêÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    EWinLossResult GetCurrentResult() const;

    /**
     * ?≤Â??ùË??ÜÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    float GetWinLossScore() const;

    /**
     * Ê™¢Êü•?ØÂê¶Â∑≤Á???     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    bool IsGameOver() const;

    /**
     * ?≤Â?ÁµêÊ??üÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FString GetGameOverReason() const;

protected:
    // Â∑≤Ë®ª?äÁ??ùË?Ê¢ù‰ª∂
    UPROPERTY()
    TArray<FWinLossCondition> RegisteredConditions;

    // Ê¢ù‰ª∂ID?∞Ê?‰ª∂Á??†Â?
    UPROPERTY()
    TMap<FString, FWinLossCondition> ConditionMap;

    // ?äÊà≤?Ä??    UPROPERTY()
    TMap<FString, FString> GameState;

    // ?ùË?Áµ±Ë?
    UPROPERTY()
    FWinLossStatistics Statistics;

    // ?∂Â?Ë©ï‰º∞ÁµêÊ?
    UPROPERTY()
    FWinLossEvaluationResult CurrentEvaluation;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ??ãÊ≠∑?≤Â?Ë≤†Ê?‰ª?    void InitializeHistoricalWinLossConditions(};

    // ?µÂª∫Ëªç‰??ùË?Ê¢ù‰ª∂
    void CreateMilitaryConditions(};

    // ?µÂª∫?øÊ≤ª?ùË?Ê¢ù‰ª∂
    void CreatePoliticalConditions(};

    // ?µÂª∫Á∂ìÊ??ùË?Ê¢ù‰ª∂
    void CreateEconomicConditions(};

    // ?µÂª∫?òÂ??ùË?Ê¢ù‰ª∂
    void CreateTerritorialConditions(};

    // ?µÂª∫?ÇÈ??ùË?Ê¢ù‰ª∂
    void CreateTimeBasedConditions(};

    // ?µÂª∫?ÜÊï∏?ùË?Ê¢ù‰ª∂
    void CreateScoreBasedConditions(};

    // Ê™¢Êü•Ëªç‰?Ê¢ù‰ª∂
    bool CheckMilitaryCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•?øÊ≤ªÊ¢ù‰ª∂
    bool CheckPoliticalCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•Á∂ìÊ?Ê¢ù‰ª∂
    bool CheckEconomicCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•?òÂ?Ê¢ù‰ª∂
    bool CheckTerritorialCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•?ÇÈ?Ê¢ù‰ª∂
    bool CheckTimeBasedCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•?ÜÊï∏Ê¢ù‰ª∂
    bool CheckScoreBasedCondition(const FWinLossCondition& Condition};

    // Ê™¢Êü•?™Â?Áæ©Ê?‰ª?    bool CheckCustomCondition(const FWinLossCondition& Condition};

    // Ë©ï‰º∞Ê¢ù‰ª∂ÊªøË∂≥Â∫?    float EvaluateConditionSatisfaction(const FWinLossCondition& Condition};

    // Ë®àÁ??ùË??ÜÊï∏
    void CalculateWinLossScores(FWinLossEvaluationResult& Result};

    // Á¢∫Â??ÄÁµÇÁ???    EWinLossResult DetermineFinalResult(const FWinLossEvaluationResult& Result};

    // ?¥Êñ∞Áµ±Ë??∏Ê?
    void UpdateStatistics(const FWinLossEvaluationResult& Result};

    // ?öÁü•?ùË?ËÆäÂ?
    void NotifyWinLossChange(EWinLossResult OldResult, EWinLossResult NewResult};

    // ?≤Â??äÊà≤?Ä?ãÂÄ?    FString GetGameStateValue(const FString& Key) const;

    // ÊØîË??∏ÂÄ?    bool CompareValues(const FString& Value1, const FString& Operator, const FString& Value2) const;

    // È©óË?Ê¢ù‰ª∂?çÁΩÆ
    bool ValidateConditionConfig(const FWinLossCondition& Condition) const;

    // Ë®òÈ??ùË??•Ë?
    void LogWinLossEvent(const FString& Message};

    // ?üÊ??ùË??±Â?
    FString GenerateWinLossReport(const FWinLossEvaluationResult& Result) const;

    // Ëß??Ê¢ù‰ª∂?ÉÊï∏
    TArray<FString> ParseConditionParameters(const FString& ParameterString) const;

    // Ë®≠ÁΩÆÊ¢ù‰ª∂?Ä??    void SetConditionState(const FString& ConditionID, EWinLossConditionState NewState};

    // ?≤Â?Ê¢ù‰ª∂ÊªøË∂≥Â∫?    float GetConditionSatisfaction(const FString& ConditionID) const;

    // Ê™¢Êü•?úÈçµÊ¢ù‰ª∂
    bool CheckCriticalConditions(};

    // ?ïÁ??ùË?ÁµêÊ?
    void HandleGameOver(EWinLossResult Result, const FString& Reason};
};

