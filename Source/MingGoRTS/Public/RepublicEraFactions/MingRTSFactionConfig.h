// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力数据配置化系统 - Faction Data Configuration System

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Containers/Map.h"
#include "MingRTSFactionConfig.generated.h"

/**
 * 势力单位配置结构 (JSON序列化)
 */
USTRUCT(BlueprintType)
struct FFactionUnitConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString UnitID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString UnitName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString UnitNameEnglish;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString UnitType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 AttackPower = 100;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 DefensePower = 100;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 MovementSpeed = 100;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 HealthPoints = 100;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 ProductionCost = 100;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 UpkeepCost = 10;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    float TrainingTime = 10.0f;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FString> Traits;
};

/**
 * 势力机制配置结构
 */
USTRUCT(BlueprintType)
struct FFactionMechanicConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString MechanicID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString MechanicName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString Description;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString MechanicType;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    float EffectMultiplier = 1.0f;
};

/**
 * 势力完整配置结构
 */
USTRUCT(BlueprintType)
struct FFactionConfig
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString FactionID;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString FactionName;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString FactionNameEnglish;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString Difficulty;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString Category;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FLinearColor FactionColor = FLinearColor::White;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString CapitalCity;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FString> StartingTerritories;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 StartingYear = 1912;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 InitialGold = 1000;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 InitialManpower = 500;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    int32 InitialReputation = 50;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FString> Advantages;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FString> Disadvantages;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FString> RecommendedStrategies;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    FString PlayerTypeRecommendation;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FFactionUnitConfig> UniqueUnits;
    
    UPROPERTY(BlueprintReadWrite, Category = "Faction|Config")
    TArray<FFactionMechanicConfig> UniqueMechanics;
};

/**
 * 势力配置管理器 - JSON配置加载系统
 */
UCLASS(ClassGroup = (MingRTS), Blueprintable)
class MINGRTS_API UMingRTSFactionConfigManager : public UObject
{
    GENERATED_BODY()
    
public:
    UMingRTSFactionConfigManager();
    
    /** 从JSON文件加载势力配置 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool LoadFactionConfigFromJSON(const FString& FilePath, FFactionConfig& OutConfig);
    
    /** 保存势力配置到JSON文件 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool SaveFactionConfigToJSON(const FString& FilePath, const FFactionConfig& Config);
    
    /** 从字符串解析JSON配置 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool ParseFactionConfigFromString(const FString& JSONString, FFactionConfig& OutConfig);
    
    /** 将配置序列化为JSON字符串 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    FString SerializeFactionConfigToString(const FFactionConfig& Config);
    
    /** 批量加载所有势力配置 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    TArray<FFactionConfig> LoadAllFactionConfigs(const FString& ConfigDirectory);
    
    /** 获取配置目录路径 */
    UFUNCTION(BlueprintPure, Category = "Faction|Config")
    FString GetDefaultConfigDirectory() const;
    
    /** 验证配置有效性 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool ValidateFactionConfig(const FFactionConfig& Config, FString& OutErrorMessage);
    
    /** 热重载配置 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool HotReloadFactionConfig(const FString& FactionID);
    
    /** 导出示例配置文件 */
    UFUNCTION(BlueprintCallable, Category = "Faction|Config")
    bool ExportSampleConfig(const FString& FilePath, const FString& FactionID);

private:
    /** 已加载的配置缓存 */
    UPROPERTY()
    TMap<FString, FFactionConfig> LoadedConfigs;
    
    /** 将JSON对象转换为配置结构 */
    bool ConvertJsonToConfig(const TSharedPtr<FJsonObject>& JsonObject, FFactionConfig& OutConfig);
    
    /** 将配置结构转换为JSON对象 */
    TSharedPtr<FJsonObject> ConvertConfigToJson(const FFactionConfig& Config);
};
