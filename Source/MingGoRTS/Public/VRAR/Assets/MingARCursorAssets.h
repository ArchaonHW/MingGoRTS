// AR Cursor and Interaction Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - AR Interaction Assets

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingARCursorAssets.generated.h"

/**
 * AR Cursor Types
 */
UENUM(BlueprintType)
enum class EARCursorType: uint8 {
    Default         UMETA(DisplayName = "Default Cursor"),
    Touch           UMETA(DisplayName = "Touch Cursor"),
    Pinch           UMETA(DisplayName = "Pinch Cursor"),
    Drag            UMETA(DisplayName = "Drag Cursor"),
    Select          UMETA(DisplayName = "Select Cursor"),
    Place           UMETA(DisplayName = "Place Cursor"),
    Rotate          UMETA(DisplayName = "Rotate Cursor"),
    Scale           UMETA(DisplayName = "Scale Cursor"),
    Raycast         UMETA(DisplayName = "Raycast Cursor"),
    Hand            UMETA(DisplayName = "Hand Cursor"),
    Custom          UMETA(DisplayName = "Custom Cursor")
};

/**
 * AR Gesture Types
 */
UENUM(BlueprintType)
enum class EARGestureType: uint8 {
    Tap             UMETA(DisplayName = "Tap"),
    DoubleTap       UMETA(DisplayName = "Double Tap"),
    LongPress       UMETA(DisplayName = "Long Press"),
    SwipeLeft       UMETA(DisplayName = "Swipe Left"),
    SwipeRight      UMETA(DisplayName = "Swipe Right"),
    SwipeUp         UMETA(DisplayName = "Swipe Up"),
    SwipeDown       UMETA(DisplayName = "Swipe Down"),
    PinchIn         UMETA(DisplayName = "Pinch In"),
    PinchOut        UMETA(DisplayName = "Pinch Out"),
    RotateCW        UMETA(DisplayName = "Rotate Clockwise"),
    RotateCCW       UMETA(DisplayName = "Rotate Counter-Clockwise"),
    Pan             UMETA(DisplayName = "Pan")
};

/**
 * AR Cursor Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARCursorVisualConfig
{
    GENERATED_BODY()

    // Cursor Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Type")
    EARCursorType CursorType = EARCursorType::Default;

    // Mesh Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> CursorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> TouchIndicatorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> GestureTrailMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> CursorMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ActiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InactiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ValidTargetMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InvalidTargetMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor CursorColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ActiveColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InactiveColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidTargetColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidTargetColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float CursorSize = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float TouchIndicatorSize = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float TrailWidth = 3.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnablePulseAnimation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseScale = 1.3f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeInDuration = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeOutDuration = 0.3f;

    // Particle Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> TouchEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> ReleaseEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particles")
    TSoftObjectPtr<UParticleSystem> GestureEffect;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> TouchSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ReleaseSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> SelectSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ErrorSound;
};

/**
 * AR Gesture Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARGestureVisualConfig
{
    GENERATED_BODY()

    // Gesture Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture")
    EARGestureType GestureType = EARGestureType::Tap;

    // Trail Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    TSoftObjectPtr<UMaterialInterface> TrailMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    float TrailWidth = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    float TrailDuration = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    FLinearColor TrailColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trail")
    bool bFadeTrail = true;

    // Icon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    TSoftObjectPtr<UTexture2D> GestureIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    float IconSize = 30.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    float IconDisplayDuration = 1.5f;

    // Haptic Feedback (for supported devices)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic")
    TSoftObjectPtr<UFeedbackEffect> GestureHaptic;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Haptic")
    float HapticIntensity = 0.5f;
};

/**
 * AR Raycast Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARRaycastVisualConfig
{
    GENERATED_BODY()

    // Raycast Line
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
    TSoftObjectPtr<UStaticMesh> RaycastLineMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
    TSoftObjectPtr<UMaterialInterface> LineMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
    FLinearColor LineColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
    float LineWidth = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Line")
    float MaxLineLength = 1000.0f;

    // Hit Point Indicator
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
    TSoftObjectPtr<UStaticMesh> HitPointMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
    TSoftObjectPtr<UMaterialInterface> HitPointMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
    FLinearColor HitPointColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hit Point")
    float HitPointSize = 10.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bAnimateLine = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float AnimationSpeed = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bPulseHitPoint = true;
};

/**
 * AR Hand Overlay Configuration (for hand tracking)
 */
USTRUCT(BlueprintType)
struct FARHandOverlayConfig
{
    GENERATED_BODY()

    // Hand Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<USkeletalMesh> HandMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UMaterialInterface> HandMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UMaterialInterface> JointMaterial;

    // Joint Indicators
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joints")
    TSoftObjectPtr<UStaticMesh> JointMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joints")
    float JointSize = 5.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Joints")
    FLinearColor JointColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    // Gesture Recognition Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture")
    TSoftObjectPtr<UParticleSystem> GestureRecognizedEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture")
    FLinearColor GestureHighlightColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f);

    // Opacity
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
    float HandOpacity = 0.7f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
    bool bShowJoints = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
    bool bShowBones = true;
};

/**
 * AR Cursor Asset Definition
 */
UCLASS(BlueprintType)
class MINGRTS_API UMingARCursorAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Cursor Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Configs")
    FARCursorVisualConfig DefaultCursorConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Configs")
    FARCursorVisualConfig TouchCursorConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Configs")
    FARCursorVisualConfig PinchCursorConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Configs")
    FARCursorVisualConfig SelectCursorConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor Configs")
    FARCursorVisualConfig PlaceCursorConfig;

    // Gesture Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture Configs")
    FARGestureVisualConfig TapGestureConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture Configs")
    FARGestureVisualConfig SwipeGestureConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture Configs")
    FARGestureVisualConfig PinchGestureConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gesture Configs")
    FARGestureVisualConfig RotateGestureConfig;

    // Raycast Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raycast")
    FARRaycastVisualConfig RaycastConfig;

    // Hand Overlay
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Tracking")
    FARHandOverlayConfig HandOverlayConfig;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "AR Cursor Assets")
    FARCursorVisualConfig GetCursorConfig(EARCursorType CursorType) const;

    UFUNCTION(BlueprintCallable, Category = "AR Cursor Assets")
    FARGestureVisualConfig GetGestureConfig(EARGestureType GestureType) const;

    UFUNCTION(BlueprintCallable, Category = "AR Cursor Assets")
    void SetCursorColor(FLinearColor NewColor);

    UFUNCTION(BlueprintCallable, Category = "AR Cursor Assets")
    void SetCursorSize(float NewSize);
};
