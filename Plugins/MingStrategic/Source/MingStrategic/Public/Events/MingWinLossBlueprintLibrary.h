#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingWinLossCondition.h"
#include "MingWinLossBlueprintLibrary.generated.h"

/**
 * ?��?條件?��X�數�? * ?��X��X�調?��X��?條件系統?�能
 */
UCLASS()
class MINGSTRATEGIC_API UMingWinLossBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��X��?條件系統
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static UMingWinLossCondition* GetWinLossConditionSystem();

    /**
     * ?��X��?負�?件系�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void InitializeWinLossConditionSystem();

    /**
     * 註�X��?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool RegisterWinLossCondition(const FWinLossCondition& Condition};

    /**
     * ?��?註�X��?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool UnregisterWinLossCondition(const FString& ConditionID};

    /**
     * 評估?��X��?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult EvaluateWinLoss();

    /**
     * 檢查?�個�?�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static bool CheckCondition(const FString& ConditionID};

    /**
     * ?��X��?條件
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossCondition GetWinLossCondition(const FString& ConditionID};

    /**
     * ?��X�?��?負�?�?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetAllWinLossConditions();

    /**
     * ?��X��?類�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetConditionsByType(EWinLossConditionType ConditionType};

    /**
     * ?��X�鍵條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetCriticalConditions();

    /**
     * ?��?已滿足�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetSatisfiedConditions();

    /**
     * ?��X�滿足�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static TArray<FWinLossCondition> GetUnsatisfiedConditions();

    /**
     * 設置?�戲?�X     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void SetGameState(const TMap<FString, FString>& GameState};

    /**
     * ?��X�戲?�X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static TMap<FString, FString> GetGameState();

    /**
     * ?�新?�戲?�?��?     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void UpdateGameStateValue(const FString& Key, const FString& Value};

    /**
     * ?��X��?統�?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FWinLossStatistics GetWinLossStatistics();

    /**
     * ?�置統�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static void ResetStatistics();

    /**
     * 強制評估 (?�於測試)
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition")
    static FWinLossEvaluationResult ForceEvaluation();

    /**
     * ?��X��X��?結�?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static EWinLossResult GetCurrentResult();

    /**
     * ?��X��X�數
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static float GetWinLossScore();

    /**
     * 檢查?�否已�X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static bool IsGameOver();

    /**
     * ?��?結�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetGameOverReason();

    /**
     * ?��?結�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetResultName(EWinLossResult Result};

    /**
     * ?��?條件類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetConditionTypeName(EWinLossConditionType ConditionType};

    /**
     * ?��X��?級�?�?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetPriorityName(EWinLossPriority Priority};

    /**
     * ?��X�?��?�?     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition")
    static FString GetStateName(EWinLossConditionState State};

    /**
     * ?�建?��?條件
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
     * 添�?條件?�數
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
     * 設置檢查?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition SetCheckInterval(
        const FWinLossCondition& Condition,
        float CheckInterval
    };

    /**
     * 添�?條件標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddConditionTag(
        const FWinLossCondition& Condition,
        const FString& Tag
    };

    /**
     * 添�X��?事件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Creation")
    static FWinLossCondition AddRelatedEvent(
        const FWinLossCondition& Condition,
        const FString& EventID
    };

    /**
     * ?��?檢查條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchCheckConditions(const TArray<FString>& ConditionIDs};

    /**
     * ?��?註�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Batch")
    static TArray<bool> BatchRegisterConditions(const TArray<FWinLossCondition>& Conditions};

    /**
     * ?��?系統?�X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static bool IsWinLossSystemInitialized();

    /**
     * ?��?系統?�本
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static FString GetWinLossSystemVersion();

    /**
     * ?��?註�?條件?��?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetRegisteredConditionCount();

    /**
     * ?��?滿足條件?��?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetSatisfiedConditionCount();

    /**
     * ?��X�鍵條件?��?
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static int32 GetCriticalConditionCount();

    /**
     * ?��?條件滿足X     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|System")
    static float GetConditionSatisfactionRate();

    /**
     * ?��X��X�度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetVictoryProgress();

    /**
     * ?��?失�X�度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDefeatProgress();

    /**
     * ?��?平�X�度
     */
    UFUNCTION(BlueprintPure, Category = "Win Loss Condition|Progress")
    static float GetDrawProgress();

    /**
     * ?��?距離?�利?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToVictory();

    /**
     * ?��?距離失�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetTimeToDefeat();

    /**
     * ?��X�利概�?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetVictoryProbability();

    /**
     * ?��?失�?概�?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDefeatProbability();

    /**
     * ?��?平�?概�?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Prediction")
    static float GetDrawProbability();

    /**
     * ?��X��?建議
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|AI")
    static TArray<FString> GetWinLossRecommendations();

    /**
     * ?��X�要�?注�?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TArray<FString> GetCriticalConditionsToWatch();

    /**
     * ?��?條件影響?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetConditionImpactAnalysis();

    /**
     * ?��X�戲?�?�影X     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Analysis")
    static TMap<FString, float> GetGameStateImpact();

    /**
     * ?��X��?趨勢
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static TArray<EWinLossResult> GetWinLossTrend(int32 HistoryCount};

    /**
     * ?��?趨勢?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Trend")
    static FString GetTrendAnalysis();

    /**
     * 模擬?��?結�?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<EWinLossResult> SimulateWinLoss(int32 SimulationCount};

    /**
     * 模擬條件變�?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Simulation")
    static TArray<FString> SimulateConditionChanges(const FString& ConditionID, const FString& NewValue};

    /**
     * 導出?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Export")
    static bool ExportWinLossData(const FString& FilePath};

    /**
     * 導入?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Import")
    static bool ImportWinLossData(const FString& FilePath};

    /**
     * ?��X��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateWinLossReport();

    /**
     * ?��?條件詳細?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateConditionReport(const FString& ConditionID};

    /**
     * ?��?統�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Win Loss Condition|Report")
    static FString GenerateStatisticsReport();
};

