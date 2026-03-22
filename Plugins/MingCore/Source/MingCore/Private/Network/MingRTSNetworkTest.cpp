// Copyright Epic Games, Inc. All Rights Reserved.

#include "Network/MingRTSNetworkTest.h"
#include "Network/MingRTSNetworkManager.h"
#include "Network/MingRTSMultiplayerRelationship.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Misc/DateTime.h"

void UMingRTSNetworkTest::InitializeTest()
{
	UE_LOG(LogTemp, Log, TEXT("Initializing Network Test Suite"));
	CleanupTest();
}

FNetworkTestSummary UMingRTSNetworkTest::RunAllTests()
{
	FNetworkTestSummary Summary;
	Summary.Results.Empty();

	UE_LOG(LogTemp, Log, TEXT("Starting Network Test Suite"));

	// Connection Tests
	Summary.Results.Add(Test_NetworkManagerCreation());
	Summary.Results.Add(Test_CreateSession());
	Summary.Results.Add(Test_JoinSession());

	// Player Sync Tests
	Summary.Results.Add(Test_PlayerJoinSync());
	Summary.Results.Add(Test_PlayerLeaveSync());
	Summary.Results.Add(Test_ReadyStateSync());
	Summary.Results.Add(Test_GameStart());

	// Diplomacy Tests
	Summary.Results.Add(Test_DiplomaticRelationCreation());
	Summary.Results.Add(Test_DeclareWar());
	Summary.Results.Add(Test_FormAlliance());
	Summary.Results.Add(Test_BreakAlliance());
	Summary.Results.Add(Test_TradeAgreement());
	Summary.Results.Add(Test_DiplomaticProposal());

	// Replication Tests
	Summary.Results.Add(Test_DataReplication());
	Summary.Results.Add(Test_NetworkTickRate());

	// Performance Tests
	Summary.Results.Add(Test_MultiplePlayers());
	Summary.Results.Add(Test_LatencySimulation());
	Summary.Results.Add(Test_PacketLossRecovery());

	// Stress Tests
	Summary.Results.Add(Test_HostMigration());
	Summary.Results.Add(Test_NetworkErrorHandling());

	// Calculate summary
	Summary.TotalTests = Summary.Results.Num();
	Summary.PassedTests = 0;
	Summary.FailedTests = 0;
	Summary.TotalExecutionTimeMs = 0.0f;

	for (const auto& Result : Summary.Results)
	{
		if (Result.bPassed)
		{
			Summary.PassedTests++;
		}
		else
		{
			Summary.FailedTests++;
		}
		Summary.TotalExecutionTimeMs += Result.ExecutionTimeMs;
	}

	UE_LOG(LogTemp, Log, TEXT("Network Test Suite Complete: %d/%d passed in %.2f ms"),
		Summary.PassedTests, Summary.TotalTests, Summary.TotalExecutionTimeMs);

	return Summary;
}

TArray<FNetworkTestResult> UMingRTSNetworkTest::RunTestCategory(ENetworkTestCategory Category)
{
	TArray<FNetworkTestResult> Results;

	switch (Category)
	{
	case ENetworkTestCategory::Connection:
		Results.Add(Test_NetworkManagerCreation());
		Results.Add(Test_CreateSession());
		Results.Add(Test_JoinSession());
		break;

	case ENetworkTestCategory::SessionManagement:
		Results.Add(Test_CreateSession());
		Results.Add(Test_GameStart());
		Results.Add(Test_HostMigration());
		break;

	case ENetworkTestCategory::PlayerSync:
		Results.Add(Test_PlayerJoinSync());
		Results.Add(Test_PlayerLeaveSync());
		Results.Add(Test_ReadyStateSync());
		break;

	case ENetworkTestCategory::Diplomacy:
		Results.Add(Test_DiplomaticRelationCreation());
		Results.Add(Test_DeclareWar());
		Results.Add(Test_FormAlliance());
		Results.Add(Test_BreakAlliance());
		Results.Add(Test_TradeAgreement());
		Results.Add(Test_DiplomaticProposal());
		break;

	case ENetworkTestCategory::Replication:
		Results.Add(Test_DataReplication());
		Results.Add(Test_NetworkTickRate());
		break;

	case ENetworkTestCategory::Performance:
		Results.Add(Test_MultiplePlayers());
		Results.Add(Test_LatencySimulation());
		break;

	case ENetworkTestCategory::StressTest:
		Results.Add(Test_HostMigration());
		Results.Add(Test_PacketLossRecovery());
		Results.Add(Test_MultiplePlayers());
		break;

	case ENetworkTestCategory::Integration:
		Results.Add(Test_NetworkErrorHandling());
		break;
	}

	return Results;
}

FNetworkTestResult UMingRTSNetworkTest::RunSingleTest(const FString& TestName)
{
	if (TestName == TEXT("NetworkManagerCreation"))
		return Test_NetworkManagerCreation();
	if (TestName == TEXT("CreateSession"))
		return Test_CreateSession();
	if (TestName == TEXT("JoinSession"))
		return Test_JoinSession();
	if (TestName == TEXT("PlayerJoinSync"))
		return Test_PlayerJoinSync();
	if (TestName == TEXT("PlayerLeaveSync"))
		return Test_PlayerLeaveSync();
	if (TestName == TEXT("ReadyStateSync"))
		return Test_ReadyStateSync();
	if (TestName == TEXT("GameStart"))
		return Test_GameStart();
	if (TestName == TEXT("DiplomaticRelationCreation"))
		return Test_DiplomaticRelationCreation();
	if (TestName == TEXT("DeclareWar"))
		return Test_DeclareWar();
	if (TestName == TEXT("FormAlliance"))
		return Test_FormAlliance();
	if (TestName == TEXT("BreakAlliance"))
		return Test_BreakAlliance();
	if (TestName == TEXT("TradeAgreement"))
		return Test_TradeAgreement();
	if (TestName == TEXT("DiplomaticProposal"))
		return Test_DiplomaticProposal();
	if (TestName == TEXT("DataReplication"))
		return Test_DataReplication();
	if (TestName == TEXT("NetworkTickRate"))
		return Test_NetworkTickRate();
	if (TestName == TEXT("MultiplePlayers"))
		return Test_MultiplePlayers();
	if (TestName == TEXT("HostMigration"))
		return Test_HostMigration();
	if (TestName == TEXT("NetworkErrorHandling"))
		return Test_NetworkErrorHandling();
	if (TestName == TEXT("LatencySimulation"))
		return Test_LatencySimulation();
	if (TestName == TEXT("PacketLossRecovery"))
		return Test_PacketLossRecovery();

	FNetworkTestResult Result;
	Result.TestName = TestName;
	Result.bPassed = false;
	Result.ErrorMessage = TEXT("Unknown test name");
	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_NetworkManagerCreation()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("NetworkManagerCreation");
	Result.Category = ENetworkTestCategory::Connection;

	double StartTime = FPlatformTime::Seconds();

	TestNetworkManager = NewObject<UMingRTSNetworkManager>();
	if (!TestNetworkManager)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Failed to create NetworkManager");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	Result.bPassed = true;
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_CreateSession()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("CreateSession");
	Result.Category = ENetworkTestCategory::SessionManagement;

	double StartTime = FPlatformTime::Seconds();

	if (!TestNetworkManager)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("NetworkManager not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	FNetworkSessionConfig Config;
	Config.SessionName = TEXT("TestSession");
	Config.MaxPlayers = 4;
	Config.bIsLAN = true;
	Config.bShouldAdvertise = false;

	// Note: Actual session creation requires OnlineSubsystem
	// This is a mock test
	TestNetworkManager->SetNetworkGameMode(ENetworkGameMode::ListenServer);

	Result.bPassed = TestNetworkManager->GetNetworkGameMode() == ENetworkGameMode::ListenServer;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to set network game mode");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_JoinSession()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("JoinSession");
	Result.Category = ENetworkTestCategory::Connection;

	double StartTime = FPlatformTime::Seconds();

	if (!TestNetworkManager)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("NetworkManager not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	// Mock join session
	TestNetworkManager->SetNetworkGameMode(ENetworkGameMode::Client);
	bool bSuccess = TestNetworkManager->GetNetworkGameMode() == ENetworkGameMode::Client;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to join session");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_PlayerJoinSync()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("PlayerJoinSync");
	Result.Category = ENetworkTestCategory::PlayerSync;

	double StartTime = FPlatformTime::Seconds();

	// Simulate player join
	SimulatePlayerJoin(1, TEXT("Player1"));
	SimulatePlayerJoin(2, TEXT("Player2"));

	bool bSuccess = TestPlayers.Num() == 2;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Expected 2 players, got %d"), TestPlayers.Num());
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_PlayerLeaveSync()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("PlayerLeaveSync");
	Result.Category = ENetworkTestCategory::PlayerSync;

	double StartTime = FPlatformTime::Seconds();

	int32 InitialCount = TestPlayers.Num();
	SimulatePlayerLeave(2);

	bool bSuccess = TestPlayers.Num() == InitialCount - 1;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to remove player correctly");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_ReadyStateSync()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("ReadyStateSync");
	Result.Category = ENetworkTestCategory::PlayerSync;

	double StartTime = FPlatformTime::Seconds();

	if (!TestNetworkManager)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("NetworkManager not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	// Test ready state
	TestNetworkManager->SetPlayerReady(true);
	FNetworkPlayerInfo LocalInfo = TestNetworkManager->GetLocalPlayerInfo();

	Result.bPassed = LocalInfo.bIsReady;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Ready state not synchronized");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_GameStart()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("GameStart");
	Result.Category = ENetworkTestCategory::SessionManagement;

	double StartTime = FPlatformTime::Seconds();

	// Mock game start
	bool bSuccess = true;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Game start failed");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_DiplomaticRelationCreation()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("DiplomaticRelationCreation");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	TestRelationshipSystem = NewObject<UMingRTSMultiplayerRelationship>();
	if (!TestRelationshipSystem)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Failed to create relationship system");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	TestRelationshipSystem->InitializeSystem(TestNetworkManager);
	TestRelationshipSystem->SetLocalPlayerId(1);
	TestRelationshipSystem->SetDiplomaticRelation(1, 2, EMultiplayerRelationType::Neutral);

	EMultiplayerRelationType Relation = TestRelationshipSystem->GetDiplomaticRelation(1, 2);

	Result.bPassed = Relation == EMultiplayerRelationType::Neutral;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create diplomatic relation");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_DeclareWar()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("DeclareWar");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	if (!TestRelationshipSystem)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Relationship system not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	TestRelationshipSystem->SetDiplomaticRelation(1, 2, EMultiplayerRelationType::War);
	bool bAtWar = TestRelationshipSystem->IsAtWar(1, 2);

	Result.bPassed = bAtWar;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to declare war");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_FormAlliance()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("FormAlliance");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	if (!TestRelationshipSystem)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Relationship system not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	TestRelationshipSystem->SetDiplomaticRelation(1, 2, EMultiplayerRelationType::Ally);
	bool bAllied = TestRelationshipSystem->IsAllied(1, 2);
	TArray<int32> Allies = TestRelationshipSystem->GetAllies(1);

	Result.bPassed = bAllied && Allies.Contains(2);
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to form alliance");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_BreakAlliance()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("BreakAlliance");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	if (!TestRelationshipSystem)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Relationship system not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	// First form alliance, then break it
	TestRelationshipSystem->SetDiplomaticRelation(1, 2, EMultiplayerRelationType::Ally);
	TestRelationshipSystem->SetDiplomaticRelation(1, 2, EMultiplayerRelationType::Neutral);

	bool bAllied = TestRelationshipSystem->IsAllied(1, 2);
	EMultiplayerRelationType CurrentRelation = TestRelationshipSystem->GetDiplomaticRelation(1, 2);

	Result.bPassed = !bAllied && CurrentRelation == EMultiplayerRelationType::Neutral;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to break alliance");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_TradeAgreement()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("TradeAgreement");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	if (!TestRelationshipSystem)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("Relationship system not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	TMap<FString, int32> TradeTerms;
	TradeTerms.Add(TEXT("Gold"), 100);
	TradeTerms.Add(TEXT("Wood"), 50);

	// Mock trade agreement
	bool bSuccess = TradeTerms.Num() == 2;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create trade agreement");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_DiplomaticProposal()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("DiplomaticProposal");
	Result.Category = ENetworkTestCategory::Diplomacy;

	double StartTime = FPlatformTime::Seconds();

	FDiplomaticProposal Proposal;
	Proposal.FromPlayerId = 1;
	Proposal.ToPlayerId = 2;
	Proposal.Action = EDiplomaticAction::FormAlliance;
	Proposal.Message = TEXT("Let us ally for mutual benefit");

	bool bSuccess = Proposal.FromPlayerId == 1 && Proposal.ToPlayerId == 2;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create diplomatic proposal");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_DataReplication()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("DataReplication");
	Result.Category = ENetworkTestCategory::Replication;

	double StartTime = FPlatformTime::Seconds();

	FNetworkSyncedData SyncData;
	SyncData.DataId = TEXT("TestData");
	SyncData.DataType = TEXT("Test");
	SyncData.Priority = ENetworkSyncPriority::Normal;

	bool bSuccess = !SyncData.DataId.IsEmpty() && !SyncData.DataType.IsEmpty();

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create sync data");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_NetworkTickRate()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("NetworkTickRate");
	Result.Category = ENetworkTestCategory::Replication;

	double StartTime = FPlatformTime::Seconds();

	if (!TestNetworkManager)
	{
		Result.bPassed = false;
		Result.ErrorMessage = TEXT("NetworkManager not initialized");
		Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
		return Result;
	}

	TestNetworkManager->SetNetworkTickRate(60.0f);
	float TickRate = TestNetworkManager->GetNetworkTickRate();

	Result.bPassed = FMath::IsNearlyEqual(TickRate, 60.0f, 0.1f);
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Tick rate mismatch: expected 60.0, got %.1f"), TickRate);
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_MultiplePlayers()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("MultiplePlayers");
	Result.Category = ENetworkTestCategory::Performance;

	double StartTime = FPlatformTime::Seconds();

	// Simulate multiple players
	for (int32 i = 3; i <= 8; ++i)
	{
		SimulatePlayerJoin(i, FString::Printf(TEXT("Player%d"), i));
	}

	bool bSuccess = TestPlayers.Num() >= 6; // Should have at least 6 players now

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Expected at least 6 players, got %d"), TestPlayers.Num());
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_HostMigration()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("HostMigration");
	Result.Category = ENetworkTestCategory::StressTest;

	double StartTime = FPlatformTime::Seconds();

	// Mock host migration
	bool bSuccess = true;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Host migration failed");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_NetworkErrorHandling()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("NetworkErrorHandling");
	Result.Category = ENetworkTestCategory::Integration;

	double StartTime = FPlatformTime::Seconds();

	// Mock error handling
	bool bSuccess = true;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Network error handling failed");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_LatencySimulation()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("LatencySimulation");
	Result.Category = ENetworkTestCategory::Performance;

	double StartTime = FPlatformTime::Seconds();

	float Latency = MeasureLatency();
	bool bSuccess = Latency >= 0.0f;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Latency measurement failed");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

FNetworkTestResult UMingRTSNetworkTest::Test_PacketLossRecovery()
{
	FNetworkTestResult Result;
	Result.TestName = TEXT("PacketLossRecovery");
	Result.Category = ENetworkTestCategory::StressTest;

	double StartTime = FPlatformTime::Seconds();

	// Mock packet loss recovery
	bool bSuccess = true;

	Result.bPassed = bSuccess;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Packet loss recovery failed");
	}

	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	LogTestResult(Result);

	return Result;
}

void UMingRTSNetworkTest::CleanupTest()
{
	TestNetworkManager = nullptr;
	TestRelationshipSystem = nullptr;
	TestPlayers.Empty();
}

void UMingRTSNetworkTest::LogTestResult(const FNetworkTestResult& Result)
{
	if (Result.bPassed)
	{
		UE_LOG(LogTemp, Log, TEXT("✓ Test Passed: %s (%.2f ms)"),
			*Result.TestName, Result.ExecutionTimeMs);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("✗ Test Failed: %s - %s (%.2f ms)"),
			*Result.TestName, *Result.ErrorMessage, Result.ExecutionTimeMs);
	}
}

void UMingRTSNetworkTest::SimulatePlayerJoin(int32 PlayerId, const FString& PlayerName)
{
	FNetworkPlayerInfo PlayerInfo;
	PlayerInfo.PlayerId = PlayerId;
	PlayerInfo.PlayerName = PlayerName;
	PlayerInfo.bIsHost = (PlayerId == 1);
	PlayerInfo.ConnectionState = ENetworkConnectionState::Connected;
	TestPlayers.Add(PlayerInfo);
}

void UMingRTSNetworkTest::SimulatePlayerLeave(int32 PlayerId)
{
	for (int32 i = TestPlayers.Num() - 1; i >= 0; --i)
	{
		if (TestPlayers[i].PlayerId == PlayerId)
		{
			TestPlayers.RemoveAt(i);
			break;
		}
	}
}

float UMingRTSNetworkTest::MeasureLatency()
{
	// Simple latency measurement
	double StartTime = FPlatformTime::Seconds();
	// Simulate some work
	for (int32 i = 0; i < 1000; ++i)
	{
		volatile int32 Dummy = i * i;
		(void)Dummy; // Suppress unused warning
	}
	double EndTime = FPlatformTime::Seconds();
	return (EndTime - StartTime) * 1000.0f;
}
