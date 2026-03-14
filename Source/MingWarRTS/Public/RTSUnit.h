#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h" // Changed from Actor.h
#include "RTSUnit.generated.h"

class UDecalComponent;
class UStaticMeshComponent;

UCLASS()
class MINGWARRTS_API ARTSUnit : public APawn // Changed from AActor
{
    GENERATED_BODY()

public:
    ARTSUnit();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override; // 添加這行來處理玩家輸入

    // 選取功能 (給 C++ 和藍圖呼叫)
    UFUNCTION(BlueprintCallable, Category = "RTS Selection")
    void SetSelected(bool bIsSelected);

    UFUNCTION(BlueprintPure, Category = "RTS Selection")
    bool IsSelected() const { return bSelected; }

    // The current supply level of this unit (0.0 to 100.0)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "RTS Stats")
    float SupplyLevel;

    // Base combat stats
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Stats")
    float BaseAttackPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTS Stats")
    float BaseDefensePower;

protected:
    // 處理前後移動輸入
    void MoveForward(float Value);
    // 處理左右移動輸入
    void MoveRight(float Value);
    // 單位的外觀模型
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    // 單位腳底的選取圈 (Decal)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UDecalComponent* SelectionDecal;

private:
    bool bSelected;
};