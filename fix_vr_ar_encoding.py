#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
修复 VR/AR 系统头文件编码问题
"""

import os
import re

# 修复后的 MingVRSupportSystem.h 内容
MINGVR_SUPPORT_SYSTEM_H = '''// Copyright (c) 2026 MingGoRTS. All rights reserved.
// VR Support System - C1-1
// Provides VR device compatibility and VR-specific features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingVRSupportSystem.generated.h"

UENUM(BlueprintType)
enum class EVRDeviceType: uint8 {
    None = 0 UMETA(DisplayName = "None"),
    OculusRift UMETA(DisplayName = "Oculus Rift"),
    OculusQuest UMETA(DisplayName = "Oculus Quest"),
    HTCVive UMETA(DisplayName = "HTC Vive"),
    HTC VivePro UMETA(DisplayName = "HTC Vive Pro"),
    ValveIndex UMETA(DisplayName = "Valve Index"),
    WindowsMR UMETA(DisplayName = "Windows Mixed Reality"),
    PlayStationVR UMETA(DisplayName = "PlayStation VR"),
    Pico UMETA(DisplayName = "Pico VR"),
    GenericHMD UMETA(DisplayName = "Generic HMD")
};

UENUM(BlueprintType)
enum class EVRTrackingSpace: uint8 {
    Seated = 0 UMETA(DisplayName = "Seated"),
    Standing UMETA(DisplayName = "Standing"),
    RoomScale UMETA(DisplayName = "Room Scale"),
    Custom UMETA(DisplayName = "Custom")
};

UENUM(BlueprintType)
enum class EVRControllerType: uint8 {
    MotionController = 0 UMETA(DisplayName = "Motion Controller"),
    Gamepad UMETA(DisplayName = "Gamepad"),
    KeyboardMouse UMETA(DisplayName = "Keyboard & Mouse"),
    HandTracking UMETA(DisplayName = "Hand Tracking"),
    EyeTracking UMETA(DisplayName = "Eye Tracking"),
    VoiceControl UMETA(DisplayName = "Voice Control")
};

USTRUCT(BlueprintType)
struct FVRDeviceInfo
{
    GENERATED_BODY()

    UPROPERTY()
    EVRDeviceType DeviceType;

    UPROPERTY()
    FString DeviceName;

    UPROPERTY()
    FString Manufacturer;

    UPROPERTY()
    FString Model;

    UPROPERTY()
    FString SerialNumber;

    UPROPERTY()
    bool bIsConnected;

    UPROPERTY()
    bool bIsTracking;

    UPROPERTY()
    FIntPoint DisplayResolution;

    UPROPERTY()
    float RefreshRate;

    UPROPERTY()
    float IPD;

    UPROPERTY()
    float FOV;

    FVRDeviceInfo()
        : DeviceType(EVRDeviceType::None)
        , bIsConnected(false)
        , bIsTracking(false)
        , DisplayResolution(2160, 1200)
        , RefreshRate(90.0f)
        , IPD(63.0f)
        , FOV(110.0f)
    {}
};

/**
 * VR Support System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingVRSupportSystem : public UObject
{
    GENERATED_BODY()

public:
    UMingVRSupportSystem();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void InitializeVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void ShutdownVR();

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    bool IsVRConnected() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRDeviceType GetConnectedDevice() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void SetTrackingSpace(EVRTrackingSpace Space);

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    EVRTrackingSpace GetTrackingSpace() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    void RecenterTrackingOrigin();

    UFUNCTION(BlueprintPure, Category = "VR Support")
    bool IsHeadsetTracked() const;

    UFUNCTION(BlueprintPure, Category = "VR Support")
    FTransform GetHeadPose() const;

    UFUNCTION(BlueprintCallable, Category = "VR Support")
    TArray<FVRDeviceInfo> GetSupportedDevices() const;

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bVRActive;

    UPROPERTY()
    EVRDeviceType CurrentDevice;

    UPROPERTY()
    EVRTrackingSpace CurrentTrackingSpace;

    UPROPERTY()
    FVRDeviceInfo DeviceInfo;

    UPROPERTY()
    FTransform HeadPose;

    void InitializeDevice(EVRDeviceType DeviceType);
    void UpdateTrackingData();
    void ProcessControllerInput();
};
'''

# 修复后的 MingARContentOverlay.h 内容
MINGAR_CONTENT_OVERLAY_H = '''// Copyright (c) 2026 MingGoRTS. All rights reserved.
// AR Content Overlay System - C1-2
// Provides AR content overlay and AR interaction features

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "MingARContentOverlay.generated.h"

UENUM(BlueprintType)
enum class EARDeviceType: uint8 {
    None = 0 UMETA(DisplayName = "None"),
    AppleARKit UMETA(DisplayName = "Apple ARKit"),
    GoogleARCore UMETA(DisplayName = "Google ARCore"),
    MicrosoftHoloLens UMETA(DisplayName = "Microsoft HoloLens"),
    MagicLeap UMETA(DisplayName = "Magic Leap"),
    MetaQuestPro UMETA(DisplayName = "Meta Quest Pro"),
    GenericAR UMETA(DisplayName = "Generic AR")
};

UENUM(BlueprintType)
enum class EARTrackingState: uint8 {
    NotTracking = 0 UMETA(DisplayName = "Not Tracking"),
    Limited UMETA(DisplayName = "Limited"),
    Normal UMETA(DisplayName = "Normal")
};

UENUM(BlueprintType)
enum class EARAnchorType: uint8 {
    Plane = 0 UMETA(DisplayName = "Plane"),
    Point UMETA(DisplayName = "Point"),
    Image UMETA(DisplayName = "Image"),
    Face UMETA(DisplayName = "Face"),
    QRCode UMETA(DisplayName = "QR Code"),
    Object UMETA(DisplayName = "Object"),
    GeoAnchor UMETA(DisplayName = "Geo Anchor"),
    Mesh UMETA(DisplayName = "Mesh")
};

UENUM(BlueprintType)
enum class EARPlaneAlignment: uint8 {
    HorizontalUp = 0 UMETA(DisplayName = "Horizontal Up"),
    HorizontalDown UMETA(DisplayName = "Horizontal Down"),
    Vertical UMETA(DisplayName = "Vertical"),
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

    FARPlaneGeometry()
        : Center(FVector::ZeroVector)
        , Extent(FVector::ZeroVector)
        , Alignment(EARPlaneAlignment::None)
    {}
};

USTRUCT(BlueprintType)
struct FARContentOverlay
{
    GENERATED_BODY()

    UPROPERTY()
    FString ContentID;

    UPROPERTY()
    FString ContentType;

    UPROPERTY()
    UTexture2D* OverlayTexture;

    UPROPERTY()
    FVector WorldPosition;

    UPROPERTY()
    FRotator WorldRotation;

    UPROPERTY()
    FVector Scale;

    UPROPERTY()
    bool bIsVisible;

    UPROPERTY()
    float Opacity;

    FARContentOverlay()
        : OverlayTexture(nullptr)
        , WorldPosition(FVector::ZeroVector)
        , WorldRotation(FRotator::ZeroRotator)
        , Scale(FVector(1.0f))
        , bIsVisible(true)
        , Opacity(1.0f)
    {}
};

/**
 * AR Content Overlay System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingARContentOverlay : public UObject
{
    GENERATED_BODY()

public:
    UMingARContentOverlay();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void InitializeAR();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void ShutdownAR();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool IsARSupported() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool StartARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void StopARSession();

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    EARTrackingState GetTrackingState() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void AddContentOverlay(const FARContentOverlay& Overlay);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void RemoveContentOverlay(const FString& ContentID);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    void UpdateContentPosition(const FString& ContentID, const FVector& NewPosition);

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    TArray<FARPlaneGeometry> GetDetectedPlanes() const;

    UFUNCTION(BlueprintCallable, Category = "AR Content")
    bool PlaceContentOnPlane(const FString& ContentID, const FARPlaneGeometry& Plane);

protected:
    UPROPERTY()
    bool bIsInitialized;

    UPROPERTY()
    bool bARSessionActive;

    UPROPERTY()
    EARDeviceType CurrentDevice;

    UPROPERTY()
    EARTrackingState TrackingState;

    UPROPERTY()
    TArray<FARContentOverlay> ActiveOverlays;

    UPROPERTY()
    TArray<FARPlaneGeometry> DetectedPlanes;

    void InitializeARDevice();
    void UpdateTracking();
    void ProcessPlanes();
    void RenderOverlays();
};
'''

def fix_file(filepath, content):
    """修复单个文件"""
    try:
        with open(filepath, 'w', encoding='utf-8') as f:
            f.write(content)
        print(f"Fixed: {filepath}")
        return True
    except Exception as e:
        print(f"Error fixing {filepath}: {e}")
        return False

def main():
    """主函数"""
    vr_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRARM\MingVRSupportSystem.h'
    ar_file = r'C:\HW\MingGoRTS\Source\MingGoRTS\Public\VRARM\MingARContentOverlay.h'
    
    success = True
    
    # 修复 VR 文件
    if os.path.exists(vr_file):
        if not fix_file(vr_file, MINGVR_SUPPORT_SYSTEM_H):
            success = False
    else:
        print(f"Not found: {vr_file}")
    
    # 修复 AR 文件
    if os.path.exists(ar_file):
        if not fix_file(ar_file, MINGAR_CONTENT_OVERLAY_H):
            success = False
    else:
        print(f"Not found: {ar_file}")
    
    if success:
        print("\nDone! VR/AR system files fixed successfully.")
    else:
        print("\nSome files could not be fixed.")
    
    return 0 if success else 1

if __name__ == '__main__':
    exit(main())
