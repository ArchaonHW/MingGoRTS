// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR System Test Suite

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSVARSystemTest.generated.h"

/**
 * Test Categories for VR/AR System
 */
UENUM(BlueprintType)
enum class EVARTestCategory: uint8 {
    Initialization      UMETA(DisplayName = "Initialization"),
    VRDeviceDetection   UMETA(DisplayName = "VR Device Detection"),
    ARDeviceDetection   UMETA(DisplayName = "AR Device Detection"),
    VRController        UMETA(DisplayName = "VR Controller"),
    ARController        UMETA(DisplayName = "AR Controller"),
    Tracking            UMETA(DisplayName = "Tracking"),
    Interaction         UMETA(DisplayName = "Interaction"),
    Performance         UMETA(DisplayName = "Performance"),
    Assets              UMETA(DisplayName = "Assets"),
    Integration         UMETA(DisplayName = "Integration"),
    StressTest          UMETA(DisplayName = "Stress Test")
};

/**
 * Test Result Structure
 */
USTRUCT(BlueprintType)
struct FVARTestResult
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FString TestName;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    EVARTestCategory Category = EVARTestCategory::Initialization;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    bool bPassed = false;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    float ExecutionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Test Result")
    FDateTime Timestamp;
};

/**
 * Test Suite Summary
 */
USTRUCT(BlueprintType)
struct FVARTestSuiteSummary
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 TotalTests = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 PassedTests = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    int32 FailedTests = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    float TotalExecutionTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    TArray<FVARTestResult> Results;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    FDateTime StartTime;

    UPROPERTY(BlueprintReadOnly, Category = "Test Summary")
    FDateTime EndTime;

    float GetPassRate() const
    {
        return TotalTests > 0 ? (float)PassedTests / TotalTests : 0.0f;
    }
};

/**
 * MingGoRTS VR/AR System Test Suite
 * Comprehensive testing for all VR/AR components
 */
UCLASS()
class MINGRTS_API UMingRTSVARSystemTest : public UObject
{
    GENERATED_BODY()

public:
    // Test Suite Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void InitializeTestSuite();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunAllTests();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunTestsByCategory(EVARTestCategory Category);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunSingleTest(const FString& TestName);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    FVARTestSuiteSummary GetTestSummary() const { return TestSummary; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void ResetTestResults();

    // Individual Tests - Initialization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Initialization")
    FVARTestResult TestVRSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Initialization")
    FVARTestResult TestARSystemInitialization();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Initialization")
    FVARTestResult TestPerformanceOptimizerInitialization();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Initialization")
    FVARTestResult TestAssetManagerInitialization();

    // Individual Tests - VR Device Detection
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRDevice")
    FVARTestResult TestVRDeviceDetection();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRDevice")
    FVARTestResult TestVRDeviceTypeIdentification();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRDevice")
    FVARTestResult TestVRHeadsetConnection();

    // Individual Tests - AR Device Detection
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARDevice")
    FVARTestResult TestARDeviceDetection();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARDevice")
    FVARTestResult TestARDeviceTypeIdentification();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARDevice")
    FVARTestResult TestARSessionStart();

    // Individual Tests - VR Controller
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRController")
    FVARTestResult TestVRControllerTracking();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRController")
    FVARTestResult TestVRControllerInput();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRController")
    FVARTestResult TestVRHapticFeedback();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRController")
    FVARTestResult TestVRUnitSelection();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|VRController")
    FVARTestResult TestVRUnitMovement();

    // Individual Tests - AR Controller
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARController")
    FVARTestResult TestARTouchInput();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARController")
    FVARTestResult TestARGestureRecognition();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARController")
    FVARTestResult TestARRaycast();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|ARController")
    FVARTestResult TestARUnitInteraction();

    // Individual Tests - Tracking
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Tracking")
    FVARTestResult TestVRTrackingQuality();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Tracking")
    FVARTestResult TestARPlaneDetection();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Tracking")
    FVARTestResult TestARTrackingQuality();

    // Individual Tests - Interaction
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Interaction")
    FVARTestResult TestVRTeleportation();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Interaction")
    FVARTestResult TestVRPointerInteraction();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Interaction")
    FVARTestResult TestARPlaneInteraction();

    // Individual Tests - Performance
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Performance")
    FVARTestResult TestVRFrameRate();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Performance")
    FVARTestResult TestARFrameRate();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Performance")
    FVARTestResult TestAdaptiveQuality();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Performance")
    FVARTestResult TestMemoryUsage();

    // Individual Tests - Assets
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Assets")
    FVARTestResult TestAssetLoading();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Assets")
    FVARTestResult TestAssetPooling();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Assets")
    FVARTestResult TestMemoryManagement();

    // Individual Tests - Integration
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Integration")
    FVARTestResult TestVRRSIntegration();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Integration")
    FVARTestResult TestARRTSIntegration();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Integration")
    FVARTestResult TestUIIntegration();

    // Individual Tests - Stress
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Stress")
    FVARTestResult TestVRStressTest();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Stress")
    FVARTestResult TestARStressTest();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests|Stress")
    FVARTestResult TestLongDurationStability();

    // Utilities
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void PrintTestResults();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void ExportTestReport(const FString& FilePath);

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Tests|Events")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Tests|Events")
    FOnTestFailed OnTestFailed;

protected:
    void RecordTestResult(const FVARTestResult& Result);
    void LogTestStart(const FString& TestName);
    void LogTestEnd(const FString& TestName, bool bPassed, float ExecutionTime);
    float GetCurrentTimestamp() const;

    UPROPERTY()
    FVARTestSuiteSummary TestSummary;

    UPROPERTY()
    bool bIsRunning = false;

    UPROPERTY()
    int32 CurrentTestIndex = 0;
};

// Event delegate declarations
declare dynamic_multicast_delegate FOnTestCompleted(const FVARTestResult& Result);
declare dynamic_multicast_delegate FOnTestFailed(const FString& TestName, const FString& ErrorMessage);
