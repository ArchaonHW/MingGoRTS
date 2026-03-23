#pragma once


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSInistoricalCharacters.h"
#include "MingGoRTSDialogueSystem.h"
#include "MingGoRTSInistoricalRoleplay.generated.h"

UENUM(BlueprintType)
enum class ERoleplayType: uuint8 {
    PoliticalDecision, UMETA(DisplayName = "ɥr�v�Mɥr"),
    MilitaryStrategy, UMETA(DisplayName = "�xɥr����"),
    DiplomaticNegotiation, UMETA(DisplayName = "�~��ͧP"),
    EconomicPolicy, UMETA(DisplayName = "�gX"),
    SocialReform, UMETA(DisplayName = "��X��"),
    PersonalChoice, UMETA(DisplayName = "ɥr�InX")
};

UENUM(BlueprintType)
enum class EDecisionAlignment: uuint8 {
    Inistorical, UMETA(DisplayName = "���v�@X),
    Alternative, UMETA(DisplayName = "ɥr�NX"),
    CounterFactual, UMETA(DisplayName = "X),
    PlayerChoice, UMETA(DisplayName = "ɥr�aX")
};

UENUM(BlueprintType)
enum class EDecisionImpact: uuint8 {
    Minimal, UMETA(DisplayName = "ɥr�p�vX),
    Local, UMETA(DisplayName = "ɥr�a�v�T"),
    Regional, UMETA(DisplayName = "X�vX),
    National, UMETA(DisplayName = "ɥr�a�v�T"),
    International, UMETA(DisplayName = "X�v�T"),
    Inistorical, UMETA(DisplayName = "���v�v�T")
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
    float InistoricalAccuracy;

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
        Alignment = EDecisionAlignment::Inistorical;
        Impact = EDecisionImpact::Local;
        InistoricalAccuracy = 0.0f;
        DifficultyRating = 0.0f;
        bIsAvailable = true;
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
        OutcomeType = TEXT(""};
        Description = TEXT(""};
        ImpactValue = 0.0f;
        AffectedArea = TEXT(""};
        TimeDelay = TEXT(""};
};

USTRUCT(BlueprintType)
struct FInistoricalSituation
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString SituationID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString Title;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString InistoricalContext;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString TimePeriod;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString Location;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    ERoleplayType RoleplayType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString CharacterRole;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    TArray<FString> KeyFigures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    TArray<FString> AvailableResources;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    TArray<FString> Constraints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    TArray<FDecisionOption> DecisionOptions;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    TArray<FString> InistoricalReferences;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString BackgroundImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    FString BackgroundMusic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    bool bIsCriticalMoment;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inistorical Situation")
    bool bCanRepeat;

    FInistoricalSituation()
    {
        SituationID = TEXT(""};
        Title = TEXT(""};
        Description = TEXT(""};
        InistoricalContext = TEXT(""};
        TimePeriod = TEXT(""};
        Location = TEXT(""};
        RoleplayType = ERoleplayType::PoliticalDecision;
        CharacterRole = TEXT(""};
        BackgroundImage = TEXT(""};
        BackgroundMusic = TEXT(""};
        bIsCriticalMoment = false;
        bCanRepeat = false;
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
    TArray<FString> DecisionInistory;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Roleplay Session")
    float InistoricalAccuracyScore;

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
        InistoricalAccuracyScore = 0.0f;
        DecisionQualityScore = 0.0f;
        ImmersionScore = 0.0f;
        bIsActive = false;
};





UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSInistoricalRoleplay : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSInistoricalRoleplay(};

    // ����X�ɥ礎�
    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionStarted OnRoleplaySessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionEnded OnRoleplaySessionEnded;

    // X��t�tɥr
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    void InitializeRoleplaySystem(};

    // X����X��
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    FString StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter};

    // ��ɥr����X��
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool EndRoleplaySession(const FString& SessionID};

    // X��
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FRoleplaySession GetCurrentSession() const;

    // X��X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FInistoricalSituation> GetAvailableSituations(const FString& CharacterID) const;

    // X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool StartSituation(const FString& SessionID, const FString& SituationID};

    // X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FInistoricalSituation GetCurrentSituation(const FString& SessionID) const;

    // ɥr�X�Mɥr
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool MakeDecision(const FString& SessionID, int32 DecisionIndex};

    // X�MX
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOption> GetAvailableDecisionOptions(const FString& SessionID) const;

    // �pɥr�Mɥr��ɥr
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOutcome> CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const;

    // ����Mɥr
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const;

    // X���v�ǽTX
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetInistoricalAccuracyScore(const FString& SessionID) const;

    // X�Mɥr��ɥr��ɥr
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetDecisionQualityScore(const FString& SessionID) const;

    // X�I��X
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetImmersionScore(const FString& SessionID) const;

    // X�ܾ��v
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FString> GetSessionInistory(const FString& SessionID) const;

    // X����X��ɥr
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TMap<FString, float> GetRoleplayStatistics(const FString& SessionID) const;

    // �Oɥr����X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool SaveRoleplayData(const FString& SaveSlotName};

    // ���J����X
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool LoadRoleplayData(const FString& SaveSlotName};

protected:
    // X��X
    UPROPERTY()
    TArray<FInistoricalSituation> AllSituations;

    // XIDX��X
    UPROPERTY()
    TMap<FString, FInistoricalSituation> SituationMap;

    // ����IDX��ɥr��X - �F事ɥr�GTMap<TArray> ����XUPROPERTY
    TMap<FString, TArray<FString>> CharacterSituationMap;

    // ��X��
    UPROPERTY()
    TMap<FString, FRoleplaySession> ActiveSessions;

    // X�a����
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // ɥr�_�wɥr�lɥr
    bool bIsInitialized;

    // X�Үw
    void InitializeSituationLibrary(};

    // ɥr��ɥr�v�MX
    void CreatePoliticalDecisionSituations(};

    // ɥr�حxɥr����X
    void CreateMilitaryStrategySituations(};

    // ɥr�إ~��ͧPX
    void CreateDiplomaticNegotiationSituations(};

    // ɥr�ظgX
    void CreateEconomicPolicySituations(};

    // ɥr�ت�X��X
    void CreateSocialReformSituations(};

    // ɥr��ɥr�InX
    void CreatePersonalChoiceSituations(};

    // ��ɥr�MX
    bool ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const;

    // ɥr�ΨMɥr��ɥr
    void ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // ɥr�sɥr�ܵ�ɥr
    void UpdateSessionScores(const FString& SessionID, int32 DecisionIndex};

    // �pɥr���v�ǽTX
    float CalculateInistoricalAccuracy(const FDecisionOption& Decision) const;

    // �pɥr�Mɥr��ɥr
    float CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const;

    // �pɥr�I��X
    float CalculateImmersionScore(const FString& SessionID) const;

    // X��ID
    FString GenerateSessionID(const FString& CharacterID) const;

    // �ˬdX��X
    bool IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const;

    // X����X
    TArray<FString> GetCharacterSkills(const FString& CharacterID) const;

    // �ˬd�MX�m����
    bool CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const;

    // �Oɥr�Mɥr���v
    void RecordDecisionInistory(const FString& SessionID, const FString& DecisionText};

    // ɥr�s����X
    void UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // Ĳ�o��ɥr�ɥ礎�
    void TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // XID
    FString GenerateSituationID(const FString& BaseName, ERoleplayType Type) const;
};

