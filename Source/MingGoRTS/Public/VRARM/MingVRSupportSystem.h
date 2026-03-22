// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR Support System - C1-1
// Provides VR device compatibility and VR-specific features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingVRSupportSystem.generated.h"

UENUM(BlueprintType)
enum class EVRDeviceType: uint8 {
    None = 0, UMETA(DisplayName = "None"),
    OculusRift, UMETA(DisplayName = "Oculus Rift"),
    OculusQuest, UMETA(DisplayName = "Oculus Quest"),
    HTC_Vive, UMETA(DisplayName = "HTC Vive"),
    HTC_VivePro, UMETA(DisplayName = "HTC Vive Pro"),
    ValveIndex, UMETA(DisplayName = "Valve Index"),
    WindowsMR, UMETA(DisplayName = "Windows Mixed Reality"),
    PlayStationVR, UMETA(DisplayName = "PlayStation VR"),
    Pico, UMETA(DisplayName = "Pico VR"),
    GenericHMD UMETA(DisplayName = "Generic HMD")
};

UENUM(BlueprintType)
enum class EVRTrackingSpace: uint8 {
    Seated = 0, UMETA(DisplayName = "Seated"),
    Standing, UMETA(DisplayName = "Standing"),
    RoomScale, UMETA(DisplayName = "Room Scale"),
    Custom UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EVRControllerType: uint8 {
    MotionController = 0, UMETA(DisplayName = "Motion Controller"),
    Gamepad, UMETA(DisplayName = "Gamepad"),
    KeyboardMouse, UMETA(DisplayName = "Keyboard & Mouse"),
    HandTracking, UMETA(DisplayName = "Hand Tracking"),
    EyeTracking, UMETA(DisplayName = "Eye Tracking"),
    VoiceControl UMETA(DisplayName = "Voice Control")
};

USTRUCT(BlueprintType)
struct FVRDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY()
    EVRDeviceType DeviceType;

    UPROPERTY()
    FString DeviceName;

    UPROPERTY()
    FString Manufacturer;

    UPROPERTY()
    FString Model;

    UPROPERTY()
    FString SerialNumber;

    UPROPERTY()
    bool bIsConnected;

    UPROPERTY()
    bool bIsTracking;

    UPROPERTY()
    FIntPoint DisplayResolution;

    UPROPERTY()
    float RefreshRate;

    UPROPERTY()
    float IPD;

    UPROPERTY()
    float FOV;

    UPROPERTY()
    FString DriverVersion;

    UPROPERTY()
    FString FirmwareVersion;

    FVRDeviceInfo()
        : DeviceType(EVRDeviceType::None)
        , bIsConnected(false)
        , bIsTracking(false)
        , DisplayResolution(0, 0)
        , RefreshRate(90.0f)
        , IPD(63.0f)
        , FOV(110.0f)
    {}
};

USTRUCT(BlueprintType)
struct FVRTrackingData
{
    GENERATED_BODY()

    UPROPERTY()
    FVector HeadPosition;

    UPROPERTY()
    FQuat HeadRotation;

    UPROPERTY()
    FVector LeftHandPosition;

    UPROPERTY()
    FQuat LeftHandRotation;

    UPROPERTY()
    FVector RightHandPosition;

    UPROPERTY()
    FQuat RightHandRotation;

    UPROPERTY()
    FVector2D LeftThumbstick;

    UPROPERTY()
    FVector2D RightThumbstick;

    UPROPERTY()
    float LeftTrigger;

    UPROPERTY()
    float RightTrigger;

    UPROPERTY()
    float LeftGrip;

    UPROPERTY()
    float RightGrip;

    UPROPERTY()
    uint32 Timestamp;

    FVRTrackingData()
        : LeftTrigger(0.0f)
        , RightTrigger(0.0f)
        , LeftGrip(0.0f)
        , RightGrip(0.0f)
        , Timestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FVRComfortSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bEnableVignette;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float VignetteIntensity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bSnapTurn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float SnapTurnAngle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bSmoothTurn;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float SmoothTurnSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bTeleportMovement;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bSmoothLocomotion;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float SmoothLocomotionSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bEnableComfortZone;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float ComfortZoneRadius;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bReducedMotion;

    FVRComfortSettings()
        : bEnableVignette(false)
        , VignetteIntensity(0.5f)
        , bSnapTurn(true)
        , SnapTurnAngle(45.0f)
        , bSmoothTurn(false)
        , SmoothTurnSpeed(90.0f)
        , bTeleportMovement(true)
        , bSmoothLocomotion(false)
        , SmoothLocomotionSpeed(3.0f)
        , bEnableComfortZone(true)
        , ComfortZoneRadius(1000.0f)
        , bReducedMotion(false)
    {}
};

USTRUCT(BlueprintType)
struct FVRHapticFeedback
{
    GENERATED_BODY()

    UPROPERTY()
    EName Bone;

    UPROPERTY()
    float Amplitude;

    UPROPERTY()
    float Frequency;

    UPROPERTY()
    float Duration;

    UPROPERTY()
    EVRControllerType Controller;

    FVRHapticFeedback()
        : Bone(NAME_None)
        , Amplitude(0.5f)
        , Frequency(1.0f)
        , Duration(0.1f)
        , Controller(EVRControllerType::MotionController)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceConnected, FVRDeviceInfo, DeviceInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceDisconnected, EVRDeviceType, DeviceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRTrackingUpdated, FVRTrackingData, TrackingData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRControllerButtonPressed, EName, Button);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRHapticRequested, EVRControllerType, Controller, FVRHapticFeedback, Haptic);

/**
 * VR Support System
 * Provides VR device compatibility and VR-specific features
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingVRSupportSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingVRSupportSystem();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void InitializeVRSystem();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void ShutdownVRSystem();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVRDeviceConnected() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRDeviceType GetConnectedDeviceType() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVRDeviceInfo GetDeviceInfo() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    TArray<FVRDeviceInfo> GetAllConnectedDevices() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool InitializeDevice(EVRDeviceType DeviceType);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetTrackingSpace(EVRTrackingSpace TrackingSpace);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRTrackingSpace GetTrackingSpace() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVRTrackingData GetCurrentTrackingData() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVector GetHeadPosition() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FRotator GetHeadRotation() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool GetControllerPositionAndOrientation(EVRControllerType Controller, FVector& OutPosition, FRotator& OutOrientation) const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetComfortSettings(const FVRComfortSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVRComfortSettings GetComfortSettings() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void TriggerHapticFeedback(const FVRHapticFeedback& Haptic);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void PlayHapticEffect(const FString& EffectName, EVRControllerType Controller, float Scale = 1.0f);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void StopHapticFeedback(EVRControllerType Controller);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void EnableVRMode(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVRModeEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void RecenterTrackingOrigin();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetWorldToMetersScale(float Scale);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    float GetWorldToMetersScale() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void CalibrateIPD(float MeasuredIPD);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    float GetCurrentIPD() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetInterpupillaryDistance(float NewIPD);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsHeadMountedDisplayEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void EnableSpectatorScreenMode(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsSpectatorScreenModeEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetVRPerformanceLevel(int32 Level);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    int32 GetVRPerformanceLevel() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void OptimizeForVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void EnableVRHandTracking(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsHandTrackingEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetVRRenderingResolution(float ResolutionScale);

    UFUNCTION(BlueprintPure, Category = "VR Support")
    bool HasValidTrackingPosition() const;

    UFUNCTION(BlueprintPure, Category = "VR Support")
    bool IsTrackedDeviceConnected(int32 DeviceId) const;

    UPROPERTY(BlueprintAssignable, Category = "VR Events")
    FOnVRDeviceConnected OnVRDeviceConnected;

    UPROPERTY(BlueprintAssignable, Category = "VR Events")
    FOnVRDeviceDisconnected OnVRDeviceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "VR Events")
    FOnVRTrackingUpdated OnVRTrackingUpdated;

    UPROPERTY(BlueprintAssignable, Category = "VR Events")
    FOnVRControllerButtonPressed OnVRControllerButtonPressed;

    UPROPERTY(BlueprintAssignable, Category = "VR Events")
    FOnVRHapticRequested OnVRHapticRequested;

protected:
    UPROPERTY()
    bool bVRModeEnabled;

    UPROPERTY()
    bool bHandTrackingEnabled;

    UPROPERTY()
    EVRDeviceType ConnectedDevice;

    UPROPERTY()
    FVRDeviceInfo DeviceInfo;

    UPROPERTY()
    FVRComfortSettings ComfortSettings;

    UPROPERTY()
    float WorldToMetersScale;

    UPROPERTY()
    int32 PerformanceLevel;

    UPROPERTY()
    FTimerHandle TrackingUpdateTimer;

    UPROPERTY()
    FVRTrackingData CurrentTrackingData;

    void StartTrackingUpdate();
    void StopTrackingUpdate();
    void UpdateTrackingData();
    bool InitializeVRDevice(EVRDeviceType DeviceType);
    void ShutdownVRDevice();
    void OnDeviceConnected(EVRDeviceType DeviceType);
    void OnDeviceDisconnected(EVRDeviceType DeviceType);
    void ProcessVRInput();
    void ApplyComfortSettings();
    void UpdateVRPerformance();
    static UMingVRSupportSystem* Get(UObject* WorldContextObject);
};
