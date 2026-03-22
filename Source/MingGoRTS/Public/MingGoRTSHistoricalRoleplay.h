#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalRoleplay.generated.h"

UENUM(BlueprintType)
enum class ERoleplayType : uint8
{
    PoliticalDecision   UMETA(DisplayName = "?øÊ≤ªÊ±∫Á?"),
    MilitaryStrategy    UMETA(DisplayName = "Ëªç‰?Á≠ñÁï•"),
    DiplomaticNegotiation UMETA(DisplayName = "Â§ñ‰∫§Ë´áÂà§"),
    EconomicPolicy      UMETA(DisplayName = "Á∂ìÊ??øÁ?"),
    SocialReform        UMETA(DisplayName = "Á§æÊ??πÈù©"),
    PersonalChoice      UMETA(DisplayName = "?ã‰∫∫?∏Ê?")
};

UENUM(BlueprintType)
enum class EDecisionAlignment : uint8
{
    Historical          UMETA(DisplayName = "Ê≠∑Âè≤‰∏Ä??),
    Alternative         UMETA(DisplayName = "?ø‰ª£?∏Ê?"),
    CounterFactual      UMETA(DisplayName = "?ç‰?ÂØ?),
    PlayerChoice        UMETA(DisplayName = "?©ÂÆ∂?∏Ê?")
};

UENUM(BlueprintType)
enum class EDecisionImpact : uint8
{
    Minimal            UMETA(DisplayName = "?ÄÂ∞èÂΩ±??),
    Local              UMETA(DisplayName = "?¨Âú∞ÂΩ±Èüø"),
    Regional           UMETA(DisplayName = "?Ä?üÂΩ±??),
    National           UMETA(DisplayName = "?ãÂÆ∂ÂΩ±Èüø"),
    International      UMETA(DisplayName = "?ãÈ?ÂΩ±Èüø"),
    Historical          UMETA(DisplayName = "Ê≠∑Âè≤ÂΩ±Èüø")
};

USTRUCT(BlueprintType)
struct FDecisionOption
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    FString OptionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    FString OptionDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    EDecisionAlignment Alignment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    EDecisionImpact Impact;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    float HistoricalAccuracy;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    float DifficultyRating;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    TArray<FString> RequiredSkills;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    TArray<FString> Consequences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    TArray<FString> RiskFactors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Option")
    bool bIsAvailable;

    FDecisionOption()
    {
        OptionText = TEXT("");
        OptionDescription = TEXT("");
        Alignment = EDecisionAlignment::Historical;
        Impact = EDecisionImpact::Local;
        HistoricalAccuracy = 0.0f;
        DifficultyRating = 0.0f;
        bIsAvailable = true;
    }
};

USTRUCT(BlueprintType)
struct FDecisionOutcome
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    FString OutcomeType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    float ImpactValue;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    FString AffectedArea;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    FString TimeDelay;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decision Outcome")
    TArray<FString> ChainReactions;

    FDecisionOutcome()
    {
        OutcomeType = TEXT("");
        Description = TEXT("");
        ImpactValue = 0.0f;
        AffectedArea = TEXT("");
        TimeDelay = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FHistoricalSituation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString SituationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString HistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString TimePeriod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    ERoleplayType RoleplayType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString CharacterRole;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    TArray<FString> AvailableResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    TArray<FString> Constraints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    TArray<FDecisionOption> DecisionOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    TArray<FString> HistoricalReferences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString BackgroundImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    FString BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    bool bIsCriticalMoment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Historical Situation")
    bool bCanRepeat;

    FHistoricalSituation()
    {
        SituationID = TEXT("");
        Title = TEXT("");
        Description = TEXT("");
        HistoricalContext = TEXT("");
        TimePeriod = TEXT("");
        Location = TEXT("");
        RoleplayType = ERoleplayType::PoliticalDecision;
        CharacterRole = TEXT("");
        BackgroundImage = TEXT("");
        BackgroundMusic = TEXT("");
        bIsCriticalMoment = false;
        bCanRepeat = false;
    }
};

USTRUCT(BlueprintType)
struct FRoleplaySession
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    FString SessionID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    FString CharacterID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    FString CurrentSituationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    TArray<FString> CompletedSituations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    TArray<FString> DecisionHistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    float HistoricalAccuracyScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    float DecisionQualityScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    float ImmersionScore;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    FDateTime StartTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    FDateTime EndTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    bool bIsActive;

    FRoleplaySession()
    {
        SessionID = TEXT("");
        CharacterID = TEXT("");
        CurrentSituationID = TEXT("");
        HistoricalAccuracyScore = 0.0f;
        DecisionQualityScore = 0.0f;
        ImmersionScore = 0.0f;
        bIsActive = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoleplaySessionStarted, const FString&, SessionID, const FString&, CharacterID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDecisionMade, const FString&, SessionID, const FString&, SituationID, const FString&, DecisionText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoleplaySessionEnded, const FString&, SessionID, const FRoleplaySession&, SessionData);

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalRoleplay : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalRoleplay();

    // ËßíËâ≤?ÆÊ?‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionStarted OnRoleplaySessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionEnded OnRoleplaySessionEnded;

    // ?ùÂ??ñË??≤ÊâÆÊºîÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    void InitializeRoleplaySystem();

    // ?ãÂ?ËßíËâ≤?ÆÊ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    FString StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter);

    // ÁµêÊ?ËßíËâ≤?ÆÊ??ÉË©±
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool EndRoleplaySession(const FString& SessionID);

    // ?≤Â??∂Â??ÉË©±
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FRoleplaySession GetCurrentSession() const;

    // ?≤Â??ØÁî®?ÖÂ?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FHistoricalSituation> GetAvailableSituations(const FString& CharacterID) const;

    // ?ãÂ??ÖÂ?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool StartSituation(const FString& SessionID, const FString& SituationID);

    // ?≤Â??∂Â??ÖÂ?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FHistoricalSituation GetCurrentSituation(const FString& SessionID) const;

    // ?öÂá∫Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool MakeDecision(const FString& SessionID, int32 DecisionIndex);

    // ?≤Â?Ê±∫Á??∏È?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOption> GetAvailableDecisionOptions(const FString& SessionID) const;

    // Ë®àÁ?Ê±∫Á?ÁµêÊ?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOutcome> CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const;

    // Ë©ï‰º∞Ê±∫Á?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const;

    // ?≤Â?Ê≠∑Âè≤Ê∫ñÁ¢∫?ßË???    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetHistoricalAccuracyScore(const FString& SessionID) const;

    // ?≤Â?Ê±∫Á?Ë≥™È?Ë©ïÂ?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetDecisionQualityScore(const FString& SessionID) const;

    // ?≤Â?Ê≤âÊµ∏?üË???    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetImmersionScore(const FString& SessionID) const;

    // ?≤Â??ÉË©±Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FString> GetSessionHistory(const FString& SessionID) const;

    // ?≤Â?ËßíËâ≤?ÆÊ?Áµ±Ë?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TMap<FString, float> GetRoleplayStatistics(const FString& SessionID) const;

    // ‰øùÂ?ËßíËâ≤?ÆÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool SaveRoleplayData(const FString& SaveSlotName);

    // ËºâÂÖ•ËßíËâ≤?ÆÊ??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool LoadRoleplayData(const FString& SaveSlotName);

protected:
    // ?Ä?âÊ≠∑?≤Ê?Â¢?    UPROPERTY()
    TArray<FHistoricalSituation> AllSituations;

    // ?ÖÂ?ID?∞Ê?Â¢ÉÁ??†Â?
    UPROPERTY()
    TMap<FString, FHistoricalSituation> SituationMap;

    // ËßíËâ≤ID?∞Ê?Â¢ÉÂ?Ë°®Á??†Â? - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??UPROPERTY
    TMap<FString, TArray<FString>> CharacterSituationMap;

    // Ê¥ªÂ??ÉË©±
    UPROPERTY()
    TMap<FString, FRoleplaySession> ActiveSessions;

    // ?∂Â??©ÂÆ∂ËßíËâ≤
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bIsInitialized;

    // ?ùÂ??ñÊ?Â¢ÉÂ∫´
    void InitializeSituationLibrary();

    // ?µÂª∫?øÊ≤ªÊ±∫Á??ÖÂ?
    void CreatePoliticalDecisionSituations();

    // ?µÂª∫Ëªç‰?Á≠ñÁï•?ÖÂ?
    void CreateMilitaryStrategySituations();

    // ?µÂª∫Â§ñ‰∫§Ë´áÂà§?ÖÂ?
    void CreateDiplomaticNegotiationSituations();

    // ?µÂª∫Á∂ìÊ??øÁ??ÖÂ?
    void CreateEconomicPolicySituations();

    // ?µÂª∫Á§æÊ??πÈù©?ÖÂ?
    void CreateSocialReformSituations();

    // ?µÂª∫?ã‰∫∫?∏Ê??ÖÂ?
    void CreatePersonalChoiceSituations();

    // È©óË?Ê±∫Á??∏È?
    bool ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const;

    // ?âÁî®Ê±∫Á?ÁµêÊ?
    void ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // ?¥Êñ∞?ÉË©±Ë©ïÂ?
    void UpdateSessionScores(const FString& SessionID, int32 DecisionIndex);

    // Ë®àÁ?Ê≠∑Âè≤Ê∫ñÁ¢∫??    float CalculateHistoricalAccuracy(const FDecisionOption& Decision) const;

    // Ë®àÁ?Ê±∫Á?Ë≥™È?
    float CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const;

    // Ë®àÁ?Ê≤âÊµ∏??    float CalculateImmersionScore(const FString& SessionID) const;

    // ?üÊ??ÉË©±ID
    FString GenerateSessionID(const FString& CharacterID) const;

    // Ê™¢Êü•?ÖÂ??ØÁî®??    bool IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const;

    // ?≤Â?ËßíËâ≤?Ä??    TArray<FString> GetCharacterSkills(const FString& CharacterID) const;

    // Ê™¢Êü•Ê±∫Á??çÁΩÆÊ¢ù‰ª∂
    bool CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const;

    // Ë®òÈ?Ê±∫Á?Ê≠∑Âè≤
    void RecordDecisionHistory(const FString& SessionID, const FString& DecisionText);

    // ?¥Êñ∞ËßíËâ≤?ú‰?
    void UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // Ëß∏ÁôºÂæåÁ?‰∫ã‰ª∂
    void TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // ?üÊ??ÖÂ?ID
    FString GenerateSituationID(const FString& BaseName, ERoleplayType Type) const;
};
