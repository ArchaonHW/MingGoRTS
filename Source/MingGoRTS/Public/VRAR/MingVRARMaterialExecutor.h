// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MingVRARMaterialExecutor.generated.h"

/**
 * A simple executor actor for creating VR/AR materials via C++.
 * Drag this actor into any level and call ExecuteMaterialCreation() from Blueprint or C++.
 */
UCLASS(BlueprintType)
class MINGRTS_API AMingVRARMaterialExecutor : public AActor
{
    GENERATED_BODY()

public:
    AMingVRARMaterialExecutor();

    // Execute material creation (call this from Blueprint or C++)
    UFUNCTION(BlueprintCallable, Category = "VR/AR Material Creation", CallInEditor)
    bool ExecuteMaterialCreation();

    // Create only VR materials
    UFUNCTION(BlueprintCallable, Category = "VR/AR Material Creation", CallInEditor)
    bool CreateVRMaterialsOnly();

    // Create only AR materials
    UFUNCTION(BlueprintCallable, Category = "VR/AR Material Creation", CallInEditor)
    bool CreateARMaterialsOnly();

    // Get the last execution result
    UFUNCTION(BlueprintPure, Category = "VR/AR Material Creation")
    FString GetLastResult() const { return LastExecutionResult; }

    // Progress tracking
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR/AR Material Creation")
    int32 TotalMaterialsToCreate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR/AR Material Creation")
    int32 MaterialsCreated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "VR/AR Material Creation")
    bool bIsCreatingMaterials;

protected:
    UPROPERTY()
    FString LastExecutionResult;

    virtual void BeginPlay() override;
};
