#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingCharacterSelectionWidget.generated.h"

// 角色信息結構
USTRUCT(BlueprintType)
struct FCharacterInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* CharacterIcon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, float> CharacterStats;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FString> SpecialAbilities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString CharacterType;

    FCharacterInfo()
    {
        CharacterName = TEXT("");
        CharacterDescription = TEXT("");
        CharacterIcon = nullptr;
        CharacterType = TEXT("");
    }
};

// 角色選擇事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterSelected, const FCharacterInfo&, SelectedCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterSelectionConfirmed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterSelectionCancelled);

UCLASS()
class MINGGORTS_API UMingCharacterSelectionWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingCharacterSelectionWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化角色選擇界面
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void InitializeCharacterSelection();

    // 設置可用角色列表
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void SetAvailableCharacters(const TArray<FCharacterInfo>& Characters);

    // 選擇角色
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void SelectCharacter(const FCharacterInfo& Character);

    // 確認選擇
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void ConfirmSelection();

    // 取消選擇
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void CancelSelection();

    // 獲取當前選中的角色
    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FCharacterInfo GetSelectedCharacter() const { return CurrentSelectedCharacter; }

    // 顯示角色詳情
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void ShowCharacterDetails(const FCharacterInfo& Character);

    // 更新角色預覽
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void UpdateCharacterPreview(const FCharacterInfo& Character);

    // 顯示角色統計
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void DisplayCharacterStats(const TMap<FString, float>& Stats);

    // 顯示特殊能力
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void DisplaySpecialAbilities(const TArray<FString>& Abilities);

    // 事件委託
    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnCharacterSelected OnCharacterSelected;

    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnCharacterSelectionConfirmed OnCharacterSelectionConfirmed;

    UPROPERTY(BlueprintAssignable, Category = "Character Selection")
    FOnCharacterSelectionCancelled OnCharacterSelectionCancelled;

protected:
    // 藍圖實現事件
    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnWidgetInitialized();

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnCharacterHighlighted(const FCharacterInfo& Character);

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnSelectionConfirmed();

    UFUNCTION(BlueprintImplementableEvent, Category = "Character Selection")
    void OnSelectionCancelled();

    // 初始化默認角色
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void InitializeDefaultCharacters();

    // 創建聖者角色信息
    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FCharacterInfo CreateSageCharacterInfo() const;

    // 創建魔王角色信息
    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FCharacterInfo CreateDemonKingCharacterInfo() const;

    // 創建偽聖者角色信息
    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FCharacterInfo CreateFalseSageCharacterInfo() const;

    // 比較角色
    UFUNCTION(BlueprintCallable, Category = "Character Selection")
    void CompareCharacters(const FCharacterInfo& CharacterA, const FCharacterInfo& CharacterB);

    // 獲取角色建議
    UFUNCTION(BlueprintPure, Category = "Character Selection")
    FString GetCharacterRecommendation(const FString& PlayStyle) const;

protected:
    // 可用角色列表
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    TArray<FCharacterInfo> AvailableCharacters;

    // 當前選中的角色
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    FCharacterInfo CurrentSelectedCharacter;

    // 是否已確認選擇
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    bool bIsSelectionConfirmed;

    // 選擇提示文字
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Selection")
    FString SelectionHintText;
};
