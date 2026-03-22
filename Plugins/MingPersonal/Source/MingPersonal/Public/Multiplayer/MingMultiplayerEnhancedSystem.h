// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Multiplayer Enhanced System - Advanced multiplayer relationship and cooperation features

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "MingRelationshipManager.h"
#include "MingMultiplayerEnhancedSystem.generated.h"

// Multiplayer relationship types
UENUM(BlueprintType)
enum class EMultiplayerRelationshipType : uint8
{
    Ally               UMETA(DisplayName = "Ally"),
    Enemy              UMETA(DisplayName = "Enemy"),
    Neutral            UMETA(DisplayName = "Neutral"),
    Vassal             UMETA(DisplayName = "Vassal"),
    Overlord           UMETA(DisplayName = "Overlord"),
    TradePartner       UMETA(DisplayName = "Trade Partner"),
    MilitaryPact       UMETA(DisplayName = "Military Pact"),
    Rival              UMETA(DisplayName = "Rival"),
    Unknown            UMETA(DisplayName = "Unknown")
};

// Multiplayer diplomatic actions
UENUM(BlueprintType)
enum class EDiplomaticAction : uint8
{
    DeclareWar         UMETA(DisplayName = "Declare War"),
    MakePeace          UMETA(DisplayName = "Make Peace"),
    FormAlliance       UMETA(DisplayName = "Form Alliance"),
    BreakAlliance      UMETA(DisplayName = "Break Alliance"),
    OfferTrade         UMETA(DisplayName = "Offer Trade"),
    SendGift           UMETA(DisplayName = "Send Gift"),
    DemandTribute      UMETA(DisplayName = "Demand Tribute"),
    ProposeMarriage    UMETA(DisplayName = "Propose Marriage"),
    RequestMilitaryAid UMETA(DisplayName = "Request Military Aid")
};

// Multiplayer cooperative mode
UENUM(BlueprintType)
enum class ECooperativeMode : uint8
{
    FreeForAll         UMETA(DisplayName = "Free For All"),
    Teams              UMETA(DisplayName = "Teams"),
    CoopVsAI           UMETA(DisplayName = "Co-op vs AI"),
    Campaign           UMETA(DisplayName = "Campaign Co-op"),
    Sandbox            UMETA(DisplayName = "Sandbox")
};

// Player multiplayer data
USTRUCT(BlueprintType)
struct FPlayerMultiplayerData
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString PlayerId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString PlayerName = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    int32 TeamId = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 FactionId = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bIsReady = false;

    UPROPERTY(BlueprintReadOnly)
    bool bIsHost = false;

    UPROPERTY(BlueprintReadOnly)
    float Latency = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, EMultiplayerRelationshipType> Relationships;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, float> RelationshipValues;
};

// Diplomatic proposal
USTRUCT(BlueprintType)
struct FDiplomaticProposal
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ProposalId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString FromPlayerId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString ToPlayerId = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    EDiplomaticAction Action = EDiplomaticAction::OfferTrade;

    UPROPERTY(BlueprintReadOnly)
    TMap<FString, FString> Terms;

    UPROPERTY(BlueprintReadOnly)
    float TimeoutSeconds = 60.0f;

    UPROPERTY(BlueprintReadOnly)
    bool bIsActive = true;
};

// Multiplayer session configuration
USTRUCT(BlueprintType)
struct FMultiplayerSessionConfig
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    ECooperativeMode Mode = ECooperativeMode::FreeForAll;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxPlayers = 8;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Teams = 2;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllowDiplomacy = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bSharedVision = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bTeamResourceSharing = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bAllowAlliances = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bRankedMatch = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 VictoryCondition = 0; // 0=Conquest, 1=Economic, 2=Wonder
};

// Multiplayer relationship network entry
USTRUCT(BlueprintType)
struct FRelationshipNetworkEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString SourcePlayer = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    FString TargetPlayer = TEXT("");

    UPROPERTY(BlueprintReadOnly)
    EMultiplayerRelationshipType Relationship = EMultiplayerRelationshipType::Neutral;

    UPROPERTY(BlueprintReadOnly)
    float RelationshipValue = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    float TrustLevel = 50.0f;

    UPROPERTY(BlueprintReadOnly)
    float CooperationHistory = 0.0f;

    UPROPERTY(BlueprintReadOnly)
    int32 ConflictsCount = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 TradeAgreements = 0;
};

// Multiplayer Enhanced System - Advanced multiplayer features
UCLASS()
class MINGPERSONAL_API UMingMultiplayerEnhancedSystem : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    UMingMultiplayerEnhancedSystem();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    // Session management
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Session")
    void InitializeMultiplayerSession(const FMultiplayerSessionConfig& Config);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Session")
    void EndMultiplayerSession();

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Session")
    bool IsInMultiplayerSession() const { return bSessionActive; }

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Session")
    FMultiplayerSessionConfig GetSessionConfig() const { return SessionConfig; }

    // Player management
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Players")
    void RegisterPlayer(const FString& PlayerId, const FString& PlayerName, bool bIsHost);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Players")
    void UnregisterPlayer(const FString& PlayerId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Players")
    void UpdatePlayerStatus(const FString& PlayerId, bool bIsReady);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Players")
    void AssignTeam(const FString& PlayerId, int32 TeamId);

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Players")
    TArray<FPlayerMultiplayerData> GetAllPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Players")
    FPlayerMultiplayerData GetPlayerData(const FString& PlayerId) const;

    // Relationship network
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Relationships")
    void SetPlayerRelationship(const FString& SourcePlayerId, const FString& TargetPlayerId, EMultiplayerRelationshipType Relationship);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Relationships")
    void ModifyRelationshipValue(const FString& SourcePlayerId, const FString& TargetPlayerId, float Delta);

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Relationships")
    EMultiplayerRelationshipType GetPlayerRelationship(const FString& SourcePlayerId, const FString& TargetPlayerId) const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Relationships")
    float GetRelationshipValue(const FString& SourcePlayerId, const FString& TargetPlayerId) const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Relationships")
    TArray<FRelationshipNetworkEntry> GetRelationshipNetwork(const FString& PlayerId) const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Relationships")
    TArray<FString> GetAllies(const FString& PlayerId) const;

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Relationships")
    TArray<FString> GetEnemies(const FString& PlayerId) const;

    // Diplomacy
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    FString SendDiplomaticProposal(const FString& FromPlayerId, const FString& ToPlayerId, EDiplomaticAction Action, const TMap<FString, FString>& Terms);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void AcceptProposal(const FString& ProposalId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void RejectProposal(const FString& ProposalId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void CancelProposal(const FString& ProposalId);

    UFUNCTION(BlueprintPure, Category = "Multiplayer|Diplomacy")
    TArray<FDiplomaticProposal> GetPendingProposals(const FString& PlayerId) const;

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void BreakAlliance(const FString& PlayerId, const FString& AllyId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void DeclareWar(const FString& PlayerId, const FString& TargetId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Diplomacy")
    void NegotiatePeace(const FString& PlayerId, const FString& EnemyId, const TMap<FString, FString>& Terms);

    // Cooperative features
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Cooperation")
    void ShareVision(const FString& SourcePlayerId, const FString& TargetPlayerId);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Cooperation")
    void ShareResources(const FString& SourcePlayerId, const FString& TargetPlayerId, const FString& ResourceType, float Amount);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Cooperation")
    void SendMilitaryAid(const FString& SourcePlayerId, const FString& TargetPlayerId, int32 UnitCount);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Cooperation")
    void CoordinateAttack(const FString& CoordinatorId, const TArray<FString>& ParticipantIds, const FVector& TargetLocation);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Cooperation")
    void FormJointOperation(const FString& OperationName, const TArray<FString>& MemberIds);

    // Network synchronization
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Network")
    void SyncRelationshipData();

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|Network")
    void BroadcastDiplomaticEvent(const FString& EventType, const TMap<FString, FString>& EventData);

    // AI coordination in multiplayer
    UFUNCTION(BlueprintCallable, Category = "Multiplayer|AI")
    void AssignAIPlayer(const FString& PlayerId, int32 AIDifficulty);

    UFUNCTION(BlueprintCallable, Category = "Multiplayer|AI")
    void ConfigureAIBehavior(const FString& PlayerId, const FString& BehaviorProfile);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnPlayerJoined OnPlayerJoined;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnPlayerLeft OnPlayerLeft;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnRelationshipChanged OnRelationshipChanged;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnDiplomaticProposalReceived OnDiplomaticProposalReceived;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnWarDeclared OnWarDeclared;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnPeaceMade OnPeaceMade;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnAllianceFormed OnAllianceFormed;

    UPROPERTY(BlueprintAssignable, Category = "Multiplayer|Events")
    FOnCooperativeAction OnCooperativeAction;

protected:
    // Session state
    UPROPERTY()
    bool bSessionActive = false;

    UPROPERTY()
    FMultiplayerSessionConfig SessionConfig;

    UPROPERTY()
    TMap<FString, FPlayerMultiplayerData> PlayerRegistry;

    UPROPERTY()
    TMap<FString, FDiplomaticProposal> ActiveProposals;

    UPROPERTY()
    TMap<FString, FRelationshipNetworkEntry> RelationshipNetwork;

    UPROPERTY()
    TArray<FString> ActiveOperations;

    // Internal methods
    void UpdateProposals(float DeltaTime);
    void ExpireOldProposals();
    void NotifyPlayersOfChange(const FString& PlayerId, const FString& ChangeType);
    void SyncToAllPlayers(const FString& Data);
    void UpdateRelationshipTrust(const FString& PlayerId1, const FString& PlayerId2, float Delta);
    void RecordCooperation(const FString& PlayerId1, const FString& PlayerId2, float Value);
    void RecordConflict(const FString& PlayerId1, const FString& PlayerId2);
    void AutoAssignTeams();
    void BalanceTeams();
    bool AreAllies(const FString& Player1, const FString& Player2) const;
    bool AreEnemies(const FString& Player1, const FString& Player2) const;
    EMultiplayerRelationshipType EvaluateRelationship(float Value) const;
    void PropagateAlliance(const FString& Ally1, const FString& Ally2, const FString& NewAlly);
    void HandleBetrayal(const FString& Betrayer, const FString& Victim);
    void CalculateDiplomaticPower(const FString& PlayerId);
    void GenerateRelationshipHistoryReport(const FString& PlayerId);
};

// Event delegates
declare dynamic multicast delegate(FOnPlayerJoined, const FString&, PlayerId, const FString&, PlayerName);
declare dynamic multicast delegate(FOnPlayerLeft, const FString&, PlayerId);
declare dynamic multicast delegate(FOnRelationshipChanged, const FString&, SourcePlayer, const FString&, TargetPlayer, EMultiplayerRelationshipType, NewRelationship, float, Value);
declare dynamic multicast delegate(FOnDiplomaticProposalReceived, const FDiplomaticProposal&, Proposal);
declare dynamic multicast delegate(FOnWarDeclared, const FString&, Declarer, const FString&, Target);
declare dynamic multicast delegate(FOnPeaceMade, const FString&, Player1, const FString&, Player2);
declare dynamic multicast delegate(FOnAllianceFormed, const FString&, Player1, const FString&, Player2);
declare dynamic multicast delegate(FOnCooperativeAction, const FString&, ActionType, const TArray<FString>&, Participants);
