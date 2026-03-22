// Copyright Epic Games, Inc. All Rights Reserved.

#include "Network/MingRTSNetworkManager.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

UMingRTSNetworkManager::UMingRTSNetworkManager()
	: World(nullptr)
	, CurrentGameMode(ENetworkGameMode::SinglePlayer)
	, ConnectionState(ENetworkConnectionState::Disconnected)
	, NetworkTickRate(30.0f)
	, ReplicationDistance(10000.0f)
{
}

bool UMingRTSNetworkManager::InitializeNetworkManager(UWorld* InWorld)
{
	if (!InWorld)
	{
		UE_LOG(LogTemp, Error, TEXT("Cannot initialize network manager with null world"));
		return false;
	}

	World = InWorld;

	// Get Online Subsystem
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		OnlineSessionInterface = OnlineSubsystem->GetSessionInterface();
		if (OnlineSessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Log, TEXT("Online Session Interface initialized"));
		}
	}

	// Start network tick
	FTimerDelegate NetworkTickDelegate;
	NetworkTickDelegate.BindUObject(this, &UMingRTSNetworkManager::NetworkTick, 0.0f);
	World->GetTimerManager().SetTimer(NetworkTickHandle, NetworkTickDelegate, 1.0f / NetworkTickRate, true);

	UE_LOG(LogTemp, Log, TEXT("Network Manager initialized"));
	return true;
}

void UMingRTSNetworkManager::ShutdownNetworkManager()
{
	if (World)
	{
		World->GetTimerManager().ClearTimer(NetworkTickHandle);
	}

	// Leave current session if connected
	if (IsConnected())
	{
		LeaveSession();
	}

	// Clear delegates
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
		OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
		OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	}

	ConnectedPlayers.Empty();
	World = nullptr;

	UE_LOG(LogTemp, Log, TEXT("Network Manager shut down"));
}

bool UMingRTSNetworkManager::CreateSession(const FNetworkSessionConfig& Config)
{
	if (!OnlineSessionInterface.IsValid())
	{
		HandleNetworkError(TEXT("Online Session Interface not available"));
		return false;
	}

	SessionConfig = Config;
	CurrentSessionName = FName(*SessionConfig.SessionName);

	// Configure session settings
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = SessionConfig.bIsLAN;
	SessionSettings.NumPublicConnections = SessionConfig.MaxPlayers;
	SessionSettings.bShouldAdvertise = SessionConfig.bShouldAdvertise;
	SessionSettings.bAllowJoinInProgress = SessionConfig.bAllowJoinInProgress;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bUsesPresence = SessionConfig.bIsPresence;
	SessionSettings.bAllowJoinViaPresence = SessionConfig.bIsPresence;

	// Add custom settings
	for (const auto& Setting : SessionConfig.CustomSettings)
	{
		SessionSettings.Set(FName(*Setting.Key), Setting.Value, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}

	// Create delegate
	OnCreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(
		FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMingRTSNetworkManager::OnCreateSessionComplete));

	// Create session
	return OnlineSessionInterface->CreateSession(0, CurrentSessionName, SessionSettings);
}

bool UMingRTSNetworkManager::JoinSession(const FString& SessionId)
{
	if (!OnlineSessionInterface.IsValid())
	{
		HandleNetworkError(TEXT("Online Session Interface not available"));
		return false;
	}

	// Find the session with the given ID
	FOnlineSessionSearch SearchSettings;
	SearchSettings.MaxSearchResults = 100;
	SearchSettings.bIsLanQuery = SessionConfig.bIsLAN;

	// Set up delegate for join
	OnJoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(
		FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMingRTSNetworkManager::OnJoinSessionComplete));

	CurrentSessionName = FName(*SessionId);
	UpdateConnectionState(ENetworkConnectionState::Connecting);

	return true;
}

void UMingRTSNetworkManager::LeaveSession()
{
	if (!OnlineSessionInterface.IsValid())
	{
		return;
	}

	OnDestroySessionCompleteDelegateHandle = OnlineSessionInterface->AddOnDestroySessionCompleteDelegate_Handle(
		FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMingRTSNetworkManager::OnDestroySessionComplete));

	OnlineSessionInterface->DestroySession(CurrentSessionName);
	UpdateConnectionState(ENetworkConnectionState::Disconnected);
}

bool UMingRTSNetworkManager::StartGame()
{
	if (!IsHost())
	{
		UE_LOG(LogTemp, Warning, TEXT("Only host can start the game"));
		return false;
	}

	if (!AllPlayersReady())
	{
		UE_LOG(LogTemp, Warning, TEXT("Not all players are ready"));
		return false;
	}

	// Start the session
	if (OnlineSessionInterface.IsValid())
	{
		OnStartSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnStartSessionCompleteDelegate_Handle(
			FOnStartSessionCompleteDelegate::CreateUObject(this, &UMingRTSNetworkManager::OnStartSessionComplete));
		
		OnlineSessionInterface->StartSession(CurrentSessionName);
	}

	MulticastGameStarted();
	return true;
}

void UMingRTSNetworkManager::EndGame()
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->EndSession(CurrentSessionName);
	}
}

void UMingRTSNetworkManager::SetPlayerReady(bool bReady)
{
	LocalPlayer.bIsReady = bReady;
	ServerSetPlayerReady(bReady);
}

TArray<FNetworkPlayerInfo> UMingRTSNetworkManager::GetAllPlayers() const
{
	TArray<FNetworkPlayerInfo> Players;
	ConnectedPlayers.GenerateValueArray(Players);
	return Players;
}

FNetworkPlayerInfo UMingRTSNetworkManager::GetLocalPlayerInfo() const
{
	return LocalPlayer;
}

FNetworkPlayerInfo UMingRTSNetworkManager::GetPlayerInfo(int32 PlayerId) const
{
	const FNetworkPlayerInfo* Found = ConnectedPlayers.Find(PlayerId);
	if (Found)
	{
		return *Found;
	}
	return FNetworkPlayerInfo();
}

bool UMingRTSNetworkManager::IsHost() const
{
	return LocalPlayer.bIsHost;
}

bool UMingRTSNetworkManager::IsConnected() const
{
	return ConnectionState == ENetworkConnectionState::Connected || 
		   ConnectionState == ENetworkConnectionState::Ready;
}

ENetworkConnectionState UMingRTSNetworkManager::GetConnectionState() const
{
	return ConnectionState;
}

void UMingRTSNetworkManager::KickPlayer(int32 PlayerId, const FString& Reason)
{
	if (!IsHost())
	{
		return;
	}

	// Implementation would use OnlineSubsystem to kick player
	UE_LOG(LogTemp, Log, TEXT("Kicking player %d: %s"), PlayerId, *Reason);
}

void UMingRTSNetworkManager::BanPlayer(int32 PlayerId, const FString& Reason)
{
	if (!IsHost())
	{
		return;
	}

	BannedPlayers.AddUnique(PlayerId);
	KickPlayer(PlayerId, Reason);
	
	UE_LOG(LogTemp, Log, TEXT("Banning player %d: %s"), PlayerId, *Reason);
}

void UMingRTSNetworkManager::UnbanPlayer(int32 PlayerId)
{
	if (!IsHost())
	{
		return;
	}

	BannedPlayers.Remove(PlayerId);
	UE_LOG(LogTemp, Log, TEXT("Unbanned player %d"), PlayerId);
}

void UMingRTSNetworkManager::SyncDataToAll(const FNetworkSyncedData& Data)
{
	// Send to all connected players
	for (const auto& Player : ConnectedPlayers)
	{
		if (Player.Key != LocalPlayer.PlayerId)
		{
			SyncDataToPlayer(Player.Key, Data);
		}
	}
}

void UMingRTSNetworkManager::SyncDataToPlayer(int32 PlayerId, const FNetworkSyncedData& Data)
{
	ClientReceiveSyncedData(Data);
}

void UMingRTSNetworkManager::SetNetworkGameMode(ENetworkGameMode Mode)
{
	CurrentGameMode = Mode;
}

ENetworkGameMode UMingRTSNetworkManager::GetNetworkGameMode() const
{
	return CurrentGameMode;
}

void UMingRTSNetworkManager::SetNetworkTickRate(float TickRate)
{
	NetworkTickRate = FMath::Clamp(TickRate, 1.0f, 120.0f);
	
	if (World)
	{
		World->GetTimerManager().ClearTimer(NetworkTickHandle);
		FTimerDelegate NetworkTickDelegate;
		NetworkTickDelegate.BindUObject(this, &UMingRTSNetworkManager::NetworkTick, 0.0f);
		World->GetTimerManager().SetTimer(NetworkTickHandle, NetworkTickDelegate, 1.0f / NetworkTickRate, true);
	}
}

float UMingRTSNetworkManager::GetNetworkTickRate() const
{
	return NetworkTickRate;
}

void UMingRTSNetworkManager::SetReplicationDistance(float Distance)
{
	ReplicationDistance = FMath::Max(0.0f, Distance);
}

FString UMingRTSNetworkManager::GetNetworkStatistics() const
{
	FString Stats;
	Stats += FString::Printf(TEXT("Network Statistics\n"));
	Stats += FString::Printf(TEXT("====================\n"));
	Stats += FString::Printf(TEXT("Game Mode: %s\n"), *UEnum::GetValueAsString(CurrentGameMode));
	Stats += FString::Printf(TEXT("Connection State: %s\n"), *UEnum::GetValueAsString(ConnectionState));
	Stats += FString::Printf(TEXT("Connected Players: %d\n"), ConnectedPlayers.Num());
	Stats += FString::Printf(TEXT("Local Player: %s (ID: %d)\n"), *LocalPlayer.PlayerName, LocalPlayer.PlayerId);
	Stats += FString::Printf(TEXT("Is Host: %s\n"), IsHost() ? TEXT("Yes") : TEXT("No"));
	Stats += FString::Printf(TEXT("Tick Rate: %.1f Hz\n"), NetworkTickRate);
	Stats += FString::Printf(TEXT("Replication Distance: %.1f\n"), ReplicationDistance);
	
	if (ConnectedPlayers.Num() > 0)
	{
		Stats += TEXT("\nPlayer List:\n");
		for (const auto& Player : ConnectedPlayers)
		{
			Stats += FString::Printf(TEXT("  - %s (ID: %d, Ping: %.1f ms)\n"), 
				*Player.Value.PlayerName, Player.Key, Player.Value.Ping);
		}
	}
	
	return Stats;
}

void UMingRTSNetworkManager::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session created successfully: %s"), *SessionName.ToString());
		UpdateConnectionState(ENetworkConnectionState::Connected);
		
		// Set local player as host
		LocalPlayer.bIsHost = true;
		LocalPlayer.PlayerId = 1;
		LocalPlayer.PlayerName = TEXT("Host");
		ConnectedPlayers.Add(LocalPlayer.PlayerId, LocalPlayer);
	}
	else
	{
		HandleNetworkError(TEXT("Failed to create session"));
	}
}

void UMingRTSNetworkManager::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	OnlineSessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
	
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session started: %s"), *SessionName.ToString());
	}
}

void UMingRTSNetworkManager::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Joined session: %s"), *SessionName.ToString());
		UpdateConnectionState(ENetworkConnectionState::Connected);
		
		// Get connection info
		FString ConnectInfo;
		OnlineSessionInterface->GetResolvedConnectString(SessionName, ConnectInfo);
		
		// Set up local player
		LocalPlayer.bIsHost = false;
		LocalPlayer.PlayerId = FMath::RandRange(1000, 9999); // Temporary ID
		LocalPlayer.PlayerName = FString::Printf(TEXT("Player%d"), LocalPlayer.PlayerId);
	}
	else
	{
		HandleNetworkError(FString::Printf(TEXT("Failed to join session: %d"), (int32)Result));
		UpdateConnectionState(ENetworkConnectionState::Error);
	}
}

void UMingRTSNetworkManager::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	OnlineSessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	UE_LOG(LogTemp, Log, TEXT("Session destroyed: %s"), *SessionName.ToString());
}

void UMingRTSNetworkManager::OnFindSessionsComplete(bool bWasSuccessful)
{
	// Handle found sessions
}

void UMingRTSNetworkManager::NetworkTick(float DeltaTime)
{
	ProcessPlayerConnections();
	
	if (IsHost())
	{
		SyncGameStateToClients();
	}
}

void UMingRTSNetworkManager::ProcessPlayerConnections()
{
	// Check for new connections and disconnections
	// Update player ping times
	// Handle timeout disconnections
}

void UMingRTSNetworkManager::SyncGameStateToClients()
{
	// Send relevant game state to all connected clients
	// Use interest management based on ReplicationDistance
}

void UMingRTSNetworkManager::HandleNetworkError(const FString& Error)
{
	UE_LOG(LogTemp, Error, TEXT("Network Error: %s"), *Error);
	OnNetworkError.Broadcast(Error);
	UpdateConnectionState(ENetworkConnectionState::Error);
}

void UMingRTSNetworkManager::UpdateConnectionState(ENetworkGameMode NewState)
{
	if (ConnectionState != NewState)
	{
		ConnectionState = NewState;
		OnConnectionStateChanged.Broadcast(ConnectionState);
	}
}

bool UMingRTSNetworkManager::AllPlayersReady() const
{
	if (ConnectedPlayers.Num() < SessionConfig.MinPlayers)
	{
		return false;
	}
	
	for (const auto& Player : ConnectedPlayers)
	{
		if (!Player.Value.bIsReady)
		{
			return false;
		}
	}
	
	return true;
}

void UMingRTSNetworkManager::NotifyPlayerJoined(int32 PlayerId)
{
	const FNetworkPlayerInfo* PlayerInfo = ConnectedPlayers.Find(PlayerId);
	if (PlayerInfo)
	{
		OnPlayerJoined.Broadcast(*PlayerInfo);
		MulticastPlayerJoined(*PlayerInfo);
	}
}

void UMingRTSNetworkManager::NotifyPlayerLeft(int32 PlayerId)
{
	const FNetworkPlayerInfo* PlayerInfo = ConnectedPlayers.Find(PlayerId);
	if (PlayerInfo)
	{
		OnPlayerLeft.Broadcast(*PlayerInfo);
		MulticastPlayerLeft(*PlayerInfo);
		ConnectedPlayers.Remove(PlayerId);
	}
}

bool UMingRTSNetworkManager::ServerSetPlayerReady_Validate(bool bReady)
{
	return true;
}

void UMingRTSNetworkManager::ServerSetPlayerReady_Implementation(bool bReady)
{
	LocalPlayer.bIsReady = bReady;
	OnPlayerReadyChanged.Broadcast(LocalPlayer.PlayerId, bReady);
	
	if (AllPlayersReady())
	{
		OnAllPlayersReady.Broadcast();
	}
}

bool UMingRTSNetworkManager::ServerRequestGameStart_Validate()
{
	return IsHost();
}

void UMingRTSNetworkManager::ServerRequestGameStart_Implementation()
{
	StartGame();
}

void UMingRTSNetworkManager::ClientGameStarted_Implementation()
{
	UE_LOG(LogTemp, Log, TEXT("Game started on client"));
	UpdateConnectionState(ENetworkConnectionState::Ready);
}

void UMingRTSNetworkManager::ClientReceiveSyncedData_Implementation(const FNetworkSyncedData& Data)
{
	// Process received synced data
	UE_LOG(LogTemp, Verbose, TEXT("Received synced data: %s"), *Data.DataId);
}

void UMingRTSNetworkManager::MulticastPlayerJoined_Implementation(const FNetworkPlayerInfo& PlayerInfo)
{
	ConnectedPlayers.Add(PlayerInfo.PlayerId, PlayerInfo);
	UE_LOG(LogTemp, Log, TEXT("Player joined: %s"), *PlayerInfo.PlayerName);
}

void UMingRTSNetworkManager::MulticastPlayerLeft_Implementation(const FNetworkPlayerInfo& PlayerInfo)
{
	ConnectedPlayers.Remove(PlayerInfo.PlayerId);
	UE_LOG(LogTemp, Log, TEXT("Player left: %s"), *PlayerInfo.PlayerName);
}

void UMingRTSNetworkManager::MulticastGameStarted_Implementation()
{
	ClientGameStarted();
}
