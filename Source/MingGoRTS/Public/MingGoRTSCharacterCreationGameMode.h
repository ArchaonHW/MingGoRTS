#pragma once


#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSCharacterCreationManager.h"
#include "MingGoRTSCharacterCreationGameMode.generated.h"

UCLASS()
class MINGRTS_API AMingGoRTSCharacterCreationGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSCharacterCreationGameMode(};

    virtual void BeginPlay() override;

    // ï¿½ï¿½ï¿½ï¿½?•ï¿½Øºï¿½å?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // ï¿½ï¿½Ü¨ï¿½ï¿½ï¿½å?ï¿½ï¿½ UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI(};

    // ï¿½ï¿½ï¿½ï¿½?•ï¿½ï¿?UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void InideCharacterCreationUI(};

    // ï¿½ï¿½ï¿½ï¿½?•ï¿½Ø§ï¿½å?
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData};

    // ï¿½ï¿½ï¿½ï¿½?•ï¿½ï¿?
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled(};

    // ï¿½]ï¿½mï¿½ï¿½ï¿½ï¿½?•ï¿½ï¿?UI ï¿½ï¿½ï¿½O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserÉ¥ridget> CharacterCreationUIClass;

    // ï¿½]ï¿½mï¿½D?™UI ï¿½ï¿½ï¿½O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserÉ¥ridget> MainMenuUIClass;

    // ï¿½]ï¿½m?•ï¿½ï¿½å?æ± ï¿½ï¿?
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // ï¿½ï¿½ï¿½XUI
    UPROPERTY()
    TObjectPtr<class UUserÉ¥ridget> CurrentUI;

    // ?…ä??è??§É¥rï¿½Ğ«Øºï¿½?•ï¿½ï¿?
    void InitializeCharacterCreationManager(};

    // ?•ï¿½Ø¨ï¿½ï¿½ï¿½å?UI
    UUserÉ¥ridget* CreateAndShowÉ¥ridget(TSubclassOf<class UUserÉ¥ridget> É¥ridgetClass};

    // ï¿½ï¿½ï¿½ï¿½ UI
    void RemoveCurrentUI(};

    // ?•æ?ï¿½ï¿½
    void StartGameÉ¥rithCharacter(const FMingCharacterData& CharacterData};

    // ï¿½ï¿½?•ï¿½D??
    void ReturnToMainMenu(};

    // ï¿½O?•ï¿½ï¿½ï¿½ï¿½æ?äº‹é?è¦æ€§É¥rï¿½ï¿½??
    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData};
};
};

