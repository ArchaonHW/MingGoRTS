#include "MingRTSUnitManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

UMingRTSUnitManager::UMingRTSUnitManager()
{
    CurrentWorld = nullptr;
    UnitSelector = nullptr;
    Pathfinder = nullptr;
    CurrentFormation = EFormationType::None;
    MaxSelectionCount = 100;
    UnitMovementSpeed = 300.0f;
    FormationSpacing = 100.0f;
    MaxCommandQueueLength = 5;
    bEnableAutoFormation = true;
    bEnablePathOptimization = true;
}

void UMingRTSUnitManager::InitializeUnitManager(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSUnitManager: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    InitializeComponents();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSUnitManager initialized successfully"));
}

void UMingRTSUnitManager::UpdateUnitManager(float DeltaTime)
{
    if (!CurrentWorld)
    {
        return;
    }

    UpdateUnitMovement(DeltaTime);
    ProcessUnitCommands();
    UpdateGroupCenters();
    CleanupInvalidUnits();
}

void UMingRTSUnitManager::HandleUnitSelection(FVector2D ScreenPosition, bool bShiftPressed, bool bCtrlPressed)
{
    if (!UnitSelector)
    {
        return;
    }

    // 確定選擇類型
    ESelectionType SelectionType = ESelectionType::Single;
    if (bShiftPressed)
    {
        SelectionType = ESelectionType::Multiple;
    }
    else if (bCtrlPressed)
    {
        SelectionType = ESelectionType::Group;
    }

    UnitSelector->StartSelection(ScreenPosition, SelectionType);
    UnitSelector->FinishSelection();

    // 觸發選擇事件
    TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
    OnUnitsSelected.Broadcast(SelectedUnits);

    UE_LOG(LogTemp, Log, TEXT("Unit selection handled: %d units selected"), SelectedUnits.Num());
}

void UMingRTSUnitManager::MoveSelectedUnits(FVector TargetPosition, bool bQueueCommand)
{
    if (!UnitSelector || !Pathfinder)
    {
        return;
    }

    TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
    if (SelectedUnits.Num() == 0)
    {
        return;
    }

    // 計算編隊位置
    TArray<FVector> FormationPositions = GetFormationPositions(SelectedUnits, TargetPosition, CurrentFormation);

    // 為每個單位創建移動命令
    for (int32 i = 0; i < SelectedUnits.Num(); i++)
    {
        AActor* Unit = SelectedUnits[i];
        if (!IsValidUnit(Unit))
        {
            continue;
        }

        FUnitCommand Command;
        Command.CommandType = EUnitCommand::Move;
        Command.TargetPosition = FormationPositions[i];
        Command.Formation = CurrentFormation;
        Command.bIsQueued = bQueueCommand;
        Command.CommandTime = CurrentWorld->GetTimeSeconds();

        ExecuteUnitCommand(Command);
    }

    OnUnitsMoved.Broadcast(SelectedUnits);
    OnCommandIssued.Broadcast(EUnitCommand::Move, SelectedUnits, TargetPosition);

    UE_LOG(LogTemp, Log, TEXT("Moving %d units to position %s"), 
        SelectedUnits.Num(), *TargetPosition.ToString());
}

void UMingRTSUnitManager::AttackTarget(AActor* Target, bool bQueueCommand)
{
    if (!UnitSelector || !Target)
    {
        return;
    }

    TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
    if (SelectedUnits.Num() == 0)
    {
        return;
    }

    // 為每個單位創建攻擊命令
    for (AActor* Unit : SelectedUnits)
    {
        if (!IsValidUnit(Unit))
        {
            continue;
        }

        FUnitCommand Command;
        Command.CommandType = EUnitCommand::Attack;
        Command.TargetActor = Target;
        Command.bIsQueued = bQueueCommand;
        Command.CommandTime = CurrentWorld->GetTimeSeconds();

        ExecuteUnitCommand(Command);
    }

    OnCommandIssued.Broadcast(EUnitCommand::Attack, SelectedUnits, Target->GetActorLocation());

    UE_LOG(LogTemp, Log, TEXT("Attacking target with %d units"), SelectedUnits.Num());
}

void UMingRTSUnitManager::SetPatrolPath(const TArray<FVector>& Waypoints, bool bQueueCommand)
{
    if (!UnitSelector || Waypoints.Num() == 0)
    {
        return;
    }

    TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
    if (SelectedUnits.Num() == 0)
    {
        return;
    }

    // 為每個單位創建巡邏命令
    for (AActor* Unit : SelectedUnits)
    {
        if (!IsValidUnit(Unit))
        {
            continue;
        }

        FUnitCommand Command;
        Command.CommandType = EUnitCommand::Patrol;
        Command.Waypoints = Waypoints;
        Command.bIsQueued = bQueueCommand;
        Command.CommandTime = CurrentWorld->GetTimeSeconds();

        ExecuteUnitCommand(Command);
    }

    OnCommandIssued.Broadcast(EUnitCommand::Patrol, SelectedUnits, Waypoints[0]);

    UE_LOG(LogTemp, Log, TEXT("Setting patrol path with %d waypoints for %d units"), 
        Waypoints.Num(), SelectedUnits.Num());
}

void UMingRTSUnitManager::StopUnits(const TArray<AActor*>& Units)
{
    for (AActor* Unit : Units)
    {
        if (!IsValidUnit(Unit))
        {
            continue;
        }

        FUnitCommand Command;
        Command.CommandType = EUnitCommand::Stop;
        Command.CommandTime = CurrentWorld->GetTimeSeconds();

        ExecuteUnitCommand(Command);
    }

    OnCommandIssued.Broadcast(EUnitCommand::Stop, Units, FVector::ZeroVector);

    UE_LOG(LogTemp, Log, TEXT("Stopping %d units"), Units.Num());
}

void UMingRTSUnitManager::SetFormation(EFormationType FormationType)
{
    CurrentFormation = FormationType;
    
    // 如果有選中的單位，重新計算編隊位置
    TArray<AActor*> SelectedUnits = GetSelectedUnits();
    if (SelectedUnits.Num() > 0)
    {
        // 重新排列選中的單位
        FVector Center = FVector::ZeroVector;
        for (AActor* Unit : SelectedUnits)
        {
            Center += Unit->GetActorLocation();
        }
        Center /= SelectedUnits.Num();

        TArray<FVector> FormationPositions = GetFormationPositions(SelectedUnits, Center, FormationType);
        
        // 應用新的編隊位置
        for (int32 i = 0; i < SelectedUnits.Num(); i++)
        {
            AActor* Unit = SelectedUnits[i];
            if (IsValidUnit(Unit))
            {
                FUnitCommand Command;
                Command.CommandType = EUnitCommand::Move;
                Command.TargetPosition = FormationPositions[i];
                Command.Formation = FormationType;
                Command.CommandTime = CurrentWorld->GetTimeSeconds();

                ExecuteUnitCommand(Command);
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Formation set to: %d"), (int32)FormationType);
}

void UMingRTSUnitManager::CreateUnitGroup(const FString& GroupName, const TArray<AActor*>& Units)
{
    if (GroupName.IsEmpty() || Units.Num() == 0)
    {
        return;
    }

    FUnitGroup NewGroup;
    NewGroup.GroupName = GroupName;
    NewGroup.Units = Units;
    NewGroup.DefaultFormation = CurrentFormation;
    
    // 計算分組中心
    FVector Center = FVector::ZeroVector;
    for (AActor* Unit : Units)
    {
        if (IsValidUnit(Unit))
        {
            Center += Unit->GetActorLocation();
        }
    }
    Center /= Units.Num();
    NewGroup.GroupCenter = Center;

    UnitGroups.Add(GroupName, NewGroup);

    UE_LOG(LogTemp, Log, TEXT("Created unit group '%s' with %d units"), 
        *GroupName, Units.Num());
}

void UMingRTSUnitManager::SelectUnitGroup(const FString& GroupName)
{
    FUnitGroup* Group = UnitGroups.Find(GroupName);
    if (!Group || Group->Units.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit group '%s' not found or empty"), *GroupName);
        return;
    }

    if (UnitSelector)
    {
        UnitSelector->ClearSelection();
        
        for (AActor* Unit : Group->Units)
        {
            if (IsValidUnit(Unit))
            {
                UnitSelector->AddUnitToSelection(Unit, false);
            }
        }

        OnUnitsSelected.Broadcast(Group->Units);
    }

    UE_LOG(LogTemp, Log, TEXT("Selected unit group '%s': %d units"), 
        *GroupName, Group->Units.Num());
}

TArray<AActor*> UMingRTSUnitManager::GetSelectedUnits() const
{
    if (UnitSelector)
    {
        return UnitSelector->GetSelectedUnits();
    }
    return TArray<AActor*>();
}

TArray<FString> UMingRTSUnitManager::GetUnitGroups() const
{
    TArray<FString> GroupNames;
    UnitGroups.GetKeys(GroupNames);
    return GroupNames;
}

void UMingRTSUnitManager::SetMaxSelectionCount(int32 MaxCount)
{
    MaxSelectionCount = MaxCount;
    if (UnitSelector)
    {
        UnitSelector->SetMaxSelectionCount(MaxCount);
    }
}

void UMingRTSUnitManager::SetSelectionFilter(TSubclassOf<AActor> FilterClass)
{
    if (UnitSelector)
    {
        UnitSelector->SetSelectionFilter(FilterClass);
    }
}

int32 UMingRTSUnitManager::GetSelectedUnitCount() const
{
    return GetSelectedUnits().Num();
}

int32 UMingRTSUnitManager::GetTotalUnitCount() const
{
    if (!CurrentWorld)
    {
        return 0;
    }

    int32 Count = 0;
    for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
    {
        AActor* Actor = *It;
        if (IsValidUnit(Actor))
        {
            Count++;
        }
    }
    return Count;
}

bool UMingRTSUnitManager::IsUnitSelected(AActor* Unit) const
{
    TArray<AActor*> SelectedUnits = GetSelectedUnits();
    return SelectedUnits.Contains(Unit);
}

TArray<FVector> UMingRTSUnitManager::GetFormationPositions(const TArray<AActor*>& Units, FVector Center, EFormationType Formation) const
{
    TArray<FVector> Positions;
    
    switch (Formation)
    {
        case EFormationType::Line:
            Positions = CalculateLineFormation(Units, Center);
            break;
        case EFormationType::Column:
            Positions = CalculateColumnFormation(Units, Center);
            break;
        case EFormationType::Wedge:
            Positions = CalculateWedgeFormation(Units, Center);
            break;
        case EFormationType::Circle:
            Positions = CalculateCircleFormation(Units, Center);
            break;
        case EFormationType::Square:
            Positions = CalculateSquareFormation(Units, Center);
            break;
        case EFormationType::None:
        default:
            // 無編隊，所有單位移動到同一位置
            for (int32 i = 0; i < Units.Num(); i++)
            {
                Positions.Add(Center);
            }
            break;
    }

    return Positions;
}

void UMingRTSUnitManager::ExecuteUnitCommand(const FUnitCommand& Command)
{
    if (!IsValidCommand(Command))
    {
        return;
    }

    // 這裡需要根據具體的單位類型來執行命令
    // 簡化實現，實際項目中需要更複雜的命令處理系統
    
    switch (Command.CommandType)
    {
        case EUnitCommand::Move:
            // 執行移動命令
            UE_LOG(LogTemp, Log, TEXT("Executing move command to %s"), *Command.TargetPosition.ToString());
            break;
        
        case EUnitCommand::Attack:
            // 執行攻擊命令
            if (Command.TargetActor)
            {
                UE_LOG(LogTemp, Log, TEXT("Executing attack command on %s"), *Command.TargetActor->GetName());
            }
            break;
        
        case EUnitCommand::Patrol:
            // 執行巡邏命令
            UE_LOG(LogTemp, Log, TEXT("Executing patrol command with %d waypoints"), Command.Waypoints.Num());
            break;
        
        case EUnitCommand::Stop:
            // 執行停止命令
            UE_LOG(LogTemp, Log, TEXT("Executing stop command"));
            break;
        
        default:
            break;
    }
}

void UMingRTSUnitManager::ClearAllCommands()
{
    UnitCommands.Empty();
    UE_LOG(LogTemp, Log, TEXT("All unit commands cleared"));
}

void UMingRTSUnitManager::InitializeComponents()
{
    // 初始化單位選擇器
    UnitSelector = NewObject<UMingRTSUnitSelector>(this);
    if (UnitSelector)
    {
        UnitSelector->InitializeSelector(CurrentWorld);
        UnitSelector->SetMaxSelectionCount(MaxSelectionCount);
    }

    // 初始化路徑規劃器
    Pathfinder = NewObject<UMingRTSPathfinder>(this);
    if (Pathfinder)
    {
        Pathfinder->InitializePathfinder(CurrentWorld);
    }
}

void UMingRTSUnitManager::UpdateUnitMovement(float DeltaTime)
{
    // 更新所有單位的移動狀態
    // 這裡需要根據具體的單位移動系統來實現
    // 簡化實現，實際項目中需要更複雜的移動邏輯
}

void UMingRTSUnitManager::ProcessUnitCommands()
{
    // 處理所有單位的命令
    // 這裡需要根據具體的命令系統來實現
    // 簡化實現，實際項目中需要更複雜的命令處理邏輯
}

FVector UMingRTSUnitManager::CalculateFormationPosition(AActor* Unit, const TArray<AActor*>& Units, FVector Center, EFormationType Formation) const
{
    TArray<FVector> FormationPositions = GetFormationPositions(Units, Center, Formation);
    
    int32 UnitIndex = Units.Find(Unit);
    if (UnitIndex >= 0 && UnitIndex < FormationPositions.Num())
    {
        return FormationPositions[UnitIndex];
    }
    
    return Center;
}

bool UMingRTSUnitManager::IsValidCommand(const FUnitCommand& Command) const
{
    // 驗證命令的有效性
    switch (Command.CommandType)
    {
        case EUnitCommand::Move:
            return Command.TargetPosition != FVector::ZeroVector;
        
        case EUnitCommand::Attack:
            return Command.TargetActor != nullptr && IsValidUnit(Command.TargetActor);
        
        case EUnitCommand::Patrol:
            return Command.Waypoints.Num() > 0;
        
        default:
            return true;
    }
}

FUnitCommand UMingRTSUnitManager::GetCurrentCommand(AActor* Unit) const
{
    const FUnitCommand* Command = UnitCommands.Find(Unit);
    if (Command)
    {
        return *Command;
    }
    
    FUnitCommand EmptyCommand;
    EmptyCommand.CommandType = EUnitCommand::Stop;
    return EmptyCommand;
}

void UMingRTSUnitManager::SetUnitCommand(AActor* Unit, const FUnitCommand& Command)
{
    if (IsValidUnit(Unit))
    {
        UnitCommands.Add(Unit, Command);
    }
}

void UMingRTSUnitManager::ClearUnitCommand(AActor* Unit)
{
    UnitCommands.Remove(Unit);
}

TArray<FVector> UMingRTSUnitManager::CalculateLineFormation(const TArray<AActor*>& Units, FVector Center) const
{
    TArray<FVector> Positions;
    float TotalWidth = (Units.Num() - 1) * FormationSpacing;
    float StartX = Center.X - TotalWidth / 2.0f;
    
    for (int32 i = 0; i < Units.Num(); i++)
    {
        FVector Position;
        Position.X = StartX + i * FormationSpacing;
        Position.Y = Center.Y;
        Position.Z = Center.Z;
        Positions.Add(Position);
    }
    
    return Positions;
}

TArray<FVector> UMingRTSUnitManager::CalculateColumnFormation(const TArray<AActor*>& Units, FVector Center) const
{
    TArray<FVector> Positions;
    float TotalHeight = (Units.Num() - 1) * FormationSpacing;
    float StartY = Center.Y - TotalHeight / 2.0f;
    
    for (int32 i = 0; i < Units.Num(); i++)
    {
        FVector Position;
        Position.X = Center.X;
        Position.Y = StartY + i * FormationSpacing;
        Position.Z = Center.Z;
        Positions.Add(Position);
    }
    
    return Positions;
}

TArray<FVector> UMingRTSUnitManager::CalculateWedgeFormation(const TArray<AActor*>& Units, FVector Center) const
{
    TArray<FVector> Positions;
    
    for (int32 i = 0; i < Units.Num(); i++)
    {
        FVector Position = Center;
        
        if (i == 0)
        {
            // 第一個單位在中心
            Position.Z = Center.Z + FormationSpacing;
        }
        else
        {
            // 其他單位在兩側
            int32 Side = (i % 2 == 0) ? 1 : -1;
            int32 Row = (i - 1) / 2 + 1;
            Position.X += Side * Row * FormationSpacing;
            Position.Y -= Row * FormationSpacing;
        }
        
        Positions.Add(Position);
    }
    
    return Positions;
}

TArray<FVector> UMingRTSUnitManager::CalculateCircleFormation(const TArray<AActor*>& Units, FVector Center) const
{
    TArray<FVector> Positions;
    float Radius = FormationSpacing;
    
    for (int32 i = 0; i < Units.Num(); i++)
    {
        float Angle = (float)i / Units.Num() * 2.0f * PI;
        FVector Position;
        Position.X = Center.X + FMath::Cos(Angle) * Radius;
        Position.Y = Center.Y + FMath::Sin(Angle) * Radius;
        Position.Z = Center.Z;
        Positions.Add(Position);
    }
    
    return Positions;
}

TArray<FVector> UMingRTSUnitManager::CalculateSquareFormation(const TArray<AActor*>& Units, FVector Center) const
{
    TArray<FVector> Positions;
    int32 SideLength = FMath::CeilToInt(FMath::Sqrt(Units.Num()));
    float TotalSize = (SideLength - 1) * FormationSpacing;
    float StartX = Center.X - TotalSize / 2.0f;
    float StartY = Center.Y - TotalSize / 2.0f;
    
    for (int32 i = 0; i < Units.Num(); i++)
    {
        int32 Row = i / SideLength;
        int32 Col = i % SideLength;
        
        FVector Position;
        Position.X = StartX + Col * FormationSpacing;
        Position.Y = StartY + Row * FormationSpacing;
        Position.Z = Center.Z;
        Positions.Add(Position);
    }
    
    return Positions;
}

void UMingRTSUnitManager::UpdateGroupCenters()
{
    for (TPair<FString, FUnitGroup>& GroupPair : UnitGroups)
    {
        FUnitGroup& Group = GroupPair.Value;
        
        if (Group.Units.Num() == 0)
        {
            continue;
        }
        
        FVector NewCenter = FVector::ZeroVector;
        int32 ValidUnits = 0;
        
        for (AActor* Unit : Group.Units)
        {
            if (IsValidUnit(Unit))
            {
                NewCenter += Unit->GetActorLocation();
                ValidUnits++;
            }
        }
        
        if (ValidUnits > 0)
        {
            Group.GroupCenter = NewCenter / ValidUnits;
        }
    }
}

bool UMingRTSUnitManager::IsValidUnit(AActor* Unit) const
{
    if (!Unit || Unit->IsPendingKill())
    {
        return false;
    }
    
    // 這裡可以添加更多的單位有效性檢查
    // 例如檢查單位是否具有必要的組件、是否在有效的狀態等
    
    return true;
}

void UMingRTSUnitManager::CleanupInvalidUnits()
{
    // 清理選擇器中的無效單位
    if (UnitSelector)
    {
        TArray<AActor*> SelectedUnits = UnitSelector->GetSelectedUnits();
        for (int32 i = SelectedUnits.Num() - 1; i >= 0; --i)
        {
            if (!IsValidUnit(SelectedUnits[i]))
            {
                UnitSelector->RemoveUnitFromSelection(SelectedUnits[i]);
            }
        }
    }
    
    // 清理分組中的無效單位
    for (TPair<FString, FUnitGroup>& GroupPair : UnitGroups)
    {
        FUnitGroup& Group = GroupPair.Value;
        for (int32 i = Group.Units.Num() - 1; i >= 0; --i)
        {
            if (!IsValidUnit(Group.Units[i]))
            {
                Group.Units.RemoveAt(i);
            }
        }
    }
    
    // 清理命令中的無效單位
    for (auto It = UnitCommands.CreateIterator(); It; ++It)
    {
        if (!IsValidUnit(It->Key))
        {
            It.RemoveCurrent();
        }
    }
}

TSubclassOf<AActor> UMingRTSUnitManager::GetUnitType(AActor* Unit) const
{
    if (!Unit)
    {
        return nullptr;
    }
    
    return Unit->GetClass();
}

void UMingRTSUnitManager::SetUnitHighlight(AActor* Unit, bool bHighlighted)
{
    // 這裡可以添加單位高亮效果
    // 例如通過材質參數、後處理效果等方式實現
}

void UMingRTSUnitManager::UpdateSelectionVisuals()
{
    TArray<AActor*> SelectedUnits = GetSelectedUnits();
    
    // 清除所有單位的高亮
    if (CurrentWorld)
    {
        for (TActorIterator<AActor> It(CurrentWorld); It; ++It)
        {
            AActor* Actor = *It;
            if (IsValidUnit(Actor))
            {
                SetUnitHighlight(Actor, false);
            }
        }
    }
    
    // 為選中的單位添加高亮
    for (AActor* Unit : SelectedUnits)
    {
        SetUnitHighlight(Unit, true);
    }
}
