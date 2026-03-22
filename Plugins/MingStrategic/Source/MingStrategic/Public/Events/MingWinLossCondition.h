#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingWinLossCondition.generated.h"

/**
 * 勝負結果類型
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
 * 勝負條件類型
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
 * 勝負優先級
 */
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
 * 勝負條件狀態
 */
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
 * 勝負條件參數
 */
USTRUCT(BlueprintType)
struct FWinLossConditionParameter
{
    GENERATED_BODY()

    // 參數名稱
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterName;

    // 參數值
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterValue;

    // 參數類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ParameterType; // "int", "float", "bool", "string"

    // 比較操作符
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    FString ComparisonOperator; // ">", "<", "==", ">=", "<=", "contains"

    // 是否必需
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Win Loss Condition")
    bool bIsRequired;

    // 權重
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
 * 勝負條件定義
 */
USTRUCT(BlueprintType)
struct FWinLossCondition
{
    GENERATED_BODY()

    // 條件ID
    UPROPERTY(BlueprintReadOnly)
    FString ConditionID;

    // 條件名稱
    UPROPERTY(BlueprintReadOnly)
    FString ConditionName;

    // 條件描述
    UPROPERTY(BlueprintReadOnly)
    FString ConditionDescription;

    // 條件類型
    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionType ConditionType;

    // 勝負結果類型
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult ResultType;

    // 優先級
    UPROPERTY(BlueprintReadOnly)
    EWinLossPriority Priority;

    // 條件參數
    UPROPERTY(BlueprintReadOnly)
    TArray<FWinLossConditionParameter> Parameters;

    // 條件狀態
    UPROPERTY(BlueprintReadOnly)
    EWinLossConditionState State;

    // 是否為關鍵條件
    UPROPERTY(BlueprintReadOnly)
    bool bIsCriticalCondition;

    // 檢查間隔 (秒)
    UPROPERTY(BlueprintReadOnly)
    float CheckInterval;

    // 上次檢查時間
    UPROPERTY(BlueprintReadOnly)
    float LastCheckTime;

    // 滿足度分數 (0-1)
    UPROPERTY(BlueprintReadOnly)
    float SatisfactionScore;

    // 條件標籤
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> ConditionTags;

    // 相關事件ID
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
 * 勝負評估結果
 */
USTRUCT(BlueprintType)
struct FWinLossEvaluationResult
{
    GENERATED_BODY()

    // 評估結果
    UPROPERTY(BlueprintReadOnly)
    EWinLossResult Result;

    // 總分
    UPROPERTY(BlueprintReadOnly)
    float TotalScore;

    // 勝利分數
    UPROPERTY(BlueprintReadOnly)
    float VictoryScore;

    // 失敗分數
    UPROPERTY(BlueprintReadOnly)
    float DefeatScore;

    // 平局分數
    UPROPERTY(BlueprintReadOnly)
    float DrawScore;

    // 滿足的條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> SatisfiedConditions;

    // 未滿足的條件
    UPROPERTY(BlueprintReadOnly)
    TArray<FString> UnsatisfiedConditions;

    // 關鍵條件狀態
    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EWinLossConditionState> CriticalConditionStates;

    // 評估時間
    UPROPERTY(BlueprintReadOnly)
    float EvaluationTime;

    // 詳估詳情
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
 * 勝負統計數據
 */
USTRUCT(BlueprintType)
struct FWinLossStatistics
{
    GENERATED_BODY()

    // 總評估次數
    UPROPERTY(BlueprintReadOnly)
    int32 TotalEvaluations;

    // 勝利次數
    UPROPERTY(BlueprintReadOnly)
    int32 VictoryCount;

    // 失敗次數
    UPROPERTY(BlueprintReadOnly)
    int32 DefeatCount;

    // 平局次數
    UPROPERTY(BlueprintReadOnly)
    int32 DrawCount;

    // 勝率
    UPROPERTY(BlueprintReadOnly)
    float WinRate;

    // 平均評估時間
    UPROPERTY(BlueprintReadOnly)
    float AverageEvaluationTime;

    // 條件滿足率
    UPROPERTY(BlueprintReadOnly)
    float ConditionSatisfactionRate;

    // 關鍵條件觸發次數
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
 * 民國時期勝負條件系統
 * 管理所有勝負條件的檢測、評估和通知
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingWinLossCondition : public UObject
{
    GENERATED_BODY()

public:
    UMingWinLossCondition();

    /**
     * 初始化勝負條件系統
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Initialize();

    /**
     * 關閉勝負條件系統
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void Shutdown();

    /**
     * 註冊勝負條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool RegisterWinLossCondition(const FWinLossCondition& Condition);

    /**
     * 取消註冊勝負條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool UnregisterWinLossCondition(const FString& ConditionID);

    /**
     * 評估勝負狀況
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult EvaluateWinLoss();

    /**
     * 檢查單個條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    bool CheckCondition(const FString& ConditionID);

    /**
     * 獲取勝負條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossCondition GetWinLossCondition(const FString& ConditionID) const;

    /**
     * 獲取所有勝負條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetAllWinLossConditions() const;

    /**
     * 獲取指定類型的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType) const;

    /**
     * 獲取關鍵條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetCriticalConditions() const;

    /**
     * 獲取已滿足的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetSatisfiedConditions() const;

    /**
     * 獲取未滿足的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    TArray<FWinLossCondition> GetUnsatisfiedConditions() const;

    /**
     * 設置遊戲狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void SetGameState(const TMap<FString, FString>& GameState);

    /**
     * 獲取遊戲狀態
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    TMap<FString, FString> GetGameState() const;

    /**
     * 更新遊戲狀態值
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void UpdateGameStateValue(const FString& Key, const FString& Value);

    /**
     * 獲取勝負統計
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FWinLossStatistics GetWinLossStatistics() const;

    /**
     * 重置統計數據
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    void ResetStatistics();

    /**
     * 強制評估 (用於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    FWinLossEvaluationResult ForceEvaluation();

    /**
     * 獲取當前勝負結果
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    EWinLossResult GetCurrentResult() const;

    /**
     * 獲取勝負分數
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    float GetWinLossScore() const;

    /**
     * 檢查是否已結束
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    bool IsGameOver() const;

    /**
     * 獲取結束原因
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    FString GetGameOverReason() const;

protected:
    // 已註冊的勝負條件
    UPROPERTY()
    TArray<FWinLossCondition> RegisteredConditions;

    // 條件ID到條件的映射
    UPROPERTY()
    TMap<FString, FWinLossCondition> ConditionMap;

    // 遊戲狀態
    UPROPERTY()
    TMap<FString, FString> GameState;

    // 勝負統計
    UPROPERTY()
    FWinLossStatistics Statistics;

    // 當前評估結果
    UPROPERTY()
    FWinLossEvaluationResult CurrentEvaluation;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國歷史勝負條件
    void InitializeHistoricalWinLossConditions();

    // 創建軍事勝負條件
    void CreateMilitaryConditions();

    // 創建政治勝負條件
    void CreatePoliticalConditions();

    // 創建經濟勝負條件
    void CreateEconomicConditions();

    // 創建領土勝負條件
    void CreateTerritorialConditions();

    // 創建時間勝負條件
    void CreateTimeBasedConditions();

    // 創建分數勝負條件
    void CreateScoreBasedConditions();

    // 檢查軍事條件
    bool CheckMilitaryCondition(const FWinLossCondition& Condition);

    // 檢查政治條件
    bool CheckPoliticalCondition(const FWinLossCondition& Condition);

    // 檢查經濟條件
    bool CheckEconomicCondition(const FWinLossCondition& Condition);

    // 檢查領土條件
    bool CheckTerritorialCondition(const FWinLossCondition& Condition);

    // 檢查時間條件
    bool CheckTimeBasedCondition(const FWinLossCondition& Condition);

    // 檢查分數條件
    bool CheckScoreBasedCondition(const FWinLossCondition& Condition);

    // 檢查自定義條件
    bool CheckCustomCondition(const FWinLossCondition& Condition);

    // 評估條件滿足度
    float EvaluateConditionSatisfaction(const FWinLossCondition& Condition);

    // 計算勝負分數
    void CalculateWinLossScores(FWinLossEvaluationResult& Result);

    // 確定最終結果
    EWinLossResult DetermineFinalResult(const FWinLossEvaluationResult& Result);

    // 更新統計數據
    void UpdateStatistics(const FWinLossEvaluationResult& Result);

    // 通知勝負變化
    void NotifyWinLossChange(EWinLossResult OldResult, EWinLossResult NewResult);

    // 獲取遊戲狀態值
    FString GetGameStateValue(const FString& Key) const;

    // 比較數值
    bool CompareValues(const FString& Value1, const FString& Operator, const FString& Value2) const;

    // 驗證條件配置
    bool ValidateConditionConfig(const FWinLossCondition& Condition) const;

    // 記錄勝負日誌
    void LogWinLossEvent(const FString& Message);

    // 生成勝負報告
    FString GenerateWinLossReport(const FWinLossEvaluationResult& Result) const;

    // 解析條件參數
    TArray<FString> ParseConditionParameters(const FString& ParameterString) const;

    // 設置條件狀態
    void SetConditionState(const FString& ConditionID, EWinLossConditionState NewState);

    // 獲取條件滿足度
    float GetConditionSatisfaction(const FString& ConditionID) const;

    // 檢查關鍵條件
    bool CheckCriticalConditions();

    // 處理勝負結束
    void HandleGameOver(EWinLossResult Result, const FString& Reason);
};
