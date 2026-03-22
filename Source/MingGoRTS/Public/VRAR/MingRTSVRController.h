// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR Controller Component

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingRTSVRController.generated.h"

/**
 * VR Controller Types
 */
UENUM(BlueprintType)
enum class EVRControllerType : uint8
{
    LeftHand        UMETA(DisplayName = "Left Hand"),
    RightHand       UMETA(DisplayName = "Right Hand"),
    Head            UMETA(DisplayName = "Head/HMD"),
    TrackingDevice  UMETA(DisplayName = "Tracking Device")
};

/**
 * VR Interaction Modes
 */
UENUM(BlueprintType)
enum class EVRInteractionMode : uint8
{
    Pointer     UMETA(DisplayName = "Pointer/Laser"),
    DirectTouch UMETA(DisplayName = "Direct Touch"),
    Grab        UMETA(DisplayName = "Grab"),
    UIOnly      UMETA(DisplayName = "UI Interaction Only")
};

/**
 * VR Controller Button States
 */
USTRUCT(BlueprintType)
struct FVRControllerState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    EVRControllerType ControllerType = EVRControllerType::LeftHand;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    FVector Position = FVector::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    FRotator Rotation = FRotator::ZeroRotator;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    bool bIsTracking = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    bool bTriggerPressed = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    float TriggerAxis = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    bool bGripPressed = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    FVector2D Thumbstick = FVector2D::ZeroVector;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    bool bThumbstickPressed = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    bool bMenuButtonPressed = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Controller")
    float BatteryLevel = 1.0f;
};

/**
 * VR Input Action Configuration
 */
USTRUCT(BlueprintType)
struct FVRInputAction
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Input")
    FName ActionName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Input")
    EVRControllerType Controller = EVRControllerType::LeftHand;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Input")
    FName InputName; // e.g., "Trigger", "Grip", "Thumbstick"

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Input")
    bool bIsAxisInput = false;
};

/**
 * MingGoRTS VR Controller Component
 * Handles VR controller tracking, input, and interaction
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MINGGORTS_API UMingRTSVRController : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMingRTSVRController();

    // Called every frame
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
                               FActorComponentTickFunction* ThisTickFunction) override;

    // Controller Management
    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    void InitializeController(EVRControllerType Type);

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    void ShutdownController();

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    bool IsControllerTracking() const;

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    FVRControllerState GetControllerState() const { return CurrentState; }

    // Position and Rotation
    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    FVector GetControllerPosition() const;

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    FRotator GetControllerRotation() const;

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    FTransform GetControllerTransform() const;

    // Interaction
    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    void SetInteractionMode(EVRInteractionMode Mode);

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    EVRInteractionMode GetCurrentInteractionMode() const { return CurrentInteractionMode; }

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    bool GetPointerLocation(FVector& OutLocation, FVector& OutDirection) const;

    // Haptic Feedback
    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    void PlayHapticEffect(float Intensity, float Duration, bool bIsHand = true);

    UFUNCTION(BlueprintCallable, Category = "VR Controller")
    void StopHapticEffect();

    // RTS Specific Functions
    UFUNCTION(BlueprintCallable, Category = "VR Controller|RTS")
    void SelectUnitAtPointer();

    UFUNCTION(BlueprintCallable, Category = "VR Controller|RTS")
    void MoveSelectedUnits();

    UFUNCTION(BlueprintCallable, Category = "VR Controller|RTS")
    void OpenRadialMenu();

    UFUNCTION(BlueprintCallable, Category = "VR Controller|RTS")
    void PanCamera();

    // Input Handling
    UFUNCTION(BlueprintCallable, Category = "VR Controller|Input")
    void RegisterInputAction(const FVRInputAction& Action);

    UFUNCTION(BlueprintCallable, Category = "VR Controller|Input")
    void UnregisterInputAction(FName ActionName);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnControllerTrackingStateChanged OnTrackingStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnControllerButtonPressed OnButtonPressed;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnControllerButtonReleased OnButtonReleased;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnTriggerPressed OnTriggerPressed;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnTriggerReleased OnTriggerReleased;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnUnitSelected OnUnitSelected;

    UPROPERTY(BlueprintAssignable, Category = "VR Controller|Events")
    FOnUnitsMoved OnUnitsMoved;

protected:
    // Called when the game starts
    virtual void BeginPlay() override;

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
    UPROPERTY()
    EVRControllerType ControllerType = EVRControllerType::LeftHand;

    UPROPERTY()
    FVRControllerState CurrentState;

    UPROPERTY()
    EVRInteractionMode CurrentInteractionMode = EVRInteractionMode::Pointer;

    UPROPERTY()
    TArray<FVRInputAction> RegisteredActions;

    UPROPERTY()
    bool bIsInitialized = false;

    UPROPERTY()
    float HapticTimer = 0.0f;

    void UpdateControllerState(float DeltaTime);
    void ProcessInput();
    void UpdatePointerVisuals();
    void HandleRTSInput();
    void PerformLineTraceForSelection(FHitResult& OutHit);
    
    // Cached motion controller component reference
    UPROPERTY()
    TObjectPtr<class UMotionControllerComponent> MotionControllerComponent;
};

// Event delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnControllerTrackingStateChanged, bool, bIsTracking);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnControllerButtonPressed, EVRControllerType, Controller, FName, ButtonName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnControllerButtonReleased, EVRControllerType, Controller, FName, ButtonName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerPressed, EVRControllerType, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerReleased, EVRControllerType, Controller);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnitSelected, AActor*, SelectedUnit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUnitsMoved, const FVector&, MoveLocation, const TArray<AActor*>&, Units);

