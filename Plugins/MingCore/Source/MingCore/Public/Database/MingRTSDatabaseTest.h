// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDatabaseTest.generated.h"

UENUM(BlueprintType)
enum class EDatabaseTestCategory : uint8
{
	BasicOperations,
	SchemaManagement,
	QueryOperations,
	TransactionHandling,
	PerformanceBenchmark,
	DataIntegrity,
	Concurrency,
	Serialization,
	BackupRestore,
	ManagerOperations
};

USTRUCT(BlueprintType)
struct FDatabaseTestResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	FString TestName;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	bool bPassed;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	FString ErrorMessage;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	float ExecutionTimeMs;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	EDatabaseTestCategory Category;

	FDatabaseTestResult()
		: bPassed(false)
		, ExecutionTimeMs(0.0f)
	{
	}
};

USTRUCT(BlueprintType)
struct FDatabaseTestSummary
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	int32 TotalTests;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	int32 PassedTests;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	int32 FailedTests;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	float TotalExecutionTimeMs;

	UPROPERTY(BlueprintReadOnly, Category = "DatabaseTest")
	TArray<FDatabaseTestResult> Results;

	FDatabaseTestSummary()
		: TotalTests(0)
		, PassedTests(0)
		, FailedTests(0)
		, TotalExecutionTimeMs(0.0f)
	{
	}
};

UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSDatabaseTest : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DatabaseTest")
	void InitializeTest();

	UFUNCTION(BlueprintCallable, Category = "DatabaseTest")
	FDatabaseTestSummary RunAllTests();

	UFUNCTION(BlueprintCallable, Category = "DatabaseTest")
	TArray<FDatabaseTestResult> RunTestCategory(EDatabaseTestCategory Category);

	UFUNCTION(BlueprintCallable, Category = "DatabaseTest")
	FDatabaseTestResult RunSingleTest(const FString& TestName);

protected:
	UFUNCTION()
	FDatabaseTestResult Test_DatabaseInitialization();

	UFUNCTION()
	FDatabaseTestResult Test_CreateTable();

	UFUNCTION()
	FDatabaseTestResult Test_InsertRow();

	UFUNCTION()
	FDatabaseTestResult Test_UpdateRow();

	UFUNCTION()
	FDatabaseTestResult Test_DeleteRow();

	UFUNCTION()
	FDatabaseTestResult Test_QueryRows();

	UFUNCTION()
	FDatabaseTestResult Test_SaveLoadDatabase();

	UFUNCTION()
	FDatabaseTestResult Test_ExportImportJSON();

	UFUNCTION()
	FDatabaseTestResult Test_BackupRestore();

	UFUNCTION()
	FDatabaseTestResult Test_DataTypes();

	UFUNCTION()
	FDatabaseTestResult Test_PrimaryKeyConstraint();

	UFUNCTION()
	FDatabaseTestResult Test_NullableConstraint();

	UFUNCTION()
	FDatabaseTestResult Test_IndexCreation();

	UFUNCTION()
	FDatabaseTestResult Test_ManagerCreateDatabase();

	UFUNCTION()
	FDatabaseTestResult Test_SnapshotCreation();

	UFUNCTION()
	FDatabaseTestResult Test_BatchOperations();

	UFUNCTION()
	FDatabaseTestResult Test_ConcurrentAccess();

	UFUNCTION()
	FDatabaseTestResult Test_PerformanceInsert();

	UFUNCTION()
	FDatabaseTestResult Test_PerformanceQuery();

	UFUNCTION()
	FDatabaseTestResult Test_IntegrityValidation();

	UFUNCTION()
	FDatabaseTestResult Test_TableOptimization();

protected:
	UPROPERTY()
	TObjectPtr<UMingRTSDynamicDatabase> TestDatabase;

	UPROPERTY()
	FString TestDatabaseName;

	void CleanupTestData();
	void LogTestResult(const FDatabaseTestResult& Result);
};
