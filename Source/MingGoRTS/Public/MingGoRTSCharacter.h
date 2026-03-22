#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MingGoRTSCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterBackground : uint8
{
    MilitaryAcademy,
    WarlordSon,
    Revolutionary,
    ScholarOfficial,
    Merchant,
    CommonSoldier
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Health;

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Strength;

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Intelligence;
};

