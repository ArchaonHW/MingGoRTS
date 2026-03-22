// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR Performance Optimization

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingRTSVARPerformanceOptimizer.generated.h"

/**
 * VR/AR Performance Quality Levels
 */
UENUM(BlueprintType)
enum class EVARPerformanceLevel : uint8
{
    Low         UMETA(DisplayName = "Low Quality"),
    Medium      UMETA(DisplayName = "Medium Quality"),
    High        UMETA(DisplayName = "High Quality"),
    Ultra       UMETA(DisplayName = "Ultra Quality"),
    Custom      UMETA(DisplayName = "Custom")
};

/**
 * Frame Rate Targets
 */
UENUM(BlueprintType)
enum class EVARFrameRate : uint8
{
    FPS30       UMETA(DisplayName = "30 FPS"),
    FPS60       UMETA(DisplayName = "60 FPS"),
    FPS72       UMETA(DisplayName = "72 FPS"),
    FPS90       UMETA(DisplayName = "90 FPS"),
    FPS120      UMETA(DisplayName = "120 FPS")
};

/**
 * Optimization Settings Structure
 */
USTRUCT(BlueprintType)
struct FVAROptimizationSettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    EVARPerformanceLevel QualityLevel = EVARPerformanceLevel::High;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    EVARFrameRate TargetFrameRate = EVARFrameRate::FPS72;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableAdaptiveQuality = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableFoveatedRendering = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableDynamicResolution = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableOcclusionCulling = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableLODSystem = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableTextureStreaming = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bReduceParticleEffects = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bSimplifyShadows = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    int32 MaxVisibleUnits = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    float ViewDistance = 5000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    float TextureQuality = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableASW = true; // Asynchronous Spacewarp

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VR/AR Optimization")
    bool bEnableMotionSmoothing = true;
};

/**
 * Performance Metrics Structure
 */
USTRUCT(BlueprintType)
struct FVARPerformanceMetrics
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float CurrentFPS = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float AverageFPS = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float MinFPS = 999.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float MaxFPS = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float FrameTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float GPUFrameTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float CPUFrameTime = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int32 DrawCalls = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int32 TriangleCount = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float MemoryUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    float VRAMUsage = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    bool bDroppedFrames = false;

    UPROPERTY(BlueprintReadOnly, Category = "Performance Metrics")
    int32 DroppedFrameCount = 0;
};

/**
 * MingGoRTS VR/AR Performance Optimizer
 * Manages performance settings and adaptive quality for VR/AR modes
 */
UCLASS()
class MINGGORTS_API UMingRTSVARPerformanceOptimizer : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    // Initialization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    void InitializeOptimizer();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    void ShutdownOptimizer();

    // Configuration
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    void SetOptimizationSettings(const FVAROptimizationSettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    FVAROptimizationSettings GetOptimizationSettings() const { return CurrentSettings; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    void ApplyQualityLevel(EVARPerformanceLevel Level);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    EVARPerformanceLevel GetCurrentQualityLevel() const { return CurrentSettings.QualityLevel; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    void SetTargetFrameRate(EVARFrameRate FrameRate);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance")
    EVARFrameRate GetTargetFrameRate() const { return CurrentSettings.TargetFrameRate; }

    // Adaptive Quality
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    void EnableAdaptiveQuality(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    bool IsAdaptiveQualityEnabled() const { return CurrentSettings.bEnableAdaptiveQuality; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    void UpdateAdaptiveQuality();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    bool ShouldReduceQuality() const;

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    void ReduceQualityStep();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Adaptive")
    void ImproveQualityStep();

    // Performance Monitoring
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Monitoring")
    FVARPerformanceMetrics GetCurrentMetrics() const { return CurrentMetrics; }

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Monitoring")
    void StartPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Monitoring")
    void StopPerformanceMonitoring();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Monitoring")
    void ResetPerformanceStats();

    // VR Specific
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|VR")
    void EnableFoveatedRendering(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|VR")
    void SetFoveationLevel(int32 Level);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|VR")
    void EnableASW(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|VR")
    void EnableMotionSmoothing(bool bEnable);

    // AR Specific
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|AR")
    void SetARCameraResolution(int32 Width, int32 Height);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|AR")
    void ReduceARTrackingFrequency(bool bReduce);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|AR")
    void EnableARPlaneOptimization(bool bEnable);

    // Graphics Optimization
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Graphics")
    void SetViewDistance(float Distance);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Graphics")
    void SetTextureQuality(float Quality);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Graphics")
    void SetMaxVisibleUnits(int32 Count);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Graphics")
    void EnableLODSystem(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Graphics")
    void EnableOcclusionCulling(bool bEnable);

    // Memory Management
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Memory")
    void EnableTextureStreaming(bool bEnable);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Memory")
    void FlushUnusedMemory();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Memory")
    void SetMemoryBudget(float Megabytes);

    // Presets
    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyPerformancePreset(EVARPerformanceLevel Level);

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyMobileVRPreset();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyStandaloneVRPreset();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyPCVRPreset();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyMobileARPreset();

    UFUNCTION(BlueprintCallable, Category = "VR/AR Performance|Presets")
    void ApplyHoloLensPreset();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "VR/AR Performance|Events")
    FOnPerformanceLevelChanged OnPerformanceLevelChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Performance|Events")
    FOnFrameRateChanged OnFrameRateChanged;

    UPROPERTY(BlueprintAssignable, Category = "VR/AR Performance|Events")
    FOnPerformanceWarning OnPerformanceWarning;

    // Override
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

protected:
    void UpdatePerformanceMetrics(float DeltaTime);
    void ApplyOptimizationSettings();
    float GetTargetFPS() const;
    void LogPerformanceStats();

private:
    UPROPERTY()
    FVAROptimizationSettings CurrentSettings;

    UPROPERTY()
    FVARPerformanceMetrics CurrentMetrics;

    UPROPERTY()
    bool bIsInitialized = false;

    UPROPERTY()
    bool bIsMonitoring = false;

    UPROPERTY()
    float MetricsUpdateInterval = 1.0f;

    UPROPERTY()
    float MetricsUpdateTimer = 0.0f;

    UPROPERTY()
    TArray<float> FPSHistory;

    UPROPERTY()
    int32 QualityReductionSteps = 0;

    UPROPERTY()
    int32 MaxQualityReductionSteps = 3;

    UPROPERTY()
    float MemoryBudgetMB = 2048.0f;
};

// Event delegate declarations
declare dynamic multicast_delegate FOnPerformanceLevelChanged(EVARPerformanceLevel NewLevel);
declare dynamic multicast_delegate FOnFrameRateChanged(EVARFrameRate NewFrameRate);
declare dynamic multicast_delegate FOnPerformanceWarning(const FString& WarningMessage);
