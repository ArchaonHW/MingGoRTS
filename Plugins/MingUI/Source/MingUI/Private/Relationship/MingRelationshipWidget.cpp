#include "Relationship/MingRelationshipWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Border.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateColor.h"
#include "Animation/WidgetAnimation.h"
#include "Animation/UMGSequencePlayer.h"
#include "Engine/Texture2D.h"
#include "MingPersonal/Public/Relationship/MingRelationshipManager.h"
#include "MingPersonal/Public/Relationship/MingRelationshipTypes.h"
#include "MingAudio/Public/MingMetaSoundsSystem.h"

UMingRelationshipWidget::UMingRelationshipWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , RelationshipManager(nullptr)
    , CurrentViewMode(EMingRelationshipViewMode::Overview)
    , TypeFilter(EMingRelationshipType::None)
    , MinValueFilter(0.0f)
    , MaxValueFilter(100.0f)
{
}

void UMingRelationshipWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    // Bind button events
    if (OverviewButton)
    {
        OverviewButton->OnClicked.AddDynamic(this, &UMingRelationshipWidget::OnOverviewButtonClicked);
    }
    if (DetailsButton)
    {
        DetailsButton->OnClicked.AddDynamic(this, &UMingRelationshipWidget::OnDetailsButtonClicked);
    }
    if (GraphButton)
    {
        GraphButton->OnClicked.AddDynamic(this, &UMingRelationshipWidget::OnGraphButtonClicked);
    }
    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UMingRelationshipWidget::OnCloseButtonClicked);
    }
    
    // Initialize display
    RefreshDisplay();
}

void UMingRelationshipWidget::NativeDestruct()
{
    // Unbind events
    if (OverviewButton)
    {
        OverviewButton->OnClicked.RemoveAll(this);
    }
    if (DetailsButton)
    {
        DetailsButton->OnClicked.RemoveAll(this);
    }
    if (GraphButton)
    {
        GraphButton->OnClicked.RemoveAll(this);
    }
    if (CloseButton)
    {
        CloseButton->OnClicked.RemoveAll(this);
    }
    
    Super::NativeDestruct();
}

void UMingRelationshipWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    
    // Update animations or real-time elements here
}

void UMingRelationshipWidget::InitializeWidget(UMingRelationshipManager* InRelationshipManager)
{
    RelationshipManager = InRelationshipManager;
    RefreshDisplay();
}

void UMingRelationshipWidget::RefreshDisplay()
{
    if (!RelationshipManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("RelationshipWidget: No RelationshipManager set"));
        return;
    }
    
    // Refresh data cache
    DisplayDataCache = GetRelationshipDisplayData();
    
    // Apply filters
    FilterDisplayData();
    SortDisplayData();
    
    // Update UI
    PopulateCharacterList();
    
    if (!SelectedCharacterID.IsNone())
    {
        UpdateCharacterDetails();
    }
    
    OnRefreshRequested.Broadcast();
}

void UMingRelationshipWidget::SetViewMode(EMingRelationshipViewMode NewMode)
{
    if (CurrentViewMode != NewMode)
    {
        CurrentViewMode = NewMode;
        
        // Update button states
        if (OverviewButton)
        {
            OverviewButton->SetIsEnabled(CurrentViewMode != EMingRelationshipViewMode::Overview);
        }
        if (DetailsButton)
        {
            DetailsButton->SetIsEnabled(CurrentViewMode != EMingRelationshipViewMode::CharacterDetails);
        }
        if (GraphButton)
        {
            GraphButton->SetIsEnabled(CurrentViewMode != EMingRelationshipViewMode::NetworkGraph);
        }
        
        // Refresh display for new mode
        RefreshDisplay();
        OnViewModeChanged.Broadcast();
    }
}

void UMingRelationshipWidget::SelectCharacter(FName CharacterID)
{
    if (SelectedCharacterID != CharacterID)
    {
        SelectedCharacterID = CharacterID;
        UpdateCharacterDetails();
        OnCharacterSelected.Broadcast(CharacterID);
    }
}

void UMingRelationshipWidget::ClearCharacterSelection()
{
    SelectedCharacterID = FName();
    if (SelectedCharacterName)
    {
        SelectedCharacterName->SetText(FText::FromString(TEXT("Select a Character")));
    }
    if (RelationshipProgressBar)
    {
        RelationshipProgressBar->SetPercent(0.0f);
    }
}

TArray<FMingRelationshipDisplayData> UMingRelationshipWidget::GetRelationshipDisplayData() const
{
    TArray<FMingRelationshipDisplayData> Result;
    
    if (!RelationshipManager)
    {
        return Result;
    }
    
    // Get all relationships from manager
    // This is a simplified version - actual implementation would query the manager
    TArray<FName> CharacterIDs = RelationshipManager->GetAllCharacterIDs();
    
    for (const FName& CharacterID : CharacterIDs)
    {
        FMingRelationshipDisplayData Data;
        Data.CharacterID = CharacterID;
        Data.CharacterName = RelationshipManager->GetCharacterName(CharacterID);
        Data.RelationshipType = RelationshipManager->GetRelationshipType(CharacterID);
        Data.RelationshipValue = RelationshipManager->GetRelationshipValue(CharacterID);
        Data.NormalizedValue = Data.RelationshipValue / 100.0f;
        Data.RelationshipDescription = GetRelationshipDescription(Data.RelationshipValue);
        Data.RelationshipColor = GetRelationshipColor(Data.RelationshipValue);
        Data.bHasActiveQuest = RelationshipManager->HasActiveQuest(CharacterID);
        Data.bCanInteract = RelationshipManager->CanInteract(CharacterID);
        
        Result.Add(Data);
    }
    
    return Result;
}

FMingRelationshipDisplayData UMingRelationshipWidget::GetCharacterDisplayData(FName CharacterID) const
{
    FMingRelationshipDisplayData Result;
    
    for (const FMingRelationshipDisplayData& Data : DisplayDataCache)
    {
        if (Data.CharacterID == CharacterID)
        {
            Result = Data;
            break;
        }
    }
    
    return Result;
}

void UMingRelationshipWidget::SetFilterByType(EMingRelationshipType TypeFilter)
{
    this->TypeFilter = TypeFilter;
    RefreshDisplay();
}

void UMingRelationshipWidget::SetFilterByValueRange(float MinValue, float MaxValue)
{
    MinValueFilter = FMath::Clamp(MinValue, 0.0f, 100.0f);
    MaxValueFilter = FMath::Clamp(MaxValue, 0.0f, 100.0f);
    RefreshDisplay();
}

void UMingRelationshipWidget::SetSearchFilter(const FString& SearchText)
{
    SearchFilter = SearchText;
    RefreshDisplay();
}

void UMingRelationshipWidget::ClearAllFilters()
{
    TypeFilter = EMingRelationshipType::None;
    MinValueFilter = 0.0f;
    MaxValueFilter = 100.0f;
    SearchFilter.Empty();
    RefreshDisplay();
}

void UMingRelationshipWidget::SortByName(bool bAscending)
{
    DisplayDataCache.Sort([bAscending](const FMingRelationshipDisplayData& A, const FMingRelationshipDisplayData& B)
    {
        if (bAscending)
        {
            return A.CharacterName < B.CharacterName;
        }
        return A.CharacterName > B.CharacterName;
    });
    
    PopulateCharacterList();
}

void UMingRelationshipWidget::SortByRelationshipValue(bool bAscending)
{
    DisplayDataCache.Sort([bAscending](const FMingRelationshipDisplayData& A, const FMingRelationshipDisplayData& B)
    {
        if (bAscending)
        {
            return A.RelationshipValue < B.RelationshipValue;
        }
        return A.RelationshipValue > B.RelationshipValue;
    });
    
    PopulateCharacterList();
}

void UMingRelationshipWidget::SortByType()
{
    DisplayDataCache.Sort([](const FMingRelationshipDisplayData& A, const FMingRelationshipDisplayData& B)
    {
        return static_cast<uint8>(A.RelationshipType) < static_cast<uint8>(B.RelationshipType);
    });
    
    PopulateCharacterList();
}

void UMingRelationshipWidget::OpenInteractionMenu(FName CharacterID)
{
    // Implementation would open an interaction menu widget
    SelectCharacter(CharacterID);
}

void UMingRelationshipWidget::CloseInteractionMenu()
{
    // Close any open interaction menus
}

void UMingRelationshipWidget::ShowRelationshipHistory(FName CharacterID)
{
    // Display relationship history
    // Could open a modal or switch to history view
}

void UMingRelationshipWidget::HighlightRelationshipChange(FName CharacterID, float DeltaValue)
{
    // Find the character entry and highlight it
    // Play animation based on positive or negative change
}

void UMingRelationshipWidget::PlayRelationshipUpdateAnimation(FName CharacterID)
{
    // Play update animation for the character entry
}

void UMingRelationshipWidget::PlayRelationshipSound(EMingRelationshipType RelationshipType, float Value)
{
    // Play appropriate sound based on relationship type and value
    // Could use UMingMetaSoundsSystem
}

// Button Handlers

void UMingRelationshipWidget::OnOverviewButtonClicked()
{
    SetViewMode(EMingRelationshipViewMode::Overview);
}

void UMingRelationshipWidget::OnDetailsButtonClicked()
{
    SetViewMode(EMingRelationshipViewMode::CharacterDetails);
}

void UMingRelationshipWidget::OnGraphButtonClicked()
{
    SetViewMode(EMingRelationshipViewMode::NetworkGraph);
}

void UMingRelationshipWidget::OnCloseButtonClicked()
{
    // Close the widget
    RemoveFromParent();
}

void UMingRelationshipWidget::OnCharacterButtonClicked(FName CharacterID)
{
    SelectCharacter(CharacterID);
    OnRelationshipClicked.Broadcast(CharacterID, GetCharacterDisplayData(CharacterID).RelationshipValue);
}

// Internal Functions

void UMingRelationshipWidget::PopulateCharacterList()
{
    ClearCharacterList();
    
    if (!CharacterListContainer)
    {
        return;
    }
    
    for (const FMingRelationshipDisplayData& Data : DisplayDataCache)
    {
        CreateCharacterEntry(Data);
    }
}

void UMingRelationshipWidget::UpdateCharacterDetails()
{
    FMingRelationshipDisplayData Data = GetCharacterDisplayData(SelectedCharacterID);
    
    if (SelectedCharacterName)
    {
        SelectedCharacterName->SetText(FText::FromString(Data.CharacterName));
    }
    
    if (RelationshipProgressBar)
    {
        AnimateProgressBar(Data.NormalizedValue, 0.3f);
    }
    
    if (RelationshipValueText)
    {
        RelationshipValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Data.RelationshipValue)));
    }
    
    if (RelationshipDescription)
    {
        RelationshipDescription->SetText(FText::FromString(Data.RelationshipDescription));
    }
}

void UMingRelationshipWidget::UpdateProgressBar(float Value)
{
    if (RelationshipProgressBar)
    {
        RelationshipProgressBar->SetPercent(Value);
    }
}

FLinearColor UMingRelationshipWidget::GetRelationshipColor(float Value) const
{
    // Color gradient from red (hostile) to green (friendly)
    if (Value < 30.0f)
    {
        return FLinearColor(1.0f, 0.2f, 0.2f); // Red
    }
    else if (Value < 60.0f)
    {
        return FLinearColor(1.0f, 1.0f, 0.2f); // Yellow
    }
    else
    {
        return FLinearColor(0.2f, 1.0f, 0.2f); // Green
    }
}

FString UMingRelationshipWidget::GetRelationshipDescription(float Value) const
{
    if (Value >= 80.0f) return TEXT("Trusted Ally");
    if (Value >= 60.0f) return TEXT("Friendly");
    if (Value >= 40.0f) return TEXT("Neutral");
    if (Value >= 20.0f) return TEXT("Suspicious");
    return TEXT("Hostile");
}

void UMingRelationshipWidget::FilterDisplayData()
{
    TArray<FMingRelationshipDisplayData> FilteredData;
    
    for (const FMingRelationshipDisplayData& Data : DisplayDataCache)
    {
        // Type filter
        if (TypeFilter != EMingRelationshipType::None && Data.RelationshipType != TypeFilter)
        {
            continue;
        }
        
        // Value range filter
        if (Data.RelationshipValue < MinValueFilter || Data.RelationshipValue > MaxValueFilter)
        {
            continue;
        }
        
        // Search filter
        if (!SearchFilter.IsEmpty() && !Data.CharacterName.Contains(SearchFilter))
        {
            continue;
        }
        
        FilteredData.Add(Data);
    }
    
    DisplayDataCache = FilteredData;
}

void UMingRelationshipWidget::SortDisplayData()
{
    // Default sort by relationship value descending
    SortByRelationshipValue(false);
}

void UMingRelationshipWidget::CreateCharacterEntry(const FMingRelationshipDisplayData& Data)
{
    // Create a button for the character
    UButton* CharacterButton = NewObject<UButton>(this);
    if (CharacterButton)
    {
        // Create a horizontal box for the button content
        UHorizontalBox* ButtonContent = NewObject<UHorizontalBox>(this);
        
        // Add character portrait image
        UImage* PortraitImage = NewObject<UImage>(this);
        if (Data.CharacterPortrait)
        {
            PortraitImage->SetBrushFromTexture(Data.CharacterPortrait);
        }
        ButtonContent->AddChildToHorizontalBox(PortraitImage);
        
        // Add character name text
        UTextBlock* NameText = NewObject<UTextBlock>(this);
        NameText->SetText(FText::FromString(Data.CharacterName));
        ButtonContent->AddChildToHorizontalBox(NameText);
        
        // Add relationship value text
        UTextBlock* ValueText = NewObject<UTextBlock>(this);
        ValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), Data.RelationshipValue)));
        ValueText->SetColorAndOpacity(FSlateColor(Data.RelationshipColor));
        ButtonContent->AddChildToHorizontalBox(ValueText);
        
        // Bind click event
        FScriptDelegate ClickDelegate;
        ClickDelegate.BindUFunction(this, FName("OnCharacterButtonClicked"), Data.CharacterID);
        CharacterButton->OnClicked.Add(ClickDelegate);
        
        // Add to container
        CharacterListContainer->AddChildToVerticalBox(CharacterButton);
        
        // Animate entry
        AnimateCharacterEntry(CharacterButton);
    }
}

void UMingRelationshipWidget::ClearCharacterList()
{
    if (CharacterListContainer)
    {
        CharacterListContainer->ClearChildren();
    }
}

void UMingRelationshipWidget::DrawNetworkGraph()
{
    // Implementation for network graph visualization
    // Would use UE's drawing API or a custom widget
}

void UMingRelationshipWidget::CalculateNodePositions()
{
    // Calculate positions for network graph nodes
}

void UMingRelationshipWidget::DrawConnectionLines()
{
    // Draw lines between connected characters
}

void UMingRelationshipWidget::AnimateProgressBar(float TargetValue, float Duration)
{
    if (RelationshipProgressBar)
    {
        // Use UMG animation system
        // This is a simplified version - actual implementation would use proper animation
        RelationshipProgressBar->SetPercent(TargetValue);
    }
}

void UMingRelationshipWidget::AnimateCharacterEntry(UWidget* EntryWidget)
{
    // Play entry animation for the character entry
}

void UMingRelationshipWidget::PlayHighlightAnimation(UWidget* TargetWidget)
{
    // Play highlight animation
}

// Static Utility Functions

FLinearColor UMingRelationshipWidget::GetColorForRelationshipType(EMingRelationshipType Type)
{
    switch (Type)
    {
    case EMingRelationshipType::Family:
        return FLinearColor(0.8f, 0.4f, 0.8f); // Purple
    case EMingRelationshipType::Friend:
        return FLinearColor(0.4f, 0.8f, 0.4f); // Green
    case EMingRelationshipType::Rival:
        return FLinearColor(0.8f, 0.4f, 0.4f); // Red
    case EMingRelationshipType::Ally:
        return FLinearColor(0.4f, 0.4f, 0.8f); // Blue
    case EMingRelationshipType::Enemy:
        return FLinearColor(0.2f, 0.2f, 0.2f); // Dark Gray
    default:
        return FLinearColor(0.8f, 0.8f, 0.8f); // Light Gray
    }
}

FString UMingRelationshipWidget::GetRelationshipTypeDisplayName(EMingRelationshipType Type)
{
    switch (Type)
    {
    case EMingRelationshipType::Family:
        return TEXT("Family");
    case EMingRelationshipType::Friend:
        return TEXT("Friend");
    case EMingRelationshipType::Rival:
        return TEXT("Rival");
    case EMingRelationshipType::Ally:
        return TEXT("Ally");
    case EMingRelationshipType::Enemy:
        return TEXT("Enemy");
    default:
        return TEXT("Neutral");
    }
}

UTexture2D* UMingRelationshipWidget::GetDefaultPortrait()
{
    // Return a default portrait texture
    // In actual implementation, this would load from a specific path
    return nullptr;
}
