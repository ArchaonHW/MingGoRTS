#include "UI/MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UMainMenuWidget::StartGame(FName LevelName)
{
	// [QA Fix 2] Prevent double-clicking which causes engine hitches/crashes during level load
	SetIsEnabled(false);

	// Log the map loading process
	UE_LOG(LogTemp, Warning, TEXT("Loading level: %s"), *LevelName.ToString());

	// Open the specified game map
	UGameplayStatics::OpenLevel(this, LevelName);
}

void UMainMenuWidget::QuitGame()
{
	// [QA Fix 2] Prevent multiple quit triggers
	SetIsEnabled(false);

	UE_LOG(LogTemp, Warning, TEXT("Quitting game..."));

	// Get the player controller and gracefully quit
	if (APlayerController* SpecificPlayer = GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(this, SpecificPlayer, EQuitPreference::Quit, true);
	}
}