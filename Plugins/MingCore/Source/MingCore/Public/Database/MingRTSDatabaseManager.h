// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDatabaseManager.generated.h"

USTRUCT(BlueprintType)
struct FMingDatabaseConfig
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString DatabaseName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString StoragePath;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bAutoSave;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	float AutoSaveInterval;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bEnableCompression;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bEnableEncryption;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString EncryptionKey;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	int32 MaxCacheSize;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bEnableReplication;

	FMingDatabaseConfig()
		: DatabaseName(TEXT("DefaultDB"))
		, StoragePath(TEXT("Saved/Databases"))
		, bAutoSave(true)
		, AutoSaveInterval(60.0f)
		, bEnableCompression(false)
		, bEnableEncryption(false)
		, MaxCacheSize(1000)
		, bEnableReplication(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FMingDatabaseSnapshot
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FString SnapshotId;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FString DatabaseName;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FDateTime CreatedTime;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FString Description;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	int32 TableCount;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	int32 TotalRowCount;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	int64 FileSize;

	FMingDatabaseSnapshot()
		: TableCount(0)
		, TotalRowCount(0)
		, FileSize(0)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDatabaseCreated, const FString&, DatabaseName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDatabaseDestroyed, const FString&, DatabaseName);

UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSDatabaseManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool InitializeManager();

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	void ShutdownManager();

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	UMingRTSDynamicDatabase* CreateDatabase(const FMingDatabaseConfig& Config);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool DestroyDatabase(const FString& DatabaseName);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	UMingRTSDynamicDatabase* GetDatabase(const FString& DatabaseName) const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	TArray<FString> GetDatabaseNames() const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool HasDatabase(const FString& DatabaseName) const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool SaveAllDatabases();

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool LoadAllDatabases();

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	FMingDatabaseSnapshot CreateSnapshot(const FString& DatabaseName, const FString& Description);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool RestoreSnapshot(const FString& SnapshotId);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	TArray<FMingDatabaseSnapshot> GetSnapshots(const FString& DatabaseName) const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool DeleteSnapshot(const FString& SnapshotId);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool ImportDatabase(const FString& FilePath, const FString& NewDatabaseName);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool ExportDatabase(const FString& DatabaseName, const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool MigrateDatabase(const FString& SourceDatabase, const FString& TargetDatabase, 
		const TMap<FString, FString>& TableMappings);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	FString GetManagerStatistics() const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	void SetDefaultDatabase(const FString& DatabaseName);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	UMingRTSDynamicDatabase* GetDefaultDatabase() const;

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool ReplicateDatabase(const FString& DatabaseName, const FString& TargetServer);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool SyncDatabase(const FString& DatabaseName);

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	TArray<FString> ValidateAllDatabases();

	UFUNCTION(BlueprintCallable, Category = "Database|Manager")
	bool OptimizeAllDatabases();

public:
	UPROPERTY(BlueprintAssignable, Category = "Database|Manager|Events")
	FOnDatabaseCreated OnDatabaseCreated;

	UPROPERTY(BlueprintAssignable, Category = "Database|Manager|Events")
	FOnDatabaseDestroyed OnDatabaseDestroyed;

protected:
	UPROPERTY()
	TMap<FString, TObjectPtr<UMingRTSDynamicDatabase>> Databases;

	UPROPERTY()
	TMap<FString, FMingDatabaseSnapshot> Snapshots;

	UPROPERTY()
	FString DefaultDatabaseName;

	UPROPERTY()
	bool bInitialized;

	mutable FCriticalSection ManagerLock;

protected:
	FString GenerateSnapshotId() const;
	FString GetDatabaseFilePath(const FString& DatabaseName) const;
	FString GetSnapshotFilePath(const FString& SnapshotId) const;
	void OnDatabaseAutoSaveTimer(UMingRTSDynamicDatabase* Database);
};
