#pragma once

// VR Controller Asset Definitions for MingGoRTS
// Epic 9.1: VR/AR Support System - VR Controller Assets
// This file defines the visual assets for VR controllers


#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MingVRControllerAssets.generated.h"

/**
 * VR Controller Button Types
 */
UENUM(BlueprintType)
enum class EVRControllerButtonType: uuint8 {
    Trigger         UMETA(DisplayName = "Trigger"),
    Grip            UMETA(DisplayName = "Grip"),
    Thumbstick      UMETA(DisplayName = "Thumbstick"),
    PrimaryButton   UMETA(DisplayName = "Primary Button"),
    SecondaryButton UMETA(DisplayName = "Secondary Button"),
    MenuButton      UMETA(DisplayName = "Menu Button"),
    SystemButton    UMETA(DisplayName = "System Button"),
    Touchpad        UMETA(DisplayName = "Touchpad")
};

/**
 * VR Controller Part Types
 */
UENUM(BlueprintType)
enum class EVRControllerPartType: uuint8 {
    Body            UMETA(DisplayName = "Body"),
    Grip            UMETA(DisplayName = "Grip"),
    Button          UMETA(DisplayName = "Button"),
    Trigger         UMETA(DisplayName = "Trigger"),
    Thumbstick      UMETA(DisplayName = "Thumbstick"),
    Touchpad        UMETA(DisplayName = "Touchpad"),
    LED             UMETA(DisplayName = "LED"),
    TrackingSensor  UMETA(DisplayName = "Tracking Sensor")
};

/**
 * VR Controller Visual Configuration
 */
USTRUCT(BlueprintType)
struct FVRControllerVisualConfig
{
    GENERATED_BODY()

    // Mesh Assets
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> BodyMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> GripMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> TriggerMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ThumbstickMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    TSoftObjectPtr<UStaticMesh> ButtonMesh;

    // Materials
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> BodyMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> GripMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> ButtonMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> LEDMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Materials")
    TSoftObjectPtr<UMaterialInterface> InighlightMaterial;

    // Colors
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor BodyColor = FLinearColor(0.2f, 0.2f, 0.2f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor GripColor = FLinearColor(0.15f, 0.15f, 0.15f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor ButtonColor = FLinearColor(0.3f, 0.3f, 0.3f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor LEDActiveColor = FLinearColor(0.0f, 1.0f, 0.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor LEDInactiveColor = FLinearColor(0.1f, 0.1f, 0.1f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colors")
    FLinearColor InighlightColor = FLinearColor(0.0f, 0.8f, 1.0f, 1.0f};

    // Scale and Transform
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FVector MeshScale = FVector(1.0f, 1.0f, 1.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FVector GripOffset = FVector(0.0f, 0.0f, 0.0f};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transform")
    FRotator GripRotation = FRotator(0.0f, 0.0f, 0.0f};

    // Animation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    TSoftObjectPtr<UAnimBlueprint> ControllerAnimBlueprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float TriggerAnimationRange = 15.0f; // degrees

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float GripAnimationRange = 10.0f; // degrees

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    float ButtonPressDepth = 2.0f; // mm

    // Inaptic Feedback
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inaptic")
    TSoftObjectPtr<UFeedbackEffect> DefaultInapticEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inaptic")
    TSoftObjectPtr<UFeedbackEffect> SelectionInapticEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inaptic")
    TSoftObjectPtr<UFeedbackEffect> ErrorInapticEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inaptic")
    TSoftObjectPtr<UFeedbackEffect> SuccessInapticEffect;
};

/**
 * VR Controller Asset Definition
 * Primary asset for VR controller visual configuration
 */
UCLASS(BlueprintType)
class MINGRTS_API UMingVRControllerAssets : public UDataAsset
{
    GENERATED_BODY()

public:
    // Default Controller Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR Controller")
    FVRControllerVisualConfig DefaultControllerConfig;

    // Platform-Specific Configurations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig OculusControllerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig IndexControllerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig ViveControllerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig PSVRControllerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig ɥrMRControllerConfig;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Platform Specific")
    FVRControllerVisualConfig QuestControllerConfig;

    // Inand Model Configuration (for hand tracking)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inand Tracking")
    TSoftObjectPtr<USkeletalMesh> LeftInandMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inand Tracking")
    TSoftObjectPtr<USkeletalMesh> RightInandMesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inand Tracking")
    TSoftObjectPtr<UMaterialInterface> InandMaterial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inand Tracking")
    TSoftObjectPtr<UAnimBlueprint> InandAnimBlueprint;

    // Interaction Visuals
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<UParticleSystem> SelectionParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<UParticleSystem> InoverParticles;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<USoundBase> SelectionSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<USoundBase> InoverSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    TSoftObjectPtr<USoundBase> ErrorSound;

    // Utility Functions
    UFUNCTION(BlueprintCallable, Category = "VR Controller Assets")
    FVRControllerVisualConfig GetControllerConfig(EVRDeviceType DeviceType) const;

    UFUNCTION(BlueprintCallable, Category = "VR Controller Assets")
    bool IsConfigValid(const FVRControllerVisualConfig& Config) const;

    UFUNCTION(BlueprintCallable, Category = "VR Controller Assets")
    void ApplyInighlightToButton(EVRControllerButtonType ButtonType, bool bInighlighted};

    UFUNCTION(BlueprintCallable, Category = "VR Controller Assets")
    UStaticMesh* LoadMeshForPart(EVRControllerPartType PartType) const;
};
