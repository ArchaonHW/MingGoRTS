#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingDecisionConsequenceCalculator.h"
#include "MingDecisionBlueprintLibrary.generated.h"

/**
 * 決策藍圖函數庫
 * 提供藍圖可調用的決策系統功能
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * 獲取決策後果計算器
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static UMingDecisionConsequenceCalculator* GetDecisionConsequenceCalculator();

    /**
     * 初始化決策後果計算器
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static void InitializeDecisionConsequenceCalculator();

    /**
     * 註冊決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence);

    /**
     * 取消註冊決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool UnregisterDecisionConsequence(const FString& ConsequenceID);

    /**
     * 計算決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context);

    /**
     * 獲取決策後果
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID);

    /**
     * 獲取所有決策後果
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetAllDecisionConsequences();

    /**
     * 獲取指定類型的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType);

    /**
     * 獲取指定影響範圍的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope);

    /**
     * 獲取指定嚴重程度的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity);

    /**
     * 獲取指定影響類型的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType);

    /**
     * 獲取相關決策的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID);

    /**
     * 獲取相關人物的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID);

    /**
     * 獲取相關事件的後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID);

    /**
     * 觸發後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 應用後果效果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 逆轉後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 獲取後果統計
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TMap<FString, int32> GetConsequenceStatistics();

    /**
     * 獲取後果影響分析
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID);

    /**
     * 預測決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount);

    /**
     * 模擬決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount);

    /**
     * 獲取後果類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetConsequenceTypeName(EDecisionConsequenceType ConsequenceType);

    /**
     * 獲取影響範圍名稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactScopeName(EConsequenceImpactScope ImpactScope);

    /**
     * 獲取嚴重程度名稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetSeverityName(EConsequenceSeverity Severity);

    /**
     * 獲取持續時間名稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetDurationName(EConsequenceDuration Duration);

    /**
     * 獲取影響類型名稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactTypeName(EConsequenceImpactType ImpactType);

    /**
     * 創建決策上下文
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext CreateDecisionContext(
        const FString& DecisionID,
        const FString& DecisionType,
        const FString& DecisionOption,
        const FString& DecisionMakerID,
        const FString& RepublicEra,
        int32 CurrentYear,
        const FString& PlayerFaction,
        int32 DifficultyLevel
    );

    /**
     * 設置遊戲狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetGameState(const FDecisionContext& Context, const TMap<FString, FString>& GameState);

    /**
     * 設置人物狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetCharacterStates(const FDecisionContext& Context, const TMap<FString, FString>& CharacterStates);

    /**
     * 設置事件狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetEventStates(const FDecisionContext& Context, const TMap<FString, FString>& EventStates);

    /**
     * 設置資源狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetResourceStates(const FDecisionContext& Context, const TMap<FString, int32>& ResourceStates);

    /**
     * 設置建築狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetBuildingStates(const FDecisionContext& Context, const TMap<FString, FString>& BuildingStates);

    /**
     * 設置單位狀態
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetUnitStates(const FDecisionContext& Context, const TMap<FString, FString>& UnitStates);

    /**
     * 創建決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence CreateDecisionConsequence(
        const FString& ConsequenceID,
        const FString& ConsequenceName,
        const FString& ConsequenceDescription,
        EDecisionConsequenceType ConsequenceType,
        EConsequenceImpactScope ImpactScope,
        EConsequenceSeverity Severity,
        EConsequenceDuration Duration,
        EConsequenceImpactType ImpactType,
        const FString& RelatedDecisionID,
        float Probability,
        float DelayTime,
        bool bIsReversible
    );

    /**
     * 添加後果效果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceEffect(const FDecisionConsequence& Consequence, const FString& Effect);

    /**
     * 添加後果參數
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceParameter(
        const FDecisionConsequence& Consequence,
        const FString& ParameterName,
        const FString& ParameterValue,
        const FString& ParameterType,
        bool bIsRequired,
        const FString& DefaultValue,
        const FString& Description
    );

    /**
     * 添加觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddTriggerCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * 添加相關人物
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedCharacter(const FDecisionConsequence& Consequence, const FString& CharacterID);

    /**
     * 添加相關事件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedEvent(const FDecisionConsequence& Consequence, const FString& EventID);

    /**
     * 添加逆轉條件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddReversalCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * 添加後果標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceTag(const FDecisionConsequence& Consequence, const FString& Tag);

    /**
     * 批量計算決策後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<FConsequenceCalculationResult> BatchCalculateConsequences(const TArray<FDecisionContext>& Contexts);

    /**
     * 批量觸發後果
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<bool> BatchTriggerConsequences(const TArray<FString>& ConsequenceIDs, const FDecisionContext& Context);

    /**
     * 獲取系統狀態
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static bool IsDecisionConsequenceCalculatorInitialized();

    /**
     * 獲取系統版本
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static FString GetDecisionConsequenceCalculatorVersion();

    /**
     * 獲取註冊後果數量
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetRegisteredConsequenceCount();

    /**
     * 獲取已觸發後果數量
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetTriggeredConsequenceCount();

    /**
     * 獲取後果描述
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static FString GetConsequenceDescription(const FString& ConsequenceID);

    /**
     * 檢查後果是否已觸發
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceTriggered(const FString& ConsequenceID);

    /**
     * 檢查後果是否可逆轉
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceReversible(const FString& ConsequenceID);

    /**
     * 獲取後果概率
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceProbability(const FString& ConsequenceID);

    /**
     * 獲取後果延迟
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceDelay(const FString& ConsequenceID);

    /**
     * 獲取後果影響分數
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetConsequenceImpactScore(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * 獲取決策總體影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionOverallImpact(const FString& DecisionID);

    /**
     * 獲取決策正面影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionPositiveImpact(const FString& DecisionID);

    /**
     * 獲取決策負面影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionNegativeImpact(const FString& DecisionID);

    /**
     * 獲取決策風險評估
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionRiskAssessment(const FString& DecisionID);

    /**
     * 獲取決策建議
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionRecommendations(const FString& DecisionID);

    /**
     * 獲取決策警告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionWarnings(const FString& DecisionID);

    /**
     * 獲取決策機會
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionOpportunities(const FString& DecisionID);

    /**
     * 獲取決策替代方案
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionAlternatives(const FString& DecisionID);

    /**
     * 獲取決策歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<FConsequenceCalculationResult> GetDecisionHistory(const FString& DecisionID);

    /**
     * 獲取決策時間線
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<float> GetDecisionTimeline(const FString& DecisionID);

    /**
     * 獲取決策影響網絡
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Network")
    static TMap<FString, TArray<FString>> GetDecisionImpactNetwork(const FString& DecisionID);

    /**
     * 獲取決策依賴關係
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Network")
    static TArray<FString> GetDecisionDependencies(const FString& DecisionID);

    /**
     * 獲取決策後果鏈
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceChain(const FString& ConsequenceID);

    /**
     * 獲取決策後果分支
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceBranches(const FString& ConsequenceID);

    /**
     * 導出決策數據
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Export")
    static bool ExportDecisionData(const FString& FilePath);

    /**
     * 導入決策數據
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Import")
    static bool ImportDecisionData(const FString& FilePath);

    /**
     * 獲取決策報告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateDecisionReport(const FString& DecisionID);

    /**
     * 獲取後果報告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateConsequenceReport(const FString& ConsequenceID);

    /**
     * 獲取統計報告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateStatisticsReport();

    /**
     * 獲取影響分析報告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateImpactAnalysisReport(const FString& DecisionID);

    /**
     * 獲取風險評估報告
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateRiskAssessmentReport(const FString& DecisionID);

    /**
     * 重置決策系統
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ResetDecisionSystem();

    /**
     * 清除決策歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ClearDecisionHistory();

    /**
     * 備份決策數據
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool BackupDecisionData(const FString& BackupPath);

    /**
     * 恢復決策數據
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool RestoreDecisionData(const FString& BackupPath);
};
