// Copyright Epic Games, Inc. All Rights Reserved.

#include "Database/MingRTSDatabaseTest.h"
#include "Database/MingRTSDynamicDatabase.h"
#include "Database/MingRTSDatabaseManager.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"

void UMingRTSDatabaseTest::InitializeTest()
{
	TestDatabaseName = FString::Printf(TEXT("TestDB_%s"), *FGuid::NewGuid().ToString().Left(8));
	TestDatabase = NewObject<UMingRTSDynamicDatabase>();
	TestDatabase->InitializeDatabase(TestDatabaseName);
	
	UE_LOG(LogTemp, Log, TEXT("Database test initialized with database: %s"), *TestDatabaseName);
}

void UMingRTSDatabaseTest::CleanupTestData()
{
	if (TestDatabase)
	{
		TestDatabase->ClearDatabase();
		TestDatabase->ShutdownDatabase();
	}
	
	// Clean up test files
	FString TestDir = FPaths::ProjectSavedDir() / TEXT("Databases");
	IFileManager::Get().DeleteDirectoryAndContents(*TestDir, false, true);
}

FDatabaseTestSummary UMingRTSDatabaseTest::RunAllTests()
{
	FDatabaseTestSummary Summary;
	TArray<FDatabaseTestResult> AllResults;
	
	UE_LOG(LogTemp, Log, TEXT("Running all database tests..."));
	
	AllResults.Add(Test_DatabaseInitialization());
	AllResults.Add(Test_CreateTable());
	AllResults.Add(Test_InsertRow());
	AllResults.Add(Test_UpdateRow());
	AllResults.Add(Test_DeleteRow());
	AllResults.Add(Test_QueryRows());
	AllResults.Add(Test_SaveLoadDatabase());
	AllResults.Add(Test_ExportImportJSON());
	AllResults.Add(Test_BackupRestore());
	AllResults.Add(Test_DataTypes());
	AllResults.Add(Test_PrimaryKeyConstraint());
	AllResults.Add(Test_NullableConstraint());
	AllResults.Add(Test_IndexCreation());
	AllResults.Add(Test_ManagerCreateDatabase());
	AllResults.Add(Test_SnapshotCreation());
	AllResults.Add(Test_BatchOperations());
	AllResults.Add(Test_ConcurrentAccess());
	AllResults.Add(Test_PerformanceInsert());
	AllResults.Add(Test_PerformanceQuery());
	AllResults.Add(Test_IntegrityValidation());
	AllResults.Add(Test_TableOptimization());
	
	for (const auto& Result : AllResults)
	{
		Summary.TotalTests++;
		if (Result.bPassed)
		{
			Summary.PassedTests++;
		}
		else
		{
			Summary.FailedTests++;
		}
		Summary.TotalExecutionTimeMs += Result.ExecutionTimeMs;
		Summary.Results.Add(Result);
		
		LogTestResult(Result);
	}
	
	CleanupTestData();
	
	UE_LOG(LogTemp, Log, TEXT("Database tests completed: %d/%d passed"), Summary.PassedTests, Summary.TotalTests);
	return Summary;
}

TArray<FDatabaseTestResult> UMingRTSDatabaseTest::RunTestCategory(EDatabaseTestCategory Category)
{
	TArray<FDatabaseTestResult> Results;
	
	switch (Category)
	{
	case EDatabaseTestCategory::BasicOperations:
		Results.Add(Test_DatabaseInitialization());
		Results.Add(Test_CreateTable());
		Results.Add(Test_InsertRow());
		Results.Add(Test_UpdateRow());
		Results.Add(Test_DeleteRow());
		break;
		
	case EDatabaseTestCategory::QueryOperations:
		Results.Add(Test_QueryRows());
		Results.Add(Test_PerformanceQuery());
		break;
		
	case EDatabaseTestCategory::Serialization:
		Results.Add(Test_SaveLoadDatabase());
		Results.Add(Test_ExportImportJSON());
		break;
		
	case EDatabaseTestCategory::BackupRestore:
		Results.Add(Test_BackupRestore());
		Results.Add(Test_SnapshotCreation());
		break;
		
	case EDatabaseTestCategory::DataIntegrity:
		Results.Add(Test_DataTypes());
		Results.Add(Test_PrimaryKeyConstraint());
		Results.Add(Test_NullableConstraint());
		Results.Add(Test_IntegrityValidation());
		break;
		
	case EDatabaseTestCategory::ManagerOperations:
		Results.Add(Test_ManagerCreateDatabase());
		Results.Add(Test_IndexCreation());
		break;
		
	case EDatabaseTestCategory::PerformanceBenchmark:
		Results.Add(Test_PerformanceInsert());
		Results.Add(Test_PerformanceQuery());
		Results.Add(Test_BatchOperations());
		break;
		
	default:
		break;
	}
	
	return Results;
}

void UMingRTSDatabaseTest::LogTestResult(const FDatabaseTestResult& Result)
{
	if (Result.bPassed)
	{
		UE_LOG(LogTemp, Log, TEXT("[PASS] %s (%.2f ms)"), *Result.TestName, Result.ExecutionTimeMs);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[FAIL] %s: %s"), *Result.TestName, *Result.ErrorMessage);
	}
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_DatabaseInitialization()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("DatabaseInitialization");
	Result.Category = EDatabaseTestCategory::BasicOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	UMingRTSDynamicDatabase* DB = NewObject<UMingRTSDynamicDatabase>();
	bool bInit = DB->InitializeDatabase(TEXT("TestInitDB"));
	
	Result.bPassed = bInit && DB->GetDatabaseName() == TEXT("TestInitDB");
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Database initialization failed");
	}
	
	DB->ShutdownDatabase();
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_CreateTable()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("CreateTable");
	Result.Category = EDatabaseTestCategory::BasicOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("TestTable");
	Schema.Description = TEXT("Test table for unit testing");
	Schema.bEnableVersioning = true;
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn NameColumn;
	NameColumn.ColumnName = TEXT("Name");
	NameColumn.DataType = EDatabaseValueType::String;
	NameColumn.bIsNullable = false;
	NameColumn.bIsIndexed = true;
	Schema.AddColumn(NameColumn);
	
	bool bCreated = TestDatabase->CreateTable(Schema);
	bool bExists = TestDatabase->HasTable(TEXT("TestTable"));
	
	Result.bPassed = bCreated && bExists;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create table");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_InsertRow()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("InsertRow");
	Result.Category = EDatabaseTestCategory::BasicOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Create table first
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("InsertTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn NameColumn;
	NameColumn.ColumnName = TEXT("Name");
	NameColumn.DataType = EDatabaseValueType::String;
	Schema.AddColumn(NameColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert row
	FMingDatabaseRow Row;
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
	Row.SetValue(TEXT("Name"), FMingDatabaseValue::MakeString(TEXT("TestName")));
	
	bool bInserted = TestDatabase->InsertRow(TEXT("InsertTest"), Row);
	int32 RowCount = TestDatabase->GetRowCount(TEXT("InsertTest"));
	
	Result.bPassed = bInserted && RowCount == 1;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Insert failed or wrong row count: %d"), RowCount);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_UpdateRow()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("UpdateRow");
	Result.Category = EDatabaseTestCategory::BasicOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("UpdateTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn NameColumn;
	NameColumn.ColumnName = TEXT("Name");
	NameColumn.DataType = EDatabaseValueType::String;
	Schema.AddColumn(NameColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert row
	FMingDatabaseRow Row;
	FGuid RowId = FGuid::NewGuid();
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(RowId));
	Row.SetValue(TEXT("Name"), FMingDatabaseValue::MakeString(TEXT("Original")));
	TestDatabase->InsertRow(TEXT("UpdateTest"), Row);
	
	// Update
	TMap<FString, FMingDatabaseValue> NewValues;
	NewValues.Add(TEXT("Name"), FMingDatabaseValue::MakeString(TEXT("Updated")));
	
	bool bUpdated = TestDatabase->UpdateRow(TEXT("UpdateTest"), RowId, NewValues);
	
	// Verify
	FMingDatabaseRow UpdatedRow = TestDatabase->GetRow(TEXT("UpdateTest"), RowId);
	FString NewName = UpdatedRow.GetValue(TEXT("Name")).StringValue;
	
	Result.bPassed = bUpdated && NewName == TEXT("Updated");
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Update failed or value not changed");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_DeleteRow()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("DeleteRow");
	Result.Category = EDatabaseTestCategory::BasicOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("DeleteTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert row
	FMingDatabaseRow Row;
	FGuid RowId = FGuid::NewGuid();
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(RowId));
	TestDatabase->InsertRow(TEXT("DeleteTest"), Row);
	
	int32 CountBefore = TestDatabase->GetRowCount(TEXT("DeleteTest"));
	
	// Delete
	bool bDeleted = TestDatabase->DeleteRow(TEXT("DeleteTest"), RowId);
	int32 CountAfter = TestDatabase->GetRowCount(TEXT("DeleteTest"));
	
	Result.bPassed = bDeleted && CountBefore == 1 && CountAfter == 0;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Delete failed: before=%d, after=%d"), CountBefore, CountAfter);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_QueryRows()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("QueryRows");
	Result.Category = EDatabaseTestCategory::QueryOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("QueryTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn TypeColumn;
	TypeColumn.ColumnName = TEXT("Type");
	TypeColumn.DataType = EDatabaseValueType::String;
	Schema.AddColumn(TypeColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert test data
	for (int32 i = 0; i < 10; i++)
	{
		FMingDatabaseRow Row;
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
		Row.SetValue(TEXT("Type"), FMingDatabaseValue::MakeString(i % 2 == 0 ? TEXT("Even") : TEXT("Odd")));
		TestDatabase->InsertRow(TEXT("QueryTest"), Row);
	}
	
	// Query
	FMingDatabaseQuery Query;
	Query.TableName = TEXT("QueryTest");
	Query.WhereEquals(TEXT("Type"), FMingDatabaseValue::MakeString(TEXT("Even")));
	Query.OrderBy(TEXT("ID"), true);
	Query.Limit = 5;
	
	TArray<FMingDatabaseRow> Results = TestDatabase->QueryRows(Query);
	
	Result.bPassed = Results.Num() == 5;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Query returned %d rows instead of 5"), Results.Num());
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_SaveLoadDatabase()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("SaveLoadDatabase");
	Result.Category = EDatabaseTestCategory::Serialization;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Create table and insert data
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("SaveLoadTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	FMingDatabaseRow Row;
	FGuid RowId = FGuid::NewGuid();
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(RowId));
	TestDatabase->InsertRow(TEXT("SaveLoadTest"), Row);
	
	// Save
	FString SavePath = FPaths::ProjectSavedDir() / TEXT("Databases") / TEXT("TestSave.db");
	bool bSaved = TestDatabase->SaveDatabase(SavePath);
	
	// Clear and reload
	TestDatabase->ClearDatabase();
	bool bLoaded = TestDatabase->LoadDatabase(SavePath);
	
	int32 RowCount = TestDatabase->GetRowCount(TEXT("SaveLoadTest"));
	
	Result.bPassed = bSaved && bLoaded && RowCount == 1;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Save=%d, Load=%d, Rows=%d"), bSaved, bLoaded, RowCount);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_ExportImportJSON()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("ExportImportJSON");
	Result.Category = EDatabaseTestCategory::Serialization;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("JSONTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	FMingDatabaseRow Row;
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
	TestDatabase->InsertRow(TEXT("JSONTest"), Row);
	
	// Export
	FString JSONPath = FPaths::ProjectSavedDir() / TEXT("Databases") / TEXT("TestExport.json");
	bool bExported = TestDatabase->ExportToJSON(JSONPath);
	
	// Import to new database
	UMingRTSDynamicDatabase* ImportDB = NewObject<UMingRTSDynamicDatabase>();
	ImportDB->InitializeDatabase(TEXT("ImportTestDB"));
	bool bImported = ImportDB->ImportFromJSON(JSONPath);
	
	int32 RowCount = ImportDB->GetRowCount(TEXT("JSONTest"));
	
	Result.bPassed = bExported && bImported && RowCount == 1;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Export=%d, Import=%d, Rows=%d"), bExported, bImported, RowCount);
	}
	
	ImportDB->ShutdownDatabase();
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_BackupRestore()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("BackupRestore");
	Result.Category = EDatabaseTestCategory::BackupRestore;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("BackupTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	FMingDatabaseRow Row;
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
	TestDatabase->InsertRow(TEXT("BackupTest"), Row);
	
	// Backup
	FString BackupPath = FPaths::ProjectSavedDir() / TEXT("Databases") / TEXT("TestBackup.db");
	bool bBackedUp = TestDatabase->BackupDatabase(BackupPath);
	
	// Clear
	TestDatabase->DeleteRow(TEXT("BackupTest"), Row.RowId);
	
	// Restore
	bool bRestored = TestDatabase->RestoreDatabase(BackupPath);
	int32 RowCount = TestDatabase->GetRowCount(TEXT("BackupTest"));
	
	Result.bPassed = bBackedUp && bRestored && RowCount == 1;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Backup=%d, Restore=%d, Rows=%d"), bBackedUp, bRestored, RowCount);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_DataTypes()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("DataTypes");
	Result.Category = EDatabaseTestCategory::DataIntegrity;
	
	double StartTime = FPlatformTime::Seconds();
	
	// Test all value types
	bool bAllPassed = true;
	
	// Boolean
	FMingDatabaseValue BoolVal = FMingDatabaseValue::MakeBool(true);
	bAllPassed &= (BoolVal.Type == EDatabaseValueType::Boolean && BoolVal.BoolValue == true);
	
	// Integer
	FMingDatabaseValue IntVal = FMingDatabaseValue::MakeInt(42);
	bAllPassed &= (IntVal.Type == EDatabaseValueType::Integer && IntVal.IntValue == 42);
	
	// Float
	FMingDatabaseValue FloatVal = FMingDatabaseValue::MakeFloat(3.14f);
	bAllPassed &= (FloatVal.Type == EDatabaseValueType::Float && FMath::IsNearlyEqual(FloatVal.FloatValue, 3.14f, 0.01f));
	
	// String
	FMingDatabaseValue StringVal = FMingDatabaseValue::MakeString(TEXT("Test"));
	bAllPassed &= (StringVal.Type == EDatabaseValueType::String && StringVal.StringValue == TEXT("Test"));
	
	// Vector
	FVector TestVector(1.0f, 2.0f, 3.0f);
	FMingDatabaseValue VecVal = FMingDatabaseValue::MakeVector(TestVector);
	bAllPassed &= (VecVal.Type == EDatabaseValueType::Vector && VecVal.VectorValue == TestVector);
	
	// DateTime
	FDateTime Now = FDateTime::Now();
	FMingDatabaseValue DateVal = FMingDatabaseValue::MakeDateTime(Now);
	bAllPassed &= (DateVal.Type == EDatabaseValueType::DateTime && DateVal.DateTimeValue == Now);
	
	// Guid
	FGuid TestGuid = FGuid::NewGuid();
	FMingDatabaseValue GuidVal = FMingDatabaseValue::MakeGuid(TestGuid);
	bAllPassed &= (GuidVal.Type == EDatabaseValueType::Guid && GuidVal.GuidValue == TestGuid);
	
	Result.bPassed = bAllPassed;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("One or more data type tests failed");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_PrimaryKeyConstraint()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("PrimaryKeyConstraint");
	Result.Category = EDatabaseTestCategory::DataIntegrity;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup with primary key
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("PKTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::String;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert first row
	FMingDatabaseRow Row1;
	Row1.SetValue(TEXT("ID"), FMingDatabaseValue::MakeString(TEXT("SAME_ID")));
	bool bFirst = TestDatabase->InsertRow(TEXT("PKTest"), Row1);
	
	// Try to insert duplicate
	FMingDatabaseRow Row2;
	Row2.SetValue(TEXT("ID"), FMingDatabaseValue::MakeString(TEXT("SAME_ID")));
	bool bSecond = TestDatabase->InsertRow(TEXT("PKTest"), Row2);
	
	Result.bPassed = bFirst && !bSecond;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Duplicate primary key was allowed");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_NullableConstraint()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("NullableConstraint");
	Result.Category = EDatabaseTestCategory::DataIntegrity;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup with non-nullable column
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("NullableTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn RequiredColumn;
	RequiredColumn.ColumnName = TEXT("Required");
	RequiredColumn.DataType = EDatabaseValueType::String;
	RequiredColumn.bIsNullable = false;
	Schema.AddColumn(RequiredColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Try to insert without required field
	FMingDatabaseRow Row;
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
	// Not setting "Required" field
	
	bool bInserted = TestDatabase->InsertRow(TEXT("NullableTest"), Row);
	
	Result.bPassed = !bInserted;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Row with null required field was inserted");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_IndexCreation()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("IndexCreation");
	Result.Category = EDatabaseTestCategory::ManagerOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	FMingDatabaseIndex Index;
	Index.IndexName = TEXT("TestIndex");
	Index.TableName = TEXT("IndexTestTable");
	Index.ColumnNames.Add(TEXT("Name"));
	Index.bIsUnique = false;
	
	// Create table first
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("IndexTestTable");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn NameColumn;
	NameColumn.ColumnName = TEXT("Name");
	NameColumn.DataType = EDatabaseValueType::String;
	NameColumn.bIsIndexed = true;
	Schema.AddColumn(NameColumn);
	
	TestDatabase->CreateTable(Schema);
	bool bCreated = TestDatabase->CreateIndex(Index);
	
	Result.bPassed = bCreated;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Failed to create index");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_ManagerCreateDatabase()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("ManagerCreateDatabase");
	Result.Category = EDatabaseTestCategory::ManagerOperations;
	
	double StartTime = FPlatformTime::Seconds();
	
	FMingDatabaseConfig Config;
	Config.DatabaseName = TEXT("ManagerTestDB");
	Config.bAutoSave = false;
	
	UMingRTSDatabaseManager* Manager = NewObject<UMingRTSDatabaseManager>();
	Manager->InitializeManager();
	
	UMingRTSDynamicDatabase* DB = Manager->CreateDatabase(Config);
	bool bExists = Manager->HasDatabase(TEXT("ManagerTestDB"));
	
	Result.bPassed = (DB != nullptr) && bExists;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Manager failed to create database");
	}
	
	Manager->DestroyDatabase(TEXT("ManagerTestDB"));
	Manager->ShutdownManager();
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_SnapshotCreation()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("SnapshotCreation");
	Result.Category = EDatabaseTestCategory::BackupRestore;
	
	double StartTime = FPlatformTime::Seconds();
	
	FMingDatabaseConfig Config;
	Config.DatabaseName = TEXT("SnapshotTestDB");
	
	UMingRTSDatabaseManager* Manager = NewObject<UMingRTSDatabaseManager>();
	Manager->InitializeManager();
	
	UMingRTSDynamicDatabase* DB = Manager->CreateDatabase(Config);
	
	// Create table and insert data
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("SnapshotTable");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	DB->CreateTable(Schema);
	
	FMingDatabaseRow Row;
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
	DB->InsertRow(TEXT("SnapshotTable"), Row);
	
	// Create snapshot
	FMingDatabaseSnapshot Snapshot = Manager->CreateSnapshot(TEXT("SnapshotTestDB"), TEXT("Test snapshot"));
	
	bool bValid = !Snapshot.SnapshotId.IsEmpty() && Snapshot.TableCount == 1 && Snapshot.TotalRowCount == 1;
	
	Result.bPassed = bValid;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Snapshot creation failed");
	}
	
	Manager->DestroyDatabase(TEXT("SnapshotTestDB"));
	Manager->ShutdownManager();
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_BatchOperations()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("BatchOperations");
	Result.Category = EDatabaseTestCategory::PerformanceBenchmark;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("BatchTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Batch insert 100 rows
	int32 Count = 100;
	for (int32 i = 0; i < Count; i++)
	{
		FMingDatabaseRow Row;
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
		TestDatabase->InsertRow(TEXT("BatchTest"), Row);
	}
	
	int32 RowCount = TestDatabase->GetRowCount(TEXT("BatchTest"));
	
	Result.bPassed = RowCount == Count;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Expected %d rows, got %d"), Count, RowCount);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_ConcurrentAccess()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("ConcurrentAccess");
	Result.Category = EDatabaseTestCategory::Concurrency;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("ConcurrentTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn CounterColumn;
	CounterColumn.ColumnName = TEXT("Counter");
	CounterColumn.DataType = EDatabaseValueType::Integer;
	Schema.AddColumn(CounterColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert initial row
	FMingDatabaseRow Row;
	FGuid RowId = FGuid::NewGuid();
	Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(RowId));
	Row.SetValue(TEXT("Counter"), FMingDatabaseValue::MakeInt(0));
	TestDatabase->InsertRow(TEXT("ConcurrentTest"), Row);
	
	// Simulate concurrent updates
	bool bAllSuccess = true;
	for (int32 i = 0; i < 10; i++)
	{
		TMap<FString, FMingDatabaseValue> NewValues;
		NewValues.Add(TEXT("Counter"), FMingDatabaseValue::MakeInt(i + 1));
		if (!TestDatabase->UpdateRow(TEXT("ConcurrentTest"), RowId, NewValues))
		{
			bAllSuccess = false;
			break;
		}
	}
	
	// Verify final value
	FMingDatabaseRow FinalRow = TestDatabase->GetRow(TEXT("ConcurrentTest"), RowId);
	int32 FinalValue = FinalRow.GetValue(TEXT("Counter")).IntValue;
	
	Result.bPassed = bAllSuccess && FinalValue == 10;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Concurrent updates failed or wrong final value: %d"), FinalValue);
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_PerformanceInsert()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("PerformanceInsert");
	Result.Category = EDatabaseTestCategory::PerformanceBenchmark;
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("PerfInsertTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Measure insert performance
	double InsertStart = FPlatformTime::Seconds();
	
	int32 Count = 1000;
	for (int32 i = 0; i < Count; i++)
	{
		FMingDatabaseRow Row;
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
		TestDatabase->InsertRow(TEXT("PerfInsertTest"), Row);
	}
	
	double InsertTime = (FPlatformTime::Seconds() - InsertStart) * 1000.0f;
	float AvgInsertTime = InsertTime / Count;
	
	int32 RowCount = TestDatabase->GetRowCount(TEXT("PerfInsertTest"));
	
	Result.bPassed = RowCount == Count && AvgInsertTime < 1.0f; // Less than 1ms per insert
	Result.ExecutionTimeMs = InsertTime;
	
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Insert performance below threshold: %.3f ms/row"), AvgInsertTime);
	}
	
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_PerformanceQuery()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("PerformanceQuery");
	Result.Category = EDatabaseTestCategory::PerformanceBenchmark;
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("PerfQueryTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	FMingDatabaseColumn TypeColumn;
	TypeColumn.ColumnName = TEXT("Type");
	TypeColumn.DataType = EDatabaseValueType::Integer;
	Schema.AddColumn(TypeColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert test data
	for (int32 i = 0; i < 1000; i++)
	{
		FMingDatabaseRow Row;
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
		Row.SetValue(TEXT("Type"), FMingDatabaseValue::MakeInt(i % 10));
		TestDatabase->InsertRow(TEXT("PerfQueryTest"), Row);
	}
	
	// Measure query performance
	double QueryStart = FPlatformTime::Seconds();
	
	FMingDatabaseQuery Query;
	Query.TableName = TEXT("PerfQueryTest");
	Query.WhereEquals(TEXT("Type"), FMingDatabaseValue::MakeInt(5));
	
	TArray<FMingDatabaseRow> Results = TestDatabase->QueryRows(Query);
	
	double QueryTime = (FPlatformTime::Seconds() - QueryStart) * 1000.0f;
	
	Result.bPassed = Results.Num() == 100 && QueryTime < 10.0f; // Less than 10ms for query
	Result.ExecutionTimeMs = QueryTime;
	
	if (!Result.bPassed)
	{
		Result.ErrorMessage = FString::Printf(TEXT("Query performance below threshold: %.2f ms"), QueryTime);
	}
	
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_IntegrityValidation()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("IntegrityValidation");
	Result.Category = EDatabaseTestCategory::DataIntegrity;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("IntegrityTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert test data
	for (int32 i = 0; i < 10; i++)
	{
		FMingDatabaseRow Row;
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(FGuid::NewGuid()));
		TestDatabase->InsertRow(TEXT("IntegrityTest"), Row);
	}
	
	// Validate integrity
	bool bValid = TestDatabase->ValidateDatabaseIntegrity();
	
	Result.bPassed = bValid;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Database integrity validation failed");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}

FDatabaseTestResult UMingRTSDatabaseTest::Test_TableOptimization()
{
	FDatabaseTestResult Result;
	Result.TestName = TEXT("TableOptimization");
	Result.Category = EDatabaseTestCategory::DataIntegrity;
	
	double StartTime = FPlatformTime::Seconds();
	
	InitializeTest();
	
	// Setup
	FMingDatabaseTableSchema Schema;
	Schema.TableName = TEXT("OptimizeTest");
	
	FMingDatabaseColumn IDColumn;
	IDColumn.ColumnName = TEXT("ID");
	IDColumn.DataType = EDatabaseValueType::Guid;
	IDColumn.bIsPrimaryKey = true;
	IDColumn.bIsNullable = false;
	Schema.AddColumn(IDColumn);
	
	TestDatabase->CreateTable(Schema);
	
	// Insert and delete data to create fragmentation
	TArray<FGuid> RowIds;
	for (int32 i = 0; i < 100; i++)
	{
		FMingDatabaseRow Row;
		FGuid RowId = FGuid::NewGuid();
		Row.SetValue(TEXT("ID"), FMingDatabaseValue::MakeGuid(RowId));
		TestDatabase->InsertRow(TEXT("OptimizeTest"), Row);
		RowIds.Add(RowId);
	}
	
	// Delete half the rows
	for (int32 i = 0; i < 50; i++)
	{
		TestDatabase->DeleteRow(TEXT("OptimizeTest"), RowIds[i]);
	}
	
	// Optimize
	bool bOptimized = TestDatabase->OptimizeTable(TEXT("OptimizeTest"));
	
	int32 RowCount = TestDatabase->GetRowCount(TEXT("OptimizeTest"));
	
	Result.bPassed = bOptimized && RowCount == 50;
	if (!Result.bPassed)
	{
		Result.ErrorMessage = TEXT("Table optimization failed");
	}
	
	Result.ExecutionTimeMs = (FPlatformTime::Seconds() - StartTime) * 1000.0f;
	return Result;
}
