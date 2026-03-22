// Copyright Epic Games, Inc. All Rights Reserved.

#include "Database/MingRTSDynamicDatabase.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"
#include "Dom/JsonObject.h"
#include "TimerManager.h"

// FMingDatabaseValue Implementation
FMingDatabaseValue FMingDatabaseValue::MakeBool(bool Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Boolean;
	Result.BoolValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeInt(int64 Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Integer;
	Result.IntValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeFloat(float Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Float;
	Result.FloatValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeString(const FString& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::String;
	Result.StringValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeText(const FText& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Text;
	Result.TextValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeVector(const FVector& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Vector;
	Result.VectorValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeRotator(const FRotator& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Rotator;
	Result.RotatorValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeTransform(const FTransform& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Transform;
	Result.TransformValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeObject(TSoftObjectPtr<UObject> Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Object;
	Result.ObjectValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeClass(TSoftClassPtr<UObject> Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Class;
	Result.ClassValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeArray(const TArray<FString>& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Array;
	Result.ArrayValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeMap(const TMap<FString, FString>& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Map;
	Result.MapValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeBinary(const TArray<uint8>& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Binary;
	Result.BinaryValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeDateTime(const FDateTime& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::DateTime;
	Result.DateTimeValue = Value;
	return Result;
}

FMingDatabaseValue FMingDatabaseValue::MakeGuid(const FGuid& Value)
{
	FMingDatabaseValue Result;
	Result.Type = EDatabaseValueType::Guid;
	Result.GuidValue = Value;
	return Result;
}

FString FMingDatabaseValue::ToString() const
{
	switch (Type)
	{
	case EDatabaseValueType::Null:
		return TEXT("NULL");
	case EDatabaseValueType::Boolean:
		return BoolValue ? TEXT("TRUE") : TEXT("FALSE");
	case EDatabaseValueType::Integer:
		return FString::Printf(TEXT("%lld"), IntValue);
	case EDatabaseValueType::Float:
		return FString::Printf(TEXT("%f"), FloatValue);
	case EDatabaseValueType::String:
		return StringValue;
	case EDatabaseValueType::Text:
		return TextValue.ToString();
	case EDatabaseValueType::Vector:
		return FString::Printf(TEXT("(%f,%f,%f)"), VectorValue.X, VectorValue.Y, VectorValue.Z);
	case EDatabaseValueType::Rotator:
		return FString::Printf(TEXT("(%f,%f,%f)"), RotatorValue.Pitch, RotatorValue.Yaw, RotatorValue.Roll);
	case EDatabaseValueType::Transform:
		return TEXT("[Transform]");
	case EDatabaseValueType::Object:
		return ObjectValue.ToSoftObjectPath().ToString();
	case EDatabaseValueType::Class:
		return ClassValue.ToSoftObjectPath().ToString();
	case EDatabaseValueType::Array:
		return FString::Printf(TEXT("[%d items]"), ArrayValue.Num());
	case EDatabaseValueType::Map:
		return FString::Printf(TEXT("{%d items}"), MapValue.Num());
	case EDatabaseValueType::Binary:
		return FString::Printf(TEXT("<%d bytes>"), BinaryValue.Num());
	case EDatabaseValueType::DateTime:
		return DateTimeValue.ToString();
	case EDatabaseValueType::Guid:
		return GuidValue.ToString();
	default:
		return TEXT("UNKNOWN");
	}
}

bool FMingDatabaseValue::IsValid() const
{
	return Type != EDatabaseValueType::Null;
}

// FMingDatabaseRow Implementation
bool FMingDatabaseRow::HasValue(const FString& ColumnName) const
{
	return Values.Contains(ColumnName);
}

FMingDatabaseValue FMingDatabaseRow::GetValue(const FString& ColumnName) const
{
	const FMingDatabaseValue* Found = Values.Find(ColumnName);
	if (Found)
	{
		return *Found;
	}
	return FMingDatabaseValue();
}

void FMingDatabaseRow::SetValue(const FString& ColumnName, const FMingDatabaseValue& Value)
{
	Values.Add(ColumnName, Value);
	ModifiedTime = FDateTime::Now();
	Version++;
}

void FMingDatabaseRow::RemoveValue(const FString& ColumnName)
{
	Values.Remove(ColumnName);
	ModifiedTime = FDateTime::Now();
	Version++;
}

// FMingDatabaseTableSchema Implementation
bool FMingDatabaseTableSchema::HasColumn(const FString& ColumnName) const
{
	for (const auto& Column : Columns)
	{
		if (Column.ColumnName == ColumnName)
		{
			return true;
		}
	}
	return false;
}

const FMingDatabaseColumn* FMingDatabaseTableSchema::GetColumn(const FString& ColumnName) const
{
	for (const auto& Column : Columns)
	{
		if (Column.ColumnName == ColumnName)
		{
			return &Column;
		}
	}
	return nullptr;
}

void FMingDatabaseTableSchema::AddColumn(const FMingDatabaseColumn& Column)
{
	for (auto& Existing : Columns)
	{
		if (Existing.ColumnName == Column.ColumnName)
		{
			Existing = Column;
			return;
		}
	}
	Columns.Add(Column);
}

void FMingDatabaseTableSchema::RemoveColumn(const FString& ColumnName)
{
	for (int32 i = Columns.Num() - 1; i >= 0; --i)
	{
		if (Columns[i].ColumnName == ColumnName)
		{
			Columns.RemoveAt(i);
			return;
		}
	}
}

// FMingDatabaseQuery Implementation
void FMingDatabaseQuery::WhereEquals(const FString& ColumnName, const FMingDatabaseValue& Value)
{
	WhereConditions.Add(ColumnName, Value);
}

void FMingDatabaseQuery::OrderBy(const FString& ColumnName, bool Ascending)
{
	OrderByColumn = ColumnName;
	bAscending = Ascending;
}

// UMingRTSDynamicDatabase Implementation
UMingRTSDynamicDatabase::UMingRTSDynamicDatabase()
	: bAutoSaveEnabled(false)
	, AutoSaveInterval(60.0f)
{
}

bool UMingRTSDynamicDatabase::InitializeDatabase(const FString& InDatabaseName)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (InDatabaseName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Database name cannot be empty"));
		return false;
	}
	
	DatabaseName = InDatabaseName;
	Tables.Empty();
	TableSchemas.Empty();
	Indexes.Empty();
	
	UE_LOG(LogTemp, Log, TEXT("Initialized database: %s"), *DatabaseName);
	return true;
}

void UMingRTSDynamicDatabase::ShutdownDatabase()
{
	FScopeLock Lock(&DatabaseLock);
	
	if (bAutoSaveEnabled)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
		}
	}
	
	Tables.Empty();
	TableSchemas.Empty();
	Indexes.Empty();
	DatabaseName.Empty();
}

bool UMingRTSDynamicDatabase::CreateTable(const FMingDatabaseTableSchema& Schema)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (Schema.TableName.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Table name cannot be empty"));
		return false;
	}
	
	if (Tables.Contains(Schema.TableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Table '%s' already exists"), *Schema.TableName);
		return false;
	}
	
	if (Schema.Columns.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Table must have at least one column"));
		return false;
	}
	
	bool bHasPrimaryKey = false;
	for (const auto& Column : Schema.Columns)
	{
		if (Column.bIsPrimaryKey)
		{
			bHasPrimaryKey = true;
			break;
		}
	}
	
	if (!bHasPrimaryKey)
	{
		UE_LOG(LogTemp, Warning, TEXT("Table must have at least one primary key column"));
		return false;
	}
	
	TableSchemas.Add(Schema.TableName, Schema);
	Tables.Add(Schema.TableName, TArray<FMingDatabaseRow>());
	
	UE_LOG(LogTemp, Log, TEXT("Created table: %s"), *Schema.TableName);
	return true;
}

bool UMingRTSDynamicDatabase::DeleteTable(const FString& TableName)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(TableName))
	{
		return false;
	}
	
	Tables.Remove(TableName);
	TableSchemas.Remove(TableName);
	
	// Remove associated indexes
	TArray<FString> IndexesToRemove;
	for (const auto& IndexPair : Indexes)
	{
		if (IndexPair.Value.TableName == TableName)
		{
			IndexesToRemove.Add(IndexPair.Key);
		}
	}
	for (const auto& IndexName : IndexesToRemove)
	{
		Indexes.Remove(IndexName);
	}
	
	UE_LOG(LogTemp, Log, TEXT("Deleted table: %s"), *TableName);
	return true;
}

bool UMingRTSDynamicDatabase::HasTable(const FString& TableName) const
{
	FScopeLock Lock(&DatabaseLock);
	return Tables.Contains(TableName);
}

TArray<FString> UMingRTSDynamicDatabase::GetTableNames() const
{
	FScopeLock Lock(&DatabaseLock);
	TArray<FString> Result;
	Tables.GetKeys(Result);
	return Result;
}

FMingDatabaseTableSchema UMingRTSDynamicDatabase::GetTableSchema(const FString& TableName) const
{
	FScopeLock Lock(&DatabaseLock);
	const FMingDatabaseTableSchema* Found = TableSchemas.Find(TableName);
	if (Found)
	{
		return *Found;
	}
	return FMingDatabaseTableSchema();
}

bool UMingRTSDynamicDatabase::InsertRow(const FString& TableName, const FMingDatabaseRow& Row)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(TableName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Table '%s' does not exist"), *TableName);
		return false;
	}
	
	const FMingDatabaseTableSchema& Schema = TableSchemas[TableName];
	if (!ValidateRowAgainstSchema(Row, Schema))
	{
		return false;
	}
	
	// Check for duplicate primary keys
	for (const auto& ExistingRow : Tables[TableName])
	{
		bool bDuplicate = true;
		for (const auto& Column : Schema.Columns)
		{
			if (Column.bIsPrimaryKey)
			{
				FMingDatabaseValue ExistingValue = ExistingRow.GetValue(Column.ColumnName);
				FMingDatabaseValue NewValue = Row.GetValue(Column.ColumnName);
				if (ExistingValue.ToString() != NewValue.ToString())
				{
					bDuplicate = false;
					break;
				}
			}
		}
		if (bDuplicate)
		{
			UE_LOG(LogTemp, Warning, TEXT("Duplicate primary key detected"));
			return false;
		}
	}
	
	Tables[TableName].Add(Row);
	NotifyTableChanged(TableName, Row.RowId);
	
	UE_LOG(LogTemp, Log, TEXT("Inserted row into %s: %s"), *TableName, *Row.RowId.ToString());
	return true;
}

bool UMingRTSDynamicDatabase::UpdateRow(const FString& TableName, const FGuid& RowId, const TMap<FString, FMingDatabaseValue>& Values)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(TableName))
	{
		return false;
	}
	
	TArray<FMingDatabaseRow>& TableRows = Tables[TableName];
	for (auto& Row : TableRows)
	{
		if (Row.RowId == RowId)
		{
			for (const auto& ValuePair : Values)
			{
				Row.SetValue(ValuePair.Key, ValuePair.Value);
			}
			NotifyTableChanged(TableName, RowId);
			return true;
		}
	}
	
	return false;
}

bool UMingRTSDynamicDatabase::DeleteRow(const FString& TableName, const FGuid& RowId)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(TableName))
	{
		return false;
	}
	
	TArray<FMingDatabaseRow>& TableRows = Tables[TableName];
	for (int32 i = 0; i < TableRows.Num(); ++i)
	{
		if (TableRows[i].RowId == RowId)
		{
			TableRows.RemoveAt(i);
			NotifyTableChanged(TableName, RowId);
			return true;
		}
	}
	
	return false;
}

bool UMingRTSDynamicDatabase::UpsertRow(const FString& TableName, const FMingDatabaseRow& Row)
{
	FScopeLock Lock(&DatabaseLock);
	
	if (!Tables.Contains(TableName))
	{
		return false;
	}
	
	TArray<FMingDatabaseRow>& TableRows = Tables[TableName];
	for (auto& ExistingRow : TableRows)
	{
		if (ExistingRow.RowId == Row.RowId)
		{
			// Update existing
			ExistingRow = Row;
			NotifyTableChanged(TableName, Row.RowId);
			return true;
		}
	}
	
	// Insert new
	return InsertRow(TableName, Row);
}

FMingDatabaseRow UMingRTSDynamicDatabase::GetRow(const FString& TableName, const FGuid& RowId) const
{
	FScopeLock Lock(&DatabaseLock);
	
	const TArray<FMingDatabaseRow>* TableRows = Tables.Find(TableName);
	if (TableRows)
	{
		for (const auto& Row : *TableRows)
		{
			if (Row.RowId == RowId)
			{
				return Row;
			}
		}
	}
	
	return FMingDatabaseRow();
}

TArray<FMingDatabaseRow> UMingRTSDynamicDatabase::QueryRows(const FMingDatabaseQuery& Query) const
{
	FScopeLock Lock(&DatabaseLock);
	
	TArray<FMingDatabaseRow> Results;
	
	const TArray<FMingDatabaseRow>* TableRows = Tables.Find(Query.TableName);
	if (!TableRows)
	{
		return Results;
	}
	
	// Filter rows
	for (const auto& Row : *TableRows)
	{
		bool bMatches = true;
		for (const auto& Condition : Query.WhereConditions)
		{
			FMingDatabaseValue RowValue = Row.GetValue(Condition.Key);
			if (RowValue.ToString() != Condition.Value.ToString())
			{
				bMatches = false;
				break;
			}
		}
		
		if (bMatches)
		{
			Results.Add(Row);
		}
	}
	
	// Sort results
	if (!Query.OrderByColumn.IsEmpty())
	{
		Results.Sort([&](const FMingDatabaseRow& A, const FMingDatabaseRow& B)
		{
			FString ValueA = A.GetValue(Query.OrderByColumn).ToString();
			FString ValueB = B.GetValue(Query.OrderByColumn).ToString();
			return Query.bAscending ? ValueA < ValueB : ValueA > ValueB;
		});
	}
	
	// Apply offset and limit
	if (Query.Offset > 0)
	{
		int32 RemoveCount = FMath::Min(Query.Offset, Results.Num());
		Results.RemoveAt(0, RemoveCount);
	}
	
	if (Query.Limit > 0 && Results.Num() > Query.Limit)
	{
		Results.SetNum(Query.Limit);
	}
	
	return Results;
}

int32 UMingRTSDynamicDatabase::GetRowCount(const FString& TableName) const
{
	FScopeLock Lock(&DatabaseLock);
	const TArray<FMingDatabaseRow>* TableRows = Tables.Find(TableName);
	return TableRows ? TableRows->Num() : 0;
}

bool UMingRTSDynamicDatabase::SaveDatabase(const FString& FilePath)
{
	FScopeLock Lock(&DatabaseLock);
	return InternalSave(FilePath);
}

bool UMingRTSDynamicDatabase::LoadDatabase(const FString& FilePath)
{
	FScopeLock Lock(&DatabaseLock);
	return InternalLoad(FilePath);
}

bool UMingRTSDynamicDatabase::ExportToJSON(const FString& FilePath) const
{
	FScopeLock Lock(&DatabaseLock);
	
	TSharedPtr<FJsonObject> RootObject = MakeShared<FJsonObject>();
	RootObject->SetStringField(TEXT("DatabaseName"), DatabaseName);
	RootObject->SetNumberField(TEXT("Version"), 1);
	
	TArray<TSharedPtr<FJsonValue>> TablesArray;
	for (const auto& TablePair : Tables)
	{
		TSharedPtr<FJsonObject> TableObject = MakeShared<FJsonObject>();
		TableObject->SetStringField(TEXT("TableName"), TablePair.Key);
		
		TArray<TSharedPtr<FJsonValue>> RowsArray;
		for (const auto& Row : TablePair.Value)
		{
			TSharedPtr<FJsonObject> RowObject = MakeShared<FJsonObject>();
			RowObject->SetStringField(TEXT("RowId"), Row.RowId.ToString());
			
			TArray<TSharedPtr<FJsonValue>> ValuesArray;
			for (const auto& ValuePair : Row.Values)
			{
				TSharedPtr<FJsonObject> ValueObject = MakeShared<FJsonObject>();
				ValueObject->SetStringField(TEXT("Column"), ValuePair.Key);
				ValueObject->SetStringField(TEXT("Value"), ValuePair.Value.ToString());
				ValuesArray.Add(MakeShared<FJsonValueObject>(ValueObject));
			}
			RowObject->SetArrayField(TEXT("Values"), ValuesArray);
			RowsArray.Add(MakeShared<FJsonValueObject>(RowObject));
		}
		TableObject->SetArrayField(TEXT("Rows"), RowsArray);
		TablesArray.Add(MakeShared<FJsonValueObject>(TableObject));
	}
	RootObject->SetArrayField(TEXT("Tables"), TablesArray);
	
	FString OutputString;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
	if (FJsonSerializer::Serialize(RootObject.ToSharedRef(), Writer))
	{
		return FFileHelper::SaveStringToFile(OutputString, *FilePath);
	}
	
	return false;
}

bool UMingRTSDynamicDatabase::ImportFromJSON(const FString& FilePath)
{
	FScopeLock Lock(&DatabaseLock);
	
	FString Content;
	if (!FFileHelper::LoadFileToString(Content, *FilePath))
	{
		return false;
	}
	
	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Content);
	if (!FJsonSerializer::Deserialize(Reader, RootObject))
	{
		return false;
	}
	
	DatabaseName = RootObject->GetStringField(TEXT("DatabaseName"));
	Tables.Empty();
	TableSchemas.Empty();
	
	const TArray<TSharedPtr<FJsonValue>>* TablesArray;
	if (RootObject->TryGetArrayField(TEXT("Tables"), TablesArray))
	{
		for (const auto& TableValue : *TablesArray)
		{
			TSharedPtr<FJsonObject> TableObject = TableValue->AsObject();
			if (!TableObject.IsValid()) continue;
			
			FString TableName = TableObject->GetStringField(TEXT("TableName"));
			
			FMingDatabaseTableSchema Schema;
			Schema.TableName = TableName;
			TableSchemas.Add(TableName, Schema);
			Tables.Add(TableName, TArray<FMingDatabaseRow>());
			
			const TArray<TSharedPtr<FJsonValue>>* RowsArray;
			if (TableObject->TryGetArrayField(TEXT("Rows"), RowsArray))
			{
				for (const auto& RowValue : *RowsArray)
				{
					TSharedPtr<FJsonObject> RowObject = RowValue->AsObject();
					if (!RowObject.IsValid()) continue;
					
					FMingDatabaseRow Row;
					FGuid::Parse(RowObject->GetStringField(TEXT("RowId")), Row.RowId);
					
					const TArray<TSharedPtr<FJsonValue>>* ValuesArray;
					if (RowObject->TryGetArrayField(TEXT("Values"), ValuesArray))
					{
						for (const auto& ValueValue : *ValuesArray)
						{
							TSharedPtr<FJsonObject> ValueObject = ValueValue->AsObject();
							if (!ValueObject.IsValid()) continue;
							
							FString Column = ValueObject->GetStringField(TEXT("Column"));
							FString ValueStr = ValueObject->GetStringField(TEXT("Value"));
							Row.Values.Add(Column, FMingDatabaseValue::MakeString(ValueStr));
						}
					}
					
					Tables[TableName].Add(Row);
				}
			}
		}
	}
	
	return true;
}

void UMingRTSDynamicDatabase::ClearDatabase()
{
	FScopeLock Lock(&DatabaseLock);
	Tables.Empty();
	TableSchemas.Empty();
	Indexes.Empty();
}

int32 UMingRTSDynamicDatabase::GetTotalRowCount() const
{
	FScopeLock Lock(&DatabaseLock);
	int32 Total = 0;
	for (const auto& TablePair : Tables)
	{
		Total += TablePair.Value.Num();
	}
	return Total;
}

bool UMingRTSDynamicDatabase::BackupDatabase(const FString& BackupPath)
{
	return SaveDatabase(BackupPath);
}

bool UMingRTSDynamicDatabase::RestoreDatabase(const FString& BackupPath)
{
	return LoadDatabase(BackupPath);
}

bool UMingRTSDynamicDatabase::ValidateDatabaseIntegrity()
{
	FScopeLock Lock(&DatabaseLock);
	
	for (const auto& TablePair : Tables)
	{
		const FMingDatabaseTableSchema* Schema = TableSchemas.Find(TablePair.Key);
		if (!Schema)
		{
			UE_LOG(LogTemp, Error, TEXT("Missing schema for table: %s"), *TablePair.Key);
			return false;
		}
		
		for (const auto& Row : TablePair.Value)
		{
			if (!ValidateRowAgainstSchema(Row, *Schema))
			{
				UE_LOG(LogTemp, Error, TEXT("Row validation failed in table: %s"), *TablePair.Key);
				return false;
			}
		}
	}
	
	return true;
}

FString UMingRTSDynamicDatabase::GetDatabaseStatistics() const
{
	FScopeLock Lock(&DatabaseLock);
	
	FString Stats;
	Stats += FString::Printf(TEXT("Database: %s\n"), *DatabaseName);
	Stats += FString::Printf(TEXT("Tables: %d\n"), Tables.Num());
	Stats += FString::Printf(TEXT("Total Rows: %d\n"), GetTotalRowCount());
	Stats += FString::Printf(TEXT("Indexes: %d\n"), Indexes.Num());
	Stats += TEXT("\nTable Details:\n");
	
	for (const auto& TablePair : Tables)
	{
		Stats += FString::Printf(TEXT("  %s: %d rows\n"), *TablePair.Key, TablePair.Value.Num());
	}
	
	return Stats;
}

bool UMingRTSDynamicDatabase::ValidateRowAgainstSchema(const FMingDatabaseRow& Row, const FMingDatabaseTableSchema& Schema) const
{
	for (const auto& Column : Schema.Columns)
	{
		if (!Column.bIsNullable && !Row.HasValue(Column.ColumnName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Missing required column: %s"), *Column.ColumnName);
			return false;
		}
		
		if (Row.HasValue(Column.ColumnName))
		{
			FMingDatabaseValue Value = Row.GetValue(Column.ColumnName);
			// Type validation could be added here
		}
	}
	
	return true;
}

void UMingRTSDynamicDatabase::NotifyTableChanged(const FString& TableName, const FGuid& RowId)
{
	OnTableChanged.Broadcast(TableName, RowId);
}

bool UMingRTSDynamicDatabase::InternalSave(const FString& FilePath) const
{
	TArray<uint8> Data;
	FMemoryWriter Ar(Data);
	
	Ar << DatabaseName;
	Ar << TableSchemas;
	Ar << Tables;
	Ar << Indexes;
	
	return FFileHelper::SaveArrayToFile(Data, *FilePath);
}

bool UMingRTSDynamicDatabase::InternalLoad(const FString& FilePath)
{
	TArray<uint8> Data;
	if (!FFileHelper::LoadFileToArray(Data, *FilePath))
	{
		return false;
	}
	
	FMemoryReader Ar(Data);
	Ar << DatabaseName;
	Ar << TableSchemas;
	Ar << Tables;
	Ar << Indexes;
	
	return true;
}
