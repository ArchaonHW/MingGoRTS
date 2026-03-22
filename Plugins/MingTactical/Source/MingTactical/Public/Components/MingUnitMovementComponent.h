#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "MingUnitMovementComponent.generated.h"

/**
 * 移�X�令結�?
 */
USTRUCT(BlueprintType)
struct FMoveCommand
{
    GENERATED_BODY()
    
    // ?��?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ?�否?��?移�?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAttackMove;
    
    // ?�否使用?�形
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseFormation;
    
    // ?�形中�?位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector FormationOffset;
    
    FMoveCommand()
        : TargetLocation(FVector::ZeroVector)
        , bAttackMove(false)
        , bUseFormation(true)
        , FormationOffset(FVector::ZeroVector)
    {}
};

class AMingTacticalUnit;

/**
 * ?��?移�?組件
 * ?��X��X�移?��?輯�?路�?導航
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitMovementComponent(};

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 移�X�令
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocation(const FVector& TargetLocation, bool bAttackMove = false};

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocationWithFormation(const FVector& TargetLocation, const FVector& FormationOffset};

    // ?�止移�?
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement(};

    // ?�否�?��移�?
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool IsMoving() const { return bIsMoving; }

    // ?��X��X�度
    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetCurrentSpeed() const;

    // 設置移�X�度
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMoveSpeed(float NewSpeed};

    // 路�X��?
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool HasValidPath() const { return CurrentPathPoints.Num() > 0; }

    UFUNCTION(BlueprintPure, Category = "Movement")
    FVector GetCurrentTargetLocation() const;

    // ?��X��?
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateTowards(const FVector& TargetDirection, float DeltaTime};

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetDesiredRotation(const FRotator& TargetRotation};

protected:
    // ?��X�單�?    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // ?��?移�X�X    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsMoving;

    // 移�X�度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // ?��X�度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    // ?�止距離 (?��X��X��X�閾X
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AcceptanceRadius;

    // ?��?路�?�?    UPROPERTY()
    TArray<FVector> CurrentPathPoints;

    // ?��?路�?索�?
    UPROPERTY()
    int32 CurrentPathIndex;

    // ?��X��?位置
    UPROPERTY()
    FVector CurrentTargetLocation;

    // ?��X��?
    UPROPERTY()
    FRotator DesiredRotation;

    // ?�否?��?移�?
    UPROPERTY()
    bool bIsAttackMove;

    // ?�形?�移
    UPROPERTY()
    FVector FormationOffset;

    // 路�?請�?ID
    uint32 PathRequestId;

    // 導航系統引用
    class UNavigationSystemV1* NavSystem;

    // 請�?路�?
    void RequestPath(const FVector& StartLocation, const FVector& EndLocation};

    // 路�X�詢?�調
    void OnPathFound(uint32 InPathRequestId, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path};

    // 沿�?路�?移�?
    void FollowPath(float DeltaTime};

    // 平�X��?
    void SmoothRotateTowards(const FVector& Direction, float DeltaTime};

    // 檢查?�否?��X��?
    bool HasReachedTarget() const;

    // ?�新移�X�X    void UpdateMovementState(};
};

