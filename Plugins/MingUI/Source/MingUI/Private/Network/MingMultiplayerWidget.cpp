#include "Network/MingMultiplayerWidget.h"
#include "Network/MingNetworkManager.h"
#include "Network/MingLobbySystem.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ListView.h"
#include "Components/ScrollBox.h"

UMingMultiplayerWidget::UMingMultiplayerWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , NetworkManager(nullptr)
    , LobbySystem(nullptr)
    , CurrentView(TEXT("None"))
{
}

void UMingMultiplayerWidget::InitializeMultiplayerWidget(UMingNetworkManager* InNetworkManager, UMingLobbySystem* InLobbySystem)
{
    NetworkManager = InNetworkManager;
    LobbySystem = InLobbySystem;

    if (NetworkManager)
    {
        // 綁定網絡事件
        NetworkManager->OnNetworkConnected.AddDynamic(this, &UMingMultiplayerWidget::OnNetworkConnected);
        NetworkManager->OnNetworkDisconnected.AddDynamic(this, &UMingMultiplayerWidget::OnNetworkDisconnected);
        NetworkManager->OnPlayerJoined.AddDynamic(this, &UMingMultiplayerWidget::OnPlayerJoined);
        NetworkManager->OnPlayerLeft.AddDynamic(this, &UMingMultiplayerWidget::OnPlayerLeft);
    }

    if (LobbySystem)
    {
        // 綁定大廳事件
        LobbySystem->OnPlayerJoinedLobby.AddDynamic(this, &UMingMultiplayerWidget::OnPlayerJoined);
        LobbySystem->OnPlayerLeftLobby.AddDynamic(this, &UMingMultiplayerWidget::OnPlayerLeft);
        LobbySystem->OnPlayerReadyChanged.AddDynamic(this, &UMingMultiplayerWidget::OnPlayerReadyChanged);
        LobbySystem->OnGameStarting.AddDynamic(this, &UMingMultiplayerWidget::OnGameStarting);
        LobbySystem->OnLobbyChatMessage.AddDynamic(this, &UMingMultiplayerWidget::OnChatMessageReceived);
    }

    UE_LOG(LogTemp, Log, TEXT("MultiplayerWidget initialized"));
}

void UMingMultiplayerWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindUIElements();
    SetupEventBindings();

    // 默認顯示主菜單
    ShowMainMenu();
}

void UMingMultiplayerWidget::NativeDestruct()
{
    Super::NativeDestruct();

    // 解除事件綁定
    if (NetworkManager)
    {
        NetworkManager->OnNetworkConnected.RemoveDynamic(this, &UMingMultiplayerWidget::OnNetworkConnected);
        NetworkManager->OnNetworkDisconnected.RemoveDynamic(this, &UMingMultiplayerWidget::OnNetworkDisconnected);
        NetworkManager->OnPlayerJoined.RemoveDynamic(this, &UMingMultiplayerWidget::OnPlayerJoined);
        NetworkManager->OnPlayerLeft.RemoveDynamic(this, &UMingMultiplayerWidget::OnPlayerLeft);
    }
}

void UMingMultiplayerWidget::ShowMainMenu()
{
    CurrentView = TEXT("MainMenu");
    RefreshDisplay();
    UE_LOG(LogTemp, Log, TEXT("Showing main menu"));
}

void UMingMultiplayerWidget::ShowLobbyInterface()
{
    CurrentView = TEXT("Lobby");
    RefreshDisplay();
    UpdateLobbyInfoDisplay();
    UE_LOG(LogTemp, Log, TEXT("Showing lobby interface"));
}

void UMingMultiplayerWidget::ShowConnectionDialog()
{
    CurrentView = TEXT("Connection");
    RefreshDisplay();
    UE_LOG(LogTemp, Log, TEXT("Showing connection dialog"));
}

void UMingMultiplayerWidget::HideWidget()
{
    SetVisibility(ESlateVisibility::Collapsed);
    UE_LOG(LogTemp, Log, TEXT("Widget hidden"));
}

void UMingMultiplayerWidget::OnCreateServerClicked()
{
    if (NetworkManager)
    {
        FMingNetworkConfig Config;
        Config.MaxPlayers = 8;
        Config.ServerPort = 7777;

        if (NetworkManager->CreateServer(Config))
        {
            UE_LOG(LogTemp, Log, TEXT("Server created successfully"));

            // 創建大廳
            if (LobbySystem)
            {
                FLobbyConfig LobbyConfig;
                LobbyConfig.LobbyName = TEXT("My Game");
                LobbyConfig.MaxPlayers = 8;
                LobbyConfig.GameMode = EMingNetworkGameMode::Cooperative;
                LobbySystem->CreateLobby(LobbyConfig);
            }

            ShowLobbyInterface();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Failed to create server"));
        }
    }
}

void UMingMultiplayerWidget::OnJoinServerClicked()
{
    ShowConnectionDialog();
}

void UMingMultiplayerWidget::OnQuickMatchClicked()
{
    // 快速匹配功能
    UE_LOG(LogTemp, Log, TEXT("Quick match requested"));
    UpdateConnectionStatus(TEXT("Searching for games..."));
}

void UMingMultiplayerWidget::OnSettingsClicked()
{
    // 顯示設置界面
    UE_LOG(LogTemp, Log, TEXT("Settings clicked"));
}

void UMingMultiplayerWidget::OnConnectClicked()
{
    if (!NetworkManager)
    {
        return;
    }

    FString ServerAddress = TEXT("127.0.0.1");
    int32 Port = 7777;

    // 從輸入框獲取地址和端口
    if (ServerAddressInput)
    {
        ServerAddress = ServerAddressInput->GetText().ToString();
    }

    if (PortInput)
    {
        Port = FCString::Atoi(*PortInput->GetText().ToString());
        if (Port <= 0) Port = 7777;
    }

    UpdateConnectionStatus(FString::Printf(TEXT("Connecting to %s:%d..."), *ServerAddress, Port));

    if (NetworkManager->ConnectToServer(ServerAddress, Port))
    {
        UE_LOG(LogTemp, Log, TEXT("Connection initiated"));
    }
    else
    {
        UpdateConnectionStatus(TEXT("Connection failed"));
    }
}

void UMingMultiplayerWidget::OnDisconnectClicked()
{
    if (NetworkManager)
    {
        NetworkManager->DisconnectFromServer();
    }

    if (LobbySystem)
    {
        LobbySystem->LeaveLobby();
    }

    ShowMainMenu();
}

void UMingMultiplayerWidget::OnCancelConnectionClicked()
{
    ShowMainMenu();
}

void UMingMultiplayerWidget::UpdateConnectionStatus(const FString& Status)
{
    if (ConnectionStatusText)
    {
        ConnectionStatusText->SetText(FText::FromString(Status));
    }

    UE_LOG(LogTemp, Log, TEXT("Connection status: %s"), *Status);
}

void UMingMultiplayerWidget::OnLeaveLobbyClicked()
{
    if (LobbySystem)
    {
        LobbySystem->LeaveLobby();
    }

    if (NetworkManager)
    {
        NetworkManager->DisconnectFromServer();
    }

    ShowMainMenu();
}

void UMingMultiplayerWidget::OnReadyClicked()
{
    if (LobbySystem)
    {
        // 切換準備狀態
        bool bCurrentReady = LobbySystem->IsPlayerReady(NetworkManager->GetLocalPlayerID());
        LobbySystem->SetPlayerReady(!bCurrentReady);
    }
}

void UMingMultiplayerWidget::OnStartGameClicked()
{
    if (LobbySystem && LobbySystem->IsHost())
    {
        if (LobbySystem->CanStartGame())
        {
            LobbySystem->StartGame();
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Cannot start game - not all players ready"));
        }
    }
}

void UMingMultiplayerWidget::OnCancelStartClicked()
{
    if (LobbySystem && LobbySystem->IsHost())
    {
        LobbySystem->CancelGameStart();
    }
    HideCountdown();
}

void UMingMultiplayerWidget::OnChangeTeamClicked(int32 TeamID)
{
    if (LobbySystem && NetworkManager)
    {
        LobbySystem->SetPlayerTeam(NetworkManager->GetLocalPlayerID(), TeamID);
    }
}

void UMingMultiplayerWidget::OnKickPlayerClicked(int32 PlayerID)
{
    // 踢出玩家（僅主機可用）
    UE_LOG(LogTemp, Log, TEXT("Kick player requested: %d"), PlayerID);
}

void UMingMultiplayerWidget::OnSendMessageClicked()
{
    if (!ChatInput || !LobbySystem)
    {
        return;
    }

    FString Message = ChatInput->GetText().ToString();
    if (!Message.IsEmpty())
    {
        LobbySystem->SendChatMessage(Message);
        ChatInput->SetText(FText::GetEmpty());
    }
}

void UMingMultiplayerWidget::AddChatMessage(const FString& PlayerName, const FString& Message, bool bIsSystemMessage)
{
    FString FormattedMessage;
    if (bIsSystemMessage)
    {
        FormattedMessage = FString::Printf(TEXT("[System] %s"), *Message);
    }
    else
    {
        FormattedMessage = FString::Printf(TEXT("[%s]: %s"), *PlayerName, *Message);
    }

    // 添加到聊天顯示
    UE_LOG(LogTemp, Log, TEXT("Chat: %s"), *FormattedMessage);

    // 實際實現會創建聊天消息UI元素並添加到ScrollBox
}

void UMingMultiplayerWidget::ClearChat()
{
    if (ChatScrollBox)
    {
        ChatScrollBox->ClearChildren();
    }
}

void UMingMultiplayerWidget::UpdatePlayerList()
{
    if (!LobbySystem || !PlayerListView)
    {
        return;
    }

    // 清空列表
    PlayerListView->ClearListItems();

    // 添加所有玩家
    TArray<FLobbyPlayer> Players = LobbySystem->GetAllPlayers();
    for (const FLobbyPlayer& Player : Players)
    {
        AddPlayerToList(Player.PlayerID, Player.PlayerName, Player.bIsReady, Player.bIsHost, Player.TeamID);
    }
}

void UMingMultiplayerWidget::AddPlayerToList(int32 PlayerID, const FString& PlayerName, bool bIsReady, bool bIsHost, int32 TeamID)
{
    // 實際實現會創建玩家列表項UI元素
    UE_LOG(LogTemp, Log, TEXT("Player added to list: %s (ID: %d, Ready: %s, Host: %s, Team: %d)"),
        *PlayerName, PlayerID, bIsReady ? TEXT("Yes") : TEXT("No"), bIsHost ? TEXT("Yes") : TEXT("No"), TeamID);
}

void UMingMultiplayerWidget::RemovePlayerFromList(int32 PlayerID)
{
    // 實際實現會從列表中移除玩家項
    UE_LOG(LogTemp, Log, TEXT("Player removed from list: %d"), PlayerID);
}

void UMingMultiplayerWidget::UpdatePlayerStatus(int32 PlayerID, bool bIsReady)
{
    // 實際實現會更新玩家列表項的狀態
    UE_LOG(LogTemp, Log, TEXT("Player %d status updated: Ready = %s"), PlayerID, bIsReady ? TEXT("Yes") : TEXT("No"));
}

void UMingMultiplayerWidget::UpdateNetworkStatusDisplay()
{
    if (!NetworkManager)
    {
        return;
    }

    EMingNetworkConnectionState State = NetworkManager->GetConnectionState();
    FString StatusText;

    switch (State)
    {
    case EMingNetworkConnectionState::Disconnected:
        StatusText = TEXT("Disconnected");
        break;
    case EMingNetworkConnectionState::Connecting:
        StatusText = TEXT("Connecting...");
        break;
    case EMingNetworkConnectionState::Connected:
        StatusText = TEXT("Connected");
        break;
    case EMingNetworkConnectionState::ConnectionFailed:
        StatusText = TEXT("Connection Failed");
        break;
    case EMingNetworkConnectionState::Reconnecting:
        StatusText = TEXT("Reconnecting...");
        break;
    default:
        StatusText = TEXT("Unknown");
        break;
    }

    UpdateConnectionStatus(StatusText);
}

void UMingMultiplayerWidget::UpdateLobbyInfoDisplay()
{
    if (!LobbySystem)
    {
        return;
    }

    if (LobbyNameText)
    {
        LobbyNameText->SetText(FText::FromString(LobbySystem->GetLobbyName()));
    }

    if (PlayerCountText)
    {
        FString CountText = FString::Printf(TEXT("Players: %d"), LobbySystem->GetPlayerCount());
        PlayerCountText->SetText(FText::FromString(CountText));
    }

    UpdatePlayerList();
}

void UMingMultiplayerWidget::ShowCountdown(int32 Seconds)
{
    if (CountdownText)
    {
        CountdownText->SetVisibility(ESlateVisibility::Visible);
        CountdownText->SetText(FText::FromString(FString::Printf(TEXT("Game starting in: %d"), Seconds)));
    }
}

void UMingMultiplayerWidget::HideCountdown()
{
    if (CountdownText)
    {
        CountdownText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UMingMultiplayerWidget::RefreshDisplay()
{
    // 根據當前視圖刷新顯示
    if (CurrentView == TEXT("Lobby"))
    {
        UpdateLobbyInfoDisplay();
    }
}

// === 事件處理 ===

void UMingMultiplayerWidget::OnNetworkConnected()
{
    UE_LOG(LogTemp, Log, TEXT("Network connected - updating UI"));
    UpdateNetworkStatusDisplay();
}

void UMingMultiplayerWidget::OnNetworkDisconnected(const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("Network disconnected: %s"), *Reason);
    UpdateConnectionStatus(FString::Printf(TEXT("Disconnected: %s"), *Reason));
    ShowMainMenu();
}

void UMingMultiplayerWidget::OnPlayerJoined(int32 PlayerID, const FString& PlayerName)
{
    UE_LOG(LogTemp, Log, TEXT("Player joined: %s (ID: %d)"), *PlayerName, PlayerID);
    AddChatMessage(TEXT("System"), FString::Printf(TEXT("%s joined the lobby"), *PlayerName), true);
    UpdatePlayerList();
}

void UMingMultiplayerWidget::OnPlayerLeft(int32 PlayerID, const FString& Reason)
{
    UE_LOG(LogTemp, Log, TEXT("Player left: %d, Reason: %s"), PlayerID, *Reason);
    AddChatMessage(TEXT("System"), FString::Printf(TEXT("Player %d left (%s)"), PlayerID, *Reason), true);
    RemovePlayerFromList(PlayerID);
}

void UMingMultiplayerWidget::OnPlayerReadyChanged(int32 PlayerID, bool bReady)
{
    UE_LOG(LogTemp, Log, TEXT("Player %d ready state: %s"), PlayerID, bReady ? TEXT("Ready") : TEXT("Not Ready"));
    UpdatePlayerStatus(PlayerID, bReady);
}

void UMingMultiplayerWidget::OnGameStarting(int32 CountdownSeconds)
{
    UE_LOG(LogTemp, Log, TEXT("Game starting in %d seconds"), CountdownSeconds);
    ShowCountdown(CountdownSeconds);
}

void UMingMultiplayerWidget::OnChatMessageReceived(const FString& PlayerName, const FString& Message, bool bIsSystemMessage)
{
    AddChatMessage(PlayerName, Message, bIsSystemMessage);
}
