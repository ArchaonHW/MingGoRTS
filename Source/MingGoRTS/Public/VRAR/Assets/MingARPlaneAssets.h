// AR Plane Detection and Anchor Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - AR Plane and Anchor Assets

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingARPlaneAssets.generated.h"

/**
 * AR Plane Types
 */
UENUM(BlueprintType)
enum class EARPlaneType : uint8
{
    Floor           UMETA(DisplayName = "Floor"),
    Ceiling         UMETA(DisplayName = "Ceiling"),
    Wall            UMETA(DisplayName = "Wall"),
    Table           UMETA(DisplayName = "Table"),
    Seat            UMETA(DisplayName = "Seat"),
    Door            UMETA(DisplayName = "Door"),
    Window          UMETA(DisplayName = "Window"),
    Generic         UMETA(DisplayName = "Generic")
};

/**
 * AR Plane Detection State
 */
UENUM(BlueprintType)
enum class EARPlaneDetectionState : uint8
{
    Searching       UMETA(DisplayName = "Searching"),
    Detecting       UMETA(DisplayName = "Detecting"),
    Tracking        UMETA(DisplayName = "Tracking"),
    Lost            UMETA(DisplayName = "Lost"),
    Validated       UMETA(DisplayName = "Validated")
};

/**
 * AR Plane Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARPlaneVisualConfig
{
    GENERATED_BODY()

    // Plane Type
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Type")
    EARPlaneType PlaneType = EARPlaneType::Floor;

    // Mesh Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> PlaneMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> BoundaryMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> CornerMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> PlaneMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> BoundaryMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ValidPlaneMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InvalidPlaneMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> SelectedPlaneMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> HoverPlaneMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor PlaneColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.3f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor BoundaryColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.4f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.4f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SelectedColor = FLinearColor(1.0f, 0.8f, 0.0f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor HoverColor = FLinearColor(0.0f, 1.0f, 0.8f, 0.4f);

    // Pattern Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
    TSoftObjectPtr<UTexture2D> GridTexture;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
    float GridScale = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pattern")
    FLinearColor GridColor = FLinearColor(0.0f, 0.6f, 0.8f, 0.5f);

    // Boundary Settings
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
    float BoundaryWidth = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
    float CornerSize = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
    bool bShowCorners = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boundary")
    bool bShowBoundary = true;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnablePulseAnimation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseSpeed = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseOpacityMin = 0.2f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float PulseOpacityMax = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeInDuration = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float FadeOutDuration = 0.3f;

    // Detection Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    TSoftObjectPtr<UParticleSystem> DetectionEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    TSoftObjectPtr<UParticleSystem> ValidationEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    TSoftObjectPtr<USoundBase> DetectionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Detection")
    TSoftObjectPtr<USoundBase> ValidationSound;

    // Icon
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    TSoftObjectPtr<UTexture2D> PlaneTypeIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    float IconSize = 20.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Icon")
    FLinearColor IconColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
};

/**
 * AR Anchor Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARAnchorVisualConfig
{
    GENERATED_BODY()

    // Anchor Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> AnchorMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> RingMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ArrowMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> AnchorMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ActiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InactiveMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> SelectedMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor AnchorColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ActiveColor = FLinearColor(0.0f, 1.0f, 0.5f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InactiveColor = FLinearColor(0.5f, 0.5f, 0.5f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor SelectedColor = FLinearColor(1.0f, 0.8f, 0.0f, 1.0f);

    // Dimensions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float AnchorSize = 15.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float RingSize = 25.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dimensions")
    float ArrowLength = 30.0f;

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableRotation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float RotationSpeed = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableBobbing = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float BobbingAmplitude = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float BobbingSpeed = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    bool bEnableScaling = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ScalePulseMin = 0.9f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ScalePulseMax = 1.1f;

    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> PlacementEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> RemovalEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> UpdateEffect;

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> PlacementSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> RemovalSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> UpdateSound;
};

/**
 * AR Content Overlay Configuration
 */
USTRUCT(BlueprintType)
struct FARContentOverlayConfig
{
    GENERATED_BODY()

    // Content Types
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Types")
    TSoftObjectPtr<UStaticMesh> UnitOverlayMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Types")
    TSoftObjectPtr<UStaticMesh> BuildingOverlayMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Types")
    TSoftObjectPtr<UStaticMesh> ResourceOverlayMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Types")
    TSoftObjectPtr<UStaticMesh> UIPanelMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ContentMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> HologramMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> GhostMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ValidPlacementMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InvalidPlacementMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearContent ContentColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor HologramColor = FLinearColor(0.0f, 0.6f, 1.0f, 0.7f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor GhostColor = FLinearColor(1.0f, 1.0f, 1.0f, 0.4f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ValidPlacementColor = FLinearColor(0.0f, 1.0f, 0.0f, 0.6f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InvalidPlacementColor = FLinearColor(1.0f, 0.0f, 0.0f, 0.6f);

    // Effects
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> SpawnEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> DespawnEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
    TSoftObjectPtr<UParticleSystem> SelectionEffect;

    // Scaling
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scaling")
    float DefaultContentScale = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scaling")
    float MinContentScale = 0.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scaling")
    float MaxContentScale = 2.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scaling")
    bool bAutoScaleToPlane = true;

    // Occlusion
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Occlusion")
    bool bEnableOcclusion = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Occlusion")
    float OcclusionFadeDistance = 50.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Occlusion")
    float OcclusionIntensity = 0.8f;
};

/**
 * AR Scanning Visual Configuration
 */
USTRUCT(BlueprintType)
struct FARScanningVisualConfig
{
    GENERATED_BODY()

    // Scanning Mesh
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ScanningAreaMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ScanningLineMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ScanningPointMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ScanningAreaMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ScanningLineMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ScanningPointMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ScanningAreaColor = FLinearColor(0.0f, 0.5f, 1.0f, 0.2f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ScanningLineColor = FLinearColor(0.0f, 0.8f, 1.0f, 0.8f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ScanningPointColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ScanningLineSpeed = 100.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ScanningPointPulseSpeed = 3.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ScanningAreaPulseSpeed = 1.0f;

    // Instructions
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instructions")
    TSoftObjectPtr<UTexture2D> ScanningInstructionIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instructions")
    FText ScanningInstructionText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instructions")
    FLinearColor InstructionColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Audio
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> ScanningAmbientSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
    TSoftObjectPtr<USoundBase> PlaneDetectedSound;
};

/**
 * AR Plane and Anchor Asset Definition
 */
UCLASS(BlueprintType)
class MINGGORTS_API UMingARPlaneAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Plane Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Configs")
    FARPlaneVisualConfig FloorPlaneConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Configs")
    FARPlaneVisualConfig WallPlaneConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Configs")
    FARPlaneVisualConfig CeilingPlaneConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Configs")
    FARPlaneVisualConfig TablePlaneConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plane Configs")
    FARPlaneVisualConfig GenericPlaneConfig;

    // Anchor Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Anchor")
    FARAnchorVisualConfig AnchorConfig;

    // Content Overlay
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content Overlay")
    FARContentOverlayConfig ContentOverlayConfig;

    // Scanning Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Scanning")
    FARScanningVisualConfig ScanningConfig;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "AR Plane Assets")
    FARPlaneVisualConfig GetPlaneConfig(EARPlaneType PlaneType) const;

    UFUNCTION(BlueprintCallable, Category = "AR Plane Assets")
    void SetPlaneColor(EARPlaneType PlaneType, FLinearColor NewColor);

    UFUNCTION(BlueprintCallable, Category = "AR Plane Assets")
    void SetAnchorColor(FLinearColor NewColor);

    UFUNCTION(BlueprintCallable, Category = "AR Plane Assets")
    float GetRecommendedContentScale(float PlaneArea) const;
};
