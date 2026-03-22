#include "MingRTSPlayerController.h"
#include "MingSelectionManager.h"
#include "MingTacticalUnit.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

AMingRTSPlayerController::AMingRTSPlayerController()
{
    bShowMouseCursor = true;
    bEnableClickEvents = true;
    bEnableTouchEvents = false; // PC focus
    bEnableMouseOverEvents = true;
    
    CameraMoveSpeed = 1000.0f;
    EdgeScrollingThreshold = 50.0f;
    
    bIsSelecting = false;
    bIsDragging = false;
    
    SelectionManager = nullptr;
}

void AMingRTSPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
    // 添加增強輸入系統
    if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
    {
        if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem = 
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
        {
            InputSubsystem->AddMappingContext(RTSMappingContext, 0);
        }
    }
    
    // 創建選擇管理器
    SelectionManager = NewObject<UMingSelectionManager>(this);
    if (SelectionManager)
    {
        SelectionManager->RegisterComponent();
    }
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSPlayerController initialized"));
}

void AMingRTSPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    
    if (UEnhancedInputComponent* EnhancedInputComponent = 
        Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 選擇動作
        EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Started, 
                                          this, &AMingRTSPlayerController::OnSelectStarted);
        EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered,
                                          this, &AMingRTSPlayerController::OnSelectTriggered);
        EnhancedInputComponent->BindAction(SelectCompleteAction, ETriggerEvent::Completed,
                                          this, &AMingRTSPlayerController::OnSelectCompleted);
        
        // 移動和攻擊命令
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started,
                                          this, &AMingRTSPlayerController::OnMoveCommand);
        EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started,
                                          this, &AMingRTSPlayerController::OnAttackCommand);
        
        // 全選
        EnhancedInputComponent->BindAction(SelectAllAction, ETriggerEvent::Started,
                                          this, &AMingRTSPlayerController::OnSelectAllUnits);
    }
}

void AMingRTSPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);
    
    // 獲取當前鼠標位置
    GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
    
    // 處理邊緣滾動
    HandleEdgeScrolling();
    
    // 更新相機移動
    UpdateCameraMovement(DeltaTime);
    
    // 更新選擇拖動
    if (bIsDragging && SelectionManager)
    {
        SelectionManager->UpdateSelectionBox(CurrentMousePosition);
    }
}

void AMingRTSPlayerController::OnSelectStarted(const FInputActionValue& Value)
{
    if (!SelectionManager)
    {
        return;
    }
    
    FVector2D MousePosition;
    GetMousePosition(MousePosition.X, MousePosition.Y);
    
    bIsSelecting = true;
    bIsDragging = false;
    SelectionStartPosition = MousePosition;
    
    SelectionManager->StartSelectionBox(MousePosition);
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection started at: %s"), *MousePosition.ToString());
}

void AMingRTSPlayerController::OnSelectTriggered(const FInputActionValue& Value)
{
    if (!SelectionManager)
    {
        return;
    }
    
    FVector2D MousePosition;
    GetMousePosition(MousePosition.X, MousePosition.Y);
    
    // 檢查是否達到拖動閾值
    float DragDistance = FVector2D::Distance(SelectionStartPosition, MousePosition);
    if (DragDistance > 5.0f) // 拖動閾值
    {
        bIsDragging = true;
    }
    
    // 更新選擇框
    SelectionManager->UpdateSelectionBox(MousePosition);
}

void AMingRTSPlayerController::OnSelectCompleted(const FInputActionValue& Value)
{
    if (!SelectionManager)
    {
        return;
    }
    
    bIsSelecting = false;
    bIsDragging = false;
    
    // 完成選擇
    SelectionManager->EndSelectionBox();
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection completed. Selected units: %d"),
           SelectionManager->GetSelectedUnitCount());
}

void AMingRTSPlayerController::OnMoveCommand(const FInputActionValue& Value)
{
    // 檢查是否有選中的單位
    if (!SelectionManager || SelectionManager->GetSelectedUnitCount() == 0)
    {
        return;
    }
    
    // 檢查是否按住了Alt鍵（攻擊移動）
    bool bIsAttackMove = IsInputKeyDown(EKeys::LeftAlt) || IsInputKeyDown(EKeys::RightAlt);
    
    FVector TargetLocation;
    FVector TargetNormal;
    
    if (GetMouseWorldLocationAndNormal(TargetLocation, TargetNormal))
    {
        IssueMoveCommand(TargetLocation, bIsAttackMove);
    }
}

void AMingRTSPlayerController::OnAttackCommand(const FInputActionValue& Value)
{
    if (!SelectionManager || SelectionManager->GetSelectedUnitCount() == 0)
    {
        return;
    }
    
    AActor* Target = GetActorUnderMouse();
    if (Target)
    {
        IssueAttackCommand(Target);
    }
}

void AMingRTSPlayerController::OnSelectAllUnits(const FInputActionValue& Value)
{
    if (SelectionManager)
    {
        if (IsInputKeyDown(EKeys::LeftControl) || IsInputKeyDown(EKeys::RightControl))
        {
            // Ctrl+A: 選擇所有單位
            SelectionManager->SelectAllUnitsOnScreen();
        }
        else
        {
            // A: 選擇所有可見單位（在屏幕上的）
            SelectionManager->SelectAllUnitsOnScreen();
        }
    }
}

void AMingRTSPlayerController::IssueMoveCommand(const FVector& TargetLocation, bool bIsAttackMove)
{
    if (!SelectionManager)
    {
        return;
    }
    
    TArray<int32> SelectedUnits = SelectionManager->SelectedUnitIds;
    
    for (int32 UnitId : SelectedUnits)
    {
        // 發布移動命令事件
        FUnitMovedEvent MoveEvent(UnitId, TargetLocation, bIsAttackMove);
        IMingCoreEventBus::PublishEvent(MoveEvent);
    }
    
    // 廣播移動命令事件
    OnUnitsMoveCommand.Broadcast(TargetLocation);
    
    // 顯示移動命令視覺反饋
    if (GetWorld())
    {
        DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 12, FColor::Green, false, 2.0f);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Move command issued to %d units. Target: %s (AttackMove: %s)"),
           SelectedUnits.Num(), *TargetLocation.ToString(), bIsAttackMove ? TEXT("Yes") : TEXT("No"));
}

void AMingRTSPlayerController::IssueAttackCommand(AActor* Target)
{
    if (!SelectionManager || !Target)
    {
        return;
    }
    
    // 直接使用選擇管理器的單位數組（O(1) 而不是 O(N)）
    const TArray<AMingTacticalUnit*>& SelectedUnits = SelectionManager->GetSelectedUnits();
    
    // 設置所有選中單位的攻擊目標
    for (AMingTacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetAttackTarget(Target);
        }
    }
    
    // 廣播攻擊命令事件
    OnUnitsAttackCommand.Broadcast(Target);
    
    UE_LOG(LogTemp, Log, TEXT("Attack command issued to %d units. Target: %s"),
           SelectedUnits.Num(), *Target->GetName());
}

void AMingRTSPlayerController::IssueStopCommand()
{
    if (!SelectionManager)
    {
        return;
    }
    
    // 直接使用選擇管理器的單位數組（O(1) 而不是 O(N)）
    const TArray<AMingTacticalUnit*>& SelectedUnits = SelectionManager->GetSelectedUnits();
    
    for (AMingTacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            // 停止當前動作
            // 需要擴展MingTacticalUnit類別來支持Stop命令
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Stop command issued to %d units"), SelectedUnits.Num());
}

void AMingRTSPlayerController::IssueHoldPositionCommand()
{
    // 實現保持位置命令
    UE_LOG(LogTemp, Log, TEXT("Hold position command issued"));
}

void AMingRTSPlayerController::HandleEdgeScrolling()
{
    if (!GetLocalPlayer())
    {
        return;
    }
    
    int32 ViewportSizeX, ViewportSizeY;
    GetViewportSize(ViewportSizeX, ViewportSizeY);
    
    float MoveX = 0.0f;
    float MoveY = 0.0f;
    
    // 檢查鼠標位置
    if (CurrentMousePosition.X < EdgeScrollingThreshold)
    {
        MoveX = -1.0f; // 向左移動
    }
    else if (CurrentMousePosition.X > ViewportSizeX - EdgeScrollingThreshold)
    {
        MoveX = 1.0f; // 向右移動
    }
    
    if (CurrentMousePosition.Y < EdgeScrollingThreshold)
    {
        MoveY = 1.0f; // 向前移動（向上）
    }
    else if (CurrentMousePosition.Y > ViewportSizeY - EdgeScrollingThreshold)
    {
        MoveY = -1.0f; // 向後移動（向下）
    }
    
    // 應用相機移動
    if (MoveX != 0.0f || MoveY != 0.0f)
    {
        FVector CameraLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
        FVector NewLocation = CameraLocation + FVector(MoveY, MoveX, 0.0f) * CameraMoveSpeed * GetWorld()->DeltaTimeSeconds;
        
        if (GetPawn())
        {
            GetPawn()->SetActorLocation(NewLocation);
        }
    }
}

void AMingRTSPlayerController::UpdateCameraMovement(float DeltaTime)
{
    // 鍵盤相機移動
    float MoveX = 0.0f;
    float MoveY = 0.0f;
    
    if (IsInputKeyDown(EKeys::W) || IsInputKeyDown(EKeys::Up))
    {
        MoveY = 1.0f;
    }
    else if (IsInputKeyDown(EKeys::S) || IsInputKeyDown(EKeys::Down))
    {
        MoveY = -1.0f;
    }
    
    if (IsInputKeyDown(EKeys::A) || IsInputKeyDown(EKeys::Left))
    {
        MoveX = -1.0f;
    }
    else if (IsInputKeyDown(EKeys::D) || IsInputKeyDown(EKeys::Right))
    {
        MoveX = 1.0f;
    }
    
    if (MoveX != 0.0f || MoveY != 0.0f)
    {
        FVector CameraLocation = GetPawn() ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
        FVector NewLocation = CameraLocation + FVector(MoveY, MoveX, 0.0f) * CameraMoveSpeed * DeltaTime;
        
        if (GetPawn())
        {
            GetPawn()->SetActorLocation(NewLocation);
        }
    }
}

FVector AMingRTSPlayerController::GetMouseWorldLocation() const
{
    FVector WorldLocation, WorldDirection;
    
    if (DeprojectScreenPositionToWorld(CurrentMousePosition.X, CurrentMousePosition.Y,
                                          WorldLocation, WorldDirection))
    {
        return WorldLocation;
    }
    
    return FVector::ZeroVector;
}

bool AMingRTSPlayerController::GetMouseWorldLocationAndNormal(FVector& OutLocation, FVector& OutNormal) const
{
    FVector WorldLocation, WorldDirection;
    
    if (!DeprojectScreenPositionToWorld(CurrentMousePosition.X, CurrentMousePosition.Y,
                                         WorldLocation, WorldDirection))
    {
        return false;
    }
    
    // 執行射線檢測
    FVector TraceStart = WorldLocation;
    FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                              ECC_Visibility, QueryParams))
    {
        OutLocation = HitResult.Location;
        OutNormal = HitResult.Normal;
        return true;
    }
    
    return false;
}

AActor* AMingRTSPlayerController::GetActorUnderMouse() const
{
    FVector WorldLocation, WorldDirection;
    
    if (!DeprojectScreenPositionToWorld(CurrentMousePosition.X, CurrentMousePosition.Y,
                                         WorldLocation, WorldDirection))
    {
        return nullptr;
    }
    
    FVector TraceStart = WorldLocation;
    FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;
    
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    
    if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                              ECC_Visibility, QueryParams))
    {
        return HitResult.GetActor();
    }
    
    return nullptr;
}

void AMingRTSPlayerController::EnableDebugVisualization(bool bEnable)
{
    if (SelectionManager)
    {
        SelectionManager->bEnableDebugVisualization = bEnable;
    }
}

bool AMingRTSPlayerController::IsDebugVisualizationEnabled() const
{
    if (SelectionManager)
    {
        return SelectionManager->bEnableDebugVisualization;
    }
    return false;
}
