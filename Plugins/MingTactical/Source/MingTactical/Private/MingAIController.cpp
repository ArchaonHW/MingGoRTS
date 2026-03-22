#include "MingAIController.h"
#include "MingTacticalUnit.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AMingAIController::AMingAIController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    CurrentState = EMingAIState::Idle;
    AttackRange = 200.0f;
    AttackCooldown = 1.0f;
    MoveAcceptanceRadius = 50.0f;
    RotationSpeed = 5.0f;
    bUsePathfinding = true;
    UpdateInterval = 0.1f; // 10Hz update rate for performance
    TimeSinceLastUpdate = 0.0f;

    FormationOffset = FVector::ZeroVector;
    bHasFormationPosition = false;

    bHasActiveMove = false;
    LastAttackTime = 0.0f;
    StateEnterTime = 0.0f;

    // Enable ticking for AI updates
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.016f; // ~60Hz
}

void AMingAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Cache controlled unit
    ControlledUnit = Cast<AMingTacticalUnit>(InPawn);
    if (ControlledUnit.IsValid())
    {
        UE_LOG(LogTemp, Log, TEXT("AI Controller possessed unit %d"), ControlledUnit->UnitId);
    }

    // Get navigation system
    NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not found for AI Controller"));
    }

    // Initialize state
    EnterState(EMingAIState::Idle);
}

void AMingAIController::OnUnPossess()
{
    // Clear any active commands
    CommandQueue.Empty();
    
    // Stop movement
    StopMovement();

    Super::OnUnPossess();

    ControlledUnit.Reset();
    UE_LOG(LogTemp, Log, TEXT("AI Controller un-possessed unit"));
}

void AMingAIController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Performance optimization: throttle AI updates
    TimeSinceLastUpdate += DeltaTime;
    if (TimeSinceLastUpdate >= UpdateInterval)
    {
        UpdateAI(TimeSinceLastUpdate);
        TimeSinceLastUpdate = 0.0f;
    }
}

void AMingAIController::UpdateAI(float DeltaTime)
{
    if (!ControlledUnit.IsValid() || !ControlledUnit->IsAlive())
    {
        return;
    }

    // Process current command
    ProcessCurrentCommand(DeltaTime);

    // Update based on current state
    switch (CurrentState)
    {
    case EMingAIState::Moving:
        UpdateMovement(DeltaTime);
        break;
    case EMingAIState::Attacking:
        UpdateCombat(DeltaTime);
        break;
    case EMingAIState::Following:
        UpdateFormation(DeltaTime);
        break;
    default:
        break;
    }
}

void AMingAIController::ProcessCurrentCommand(float DeltaTime)
{
    if (CommandQueue.Num() > 0)
    {
        FMingAICommand& NextCommand = CommandQueue[0];
        
        // Check if command should be processed (priority check)
        if (NextCommand.Priority >= CurrentCommand.Priority || 
            CurrentState == EMingAIState::Idle)
        {
            CurrentCommand = NextCommand;
            CommandQueue.RemoveAt(0);
            
            // Execute command
            switch (CurrentCommand.CommandType)
            {
            case EMingAIState::Moving:
                MoveToLocation(CurrentCommand.TargetLocation);
                break;
            case EMingAIState::Attacking:
                if (CurrentCommand.TargetActor.IsValid())
                {
                    SetAttackTarget(CurrentCommand.TargetActor.Get());
                }
                break;
            case EMingAIState::Idle:
                StopMovement();
                break;
            default:
                break;
            }
        }
    }
}

void AMingAIController::UpdateMovement(float DeltaTime)
{
    if (!bHasActiveMove)
    {
        // Check if we have a formation position to move to
        if (bHasFormationPosition && ControlledUnit.IsValid())
        {
            FVector FormationTarget = ControlledUnit->GetActorLocation() + FormationOffset;
            MoveToLocation(FormationTarget);
        }
        else
        {
            // No active movement, transition to idle
            EnterState(EMingAIState::Idle);
        }
        return;
    }

    // Check if we've reached the destination
    if (HasReachedDestination())
    {
        bHasActiveMove = false;
        
        // If this was an attack move, look for targets
        if (CurrentCommand.CommandType == EMingAIState::Moving && 
            CurrentAttackTarget.IsValid())
        {
            AActor* Target = FindNearestEnemy(AttackRange * 2.0f);
            if (Target)
            {
                IssueAttackCommand(Target);
                return;
            }
        }
        
        EnterState(EMingAIState::Idle);
    }
}

void AMingAIController::UpdateCombat(float DeltaTime)
{
    if (!CurrentAttackTarget.IsValid() || !CurrentAttackTarget->IsValidLowLevel())
    {
        // Target lost, find new one or return to idle
        FindNewTarget();
        return;
    }

    // Check if we can attack
    if (CanAttackTarget(CurrentAttackTarget.Get()))
    {
        PerformAttack(CurrentAttackTarget.Get());
    }
    else
    {
        // Move closer to target
        MoveToActor(CurrentAttackTarget.Get(), AttackRange * 0.8f);
    }
}

void AMingAIController::UpdateFormation(float DeltaTime)
{
    if (!bHasFormationPosition || !ControlledUnit.IsValid())
    {
        return;
    }

    // Calculate desired position in formation
    FVector DesiredPosition = ControlledUnit->GetActorLocation() + FormationOffset;
    
    // Check if we need to adjust position
    float DistanceToFormation = FVector::Dist(ControlledUnit->GetActorLocation(), DesiredPosition);
    if (DistanceToFormation > MoveAcceptanceRadius)
    {
        // Move to formation position
        MoveToLocation(DesiredPosition, MoveAcceptanceRadius * 0.5f);
    }
}

void AMingAIController::EnterState(EMingAIState NewState)
{
    if (CurrentState == NewState)
    {
        return;
    }

    EMingAIState OldState = CurrentState;
    CurrentState = NewState;
    StateEnterTime = GetWorld()->GetTimeSeconds();

    ExitState(OldState);

    // Handle state entry
    switch (NewState)
    {
    case EMingAIState::Moving:
        UE_LOG(LogTemp, Verbose, TEXT("AI entering Move state"));
        break;
    case EMingAIState::Attacking:
        UE_LOG(LogTemp, Verbose, TEXT("AI entering Attack state"));
        break;
    case EMingAIState::Idle:
        UE_LOG(LogTemp, Verbose, TEXT("AI entering Idle state"));
        break;
    default:
        break;
    }
}

void AMingAIController::ExitState(EMingAIState OldState)
{
    // Cleanup when leaving a state
    switch (OldState)
    {
    case EMingAIState::Moving:
        // Stop movement when leaving move state
        if (CurrentState != EMingAIState::Attacking)
        {
            StopMovement();
        }
        break;
    case EMingAIState::Attacking:
        CurrentAttackTarget.Reset();
        break;
    default:
        break;
    }
}

void AMingAIController::IssueMoveCommand(const FVector& TargetLocation, bool bAttackMove)
{
    FMingAICommand Command;
    Command.CommandType = EMingAIState::Moving;
    Command.TargetLocation = TargetLocation;
    Command.Priority = bAttackMove ? 2.0f : 1.0f;

    if (bAttackMove)
    {
        // For attack move, we'll look for enemies along the way
        CurrentAttackTarget = nullptr; // Reset attack target
    }

    CommandQueue.Add(Command);
    
    // If idle, process immediately
    if (CurrentState == EMingAIState::Idle)
    {
        ProcessCurrentCommand(0.0f);
    }

    EnterState(EMingAIState::Moving);
    
    UE_LOG(LogTemp, Log, TEXT("Move command issued: %s (AttackMove: %s)"), 
           *TargetLocation.ToString(), bAttackMove ? TEXT("Yes") : TEXT("No"));
}

void AMingAIController::IssueAttackCommand(AActor* Target)
{
    if (!Target)
    {
        return;
    }

    FMingAICommand Command;
    Command.CommandType = EMingAIState::Attacking;
    Command.TargetActor = Target;
    Command.Priority = 3.0f; // Higher priority than movement

    CommandQueue.Add(Command);

    // Interrupt current action if it's lower priority
    if (CurrentState != EMingAIState::Attacking || CurrentCommand.Priority < Command.Priority)
    {
        CommandQueue.Sort([](const FMingAICommand& A, const FMingAICommand& B)
        {
            return A.Priority > B.Priority;
        });
        
        ProcessCurrentCommand(0.0f);
    }

    EnterState(EMingAIState::Attacking);
    
    UE_LOG(LogTemp, Log, TEXT("Attack command issued against: %s"), *Target->GetName());
}

void AMingAIController::IssueStopCommand()
{
    CommandQueue.Empty();
    StopMovement();
    CurrentAttackTarget.Reset();
    
    EnterState(EMingAIState::Idle);
    
    UE_LOG(LogTemp, Log, TEXT("Stop command issued"));
}

void AMingAIController::IssuePatrolCommand(const FVector& PatrolPoint)
{
    // For simplicity, just move to patrol point
    IssueMoveCommand(PatrolPoint);
    
    // TODO: Implement full patrol behavior (move between multiple points)
    UE_LOG(LogTemp, Log, TEXT("Patrol command issued to: %s"), *PatrolPoint.ToString());
}

void AMingAIController::IssueFollowCommand(AActor* Leader, float FollowDistance)
{
    if (!Leader)
    {
        return;
    }

    FMingAICommand Command;
    Command.CommandType = EMingAIState::Following;
    Command.TargetActor = Leader;
    Command.Priority = 1.5f;

    // Calculate formation position behind leader
    FVector LeaderLocation = Leader->GetActorLocation();
    FVector LeaderForward = Leader->GetActorForwardVector();
    Command.TargetLocation = LeaderLocation - (LeaderForward * FollowDistance);

    CommandQueue.Add(Command);
    
    UE_LOG(LogTemp, Log, TEXT("Follow command issued: following %s at distance %.0f"), 
           *Leader->GetName(), FollowDistance);
}

void AMingAIController::SetFormationPosition(const FVector& Offset)
{
    FormationOffset = Offset;
    bHasFormationPosition = true;
}

void AMingAIController::ClearFormationPosition()
{
    FormationOffset = FVector::ZeroVector;
    bHasFormationPosition = false;
}

bool AMingAIController::HasFormationPosition() const
{
    return bHasFormationPosition;
}

bool AMingAIController::MoveToLocation(const FVector& TargetLocation, float AcceptanceRadius)
{
    if (!NavSystem)
    {
        return false;
    }

    // Use UE5's built-in navigation
    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalLocation(TargetLocation);
    MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
    MoveRequest.SetUsePathfinding(bUsePathfinding);
    MoveRequest.SetAllowPartialPath(true);

    FPathFollowingRequestResult MoveResult = MoveTo(MoveRequest);
    
    if (MoveResult.Code == EPathFollowingRequestResult::Type::RequestSuccessful)
    {
        CurrentMoveRequest = MoveResult.MoveId;
        CurrentDestination = TargetLocation;
        bHasActiveMove = true;
        
        return true;
    }

    return false;
}

bool AMingAIController::MoveToActor(AActor* TargetActor, float AcceptanceRadius)
{
    if (!TargetActor)
    {
        return false;
    }

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(TargetActor);
    MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
    MoveRequest.SetUsePathfinding(bUsePathfinding);

    FPathFollowingRequestResult MoveResult = MoveTo(MoveRequest);
    
    if (MoveResult.Code == EPathFollowingRequestResult::Type::RequestSuccessful)
    {
        CurrentMoveRequest = MoveResult.MoveId;
        CurrentDestination = TargetActor->GetActorLocation();
        bHasActiveMove = true;
        
        return true;
    }

    return false;
}

void AMingAIController::StopMovement()
{
    Super::StopMovement();
    
    bHasActiveMove = false;
    CurrentMoveRequest = FAIRequestID::InvalidRequestId;
    CurrentDestination = FVector::ZeroVector;
}

AActor* AMingAIController::FindNearestEnemy(float SearchRadius) const
{
    if (!ControlledUnit.IsValid())
    {
        return nullptr;
    }

    FVector SearchLocation = ControlledUnit->GetActorLocation();
    TArray<FOverlapResult> OverlapResults;
    
    FCollisionShape CollisionShape = FCollisionShape::MakeSphere(SearchRadius);
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(ControlledUnit.Get());

    // Perform overlap check
    GetWorld()->OverlapMultiByChannel(OverlapResults, SearchLocation, FQuat::Identity, 
                                       ECC_Pawn, CollisionShape, QueryParams);

    AActor* NearestEnemy = nullptr;
    float MinDistance = MAX_flt;

    for (const FOverlapResult& Result : OverlapResults)
    {
        AActor* PotentialTarget = Result.GetActor();
        if (PotentialTarget && PotentialTarget != ControlledUnit.Get())
        {
            // TODO: Add faction/team check here
            float Distance = FVector::Dist(SearchLocation, PotentialTarget->GetActorLocation());
            if (Distance < MinDistance)
            {
                MinDistance = Distance;
                NearestEnemy = PotentialTarget;
            }
        }
    }

    return NearestEnemy;
}

AActor* AMingAIController::FindWeakestEnemy(float SearchRadius) const
{
    // For now, just return the nearest enemy
    // TODO: Implement health-based target selection
    return FindNearestEnemy(SearchRadius);
}

bool AMingAIController::CanAttackTarget(AActor* Target) const
{
    if (!Target || !ControlledUnit.IsValid())
    {
        return false;
    }

    float DistanceToTarget = FVector::Dist(ControlledUnit->GetActorLocation(), Target->GetActorLocation());
    return DistanceToTarget <= AttackRange;
}

void AMingAIController::SetAttackTarget(AActor* Target)
{
    if (CurrentAttackTarget.IsValid())
    {
        // Unbind from old target's destruction event
        CurrentAttackTarget->OnDestroyed.RemoveDynamic(this, &AMingAIController::OnTargetDestroyed);
    }

    CurrentAttackTarget = Target;

    if (Target)
    {
        // Bind to new target's destruction event
        Target->OnDestroyed.AddUniqueDynamic(this, &AMingAIController::OnTargetDestroyed);
    }
}

void AMingAIController::PerformAttack(AActor* Target)
{
    if (!Target || !ControlledUnit.IsValid())
    {
        return;
    }

    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastAttackTime < AttackCooldown)
    {
        return; // Still in cooldown
    }

    // Perform the attack
    // This should integrate with the unit's attack system
    ControlledUnit->AttackTarget(Target);
    
    LastAttackTime = CurrentTime;
    
    // Debug visualization
    if (GEngine && GEngine->GameViewport)
    {
        DrawDebugLine(GetWorld(), ControlledUnit->GetActorLocation(), Target->GetActorLocation(), 
                      FColor::Red, false, 0.5f, 0, 2.0f);
    }
}

void AMingAIController::FindNewTarget()
{
    AActor* NewTarget = FindNearestEnemy(AttackRange * 2.0f);
    if (NewTarget)
    {
        SetAttackTarget(NewTarget);
    }
    else
    {
        // No targets found, return to idle
        CurrentAttackTarget.Reset();
        EnterState(EMingAIState::Idle);
    }
}

bool AMingAIController::HasReachedDestination() const
{
    if (!ControlledUnit.IsValid() || !bHasActiveMove)
    {
        return true;
    }

    float DistanceToDestination = FVector::Dist(ControlledUnit->GetActorLocation(), CurrentDestination);
    return DistanceToDestination <= MoveAcceptanceRadius;
}

FVector AMingAIController::GetDestination() const
{
    return CurrentDestination;
}

void AMingAIController::OnTargetDestroyed(AActor* DestroyedActor)
{
    if (DestroyedActor == CurrentAttackTarget.Get())
    {
        UE_LOG(LogTemp, Log, TEXT("AI target destroyed, finding new target"));
        FindNewTarget();
    }
}

void AMingAIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
    if (RequestID == CurrentMoveRequest)
    {
        bHasActiveMove = false;
        
        if (Result == EPathFollowingResult::Success)
        {
            UE_LOG(LogTemp, Verbose, TEXT("AI move completed successfully"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("AI move failed or blocked"));
        }
        
        // If we were moving, transition to idle
        if (CurrentState == EMingAIState::Moving)
        {
            EnterState(EMingAIState::Idle);
        }
    }
}
