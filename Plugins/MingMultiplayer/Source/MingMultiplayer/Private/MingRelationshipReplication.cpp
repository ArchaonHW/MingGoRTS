#include "MingRelationshipReplication.h"
#include "MingNetworkManager.h"
#include "Relationship/MingRelationshipManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UMingRelationshipReplication::UMingRelationshipReplication()
    : CurrentSyncMode(EMingRelationshipSyncMode::DeltaSync)
    , SyncInterval(1.0f)
    , ProximityRadius(10000.0f)
    , LastSyncTime(0.0f)
    , SyncVersion(0)
    , TotalSyncOperations(0)
    , FailedSyncOperations(0)
{
}

void UMingRelationshipReplication::InitializeReplication(UMingNetworkManager* NetworkManager, UMingRelationshipManager* RelationshipManager)
{
    NetworkMgr = NetworkManager;
    RelationshipMgr = RelationshipManager;
    
    if (NetworkMgr)
    {
        // Bind to network events
        NetworkMgr->OnPlayerConnected.AddDynamic(this, &UMingRelationshipReplication::OnPlayerJoined);
        NetworkMgr->OnPlayerDisconnected.AddDynamic(this, &UMingRelationshipReplication::OnPlayerLeft);
    }
    
    if (RelationshipMgr)
    {
        // Bind to relationship change events
        // RelationshipMgr->OnRelationshipChanged.AddDynamic(this, &UMingRelationshipReplication::OnRelationshipChanged);
    }
    
    // Start sync timer
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->GetTimerManager().SetTimer(
            *(new FTimerHandle()),
            FTimerDelegate::CreateUObject(this, &UMingRelationshipReplication::OnNetworkTick, SyncInterval),
            SyncInterval,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("Relationship replication initialized"));
}

void UMingRelationshipReplication::ShutdownReplication()
{
    if (NetworkMgr)
    {
        NetworkMgr->OnPlayerConnected.RemoveAll(this);
        NetworkMgr->OnPlayerDisconnected.RemoveAll(this);
    }
    
    if (RelationshipMgr)
    {
        // RelationshipMgr->OnRelationshipChanged.RemoveAll(this);
    }
    
    CachedRelationshipData.Empty();
    PendingDeltas.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Relationship replication shutdown"));
}

void UMingRelationshipReplication::SetSyncMode(EMingRelationshipSyncMode Mode)
{
    CurrentSyncMode = Mode;
    UE_LOG(LogTemp, Log, TEXT("Sync mode changed to: %d"), static_cast<int32>(Mode));
}

void UMingRelationshipReplication::SetSyncInterval(float IntervalSeconds)
{
    SyncInterval = FMath::Clamp(IntervalSeconds, 0.1f, 10.0f);
}

void UMingRelationshipReplication::SetProximityRadius(float Radius)
{
    ProximityRadius = FMath::Clamp(Radius, 1000.0f, 100000.0f);
}

void UMingRelationshipReplication::RequestFullSync(int32 TargetPlayerID)
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    FMingRelationshipSyncRequest Request = BuildSyncRequest(TargetPlayerID, true);
    SendSyncRequest(Request);
    
    UE_LOG(LogTemp, Log, TEXT("Requested full sync for player %d"), TargetPlayerID);
}

void UMingRelationshipReplication::RequestPartialSync(const TArray<FName>& CharacterIDs)
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    FMingRelationshipSyncRequest Request;
    Request.RequestingPlayerID = NetworkMgr->GetLocalPlayerID();
    Request.RequestedCharacters = CharacterIDs;
    Request.bFullSync = false;
    Request.LastKnownVersion = SyncVersion;
    Request.RequestTimestamp = GetWorld()->GetTimeSeconds();
    
    SendSyncRequest(Request);
    
    UE_LOG(LogTemp, Log, TEXT("Requested partial sync for %d characters"), CharacterIDs.Num());
}

void UMingRelationshipReplication::ForceSyncAll()
{
    if (!NetworkMgr)
    {
        return;
    }
    
    // Request sync for all connected players
    TArray<FMingPlayerNetworkInfo> AllPlayers = NetworkMgr->GetAllPlayers();
    for (const FMingPlayerNetworkInfo& Player : AllPlayers)
    {
        RequestFullSync(Player.PlayerID);
    }
}

void UMingRelationshipReplication::ServerUpdateRelationship(int32 PlayerID, FName CharacterID, float NewValue, const FString& Reason)
{
    if (!NetworkMgr || !NetworkMgr->IsHost())
    {
        return;
    }
    
    // Update relationship on server
    if (RelationshipMgr)
    {
        float OldValue = RelationshipMgr->GetRelationshipValue(CharacterID);
        RelationshipMgr->SetRelationshipValue(CharacterID, NewValue);
        
        // Create and broadcast delta
        FMingRelationshipSyncDelta Delta;
        Delta.CharacterID = CharacterID;
        Delta.OldValue = OldValue;
        Delta.NewValue = NewValue;
        Delta.ChangeReason = Reason;
        Delta.SourcePlayerID = PlayerID;
        Delta.Timestamp = GetWorld()->GetTimeSeconds();
        
        BroadcastDeltaToClients(Delta);
        
        UE_LOG(LogTemp, Log, TEXT("Server updated relationship for %s: %.2f -> %.2f"), 
            *CharacterID.ToString(), OldValue, NewValue);
    }
}

void UMingRelationshipReplication::ServerAddRelationshipModifier(int32 PlayerID, FName CharacterID, const FString& Modifier, float Value)
{
    if (!NetworkMgr || !NetworkMgr->IsHost() || !RelationshipMgr)
    {
        return;
    }
    
    // Add modifier logic would go here
    // RelationshipMgr->AddModifier(CharacterID, Modifier, Value);
    
    UE_LOG(LogTemp, Log, TEXT("Added modifier %s to %s for player %d"), 
        *Modifier, *CharacterID.ToString(), PlayerID);
}

void UMingRelationshipReplication::ServerRemoveRelationshipModifier(int32 PlayerID, FName CharacterID, const FString& Modifier)
{
    if (!NetworkMgr || !NetworkMgr->IsHost() || !RelationshipMgr)
    {
        return;
    }
    
    // Remove modifier logic would go here
    
    UE_LOG(LogTemp, Log, TEXT("Removed modifier %s from %s for player %d"), 
        *Modifier, *CharacterID.ToString(), PlayerID);
}

void UMingRelationshipReplication::ClientApplySyncData(const FMingRelationshipSyncResponse& SyncData)
{
    if (!RelationshipMgr)
    {
        return;
    }
    
    // Apply full relationship data
    for (const FMingReplicatedRelationshipData& Data : SyncData.RelationshipData)
    {
        if (Data.IsValid())
        {
            RelationshipMgr->SetRelationshipValue(Data.CharacterID, Data.RelationshipValue);
            // Apply other relationship properties
        }
    }
    
    // Apply deltas
    for (const FMingRelationshipSyncDelta& Delta : SyncData.Deltas)
    {
        ClientApplyDelta(Delta);
    }
    
    // Update sync version
    SyncVersion = SyncData.ServerVersion;
    
    OnRelationshipSynced.Broadcast(SyncData.TargetPlayerID);
    
    UE_LOG(LogTemp, Log, TEXT("Applied sync data: %d relationships, %d deltas"), 
        SyncData.RelationshipData.Num(), SyncData.Deltas.Num());
}

void UMingRelationshipReplication::ClientApplyDelta(const FMingRelationshipSyncDelta& Delta)
{
    if (!RelationshipMgr || !Delta.CharacterID.IsValid())
    {
        return;
    }
    
    // Validate delta
    if (!ValidateDelta(Delta))
    {
        UE_LOG(LogTemp, Warning, TEXT("Invalid delta rejected"));
        return;
    }
    
    // Apply delta to local data
    ApplyDeltaToLocalData(Delta);
    
    UE_LOG(LogTemp, Log, TEXT("Applied delta for %s: %.2f -> %.2f"), 
        *Delta.CharacterID.ToString(), Delta.OldValue, Delta.NewValue);
}

bool UMingRelationshipReplication::ValidateSyncData(const FMingRelationshipSyncResponse& SyncData) const
{
    // Check timestamp is reasonable
    float CurrentTime = GetWorld()->GetTimeSeconds();
    if (SyncData.ServerTimestamp > CurrentTime + 60.0f || SyncData.ServerTimestamp < CurrentTime - 300.0f)
    {
        return false;
    }
    
    // Check version
    if (SyncData.ServerVersion < 0)
    {
        return false;
    }
    
    // Validate each relationship data
    for (const FMingReplicatedRelationshipData& Data : SyncData.RelationshipData)
    {
        if (!Data.IsValid())
        {
            return false;
        }
        
        if (Data.RelationshipValue < 0.0f || Data.RelationshipValue > 100.0f)
        {
            return false;
        }
    }
    
    return true;
}

bool UMingRelationshipReplication::ValidateDelta(const FMingRelationshipSyncDelta& Delta) const
{
    if (!Delta.CharacterID.IsValid())
    {
        return false;
    }
    
    if (Delta.NewValue < 0.0f || Delta.NewValue > 100.0f)
    {
        return false;
    }
    
    if (Delta.SourcePlayerID < 0)
    {
        return false;
    }
    
    return true;
}

void UMingRelationshipReplication::ResolveConflict(const FMingReplicatedRelationshipData& LocalData, 
    const FMingReplicatedRelationshipData& RemoteData)
{
    // Conflict resolution strategy: use higher version or server data
    FMingReplicatedRelationshipData ResolvedData;
    
    if (RemoteData.SyncVersion > LocalData.SyncVersion)
    {
        ResolvedData = RemoteData;
    }
    else
    {
        // If versions are equal, use server data if this is client
        if (NetworkMgr && !NetworkMgr->IsHost())
        {
            ResolvedData = RemoteData;
        }
        else
        {
            ResolvedData = LocalData;
        }
    }
    
    // Update local data
    CachedRelationshipData.Add(ResolvedData.CharacterID, ResolvedData);
    
    OnSyncConflict.Broadcast(LocalData, RemoteData);
    
    UE_LOG(LogTemp, Log, TEXT("Resolved conflict for %s"), *ResolvedData.CharacterID.ToString());
}

FMingReplicatedRelationshipData UMingRelationshipReplication::MergeRelationshipData(
    const TArray<FMingReplicatedRelationshipData>& DataVersions)
{
    FMingReplicatedRelationshipData MergedData;
    
    if (DataVersions.Num() == 0)
    {
        return MergedData;
    }
    
    // Use most recent version
    int32 LatestVersion = 0;
    for (const FMingReplicatedRelationshipData& Data : DataVersions)
    {
        if (Data.SyncVersion > LatestVersion)
        {
            LatestVersion = Data.SyncVersion;
            MergedData = Data;
        }
    }
    
    return MergedData;
}

float UMingRelationshipReplication::GetAverageSyncLatency() const
{
    if (SyncLatencies.Num() == 0)
    {
        return 0.0f;
    }
    
    float Total = 0.0f;
    for (float Latency : SyncLatencies)
    {
        Total += Latency;
    }
    
    return Total / SyncLatencies.Num();
}

int32 UMingRelationshipReplication::GetPendingSyncCount() const
{
    return PendingDeltas.Num();
}

void UMingRelationshipReplication::CompressSyncData(FMingRelationshipSyncResponse& SyncData)
{
    // Compression logic would be implemented here
    // Using Oodle or zlib compression
}

void UMingRelationshipReplication::DecompressSyncData(FMingRelationshipSyncResponse& SyncData)
{
    // Decompression logic would be implemented here
}

int32 UMingRelationshipReplication::CalculateSyncDataSize(const FMingRelationshipSyncResponse& SyncData) const
{
    int32 Size = sizeof(FMingRelationshipSyncResponse);
    Size += SyncData.RelationshipData.Num() * sizeof(FMingReplicatedRelationshipData);
    Size += SyncData.Deltas.Num() * sizeof(FMingRelationshipSyncDelta);
    
    return Size;
}

void UMingRelationshipReplication::PerformFullSync()
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    // Request full sync from server
    FMingRelationshipSyncRequest Request = BuildSyncRequest(NetworkMgr->GetLocalPlayerID(), true);
    SendSyncRequest(Request);
}

void UMingRelationshipReplication::PerformDeltaSync()
{
    if (!NetworkMgr || !NetworkMgr->IsConnected() || PendingDeltas.Num() == 0)
    {
        return;
    }
    
    // Send accumulated deltas
    for (const FMingRelationshipSyncDelta& Delta : PendingDeltas)
    {
        TArray<uint8> SerializedDelta = SerializeDelta(Delta);
        NetworkMgr->BroadcastMessage(1, SerializedDelta, EMingSyncPriority::Normal);
    }
    
    PendingDeltas.Empty();
}

void UMingRelationshipReplication::PerformEventDrivenSync()
{
    // Event-driven sync is handled by the change event callbacks
}

void UMingRelationshipReplication::PerformProximitySync()
{
    if (!RelationshipMgr || !NetworkMgr)
    {
        return;
    }
    
    // Only sync characters within proximity radius
    TArray<FName> ProximityCharacters;
    // Get characters within proximity
    
    if (ProximityCharacters.Num() > 0)
    {
        RequestPartialSync(ProximityCharacters);
    }
}

void UMingRelationshipReplication::SendSyncRequest(const FMingRelationshipSyncRequest& Request)
{
    if (!NetworkMgr)
    {
        return;
    }
    
    // Serialize and send request
    // TArray<uint8> SerializedRequest = SerializeRequest(Request);
    // NetworkMgr->SendMessage(ServerPlayerID, 0, SerializedRequest, EMingSyncPriority::High, true);
}

void UMingRelationshipReplication::ProcessSyncResponse(const FMingRelationshipSyncResponse& Response)
{
    if (!ValidateSyncData(Response))
    {
        OnSyncError.Broadcast(TEXT("Invalid sync data received"));
        return;
    }
    
    ClientApplySyncData(Response);
    RecordSyncOperation(true);
}

void UMingRelationshipReplication::TrackSyncLatency(float Latency)
{
    SyncLatencies.Add(Latency);
    
    // Keep only last 100 measurements
    if (SyncLatencies.Num() > 100)
    {
        SyncLatencies.RemoveAt(0);
    }
}

void UMingRelationshipReplication::RecordSyncOperation(bool bSuccess)
{
    TotalSyncOperations++;
    if (!bSuccess)
    {
        FailedSyncOperations++;
    }
}

bool UMingRelationshipReplication::ShouldSyncCharacter(FName CharacterID) const
{
    // Check if character should be synced based on sync mode
    switch (CurrentSyncMode)
    {
    case EMingRelationshipSyncMode::FullSync:
        return true;
    case EMingRelationshipSyncMode::DeltaSync:
        return true;
    case EMingRelationshipSyncMode::EventDriven:
        return true;
    case EMingRelationshipSyncMode::ProximityBased:
        return IsInProximity(CharacterID);
    default:
        return true;
    }
}

bool UMingRelationshipReplication::IsInProximity(FName CharacterID) const
{
    // Check if character is within proximity radius
    // This would use world position data
    return true; // Placeholder
}

FMingRelationshipSyncRequest UMingRelationshipReplication::BuildSyncRequest(int32 PlayerID, bool bFullSync)
{
    FMingRelationshipSyncRequest Request;
    Request.RequestingPlayerID = PlayerID;
    Request.bFullSync = bFullSync;
    Request.LastKnownVersion = SyncVersion;
    Request.RequestTimestamp = GetWorld()->GetTimeSeconds();
    
    if (!bFullSync)
    {
        // Add recently changed characters
        // Request.RequestedCharacters = GetRecentlyChangedCharacters();
    }
    
    return Request;
}

FMingRelationshipSyncResponse UMingRelationshipReplication::BuildSyncResponse(const FMingRelationshipSyncRequest& Request)
{
    FMingRelationshipSyncResponse Response;
    Response.TargetPlayerID = Request.RequestingPlayerID;
    Response.ServerVersion = ++SyncVersion;
    Response.ServerTimestamp = GetWorld()->GetTimeSeconds();
    Response.bIsIncremental = !Request.bFullSync;
    
    if (RelationshipMgr)
    {
        if (Request.bFullSync)
        {
            // Add all relationships
            TArray<FName> AllCharacters = RelationshipMgr->GetAllCharacterIDs();
            for (const FName& CharacterID : AllCharacters)
            {
                FMingReplicatedRelationshipData Data;
                Data.CharacterID = CharacterID;
                Data.RelationshipValue = RelationshipMgr->GetRelationshipValue(CharacterID);
                Data.RelationshipType = RelationshipMgr->GetRelationshipType(CharacterID);
                Data.RelationshipLevel = RelationshipMgr->GetRelationshipLevel(CharacterID);
                Data.SyncVersion = SyncVersion;
                
                Response.RelationshipData.Add(Data);
            }
        }
        else
        {
            // Add only requested characters
            for (const FName& CharacterID : Request.RequestedCharacters)
            {
                if (ShouldSyncCharacter(CharacterID))
                {
                    FMingReplicatedRelationshipData Data;
                    Data.CharacterID = CharacterID;
                    Data.RelationshipValue = RelationshipMgr->GetRelationshipValue(CharacterID);
                    Data.RelationshipType = RelationshipMgr->GetRelationshipType(CharacterID);
                    Data.RelationshipLevel = RelationshipMgr->GetRelationshipLevel(CharacterID);
                    Data.SyncVersion = SyncVersion;
                    
                    Response.RelationshipData.Add(Data);
                }
            }
        }
    }
    
    return Response;
}

void UMingRelationshipReplication::ApplyDeltaToLocalData(const FMingRelationshipSyncDelta& Delta)
{
    if (!RelationshipMgr)
    {
        return;
    }
    
    // Apply the delta change
    RelationshipMgr->SetRelationshipValue(Delta.CharacterID, Delta.NewValue);
    
    // Update cache
    if (CachedRelationshipData.Contains(Delta.CharacterID))
    {
        CachedRelationshipData[Delta.CharacterID].RelationshipValue = Delta.NewValue;
    }
}

void UMingRelationshipReplication::BroadcastDeltaToClients(const FMingRelationshipSyncDelta& Delta)
{
    if (!NetworkMgr)
    {
        return;
    }
    
    // Serialize and broadcast delta to all connected clients
    TArray<uint8> SerializedDelta = SerializeDelta(Delta);
    NetworkMgr->BroadcastMessage(1, SerializedDelta, EMingSyncPriority::Normal);
}

void UMingRelationshipReplication::OnNetworkTick(float DeltaTime)
{
    // Perform sync based on current mode
    switch (CurrentSyncMode)
    {
    case EMingRelationshipSyncMode::FullSync:
        PerformFullSync();
        break;
    case EMingRelationshipSyncMode::DeltaSync:
        PerformDeltaSync();
        break;
    case EMingRelationshipSyncMode::EventDriven:
        PerformEventDrivenSync();
        break;
    case EMingRelationshipSyncMode::ProximityBased:
        PerformProximitySync();
        break;
    }
}

void UMingRelationshipReplication::OnPlayerJoined(int32 PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d joined, requesting relationship sync"), PlayerID);
    
    // Request full sync for new player
    if (NetworkMgr && NetworkMgr->IsHost())
    {
        // Send current relationship data to new player
        FMingRelationshipSyncRequest Request;
        Request.RequestingPlayerID = PlayerID;
        Request.bFullSync = true;
        
        FMingRelationshipSyncResponse Response = BuildSyncResponse(Request);
        
        // Send response to player
        // TArray<uint8> SerializedResponse = SerializeSyncData(Response);
        // NetworkMgr->SendMessage(PlayerID, 0, SerializedResponse, EMingSyncPriority::High, true);
    }
}

void UMingRelationshipReplication::OnPlayerLeft(int32 PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d left, cleaning up relationship data"), PlayerID);
    
    // Cleanup any player-specific data
    // This might involve transferring ownership of relationships
}

void UMingRelationshipReplication::OnRelationshipChanged(FName CharacterID, float OldValue, float NewValue)
{
    // Create delta for the change
    FMingRelationshipSyncDelta Delta;
    Delta.CharacterID = CharacterID;
    Delta.OldValue = OldValue;
    Delta.NewValue = NewValue;
    Delta.SourcePlayerID = NetworkMgr ? NetworkMgr->GetLocalPlayerID() : -1;
    Delta.Timestamp = GetWorld()->GetTimeSeconds();
    
    // Add to pending deltas
    PendingDeltas.Add(Delta);
    
    // For event-driven sync, broadcast immediately
    if (CurrentSyncMode == EMingRelationshipSyncMode::EventDriven)
    {
        BroadcastDeltaToClients(Delta);
    }
}

TArray<uint8> UMingRelationshipReplication::SerializeSyncData(const FMingRelationshipSyncResponse& SyncData)
{
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);
    Writer << const_cast<FMingRelationshipSyncResponse&>(SyncData);
    return Data;
}

FMingRelationshipSyncResponse UMingRelationshipReplication::DeserializeSyncData(const TArray<uint8>& Data)
{
    FMingRelationshipSyncResponse SyncData;
    FMemoryReader Reader(Data);
    Reader << SyncData;
    return SyncData;
}

TArray<uint8> UMingRelationshipReplication::SerializeDelta(const FMingRelationshipSyncDelta& Delta)
{
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);
    Writer << const_cast<FMingRelationshipSyncDelta&>(Delta);
    return Data;
}

FMingRelationshipSyncDelta UMingRelationshipReplication::DeserializeDelta(const TArray<uint8>& Data)
{
    FMingRelationshipSyncDelta Delta;
    FMemoryReader Reader(Data);
    Reader << Delta;
    return Delta;
}
