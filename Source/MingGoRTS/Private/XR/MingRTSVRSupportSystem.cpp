#include "XR/MingRTSVRSupportSystem.h"
#include "Misc/DateTime.h"

UMingRTSVRSupportSystem::UMingRTSVRSupportSystem()
    : SystemStatus(EXRSystemStatus::NotAvailable)
    , bXREnabled(false)
    , bVRMode(true)
    , ConnectedVRDevice(EVRDeviceType::None)
    , ConnectedARDevice(EARDeviceType::None)
    , bAdaptiveQualityEnabled(true)
    , FixedFoveatedLevel(2)
    , ResolutionScale(1.0f)
{
}

void UMingRTSVRSupportSystem::InitializeXRSupport()
{
    LoadDefaultSettings();
    
    // 檢查VR/AR可用性
    #if WITH_EDITOR
        SystemStatus = EXRSystemStatus::Available;
    #else
        // 實際平台檢測
        SystemStatus = EXRSystemStatus::Available;
    #endif
    
    UE_LOG(LogTemp, Log, TEXT("XR Support System Initialized. Status: %d"), (int32)SystemStatus);
}

bool UMingRTSVRSupportSystem::IsVRAvailable() const
{
    return SystemStatus == EXRSystemStatus::Available || 
           SystemStatus == EXRSystemStatus::Ready ||
           SystemStatus == EXRSystemStatus::Running;
}

bool UMingRTSVRSupportSystem::StartVRMode()
{
    if (!IsVRAvailable())
    {
        UE_LOG(LogTemp, Warning, TEXT("VR is not available"));
        return false;
    }
    
    SystemStatus = EXRSystemStatus::Initializing;
    
    InitializeVR();
    
    bVRMode = true;
    bXREnabled = true;
    SystemStatus = EXRSystemStatus::Running;
    
    UE_LOG(LogTemp, Log, TEXT("VR mode started"));
    return true;
}

void UMingRTSVRSupportSystem::StopVRMode()
{
    ShutdownVR();
    
    bXREnabled = false;
    SystemStatus = EXRSystemStatus::Ready;
    
    UE_LOG(LogTemp, Log, TEXT("VR mode stopped"));
}

EVRDeviceType UMingRTSVRSupportSystem::GetConnectedVRDevice() const
{
    return ConnectedVRDevice;
}

FVRTrackingData UMingRTSVRSupportSystem::GetHeadTrackingData() const
{
    FVRTrackingData Data;
    
    if (SystemStatus == EXRSystemStatus::Running)
    {
        // 模擬頭部追踪數據
        Data.Position = FVector(0.0f, 0.0f, 170.0f); // 玩家身高
        Data.Rotation = FQuat::Identity;
        Data.bIsTracked = true;
        Data.TrackingConfidence = 1.0f;
    }
    
    return Data;
}

FVRTrackingData UMingRTSVRSupportSystem::GetHandTrackingData(bool bLeftHand) const
{
    FVRTrackingData Data;
    
    if (SystemStatus == EXRSystemStatus::Running)
    {
        // 模擬手部追踪數據
        if (bLeftHand)
        {
            Data.Position = FVector(50.0f, -30.0f, 120.0f);
        }
        else
        {
            Data.Position = FVector(50.0f, 30.0f, 120.0f);
        }
        Data.Rotation = FQuat::Identity;
        Data.bIsTracked = true;
        Data.TrackingConfidence = 0.95f;
    }
    
    return Data;
}

FVRBoundaryData UMingRTSVRSupportSystem::GetBoundaryData() const
{
    return BoundaryData;
}

bool UMingRTSVRSupportSystem::IsInsideBoundary(const FVector& Location) const
{
    if (!BoundaryData.bIsValid)
    {
        return true; // 沒有邊界時默認在範圍內
    }
    
    // 簡化的邊界檢查
    FVector LocalLocation = Location - BoundaryData.Center;
    return LocalLocation.Size2D() <= BoundaryData.PlayAreaSize;
}

void UMingRTSVRSupportSystem::SetVRSettings(const FVRSettings& Settings)
{
    if (ValidateVRSettings(Settings))
    {
        CurrentVRSettings = Settings;
        UE_LOG(LogTemp, Log, TEXT("VR settings updated"));
    }
}

FVRSettings UMingRTSVRSupportSystem::GetVRSettings() const
{
    return CurrentVRSettings;
}

void UMingRTSVRSupportSystem::CalibrateVR()
{
    SystemStatus = EXRSystemStatus::Calibrating;
    
    UE_LOG(LogTemp, Log, TEXT("VR calibration started..."));
    
    // 模擬校準過程
    SystemStatus = EXRSystemStatus::Running;
    
    UE_LOG(LogTemp, Log, TEXT("VR calibration completed"));
}

void UMingRTSVRSupportSystem::RecenterVR()
{
    UE_LOG(LogTemp, Log, TEXT("VR recentered"));
}

bool UMingRTSVRSupportSystem::IsARAvailable() const
{
    #if PLATFORM_IOS || PLATFORM_ANDROID
        return true;
    #else
        return false;
    #endif
}

bool UMingRTSVRSupportSystem::StartARMode()
{
    if (!IsARAvailable())
    {
        UE_LOG(LogTemp, Warning, TEXT("AR is not available on this platform"));
        return false;
    }
    
    SystemStatus = EXRSystemStatus::Initializing;
    
    InitializeAR();
    
    bVRMode = false;
    bXREnabled = true;
    SystemStatus = EXRSystemStatus::Running;
    
    UE_LOG(LogTemp, Log, TEXT("AR mode started"));
    return true;
}

void UMingRTSVRSupportSystem::StopARMode()
{
    ShutdownAR();
    
    bXREnabled = false;
    SystemStatus = EXRSystemStatus::Ready;
    
    UE_LOG(LogTemp, Log, TEXT("AR mode stopped"));
}

EARDeviceType UMingRTSVRSupportSystem::GetConnectedARDevice() const
{
    #if PLATFORM_IOS
        return EARDeviceType::ARKit;
    #elif PLATFORM_ANDROID
        return EARDeviceType::ARCore;
    #else
        return EARDeviceType::None;
    #endif
}

void UMingRTSVRSupportSystem::SetARTrackingConfig(const FARTrackingConfig& Config)
{
    CurrentARConfig = Config;
    UE_LOG(LogTemp, Log, TEXT("AR tracking config updated"));
}

TArray<FTransform> UMingRTSVRSupportSystem::GetDetectedPlanes() const
{
    return DetectedPlanes;
}

FTransform UMingRTSVRSupportSystem::GetARCameraTransform() const
{
    FTransform Transform;
    
    if (SystemStatus == EXRSystemStatus::Running && !bVRMode)
    {
        Transform.SetLocation(FVector(0.0f, 0.0f, 150.0f));
        Transform.SetRotation(FQuat::Identity);
    }
    
    return Transform;
}

void UMingRTSVRSupportSystem::PlaceObjectInAR(const FVector& Location, const FRotator& Rotation)
{
    if (SystemStatus != EXRSystemStatus::Running || bVRMode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot place object: AR not running"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Object placed in AR at: %s"), *Location.ToString());
}

EXRSystemStatus UMingRTSVRSupportSystem::GetSystemStatus() const
{
    return SystemStatus;
}

void UMingRTSVRSupportSystem::SetXREnabled(bool bEnabled)
{
    bXREnabled = bEnabled;
    
    if (!bEnabled && SystemStatus == EXRSystemStatus::Running)
    {
        if (bVRMode)
        {
            StopVRMode();
        }
        else
        {
            StopARMode();
        }
    }
}

bool UMingRTSVRSupportSystem::IsXREnabled() const
{
    return bXREnabled;
}

void UMingRTSVRSupportSystem::SetXRMode(bool bInVRMode)
{
    if (bVRMode == bInVRMode)
    {
        return;
    }
    
    // 停止當前模式
    if (SystemStatus == EXRSystemStatus::Running)
    {
        if (bVRMode)
        {
            StopVRMode();
        }
        else
        {
            StopARMode();
        }
    }
    
    bVRMode = bInVRMode;
    
    // 啟動新模式
    if (bXREnabled)
    {
        if (bVRMode)
        {
            StartVRMode();
        }
        else
        {
            StartARMode();
        }
    }
}

bool UMingRTSVRSupportSystem::IsVRMode() const
{
    return bVRMode;
}

bool UMingRTSVRSupportSystem::GetXRButtonPressed(EVRInputType InputType, int32 ButtonIndex) const
{
    // 模擬輸入檢測
    return false;
}

float UMingRTSVRSupportSystem::GetXRAxisValue(EVRInputType InputType, int32 AxisIndex) const
{
    // 模擬軸輸入
    return 0.0f;
}

FVector2D UMingRTSVRSupportSystem::GetXRTouchpadPosition(EVRInputType InputType) const
{
    // 模擬觸控板位置
    return FVector2D::ZeroVector;
}

void UMingRTSVRSupportSystem::SetXRInputMapping(const TMap<EVRInputType, FName>& InputMapping)
{
    UE_LOG(LogTemp, Log, TEXT("XR input mapping updated with %d entries"), InputMapping.Num());
}

void UMingRTSVRSupportSystem::SetAdaptiveQualityEnabled(bool bEnabled)
{
    bAdaptiveQualityEnabled = bEnabled;
    UE_LOG(LogTemp, Log, TEXT("Adaptive quality %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVRSupportSystem::SetFixedFoveatedLevel(int32 Level)
{
    FixedFoveatedLevel = FMath::Clamp(Level, 0, 4);
    UE_LOG(LogTemp, Log, TEXT("Fixed foveated level set to: %d"), FixedFoveatedLevel);
}

void UMingRTSVRSupportSystem::SetResolutionScale(float Scale)
{
    ResolutionScale = FMath::Clamp(Scale, 0.5f, 2.0f);
    UE_LOG(LogTemp, Log, TEXT("Resolution scale set to: %.2f"), ResolutionScale);
}

void UMingRTSVRSupportSystem::InitializeVR()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing VR..."));
    
    // 模擬設備檢測
    ConnectedVRDevice = EVRDeviceType::Generic;
    
    // 設置默認邊界
    BoundaryData.bIsValid = true;
    BoundaryData.Center = FVector::ZeroVector;
    BoundaryData.MaxHeight = 200.0f;
    BoundaryData.PlayAreaSize = 200.0f; // 2米 x 2米
    BoundaryData.BoundaryPoints = {
        FVector(-100.0f, -100.0f, 0.0f),
        FVector(100.0f, -100.0f, 0.0f),
        FVector(100.0f, 100.0f, 0.0f),
        FVector(-100.0f, 100.0f, 0.0f)
    };
    
    OnVRDeviceConnected.Broadcast(ConnectedVRDevice);
}

void UMingRTSVRSupportSystem::ShutdownVR()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down VR..."));
    
    ConnectedVRDevice = EVRDeviceType::None;
    
    OnVRDeviceDisconnected.Broadcast();
}

void UMingRTSVRSupportSystem::InitializeAR()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing AR..."));
    
    ConnectedARDevice = GetConnectedARDevice();
    
    // 模擬平面檢測
    if (CurrentARConfig.bEnablePlaneDetection)
    {
        FTransform Plane;
        Plane.SetLocation(FVector(0.0f, 0.0f, 0.0f));
        Plane.SetRotation(FQuat::Identity);
        DetectedPlanes.Add(Plane);
        
        OnARPlaneDetected.Broadcast();
    }
}

void UMingRTSVRSupportSystem::ShutdownAR()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down AR..."));
    
    DetectedPlanes.Empty();
    ConnectedARDevice = EARDeviceType::None;
}

void UMingRTSVRSupportSystem::UpdateTracking()
{
    if (SystemStatus != EXRSystemStatus::Running)
    {
        return;
    }
    
    // 更新追踪數據
    CheckBoundary();
    OptimizePerformance();
}

void UMingRTSVRSupportSystem::CheckBoundary()
{
    if (!BoundaryData.bIsValid)
    {
        return;
    }
    
    FVRTrackingData HeadData = GetHeadTrackingData();
    bool bInside = IsInsideBoundary(HeadData.Position);
    
    if (!bInside)
    {
        OnBoundaryEntered.Broadcast(true);
    }
}

void UMingRTSVRSupportSystem::OptimizePerformance()
{
    if (!bAdaptiveQualityEnabled)
    {
        return;
    }
    
    // 簡化的性能優化邏輯
    // 實際應該根據幀率動態調整
}

bool UMingRTSVRSupportSystem::ValidateVRSettings(const FVRSettings& Settings) const
{
    if (Settings.IPD < 0.05f || Settings.IPD > 0.08f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid IPD value: %.3f"), Settings.IPD);
        return false;
    }
    
    if (Settings.WorldScale < 0.1f || Settings.WorldScale > 10.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid world scale: %.2f"), Settings.WorldScale);
        return false;
    }
    
    return true;
}

void UMingRTSVRSupportSystem::LoadDefaultSettings()
{
    CurrentVRSettings = FVRSettings();
    CurrentARConfig = FARTrackingConfig();
    
    UE_LOG(LogTemp, Log, TEXT("Default XR settings loaded"));
}
