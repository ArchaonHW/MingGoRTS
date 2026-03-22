#include "MingGoRTSPlayerController.h"
#include "MingGoRTSCamera.h"
#include "MingGoRTSUnit.h"
#include "MingGoRTSHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMingGoRTSPlayerController::AMingGoRTSPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableMouseOverEvents = true;
    bIsSelecting = false;
    SelectionStart = FVector2D::ZeroVector;
    SelectionEnd = FVector2D::ZeroVector;
    RTSCamera = nullptr;
}

void AMingGoRTSPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取 RTS 相機
    RTSCamera = Cast<AMingGoRTSCamera>(GetPawn());
    
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS PlayerController Started"));
}

void AMingGoRTSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    // 滑鼠輸入
    InputComponent->BindAction("LeftMouse", IE_Pressed, this, &AMingGoRTSPlayerController::OnLeftMouseButtonPressed);
    InputComponent->BindAction("LeftMouse", IE_Released, this, &AMingGoRTSPlayerController::OnLeftMouseButtonReleased);
    InputComponent->BindAction("RightMouse", IE_Pressed, this, &AMingGoRTSPlayerController::OnRightMouseButtonPressed);
    InputComponent->BindAction("MouseScrollUp", IE_Pressed, this, &AMingGoRTSPlayerController::OnMouseScrollUp);
    InputComponent->BindAction("MouseScrollDown", IE_Pressed, this, &AMingGoRTSPlayerController::OnMouseScrollDown);

    // 相機移動
    InputComponent->BindAxis("MoveCameraForward", this, &AMingGoRTSPlayerController::OnMoveCameraForward);
    InputComponent->BindAxis("MoveCameraRight", this, &AMingGoRTSPlayerController::OnMoveCameraRight);
}

void AMingGoRTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // 更新選擇框
    if (bIsSelecting)
    {
        float MouseX, MouseY;
        GetMousePosition(MouseX, MouseY);
        SelectionEnd = FVector2D(MouseX, MouseY);
    }
}

void AMingGoRTSPlayerController::OnLeftMouseButtonPressed()
{
    StartSelection();
}

void AMingGoRTSPlayerController::OnLeftMouseButtonReleased()
{
    EndSelection();
}

void AMingGoRTSPlayerController::OnRightMouseButtonPressed()
{
    // 獲取滑鼠位置並轉換為世界座標
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        FVector WorldLocation, WorldDirection;
        if (DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
        {
            // 簡單的地面投射
            FVector GroundLocation = WorldLocation + WorldDirection * 1000.0f;
            GroundLocation.Z = 0.0f; // 假設地面在 Z=0
            
            MoveSelectedUnits(GroundLocation);
        }
    }
}

void AMingGoRTSPlayerController::OnMouseScrollUp()
{
    ZoomCamera(1.0f);
}

void AMingGoRTSPlayerController::OnMouseScrollDown()
{
    ZoomCamera(-1.0f);
}

void AMingGoRTSPlayerController::OnMoveCameraForward(float Value)
{
    if (RTSCamera && Value != 0.0f)
    {
        FVector Direction = RTSCamera->GetActorForwardVector();
        RTSCamera->MoveCamera(Direction * Value);
    }
}

void AMingGoRTSPlayerController::OnMoveCameraRight(float Value)
{
    if (RTSCamera && Value != 0.0f)
    {
        FVector Direction = RTSCamera->GetActorRightVector();
        RTSCamera->MoveCamera(Direction * Value);
    }
}

void AMingGoRTSPlayerController::StartSelection()
{
    bIsSelecting = true;
    
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        SelectionStart = FVector2D(MouseX, MouseY);
        SelectionEnd = SelectionStart;
    }

    // 清除之前選擇的單位
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();
}

void AMingGoRTSPlayerController::EndSelection()
{
    if (bIsSelecting)
    {
        SelectUnitsInRect(SelectionStart, SelectionEnd);
        bIsSelecting = false;
    }
}

void AMingGoRTSPlayerController::SelectUnitsInRect(FVector2D Start, FVector2D End)
{
    // 獲取 HUD 來繪製選擇框
    AMingGoRTSHUD* HUD = Cast<AMingGoRTSHUD>(GetHUD());
    if (HUD)
    {
        HUD->ClearSelectionBox();
    }

    // 這裡應該實現單位選擇邏輯
    // 簡化版本：獲取所有單位並檢查是否在選擇框內
    TArray<AActor*> AllUnits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMingGoRTSUnit::StaticClass(), AllUnits);

    for (AActor* Actor : AllUnits)
    {
        AMingGoRTSUnit* Unit = Cast<AMingGoRTSUnit>(Actor);
        if (Unit)
        {
            // 簡化的選擇邏輯 - 實際應該檢查螢幕座標
            SelectedUnits.Add(Unit);
            Unit->SetSelected(true);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d units"), SelectedUnits.Num());
}

void AMingGoRTSPlayerController::MoveSelectedUnits(FVector Location)
{
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->MoveToLocation(Location);
        }
    }
}

void AMingGoRTSPlayerController::ZoomCamera(float Delta)
{
    if (RTSCamera)
    {
        float CurrentDistance = RTSCamera->SpringArm->TargetArmLength;
        RTSCamera->SetCameraDistance(CurrentDistance - Delta * 100.0f);
    }
}
