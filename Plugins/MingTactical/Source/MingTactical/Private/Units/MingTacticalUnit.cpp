#include "Units/MingTacticalUnit.h"
#include "Components/StaticMeshComponent.h"
#include "MingCoreEventBus.h"
#include "MingTacticalManager.h"

// 靜態計數器初始化
int32 AMingTacticalUnit::GlobalUnitCounter = 0;

AMingTacticalUnit::AMingTacticalUnit()
{
    // 設置Tick
    PrimaryActorTick.bCanEverTick = true;
    
    // 創建根組件
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    
    // 創建網格組件
    UnitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMesh"));
    UnitMesh->SetupAttachment(RootComponent);
    
    // 創建移動組件
    MovementComponent = CreateDefaultSubobject<UMingUnitMovementComponent>(TEXT("MovementComponent"));
    
    // 創建戰鬥組件
    CombatComponent = CreateDefaultSubobject<UMingUnitCombatComponent>(TEXT("CombatComponent"));
    
    // 初始化屬性
    UnitId = -1;
    TeamId = 0;
    UnitType = EUnitType::Infantry;
    bIsSelected = false;
    CurrentState = EUnitState::Idle;
    
    // 設置碰撞
    SetActorEnableCollision(true);
}

void AMingTacticalUnit::BeginPlay()
{
    Super::BeginPlay();
    
    // 分配唯一ID
    UnitId = ++GlobalUnitCounter;
    
    // 確保生命值正確
    UnitStats.CurrentHealth = UnitStats.MaxHealth;
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d spawned (Type: %s, Team: %d)"),
        UnitId,
        *UEnum::GetValueAsString(UnitType),
        TeamId);
}

void AMingTacticalUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 更新選擇視覺效果
    if (bIsSelected)
    {
        UpdateSelectionVisuals();
    }
}

void AMingTacticalUnit::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // 如果單位還活著，發布死亡事件
    if (IsAlive())
    {
        PublishUnitDeathEvent();
    }
    
    Super::EndPlay(EndPlayReason);
}

void AMingTacticalUnit::InitializeUnit(EUnitType InUnitType, int32 InTeamId)
{
    UnitType = InUnitType;
    TeamId = InTeamId;
    
    // 根據單位類型調整屬性
    switch (UnitType)
    {
    case EUnitType::Infantry:
        UnitStats.MaxHealth = 100.0f;
        UnitStats.MoveSpeed = 300.0f;
        UnitStats.AttackDamage = 20.0f;
        UnitStats.AttackRange = 100.0f;
        break;
        
    case EUnitType::Cavalry:
        UnitStats.MaxHealth = 150.0f;
        UnitStats.MoveSpeed = 500.0f;
        UnitStats.AttackDamage = 35.0f;
        UnitStats.AttackRange = 80.0f;
        break;
        
    case EUnitType::Artillery:
        UnitStats.MaxHealth = 80.0f;
        UnitStats.MoveSpeed = 150.0f;
        UnitStats.AttackDamage = 80.0f;
        UnitStats.AttackRange = 500.0f;
        break;
        
    case EUnitType::Support:
        UnitStats.MaxHealth = 120.0f;
        UnitStats.MoveSpeed = 250.0f;
        UnitStats.AttackDamage = 10.0f;
        UnitStats.AttackRange = 150.0f;
        break;
        
    case EUnitType::Commander:
        UnitStats.MaxHealth = 200.0f;
        UnitStats.MoveSpeed = 350.0f;
        UnitStats.AttackDamage = 40.0f;
        UnitStats.AttackRange = 120.0f;
        break;
    }
    
    UnitStats.CurrentHealth = UnitStats.MaxHealth;
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d initialized as %s"),
        UnitId, *UEnum::GetValueAsString(UnitType));
}

void AMingTacticalUnit::SetSelected(bool bSelected)
{
    if (bIsSelected != bSelected)
    {
        bIsSelected = bSelected;
        UpdateSelectionVisuals();
        
        // 發布選擇事件
        if (bIsSelected)
        {
            PublishUnitSelectedEvent();
        }
        
        UE_LOG(LogTemp, Verbose, TEXT("Unit %d selection: %s"),
            UnitId, bIsSelected ? TEXT("Selected") : TEXT("Deselected"));
    }
}

void AMingTacticalUnit::TakeDamage(float DamageAmount, AActor* DamageCauser)
{
    if (!IsAlive() || DamageAmount <= 0)
    {
        return;
    }
    
    // 計算實際傷害 (考慮防禦)
    float ActualDamage = FMath::Max(1.0f, DamageAmount - UnitStats.Defense);
    UnitStats.CurrentHealth -= ActualDamage;
    
    // 切換到受傷狀態
    ChangeState(EUnitState::TakingDamage);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d took %f damage (Health: %f/%f)"),
        UnitId, ActualDamage, UnitStats.CurrentHealth, UnitStats.MaxHealth);
    
    // 檢查死亡
    if (UnitStats.CurrentHealth <= 0)
    {
        UnitStats.CurrentHealth = 0;
        OnDeath();
    }
}

void AMingTacticalUnit::Heal(float HealAmount)
{
    if (!IsAlive() || HealAmount <= 0)
    {
        return;
    }
    
    UnitStats.CurrentHealth = FMath::Min(UnitStats.MaxHealth, UnitStats.CurrentHealth + HealAmount);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d healed %f (Health: %f/%f)"),
        UnitId, HealAmount, UnitStats.CurrentHealth, UnitStats.MaxHealth);
}

float AMingTacticalUnit::GetHealthPercent() const
{
    if (UnitStats.MaxHealth <= 0)
    {
        return 0.0f;
    }
    return UnitStats.CurrentHealth / UnitStats.MaxHealth;
}

void AMingTacticalUnit::ChangeState(EUnitState NewState)
{
    if (CurrentState != NewState)
    {
        EUnitState OldState = CurrentState;
        CurrentState = NewState;
        
        UE_LOG(LogTemp, Verbose, TEXT("Unit %d state: %s -> %s"),
            UnitId,
            *UEnum::GetValueAsString(OldState),
            *UEnum::GetValueAsString(NewState));
        
        // TODO: 觸發狀態變更事件
    }
}

void AMingTacticalUnit::UpdateSelectionVisuals()
{
    // TODO: 實現選擇環渲染或高亮效果
    // 可以通過動態材質或附加一個選擇環網格來實現
}

void AMingTacticalUnit::OnDeath()
{
    ChangeState(EUnitState::Dead);
    
    // 發布死亡事件
    PublishUnitDeathEvent();
    
    UE_LOG(LogTemp, Log, TEXT("Unit %d died"), UnitId);
    
    // TODO: 播放死亡動畫
    // TODO: 延遲銷毀或進入屍體狀態
    
    // 暫時立即銷毀
    Destroy();
}

void AMingTacticalUnit::PublishUnitSelectedEvent()
{
    // 使用事件總線發布單位選擇事件
    FUnitSelectedEvent Event(UnitId, FVector2D::ZeroVector);
    IMingCoreEventBus::PublishEvent(Event);
}

void AMingTacticalUnit::PublishUnitDeathEvent()
{
    // 可以定義一個單位死亡事件
    // FUnitDeathEvent Event(UnitId, TeamId);
    // IMingCoreEventBus::PublishEvent(Event);
    
    UE_LOG(LogTemp, Verbose, TEXT("Unit %d death event published"), UnitId);
}
