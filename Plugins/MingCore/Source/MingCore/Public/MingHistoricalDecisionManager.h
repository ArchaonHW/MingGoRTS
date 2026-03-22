#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalDecisionManager.generated.h"

// Ê±∫Á?È°ûÂ??öË?
UENUM(BlueprintType)
enum class EMingDecisionType : uint8
{
    Political,      // ?øÊ≤ªÊ±∫Á?
    Military,       // Ëªç‰?Ê±∫Á?
    Economic,       // Á∂ìÊ?Ê±∫Á?
    Diplomatic,     // Â§ñ‰∫§Ê±∫Á?
    Personal        // ?ã‰∫∫Ê±∫Á?
};

// Ê±∫Á??çË??ßÁ???UENUM(BlueprintType)
enum class EMingDecisionImportance : uint8
{
    Minor,          // Ê¨°Ë?
    Moderate,       // ‰∏≠Á?
    Major,          // ?çË?
    Critical        // ?úÈçµ
};

// Ê±∫Á??∏È?ÁµêÊ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionOption
{
    GENERATED_BODY()

    // ?∏È?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // ?∏È?Ê®ôÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // ?∏È??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // ?∏È?Ê¨äÈ? (ÂΩ±ÈüøÊ±∫Á?ÁµêÊ?)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OptionWeight;

    // ?∏È??çÁΩÆÊ¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // ?∏È?ÂæåÊ?ÂΩ±Èüø
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

// Ê≠∑Âè≤Ê±∫Á?ÁµêÊ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalDecision
{
    GENERATED_BODY()

    // Ê±∫Á?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // Ê±∫Á?Ê®ôÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionTitle;

    // Ê±∫Á??èËø∞
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // Ê≠∑Âè≤?åÊôØ‰ø°ÊÅØ
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalContext;

    // Ê±∫Á?È°ûÂ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionType DecisionType;

    // Ê±∫Á??çË???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionImportance Importance;

    // Ê±∫Á??ÇÈ?Èª?(Âπ¥‰ªΩ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;

    // Ê±∫Á??∏È??óË°®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionOption> Options;

    // Ê±∫Á?Ëß∏ÁôºÊ¢ù‰ª∂
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggerConditions;

    // ?ØÂê¶Â∑≤Á??öÂá∫Ê±∫Á?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDecisionMade;

    // ?©ÂÆ∂?∏Ê??ÑÈÅ∏?ÖID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // Ê±∫Á??ÇÈ???    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// Ê±∫Á?ÁµêÊ?ÁµêÊ?
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionResult
{
    GENERATED_BODY()

    // Ê±∫Á?ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // ?∏Ê??ÑÈÅ∏?ÖID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // Ê±∫Á??ÇÈ?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTime;

    // ÂΩ±Èüø?ÑÂ±¨??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> AffectedAttributes;

    // Ê≠∑Âè≤Ë∑ØÂ?ÂΩ±Èüø
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    // Ëß???ÑÊñ∞Ê±∫Á?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDecisions;

    // Ëß∏Áôº?ÑÂ?Á∫å‰?‰ª?    UPROPERTY(EditAnywhere, BlueprintReadWrite)
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

// Ê±∫Á?‰∫ã‰ª∂ÂßîË?
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDecisionPresented, int32, DecisionID, const FMingHistoricalDecision&, Decision, const TArray<FMingDecisionOption>&, AvailableOptions);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDecisionMade, int32, DecisionID, int32, ChosenOptionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDecisionCompleted, const FMingDecisionResult&, Result);

/**
 * Ê≠∑Âè≤Ê±∫Á?ÁÆ°Á??? * Ë≤†Ë≤¨ÁÆ°Á??äÊà≤‰∏≠Á?Ê≠∑Âè≤Ê±∫Á?Á≥ªÁµ±
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalDecisionManager();

    // ?ùÂ??ñÊ±∫Á≠ñÁ≥ªÁµ?    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool InitializeDecisionSystem();

    // Ë®ªÂ?Ê≠∑Âè≤Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool RegisterHistoricalDecision(const FMingHistoricalDecision& Decision);

    // Ê™¢Êü•Ê±∫Á?Ëß∏ÁôºÊ¢ù‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool CheckDecisionTriggerConditions(int32 DecisionID);

    // ?àÁèæÊ±∫Á?Áµ¶Áé©ÂÆ?    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool PresentDecisionToPlayer(int32 DecisionID);

    // ?ïÁ??©ÂÆ∂Ê±∫Á?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool ProcessPlayerDecision(int32 DecisionID, int32 ChosenOptionID);

    // ?≤Â?Ê±∫Á?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    FMingHistoricalDecision GetDecisionInfo(int32 DecisionID) const;

    // ?≤Â??ØÁî®Ê±∫Á??∏È?
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionOption> GetAvailableOptions(int32 DecisionID) const;

    // ?≤Â?Ê±∫Á?Ê≠∑Âè≤
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionResult> GetDecisionHistory() const;

    // ?≤Â??∂Â?Âπ¥‰ªΩ?ÑÂèØ?®Ê±∫Á≠?    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<int32> GetAvailableDecisionsForYear(int32 CurrentYear);

    // Ë®àÁ?Ê±∫Á?ÂΩ±Èüø
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    FMingDecisionResult CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // ‰øùÂ?Ê±∫Á??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool SaveDecisionData();

    // ËºâÂÖ•Ê±∫Á??∏Ê?
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool LoadDecisionData();

    // Ê∏ÖÈô§?Ä?âÊ±∫Á≠ñÊï∏??    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    void ClearAllDecisions();

    // ?≤Â?Ê±∫Á?Áµ±Ë?‰ø°ÊÅØ
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TMap<EMingDecisionType, int32> GetDecisionStatistics() const;

    // ‰∫ã‰ª∂ÂßîË?
    UPROPERTY(BlueprintAssignable)
    FOnDecisionPresented OnDecisionPresented;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionCompleted OnDecisionCompleted;

protected:
    // Ê±∫Á??∏Ê?Â∫?    UPROPERTY()
    TMap<int32, FMingHistoricalDecision> DecisionDatabase;

    // Ê±∫Á?Ê≠∑Âè≤Ë®òÈ?
    UPROPERTY()
    TArray<FMingDecisionResult> DecisionHistory;

    // ?∂Â?Ê¥ªË?Ê±∫Á?
    UPROPERTY()
    TArray<int32> ActiveDecisions;

    // ?©ÂÆ∂Â±¨ÊÄßÂΩ±??    UPROPERTY()
    TMap<FString, float> PlayerAttributes;

    // Ê≠∑Âè≤Ë∑ØÂ?ËøΩËπ§
    UPROPERTY()
    TArray<FString> HistoricalPath;

    // ?ØÂê¶Â∑≤Â?ÂßãÂ?
    UPROPERTY()
    bool bInitialized;

private:
    // ËºâÂÖ•?êË®≠Ê±∫Á??∏Ê?
    void LoadDefaultDecisions();

    // È©óË?Ê±∫Á??∏Ê?
    bool ValidateDecisionData(const FMingHistoricalDecision& Decision) const;

    // ?¥Êñ∞?©ÂÆ∂Â±¨ÊÄ?    void UpdatePlayerAttributes(const FMingDecisionResult& Result);

    // ?¥Êñ∞Ê≠∑Âè≤Ë∑ØÂ?
    void UpdateHistoricalPath(const FMingDecisionResult& Result);

    // Ëß???∞Ê±∫Á≠?    void UnlockNewDecisions(const FMingDecisionResult& Result);

    // Ëß∏ÁôºÂæåÁ?‰∫ã‰ª∂
    void TriggerFollowUpEvents(const FMingDecisionResult& Result);

    // Ë®òÈ?Ê±∫Á?ÁµêÊ?
    void RecordDecisionResult(const FMingDecisionResult& Result);
};
