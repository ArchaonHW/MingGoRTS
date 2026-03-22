// Copyright Epic Games, Inc. All Rights Reserved.

#include "Database/MingRTSDatabaseManager.h"
#include "Database/MingRTSDynamicDatabase.h"
#include "HAL/FileManager.h"
#include "Misc/Paths.h"

UMingRTSDatabaseManager* UMingRTSDatabaseManager::Instance = nullptr;

UMingRTSDatabaseManager* UMingRTSDatabaseManager::Get()
{
	if (!Instance)
	{
		Instance = NewObject<UMingRTSDatabaseManager>();
		Instance->InitializeManager();
	}
	return Instance;
}

void UMingRTSDatabaseManager::InitializeManager()
{
	FScopeLock Lock(&ManagerLock);
	
	if (bInitialized)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Initializing Database Manager"));
	
	// Ensure storage directory exists
	FString StorageDir = FPaths::ProjectSavedDir() / TEXT("Databases");
	IFileManager::Get().MakeDirectory(*StorageDir, true);
	
	bInitialized = true;
	
	// Load existing databases
	LoadAllDatabases();
}

void UMingRTSDatabaseManager::ShutdownManager()
{
	FScopeLock Lock(&ManagerLock);
	
	if (!bInitialized)
	{
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Shutting down Database Manager"));
	
	// Save all databases
	SaveAllDatabases();
	
	// Clear all databases
	for (auto& Pair : Databases)
	{
		if (Pair.Value)
		{
			Pair.Value->ShutdownDatabase();
		}
	}
	Databases.Empty();
	
	bInitialized = false;
}

UMingRTSDynamicDatabase* UMingRTSDatabaseManager::CreateDatabase(const FMingDatabaseConfig& Config)
{
	FScopeLock Lock(&ManagerLock);
	
	if (!bInitialized)
	{
		UE_LOG(LogTemp, Error, TEXT("Database Manager not initialized"));
		return nullptr;
	}
	
	if (Config.DatabaseName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Database name cannot be empty"));
		return nullptr;
	}
	
	if (Databases.Contains(Config.DatabaseName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Database '%s' already exists"), *Config.DatabaseName);
		return Databases[Config.DatabaseName];
	}
	
	UMingRTSDynamicDatabase* NewDatabase = NewObject<UMingRTSDynamicDatabase>();
	NewDatabase->InitializeDatabase(Config.DatabaseName);
	
	// Load existing data if available
	FString FilePath = GetDatabaseFilePath(Config.DatabaseName);
	if (FPaths::FileExists(FilePath))
	{
		NewDatabase->LoadDatabase(FilePath);
	}
	
	Databases.Add(Config.DatabaseName, NewDatabase);
	
	OnDatabaseCreated.Broadcast(Config.DatabaseName);
	
	UE_LOG(LogTemp, Log, TEXT("Created database: %s"), *Config.DatabaseName);
	return NewDatabase;
}

bool UMingRTSDatabaseManager::DestroyDatabase(const FString& DatabaseName)
{
	FScopeLock Lock(&ManagerLock);
	
	UMingRTSDynamicDatabase** Found = Databases.Find(DatabaseName);
	if (!Found || !*Found)
	{
		return false;
	}
	
	// Save before destroying
	(*Found)->SaveDatabase(GetDatabaseFilePath(DatabaseName));
	(*Found)->ShutdownDatabase();
	
	Databases.Remove(DatabaseName);
	
	OnDatabaseDestroyed.Broadcast(DatabaseName);
	
	UE_LOG(LogTemp, Log, TEXT("Destroyed database: %s"), *DatabaseName);
	return true;
}

UMingRTSDynamicDatabase* UMingRTSDatabaseManager::GetDatabase(const FString& DatabaseName) const
{
	FScopeLock Lock(&ManagerLock);
	
	const TObjectPtr<UMingRTSDynamicDatabase>* Found = Databases.Find(DatabaseName);
	return Found ? Found->Get() : nullptr;
}

TArray<FString> UMingRTSDatabaseManager::GetDatabaseNames() const
{
	FScopeLock Lock(&ManagerLock);
	TArray<FString> Result;
	Databases.GetKeys(Result);
	return Result;
}

bool UMingRTSDatabaseManager::HasDatabase(const FString& DatabaseName) const
{
	FScopeLock Lock(&ManagerLock);
	return Databases.Contains(DatabaseName);
}

bool UMingRTSDatabaseManager::SaveAllDatabases()
{
	FScopeLock Lock(&ManagerLock);
	
	bool bAllSuccess = true;
	for (const auto& Pair : Databases)
	{
		if (Pair.Value)
		{
			FString FilePath = GetDatabaseFilePath(Pair.Key);
			if (!Pair.Value->SaveDatabase(FilePath))
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to save database: %s"), *Pair.Key);
				bAllSuccess = false;
			}
		}
	}
	
	return bAllSuccess;
}

bool UMingRTSDatabaseManager::LoadAllDatabases()
{
	FScopeLock Lock(&ManagerLock);
	
	FString StorageDir = FPaths::ProjectSavedDir() / TEXT("Databases");
	
	// Find all .db files
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFiles(FoundFiles, *(StorageDir / TEXT("*.db")), true, false);
	
	for (const FString& FileName : FoundFiles)
	{
		FString DatabaseName = FPaths::GetBaseFilename(FileName);
		if (!Databases.Contains(DatabaseName))
		{
			FMingDatabaseConfig Config;
			Config.DatabaseName = DatabaseName;
			CreateDatabase(Config);
		}
	}
	
	return true;
}

FMingDatabaseSnapshot UMingRTSDatabaseManager::CreateSnapshot(const FString& DatabaseName, const FString& Description)
{
	FScopeLock Lock(&ManagerLock);
	
	FMingDatabaseSnapshot Snapshot;
	Snapshot.SnapshotId = GenerateSnapshotId();
	Snapshot.DatabaseName = DatabaseName;
	Snapshot.CreatedTime = FDateTime::Now();
	Snapshot.Description = Description;
	
	UMingRTSDynamicDatabase* Database = GetDatabase(DatabaseName);
	if (Database)
	{
		Snapshot.TableCount = Database->GetTableNames().Num();
		Snapshot.TotalRowCount = Database->GetTotalRowCount();
		
		// Save snapshot
		FString SnapshotPath = GetSnapshotFilePath(Snapshot.SnapshotId);
		if (Database->SaveDatabase(SnapshotPath))
		{
			// Get file size
			int64 FileSize = IFileManager::Get().FileSize(*SnapshotPath);
			Snapshot.FileSize = FileSize;
			
			Snapshots.Add(Snapshot.SnapshotId, Snapshot);
			
			UE_LOG(LogTemp, Log, TEXT("Created snapshot: %s"), *Snapshot.SnapshotId);
		}
	}
	
	return Snapshot;
}

bool UMingRTSDatabaseManager::RestoreSnapshot(const FString& SnapshotId)
{
	FScopeLock Lock(&ManagerLock);
	
	const FMingDatabaseSnapshot* Snapshot = Snapshots.Find(SnapshotId);
	if (!Snapshot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Snapshot not found: %s"), *SnapshotId);
		return false;
	}
	
	FString SnapshotPath = GetSnapshotFilePath(SnapshotId);
	if (!FPaths::FileExists(SnapshotPath))
	{
		UE_LOG(LogTemp, Warning, TEXT("Snapshot file not found: %s"), *SnapshotPath);
		return false;
	}
	
	// Get or create database
	UMingRTSDynamicDatabase* Database = GetDatabase(Snapshot->DatabaseName);
	if (!Database)
	{
		FMingDatabaseConfig Config;
		Config.DatabaseName = Snapshot->DatabaseName;
		Database = CreateDatabase(Config);
	}
	
	if (Database)
	{
		return Database->LoadDatabase(SnapshotPath);
	}
	
	return false;
}

TArray<FMingDatabaseSnapshot> UMingRTSDatabaseManager::GetSnapshots(const FString& DatabaseName) const
{
	FScopeLock Lock(&ManagerLock);
	
	TArray<FMingDatabaseSnapshot> Result;
	for (const auto& Pair : Snapshots)
	{
		if (Pair.Value.DatabaseName == DatabaseName)
		{
			Result.Add(Pair.Value);
		}
	}
	
	// Sort by creation time (newest first)
	Result.Sort([](const FMingDatabaseSnapshot& A, const FMingDatabaseSnapshot& B)
	{
		return A.CreatedTime > B.CreatedTime;
	});
	
	return Result;
}

bool UMingRTSDatabaseManager::DeleteSnapshot(const FString& SnapshotId)
{
	FScopeLock Lock(&ManagerLock);
	
	if (!Snapshots.Contains(SnapshotId))
	{
		return false;
	}
	
	FString SnapshotPath = GetSnapshotFilePath(SnapshotId);
	if (FPaths::FileExists(SnapshotPath))
	{
		IFileManager::Get().Delete(*SnapshotPath);
	}
	
	Snapshots.Remove(SnapshotId);
	
	UE_LOG(LogTemp, Log, TEXT("Deleted snapshot: %s"), *SnapshotId);
	return true;
}

bool UMingRTSDatabaseManager::ImportDatabase(const FString& FilePath, const FString& NewDatabaseName)
{
	if (!FPaths::FileExists(FilePath))
	{
		return false;
	}
	
	FMingDatabaseConfig Config;
	Config.DatabaseName = NewDatabaseName;
	
	UMingRTSDynamicDatabase* Database = CreateDatabase(Config);
	if (!Database)
	{
		return false;
	}
	
	return Database->LoadDatabase(FilePath);
}

bool UMingRTSDatabaseManager::ExportDatabase(const FString& DatabaseName, const FString& FilePath)
{
	UMingRTSDynamicDatabase* Database = GetDatabase(DatabaseName);
	if (!Database)
	{
		return false;
	}
	
	return Database->SaveDatabase(FilePath);
}

bool UMingRTSDatabaseManager::MigrateDatabase(const FString& SourceDatabase, const FString& TargetDatabase, 
	const TMap<FString, FString>& TableMappings)
{
	UMingRTSDynamicDatabase* Source = GetDatabase(SourceDatabase);
	if (!Source)
	{
		return false;
	}
	
	// Create or get target database
	UMingRTSDynamicDatabase* Target = GetDatabase(TargetDatabase);
	if (!Target)
	{
		FMingDatabaseConfig Config;
		Config.DatabaseName = TargetDatabase;
		Target = CreateDatabase(Config);
	}
	
	if (!Target)
	{
		return false;
	}
	
	// Copy tables according to mappings
	for (const auto& Mapping : TableMappings)
	{
		FMingDatabaseTableSchema Schema = Source->GetTableSchema(Mapping.Key);
		if (Schema.TableName.IsEmpty())
		{
			continue;
		}
		
		// Rename table in schema
		Schema.TableName = Mapping.Value;
		
		// Create table in target
		Target->CreateTable(Schema);
		
		// Copy data
		FMingDatabaseQuery Query;
		Query.TableName = Mapping.Key;
		TArray<FMingDatabaseRow> Rows = Source->QueryRows(Query);
		
		for (const auto& Row : Rows)
		{
			FMingDatabaseRow NewRow = Row;
			NewRow.RowId = FGuid::NewGuid(); // Generate new ID
			Target->InsertRow(Mapping.Value, NewRow);
		}
	}
	
	return true;
}

FString UMingRTSDatabaseManager::GetManagerStatistics() const
{
	FScopeLock Lock(&ManagerLock);
	
	FString Stats;
	Stats += FString::Printf(TEXT("Database Manager Statistics\n"));
	Stats += FString::Printf(TEXT("===========================\n"));
	Stats += FString::Printf(TEXT("Total Databases: %d\n"), Databases.Num());
	Stats += FString::Printf(TEXT("Total Snapshots: %d\n"), Snapshots.Num());
	Stats += FString::Printf(TEXT("\nDatabases:\n"));
	
	for (const auto& Pair : Databases)
	{
		if (Pair.Value)
		{
			Stats += FString::Printf(TEXT("  - %s: %d tables, %d rows\n"), 
				*Pair.Key, 
				Pair.Value->GetTableNames().Num(),
				Pair.Value->GetTotalRowCount());
		}
	}
	
	return Stats;
}

void UMingRTSDatabaseManager::SetDefaultDatabase(const FString& DatabaseName)
{
	if (Databases.Contains(DatabaseName))
	{
		DefaultDatabaseName = DatabaseName;
	}
}

UMingRTSDynamicDatabase* UMingRTSDatabaseManager::GetDefaultDatabase() const
{
	if (!DefaultDatabaseName.IsEmpty())
	{
		return GetDatabase(DefaultDatabaseName);
	}
	
	// Return first database if available
	for (const auto& Pair : Databases)
	{
		if (Pair.Value)
		{
			return Pair.Value;
		}
	}
	
	return nullptr;
}

TArray<FString> UMingRTSDatabaseManager::ValidateAllDatabases()
{
	TArray<FString> Issues;
	
	for (const auto& Pair : Databases)
	{
		if (Pair.Value && !Pair.Value->ValidateDatabaseIntegrity())
		{
			Issues.Add(FString::Printf(TEXT("Database '%s' validation failed"), *Pair.Key));
		}
	}
	
	return Issues;
}

bool UMingRTSDatabaseManager::OptimizeAllDatabases()
{
	bool bAllSuccess = true;
	
	for (const auto& Pair : Databases)
	{
		if (Pair.Value)
		{
			TArray<FString> TablesToOptimize = Pair.Value->GetTablesNeedingOptimization();
			for (const FString& TableName : TablesToOptimize)
			{
				if (!Pair.Value->OptimizeTable(TableName))
				{
					bAllSuccess = false;
				}
			}
		}
	}
	
	return bAllSuccess;
}

FString UMingRTSDatabaseManager::GenerateSnapshotId() const
{
	return FGuid::NewGuid().ToString();
}

FString UMingRTSDatabaseManager::GetDatabaseFilePath(const FString& DatabaseName) const
{
	return FPaths::ProjectSavedDir() / TEXT("Databases") / FString::Printf(TEXT("%s.db"), *DatabaseName);
}

FString UMingRTSDatabaseManager::GetSnapshotFilePath(const FString& SnapshotId) const
{
	return FPaths::ProjectSavedDir() / TEXT("Databases") / TEXT("Snapshots") / FString::Printf(TEXT("%s.snapshot"), *SnapshotId);
}
