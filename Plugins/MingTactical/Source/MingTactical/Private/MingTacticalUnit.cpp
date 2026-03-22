#include "MingTacticalUnit.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"

// 靜態成員初始化
TArray<AMingTacticalUnit*> AMingTacticalUnit::UnitPool;
int32 AMingTacticalUnit::NextUnitId = 1;

AMingTacticalUnit::AMingTacticalUnit()
{
    PrimaryActorTick.bCanEverTick = true;
    
    // 設置默認值
    UnitId = NextUnitId++;
    UnitName = TEXT("DefaultUnit");
    MaxHealth = 100;
    CurrentHealth = MaxHealth;
    MovementSpeed = 300.0f;
    AttackDamage = 25;
    AttackRange = 200.0f;
    AttackCooldown = 1.0f;
    
    // 初始化狀態
    bIsSelected = false;
    bIsMoving = false;
    bIsAttacking = false;
    TargetPosition = FVector::ZeroVector;
    
    // 初始化性能變量
    LastAttackTime = 0.0f;
    LastUpdateTime = 0.0f;
    
    // 初始化移動相關
    NavSystem = nullptr;
    CurrentPathIndex = 0;
    CurrentTarget = nullptr;
    bHasValidTarget = false;
    
    // 創建組件
    USphereComponent* CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(50.0f);
    
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    
    // 設置碰撞
    CollisionSphere->SetCollisionProfileName(TEXT("Unit"));
    MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void AMingTacticalUnit::BeginPlay()
{
    Super::BeginPlay();
    
    // 獲取導航系統
    NavSystem = FNavigationSystem::GetCurrent<UNavigationSystem>(GetWorld());
    
    // 訂閱事件
    SubscribeToEvents();
    
    // 發布單位創建事件
    UE_LOG(LogTemp, Log, TEXT("Unit %s (ID: %d) created at position %s"), 
           *UnitName, UnitId, *GetActorLocation().ToString());
}

void AMingTacticalUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 性能優化：限制更新頻率
    LastUpdateTime += DeltaTime;
    if (LastUpdateTime < 0.016f) // 60 FPS限制
    {
        return;
    }
    
    UpdateUnitState(DeltaTime);
    LastUpdateTime = 0.0f;
}

void AMingTacticalUnit::SubscribeToEvents()
{
    // 訂閱相關事件
    IMingCoreEventBus::Subscribe<FUnitSelectedEvent>(this, &AMingTacticalUnit::OnUnitSelected);
    IMingCoreEventBus::Subscribe<FUnitMovedEvent>(this, &AMingTacticalUnit::OnUnitMoved);
}

void AMingTacticalUnit::UnsubscribeFromEvents()
{
    // 取消訂閱事件
    IMingCoreEventBus::Unsubscribe<FUnitSelectedEvent>(this);
    IMingCoreEventBus::Unsubscribe<FUnitMovedEvent>(this);
}

void AMingTacticalUnit::OnUnitSelected(const FUnitSelectedEvent& Event)
{
    if (Event.UnitId == this->UnitId)
    {
        SelectUnit();
    }
}

void AMingTacticalUnit::OnUnitMoved(const FUnitMovedEvent& Event)
{
    if (Event.UnitId == this->UnitId)
    {
        MoveToPosition(Event.TargetPosition, Event.bIsAttackMove);
    }
}

void AMingTacticalUnit::UpdateUnitState(float DeltaTime)
{
    if (!IsAlive())
    {
        return;
    }
    
    // 處理移動
    if (bIsMoving)
    {
        ProcessMovement(DeltaTime);
    }
    
    // 處理戰鬥
    if (bIsAttacking && bHasValidTarget)
    {
        ProcessCombat(DeltaTime);
    }
}

void AMingTacticalUnit::ProcessMovement(float DeltaTime)
{
    if (!NavSystem || CurrentPathIndex >= CurrentPath->GetPathPoints().Num())
    {
        bIsMoving = false;
        return;
    }
    
    const TArray<FNavPathPoint>& PathPoints = CurrentPath->GetPathPoints();
    const FNavPathPoint& CurrentTargetPoint = PathPoints[CurrentPathIndex];
    
    FVector CurrentLocation = GetActorLocation();
    FVector TargetLocation = CurrentTargetPoint.Location;
    
    // 計算移動方向
    FVector Direction = (TargetLocation - CurrentLocation);
    Direction.Z = 0; // 保持在地面上
    Direction.Normalize();
    
    // 移動單位
    FVector NewLocation = CurrentLocation + Direction * MovementSpeed * DeltaTime;
    SetActorLocation(NewLocation);
    
    // 檢查是否到達當前路徑點
    float DistanceToTarget = FVector::Dist2D(NewLocation, TargetLocation);
    if (DistanceToTarget < 50.0f) // 到達閾值
    {
        CurrentPathIndex++;
        if (CurrentPathIndex >= CurrentPath->GetPathPoints().Num())
        {
            bIsMoving = false;
            CurrentPath.Reset();
        }
    }
}

void AMingTacticalUnit::ProcessCombat(float DeltaTime)
{
    if (!bHasValidTarget || !CurrentTarget)
    {
        bIsAttacking = false;
        return;
    }
    
    // 檢查目標是否還在攻擊範圍內
    if (!IsInAttackRange(CurrentTarget))
    {
        // 移動到攻擊範圍
        MoveToPosition(CurrentTarget->GetActorLocation());
        return;
    }
    
    // 檢查攻擊冷卻
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastAttackTime >= AttackCooldown)
    {
        AttackTarget(CurrentTarget);
        LastAttackTime = CurrentTime;
    }
}

void AMingTacticalUnit::MoveToPosition(const FVector& Position, bool bIsAttackMove)
{
    if (!NavSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation system not available for unit %d"), UnitId);
        return;
    }
    
    // 計算路徑
    FNavPathSharedPtr Path;
    const FNavAgentProperties& AgentProps = GetNavAgentProperties();
    NavSystem->FindPathToLocationSynchronously(GetWorld(), GetActorLocation(), Position, nullptr, Path);
    
    if (Path.IsValid() && Path->GetPathPoints().Num() > 0)
    {
        CurrentPath = Path;
        CurrentPathIndex = 0;
        bIsMoving = true;
        TargetPosition = Position;
        
        UE_LOG(LogTemp, Log, TEXT("Unit %d moving to position %s (Path points: %d)"), 
               UnitId, *Position.ToString(), Path->GetPathPoints().Num());
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to find path for unit %d to position %s"), 
               UnitId, *Position.ToString());
    }
}

void AMingTacticalUnit::AttackTarget(AActor* Target)
{
    if (!Target || !IsInAttackRange(Target))
    {
        return;
    }
    
    // 這裡應該應用傷害到目標
    // 目前只是記錄日誌
    UE_LOG(LogTemp, Log, TEXT("Unit %d attacking target %s for %d damage"), 
           UnitId, *Target->GetName(), AttackDamage);
    
    // 發布攻擊事件（如果需要的話）
    // IMingCoreEventBus::PublishEvent(FUnitAttackEvent(UnitId, Target));
}

void AMingTacticalUnit::StopCurrentAction()
{
    bIsMoving = false;
    bIsAttacking = false;
    CurrentTarget = nullptr;
    bHasValidTarget = false;
    CurrentPath.Reset();
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d stopped all actions"), UnitId);
}

// Blueprint callable functions
void AMingTacticalUnit::SelectUnit()
{
    if (!bIsSelected)
    {
        bIsSelected = true;
        UE_LOG(LogTemp, Log, TEXT("Unit %d selected"), UnitId);
        
        // 發布選擇事件
        FUnitSelectedEvent SelectionEvent(UnitId, FVector2D::ZeroVector);
        IMingCoreEventBus::PublishEvent(SelectionEvent);
    }
}

void AMingTacticalUnit::DeselectUnit()
{
    if (bIsSelected)
    {
        bIsSelected = false;
        UE_LOG(LogTemp, Log, TEXT("Unit %d deselected"), UnitId);
    }
}

void AMingTacticalUnit::MoveToLocation(const FVector& TargetLocation, bool bAttackMove)
{
    if (IsAlive())
    {
        StopCurrentAction();
        MoveToPosition(TargetLocation, bAttackMove);
        
        // 發布移動事件
        FUnitMovedEvent MoveEvent(UnitId, TargetLocation, bAttackMove);
        IMingCoreEventBus::PublishEvent(MoveEvent);
    }
}

void AMingTacticalUnit::SetAttackTarget(AActor* Target)
{
    if (IsAlive() && Target)
    {
        CurrentTarget = Target;
        bHasValidTarget = true;
        bIsAttacking = true;
        bIsMoving = false;
        
        UE_LOG(LogTemp, Log, TEXT("Unit %d set attack target to %s"), UnitId, *Target->GetName());
    }
}

bool AMingTacticalUnit::CanAttack() const
{
    return IsAlive() && (GetWorld()->GetTimeSeconds() - LastAttackTime) >= AttackCooldown;
}

bool AMingTacticalUnit::IsInAttackRange(AActor* Target) const
{
    if (!Target)
    {
        return false;
    }
    
    float Distance = FVector::Dist(GetActorLocation(), Target->GetActorLocation());
    return Distance <= AttackRange;
}

// 靜態工廠方法
AMingTacticalUnit* AMingTacticalUnit::CreateUnit(UWorld* World, const FVector& Location)
{
    if (!World)
    {
        return nullptr;
    }
    
    // 嘗試從對象池獲取
    AMingTacticalUnit* Unit = nullptr;
    if (UnitPool.Num() > 0)
    {
        Unit = UnitPool.Pop();
        Unit->SetActorLocation(Location);
        Unit->SetActorHiddenInGame(false);
    }
    else
    {
        // 創建新單位
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        Unit = World->SpawnActor<AMingTacticalUnit>(AMingTacticalUnit::StaticClass(), Location, FRotator::ZeroRotator, SpawnParams);
    }
    
    if (Unit)
    {
        Unit->CurrentHealth = Unit->MaxHealth; // 重置生命值
        Unit->UnitId = NextUnitId++;
    }
    
    return Unit;
}

void AMingTacticalUnit::DestroyUnit(AMingTacticalUnit* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    // 取消訂閱事件
    Unit->UnsubscribeFromEvents();
    
    // 添加到對象池
    Unit->SetActorHiddenInGame(true);
    Unit->StopCurrentAction();
    UnitPool.Add(Unit);
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d returned to pool"), Unit->UnitId);
}
