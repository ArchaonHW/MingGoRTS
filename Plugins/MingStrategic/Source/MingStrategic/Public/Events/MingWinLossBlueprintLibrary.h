#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingWinLossCondition.h"
#include "MingWinLossBlueprintLibrary.generated.h"

/**
 * 勝負條件藍圖函數庫
 * 提供藍圖可調用的勝負條件系統功能
 */
UCLASS()
class MINGSTRATEGIC_API UMingWinLossBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取勝負條件系統
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static UMingWinLossCondition* GetWinLossConditionSystem();

    /**
     * 初始化勝負條件系統
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void InitializeWinLossConditionSystem();

    /**
     * 註冊勝負條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool RegisterWinLossCondition(const FWinLossCondition& Condition);

    /**
     * 取消註冊勝負條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool UnregisterWinLossCondition(const FString& ConditionID);

    /**
     * 評估勝負狀況
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult EvaluateWinLoss();

    /**
     * 檢查單個條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool CheckCondition(const FString& ConditionID);

    /**
     * 獲取勝負條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossCondition GetWinLossCondition(const FString& ConditionID);

    /**
     * 獲取所有勝負條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetAllWinLossConditions();

    /**
     * 獲取指定類型的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType);

    /**
     * 獲取關鍵條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetCriticalConditions();

    /**
     * 獲取已滿足的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetSatisfiedConditions();

    /**
     * 獲取未滿足的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetUnsatisfiedConditions();

    /**
     * 設置遊戲狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void SetGameState(const TMap<FString, FString>& GameState);

    /**
     * 獲取遊戲狀態
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TMap<FString, FString> GetGameState();

    /**
     * 更新遊戲狀態值
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void UpdateGameStateValue(const FString& Key, const FString& Value);

    /**
     * 獲取勝負統計
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossStatistics GetWinLossStatistics();

    /**
     * 重置統計數據
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void ResetStatistics();

    /**
     * 強制評估 (用於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult ForceEvaluation();

    /**
     * 獲取當前勝負結果
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static EWinLossResult GetCurrentResult();

    /**
     * 獲取勝負分數
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static float GetWinLossScore();

    /**
     * 檢查是否已結束
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static bool IsGameOver();

    /**
     * 獲取結束原因
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetGameOverReason();

    /**
     * 獲取結果名稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetResultName(EWinLossResult Result);

    /**
     * 獲取條件類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetConditionTypeName(EWinLossConditionType ConditionType);

    /**
     * 獲取優先級名稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetPriorityName(EWinLossPriority Priority);

    /**
     * 獲取狀態名稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetStateName(EWinLossConditionState State);

    /**
     * 創建勝負條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition CreateWinLossCondition(
        const FString& ConditionID,
        const FString& ConditionName,
        const FString& ConditionDescription,
        EWinLossConditionType ConditionType,
        EWinLossResult ResultType,
        EWinLossPriority Priority,
        bool bIsCriticalCondition
    );

    /**
     * 添加條件參數
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddConditionParameter(
        const FWinLossCondition& Condition,
        const FString& ParameterName,
        const FString& ParameterValue,
        const FString& ComparisonOperator,
        bool bIsRequired,
        float Weight
    );

    /**
     * 設置檢查間隔
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition SetCheckInterval(
        const FWinLossCondition& Condition,
        float CheckInterval
    );

    /**
     * 添加條件標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddConditionTag(
        const FWinLossCondition& Condition,
        const FString& Tag
    );

    /**
     * 添加相關事件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddRelatedEvent(
        const FWinLossCondition& Condition,
        const FString& EventID
    );

    /**
     * 批量檢查條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchCheckConditions(const TArray<FString>& ConditionIDs);

    /**
     * 批量註冊條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchRegisterConditions(const TArray<FWinLossCondition>& Conditions);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static bool IsWinLossSystemInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static FString GetWinLossSystemVersion();

    /**
     * 獲取註冊條件數量
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetRegisteredConditionCount();

    /**
     * 獲取滿足條件數量
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetSatisfiedConditionCount();

    /**
     * 獲取關鍵條件數量
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetCriticalConditionCount();

    /**
     * 獲取條件滿足率
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static float GetConditionSatisfactionRate();

    /**
     * 獲取勝負進度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetVictoryProgress();

    /**
     * 獲取失敗進度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDefeatProgress();

    /**
     * 獲取平局進度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDrawProgress();

    /**
     * 獲取距離勝利時間
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToVictory();

    /**
     * 獲取距離失敗時間
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToDefeat();

    /**
     * 獲取勝利概率
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetVictoryProbability();

    /**
     * 獲取失敗概率
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDefeatProbability();

    /**
     * 獲取平局概率
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDrawProbability();

    /**
     * 獲取勝負建議
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|AI")
    static TArray<FString> GetWinLossRecommendations();

    /**
     * 獲取需要關注的條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TArray<FString> GetCriticalConditionsToWatch();

    /**
     * 獲取條件影響分析
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetConditionImpactAnalysis();

    /**
     * 獲取遊戲狀態影響
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetGameStateImpact();

    /**
     * 獲取勝負趨勢
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static TArray<EWinLossResult> GetWinLossTrend(int32 HistoryCount);

    /**
     * 獲取趨勢分析
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static FString GetTrendAnalysis();

    /**
     * 模擬勝負結果
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<EWinLossResult> SimulateWinLoss(int32 SimulationCount);

    /**
     * 模擬條件變化
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<FString> SimulateConditionChanges(const FString& ConditionID, const FString& NewValue);

    /**
     * 導出勝負數據
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Export")
    static bool ExportWinLossData(const FString& FilePath);

    /**
     * 導入勝負數據
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Import")
    static bool ImportWinLossData(const FString& FilePath);

    /**
     * 獲取勝負報告
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateWinLossReport();

    /**
     * 獲取條件詳細報告
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateConditionReport(const FString& ConditionID);

    /**
     * 獲取統計報告
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateStatisticsReport();
};
