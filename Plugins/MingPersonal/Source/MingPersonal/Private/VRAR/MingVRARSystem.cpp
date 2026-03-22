// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR/AR System Implementation

#include "VRAR/MingVRARSystem.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/ConfigCacheIni.h"

// Constants for VR/AR configuration
namespace VRARConstants
{
    constexpr float DefaultIPD = 0.064f;
    constexpr int32 DefaultRefreshRate = 90;
    constexpr float DefaultTrackingRadius = 3.0f;
    constexpr float ComfortVignetteScale = 0.8f;
}

UMingVRARSystem::UMingVRARSystem()
    : SessionState(EVRARSessionState::Idle)
    , ActiveDevice(EVRDeviceType::None)
{
    CurrentSettings.DisplayMode = EVRARDisplayMode::FullVR;
    CurrentSettings.TrackingSpace = EVRARTrackingSpace::RoomScale;
    CurrentSettings.InteractionMode = EVRARInteractionMode::Controller;
    CurrentSettings.MovementSpeed = 1.0f;
    CurrentSettings.TurnSmoothing = 0.5f;
    CurrentSettings.bSnapTurnEnabled = true;
    CurrentSettings.SnapTurnAngle = 45.0f;
    CurrentSettings.bComfortVignette = false;
    CurrentSettings.IPD = VRARConstants::DefaultIPD;
    CurrentSettings.WorldScale = 1.0f;
}

void UMingVRARSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: Initializing VR/AR subsystem..."));
    LoadSettings();
    CacheDeviceCapabilities();
    UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: VR/AR subsystem initialized"));
}

void UMingVRARSystem::Deinitialize()
{
    UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: Deinitializing..."));
    if (SessionState == EVRARSessionState::Running || SessionState == EVRARSessionState::Paused)
    {
        ShutdownVRARSession();
    }
    Super::Deinitialize();
}

bool UMingVRARSystem::InitializeVRARSession(EVRDeviceType DeviceType)
{
    UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: Initializing session for device: %s"),
        *UEnum::GetValueAsString(DeviceType));

    if (SessionState == EVRARSessionState::Running)
    {
        ShutdownVRARSession();
    }

    SessionState = EVRARSessionState::Initializing;
    HandleSessionStateChange(SessionState);

    bool bIsAR = (DeviceType == EVRDeviceType::HoloLens ||
                  DeviceType == EVRDeviceType::MagicLeap ||
                  DeviceType == EVRDeviceType::iOS_AR ||
                  DeviceType == EVRDeviceType::Android_AR);

    bool bSuccess = bIsAR ? SetupARCamera() : SetupVRRendering();

    if (bSuccess)
    {
        ActiveDevice = DeviceType;
        SessionState = EVRARSessionState::Running;
        SetupInputMappings();
        ApplyComfortSettings();
        UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: Session initialized successfully"));
    }
    else
    {
        SessionState = EVRARSessionState::Error;
        UE_LOG(LogTemp, Error, TEXT("MingVRARSystem: Failed to initialize session"));
    }

    HandleSessionStateChange(SessionState);
    return bSuccess;
}

void UMingVRARSystem::ShutdownVRARSession()
{
    UE_LOG(LogTemp, Log, TEXT("MingVRARSystem: Shutting down session..."));
    SessionState = EVRARSessionState::ShuttingDown;
    HandleSessionStateChange(SessionState);
    CleanupVRAR();
    ActiveDevice = EVRDeviceType::None;
    SessionState = EVRARSessionState::Idle;
    HandleSessionStateChange(SessionState);
}

void UMingVRARSystem::PauseVRARSession()
{
    if (SessionState == EVRARSessionState::Running)
    {
        SessionState = EVRARSessionState::Paused;
        HandleSessionStateChange(SessionState);
    }
}

void UMingVRARSystem::ResumeVRARSession()
{
    if (SessionState == EVRARSessionState::Paused)
    {
        SessionState = EVRARSessionState::Running;
        HandleSessionStateChange(SessionState);
    }
}

TArray<EVRDeviceType> UMingVRARSystem::GetAvailableDevices()
{
    TArray<EVRDeviceType> AvailableDevices;

    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
    {
        FString HMDDeviceName = UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName();

        if (HMDDeviceName.Contains("Oculus", ESearchCase::IgnoreCase))
        {
            if (HMDDeviceName.Contains("Quest 2", ESearchCase::IgnoreCase))
                AvailableDevices.Add(EVRDeviceType::OculusQuest2);
            else if (HMDDeviceName.Contains("Quest", ESearchCase::IgnoreCase))
                AvailableDevices.Add(EVRDeviceType::OculusQuest);
            else
                AvailableDevices.Add(EVRDeviceType::OculusRift);
        }
        else if (HMDDeviceName.Contains("Vive", ESearchCase::IgnoreCase))
        {
            AvailableDevices.Add(HMDDeviceName.Contains("Pro", ESearchCase::IgnoreCase) 
                ? EVRDeviceType::HTC_VivePro : EVRDeviceType::HTC_Vive);
        }
        else if (HMDDeviceName.Contains("Index", ESearchCase::IgnoreCase))
            AvailableDevices.Add(EVRDeviceType::ValveIndex);
        else if (HMDDeviceName.Contains("Windows", ESearchCase::IgnoreCase))
            AvailableDevices.Add(EVRDeviceType::WindowsMR);
        else
            AvailableDevices.Add(EVRDeviceType::GenericVR);
    }

    return AvailableDevices;
}

FVRDeviceCapabilities UMingVRARSystem::GetDeviceCapabilities(EVRDeviceType DeviceType)
{
    if (DeviceCapabilitiesCache.Contains(DeviceType))
        return DeviceCapabilitiesCache[DeviceType];

    FVRDeviceCapabilities DefaultCaps;
    DefaultCaps.DeviceName = UEnum::GetValueAsString(DeviceType);
    return DefaultCaps;
}

bool UMingVRARSystem::IsDeviceConnected(EVRDeviceType DeviceType) const
{
    if (DeviceType == EVRDeviceType::None)
        return false;

    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayConnected())
        return ActiveDevice == DeviceType;

    return false;
}

bool UMingVRARSystem::AutoDetectAndInitialize()
{
    TArray<EVRDeviceType> AvailableDevices = GetAvailableDevices();
    if (AvailableDevices.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("MingVRARSystem: No VR/AR devices detected"));
        return false;
    }
    return InitializeVRARSession(AvailableDevices[0]);
}

void UMingVRARSystem::ApplySettings(const FVRARSettings& NewSettings)
{
    CurrentSettings = NewSettings;
    if (CurrentSettings.IPD != VRARConstants::DefaultIPD)
    {
        UHeadMountedDisplayFunctionLibrary::SetWorldToMetersScale(CurrentSettings.WorldScale);
    }
    ApplyComfortSettings();
}

void UMingVRARSystem::ResetToDefaultSettings()
{
    CurrentSettings.DisplayMode = EVRARDisplayMode::FullVR;
    CurrentSettings.TrackingSpace = EVRARTrackingSpace::RoomScale;
    CurrentSettings.InteractionMode = EVRARInteractionMode::Controller;
    CurrentSettings.MovementSpeed = 1.0f;
    CurrentSettings.TurnSmoothing = 0.5f;
    CurrentSettings.bSnapTurnEnabled = true;
    CurrentSettings.SnapTurnAngle = 45.0f;
    CurrentSettings.bComfortVignette = false;
    CurrentSettings.IPD = VRARConstants::DefaultIPD;
    CurrentSettings.WorldScale = 1.0f;
    ApplySettings(CurrentSettings);
}

void UMingVRARSystem::SaveSettings()
{
    const FString ConfigSection = TEXT("VRARSettings");
    const FString ConfigFile = FPaths::ProjectConfigDir() / TEXT("VRAR.ini");

    GConfig->SetInt(ConfigSection, TEXT("DisplayMode"), (int32)CurrentSettings.DisplayMode, ConfigFile);
    GConfig->SetInt(ConfigSection, TEXT("TrackingSpace"), (int32)CurrentSettings.TrackingSpace, ConfigFile);
    GConfig->SetInt(ConfigSection, TEXT("InteractionMode"), (int32)CurrentSettings.InteractionMode, ConfigFile);
    GConfig->SetFloat(ConfigSection, TEXT("MovementSpeed"), CurrentSettings.MovementSpeed, ConfigFile);
    GConfig->SetBool(ConfigSection, TEXT("bSnapTurnEnabled"), CurrentSettings.bSnapTurnEnabled, ConfigFile);
    GConfig->SetFloat(ConfigSection, TEXT("SnapTurnAngle"), CurrentSettings.SnapTurnAngle, ConfigFile);
    GConfig->SetBool(ConfigSection, TEXT("bComfortVignette"), CurrentSettings.bComfortVignette, ConfigFile);
    GConfig->SetFloat(ConfigSection, TEXT("IPD"), CurrentSettings.IPD, ConfigFile);
    GConfig->SetFloat(ConfigSection, TEXT("WorldScale"), CurrentSettings.WorldScale, ConfigFile);

    GConfig->Flush(false, ConfigFile);
}

void UMingVRARSystem::LoadSettings()
{
    const FString ConfigSection = TEXT("VRARSettings");
    const FString ConfigFile = FPaths::ProjectConfigDir() / TEXT("VRAR.ini");

    int32 IntValue;
    if (GConfig->GetInt(ConfigSection, TEXT("DisplayMode"), IntValue, ConfigFile))
        CurrentSettings.DisplayMode = (EVRARDisplayMode)IntValue;
    if (GConfig->GetInt(ConfigSection, TEXT("TrackingSpace"), IntValue, ConfigFile))
        CurrentSettings.TrackingSpace = (EVRARTrackingSpace)IntValue;
    if (GConfig->GetInt(ConfigSection, TEXT("InteractionMode"), IntValue, ConfigFile))
        CurrentSettings.InteractionMode = (EVRARInteractionMode)IntValue;

    GConfig->GetFloat(ConfigSection, TEXT("MovementSpeed"), CurrentSettings.MovementSpeed, ConfigFile);
    GConfig->GetBool(ConfigSection, TEXT("bSnapTurnEnabled"), CurrentSettings.bSnapTurnEnabled, ConfigFile);
    GConfig->GetFloat(ConfigSection, TEXT("SnapTurnAngle"), CurrentSettings.SnapTurnAngle, ConfigFile);
    GConfig->GetBool(ConfigSection, TEXT("bComfortVignette"), CurrentSettings.bComfortVignette, ConfigFile);
    GConfig->GetFloat(ConfigSection, TEXT("IPD"), CurrentSettings.IPD, ConfigFile);
    GConfig->GetFloat(ConfigSection, TEXT("WorldScale"), CurrentSettings.WorldScale, ConfigFile);
}

bool UMingVRARSystem::SwitchDisplayMode(EVRARDisplayMode NewMode)
{
    if (SessionState != EVRARSessionState::Running)
        return false;
    CurrentSettings.DisplayMode = NewMode;
    return true;
}

EVRARDisplayMode UMingVRARSystem::GetCurrentDisplayMode() const
{
    return CurrentSettings.DisplayMode;
}

bool UMingVRARSystem::SetInteractionMode(EVRARInteractionMode NewMode)
{
    CurrentSettings.InteractionMode = NewMode;
    return true;
}

EVRARInteractionMode UMingVRARSystem::GetCurrentInteractionMode() const
{
    return CurrentSettings.InteractionMode;
}

void UMingVRARSystem::RecenterView()
{
    UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void UMingVRARSystem::SetSeatedOrigin(const FVector& NewOrigin)
{
}

FVector UMingVRARSystem::GetHeadPosition() const
{
    FVector Position;
    FRotator Rotation;
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
    return Position;
}

FRotator UMingVRARSystem::GetHeadRotation() const
{
    FVector Position;
    FRotator Rotation;
    UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Rotation, Position);
    return Rotation;
}

void UMingVRARSystem::EnableRTSModeInVR()
{
    if (CurrentSettings.TrackingSpace == EVRARTrackingSpace::RoomScale)
        CurrentSettings.MovementSpeed = 1.5f;
    else
        CurrentSettings.MovementSpeed = 0.8f;
    ApplySettings(CurrentSettings);
}

void UMingVRARSystem::EnableSpectatorMode()
{
    CurrentSettings.DisplayMode = EVRARDisplayMode::Theater;
    CurrentSettings.InteractionMode = EVRARInteractionMode::HeadGaze;
    ApplySettings(CurrentSettings);
}

void UMingVRARSystem::SetCommanderViewMode(bool bEnabled)
{
    if (bEnabled)
    {
        CurrentSettings.TrackingSpace = EVRARTrackingSpace::Standing;
        CurrentSettings.DisplayMode = EVRARDisplayMode::FullVR;
    }
    ApplySettings(CurrentSettings);
}

bool UMingVRARSystem::IsInVR() const
{
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled() &&
           ActiveDevice != EVRDeviceType::None;
}

bool UMingVRARSystem::IsInAR() const
{
    return ActiveDevice == EVRDeviceType::HoloLens ||
           ActiveDevice == EVRDeviceType::MagicLeap;
}

FVector2D UMingVRARSystem::WorldToVRScreenPosition(const FVector& WorldPosition)
{
    return FVector2D::ZeroVector;
}

bool UMingVRARSystem::IsVRARSupported()
{
    return true;
}

FString UMingVRARSystem::GetRuntimeVersion() const
{
    return TEXT("1.0.0");
}

bool UMingVRARSystem::SetupVRRendering()
{
    if (!UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
    {
        UHeadMountedDisplayFunctionLibrary::EnableHMD(true);
    }
    return UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled();
}

bool UMingVRARSystem::SetupARCamera()
{
    return true;
}

void UMingVRARSystem::SetupInputMappings()
{
}

void UMingVRARSystem::CleanupVRAR()
{
    if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
    {
        UHeadMountedDisplayFunctionLibrary::EnableHMD(false);
    }
}

void UMingVRARSystem::ApplyComfortSettings()
{
}

void UMingVRARSystem::CacheDeviceCapabilities()
{
    FVRDeviceCapabilities OculusQuest2;
    OculusQuest2.DeviceName = TEXT("Oculus Quest 2");
    OculusQuest2.bSupportsHandTracking = true;
    OculusQuest2.bSupportsHaptics = true;
    OculusQuest2.bSupportsRoomScale = true;
    OculusQuest2.RefreshRate = 120;
    OculusQuest2.MaxTrackingRadius = 5.0f;
    DeviceCapabilitiesCache.Add(EVRDeviceType::OculusQuest2, OculusQuest2);

    FVRDeviceCapabilities ValveIndex;
    ValveIndex.DeviceName = TEXT("Valve Index");
    ValveIndex.bSupportsHandTracking = true;
    ValveIndex.bSupportsFingerTracking = true;
    ValveIndex.bSupportsHaptics = true;
    ValveIndex.bSupportsRoomScale = true;
    ValveIndex.RefreshRate = 144;
    ValveIndex.MaxTrackingRadius = 7.0f;
    DeviceCapabilitiesCache.Add(EVRDeviceType::ValveIndex, ValveIndex);
}

void UMingVRARSystem::HandleSessionStateChange(EVRARSessionState NewState)
{
    OnSessionStateChanged.Broadcast(NewState);
}
