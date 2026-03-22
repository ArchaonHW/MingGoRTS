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
    bIsAdditiveSelection = false;
    SelectionStart = FVector2D::ZeroVector;
    SelectionEnd = FVector2D::ZeroVector;
    RTSCamera = nullptr;
    LastClickTime = 0.0f;
    DoubleClickThreshold = 0.3f; // 300ms 雙擊閾值
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
    
    // 選擇過濾快捷鍵
    InputComponent->BindAction("SelectSameType", IE_Pressed, this, &AMingGoRTSPlayerController::SelectSameType);
    InputComponent->BindAction("SelectDamaged", IE_Pressed, this, &AMingGoRTSPlayerController::SelectDamaged);
    InputComponent->BindAction("SelectIdle", IE_Pressed, this, &AMingGoRTSPlayerController::SelectIdle);
    InputComponent->BindAction("DeselectAll", IE_Pressed, this, &AMingGoRTSPlayerController::DeselectAll);

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
    // 檢測雙擊
    float CurrentTime = GetWorld()->GetTimeSeconds();
    float TimeSinceLastClick = CurrentTime - LastClickTime;
    
    if (TimeSinceLastClick <= DoubleClickThreshold)
    {
        // 雙擊檢測到 - 選擇屏幕上所有同類型單位
        UE_LOG(LogTemp, Log, TEXT("Double-click detected - selecting same type units"));
        SelectSameType();
        LastClickTime = 0.0f; // 重置避免三擊觸發
        return;
    }
    
    LastClickTime = CurrentTime;
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
    
    // 檢查是否按住 Ctrl 鍵進行添加選擇
    bIsAdditiveSelection = IsInputKeyDown(EKeys::LeftControl) || IsInputKeyDown(EKeys::RightControl);
    
    float MouseX, MouseY;
    if (GetMousePosition(MouseX, MouseY))
    {
        SelectionStart = FVector2D(MouseX, MouseY);
        SelectionEnd = SelectionStart;
    }

    // 如果不是添加模式，清除之前選擇的單位
    if (!bIsAdditiveSelection)
    {
        for (AMingGoRTSUnit* Unit : SelectedUnits)
        {
            if (Unit)
            {
                Unit->SetSelected(false);
            }
        }
        SelectedUnits.Empty();
    }
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

    // 確定選擇框的邊界
    FVector2D Min(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
    FVector2D Max(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));

    // 框太小時視為單擊選擇
    bool bIsClick = (Max - Min).SizeSquared() < 100.0f; // 10x10 pixels threshold

    // 獲取所有單位
    TArray<AActor*> AllUnits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMingGoRTSUnit::StaticClass(), AllUnits);

    // 如果不是添加模式，清除當前選擇
    if (!bIsAdditiveSelection)
    {
        for (AMingGoRTSUnit* Unit : SelectedUnits)
        {
            if (Unit)
            {
                Unit->SetSelected(false);
            }
        }
        SelectedUnits.Empty();
    }

    int32 NewlySelectedCount = 0;
    for (AActor* Actor : AllUnits)
    {
        AMingGoRTSUnit* Unit = Cast<AMingGoRTSUnit>(Actor);
        if (!Unit)
        {
            continue;
        }

        // 將單位世界位置轉換為屏幕位置
        FVector UnitLocation = Unit->GetActorLocation();
        FVector2D ScreenPos;
        
        if (ProjectWorldLocationToScreen(UnitLocation, ScreenPos))
        {
            bool bIsInBox = (ScreenPos.X >= Min.X && ScreenPos.X <= Max.X &&
                            ScreenPos.Y >= Min.Y && ScreenPos.Y <= Max.Y);

            // 如果是單擊，檢查是否點擊在單位上（擴大的命中區域）
            if (bIsClick)
            {
                const float HitTolerance = 20.0f;
                FVector2D ClickCenter = (Min + Max) * 0.5f;
                bIsInBox = (FMath::Abs(ScreenPos.X - ClickCenter.X) <= HitTolerance &&
                           FMath::Abs(ScreenPos.Y - ClickCenter.Y) <= HitTolerance);
            }

            if (bIsInBox)
            {
                if (!SelectedUnits.Contains(Unit))
                {
                    SelectedUnits.Add(Unit);
                    Unit->SetSelected(true);
                    NewlySelectedCount++;
                }
            }
        }
    }

    // 如果是單擊且沒有選中任何單位，發送移動命令
    if (bIsClick && NewlySelectedCount == 0 && SelectedUnits.Num() > 0)
    {
        FHitResult Hit;
        if (GetHitResultUnderCursor(ECC_Visibility, false, Hit))
        {
            MoveSelectedUnits(Hit.Location);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d units in rect (click=%s)"), 
        NewlySelectedCount, bIsClick ? TEXT("true") : TEXT("false"));
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

void AMingGoRTSPlayerController::SelectSameType()
{
    if (SelectedUnits.Num() == 0)
    {
        return;
    }

    // 獲取主選單位的類型
    AMingGoRTSUnit* PrimaryUnit = SelectedUnits[0];
    if (!PrimaryUnit)
    {
        return;
    }

    FString UnitType = PrimaryUnit->GetUnitType();
    
    // 清除當前選擇
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();

    // 選擇屏幕上所有同類型單位
    TArray<AActor*> AllUnits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMingGoRTSUnit::StaticClass(), AllUnits);
    
    for (AActor* Actor : AllUnits)
    {
        AMingGoRTSUnit* Unit = Cast<AMingGoRTSUnit>(Actor);
        if (Unit && Unit->GetUnitType() == UnitType)
        {
            FVector UnitLocation = Unit->GetActorLocation();
            FVector2D ScreenPos;
            if (ProjectWorldLocationToScreen(UnitLocation, ScreenPos))
            {
                // 只選擇屏幕內的可見單位
                int32 ScreenX, ScreenY;
                GetViewportSize(ScreenX, ScreenY);
                if (ScreenPos.X >= 0 && ScreenPos.X <= ScreenX && ScreenPos.Y >= 0 && ScreenPos.Y <= ScreenY)
                {
                    SelectedUnits.Add(Unit);
                    Unit->SetSelected(true);
                }
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d units of type %s"), SelectedUnits.Num(), *UnitType);
}

void AMingGoRTSPlayerController::SelectDamaged()
{
    // 從當前選擇中過濾出受損單位
    TArray<AMingGoRTSUnit*> DamagedUnits;
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit && Unit->IsDamaged())
        {
            DamagedUnits.Add(Unit);
        }
    }

    // 清除當前選擇
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();

    // 只保留受損單位
    for (AMingGoRTSUnit* Unit : DamagedUnits)
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(true);
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d damaged units"), SelectedUnits.Num());
}

void AMingGoRTSPlayerController::SelectIdle()
{
    // 從當前選擇中過濾出空閒單位
    TArray<AMingGoRTSUnit*> IdleUnits;
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit && Unit->GetUnitState() == ERTSUnitState::Idle)
        {
            IdleUnits.Add(Unit);
        }
    }

    // 清除當前選擇
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();

    // 只保留空閒單位
    for (AMingGoRTSUnit* Unit : IdleUnits)
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(true);
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d idle units"), SelectedUnits.Num());
}

void AMingGoRTSPlayerController::DeselectAll()
{
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    SelectedUnits.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Deselected all units"));
}
