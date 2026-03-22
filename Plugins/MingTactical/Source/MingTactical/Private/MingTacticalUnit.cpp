#include "MingTacticalUnit.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Materials/MaterialInterface.h"
#include "MingBuilding/Source/MingBuilding/Public/MingResourceNode.h"
#include "MingBuilding/Source/MingBuilding/Public/MingResourceSystem.h"

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
    bIsGathering = false;
    bIsCarryingResources = false;
    TargetPosition = FVector::ZeroVector;
    
    // 初始化性能變量
    LastAttackTime = 0.0f;
    LastUpdateTime = 0.0f;
    
    // 初始化移動相關
    NavSystem = nullptr;
    CurrentPathIndex = 0;
    CurrentTarget = nullptr;
    bHasValidTarget = false;
    
    // 初始化採集相關
    TargetResourceNode = nullptr;
    ResourceSystem = nullptr;
    GatheringProgress = 0.0f;
    GatheringTime = 2.0f;
    CarriedResourceAmount = 0;
    CarriedResourceType = EMingResourceType::Food;
    DeliveryLocation = FVector::ZeroVector;
    
    // 初始化選擇視覺反饋
    SelectionRingSize = 100.0f;
    
    // 創建組件
    USphereComponent* CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    RootComponent = CollisionSphere;
    CollisionSphere->SetSphereRadius(50.0f);
    
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    
    // 創建選擇環組件（默認隱藏）
    SelectionRingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionRingMesh"));
    SelectionRingMesh->SetupAttachment(RootComponent);
    SelectionRingMesh->SetVisibility(false);
    SelectionRingMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    SelectionRingMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f)); // 在單位底部
    
    // 創建選擇貼花組件（默認隱藏）
    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetVisibility(false);
    SelectionDecal->DecalSize = FVector(100.0f, 100.0f, 100.0f);
    SelectionDecal->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    
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
    
    // 處理採集
    if (bIsGathering)
    {
        ProcessGathering(DeltaTime);
    }
    
    // 處理資源運送
    if (bIsCarryingResources)
    {
        ProcessResourceDelivery(DeltaTime);
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
    bIsGathering = false;
    CurrentTarget = nullptr;
    bHasValidTarget = false;
    TargetResourceNode = nullptr;
    CurrentPath.Reset();
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d stopped all actions"), UnitId);
}

// 採集相關方法
void AMingTacticalUnit::GatherResource(AMingResourceNode* ResourceNode)
{
    if (!ResourceNode || !ResourceNode->CanBeGathered())
    {
        UE_LOG(LogTemp, Warning, TEXT("Unit %d cannot gather from resource node"), UnitId);
        return;
    }

    // 停止當前動作
    StopCurrentAction();
    
    // 設置採集目標
    TargetResourceNode = ResourceNode;
    bIsGathering = true;
    GatheringProgress = 0.0f;
    GatheringTime = ResourceNode->GatherTime;
    
    // 註冊為採集者
    ResourceNode->RegisterGatherer();
    
    // 移動到資源節點
    MoveToPosition(ResourceNode->GetActorLocation());
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d started gathering from %s"), UnitId, *ResourceNode->GetName());
}

void AMingTacticalUnit::DeliverResources()
{
    if (!bIsCarryingResources || CarriedResourceAmount <= 0)
    {
        return;
    }

    // 移動到送達位置
    if (!DeliveryLocation.IsZero())
    {
        MoveToPosition(DeliveryLocation);
        bIsMoving = true;
    }
    else
    {
        // 如果沒有設置送達位置，直接添加到資源系統
        if (ResourceSystem)
        {
            ResourceSystem->AddResource(CarriedResourceType, CarriedResourceAmount);
            UE_LOG(LogTemp, Log, TEXT("Unit %d delivered %d %s"), UnitId, 
                   CarriedResourceAmount, *UMingResourceSystem::GetResourceName(CarriedResourceType));
        }
        
        // 清除運送狀態
        bIsCarryingResources = false;
        CarriedResourceAmount = 0;
    }
}

void AMingTacticalUnit::ProcessGathering(float DeltaTime)
{
    if (!TargetResourceNode || !TargetResourceNode->CanBeGathered())
    {
        StopGathering();
        return;
    }

    // 檢查是否已到達資源節點
    float DistanceToTarget = FVector::Dist(GetActorLocation(), TargetResourceNode->GetActorLocation());
    if (DistanceToTarget > 100.0f) // 到達閾值
    {
        // 還在移動中，等待到達
        return;
    }

    // 停止移動，開始採集
    bIsMoving = false;
    
    // 更新採集進度
    GatheringProgress += DeltaTime;
    
    if (GatheringProgress >= GatheringTime)
    {
        // 採集完成
        int32 GatheredAmount = TargetResourceNode->GatherResource(TargetResourceNode->GatherAmountPerTrip);
        
        if (GatheredAmount > 0)
        {
            // 開始運送資源
            bIsGathering = false;
            bIsCarryingResources = true;
            CarriedResourceAmount = GatheredAmount;
            CarriedResourceType = TargetResourceNode->ResourceType;
            
            // 取消註冊採集者
            TargetResourceNode->UnregisterGatherer();
            
            // 設置送達位置（默認為玩家位置或基地位置）
            if (ResourceSystem)
            {
                DeliveryLocation = GetActorLocation() + FVector(0, 0, 100); // 臨時位置
            }
            
            // 開始送達
            DeliverResources();
            
            UE_LOG(LogTemp, Log, TEXT("Unit %d gathered %d %s, now delivering"), UnitId,
                   GatheredAmount, *UMingResourceSystem::GetResourceName(CarriedResourceType));
        }
        else
        {
            // 資源耗盡，停止採集
            StopGathering();
        }
    }
}

void AMingTacticalUnit::ProcessResourceDelivery(float DeltaTime)
{
    if (!bIsCarryingResources)
    {
        return;
    }

    // 檢查是否已到達送達位置
    if (!DeliveryLocation.IsZero())
    {
        float DistanceToDelivery = FVector::Dist(GetActorLocation(), DeliveryLocation);
        if (DistanceToDelivery > 100.0f) // 到達閾值
        {
            // 還在移動中
            return;
        }
    }

    // 送達完成，添加資源到系統
    if (ResourceSystem)
    {
        ResourceSystem->AddResource(CarriedResourceType, CarriedResourceAmount);
        
        UE_LOG(LogTemp, Log, TEXT("Unit %d delivered %d %s"), UnitId,
               CarriedResourceAmount, *UMingResourceSystem::GetResourceName(CarriedResourceType));
    }

    // 清除運送狀態
    bIsCarryingResources = false;
    bIsMoving = false;
    CarriedResourceAmount = 0;
    DeliveryLocation = FVector::ZeroVector;
}

// Blueprint callable functions
void AMingTacticalUnit::SelectUnit()
{
    if (!bIsSelected)
    {
        bIsSelected = true;
        UE_LOG(LogTemp, Log, TEXT("Unit %d selected"), UnitId);
        
        // 顯示選擇視覺反饋
        ShowSelectionHighlight();
        
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
        
        // 隱藏選擇視覺反饋
        HideSelectionHighlight();
    }
}

void AMingTacticalUnit::UpdateSelectionVisuals()
{
    // 更新選擇視覺效果（如閃爍、旋轉等動畫效果）
    if (bIsSelected)
    {
        // 可以在此處添加選擇環的動畫效果
        // 例如：讓選擇環緩慢旋轉或閃爍
        if (SelectionRingMesh && SelectionRingMesh->IsVisible())
        {
            // 緩慢旋轉選擇環
            FRotator CurrentRotation = SelectionRingMesh->GetRelativeRotation();
            CurrentRotation.Yaw += 0.5f; // 每幀旋轉0.5度
            SelectionRingMesh->SetRelativeRotation(CurrentRotation);
        }
    }
}

void AMingTacticalUnit::ShowSelectionHighlight()
{
    // 顯示選擇環
    if (SelectionRingMesh)
    {
        SelectionRingMesh->SetVisibility(true);
        
        // 如果設置了材質，應用它
        if (SelectionRingMaterial)
        {
            SelectionRingMesh->SetMaterial(0, SelectionRingMaterial);
        }
    }
    
    // 顯示選擇貼花
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(true);
        
        // 如果設置了材質，應用它
        if (SelectionDecalMaterial)
        {
            SelectionDecal->SetDecalMaterial(SelectionDecalMaterial);
        }
    }
    
    // 調整選擇環大小
    if (SelectionRingMesh)
    {
        SelectionRingMesh->SetWorldScale3D(FVector(SelectionRingSize / 100.0f));
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d selection highlight shown"), UnitId);
}

void AMingTacticalUnit::HideSelectionHighlight()
{
    // 隱藏選擇環
    if (SelectionRingMesh)
    {
        SelectionRingMesh->SetVisibility(false);
    }
    
    // 隱藏選擇貼花
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(false);
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d selection highlight hidden"), UnitId);
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

// 採集相關 Blueprint callable functions
void AMingTacticalUnit::StartGathering(AMingResourceNode* ResourceNode)
{
    if (IsAlive() && ResourceNode)
    {
        // 獲取資源系統引用
        if (!ResourceSystem)
        {
            // 從遊戲模式或玩家控制器獲取資源系統
            // 這裡需要根據實際項目結構來獲取
            // 暫時創建一個臨時的資源系統引用
            ResourceSystem = NewObject<UMingResourceSystem>();
            ResourceSystem->InitializeDefaultResources();
        }
        
        GatherResource(ResourceNode);
    }
}

void AMingTacticalUnit::StopGathering()
{
    if (bIsGathering && TargetResourceNode)
    {
        // 取消註冊採集者
        TargetResourceNode->UnregisterGatherer();
    }
    
    bIsGathering = false;
    TargetResourceNode = nullptr;
    GatheringProgress = 0.0f;
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d stopped gathering"), UnitId);
}

void AMingTacticalUnit::ReturnToBase()
{
    if (bIsCarryingResources)
    {
        DeliverResources();
    }
    else if (bIsGathering)
    {
        StopGathering();
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
