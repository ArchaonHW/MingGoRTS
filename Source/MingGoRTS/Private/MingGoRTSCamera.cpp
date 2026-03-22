#include "MingGoRTSCamera.h"

AMingGoRTSCamera::AMingGoRTSCamera()
{
    PrimaryActorTick.bCanEverTick = true;

    // 創建組件
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    RootComponent = SpringArm;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);

    // 設置預設值
    SpringArm->TargetArmLength = 1000.0f;
    SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = false;
    SpringArm->bUsePawnControlRotation = false;

    DefaultDistance = 1000.0f;

    AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AMingGoRTSCamera::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Camera Started"));
}

void AMingGoRTSCamera::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMingGoRTSCamera::SetCameraDistance(float Distance)
{
    if (SpringArm)
    {
        SpringArm->TargetArmLength = FMath::Clamp(Distance, 500.0f, 3000.0f);
    }
}

void AMingGoRTSCamera::MoveCamera(const FVector& Direction)
{
    if (SpringArm)
    {
        AddActorWorldOffset(Direction * GetWorld()->GetDeltaSeconds() * 500.0f, true);
    }
}
