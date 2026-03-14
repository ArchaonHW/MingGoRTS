#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "RTSCameraPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MINGWARRTS_API ARTSCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ARTSCameraPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	// --- Components ---
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	// --- Settings ---
	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float MinZoom;

	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float MaxZoom;

	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float EdgeScrollSpeed;

	UPROPERTY(EditAnywhere, Category = "RTS Camera")
	float EdgeScrollThreshold;

	float TargetZoom;

	// --- Functions ---
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ZoomCamera(float AxisValue);
	void HandleEdgeScrolling(float DeltaTime);
};
