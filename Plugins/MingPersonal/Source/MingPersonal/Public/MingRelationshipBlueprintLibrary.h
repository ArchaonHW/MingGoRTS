#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MingRelationshipManager.h"
#include "MingRelationshipBlueprintLibrary.generated.h"

/**
 * ???X?羘??╰??Blueprint ??计?? * ???X?獽XBlueprint ??ㄓ砐????玒?X??X?
 */
UCLASS()
class MINGPERSONAL_API UMingRelationshipBlueprintLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    // ???X???恨?X?龟??    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static UMingRelationshipManager* GetRelationshipManager();

    // ????恨?X?计
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void UpdateCharacterRelationship(const FString& CharacterID, float ChangeAmount, const FString& Reason);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static FRelationshipData GetCharacterRelationship(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static ERelationshipType GetRelationshipType(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static float GetRelationshipValue(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void AddSharedMemory(const FString& CharacterID, const FString& Memory);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static TArray<FString> GetSharedMemories(const FString& CharacterID};

    // ????恨?X?计
    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    static void UpdateRegionReputation(const FString& RegionID, float ChangeAmount, const FString& Reason);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static FReputationData GetRegionReputation(const FString& RegionID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static EReputationLevel GetReputationLevel(const FString& RegionID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static float GetReputationScore(const FString& RegionID);

    // ??栏紇臫??计
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static TArray<FString> GetDialogueOptions(const FString& CharacterID};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static float GetPriceModifier(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static bool CanAcceptQuest(const FString& QuestID, const FString& RegionID);

    // ???X??X?计
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void ProcessInteractionEffect(const FString& CharacterID, const FString& RegionID, 
                                       float RelationshipChange, float ReputationChange, const FString& Reason);

    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void ProcessBatchInteractions(const TArray<FInteractionEffect>& Interactions);

    // 獽??计
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static bool IsFriend(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static bool IsEnemy(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static bool IsNeutral(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static bool HasGoodReputation(const FString& RegionID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static bool HasBadReputation(const FString& RegionID);

    // 才﹃?X?ㄧX    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static FString GetRelationshipTypeString(ERelationshipType RelationshipType);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static FString GetReputationLevelString(EReputationLevel ReputationLevel);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static FString GetRelationshipDescription(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static FString GetReputationDescription(const FString& RegionID);

    // 参?X?计
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static int32 GetTotalRelationshipCount();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static int32 GetFriendCount();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static int32 GetEnemyCount();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static int32 GetKnownRegionCount();

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static TArray<FString> GetAllKnownRegions(};

    // 蔼?X?高??计
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static TArray<FString> GetCharactersByRelationshipType(ERelationshipType RelationshipType};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static TArray<FString> GetCharactersByRelationshipRange(float MinValue, float MaxValue};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static TArray<FString> GetRegionsByReputationLevel(EReputationLevel ReputationLevel};

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static TArray<FString> GetRegionsByReputationRange(float MinScore, float MaxScore};

    // 喷?X?计
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Relationship System")
    static bool IsValidCharacterID(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Reputation System")
    static bool IsValidRegionID(const FString& RegionID);

    // 秸刚??计
    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void LogAllRelationships();

    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    static void LogAllReputations();

    UFUNCTION(BlueprintCallable, Category = "Relationship System")
    static void LogRelationshipStatus(const FString& CharacterID);

    UFUNCTION(BlueprintCallable, Category = "Reputation System")
    static void LogReputationStatus(const FString& RegionID};
);

