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

    virtual void BeginPlay() overHide;

    // ����動�غ�務
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // ��ܨ���動�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI(};

    // ����動�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void InideCharacterCreationUI(};

    // ����動�ا�動
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData};

    // ����動��
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled(};

    // �]�m����動�� UI ���O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserɥridget> CharacterCreationUIClass;

    // �]�m�D務UI ���O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserɥridget> MainMenuUIClass;

    // �]�m動��動池��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // ���XUI
    UPROPERTY()
    TObjectPtr<class UUserɥridget> CurrentUI;

    // 故事重要性ɥr�Ыغ�動��
    void InitializeCharacterCreationManager(};

    // 動�ب���動UI
    UUserɥridget* CreateAndShowɥridget(TSubclassOf<class UUserɥridget> ɥridgetClass};

    // ���� UI
    void RemoveCurrentUI(};

    // 動池��
    void StartGameɥrithCharacter(const FMingCharacterData& CharacterData};

    // ��動�D務
    void ReturnToMainMenu(};

    // �O動����故事重要性ɥr��動
    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData};
};
};
