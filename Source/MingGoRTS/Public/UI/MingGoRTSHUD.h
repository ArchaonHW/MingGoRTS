// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MingGoRTSHUD.generated.h"

/**
 * MingGoRTS HUD 類別
 * 管理遊戲中所有 UI 元素的顯示
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class AMingGoRTSHUD : public AHUD
{
    GENERATED_BODY()

public:
    AMingGoRTSHUD();

    virtual void BeginPlay() override;
    virtual void DrawHUD() override;

    /** 顯示主要遊戲介面 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ShowGameInterface();

    /** 隱藏主要遊戲介面 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void HideGameInterface();

    /** 顯示資源面板 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ShowResourcePanel();

    /** 隱藏資源面板 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void HideResourcePanel();

    /** 顯示單位選擇面板 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ShowUnitSelectionPanel();

    /** 隱藏單位選擇面板 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void HideUnitSelectionPanel();

    /** 顯示迷你地圖 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ShowMinimap();

    /** 隱藏迷你地圖 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void HideMinimap();

    /** 顯示聊天視窗 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ShowChatWindow();

    /** 隱藏聊天視窗 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void HideChatWindow();

    /** 切換選單顯示 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void ToggleInGameMenu();

    /** 設置 HUD 可見性 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS HUD")
    void SetHUDVisibility(bool bVisible);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> GameInterfaceClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> ResourcePanelClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> UnitSelectionPanelClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> MinimapClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> ChatWindowClass;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS HUD")
    TSubclassOf<class UUserWidget> InGameMenuClass;

    UPROPERTY()
    TObjectPtr<class UUserWidget> GameInterfaceWidget;

    UPROPERTY()
    TObjectPtr<class UUserWidget> ResourcePanelWidget;

    UPROPERTY()
    TObjectPtr<class UUserWidget> UnitSelectionPanelWidget;

    UPROPERTY()
    TObjectPtr<class UUserWidget> MinimapWidget;

    UPROPERTY()
    TObjectPtr<class UUserWidget> ChatWindowWidget;

    UPROPERTY()
    TObjectPtr<class UUserWidget> InGameMenuWidget;

    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS HUD")
    bool bHUDVisible;

    void InitializeWidgets();
    void CleanupWidgets();
};
