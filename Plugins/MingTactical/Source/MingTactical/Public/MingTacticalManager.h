#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingTacticalManager.generated.h"

/**
 * 戰術層管理器
 * 管理大規模戰鬥和單位控制
 */
UCLASS()
class MINGTACTICAL_API UMingTacticalManager : public UObject
{
    GENERATED_BODY()

public:
    UMingTacticalManager();

    virtual void Initialize();
    virtual void Shutdown();

    void SetupEventSubscriptions();
    void CleanupEventSubscriptions();

    // 單位控制接口
    UFUNCTION(BlueprintCallable, Category = "Tactical")
    void SelectUnits(const TArray<int32>& UnitIds);

    UFUNCTION(BlueprintCallable, Category = "Tactical")
    void MoveUnits(const TArray<int32>& UnitIds, const FVector& TargetPosition, bool bAttackMove = false);

    // 性能監控
    UFUNCTION(BlueprintCallable, Category = "Performance")
    int32 GetActiveUnitCount() const { return ActiveUnitCount; }

protected:
    void OnUnitSelected(const int32 UnitId);
    void OnUnitMoved(const int32 UnitId, const FVector& NewPosition);

private:
    bool bIsInitialized;
    int32 ActiveUnitCount;
    
    // 記憶體池預分配標記
    bool bMemoryPoolInitialized;
};
