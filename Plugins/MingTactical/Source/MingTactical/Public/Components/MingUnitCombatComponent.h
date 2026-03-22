#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingUnitCombatComponent.generated.h"

class AMingTacticalUnit;

/**
 * ???X?�O��??
 */
USTRUCT(BlueprintType)
struct FAttackCommand
{
    GENERATED_BODY()
    
    // 目標數量
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AMingTacticalUnit> TargetUnit;
    
    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ??�_摧毀�m��X
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLocationAttack;
    
    FAttackCommand()
        : TargetUnit(nullptr)
        , TargetLocation(FVector::ZeroVector)
        , bIsLocationAttack(false)
    {}
};

/**
 * ???X?���ե�
 * 故事重要性?��??�B���m?X?�`??��
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitCombatComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ???X?�O
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackTarget(AMingTacticalUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackLocation(const FVector& TargetLocation);

    // ??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAttack();

    // ??�_??��???X?��
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsTargetInRange(AMingTacticalUnit* Target) const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsLocationInRange(const FVector& Location) const;

    // 故事選項?
    UFUNCTION(BlueprintPure, Category = "Combat")
    AMingTacticalUnit* GetCurrentTarget() const { return CurrentTarget.Get(); }

    // ??�_??�H摧毀
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool CanAttack() const;

    // �]�m???X?�o
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackCooldown(float Cooldown);

protected:
    // ???X?��??
    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // 故事選項?
    UPROPERTY()
    TWeakObjectPtr<AMingTacticalUnit> CurrentTarget;

    // 摧毀��m摧毀
    UPROPERTY()
    FVector TargetAttackLocation;

    // ??�_??��摧毀??
    UPROPERTY()
    bool bIsAttackingLocation;

    // ??�_??��X
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    // ???X?�o摧毀
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    // ???X?�o摧毀
    UPROPERTY()
    float CurrentCooldown;

    // 摧毀�d??
    UPROPERTY()
    float AttackRange;

    // ???X?�`
    UPROPERTY()
    float AttackDamage;

    // 目標數量
    void PerformAttack();

    // ��ؼгy?X?�`
    void DealDamageToTarget(AMingTacticalUnit* Target);

    // �M��d?X??X?�H
    AMingTacticalUnit* FindEnemyInRange();

    // ???X?��??
    void RotateToTarget(float DeltaTime);

    // �ˬd???X?�_摧毀
    bool IsValidTarget(AMingTacticalUnit* Target) const;

    // ???X?�eĲ�o
    void TriggerAttackAnimation();
};

