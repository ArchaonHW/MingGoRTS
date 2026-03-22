// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR Support Implementation

#include "VRAR/MingRTSVRSupport.h"
#include "Engine/Engine.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "XRDeviceVisualizationComponent.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingVRSupport, Log, All);

void UMingRTSVRSupport::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingVRSupport, Log, TEXT("Initializing MingRTSVRSupport..."));
    
    // Set default comfort settings
    ComfortSettings.bUseVignette = true;
    ComfortSettings.bUseTeleport = true;
    ComfortSettings.bSnapTurn = true;
    ComfortSettings.SnapTurnAngle = 45.0f;
    ComfortSettings.MovementSpeed = 1.0f;
    ComfortSettings.bUseComfortMode = false;
    
    // Try to auto-detect and initialize VR if available
    if (IsVRDeviceConnected())
    {
        UE_LOG(LogMingVRSupport, Log, TEXT("VR device detected, auto-enabling VR..."));
        EnableVR();
    }
}

void UMingRTSVRSupport::Deinitialize()
{
    UE_LOG(LogMingVRSupport, Log, TEXT("Shutting down MingRTSVRSupport..."));
    
    if (IsVREnabled())
    {
        DisableVR();
    }
    
    Super::Deinitialize();
}

void UMingRTSVRSupport::InitializeVRSupport()
{
    if (VRStatus == EVRSystemStatus::Ready || VRStatus == EVRSystemStatus::Active)
    {
        UE_LOG(LogMingVRSupport, Warning, TEXT("VR Support already initialized"));
        return;
    }
    
    UpdateVRStatus(EVRSystemStatus::Initializing);
    
    // Detect connected VR device
    CurrentDeviceInfo = GetVRDeviceInfo();
    
    if (CurrentDeviceInfo.HeadsetType != EVRHeadsetType::None)
    {
        UE_LOG(LogMingVRSupport, Log, TEXT("VR Device detected: %s"), *CurrentDeviceInfo.DeviceName);
        SetupVRInputMappings();
        UpdateVRStatus(EVRSystemStatus::Ready);
        OnVRDeviceConnected.Broadcast(CurrentDeviceInfo.HeadsetType);
    }
    else
    {
        UE_LOG(LogMingVRSupport, Warning, TEXT("No VR device detected"));
        UpdateVRStatus(EVRSystemStatus::Error);
    }
}

void UMingRTSVRSupport::ShutdownVRSupport()
{
    if (VRStatus == EVRSystemStatus::NotInitialized)
    {
        return;
    }
    
    UpdateVRStatus(EVRSystemStatus::ShuttingDown);
    
    if (bVREnabled)
    {
        DisableVR();
    }
    
    UpdateVRStatus(EVRSystemStatus::NotInitialized);
    UE_LOG(LogMingVRSupport, Log, TEXT("VR Support shutdown complete"));
}

bool UMingRTSVRSupport::IsVRDeviceConnected() const
{
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

FVRDeviceInfo UMingRTSVRSupport::GetVRDeviceInfo() const
{
    FVRDeviceInfo Info;
    
    if (!IsVRDeviceConnected())
    {
        Info.HeadsetType = EVRHeadsetType::None;
        return Info;
    }
    
    Info.HeadsetType = DetectVRHeadsetType();
    
    // Get HMD device data
    FHMDDeviceData HMDData;
    if (UHeadMountedDisplayFunctionLibrary::GetHMDDeviceData(HMDData))
    {
        Info.DeviceName = HMDData.DeviceName;
        Info.DisplayResolution = FVector2D(HMDData.ResolutionX, HMDData.ResolutionY);
        Info.RefreshRate = HMDData.RefreshRate;
    }
    
    // Default values for most VR headsets
    Info.bHasHandTracking = (Info.HeadsetType == EVRHeadsetType::OculusQuest ||
                              Info.HeadsetType == EVRHeadsetType::Valve_Index);
    Info.bHasEyeTracking = (Info.HeadsetType == EVRHeadsetType::Valve_Index);
    Info.bHasHapticFeedback = true;
    Info.ControllerCount = 2;
    
    return Info;
}

EVRHeadsetType UMingRTSVRSupport::DetectVRHeadsetType() const
{
    if (!IsVRDeviceConnected())
    {
        return EVRHeadsetType::None;
    }
    
    FString DeviceName;
    FHMDDeviceData HMDData;
    
    if (UHeadMountedDisplayFunctionLibrary::GetHMDDeviceData(HMDData))
    {
        DeviceName = HMDData.DeviceName.ToLower();
    }
    else
    {
        // Try to get device name from HMD interface
        DeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName().ToString().ToLower();
    }
    
    if (DeviceName.Contains("oculus") || DeviceName.Contains("rift"))
    {
        return EVRHeadsetType::OculusRift;
    }
    else if (DeviceName.Contains("quest"))
    {
        return EVRHeadsetType::OculusQuest;
    }
    else if (DeviceName.Contains("vive"))
    {
        return EVRHeadsetType::HTC_Vive;
    }
    else if (DeviceName.Contains("index"))
    {
        return EVRHeadsetType::Valve_Index;
    }
    else if (DeviceName.Contains("windows") || DeviceName.Contains("wmr"))
    {
        return EVRHeadsetType::WindowsMR;
    }
    else if (DeviceName.Contains("psvr") || DeviceName.Contains("playstation"))
    {
        return EVRHeadsetType::PSVR;
    }
    
    return EVRHeadsetType::Custom;
}

bool UMingRTSVRSupport::EnableVR()
{
    if (bVREnabled)
    {
        return true;
    }
    
    if (!IsVRDeviceConnected())
    {
        UE_LOG(LogMingVRSupport, Error, TEXT("Cannot enable VR: No VR device connected"));
        return false;
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("Enabling VR mode..."));
    
    // Enable HMD
    UHeadMountedDisplayFunctionLibrary::SetEnableHMD(true);
    
    // Apply optimization settings
    ApplyVROptimizationSettings();
    
    // Adapt UI for VR
    AdaptUIToVR();
    
    bVREnabled = true;
    UpdateVRStatus(EVRSystemStatus::Active);
    
    UE_LOG(LogMingVRSupport, Log, TEXT("VR mode enabled successfully"));
    return true;
}

void UMingRTSVRSupport::DisableVR()
{
    if (!bVREnabled)
    {
        return;
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("Disabling VR mode..."));
    
    // Restore UI from VR
    RestoreUIFromVR();
    
    // Disable HMD
    UHeadMountedDisplayFunctionLibrary::SetEnableHMD(false);
    
    bVREnabled = false;
    
    if (VRStatus == EVRSystemStatus::Active)
    {
        UpdateVRStatus(EVRSystemStatus::Ready);
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("VR mode disabled"));
}

void UMingRTSVRSupport::SetTrackingSpace(EVRTrackingSpace Space)
{
    CurrentTrackingSpace = Space;
    
    switch (Space)
    {
        case EVRTrackingSpace::Stationary:
            UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
            break;
            
        case EVRTrackingSpace::RoomScale:
            UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
            break;
            
        case EVRTrackingSpace::WorldScale:
            UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);
            break;
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("Tracking space set to: %s"), 
           *UEnum::GetValueAsString(Space));
}

void UMingRTSVRSupport::RecenterHMD()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
    UE_LOG(LogMingVRSupport, Log, TEXT("HMD recentered"));
}

void UMingRTSVRSupport::SetComfortSettings(const FVRComfortSettings& Settings)
{
    ComfortSettings = Settings;
    UE_LOG(LogMingVRSupport, Log, TEXT("VR comfort settings updated"));
}

void UMingRTSVRSupport::SetVROptimizationLevel(int32 Level)
{
    VROptimizationLevel = FMath::Clamp(Level, 0, 3);
    
    if (bVREnabled)
    {
        ApplyVROptimizationSettings();
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("VR optimization level set to: %d"), VROptimizationLevel);
}

int32 UMingRTSVRSupport::GetCurrentVROptimizationLevel() const
{
    return VROptimizationLevel;
}

void UMingRTSVRSupport::AdaptUIToVR()
{
    UE_LOG(LogMingVRSupport, Log, TEXT("Adapting UI for VR..."));
    
    // Set world locked UI mode for VR
    // This would typically interact with your UI manager
    // For now, we log that adaptation is happening
    
    // Adjust UI scale for VR
    // Move UI to comfortable viewing distance
    // Increase text size for readability
}

void UMingRTSVRSupport::RestoreUIFromVR()
{
    UE_LOG(LogMingVRSupport, Log, TEXT("Restoring UI from VR..."));
    
    // Restore standard screen-space UI
    // Reset UI scale and positioning
}

void UMingRTSVRSupport::OnVRDeviceConnectionChanged(bool bConnected)
{
    if (bConnected)
    {
        CurrentDeviceInfo = GetVRDeviceInfo();
        OnVRDeviceConnected.Broadcast(CurrentDeviceInfo.HeadsetType);
    }
    else
    {
        OnVRDeviceDisconnected.Broadcast();
        
        if (bVREnabled)
        {
            DisableVR();
        }
    }
}

void UMingRTSVRSupport::UpdateVRStatus(EVRSystemStatus NewStatus)
{
    if (VRStatus != NewStatus)
    {
        EVRSystemStatus OldStatus = VRStatus;
        VRStatus = NewStatus;
        OnVRStatusChanged.Broadcast(OldStatus, NewStatus);
        
        UE_LOG(LogMingVRSupport, Log, TEXT("VR Status changed: %s -> %s"),
               *UEnum::GetValueAsString(OldStatus),
               *UEnum::GetValueAsString(NewStatus));
    }
}

void UMingRTSVRSupport::ApplyVROptimizationSettings()
{
    // Apply performance settings based on optimization level
    switch (VROptimizationLevel)
    {
        case 0: // Low - Maximum quality
            // Enable all rendering features
            // Full resolution
            break;
            
        case 1: // Medium - Balanced
            // Moderate quality settings
            break;
            
        case 2: // High - Performance focused
            // Reduce some post-processing
            // Optimize shadows
            break;
            
        case 3: // Ultra - Maximum performance
            // Aggressive optimization
            // Reduced rendering resolution
            // Simplified shaders
            break;
    }
    
    UE_LOG(LogMingVRSupport, Log, TEXT("Applied VR optimization level: %d"), VROptimizationLevel);
}

void UMingRTSVRSupport::SetupVRInputMappings()
{
    UE_LOG(LogMingVRSupport, Log, TEXT("Setting up VR input mappings..."));
    
    // Configure input for VR controllers
    // Map VR controller buttons to game actions
    // Setup haptic feedback profiles
}
