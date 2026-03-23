// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSGameMode.generated.h"

/**
 * MingGoRTS 主要遊戲模式
 * 提供基礎遊戲流程和 UI 管理
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class AMingGoRTSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSGameMode();

    virtual void BeginPlay() override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    /** 開始遊戲 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS GameFlow")
    void StartGame();

    /** 結束遊戲 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS GameFlow")
    void EndGame();

    /** 暫停遊戲 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS GameFlow")
    void PauseGame();

    /** 恢復遊戲 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS GameFlow")
    void ResumeGame();

    /** 顯示主選單 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void ShowMainMenu();

    /** 隱藏主選單 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void HideMainMenu();

    /** 顯示遊戲 HUD */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void ShowGameHUD();

    /** 隱藏遊戲 HUD */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void HideGameHUD();

    /** 切換到遊戲畫面 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void SwitchToGameView();

    /** 切換到選單畫面 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS UI")
    void SwitchToMenuView();

    /** 遊戲狀態取得 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS GameFlow")
    bool IsGameRunning() const { return bGameRunning; }

    UFUNCTION(BlueprintPure, Category = "MingGoRTS GameFlow")
    bool IsGamePaused() const { return bGamePaused; }

protected:
    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI")
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI")
    TSubclassOf<class UUserWidget> GameHUDWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS UI")
    TSubclassOf<class AHUD> HUDClass;

    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentMainMenu;

    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentGameHUD;

    UPROPERTY()
    TObjectPtr<class AMingGoRTSHUD> GameHUD;

    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS GameFlow")
    bool bGameRunning;

    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS GameFlow")
    bool bGamePaused;

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;

    void SetupDefaultPlayerController();
    void InitializeUI();
};
