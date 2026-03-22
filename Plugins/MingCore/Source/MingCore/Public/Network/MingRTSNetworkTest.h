// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSNetworkTest.generated.h"

UENUM(BlueprintType)
enum class ENetworkTestCategory: uint8 {
	Connection,
	SessionManagement,
	PlayerSync,
	Diplomacy,
	Replication,
	Performance,
	StressTest,
	Integration
};

USTRUCT(BlueprintType)
struct FNetworkTestResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	FString TestName;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	bool bPassed;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	FString ErrorMessage;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	float ExecutionTimeMs;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	ENetworkTestCategory Category;

	FNetworkTestResult()
		: bPassed(false)
		, ExecutionTimeMs(0.0f)
		, Category(ENetworkTestCategory::Connection)
	{
	}
};

USTRUCT(BlueprintType)
struct FNetworkTestSummary
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	int32 TotalTests;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	int32 PassedTests;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	int32 FailedTests;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	float TotalExecutionTimeMs;

	UPROPERTY(BlueprintReadOnly, Category = "NetworkTest")
	TArray<FNetworkTestResult> Results;

	FNetworkTestSummary()
		: TotalTests(0)
		, PassedTests(0)
		, FailedTests(0)
		, TotalExecutionTimeMs(0.0f)
	{
	}
};

UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSNetworkTest : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "NetworkTest")
	void InitializeTest();

	UFUNCTION(BlueprintCallable, Category = "NetworkTest")
	FNetworkTestSummary RunAllTests();

	UFUNCTION(BlueprintCallable, Category = "NetworkTest")
	TArray<FNetworkTestResult> RunTestCategory(ENetworkTestCategory Category);

	UFUNCTION(BlueprintCallable, Category = "NetworkTest")
	FNetworkTestResult RunSingleTest(const FString& TestName);

protected:
	UFUNCTION()
	FNetworkTestResult Test_NetworkManagerCreation();

	UFUNCTION()
	FNetworkTestResult Test_CreateSession();

	UFUNCTION()
	FNetworkTestResult Test_JoinSession();

	UFUNCTION()
	FNetworkTestResult Test_PlayerJoinSync();

	UFUNCTION()
	FNetworkTestResult Test_PlayerLeaveSync();

	UFUNCTION()
	FNetworkTestResult Test_ReadyStateSync();

	UFUNCTION()
	FNetworkTestResult Test_GameStart();

	UFUNCTION()
	FNetworkTestResult Test_DiplomaticRelationCreation();

	UFUNCTION()
	FNetworkTestResult Test_DeclareWar();

	UFUNCTION()
	FNetworkTestResult Test_FormAlliance();

	UFUNCTION()
	FNetworkTestResult Test_BreakAlliance();

	UFUNCTION()
	FNetworkTestResult Test_TradeAgreement();

	UFUNCTION()
	FNetworkTestResult Test_DiplomaticProposal();

	UFUNCTION()
	FNetworkTestResult Test_DataReplication();

	UFUNCTION()
	FNetworkTestResult Test_NetworkTickRate();

	UFUNCTION()
	FNetworkTestResult Test_MultiplePlayers();

	UFUNCTION()
	FNetworkTestResult Test_HostMigration();

	UFUNCTION()
	FNetworkTestResult Test_NetworkErrorHandling();

	UFUNCTION()
	FNetworkTestResult Test_LatencySimulation();

	UFUNCTION()
	FNetworkTestResult Test_PacketLossRecovery();

protected:
	UPROPERTY()
	TObjectPtr<UMingRTSNetworkManager> TestNetworkManager;

	UPROPERTY()
	TObjectPtr<UMingRTSMultiplayerRelationship> TestRelationshipSystem;

	UPROPERTY()
	TArray<FNetworkPlayerInfo> TestPlayers;

	void CleanupTest();
	void LogTestResult(const FNetworkTestResult& Result);
	void SimulatePlayerJoin(int32 PlayerId, const FString& PlayerName);
	void SimulatePlayerLeave(int32 PlayerId);
	float MeasureLatency();
};
