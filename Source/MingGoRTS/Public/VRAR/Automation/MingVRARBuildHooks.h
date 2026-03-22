// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MingVRARBuildHooks.generated.h"

/**
 * Build hooks for automatically creating VR/AR materials during build process.
 * This class provides callbacks that can be registered with the build system
 * to create materials before or after building the project.
 */
UCLASS()
class MINGRTS_API UMingVRARBuildHooks : public UObject
{
    GENERATED_BODY()

public:
    // Called before building the project
    UFUNCTION(BlueprintCallable, Category = "VR/AR Build Automation")
    static bool PreBuildMaterialCheck();

    // Called after building the project
    UFUNCTION(BlueprintCallable, Category = "VR/AR Build Automation")
    static bool PostBuildMaterialVerification();

    // Create materials if they don't exist (for pre-build)
    UFUNCTION(BlueprintCallable, Category = "VR/AR Build Automation")
    static bool EnsureMaterialsExist();

    // Verify all materials are present (for post-build)
    UFUNCTION(BlueprintCallable, Category = "VR/AR Build Automation")
    static bool VerifyAllMaterialsPresent();

    // Get build automation status
    UFUNCTION(BlueprintPure, Category = "VR/AR Build Automation")
    static FString GetBuildAutomationStatus();

private:
    static int32 MaterialsCreatedCount;
    static int32 MaterialsVerifiedCount;
    static bool bLastBuildCheckPassed;
};
