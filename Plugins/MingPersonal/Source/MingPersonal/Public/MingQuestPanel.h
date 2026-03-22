#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingQuestPanel.generated.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
    Available,      // ??接X    Active,         // ??????    Completed,      // 已?X    Failed,         // 失??
    Abandoned       // ????
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
    Main,           // 主??任??
    Side,           // ????任??
    Daily,          // 每日任??
    Weekly,         // 每週任X    Special,        // ????任??
    Relationship,    // ????任??
    Reputation,      // ????任??
    Exploration      // ??索任??
};

USTRUCT(BlueprintType)
struct FQuestRequirement
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Quest Requirement")
    FString RequirementText;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Requirement")
    bool bIsCompleted;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Requirement")
    float Progress;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Requirement")
    float TargetValue;

    FQuestRequirement()
    {
        RequirementText = TEXT("");
        bIsCompleted = false;
        Progress = 0.0f;
        TargetValue = 1.0f;
    }
};

USTRUCT(BlueprintType)
struct FQuestReward
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Quest Reward")
    FString RewardType;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Reward")
    int32 Amount;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Reward")
    FString Description;

    FQuestReward()
    {
        RewardType = TEXT("");
        Amount = 0;
        Description = TEXT("");
    }
};

USTRUCT(BlueprintType)
struct FQuestData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FString QuestID;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FString QuestTitle;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FString QuestDescription;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    EQuestType QuestType;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    EQuestStatus QuestStatus;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FString RegionID;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FString QuestGiverID;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    int32 SuggestedLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    float Difficulty;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestRequirement> Requirements;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestReward> Rewards;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    float TimeLimit;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    bool bHasTimeLimit;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FDateTime EndTime;

    FQuestData()
    {
        QuestID = TEXT("");
        QuestTitle = TEXT("");
        QuestDescription = TEXT("");
        QuestType = EQuestType::Side;
        QuestStatus = EQuestStatus::Available;
        RegionID = TEXT("");
        QuestGiverID = TEXT("");
        SuggestedLevel = 1;
        Difficulty = 1.0f;
        TimeLimit = 0.0f;
        bHasTimeLimit = false;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, const FString&, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, const FString&, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed, const FString&, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestSelected, const FString&, QuestID, EQuestStatus, Status);

/**
 * 任?X?板Widget
 * 管??任?X?面
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingQuestPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingQuestPanel(const FObjectInitializer& ObjectInitializer};

    // ???X    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void InitializeQuestPanel(UMingRelationshipManager* InRelationshipManager);

    // 任??管??
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void AddQuest(const FQuestData& QuestData);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void RemoveQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void UpdateQuest(const FString& QuestID, const FQuestData& QuestData);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void AcceptQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void AbandonQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void CompleteQuest(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void FailQuest(const FString& QuestID);

    // 任?X?詢
    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    FQuestData GetQuest(const FString& QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    TArray<FQuestData> GetQuestsByStatus(EQuestStatus Status) const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    TArray<FQuestData> GetQuestsByType(EQuestType Type) const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    TArray<FQuestData> GetQuestsByRegion(const FString& RegionID) const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    bool CanAcceptQuest(const FString& QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    bool HasActiveQuests() const;

    // ??濾??????    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void FilterByStatus(EQuestStatus Status, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void FilterByType(EQuestType Type, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void FilterByRegion(const FString& RegionID, bool bActive);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void SortByDifficulty(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void SortByLevel(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void SortByTime(bool bAscending = true);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ClearFilters();

    // ??索
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void SearchQuests(const FString& SearchText);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ClearSearch();

    // 統??信息
    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    int32 GetTotalQuestCount() const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    int32 GetAvailableQuestCount() const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    int32 GetActiveQuestCount() const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    int32 GetCompletedQuestCount() const;

    UFUNCTION(BlueprintPure, Category = "Quest Panel")
    float GetQuestCompletionRate() const;

    // 詳細信息
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ShowQuestDetails(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void HideQuestDetails();

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void UpdateQuestProgress(const FString& QuestID, const FString& RequirementText, float Progress);

    // 互?X???
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestClicked(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestDoubleClicked(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestRightClicked(const FString& QuestID);

    // 導出???X    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ExportQuestData();

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ImportQuestData();

    // 事件
    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestAccepted OnQuestAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestFailed OnQuestFailed;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestSelected OnQuestSelected;

protected:
    // 系統引用
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // 任?X???
    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestData> AllQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestData> DisplayedQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FQuestData SelectedQuest;

    // ??濾??X    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EQuestStatus, bool> StatusFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EQuestType, bool> TypeFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TSet<FString> RegionFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    FString CurrentSearchText;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    bool bIsFiltered = false;

    // ???X?X    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByDifficulty = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByLevel = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByTime = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortAscending = true;

    // UI組件引用（??要在Blueprint中??定??
    UPROPERTY(meta = (BindWidget))
    class UListView* QuestListView;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TotalQuestText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* AvailableQuestText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ActiveQuestText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CompletedQuestText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* CompletionRateText;

    UPROPERTY(meta = (BindWidget))
    class UEditableTextBox* SearchBox;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* DetailsScrollBox;

    // ??擬??數
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ??部??數
    void ProcessQuestData();
    void ApplyFilters();
    void ApplySorting();
    void UpdateStatistics();
    void UpdateUI();
    FLinearColor GetQuestStatusColor(EQuestStatus Status) const;
    FString GetQuestTypeDisplayName(EQuestType Type) const;
    FString GetQuestStatusDisplayName(EQuestStatus Status) const;

    // 事件綁??
    void SetupEventBindings();
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // 輔助??數
    bool PassesFilter(const FQuestData& QuestData) const;
    bool PassesSearch(const FQuestData& QuestData) const;
    void RefreshDisplayList();
    void CheckQuestTimeLimits();
    void GenerateRandomQuests();
);

