// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Language Entry Widget for Language Selection List

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Localization/MingRTSLocalizationSystem.h"
#include "MingLanguageEntryWidget.generated.h"

/**
 * Individual language entry widget for the language selection list
 * Displays language name, flag, and selection state
 */
UCLASS(ClassGroup = (MingRTS), BlueprintType, Blueprintable)
class MINGRTS_API UMingLanguageEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingLanguageEntryWidget();

    /** Set the language code for this entry */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetLanguageCode(ERTSLanguageCode LanguageCode);

    /** Get the language code */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    ERTSLanguageCode GetLanguageCode() const { return LanguageCode; }

    /** Set whether this entry is selected */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetIsSelected(bool bSelected);

    /** Get whether this entry is selected */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool GetIsSelected() const { return bIsSelected; }

    /** Set whether this entry is available */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetIsAvailable(bool bAvailable);

    /** Get whether this entry is available */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool GetIsAvailable() const { return bIsAvailable; }

    /** Get language display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetLanguageDisplayName() const;

    /** Get language native name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetLanguageNativeName() const;

    /** Get flag texture path */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetFlagTexturePath() const;

    /** Handle entry click */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void OnEntryClicked();

    /** Update entry appearance */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void UpdateAppearance();

protected:
    /** Called when widget is constructed */
    virtual void NativeConstruct() override;

    /** Update visual state */
    void UpdateVisualState();

    /** Update text display */
    void UpdateTextDisplay();

    /** Update flag display */
    void UpdateFlagDisplay();

    /** Update selection indicator */
    void UpdateSelectionIndicator();

private:
    /** Language code for this entry */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    ERTSLanguageCode LanguageCode;

    /** Selection state */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    bool bIsSelected;

    /** Availability state */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    bool bIsAvailable;

    /** UI Elements - Language Name Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> LanguageNameText;

    /** UI Elements - Native Name Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> NativeNameText;

    /** UI Elements - Flag Image */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> FlagImage;

    /** UI Elements - Selection Indicator */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> SelectionIndicator;

    /** UI Elements - Availability Overlay */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UBorder> AvailabilityOverlay;

    /** UI Elements - Main Button */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> EntryButton;

    /** Localization system reference */
    UPROPERTY()
    TObjectPtr<UMingRTSLocalizationSystem> LocalizationSystem;

    /** Default flag texture path */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FString DefaultFlagPath;

    /** Selected appearance style */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FSlateColor SelectedColor;

    /** Unselected appearance style */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FSlateColor UnselectedColor;

    /** Unavailable appearance style */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FSlateColor UnavailableColor;

public:
    /** Delegate for entry selection */
    

    /** Event fired when this entry is selected */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Localization|UI|Events")
    FOnLanguageEntrySelected OnLanguageSelected;
};


