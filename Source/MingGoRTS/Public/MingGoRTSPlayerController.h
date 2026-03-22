#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MingGoRTSPlayerController.generated.h"

UCLASS()
class MINGRTS_API AMingGoRTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMingGoRTSPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;

    // 輸入處理
    void OnLeftMouseButtonPressed();
    void OnLeftMouseButtonReleased();
    void OnRightMouseButtonPressed();
    void OnMouseScrollUp();
    void OnMouseScrollDown();
    void OnMoveCameraForward(float Value);
    void OnMoveCameraRight(float Value);

public:
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void StartSelection();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void EndSelection();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnitsInRect(FVector2D Start, FVector2D End);

    UFUNCTION(BlueprintCallable, Category = "Command")
    void MoveSelectedUnits(FVector Location);

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ZoomCamera(float Delta);

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectSameType();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectDamaged();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectIdle();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void DeselectAll();

private:
    UPROPERTY()
    bool bIsSelecting;

    UPROPERTY()
    bool bIsAdditiveSelection;

    UPROPERTY()
    float LastClickTime;

    float DoubleClickThreshold;

    UPROPERTY()
    FVector2D SelectionStart;

    UPROPERTY()
    FVector2D SelectionEnd;

    UPROPERTY()
    TArray<class AMingGoRTSUnit*> SelectedUnits;

    UPROPERTY()
    class AMingGoRTSCamera* RTSCamera;
};


#endif // MINGGORTSPLAYERCONTROLLER_H
