#include "MingUnitBlueprintLibrary.h"
#include "MingUnitController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"

AMingUnitController* UMingUnitBlueprintLibrary::GetUnitController()
{
    // 這裡應該從遊戲實例或單例獲取單位控制器
    // 簡化實作
    static AMingUnitController* ControllerInstance = nullptr;
    if (!ControllerInstance)
    {
        UWorld* World = GEngine ? GEngine->GetWorldFromContextObject(GEngine, EGetWorldErrorMode::LogAndReturnNull) : nullptr;
        if (World)
        {
            ControllerInstance = World->SpawnActor<AMingUnitController>();
            ControllerInstance->InitializeUnitController();
        }
    }
    
    return ControllerInstance;
}

void UMingUnitBlueprintLibrary::InitializeUnitController()
{
    AMingUnitController* Controller = GetUnitController();
    if (Controller)
    {
        Controller->InitializeUnitController();
        UE_LOG(LogTemp, Log, TEXT("Unit controller initialized from Blueprint"));
    }
}

void UMingUnitBlueprintLibrary::ShutdownUnitController()
{
    AMingUnitController* Controller = GetUnitController();
    if (Controller)
    {
        Controller->ShutdownUnitController();
        UE_LOG(LogTemp, Log, TEXT("Unit controller shutdown from Blueprint"));
    }
}

bool UMingUnitBlueprintLibrary::AddControlledUnit(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        UE_LOG(LogTemp, Error, TEXT("Unit controller not available"));
        return false;
    }
    
    return Controller->AddControlledUnit(Unit);
}

bool UMingUnitBlueprintLibrary::RemoveControlledUnit(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->RemoveControlledUnit(Unit);
}

TArray<AMingTacticalUnit*> UMingUnitBlueprintLibrary::GetControlledUnits()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TArray<AMingTacticalUnit*>();
    }
    
    return Controller->GetControlledUnits();
}

TArray<AMingTacticalUnit*> UMingUnitBlueprintLibrary::GetUnitsByType(const FString& UnitType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TArray<AMingTacticalUnit*>();
    }
    
    return Controller->GetUnitsByType(UnitType);
}

TArray<AMingTacticalUnit*> UMingUnitBlueprintLibrary::GetActiveUnits()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TArray<AMingTacticalUnit*>();
    }
    
    return Controller->GetActiveUnits();
}

FUnitControlResult UMingUnitBlueprintLibrary::IssueUnitCommand(const FUnitCommand& Command)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->IssueUnitCommand(Command);
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::BatchIssueCommands(const TArray<FUnitCommand>& Commands)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        TArray<FUnitControlResult> Results;
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        Results.Add(Result);
        return Results;
    }
    
    return Controller->BatchIssueCommands(Commands);
}

bool UMingUnitBlueprintLibrary::CancelCommand(const FString& CommandID)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->CancelCommand(CommandID);
}

bool UMingUnitBlueprintLibrary::CancelAllCommands()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->CancelAllCommands();
}

FUnitControlResult UMingUnitBlueprintLibrary::MoveUnitsToLocation(const TArray<AMingTacticalUnit*>& Units, const FVector& TargetLocation, EUnitMovementType MovementType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->MoveUnitsToLocation(Units, TargetLocation, MovementType);
}

FUnitControlResult UMingUnitBlueprintLibrary::AttackTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetUnit, EUnitAttackType AttackType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->AttackTarget(Units, TargetUnit, AttackType);
}

FUnitControlResult UMingUnitBlueprintLibrary::AttackBuilding(const TArray<AMingTacticalUnit*>& Units, AActor* TargetBuilding, EUnitAttackType AttackType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->AttackBuilding(Units, TargetBuilding, AttackType);
}

FUnitControlResult UMingUnitBlueprintLibrary::StopUnits(const TArray<AMingTacticalUnit*>& Units)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->StopUnits(Units);
}

FUnitControlResult UMingUnitBlueprintLibrary::HoldPosition(const TArray<AMingTacticalUnit*>& Units, const FVector& HoldLocation)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->HoldPosition(Units, HoldLocation);
}

FUnitControlResult UMingUnitBlueprintLibrary::PatrolPath(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& PatrolPoints)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->PatrolPath(Units, PatrolPoints);
}

FUnitControlResult UMingUnitBlueprintLibrary::GuardTarget(const TArray<AMingTacticalUnit*>& Units, AMingTacticalUnit* TargetToGuard)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->GuardTarget(Units, TargetToGuard);
}

FUnitControlResult UMingUnitBlueprintLibrary::RetreatUnits(const TArray<AMingTacticalUnit*>& Units, const FVector& RetreatLocation)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->RetreatUnits(Units, RetreatLocation);
}

FUnitControlResult UMingUnitBlueprintLibrary::FormUpUnits(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& FormationCenter)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->FormUpUnits(Units, FormationType, FormationCenter);
}

FUnitControlResult UMingUnitBlueprintLibrary::UseAbility(const TArray<AMingTacticalUnit*>& Units, const FString& AbilityID, AMingTacticalUnit* TargetUnit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        return Result;
    }
    
    return Controller->UseAbility(Units, AbilityID, TargetUnit);
}

EUnitState UMingUnitBlueprintLibrary::GetUnitState(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return EUnitState::Idle;
    }
    
    return Controller->GetUnitState(Unit);
}

bool UMingUnitBlueprintLibrary::SetUnitState(AMingTacticalUnit* Unit, EUnitState NewState)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->SetUnitState(Unit, NewState);
}

TArray<FUnitCommand> UMingUnitBlueprintLibrary::GetUnitCommandQueue(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TArray<FUnitCommand>();
    }
    
    return Controller->GetUnitCommandQueue(Unit);
}

FUnitCommand UMingUnitBlueprintLibrary::GetCurrentCommand(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return FUnitCommand();
    }
    
    return Controller->GetCurrentCommand(Unit);
}

bool UMingUnitBlueprintLibrary::CanExecuteCommand(AMingTacticalUnit* Unit, const FUnitCommand& Command)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->CanExecuteCommand(Unit, Command);
}

TMap<FString, int32> UMingUnitBlueprintLibrary::GetUnitControlStatistics()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TMap<FString, int32>();
    }
    
    return Controller->GetUnitControlStatistics();
}

TMap<FString, float> UMingUnitBlueprintLibrary::GetPerformanceMetrics()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TMap<FString, float>();
    }
    
    return Controller->GetPerformanceMetrics();
}

FString UMingUnitBlueprintLibrary::GetStateName(EUnitState State)
{
    switch (State)
    {
    case EUnitState::Idle:
        return TEXT("空閒");
    case EUnitState::Moving:
        return TEXT("移動中");
    case EUnitState::Attacking:
        return TEXT("攻擊中");
    case EUnitState::Defending:
        return TEXT("防禦中");
    case EUnitState::Patrolling:
        return TEXT("巡邏中");
    case EUnitState::Guarding:
        return TEXT("守護中");
    case EUnitState::Retreating:
        return TEXT("撤退中");
    case EUnitState::Dead:
        return TEXT("死亡");
    case EUnitState::Disabled:
        return TEXT("殘廢");
    case EUnitState::Captured:
        return TEXT("被捕獲");
    case EUnitState::Building:
        return TEXT("建設中");
    case EUnitState::Healing:
        return TEXT("治療中");
    case EUnitState::Repairing:
        return TEXT("修理中");
    default:
        return TEXT("未知狀態");
    }
}

FString UMingUnitBlueprintLibrary::GetCommandTypeName(EUnitCommandType CommandType)
{
    switch (CommandType)
    {
    case EUnitCommandType::Move:
        return TEXT("移動");
    case EUnitCommandType::Attack:
        return TEXT("攻擊");
    case EUnitCommandType::Stop:
        return TEXT("停止");
    case EUnitCommandType::Hold:
        return TEXT("防禦");
    case EUnitCommandType::Patrol:
        return TEXT("巡邏");
    case EUnitCommandType::Guard:
        return TEXT("守護");
    case EUnitCommandType::Retreat:
        return TEXT("撤退");
    case EUnitCommandType::Build:
        return TEXT("建設");
    case EUnitCommandType::Repair:
        return TEXT("修理");
    case EUnitCommandType::Heal:
        return TEXT("治療");
    case EUnitCommandType::Capture:
        return TEXT("捕獲");
    case EUnitCommandType::UseAbility:
        return TEXT("使用技能");
    case EUnitCommandType::FormUp:
        return TEXT("編隊");
    case EUnitCommandType::Dismiss:
        return TEXT("解散");
    default:
        return TEXT("未知命令");
    }
}

FString UMingUnitBlueprintLibrary::GetPriorityName(EUnitCommandPriority Priority)
{
    switch (Priority)
    {
    case EUnitCommandPriority::Critical:
        return TEXT("關鍵");
    case EUnitCommandPriority::High:
        return TEXT("高");
    case EUnitCommandPriority::Medium:
        return TEXT("中");
    case EUnitCommandPriority::Low:
        return TEXT("低");
    case EUnitCommandPriority::Background:
        return TEXT("背景");
    default:
        return TEXT("未知優先級");
    }
}

FString UMingUnitBlueprintLibrary::GetMovementTypeName(EUnitMovementType MovementType)
{
    switch (MovementType)
    {
    case EUnitMovementType::Walk:
        return TEXT("步行");
    case EUnitMovementType::Run:
        return TEXT("跑步");
    case EUnitMovementType::Sprint:
        return TEXT("衝刺");
    case EUnitMovementType::Crawl:
        return TEXT("爬行");
    case EUnitMovementType::Swim:
        return TEXT("游泳");
    case EUnitMovementType::Fly:
        return TEXT("飛行");
    case EUnitMovementType::Teleport:
        return TEXT("傳送");
    case EUnitMovementType::Charge:
        return TEXT("衝鋒");
    default:
        return TEXT("未知移動類型");
    }
}

FString UMingUnitBlueprintLibrary::GetAttackTypeName(EUnitAttackType AttackType)
{
    switch (AttackType)
    {
    case EUnitAttackType::Melee:
        return TEXT("近戰");
    case EUnitAttackType::Ranged:
        return TEXT("遠程");
    case EUnitAttackType::Area:
        return TEXT("範圍");
    case EUnitAttackType::Splash:
        return TEXT("濺射");
    case EUnitAttackType::DOT:
        return TEXT("持續傷害");
    case EUnitAttackType::Debuff:
        return TEXT("減益");
    case EUnitAttackType::Buff:
        return TEXT("增益");
    case EUnitAttackType::Heal:
        return TEXT("治療");
    default:
        return TEXT("未知攻擊類型");
    }
}

FString UMingUnitBlueprintLibrary::GetFormationTypeName(EUnitFormationType FormationType)
{
    switch (FormationType)
    {
    case EUnitFormationType::Line:
        return TEXT("線形");
    case EUnitFormationType::Column:
        return TEXT("縱隊");
    case EUnitFormationType::Wedge:
        return TEXT("楔形");
    case EUnitFormationType::Circle:
        return TEXT("圓形");
    case EUnitFormationType::Square:
        return TEXT("方形");
    case EUnitFormationType::Diamond:
        return TEXT("菱形");
    case EUnitFormationType::Scattered:
        return TEXT("分散");
    case EUnitFormationType::Custom:
        return TEXT("自定義");
    default:
        return TEXT("未知編隊");
    }
}

FUnitCommand UMingUnitBlueprintLibrary::CreateUnitCommand(
    EUnitCommandType CommandType,
    EUnitCommandPriority Priority,
    const FVector& TargetLocation,
    AMingTacticalUnit* TargetUnit,
    AActor* TargetBuilding,
    EUnitMovementType MovementType,
    EUnitAttackType AttackType,
    EUnitFormationType FormationType)
{
    FUnitCommand Command;
    Command.CommandType = CommandType;
    Command.Priority = Priority;
    Command.TargetLocation = TargetLocation;
    Command.TargetUnit = TargetUnit;
    Command.TargetBuilding = TargetBuilding;
    Command.MovementType = MovementType;
    Command.AttackType = AttackType;
    Command.FormationType = FormationType;
    Command.CommandID = FString::Printf(TEXT("cmd_%d"), FDateTime::Now().GetMillisecond());
    Command.CommandTime = FPlatformTime::Seconds();
    Command.bIsExecuted = false;
    Command.bIsCompleted = false;
    Command.bIsCancelled = false;
    Command.RetryCount = 0;
    Command.MaxRetryCount = 3;
    
    return Command;
}

FUnitCommand UMingUnitBlueprintLibrary::SetCommandParameter(const FUnitCommand& Command, const FString& ParameterName, const FString& ParameterValue)
{
    FUnitCommand ModifiedCommand = Command;
    ModifiedCommand.CommandParameters.Add(ParameterName, ParameterValue);
    return ModifiedCommand;
}

FUnitCommand UMingUnitBlueprintLibrary::SetCommandRetryCount(const FUnitCommand& Command, int32 MaxRetryCount)
{
    FUnitCommand ModifiedCommand = Command;
    ModifiedCommand.MaxRetryCount = FMath::Max(0, MaxRetryCount);
    return ModifiedCommand;
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::BatchMoveUnits(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& TargetLocations, EUnitMovementType MovementType)
{
    TArray<FUnitControlResult> Results;
    
    for (int32 i = 0; i < Units.Num() && i < TargetLocations.Num(); ++i)
    {
        FUnitControlResult Result = MoveUnitsToLocation({Units[i]}, TargetLocations[i], MovementType);
        Results.Add(Result);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch moved %d units"), Results.Num());
    
    return Results;
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::BatchAttackTargets(const TArray<AMingTacticalUnit*>& Units, const TArray<AMingTacticalUnit*>& TargetUnits, EUnitAttackType AttackType)
{
    TArray<FUnitControlResult> Results;
    
    for (int32 i = 0; i < Units.Num() && i < TargetUnits.Num(); ++i)
    {
        FUnitControlResult Result = AttackTarget({Units[i]}, TargetUnits[i], AttackType);
        Results.Add(Result);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Batch attacked %d targets"), Results.Num());
    
    return Results;
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::BatchStopUnits(const TArray<AMingTacticalUnit*>& Units)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        TArray<FUnitControlResult> Results;
        FUnitControlResult Result;
        Result.ErrorMessage = TEXT("Unit controller not available");
        Results.Add(Result);
        return Results;
    }
    
    return Controller->StopUnits(Units);
}

bool UMingUnitBlueprintLibrary::IsUnitControllerInitialized()
{
    AMingUnitController* Controller = GetUnitController();
    return Controller != nullptr;
}

FString UMingUnitBlueprintLibrary::GetUnitControllerVersion()
{
    return TEXT("1.0.0");
}

int32 UMingUnitBlueprintLibrary::GetControlledUnitCount()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return 0;
    }
    
    return Controller->GetControlledUnits().Num();
}

int32 UMingUnitBlueprintLibrary::GetActiveUnitCount()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return 0;
    }
    
    return Controller->GetActiveUnits().Num();
}

int32 UMingUnitBlueprintLibrary::GetTotalCommandCount()
{
    int32 TotalCount = 0;
    
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return 0;
    }
    
    TArray<AMingTacticalUnit*> Units = Controller->GetControlledUnits();
    for (AMingTacticalUnit* Unit : Units)
    {
        TotalCount += Controller->GetUnitCommandQueue(Unit).Num();
    }
    
    return TotalCount;
}

int32 UMingUnitBlueprintLibrary::GetExecutingCommandCount()
{
    int32 ExecutingCount = 0;
    
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return 0;
    }
    
    TArray<AMingTacticalUnit*> Units = Controller->GetControlledUnits();
    for (AMingTacticalUnit* Unit : Units)
    {
        FUnitCommand CurrentCommand = Controller->GetCurrentCommand(Unit);
        if (!CurrentCommand.CommandID.IsEmpty())
        {
            ExecutingCount++;
        }
    }
    
    return ExecutingCount;
}

bool UMingUnitBlueprintLibrary::IsUnitControlled(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->GetControlledUnits().Contains(Unit);
}

bool UMingUnitBlueprintLibrary::IsUnitActive(AMingTacticalUnit* Unit)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->GetActiveUnits().Contains(Unit);
}

bool UMingUnitBlueprintLibrary::IsUnitIdle(AMingTacticalUnit* Unit)
{
    return GetUnitState(Unit) == EUnitState::Idle;
}

bool UMingUnitBlueprintLibrary::IsUnitMoving(AMingTacticalUnit* Unit)
{
    return GetUnitState(Unit) == EUnitState::Moving;
}

bool UMingUnitBlueprintLibrary::IsUnitAttacking(AMingTacticalUnit* Unit)
{
    return GetUnitState(Unit) == EUnitState::Attacking;
}

bool UMingUnitBlueprintLibrary::IsUnitDefending(AMingTacticalUnit* Unit)
{
    return GetUnitState(Unit) == EUnitState::Defending;
}

FVector UMingUnitBlueprintLibrary::GetUnitLocation(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return FVector::ZeroVector;
    }
    
    return Unit->GetActorLocation();
}

FRotator UMingUnitBlueprintLibrary::GetUnitRotation(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return FRotator::ZeroRotator;
    }
    
    return Unit->GetActorRotation();
}

bool UMingUnitBlueprintLibrary::SetUnitLocation(AMingTacticalUnit* Unit, const FVector& NewLocation)
{
    if (!Unit)
    {
        return false;
    }
    
    Unit->SetActorLocation(NewLocation);
    return true;
}

bool UMingUnitBlueprintLibrary::SetUnitRotation(AMingTacticalUnit* Unit, const FRotator& NewRotation)
{
    if (!Unit)
    {
        return false;
    }
    
    Unit->SetActorRotation(NewRotation);
    return true;
}

float UMingUnitBlueprintLibrary::CalculateDistance(const FVector& PointA, const FVector& PointB)
{
    return FVector::Dist(PointA, PointB);
}

float UMingUnitBlueprintLibrary::CalculateUnitDistance(AMingTacticalUnit* UnitA, AMingTacticalUnit* UnitB)
{
    if (!UnitA || !UnitB)
    {
        return 0.0f;
    }
    
    return CalculateDistance(GetUnitLocation(UnitA), GetUnitLocation(UnitB));
}

bool UMingUnitBlueprintLibrary::IsInRange(const FVector& PointA, const FVector& PointB, float Range)
{
    return CalculateDistance(PointA, PointB) <= Range;
}

bool UMingUnitBlueprintLibrary::IsUnitInAttackRange(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller || !Attacker || !Target)
    {
        return false;
    }
    
    return Controller->IsInAttackRange(Attacker, Target);
}

float UMingUnitBlueprintLibrary::GetMovementSpeed(AMingTacticalUnit* Unit, EUnitMovementType MovementType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller || !Unit)
    {
        return 0.0f;
    }
    
    return Controller->GetMovementSpeed(Unit, MovementType);
}

float UMingUnitBlueprintLibrary::CalculateMovementTime(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation, EUnitMovementType MovementType)
{
    if (!Unit)
    {
        return 0.0f;
    }
    
    float Distance = CalculateDistance(StartLocation, TargetLocation);
    float Speed = GetMovementSpeed(Unit, MovementType);
    
    return Speed > 0.0f ? Distance / Speed : 0.0f;
}

float UMingUnitBlueprintLibrary::CalculateAttackDamage(AMingTacticalUnit* Attacker, AMingTacticalUnit* Target, EUnitAttackType AttackType)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller || !Attacker || !Target)
    {
        return 0.0f;
    }
    
    return Controller->CalculateAttackDamage(Attacker, Target, AttackType);
}

TArray<FVector> UMingUnitBlueprintLibrary::CalculateFormationPositions(const TArray<AMingTacticalUnit*>& Units, EUnitFormationType FormationType, const FVector& Center)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TArray<FVector>();
    }
    
    return Controller->CalculateFormationPositions(Units, FormationType, Center);
}

bool UMingUnitBlueprintLibrary::IsFormationComplete(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    return Controller->IsFormationComplete(Units, FormationPositions);
}

bool UMingUnitBlueprintLibrary::UpdateFormation(const TArray<AMingTacticalUnit*>& Units, const TArray<FVector>& FormationPositions)
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return false;
    }
    
    Controller->UpdateFormation(Units, FormationPositions);
    return true;
}

TArray<FString> UMingUnitBlueprintLibrary::GetUnitRecommendations(AMingTacticalUnit* Unit)
{
    TArray<FString> Recommendations;
    
    if (!Unit)
    {
        return Recommendations;
    }
    
    EUnitState State = GetUnitState(Unit);
    
    switch (State)
    {
    case EUnitState::Idle:
        Recommendations.Add(TEXT("移動到戰略位置"));
        Recommendations.Add(TEXT("攻擊敵方目標"));
        Recommendations.Add(TEXT("加入編隊"));
        break;
    case EUnitState::Moving:
        Recommendations.Add(TEXT("繼續移動"));
        Recommendations.Add(TEXT("停止移動"));
        Recommendations.Add(TEXT("攻擊途中目標"));
        break;
    case EUnitState::Attacking:
        Recommendations.Add(TEXT("繼續攻擊"));
        Recommendations.Add(TEXT("停止攻擊"));
        Recommendations.Add(TEXT("撤退"));
        break;
    case EUnitState::Defending:
        Recommendations.Add(TEXT("繼續防禦"));
        Recommendations.Add(TEXT("反擊"));
        Recommendations.Add(TEXT("撤退"));
        break;
    default:
        Recommendations.Add(TEXT("等待命令"));
        break;
    }
    
    return Recommendations;
}

TArray<FString> UMingUnitBlueprintLibrary::GetTacticalRecommendations(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FString> Recommendations;
    
    if (Units.Num() == 0)
    {
        return Recommendations;
    }
    
    // 分析單位組合
    int32 MeleeCount = 0;
    int32 RangedCount = 0;
    
    for (AMingTacticalUnit* Unit : Units)
    {
        // 簡化實作：基於單位類型判斷
        FString UnitType = Unit ? Unit->GetUnitType() : TEXT("");
        if (UnitType.Contains(TEXT("melee")))
        {
            MeleeCount++;
        }
        else if (UnitType.Contains(TEXT("ranged")))
        {
            RangedCount++;
        }
    }
    
    if (MeleeCount > RangedCount)
    {
        Recommendations.Add(TEXT("建議近戰編隊"));
        Recommendations.Add(TEXT("優先攻擊近距離目標"));
    }
    else if (RangedCount > MeleeCount)
    {
        Recommendations.Add(TEXT("建議遠程編隊"));
        Recommendations.Add(TEXT("保持距離攻擊"));
    }
    else
    {
        Recommendations.Add(TEXT("混合編隊"));
        Recommendations.Add(TEXT("平衡戰術"));
    }
    
    if (Units.Num() >= 5)
    {
        Recommendations.Add(TEXT("考慮分組行動"));
        Recommendations.Add(TEXT("使用編隊陣型"));
    }
    
    return Recommendations;
}

TArray<AMingTacticalUnit*> UMingUnitBlueprintLibrary::GetAttackTargets(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<AMingTacticalUnit*> Targets;
    
    // 簡化實作：返回空數組
    // 實際應該根據遊戲狀態查找敵方單位
    return Targets;
}

TArray<FVector> UMingUnitBlueprintLibrary::GetDefensePositions(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FVector> DefensePositions;
    
    if (Units.Num() == 0)
    {
        return DefensePositions;
    }
    
    // 簡化實作：基於單位位置生成防禦位置
    FVector Center = FVector::ZeroVector;
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Center += GetUnitLocation(Unit);
        }
    }
    
    if (Units.Num() > 0)
    {
        Center /= Units.Num();
    }
    
    // 生成圍形防禦陣地
    float Radius = 200.0f;
    for (int32 i = 0; i < Units.Num(); ++i++)
    {
        float Angle = (float)i / Units.Num() * 2 * PI;
        FVector Position = Center;
        Position.X += FMath::Cos(Angle) * Radius;
        Position.Y += FMath::Sin(Angle) * Radius;
        DefensePositions.Add(Position);
    }
    
    return DefensePositions;
}

TArray<FVector> UMingUnitBlueprintLibrary::GetRetreatPositions(const TArray<AMingTacticalUnit*>& Units)
{
    TArray<FVector> RetreatPositions;
    
    if (Units.Num() == 0)
    {
        return RetreatPositions;
    }
    
    // 簡化實作：生成撤退位置
    FVector Center = FVector::ZeroVector;
    for (AMingTacticalUnit* Unit : Units)
    {
        if (Unit)
        {
            Center += GetUnitLocation(Unit);
        }
    }
    
    if (Units.Num() > 0)
    {
        Center /= Units.Num();
    }
    
    // 生成安全撤退位置
    for (int32 i = 0; i < Units.Num(); ++i++)
    {
        FVector RetreatPosition = Center;
        RetreatPosition.X += FMath::FRandRange(-500.0f, -200.0f);
        RetreatPosition.Y += FMath::FRandRange(-500.0f, -200.0f);
        RetreatPositions.Add(RetreatPosition);
    }
    
    return RetreatPositions;
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::SimulateBattle(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders, int32 SimulationCount)
{
    TArray<FUnitControlResult> SimulationResults;
    
    for (int32 i = 0; i < SimulationCount; ++i)
    {
        FUnitControlResult Result;
        Result.ExecuteTime = FPlatformTime::Seconds();
        Result.bSuccess = true;
        Result.ResultDescription = FString::Printf(TEXT("Battle simulation %d"), i + 1);
        
        // 簡化模擬：基於單位數量判斷結果
        if (Attackers.Num() > Defenders.Num())
        {
            Result.DamageDealt = (Attackers.Num() - Defenders.Num()) * 10.0f;
            Result.ResultDescription += TEXT(" - Attacker victory");
        }
        else if (Defenders.Num() > Attackers.Num())
        {
            Result.DamageDealt = (Defenders.Num() - Attackers.Num()) * 10.0f;
            Result.ResultDescription += TEXT(" - Defender victory");
        }
        else
        {
            Result.ResultDescription += TEXT(" - Draw");
        }
        
        SimulationResults.Add(Result);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Simulated %d battles"), SimulationCount);
    
    return SimulationResults;
}

TArray<FVector> UMingUnitBlueprintLibrary::SimulateMovementPath(AMingTacticalUnit* Unit, const FVector& StartLocation, const FVector& TargetLocation)
{
    TArray<FVector> Path;
    
    if (!Unit)
    {
        return Path;
    }
    
    // 簡化模擬：直線路徑
    Path.Add(StartLocation);
    
    // 添加中間點
    FVector MidPoint = (StartLocation + TargetLocation) / 2.0f;
    Path.Add(MidPoint);
    
    Path.Add(TargetLocation);
    
    return Path;
}

TArray<FUnitCommand> UMingUnitBlueprintLibrary::GetUnitHistory(AMingTacticalUnit* Unit)
{
    // 簡化實作：返回空數組
    return TArray<FUnitCommand>();
}

TArray<FUnitControlResult> UMingUnitBlueprintLibrary::GetCommandHistory()
{
    // 簡化實作：返回空數組
    return TArray<FUnitControlResult>();
}

TMap<FString, int32> UMingUnitBlueprintLibrary::GetBattleStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return Stats;
    }
    
    TMap<FString, int32> ControlStats = Controller->GetUnitControlStatistics();
    
    Stats.Add(TEXT("total_commands"), ControlStats.FindRef(TEXT("total_commands")));
    Stats.Add(TEXT("successful_commands"), ControlStats.FindRef(TEXT("successful_commands")));
    Stats.Add(TEXT("failed_commands"), ControlStats.FindRef(TEXT("failed_commands")));
    
    return Stats;
}

TMap<FString, int32> UMingUnitBlueprintLibrary::GetUnitStatistics()
{
    TMap<FString, int32> Stats;
    
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return Stats;
    }
    
    TArray<AMingTacticalUnit*> Units = Controller->GetControlledUnits();
    Stats.Add(TEXT("total_units"), Units.Num());
    Stats.Add(TEXT("active_units"), Controller->GetActiveUnits().Num());
    
    // 按狀態統計
    TMap<EUnitState, int32> StateCounts;
    for (AMingTacticalUnit* Unit : Units)
    {
        EUnitState State = Controller->GetUnitState(Unit);
        StateCounts.FindOrAdd(State, 0)++;
    }
    
    for (const auto& StateCount : StateCounts)
    {
        Stats.Add(GetStateName(StateCount.Key), StateCount.Value);
    }
    
    return Stats;
}

TMap<FString, float> UMingUnitBlueprintLibrary::GetPerformanceStatistics()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TMap<FString, float>();
    }
    
    return Controller->GetPerformanceMetrics();
}

bool UMingUnitBlueprintLibrary::ExportUnitData(const FString& FilePath)
{
    // 簡化實作：記錄導出操作
    UE_LOG(LogTemp, Log, TEXT("Unit data would be exported to: %s"), *FilePath);
    return true;
}

bool UMingUnitBlueprintLibrary::ImportUnitData(const FString& FilePath)
{
    // 簡化實作：記錄導入操作
    UE_LOG(LogTemp, Log, TEXT("Unit data would be imported from: %s"), *FilePath);
    return true;
}

FString UMingUnitBlueprintLibrary::GenerateUnitReport(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return TEXT("Invalid unit");
    }
    
    FString Report = TEXT("=== 單位報告 ===\n");
    Report += FString::Printf(TEXT("單位名稱: %s\n"), *Unit->GetName());
    Report += FString::Printf(TEXT("單位類型: %s\n"), *Unit->GetUnitType());
    Report += FString::Printf(TEXT("單位狀態: %s\n"), *GetStateName(GetUnitState(Unit)));
    Report += FString::Printf(TEXT("單位位置: %s\n"), *GetUnitLocation(Unit).ToString());
    Report += FString::Printf(TEXT("單位旋轉: %s\n"), *GetUnitRotation(Unit).ToString());
    
    AMingUnitController* Controller = GetUnitController();
    if (Controller)
    {
        TArray<FUnitCommand> CommandQueue = Controller->GetUnitCommandQueue(Unit);
        Report += FString::Printf(TEXT("命令隊列長度: %d\n"), CommandQueue.Num());
        
        FUnitCommand CurrentCommand = Controller->GetCurrentCommand(Unit);
        if (!CurrentCommand.CommandID.IsEmpty())
        {
            Report += FString::Printf(TEXT("當前命令: %s\n"), *GetCommandTypeName(CurrentCommand.CommandType));
        }
    }
    
    return Report;
}

FString UMingUnitBlueprintLibrary::GenerateControllerReport()
{
    AMingUnitController* Controller = GetUnitController();
    if (!Controller)
    {
        return TEXT("Unit controller not available");
    }
    
    FString Report = TEXT("=== 單位控制器報告 ===\n");
    
    TMap<FString, int32> Stats = GetUnitStatistics();
    Report += FString::Printf(TEXT("受控單位數量: %d\n"), Stats.FindRef(TEXT("total_units")));
    Report += FString::Printf(TEXT("活躍單位數量: %d\n"), Stats.FindRef(TEXT("active_units")));
    
    TMap<FString, int32> BattleStats = GetBattleStatistics();
    Report += FString::Printf(TEXT("總命令數量: %d\n"), BattleStats.FindRef(TEXT("total_commands")));
    Report += FString::Printf(TEXT("成功命令數量: %d\n"), BattleStats.FindRef(TEXT("successful_commands")));
    Report += FString::Printf(TEXT("失敗命令數量: %d\n"), BattleStats.FindRef(TEXT("failed_commands")));
    
    TMap<FString, float> PerformanceStats = GetPerformanceStatistics();
    Report += FString::Printf(TEXT("命令成功率: %.1f%%\n"), PerformanceStats.FindRef(TEXT("command_success_rate")) * 100.0f);
    Report += FString::Printf(TEXT("平均隊列大小: %.1f\n"), PerformanceStats.FindRef(TEXT("average_queue_size")));
    
    return Report;
}

FString UMingUnitBlueprintLibrary::GenerateBattleReport(const TArray<AMingTacticalUnit*>& Attackers, const TArray<AMingTacticalUnit*>& Defenders)
{
    FString Report = TEXT("=== 戰鬥報告 ===\n");
    Report += FString::Printf(TEXT("攻擊方單位: %d\n"), Attackers.Num());
    Report += FString::Printf(TEXT("防禦方單位: %d\n"), Defenders.Num());
    
    if (Attackers.Num() > 0)
    {
        Report += TEXT("\n=== 攻擊方單位 ===\n");
        for (AMingTacticalUnit* Unit : Attackers)
        {
            if (Unit)
            {
                Report += FString::Printf(TEXT("- %s (%s)\n"), *Unit->GetName(), *GetStateName(GetUnitState(Unit)));
            }
        }
    }
    
    if (Defenders.Num() > 0)
    {
        Report += TEXT("\n=== 防禦方單位 ===\n");
        for (AMingTacticalUnit* Unit : Defenders)
        {
            if (Unit)
            {
                Report += FString::Printf(TEXT("- %s (%s)\n"), *Unit->GetName(), *GetStateName(GetUnitState(Unit)));
            }
        }
    }
    
    return Report;
}

void UMingUnitBlueprintLibrary::ResetUnitController()
{
    AMingUnitController* Controller = GetUnitController();
    if (Controller)
    {
        Controller->ShutdownUnitController();
        Controller->InitializeUnitController();
        UE_LOG(LogTemp, Log, TEXT("Unit controller reset"));
    }
}

void UMingUnitBlueprintLibrary::ClearAllCommands()
{
    AMingUnitController* Controller = GetUnitController();
    if (Controller)
    {
        Controller->CancelAllCommands();
        UE_LOG(LogTemp, Log, TEXT("All commands cleared"));
    }
}

void UMingUnitBlueprintLibrary::ClearUnitHistory()
{
    // 簡化實作：記錄清除操作
    UE_LOG(LogTemp, Log, TEXT("Unit history cleared"));
}

bool UMingUnitBlueprintLibrary::BackupUnitData(const FString& BackupPath)
{
    // 簡化實作：記錄備份操作
    UE_LOG(LogTemp, Log, TEXT("Unit data would be backed up to: %s"), *BackupPath);
    return true;
}

bool UMingUnitBlueprintLibrary::RestoreUnitData(const FString& BackupPath)
{
    // 簡化實作：記錄恢復操作
    UE_LOG(LogTemp, Log, TEXT("Unit data would be restored from: %s"), *BackupPath);
    return true;
}
