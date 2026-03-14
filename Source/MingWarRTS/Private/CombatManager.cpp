#include "CombatManager.h"
#include "RTSUnit.h"
#include "Math/UnrealMathUtility.h"

float UCombatManager::CalculateEffectiveAttack(ARTSUnit* Attacker)
{
    if (!Attacker) return 0.0f;

    // Get the base attack power
    float EffectiveAttack = Attacker->BaseAttackPower;

    // Determine and apply the supply penalty multiplier
    float SupplyPenalty = GetSupplyPenaltyMultiplier(Attacker->SupplyLevel);
    EffectiveAttack *= SupplyPenalty;

    UE_LOG(LogTemp, Log, TEXT("Unit %s Effective Attack: %.2f (Base: %.2f, Supply Penalty: %.2f)"), 
        *Attacker->GetName(), EffectiveAttack, Attacker->BaseAttackPower, SupplyPenalty);

    return EffectiveAttack;
}

float UCombatManager::CalculateEffectiveDefense(ARTSUnit* Defender)
{
    if (!Defender) return 0.0f;

    // Get the base defense power
    float EffectiveDefense = Defender->BaseDefensePower;

    // Determine and apply the supply penalty multiplier
    float SupplyPenalty = GetSupplyPenaltyMultiplier(Defender->SupplyLevel);
    EffectiveDefense *= SupplyPenalty;

    UE_LOG(LogTemp, Log, TEXT("Unit %s Effective Defense: %.2f (Base: %.2f, Supply Penalty: %.2f)"), 
        *Defender->GetName(), EffectiveDefense, Defender->BaseDefensePower, SupplyPenalty);

    return EffectiveDefense;
}

float UCombatManager::GetSupplyPenaltyMultiplier(float CurrentSupplyLevel)
{
    // Define supply penalty thresholds (Sprint 2 implementation)
    // - 90% to 100%: No penalty (Multiplier = 1.0)
    // - 75% to 89.9%: Mild penalty (Multiplier = 0.9)
    // - 50% to 74.9%: Moderate penalty (Multiplier = 0.75)
    // - Below 50%: Severe penalty (Multiplier = 0.5)

    if (CurrentSupplyLevel >= 90.0f)
    {
        return 1.0f;
    }
    else if (CurrentSupplyLevel >= 75.0f)
    {
        return 0.9f;
    }
    else if (CurrentSupplyLevel >= 50.0f)
    {
        return 0.75f;
    }
    else
    {
        return 0.5f;
    }
}