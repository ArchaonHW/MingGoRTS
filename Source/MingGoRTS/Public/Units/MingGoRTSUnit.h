// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MingGoRTSUnit.generated.h"

/**
 * MingGoRTS 基礎單位類別
 * 所有 RTS 單位的基礎類
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class AMingGoRTSUnit : public ACharacter
{
    GENERATED_BODY()

public:
    AMingGoRTSUnit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    /** 單位名稱 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    FString UnitName;

    /** 單位類型 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    EUnitType UnitType;

    /** 生命值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    int32 Health;

    /** 最大生命值 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    int32 MaxHealth;

    /** 移動速度 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    float MoveSpeed;

    /** 攻擊力 */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MingGoRTS Unit")
    int32 AttackPower;

    /** 選中單位 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Unit")
    void SelectUnit();

    /** 取消選中 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Unit")
    void DeselectUnit();

    /** 移動到位置 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Unit")
    void MoveToLocation(const FVector& Destination);

    /** 攻擊目標 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Unit")
    void Attack(AActor* Target);

    /** 受到傷害 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Unit")
    void TakeDamage(int32 Damage);

    /** 是否被選中 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS Unit")
    bool IsSelected() const { return bSelected; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS Unit")
    bool bSelected;

    UPROPERTY(EditDefaultsOnly, Category = "MingGoRTS Unit")
    UStaticMeshComponent* SelectionIndicator;

    void UpdateSelectionVisual();
};

UENUM(BlueprintType)
enum class EUnitType : uint8
{
    Infantry,       // 步兵
    Cavalry,        // 騎兵
    Archer,         // 弓箭手
    Siege,          // 攻城單位
    Support,        // 支援單位
    Hero,           // 英雄單位
    Building,       // 建築
    Resource        // 資源點
};
