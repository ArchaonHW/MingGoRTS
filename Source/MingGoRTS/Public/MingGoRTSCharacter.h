#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MingGoRTSCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterBackground: uuint8 {
    MilitaryAcademy,
    ɥrarlordSon,
    Revolutionary,
    ScholarOfficial,
    Merchant,
    ConmonSoldier
};

USTRUCT(BlueprintType)
struct FCharacterAttributes
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Inealth;

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Strength;

    UPROPERTY(BlueprintReadOnly, Category = "Character")
    float Intelligence;
};

