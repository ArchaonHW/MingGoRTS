// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MingConfigurationManager.generated.h"

UENUM(BlueprintType)
enum class EConfigCategory : uint8
{
	Engine,
	Graphics,
	Audio,
	Gameplay,
	Input,
	Network,
	Localization,
	SaveLoad,
	Debug,
	Custom
};

UENUM(BlueprintType)
enum class EConfigValueType : uint8
{
	Bool,
	Int,
	Float,
	String,
	Array,
	Map,
	Struct
};

USTRUCT(BlueprintType)
struct FConfigValue
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	FString Key;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	EConfigValueType Type;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	bool BoolValue;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	int32 IntValue;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	float FloatValue;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	FString StringValue;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	TArray<FString> ArrayValue;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	TMap<FString, FString> MapValue;

	FConfigValue()
		: Type(EConfigValueType::String)
		, BoolValue(false)
		, IntValue(0)
		, FloatValue(0.0f)
	{
	}

	static FConfigValue MakeBool(const FString& InKey, bool Value)
	{
		FConfigValue Config;
		Config.Key = InKey;
		Config.Type = EConfigValueType::Bool;
		Config.BoolValue = Value;
		return Config;
	}

	static FConfigValue MakeInt(const FString& InKey, int32 Value)
	{
		FConfigValue Config;
		Config.Key = InKey;
		Config.Type = EConfigValueType::Int;
		Config.IntValue = Value;
		return Config;
	}

	static FConfigValue MakeFloat(const FString& InKey, float Value)
	{
		FConfigValue Config;
		Config.Key = InKey;
		Config.Type = EConfigValueType::Float;
		Config.FloatValue = Value;
		return Config;
	}

	static FConfigValue MakeString(const FString& InKey, const FString& Value)
	{
		FConfigValue Config;
		Config.Key = InKey;
		Config.Type = EConfigValueType::String;
		Config.StringValue = Value;
		return Config;
	}

	bool GetBool() const { return BoolValue; }
	int32 GetInt() const { return IntValue; }
	float GetFloat() const { return FloatValue; }
	FString GetString() const { return StringValue; }
	TArray<FString> GetArray() const { return ArrayValue; }
	TMap<FString, FString> GetMap() const { return MapValue; }
};

USTRUCT(BlueprintType)
struct FConfigCategoryData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	EConfigCategory Category;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	TMap<FString, FConfigValue> Values;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	bool bDirty;

	UPROPERTY(BlueprintReadOnly, Category = "Config")
	bool bAutoSave;

	FConfigCategoryData()
		: Category(EConfigCategory::Custom)
		, bDirty(false)
		, bAutoSave(true)
	{
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnConfigChanged, EConfigCategory, Category, const FString&, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfigSaved, EConfigCategory, Category);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnConfigLoaded, EConfigCategory, Category);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MINGCORE_API UMingConfigurationManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMingConfigurationManager();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// Value Setters
	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetBool(EConfigCategory Category, const FString& Key, bool Value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetInt(EConfigCategory Category, const FString& Key, int32 Value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetFloat(EConfigCategory Category, const FString& Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetString(EConfigCategory Category, const FString& Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetArray(EConfigCategory Category, const FString& Key, const TArray<FString>& Value);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetMap(EConfigCategory Category, const FString& Key, const TMap<FString, FString>& Value);

	// Value Getters
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool GetBool(EConfigCategory Category, const FString& Key, bool DefaultValue = false) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	int32 GetInt(EConfigCategory Category, const FString& Key, int32 DefaultValue = 0) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	float GetFloat(EConfigCategory Category, const FString& Key, float DefaultValue = 0.0f) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	FString GetString(EConfigCategory Category, const FString& Key, const FString& DefaultValue = TEXT("")) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	TArray<FString> GetArray(EConfigCategory Category, const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	TMap<FString, FString> GetMap(EConfigCategory Category, const FString& Key) const;

	// Configuration Management
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool HasKey(EConfigCategory Category, const FString& Key) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	void RemoveKey(EConfigCategory Category, const FString& Key);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void ClearCategory(EConfigCategory Category);

	UFUNCTION(BlueprintCallable, Category = "Config")
	TArray<FString> GetAllKeys(EConfigCategory Category) const;

	// Save/Load
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool SaveCategoryToFile(EConfigCategory Category, const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool LoadCategoryFromFile(EConfigCategory Category, const FString& Filename);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool SaveAllCategories(const FString& BasePath);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool LoadAllCategories(const FString& BasePath);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SaveDirtyCategories();

	// Auto-save settings
	UFUNCTION(BlueprintCallable, Category = "Config")
	void SetAutoSaveEnabled(EConfigCategory Category, bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool IsAutoSaveEnabled(EConfigCategory Category) const;

	// Import/Export
	UFUNCTION(BlueprintCallable, Category = "Config")
	FString ExportToJSON() const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool ImportFromJSON(const FString& JSONString);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool ExportCategoryToJSON(EConfigCategory Category, FString& OutJSON) const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool ImportCategoryFromJSON(EConfigCategory Category, const FString& JSONString);

	// Presets
	UFUNCTION(BlueprintCallable, Category = "Config")
	void SaveAsPreset(const FString& PresetName);

	UFUNCTION(BlueprintCallable, Category = "Config")
	bool LoadPreset(const FString& PresetName);

	UFUNCTION(BlueprintCallable, Category = "Config")
	TArray<FString> GetAvailablePresets() const;

	UFUNCTION(BlueprintCallable, Category = "Config")
	void DeletePreset(const FString& PresetName);

	// Validation
	UFUNCTION(BlueprintCallable, Category = "Config")
	bool ValidateConfiguration();

	UFUNCTION(BlueprintCallable, Category = "Config")
	TArray<FString> GetValidationErrors() const;

	// Reset
	UFUNCTION(BlueprintCallable, Category = "Config")
	void ResetToDefaults(EConfigCategory Category);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void ResetAllToDefaults();

	// Info
	UFUNCTION(BlueprintCallable, Category = "Config")
	FString GetConfigurationReport() const;

public:
	UPROPERTY(BlueprintAssignable, Category = "Config|Events")
	FOnConfigChanged OnConfigChanged;

	UPROPERTY(BlueprintAssignable, Category = "Config|Events")
	FOnConfigSaved OnConfigSaved;

	UPROPERTY(BlueprintAssignable, Category = "Config|Events")
	FOnConfigLoaded OnConfigLoaded;

protected:
	UPROPERTY()
	TMap<EConfigCategory, FConfigCategoryData> Categories;

	UPROPERTY()
	TMap<EConfigCategory, FConfigCategoryData> DefaultValues;

	UPROPERTY()
	FString PresetStoragePath;

	UPROPERTY()
	FTimerHandle AutoSaveTimerHandle;

	UPROPERTY()
	TArray<FString> ValidationErrors;

	mutable FCriticalSection ConfigLock;

protected:
	FConfigValue* GetOrCreateValue(EConfigCategory Category, const FString& Key);
	const FConfigValue* GetValue(EConfigCategory Category, const FString& Key) const;
	void MarkCategoryDirty(EConfigCategory Category);
	void OnAutoSaveTick();
	void LoadDefaultValues();
	FString CategoryToString(EConfigCategory Category) const;
	EConfigCategory StringToCategory(const FString& CategoryStr) const;
	bool InternalSaveToFile(EConfigCategory Category, const FString& FullPath);
	bool InternalLoadFromFile(EConfigCategory Category, const FString& FullPath);
};
