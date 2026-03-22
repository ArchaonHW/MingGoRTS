#include "MingSelectionManager.h"
#include "MingTacticalUnit.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Blueprint/UserWidget.h"

UMingSelectionManager::UMingSelectionManager()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickGroup = TG_PrePhysics;

    SelectionBoxThreshold = 5.0f;
    bEnableDebugVisualization = false;
    bWasCtrlPressed = false;
    bWasShiftPressed = false;
    SelectionStartTime = 0.0;
}

void UMingSelectionManager::BeginPlay()
{
    Super::BeginPlay();

    OwningController = Cast<APlayerController>(GetOwner());
    if (!OwningController)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingSelectionManager requires a PlayerController as owner"));
        return;
    }

    UE_LOG(LogTemp, Log, TEXT("MingSelectionManager initialized"));
}

void UMingSelectionManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!OwningController.IsValid())
    {
        return;
    }

    // 更新選擇框視覺化
    if (CurrentSelectionBox.bIsActive && bEnableDebugVisualization)
    {
        VisualizeSelectionBox();
    }
}

void UMingSelectionManager::StartSelectionBox(const FVector2D& ScreenPosition)
{
    if (!OwningController.IsValid())
    {
        return;
    }

    // 檢查按鍵狀態
    bWasCtrlPressed = OwningController->IsInputKeyDown(EKeys::LeftControl) ||
                      OwningController->IsInputKeyDown(EKeys::RightControl);
    bWasShiftPressed = OwningController->IsInputKeyDown(EKeys::LeftShift) ||
                       OwningController->IsInputKeyDown(EKeys::RightShift);

    // 初始化選擇框
    CurrentSelectionBox.StartPosition = ScreenPosition;
    CurrentSelectionBox.CurrentPosition = ScreenPosition;
    CurrentSelectionBox.bIsActive = false; // 等待拖動閾值

    SelectionStartPosition = ScreenPosition;
    SelectionStartTime = FPlatformTime::Seconds();

    UE_LOG(LogTemp, Log, TEXT("Selection started at position: %s"), *ScreenPosition.ToString());
}

void UMingSelectionManager::UpdateSelectionBox(const FVector2D& ScreenPosition)
{
    if (!OwningController.IsValid())
    {
        return;
    }

    CurrentSelectionBox.CurrentPosition = ScreenPosition;

    // 檢查是否達到拖動閾值
    float DragDistance = FVector2D::Distance(SelectionStartPosition, ScreenPosition);
    double CurrentTime = FPlatformTime::Seconds();
    double TimeHeld = CurrentTime - SelectionStartTime;

    // 如果拖動距離超過閾值或時間超過延遲閾值，啟動框選模式
    if (!CurrentSelectionBox.bIsActive)
    {
        if (DragDistance > SelectionBoxThreshold || TimeHeld > SELECTION_DELAY_THRESHOLD)
        {
            CurrentSelectionBox.bIsActive = true;
            UE_LOG(LogTemp, Log, TEXT("Box selection activated"));
        }
    }

    // 即時更新選擇預覽
    if (CurrentSelectionBox.bIsActive && bEnableDebugVisualization)
    {
        UpdateSelectedUnits();
    }
}

void UMingSelectionManager::EndSelectionBox()
{
    if (!OwningController.IsValid())
    {
        return;
    }

    double SelectionEndTime = FPlatformTime::Seconds();
    double SelectionDuration = SelectionEndTime - SelectionStartTime;
    float DragDistance = FVector2D::Distance(SelectionStartPosition, CurrentSelectionBox.CurrentPosition);

    // 判斷是框選還是單點選擇
    if (CurrentSelectionBox.bIsActive && IsSelectionBoxValid())
    {
        // 框選模式
        PerformBoxSelection();
        UE_LOG(LogTemp, Log, TEXT("Box selection completed"));
    }
    else if (DragDistance < SelectionBoxThreshold && SelectionDuration < SELECTION_DELAY_THRESHOLD)
    {
        // 單點選擇模式
        PerformSingleSelection(CurrentSelectionBox.CurrentPosition);
        UE_LOG(LogTemp, Log, TEXT("Single click selection performed"));
    }

    // 重置選擇框
    CurrentSelectionBox.bIsActive = false;
    CurrentSelectionBox.StartPosition = FVector2D::ZeroVector;
    CurrentSelectionBox.CurrentPosition = FVector2D::ZeroVector;

    bWasCtrlPressed = false;
    bWasShiftPressed = false;
}

void UMingSelectionManager::SelectSingleUnit(int32 UnitId, bool bAddToSelection)
{
    if (!bAddToSelection)
    {
        DeselectAllUnits();
    }

    if (!SelectedUnitIds.Contains(UnitId))
    {
        SelectedUnitIds.Add(UnitId);

        // 發布事件
        FUnitSelectedEvent Event(UnitId, FVector2D::ZeroVector);
        IMingCoreEventBus::PublishEvent(Event);

        OnUnitSelected.Broadcast(UnitId);

        UE_LOG(LogTemp, Log, TEXT("Unit %d selected"), UnitId);
    }
}

void UMingSelectionManager::DeselectUnit(int32 UnitId)
{
    if (SelectedUnitIds.Contains(UnitId))
    {
        SelectedUnitIds.Remove(UnitId);
        OnUnitDeselected.Broadcast(UnitId);

        UE_LOG(LogTemp, Log, TEXT("Unit %d deselected"), UnitId);
    }
}

void UMingSelectionManager::DeselectAllUnits()
{
    if (SelectedUnitIds.Num() > 0)
    {
        TArray<int32> UnitsToDeselect = SelectedUnitIds;
        SelectedUnitIds.Empty();

        for (int32 UnitId : UnitsToDeselect)
        {
            OnUnitDeselected.Broadcast(UnitId);
        }

        OnSelectionCleared.Broadcast();

        UE_LOG(LogTemp, Log, TEXT("All units deselected (%d units)"), UnitsToDeselect.Num());
    }
}

void UMingSelectionManager::InvertSelection()
{
    // 獲取場景中所有單位
    TArray<AMingTacticalUnit*> AllUnits;
    for (TActorIterator<AMingTacticalUnit> It(GetWorld()); It; ++It)
    {
        AllUnits.Add(*It);
    }

    TArray<int32> NewSelection;
    for (AMingTacticalUnit* Unit : AllUnits)
    {
        if (Unit && !SelectedUnitIds.Contains(Unit->UnitId))
        {
            NewSelection.Add(Unit->UnitId);
        }
    }

    DeselectAllUnits();

    for (int32 UnitId : NewSelection)
    {
        SelectSingleUnit(UnitId, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Selection inverted: %d units now selected"), NewSelection.Num());
}

bool UMingSelectionManager::IsUnitSelected(int32 UnitId) const
{
    return SelectedUnitIds.Contains(UnitId);
}

int32 UMingSelectionManager::GetSelectedUnitCount() const
{
    return SelectedUnitIds.Num();
}

void UMingSelectionManager::SelectAllUnitsOnScreen()
{
    DeselectAllUnits();

    // 獲取所有在屏幕上的單位
    TArray<AMingTacticalUnit*> ScreenUnits;
    for (TActorIterator<AMingTacticalUnit> It(GetWorld()); It; ++It)
    {
        AMingTacticalUnit* Unit = *It;
        if (Unit && OwningController.IsValid())
        {
            FVector2D ScreenPosition;
            if (OwningController->ProjectWorldLocationToScreen(Unit->GetActorLocation(), ScreenPosition))
            {
                int32 ViewportSizeX, ViewportSizeY;
                OwningController->GetViewportSize(ViewportSizeX, ViewportSizeY);

                // 檢查是否在視野範圍內
                if (ScreenPosition.X >= 0 && ScreenPosition.X <= ViewportSizeX &&
                    ScreenPosition.Y >= 0 && ScreenPosition.Y <= ViewportSizeY)
                {
                    ScreenUnits.Add(Unit);
                }
            }
        }
    }

    // 選擇所有在屏幕上的單位
    for (AMingTacticalUnit* Unit : ScreenUnits)
    {
        SelectSingleUnit(Unit->UnitId, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Selected all %d units on screen"), ScreenUnits.Num());
}

void UMingSelectionManager::SelectUnitsByType(FString UnitType)
{
    DeselectAllUnits();

    for (TActorIterator<AMingTacticalUnit> It(GetWorld()); It; ++It)
    {
        AMingTacticalUnit* Unit = *It;
        if (Unit && Unit->UnitName.Contains(UnitType))
        {
            SelectSingleUnit(Unit->UnitId, true);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Selected units by type '%s': %d units"), *UnitType, GetSelectedUnitCount());
}

void UMingSelectionManager::PerformBoxSelection()
{
    if (!IsSelectionBoxValid())
    {
        return;
    }

    // 獲取框選範圍內的所有單位
    TArray<AMingTacticalUnit*> UnitsInBox = GetUnitsInScreenRect(CurrentSelectionBox.GetScreenRect());

    TArray<int32> PreviouslySelected = SelectedUnitIds;
    TArray<int32> NewlySelected;
    TArray<int32> NewlyDeselected;

    // 處理選擇邏輯
    if (!bWasCtrlPressed && !bWasShiftPressed)
    {
        // 正常選擇：清除舊選擇，選擇新單位
        DeselectAllUnits();
    }

    if (bWasShiftPressed)
    {
        // Shift選擇：移除在框內的已選單位（反選）
        for (AMingTacticalUnit* Unit : UnitsInBox)
        {
            if (Unit)
            {
                int32 UnitId = Unit->UnitId;
                if (SelectedUnitIds.Contains(UnitId))
                {
                    DeselectUnit(UnitId);
                    NewlyDeselected.Add(UnitId);
                }
                else
                {
                    SelectSingleUnit(UnitId, true);
                    NewlySelected.Add(UnitId);
                }
            }
        }
    }
    else
    {
        // 正常或Ctrl選擇：添加新單位
        for (AMingTacticalUnit* Unit : UnitsInBox)
        {
            if (Unit && !SelectedUnitIds.Contains(Unit->UnitId))
            {
                SelectSingleUnit(Unit->UnitId, true);
                NewlySelected.Add(Unit->UnitId);
            }
        }
    }

    // 發布批量選擇事件
    PublishSelectionEvents(NewlySelected, NewlyDeselected);

    UE_LOG(LogTemp, Log, TEXT("Box selection: %d units in box, %d newly selected, %d newly deselected"),
           UnitsInBox.Num(), NewlySelected.Num(), NewlyDeselected.Num());
}

void UMingSelectionManager::PerformSingleSelection(const FVector2D& ScreenPosition)
{
    TArray<AMingTacticalUnit*> UnitsUnderCursor = GetUnitUnderCursor(ScreenPosition);

    if (UnitsUnderCursor.Num() == 0)
    {
        // 點擊空白處，取消選擇
        if (!bWasCtrlPressed)
        {
            DeselectAllUnits();
        }
        return;
    }

    // 選擇最上面的單位（或者實現單位選擇輪換）
    AMingTacticalUnit* TargetUnit = UnitsUnderCursor[0];

    if (!bWasCtrlPressed)
    {
        // 如果沒有Ctrl，且點擊的單位已經被選中，則什麼都不做（保持選擇）
        // 如果點擊的單位未被選中，則清除其他選擇並選中該單位
        if (!SelectedUnitIds.Contains(TargetUnit->UnitId))
        {
            DeselectAllUnits();
            SelectSingleUnit(TargetUnit->UnitId, false);
        }
    }
    else
    {
        // Ctrl選擇：切換單位選擇狀態
        if (SelectedUnitIds.Contains(TargetUnit->UnitId))
        {
            DeselectUnit(TargetUnit->UnitId);
        }
        else
        {
            SelectSingleUnit(TargetUnit->UnitId, true);
        }
    }
}

TArray<AMingTacticalUnit*> UMingSelectionManager::GetUnitsInScreenRect(const FRect& ScreenRect)
{
    TArray<AMingTacticalUnit*> Result;

    if (!OwningController.IsValid())
    {
        return Result;
    }

    for (TActorIterator<AMingTacticalUnit> It(GetWorld()); It; ++It)
    {
        AMingTacticalUnit* Unit = *It;
        if (!Unit)
        {
            continue;
        }

        // 將單位世界位置轉換為屏幕位置
        FVector2D ScreenPosition;
        if (OwningController->ProjectWorldLocationToScreen(Unit->GetActorLocation(), ScreenPosition))
        {
            // 檢查是否在選擇框內
            if (ScreenRect.Contains(ScreenPosition))
            {
                Result.Add(Unit);
            }
        }
    }

    return Result;
}

TArray<AMingTacticalUnit*> UMingSelectionManager::GetUnitUnderCursor(const FVector2D& ScreenPosition)
{
    TArray<AMingTacticalUnit*> Result;

    if (!OwningController.IsValid() || !GetWorld())
    {
        return Result;
    }

    // 執行射線檢測
    FHitResult HitResult;
    FVector WorldLocation, WorldDirection;

    if (OwningController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y,
                                                            WorldLocation, WorldDirection))
    {
        FVector TraceStart = WorldLocation;
        FVector TraceEnd = WorldLocation + WorldDirection * 10000.0f;

        FCollisionQueryParams QueryParams;
        QueryParams.bTraceComplex = false;
        QueryParams.bReturnPhysicalMaterial = false;

        // 檢測單位類別
        if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd,
                                                  ECC_GameTraceChannel1, QueryParams))
        {
            AMingTacticalUnit* HitUnit = Cast<AMingTacticalUnit>(HitResult.GetActor());
            if (HitUnit)
            {
                Result.Add(HitUnit);
            }
        }
    }

    return Result;
}

void UMingSelectionManager::PublishSelectionEvents(const TArray<int32>& NewlySelected,
                                                    const TArray<int32>& NewlyDeselected)
{
    // 發布選擇事件
    for (int32 UnitId : NewlySelected)
    {
        FUnitSelectedEvent Event(UnitId, FVector2D::ZeroVector);
        IMingCoreEventBus::PublishEvent(Event);
    }

    // 發布取消選擇事件
    for (int32 UnitId : NewlyDeselected)
    {
        FUnitMovedEvent Event(UnitId, FVector::ZeroVector, false); // 使用FUnitMovedEvent作為通用事件
        IMingCoreEventBus::PublishEvent(Event);
    }
}

void UMingSelectionManager::UpdateSelectedUnits()
{
    // 即時更新選擇預覽（可選：在選擇過程中高亮顯示將被選中的單位）
    // 這裡可以添加視覺預覽邏輯
}

void UMingSelectionManager::VisualizeSelectionBox()
{
    if (!GetWorld() || !CurrentSelectionBox.bIsActive)
    {
        return;
    }

    FRect Rect = CurrentSelectionBox.GetScreenRect();
    FVector2D MinPoint(Rect.Left, Rect.Top);
    FVector2D MaxPoint(Rect.Right, Rect.Bottom);

    // 注意：這裡需要將屏幕坐標轉換為世界坐標來繪製
    // 簡化版本：使用Blueprint在屏幕上繪製

    // 在真實實現中，這裡應該調用UMG或渲染系統來繪製選擇框
    UE_LOG(LogTemp, VeryVerbose, TEXT("Selection box visualization: (%f, %f) to (%f, %f)"),
           Rect.Left, Rect.Top, Rect.Right, Rect.Bottom);
}

bool UMingSelectionManager::IsSelectionBoxValid() const
{
    FRect Rect = CurrentSelectionBox.GetScreenRect();
    return Rect.Width() >= SelectionBoxThreshold && Rect.Height() >= SelectionBoxThreshold;
}
