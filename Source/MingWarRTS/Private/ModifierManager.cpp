#include "ModifierManager.h"

UModifierManager::UModifierManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UModifierManager::ApplyModifier(FName ModifierID, float Value)
{
    // Add or update the modifier in our map
    ActiveModifiers.FindOrAdd(ModifierID) = Value;

    // Broadcast the change to any listening systems
    OnModifierApplied.Broadcast(ModifierID, Value);
    
    UE_LOG(LogTemp, Log, TEXT("Modifier '%s' applied with value: %f"), *ModifierID.ToString(), Value);
}
