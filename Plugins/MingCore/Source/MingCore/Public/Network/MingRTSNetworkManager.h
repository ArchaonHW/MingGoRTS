// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MingRTSNetworkManager.generated.h"

UENUM(BlueprintType)
enum class ENetworkGameMode: uint8 {
	SinglePlayer		UMETA(DisplayName = "單人遊戲"),
	ListenServer			UMETA(DisplayName = "監聽服務器"),
	DedicatedServer			UMETA(DisplayName = "專用服務器"),
	Client				UMETA(DisplayName = "客戶端")
};

UENUM(BlueprintType)
enum class ENetworkConnectionState: uint8 {
	Disconnected			UMETA(DisplayName = "斷開連接"),
	Connecting				UMETA(DisplayName = "正在連接"),
	Connected				UMETA(DisplayName = "已連接"),
	Ready					UMETA(DisplayName = "就緒"),
	Error					UMETA(DisplayName = "錯誤")
};

UENUM(BlueprintType)
enum class ENetworkSyncPriority: uint8 {
	Critical				UMETA(DisplayName = "關鍵"),
	High					UMETA(DisplayName = "高"),
	Normal					UMETA(DisplayName = "普通"),
	Low						UMETA(DisplayName = "低"),
	Background				UMETA(DisplayName = "後台")
};

USTRUCT(BlueprintType)
struct FNetworkPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	int32 PlayerId;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	bool bIsHost;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	bool bIsReady;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	float Ping;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	ENetworkConnectionState ConnectionState;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	FDateTime JoinTime;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	TMap<FString, FString> PlayerData;

	FNetworkPlayerInfo()
		: PlayerId(0)
		, bIsHost(false)
		, bIsReady(false)
		, Ping(0.0f)
		, ConnectionState(ENetworkConnectionState::Disconnected)
		, JoinTime(FDateTime::Now())
	{
	}
};

USTRUCT(BlueprintType)
struct FNetworkSessionConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	FString SessionName;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	bool bIsLAN;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	bool bIsPresence;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	bool bShouldAdvertise;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	bool bAllowJoinInProgress;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	int32 MinPlayers;

	UPROPERTY(BlueprintReadWrite, Category = "Network")
	TMap<FString, FString> CustomSettings;

	FNetworkSessionConfig()
		: SessionName(TEXT("MingRTSGame"))
		, MaxPlayers(8)
		, bIsLAN(false)
		, bIsPresence(true)
		, bShouldAdvertise(true)
		, bAllowJoinInProgress(true)
		, MinPlayers(2)
	{
	}
};

USTRUCT(BlueprintType)
struct FNetworkSyncedData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	FString DataId;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	FString DataType;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	TArray<uint8> Data;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	ENetworkSyncPriority Priority;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	float Timestamp;

	UPROPERTY(BlueprintReadOnly, Category = "Network")
	int32 Version;

	FNetworkSyncedData()
		: Priority(ENetworkSyncPriority::Normal)
		, Timestamp(0.0f)
		, Version(1)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerJoined, const FNetworkPlayerInfo&, PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerLeft, const FNetworkPlayerInfo&, PlayerInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerReadyChanged, int32, PlayerId, bool, bIsReady);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNetworkError, const FString&, ErrorMessage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConnectionStateChanged, ENetworkConnectionState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllPlayersReady);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingRTSNetworkManager : public UObject
{
	GENERATED_BODY()

public:
	UMingRTSNetworkManager();

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool InitializeNetworkManager(UWorld* InWorld);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void ShutdownNetworkManager();

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool CreateSession(const FNetworkSessionConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool JoinSession(const FString& SessionId);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void LeaveSession();

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool StartGame();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetPlayerReady(bool bReady);

	UFUNCTION(BlueprintCallable, Category = "Network")
	TArray<FNetworkPlayerInfo> GetAllPlayers() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	FNetworkPlayerInfo GetLocalPlayerInfo() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	FNetworkPlayerInfo GetPlayerInfo(int32 PlayerId) const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool IsHost() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	ENetworkConnectionState GetConnectionState() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	void KickPlayer(int32 PlayerId, const FString& Reason);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void BanPlayer(int32 PlayerId, const FString& Reason);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void UnbanPlayer(int32 PlayerId);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SyncDataToAll(const FNetworkSyncedData& Data);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SyncDataToPlayer(int32 PlayerId, const FNetworkSyncedData& Data);

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetNetworkGameMode(ENetworkGameMode Mode);

	UFUNCTION(BlueprintCallable, Category = "Network")
	ENetworkGameMode GetNetworkGameMode() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetNetworkTickRate(float TickRate);

	UFUNCTION(BlueprintCallable, Category = "Network")
	float GetNetworkTickRate() const;

	UFUNCTION(BlueprintCallable, Category = "Network")
	void SetReplicationDistance(float Distance);

	UFUNCTION(BlueprintCallable, Category = "Network")
	FString GetNetworkStatistics() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnPlayerJoined OnPlayerJoined;

	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnPlayerLeft OnPlayerLeft;

	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnPlayerReadyChanged OnPlayerReadyChanged;

	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnNetworkError OnNetworkError;

	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnConnectionStateChanged OnConnectionStateChanged;

	UPROPERTY(BlueprintAssignable, Category = "Network|Events")
	FOnAllPlayersReady OnAllPlayersReady;

protected:
	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	ENetworkGameMode CurrentGameMode;

	UPROPERTY()
	ENetworkConnectionState ConnectionState;

	UPROPERTY()
	FNetworkSessionConfig SessionConfig;

	UPROPERTY()
	TMap<int32, FNetworkPlayerInfo> ConnectedPlayers;

	UPROPERTY()
	FNetworkPlayerInfo LocalPlayer;

	UPROPERTY()
	TArray<int32> BannedPlayers;

	UPROPERTY()
	float NetworkTickRate;

	UPROPERTY()
	float ReplicationDistance;

	IOnlineSessionPtr OnlineSessionInterface;
	FName CurrentSessionName;
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

protected:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);

	void NetworkTick(float DeltaTime);
	void ProcessPlayerConnections();
	void SyncGameStateToClients();
	void HandleNetworkError(const FString& Error);
	void UpdateConnectionState(ENetworkGameMode NewState);
	bool AllPlayersReady() const;
	void NotifyPlayerJoined(int32 PlayerId);
	void NotifyPlayerLeft(int32 PlayerId);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetPlayerReady(bool bReady);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRequestGameStart();

	UFUNCTION(Client, Reliable)
	void ClientGameStarted();

	UFUNCTION(Client, Reliable)
	void ClientReceiveSyncedData(const FNetworkSyncedData& Data);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerJoined(const FNetworkPlayerInfo& PlayerInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlayerLeft(const FNetworkPlayerInfo& PlayerInfo);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastGameStarted();
};
