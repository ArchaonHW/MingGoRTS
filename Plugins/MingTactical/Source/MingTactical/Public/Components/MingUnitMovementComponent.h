#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AI/Navigation/NavigationTypes.h"
#include "MingUnitMovementComponent.generated.h"

/**
 * ç§»å??½ä»¤çµæ?
 */
USTRUCT(BlueprintType)
struct FMoveCommand
{
    GENERATED_BODY()
    
    // ?®æ?ä½ç½®
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector TargetLocation;
    
    // ?¯å¦?»æ?ç§»å?
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAttackMove;
    
    // ?¯å¦ä½¿ç”¨?Šå½¢
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseFormation;
    
    // ?Šå½¢ä¸­ç?ä½ç½®
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
 * ?®ä?ç§»å?çµ„ä»¶
 * ?•ç??®ä??„ç§»?•é?è¼¯å?è·¯å?å°èˆª
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGTACTICAL_API UMingUnitMovementComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMingUnitMovementComponent();

    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // ç§»å??½ä»¤
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocation(const FVector& TargetLocation, bool bAttackMove = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveToLocationWithFormation(const FVector& TargetLocation, const FVector& FormationOffset);

    // ?œæ­¢ç§»å?
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    // ?¯å¦æ­?œ¨ç§»å?
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool IsMoving() const { return bIsMoving; }

    // ?²å??¶å??Ÿåº¦
    UFUNCTION(BlueprintPure, Category = "Movement")
    float GetCurrentSpeed() const;

    // è¨­ç½®ç§»å??Ÿåº¦
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetMoveSpeed(float NewSpeed);

    // è·¯å??¸é?
    UFUNCTION(BlueprintPure, Category = "Movement")
    bool HasValidPath() const { return CurrentPathPoints.Num() > 0; }

    UFUNCTION(BlueprintPure, Category = "Movement")
    FVector GetCurrentTargetLocation() const;

    // ?‹è??¸é?
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateTowards(const FVector& TargetDirection, float DeltaTime);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void SetDesiredRotation(const FRotator& TargetRotation);

protected:
    // ?æ??…å–®ä½?    UPROPERTY()
    TObjectPtr<AMingTacticalUnit> OwnerUnit;

    // ?¶å?ç§»å??€??    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    bool bIsMoving;

    // ç§»å??Ÿåº¦
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MoveSpeed;

    // ?‹è??Ÿåº¦
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float RotationSpeed;

    // ?œæ­¢è·é›¢ (?°é??®æ??„è??¢é–¾??
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AcceptanceRadius;

    // ?¶å?è·¯å?é»?    UPROPERTY()
    TArray<FVector> CurrentPathPoints;

    // ?¶å?è·¯å?ç´¢å?
    UPROPERTY()
    int32 CurrentPathIndex;

    // ?¶å??®æ?ä½ç½®
    UPROPERTY()
    FVector CurrentTargetLocation;

    // ?®æ??‹è?
    UPROPERTY()
    FRotator DesiredRotation;

    // ?¯å¦?»æ?ç§»å?
    UPROPERTY()
    bool bIsAttackMove;

    // ?Šå½¢?ç§»
    UPROPERTY()
    FVector FormationOffset;

    // è·¯å?è«‹æ?ID
    uint32 PathRequestId;

    // å°èˆªç³»çµ±å¼•ç”¨
    class UNavigationSystemV1* NavSystem;

    // è«‹æ?è·¯å?
    void RequestPath(const FVector& StartLocation, const FVector& EndLocation);

    // è·¯å??¥è©¢?èª¿
    void OnPathFound(uint32 InPathRequestId, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path);

    // æ²¿è?è·¯å?ç§»å?
    void FollowPath(float DeltaTime);

    // å¹³æ??‹è?
    void SmoothRotateTowards(const FVector& Direction, float DeltaTime);

    // æª¢æŸ¥?¯å¦?°é??®æ?
    bool HasReachedTarget() const;

    // ?´æ–°ç§»å??€??    void UpdateMovementState();
};
