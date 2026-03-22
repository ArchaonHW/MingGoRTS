#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingVRTypes.h"
#include "MingVRManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceConnected, const FMingVRDeviceInfo&, DeviceInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceDisconnected, const FString&, DeviceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRModeChanged, EMingVRMode, OldMode, EMingVRMode, NewMode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRRoomScaleSetup, const FMingVRRoomBounds&, RoomBounds);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnVRInteraction, EMingVRInteractionType, InteractionType, const FMingVRInteractionData&, InteractionData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRComfortModeChanged, EMingVRComfortMode, NewMode);

UCLASS(ClassGroup = (VR, Immersive))
class MINGMULTIPLAYER_API UMingVRManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingVRManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // VR System Management
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool InitializeVRSystem();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void ShutdownVRSystem();

    UFUNCTION(BlueprintPure, Category = "VR")
    bool IsVREnabled() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    bool IsVRDeviceAvailable() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    TArray<FMingVRDeviceInfo> GetConnectedVRDevices() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FMingVRDeviceInfo GetPrimaryVRDevice() const;

    // VR Mode Management
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool SetVRMode(EMingVRMode VRMode);

    UFUNCTION(BlueprintPure, Category = "VR")
    EMingVRMode GetCurrentVRMode() const;

    UFUNCTION(BlueprintCallable, Category = "VR")
    bool EnableRoomScale();

    UFUNCTION(BlueprintCallable, Category = "VR")
    bool EnableSeatedMode();

    UFUNCTION(BlueprintCallable, Category = "VR")
    bool EnableStandingMode();

    // Room Scale Setup
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool SetupRoomScale();

    UFUNCTION(BlueprintCallable, Category = "VR")
    bool CalibratePlayArea();

    UFUNCTION(BlueprintPure, Category = "VR")
    FMingVRRoomBounds GetRoomBounds() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FVector GetPlayAreaCenter() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FVector GetPlayAreaSize() const;

    // VR Camera and View
    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetVRCamera(class APlayerCameraManager* CameraManager);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void UpdateVRView();

    UFUNCTION(BlueprintPure, Category = "VR")
    FVector GetVRHeadPosition() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FRotator GetVRHeadRotation() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FTransform GetVRHeadTransform() const;

    // VR Controllers
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool InitializeVRControllers();

    UFUNCTION(BlueprintPure, Category = "VR")
    TArray<FMingVRControllerInfo> GetVRControllers() const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FMingVRControllerInfo GetVRController(EVRHand Hand) const;

    UFUNCTION(BlueprintPure, Category = "VR")
    bool IsVRControllerConnected(EVRHand Hand) const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FVector GetControllerPosition(EVRHand Hand) const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FRotator GetControllerRotation(EVRHand Hand) const;

    UFUNCTION(BlueprintPure, Category = "VR")
    FTransform GetControllerTransform(EVRHand Hand) const;

    // VR Interaction
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool EnableVRInteraction();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void ProcessVRInteraction();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetHapticFeedback(EVRHand Hand, float Intensity, float Duration);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void TriggerHapticPulse(EVRHand Hand, float Strength);

    // VR UI
    UFUNCTION(BlueprintCallable, Category = "VR")
    bool InitializeVRUI();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void ShowVRUI(class UUserWidget* Widget);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void HideVRUI();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetVRUIScale(float Scale);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void AttachUIToController(EVRHand Hand, class UUserWidget* Widget);

    // Comfort Settings
    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetComfortMode(EMingVRComfortMode ComfortMode);

    UFUNCTION(BlueprintPure, Category = "VR")
    EMingVRComfortMode GetComfortMode() const;

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetSnapTurning(bool bEnabled, float SnapAngle = 45.0f);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetVignetteStrength(float Strength);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetMovementSpeed(float Speed);

    // Performance
    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetVRRenderSettings(const FMingVRRenderSettings& Settings);

    UFUNCTION(BlueprintPure, Category = "VR")
    FMingVRRenderSettings GetVRRenderSettings() const;

    UFUNCTION(BlueprintCallable, Category = "VR")
    void EnableDynamicResolution(bool bEnabled);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetTargetFrameRate(int32 TargetFPS);

    UFUNCTION(BlueprintPure, Category = "VR")
    float GetVRFrameRate() const;

    // Game Integration
    UFUNCTION(BlueprintCallable, Category = "VR")
    void IntegrateWithGameMode(class AGameModeBase* GameMode);

    UFUNCTION(BlueprintCallable, Category = "VR")
    void SetupRTSInterface();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void EnableRTSControls();

    UFUNCTION(BlueprintCallable, Category = "VR")
    void UpdateRTSInterface();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRDeviceConnected OnVRDeviceConnected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRDeviceDisconnected OnVRDeviceDisconnected;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRModeChanged OnVRModeChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRRoomScaleSetup OnVRRoomScaleSetup;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRInteraction OnVRInteraction;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnVRComfortModeChanged OnVRComfortModeChanged;

protected:
    UPROPERTY()
    bool bVREnabled;

    UPROPERTY()
    EMingVRMode CurrentVRMode;

    UPROPERTY()
    EMingVRComfortMode ComfortMode;

    UPROPERTY()
    TArray<FMingVRDeviceInfo> ConnectedDevices;

    UPROPERTY()
    FMingVRRoomBounds RoomBounds;

    UPROPERTY()
    TArray<FMingVRControllerInfo> VRControllers;

    UPROPERTY()
    FMingVRRenderSettings RenderSettings;

    UPROPERTY()
    class APlayerCameraManager* VRCameraManager;

    UPROPERTY()
    bool bRoomScaleEnabled;

    UPROPERTY()
    bool bSnapTurningEnabled;

    UPROPERTY()
    float SnapTurnAngle;

    UPROPERTY()
    float VignetteStrength;

    UPROPERTY()
    float MovementSpeed;

    UPROPERTY()
    bool bDynamicResolutionEnabled;

    UPROPERTY()
    int32 TargetFrameRate;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    float UpdateInterval;

    // Internal Functions
    void UpdateVRDevices(float DeltaTime);
    void UpdateVRControllers(float DeltaTime);
    void UpdateVRInteraction(float DeltaTime);
    void UpdateVRUI(float DeltaTime);
    void UpdateComfortSettings(float DeltaTime);
    void UpdatePerformance(float DeltaTime);
    void ProcessHapticFeedback();
    void HandleVRInput();
    void UpdateRTSInterface(float DeltaTime);
    void ValidateVRSystem();

    // Helper Functions
    bool InitializeOpenVR();
    bool InitializeOculus();
    bool InitializeSteamVR();
    void SetupVRRendering();
    void ConfigureVRInput();
    void InitializeHaptics();
    void SetupVRPhysics();
    void CalibrateTrackingSpace();
    void UpdateCameraTransform();
    void ProcessControllerInput();
    void HandleGestureRecognition();

private:
    bool ValidateVRDevice(const FMingVRDeviceInfo& DeviceInfo) const;
    FString GenerateDeviceID() const;
    void CleanupVRResources();
    void ResetVRSystem();
};
