// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - AR Controller Component

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MingRTSARController.generated.h"

/**
 * AR Gesture Types for interaction
 */
UENUM(BlueprintType)
enum class EARGestureType: uint8 {
    None                UMETA(DisplayName = "None"),
    Tap                 UMETA(DisplayName = "Tap"),
    DoubleTap           UMETA(DisplayName = "Double Tap"),
    LongPress           UMETA(DisplayName = "Long Press"),
    Swipe               UMETA(DisplayName = "Swipe"),
    Pinch               UMETA(DisplayName = "Pinch"),
    Pan                 UMETA(DisplayName = "Pan"),
    Rotate              UMETA(DisplayName = "Rotate"),
    TwoFingerTap        UMETA(DisplayName = "Two Finger Tap"),
    TwoFingerSwipe      UMETA(DisplayName = "Two Finger Swipe")
};

/**
 * AR Interaction Modes
 */
UENUM(BlueprintType)
enum class EARInteractionMode: uint8 {
    Touch               UMETA(DisplayName = "Touch"),
    AirTap              UMETA(DisplayName = "Air Tap"),
    HandGesture         UMETA(DisplayName = "Hand Gesture"),
    Controller          UMETA(DisplayName = "Controller"),
    Voice               UMETA(DisplayName = "Voice"),
    EyeTracking         UMETA(DisplayName = "Eye Tracking"),
    Combined            UMETA(DisplayName = "Combined")
};

/**
 * AR Controller Input State
 */
USTRUCT(BlueprintType)
struct FARControllerState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    bool bIsActive = false;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    FVector PointerPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    FVector PointerDirection;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    bool bIsTouching = false;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    FVector2D TouchPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    FVector2D LastTouchPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    float TouchPressure = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    float TouchDuration = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    bool bIsPinching = false;

    UPROPERTY(BlueprintReadOnly, Category = "AR Controller")
    float PinchScale = 1.0f;
};

/**
 * AR Gesture Event Data
 */
USTRUCT(BlueprintType)
struct FARGestureEvent
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    EARGestureType GestureType = EARGestureType::None;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    FVector2D StartPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    FVector2D EndPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    FVector3D WorldStartPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    FVector3D WorldEndPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    float Duration = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    float Velocity = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "AR Gesture")
    int32 FingerCount = 1;
};

/**
 * AR Virtual Object Info
 */
USTRUCT(BlueprintType)
struct FARVirtualObject
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    FString ObjectID;

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    FVector WorldPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    FRotator WorldRotation;

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    FVector Scale = FVector(1.0f);

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    bool bIsSelectable = true;

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    bool bIsMovable = true;

    UPROPERTY(BlueprintReadOnly, Category = "AR Object")
    AActor* AssociatedActor = nullptr;
};

/**
 * MingGoRTS AR Controller
 * Handles AR input gestures and RTS-specific interactions
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSARController : public UActorComponent
{
    GENERATED_BODY()

public:
    // Sets default values for this component's properties
    UMingRTSARController();

    // Initialization
    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    void InitializeController();

    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    void ShutdownController();

    // Controller State
    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    bool IsControllerActive() const { return CurrentState.bIsActive; }

    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    FARControllerState GetControllerState() const { return CurrentState; }

    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    EARInteractionMode GetInteractionMode() const { return CurrentInteractionMode; }

    UFUNCTION(BlueprintCallable, Category = "AR Controller")
    void SetInteractionMode(EARInteractionMode Mode);

    // Touch/Gesture Handling
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Input")
    void ProcessTouchInput(const FVector2D& ScreenPosition, bool bIsPressed, float Pressure = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Input")
    void ProcessPinchGesture(float Scale);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Input")
    void ProcessRotationGesture(float Rotation);

    // RTS Commands
    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void SelectUnitAtTouch(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void SelectUnitAtPointer();

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void MoveSelectedUnits(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void MoveSelectedUnitsToWorld(const FVector& WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void CommandAttack(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void OpenContextMenu(const FVector2D& ScreenPosition);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|RTS")
    void SpawnUnitAtPlane(const FVector& PlaneLocation, TSubclassOf<AActor> UnitClass);

    // AR Raycasting
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Raycast")
    bool PerformScreenRaycast(const FVector2D& ScreenPosition, FHitResult& OutHit);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Raycast")
    bool PerformWorldRaycast(const FVector& Start, const FVector& Direction, FHitResult& OutHit);

    // Virtual Object Management
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Objects")
    void RegisterVirtualObject(const FARVirtualObject& Object);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Objects")
    void UnregisterVirtualObject(const FString& ObjectID);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Objects")
    TArray<FARVirtualObject> GetVirtualObjectsInView() const;

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Objects")
    FARVirtualObject* GetVirtualObjectAtScreenPosition(const FVector2D& ScreenPosition);

    // Camera Control
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Camera")
    void PanCamera(const FVector2D& Delta);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Camera")
    void ZoomCamera(float Delta);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Camera")
    void RotateCamera(float DeltaRotation);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Camera")
    void ResetCameraToDefault();

    // Haptic Feedback
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Feedback")
    void PlayHapticFeedback(float Intensity, float Duration);

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Feedback")
    void PlaySelectionFeedback();

    UFUNCTION(BlueprintCallable, Category = "AR Controller|Feedback")
    void PlayCommandFeedback();

    // Gesture Recognition
    UFUNCTION(BlueprintCallable, Category = "AR Controller|Gestures")
    void RecognizeGesture(const FARGestureEvent& Gesture);

    UPROPERTY(BlueprintAssignable, Category = "AR Controller|Events")
    FOnARGestureRecognized OnGestureRecognized;

    UPROPERTY(BlueprintAssignable, Category = "AR Controller|Events")
    FOnARUnitSelected OnUnitSelected;

    UPROPERTY(BlueprintAssignable, Category = "AR Controller|Events")
    FOnARUnitsMoved OnUnitsMoved;

    UPROPERTY(BlueprintAssignable, Category = "AR Controller|Events")
    FOnARCommandIssued OnCommandIssued;

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
                               FActorComponentTickFunction* ThisTickFunction) override;

    void UpdateControllerState(float DeltaTime);
    void ProcessGestures(float DeltaTime);
    void HandleTouchGesture();
    void HandlePinchGesture();
    void HandlePanGesture();
    void HandleRotateGesture();
    void PerformVirtualObjectHitTest();
    void UpdatePointerVisuals();

private:
    UPROPERTY()
    EARInteractionMode CurrentInteractionMode = EARInteractionMode::Touch;

    UPROPERTY()
    FARControllerState CurrentState;

    UPROPERTY()
    TArray<FARVirtualObject> RegisteredVirtualObjects;

    UPROPERTY()
    TArray<FARGestureEvent> GestureHistory;

    UPROPERTY()
    float LongPressThreshold = 0.5f;

    UPROPERTY()
    float DoubleTapThreshold = 0.3f;

    UPROPERTY()
    float SwipeVelocityThreshold = 500.0f;

    UPROPERTY()
    float CameraPanSpeed = 1000.0f;

    UPROPERTY()
    float CameraZoomSpeed = 500.0f;

    UPROPERTY()
    float CameraRotationSpeed = 90.0f;

    UPROPERTY()
    bool bIsInitialized = false;

    UPROPERTY()
    float HapticTimer = 0.0f;

    UPROPERTY()
    FVector CameraDefaultPosition;

    UPROPERTY()
    FRotator CameraDefaultRotation;
};

// Event delegate declarations
declare dynamic multicast_delegate FOnARGestureRecognized(const FARGestureEvent& Gesture);
declare dynamic multicast_delegate FOnARUnitSelected(AActor* SelectedUnit);
declare dynamic multicast_delegate FOnARUnitsMoved(const FVector& TargetLocation, const TArray<AActor*>& Units);
declare dynamic multicast_delegate FOnARCommandIssued(const FString& CommandType, const FVector& TargetLocation);
