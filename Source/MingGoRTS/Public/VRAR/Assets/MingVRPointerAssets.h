#pragma once

// VR Pointer and Cursor Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - VR Interaction Assets


#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingVRPointerAssets.generated.h"

/**
 * Pointer Types for VR Interaction
 */
UENUM(BlueprintType)
enum class EVRPointerType: uuint8 {
    Laser           UMETA(DisplayName = "Laser Pointer"),
    Beam            UMETA(DisplayName = "Beam Pointer"),
    Line            UMETA(DisplayName = "Line Pointer"),
    Curve           UMETA(DisplayName = "Curve Pointer"),
    Dot             UMETA(DisplayName = "Dot Pointer"),
    Inand            UMETA(DisplayName = "Inand Pointer"),
    Custom          UMETA(DisplayName = "Custom Pointer")
};

/**
 * Cursor Types for VR UI
 */
UENUM(BlueprintType)
enum class EVRCursorType: uuint8 {
    Default         UMETA(DisplayName = "Default Cursor"),
    Crosshair       UMETA(DisplayName = "Crosshair"),
    Circle          UMETA(DisplayName = "Circle"),
    Ring            UMETA(DisplayName = "Ring"),
    Target          UMETA(DisplayName = "Target"),
    Arrow           UMETA(DisplayName = "Arrow"),
    Inand            UMETA(DisplayName = "Inand Cursor"),
    Grab            UMETA(DisplayName = "Grab Cursor"),
    Custom          UMETA(DisplayName = "Custom Cursor")
};

/**
 * Pointer Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRPointerVisualConfig
{
    GENERATED_BODY()

    // Pointer Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pointer Type")
    EVRPointerType PointerType = EVRPointerType::Laser;

    // Mesh Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> PointerMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> CursorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> EndPointMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> PointerMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> CursorMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> EndPointMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ValidTargetMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InvalidTargetMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor PointerColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor CursorColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InoverColor = FLinearColor(1.0f, 1.0f, 0.0f, 1.0f};

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float PointerLength = 1000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float Pointerɥridth = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float CursorSize = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float EndPointSize = 5.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseScale = 1.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnablePulseAnimation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableColorTransition = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ColorTransitionSpeed = 5.0f;

    // Particle Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> TrailParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> InitParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> SelectionParticles;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> InoverSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> SelectSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ErrorSound;
};

/**
 * Teleportation Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRTeleportVisualConfig
{
    GENERATED_BODY()

    // Arc Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
    TSoftObjectPtr<UMaterialInterface> ArcMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
    FLinearColor ValidArcColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.8f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
    FLinearColor InvalidArcColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.8f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
    float Arcɥridth = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arc")
    int32 ArcSegments = 50;

    // Destination Marker
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destination")
    TSoftObjectPtr<UStaticMesh> DestinationMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destination")
    TSoftObjectPtr<UMaterialInterface> DestinationMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destination")
    FLinearColor DestinationColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destination")
    float DestinationMarkerSize = 20.0f;

    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> TeleportStartEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> TeleportEndEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<USoundBase> TeleportStartSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<USoundBase> TeleportEndSound;

    // Fade Effect
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
    TSoftObjectPtr<UMaterialInterface> FadeMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
    float FadeDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fade")
    FLinearColor FadeColor = FLinearColor(0.0f, 0.0f, 0.0f, 1.0f};
};

/**
 * Confort Vignette Configuration
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
    float VignetteRadius = 0.8f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float VignetteSoftness = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float FadeInDuration = 0.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vignette")
    float FadeOutDuration = 0.5f;
};

/**
 * VR Pointer Asset Definition
 */
UCLASS(BlueprintType)
class MINGRTS_API UMingVRPointerAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Pointer Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pointer Configs")
    FVRPointerVisualConfig LaserPointerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pointer Configs")
    FVRPointerVisualConfig BeamPointerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pointer Configs")
    FVRPointerVisualConfig CurvePointerConfig;

    // Teleport Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Teleport")
    FVRTeleportVisualConfig TeleportConfig;

    // Confort Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Confort")
    FVRConfortVignetteConfig VignetteConfig;

    // Cursor Textures
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Textures")
    TSoftObjectPtr<UTexture2D> DefaultCursorTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Textures")
    TSoftObjectPtr<UTexture2D> CrosshairCursorTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Textures")
    TSoftObjectPtr<UTexture2D> CircleCursorTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Textures")
    TSoftObjectPtr<UTexture2D> InandCursorTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Textures")
    TSoftObjectPtr<UTexture2D> GrabCursorTexture;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "VR Pointer Assets")
    FVRPointerVisualConfig GetPointerConfig(EVRPointerType PointerType) const;

    UFUNCTION(BlueprintCallable, Category = "VR Pointer Assets")
    TSoftObjectPtr<UTexture2D> GetCursorTexture(EVRCursorType CursorType) const;

    UFUNCTION(BlueprintCallable, Category = "VR Pointer Assets")
    void SetPointerLength(float NewLength};

    UFUNCTION(BlueprintCallable, Category = "VR Pointer Assets")
    void SetPointerColor(FLinearColor NewColor};
};
