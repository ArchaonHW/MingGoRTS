#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "MingRelationshipManager.h"
#include "MingQuestPanel.generated.h"

UENUM(BlueprintType)
enum class EQuestStatus : uint8
{
    Available,      // ?ØÊé•??    Active,         // ?≤Ë?‰∏?    Completed,      // Â∑≤Â???    Failed,         // Â§±Ê?
    Abandoned       // ?æÊ?
};

UENUM(BlueprintType)
enum class EQuestType : uint8
{
    Main,           // ‰∏ªÁ?‰ªªÂ?
    Side,           // ?ØÁ?‰ªªÂ?
    Daily,          // ÊØèÊó•‰ªªÂ?
    Weekly,         // ÊØèÈÄ±‰ªª??    Special,        // ?πÊ?‰ªªÂ?
    Relationship,    // ?ú‰?‰ªªÂ?
    Reputation,      // ?≤Ê?‰ªªÂ?
    Exploration      // ?¢Á¥¢‰ªªÂ?
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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, const FString&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, const FString&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestFailed, const FString&, QuestID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnQuestSelected, const FString&, QuestID, EQuestStatus, Status);

/**
 * ‰ªªÂ??¢ÊùøWidget
 * ÁÆ°Á?‰ªªÂ??åÈù¢
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingQuestPanel : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingQuestPanel(const FObjectInitializer& ObjectInitializer);

    // ?ùÂ???    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void InitializeQuestPanel(UMingRelationshipManager* InRelationshipManager);

    // ‰ªªÂ?ÁÆ°Á?
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

    // ‰ªªÂ??•Ë©¢
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

    // ?éÊøæ?åÊ?Â∫?    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
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

    // ?úÁ¥¢
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void SearchQuests(const FString& SearchText);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ClearSearch();

    // Áµ±Ë?‰ø°ÊÅØ
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

    // Ë©≥Á¥∞‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ShowQuestDetails(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void HideQuestDetails();

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void UpdateQuestProgress(const FString& QuestID, const FString& RequirementText, float Progress);

    // ‰∫íÂ??ç‰?
    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestClicked(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestDoubleClicked(const FString& QuestID);

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void OnQuestRightClicked(const FString& QuestID);

    // Â∞éÂá∫?åÂ???    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ExportQuestData();

    UFUNCTION(BlueprintCallable, Category = "Quest Panel")
    void ImportQuestData();

    // ‰∫ã‰ª∂
    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestAccepted OnQuestAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestFailed OnQuestFailed;

    UPROPERTY(BlueprintAssignable, Category = "Quest Events")
    FOnQuestSelected OnQuestSelected;

protected:
    // Á≥ªÁµ±ÂºïÁî®
    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipManager;

    // ‰ªªÂ??∏Ê?
    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestData> AllQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    TArray<FQuestData> DisplayedQuests;

    UPROPERTY(BlueprintReadOnly, Category = "Quest Data")
    FQuestData SelectedQuest;

    // ?éÊøæ?Ä??    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EQuestStatus, bool> StatusFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TMap<EQuestType, bool> TypeFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    TSet<FString> RegionFilters;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    FString CurrentSearchText;

    UPROPERTY(BlueprintReadOnly, Category = "Filter State")
    bool bIsFiltered = false;

    // ?íÂ??Ä??    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByDifficulty = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByLevel = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortByTime = false;

    UPROPERTY(BlueprintReadOnly, Category = "Sort State")
    bool bSortAscending = true;

    // UIÁµÑ‰ª∂ÂºïÁî®ÔºàÈ?Ë¶ÅÂú®Blueprint‰∏≠Á?ÂÆöÔ?
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

    // ?õÊì¨?ΩÊï∏
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    // ?ßÈÉ®?ΩÊï∏
    void ProcessQuestData();
    void ApplyFilters();
    void ApplySorting();
    void UpdateStatistics();
    void UpdateUI();
    FLinearColor GetQuestStatusColor(EQuestStatus Status) const;
    FString GetQuestTypeDisplayName(EQuestType Type) const;
    FString GetQuestStatusDisplayName(EQuestStatus Status) const;

    // ‰∫ã‰ª∂Á∂ÅÂ?
    void SetupEventBindings();
    void OnReputationChanged(const FString& RegionID, float OldValue, float NewValue, const FString& Reason);

private:
    // ËºîÂä©?ΩÊï∏
    bool PassesFilter(const FQuestData& QuestData) const;
    bool PassesSearch(const FQuestData& QuestData) const;
    void RefreshDisplayList();
    void CheckQuestTimeLimits();
    void GenerateRandomQuests();
};
