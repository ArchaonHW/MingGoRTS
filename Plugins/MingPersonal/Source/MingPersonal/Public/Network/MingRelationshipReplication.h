#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRelationshipReplication.generated.h"

// Forward declarations
class UMingNetworkManager;
class UMingRelationshipManager;

// Replication modes
UENUM(BlueprintType)
enum class ERelationshipReplicationMode : uint8
{
    ServerAuthoritative     UMETA(DisplayName = "Server Authoritative"),
    ClientPredictive        UMETA(DisplayName = "Client Predictive"),
    Hybrid                  UMETA(DisplayName = "Hybrid"),
    EventDriven             UMETA(DisplayName = "Event Driven")
};

// Sync priorities
UENUM(BlueprintType)
enum class ERelationshipSyncPriority : uint8
{
    Low                     UMETA(DisplayName = "Low"),
    Medium                  UMETA(DisplayName = "Medium"),
    High                    UMETA(DisplayName = "High"),
    Critical                UMETA(DisplayName = "Critical")
};

// Network relationship data
USTRUCT()
struct FNetworkRelationshipData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RelationshipID;

    UPROPERTY()
    int32 SourcePlayerID;

    UPROPERTY()
    int32 TargetPlayerID;

    UPROPERTY()
    int32 RelationshipType;

    UPROPERTY()
    float RelationshipValue;

    UPROPERTY()
    FString LastInteractionTime;

    UPROPERTY()
    bool bIsActive;

    UPROPERTY()
    int32 Version;

    FNetworkRelationshipData()
        : RelationshipID(0)
        , SourcePlayerID(-1)
        , TargetPlayerID(-1)
        , RelationshipType(0)
        , RelationshipValue(0.0f)
        , LastInteractionTime(TEXT(""))
        , bIsActive(true)
        , Version(0)
    {}
};

// Network reputation data
USTRUCT()
struct FNetworkReputationData
{
    GENERATED_BODY()

    UPROPERTY()
    int32 PlayerID;

    UPROPERTY()
    FString RegionID;

    UPROPERTY()
    float ReputationValue;

    UPROPERTY()
    int32 ReputationTier;

    UPROPERTY()
    TArray<int32> ActiveEffects;

    UPROPERTY()
    int32 Version;

    FNetworkReputationData()
        : PlayerID(-1)
        , RegionID(TEXT(""))
        , ReputationValue(0.0f)
        , ReputationTier(0)
        , Version(0)
    {}
};

// Replication batch
USTRUCT()
struct FRelationshipReplicationBatch
{
    GENERATED_BODY()

    UPROPERTY()
    int32 BatchID;

    UPROPERTY()
    int32 SenderPlayerID;

    UPROPERTY()
    float Timestamp;

    UPROPERTY()
    TArray<FNetworkRelationshipData> RelationshipUpdates;

    UPROPERTY()
    TArray<FNetworkReputationData> ReputationUpdates;

    UPROPERTY()
    bool bIsFullSync;

    FRelationshipReplicationBatch()
        : BatchID(0)
        , SenderPlayerID(-1)
        , Timestamp(0.0f)
        , bIsFullSync(false)
    {}
};

// Sync statistics
USTRUCT(BlueprintType)
struct FRelationshipSyncStats
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    int32 TotalRelationshipsSynced;

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    int32 TotalReputationsSynced;

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    float AverageSyncTime;

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    int32 FailedSyncs;

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    int32 ConflictResolutions;

    UPROPERTY(BlueprintReadOnly, Category = "Sync")
    float LastSyncTimestamp;

    FRelationshipSyncStats()
        : TotalRelationshipsSynced(0)
        , TotalReputationsSynced(0)
        , AverageSyncTime(0.0f)
        , FailedSyncs(0)
        , ConflictResolutions(0)
        , LastSyncTimestamp(0.0f)
    {}
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRelationshipSyncCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelationshipSyncFailed, FString, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConflictDetected, int32, RelationshipID);

/**
 * ?��X��?網絡複製系統
 * 負責?��?人�X�中?�步?��X�聲?�數X */
UCLASS(ClassGroup = (MingGoRTS), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingRelationshipReplication : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipReplication();

    // Initialize/Shutdown
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void Initialize(UMingNetworkManager* InNetworkManager);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void Shutdown();

    // Server-side functions
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerInitializePlayerRelationships(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerUpdateRelationship(int32 RelationshipID, int32 SourcePlayerID, int32 TargetPlayerID, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerUpdateReputation(int32 PlayerID, const FString& RegionID, float NewValue);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    FRelationshipReplicationBatch ServerCreateReplicationBatch(int32 TargetPlayerID, bool bFullSync = false);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerBroadcastToAllPlayers(const FRelationshipReplicationBatch& Batch);

    // Client-side functions
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientRequestFullSync();

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientProcessReplicationBatch(const FRelationshipReplicationBatch& Batch);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientSendRelationshipUpdate(int32 RelationshipID, float DeltaValue);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientSendReputationUpdate(const FString& RegionID, float DeltaValue);

    // Conflict resolution
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ResolveConflict_ServerAuthoritative(int32 RelationshipID);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ResolveConflict_ClientWins(int32 RelationshipID);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ResolveConflict_MergeValues(int32 RelationshipID, float MergeWeight);

    // Sync management
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void StartPeriodicSync(float SyncInterval = 5.0f);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void StopPeriodicSync();

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ForceImmediateSync();

    // Settings
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void SetReplicationMode(ERelationshipReplicationMode NewMode);

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    ERelationshipReplicationMode GetReplicationMode() const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void SetSyncPriority(int32 RelationshipID, ERelationshipSyncPriority Priority);

    // Statistics
    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    FRelationshipSyncStats GetSyncStats() const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ResetSyncStats();

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void GetDetailedSyncReport(FString& OutReport) const;

    // Validation
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    bool ValidateRelationshipData(const FNetworkRelationshipData& Data) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    bool ValidateReputationData(const FNetworkReputationData& Data) const;

    // Utility
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void CompressReplicationBatch(FRelationshipReplicationBatch& Batch);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void DecompressReplicationBatch(FRelationshipReplicationBatch& Batch);

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Replication Events")
    FOnRelationshipSyncCompleted OnSyncCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Replication Events")
    FOnRelationshipSyncFailed OnSyncFailed;

    UPROPERTY(BlueprintAssignable, Category = "Replication Events")
    FOnConflictDetected OnConflictDetected;

private:
    UPROPERTY()
    UMingNetworkManager* NetworkManager;

    UPROPERTY()
    ERelationshipReplicationMode ReplicationMode;

    UPROPERTY()
    FRelationshipSyncStats SyncStats;

    UPROPERTY()
    TMap<int32, ERelationshipSyncPriority> RelationshipPriorities;

    UPROPERTY()
    TMap<int32, FNetworkRelationshipData> CachedRelationships;

    UPROPERTY()
    TMap<int32, FNetworkReputationData> CachedReputations;

    UPROPERTY()
    FTimerHandle PeriodicSyncTimer;

    UPROPERTY()
    bool bIsPeriodicSyncActive;

    UPROPERTY()
    float LastFullSyncTime;

    // Internal functions
    void PerformPeriodicSync();
    void SendBatchToPlayer(int32 PlayerID, const FRelationshipReplicationBatch& Batch);
    void ApplyRelationshipUpdate(const FNetworkRelationshipData& Update);
    void ApplyReputationUpdate(const FNetworkReputationData& Update);
    bool CheckForConflicts(const FNetworkRelationshipData& Incoming, const FNetworkRelationshipData& Existing);
    int32 GenerateBatchID();
    void RecordSyncSuccess(float SyncTime);
    void RecordSyncFailure(const FString& Reason);
    void UpdateCache(const FRelationshipReplicationBatch& Batch);
};

