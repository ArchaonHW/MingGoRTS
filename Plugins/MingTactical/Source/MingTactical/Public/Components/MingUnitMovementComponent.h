#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "MingUnitMovementComponent.generated.h"

/**
 * ��?X?�O��??
 */
USTRUCT(BlueprintType)
struct FMoveCommand
{
    GENERATED_BODY()
    
    // 摧毀��m
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ??�_摧毀��??
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAttackMove;
    
    // ??�_�ϥ�??��
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseFormation;
    
    // ??�Τ�??��m
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
 * 摧毀��??�ե�
 * 故事重要性?��摧毀��??��??�ɯ�
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitMovementComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ��?X?�O
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocation(const FVector& TargetLocation, bool bAttackMove = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocationWithFormation(const FVector& TargetLocation, const FVector& FormationOffset);

    // ??�??
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    // ??�_摧毀��??
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool IsMoving() const { return bIsMoving; }

    // 故事重要性?��
    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetCurrentSpeed() const;

    // �]�m��?X?��
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMoveSpeed(float NewSpeed);

    // ��?X???
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool HasValidPath() const { return CurrentPathPoints.Num() > 0; }

    UFUNCTION(BlueprintPure, Category = "Movement")
    FVector GetCurrentTargetLocation() const;

    // 目標數量
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateTowards(const FVector& TargetDirection, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetDesiredRotation(const FRotator& TargetRotation);

protected:
    // ???X?��??
    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // 摧毀��?X?X
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsMoving;

    // ��?X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // ???X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    // ??��Z�� (故事選項X?�HX
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AcceptanceRadius;

    // 摧毀��摧毀
    UPROPERTY()
    TArray<FVector> CurrentPathPoints;

    // 摧毀��??��??
    UPROPERTY()
    int32 CurrentPathIndex;

    // 目標數量��m
    UPROPERTY()
    FVector CurrentTargetLocation;

    // 目標數量
    UPROPERTY()
    FRotator DesiredRotation;

    // ??�_摧毀��??
    UPROPERTY()
    bool bIsAttackMove;

    // ??��??��
    UPROPERTY()
    FVector FormationOffset;

    // ��??��??ID
    uint32 PathRequestId;

    // �ɯ�t�Τޥ�
    class UNavigationSystemV1* NavSystem;

    // ��??��??
    void RequestPath(const FVector& StartLocation, const FVector& EndLocation);

    // ��?X?��??��
    void OnPathFound(uint32 InPathRequestId, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path);

    // �u??��??��??
    void FollowPath(float DeltaTime);

    // ��?X???
    void SmoothRotateTowards(const FVector& Direction, float DeltaTime);

    // �ˬd??�_目標數量
    bool HasReachedTarget() const;

    // ??�s��?X?X
    void UpdateMovementState();
};

