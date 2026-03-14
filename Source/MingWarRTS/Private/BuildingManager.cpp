#include "BuildingManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

void UBuildingManager::LoadAllBuildingData()
{
    const FString FilePath = FPaths::ProjectContentDir() + TEXT("Data/Buildings.json");
    FString JsonString;

    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load Buildings.json from: %s"), *FilePath);
        return;
    }

    TSharedPtr<FJsonValue> RootJsonValue;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, RootJsonValue) || !RootJsonValue.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse Buildings.json."));
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* JsonArray;
    if (!RootJsonValue->TryGetArray(JsonArray))
    {
        UE_LOG(LogTemp, Error, TEXT("Buildings.json root is not an array."));
        return;
    }

    for (const TSharedPtr<FJsonValue>& BuildingValue : *JsonArray)
    {
        TSharedPtr<FJsonObject> BuildingObject = BuildingValue->AsObject();
        if (BuildingObject.IsValid())
        {
            FBuildingData NewBuilding;
            NewBuilding.BuildingID = FName(*BuildingObject->GetStringField("BuildingID"));
            NewBuilding.Name = BuildingObject->GetStringField("Name");
            NewBuilding.Description = BuildingObject->GetStringField("Description");
            NewBuilding.BuildTimeSeconds = BuildingObject->GetNumberField("BuildTimeSeconds");

            // Parse ConstructionCost
            const TSharedPtr<FJsonObject>* CostObject;
            if (BuildingObject->TryGetObjectField("ConstructionCost", CostObject))
            {
                for (auto& CostItem : (*CostObject)->Values)
                {
                    NewBuilding.ConstructionCost.Add(FName(*CostItem.Key), CostItem.Value->AsNumber());
                }
            }

            // Parse ResourceProductionRate
            const TSharedPtr<FJsonObject>* ProductionObject;
            if (BuildingObject->TryGetObjectField("ResourceProductionRate", ProductionObject))
            {
                for (auto& ProductionItem : (*ProductionObject)->Values)
                {
                    NewBuilding.ResourceProductionRate.Add(FName(*ProductionItem.Key), ProductionItem.Value->AsNumber());
                }
            }

            // Parse UnlocksUnits
            const TArray<TSharedPtr<FJsonValue>>* UnlocksArray = nullptr;
            if (BuildingObject->TryGetArrayField("UnlocksUnits", UnlocksArray))
            {
                for (const TSharedPtr<FJsonValue>& UnlockValue : *UnlocksArray)
                {
                    NewBuilding.UnlocksUnits.Add(FName(*UnlockValue->AsString()));
                }
            }

            AllBuildingsData.Add(NewBuilding.BuildingID, NewBuilding);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully loaded %d buildings."), AllBuildingsData.Num());
}

bool UBuildingManager::HasBuilding(FName BuildingID) const
{
    return PlayerOwnedBuildings.Contains(BuildingID);
}