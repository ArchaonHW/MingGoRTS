#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * Base C++ class for the Main Menu UI Widget.
 * Contains core logic that can be hooked up to UMG buttons in Editor.
 */
UCLASS()
class MINGWARRTS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Function to start the game and load a level
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void StartGame(FName LevelName);

	// Function to gracefully quit the game
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void QuitGame();
};