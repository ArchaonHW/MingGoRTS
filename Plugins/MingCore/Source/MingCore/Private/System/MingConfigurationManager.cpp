// Copyright Epic Games, Inc. All Rights Reserved.

#include "System/MingConfigurationManager.h"
#include "TimerManager.h"
#include "Engine/GameInstance.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

UMingConfigurationManager::UMingConfigurationManager()
	: PresetStoragePath(FPaths::ProjectSavedDir() / TEXT("ConfigPresets"))
{
}

void UMingConfigurationManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Initialize all categories
	for (int32 i = 0; i < static_cast<int32>(EConfigCategory::Custom) + 1; ++i)
	{
		EConfigCategory Category = static_cast<EConfigCategory>(i);
		
		FConfigCategoryData Data;
		Data.Category = Category;
		Data.bDirty = false;
		Data.bAutoSave = true;
		
		Categories.Add(Category, Data);
	}

	// Load default values
	LoadDefaultValues();

	// Start auto-save timer
	if (GetGameInstance())
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &UMingConfigurationManager::OnAutoSaveTick);
		GetGameInstance()->GetTimerManager().SetTimer(AutoSaveTimerHandle, TimerDelegate, 30.0f, true);
	}

	UE_LOG(LogTemp, Log, TEXT("Configuration Manager initialized"));
}

void UMingConfigurationManager::Deinitialize()
{
	// Save dirty categories before shutdown
	SaveDirtyCategories();

	if (GetGameInstance())
	{
		GetGameInstance()->GetTimerManager().ClearTimer(AutoSaveTimerHandle);
	}

	Super::Deinitialize();
}

void UMingConfigurationManager::SetBool(EConfigCategory Category, const FString& Key, bool Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::Bool;
		ConfigValue->BoolValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::SetInt(EConfigCategory Category, const FString& Key, int32 Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::Int;
		ConfigValue->IntValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::SetFloat(EConfigCategory Category, const FString& Key, float Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::Float;
		ConfigValue->FloatValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::SetString(EConfigCategory Category, const FString& Key, const FString& Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::String;
		ConfigValue->StringValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::SetArray(EConfigCategory Category, const FString& Key, const TArray<FString>& Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::Array;
		ConfigValue->ArrayValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::SetMap(EConfigCategory Category, const FString& Key, const TMap<FString, FString>& Value)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigValue* ConfigValue = GetOrCreateValue(Category, Key);
	if (ConfigValue)
	{
		ConfigValue->Type = EConfigValueType::Map;
		ConfigValue->MapValue = Value;
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

bool UMingConfigurationManager::GetBool(EConfigCategory Category, const FString& Key, bool DefaultValue) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::Bool)
	{
		return ConfigValue->BoolValue;
	}
	return DefaultValue;
}

int32 UMingConfigurationManager::GetInt(EConfigCategory Category, const FString& Key, int32 DefaultValue) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::Int)
	{
		return ConfigValue->IntValue;
	}
	return DefaultValue;
}

float UMingConfigurationManager::GetFloat(EConfigCategory Category, const FString& Key, float DefaultValue) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::Float)
	{
		return ConfigValue->FloatValue;
	}
	return DefaultValue;
}

FString UMingConfigurationManager::GetString(EConfigCategory Category, const FString& Key, const FString& DefaultValue) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::String)
	{
		return ConfigValue->StringValue;
	}
	return DefaultValue;
}

TArray<FString> UMingConfigurationManager::GetArray(EConfigCategory Category, const FString& Key) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::Array)
	{
		return ConfigValue->ArrayValue;
	}
	return TArray<FString>();
}

TMap<FString, FString> UMingConfigurationManager::GetMap(EConfigCategory Category, const FString& Key) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigValue* ConfigValue = GetValue(Category, Key);
	if (ConfigValue && ConfigValue->Type == EConfigValueType::Map)
	{
		return ConfigValue->MapValue;
	}
	return TMap<FString, FString>();
}

bool UMingConfigurationManager::HasKey(EConfigCategory Category, const FString& Key) const
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		return CategoryData->Values.Contains(Key);
	}
	return false;
}

void UMingConfigurationManager::RemoveKey(EConfigCategory Category, const FString& Key)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		CategoryData->Values.Remove(Key);
		MarkCategoryDirty(Category);
		
		OnConfigChanged.Broadcast(Category, Key);
	}
}

void UMingConfigurationManager::ClearCategory(EConfigCategory Category)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		CategoryData->Values.Empty();
		MarkCategoryDirty(Category);
	}
}

TArray<FString> UMingConfigurationManager::GetAllKeys(EConfigCategory Category) const
{
	FScopeLock Lock(&ConfigLock);
	
	TArray<FString> Keys;
	const FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		CategoryData->Values.GenerateKeyArray(Keys);
	}
	return Keys;
}

bool UMingConfigurationManager::SaveCategoryToFile(EConfigCategory Category, const FString& Filename)
{
	FString FullPath = FPaths::ProjectSavedDir() / Filename;
	return InternalSaveToFile(Category, FullPath);
}

bool UMingConfigurationManager::LoadCategoryFromFile(EConfigCategory Category, const FString& Filename)
{
	FString FullPath = FPaths::ProjectSavedDir() / Filename;
	return InternalLoadFromFile(Category, FullPath);
}

bool UMingConfigurationManager::SaveAllCategories(const FString& BasePath)
{
	bool bSuccess = true;
	
	for (const auto& Pair : Categories)
	{
		FString Filename = FString::Printf(TEXT("%s/%s.json"), *BasePath, *CategoryToString(Pair.Key));
		if (!SaveCategoryToFile(Pair.Key, Filename))
		{
			bSuccess = false;
		}
	}
	
	return bSuccess;
}

bool UMingConfigurationManager::LoadAllCategories(const FString& BasePath)
{
	bool bSuccess = true;
	
	for (const auto& Pair : Categories)
	{
		FString Filename = FString::Printf(TEXT("%s/%s.json"), *BasePath, *CategoryToString(Pair.Key));
		if (!LoadCategoryFromFile(Pair.Key, Filename))
		{
			bSuccess = false;
		}
	}
	
	return bSuccess;
}

void UMingConfigurationManager::SaveDirtyCategories()
{
	for (auto& Pair : Categories)
	{
		if (Pair.Value.bDirty && Pair.Value.bAutoSave)
		{
			FString Filename = FString::Printf(TEXT("Config_%s.json"), *CategoryToString(Pair.Key));
			if (SaveCategoryToFile(Pair.Key, Filename))
			{
				Pair.Value.bDirty = false;
				OnConfigSaved.Broadcast(Pair.Key);
			}
		}
	}
}

void UMingConfigurationManager::SetAutoSaveEnabled(EConfigCategory Category, bool bEnabled)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		CategoryData->bAutoSave = bEnabled;
	}
}

bool UMingConfigurationManager::IsAutoSaveEnabled(EConfigCategory Category) const
{
	FScopeLock Lock(&ConfigLock);
	
	const FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		return CategoryData->bAutoSave;
	}
	return false;
}

FString UMingConfigurationManager::ExportToJSON() const
{
	FString Result;
	
	TSharedRef<FJsonObject> RootObject = MakeShareable(new FJsonObject);
	
	for (const auto& Pair : Categories)
	{
		TSharedRef<FJsonObject> CategoryObject = MakeShareable(new FJsonObject);
		
		for (const auto& ValuePair : Pair.Value.Values)
		{
			const FConfigValue& Value = ValuePair.Value;
			
			switch (Value.Type)
			{
			case EConfigValueType::Bool:
				CategoryObject->SetBoolField(Value.Key, Value.BoolValue);
				break;
			case EConfigValueType::Int:
				CategoryObject->SetNumberField(Value.Key, Value.IntValue);
				break;
			case EConfigValueType::Float:
				CategoryObject->SetNumberField(Value.Key, Value.FloatValue);
				break;
			case EConfigValueType::String:
				CategoryObject->SetStringField(Value.Key, Value.StringValue);
				break;
			default:
				break;
			}
		}
		
		RootObject->SetObjectField(CategoryToString(Pair.Key), CategoryObject);
	}
	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Result);
	FJsonSerializer::Serialize(RootObject, Writer);
	
	return Result;
}

bool UMingConfigurationManager::ImportFromJSON(const FString& JSONString)
{
	TSharedPtr<FJsonObject> RootObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);
	
	if (!FJsonSerializer::Deserialize(Reader, RootObject))
	{
		return false;
	}
	
	for (const auto& Pair : RootObject->Values)
	{
		EConfigCategory Category = StringToCategory(Pair.Key);
		TSharedPtr<FJsonObject> CategoryObject = Pair.Value->AsObject();
		
		if (CategoryObject.IsValid())
		{
			ImportCategoryFromJSON(Category, CategoryObject);
		}
	}
	
	return true;
}

bool UMingConfigurationManager::ExportCategoryToJSON(EConfigCategory Category, FString& OutJSON) const
{
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (!CategoryData)
	{
		return false;
	}
	
	TSharedRef<FJsonObject> CategoryObject = MakeShareable(new FJsonObject);
	
	for (const auto& ValuePair : CategoryData->Values)
	{
		const FConfigValue& Value = ValuePair.Value;
		
		switch (Value.Type)
		{
		case EConfigValueType::Bool:
			CategoryObject->SetBoolField(Value.Key, Value.BoolValue);
			break;
		case EConfigValueType::Int:
			CategoryObject->SetNumberField(Value.Key, Value.IntValue);
			break;
		case EConfigValueType::Float:
			CategoryObject->SetNumberField(Value.Key, Value.FloatValue);
			break;
		case EConfigValueType::String:
			CategoryObject->SetStringField(Value.Key, Value.StringValue);
			break;
		default:
			break;
		}
	}
	
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJSON);
	FJsonSerializer::Serialize(CategoryObject, Writer);
	
	return true;
}

bool UMingConfigurationManager::ImportCategoryFromJSON(EConfigCategory Category, const FString& JSONString)
{
	TSharedPtr<FJsonObject> CategoryObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);
	
	if (!FJsonSerializer::Deserialize(Reader, CategoryObject))
	{
		return false;
	}
	
	ClearCategory(Category);
	
	for (const auto& Pair : CategoryObject->Values)
	{
		const FString& Key = Pair.Key;
		const TSharedPtr<FJsonValue>& Value = Pair.Value;
		
		if (Value->Type == EJson::Boolean)
		{
			SetBool(Category, Key, Value->AsBool());
		}
		else if (Value->Type == EJson::Number)
		{
			double NumValue = Value->AsNumber();
			if (NumValue == FMath::RoundToInt(NumValue))
			{
				SetInt(Category, Key, (int32)NumValue);
			}
			else
			{
				SetFloat(Category, Key, (float)NumValue);
			}
		}
		else if (Value->Type == EJson::String)
		{
			SetString(Category, Key, Value->AsString());
		}
	}
	
	OnConfigLoaded.Broadcast(Category);
	return true;
}

void UMingConfigurationManager::SaveAsPreset(const FString& PresetName)
{
	FString PresetPath = PresetStoragePath / PresetName;
	SaveAllCategories(PresetPath);
}

bool UMingConfigurationManager::LoadPreset(const FString& PresetName)
{
	FString PresetPath = PresetStoragePath / PresetName;
	return LoadAllCategories(PresetPath);
}

TArray<FString> UMingConfigurationManager::GetAvailablePresets() const
{
	TArray<FString> Presets;
	
	IFileManager::Get().FindFiles(Presets, *PresetStoragePath, TEXT("*"));
	
	return Presets;
}

void UMingConfigurationManager::DeletePreset(const FString& PresetName)
{
	FString PresetPath = PresetStoragePath / PresetName;
	IFileManager::Get().DeleteDirectoryRecursively(*PresetPath);
}

bool UMingConfigurationManager::ValidateConfiguration()
{
	ValidationErrors.Empty();
	
	// Add validation rules as needed
	// Example: Check required keys exist
	
	return ValidationErrors.Num() == 0;
}

TArray<FString> UMingConfigurationManager::GetValidationErrors() const
{
	return ValidationErrors;
}

void UMingConfigurationManager::ResetToDefaults(EConfigCategory Category)
{
	FScopeLock Lock(&ConfigLock);
	
	FConfigCategoryData* Defaults = DefaultValues.Find(Category);
	FConfigCategoryData* Current = Categories.Find(Category);
	
	if (Defaults && Current)
	{
		Current->Values = Defaults->Values;
		Current->bDirty = true;
	}
}

void UMingConfigurationManager::ResetAllToDefaults()
{
	for (int32 i = 0; i < static_cast<int32>(EConfigCategory::Custom) + 1; ++i)
	{
		EConfigCategory Category = static_cast<EConfigCategory>(i);
		ResetToDefaults(Category);
	}
}

FString UMingConfigurationManager::GetConfigurationReport() const
{
	FScopeLock Lock(&ConfigLock);
	
	FString Report;
	Report += TEXT("=== Configuration Manager Report ===\n\n");
	
	int32 TotalKeys = 0;
	for (const auto& Pair : Categories)
	{
		TotalKeys += Pair.Value.Values.Num();
	}
	Report += FString::Printf(TEXT("Total Categories: %d\n"), Categories.Num());
	Report += FString::Printf(TEXT("Total Configuration Keys: %d\n\n"), TotalKeys);
	
	Report += TEXT("Configuration by Category:\n");
	for (const auto& Pair : Categories)
	{
		const FConfigCategoryData& Data = Pair.Value;
		Report += FString::Printf(TEXT("  %s: %d keys (Auto-save: %s, Dirty: %s)\n"),
			*CategoryToString(Pair.Key),
			Data.Values.Num(),
			Data.bAutoSave ? TEXT("Yes") : TEXT("No"),
			Data.bDirty ? TEXT("Yes") : TEXT("No"));
	}
	
	Report += FString::Printf(TEXT("\nPreset Storage: %s\n"), *PresetStoragePath);
	Report += FString::Printf(TEXT("Available Presets: %d\n"), GetAvailablePresets().Num());
	
	return Report;
}

FConfigValue* UMingConfigurationManager::GetOrCreateValue(EConfigCategory Category, const FString& Key)
{
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		FConfigValue* Existing = CategoryData->Values.Find(Key);
		if (Existing)
		{
			return Existing;
		}
		
		FConfigValue NewValue;
		NewValue.Key = Key;
		return &CategoryData->Values.Add(Key, NewValue);
	}
	return nullptr;
}

const FConfigValue* UMingConfigurationManager::GetValue(EConfigCategory Category, const FString& Key) const
{
	const FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		return CategoryData->Values.Find(Key);
	}
	return nullptr;
}

void UMingConfigurationManager::MarkCategoryDirty(EConfigCategory Category)
{
	FConfigCategoryData* CategoryData = Categories.Find(Category);
	if (CategoryData)
	{
		CategoryData->bDirty = true;
	}
}

void UMingConfigurationManager::OnAutoSaveTick()
{
	SaveDirtyCategories();
}

void UMingConfigurationManager::LoadDefaultValues()
{
	// Set up default values for each category
	
	// Engine defaults
	SetBool(EConfigCategory::Engine, TEXT("UseVSync"), true);
	SetInt(EConfigCategory::Engine, TEXT("MaxFPS"), 60);
	
	// Graphics defaults
	SetInt(EConfigCategory::Graphics, TEXT("ResolutionX"), 1920);
	SetInt(EConfigCategory::Graphics, TEXT("ResolutionY"), 1080);
	SetInt(EConfigCategory::Graphics, TEXT("QualityLevel"), 3);
	SetBool(EConfigCategory::Graphics, TEXT("Fullscreen"), true);
	
	// Audio defaults
	SetFloat(EConfigCategory::Audio, TEXT("MasterVolume"), 1.0f);
	SetFloat(EConfigCategory::Audio, TEXT("MusicVolume"), 0.8f);
	SetFloat(EConfigCategory::Audio, TEXT("SFXVolume"), 1.0f);
	SetBool(EConfigCategory::Audio, TEXT("Enable3DAudio"), true);
	
	// Gameplay defaults
	SetInt(EConfigCategory::Gameplay, TEXT("Difficulty"), 1);
	SetBool(EConfigCategory::Gameplay, TEXT("AutoSave"), true);
	SetBool(EConfigCategory::Gameplay, TEXT("TutorialsEnabled"), true);
	
	// Network defaults
	SetBool(EConfigCategory::Network, TEXT("EnableUPnP"), true);
	SetInt(EConfigCategory::Network, TEXT("MaxPlayers"), 8);
	SetInt(EConfigCategory::Network, TEXT("ServerPort"), 7777);
	
	// Copy to default values
	DefaultValues = Categories;
}

FString UMingConfigurationManager::CategoryToString(EConfigCategory Category) const
{
	return UEnum::GetValueAsString(Category);
}

EConfigCategory UMingConfigurationManager::StringToCategory(const FString& CategoryStr) const
{
	for (int32 i = 0; i < static_cast<int32>(EConfigCategory::Custom) + 1; ++i)
	{
		EConfigCategory Category = static_cast<EConfigCategory>(i);
		if (CategoryToString(Category) == CategoryStr)
		{
			return Category;
		}
	}
	return EConfigCategory::Custom;
}

bool UMingConfigurationManager::InternalSaveToFile(EConfigCategory Category, const FString& FullPath)
{
	FString JSON;
	if (!ExportCategoryToJSON(Category, JSON))
	{
		return false;
	}
	
	return FFileHelper::SaveStringToFile(JSON, *FullPath);
}

bool UMingConfigurationManager::InternalLoadFromFile(EConfigCategory Category, const FString& FullPath)
{
	FString JSON;
	if (!FFileHelper::LoadFileToString(JSON, *FullPath))
	{
		return false;
	}
	
	return ImportCategoryFromJSON(Category, JSON);
}
