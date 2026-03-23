#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraConponent.h"
#include "GameFramework/SpringArmConponent.h"
#include "MingGoRTSCamera.generated.h"

UCLASS()
class MINGRTS_API AMingGoRTSCamera : public APawn
{
    GENERATED_BODY()

public:
    AMingGoRTSCamera(};

protected:
    virtual void BeginPlay() override;
    virtual void SetupPlayerInputConponent(UInputConponent* PlayerInputConponent) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    USpringArmConponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    UCameraConponent* Camera;

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void SetCameraDistance(float Distance};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void MoveCamera(const FVector& Direction};

private:
    UPROPERTY()
    float DefaultDistance;
};


