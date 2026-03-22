#include "Reputation/MingReputationWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "MingPersonal/Public/MingPersonalManager.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"

UMingReputationWidget::UMingReputationWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , PersonalManager(nullptr)
    , CurrentViewMode(EMingReputationViewMode::Overview)
    , MinimumRankFilter(0)
    , bFilterQuestsOnly(false)
{
}

void UMingReputationWidget::NativeConstruct()
{
    Super::NativeConstruct();
    RefreshDisplay();
}

void UMingReputationWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingReputationWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMingReputationWidget::InitializeWidget(UMingPersonalManager* InPersonalManager)
{
    PersonalManager = InPersonalManager;
    RefreshDisplay();
}

void UMingReputationWidget::RefreshDisplay()
{
    DisplayDataCache = GetAllReputationData();
    FilterAndSortData();
    
    switch (CurrentViewMode)
    {
    case EMingReputationViewMode::Overview:
        PopulateRegionList();
        UpdateStatsOverview();
        break;
    case EMingReputationViewMode::RegionDetails:
        PopulateRegionList();
        if (!SelectedRegionID.IsNone())
        {
            UpdateRegionDetails();
        }
        break;
    case EMingReputationViewMode::Timeline:
        DrawTimeline();
        break;
    case EMingReputationViewMode::Comparison:
        DrawReputationGraph();
        break;
    }
}

void UMingReputationWidget::SetViewMode(EMingReputationViewMode NewMode)
{
    if (CurrentViewMode != NewMode)
    {
        CurrentViewMode = NewMode;
        RefreshDisplay();
        OnViewModeChanged.Broadcast();
    }
}

void UMingReputationWidget::SelectRegion(FName RegionID)
{
    if (SelectedRegionID != RegionID)
    {
        SelectedRegionID = RegionID;
        UpdateRegionDetails();
        OnRegionSelected.Broadcast(RegionID);
    }
}

void UMingReputationWidget::ClearRegionSelection()
{
    SelectedRegionID = FName();
    if (SelectedRegionName)
    {
        SelectedRegionName->SetText(FText::FromString(TEXT("Select a Region")));
    }
}

TArray<FMingReputationDisplayData> UMingReputationWidget::GetAllReputationData() const
{
    TArray<FMingReputationDisplayData> Result;
    
    if (!PersonalManager)
    {
        return Result;
    }
    
    // Get all regions and their reputation data
    // This is simplified - actual implementation would query the personal manager
    TArray<FName> RegionIDs; // = PersonalManager->GetAllRegionIDs();
    
    for (const FName& RegionID : RegionIDs)
    {
        FMingReputationDisplayData Data;
        Data.RegionID = RegionID;
        // Data.RegionName = PersonalManager->GetRegionName(RegionID);
        // Data.ReputationValue = PersonalManager->GetReputation(RegionID);
        // Data.ReputationRank = PersonalManager->GetReputationRank(RegionID);
        Data.RankTitle = GetRankTitle(Data.ReputationRank);
        Data.ReputationColor = GetReputationColor(Data.ReputationValue);
        Data.NextRankProgress = CalculateNextRankProgress(Data.ReputationValue, Data.ReputationRank);
        // Data.ReputationToNextRank = PersonalManager->GetReputationToNextRank(RegionID);
        // Data.AvailableQuests = PersonalManager->GetAvailableQuestsCount(RegionID);
        // Data.CompletedQuests = PersonalManager->GetCompletedQuestsCount(RegionID);
        
        Result.Add(Data);
    }
    
    return Result;
}

FMingReputationDisplayData UMingReputationWidget::GetRegionData(FName RegionID) const
{
    for (const FMingReputationDisplayData& Data : DisplayDataCache)
    {
        if (Data.RegionID == RegionID)
        {
            return Data;
        }
    }
    return FMingReputationDisplayData();
}

float UMingReputationWidget::GetTotalReputation() const
{
    float Total = 0.0f;
    for (const FMingReputationDisplayData& Data : DisplayDataCache)
    {
        Total += Data.ReputationValue;
    }
    return Total;
}

int32 UMingReputationWidget::GetAverageRank() const
{
    if (DisplayDataCache.Num() == 0)
    {
        return 0;
    }
    
    int32 TotalRank = 0;
    for (const FMingReputationDisplayData& Data : DisplayDataCache)
    {
        TotalRank += Data.ReputationRank;
    }
    
    return TotalRank / DisplayDataCache.Num();
}

TArray<FMingReputationHistoryPoint> UMingReputationWidget::GetReputationHistory(FName RegionID, int32 MaxPoints) const
{
    TArray<FMingReputationHistoryPoint> History;
    
    // Query history from personal manager
    // This is a placeholder - actual implementation would get real history data
    
    return History;
}

void UMingReputationWidget::SortByReputation(bool bDescending)
{
    DisplayDataCache.Sort([bDescending](const FMingReputationDisplayData& A, const FMingReputationDisplayData& B)
    {
        if (bDescending)
        {
            return A.ReputationValue > B.ReputationValue;
        }
        return A.ReputationValue < B.ReputationValue;
    });
    
    PopulateRegionList();
}

void UMingReputationWidget::SortByRegionName(bool bAscending)
{
    DisplayDataCache.Sort([bAscending](const FMingReputationDisplayData& A, const FMingReputationDisplayData& B)
    {
        if (bAscending)
        {
            return A.RegionName < B.RegionName;
        }
        return A.RegionName > B.RegionName;
    });
    
    PopulateRegionList();
}

void UMingReputationWidget::SortByRank(bool bDescending)
{
    DisplayDataCache.Sort([bDescending](const FMingReputationDisplayData& A, const FMingReputationDisplayData& B)
    {
        if (bDescending)
        {
            return A.ReputationRank > B.ReputationRank;
        }
        return A.ReputationRank < B.ReputationRank;
    });
    
    PopulateRegionList();
}

void UMingReputationWidget::FilterByMinimumRank(int32 MinRank)
{
    MinimumRankFilter = MinRank;
    FilterAndSortData();
    PopulateRegionList();
}

void UMingReputationWidget::ShowOnlyRegionsWithQuests()
{
    bFilterQuestsOnly = true;
    FilterAndSortData();
    PopulateRegionList();
}

void UMingReputationWidget::ClearFilters()
{
    MinimumRankFilter = 0;
    bFilterQuestsOnly = false;
    FilterAndSortData();
    PopulateRegionList();
}

void UMingReputationWidget::HighlightReputationChange(FName RegionID, float DeltaValue)
{
    // Find and highlight the region entry
    PlayHighlightEffect(RegionID);
}

void UMingReputationWidget::PlayRankUpAnimation(FName RegionID, int32 NewRank)
{
    // Play rank up animation for the region
    OnRankChanged.Broadcast(NewRank);
}

void UMingReputationWidget::ShowReputationEffectNotification(FName RegionID, const FString& EffectDescription)
{
    // Show a notification for reputation effect
}

void UMingReputationWidget::PlayReputationSound(float Value, float Delta)
{
    // Play sound based on reputation change
}

// Button Handlers

void UMingReputationWidget::OnOverviewButtonClicked()
{
    SetViewMode(EMingReputationViewMode::Overview);
}

void UMingReputationWidget::OnDetailsButtonClicked()
{
    SetViewMode(EMingReputationViewMode::RegionDetails);
}

void UMingReputationWidget::OnTimelineButtonClicked()
{
    SetViewMode(EMingReputationViewMode::Timeline);
}

void UMingReputationWidget::OnCloseButtonClicked()
{
    RemoveFromParent();
}

void UMingReputationWidget::OnRegionButtonClicked(FName RegionID)
{
    SelectRegion(RegionID);
    FMingReputationDisplayData Data = GetRegionData(RegionID);
    OnReputationClicked.Broadcast(RegionID, Data.ReputationValue);
}

// Internal Functions

void UMingReputationWidget::PopulateRegionList()
{
    ClearRegionList();
    
    if (!RegionListContainer)
    {
        return;
    }
    
    for (const FMingReputationDisplayData& Data : DisplayDataCache)
    {
        CreateRegionEntry(Data);
    }
}

void UMingReputationWidget::UpdateRegionDetails()
{
    FMingReputationDisplayData Data = GetRegionData(SelectedRegionID);
    
    if (SelectedRegionName)
    {
        SelectedRegionName->SetText(FText::FromString(Data.RegionName));
    }
    
    if (RankTitleText)
    {
        RankTitleText->SetText(FText::FromString(Data.RankTitle));
    }
    
    if (ReputationProgressBar)
    {
        AnimateProgressBar(Data.NextRankProgress, 0.5f);
    }
    
    if (ReputationValueText)
    {
        ReputationValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Data.ReputationValue)));
    }
    
    if (NextRankText)
    {
        NextRankText->SetText(FText::FromString(FString::Printf(TEXT("%.0f to next rank"), Data.ReputationToNextRank)));
    }
    
    if (QuestsAvailableText)
    {
        QuestsAvailableText->SetText(FText::FromString(FString::Printf(TEXT("%d quests available"), Data.AvailableQuests)));
    }
    
    // Update effects list
    ClearEffectsList();
    for (const FString& Effect : Data.ActiveEffects)
    {
        CreateEffectEntry(Effect);
    }
}

void UMingReputationWidget::UpdateStatsOverview()
{
    // Update overview statistics
}

void UMingReputationWidget::DrawReputationGraph()
{
    // Draw comparison graph
}

void UMingReputationWidget::DrawTimeline()
{
    // Draw reputation history timeline
}

void UMingReputationWidget::CreateRegionEntry(const FMingReputationDisplayData& Data)
{
    if (!RegionListContainer)
    {
        return;
    }
    
    UButton* RegionButton = NewObject<UButton>(this);
    if (RegionButton)
    {
        // Create horizontal layout for button content
        UHorizontalBox* ButtonContent = NewObject<UHorizontalBox>(this);
        
        // Region name
        UTextBlock* NameText = NewObject<UTextBlock>(this);
        NameText->SetText(FText::FromString(Data.RegionName));
        ButtonContent->AddChildToHorizontalBox(NameText);
        
        // Rank badge
        UTextBlock* RankText = NewObject<UTextBlock>(this);
        RankText->SetText(FText::FromString(FString::Printf(TEXT("Rank %d"), Data.ReputationRank)));
        ButtonContent->AddChildToHorizontalBox(RankText);
        
        // Reputation value
        UTextBlock* ValueText = NewObject<UTextBlock>(this);
        ValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Data.ReputationValue)));
        ValueText->SetColorAndOpacity(FSlateColor(Data.ReputationColor));
        ButtonContent->AddChildToHorizontalBox(ValueText);
        
        // Quest indicator
        if (Data.AvailableQuests > 0)
        {
            UTextBlock* QuestIndicator = NewObject<UTextBlock>(this);
            QuestIndicator->SetText(FText::FromString(FString::Printf(TEXT("[%d]"), Data.AvailableQuests)));
            ButtonContent->AddChildToHorizontalBox(QuestIndicator);
        }
        
        // Bind click event
        FScriptDelegate ClickDelegate;
        ClickDelegate.BindUFunction(this, FName("OnRegionButtonClicked"), Data.RegionID);
        RegionButton->OnClicked.Add(ClickDelegate);
        
        RegionListContainer->AddChildToVerticalBox(RegionButton);
    }
}

void UMingReputationWidget::CreateEffectEntry(const FString& EffectDescription)
{
    if (!EffectsContainer)
    {
        return;
    }
    
    UTextBlock* EffectText = NewObject<UTextBlock>(this);
    EffectText->SetText(FText::FromString(TEXT("• ") + EffectDescription));
    EffectsContainer->AddChildToVerticalBox(EffectText);
}

void UMingReputationWidget::ClearRegionList()
{
    if (RegionListContainer)
    {
        RegionListContainer->ClearChildren();
    }
}

void UMingReputationWidget::ClearEffectsList()
{
    if (EffectsContainer)
    {
        EffectsContainer->ClearChildren();
    }
}

void UMingReputationWidget::FilterAndSortData()
{
    // Apply filters
    TArray<FMingReputationDisplayData> FilteredData;
    
    for (const FMingReputationDisplayData& Data : DisplayDataCache)
    {
        // Rank filter
        if (Data.ReputationRank < MinimumRankFilter)
        {
            continue;
        }
        
        // Quests filter
        if (bFilterQuestsOnly && Data.AvailableQuests == 0)
        {
            continue;
        }
        
        FilteredData.Add(Data);
    }
    
    DisplayDataCache = FilteredData;
    
    // Default sort by reputation descending
    SortByReputation(true);
}

FLinearColor UMingReputationWidget::GetReputationColor(float Value) const
{
    // Color based on reputation value
    if (Value < 25.0f)
    {
        return FLinearColor(0.8f, 0.2f, 0.2f); // Red - Hostile
    }
    else if (Value < 50.0f)
    {
        return FLinearColor(0.8f, 0.6f, 0.2f); // Orange - Unfriendly
    }
    else if (Value < 75.0f)
    {
        return FLinearColor(0.8f, 0.8f, 0.2f); // Yellow - Neutral
    }
    else
    {
        return FLinearColor(0.2f, 0.8f, 0.2f); // Green - Friendly
    }
}

FString UMingReputationWidget::GetRankTitle(int32 Rank) const
{
    switch (Rank)
    {
    case 0: return TEXT("Outcast");
    case 1: return TEXT("Stranger");
    case 2: return TEXT("Visitor");
    case 3: return TEXT("Resident");
    case 4: return TEXT("Citizen");
    case 5: return TEXT("Honored");
    case 6: return TEXT("Distinguished");
    case 7: return TEXT("Renowned");
    case 8: return TEXT("Legendary");
    case 9: return TEXT("Hero");
    default: return TEXT("Unknown");
    }
}

float UMingReputationWidget::CalculateNextRankProgress(float Value, int32 CurrentRank) const
{
    // Calculate progress to next rank
    float RankThreshold = CurrentRank * 10.0f; // Simplified
    float NextRankThreshold = (CurrentRank + 1) * 10.0f;
    
    if (Value >= NextRankThreshold)
    {
        return 1.0f;
    }
    
    return (Value - RankThreshold) / (NextRankThreshold - RankThreshold);
}

void UMingReputationWidget::AnimateProgressBar(float TargetValue, float Duration)
{
    if (ReputationProgressBar)
    {
        ReputationProgressBar->SetPercent(TargetValue);
    }
}

void UMingReputationWidget::AnimateRankUp(UWidget* TargetWidget)
{
    // Play rank up animation
}

void UMingReputationWidget::PlayHighlightEffect(FName RegionID)
{
    // Highlight the region entry
}

// Static Utilities

FLinearColor UMingReputationWidget::GetColorForReputationValue(float Value)
{
    if (Value < 25.0f) return FLinearColor(0.8f, 0.2f, 0.2f);
    if (Value < 50.0f) return FLinearColor(0.8f, 0.6f, 0.2f);
    if (Value < 75.0f) return FLinearColor(0.8f, 0.8f, 0.2f);
    return FLinearColor(0.2f, 0.8f, 0.2f);
}

FString UMingReputationWidget::GetRankTitleForLevel(int32 Rank)
{
    switch (Rank)
    {
    case 0: return TEXT("Outcast");
    case 1: return TEXT("Stranger");
    case 2: return TEXT("Visitor");
    case 3: return TEXT("Resident");
    case 4: return TEXT("Citizen");
    case 5: return TEXT("Honored");
    case 6: return TEXT("Distinguished");
    case 7: return TEXT("Renowned");
    case 8: return TEXT("Legendary");
    case 9: return TEXT("Hero");
    default: return TEXT("Unknown");
    }
}

int32 UMingReputationWidget::GetMaxRank()
{
    return 9; // Hero is max rank
}
