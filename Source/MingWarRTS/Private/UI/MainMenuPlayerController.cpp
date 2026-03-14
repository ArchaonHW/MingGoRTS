#include "UI/MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Show the mouse cursor
	bShowMouseCursor = true;

	FInputModeUIOnly InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	// Create and add the Main Menu Widget to the screen if a class is set
	if (MainMenuWidgetClass)
	{
		MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
		if (MainMenuWidgetInstance)
		{
			MainMenuWidgetInstance->AddToViewport();
			// [QA Fix 1] Set focus immediately so player doesn't have to click the screen first to activate buttons
			InputModeData.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuWidgetClass is not set in the PlayerController Blueprint!"));
	}

	// Apply the input mode
	SetInputMode(InputModeData);
}