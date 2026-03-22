// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Multiplayer Relationship Manager Implementation - B1-2

#include "Network/MingMultiplayerRelationshipManager.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "TimerManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogMultiplayerRelations, Log, All);

UMingMultiplayerRelationshipManager::UMingMultiplayerRelationshipManager()
    : bInitialized(false)
    , ServerAuthorityPlayerID(0)
    , RelationDecayRate(0.1f)
    , TrustDecayRate(0.05f)
{
}

void UMingMultiplayerRelationshipManager::InitializeRelationManager()
{
    bInitialized = true;
    Relations.Empty();
    Players.Empty();
    Alliances.Empty();
    PendingUpdates.Empty();

    // Start relation decay timer
    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().SetTimer(
            RelationDecayTimer,
            this,
            &UMingMultiplayerRelationshipManager::DecayRelationsOverTime,
            60.0f,
            true
        );
    }

    UE_LOG(LogMultiplayerRelations, Log, TEXT("Multiplayer Relationship Manager initialized"));
}

void UMingMultiplayerRelationshipManager::ShutdownRelationManager()
{
    bInitialized = false;

    if (GEngine && GEngine->GetCurrentWorldContext())
    {
        GEngine->GetCurrentWorldContext()->World()->GetTimerManager().ClearTimer(RelationDecayTimer);
    }

    UE_LOG(LogMultiplayerRelations, Log, TEXT("Multiplayer Relationship Manager shutdown"));
}

void UMingMultiplayerRelationshipManager::RegisterPlayer(const FMultiplayerPlayerInfo& PlayerInfo)
{
    if (!bInitialized) return;

    Players.Add(PlayerInfo.PlayerID, PlayerInfo);
    UE_LOG(LogMultiplayerRelations, Log, TEXT("Registered player %d: %s"), PlayerInfo.PlayerID, *PlayerInfo.PlayerName);
}

void UMingMultiplayerRelationshipManager::UnregisterPlayer(int32 PlayerID)
{
    if (!bInitialized) return;

    // Remove all relations involving this player
    for (int32 i = Relations.Num() - 1; i >= 0; --i)
    {
        if (Relations[i].PlayerID1 == PlayerID || Relations[i].PlayerID2 == PlayerID)
        {
            Relations.RemoveAt(i);
        }
    }

    Players.Remove(PlayerID);

    // Remove from alliances
    for (auto& Pair : Alliances)
    {
        Pair.Value.Remove(PlayerID);
    }

    UE_LOG(LogMultiplayerRelations, Log, TEXT("Unregistered player %d"), PlayerID);
}

bool UMingMultiplayerRelationshipManager::EstablishRelation(int32 PlayerID1, int32 PlayerID2, EMultiplayerRelationType Type)
{
    if (!bInitialized) return false;
    if (!Players.Contains(PlayerID1) || !Players.Contains(PlayerID2)) return false;

    // Check if relation already exists
    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            // Update existing relation
            Relation.RelationType = Type;
            Relation.Status = EMultiplayerRelationStatus::Active;
            Relation.LastInteractionTime = FPlatformTime::Seconds();
            NotifyClientsOfChange(PlayerID1, PlayerID2);
            return true;
        }
    }

    // Create new relation
    FMultiplayerRelationEntry NewRelation;
    NewRelation.PlayerID1 = PlayerID1;
    NewRelation.PlayerID2 = PlayerID2;
    NewRelation.RelationType = Type;
    NewRelation.Status = EMultiplayerRelationStatus::Active;
    NewRelation.RelationValue = 50.0f;
    NewRelation.EstablishedTime = FPlatformTime::Seconds();
    NewRelation.LastInteractionTime = NewRelation.EstablishedTime;
    NewRelation.TrustLevel = 50;

    Relations.Add(NewRelation);

    OnMultiplayerRelationChanged.Broadcast(PlayerID1, PlayerID2);
    UE_LOG(LogMultiplayerRelations, Log, TEXT("Established %s relation between %d and %d"),
        *UEnum::GetValueAsString(Type), PlayerID1, PlayerID2);

    return true;
}

bool UMingMultiplayerRelationshipManager::BreakRelation(int32 PlayerID1, int32 PlayerID2, ERelationChangeReason Reason)
{
    if (!bInitialized) return false;

    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            Relation.Status = EMultiplayerRelationStatus::Broken;
            Relation.RelationValue = FMath::Max(0.0f, Relation.RelationValue - 30.0f);

            OnPlayerRelationBroken.Broadcast(PlayerID1, PlayerID2);
            NotifyClientsOfChange(PlayerID1, PlayerID2);

            UE_LOG(LogMultiplayerRelations, Log, TEXT("Broke relation between %d and %d. Reason: %s"),
                PlayerID1, PlayerID2, *UEnum::GetValueAsString(Reason));
            return true;
        }
    }

    return false;
}

bool UMingMultiplayerRelationshipManager::ModifyRelationValue(int32 PlayerID1, int32 PlayerID2, float Delta, ERelationChangeReason Reason)
{
    if (!bInitialized) return false;

    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            float OldValue = Relation.RelationValue;
            Relation.RelationValue = FMath::Clamp(Relation.RelationValue + Delta, 0.0f, 100.0f);
            Relation.LastInteractionTime = FPlatformTime::Seconds();

            // Update trust based on positive interactions
            if (Delta > 0)
            {
                Relation.TrustLevel = FMath::Min(100, Relation.TrustLevel + static_cast<int32>(Delta / 5.0f));
            }
            else if (Delta < -20.0f)
            {
                Relation.TrustLevel = FMath::Max(0, Relation.TrustLevel + static_cast<int32>(Delta / 10.0f));
            }

            NotifyClientsOfChange(PlayerID1, PlayerID2);

            UE_LOG(LogMultiplayerRelations, Verbose, TEXT("Modified relation value between %d and %d: %.1f -> %.1f"),
                PlayerID1, PlayerID2, OldValue, Relation.RelationValue);
            return true;
        }
    }

    return false;
}

FMultiplayerRelationEntry UMingMultiplayerRelationshipManager::GetRelation(int32 PlayerID1, int32 PlayerID2) const
{
    for (const auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            return Relation;
        }
    }
    return FMultiplayerRelationEntry();
}

TArray<FMultiplayerRelationEntry> UMingMultiplayerRelationshipManager::GetAllRelations(int32 PlayerID) const
{
    TArray<FMultiplayerRelationEntry> PlayerRelations;
    for (const auto& Relation : Relations)
    {
        if (Relation.PlayerID1 == PlayerID || Relation.PlayerID2 == PlayerID)
        {
            PlayerRelations.Add(Relation);
        }
    }
    return PlayerRelations;
}

TArray<FMultiplayerPlayerInfo> UMingMultiplayerRelationshipManager::GetAllies(int32 PlayerID) const
{
    TArray<FMultiplayerPlayerInfo> Allies;
    for (const auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID || Relation.PlayerID2 == PlayerID) &&
            Relation.RelationType == EMultiplayerRelationType::Ally &&
            Relation.Status == EMultiplayerRelationStatus::Active)
        {
            int32 AllyID = (Relation.PlayerID1 == PlayerID) ? Relation.PlayerID2 : Relation.PlayerID1;
            if (Players.Contains(AllyID))
            {
                Allies.Add(Players[AllyID]);
            }
        }
    }
    return Allies;
}

TArray<FMultiplayerPlayerInfo> UMingMultiplayerRelationshipManager::GetEnemies(int32 PlayerID) const
{
    TArray<FMultiplayerPlayerInfo> Enemies;
    for (const auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID || Relation.PlayerID2 == PlayerID) &&
            Relation.RelationType == EMultiplayerRelationType::Enemy &&
            Relation.Status == EMultiplayerRelationStatus::Active)
        {
            int32 EnemyID = (Relation.PlayerID1 == PlayerID) ? Relation.PlayerID2 : Relation.PlayerID1;
            if (Players.Contains(EnemyID))
            {
                Enemies.Add(Players[EnemyID]);
            }
        }
    }
    return Enemies;
}

bool UMingMultiplayerRelationshipManager::CanPlayerAttack(int32 AttackerID, int32 TargetID) const
{
    // Check if they are enemies or have hostile relations
    FMultiplayerRelationEntry Relation = GetRelation(AttackerID, TargetID);

    if (Relation.RelationType == EMultiplayerRelationType::Enemy)
    {
        return true;
    }

    if (Relation.RelationType == EMultiplayerRelationType::Ally)
    {
        return false; // Cannot attack allies
    }

    // Neutral players can be attacked in most cases
    return true;
}

bool UMingMultiplayerRelationshipManager::CanPlayerTrade(int32 PlayerID1, int32 PlayerID2) const
{
    FMultiplayerRelationEntry Relation = GetRelation(PlayerID1, PlayerID2);

    // Cannot trade with enemies
    if (Relation.RelationType == EMultiplayerRelationType::Enemy)
    {
        return false;
    }

    return Relation.Status == EMultiplayerRelationStatus::Active ||
           Relation.RelationType == EMultiplayerRelationType::TradePartner;
}

void UMingMultiplayerRelationshipManager::RecordInteraction(int32 PlayerID1, int32 PlayerID2, const FString& InteractionType)
{
    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            Relation.LastInteractionTime = FPlatformTime::Seconds();
            break;
        }
    }
}

void UMingMultiplayerRelationshipManager::UpdateTrustLevel(int32 PlayerID1, int32 PlayerID2, int32 TrustDelta)
{
    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            Relation.TrustLevel = FMath::Clamp(Relation.TrustLevel + TrustDelta, 0, 100);
            UE_LOG(LogMultiplayerRelations, Verbose, TEXT("Trust level updated between %d and %d: %d"),
                PlayerID1, PlayerID2, Relation.TrustLevel);
            break;
        }
    }
}

bool UMingMultiplayerRelationshipManager::FormAlliance(const TArray<int32>& AllianceMembers, const FString& AllianceName)
{
    if (AllianceMembers.Num() < 2) return false;

    int32 AllianceID = Alliances.Num();
    Alliances.Add(AllianceID, AllianceMembers);

    // Establish ally relations between all members
    for (int32 i = 0; i < AllianceMembers.Num(); ++i)
    {
        for (int32 j = i + 1; j < AllianceMembers.Num(); ++j)
        {
            EstablishRelation(AllianceMembers[i], AllianceMembers[j], EMultiplayerRelationType::Ally);
        }
    }

    OnPlayerAllianceFormed.Broadcast(AllianceMembers);
    UE_LOG(LogMultiplayerRelations, Log, TEXT("Alliance '%s' formed with %d members"), *AllianceName, AllianceMembers.Num());

    return true;
}

bool UMingMultiplayerRelationshipManager::DissolveAlliance(const TArray<int32>& AllianceMembers)
{
    // Find and remove the alliance
    for (auto It = Alliances.CreateIterator(); It; ++It)
    {
        if (It.Value() == AllianceMembers)
        {
            // Break relations between members
            for (int32 i = 0; i < AllianceMembers.Num(); ++i)
            {
                for (int32 j = i + 1; j < AllianceMembers.Num(); ++j)
                {
                    BreakRelation(AllianceMembers[i], AllianceMembers[j], ERelationChangeReason::PlayerAction);
                }
            }

            It.RemoveCurrent();
            UE_LOG(LogMultiplayerRelations, Log, TEXT("Alliance dissolved"));
            return true;
        }
    }
    return false;
}

void UMingMultiplayerRelationshipManager::SetSharedObjective(int32 PlayerID1, int32 PlayerID2, const FString& Objective)
{
    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            if (!Relation.SharedObjectives.Contains(Objective))
            {
                Relation.SharedObjectives.Add(Objective);
            }
            break;
        }
    }
}

void UMingMultiplayerRelationshipManager::RecordConflict(int32 PlayerID1, int32 PlayerID2, const FString& ConflictDescription)
{
    for (auto& Relation : Relations)
    {
        if ((Relation.PlayerID1 == PlayerID1 && Relation.PlayerID2 == PlayerID2) ||
            (Relation.PlayerID1 == PlayerID2 && Relation.PlayerID2 == PlayerID1))
        {
            Relation.ConflictHistory.Add(ConflictDescription);
            // Negative impact on relation
            ModifyRelationValue(PlayerID1, PlayerID2, -10.0f, ERelationChangeReason::ResourceDispute);
            break;
        }
    }
}

void UMingMultiplayerRelationshipManager::ReplicateRelationData(int32 TargetPlayerID)
{
    // Replicate relation data to specific client
    UE_LOG(LogMultiplayerRelations, Verbose, TEXT("Replicating relation data to player %d"), TargetPlayerID);
}

void UMingMultiplayerRelationshipManager::SynchronizeRelationsAcrossClients()
{
    // Synchronize all relation data across all connected clients
    for (const auto& Relation : Relations)
    {
        NotifyClientsOfChange(Relation.PlayerID1, Relation.PlayerID2);
    }
}

void UMingMultiplayerRelationshipManager::HandlePlayerDisconnect(int32 PlayerID)
{
    if (Players.Contains(PlayerID))
    {
        Players[PlayerID].bIsOnline = false;
        Players[PlayerID].LastUpdateTimestamp = FPlatformTime::Seconds();
    }

    UE_LOG(LogMultiplayerRelations, Log, TEXT("Player %d disconnected"), PlayerID);
}

void UMingMultiplayerRelationshipManager::HandlePlayerReconnect(int32 PlayerID)
{
    if (Players.Contains(PlayerID))
    {
        Players[PlayerID].bIsOnline = true;
        Players[PlayerID].LastUpdateTimestamp = FPlatformTime::Seconds();

        // Sync offline changes
        SyncOfflinePlayerRelations(PlayerID);
    }

    UE_LOG(LogMultiplayerRelations, Log, TEXT("Player %d reconnected"), PlayerID);
}

int32 UMingMultiplayerRelationshipManager::GetOnlinePlayerCount() const
{
    int32 Count = 0;
    for (const auto& Pair : Players)
    {
        if (Pair.Value.bIsOnline)
        {
            Count++;
        }
    }
    return Count;
}

void UMingMultiplayerRelationshipManager::ProcessRelationUpdates()
{
    ProcessQueuedUpdates();
}

void UMingMultiplayerRelationshipManager::DecayRelationsOverTime()
{
    uint32 CurrentTime = FPlatformTime::Seconds();

    for (auto& Relation : Relations)
    {
        uint32 TimeSinceInteraction = CurrentTime - Relation.LastInteractionTime;

        if (TimeSinceInteraction > 300) // 5 minutes
        {
            float Decay = CalculateRelationDecay(Relation.RelationValue, TimeSinceInteraction);
            Relation.RelationValue = FMath::Max(0.0f, Relation.RelationValue - Decay);
        }

        // Decay trust over time for non-allies
        if (Relation.RelationType != EMultiplayerRelationType::Ally &&
            Relation.RelationType != EMultiplayerRelationType::Friend)
        {
            Relation.TrustLevel = FMath::Max(0, Relation.TrustLevel - 1);
        }
    }
}

bool UMingMultiplayerRelationshipManager::AreInAlliance(int32 PlayerID1, int32 PlayerID2) const
{
    for (const auto& Pair : Alliances)
    {
        const TArray<int32>& Members = Pair.Value;
        if (Members.Contains(PlayerID1) && Members.Contains(PlayerID2))
        {
            return true;
        }
    }
    return false;
}

void UMingMultiplayerRelationshipManager::NotifyClientsOfChange(int32 PlayerID1, int32 PlayerID2)
{
    // Notify clients about relation change
    FMultiplayerRelationUpdate Update;
    Update.PlayerID1 = PlayerID1;
    Update.PlayerID2 = PlayerID2;
    Update.Timestamp = FPlatformTime::Seconds();

    BroadcastRelationUpdate(Update);
}

void UMingMultiplayerRelationshipManager::BroadcastRelationUpdate(const FMultiplayerRelationUpdate& Update)
{
    // Broadcast to all connected clients
    UE_LOG(LogMultiplayerRelations, Verbose, TEXT("Broadcasting relation update: %d <-> %d"), Update.PlayerID1, Update.PlayerID2);
}

bool UMingMultiplayerRelationshipManager::ValidateRelationRequest(int32 PlayerID1, int32 PlayerID2, EMultiplayerRelationType Type) const
{
    // Check if players exist
    if (!Players.Contains(PlayerID1) || !Players.Contains(PlayerID2)) return false;

    // Check if already has conflicting relation
    FMultiplayerRelationEntry ExistingRelation = GetRelation(PlayerID1, PlayerID2);
    if (ExistingRelation.Status == EMultiplayerRelationStatus::Active)
    {
        if (ExistingRelation.RelationType == EMultiplayerRelationType::Enemy && Type == EMultiplayerRelationType::Ally)
        {
            return false; // Cannot ally with enemies directly
        }
    }

    return true;
}

float UMingMultiplayerRelationshipManager::CalculateRelationDecay(float CurrentValue, uint32 TimeElapsed) const
{
    // Exponential decay based on time
    float DecayFactor = FMath::Exp(-TimeElapsed / 3600.0f); // 1 hour time constant
    return CurrentValue * (1.0f - DecayFactor) * RelationDecayRate;
}

void UMingMultiplayerRelationshipManager::HandleRelationExpiration()
{
    uint32 CurrentTime = FPlatformTime::Seconds();

    for (auto& Relation : Relations)
    {
        if (Relation.Status == EMultiplayerRelationStatus::Pending)
        {
            // Expire pending relations after 10 minutes
            if (CurrentTime - Relation.EstablishedTime > 600)
            {
                Relation.Status = EMultiplayerRelationStatus::Expired;
            }
        }
    }
}

void UMingMultiplayerRelationshipManager::UpdateAllianceRelations(int32 AllianceID)
{
    if (!Alliances.Contains(AllianceID)) return;

    const TArray<int32>& Members = Alliances[AllianceID];

    // Update all pairwise relations
    for (int32 i = 0; i < Members.Num(); ++i)
    {
        for (int32 j = i + 1; j < Members.Num(); ++j)
        {
            ModifyRelationValue(Members[i], Members[j], 5.0f, ERelationChangeReason::PlayerAction);
        }
    }
}

void UMingMultiplayerRelationshipManager::SyncOfflinePlayerRelations(int32 PlayerID)
{
    // Send all accumulated updates to reconnected player
    for (const auto& Update : PendingUpdates)
    {
        if (Update.PlayerID1 == PlayerID || Update.PlayerID2 == PlayerID)
        {
            // Send update to player
        }
    }
}

void UMingMultiplayerRelationshipManager::QueueRelationUpdate(const FMultiplayerRelationUpdate& Update)
{
    PendingUpdates.Add(Update);
}

void UMingMultiplayerRelationshipManager::ProcessQueuedUpdates()
{
    for (const auto& Update : PendingUpdates)
    {
        // Process each update
        ModifyRelationValue(Update.PlayerID1, Update.PlayerID2, Update.RelationDelta, Update.Reason);
    }
    PendingUpdates.Empty();
}

static UMingMultiplayerRelationshipManager* UMingMultiplayerRelationshipManager::Get(UObject* WorldContextObject)
{
    static UMingMultiplayerRelationshipManager* Instance = nullptr;
    if (!Instance)
    {
        Instance = NewObject<UMingMultiplayerRelationshipManager>();
        Instance->AddToRoot();
    }
    return Instance;
}
