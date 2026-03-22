// Copyright (c) 2026 MingGoRTS. All rights reserved.
// Lobby System for Multiplayer Games

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingLobbySystem.generated.h"

UENUM(BlueprintType)
enum class EMingLobbyType : uint8
{
    Public, UMETA(DisplayName = "Public Lobby"),
    Private, UMETA(DisplayName = "Private Lobby"),
    FriendsOnly, UMETA(DisplayName = "Friends Only"),
    Tournament, UMETA(DisplayName = "Tournament Lobby"),
    Custom, UMETA(DisplayName = "Custom Lobby")
};

UENUM(BlueprintType)
enum class EMingLobbyState : uint8
{
    Waiting, UMETA(DisplayName = "Waiting"),
    InProgress, UMETA(DisplayName = "In Progress"),
    Starting, UMETA(DisplayName = "Starting"),
    Finished, UMETA(DisplayName = "Finished"),
    Cancelled, UMETA(DisplayName = "Cancelled")
};

UENUM(BlueprintType)
enum class EMingLobbyPlayerState : uint8
{
    Connected, UMETA(DisplayName = "Connected"),
    Ready, UMETA(DisplayName = "Ready"),
    NotReady, UMETA(DisplayName = "Not Ready"),
    Loading, UMETA(DisplayName = "Loading"),
    Disconnected, UMETA(DisplayName = "Disconnected")
};

USTRUCT(BlueprintType)
struct FMingLobbySettings
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Settings")
    EMingLobbyType LobbyType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Settings")
    int32 MaxPlayers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Settings")
    FString LobbyName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Settings")
    FString Password;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lobby Settings")
    bool bIsRanked;

    FMingLobbySettings()
        : LobbyType(EMingLobbyType::Public)
        , MaxPlayers(8)
        , LobbyName(TEXT(""))
        , Password(TEXT(""))
        , bIsRanked(false)
    {}
};

USTRUCT(BlueprintType)
struct FMingLobbyPlayer
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Lobby Player")
    int32 PlayerID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby Player")
    FString PlayerName;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby Player")
    EMingLobbyPlayerState PlayerState;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby Player")
    int32 TeamID;

    UPROPERTY(BlueprintReadOnly, Category = "Lobby Player")
    bool bIsHost;

    FMingLobbyPlayer()
        : PlayerID(0)
        , PlayerName(TEXT(""))
        , PlayerState(EMingLobbyPlayerState::Connected)
        , TeamID(0)
        , bIsHost(false)
    {}
};

/**
 * MingGoRTS Lobby System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGPERSONAL_API UMingLobbySystem : public UObject
{
    GENERATED_BODY()

public:
    UMingLobbySystem();

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void InitializeLobbySystem();

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    bool CreateLobby(const FMingLobbySettings& Settings);

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    bool JoinLobby(const FString& LobbyID, const FString& Password = TEXT(""));

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void LeaveLobby();

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void StartLobby();

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void CancelLobby();

    UFUNCTION(BlueprintPure, Category = "Lobby System")
    bool IsInLobby() const;

    UFUNCTION(BlueprintPure, Category = "Lobby System")
    EMingLobbyState GetLobbyState() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void SetPlayerReady(bool bReady);

    UFUNCTION(BlueprintPure, Category = "Lobby System")
    TArray<FMingLobbyPlayer> GetPlayers() const;

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void KickPlayer(int32 PlayerID);

    UFUNCTION(BlueprintCallable, Category = "Lobby System")
    void UpdateLobbySettings(const FMingLobbySettings& Settings);

protected:
    UPROPERTY()
    FMingLobbySettings CurrentSettings;

    UPROPERTY()
    TArray<FMingLobbyPlayer> Players;

    UPROPERTY()
    EMingLobbyState CurrentState;

    UPROPERTY()
    int32 LocalPlayerID;

    UPROPERTY()
    bool bIsInLobby;

    void BroadcastLobbyUpdate();
    FMingLobbyPlayer* GetPlayerByID(int32 PlayerID);
    const FMingLobbyPlayer* GetPlayerByID(int32 PlayerID) const;
    void AddPlayerToTeam(int32 PlayerID, int32 TeamID);
    void RemovePlayerFromTeam(int32 PlayerID);
    void UpdatePlayerState(int32 PlayerID, EMingLobbyPlayerState NewState);
    bool ValidateSettings(const FMingLobbySettings& Settings) const;
    void SendSystemMessage(const FString& Message);
};
