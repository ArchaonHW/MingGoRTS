// Copyright (c) 2026 MingGoRTS. All rights reserved.
// MingUISystem - Core UI Management System

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MingUISystem.generated.h"

UENUM(BlueprintType)
enum class EUIState : uint8 {
    MainMenu            UMETA(DisplayName = "Main Menu"),
    Loading             UMETA(DisplayName = "Loading"),
    InGame              UMETA(DisplayName = "In Game"),
    Paused              UMETA(DisplayName = "Paused"),
    Options             UMETA(DisplayName = "Options"),
    SaveLoad            UMETA(DisplayName = "Save/Load"),
    Cutscene            UMETA(DisplayName = "Cutscene"),
    ModalDialog         UMETA(DisplayName = "Modal Dialog")
};

UENUM(BlueprintType)
enum class EUIInputMode : uint8 {
    GameOnly            UMETA(DisplayName = "Game Only"),
    UIOnly              UMETA(DisplayName = "UI Only"),
    GameAndUI           UMETA(DisplayName = "Game And UI")
};

USTRUCT(BlueprintType)
struct FUIConfig {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> MainMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> HUDClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> PauseMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> OptionsMenuClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> SaveLoadClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> LoadingScreenClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class UUserWidget> ModalDialogClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bShowMouseCursor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableClickEvents;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableMouseOverEvents;

    FUIConfig()
        : bShowMouseCursor(true)
        , bEnableClickEvents(true)
        , bEnableMouseOverEvents(true) {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUIStateChanged, EUIState, NewState};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputModeChanged, EUIInputMode, NewMode};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWidgetOpened, FString, WidgetName, class UUserWidget*, Widget};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetClosed, FString, WidgetName};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGUI_API UMingUISystem : public UObject {
    GENERATED_BODY()

public:
    UMingUISystem(};

    UFUNCTION(BlueprintCallable, Category = "UI System")
    void InitializeUISystem(class APlayerController* OwnerController};

    UFUNCTION(BlueprintCallable, Category = "UI System")
    void ShutdownUISystem(};

    UFUNCTION(BlueprintCallable, Category = "UI System")
    void SetUIConfig(const FUIConfig& Config};

    UFUNCTION(BlueprintPure, Category = "UI System")
    FUIConfig GetUIConfig() const;

    UFUNCTION(BlueprintCallable, Category = "UI System State")
    void ChangeUIState(EUIState NewState};

    UFUNCTION(BlueprintPure, Category = "UI System State")
    EUIState GetCurrentUIState() const;

    UFUNCTION(BlueprintCallable, Category = "UI System State")
    void SetInputMode(EUIInputMode InputMode};

    UFUNCTION(BlueprintPure, Category = "UI System State")
    EUIInputMode GetCurrentInputMode() const;

    UPROPERTY(BlueprintAssignable, Category = "UI System Events")
    FOnUIStateChanged OnUIStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "UI System Events")
    FOnInputModeChanged OnInputModeChanged;

    UFUNCTION(BlueprintCallable, Category = "UI System Widgets")
    UUserWidget* OpenWidget(const FString& WidgetName, TSubclassOf<UUserWidget> WidgetClass};

    UFUNCTION(BlueprintCallable, Category = "UI System Widgets")
    void CloseWidget(const FString& WidgetName};

    UFUNCTION(BlueprintCallable, Category = "UI System Widgets")
    void CloseAllWidgets(};

    UFUNCTION(BlueprintPure, Category = "UI System Widgets")
    UUserWidget* GetWidget(const FString& WidgetName) const;

    UFUNCTION(BlueprintPure, Category = "UI System Widgets")
    bool IsWidgetOpen(const FString& WidgetName) const;

    UPROPERTY(BlueprintAssignable, Category = "UI System Events")
    FOnWidgetOpened OnWidgetOpened;

    UPROPERTY(BlueprintAssignable, Category = "UI System Events")
    FOnWidgetClosed OnWidgetClosed;

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void ShowMainMenu(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void HideMainMenu(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void ShowHUD(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void HideHUD(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void ShowPauseMenu(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void HidePauseMenu(};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void ShowLoadingScreen(const FString& LoadingText};

    UFUNCTION(BlueprintCallable, Category = "UI System Screens")
    void HideLoadingScreen(};

    UFUNCTION(BlueprintCallable, Category = "UI System Dialogs")
    void ShowModalDialog(const FString& Title, const FString& Message, const FString& ConfirmText, const FString& CancelText};

    UFUNCTION(BlueprintCallable, Category = "UI System Dialogs")
    void HideModalDialog(};

    UFUNCTION(BlueprintCallable, Category = "UI System")
    void Tick(float DeltaTime};

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    FUIConfig CurrentConfig;

    UPROPERTY()
    EUIState CurrentState;

    UPROPERTY()
    EUIInputMode CurrentInputMode;

    UPROPERTY()
    APlayerController* OwnerController;

    UPROPERTY()
    TMap<FString, UUserWidget*> ActiveWidgets;

    UPROPERTY()
    UUserWidget* CurrentMainMenu;

    UPROPERTY()
    UUserWidget* CurrentHUD;

    UPROPERTY()
    UUserWidget* CurrentPauseMenu;

    UPROPERTY()
    UUserWidget* CurrentLoadingScreen;

    UPROPERTY()
    UUserWidget* CurrentModalDialog;

    void SetInputModeGameOnly(};
    void SetInputModeUIOnly(};
    void SetInputModeGameAndUI(};
    void UpdateMouseCursor(};
    void CleanupClosedWidgets(};
    void HandleModalConfirm(};
    void HandleModalCancel(};
};
