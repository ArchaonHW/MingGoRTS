#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLobbySystem.generated.h"

// Forward declarations
class UMingNetworkManager;
class UMingPersonalManager;

// Lobby game mode
UENUM(BlueprintType)
enum class EMingLobbyGameMode : uint8
{
    RTSStandard       UMETA(DisplayName = "RTS Standard"),
    RTSTeamBattle     UMETA(DisplayName = "RTS Team Battle"),
    RTSCoopCampaign   UMETA(DisplayName = "RTS Co-op Campaign"),
    FreeForAll        UMETA(DisplayName = "Free For All"),
    HistoricalBattle  UMETA(DisplayName = "Historical Battle"),
    Custom            UMETA(DisplayName = "Custom")
};

// Lobby player state
UENUM(BlueprintType)
enum class EMingLobbyPlayerState : uint8
{
    Connected         UMETA(DisplayName = "Connected"),
    Ready             UMETA(DisplayName = "Ready"),
    Loading           UMETA(DisplayName = "Loading"),
    InGame            UMETA(DisplayName = "In Game"),
    Disconnected      UMETA(DisplayName = "Disconnected")
};

// Lobby player info
USTRUCT(BlueprintType)
struct FMingLobbyPlayer
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    int32 PlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    int32 TeamID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    EMingLobbyPlayerState PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    bool bIsHost;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString PlayerAvatar;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    int32 PlayerLevel;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString SelectedFaction;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString SelectedColor;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    bool bIsSpectator;

    FMingLobbyPlayer()
        : PlayerID(-1)
        , PlayerName(TEXT(""))
        , TeamID(0)
        , PlayerState(EMingLobbyPlayerState::Connected)
        , bIsHost(false)
        , PlayerAvatar(TEXT(""))
        , PlayerLevel(1)
        , SelectedFaction(TEXT("Ming"))
        , SelectedColor(TEXT("Red"))
        , bIsSpectator(false)
    {}
};

// Lobby settings
USTRUCT(BlueprintType)
struct FMingLobbySettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    FString LobbyName;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    EMingLobbyGameMode GameMode;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    FString MapName;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    int32 MaxPlayers;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    int32 NumTeams;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    bool bAllowSpectators;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    bool bAutoBalanceTeams;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    int32 StartingResources;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    int32 GameTimeLimit;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    TArray<FString> AvailableFactions;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    TArray<FString> DisabledUnits;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    bool bEnableCheats;

    UPROPERTY(BlueprintReadWrite, Category = "Lobby")
    float GameSpeed;

    FMingLobbySettings()
        : LobbyName(TEXT("MingGoRTS Lobby"))
        , GameMode(EMingLobbyGameMode::RTSStandard)
        , MapName(TEXT("Ming Dynasty Battlefield"))
        , MaxPlayers(4)
        , NumTeams(2)
        , bAllowSpectators(true)
        , bAutoBalanceTeams(true)
        , StartingResources(1000)
        , GameTimeLimit(0)
        , bEnableCheats(false)
        , GameSpeed(1.0f)
    {
        AvailableFactions.Add(TEXT("Ming"));
        AvailableFactions.Add(TEXT("Qing"));
        AvailableFactions.Add(TEXT("Japanese"));
        AvailableFactions.Add(TEXT("Mongol"));
    }
};

// Lobby chat message
USTRUCT(BlueprintType)
struct FMingLobbyChatMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    int32 SenderPlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString SenderName;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString Message;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    float Timestamp;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    bool bIsSystemMessage;

    FMingLobbyChatMessage()
        : SenderPlayerID(-1)
        , SenderName(TEXT(""))
        , Message(TEXT(""))
        , Timestamp(0.0f)
        , bIsSystemMessage(false)
    {}
};

// Lobby team info
USTRUCT(BlueprintType)
struct FMingLobbyTeam
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    int32 TeamID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString TeamName;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    FString TeamColor;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    TArray<int32> PlayerIDs;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby")
    bool bIsFull;

    FMingLobbyTeam()
        : TeamID(0)
        , TeamName(TEXT("Team 1"))
        , TeamColor(TEXT("Blue"))
        , bIsFull(false)
    {}
};

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerJoined, FMingLobbyPlayer, Player);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerLeft, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerStateChanged, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyPlayerTeamChanged, int32, PlayerID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbySettingsChanged, FMingLobbySettings, NewSettings);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyChatMessageReceived, FMingLobbyChatMessage, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyGameStarting);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLobbyGameStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCountdownChanged, int32, SecondsRemaining);

/**
 * 大廳系統
 * 管�?多人?�戲大廳?�創建、�??��??�置
 */
UCLASS(ClassGroup = (MingGoRTS), meta = (BlueprintSpawnableComponent))
class MINGPERSONAL_API UMingLobbySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingLobbySystem();

    // Initialize/Shutdown
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void Initialize(UMingNetworkManager* InNetworkManager);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void Shutdown();

    // Lobby creation
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    bool CreateLobby(const FMingLobbySettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void CloseLobby();

    // Lobby joining
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    bool JoinLobby(const FString& LobbyID, const FString& Password = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void LeaveLobby();

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool IsInLobby() const;

    // Player management
    UFUNCTION(BlueprintPure, Category = "Lobby")
    TArray<FMingLobbyPlayer> GetLobbyPlayers() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    FMingLobbyPlayer GetLocalPlayer() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerReady(bool bReady);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerTeam(int32 TeamID);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerFaction(const FString& FactionName);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerColor(const FString& ColorName);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetPlayerSpectator(bool bSpectator);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void KickPlayerFromLobby(int32 PlayerID, const FString& Reason);

    // Settings management
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void UpdateLobbySettings(const FMingLobbySettings& NewSettings);

    UFUNCTION(BlueprintPure, Category = "Lobby")
    FMingLobbySettings GetLobbySettings() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetGameMode(EMingLobbyGameMode NewMode);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetMap(const FString& MapName);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetMaxPlayers(int32 MaxPlayers);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SetNumTeams(int32 NumTeams);

    // Team management
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void AutoBalanceTeams();

    UFUNCTION(BlueprintPure, Category = "Lobby")
    TArray<FMingLobbyTeam> GetTeams() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    int32 GetTeamPlayerCount(int32 TeamID) const;

    // Game start
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void StartGameCountdown(int32 CountdownSeconds = 10);

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void CancelGameCountdown();

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool IsCountdownActive() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    int32 GetCountdownRemaining() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void StartGame();

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool CanStartGame() const;

    // Chat system
    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void SendLobbyChatMessage(const FString& Message);

    UFUNCTION(BlueprintPure, Category = "Lobby")
    TArray<FMingLobbyChatMessage> GetChatHistory() const;

    // Lobby info
    UFUNCTION(BlueprintPure, Category = "Lobby")
    FString GetLobbyID() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool IsLobbyHost() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    int32 GetPlayerCount() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    int32 GetReadyPlayerCount() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby")
    void GetLobbyInfo(FString& OutInfo) const;

    // Validation
    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool IsPlayerReady(int32 PlayerID) const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool AreAllPlayersReady() const;

    UFUNCTION(BlueprintPure, Category = "Lobby")
    bool IsValidTeamConfiguration() const;

    // Delegates
    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyPlayerJoined OnPlayerJoined;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyPlayerLeft OnPlayerLeft;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyPlayerStateChanged OnPlayerStateChanged;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyPlayerTeamChanged OnPlayerTeamChanged;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbySettingsChanged OnSettingsChanged;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyChatMessageReceived OnChatMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyGameStarting OnGameStarting;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyGameStarted OnGameStarted;

    UPROPERTY(BlueprintAssignable, Category = "Lobby Events")
    FOnLobbyCountdownChanged OnCountdownChanged;

private:
    UPROPERTY()
    UMingNetworkManager* NetworkManager;

    UPROPERTY()
    FString LobbyID;

    UPROPERTY()
    FMingLobbySettings CurrentSettings;

    UPROPERTY()
    TArray<FMingLobbyPlayer> LobbyPlayers;

    UPROPERTY()
    TArray<FMingLobbyChatMessage> ChatHistory;

    UPROPERTY()
    TArray<FMingLobbyTeam> Teams;

    UPROPERTY()
    bool bInLobby;

    UPROPERTY()
    bool bIsHost;

    UPROPERTY()
    bool bCountdownActive;

    UPROPERTY()
    int32 CountdownRemaining;

    UPROPERTY()
    FTimerHandle CountdownTimerHandle;

    // Internal functions
    void GenerateLobbyID();
    void UpdateTeams();
    void BroadcastSettingsChanged();
    void BroadcastChatMessage(const FMingLobbyChatMessage& Message);
    void UpdateCountdown();
    void OnCountdownFinished();
    FMingLobbyPlayer* GetPlayerByID(int32 PlayerID);
    const FMingLobbyPlayer* GetPlayerByID(int32 PlayerID) const;
    void AddPlayerToTeam(int32 PlayerID, int32 TeamID);
    void RemovePlayerFromTeam(int32 PlayerID);
    void UpdatePlayerState(int32 PlayerID, EMingLobbyPlayerState NewState);
    bool ValidateSettings(const FMingLobbySettings& Settings) const;
    void SendSystemMessage(const FString& Message);
};
