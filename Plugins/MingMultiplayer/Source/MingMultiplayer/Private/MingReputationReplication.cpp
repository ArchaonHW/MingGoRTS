#include "MingReputationReplication.h"
#include "MingNetworkManager.h"
#include "Reputation/MingReputationManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"

UMingReputationReplication::UMingReputationReplication()
    : CurrentSyncMode(EMingReputationSyncMode::DeltaSync)
    , SyncInterval(2.0f)
    , RegionalSyncRadius(50000.0f)
    , LastSyncTime(0.0f)
    , SyncVersion(0)
    , TotalSyncOperations(0)
    , FailedSyncOperations(0)
{
}

void UMingReputationReplication::InitializeReplication(UMingNetworkManager* NetworkManager, UMingReputationManager* ReputationManager)
{
    NetworkMgr = NetworkManager;
    ReputationMgr = ReputationManager;
    
    if (NetworkMgr)
    {
        NetworkMgr->OnPlayerConnected.AddDynamic(this, &UMingReputationReplication::OnPlayerJoined);
        NetworkMgr->OnPlayerDisconnected.AddDynamic(this, &UMingReputationReplication::OnPlayerLeft);
    }
    
    if (ReputationMgr)
    {
        // Bind to reputation change events
        // ReputationMgr->OnReputationChanged.AddDynamic(this, &UMingReputationReplication::OnReputationChangedInternal);
        // ReputationMgr->OnRankChanged.AddDynamic(this, &UMingReputationReplication::OnRankChangedInternal);
    }
    
    // Start sync timer
    if (UWorld* World = GEngine->GetWorldFromContextObject(this, EGetWorldErrorMode::LogAndReturnNull))
    {
        World->GetTimerManager().SetTimer(
            *(new FTimerHandle()),
            FTimerDelegate::CreateUObject(this, &UMingReputationReplication::OnNetworkTick, SyncInterval),
            SyncInterval,
            true
        );
    }
    
    UE_LOG(LogTemp, Log, TEXT("Reputation replication initialized"));
}

void UMingReputationReplication::ShutdownReplication()
{
    if (NetworkMgr)
    {
        NetworkMgr->OnPlayerConnected.RemoveAll(this);
        NetworkMgr->OnPlayerDisconnected.RemoveAll(this);
    }
    
    if (ReputationMgr)
    {
        // ReputationMgr->OnReputationChanged.RemoveAll(this);
        // ReputationMgr->OnRankChanged.RemoveAll(this);
    }
    
    CachedReputationData.Empty();
    PendingDeltas.Empty();
    PlayersInRegions.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("Reputation replication shutdown"));
}

void UMingReputationReplication::SetSyncMode(EMingReputationSyncMode Mode)
{
    CurrentSyncMode = Mode;
    UE_LOG(LogTemp, Log, TEXT("Reputation sync mode changed to: %d"), static_cast<int32>(Mode));
}

void UMingReputationReplication::SetSyncInterval(float IntervalSeconds)
{
    SyncInterval = FMath::Clamp(IntervalSeconds, 0.5f, 10.0f);
}

void UMingReputationReplication::SetRegionalSyncRadius(float Radius)
{
    RegionalSyncRadius = FMath::Clamp(Radius, 10000.0f, 200000.0f);
}

void UMingReputationReplication::RequestFullSync(int32 TargetPlayerID)
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    SendReputationSyncRequest(TargetPlayerID, true);
    UE_LOG(LogTemp, Log, TEXT("Requested full reputation sync for player %d"), TargetPlayerID);
}

void UMingReputationReplication::RequestRegionalSync(const TArray<FName>& RegionIDs)
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    TArray<FMingReplicatedReputationData> RegionalData = BuildRegionalSyncData(RegionIDs);
    
    // Serialize and send
    TArray<uint8> SerializedData = SerializeReputationData(RegionalData);
    NetworkMgr->SendMessage(NetworkMgr->GetLocalPlayerID(), 2, SerializedData, EMingSyncPriority::Normal, true);
    
    UE_LOG(LogTemp, Log, TEXT("Requested regional sync for %d regions"), RegionIDs.Num());
}

void UMingReputationReplication::ForceSyncAllRegions()
{
    if (!ReputationMgr)
    {
        return;
    }
    
    TArray<FName> AllRegions = ReputationMgr->GetAllRegionIDs();
    RequestRegionalSync(AllRegions);
}

void UMingReputationReplication::ServerUpdateReputation(int32 PlayerID, FName RegionID, float NewValue, const FString& Reason)
{
    if (!NetworkMgr || !NetworkMgr->IsHost() || !ReputationMgr)
    {
        return;
    }
    
    float OldValue = ReputationMgr->GetReputation(RegionID);
    ReputationMgr->SetReputation(RegionID, NewValue);
    
    // Create and broadcast delta
    FMingReputationSyncDelta Delta;
    Delta.RegionID = RegionID;
    Delta.OldValue = OldValue;
    Delta.NewValue = NewValue;
    Delta.OldRank = ReputationMgr->GetReputationRank(RegionID);
    Delta.NewRank = CalculateRank(NewValue);
    Delta.ChangeReason = Reason;
    Delta.SourcePlayerID = PlayerID;
    Delta.Timestamp = GetWorld()->GetTimeSeconds();
    
    BroadcastDeltaToClients(Delta);
    
    // Update regional statistics
    UpdateRegionalStatistics(RegionID);
    
    UE_LOG(LogTemp, Log, TEXT("Server updated reputation for %s: %.2f -> %.2f"), 
        *RegionID.ToString(), OldValue, NewValue);
}

void UMingReputationReplication::ServerUpdateReputationRank(int32 PlayerID, FName RegionID, int32 NewRank)
{
    if (!NetworkMgr || !NetworkMgr->IsHost() || !ReputationMgr)
    {
        return;
    }
    
    float CurrentReputation = ReputationMgr->GetReputation(RegionID);
    int32 OldRank = ReputationMgr->GetReputationRank(RegionID);
    
    // Update rank
    // ReputationMgr->SetReputationRank(RegionID, NewRank);
    
    UE_LOG(LogTemp, Log, TEXT("Server updated rank for %s: %d -> %d", *RegionID.ToString(), OldRank, NewRank));
}

void UMingReputationReplication::ServerCompleteQuest(int32 PlayerID, FName RegionID, const FString& QuestID)
{
    if (!NetworkMgr || !NetworkMgr->IsHost() || !ReputationMgr)
    {
        return;
    }
    
    // Add quest to completed list
    // ReputationMgr->AddCompletedQuest(RegionID, QuestID);
    
    // Update reputation based on quest completion
    float ReputationGain = 5.0f; // Base reputation gain
    float CurrentReputation = ReputationMgr->GetReputation(RegionID);
    ServerUpdateReputation(PlayerID, RegionID, CurrentReputation + ReputationGain, 
        FString::Printf(TEXT("Completed quest: %s"), *QuestID));
}

void UMingReputationReplication::ClientApplyReputationData(const TArray<FMingReplicatedReputationData>& ReputationData)
{
    if (!ReputationMgr)
    {
        return;
    }
    
    for (const FMingReplicatedReputationData& Data : ReputationData)
    {
        if (ValidateReputationData(Data))
        {
            ReputationMgr->SetReputation(Data.RegionID, Data.ReputationValue);
            // Apply other reputation properties
            
            CachedReputationData.Add(Data.RegionID, Data);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied reputation data for %d regions"), ReputationData.Num());
}

void UMingReputationReplication::ClientApplyReputationDelta(const FMingReputationSyncDelta& Delta)
{
    if (!ValidateDelta(Delta) || !ReputationMgr)
    {
        return;
    }
    
    ApplyDeltaToLocalData(Delta);
    
    OnReputationChanged.Broadcast(Delta.RegionID, Delta.NewValue);
    if (Delta.OldRank != Delta.NewRank)
    {
        OnRankChanged.Broadcast(Delta.RegionID, Delta.NewRank);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Applied reputation delta for %s"), *Delta.RegionID.ToString());
}

void UMingReputationReplication::ClientApplyRegionalData(const FMingRegionalSyncData& RegionalData)
{
    OnRegionalDataUpdated.Broadcast(RegionalData.RegionID);
    
    UE_LOG(LogTemp, Log, TEXT("Applied regional data for %s: %d players"), 
        *RegionalData.RegionID.ToString(), RegionalData.TotalPlayersInRegion);
}

bool UMingReputationReplication::ValidateReputationData(const FMingReplicatedReputationData& Data) const
{
    if (!Data.IsValid())
    {
        return false;
    }
    
    if (Data.ReputationValue < 0.0f || Data.ReputationValue > 100.0f)
    {
        return false;
    }
    
    if (Data.ReputationRank < 0 || Data.ReputationRank > 9)
    {
        return false;
    }
    
    return true;
}

bool UMingReputationReplication::ValidateDelta(const FMingReputationSyncDelta& Delta) const
{
    if (!Delta.RegionID.IsValid())
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

void UMingReputationReplication::ResolveConflict(const FMingReplicatedReputationData& LocalData, 
    const FMingReplicatedReputationData& RemoteData)
{
    // Server authority wins for reputation
    FMingReplicatedReputationData ResolvedData = RemoteData;
    
    CachedReputationData.Add(ResolvedData.RegionID, ResolvedData);
    
    UE_LOG(LogTemp, Log, TEXT("Resolved reputation conflict for %s using server data"), 
        *ResolvedData.RegionID.ToString());
}

float UMingReputationReplication::GetAverageSyncLatency() const
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

int32 UMingReputationReplication::GetPendingSyncCount() const
{
    return PendingDeltas.Num();
}

FMingRegionalSyncData UMingReputationReplication::GetRegionalData(FName RegionID) const
{
    FMingRegionalSyncData Data;
    Data.RegionID = RegionID;
    
    if (PlayersInRegions.Contains(RegionID))
    {
        const TArray<int32>& Players = PlayersInRegions[RegionID];
        Data.TotalPlayersInRegion = Players.Num();
        
        // Calculate average reputation
        if (ReputationMgr && Players.Num() > 0)
        {
            float TotalReputation = 0.0f;
            for (int32 PlayerID : Players)
            {
                // Get player reputation for this region
                TotalReputation += ReputationMgr->GetReputation(RegionID);
            }
            Data.RegionAverageReputation = TotalReputation / Players.Num();
        }
    }
    
    return Data;
}

void UMingReputationReplication::RegisterPlayerInRegion(int32 PlayerID, FName RegionID)
{
    if (!PlayersInRegions.Contains(RegionID))
    {
        PlayersInRegions.Add(RegionID, TArray<int32>());
    }
    
    TArray<int32>& Players = PlayersInRegions[RegionID];
    if (!Players.Contains(PlayerID))
    {
        Players.Add(PlayerID);
        UE_LOG(LogTemp, Log, TEXT("Registered player %d in region %s"), PlayerID, *RegionID.ToString());
    }
}

void UMingReputationReplication::UnregisterPlayerFromRegion(int32 PlayerID, FName RegionID)
{
    if (PlayersInRegions.Contains(RegionID))
    {
        PlayersInRegions[RegionID].Remove(PlayerID);
        UE_LOG(LogTemp, Log, TEXT("Unregistered player %d from region %s"), PlayerID, *RegionID.ToString());
    }
}

TArray<int32> UMingReputationReplication::GetPlayersInRegion(FName RegionID) const
{
    if (PlayersInRegions.Contains(RegionID))
    {
        return PlayersInRegions[RegionID];
    }
    
    return TArray<int32>();
}

bool UMingReputationReplication::IsPlayerInRegion(int32 PlayerID, FName RegionID) const
{
    if (PlayersInRegions.Contains(RegionID))
    {
        return PlayersInRegions[RegionID].Contains(PlayerID);
    }
    
    return false;
}

void UMingReputationReplication::CompressSyncData(TArray<FMingReplicatedReputationData>& SyncData)
{
    // Compression implementation
}

void UMingReputationReplication::DecompressSyncData(TArray<FMingReplicatedReputationData>& SyncData)
{
    // Decompression implementation
}

int32 UMingReputationReplication::CalculateSyncDataSize(const TArray<FMingReplicatedReputationData>& SyncData) const
{
    int32 Size = 0;
    for (const FMingReplicatedReputationData& Data : SyncData)
    {
        Size += sizeof(FMingReplicatedReputationData);
        Size += Data.ActiveQuests.Num() * sizeof(FString);
        Size += Data.CompletedQuests.Num() * sizeof(FString);
        Size += Data.ActiveEffects.Num() * sizeof(FString);
    }
    return Size;
}

void UMingReputationReplication::PerformFullSync()
{
    if (!NetworkMgr || !NetworkMgr->IsConnected())
    {
        return;
    }
    
    TArray<FMingReplicatedReputationData> FullData = BuildFullSyncData();
    TArray<uint8> SerializedData = SerializeReputationData(FullData);
    
    NetworkMgr->SendMessage(NetworkMgr->GetLocalPlayerID(), 2, SerializedData, EMingSyncPriority::High, true);
}

void UMingReputationReplication::PerformDeltaSync()
{
    if (!NetworkMgr || !NetworkMgr->IsConnected() || PendingDeltas.Num() == 0)
    {
        return;
    }
    
    for (const FMingReputationSyncDelta& Delta : PendingDeltas)
    {
        TArray<uint8> SerializedDelta = SerializeDelta(Delta);
        NetworkMgr->BroadcastMessage(3, SerializedDelta, EMingSyncPriority::Normal);
    }
    
    PendingDeltas.Empty();
}

void UMingReputationReplication::PerformEventDrivenSync()
{
    // Event-driven sync handled by callbacks
}

void UMingReputationReplication::PerformRegionalSync()
{
    if (!NetworkMgr || !NetworkMgr->IsConnected() || !ReputationMgr)
    {
        return;
    }
    
    // Get regions within sync radius
    TArray<FName> NearbyRegions;
    TArray<FName> AllRegions = ReputationMgr->GetAllRegionIDs();
    
    for (const FName& RegionID : AllRegions)
    {
        if (IsInRegionalSyncRange(RegionID))
        {
            NearbyRegions.Add(RegionID);
        }
    }
    
    if (NearbyRegions.Num() > 0)
    {
        RequestRegionalSync(NearbyRegions);
    }
}

void UMingReputationReplication::SendReputationSyncRequest(int32 TargetPlayerID, bool bFullSync)
{
    // Send sync request to server
    TArray<uint8> RequestData;
    RequestData.Add(bFullSync ? 1 : 0);
    
    NetworkMgr->SendMessage(TargetPlayerID, 2, RequestData, EMingSyncPriority::High, true);
}

void UMingReputationReplication::ProcessReputationSyncResponse(const TArray<FMingReplicatedReputationData>& ReputationData)
{
    ClientApplyReputationData(ReputationData);
    RecordSyncOperation(true);
}

void UMingReputationReplication::TrackSyncLatency(float Latency)
{
    SyncLatencies.Add(Latency);
    if (SyncLatencies.Num() > 100)
    {
        SyncLatencies.RemoveAt(0);
    }
}

void UMingReputationReplication::RecordSyncOperation(bool bSuccess)
{
    TotalSyncOperations++;
    if (!bSuccess)
    {
        FailedSyncOperations++;
    }
}

bool UMingReputationReplication::ShouldSyncRegion(FName RegionID) const
{
    switch (CurrentSyncMode)
    {
    case EMingReputationSyncMode::FullSync:
        return true;
    case EMingReputationSyncMode::DeltaSync:
        return true;
    case EMingReputationSyncMode::EventDriven:
        return true;
    case EMingReputationSyncMode::RegionalOnly:
        return IsInRegionalSyncRange(RegionID);
    default:
        return true;
    }
}

bool UMingReputationReplication::IsInRegionalSyncRange(FName RegionID) const
{
    // Check if region is within sync radius
    return true; // Placeholder
}

TArray<FMingReplicatedReputationData> UMingReputationReplication::BuildFullSyncData()
{
    TArray<FMingReplicatedReputationData> Result;
    
    if (!ReputationMgr)
    {
        return Result;
    }
    
    TArray<FName> AllRegions = ReputationMgr->GetAllRegionIDs();
    for (const FName& RegionID : AllRegions)
    {
        FMingReplicatedReputationData Data;
        Data.RegionID = RegionID;
        Data.ReputationValue = ReputationMgr->GetReputation(RegionID);
        Data.ReputationRank = ReputationMgr->GetReputationRank(RegionID);
        Data.SyncVersion = SyncVersion;
        Data.LastSyncTime = GetWorld()->GetTimeSeconds();
        
        Result.Add(Data);
    }
    
    return Result;
}

TArray<FMingReplicatedReputationData> UMingReputationReplication::BuildRegionalSyncData(const TArray<FName>& RegionIDs)
{
    TArray<FMingReplicatedReputationData> Result;
    
    if (!ReputationMgr)
    {
        return Result;
    }
    
    for (const FName& RegionID : RegionIDs)
    {
        if (ShouldSyncRegion(RegionID))
        {
            FMingReplicatedReputationData Data;
            Data.RegionID = RegionID;
            Data.ReputationValue = ReputationMgr->GetReputation(RegionID);
            Data.ReputationRank = ReputationMgr->GetReputationRank(RegionID);
            Data.SyncVersion = SyncVersion;
            Data.LastSyncTime = GetWorld()->GetTimeSeconds();
            
            Result.Add(Data);
        }
    }
    
    return Result;
}

TArray<FMingReplicatedReputationData> UMingReputationReplication::BuildDeltaSyncData()
{
    TArray<FMingReplicatedReputationData> Result;
    // Build delta sync data based on pending changes
    return Result;
}

void UMingReputationReplication::ApplyDeltaToLocalData(const FMingReputationSyncDelta& Delta)
{
    if (!ReputationMgr)
    {
        return;
    }
    
    ReputationMgr->SetReputation(Delta.RegionID, Delta.NewValue);
    
    // Update cache
    if (CachedReputationData.Contains(Delta.RegionID))
    {
        CachedReputationData[Delta.RegionID].ReputationValue = Delta.NewValue;
        CachedReputationData[Delta.RegionID].ReputationRank = Delta.NewRank;
    }
}

void UMingReputationReplication::BroadcastDeltaToClients(const FMingReputationSyncDelta& Delta)
{
    if (!NetworkMgr)
    {
        return;
    }
    
    TArray<uint8> SerializedDelta = SerializeDelta(Delta);
    NetworkMgr->BroadcastMessage(3, SerializedDelta, EMingSyncPriority::Normal);
}

void UMingReputationReplication::UpdateRegionalStatistics(FName RegionID)
{
    FMingRegionalSyncData Data = GetRegionalData(RegionID);
    
    // Broadcast regional update to interested clients
    OnRegionalDataUpdated.Broadcast(RegionID);
}

void UMingReputationReplication::OnNetworkTick(float DeltaTime)
{
    LastSyncTime += DeltaTime;
    
    if (LastSyncTime >= SyncInterval)
    {
        switch (CurrentSyncMode)
        {
        case EMingReputationSyncMode::FullSync:
            PerformFullSync();
            break;
        case EMingReputationSyncMode::DeltaSync:
            PerformDeltaSync();
            break;
        case EMingReputationSyncMode::EventDriven:
            PerformEventDrivenSync();
            break;
        case EMingReputationSyncMode::RegionalOnly:
            PerformRegionalSync();
            break;
        }
        
        LastSyncTime = 0.0f;
    }
}

void UMingReputationReplication::OnPlayerJoined(int32 PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d joined, sending reputation data"), PlayerID);
    
    if (NetworkMgr && NetworkMgr->IsHost())
    {
        // Send full reputation data to new player
        TArray<FMingReplicatedReputationData> FullData = BuildFullSyncData();
        TArray<uint8> SerializedData = SerializeReputationData(FullData);
        NetworkMgr->SendMessage(PlayerID, 2, SerializedData, EMingSyncPriority::High, true);
    }
}

void UMingReputationReplication::OnPlayerLeft(int32 PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d left, cleaning up reputation data"), PlayerID);
    
    // Remove player from all regions
    for (auto& Pair : PlayersInRegions)
    {
        Pair.Value.Remove(PlayerID);
    }
}

void UMingReputationReplication::OnReputationChangedInternal(FName RegionID, float OldValue, float NewValue)
{
    FMingReputationSyncDelta Delta;
    Delta.RegionID = RegionID;
    Delta.OldValue = OldValue;
    Delta.NewValue = NewValue;
    Delta.OldRank = CalculateRank(OldValue);
    Delta.NewRank = CalculateRank(NewValue);
    Delta.SourcePlayerID = NetworkMgr ? NetworkMgr->GetLocalPlayerID() : -1;
    Delta.Timestamp = GetWorld()->GetTimeSeconds();
    
    PendingDeltas.Add(Delta);
    
    if (CurrentSyncMode == EMingReputationSyncMode::EventDriven)
    {
        BroadcastDeltaToClients(Delta);
    }
    
    OnReputationChanged.Broadcast(RegionID, NewValue);
}

void UMingReputationReplication::OnRankChangedInternal(FName RegionID, int32 OldRank, int32 NewRank)
{
    OnRankChanged.Broadcast(RegionID, NewRank);
}

int32 UMingReputationReplication::CalculateRank(float ReputationValue) const
{
    return FMath::FloorToInt(ReputationValue / 10.0f);
}

TArray<uint8> UMingReputationReplication::SerializeReputationData(const TArray<FMingReplicatedReputationData>& ReputationData)
{
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);
    
    int32 Count = ReputationData.Num();
    Writer << Count;
    
    for (const FMingReplicatedReputationData& RepData : ReputationData)
    {
        Writer << const_cast<FMingReplicatedReputationData&>(RepData);
    }
    
    return Data;
}

TArray<FMingReplicatedReputationData> UMingReputationReplication::DeserializeReputationData(const TArray<uint8>& Data)
{
    TArray<FMingReplicatedReputationData> Result;
    FMemoryReader Reader(Data);
    
    int32 Count;
    Reader << Count;
    
    for (int32 i = 0; i < Count; i++)
    {
        FMingReplicatedReputationData RepData;
        Reader << RepData;
        Result.Add(RepData);
    }
    
    return Result;
}

TArray<uint8> UMingReputationReplication::SerializeDelta(const FMingReputationSyncDelta& Delta)
{
    TArray<uint8> Data;
    FMemoryWriter Writer(Data);
    Writer << const_cast<FMingReputationSyncDelta&>(Delta);
    return Data;
}

FMingReputationSyncDelta UMingReputationReplication::DeserializeDelta(const TArray<uint8>& Data)
{
    FMingReputationSyncDelta Delta;
    FMemoryReader Reader(Data);
    Reader << Delta;
    return Delta;
}
