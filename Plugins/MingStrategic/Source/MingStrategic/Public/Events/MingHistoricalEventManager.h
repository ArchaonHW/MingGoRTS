#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalEvents.h"
#include "Events/MingEventTrigger.h"
#include "MingHistoricalEventManager.generated.h"

/**
 * 民國歷史事件管理器
 * 管理所有民國時期的歷史事件和動態觸發
 */
UCLASS(BlueprintType)
class MINGSTRATEGIC_API UMingHistoricalEventManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalEventManager();

    /**
     * 初始化事件管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Initialize();

    /**
     * 關閉事件管理器
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void Shutdown();

    /**
     * 註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    /**
     * 取消註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool UnregisterHistoricalEvent(const FString& EventID);

    /**
     * 檢查事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    bool CheckEventTriggerConditions(const FString& EventID, const FEventTriggerContext& Context);

    /**
     * 觸發歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult TriggerHistoricalEvent(const FString& EventID, const FEventTriggerContext& Context);

    /**
     * 獲取歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FMingHistoricalEvent GetHistoricalEvent(const FString& EventID) const;

    /**
     * 獲取所有歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetAllHistoricalEvents() const;

    /**
     * 獲取指定時期的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventsByEra(ERepublicEra Era) const;

    /**
     * 獲取指定類型的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventsByType(EHistoricalEventType EventType) const;

    /**
     * 獲取關鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetCriticalEvents() const;

    /**
     * 獲取可觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FString> GetTriggerableEvents(const FEventTriggerContext& Context) const;

    /**
     * 獲取事件鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetEventChain(const FString& EventID) const;

    /**
     * 更新遊戲上下文
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void UpdateGameContext(const FEventTriggerContext& Context);

    /**
     * 獲取當前遊戲上下文
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    FEventTriggerContext GetCurrentGameContext() const;

    /**
     * 設置遊戲時間
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetGameTime(float GameTime);

    /**
     * 設置民國時期
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void SetRepublicEra(ERepublicEra Era);

    /**
     * 添加玩家決策
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue);

    /**
     * 獲取事件統計
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    TMap<FString, int32> GetEventStatistics() const;

    /**
     * 重置所有事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    void ResetAllEvents();

    /**
     * 強制觸發事件 (用於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    FEventExecutionResult ForceTriggerEvent(const FString& EventID);

    /**
     * 獲取推薦事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    TArray<FMingHistoricalEvent> GetRecommendedEvents(const FEventTriggerContext& Context) const;

protected:
    // 已註冊的歷史事件
    UPROPERTY()
    TArray<FMingHistoricalEvent> RegisteredEvents;

    // 事件ID到事件的映射
    UPROPERTY()
    TMap<FString, FMingHistoricalEvent> EventMap;

    // 當前遊戲上下文
    UPROPERTY()
    FEventTriggerContext CurrentContext;

    // 事件統計
    UPROPERTY()
    TMap<FString, int32> EventStatistics;

    // 已觸發的事件
    UPROPERTY()
    TArray<FString> TriggeredEvents;

    // 是否已初始化
    bool bIsInitialized;

    // 初始化民國歷史事件庫
    void InitializeHistoricalEventLibrary();

    // 創建早期共和國事件
    void CreateEarlyRepublicEvents();

    // 創建南京十年事件
    void CreateNanjingDecadeEvents();

    // 創建抗戰事件
    void CreateWarOfResistanceEvents();

    // 創建內戰事件
    void CreateCivilWarEvents();

    // 檢查時間條件
    bool CheckTimeCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查決策條件
    bool CheckDecisionCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查資源條件
    bool CheckResourceCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查建築條件
    bool CheckBuildingCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查軍事條件
    bool CheckMilitaryCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 檢查外交條件
    bool CheckDiplomaticCondition(const FString& Condition, const FEventTriggerContext& Context) const;

    // 執行事件後果
    bool ExecuteEventConsequence(const FString& Consequence, const FString& EventID);

    // 應用事件影響
    void ApplyEventImpact(const FMingHistoricalEvent& Event);

    // 更新事件統計
    void UpdateEventStatistics(const FString& EventID);

    // 驗證事件依賴
    bool ValidateEventDependencies(const FString& EventID) const;

    // 獲取事件影響分數
    float CalculateEventImpact(const FMingHistoricalEvent& Event) const;

    // 記錄事件日誌
    void LogEvent(const FString& EventID, const FString& Message);

    // 生成事件報告
    FString GenerateEventReport(const FString& EventID, const FEventExecutionResult& Result) const;

    // 解析條件字符串
    TArray<FString> ParseConditionString(const FString& Condition) const;

    // 比較數值
    bool CompareValues(float Value1, const FString& Operator, float Value2) const;

    // 獲取上下文值
    float GetContextValue(const FString& Key, const FEventTriggerContext& Context) const;

    // 設置上下文值
    void SetContextValue(const FString& Key, float Value, FEventTriggerContext& Context);
};
