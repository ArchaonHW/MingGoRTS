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

    // 角色??建管?X    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // 顯示角色??建 UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI();

    // ????角色??建 UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void HideCharacterCreationUI();

    // ????角色??建完??
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData};

    // ????角色??建????
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled();

    // 設置角色??建 UI 類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> CharacterCreationUIClass;

    // 設置主?XUI 類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> MainMenuUIClass;

    // 設置??戲???X?景
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // ????顯示XUI
    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentUI;

    // ???X??X?創建管??器
    void InitializeCharacterCreationManager();

    // ??建並顯??UI
    UUserWidget* CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass};

    // 移除???? UI
    void RemoveCurrentUI();

    // ???X?戲
    void StartGameWithCharacter(const FMingCharacterData& CharacterData};

    // 返??主?X    void ReturnToMainMenu();

    // 保??角色???X??X?實??    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData};
};

