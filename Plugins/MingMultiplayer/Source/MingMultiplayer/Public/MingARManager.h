#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingARTypes.h"
#include "MingARManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARSessionStarted, const FMingARSessionInfo&, SessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARSessionEnded, const FString&, SessionID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnARPlaneDetected, const FMingARPlane&, Plane, bool bIsNew);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnARAnchorPlaced, const FString&, AnchorID, const FVector&, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnARTrackingStateChanged, EMingARTrackingState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnARInteraction, EMingARInteractionType, InteractionType, const FMingARInteractionData&, InteractionData);

UCLASS(ClassGroup = (AR, Immersive))
class MINGMULTIPLAYER_API UMingARManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingARManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // AR System Management
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool InitializeARSystem();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ShutdownARSystem();

    UFUNCTION(BlueprintPure, Category = "AR")
    bool IsAREnabled() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    bool IsARSupported() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    TArray<FMingARDeviceInfo> GetSupportedARDevices() const;

    // AR Session Management
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool StartARSession(const FMingARSessionConfig& SessionConfig);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void StopARSession();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void PauseARSession();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ResumeARSession();

    UFUNCTION(BlueprintPure, Category = "AR")
    bool IsARSessionActive() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    FMingARSessionInfo GetCurrentSessionInfo() const;

    // AR World Tracking
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableWorldTracking();

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnablePlaneDetection();

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableFaceTracking();

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableImageTracking();

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableObjectTracking();

    UFUNCTION(BlueprintPure, Category = "AR")
    EMingARTrackingState GetTrackingState() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    TArray<FMingARPlane> GetDetectedPlanes() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    TArray<FMingARAnchor> GetPlacedAnchors() const;

    // AR Plane Management
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool CreatePlaneAnchor(const FVector& Location, const FVector& Normal);

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool RemoveAnchor(const FString& AnchorID);

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool UpdateAnchorTransform(const FString& AnchorID, const FTransform& Transform);

    UFUNCTION(BlueprintPure, Category = "AR")
    FMingARAnchor GetAnchor(const FString& AnchorID) const;

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ClearAllAnchors();

    // AR Content Placement
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool PlaceARContent(class AActor* Content, const FVector& WorldLocation);

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool PlaceARContentAtAnchor(class AActor* Content, const FString& AnchorID);

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool AttachToARPlane(class AActor* Content, const FString& PlaneID);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void RemoveARContent(class AActor* Content);

    UFUNCTION(BlueprintPure, Category = "AR")
    TArray<class AActor*> GetPlacedARContent() const;

    // AR Camera and View
    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetARCamera(class APlayerCameraManager* CameraManager);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void UpdateARCamera();

    UFUNCTION(BlueprintPure, Category = "AR")
    FVector GetARCameraPosition() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    FRotator GetARCameraRotation() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    FTransform GetARCameraTransform() const;

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetARCameraCullingDistance(float Distance);

    // AR Interaction
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableARInteraction();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ProcessARTouch();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ProcessARGesture();

    UFUNCTION(BlueprintPure, Category = "AR")
    FVector GetTouchLocationInAR() const;

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool RaycastToARPlane(const FVector2D& ScreenPosition, FVector& HitLocation, FVector& HitNormal);

    UFUNCTION(BlueprintCallable, Category = "AR")
    bool RaycastToARContent(const FVector2D& ScreenPosition, class AActor*& HitContent);

    // AR Lighting and Environment
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableAREnvironmentProbe();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void UpdateAREnvironment();

    UFUNCTION(BlueprintPure, Category = "AR")
    FMingAREnvironmentInfo GetEnvironmentInfo() const;

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetAREnvironmentIntensity(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetARColorCorrection(const FLinearColor& ColorCorrection);

    // AR Occlusion
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableAROcclusion();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetAROcclusionMode(EMingAROcclusionMode OcclusionMode);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void UpdateAROcclusion();

    // AR Performance
    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetARRenderSettings(const FMingARRenderSettings& Settings);

    UFUNCTION(BlueprintPure, Category = "AR")
    FMingARRenderSettings GetARRenderSettings() const;

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetARQualityLevel(EMingARQualityLevel QualityLevel);

    UFUNCTION(BlueprintPure, Category = "AR")
    EMingARQualityLevel GetARQualityLevel() const;

    UFUNCTION(BlueprintPure, Category = "AR")
    float GetARFrameRate() const;

    // Game Integration
    UFUNCTION(BlueprintCallable, Category = "AR")
    void IntegrateWithGameMode(class AGameModeBase* GameMode);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void SetupRTSARInterface();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void EnableRTSARControls();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void UpdateRTSARInterface();

    // Educational Features
    UFUNCTION(BlueprintCallable, Category = "AR")
    bool EnableAREducationalMode();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void ShowARHistoricalInfo(const FString& LocationID);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void PlaceARHistoricalMarker(const FVector& Location, const FString& HistoricalInfo);

    UFUNCTION(BlueprintCallable, Category = "AR")
    void StartARHistoricalTour();

    UFUNCTION(BlueprintCallable, Category = "AR")
    void StopARHistoricalTour();

    UFUNCTION(BlueprintPure, Category = "AR")
    TArray<FMingARHistoricalMarker> GetHistoricalMarkers() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARSessionStarted OnARSessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARSessionEnded OnARSessionEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARPlaneDetected OnARPlaneDetected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARAnchorPlaced OnARAnchorPlaced;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARTrackingStateChanged OnARTrackingStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnARInteraction OnARInteraction;

protected:
    UPROPERTY()
    bool bAREnabled;

    UPROPERTY()
    bool bARSessionActive;

    UPROPERTY()
    EMingARTrackingState TrackingState;

    UPROPERTY()
    FMingARSessionInfo CurrentSession;

    UPROPERTY()
    TArray<FMingARPlane> DetectedPlanes;

    UPROPERTY()
    TArray<FMingARAnchor> PlacedAnchors;

    UPROPERTY()
    TArray<class AActor*> ARContent;

    UPROPERTY()
    class APlayerCameraManager* ARCameraManager;

    UPROPERTY()
    FMingARRenderSettings RenderSettings;

    UPROPERTY()
    FMingAREnvironmentInfo EnvironmentInfo;

    UPROPERTY()
    EMingARQualityLevel QualityLevel;

    UPROPERTY()
    EMingAROcclusionMode OcclusionMode;

    UPROPERTY()
    bool bAREducationalMode;

    UPROPERTY()
    TArray<FMingARHistoricalMarker> HistoricalMarkers;

    UPROPERTY()
    bool bHistoricalTourActive;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    float UpdateInterval;

    // Internal Functions
    void UpdateARDevices(float DeltaTime);
    void UpdateARTracking(float DeltaTime);
    void UpdateARPlanes(float DeltaTime);
    void UpdateARAnchors(float DeltaTime);
    void UpdateARInteraction(float DeltaTime);
    void UpdateAREnvironment(float DeltaTime);
    void UpdateARPerformance(float DeltaTime);
    void ProcessARTouchInput();
    void ProcessARGestureInput();
    void UpdateARRendering();
    void ValidateARSystem();

    // Helper Functions
    bool InitializeARKit();
    bool InitializeARCore();
    bool InitializeARFoundation();
    void SetupARRendering();
    void ConfigureARInput();
    void InitializeARTracking();
    void SetupARLighting();
    void CalibrateARSpace();
    void UpdateCameraTransform();
    void ProcessARContentPlacement();
    void HandleARContentInteraction();

private:
    bool ValidateARDevice(const FMingARDeviceInfo& DeviceInfo) const;
    FString GenerateAnchorID() const;
    void CleanupARResources();
    void ResetARSystem();
    void UpdateHistoricalTour(float DeltaTime);
};
