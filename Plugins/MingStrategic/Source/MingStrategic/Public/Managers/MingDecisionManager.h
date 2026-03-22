#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingDecision.h"
#include "MingDecisionManager.generated.h"

/**
 * 已做出的決策記錄
 */
USTRUCT(BlueprintType)
struct FMingDecisionRecord
{
    GENERATED_BODY()
    
    // 決策ID
    UPROPERTY(BlueprintReadOnly)
    FString DecisionId;
    
    // 選擇的選項ID
    UPROPERTY(BlueprintReadOnly)
    FString SelectedOptionId;
    
    // 決策時間
    UPROPERTY(BlueprintReadOnly)
    float DecisionTime;
    
    // 遊戲內日期
    UPROPERTY(BlueprintReadOnly)
    FString InGameDate;
    
    // 決策標題
    UPROPERTY(BlueprintReadOnly)
    FText DecisionTitle;
    
    // 選擇的選項文本
    UPROPERTY(BlueprintReadOnly)
    FText SelectedOptionTitle;
    
    FMingDecisionRecord()
        : DecisionTime(0.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionTriggered, const FMingDecision&, Decision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, const FString&, DecisionId, const FString&, SelectedOptionId);

/**
 * 決策管理器
 * 管理所有歷史決策的觸發、執行和記錄
 */
UCLASS()
class MINGSTRATEGIC_API UMingDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingDecisionManager();

    // 初始化
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Initialize();

    // 關閉
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void Shutdown();

    // 註冊決策
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void RegisterDecision(const FMingDecision& Decision);

    // 手動觸發決策
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool TriggerDecision(const FString& DecisionId);

    // 執行決策選項
    UFUNCTION(BlueprintCallable, Category = "Decision")
    bool ExecuteDecisionOption(const FString& DecisionId, const FString& OptionId);

    // 獲取當前活躍決策
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecision>& GetActiveDecisions() const { return ActiveDecisions; }

    // 獲取決策歷史
    UFUNCTION(BlueprintPure, Category = "Decision")
    const TArray<FMingDecisionRecord>& GetDecisionHistory() const { return DecisionHistory; }

    // 檢查決策是否已做過
    UFUNCTION(BlueprintPure, Category = "Decision")
    bool HasDecisionBeenMade(const FString& DecisionId) const;

    // 獲取特定決策的記錄
    UFUNCTION(BlueprintPure, Category = "Decision")
    FMingDecisionRecord GetDecisionRecord(const FString& DecisionId) const;

    // 更新遊戲時間 (用於時間觸發的決策)
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void UpdateGameTime(float CurrentGameTime);

    // 檢查條件觸發的決策
    UFUNCTION(BlueprintCallable, Category = "Decision")
    void CheckConditionBasedDecisions();

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionTriggered OnDecisionTriggered;

    UPROPERTY(BlueprintAssignable, Category = "Decision|Events")
    FOnDecisionMade OnDecisionMade;

    // 靜態獲取實例
    static UMingDecisionManager* Get();

private:
    // 所有註冊的決策
    UPROPERTY()
    TArray<FMingDecision> RegisteredDecisions;

    // 當前活躍的決策 (等待玩家選擇)
    UPROPERTY()
    TArray<FMingDecision> ActiveDecisions;

    // 決策歷史記錄
    UPROPERTY()
    TArray<FMingDecisionRecord> DecisionHistory;

    // 已完成的決策ID集合
    UPROPERTY()
    TSet<FString> CompletedDecisionIds;

    // 當前遊戲時間
    float CurrentGameTime;

    // 是否已初始化
    bool bInitialized;

    // 單例實例
    static UMingDecisionManager* Instance;

    // 內部方法
    void ProcessTimeBasedDecisions();
    void ProcessEventBasedDecisions(const FString& EventName);
    bool CheckTriggerCondition(const FMingDecision& Decision) const;
    void ApplyDecisionEffects(const TArray<FMingDecisionEffect>& Effects);
    void RecordDecision(const FString& DecisionId, const FString& OptionId, const FText& DecisionTitle, const FText& OptionTitle);
    FMingDecision* FindRegisteredDecision(const FString& DecisionId);
    FMingDecision* FindActiveDecision(const FString& DecisionId);
};
