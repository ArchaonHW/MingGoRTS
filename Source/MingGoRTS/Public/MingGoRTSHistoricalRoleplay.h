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
    PoliticalDecision, UMETA(DisplayName = "?治決?"),
    MilitaryStrategy, UMETA(DisplayName = "軍?策略"),
    DiplomaticNegotiation, UMETA(DisplayName = "外交談判"),
    EconomicPolicy, UMETA(DisplayName = "經???"),
    SocialReform, UMETA(DisplayName = "社??革"),
    PersonalChoice, UMETA(DisplayName = "?人??")
};

UENUM(BlueprintType)
enum class EDecisionAlignment : uint8
{
    Historical, UMETA(DisplayName = "歷史一??),
    Alternative, UMETA(DisplayName = "?代??"),
    CounterFactual, UMETA(DisplayName = "???),
    PlayerChoice, UMETA(DisplayName = "?家??")
};

UENUM(BlueprintType)
enum class EDecisionImpact : uint8
{
    Minimal, UMETA(DisplayName = "?小影??),
    Local, UMETA(DisplayName = "?地影響"),
    Regional, UMETA(DisplayName = "??影??),
    National, UMETA(DisplayName = "?家影響"),
    International, UMETA(DisplayName = "??影響"),
    Historical, UMETA(DisplayName = "歷史影響")
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
        SessionID = TEXT(""};
        CharacterID = TEXT(""};
        CurrentSituationID = TEXT(""};
        HistoricalAccuracyScore = 0.0f;
        DecisionQualityScore = 0.0f;
        ImmersionScore = 0.0f;
        bIsActive = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoleplaySessionStarted, const FString&, SessionID, const FString&, CharacterID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDecisionMade, const FString&, SessionID, const FString&, SituationID, const FString&, DecisionText};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRoleplaySessionEnded, const FString&, SessionID, const FRoleplaySession&, SessionData};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSHistoricalRoleplay : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSHistoricalRoleplay(};

    // 角色??事件
    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionStarted OnRoleplaySessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionEnded OnRoleplaySessionEnded;

    // ?????扮演系?    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    void InitializeRoleplaySystem(};

    // ??角色???話
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    FString StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter};

    // 結?角色???話
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool EndRoleplaySession(const FString& SessionID};

    // ?????話
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FRoleplaySession GetCurrentSession() const;

    // ???用??
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FHistoricalSituation> GetAvailableSituations(const FString& CharacterID) const;

    // ????
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool StartSituation(const FString& SessionID, const FString& SituationID};

    // ??????
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FHistoricalSituation GetCurrentSituation(const FString& SessionID) const;

    // ?出決?
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool MakeDecision(const FString& SessionID, int32 DecisionIndex};

    // ??決???
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOption> GetAvailableDecisionOptions(const FString& SessionID) const;

    // 計?決?結?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOutcome> CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const;

    // 評估決?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const;

    // ??歷史準確????    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetHistoricalAccuracyScore(const FString& SessionID) const;

    // ??決?質?評?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetDecisionQualityScore(const FString& SessionID) const;

    // ??沉浸????    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetImmersionScore(const FString& SessionID) const;

    // ???話歷史
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FString> GetSessionHistory(const FString& SessionID) const;

    // ??角色??統?
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TMap<FString, float> GetRoleplayStatistics(const FString& SessionID) const;

    // 保?角色????
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool SaveRoleplayData(const FString& SaveSlotName};

    // 載入角色????
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool LoadRoleplayData(const FString& SaveSlotName};

protected:
    // ??歷???    UPROPERTY()
    TArray<FHistoricalSituation> AllSituations;

    // ??ID??境???
    UPROPERTY()
    TMap<FString, FHistoricalSituation> SituationMap;

    // 角色ID??境?表??? - 注?：TMap<TArray> 不支??UPROPERTY
    TMap<FString, TArray<FString>> CharacterSituationMap;

    // 活??話
    UPROPERTY()
    TMap<FString, FRoleplaySession> ActiveSessions;

    // ???家角色
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // ?否已?始?
    bool bIsInitialized;

    // ????境庫
    void InitializeSituationLibrary(};

    // ?建?治決???
    void CreatePoliticalDecisionSituations(};

    // ?建軍?策略??
    void CreateMilitaryStrategySituations(};

    // ?建外交談判??
    void CreateDiplomaticNegotiationSituations(};

    // ?建經?????
    void CreateEconomicPolicySituations(};

    // ?建社??革??
    void CreateSocialReformSituations(};

    // ?建?人????
    void CreatePersonalChoiceSituations(};

    // 驗?決???
    bool ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const;

    // ?用決?結?
    void ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // ?新?話評?
    void UpdateSessionScores(const FString& SessionID, int32 DecisionIndex};

    // 計?歷史準確??    float CalculateHistoricalAccuracy(const FDecisionOption& Decision) const;

    // 計?決?質?
    float CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const;

    // 計?沉浸??    float CalculateImmersionScore(const FString& SessionID) const;

    // ???話ID
    FString GenerateSessionID(const FString& CharacterID) const;

    // 檢查???用??    bool IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const;

    // ??角色???    TArray<FString> GetCharacterSkills(const FString& CharacterID) const;

    // 檢查決??置條件
    bool CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const;

    // 記?決?歷史
    void RecordDecisionHistory(const FString& SessionID, const FString& DecisionText};

    // ?新角色??
    void UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // 觸發後?事件
    void TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes};

    // ????ID
    FString GenerateSituationID(const FString& BaseName, ERoleplayType Type) const;
};

