#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingCrossPlatform.generated.h"

UENUM(BlueprintType)
enum class EMingPlatform : uint8
{
    Windows UMETA(DisplayName = "Windows"),
    Android UMETA(DisplayName = "Android"),
    iOS UMETA(DisplayName = "iOS"),
    Web UMETA(DisplayName = "Web"),
    Console UMETA(DisplayName = "Console")
};

UENUM(BlueprintType)
enum class EMingInputMethod : uint8
{
    MouseKeyboard UMETA(DisplayName = "Mouse & Keyboard"),
    Touch UMETA(DisplayName = "Touch"),
    Gamepad UMETA(DisplayName = "Gamepad"),
    Mixed UMETA(DisplayName = "Mixed")
};

UENUM(BlueprintType)
enum class EMingGraphicsQuality : uint8
{
    Low UMETA(DisplayName = "Low"),
    Medium UMETA(DisplayName = "Medium"),
    High UMETA(DisplayName = "High"),
    Ultra UMETA(DisplayName = "Ultra"),
    Custom UMETA(DisplayName = "Custom")
};

USTRUCT(BlueprintType)
struct FMingPlatformSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingPlatform Platform;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingInputMethod PreferredInput;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EMingGraphicsQuality GraphicsQuality;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResolutionX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ResolutionY;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bFullscreen;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float UIScale;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableVSync;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 TargetFPS;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MasterVolume;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableHapticFeedback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAdaptivePerformance;

    FMingPlatformSettings()
        : Platform(EMingPlatform::Windows)
        , PreferredInput(EMingInputMethod::MouseKeyboard)
        , GraphicsQuality(EMingGraphicsQuality::High)
        , ResolutionX(1920)
        , ResolutionY(1080)
        , bFullscreen(true)
        , UIScale(1.0f)
        , bEnableVSync(true)
        , TargetFPS(60)
        , MasterVolume(1.0f)
        , bEnableHapticFeedback(true)
        , bAdaptivePerformance(true)
    {}
};

USTRUCT(BlueprintType)
struct FMingPlatformCapabilities
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsHighResTextures;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsAdvancedLighting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsPostProcessing;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsShadows;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxTextureSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxDrawDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsHapticFeedback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsCloudSave;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSupportsMultiplayer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers;

    FMingPlatformCapabilities()
        : bSupportsHighResTextures(true)
        , bSupportsAdvancedLighting(true)
        , bSupportsPostProcessing(true)
        , bSupportsShadows(true)
        , bSupportsParticles(true)
        , MaxTextureSize(4096)
        , MaxParticles(10000)
        , MaxDrawDistance(10000.0f)
        , bSupportsHapticFeedback(false)
        , bSupportsCloudSave(true)
        , bSupportsMultiplayer(true)
        , MaxPlayers(8)
    {}
};

/**
 * Cross-Platform System for MingGoRTS
 * Manages platform-specific adaptations and optimizations
 */
UCLASS(ClassGroup = (Platform), Blueprintable)
class MINGPLATFORM_API UMingCrossPlatform : public UObject
{
    GENERATED_BODY()

public:
    UMingCrossPlatform(};

    // Platform Detection
    UFUNCTION(BlueprintCallable, Category = "Platform")
    void InitializePlatform(};

    UFUNCTION(BlueprintPure, Category = "Platform")
    EMingPlatform GetCurrentPlatform() const;

    UFUNCTION(BlueprintPure, Category = "Platform")
    FString GetPlatformName() const;

    UFUNCTION(BlueprintPure, Category = "Platform")
    bool IsMobilePlatform() const;

    UFUNCTION(BlueprintPure, Category = "Platform")
    bool IsConsolePlatform() const;

    UFUNCTION(BlueprintPure, Category = "Platform")
    bool IsDesktopPlatform() const;

    // Settings Management
    UFUNCTION(BlueprintCallable, Category = "Settings")
    void LoadPlatformSettings(};

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SavePlatformSettings(};

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void ApplyPlatformSettings(const FMingPlatformSettings& Settings};

    UFUNCTION(BlueprintPure, Category = "Settings")
    FMingPlatformSettings GetCurrentSettings() const;

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetGraphicsQuality(EMingGraphicsQuality Quality};

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetResolution(int32 Width, int32 Height};

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetFullscreen(bool bFullscreen};

    UFUNCTION(BlueprintCallable, Category = "Settings")
    void SetUIScale(float Scale};

    // Input Adaptation
    UFUNCTION(BlueprintCallable, Category = "Input")
    void AdaptInputScheme(};

    UFUNCTION(BlueprintCallable, Category = "Input")
    void SetInputMethod(EMingInputMethod InputMethod};

    UFUNCTION(BlueprintPure, Category = "Input")
    EMingInputMethod GetPreferredInputMethod() const;

    UFUNCTION(BlueprintCallable, Category = "Input")
    void EnableTouchControls(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Input")
    void EnableGamepadSupport(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Input")
    void CalibrateTouchControls(};

    // UI Adaptation
    UFUNCTION(BlueprintCallable, Category = "UI")
    void AdaptUIForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetMobileUILayout(};

    UFUNCTION(BlueprintCallable, Category = "UI")
    void SetDesktopUILayout(};

    UFUNCTION(BlueprintCallable, Category = "UI")
    void AdjustUIElements(};

    UFUNCTION(BlueprintCallable, Category = "UI")
    void OptimizeTouchTargets(};

    // Performance Optimization
    UFUNCTION(BlueprintCallable, Category = "Performance")
    void OptimizeForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void EnableAdaptivePerformance(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void SetTargetFPS(int32 FPS};

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void AdjustQualitySettings(};

    UFUNCTION(BlueprintCallable, Category = "Performance")
    void MonitorPerformance(};

    // Graphics Adaptation
    UFUNCTION(BlueprintCallable, Category = "Graphics")
    void AdaptGraphicsForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "Graphics")
    void SetTextureQuality(int32 Quality};

    UFUNCTION(BlueprintCallable, Category = "Graphics")
    void SetShadowQuality(int32 Quality};

    UFUNCTION(BlueprintCallable, Category = "Graphics")
    void SetParticleQuality(int32 Quality};

    UFUNCTION(BlueprintCallable, Category = "Graphics")
    void EnablePostProcessing(bool bEnable};

    // Audio Adaptation
    UFUNCTION(BlueprintCallable, Category = "Audio")
    void AdaptAudioForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void SetAudioQuality(int32 Quality};

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void EnableHapticFeedback(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Audio")
    void OptimizeAudioLatency(};

    // Storage Adaptation
    UFUNCTION(BlueprintCallable, Category = "Storage")
    void AdaptStorageForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "Storage")
    void EnableCloudSave(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Storage")
    void SetAutoSaveInterval(float Interval};

    UFUNCTION(BlueprintCallable, Category = "Storage")
    void CompressSaveData(bool bCompress};

    // Network Adaptation
    UFUNCTION(BlueprintCallable, Category = "Network")
    void AdaptNetworkForPlatform(};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void SetNetworkQuality(int32 Quality};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void EnableMobileOptimizedNetworking(bool bEnable};

    UFUNCTION(BlueprintCallable, Category = "Network")
    void AdjustBandwidthUsage(};

    // Platform Capabilities
    UFUNCTION(BlueprintPure, Category = "Capabilities")
    FMingPlatformCapabilities GetPlatformCapabilities() const;

    UFUNCTION(BlueprintPure, Category = "Capabilities")
    bool SupportsFeature(const FString& Feature) const;

    UFUNCTION(BlueprintPure, Category = "Capabilities")
    int32 GetMaxTextureSize() const;

    UFUNCTION(BlueprintPure, Category = "Capabilities")
    int32 GetMaxPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Capabilities")
    bool SupportsHapticFeedback() const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlatformChanged, EMingPlatform, OldPlatform, EMingPlatform, NewPlatform};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSettingsChanged, const FMingPlatformSettings&, Settings, const FString&, SettingName};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPerformanceModeChanged, const FString&, Mode};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlatformChanged OnPlatformChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSettingsChanged OnSettingsChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPerformanceModeChanged OnPerformanceModeChanged;

    // Utility
    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetPlatformName(EMingPlatform Platform};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetInputMethodName(EMingInputMethod InputMethod};

    UFUNCTION(BlueprintPure, Category = "Utility")
    static FString GetGraphicsQualityName(EMingGraphicsQuality Quality};

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    FString SavePlatformData() const;

    UFUNCTION(BlueprintCallable, Category = "Persistence")
    void LoadPlatformData(const FString& JsonString};

protected:
    UPROPERTY()
    EMingPlatform CurrentPlatform;

    UPROPERTY()
    FMingPlatformSettings PlatformSettings;

    UPROPERTY()
    FMingPlatformCapabilities PlatformCapabilities;

    UPROPERTY()
    TWeakObjectPtr<UWorld> WorldContext;

    // Internal Functions
    void DetectCurrentPlatform(};
    void InitializePlatformCapabilities(};
    void ApplyDefaultSettings(};

    // Platform-specific adaptations
    void AdaptForWindows(};
    void AdaptForAndroid(};
    void AdaptForiOS(};
    void AdaptForWeb(};
    void AdaptForConsole(};

    // Performance monitoring
    void StartPerformanceMonitoring(};
    void StopPerformanceMonitoring(};
    void UpdatePerformanceMetrics(};

    // Settings helpers
    void ApplyGraphicsSettings(};
    void ApplyAudioSettings(};
    void ApplyInputSettings(};
    void ApplyUISettings(};

    // Helpers
    bool IsFeatureSupported(const FString& Feature) const;
    void OptimizeForHardware(};
    void AdjustQualityBasedOnPerformance(};
};

