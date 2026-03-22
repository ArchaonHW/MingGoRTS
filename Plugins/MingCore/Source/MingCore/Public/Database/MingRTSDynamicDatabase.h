// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingRTSDynamicDatabase.generated.h"

UENUM(BlueprintType)
enum class EDatabaseValueType : uint8
{
	Null		UMETA(DisplayName = "空值"),
	Boolean		UMETA(DisplayName = "布爾值"),
	Integer		UMETA(DisplayName = "整數"),
	Float		UMETA(DisplayName = "浮點數"),
	String		UMETA(DisplayName = "字符串"),
	Text		UMETA(DisplayName = "文本"),
	Vector		UMETA(DisplayName = "向量"),
	Rotator		UMETA(DisplayName = "旋轉"),
	Transform	UMETA(DisplayName = "變換"),
	Object		UMETA(DisplayName = "對象引用"),
	Class		UMETA(DisplayName = "類引用"),
	Array		UMETA(DisplayName = "數組"),
	Map			UMETA(DisplayName = "映射"),
	Binary		UMETA(DisplayName = "二進制數據"),
	DateTime	UMETA(DisplayName = "日期時間"),
	Guid		UMETA(DisplayName = "唯一標識符")
};

USTRUCT(BlueprintType)
struct FMingDatabaseValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	EDatabaseValueType Type;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	bool BoolValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	int64 IntValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	float FloatValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FString StringValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FText TextValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FVector VectorValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FRotator RotatorValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FTransform TransformValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TSoftObjectPtr<UObject> ObjectValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TSoftClassPtr<UObject> ClassValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TArray<FString> ArrayValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TMap<FString, FString> MapValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TArray<uint8> BinaryValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FDateTime DateTimeValue;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FGuid GuidValue;

	FMingDatabaseValue()
		: Type(EDatabaseValueType::Null)
		, BoolValue(false)
		, IntValue(0)
		, FloatValue(0.0f)
	{
	}

	static FMingDatabaseValue MakeBool(bool Value);
	static FMingDatabaseValue MakeInt(int64 Value);
	static FMingDatabaseValue MakeFloat(float Value);
	static FMingDatabaseValue MakeString(const FString& Value);
	static FMingDatabaseValue MakeText(const FText& Value);
	static FMingDatabaseValue MakeVector(const FVector& Value);
	static FMingDatabaseValue MakeRotator(const FRotator& Value);
	static FMingDatabaseValue MakeTransform(const FTransform& Value);
	static FMingDatabaseValue MakeObject(TSoftObjectPtr<UObject> Value);
	static FMingDatabaseValue MakeClass(TSoftClassPtr<UObject> Value);
	static FMingDatabaseValue MakeArray(const TArray<FString>& Value);
	static FMingDatabaseValue MakeMap(const TMap<FString, FString>& Value);
	static FMingDatabaseValue MakeBinary(const TArray<uint8>& Value);
	static FMingDatabaseValue MakeDateTime(const FDateTime& Value);
	static FMingDatabaseValue MakeGuid(const FGuid& Value);

	FString ToString() const;
	bool IsValid() const;
};

USTRUCT(BlueprintType)
struct FMingDatabaseColumn
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString ColumnName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	EDatabaseValueType DataType;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bIsPrimaryKey;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bIsUnique;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bIsNullable;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bIsIndexed;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FMingDatabaseValue DefaultValue;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString Description;

	FMingDatabaseColumn()
		: DataType(EDatabaseValueType::String)
		, bIsPrimaryKey(false)
		, bIsUnique(false)
		, bIsNullable(true)
		, bIsIndexed(false)
	{
	}
};

USTRUCT(BlueprintType)
struct FMingDatabaseRow
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FGuid RowId;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	TMap<FString, FMingDatabaseValue> Values;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FDateTime CreatedTime;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	FDateTime ModifiedTime;

	UPROPERTY(BlueprintReadOnly, Category = "Database")
	int32 Version;

	FMingDatabaseRow()
		: RowId(FGuid::NewGuid())
		, CreatedTime(FDateTime::Now())
		, ModifiedTime(FDateTime::Now())
		, Version(1)
	{
	}

	bool HasValue(const FString& ColumnName) const;
	FMingDatabaseValue GetValue(const FString& ColumnName) const;
	void SetValue(const FString& ColumnName, const FMingDatabaseValue& Value);
	void RemoveValue(const FString& ColumnName);
};

USTRUCT(BlueprintType)
struct FMingDatabaseTableSchema
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString TableName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	TArray<FMingDatabaseColumn> Columns;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bEnableVersioning;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bEnableAuditLog;

	FMingDatabaseTableSchema()
		: bEnableVersioning(false)
		, bEnableAuditLog(false)
	{
	}

	bool HasColumn(const FString& ColumnName) const;
	const FMingDatabaseColumn* GetColumn(const FString& ColumnName) const;
	void AddColumn(const FMingDatabaseColumn& Column);
	void RemoveColumn(const FString& ColumnName);
};

USTRUCT(BlueprintType)
struct FMingDatabaseQuery
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString TableName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	TArray<FString> SelectColumns;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	TMap<FString, FMingDatabaseValue> WhereConditions;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString OrderByColumn;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bAscending;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	int32 Limit;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	int32 Offset;

	FMingDatabaseQuery()
		: bAscending(true)
		, Limit(-1)
		, Offset(0)
	{
	}

	void SelectAll() { SelectColumns.Empty(); }
	void SelectColumn(const FString& ColumnName) { SelectColumns.Add(ColumnName); }
	void WhereEquals(const FString& ColumnName, const FMingDatabaseValue& Value);
	void OrderBy(const FString& ColumnName, bool Ascending = true);
};

USTRUCT(BlueprintType)
struct FMingDatabaseIndex
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString IndexName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	FString TableName;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	TArray<FString> ColumnNames;

	UPROPERTY(BlueprintReadWrite, Category = "Database")
	bool bIsUnique;

	FMingDatabaseIndex()
		: bIsUnique(false)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDatabaseTableChanged, const FString&, TableName, const FGuid&, RowId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDatabaseReloaded, const FString&, DatabaseName);

UCLASS(BlueprintType, Blueprintable)
class MINGCORE_API UMingRTSDynamicDatabase : public UObject
{
	GENERATED_BODY()

public:
	UMingRTSDynamicDatabase();

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InitializeDatabase(const FString& DatabaseName);

	UFUNCTION(BlueprintCallable, Category = "Database")
	void ShutdownDatabase();

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool CreateTable(const FMingDatabaseTableSchema& Schema);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool DeleteTable(const FString& TableName);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool HasTable(const FString& TableName) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	TArray<FString> GetTableNames() const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	FMingDatabaseTableSchema GetTableSchema(const FString& TableName) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool InsertRow(const FString& TableName, const FMingDatabaseRow& Row);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool UpdateRow(const FString& TableName, const FGuid& RowId, const TMap<FString, FMingDatabaseValue>& Values);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool DeleteRow(const FString& TableName, const FGuid& RowId);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool UpsertRow(const FString& TableName, const FMingDatabaseRow& Row);

	UFUNCTION(BlueprintCallable, Category = "Database")
	FMingDatabaseRow GetRow(const FString& TableName, const FGuid& RowId) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	TArray<FMingDatabaseRow> QueryRows(const FMingDatabaseQuery& Query) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	int32 GetRowCount(const FString& TableName) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool CreateIndex(const FMingDatabaseIndex& Index);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool DeleteIndex(const FString& IndexName);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool SaveDatabase(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool LoadDatabase(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ExportToJSON(const FString& FilePath) const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ImportFromJSON(const FString& FilePath);

	UFUNCTION(BlueprintCallable, Category = "Database")
	void ClearDatabase();

	UFUNCTION(BlueprintCallable, Category = "Database")
	FString GetDatabaseName() const { return DatabaseName; }

	UFUNCTION(BlueprintCallable, Category = "Database")
	int32 GetTotalRowCount() const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool BackupDatabase(const FString& BackupPath);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool RestoreDatabase(const FString& BackupPath);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ExecuteTransaction(const TArray<FString>& Operations);

	UFUNCTION(BlueprintCallable, Category = "Database")
	void SetAutoSaveEnabled(bool bEnabled, float IntervalSeconds = 60.0f);

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool ValidateDatabaseIntegrity();

	UFUNCTION(BlueprintCallable, Category = "Database")
	TArray<FString> GetTablesNeedingOptimization() const;

	UFUNCTION(BlueprintCallable, Category = "Database")
	bool OptimizeTable(const FString& TableName);

	UFUNCTION(BlueprintCallable, Category = "Database")
	FString GetDatabaseStatistics() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Database|Events")
	FOnDatabaseTableChanged OnTableChanged;

	UPROPERTY(BlueprintAssignable, Category = "Database|Events")
	FOnDatabaseReloaded OnDatabaseReloaded;

protected:
	UPROPERTY()
	FString DatabaseName;

	UPROPERTY()
	TMap<FString, FMingDatabaseTableSchema> TableSchemas;

	UPROPERTY()
	TMap<FString, TArray<FMingDatabaseRow>> Tables;

	UPROPERTY()
	TMap<FString, FMingDatabaseIndex> Indexes;

	UPROPERTY()
	bool bAutoSaveEnabled;

	UPROPERTY()
	float AutoSaveInterval;

	FTimerHandle AutoSaveTimerHandle;
	mutable FCriticalSection DatabaseLock;

protected:
	void AutoSaveTick();
	bool ValidateRowAgainstSchema(const FMingDatabaseRow& Row, const FMingDatabaseTableSchema& Schema) const;
	void NotifyTableChanged(const FString& TableName, const FGuid& RowId);
	bool InternalSave(const FString& FilePath) const;
	bool InternalLoad(const FString& FilePath);
	void RebuildIndexes(const FString& TableName);
	TArray<FMingDatabaseRow*> FindRowsByCondition(const FString& TableName, const TMap<FString, FMingDatabaseValue>& Conditions);
};
