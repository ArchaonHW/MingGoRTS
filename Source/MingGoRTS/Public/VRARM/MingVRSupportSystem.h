// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR Support System - C1-1
// Provides VR device compatibility and VR-specific features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingVRSupportSystem.generated.h"

UENUM(BlueprintType)
enum class EVRDeviceType : uint8 {
    None                UMETA(DisplayName = "None"),
    OculusRift          UMETA(DisplayName = "Oculus Rift"),
    HTCVive             UMETA(DisplayName = "HTC Vive"),
    WindowsMR           UMETA(DisplayName = "Windows Mixed Reality"),
    ValveIndex          UMETA(DisplayName = "Valve Index"),
    PlayStationVR       UMETA(DisplayName = "PlayStation VR"),
    Pico                UMETA(DisplayName = "Pico"),
    MobileVR            UMETA(DisplayName = "行動VR (例如: Gear VR, Daydream)"),
    Custom              UMETA(DisplayName = "自定義")
};

UENUM(BlueprintType)
enum class EVRTrackingSpace : uint8 {
    Seated              UMETA(DisplayName = "Seated"),
    Standing            UMETA(DisplayName = "Standing"),
    RoomScale           UMETA(DisplayName = "Room Scale"),
    Custom              UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EVRControllerType : uint8 {
    MotionController    UMETA(DisplayName = "Motion Controller"),
    Gamepad             UMETA(DisplayName = "Gamepad"),
    KeyboardMouse       UMETA(DisplayName = "Keyboard & Mouse"),
    HandTracking        UMETA(DisplayName = "Hand Tracking"),
    EyeTracking         UMETA(DisplayName = "Eye Tracking"),
    VoiceControl        UMETA(DisplayName = "Voice Control")
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

    FVRDeviceInfo()
        : DeviceType(EVRDeviceType::None)
        , bIsConnected(false)
        , bIsTracking(false)
        , DisplayResolution(2160, 1200)
        , RefreshRate(90.0f)
        , IPD(63.0f)
        , FOV(110.0f)
    {}
};

/**
 * VR Support System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingVRSupportSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingVRSupportSystem();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void InitializeVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void ShutdownVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVRConnected() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRDeviceType GetConnectedDevice() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetTrackingSpace(EVRTrackingSpace Space);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRTrackingSpace GetTrackingSpace() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void RecenterTrackingOrigin();

    UFUNCTION(BlueprintPure, Category = "VR Support")
    bool IsHeadsetTracked() const;

    UFUNCTION(BlueprintPure, Category = "VR Support")
    FTransform GetHeadPose() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    TArray<FVRDeviceInfo> GetSupportedDevices() const;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bVRActive;

    UPROPERTY()
    EVRDeviceType CurrentDevice;

    UPROPERTY()
    EVRTrackingSpace CurrentTrackingSpace;

    UPROPERTY()
    FVRDeviceInfo DeviceInfo;

    UPROPERTY()
    FTransform HeadPose;

    void InitializeDevice(EVRDeviceType DeviceType);
    void UpdateTrackingData();
    void ProcessControllerInput();
};
