// Copyright Epic Games, Inc. All Rights Reserved.

#include "RTS/MingRTSCombatSystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UMingRTSCombatSystem::UMingRTSCombatSystem()
{
    // 初始化伤害倍率表
    DamageTypeMultipliers.Add(FName("Physical_Light"), 1.0f);
    DamageTypeMultipliers.Add(FName("Physical_Medium"), 0.75f);
    DamageTypeMultipliers.Add(FName("Physical_Heavy"), 0.5f);
    DamageTypeMultipliers.Add(FName("Physical_Fortified"), 0.25f);
    
    DamageTypeMultipliers.Add(FName("Piercing_Light"), 1.5f);
    DamageTypeMultipliers.Add(FName("Piercing_Medium"), 1.25f);
    DamageTypeMultipliers.Add(FName("Piercing_Heavy"), 1.0f);
    DamageTypeMultipliers.Add(FName("Piercing_Fortified"), 0.75f);
    
    DamageTypeMultipliers.Add(FName("Explosive_Light"), 1.25f);
    DamageTypeMultipliers.Add(FName("Explosive_Medium"), 1.5f);
    DamageTypeMultipliers.Add(FName("Explosive_Heavy"), 1.25f);
    DamageTypeMultipliers.Add(FName("Explosive_Fortified"), 1.0f);
    
    DamageTypeMultipliers.Add(FName("Fire_Light"), 1.5f);
    DamageTypeMultipliers.Add(FName("Fire_Medium"), 1.25f);
    DamageTypeMultipliers.Add(FName("Fire_Heavy"), 0.75f);
    DamageTypeMultipliers.Add(FName("Fire_Fortified"), 0.5f);

    // 初始化单位克制表
    UnitTypeAdvantages.Add(FName("Infantry_Cavalry"), 1.5f);
    UnitTypeAdvantages.Add(FName("Infantry_Archers"), 0.75f);
    UnitTypeAdvantages.Add(FName("Cavalry_Archers"), 1.5f);
    UnitTypeAdvantages.Add(FName("Cavalry_Infantry"), 0.75f);
    UnitTypeAdvantages.Add(FName("Archers_Infantry"), 1.5f);
    UnitTypeAdvantages.Add(FName("Archers_Cavalry"), 0.75f);
    UnitTypeAdvantages.Add(FName("Artillery_Infantry"), 1.25f);
    UnitTypeAdvantages.Add(FName("Artillery_Cavalry"), 1.25f);
}

void UMingRTSCombatSystem::InitializeCombatSystem()
{
    UnitCombatStats.Empty();
    UE_LOG(LogTemp, Log, TEXT("RTS Combat System initialized"));
}

float UMingRTSCombatSystem::CalculateDamage(const FRTSAttackInfo& AttackInfo, const FRTSCombatStats& DefenderStats)
{
    if (!AttackInfo.Attacker)
    {
        return 0.0f;
    }

    float BaseDamage = AttackInfo.Damage;
    
    // 应用护甲减免
    float ArmorReduction = CalculateArmorReduction(DefenderStats.Armor, AttackInfo.DamageType);
    float DamageAfterArmor = BaseDamage * (1.0f - ArmorReduction);
    
    // 应用伤害类型倍率
    float TypeMultiplier = GetDamageMultiplier(AttackInfo.DamageType, DefenderStats.ArmorType);
    float FinalDamage = DamageAfterArmor * TypeMultiplier;
    
    // 暴击伤害
    if (AttackInfo.bIsCritical)
    {
        FinalDamage *= 2.0f;
    }
    
    return FMath::Max(0.0f, FinalDamage);
}

float UMingRTSCombatSystem::GetDamageMultiplier(ERTSDamageType DamageType, ERTSArmorType ArmorType) const
{
    FString Key = FString::Printf(TEXT("%s_%s"), 
        *UEnum::GetValueAsString(DamageType),
        *UEnum::GetValueAsString(ArmorType));
    
    float* Multiplier = DamageTypeMultipliers.Find(FName(*Key));
    return Multiplier ? *Multiplier : 1.0f;
}

bool UMingRTSCombatSystem::IsAttackInRange(AActor* Attacker, AActor* Target, float AttackRange) const
{
    if (!Attacker || !Target)
    {
        return false;
    }
    
    float Distance = FVector::Distance(Attacker->GetActorLocation(), Target->GetActorLocation());
    return Distance <= AttackRange;
}

float UMingRTSCombatSystem::GetUnitTypeAdvantage(ERTSUnitType AttackerType, ERTSUnitType DefenderType) const
{
    FString Key = FString::Printf(TEXT("%s_%s"),
        *UEnum::GetValueAsString(AttackerType),
        *UEnum::GetValueAsString(DefenderType));
    
    float* Advantage = UnitTypeAdvantages.Find(FName(*Key));
    return Advantage ? *Advantage : 1.0f;
}

void UMingRTSCombatSystem::ModifyMorale(AActor* Unit, float MoraleDelta)
{
    if (!Unit)
    {
        return;
    }
    
    FRTSCombatStats* Stats = UnitCombatStats.Find(Unit);
    if (Stats)
    {
        Stats->Morale = FMath::Clamp(Stats->Morale + MoraleDelta, 0.0f, Stats->MaxMorale);
        OnMoraleChanged.Broadcast(Unit);
        
        // 士气过低时的效果
        if (Stats->Morale <= 0.0f)
        {
            // 可能触发溃逃
            UE_LOG(LogTemp, Warning, TEXT("Unit %s has broken morale!"), *Unit->GetName());
        }
    }
}

float UMingRTSCombatSystem::GetMoraleEffectMultiplier(float CurrentMorale, float MaxMorale) const
{
    float MoralePercent = CurrentMorale / MaxMorale;
    
    if (MoralePercent > 0.75f)
    {
        return 1.1f; // 高士气加成
    }
    else if (MoralePercent > 0.5f)
    {
        return 1.0f; // 正常
    }
    else if (MoralePercent > 0.25f)
    {
        return 0.8f; // 低士气减益
    }
    else
    {
        return 0.5f; // 极低士气
    }
}

void UMingRTSCombatSystem::RegisterUnitCombatStats(AActor* Unit, const FRTSCombatStats& Stats)
{
    if (Unit)
    {
        UnitCombatStats.Add(Unit, Stats);
    }
}

void UMingRTSCombatSystem::UnregisterUnitCombatStats(AActor* Unit)
{
    if (Unit)
    {
        UnitCombatStats.Remove(Unit);
    }
}

FRTSCombatStats UMingRTSCombatSystem::GetUnitCombatStats(AActor* Unit) const
{
    const FRTSCombatStats* Stats = UnitCombatStats.Find(Unit);
    return Stats ? *Stats : FRTSCombatStats();
}

void UMingRTSCombatSystem::ProcessAttack(const FRTSAttackInfo& AttackInfo)
{
    if (!AttackInfo.Attacker || !AttackInfo.Target)
    {
        return;
    }
    
    FRTSCombatStats DefenderStats = GetUnitCombatStats(AttackInfo.Target);
    float FinalDamage = CalculateDamage(AttackInfo, DefenderStats);
    
    ApplyDamage(AttackInfo.Target, FinalDamage, AttackInfo.DamageType, AttackInfo.Attacker);
    
    // 影响士气
    float MoraleImpact = -FinalDamage * 0.1f;
    ModifyMorale(AttackInfo.Target, MoraleImpact);
    
    // 广播攻击事件
    OnUnitDamaged.Broadcast(AttackInfo);
    
    UE_LOG(LogTemp, Log, TEXT("%s attacked %s for %.1f damage"),
        *AttackInfo.Attacker->GetName(),
        *AttackInfo.Target->GetName(),
        FinalDamage);
}

void UMingRTSCombatSystem::ApplyDamage(AActor* Target, float Damage, ERTSDamageType DamageType, AActor* Instigator)
{
    if (!Target || Damage <= 0.0f)
    {
        return;
    }
    
    FRTSCombatStats* Stats = UnitCombatStats.Find(Target);
    if (Stats)
    {
        Stats->Health = FMath::Max(0.0f, Stats->Health - Damage);
        
        // 检查死亡
        if (Stats->Health <= 0.0f)
        {
            CheckForUnitDeath(Target);
            OnUnitDied.Broadcast(Target, Instigator);
        }
    }
}

void UMingRTSCombatSystem::HealUnit(AActor* Unit, float HealAmount)
{
    if (!Unit || HealAmount <= 0.0f)
    {
        return;
    }
    
    FRTSCombatStats* Stats = UnitCombatStats.Find(Unit);
    if (Stats)
    {
        Stats->Health = FMath::Min(Stats->MaxHealth, Stats->Health + HealAmount);
    }
}

bool UMingRTSCombatSystem::IsUnitAlive(AActor* Unit) const
{
    const FRTSCombatStats* Stats = UnitCombatStats.Find(Unit);
    return Stats && Stats->Health > 0.0f;
}

float UMingRTSCombatSystem::GetUnitHealthPercent(AActor* Unit) const
{
    const FRTSCombatStats* Stats = UnitCombatStats.Find(Unit);
    if (Stats && Stats->MaxHealth > 0.0f)
    {
        return Stats->Health / Stats->MaxHealth;
    }
    return 0.0f;
}

void UMingRTSCombatSystem::ProcessAreaDamage(const FVector& Center, float Radius, float Damage, ERTSDamageType DamageType, AActor* Instigator)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    
    // 绘制调试范围
    DrawDebugSphere(World, Center, Radius, 32, FColor::Red, false, 2.0f);
    
    // 查找范围内的所有单位
    TArray<FOverlapResult> Overlaps;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);
    
    World->OverlapMultiByChannel(Overlaps, Center, FQuat::Identity, ECC_Pawn, Sphere);
    
    for (const FOverlapResult& Overlap : Overlaps)
    {
        AActor* Target = Overlap.GetActor();
        if (Target && Target != Instigator)
        {
            // 计算距离衰减
            float Distance = FVector::Distance(Center, Target->GetActorLocation());
            float DamageFalloff = 1.0f - (Distance / Radius);
            float FinalDamage = Damage * FMath::Max(0.0f, DamageFalloff);
            
            ApplyDamage(Target, FinalDamage, DamageType, Instigator);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Area damage dealt to %d units"), Overlaps.Num());
}

float UMingRTSCombatSystem::CalculateArmorReduction(float Armor, ERTSDamageType DamageType) const
{
    // 护甲减免公式: 护甲 / (护甲 + 100)
    float Reduction = Armor / (Armor + 100.0f);
    return FMath::Clamp(Reduction, 0.0f, 0.9f); // 最大90%减免
}

void UMingRTSCombatSystem::CheckForUnitDeath(AActor* Unit)
{
    if (!Unit)
    {
        return;
    }
    
    // 这里可以添加死亡动画、特效、掉落等逻辑
    UE_LOG(LogTemp, Log, TEXT("Unit %s has died"), *Unit->GetName());
    
    // 延迟销毁或播放死亡动画
    // Unit->Destroy();
}
