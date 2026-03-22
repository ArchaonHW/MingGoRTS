#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Reputation/MingReputationTypes.h"
#include "MingReputationReplication.generated.h"

UENUM(BlueprintType)
enum class EMingReputationSyncMode : uint8
{
    FullSync UMETA(DisplayName = "Full Sync"),
    DeltaSync UMETA(DisplayName = "Delta Sync"),
    EventDriven UMETA(DisplayName = "Event Driven"),
    RegionalOnly UMETA(DisplayName = "Regional Only")
};

USTRUCT(BlueprintType)
struct FMingReplicatedReputationData
{
    GENERATED_BODY()

    UPROPERTY()
    FName RegionID;

    UPROPERTY()
    float ReputationValue;

    UPROPERTY()
    int32 ReputationRank;

    UPROPERTY()
    TArray<FString> ActiveQuests;

    UPROPERTY()
    TArray<FString> CompletedQuests;

    UPROPERTY()
    TArray<FString> ActiveEffects;

    UPROPERTY()
    int32 OwnerPlayerID;

    UPROPERTY()
    float LastSyncTime;

    UPROPERTY()
    int32 SyncVersion;

    FMingReplicatedReputationData()
        : ReputationValue(50.0f)
        , ReputationRank(0)
        , OwnerPlayerID(-1)
        , LastSyncTime(0.0f)
        , SyncVersion(0)
    {}

    bool IsValid() const
    {
        return !RegionID.IsNone() && OwnerPlayerID >= 0;
    }
};

USTRUCT(BlueprintType)
struct FMingReputationSyncDelta
{
    GENERATED_BODY()

    UPROPERTY()
    int32 DeltaType;

    UPROPERTY()
    FName RegionID;

    UPROPERTY()
    float OldValue;

    UPROPERTY()
    float NewValue;

    UPROPERTY()
    int32 OldRank;

    UPROPERTY()
    int32 NewRank;

    UPROPERTY()
    FString ChangeReason;

    UPROPERTY()
    int32 SourcePlayerID;

    UPROPERTY()
    float Timestamp;

    FMingReputationSyncDelta()
        : DeltaType(0)
        , OldValue(0.0f)
        , NewValue(0.0f)
        , OldRank(0)
        , NewRank(0)
        , SourcePlayerID(-1)
        , Timestamp(0.0f)
    {}
};

USTRUCT(BlueprintType)
struct FMingRegionalSyncData
{
    GENERATED_BODY()

    UPROPERTY()
    FName RegionID;

    UPROPERTY()
    TArray<FMingReplicatedReputationData> PlayerReputations;

    UPROPERTY()
    float RegionAverageReputation;

    UPROPERTY()
    int32 TotalPlayersInRegion;

    UPROPERTY()
    int32 AllyCount;

    UPROPERTY()
    int32 HostileCount;

    FMingRegionalSyncData()
        : RegionAverageReputation(50.0f)
        , TotalPlayersInRegion(0)
        , AllyCount(0)
        , HostileCount(0)
    {}
};

/**
 * Multiplayer Reputation Replication Manager
 * Handles syncing reputation data across network clients
 */
UCLASS(ClassGroup = (Multiplayer, Reputation))
class MINGMULTIPLAYER_API UMingReputationReplication : public UObject
{
    GENERATED_BODY()

public:
    UMingReputationReplication();

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void InitializeReplication(class UMingNetworkManager* NetworkManager, class UMingReputationManager* ReputationManager};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ShutdownReplication();

    // Sync Configuration
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void SetSyncMode(EMingReputationSyncMode Mode};

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    EMingReputationSyncMode GetSyncMode() const { return CurrentSyncMode; }

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void SetSyncInterval(float IntervalSeconds};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void SetRegionalSyncRadius(float Radius};

    // Manual Sync
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void RequestFullSync(int32 TargetPlayerID};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void RequestRegionalSync(const TArray<FName>& RegionIDs};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ForceSyncAllRegions();

    // Server-side Operations
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ServerUpdateReputation(int32 PlayerID, FName RegionID, float NewValue, const FString& Reason};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ServerUpdateReputationRank(int32 PlayerID, FName RegionID, int32 NewRank};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ServerCompleteQuest(int32 PlayerID, FName RegionID, const FString& QuestID};

    // Client-side Operations
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ClientApplyReputationData(const TArray<FMingReplicatedReputationData>& ReputationData};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ClientApplyReputationDelta(const FMingReputationSyncDelta& Delta};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ClientApplyRegionalData(const FMingRegionalSyncData& RegionalData};

    // Validation
    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    bool ValidateReputationData(const FMingReplicatedReputationData& Data) const;

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    bool ValidateDelta(const FMingReputationSyncDelta& Delta) const;

    // Conflict Resolution
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void ResolveConflict(const FMingReplicatedReputationData& LocalData, const FMingReplicatedReputationData& RemoteData};

    // Statistics
    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    int32 GetTotalSyncOperations() const { return TotalSyncOperations; }

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    int32 GetFailedSyncOperations() const { return FailedSyncOperations; }

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    float GetAverageSyncLatency() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    int32 GetPendingSyncCount() const;

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    FMingRegionalSyncData GetRegionalData(FName RegionID) const;

    // Regional Operations
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void RegisterPlayerInRegion(int32 PlayerID, FName RegionID};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void UnregisterPlayerFromRegion(int32 PlayerID, FName RegionID};

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    TArray<int32> GetPlayersInRegion(FName RegionID) const;

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    bool IsPlayerInRegion(int32 PlayerID, FName RegionID) const;

    // Utilities
    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void CompressSyncData(TArray<FMingReplicatedReputationData>& SyncData};

    UFUNCTION(BlueprintCallable, Category = "Reputation Replication")
    void DecompressSyncData(TArray<FMingReplicatedReputationData>& SyncData};

    UFUNCTION(BlueprintPure, Category = "Reputation Replication")
    int32 CalculateSyncDataSize(const TArray<FMingReplicatedReputationData>& SyncData) const;

    // Event Delegates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReputationSynced, int32, PlayerID};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReputationChanged, FName, RegionID, float, NewValue};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRankChanged, FName, RegionID, int32, NewRank};
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegionalDataUpdated, FName, RegionID};

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnReputationSynced OnReputationSynced;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnReputationChanged OnReputationChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRankChanged OnRankChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRegionalDataUpdated OnRegionalDataUpdated;

protected:
    UPROPERTY()
    TObjectPtr<UMingNetworkManager> NetworkMgr;

    UPROPERTY()
    TObjectPtr<UMingReputationManager> ReputationMgr;

    UPROPERTY()
    EMingReputationSyncMode CurrentSyncMode;

    UPROPERTY()
    float SyncInterval;

    UPROPERTY()
    float RegionalSyncRadius;

    UPROPERTY()
    float LastSyncTime;

    UPROPERTY()
    int32 SyncVersion;

    UPROPERTY()
    TMap<FName, FMingReplicatedReputationData> CachedReputationData;

    UPROPERTY()
    TArray<FMingReputationSyncDelta> PendingDeltas;

    UPROPERTY()
    TMap<FName, TArray<int32>> PlayersInRegions;

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
    void PerformRegionalSync();

    void SendReputationSyncRequest(int32 TargetPlayerID, bool bFullSync};
    void ProcessReputationSyncResponse(const TArray<FMingReplicatedReputationData>& ReputationData};

    void TrackSyncLatency(float Latency};
    void RecordSyncOperation(bool bSuccess};

    bool ShouldSyncRegion(FName RegionID) const;
    bool IsInRegionalSyncRange(FName RegionID) const;

    TArray<FMingReplicatedReputationData> BuildFullSyncData();
    TArray<FMingReplicatedReputationData> BuildRegionalSyncData(const TArray<FName>& RegionIDs};
    TArray<FMingReplicatedReputationData> BuildDeltaSyncData();

    void ApplyDeltaToLocalData(const FMingReputationSyncDelta& Delta};
    void BroadcastDeltaToClients(const FMingReputationSyncDelta& Delta};
    void UpdateRegionalStatistics(FName RegionID};

    void OnNetworkTick(float DeltaTime};
    void OnPlayerJoined(int32 PlayerID};
    void OnPlayerLeft(int32 PlayerID};
    void OnReputationChangedInternal(FName RegionID, float OldValue, float NewValue};
    void OnRankChangedInternal(FName RegionID, int32 OldRank, int32 NewRank};

    // Serialization
    TArray<uint8> SerializeReputationData(const TArray<FMingReplicatedReputationData>& ReputationData};
    TArray<FMingReplicatedReputationData> DeserializeReputationData(const TArray<uint8>& Data};

    TArray<uint8> SerializeDelta(const FMingReputationSyncDelta& Delta};
    FMingReputationSyncDelta DeserializeDelta(const TArray<uint8>& Data};
};

