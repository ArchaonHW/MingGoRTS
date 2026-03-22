#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingGoRTSCharacter.h"
#include "MingGoRTSHistoricalCharacters.h"
#include "MingGoRTSRelationshipNetwork.generated.h"

UENUM(BlueprintType)
enum class EMingGameRelationshipType: uint8 {
    Family, UMETA(DisplayName = "Family"),
    Friend, UMETA(DisplayName = "Friend"),
    Mentor, UMETA(DisplayName = "Mentor"),
    Rival, UMETA(DisplayName = "Rival"),
    Ally, UMETA(DisplayName = "Ally"),
    Enemy, UMETA(DisplayName = "Enemy"),
    Colleague, UMETA(DisplayName = "Colleague"),
    Subordinate, UMETA(DisplayName = "Subordinate"),
    Superior, UMETA(DisplayName = "Superior"),
    Neutral, UMETA(DisplayName = "Neutral")
};

/**
 * Relationship data structure
 */
USTRUCT(BlueprintType)
struct FMingGameRelationship
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    FString CharacterA;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    FString CharacterB;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    EMingGameRelationshipType RelationshipType;

    UPROPERTY(BlueprintReadOnly, Category = "Relationship")
    float Strength;

    FMingGameRelationship()
        : CharacterA(TEXT(""))
        , CharacterB(TEXT(""))
        , RelationshipType(EMingGameRelationshipType::Neutral)
        , Strength(0.0f)
    {}
};

/**
 * MingGoRTS Relationship Network System
 */
UCLASS(BlueprintType, Blueprintable)
class MINGRTS_API UMingGoRTSRelationshipNetwork : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSRelationshipNetwork();

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    void InitializeRelationshipNetwork();

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    FMingGameRelationship GetRelationship(const FString& CharacterA, const FString& CharacterB);

    UFUNCTION(BlueprintCallable, Category = "Relationship Network")
    bool SetRelationship(const FString& CharacterA, const FString& CharacterB, EMingGameRelationshipType Type, float Strength);

    UFUNCTION(BlueprintPure, Category = "Relationship Network")
    TArray<FMingGameRelationship> GetCharacterRelationships(const FString& CharacterName);

protected:
    UPROPERTY()
    TMap<FString, FMingGameRelationship> RelationshipMap;

    void InitializeDefaultRelationships();
};
