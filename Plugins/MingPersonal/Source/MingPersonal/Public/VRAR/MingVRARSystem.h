// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR/AR Support System - Epic 9.1
//
// This system provides comprehensive VR/AR support for MingGoRTS,
// enabling immersive gameplay on VR headsets and AR devices.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "InputCoreTypes.h"
#include "MingVRARSystem.generated.h"

// VR/AR device types supported
UENUM(BlueprintType)
enum class EVRDeviceType: uint8 {
    None            UMETA(DisplayName = "None"),
    OculusRift      UMETA(DisplayName = "Oculus Rift"),
    OculusQuest     UMETA(DisplayName = "Oculus Quest"),
    OculusQuest2    UMETA(DisplayName = "Oculus Quest 2"),
    HTC_Vive        UMETA(DisplayName = "HTC Vive"),
    HTC_VivePro     UMETA(DisplayName = "HTC Vive Pro"),
    ValveIndex      UMETA(DisplayName = "Valve Index"),
    PSVR            UMETA(DisplayName = "PlayStation VR"),
    PSVR2           UMETA(DisplayName = "PlayStation VR2"),
    WindowsMR       UMETA(DisplayName = "Windows Mixed Reality"),
    GenericVR       UMETA(DisplayName = "Generic VR Device"),
    HoloLens        UMETA(DisplayName = "Microsoft HoloLens"),
    MagicLeap       UMETA(DisplayName = "Magic Leap"),
    iOS_AR          UMETA(DisplayName = "iOS ARKit"),
    Android_AR      UMETA(DisplayName = "Android ARCore"),
    Custom          UMETA(DisplayName = "Custom Device")
};

// VR/AR session states
UENUM(BlueprintType)
enum class EVRARSessionState: uint8 {
    Idle                UMETA(DisplayName = "Idle"),
    Initializing        UMETA(DisplayName = "Initializing"),
    Ready               UMETA(DisplayName = "Ready"),
    Running             UMETA(DisplayName = "Running"),
    Paused              UMETA(DisplayName = "Paused"),
    Error               UMETA(DisplayName = "Error"),
    ShuttingDown        UMETA(DisplayName = "Shutting Down")
};

// VR/AR interaction modes
UENUM(BlueprintType)
enum class EVRARInteractionMode: uint8 {
    Controller          UMETA(DisplayName = "Controller"),
    HandTracking        UMETA(DisplayName = "Hand Tracking"),
    Gesture             UMETA(DisplayName = "Gesture"),
    EyeTracking         UMETA(DisplayName = "Eye Tracking"),
    Voice               UMETA(DisplayName = "Voice Control"),
    Mixed               UMETA(DisplayName = "Mixed Input"),
    HeadGaze            UMETA(DisplayName = "Head Gaze")
};

// VR/AR display modes
UENUM(BlueprintType)
enum class EVRARDisplayMode: uint8 {
    FullVR              UMETA(DisplayName = "Full VR"),
    AR_Passthrough      UMETA(DisplayName = "AR Passthrough"),
    AR_Overlay          UMETA(DisplayName = "AR Overlay"),
    MixedReality        UMETA(DisplayName = "Mixed Reality"),
    Theater             UMETA(DisplayName = "Theater Mode")
};

// VR/AR tracking space
UENUM(BlueprintType)
enum class EVRARTrackingSpace: uint8 {
    Seated              UMETA(DisplayName = "Seated"),
    Standing            UMETA(DisplayName = "Standing"),
    RoomScale           UMETA(DisplayName = "Room Scale"),
    WorldScale          UMETA(DisplayName = "World Scale")
};

// Device capability info
USTRUCT(BlueprintType)
struct FVRDeviceCapabilities
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHandTracking = false;

    UPROPERTY(BlueprintReadOnly)
    bool bSupportsEyeTracking = false;

    UPROPERTY(BlueprintReadOnly)
    bool bSupportsHaptics = false;

    UPROPERTY(BlueprintReadOnly)
    bool bSupportsFingerTracking = false;

    UPROPERTY(BlueprintReadOnly)
    bool bSupportsRoomScale = false;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxControllers = 2;

    UPROPERTY(BlueprintReadOnly)
    float MaxTrackingRadius = 3.0f; // meters

    UPROPERTY(BlueprintReadOnly)
    int32 RefreshRate = 90;

    UPROPERTY(BlueprintReadOnly)
    FString DeviceName = TEXT("Unknown");
};

// VR/AR settings configuration
USTRUCT(BlueprintType)
struct FVRARSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRARDisplayMode DisplayMode = EVRARDisplayMode::FullVR;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRARTrackingSpace TrackingSpace = EVRARTrackingSpace::RoomScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRARInteractionMode InteractionMode = EVRARInteractionMode::Controller;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MovementSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TurnSmoothing = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSnapTurnEnabled = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SnapTurnAngle = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bComfortVignette = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IPD = 0.064f; // meters

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WorldScale = 1.0f;
};

// VR/AR system - Main subsystem managing VR/AR functionality
UCLASS()
class MINGPERSONAL_API UMingVRARSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingVRARSystem();

    // Subsystem lifecycle
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // Session management
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Session")
    bool InitializeVRARSession(EVRDeviceType DeviceType);

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Session")
    void ShutdownVRARSession();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Session")
    void PauseVRARSession();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Session")
    void ResumeVRARSession();

    UFUNCTION(BlueprintPure, Category = "VR/AR|Session")
    EVRARSessionState GetSessionState() const { return SessionState; }

    UFUNCTION(BlueprintPure, Category = "VR/AR|Session")
    EVRDeviceType GetActiveDevice() const { return ActiveDevice; }

    // Device detection and capabilities
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Device")
    TArray<EVRDeviceType> GetAvailableDevices();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Device")
    FVRDeviceCapabilities GetDeviceCapabilities(EVRDeviceType DeviceType);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Device")
    bool IsDeviceConnected(EVRDeviceType DeviceType) const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Device")
    bool AutoDetectAndInitialize();

    // Settings management
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Settings")
    void ApplySettings(const FVRARSettings& NewSettings);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Settings")
    FVRARSettings GetCurrentSettings() const { return CurrentSettings; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Settings")
    void ResetToDefaultSettings();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Settings")
    void SaveSettings();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Settings")
    void LoadSettings();

    // Display mode switching
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Display")
    bool SwitchDisplayMode(EVRARDisplayMode NewMode);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Display")
    EVRARDisplayMode GetCurrentDisplayMode() const;

    // Interaction mode
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Interaction")
    bool SetInteractionMode(EVRARInteractionMode NewMode);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Interaction")
    EVRARInteractionMode GetCurrentInteractionMode() const;

    // Camera and view
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Camera")
    void RecenterView();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Camera")
    void SetSeatedOrigin(const FVector& NewOrigin);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Camera")
    FVector GetHeadPosition() const;

    UFUNCTION(BlueprintPure, Category = "VR/AR|Camera")
    FRotator GetHeadRotation() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR/AR|Events")
    FOnVRARSessionStateChanged OnSessionStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR|Events")
    FOnVRARDeviceConnected OnDeviceConnected;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR|Events")
    FOnVRARDeviceDisconnected OnDeviceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR|Events")
    FOnVRARTrackingLost OnTrackingLost;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR|Events")
    FOnVRARTrackingRestored OnTrackingRestored;

    // Gameplay integration
    UFUNCTION(BlueprintCallable, Category = "VR/AR|Gameplay")
    void EnableRTSModeInVR();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Gameplay")
    void EnableSpectatorMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR|Gameplay")
    void SetCommanderViewMode(bool bEnabled);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Gameplay")
    bool IsInVR() const;

    UFUNCTION(BlueprintPure, Category = "VR/AR|Gameplay")
    bool IsInAR() const;

    // Utility functions
    UFUNCTION(BlueprintPure, Category = "VR/AR|Utility")
    static FVector2D WorldToVRScreenPosition(const FVector& WorldPosition);

    UFUNCTION(BlueprintPure, Category = "VR/AR|Utility")
    static bool IsVRARSupported();

    UFUNCTION(BlueprintPure, Category = "VR/AR|Utility")
    FString GetRuntimeVersion() const;

protected:
    // Internal state
    UPROPERTY()
    EVRARSessionState SessionState = EVRARSessionState::Idle;

    UPROPERTY()
    EVRDeviceType ActiveDevice = EVRDeviceType::None;

    UPROPERTY()
    FVRARSettings CurrentSettings;

    UPROPERTY()
    TMap<EVRDeviceType, FVRDeviceCapabilities> DeviceCapabilitiesCache;

    // Internal methods
    bool SetupVRRendering();
    bool SetupARCamera();
    void SetupInputMappings();
    void CleanupVRAR();
    void OnTrackingStateChanged(bool bHasTracking);
    void UpdateHeadPose();
    void ApplyComfortSettings();

    // Delegates
    void HandleSessionStateChange(EVRARSessionState NewState);
    void HandleDeviceConnection(EVRDeviceType Device);
    void HandleDeviceDisconnection(EVRDeviceType Device);
};

// Event delegates
declare dynamic multicast delegate(FOnVRARSessionStateChanged, EVRARSessionState, NewState);
declare dynamic multicast delegate(FOnVRARDeviceConnected, EVRDeviceType, DeviceType);
declare dynamic multicast delegate(FOnVRARDeviceDisconnected, EVRDeviceType, DeviceType);
declare dynamic multicast delegate(FOnVRARTrackingLost);
declare dynamic multicast delegate(FOnVRARTrackingRestored);
