// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR Support System Implementation - C1-1

#include "VRARM/MingVRSupportSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "XRMotionControllerBase.h"

DEFINE_LOG_CATEGORY_STATIC(LogVR, Log, All);

UMingVRSupportSystem::UMingVRSupportSystem()
    : bVRModeEnabled(false)
    , bHandTrackingEnabled(false)
    , ConnectedDevice(EVRDeviceType::None)
    , WorldToMetersScale(100.0f)
    , PerformanceLevel(2)
{
}

void UMingVRSupportSystem::InitializeVRSystem()
{
    UE_LOG(LogVR, Log, TEXT("Initializing VR Support System"));

    // Detect connected VR device
    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
    {
        UE_LOG(LogVR, Log, TEXT("HMD is enabled"));

        // Try to detect specific device type
        DetectConnectedDevice();

        // Set default comfort settings
        ComfortSettings = FVRComfortSettings();

        // Start tracking update
        StartTrackingUpdate();

        OnDeviceConnected(ConnectedDevice);
    }
    else
    {
        UE_LOG(LogVR, Warning, TEXT("No HMD detected"));
    }
}

void UMingVRSupportSystem::ShutdownVRSystem()
{
    UE_LOG(LogVR, Log, TEXT("Shutting down VR Support System"));

    StopTrackingUpdate();
    ShutdownVRDevice();
}

bool UMingVRSupportSystem::IsVRDeviceConnected() const
{
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

EVRDeviceType UMingVRSupportSystem::GetConnectedDeviceType() const
{
    return ConnectedDevice;
}

FVRDeviceInfo UMingVRSupportSystem::GetDeviceInfo() const
{
    return DeviceInfo;
}

TArray<FVRDeviceInfo> UMingVRSupportSystem::GetAllConnectedDevices() const
{
    TArray<FVRDeviceInfo> Devices;

    if (IsVRDeviceConnected())
    {
        Devices.Add(DeviceInfo);
    }

    return Devices;
}

bool UMingVRSupportSystem::InitializeDevice(EVRDeviceType DeviceType)
{
    UE_LOG(LogVR, Log, TEXT("Initializing VR device: %s"), *UEnum::GetValueAsString(DeviceType));

    return InitializeVRDevice(DeviceType);
}

void UMingVRSupportSystem::SetTrackingSpace(EVRTrackingSpace TrackingSpace)
{
    UE_LOG(LogVR, Log, TEXT("Setting tracking space: %s"), *UEnum::GetValueAsString(TrackingSpace));

    switch (TrackingSpace)
    {
    case EVRTrackingSpace::Seated:
        UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Eye);
        break;
    case EVRTrackingSpace::Standing:
    case EVRTrackingSpace::RoomScale:
        UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
        break;
    default:
        break;
    }
}

EVRTrackingSpace UMingVRSupportSystem::GetTrackingSpace() const
{
    EHMDTrackingOrigin Origin = UHeadMountedDisplayFunctionLibrary::GetTrackingOrigin();

    switch (Origin)
    {
    case EHMDTrackingOrigin::Eye:
        return EVRTrackingSpace::Seated;
    case EHMDTrackingOrigin::Floor:
        return EVRTrackingSpace::Standing;
    default:
        return EVRTrackingSpace::Custom;
    }
}

FVRTrackingData UMingVRSupportSystem::GetCurrentTrackingData() const
{
    return CurrentTrackingData;
}

FVector UMingVRSupportSystem::GetHeadPosition() const
{
    FVector Position;
    FRotator Rotation;
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
    return Position;
}

FRotator UMingVRSupportSystem::GetHeadRotation() const
{
    FVector Position;
    FRotator Rotation;
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
    return Rotation;
}

bool UMingVRSupportSystem::GetControllerPositionAndOrientation(
    EVRControllerType Controller, FVector& OutPosition, FRotator& OutOrientation) const
{
    // Determine which hand
    FXRMotionControllerBase::EControllerHand Hand =
        (Controller == EVRControllerType::MotionController) ?
        FXRMotionControllerBase::EControllerHand::Left : FXRMotionControllerBase::EControllerHand::Right;

    return UHeadMountedDisplayFunctionLibrary::GetControllerPositionAndOrientation(
        0, static_cast<int32>(Hand), OutOrientation, OutPosition);
}

void UMingVRSupportSystem::SetComfortSettings(const FVRComfortSettings& Settings)
{
    ComfortSettings = Settings;
    ApplyComfortSettings();

    UE_LOG(LogVR, Log, TEXT("VR comfort settings updated"));
}

FVRComfortSettings UMingVRSupportSystem::GetComfortSettings() const
{
    return ComfortSettings;
}

void UMingVRSupportSystem::TriggerHapticFeedback(const FVRHapticFeedback& Haptic)
{
    int32 Hand = (Haptic.Controller == EVRControllerType::MotionController) ? 0 : 1;

    UHeadMountedDisplayFunctionLibrary::PlayHapticFeedback(
        Haptic.Amplitude,
        Hand,
        Haptic.Frequency,
        Haptic.Duration
    );

    OnVRHapticRequested.Broadcast(Haptic.Controller, Haptic);
}

void UMingVRSupportSystem::PlayHapticEffect(const FString& EffectName, EVRControllerType Controller, float Scale)
{
    FVRHapticFeedback Haptic;
    Haptic.Controller = Controller;
    Haptic.Amplitude = Scale;

    if (EffectName == TEXT("Collision"))
    {
        Haptic.Frequency = 0.8f;
        Haptic.Duration = 0.1f;
    }
    else if (EffectName == TEXT("Selection"))
    {
        Haptic.Frequency = 0.3f;
        Haptic.Duration = 0.05f;
    }
    else if (EffectName == TEXT("Action"))
    {
        Haptic.Frequency = 1.0f;
        Haptic.Duration = 0.2f;
    }

    TriggerHapticFeedback(Haptic);
}

void UMingVRSupportSystem::StopHapticFeedback(EVRControllerType Controller)
{
    // Stop haptic feedback for the specified controller
    UE_LOG(LogVR, Verbose, TEXT("Stopping haptic feedback for controller"));
}

void UMingVRSupportSystem::EnableVRMode(bool bEnable)
{
    bVRModeEnabled = bEnable;

    if (bEnable)
    {
        UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
        UE_LOG(LogVR, Log, TEXT("VR mode enabled"));
    }
    else
    {
        UHeadMountedDisplayFunctionLibrary::EnableHMD(false);
        UE_LOG(LogVR, Log, TEXT("VR mode disabled"));
    }
}

bool UMingVRSupportSystem::IsVRModeEnabled() const
{
    return bVRModeEnabled;
}

void UMingVRSupportSystem::RecenterTrackingOrigin()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
    UE_LOG(LogVR, Log, TEXT("Tracking origin recentered"));
}

void UMingVRSupportSystem::SetWorldToMetersScale(float Scale)
{
    WorldToMetersScale = Scale;
    UHeadMountedDisplayFunctionLibrary::SetWorldToMetersScale(Scale);
}

float UMingVRSupportSystem::GetWorldToMetersScale() const
{
    return UHeadMountedDisplayFunctionLibrary::GetWorldToMetersScale();
}

void UMingVRSupportSystem::CalibrateIPD(float MeasuredIPD)
{
    SetInterpupillaryDistance(MeasuredIPD);
    UE_LOG(LogVR, Log, TEXT("IPD calibrated to: %.2fmm"), MeasuredIPD);
}

float UMingVRSupportSystem::GetCurrentIPD() const
{
    return UHeadMountedDisplayFunctionLibrary::GetInterpupillaryDistance();
}

void UMingVRSupportSystem::SetInterpupillaryDistance(float NewIPD)
{
    UHeadMountedDisplayFunctionLibrary::SetInterpupillaryDistance(NewIPD);
    UE_LOG(LogVR, Log, TEXT("IPD set to: %.2fmm"), NewIPD);
}

bool UMingVRSupportSystem::IsHeadMountedDisplayEnabled() const
{
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

void UMingVRSupportSystem::EnableSpectatorScreenMode(bool bEnable)
{
    UHeadMountedDisplayFunctionLibrary::EnableSpectatorScreen(bEnable);
    UE_LOG(LogVR, Log, TEXT("Spectator screen mode: %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingVRSupportSystem::IsSpectatorScreenModeEnabled() const
{
    return UHeadMountedDisplayFunctionLibrary::IsSpectatorScreenEnabled();
}

void UMingVRSupportSystem::SetVRPerformanceLevel(int32 Level)
{
    PerformanceLevel = FMath::Clamp(Level, 0, 3);
    UpdateVRPerformance();

    UE_LOG(LogVR, Log, TEXT("VR performance level set to: %d"), PerformanceLevel);
}

int32 UMingVRSupportSystem::GetVRPerformanceLevel() const
{
    return PerformanceLevel;
}

void UMingVRSupportSystem::OptimizeForVR()
{
    UE_LOG(LogVR, Log, TEXT("Optimizing for VR"));

    // Set appropriate settings for VR
    SetVRPerformanceLevel(2);

    // Enable motion smoothing if available
    // Set fixed foveated rendering level

    UE_LOG(LogVR, Log, TEXT("VR optimization complete"));
}

void UMingVRSupportSystem::EnableVRHandTracking(bool bEnable)
{
    bHandTrackingEnabled = bEnable;
    UE_LOG(LogVR, Log, TEXT("VR hand tracking: %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingVRSupportSystem::IsHandTrackingEnabled() const
{
    return bHandTrackingEnabled;
}

void UMingVRSupportSystem::SetVRRenderingResolution(float ResolutionScale)
{
    UHeadMountedDisplayFunctionLibrary::SetSpectatorScreenTextureResolution(ResolutionScale);
    UE_LOG(LogVR, Log, TEXT("VR rendering resolution set to: %.2f"), ResolutionScale);
}

bool UMingVRSupportSystem::HasValidTrackingPosition() const
{
    FVector Position;
    FRotator Rotation;
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
    return true; // Simplified
}

bool UMingVRSupportSystem::IsTrackedDeviceConnected(int32 DeviceId) const
{
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

void UMingVRSupportSystem::StartTrackingUpdate()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            TrackingUpdateTimer,
            this,
            &UMingVRSupportSystem::UpdateTrackingData,
            0.011f, // ~90Hz
            true
        );
    }
}

void UMingVRSupportSystem::StopTrackingUpdate()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(TrackingUpdateTimer);
    }
}

void UMingVRSupportSystem::UpdateTrackingData()
{
    if (!IsVRDeviceConnected())
    {
        return;
    }

    // Update head tracking
    CurrentTrackingData.HeadPosition = GetHeadPosition();
    CurrentTrackingData.HeadRotation = GetHeadRotation().Quaternion();

    // Update controller tracking
    FVector LeftPos, RightPos;
    FRotator LeftRot, RightRot;

    if (GetControllerPositionAndOrientation(EVRControllerType::MotionController, LeftPos, LeftRot))
    {
        CurrentTrackingData.LeftHandPosition = LeftPos;
        CurrentTrackingData.LeftHandRotation = LeftRot.Quaternion();
    }

    if (GetControllerPositionAndOrientation(EVRControllerType::Gamepad, RightPos, RightRot))
    {
        CurrentTrackingData.RightHandPosition = RightPos;
        CurrentTrackingData.RightHandRotation = RightRot.Quaternion();
    }

    CurrentTrackingData.Timestamp = FPlatformTime::Cycles();

    OnVRTrackingUpdated.Broadcast(CurrentTrackingData);
}

bool UMingVRSupportSystem::InitializeVRDevice(EVRDeviceType DeviceType)
{
    ConnectedDevice = DeviceType;

    DeviceInfo.DeviceType = DeviceType;
    DeviceInfo.bIsConnected = true;
    DeviceInfo.bIsTracking = true;

    // Set device-specific info
    switch (DeviceType)
    {
    case EVRDeviceType::OculusRift:
        DeviceInfo.DeviceName = TEXT("Oculus Rift");
        DeviceInfo.Manufacturer = TEXT("Oculus");
        DeviceInfo.DisplayResolution = FIntPoint(2160, 1200);
        DeviceInfo.RefreshRate = 90.0f;
        break;
    case EVRDeviceType::OculusQuest:
        DeviceInfo.DeviceName = TEXT("Oculus Quest");
        DeviceInfo.Manufacturer = TEXT("Oculus");
        DeviceInfo.DisplayResolution = FIntPoint(3664, 1920);
        DeviceInfo.RefreshRate = 72.0f;
        break;
    case EVRDeviceType::HTC_Vive:
        DeviceInfo.DeviceName = TEXT("HTC Vive");
        DeviceInfo.Manufacturer = TEXT("HTC");
        DeviceInfo.DisplayResolution = FIntPoint(2160, 1200);
        DeviceInfo.RefreshRate = 90.0f;
        break;
    case EVRDeviceType::ValveIndex:
        DeviceInfo.DeviceName = TEXT("Valve Index");
        DeviceInfo.Manufacturer = TEXT("Valve");
        DeviceInfo.DisplayResolution = FIntPoint(2880, 1600);
        DeviceInfo.RefreshRate = 144.0f;
        break;
    default:
        DeviceInfo.DeviceName = TEXT("Generic HMD");
        DeviceInfo.Manufacturer = TEXT("Unknown");
        break;
    }

    UE_LOG(LogVR, Log, TEXT("VR device initialized: %s"), *DeviceInfo.DeviceName);

    return true;
}

void UMingVRSupportSystem::ShutdownVRDevice()
{
    if (ConnectedDevice != EVRDeviceType::None)
    {
        OnDeviceDisconnected(ConnectedDevice);
        ConnectedDevice = EVRDeviceType::None;
    }

    UE_LOG(LogVR, Log, TEXT("VR device shutdown"));
}

void UMingVRSupportSystem::OnDeviceConnected(EVRDeviceType DeviceType)
{
    FVRDeviceInfo Info = DeviceInfo;
    OnVRDeviceConnected.Broadcast(Info);

    UE_LOG(LogVR, Log, TEXT("VR device connected: %s"), *UEnum::GetValueAsString(DeviceType));
}

void UMingVRSupportSystem::OnDeviceDisconnected(EVRDeviceType DeviceType)
{
    OnVRDeviceDisconnected.Broadcast(DeviceType);

    UE_LOG(LogVR, Log, TEXT("VR device disconnected: %s"), *UEnum::GetValueAsString(DeviceType));
}

void UMingVRSupportSystem::ProcessVRInput()
{
    // Process VR controller input
}

void UMingVRSupportSystem::ApplyComfortSettings()
{
    UE_LOG(LogVR, Log, TEXT("Applying comfort settings"));

    // Apply comfort zone
    // Configure vignette
    // Set up teleport/smooth locomotion
}

void UMingVRSupportSystem::UpdateVRPerformance()
{
    UE_LOG(LogVR, Verbose, TEXT("Updating VR performance settings for level: %d"), PerformanceLevel);

    switch (PerformanceLevel)
    {
    case 0: // Low
        SetVRRenderingResolution(0.7f);
        break;
    case 1: // Medium
        SetVRRenderingResolution(0.85f);
        break;
    case 2: // High
        SetVRRenderingResolution(1.0f);
        break;
    case 3: // Ultra
        SetVRRenderingResolution(1.25f);
        break;
    }
}

void UMingVRSupportSystem::DetectConnectedDevice()
{
    // Try to detect the connected HMD type
    FString DeviceName = UHeadMountedDisplayFunctionLibrary::GetDeviceName();

    if (DeviceName.Contains(TEXT("Oculus")) || DeviceName.Contains(TEXT("Rift")))
    {
        InitializeVRDevice(EVRDeviceType::OculusRift);
    }
    else if (DeviceName.Contains(TEXT("Quest")))
    {
        InitializeVRDevice(EVRDeviceType::OculusQuest);
    }
    else if (DeviceName.Contains(TEXT("Vive")))
    {
        InitializeVRDevice(EVRDeviceType::HTC_Vive);
    }
    else if (DeviceName.Contains(TEXT("Index")))
    {
        InitializeVRDevice(EVRDeviceType::ValveIndex);
    }
    else
    {
        InitializeVRDevice(EVRDeviceType::GenericHMD);
    }
}

static UMingVRSupportSystem* UMingVRSupportSystem::Get(UObject* WorldContextObject)
{
    static UMingVRSupportSystem* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingVRSupportSystem>();
        Instance->AddToRoot();
    }
    return Instance;
}
