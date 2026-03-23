// Copyright (c) 2026 MingGoRTS. All rights reserved.

#include "MingGoRTS.h"
#include "Units/MingGoRTSUnit.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"

AMingGoRTSUnit::AMingGoRTSUnit()
{
    PrimaryActorTick.bCanEverTick = true;

    // 初始化單位屬性
    UnitName = TEXT("Unit");
    UnitType = EUnitType::Infantry;
    Health = 100;
    MaxHealth = 100;
    MoveSpeed = 300.0f;
    AttackPower = 10;
    bSelected = false;

    // 設置移動速度
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MaxWalkSpeed = MoveSpeed;
    }

    // 創建選中指示器
    SelectionIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionIndicator"));
    SelectionIndicator->SetupAttachment(RootComponent);
    SelectionIndicator->SetVisibility(false);
}

void AMingGoRTSUnit::BeginPlay()
{
    Super::BeginPlay();

    UE_LOG(LogTemp, Log, TEXT("Unit %s spawned"), *UnitName);
}

void AMingGoRTSUnit::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMingGoRTSUnit::SelectUnit()
{
    bSelected = true;
    UpdateSelectionVisual();

    UE_LOG(LogTemp, Log, TEXT("Unit %s selected"), *UnitName);
}

void AMingGoRTSUnit::DeselectUnit()
{
    bSelected = false;
    UpdateSelectionVisual();
}

void AMingGoRTSUnit::MoveToLocation(const FVector& Destination)
{
    // 設置移動目標
    if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
    {
        MoveComp->MoveToLocation(Destination);
    }

    UE_LOG(LogTemp, Log, TEXT("Unit %s moving to %s"), *UnitName, *Destination.ToString());
}

void AMingGoRTSUnit::Attack(AActor* Target)
{
    if (Target)
    {
        UE_LOG(LogTemp, Log, TEXT("Unit %s attacking %s"), *UnitName, *Target->GetName());
    }
}

void AMingGoRTSUnit::TakeDamage(int32 Damage)
{
    Health = FMath::Max(0, Health - Damage);

    UE_LOG(LogTemp, Log, TEXT("Unit %s took %d damage, Health: %d/%d"),
        *UnitName, Damage, Health, MaxHealth);

    if (Health <= 0)
    {
        // 單位死亡
        UE_LOG(LogTemp, Log, TEXT("Unit %s destroyed"), *UnitName);
        Destroy();
    }
}

void AMingGoRTSUnit::UpdateSelectionVisual()
{
    if (SelectionIndicator)
    {
        SelectionIndicator->SetVisibility(bSelected);
    }
}
