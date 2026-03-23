// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "Game/MingGoRTSGameMode.h"
#include "Game/MingGoRTSPlayerController.h"
#include "UI/MingGoRTSHUD.h"
#include "UI/MingGoRTSMainMenuWidget.h"
#include "UI/MingGoRTSGameHUDWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMingGoRTSGameMode::AMingGoRTSGameMode()
{
    bGameRunning = false;
    bGamePaused = false;

    // 設置預設 PlayerController
    PlayerControllerClass = AMingGoRTSPlayerController::StaticClass();

    // 設置預設 HUD
    HUDClass = AMingGoRTSHUD::StaticClass();
}

void AMingGoRTSGameMode::BeginPlay()
{
    Super::BeginPlay();

    InitializeUI();
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS GameMode Started"));
}

void AMingGoRTSGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
    Super::InitGame(MapName, Options, ErrorMessage);

    UE_LOG(LogTemp, Log, TEXT("Initializing MingGoRTS Game: %s"), *MapName);
}

void AMingGoRTSGameMode::StartPlay()
{
    Super::StartPlay();

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Game Started"));
}

void AMingGoRTSGameMode::PostLogin(APlayerController* NewPlayer)
{
    Super::PostLogin(NewPlayer);

    UE_LOG(LogTemp, Log, TEXT("Player logged in: %s"), *NewPlayer->GetName());

    // 顯示主選單給新玩家
    if (AMingGoRTSPlayerController* MingController = Cast<AMingGoRTSPlayerController>(NewPlayer))
    {
        ShowMainMenu();
    }
}

void AMingGoRTSGameMode::Logout(AController* Exiting)
{
    Super::Logout(Exiting);

    UE_LOG(LogTemp, Log, TEXT("Player logged out: %s"), *Exiting->GetName());
}

void AMingGoRTSGameMode::StartGame()
{
    bGameRunning = true;
    bGamePaused = false;

    HideMainMenu();
    ShowGameHUD();

    UE_LOG(LogTemp, Log, TEXT("Game Started"));
}

void AMingGoRTSGameMode::EndGame()
{
    bGameRunning = false;
    bGamePaused = false;

    HideGameHUD();
    ShowMainMenu();

    UE_LOG(LogTemp, Log, TEXT("Game Ended"));
}

void AMingGoRTSGameMode::PauseGame()
{
    if (bGameRunning && !bGamePaused)
    {
        bGamePaused = true;
        UGameplayStatics::SetGamePaused(this, true);

        UE_LOG(LogTemp, Log, TEXT("Game Paused"));
    }
}

void AMingGoRTSGameMode::ResumeGame()
{
    if (bGamePaused)
    {
        bGamePaused = false;
        UGameplayStatics::SetGamePaused(this, false);

        UE_LOG(LogTemp, Log, TEXT("Game Resumed"));
    }
}

void AMingGoRTSGameMode::ShowMainMenu()
{
    if (CurrentMainMenu || !MainMenuWidgetClass)
    {
        return;
    }

    CurrentMainMenu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
    if (CurrentMainMenu)
    {
        CurrentMainMenu->AddToViewport(100);

        // 設置輸入模式
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(CurrentMainMenu->TakeWidget());
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void AMingGoRTSGameMode::HideMainMenu()
{
    if (CurrentMainMenu)
    {
        CurrentMainMenu->RemoveFromParent();
        CurrentMainMenu = nullptr;
    }
}

void AMingGoRTSGameMode::ShowGameHUD()
{
    if (CurrentGameHUD || !GameHUDWidgetClass)
    {
        return;
    }

    CurrentGameHUD = CreateWidget<UUserWidget>(GetWorld(), GameHUDWidgetClass);
    if (CurrentGameHUD)
    {
        CurrentGameHUD->AddToViewport(0);

        // 設置遊戲輸入模式
        if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            FInputModeGameAndUI InputMode;
            InputMode.SetHideCursorDuringCapture(false);
            PC->SetInputMode(InputMode);
            PC->bShowMouseCursor = true;
        }
    }
}

void AMingGoRTSGameMode::HideGameHUD()
{
    if (CurrentGameHUD)
    {
        CurrentGameHUD->RemoveFromParent();
        CurrentGameHUD = nullptr;
    }
}

void AMingGoRTSGameMode::SwitchToGameView()
{
    HideMainMenu();
    ShowGameHUD();
}

void AMingGoRTSGameMode::SwitchToMenuView()
{
    HideGameHUD();
    ShowMainMenu();
}

void AMingGoRTSGameMode::SetupDefaultPlayerController()
{
    // 已在構造函數中設置
}

void AMingGoRTSGameMode::InitializeUI()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing UI..."));
    
    // 延遲一幀後顯示主選單，確保世界已完全初始化
    FTimerHandle TimerHandle;
    GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMingGoRTSGameMode::ShowMainMenu, 0.1f, false);
}
