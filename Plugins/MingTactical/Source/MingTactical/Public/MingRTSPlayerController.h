#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "MingRTSPlayerController.generated.h"

UCLASS()
class MINGTACTICAL_API AMingRTSPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    AMingRTSPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;
    virtual void PlayerTick(float DeltaTime) override;

    // Enhanced Input Actions
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SelectAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SelectCompleteAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* AttackAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputAction* SelectAllAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    class UInputMappingContext* RTSMappingContext;

    // Input handlers
    UFUNCTION()
    void OnSelectStarted(const FInputActionValue& Value);

    UFUNCTION()
    void OnSelectTriggered(const FInputActionValue& Value);

    UFUNCTION()
    void OnSelectCompleted(const FInputActionValue& Value);

    UFUNCTION()
    void OnMoveCommand(const FInputActionValue& Value);

    UFUNCTION()
    void OnAttackCommand(const FInputActionValue& Value);

    UFUNCTION()
    void OnSelectAllUnits(const FInputActionValue& Value);

    // Selection Management
    UPROPERTY(BlueprintReadOnly, Category = "Selection")
    class UMingSelectionManager* SelectionManager;

    // Camera Control
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float CameraMoveSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
    float EdgeScrollingThreshold;

    void UpdateCameraMovement(float DeltaTime);
    void HandleEdgeScrolling();

    // Command System
    UFUNCTION(BlueprintCallable, Category = "Commands")
    void IssueMoveCommand(const FVector& TargetLocation, bool bIsAttackMove = false);

    UFUNCTION(BlueprintCallable, Category = "Commands")
    void IssueAttackCommand(AActor* Target);

    UFUNCTION(BlueprintCallable, Category = "Commands")
    void IssueStopCommand();

    UFUNCTION(BlueprintCallable, Category = "Commands")
    void IssueHoldPositionCommand();

    // Utility functions
    FVector GetMouseWorldLocation() const;
    bool GetMouseWorldLocationAndNormal(FVector& OutLocation, FVector& OutNormal) const;
    AActor* GetActorUnderMouse() const;

    // Input state tracking
    bool bIsSelecting;
    bool bIsDragging;
    FVector2D CurrentMousePosition;
    FVector2D SelectionStartPosition;

    // Unit Control Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitsMoveCommand, const FVector&, TargetLocation);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitsAttackCommand, AActor*, Target);

    UPROPERTY(BlueprintAssignable, Category = "Commands|Events")
    FOnUnitsMoveCommand OnUnitsMoveCommand;

    UPROPERTY(BlueprintAssignable, Category = "Commands|Events")
    FOnUnitsAttackCommand OnUnitsAttackCommand;

public:
    UFUNCTION(BlueprintPure, Category = "Selection")
    UMingSelectionManager* GetSelectionManager() const { return SelectionManager; }

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void EnableDebugVisualization(bool bEnable);

    UFUNCTION(BlueprintPure, Category = "Debug")
    bool IsDebugVisualizationEnabled() const;
};
