#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * Controller specifically for the Main Menu.
 * Handles showing the mouse cursor, setting input mode, and spawning the menu widget.
 */
UCLASS()
class MINGWARRTS_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	// The UMG Widget class to spawn for the main menu
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<class UUserWidget> MainMenuWidgetClass;

protected:
	virtual void BeginPlay() override;

private:
	// Reference to the created widget
	UPROPERTY()
	class UUserWidget* MainMenuWidgetInstance;
};
