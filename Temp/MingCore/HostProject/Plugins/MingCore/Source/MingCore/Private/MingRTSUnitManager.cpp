#include "MingRTSUnitManager.h"
#include "MingGoRTSUnit.h"
#include "MingGoRTSPlayerController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"

UMingRTSUnitManager::UMingRTSUnitManager()
    : CurrentFormation(ERTSFormationType::None)
    , bIsInitialized(false)
    , SelectionBoxMargin(50.0f)
    , FormationSpacing(150.0f)
    , bAutoFormation(true)
{
}

void UMingRTSUnitManager::InitializeUnitManager(AMingGoRTSPlayerController* InPlayerController)
{
    PlayerController = InPlayerController;
    
    if (PlayerController)
    {
        bIsInitialized = true;
        UE_LOG(LogTemp, Log, TEXT("RTS Unit Manager initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RTS Unit Manager initialization failed - PlayerController is null"));
    }
}

void UMingRTSUnitManager::RegisterUnit(AMingGoRTSUnit* Unit)
{
    if (!Unit || !bIsInitialized)
    {
        return;
    }

    FString UnitID = GenerateUnitID(ERTSUnitType::Infantry); // 應該從Unit獲取實際類型
    
    if (!AllUnits.Contains(UnitID))
    {
        AllUnits.Add(UnitID, Unit);
        
        // 初始化單位數據
        FRTSUnitData UnitData;
        UnitData.UnitID = UnitID;
        UnitData.CurrentLocation = Unit->GetActorLocation();
        UnitData.bIsSelected = false;
        
        UE_LOG(LogTemp, Log, TEXT("Registered unit: %s"), *UnitID);
    }
}

void UMingRTSUnitManager::UnregisterUnit(AMingGoRTSUnit* Unit)
{
    if (!Unit)
    {
        return;
    }

    // 從所有映射中移除單位
    for (auto It = AllUnits.CreateIterator(); It; ++It)
    {
        if (It->Value == Unit)
        {
            AllUnits.Remove(It->Key);
            break;
        }
    }

    // 從選擇列表中移除
    SelectedUnits.Remove(Unit);
    
    // 從群組中移除
    for (auto& GroupPair : UnitGroups)
    {
        GroupPair.Value.Remove(Unit);
    }

    UE_LOG(LogTemp, Log, TEXT("Unregistered unit"));
}

AMingGoRTSUnit* UMingRTSUnitManager::GetUnit(const FString& UnitID) const
{
    if (AllUnits.Contains(UnitID))
    {
        return AllUnits[UnitID];
    }
    return nullptr;
}

TArray<AMingGoRTSUnit*> UMingRTSUnitManager::GetAllUnits() const
{
    TArray<AMingGoRTSUnit*> Units;
    for (const auto& UnitPair : AllUnits)
    {
        if (UnitPair.Value)
        {
            Units.Add(UnitPair.Value);
        }
    }
    return Units;
}

TArray<AMingGoRTSUnit*> UMingRTSUnitManager::GetUnitsByType(ERTSUnitType UnitType) const
{
    TArray<AMingGoRTSUnit*> Units;
    for (const auto& UnitPair : AllUnits)
    {
        if (UnitPair.Value)
        {
            // 這裡需要檢查單位的實際類型
            // 暫時返回所有單位
            Units.Add(UnitPair.Value);
        }
    }
    return Units;
}

TArray<AMingGoRTSUnit*> UMingRTSUnitManager::GetUnitsByState(ERTSUnitState State) const
{
    TArray<AMingGoRTSUnit*> Units;
    for (const auto& UnitPair : AllUnits)
    {
        if (UnitPair.Value && GetUnitState(UnitPair.Value) == State)
        {
            Units.Add(UnitPair.Value);
        }
    }
    return Units;
}

void UMingRTSUnitManager::SelectUnit(AMingGoRTSUnit* Unit, bool bAddToSelection)
{
    if (!Unit)
    {
        return;
    }

    if (!bAddToSelection)
    {
        DeselectAllUnits();
    }

    if (!SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Add(Unit);
        Unit->SetSelected(true);
        
        // 更新單位數據
        for (auto& UnitPair : AllUnits)
        {
            if (UnitPair.Value == Unit)
            {
                FRTSUnitData UnitData = GetUnitData(Unit);
                UnitData.bIsSelected = true;
                SetUnitData(Unit, UnitData);
                break;
            }
        }
        
        NotifyUnitSelected(Unit, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Selected unit. Total selected: %d"), SelectedUnits.Num());
}

void UMingRTSUnitManager::SelectUnits(const TArray<AMingGoRTSUnit*>& Units)
{
    DeselectAllUnits();
    
    for (AMingGoRTSUnit* Unit : Units)
    {
        if (Unit)
        {
            SelectUnit(Unit, true);
        }
    }
}

void UMingRTSUnitManager::SelectUnitsInRect(FVector2D ScreenStart, FVector2D ScreenEnd)
{
    if (!PlayerController)
    {
        return;
    }

    DeselectAllUnits();

    // 獲取所有單位並檢查是否在選擇框內
    TArray<AMingGoRTSUnit*> AllUnitList = GetAllUnits();
    
    for (AMingGoRTSUnit* Unit : AllUnitList)
    {
        if (IsUnitInSelectionRect(Unit, ScreenStart, ScreenEnd))
        {
            SelectUnit(Unit, true);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Selected %d units in rectangle"), SelectedUnits.Num());
}

void UMingRTSUnitManager::DeselectAllUnits()
{
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        if (Unit)
        {
            Unit->SetSelected(false);
            NotifyUnitSelected(Unit, false);
        }
    }
    
    SelectedUnits.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Deselected all units"));
}

void UMingRTSUnitManager::DeselectUnit(AMingGoRTSUnit* Unit)
{
    if (Unit && SelectedUnits.Contains(Unit))
    {
        SelectedUnits.Remove(Unit);
        Unit->SetSelected(false);
        NotifyUnitSelected(Unit, false);
    }
}

bool UMingRTSUnitManager::IsUnitSelected(AMingGoRTSUnit* Unit) const
{
    return SelectedUnits.Contains(Unit);
}

void UMingRTSUnitManager::MoveUnits(const TArray<AMingGoRTSUnit*>& Units, const FVector& TargetLocation)
{
    if (Units.Num() == 0)
    {
        return;
    }

    bool bUseFormation = bAutoFormation && Units.Num() > 1;
    
    if (bUseFormation)
    {
        // 計算陣型並移動單位
        FRTSFormationData Formation = CalculateFormation(CurrentFormation, Units, TargetLocation);
        CurrentFormationData = Formation;
        
        for (int32 i = 0; i < Units.Num(); ++i)
        {
            AMingGoRTSUnit* Unit = Units[i];
            if (Unit)
            {
                FVector FormationPosition = CalculateFormationPosition(Unit, Formation);
                Unit->MoveToLocation(FormationPosition);
                SetUnitState(Unit, ERTSUnitState::Moving);
            }
        }
        
        NotifyUnitsMoved(Units, TargetLocation, true);
    }
    else
    {
        // 直接移動到目標位置
        for (AMingGoRTSUnit* Unit : Units)
        {
            if (Unit)
            {
                Unit->MoveToLocation(TargetLocation);
                SetUnitState(Unit, ERTSUnitState::Moving);
            }
        }
        
        NotifyUnitsMoved(Units, TargetLocation, false);
    }

    UE_LOG(LogTemp, Log, TEXT("Moving %d units to location: %s"), Units.Num(), *TargetLocation.ToString());
}

void UMingRTSUnitManager::MoveSelectedUnits(const FVector& TargetLocation)
{
    MoveUnits(SelectedUnits, TargetLocation);
}

void UMingRTSUnitManager::StopUnitMovement(AMingGoRTSUnit* Unit)
{
    if (Unit)
    {
        SetUnitState(Unit, ERTSUnitState::Idle);
        UE_LOG(LogTemp, Log, TEXT("Stopped unit movement"));
    }
}

void UMingRTSUnitManager::StopAllUnitMovement()
{
    for (AMingGoRTSUnit* Unit : SelectedUnits)
    {
        StopUnitMovement(Unit);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Stopped all unit movement"));
}

bool UMingRTSUnitManager::IsUnitMoving(AMingGoRTSUnit* Unit) const
{
    return GetUnitState(Unit) == ERTSUnitState::Moving;
}

void UMingRTSUnitManager::SetFormation(ERTSFormationType FormationType)
{
    CurrentFormation = FormationType;
    
    if (SelectedUnits.Num() > 1)
    {
        ApplyFormationToSelectedUnits(SelectedUnits[0]->GetActorLocation());
    }
    
    UE_LOG(LogTemp, Log, TEXT("Set formation type: %d"), (int32)FormationType);
}

void UMingRTSUnitManager::ApplyFormationToSelectedUnits(const FVector& Center)
{
    if (SelectedUnits.Num() <= 1)
    {
        return;
    }

    FRTSFormationData Formation = CalculateFormation(CurrentFormation, SelectedUnits, Center);
    CurrentFormationData = Formation;
    
    for (int32 i = 0; i < SelectedUnits.Num(); ++i)
    {
        AMingGoRTSUnit* Unit = SelectedUnits[i];
        if (Unit)
        {
            FVector FormationPosition = CalculateFormationPosition(Unit, Formation);
            Unit->MoveToLocation(FormationPosition);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied formation to %d selected units"), SelectedUnits.Num());
}

FRTSFormationData UMingRTSUnitManager::CalculateFormation(ERTSFormationType FormationType, const TArray<AMingGoRTSUnit*>& Units, const FVector& Center)
{
    FRTSFormationData Formation;
    Formation.FormationType = FormationType;
    Formation.FormationCenter = Center;
    Formation.FormationSpacing = FormationSpacing;
    Formation.UnitPositions.Empty();

    int32 UnitCount = Units.Num();
    if (UnitCount == 0)
    {
        return Formation;
    }

    switch (FormationType)
    {
        case ERTSFormationType::Line:
        {
            // 線形陣型
            float TotalWidth = (UnitCount - 1) * FormationSpacing;
            for (int32 i = 0; i < UnitCount; ++i)
            {
                FVector Position = Center + FVector((i * FormationSpacing) - (TotalWidth / 2.0f), 0.0f, 0.0f);
                Formation.UnitPositions.Add(Position);
            }
            break;
        }
        
        case ERTSFormationType::Column:
        {
            // 縱隊陣型
            float TotalDepth = (UnitCount - 1) * FormationSpacing;
            for (int32 i = 0; i < UnitCount; ++i)
            {
                FVector Position = Center + FVector(0.0f, (i * FormationSpacing) - (TotalDepth / 2.0f), 0.0f);
                Formation.UnitPositions.Add(Position);
            }
            break;
        }
        
        case ERTSFormationType::Wedge:
        {
            // 楔形陣型
            for (int32 i = 0; i < UnitCount; ++i)
            {
                int32 Row = FMath::FloorToInt(FMath::Sqrt(i));
                int32 Col = i - (Row * Row);
                FVector Position = Center + FVector(Row * FormationSpacing, (Col - Row * 0.5f) * FormationSpacing, 0.0f);
                Formation.UnitPositions.Add(Position);
            }
            break;
        }
        
        case ERTSFormationType::Circle:
        {
            // 圓形陣型
            float Radius = FormationSpacing;
            for (int32 i = 0; i < UnitCount; ++i)
            {
                float Angle = (2.0f * PI * i) / UnitCount;
                FVector Position = Center + FVector(FMath::Cos(Angle) * Radius, FMath::Sin(Angle) * Radius, 0.0f);
                Formation.UnitPositions.Add(Position);
            }
            break;
        }
        
        case ERTSFormationType::Square:
        {
            // 方形陣型
            int32 SideLength = FMath::CeilToInt(FMath::Sqrt(UnitCount));
            for (int32 i = 0; i < UnitCount; ++i)
            {
                int32 Row = i / SideLength;
                int32 Col = i % SideLength;
                float OffsetX = (Col - (SideLength - 1) * 0.5f) * FormationSpacing;
                float OffsetY = (Row - (SideLength - 1) * 0.5f) * FormationSpacing;
                FVector Position = Center + FVector(OffsetX, OffsetY, 0.0f);
                Formation.UnitPositions.Add(Position);
            }
            break;
        }
        
        default:
            break;
    }

    return Formation;
}

void UMingRTSUnitManager::UpdateFormation()
{
    if (SelectedUnits.Num() > 1 && CurrentFormation != ERTSFormationType::None)
    {
        ApplyFormationToSelectedUnits(CurrentFormationData.FormationCenter);
    }
}

void UMingRTSUnitManager::SetUnitState(AMingGoRTSUnit* Unit, ERTSUnitState NewState)
{
    if (!Unit)
    {
        return;
    }

    // 更新單位數據中的狀態
    for (auto& UnitPair : AllUnits)
    {
        if (UnitPair.Value == Unit)
        {
            FRTSUnitData UnitData = GetUnitData(Unit);
            ERTSUnitState OldState = UnitData.CurrentState;
            UnitData.CurrentState = NewState;
            SetUnitData(Unit, UnitData);
            
            if (OldState != NewState)
            {
                NotifyUnitStateChanged(Unit, NewState);
            }
            break;
        }
    }
}

ERTSUnitState UMingRTSUnitManager::GetUnitState(AMingGoRTSUnit* Unit) const
{
    if (!Unit)
    {
        return ERTSUnitState::Dead;
    }

    FRTSUnitData UnitData = GetUnitData(Unit);
    return UnitData.CurrentState;
}

void UMingRTSUnitManager::UpdateUnitStates(float DeltaTime)
{
    // 更新所有單位的狀態
    for (const auto& UnitPair : AllUnits)
    {
        AMingGoRTSUnit* Unit = UnitPair.Value;
        if (Unit)
        {
            ERTSUnitState CurrentState = GetUnitState(Unit);
            
            // 檢查移動狀態
            if (CurrentState == ERTSUnitState::Moving)
            {
                FVector CurrentLocation = Unit->GetActorLocation();
                FVector TargetLocation = GetUnitData(Unit).TargetLocation;
                
                if (FVector::Dist(CurrentLocation, TargetLocation) < 50.0f)
                {
                    SetUnitState(Unit, ERTSUnitState::Idle);
                }
            }
        }
    }
}

void UMingRTSUnitManager::AttackUnit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    if (!Attacker || !Target || !CanAttackUnit(Attacker, Target))
    {
        return;
    }

    SetUnitState(Attacker, ERTSUnitState::Attacking);
    
    // 這裡應該實現實際的攻擊邏輯
    UE_LOG(LogTemp, Log, TEXT("Unit attacking target"));
}

void UMingRTSUnitManager::StopAttack(AMingGoRTSUnit* Unit)
{
    if (Unit)
    {
        SetUnitState(Unit, ERTSUnitState::Idle);
    }
}

bool UMingRTSUnitManager::CanAttackUnit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target) const
{
    if (!Attacker || !Target)
    {
        return false;
    }

    float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
    float AttackRange = GetUnitData(Attacker).AttackRange;
    
    return Distance <= AttackRange && IsUnitAlive(Attacker) && IsUnitAlive(Target);
}

TArray<AMingGoRTSUnit*> UMingRTSUnitManager::GetUnitsInRange(AMingGoRTSUnit* Unit, float Range) const
{
    TArray<AMingGoRTSUnit*> UnitsInRange;
    
    if (!Unit)
    {
        return UnitsInRange;
    }

    FVector UnitLocation = Unit->GetActorLocation();
    
    for (const auto& UnitPair : AllUnits)
    {
        AMingGoRTSUnit* OtherUnit = UnitPair.Value;
        if (OtherUnit && OtherUnit != Unit)
        {
            float Distance = FVector::Dist(UnitLocation, OtherUnit->GetActorLocation());
            if (Distance <= Range)
            {
                UnitsInRange.Add(OtherUnit);
            }
        }
    }
    
    return UnitsInRange;
}

FRTSUnitData UMingRTSUnitManager::GetUnitData(AMingGoRTSUnit* Unit) const
{
    FRTSUnitData UnitData;
    
    if (!Unit)
    {
        return UnitData;
    }

    // 從單位獲取實際數據
    UnitData.CurrentLocation = Unit->GetActorLocation();
    UnitData.bIsSelected = IsUnitSelected(Unit);
    
    return UnitData;
}

void UMingRTSUnitManager::SetUnitData(AMingGoRTSUnit* Unit, const FRTSUnitData& UnitData)
{
    // 這裡應該將數據設置回單位
    // 暫時只記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Set unit data for unit"));
}

void UMingRTSUnitManager::UpdateUnitHealth(AMingGoRTSUnit* Unit, float HealthChange)
{
    if (!Unit)
    {
        return;
    }

    FRTSUnitData UnitData = GetUnitData(Unit);
    UnitData.Health = FMath::Clamp(UnitData.Health + HealthChange, 0.0f, UnitData.MaxHealth);
    SetUnitData(Unit, UnitData);
    
    NotifyUnitDamaged(Unit);
    
    if (UnitData.Health <= 0.0f)
    {
        SetUnitState(Unit, ERTSUnitState::Dead);
    }
}

bool UMingRTSUnitManager::IsUnitAlive(AMingGoRTSUnit* Unit) const
{
    if (!Unit)
    {
        return false;
    }

    return GetUnitData(Unit).Health > 0.0f;
}

TArray<FVector> UMingRTSUnitManager::CalculatePath(AMingGoRTSUnit* Unit, const FVector& TargetLocation)
{
    TArray<FVector> Path;
    
    // 簡單的直线路徑
    if (Unit)
    {
        Path.Add(Unit->GetActorLocation());
        Path.Add(TargetLocation);
    }
    
    return Path;
}

bool UMingRTSUnitManager::HasValidPath(AMingGoRTSUnit* Unit) const
{
    // 簡單檢查：單位存在且活著
    return Unit != nullptr && IsUnitAlive(Unit);
}

void UMingRTSUnitManager::RecalculatePaths()
{
    UE_LOG(LogTemp, Log, TEXT("Recalculating paths for all units"));
}

void UMingRTSUnitManager::CreateUnitGroup(const TArray<AMingGoRTSUnit*>& Units, const FString& GroupName)
{
    if (Units.Num() == 0 || GroupName.IsEmpty())
    {
        return;
    }

    UnitGroups.Add(GroupName, Units);
    
    UE_LOG(LogTemp, Log, TEXT("Created unit group: %s with %d units"), *GroupName, Units.Num());
}

void UMingRTSUnitManager::SelectUnitGroup(const FString& GroupName)
{
    if (UnitGroups.Contains(GroupName))
    {
        SelectUnits(UnitGroups[GroupName]);
        UE_LOG(LogTemp, Log, TEXT("Selected unit group: %s"), *GroupName);
    }
}

TArray<FString> UMingRTSUnitManager::GetUnitGroups() const
{
    TArray<FString> GroupNames;
    
    for (const auto& GroupPair : UnitGroups)
    {
        GroupNames.Add(GroupPair.Key);
    }
    
    return GroupNames;
}

int32 UMingRTSUnitManager::GetTotalUnitCount() const
{
    return AllUnits.Num();
}

int32 UMingRTSUnitManager::GetUnitCountByType(ERTSUnitType UnitType) const
{
    return GetUnitsByType(UnitType).Num();
}

int32 UMingRTSUnitManager::GetAliveUnitCount() const
{
    int32 Count = 0;
    
    for (const auto& UnitPair : AllUnits)
    {
        if (UnitPair.Value && IsUnitAlive(UnitPair.Value))
        {
            Count++;
        }
    }
    
    return Count;
}

int32 UMingRTSUnitManager::GetMovingUnitCount() const
{
    return GetUnitsByState(ERTSUnitState::Moving).Num();
}

int32 UMingRTSUnitManager::GetAttackingUnitCount() const
{
    return GetUnitsByState(ERTSUnitState::Attacking).Num();
}

FVector UMingRTSUnitManager::CalculateFormationPosition(AMingGoRTSUnit* Unit, const FRTSFormationData& Formation) const
{
    if (!Unit || Formation.UnitPositions.Num() == 0)
    {
        return Formation.FormationCenter;
    }

    // 簡單的分配：根據單位在選擇列表中的索引分配位置
    int32 UnitIndex = SelectedUnits.IndexOf(Unit);
    if (UnitIndex >= 0 && UnitIndex < Formation.UnitPositions.Num())
    {
        return Formation.UnitPositions[UnitIndex];
    }
    
    return Formation.FormationCenter;
}

bool UMingRTSUnitManager::IsUnitInSelectionRect(AMingGoRTSUnit* Unit, FVector2D ScreenStart, FVector2D ScreenEnd) const
{
    if (!Unit || !PlayerController)
    {
        return false;
    }

    // 獲取單位的螢幕位置
    FVector2D MinScreen(FMath::Min(ScreenStart.X, ScreenEnd.X), FMath::Min(ScreenStart.Y, ScreenEnd.Y));
    FVector2D MaxScreen(FMath::Max(ScreenStart.X, ScreenEnd.X), FMath::Max(ScreenStart.Y, ScreenEnd.Y));

    FVector2D UnitScreenLocation;
    if (PlayerController->ProjectWorldLocationToScreen(Unit->GetActorLocation(), UnitScreenLocation))
    {
        return UnitScreenLocation.X >= MinScreen.X && UnitScreenLocation.X <= MaxScreen.X &&
               UnitScreenLocation.Y >= MinScreen.Y && UnitScreenLocation.Y <= MaxScreen.Y;
    }

    return false;
}

void UMingRTSUnitManager::CleanupDeadUnits()
{
    TArray<FString> UnitsToRemove;
    
    for (const auto& UnitPair : AllUnits)
    {
        if (!IsUnitAlive(UnitPair.Value))
        {
            UnitsToRemove.Add(UnitPair.Key);
        }
    }
    
    for (const FString& UnitID : UnitsToRemove)
    {
        AMingGoRTSUnit* Unit = AllUnits[UnitID];
        UnregisterUnit(Unit);
    }
}

FString UMingRTSUnitManager::GenerateUnitID(ERTSUnitType UnitType) const
{
    static int32 UnitCounter = 0;
    return FString::Printf(TEXT("Unit_%d_%d"), (int32)UnitType, UnitCounter++);
}

void UMingRTSUnitManager::NotifyUnitSelected(AMingGoRTSUnit* Unit, bool bIsSelected)
{
    OnUnitSelected.Broadcast(Unit, bIsSelected);
}

void UMingRTSUnitManager::NotifyUnitStateChanged(AMingGoRTSUnit* Unit, ERTSUnitState NewState)
{
    OnUnitStateChanged.Broadcast(Unit, NewState);
}

void UMingRTSUnitManager::NotifyUnitsMoved(const TArray<AMingGoRTSUnit*>& Units, const FVector& TargetLocation, bool bIsFormation)
{
    OnUnitsMoved.Broadcast(Units, TargetLocation, bIsFormation);
}

void UMingRTSUnitManager::NotifyUnitDamaged(AMingGoRTSUnit* Unit)
{
    OnUnitDamaged.Broadcast(Unit);
}
