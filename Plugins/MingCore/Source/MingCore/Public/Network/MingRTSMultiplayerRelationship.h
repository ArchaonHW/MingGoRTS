// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSMultiplayerRelationship.generated.h"

UENUM(BlueprintType)
enum class EMultiplayerRelationType : uint8
{
	Neutral					UMETA(DisplayName = "中立"),
	Ally					UMETA(DisplayName = "盟友"),
	Enemy					UMETA(DisplayName = "敵人"),
	TradePartner			UMETA(DisplayName = "貿易夥伴"),
	Vassal					UMETA(DisplayName = "附庸"),
	Overlord				UMETA(DisplayName = "宗主"),
	NonAggression			UMETA(DisplayName = "互不侵犯"),
	War						UMETA(DisplayName = "戰爭")
};

UENUM(BlueprintType)
enum class EDiplomaticAction : uint8
{
	DeclareWar				UMETA(DisplayName = "宣戰"),
	OfferPeace				UMETA(DisplayName = "求和"),
	FormAlliance			UMETA(DisplayName = "結盟"),
	BreakAlliance			UMETA(DisplayName = "撕毀盟約"),
	OfferTrade				UMETA(DisplayName = "提出貿易"),
	CancelTrade				UMETA(DisplayName = "取消貿易"),
	DemandTribute			UMETA(DisplayName = "要求進貢"),
	SendGift				UMETA(DisplayName = "贈送禮物"),
	ProposeMarriage			UMETA(DisplayName = "聯姻提議"),
	OfferVassalization		UMETA(DisplayName = "提出附庸")
};

USTRUCT(BlueprintType)
struct FDiplomaticProposal
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FGuid ProposalId;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 FromPlayerId;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 ToPlayerId;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	EDiplomaticAction Action;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FString Message;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	TMap<FString, int32> ResourceDemands;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FDateTime ProposedTime;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	float ExpirationTime;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	bool bRequiresAcceptance;

	FDiplomaticProposal()
		: ProposalId(FGuid::NewGuid())
		, FromPlayerId(0)
		, ToPlayerId(0)
		, Action(EDiplomaticAction::SendGift)
		, ExpirationTime(300.0f)
		, bRequiresAcceptance(true)
		, ProposedTime(FDateTime::Now())
	{
	}
};

USTRUCT(BlueprintType)
struct FPlayerDiplomaticRelation
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 PlayerId1;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 PlayerId2;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	EMultiplayerRelationType RelationType;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	float TrustLevel;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	float RelationStrength;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	TMap<FString, float> TradeAgreements;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FDateTime LastInteraction;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 AllianceDuration;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	bool bHasNonAggressionPact;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	float NonAggressionExpiration;

	FPlayerDiplomaticRelation()
		: PlayerId1(0)
		, PlayerId2(0)
		, RelationType(EMultiplayerRelationType::Neutral)
		, TrustLevel(0.0f)
		, RelationStrength(50.0f)
		, AllianceDuration(0)
		, bHasNonAggressionPact(false)
		, NonAggressionExpiration(0.0f)
		, LastInteraction(FDateTime::Now())
	{
	}

	bool InvolvesPlayer(int32 PlayerId) const
	{
		return PlayerId1 == PlayerId || PlayerId2 == PlayerId;
	}

	int32 GetOtherPlayer(int32 PlayerId) const
	{
		return (PlayerId == PlayerId1) ? PlayerId2 : PlayerId1;
	}
};

USTRUCT(BlueprintType)
struct FDiplomaticEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FGuid EventId;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FString EventType;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 InvolvedPlayerId1;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	int32 InvolvedPlayerId2;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FString Description;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	FDateTime Timestamp;

	UPROPERTY(BlueprintReadOnly, Category = "Diplomacy")
	float Impact;

	FDiplomaticEvent()
		: EventId(FGuid::NewGuid())
		, InvolvedPlayerId1(0)
		, InvolvedPlayerId2(0)
		, Impact(0.0f)
		, Timestamp(FDateTime::Now())
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRelationChanged, int32, PlayerId1, int32, PlayerId2);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDiplomaticProposalReceived, const FDiplomaticProposal&, Proposal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDiplomaticProposalAccepted, const FDiplomaticProposal&, Proposal, bool, bAccepted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWarDeclared, const FDiplomaticEvent&, Event);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPeaceMade, const FDiplomaticEvent&, Event);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingRTSMultiplayerRelationship : public UObject
{
	GENERATED_BODY()

public:
	UMingRTSMultiplayerRelationship();

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	bool InitializeSystem(UMingRTSNetworkManager* InNetworkManager);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void ShutdownSystem();

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void SetLocalPlayerId(int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	int32 GetLocalPlayerId() const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void SetDiplomaticRelation(int32 PlayerId1, int32 PlayerId2, EMultiplayerRelationType RelationType);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	EMultiplayerRelationType GetDiplomaticRelation(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	FPlayerDiplomaticRelation GetRelationDetails(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<FPlayerDiplomaticRelation> GetAllRelationsForPlayer(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<FPlayerDiplomaticRelation> GetAllRelations() const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<int32> GetAllies(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<int32> GetEnemies(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<int32> GetTradePartners(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	bool IsAtWar(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	bool IsAllied(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	bool HasNonAggressionPact(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void DeclareWar(int32 TargetPlayerId, const FString& CasusBelli);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void OfferPeace(int32 TargetPlayerId, const TMap<FString, int32>& Terms);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void FormAlliance(int32 TargetPlayerId);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void BreakAlliance(int32 TargetPlayerId);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void ProposeNonAggressionPact(int32 TargetPlayerId, float Duration);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void ProposeTradeAgreement(int32 TargetPlayerId, const TMap<FString, int32>& TradeTerms);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void SendDiplomaticProposal(const FDiplomaticProposal& Proposal);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void AcceptProposal(const FGuid& ProposalId);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void RejectProposal(const FGuid& ProposalId, const FString& Reason);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void SetTrustLevel(int32 PlayerId1, int32 PlayerId2, float TrustLevel);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	float GetTrustLevel(int32 PlayerId1, int32 PlayerId2) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void ModifyTrustLevel(int32 PlayerId1, int32 PlayerId2, float Delta);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void RecordDiplomaticEvent(const FString& EventType, int32 PlayerId1, int32 PlayerId2, 
		const FString& Description, float Impact);

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	TArray<FDiplomaticEvent> GetDiplomaticHistory(int32 PlayerId, int32 Limit = 50) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void ClearAllRelations();

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	FString GetDiplomaticSummary(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "MultiplayerDiplomacy")
	void Tick(float DeltaTime);

public:
	UPROPERTY(BlueprintAssignable, Category = "MultiplayerDiplomacy|Events")
	FOnRelationChanged OnRelationChanged;

	UPROPERTY(BlueprintAssignable, Category = "MultiplayerDiplomacy|Events")
	FOnDiplomaticProposalReceived OnDiplomaticProposalReceived;

	UPROPERTY(BlueprintAssignable, Category = "MultiplayerDiplomacy|Events")
	FOnDiplomaticProposalAccepted OnDiplomaticProposalAccepted;

	UPROPERTY(BlueprintAssignable, Category = "MultiplayerDiplomacy|Events")
	FOnWarDeclared OnWarDeclared;

	UPROPERTY(BlueprintAssignable, Category = "MultiplayerDiplomacy|Events")
	FOnPeaceMade OnPeaceMade;

protected:
	UPROPERTY()
	int32 LocalPlayerId;

	UPROPERTY()
	TMap<FGuid, FPlayerDiplomaticRelation> DiplomaticRelations;

	UPROPERTY()
	TMap<FGuid, FDiplomaticProposal> PendingProposals;

	UPROPERTY()
	TArray<FDiplomaticEvent> DiplomaticHistory;

	UPROPERTY()
	UMingRTSNetworkManager* NetworkManager;

	UPROPERTY()
	float RelationUpdateInterval;

	UPROPERTY()
	float TimeSinceLastUpdate;

protected:
	FGuid GetRelationKey(int32 PlayerId1, int32 PlayerId2) const;
	bool ArePlayersValid(int32 PlayerId1, int32 PlayerId2) const;
	void BroadcastRelationChange(int32 PlayerId1, int32 PlayerId2);
	void ProcessPendingProposals(float DeltaTime);
	void ExpireOldProposals();
	void UpdateRelationStrengths(float DeltaTime);
	void SyncRelationToNetwork(const FPlayerDiplomaticRelation& Relation);
	void SyncProposalToNetwork(const FDiplomaticProposal& Proposal);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerDeclareWar(int32 TargetPlayerId, const FString& CasusBelli);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerOfferPeace(int32 TargetPlayerId, const TMap<FString, int32>& Terms);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFormAlliance(int32 TargetPlayerId);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerBreakAlliance(int32 TargetPlayerId);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSendProposal(const FDiplomaticProposal& Proposal);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRespondToProposal(const FGuid& ProposalId, bool bAccept, const FString& Reason);

	UFUNCTION(Client, Reliable)
	void ClientReceiveProposal(const FDiplomaticProposal& Proposal);

	UFUNCTION(Client, Reliable)
	void ClientProposalResponse(const FGuid& ProposalId, bool bAccepted);

	UFUNCTION(Client, Reliable)
	void ClientSyncRelation(const FPlayerDiplomaticRelation& Relation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRelationChanged(const FPlayerDiplomaticRelation& Relation);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastWarDeclared(int32 AggressorId, int32 DefenderId, const FString& CasusBelli);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPeaceMade(int32 PlayerId1, int32 PlayerId2, const TMap<FString, int32>& Terms);
};
