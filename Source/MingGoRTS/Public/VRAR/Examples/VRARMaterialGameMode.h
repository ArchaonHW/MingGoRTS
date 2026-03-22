// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "VRARMaterialGameMode.generated.h"

/**
 * Example GameMode showing how to create VR/AR materials via C++ code.
 * 
 * Usage:
 * 1. Compile the project
 * 2. Set this as the default GameMode in World Settings
 * 3. Press PIE (Play in Editor) or run the game
 * 4. Check Output Log for creation results
 * 5. Verify materials in Content Browser at /Game/VR/Materials/ and /Game/AR/Materials/
 */
UCLASS()
class MINGRTS_API AVRARMaterialGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AVRARMaterialGameMode();

    // Called when the game starts
    virtual void BeginPlay() override;

    // Blueprint callable function to create all materials
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials")
    bool CreateAllVRARMaterials();

    // Create only VR materials
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials")
    bool CreateVRMaterialsOnly();

    // Create only AR materials  
    UFUNCTION(BlueprintCallable, Category = "VR/AR Materials")
    bool CreateARMaterialsOnly();

    // Get the creation report
    UFUNCTION(BlueprintPure, Category = "VR/AR Materials")
    FString GetLastCreationReport() const { return LastReport; }

protected:
    // Results tracking
    UPROPERTY()
    FString LastReport;

    UPROPERTY()
    int32 MaterialsCreatedCount;

    UPROPERTY()
    bool bCreationSuccessful;
};
