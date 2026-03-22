#include "MingRTSUnitSelector.h"
#include "MingRTSUnitManager.h"
#include "MingGoRTSPlayerController.h"
#include "MingGoRTSUnit.h"
#include "Engine/World.h"
#include "Engine/PlayerController.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SceneComponent.h"

UMingRTSUnitSelector::UMingRTSUnitSelector()
{
    PlayerController = nullptr;
    UnitManager = nullptr;
    bIsSelecting = false;
    SelectionStartPoint = FVector2D::ZeroVector;
    SelectionEndPoint = FVector2D::ZeroVector;
}

void UMingRTSUnitSelector::InitializeSelector(AMingGoRTSPlayerController* InPlayerController)
{
    PlayerController = InPlayerController;
    
    if (PlayerController)
    {
        UnitManager = NewObject<UMingRTSUnitManager>();
        UnitManager->InitializeUnitManager(PlayerController);
    }
}

void UMingRTSUnitSelector::SelectUnit(AMingGoRTSUnit* Unit, bool bAddToSelection)
{
    if (!Unit || !UnitManager)
    {
        return;
    }

    TArray<AMingGoRTSUnit*> Units;
    Units.Add(Unit);
    SelectUnits(Units, bAddToSelection);
}

void UMingRTSUnitSelector::SelectUnits(const TArray<AMingGoRTSUnit*>& Units, bool bAddToSelection)
{
    if (!ValidateSelection(Units))
    {
        return;
    }

    TArray<AMingGoRTSUnit*> NewSelection = Units;
    
    if (bAddToSelection)
    {
        // 添加到現有選擇
        for (AMingGoRTSUnit* Unit : Units)
        {
            if (!CurrentSelection.SelectedUnits.Contains(Unit))
            {
                NewSelection.Add(Unit);
            }
        }
    }

    UpdateCurrentSelection(NewSelection, bAddToSelection ? ERTSSelectionType::Multiple : ERTSSelectionType::Single);
    
    if (bAutoSaveSelectionHistory)
    {
        SaveSelectionToHistory();
    }
}

void UMingRTSUnitSelector::SelectUnitsInBox(FVector2D ScreenStart, FVector2D ScreenEnd, bool bAddToSelection)
{
    if (!UnitManager)
    {
        return;
    }

    TArray<AMingGoRTSUnit*> UnitsInBox = GetUnitsInBox(ScreenStart, ScreenEnd);
    SelectUnits(UnitsInBox, bAddToSelection);
}

void UMingRTSUnitSelector::DeselectAllUnits()
{
    if (CurrentSelection.SelectedUnits.Num() > 0)
    {
        ERTSSelectionType PreviousType = CurrentSelection.SelectionType;
        ClearSelectionInternal();
        OnSelectionCleared.Broadcast(PreviousType);
    }
}

void UMingRTSUnitSelector::DeselectUnit(AMingGoRTSUnit* Unit)
{
    if (!Unit || !CurrentSelection.SelectedUnits.Contains(Unit))
    {
        return;
    }

    TArray<AMingGoRTSUnit*> NewSelection = CurrentSelection.SelectedUnits;
    NewSelection.Remove(Unit);
    UpdateCurrentSelection(NewSelection, ERTSSelectionType::Multiple);
}

bool UMingRTSUnitSelector::IsUnitSelected(AMingGoRTSUnit* Unit) const
{
    return CurrentSelection.SelectedUnits.Contains(Unit);
}

AMingGoRTSUnit* UMingRTSUnitSelector::GetPrimarySelection() const
{
    if (CurrentSelection.SelectedUnits.Num() > 0)
    {
        return CurrentSelection.SelectedUnits[0];
    }
    return nullptr;
}

TArray<AMingGoRTSUnit*> UMingRTSUnitSelector::FilterSelection(ERTSSelectionFilter FilterType) const
{
    TArray<AMingGoRTSUnit*> FilteredUnits;

    for (AMingGoRTSUnit* Unit : CurrentSelection.SelectedUnits)
    {
        if (!Unit)
        {
            continue;
        }

        bool bInclude = false;
        switch (FilterType)
        {
        case ERTSSelectionFilter::All:
            bInclude = true;
            break;
        case ERTSSelectionFilter::SameType:
            if (CurrentSelection.SelectedUnits.Num() > 0)
            {
                bInclude = (Unit->GetUnitType() == CurrentSelection.SelectedUnits[0]->GetUnitType());
            }
            break;
        case ERTSSelectionFilter::SameState:
            if (CurrentSelection.SelectedUnits.Num() > 0)
            {
                bInclude = (Unit->GetUnitState() == CurrentSelection.SelectedUnits[0]->GetUnitState());
            }
            break;
        case ERTSSelectionFilter::Damaged:
            bInclude = Unit->IsDamaged();
            break;
        case ERTSSelectionFilter::Idle:
            bInclude = (Unit->GetUnitState() == ERTSUnitState::Idle);
            break;
        }

        if (bInclude)
        {
            FilteredUnits.Add(Unit);
        }
    }

    return FilteredUnits;
}

void UMingRTSUnitSelector::ApplySelectionFilter(ERTSSelectionFilter FilterType)
{
    TArray<AMingGoRTSUnit*> FilteredUnits = FilterSelection(FilterType);
    UpdateCurrentSelection(FilteredUnits, ERTSSelectionType::Multiple);
}

bool UMingRTSUnitSelector::IsUnitInSelectionBox(AMingGoRTSUnit* Unit, FVector2D BoxStart, FVector2D BoxEnd) const
{
    if (!Unit || !PlayerController)
    {
        return false;
    }

    FVector UnitLocation = Unit->GetActorLocation();
    FVector2D UnitScreenPos = WorldToScreen(UnitLocation);

    return IsPointInBox(UnitScreenPos, BoxStart, BoxEnd);
}

TArray<AMingGoRTSUnit*> UMingRTSUnitSelector::GetUnitsInBox(FVector2D BoxStart, FVector2D BoxEnd) const
{
    TArray<AMingGoRTSUnit*> UnitsInBox;

    if (!UnitManager)
    {
        return UnitsInBox;
    }

    TArray<AMingGoRTSUnit*> AllUnits = UnitManager->GetAllUnits();

    for (AMingGoRTSUnit* Unit : AllUnits)
    {
        if (IsUnitInSelectionBox(Unit, BoxStart, BoxEnd))
        {
            UnitsInBox.Add(Unit);
        }
    }

    return UnitsInBox;
}

void UMingRTSUnitSelector::CreateUnitGroup(const FString& GroupName, const TArray<AMingGoRTSUnit*>& Units)
{
    if (GroupName.IsEmpty())
    {
        return;
    }

    UnitGroups.Add(GroupName, Units);
}

void UMingRTSUnitSelector::SelectUnitGroup(const FString& GroupName, bool bAddToSelection)
{
    TArray<AMingGoRTSUnit*>* GroupUnits = UnitGroups.Find(GroupName);
    if (GroupUnits)
    {
        SelectUnits(*GroupUnits, bAddToSelection);
    }
}

TArray<AMingGoRTSUnit*> UMingRTSUnitSelector::GetUnitGroup(const FString& GroupName) const
{
    TArray<AMingGoRTSUnit*>* GroupUnits = UnitGroups.Find(GroupName);
    if (GroupUnits)
    {
        return *GroupUnits;
    }
    return TArray<AMingGoRTSUnit*>();
}

TArray<FString> UMingRTSUnitSelector::GetAllUnitGroups() const
{
    TArray<FString> GroupNames;
    UnitGroups.GetKeys(GroupNames);
    return GroupNames;
}

void UMingRTSUnitSelector::SaveSelectionToHistory()
{
    if (SelectionHistory.Num() >= MaxSelectionHistory)
    {
        SelectionHistory.RemoveAt(0);
    }
    SelectionHistory.Add(CurrentSelection);
}

void UMingRTSUnitSelector::RestoreSelectionFromHistory(int32 HistoryIndex)
{
    if (SelectionHistory.Num() == 0)
    {
        return;
    }

    int32 Index = HistoryIndex;
    if (Index < 0)
    {
        Index = SelectionHistory.Num() - 1;
    }

    if (Index >= 0 && Index < SelectionHistory.Num())
    {
        FRTSSelectionData HistorySelection = SelectionHistory[Index];
        UpdateCurrentSelection(HistorySelection.SelectedUnits, HistorySelection.SelectionType);
    }
}

FVector UMingRTSUnitSelector::GetSelectionCenter() const
{
    return CurrentSelection.SelectionCenter;
}

float UMingRTSUnitSelector::GetSelectionRadius() const
{
    return CurrentSelection.SelectionRadius;
}

void UMingRTSUnitSelector::ShowSelectionEffects()
{
    if (!bShowSelectionEffects)
    {
        return;
    }

    UpdateSelectionVisuals();
}

void UMingRTSUnitSelector::HideSelectionEffects()
{
    // 隱藏所有選擇視覺效果
    for (AMingGoRTSUnit* Unit : CurrentSelection.SelectedUnits)
    {
        if (Unit)
        {
            Unit->HideSelectionEffect();
        }
    }
}

void UMingRTSUnitSelector::UpdateSelectionVisuals()
{
    // 更新選擇視覺效果
    for (AMingGoRTSUnit* Unit : CurrentSelection.SelectedUnits)
    {
        if (Unit)
        {
            Unit->ShowSelectionEffect();
        }
    }
}

void UMingRTSUnitSelector::UpdateCurrentSelection(const TArray<AMingGoRTSUnit*>& Units, ERTSSelectionType SelectionType)
{
    // 清除之前的選擇效果
    HideSelectionEffects();

    // 更新選擇數據
    CurrentSelection.SelectedUnits = Units;
    CurrentSelection.SelectionType = SelectionType;
    CurrentSelection.SelectionTime = FDateTime::Now();

    // 計算選擇中心和半徑
    if (Units.Num() > 0)
    {
        FVector Center = FVector::ZeroVector;
        float MaxDistance = 0.0f;

        for (AMingGoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Center += Unit->GetActorLocation();
            }
        }

        Center /= Units.Num();
        CurrentSelection.SelectionCenter = Center;

        // 計算最大距離
        for (AMingGoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                float Distance = FVector::Dist(Unit->GetActorLocation(), Center);
                MaxDistance = FMath::Max(MaxDistance, Distance);
            }
        }
        CurrentSelection.SelectionRadius = MaxDistance;
    }
    else
    {
        CurrentSelection.SelectionCenter = FVector::ZeroVector;
        CurrentSelection.SelectionRadius = 0.0f;
    }

    // 通知選擇變化
    NotifySelectionChanged();

    // 顯示新的選擇效果
    if (bShowSelectionEffects)
    {
        ShowSelectionEffects();
    }
}

void UMingRTSUnitSelector::ClearSelectionInternal()
{
    HideSelectionEffects();
    CurrentSelection.SelectedUnits.Empty();
    CurrentSelection.SelectionType = ERTSSelectionType::Single;
    CurrentSelection.SelectionCenter = FVector::ZeroVector;
    CurrentSelection.SelectionRadius = 0.0f;
    CurrentSelection.SelectionTime = FDateTime::Now();
}

bool UMingRTSUnitSelector::ValidateSelection(const TArray<AMingGoRTSUnit*>& Units) const
{
    if (Units.Num() == 0)
    {
        return false;
    }

    // 檢查所有單位是否有效
    for (AMingGoRTSUnit* Unit : Units)
    {
        if (!Unit || !Unit->IsValidLowLevel())
        {
            return false;
        }
    }

    // 檢查選擇半徑限制
    if (Units.Num() > 1)
    {
        FVector Center = FVector::ZeroVector;
        for (AMingGoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Center += Unit->GetActorLocation();
            }
        }
        Center /= Units.Num();

        for (AMingGoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                float Distance = FVector::Dist(Unit->GetActorLocation(), Center);
                if (Distance > MaxSelectionRadius)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

void UMingRTSUnitSelector::NotifySelectionChanged()
{
    OnUnitsSelected.Broadcast(CurrentSelection.SelectedUnits, CurrentSelection.SelectionType);
}

FVector2D UMingRTSUnitSelector::WorldToScreen(const FVector& WorldLocation) const
{
    if (!PlayerController)
    {
        return FVector2D::ZeroVector;
    }

    FVector2D ScreenLocation;
    PlayerController->ProjectWorldLocationToScreen(WorldLocation, ScreenLocation);
    return ScreenLocation;
}

FVector UMingRTSUnitSelector::ScreenToWorld(const FVector2D& ScreenLocation, float Z) const
{
    if (!PlayerController)
    {
        return FVector::ZeroVector;
    }

    FVector WorldLocation;
    FVector WorldDirection;
    PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);

    // 計算指定Z高度的世界位置
    if (WorldDirection.Z != 0.0f)
    {
        float T = (Z - WorldLocation.Z) / WorldDirection.Z;
        WorldLocation += WorldDirection * T;
    }

    return WorldLocation;
}

bool UMingRTSUnitSelector::IsPointInBox(FVector2D Point, FVector2D BoxStart, FVector2D BoxEnd) const
{
    FVector2D Min = FVector2D(FMath::Min(BoxStart.X, BoxEnd.X), FMath::Min(BoxStart.Y, BoxEnd.Y));
    FVector2D Max = FVector2D(FMath::Max(BoxStart.X, BoxEnd.X), FMath::Max(BoxStart.Y, BoxEnd.Y));

    return (Point.X >= Min.X && Point.X <= Max.X && Point.Y >= Min.Y && Point.Y <= Max.Y);
}
