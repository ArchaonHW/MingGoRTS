#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MingGoRTSPlayerController.generated.h"

UCLASS()
class MINGGORTS_API AMingGoRTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMingGoRTSPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;

    // è¼¸å…¥?•ç?
    void OnLeftMouseButtonPressed();
    void OnLeftMouseButtonReleased();
    void OnRightMouseButtonPressed();
    void OnMouseScrollUp();
    void OnMouseScrollDown();
    void OnMoveCameraForward(float Value};
    void OnMoveCameraRight(float Value};

public:
    UFUNCTION(BlueprintCallable, Category = "Selection")
    void StartSelection();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void EndSelection();

    UFUNCTION(BlueprintCallable, Category = "Selection")
    void SelectUnitsInRect(FVector2D Start, FVector2D End};

    UFUNCTION(BlueprintCallable, Category = "Command")
    void MoveSelectedUnits(FVector Location};

    UFUNCTION(BlueprintCallable, Category = "Camera")
    void ZoomCamera(float Delta};

private:
    UPROPERTY()
    bool bIsSelecting;

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
