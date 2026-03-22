#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingUnitController.h"
#include "MingTacticalUnit.generated.h"

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Infantry,
    Cavalry,
    Artillery,
    Tank,
    Aircraft,
    Support
};

USTRUCT(BlueprintType)
struct FUnitStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 CurrentHealth;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackRange;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AttackCooldown;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Defense;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MoveSpeed;

    FUnitStats()
        : MaxHealth(100)
        , CurrentHealth(100)
        , AttackDamage(10.0f)
        , AttackRange(100.0f)
        , AttackCooldown(1.0f)
        , Defense(5.0f)
        , MoveSpeed(300.0f)
    {}
};

class UMingUnitMovementComponent;
class UMingUnitCombatComponent;

UCLASS(ClassGroup = (Gameplay), Blueprintable)
class MINGTACTICAL_API AMingTacticalUnit : public AActor
{
    GENERATED_BODY()

public:
    AMingTacticalUnit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION(BlueprintPure, Category = "Unit")
    bool IsAlive() const { return UnitStats.CurrentHealth > 0; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    int32 GetUnitId() const { return UnitId; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    int32 GetTeamId() const { return TeamId; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetTeamId(int32 NewTeamId) { TeamId = NewTeamId; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    EUnitType GetUnitType() const { return UnitType; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetUnitType(EUnitType NewType) { UnitType = NewType; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    bool IsSelected() const { return bIsSelected; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetSelected(bool bSelected) { bIsSelected = bSelected; }

    UFUNCTION(BlueprintPure, Category = "Unit")
    EMingUnitState GetCurrentState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void SetCurrentState(EMingUnitState NewState) { CurrentState = NewState; }

    UFUNCTION(BlueprintPure, Category = "Unit Stats")
    const FUnitStats& GetUnitStats() const { return UnitStats; }

    UFUNCTION(BlueprintCallable, Category = "Unit Stats")
    void SetUnitStats(const FUnitStats& NewStats) { UnitStats = NewStats; }

    UFUNCTION(BlueprintCallable, Category = "Unit Stats")
    void TakeDamage(int32 DamageAmount);

    UFUNCTION(BlueprintCallable, Category = "Unit Stats")
    void Heal(int32 HealAmount);

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void MoveToLocation(const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Unit")
    void AttackTarget(AActor* Target);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UStaticMeshComponent> UnitMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UMingUnitMovementComponent> MovementComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<UMingUnitCombatComponent> CombatComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 UnitId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    int32 TeamId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    EUnitType UnitType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    bool bIsSelected;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit")
    EMingUnitState CurrentState;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unit Stats")
    FUnitStats UnitStats;

private:
    static int32 GlobalUnitCounter;

    void RegisterToInstancedRendering();
    void UnregisterFromInstancedRendering();
    void RegisterToSpatialPartition();
    void UnregisterFromSpatialPartition();
    void PublishUnitDeathEvent();
    void UpdateSelectionVisuals();
};
