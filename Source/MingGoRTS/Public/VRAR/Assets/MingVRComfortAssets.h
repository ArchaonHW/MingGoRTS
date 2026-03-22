// VR Teleport Marker and Comfort Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - VR Comfort Assets

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingVRComfortAssets.generated.h"

/**
 * Teleport Marker Types
 */
UENUM(BlueprintType)
enum class ETeleportMarkerType : uint8
{
    Default         UMETA(DisplayName = "Default Marker"),
    Valid           UMETA(DisplayName = "Valid Destination"),
    Invalid         UMETA(DisplayName = "Invalid Destination"),
    Selected        UMETA(DisplayName = "Selected Destination"),
    Cooldown        UMETA(DisplayName = "Cooldown Marker"),
    Portal          UMETA(DisplayName = "Portal Marker"),
    Waypoint        UMETA(DisplayName = "Waypoint Marker")
};

/**
 * Comfort Setting Visual Types
 */
UENUM(BlueprintType)
enum class EComfortVisualType : uint8
{
    Vignette        UMETA(DisplayName = "Vignette"),
    Grid            UMETA(DisplayName = "Reference Grid"),
    Horizon         UMETA(DisplayName = "Horizon Line"),
    Cockpit         UMETA(DisplayName = "Cockpit Frame"),
    Tunnel          UMETA(DisplayName = "Tunnel Vision"),
    Fade            UMETA(DisplayName = "Fade to Black")
};

/**
 * VR Teleport Marker Configuration
 */
USTRUCT(BlueprintType)
struct FVRTeleportMarkerConfig
{
    GENERATED_BODY()

    // Marker Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Marker Type")
    ETeleportMarkerType MarkerType = ETeleportMarkerType::Default;

    // Mesh Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> BaseMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> RingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ArrowMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> GlowMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> PortalMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> BaseMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ValidMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InvalidMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> SelectedMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> GlowMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor BaseColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SelectedColor = FLinearColor(1.0f, 0.8f, 0.0f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor GlowColor = FLinearColor(0.0f, 0.9f, 1.0f, 0.5f);

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float BaseSize = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float RingSize = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float ArrowHeight = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float GlowRadius = 50.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableRotation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float RotationSpeed = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnablePulse = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseScaleMin = 0.9f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseScaleMax = 1.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableBobbing = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float BobbingAmplitude = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float BobbingSpeed = 1.5f;

    // Particle Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> IdleEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> ValidEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> InvalidEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> TeleportStartEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> TeleportEndEffect;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> HoverSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> SelectSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> TeleportStartSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> TeleportEndSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> InvalidSound;

    // Cooldown
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
    TSoftObjectPtr<UMaterialInterface> CooldownMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
    FLinearColor CooldownColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
    float CooldownSweepSpeed = 1.0f;
};

/**
 * VR Comfort Vignette Configuration
 */
USTRUCT(BlueprintType)
struct FVRComfortVignetteConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    TSoftObjectPtr<UMaterialInterface> VignetteMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    FLinearColor VignetteColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float VignetteRadius = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float VignetteSoftness = 0.4f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float Intensity = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeInDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeOutDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bAnimateIntensity = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float IntensityPulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float IntensityPulseMin = 0.6f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float IntensityPulseMax = 1.0f;
};

/**
 * VR Reference Grid Configuration
 */
USTRUCT(BlueprintType)
struct FVRReferenceGridConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    TSoftObjectPtr<UMaterialInterface> GridMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FLinearColor GridColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.3f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FLinearColor MajorLineColor = FLinearColor(0.0f, 0.9f, 1.0f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FLinearColor MinorLineColor = FLinearColor(0.0f, 0.6f, 0.8f, 0.2f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float GridSize = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float GridSpacing = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float LineWidth = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float FadeDistance = 500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bAnimateGrid = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float GridPulseSpeed = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float GridOpacityMin = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float GridOpacityMax = 0.4f;
};

/**
 * VR Horizon Line Configuration
 */
USTRUCT(BlueprintType)
struct FVRHorizonLineConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    TSoftObjectPtr<UStaticMesh> HorizonMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    TSoftObjectPtr<UMaterialInterface> HorizonMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    FLinearColor HorizonColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    float HorizonDistance = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Horizon")
    float LineThickness = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bAlwaysVisible = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeInSpeed = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeOutSpeed = 2.0f;
};

/**
 * VR Snap Turn Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRSnapTurnConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    TSoftObjectPtr<UMaterialInterface> SnapTurnMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    FLinearColor SnapTurnColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float IndicatorSize = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visual")
    float IndicatorDistance = 200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float SnapDuration = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> SnapSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic")
    TSoftObjectPtr<UFeedbackEffect> SnapHaptic;
};

/**
 * VR Comfort Asset Definition
 */
UCLASS(BlueprintType)
class MINGGORTS_API UMingVRComfortAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Teleport Marker Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport Markers")
    FVRTeleportMarkerConfig DefaultMarkerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport Markers")
    FVRTeleportMarkerConfig ValidMarkerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport Markers")
    FVRTeleportMarkerConfig InvalidMarkerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport Markers")
    FVRTeleportMarkerConfig PortalMarkerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport Markers")
    FVRTeleportMarkerConfig WaypointMarkerConfig;

    // Comfort Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comfort")
    FVRComfortVignetteConfig VignetteConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comfort")
    FVRReferenceGridConfig ReferenceGridConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comfort")
    FVRHorizonLineConfig HorizonLineConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Comfort")
    FVRSnapTurnConfig SnapTurnConfig;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "VR Comfort Assets")
    FVRTeleportMarkerConfig GetMarkerConfig(ETeleportMarkerType MarkerType) const;

    UFUNCTION(BlueprintCallable, Category = "VR Comfort Assets")
    void SetMarkerColor(ETeleportMarkerType MarkerType, FLinearColor NewColor);

    UFUNCTION(BlueprintCallable, Category = "VR Comfort Assets")
    void SetVignetteIntensity(float NewIntensity);

    UFUNCTION(BlueprintCallable, Category = "VR Comfort Assets")
    void EnableComfortVisual(EComfortVisualType VisualType, bool bEnable);
};
