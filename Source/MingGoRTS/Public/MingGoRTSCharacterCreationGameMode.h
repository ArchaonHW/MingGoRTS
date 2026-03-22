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
    AMingGoRTSCharacterCreationGameMode();

    virtual void BeginPlay() override;

    // ����??�غ�?X
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // ��ܨ���??�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI();

    // ????����??�� UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void HideCharacterCreationUI();

    // ????����??�ا�??
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData);

    // ????����??��????
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
    // ????���XUI
    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentUI;

    // ???X??X?�Ыغ�??��
    void InitializeCharacterCreationManager();

    // ??�ب���??UI
    UUserWidget* CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass);

    // ����???? UI
    void RemoveCurrentUI();

    // ???X?��
    void StartGameWithCharacter(const FMingCharacterData& CharacterData);

    // ��??�D?X
    void ReturnToMainMenu();

    // �O??����???X??X?��??
    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData);
);

