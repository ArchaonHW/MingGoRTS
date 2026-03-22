#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecision.h"
#include "MingDecisionManager.generated.h"

/**
 * 已�X��?決�?記�?
 */
USTRUCT(BlueprintType)
struct FMingDecisionRecord
{
    GENERATED_BODY()
    
    // 決�?ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionId;
    
    // ?��X�選?�ID
    UPROPERTY(BlueprintReadOnly)
    FString SelectedOptionId;
    
    // 決�X��?
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;
    
    // ?�戲?�日X
    UPROPERTY(BlueprintReadOnly)
    FString InGameDate;
    
    // 決�?標�?
    UPROPERTY(BlueprintReadOnly)
    FText DecisionTitle;
    
    // ?��X�選?��X
    UPROPERTY(BlueprintReadOnly)
    FText SelectedOptionTitle;
    
    FMingDecisionRecord()
        : DecisionTime(0.0f)
    {}
};




/**
 * 決�?管�X * 管�X�?�歷?�決策�?觸發?�執行�?記�?
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionManager();

    // ?��X
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Initialize();

    // ?��?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Shutdown();

    // 註�?決�?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void RegisterDecision(const FMingDecision& Decision);

    // ?��?觸發決�?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool TriggerDecision(const FString& DecisionId);

    // ?��?決�X��?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool ExecuteDecisionOption(const FString& DecisionId, const FString& OptionId);

    // ?��X��?活�?決�?
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecision>& GetActiveDecisions() const { return ActiveDecisions; }

    // ?��?決�?歷史
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecisionRecord>& GetDecisionHistory() const { return DecisionHistory; }

    // 檢查決�X�否已�X
    UFUNCTION(BlueprintPure, Category = "Decision")
    bool HasDecisionBeenMade(const FString& DecisionId) const;

    // ?��X��?決�X��X
    UFUNCTION(BlueprintPure, Category = "Decision")
    FMingDecisionRecord GetDecisionRecord(const FString& DecisionId) const;

    // ?�新?�戲?��? (?�於?��?觸發?�決�?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void UpdateGameTime(float CurrentGameTime);

    // 檢查條件觸發?�決�?
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void CheckConditionBasedDecisions();

    // 事件委�?
    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionTriggered OnDecisionTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionMade OnDecisionMade;

    // ?��X��?實�?
    static UMingDecisionManager* Get();

private:
    // ?�?�註?��?決�?
    UPROPERTY()
    TArray<FMingDecision> RegisteredDecisions;

    // ?��?活�X�決�?(等�X�家?��?)
    UPROPERTY()
    TArray<FMingDecision> ActiveDecisions;

    // 決�?歷史記�?
    UPROPERTY()
    TArray<FMingDecisionRecord> DecisionHistory;

    // 已�X��?決�?ID?��?
    UPROPERTY()
    TSet<FString> CompletedDecisionIds;

    // ?��X�戲?��?
    float CurrentGameTime;

    // ?�否已�?始�?
    bool bInitialized;

    // ?��?實�?
    static UMingDecisionManager* Instance;

    // ?�部?��?
    void ProcessTimeBasedDecisions();
    void ProcessEventBasedDecisions(const FString& EventName);
    bool CheckTriggerCondition(const FMingDecision& Decision) const;
    void ApplyDecisionEffects(const TArray<FMingDecisionEffect>& Effects);
    void RecordDecision(const FString& DecisionId, const FString& OptionId, const FText& DecisionTitle, const FText& OptionTitle);
    FMingDecision* FindRegisteredDecision(const FString& DecisionId);
    FMingDecision* FindActiveDecision(const FString& DecisionId);
);

