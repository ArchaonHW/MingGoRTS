#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingDecisionConsequenceCalculator.h"
#include "MingDecisionBlueprintLibrary.generated.h"

/**
 * 決�X��X�數�? * ?��X��X�調?��?決�?系統?�能
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?��?決�?後�?計�X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static UMingDecisionConsequenceCalculator* GetDecisionConsequenceCalculator();

    /**
     * ?��X�決策�X��?算器
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static void InitializeDecisionConsequenceCalculator();

    /**
     * 註�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence);

    /**
     * ?��?註�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool UnregisterDecisionConsequence(const FString& ConsequenceID);

    /**
     * 計�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context);

    /**
     * ?��?決�?後�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID);

    /**
     * ?��X�?�決策�X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetAllDecisionConsequences();

    /**
     * ?��X��?類�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType);

    /**
     * ?��X��?影響範�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope);

    /**
     * ?��X��X��?程度?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity);

    /**
     * ?��X��?影響類�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType);

    /**
     * ?��X��?決�X��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID);

    /**
     * ?��X��?人物?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID);

    /**
     * ?��X��?事件?��X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID);

    /**
     * 觸發後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?�用後�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?��?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?��?後�?統�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TMap<FString, int32> GetConsequenceStatistics();

    /**
     * ?��?後�?影響?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID);

    /**
     * ?�測決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount);

    /**
     * 模擬決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount);

    /**
     * ?��?後�?類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetConsequenceTypeName(EDecisionConsequenceType ConsequenceType);

    /**
     * ?��?影響範�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactScopeName(EConsequenceImpactScope ImpactScope);

    /**
     * ?��X��?程度?�稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetSeverityName(EConsequenceSeverity Severity);

    /**
     * ?��X��X��X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetDurationName(EConsequenceDuration Duration);

    /**
     * ?��?影響類�X�稱
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactTypeName(EConsequenceImpactType ImpactType);

    /**
     * ?�建決�?上�X     */
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
     * 設置?�戲?�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetGameState(const FDecisionContext& Context, const TMap<FString, FString>& GameState);

    /**
     * 設置人物?�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetCharacterStates(const FDecisionContext& Context, const TMap<FString, FString>& CharacterStates);

    /**
     * 設置事件?�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetEventStates(const FDecisionContext& Context, const TMap<FString, FString>& EventStates);

    /**
     * 設置資�X�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetResourceStates(const FDecisionContext& Context, const TMap<FString, int32>& ResourceStates);

    /**
     * 設置建�X�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetBuildingStates(const FDecisionContext& Context, const TMap<FString, FString>& BuildingStates);

    /**
     * 設置?��X�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetUnitStates(const FDecisionContext& Context, const TMap<FString, FString>& UnitStates);

    /**
     * ?�建決�?後�?
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
     * 添�?後�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceEffect(const FDecisionConsequence& Consequence, const FString& Effect);

    /**
     * 添�?後�X�數
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
     * 添�?觸發條件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddTriggerCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * 添�X��?人物
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedCharacter(const FDecisionConsequence& Consequence, const FString& CharacterID);

    /**
     * 添�X��?事件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedEvent(const FDecisionConsequence& Consequence, const FString& EventID);

    /**
     * 添�X��?條件
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddReversalCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * 添�?後�?標籤
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceTag(const FDecisionConsequence& Consequence, const FString& Tag);

    /**
     * ?��?計�?決�?後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<FConsequenceCalculationResult> BatchCalculateConsequences(const TArray<FDecisionContext>& Contexts);

    /**
     * ?��?觸發後�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<bool> BatchTriggerConsequences(const TArray<FString>& ConsequenceIDs, const FDecisionContext& Context);

    /**
     * ?��?系統?�X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static bool IsDecisionConsequenceCalculatorInitialized();

    /**
     * ?��?系統?�本
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static FString GetDecisionConsequenceCalculatorVersion();

    /**
     * ?��?註�?後�X��?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetRegisteredConsequenceCount();

    /**
     * ?��?已觸?��X�數X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetTriggeredConsequenceCount();

    /**
     * ?��?後�X�述
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static FString GetConsequenceDescription(const FString& ConsequenceID);

    /**
     * 檢查後�X�否已觸X     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceTriggered(const FString& ConsequenceID);

    /**
     * 檢查後�X�否?�逆�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceReversible(const FString& ConsequenceID);

    /**
     * ?��?後�?概�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceProbability(const FString& ConsequenceID);

    /**
     * ?��?後�?延�?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceDelay(const FString& ConsequenceID);

    /**
     * ?��?後�?影響?�數
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetConsequenceImpactScore(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?��?決�?總�?影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionOverallImpact(const FString& DecisionID);

    /**
     * ?��?決�?�?��影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionPositiveImpact(const FString& DecisionID);

    /**
     * ?��?決�?負面影響
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionNegativeImpact(const FString& DecisionID);

    /**
     * ?��?決�?風險評估
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionRiskAssessment(const FString& DecisionID);

    /**
     * ?��?決�?建議
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionRecommendations(const FString& DecisionID);

    /**
     * ?��?決�?警�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionWarnings(const FString& DecisionID);

    /**
     * ?��?決�?機�?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionOpportunities(const FString& DecisionID);

    /**
     * ?��?決�X�代?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionAlternatives(const FString& DecisionID);

    /**
     * ?��?決�?歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<FConsequenceCalculationResult> GetDecisionHistory(const FString& DecisionID);

    /**
     * ?��?決�X��?�?     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<float> GetDecisionTimeline(const FString& DecisionID);

    // ?��?決�?影響網絡 - 注�?：TMap<TArray> 不支XBlueprint
    static TMap<FString, TArray<FString>> GetDecisionImpactNetwork(const FString& DecisionID);

    /**
     * ?��?決�?依賴?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Network")
    static TArray<FString> GetDecisionDependencies(const FString& DecisionID);

    /**
     * ?��?決�?後�X     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceChain(const FString& ConsequenceID);

    /**
     * ?��?決�?後�X�支
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceBranches(const FString& ConsequenceID);

    /**
     * 導出決�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Export")
    static bool ExportDecisionData(const FString& FilePath);

    /**
     * 導入決�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Import")
    static bool ImportDecisionData(const FString& FilePath);

    /**
     * ?��?決�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateDecisionReport(const FString& DecisionID);

    /**
     * ?��?後�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateConsequenceReport(const FString& ConsequenceID);

    /**
     * ?��?統�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateStatisticsReport();

    /**
     * ?��?影響?��X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateImpactAnalysisReport(const FString& DecisionID);

    /**
     * ?��?風險評估?��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateRiskAssessmentReport(const FString& DecisionID);

    /**
     * ?�置決�?系統
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ResetDecisionSystem();

    /**
     * 清除決�?歷史
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ClearDecisionHistory();

    /**
     * ?�份決�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool BackupDecisionData(const FString& BackupPath);

    /**
     * ?�復決�X��?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool RestoreDecisionData(const FString& BackupPath);
};

