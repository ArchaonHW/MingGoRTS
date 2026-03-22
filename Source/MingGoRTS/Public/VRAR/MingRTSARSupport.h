// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - AR Support Component

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/Engine.h"
#include "MingRTSARSupport.generated.h"

/**
 * AR Device Types supported by MingGoRTS
 */
UENUM(BlueprintType)
enum class EARDeviceType: uint8 {
    None                UMETA(DisplayName = "None"),
    ARCore              UMETA(DisplayName = "ARCore (Android)"),
    ARKit               UMETA(DisplayName = "ARKit (iOS)"),
    MicrosoftHoloLens   UMETA(DisplayName = "Microsoft HoloLens"),
    MagicLeap           UMETA(DisplayName = "Magic Leap"),
    Nreal               UMETA(DisplayName = "Nreal Light"),
    Custom              UMETA(DisplayName = "Custom AR Device")
};

/**
 * AR Tracking Quality Levels
 */
UENUM(BlueprintType)
enum class EARTrackingQuality: uint8 {
    NotAvailable    UMETA(DisplayName = "Not Available"),
    Limited         UMETA(DisplayName = "Limited Tracking"),
    Normal          UMETA(DisplayName = "Normal Tracking"),
    Good            UMETA(DisplayName = "Good Tracking"),
    Excellent       UMETA(DisplayName = "Excellent Tracking")
};

/**
 * AR Session Status
 */
UENUM(BlueprintType)
enum class EARSessionStatus: uint8 {
    NotStarted      UMETA(DisplayName = "Not Started"),
    Starting        UMETA(DisplayName = "Starting"),
    Running         UMETA(DisplayName = "Running"),
    Paused          UMETA(DisplayName = "Paused"),
    Interrupted     UMETA(DisplayName = "Interrupted"),
    Error           UMETA(DisplayName = "Error"),
    Stopping        UMETA(DisplayName = "Stopping")
};

/**
 * AR Content Overlay Types
 */
UENUM(BlueprintType)
enum class EARContentOverlayType: uint8 {
    GameWorld       UMETA(DisplayName = "Game World Overlay"),
    UI              UMETA(DisplayName = "UI Overlay"),
    Minimap         UMETA(DisplayName = "Minimap Overlay"),
    UnitInfo        UMETA(DisplayName = "Unit Info Overlay"),
    BuildingInfo    UMETA(DisplayName = "Building Info Overlay"),
    TerrainInfo     UMETA(DisplayName = "Terrain Info Overlay"),
    Tactical        UMETA(DisplayName = "Tactical Overlay"),
    Custom          UMETA(DisplayName = "Custom Overlay")
};

/**
 * AR Plane Detection Modes
 */
UENUM(BlueprintType)
enum class EARPlaneDetectionMode: uint8 {
    None            UMETA(DisplayName = "None"),
    Horizontal      UMETA(DisplayName = "Horizontal Only"),
    Vertical        UMETA(DisplayName = "Vertical Only"),
    Both            UMETA(DisplayName = "Both Horizontal and Vertical")
};

/**
 * AR Session Configuration
 */
USTRUCT(BlueprintType)
struct FARSessionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    EARPlaneDetectionMode PlaneDetectionMode = EARPlaneDetectionMode::Horizontal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bEnableLightEstimation = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bEnableAutoFocus = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bUseMetalForARKit = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bEnableSceneDepth = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    bool bEnablePeopleOcclusion = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Session")
    TArray<EARContentOverlayType> EnabledOverlays;
};

/**
 * AR Tracked Plane Information
 */
USTRUCT(BlueprintType)
struct FARTrackedPlane
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    FGuid PlaneID;

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    FVector CenterPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    FVector Extents;

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    FQuat Orientation;

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    bool bIsHorizontal = true;

    UPROPERTY(BlueprintReadOnly, Category = "AR Plane")
    TArray<FVector> BoundaryPoints;
};

/**
 * AR Content Overlay Info
 */
USTRUCT(BlueprintType)
struct FARContentOverlay
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    EARContentOverlayType OverlayType = EARContentOverlayType::GameWorld;

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    FTransform WorldTransform;

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    FVector2D ScreenPosition;

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    float DistanceFromCamera = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    bool bIsVisible = true;

    UPROPERTY(BlueprintReadOnly, Category = "AR Overlay")
    float Opacity = 1.0f;
};

/**
 * MingGoRTS AR Support System
 * Provides comprehensive AR functionality for the RTS game
 */
UCLASS()
class MINGRTS_API UMingRTSARSupport : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "AR Support")
    void InitializeARSupport();

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    void ShutdownARSupport();

    // Device Management
    UFUNCTION(BlueprintCallable, Category = "AR Support")
    bool IsARSupported() const;

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    EARDeviceType GetARDeviceType() const;

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    bool IsARSessionRunning() const;

    // Session Control
    UFUNCTION(BlueprintCallable, Category = "AR Support")
    bool StartARSession(const FARSessionConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    void StopARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    void PauseARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    void ResumeARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    EARSessionStatus GetARSessionStatus() const { return SessionStatus; }

    // Tracking Quality
    UFUNCTION(BlueprintCallable, Category = "AR Support")
    EARTrackingQuality GetTrackingQuality() const;

    UFUNCTION(BlueprintCallable, Category = "AR Support")
    bool IsTrackingGood() const;

    // Plane Detection
    UFUNCTION(BlueprintCallable, Category = "AR Support|Planes")
    TArray<FARTrackedPlane> GetDetectedPlanes() const;

    UFUNCTION(BlueprintCallable, Category = "AR Support|Planes")
    void SetPlaneDetectionMode(EARPlaneDetectionMode Mode};

    // Content Overlay
    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    void RegisterContentOverlay(EARContentOverlayType Type, const FTransform& WorldTransform);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    void UnregisterContentOverlay(EARContentOverlayType Type);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    void UpdateOverlayPosition(EARContentOverlayType Type, const FTransform& NewTransform);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    void SetOverlayVisibility(EARContentOverlayType Type, bool bVisible);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    void SetOverlayOpacity(EARContentOverlayType Type, float Opacity);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Overlay")
    TArray<FARContentOverlay> GetActiveOverlays() const;

    // Raycasting
    UFUNCTION(BlueprintCallable, Category = "AR Support|Raycast")
    bool PerformARRaycast(const FVector2D& ScreenPosition, FVector& OutHitLocation, FVector& OutHitNormal);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Raycast")
    bool PerformARRaycastFromCenter(FVector& OutHitLocation, FVector& OutHitNormal};

    // Game World Integration
    UFUNCTION(BlueprintCallable, Category = "AR Support|Game")
    void SpawnGameWorldOnPlane(const FARTrackedPlane& Plane, const FVector& Offset);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Game")
    void AlignGameWorldToRealWorld();

    UFUNCTION(BlueprintCallable, Category = "AR Support|Game")
    void SetGameWorldScale(float Scale);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Game")
    float GetGameWorldScale() const { return GameWorldScale; }

    // Lighting
    UFUNCTION(BlueprintCallable, Category = "AR Support|Lighting")
    FLinearColor GetAREnvironmentColor() const;

    UFUNCTION(BlueprintCallable, Category = "AR Support|Lighting")
    float GetAREnvironmentIntensity() const;

    // Performance
    UFUNCTION(BlueprintCallable, Category = "AR Support|Performance")
    void SetARPerformanceMode(bool bLowPowerMode);

    UFUNCTION(BlueprintCallable, Category = "AR Support|Performance")
    bool IsLowPowerModeEnabled() const { return bLowPowerMode; }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "AR Support|Events")
    FOnARSessionStarted OnARSessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "AR Support|Events")
    FOnARSessionStopped OnARSessionStopped;

    UPROPERTY(BlueprintAssignable, Category = "AR Support|Events")
    FOnARPlaneDetected OnARPlaneDetected;

    UPROPERTY(BlueprintAssignable, Category = "AR Support|Events")
    FOnARTrackingQualityChanged OnTrackingQualityChanged;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

private:
    UPROPERTY()
    EARSessionStatus SessionStatus = EARSessionStatus::NotStarted;

    UPROPERTY()
    FARSessionConfig CurrentSessionConfig;

    UPROPERTY()
    TArray<FARTrackedPlane> DetectedPlanes;

    UPROPERTY()
    TMap<EARContentOverlayType, FARContentOverlay> ActiveOverlays;

    UPROPERTY()
    float GameWorldScale = 1.0f;

    UPROPERTY()
    bool bLowPowerMode = false;

    UPROPERTY()
    FTransform RealWorldToGameWorldTransform;

    void UpdateDetectedPlanes();
    void UpdateContentOverlays();
    void UpdateLightingEstimation();
    void UpdateSessionStatus(EARSessionStatus NewStatus);
    EARDeviceType DetectARDeviceType() const;
};

// Event delegate declarations
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnARSessionStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnARSessionStopped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARPlaneDetected, const FARTrackedPlane&, Plane};



