#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Events/MingDecisionConsequenceCalculator.h"
#include "MingDecisionBlueprintLibrary.generated.h"

/**
 * Ê±∫Á??çÂ??ΩÊï∏Â∫? * ?ê‰??çÂ??ØË™ø?®Á?Ê±∫Á?Á≥ªÁµ±?üËÉΩ
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /**
     * ?≤Â?Ê±∫Á?ÂæåÊ?Ë®àÁ???     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static UMingDecisionConsequenceCalculator* GetDecisionConsequenceCalculator();

    /**
     * ?ùÂ??ñÊ±∫Á≠ñÂ??úË?ÁÆóÂô®
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static void InitializeDecisionConsequenceCalculator();

    /**
     * Ë®ªÂ?Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool RegisterDecisionConsequence(const FDecisionConsequence& Consequence);

    /**
     * ?ñÊ?Ë®ªÂ?Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool UnregisterDecisionConsequence(const FString& ConsequenceID);

    /**
     * Ë®àÁ?Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static FConsequenceCalculationResult CalculateDecisionConsequences(const FDecisionContext& Context);

    /**
     * ?≤Â?Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FDecisionConsequence GetDecisionConsequence(const FString& ConsequenceID);

    /**
     * ?≤Â??Ä?âÊ±∫Á≠ñÂ???     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetAllDecisionConsequences();

    /**
     * ?≤Â??áÂ?È°ûÂ??ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByType(EDecisionConsequenceType ConsequenceType);

    /**
     * ?≤Â??áÂ?ÂΩ±ÈüøÁØÑÂ??ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByScope(EConsequenceImpactScope ImpactScope);

    /**
     * ?≤Â??áÂ??¥È?Á®ãÂ∫¶?ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesBySeverity(EConsequenceSeverity Severity);

    /**
     * ?≤Â??áÂ?ÂΩ±ÈüøÈ°ûÂ??ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesByImpactType(EConsequenceImpactType ImpactType);

    /**
     * ?≤Â??∏È?Ê±∫Á??ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForDecision(const FString& DecisionID);

    /**
     * ?≤Â??∏È?‰∫∫Áâ©?ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForCharacter(const FString& CharacterID);

    /**
     * ?≤Â??∏È?‰∫ã‰ª∂?ÑÂ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> GetConsequencesForEvent(const FString& EventID);

    /**
     * Ëß∏ÁôºÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool TriggerConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?âÁî®ÂæåÊ??àÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ApplyConsequenceEffects(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?ÜË?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static bool ReverseConsequence(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?≤Â?ÂæåÊ?Áµ±Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static TMap<FString, int32> GetConsequenceStatistics();

    /**
     * ?≤Â?ÂæåÊ?ÂΩ±Èüø?ÜÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TMap<FString, float> GetConsequenceImpactAnalysis(const FString& DecisionID);

    /**
     * ?êÊ∏¨Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FDecisionConsequence> PredictDecisionConsequences(const FDecisionContext& Context, int32 PredictionCount);

    /**
     * Ê®°Êì¨Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence")
    static TArray<FConsequenceCalculationResult> SimulateDecisionConsequences(const FDecisionContext& Context, int32 SimulationCount);

    /**
     * ?≤Â?ÂæåÊ?È°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetConsequenceTypeName(EDecisionConsequenceType ConsequenceType);

    /**
     * ?≤Â?ÂΩ±ÈüøÁØÑÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactScopeName(EConsequenceImpactScope ImpactScope);

    /**
     * ?≤Â??¥È?Á®ãÂ∫¶?çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetSeverityName(EConsequenceSeverity Severity);

    /**
     * ?≤Â??ÅÁ??ÇÈ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetDurationName(EConsequenceDuration Duration);

    /**
     * ?≤Â?ÂΩ±ÈüøÈ°ûÂ??çÁ®±
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence")
    static FString GetImpactTypeName(EConsequenceImpactType ImpactType);

    /**
     * ?µÂª∫Ê±∫Á?‰∏ä‰???     */
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
     * Ë®≠ÁΩÆ?äÊà≤?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetGameState(const FDecisionContext& Context, const TMap<FString, FString>& GameState);

    /**
     * Ë®≠ÁΩÆ‰∫∫Áâ©?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetCharacterStates(const FDecisionContext& Context, const TMap<FString, FString>& CharacterStates);

    /**
     * Ë®≠ÁΩÆ‰∫ã‰ª∂?Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetEventStates(const FDecisionContext& Context, const TMap<FString, FString>& EventStates);

    /**
     * Ë®≠ÁΩÆË≥áÊ??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetResourceStates(const FDecisionContext& Context, const TMap<FString, int32>& ResourceStates);

    /**
     * Ë®≠ÁΩÆÂª∫Á??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetBuildingStates(const FDecisionContext& Context, const TMap<FString, FString>& BuildingStates);

    /**
     * Ë®≠ÁΩÆ?Æ‰??Ä??     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionContext SetUnitStates(const FDecisionContext& Context, const TMap<FString, FString>& UnitStates);

    /**
     * ?µÂª∫Ê±∫Á?ÂæåÊ?
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
     * Ê∑ªÂ?ÂæåÊ??àÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceEffect(const FDecisionConsequence& Consequence, const FString& Effect);

    /**
     * Ê∑ªÂ?ÂæåÊ??ÉÊï∏
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
     * Ê∑ªÂ?Ëß∏ÁôºÊ¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddTriggerCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * Ê∑ªÂ??∏È?‰∫∫Áâ©
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedCharacter(const FDecisionConsequence& Consequence, const FString& CharacterID);

    /**
     * Ê∑ªÂ??∏È?‰∫ã‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddRelatedEvent(const FDecisionConsequence& Consequence, const FString& EventID);

    /**
     * Ê∑ªÂ??ÜË?Ê¢ù‰ª∂
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddReversalCondition(const FDecisionConsequence& Consequence, const FString& Condition);

    /**
     * Ê∑ªÂ?ÂæåÊ?Ê®ôÁ±§
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Creation")
    static FDecisionConsequence AddConsequenceTag(const FDecisionConsequence& Consequence, const FString& Tag);

    /**
     * ?πÈ?Ë®àÁ?Ê±∫Á?ÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<FConsequenceCalculationResult> BatchCalculateConsequences(const TArray<FDecisionContext>& Contexts);

    /**
     * ?πÈ?Ëß∏ÁôºÂæåÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Batch")
    static TArray<bool> BatchTriggerConsequences(const TArray<FString>& ConsequenceIDs, const FDecisionContext& Context);

    /**
     * ?≤Â?Á≥ªÁµ±?Ä??     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static bool IsDecisionConsequenceCalculatorInitialized();

    /**
     * ?≤Â?Á≥ªÁµ±?àÊú¨
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static FString GetDecisionConsequenceCalculatorVersion();

    /**
     * ?≤Â?Ë®ªÂ?ÂæåÊ??∏È?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetRegisteredConsequenceCount();

    /**
     * ?≤Â?Â∑≤Ëß∏?ºÂ??úÊï∏??     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|System")
    static int32 GetTriggeredConsequenceCount();

    /**
     * ?≤Â?ÂæåÊ??èËø∞
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static FString GetConsequenceDescription(const FString& ConsequenceID);

    /**
     * Ê™¢Êü•ÂæåÊ??ØÂê¶Â∑≤Ëß∏??     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceTriggered(const FString& ConsequenceID);

    /**
     * Ê™¢Êü•ÂæåÊ??ØÂê¶?ØÈÄÜË?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static bool IsConsequenceReversible(const FString& ConsequenceID);

    /**
     * ?≤Â?ÂæåÊ?Ê¶ÇÁ?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceProbability(const FString& ConsequenceID);

    /**
     * ?≤Â?ÂæåÊ?Âª∂Ë?
     */
    UFUNCTION(BlueprintPure, Category = "Decision Consequence|Info")
    static float GetConsequenceDelay(const FString& ConsequenceID);

    /**
     * ?≤Â?ÂæåÊ?ÂΩ±Èüø?ÜÊï∏
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetConsequenceImpactScore(const FString& ConsequenceID, const FDecisionContext& Context);

    /**
     * ?≤Â?Ê±∫Á?Á∏ΩÈ?ÂΩ±Èüø
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionOverallImpact(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Ê≠?ù¢ÂΩ±Èüø
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionPositiveImpact(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Ë≤†Èù¢ÂΩ±Èüø
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionNegativeImpact(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?È¢®Èö™Ë©ï‰º∞
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Analysis")
    static float GetDecisionRiskAssessment(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Âª∫Ë≠∞
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionRecommendations(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Ë≠¶Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionWarnings(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Ê©üÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionOpportunities(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á??ø‰ª£?πÊ?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|AI")
    static TArray<FString> GetDecisionAlternatives(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<FConsequenceCalculationResult> GetDecisionHistory(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á??ÇÈ?Á∑?     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|History")
    static TArray<float> GetDecisionTimeline(const FString& DecisionID);

    // ?≤Â?Ê±∫Á?ÂΩ±ÈüøÁ∂≤Áµ° - Ê≥®Ê?ÔºöTMap<TArray> ‰∏çÊîØ??Blueprint
    static TMap<FString, TArray<FString>> GetDecisionImpactNetwork(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?‰æùË≥¥?ú‰?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Network")
    static TArray<FString> GetDecisionDependencies(const FString& DecisionID);

    /**
     * ?≤Â?Ê±∫Á?ÂæåÊ???     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceChain(const FString& ConsequenceID);

    /**
     * ?≤Â?Ê±∫Á?ÂæåÊ??ÜÊîØ
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Chain")
    static TArray<FDecisionConsequence> GetConsequenceBranches(const FString& ConsequenceID);

    /**
     * Â∞éÂá∫Ê±∫Á??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Export")
    static bool ExportDecisionData(const FString& FilePath);

    /**
     * Â∞éÂÖ•Ê±∫Á??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Import")
    static bool ImportDecisionData(const FString& FilePath);

    /**
     * ?≤Â?Ê±∫Á??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateDecisionReport(const FString& DecisionID);

    /**
     * ?≤Â?ÂæåÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateConsequenceReport(const FString& ConsequenceID);

    /**
     * ?≤Â?Áµ±Ë??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateStatisticsReport();

    /**
     * ?≤Â?ÂΩ±Èüø?ÜÊ??±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateImpactAnalysisReport(const FString& DecisionID);

    /**
     * ?≤Â?È¢®Èö™Ë©ï‰º∞?±Â?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|Report")
    static FString GenerateRiskAssessmentReport(const FString& DecisionID);

    /**
     * ?çÁΩÆÊ±∫Á?Á≥ªÁµ±
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ResetDecisionSystem();

    /**
     * Ê∏ÖÈô§Ê±∫Á?Ê≠∑Âè≤
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static void ClearDecisionHistory();

    /**
     * ?ô‰ªΩÊ±∫Á??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool BackupDecisionData(const FString& BackupPath);

    /**
     * ?¢Âæ©Ê±∫Á??∏Ê?
     */
    UFUNCTION(BlueprintCallable, Category = "Decision Consequence|System")
    static bool RestoreDecisionData(const FString& BackupPath);
};
