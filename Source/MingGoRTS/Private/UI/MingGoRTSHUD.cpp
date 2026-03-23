// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "UI/MingGoRTSHUD.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Texture2D.h"

AMingGoRTSHUD::AMingGoRTSHUD()
{
    bHUDVisible = true;
}

void AMingGoRTSHUD::BeginPlay()
{
    Super::BeginPlay();

    InitializeWidgets();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS HUD Initialized"));
}

void AMingGoRTSHUD::DrawHUD()
{
    Super::DrawHUD();

    // 這裡可以繪製自訂的 HUD 元素
}

void AMingGoRTSHUD::ShowGameInterface()
{
    if (GameInterfaceWidget)
    {
        GameInterfaceWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingGoRTSHUD::HideGameInterface()
{
    if (GameInterfaceWidget)
    {
        GameInterfaceWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::ShowResourcePanel()
{
    if (ResourcePanelWidget)
    {
        ResourcePanelWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingGoRTSHUD::HideResourcePanel()
{
    if (ResourcePanelWidget)
    {
        ResourcePanelWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::ShowUnitSelectionPanel()
{
    if (UnitSelectionPanelWidget)
    {
        UnitSelectionPanelWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingGoRTSHUD::HideUnitSelectionPanel()
{
    if (UnitSelectionPanelWidget)
    {
        UnitSelectionPanelWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::ShowMinimap()
{
    if (MinimapWidget)
    {
        MinimapWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingGoRTSHUD::HideMinimap()
{
    if (MinimapWidget)
    {
        MinimapWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::ShowChatWindow()
{
    if (ChatWindowWidget)
    {
        ChatWindowWidget->SetVisibility(ESlateVisibility::Visible);
    }
}

void AMingGoRTSHUD::HideChatWindow()
{
    if (ChatWindowWidget)
    {
        ChatWindowWidget->SetVisibility(ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::ToggleInGameMenu()
{
    if (InGameMenuWidget)
    {
        if (InGameMenuWidget->IsVisible())
        {
            InGameMenuWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        else
        {
            InGameMenuWidget->SetVisibility(ESlateVisibility::Visible);
        }
    }
}

void AMingGoRTSHUD::SetHUDVisibility(bool bVisible)
{
    bHUDVisible = bVisible;

    if (GameInterfaceWidget)
    {
        GameInterfaceWidget->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }
}

void AMingGoRTSHUD::InitializeWidgets()
{
    // 初始化 Widgets
    if (GameInterfaceClass)
    {
        GameInterfaceWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), GameInterfaceClass);
        if (GameInterfaceWidget)
        {
            GameInterfaceWidget->AddToViewport(10);
        }
    }

    if (ResourcePanelClass)
    {
        ResourcePanelWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), ResourcePanelClass);
        if (ResourcePanelWidget)
        {
            ResourcePanelWidget->AddToViewport(20);
        }
    }

    if (MinimapClass)
    {
        MinimapWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), MinimapClass);
        if (MinimapWidget)
        {
            MinimapWidget->AddToViewport(30);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("HUD Widgets Initialized"));
}

void AMingGoRTSHUD::CleanupWidgets()
{
    if (GameInterfaceWidget)
    {
        GameInterfaceWidget->RemoveFromParent();
        GameInterfaceWidget = nullptr;
    }

    if (ResourcePanelWidget)
    {
        ResourcePanelWidget->RemoveFromParent();
        ResourcePanelWidget = nullptr;
    }

    if (UnitSelectionPanelWidget)
    {
        UnitSelectionPanelWidget->RemoveFromParent();
        UnitSelectionPanelWidget = nullptr;
    }

    if (MinimapWidget)
    {
        MinimapWidget->RemoveFromParent();
        MinimapWidget = nullptr;
    }

    if (ChatWindowWidget)
    {
        ChatWindowWidget->RemoveFromParent();
        ChatWindowWidget = nullptr;
    }

    if (InGameMenuWidget)
    {
        InGameMenuWidget->RemoveFromParent();
        InGameMenuWidget = nullptr;
    }
}
