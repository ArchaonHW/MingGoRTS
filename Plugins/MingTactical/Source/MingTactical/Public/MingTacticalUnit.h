#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingCore/Source/MingCore/Public/MingCoreEventBus.h"
#include "MingTacticalUnit.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MINGTACTICAL_API AMingTacticalUnit : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalUnit();

    // 單位基本屬性
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Basic")
    int32 UnitId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Basic")
    FString UnitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Basic")
    int32 MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Basic")
    int32 CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Movement")
    float MovementSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Combat")
    int32 AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Combat")
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit|Combat")
    float AttackCooldown;

    // 單位狀態
    UPROPERTY(BlueprintReadOnly, Category = "Unit|State")
    bool bIsSelected;

    UPROPERTY(BlueprintReadOnly, Category = "Unit|State")
    bool bIsMoving;

    UPROPERTY(BlueprintReadOnly, Category = "Unit|State")
    bool bIsAttacking;

    UPROPERTY(BlueprintReadOnly, Category = "Unit|State")
    FVector TargetPosition;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // 事件處理
    UFUNCTION()
    void OnUnitSelected(const FUnitSelectedEvent& Event);

    UFUNCTION()
    void OnUnitMoved(const FUnitMovedEvent& Event);

    // 單位行為
    void MoveToPosition(const FVector& Position, bool bIsAttackMove = false);
    void AttackTarget(class AActor* Target);
    void StopCurrentAction();

    // 性能優化：避免Tick()濫用
    void UpdateUnitState(float DeltaTime);
    void ProcessMovement(float DeltaTime);
    void ProcessCombat(float DeltaTime);

private:
    // 記憶體池相關
    static TArray<AMingTacticalUnit*> UnitPool;
    static int32 NextUnitId;

    // 性能計時器
    float LastAttackTime;
    float LastUpdateTime;

    // 路徑尋找
    class UNavigationSystem* NavSystem;
    FNavPathSharedPtr CurrentPath;
    int32 CurrentPathIndex;

    // 目標相關
    TObjectPtr<AActor> CurrentTarget;
    bool bHasValidTarget;

    // 事件訂閱管理
    void SubscribeToEvents();
    void UnsubscribeFromEvents();

public:
    // 靜態工廠方法（記憶體池優化）
    static AMingTacticalUnit* CreateUnit(UWorld* World, const FVector& Location);
    static void DestroyUnit(AMingTacticalUnit* Unit);

    // Blueprint callable functions
    UFUNCTION(BlueprintCallable, Category = "Unit|Control")
    void SelectUnit();

    UFUNCTION(BlueprintCallable, Category = "Unit|Control")
    void DeselectUnit();

    UFUNCTION(BlueprintCallable, Category = "Unit|Movement")
    void MoveToLocation(const FVector& TargetLocation, bool bAttackMove = false);

    UFUNCTION(BlueprintCallable, Category = "Unit|Combat")
    void SetAttackTarget(AActor* Target);

    // 獲取單位信息
    UFUNCTION(BlueprintPure, Category = "Unit|Info")
    bool IsAlive() const { return CurrentHealth > 0; }

    UFUNCTION(BlueprintPure, Category = "Unit|Info")
    float GetHealthPercentage() const { return (float)CurrentHealth / (float)MaxHealth; }

    UFUNCTION(BlueprintPure, Category = "Unit|Info")
    bool CanAttack() const;

    UFUNCTION(BlueprintPure, Category = "Unit|Info")
    bool IsInAttackRange(AActor* Target) const;
};
