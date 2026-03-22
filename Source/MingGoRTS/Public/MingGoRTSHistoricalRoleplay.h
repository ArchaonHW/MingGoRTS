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
    PoliticalDecision   UMETA(DisplayName = "政治決策"),
    MilitaryStrategy    UMETA(DisplayName = "軍事策略"),
    DiplomaticNegotiation UMETA(DisplayName = "外交談判"),
    EconomicPolicy      UMETA(DisplayName = "經濟政策"),
    SocialReform        UMETA(DisplayName = "社會改革"),
    PersonalChoice      UMETA(DisplayName = "個人選擇")
};

UENUM(BlueprintType)
enum class EDecisionAlignment : uint8
{
    Historical          UMETA(DisplayName = "歷史一致"),
    Alternative         UMETA(DisplayName = "替代選擇"),
    CounterFactual      UMETA(DisplayName = "反事實"),
    PlayerChoice        UMETA(DisplayName = "玩家選擇")
};

UENUM(BlueprintType)
enum class EDecisionImpact : uint8
{
    Minimal            UMETA(DisplayName = "最小影響"),
    Local              UMETA(DisplayName = "本地影響"),
    Regional           UMETA(DisplayName = "區域影響"),
    National           UMETA(DisplayName = "國家影響"),
    International      UMETA(DisplayName = "國際影響"),
    Historical          UMETA(DisplayName = "歷史影響")
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

    // 角色扮演事件
    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionStarted OnRoleplaySessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable, Category = "Roleplay System")
    FOnRoleplaySessionEnded OnRoleplaySessionEnded;

    // 初始化角色扮演系統
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    void InitializeRoleplaySystem();

    // 開始角色扮演會話
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    FString StartRoleplaySession(const FString& CharacterID, const FMingCharacterData& PlayerCharacter);

    // 結束角色扮演會話
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool EndRoleplaySession(const FString& SessionID);

    // 獲取當前會話
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FRoleplaySession GetCurrentSession() const;

    // 獲取可用情境
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FHistoricalSituation> GetAvailableSituations(const FString& CharacterID) const;

    // 開始情境
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool StartSituation(const FString& SessionID, const FString& SituationID);

    // 獲取當前情境
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    FHistoricalSituation GetCurrentSituation(const FString& SessionID) const;

    // 做出決策
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool MakeDecision(const FString& SessionID, int32 DecisionIndex);

    // 獲取決策選項
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOption> GetAvailableDecisionOptions(const FString& SessionID) const;

    // 計算決策結果
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FDecisionOutcome> CalculateDecisionOutcomes(const FString& SessionID, int32 DecisionIndex) const;

    // 評估決策
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float EvaluateDecision(const FString& SessionID, int32 DecisionIndex) const;

    // 獲取歷史準確性評分
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetHistoricalAccuracyScore(const FString& SessionID) const;

    // 獲取決策質量評分
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetDecisionQualityScore(const FString& SessionID) const;

    // 獲取沉浸感評分
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    float GetImmersionScore(const FString& SessionID) const;

    // 獲取會話歷史
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TArray<FString> GetSessionHistory(const FString& SessionID) const;

    // 獲取角色扮演統計
    UFUNCTION(BlueprintPure, Category = "Roleplay System")
    TMap<FString, float> GetRoleplayStatistics(const FString& SessionID) const;

    // 保存角色扮演數據
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool SaveRoleplayData(const FString& SaveSlotName);

    // 載入角色扮演數據
    UFUNCTION(BlueprintCallable, Category = "Roleplay System")
    bool LoadRoleplayData(const FString& SaveSlotName);

protected:
    // 所有歷史情境
    UPROPERTY()
    TArray<FHistoricalSituation> AllSituations;

    // 情境ID到情境的映射
    UPROPERTY()
    TMap<FString, FHistoricalSituation> SituationMap;

    // 角色ID到情境列表的映射 - 注意：TMap<TArray> 不支持 UPROPERTY
    TMap<FString, TArray<FString>> CharacterSituationMap;

    // 活動會話
    UPROPERTY()
    TMap<FString, FRoleplaySession> ActiveSessions;

    // 當前玩家角色
    UPROPERTY()
    FMingCharacterData CurrentPlayerCharacter;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化情境庫
    void InitializeSituationLibrary();

    // 創建政治決策情境
    void CreatePoliticalDecisionSituations();

    // 創建軍事策略情境
    void CreateMilitaryStrategySituations();

    // 創建外交談判情境
    void CreateDiplomaticNegotiationSituations();

    // 創建經濟政策情境
    void CreateEconomicPolicySituations();

    // 創建社會改革情境
    void CreateSocialReformSituations();

    // 創建個人選擇情境
    void CreatePersonalChoiceSituations();

    // 驗證決策選項
    bool ValidateDecisionOption(const FDecisionOption& Option, const FString& CharacterID) const;

    // 應用決策結果
    void ApplyDecisionOutcomes(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // 更新會話評分
    void UpdateSessionScores(const FString& SessionID, int32 DecisionIndex);

    // 計算歷史準確性
    float CalculateHistoricalAccuracy(const FDecisionOption& Decision) const;

    // 計算決策質量
    float CalculateDecisionQuality(const FDecisionOption& Decision, const FString& CharacterID) const;

    // 計算沉浸感
    float CalculateImmersionScore(const FString& SessionID) const;

    // 生成會話ID
    FString GenerateSessionID(const FString& CharacterID) const;

    // 檢查情境可用性
    bool IsSituationAvailable(const FString& SituationID, const FString& CharacterID) const;

    // 獲取角色技能
    TArray<FString> GetCharacterSkills(const FString& CharacterID) const;

    // 檢查決策前置條件
    bool CheckDecisionPrerequisites(const FDecisionOption& Option, const FString& CharacterID) const;

    // 記錄決策歷史
    void RecordDecisionHistory(const FString& SessionID, const FString& DecisionText);

    // 更新角色關係
    void UpdateCharacterRelationships(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // 觸發後續事件
    void TriggerFollowUpEvents(const FString& SessionID, const TArray<FDecisionOutcome>& Outcomes);

    // 生成情境ID
    FString GenerateSituationID(const FString& BaseName, ERoleplayType Type) const;
};
