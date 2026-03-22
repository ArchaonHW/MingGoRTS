#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MingGoRTSCharacterCreationManager.h"
#include "MingGoRTSCharacterCreationGameMode.generated.h"

UCLASS()
class MINGGORTS_API AMingGoRTSCharacterCreationGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMingGoRTSCharacterCreationGameMode(};

    virtual void BeginPlay() override;

    // è§’è‰²?µå»ºç®¡ç???    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // é¡¯ç¤ºè§’è‰²?µå»º UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI(};

    // ?±è?è§’è‰²?µå»º UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void HideCharacterCreationUI(};

    // ?•ç?è§’è‰²?µå»ºå®Œæ?
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData};

    // ?•ç?è§’è‰²?µå»º?–æ?
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled(};

    // è¨­ç½®è§’è‰²?µå»º UI é¡åˆ¥
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> CharacterCreationUIClass;

    // è¨­ç½®ä¸»è???UI é¡åˆ¥
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> MainMenuUIClass;

    // è¨­ç½®?Šæˆ²?‹å??´æ™¯
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // ?¶å?é¡¯ç¤º??UI
    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentUI;

    // ?å??–è??²å‰µå»ºç®¡?†å™¨
    void InitializeCharacterCreationManager(};

    // ?µå»ºä¸¦é¡¯ç¤?UI
    UUserWidget* CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass};

    // ç§»é™¤?¶å? UI
    void RemoveCurrentUI(};

    // ?‹å??Šæˆ²
    void StartGameWithCharacter(const FMingCharacterData& CharacterData};

    // è¿”å?ä¸»è???    void ReturnToMainMenu(};

    // ä¿å?è§’è‰²?¸æ??°é??²å¯¦ä¾?    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData};
};

