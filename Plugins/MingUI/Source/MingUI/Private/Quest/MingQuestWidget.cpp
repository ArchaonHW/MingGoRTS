#include "Quest/MingQuestWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/ProgressBar.h"
#include "Components/CheckBox.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "MingPersonal/Public/MingPersonalManager.h"

UMingQuestWidget::UMingQuestWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , CurrentViewMode(EMingQuestViewMode::Active)
    , CurrentSortType(EMingQuestSortType::ByProgress)
    , bSortAscending(false)
    , bShowOnlyAvailable(false)
    , bShowOnlyPriority(false)
{
}

void UMingQuestWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Bind button events
    if (ActiveTabButton)
    {
        ActiveTabButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnActiveTabClicked);
    }
    if (AvailableTabButton)
    {
        AvailableTabButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnAvailableTabClicked);
    }
    if (CompletedTabButton)
    {
        CompletedTabButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnCompletedTabClicked);
    }
    if (AcceptButton)
    {
        AcceptButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnAcceptButtonClicked);
    }
    if (AbandonButton)
    {
        AbandonButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnAbandonButtonClicked);
    }
    if (TrackButton)
    {
        TrackButton->OnClicked.AddDynamic(this, &UMingQuestWidget::OnTrackButtonClicked);
    }
}

void UMingQuestWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingQuestWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // Update time remaining displays
    if (TimeRemainingText && !SelectedQuestID.IsNone())
    {
        FMingQuestDisplayData Data = GetQuestData(SelectedQuestID);
        if (Data.bHasTimeLimit && Data.TimeRemaining > 0)
        {
            TimeRemainingText->SetText(FText::FromString(FormatTimeRemaining(Data.TimeRemaining)));
        }
    }
}

void UMingQuestWidget::InitializeWidget(UMingPersonalManager* InPersonalManager)
{
    PersonalManager = InPersonalManager;
    RefreshQuestList();
}

void UMingQuestWidget::RefreshQuestList()
{
    DisplayDataCache = GetQuestDisplayData();
    FilterAndSortData();
    PopulateQuestList();
    
    if (!SelectedQuestID.IsNone())
    {
        UpdateQuestDetails();
    }
}

void UMingQuestWidget::SetViewMode(EMingQuestViewMode NewMode)
{
    if (CurrentViewMode != NewMode)
    {
        CurrentViewMode = NewMode;
        
        // Update tab button states
        if (ActiveTabButton)
        {
            ActiveTabButton->SetIsEnabled(CurrentViewMode != EMingQuestViewMode::Active);
        }
        if (AvailableTabButton)
        {
            AvailableTabButton->SetIsEnabled(CurrentViewMode != EMingQuestViewMode::Available);
        }
        if (CompletedTabButton)
        {
            CompletedTabButton->SetIsEnabled(CurrentViewMode != EMingQuestViewMode::Completed);
        }
        
        RefreshQuestList();
        OnViewModeChanged.Broadcast();
    }
}

void UMingQuestWidget::SelectQuest(FName QuestID)
{
    if (SelectedQuestID != QuestID)
    {
        SelectedQuestID = QuestID;
        UpdateQuestDetails();
        OnQuestSelected.Broadcast(QuestID);
    }
}

void UMingQuestWidget::ClearQuestSelection()
{
    SelectedQuestID = FName();
    if (SelectedQuestName)
    {
        SelectedQuestName->SetText(FText::FromString(TEXT("Select a Quest")));
    }
    UpdateButtonStates();
}

TArray<FMingQuestDisplayData> UMingQuestWidget::GetQuestDisplayData() const
{
    TArray<FMingQuestDisplayData> Result;
    
    // This would query the personal manager for quest data
    // Placeholder implementation
    
    return Result;
}

FMingQuestDisplayData UMingQuestWidget::GetQuestData(FName QuestID) const
{
    for (const FMingQuestDisplayData& Data : DisplayDataCache)
    {
        if (Data.QuestID == QuestID)
        {
            return Data;
        }
    }
    return FMingQuestDisplayData();
}

TArray<FMingQuestDisplayData> UMingQuestWidget::GetTrackedQuests() const
{
    TArray<FMingQuestDisplayData> Tracked;
    
    for (const FMingQuestDisplayData& Data : DisplayDataCache)
    {
        // Check if quest is being tracked
        // This would query the personal manager
    }
    
    return Tracked;
}

void UMingQuestWidget::SortQuests(EMingQuestSortType SortType, bool bAscending)
{
    CurrentSortType = SortType;
    bSortAscending = bAscending;
    FilterAndSortData();
    PopulateQuestList();
}

void UMingQuestWidget::FilterByRegion(FName RegionID)
{
    RegionFilter = RegionID;
    RefreshQuestList();
}

void UMingQuestWidget::FilterByGiver(FName CharacterID)
{
    GiverFilter = CharacterID;
    RefreshQuestList();
}

void UMingQuestWidget::ShowOnlyAvailable()
{
    bShowOnlyAvailable = true;
    RefreshQuestList();
}

void UMingQuestWidget::ShowOnlyPriority()
{
    bShowOnlyPriority = true;
    RefreshQuestList();
}

void UMingQuestWidget::ClearFilters()
{
    RegionFilter = FName();
    GiverFilter = FName();
    bShowOnlyAvailable = false;
    bShowOnlyPriority = false;
    RefreshQuestList();
}

bool UMingQuestWidget::CanAcceptQuest(FName QuestID) const
{
    FMingQuestDisplayData Data = GetQuestData(QuestID);
    return HasSufficientReputation(QuestID) && HasSufficientRelationship(QuestID);
}

FString UMingQuestWidget::GetQuestRequirementText(FName QuestID) const
{
    FMingQuestDisplayData Data = GetQuestData(QuestID);
    FString Requirements;
    
    if (Data.RequiredReputationRank > 0)
    {
        Requirements += FString::Printf(TEXT("Rank %d required\n"), Data.RequiredReputationRank);
    }
    
    if (Data.RequiredRelationship > 0)
    {
        Requirements += FString::Printf(TEXT("%.0f relationship required\n"), Data.RequiredRelationship);
    }
    
    return Requirements;
}

bool UMingQuestWidget::HasSufficientReputation(FName QuestID) const
{
    FMingQuestDisplayData Data = GetQuestData(QuestID);
    // Check against current reputation rank
    return true; // Placeholder
}

bool UMingQuestWidget::HasSufficientRelationship(FName QuestID) const
{
    FMingQuestDisplayData Data = GetQuestData(QuestID);
    // Check against current relationship value
    return true; // Placeholder
}

void UMingQuestWidget::HighlightQuestUpdate(FName QuestID)
{
    PlayHighlightEffects();
}

void UMingQuestWidget::PlayQuestAcceptedAnimation(FName QuestID)
{
    PlayQuestSound(QuestID, 1); // Accept sound
    
    // Find and animate the quest entry
    // AnimateQuestEntry(...);
}

void UMingQuestWidget::PlayQuestCompletedAnimation(FName QuestID)
{
    PlayQuestSound(QuestID, 2); // Complete sound
    PlayCompletionEffects();
    OnQuestCompleted.Broadcast(QuestID);
}

void UMingQuestWidget::ShowProgressUpdate(FName QuestID, float OldProgress, float NewProgress)
{
    if (SelectedQuestID == QuestID && QuestProgressBar)
    {
        AnimateProgressBar(NewProgress);
    }
}

void UMingQuestWidget::FlashObjective(int32 ObjectiveIndex)
{
    // Flash the objective at the given index
}

void UMingQuestWidget::PlayQuestSound(FName QuestID, int32 EventType)
{
    // Play quest-related sound
    // 0 = Update, 1 = Accept, 2 = Complete, 3 = Abandon
}

void UMingQuestWidget::UpdateTrackingHUD()
{
    // Update the tracking HUD display
}

void UMingQuestWidget::ShowTrackingHUD(bool bShow)
{
    // Show or hide the tracking HUD
}

// Button Handlers

void UMingQuestWidget::OnActiveTabClicked()
{
    SetViewMode(EMingQuestViewMode::Active);
}

void UMingQuestWidget::OnAvailableTabClicked()
{
    SetViewMode(EMingQuestViewMode::Available);
}

void UMingQuestWidget::OnCompletedTabClicked()
{
    SetViewMode(EMingQuestViewMode::Completed);
}

void UMingQuestWidget::OnQuestButtonClicked(FName QuestID)
{
    SelectQuest(QuestID);
}

void UMingQuestWidget::OnAcceptButtonClicked()
{
    if (!SelectedQuestID.IsNone() && CanAcceptQuest(SelectedQuestID))
    {
        AcceptQuest(SelectedQuestID);
        PlayQuestAcceptedAnimation(SelectedQuestID);
        OnQuestAccepted.Broadcast(SelectedQuestID);
    }
}

void UMingQuestWidget::OnAbandonButtonClicked()
{
    if (!SelectedQuestID.IsNone())
    {
        AbandonQuest(SelectedQuestID);
        OnQuestAbandoned.Broadcast(SelectedQuestID);
    }
}

void UMingQuestWidget::OnTrackButtonClicked()
{
    if (!SelectedQuestID.IsNone())
    {
        TrackQuest(SelectedQuestID);
    }
}

void UMingQuestWidget::OnCloseButtonClicked()
{
    RemoveFromParent();
}

// Internal Functions

void UMingQuestWidget::PopulateQuestList()
{
    ClearQuestList();
    
    if (!QuestListContainer)
    {
        return;
    }
    
    for (const FMingQuestDisplayData& Data : DisplayDataCache)
    {
        CreateQuestEntry(Data);
    }
}

void UMingQuestWidget::UpdateQuestDetails()
{
    FMingQuestDisplayData Data = GetQuestData(SelectedQuestID);
    
    if (SelectedQuestName)
    {
        SelectedQuestName->SetText(FText::FromString(Data.QuestName));
    }
    
    if (QuestDescriptionText)
    {
        QuestDescriptionText->SetText(Data.QuestDescription);
    }
    
    if (QuestProgressBar)
    {
        QuestProgressBar->SetPercent(Data.ProgressPercent / 100.0f);
    }
    
    if (ProgressText)
    {
        FString ProgressString = FString::Printf(TEXT("%d/%d"), Data.CurrentObjective, Data.TotalObjectives);
        ProgressText->SetText(FText::FromString(ProgressString));
    }
    
    // Update objectives
    ClearObjectivesList();
    for (int32 i = 0; i < Data.ObjectivesList.Num(); i++)
    {
        bool bCompleted = i < Data.CurrentObjective;
        CreateObjectiveEntry(Data.ObjectivesList[i], bCompleted, i);
    }
    
    // Update rewards
    ClearRewardsList();
    for (const FText& Reward : Data.RewardsList)
    {
        CreateRewardEntry(Reward);
    }
    
    // Update requirements
    if (RequirementsContainer)
    {
        RequirementsContainer->ClearChildren();
        FString ReqText = GetQuestRequirementText(SelectedQuestID);
        if (!ReqText.IsEmpty())
        {
            UTextBlock* ReqLabel = NewObject<UTextBlock>(this);
            ReqLabel->SetText(FText::FromString(TEXT("Requirements:")));
            RequirementsContainer->AddChildToHorizontalBox(ReqLabel);
            
            UTextBlock* ReqValue = NewObject<UTextBlock>(this);
            ReqValue->SetText(FText::FromString(ReqText));
            RequirementsContainer->AddChildToHorizontalBox(ReqValue);
        }
    }
    
    UpdateButtonStates();
}

void UMingQuestWidget::CreateQuestEntry(const FMingQuestDisplayData& Data)
{
    if (!QuestListContainer)
    {
        return;
    }
    
    UButton* QuestButton = NewObject<UButton>(this);
    if (QuestButton)
    {
        UHorizontalBox* ButtonContent = NewObject<UHorizontalBox>(this);
        
        // Quest icon
        if (Data.QuestIcon)
        {
            UImage* IconImage = NewObject<UImage>(this);
            IconImage->SetBrushFromTexture(Data.QuestIcon);
            ButtonContent->AddChildToHorizontalBox(IconImage);
        }
        
        // Quest name
        UTextBlock* NameText = NewObject<UTextBlock>(this);
        NameText->SetText(FText::FromString(Data.QuestName));
        ButtonContent->AddChildToHorizontalBox(NameText);
        
        // Progress indicator
        UTextBlock* ProgressIndicator = NewObject<UTextBlock>(this);
        FString ProgressStr = FString::Printf(TEXT("(%d/%d)"), Data.CurrentObjective, Data.TotalObjectives);
        ProgressIndicator->SetText(FText::FromString(ProgressStr));
        ButtonContent->AddChildToHorizontalBox(ProgressIndicator);
        
        // Priority marker
        if (Data.bIsPriority)
        {
            UTextBlock* PriorityMarker = NewObject<UTextBlock>(this);
            PriorityMarker->SetText(FText::FromString(TEXT("[!]")));
            ButtonContent->AddChildToHorizontalBox(PriorityMarker);
        }
        
        // Bind click event
        FScriptDelegate ClickDelegate;
        ClickDelegate.BindUFunction(this, FName("OnQuestButtonClicked"), Data.QuestID);
        QuestButton->OnClicked.Add(ClickDelegate);
        
        QuestListContainer->AddChildToVerticalBox(QuestButton);
    }
}

void UMingQuestWidget::CreateObjectiveEntry(const FText& ObjectiveText, bool bIsCompleted, int32 Index)
{
    if (!ObjectivesContainer)
    {
        return;
    }
    
    UHorizontalBox* ObjectiveRow = NewObject<UHorizontalBox>(this);
    
    // Checkbox
    UCheckBox* CheckBox = NewObject<UCheckBox>(this);
    CheckBox->SetIsChecked(bIsCompleted);
    CheckBox->SetIsEnabled(false); // Read-only
    ObjectiveRow->AddChildToHorizontalBox(CheckBox);
    
    // Objective text
    UTextBlock* ObjText = NewObject<UTextBlock>(this);
    ObjText->SetText(ObjectiveText);
    if (bIsCompleted)
    {
        // Strike-through effect would be applied via style
    }
    ObjectiveRow->AddChildToHorizontalBox(ObjText);
    
    ObjectivesContainer->AddChildToVerticalBox(ObjectiveRow);
}

void UMingQuestWidget::CreateRewardEntry(const FText& RewardText)
{
    if (!RewardsContainer)
    {
        return;
    }
    
    UTextBlock* RewardLabel = NewObject<UTextBlock>(this);
    RewardLabel->SetText(FText::FromString(TEXT("+ ")));
    
    UHorizontalBox* RewardRow = NewObject<UHorizontalBox>(this);
    RewardRow->AddChildToHorizontalBox(RewardLabel);
    
    UTextBlock* RewardValue = NewObject<UTextBlock>(this);
    RewardValue->SetText(RewardText);
    RewardRow->AddChildToHorizontalBox(RewardValue);
    
    RewardsContainer->AddChildToVerticalBox(RewardRow);
}

void UMingQuestWidget::ClearQuestList()
{
    if (QuestListContainer)
    {
        QuestListContainer->ClearChildren();
    }
}

void UMingQuestWidget::ClearObjectivesList()
{
    if (ObjectivesContainer)
    {
        ObjectivesContainer->ClearChildren();
    }
}

void UMingQuestWidget::ClearRewardsList()
{
    if (RewardsContainer)
    {
        RewardsContainer->ClearChildren();
    }
}

void UMingQuestWidget::FilterAndSortData()
{
    // Apply filters
    TArray<FMingQuestDisplayData> FilteredData;
    
    for (const FMingQuestDisplayData& Data : DisplayDataCache)
    {
        // View mode filter
        bool bMatchesMode = false;
        switch (CurrentViewMode)
        {
        case EMingQuestViewMode::Active:
            bMatchesMode = (Data.QuestStatus == 1);
            break;
        case EMingQuestViewMode::Available:
            bMatchesMode = (Data.QuestStatus == 0);
            break;
        case EMingQuestViewMode::Completed:
            bMatchesMode = (Data.QuestStatus == 2);
            break;
        default:
            bMatchesMode = true;
        }
        
        if (!bMatchesMode)
        {
            continue;
        }
        
        // Region filter
        if (!RegionFilter.IsNone() && Data.TargetRegionID != RegionFilter)
        {
            continue;
        }
        
        // Giver filter
        if (!GiverFilter.IsNone() && Data.GiverCharacterID != GiverFilter)
        {
            continue;
        }
        
        // Available filter
        if (bShowOnlyAvailable && !Data.bCanAccept)
        {
            continue;
        }
        
        // Priority filter
        if (bShowOnlyPriority && !Data.bIsPriority)
        {
            continue;
        }
        
        FilteredData.Add(Data);
    }
    
    DisplayDataCache = FilteredData;
    
    // Apply sorting
    switch (CurrentSortType)
    {
    case EMingQuestSortType::ByName:
        DisplayDataCache.Sort([this](const FMingQuestDisplayData& A, const FMingQuestDisplayData& B)
        {
            if (bSortAscending)
                return A.QuestName < B.QuestName;
            return A.QuestName > B.QuestName;
        });
        break;
        
    case EMingQuestSortType::ByProgress:
        DisplayDataCache.Sort([this](const FMingQuestDisplayData& A, const FMingQuestDisplayData& B)
        {
            if (bSortAscending)
                return A.ProgressPercent < B.ProgressPercent;
            return A.ProgressPercent > B.ProgressPercent;
        });
        break;
        
    case EMingQuestSortType::ByReputation:
        DisplayDataCache.Sort([this](const FMingQuestDisplayData& A, const FMingQuestDisplayData& B)
        {
            if (bSortAscending)
                return A.ReputationReward < B.ReputationReward;
            return A.ReputationReward > B.ReputationReward;
        });
        break;
        
    default:
        break;
    }
}

void UMingQuestWidget::UpdateButtonStates()
{
    if (!SelectedQuestID.IsNone())
    {
        FMingQuestDisplayData Data = GetQuestData(SelectedQuestID);
        
        if (AcceptButton)
        {
            AcceptButton->SetIsEnabled(Data.bCanAccept && Data.QuestStatus == 0);
        }
        if (AbandonButton)
        {
            AbandonButton->SetIsEnabled(Data.QuestStatus == 1);
        }
        if (TrackButton)
        {
            // Enable based on tracking status
            TrackButton->SetIsEnabled(true);
        }
    }
    else
    {
        if (AcceptButton)
        {
            AcceptButton->SetIsEnabled(false);
        }
        if (AbandonButton)
        {
            AbandonButton->SetIsEnabled(false);
        }
        if (TrackButton)
        {
            TrackButton->SetIsEnabled(false);
        }
    }
}

FLinearColor UMingQuestWidget::GetQuestColor(int32 Status) const
{
    switch (Status)
    {
    case 0: // Available
        return FLinearColor(0.8f, 0.8f, 0.8f);
    case 1: // Active
        return FLinearColor(0.2f, 0.8f, 0.4f);
    case 2: // Completed
        return FLinearColor(0.8f, 0.6f, 0.2f);
    default:
        return FLinearColor::White;
    }
}

FString UMingQuestWidget::FormatTimeRemaining(float Seconds) const
{
    int32 Hours = FMath::FloorToInt(Seconds / 3600.0f);
    int32 Minutes = FMath::FloorToInt((Seconds - Hours * 3600.0f) / 60.0f);
    int32 RemainingSeconds = FMath::FloorToInt(Seconds - Hours * 3600.0f - Minutes * 60.0f);
    
    if (Hours > 0)
    {
        return FString::Printf(TEXT("%dh %dm"), Hours, Minutes);
    }
    else if (Minutes > 0)
    {
        return FString::Printf(TEXT("%dm %ds"), Minutes, RemainingSeconds);
    }
    else
    {
        return FString::Printf(TEXT("%ds"), RemainingSeconds);
    }
}

void UMingQuestWidget::AcceptQuest(FName QuestID)
{
    // Accept the quest through the personal manager
}

void UMingQuestWidget::AbandonQuest(FName QuestID)
{
    // Abandon the quest
}

void UMingQuestWidget::TrackQuest(FName QuestID)
{
    // Start tracking the quest
}

void UMingQuestWidget::UntrackQuest(FName QuestID)
{
    // Stop tracking the quest
}

void UMingQuestWidget::CompleteQuestObjective(FName QuestID, int32 ObjectiveIndex)
{
    // Mark objective as complete
    OnObjectiveCompleted.Broadcast(QuestID, ObjectiveIndex);
}

void UMingQuestWidget::AnimateProgressBar(float TargetValue)
{
    if (QuestProgressBar)
    {
        QuestProgressBar->SetPercent(TargetValue / 100.0f);
    }
}

void UMingQuestWidget::AnimateQuestEntry(UWidget* EntryWidget)
{
    // Play entry animation
}

void UMingQuestWidget::PlayCompletionEffects()
{
    // Play completion celebration effects
}

void UMingQuestWidget::PlayHighlightEffects()
{
    // Play highlight animation
}

// Static Utilities

FLinearColor UMingQuestWidget::GetStatusColor(int32 Status)
{
    switch (Status)
    {
    case 0: return FLinearColor(0.8f, 0.8f, 0.8f);
    case 1: return FLinearColor(0.2f, 0.8f, 0.4f);
    case 2: return FLinearColor(0.8f, 0.6f, 0.2f);
    default: return FLinearColor::White;
    }
}

FString UMingQuestWidget::GetStatusText(int32 Status)
{
    switch (Status)
    {
    case 0: return TEXT("Available");
    case 1: return TEXT("Active");
    case 2: return TEXT("Completed");
    default: return TEXT("Unknown");
    }
}
