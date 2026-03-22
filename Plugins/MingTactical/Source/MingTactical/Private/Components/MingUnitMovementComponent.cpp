#include "Components/MingUnitMovementComponent.h"
#include "Units/MingTacticalUnit.h"
#include "NavigationSystem.h"
#include "NavMesh/NavMeshPath.h"
#include "AI/Navigation/NavigationTypes.h"

UMingUnitMovementComponent::UMingUnitMovementComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    bIsMoving = false;
    MoveSpeed = 300.0f;
    RotationSpeed = 360.0f;
    AcceptanceRadius = 50.0f;
    CurrentPathIndex = 0;
    PathRequestId = 0;
    bIsAttackMove = false;
    FormationOffset = FVector::ZeroVector;
    
    NavSystem = nullptr;
}

void UMingUnitMovementComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取擁有者單位
    OwnerUnit = Cast<AMingTacticalUnit>(GetOwner());
    
    // 獲取導航系統
    NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    
    if (OwnerUnit)
    {
        // 從單位屬性獲取移動速度
        MoveSpeed = OwnerUnit->UnitStats.MoveSpeed;
    }
}

void UMingUnitMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (bIsMoving)
    {
        FollowPath(DeltaTime);
    }
}

void UMingUnitMovementComponent::MoveToLocation(const FVector& TargetLocation, bool bAttackMove)
{
    if (!OwnerUnit || !NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot move: Owner or NavSystem is null"));
        return;
    }
    
    FVector CurrentLocation = OwnerUnit->GetActorLocation();
    
    // 檢查是否已經在目標位置附近
    if (FVector::Distance(CurrentLocation, TargetLocation) <= AcceptanceRadius)
    {
        UE_LOG(LogTemp, Verbose, TEXT("Already at target location"));
        return;
    }
    
    CurrentTargetLocation = TargetLocation;
    bIsAttackMove = bAttackMove;
    FormationOffset = FVector::ZeroVector;
    
    // 請求路徑
    RequestPath(CurrentLocation, TargetLocation);
    
    UE_LOG(LogTemp, Verbose, TEXT("Move command issued to (%f, %f, %f)"),
        TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
}

void UMingUnitMovementComponent::MoveToLocationWithFormation(const FVector& TargetLocation, const FVector& InFormationOffset)
{
    FormationOffset = InFormationOffset;
    
    // 調整目標位置以考慮隊形偏移
    FVector AdjustedTarget = TargetLocation + FormationOffset;
    
    MoveToLocation(AdjustedTarget, false);
}

void UMingUnitMovementComponent::StopMovement()
{
    bIsMoving = false;
    CurrentPathPoints.Empty();
    CurrentPathIndex = 0;
    
    if (OwnerUnit)
    {
        // 設置單位狀態為 Idle
        OwnerUnit->ChangeState(EUnitState::Idle);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Movement stopped"));
}

float UMingUnitMovementComponent::GetCurrentSpeed() const
{
    return bIsMoving ? MoveSpeed : 0.0f;
}

void UMingUnitMovementComponent::SetMoveSpeed(float NewSpeed)
{
    MoveSpeed = FMath::Max(0.0f, NewSpeed);
    
    if (OwnerUnit)
    {
        OwnerUnit->UnitStats.MoveSpeed = MoveSpeed;
    }
}

FVector UMingUnitMovementComponent::GetCurrentTargetLocation() const
{
    return CurrentTargetLocation;
}

void UMingUnitMovementComponent::RotateTowards(const FVector& TargetDirection, float DeltaTime)
{
    if (!OwnerUnit || TargetDirection.IsNearlyZero())
    {
        return;
    }
    
    FVector NormalizedDirection = TargetDirection.GetSafeNormal();
    FRotator TargetRotation = NormalizedDirection.Rotation();
    
    FRotator CurrentRotation = OwnerUnit->GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed / 360.0f);
    
    OwnerUnit->SetActorRotation(NewRotation);
}

void UMingUnitMovementComponent::SetDesiredRotation(const FRotator& TargetRotation)
{
    DesiredRotation = TargetRotation;
}

void UMingUnitMovementComponent::RequestPath(const FVector& StartLocation, const FVector& EndLocation)
{
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not available"));
        return;
    }
    
    // 簡化版：直接設置直線路徑
    // 在實際實現中，應該使用 NavSystem->FindPathToLocationAsync
    CurrentPathPoints.Empty();
    CurrentPathPoints.Add(StartLocation);
    CurrentPathPoints.Add(EndLocation);
    CurrentPathIndex = 1; // 從第二個點開始移動
    
    bIsMoving = true;
    
    if (OwnerUnit)
    {
        OwnerUnit->ChangeState(bIsAttackMove ? EUnitState::Attacking : EUnitState::Moving);
    }
    
    // 發布移動事件
    FUnitMovedEvent Event(OwnerUnit ? OwnerUnit->UnitId : -1, EndLocation, bIsAttackMove);
    IMingCoreEventBus::PublishEvent(Event);
    
    UE_LOG(LogTemp, Verbose, TEXT("Path requested from (%f, %f, %f) to (%f, %f, %f)"),
        StartLocation.X, StartLocation.Y, StartLocation.Z,
        EndLocation.X, EndLocation.Y, EndLocation.Z);
}

void UMingUnitMovementComponent::OnPathFound(uint32 InPathRequestId, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path)
{
    if (Result == ENavigationQueryResult::Success && Path.IsValid())
    {
        CurrentPathPoints.Empty();
        
        // 將路徑點轉換為 FVector 數組
        for (const FNavPathPoint& PathPoint : Path->GetPathPoints())
        {
            CurrentPathPoints.Add(PathPoint.Location);
        }
        
        CurrentPathIndex = 1; // 從第二個點開始（第一個是當前位置）
        bIsMoving = true;
        
        UE_LOG(LogTemp, Verbose, TEXT("Path found with %d points"), CurrentPathPoints.Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Path finding failed"));
        bIsMoving = false;
    }
}

void UMingUnitMovementComponent::FollowPath(float DeltaTime)
{
    if (!OwnerUnit || CurrentPathPoints.Num() == 0)
    {
        StopMovement();
        return;
    }
    
    // 檢查是否還有路徑點
    if (CurrentPathIndex >= CurrentPathPoints.Num())
    {
        // 到達終點
        StopMovement();
        return;
    }
    
    FVector CurrentLocation = OwnerUnit->GetActorLocation();
    FVector TargetPoint = CurrentPathPoints[CurrentPathIndex];
    
    // 計算到目標點的方向
    FVector Direction = TargetPoint - CurrentLocation;
    Direction.Z = 0; // 保持在地面上
    float DistanceToTarget = Direction.Size();
    
    // 檢查是否到達當前路徑點
    if (DistanceToTarget <= AcceptanceRadius)
    {
        CurrentPathIndex++;
        
        // 如果已經是最後一個點，停止移動
        if (CurrentPathIndex >= CurrentPathPoints.Num())
        {
            StopMovement();
        }
        return;
    }
    
    // 移動向目標
    Direction.Normalize();
    
    // 計算這幀的移動距離
    float MoveDistance = MoveSpeed * DeltaTime;
    
    // 如果這幀會超過目標，只移動到目標
    if (MoveDistance >= DistanceToTarget)
    {
        MoveDistance = DistanceToTarget;
    }
    
    // 計算新位置
    FVector NewLocation = CurrentLocation + Direction * MoveDistance;
    NewLocation.Z = CurrentLocation.Z; // 保持原來的高度
    
    OwnerUnit->SetActorLocation(NewLocation);
    
    // 旋轉向移動方向
    RotateTowards(Direction, DeltaTime);
}

void UMingUnitMovementComponent::SmoothRotateTowards(const FVector& Direction, float DeltaTime)
{
    RotateTowards(Direction, DeltaTime);
}

bool UMingUnitMovementComponent::HasReachedTarget() const
{
    if (!OwnerUnit || CurrentPathPoints.Num() == 0)
    {
        return true;
    }
    
    FVector CurrentLocation = OwnerUnit->GetActorLocation();
    FVector FinalTarget = CurrentPathPoints.Last();
    
    float DistanceToTarget = FVector::Distance(CurrentLocation, FinalTarget);
    return DistanceToTarget <= AcceptanceRadius;
}

void UMingUnitMovementComponent::UpdateMovementState()
{
    if (!bIsMoving && OwnerUnit)
    {
        if (OwnerUnit->GetCurrentState() == EUnitState::Moving ||
            OwnerUnit->GetCurrentState() == EUnitState::Attacking)
        {
            OwnerUnit->ChangeState(EUnitState::Idle);
        }
    }
}
