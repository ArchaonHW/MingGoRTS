#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecisionConsequenceCalculator.generated.h"

/**
 * 決�?後�?類�?
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
 * 後�?影響範�?
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
 * 後�X��?程度
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
 * 後�X��X��?
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
 * 後�?影響類�?
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
 * 後�X�數
 */
USTRUCT(BlueprintType)
struct FConsequenceParameter
{
    GENERATED_BODY()

    // ?�數?�稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterName;

    // ?�數X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterValue;

    // ?�數類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString ParameterType; // "int", "float", "bool", "string", "array"

    // ?�否必�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    bool bIsRequired;

    // 默�X    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Consequence Parameter")
    FString DefaultValue;

    // ?�述
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
 * 決�?後�?定義
 */
USTRUCT(BlueprintType)
struct FDecisionConsequence
{
    GENERATED_BODY()

    // 後�?ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // 後�X�稱
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceName;

    // 後�X�述
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceDescription;

    // 後�?類�?
    UPROPERTY(BlueprintReadOnly)
    EDecisionConsequenceType ConsequenceType;

    // 影響範�?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactScope ImpactScope;

    // ?��?程度
    UPROPERTY(BlueprintReadOnly)
    EConsequenceSeverity Severity;

    // ?��X��?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceDuration Duration;

    // 影響類�?
    UPROPERTY(BlueprintReadOnly)
    EConsequenceImpactType ImpactType;

    // 後�X�數
    UPROPERTY(BlueprintReadOnly)
    TArray<FConsequenceParameter> Parameters;

    // 觸發條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> TriggerConditions;

    // 後�X��?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> Effects;

    // 後�?標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConsequenceTags;

    // ?��?決�?ID
    UPROPERTY(BlueprintReadOnly)
    FString RelatedDecisionID;

    // ?��?人物ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedCharacterIDs;

    // ?��?事件ID
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> RelatedEventIDs;

    // ?��?概�? (0-1)
    UPROPERTY(BlueprintReadOnly)
    float Probability;

    // 延�X��? (�?
    UPROPERTY(BlueprintReadOnly)
    float DelayTime;

    // ?�卻?��? (�?
    UPROPERTY(BlueprintReadOnly)
    float CooldownTime;

    // ?�否已觸X    UPROPERTY(BlueprintReadOnly)
    bool bIsTriggered;

    // 觸發?��?
    UPROPERTY(BlueprintReadOnly)
    float TriggerTime;

    // ?�否?�逆�?
    UPROPERTY(BlueprintReadOnly)
    bool bIsReversible;

    // ?�逆�?條件
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
 * 後�?計�?結�?
 */
USTRUCT(BlueprintType)
struct FConsequenceCalculationResult
{
    GENERATED_BODY()

    // ?�否?��?計�?
    UPROPERTY(BlueprintReadOnly)
    bool bSuccess;

    // 後�?ID
    UPROPERTY(BlueprintReadOnly)
    FString ConsequenceID;

    // 計�X��?
    UPROPERTY(BlueprintReadOnly)
    float CalculationTime;

    // 計�X��?後�?
    UPROPERTY(BlueprintReadOnly)
    TArray<FDecisionConsequence> CalculatedConsequences;

    // 影響?��X��X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> AffectedGameState;

    // 影響?�人X    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedCharacters;

    // 影響?��?�?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedEvents;

    // 影響?��?�?    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> AffectedResources;

    // 影響?�建�?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedBuildings;

    // 影響?�單�?    UPROPERTY(BlueprintReadOnly)
    TArray<FString> AffectedUnits;

    // 總�?影響?�數
    UPROPERTY(BlueprintReadOnly)
    float OverallImpactScore;

    // �?��影響?�數
    UPROPERTY(BlueprintReadOnly)
    float PositiveImpactScore;

    // 負面影響?�數
    UPROPERTY(BlueprintReadOnly)
    float NegativeImpactScore;

    // 計�?詳�?
    UPROPERTY(BlueprintReadOnly)
    FString CalculationDetails;

    // ?�誤信息
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
 * 決�?上�X */
USTRUCT(BlueprintType)
struct FDecisionContext
{
    GENERATED_BODY()

    // 決�?ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionID;

    // 決�X��?
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;

    // 決�X��?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionLocation;

    // 決�X�ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionMakerID;

    // 決�?類�?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionType;

    // 決�X��?
    UPROPERTY(BlueprintReadOnly)
    FString DecisionOption;

    // ?��X�戲?�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CurrentGameState;

    // ?��?人物?�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> CharacterStates;

    // ?��?事件?�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> EventStates;

    // 資�X�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, int32> ResourceStates;

    // 建�X�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> BuildingStates;

    // ?��X�X    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> UnitStates;

    // 民�X��?
    UPROPERTY(BlueprintReadOnly)
    FString RepublicEra;

    // ?��?年份
    UPROPERTY(BlueprintReadOnly)
    int32 CurrentYear;

    // ?�家X
    UPROPERTY(BlueprintReadOnly)
    FString PlayerFaction;

    // X��等�?
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
 * 決�?後�?計�X * 計�?決�X��?種�X��?影響
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingDecisionConsequenceCalculator : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionConsequenceCalculator(};

    /**
     * ?��X��X��?算器
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Initialize(};

    /**
     * ?��?後�?計�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    void Shutdown(};

    /**
     * 註�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence};

    /**
     * ?��?註�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool UnregisterDecisionConsequence(const FString& ConsequenceID};

    /**
     * 計�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context};

    /**
     * ?��?決�?後�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID) const;

    /**
     * ?��X�?�決策�X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetAllDecisionConsequences() const;

    /**
     * ?��X��?類�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType) const;

    /**
     * ?��X��?影響範�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope) const;

    /**
     * ?��X��X��?程度?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity) const;

    /**
     * ?��X��?影響類�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType) const;

    /**
     * ?��X��?決�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID) const;

    /**
     * ?��X��?人物?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID) const;

    /**
     * ?��X��?事件?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID) const;

    /**
     * 觸發後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?�用後�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?��?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context};

    /**
     * ?��?後�?統�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    TMap<FString, int32> GetConsequenceStatistics() const;

    /**
     * ?��?後�?影響?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID) const;

    /**
     * ?�測決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount};

    /**
     * 模擬決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount};

protected:
    // 已註?��?決�?後�?
    UPROPERTY()
    TArray<FDecisionConsequence> RegisteredConsequences;

    // 後�?ID?��X��X��?
    UPROPERTY()
    TMap<FString, FDecisionConsequence> ConsequenceMap;

    // 後�?統�?
    UPROPERTY()
    TMap<FString, int32> ConsequenceStatistics;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��X��X�歷?�決策�X�庫
    void InitializeHistoricalDecisionConsequences(};

    // ?�建?�治決�?後�?
    void CreatePoliticalDecisionConsequences(};

    // ?�建軍�?決�?後�?
    void CreateMilitaryDecisionConsequences(};

    // ?�建經�?決�?後�?
    void CreateEconomicDecisionConsequences(};

    // ?�建社�?決�?後�?
    void CreateSocialDecisionConsequences(};

    // ?�建?��?決�?後�?
    void CreateCulturalDecisionConsequences(};

    // ?�建外交決�?後�?
    void CreateDiplomaticDecisionConsequences(};

    // 計�X��?後�?
    void CalculateImmediateConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�?延�?後�?
    void CalculateDelayedConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�?累�?後�?
    void CalculateCumulativeConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�?條件後�?
    void CalculateConditionalConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�X��?後�?
    void CalculateRandomConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�X��?後�?
    void CalculateHiddenConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�?永�?後�?
    void CalculatePermanentConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�X��?後�?
    void CalculateTemporaryConsequences(const FDecisionContext& Context, FConsequenceCalculationResult& Result};

    // 計�?後�?影響?�數
    float CalculateConsequenceImpact(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計�?總�?影響?�數
    void CalculateOverallImpact(FConsequenceCalculationResult& Result) const;

    // 檢查後�?條件
    bool CheckConsequenceConditions(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // ?�用後�X��X��X    void ApplyConsequenceToGameState(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, FString>& AffectedStates) const;

    // ?�用後�X�人X    void ApplyConsequenceToCharacters(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedCharacters) const;

    // ?�用後�X��?�?    void ApplyConsequenceToEvents(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedEvents) const;

    // ?�用後�X��?�?    void ApplyConsequenceToResources(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TMap<FString, int32>& AffectedResources) const;

    // ?�用後�X�建�?    void ApplyConsequenceToBuildings(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedBuildings) const;

    // ?�用後�X�單�?    void ApplyConsequenceToUnits(const FDecisionConsequence& Consequence, const FDecisionContext& Context, TArray<FString>& AffectedUnits) const;

    // ?�新後�?統�?
    void UpdateConsequenceStatistics(const FString& ConsequenceID};

    // ?��?後�X�述
    FString GetConsequenceDescription(const FString& ConsequenceID) const;

    // 驗�?後�X�置
    bool ValidateConsequenceConfig(const FDecisionConsequence& Consequence) const;

    // 記�?後�X��?
    void LogConsequenceEvent(const FString& ConsequenceID, const FString& Message};

    // ?��?後�X��?
    FString GenerateConsequenceReport(const FString& ConsequenceID, const FConsequenceCalculationResult& Result) const;

    // �X後�X�數
    TArray<FString> ParseConsequenceParameters(const FString& ParameterString) const;

    // 計�?後�?概�?
    float CalculateConsequenceProbability(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計�?後�?延�?
    float CalculateConsequenceDelay(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 計�?後�X��X��?
    float CalculateConsequenceDuration(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // 檢查後�X�否?�逆�?
    bool IsConsequenceReversible(const FDecisionConsequence& Consequence, const FDecisionContext& Context) const;

    // ?��?後�X��?�?    int32 GetConsequencePriority(const FDecisionConsequence& Consequence) const;

    // ?��?後�?
    void SortConsequencesByPriority(TArray<FDecisionConsequence>& Consequences) const;
};

