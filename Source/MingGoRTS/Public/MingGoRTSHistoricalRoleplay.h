#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSHistoricalRoleplay.generated.h"

UENUM(BlueprintType)
enum class ERoleplayType: uint8 {
    PoliticalDecision, UMETA(DisplayName = "?�v�M?"),
    MilitaryStrategy, UMETA(DisplayName = "�x?����"),
    DiplomaticNegotiation, UMETA(DisplayName = "�~��ͧP"),
    EconomicPolicy, UMETA(DisplayName = "�gX"),
    SocialReform, UMETA(DisplayName = "��X��"),
    PersonalChoice, UMETA(DisplayName = "?�HX")
};

UENUM(BlueprintType)
enum class EDecisionAlignment: uint8 {
    Historical, UMETA(DisplayName = "���v�@X),
    Alternative, UMETA(DisplayName = "?�NX"),
    CounterFactual, UMETA(DisplayName = "X),
    PlayerChoice, UMETA(DisplayName = "?�aX")
};

UENUM(BlueprintType)
enum class EDecisionImpact: uint8 {
    Minimal, UMETA(DisplayName = "?�p�vX),
    Local, UMETA(DisplayName = "?�a�v�T"),
    Regional, UMETA(DisplayName = "X�vX),
    National, UMETA(DisplayName = "?�a�v�T"),
    International, UMETA(DisplayName = "X�v�T"),
    Historical, UMETA(DisplayName = "���v�v�T")
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
        OptionText = TEXT(""};
        OptionDescription = TEXT(""};
        Alignment = EDecisionAlignment::Historical;
        Impact = EDecisionImpact::Local;
        HistoricalAccuracy = 0.0f;
        DifficultyRating = 0.0f;
        bIsAvailable = true;
    }
);

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
        OutcomeType = TEXT(""};
        Description = TEXT(""};
        ImpactValue = 0.0f;
        AffectedArea = TEXT(""};
        TimeDelay = TEXT(""};
    }
);

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
        SituationID = TEXT(""};
        Title = TEXT(""};
        Description = TEXT(""};
        HistoricalContext = TEXT(""};
        TimePeriod = TEXT(""};
        Location = TEXT(""};
        RoleplayType = ERoleplayType::PoliticalDecision;
        CharacterRole = TEXT(""};
        BackgroundImage = TEXT(""};
        BackgroundMusic = TEXT(""};
        bIsCriticalMoment = false;
        bCanRepeat = false;
    }
);

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
        SessionID = TEXT(""};
        CharacterID = TEXT(""};
        CurrentSituationID = TEXT(""};
        HistoricalAccuracyScore = 0.0f;
        DecisionQualityScore = 0.0f;
        ImmersionScore = 0.0f;
        bIsActive = false;
    }
);





UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalRoleplay : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalRoleplay();

    // ����X�ƥ�
    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionStarted OnRoleplaySessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionEnded OnRoleplaySessionEnded;

    // X��t�t?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    void InitializeRoleplaySystem();

    // X����X��
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    FString StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter);

    // ��?����X��
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool EndRoleplaySession(const FString& SessionID);

    // X��
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FRoleplaySession GetCurrentSession() const;

    // X��X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FHistoricalSituation> GetAvailableSituations(const FString& CharacterID) const;

    // X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool StartSituation(const FString& SessionID, const FString& SituationID);

    // X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FHistoricalSituation GetCurrentSituation(const FString& SessionID) const;

    // ?�X�M?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool MakeDecision(const FString& SessionID, int32 DecisionIndex);

    // X�MX
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOption> GetAvailableDecisionOptions(const FString& SessionID) const;

    // �p?�M?��?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOutcome> CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const;

    // ����M?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const;

    // X���v�ǽTX
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetHistoricalAccuracyScore(const FString& SessionID) const;

    // X�M?��?��?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetDecisionQualityScore(const FString& SessionID) const;

    // X�I��X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetImmersionScore(const FString& SessionID) const;

    // X�ܾ��v
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FString> GetSessionHistory(const FString& SessionID) const;

    // X����X��?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TMap<FString, float> GetRoleplayStatistics(const FString& SessionID) const;

    // �O?����X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool SaveRoleplayData(const FString& SaveSlotName);

    // ���J����X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool LoadRoleplayData(const FString& SaveSlotName);

protected:
    // X��X
    UPROPERTY()
    TArray<FHistoricalSituation> AllSituations;

    // XIDX��X
    UPROPERTY()
    TMap<FString, FHistoricalSituation> SituationMap;

    // ����IDX��?��X - �`?�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FString>> CharacterSituationMap;

    // ��X��
    UPROPERTY()
    TMap<FString, FRoleplaySession> ActiveSessions;

    // X�a����
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // ?�_�w?�l?
    bool bIsInitialized;

    // X�Үw
    void InitializeSituationLibrary();

    // ?��?�v�MX
    void CreatePoliticalDecisionSituations();

    // ?�حx?����X
    void CreateMilitaryStrategySituations();

    // ?�إ~��ͧPX
    void CreateDiplomaticNegotiationSituations();

    // ?�ظgX
    void CreateEconomicPolicySituations();

    // ?�ت�X��X
    void CreateSocialReformSituations();

    // ?��?�HX
    void CreatePersonalChoiceSituations();

    // ��?�MX
    bool ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const;

    // ?�ΨM?��?
    void ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // ?�s?�ܵ�?
    void UpdateSessionScores(const FString& SessionID, int32 DecisionIndex);

    // �p?���v�ǽTX
    float CalculateHistoricalAccuracy(const FDecisionOption& Decision) const;

    // �p?�M?��?
    float CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const;

    // �p?�I��X
    float CalculateImmersionScore(const FString& SessionID) const;

    // X��ID
    FString GenerateSessionID(const FString& CharacterID) const;

    // �ˬdX��X
    bool IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const;

    // X����X
    TArray<FString> GetCharacterSkills(const FString& CharacterID) const;

    // �ˬd�MX�m����
    bool CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const;

    // �O?�M?���v
    void RecordDecisionHistory(const FString& SessionID, const FString& DecisionText);

    // ?�s����X
    void UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // Ĳ�o��?�ƥ�
    void TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // XID
    FString GenerateSituationID(const FString& BaseName, ERoleplayType Type) const;
};

