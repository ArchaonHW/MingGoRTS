// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - Enhanced VR/AR Integration

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "VRAR/MingRTSVRSupport.h"
#include "VRAR/MingRTSARSupport.h"
#include "MingRTSVARIntegrationSystem.generated.h"

/**
 * VR/AR Integration Modes
 */
UENUM(BlueprintType)
enum class EVARIntegrationMode : uint8 {
    VR_Only         UMETA(DisplayName = "VR Only"),
    AR_Only         UMETA(DisplayName = "AR Only"),
    Mixed           UMETA(DisplayName = "Mixed VR/AR"),
    Auto            UMETA(DisplayName = "Auto Detect"),
    Disabled        UMETA(DisplayName = "Disabled")
};

/**
 * VR/AR Performance Profiles
 */
UENUM(BlueprintType)
enum class EVARPerformanceProfile : uint8 {
    UltraLow        UMETA(DisplayName = "Ultra Low - Mobile"),
    Low             UMETA(DisplayName = "Low - Entry Level"),
    Medium          UMETA(DisplayName = "Medium - Standard"),
    High            UMETA(DisplayName = "High - Premium"),
    Ultra           UMETA(DisplayName = "Ultra - High End"),
    Custom          UMETA(DisplayName = "Custom")
};

/**
 * VR/AR Interaction Types
 */
UENUM(BlueprintType)
enum class EVARInteractionType : uint8 {
    Gaze            UMETA(DisplayName = "Gaze Based"),
    Controller       UMETA(DisplayName = "Controller Based"),
    HandTracking     UMETA(DisplayName = "Hand Tracking"),
    Voice            UMETA(DisplayName = "Voice Commands"),
    Gesture          UMETA(DisplayName = "Gesture Recognition"),
    Hybrid           UMETA(DisplayName = "Hybrid Multiple")
};

/**
 * VR/AR UI Scaling Modes
 */
UENUM(BlueprintType)
enum class EVARUIScalingMode : uint8 {
    Fixed            UMETA(DisplayName = "Fixed Scale"),
    DistanceBased     UMETA(DisplayName = "Distance Based"),
    Adaptive         UMETA(DisplayName = "Adaptive"),
    Auto             UMETA(DisplayName = "Automatic")
};

/**
 * VR/AR Integration Configuration
 */
USTRUCT(BlueprintType)
struct FVARIntegrationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    EVARIntegrationMode IntegrationMode = EVARIntegrationMode::Auto;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    EVARPerformanceProfile PerformanceProfile = EVARPerformanceProfile::Medium;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    EVARInteractionType PrimaryInteraction = EVARInteractionType::Controller;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    EVARUIScalingMode UIScalingMode = EVARUIScalingMode::Adaptive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    bool bEnableCrossPlatform = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    bool bEnablePerformanceMonitoring = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    bool bEnableAdaptiveQuality = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    float TargetFrameRate = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    int32 MaxRenderDistance = 10000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Integration")
    bool bEnableSpatialAudio = true;
};

/**
 * VR/AR System Performance Metrics
 */
USTRUCT(BlueprintType)
struct FVARPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float CurrentFrameRate = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float AverageFrameRate = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float FrameTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    int32 DrawCalls = 0;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float GPUUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float CPUUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    int64 MemoryUsage = 0;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    float NetworkLatency = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    EVARPerformanceProfile CurrentProfile = EVARPerformanceProfile::Medium;

    UPROPERTY(BlueprintReadOnly, Category = "VR/AR Performance")
    bool bIsPerformanceOptimal = true;
};

/**
 * Enhanced VR/AR Integration System
 * Provides unified management for both VR and AR functionality
 */
UCLASS()
class MINGRTS_API UMingRTSVARIntegrationSystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void InitializeVARRIntegration();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void ShutdownVARRIntegration();

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SetIntegrationConfig(const FVARIntegrationConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    FVARIntegrationConfig GetIntegrationConfig() const { return IntegrationConfig; }

    // Mode Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SwitchToVRMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SwitchToARMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SwitchToMixedMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    EVARIntegrationMode GetCurrentMode() const { return CurrentMode; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    bool IsVREnabled() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    bool IsAREnabled() const;

    // Performance Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SetPerformanceProfile(EVARPerformanceProfile Profile);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    EVARPerformanceProfile GetCurrentPerformanceProfile() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    FVARPerformanceMetrics GetPerformanceMetrics() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void EnableAdaptiveQuality(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    bool IsAdaptiveQualityEnabled() const { return bAdaptiveQualityEnabled; }

    // UI Adaptation
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void AdaptUIForCurrentMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SetUIScalingMode(EVARUIScalingMode Mode);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    EVARUIScalingMode GetUIScalingMode() const { return CurrentUIScalingMode; }

    // Interaction Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SetPrimaryInteractionType(EVARInteractionType Type);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    EVARInteractionType GetPrimaryInteractionType() const { return PrimaryInteractionType; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    TArray<EVARInteractionType> GetAvailableInteractionTypes() const;

    // Cross-Platform Support
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    bool IsCrossPlatformEnabled() const { return bCrossPlatformEnabled; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void EnableCrossPlatformSupport(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void SyncSettingsAcrossPlatforms();

    // Device Detection and Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    bool DetectAndInitializeBestMode();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    TArray<EVRHeadsetType> GetSupportedVRDevices() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    TArray<EARDeviceType> GetSupportedARDevices() const;

    // Quality and Optimization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void OptimizeForDevice();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void ApplyQualitySettings();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Integration")
    void ResetToDefaults();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR/AR Integration|Events")
    FOnVARRModeChanged OnVARRModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Integration|Events")
    FOnVARRPerformanceProfileChanged OnVARRPerformanceProfileChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Integration|Events")
    FOnVARRDeviceConnected OnVARRDeviceConnected;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Integration|Events")
    FOnVARRDeviceDisconnected OnVARRDeviceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Integration|Events")
    FOnVARRPerformanceWarning OnVARRPerformanceWarning;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

protected:
    // Internal Systems
    UPROPERTY()
    TObjectPtr<UMingRTSVRSupport> VRSupportSystem;

    UPROPERTY()
    TObjectPtr<UMingRTSARSupport> ARSupportSystem;

    // Configuration
    UPROPERTY()
    FVARIntegrationConfig IntegrationConfig;

    UPROPERTY()
    EVARIntegrationMode CurrentMode = EVARIntegrationMode::Disabled;

    UPROPERTY()
    EVARPerformanceProfile CurrentPerformanceProfile = EVARPerformanceProfile::Medium;

    UPROPERTY()
    EVARInteractionType PrimaryInteractionType = EVARInteractionType::Controller;

    UPROPERTY()
    EVARUIScalingMode CurrentUIScalingMode = EVARUIScalingMode::Adaptive;

    UPROPERTY()
    bool bCrossPlatformEnabled = true;

    UPROPERTY()
    bool bAdaptiveQualityEnabled = true;

    UPROPERTY()
    FVARPerformanceMetrics CurrentMetrics;

    // Performance Monitoring
    UPROPERTY()
    bool bPerformanceMonitoringEnabled = true;

    UPROPERTY()
    float PerformanceUpdateInterval = 1.0f;

    FTimerHandle PerformanceUpdateTimer;

    // Internal Methods
    void InitializeInternalSystems();
    void UpdatePerformanceMetrics();
    void ApplyPerformanceProfile(EVARPerformanceProfile Profile);
    void HandleModeChange(EVARIntegrationMode NewMode);
    void OptimizeForCurrentDevice();
    void SetupDefaultConfiguration();
    void ValidateConfiguration();
    void OnVRDeviceConnected(EVRHeadsetType HeadsetType);
    void OnVRDeviceDisconnected();
    void OnARSessionStarted();
    void OnARSessionStopped();
    void OnTrackingQualityChanged(EARTrackingQuality Quality);
    void BroadcastPerformanceWarning();
    bool ShouldSwitchToLowPerformanceMode();
    void EnableAutomaticQualityAdjustment();
    void DisableAutomaticQualityAdjustment();
};

// Event delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVARRModeChanged, EVARIntegrationMode, OldMode, EVARIntegrationMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVARRPerformanceProfileChanged, EVARPerformanceProfile, NewProfile);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVARRDeviceConnected, const FString&, DeviceName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVARRDeviceDisconnected, const FString&, DeviceName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVARRPerformanceWarning, const FString&, WarningMessage);
