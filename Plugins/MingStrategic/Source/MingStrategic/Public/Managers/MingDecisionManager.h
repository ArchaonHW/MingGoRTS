#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecision.h"
#include "MingDecisionManager.generated.h"

/**
 * Â∑≤Â??∫Á?Ê±∫Á?Ë®òÈ?
 */
USTRUCT(BlueprintType)
struct FMingDecisionRecord
{
    GENERATED_BODY()
    
    // Ê±∫Á?ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionId;
    
    // ?∏Ê??ÑÈÅ∏?ÖID
    UPROPERTY(BlueprintReadOnly)
    FString SelectedOptionId;
    
    // Ê±∫Á??ÇÈ?
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;
    
    // ?äÊà≤?ßÊó•??    UPROPERTY(BlueprintReadOnly)
    FString InGameDate;
    
    // Ê±∫Á?Ê®ôÈ?
    UPROPERTY(BlueprintReadOnly)
    FText DecisionTitle;
    
    // ?∏Ê??ÑÈÅ∏?ÖÊ???    UPROPERTY(BlueprintReadOnly)
    FText SelectedOptionTitle;
    
    FMingDecisionRecord()
        : DecisionTime(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionTriggered, const FMingDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, const FString&, DecisionId, const FString&, SelectedOptionId);

/**
 * Ê±∫Á?ÁÆ°Á??? * ÁÆ°Á??Ä?âÊ≠∑?≤Ê±∫Á≠ñÁ?Ëß∏Áôº?ÅÂü∑Ë°åÂ?Ë®òÈ?
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionManager();

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Initialize();

    // ?úÈ?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Shutdown();

    // Ë®ªÂ?Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void RegisterDecision(const FMingDecision& Decision);

    // ?ãÂ?Ëß∏ÁôºÊ±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool TriggerDecision(const FString& DecisionId);

    // ?∑Ë?Ê±∫Á??∏È?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool ExecuteDecisionOption(const FString& DecisionId, const FString& OptionId);

    // ?≤Â??∂Â?Ê¥ªË?Ê±∫Á?
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecision>& GetActiveDecisions() const { return ActiveDecisions; }

    // ?≤Â?Ê±∫Á?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecisionRecord>& GetDecisionHistory() const { return DecisionHistory; }

    // Ê™¢Êü•Ê±∫Á??ØÂê¶Â∑≤Â???    UFUNCTION(BlueprintPure, Category = "Decision")
    bool HasDecisionBeenMade(const FString& DecisionId) const;

    // ?≤Â??πÂ?Ê±∫Á??ÑË???    UFUNCTION(BlueprintPure, Category = "Decision")
    FMingDecisionRecord GetDecisionRecord(const FString& DecisionId) const;

    // ?¥Êñ∞?äÊà≤?ÇÈ? (?®Êñº?ÇÈ?Ëß∏Áôº?ÑÊ±∫Á≠?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void UpdateGameTime(float CurrentGameTime);

    // Ê™¢Êü•Ê¢ù‰ª∂Ëß∏Áôº?ÑÊ±∫Á≠?    UFUNCTION(BlueprintCallable, Category = "Decision")
    void CheckConditionBasedDecisions();

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionTriggered OnDecisionTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionMade OnDecisionMade;

    // ?úÊ??≤Â?ÂØ¶‰?
    static UMingDecisionManager* Get();

private:
    // ?Ä?âË®ª?äÁ?Ê±∫Á?
    UPROPERTY()
    TArray<FMingDecision> RegisteredDecisions;

    // ?∂Â?Ê¥ªË??ÑÊ±∫Á≠?(Á≠âÂ??©ÂÆ∂?∏Ê?)
    UPROPERTY()
    TArray<FMingDecision> ActiveDecisions;

    // Ê±∫Á?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FMingDecisionRecord> DecisionHistory;

    // Â∑≤Â??êÁ?Ê±∫Á?ID?ÜÂ?
    UPROPERTY()
    TSet<FString> CompletedDecisionIds;

    // ?∂Â??äÊà≤?ÇÈ?
    float CurrentGameTime;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    bool bInitialized;

    // ?Æ‰?ÂØ¶‰?
    static UMingDecisionManager* Instance;

    // ?ßÈÉ®?πÊ?
    void ProcessTimeBasedDecisions();
    void ProcessEventBasedDecisions(const FString& EventName);
    bool CheckTriggerCondition(const FMingDecision& Decision) const;
    void ApplyDecisionEffects(const TArray<FMingDecisionEffect>& Effects);
    void RecordDecision(const FString& DecisionId, const FString& OptionId, const FText& DecisionTitle, const FText& OptionTitle);
    FMingDecision* FindRegisteredDecision(const FString& DecisionId);
    FMingDecision* FindActiveDecision(const FString& DecisionId);
};
