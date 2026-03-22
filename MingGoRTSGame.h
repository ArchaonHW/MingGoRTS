#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * MingGoRTS game module main header file
 */
UCLASS(BlueprintType)
class MINGGORTS_API UMingGoRTSGame : public UObject
{
    GENERATED_BODY()

public:
    UMingGoRTSGame();

    UFUNCTION(BlueprintCallable, Category = "Game")
    void InitializeGame();
};