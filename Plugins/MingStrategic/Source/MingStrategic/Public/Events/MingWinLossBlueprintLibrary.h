#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingWinLossCondition.h"
#include "MingWinLossBlueprintLibrary.generated.h"

/**
 * ?ùË?Ê¢ù‰ª∂?çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á??ùË?Ê¢ù‰ª∂Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGSTRATEGIC_API UMingWinLossBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â??ùË?Ê¢ù‰ª∂Á≥ªÁµ±
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static UMingWinLossCondition* GetWinLossConditionSystem(};

    /**
     * ?ùÂ??ñÂ?Ë≤†Ê?‰ª∂Á≥ªÁµ?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void InitializeWinLossConditionSystem(};

    /**
     * Ë®ªÂ??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool RegisterWinLossCondition(const FWinLossCondition& Condition};

    /**
     * ?ñÊ?Ë®ªÂ??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool UnregisterWinLossCondition(const FString& ConditionID};

    /**
     * Ë©ï‰º∞?ùË??ÄÊ≥?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult EvaluateWinLoss(};

    /**
     * Ê™¢Êü•?ÆÂÄãÊ?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool CheckCondition(const FString& ConditionID};

    /**
     * ?≤Â??ùË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossCondition GetWinLossCondition(const FString& ConditionID};

    /**
     * ?≤Â??Ä?âÂ?Ë≤†Ê?‰ª?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetAllWinLossConditions(};

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÊ?‰ª?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType};

    /**
     * ?≤Â??úÈçµÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetCriticalConditions(};

    /**
     * ?≤Â?Â∑≤ÊªøË∂≥Á?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetSatisfiedConditions(};

    /**
     * ?≤Â??™ÊªøË∂≥Á?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetUnsatisfiedConditions(};

    /**
     * Ë®≠ÁΩÆ?äÊà≤?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void SetGameState(const TMap<FString, FString>& GameState};

    /**
     * ?≤Â??äÊà≤?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TMap<FString, FString> GetGameState(};

    /**
     * ?¥Êñ∞?äÊà≤?Ä?ãÂÄ?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void UpdateGameStateValue(const FString& Key, const FString& Value};

    /**
     * ?≤Â??ùË?Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossStatistics GetWinLossStatistics(};

    /**
     * ?çÁΩÆÁµ±Ë??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void ResetStatistics(};

    /**
     * Âº∑Âà∂Ë©ï‰º∞ (?®ÊñºÊ∏¨Ë©¶)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult ForceEvaluation(};

    /**
     * ?≤Â??∂Â??ùË?ÁµêÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static EWinLossResult GetCurrentResult(};

    /**
     * ?≤Â??ùË??ÜÊï∏
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static float GetWinLossScore(};

    /**
     * Ê™¢Êü•?ØÂê¶Â∑≤Á???     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static bool IsGameOver(};

    /**
     * ?≤Â?ÁµêÊ??üÂ?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetGameOverReason(};

    /**
     * ?≤Â?ÁµêÊ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetResultName(EWinLossResult Result};

    /**
     * ?≤Â?Ê¢ù‰ª∂È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetConditionTypeName(EWinLossConditionType ConditionType};

    /**
     * ?≤Â??™Â?Á¥öÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetPriorityName(EWinLossPriority Priority};

    /**
     * ?≤Â??Ä?ãÂ?Á®?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetStateName(EWinLossConditionState State};

    /**
     * ?µÂª∫?ùË?Ê¢ù‰ª∂
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
    };

    /**
     * Ê∑ªÂ?Ê¢ù‰ª∂?ÉÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddConditionParameter(
        const FWinLossCondition& Condition,
        const FString& ParameterName,
        const FString& ParameterValue,
        const FString& ComparisonOperator,
        bool bIsRequired,
        float Weight
    };

    /**
     * Ë®≠ÁΩÆÊ™¢Êü•?ìÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition SetCheckInterval(
        const FWinLossCondition& Condition,
        float CheckInterval
    };

    /**
     * Ê∑ªÂ?Ê¢ù‰ª∂Ê®ôÁ±§
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddConditionTag(
        const FWinLossCondition& Condition,
        const FString& Tag
    };

    /**
     * Ê∑ªÂ??∏È?‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddRelatedEvent(
        const FWinLossCondition& Condition,
        const FString& EventID
    };

    /**
     * ?πÈ?Ê™¢Êü•Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchCheckConditions(const TArray<FString>& ConditionIDs};

    /**
     * ?πÈ?Ë®ªÂ?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchRegisterConditions(const TArray<FWinLossCondition>& Conditions};

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static bool IsWinLossSystemInitialized(};

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static FString GetWinLossSystemVersion(};

    /**
     * ?≤Â?Ë®ªÂ?Ê¢ù‰ª∂?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetRegisteredConditionCount(};

    /**
     * ?≤Â?ÊªøË∂≥Ê¢ù‰ª∂?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetSatisfiedConditionCount(};

    /**
     * ?≤Â??úÈçµÊ¢ù‰ª∂?∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetCriticalConditionCount(};

    /**
     * ?≤Â?Ê¢ù‰ª∂ÊªøË∂≥??     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static float GetConditionSatisfactionRate(};

    /**
     * ?≤Â??ùË??≤Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetVictoryProgress(};

    /**
     * ?≤Â?Â§±Ê??≤Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDefeatProgress(};

    /**
     * ?≤Â?Âπ≥Â??≤Â∫¶
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDrawProgress(};

    /**
     * ?≤Â?Ë∑ùÈõ¢?ùÂà©?ÇÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToVictory(};

    /**
     * ?≤Â?Ë∑ùÈõ¢Â§±Ê??ÇÈ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToDefeat(};

    /**
     * ?≤Â??ùÂà©Ê¶ÇÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetVictoryProbability(};

    /**
     * ?≤Â?Â§±Ê?Ê¶ÇÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDefeatProbability(};

    /**
     * ?≤Â?Âπ≥Â?Ê¶ÇÁ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDrawProbability(};

    /**
     * ?≤Â??ùË?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|AI")
    static TArray<FString> GetWinLossRecommendations(};

    /**
     * ?≤Â??ÄË¶ÅÈ?Ê≥®Á?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TArray<FString> GetCriticalConditionsToWatch(};

    /**
     * ?≤Â?Ê¢ù‰ª∂ÂΩ±Èüø?ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetConditionImpactAnalysis(};

    /**
     * ?≤Â??äÊà≤?Ä?ãÂΩ±??     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetGameStateImpact(};

    /**
     * ?≤Â??ùË?Ë∂®Âã¢
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static TArray<EWinLossResult> GetWinLossTrend(int32 HistoryCount};

    /**
     * ?≤Â?Ë∂®Âã¢?ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static FString GetTrendAnalysis(};

    /**
     * Ê®°Êì¨?ùË?ÁµêÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<EWinLossResult> SimulateWinLoss(int32 SimulationCount};

    /**
     * Ê®°Êì¨Ê¢ù‰ª∂ËÆäÂ?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<FString> SimulateConditionChanges(const FString& ConditionID, const FString& NewValue};

    /**
     * Â∞éÂá∫?ùË??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Export")
    static bool ExportWinLossData(const FString& FilePath};

    /**
     * Â∞éÂÖ•?ùË??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Import")
    static bool ImportWinLossData(const FString& FilePath};

    /**
     * ?≤Â??ùË??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateWinLossReport(};

    /**
     * ?≤Â?Ê¢ù‰ª∂Ë©≥Á¥∞?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateConditionReport(const FString& ConditionID};

    /**
     * ?≤Â?Áµ±Ë??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateStatisticsReport(};
};

