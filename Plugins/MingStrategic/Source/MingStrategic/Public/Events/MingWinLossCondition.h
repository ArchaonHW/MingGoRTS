#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingWinLossCondition.generated.h"

/**
 * ?��?結�?類�?
 */
UENUM(BlueprintType)
enum class EWinLossResult: uint8 {
    Victory             UMETA(DisplayName = "Victory"),
    Defeat              UMETA(DisplayName = "Defeat"),
    Draw                UMETA(DisplayName = "Draw"),
    Stalemate           UMETA(DisplayName = "Stalemate"),
    InProgress          UMETA(DisplayName = "In Progress"),
    Unknown             UMETA(DisplayName = "Unknown")
};

/**
 * ?��?條件類�?
 */
UENUM(BlueprintType)
enum class EWinLossConditionType: uint8 {
    Military             UMETA(DisplayName = "Military"),
    Political            UMETA(DisplayName = "Political"),
    Economic             UMETA(DisplayName = "Economic"),
    Territorial          UMETA(DisplayName = "Territorial"),
    TimeBased            UMETA(DisplayName = "Time Based"),
    ScoreBased           UMETA(DisplayName = "Score Based"),
    Custom               UMETA(DisplayName = "Custom")
};

/**
 * ?��X��?�? */
UENUM(BlueprintType)
enum class EWinLossPriority: uint8 {
    Critical            UMETA(DisplayName = "Critical"),
    High                UMETA(DisplayName = "High"),
    Medium              UMETA(DisplayName = "Medium"),
    Low                 UMETA(DisplayName = "Low"),
    Background          UMETA(DisplayName = "Background")
};

/**
 * ?��?條件?�X */
UENUM(BlueprintType)
enum class EWinLossConditionState: uint8 {
    Inactive            UMETA(DisplayName = "Inactive"),
    Active              UMETA(DisplayName = "Active"),
    Satisfied           UMETA(DisplayName = "Satisfied"),
    Failed              UMETA(DisplayName = "Failed"),
    Disabled            UMETA(DisplayName = "Disabled")
};

/**
 * ?��?條件?�數
 */
USTRUCT(BlueprintType)
struct FWinLossConditionParameter
{
    GENERATED_BODY()

    // ?�數?�稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterName;

    // ?�數X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterValue;

    // ?�數類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterType; // "int", "float", "bool", "string"

    // 比�X��?�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ComparisonOperator; // ">", "<", "==", ">=", "<=", "contains"

    // ?�否必�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    bool bIsRequired;

    // 權�?
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
 * ?��?條件定義
 */
USTRUCT(BlueprintType)
struct FWinLossCondition
{
    GENERATED_BODY()

    // 條件ID
    UPROPERTY(BlueprintReadOnly)
    FString ConditionID;

    // 條件?�稱
    UPROPERTY(BlueprintReadOnly)
    FString ConditionName;

    // 條件?�述
    UPROPERTY(BlueprintReadOnly)
    FString ConditionDescription;

    // 條件類�?
    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionType ConditionType;

    // ?��?結�?類�?
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult ResultType;

    // ?��?�?
    UPROPERTY(BlueprintReadOnly)
    EWinLossPriority Priority;

    // 條件?�數
    UPROPERTY(BlueprintReadOnly)
    TArray<FWinLossConditionParameter> Parameters;

    // 條件?�X
    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionState State;

    // ?�否?��X��?�?
    UPROPERTY(BlueprintReadOnly)
    bool bIsCriticalCondition;

    // 檢查?��? (�?
    UPROPERTY(BlueprintReadOnly)
    float CheckInterval;

    // 上次檢查?��?
    UPROPERTY(BlueprintReadOnly)
    float LastCheckTime;

    // 滿足度�X(0-1)
    UPROPERTY(BlueprintReadOnly)
    float SatisfactionScore;

    // 條件標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConditionTags;

    // ?��?事件ID
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
 * ?��?評估結�?
 */
USTRUCT(BlueprintType)
struct FWinLossEvaluationResult
{
    GENERATED_BODY()

    // 評估結�?
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult Result;

    // 總�?
    UPROPERTY(BlueprintReadOnly)
    float TotalScore;

    // ?�利?�數
    UPROPERTY(BlueprintReadOnly)
    float VictoryScore;

    // 失�X�數
    UPROPERTY(BlueprintReadOnly)
    float DefeatScore;

    // 平�X�數
    UPROPERTY(BlueprintReadOnly)
    float DrawScore;

    // 滿足?��?�?
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SatisfiedConditions;

    // ?�滿足�?條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnsatisfiedConditions;

    // ?�鍵條件?�X
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EWinLossConditionState> CriticalConditionStates;

    // 評估?��?
    UPROPERTY(BlueprintReadOnly)
    float EvaluationTime;

    // 詳估詳�?
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
 * ?��?統�X��?
 */
USTRUCT(BlueprintType)
struct FWinLossStatistics
{
    GENERATED_BODY()

    // 總�?估次X
    UPROPERTY(BlueprintReadOnly)
    int32 TotalEvaluations;

    // ?�利次數
    UPROPERTY(BlueprintReadOnly)
    int32 VictoryCount;

    // 失�?次數
    UPROPERTY(BlueprintReadOnly)
    int32 DefeatCount;

    // 平�?次數
    UPROPERTY(BlueprintReadOnly)
    int32 DrawCount;

    // ?��?
    UPROPERTY(BlueprintReadOnly)
    float WinRate;

    // 平�?評估?��?
    UPROPERTY(BlueprintReadOnly)
    float AverageEvaluationTime;

    // 條件滿足X
    UPROPERTY(BlueprintReadOnly)
    float ConditionSatisfactionRate;

    // ?�鍵條件觸發次數
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
 * 民�X��X��?條件系統
 * 管�X�?��?負�?件�?檢測?��?估�X�知
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingWinLossCondition : public UObject
{
    GENERATED_BODY()

public:
    UMingWinLossCondition();

    /**
     * ?��X��?負�?件系�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Initialize();

    /**
     * ?��X��?條件系統
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Shutdown();

    /**
     * 註�X��?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool RegisterWinLossCondition(const FWinLossCondition& Condition);

    /**
     * ?��?註�X��?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool UnregisterWinLossCondition(const FString& ConditionID);

    /**
     * 評估?��X��?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult EvaluateWinLoss();

    /**
     * 檢查?�個�?�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool CheckCondition(const FString& ConditionID);

    /**
     * ?��X��?條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossCondition GetWinLossCondition(const FString& ConditionID) const;

    /**
     * ?��X�?��?負�?�?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetAllWinLossConditions() const;

    /**
     * ?��X��?類�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType) const;

    /**
     * ?��X�鍵條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetCriticalConditions() const;

    /**
     * ?��?已滿足�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetSatisfiedConditions() const;

    /**
     * ?��X�滿足�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetUnsatisfiedConditions() const;

    /**
     * 設置?�戲?�X     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void SetGameState(const TMap<FString, FString>& GameState);

    /**
     * ?��X�戲?�X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TMap<FString, FString> GetGameState() const;

    /**
     * ?�新?�戲?�?��?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void UpdateGameStateValue(const FString& Key, const FString& Value);

    /**
     * ?��X��?統�?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossStatistics GetWinLossStatistics() const;

    /**
     * ?�置統�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void ResetStatistics();

    /**
     * 強制評估 (?�於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult ForceEvaluation();

    /**
     * ?��X��X��?結�?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    EWinLossResult GetCurrentResult() const;

    /**
     * ?��X��X�數
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    float GetWinLossScore() const;

    /**
     * 檢查?�否已�X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    bool IsGameOver() const;

    /**
     * ?��?結�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FString GetGameOverReason() const;

protected:
    // 已註?��X��?條件
    UPROPERTY()
    TArray<FWinLossCondition> RegisteredConditions;

    // 條件ID?��?件�X��?
    UPROPERTY()
    TMap<FString, FWinLossCondition> ConditionMap;

    // ?�戲?�X
    UPROPERTY()
    TMap<FString, FString> GameState;

    // ?��?統�?
    UPROPERTY()
    FWinLossStatistics Statistics;

    // ?��?評估結�?
    UPROPERTY()
    FWinLossEvaluationResult CurrentEvaluation;

    // ?�否已�?始�?
    bool bIsInitialized;

    // ?��X��X�歷?��?負�?�?
    void InitializeHistoricalWinLossConditions();

    // ?�建軍�X��?條件
    void CreateMilitaryConditions();

    // ?�建?�治?��?條件
    void CreatePoliticalConditions();

    // ?�建經�X��?條件
    void CreateEconomicConditions();

    // ?�建?��X��?條件
    void CreateTerritorialConditions();

    // ?�建?��X��?條件
    void CreateTimeBasedConditions();

    // ?�建?�數?��?條件
    void CreateScoreBasedConditions();

    // 檢查軍�?條件
    bool CheckMilitaryCondition(const FWinLossCondition& Condition);

    // 檢查?�治條件
    bool CheckPoliticalCondition(const FWinLossCondition& Condition);

    // 檢查經�?條件
    bool CheckEconomicCondition(const FWinLossCondition& Condition);

    // 檢查?��?條件
    bool CheckTerritorialCondition(const FWinLossCondition& Condition);

    // 檢查?��?條件
    bool CheckTimeBasedCondition(const FWinLossCondition& Condition);

    // 檢查?�數條件
    bool CheckScoreBasedCondition(const FWinLossCondition& Condition);

    // 檢查?��?義�?�?
    bool CheckCustomCondition(const FWinLossCondition& Condition);

    // 評估條件滿足�?
    float EvaluateConditionSatisfaction(const FWinLossCondition& Condition);

    // 計�X��X�數
    void CalculateWinLossScores(FWinLossEvaluationResult& Result);

    // 確�X�終�X
    EWinLossResult DetermineFinalResult(const FWinLossEvaluationResult& Result);

    // ?�新統�X��?
    void UpdateStatistics(const FWinLossEvaluationResult& Result);

    // ?�知?��?變�?
    void NotifyWinLossChange(EWinLossResult OldResult, EWinLossResult NewResult);

    // ?��X�戲?�?��?
    FString GetGameStateValue(const FString& Key) const;

    // 比�X��?
    bool CompareValues(const FString& Value1, const FString& Operator, const FString& Value2) const;

    // 驗�?條件?�置
    bool ValidateConditionConfig(const FWinLossCondition& Condition) const;

    // 記�X��X��?
    void LogWinLossEvent(const FString& Message);

    // ?��X��X��?
    FString GenerateWinLossReport(const FWinLossEvaluationResult& Result) const;

    // �X條件?�數
    TArray<FString> ParseConditionParameters(const FString& ParameterString) const;

    // 設置條件?�X
    void SetConditionState(const FString& ConditionID, EWinLossConditionState NewState);

    // ?��?條件滿足�?
    float GetConditionSatisfaction(const FString& ConditionID) const;

    // 檢查?�鍵條件
    bool CheckCriticalConditions();

    // ?��X��?結�?
    void HandleGameOver(EWinLossResult Result, const FString& Reason);
);

