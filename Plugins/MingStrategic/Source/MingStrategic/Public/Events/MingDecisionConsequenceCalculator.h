#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceCalculator.generated.h"

/**
 * æ±ºç?å¾Œæ?é¡å?
 */
UENUM(BlueprintType)
enum class EDecisionConsequenceType : uint8
{
    Immediate           UMETA(DisplayName = "Immediate"),
    Delayed             UMETA(DisplayName = "Delayed"),
    Cumulative          UMETA(DisplayName = "Cumulative"),
    Conditional         UMETA(DisplayName = "Conditional"),
    Random              UMETA(DisplayName = "Random"),
    Hidden              UMETA(DisplayName = "Hidden"),
    Permanent           UMETA(DisplayName = "Permanent"),
    Temporary           UMETA(DisplayName = "Temporary")
};

/**
 * å¾Œæ?å½±éŸ¿ç¯„å?
 */
UENUM(BlueprintType)
enum class EConsequenceImpactScope : uint8
{
    Personal            UMETA(DisplayName = "Personal"),
    Local               UMETA(DisplayName = "Local"),
    Regional            UMETA(DisplayName = "Regional"),
    National            UMETA(DisplayName = "National"),
    International       UMETA(DisplayName = "International"),
    Global              UMETA(DisplayName = "Global")
};

/**
 * å¾Œæ??´é?ç¨‹åº¦
 */
UENUM(BlueprintType)
enum class EConsequenceSeverity : uint8
{
    Trivial             UMETA(DisplayName = "Trivial"),
    Minor               UMETA(DisplayName = "Minor"),
    Moderate            UMETA(DisplayName = "Moderate"),
    Major               UMETA(DisplayName = "Major"),
    Critical            UMETA(DisplayName = "Critical"),
    Catastrophic        UMETA(DisplayName = "Catastrophic")
};

/**
 * å¾Œæ??ç??‚é?
 */
UENUM(BlueprintType)
enum class EConsequenceDuration : uint8
{
    Instant             UMETA(DisplayName = "Instant"),
    Short               UMETA(DisplayName = "Short"),
    Medium              UMETA(DisplayName = "Medium"),
    Long                UMETA(DisplayName = "Long"),
    Permanent           UMETA(DisplayName = "Permanent"),
    Indefinite          UMETA(DisplayName = "Indefinite")
};

/**
 * å¾Œæ?å½±éŸ¿é¡å?
 */
UENUM(BlueprintType)
enum class EConsequenceImpactType : uint8
{
    Positive            UMETA(DisplayName = "Positive"),
    Negative            UMETA(DisplayName = "Negative"),
    Mixed               UMETA(DisplayName = "Mixed"),
    Neutral             UMETA(DisplayName = "Neutral"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * å¾Œæ??ƒæ•¸
 */
USTRUCT(BlueprintType)
struct FConsequenceParameter
{
    GENERATED_BODY()

    // ?ƒæ•¸?ç¨±
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterName;

    // ?ƒæ•¸??    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterValue;

    // ?ƒæ•¸é¡å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterType; // "int", "float", "bool", "string", "array"

    // ?¯å¦å¿…é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    bool bIsRequired;

    // é»˜è???    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString DefaultValue;

    // ?è¿°
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString Description;

    FConsequenceParameter()
        : ParameterName(TEXT(""))
        , ParameterValue(TEXT(""))
        , ParameterType(TEXT("string"))
        , bIsRequired(false)
        , DefaultValue(TEXT(""))
        , Description(TEXT(""))
    {}
};

/**
 * æ±ºç?å¾Œæ?å®šç¾©
 */
USTRUCT(BlueprintType)
struct FDecisionConsequence
{
    GENERATED_BODY()

    // å¾Œæ?ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // å¾Œæ??ç¨±
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceName;

    // å¾Œæ??è¿°
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // å¾Œæ?é¡å?
    UPROPERTY(BlueprintReadOnly)
    EDecisionConsequenceType ConsequenceType;

    // å½±éŸ¿ç¯„å?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactScope ImpactScope;

    // ?´é?ç¨‹åº¦
    UPROPERTY(BlueprintReadOnly)
    EConsequenceSeverity Severity;

    // ?ç??‚é?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceDuration Duration;

    // å½±éŸ¿é¡å?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactType ImpactType;

    // å¾Œæ??ƒæ•¸
    UPROPERTY(BlueprintReadOnly)
    TArray<FConsequenceParameter> Parameters;

    // è§¸ç™¼æ¢ä»¶
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggerConditions;

    // å¾Œæ??ˆæ?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Effects;

    // å¾Œæ?æ¨™ç±¤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConsequenceTags;

    // ?¸é?æ±ºç?ID
    UPROPERTY(BlueprintReadOnly)
    FString RelatedDecisionID;

    // ?¸é?äººç‰©ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedCharacterIDs;

    // ?¸é?äº‹ä»¶ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedEventIDs;

    // ?¼ç?æ¦‚ç? (0-1)
    UPROPERTY(BlueprintReadOnly)
    float Probability;

    // å»¶è??‚é? (ç§?
    UPROPERTY(BlueprintReadOnly)
    float DelayTime;

    // ?·å»?‚é? (ç§?
    UPROPERTY(BlueprintReadOnly)
    float CooldownTime;

    // ?¯å¦å·²è§¸??    UPROPERTY(BlueprintReadOnly)
    bool bIsTriggered;

    // è§¸ç™¼?‚é?
    UPROPERTY(BlueprintReadOnly)
    float TriggerTime;

    // ?¯å¦?¯é€†è?
    UPROPERTY(BlueprintReadOnly)
    bool bIsReversible;

    // ?¯é€†è?æ¢ä»¶
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ReversalConditions;

    FDecisionConsequence()
        : ConsequenceID(TEXT(""))
        , ConsequenceName(TEXT(""))
        , ConsequenceDescription(TEXT(""))
        , ConsequenceType(EDecisionConsequenceType::Immediate)
        , ImpactScope(EConsequenceImpactScope::Local)
        , Severity(EConsequenceSeverity::Moderate)
        , Duration(EConsequenceDuration::Medium)
        , ImpactType(EConsequenceImpactType::Neutral)
        , RelatedDecisionID(TEXT(""))
        , Probability(1.0f)
        , DelayTime(0.0f)
        , CooldownTime(0.0f)
        , bIsTriggered(false)
        , TriggerTime(0.0f)
        , bIsReversible(false)
    {}
};

/**
 * å¾Œæ?è¨ˆç?çµæ?
 */
USTRUCT(BlueprintType)
struct FConsequenceCalculationResult
{
    GENERATED_BODY()

    // ?¯å¦?å?è¨ˆç?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // å¾Œæ?ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // è¨ˆç??‚é?
    UPROPERTY(BlueprintReadOnly)
    float CalculationTime;

    // è¨ˆç??ºç?å¾Œæ?
    UPROPERTY(BlueprintReadOnly)
    TArray<FDecisionConsequence> CalculatedConsequences;

    // å½±éŸ¿?„é??²ç???    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> AffectedGameState;

    // å½±éŸ¿?„äºº??    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedCharacters;

    // å½±éŸ¿?„ä?ä»?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedEvents;

    // å½±éŸ¿?„è?æº?    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> AffectedResources;

    // å½±éŸ¿?„å»ºç¯?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedBuildings;

    // å½±éŸ¿?„å–®ä½?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedUnits;

    // ç¸½é?å½±éŸ¿?†æ•¸
    UPROPERTY(BlueprintReadOnly)
    float OverallImpactScore;

    // æ­?¢å½±éŸ¿?†æ•¸
    UPROPERTY(BlueprintReadOnly)
    float PositiveImpactScore;

    // è² é¢å½±éŸ¿?†æ•¸
    UPROPERTY(BlueprintReadOnly)
    float NegativeImpactScore;

    // è¨ˆç?è©³æ?
    UPROPERTY(BlueprintReadOnly)
    FString CalculationDetails;

    // ?¯èª¤ä¿¡æ¯
    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    FConsequenceCalculationResult()
        : bSuccess(false)
        , ConsequenceID(TEXT(""))
        , CalculationTime(0.0f)
        , OverallImpactScore(0.0f)
        , PositiveImpactScore(0.0f)
        , NegativeImpactScore(0.0f)
        , CalculationDetails(TEXT(""))
        , ErrorMessage(TEXT(""))
    {}
};

/**
 * æ±ºç?ä¸Šä??? */
USTRUCT(BlueprintType)
struct FDecisionContext
{
    GENERATED_BODY()

    // æ±ºç?ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    // æ±ºç??‚é?
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // æ±ºç??°é?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionLocation;

    // æ±ºç??…ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionMakerID;

    // æ±ºç?é¡å?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    // æ±ºç??¸é?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionOption;

    // ?¶å??Šæˆ²?€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CurrentGameState;

    // ?¸é?äººç‰©?€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CharacterStates;

    // ?¸é?äº‹ä»¶?€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventStates;

    // è³‡æ??€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceStates;

    // å»ºç??€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> BuildingStates;

    // ?®ä??€??    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> UnitStates;

    // æ°‘å??‚æ?
    UPROPERTY(BlueprintReadOnly)
    FString RepublicEra;

    // ?¶å?å¹´ä»½
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    // ?©å®¶???
    UPROPERTY(BlueprintReadOnly)
    FString PlayerFaction;

    // ??º¦ç­‰ç?
    UPROPERTY(BlueprintReadOnly)
    int32 DifficultyLevel;

    FDecisionContext()
        : DecisionID(TEXT(""))
        , DecisionTime(0.0f)
        , DecisionLocation(TEXT(""))
        , DecisionMakerID(TEXT(""))
        , DecisionType(TEXT(""))
        , DecisionOption(TEXT(""))
        , RepublicEra(TEXT(""))
        , CurrentYear(1912)
        , PlayerFaction(TEXT(""))
        , DifficultyLevel(1)
    {}
};

/**
 * æ±ºç?å¾Œæ?è¨ˆç??? * è¨ˆç?æ±ºç??„å?ç¨®å??œå?å½±éŸ¿
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingDecisionConsequenceCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionConsequenceCalculator(};

    /**
     * ?å??–å??œè?ç®—å™¨
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Initialize(};

    /**
     * ?œé?å¾Œæ?è¨ˆç???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Shutdown(};

    /**
     * è¨»å?æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence};

    /**
     * ?–æ?è¨»å?æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool UnregisterDecisionConsequence(const FString& ConsequenceID};

    /**
     * è¨ˆç?æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context};

    /**
     * ?²å?æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID) const;

    /**
     * ?²å??€?‰æ±ºç­–å???     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetAllDecisionConsequences() const;

    /**
     * ?²å??‡å?é¡å??„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType) const;

    /**
     * ?²å??‡å?å½±éŸ¿ç¯„å??„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope) const;

    /**
     * ?²å??‡å??´é?ç¨‹åº¦?„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity) const;

    /**
     * ?²å??‡å?å½±éŸ¿é¡å??„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType) const;

    /**
     * ?²å??¸é?æ±ºç??„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID) const;

    /**
     * ?²å??¸é?äººç‰©?„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID) const;

    /**
     * ?²å??¸é?äº‹ä»¶?„å???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID) const;

    /**
     * è§¸ç™¼å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?‰ç”¨å¾Œæ??ˆæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?†è?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?²å?å¾Œæ?çµ±è?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TMap<FString, int32> GetConsequenceStatistics() const;

    /**
     * ?²å?å¾Œæ?å½±éŸ¿?†æ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID) const;

    /**
     * ?æ¸¬æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount};

    /**
     * æ¨¡æ“¬æ±ºç?å¾Œæ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount};

protected:
    // å·²è¨»?Šç?æ±ºç?å¾Œæ?
    UPROPERTY()
    TArray<FDecisionConsequence> RegisteredConsequences;

    // å¾Œæ?ID?°å??œç?? å?
    UPROPERTY()
    TMap<FString, FDecisionConsequence> ConsequenceMap;

    // å¾Œæ?çµ±è?
    UPROPERTY()
    TMap<FString, int32> ConsequenceStatistics;

    // ?¯å¦å·²å?å§‹å?
    bool bIsInitialized;

    // ?å??–æ??‹æ­·?²æ±ºç­–å??œåº«
    void InitializeHistoricalDecisionConsequences(};

    // ?µå»º?¿æ²»æ±ºç?å¾Œæ?
    void CreatePoliticalDecisionConsequences(};

    // ?µå»ºè»ä?æ±ºç?å¾Œæ?
    void CreateMilitaryDecisionConsequences(};

    // ?µå»ºç¶“æ?æ±ºç?å¾Œæ?
    void CreateEconomicDecisionConsequences(};

    // ?µå»ºç¤¾æ?æ±ºç?å¾Œæ?
    void CreateSocialDecisionConsequences(};

    // ?µå»º?‡å?æ±ºç?å¾Œæ?
    void CreateCulturalDecisionConsequences(};

    // ?µå»ºå¤–äº¤æ±ºç?å¾Œæ?
    void CreateDiplomaticDecisionConsequences(};

    // è¨ˆç??³æ?å¾Œæ?
    void CalculateImmediateConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç?å»¶è?å¾Œæ?
    void CalculateDelayedConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç?ç´¯ç?å¾Œæ?
    void CalculateCumulativeConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç?æ¢ä»¶å¾Œæ?
    void CalculateConditionalConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç??¨æ?å¾Œæ?
    void CalculateRandomConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç??±è?å¾Œæ?
    void CalculateHiddenConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç?æ°¸ä?å¾Œæ?
    void CalculatePermanentConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç??¨æ?å¾Œæ?
    void CalculateTemporaryConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // è¨ˆç?å¾Œæ?å½±éŸ¿?†æ•¸
    float CalculateConsequenceImpact(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // è¨ˆç?ç¸½é?å½±éŸ¿?†æ•¸
    void CalculateOverallImpact(FConsequenceCalculationResult& Result) const;

    // æª¢æŸ¥å¾Œæ?æ¢ä»¶
    bool CheckConsequenceConditions(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // ?‰ç”¨å¾Œæ??°é??²ç???    void ApplyConsequenceToGameState(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, FString>& AffectedStates) const;

    // ?‰ç”¨å¾Œæ??°äºº??    void ApplyConsequenceToCharacters(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedCharacters) const;

    // ?‰ç”¨å¾Œæ??°ä?ä»?    void ApplyConsequenceToEvents(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedEvents) const;

    // ?‰ç”¨å¾Œæ??°è?æº?    void ApplyConsequenceToResources(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, int32>& AffectedResources) const;

    // ?‰ç”¨å¾Œæ??°å»ºç¯?    void ApplyConsequenceToBuildings(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedBuildings) const;

    // ?‰ç”¨å¾Œæ??°å–®ä½?    void ApplyConsequenceToUnits(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedUnits) const;

    // ?´æ–°å¾Œæ?çµ±è?
    void UpdateConsequenceStatistics(const FString& ConsequenceID};

    // ?²å?å¾Œæ??è¿°
    FString GetConsequenceDescription(const FString& ConsequenceID) const;

    // é©—è?å¾Œæ??ç½®
    bool ValidateConsequenceConfig(const FDecisionConsequence& Consequence) const;

    // è¨˜é?å¾Œæ??¥è?
    void LogConsequenceEvent(const FString& ConsequenceID, const FString& Message};

    // ?Ÿæ?å¾Œæ??±å?
    FString GenerateConsequenceReport(const FString& ConsequenceID, const FConsequenceCalculationResult& Result) const;

    // è§??å¾Œæ??ƒæ•¸
    TArray<FString> ParseConsequenceParameters(const FString& ParameterString) const;

    // è¨ˆç?å¾Œæ?æ¦‚ç?
    float CalculateConsequenceProbability(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // è¨ˆç?å¾Œæ?å»¶è?
    float CalculateConsequenceDelay(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // è¨ˆç?å¾Œæ??ç??‚é?
    float CalculateConsequenceDuration(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // æª¢æŸ¥å¾Œæ??¯å¦?¯é€†è?
    bool IsConsequenceReversible(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // ?²å?å¾Œæ??ªå?ç´?    int32 GetConsequencePriority(const FDecisionConsequence& Consequence) const;

    // ?’å?å¾Œæ?
    void SortConsequencesByPriority(TArray<FDecisionConsequence>& Consequences) const;
};

