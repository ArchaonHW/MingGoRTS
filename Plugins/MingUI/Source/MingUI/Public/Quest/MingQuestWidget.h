#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingQuestWidget.generated.h"

UENUM(BlueprintType)
enum class EMingQuestViewMode : uint8 {
    Active    UMETA(DisplayName = "Active Quests"),
    Available UMETA(DisplayName = "Available Quests"),
    Completed UMETA(DisplayName = "Completed Quests"),
    All       UMETA(DisplayName = "All Quests")
};

UENUM(BlueprintType)
enum class EMingQuestSortType : uint8 {
    ByName         UMETA(DisplayName = "Name"),
    ByProgress     UMETA(DisplayName = "Progress"),
    ByReputation   UMETA(DisplayName = "Reputation Reward"),
    ByRelationship UMETA(DisplayName = "Relationship Impact"),
    ByTime         UMETA(DisplayName = "Time Remaining")
};

USTRUCT(BlueprintType)
struct FMingQuestDisplayData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FName QuestID;

    UPROPERTY(BlueprintReadWrite)
    FString QuestName;

    UPROPERTY(BlueprintReadWrite)
    FText QuestDescription;

    UPROPERTY(BlueprintReadWrite)
    int32 QuestStatus; // 0=Available, 1=Active, 2=Completed

    UPROPERTY(BlueprintReadWrite)
    float ProgressPercent;

    UPROPERTY(BlueprintReadWrite)
    int32 CurrentObjective;

    UPROPERTY(BlueprintReadWrite)
    int32 TotalObjectives;

    UPROPERTY(BlueprintReadWrite)
    FName GiverCharacterID;

    UPROPERTY(BlueprintReadWrite)
    FString GiverName;

    UPROPERTY(BlueprintReadWrite)
    FName TargetRegionID;

    UPROPERTY(BlueprintReadWrite)
    FString TargetRegionName;

    UPROPERTY(BlueprintReadWrite)
    float ReputationReward;

    UPROPERTY(BlueprintReadWrite)
    float RelationshipImpact;

    UPROPERTY(BlueprintReadWrite)
    int32 RequiredReputationRank;

    UPROPERTY(BlueprintReadWrite)
    float RequiredRelationship;

    UPROPERTY(BlueprintReadWrite)
    bool bCanAccept;

    UPROPERTY(BlueprintReadWrite)
    FString UnavailableReason;

    UPROPERTY(BlueprintReadWrite)
    bool bHasTimeLimit;

    UPROPERTY(BlueprintReadWrite)
    float TimeRemaining;

    UPROPERTY(BlueprintReadWrite)
    TArray<FText> ObjectivesList;

    UPROPERTY(BlueprintReadWrite)
    TArray<FText> RewardsList;

    UPROPERTY(BlueprintReadWrite)
    UTexture2D* QuestIcon;

    UPROPERTY(BlueprintReadWrite)
    FLinearColor QuestColor;

    UPROPERTY(BlueprintReadWrite)
    bool bIsPriority;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestSelected, FName, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAccepted, FName, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestAbandoned, FName, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestCompleted, FName, QuestID};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnObjectiveCompleted, FName, QuestID, int32, ObjectiveIndex};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnViewModeChanged, EMingQuestViewMode, NewMode};

/**
 * Quest Panel Widget
 * Displays quest information with reputation/relationship requirements
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingQuestWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingQuestWidget(const FObjectInitializer& ObjectInitializer};

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void InitializeWidget(class UMingPersonalManager* InPersonalManager};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void RefreshQuestList(};

    // View Modes
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void SetViewMode(EMingQuestViewMode NewMode};

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    EMingQuestViewMode GetCurrentViewMode() const { return CurrentViewMode; }

    // Quest Selection
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void SelectQuest(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ClearQuestSelection(};

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    FName GetSelectedQuest() const { return SelectedQuestID; }

    // Quest Actions
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void AcceptQuest(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void AbandonQuest(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void TrackQuest(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void UntrackQuest(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void CompleteQuestObjective(FName QuestID, int32 ObjectiveIndex};

    // Display Data
    UFUNCTION(BlueprintPure, Category = "Quest UI")
    TArray<FMingQuestDisplayData> GetQuestDisplayData() const;

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    FMingQuestDisplayData GetQuestData(FName QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    TArray<FMingQuestDisplayData> GetTrackedQuests() const;

    // Filters and Sorting
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void SortQuests(EMingQuestSortType SortType, bool bAscending = true};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void FilterByRegion(FName RegionID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void FilterByGiver(FName CharacterID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowOnlyAvailable(};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowOnlyPriority(};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ClearFilters(};

    // Requirements Check
    UFUNCTION(BlueprintPure, Category = "Quest UI")
    bool CanAcceptQuest(FName QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    FString GetQuestRequirementText(FName QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    bool HasSufficientReputation(FName QuestID) const;

    UFUNCTION(BlueprintPure, Category = "Quest UI")
    bool HasSufficientRelationship(FName QuestID) const;

    // Visual Feedback
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void HighlightQuestUpdate(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void PlayQuestAcceptedAnimation(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void PlayQuestCompletedAnimation(FName QuestID};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowProgressUpdate(FName QuestID, float OldProgress, float NewProgress};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void FlashObjective(int32 ObjectiveIndex};

    // Audio Integration
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void PlayQuestSound(FName QuestID, int32 EventType};

    // Tracking HUD
    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void UpdateTrackingHUD(};

    UFUNCTION(BlueprintCallable, Category = "Quest UI")
    void ShowTrackingHUD(bool bShow};

    // Event Delegates
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuestSelected OnQuestSelected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuestAccepted OnQuestAccepted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuestAbandoned OnQuestAbandoned;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnQuestCompleted OnQuestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnObjectiveCompleted OnObjectiveCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnViewModeChanged OnViewModeChanged;

protected:
    // UI Components
    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* QuestListContainer;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* QuestScrollBox;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TitleText;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* SelectedQuestName;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* QuestDescriptionText;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* ObjectivesContainer;

    UPROPERTY(meta = (BindWidget))
    class UVerticalBox* RewardsContainer;

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* QuestProgressBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* ProgressText;

    UPROPERTY(meta = (BindWidget))
    class UHorizontalBox* RequirementsContainer;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TimeRemainingText;

    UPROPERTY(meta = (BindWidget))
    class UButton* AcceptButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AbandonButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* TrackButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* ActiveTabButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* AvailableTabButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* CompletedTabButton;

    // Data
    UPROPERTY()
    class UMingPersonalManager* PersonalManager;

    UPROPERTY()
    EMingQuestViewMode CurrentViewMode;

    UPROPERTY()
    FName SelectedQuestID;

    UPROPERTY()
    TArray<FMingQuestDisplayData> DisplayDataCache;

    UPROPERTY()
    FName RegionFilter;

    UPROPERTY()
    FName GiverFilter;

    UPROPERTY()
    bool bShowOnlyAvailable;

    UPROPERTY()
    bool bShowOnlyPriority;

    UPROPERTY()
    EMingQuestSortType CurrentSortType;

    UPROPERTY()
    bool bSortAscending;

    // Button Handlers
    UFUNCTION()
    void OnActiveTabClicked(};

    UFUNCTION()
    void OnAvailableTabClicked(};

    UFUNCTION()
    void OnCompletedTabClicked(};

    UFUNCTION()
    void OnQuestButtonClicked(FName QuestID};

    UFUNCTION()
    void OnAcceptButtonClicked(};

    UFUNCTION()
    void OnAbandonButtonClicked(};

    UFUNCTION()
    void OnTrackButtonClicked(};

    UFUNCTION()
    void OnCloseButtonClicked(};

    // Internal Functions
    void PopulateQuestList(};
    void UpdateQuestDetails(};
    void CreateQuestEntry(const FMingQuestDisplayData& Data};
    void CreateObjectiveEntry(const FText& ObjectiveText, bool bIsCompleted, int32 Index};
    void CreateRewardEntry(const FText& RewardText};
    void ClearQuestList(};
    void ClearObjectivesList(};
    void ClearRewardsList(};
    void FilterAndSortData(};
    void UpdateButtonStates(};
    FLinearColor GetQuestColor(int32 Status) const;
    FString FormatTimeRemaining(float Seconds) const;

    // Animation
    void AnimateProgressBar(float TargetValue};
    void AnimateQuestEntry(UWidget* EntryWidget};
    void PlayCompletionEffects(};

public:
    // Static Utilities
    UFUNCTION(BlueprintPure, Category = "Quest UI", meta = (StaticFunction))
    static FLinearColor GetStatusColor(int32 Status};

    UFUNCTION(BlueprintPure, Category = "Quest UI", meta = (StaticFunction))
    static FString GetStatusText(int32 Status};
};
