#include "Managers/MingSelectionManager.h"
#include "Units/MingTacticalUnit.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

// 靜態實例初始化
UMingSelectionManager* UMingSelectionManager::Instance = nullptr;

UMingSelectionManager::UMingSelectionManager()
    : bInitialized(false)
{
}

void UMingSelectionManager::Initialize()
{
    if (bInitialized)
    {
        return;
    }
    
    SelectedUnits.Empty();
    SelectionBox = FSelectionBox();
    
    Instance = this;
    bInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("SelectionManager initialized"));
}

void UMingSelectionManager::Shutdown()
{
    if (!bInitialized)
    {
        return;
    }
    
    // 取消所有選擇
    DeselectAll();
    
    SelectedUnits.Empty();
    Instance = nullptr;
    bInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("SelectionManager shutdown"));
}

void UMingSelectionManager::SelectUnit(AMingTacticalUnit* Unit, bool bAdditive)
{
    if (!Unit || !Unit->IsAlive())
    {
        return;
    }
    
    // 如果不是加法選擇，先清除之前的選擇
    if (!bAdditive)
    {
        DeselectAll();
    }
    
    // 檢查是否已經在選擇列表中
    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(true);
        
        UE_LOG(LogTemp, Verbose, TEXT("Unit %d selected (Total: %d)"),
            Unit->UnitId, SelectedUnits.Num());
        
        // 發布選擇變更事件
        PublishSelectionChangedEvent();
    }
}

void UMingSelectionManager::SelectUnits(const TArray<AMingTacticalUnit*>& Units, bool bAdditive)
{
    if (!bAdditive)
    {
        DeselectAll();
    }
    
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit && Unit->IsAlive() && !SelectedUnits.Contains(Unit))
        {
            SelectedUnits.Add(Unit);
            Unit->SetSelected(true);
        }
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Selected %d units (Total: %d)"),
        Units.Num(), SelectedUnits.Num());
    
    PublishSelectionChangedEvent();
}

void UMingSelectionManager::DeselectUnit(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    if (SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Remove(Unit);
        Unit->SetSelected(false);
        
        UE_LOG(LogTemp, Verbose, TEXT("Unit %d deselected (Total: %d)"),
            Unit->UnitId, SelectedUnits.Num());
        
        PublishSelectionChangedEvent();
    }
}

void UMingSelectionManager::DeselectAll()
{
    if (SelectedUnits.Num() == 0)
    {
        return;
    }
    
    // 取消所有單位的選擇狀態
    for (AMingTacticalUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
        }
    }
    
    int32 PreviousCount = SelectedUnits.Num();
    SelectedUnits.Empty();
    
    UE_LOG(LogTemp, Verbose, TEXT("Deselected all %d units"), PreviousCount);
    
    PublishSelectionChangedEvent();
}

void UMingSelectionManager::StartBoxSelection(const FVector2D& StartPosition)
{
    SelectionBox.StartScreenPosition = StartPosition;
    SelectionBox.CurrentScreenPosition = StartPosition;
    SelectionBox.bIsDrawing = true;
    
    UE_LOG(LogTemp, Verbose, TEXT("Box selection started at (%f, %f)"),
        StartPosition.X, StartPosition.Y);
}

void UMingSelectionManager::UpdateBoxSelection(const FVector2D& CurrentPosition)
{
    if (!SelectionBox.bIsDrawing)
    {
        return;
    }
    
    SelectionBox.CurrentScreenPosition = CurrentPosition;
    
    // 實時更新選擇（可選）
    // UpdateBoxSelectedUnits();
}

void UMingSelectionManager::EndBoxSelection()
{
    if (!SelectionBox.bIsDrawing)
    {
        return;
    }
    
    // 執行框選
    UpdateBoxSelectedUnits();
    
    SelectionBox.bIsDrawing = false;
    
    UE_LOG(LogTemp, Verbose, TEXT("Box selection ended, selected %d units"),
        SelectedUnits.Num());
}

void UMingSelectionManager::UpdateBoxSelectedUnits()
{
    // 獲取框選區域
    FVector2D MinPoint(
        FMath::Min(SelectionBox.StartScreenPosition.X, SelectionBox.CurrentScreenPosition.X),
        FMath::Min(SelectionBox.StartScreenPosition.Y, SelectionBox.CurrentScreenPosition.Y)
    );
    
    FVector2D MaxPoint(
        FMath::Max(SelectionBox.StartScreenPosition.X, SelectionBox.CurrentScreenPosition.X),
        FMath::Max(SelectionBox.StartScreenPosition.Y, SelectionBox.CurrentScreenPosition.Y)
    );
    
    // 獲取所有單位
    TArray<AActor*> AllUnits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMingTacticalUnit::StaticClass(), AllUnits);
    
    // 收集在選擇框內的單位
    TArray<AMingTacticalUnit*> UnitsInBox;
    
    for (AActor* Actor : AllUnits)
    {
        AMingTacticalUnit* Unit = Cast<AMingTacticalUnit>(Actor);
        if (Unit && Unit->IsAlive())
        {
            FVector2D UnitScreenPos = GetUnitScreenPosition(Unit);
            
            // 檢查單位是否在選擇框內
            if (UnitScreenPos.X >= MinPoint.X && UnitScreenPos.X <= MaxPoint.X &&
                UnitScreenPos.Y >= MinPoint.Y && UnitScreenPos.Y <= MaxPoint.Y)
            {
                UnitsInBox.Add(Unit);
            }
        }
    }
    
    // 選擇框內的單位
    if (UnitsInBox.Num() > 0)
    {
        SelectUnits(UnitsInBox, false); // 替換當前選擇
    }
    else
       {
        // 如果框內沒有單位，取消所有選擇
        DeselectAll();
    }
}

bool UMingSelectionManager::IsUnitInSelectionBox(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return false;
    }
    
    FVector2D UnitScreenPos = GetUnitScreenPosition(Unit);
    
    FVector2D MinPoint(
        FMath::Min(SelectionBox.StartScreenPosition.X, SelectionBox.CurrentScreenPosition.X),
        FMath::Min(SelectionBox.StartScreenPosition.Y, SelectionBox.CurrentScreenPosition.Y)
    );
    
    FVector2D MaxPoint(
        FMath::Max(SelectionBox.StartScreenPosition.X, SelectionBox.CurrentScreenPosition.X),
        FMath::Max(SelectionBox.StartScreenPosition.Y, SelectionBox.CurrentScreenPosition.Y)
    );
    
    return (UnitScreenPos.X >= MinPoint.X && UnitScreenPos.X <= MaxPoint.X &&
            UnitScreenPos.Y >= MinPoint.Y && UnitScreenPos.Y <= MaxPoint.Y);
}

FVector2D UMingSelectionManager::GetUnitScreenPosition(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return FVector2D::ZeroVector;
    }
    
    // 獲取玩家控制器和視口
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return FVector2D::ZeroVector;
    }
    
    FVector UnitLocation = Unit->GetActorLocation();
    FVector2D ScreenPosition;
    
    if (PlayerController->ProjectWorldLocationToScreen(UnitLocation, ScreenPosition))
    {
        return ScreenPosition;
    }
    
    return FVector2D::ZeroVector;
}

void UMingSelectionManager::PublishSelectionChangedEvent()
{
    // 發布選擇變更事件
    // TODO: 定義一個選擇變更事件並發布
    // FSelectionChangedEvent Event(SelectedUnits.Num());
    // IMingCoreEventBus::PublishEvent(Event);
    
    UE_LOG(LogTemp, Verbose, TEXT("Selection changed: %d units selected"), SelectedUnits.Num());
}

UMingSelectionManager* UMingSelectionManager::Get()
{
    return Instance;
}
