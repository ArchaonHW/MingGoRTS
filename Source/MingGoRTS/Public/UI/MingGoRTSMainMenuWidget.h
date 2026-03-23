// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingGoRTSMainMenuWidget.generated.h"

/**
 * MingGoRTS 主選單 Widget
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class UMingGoRTSMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSMainMenuWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Main Menu")
    void StartNewGame();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Main Menu")
    void ContinueGame();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Main Menu")
    void OpenSettings();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Main Menu")
    void OpenCredits();

    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Main Menu")
    void QuitGame();

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> StartGameButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> ContinueButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> SettingsButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> CreditsButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> QuitButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> GameTitleText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> BackgroundImage;

    UFUNCTION()
    void OnStartGameClicked();

    UFUNCTION()
    void OnContinueClicked();

    UFUNCTION()
    void OnSettingsClicked();

    UFUNCTION()
    void OnCreditsClicked();

    UFUNCTION()
    void OnQuitClicked();
};
