// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingGoRTSGameHUDWidget.generated.h"

/**
 * MingGoRTS 遊戲 HUD Widget
 * 顯示遊戲中的主要資訊和介面
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class UMingGoRTSGameHUDWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingGoRTSGameHUDWidget(const FObjectInitializer& ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    /** 更新資源顯示 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Game HUD")
    void UpdateResources(int32 Gold, int32 Wood, int32 Food, int32 Population);

    /** 更新選中單位資訊 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Game HUD")
    void UpdateSelectedUnit(const FString& UnitName, int32 Health, int32 MaxHealth);

    /** 顯示通知訊息 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Game HUD")
    void ShowNotification(const FString& Message, float Duration = 3.0f);

    /** 顯示遊戲時間 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Game HUD")
    void UpdateGameTime(const FString& TimeString);

    /** 設置迷你地圖紋理 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Game HUD")
    void SetMinimapTexture(class UTexture2D* MinimapTexture);

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> GoldText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> WoodText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> FoodText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> PopulationText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> GameTimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> SelectedUnitName;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UProgressBar> SelectedUnitHealthBar;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UImage> MinimapImage;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UOverlay> NotificationOverlay;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UTextBlock> NotificationText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<class UButton> MenuButton;

    UFUNCTION()
    void OnMenuButtonClicked();

    void HideNotification();

    FTimerHandle NotificationTimerHandle;
};
