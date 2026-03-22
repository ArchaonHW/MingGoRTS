#include "MingRTSUnitSelector.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/LocalPlayer.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"

UMingRTSUnitSelector::UMingRTSUnitSelector()
{
    CurrentWorld = nullptr;
    MaxSelectionCount = 100;
    bIsSelecting = false;
    SelectionBoxColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.3f);
    SelectionBoxLineWidth = 2.0f;
    SelectionHighlightColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f);
}

void UMingRTSUnitSelector::InitializeSelector(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSUnitSelector: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    ClearSelection();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSUnitSelector initialized successfully"));
}

void UMingRTSUnitSelector::StartSelection(FVector2D ScreenPosition, ESelectionType Type)
{
    if (!CurrentWorld)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSUnitSelector: Cannot start selection without valid world"));
        return;
    }

    // 清除之前的選擇（除非是添加模式）
    if (Type != ESelectionType::Group)
    {
        ClearSelection();
    }

    CurrentSelection.SelectionStart = ScreenPosition;
    CurrentSelection.SelectionEnd = ScreenPosition;
    CurrentSelection.SelectionType = Type;
    bIsSelecting = true;

    UE_LOG(LogTemp, Log, TEXT("Started selection at position: %s, Type: %d"), 
        *ScreenPosition.ToString(), (int32)Type);
}

void UMingRTSUnitSelector::UpdateSelection(FVector2D ScreenPosition)
{
    if (!bIsSelecting || !CurrentWorld)
    {
        return;
    }

    CurrentSelection.SelectionEnd = ScreenPosition;

    // 如果是框選模式，實時更新選中的單位
    if (CurrentSelection.SelectionType == ESelectionType::Box)
    {
        TArray<AActor*> UnitsInBox = GetUnitsInSelectionBox(
            CurrentSelection.SelectionStart, 
            CurrentSelection.SelectionEnd
        );
        
        UnitsInBox = ApplySelectionFilter(UnitsInBox);

        // 檢查最大選擇數量限制
        if (UnitsInBox.Num() > MaxSelectionCount)
        {
            UnitsInBox.SetNum(MaxSelectionCount);
        }

        CurrentSelection.SelectedUnits = UnitsInBox;
        UpdateSelectionVisuals();
        TriggerSelectionChanged();
    }

    // 繪製選擇框（僅在編輯器中）
    #if WITH_EDITOR
    if (CurrentWorld && CurrentWorld->IsEditorWorld())
    {
        DrawSelectionBox();
    }
    #endif
}

void UMingRTSUnitSelector::FinishSelection()
{
    if (!bIsSelecting || !CurrentWorld)
    {
        return;
    }

    bIsSelecting = false;

    // 根據選擇類型處理最終選擇
    switch (CurrentSelection.SelectionType)
    {
        case ESelectionType::Single:
        {
            // 單擊選擇最近的單位
            FVector WorldPos = ScreenToWorld(CurrentSelection.SelectionEnd);
            TArray<AActor*> NearbyUnits;
            
            // 球形檢測附近的單位
            TArray<FHitResult> HitResults;
            FCollisionShape CollisionShape;
            CollisionShape.SetSphere(100.0f);
            
            if (CurrentWorld->SweepMultiByChannel(
                HitResults, 
                WorldPos, 
                WorldPos, 
                FQuat::Identity, 
                ECC_Visibility, 
                CollisionShape))
            {
                for (const FHitResult& Hit : HitResults)
                {
                    AActor* HitActor = Hit.GetActor();
                    if (HitActor && IsUnitSelectable(HitActor))
                    {
                        NearbyUnits.Add(HitActor);
                    }
                }
            }

            NearbyUnits = ApplySelectionFilter(NearbyUnits);
            
            if (NearbyUnits.Num() > 0)
            {
                // 選擇最近的單位
                AActor* ClosestUnit = NearbyUnits[0];
                float ClosestDistance = FVector::Dist(WorldPos, ClosestUnit->GetActorLocation());
                
                for (AActor* Unit : NearbyUnits)
                {
                    float Distance = FVector::Dist(WorldPos, Unit->GetActorLocation());
                    if (Distance < ClosestDistance)
                    {
                        ClosestDistance = Distance;
                        ClosestUnit = Unit;
                    }
                }
                
                CurrentSelection.SelectedUnits.Add(ClosestUnit);
                OnUnitSelected.Broadcast(ClosestUnit, true);
            }
            break;
        }

        case ESelectionType::Box:
        {
            // 框選已在 UpdateSelection 中處理
            break;
        }

        case ESelectionType::Multiple:
        {
            // 多選模式：添加框選區域內的所有單位
            TArray<AActor*> UnitsInBox = GetUnitsInSelectionBox(
                CurrentSelection.SelectionStart, 
                CurrentSelection.SelectionEnd
            );
            
            UnitsInBox = ApplySelectionFilter(UnitsInBox);

            // 檢查最大選擇數量限制
            for (AActor* Unit : UnitsInBox)
            {
                if (CurrentSelection.SelectedUnits.Num() >= MaxSelectionCount)
                {
                    break;
                }
                
                if (!CurrentSelection.SelectedUnits.Contains(Unit))
                {
                    CurrentSelection.SelectedUnits.Add(Unit);
                    OnUnitSelected.Broadcast(Unit, true);
                }
            }
            break;
        }

        case ESelectionType::Group:
        {
            // 分組選擇：保持現有選擇，添加新單位
            TArray<AActor*> UnitsInBox = GetUnitsInSelectionBox(
                CurrentSelection.SelectionStart, 
                CurrentSelection.SelectionEnd
            );
            
            UnitsInBox = ApplySelectionFilter(UnitsInBox);

            // 添加到現有選擇
            for (AActor* Unit : UnitsInBox)
            {
                if (CurrentSelection.SelectedUnits.Num() >= MaxSelectionCount)
                {
                    break;
                }
                
                if (!CurrentSelection.SelectedUnits.Contains(Unit))
                {
                    CurrentSelection.SelectedUnits.Add(Unit);
                    OnUnitSelected.Broadcast(Unit, true);
                }
            }
            break;
        }
    }

    UpdateSelectionVisuals();
    TriggerSelectionChanged();

    UE_LOG(LogTemp, Log, TEXT("Finished selection. Selected %d units"), 
        CurrentSelection.SelectedUnits.Num());
}

void UMingRTSUnitSelector::AddUnitToSelection(AActor* Unit, bool bAddToExisting)
{
    if (!Unit || !IsUnitSelectable(Unit))
    {
        return;
    }

    if (!bAddToExisting)
    {
        ClearSelection();
    }

    if (!CurrentSelection.SelectedUnits.Contains(Unit) && 
        CurrentSelection.SelectedUnits.Num() < MaxSelectionCount)
    {
        CurrentSelection.SelectedUnits.Add(Unit);
        OnUnitSelected.Broadcast(Unit, true);
        UpdateSelectionVisuals();
        TriggerSelectionChanged();
    }
}

void UMingRTSUnitSelector::RemoveUnitFromSelection(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }

    if (CurrentSelection.SelectedUnits.Remove(Unit) > 0)
    {
        OnUnitSelected.Broadcast(Unit, false);
        UpdateSelectionVisuals();
        TriggerSelectionChanged();
    }
}

void UMingRTSUnitSelector::ClearSelection()
{
    for (AActor* Unit : CurrentSelection.SelectedUnits)
    {
        OnUnitSelected.Broadcast(Unit, false);
    }

    CurrentSelection.SelectedUnits.Empty();
    CurrentSelection.SelectionType = ESelectionType::Single;
    bIsSelecting = false;
    
    UpdateSelectionVisuals();
    TriggerSelectionChanged();
    
    UE_LOG(LogTemp, Log, TEXT("Selection cleared"));
}

void UMingRTSUnitSelector::SelectAllUnits()
{
    if (!CurrentWorld)
    {
        return;
    }

    ClearSelection();

    // 獲取世界中所有可選單位
    TArray<AActor*> AllUnits;
    for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
    {
        AActor* Actor = *It;
        if (IsUnitSelectable(Actor))
        {
            AllUnits.Add(Actor);
        }
    }

    AllUnits = ApplySelectionFilter(AllUnits);

    // 應用最大選擇數量限制
    if (AllUnits.Num() > MaxSelectionCount)
    {
        AllUnits.SetNum(MaxSelectionCount);
    }

    CurrentSelection.SelectedUnits = AllUnits;
    CurrentSelection.SelectionType = ESelectionType::Multiple;

    for (AActor* Unit : AllUnits)
    {
        OnUnitSelected.Broadcast(Unit, true);
    }

    UpdateSelectionVisuals();
    TriggerSelectionChanged();

    UE_LOG(LogTemp, Log, TEXT("Selected all units: %d units"), AllUnits.Num());
}

void UMingRTSUnitSelector::SetSelectionFilter(TSubclassOf<AActor> FilterClass)
{
    SelectionFilterClass = FilterClass;
    
    // 如果有現有選擇，重新應用過濾器
    if (CurrentSelection.SelectedUnits.Num() > 0)
    {
        TArray<AActor*> FilteredUnits = ApplySelectionFilter(CurrentSelection.SelectedUnits);
        CurrentSelection.SelectedUnits = FilteredUnits;
        UpdateSelectionVisuals();
        TriggerSelectionChanged();
    }
}

void UMingRTSUnitSelector::CreateUnitGroup(const FString& GroupName, const TArray<AActor*>& Units)
{
    if (GroupName.IsEmpty() || Units.Num() == 0)
    {
        return;
    }

    UnitGroups.Add(GroupName, Units);
    
    UE_LOG(LogTemp, Log, TEXT("Created unit group '%s' with %d units"), 
        *GroupName, Units.Num());
}

void UMingRTSUnitSelector::SelectUnitGroup(const FString& GroupName)
{
    TArray<AActor*>* GroupUnits = UnitGroups.Find(GroupName);
    if (!GroupUnits || GroupUnits->Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit group '%s' not found or empty"), *GroupName);
        return;
    }

    ClearSelection();
    CurrentSelection.SelectedUnits = *GroupUnits;
    CurrentSelection.SelectionType = ESelectionType::Group;

    for (AActor* Unit : *GroupUnits)
    {
        OnUnitSelected.Broadcast(Unit, true);
    }

    UpdateSelectionVisuals();
    TriggerSelectionChanged();

    UE_LOG(LogTemp, Log, TEXT("Selected unit group '%s': %d units"), 
        *GroupName, GroupUnits->Num());
}

TArray<FString> UMingRTSUnitSelector::GetUnitGroups() const
{
    TArray<FString> GroupNames;
    UnitGroups.GetKeys(GroupNames);
    return GroupNames;
}

bool UMingRTSUnitSelector::IsUnitSelectable(AActor* Unit) const
{
    if (!Unit)
    {
        return false;
    }

    // 檢查過濾器
    if (SelectionFilterClass && !Unit->IsA(SelectionFilterClass))
    {
        return false;
    }

    // 檢查單位是否有效（可根據需要添加更多條件）
    if (Unit->IsPendingKill())
    {
        return false;
    }

    // 檢查單位是否在遊戲世界中
    if (!Unit->GetWorld())
    {
        return false;
    }

    return true;
}

FVector UMingRTSUnitSelector::ScreenToWorld(FVector2D ScreenPosition)
{
    if (!CurrentWorld)
    {
        return FVector::ZeroVector;
    }

    APlayerController* PlayerController = CurrentWorld->GetFirstPlayerController();
    if (!PlayerController)
    {
        return FVector::ZeroVector;
    }

    FVector WorldLocation, WorldDirection;
    if (PlayerController->DeprojectScreenPositionToWorld(
        ScreenPosition.X, 
        ScreenPosition.Y, 
        WorldLocation, 
        WorldDirection))
    {
        // 射線檢測到地面
        FHitResult HitResult;
        FVector EndLocation = WorldLocation + WorldDirection * 10000.0f;
        
        if (CurrentWorld->LineTraceSingleByChannel(
            HitResult, 
            WorldLocation, 
            EndLocation, 
            ECC_Visibility))
        {
            return HitResult.Location;
        }
    }

    return WorldLocation;
}

TArray<AActor*> UMingRTSUnitSelector::GetUnitsInSelectionBox(FVector2D Start, FVector2D End)
{
    TArray<AActor*> UnitsInBox;

    if (!CurrentWorld)
    {
        return UnitsInBox;
    }

    // 獲取框選的矩形區域
    FVector2D MinPoint(FMath::Min(Start.X, End.X), FMath::Min(Start.Y, End.Y));
    FVector2D MaxPoint(FMath::Max(Start.X, End.X), FMath::Max(Start.Y, End.Y));

    // 遍歷所有可能的單位
    for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
    {
        AActor* Actor = *It;
        if (!IsUnitSelectable(Actor))
        {
            continue;
        }

        // 檢查單位是否在框選區域內
        if (IsUnitInBox(Actor, MinPoint, MaxPoint))
        {
            UnitsInBox.Add(Actor);
        }
    }

    return UnitsInBox;
}

TArray<AActor*> UMingRTSUnitSelector::ApplySelectionFilter(const TArray<AActor*>& Units) const
{
    if (!SelectionFilterClass)
    {
        return Units;
    }

    TArray<AActor*> FilteredUnits;
    for (AActor* Unit : Units)
    {
        if (Unit->IsA(SelectionFilterClass))
        {
            FilteredUnits.Add(Unit);
        }
    }

    return FilteredUnits;
}

void UMingRTSUnitSelector::UpdateSelectionVisuals()
{
    // 這裡可以添加選擇高亮視覺效果
    // 例如：為選中的單位添加高亮材質、輪廓等
    
    for (AActor* Unit : CurrentSelection.SelectedUnits)
    {
        if (Unit)
        {
            // 添加選擇高亮效果
            // 可以通過材質參數、後處理效果等方式實現
        }
    }
}

void UMingRTSUnitSelector::TriggerSelectionChanged()
{
    OnSelectionChanged.Broadcast(CurrentSelection);
}

bool UMingRTSUnitSelector::IsUnitInBox(AActor* Unit, FVector2D BoxStart, FVector2D BoxEnd) const
{
    if (!Unit || !CurrentWorld)
    {
        return false;
    }

    APlayerController* PlayerController = CurrentWorld->GetFirstPlayerController();
    if (!PlayerController)
    {
        return false;
    }

    // 獲取單位的屏幕位置
    FVector2D UnitScreenPos = GetUnitScreenPosition(Unit);
    
    // 檢查是否在框選矩形內
    return UnitScreenPos.X >= BoxStart.X && UnitScreenPos.X <= BoxEnd.X &&
           UnitScreenPos.Y >= BoxStart.Y && UnitScreenPos.Y <= BoxEnd.Y;
}

FVector2D UMingRTSUnitSelector::GetUnitScreenPosition(AActor* Unit) const
{
    if (!Unit || !CurrentWorld)
    {
        return FVector2D::ZeroVector;
    }

    APlayerController* PlayerController = CurrentWorld->GetFirstPlayerController();
    if (!PlayerController)
    {
        return FVector2D::ZeroVector;
    }

    FVector2D ScreenPosition;
    if (PlayerController->ProjectWorldLocationToScreen(Unit->GetActorLocation(), ScreenPosition))
    {
        return ScreenPosition;
    }

    return FVector2D::ZeroVector;
}

bool UMingRTSUnitSelector::ValidateSelection() const
{
    // 清理無效單位
    for (int32 i = CurrentSelection.SelectedUnits.Num() - 1; i >= 0; --i)
    {
        AActor* Unit = CurrentSelection.SelectedUnits[i];
        if (!Unit || Unit->IsPendingKill() || !IsUnitSelectable(Unit))
        {
            return false;
        }
    }

    return CurrentSelection.SelectedUnits.Num() <= MaxSelectionCount;
}

void UMingRTSUnitSelector::CleanupInvalidUnits()
{
    for (int32 i = CurrentSelection.SelectedUnits.Num() - 1; i >= 0; --i)
    {
        AActor* Unit = CurrentSelection.SelectedUnits[i];
        if (!Unit || Unit->IsPendingKill() || !IsUnitSelectable(Unit))
        {
            CurrentSelection.SelectedUnits.RemoveAt(i);
        }
    }
}

#if WITH_EDITOR
void UMingRTSUnitSelector::DrawSelectionBox()
{
    if (!CurrentWorld || !bIsSelecting)
    {
        return;
    }

    APlayerController* PlayerController = CurrentWorld->GetFirstPlayerController();
    if (!PlayerController)
    {
        return;
    }

    // 獲取框選的矩形頂點
    FVector StartWorld = ScreenToWorld(CurrentSelection.SelectionStart);
    FVector EndWorld = ScreenToWorld(CurrentSelection.SelectionEnd);
    
    // 簡化的框選繪製（實際項目中可能需要更複雜的實現）
    DrawDebugSphere(CurrentWorld, StartWorld, 50.0f, 16, SelectionBoxColor.ToFColor(true));
    DrawDebugSphere(CurrentWorld, EndWorld, 50.0f, 16, SelectionBoxColor.ToFColor(true));
    DrawDebugLine(CurrentWorld, StartWorld, EndWorld, SelectionBoxColor.ToFColor(true), false, -1.0f, 0, SelectionBoxLineWidth);
}
#endif
