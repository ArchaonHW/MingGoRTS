// Sample C++ file for testing Code Review workflow
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

/**
 * Sample UI Manager class for testing code review functionality
 * This class demonstrates various code patterns and potential issues
 */
class MINGUI_API UMingUIManager : public UObject
{
    GENERATED_BODY()

public:
    UMingUIManager();

    /**
     * Initializes the UI system
     * @param bEnableDebug - Whether to enable debug mode
     * @return true if initialization succeeded
     */
    bool InitializeUI(bool bEnableDebug = false);

    /**
     * Updates the UI elements
     * This function should be called every frame
     */
    void UpdateUI();

    /**
     * Processes user input
     * @param InputAction - The action to process
     */
    void ProcessInput(FString InputAction);

private:
    bool bIsInitialized;
    float LastUpdateTime;
    
    // Helper function for validation
    bool ValidateInput(const FString& Input);
};
