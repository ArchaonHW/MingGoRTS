#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalManager.generated.h"

/**
 * ?∞Ë?Â±§ÁÆ°?ÜÂô®
 * ÁÆ°Á?Â§ßË?Ê®°Êà∞È¨•Â??Æ‰??ßÂà∂
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalManager(};

    virtual void Initialize(};
    virtual void Shutdown(};

    void SetupEventSubscriptions(};
    void CleanupEventSubscriptions(};

    // ?Æ‰??ßÂà∂?•Âè£
    UFUNCTION(BlueprintCallable, Category = "Tactical")
    void SelectUnits(const TArray<int32>& UnitIds};

    UFUNCTION(BlueprintCallable, Category = "Tactical")
    void MoveUnits(const TArray<int32>& UnitIds, const FVector& TargetPosition, bool bAttackMove = false};

    // ?ßËÉΩ??éß
    UFUNCTION(BlueprintCallable, Category = "Performance")
    int32 GetActiveUnitCount() const { return ActiveUnitCount; }

protected:
    void OnUnitSelected(const int32 UnitId};
    void OnUnitMoved(const int32 UnitId, const FVector& NewPosition};

private:
    bool bIsInitialized;
    int32 ActiveUnitCount;
    
    // Ë®òÊÜ∂È´îÊ??êÂ??çÊ?Ë®?    bool bMemoryPoolInitialized;
};

