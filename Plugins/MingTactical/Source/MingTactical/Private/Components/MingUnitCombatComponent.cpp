#include "Components/MingUnitCombatComponent.h"
#include "Units/MingTacticalUnit.h"
#include "Kismet/GameplayStatics.h"

UMingUnitCombatComponent::UMingUnitCombatComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    CurrentTarget = nullptr;
    TargetAttackLocation = FVector::ZeroVector;
    bIsAttackingLocation = false;
    bIsAttacking = false;
    AttackCooldown = 1.0f;
    CurrentCooldown = 0.0f;
    AttackRange = 100.0f;
    AttackDamage = 20.0f;
    
    OwnerUnit = nullptr;
}

void UMingUnitCombatComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取擁有者單位
    OwnerUnit = Cast<AMingTacticalUnit>(GetOwner());
    
    if (OwnerUnit)
    {
        // 從單位屬性獲取攻擊參數
        AttackRange = OwnerUnit->UnitStats.AttackRange;
        AttackDamage = OwnerUnit->UnitStats.AttackDamage;
        AttackCooldown = 1.0f / OwnerUnit->UnitStats.AttackSpeed;
    }
}

void UMingUnitCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 更新冷卻時間
    if (CurrentCooldown > 0)
    {
        CurrentCooldown -= DeltaTime;
    }
    
    // 處理攻擊邏輯
    if (bIsAttacking)
    {
        if (CurrentTarget.IsValid())
        {
            // 有目標單位
            AMingTacticalUnit* Target = CurrentTarget.Get();
            
            if (!IsValidTarget(Target))
            {
                // 目標無效，停止攻擊
                StopAttack();
                return;
            }
            
            // 旋轉向目標
            RotateToTarget(DeltaTime);
            
            // 檢查是否在攻擊範圍內
            if (IsTargetInRange(Target))
            {
                // 執行攻擊
                if (CurrentCooldown <= 0)
                {
                    PerformAttack();
                }
            }
            else
            {
                // 目標不在範圍內，可能需要移動靠近
                // TODO: 通知移動組件向目標移動
            }
        }
        else if (bIsAttackingLocation)
        {
            // 攻擊地面位置
            if (IsLocationInRange(TargetAttackLocation))
            {
                if (CurrentCooldown <= 0)
                {
                    PerformAttack();
                }
            }
        }
        else
        {
            // 尋找範圍內的敵人
            AMingTacticalUnit* Enemy = FindEnemyInRange();
            if (Enemy)
            {
                AttackTarget(Enemy);
            }
        }
    }
}

void UMingUnitCombatComponent::AttackTarget(AMingTacticalUnit* Target)
{
    if (!OwnerUnit || !Target)
    {
        return;
    }
    
    // 不能攻擊自己或隊友
    if (Target == OwnerUnit || Target->TeamId == OwnerUnit->TeamId)
    {
        return;
    }
    
    CurrentTarget = Target;
    bIsAttackingLocation = false;
    bIsAttacking = true;
    
    // 設置單位狀態
    OwnerUnit->ChangeState(EUnitState::Attacking);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d attacking target unit %d"),
        OwnerUnit->UnitId, Target->UnitId);
}

void UMingUnitCombatComponent::AttackLocation(const FVector& TargetLocation)
{
    if (!OwnerUnit)
    {
        return;
    }
    
    TargetAttackLocation = TargetLocation;
    bIsAttackingLocation = true;
    CurrentTarget = nullptr;
    bIsAttacking = true;
    
    // 設置單位狀態
    OwnerUnit->ChangeState(EUnitState::Attacking);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d attacking location (%f, %f, %f)"),
        OwnerUnit->UnitId, TargetLocation.X, TargetLocation.Y, TargetLocation.Z);
}

void UMingUnitCombatComponent::StopAttack()
{
    bIsAttacking = false;
    CurrentTarget = nullptr;
    bIsAttackingLocation = false;
    
    if (OwnerUnit)
    {
        // 如果正在移動，保持移動狀態，否則設為 Idle
        if (OwnerUnit->GetCurrentState() == EUnitState::Attacking)
        {
            OwnerUnit->ChangeState(EUnitState::Idle);
        }
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Attack stopped"));
}

bool UMingUnitCombatComponent::IsTargetInRange(AMingTacticalUnit* Target) const
{
    if (!OwnerUnit || !Target)
    {
        return false;
    }
    
    FVector OwnerLocation = OwnerUnit->GetActorLocation();
    FVector TargetLocation = Target->GetActorLocation();
    
    float Distance = FVector::Distance(OwnerLocation, TargetLocation);
    return Distance <= AttackRange;
}

bool UMingUnitCombatComponent::IsLocationInRange(const FVector& Location) const
{
    if (!OwnerUnit)
    {
        return false;
    }
    
    FVector OwnerLocation = OwnerUnit->GetActorLocation();
    float Distance = FVector::Distance(OwnerLocation, Location);
    return Distance <= AttackRange;
}

bool UMingUnitCombatComponent::CanAttack() const
{
    return CurrentCooldown <= 0 && OwnerUnit && OwnerUnit->IsAlive();
}

void UMingUnitCombatComponent::SetAttackCooldown(float Cooldown)
{
    AttackCooldown = FMath::Max(0.1f, Cooldown);
}

void UMingUnitCombatComponent::PerformAttack()
{
    if (!CanAttack())
    {
        return;
    }
    
    // 設置冷卻
    CurrentCooldown = AttackCooldown;
    
    // 觸發攻擊動畫
    TriggerAttackAnimation();
    
    // 對目標造成傷害
    if (CurrentTarget.IsValid())
    {
        DealDamageToTarget(CurrentTarget.Get());
    }
    else if (bIsAttackingLocation)
    {
        // 攻擊地面位置的邏輯（例如範圍傷害）
        // TODO: 實現範圍攻擊
        UE_LOG(LogTemp, Verbose, TEXT("Attacking location (%f, %f, %f)"),
            TargetAttackLocation.X, TargetAttackLocation.Y, TargetAttackLocation.Z);
    }
}

void UMingUnitCombatComponent::DealDamageToTarget(AMingTacticalUnit* Target)
{
    if (!Target || !OwnerUnit)
    {
        return;
    }
    
    // 對目標造成傷害
    Target->TakeDamage(AttackDamage, OwnerUnit);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d dealt %f damage to unit %d"),
        OwnerUnit->UnitId, AttackDamage, Target->UnitId);
}

AMingTacticalUnit* UMingUnitCombatComponent::FindEnemyInRange()
{
    if (!OwnerUnit)
    {
        return nullptr;
    }
    
    // 簡單的範圍檢測：獲取所有 AMingTacticalUnit 並檢查距離和隊伍
    TArray<AActor*> AllUnits;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMingTacticalUnit::StaticClass(), AllUnits);
    
    AMingTacticalUnit* ClosestEnemy = nullptr;
    float ClosestDistance = AttackRange;
    
    for (AActor* Actor : AllUnits)
    {
        AMingTacticalUnit* Unit = Cast<AMingTacticalUnit>(Actor);
        if (Unit && Unit != OwnerUnit && Unit->IsAlive())
        {
            // 檢查是否為敵人
            if (Unit->TeamId != OwnerUnit->TeamId)
            {
                float Distance = FVector::Distance(OwnerUnit->GetActorLocation(), Unit->GetActorLocation());
                if (Distance <= AttackRange && Distance < ClosestDistance)
                {
                    ClosestEnemy = Unit;
                    ClosestDistance = Distance;
                }
            }
        }
    }
    
    return ClosestEnemy;
}

void UMingUnitCombatComponent::RotateToTarget(float DeltaTime)
{
    if (!OwnerUnit)
    {
        return;
    }
    
    FVector TargetLocation;
    
    if (CurrentTarget.IsValid())
    {
        TargetLocation = CurrentTarget->GetActorLocation();
    }
    else if (bIsAttackingLocation)
    {
        TargetLocation = TargetAttackLocation;
    }
    else
    {
        return;
    }
    
    // 計算方向
    FVector Direction = TargetLocation - OwnerUnit->GetActorLocation();
    Direction.Z = 0; // 保持在地面上
    
    if (!Direction.IsNearlyZero())
    {
        // 使用移動組件的旋轉功能
        UMingUnitMovementComponent* MoveComp = OwnerUnit->GetMovementComponent();
        if (MoveComp)
        {
            MoveComp->RotateTowards(Direction, DeltaTime);
        }
    }
}

bool UMingUnitCombatComponent::IsValidTarget(AMingTacticalUnit* Target) const
{
    return Target && Target->IsAlive();
}

void UMingUnitCombatComponent::TriggerAttackAnimation()
{
    // TODO: 觸發攻擊動畫
    // 可以使用 Animation Blueprint 或播放蒙太奇
    UE_LOG(LogTemp, Verbose, TEXT("Attack animation triggered"));
}
