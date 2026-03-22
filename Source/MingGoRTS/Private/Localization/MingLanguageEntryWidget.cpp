// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Language Entry Widget Implementation

#include "Localization/MingLanguageEntryWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingLanguageEntry, Log, All);

UMingLanguageEntryWidget::UMingLanguageEntryWidget()
    : LanguageCode(ELanguageCode::en_US)
    , bIsSelected(false)
    , bIsAvailable(true)
    , DefaultFlagPath(TEXT("/Game/UI/Flags/Default"))
{
    // Initialize appearance colors
    SelectedColor = FSlateColor(FLinearColor(0.2f, 0.6f, 1.0f, 1.0f)); // Blue
    UnselectedColor = FSlateColor(FLinearColor(0.8f, 0.8f, 0.8f, 1.0f)); // Gray
    UnavailableColor = FSlateColor(FLinearColor(0.4f, 0.4f, 0.4f, 0.5f)); // Dimmed gray
}

void UMingLanguageEntryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogMingLanguageEntry, Log, TEXT("Constructing Language Entry Widget for language: %d"), (int32)LanguageCode);

    // Get localization system
    if (GEngine)
    {
        LocalizationSystem = FindObject<UMingRTSLocalizationSystem>(GEngine, TEXT("MingRTSLocalizationSystem"));
    }

    // Bind to button click
    if (EntryButton)
    {
        EntryButton->OnClicked.AddDynamic(this, &UMingLanguageEntryWidget::OnEntryClicked);
    }

    // Update appearance
    UpdateAppearance();
}

void UMingLanguageEntryWidget::SetLanguageCode(ELanguageCode InLanguageCode)
{
    if (LanguageCode != InLanguageCode)
    {
        LanguageCode = InLanguageCode;
        UpdateAppearance();
        
        UE_LOG(LogMingLanguageEntry, Log, TEXT("Language code set to: %d"), (int32)LanguageCode);
    }
}

void UMingLanguageEntryWidget::SetIsSelected(bool bSelected)
{
    if (bIsSelected != bSelected)
    {
        bIsSelected = bSelected;
        UpdateVisualState();
        
        UE_LOG(LogMingLanguageEntry, Log, TEXT("Selection state changed to: %s"), bSelected ? TEXT("Selected") : TEXT("Unselected"));
    }
}

void UMingLanguageEntryWidget::SetIsAvailable(bool bAvailable)
{
    if (bIsAvailable != bAvailable)
    {
        bIsAvailable = bAvailable;
        UpdateVisualState();
        
        UE_LOG(LogMingLanguageEntry, Log, TEXT("Availability state changed to: %s"), bAvailable ? TEXT("Available") : TEXT("Unavailable"));
    }
}

FString UMingLanguageEntryWidget::GetLanguageDisplayName() const
{
    if (!LocalizationSystem)
    {
        return TEXT("Unknown");
    }

    FLanguagePack LanguageInfo = LocalizationSystem->GetLanguageInfo(LanguageCode);
    return LanguageInfo.LanguageName;
}

FString UMingLanguageEntryWidget::GetLanguageNativeName() const
{
    if (!LocalizationSystem)
    {
        return TEXT("Unknown");
    }

    FLanguagePack LanguageInfo = LocalizationSystem->GetLanguageInfo(LanguageCode);
    return LanguageInfo.NativeName;
}

FString UMingLanguageEntryWidget::GetFlagTexturePath() const
{
    // Return specific flag path based on language code
    switch (LanguageCode)
    {
    case ELanguageCode::zh_CN:
        return TEXT("/Game/UI/Flags/China");
    case ELanguageCode::zh_TW:
        return TEXT("/Game/UI/Flags/Taiwan");
    case ELanguageCode::en_US:
        return TEXT("/Game/UI/Flags/USA");
    case ELanguageCode::ja_JP:
        return TEXT("/Game/UI/Flags/Japan");
    case ELanguageCode::ko_KR:
        return TEXT("/Game/UI/Flags/Korea");
    case ELanguageCode::fr_FR:
        return TEXT("/Game/UI/Flags/France");
    case ELanguageCode::de_DE:
        return TEXT("/Game/UI/Flags/Germany");
    case ELanguageCode::es_ES:
        return TEXT("/Game/UI/Flags/Spain");
    case ELanguageCode::ru_RU:
        return TEXT("/Game/UI/Flags/Russia");
    case ELanguageCode::ar_SA:
        return TEXT("/Game/UI/Flags/SaudiArabia");
    default:
        return DefaultFlagPath;
    }
}

void UMingLanguageEntryWidget::OnEntryClicked()
{
    if (!bIsAvailable)
    {
        UE_LOG(LogMingLanguageEntry, Warning, TEXT("Clicked on unavailable language entry"));
        return;
    }

    UE_LOG(LogMingLanguageEntry, Log, TEXT("Language entry clicked: %d"), (int32)LanguageCode);

    // Fire selection event
    OnLanguageSelected.Broadcast(LanguageCode, this);

    // Update visual feedback
    SetIsSelected(true);
}

void UMingLanguageEntryWidget::UpdateAppearance()
{
    UpdateTextDisplay();
    UpdateFlagDisplay();
    UpdateVisualState();
}

void UMingLanguageEntryWidget::UpdateVisualState()
{
    // Update selection indicator
    UpdateSelectionIndicator();

    // Update availability overlay
    if (AvailabilityOverlay)
    {
        AvailabilityOverlay->SetVisibility(bIsAvailable ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    }

    // Update button state
    if (EntryButton)
    {
        EntryButton->SetIsEnabled(bIsAvailable);
    }

    // Update text colors based on state
    FSlateColor TargetColor;
    if (!bIsAvailable)
    {
        TargetColor = UnavailableColor;
    }
    else if (bIsSelected)
    {
        TargetColor = SelectedColor;
    }
    else
    {
        TargetColor = UnselectedColor;
    }

    if (LanguageNameText)
    {
        LanguageNameText->SetColorAndOpacity(TargetColor);
    }

    if (NativeNameText)
    {
        NativeNameText->SetColorAndOpacity(TargetColor);
    }
}

void UMingLanguageEntryWidget::UpdateTextDisplay()
{
    if (LanguageNameText)
    {
        FString DisplayName = GetLanguageDisplayName();
        LanguageNameText->SetText(FText::FromString(DisplayName));
    }

    if (NativeNameText)
    {
        FString NativeName = GetLanguageNativeName();
        NativeNameText->SetText(FText::FromString(NativeName));
    }
}

void UMingLanguageEntryWidget::UpdateFlagDisplay()
{
    if (!FlagImage)
    {
        return;
    }

    FString FlagPath = GetFlagTexturePath();
    
    // Check if flag texture exists
    if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*FlagPath))
    {
        // TODO: Load and set the flag texture
        // FlagImage->SetBrushFromTexture(LoadedTexture);
        UE_LOG(LogMingLanguageEntry, Log, TEXT("Flag texture path: %s"), *FlagPath);
    }
    else
    {
        UE_LOG(LogMingLanguageEntry, Warning, TEXT("Flag texture not found: %s"), *FlagPath);
    }
}

void UMingLanguageEntryWidget::UpdateSelectionIndicator()
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
