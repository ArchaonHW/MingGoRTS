#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSRelationshipNetwork.generated.h"

UENUM(BlueprintType)
enum class EMingGameRelationshipType : uint8
{
    Family,         UMETA(DisplayName = "Ë¶™Â±¨?ú‰?"),
    Friend,         UMETA(DisplayName = "?ãÂ??ú‰?"),
    Mentor,         UMETA(DisplayName = "Â∏´Â??ú‰?"),
    Rival,          UMETA(DisplayName = "Â∞çÊ??ú‰?"),
    Ally,           UMETA(DisplayName = "?üÂ??ú‰?"),
    Enemy,          UMETA(DisplayName = "?µ‰∫∫?ú‰?"),
    Colleague,      UMETA(DisplayName = "?å‰??ú‰?"),
    Subordinate,    UMETA(DisplayName = "‰∏ãÂ±¨?ú‰?"),
    Superior,       UMETA(DisplayName = "‰∏äÂè∏?ú‰?"),
    Neutral,        UMETA(DisplayName = "‰∏≠Á??ú‰?")
};

USTRUCT(BlueprintType)
struct FMingRelationshipNode
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    FString CharacterId;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    TMap<EMingGameRelationshipType, float> Relationships;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    float Reputation;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    bool IsActive;

    FMingRelationshipNode()
    {
        CharacterId = TEXT(""};
        CharacterName = TEXT(""};
        Relationships.Empty(};
        Reputation = 0.0f;
        IsActive = true;
    }
};

UCLASS(BlueprintType, Blueprintable)
class MINGGORTS_API UMingGoRTSRelationshipNetwork : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSRelationshipNetwork(};

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void AddRelationshipNode(const FString& CharacterId, const FString& CharacterName};

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void SetRelationship(const FString& FromCharacter, const FString& ToCharacter, EMingGameRelationshipType RelationshipType, float Strength};

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float GetRelationshipStrength(const FString& FromCharacter, const FString& ToCharacter, EMingGameRelationshipType RelationshipType) const;

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FMingRelationshipNode> GetAllRelationshipNodes() const;

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FString> GetConnectedCharacters(const FString& CharacterId) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void UpdateReputation(const FString& CharacterId, float Delta};

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    float GetReputation(const FString& CharacterId) const;

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void RemoveCharacter(const FString& CharacterId};

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void ClearAllRelationships(};

protected:
    UPROPERTY(BlueprintReadOnly, Category = "Relationship Network")
    TMap<FString, FMingRelationshipNode> RelationshipNodes;

private:
    bool ValidateCharacterId(const FString& CharacterId) const;
    void OnRelationshipChanged(const FString& FromCharacter, const FString& ToCharacter, EMingGameRelationshipType RelationshipType, float OldStrength, float NewStrength};
};

