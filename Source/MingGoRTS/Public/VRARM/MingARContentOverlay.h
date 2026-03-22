// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AR Content Overlay System - C1-2
// Provides AR content overlay and AR interaction features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingARContentOverlay.generated.h"

UENUM(BlueprintType)
UENUM(BlueprintType)\nenum class EARDeviceType : uint8\n{
    None = 0, UMETA(DisplayName = "None"),
    AppleARKit, UMETA(DisplayName = "Apple ARKit"),
    GoogleARCore, UMETA(DisplayName = "Google ARCore"),
    MicrosoftHoloLens, UMETA(DisplayName = "Microsoft HoloLens"),
    MagicLeap, UMETA(DisplayName = "Magic Leap"),
    MetaQuestPro, UMETA(DisplayName = "Meta Quest Pro"),
    GenericAR UMETA(DisplayName = "Generic AR")
};

UENUM(BlueprintType)
enum class EARTrackingState: uint8 {
    NotTracking = 0, UMETA(DisplayName = "Not Tracking"),
    Limited, UMETA(DisplayName = "Limited"),
    Normal, UMETA(DisplayName = "Normal")
};

UENUM(BlueprintType)
enum class EARAnchorType: uint8 {
    Plane = 0, UMETA(DisplayName = "Plane"),
    Point, UMETA(DisplayName = "Point"),
    Image, UMETA(DisplayName = "Image"),
    Face, UMETA(DisplayName = "Face"),
    QRCode, UMETA(DisplayName = "QR Code"),
    Object, UMETA(DisplayName = "Object"),
    GeoAnchor, UMETA(DisplayName = "Geo Anchor"),
    Mesh, UMETA(DisplayName = "Mesh")
};

UENUM(BlueprintType)
enum class EARPlaneAlignment: uint8 {
    HorizontalUp = 0, UMETA(DisplayName = "Horizontal Up"),
    HorizontalDown, UMETA(DisplayName = "Horizontal Down"),
    Vertical, UMETA(DisplayName = "Vertical"),
    None UMETA(DisplayName = "None")
};

USTRUCT(BlueprintType)
struct FARPlaneGeometry
{
    GENERATED_BODY()

    UPROPERTY()
    FVector Center;

    UPROPERTY()
    FVector Extent;

    UPROPERTY()
    EARPlaneAlignment Alignment;

    UPROPERTY()
    TArray<FVector> BoundaryPoints;

    UPROPERTY()
    FQuat Orientation;

    FARPlaneGeometry()
        : Center(FVector::ZeroVector)
        , Extent(FVector::ZeroVector)
        , Alignment(EARPlaneAlignment::None)
    {}
};

USTRUCT(BlueprintType)
struct FARTrackedImage
{
    GENERATED_BODY()

    UPROPERTY()
    FName ImageName;

    UPROPERTY()
    UTexture2D* ImageTexture;

    UPROPERTY()
    FVector2D PhysicalSize;

    UPROPERTY()
    FTransform TrackedTransform;

    UPROPERTY()
    float TrackingConfidence;

    UPROPERTY()
    bool bIsTracked;

    FARTrackedImage()
        : ImageTexture(nullptr)
        , PhysicalSize(FVector2D::ZeroVector)
        , TrackingConfidence(0.0f)
        , bIsTracked(false)
    {}
};

USTRUCT(BlueprintType)
struct FARAnchorData
{
    GENERATED_BODY()

    UPROPERTY()
    FName AnchorID;

    UPROPERTY()
    EARAnchorType Type;

    UPROPERTY()
    FTransform Transform;

    UPROPERTY()
    bool bIsValid;

    UPROPERTY()
    float Confidence;

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    FARPlaneGeometry PlaneGeometry;

    UPROPERTY()
    FARTrackedImage TrackedImage;

    FARAnchorData()
        : Type(EARAnchorType::Plane)
        , bIsValid(false)
        , Confidence(0.0f)
        , Timestamp(0)
    {}
};

USTRUCT(BlueprintType)
struct FAROverlayConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnablePlaneDetection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableHorizontalPlaneDetection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableVerticalPlaneDetection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableImageTracking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableFaceTracking;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableObjectDetection;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableMeshGeneration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableAutoFocus;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    float MinPlaneSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    TArray<FARTrackedImage> ReferenceImages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableLightEstimation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AR Overlay")
    bool bEnableDepthTracking;

    FAROverlayConfig()
        : bEnablePlaneDetection(true)
        , bEnableHorizontalPlaneDetection(true)
        , bEnableVerticalPlaneDetection(true)
        , bEnableImageTracking(false)
        , bEnableFaceTracking(false)
        , bEnableObjectDetection(false)
        , bEnableMeshGeneration(false)
        , bEnableAutoFocus(true)
        , MinPlaneSize(100.0f)
        , bEnableLightEstimation(true)
        , bEnableDepthTracking(false)
    {}
};

USTRUCT(BlueprintType)
struct FARContentOverlay
{
    GENERATED_BODY()

    UPROPERTY()
    FName OverlayID;

    UPROPERTY()
    FName AnchorID;

    UPROPERTY()
    TSoftObjectPtr<UStaticMesh> StaticMesh;

    UPROPERTY()
    TArray<UMaterialInterface*> Materials;

    UPROPERTY()
    FTransform RelativeTransform;

    UPROPERTY()
    bool bOccludeByEnvironment;

    UPROPERTY()
    bool bReceiveEnvironmentLighting;

    UPROPERTY()
    bool bCastShadows;

    FARContentOverlay()
        : bOccludeByEnvironment(true)
        , bReceiveEnvironmentLighting(true)
        , bCastShadows(true)
    {}
};

USTRUCT(BlueprintType)
struct FARLightEstimate
{
    GENERATED_BODY()

    UPROPERTY()
    FLinearColor AmbientColor;

    UPROPERTY()
    float AmbientIntensity;

    UPROPERTY()
    FLinearColor ColorTemperature;

    UPROPERTY()
    FVector DirectionalLightDirection;

    UPROPERTY()
    float DirectionalLightIntensity;

    UPROPERTY()
    bool bIsValid;

    FARLightEstimate()
        : AmbientIntensity(1.0f)
        , DirectionalLightIntensity(1.0f)
        , bIsValid(false)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARSessionStarted, EARDeviceType, DeviceType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnARSessionStopped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARPlaneDetected, FARAnchorData, PlaneAnchor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARImageTracked, FARTrackedImage, TrackedImage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARAnchorAdded, FARAnchorData, Anchor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARAnchorUpdated, FARAnchorData, Anchor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARAnchorRemoved, FName, AnchorID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARLightEstimateUpdated, FARLightEstimate, LightEstimate);

/**
 * AR Content Overlay System
 * Provides AR content overlay and AR interaction features
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingARContentOverlay : public UObject
{
    GENERATED_BODY()

public:
    UMingARContentOverlay();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void InitializeARSystem(const FAROverlayConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void ShutdownARSystem();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool StartARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void StopARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool IsARSessionRunning() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    EARDeviceType GetARDeviceType() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    EARTrackingState GetTrackingState() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    FAROverlayConfig GetConfiguration() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void UpdateConfiguration(const FAROverlayConfig& NewConfig);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    TArray<FARAnchorData> GetAllAnchors() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    TArray<FARAnchorData> GetPlaneAnchors() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    TArray<FARAnchorData> GetImageAnchors() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    FARAnchorData GetAnchorByID(FName AnchorID) const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    FName AddContentOverlay(const FARContentOverlay& Overlay);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void RemoveContentOverlay(FName OverlayID);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void UpdateContentOverlayTransform(FName OverlayID, const FTransform& NewTransform);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    TArray<FARContentOverlay> GetAllOverlays() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool PinComponentToAnchor(USceneComponent* Component, FName AnchorID);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void SetOverlayVisibility(FName OverlayID, bool bVisible);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void SetOverlayMaterial(FName OverlayID, UMaterialInterface* Material);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void EnableOcclusion(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool IsOcclusionEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    FARLightEstimate GetCurrentLightEstimate() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void SetAREnvironmentRendering(bool bEnableEnvironment);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool IsEnvironmentRenderingEnabled() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void GenerateEnvironmentMesh();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool HitTestAtScreenPosition(const FVector2D& ScreenPosition, FARAnchorData& OutHitResult);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool AddManualAnchor(const FTransform& WorldTransform, EARAnchorType Type);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void RemoveAnchor(FName AnchorID);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void LoadReferenceImages(const TArray<FString>& ImagePaths);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void SaveARWorldMap(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool LoadARWorldMap(const FString& FilePath);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void SetContentScale(float Scale);

    UFUNCTION(BlueprintPure, Category = "AR Content")
    float GetContentScale() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void EnableCollaborativeSession(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool IsCollaborativeSessionEnabled() const;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARSessionStarted OnARSessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARSessionStopped OnARSessionStopped;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARPlaneDetected OnARPlaneDetected;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARImageTracked OnARImageTracked;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARAnchorAdded OnARAnchorAdded;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARAnchorUpdated OnARAnchorUpdated;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARAnchorRemoved OnARAnchorRemoved;

    UPROPERTY(BlueprintAssignable, Category = "AR Events")
    FOnARLightEstimateUpdated OnARLightEstimateUpdated;

protected:
    UPROPERTY()
    FAROverlayConfig Config;

    UPROPERTY()
    bool bARSessionRunning;

    UPROPERTY()
    EARDeviceType CurrentDevice;

    UPROPERTY()
    EARTrackingState TrackingState;

    UPROPERTY()
    TArray<FARAnchorData> Anchors;

    UPROPERTY()
    TMap<FName, FARContentOverlay> ContentOverlays;

    UPROPERTY()
    FARLightEstimate CurrentLightEstimate;

    UPROPERTY()
    float ContentScale;

    UPROPERTY()
    bool bOcclusionEnabled;

    UPROPERTY()
    bool bCollaborativeSession;

    UPROPERTY()
    FTimerHandle ARUpdateTimer;

    void StartARUpdate();
    void StopARUpdate();
    void UpdateARFrame();
    void ProcessARAnchors();
    void ProcessARImages();
    void UpdateEnvironmentLighting();
    void GenerateMeshFromAnchors();
    bool InitializeARKit();
    bool InitializeARCore();
    bool InitializeHoloLens();
    void ProcessHitResult(const FARAnchorData& HitResult);
    static UMingARContentOverlay* Get(UObject* WorldContextObject);
};
