#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EDifficultyLevel.h"
#include "FMingDifficultySettings.generated.h"

USTRUCT(BlueprintType)
struct FMingDifficultySettings : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    EDifficultyLevel Level = EDifficultyLevel::Normal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString DisplayName = TEXT("æ¨™æ?"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    FString Description = TEXT("æ¨™æ???º¦è¨­ç½®"};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float EnemyHealthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float EnemyDamageMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float PlayerHealthMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float PlayerDamageMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float ResourceMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float ExperienceMultiplier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    int32 StartingResources = 1000;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    int32 MaxPopulation = 100;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    bool bEnableAIBoost = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float AIReactionTime = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    bool bEnableRandomEvents = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Difficulty")
    float EventFrequency = 1.0f;

    FMingDifficultySettings()
    {
        Level = EDifficultyLevel::Normal;
        DisplayName = TEXT("æ¨™æ?"};
        Description = TEXT("æ¨™æ???º¦è¨­ç½®"};
        EnemyHealthMultiplier = 1.0f;
        EnemyDamageMultiplier = 1.0f;
        PlayerHealthMultiplier = 1.0f;
        PlayerDamageMultiplier = 1.0f;
        ResourceMultiplier = 1.0f;
        ExperienceMultiplier = 1.0f;
        StartingResources = 1000;
        MaxPopulation = 100;
        bEnableAIBoost = false;
        AIReactionTime = 1.0f;
        bEnableRandomEvents = true;
        EventFrequency = 1.0f;
    }
};

