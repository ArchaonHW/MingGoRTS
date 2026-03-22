#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingHistoricalDecisionManager.generated.h"

// �M??��?X???
UENUM(BlueprintType)
enum class EMingDecisionType: uint8 {
    Political,      // ??�v�M??
    Military,       // �x??�M??
    Economic,       // �g??�M??
    Diplomatic,     // �~��M??
    Personal        // ??�H�M??
};

// �M?X??X??XUENUM(BlueprintType)
enum class EMingDecisionImportance: uint8 {
    Minor,          // ��??
    Moderate,       // ��??
    Major,          // 摧毀
    Critical        // ??��
};

// �M?X???��??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionOption
{
    GENERATED_BODY()

    // 目標ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 OptionID;

    // 摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionTitle;

    // ???X?�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString OptionDescription;

    // 摧毀�v?? (�v�T�M??��??)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float OptionWeight;

    // ???X?�m����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> Prerequisites;

    // 摧毀��??�v�T
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> Consequences;

    FMingDecisionOption()
    {
        OptionID = -1;
        OptionTitle = TEXT(""};
        OptionDescription = TEXT("");
        OptionWeight = 1.0f;
        Prerequisites.Empty();
        Consequences.Empty();
    }
};

// ���v�M??��??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingHistoricalDecision
{
    GENERATED_BODY()

    // �M??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // �M??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionTitle;

    // �M?X?�z
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString DecisionDescription;

    // ���v??���H��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString HistoricalContext;

    // �M??��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingDecisionType DecisionType;

    
    EMingDecisionImportance Importance;

    // �M?X摧毀?(�~��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Year;

    // �M?X??X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FMingDecisionOption> Options;

    // �M??Ĳ�o����
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> TriggerConditions;

    // ??�_�w?X?�X�M??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bDecisionMade;

    // ??�a???X?��??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    
    FDateTime DecisionTimestamp;

    FMingHistoricalDecision()
    {
        DecisionID = -1;
        DecisionTitle = TEXT(""};
        DecisionDescription = TEXT(""};
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

// �M??��??��??
USTRUCT(BlueprintType)
struct FINGCORE_API FMingDecisionResult
{
    GENERATED_BODY()

    // �M??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 DecisionID;

    // ???X?��??ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ChosenOptionID;

    // �M?X???
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FDateTime DecisionTime;

    
    TMap<FString, float> AffectedAttributes;

    // ���v��??�v�T
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> HistoricalPathChanges;

    // ?X?�s�M??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> UnlockedDecisions;

    
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

// �M??�ƥ�e??




/**
 * ���v�M??��?X * �t�d��?X?����??���v�M??�t��
 */
UCLASS(ClassGroup = (Historical), Blueprintable, BlueprintType)
class MINGCORE_API UMingHistoricalDecisionManager : public UObject
{
    GENERATED_BODY()

public:
    UMingHistoricalDecisionManager();

    
    bool InitializeDecisionSystem();

    // ��??���v�M??
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool RegisterHistoricalDecision(const FMingHistoricalDecision& Decision) {};

    // �ˬd�M??Ĳ�o����
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool CheckDecisionTriggerConditions(int32 DecisionID);

    
    bool PresentDecisionToPlayer(int32 DecisionID);

    // ???X?�a�M??
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool ProcessPlayerDecision(int32 DecisionID, int32 ChosenOptionID);

    // 摧毀�M??�H��
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    FMingHistoricalDecision GetDecisionInfo(int32 DecisionID) const;

    // ???X?�ΨM?X???
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionOption> GetAvailableOptions(int32 DecisionID) const;

    // 摧毀�M??���v
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TArray<FMingDecisionResult> GetDecisionHistory() const;

    
    TArray<int32> GetAvailableDecisionsForYear(int32 CurrentYear);

    // �p??�M??�v�T
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    FMingDecisionResult CalculateDecisionImpact(int32 DecisionID, int32 ChosenOptionID);

    // �O??�M?X???
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool SaveDecisionData();

    // ���J�M?X???
    UFUNCTION(BlueprintCallable, Category = "Historical Decision")
    bool LoadDecisionData();

    
    void ClearAllDecisions();

    // 摧毀�M??��??�H��
    UFUNCTION(BlueprintPure, Category = "Historical Decision")
    TMap<EMingDecisionType, int32> GetDecisionStatistics() const;

    // �ƥ�e??
    UPROPERTY(BlueprintAssignable)
    FOnDecisionPresented OnDecisionPresented;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionMade OnDecisionMade;

    UPROPERTY(BlueprintAssignable)
    FOnDecisionCompleted OnDecisionCompleted;

protected:
    
    TMap<int32, FMingHistoricalDecision> DecisionDatabase;

    // �M??���v�O??
    UPROPERTY()
    TArray<FMingDecisionResult> DecisionHistory;

    // 摧毀��??�M??
    UPROPERTY()
    TArray<int32> ActiveDecisions;

    
    TMap<FString, float> PlayerAttributes;

    // ���v��??�l��
    UPROPERTY()
    TArray<FString> HistoricalPath;

    // ??�_�w??�l??
    UPROPERTY()
    bool bInitialized;

private:
    // ���J??�]�M?X???
    void LoadDefaultDecisions();

    // ��??�M?X???
    bool ValidateDecisionData(const FMingHistoricalDecision& Decision) const;

    // ??�s??�a��??
    void UpdatePlayerAttributes(const FMingDecisionResult& Result) {};

    // ??�s���v��??
    void UpdateHistoricalPath(const FMingDecisionResult& Result) {};

    // ?X?�M??
    void UnlockNewDecisions(const FMingDecisionResult& Result) {};

    // Ĳ�o��??�ƥ�
    void TriggerFollowUpEvents(const FMingDecisionResult& Result) {};

    // �O??�M??��??
    void RecordDecisionResult(const FMingDecisionResult& Result) {};
};

