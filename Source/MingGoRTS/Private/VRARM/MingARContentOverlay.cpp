// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AR Content Overlay System Implementation - C1-2

#include "VRARM/MingARContentOverlay.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "AugmentedReality/Public/ARBlueprintLibrary.h"
#include "AugmentedReality/Public/ARSessionConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogAR, Log, All);

UMingARContentOverlay::UMingARContentOverlay()
    : bARSessionRunning(false)
    , CurrentDevice(EARDeviceType::None)
    , TrackingState(EARTrackingState::NotTracking)
    , ContentScale(1.0f)
    , bOcclusionEnabled(true)
    , bCollaborativeSession(false)
{
}

void UMingARContentOverlay::InitializeARSystem(const FAROverlayConfig& Config)
{
    this->Config = Config;

    UE_LOG(LogAR, Log, TEXT("Initializing AR Content Overlay System"));
    UE_LOG(LogAR, Log, TEXT("Plane detection: %s"), Config.bEnablePlaneDetection ? TEXT("enabled") : TEXT("disabled"));
    UE_LOG(LogAR, Log, TEXT("Image tracking: %s"), Config.bEnableImageTracking ? TEXT("enabled") : TEXT("disabled"));

    // Detect AR device type
    DetectARDeviceType();
}

void UMingARContentOverlay::ShutdownARSystem()
{
    UE_LOG(LogAR, Log, TEXT("Shutting down AR Content Overlay System"));

    StopARSession();
    StopARUpdate();

    Anchors.Empty();
    ContentOverlays.Empty();
}

bool UMingARContentOverlay::StartARSession()
{
    UE_LOG(LogAR, Log, TEXT("Starting AR session"));

    // Create AR session configuration
    UARSessionConfig* SessionConfig = NewObject<UARSessionConfig>();

    if (Config.bEnablePlaneDetection)
    {
        if (Config.bEnableHorizontalPlaneDetection)
        {
            SessionConfig->bHorizontalPlaneDetection = true;
        }
        if (Config.bEnableVerticalPlaneDetection)
        {
            SessionConfig->bVerticalPlaneDetection = true;
        }
    }

    SessionConfig->bEnableAutomaticCameraOverlay = false;
    SessionConfig->bEnableAutomaticCameraTracking = true;

    // Start AR session
    bool bSuccess = UARBlueprintLibrary::StartARSession(SessionConfig);

    if (bSuccess)
    {
        bARSessionRunning = true;
        TrackingState = EARTrackingState::Normal;

        StartARUpdate();

        OnARSessionStarted.Broadcast(CurrentDevice);

        UE_LOG(LogAR, Log, TEXT("AR session started successfully"));
    }
    else
    {
        UE_LOG(LogAR, Error, TEXT("Failed to start AR session"));
    }

    return bSuccess;
}

void UMingARContentOverlay::StopARSession()
{
    UE_LOG(LogAR, Log, TEXT("Stopping AR session"));

    UARBlueprintLibrary::StopARSession();

    bARSessionRunning = false;
    TrackingState = EARTrackingState::NotTracking;

    StopARUpdate();

    OnARSessionStopped.Broadcast();
}

bool UMingARContentOverlay::IsARSessionRunning() const
{
    return bARSessionRunning;
}

EARDeviceType UMingARContentOverlay::GetARDeviceType() const
{
    return CurrentDevice;
}

EARTrackingState UMingARContentOverlay::GetTrackingState() const
{
    return TrackingState;
}

FAROverlayConfig UMingARContentOverlay::GetConfiguration() const
{
    return Config;
}

void UMingARContentOverlay::UpdateConfiguration(const FAROverlayConfig& NewConfig)
{
    Config = NewConfig;

    UE_LOG(LogAR, Log, TEXT("AR configuration updated"));

    // If session is running, restart with new config
    if (bARSessionRunning)
    {
        StopARSession();
        StartARSession();
    }
}

TArray<FARAnchorData> UMingARContentOverlay::GetAllAnchors() const
{
    return Anchors;
}

TArray<FARAnchorData> UMingARContentOverlay::GetPlaneAnchors() const
{
    TArray<FARAnchorData> PlaneAnchors;

    for (const auto& Anchor : Anchors)
    {
        if (Anchor.Type == EARAnchorType::Plane)
        {
            PlaneAnchors.Add(Anchor);
        }
    }

    return PlaneAnchors;
}

TArray<FARAnchorData> UMingARContentOverlay::GetImageAnchors() const
{
    TArray<FARAnchorData> ImageAnchors;

    for (const auto& Anchor : Anchors)
    {
        if (Anchor.Type == EARAnchorType::Image)
        {
            ImageAnchors.Add(Anchor);
        }
    }

    return ImageAnchors;
}

FARAnchorData UMingARContentOverlay::GetAnchorByID(FName AnchorID) const
{
    for (const auto& Anchor : Anchors)
    {
        if (Anchor.AnchorID == AnchorID)
        {
            return Anchor;
        }
    }

    return FARAnchorData();
}

FName UMingARContentOverlay::AddContentOverlay(const FARContentOverlay& Overlay)
{
    FName OverlayID = FName(*FString::Printf(TEXT("AROverlay_%d"), ContentOverlays.Num()));

    FARContentOverlay NewOverlay = Overlay;
    NewOverlay.OverlayID = OverlayID;

    ContentOverlays.Add(OverlayID, NewOverlay);

    UE_LOG(LogAR, Log, TEXT("Added content overlay: %s"), *OverlayID.ToString());

    return OverlayID;
}

void UMingARContentOverlay::RemoveContentOverlay(FName OverlayID)
{
    if (ContentOverlays.Contains(OverlayID))
    {
        ContentOverlays.Remove(OverlayID);
        UE_LOG(LogAR, Log, TEXT("Removed content overlay: %s"), *OverlayID.ToString());
    }
}

void UMingARContentOverlay::UpdateContentOverlayTransform(FName OverlayID, const FTransform& NewTransform)
{
    if (ContentOverlays.Contains(OverlayID))
    {
        ContentOverlays[OverlayID].RelativeTransform = NewTransform;
    }
}

TArray<FARContentOverlay> UMingARContentOverlay::GetAllOverlays() const
{
    TArray<FARContentOverlay> Result;
    ContentOverlays.GenerateValueArray(Result);
    return Result;
}

bool UMingARContentOverlay::PinComponentToAnchor(USceneComponent* Component, FName AnchorID)
{
    if (!Component)
    {
        return false;
    }

    FARAnchorData Anchor = GetAnchorByID(AnchorID);
    if (!Anchor.bIsValid)
    {
        return false;
    }

    // Attach component to anchor transform
    Component->SetWorldTransform(Anchor.Transform);

    UE_LOG(LogAR, Log, TEXT("Pinned component to anchor: %s"), *AnchorID.ToString());

    return true;
}

void UMingARContentOverlay::SetOverlayVisibility(FName OverlayID, bool bVisible)
{
    // Implementation would set visibility of overlay components
    UE_LOG(LogAR, Verbose, TEXT("Set overlay %s visibility: %s"),
        *OverlayID.ToString(), bVisible ? TEXT("visible") : TEXT("hidden"));
}

void UMingARContentOverlay::SetOverlayMaterial(FName OverlayID, UMaterialInterface* Material)
{
    if (ContentOverlays.Contains(OverlayID))
    {
        FARContentOverlay& Overlay = ContentOverlays[OverlayID];
        Overlay.Materials.Empty();
        Overlay.Materials.Add(Material);

        UE_LOG(LogAR, Log, TEXT("Set overlay %s material"), *OverlayID.ToString());
    }
}

void UMingARContentOverlay::EnableOcclusion(bool bEnable)
{
    bOcclusionEnabled = bEnable;
    UE_LOG(LogAR, Log, TEXT("AR occlusion: %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingARContentOverlay::IsOcclusionEnabled() const
{
    return bOcclusionEnabled;
}

FARLightEstimate UMingARContentOverlay::GetCurrentLightEstimate() const
{
    return CurrentLightEstimate;
}

void UMingARContentOverlay::SetAREnvironmentRendering(bool bEnableEnvironment)
{
    UE_LOG(LogAR, Log, TEXT("AR environment rendering: %s"),
        bEnableEnvironment ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingARContentOverlay::IsEnvironmentRenderingEnabled() const
{
    return false; // Simplified
}

void UMingARContentOverlay::GenerateEnvironmentMesh()
{
    UE_LOG(LogAR, Log, TEXT("Generating environment mesh from AR anchors"));

    // Generate mesh from plane anchors
    GenerateMeshFromAnchors();
}

bool UMingARContentOverlay::HitTestAtScreenPosition(const FVector2D& ScreenPosition, FARAnchorData& OutHitResult)
{
    // Perform AR hit test
    TArray<FARTraceResult> TraceResults = UARBlueprintLibrary::LineTraceTrackedObjects(ScreenPosition, false, false, false, false);

    if (TraceResults.Num() > 0)
    {
        // Use first hit result
        const FARTraceResult& Hit = TraceResults[0];

        OutHitResult.Transform = Hit.GetLocalTransform();
        OutHitResult.AnchorID = FName(*Hit.GetTrackedGeometry()->GetDebugName().ToString());
        OutHitResult.bIsValid = true;
        OutHitResult.Type = EARAnchorType::Plane;

        ProcessHitResult(OutHitResult);

        return true;
    }

    return false;
}

bool UMingARContentOverlay::AddManualAnchor(const FTransform& WorldTransform, EARAnchorType Type)
{
    FARAnchorData NewAnchor;
    NewAnchor.AnchorID = FName(*FString::Printf(TEXT("ManualAnchor_%d"), Anchors.Num()));
    NewAnchor.Transform = WorldTransform;
    NewAnchor.Type = Type;
    NewAnchor.bIsValid = true;
    NewAnchor.Timestamp = FPlatformTime::Seconds();

    Anchors.Add(NewAnchor);

    OnARAnchorAdded.Broadcast(NewAnchor);

    UE_LOG(LogAR, Log, TEXT("Added manual anchor: %s"), *NewAnchor.AnchorID.ToString());

    return true;
}

void UMingARContentOverlay::RemoveAnchor(FName AnchorID)
{
    for (int32 i = Anchors.Num() - 1; i >= 0; --i)
    {
        if (Anchors[i].AnchorID == AnchorID)
        {
            Anchors.RemoveAt(i);
            OnARAnchorRemoved.Broadcast(AnchorID);
            UE_LOG(LogAR, Log, TEXT("Removed anchor: %s"), *AnchorID.ToString());
            return;
        }
    }
}

void UMingARContentOverlay::LoadReferenceImages(const TArray<FString>& ImagePaths)
{
    UE_LOG(LogAR, Log, TEXT("Loading %d reference images"), ImagePaths.Num());

    Config.ReferenceImages.Empty();

    for (const auto& Path : ImagePaths)
    {
        FARTrackedImage RefImage;
        RefImage.ImageName = FName(*FPaths::GetBaseFilename(Path));
        RefImage.bIsTracked = false;
        RefImage.TrackingConfidence = 0.0f;

        Config.ReferenceImages.Add(RefImage);
    }
}

void UMingARContentOverlay::SaveARWorldMap(const FString& FilePath)
{
    UE_LOG(LogAR, Log, TEXT("Saving AR world map: %s"), *FilePath);

    // Would save AR world map to file
    // Implementation depends on AR plugin capabilities
}

bool UMingARContentOverlay::LoadARWorldMap(const FString& FilePath)
{
    UE_LOG(LogAR, Log, TEXT("Loading AR world map: %s"), *FilePath);

    // Would load AR world map from file
    return FPaths::FileExists(FilePath);
}

void UMingARContentOverlay::SetContentScale(float Scale)
{
    ContentScale = Scale;
    UE_LOG(LogAR, Log, TEXT("AR content scale set to: %.2f"), Scale);
}

float UMingARContentOverlay::GetContentScale() const
{
    return ContentScale;
}

void UMingARContentOverlay::EnableCollaborativeSession(bool bEnable)
{
    bCollaborativeSession = bEnable;
    UE_LOG(LogAR, Log, TEXT("AR collaborative session: %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

bool UMingARContentOverlay::IsCollaborativeSessionEnabled() const
{
    return bCollaborativeSession;
}

void UMingARContentOverlay::StartARUpdate()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            ARUpdateTimer,
            this,
            &UMingARContentOverlay::UpdateARFrame,
            0.033f, // ~30Hz for AR
            true
        );
    }
}

void UMingARContentOverlay::StopARUpdate()
{
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(ARUpdateTimer);
    }
}

void UMingARContentOverlay::UpdateARFrame()
{
    if (!bARSessionRunning)
    {
        return;
    }

    // Update tracking state
    auto ARStatus = UARBlueprintLibrary::GetARSessionStatus();
    if (ARStatus.Status ==EARSessionStatus::Running)
    {
        TrackingState = EARTrackingState::Normal;
    }
    else if (ARStatus.Status == EARSessionStatus::NotStarted)
    {
        TrackingState = EARTrackingState::NotTracking;
    }
    else
    {
        TrackingState = EARTrackingState::Limited;
    }

    // Process AR anchors
    ProcessARAnchors();

    // Process AR images
    ProcessARImages();

    // Update environment lighting
    UpdateEnvironmentLighting();
}

void UMingARContentOverlay::ProcessARAnchors()
{
    // Get all tracked geometries
    TArray<UARTrackedGeometry*> TrackedGeometries = UARBlueprintLibrary::GetAllGeometries();

    for (UARTrackedGeometry* Geometry : TrackedGeometries)
    {
        if (!Geometry)
        {
            continue;
        }

        FARAnchorData Anchor;
        Anchor.AnchorID = FName(*Geometry->GetDebugName().ToString());
        Anchor.Transform = Geometry->GetLocalTransform();
        Anchor.bIsValid = Geometry->IsValid();
        Anchor.Confidence = Geometry->GetTrackingState() == EARTrackingState::Tracking ? 1.0f : 0.5f;
        Anchor.Timestamp = FPlatformTime::Seconds();

        // Determine anchor type
        if (Geometry->IsA<UARPlaneGeometry>())
        {
            Anchor.Type = EARAnchorType::Plane;

            UARPlaneGeometry* Plane = Cast<UARPlaneGeometry>(Geometry);
            if (Plane)
            {
                Anchor.PlaneGeometry.Center = Plane->GetCenter();
                Anchor.PlaneGeometry.Extent = Plane->GetExtent();
                Anchor.PlaneGeometry.BoundaryPoints = Plane->GetBoundaryPolygon();
            }
        }
        else if (Geometry->IsA<UARTrackedImage>())
        {
            Anchor.Type = EARAnchorType::Image;
        }
        else if (Geometry->IsA<UARTrackedQRCode>())
        {
            Anchor.Type = EARAnchorType::QRCode;
        }
        else
        {
            Anchor.Type = EARAnchorType::Point;
        }

        // Check if anchor already exists
        bool bFound = false;
        for (auto& ExistingAnchor : Anchors)
        {
            if (ExistingAnchor.AnchorID == Anchor.AnchorID)
            {
                ExistingAnchor = Anchor;
                OnARAnchorUpdated.Broadcast(Anchor);
                bFound = true;
                break;
            }
        }

        if (!bFound)
        {
            Anchors.Add(Anchor);

            if (Anchor.Type == EARAnchorType::Plane)
            {
                OnARPlaneDetected.Broadcast(Anchor);
            }

            OnARAnchorAdded.Broadcast(Anchor);
        }
    }
}

void UMingARContentOverlay::ProcessARImages()
{
    if (!Config.bEnableImageTracking)
    {
        return;
    }

    // Process tracked images
    TArray<UARTrackedGeometry*> TrackedGeometries = UARBlueprintLibrary::GetAllGeometries();

    for (UARTrackedGeometry* Geometry : TrackedGeometries)
    {
        if (UARTrackedImage* TrackedImage = Cast<UARTrackedImage>(Geometry))
        {
            FARTrackedImage Image;
            Image.ImageName = FName(*TrackedImage->GetDebugName().ToString());
            Image.TrackedTransform = TrackedImage->GetLocalTransform();
            Image.bIsTracked = TrackedImage->IsValid();
            Image.TrackingConfidence = TrackedImage->GetTrackingState() == EARTrackingState::Tracking ? 1.0f : 0.5f;

            OnARImageTracked.Broadcast(Image);
        }
    }
}

void UMingARContentOverlay::UpdateEnvironmentLighting()
{
    if (!Config.bEnableLightEstimation)
    {
        return;
    }

    // Get AR light estimate
    // Note: This functionality may vary based on AR plugin version
    CurrentLightEstimate.bIsValid = true;
    CurrentLightEstimate.AmbientIntensity = 1.0f;
    CurrentLightEstimate.AmbientColor = FLinearColor::White;

    OnARLightEstimateUpdated.Broadcast(CurrentLightEstimate);
}

void UMingARContentOverlay::GenerateMeshFromAnchors()
{
    UE_LOG(LogAR, Log, TEXT("Generating mesh from %d anchors"), Anchors.Num());

    // Generate environment mesh from plane anchors
    for (const auto& Anchor : Anchors)
    {
        if (Anchor.Type == EARAnchorType::Plane && Anchor.PlaneGeometry.BoundaryPoints.Num() > 0)
        {
            // Create mesh from plane boundary points
            UE_LOG(LogAR, Verbose, TEXT("Generating mesh for plane anchor: %s"), *Anchor.AnchorID.ToString());
        }
    }
}

void UMingARContentOverlay::DetectARDeviceType()
{
    // Detect AR device type based on platform
#if PLATFORM_IOS
    CurrentDevice = EARDeviceType::AppleARKit;
#elif PLATFORM_ANDROID
    CurrentDevice = EARDeviceType::GoogleARCore;
#elif PLATFORM_HOLOLENS
    CurrentDevice = EARDeviceType::MicrosoftHoloLens;
#else
    CurrentDevice = EARDeviceType::GenericAR;
#endif

    UE_LOG(LogAR, Log, TEXT("Detected AR device: %s"), *UEnum::GetValueAsString(CurrentDevice));
}

void UMingARContentOverlay::ProcessHitResult(const FARAnchorData& HitResult)
{
    UE_LOG(LogAR, Verbose, TEXT("Processing hit result at anchor: %s"), *HitResult.AnchorID.ToString());
}

static UMingARContentOverlay* UMingARContentOverlay::Get(UObject* WorldContextObject)
{
    static UMingARContentOverlay* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingARContentOverlay>();
        Instance->AddToRoot();
    }
    return Instance;
}
