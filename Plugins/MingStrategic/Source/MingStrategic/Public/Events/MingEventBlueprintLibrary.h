#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalEventManager.h"
#include "MingEventBlueprintLibrary.generated.h"

/**
 * 事件藍圖函數庫
 * 提供藍圖可調用的歷史事件系統功能
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取歷史事件管理器
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static UMingHistoricalEventManager* GetHistoricalEventManager();

    /**
     * 初始化歷史事件系統
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void InitializeHistoricalEventSystem();

    /**
     * 註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    /**
     * 取消註冊歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool UnregisterHistoricalEvent(const FString& EventID);

    /**
     * 檢查事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool CheckEventTriggerConditions(const FString& EventID);

    /**
     * 觸發歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool TriggerHistoricalEvent(const FString& EventID);

    /**
     * 獲取歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FMingHistoricalEvent GetHistoricalEvent(const FString& EventID);

    /**
     * 獲取所有歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetAllHistoricalEvents();

    /**
     * 獲取指定時期的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByEra(ERepublicEra Era);

    /**
     * 獲取指定類型的事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByType(EHistoricalEventType EventType);

    /**
     * 獲取關鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetCriticalEvents();

    /**
     * 獲取可觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FString> GetTriggerableEvents();

    /**
     * 獲取事件鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventChain(const FString& EventID);

    /**
     * 設置遊戲時間
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetGameTime(float GameTime);

    /**
     * 獲取遊戲時間
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static float GetGameTime();

    /**
     * 設置民國時期
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetRepublicEra(ERepublicEra Era);

    /**
     * 獲取當前民國時期
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static ERepublicEra GetCurrentRepublicEra();

    /**
     * 獲取當前年份
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static int32 GetCurrentYear();

    /**
     * 添加玩家決策
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue);

    /**
     * 獲取玩家決策
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetPlayerDecision(const FString& DecisionID);

    /**
     * 獲取事件統計
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TMap<FString, int32> GetEventStatistics();

    /**
     * 重置所有事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void ResetAllEvents();

    /**
     * 強制觸發事件 (用於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool ForceTriggerEvent(const FString& EventID);

    /**
     * 獲取推薦事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetRecommendedEvents();

    /**
     * 獲取事件描述
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventDescription(const FString& EventID);

    /**
     * 獲取時期名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEraName(ERepublicEra Era);

    /**
     * 獲取事件類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventTypeName(EHistoricalEventType EventType);

    /**
     * 獲取影響範圍名稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetImpactScopeName(EEventImpactScope ImpactScope);

    /**
     * 創建歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent CreateHistoricalEvent(
        const FString& EventID,
        const FString& EventName,
        const FString& EventDescription,
        ERepublicEra Era,
        int32 HistoricalYear,
        EHistoricalEventType EventType,
        EEventImpactScope ImpactScope,
        bool bIsCriticalEvent
    );

    /**
     * 添加事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddTriggerCondition(
        const FMingHistoricalEvent& Event,
        const FString& Condition
    );

    /**
     * 添加事件後果
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddEventConsequence(
        const FMingHistoricalEvent& Event,
        const FString& Consequence
    );

    /**
     * 添加相關人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedFigure(
        const FMingHistoricalEvent& Event,
        const FString& FigureName
    );

    /**
     * 添加相關地點
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedLocation(
        const FMingHistoricalEvent& Event,
        const FString& LocationName
    );

    /**
     * 設置前置事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetPrerequisiteEvent(
        const FMingHistoricalEvent& Event,
        const FString& PrerequisiteEventID
    );

    /**
     * 設置後續事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetFollowUpEvent(
        const FMingHistoricalEvent& Event,
        const FString& FollowUpEventID
    );

    /**
     * 批量觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchTriggerEvents(const TArray<FString>& EventIDs);

    /**
     * 批量檢查事件條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchCheckEventConditions(const TArray<FString>& EventIDs);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static bool IsHistoricalEventSystemInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static FString GetHistoricalEventSystemVersion();

    /**
     * 獲取已觸發事件數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetTriggeredEventCount();

    /**
     * 獲取註冊事件數量
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetRegisteredEventCount();

    /**
     * 獲取事件觸發率
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static float GetEventTriggerRate();

    /**
     * 獲取歷史準確性評分
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Analysis")
    static float GetHistoricalAccuracyScore();

    /**
     * 獲取事件影響分析
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetEventImpactAnalysis();

    /**
     * 獲取玩家決策影響
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetPlayerDecisionImpact();

    /**
     * 獲取時期進度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetEraProgress();

    /**
     * 獲取歷史進度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetHistoricalProgress();

    /**
     * 獲取下一個重要事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static FMingHistoricalEvent GetNextMajorEvent();

    /**
     * 獲取可能的事件分支
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static TArray<FMingHistoricalEvent> GetPossibleEventBranches(const FString& EventID);

    /**
     * 獲取事件建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|AI")
    static TArray<FString> GetEventRecommendations();

    /**
     * 模擬事件觸發
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Simulation")
    static TArray<FString> SimulateEventTriggers(float SimulationTime);

    /**
     * 獲取事件時間線
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Visualization")
    static TArray<FMingHistoricalEvent> GetEventTimeline(ERepublicEra Era);

    // 獲取事件關係圖 - 注意：TMap<TArray> 不支持 Blueprint
    static TMap<FString, TArray<FString>> GetEventRelationships();

    /**
     * 導出事件數據
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Export")
    static bool ExportEventData(const FString& FilePath);

    /**
     * 導入事件數據
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Import")
    static bool ImportEventData(const FString& FilePath);
};
