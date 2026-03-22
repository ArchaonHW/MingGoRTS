// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - AR Controller Implementation

#include "VRAR/MingRTSARController.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingARController, Log, All);

UMingRTSARController::UMingRTSARController()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UMingRTSARController::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogMingARController, Log, TEXT("AR Controller BeginPlay"));
}

void UMingRTSARController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogMingARController, Log, TEXT("AR Controller EndPlay"));
    ShutdownController();
    Super::EndPlay(EndPlayReason);
}

void UMingRTSARController::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!bIsInitialized)
    {
        return;
    }

    UpdateControllerState(DeltaTime);
    ProcessGestures(DeltaTime);
    UpdatePointerVisuals();

    if (HapticTimer > 0.0f)
    {
        HapticTimer -= DeltaTime;
        if (HapticTimer <= 0.0f)
        {
            HapticTimer = 0.0f;
        }
    }
}

void UMingRTSARController::InitializeController()
{
    bIsInitialized = true;
    CurrentState.bIsActive = true;

    UE_LOG(LogMingARController, Log, TEXT("AR Controller initialized"));
}

void UMingRTSARController::ShutdownController()
{
    bIsInitialized = false;
    CurrentState.bIsActive = false;

    UE_LOG(LogMingARController, Log, TEXT("AR Controller shutdown"));
}

void UMingRTSARController::SetInteractionMode(EARInteractionMode Mode)
{
    CurrentInteractionMode = Mode;
    UE_LOG(LogMingARController, Log, TEXT("Interaction mode changed to: %s"),
           *UEnum::GetValueAsString(Mode));
}

void UMingRTSARController::ProcessTouchInput(const FVector2D& ScreenPosition, bool bIsPressed, float Pressure)
{
    if (!bIsInitialized)
    {
        return;
    }

    if (bIsPressed)
    {
        // Touch started or ongoing
        if (!CurrentState.bIsTouching)
        {
            // Touch started
            CurrentState.LastTouchPosition = ScreenPosition;
            CurrentState.TouchDuration = 0.0f;
        }
        CurrentState.bIsTouching = true;
        CurrentState.TouchPosition = ScreenPosition;
        CurrentState.TouchPressure = Pressure;
    }
    else
    {
        // Touch ended - check for gestures
        if (CurrentState.bIsTouching)
        {
            HandleTouchGesture();
        }
        CurrentState.bIsTouching = false;
        CurrentState.TouchDuration = 0.0f;
    }
}

void UMingRTSARController::ProcessPinchGesture(float Scale)
{
    if (!bIsInitialized)
    {
        return;
    }

    CurrentState.bIsPinching = (Scale != 1.0f);
    CurrentState.PinchScale = Scale;

    if (CurrentState.bIsPinching)
    {
        HandlePinchGesture();
    }
}

void UMingRTSARController::ProcessRotationGesture(float Rotation)
{
    if (!bIsInitialized)
    {
        return;
    }

    HandleRotateGesture();
}

void UMingRTSARController::SelectUnitAtTouch(const FVector2D& ScreenPosition)
{
    FHitResult HitResult;
    if (PerformScreenRaycast(ScreenPosition, HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogMingARController, Log, TEXT("Selected unit at touch: %s"), *HitActor->GetName());
            OnUnitSelected.Broadcast(HitActor);
            PlaySelectionFeedback();
        }
    }
}

void UMingRTSARController::SelectUnitAtPointer()
{
    SelectUnitAtTouch(CurrentState.TouchPosition);
}

void UMingRTSARController::MoveSelectedUnits(const FVector2D& ScreenPosition)
{
    FHitResult HitResult;
    if (PerformScreenRaycast(ScreenPosition, HitResult))
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        TArray<AActor*> SelectedUnits; // This would come from your selection manager

        UE_LOG(LogMingARController, Log, TEXT("Moving units to: %s"), *TargetLocation.ToString());
        OnUnitsMoved.Broadcast(TargetLocation, SelectedUnits);
        PlayCommandFeedback();
    }
}

void UMingRTSARController::MoveSelectedUnitsToWorld(const FVector& WorldLocation)
{
    TArray<AActor*> SelectedUnits;
    UE_LOG(LogMingARController, Log, TEXT("Moving units to world location: %s"), *WorldLocation.ToString());
    OnUnitsMoved.Broadcast(WorldLocation, SelectedUnits);
    PlayCommandFeedback();
}

void UMingRTSARController::CommandAttack(const FVector2D& ScreenPosition)
{
    FHitResult HitResult;
    if (PerformScreenRaycast(ScreenPosition, HitResult))
    {
        FVector TargetLocation = HitResult.ImpactPoint;
        UE_LOG(LogMingARController, Log, TEXT("Attack command at: %s"), *TargetLocation.ToString());
        OnCommandIssued.Broadcast(TEXT("Attack"), TargetLocation);
        PlayCommandFeedback();
    }
}

void UMingRTSARController::OpenContextMenu(const FVector2D& ScreenPosition)
{
    UE_LOG(LogMingARController, Log, TEXT("Opening context menu at: %s"), *ScreenPosition.ToString());
    OnCommandIssued.Broadcast(TEXT("ContextMenu"), FVector(ScreenPosition.X, ScreenPosition.Y, 0));
    PlayHapticFeedback(0.3f, 0.1f);
}

void UMingRTSARController::SpawnUnitAtPlane(const FVector& PlaneLocation, TSubclassOf<AActor> UnitClass)
{
    if (!UnitClass)
    {
        UE_LOG(LogMingARController, Warning, TEXT("Cannot spawn unit: Invalid unit class"));
        return;
    }

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = GetOwner();

    AActor* SpawnedUnit = World->SpawnActor<AActor>(UnitClass, PlaneLocation, FRotator::ZeroRotator, SpawnParams);
    if (SpawnedUnit)
    {
        UE_LOG(LogMingARController, Log, TEXT("Spawned unit at: %s"), *PlaneLocation.ToString());
        PlayCommandFeedback();
    }
}

bool UMingRTSARController::PerformScreenRaycast(const FVector2D& ScreenPosition, FHitResult& OutHit)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return false;
    }

    FVector WorldOrigin;
    FVector WorldDirection;

    if (PlayerController->DeprojectScreenPositionToWorld(ScreenPosition.X, ScreenPosition.Y, WorldOrigin, WorldDirection))
    {
        FVector TraceEnd = WorldOrigin + (WorldDirection * 10000.0f);

        FCollisionQueryParams QueryParams;
        QueryParams.bTraceComplex = true;
        QueryParams.bReturnPhysicalMaterial = false;

        return GetWorld()->LineTraceSingleByChannel(OutHit, WorldOrigin, TraceEnd, ECC_Visibility, QueryParams);
    }

    return false;
}

bool UMingRTSARController::PerformWorldRaycast(const FVector& Start, const FVector& Direction, FHitResult& OutHit)
{
    FVector TraceEnd = Start + (Direction * 10000.0f);

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;

    return GetWorld()->LineTraceSingleByChannel(OutHit, Start, TraceEnd, ECC_Visibility, QueryParams);
}

void UMingRTSARController::RegisterVirtualObject(const FARVirtualObject& Object)
{
    // Check if object already exists
    for (int32 i = 0; i < RegisteredVirtualObjects.Num(); ++i)
    {
        if (RegisteredVirtualObjects[i].ObjectID == Object.ObjectID)
        {
            RegisteredVirtualObjects[i] = Object;
            UE_LOG(LogMingARController, Log, TEXT("Updated virtual object: %s"), *Object.ObjectID);
            return;
        }
    }

    RegisteredVirtualObjects.Add(Object);
    UE_LOG(LogMingARController, Log, TEXT("Registered virtual object: %s"), *Object.ObjectID);
}

void UMingRTSARController::UnregisterVirtualObject(const FString& ObjectID)
{
    for (int32 i = RegisteredVirtualObjects.Num() - 1; i >= 0; --i)
    {
        if (RegisteredVirtualObjects[i].ObjectID == ObjectID)
        {
            RegisteredVirtualObjects.RemoveAt(i);
            UE_LOG(LogMingARController, Log, TEXT("Unregistered virtual object: %s"), *ObjectID);
            return;
        }
    }
}

TArray<FARVirtualObject> UMingRTSARController::GetVirtualObjectsInView() const
{
    TArray<FARVirtualObject> Result;

    for (const FARVirtualObject& Object : RegisteredVirtualObjects)
    {
        // Check if object is in view (would need camera frustum check in production)
        Result.Add(Object);
    }

    return Result;
}

FARVirtualObject* UMingRTSARController::GetVirtualObjectAtScreenPosition(const FVector2D& ScreenPosition)
{
    FHitResult HitResult;
    if (PerformScreenRaycast(ScreenPosition, HitResult))
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            for (FARVirtualObject& Object : RegisteredVirtualObjects)
            {
                if (Object.AssociatedActor == HitActor)
                {
                    return &Object;
                }
            }
        }
    }

    return nullptr;
}

void UMingRTSARController::PanCamera(const FVector2D& Delta)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    // Get pawn and camera
    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn)
    {
        return;
    }

    FVector CurrentLocation = Pawn->GetActorLocation();
    FVector Forward = Pawn->GetActorForwardVector();
    FVector Right = Pawn->GetActorRightVector();

    // Calculate pan movement
    FVector PanDelta = (Forward * Delta.Y * CameraPanSpeed) + (Right * Delta.X * CameraPanSpeed);
    FVector NewLocation = CurrentLocation + PanDelta;

    Pawn->SetActorLocation(NewLocation);

    UE_LOG(LogMingARController, Verbose, TEXT("Camera panned by: %s"), *PanDelta.ToString());
}

void UMingRTSARController::ZoomCamera(float Delta)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn)
    {
        return;
    }

    FVector CurrentLocation = Pawn->GetActorLocation();
    FVector Forward = Pawn->GetActorForwardVector();

    // Zoom by moving forward/backward
    FVector ZoomDelta = Forward * Delta * CameraZoomSpeed;
    FVector NewLocation = CurrentLocation + ZoomDelta;

    Pawn->SetActorLocation(NewLocation);

    UE_LOG(LogMingARController, Verbose, TEXT("Camera zoomed by: %s"), *ZoomDelta.ToString());
}

void UMingRTSARController::RotateCamera(float DeltaRotation)
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn)
    {
        return;
    }

    FRotator CurrentRotation = Pawn->GetActorRotation();
    FRotator NewRotation = CurrentRotation;
    NewRotation.Yaw += DeltaRotation * CameraRotationSpeed;

    Pawn->SetActorRotation(NewRotation);

    UE_LOG(LogMingARController, Verbose, TEXT("Camera rotated by: %f degrees"), DeltaRotation * CameraRotationSpeed);
}

void UMingRTSARController::ResetCameraToDefault()
{
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (!PlayerController)
    {
        return;
    }

    APawn* Pawn = PlayerController->GetPawn();
    if (!Pawn)
    {
        return;
    }

    Pawn->SetActorLocation(CameraDefaultPosition);
    Pawn->SetActorRotation(CameraDefaultRotation);

    UE_LOG(LogMingARController, Log, TEXT("Camera reset to default"));
}

void UMingRTSARController::PlayHapticFeedback(float Intensity, float Duration)
{
    HapticTimer = Duration;

    // In production, this would trigger device haptic feedback
    UE_LOG(LogMingARController, Verbose, TEXT("Playing haptic: Intensity=%f, Duration=%f"), Intensity, Duration);
}

void UMingRTSARController::PlaySelectionFeedback()
{
    PlayHapticFeedback(0.3f, 0.05f);
}

void UMingRTSARController::PlayCommandFeedback()
{
    PlayHapticFeedback(0.5f, 0.1f);
}

void UMingRTSARController::RecognizeGesture(const FARGestureEvent& Gesture)
{
    GestureHistory.Add(Gesture);

    // Keep only recent gestures
    while (GestureHistory.Num() > 10)
    {
        GestureHistory.RemoveAt(0);
    }

    OnGestureRecognized.Broadcast(Gesture);

    UE_LOG(LogMingARController, Log, TEXT("Gesture recognized: %s"),
           *UEnum::GetValueAsString(Gesture.GestureType));
}

void UMingRTSARController::UpdateControllerState(float DeltaTime)
{
    if (CurrentState.bIsTouching)
    {
        CurrentState.TouchDuration += DeltaTime;
    }

    // Update pointer position based on controller
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PlayerController)
    {
        FVector WorldOrigin;
        FVector WorldDirection;

        if (PlayerController->DeprojectScreenPositionToWorld(
            CurrentState.TouchPosition.X,
            CurrentState.TouchPosition.Y,
            WorldOrigin,
            WorldDirection))
        {
            CurrentState.PointerPosition = WorldOrigin;
            CurrentState.PointerDirection = WorldDirection;
        }
    }
}

void UMingRTSARController::ProcessGestures(float DeltaTime)
{
    if (!CurrentState.bIsTouching)
    {
        return;
    }

    // Detect gestures based on touch movement and duration
    FVector2D TouchDelta = CurrentState.TouchPosition - CurrentState.LastTouchPosition;
    float TouchDistance = TouchDelta.Size();

    // Swipe detection
    if (TouchDistance > SwipeVelocityThreshold * DeltaTime)
    {
        FARGestureEvent Gesture;
        Gesture.GestureType = EARGestureType::Swipe;
        Gesture.StartPosition = CurrentState.LastTouchPosition;
        Gesture.EndPosition = CurrentState.TouchPosition;
        Gesture.Velocity = TouchDistance / DeltaTime;

        RecognizeGesture(Gesture);
    }

    // Long press detection
    if (CurrentState.TouchDuration >= LongPressThreshold && TouchDistance < 10.0f)
    {
        FARGestureEvent Gesture;
        Gesture.GestureType = EARGestureType::LongPress;
        Gesture.StartPosition = CurrentState.LastTouchPosition;
        Gesture.Duration = CurrentState.TouchDuration;

        RecognizeGesture(Gesture);
    }

    CurrentState.LastTouchPosition = CurrentState.TouchPosition;
}

void UMingRTSARController::HandleTouchGesture()
{
    FVector2D TouchDelta = CurrentState.TouchPosition - CurrentState.LastTouchPosition;
    float TouchDistance = TouchDelta.Size();
    float TouchTime = CurrentState.TouchDuration;

    FARGestureEvent Gesture;
    Gesture.StartPosition = CurrentState.LastTouchPosition;
    Gesture.EndPosition = CurrentState.TouchPosition;
    Gesture.Duration = TouchTime;

    // Tap detection
    if (TouchDistance < 20.0f && TouchTime < LongPressThreshold)
    {
        Gesture.GestureType = EARGestureType::Tap;

        // Check for double tap
        if (GestureHistory.Num() > 0)
        {
            FARGestureEvent& LastGesture = GestureHistory.Last();
            if (LastGesture.GestureType == EARGestureType::Tap &&
                (FPlatformTime::Seconds() - LastGesture.Duration) < DoubleTapThreshold)
            {
                Gesture.GestureType = EARGestureType::DoubleTap;
            }
        }

        RecognizeGesture(Gesture);

        // Perform selection on tap
        if (Gesture.GestureType == EARGestureType::Tap)
        {
            SelectUnitAtTouch(CurrentState.TouchPosition);
        }
        else if (Gesture.GestureType == EARGestureType::DoubleTap)
        {
            OpenContextMenu(CurrentState.TouchPosition);
        }
    }
    else if (TouchDistance >= 20.0f)
    {
        // This was a swipe
        Gesture.GestureType = EARGestureType::Swipe;
        Gesture.Velocity = TouchDistance / TouchTime;
        RecognizeGesture(Gesture);

        // Pan camera on swipe
        FVector2D SwipeDelta = TouchDelta.GetSafeNormal();
        PanCamera(SwipeDelta);
    }
}

void UMingRTSARController::HandlePinchGesture()
{
    // Zoom camera based on pinch scale
    float ZoomDelta = (CurrentState.PinchScale - 1.0f) * -1.0f; // Invert for natural feel
    ZoomCamera(ZoomDelta);

    FARGestureEvent Gesture;
    Gesture.GestureType = EARGestureType::Pinch;
    Gesture.FingerCount = 2;
    RecognizeGesture(Gesture);
}

void UMingRTSARController::HandlePanGesture()
{
    // Pan is handled in ProcessGestures
}

void UMingRTSARController::HandleRotateGesture()
{
    FARGestureEvent Gesture;
    Gesture.GestureType = EARGestureType::Rotate;
    Gesture.FingerCount = 2;
    RecognizeGesture(Gesture);
}

void UMingRTSARController::PerformVirtualObjectHitTest()
{
    FARVirtualObject* Object = GetVirtualObjectAtScreenPosition(CurrentState.TouchPosition);
    if (Object)
    {
        UE_LOG(LogMingARController, Log, TEXT("Hit virtual object: %s"), *Object->ObjectID);

        if (Object->bIsSelectable)
        {
            OnUnitSelected.Broadcast(Object->AssociatedActor);
            PlaySelectionFeedback();
        }
    }
}

void UMingRTSARController::UpdatePointerVisuals()
{
    if (!CurrentState.bIsActive)
    {
        return;
    }

    // Draw debug visualization for pointer
    if (CurrentState.bIsTouching)
    {
        FVector Start = CurrentState.PointerPosition;
        FVector End = Start + (CurrentState.PointerDirection * 1000.0f);

        DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, -1.0f, 0, 2.0f);

        // Draw touch position indicator
        FHitResult HitResult;
        if (PerformScreenRaycast(CurrentState.TouchPosition, HitResult))
        {
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 16, FColor::Green, false, -1.0f, 0, 1.0f);
        }
    }
}
