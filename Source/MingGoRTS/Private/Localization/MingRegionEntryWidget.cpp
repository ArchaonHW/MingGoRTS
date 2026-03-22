// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Region Entry Widget Implementation

#include "Localization/MingRegionEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingRegionEntry, Log, All);

UMingRegionEntryWidget::UMingRegionEntryWidget()
    : RegionCode(ECulturalRegion::Global)
    , bIsSelected(false)
    , DefaultIconPath(TEXT("/Game/UI/Regions/Default"))
{
    // Initialize appearance colors
    SelectedColor = FSlateColor(FLinearColor(0.2f, 0.6f, 1.0f, 1.0f)); // Blue
    UnselectedColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.0f)); // Gray
}

void UMingRegionEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogMingRegionEntry, Log, TEXT("Constructing Region Entry Widget for region: %d"), (int32)RegionCode);

    // Get cultural adaptation system
    if (GEngine)
    {
        CulturalSystem = FindObject<UMingRTSCulturalAdaptationSystem>(GEngine, TEXT("MingRTSCulturalAdaptationSystem"));
    }

    // Bind to button click
    if (EntryButton)
    {
        EntryButton->OnClicked.AddDynamic(this, &UMingRegionEntryWidget::OnEntryClicked);
    }

    // Update appearance
    UpdateAppearance();
}

void UMingRegionEntryWidget::SetRegionCode(ECulturalRegion InRegionCode)
{
    if (RegionCode != InRegionCode)
    {
        RegionCode = InRegionCode;
        UpdateAppearance();
        
        UE_LOG(LogMingRegionEntry, Log, TEXT("Region code set to: %d"), (int32)RegionCode);
    }
}

void UMingRegionEntryWidget::SetIsSelected(bool bSelected)
{
    if (bIsSelected != bSelected)
    {
        bIsSelected = bSelected;
        UpdateVisualState();
        
        UE_LOG(LogMingRegionEntry, Log, TEXT("Selection state changed to: %s"), bSelected ? TEXT("Selected") : TEXT("Unselected"));
    }
}

FString UMingRegionEntryWidget::GetRegionDisplayName() const
{
    if (!CulturalSystem)
    {
        return TEXT("Unknown Region");
    }

    return CulturalSystem->GetRegionDisplayName(RegionCode);
}

FString UMingRegionEntryWidget::GetRegionDescription() const
{
    // Return description based on region code
    switch (RegionCode)
    {
    case ECulturalRegion::EastAsia:
        return TEXT("East Asian cultural region with traditional values");
    case ECulturalRegion::SoutheastAsia:
        return TEXT("Southeast Asian cultural region with diverse traditions");
    case ECulturalRegion::SouthAsia:
        return TEXT("South Asian cultural region with rich heritage");
    case ECulturalRegion::MiddleEast:
        return TEXT("Middle Eastern cultural region with Islamic influence");
    case ECulturalRegion::WesternEurope:
        return TEXT("Western European cultural region with modern values");
    case ECulturalRegion::EasternEurope:
        return TEXT("Eastern European cultural region with Slavic heritage");
    case ECulturalRegion::NorthAmerica:
        return TEXT("North American cultural region with Western influence");
    case ECulturalRegion::LatinAmerica:
        return TEXT("Latin American cultural region with vibrant traditions");
    case ECulturalRegion::Oceania:
        return TEXT("Oceanian cultural region with Pacific heritage");
    case ECulturalRegion::Africa:
        return TEXT("African cultural region with diverse traditions");
    case ECulturalRegion::Global:
        return TEXT("Global cultural region with balanced content");
    default:
        return TEXT("Unknown cultural region");
    }
}

FString UMingRegionEntryWidget::GetRegionIconPath() const
{
    // Return specific icon path based on region code
    switch (RegionCode)
    {
    case ECulturalRegion::EastAsia:
        return TEXT("/Game/UI/Regions/EastAsia");
    case ECulturalRegion::SoutheastAsia:
        return TEXT("/Game/UI/Regions/SoutheastAsia");
    case ECulturalRegion::SouthAsia:
        return TEXT("/Game/UI/Regions/SouthAsia");
    case ECulturalRegion::MiddleEast:
        return TEXT("/Game/UI/Regions/MiddleEast");
    case ECulturalRegion::WesternEurope:
        return TEXT("/Game/UI/Regions/WesternEurope");
    case ECulturalRegion::EasternEurope:
        return TEXT("/Game/UI/Regions/EasternEurope");
    case ECulturalRegion::NorthAmerica:
        return TEXT("/Game/UI/Regions/NorthAmerica");
    case ECulturalRegion::LatinAmerica:
        return TEXT("/Game/UI/Regions/LatinAmerica");
    case ECulturalRegion::Oceania:
        return TEXT("/Game/UI/Regions/Oceania");
    case ECulturalRegion::Africa:
        return TEXT("/Game/UI/Regions/Africa");
    case ECulturalRegion::Global:
        return TEXT("/Game/UI/Regions/Global");
    default:
        return DefaultIconPath;
    }
}

void UMingRegionEntryWidget::OnEntryClicked()
{
    UE_LOG(LogMingRegionEntry, Log, TEXT("Region entry clicked: %d"), (int32)RegionCode);

    // Fire selection event
    OnRegionSelected.Broadcast(RegionCode, this);

    // Update visual feedback
    SetIsSelected(true);
}

void UMingRegionEntryWidget::UpdateAppearance()
{
    UpdateTextDisplay();
    UpdateIconDisplay();
    UpdateVisualState();
}

void UMingRegionEntryWidget::UpdateVisualState()
{
    // Update selection indicator
    UpdateSelectionIndicator();

    // Update text colors based on state
    FSlateColor TargetColor = bIsSelected ? SelectedColor : UnselectedColor;

    if (RegionNameText)
    {
        RegionNameText->SetColorAndOpacity(TargetColor);
    }

    if (RegionDescriptionText)
    {
        RegionDescriptionText->SetColorAndOpacity(TargetColor);
    }
}

void UMingRegionEntryWidget::UpdateTextDisplay()
{
    if (RegionNameText)
    {
        FString DisplayName = GetRegionDisplayName();
        RegionNameText->SetText(FText::FromString(DisplayName));
    }

    if (RegionDescriptionText)
    {
        FString Description = GetRegionDescription();
        RegionDescriptionText->SetText(FText::FromString(Description));
    }
}

void UMingRegionEntryWidget::UpdateIconDisplay()
{
    if (!RegionIconImage)
    {
        return;
    }

    FString IconPath = GetRegionIconPath();
    
    // Check if icon texture exists
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*IconPath))
    {
        // TODO: Load and set the icon texture
        // RegionIconImage->SetBrushFromTexture(LoadedTexture);
        UE_LOG(LogMingRegionEntry, Log, TEXT("Region icon texture path: %s"), *IconPath);
    }
    else
    {
        UE_LOG(LogMingRegionEntry, Warning, TEXT("Region icon texture not found: %s"), *IconPath);
    }
}

void UMingRegionEntryWidget::UpdateSelectionIndicator()
{
    if (!SelectionIndicator)
    {
        return;
    }

    if (bIsSelected)
    {
        SelectionIndicator->SetVisibility(ESlateVisibility::Visible);
        // TODO: Set selected appearance
    }
    else
    {
        SelectionIndicator->SetVisibility(ESlateVisibility::Hidden);
    }
}
