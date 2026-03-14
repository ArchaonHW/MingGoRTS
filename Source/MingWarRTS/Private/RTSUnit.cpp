#include "RTSUnit.h"
#include "Components/InputComponent.h"
#include "Components/DecalComponent.h"
#include "Components/StaticMeshComponent.h"

ARTSUnit::ARTSUnit()
{
    PrimaryActorTick.bCanEverTick = true;
    bSelected = false;
    SupplyLevel = 100.0f;
    BaseAttackPower = 10.0f; // Initial placeholder value
    BaseDefensePower = 5.0f; // Initial placeholder value

    // 建立單位的 3D 模型元件 (Root)
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitMesh"));
    RootComponent = MeshComponent;
    
    // 設定單位的碰撞類型為 Pawn，這樣射線 (Raycast) 才能打中它
    MeshComponent->SetCollisionProfileName(TEXT("Pawn"));
    
    // 建立腳底選取圈的貼花 (Decal)
    SelectionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("SelectionDecal"));
    SelectionDecal->SetupAttachment(RootComponent);
    // 貼花需要朝向正下方 (Pitch = -90) 才能投影在地板上
    SelectionDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
    SelectionDecal->DecalSize = FVector(32.0f, 64.0f, 64.0f);
    SelectionDecal->SetVisibility(false); // 預設隱藏
}

void ARTSUnit::BeginPlay()
{
    Super::BeginPlay();
}

void ARTSUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ARTSUnit::SetSelected(bool bIsSelected)
{
    bSelected = bIsSelected;
    
    // 開啟或關閉腳底的圈圈
    if (SelectionDecal)
    {
        SelectionDecal->SetVisibility(bSelected);
    }
}

void ARTSUnit::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    
    // 將輸入軸綁定到我們的移動函數上
    PlayerInputComponent->BindAxis("MoveForward", this, &ARTSUnit::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ARTSUnit::MoveRight);
}

void ARTSUnit::MoveForward(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 找到哪個方向是 "前方"
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // 獲取前向向量
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ARTSUnit::MoveRight(float Value)
{
    if ((Controller != nullptr) && (Value != 0.0f))
    {
        // 找到哪個方向是 "右方"
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
    
        // 獲取右向向量
        const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}