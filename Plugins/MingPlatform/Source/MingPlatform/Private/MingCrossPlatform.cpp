#include "MingCrossPlatform.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"

UMingCrossPlatform::UMingCrossPlatform()
{
    WorldContext = GetWorld();
    CurrentPlatform = EMingPlatform::Windows;
}

void UMingCrossPlatform::InitializePlatform()
{
    DetectCurrentPlatform();
    InitializePlatformCapabilities();
    LoadPlatformSettings();
    ApplyDefaultSettings();
    
    UE_LOG(LogTemp, Log, TEXT("Cross-platform system initialized for: %s"), *GetPlatformName(CurrentPlatform));
}

void UMingCrossPlatform::DetectCurrentPlatform()
{
    EMingPlatform OldPlatform = CurrentPlatform;
    
#if PLATFORM_WINDOWS
    CurrentPlatform = EMingPlatform::Windows;
#elif PLATFORM_ANDROID
    CurrentPlatform = EMingPlatform::Android;
#elif PLATFORM_IOS
    CurrentPlatform = EMingPlatform::iOS;
#elif PLATFORM_MAC
    CurrentPlatform = EMingPlatform::Windows; // Treat Mac as Windows for simplicity
#elif PLATFORM_LINUX
    CurrentPlatform = EMingPlatform::Windows; // Treat Linux as Windows for simplicity
#else
    CurrentPlatform = EMingPlatform::Web;
#endif
    
    if (OldPlatform != CurrentPlatform)
    {
        OnPlatformChanged.Broadcast(OldPlatform, CurrentPlatform);
    }
}

void UMingCrossPlatform::InitializePlatformCapabilities()
{
    PlatformCapabilities = FMingPlatformCapabilities();
    
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        PlatformCapabilities.bSupportsHighResTextures = true;
        PlatformCapabilities.bSupportsAdvancedLighting = true;
        PlatformCapabilities.bSupportsPostProcessing = true;
        PlatformCapabilities.bSupportsShadows = true;
        PlatformCapabilities.bSupportsParticles = true;
        PlatformCapabilities.MaxTextureSize = 4096;
        PlatformCapabilities.MaxParticles = 10000;
        PlatformCapabilities.MaxDrawDistance = 10000.0f;
        PlatformCapabilities.bSupportsHapticFeedback = false;
        PlatformCapabilities.bSupportsCloudSave = true;
        PlatformCapabilities.bSupportsMultiplayer = true;
        PlatformCapabilities.MaxPlayers = 8;
        break;
        
    case EMingPlatform::Android:
        PlatformCapabilities.bSupportsHighResTextures = true;
        PlatformCapabilities.bSupportsAdvancedLighting = false;
        PlatformCapabilities.bSupportsPostProcessing = false;
        PlatformCapabilities.bSupportsShadows = true;
        PlatformCapabilities.bSupportsParticles = true;
        PlatformCapabilities.MaxTextureSize = 2048;
        PlatformCapabilities.MaxParticles = 5000;
        PlatformCapabilities.MaxDrawDistance = 5000.0f;
        PlatformCapabilities.bSupportsHapticFeedback = true;
        PlatformCapabilities.bSupportsCloudSave = true;
        PlatformCapabilities.bSupportsMultiplayer = true;
        PlatformCapabilities.MaxPlayers = 4;
        break;
        
    case EMingPlatform::iOS:
        PlatformCapabilities.bSupportsHighResTextures = true;
        PlatformCapabilities.bSupportsAdvancedLighting = false;
        PlatformCapabilities.bSupportsPostProcessing = false;
        PlatformCapabilities.bSupportsShadows = true;
        PlatformCapabilities.bSupportsParticles = true;
        PlatformCapabilities.MaxTextureSize = 2048;
        PlatformCapabilities.MaxParticles = 3000;
        PlatformCapabilities.MaxDrawDistance = 4000.0f;
        PlatformCapabilities.bSupportsHapticFeedback = true;
        PlatformCapabilities.bSupportsCloudSave = true;
        PlatformCapabilities.bSupportsMultiplayer = true;
        PlatformCapabilities.MaxPlayers = 4;
        break;
        
    case EMingPlatform::Web:
        PlatformCapabilities.bSupportsHighResTextures = false;
        PlatformCapabilities.bSupportsAdvancedLighting = false;
        PlatformCapabilities.bSupportsPostProcessing = false;
        PlatformCapabilities.bSupportsShadows = false;
        PlatformCapabilities.bSupportsParticles = true;
        PlatformCapabilities.MaxTextureSize = 1024;
        PlatformCapabilities.MaxParticles = 2000;
        PlatformCapabilities.MaxDrawDistance = 3000.0f;
        PlatformCapabilities.bSupportsHapticFeedback = false;
        PlatformCapabilities.bSupportsCloudSave = false;
        PlatformCapabilities.bSupportsMultiplayer = false;
        PlatformCapabilities.MaxPlayers = 2;
        break;
        
    case EMingPlatform::Console:
        PlatformCapabilities.bSupportsHighResTextures = true;
        PlatformCapabilities.bSupportsAdvancedLighting = true;
        PlatformCapabilities.bSupportsPostProcessing = true;
        PlatformCapabilities.bSupportsShadows = true;
        PlatformCapabilities.bSupportsParticles = true;
        PlatformCapabilities.MaxTextureSize = 2048;
        PlatformCapabilities.MaxParticles = 8000;
        PlatformCapabilities.MaxDrawDistance = 8000.0f;
        PlatformCapabilities.bSupportsHapticFeedback = true;
        PlatformCapabilities.bSupportsCloudSave = true;
        PlatformCapabilities.bSupportsMultiplayer = true;
        PlatformCapabilities.MaxPlayers = 8;
        break;
    }
}

void UMingCrossPlatform::ApplyDefaultSettings()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        PlatformSettings.PreferredInput = EMingInputMethod::MouseKeyboard;
        PlatformSettings.GraphicsQuality = EMingGraphicsQuality::High;
        PlatformSettings.ResolutionX = 1920;
        PlatformSettings.ResolutionY = 1080;
        PlatformSettings.bFullscreen = true;
        PlatformSettings.UIScale = 1.0f;
        PlatformSettings.TargetFPS = 60;
        PlatformSettings.bEnableHapticFeedback = false;
        break;
        
    case EMingPlatform::Android:
        PlatformSettings.PreferredInput = EMingInputMethod::Touch;
        PlatformSettings.GraphicsQuality = EMingGraphicsQuality::Medium;
        PlatformSettings.ResolutionX = 1280;
        PlatformSettings.ResolutionY = 720;
        PlatformSettings.bFullscreen = true;
        PlatformSettings.UIScale = 1.5f;
        PlatformSettings.TargetFPS = 30;
        PlatformSettings.bEnableHapticFeedback = true;
        break;
        
    case EMingPlatform::iOS:
        PlatformSettings.PreferredInput = EMingInputMethod::Touch;
        PlatformSettings.GraphicsQuality = EMingGraphicsQuality::Medium;
        PlatformSettings.ResolutionX = 1136;
        PlatformSettings.ResolutionY = 640;
        PlatformSettings.bFullscreen = true;
        PlatformSettings.UIScale = 1.2f;
        PlatformSettings.TargetFPS = 30;
        PlatformSettings.bEnableHapticFeedback = true;
        break;
        
    case EMingPlatform::Web:
        PlatformSettings.PreferredInput = EMingInputMethod::MouseKeyboard;
        PlatformSettings.GraphicsQuality = EMingGraphicsQuality::Low;
        PlatformSettings.ResolutionX = 1024;
        PlatformSettings.ResolutionY = 768;
        PlatformSettings.bFullscreen = false;
        PlatformSettings.UIScale = 1.0f;
        PlatformSettings.TargetFPS = 30;
        PlatformSettings.bEnableHapticFeedback = false;
        break;
        
    case EMingPlatform::Console:
        PlatformSettings.PreferredInput = EMingInputMethod::Gamepad;
        PlatformSettings.GraphicsQuality = EMingGraphicsQuality::High;
        PlatformSettings.ResolutionX = 1920;
        PlatformSettings.ResolutionY = 1080;
        PlatformSettings.bFullscreen = true;
        PlatformSettings.UIScale = 1.0f;
        PlatformSettings.TargetFPS = 60;
        PlatformSettings.bEnableHapticFeedback = true;
        break;
    }
    
    ApplyPlatformSettings(PlatformSettings);
}

EMingPlatform UMingCrossPlatform::GetCurrentPlatform() const
{
    return CurrentPlatform;
}

FString UMingCrossPlatform::GetPlatformName() const
{
    return GetPlatformName(CurrentPlatform);
}

bool UMingCrossPlatform::IsMobilePlatform() const
{
    return CurrentPlatform == EMingPlatform::Android || CurrentPlatform == EMingPlatform::iOS;
}

bool UMingCrossPlatform::IsConsolePlatform() const
{
    return CurrentPlatform == EMingPlatform::Console;
}

bool UMingCrossPlatform::IsDesktopPlatform() const
{
    return CurrentPlatform == EMingPlatform::Windows;
}

void UMingCrossPlatform::LoadPlatformSettings()
{
    // This would load settings from platform-specific storage
    UE_LOG(LogTemp, Log, TEXT("Loading platform settings for %s"), *GetPlatformName(CurrentPlatform));
}

void UMingCrossPlatform::SavePlatformSettings()
{
    // This would save settings to platform-specific storage
    UE_LOG(LogTemp, Log, TEXT("Saving platform settings for %s"), *GetPlatformName(CurrentPlatform));
}

void UMingCrossPlatform::ApplyPlatformSettings(const FMingPlatformSettings& Settings)
{
    PlatformSettings = Settings;
    
    ApplyGraphicsSettings();
    ApplyAudioSettings();
    ApplyInputSettings();
    ApplyUISettings();
    
    OnSettingsChanged.Broadcast(Settings, TEXT("All"));
    
    UE_LOG(LogTemp, Log, TEXT("Applied platform settings"));
}

FMingPlatformSettings UMingCrossPlatform::GetCurrentSettings() const
{
    return PlatformSettings;
}

void UMingCrossPlatform::SetGraphicsQuality(EMingGraphicsQuality Quality)
{
    PlatformSettings.GraphicsQuality = Quality;
    ApplyGraphicsSettings();
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("GraphicsQuality"));
}

void UMingCrossPlatform::SetResolution(int32 Width, int32 Height)
{
    PlatformSettings.ResolutionX = Width;
    PlatformSettings.ResolutionY = Height;
    ApplyGraphicsSettings();
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("Resolution"));
}

void UMingCrossPlatform::SetFullscreen(bool bFullscreen)
{
    PlatformSettings.bFullscreen = bFullscreen;
    ApplyGraphicsSettings();
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("Fullscreen"));
}

void UMingCrossPlatform::SetUIScale(float Scale)
{
    PlatformSettings.UIScale = FMath::Clamp(Scale, 0.5f, 2.0f);
    ApplyUISettings();
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("UIScale"));
}

void UMingCrossPlatform::AdaptInputScheme()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        SetInputMethod(EMingInputMethod::MouseKeyboard);
        break;
    case EMingPlatform::Android:
    case EMingPlatform::iOS:
        SetInputMethod(EMingInputMethod::Touch);
        break;
    case EMingPlatform::Console:
        SetInputMethod(EMingInputMethod::Gamepad);
        break;
    case EMingPlatform::Web:
        SetInputMethod(EMingInputMethod::MouseKeyboard);
        break;
    }
}

void UMingCrossPlatform::SetInputMethod(EMingInputMethod InputMethod)
{
    PlatformSettings.PreferredInput = InputMethod;
    ApplyInputSettings();
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("InputMethod"));
}

EMingInputMethod UMingCrossPlatform::GetPreferredInputMethod() const
{
    return PlatformSettings.PreferredInput;
}

void UMingCrossPlatform::EnableTouchControls(bool bEnable)
{
    if (IsMobilePlatform())
    {
        // Enable/disable touch controls based on platform
        UE_LOG(LogTemp, Log, TEXT("Touch controls %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::EnableGamepadSupport(bool bEnable)
{
    if (IsConsolePlatform() || PlatformSettings.PreferredInput == EMingInputMethod::Gamepad)
    {
        // Enable/disable gamepad support
        UE_LOG(LogTemp, Log, TEXT("Gamepad support %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::CalibrateTouchControls()
{
    if (IsMobilePlatform())
    {
        // Calibrate touch controls for mobile devices
        UE_LOG(LogTemp, Log, TEXT("Calibrating touch controls"));
    }
}

void UMingCrossPlatform::AdaptUIForPlatform()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        SetDesktopUILayout();
        break;
    case EMingPlatform::Android:
    case EMingPlatform::iOS:
        SetMobileUILayout();
        break;
    case EMingPlatform::Console:
        SetDesktopUILayout(); // Console uses desktop layout with gamepad navigation
        break;
    case EMingPlatform::Web:
        SetDesktopUILayout();
        break;
    }
}

void UMingCrossPlatform::SetMobileUILayout()
{
    PlatformSettings.UIScale = IsMobilePlatform() ? 1.5f : 1.0f;
    ApplyUISettings();
    
    // Optimize touch targets for mobile
    OptimizeTouchTargets();
    
    UE_LOG(LogTemp, Log, TEXT("Applied mobile UI layout"));
}

void UMingCrossPlatform::SetDesktopUILayout()
{
    PlatformSettings.UIScale = 1.0f;
    ApplyUISettings();
    
    UE_LOG(LogTemp, Log, TEXT("Applied desktop UI layout"));
}

void UMingCrossPlatform::AdjustUIElements()
{
    // Adjust UI elements based on platform and screen size
    if (IsMobilePlatform())
    {
        // Increase button sizes, adjust spacing, etc.
        UE_LOG(LogTemp, Log, TEXT("Adjusting UI elements for mobile"));
    }
}

void UMingCrossPlatform::OptimizeTouchTargets()
{
    if (IsMobilePlatform())
    {
        // Ensure touch targets are at least 44x44 pixels
        UE_LOG(LogTemp, Log, TEXT("Optimizing touch targets"));
    }
}

void UMingCrossPlatform::OptimizeForPlatform()
{
    AdaptGraphicsForPlatform();
    AdaptAudioForPlatform();
    AdaptInputScheme();
    AdaptUIForPlatform();
    AdaptStorageForPlatform();
    AdaptNetworkForPlatform();
    
    UE_LOG(LogTemp, Log, TEXT("Optimized for platform: %s"), *GetPlatformName(CurrentPlatform));
}

void UMingCrossPlatform::EnableAdaptivePerformance(bool bEnable)
{
    PlatformSettings.bAdaptivePerformance = bEnable;
    
    if (bEnable)
    {
        StartPerformanceMonitoring();
    }
    else
    {
        StopPerformanceMonitoring();
    }
    
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("AdaptivePerformance"));
}

void UMingCrossPlatform::SetTargetFPS(int32 FPS)
{
    PlatformSettings.TargetFPS = FMath::Clamp(FPS, 15, 120);
    
    // Apply FPS limit
    if (WorldContext.IsValid())
    {
        // This would set the FPS limit in the game instance
        UE_LOG(LogTemp, Log, TEXT("Target FPS set to %d"), PlatformSettings.TargetFPS);
    }
    
    OnSettingsChanged.Broadcast(PlatformSettings, TEXT("TargetFPS"));
}

void UMingCrossPlatform::AdjustQualitySettings()
{
    if (PlatformSettings.bAdaptivePerformance)
    {
        AdjustQualityBasedOnPerformance();
    }
}

void UMingCrossPlatform::MonitorPerformance()
{
    if (PlatformSettings.bAdaptivePerformance)
    {
        UpdatePerformanceMetrics();
    }
}

void UMingCrossPlatform::AdaptGraphicsForPlatform()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        SetGraphicsQuality(EMingGraphicsQuality::High);
        break;
    case EMingPlatform::Android:
        SetGraphicsQuality(EMingGraphicsQuality::Medium);
        break;
    case EMingPlatform::iOS:
        SetGraphicsQuality(EMingGraphicsQuality::Medium);
        break;
    case EMingPlatform::Web:
        SetGraphicsQuality(EMingGraphicsQuality::Low);
        break;
    case EMingPlatform::Console:
        SetGraphicsQuality(EMingGraphicsQuality::High);
        break;
    }
    
    ApplyGraphicsSettings();
}

void UMingCrossPlatform::SetTextureQuality(int32 Quality)
{
    // Apply texture quality based on platform capabilities
    int32 MaxQuality = PlatformCapabilities.MaxTextureSize;
    int32 AppliedQuality = FMath::Min(Quality, MaxQuality);
    
    UE_LOG(LogTemp, Log, TEXT("Texture quality set to %d"), AppliedQuality);
}

void UMingCrossPlatform::SetShadowQuality(int32 Quality)
{
    if (PlatformCapabilities.bSupportsShadows)
    {
        // Apply shadow quality
        UE_LOG(LogTemp, Log, TEXT("Shadow quality set to %d"), Quality);
    }
}

void UMingCrossPlatform::SetParticleQuality(int32 Quality)
{
    if (PlatformCapabilities.bSupportsParticles)
    {
        // Apply particle quality
        UE_LOG(LogTemp, Log, TEXT("Particle quality set to %d"), Quality);
    }
}

void UMingCrossPlatform::EnablePostProcessing(bool bEnable)
{
    if (PlatformCapabilities.bSupportsPostProcessing)
    {
        // Enable/disable post-processing
        UE_LOG(LogTemp, Log, TEXT("Post-processing %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::AdaptAudioForPlatform()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        SetAudioQuality(3); // High quality
        break;
    case EMingPlatform::Android:
    case EMingPlatform::iOS:
        SetAudioQuality(2); // Medium quality
        break;
    case EMingPlatform::Web:
        SetAudioQuality(1); // Low quality
        break;
    case EMingPlatform::Console:
        SetAudioQuality(3); // High quality
        break;
    }
    
    ApplyAudioSettings();
}

void UMingCrossPlatform::SetAudioQuality(int32 Quality)
{
    // Apply audio quality based on platform
    UE_LOG(LogTemp, Log, TEXT("Audio quality set to %d"), Quality);
}

void UMingCrossPlatform::EnableHapticFeedback(bool bEnable)
{
    if (PlatformCapabilities.bSupportsHapticFeedback)
    {
        PlatformSettings.bEnableHapticFeedback = bEnable;
        UE_LOG(LogTemp, Log, TEXT("Haptic feedback %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::OptimizeAudioLatency()
{
    // Optimize audio latency for mobile platforms
    if (IsMobilePlatform())
    {
        UE_LOG(LogTemp, Log, TEXT("Optimizing audio latency for mobile"));
    }
}

void UMingCrossPlatform::AdaptStorageForPlatform()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        EnableCloudSave(true);
        SetAutoSaveInterval(300.0f); // 5 minutes
        break;
    case EMingPlatform::Android:
    case EMingPlatform::iOS:
        EnableCloudSave(true);
        SetAutoSaveInterval(180.0f); // 3 minutes
        break;
    case EMingPlatform::Web:
        EnableCloudSave(false);
        SetAutoSaveInterval(600.0f); // 10 minutes
        break;
    case EMingPlatform::Console:
        EnableCloudSave(true);
        SetAutoSaveInterval(240.0f); // 4 minutes
        break;
    }
}

void UMingCrossPlatform::EnableCloudSave(bool bEnable)
{
    if (PlatformCapabilities.bSupportsCloudSave)
    {
        // Enable/disable cloud save
        UE_LOG(LogTemp, Log, TEXT("Cloud save %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::SetAutoSaveInterval(float Interval)
{
    // Set auto-save interval
    UE_LOG(LogTemp, Log, TEXT("Auto-save interval set to %.1f seconds"), Interval);
}

void UMingCrossPlatform::CompressSaveData(bool bCompress)
{
    // Enable/disable save data compression
    UE_LOG(LogTemp, Log, TEXT("Save data compression %s"), bCompress ? TEXT("enabled") : TEXT("disabled"));
}

void UMingCrossPlatform::AdaptNetworkForPlatform()
{
    switch (CurrentPlatform)
    {
    case EMingPlatform::Windows:
        SetNetworkQuality(3); // High quality
        break;
    case EMingPlatform::Android:
    case EMingPlatform::iOS:
        SetNetworkQuality(2); // Medium quality
        EnableMobileOptimizedNetworking(true);
        break;
    case EMingPlatform::Web:
        SetNetworkQuality(1); // Low quality
        break;
    case EMingPlatform::Console:
        SetNetworkQuality(3); // High quality
        break;
    }
}

void UMingCrossPlatform::SetNetworkQuality(int32 Quality)
{
    // Set network quality based on platform
    UE_LOG(LogTemp, Log, TEXT("Network quality set to %d"), Quality);
}

void UMingCrossPlatform::EnableMobileOptimizedNetworking(bool bEnable)
{
    if (IsMobilePlatform())
    {
        // Enable mobile-optimized networking
        UE_LOG(LogTemp, Log, TEXT("Mobile-optimized networking %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
    }
}

void UMingCrossPlatform::AdjustBandwidthUsage()
{
    // Adjust bandwidth usage based on platform
    if (IsMobilePlatform())
    {
        // Reduce bandwidth usage for mobile
        UE_LOG(LogTemp, Log, TEXT("Adjusting bandwidth usage for mobile"));
    }
}

FMingPlatformCapabilities UMingCrossPlatform::GetPlatformCapabilities() const
{
    return PlatformCapabilities;
}

bool UMingCrossPlatform::SupportsFeature(const FString& Feature) const
{
    return IsFeatureSupported(Feature);
}

int32 UMingCrossPlatform::GetMaxTextureSize() const
{
    return PlatformCapabilities.MaxTextureSize;
}

int32 UMingCrossPlatform::GetMaxPlayers() const
{
    return PlatformCapabilities.MaxPlayers;
}

bool UMingCrossPlatform::SupportsHapticFeedback() const
{
    return PlatformCapabilities.bSupportsHapticFeedback;
}

FString UMingCrossPlatform::GetPlatformName(EMingPlatform Platform)
{
    switch (Platform)
    {
    case EMingPlatform::Windows: return TEXT("Windows");
    case EMingPlatform::Android: return TEXT("Android");
    case EMingPlatform::iOS: return TEXT("iOS");
    case EMingPlatform::Web: return TEXT("Web");
    case EMingPlatform::Console: return TEXT("Console");
    default: return TEXT("Unknown");
    }
}

FString UMingCrossPlatform::GetInputMethodName(EMingInputMethod InputMethod)
{
    switch (InputMethod)
    {
    case EMingInputMethod::MouseKeyboard: return TEXT("鼠標鍵盤");
    case EMingInputMethod::Touch: return TEXT("觸控");
    case EMingInputMethod::Gamepad: return TEXT("遊戲手把");
    case EMingInputMethod::Mixed: return TEXT("混合");
    default: return TEXT("未知");
    }
}

FString UMingCrossPlatform::GetGraphicsQualityName(EMingGraphicsQuality Quality)
{
    switch (Quality)
    {
    case EMingGraphicsQuality::Low: return TEXT("低");
    case EMingGraphicsQuality::Medium: return TEXT("中");
    case EMingGraphicsQuality::High: return TEXT("高");
    case EMingGraphicsQuality::Ultra: return TEXT("超高");
    case EMingGraphicsQuality::Custom: return TEXT("自定義");
    default: return TEXT("未知");
    }
}

FString UMingCrossPlatform::SavePlatformData() const
{
    FString Result = TEXT("{\n");
    Result += FString::Printf(TEXT("  \"platform\": \"%s\",\n"), *GetPlatformName(CurrentPlatform));
    Result += FString::Printf(TEXT("  \"input_method\": \"%s\",\n"), *GetInputMethodName(PlatformSettings.PreferredInput));
    Result += FString::Printf(TEXT("  \"graphics_quality\": \"%s\",\n"), *GetGraphicsQualityName(PlatformSettings.GraphicsQuality));
    Result += FString::Printf(TEXT("  \"resolution\": [%d, %d],\n"), PlatformSettings.ResolutionX, PlatformSettings.ResolutionY);
    Result += FString::Printf(TEXT("  \"fullscreen\": %s,\n"), PlatformSettings.bFullscreen ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"ui_scale\": %.2f,\n"), PlatformSettings.UIScale);
    Result += FString::Printf(TEXT("  \"target_fps\": %d,\n"), PlatformSettings.TargetFPS);
    Result += FString::Printf(TEXT("  \"adaptive_performance\": %s,\n"), PlatformSettings.bAdaptivePerformance ? TEXT("true") : TEXT("false"));
    Result += FString::Printf(TEXT("  \"haptic_feedback\": %s\n"), PlatformSettings.bEnableHapticFeedback ? TEXT("true") : TEXT("false"));
    Result += TEXT("}\n");
    
    return Result;
}

void UMingCrossPlatform::LoadPlatformData(const FString& JsonString)
{
    // Parse JSON and restore platform data
    UE_LOG(LogTemp, Log, TEXT("Loading platform data"));
}

void UMingCrossPlatform::ApplyGraphicsSettings()
{
    // Apply graphics settings based on platform
    switch (PlatformSettings.GraphicsQuality)
    {
    case EMingGraphicsQuality::Low:
        SetTextureQuality(1024);
        SetShadowQuality(1);
        SetParticleQuality(1);
        EnablePostProcessing(false);
        break;
    case EMingGraphicsQuality::Medium:
        SetTextureQuality(2048);
        SetShadowQuality(2);
        SetParticleQuality(2);
        EnablePostProcessing(false);
        break;
    case EMingGraphicsQuality::High:
        SetTextureQuality(4096);
        SetShadowQuality(3);
        SetParticleQuality(3);
        EnablePostProcessing(true);
        break;
    case EMingGraphicsQuality::Ultra:
        SetTextureQuality(8192);
        SetShadowQuality(4);
        SetParticleQuality(4);
        EnablePostProcessing(true);
        break;
    case EMingGraphicsQuality::Custom:
        // Use custom settings
        break;
    }
    
    SetTargetFPS(PlatformSettings.TargetFPS);
}

void UMingCrossPlatform::ApplyAudioSettings()
{
    // Apply audio settings
    SetAudioQuality(PlatformSettings.GraphicsQuality == EMingGraphicsQuality::Low ? 1 : 2);
    EnableHapticFeedback(PlatformSettings.bEnableHapticFeedback);
}

void UMingCrossPlatform::ApplyInputSettings()
{
    // Apply input settings
    EnableTouchControls(PlatformSettings.PreferredInput == EMingInputMethod::Touch);
    EnableGamepadSupport(PlatformSettings.PreferredInput == EMingInputMethod::Gamepad);
}

void UMingCrossPlatform::ApplyUISettings()
{
    // Apply UI settings
    AdjustUIElements();
}

void UMingCrossPlatform::StartPerformanceMonitoring()
{
    // Start performance monitoring
    UE_LOG(LogTemp, Log, TEXT("Starting performance monitoring"));
}

void UMingCrossPlatform::StopPerformanceMonitoring()
{
    // Stop performance monitoring
    UE_LOG(LogTemp, Log, TEXT("Stopping performance monitoring"));
}

void UMingCrossPlatform::UpdatePerformanceMetrics()
{
    // Update performance metrics
    if (PlatformSettings.bAdaptivePerformance)
    {
        // This would monitor FPS, memory usage, etc.
        AdjustQualityBasedOnPerformance();
    }
}

bool UMingCrossPlatform::IsFeatureSupported(const FString& Feature) const
{
    if (Feature == TEXT("HighResTextures"))
    {
        return PlatformCapabilities.bSupportsHighResTextures;
    }
    else if (Feature == TEXT("AdvancedLighting"))
    {
        return PlatformCapabilities.bSupportsAdvancedLighting;
    }
    else if (Feature == TEXT("PostProcessing"))
    {
        return PlatformCapabilities.bSupportsPostProcessing;
    }
    else if (Feature == TEXT("Shadows"))
    {
        return PlatformCapabilities.bSupportsShadows;
    }
    else if (Feature == TEXT("Particles"))
    {
        return PlatformCapabilities.bSupportsParticles;
    }
    else if (Feature == TEXT("HapticFeedback"))
    {
        return PlatformCapabilities.bSupportsHapticFeedback;
    }
    else if (Feature == TEXT("CloudSave"))
    {
        return PlatformCapabilities.bSupportsCloudSave;
    }
    else if (Feature == TEXT("Multiplayer"))
    {
        return PlatformCapabilities.bSupportsMultiplayer;
    }
    
    return false;
}

void UMingCrossPlatform::OptimizeForHardware()
{
    // Optimize settings based on hardware capabilities
    UE_LOG(LogTemp, Log, TEXT("Optimizing for hardware"));
}

void UMingCrossPlatform::AdjustQualityBasedOnPerformance()
{
    // Adjust quality settings based on current performance
    UE_LOG(LogTemp, Log, TEXT("Adjusting quality based on performance"));
}
