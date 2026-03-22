// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR Support Component

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "XRBase.h"
#include "MingRTSVRSupport.generated.h"

/**
 * VR Headset Types supported by MingGoRTS
 */
UENUM(BlueprintType)
enum class EVRHeadsetType: uint8 {
    None            UMETA(DisplayName = "None"),
    OculusRift      UMETA(DisplayName = "Oculus Rift"),
    OculusQuest     UMETA(DisplayName = "Oculus Quest"),
    HTC_Vive        UMETA(DisplayName = "HTC Vive"),
    Valve_Index     UMETA(DisplayName = "Valve Index"),
    WindowsMR       UMETA(DisplayName = "Windows Mixed Reality"),
    PSVR            UMETA(DisplayName = "PlayStation VR"),
    Custom          UMETA(DisplayName = "Custom Device")
};

/**
 * VR Tracking Space modes
 */
UENUM(BlueprintType)
enum class EVRTrackingSpace: uint8 {
    Stationary  UMETA(DisplayName = "Stationary - Seated/Standing"),
    RoomScale   UMETA(DisplayName = "Room Scale"),
    WorldScale  UMETA(DisplayName = "World Scale")
};

/**
 * VR System Status
 */
UENUM(BlueprintType)
enum class EVRSystemStatus: uint8 {
    NotInitialized  UMETA(DisplayName = "Not Initialized"),
    Initializing    UMETA(DisplayName = "Initializing"),
    Ready           UMETA(DisplayName = "Ready"),
    Active          UMETA(DisplayName = "Active"),
    Error           UMETA(DisplayName = "Error"),
    ShuttingDown    UMETA(DisplayName = "Shutting Down")
};

/**
 * VR Comfort Options
 */
USTRUCT(BlueprintType)
struct FVRComfortSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bUseVignette = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bUseTeleport = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bSnapTurn = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float SnapTurnAngle = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    float MovementSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Comfort")
    bool bUseComfortMode = false;
};

/**
 * VR Device Info
 */
USTRUCT(BlueprintType)
struct FVRDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    EVRHeadsetType HeadsetType = EVRHeadsetType::None;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    FString DeviceName;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    FVector2D DisplayResolution;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    float RefreshRate = 90.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    bool bHasHandTracking = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    bool bHasEyeTracking = false;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    bool bHasHapticFeedback = true;

    UPROPERTY(BlueprintReadOnly, Category = "VR Device")
    int32 ControllerCount = 2;
};

/**
 * MingGoRTS VR Support System
 * Provides comprehensive VR functionality for the RTS game
 */
UCLASS()
class MINGRTS_API UMingRTSVRSupport : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void InitializeVRSupport();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void ShutdownVRSupport();

    // Device Management
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVRDeviceConnected() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVRDeviceInfo GetVRDeviceInfo() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRHeadsetType DetectVRHeadsetType() const;

    // System Control
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool EnableVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void DisableVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVREnabled() const { return bVREnabled; };

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRSystemStatus GetVRStatus() const { return VRStatus; }

    // Tracking and Space
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetTrackingSpace(EVRTrackingSpace Space);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRTrackingSpace GetTrackingSpace() const { return CurrentTrackingSpace; };

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void RecenterHMD();

    // Comfort Settings
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetComfortSettings(const FVRComfortSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    FVRComfortSettings GetComfortSettings() const { return ComfortSettings; }

    // Performance Optimization
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetVROptimizationLevel(int32 Level}; // 0-3 (Low to Ultra)

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    int32 GetCurrentVROptimizationLevel() const;

    // UI Adaptation
    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void AdaptUIToVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void RestoreUIFromVR();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR Support|Events")
    FOnVRDeviceConnected OnVRDeviceConnected;

    UPROPERTY(BlueprintAssignable, Category = "VR Support|Events")
    FOnVRDeviceDisconnected OnVRDeviceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "VR Support|Events")
    FOnVRStatusChanged OnVRStatusChanged;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    UPROPERTY()
    bool bVREnabled = false;

    UPROPERTY()
    EVRSystemStatus VRStatus = EVRSystemStatus::NotInitialized;

    UPROPERTY()
    EVRTrackingSpace CurrentTrackingSpace = EVRTrackingSpace::Stationary;

    UPROPERTY()
    FVRComfortSettings ComfortSettings;

    UPROPERTY()
    FVRDeviceInfo CurrentDeviceInfo;

    UPROPERTY()
    int32 VROptimizationLevel = 2; // Default to High

    void OnVRDeviceConnectionChanged(bool bConnected);
    void UpdateVRStatus(EVRSystemStatus NewStatus);
    void ApplyVROptimizationSettings();
    void SetupVRInputMappings();
};

// Event delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceConnected, EVRHeadsetType, HeadsetType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVRDeviceDisconnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRStatusChanged, EVRSystemStatus, OldStatus, EVRSystemStatus, NewStatus};

