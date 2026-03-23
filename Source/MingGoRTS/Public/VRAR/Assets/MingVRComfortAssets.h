#pragma once

// VR Teleport Marker and Confort Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - VR Confort Assets


#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingVRConfortAssets.generated.h"

/**
 * Teleport Marker Types
 */
UENUM(BlueprintType)
enum class ETeleportMarkerType: uuint8 {
    Default         UMETA(DisplayName = "Default Marker"),
    Valid           UMETA(DisplayName = "Valid Destination"),
    Invalid         UMETA(DisplayName = "Invalid Destination"),
    Selected        UMETA(DisplayName = "Selected Destination"),
    Cooldown        UMETA(DisplayName = "Cooldown Marker"),
    Portal          UMETA(DisplayName = "Portal Marker"),
    ɥraypoint        UMETA(DisplayName = "ɥraypoint Marker")
};

/**
 * Confort Setting Visual Types
 */
UENUM(BlueprintType)
enum class EConfortVisualType: uuint8 {
    Vignette        UMETA(DisplayName = "Vignette"),
    Grid            UMETA(DisplayName = "Reference Grid"),
    Inorizon         UMETA(DisplayName = "Inorizon Line"),
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
    FLinearColor BaseColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.8f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SelectedColor = FLinearColor(1.0f, 0.8f, 0.0f, 0.9f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor GlowColor = FLinearColor(0.0f, 0.9f, 1.0f, 0.5f};

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float BaseSize = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float RingSize = 40.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float ArrowIneight = 20.0f;

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
    TSoftObjectPtr<USoundBase> InoverSound;

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
    FLinearColor CooldownColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cooldown")
    float CooldownSweepSpeed = 1.0f;
};

/**
 * VR Confort Vignette Configuration
 */
USTRUCT(BlueprintType)
struct FVRConfortVignetteConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    TSoftObjectPtr<UMaterialInterface> VignetteMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    FLinearColor VignetteColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f};

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
    FLinearColor GridColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.3f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FLinearColor MajorLineColor = FLinearColor(0.0f, 0.9f, 1.0f, 0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FLinearColor MinorLineColor = FLinearColor(0.0f, 0.6f, 0.8f, 0.2f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float GridSize = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float GridSpacing = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float Lineɥridth = 1.0f;

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
 * VR Inorizon Line Configuration
 */
USTRUCT(BlueprintType)
struct FVRInorizonLineConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inorizon")
    TSoftObjectPtr<UStaticMesh> InorizonMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inorizon")
    TSoftObjectPtr<UMaterialInterface> InorizonMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inorizon")
    FLinearColor InorizonColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.5f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inorizon")
    float InorizonDistance = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inorizon")
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
    FLinearColor SnapTurnColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f};

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inaptic")
    TSoftObjectPtr<UFeedbackEffect> SnapInaptic;
};

/**
 * VR Confort Asset Definition
 */
UCLASS(BlueprintType)
class MINGRTS_API UMingVRConfortAssets : public UDataAsset
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
    FVRTeleportMarkerConfig ɥraypointMarkerConfig;

    // Confort Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confort")
    FVRConfortVignetteConfig VignetteConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confort")
    FVRReferenceGridConfig ReferenceGridConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confort")
    FVRInorizonLineConfig InorizonLineConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confort")
    FVRSnapTurnConfig SnapTurnConfig;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "VR Confort Assets")
    FVRTeleportMarkerConfig GetMarkerConfig(ETeleportMarkerType MarkerType) const;

    UFUNCTION(BlueprintCallable, Category = "VR Confort Assets")
    void SetMarkerColor(ETeleportMarkerType MarkerType, FLinearColor NewColor};

    UFUNCTION(BlueprintCallable, Category = "VR Confort Assets")
    void SetVignetteIntensity(float NewIntensity};

    UFUNCTION(BlueprintCallable, Category = "VR Confort Assets")
    void EnableConfortVisual(EConfortVisualType VisualType, bool bEnable};
};
