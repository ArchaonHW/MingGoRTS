// Copyright (c) 2026 MingGoRTS. All rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MingGoRTSPlayerController.generated.h"

/**
 * MingGoRTS 玩家控制器
 * 處理玩家輸入和遊戲互動
 */
UCLASS(MinimalAPI, BlueprintType, Blueprintable)
class AMingGoRTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMingGoRTSPlayerController();

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;

    /** 選擇單位 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void SelectUnit(class AMingGoRTSUnit* Unit);

    /** 選擇多個單位 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void SelectUnits(const TArray<class AMingGoRTSUnit*>& Units);

    /** 移動選中單位 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void MoveSelectedUnits(const FVector& Destination);

    /** 攻擊目標 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void AttackTarget(class AActor* Target);

    /** 打開選單 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void OpenMenu();

    /** 關閉選單 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void CloseMenu();

    /** 切換選單 */
    UFUNCTION(BlueprintCallable, Category = "MingGoRTS Player")
    void ToggleMenu();

    /** 取得選中單位 */
    UFUNCTION(BlueprintPure, Category = "MingGoRTS Player")
    const TArray<class AMingGoRTSUnit*>& GetSelectedUnits() const { return SelectedUnits; }

protected:
    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS Player")
    TArray<class AMingGoRTSUnit*> SelectedUnits;

    UPROPERTY(BlueprintReadOnly, Category = "MingGoRTS Player")
    bool bMenuOpen;

    /** 滑鼠左鍵點擊 */
    void OnLeftMouseClick();

    /** 滑鼠右鍵點擊 */
    void OnRightMouseClick();

    /** 選框選擇 */
    void OnBoxSelect();

    /** 取消選擇 */
    void OnDeselect();

    /** 打開選單輸入 */
    void OnOpenMenuInput();

    /** 取得滑鼠位置對應的世界位置 */
    bool GetMouseWorldPosition(FVector& OutWorldPosition);

    /** 取得滑鼠下方的單位 */
    class AMingGoRTSUnit* GetUnitUnderMouse();
};
