// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - Enhanced VR/AR Integration Implementation

#include "VRAR/MingRTSVARIntegrationSystem.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "HAL/PlatformFilemanager.h"
#include "Stats/Stats.h"
#include "Logging/LogMacros.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingVARRIntegration, Log, All);

UMingRTSVARIntegrationSystem::UMingRTSVARIntegrationSystem()
{
    // Set default configuration
    SetupDefaultConfiguration();
}

void UMingRTSVARIntegrationSystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Initializing MingRTSVARIntegrationSystem..."));
    
    // Initialize internal systems
    InitializeInternalSystems();
    
    // Detect and initialize best mode
    DetectAndInitializeBestMode();
    
    // Start performance monitoring
    if (bPerformanceMonitoringEnabled)
    {
        GetWorld()->GetTimerManager().SetTimer(
            PerformanceUpdateTimer,
            this,
            &UMingRTSVARIntegrationSystem::UpdatePerformanceMetrics,
            PerformanceUpdateInterval,
            true
        );
    }
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("MingRTSVARIntegrationSystem initialized successfully"));
}

void UMingRTSVARIntegrationSystem::Deinitialize()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Shutting down MingRTSVARIntegrationSystem..."));
    
    // Clear performance timer
    if (PerformanceUpdateTimer.IsValid())
    {
        GetWorld()->GetTimerManager().ClearTimer(PerformanceUpdateTimer);
    }
    
    // Shutdown internal systems
    if (VRSupportSystem)
    {
        VRSupportSystem->ShutdownVRSupport();
    }
    
    if (ARSupportSystem)
    {
        ARSupportSystem->ShutdownARSupport();
    }
    
    CurrentMode = EVARIntegrationMode::Disabled;
    
    Super::Deinitialize();
}

void UMingRTSVARIntegrationSystem::InitializeVARRIntegration()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Initializing VR/AR Integration..."));
    
    // Validate configuration
    ValidateConfiguration();
    
    // Initialize internal systems
    InitializeInternalSystems();
    
    // Apply initial configuration
    ApplyPerformanceProfile(CurrentPerformanceProfile);
    AdaptUIForCurrentMode();
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("VR/AR Integration initialized"));
}

void UMingRTSVARIntegrationSystem::ShutdownVARRIntegration()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Shutting down VR/AR Integration..."));
    
    // Shutdown both VR and AR systems
    if (VRSupportSystem)
    {
        VRSupportSystem->ShutdownVRSupport();
    }
    
    if (ARSupportSystem)
    {
        ARSupportSystem->ShutdownARSupport();
    }
    
    CurrentMode = EVARIntegrationMode::Disabled;
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("VR/AR Integration shutdown complete"));
}

void UMingRTSVARIntegrationSystem::SetIntegrationConfig(const FVARIntegrationConfig& Config)
{
    IntegrationConfig = Config;
    
    // Apply configuration changes
    CurrentMode = Config.IntegrationMode;
    CurrentPerformanceProfile = Config.PerformanceProfile;
    PrimaryInteractionType = Config.PrimaryInteraction;
    CurrentUIScalingMode = Config.UIScalingMode;
    bCrossPlatformEnabled = Config.bEnableCrossPlatform;
    bPerformanceMonitoringEnabled = Config.bEnablePerformanceMonitoring;
    bAdaptiveQualityEnabled = Config.bEnableAdaptiveQuality;
    
    // Apply changes to systems
    ApplyPerformanceProfile(CurrentPerformanceProfile);
    AdaptUIForCurrentMode();
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("VR/AR Integration configuration updated"));
}

void UMingRTSVARIntegrationSystem::SwitchToVRMode()
{
    if (CurrentMode == EVARIntegrationMode::VR_Only)
    {
        UE_LOG(LogMingVARRIntegration, Warning, TEXT("Already in VR mode"));
        return;
    }
    
    EVARIntegrationMode OldMode = CurrentMode;
    CurrentMode = EVARIntegrationMode::VR_Only;
    
    // Shutdown AR if running
    if (ARSupportSystem && ARSupportSystem->IsARSessionRunning())
    {
        ARSupportSystem->StopARSession();
    }
    
    // Initialize VR
    if (VRSupportSystem)
    {
        VRSupportSystem->InitializeVRSupport();
        if (VRSupportSystem->IsVRDeviceConnected())
        {
            VRSupportSystem->EnableVR();
        }
    }
    
    // Adapt UI for VR
    AdaptUIForCurrentMode();
    
    HandleModeChange(CurrentMode);
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Switched to VR mode"));
}

void UMingRTSVARIntegrationSystem::SwitchToARMode()
{
    if (CurrentMode == EVARIntegrationMode::AR_Only)
    {
        UE_LOG(LogMingVARRIntegration, Warning, TEXT("Already in AR mode"));
        return;
    }
    
    EVARIntegrationMode OldMode = CurrentMode;
    CurrentMode = EVARIntegrationMode::AR_Only;
    
    // Shutdown VR if running
    if (VRSupportSystem && VRSupportSystem->IsVREnabled())
    {
        VRSupportSystem->DisableVR();
    }
    
    // Initialize AR
    if (ARSupportSystem)
    {
        FARSessionConfig Config;
        Config.PlaneDetectionMode = EARPlaneDetectionMode::Horizontal;
        Config.bEnableLightEstimation = true;
        Config.bEnableAutoFocus = true;
        ARSupportSystem->StartARSession(Config);
    }
    
    // Adapt UI for AR
    AdaptUIForCurrentMode();
    
    HandleModeChange(CurrentMode);
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Switched to AR mode"));
}

void UMingRTSVARIntegrationSystem::SwitchToMixedMode()
{
    if (CurrentMode == EVARIntegrationMode::Mixed)
    {
        UE_LOG(LogMingVARRIntegration, Warning, TEXT("Already in mixed mode"));
        return;
    }
    
    EVARIntegrationMode OldMode = CurrentMode;
    CurrentMode = EVARIntegrationMode::Mixed;
    
    // Initialize both VR and AR
    if (VRSupportSystem)
    {
        VRSupportSystem->InitializeVRSupport();
        if (VRSupportSystem->IsVRDeviceConnected())
        {
            VRSupportSystem->EnableVR();
        }
    }
    
    if (ARSupportSystem)
    {
        FARSessionConfig Config;
        Config.PlaneDetectionMode = EARPlaneDetectionMode::Both;
        Config.bEnableLightEstimation = true;
        Config.bEnableAutoFocus = true;
        ARSupportSystem->StartARSession(Config);
    }
    
    // Adapt UI for mixed mode
    AdaptUIForCurrentMode();
    
    HandleModeChange(CurrentMode);
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Switched to mixed VR/AR mode"));
}

bool UMingRTSVARIntegrationSystem::IsVREnabled() const
{
    return VRSupportSystem && VRSupportSystem->IsVREnabled();
}

bool UMingRTSVARIntegrationSystem::IsAREnabled() const
{
    return ARSupportSystem && ARSupportSystem->IsARSessionRunning();
}

void UMingRTSVARIntegrationSystem::SetPerformanceProfile(EVARPerformanceProfile Profile)
{
    EVARPerformanceProfile OldProfile = CurrentPerformanceProfile;
    CurrentPerformanceProfile = Profile;
    
    ApplyPerformanceProfile(Profile);
    
    OnVARRPerformanceProfileChanged.Broadcast(Profile);
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Performance profile changed to: %s"), 
           *UEnum::GetValueAsString(Profile));
}

EVARPerformanceProfile UMingRTSVARIntegrationSystem::GetCurrentPerformanceProfile() const
{
    return CurrentPerformanceProfile;
}

FVARPerformanceMetrics UMingRTSVARIntegrationSystem::GetPerformanceMetrics() const
{
    return CurrentMetrics;
}

void UMingRTSVARIntegrationSystem::EnableAdaptiveQuality(bool bEnable)
{
    bAdaptiveQualityEnabled = bEnable;
    
    if (bEnable)
    {
        EnableAutomaticQualityAdjustment();
    }
    else
    {
        DisableAutomaticQualityAdjustment();
    }
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Adaptive quality %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARIntegrationSystem::AdaptUIForCurrentMode()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Adapting UI for current mode: %s"), 
           *UEnum::GetValueAsString(CurrentMode));
    
    switch (CurrentMode)
    {
        case EVARIntegrationMode::VR_Only:
            if (VRSupportSystem)
            {
                VRSupportSystem->AdaptUIToVR();
            }
            break;
            
        case EVARIntegrationMode::AR_Only:
            // AR UI adaptation would go here
            break;
            
        case EVARIntegrationMode::Mixed:
            // Mixed mode UI adaptation would go here
            break;
            
        default:
            break;
    }
}

void UMingRTSVARIntegrationSystem::SetUIScalingMode(EVARUIScalingMode Mode)
{
    CurrentUIScalingMode = Mode;
    
    // Apply UI scaling based on mode
    switch (Mode)
    {
        case EVARUIScalingMode::Fixed:
            // Apply fixed scaling
            break;
            
        case EVARUIScalingMode::DistanceBased:
            // Apply distance-based scaling
            break;
            
        case EVARUIScalingMode::Adaptive:
            // Apply adaptive scaling
            break;
            
        case EVARUIScalingMode::Auto:
            // Apply automatic scaling
            break;
    }
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("UI scaling mode set to: %s"), 
           *UEnum::GetValueAsString(Mode));
}

void UMingRTSVARIntegrationSystem::SetPrimaryInteractionType(EVARInteractionType Type)
{
    PrimaryInteractionType = Type;
    
    // Configure interaction system
    switch (Type)
    {
        case EVARInteractionType::Gaze:
            // Setup gaze-based interaction
            break;
            
        case EVARInteractionType::Controller:
            // Setup controller interaction
            break;
            
        case EVARInteractionType::HandTracking:
            // Setup hand tracking
            break;
            
        case EVARInteractionType::Voice:
            // Setup voice commands
            break;
            
        case EVARInteractionType::Gesture:
            // Setup gesture recognition
            break;
            
        case EVARInteractionType::Hybrid:
            // Setup hybrid interaction
            break;
    }
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Primary interaction type set to: %s"), 
           *UEnum::GetValueAsString(Type));
}

TArray<EVARInteractionType> UMingRTSVARIntegrationSystem::GetAvailableInteractionTypes() const
{
    TArray<EVARInteractionType> AvailableTypes;
    
    // Check available interaction types based on current hardware
    if (VRSupportSystem && VRSupportSystem->IsVRDeviceConnected())
    {
        AvailableTypes.Add(EVARInteractionType::Controller);
        
        FVRDeviceInfo DeviceInfo = VRSupportSystem->GetVRDeviceInfo();
        if (DeviceInfo.bHasHandTracking)
        {
            AvailableTypes.Add(EVARInteractionType::HandTracking);
        }
    }
    
    if (ARSupportSystem && ARSupportSystem->IsARSupported())
    {
        AvailableTypes.Add(EVARInteractionType::Gaze);
        AvailableTypes.Add(EVARInteractionType::Gesture);
    }
    
    // Voice and hybrid are generally available
    AvailableTypes.Add(EVARInteractionType::Voice);
    AvailableTypes.Add(EVARInteractionType::Hybrid);
    
    return AvailableTypes;
}

void UMingRTSVARIntegrationSystem::EnableCrossPlatformSupport(bool bEnable)
{
    bCrossPlatformEnabled = bEnable;
    
    if (bEnable)
    {
        SyncSettingsAcrossPlatforms();
    }
    
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Cross-platform support %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARIntegrationSystem::SyncSettingsAcrossPlatforms()
{
    // Platform-specific settings synchronization
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Syncing settings across platforms..."));
    
    // This would implement actual cross-platform synchronization
    // For now, we'll just log the action
}

bool UMingRTSVARIntegrationSystem::DetectAndInitializeBestMode()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Detecting and initializing best VR/AR mode..."));
    
    bool bVRDeviceConnected = VRSupportSystem && VRSupportSystem->IsVRDeviceConnected();
    bool bARDeviceSupported = ARSupportSystem && ARSupportSystem->IsARSupported();
    
    if (bVRDeviceConnected && bARDeviceSupported)
    {
        // Both available - use mixed mode or preference
        if (IntegrationConfig.IntegrationMode == EVARIntegrationMode::Auto)
        {
            SwitchToMixedMode();
        }
    }
    else if (bVRDeviceConnected)
    {
        // Only VR available
        SwitchToVRMode();
    }
    else if (bARDeviceSupported)
    {
        // Only AR available
        SwitchToARMode();
    }
    else
    {
        // No VR/AR available
        CurrentMode = EVARIntegrationMode::Disabled;
        UE_LOG(LogMingVARRIntegration, Warning, TEXT("No VR/AR devices detected"));
        return false;
    }
    
    return true;
}

TArray<EVRHeadsetType> UMingRTSVARIntegrationSystem::GetSupportedVRDevices() const
{
    TArray<EVRHeadsetType> SupportedDevices;
    
    if (VRSupportSystem)
    {
        // Check for supported VR devices
        if (VRSupportSystem->IsVRDeviceConnected())
        {
            FVRDeviceInfo DeviceInfo = VRSupportSystem->GetVRDeviceInfo();
            SupportedDevices.Add(DeviceInfo.HeadsetType);
        }
    }
    
    return SupportedDevices;
}

TArray<EARDeviceType> UMingRTSVARIntegrationSystem::GetSupportedARDevices() const
{
    TArray<EARDeviceType> SupportedDevices;
    
    if (ARSupportSystem)
    {
        // Check for supported AR devices
        if (ARSupportSystem->IsARSupported())
        {
            EARDeviceType DeviceType = ARSupportSystem->GetARDeviceType();
            if (DeviceType != EARDeviceType::None)
            {
                SupportedDevices.Add(DeviceType);
            }
        }
    }
    
    return SupportedDevices;
}

void UMingRTSVARIntegrationSystem::OptimizeForDevice()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Optimizing for current device..."));
    
    OptimizeForCurrentDevice();
}

void UMingRTSVARIntegrationSystem::ApplyQualitySettings()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Applying quality settings..."));
    
    ApplyPerformanceProfile(CurrentPerformanceProfile);
}

void UMingRTSVARIntegrationSystem::ResetToDefaults()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Resetting to default settings..."));
    
    SetupDefaultConfiguration();
    ApplyPerformanceProfile(CurrentPerformanceProfile);
    AdaptUIForCurrentMode();
}

void UMingRTSVARIntegrationSystem::InitializeInternalSystems()
{
    // Get or create VR support system
    if (!VRSupportSystem)
    {
        VRSupportSystem = GetGameInstance()->GetSubsystem<UMingRTSVRSupport>();
    }
    
    // Get or create AR support system
    if (!ARSupportSystem)
    {
        ARSupportSystem = GetGameInstance()->GetSubsystem<UMingRTSARSupport>();
    }
    
    // Bind event handlers
    if (VRSupportSystem)
    {
        VRSupportSystem->OnVRDeviceConnected.AddDynamic(this, &UMingRTSVARIntegrationSystem::OnVRDeviceConnected);
        VRSupportSystem->OnVRDeviceDisconnected.AddDynamic(this, &UMingRTSVARIntegrationSystem::OnVRDeviceDisconnected);
    }
    
    if (ARSupportSystem)
    {
        ARSupportSystem->OnARSessionStarted.AddDynamic(this, &UMingRTSVARIntegrationSystem::OnARSessionStarted);
        ARSupportSystem->OnARSessionStopped.AddDynamic(this, &UMingRTSVARIntegrationSystem::OnARSessionStopped);
        ARSupportSystem->OnTrackingQualityChanged.AddDynamic(this, &UMingRTSVARIntegrationSystem::OnTrackingQualityChanged);
    }
}

void UMingRTSVARIntegrationSystem::UpdatePerformanceMetrics()
{
    if (!bPerformanceMonitoringEnabled)
    {
        return;
    }
    
    // Update performance metrics
    // This would typically gather real performance data
    CurrentMetrics.CurrentFrameRate = 1.0f / CurrentMetrics.FrameTime;
    CurrentMetrics.AverageFrameRate = (CurrentMetrics.AverageFrameRate + CurrentMetrics.CurrentFrameRate) / 2.0f;
    
    // Check if performance is optimal
    CurrentMetrics.bIsPerformanceOptimal = CurrentMetrics.CurrentFrameRate >= IntegrationConfig.TargetFrameRate * 0.9f;
    
    // Broadcast performance warning if needed
    if (!CurrentMetrics.bIsPerformanceOptimal && ShouldSwitchToLowPerformanceMode())
    {
        BroadcastPerformanceWarning();
    }
}

void UMingRTSVARIntegrationSystem::ApplyPerformanceProfile(EVARPerformanceProfile Profile)
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Applying performance profile: %s"), 
           *UEnum::GetValueAsString(Profile));
    
    switch (Profile)
    {
        case EVARPerformanceProfile::UltraLow:
            // Ultra low settings for mobile devices
            if (VRSupportSystem)
            {
                VRSupportSystem->SetVROptimizationLevel(3); // Ultra performance
            }
            break;
            
        case EVARPerformanceProfile::Low:
            // Low settings for entry level
            if (VRSupportSystem)
            {
                VRSupportSystem->SetVROptimizationLevel(2); // High performance
            }
            break;
            
        case EVARPerformanceProfile::Medium:
            // Medium settings for standard devices
            if (VRSupportSystem)
            {
                VRSupportSystem->SetVROptimizationLevel(1); // Medium performance
            }
            break;
            
        case EVARPerformanceProfile::High:
            // High settings for premium devices
            if (VRSupportSystem)
            {
                VRSupportSystem->SetVROptimizationLevel(0); // Low performance (high quality)
            }
            break;
            
        case EVARPerformanceProfile::Ultra:
            // Ultra settings for high-end devices
            if (VRSupportSystem)
            {
                VRSupportSystem->SetVROptimizationLevel(0); // Maximum quality
            }
            break;
            
        case EVARPerformanceProfile::Custom:
            // Use custom settings from config
            break;
    }
    
    CurrentMetrics.CurrentProfile = Profile;
}

void UMingRTSVARIntegrationSystem::HandleModeChange(EVARIntegrationMode NewMode)
{
    // This would handle any additional logic needed when mode changes
    UE_LOG(LogMingVARRIntegration, Log, TEXT("VR/AR mode changed to: %s"), 
           *UEnum::GetValueAsString(NewMode));
}

void UMingRTSVARIntegrationSystem::OptimizeForCurrentDevice()
{
    // Device-specific optimizations
    if (VRSupportSystem && VRSupportSystem->IsVRDeviceConnected())
    {
        FVRDeviceInfo DeviceInfo = VRSupportSystem->GetVRDeviceInfo();
        
        // Optimize based on device capabilities
        if (DeviceInfo.RefreshRate < 90.0f)
        {
            // Lower refresh rate - need more aggressive optimization
            SetPerformanceProfile(EVARPerformanceProfile::High);
        }
        else if (DeviceInfo.DisplayResolution.X < 2160)
        {
            // Lower resolution - can use higher quality
            SetPerformanceProfile(EVARPerformanceProfile::Medium);
        }
    }
    
    if (ARSupportSystem && ARSupportSystem->IsARSupported())
    {
        // AR-specific optimizations
        EARDeviceType DeviceType = ARSupportSystem->GetARDeviceType();
        
        switch (DeviceType)
        {
            case EARDeviceType::ARCore:
                // Android AR optimizations
                break;
                
            case EARDeviceType::ARKit:
                // iOS AR optimizations
                break;
                
            default:
                break;
        }
    }
}

void UMingRTSVARIntegrationSystem::SetupDefaultConfiguration()
{
    IntegrationConfig.IntegrationMode = EVARIntegrationMode::Auto;
    IntegrationConfig.PerformanceProfile = EVARPerformanceProfile::Medium;
    IntegrationConfig.PrimaryInteraction = EVARInteractionType::Controller;
    IntegrationConfig.UIScalingMode = EVARUIScalingMode::Adaptive;
    IntegrationConfig.bEnableCrossPlatform = true;
    IntegrationConfig.bEnablePerformanceMonitoring = true;
    IntegrationConfig.bEnableAdaptiveQuality = true;
    IntegrationConfig.TargetFrameRate = 90.0f;
    IntegrationConfig.MaxRenderDistance = 10000;
    IntegrationConfig.bEnableSpatialAudio = true;
}

void UMingRTSVARIntegrationSystem::ValidateConfiguration()
{
    // Validate configuration settings
    if (IntegrationConfig.TargetFrameRate <= 0.0f)
    {
        IntegrationConfig.TargetFrameRate = 90.0f;
    }
    
    if (IntegrationConfig.MaxRenderDistance <= 0)
    {
        IntegrationConfig.MaxRenderDistance = 10000;
    }
}

void UMingRTSVARIntegrationSystem::OnVRDeviceConnected(EVRHeadsetType HeadsetType)
{
    FString DeviceName = UEnum::GetValueAsString(HeadsetType);
    OnVARRDeviceConnected.Broadcast(DeviceName);
    
    // Re-evaluate best mode
    if (IntegrationConfig.IntegrationMode == EVARIntegrationMode::Auto)
    {
        DetectAndInitializeBestMode();
    }
}

void UMingRTSVARIntegrationSystem::OnVRDeviceDisconnected()
{
    FString DeviceName = TEXT("VR Device");
    OnVARRDeviceDisconnected.Broadcast(DeviceName);
    
    // Re-evaluate best mode
    if (IntegrationConfig.IntegrationMode == EVARIntegrationMode::Auto)
    {
        DetectAndInitializeBestMode();
    }
}

void UMingRTSVARIntegrationSystem::OnARSessionStarted()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("AR session started"));
    
    // Re-evaluate best mode
    if (IntegrationConfig.IntegrationMode == EVARIntegrationMode::Auto)
    {
        DetectAndInitializeBestMode();
    }
}

void UMingRTSVARIntegrationSystem::OnARSessionStopped()
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("AR session stopped"));
    
    // Re-evaluate best mode
    if (IntegrationConfig.IntegrationMode == EVARIntegrationMode::Auto)
    {
        DetectAndInitializeBestMode();
    }
}

void UMingRTSVARIntegrationSystem::OnTrackingQualityChanged(EARTrackingQuality Quality)
{
    UE_LOG(LogMingVARRIntegration, Log, TEXT("AR tracking quality changed to: %s"), 
           *UEnum::GetValueAsString(Quality));
    
    // Adjust performance based on tracking quality
    if (Quality == EARTrackingQuality::Limited || Quality == EARTrackingQuality::NotAvailable)
    {
        // Poor tracking - might need to adjust settings
        if (bAdaptiveQualityEnabled)
        {
            SetPerformanceProfile(EVARPerformanceProfile::High);
        }
    }
}

void UMingRTSVARIntegrationSystem::BroadcastPerformanceWarning()
{
    FString WarningMessage = TEXT("Performance below optimal threshold");
    OnVARRPerformanceWarning.Broadcast(WarningMessage);
}

bool UMingRTSVARIntegrationSystem::ShouldSwitchToLowPerformanceMode()
{
    return CurrentMetrics.CurrentFrameRate < IntegrationConfig.TargetFrameRate * 0.7f;
}

void UMingRTSVARIntegrationSystem::EnableAutomaticQualityAdjustment()
{
    // Enable automatic quality adjustment based on performance
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Automatic quality adjustment enabled"));
}

void UMingRTSVARIntegrationSystem::DisableAutomaticQualityAdjustment()
{
    // Disable automatic quality adjustment
    UE_LOG(LogMingVARRIntegration, Log, TEXT("Automatic quality adjustment disabled"));
}
