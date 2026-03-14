#include "IdeaManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

#include "IdeaManager.h"
#include "Misc/Paths.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "Kismet/GameplayStatics.h"
#include "ResourceManager.h"
#include "ModifierManager.h"

UIdeaManager::UIdeaManager()
{
    PrimaryComponentTick.bCanEverTick = false;
}

// Helper to get ResourceManager
UResourceManager* GetResourceManager(UActorComponent* Caller)
{
    if (AActor* Owner = Caller->GetOwner())
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(Owner))
        {
            return PlayerController->FindComponentByClass<UResourceManager>();
        }
    }
    return nullptr;
}

// Helper to get ModifierManager
UModifierManager* GetModifierManager(UActorComponent* Caller)
{
    if (AActor* Owner = Caller->GetOwner())
    {
        if (APlayerController* PlayerController = Cast<APlayerController>(Owner))
        {
            return PlayerController->FindComponentByClass<UModifierManager>();
        }
    }
    return nullptr;
}

void UIdeaManager::LoadAllIdeaGroups()
{
    const FString FilePath = FPaths::ProjectContentDir() + TEXT("Data/IdeaGroups.json");
    FString JsonString;

    if (!FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to load IdeaGroups.json from: %s"), *FilePath);
        return;
    }

    TSharedPtr<FJsonValue> RootJsonValue;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (!FJsonSerializer::Deserialize(Reader, RootJsonValue) || !RootJsonValue.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse IdeaGroups.json."));
        return;
    }

    const TArray<TSharedPtr<FJsonValue>>* JsonArray;
    if (!RootJsonValue->TryGetArray(JsonArray))
    {
        UE_LOG(LogTemp, Error, TEXT("JSON root is not a valid array."));
        return;
    }


    for (const TSharedPtr<FJsonValue>& GroupValue : *JsonArray)
    {
        TSharedPtr<FJsonObject> GroupObject = GroupValue->AsObject();
        if (GroupObject.IsValid())
        {
            FIdeaGroup IdeaGroup;
            IdeaGroup.GroupID = FName(*GroupObject->GetStringField("GroupID"));
            IdeaGroup.GroupName = GroupObject->GetStringField("GroupName");
            IdeaGroup.Description = GroupObject->GetStringField("Description");

            const TArray<TSharedPtr<FJsonValue>>* IdeasArray = nullptr;
            if (GroupObject->TryGetArrayField("Ideas", IdeasArray))
            {
                for (const TSharedPtr<FJsonValue>& IdeaValue : *IdeasArray)
                {
                    TSharedPtr<FJsonObject> IdeaObject = IdeaValue->AsObject();
                    if (IdeaObject.IsValid())
                    {
                        FIdea Idea;
                        Idea.IdeaID = FName(*IdeaObject->GetStringField("IdeaID"));
                        Idea.Name = IdeaObject->GetStringField("Name");
                        Idea.Cost = IdeaObject->GetIntegerField("Cost");
                        Idea.Description = IdeaObject->GetStringField("Description");

                        const TSharedPtr<FJsonObject>* ModifiersObject;
                        if (IdeaObject->TryGetObjectField("Modifiers", ModifiersObject))
                        {
                            for (auto& Modifier : (*ModifiersObject)->Values)
                            {
                                Idea.Modifiers.Add(FName(*Modifier.Key), Modifier.Value->AsNumber());
                            }
                        }
                        IdeaGroup.Ideas.Add(Idea);
                    }
                }
            }
            AllIdeaGroups.Add(IdeaGroup);
        }
    }

    UE_LOG(LogTemp, Log, TEXT("Successfully loaded %d idea groups."), AllIdeaGroups.Num());
}

void UIdeaManager::ChooseIdeaGroup(FName GroupID)
{
    // Find the chosen group
    for (const FIdeaGroup& Group : AllIdeaGroups)
    {
        if (Group.GroupID == GroupID)
        {
            PlayerChosenGroup = Group;
            UE_LOG(LogTemp, Log, TEXT("Player chose idea group: %s"), *GroupID.ToString());
            return;
        }
    }
    UE_LOG(LogTemp, Warning, TEXT("Attempted to choose non-existent idea group: %s"), *GroupID.ToString());
}

bool UIdeaManager::UnlockIdea(FName IdeaID)
{
    if (PlayerChosenGroup.GroupID.IsNone())
    {
        UE_LOG(LogTemp, Warning, TEXT("Cannot unlock idea; no idea group has been chosen yet."));
        return false;
    }

    // Check if already unlocked
    if (UnlockedIdeas.Contains(IdeaID))
    {
        UE_LOG(LogTemp, Log, TEXT("Idea '%s' is already unlocked."), *IdeaID.ToString());
        return true; // Already unlocked is considered a success for idempotence
    }

    // Find the idea within the chosen group
    const FIdea* FoundIdea = nullptr;
    for (const FIdea& Idea : PlayerChosenGroup.Ideas)
    {
        if (Idea.IdeaID == IdeaID)
        {
            FoundIdea = &Idea;
            break;
        }
    }

    if (!FoundIdea)
    {
        UE_LOG(LogTemp, Warning, TEXT("Attempted to unlock non-existent idea '%s' in chosen group."), *IdeaID.ToString());
        return false;
    }

    // Get ResourceManager and ModifierManager instances
    UResourceManager* ResourceManager = GetResourceManager(this);
    UModifierManager* ModifierManager = GetModifierManager(this);

    if (!ResourceManager || !ModifierManager)
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to get ResourceManager or ModifierManager. Cannot unlock idea."));
        return false;
    }

    // Check if player has enough political power (assuming resource ID for political power is 'res_political_power')
    const FName PoliticalPowerID = FName(TEXT("res_political_power"));
    int32* CurrentPoliticalPower = ResourceManager->PlayerResources.Find(PoliticalPowerID);

    if (!CurrentPoliticalPower || *CurrentPoliticalPower < FoundIdea->Cost)
    {
        UE_LOG(LogTemp, Warning, TEXT("Not enough political power to unlock idea '%s'. Cost: %d, Current: %d"),
            *IdeaID.ToString(), FoundIdea->Cost, CurrentPoliticalPower ? *CurrentPoliticalPower : 0);
        return false;
    }

    // Deduct cost
    *CurrentPoliticalPower -= FoundIdea->Cost;
    UE_LOG(LogTemp, Log, TEXT("Deducted %d political power for idea '%s'. Remaining: %d"),
        FoundIdea->Cost, *IdeaID.ToString(), *CurrentPoliticalPower);

    // Apply modifiers
    for (auto& Modifier : FoundIdea->Modifiers)
    {
        ModifierManager->ApplyModifier(Modifier.Key, Modifier.Value);
    }

    // Add to unlocked ideas
    UnlockedIdeas.Add(IdeaID);
    UE_LOG(LogTemp, Log, TEXT("Successfully unlocked idea: %s"), *IdeaID.ToString());

    // Broadcast the event so other systems (like TriggerManager) know
    OnIdeaUnlockedEvent.Broadcast(IdeaID);

    return true;
}