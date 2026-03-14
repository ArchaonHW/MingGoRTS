#include "SupplyManager.h"
#include "RTSUnit.h"

USupplyManager::USupplyManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USupplyManager::UpdateSupplyLevels(const TArray<ARTSUnit*>& AllUnits)
{
    if (AllUnits.Num() == 0) return;

    for (ARTSUnit* Unit : AllUnits)
    {
        if (!Unit) continue;

        if (IsLocationSupplied(Unit->GetActorLocation()))
        {
            // If supplied, regenerate supply (up to a max of 100)
            Unit->SupplyLevel = FMath::Min(Unit->SupplyLevel + 0.1f, 100.0f);
        }
        else
        {
            // If not supplied, decrease supply (down to a min of 0)
            Unit->SupplyLevel = FMath::Max(Unit->SupplyLevel - 0.05f, 0.0f);
        }
    }
}

bool USupplyManager::IsLocationSupplied(const FVector& Location)
{
    // Simplified logic for Sprint 1:
    // Only the area near the world origin (0,0,0) is considered supplied.
    // We will replace this with a real supply network calculation in a future sprint.
    const float SupplyRadius = 3000.0f; // 30 meters
    return Location.Size() < SupplyRadius;
}
