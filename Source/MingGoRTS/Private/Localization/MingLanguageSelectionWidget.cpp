// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Language Selection UI Widget Implementation

#include "Localization/MingLanguageSelectionWidget.h"
#include "Components/ListView.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Input/SSlider.h"
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Images/SImage.h"
#include "Engine/Engine.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingLanguageSelection, Log, All);

UMingLanguageSelectionWidget::UMingLanguageSelectionWidget()
    : SelectedLanguage(ELanguageCode::zh_CN)
    , SelectedRegion(ECulturalRegion::EastAsia)
    , OriginalLanguage(ELanguageCode::zh_CN)
    , OriginalRegion(ECulturalRegion::EastAsia)
    , bSettingsModified(false)
    , bRestartRequired(false)
{
    // Initialize default flag paths
    LanguageFlagPaths.Add(ELanguageCode::zh_CN, TEXT("/Game/UI/Flags/China"));
    LanguageFlagPaths.Add(ELanguageCode::zh_TW, TEXT("/Game/UI/Flags/Taiwan"));
    LanguageFlagPaths.Add(ELanguageCode::en_US, TEXT("/Game/UI/Flags/USA"));
    LanguageFlagPaths.Add(ELanguageCode::ja_JP, TEXT("/Game/UI/Flags/Japan"));
    LanguageFlagPaths.Add(ELanguageCode::ko_KR, TEXT("/Game/UI/Flags/Korea"));
    LanguageFlagPaths.Add(ELanguageCode::fr_FR, TEXT("/Game/UI/Flags/France"));
    LanguageFlagPaths.Add(ELanguageCode::de_DE, TEXT("/Game/UI/Flags/Germany"));
    LanguageFlagPaths.Add(ELanguageCode::es_ES, TEXT("/Game/UI/Flags/Spain"));
    LanguageFlagPaths.Add(ELanguageCode::ru_RU, TEXT("/Game/UI/Flags/Russia"));
    LanguageFlagPaths.Add(ELanguageCode::ar_SA, TEXT("/Game/UI/Flags/SaudiArabia"));

    // Initialize default region icon paths
    RegionIconPaths.Add(ECulturalRegion::EastAsia, TEXT("/Game/UI/Regions/EastAsia"));
    RegionIconPaths.Add(ECulturalRegion::SoutheastAsia, TEXT("/Game/UI/Regions/SoutheastAsia"));
    RegionIconPaths.Add(ECulturalRegion::SouthAsia, TEXT("/Game/UI/Regions/SouthAsia"));
    RegionIconPaths.Add(ECulturalRegion::MiddleEast, TEXT("/Game/UI/Regions/MiddleEast"));
    RegionIconPaths.Add(ECulturalRegion::WesternEurope, TEXT("/Game/UI/Regions/WesternEurope"));
    RegionIconPaths.Add(ECulturalRegion::EasternEurope, TEXT("/Game/UI/Regions/EasternEurope"));
    RegionIconPaths.Add(ECulturalRegion::NorthAmerica, TEXT("/Game/UI/Regions/NorthAmerica"));
    RegionIconPaths.Add(ECulturalRegion::LatinAmerica, TEXT("/Game/UI/Regions/LatinAmerica"));
    RegionIconPaths.Add(ECulturalRegion::Oceania, TEXT("/Game/UI/Regions/Oceania"));
    RegionIconPaths.Add(ECulturalRegion::Africa, TEXT("/Game/UI/Regions/Africa"));
    RegionIconPaths.Add(ECulturalRegion::Global, TEXT("/Game/UI/Regions/Global"));

    // Initialize preview text keys
    PreviewTextKeys.Add(TEXT("UI.OK"));
    PreviewTextKeys.Add(TEXT("UI.Cancel"));
    PreviewTextKeys.Add(TEXT("UI.Yes"));
    PreviewTextKeys.Add(TEXT("UI.No"));
    PreviewTextKeys.Add(TEXT("Game.Title"));
    PreviewTextKeys.Add(TEXT("Unit.Worker.Name"));
}

void UMingLanguageSelectionWidget::NativeConstruct()
{
    Super::NativeConstruct();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Initializing Language Selection Widget"));

    // Initialize systems
    InitializeLanguageSelection();

    // Bind to UI elements
    if (ApplyButton)
    {
        ApplyButton->OnClicked.AddDynamic(this, &UMingLanguageSelectionWidget::ApplySettings);
    }

    if (CancelButton)
    {
        CancelButton->OnClicked.AddDynamic(this, &UMingLanguageSelectionWidget::CancelChanges);
    }

    if (ResetButton)
    {
        ResetButton->OnClicked.AddDynamic(this, &UMingLanguageSelectionWidget::ResetToDefaults);
    }

    // Bind to localization events
    BindToLocalizationEvents();

    // Load saved settings
    LoadSettings();

    // Update UI
    UpdateUIElements();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Language Selection Widget initialized successfully"));
}

void UMingLanguageSelectionWidget::NativeDestruct()
{
    UnbindFromLocalizationEvents();
    Super::NativeDestruct();
}

void UMingLanguageSelectionWidget::InitializeLanguageSelection()
{
    // Get localization systems
    if (GEngine)
    {
        // Try to get existing systems or create new ones
        LocalizationSystem = FindObject<UMingRTSLocalizationSystem>(GEngine, TEXT("MingRTSLocalizationSystem"));
        if (!LocalizationSystem)
        {
            LocalizationSystem = NewObject<UMingRTSLocalizationSystem>(GEngine);
            LocalizationSystem->InitializeLocalizationSystem();
        }

        CulturalSystem = FindObject<UMingRTSCulturalAdaptationSystem>(GEngine, TEXT("MingRTSCulturalAdaptationSystem"));
        if (!CulturalSystem)
        {
            CulturalSystem = NewObject<UMingRTSCulturalAdaptationSystem>(GEngine);
            CulturalSystem->InitializeCulturalSystem();
        }
    }

    // Refresh available options
    RefreshLanguageList();
    RefreshRegionList();

    // Set current selections
    if (LocalizationSystem)
    {
        SelectedLanguage = LocalizationSystem->GetCurrentLanguage();
        OriginalLanguage = SelectedLanguage;
    }

    if (CulturalSystem)
    {
        SelectedRegion = CulturalSystem->GetCurrentRegion();
        OriginalRegion = SelectedRegion;
    }
}

void UMingLanguageSelectionWidget::RefreshLanguageList()
{
    if (!LocalizationSystem)
    {
        return;
    }

    AvailableLanguages = LocalizationSystem->GetSupportedLanguages();
    
    UE_LOG(LogMingLanguageSelection, Log, TEXT("Refreshed language list with %d languages"), AvailableLanguages.Num());

    // Update language list view if available
    if (LanguageListView)
    {
        LanguageListView->ClearListItems();
        // TODO: Add language entry widgets to the list view
    }
}

void UMingLanguageSelectionWidget::RefreshRegionList()
{
    if (!CulturalSystem)
    {
        return;
    }

    AvailableRegions = CulturalSystem->GetAllRegions();
    
    UE_LOG(LogMingLanguageSelection, Log, TEXT("Refreshed region list with %d regions"), AvailableRegions.Num());

    // Update region list view if available
    if (RegionListView)
    {
        RegionListView->ClearListItems();
        // TODO: Add region entry widgets to the list view
    }
}

void UMingLanguageSelectionWidget::SetCurrentLanguage(ELanguageCode LanguageCode)
{
    if (SelectedLanguage != LanguageCode)
    {
        SelectedLanguage = LanguageCode;
        bSettingsModified = true;
        
        // Check if restart is needed
        if (LocalizationSystem)
        {
            ELanguageCode CurrentLang = LocalizationSystem->GetCurrentLanguage();
            bRestartRequired = (CurrentLang != LanguageCode);
        }

        UpdateLanguagePreview();
        UpdateUIElements();

        UE_LOG(LogMingLanguageSelection, Log, TEXT("Language selected: %d"), (int32)LanguageCode);
    }
}

void UMingLanguageSelectionWidget::SetCurrentRegion(ECulturalRegion Region)
{
    if (SelectedRegion != Region)
    {
        SelectedRegion = Region;
        bSettingsModified = true;

        UpdateRegionPreview();
        UpdateUIElements();

        UE_LOG(LogMingLanguageSelection, Log, TEXT("Region selected: %d"), (int32)Region);
    }
}

FString UMingLanguageSelectionWidget::GetCurrentLanguageDisplayName() const
{
    if (!LocalizationSystem)
    {
        return TEXT("Unknown");
    }

    FLanguagePack LanguageInfo = LocalizationSystem->GetLanguageInfo(SelectedLanguage);
    return LanguageInfo.LanguageName;
}

FString UMingLanguageSelectionWidget::GetCurrentRegionDisplayName() const
{
    if (!CulturalSystem)
    {
        return TEXT("Unknown");
    }

    return CulturalSystem->GetRegionDisplayName(SelectedRegion);
}

bool UMingLanguageSelectionWidget::IsLanguageAvailable(ELanguageCode LanguageCode) const
{
    if (!LocalizationSystem)
    {
        return false;
    }

    return LocalizationSystem->IsLanguageLoaded(LanguageCode);
}

bool UMingLanguageSelectionWidget::IsRegionAvailable(ECulturalRegion Region) const
{
    return AvailableRegions.Contains(Region);
}

FString UMingLanguageSelectionWidget::GetLanguageFlagPath(ELanguageCode LanguageCode) const
{
    const FString* FlagPath = LanguageFlagPaths.Find(LanguageCode);
    return FlagPath ? *FlagPath : FString();
}

FString UMingLanguageSelectionWidget::GetRegionIconPath(ECulturalRegion Region) const
{
    const FString* IconPath = RegionIconPaths.Find(Region);
    return IconPath ? *IconPath : FString();
}

void UMingLanguageSelectionWidget::ApplySettings()
{
    if (!ValidateSettings())
    {
        UE_LOG(LogMingLanguageSelection, Warning, TEXT("Settings validation failed"));
        return;
    }

    // Apply language change
    if (LocalizationSystem && SelectedLanguage != LocalizationSystem->GetCurrentLanguage())
    {
        LocalizationSystem->SetLanguage(SelectedLanguage);
    }

    // Apply region change
    if (CulturalSystem && SelectedRegion != CulturalSystem->GetCurrentRegion())
    {
        CulturalSystem->SetPlayerRegion(SelectedRegion);
    }

    // Save settings
    SaveSettings();

    // Reset modification flag
    bSettingsModified = false;

    // Show restart dialog if needed
    if (bRestartRequired)
    {
        ShowRestartRequiredDialog();
    }

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Settings applied successfully"));
}

void UMingLanguageSelectionWidget::ResetToDefaults()
{
    // Reset to system defaults
    if (LocalizationSystem)
    {
        ELanguageCode SystemLanguage = LocalizationSystem->GetCurrentLanguage();
        SelectedLanguage = SystemLanguage;
    }

    if (CulturalSystem)
    {
        ECulturalRegion SystemRegion = CulturalSystem->DetectRegionFromSystem();
        SelectedRegion = SystemRegion;
    }

    bSettingsModified = true;
    UpdateUIElements();
    UpdateLanguagePreview();
    UpdateRegionPreview();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Settings reset to defaults"));
}

void UMingLanguageSelectionWidget::CancelChanges()
{
    // Restore original settings
    SelectedLanguage = OriginalLanguage;
    SelectedRegion = OriginalRegion;
    bSettingsModified = false;
    bRestartRequired = false;

    UpdateUIElements();
    UpdateLanguagePreview();
    UpdateRegionPreview();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Settings changes cancelled"));
}

void UMingLanguageSelectionWidget::BindToLocalizationEvents()
{
    if (LocalizationSystem)
    {
        LocalizationSystem->OnLanguageChanged.AddDynamic(this, &UMingLanguageSelectionWidget::OnLanguageChanged);
    }

    if (CulturalSystem)
    {
        CulturalSystem->OnRegionChanged.AddDynamic(this, &UMingLanguageSelectionWidget::OnRegionChanged);
    }
}

void UMingLanguageSelectionWidget::UnbindFromLocalizationEvents()
{
    if (LocalizationSystem)
    {
        LocalizationSystem->OnLanguageChanged.RemoveDynamic(this, &UMingLanguageSelectionWidget::OnLanguageChanged);
    }

    if (CulturalSystem)
    {
        CulturalSystem->OnRegionChanged.RemoveDynamic(this, &UMingLanguageSelectionWidget::OnRegionChanged);
    }
}

void UMingLanguageSelectionWidget::UpdateUIElements()
{
    // Update current language text
    if (CurrentLanguageText)
    {
        CurrentLanguageText->SetText(FText::FromString(GetCurrentLanguageDisplayName()));
    }

    // Update current region text
    if (CurrentRegionText)
    {
        CurrentRegionText->SetText(FText::FromString(GetCurrentRegionDisplayName()));
    }

    // Update button states
    if (ApplyButton)
    {
        ApplyButton->SetIsEnabled(bSettingsModified);
    }

    if (CancelButton)
    {
        CancelButton->SetIsEnabled(bSettingsModified);
    }
}

void UMingLanguageSelectionWidget::UpdateLanguagePreview()
{
    if (!LocalizationSystem || !PreviewText)
    {
        return;
    }

    // Create preview text with current language
    FString PreviewString;
    for (const FString& Key : PreviewTextKeys)
    {
        FString LocalizedText = LocalizationSystem->GetLocalizedText(Key);
        PreviewString += FString::Printf(TEXT("%s: %s\n"), *Key, *LocalizedText);
    }

    PreviewText->SetText(FText::FromString(PreviewString));
}

void UMingLanguageSelectionWidget::UpdateRegionPreview()
{
    if (!CulturalSystem)
    {
        return;
    }

    // Update region-specific preview
    FRegionalGameplayParams Params = CulturalSystem->GetRegionalGameplayParams(SelectedRegion);
    
    FString RegionInfo = FString::Printf(TEXT("Region: %s\n"), *GetCurrentRegionDisplayName());
    RegionInfo += FString::Printf(TEXT("Difficulty: %.2f\n"), Params.DifficultyMultiplier);
    RegionInfo += FString::Printf(TEXT("Resources: %.2f\n"), Params.ResourceMultiplier);
    RegionInfo += FString::Printf(TEXT("AI Aggressiveness: %.2f\n"), Params.AIAggressiveness);

    // Could display this in a separate text block
    UE_LOG(LogMingLanguageSelection, Log, TEXT("Region Preview: %s"), *RegionInfo);
}

void UMingLanguageSelectionWidget::SaveSettings()
{
    if (LocalizationSystem)
    {
        LocalizationSystem->SaveLanguagePreferences();
    }

    if (CulturalSystem)
    {
        CulturalSystem->SaveCulturalPreferences();
    }

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Settings saved"));
}

void UMingLanguageSelectionWidget::LoadSettings()
{
    if (LocalizationSystem)
    {
        LocalizationSystem->LoadLanguagePreferences();
        SelectedLanguage = LocalizationSystem->GetCurrentLanguage();
    }

    if (CulturalSystem)
    {
        CulturalSystem->LoadCulturalPreferences();
        SelectedRegion = CulturalSystem->GetCurrentRegion();
    }

    OriginalLanguage = SelectedLanguage;
    OriginalRegion = SelectedRegion;

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Settings loaded"));
}

void UMingLanguageSelectionWidget::OnLanguageChanged(ELanguageCode NewLanguage)
{
    SelectedLanguage = NewLanguage;
    OriginalLanguage = NewLanguage;
    UpdateUIElements();
    UpdateLanguagePreview();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Language changed event received: %d"), (int32)NewLanguage);
}

void UMingLanguageSelectionWidget::OnRegionChanged(ECulturalRegion NewRegion)
{
    SelectedRegion = NewRegion;
    OriginalRegion = NewRegion;
    UpdateUIElements();
    UpdateRegionPreview();

    UE_LOG(LogMingLanguageSelection, Log, TEXT("Region changed event received: %d"), (int32)NewRegion);
}

bool UMingLanguageSelectionWidget::ValidateSettings() const
{
    // Check if language is available
    if (!IsLanguageAvailable(SelectedLanguage))
    {
        UE_LOG(LogMingLanguageSelection, Error, TEXT("Selected language is not available"));
        return false;
    }

    // Check if region is available
    if (!IsRegionAvailable(SelectedRegion))
    {
        UE_LOG(LogMingLanguageSelection, Error, TEXT("Selected region is not available"));
        return false;
    }

    return true;
}

void UMingLanguageSelectionWidget::ShowConfirmationDialog()
{
    // TODO: Implement confirmation dialog
    UE_LOG(LogMingLanguageSelection, Log, TEXT("Confirmation dialog requested"));
}

void UMingLanguageSelectionWidget::ShowRestartRequiredDialog()
{
    // TODO: Implement restart required dialog
    UE_LOG(LogMingLanguageSelection, Log, TEXT("Restart required dialog requested"));
    
    // For now, just log the message
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, 
            TEXT("Language change requires restart to take full effect"));
    }
}
