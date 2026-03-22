// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 7.1: Region Entry Widget for Region Selection List

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Localization/MingRTSCulturalAdaptationSystem.h"
#include "MingRegionEntryWidget.generated.h"

/**
 * Individual region entry widget for the region selection list
 * Displays region name, icon, and selection state
 */
UCLASS(ClassGroup = (MingRTS), BlueprintType, Blueprintable)
class MINGGORTS_API UMingRegionEntryWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingRegionEntryWidget();

    /** Set the region code for this entry */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetRegionCode(ECulturalRegion RegionCode);

    /** Get the region code */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    ECulturalRegion GetRegionCode() const { return RegionCode; }

    /** Set whether this entry is selected */
    UFUNCTION(BlueprintCallable, Category = "MingRTS|Localization|UI")
    void SetIsSelected(bool bSelected);

    /** Get whether this entry is selected */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    bool GetIsSelected() const { return bIsSelected; }

    /** Get region display name */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetRegionDisplayName() const;

    /** Get region description */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetRegionDescription() const;

    /** Get region icon texture path */
    UFUNCTION(BlueprintPure, Category = "MingRTS|Localization|UI")
    FString GetRegionIconPath() const;

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

    /** Update icon display */
    void UpdateIconDisplay();

    /** Update selection indicator */
    void UpdateSelectionIndicator();

private:
    /** Region code for this entry */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    ECulturalRegion RegionCode;

    /** Selection state */
    UPROPERTY(BlueprintReadOnly, Category = "MingRTS|Localization|UI", meta = (AllowPrivateAccess = "true"))
    bool bIsSelected;

    /** UI Elements - Region Name Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> RegionNameText;

    /** UI Elements - Region Description Text */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> RegionDescriptionText;

    /** UI Elements - Region Icon Image */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> RegionIconImage;

    /** UI Elements - Selection Indicator */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> SelectionIndicator;

    /** UI Elements - Main Button */
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> EntryButton;

    /** Cultural adaptation system reference */
    UPROPERTY()
    TObjectPtr<UMingRTSCulturalAdaptationSystem> CulturalSystem;

    /** Default icon texture path */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FString DefaultIconPath;

    /** Selected appearance style */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FSlateColor SelectedColor;

    /** Unselected appearance style */
    UPROPERTY(EditDefaultsOnly, Category = "MingRTS|Localization|UI")
    FSlateColor UnselectedColor;

public:
    /** Delegate for entry selection */
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRegionEntrySelected, ECulturalRegion, RegionCode, class UMingRegionEntryWidget*, EntryWidget);

    /** Event fired when this entry is selected */
    UPROPERTY(BlueprintAssignable, Category = "MingRTS|Localization|UI|Events")
    FOnRegionEntrySelected OnRegionSelected;
};
