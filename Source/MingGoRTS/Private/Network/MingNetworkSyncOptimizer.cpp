// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Network Synchronization Optimizer Implementation - B1-1

#include "Network/MingNetworkSyncOptimizer.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogNetworkSync, Log, All);

UMingNetworkSyncOptimizer::UMingNetworkSyncOptimizer()
    : CurrentBandwidthUsage(0.0f)
    , AverageLatency(0.0f)
    , PacketLossRate(0.0f)
    , bOptimizationEnabled(false)
    , CurrentPredictionMode(EPredictionMode::Linear)
    , bDeltaCompressionEnabled(true)
    , TotalPacketsSent(0)
    , TotalPacketsLost(0)
    , CurrentSequenceNumber(0)
{
}

void UMingNetworkSyncOptimizer::InitializeSyncOptimizer(const FSyncOptimizationConfig& Config)
{
    CurrentConfig = Config;
    bOptimizationEnabled = true;
    bDeltaCompressionEnabled = Config.bUseDeltaCompression;
    CurrentPredictionMode = Config.bEnablePrediction ? EPredictionMode::Linear : EPredictionMode::None;

    // Start optimization timers
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            HeartbeatTimer,
            this,
            &UMingNetworkSyncOptimizer::ProcessSyncQueue,
            CurrentConfig.SyncInterval,
            true
        );
    }

    UE_LOG(LogNetworkSync, Log, TEXT("Network Sync Optimizer initialized with interval: %.3f"), Config.SyncInterval);
}

void UMingNetworkSyncOptimizer::ShutdownSyncOptimizer()
{
    bOptimizationEnabled = false;

    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(HeartbeatTimer);
    }

    SyncQueue.Empty();
    PriorityQueue.Empty();
    ObjectPriorities.Empty();
    LastSyncedData.Empty();

    UE_LOG(LogNetworkSync, Log, TEXT("Network Sync Optimizer shutdown"));
}

void UMingNetworkSyncOptimizer::RegisterSyncObject(uint32 ObjectID, ESyncPriority Priority)
{
    ObjectPriorities.Add(ObjectID, Priority);
    UE_LOG(LogNetworkSync, Verbose, TEXT("Registered sync object %u with priority %d"), ObjectID, static_cast<int32>(Priority));
}

void UMingNetworkSyncOptimizer::UnregisterSyncObject(uint32 ObjectID)
{
    ObjectPriorities.Remove(ObjectID);
    LastSyncedData.Remove(ObjectID);
    UE_LOG(LogNetworkSync, Verbose, TEXT("Unregistered sync object %u"), ObjectID);
}

void UMingNetworkSyncOptimizer::QueueSyncData(const FNetworkSyncData& SyncData)
{
    FScopeLock Lock(&SyncMutex);

    // Check if we should filter based on distance
    if (CurrentConfig.MaxSyncDistance > 0.0f)
    {
        // Distance filtering logic would go here
    }

    // Add to appropriate queue based on priority
    if (ObjectPriorities.Contains(SyncData.ObjectID))
    {
        ESyncPriority Priority = ObjectPriorities[SyncData.ObjectID];
        if (Priority == ESyncPriority::Critical || Priority == ESyncPriority::High)
        {
            PriorityQueue.Add(SyncData);
        }
        else
        {
            SyncQueue.Add(SyncData);
        }
    }
    else
    {
        SyncQueue.Add(SyncData);
    }

    // Apply delta compression if enabled
    if (bDeltaCompressionEnabled)
    {
        ApplyDeltaCompression(SyncQueue.Last());
    }
}

void UMingNetworkSyncOptimizer::SetObjectPriority(uint32 ObjectID, ESyncPriority NewPriority)
{
    if (ObjectPriorities.Contains(ObjectID))
    {
        ObjectPriorities[ObjectID] = NewPriority;
    }
}

float UMingNetworkSyncOptimizer::GetCurrentBandwidthUsage() const
{
    return CurrentBandwidthUsage;
}

float UMingNetworkSyncOptimizer::GetAverageLatency() const
{
    return GetSmoothedLatency();
}

float UMingNetworkSyncOptimizer::GetPacketLossRate() const
{
    if (TotalPacketsSent > 0)
    {
        return (TotalPacketsLost / static_cast<float>(TotalPacketsSent)) * 100.0f;
    }
    return 0.0f;
}

void UMingNetworkSyncOptimizer::EnableDeltaCompression(bool bEnable)
{
    bDeltaCompressionEnabled = bEnable;
    UE_LOG(LogNetworkSync, Log, TEXT("Delta compression %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingNetworkSyncOptimizer::SetPredictionMode(EPredictionMode Mode)
{
    CurrentPredictionMode = Mode;
    UE_LOG(LogNetworkSync, Log, TEXT("Prediction mode set to: %s"), *UEnum::GetValueAsString(Mode));
}

void UMingNetworkSyncOptimizer::OptimizeSyncRate(float TargetBandwidth)
{
    if (TargetBandwidth <= 0.0f) return;

    CurrentConfig.NetworkBandwidthLimit = TargetBandwidth;

    // Adjust sync interval based on bandwidth
    if (CurrentBandwidthUsage > TargetBandwidth * 0.9f)
    {
        CurrentConfig.SyncInterval *= 1.1f; // Slow down
        UE_LOG(LogNetworkSync, Warning, TEXT("Bandwidth limit approaching. Increasing sync interval to %.3f"), CurrentConfig.SyncInterval);
    }
    else if (CurrentBandwidthUsage < TargetBandwidth * 0.5f)
    {
        CurrentConfig.SyncInterval = FMath::Max(CurrentConfig.SyncInterval * 0.9f, 0.016f); // Speed up
    }

    OnSyncOptimized.Broadcast(TargetBandwidth - CurrentBandwidthUsage);
}

void UMingNetworkSyncOptimizer::SetDistanceBasedPriority(const FVector& ObserverLocation)
{
    InterestBasedFiltering(ObserverLocation);
}

void UMingNetworkSyncOptimizer::EnableAdaptiveSync(bool bEnable)
{
    CurrentConfig.bUseAdaptiveSyncRate = bEnable;
    UE_LOG(LogNetworkSync, Log, TEXT("Adaptive sync %s"), bEnable ? TEXT("enabled") : TEXT("disabled"));
}

void UMingNetworkSyncOptimizer::SetInterestManagement(bool bEnable, float InterestRadius)
{
    CurrentConfig.MaxSyncDistance = bEnable ? InterestRadius : 0.0f;
    UE_LOG(LogNetworkSync, Log, TEXT("Interest management %s, radius: %.1f"), bEnable ? TEXT("enabled") : TEXT("disabled"), InterestRadius);
}

void UMingNetworkSyncOptimizer::ConfigureLODLevel(uint32 ObjectID, int32 LODLevel)
{
    // LOD configuration affects sync frequency
    if (ObjectPriorities.Contains(ObjectID))
    {
        // Lower LOD = lower sync priority
        ESyncPriority NewPriority = static_cast<ESyncPriority>(FMath::Min(LODLevel, 4));
        SetObjectPriority(ObjectID, NewPriority);
    }
}

void UMingNetworkSyncOptimizer::ProcessSyncQueue()
{
    if (!bOptimizationEnabled) return;

    FScopeLock Lock(&SyncMutex);

    // Process priority queue first
    int32 ProcessedCount = 0;
    while (PriorityQueue.Num() > 0 && ProcessedCount < CurrentConfig.MaxObjectsPerFrame / 2)
    {
        FNetworkSyncData Data = PriorityQueue[0];
        PriorityQueue.RemoveAt(0);

        // Send to network
        UpdateBandwidthMetrics(sizeof(FNetworkSyncData));
        ProcessedCount++;

        // Predict movement if enabled
        if (CurrentPredictionMode != EPredictionMode::None)
        {
            PredictMovement(Data.ObjectID, Data);
        }

        // Store for delta compression
        LastSyncedData.Add(Data.ObjectID, Data);
    }

    // Process regular queue
    while (SyncQueue.Num() > 0 && ProcessedCount < CurrentConfig.MaxObjectsPerFrame)
    {
        FNetworkSyncData Data = SyncQueue[0];
        SyncQueue.RemoveAt(0);

        // Check if still relevant
        if (ObjectPriorities.Contains(Data.ObjectID) && ObjectPriorities[Data.ObjectID] == ESyncPriority::Background)
        {
            continue; // Skip background priority objects when busy
        }

        UpdateBandwidthMetrics(sizeof(FNetworkSyncData));
        ProcessedCount++;

        LastSyncedData.Add(Data.ObjectID, Data);
    }

    // Update metrics
    TotalPacketsSent += ProcessedCount;
    DetectNetworkIssues();

    // Adaptive sync rate adjustment
    if (CurrentConfig.bUseAdaptiveSyncRate)
    {
        AdaptiveSyncRateAdjustment();
    }
}

void UMingNetworkSyncOptimizer::ApplyDeltaCompression(FNetworkSyncData& Data)
{
    if (LastSyncedData.Contains(Data.ObjectID))
    {
        FNetworkSyncData& LastData = LastSyncedData[Data.ObjectID];

        // Only send differences
        FVector DeltaLocation = FVector(Data.Location) - FVector(LastData.Location);
        if (DeltaLocation.SizeSquared() < 1.0f)
        {
            Data.Location = LastData.Location; // No significant change
        }

        // Compress rotation if minimal change
        if (FMath::Abs(Data.RotationPitch - LastData.RotationPitch) < 2 &&
            FMath::Abs(Data.RotationYaw - LastData.RotationYaw) < 2)
        {
            Data.RotationPitch = LastData.RotationPitch;
            Data.RotationYaw = LastData.RotationYaw;
        }
    }
}

void UMingNetworkSyncOptimizer::PredictMovement(uint32 ObjectID, FNetworkSyncData& Data)
{
    if (CurrentPredictionMode == EPredictionMode::Linear && LastSyncedData.Contains(ObjectID))
    {
        FVector CurrentLoc = FVector(Data.Location);
        FVector Velocity = FVector(Data.Velocity);

        // Simple linear prediction
        FVector PredictedLoc = CurrentLoc + Velocity * CurrentConfig.SyncInterval;

        // Store prediction for comparison
        Data.CustomData.Empty();
        Data.CustomData.Add(static_cast<uint8>(PredictedLoc.X));
        Data.CustomData.Add(static_cast<uint8>(PredictedLoc.Y));
        Data.CustomData.Add(static_cast<uint8>(PredictedLoc.Z));
    }
}

void UMingNetworkSyncOptimizer::UpdateBandwidthMetrics(int32 BytesSent)
{
    // Simple moving average
    float BytesPerSecond = BytesSent / CurrentConfig.SyncInterval;
    CurrentBandwidthUsage = CurrentBandwidthUsage * 0.9f + BytesPerSecond * 0.1f;
}

void UMingNetworkSyncOptimizer::DetectNetworkIssues()
{
    // Check for packet loss spikes
    float CurrentPacketLoss = GetPacketLossRate();
    if (CurrentPacketLoss > 5.0f)
    {
        OnPacketLossDetected.Broadcast(CurrentPacketLoss);
    }

    // Check for latency spikes
    float CurrentLatency = GetAverageLatency();
    if (CurrentLatency > 100.0f)
    {
        OnLatencySpike.Broadcast(static_cast<int32>(CurrentLatency));
    }
}

float UMingNetworkSyncOptimizer::CalculateSyncPriority(uint32 ObjectID) const
{
    if (ObjectPriorities.Contains(ObjectID))
    {
        return static_cast<float>(ObjectPriorities[ObjectID]);
    }
    return 2.0f; // Default medium priority
}

void UMingNetworkSyncOptimizer::AdaptiveSyncRateAdjustment()
{
    // Adjust sync rate based on network conditions
    if (PacketLossRate > 3.0f)
    {
        // Network struggling, reduce sync rate
        CurrentConfig.MaxObjectsPerFrame = FMath::Max(CurrentConfig.MaxObjectsPerFrame - 5, 20);
    }
    else if (PacketLossRate < 1.0f && CurrentBandwidthUsage < CurrentConfig.NetworkBandwidthLimit * 0.7f)
    {
        // Network good, can increase sync rate
        CurrentConfig.MaxObjectsPerFrame = FMath::Min(CurrentConfig.MaxObjectsPerFrame + 2, 200);
    }
}

void UMingNetworkSyncOptimizer::InterestBasedFiltering(const FVector& ObserverLocation)
{
    // Mark objects outside interest radius as background priority
    for (auto& Pair : ObjectPriorities)
    {
        uint32 ObjectID = Pair.Key;
        // In real implementation, would check actual distance to object
        // For now, using random assignment for demonstration
        if (FMath::RandRange(0, 100) > 70)
        {
            Pair.Value = ESyncPriority::Background;
        }
    }
}

bool UMingNetworkSyncOptimizer::ShouldSyncObject(uint32 ObjectID, const FVector& ObserverLocation) const
{
    if (CurrentConfig.MaxSyncDistance <= 0.0f) return true;

    // Check if object is within sync distance
    // Would need actual object location for real implementation
    return true;
}

void UMingNetworkSyncOptimizer::CompressPacket(TArray<uint8>& PacketData)
{
    // Implement compression algorithm
    UE_LOG(LogNetworkSync, Verbose, TEXT("Compressing packet of size %d"), PacketData.Num());
}

void UMingNetworkSyncOptimizer::DecompressPacket(const TArray<uint8>& CompressedData, TArray<uint8>& OutData)
{
    // Implement decompression algorithm
    OutData = CompressedData;
}

void UMingNetworkSyncOptimizer::RecordLatencyMeasurement(float Latency)
{
    LatencyHistory.Add(Latency);
    if (LatencyHistory.Num() > 100)
    {
        LatencyHistory.RemoveAt(0);
    }
}

float UMingNetworkSyncOptimizer::GetSmoothedLatency() const
{
    if (LatencyHistory.Num() == 0) return 0.0f;

    float Sum = 0.0f;
    for (float Latency : LatencyHistory)
    {
        Sum += Latency;
    }
    return Sum / LatencyHistory.Num();
}

void UMingNetworkSyncOptimizer::HandlePacketLoss()
{
    TotalPacketsLost++;
    UE_LOG(LogNetworkSync, Warning, TEXT("Packet loss detected. Total lost: %d"), TotalPacketsLost);
}

void UMingNetworkSyncOptimizer::RebuildSyncState()
{
    // Rebuild synchronization state after significant packet loss
    UE_LOG(LogNetworkSync, Log, TEXT("Rebuilding sync state"));
    LastSyncedData.Empty();
}

UMingNetworkSyncOptimizer* UMingNetworkSyncOptimizer::Get(UObject* WorldContextObject)
{
    static UMingNetworkSyncOptimizer* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingNetworkSyncOptimizer>();
        Instance->AddToRoot();
    }
    return Instance;
}
