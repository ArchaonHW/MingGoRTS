#include "Network/MingRelationshipReplication.h"
#include "Network/MingNetworkManager.h"
#include "Engine/World.h"
#include "TimerManager.h"

UMingRelationshipReplication::UMingRelationshipReplication()
    : NetworkManager(nullptr)
    , ReplicationMode(ERelationshipReplicationMode::ServerAuthoritative)
    , bIsPeriodicSyncActive(false)
    , LastFullSyncTime(0.0f)
{
}

void UMingRelationshipReplication::Initialize(UMingNetworkManager* InNetworkManager)
{
    NetworkManager = InNetworkManager;
    SyncStats = FRelationshipSyncStats();
    RelationshipPriorities.Empty();
    CachedRelationships.Empty();
    CachedReputations.Empty();
    bIsPeriodicSyncActive = false;
    LastFullSyncTime = 0.0f;

    UE_LOG(LogTemp, Log, TEXT("MingRelationshipReplication initialized"));
}

void UMingRelationshipReplication::Shutdown()
{
    StopPeriodicSync();
    
    NetworkManager = nullptr;
    CachedRelationships.Empty();
    CachedReputations.Empty();

    UE_LOG(LogTemp, Log, TEXT("MingRelationshipReplication shutdown"));
}

void UMingRelationshipReplication::ServerInitializePlayerRelationships(int32 PlayerID)
{
    if (!NetworkManager)
    {
        return;
    }

    // Initialize default relationships for the player
    UE_LOG(LogTemp, Log, TEXT("Initializing relationships for player %d"), PlayerID);

    // In a real implementation, this would load from save data or create defaults
    // For now, create some sample relationships with other players
    TArray<FMingPlayerNetworkInfo> AllPlayers = NetworkManager->GetAllPlayers();
    
    for (const auto& OtherPlayer : AllPlayers)
    {
        if (OtherPlayer.PlayerID != PlayerID)
        {
            FNetworkRelationshipData NewRelationship;
            NewRelationship.RelationshipID = GenerateBatchID();
            NewRelationship.SourcePlayerID = PlayerID;
            NewRelationship.TargetPlayerID = OtherPlayer.PlayerID;
            NewRelationship.RelationshipType = 0; // Neutral
            NewRelationship.RelationshipValue = 50.0f; // Neutral value
            NewRelationship.bIsActive = true;
            NewRelationship.Version = 1;

            CachedRelationships.Add(NewRelationship.RelationshipID, NewRelationship);
        }
    }
}

void UMingRelationshipReplication::ServerUpdateRelationship(int32 RelationshipID, int32 SourcePlayerID, int32 TargetPlayerID, float NewValue)
{
    FNetworkRelationshipData* ExistingData = CachedRelationships.Find(RelationshipID);
    
    if (ExistingData)
    {
        ExistingData->RelationshipValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
        ExistingData->LastInteractionTime = FDateTime::Now().ToString();
        ExistingData->Version++;
    }
    else
    {
        FNetworkRelationshipData NewData;
        NewData.RelationshipID = RelationshipID;
        NewData.SourcePlayerID = SourcePlayerID;
        NewData.TargetPlayerID = TargetPlayerID;
        NewData.RelationshipValue = FMath::Clamp(NewValue, 0.0f, 100.0f);
        NewData.LastInteractionTime = FDateTime::Now().ToString();
        NewData.bIsActive = true;
        NewData.Version = 1;

        CachedRelationships.Add(RelationshipID, NewData);
    }

    UE_LOG(LogTemp, Log, TEXT("Server updated relationship %d: value = %.2f"), RelationshipID, NewValue);

    // Broadcast to relevant players
    FRelationshipReplicationBatch Batch;
    Batch.BatchID = GenerateBatchID();
    Batch.SenderPlayerID = -1; // Server
    Batch.Timestamp = GetWorld()->GetTimeSeconds();
    Batch.bIsFullSync = false;
    
    FNetworkRelationshipData UpdateData;
    if (CachedRelationships.Contains(RelationshipID))
    {
        UpdateData = CachedRelationships[RelationshipID];
        Batch.RelationshipUpdates.Add(UpdateData);
        ServerBroadcastToAllPlayers(Batch);
    }
}

void UMingRelationshipReplication::ServerUpdateReputation(int32 PlayerID, const FString& RegionID, float NewValue)
{
    int32 CacheKey = PlayerID + RegionID.GetHashCode();
    
    FNetworkReputationData* ExistingData = CachedReputations.Find(CacheKey);
    
    if (ExistingData)
    {
        ExistingData->ReputationValue = FMath::Clamp(NewValue, -100.0f, 100.0f);
        ExistingData->Version++;
    }
    else
    {
        FNetworkReputationData NewData;
        NewData.PlayerID = PlayerID;
        NewData.RegionID = RegionID;
        NewData.ReputationValue = FMath::Clamp(NewValue, -100.0f, 100.0f);
        NewData.Version = 1;

        CachedReputations.Add(CacheKey, NewData);
    }

    UE_LOG(LogTemp, Log, TEXT("Server updated reputation for player %d in region %s: value = %.2f"), 
        PlayerID, *RegionID, NewValue);
}

FRelationshipReplicationBatch UMingRelationshipReplication::ServerCreateReplicationBatch(int32 TargetPlayerID, bool bFullSync)
{
    FRelationshipReplicationBatch Batch;
    Batch.BatchID = GenerateBatchID();
    Batch.SenderPlayerID = -1; // Server
    Batch.Timestamp = GetWorld()->GetTimeSeconds();
    Batch.bIsFullSync = bFullSync;

    // Add relationship updates relevant to target player
    for (const auto& Pair : CachedRelationships)
    {
        const FNetworkRelationshipData& RelData = Pair.Value;
        
        // Include if it involves the target player
        if (RelData.SourcePlayerID == TargetPlayerID || RelData.TargetPlayerID == TargetPlayerID)
        {
            Batch.RelationshipUpdates.Add(RelData);
        }
    }

    // Add reputation updates for target player
    for (const auto& Pair : CachedReputations)
    {
        const FNetworkReputationData& RepData = Pair.Value;
        
        if (RepData.PlayerID == TargetPlayerID)
        {
            Batch.ReputationUpdates.Add(RepData);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Created replication batch %d with %d relationships and %d reputations"), 
        Batch.BatchID, Batch.RelationshipUpdates.Num(), Batch.ReputationUpdates.Num());

    return Batch;
}

void UMingRelationshipReplication::ServerBroadcastToAllPlayers(const FRelationshipReplicationBatch& Batch)
{
    if (!NetworkManager)
    {
        return;
    }

    TArray<FMingPlayerNetworkInfo> AllPlayers = NetworkManager->GetAllPlayers();
    
    for (const auto& Player : AllPlayers)
    {
        if (!Player.bIsHost) // Don't send to server/host again
        {
            SendBatchToPlayer(Player.PlayerID, Batch);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Broadcasted batch %d to %d players"), Batch.BatchID, AllPlayers.Num() - 1);
}

void UMingRelationshipReplication::ClientRequestFullSync()
{
    if (!NetworkManager)
    {
        return;
    }

    int32 LocalPlayerID = NetworkManager->GetLocalPlayerID();
    
    UE_LOG(LogTemp, Log, TEXT("Client requesting full sync for player %d"), LocalPlayerID);

    // In a real implementation, this would send a reliable RPC to the server
    // For now, simulate receiving a full sync
    FRelationshipReplicationBatch FullBatch = ServerCreateReplicationBatch(LocalPlayerID, true);
    ClientProcessReplicationBatch(FullBatch);
}

void UMingRelationshipReplication::ClientProcessReplicationBatch(const FRelationshipReplicationBatch& Batch)
{
    UE_LOG(LogTemp, Log, TEXT("Processing replication batch %d"), Batch.BatchID);

    // Apply relationship updates
    for (const auto& RelUpdate : Batch.RelationshipUpdates)
    {
        ApplyRelationshipUpdate(RelUpdate);
    }

    // Apply reputation updates
    for (const auto& RepUpdate : Batch.ReputationUpdates)
    {
        ApplyReputationUpdate(RepUpdate);
    }

    // Update cache
    UpdateCache(Batch);

    // Update stats
    RecordSyncSuccess(GetWorld()->GetTimeSeconds() - Batch.Timestamp);

    // Broadcast completion
    OnSyncCompleted.Broadcast();

    UE_LOG(LogTemp, Log, TEXT("Finished processing batch %d"), Batch.BatchID);
}

void UMingRelationshipReplication::ClientSendRelationshipUpdate(int32 RelationshipID, float DeltaValue)
{
    if (!NetworkManager)
    {
        return;
    }

    int32 LocalPlayerID = NetworkManager->GetLocalPlayerID();

    // In predictive mode, apply locally first
    if (ReplicationMode == ERelationshipReplicationMode::ClientPredictive ||
        ReplicationMode == ERelationshipReplicationMode::Hybrid)
    {
        if (CachedRelationships.Contains(RelationshipID))
        {
            CachedRelationships[RelationshipID].RelationshipValue += DeltaValue;
        }
    }

    // Send to server
    // In a real implementation, this would be an unreliable RPC
    UE_LOG(LogTemp, Log, TEXT("Client %d sending relationship update: ID=%d, Delta=%.2f"), 
        LocalPlayerID, RelationshipID, DeltaValue);

    // Simulate server processing
    if (CachedRelationships.Contains(RelationshipID))
    {
        FNetworkRelationshipData& Data = CachedRelationships[RelationshipID];
        ServerUpdateRelationship(RelationshipID, Data.SourcePlayerID, Data.TargetPlayerID, 
            Data.RelationshipValue + DeltaValue);
    }
}

void UMingRelationshipReplication::ClientSendReputationUpdate(const FString& RegionID, float DeltaValue)
{
    if (!NetworkManager)
    {
        return;
    }

    int32 LocalPlayerID = NetworkManager->GetLocalPlayerID();

    UE_LOG(LogTemp, Log, TEXT("Client %d sending reputation update: Region=%s, Delta=%.2f"), 
        LocalPlayerID, *RegionID, DeltaValue);

    // Simulate server processing
    if (CachedReputations.Num() > 0)
    {
        for (auto& Pair : CachedReputations)
        {
            if (Pair.Value.PlayerID == LocalPlayerID && Pair.Value.RegionID == RegionID)
            {
                ServerUpdateReputation(LocalPlayerID, RegionID, Pair.Value.ReputationValue + DeltaValue);
                break;
            }
        }
    }
}

void UMingRelationshipReplication::ResolveConflict_ServerAuthoritative(int32 RelationshipID)
{
    UE_LOG(LogTemp, Log, TEXT("Resolving conflict for relationship %d using server authoritative"), RelationshipID);
    
    // Server version wins - already in cache as server data
    SyncStats.ConflictResolutions++;
    OnConflictDetected.Broadcast(RelationshipID);
}

void UMingRelationshipReplication::ResolveConflict_ClientWins(int32 RelationshipID)
{
    UE_LOG(LogTemp, Log, TEXT("Resolving conflict for relationship %d using client wins"), RelationshipID);
    
    // In a real implementation, this would require the client to resend their version
    // For now, just increment the conflict counter
    SyncStats.ConflictResolutions++;
    OnConflictDetected.Broadcast(RelationshipID);
}

void UMingRelationshipReplication::ResolveConflict_MergeValues(int32 RelationshipID, float MergeWeight)
{
    UE_LOG(LogTemp, Log, TEXT("Resolving conflict for relationship %d using merge with weight %.2f"), 
        RelationshipID, MergeWeight);
    
    if (CachedRelationships.Contains(RelationshipID))
    {
        // Weighted average merge
        FNetworkRelationshipData& Data = CachedRelationships[RelationshipID];
        // In a real implementation, we would have both server and client values
        // For now, just log the operation
        Data.Version++;
    }

    SyncStats.ConflictResolutions++;
    OnConflictDetected.Broadcast(RelationshipID);
}

void UMingRelationshipReplication::StartPeriodicSync(float SyncInterval)
{
    if (bIsPeriodicSyncActive)
    {
        return;
    }

    bIsPeriodicSyncActive = true;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(PeriodicSyncTimer, this, 
            &UMingRelationshipReplication::PerformPeriodicSync, SyncInterval, true);
    }

    UE_LOG(LogTemp, Log, TEXT("Started periodic sync with interval %.2f seconds"), SyncInterval);
}

void UMingRelationshipReplication::StopPeriodicSync()
{
    if (!bIsPeriodicSyncActive)
    {
        return;
    }

    bIsPeriodicSyncActive = false;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(PeriodicSyncTimer);
    }

    UE_LOG(LogTemp, Log, TEXT("Stopped periodic sync"));
}

void UMingRelationshipReplication::ForceImmediateSync()
{
    UE_LOG(LogTemp, Log, TEXT("Forcing immediate sync"));
    PerformPeriodicSync();
}

void UMingRelationshipReplication::SetReplicationMode(ERelationshipReplicationMode NewMode)
{
    ReplicationMode = NewMode;
    UE_LOG(LogTemp, Log, TEXT("Replication mode changed to %s"), 
        *UEnum::GetValueAsString(ReplicationMode));
}

ERelationshipReplicationMode UMingRelationshipReplication::GetReplicationMode() const
{
    return ReplicationMode;
}

void UMingRelationshipReplication::SetSyncPriority(int32 RelationshipID, ERelationshipSyncPriority Priority)
{
    RelationshipPriorities.Add(RelationshipID, Priority);
    UE_LOG(LogTemp, Log, TEXT("Set sync priority for relationship %d to %s"), 
        RelationshipID, *UEnum::GetValueAsString(Priority));
}

FRelationshipSyncStats UMingRelationshipReplication::GetSyncStats() const
{
    return SyncStats;
}

void UMingRelationshipReplication::ResetSyncStats()
{
    SyncStats = FRelationshipSyncStats();
    UE_LOG(LogTemp, Log, TEXT("Sync stats reset"));
}

void UMingRelationshipReplication::GetDetailedSyncReport(FString& OutReport) const
{
    OutReport = FString::Printf(TEXT(
        "=== Relationship Replication Sync Report ===\n"
        "Replication Mode: %s\n"
        "Total Relationships Synced: %d\n"
        "Total Reputations Synced: %d\n"
        "Average Sync Time: %.3f seconds\n"
        "Failed Syncs: %d\n"
        "Conflict Resolutions: %d\n"
        "Last Full Sync: %.2f seconds ago\n"
        "Cached Relationships: %d\n"
        "Cached Reputations: %d\n"
        "Periodic Sync Active: %s\n"),
        *UEnum::GetValueAsString(ReplicationMode),
        SyncStats.TotalRelationshipsSynced,
        SyncStats.TotalReputationsSynced,
        SyncStats.AverageSyncTime,
        SyncStats.FailedSyncs,
        SyncStats.ConflictResolutions,
        GetWorld() ? GetWorld()->GetTimeSeconds() - LastFullSyncTime : 0.0f,
        CachedRelationships.Num(),
        CachedReputations.Num(),
        bIsPeriodicSyncActive ? TEXT("Yes") : TEXT("No")
    );
}

bool UMingRelationshipReplication::ValidateRelationshipData(const FNetworkRelationshipData& Data) const
{
    return Data.RelationshipID >= 0 &&
           Data.SourcePlayerID >= 0 &&
           Data.TargetPlayerID >= 0 &&
           Data.SourcePlayerID != Data.TargetPlayerID &&
           Data.Version > 0;
}

bool UMingRelationshipReplication::ValidateReputationData(const FNetworkReputationData& Data) const
{
    return Data.PlayerID >= 0 &&
           !Data.RegionID.IsEmpty() &&
           Data.Version > 0;
}

void UMingRelationshipReplication::CompressReplicationBatch(FRelationshipReplicationBatch& Batch)
{
    // Remove duplicate updates (keep only latest version for each ID)
    TMap<int32, FNetworkRelationshipData> UniqueRelationships;
    for (const auto& Rel : Batch.RelationshipUpdates)
    {
        if (!UniqueRelationships.Contains(Rel.RelationshipID) ||
            UniqueRelationships[Rel.RelationshipID].Version < Rel.Version)
        {
            UniqueRelationships.Add(Rel.RelationshipID, Rel);
        }
    }
    
    Batch.RelationshipUpdates.Empty();
    for (const auto& Pair : UniqueRelationships)
    {
        Batch.RelationshipUpdates.Add(Pair.Value);
    }

    UE_LOG(LogTemp, Log, TEXT("Compressed batch %d: %d relationships remaining"), 
        Batch.BatchID, Batch.RelationshipUpdates.Num());
}

void UMingRelationshipReplication::DecompressReplicationBatch(FRelationshipReplicationBatch& Batch)
{
    // No decompression needed for this implementation
    // In a real implementation, this might handle delta compression
    UE_LOG(LogTemp, Log, TEXT("Decompressed batch %d"), Batch.BatchID);
}

// Private helper functions

void UMingRelationshipReplication::PerformPeriodicSync()
{
    if (!NetworkManager)
    {
        return;
    }

    int32 LocalPlayerID = NetworkManager->GetLocalPlayerID();
    
    if (NetworkManager->IsHost())
    {
        // Server: create and broadcast batches to all clients
        for (const auto& Player : NetworkManager->GetAllPlayers())
        {
            if (!Player.bIsHost)
            {
                FRelationshipReplicationBatch Batch = ServerCreateReplicationBatch(Player.PlayerID, false);
                SendBatchToPlayer(Player.PlayerID, Batch);
            }
        }
    }
    else
    {
        // Client: request sync
        ClientRequestFullSync();
    }

    LastFullSyncTime = GetWorld()->GetTimeSeconds();
}

void UMingRelationshipReplication::SendBatchToPlayer(int32 PlayerID, const FRelationshipReplicationBatch& Batch)
{
    // In a real implementation, this would send a reliable RPC to the specific player
    UE_LOG(LogTemp, Log, TEXT("Sending batch %d to player %d"), Batch.BatchID, PlayerID);
}

void UMingRelationshipReplication::ApplyRelationshipUpdate(const FNetworkRelationshipData& Update)
{
    if (!ValidateRelationshipData(Update))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid relationship update received"));
        return;
    }

    // Check for conflicts
    if (CachedRelationships.Contains(Update.RelationshipID))
    {
        FNetworkRelationshipData& Existing = CachedRelationships[Update.RelationshipID];
        
        if (CheckForConflicts(Update, Existing))
        {
            UE_LOG(LogTemp, Warning, TEXT("Conflict detected for relationship %d"), Update.RelationshipID);
            OnConflictDetected.Broadcast(Update.RelationshipID);
            
            // Default to server authoritative resolution
            if (ReplicationMode == ERelationshipReplicationMode::ServerAuthoritative)
            {
                if (Update.Version > Existing.Version)
                {
                    Existing = Update;
                }
            }
        }
        else
        {
            Existing = Update;
        }
    }
    else
    {
        CachedRelationships.Add(Update.RelationshipID, Update);
    }

    SyncStats.TotalRelationshipsSynced++;
}

void UMingRelationshipReplication::ApplyReputationUpdate(const FNetworkReputationData& Update)
{
    if (!ValidateReputationData(Update))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid reputation update received"));
        return;
    }

    int32 CacheKey = Update.PlayerID + Update.RegionID.GetHashCode();
    CachedReputations.Add(CacheKey, Update);

    SyncStats.TotalReputationsSynced++;
}

bool UMingRelationshipReplication::CheckForConflicts(const FNetworkRelationshipData& Incoming, 
    const FNetworkRelationshipData& Existing)
{
    // Conflict if versions are incompatible
    // This is a simple check - real implementation might be more complex
    return FMath::Abs(Incoming.Version - Existing.Version) > 1;
}

int32 UMingRelationshipReplication::GenerateBatchID()
{
    static int32 NextBatchID = 1;
    return NextBatchID++;
}

void UMingRelationshipReplication::RecordSyncSuccess(float SyncTime)
{
    SyncStats.LastSyncTimestamp = GetWorld()->GetTimeSeconds();
    
    // Update average sync time
    int32 TotalSyncs = SyncStats.TotalRelationshipsSynced + SyncStats.TotalReputationsSynced;
    if (TotalSyncs > 0)
    {
        SyncStats.AverageSyncTime = ((SyncStats.AverageSyncTime * (TotalSyncs - 1)) + SyncTime) / TotalSyncs;
    }
}

void UMingRelationshipReplication::RecordSyncFailure(const FString& Reason)
{
    SyncStats.FailedSyncs++;
    OnSyncFailed.Broadcast(Reason);
    UE_LOG(LogTemp, Error, TEXT("Sync failed: %s"), *Reason);
}

void UMingRelationshipReplication::UpdateCache(const FRelationshipReplicationBatch& Batch)
{
    // Cache is already updated in Apply functions
    // This function could be used for additional cache management
    LastFullSyncTime = GetWorld()->GetTimeSeconds();
}
