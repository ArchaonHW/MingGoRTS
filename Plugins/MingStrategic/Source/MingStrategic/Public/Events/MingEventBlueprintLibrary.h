#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingHistoricalEventManager.h"
#include "MingEventBlueprintLibrary.generated.h"

/**
 * ‰∫ã‰ª∂?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á?Ê≠∑Âè≤‰∫ã‰ª∂Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGSTRATEGIC_API UMingEventBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?Ê≠∑Âè≤‰∫ã‰ª∂ÁÆ°Á???     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static UMingHistoricalEventManager* GetHistoricalEventManager();

    /**
     * ?ùÂ??ñÊ≠∑?≤‰?‰ª∂Á≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void InitializeHistoricalEventSystem();

    /**
     * Ë®ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool RegisterHistoricalEvent(const FMingHistoricalEvent& Event);

    /**
     * ?ñÊ?Ë®ªÂ?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool UnregisterHistoricalEvent(const FString& EventID);

    /**
     * Ê™¢Êü•‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool CheckEventTriggerConditions(const FString& EventID);

    /**
     * Ëß∏ÁôºÊ≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool TriggerHistoricalEvent(const FString& EventID);

    /**
     * ?≤Â?Ê≠∑Âè≤‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FMingHistoricalEvent GetHistoricalEvent(const FString& EventID);

    /**
     * ?≤Â??Ä?âÊ≠∑?≤‰?‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetAllHistoricalEvents();

    /**
     * ?≤Â??áÂ??ÇÊ??Ñ‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByEra(ERepublicEra Era);

    /**
     * ?≤Â??áÂ?È°ûÂ??Ñ‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventsByType(EHistoricalEventType EventType);

    /**
     * ?≤Â??úÈçµ‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetCriticalEvents();

    /**
     * ?≤Â??ØËß∏?º‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FString> GetTriggerableEvents();

    /**
     * ?≤Â?‰∫ã‰ª∂??     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetEventChain(const FString& EventID);

    /**
     * Ë®≠ÁΩÆ?äÊà≤?ÇÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetGameTime(float GameTime);

    /**
     * ?≤Â??äÊà≤?ÇÈ?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static float GetGameTime();

    /**
     * Ë®≠ÁΩÆÊ∞ëÂ??ÇÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void SetRepublicEra(ERepublicEra Era);

    /**
     * ?≤Â??∂Â?Ê∞ëÂ??ÇÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static ERepublicEra GetCurrentRepublicEra();

    /**
     * ?≤Â??∂Â?Âπ¥‰ªΩ
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static int32 GetCurrentYear();

    /**
     * Ê∑ªÂ??©ÂÆ∂Ê±∫Á?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void AddPlayerDecision(const FString& DecisionID, const FString& DecisionValue);

    /**
     * ?≤Â??©ÂÆ∂Ê±∫Á?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetPlayerDecision(const FString& DecisionID);

    /**
     * ?≤Â?‰∫ã‰ª∂Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static TMap<FString, int32> GetEventStatistics();

    /**
     * ?çÁΩÆ?Ä?â‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static void ResetAllEvents();

    /**
     * Âº∑Âà∂Ëß∏Áôº‰∫ã‰ª∂ (?®ÊñºÊ∏¨Ë©¶)
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static bool ForceTriggerEvent(const FString& EventID);

    /**
     * ?≤Â??®Ëñ¶‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events")
    static TArray<FMingHistoricalEvent> GetRecommendedEvents();

    /**
     * ?≤Â?‰∫ã‰ª∂?èËø∞
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventDescription(const FString& EventID);

    /**
     * ?≤Â??ÇÊ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEraName(ERepublicEra Era);

    /**
     * ?≤Â?‰∫ã‰ª∂È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetEventTypeName(EHistoricalEventType EventType);

    /**
     * ?≤Â?ÂΩ±ÈüøÁØÑÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events")
    static FString GetImpactScopeName(EEventImpactScope ImpactScope);

    /**
     * ?µÂª∫Ê≠∑Âè≤‰∫ã‰ª∂
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
     * Ê∑ªÂ?‰∫ã‰ª∂Ëß∏ÁôºÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddTriggerCondition(
        const FMingHistoricalEvent& Event,
        const FString& Condition
    );

    /**
     * Ê∑ªÂ?‰∫ã‰ª∂ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddEventConsequence(
        const FMingHistoricalEvent& Event,
        const FString& Consequence
    );

    /**
     * Ê∑ªÂ??∏È?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedFigure(
        const FMingHistoricalEvent& Event,
        const FString& FigureName
    );

    /**
     * Ê∑ªÂ??∏È??∞È?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent AddRelatedLocation(
        const FMingHistoricalEvent& Event,
        const FString& LocationName
    );

    /**
     * Ë®≠ÁΩÆ?çÁΩÆ‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetPrerequisiteEvent(
        const FMingHistoricalEvent& Event,
        const FString& PrerequisiteEventID
    );

    /**
     * Ë®≠ÁΩÆÂæåÁ?‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Creation")
    static FMingHistoricalEvent SetFollowUpEvent(
        const FMingHistoricalEvent& Event,
        const FString& FollowUpEventID
    );

    /**
     * ?πÈ?Ëß∏Áôº‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchTriggerEvents(const TArray<FString>& EventIDs);

    /**
     * ?πÈ?Ê™¢Êü•‰∫ã‰ª∂Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Batch")
    static TArray<bool> BatchCheckEventConditions(const TArray<FString>& EventIDs);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static bool IsHistoricalEventSystemInitialized();

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static FString GetHistoricalEventSystemVersion();

    /**
     * ?≤Â?Â∑≤Ëß∏?º‰?‰ª∂Êï∏??     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetTriggeredEventCount();

    /**
     * ?≤Â?Ë®ªÂ?‰∫ã‰ª∂?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static int32 GetRegisteredEventCount();

    /**
     * ?≤Â?‰∫ã‰ª∂Ëß∏Áôº??     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|System")
    static float GetEventTriggerRate();

    /**
     * ?≤Â?Ê≠∑Âè≤Ê∫ñÁ¢∫?ßË???     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Analysis")
    static float GetHistoricalAccuracyScore();

    /**
     * ?≤Â?‰∫ã‰ª∂ÂΩ±Èüø?ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetEventImpactAnalysis();

    /**
     * ?≤Â??©ÂÆ∂Ê±∫Á?ÂΩ±Èüø
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Analysis")
    static TMap<FString, float> GetPlayerDecisionImpact();

    /**
     * ?≤Â??ÇÊ??≤Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetEraProgress();

    /**
     * ?≤Â?Ê≠∑Âè≤?≤Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Historical Events|Progress")
    static float GetHistoricalProgress();

    /**
     * ?≤Â?‰∏ã‰??ãÈ?Ë¶Å‰?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static FMingHistoricalEvent GetNextMajorEvent();

    /**
     * ?≤Â??ØËÉΩ?Ñ‰?‰ª∂Â???     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Prediction")
    static TArray<FMingHistoricalEvent> GetPossibleEventBranches(const FString& EventID);

    /**
     * ?≤Â?‰∫ã‰ª∂Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|AI")
    static TArray<FString> GetEventRecommendations();

    /**
     * Ê®°Êì¨‰∫ã‰ª∂Ëß∏Áôº
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Simulation")
    static TArray<FString> SimulateEventTriggers(float SimulationTime);

    /**
     * ?≤Â?‰∫ã‰ª∂?ÇÈ?Á∑?     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Visualization")
    static TArray<FMingHistoricalEvent> GetEventTimeline(ERepublicEra Era);

    // ?≤Â?‰∫ã‰ª∂?ú‰???- Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??Blueprint
    static TMap<FString, TArray<FString>> GetEventRelationships();

    /**
     * Â∞éÂá∫‰∫ã‰ª∂?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Export")
    static bool ExportEventData(const FString& FilePath);

    /**
     * Â∞éÂÖ•‰∫ã‰ª∂?∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Historical Events|Import")
    static bool ImportEventData(const FString& FilePath);
};
