#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSRelationshipNetwork.generated.h"

UENUM(BlueprintType)
enum class EMingGameRelationshipType : uint8
{
    Family,         UMETA(DisplayName = "親屬?��?"),
    Friend,         UMETA(DisplayName = "?��X��?"),
    Mentor,         UMETA(DisplayName = "師�X��?"),
    Rival,          UMETA(DisplayName = "對�X��?"),
    Ally,           UMETA(DisplayName = "?��X��?"),
    Enemy,          UMETA(DisplayName = "?�人?��?"),
    Colleague,      UMETA(DisplayName = "?��X��?"),
    Subordinate,    UMETA(DisplayName = "下屬?��?"),
    Superior,       UMETA(DisplayName = "上司?��?"),
    Neutral,        UMETA(DisplayName = "中�X��?")
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

