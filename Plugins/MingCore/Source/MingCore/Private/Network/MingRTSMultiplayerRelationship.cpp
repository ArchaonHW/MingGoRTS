// Copyright Epic Games, Inc. All Rights Reserved.

#include "Network/MingRTSMultiplayerRelationship.h"
#include "Network/MingRTSNetworkManager.h"
#include "Engine/Engine.h"

UMingRTSMultiplayerRelationship::UMingRTSMultiplayerRelationship()
	: LocalPlayerId(0)
	, NetworkManager(nullptr)
	, RelationUpdateInterval(1.0f)
	, TimeSinceLastUpdate(0.0f)
{
}

bool UMingRTSMultiplayerRelationship::InitializeSystem(UMingRTSNetworkManager* InNetworkManager)
{
	if (!InNetworkManager)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot initialize multiplayer relationship system with null network manager"));
		return false;
	}

	NetworkManager = InNetworkManager;
	
	// Bind to network events
	NetworkManager->OnPlayerJoined.AddDynamic(this, &UMingRTSMultiplayerRelationship::OnNetworkPlayerJoined);
	NetworkManager->OnPlayerLeft.AddDynamic(this, &UMingRTSMultiplayerRelationship::OnNetworkPlayerLeft);

	UE_LOG(LogTemp, Log, TEXT("Multiplayer Relationship System initialized"));
	return true;
}

void UMingRTSMultiplayerRelationship::ShutdownSystem()
{
	if (NetworkManager)
	{
		NetworkManager->OnPlayerJoined.RemoveDynamic(this, &UMingRTSMultiplayerRelationship::OnNetworkPlayerJoined);
		NetworkManager->OnPlayerLeft.RemoveDynamic(this, &UMingRTSMultiplayerRelationship::OnNetworkPlayerLeft);
	}

	ClearAllRelations();
	NetworkManager = nullptr;

	UE_LOG(LogTemp, Log, TEXT("Multiplayer Relationship System shut down"));
}

void UMingRTSMultiplayerRelationship::SetLocalPlayerId(int32 PlayerId)
{
	LocalPlayerId = PlayerId;
}

int32 UMingRTSMultiplayerRelationship::GetLocalPlayerId() const
{
	return LocalPlayerId;
}

void UMingRTSMultiplayerRelationship::SetDiplomaticRelation(int32 PlayerId1, int32 PlayerId2, EMultiplayerRelationType RelationType)
{
	if (!ArePlayersValid(PlayerId1, PlayerId2))
	{
		return;
	}

	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	FPlayerDiplomaticRelation& Relation = DiplomaticRelations.FindOrAdd(Key);
	
	Relation.PlayerId1 = FMath::Min(PlayerId1, PlayerId2);
	Relation.PlayerId2 = FMath::Max(PlayerId1, PlayerId2);
	
	EMultiplayerRelationType OldType = Relation.RelationType;
	Relation.RelationType = RelationType;
	Relation.LastInteraction = FDateTime::Now();

	// Handle specific relation type changes
	switch (RelationType)
	{
	case EMultiplayerRelationType::Ally:
		Relation.AllianceDuration = 0;
		break;
	case EMultiplayerRelationType::War:
		Relation.bHasNonAggressionPact = false;
		Relation.NonAggressionExpiration = 0.0f;
		break;
	case EMultiplayerRelationType::Neutral:
		Relation.TradeAgreements.Empty();
		break;
	default:
		break;
	}

	// Sync to network if we initiated the change
	if (PlayerId1 == LocalPlayerId || PlayerId2 == LocalPlayerId)
	{
		SyncRelationToNetwork(Relation);
	}

	BroadcastRelationChange(PlayerId1, PlayerId2);

	UE_LOG(LogTemp, Log, TEXT("Diplomatic relation changed between %d and %d: %s"), 
		PlayerId1, PlayerId2, *UEnum::GetValueAsString(RelationType));
}

EMultiplayerRelationType UMingRTSMultiplayerRelationship::GetDiplomaticRelation(int32 PlayerId1, int32 PlayerId2) const
{
	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	const FPlayerDiplomaticRelation* Found = DiplomaticRelations.Find(Key);
	return Found ? Found->RelationType : EMultiplayerRelationType::Neutral;
}

FPlayerDiplomaticRelation UMingRTSMultiplayerRelationship::GetRelationDetails(int32 PlayerId1, int32 PlayerId2) const
{
	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	const FPlayerDiplomaticRelation* Found = DiplomaticRelations.Find(Key);
	return Found ? *Found : FPlayerDiplomaticRelation();
}

TArray<FPlayerDiplomaticRelation> UMingRTSMultiplayerRelationship::GetAllRelationsForPlayer(int32 PlayerId) const
{
	TArray<FPlayerDiplomaticRelation> Result;
	for (const auto& Pair : DiplomaticRelations)
	{
		if (Pair.Value.InvolvesPlayer(PlayerId))
		{
			Result.Add(Pair.Value);
		}
	}
	return Result;
}

TArray<FPlayerDiplomaticRelation> UMingRTSMultiplayerRelationship::GetAllRelations() const
{
	TArray<FPlayerDiplomaticRelation> Result;
	DiplomaticRelations.GenerateValueArray(Result);
	return Result;
}

TArray<int32> UMingRTSMultiplayerRelationship::GetAllies(int32 PlayerId) const
{
	TArray<int32> Allies;
	for (const auto& Pair : DiplomaticRelations)
	{
		if (Pair.Value.InvolvesPlayer(PlayerId) && Pair.Value.RelationType == EMultiplayerRelationType::Ally)
		{
			Allies.Add(Pair.Value.GetOtherPlayer(PlayerId));
		}
	}
	return Allies;
}

TArray<int32> UMingRTSMultiplayerRelationship::GetEnemies(int32 PlayerId) const
{
	TArray<int32> Enemies;
	for (const auto& Pair : DiplomaticRelations)
	{
		if (Pair.Value.InvolvesPlayer(PlayerId) && Pair.Value.RelationType == EMultiplayerRelationType::War)
		{
			Enemies.Add(Pair.Value.GetOtherPlayer(PlayerId));
		}
	}
	return Enemies;
}

TArray<int32> UMingRTSMultiplayerRelationship::GetTradePartners(int32 PlayerId) const
{
	TArray<int32> Partners;
	for (const auto& Pair : DiplomaticRelations)
	{
		if (Pair.Value.InvolvesPlayer(PlayerId) && 
			(Pair.Value.RelationType == EMultiplayerRelationType::TradePartner ||
			 Pair.Value.RelationType == EMultiplayerRelationType::Ally) &&
			 Pair.Value.TradeAgreements.Num() > 0)
		{
			Partners.Add(Pair.Value.GetOtherPlayer(PlayerId));
		}
	}
	return Partners;
}

bool UMingRTSMultiplayerRelationship::IsAtWar(int32 PlayerId1, int32 PlayerId2) const
{
	return GetDiplomaticRelation(PlayerId1, PlayerId2) == EMultiplayerRelationType::War;
}

bool UMingRTSMultiplayerRelationship::IsAllied(int32 PlayerId1, int32 PlayerId2) const
{
	return GetDiplomaticRelation(PlayerId1, PlayerId2) == EMultiplayerRelationType::Ally;
}

bool UMingRTSMultiplayerRelationship::HasNonAggressionPact(int32 PlayerId1, int32 PlayerId2) const
{
	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	const FPlayerDiplomaticRelation* Found = DiplomaticRelations.Find(Key);
	return Found && Found->bHasNonAggressionPact && Found->NonAggressionExpiration > 0;
}

void UMingRTSMultiplayerRelationship::DeclareWar(int32 TargetPlayerId, const FString& CasusBelli)
{
	if (TargetPlayerId == LocalPlayerId)
	{
		return;
	}

	ServerDeclareWar(TargetPlayerId, CasusBelli);
	
	RecordDiplomaticEvent(TEXT("WarDeclared"), LocalPlayerId, TargetPlayerId, 
		FString::Printf(TEXT("%s declared war on %s: %s"), 
			*FString::FromInt(LocalPlayerId), *FString::FromInt(TargetPlayerId), *CasusBelli), -50.0f);
}

void UMingRTSMultiplayerRelationship::OfferPeace(int32 TargetPlayerId, const TMap<FString, int32>& Terms)
{
	if (TargetPlayerId == LocalPlayerId)
	{
		return;
	}

	if (!IsAtWar(LocalPlayerId, TargetPlayerId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot offer peace: not at war with player %d"), TargetPlayerId);
		return;
	}

	FDiplomaticProposal Proposal;
	Proposal.FromPlayerId = LocalPlayerId;
	Proposal.ToPlayerId = TargetPlayerId;
	Proposal.Action = EDiplomaticAction::OfferPeace;
	Proposal.Message = TEXT("We wish to end this conflict.");
	Proposal.ResourceDemands = Terms;
	
	SendDiplomaticProposal(Proposal);
}

void UMingRTSMultiplayerRelationship::FormAlliance(int32 TargetPlayerId)
{
	if (TargetPlayerId == LocalPlayerId)
	{
		return;
	}

	if (IsAtWar(LocalPlayerId, TargetPlayerId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot form alliance while at war"));
		return;
	}

	FDiplomaticProposal Proposal;
	Proposal.FromPlayerId = LocalPlayerId;
	Proposal.ToPlayerId = TargetPlayerId;
	Proposal.Action = EDiplomaticAction::FormAlliance;
	Proposal.Message = TEXT("Let us join forces for mutual benefit.");
	
	SendDiplomaticProposal(Proposal);
}

void UMingRTSMultiplayerRelationship::BreakAlliance(int32 TargetPlayerId)
{
	if (TargetPlayerId == LocalPlayerId)
	{
		return;
	}

	if (!IsAllied(LocalPlayerId, TargetPlayerId))
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot break alliance: not allied with player %d"), TargetPlayerId);
		return;
	}

	ServerBreakAlliance(TargetPlayerId);
	
	ModifyTrustLevel(LocalPlayerId, TargetPlayerId, -30.0f);
	RecordDiplomaticEvent(TEXT("AllianceBroken"), LocalPlayerId, TargetPlayerId, 
		TEXT("Alliance has been broken"), -20.0f);
}

void UMingRTSMultiplayerRelationship::ProposeNonAggressionPact(int32 TargetPlayerId, float Duration)
{
	if (TargetPlayerId == LocalPlayerId || IsAtWar(LocalPlayerId, TargetPlayerId))
	{
		return;
	}

	FDiplomaticProposal Proposal;
	Proposal.FromPlayerId = LocalPlayerId;
	Proposal.ToPlayerId = TargetPlayerId;
	Proposal.Action = EDiplomaticAction::OfferPeace;
	Proposal.Message = FString::Printf(TEXT("Let us agree to peace for %.0f seconds."), Duration);
	
	SendDiplomaticProposal(Proposal);
}

void UMingRTSMultiplayerRelationship::ProposeTradeAgreement(int32 TargetPlayerId, const TMap<FString, int32>& TradeTerms)
{
	if (TargetPlayerId == LocalPlayerId || IsAtWar(LocalPlayerId, TargetPlayerId))
	{
		return;
	}

	FDiplomaticProposal Proposal;
	Proposal.FromPlayerId = LocalPlayerId;
	Proposal.ToPlayerId = TargetPlayerId;
	Proposal.Action = EDiplomaticAction::OfferTrade;
	Proposal.Message = TEXT("I propose a trade agreement.");
	Proposal.ResourceDemands = TradeTerms;
	
	SendDiplomaticProposal(Proposal);
}

void UMingRTSMultiplayerRelationship::SendDiplomaticProposal(const FDiplomaticProposal& Proposal)
{
	PendingProposals.Add(Proposal.ProposalId, Proposal);
	ServerSendProposal(Proposal);
	
	UE_LOG(LogTemp, Log, TEXT("Sent diplomatic proposal to player %d"), Proposal.ToPlayerId);
}

void UMingRTSMultiplayerRelationship::AcceptProposal(const FGuid& ProposalId)
{
	ServerRespondToProposal(ProposalId, true, TEXT(""));
}

void UMingRTSMultiplayerRelationship::RejectProposal(const FGuid& ProposalId, const FString& Reason)
{
	ServerRespondToProposal(ProposalId, false, Reason);
}

void UMingRTSMultiplayerRelationship::SetTrustLevel(int32 PlayerId1, int32 PlayerId2, float TrustLevel)
{
	if (!ArePlayersValid(PlayerId1, PlayerId2))
	{
		return;
	}

	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	FPlayerDiplomaticRelation& Relation = DiplomaticRelations.FindOrAdd(Key);
	Relation.PlayerId1 = FMath::Min(PlayerId1, PlayerId2);
	Relation.PlayerId2 = FMath::Max(PlayerId1, PlayerId2);
	Relation.TrustLevel = FMath::Clamp(TrustLevel, -100.0f, 100.0f);
}

float UMingRTSMultiplayerRelationship::GetTrustLevel(int32 PlayerId1, int32 PlayerId2) const
{
	FGuid Key = GetRelationKey(PlayerId1, PlayerId2);
	const FPlayerDiplomaticRelation* Found = DiplomaticRelations.Find(Key);
	return Found ? Found->TrustLevel : 0.0f;
}

void UMingRTSMultiplayerRelationship::ModifyTrustLevel(int32 PlayerId1, int32 PlayerId2, float Delta)
{
	float CurrentLevel = GetTrustLevel(PlayerId1, PlayerId2);
	SetTrustLevel(PlayerId1, PlayerId2, CurrentLevel + Delta);
}

void UMingRTSMultiplayerRelationship::RecordDiplomaticEvent(const FString& EventType, int32 PlayerId1, 
	int32 PlayerId2, const FString& Description, float Impact)
{
	FDiplomaticEvent Event;
	Event.EventType = EventType;
	Event.InvolvedPlayerId1 = PlayerId1;
	Event.InvolvedPlayerId2 = PlayerId2;
	Event.Description = Description;
	Event.Impact = Impact;
	Event.Timestamp = FDateTime::Now();
	
	DiplomaticHistory.Add(Event);
	
	// Trim history if too large
	if (DiplomaticHistory.Num() > 1000)
	{
		DiplomaticHistory.RemoveAt(0, DiplomaticHistory.Num() - 1000);
	}
}

TArray<FDiplomaticEvent> UMingRTSMultiplayerRelationship::GetDiplomaticHistory(int32 PlayerId, int32 Limit) const
{
	TArray<FDiplomaticEvent> Result;
	int32 Count = 0;
	
	// Iterate backwards to get most recent first
	for (int32 i = DiplomaticHistory.Num() - 1; i >= 0 && Count < Limit; --i)
	{
		const FDiplomaticEvent& Event = DiplomaticHistory[i];
		if (Event.InvolvedPlayerId1 == PlayerId || Event.InvolvedPlayerId2 == PlayerId)
		{
			Result.Add(Event);
			Count++;
		}
	}
	
	return Result;
}

void UMingRTSMultiplayerRelationship::ClearAllRelations()
{
	DiplomaticRelations.Empty();
	PendingProposals.Empty();
	DiplomaticHistory.Empty();
}

FString UMingRTSMultiplayerRelationship::GetDiplomaticSummary(int32 PlayerId) const
{
	FString Summary;
	Summary += FString::Printf(TEXT("Diplomatic Summary for Player %d\n"), PlayerId);
	Summary += TEXT("================================\n");
	
	TArray<FPlayerDiplomaticRelation> Relations = GetAllRelationsForPlayer(PlayerId);
	
	Summary += FString::Printf(TEXT("Total Relations: %d\n"), Relations.Num());
	
	int32 Allies = 0, Enemies = 0, TradePartners = 0, AtWar = 0;
	for (const auto& Relation : Relations)
	{
		switch (Relation.RelationType)
		{
		case EMultiplayerRelationType::Ally: Allies++; break;
		case EMultiplayerRelationType::Enemy: Enemies++; break;
		case EMultiplayerRelationType::TradePartner: TradePartners++; break;
		case EMultiplayerRelationType::War: AtWar++; break;
		default: break;
		}
	}
	
	Summary += FString::Printf(TEXT("Allies: %d\n"), Allies);
	Summary += FString::Printf(TEXT("Enemies: %d\n"), Enemies);
	Summary += FString::Printf(TEXT("Trade Partners: %d\n"), TradePartners);
	Summary += FString::Printf(TEXT("At War: %d\n"), AtWar);
	
	Summary += TEXT("\nDetailed Relations:\n");
	for (const auto& Relation : Relations)
	{
		int32 OtherPlayer = Relation.GetOtherPlayer(PlayerId);
		Summary += FString::Printf(TEXT("- Player %d: %s (Trust: %.1f, Strength: %.1f)\n"),
			OtherPlayer,
			*UEnum::GetValueAsString(Relation.RelationType),
			Relation.TrustLevel,
			Relation.RelationStrength);
	}
	
	return Summary;
}

void UMingRTSMultiplayerRelationship::Tick(float DeltaTime)
{
	TimeSinceLastUpdate += DeltaTime;
	
	if (TimeSinceLastUpdate >= RelationUpdateInterval)
	{
		ProcessPendingProposals(TimeSinceLastUpdate);
		ExpireOldProposals();
		UpdateRelationStrengths(TimeSinceLastUpdate);
		TimeSinceLastUpdate = 0.0f;
	}
}

FGuid UMingRTSMultiplayerRelationship::GetRelationKey(int32 PlayerId1, int32 PlayerId2) const
{
	// Create a consistent key regardless of order
	int32 MinId = FMath::Min(PlayerId1, PlayerId2);
	int32 MaxId = FMath::Max(PlayerId1, PlayerId2);
	FString KeyString = FString::Printf(TEXT("%d_%d"), MinId, MaxId);
	return FGuid::NewGuidFromString(*KeyString);
}

bool UMingRTSMultiplayerRelationship::ArePlayersValid(int32 PlayerId1, int32 PlayerId2) const
{
	return PlayerId1 > 0 && PlayerId2 > 0 && PlayerId1 != PlayerId2;
}

void UMingRTSMultiplayerRelationship::BroadcastRelationChange(int32 PlayerId1, int32 PlayerId2)
{
	OnRelationChanged.Broadcast(PlayerId1, PlayerId2);
}

void UMingRTSMultiplayerRelationship::ProcessPendingProposals(float DeltaTime)
{
	TArray<FGuid> ExpiredProposals;
	
	for (auto& Pair : PendingProposals)
	{
		FDiplomaticProposal& Proposal = Pair.Value;
		float ElapsedTime = (FDateTime::Now() - Proposal.ProposedTime).GetTotalSeconds();
		
		if (ElapsedTime > Proposal.ExpirationTime)
		{
			ExpiredProposals.Add(Proposal.ProposalId);
		}
	}
	
	for (const FGuid& Id : ExpiredProposals)
	{
		PendingProposals.Remove(Id);
	}
}

void UMingRTSMultiplayerRelationship::ExpireOldProposals()
{
	// Already handled in ProcessPendingProposals
}

void UMingRTSMultiplayerRelationship::UpdateRelationStrengths(float DeltaTime)
{
	// Gradually decay relation strength if not interacted with
	for (auto& Pair : DiplomaticRelations)
	{
		FPlayerDiplomaticRelation& Relation = Pair.Value;
		float TimeSinceInteraction = (FDateTime::Now() - Relation.LastInteraction).GetTotalSeconds();
		
		if (TimeSinceInteraction > 300.0f) // 5 minutes
		{
			// Decay relation strength slowly
			Relation.RelationStrength = FMath::Max(0.0f, Relation.RelationStrength - (DeltaTime * 0.1f));
		}
	}
}

void UMingRTSMultiplayerRelationship::SyncRelationToNetwork(const FPlayerDiplomaticRelation& Relation)
{
	if (NetworkManager)
	{
		FNetworkSyncedData Data;
		Data.DataId = TEXT("RelationUpdate");
		Data.DataType = TEXT("DiplomaticRelation");
		// Serialize relation data
		MulticastRelationChanged(Relation);
	}
}

void UMingRTSMultiplayerRelationship::SyncProposalToNetwork(const FDiplomaticProposal& Proposal)
{
	if (NetworkManager && Proposal.FromPlayerId == LocalPlayerId)
	{
		FNetworkSyncedData Data;
		Data.DataId = Proposal.ProposalId.ToString();
		Data.DataType = TEXT("DiplomaticProposal");
		// Serialize proposal data
	}
}

bool UMingRTSMultiplayerRelationship::ServerDeclareWar_Validate(int32 TargetPlayerId, const FString& CasusBelli)
{
	return TargetPlayerId != LocalPlayerId && !CasusBelli.IsEmpty();
}

void UMingRTSMultiplayerRelationship::ServerDeclareWar_Implementation(int32 TargetPlayerId, const FString& CasusBelli)
{
	SetDiplomaticRelation(LocalPlayerId, TargetPlayerId, EMultiplayerRelationType::War);
	MulticastWarDeclared(LocalPlayerId, TargetPlayerId, CasusBelli);
	
	FDiplomaticEvent Event;
	Event.EventType = TEXT("WarDeclared");
	Event.InvolvedPlayerId1 = LocalPlayerId;
	Event.InvolvedPlayerId2 = TargetPlayerId;
	Event.Description = CasusBelli;
	Event.Impact = -50.0f;
	OnWarDeclared.Broadcast(Event);
}

bool UMingRTSMultiplayerRelationship::ServerOfferPeace_Validate(int32 TargetPlayerId, const TMap<FString, int32>& Terms)
{
	return TargetPlayerId != LocalPlayerId;
}

void UMingRTSMultiplayerRelationship::ServerOfferPeace_Implementation(int32 TargetPlayerId, const TMap<FString, int32>& Terms)
{
	// Set to neutral relation
	SetDiplomaticRelation(LocalPlayerId, TargetPlayerId, EMultiplayerRelationType::Neutral);
	MulticastPeaceMade(LocalPlayerId, TargetPlayerId, Terms);
	
	FDiplomaticEvent Event;
	Event.EventType = TEXT("PeaceMade");
	Event.InvolvedPlayerId1 = LocalPlayerId;
	Event.InvolvedPlayerId2 = TargetPlayerId;
	Event.Description = TEXT("Peace agreement signed");
	Event.Impact = 20.0f;
	OnPeaceMade.Broadcast(Event);
}

bool UMingRTSMultiplayerRelationship::ServerFormAlliance_Validate(int32 TargetPlayerId)
{
	return TargetPlayerId != LocalPlayerId;
}

void UMingRTSMultiplayerRelationship::ServerFormAlliance_Implementation(int32 TargetPlayerId)
{
	SetDiplomaticRelation(LocalPlayerId, TargetPlayerId, EMultiplayerRelationType::Ally);
	ModifyTrustLevel(LocalPlayerId, TargetPlayerId, 20.0f);
}

bool UMingRTSMultiplayerRelationship::ServerBreakAlliance_Validate(int32 TargetPlayerId)
{
	return TargetPlayerId != LocalPlayerId;
}

void UMingRTSMultiplayerRelationship::ServerBreakAlliance_Implementation(int32 TargetPlayerId)
{
	SetDiplomaticRelation(LocalPlayerId, TargetPlayerId, EMultiplayerRelationType::Neutral);
}

bool UMingRTSMultiplayerRelationship::ServerSendProposal_Validate(const FDiplomaticProposal& Proposal)
{
	return Proposal.FromPlayerId == LocalPlayerId && Proposal.ToPlayerId != LocalPlayerId;
}

void UMingRTSMultiplayerRelationship::ServerSendProposal_Implementation(const FDiplomaticProposal& Proposal)
{
	// Store proposal and notify target player
	PendingProposals.Add(Proposal.ProposalId, Proposal);
	ClientReceiveProposal(Proposal);
}

bool UMingRTSMultiplayerRelationship::ServerRespondToProposal_Validate(const FGuid& ProposalId, bool bAccept, const FString& Reason)
{
	return !ProposalId.IsValid() || true;
}

void UMingRTSMultiplayerRelationship::ServerRespondToProposal_Implementation(const FGuid& ProposalId, bool bAccept, const FString& Reason)
{
	FDiplomaticProposal* Proposal = PendingProposals.Find(ProposalId);
	if (Proposal)
	{
		// Process acceptance or rejection
		if (bAccept)
		{
			// Execute the proposed action
			switch (Proposal->Action)
			{
			case EDiplomaticAction::FormAlliance:
				SetDiplomaticRelation(Proposal->FromPlayerId, Proposal->ToPlayerId, EMultiplayerRelationType::Ally);
				break;
			case EDiplomaticAction::OfferPeace:
				SetDiplomaticRelation(Proposal->FromPlayerId, Proposal->ToPlayerId, EMultiplayerRelationType::Neutral);
				break;
			default:
				break;
			}
		}
		
		// Notify proposer of response
		ClientProposalResponse(ProposalId, bAccept);
		OnDiplomaticProposalAccepted.Broadcast(*Proposal, bAccept);
		
		PendingProposals.Remove(ProposalId);
	}
}

void UMingRTSMultiplayerRelationship::ClientReceiveProposal_Implementation(const FDiplomaticProposal& Proposal)
{
	PendingProposals.Add(Proposal.ProposalId, Proposal);
	OnDiplomaticProposalReceived.Broadcast(Proposal);
	
	UE_LOG(LogTemp, Log, TEXT("Received diplomatic proposal from player %d"), Proposal.FromPlayerId);
}

void UMingRTSMultiplayerRelationship::ClientProposalResponse_Implementation(const FGuid& ProposalId, bool bAccepted)
{
	PendingProposals.Remove(ProposalId);
	UE_LOG(LogTemp, Log, TEXT("Proposal %s was %s"), *ProposalId.ToString(), 
		bAccepted ? TEXT("accepted") : TEXT("rejected"));
}

void UMingRTSMultiplayerRelationship::ClientSyncRelation_Implementation(const FPlayerDiplomaticRelation& Relation)
{
	FGuid Key = GetRelationKey(Relation.PlayerId1, Relation.PlayerId2);
	DiplomaticRelations.Add(Key, Relation);
}

void UMingRTSMultiplayerRelationship::MulticastRelationChanged_Implementation(const FPlayerDiplomaticRelation& Relation)
{
	FGuid Key = GetRelationKey(Relation.PlayerId1, Relation.PlayerId2);
	DiplomaticRelations.Add(Key, Relation);
	BroadcastRelationChange(Relation.PlayerId1, Relation.PlayerId2);
}

void UMingRTSMultiplayerRelationship::MulticastWarDeclared_Implementation(int32 AggressorId, int32 DefenderId, const FString& CasusBelli)
{
	SetDiplomaticRelation(AggressorId, DefenderId, EMultiplayerRelationType::War);
	
	FDiplomaticEvent Event;
	Event.EventType = TEXT("WarDeclared");
	Event.InvolvedPlayerId1 = AggressorId;
	Event.InvolvedPlayerId2 = DefenderId;
	Event.Description = CasusBelli;
	Event.Impact = -50.0f;
	OnWarDeclared.Broadcast(Event);
}

void UMingRTSMultiplayerRelationship::MulticastPeaceMade_Implementation(int32 PlayerId1, int32 PlayerId2, const TMap<FString, int32>& Terms)
{
	SetDiplomaticRelation(PlayerId1, PlayerId2, EMultiplayerRelationType::Neutral);
	
	FDiplomaticEvent Event;
	Event.EventType = TEXT("PeaceMade");
	Event.InvolvedPlayerId1 = PlayerId1;
	Event.InvolvedPlayerId2 = PlayerId2;
	Event.Description = TEXT("Peace agreement signed");
	Event.Impact = 20.0f;
	OnPeaceMade.Broadcast(Event);
}

void UMingRTSMultiplayerRelationship::OnNetworkPlayerJoined(const FNetworkPlayerInfo& PlayerInfo)
{
	// Initialize neutral relation with new player
	if (PlayerInfo.PlayerId != LocalPlayerId)
	{
		SetDiplomaticRelation(LocalPlayerId, PlayerInfo.PlayerId, EMultiplayerRelationType::Neutral);
		UE_LOG(LogTemp, Log, TEXT("Initialized neutral relation with new player %d"), PlayerInfo.PlayerId);
	}
}

void UMingRTSMultiplayerRelationship::OnNetworkPlayerLeft(const FNetworkPlayerInfo& PlayerInfo)
{
	// Clean up relations with departed player
	FGuid Key = GetRelationKey(LocalPlayerId, PlayerInfo.PlayerId);
	DiplomaticRelations.Remove(Key);
	
	UE_LOG(LogTemp, Log, TEXT("Removed relations for departed player %d"), PlayerInfo.PlayerId);
}
