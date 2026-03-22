#include "MingRTSCombatSystem.h"
#include "MingRTSUnitManager.h"
#include "MingGoRTSUnit.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/Engine.h"

UMingRTSCombatSystem::UMingRTSCombatSystem()
    : BaseDamageMultiplier(1.0f)
    , BaseDefenseMultiplier(1.0f)
    , CriticalHitBonus(1.5f)
    , bEnableCriticalHits(true)
    , bEnableDodging(true)
    , bEnableBlocking(true)
    , DifficultyMultiplier(1.0f)
    , TotalDamageDealt(0)
    , UnitsKilled(0)
    , bIsInitialized(false)
{
    // 初始化單位類型傷害修正
    UnitTypeDamageModifiers.Add(ERTSUnitType::Infantry, 1.0f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Cavalry, 1.2f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Artillery, 1.5f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Scout, 0.8f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Engineer, 0.9f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Medic, 0.6f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Commander, 1.3f);
    UnitTypeDamageModifiers.Add(ERTSUnitType::Supply, 0.5f);

    // 初始化單位類型防禦修正
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Infantry, 1.0f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Cavalry, 0.9f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Artillery, 0.7f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Scout, 1.1f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Engineer, 1.2f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Medic, 1.3f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Commander, 1.4f);
    UnitTypeDefenseModifiers.Add(ERTSUnitType::Supply, 1.1f);

    // 初始化傷害類型效果
    DamageTypeEffectiveness.Add(ERTSDamageType::Physical, 1.0f);
    DamageTypeEffectiveness.Add(ERTSDamageType::Fire, 1.2f);
    DamageTypeEffectiveness.Add(ERTSDamageType::Explosive, 1.5f);
    DamageTypeEffectiveness.Add(ERTSDamageType::Piercing, 1.1f);
    DamageTypeEffectiveness.Add(ERTSDamageType::Magic, 1.3f);
    DamageTypeEffectiveness.Add(ERTSDamageType::Poison, 0.8f);
}

void UMingRTSCombatSystem::InitializeCombatSystem(UMingRTSUnitManager* InUnitManager)
{
    UnitManager = InUnitManager;
    
    if (UnitManager)
    {
        bIsInitialized = true;
        UE_LOG(LogTemp, Log, TEXT("RTS Combat System initialized"));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("RTS Combat System initialization failed - UnitManager is null"));
    }
}

void UMingRTSCombatSystem::SetUnitCombatStats(AMingGoRTSUnit* Unit, const FRTSCombatStats& CombatStats)
{
    if (!Unit || !bIsInitialized)
    {
        return;
    }

    UnitCombatStats.Add(Unit, CombatStats);
    
    UE_LOG(LogTemp, Log, TEXT("Set combat stats for unit"));
}

FRTSCombatStats UMingRTSCombatSystem::GetUnitCombatStats(AMingGoRTSUnit* Unit) const
{
    if (UnitCombatStats.Contains(Unit))
    {
        return UnitCombatStats[Unit];
    }
    
    // 返回默認戰鬥統計
    return FRTSCombatStats();
}

void UMingRTSCombatSystem::UpdateCombatStats(AMingGoRTSUnit* Unit, float DamageMultiplier, float DefenseMultiplier)
{
    if (!Unit)
    {
        return;
    }

    FRTSCombatStats Stats = GetUnitCombatStats(Unit);
    Stats.AttackDamage *= DamageMultiplier;
    Stats.DefenseRating *= DefenseMultiplier;
    
    UnitCombatStats.Add(Unit, Stats);
    
    UE_LOG(LogTemp, Log, TEXT("Updated combat stats for unit"));
}

FRTSCombatResult UMingRTSCombatSystem::ExecuteCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    FRTSCombatResult Result;
    
    if (!Attacker || !Target || !CanAttack(Attacker, Target))
    {
        Result.ResultMessage = TEXT("Invalid combat parameters");
        return Result;
    }

    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    FRTSCombatStats TargetStats = GetUnitCombatStats(Target);
    
    // 檢查是否命中
    Result.bHit = CheckHit(Attacker, Target);
    
    if (Result.bHit)
    {
        // 檢查閃避
        Result.bDodged = CheckDodge(Target);
        
        if (!Result.bDodged)
        {
            // 檢查格擋
            Result.bBlocked = CheckBlock(Target);
            
            // 計算傷害
            Result.DamageDealt = CalculateDamage(Attacker, Target);
            
            if (Result.bBlocked)
            {
                Result.DamageBlocked = Result.DamageDealt * 0.5f; // 格擋減少50%傷害
                Result.DamageDealt -= Result.DamageBlocked;
            }
            
            // 檢查暴擊
            Result.bCritical = CheckCritical(Attacker);
            if (Result.bCritical)
            {
                Result.DamageDealt *= CriticalHitBonus;
            }
            
            Result.DamageType = AttackerStats.DamageType;
            Result.ResultMessage = GenerateCombatResultMessage(Result);
            
            // 應用傷害
            ApplyCombatResult(Attacker, Target, Result);
            
            NotifyCombatHit(Attacker, Target, Result);
        }
        else
        {
            Result.ResultMessage = TEXT("Target dodged the attack");
            NotifyCombatMissed(Attacker, Target);
        }
    }
    else
    {
        Result.ResultMessage = TEXT("Attack missed");
        NotifyCombatMissed(Attacker, Target);
    }

    return Result;
}

bool UMingRTSCombatSystem::CanAttack(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    if (!Attacker || !Target || Attacker == Target)
    {
        return false;
    }

    if (!UnitManager)
    {
        return false;
    }

    // 檢查距離
    float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    
    return Distance <= AttackerStats.AttackRange;
}

void UMingRTSCombatSystem::StartCombat(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    if (!Attacker || !Target || !CanAttack(Attacker, Target))
    {
        return;
    }

    // 添加到戰鬥對
    CombatPairs.Add(Attacker, Target);
    
    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    
    NotifyCombatStarted(Attacker, Target, AttackerStats.CombatType, AttackerStats.DamageType);
    
    UE_LOG(LogTemp, Log, TEXT("Started combat between units"));
}

void UMingRTSCombatSystem::StopCombat(AMingGoRTSUnit* Unit)
{
    if (!Unit)
    {
        return;
    }

    // 從戰鬥對中移除
    CombatPairs.Remove(Unit);
    
    // 檢查是否為目標
    TArray<AMingGoRTSUnit*> AttackersToRemove;
    for (auto& CombatPair : CombatPairs)
    {
        if (CombatPair.Value == Unit)
        {
            AttackersToRemove.Add(CombatPair.Key);
        }
    }
    
    for (AMingGoRTSUnit* Attacker : AttackersToRemove)
    {
        CombatPairs.Remove(Attacker);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Stopped combat for unit"));
}

bool UMingRTSCombatSystem::IsInCombat(AMingGoRTSUnit* Unit) const
{
    return CombatPairs.Contains(Unit);
}

float UMingRTSCombatSystem::CalculateDamage(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    if (!Attacker || !Target)
    {
        return 0.0f;
    }

    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    FRTSCombatStats TargetStats = GetUnitCombatStats(Target);
    
    // 基礎傷害計算
    float BaseDamage = AttackerStats.AttackDamage * BaseDamageMultiplier;
    
    // 應用姿態加成
    ERTSCombatStance AttackerStance = GetCombatStance(Attacker);
    float StanceMultiplier = GetStanceDamageMultiplier(AttackerStance);
    BaseDamage *= StanceMultiplier;
    
    // 應用傷害類型加成
    float TypeMultiplier = GetDamageTypeMultiplier(AttackerStats.DamageType, TargetStats.DamageType);
    BaseDamage *= TypeMultiplier;
    
    // 應用防禦減免
    float FinalDamage = ApplyDamageReduction(BaseDamage, AttackerStats.DamageType, TargetStats.DefenseRating);
    
    return FMath::Max(0.0f, FinalDamage);
}

float UMingRTSCombatSystem::ApplyDamageReduction(float BaseDamage, ERTSDamageType DamageType, float DefenseRating)
{
    float DefenseMultiplier = BaseDefenseMultiplier;
    float DefenseValue = DefenseRating * DefenseMultiplier;
    
    // 根據傷害類型調整防禦效果
    switch (DamageType)
    {
        case ERTSDamageType::Physical:
            // 物理傷害受防禦影響最大
            DefenseValue *= 1.0f;
            break;
        case ERTSDamageType::Fire:
        case ERTSDamageType::Explosive:
            // 火焰和爆炸傷害防禦效果較低
            DefenseValue *= 0.5f;
            break;
        case ERTSDamageType::Piercing:
            // 穿刺傷害部分忽略防禦
            DefenseValue *= 0.7f;
            break;
        case ERTSDamageType::Magic:
        case ERTSDamageType::Poison:
            // 魔法和毒素傷害防禦效果最低
            DefenseValue *= 0.3f;
            break;
    }
    
    float DamageReduction = DefenseValue / (DefenseValue + 100.0f);
    return BaseDamage * (1.0f - DamageReduction);
}

bool UMingRTSCombatSystem::CheckHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    if (!Attacker || !Target)
    {
        return false;
    }

    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    
    // 基礎命中率
    float HitChance = AttackerStats.Accuracy;
    
    // 距離影響命中率
    float Distance = FVector::Dist(Attacker->GetActorLocation(), Target->GetActorLocation());
    float DistancePenalty = Distance / AttackerStats.AttackRange;
    HitChance -= DistancePenalty * 0.2f;
    
    // 隨機判定
    float RandomRoll = FMath::FRandRange(0.0f, 1.0f);
    return RandomRoll <= HitChance;
}

bool UMingRTSCombatSystem::CheckCritical(AMingGoRTSUnit* Attacker)
{
    if (!Attacker || !bEnableCriticalHits)
    {
        return false;
    }

    FRTSCombatStats AttackerStats = GetUnitCombatStats(Attacker);
    float RandomRoll = FMath::FRandRange(0.0f, 1.0f);
    
    return RandomRoll <= AttackerStats.CriticalChance;
}

bool UMingRTSCombatSystem::CheckDodge(AMingGoRTSUnit* Target)
{
    if (!Target || !bEnableDodging)
    {
        return false;
    }

    FRTSCombatStats TargetStats = GetUnitCombatStats(Target);
    float RandomRoll = FMath::FRandRange(0.0f, 1.0f);
    
    return RandomRoll <= TargetStats.DodgeChance;
}

bool UMingRTSCombatSystem::CheckBlock(AMingGoRTSUnit* Target)
{
    if (!Target || !bEnableBlocking)
    {
        return false;
    }

    FRTSCombatStats TargetStats = GetUnitCombatStats(Target);
    float RandomRoll = FMath::FRandRange(0.0f, 1.0f);
    
    return RandomRoll <= TargetStats.BlockChance;
}

void UMingRTSCombatSystem::SetCombatStance(AMingGoRTSUnit* Unit, ERTSCombatStance Stance)
{
    if (!Unit)
    {
        return;
    }

    UnitCombatStances.Add(Unit, Stance);
    
    UE_LOG(LogTemp, Log, TEXT("Set combat stance: %d"), (int32)Stance);
}

ERTSCombatStance UMingRTSCombatSystem::GetCombatStance(AMingGoRTSUnit* Unit) const
{
    if (UnitCombatStances.Contains(Unit))
    {
        return UnitCombatStances[Unit];
    }
    
    return ERTSCombatStance::Neutral;
}

float UMingRTSCombatSystem::GetStanceDamageMultiplier(ERTSCombatStance Stance) const
{
    switch (Stance)
    {
        case ERTSCombatStance::Aggressive:
            return 1.2f; // 攻擊姿態增加20%傷害
        case ERTSCombatStance::Defensive:
            return 0.8f; // 防禦姿態減少20%傷害
        case ERTSCombatStance::Neutral:
            return 1.0f; // 中立姿態無加成
        case ERTSCombatStance::Passive:
            return 0.6f; // 被動姿態減少40%傷害
        default:
            return 1.0f;
    }
}

float UMingRTSCombatSystem::GetStanceDefenseMultiplier(ERTSCombatStance Stance) const
{
    switch (Stance)
    {
        case ERTSCombatStance::Aggressive:
            return 0.8f; // 攻擊姿態減少20%防禦
        case ERTSCombatStance::Defensive:
            return 1.3f; // 防禦姿態增加30%防禦
        case ERTSCombatStance::Neutral:
            return 1.0f; // 中立姿態無加成
        case ERTSCombatStance::Passive:
            return 1.1f; // 被動姿態增加10%防禦
        default:
            return 1.0f;
    }
}

TArray<AMingGoRTSUnit*> UMingRTSCombatSystem::GetUnitsInArea(const FVector& Center, float Radius, AMingGoRTSUnit* ExcludedUnit)
{
    TArray<AMingGoRTSUnit*> UnitsInArea;
    
    if (!UnitManager)
    {
        return UnitsInArea;
    }

    TArray<AMingGoRTSUnit*> AllUnits = UnitManager->GetAllUnits();
    
    for (AMingGoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != ExcludedUnit)
        {
            float Distance = FVector::Dist(Center, Unit->GetActorLocation());
            if (Distance <= Radius)
            {
                UnitsInArea.Add(Unit);
            }
        }
    }
    
    return UnitsInArea;
}

void UMingRTSCombatSystem::ExecuteAreaAttack(AMingGoRTSUnit* Attacker, const FVector& Center, float Radius)
{
    if (!Attacker)
    {
        return;
    }

    TArray<AMingGoRTSUnit*> UnitsInArea = GetUnitsInArea(Center, Radius, Attacker);
    
    for (AMingGoRTSUnit* Target : UnitsInArea)
    {
        ExecuteCombat(Attacker, Target);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executed area attack affecting %d units"), UnitsInArea.Num());
}

void UMingRTSCombatSystem::ExecuteLineAttack(AMingGoRTSUnit* Attacker, const FVector& Start, const FVector& End, float Width)
{
    if (!Attacker)
    {
        return;
    }

    // 簡化的線性攻擊實現
    FVector Direction = (End - Start).GetSafeNormal();
    float Distance = FVector::Dist(Start, End);
    
    TArray<AMingGoRTSUnit*> AllUnits = UnitManager->GetAllUnits();
    
    for (AMingGoRTSUnit* Unit : AllUnits)
    {
        if (Unit && Unit != Attacker)
        {
            FVector UnitLocation = Unit->GetActorLocation();
            FVector ToUnit = UnitLocation - Start;
            
            // 檢查是否在攻擊線上
            float Projection = FVector::DotProduct(ToUnit, Direction);
            if (Projection >= 0.0f && Projection <= Distance)
            {
                FVector ClosestPoint = Start + Direction * Projection;
                float DistanceFromLine = FVector::Dist(ClosestPoint, UnitLocation);
                
                if (DistanceFromLine <= Width)
                {
                    ExecuteCombat(Attacker, Unit);
                }
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Executed line attack"));
}

void UMingRTSCombatSystem::AddAbility(AMingGoRTSUnit* Unit, const FRTSAbilityData& Ability)
{
    if (!Unit)
    {
        return;
    }

    if (!UnitAbilities.Contains(Unit))
    {
        UnitAbilities.Add(Unit, TArray<FRTSAbilityData>());
    }
    
    UnitAbilities[Unit].Add(Ability);
    
    UE_LOG(LogTemp, Log, TEXT("Added ability: %s"), *Ability.AbilityName);
}

void UMingRTSCombatSystem::RemoveAbility(AMingGoRTSUnit* Unit, const FString& AbilityName)
{
    if (!Unit || !UnitAbilities.Contains(Unit))
    {
        return;
    }

    TArray<FRTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    for (int32 i = 0; i < Abilities.Num(); ++i)
    {
        if (Abilities[i].AbilityName == AbilityName)
        {
            Abilities.RemoveAt(i);
            break;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Removed ability: %s"), *AbilityName);
}

bool UMingRTSCombatSystem::UseAbility(AMingGoRTSUnit* Unit, const FString& AbilityName, AMingGoRTSUnit* Target)
{
    if (!Unit || !UnitAbilities.Contains(Unit))
    {
        return false;
    }

    TArray<FRTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    for (FRTSAbilityData& Ability : Abilities)
    {
        if (Ability.AbilityName == AbilityName && Ability.bIsAvailable && Ability.CurrentCooldown <= 0.0f)
        {
            // 使用技能
            Ability.CurrentCooldown = Ability.Cooldown;
            Ability.bIsActive = true;
            
            // 這裡應該實現技能效果
            UE_LOG(LogTemp, Log, TEXT("Used ability: %s"), *AbilityName);
            
            NotifyAbilityUsed(Unit, AbilityName);
            return true;
        }
    }
    
    return false;
}

TArray<FRTSAbilityData> UMingRTSCombatSystem::GetUnitAbilities(AMingGoRTSUnit* Unit) const
{
    if (UnitAbilities.Contains(Unit))
    {
        return UnitAbilities[Unit];
    }
    
    return TArray<FRTSAbilityData>();
}

bool UMingRTSCombatSystem::IsAbilityAvailable(AMingGoRTSUnit* Unit, const FString& AbilityName) const
{
    if (!Unit || !UnitAbilities.Contains(Unit))
    {
        return false;
    }

    const TArray<FRTSAbilityData>& Abilities = UnitAbilities[Unit];
    
    for (const FRTSAbilityData& Ability : Abilities)
    {
        if (Ability.AbilityName == AbilityName && Ability.bIsAvailable && Ability.CurrentCooldown <= 0.0f)
        {
            return true;
        }
    }
    
    return false;
}

void UMingRTSCombatSystem::ApplyStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName, float Duration, float Power)
{
    if (!Unit)
    {
        return;
    }

    if (!StatusEffects.Contains(Unit))
    {
        StatusEffects.Add(Unit, TMap<FString, float>());
    }
    
    StatusEffects[Unit].Add(EffectName, Duration);
    
    UE_LOG(LogTemp, Log, TEXT("Applied status effect: %s"), *EffectName);
}

void UMingRTSCombatSystem::RemoveStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName)
{
    if (!Unit || !StatusEffects.Contains(Unit))
    {
        return;
    }

    StatusEffects[Unit].Remove(EffectName);
    
    UE_LOG(LogTemp, Log, TEXT("Removed status effect: %s"), *EffectName);
}

bool UMingRTSCombatSystem::HasStatusEffect(AMingGoRTSUnit* Unit, const FString& EffectName) const
{
    if (!Unit || !StatusEffects.Contains(Unit))
    {
        return false;
    }

    return StatusEffects[Unit].Contains(EffectName);
}

void UMingRTSCombatSystem::UpdateStatusEffects(float DeltaTime)
{
    for (auto& StatusPair : StatusEffects)
    {
        AMingGoRTSUnit* Unit = StatusPair.Key;
        TMap<FString, float>& Effects = StatusPair.Value;
        
        TArray<FString> EffectsToRemove;
        
        for (auto& EffectPair : Effects)
        {
            EffectPair.Value -= DeltaTime;
            if (EffectPair.Value <= 0.0f)
            {
                EffectsToRemove.Add(EffectPair.Key);
            }
        }
        
        for (const FString& EffectName : EffectsToRemove)
        {
            RemoveStatusEffect(Unit, EffectName);
        }
    }
}

void UMingRTSCombatSystem::UpdateCombat(float DeltaTime)
{
    if (!bIsInitialized)
    {
        return;
    }

    UpdateAbilityCooldowns(DeltaTime);
    UpdateStatusEffects(DeltaTime);
    ProcessCombatQueue();
    CleanupDeadUnits();
}

void UMingRTSCombatSystem::ProcessCombatQueue()
{
    // 處理所有戰鬥對
    TArray<AMingGoRTSUnit*> ProcessedAttackers;
    
    for (auto& CombatPair : CombatPairs)
    {
        AMingGoRTSUnit* Attacker = CombatPair.Key;
        AMingGoRTSUnit* Target = CombatPair.Value;
        
        if (Attacker && Target && !ProcessedAttackers.Contains(Attacker))
        {
            ExecuteCombat(Attacker, Target);
            ProcessedAttackers.Add(Attacker);
        }
    }
}

int32 UMingRTSCombatSystem::GetActiveCombatCount() const
{
    return CombatPairs.Num();
}

int32 UMingRTSCombatSystem::GetTotalDamageDealt() const
{
    return TotalDamageDealt;
}

int32 UMingRTSCombatSystem::GetUnitsKilled() const
{
    return UnitsKilled;
}

void UMingRTSCombatSystem::ResetCombatStats()
{
    TotalDamageDealt = 0;
    UnitsKilled = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Reset combat statistics"));
}

void UMingRTSCombatSystem::ProcessCombatPair(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, float DeltaTime)
{
    // 這裡可以實現持續戰鬥邏輯
    // 例如：攻擊速度計算、持續傷害等
}

void UMingRTSCombatSystem::ApplyCombatResult(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result)
{
    if (!Attacker || !Target || !UnitManager)
    {
        return;
    }

    // 更新傷害統計
    TotalDamageDealt += FMath::RoundToInt(Result.DamageDealt);
    
    // 應用傷害到目標
    UnitManager->UpdateUnitHealth(Target, -Result.DamageDealt);
    
    // 檢查是否擊殺
    if (!UnitManager->IsUnitAlive(Target))
    {
        UnitsKilled++;
        NotifyUnitKilled(Target, Attacker);
        StopCombat(Target);
        StopCombat(Attacker);
    }
}

FString UMingRTSCombatSystem::GenerateCombatResultMessage(const FRTSCombatResult& Result) const
{
    FString Message;
    
    if (Result.bDodged)
    {
        Message = TEXT("Dodged!");
    }
    else if (Result.bBlocked)
    {
        Message = FString::Printf(TEXT("Blocked! %.1f damage"), Result.DamageDealt);
    }
    else if (Result.bCritical)
    {
        Message = FString::Printf(TEXT("Critical Hit! %.1f damage"), Result.DamageDealt);
    }
    else
    {
        Message = FString::Printf(TEXT("Hit! %.1f damage"), Result.DamageDealt);
    }
    
    return Message;
}

float UMingRTSCombatSystem::GetDamageTypeMultiplier(ERTSDamageType DamageType, ERTSDamageType DefenseType) const
{
    // 傷害類型相互作用矩陣
    switch (DamageType)
    {
        case ERTSDamageType::Physical:
            return 1.0f;
        case ERTSDamageType::Fire:
            return 1.2f; // 火焰對大多數目標有效
        case ERTSDamageType::Explosive:
            return 1.5f; // 爆炸傷害對群體有效
        case ERTSDamageType::Piercing:
            return 1.1f; // 穿刺傷害對輕甲有效
        case ERTSDamageType::Magic:
            return 1.3f; // 魔法傷害對無防禦目標有效
        case ERTSDamageType::Poison:
            return 0.8f; // 毒素傷害持續但初始傷害較低
        default:
            return 1.0f;
    }
}

void UMingRTSCombatSystem::CleanupDeadUnits()
{
    if (!UnitManager)
    {
        return;
    }

    TArray<AMingGoRTSUnit*> DeadUnits;
    
    for (const auto& CombatPair : CombatPairs)
    {
        AMingGoRTSUnit* Attacker = CombatPair.Key;
        AMingGoRTSUnit* Target = CombatPair.Value;
        
        if (!UnitManager->IsUnitAlive(Attacker))
        {
            DeadUnits.Add(Attacker);
        }
        
        if (!UnitManager->IsUnitAlive(Target))
        {
            DeadUnits.Add(Target);
        }
    }
    
    for (AMingGoRTSUnit* DeadUnit : DeadUnits)
    {
        StopCombat(DeadUnit);
    }
}

void UMingRTSCombatSystem::UpdateAbilityCooldowns(float DeltaTime)
{
    for (auto& AbilityPair : UnitAbilities)
    {
        TArray<FRTSAbilityData>& Abilities = AbilityPair.Value;
        
        for (FRTSAbilityData& Ability : Abilities)
        {
            if (Ability.CurrentCooldown > 0.0f)
            {
                Ability.CurrentCooldown -= DeltaTime;
                if (Ability.CurrentCooldown <= 0.0f)
                {
                    Ability.bIsAvailable = true;
                    Ability.bIsActive = false;
                }
            }
        }
    }
}

void UMingRTSCombatSystem::NotifyCombatStarted(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, ERTSCombatType CombatType, ERTSDamageType DamageType)
{
    OnCombatStarted.Broadcast(Attacker, Target, CombatType, DamageType);
}

void UMingRTSCombatSystem::NotifyCombatHit(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target, const FRTSCombatResult& Result)
{
    OnCombatHit.Broadcast(Attacker, Target, Result);
}

void UMingRTSCombatSystem::NotifyCombatMissed(AMingGoRTSUnit* Attacker, AMingGoRTSUnit* Target)
{
    OnCombatMissed.Broadcast(Attacker, Target);
}

void UMingRTSCombatSystem::NotifyUnitKilled(AMingGoRTSUnit* Victim, AMingGoRTSUnit* Killer)
{
    OnUnitKilled.Broadcast(Victim, Killer);
}

void UMingRTSCombatSystem::NotifyAbilityUsed(AMingGoRTSUnit* Unit, const FString& AbilityName)
{
    OnAbilityUsed.Broadcast(Unit, AbilityName);
}

// 戰鬥平衡調整功能
void UMingRTSCombatSystem::BalanceCombatStats()
{
    // 根據遊戲數據平衡戰鬥統計
    UE_LOG(LogTemp, Log, TEXT("Balancing combat stats..."));

    // 調整基礎傷害和防禦
    BaseDamageMultiplier *= DifficultyMultiplier;
    BaseDefenseMultiplier *= DifficultyMultiplier;

    // 更新所有單位的戰鬥統計
    for (auto& StatPair : UnitCombatStats)
    {
        AMingGoRTSUnit* Unit = StatPair.Key;
        FRTSCombatStats& Stats = StatPair.Value;

        // 應用單位類型修正
        if (UnitTypeDamageModifiers.Contains(Unit->GetUnitType()))
        {
            Stats.AttackDamage *= UnitTypeDamageModifiers[Unit->GetUnitType()];
        }

        if (UnitTypeDefenseModifiers.Contains(Unit->GetUnitType()))
        {
            Stats.DefenseRating *= UnitTypeDefenseModifiers[Unit->GetUnitType()];
        }

        // 應用傷害類型效果
        if (DamageTypeEffectiveness.Contains(Stats.DamageType))
        {
            Stats.AttackDamage *= DamageTypeEffectiveness[Stats.DamageType];
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Combat stats balanced successfully"));
}

void UMingRTSCombatSystem::AdjustUnitTypeAdvantages()
{
    // 調整單位類型之間的優勢關係
    UE_LOG(LogTemp, Log, TEXT("Adjusting unit type advantages..."));

    // 步兵 vs 騎兵：步兵有優勢
    UnitTypeDamageModifiers[ERTSUnitType::Infantry] = 1.2f;
    UnitTypeDefenseModifiers[ERTSUnitType::Cavalry] = 0.8f;

    // 騎兵 vs 火炮：騎兵有優勢
    UnitTypeDamageModifiers[ERTSUnitType::Cavalry] = 1.3f;
    UnitTypeDefenseModifiers[ERTSUnitType::Artillery] = 0.7f;

    // 火炮 vs 建築：火炮有優勢
    UnitTypeDamageModifiers[ERTSUnitType::Artillery] = 1.5f;

    // 斥候 vs 偵察：斥候有優勢
    UnitTypeDamageModifiers[ERTSUnitType::Scout] = 1.1f;

    UE_LOG(LogTemp, Log, TEXT("Unit type advantages adjusted"));
}

void UMingRTSCombatSystem::SetDifficultyMultiplier(float Multiplier)
{
    DifficultyMultiplier = FMath::Clamp(Multiplier, 0.1f, 3.0f);
    UE_LOG(LogTemp, Log, TEXT("Difficulty multiplier set to: %f"), DifficultyMultiplier);
}

void UMingRTSCombatSystem::OptimizeCombatPerformance()
{
    // 優化戰鬥系統性能
    UE_LOG(LogTemp, Log, TEXT("Optimizing combat performance..."));

    // 清理無效的戰鬥對
    TArray<AMingGoRTSUnit*> InvalidPairs;
    for (auto& CombatPair : CombatPairs)
    {
        if (!CombatPair.Key || !CombatPair.Value || 
            !UnitManager->IsUnitAlive(CombatPair.Key) || 
            !UnitManager->IsUnitAlive(CombatPair.Value))
        {
            InvalidPairs.Add(CombatPair.Key);
        }
    }

    for (AMingGoRTSUnit* InvalidUnit : InvalidPairs)
    {
        CombatPairs.Remove(InvalidUnit);
    }

    // 清理無效的單位統計
    TArray<AMingGoRTSUnit*> InvalidStats;
    for (auto& StatPair : UnitCombatStats)
    {
        if (!StatPair.Key || !UnitManager->IsUnitAlive(StatPair.Key))
        {
            InvalidStats.Add(StatPair.Key);
        }
    }

    for (AMingGoRTSUnit* InvalidUnit : InvalidStats)
    {
        UnitCombatStats.Remove(InvalidUnit);
        UnitCombatStances.Remove(InvalidUnit);
        UnitAbilities.Remove(InvalidUnit);
        StatusEffects.Remove(InvalidUnit);
    }

    UE_LOG(LogTemp, Log, TEXT("Combat performance optimized"));
}
