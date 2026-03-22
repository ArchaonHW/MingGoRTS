// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Network Synchronization Optimizer - B1-1
// Optimizes network synchronization for multiplayer games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/Engine.h"
#include "Networking.h"
#include "MingNetworkSyncOptimizer.generated.h"

UENUM(BlueprintType)
enum class ESyncPriority : uint8
{
    Critical = 0, UMETA(DisplayName = "Critical - Immediate Sync"),
    High, UMETA(DisplayName = "High - Within 1 Frame"),
    Medium, UMETA(DisplayName = "Medium - Within 5 Frames"),
    Low, UMETA(DisplayName = "Low - Within 30 Frames"),
    Background UMETA(DisplayName = "Background - On Demand")
};

UENUM(BlueprintType)
enum class ENetworkCompression : uint8
{
    None = 0, UMETA(DisplayName = "No Compression"),
    Delta, UMETA(DisplayName = "Delta Compression"),
    LZ4, UMETA(DisplayName = "LZ4 Compression"),
    Adaptive UMETA(DisplayName = "Adaptive Compression")
};

UENUM(BlueprintType)
enum class EPredictionMode : uint8
{
    None = 0, UMETA(DisplayName = "No Prediction"),
    Linear, UMETA(DisplayName = "Linear Extrapolation"),
    Advanced, UMETA(DisplayName = "Advanced Prediction"),
    MachineLearning UMETA(DisplayName = "ML-Based Prediction")
};

USTRUCT(BlueprintType)
struct FNetworkSyncData
{
    GENERATED_BODY()

    UPROPERTY()
    uint32 ObjectID;

    UPROPERTY()
    FVector_NetQuantize Location;

    UPROPERTY()
    FVector_NetQuantize Velocity;

    UPROPERTY()
    uint8 RotationPitch;

    UPROPERTY()
    uint8 RotationYaw;

    UPROPERTY()
    uint32 Timestamp;

    UPROPERTY()
    uint16 SequenceNumber;

    UPROPERTY()
    uint8 SyncPriority;

    UPROPERTY()
    TArray<uint8> CustomData;

    FNetworkSyncData()
        : ObjectID(0)
        , Timestamp(0)
        , SequenceNumber(0)
        , SyncPriority(2)
    {}
};

USTRUCT(BlueprintType)
struct FSyncOptimizationConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    float SyncInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    int32 MaxObjectsPerFrame;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    float NetworkBandwidthLimit;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    bool bUseDeltaCompression;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    bool bEnablePrediction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    bool bUseAdaptiveSyncRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    float HighPriorityDistance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sync Optimization")
    float MaxSyncDistance;

    FSyncOptimizationConfig()
        : SyncInterval(0.033f)
        , MaxObjectsPerFrame(100)
        , NetworkBandwidthLimit(1024.0f)
        , bUseDeltaCompression(true)
        , bEnablePrediction(true)
        , bUseAdaptiveSyncRate(true)
        , HighPriorityDistance(1000.0f)
        , MaxSyncDistance(5000.0f)
    {}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSyncOptimized, float, BandwidthSaved};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPacketLossDetected, float, PacketLossRate};
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLatencySpike, int32, LatencyMs};
/**
 * Network Synchronization Optimizer
 * Optimizes network traffic for multiplayer games
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingNetworkSyncOptimizer : public UObject
{
    GENERATED_BODY()

public:
    UMingNetworkSyncOptimizer(};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void InitializeSyncOptimizer(const FSyncOptimizationConfig& Config};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void ShutdownSyncOptimizer(};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void RegisterSyncObject(uint32 ObjectID, ESyncPriority Priority};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void UnregisterSyncObject(uint32 ObjectID};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void QueueSyncData(const FNetworkSyncData& SyncData};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void SetObjectPriority(uint32 ObjectID, ESyncPriority NewPriority};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    float GetCurrentBandwidthUsage() const;

    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    float GetAverageLatency() const;

    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    float GetPacketLossRate() const;

    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void EnableDeltaCompression(bool bEnable};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void SetPredictionMode(EPredictionMode Mode};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void OptimizeSyncRate(float TargetBandwidth};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void SetDistanceBasedPriority(const FVector& ObserverLocation};
    UFUNCTION(BlueprintPure, Category = "Network Sync")
    int32 GetSyncQueueSize() const { return SyncQueue.Num(); }

    UFUNCTION(BlueprintPure, Category = "Network Sync")
    bool IsSyncOptimized() const { return bOptimizationEnabled; }

    // Advanced optimization features
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void EnableAdaptiveSync(bool bEnable};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void SetInterestManagement(bool bEnable, float InterestRadius};
    UFUNCTION(BlueprintCallable, Category = "Network Sync")
    void ConfigureLODLevel(uint32 ObjectID, int32 LODLevel};
    UPROPERTY(BlueprintAssignable, Category = "Network Sync Events")
    FOnSyncOptimized OnSyncOptimized;

    UPROPERTY(BlueprintAssignable, Category = "Network Sync Events")
    FOnPacketLossDetected OnPacketLossDetected;

    UPROPERTY(BlueprintAssignable, Category = "Network Sync Events")
    FOnLatencySpike OnLatencySpike;

protected:
    UPROPERTY()
    FSyncOptimizationConfig CurrentConfig;

    UPROPERTY()
    TMap<uint32, ESyncPriority> ObjectPriorities;

    UPROPERTY()
    TArray<FNetworkSyncData> SyncQueue;

    UPROPERTY()
    TArray<FNetworkSyncData> PriorityQueue;

    UPROPERTY()
    TMap<uint32, FNetworkSyncData> LastSyncedData;

    UPROPERTY()
    float CurrentBandwidthUsage;

    UPROPERTY()
    float AverageLatency;

    UPROPERTY()
    float PacketLossRate;

    UPROPERTY()
    bool bOptimizationEnabled;

    UPROPERTY()
    EPredictionMode CurrentPredictionMode;

    UPROPERTY()
    bool bDeltaCompressionEnabled;

    UPROPERTY()
    int32 TotalPacketsSent;

    UPROPERTY()
    int32 TotalPacketsLost;

    UPROPERTY()
    TArray<float> LatencyHistory;

    UPROPERTY()
    int32 CurrentSequenceNumber;

    void ProcessSyncQueue(};
    void ApplyDeltaCompression(FNetworkSyncData& Data};
    void PredictMovement(uint32 ObjectID, FNetworkSyncData& Data};
    void UpdateBandwidthMetrics(int32 BytesSent};
    void DetectNetworkIssues(};
    float CalculateSyncPriority(uint32 ObjectID) const;
    void AdaptiveSyncRateAdjustment(};
    void InterestBasedFiltering(const FVector& ObserverLocation};
    bool ShouldSyncObject(uint32 ObjectID, const FVector& ObserverLocation) const;
    void CompressPacket(TArray<uint8>& PacketData};
    void DecompressPacket(const TArray<uint8>& CompressedData, TArray<uint8>& OutData};
    void RecordLatencyMeasurement(float Latency};
    float GetSmoothedLatency() const;
    void HandlePacketLoss(};
    void RebuildSyncState(};
    static UMingNetworkSyncOptimizer* Get(UObject* WorldContextObject};
};
