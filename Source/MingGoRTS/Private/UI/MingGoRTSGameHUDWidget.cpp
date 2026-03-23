// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "UI/MingGoRTSGameHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/Button.h"
#include "Engine/Texture2D.h"
#include "TimerManager.h"

UMingGoRTSGameHUDWidget::UMingGoRTSGameHUDWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMingGoRTSGameHUDWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (MenuButton)
    {
        MenuButton->OnClicked.AddDynamic(this, &UMingGoRTSGameHUDWidget::OnMenuButtonClicked);
    }

    UE_LOG(LogTemp, Log, TEXT("Game HUD Widget Constructed"));
}

void UMingGoRTSGameHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMingGoRTSGameHUDWidget::UpdateResources(int32 Gold, int32 Wood, int32 Food, int32 Population)
{
    if (GoldText)
    {
        GoldText->SetText(FText::FromString(FString::Printf(TEXT("Gold: %d"), Gold)));
    }

    if (WoodText)
    {
        WoodText->SetText(FText::FromString(FString::Printf(TEXT("Wood: %d"), Wood)));
    }

    if (FoodText)
    {
        FoodText->SetText(FText::FromString(FString::Printf(TEXT("Food: %d"), Food)));
    }

    if (PopulationText)
    {
        PopulationText->SetText(FText::FromString(FString::Printf(TEXT("Pop: %d"), Population)));
    }
}

void UMingGoRTSGameHUDWidget::UpdateSelectedUnit(const FString& UnitName, int32 Health, int32 MaxHealth)
{
    if (SelectedUnitName)
    {
        SelectedUnitName->SetText(FText::FromString(UnitName));
    }

    if (SelectedUnitHealthBar)
    {
        float HealthPercent = MaxHealth > 0 ? (float)Health / (float)MaxHealth : 0.0f;
        SelectedUnitHealthBar->SetPercent(HealthPercent);
    }
}

void UMingGoRTSGameHUDWidget::ShowNotification(const FString& Message, float Duration)
{
    if (NotificationText && NotificationOverlay)
    {
        NotificationText->SetText(FText::FromString(Message));
        NotificationOverlay->SetVisibility(ESlateVisibility::Visible);

        // 設置定時器隱藏通知
        FTimerHandle TempHandle;
        GetWorld()->GetTimerManager().SetTimer(TempHandle, this, &UMingGoRTSGameHUDWidget::HideNotification, Duration, false);
    }
}

void UMingGoRTSGameHUDWidget::UpdateGameTime(const FString& TimeString)
{
    if (GameTimeText)
    {
        GameTimeText->SetText(FText::FromString(TimeString));
    }
}

void UMingGoRTSGameHUDWidget::SetMinimapTexture(UTexture2D* MinimapTexture)
{
    if (MinimapImage && MinimapTexture)
    {
        // 這裡可以設置迷你地圖紋理
    }
}

void UMingGoRTSGameHUDWidget::OnMenuButtonClicked()
{
    UE_LOG(LogTemp, Log, TEXT("Menu Button Clicked"));
    // 通知遊戲模式顯示選單
}

void UMingGoRTSGameHUDWidget::HideNotification()
{
    if (NotificationOverlay)
    {
        NotificationOverlay->SetVisibility(ESlateVisibility::Hidden);
    }
}
