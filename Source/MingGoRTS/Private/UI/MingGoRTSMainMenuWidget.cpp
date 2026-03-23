// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "UI/MingGoRTSMainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Game/MingGoRTSGameMode.h"

UMingGoRTSMainMenuWidget::UMingGoRTSMainMenuWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UMingGoRTSMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 綁定按鈕事件
    if (StartGameButton)
    {
        StartGameButton->OnClicked.AddDynamic(this, &UMingGoRTSMainMenuWidget::OnStartGameClicked);
    }

    if (ContinueButton)
    {
        ContinueButton->OnClicked.AddDynamic(this, &UMingGoRTSMainMenuWidget::OnContinueClicked);
    }

    if (SettingsButton)
    {
        SettingsButton->OnClicked.AddDynamic(this, &UMingGoRTSMainMenuWidget::OnSettingsClicked);
    }

    if (CreditsButton)
    {
        CreditsButton->OnClicked.AddDynamic(this, &UMingGoRTSMainMenuWidget::OnCreditsClicked);
    }

    if (QuitButton)
    {
        QuitButton->OnClicked.AddDynamic(this, &UMingGoRTSMainMenuWidget::OnQuitClicked);
    }

    UE_LOG(LogTemp, Log, TEXT("Main Menu Widget Constructed"));
}

void UMingGoRTSMainMenuWidget::NativeDestruct()
{
    Super::NativeDestruct();
}

void UMingGoRTSMainMenuWidget::StartNewGame()
{
    UE_LOG(LogTemp, Log, TEXT("Starting New Game..."));

    if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
    {
        if (AMingGoRTSGameMode* MingGameMode = Cast<AMingGoRTSGameMode>(GameMode))
        {
            MingGameMode->StartGame();
        }
    }
}

void UMingGoRTSMainMenuWidget::ContinueGame()
{
    UE_LOG(LogTemp, Log, TEXT("Continuing Game..."));
    // TODO: 載入存檔
}

void UMingGoRTSMainMenuWidget::OpenSettings()
{
    UE_LOG(LogTemp, Log, TEXT("Opening Settings..."));
    // TODO: 顯示設定選單
}

void UMingGoRTSMainMenuWidget::OpenCredits()
{
    UE_LOG(LogTemp, Log, TEXT("Opening Credits..."));
    // TODO: 顯示製作人員
}

void UMingGoRTSMainMenuWidget::QuitGame()
{
    UE_LOG(LogTemp, Log, TEXT("Quitting Game..."));
    UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}

void UMingGoRTSMainMenuWidget::OnStartGameClicked()
{
    StartNewGame();
}

void UMingGoRTSMainMenuWidget::OnContinueClicked()
{
    ContinueGame();
}

void UMingGoRTSMainMenuWidget::OnSettingsClicked()
{
    OpenSettings();
}

void UMingGoRTSMainMenuWidget::OnCreditsClicked()
{
    OpenCredits();
}

void UMingGoRTSMainMenuWidget::OnQuitClicked()
{
    QuitGame();
}
