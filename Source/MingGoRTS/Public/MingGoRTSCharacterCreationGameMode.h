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

    // 角色創建管理器
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Creation")
    TObjectPtr<UMingGoRTSCharacterCreationManager> CharacterCreationManager;

    // 顯示角色創建 UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void ShowCharacterCreationUI();

    // 隱藏角色創建 UI
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void HideCharacterCreationUI();

    // 處理角色創建完成
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCompleted(const FMingCharacterData& CharacterData);

    // 處理角色創建取消
    UFUNCTION(BlueprintCallable, Category = "Character Creation")
    void OnCharacterCreationCancelled();

    // 設置角色創建 UI 類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> CharacterCreationUIClass;

    // 設置主菜單 UI 類別
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    TSubclassOf<class UUserWidget> MainMenuUIClass;

    // 設置遊戲開始場景
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Creation")
    FString GameStartLevelName;

protected:
    // 當前顯示的 UI
    UPROPERTY()
    TObjectPtr<class UUserWidget> CurrentUI;

    // 初始化角色創建管理器
    void InitializeCharacterCreationManager();

    // 創建並顯示 UI
    UUserWidget* CreateAndShowWidget(TSubclassOf<class UUserWidget> WidgetClass);

    // 移除當前 UI
    void RemoveCurrentUI();

    // 開始遊戲
    void StartGameWithCharacter(const FMingCharacterData& CharacterData);

    // 返回主菜單
    void ReturnToMainMenu();

    // 保存角色數據到遊戲實例
    void SaveCharacterToGameInstance(const FMingCharacterData& CharacterData);
};
