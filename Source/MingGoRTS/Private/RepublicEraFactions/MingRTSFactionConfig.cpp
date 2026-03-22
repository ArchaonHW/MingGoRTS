// Copyright (c) 2026 MingGoRTS. All rights reserved.
// 势力数据配置化系统实现

#include "RepublicEraFactions/MingRTSFactionConfig.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonSerializer.h"
#include "Serialization/JsonWriter.h"

DEFINE_LOG_CATEGORY_STATIC(LogFactionConfig, Log, All);

UMingRTSFactionConfigManager::UMingRTSFactionConfigManager()
{
}

FString UMingRTSFactionConfigManager::GetDefaultConfigDirectory() const
{
    return FPaths::ProjectContentDir() / TEXT("FactionConfigs");
}

bool UMingRTSFactionConfigManager::LoadFactionConfigFromJSON(const FString& FilePath, FFactionConfig& OutConfig)
{
    FString JSONString;
    if (!FFileHelper::LoadFileToString(JSONString, *FilePath))
    {
        UE_LOG(LogFactionConfig, Error, TEXT("Failed to load JSON file: %s"), *FilePath);
        return false;
    }
    
    return ParseFactionConfigFromString(JSONString, OutConfig);
}

bool UMingRTSFactionConfigManager::ParseFactionConfigFromString(const FString& JSONString, FFactionConfig& OutConfig)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JSONString);
    
    if (!FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        UE_LOG(LogFactionConfig, Error, TEXT("Failed to parse JSON string"));
        return false;
    }
    
    return ConvertJsonToConfig(JsonObject, OutConfig);
}

bool UMingRTSFactionConfigManager::ConvertJsonToConfig(const TSharedPtr<FJsonObject>& JsonObject, FFactionConfig& OutConfig)
{
    OutConfig.FactionID = JsonObject->GetStringField(TEXT("FactionID"));
    OutConfig.FactionName = JsonObject->GetStringField(TEXT("FactionName"));
    OutConfig.FactionNameEnglish = JsonObject->GetStringField(TEXT("FactionNameEnglish"));
    OutConfig.Difficulty = JsonObject->GetStringField(TEXT("Difficulty"));
    OutConfig.Category = JsonObject->GetStringField(TEXT("Category"));
    OutConfig.CapitalCity = JsonObject->GetStringField(TEXT("CapitalCity"));
    OutConfig.StartingYear = JsonObject->GetIntegerField(TEXT("StartingYear"));
    OutConfig.InitialGold = JsonObject->GetIntegerField(TEXT("InitialGold"));
    OutConfig.InitialManpower = JsonObject->GetIntegerField(TEXT("InitialManpower"));
    OutConfig.InitialReputation = JsonObject->GetIntegerField(TEXT("InitialReputation"));
    
    // Parse color
    const TSharedPtr<FJsonObject>* ColorObj;
    if (JsonObject->TryGetObjectField(TEXT("FactionColor"), ColorObj))
    {
        float R = (*ColorObj)->GetNumberField(TEXT("R"));
        float G = (*ColorObj)->GetNumberField(TEXT("G"));
        float B = (*ColorObj)->GetNumberField(TEXT("B"));
        float A = (*ColorObj)->GetNumberField(TEXT("A"));
        OutConfig.FactionColor = FLinearColor(R, G, B, A);
    }
    
    // Parse string arrays
    const TArray<TSharedPtr<FJsonValue>>* TerritoriesArray;
    if (JsonObject->TryGetArrayField(TEXT("StartingTerritories"), TerritoriesArray))
    {
        for (const auto& Value : *TerritoriesArray)
        {
            OutConfig.StartingTerritories.Add(Value->AsString());
        }
    }
    
    const TArray<TSharedPtr<FJsonValue>>* AdvantagesArray;
    if (JsonObject->TryGetArrayField(TEXT("Advantages"), AdvantagesArray))
    {
        for (const auto& Value : *AdvantagesArray)
        {
            OutConfig.Advantages.Add(Value->AsString());
        }
    }
    
    const TArray<TSharedPtr<FJsonValue>>* DisadvantagesArray;
    if (JsonObject->TryGetArrayField(TEXT("Disadvantages"), DisadvantagesArray))
    {
        for (const auto& Value : *DisadvantagesArray)
        {
            OutConfig.Disadvantages.Add(Value->AsString());
        }
    }
    
    // Parse units
    const TArray<TSharedPtr<FJsonValue>>* UnitsArray;
    if (JsonObject->TryGetArrayField(TEXT("UniqueUnits"), UnitsArray))
    {
        for (const auto& Value : *UnitsArray)
        {
            const TSharedPtr<FJsonObject>* UnitObj;
            if (Value->TryGetObject(UnitObj))
            {
                FFactionUnitConfig UnitConfig;
                UnitConfig.UnitID = (*UnitObj)->GetStringField(TEXT("UnitID"));
                UnitConfig.UnitName = (*UnitObj)->GetStringField(TEXT("UnitName"));
                UnitConfig.UnitNameEnglish = (*UnitObj)->GetStringField(TEXT("UnitNameEnglish"));
                UnitConfig.Description = (*UnitObj)->GetStringField(TEXT("Description"));
                UnitConfig.UnitType = (*UnitObj)->GetStringField(TEXT("UnitType"));
                UnitConfig.AttackPower = (*UnitObj)->GetIntegerField(TEXT("AttackPower"));
                UnitConfig.DefensePower = (*UnitObj)->GetIntegerField(TEXT("DefensePower"));
                UnitConfig.MovementSpeed = (*UnitObj)->GetIntegerField(TEXT("MovementSpeed"));
                UnitConfig.HealthPoints = (*UnitObj)->GetIntegerField(TEXT("HealthPoints"));
                UnitConfig.ProductionCost = (*UnitObj)->GetIntegerField(TEXT("ProductionCost"));
                UnitConfig.UpkeepCost = (*UnitObj)->GetIntegerField(TEXT("UpkeepCost"));
                UnitConfig.TrainingTime = (*UnitObj)->GetNumberField(TEXT("TrainingTime"));
                
                const TArray<TSharedPtr<FJsonValue>>* TraitsArray;
                if ((*UnitObj)->TryGetArrayField(TEXT("Traits"), TraitsArray))
                {
                    for (const auto& TraitValue : *TraitsArray)
                    {
                        UnitConfig.Traits.Add(TraitValue->AsString());
                    }
                }
                
                OutConfig.UniqueUnits.Add(UnitConfig);
            }
        }
    }
    
    // Parse mechanics
    const TArray<TSharedPtr<FJsonValue>>* MechanicsArray;
    if (JsonObject->TryGetArrayField(TEXT("UniqueMechanics"), MechanicsArray))
    {
        for (const auto& Value : *MechanicsArray)
        {
            const TSharedPtr<FJsonObject>* MechanicObj;
            if (Value->TryGetObject(MechanicObj))
            {
                FFactionMechanicConfig MechanicConfig;
                MechanicConfig.MechanicID = (*MechanicObj)->GetStringField(TEXT("MechanicID"));
                MechanicConfig.MechanicName = (*MechanicObj)->GetStringField(TEXT("MechanicName"));
                MechanicConfig.Description = (*MechanicObj)->GetStringField(TEXT("Description"));
                MechanicConfig.MechanicType = (*MechanicObj)->GetStringField(TEXT("MechanicType"));
                MechanicConfig.EffectMultiplier = (*MechanicObj)->GetNumberField(TEXT("EffectMultiplier"));
                
                OutConfig.UniqueMechanics.Add(MechanicConfig);
            }
        }
    }
    
    // Cache the loaded config
    LoadedConfigs.Add(OutConfig.FactionID, OutConfig);
    
    UE_LOG(LogFactionConfig, Log, TEXT("Successfully loaded faction config: %s"), *OutConfig.FactionName);
    return true;
}

bool UMingRTSFactionConfigManager::SaveFactionConfigToJSON(const FString& FilePath, const FFactionConfig& Config)
{
    FString JSONString = SerializeFactionConfigToString(Config);
    
    if (FFileHelper::SaveStringToFile(JSONString, *FilePath))
    {
        UE_LOG(LogFactionConfig, Log, TEXT("Successfully saved faction config to: %s"), *FilePath);
        return true;
    }
    
    UE_LOG(LogFactionConfig, Error, TEXT("Failed to save faction config to: %s"), *FilePath);
    return false;
}

FString UMingRTSFactionConfigManager::SerializeFactionConfigToString(const FFactionConfig& Config)
{
    TSharedPtr<FJsonObject> JsonObject = ConvertConfigToJson(Config);
    
    FString OutputString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutputString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
    
    return OutputString;
}

TSharedPtr<FJsonObject> UMingRTSFactionConfigManager::ConvertConfigToJson(const FFactionConfig& Config)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    
    JsonObject->SetStringField(TEXT("FactionID"), Config.FactionID);
    JsonObject->SetStringField(TEXT("FactionName"), Config.FactionName);
    JsonObject->SetStringField(TEXT("FactionNameEnglish"), Config.FactionNameEnglish);
    JsonObject->SetStringField(TEXT("Difficulty"), Config.Difficulty);
    JsonObject->SetStringField(TEXT("Category"), Config.Category);
    JsonObject->SetStringField(TEXT("CapitalCity"), Config.CapitalCity);
    JsonObject->SetNumberField(TEXT("StartingYear"), Config.StartingYear);
    JsonObject->SetNumberField(TEXT("InitialGold"), Config.InitialGold);
    JsonObject->SetNumberField(TEXT("InitialManpower"), Config.InitialManpower);
    JsonObject->SetNumberField(TEXT("InitialReputation"), Config.InitialReputation);
    
    // Color
    TSharedPtr<FJsonObject> ColorObj = MakeShareable(new FJsonObject);
    ColorObj->SetNumberField(TEXT("R"), Config.FactionColor.R);
    ColorObj->SetNumberField(TEXT("G"), Config.FactionColor.G);
    ColorObj->SetNumberField(TEXT("B"), Config.FactionColor.B);
    ColorObj->SetNumberField(TEXT("A"), Config.FactionColor.A);
    JsonObject->SetObjectField(TEXT("FactionColor"), ColorObj);
    
    // Arrays
    TArray<TSharedPtr<FJsonValue>> TerritoriesArray;
    for (const FString& Territory : Config.StartingTerritories)
    {
        TerritoriesArray.Add(MakeShareable(new FJsonValueString(Territory)));
    }
    JsonObject->SetArrayField(TEXT("StartingTerritories"), TerritoriesArray);
    
    TArray<TSharedPtr<FJsonValue>> AdvantagesArray;
    for (const FString& Advantage : Config.Advantages)
    {
        AdvantagesArray.Add(MakeShareable(new FJsonValueString(Advantage)));
    }
    JsonObject->SetArrayField(TEXT("Advantages"), AdvantagesArray);
    
    TArray<TSharedPtr<FJsonValue>> DisadvantagesArray;
    for (const FString& Disadvantage : Config.Disadvantages)
    {
        DisadvantagesArray.Add(MakeShareable(new FJsonValueString(Disadvantage)));
    }
    JsonObject->SetArrayField(TEXT("Disadvantages"), DisadvantagesArray);
    
    // Units
    TArray<TSharedPtr<FJsonValue>> UnitsArray;
    for (const FFactionUnitConfig& Unit : Config.UniqueUnits)
    {
        TSharedPtr<FJsonObject> UnitObj = MakeShareable(new FJsonObject);
        UnitObj->SetStringField(TEXT("UnitID"), Unit.UnitID);
        UnitObj->SetStringField(TEXT("UnitName"), Unit.UnitName);
        UnitObj->SetStringField(TEXT("UnitNameEnglish"), Unit.UnitNameEnglish);
        UnitObj->SetStringField(TEXT("Description"), Unit.Description);
        UnitObj->SetStringField(TEXT("UnitType"), Unit.UnitType);
        UnitObj->SetNumberField(TEXT("AttackPower"), Unit.AttackPower);
        UnitObj->SetNumberField(TEXT("DefensePower"), Unit.DefensePower);
        UnitObj->SetNumberField(TEXT("MovementSpeed"), Unit.MovementSpeed);
        UnitObj->SetNumberField(TEXT("HealthPoints"), Unit.HealthPoints);
        UnitObj->SetNumberField(TEXT("ProductionCost"), Unit.ProductionCost);
        UnitObj->SetNumberField(TEXT("UpkeepCost"), Unit.UpkeepCost);
        UnitObj->SetNumberField(TEXT("TrainingTime"), Unit.TrainingTime);
        
        TArray<TSharedPtr<FJsonValue>> TraitsArray;
        for (const FString& Trait : Unit.Traits)
        {
            TraitsArray.Add(MakeShareable(new FJsonValueString(Trait)));
        }
        UnitObj->SetArrayField(TEXT("Traits"), TraitsArray);
        
        UnitsArray.Add(MakeShareable(new FJsonValueObject(UnitObj)));
    }
    JsonObject->SetArrayField(TEXT("UniqueUnits"), UnitsArray);
    
    // Mechanics
    TArray<TSharedPtr<FJsonValue>> MechanicsArray;
    for (const FFactionMechanicConfig& Mechanic : Config.UniqueMechanics)
    {
        TSharedPtr<FJsonObject> MechanicObj = MakeShareable(new FJsonObject);
        MechanicObj->SetStringField(TEXT("MechanicID"), Mechanic.MechanicID);
        MechanicObj->SetStringField(TEXT("MechanicName"), Mechanic.MechanicName);
        MechanicObj->SetStringField(TEXT("Description"), Mechanic.Description);
        MechanicObj->SetStringField(TEXT("MechanicType"), Mechanic.MechanicType);
        MechanicObj->SetNumberField(TEXT("EffectMultiplier"), Mechanic.EffectMultiplier);
        
        MechanicsArray.Add(MakeShareable(new FJsonValueObject(MechanicObj)));
    }
    JsonObject->SetArrayField(TEXT("UniqueMechanics"), MechanicsArray);
    
    return JsonObject;
}

TArray<FFactionConfig> UMingRTSFactionConfigManager::LoadAllFactionConfigs(const FString& ConfigDirectory)
{
    TArray<FFactionConfig> AllConfigs;
    
    TArray<FString> FoundFiles;
    IFileManager::Get().FindFiles(FoundFiles, *(ConfigDirectory / TEXT("*.json")), true, false);
    
    for (const FString& FileName : FoundFiles)
    {
        FFactionConfig Config;
        FString FullPath = ConfigDirectory / FileName;
        if (LoadFactionConfigFromJSON(FullPath, Config))
        {
            AllConfigs.Add(Config);
        }
    }
    
    UE_LOG(LogFactionConfig, Log, TEXT("Loaded %d faction configs from %s"), AllConfigs.Num(), *ConfigDirectory);
    return AllConfigs;
}

bool UMingRTSFactionConfigManager::ValidateFactionConfig(const FFactionConfig& Config, FString& OutErrorMessage)
{
    if (Config.FactionID.IsEmpty())
    {
        OutErrorMessage = TEXT("FactionID cannot be empty");
        return false;
    }
    
    if (Config.FactionName.IsEmpty())
    {
        OutErrorMessage = TEXT("FactionName cannot be empty");
        return false;
    }
    
    if (Config.UniqueUnits.Num() == 0)
    {
        OutErrorMessage = TEXT("At least one unique unit is required");
        return false;
    }
    
    if (Config.UniqueMechanics.Num() == 0)
    {
        OutErrorMessage = TEXT("At least one unique mechanic is required");
        return false;
    }
    
    // Validate unit stats are within balance ranges
    for (const FFactionUnitConfig& Unit : Config.UniqueUnits)
    {
        if (Unit.AttackPower < 40 || Unit.AttackPower > 180)
        {
            OutErrorMessage = FString::Printf(TEXT("Unit %s AttackPower (%d) out of range [40, 180]"), 
                *Unit.UnitID, Unit.AttackPower);
            return false;
        }
        
        if (Unit.MovementSpeed > 140)
        {
            OutErrorMessage = FString::Printf(TEXT("Unit %s MovementSpeed (%d) exceeds recommended maximum of 140"), 
                *Unit.UnitID, Unit.MovementSpeed);
            return false;
        }
    }
    
    // Validate mechanic multipliers
    for (const FFactionMechanicConfig& Mechanic : Config.UniqueMechanics)
    {
        if (Mechanic.EffectMultiplier < 1.0f || Mechanic.EffectMultiplier > 2.0f)
        {
            OutErrorMessage = FString::Printf(TEXT("Mechanic %s EffectMultiplier (%.2f) out of range [1.0, 2.0]"), 
                *Mechanic.MechanicID, Mechanic.EffectMultiplier);
            return false;
        }
    }
    
    return true;
}

bool UMingRTSFactionConfigManager::HotReloadFactionConfig(const FString& FactionID)
{
    FString ConfigPath = GetDefaultConfigDirectory() / FactionID + TEXT(".json");
    
    FFactionConfig Config;
    if (LoadFactionConfigFromJSON(ConfigPath, Config))
    {
        LoadedConfigs.Add(FactionID, Config);
        UE_LOG(LogFactionConfig, Log, TEXT("Hot reloaded faction config: %s"), *FactionID);
        return true;
    }
    
    return false;
}

bool UMingRTSFactionConfigManager::ExportSampleConfig(const FString& FilePath, const FString& FactionID)
{
    FFactionConfig SampleConfig;
    SampleConfig.FactionID = FactionID;
    SampleConfig.FactionName = TEXT("示例势力");
    SampleConfig.FactionNameEnglish = TEXT("Sample Faction");
    SampleConfig.Difficulty = TEXT("Normal");
    SampleConfig.Category = TEXT("RegionalWarlord");
    SampleConfig.FactionColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
    SampleConfig.CapitalCity = TEXT("示例首都");
    SampleConfig.StartingTerritories.Add(TEXT("领土1"));
    SampleConfig.StartingTerritories.Add(TEXT("领土2"));
    SampleConfig.StartingYear = 1912;
    SampleConfig.InitialGold = 1000;
    SampleConfig.InitialManpower = 500;
    SampleConfig.InitialReputation = 50;
    SampleConfig.Advantages.Add(TEXT("优势1"));
    SampleConfig.Advantages.Add(TEXT("优势2"));
    SampleConfig.Disadvantages.Add(TEXT("劣势1"));
    SampleConfig.Disadvantages.Add(TEXT("劣势2"));
    
    // Add sample unit
    FFactionUnitConfig SampleUnit;
    SampleUnit.UnitID = TEXT("SampleUnit");
    SampleUnit.UnitName = TEXT("示例单位");
    SampleUnit.UnitNameEnglish = TEXT("Sample Unit");
    SampleUnit.Description = TEXT("这是一个示例单位");
    SampleUnit.UnitType = TEXT("Infantry");
    SampleUnit.AttackPower = 100;
    SampleUnit.DefensePower = 100;
    SampleUnit.MovementSpeed = 100;
    SampleUnit.HealthPoints = 100;
    SampleUnit.ProductionCost = 100;
    SampleUnit.UpkeepCost = 10;
    SampleUnit.TrainingTime = 10.0f;
    SampleUnit.Traits.Add(TEXT("Infantry"));
    SampleConfig.UniqueUnits.Add(SampleUnit);
    
    // Add sample mechanic
    FFactionMechanicConfig SampleMechanic;
    SampleMechanic.MechanicID = TEXT("SampleMechanic");
    SampleMechanic.MechanicName = TEXT("示例机制");
    SampleMechanic.Description = TEXT("这是一个示例机制");
    SampleMechanic.MechanicType = TEXT("Military");
    SampleMechanic.EffectMultiplier = 1.2f;
    SampleConfig.UniqueMechanics.Add(SampleMechanic);
    
    return SaveFactionConfigToJSON(FilePath, SampleConfig);
}
