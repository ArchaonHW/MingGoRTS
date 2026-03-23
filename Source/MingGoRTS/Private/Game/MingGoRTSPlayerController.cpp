// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "Game/MingGoRTSPlayerController.h"
#include "UI/MingGoRTSHUD.h"

AMingGoRTSPlayerController::AMingGoRTSPlayerController()
{
    bMenuOpen = false;
}

void AMingGoRTSPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // 設置滑鼠游標
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Crosshairs;

    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Player Controller Ready"));
}

void AMingGoRTSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 綁定輸入動作
    InputComponent->BindAction("LeftMouseClick", IE_Pressed, this, &AMingGoRTSPlayerController::OnLeftMouseClick);
    InputComponent->BindAction("RightMouseClick", IE_Pressed, this, &AMingGoRTSPlayerController::OnRightMouseClick);
    InputComponent->BindAction("BoxSelect", IE_Pressed, this, &AMingGoRTSPlayerController::OnBoxSelect);
    InputComponent->BindAction("Deselect", IE_Pressed, this, &AMingGoRTSPlayerController::OnDeselect);
    InputComponent->BindAction("OpenMenu", IE_Pressed, this, &AMingGoRTSPlayerController::OnOpenMenuInput);
}

void AMingGoRTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
}

void AMingGoRTSPlayerController::SelectUnit(class AMingGoRTSUnit* Unit)
{
    if (Unit)
    {
        SelectedUnits.Empty();
        SelectedUnits.Add(Unit);

        UE_LOG(LogTemp, Log, TEXT("Selected Unit: %s"), *Unit->GetName());
    }
}

void AMingGoRTSPlayerController::SelectUnits(const TArray<class AMingGoRTSUnit*>& Units)
{
    SelectedUnits = Units;

    UE_LOG(LogTemp, Log, TEXT("Selected %d Units"), Units.Num());
}

void AMingGoRTSPlayerController::MoveSelectedUnits(const FVector& Destination)
{
    for (auto* Unit : SelectedUnits)
    {
        if (Unit)
        {
            // 發出移動命令
            UE_LOG(LogTemp, Log, TEXT("Moving unit to: %s"), *Destination.ToString());
        }
    }
}

void AMingGoRTSPlayerController::AttackTarget(AActor* Target)
{
    if (Target && SelectedUnits.Num() > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Attacking target: %s"), *Target->GetName());
    }
}

void AMingGoRTSPlayerController::OpenMenu()
{
    if (!bMenuOpen)
    {
        bMenuOpen = true;

        // 暫停遊戲
        if (AMingGoRTSHUD* GameHUD = Cast<AMingGoRTSHUD>(GetHUD()))
        {
            GameHUD->ToggleInGameMenu();
        }

        // 設置 UI 輸入模式
        FInputModeUIOnly InputMode;
        InputMode.SetWidgetToFocus(nullptr);
        SetInputMode(InputMode);
        bShowMouseCursor = true;

        UE_LOG(LogTemp, Log, TEXT("Menu Opened"));
    }
}

void AMingGoRTSPlayerController::CloseMenu()
{
    if (bMenuOpen)
    {
        bMenuOpen = false;

        // 隱藏選單
        if (AMingGoRTSHUD* GameHUD = Cast<AMingGoRTSHUD>(GetHUD()))
        {
            GameHUD->ToggleInGameMenu();
        }

        // 恢復遊戲輸入模式
        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);
        SetInputMode(InputMode);
        bShowMouseCursor = true;

        UE_LOG(LogTemp, Log, TEXT("Menu Closed"));
    }
}

void AMingGoRTSPlayerController::ToggleMenu()
{
    if (bMenuOpen)
    {
        CloseMenu();
    }
    else
    {
        OpenMenu();
    }
}

void AMingGoRTSPlayerController::OnLeftMouseClick()
{
    // 處理左鍵點擊
    if (AMingGoRTSUnit* Unit = GetUnitUnderMouse())
    {
        SelectUnit(Unit);
    }
}

void AMingGoRTSPlayerController::OnRightMouseClick()
{
    // 處理右鍵點擊 - 移動或攻擊
    FVector WorldPosition;
    if (GetMouseWorldPosition(WorldPosition))
    {
        MoveSelectedUnits(WorldPosition);
    }
}

void AMingGoRTSPlayerController::OnBoxSelect()
{
    // 處理框選
    UE_LOG(LogTemp, Log, TEXT("Box Select Triggered"));
}

void AMingGoRTSPlayerController::OnDeselect()
{
    // 取消選擇
    SelectedUnits.Empty();
    UE_LOG(LogTemp, Log, TEXT("Deselected all units"));
}

void AMingGoRTSPlayerController::OnOpenMenuInput()
{
    ToggleMenu();
}

bool AMingGoRTSPlayerController::GetMouseWorldPosition(FVector& OutWorldPosition)
{
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        FVector WorldDirection;
        if (DeprojectMousePositionToWorld(OutWorldPosition, WorldDirection))
        {
            return true;
        }
    }
    return false;
}

class AMingGoRTSUnit* AMingGoRTSPlayerController::GetUnitUnderMouse()
{
    // 這裡需要實作射線檢測來取得滑鼠下方的單位
    return nullptr;
}
