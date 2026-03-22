#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSVRSupportSystem.generated.h"

/**
 * VR設備類型
 */
UENUM(BlueprintType)
enum class EVRDeviceType: uint8 {
    None                  UMETA(DisplayName = "無"),
    OculusRift            UMETA(DisplayName = "Oculus Rift"),
    OculusQuest           UMETA(DisplayName = "Oculus Quest"),
    HTC_Vive              UMETA(DisplayName = "HTC Vive"),
    ValveIndex            UMETA(DisplayName = "Valve Index"),
    WindowsMR             UMETA(DisplayName = "Windows Mixed Reality"),
    PSVR                  UMETA(DisplayName = "PlayStation VR"),
    PICO                  UMETA(DisplayName = "PICO VR"),
    Cardboard             UMETA(DisplayName = "Google Cardboard"),
    Generic               UMETA(DisplayName = "通用設備")
};

/**
 * AR設備類型
 */
UENUM(BlueprintType)
enum class EARDeviceType: uint8 {
    None                  UMETA(DisplayName = "無"),
    ARKit                 UMETA(DisplayName = "ARKit (iOS)"),
    ARCore                UMETA(DisplayName = "ARCore (Android)"),
    HoloLens              UMETA(DisplayName = "Microsoft HoloLens"),
    MagicLeap             UMETA(DisplayName = "Magic Leap"),
    Generic               UMETA(DisplayName = "通用設備")
};

/**
 * VR/AR系統狀態
 */
UENUM(BlueprintType)
enum class EXRSystemStatus: uint8 {
    NotAvailable          UMETA(DisplayName = "不可用"),
    Available             UMETA(DisplayName = "可用"),
    Initializing          UMETA(DisplayName = "初始化中"),
    Ready                 UMETA(DisplayName = "就緒"),
    Running               UMETA(DisplayName = "運行中"),
    Error                 UMETA(DisplayName = "錯誤"),
    Calibrating           UMETA(DisplayName = "校準中")
};

/**
 * VR追踪模式
 */
UENUM(BlueprintType)
enum class EVRTrackingMode: uint8 {
    Seated                UMETA(DisplayName = "坐姿"),
    Standing              UMETA(DisplayName = "站立"),
    RoomScale             UMETA(DisplayName = "房間規模")
};

/**
 * VR輸入類型
 */
UENUM(BlueprintType)
enum class EVRInputType: uint8 {
    Head                  UMETA(DisplayName = "頭部"),
    LeftHand              UMETA(DisplayName = "左手"),
    RightHand             UMETA(DisplayName = "右手"),
    Gamepad               UMETA(DisplayName = "遊戲手柄"),
    Voice                 UMETA(DisplayName = "語音")
};

/**
 * VR追踪數據
 */
USTRUCT(BlueprintType)
struct FVRTrackingData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Position;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FQuat Rotation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Velocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector AngularVelocity;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsTracked;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float TrackingConfidence;
    
    FVRTrackingData()
        : Position(FVector::ZeroVector)
        , Rotation(FQuat::Identity)
        , Velocity(FVector::ZeroVector)
        , AngularVelocity(FVector::ZeroVector)
        , bIsTracked(false)
        , TrackingConfidence(1.0f)
    {}
};

/**
 * VR空間邊界
 */
USTRUCT(BlueprintType)
struct FVRBoundaryData
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FVector> BoundaryPoints;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector Center;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MaxHeight;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PlayAreaSize;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsValid;
    
    FVRBoundaryData()
        : Center(FVector::ZeroVector)
        , MaxHeight(0.0f)
        , PlayAreaSize(0.0f)
        , bIsValid(false)
    {}
};

/**
 * VR設置
 */
USTRUCT(BlueprintType)
struct FVRSettings
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRDeviceType DeviceType;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EVRTrackingMode TrackingMode;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float IPD;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WorldScale;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseSnapTurn;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SnapTurnAngle;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUseTeleportMovement;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VignetteStrength;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bComfortMode;
    
    FVRSettings()
        : DeviceType(EVRDeviceType::None)
        , TrackingMode(EVRTrackingMode::Standing)
        , IPD(0.064f)
        , WorldScale(1.0f)
        , bUseSnapTurn(true)
        , SnapTurnAngle(45.0f)
        , bUseTeleportMovement(true)
        , VignetteStrength(0.3f)
        , bComfortMode(false)
    {}
};

/**
 * AR追踪配置
 */
USTRUCT(BlueprintType)
struct FARTrackingConfig
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnablePlaneDetection;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableImageTracking;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableFaceTracking;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableLightEstimation;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bEnableEnvironmentProbes;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float MinPlaneSize;
    
    FARTrackingConfig()
        : bEnablePlaneDetection(true)
        , bEnableImageTracking(false)
        , bEnableFaceTracking(false)
        , bEnableLightEstimation(true)
        , bEnableEnvironmentProbes(true)
        , MinPlaneSize(0.1f)
    {}
};

/**
 * VR/AR支持系統
 */
UCLASS(ClassGroup=(MingGoRTS), meta=(BlueprintSpawnableComponent))
class MINGRTS_API UMingRTSVRSupportSystem : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSVRSupportSystem();
    
    // 初始化
    UFUNCTION(BlueprintCallable, Category = "XR")
    void InitializeXRSupport();
    
    // VR功能
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    bool IsVRAvailable() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    bool StartVRMode();
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    void StopVRMode();
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    EVRDeviceType GetConnectedVRDevice() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    FVRTrackingData GetHeadTrackingData() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    FVRTrackingData GetHandTrackingData(bool bLeftHand) const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    FVRBoundaryData GetBoundaryData() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    bool IsInsideBoundary(const FVector& Location) const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    void SetVRSettings(const FVRSettings& Settings);
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    FVRSettings GetVRSettings() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    void CalibrateVR();
    
    UFUNCTION(BlueprintCallable, Category = "XR|VR")
    void RecenterVR();
    
    // AR功能
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    bool IsARAvailable() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    bool StartARMode();
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    void StopARMode();
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    EARDeviceType GetConnectedARDevice() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    void SetARTrackingConfig(const FARTrackingConfig& Config);
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    TArray<FTransform> GetDetectedPlanes() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    FTransform GetARCameraTransform() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|AR")
    void PlaceObjectInAR(const FVector& Location, const FRotator& Rotation);
    
    // 通用功能
    UFUNCTION(BlueprintCallable, Category = "XR")
    EXRSystemStatus GetSystemStatus() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR")
    void SetXREnabled(bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "XR")
    bool IsXREnabled() const;
    
    UFUNCTION(BlueprintCallable, Category = "XR")
    void SetXRMode(bool bVRMode);
    
    UFUNCTION(BlueprintCallable, Category = "XR")
    bool IsVRMode() const;
    
    // 輸入處理
    UFUNCTION(BlueprintCallable, Category = "XR|Input")
    bool GetXRButtonPressed(EVRInputType InputType, int32 ButtonIndex) const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|Input")
    float GetXRAxisValue(EVRInputType InputType, int32 AxisIndex) const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|Input")
    FVector2D GetXRTouchpadPosition(EVRInputType InputType) const;
    
    UFUNCTION(BlueprintCallable, Category = "XR|Input")
    void SetXRInputMapping(const TMap<EVRInputType, FName>& InputMapping);
    
    // 性能優化
    UFUNCTION(BlueprintCallable, Category = "XR|Performance")
    void SetAdaptiveQualityEnabled(bool bEnabled);
    
    UFUNCTION(BlueprintCallable, Category = "XR|Performance")
    void SetFixedFoveatedLevel(int32 Level);
    
    UFUNCTION(BlueprintCallable, Category = "XR|Performance")
    void SetResolutionScale(float Scale);
    
    // 事件委託
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnVRDeviceConnected, EVRDeviceType, DeviceType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnVRDeviceDisconnected);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTrackingLost, EVRInputType, InputType);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBoundaryEntered, bool, bViolated);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnARPlaneDetected);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnXRError, const FString&, ErrorMessage);
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnVRDeviceConnected OnVRDeviceConnected;
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnVRDeviceDisconnected OnVRDeviceDisconnected;
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnTrackingLost OnTrackingLost;
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnBoundaryEntered OnBoundaryEntered;
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnARPlaneDetected OnARPlaneDetected;
    
    UPROPERTY(BlueprintAssignable, Category = "XR|Events")
    FOnXRError OnXRError;
    
private:
    UPROPERTY()
    EXRSystemStatus SystemStatus;
    
    UPROPERTY()
    bool bXREnabled;
    
    UPROPERTY()
    bool bVRMode;
    
    UPROPERTY()
    EVRDeviceType ConnectedVRDevice;
    
    UPROPERTY()
    EARDeviceType ConnectedARDevice;
    
    UPROPERTY()
    FVRSettings CurrentVRSettings;
    
    UPROPERTY()
    FARTrackingConfig CurrentARConfig;
    
    UPROPERTY()
    FVRBoundaryData BoundaryData;
    
    UPROPERTY()
    TArray<FTransform> DetectedPlanes;
    
    UPROPERTY()
    bool bAdaptiveQualityEnabled;
    
    UPROPERTY()
    int32 FixedFoveatedLevel;
    
    UPROPERTY()
    float ResolutionScale;
    
    void InitializeVR();
    void ShutdownVR();
    void InitializeAR();
    void ShutdownAR();
    void UpdateTracking();
    void CheckBoundary();
    void OptimizePerformance();
    bool ValidateVRSettings(const FVRSettings& Settings) const;
    void LoadDefaultSettings();
};
