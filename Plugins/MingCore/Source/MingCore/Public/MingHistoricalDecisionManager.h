#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalDecisionManager.generated.h"

// 決策類型枚舉
UENUM(BlueprintType)
enum class EMingDecisionType : uint8
{
    Political,      // 政治決策
    Military,       // 軍事決策
    Economic,       // 經濟決策
    Diplomatic,     // 外交決策
    Personal        // 個人決策
};

// 決策重要性級別
UENUM(BlueprintType)
enum class EMingDecisionImportance : uint8
{
    Minor,          // 次要
    Moderate,       // 中等
    Major,          // 重要
    Critical        // 關鍵
};

// 決策選項結構
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionOption
{
    GENERATED_BODY()

    // 選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // 選項標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // 選項描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // 選項權重 (影響決策結果)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OptionWeight;

    // 選項前置條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // 選項後果影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    FMingDecisionOption()
    {
        OptionID = -1;
        OptionTitle = TEXT("");
        OptionDescription = TEXT("");
        OptionWeight = 1.0f;
        Prerequisites.Empty();
        Consequences.Empty();
    }
};

// 歷史決策結構
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalDecision
{
    GENERATED_BODY()

    // 決策ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 決策標題
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionTitle;

    // 決策描述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // 歷史背景信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalContext;

    // 決策類型
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionType DecisionType;

    // 決策重要性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionImportance Importance;

    // 決策時間點 (年份)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;

    // 決策選項列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionOption> Options;

    // 決策觸發條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggerConditions;

    // 是否已經做出決策
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDecisionMade;

    // 玩家選擇的選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 決策時間戳
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTimestamp;

    FMingHistoricalDecision()
    {
        DecisionID = -1;
        DecisionTitle = TEXT("");
        DecisionDescription = TEXT("");
        HistoricalContext = TEXT("");
        DecisionType = EMingDecisionType::Political;
        Importance = EMingDecisionImportance::Moderate;
        Year = 1920;
        Options.Empty();
        TriggerConditions.Empty();
        bDecisionMade = false;
        ChosenOptionID = -1;
        DecisionTimestamp = FDateTime::Now();
    }
};

// 決策結果結構
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionResult
{
    GENERATED_BODY()

    // 決策ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 選擇的選項ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 決策時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTime;

    // 影響的屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> AffectedAttributes;

    // 歷史路徑影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    // 解鎖的新決策
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDecisions;

    // 觸發的後續事件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggeredEvents;

    FMingDecisionResult()
    {
        DecisionID = -1;
        ChosenOptionID = -1;
        DecisionTime = FDateTime::Now();
        AffectedAttributes.Empty();
        HistoricalPathChanges.Empty();
        UnlockedDecisions.Empty();
        TriggeredEvents.Empty();
    }
};

// 決策事件委託
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDecisionPresented, int32, DecisionID, const FMingHistoricalDecision&, Decision, const TArray<FMingDecisionOption>&, AvailableOptions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, int32, DecisionID, int32, ChosenOptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionCompleted, const FMingDecisionResult&, Result);

/**
 * 歷史決策管理器
 * 負責管理遊戲中的歷史決策系統
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalDecisionManager();

    // 初始化決策系統
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool InitializeDecisionSystem();

    // 註冊歷史決策
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool RegisterHistoricalDecision(const FMingHistoricalDecision& Decision);

    // 檢查決策觸發條件
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool CheckDecisionTriggerConditions(int32 DecisionID);

    // 呈現決策給玩家
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool PresentDecisionToPlayer(int32 DecisionID);

    // 處理玩家決策
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool ProcessPlayerDecision(int32 DecisionID, int32 ChosenOptionID);

    // 獲取決策信息
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    FMingHistoricalDecision GetDecisionInfo(int32 DecisionID) const;

    // 獲取可用決策選項
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionOption> GetAvailableOptions(int32 DecisionID) const;

    // 獲取決策歷史
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionResult> GetDecisionHistory() const;

    // 獲取當前年份的可用決策
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<int32> GetAvailableDecisionsForYear(int32 CurrentYear);

    // 計算決策影響
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    FMingDecisionResult CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // 保存決策數據
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool SaveDecisionData();

    // 載入決策數據
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool LoadDecisionData();

    // 清除所有決策數據
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    void ClearAllDecisions();

    // 獲取決策統計信息
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TMap<EMingDecisionType, int32> GetDecisionStatistics() const;

    // 事件委託
    UPROPERTY(BlueprintAssignable)
    FOnDecisionPresented OnDecisionPresented;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionCompleted OnDecisionCompleted;

protected:
    // 決策數據庫
    UPROPERTY()
    TMap<int32, FMingHistoricalDecision> DecisionDatabase;

    // 決策歷史記錄
    UPROPERTY()
    TArray<FMingDecisionResult> DecisionHistory;

    // 當前活躍決策
    UPROPERTY()
    TArray<int32> ActiveDecisions;

    // 玩家屬性影響
    UPROPERTY()
    TMap<FString, float> PlayerAttributes;

    // 歷史路徑追蹤
    UPROPERTY()
    TArray<FString> HistoricalPath;

    // 是否已初始化
    UPROPERTY()
    bool bInitialized;

private:
    // 載入預設決策數據
    void LoadDefaultDecisions();

    // 驗證決策數據
    bool ValidateDecisionData(const FMingHistoricalDecision& Decision) const;

    // 更新玩家屬性
    void UpdatePlayerAttributes(const FMingDecisionResult& Result);

    // 更新歷史路徑
    void UpdateHistoricalPath(const FMingDecisionResult& Result);

    // 解鎖新決策
    void UnlockNewDecisions(const FMingDecisionResult& Result);

    // 觸發後續事件
    void TriggerFollowUpEvents(const FMingDecisionResult& Result);

    // 記錄決策結果
    void RecordDecisionResult(const FMingDecisionResult& Result);
};
