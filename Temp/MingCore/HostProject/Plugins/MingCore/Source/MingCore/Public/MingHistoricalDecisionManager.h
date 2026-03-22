#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalDecisionManager.generated.h"

// 決�?類�X��?
UENUM(BlueprintType)
enum class EMingDecisionType : uint8
{
    Political,      // ?�治決�?
    Military,       // 軍�?決�?
    Economic,       // 經�?決�?
    Diplomatic,     // 外交決�?
    Personal        // ?�人決�?
};

// 決�X��X��XUENUM(BlueprintType)
enum class EMingDecisionImportance : uint8
{
    Minor,          // 次�?
    Moderate,       // 中�?
    Major,          // ?��?
    Critical        // ?�鍵
};

// 決�X��?結�?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionOption
{
    GENERATED_BODY()

    // ?��?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // ?��?標�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // ?��X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // ?��?權�? (影響決�?結�?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OptionWeight;

    // ?��X�置條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // ?��?後�?影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    FMingDecisionOption()
    {
        OptionID = -1;
        OptionTitle = TEXT(""};
        OptionDescription = TEXT(""};
        OptionWeight = 1.0f;
        Prerequisites.Empty(};
        Consequences.Empty(};
    }
};

// 歷史決�?結�?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalDecision
{
    GENERATED_BODY()

    // 決�?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // 決�?標�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionTitle;

    // 決�X�述
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // 歷史?�景信息
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalContext;

    // 決�?類�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionType DecisionType;

    // 決�X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionImportance Importance;

    // 決�X��?�?(年份)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;

    // 決�X��X�表
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionOption> Options;

    // 決�?觸發條件
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggerConditions;

    // ?�否已�X�出決�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDecisionMade;

    // ?�家?��X�選?�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 決�X��X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTimestamp;

    FMingHistoricalDecision()
    {
        DecisionID = -1;
        DecisionTitle = TEXT(""};
        DecisionDescription = TEXT(""};
        HistoricalContext = TEXT(""};
        DecisionType = EMingDecisionType::Political;
        Importance = EMingDecisionImportance::Moderate;
        Year = 1920;
        Options.Empty(};
        TriggerConditions.Empty(};
        bDecisionMade = false;
        ChosenOptionID = -1;
        DecisionTimestamp = FDateTime::Now(};
    }
};

// 決�?結�?結�?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionResult
{
    GENERATED_BODY()

    // 決�?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // ?��X�選?�ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // 決�X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTime;

    // 影響?�屬X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> AffectedAttributes;

    // 歷史路�?影響
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    // �X�新決�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDecisions;

    // 觸發?��?續�?�?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggeredEvents;

    FMingDecisionResult()
    {
        DecisionID = -1;
        ChosenOptionID = -1;
        DecisionTime = FDateTime::Now(};
        AffectedAttributes.Empty(};
        HistoricalPathChanges.Empty(};
        UnlockedDecisions.Empty(};
        TriggeredEvents.Empty(};
    }
};

// 決�?事件委�?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDecisionPresented, int32, DecisionID, const FMingHistoricalDecision&, Decision, const TArray<FMingDecisionOption>&, AvailableOptions};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, int32, DecisionID, int32, ChosenOptionID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionCompleted, const FMingDecisionResult&, Result};

/**
 * 歷史決�?管�X * 負責管�X�戲中�?歷史決�?系統
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalDecisionManager(};

    // ?��X�決策系�?    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool InitializeDecisionSystem(};

    // 註�?歷史決�?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool RegisterHistoricalDecision(const FMingHistoricalDecision& Decision) {};

    // 檢查決�?觸發條件
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool CheckDecisionTriggerConditions(int32 DecisionID};

    // ?�現決�?給玩�?    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool PresentDecisionToPlayer(int32 DecisionID};

    // ?��X�家決�?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool ProcessPlayerDecision(int32 DecisionID, int32 ChosenOptionID};

    // ?��?決�?信息
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    FMingHistoricalDecision GetDecisionInfo(int32 DecisionID) const;

    // ?��X�用決�X��?
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionOption> GetAvailableOptions(int32 DecisionID) const;

    // ?��?決�?歷史
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionResult> GetDecisionHistory() const;

    // ?��X��?年份?�可?�決�?    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<int32> GetAvailableDecisionsForYear(int32 CurrentYear};

    // 計�?決�?影響
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    FMingDecisionResult CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID};

    // 保�?決�X��?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool SaveDecisionData(};

    // 載入決�X��?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool LoadDecisionData(};

    // 清除?�?�決策數X    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    void ClearAllDecisions(};

    // ?��?決�?統�?信息
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TMap<EMingDecisionType, int32> GetDecisionStatistics() const;

    // 事件委�?
    UPROPERTY(BlueprintAssignable)
    FOnDecisionPresented OnDecisionPresented;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionCompleted OnDecisionCompleted;

protected:
    // 決�X��?�?    UPROPERTY()
    TMap<int32, FMingHistoricalDecision> DecisionDatabase;

    // 決�?歷史記�?
    UPROPERTY()
    TArray<FMingDecisionResult> DecisionHistory;

    // ?��?活�?決�?
    UPROPERTY()
    TArray<int32> ActiveDecisions;

    // ?�家屬性影X    UPROPERTY()
    TMap<FString, float> PlayerAttributes;

    // 歷史路�?追蹤
    UPROPERTY()
    TArray<FString> HistoricalPath;

    // ?�否已�?始�?
    UPROPERTY()
    bool bInitialized;

private:
    // 載入?�設決�X��?
    void LoadDefaultDecisions(};

    // 驗�?決�X��?
    bool ValidateDecisionData(const FMingHistoricalDecision& Decision) const;

    // ?�新?�家屬�?    void UpdatePlayerAttributes(const FMingDecisionResult& Result) {};

    // ?�新歷史路�?
    void UpdateHistoricalPath(const FMingDecisionResult& Result) {};

    // �X�決�?    void UnlockNewDecisions(const FMingDecisionResult& Result) {};

    // 觸發後�?事件
    void TriggerFollowUpEvents(const FMingDecisionResult& Result) {};

    // 記�?決�?結�?
    void RecordDecisionResult(const FMingDecisionResult& Result) {};
};

