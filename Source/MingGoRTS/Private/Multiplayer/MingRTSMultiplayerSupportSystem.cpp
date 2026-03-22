#include "Multiplayer/MingRTSMultiplayerSupportSystem.h"
#include "Misc/DateTime.h"

UMingRTSMultiplayerSupportSystem::UMingRTSMultiplayerSupportSystem()
    : ConnectionState(EMultiplayerConnectionState::Offline)
    , ServerPort(7777)
    , bIsHost(false)
    , CurrentPing(0)
{
}

void UMingRTSMultiplayerSupportSystem::InitializeMultiplayerSystem()
{
    LocalPlayer.PlayerID = FString::Printf(TEXT("PLAYER_%d"), FMath::Rand());
    LocalPlayer.PlayerName = TEXT("Player") + FString::FromInt(FMath::RandRange(1, 9999));
    LocalPlayer.Role = ENetworkPlayerRole::None;
    LocalPlayer.bIsReady = false;
    LocalPlayer.TeamID = 0;
    
    UE_LOG(LogTemp, Log, TEXT("Multiplayer Support System Initialized. Local Player: %s"), *LocalPlayer.PlayerID);
}

void UMingRTSMultiplayerSupportSystem::ConnectToServer(const FString& InServerAddress, int32 InPort)
{
    if (ConnectionState != EMultiplayerConnectionState::Offline)
    {
        UE_LOG(LogTemp, Warning, TEXT("Already connected or connecting"));
        return;
    }
    
    ServerAddress = InServerAddress;
    ServerPort = InPort;
    
    UpdateConnectionState(EMultiplayerConnectionState::Connecting);
    
    // 模擬連接過程
    UE_LOG(LogTemp, Log, TEXT("Connecting to server: %s:%d"), *ServerAddress, ServerPort);
    
    // 這裡應該實際連接到服務器
    // 簡化處理，模擬成功連接
    UpdateConnectionState(EMultiplayerConnectionState::Connected);
    UpdateConnectionState(EMultiplayerConnectionState::Authenticating);
    
    // 模擬認證成功
    LocalPlayer.Role = ENetworkPlayerRole::Client;
    UpdateConnectionState(EMultiplayerConnectionState::Authenticated);
    
    OnConnected.Broadcast(true);
    UE_LOG(LogTemp, Log, TEXT("Connected to server successfully"));
}

void UMingRTSMultiplayerSupportSystem::Disconnect()
{
    if (ConnectionState == EMultiplayerConnectionState::Offline)
    {
        return;
    }
    
    UpdateConnectionState(EMultiplayerConnectionState::Disconnecting);
    
    // 離開當前房間
    if (!CurrentRoom.RoomID.IsEmpty())
    {
        LeaveRoom();
    }
    
    ShutdownNetworking();
    
    UpdateConnectionState(EMultiplayerConnectionState::Offline);
    LocalPlayer.Role = ENetworkPlayerRole::None;
    bIsHost = false;
    
    OnDisconnected.Broadcast(TEXT("User disconnected"));
    UE_LOG(LogTemp, Log, TEXT("Disconnected from server"));
}

FString UMingRTSMultiplayerSupportSystem::CreateRoom(const FMultiplayerSessionConfig& Config)
{
    if (ConnectionState != EMultiplayerConnectionState::Authenticated)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot create room: Not authenticated"));
        return FString();
    }
    
    FString RoomID = GenerateRoomID();
    
    CurrentRoom.RoomID = RoomID;
    CurrentRoom.RoomName = Config.SessionName.IsEmpty() ? TEXT("New Room") : Config.SessionName;
    CurrentRoom.HostID = LocalPlayer.PlayerID;
    CurrentRoom.GameMode = Config.GameMode;
    CurrentRoom.MaxPlayers = Config.MaxPlayers;
    CurrentRoom.CurrentPlayers = 1;
    CurrentRoom.MapName = Config.MapName;
    CurrentRoom.bHasPassword = !Config.CustomSettings.FindRef(TEXT("Password")).IsEmpty();
    CurrentRoom.Password = Config.CustomSettings.FindRef(TEXT("Password"));
    CurrentRoom.bIsInProgress = false;
    
    // 添加房主到玩家列表
    LocalPlayer.Role = ENetworkPlayerRole::Host;
    LocalPlayer.JoinTime = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
    CurrentRoom.Players.Add(LocalPlayer);
    
    bIsHost = true;
    
    UpdateConnectionState(EMultiplayerConnectionState::Joining);
    
    OnRoomCreated.Broadcast(RoomID);
    
    UE_LOG(LogTemp, Log, TEXT("Room created: %s (%s)"), *RoomID, *CurrentRoom.RoomName);
    return RoomID;
}

bool UMingRTSMultiplayerSupportSystem::JoinRoom(const FString& RoomID, const FString& Password)
{
    if (ConnectionState != EMultiplayerConnectionState::Authenticated)
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot join room: Not authenticated"));
        return false;
    }
    
    // 查找房間
    for (const FMultiplayerRoom& Room : AvailableRooms)
    {
        if (Room.RoomID == RoomID)
        {
            // 檢查密碼
            if (Room.bHasPassword && Room.Password != Password)
            {
                UE_LOG(LogTemp, Warning, TEXT("Incorrect password for room: %s"), *RoomID);
                return false;
            }
            
            // 檢查是否已滿
            if (Room.CurrentPlayers >= Room.MaxPlayers)
            {
                UE_LOG(LogTemp, Warning, TEXT("Room is full: %s"), *RoomID);
                return false;
            }
            
            // 檢查遊戲是否進行中
            if (Room.bIsInProgress && !Room.bAllowJoinInProgress)
            {
                UE_LOG(LogTemp, Warning, TEXT("Game in progress, cannot join: %s"), *RoomID);
                return false;
            }
            
            CurrentRoom = Room;
            CurrentRoom.CurrentPlayers++;
            
            LocalPlayer.Role = ENetworkPlayerRole::Client;
            LocalPlayer.JoinTime = FDateTime::Now().ToString(TEXT("%Y-%m-%d %H:%M:%S"));
            CurrentRoom.Players.Add(LocalPlayer);
            
            bIsHost = false;
            
            UpdateConnectionState(EMultiplayerConnectionState::Joining);
            
            OnPlayerJoined.Broadcast(LocalPlayer);
            
            UE_LOG(LogTemp, Log, TEXT("Joined room: %s"), *RoomID);
            return true;
        }
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Room not found: %s"), *RoomID);
    return false;
}

void UMingRTSMultiplayerSupportSystem::LeaveRoom()
{
    if (CurrentRoom.RoomID.IsEmpty())
    {
        return;
    }
    
    // 如果是房主，關閉房間或轉讓房主
    if (bIsHost)
    {
        // 轉讓給下一個玩家
        for (const FNetworkPlayerInfo& Player : CurrentRoom.Players)
        {
            if (Player.PlayerID != LocalPlayer.PlayerID)
            {
                TransferHost(Player.PlayerID);
                break;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Left room: %s"), *CurrentRoom.RoomID);
    
    CurrentRoom = FMultiplayerRoom();
    UpdateConnectionState(EMultiplayerConnectionState::Authenticated);
}

TArray<FMultiplayerRoom> UMingRTSMultiplayerSupportSystem::FindRooms(EMultiplayerGameMode ModeFilter)
{
    TArray<FMultiplayerRoom> Result;
    
    // 這裡應該從服務器獲取房間列表
    // 簡化處理，返回示例房間
    
    FMultiplayerRoom ExampleRoom1;
    ExampleRoom1.RoomID = TEXT("ROOM_001");
    ExampleRoom1.RoomName = TEXT("Coop Mission Alpha");
    ExampleRoom1.HostID = TEXT("HOST_001");
    ExampleRoom1.GameMode = EMultiplayerGameMode::Cooperative;
    ExampleRoom1.MaxPlayers = 4;
    ExampleRoom1.CurrentPlayers = 2;
    ExampleRoom1.MapName = TEXT("Map_Coastal");
    ExampleRoom1.bHasPassword = false;
    ExampleRoom1.bIsInProgress = false;
    
    FMultiplayerRoom ExampleRoom2;
    ExampleRoom2.RoomID = TEXT("ROOM_002");
    ExampleRoom2.RoomName = TEXT("1v1 Ranked");
    ExampleRoom2.HostID = TEXT("HOST_002");
    ExampleRoom2.GameMode = EMultiplayerGameMode::Competitive;
    ExampleRoom2.MaxPlayers = 2;
    ExampleRoom2.CurrentPlayers = 1;
    ExampleRoom2.MapName = TEXT("Map_Desert");
    ExampleRoom2.bHasPassword = true;
    ExampleRoom2.bIsInProgress = false;
    
    AvailableRooms.Empty();
    AvailableRooms.Add(ExampleRoom1);
    AvailableRooms.Add(ExampleRoom2);
    
    for (const FMultiplayerRoom& Room : AvailableRooms)
    {
        if (ModeFilter == Room.GameMode || ModeFilter == EMultiplayerGameMode::Custom)
        {
            Result.Add(Room);
        }
    }
    
    return Result;
}

void UMingRTSMultiplayerSupportSystem::StartGame()
{
    if (!bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Only host can start the game"));
        return;
    }
    
    if (CurrentRoom.RoomID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Not in a room"));
        return;
    }
    
    // 檢查所有玩家是否準備好
    bool bAllReady = true;
    for (const FNetworkPlayerInfo& Player : CurrentRoom.Players)
    {
        if (!Player.bIsReady && Player.PlayerID != CurrentRoom.HostID)
        {
            bAllReady = false;
            break;
        }
    }
    
    if (!bAllReady)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not all players are ready"));
        return;
    }
    
    CurrentRoom.bIsInProgress = true;
    
    UpdateConnectionState(EMultiplayerConnectionState::InGame);
    
    OnGameStarted.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("Game started in room: %s"), *CurrentRoom.RoomID);
}

void UMingRTSMultiplayerSupportSystem::SetPlayerReady(bool bReady)
{
    LocalPlayer.bIsReady = bReady;
    
    // 更新房間中的玩家信息
    for (FNetworkPlayerInfo& Player : CurrentRoom.Players)
    {
        if (Player.PlayerID == LocalPlayer.PlayerID)
        {
            Player.bIsReady = bReady;
            break;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Player %s is %s"), *LocalPlayer.PlayerID, bReady ? TEXT("ready") : TEXT("not ready"));
}

FNetworkPlayerInfo UMingRTSMultiplayerSupportSystem::GetLocalPlayerInfo() const
{
    return LocalPlayer;
}

TArray<FNetworkPlayerInfo> UMingRTSMultiplayerSupportSystem::GetAllPlayers() const
{
    if (!CurrentRoom.RoomID.IsEmpty())
    {
        return CurrentRoom.Players;
    }
    return TArray<FNetworkPlayerInfo>();
}

void UMingRTSMultiplayerSupportSystem::SendChatMessage(const FString& Message)
{
    if (CurrentRoom.RoomID.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Not in a room, cannot send message"));
        return;
    }
    
    // 廣播給所有玩家
    BroadcastToAllPlayers(Message);
    
    UE_LOG(LogTemp, Log, TEXT("[%s]: %s"), *LocalPlayer.PlayerName, *Message);
}

void UMingRTSMultiplayerSupportSystem::SendGameCommand(const FString& CommandType, const TMap<FString, FString>& Parameters)
{
    if (ConnectionState != EMultiplayerConnectionState::InGame)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not in game, cannot send command"));
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("Game command sent: %s"), *CommandType);
}

void UMingRTSMultiplayerSupportSystem::KickPlayer(const FString& PlayerID, const FString& Reason)
{
    if (!bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Only host can kick players"));
        return;
    }
    
    for (int32 i = 0; i < CurrentRoom.Players.Num(); ++i)
    {
        if (CurrentRoom.Players[i].PlayerID == PlayerID)
        {
            UE_LOG(LogTemp, Log, TEXT("Player %s kicked. Reason: %s"), *PlayerID, *Reason);
            
            OnPlayerLeft.Broadcast(PlayerID);
            
            CurrentRoom.Players.RemoveAt(i);
            CurrentRoom.CurrentPlayers--;
            return;
        }
    }
}

void UMingRTSMultiplayerSupportSystem::TransferHost(const FString& NewHostID)
{
    if (!bIsHost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Only host can transfer host"));
        return;
    }
    
    for (FNetworkPlayerInfo& Player : CurrentRoom.Players)
    {
        if (Player.PlayerID == NewHostID)
        {
            CurrentRoom.HostID = NewHostID;
            Player.Role = ENetworkPlayerRole::Host;
            
            if (NewHostID == LocalPlayer.PlayerID)
            {
                bIsHost = true;
                LocalPlayer.Role = ENetworkPlayerRole::Host;
            }
            else
            {
                bIsHost = false;
                LocalPlayer.Role = ENetworkPlayerRole::Client;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Host transferred to: %s"), *NewHostID);
            return;
        }
    }
}

void UMingRTSMultiplayerSupportSystem::SetPlayerTeam(const FString& PlayerID, int32 TeamID)
{
    for (FNetworkPlayerInfo& Player : CurrentRoom.Players)
    {
        if (Player.PlayerID == PlayerID)
        {
            Player.TeamID = TeamID;
            
            if (PlayerID == LocalPlayer.PlayerID)
            {
                LocalPlayer.TeamID = TeamID;
            }
            
            UE_LOG(LogTemp, Log, TEXT("Player %s joined team %d"), *PlayerID, TeamID);
            return;
        }
    }
}

EMultiplayerConnectionState UMingRTSMultiplayerSupportSystem::GetConnectionState() const
{
    return ConnectionState;
}

FMultiplayerRoom UMingRTSMultiplayerSupportSystem::GetCurrentRoom() const
{
    return CurrentRoom;
}

bool UMingRTSMultiplayerSupportSystem::IsHost() const
{
    return bIsHost;
}

void UMingRTSMultiplayerSupportSystem::ReplicateData(const FNetworkReplicationData& Data)
{
    if (ConnectionState != EMultiplayerConnectionState::InGame)
    {
        return;
    }
    
    // 這裡應該實際發送數據到服務器
    OnDataReplicated.Broadcast(Data.ObjectID, Data);
}

int32 UMingRTSMultiplayerSupportSystem::GetPing() const
{
    // 模擬延遲計算
    return CurrentPing;
}

void UMingRTSMultiplayerSupportSystem::InitializeNetworking()
{
    UE_LOG(LogTemp, Log, TEXT("Initializing networking..."));
}

void UMingRTSMultiplayerSupportSystem::ShutdownNetworking()
{
    UE_LOG(LogTemp, Log, TEXT("Shutting down networking..."));
}

void UMingRTSMultiplayerSupportSystem::UpdateConnectionState(EMultiplayerConnectionState NewState)
{
    ConnectionState = NewState;
    UE_LOG(LogTemp, Verbose, TEXT("Connection state changed to: %d"), (int32)NewState);
}

void UMingRTSMultiplayerSupportSystem::HandlePlayerJoined(const FNetworkPlayerInfo& Player)
{
    UE_LOG(LogTemp, Log, TEXT("Player joined: %s (%s)"), *Player.PlayerName, *Player.PlayerID);
    OnPlayerJoined.Broadcast(Player);
}

void UMingRTSMultiplayerSupportSystem::HandlePlayerLeft(const FString& PlayerID)
{
    UE_LOG(LogTemp, Log, TEXT("Player left: %s"), *PlayerID);
    OnPlayerLeft.Broadcast(PlayerID);
}

void UMingRTSMultiplayerSupportSystem::HandleChatMessage(const FString& PlayerID, const FString& Message)
{
    OnChatReceived.Broadcast(PlayerID, Message);
}

void UMingRTSMultiplayerSupportSystem::HandleGameCommand(const FString& PlayerID, const FString& Command)
{
    UE_LOG(LogTemp, Log, TEXT("Game command from %s: %s"), *PlayerID, *Command);
}

void UMingRTSMultiplayerSupportSystem::BroadcastToAllPlayers(const FString& Message)
{
    // 這裡應該實際廣播到所有玩家
    UE_LOG(LogTemp, Verbose, TEXT("Broadcasting to all players: %s"), *Message);
}

void UMingRTSMultiplayerSupportSystem::SynchronizeGameState()
{
    if (ConnectionState != EMultiplayerConnectionState::InGame)
    {
        return;
    }
    
    UE_LOG(LogTemp, Verbose, TEXT("Synchronizing game state..."));
}

void UMingRTSMultiplayerSupportSystem::ProcessReplicationData(const FNetworkReplicationData& Data)
{
    UE_LOG(LogTemp, Verbose, TEXT("Processing replication data for object: %s"), *Data.ObjectID);
}

FString UMingRTSMultiplayerSupportSystem::GenerateRoomID()
{
    return FString::Printf(TEXT("ROOM_%s_%d"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")), FMath::RandRange(1000, 9999));
}
