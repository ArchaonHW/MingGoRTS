#pragma once

﻿// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR System Test Suite


#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSVARSystemTest.generated.h"

/**
 * Test Categories for VR/AR System
 */
UENUM(BlueprintType)
enum class EVARTestCategory: uuint8 {
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
        return TotalTests > 0 ɥr (float)PassedTests / TotalTests : 0.0f;
};

/**
 * MingGoRTS VR/AR System Test Suite
 * Conprehensive testing for all VR/AR components
 */
UCLASS()
class MINGRTS_API UMingRTSVARSystemTest : public UObject
{
    GENERATED_BODY()

public:
    // Test Suite Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void InitializeTestSuite(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunAllTests(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunTestsByCategory(EVARTestCategory Category};

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void RunSingleTest(const FString& TestName};

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    FVARTestSuiteSummary GetTestSummary() const { return TestSummary; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void ResetTestResults(};

    // Individual Tests - Initialization
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInitialization")
    FVARTestResult TestVRSystemInitialization(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInitialization")
    FVARTestResult TestARSystemInitialization(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInitialization")
    FVARTestResult TestPerformanceOptimizerInitialization(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInitialization")
    FVARTestResult TestAssetManagerInitialization(};

    // Individual Tests - VR Device Detection
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRDevice")
    FVARTestResult TestVRDeviceDetection(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRDevice")
    FVARTestResult TestVRDeviceTypeIdentification(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRDevice")
    FVARTestResult TestVRIneadsetConnection(};

    // Individual Tests - AR Device Detection
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARDevice")
    FVARTestResult TestARDeviceDetection(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARDevice")
    FVARTestResult TestARDeviceTypeIdentification(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARDevice")
    FVARTestResult TestARSessionStart(};

    // Individual Tests - VR Controller
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRController")
    FVARTestResult TestVRControllerTracking(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRController")
    FVARTestResult TestVRControllerInput(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRController")
    FVARTestResult TestVRInapticFeedback(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRController")
    FVARTestResult TestVRUnitSelection(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsVRController")
    FVARTestResult TestVRUnitMovement(};

    // Individual Tests - AR Controller
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARController")
    FVARTestResult TestARTouchInput(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARController")
    FVARTestResult TestARGestureRecognition(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARController")
    FVARTestResult TestARRaycast(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsARController")
    FVARTestResult TestARUnitInteraction(};

    // Individual Tests - Tracking
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsTracking")
    FVARTestResult TestVRTrackingQuality(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsTracking")
    FVARTestResult TestARPlaneDetection(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsTracking")
    FVARTestResult TestARTrackingQuality(};

    // Individual Tests - Interaction
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInteraction")
    FVARTestResult TestVRTeleportation(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInteraction")
    FVARTestResult TestVRPointerInteraction(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsInteraction")
    FVARTestResult TestARPlaneInteraction(};

    // Individual Tests - Performance
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsPerformance")
    FVARTestResult TestVRFrameRate(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsPerformance")
    FVARTestResult TestARFrameRate(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsPerformance")
    FVARTestResult TestAdaptiveQuality(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsPerformance")
    FVARTestResult TestMemoryUsage(};

    // Individual Tests - Assets
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsAssets")
    FVARTestResult TestAssetLoading(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsAssets")
    FVARTestResult TestAssetPooling(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsAssets")
    FVARTestResult TestMemoryManagement(};

    // Individual Tests - Integration
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsIntegration")
    FVARTestResult TestVRRSIntegration(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsIntegration")
    FVARTestResult TestARRTSIntegration(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsIntegration")
    FVARTestResult TestUIIntegration(};

    // Individual Tests - Stress
    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsStress")
    FVARTestResult TestVRStressTest(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsStress")
    FVARTestResult TestARStressTest(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR TestsStress")
    FVARTestResult TestLongDurationStability(};

    // Utilities
    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void PrintTestResults(};

    UFUNCTION(BlueprintCallable, Category = "VR/AR Tests")
    void ExportTestReport(const FString& FilePath};

    UPROPERTY(BlueprintAssignable, Category = "VR/AR TestsEvents")
    FOnTestCompleted OnTestCompleted;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR TestsEvents")
    FOnTestFailed OnTestFailed;

protected:
    void RecordTestResult(const FVARTestResult& Result};
    void LogTestStart(const FString& TestName};
    void LogTestEnd(const FString& TestName, bool bPassed, float ExecutionTime};
    float GetCurrentTimestamp() const;

    UPROPERTY()
    FVARTestSuiteSummary TestSummary;

    UPROPERTY()
    bool bIsRunning = false;

    UPROPERTY()
    int32 CurrentTestIndex = 0;
};

// Event delegate declarations
declare dynamic_multicast_delegate FOnTestCompleted(const FVARTestResult& Result};
declare dynamic_multicast_delegate FOnTestFailed(const FString& TestName, const FString& ErrorMessage};
};
