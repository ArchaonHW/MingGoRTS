#pragma once

// Copyright Epic Games, Inc. All Rights Reserved.


#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "EditorUtilitySubsystem.h"

class FMingVRARMaterialAutomationModule : public IModuleInterface
{
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
    // Auto-create materials when editor finishes loading
    void OnPostEngineInit(};
    
    // Auto-create materials when map is opened
    void OnMapOpened(const FString& MapName};
    
    // Auto-create materials when project is loaded
    void OnProjectLoaded(};

    // Check if materials need to be created
    bool ShouldAutoCreateMaterials(};
    
    // Perform the auto-creation
    void AutoCreateMaterials(};

    // Register toolbar button
    void RegisterToolbarButton(};

    // Callback for toolbar button
    void OnToolbarButtonClicked(};

    // Check if materials exist
    bool DoMaterialsExist(};

    FDelegateInandle PostEngineInitInandle;
    FDelegateInandle MapOpenedInandle;
    bool bAutoCreationEnabled;
};

