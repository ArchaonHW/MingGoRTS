#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingUnitCombatComponent.generated.h"

class AMingTacticalUnit;

/**
 * ?��X�令結�?
 */
USTRUCT(BlueprintType)
struct FAttackCommand
{
    GENERATED_BODY()
    
    // ?��X��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AMingTacticalUnit> TargetUnit;
    
    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ?�否?��?置攻X    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLocationAttack;
    
    FAttackCommand()
        : TargetUnit(nullptr)
        , TargetLocation(FVector::ZeroVector)
        , bIsLocationAttack(false)
    {}
};

/**
 * ?��X�鬥組件
 * ?��X��X�攻?�、防禦�X�害?�輯
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitCombatComponent(};

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ?��X�令
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackTarget(AMingTacticalUnit* Target};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackLocation(const FVector& TargetLocation};

    // ?�止?��?
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAttack(};

    // ?�否?�攻?��X�內
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsTargetInRange(AMingTacticalUnit* Target) const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsLocationInRange(const FVector& Location) const;

    // ?��X��X��?
    UFUNCTION(BlueprintPure, Category = "Combat")
    AMingTacticalUnit* GetCurrentTarget() const { return CurrentTarget.Get(}; }

    // ?�否?�以?��?
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool CanAttack() const;

    // 設置?��X�卻
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackCooldown(float Cooldown};

protected:
    // ?��X�單�?    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // ?��X��X��?
    UPROPERTY()
    TWeakObjectPtr<AMingTacticalUnit> CurrentTarget;

    // ?��?位置?��?
    UPROPERTY()
    FVector TargetAttackLocation;

    // ?�否?�攻?��?�?    UPROPERTY()
    bool bIsAttackingLocation;

    // ?�否?�攻X    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    // ?��X�卻?��?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    // ?��X�卻?��?
    UPROPERTY()
    float CurrentCooldown;

    // ?��?範�?
    UPROPERTY()
    float AttackRange;

    // ?��X�害
    UPROPERTY()
    float AttackDamage;

    // ?��X��?
    void PerformAttack(};

    // 對目標造�X�害
    void DealDamageToTarget(AMingTacticalUnit* Target};

    // 尋找範�X��X�人
    AMingTacticalUnit* FindEnemyInRange(};

    // ?��X�目�?    void RotateToTarget(float DeltaTime};

    // 檢查?��X�否?��?
    bool IsValidTarget(AMingTacticalUnit* Target) const;

    // ?��X�畫觸發
    void TriggerAttackAnimation(};
};

