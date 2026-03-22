#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Relationship/MingRelationshipTypes.h"
#include "MingRelationshipReplication.generated.h"

UENUM(BlueprintType)
enum class EMingRelationshipSyncMode : uint8
{
    FullSync UMETA(DisplayName = "Full Sync"),
    DeltaSync UMETA(DisplayName = "Delta Sync"),
    EventDriven UMETA(DisplayName = "Event Driven"),
    ProximityBased UMETA(DisplayName = "Proximity Based")
};

USTRUCT(BlueprintType)
struct FMingReplicatedRelationshipData
{
    GENERATED_BODY()

    UPROPERTY()
    FName CharacterID;

    UPROPERTY()
    float RelationshipValue;

    UPROPERTY()
    EMingRelationshipType RelationshipType;

    UPROPERTY()
    int32 RelationshipLevel;

    UPROPERTY()
    TArray<FString> ActiveModifiers;

    UPROPERTY()
    int32 OwnerPlayerID;

    UPROPERTY()
    float LastSyncTime;

    UPROPERTY()
    int32 SyncVersion;

    FMingReplicatedRelationshipData()
        : RelationshipValue(50.0f)
        , RelationshipType(EMingRelationshipType::None)
        , RelationshipLevel(0)
        , OwnerPlayerID(-1)
        , LastSyncTime(0.0f)
        , SyncVersion(0)
    {}

    bool IsValid() const
    {
        return !CharacterID.IsNone() && OwnerPlayerID >= 0;
    }
};

USTRUCT(BlueprintType)
struct FMingRelationshipSyncDelta
{
    GENERATED_BODY()

    UPROPERTY()
    int32 DeltaType;

    UPROPERTY()
    FName CharacterID;

    UPROPERTY()
    float OldValue;

    UPROPERTY()
    float NewValue;

    UPROPERTY()
    FString ChangeReason;

    UPROPERTY()
    int32 SourcePlayerID;

    UPROPERTY()
    float Timestamp;

    FMingRelationshipSyncDelta()
        : DeltaType(0)
        , OldValue(0.0f)
        , NewValue(0.0f)
        , SourcePlayerID(-1)
        , Timestamp(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingRelationshipSyncRequest
{
    GENERATED_BODY()

    UPROPERTY()
    int32 RequestingPlayerID;

    UPROPERTY()
    TArray<FName> RequestedCharacters;

    UPROPERTY()
    bool bFullSync;

    UPROPERTY()
    int32 LastKnownVersion;

    UPROPERTY()
    float RequestTimestamp;

    FMingRelationshipSyncRequest()
        : RequestingPlayerID(-1)
        , bFullSync(false)
        , LastKnownVersion(0)
        , RequestTimestamp(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingRelationshipSyncResponse
{
    GENERATED_BODY()

    UPROPERTY()
    int32 TargetPlayerID;

    UPROPERTY()
    TArray<FMingReplicatedRelationshipData> RelationshipData;

    UPROPERTY()
    TArray<FMingRelationshipSyncDelta> Deltas;

    UPROPERTY()
    int32 ServerVersion;

    UPROPERTY()
    float ServerTimestamp;

    UPROPERTY()
    bool bIsIncremental;

    FMingRelationshipSyncResponse()
        : TargetPlayerID(-1)
        , ServerVersion(0)
        , ServerTimestamp(0.0f)
        , bIsIncremental(false)
    {}
};

/**
 * Multiplayer Relationship Replication Manager
 * Handles syncing relationship data across network clients
 */
UCLASS(ClassGroup = (Multiplayer, Relationship))
class MINGMULTIPLAYER_API UMingRelationshipReplication : public UObject
{
    GENERATED_BODY()

public:
    UMingRelationshipReplication();

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void InitializeReplication(class UMingNetworkManager* NetworkManager, class UMingRelationshipManager* RelationshipManager);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ShutdownReplication();

    // Sync Configuration
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void SetSyncMode(EMingRelationshipSyncMode Mode);

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    EMingRelationshipSyncMode GetSyncMode() const { return CurrentSyncMode; }

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void SetSyncInterval(float IntervalSeconds);

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    float GetSyncInterval() const { return SyncInterval; }

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void SetProximityRadius(float Radius);

    // Manual Sync
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void RequestFullSync(int32 TargetPlayerID);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void RequestPartialSync(const TArray<FName>& CharacterIDs);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ForceSyncAll();

    // Server-side Operations
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerUpdateRelationship(int32 PlayerID, FName CharacterID, float NewValue, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerAddRelationshipModifier(int32 PlayerID, FName CharacterID, const FString& Modifier, float Value);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ServerRemoveRelationshipModifier(int32 PlayerID, FName CharacterID, const FString& Modifier);

    // Client-side Operations
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientApplySyncData(const FMingRelationshipSyncResponse& SyncData);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ClientApplyDelta(const FMingRelationshipSyncDelta& Delta);

    // Validation
    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    bool ValidateSyncData(const FMingRelationshipSyncResponse& SyncData) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    bool ValidateDelta(const FMingRelationshipSyncDelta& Delta) const;

    // Conflict Resolution
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void ResolveConflict(const FMingReplicatedRelationshipData& LocalData, const FMingReplicatedRelationshipData& RemoteData);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    FMingReplicatedRelationshipData MergeRelationshipData(const TArray<FMingReplicatedRelationshipData>& DataVersions);

    // Statistics
    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    int32 GetTotalSyncOperations() const { return TotalSyncOperations; }

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    int32 GetFailedSyncOperations() const { return FailedSyncOperations; }

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    float GetAverageSyncLatency() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    int32 GetPendingSyncCount() const;

    // Utilities
    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void CompressSyncData(FMingRelationshipSyncResponse& SyncData);

    UFUNCTION(BlueprintCallable, Category = "Relationship Replication")
    void DecompressSyncData(FMingRelationshipSyncResponse& SyncData);

    UFUNCTION(BlueprintPure, Category = "Relationship Replication")
    int32 CalculateSyncDataSize(const FMingRelationshipSyncResponse& SyncData) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRelationshipSynced, int32, PlayerID);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSyncConflict, const FMingReplicatedRelationshipData&, Local, const FMingReplicatedRelationshipData&, Remote);
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSyncError, const FString&, ErrorMessage);

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRelationshipSynced OnRelationshipSynced;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSyncConflict OnSyncConflict;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnSyncError OnSyncError;

protected:
    UPROPERTY()
    TObjectPtr<UMingNetworkManager> NetworkMgr;

    UPROPERTY()
    TObjectPtr<UMingRelationshipManager> RelationshipMgr;

    UPROPERTY()
    EMingRelationshipSyncMode CurrentSyncMode;

    UPROPERTY()
    float SyncInterval;

    UPROPERTY()
    float ProximityRadius;

    UPROPERTY()
    float LastSyncTime;

    UPROPERTY()
    int32 SyncVersion;

    UPROPERTY()
    TMap<int32, FMingReplicatedRelationshipData> CachedRelationshipData;

    UPROPERTY()
    TArray<FMingRelationshipSyncDelta> PendingDeltas;

    UPROPERTY()
    int32 TotalSyncOperations;

    UPROPERTY()
    int32 FailedSyncOperations;

    UPROPERTY()
    TArray<float> SyncLatencies;

    // Internal Functions
    void PerformFullSync();
    void PerformDeltaSync();
    void PerformEventDrivenSync();
    void PerformProximitySync();
    
    void SendSyncRequest(const FMingRelationshipSyncRequest& Request);
    void ProcessSyncResponse(const FMingRelationshipSyncResponse& Response);
    
    void TrackSyncLatency(float Latency);
    void RecordSyncOperation(bool bSuccess);
    
    bool ShouldSyncCharacter(FName CharacterID) const;
    bool IsInProximity(FName CharacterID) const;
    
    FMingRelationshipSyncRequest BuildSyncRequest(int32 PlayerID, bool bFullSync);
    FMingRelationshipSyncResponse BuildSyncResponse(const FMingRelationshipSyncRequest& Request);
    
    void ApplyDeltaToLocalData(const FMingRelationshipSyncDelta& Delta);
    void BroadcastDeltaToClients(const FMingRelationshipSyncDelta& Delta);
    
    void OnNetworkTick(float DeltaTime);
    void OnPlayerJoined(int32 PlayerID);
    void OnPlayerLeft(int32 PlayerID);
    void OnRelationshipChanged(FName CharacterID, float OldValue, float NewValue);

    // Serialization
    TArray<uint8> SerializeSyncData(const FMingRelationshipSyncResponse& SyncData);
    FMingRelationshipSyncResponse DeserializeSyncData(const TArray<uint8>& Data);
    
    TArray<uint8> SerializeDelta(const FMingRelationshipSyncDelta& Delta);
    FMingRelationshipSyncDelta DeserializeDelta(const TArray<uint8>& Data);
};
