#include "MingGoRTSUnit.h"
#include "Components/DecalComponent.h"

AMingGoRTSUnit::AMingGoRTSUnit()
{
    bIsSelected = false;
    bIsMoving = false;
    TargetMoveLocation = FVector::ZeroVector;

    // 創建選擇貼花組件
    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecal->SetupAttachment(RootComponent);
    SelectionDecal->SetVisibility(false);
    
    // 設置選擇貼花
    SelectionDecal->DecalSize = FVector(64.0f, 64.0f, 64.0f);
    SelectionDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
}

void AMingGoRTSUnit::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Log, TEXT("MingGoRTS Unit Started"));
}

void AMingGoRTSUnit::SetSelected(bool bNewIsSelected)
{
    bIsSelected = bNewIsSelected;
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(bIsSelected);
    }
}

void AMingGoRTSUnit::MoveToLocation(const FVector& TargetLocation)
{
    this->TargetMoveLocation = TargetLocation;
    bIsMoving = true;
    
    UE_LOG(LogTemp, Log, TEXT("Unit moving to location: %s"), *TargetLocation.ToString());
}
