#include "MingRTSCombatSystem.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Math/UnrealMathUtility.h"

UMingRTSCombatSystem::UMingRTSCombatSystem()
{
    CurrentWorld = nullptr;
    CurrentTime = 0.0f;
    bCombatEnabled = true;
    GlobalDamageMultiplier = 1.0f;
    bEnableCombatEffects = true;
    bEnableCombatSounds = true;
    AIUpdateInterval = 0.1f;
    LastAIUpdateTime = 0.0f;
}

void UMingRTSCombatSystem::InitializeCombatSystem(UWorld* World)
{
    if (!World)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingRTSCombatSystem: Invalid world provided"));
        return;
    }

    CurrentWorld = World;
    CurrentTime = 0.0f;
    CombatUnits.Empty();
    EnemyRelationships.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("MingRTSCombatSystem initialized successfully"));
}

void UMingRTSCombatSystem::UpdateCombatSystem(float DeltaTime)
{
    if (!CurrentWorld || !bCombatEnabled)
    {
        return;
    }

    CurrentTime += DeltaTime;

    // 更新單位狀態
    UpdateUnitStates(DeltaTime);

    // 處理戰鬥邏輯
    ProcessCombat(DeltaTime);

    // 處理單位AI
    if (CurrentTime - LastAIUpdateTime >= AIUpdateInterval)
    {
        ProcessUnitAI(DeltaTime);
        LastAIUpdateTime = CurrentTime;
    }

    // 清理死亡單位
    CleanupDeadUnits();
    
    // 清理無效單位
    CleanupInvalidUnits();
}

void UMingRTSCombatSystem::RegisterCombatUnit(AActor* Unit, const FCombatStats& Stats)
{
    if (!Unit || !IsValidCombatUnit(Unit))
    {
        return;
    }

    FCombatUnit CombatUnit;
    CombatUnit.Unit = Unit;
    CombatUnit.Stats = Stats;
    CombatUnit.CurrentState = ECombatState::Idle;
    CombatUnit.CurrentTarget = nullptr;
    CombatUnit.LastAttackTime = 0.0f;
    CombatUnit.StateChangeTime = CurrentTime;

    CombatUnits.Add(Unit, CombatUnit);

    UE_LOG(LogTemp, Log, TEXT("Registered combat unit: %s"), *Unit->GetName());
}

void UMingRTSCombatSystem::UnregisterCombatUnit(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }

    CombatUnits.Remove(Unit);
    EnemyRelationships.Remove(Unit);

    UE_LOG(LogTemp, Log, TEXT("Unregistered combat unit: %s"), *Unit->GetName());
}

void UMingRTSCombatSystem::StartAttack(AActor* Attacker, AActor* Target, EAttackType AttackType)
{
    if (!Attacker || !Target || !CanAttack(Attacker, Target))
    {
        return;
    }

    FCombatUnit* AttackerData = GetCombatUnit(Attacker);
    if (!AttackerData)
    {
        return;
    }

    // 檢查攻擊冷卻
    float AttackCooldown = 1.0f / AttackerData->Stats.AttackSpeed;
    if (CurrentTime - AttackerData->LastAttackTime < AttackCooldown)
    {
        return;
    }

    // 創建攻擊數據
    FAttackData AttackData;
    AttackData.Attacker = Attacker;
    AttackData.Target = Target;
    AttackData.AttackType = AttackType;
    AttackData.DamageType = EDamageType::Physical; // 默認物理傷害
    AttackData.AttackLocation = Target->GetActorLocation();
    AttackData.AttackTime = CurrentTime;

    // 計算傷害
    AttackData.Damage = CalculateDamage(AttackData);
    AttackData.bIsCritical = IsCriticalHit(AttackerData->Stats);

    // 更新攻擊者狀態
    AttackerData->CurrentState = ECombatState::Attacking;
    AttackerData->CurrentTarget = Target;
    AttackerData->LastAttackTime = CurrentTime;

    // 觸發攻擊開始事件
    OnAttackStarted.Broadcast(AttackData);

    // 播放攻擊動畫
    PlayAttackAnimation(Attacker, AttackType);

    // 延遲執行攻擊（根據攻擊類型）
    float AttackDelay = 0.3f; // 默認延遲
    switch (AttackType)
    {
        case EAttackType::Melee:
            AttackDelay = 0.2f;
            break;
        case EAttackType::Ranged:
            AttackDelay = 0.5f;
            break;
        case EAttackType::Area:
            AttackDelay = 0.8f;
            break;
        case EAttackType::Special:
            AttackDelay = 1.0f;
            break;
    }

    // 延遲執行攻擊
    FTimerHandle AttackTimer;
    FTimerDelegate AttackDelegate;
    AttackDelegate.BindUObject(this, &UMingRTSCombatSystem::ExecuteAttack, AttackData);
    
    if (CurrentWorld)
    {
        CurrentWorld->GetTimerManager().SetTimer(AttackTimer, AttackDelegate, AttackDelay, false);
    }

    UE_LOG(LogTemp, Log, TEXT("Started attack: %s -> %s, Type: %d, Damage: %.1f"), 
        *Attacker->GetName(), *Target->GetName(), (int32)AttackType, AttackData.Damage);
}

void UMingRTSCombatSystem::ExecuteAttack(const FAttackData& AttackData)
{
    if (!AttackData.Attacker || !AttackData.Target)
    {
        return;
    }

    // 應用傷害
    ApplyDamage(AttackData.Target, AttackData.Damage, AttackData.DamageType, AttackData.Attacker);

    // 創建戰鬥特效
    CreateCombatEffect(AttackData);

    // 播放戰鬥音效
    PlayCombatSound(AttackData);

    // 更新攻擊者狀態
    FCombatUnit* AttackerData = GetCombatUnit(AttackData.Attacker);
    if (AttackerData)
    {
        AttackerData->CurrentState = ECombatState::Idle;
        AttackerData->CurrentTarget = nullptr;
    }

    // 觸發攻擊完成事件
    OnAttackCompleted.Broadcast(AttackData);

    UE_LOG(LogTemp, Log, TEXT("Executed attack: %s -> %s, Damage: %.1f"), 
        *AttackData.Attacker->GetName(), *AttackData.Target->GetName(), AttackData.Damage);
}

float UMingRTSCombatSystem::CalculateDamage(const FAttackData& AttackData) const
{
    if (!AttackData.Attacker)
    {
        return 0.0f;
    }

    const FCombatUnit* AttackerData = CombatUnits.Find(AttackData.Attacker);
    if (!AttackerData)
    {
        return 0.0f;
    }

    const FCombatUnit* TargetData = CombatUnits.Find(AttackData.Target);
    const FCombatStats* TargetStats = TargetData ? &TargetData->Stats : nullptr;

    // 計算基礎傷害
    float BaseDamage = CalculateBaseDamage(AttackerData->Stats, TargetStats ? *TargetStats : FCombatStats());

    // 計算傷害修正
    float DamageModifier = CalculateDamageModifier(AttackData.DamageType, TargetStats ? *TargetStats : FCombatStats());

    // 檢查暴擊
    bool bIsCritical = IsCriticalHit(AttackerData->Stats);
    float FinalDamage = BaseDamage * DamageModifier * GlobalDamageMultiplier;

    if (bIsCritical)
    {
        FinalDamage = CalculateCriticalDamage(FinalDamage, AttackerData->Stats);
    }

    return FMath::Max(0.0f, FinalDamage);
}

void UMingRTSCombatSystem::ApplyDamage(AActor* Target, float Damage, EDamageType DamageType, AActor* DamageSource)
{
    if (!Target || Damage <= 0.0f)
    {
        return;
    }

    FCombatUnit* TargetData = GetCombatUnit(Target);
    if (!TargetData)
    {
        return;
    }

    // 應用傷害
    TargetData->Stats.CurrentHealth = FMath::Max(0.0f, TargetData->Stats.CurrentHealth - Damage);

    // 應用狀態效果
    ApplyStatusEffects(Target, DamageType, Damage);

    // 觸發傷害事件
    OnUnitDamaged.Broadcast(Target, Damage);

    // 檢查是否死亡
    if (TargetData->Stats.CurrentHealth <= 0.0f)
    {
        TargetData->CurrentState = ECombatState::Dead;
        TargetData->StateChangeTime = CurrentTime;
        
        // 觸發死亡事件
        OnUnitDied.Broadcast(Target);
        
        UE_LOG(LogTemp, Log, TEXT("Unit died: %s"), *Target->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Unit damaged: %s, Damage: %.1f, Health: %.1f/%.1f"), 
            *Target->GetName(), Damage, TargetData->Stats.CurrentHealth, TargetData->Stats.MaxHealth);
    }
}

bool UMingRTSCombatSystem::IsInRange(AActor* Attacker, AActor* Target, float RangeOverride) const
{
    if (!Attacker || !Target)
    {
        return false;
    }

    const FCombatUnit* AttackerData = CombatUnits.Find(Attacker);
    if (!AttackerData)
    {
        return false;
    }

    float Range = RangeOverride > 0.0f ? RangeOverride : AttackerData->Stats.AttackRange;
    float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
    
    return Distance <= Range;
}

bool UMingRTSCombatSystem::IsInVisionRange(AActor* Observer, AActor* Target) const
{
    if (!Observer || !Target)
    {
        return false;
    }

    const FCombatUnit* ObserverData = CombatUnits.Find(Observer);
    if (!ObserverData)
    {
        return false;
    }

    float Distance = FVector::Dist(Observer->GetActorLocation(), Target->GetActorLocation());
    return Distance <= ObserverData->Stats.VisionRange;
}

AActor* UMingRTSCombatSystem::GetNearestEnemy(AActor* Unit, float SearchRange) const
{
    if (!Unit)
    {
        return nullptr;
    }

    const FCombatUnit* UnitData = CombatUnits.Find(Unit);
    if (!UnitData)
    {
        return nullptr;
    }

    float VisionRange = SearchRange > 0.0f ? SearchRange : UnitData->Stats.VisionRange;
    AActor* NearestEnemy = nullptr;
    float NearestDistance = VisionRange;

    for (const TPair<TObjectPtr<AActor>, FCombatUnit>& CombatUnitPair : CombatUnits)
    {
        AActor* OtherUnit = CombatUnitPair.Key;
        const FCombatUnit& OtherData = CombatUnitPair.Value;

        if (OtherUnit == Unit || !IsUnitAlive(OtherUnit))
        {
            continue;
        }

        if (!IsEnemy(Unit, OtherUnit))
        {
            continue;
        }

        float Distance = FVector::Dist(Unit->GetActorLocation(), OtherUnit->GetActorLocation());
        if (Distance < NearestDistance)
        {
            NearestDistance = Distance;
            NearestEnemy = OtherUnit;
        }
    }

    return NearestEnemy;
}

TArray<AActor*> UMingRTSCombatSystem::GetEnemiesInRange(AActor* Unit, float Range) const
{
    TArray<AActor*> EnemiesInRange;

    if (!Unit)
    {
        return EnemiesInRange;
    }

    for (const TPair<TObjectPtr<AActor>, FCombatUnit>& CombatUnitPair : CombatUnits)
    {
        AActor* OtherUnit = CombatUnitPair.Key;
        const FCombatUnit& OtherData = CombatUnitPair.Value;

        if (OtherUnit == Unit || !IsUnitAlive(OtherUnit))
        {
            continue;
        }

        if (!IsEnemy(Unit, OtherUnit))
        {
            continue;
        }

        float Distance = FVector::Dist(Unit->GetActorLocation(), OtherUnit->GetActorLocation());
        if (Distance <= Range)
        {
            EnemiesInRange.Add(OtherUnit);
        }
    }

    return EnemiesInRange;
}

void UMingRTSCombatSystem::SetUnitState(AActor* Unit, ECombatState NewState)
{
    FCombatUnit* UnitData = GetCombatUnit(Unit);
    if (UnitData)
    {
        UnitData->CurrentState = NewState;
        UnitData->StateChangeTime = CurrentTime;
    }
}

ECombatState UMingRTSCombatSystem::GetUnitState(AActor* Unit) const
{
    const FCombatUnit* UnitData = CombatUnits.Find(Unit);
    if (UnitData)
    {
        return UnitData->CurrentState;
    }
    return ECombatState::Idle;
}

FCombatStats UMingRTSCombatSystem::GetUnitStats(AActor* Unit) const
{
    const FCombatUnit* UnitData = CombatUnits.Find(Unit);
    if (UnitData)
    {
        return UnitData->Stats;
    }
    return FCombatStats();
}

void UMingRTSCombatSystem::UpdateUnitStats(AActor* Unit, const FCombatStats& NewStats)
{
    FCombatUnit* UnitData = GetCombatUnit(Unit);
    if (UnitData)
    {
        UnitData->Stats = NewStats;
    }
}

void UMingRTSCombatSystem::HealUnit(AActor* Unit, float HealAmount)
{
    FCombatUnit* UnitData = GetCombatUnit(Unit);
    if (UnitData && IsUnitAlive(Unit))
    {
        UnitData->Stats.CurrentHealth = FMath::Min(UnitData->Stats.MaxHealth, UnitData->Stats.CurrentHealth + HealAmount);
        
        UE_LOG(LogTemp, Log, TEXT("Unit healed: %s, Heal: %.1f, Health: %.1f/%.1f"), 
            *Unit->GetName(), HealAmount, UnitData->Stats.CurrentHealth, UnitData->Stats.MaxHealth);
    }
}

void UMingRTSCombatSystem::ReviveUnit(AActor* Unit, float HealthPercentage)
{
    FCombatUnit* UnitData = GetCombatUnit(Unit);
    if (UnitData && UnitData->CurrentState == ECombatState::Dead)
    {
        UnitData->Stats.CurrentHealth = UnitData->Stats.MaxHealth * FMath::Clamp(HealthPercentage, 0.0f, 1.0f);
        UnitData->CurrentState = ECombatState::Idle;
        UnitData->StateChangeTime = CurrentTime;
        
        UE_LOG(LogTemp, Log, TEXT("Unit revived: %s, Health: %.1f/%.1f"), 
            *Unit->GetName(), UnitData->Stats.CurrentHealth, UnitData->Stats.MaxHealth);
    }
}

bool UMingRTSCombatSystem::IsUnitAlive(AActor* Unit) const
{
    const FCombatUnit* UnitData = CombatUnits.Find(Unit);
    if (UnitData)
    {
        return UnitData->CurrentState != ECombatState::Dead && UnitData->Stats.CurrentHealth > 0.0f;
    }
    return false;
}

int32 UMingRTSCombatSystem::GetAliveUnitCount() const
{
    int32 Count = 0;
    for (const TPair<TObjectPtr<AActor>, FCombatUnit>& CombatUnitPair : CombatUnits)
    {
        if (IsUnitAlive(CombatUnitPair.Key))
        {
            Count++;
        }
    }
    return Count;
}

int32 UMingRTSCombatSystem::GetTotalUnitCount() const
{
    return CombatUnits.Num();
}

void UMingRTSCombatSystem::SetUnitRelationship(AActor* Unit, AActor* Target, bool bIsEnemy)
{
    if (!Unit || !Target)
    {
        return;
    }

    TArray<TObjectPtr<AActor>>& Enemies = EnemyRelationships.FindOrAdd(Unit);
    
    if (bIsEnemy)
    {
        if (!Enemies.Contains(Target))
        {
            Enemies.Add(Target);
        }
    }
    else
    {
        Enemies.Remove(Target);
    }
}

bool UMingRTSCombatSystem::IsEnemy(AActor* Unit, AActor* Target) const
{
    if (!Unit || !Target)
    {
        return false;
    }

    const TArray<TObjectPtr<AActor>>* Enemies = EnemyRelationships.Find(Unit);
    if (Enemies)
    {
        return Enemies->Contains(Target);
    }

    return false;
}

void UMingRTSCombatSystem::ProcessUnitAI(float DeltaTime)
{
    for (TPair<TObjectPtr<AActor>, FCombatUnit>& CombatUnitPair : CombatUnits)
    {
        AActor* Unit = CombatUnitPair.Key;
        FCombatUnit& UnitData = CombatUnitPair.Value;

        if (!IsUnitAlive(Unit))
        {
            continue;
        }

        // 根據狀態處理AI
        switch (UnitData.CurrentState)
        {
            case ECombatState::Idle:
                // 尋找最近的敵人
                AActor* NearestEnemy = GetNearestEnemy(Unit);
                if (NearestEnemy && IsInRange(Unit, NearestEnemy))
                {
                    StartAttack(Unit, NearestEnemy);
                }
                break;

            case ECombatState::Attacking:
                // 檢查目標是否仍然有效
                if (UnitData.CurrentTarget && (!IsUnitAlive(UnitData.CurrentTarget) || !IsInRange(Unit, UnitData.CurrentTarget)))
                {
                    UnitData.CurrentState = ECombatState::Idle;
                    UnitData.CurrentTarget = nullptr;
                }
                break;

            default:
                break;
        }
    }
}

void UMingRTSCombatSystem::ProcessCombat(float DeltaTime)
{
    // 戰鬥邏輯處理
    // 大部分戰鬥邏輯在 ProcessUnitAI 和 ExecuteAttack 中處理
}

bool UMingRTSCombatSystem::CanAttack(AActor* Attacker, AActor* Target) const
{
    if (!Attacker || !Target)
    {
        return false;
    }

    if (!IsUnitAlive(Attacker) || !IsUnitAlive(Target))
    {
        return false;
    }

    if (!IsEnemy(Attacker, Target))
    {
        return false;
    }

    if (!IsInRange(Attacker, Target))
    {
        return false;
    }

    return true;
}

void UMingRTSCombatSystem::PlayAttackAnimation(AActor* Attacker, EAttackType AttackType)
{
    // 這裡可以添加攻擊動畫播放邏輯
    // 根據攻擊類型播放不同的動畫
    UE_LOG(LogTemp, Log, TEXT("Playing attack animation for %s, Type: %d"), 
        *Attacker->GetName(), (int32)AttackType);
}

void UMingRTSCombatSystem::CreateCombatEffect(const FAttackData& AttackData)
{
    if (!bEnableCombatEffects)
    {
        return;
    }

    // 這裡可以添加戰鬥特效創建邏輯
    // 例如爆炸、血濺、魔法效果等
    
    #if WITH_EDITOR
    if (CurrentWorld)
    {
        // 在編輯器中繪製調試特效
        DrawDebugSphere(CurrentWorld, AttackData.AttackLocation, 50.0f, 16, 
            FColor::Red, false, 1.0f);
    }
    #endif

    UE_LOG(LogTemp, Log, TEXT("Created combat effect at %s"), *AttackData.AttackLocation.ToString());
}

void UMingRTSCombatSystem::PlayCombatSound(const FAttackData& AttackData)
{
    if (!bEnableCombatSounds)
    {
        return;
    }

    // 這裡可以添加戰鬥音效播放邏輯
    // 根據攻擊類型和傷害類型播放不同的音效
    
    UE_LOG(LogTemp, Log, TEXT("Playing combat sound for attack: %s -> %s"), 
        *AttackData.Attacker->GetName(), *AttackData.Target->GetName());
}

void UMingRTSCombatSystem::UpdateUnitStates(float DeltaTime)
{
    for (TPair<TObjectPtr<AActor>, FCombatUnit>& CombatUnitPair : CombatUnits)
    {
        AActor* Unit = CombatUnitPair.Key;
        FCombatUnit& UnitData = CombatUnitPair.Value;

        // 根據狀態更新單位
        switch (UnitData.CurrentState)
        {
            case ECombatState::Dead:
                // 死亡狀態處理
                break;

            case ECombatState::Attacking:
                // 攻擊狀態處理
                break;

            default:
                break;
        }
    }
}

void UMingRTSCombatSystem::CleanupDeadUnits()
{
    // 清理死亡單位（可選，根據遊戲需求決定是否立即移除）
    // 這裡只是記錄死亡單位，實際移除可能需要其他邏輯
}

float UMingRTSCombatSystem::CalculateBaseDamage(const FCombatStats& AttackerStats, const FCombatStats& TargetStats) const
{
    // 基礎傷害 = 攻擊力 - 防禦力
    float BaseDamage = AttackerStats.AttackPower - TargetStats.DefensePower;
    return FMath::Max(1.0f, BaseDamage); // 最少1點傷害
}

float UMingRTSCombatSystem::CalculateDamageModifier(EDamageType DamageType, const FCombatStats& TargetStats) const
{
    // 根據傷害類型計算傷害修正
    // 這裡可以添加元素抗性、弱點等機制
    float Modifier = 1.0f;
    
    switch (DamageType)
    {
        case EDamageType::Physical:
            Modifier = 1.0f;
            break;
        case EDamageType::Fire:
            Modifier = 1.2f; // 火焰傷害加成
            break;
        case EDamageType::Ice:
            Modifier = 0.8f; // 冰霜傷害減免
            break;
        case EDamageType::Lightning:
            Modifier = 1.1f;
            break;
        case EDamageType::Poison:
            Modifier = 0.9f;
            break;
        case EDamageType::Holy:
            Modifier = 1.3f;
            break;
        case EDamageType::Dark:
            Modifier = 1.15f;
            break;
    }

    return Modifier;
}

bool UMingRTSCombatSystem::IsCriticalHit(const FCombatStats& AttackerStats) const
{
    return FMath::RandRange(0.0f, 1.0f) < AttackerStats.CriticalChance;
}

float UMingRTSCombatSystem::CalculateCriticalDamage(float BaseDamage, const FCombatStats& AttackerStats) const
{
    return BaseDamage * AttackerStats.CriticalMultiplier;
}

void UMingRTSCombatSystem::ApplyStatusEffects(AActor* Target, EDamageType DamageType, float Damage)
{
    // 這裡可以添加狀態效果應用邏輯
    // 例如中毒、燃燒、冰凍等效果
    
    switch (DamageType)
    {
        case EDamageType::Poison:
            // 中毒效果
            break;
        case EDamageType::Fire:
            // 燃燒效果
            break;
        case EDamageType::Ice:
            // 冰凍效果
            break;
        default:
            break;
    }
}

FCombatUnit* UMingRTSCombatSystem::GetCombatUnit(AActor* Unit)
{
    return CombatUnits.Find(Unit);
}

bool UMingRTSCombatSystem::IsValidCombatUnit(AActor* Unit) const
{
    if (!Unit || Unit->IsPendingKill())
    {
        return false;
    }
    
    return CombatUnits.Contains(Unit);
}

void UMingRTSCombatSystem::CleanupInvalidUnits()
{
    // 清理無效單位
    for (auto It = CombatUnits.CreateIterator(); It; ++It)
    {
        if (!IsValidCombatUnit(It->Key))
        {
            It.RemoveCurrent();
        }
    }

    // 清理敵對關係中的無效單位
    for (auto It = EnemyRelationships.CreateIterator(); It; ++It)
    {
        if (!IsValidCombatUnit(It->Key))
        {
            It.RemoveCurrent();
        }
        else
        {
            TArray<TObjectPtr<AActor>>& Enemies = It->Value;
            for (int32 i = Enemies.Num() - 1; i >= 0; --i)
            {
                if (!IsValidCombatUnit(Enemies[i]))
                {
                    Enemies.RemoveAt(i);
                }
            }
        }
    }
}

int32 UMingRTSCombatSystem::GetUnitTeam(AActor* Unit) const
{
    // 這裡可以添加隊伍獲取邏輯
    // 簡化實現，返回0表示無隊伍
    return 0;
}

void UMingRTSCombatSystem::SetUnitTeam(AActor* Unit, int32 TeamID)
{
    // 這裡可以添加隊伍設置邏輯
    // 可以用於自動設置敵對關係
}
