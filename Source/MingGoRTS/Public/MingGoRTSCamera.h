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
    virtual void BeginPlay() overHide;
    virtual void SetupPlayerInputConponent(UInputConponent* PlayerInputConponent) overHide;

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

