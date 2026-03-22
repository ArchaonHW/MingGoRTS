#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingUnitCombatComponent.generated.h"

class AMingTacticalUnit;

/**
 * ?»æ??½ä»¤çµæ?
 */
USTRUCT(BlueprintType)
struct FAttackCommand
{
    GENERATED_BODY()
    
    // ?®æ??®ä?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TWeakObjectPtr<AMingTacticalUnit> TargetUnit;
    
    // ?®æ?ä½ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ?¯å¦?ºä?ç½®æ”»??    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsLocationAttack;
    
    FAttackCommand()
        : TargetUnit(nullptr)
        , TargetLocation(FVector::ZeroVector)
        , bIsLocationAttack(false)
    {}
};

/**
 * ?®ä??°é¬¥çµ„ä»¶
 * ?•ç??®ä??„æ”»?Šã€é˜²ç¦¦å??·å®³?è¼¯
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitCombatComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitCombatComponent(};

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ?»æ??½ä»¤
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackTarget(AMingTacticalUnit* Target};

    UFUNCTION(BlueprintCallable, Category = "Combat")
    void AttackLocation(const FVector& TargetLocation};

    // ?œæ­¢?»æ?
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void StopAttack(};

    // ?¯å¦?¨æ”»?Šç??å…§
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsTargetInRange(AMingTacticalUnit* Target) const;

    UFUNCTION(BlueprintPure, Category = "Combat")
    bool IsLocationInRange(const FVector& Location) const;

    // ?²å??¶å??®æ?
    UFUNCTION(BlueprintPure, Category = "Combat")
    AMingTacticalUnit* GetCurrentTarget() const { return CurrentTarget.Get(}; }

    // ?¯å¦?¯ä»¥?»æ?
    UFUNCTION(BlueprintPure, Category = "Combat")
    bool CanAttack() const;

    // è¨­ç½®?»æ??·å»
    UFUNCTION(BlueprintCallable, Category = "Combat")
    void SetAttackCooldown(float Cooldown};

protected:
    // ?æ??…å–®ä½?    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // ?¶å??»æ??®æ?
    UPROPERTY()
    TWeakObjectPtr<AMingTacticalUnit> CurrentTarget;

    // ?®æ?ä½ç½®?»æ?
    UPROPERTY()
    FVector TargetAttackLocation;

    // ?¯å¦?¨æ”»?Šä?ç½?    UPROPERTY()
    bool bIsAttackingLocation;

    // ?¯å¦?¨æ”»??    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
    bool bIsAttacking;

    // ?»æ??·å»?‚é?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    float AttackCooldown;

    // ?¶å??·å»?‚é?
    UPROPERTY()
    float CurrentCooldown;

    // ?»æ?ç¯„å?
    UPROPERTY()
    float AttackRange;

    // ?»æ??·å®³
    UPROPERTY()
    float AttackDamage;

    // ?·è??»æ?
    void PerformAttack(};

    // å°ç›®æ¨™é€ æ??·å®³
    void DealDamageToTarget(AMingTacticalUnit* Target};

    // å°‹æ‰¾ç¯„å??§ç??µäºº
    AMingTacticalUnit* FindEnemyInRange(};

    // ?‹è??‘ç›®æ¨?    void RotateToTarget(float DeltaTime};

    // æª¢æŸ¥?®æ??¯å¦?‰æ?
    bool IsValidTarget(AMingTacticalUnit* Target) const;

    // ?»æ??•ç•«è§¸ç™¼
    void TriggerAttackAnimation(};
};

