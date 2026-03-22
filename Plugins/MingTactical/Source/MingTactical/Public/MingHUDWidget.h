#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingHUDWidget.generated.h"

/**
 * Main HUD Widget for MingGoRTS
 * Parent class for the main game HUD
 */
UCLASS(Abstract, Blueprintable)
class MINGTACTICAL_API UMingHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // Selection Widget
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UMingSelectionWidget> SelectionWidget;

    // Resource Display
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UTextBlock> ResourceText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UHorizontalBox> ResourceContainer;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UMingResourceDisplayWidget> ResourceDisplayWidget;

    // Unit Info Panel
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UBorder> UnitInfoPanel;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UTextBlock> UnitNameText;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UProgressBar> HealthBar;

    // Command Buttons Container
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UUniformGridPanel> CommandButtonsContainer;

    // Minimap
    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UImage> MinimapImage;

    // Debug Info
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool bShowDebugInfo;

    UPROPERTY(meta = (BindWidgetOptional))
    TObjectPtr<class UTextBlock> DebugInfoText;

    // Functions
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowSelectionWidget();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HideSelectionWidget();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateUnitInfo(class AMingTacticalUnit* Unit};

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ClearUnitInfo();

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void UpdateResources(const TMap<FString, int32>& Resources};

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowCommandButtons(const TArray<FName>& AvailableCommands};

    UFUNCTION(BlueprintCallable, Category = "HUD")
    void HideCommandButtons();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void UpdateDebugInfo(const FString& DebugText};

protected:
    UPROPERTY()
    TWeakObjectPtr<class AMingRTSPlayerController> OwningController;

    UPROPERTY()
    TWeakObjectPtr<class UMingSelectionManager> SelectionManager;

    virtual bool Initialize() override;
    void SetupBindings();
    void OnSelectionChanged(int32 UnitId};
};

