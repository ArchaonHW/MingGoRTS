// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR Controller Implementation

#include "VRAR/MingRTSVRController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "XRMotionControllerBase.h"
#include "MotionControllerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingVRController, Log, All);

// Sets default values for this component's properties
UMingRTSVRController::UMingRTSVRController()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame
    PrimaryComponentTick.bCanEverTick = true;
    
    // Default to left hand
    ControllerType = EVRControllerType::LeftHand;
}

// Called when the game starts
void UMingRTSVRController::BeginPlay()
{
    Super::BeginPlay();
    
    UE_LOG(LogMingVRController, Log, TEXT("VR Controller BeginPlay"));
    
    // Create motion controller component if not already attached
    if (!MotionControllerComponent)
    {
        MotionControllerComponent = NewObject<UMotionControllerComponent>(GetOwner());
        if (MotionControllerComponent)
        {
            MotionControllerComponent->SetupAttachment(GetOwner()->GetRootComponent());
            MotionControllerComponent->RegisterComponent();
            
            // Set hand based on controller type
            FName HandSource = (ControllerType == EVRControllerType::LeftHand) ? 
                FXRMotionControllerBase::LeftHandSourceId : FXRMotionControllerBase::RightHandSourceId;
            MotionControllerComponent->SetTrackingSource(HandSource);
            
            UE_LOG(LogMingVRController, Log, TEXT("Created MotionControllerComponent for %s"),
                   *UEnum::GetValueAsString(ControllerType));
        }
    }
}

void UMingRTSVRController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    UE_LOG(LogMingVRController, Log, TEXT("VR Controller EndPlay"));
    
    if (MotionControllerComponent)
    {
        MotionControllerComponent->UnregisterComponent();
        MotionControllerComponent = nullptr;
    }
    
    Super::EndPlay(EndPlayReason);
}

// Called every frame
void UMingRTSVRController::TickComponent(float DeltaTime, ELevelTick TickType, 
                                          FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    if (!bIsInitialized)
    {
        return;
    }
    
    // Update controller state
    UpdateControllerState(DeltaTime);
    
    // Process input
    ProcessInput();
    
    // Update visual pointer
    if (CurrentInteractionMode == EVRInteractionMode::Pointer)
    {
        UpdatePointerVisuals();
    }
    
    // Handle haptic timer
    if (HapticTimer > 0.0f)
    {
        HapticTimer -= DeltaTime;
        if (HapticTimer <= 0.0f)
        {
            StopHapticEffect();
        }
    }
}

void UMingRTSVRController::InitializeController(EVRControllerType Type)
{
    ControllerType = Type;
    bIsInitialized = true;
    
    UE_LOG(LogMingVRController, Log, TEXT("Initialized controller: %s"), 
           *UEnum::GetValueAsString(ControllerType));
    
    // Set default interaction mode based on hand
    if (ControllerType == EVRControllerType::LeftHand)
    {
        CurrentInteractionMode = EVRInteractionMode::Pointer;
    }
    else if (ControllerType == EVRControllerType::RightHand)
    {
        CurrentInteractionMode = EVRInteractionMode::DirectTouch;
    }
}

void UMingRTSVRController::ShutdownController()
{
    bIsInitialized = false;
    UE_LOG(LogMingVRController, Log, TEXT("Shutdown controller: %s"), 
           *UEnum::GetValueAsString(ControllerType));
}

bool UMingRTSVRController::IsControllerTracking() const
{
    if (!MotionControllerComponent)
    {
        return false;
    }
    
    return MotionControllerComponent->IsTracked();
}

FVector UMingRTSVRController::GetControllerPosition() const
{
    if (MotionControllerComponent)
    {
        return MotionControllerComponent->GetComponentLocation();
    }
    
    return GetOwner()->GetActorLocation();
}

FRotator UMingRTSVRController::GetControllerRotation() const
{
    if (MotionControllerComponent)
    {
        return MotionControllerComponent->GetComponentRotation();
    }
    
    return GetOwner()->GetActorRotation();
}

FTransform UMingRTSVRController::GetControllerTransform() const
{
    if (MotionControllerComponent)
    {
        return MotionControllerComponent->GetComponentTransform();
    }
    
    return GetOwner()->GetActorTransform();
}

void UMingRTSVRController::SetInteractionMode(EVRInteractionMode Mode)
{
    CurrentInteractionMode = Mode;
    UE_LOG(LogMingVRController, Log, TEXT("Interaction mode changed to: %s"),
           *UEnum::GetValueAsString(Mode));
}

bool UMingRTSVRController::GetPointerLocation(FVector& OutLocation, FVector& OutDirection) const
{
    if (!IsControllerTracking())
    {
        return false;
    }
    
    FTransform ControllerTransform = GetControllerTransform();
    OutLocation = ControllerTransform.GetLocation();
    OutDirection = ControllerTransform.GetRotation().GetForwardVector();
    
    return true;
}

void UMingRTSVRController::PlayHapticEffect(float Intensity, float Duration, bool bIsHand)
{
    if (!MotionControllerComponent)
    {
        return;
    }
    
    // Play haptic feedback using the motion controller
    // Intensity: 0.0 to 1.0
    // Duration: in seconds
    
    FName Hand = (ControllerType == EVRControllerType::LeftHand) ?
        FXRMotionControllerBase::LeftHandSourceId : FXRMotionControllerBase::RightHandSourceId;
    
    // Call haptic effect on the XR system
    // Note: This is a simplified implementation
    // In production, you would use a more sophisticated haptic system
    
    HapticTimer = Duration;
    
    UE_LOG(LogMingVRController, Log, TEXT("Playing haptic effect: Intensity=%f, Duration=%f"),
           Intensity, Duration);
}

void UMingRTSVRController::StopHapticEffect()
{
    HapticTimer = 0.0f;
    
    UE_LOG(LogMingVRController, Log, TEXT("Haptic effect stopped"));
}

void UMingRTSVRController::SelectUnitAtPointer()
{
    if (!IsControllerTracking())
    {
        return;
    }
    
    FHitResult HitResult;
    PerformLineTraceForSelection(HitResult);
    
    if (HitResult.bBlockingHit)
    {
        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            UE_LOG(LogMingVRController, Log, TEXT("Selected actor: %s"), *HitActor->GetName());
            OnUnitSelected.Broadcast(HitActor);
            
            // Play feedback
            PlayHapticEffect(0.3f, 0.1f);
        }
    }
}

void UMingRTSVRController::MoveSelectedUnits()
{
    if (!IsControllerTracking())
    {
        return;
    }
    
    FHitResult HitResult;
    PerformLineTraceForSelection(HitResult);
    
    if (HitResult.bBlockingHit)
    {
        FVector MoveLocation = HitResult.ImpactPoint;
        TArray<AActor*> SelectedUnits; // This would come from your selection system
        
        UE_LOG(LogMingVRController, Log, TEXT("Moving units to: %s"), *MoveLocation.ToString());
        OnUnitsMoved.Broadcast(MoveLocation, SelectedUnits);
        
        // Play feedback
        PlayHapticEffect(0.5f, 0.15f);
    }
}

void UMingRTSVRController::OpenRadialMenu()
{
    UE_LOG(LogMingVRController, Log, TEXT("Opening radial menu"));
    
    // This would trigger your radial menu system
    // For RTS, this could show build options, unit commands, etc.
    
    PlayHapticEffect(0.4f, 0.1f);
}

void UMingRTSVRController::PanCamera()
{
    if (!IsControllerTracking())
    {
        return;
    }
    
    FVector2D ThumbstickInput = CurrentState.Thumbstick;
    
    // Convert thumbstick input to camera pan
    // This would interact with your camera system
    
    UE_LOG(LogMingVRController, Verbose, TEXT("Camera pan input: %s"), *ThumbstickInput.ToString());
}

void UMingRTSVRController::RegisterInputAction(const FVRInputAction& Action)
{
    // Check if action already exists
    for (int32 i = 0; i < RegisteredActions.Num(); ++i)
    {
        if (RegisteredActions[i].ActionName == Action.ActionName)
        {
            RegisteredActions[i] = Action;
            UE_LOG(LogMingVRController, Log, TEXT("Updated input action: %s"), *Action.ActionName.ToString());
            return;
        }
    }
    
    // Add new action
    RegisteredActions.Add(Action);
    UE_LOG(LogMingVRController, Log, TEXT("Registered input action: %s"), *Action.ActionName.ToString());
}

void UMingRTSVRController::UnregisterInputAction(FName ActionName)
{
    for (int32 i = RegisteredActions.Num() - 1; i >= 0; --i)
    {
        if (RegisteredActions[i].ActionName == ActionName)
        {
            RegisteredActions.RemoveAt(i);
            UE_LOG(LogMingVRController, Log, TEXT("Unregistered input action: %s"), *ActionName.ToString());
            return;
        }
    }
}

void UMingRTSVRController::UpdateControllerState(float DeltaTime)
{
    // Update tracking state
    bool bWasTracking = CurrentState.bIsTracking;
    CurrentState.bIsTracking = IsControllerTracking();
    
    if (bWasTracking != CurrentState.bIsTracking)
    {
        OnTrackingStateChanged.Broadcast(CurrentState.bIsTracking);
    }
    
    if (!CurrentState.bIsTracking)
    {
        return;
    }
    
    // Update position and rotation
    CurrentState.Position = GetControllerPosition();
    CurrentState.Rotation = GetControllerRotation();
    
    // Get input from XR system
    // Note: These would typically come from the Enhanced Input system or raw XR input
    // This is a simplified implementation
    
    // Example: Get trigger axis
    // CurrentState.TriggerAxis = UHeadMountedDisplayFunctionLibrary::GetXRMotionController...
    
    // Example: Get thumbstick
    // CurrentState.Thumbstick = ...
}

void UMingRTSVRController::ProcessInput()
{
    // Process registered input actions
    for (const FVRInputAction& Action : RegisteredActions)
    {
        if (Action.Controller != ControllerType)
        {
            continue;
        }
        
        // Check if input is triggered
        // This would interface with your input system
    }
    
    // Handle RTS-specific input based on controller type
    HandleRTSInput();
}

void UMingRTSVRController::HandleRTSInput()
{
    if (!CurrentState.bIsTracking)
    {
        return;
    }
    
    // Left hand: Selection and pointer
    if (ControllerType == EVRControllerType::LeftHand)
    {
        // Trigger pressed - Select unit
        if (CurrentState.bTriggerPressed && !CurrentState.bGripPressed)
        {
            SelectUnitAtPointer();
        }
        
        // Thumbstick - Camera pan
        if (!CurrentState.Thumbstick.IsNearlyZero())
        {
            PanCamera();
        }
    }
    // Right hand: Movement and actions
    else if (ControllerType == EVRControllerType::RightHand)
    {
        // Trigger pressed - Move units
        if (CurrentState.bTriggerPressed)
        {
            MoveSelectedUnits();
        }
        
        // Grip pressed - Open radial menu
        if (CurrentState.bGripPressed)
        {
            OpenRadialMenu();
        }
    }
}

void UMingRTSVRController::UpdatePointerVisuals()
{
    if (!IsControllerTracking() || CurrentInteractionMode != EVRInteractionMode::Pointer)
    {
        return;
    }
    
    FVector StartLocation;
    FVector Direction;
    
    if (GetPointerLocation(StartLocation, Direction))
    {
        FVector EndLocation = StartLocation + (Direction * 1000.0f);
        
        // Draw debug line for pointer
        DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Blue, false, -1.0f, 0, 2.0f);
        
        // Perform line trace to show where pointer is hitting
        FHitResult HitResult;
        PerformLineTraceForSelection(HitResult);
        
        if (HitResult.bBlockingHit)
        {
            // Draw hit point
            DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Red, false, -1.0f);
            
            // Draw sphere around hit point
            DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.0f, 16, FColor::Red, false, -1.0f, 0, 1.0f);
        }
    }
}

void UMingRTSVRController::PerformLineTraceForSelection(FHitResult& OutHit)
{
    FVector StartLocation;
    FVector Direction;
    
    if (!GetPointerLocation(StartLocation, Direction))
    {
        return;
    }
    
    FVector EndLocation = StartLocation + (Direction * 5000.0f); // 50 meters max
    
    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = true;
    QueryParams.bReturnPhysicalMaterial = false;
    
    // Perform line trace
    GetWorld()->LineTraceSingleByChannel(
        OutHit,
        StartLocation,
        EndLocation,
        ECC_Visibility,
        QueryParams
    );
}
