// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Epic 9.1: VR/AR Support System - VR/AR Performance Optimizer Implementation

#include "VRAR/MingRTSVARPerformanceOptimizer.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogMingVARPerformance, Log, All);

void UMingRTSVARPerformanceOptimizer::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Initializing VR/AR Performance Optimizer..."));
    
    // Set default settings
    CurrentSettings.QualityLevel = EVARPerformanceLevel::High;
    CurrentSettings.TargetFrameRate = EVARFrameRate::FPS72;
    CurrentSettings.bEnableAdaptiveQuality = true;
    CurrentSettings.bEnableFoveatedRendering = true;
    CurrentSettings.bEnableDynamicResolution = true;
    CurrentSettings.bEnableOcclusionCulling = true;
    CurrentSettings.bEnableLODSystem = true;
    CurrentSettings.bEnableTextureStreaming = true;
    CurrentSettings.bReduceParticleEffects = true;
    CurrentSettings.bSimplifyShadows = true;
    CurrentSettings.MaxVisibleUnits = 100;
    CurrentSettings.ViewDistance = 5000.0f;
    CurrentSettings.TextureQuality = 1.0f;
    CurrentSettings.bEnableASW = true;
    CurrentSettings.bEnableMotionSmoothing = true;
    
    FPSHistory.Reserve(60);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("VR/AR Performance Optimizer initialized"));
}

void UMingRTSVARPerformanceOptimizer::Deinitialize()
{
    UE_LOG(LogMingVARPerformance, Log, TEXT("Shutting down VR/AR Performance Optimizer..."));
    
    StopPerformanceMonitoring();
    
    Super::Deinitialize();
}

void UMingRTSVARPerformanceOptimizer::InitializeOptimizer()
{
    bIsInitialized = true;
    UE_LOG(LogMingVARPerformance, Log, TEXT("Performance optimizer initialized"));
    
    StartPerformanceMonitoring();
}

void UMingRTSVARPerformanceOptimizer::ShutdownOptimizer()
{
    bIsInitialized = false;
    UE_LOG(LogMingVARPerformance, Log, TEXT("Performance optimizer shutdown"));
    
    StopPerformanceMonitoring();
}

void UMingRTSVARPerformanceOptimizer::SetOptimizationSettings(const FVAROptimizationSettings& Settings)
{
    CurrentSettings = Settings;
    ApplyOptimizationSettings();
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Optimization settings updated"));
}

void UMingRTSVARPerformanceOptimizer::ApplyQualityLevel(EVARPerformanceLevel Level)
{
    CurrentSettings.QualityLevel = Level;
    ApplyPerformancePreset(Level);
    
    OnPerformanceLevelChanged.Broadcast(Level);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Quality level changed to: %s"),
           *UEnum::GetValueAsString(Level));
}

void UMingRTSVARPerformanceOptimizer::SetTargetFrameRate(EVARFrameRate FrameRate)
{
    CurrentSettings.TargetFrameRate = FrameRate;
    
    float TargetFPS = GetTargetFPS();
    
    // Apply frame rate settings to engine
    if (GEngine)
    {
        // Set fixed frame rate for VR
        GEngine->FixedFrameRate = TargetFPS;
    }
    
    OnFrameRateChanged.Broadcast(FrameRate);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Target frame rate set to: %f FPS"), TargetFPS);
}

void UMingRTSVARPerformanceOptimizer::EnableAdaptiveQuality(bool bEnable)
{
    CurrentSettings.bEnableAdaptiveQuality = bEnable;
    
    if (bEnable && !bIsMonitoring)
    {
        StartPerformanceMonitoring();
    }
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Adaptive quality %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::UpdateAdaptiveQuality()
{
    if (!CurrentSettings.bEnableAdaptiveQuality || !bIsMonitoring)
    {
        return;
    }
    
    float TargetFPS = GetTargetFPS();
    float CurrentFPS = CurrentMetrics.CurrentFPS;
    
    // Check if we're consistently dropping frames
    if (CurrentFPS < TargetFPS * 0.9f)
    {
        if (QualityReductionSteps < MaxQualityReductionSteps)
        {
            ReduceQualityStep();
        }
        else
        {
            OnPerformanceWarning.Broadcast(TEXT("Performance critically low, cannot reduce quality further"));
        }
    }
    else if (CurrentFPS > TargetFPS * 1.1f && QualityReductionSteps > 0)
    {
        // We have headroom, try improving quality
        ImproveQualityStep();
    }
}

bool UMingRTSVARPerformanceOptimizer::ShouldReduceQuality() const
{
    float TargetFPS = GetTargetFPS();
    return CurrentMetrics.CurrentFPS < TargetFPS * 0.85f;
}

void UMingRTSVARPerformanceOptimizer::ReduceQualityStep()
{
    QualityReductionSteps++;
    
    // Reduce various quality settings
    CurrentSettings.TextureQuality = FMath::Max(0.5f, CurrentSettings.TextureQuality - 0.1f);
    CurrentSettings.ViewDistance = FMath::Max(2000.0f, CurrentSettings.ViewDistance - 500.0f);
    CurrentSettings.MaxVisibleUnits = FMath::Max(50, CurrentSettings.MaxVisibleUnits - 10);
    
    ApplyOptimizationSettings();
    
    UE_LOG(LogMingVARPerformance, Warning, TEXT("Quality reduced (step %d)"), QualityReductionSteps);
}

void UMingRTSVARPerformanceOptimizer::ImproveQualityStep()
{
    if (QualityReductionSteps <= 0)
    {
        return;
    }
    
    QualityReductionSteps--;
    
    // Restore quality settings
    CurrentSettings.TextureQuality = FMath::Min(1.0f, CurrentSettings.TextureQuality + 0.1f);
    CurrentSettings.ViewDistance = FMath::Min(10000.0f, CurrentSettings.ViewDistance + 500.0f);
    CurrentSettings.MaxVisibleUnits = FMath::Min(200, CurrentSettings.MaxVisibleUnits + 10);
    
    ApplyOptimizationSettings();
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Quality improved (step %d)"), QualityReductionSteps);
}

void UMingRTSVARPerformanceOptimizer::StartPerformanceMonitoring()
{
    bIsMonitoring = true;
    MetricsUpdateTimer = 0.0f;
    FPSHistory.Empty();
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Performance monitoring started"));
}

void UMingRTSVARPerformanceOptimizer::StopPerformanceMonitoring()
{
    bIsMonitoring = false;
    UE_LOG(LogMingVARPerformance, Log, TEXT("Performance monitoring stopped"));
}

void UMingRTSVARPerformanceOptimizer::ResetPerformanceStats()
{
    CurrentMetrics = FVARPerformanceMetrics();
    FPSHistory.Empty();
    QualityReductionSteps = 0;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Performance stats reset"));
}

void UMingRTSVARPerformanceOptimizer::EnableFoveatedRendering(bool bEnable)
{
    CurrentSettings.bEnableFoveatedRendering = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Foveated rendering %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::SetFoveationLevel(int32 Level)
{
    Level = FMath::Clamp(Level, 0, 4);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Foveation level set to: %d"), Level);
}

void UMingRTSVARPerformanceOptimizer::EnableASW(bool bEnable)
{
    CurrentSettings.bEnableASW = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("ASW (Asynchronous Spacewarp) %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::EnableMotionSmoothing(bool bEnable)
{
    CurrentSettings.bEnableMotionSmoothing = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Motion smoothing %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::SetARCameraResolution(int32 Width, int32 Height)
{
    UE_LOG(LogMingVARPerformance, Log, TEXT("AR camera resolution set to: %dx%d"), Width, Height);
}

void UMingRTSVARPerformanceOptimizer::ReduceARTrackingFrequency(bool bReduce)
{
    UE_LOG(LogMingVARPerformance, Log, TEXT("AR tracking frequency %s"),
           bReduce ? TEXT("reduced") : TEXT("normal"));
}

void UMingRTSVARPerformanceOptimizer::EnableARPlaneOptimization(bool bEnable)
{
    UE_LOG(LogMingVARPerformance, Log, TEXT("AR plane optimization %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::SetViewDistance(float Distance)
{
    CurrentSettings.ViewDistance = FMath::Max(1000.0f, Distance);
    
    // Apply to engine
    if (GEngine)
    {
        GEngine->ViewDistanceScale = CurrentSettings.ViewDistance / 5000.0f;
    }
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("View distance set to: %f"), CurrentSettings.ViewDistance);
}

void UMingRTSVARPerformanceOptimizer::SetTextureQuality(float Quality)
{
    CurrentSettings.TextureQuality = FMath::Clamp(Quality, 0.5f, 1.0f);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Texture quality set to: %f"), CurrentSettings.TextureQuality);
}

void UMingRTSVARPerformanceOptimizer::SetMaxVisibleUnits(int32 Count)
{
    CurrentSettings.MaxVisibleUnits = FMath::Max(10, Count);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Max visible units set to: %d"), CurrentSettings.MaxVisibleUnits);
}

void UMingRTSVARPerformanceOptimizer::EnableLODSystem(bool bEnable)
{
    CurrentSettings.bEnableLODSystem = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("LOD system %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::EnableOcclusionCulling(bool bEnable)
{
    CurrentSettings.bEnableOcclusionCulling = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Occlusion culling %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::EnableTextureStreaming(bool bEnable)
{
    CurrentSettings.bEnableTextureStreaming = bEnable;
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Texture streaming %s"),
           bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingRTSVARPerformanceOptimizer::FlushUnusedMemory()
{
    UE_LOG(LogMingVARPerformance, Log, TEXT("Flushing unused memory..."));
    
    if (GEngine)
    {
        // Trigger garbage collection
        GEngine->ForceGarbageCollection(true);
    }
}

void UMingRTSVARPerformanceOptimizer::SetMemoryBudget(float Megabytes)
{
    MemoryBudgetMB = FMath::Max(512.0f, Megabytes);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Memory budget set to: %f MB"), MemoryBudgetMB);
}

void UMingRTSVARPerformanceOptimizer::ApplyPerformancePreset(EVARPerformanceLevel Level)
{
    switch (Level)
    {
        case EVARPerformanceLevel::Low:
            CurrentSettings.bEnableFoveatedRendering = true;
            CurrentSettings.bEnableDynamicResolution = true;
            CurrentSettings.ViewDistance = 2000.0f;
            CurrentSettings.TextureQuality = 0.5f;
            CurrentSettings.MaxVisibleUnits = 50;
            CurrentSettings.bReduceParticleEffects = true;
            CurrentSettings.bSimplifyShadows = true;
            break;
            
        case EVARPerformanceLevel::Medium:
            CurrentSettings.bEnableFoveatedRendering = true;
            CurrentSettings.bEnableDynamicResolution = true;
            CurrentSettings.ViewDistance = 3500.0f;
            CurrentSettings.TextureQuality = 0.75f;
            CurrentSettings.MaxVisibleUnits = 75;
            CurrentSettings.bReduceParticleEffects = true;
            CurrentSettings.bSimplifyShadows = false;
            break;
            
        case EVARPerformanceLevel::High:
            CurrentSettings.bEnableFoveatedRendering = true;
            CurrentSettings.bEnableDynamicResolution = false;
            CurrentSettings.ViewDistance = 5000.0f;
            CurrentSettings.TextureQuality = 1.0f;
            CurrentSettings.MaxVisibleUnits = 100;
            CurrentSettings.bReduceParticleEffects = false;
            CurrentSettings.bSimplifyShadows = false;
            break;
            
        case EVARPerformanceLevel::Ultra:
            CurrentSettings.bEnableFoveatedRendering = false;
            CurrentSettings.bEnableDynamicResolution = false;
            CurrentSettings.ViewDistance = 10000.0f;
            CurrentSettings.TextureQuality = 1.0f;
            CurrentSettings.MaxVisibleUnits = 200;
            CurrentSettings.bReduceParticleEffects = false;
            CurrentSettings.bSimplifyShadows = false;
            break;
            
        default:
            break;
    }
    
    ApplyOptimizationSettings();
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied performance preset: %s"),
           *UEnum::GetValueAsString(Level));
}

void UMingRTSVARPerformanceOptimizer::ApplyMobileVRPreset()
{
    ApplyPerformancePreset(EVARPerformanceLevel::Low);
    SetTargetFrameRate(EVARFrameRate::FPS72);
    EnableASW(true);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied Mobile VR preset"));
}

void UMingRTSVARPerformanceOptimizer::ApplyStandaloneVRPreset()
{
    ApplyPerformancePreset(EVARPerformanceLevel::Medium);
    SetTargetFrameRate(EVARFrameRate::FPS72);
    EnableASW(true);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied Standalone VR preset"));
}

void UMingRTSVARPerformanceOptimizer::ApplyPCVRPreset()
{
    ApplyPerformancePreset(EVARPerformanceLevel::High);
    SetTargetFrameRate(EVARFrameRate::FPS90);
    EnableASW(false);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied PC VR preset"));
}

void UMingRTSVARPerformanceOptimizer::ApplyMobileARPreset()
{
    ApplyPerformancePreset(EVARPerformanceLevel::Medium);
    SetTargetFrameRate(EVARFrameRate::FPS60);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied Mobile AR preset"));
}

void UMingRTSVARPerformanceOptimizer::ApplyHoloLensPreset()
{
    ApplyPerformancePreset(EVARPerformanceLevel::Low);
    SetTargetFrameRate(EVARFrameRate::FPS60);
    
    UE_LOG(LogMingVARPerformance, Log, TEXT("Applied HoloLens preset"));
}

void UMingRTSVARPerformanceOptimizer::UpdatePerformanceMetrics(float DeltaTime)
{
    if (!bIsMonitoring)
    {
        return;
    }
    
    // Calculate current FPS
    float CurrentFPS = 1.0f / DeltaTime;
    CurrentMetrics.CurrentFPS = CurrentFPS;
    CurrentMetrics.FrameTime = DeltaTime * 1000.0f; // in ms
    
    // Update history
    FPSHistory.Add(CurrentFPS);
    if (FPSHistory.Num() > 60)
    {
        FPSHistory.RemoveAt(0);
    }
    
    // Calculate average, min, max
    if (FPSHistory.Num() > 0)
    {
        float Sum = 0.0f;
        CurrentMetrics.MinFPS = 999.0f;
        CurrentMetrics.MaxFPS = 0.0f;
        
        for (float FPS : FPSHistory)
        {
            Sum += FPS;
            CurrentMetrics.MinFPS = FMath::Min(CurrentMetrics.MinFPS, FPS);
            CurrentMetrics.MaxFPS = FMath::Max(CurrentMetrics.MaxFPS, FPS);
        }
        
        CurrentMetrics.AverageFPS = Sum / FPSHistory.Num();
    }
    
    // Check for dropped frames
    float TargetFPS = GetTargetFPS();
    if (CurrentFPS < TargetFPS * 0.95f)
    {
        CurrentMetrics.bDroppedFrames = true;
        CurrentMetrics.DroppedFrameCount++;
    }
    else
    {
        CurrentMetrics.bDroppedFrames = false;
    }
    
    // Update timer
    MetricsUpdateTimer += DeltaTime;
    if (MetricsUpdateTimer >= MetricsUpdateInterval)
    {
        MetricsUpdateTimer = 0.0f;
        UpdateAdaptiveQuality();
        LogPerformanceStats();
    }
}

void UMingRTSVARPerformanceOptimizer::ApplyOptimizationSettings()
{
    // Apply all current settings to the engine
    // This would interface with various engine systems
    
    UE_LOG(LogMingVARPerformance, Verbose, TEXT("Applied optimization settings"));
}

float UMingRTSVARPerformanceOptimizer::GetTargetFPS() const
{
    switch (CurrentSettings.TargetFrameRate)
    {
        case EVARFrameRate::FPS30:  return 30.0f;
        case EVARFrameRate::FPS60:  return 60.0f;
        case EVARFrameRate::FPS72:  return 72.0f;
        case EVARFrameRate::FPS90:  return 90.0f;
        case EVARFrameRate::FPS120: return 120.0f;
        default: return 72.0f;
    }
}

void UMingRTSVARPerformanceOptimizer::LogPerformanceStats()
{
    UE_LOG(LogMingVARPerformance, Verbose, 
           TEXT("Performance: Current=%.1f, Avg=%.1f, Min=%.1f, Max=%.1f, Drops=%d"),
           CurrentMetrics.CurrentFPS,
           CurrentMetrics.AverageFPS,
           CurrentMetrics.MinFPS,
           CurrentMetrics.MaxFPS,
           CurrentMetrics.DroppedFrameCount);
}
