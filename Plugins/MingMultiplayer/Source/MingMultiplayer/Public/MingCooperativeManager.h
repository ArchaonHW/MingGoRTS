#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingMultiplayerTypes.h"
#include "MingCooperativeManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooperativeSessionStarted, const FString&, SessionID, const FMingCooperativeSessionInfo&, SessionInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooperativeSessionEnded, const FString&, SessionID, const FString&, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnPlayerJoinedCooperative, const FString&, SessionID, int32, PlayerID, const FMingPlayerCooperativeInfo&, PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerLeftCooperative, const FString&, SessionID, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooperativeObjectiveCompleted, const FString&, SessionID, const FMingCooperativeObjective&, Objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCooperativeObjectiveFailed, const FString&, SessionID, const FMingCooperativeObjective&, Objective);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCoopGameStateChanged, const FString&, SessionID, EMingCoopGameState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnCoopScoreUpdated, const FString&, SessionID, int32, PlayerID, int32, NewScore, const FString&, ScoreReason);

UCLASS(ClassGroup = (Multiplayer, Cooperative))
class MINGMULTIPLAYER_API UMingCooperativeManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UMingCooperativeManager();

    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override { return true; }
    virtual TStatId GetStatId() const override;

    // Session Management
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    bool CreateCooperativeSession(const FMingCooperativeSessionInfo& SessionInfo);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    bool JoinCooperativeSession(const FString& SessionID, const FMingPlayerCooperativeInfo& PlayerInfo);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void LeaveCooperativeSession();

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void EndCooperativeSession(const FString& Reason);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    bool IsInCooperativeSession() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    FString GetCurrentSessionID() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    FMingCooperativeSessionInfo GetCurrentSessionInfo() const;

    // Player Management
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void UpdatePlayerRole(int32 PlayerID, EMingCoopRole NewRole);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SetPlayerReady(int32 PlayerID, bool bReady);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    bool AreAllPlayersReady() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    TArray<FMingPlayerCooperativeInfo> GetCooperativePlayers() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    FMingPlayerCooperativeInfo GetPlayerCooperativeInfo(int32 PlayerID) const;

    // Objective Management
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void AddObjective(const FMingCooperativeObjective& Objective);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void CompleteObjective(const FString& ObjectiveID);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void FailObjective(const FString& ObjectiveID, const FString& Reason);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    TArray<FMingCooperativeObjective> GetActiveObjectives() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    TArray<FMingCooperativeObjective> GetCompletedObjectives() const;

    // Resource Sharing
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    bool ShareResource(int32 TargetPlayerID, const FMingResourceShareRequest& ResourceRequest);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void ProcessResourceShare(const FMingResourceShareRequest& ResourceRequest);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SetResourceSharingPolicy(EMingResourceSharingPolicy Policy);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    EMingResourceSharingPolicy GetResourceSharingPolicy() const;

    // Communication
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SendCooperativeMessage(const FString& Message, EMingCoopMessageType MessageType);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SendPlayerPing(int32 TargetPlayerID, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void BroadcastTacticalUpdate(const FMingTacticalUpdate& Update);

    // Game State Management
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SetCoopGameState(EMingCoopGameState NewState);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    EMingCoopGameState GetCoopGameState() const;

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void PauseCooperativeSession();

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void ResumeCooperativeSession();

    // Scoring and Progress
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void UpdatePlayerScore(int32 PlayerID, int32 ScoreChange, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void AwardSharedAchievement(const FString& AchievementID);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    FMingCoopProgress GetCooperativeProgress() const;

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    FMingCoopScore GetCooperativeScores() const;

    // AI Assistance
    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void RequestAIAssistance(EMingAIAssistanceType AssistanceType, const FVector& Location);

    UFUNCTION(BlueprintCallable, Category = "Cooperative")
    void SetAIAssistanceLevel(EMingAIAssistanceLevel Level);

    UFUNCTION(BlueprintPure, Category = "Cooperative")
    EMingAIAssistanceLevel GetAIAssistanceLevel() const;

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCooperativeSessionStarted OnCooperativeSessionStarted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCooperativeSessionEnded OnCooperativeSessionEnded;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerJoinedCooperative OnPlayerJoinedCooperative;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerLeftCooperative OnPlayerLeftCooperative;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCooperativeObjectiveCompleted OnCooperativeObjectiveCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCooperativeObjectiveFailed OnCooperativeObjectiveFailed;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCoopGameStateChanged OnCoopGameStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnCoopScoreUpdated OnCoopScoreUpdated;

protected:
    UPROPERTY()
    FMingCooperativeSessionInfo CurrentSession;

    UPROPERTY()
    TMap<int32, FMingPlayerCooperativeInfo> CooperativePlayers;

    UPROPERTY()
    TArray<FMingCooperativeObjective> ActiveObjectives;

    UPROPERTY()
    TArray<FMingCooperativeObjective> CompletedObjectives;

    UPROPERTY()
    EMingCoopGameState CurrentGameState;

    UPROPERTY()
    EMingResourceSharingPolicy ResourceSharingPolicy;

    UPROPERTY()
    EMingAIAssistanceLevel AIAssistanceLevel;

    UPROPERTY()
    FMingCoopProgress SessionProgress;

    UPROPERTY()
    FMingCoopScore SessionScores;

    UPROPERTY()
    bool bSessionActive;

    UPROPERTY()
    float LastUpdateTime;

    UPROPERTY()
    float UpdateInterval;

    // Internal Functions
    void UpdateCooperativeSession(float DeltaTime);
    void ProcessResourceSharing();
    void UpdateAIAssistance();
    void CheckObjectives();
    void SyncCooperativeState();
    void BroadcastSessionState();
    void HandlePlayerDisconnection(int32 PlayerID);
    void ValidateCooperativeIntegrity();
    void CalculateSessionScores();
    void GenerateSessionReport();

    // Delegates
    UFUNCTION()
    void OnNetworkMessageReceived(const FMingNetworkMessage& Message);

    UFUNCTION()
    void OnPlayerNetworkStateChanged(int32 PlayerID, EMingConnectionState NewState);

private:
    bool ValidateSessionInfo(const FMingCooperativeSessionInfo& SessionInfo) const;
    bool ValidatePlayerInfo(const FMingPlayerCooperativeInfo& PlayerInfo) const;
    FString GenerateSessionID() const;
    void InitializeDefaultObjectives();
    void CleanupSession();
};
