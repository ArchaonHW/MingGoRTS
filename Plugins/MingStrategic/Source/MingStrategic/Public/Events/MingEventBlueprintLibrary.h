#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalEventManager.h"
#include "MingEventBlueprintLibrary.generated.h"

/**
 * 事件?��X�數�? * ?��X��X�調?��?歷史事件系統?�能
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��?歷史事件管�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static UMingHistoricalEventManager* GetHistoricalEventManager(};

    /**
     * ?��X�歷?��?件系�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void InitializeHistoricalEventSystem(};

    /**
     * 註�?歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event};

    /**
     * ?��?註�?歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool UnregisterHistoricalEvent(const FString& EventID};

    /**
     * 檢查事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool CheckEventTriggerConditions(const FString& EventID};

    /**
     * 觸發歷史事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool TriggerHistoricalEvent(const FString& EventID};

    /**
     * ?��?歷史事件
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FMingHistoricalEvent GetHistoricalEvent(const FString& EventID};

    /**
     * ?��X�?�歷?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetAllHistoricalEvents(};

    /**
     * ?��X��X��X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByEra(ERepublicEra Era};

    /**
     * ?��X��?類�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByType(EHistoricalEventType EventType};

    /**
     * ?��X�鍵事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetCriticalEvents(};

    /**
     * ?��X�觸?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FString> GetTriggerableEvents(};

    /**
     * ?��?事件X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventChain(const FString& EventID};

    /**
     * 設置?�戲?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetGameTime(float GameTime};

    /**
     * ?��X�戲?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static float GetGameTime(};

    /**
     * 設置民�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetRepublicEra(ERepublicEra Era};

    /**
     * ?��X��?民�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static ERepublicEra GetCurrentRepublicEra(};

    /**
     * ?��X��?年份
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static int32 GetCurrentYear(};

    /**
     * 添�X�家決�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue};

    /**
     * ?��X�家決�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetPlayerDecision(const FString& DecisionID};

    /**
     * ?��?事件統�?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TMap<FString, int32> GetEventStatistics(};

    /**
     * ?�置?�?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void ResetAllEvents(};

    /**
     * 強制觸發事件 (?�於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool ForceTriggerEvent(const FString& EventID};

    /**
     * ?��X�薦事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetRecommendedEvents(};

    /**
     * ?��?事件?�述
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventDescription(const FString& EventID};

    /**
     * ?��X��X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEraName(ERepublicEra Era};

    /**
     * ?��?事件類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventTypeName(EHistoricalEventType EventType};

    /**
     * ?��?影響範�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetImpactScopeName(EEventImpactScope ImpactScope};

    /**
     * ?�建歷史事件
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
    };

    /**
     * 添�?事件觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddTriggerCondition(
        const FMingHistoricalEvent& Event,
        const FString& Condition
    };

    /**
     * 添�?事件後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddEventConsequence(
        const FMingHistoricalEvent& Event,
        const FString& Consequence
    };

    /**
     * 添�X��?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedFigure(
        const FMingHistoricalEvent& Event,
        const FString& FigureName
    };

    /**
     * 添�X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedLocation(
        const FMingHistoricalEvent& Event,
        const FString& LocationName
    };

    /**
     * 設置?�置事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetPrerequisiteEvent(
        const FMingHistoricalEvent& Event,
        const FString& PrerequisiteEventID
    };

    /**
     * 設置後�?事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetFollowUpEvent(
        const FMingHistoricalEvent& Event,
        const FString& FollowUpEventID
    };

    /**
     * ?��?觸發事件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchTriggerEvents(const TArray<FString>& EventIDs};

    /**
     * ?��?檢查事件條件
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchCheckEventConditions(const TArray<FString>& EventIDs};

    /**
     * ?��?系統?�X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static bool IsHistoricalEventSystemInitialized(};

    /**
     * ?��?系統?�本
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static FString GetHistoricalEventSystemVersion(};

    /**
     * ?��?已觸?��?件數X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetTriggeredEventCount(};

    /**
     * ?��?註�?事件?��?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetRegisteredEventCount(};

    /**
     * ?��?事件觸發X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static float GetEventTriggerRate(};

    /**
     * ?��?歷史準確?��X     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Analysis")
    static float GetHistoricalAccuracyScore(};

    /**
     * ?��?事件影響?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetEventImpactAnalysis(};

    /**
     * ?��X�家決�?影響
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetPlayerDecisionImpact(};

    /**
     * ?��X��X�度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetEraProgress(};

    /**
     * ?��?歷史?�度
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetHistoricalProgress(};

    /**
     * ?��?下�X��?要�?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static FMingHistoricalEvent GetNextMajorEvent(};

    /**
     * ?��X�能?��?件�X     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static TArray<FMingHistoricalEvent> GetPossibleEventBranches(const FString& EventID};

    /**
     * ?��?事件建議
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|AI")
    static TArray<FString> GetEventRecommendations(};

    /**
     * 模擬事件觸發
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Simulation")
    static TArray<FString> SimulateEventTriggers(float SimulationTime};

    /**
     * ?��?事件?��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Visualization")
    static TArray<FMingHistoricalEvent> GetEventTimeline(ERepublicEra Era};

    // ?��?事件?��X- 注�?：TMap<TArray> 不支XBlueprint
    static TMap<FString, TArray<FString>> GetEventRelationships(};

    /**
     * 導出事件?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Export")
    static bool ExportEventData(const FString& FilePath};

    /**
     * 導入事件?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Import")
    static bool ImportEventData(const FString& FilePath};
};

