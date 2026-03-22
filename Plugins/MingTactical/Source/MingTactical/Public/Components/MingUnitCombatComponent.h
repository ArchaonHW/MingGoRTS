#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingUnitCombatComponent.generated.h"

class AMingTacticalUnit;

/**
 * 攻擊命令結構
 */
USTRUCT(BlueprintType)
struct FAttackCommand
{
    GENERATED_BODY()
    
    // 目標單位
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AMingTacticalUnit> TargetUnit;
    
    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // 是否為位置攻擊
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLocationAttack;
    
    FAttackCommand()
        : TargetUnit(nullptr)
        , TargetLocation(FVector::ZeroVector)
        , bIsLocationAttack(false)
    {}
};

/**
 * 單位戰鬥組件
 * 處理單位的攻擊、防禦和傷害邏輯
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitCombatComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 攻擊命令
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackTarget(AMingTacticalUnit* Target);

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackLocation(const FVector& TargetLocation);

    // 停止攻擊
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAttack();

    // 是否在攻擊範圍內
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsTargetInRange(AMingTacticalUnit* Target) const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsLocationInRange(const FVector& Location) const;

    // 獲取當前目標
    UFUNCTION(BlueprintPure, Category = "Combat")
    AMingTacticalUnit* GetCurrentTarget() const { return CurrentTarget.Get(); }

    // 是否可以攻擊
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool CanAttack() const;

    // 設置攻擊冷卻
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackCooldown(float Cooldown);

protected:
    // 擁有者單位
    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // 當前攻擊目標
    UPROPERTY()
    TWeakObjectPtr<AMingTacticalUnit> CurrentTarget;

    // 目標位置攻擊
    UPROPERTY()
    FVector TargetAttackLocation;

    // 是否在攻擊位置
    UPROPERTY()
    bool bIsAttackingLocation;

    // 是否在攻擊
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    // 攻擊冷卻時間
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    // 當前冷卻時間
    UPROPERTY()
    float CurrentCooldown;

    // 攻擊範圍
    UPROPERTY()
    float AttackRange;

    // 攻擊傷害
    UPROPERTY()
    float AttackDamage;

    // 執行攻擊
    void PerformAttack();

    // 對目標造成傷害
    void DealDamageToTarget(AMingTacticalUnit* Target);

    // 尋找範圍內的敵人
    AMingTacticalUnit* FindEnemyInRange();

    // 旋轉向目標
    void RotateToTarget(float DeltaTime);

    // 檢查目標是否有效
    bool IsValidTarget(AMingTacticalUnit* Target) const;

    // 攻擊動畫觸發
    void TriggerAttackAnimation();
};
