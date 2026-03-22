// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Language Selection UI Widget

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "MingLanguageSelectionWidget.generated.h"

/**
 * Language selection widget for MingGoRTS
 * Provides UI for language and cultural region selection
 */
UCLASS(ClassGroup = (MingRTS), BlueprintType, Blueprintable)
class MINGGORTS_API UMingLanguageSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLanguageSelectionWidget();

    /** Initialize the language selection widget */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void InitializeLanguageSelection();

    /** Refresh available languages list */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void RefreshLanguageList();

    /** Refresh available regions list */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void RefreshRegionList();

    /** Set current language */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetCurrentLanguage(ERTSLanguageCode LanguageCode};

    /** Set current region */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetCurrentRegion(ECulturalRegion Region};

    /** Get current language display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetCurrentLanguageDisplayName() const;

    /** Get current region display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetCurrentRegionDisplayName() const;

    /** Check if language is available */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool IsLanguageAvailable(ERTSLanguageCode LanguageCode) const;

    /** Check if region is available */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool IsRegionAvailable(ECulturalRegion Region) const;

    /** Get language flag texture path */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetLanguageFlagPath(ERTSLanguageCode LanguageCode) const;

    /** Get region icon texture path */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetRegionIconPath(ECulturalRegion Region) const;

    /** Apply settings and close */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void ApplySettings();

    /** Reset to default settings */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void ResetToDefaults();

    /** Cancel changes */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void CancelChanges();

protected:
    /** Called when widget is constructed */
    virtual void NativeConstruct() override;

    /** Called when widget is destroyed */
    virtual void NativeDestruct() override;

    /** Bind to localization system events */
    void BindToLocalizationEvents();

    /** Unbind from localization system events */
    void UnbindFromLocalizationEvents();

    /** Update UI elements */
    void UpdateUIElements();

    /** Update language preview */
    void UpdateLanguagePreview();

    /** Update region preview */
    void UpdateRegionPreview();

    /** Save current settings */
    void SaveSettings();

    /** Load saved settings */
    void LoadSettings();

    /** Handle language change event */
    UFUNCTION()
    void OnLanguageChanged(ERTSLanguageCode NewLanguage};

    /** Handle region change event */
    UFUNCTION()
    void OnRegionChanged(ECulturalRegion NewRegion};

    /** Validate settings */
    bool ValidateSettings() const;

    /** Show confirmation dialog */
    void ShowConfirmationDialog();

    /** Show restart required dialog */
    void ShowRestartRequiredDialog();

private:
    /** Localization system reference */
    UPROPERTY()
    TObjectPtr<UMingRTSLocalizationSystem> LocalizationSystem;

    /** Cultural adaptation system reference */
    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> CulturalSystem;

    /** Current selected language */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    ERTSLanguageCode SelectedLanguage;

    /** Current selected region */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    ECulturalRegion SelectedRegion;

    /** Original language before changes */
    ERTSLanguageCode OriginalLanguage;

    /** Original region before changes */
    ECulturalRegion OriginalRegion;

    /** Available languages list */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    TArray<ERTSLanguageCode> AvailableLanguages;

    /** Available regions list */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    TArray<ECulturalRegion> AvailableRegions;

    /** Whether settings have been modified */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    bool bSettingsModified;

    /** Whether restart is required */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    bool bRestartRequired;

    /** UI Elements - Language List */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UListView> LanguageListView;

    /** UI Elements - Region List */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UListView> RegionListView;

    /** UI Elements - Current Language Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> CurrentLanguageText;

    /** UI Elements - Current Region Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> CurrentRegionText;

    /** UI Elements - Preview Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> PreviewText;

    /** UI Elements - Apply Button */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> ApplyButton;

    /** UI Elements - Cancel Button */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> CancelButton;

    /** UI Elements - Reset Button */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> ResetButton;

    /** Language entry widget class */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    TSubclassOf<class UUserWidget> LanguageEntryWidgetClass;

    /** Region entry widget class */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    TSubclassOf<class UUserWidget> RegionEntryWidgetClass;

    /** Flag texture paths for languages */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    TMap<ERTSLanguageCode, FString> LanguageFlagPaths;

    /** Region icon paths */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    TMap<ECulturalRegion, FString> RegionIconPaths;

    /** Preview text keys for demonstration */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    TArray<FString> PreviewTextKeys;

public:
    /** Get available languages */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    TArray<ERTSLanguageCode> GetAvailableLanguages() const { return AvailableLanguages; }

    /** Get available regions */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    TArray<ECulturalRegion> GetAvailableRegions() const { return AvailableRegions; }

    /** Check if settings need restart */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool NeedsRestart() const { return bRestartRequired; }

    /** Check if settings are modified */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool IsSettingsModified() const { return bSettingsModified; }
};

