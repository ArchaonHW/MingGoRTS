#include "RTSCameraPawn.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"

ARTSCameraPawn::ARTSCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Component Setup
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 2000.0f;
	SpringArm->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); // Angled down like an RTS
	SpringArm->bDoCollisionTest = false; // Prevent camera from colliding with buildings/terrain

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Default Settings
	MoveSpeed = 2000.0f;
	ZoomSpeed = 300.0f;
	MinZoom = 800.0f;
	MaxZoom = 4000.0f;
	EdgeScrollSpeed = 2000.0f;
	EdgeScrollThreshold = 20.0f;

	TargetZoom = SpringArm->TargetArmLength;
}

void ARTSCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	TargetZoom = SpringArm->TargetArmLength;
}

void ARTSCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. Smooth Camera Zoom
	if (FMath::Abs(SpringArm->TargetArmLength - TargetZoom) > 1.0f)
	{
		SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, TargetZoom, DeltaTime, 10.0f);
	}

	// 2. Edge Scrolling
	HandleEdgeScrolling(DeltaTime);
}

void ARTSCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Axis inputs from Engine Settings (W/A/S/D and Mouse Wheel)
	PlayerInputComponent->BindAxis("MoveForward", this, &ARTSCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ARTSCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("Zoom", this, &ARTSCameraPawn::ZoomCamera);
}

void ARTSCameraPawn::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		// Move along the X-Y plane using SpringArm's forward rotation
		FVector Forward = SpringArm->GetForwardVector();
		Forward.Z = 0.0f; // Stay flat on the ground
		Forward.Normalize();
		AddActorWorldOffset(Forward * AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
	}
}

void ARTSCameraPawn::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		FVector Right = SpringArm->GetRightVector();
		Right.Z = 0.0f;
		Right.Normalize();
		AddActorWorldOffset(Right * AxisValue * MoveSpeed * GetWorld()->GetDeltaSeconds(), true);
	}
}

void ARTSCameraPawn::ZoomCamera(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		TargetZoom = FMath::Clamp(TargetZoom - (AxisValue * ZoomSpeed), MinZoom, MaxZoom);
	}
}

void ARTSCameraPawn::HandleEdgeScrolling(float DeltaTime)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	float MouseX, MouseY;
	if (PC->GetMousePosition(MouseX, MouseY))
	{
		float ScrollX = 0.0f;
		float ScrollY = 0.0f;

		// Check left/right edges
		if (MouseX <= EdgeScrollThreshold) 
			ScrollX = -1.0f;
		else if (MouseX >= ViewportSizeX - EdgeScrollThreshold) 
			ScrollX = 1.0f;

		// Check top/bottom edges (Y=0 is top of screen in UE)
		if (MouseY <= EdgeScrollThreshold) 
			ScrollY = 1.0f; // Move Forward (Up on screen)
		else if (MouseY >= ViewportSizeY - EdgeScrollThreshold) 
			ScrollY = -1.0f; // Move Backward (Down on screen)

		if (ScrollX != 0.0f || ScrollY != 0.0f)
		{
			FVector Forward = SpringArm->GetForwardVector();
			Forward.Z = 0.0f;
			Forward.Normalize();

			FVector Right = SpringArm->GetRightVector();
			Right.Z = 0.0f;
			Right.Normalize();

			FVector MoveDelta = (Forward * ScrollY + Right * ScrollX).GetSafeNormal() * EdgeScrollSpeed * DeltaTime;
			AddActorWorldOffset(MoveDelta, true);
		}
	}
}