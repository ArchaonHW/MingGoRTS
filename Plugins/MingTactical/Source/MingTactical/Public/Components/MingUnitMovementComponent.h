#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "MingUnitMovementComponent.generated.h"

/**
 * 移動命令結構
 */
USTRUCT(BlueprintType)
struct FMoveCommand
{
    GENERATED_BODY()
    
    // 目標位置
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // 是否攻擊移動
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAttackMove;
    
    // 是否使用隊形
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseFormation;
    
    // 隊形中的位置
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
 * 單位移動組件
 * 處理單位的移動邏輯和路徑導航
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitMovementComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 移動命令
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocation(const FVector& TargetLocation, bool bAttackMove = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocationWithFormation(const FVector& TargetLocation, const FVector& FormationOffset);

    // 停止移動
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    // 是否正在移動
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool IsMoving() const { return bIsMoving; }

    // 獲取當前速度
    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetCurrentSpeed() const;

    // 設置移動速度
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMoveSpeed(float NewSpeed);

    // 路徑相關
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool HasValidPath() const { return CurrentPathPoints.Num() > 0; }

    UFUNCTION(BlueprintPure, Category = "Movement")
    FVector GetCurrentTargetLocation() const;

    // 旋轉相關
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateTowards(const FVector& TargetDirection, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetDesiredRotation(const FRotator& TargetRotation);

protected:
    // 擁有者單位
    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // 當前移動狀態
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsMoving;

    // 移動速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // 旋轉速度
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    // 停止距離 (到達目標的距離閾值)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AcceptanceRadius;

    // 當前路徑點
    UPROPERTY()
    TArray<FVector> CurrentPathPoints;

    // 當前路徑索引
    UPROPERTY()
    int32 CurrentPathIndex;

    // 當前目標位置
    UPROPERTY()
    FVector CurrentTargetLocation;

    // 目標旋轉
    UPROPERTY()
    FRotator DesiredRotation;

    // 是否攻擊移動
    UPROPERTY()
    bool bIsAttackMove;

    // 隊形偏移
    UPROPERTY()
    FVector FormationOffset;

    // 路徑請求ID
    uint32 PathRequestId;

    // 導航系統引用
    class UNavigationSystemV1* NavSystem;

    // 請求路徑
    void RequestPath(const FVector& StartLocation, const FVector& EndLocation);

    // 路徑查詢回調
    void OnPathFound(uint32 InPathRequestId, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path);

    // 沿著路徑移動
    void FollowPath(float DeltaTime);

    // 平滑旋轉
    void SmoothRotateTowards(const FVector& Direction, float DeltaTime);

    // 檢查是否到達目標
    bool HasReachedTarget() const;

    // 更新移動狀態
    void UpdateMovementState();
};
