// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingMainHUD - Main Heads-Up Display

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingMainHUD.generated.h"

UENUM(BlueprintType)
enum class EHUDPanelType : uint8 {
    ResourcePanel       UMETA(DisplayName = "Resource Panel"),
    UnitPanel           UMETA(DisplayName = "Unit Panel"),
    BuildingPanel       UMETA(DisplayName = "Building Panel"),
    MinimapPanel        UMETA(DisplayName = "Minimap Panel"),
    ObjectivePanel      UMETA(DisplayName = "Objective Panel"),
    MessagePanel        UMETA(DisplayName = "Message Panel"),
    MenuPanel           UMETA(DisplayName = "Menu Panel"),
    AbilityPanel        UMETA(DisplayName = "Ability Panel")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPanelOpened, EHUDPanelType, PanelType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPanelClosed, EHUDPanelType, PanelType};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMenuRequested, FString, MenuName};

UCLASS()
class MINGUI_API UMingMainHUD : public UUserWidget {
    GENERATED_BODY()

public:
    UMingMainHUD(const FObjectInitializer& ObjectInitializer};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void InitializeHUD(class APlayerController* OwnerController};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowPanel(EHUDPanelType PanelType};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void HidePanel(EHUDPanelType PanelType};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void TogglePanel(EHUDPanelType PanelType};

    UFUNCTION(BlueprintPure, Category = "Main HUD")
    bool IsPanelVisible(EHUDPanelType PanelType) const;

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowAllPanels(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void HideAllPanels(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowGameMenu(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void HideGameMenu(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowPauseMenu(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void HidePauseMenu(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void UpdateResourceDisplay(const TMap<FString, float>& ResourceValues};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowNotification(const FString& Message, float Duration = 3.0f};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ShowObjective(const FString& ObjectiveText};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void ClearObjective(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD")
    void SetMinimapTexture(class UTexture2D* MinimapTexture};

    UPROPERTY(BlueprintAssignable, Category = "Main HUD Events")
    FOnPanelOpened OnPanelOpened;

    UPROPERTY(BlueprintAssignable, Category = "Main HUD Events")
    FOnPanelClosed OnPanelClosed;

    UPROPERTY(BlueprintAssignable, Category = "Main HUD Events")
    FOnMenuRequested OnMenuRequested;

protected:
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UCanvasPanel* MainCanvas;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* ResourcePanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* UnitPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* BuildingPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* MinimapPanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* ObjectivePanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* MessagePanel;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* MenuOverlay;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UOverlay* PauseOverlay;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ResourceText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UTextBlock* ObjectiveText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UImage* MinimapImage;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    class UVerticalBox* NotificationContainer;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main HUD")
    TSubclassOf<UUserWidget> NotificationWidgetClass;

    UPROPERTY(BlueprintReadOnly)
    TMap<EHUDPanelType, UOverlay*> PanelMap;

    UPROPERTY(BlueprintReadOnly)
    APlayerController* OwningController;

    UPROPERTY(BlueprintReadOnly)
    TSet<EHUDPanelType> VisiblePanels;

    UFUNCTION(BlueprintCallable, Category = "Main HUD Internal")
    void BuildPanelMap(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD Internal")
    void SetupPanelVisibility(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD Internal")
    void HandleMenuButtonClicked(};

    UFUNCTION(BlueprintCallable, Category = "Main HUD Internal")
    void HandlePauseButtonClicked(};

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    void CreateNotification(const FString& Message, float Duration};
    void AnimatePanelTransition(UOverlay* Panel, bool bShow};
};
