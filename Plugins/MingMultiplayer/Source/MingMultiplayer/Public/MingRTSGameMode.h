#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingMultiplayerTypes.h"
#include "MingRTSGameMode.generated.h"

UENUM(BlueprintType)
enum class EMingServerMode: uint8 {
    ListenServer UMETA(DisplayName = "Listen Server"),
    DedicatedServer UMETA(DisplayName = "Dedicated Server"),
    Offline UMETA(DisplayName = "Offline")
};

USTRUCT(BlueprintType)
struct FMingServerConfiguration
{
    GENERATED_BODY()

    UPROPERTY()
    EMingServerMode ServerMode;

    UPROPERTY()
    int32 MaxPlayers;

    UPROPERTY()
    int32 TickRate;

    UPROPERTY()
    bool bUsePassword;

    UPROPERTY()
    FString ServerPassword;

    UPROPERTY()
    bool bLanOnly;

    UPROPERTY()
    bool bAllowCheats;

    UPROPERTY()
    bool bRequireAuthentication;

    UPROPERTY()
    float IdleKickTime;

    UPROPERTY()
    int32 ReservedSlots;

    FMingServerConfiguration()
        : ServerMode(EMingServerMode::ListenServer)
        , MaxPlayers(8)
        , TickRate(30)
        , bUsePassword(false)
        , bLanOnly(false)
        , bAllowCheats(false)
        , bRequireAuthentication(false)
        , IdleKickTime(300.0f)
        , ReservedSlots(0)
    {}
};

/**
 * MingGoRTS Multiplayer Game Mode
 * Handles server-side multiplayer logic
 */
UCLASS(ClassGroup = (Multiplayer, GameMode))
class MINGMULTIPLAYER_API AMingRTSGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingRTSGameMode();

    virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
    virtual void StartPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    
    virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, 
        FString& ErrorMessage) override;
    virtual void PostLogin(APlayerController* NewPlayer) override;
    virtual void Logout(AController* Exiting) override;

    // Server Configuration
    UFUNCTION(BlueprintCallable, Category = "Server")
    void ConfigureServer(const FMingServerConfiguration& Configuration);

    UFUNCTION(BlueprintPure, Category = "Server")
    FMingServerConfiguration GetServerConfiguration() const { return ServerConfig; }

    UFUNCTION(BlueprintCallable, Category = "Server")
    void SetMaxPlayers(int32 MaxPlayers);

    UFUNCTION(BlueprintCallable, Category = "Server")
    void SetServerPassword(const FString& Password);

    UFUNCTION(BlueprintCallable, Category = "Server")
    void EnableCheats(bool bEnabled);

    // Player Management
    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void KickPlayer(int32 PlayerID, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void BanPlayer(int32 PlayerID, const FString& Reason, int32 DurationMinutes);

    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void UnbanPlayer(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Player Management")
    void TransferHost(int32 NewHostPlayerID);

    UFUNCTION(BlueprintPure, Category = "Player Management")
    TArray<int32> GetConnectedPlayerIDs() const;

    UFUNCTION(BlueprintPure, Category = "Player Management")
    int32 GetPlayerCount() const;

    UFUNCTION(BlueprintPure, Category = "Player Management")
    bool IsPlayerBanned(int32 PlayerID) const;

    // Game State Management
    UFUNCTION(BlueprintCallable, Category = "Game State")
    void PauseGame();

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void ResumeGame();

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void RestartGame();

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void ChangeMap(const FString& MapName);

    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsGamePaused() const { return bGamePaused; }

    // Data Synchronization
    UFUNCTION(BlueprintCallable, Category = "Sync")
    void BroadcastGameState();

    UFUNCTION(BlueprintCallable, Category = "Sync")
    void SyncPlayerData(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Sync")
    void SyncAllPlayersData();

    // Relationship & Reputation Server Logic
    UFUNCTION(BlueprintCallable, Category = "Personal System")
    void ServerUpdateRelationship(int32 PlayerID, FName CharacterID, float Value, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Personal System")
    void ServerUpdateReputation(int32 PlayerID, FName RegionID, float Value, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Personal System")
    void ServerCompleteQuest(int32 PlayerID, const FString& QuestID);

    // RPC Validation
    UFUNCTION(BlueprintCallable, Category = "Validation")
    bool ValidatePlayerAction(int32 PlayerID, int32 ActionType, const TArray<uint8>& ActionData);

    UFUNCTION(BlueprintCallable, Category = "Validation")
    bool ValidateRelationshipChange(int32 PlayerID, FName CharacterID, float NewValue) const;

    UFUNCTION(BlueprintCallable, Category = "Validation")
    bool ValidateReputationChange(int32 PlayerID, FName RegionID, float NewValue) const;

    // Server Statistics
    UFUNCTION(BlueprintPure, Category = "Statistics")
    float GetServerUptime() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    int32 GetTotalBytesTransferred() const;

    UFUNCTION(BlueprintPure, Category = "Statistics")
    float GetAveragePlayerPing() const;

protected:
    UPROPERTY()
    FMingServerConfiguration ServerConfig;

    UPROPERTY()
    TSet<int32> BannedPlayers;

    UPROPERTY()
    TMap<int32, float> PlayerJoinTimes;

    UPROPERTY()
    TMap<int32, float> PlayerLastActiveTimes;

    UPROPERTY()
    bool bGamePaused;

    UPROPERTY()
    float ServerStartTime;

    UPROPERTY()
    int32 NextPlayerID;

    UPROPERTY()
    TArray<int32> ReservedPlayerIDs;

    UPROPERTY()
    TObjectPtr<class UMingNetworkManager> NetworkManager;

    UPROPERTY()
    TObjectPtr<class UMingRelationshipReplication> RelationshipReplication;

    UPROPERTY()
    TObjectPtr<class UMingReputationReplication> ReputationReplication;

    // Internal Functions
    void InitializeMultiplayerSystems();
    void ShutdownMultiplayerSystems();
    
    void CheckIdlePlayers();
    void UpdatePlayerActivity(int32 PlayerID);
    
    int32 AssignPlayerID();
    void ReleasePlayerID(int32 PlayerID);
    
    bool AuthenticatePlayer(int32 PlayerID, const FString& AuthToken);
    void HandlePlayerTimeout(int32 PlayerID);
    
    void OnGamePaused();
    void OnGameResumed();
    void OnPlayerActivity(int32 PlayerID);
    
    void SaveServerState();
    void LoadServerState();

    // Admin Commands
    UFUNCTION(Exec)
    void AdminKick(const FString& PlayerName, const FString& Reason);

    UFUNCTION(Exec)
    void AdminBan(const FString& PlayerName, const FString& Reason, int32 DurationMinutes);

    UFUNCTION(Exec)
    void AdminPause();

    UFUNCTION(Exec)
    void AdminUnpause();

    UFUNCTION(Exec)
    void AdminChangeMap(const FString& MapName);

    UFUNCTION(Exec)
    void AdminSetMaxPlayers(int32 MaxPlayers);

public:
    // Events
    
    
    
    
    

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerJoined OnPlayerJoined;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerLeft OnPlayerLeft;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnPlayerKicked OnPlayerKicked;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGamePaused OnGamePaused;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnGameResumed OnGameResumed;
};

