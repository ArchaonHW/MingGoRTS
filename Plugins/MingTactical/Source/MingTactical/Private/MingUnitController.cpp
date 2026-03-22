#include "MingUnitController.h"
#include "MingTacticalUnit.h"
#include "MingSelectionManager.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"

AMingUnitController::AMingUnitController()
    : bIsInitialized(false)
    , SelectionManager(nullptr)
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 預分配容量
    ControlledUnits.Reserve(100);
    UnitStates.Reserve(100);
    UnitCommandQueues.Reserve(100);
    CurrentCommands.Reserve(100);
    ControlStatistics.Reserve(50);
    PerformanceMetrics.Reserve(20);
}

void AMingUnitController::InitializeUnitController()
{
    if (bIsInitialized)
    {
        return;
    }

    // 獲取選擇管理器
    UWorld* World = GetWorld();
    if (World)
    {
        // 簡化實作：假設選擇管理器已存在
        // SelectionManager = World->GetSubsystem<UMingSelectionManager>();
    }

    // 初始化統計數據
    ControlStatistics.Add(TEXT("total_commands"), 0);
    ControlStatistics.Add(TEXT("successful_commands"), 0);
    ControlStatistics.Add(TEXT("failed_commands"), 0);
    ControlStatistics.Add(TEXT("cancelled_commands"), 0);
    ControlStatistics.Add(TEXT("units_controlled"), 0);
    
    // 初始化性能指標
    PerformanceMetrics.Add(TEXT("command_execution_time"), 0.0f);
    PerformanceMetrics.Add(TEXT("units_per_second"), 0.0f);
    PerformanceMetrics.Add(TEXT("command_success_rate"), 0.0f);
    PerformanceMetrics.Add(TEXT("average_queue_size"), 0.0f);
    
    bIsInitialized = true;
    
    UE_LOG(LogTemp, Log, TEXT("MingUnitController initialized"));
}

void AMingUnitController::ShutdownUnitController()
{
    if (!bIsInitialized)
    {
        return;
    }

    // 清理所有命令
    CancelAllCommands();
    
    // 清理單位
    ControlledUnits.Empty();
    UnitStates.Empty();
    UnitCommandQueues.Empty();
    CurrentCommands.Empty();
    
    bIsInitialized = false;
    
    UE_LOG(LogTemp, Log, TEXT("MingUnitController shutdown"));
}

bool AMingUnitController::AddControlledUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        UE_LOG(LogTemp, Error, TEXT("UnitController not initialized or invalid unit"));
        return false;
    }

    if (ControlledUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit already controlled"));
        return false;
    }

    if (!ValidateUnit(Unit))
    {
        UE_LOG(LogTemp, Error, TEXT("Unit validation failed"));
        return false;
    }

    // 添加到受控列表
    ControlledUnits.Add(Unit);
    UnitStates.Add(Unit, EUnitState::Idle);
    UnitCommandQueues.Add(Unit, TArray<FUnitCommand>());
    
    // 更新統計
    int32* Count = ControlStatistics.Find(TEXT("units_controlled"));
    if (Count)
    {
        (*Count)++;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit added to control: %s"), *Unit->GetName());
    
    return true;
}

bool AMingUnitController::RemoveControlledUnit(AMingTacticalUnit* Unit)
{
    if (!bIsInitialized || !Unit)
    {
        return false;
    }

    if (!ControlledUnits.Contains(Unit))
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit not controlled"));
        return false;
    }

    // 取消所有命令
    TArray<FUnitCommand> Commands = UnitCommandQueues.FindRef(Unit);
    for (const FUnitCommand& Command : Commands)
    {
        CancelCommand(Command.CommandID);
    }

    // 從各個映射中移除
    ControlledUnits.Remove(Unit);
    UnitStates.Remove(Unit);
    UnitCommandQueues.Remove(Unit);
    CurrentCommands.Remove(Unit);
    
    // 更新統計
    int32* Count = ControlStatistics.Find(TEXT("units_controlled"));
    if (Count && *Count > 0)
    {
        (*Count)--;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Unit removed from control: %s"), *Unit->GetName());
    
    return true;
}

TArray<AMingTacticalUnit*> AMingUnitController::GetControlledUnits() const
{
    return ControlledUnits;
}

TArray<AMingTacticalUnit*> AMingUnitController::GetUnitsByType(const FString& UnitType) const
{
    TArray<AMingTacticalUnit*> Result;
    
    for (AMingTacticalUnit* Unit : ControlledUnits)
    {
        if (Unit && Unit->GetUnitType() == UnitType)
        {
            Result.Add(Unit);
        }
    }
    
    return Result;
}

TArray<AMingTacticalUnit*> AMingUnitController::GetActiveUnits() const
{
    TArray<AMingTacticalUnit*> Result;
    
    for (AMingTacticalUnit* Unit : ControlledUnits)
    {
        if (Unit && IsUnitAlive(Unit))
        {
            Result.Add(Unit);
        }
    }
    
    return Result;
}

FUnitControlResult AMingUnitController::IssueUnitCommand(const FUnitCommand& Command)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (!bIsInitialized)
    {
        Result.ErrorMessage = TEXT("UnitController not initialized");
        return Result;
    }

    if (!ValidateCommand(Command))
    {
        Result.ErrorMessage = TEXT("Invalid command");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Issuing command: %s"), *GetCommandDescription(Command));

    // 根據命令類型處理
    switch (Command.CommandType)
    {
    case EUnitCommandType::Move:
        Result = MoveUnitsToLocation(ControlledUnits, Command.TargetLocation, Command.MovementType);
        break;
    case EUnitCommandType::Attack:
        if (Command.TargetUnit)
        {
            Result = AttackTarget(ControlledUnits, Command.TargetUnit, Command.AttackType);
        }
        else if (Command.TargetBuilding)
        {
            Result = AttackBuilding(ControlledUnits, Command.TargetBuilding, Command.AttackType);
        }
        break;
    case EUnitCommandType::Stop:
        Result = StopUnits(ControlledUnits);
        break;
    case EUnitCommandType::Hold:
        Result = HoldPosition(ControlledUnits, Command.TargetLocation);
        break;
    case EUnitCommandType::Patrol:
        // 需要從參數中獲取巡邏點
        break;
    case EUnitCommandType::Guard:
        if (Command.TargetUnit)
        {
            Result = GuardTarget(ControlledUnits, Command.TargetUnit);
        }
        break;
    case EUnitCommandType::Retreat:
        Result = RetreatUnits(ControlledUnits, Command.TargetLocation);
        break;
    case EUnitCommandType::FormUp:
        Result = FormUpUnits(ControlledUnits, Command.FormationType, Command.TargetLocation);
        break;
    default:
        Result.ErrorMessage = FString::Printf(TEXT("Unsupported command type: %d"), static_cast<int32>(Command.CommandType));
        break;
    }

    // 更新統計
    int32* TotalCommands = ControlStatistics.Find(TEXT("total_commands"));
    if (TotalCommands)
    {
        (*TotalCommands)++;
    }
    
    if (Result.bSuccess)
    {
        int32* SuccessfulCommands = ControlStatistics.Find(TEXT("successful_commands"));
        if (SuccessfulCommands)
        {
            (*SuccessfulCommands)++;
        }
    }
    else
    {
        int32* FailedCommands = ControlStatistics.Find(TEXT("failed_commands"));
        if (FailedCommands)
        {
            (*FailedCommands)++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Command issued: %s - %s"), *GetCommandDescription(Command), Result.bSuccess ? TEXT("Success") : TEXT("Failed"));
    
    return Result;
}

TArray<FUnitControlResult> AMingUnitController::BatchIssueCommands(const TArray<FUnitCommand>& Commands)
{
    TArray<FUnitControlResult> Results;
    
    for (const FUnitCommand& Command : Commands)
    {
        Results.Add(IssueUnitCommand(Command));
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch issued %d commands"), Results.Num());
    
    return Results;
}

bool AMingUnitController::CancelCommand(const FString& CommandID)
{
    bool bCancelled = false;
    
    // 遍歸所有單位的命令隊列
    for (auto& QueuePair : UnitCommandQueues)
    {
        TArray<FUnitCommand>& CommandQueue = QueuePair.Value;
        
        for (int32 i = 0; i < CommandQueue.Num(); ++i)
        {
            if (CommandQueue[i].CommandID == CommandID)
            {
                CommandQueue[i].bIsCancelled = true;
                bCancelled = true;
                
                UE_LOG(LogTemp, Log, TEXT("Command cancelled: %s"), *CommandID);
                
                // 如果是當前執行的命令，也需要處理
                if (CurrentCommands.Contains(QueuePair.Key) && CurrentCommands[QueuePair.Key].CommandID == CommandID)
                {
                    CurrentCommands.Remove(QueuePair.Key);
                }
                
                break;
            }
        }
        
        if (bCancelled)
        {
            break;
        }
    }
    
    if (bCancelled)
    {
        int32* Count = ControlStatistics.Find(TEXT("cancelled_commands"));
        if (Count)
        {
            (*Count)++;
        }
    }
    
    return bCancelled;
}

bool AMingUnitController::CancelAllCommands()
{
    int32 CancelledCount = 0;
    
    for (auto& QueuePair : UnitCommandQueues)
    {
        TArray<FUnitCommand>& CommandQueue = QueuePair.Value;
        
        for (FUnitCommand& Command : CommandQueue)
        {
            if (!Command.bIsCancelled && !Command.bIsCompleted)
            {
                Command.bIsCancelled = true;
                CancelledCount++;
            }
        }
    }
    
    CurrentCommands.Empty();
    
    // 更新統計
    int32* Count = ControlStatistics.Find(TEXT("cancelled_commands"));
    if (Count)
    {
        (*Count) += CancelledCount;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Cancelled %d commands"), CancelledCount);
    
    return CancelledCount > 0;
}

FUnitControlResult AMingUnitController::MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0)
    {
        Result.ErrorMessage = TEXT("No units provided");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Moving %d units to location: %s"), Units.Num(), *TargetLocation.ToString());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建移動命令
        FUnitCommand MoveCommand;
        MoveCommand.CommandID = FString::Printf(TEXT("move_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        MoveCommand.CommandType = EUnitCommandType::Move;
        MoveCommand.Priority = EUnitCommandPriority::Medium;
        MoveCommand.TargetLocation = TargetLocation;
        MoveCommand.MovementType = MovementType;
        MoveCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(MoveCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Moving);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Moved %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0 || !TargetUnit)
    {
        Result.ErrorMessage = TEXT("No units or invalid target");
        return Result;
    }

    if (!ValidateUnit(TargetUnit))
    {
        Result.ErrorMessage = TEXT("Invalid target unit");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Attacking target: %s with %d units"), *TargetUnit->GetName(), Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 檢查攻擊範圍
        if (!IsInAttackRange(Unit, TargetUnit))
        {
            // 需要先移動到攻擊範圍
            FVector MoveLocation = TargetUnit->GetActorLocation();
            MoveUnitsToLocation({Unit}, MoveLocation, EUnitMovementType::Run);
        }

        // 創建攻擊命令
        FUnitCommand AttackCommand;
        AttackCommand.CommandID = FString::Printf(TEXT("attack_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        AttackCommand.CommandType = EUnitCommandType::Attack;
        AttackCommand.Priority = EUnitCommandPriority::High;
        AttackCommand.TargetUnit = TargetUnit;
        AttackCommand.AttackType = AttackType;
        AttackCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(AttackCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Attacking);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Attacked with %d units"), Result.AffectedUnits.Num());
    Result.TargetUnit = TargetUnit;
    
    return Result;
}

FUnitControlResult AMingUnitController::AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0 || !TargetBuilding)
    {
        Result.ErrorMessage = TEXT("No units or invalid target building");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Attacking building: %s with %d units"), *TargetBuilding->GetName(), Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建攻擊命令
        FUnitCommand AttackCommand;
        AttackCommand.CommandID = FString::Printf(TEXT("attack_building_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        AttackCommand.CommandType = EUnitCommandType::Attack;
        AttackCommand.Priority = EUnitCommandPriority::High;
        AttackCommand.TargetBuilding = TargetBuilding;
        AttackCommand.AttackType = AttackType;
        AttackCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(AttackCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Attacking);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Attacked building with %d units"), Result.AffectedUnits.Num());
    Result.TargetBuilding = TargetBuilding;
    
    return Result;
}

FUnitControlResult AMingUnitController::StopUnits(const TArray<AMingTacticalUnit*>& Units)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0)
    {
        Result.ErrorMessage = TEXT("No units provided");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Stopping %d units"), Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit))
        {
            continue;
        }

        // 取消所有命令
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        for (FUnitCommand& Command : CommandQueue)
        {
            Command.bIsCancelled = true;
        }
        
        // 清空命令隊列
        CommandQueue.Empty();
        
        // 移除當前命令
        CurrentCommands.Remove(Unit);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Idle);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Stopped %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0)
    {
        Result.ErrorMessage = TEXT("No units provided");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Holding position for %d units"), Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建防禦命令
        FUnitCommand HoldCommand;
        HoldCommand.CommandID = FString::Printf(TEXT("hold_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        HoldCommand.CommandType = EUnitCommandType::Hold;
        HoldCommand.Priority = EUnitCommandPriority::Medium;
        HoldCommand.TargetLocation = HoldLocation;
        HoldCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(HoldCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Defending);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Held position for %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0 || PatrolPoints.Num() < 2)
    {
        Result.ErrorMessage = TEXT("Invalid units or patrol points");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Setting patrol path for %d units with %d points"), Units.Num(), PatrolPoints.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建巡邏命令
        FUnitCommand PatrolCommand;
        PatrolCommand.CommandID = FString::Printf(TEXT("patrol_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        PatrolCommand.CommandType = EUnitCommandType::Patrol;
        PatrolCommand.Priority = EUnitCommandPriority::Low;
        PatrolCommand.CommandTime = FPlatformTime::Seconds();
        
        // 將巡邏點存儲在參數中
        for (int32 i = 0; i < PatrolPoints.Num(); ++i)
        {
            PatrolCommand.CommandParameters.Add(FString::Printf(TEXT("patrol_point_%d"), i), PatrolPoints[i].ToString());
        }
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(PatrolCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Patrolling);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Set patrol for %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0 || !TargetToGuard)
    {
        Result.ErrorMessage = TEXT("No units or invalid target");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Guarding target: %s with %d units"), *TargetToGuard->GetName(), Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建守護命令
        FUnitCommand GuardCommand;
        GuardCommand.CommandID = FString::Printf(TEXT("guard_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        GuardCommand.CommandType = EUnitCommandType::Guard;
        GuardCommand.Priority = EUnitCommandPriority::Medium;
        GuardCommand.TargetUnit = TargetToGuard;
        GuardCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(GuardCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Guarding);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Guarding with %d units"), Result.AffectedUnits.Num());
    Result.TargetUnit = TargetToGuard;
    
    return Result;
}

FUnitControlResult AMingUnitController::RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0)
    {
        Result.ErrorMessage = TEXT("No units provided");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Retreating %d units to location: %s"), Units.Num(), *RetreatLocation.ToString());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建撤退命令
        FUnitCommand RetreatCommand;
        RetreatCommand.CommandID = FString::Printf(TEXT("retreat_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        RetreatCommand.CommandType = EUnitCommandType::Retreat;
        RetreatCommand.Priority = EUnitCommandPriority::Critical;
        RetreatCommand.TargetLocation = RetreatLocation;
        RetreatCommand.MovementType = EUnitMovementType::Sprint;
        RetreatCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(RetreatCommand);
        
        // 設置單位狀態
        SetUnitState(Unit, EUnitState::Retreating);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Retreated %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0)
    {
        Result.ErrorMessage = TEXT("No units provided");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Forming up %d units in %s formation"), Units.Num(), *UEnum::GetValueAsString(FormationType));

    // 計算編隊位置
    TArray<FVector> FormationPositions = CalculateFormationPositions(Units, FormationType, FormationCenter);
    
    if (FormationPositions.Num() != Units.Num())
    {
        Result.ErrorMessage = TEXT("Formation positions calculation failed");
        return Result;
    }

    for (int32 i = 0; i < Units.Num(); ++i)
    {
        AMingTacticalUnit* Unit = Units[i];
        
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建編隊命令
        FUnitCommand FormUpCommand;
        FormUpCommand.CommandID = FString::Printf(TEXT("formup_%s_%d"), *Unit->GetName(), FDateTime::Now().GetMillisecond());
        FormUpCommand.CommandType = EUnitCommandType::FormUp;
        FormUpCommand.Priority = EUnitCommandPriority::Medium;
        FormUpCommand.TargetLocation = FormationPositions[i];
        FormUpCommand.FormationType = FormationType;
        FormUpCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(FormUpCommand);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Formed up %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

FUnitControlResult AMingUnitController::UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit)
{
    FUnitControlResult Result;
    Result.ExecuteTime = FPlatformTime::Seconds();
    
    if (Units.Num() == 0 || AbilityID.IsEmpty())
    {
        Result.ErrorMessage = TEXT("No units or invalid ability ID");
        return Result;
    }

    UE_LOG(LogTemp, Log, TEXT("Using ability %s for %d units"), *AbilityID, Units.Num());

    for (AMingTacticalUnit* Unit : Units)
    {
        if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
        {
            continue;
        }

        // 創建技能命令
        FUnitCommand AbilityCommand;
        AbilityCommand.CommandID = FString::Printf(TEXT("ability_%s_%s_%d"), *Unit->GetName(), *AbilityID, FDateTime::Now().GetMillisecond());
        AbilityCommand.CommandType = EUnitCommandType::UseAbility;
        AbilityCommand.Priority = EUnitCommandPriority::High;
        AbilityCommand.TargetUnit = TargetUnit;
        AbilityCommand.CommandParameters.Add(TEXT("ability_id"), AbilityID);
        AbilityCommand.CommandTime = FPlatformTime::Seconds();
        
        // 添加到命令隊列
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Add(AbilityCommand);
        
        Result.AffectedUnits.Add(Unit);
    }

    Result.bSuccess = Result.AffectedUnits.Num() > 0;
    Result.ResultDescription = FString::Printf(TEXT("Used ability for %d units"), Result.AffectedUnits.Num());
    
    return Result;
}

EUnitState AMingUnitController::GetUnitState(AMingTacticalUnit* Unit) const
{
    const EUnitState* State = UnitStates.Find(Unit);
    return State ? *State : EUnitState::Idle;
}

bool AMingUnitController::SetUnitState(AMingTacticalUnit* Unit, EUnitState NewState)
{
    if (!ValidateUnit(Unit))
    {
        return false;
    }

    EUnitState OldState = GetUnitState(Unit);
    UnitStates.Add(Unit, NewState);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %s state changed: %s -> %s"), 
        *Unit->GetName(), *GetStateDescription(OldState), *GetStateDescription(NewState));
    
    return true;
}

TArray<FUnitCommand> AMingUnitController::GetUnitCommandQueue(AMingTacticalUnit* Unit) const
{
    const TArray<FUnitCommand>* Queue = UnitCommandQueues.Find(Unit);
    return Queue ? *Queue : TArray<FUnitCommand>();
}

FUnitCommand AMingUnitController::GetCurrentCommand(AMingTacticalUnit* Unit) const
{
    const FUnitCommand* Command = CurrentCommands.Find(Unit);
    return Command ? *Command : FUnitCommand();
}

bool AMingUnitController::CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command) const
{
    if (!ValidateUnit(Unit) || !IsUnitAlive(Unit))
    {
        return false;
    }

    EUnitState CurrentState = GetUnitState(Unit);
    
    // 檢查狀態兼容性
    switch (Command.CommandType)
    {
    case EUnitCommandType::Move:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Moving;
    case EUnitCommandType::Attack:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Attacking || CurrentState == EUnitState::Moving;
    case EUnitCommandType::Stop:
        return true; // 可以隨時停止
    case EUnitCommandType::Hold:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Defending;
    case EUnitCommandType::Patrol:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Patrolling;
    case EUnitCommandType::Guard:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Guarding;
    case EUnitCommandType::Retreat:
        return true; // 可以隨時撤退
    case EUnitCommandType::UseAbility:
        return CurrentState == EUnitState::Idle || CurrentState == EUnitState::Attacking;
    default:
        return false;
    }
}

TMap<FString, int32> AMingUnitController::GetUnitControlStatistics() const
{
    return ControlStatistics;
}

TMap<FString, float> AMingUnitController::GetPerformanceMetrics() const
{
    return PerformanceMetrics;
}

void AMingUnitController::Tick(float DeltaTime)
{
    SuperTick(DeltaTime);

    if (!bIsInitialized)
    {
        return;
    }

    // 更新所有單位狀態
    for (AMingTacticalUnit* Unit : ControlledUnits)
    {
        if (ValidateUnit(Unit))
        {
            UpdateUnitState(Unit);
        }
    }

    // 更新性能指標
    UpdatePerformanceMetrics();
}

// === 內部方法實現 ===

void AMingUnitController::ExecuteUnitCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    if (!ValidateUnit(Unit))
    {
        return;
    }

    UE_LOG(LogTemp, Verbose, TEXT("Executing command for unit %s: %s"), *Unit->GetName(), *GetCommandDescription(Command));

    // 根據命令類型執行
    switch (Command.CommandType)
    {
    case EUnitCommandType::Move:
        HandleMoveCommand(Unit, Command);
        break;
    case EUnitCommandType::Attack:
        HandleAttackCommand(Unit, Command);
        break;
    case EUnitCommandType::Stop:
        HandleStopCommand(Unit, Command);
        break;
    case EUnitCommandType::Hold:
        HandleHoldCommand(Unit, Command);
        break;
    case EUnitCommandType::Patrol:
        HandlePatrolCommand(Unit, Command);
        break;
    case EUnitCommandType::Guard:
        HandleGuardCommand(Unit, Command);
        break;
    case EUnitCommandType::Retreat:
        HandleRetreatCommand(Unit, Command);
        break;
    case EUnitCommandType::Build:
        HandleBuildCommand(Unit, Command);
        break;
    case EUnitCommandType::Repair:
        HandleRepairCommand(Unit, Command);
        break;
    case EUnitCommandType::Heal:
        HandleHealCommand(Unit, Command);
        break;
    case EUnitCommandType::Capture:
        HandleCaptureCommand(Unit, Command);
        break;
    case EUnitCommandType::UseAbility:
        HandleAbilityCommand(Unit, Command);
        break;
    case EUnitCommandType::FormUp:
        HandleFormUpCommand(Unit, Command);
        break;
    default:
        UE_LOG(LogTemp, Warning, TEXT("Unknown command type: %d"), static_cast<int32>(Command.CommandType));
        break;
    }
}

void AMingUnitController::HandleMoveCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 計算路徑
    TArray<FVector> Path = CalculatePath(Unit, Command.TargetLocation);
    
    if (Path.Num() > 0)
    {
        // 設置單位目標位置
        SetUnitLocation(Unit, Path[0]);
        
        // 簡化實作：直接移動到目標
        SetUnitLocation(Unit, Command.TargetLocation);
        
        // 檢查是否到達目標
        if (FVector::Dist(GetUnitLocation(Unit), Command.TargetLocation) < 50.0f)
        {
            OnCommandCompleted(Unit, Command);
        }
    }
    else
    {
        OnCommandFailed(Unit, Command, TEXT("Path calculation failed"));
    }
}

void AMingUnitController::HandleAttackCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    if (Command.TargetUnit && IsUnitAlive(Command.TargetUnit))
    {
        // 檢查攻擊範圍
        if (IsInAttackRange(Unit, Command.TargetUnit))
        {
            // 計算傷害
            float Damage = CalculateAttackDamage(Unit, Command.TargetUnit, Command.AttackType);
            
            // 應用傷害
            ApplyDamage(Command.TargetUnit, Damage, Unit);
            
            // 檢查目標是否死亡
            if (!IsUnitAlive(Command.TargetUnit))
            {
                OnCommandCompleted(Unit, Command);
            }
        }
        else
        {
            // 需要移動到攻擊範圍
            FVector MoveLocation = Command.TargetUnit->GetActorLocation();
            SetUnitLocation(Unit, MoveLocation);
        }
    }
    else
    {
        OnCommandCompleted(Unit, Command);
    }
}

void AMingUnitController::HandleStopCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 停止所有動作
    SetUnitState(Unit, EUnitState::Idle);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleHoldCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 防禦位置
    SetUnitState(Unit, EUnitState::Defending);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandlePatrolCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 簡化實作：巡邏第一個點
    FString* FirstPoint = Command.CommandParameters.Find(TEXT("patrol_point_0"));
    if (FirstPoint)
    {
        FVector PatrolPoint = FVector(*FirstPoint);
        SetUnitLocation(Unit, PatrolPoint);
    }
    
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleGuardCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    if (Command.TargetUnit && IsUnitAlive(Command.TargetUnit))
    {
        // 守護目標
        FVector GuardPosition = Command.TargetUnit->GetActorLocation();
        SetUnitLocation(Unit, GuardPosition);
    }
    
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleRetreatCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 撤退到目標位置
    SetUnitLocation(Unit, Command.TargetLocation);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleBuildCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 建設邏輯
    SetUnitState(Unit, EUnitState::Building);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleRepairCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 修理邏輯
    SetUnitState(Unit, EUnitState::Repairing);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleHealCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 治療邏輯
    SetUnitState(Unit, EUnitState::Healing);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleCaptureCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 捕獲邏輯
    SetUnitState(Unit, EUnitState::Captured);
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleAbilityCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 技能邏輯
    FString* AbilityID = Command.CommandParameters.Find(TEXT("ability_id"));
    if (AbilityID)
    {
        UE_LOG(LogTemp, Log, TEXT("Unit %s used ability: %s"), *Unit->GetName(), **AbilityID);
    }
    
    OnCommandCompleted(Unit, Command);
}

void AMingUnitController::HandleFormUpCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    // 編隊邏輯
    SetUnitLocation(Unit, Command.TargetLocation);
    OnCommandCompleted(Unit, Command);
}

bool AMingUnitController::IsCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command) const
{
    switch (Command.CommandType)
    {
    case EUnitCommandType::Move:
        return FVector::Dist(GetUnitLocation(Unit), Command.TargetLocation) < 50.0f;
    case EUnitCommandType::Attack:
        return !Command.TargetUnit || !IsUnitAlive(Command.TargetUnit);
    case EUnitCommandType::Stop:
    case EUnitCommandType::Hold:
    case EUnitCommandType::Patrol:
    case EUnitCommandType::Guard:
    case EUnitCommandType::Retreat:
    case EUnitCommandType::Build:
    case EUnitCommandType::Repair:
    case EUnitCommandType::Heal:
    case EUnitCommandType::Capture:
    case EUnitCommandType::UseAbility:
    case EUnitCommandType::FormUp:
        return true; // 這些命令立即完成
    default:
        return false;
    }
}

bool AMingUnitController::IsCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command) const
{
    if (!IsUnitAlive(Unit))
    {
        return true;
    }
    
    // 檢查重試次數
    if (Command.RetryCount >= Command.MaxRetryCount)
    {
        return true;
    }
    
    return false;
}

void AMingUnitController::OnCommandCompleted(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    UE_LOG(LogTemp, Log, TEXT("Command completed: %s for unit %s"), *GetCommandDescription(Command), *Unit->GetName());
    
    // 從當前命令中移除
    CurrentCommands.Remove(Unit);
    
    // 從命令隊列中移除
    TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
    for (int32 i = 0; i < CommandQueue.Num(); ++i)
    {
        if (CommandQueue[i].CommandID == Command.CommandID)
        {
            CommandQueue[i].bIsCompleted = true;
            break;
        }
    }
    
    // 設置為空閒狀態
    SetUnitState(Unit, EUnitState::Idle);
}

void AMingUnitController::OnCommandFailed(AMingTacticalUnit* Unit, const FUnitCommand& Command, const FString& ErrorReason)
{
    UE_LOG(LogTemp, Warning, TEXT("Command failed: %s for unit %s - %s"), *GetCommandDescription(Command), *Unit->GetName(), *ErrorReason);
    
    // 增加重試次數
    FUnitCommand ModifiedCommand = Command;
    ModifiedCommand.RetryCount++;
    
    // 如果還可以重試，重新加入隊列
    if (ModifiedCommand.RetryCount < ModifiedCommand.MaxRetryCount)
    {
        TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
        CommandQueue.Insert(ModifiedCommand, 0); // 插入到隊列前面
    }
    else
    {
        // 從當前命令中移除
        CurrentCommands.Remove(Unit);
        
        // 設置為空閒狀態
        SetUnitState(Unit, EUnitState::Idle);
    }
}

void AMingUnitController::UpdateUnitState(AMingTacticalUnit* Unit)
{
    if (!ValidateUnit(Unit))
    {
        return;
    }

    // 檢查是否有當前命令
    if (!CurrentCommands.Contains(Unit))
    {
        // 獲取下一個命令
        FUnitCommand NextCommand = GetNextCommand(Unit);
        if (NextCommand.CommandID.IsEmpty())
        {
            // 沒有命令，設置為空閒
            SetUnitState(Unit, EUnitState::Idle);
        }
        else
        {
            // 執行下一個命令
            CurrentCommands.Add(Unit, NextCommand);
            ExecuteUnitCommand(Unit, NextCommand);
        }
    }
    else
    {
        // 檢查當前命令狀態
        const FUnitCommand& CurrentCommand = CurrentCommands[Unit];
        
        if (IsCommandCompleted(Unit, CurrentCommand))
        {
            OnCommandCompleted(Unit, CurrentCommand);
        }
        else if (IsCommandFailed(Unit, CurrentCommand))
        {
            OnCommandFailed(Unit, CurrentCommand, TEXT("Command execution failed"));
        }
        else
        {
            // 繼續執行當前命令
            ExecuteUnitCommand(Unit, CurrentCommand);
        }
    }
}

FUnitCommand AMingUnitController::GetNextCommand(AMingTacticalUnit* Unit)
{
    TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
    
    // 清理已完成或取消的命令
    CleanupCompletedCommands(Unit);
    
    if (CommandQueue.Num() > 0)
    {
        FUnitCommand NextCommand = CommandQueue[0];
        CommandQueue.RemoveAt(0);
        return NextCommand;
    }
    
    return FUnitCommand();
}

void AMingUnitController::CleanupCompletedCommands(AMingTacticalUnit* Unit)
{
    TArray<FUnitCommand>& CommandQueue = UnitCommandQueues.FindRef(Unit);
    
    for (int32 i = CommandQueue.Num() - 1; i >= 0; --i)
    {
        if (CommandQueue[i].bIsCompleted || CommandQueue[i].bIsCancelled)
        {
            CommandQueue.RemoveAt(i);
        }
    }
}

TArray<FVector> AMingUnitController::CalculatePath(AMingTacticalUnit* Unit, const FVector& TargetLocation)
{
    // 簡化實作：直接返回目標位置
    TArray<FVector> Path;
    Path.Add(TargetLocation);
    return Path;
}

bool AMingUnitController::IsPathValid(AMingTacticalUnit* Unit, const TArray<FVector>& Path) const
{
    return Path.Num() > 0;
}

float AMingUnitController::GetAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const
{
    // 簡化實作：返回固定攻擊範圍
    return 200.0f;
}

bool AMingUnitController::IsInAttackRange(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit) const
{
    if (!Unit || !TargetUnit)
    {
        return false;
    }
    
    float Distance = FVector::Dist(GetUnitLocation(Unit), GetUnitLocation(TargetUnit));
    float AttackRange = GetAttackRange(Unit, TargetUnit);
    
    return Distance <= AttackRange;
}

float AMingUnitController::GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType) const
{
    // 簡化實作：基於移動類型返回速度
    switch (MovementType)
    {
    case EUnitMovementType::Walk:
        return 100.0f;
    case EUnitMovementType::Run:
        return 200.0f;
    case EUnitMovementType::Sprint:
        return 300.0f;
    case EUnitMovementType::Crawl:
        return 50.0f;
    case EUnitMovementType::Swim:
        return 80.0f;
    case EUnitMovementType::Fly:
        return 400.0f;
    case EUnitMovementType::Teleport:
        return 1000.0f;
    case EUnitMovementType::Charge:
        return 350.0f;
    default:
        return 200.0f;
    }
}

float AMingUnitController::CalculateAttackDamage(AMingTacticalUnit* Unit, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType) const
{
    // 簡化實作：基於攻擊類型返回傷害
    switch (AttackType)
    {
    case EUnitAttackType::Melee:
        return 50.0f;
    case EUnitAttackType::Ranged:
        return 30.0f;
    case EUnitAttackType::Area:
        return 40.0f;
    case EUnitAttackType::Splash:
        return 35.0f;
    case EUnitAttackType::DOT:
        return 10.0f;
    case EUnitAttackType::Debuff:
        return 0.0f;
    case EUnitAttackType::Buff:
        return 0.0f;
    case EUnitAttackType::Heal:
        return 25.0f;
    default:
        return 30.0f;
    }
}

void AMingUnitController::ApplyDamage(AMingTacticalUnit* TargetUnit, float Damage, AMingTacticalUnit* AttackerUnit)
{
    if (!TargetUnit || !AttackerUnit)
    {
        return;
    }
    
    // 簡化實作：記錄傷害
    UE_LOG(LogTemp, Log, TEXT("Unit %s dealt %.1f damage to %s"), *AttackerUnit->GetName(), Damage, *TargetUnit->GetName());
    
    // 這裡應該調用單位的傷害系統
    // TargetUnit->TakeDamage(Damage, AttackerUnit);
}

bool AMingUnitController::IsUnitAlive(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return false;
    }
    
    // 簡化實作：檢查單位是否有效
    return Unit->IsValidLowLevel();
}

FVector AMingUnitController::GetUnitLocation(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return FVector::ZeroVector;
    }
    
    return Unit->GetActorLocation();
}

void AMingUnitController::SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation)
{
    if (!Unit)
    {
        return;
    }
    
    Unit->SetActorLocation(NewLocation);
}

FRotator AMingUnitController::GetUnitRotation(AMingTacticalUnit* Unit) const
{
    if (!Unit)
    {
        return FRotator::ZeroRotator;
    }
    
    return Unit->GetActorRotation();
}

void AMingUnitController::SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation)
{
    if (!Unit)
    {
        return;
    }
    
    Unit->SetActorRotation(NewRotation);
}

void AMingUnitController::UpdatePerformanceMetrics()
{
    // 更新命令成功率
    int32 TotalCommands = ControlStatistics.FindRef(TEXT("total_commands"));
    int32 SuccessfulCommands = ControlStatistics.FindRef(TEXT("successful_commands"));
    
    if (TotalCommands > 0)
    {
        float SuccessRate = (float)SuccessfulCommands / TotalCommands;
        PerformanceMetrics.Add(TEXT("command_success_rate"), SuccessRate);
    }
    
    // 更新平均隊列大小
    int32 TotalQueueSize = 0;
    int32 UnitCount = 0;
    
    for (const auto& QueuePair : UnitCommandQueues)
    {
        TotalQueueSize += QueuePair.Value.Num();
        UnitCount++;
    }
    
    if (UnitCount > 0)
    {
        float AverageQueueSize = (float)TotalQueueSize / UnitCount;
        PerformanceMetrics.Add(TEXT("average_queue_size"), AverageQueueSize);
    }
}

void AMingUnitController::LogControlEvent(const FString& EventType, const FString& Details)
{
    UE_LOG(LogTemp, Log, TEXT("[UnitController] %s: %s"), *EventType, *Details);
}

bool AMingUnitController::ValidateUnit(AMingTacticalUnit* Unit) const
{
    return Unit != nullptr && Unit->IsValidLowLevel();
}

bool AMingUnitController::ValidateCommand(const FUnitCommand& Command) const
{
    return !Command.CommandID.IsEmpty() && Command.CommandType != EUnitCommandType::Stop;
}

FString AMingUnitController::GetCommandDescription(const FUnitCommand& Command) const
{
    switch (Command.CommandType)
    {
    case EUnitCommandType::Move:
        return FString::Printf(TEXT("Move to %s"), *Command.TargetLocation.ToString());
    case EUnitCommandType::Attack:
        if (Command.TargetUnit)
        {
            return FString::Printf(TEXT("Attack %s"), *Command.TargetUnit->GetName());
        }
        else if (Command.TargetBuilding)
        {
            return FString::Printf(TEXT("Attack building %s"), *Command.TargetBuilding->GetName());
        }
        break;
    case EUnitCommandType::Stop:
        return TEXT("Stop");
    case EUnitCommandType::Hold:
        return FString::Printf(TEXT("Hold at %s"), *Command.TargetLocation.ToString());
    case EUnitCommandType::Patrol:
        return TEXT("Patrol");
    case EUnitCommandType::Guard:
        if (Command.TargetUnit)
        {
            return FString::Printf(TEXT("Guard %s"), *Command.TargetUnit->GetName());
        }
        break;
    case EUnitCommandType::Retreat:
        return FString::Printf(TEXT("Retreat to %s"), *Command.TargetLocation.ToString());
    case EUnitCommandType::Build:
        return TEXT("Build");
    case EUnitCommandType::Repair:
        return TEXT("Repair");
    case EUnitCommandType::Heal:
        return TEXT("Heal");
    case EUnitCommandType::Capture:
        return TEXT("Capture");
    case EUnitCommandType::UseAbility:
        return TEXT("Use Ability");
    case EUnitCommandType::FormUp:
        return FString::Printf(TEXT("Form up in %s"), *UEnum::GetValueAsString(Command.FormationType));
    default:
        return TEXT("Unknown Command");
    }
    
    return TEXT("Unknown Command");
}

FString AMingUnitController::GetStateDescription(EUnitState State) const
{
    switch (State)
    {
    case EUnitState::Idle:
        return TEXT("Idle");
    case EUnitState::Moving:
        return TEXT("Moving");
    case EUnitState::Attacking:
        return TEXT("Attacking");
    case EUnitState::Defending:
        return TEXT("Defending");
    case EUnitState::Patrolling:
        return TEXT("Patrolling");
    case EUnitState::Guarding:
        return TEXT("Guarding");
    case EUnitState::Retreating:
        return TEXT("Retreating");
    case EUnitState::Dead:
        return TEXT("Dead");
    case EUnitState::Disabled:
        return TEXT("Disabled");
    case EUnitState::Captured:
        return TEXT("Captured");
    case EUnitState::Building:
        return TEXT("Building");
    case EUnitState::Healing:
        return TEXT("Healing");
    case EUnitState::Repairing:
        return TEXT("Repairing");
    default:
        return TEXT("Unknown");
    }
}

TArray<FVector> AMingUnitController::CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center)
{
    TArray<FVector> Positions;
    
    if (Units.Num() == 0)
    {
        return Positions;
    }
    
    float Spacing = 100.0f; // 單位間距
    
    switch (FormationType)
    {
    case EUnitFormationType::Line:
        {
            // 線形編隊
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                FVector Position = Center;
                Position.X += (i - Units.Num() / 2) * Spacing;
                Positions.Add(Position);
            }
            break;
        }
    case EUnitFormationType::Column:
        {
            // 縱隊編隊
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                FVector Position = Center;
                Position.Y += (i - Units.Num() / 2) * Spacing;
                Positions.Add(Position);
            }
            break;
        }
    case EUnitFormationType::Wedge:
        {
            // 楔形編隊
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                FVector Position = Center;
                if (i == 0)
                {
                    // 頂部單位
                    Position.Y += Spacing * 2;
                }
                else
                {
                    // 側翼單位
                    Position.X += (i % 2 == 0 ? -1 : 1) * Spacing;
                    Position.Y += Spacing * (2 - i / 2);
                }
                Positions.Add(Position);
            }
            break;
        }
    case EUnitFormationType::Circle:
        {
            // 圓形編隊
            float Radius = Spacing * Units.Num() / (2 * PI);
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                float Angle = (float)i / Units.Num() * 2 * PI;
                FVector Position = Center;
                Position.X += FMath::Cos(Angle) * Radius;
                Position.Y += FMath::Sin(Angle) * Radius;
                Positions.Add(Position);
            }
            break;
        }
    case EUnitFormationType::Square:
        {
            // 方形編隊
            int32 SideLength = FMath::CeilToFloat(FMath::Sqrt(Units.Num()));
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                int32 Row = i / SideLength;
                int32 Col = i % SideLength;
                FVector Position = Center;
                Position.X += (Col - SideLength / 2) * Spacing;
                Position.Y += (Row - SideLength / 2) * Spacing;
                Positions.Add(Position);
            }
            break;
        }
    default:
        {
            // 默認分散編隊
            for (int32 i = 0; i < Units.Num(); ++i)
            {
                FVector Position = Center;
                Position.X += FMath::FRandRange(-Spacing, Spacing);
                Position.Y += FMath::FRandRange(-Spacing, Spacing);
                Positions.Add(Position);
            }
            break;
        }
    }
    
    return Positions;
}

bool AMingUnitController::IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions)
{
    if (Units.Num() != FormationPositions.Num())
    {
        return false;
    }
    
    for (int32 i = 0; i < Units.Num(); ++i)
    {
        if (!ValidateUnit(Units[i]))
        {
            return false;
        }
        
        float Distance = FVector::Dist(GetUnitLocation(Units[i]), FormationPositions[i]);
        if (Distance > 50.0f) // 容忍範圍
        {
            return false;
        }
    }
    
    return true;
}

void AMingUnitController::UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions)
{
    for (int32 i = 0; i < Units.Num() && i < FormationPositions.Num(); ++i)
    {
        if (ValidateUnit(Units[i]))
        {
            SetUnitLocation(Units[i], FormationPositions[i]);
        }
    }
}
