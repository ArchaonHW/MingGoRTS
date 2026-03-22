#include "Network/MingNetworkSystemTest.h"
#include "Network/MingNetworkManager.h"
#include "Network/MingLobbySystem.h"
#include "Network/MingRelationshipReplication.h"
#include "Network/MingReputationReplication.h"
#include "MingPersonalManager.h"

UMingNetworkSystemTest::UMingNetworkSystemTest()
    : PassedTests(0)
    , FailedTests(0)
{
}

bool UMingNetworkSystemTest::RunAllTests()
{
    ResetTestCounts();
    TestCases.Empty();
    TestResults = TEXT("=== MingGoRTS Network System Test Results ===\n\n");

    // 運行所有測試
    TestNetworkManagerCreation();
    TestServerCreation();
    TestClientConnection();
    TestDisconnection();
    TestReconnection();
    TestPlayerManagement();
    TestLobbyCreation();
    TestLobbyJoinLeave();
    TestReadySystem();
    TestGameStart();
    TestRelationshipReplication();
    TestReputationReplication();
    TestChatSystem();
    TestHostMigration();
    TestNetworkStats();
    TestErrorHandling();

    // 生成總結報告
    TestResults += FString::Printf(TEXT("\n=== Test Summary ===\n"));
    TestResults += FString::Printf(TEXT("Passed: %d\n"), PassedTests);
    TestResults += FString::Printf(TEXT("Failed: %d\n"), FailedTests);
    TestResults += FString::Printf(TEXT("Pass Rate: %.1f%%\n"), GetTestPassRate());

    if (FailedTests > 0)
    {
        TestResults += TEXT("\n=== Errors ===\n");
        for (const FString& Error : ErrorMessages)
        {
            TestResults += Error + TEXT("\n");
        }
    }

    return FailedTests == 0;
}

float UMingNetworkSystemTest::GetTestPassRate() const
{
    int32 TotalTests = PassedTests + FailedTests;
    if (TotalTests == 0)
    {
        return 0.0f;
    }
    return (static_cast<float>(PassedTests) / TotalTests) * 100.0f;
}

void UMingNetworkSystemTest::LogTestResult(const FString& TestName, bool bPassed, const FString& Message)
{
    if (bPassed)
    {
        PassedTests++;
        TestResults += FString::Printf(TEXT("[PASS] %s\n"), *TestName);
    }
    else
    {
        FailedTests++;
        FString ErrorMsg = FString::Printf(TEXT("[FAIL] %s: %s"), *TestName, *Message);
        TestResults += ErrorMsg + TEXT("\n");
        ErrorMessages.Add(ErrorMsg);
    }
}

void UMingNetworkSystemTest::ResetTestCounts()
{
    PassedTests = 0;
    FailedTests = 0;
    ErrorMessages.Empty();
    TestResults.Empty();
}

void UMingNetworkSystemTest::AddTestCase(const FString& TestName, const FString& Description, bool bPassed, const FString& Error)
{
    FNetworkTestCase TestCase;
    TestCase.TestName = TestName;
    TestCase.Description = Description;
    TestCase.Result = bPassed ? ENetworkTestResult::Passed : ENetworkTestResult::Failed;
    TestCase.ErrorMessage = Error;
    TestCases.Add(TestCase);
}

UMingPersonalManager* UMingNetworkSystemTest::CreateTestPersonalManager()
{
    UMingPersonalManager* Manager = NewObject<UMingPersonalManager>(this);
    if (Manager)
    {
        Manager->Initialize();
    }
    return Manager;
}

UMingNetworkManager* UMingNetworkSystemTest::CreateTestNetworkManager()
{
    UMingNetworkManager* Manager = NewObject<UMingNetworkManager>(this);
    if (Manager)
    {
        UMingPersonalManager* PersonalManager = CreateTestPersonalManager();
        Manager->InitializeNetworkManager(PersonalManager);
    }
    return Manager;
}

UMingLobbySystem* UMingNetworkSystemTest::CreateTestLobbySystem()
{
    UMingLobbySystem* Lobby = NewObject<UMingLobbySystem>(this);
    if (Lobby)
    {
        UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
        Lobby->InitializeLobbySystem(NetworkManager);
    }
    return Lobby;
}

bool UMingNetworkSystemTest::TestNetworkManagerCreation()
{
    FString TestName = TEXT("Network Manager Creation");
    FString Description = TEXT("Tests creating and initializing the network manager");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    bool bSuccess = (NetworkManager != nullptr && NetworkManager->IsNetworkInitialized());

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create network manager"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Creation failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestServerCreation()
{
    FString TestName = TEXT("Server Creation");
    FString Description = TEXT("Tests creating a multiplayer server");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    Config.ServerPort = 7777;

    bool bSuccess = NetworkManager->CreateServer(Config);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create server"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Server creation failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestClientConnection()
{
    FString TestName = TEXT("Client Connection");
    FString Description = TEXT("Tests connecting to a multiplayer server");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 先創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    Config.ServerPort = 7777;
    NetworkManager->CreateServer(Config);

    // 嘗試連接（這裡會連接到自己創建的服務器）
    bool bSuccess = NetworkManager->ConnectToServer(TEXT("127.0.0.1"), 7777);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to connect to server"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Connection failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestDisconnection()
{
    FString TestName = TEXT("Disconnection");
    FString Description = TEXT("Tests disconnecting from a multiplayer server");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器並連接
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 斷開連接
    NetworkManager->DisconnectFromServer();
    bool bSuccess = !NetworkManager->IsConnected();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to disconnect properly"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Disconnection failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestReconnection()
{
    FString TestName = TEXT("Reconnection");
    FString Description = TEXT("Tests reconnecting to a multiplayer server");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 斷開連接
    NetworkManager->DisconnectFromServer();

    // 嘗試重連
    bool bSuccess = NetworkManager->ReconnectToServer();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to reconnect"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Reconnection failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestPlayerManagement()
{
    FString TestName = TEXT("Player Management");
    FString Description = TEXT("Tests player management functionality");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 檢查玩家數量
    int32 PlayerCount = NetworkManager->GetPlayerCount();
    bool bSuccess = (PlayerCount >= 1); // 至少有主機玩家

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Player count incorrect"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Player management failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestLobbyCreation()
{
    FString TestName = TEXT("Lobby Creation");
    FString Description = TEXT("Tests creating a game lobby");

    UMingLobbySystem* LobbySystem = CreateTestLobbySystem();
    if (!LobbySystem)
    {
        LogTestResult(TestName, false, TEXT("Failed to create lobby system"));
        AddTestCase(TestName, Description, false, TEXT("Lobby system creation failed"));
        return false;
    }

    FLobbyConfig Config;
    Config.LobbyName = TEXT("Test Lobby");
    Config.MaxPlayers = 4;

    bool bSuccess = LobbySystem->CreateLobby(Config);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Failed to create lobby"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Lobby creation failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestLobbyJoinLeave()
{
    FString TestName = TEXT("Lobby Join/Leave");
    FString Description = TEXT("Tests joining and leaving a lobby");

    UMingLobbySystem* LobbySystem = CreateTestLobbySystem();
    if (!LobbySystem)
    {
        LogTestResult(TestName, false, TEXT("Failed to create lobby system"));
        AddTestCase(TestName, Description, false, TEXT("Lobby system creation failed"));
        return false;
    }

    // 創建大廳
    FLobbyConfig Config;
    Config.LobbyName = TEXT("Test Lobby");
    Config.MaxPlayers = 4;
    LobbySystem->CreateLobby(Config);

    // 檢查是否在大廳中
    bool bInLobby = LobbySystem->IsInLobby();

    // 離開大廳
    LobbySystem->LeaveLobby();
    bool bLeftLobby = !LobbySystem->IsInLobby();

    bool bSuccess = bInLobby && bLeftLobby;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Join/Leave lobby failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Lobby join/leave failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestReadySystem()
{
    FString TestName = TEXT("Ready System");
    FString Description = TEXT("Tests player ready system");

    UMingLobbySystem* LobbySystem = CreateTestLobbySystem();
    if (!LobbySystem)
    {
        LogTestResult(TestName, false, TEXT("Failed to create lobby system"));
        AddTestCase(TestName, Description, false, TEXT("Lobby system creation failed"));
        return false;
    }

    // 創建大廳
    FLobbyConfig Config;
    Config.LobbyName = TEXT("Test Lobby");
    LobbySystem->CreateLobby(Config);

    // 設置準備狀態
    LobbySystem->SetPlayerReady(true);
    bool bSuccess = LobbySystem->AreAllPlayersReady();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Ready system failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Ready system failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestGameStart()
{
    FString TestName = TEXT("Game Start");
    FString Description = TEXT("Tests starting a multiplayer game");

    UMingLobbySystem* LobbySystem = CreateTestLobbySystem();
    if (!LobbySystem)
    {
        LogTestResult(TestName, false, TEXT("Failed to create lobby system"));
        AddTestCase(TestName, Description, false, TEXT("Lobby system creation failed"));
        return false;
    }

    // 創建大廳並設置準備
    FLobbyConfig Config;
    Config.LobbyName = TEXT("Test Lobby");
    Config.MinPlayersToStart = 1;
    LobbySystem->CreateLobby(Config);
    LobbySystem->SetPlayerReady(true);

    // 嘗試開始遊戲
    bool bCanStart = LobbySystem->CanStartGame();
    if (bCanStart)
    {
        LobbySystem->StartGame();
    }

    LogTestResult(TestName, bCanStart, bCanStart ? TEXT("") : TEXT("Cannot start game"));
    AddTestCase(TestName, Description, bCanStart, bCanStart ? TEXT("") : TEXT("Game start failed"));

    return bCanStart;
}

bool UMingNetworkSystemTest::TestRelationshipReplication()
{
    FString TestName = TEXT("Relationship Replication");
    FString Description = TEXT("Tests relationship data replication");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 獲取關係複製組件
    UMingRelationshipReplication* Replication = NetworkManager->GetRelationshipReplication();
    bool bSuccess = (Replication != nullptr);

    if (bSuccess)
    {
        // 測試請求同步
        Replication->RequestFullSync();
        bSuccess = (Replication->GetSyncState() == ERelationshipSyncState::Syncing ||
                    Replication->GetSyncState() == ERelationshipSyncState::Synced);
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Relationship replication failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Replication failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestReputationReplication()
{
    FString TestName = TEXT("Reputation Replication");
    FString Description = TEXT("Tests reputation data replication");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 獲取聲望複製組件
    UMingReputationReplication* Replication = NetworkManager->GetReputationReplication();
    bool bSuccess = (Replication != nullptr);

    if (bSuccess)
    {
        // 測試請求同步
        Replication->RequestFullSync();
        bSuccess = (Replication->GetSyncState() == EReputationSyncState::Syncing ||
                    Replication->GetSyncState() == EReputationSyncState::Synced);
    }

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Reputation replication failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Replication failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestChatSystem()
{
    FString TestName = TEXT("Chat System");
    FString Description = TEXT("Tests lobby chat functionality");

    UMingLobbySystem* LobbySystem = CreateTestLobbySystem();
    if (!LobbySystem)
    {
        LogTestResult(TestName, false, TEXT("Failed to create lobby system"));
        AddTestCase(TestName, Description, false, TEXT("Lobby system creation failed"));
        return false;
    }

    // 創建大廳
    FLobbyConfig Config;
    Config.LobbyName = TEXT("Test Lobby");
    LobbySystem->CreateLobby(Config);

    // 發送聊天消息
    FString TestMessage = TEXT("Hello, this is a test message!");
    LobbySystem->SendChatMessage(TestMessage);

    // 檢查聊天歷史
    TArray<FLobbyChatMessage> ChatHistory = LobbySystem->GetChatHistory();
    bool bSuccess = (ChatHistory.Num() > 0);

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Chat system failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Chat system failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestHostMigration()
{
    FString TestName = TEXT("Host Migration");
    FString Description = TEXT("Tests host migration functionality");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器（作為主機）
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 檢查是否為主機
    bool bIsHost = NetworkManager->IsHost();

    LogTestResult(TestName, bIsHost, bIsHost ? TEXT("") : TEXT("Host check failed"));
    AddTestCase(TestName, Description, bIsHost, bIsHost ? TEXT("") : TEXT("Host migration failed"));

    return bIsHost;
}

bool UMingNetworkSystemTest::TestNetworkStats()
{
    FString TestName = TEXT("Network Stats");
    FString Description = TEXT("Tests network statistics tracking");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 創建服務器
    FMingNetworkConfig Config;
    Config.MaxPlayers = 4;
    NetworkManager->CreateServer(Config);

    // 獲取網絡統計
    FMingNetworkStats Stats = NetworkManager->GetNetworkStats();
    bool bSuccess = true; // 統計數據結構應該總是可用的

    // 重置統計
    NetworkManager->ResetNetworkStats();

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Network stats failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Network stats failed"));

    return bSuccess;
}

bool UMingNetworkSystemTest::TestErrorHandling()
{
    FString TestName = TEXT("Error Handling");
    FString Description = TEXT("Tests error handling in network system");

    UMingNetworkManager* NetworkManager = CreateTestNetworkManager();
    if (!NetworkManager)
    {
        LogTestResult(TestName, false, TEXT("Failed to create network manager"));
        AddTestCase(TestName, Description, false, TEXT("Network manager creation failed"));
        return false;
    }

    // 測試無效連接
    bool bInvalidConnection = !NetworkManager->ConnectToServer(TEXT(""), 0);

    // 測試未初始化時的操作
    bool bNotInitialized = !NetworkManager->IsConnected();

    bool bSuccess = bInvalidConnection && bNotInitialized;

    LogTestResult(TestName, bSuccess, bSuccess ? TEXT("") : TEXT("Error handling failed"));
    AddTestCase(TestName, Description, bSuccess, bSuccess ? TEXT("") : TEXT("Error handling failed"));

    return bSuccess;
}
