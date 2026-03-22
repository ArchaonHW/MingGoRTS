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
    AMingGoRTSCharacterCreationGameMode();

    virtual void BeginPlay() override;

    // ����??�غ�?X
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // ��ܨ���??�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI();

    // 摧毀����??�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void HideCharacterCreationUI();

    // 摧毀����??�ا�??
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData);

    // 摧毀����??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled();

    // �]�m����??�� UI ���O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> CharacterCreationUIClass;

    // �]�m�D?XUI ���O
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> MainMenuUIClass;

    // �]�m??��???X?��
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // 摧毀���XUI
    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentUI;

    // 故事重要性?�Ыغ�??��
    void InitializeCharacterCreationManager();

    // ??�ب���??UI
    UUserWidget* CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass);

    // ����摧毀 UI
    void RemoveCurrentUI();

    // ???X?��
    void StartGameWithCharacter(const FMingCharacterData& CharacterData);

    // ��??�D?X
    void ReturnToMainMenu();

    // �O??����故事重要性?��??
    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData);
);

