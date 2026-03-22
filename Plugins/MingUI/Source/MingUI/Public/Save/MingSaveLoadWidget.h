#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MingSaveLoadWidget.generated.h"

// Forward declarations
class UMingSaveGameManager;
class UButton;
class UTextBlock;
class UImage;
class UScrollBox;
class UVerticalBox;
class UEditableTextBox;

USTRUCT(BlueprintType)
struct FSaveSlotUIInfo
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 SlotIndex;

    UPROPERTY(BlueprintReadOnly)
    FString SaveName;

    UPROPERTY(BlueprintReadOnly)
    FString SaveDate;

    UPROPERTY(BlueprintReadOnly)
    FString PlayTime;

    UPROPERTY(BlueprintReadOnly)
    bool bIsEmpty;

    UPROPERTY(BlueprintReadOnly)
    bool bIsAutosave;

    UPROPERTY(BlueprintReadOnly)
    UTexture2D* Thumbnail;

    FSaveSlotUIInfo()
        : SlotIndex(0)
        , bIsEmpty(true)
        , bIsAutosave(false)
        , Thumbnail(nullptr)
    {}
};

/**
 * 保存/載入界面組件
 * 提供存檔槽列表顯示、保存、載入、刪除功能
 */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingSaveLoadWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer);

    // 初始化組件
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void InitializeSaveLoadWidget(UMingSaveGameManager* InSaveManager);

    // 設置模式（保存或載入）
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetMode(bool bInSaveMode);

    // 刷新存檔列表
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void RefreshSaveSlotList();

    // 選擇存檔槽
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectSaveSlot(int32 SlotIndex);

    // 獲取當前選中的存檔槽
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }

    // 執行保存
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteSave();

    // 執行載入
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteLoad();

    // 執行刪除
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteDelete();

    // 設置存檔名稱
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetSaveName(const FString& NewName);

    // 獲取存檔名稱
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString GetSaveName() const;

    // 創建新存檔（使用第一個空槽）
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void CreateNewSave();

    // 導出存檔
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExportSelectedSave();

    // 導入存檔
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ImportSave();

    // 切換到快速保存槽
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectQuickSaveSlot();

    // 切換到自動保存槽
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectAutoSaveSlot();

    // 事件回調
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveCompleted(bool bSuccess, int32 SlotIndex);

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnLoadCompleted(bool bSuccess, int32 SlotIndex);

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnDeleteCompleted(bool bSuccess, int32 SlotIndex);

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveSlotSelected(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo);

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveSlotListRefreshed(const TArray<FSaveSlotUIInfo>& SlotInfos);

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnErrorOccurred(const FString& ErrorMessage);

protected:
    // Native Construct
    virtual void NativeConstruct() override;

    // Native Destruct
    virtual void NativeDestruct() override;

    // 綁定UI事件
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void BindUIEvents();

    // 更新UI狀態
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void UpdateUIState();

    // 獲取存檔槽UI信息
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    FSaveSlotUIInfo GetSaveSlotUIInfo(int32 SlotIndex) const;

    // 確認對話框
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void ShowConfirmDialog(const FString& Title, const FString& Message);

    // 格式化時間
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString FormatPlayTime(int32 TotalSeconds) const;

    // 驗證輸入
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    bool IsValidSaveName(const FString& Name) const;

private:
    // 保存管理器引用
    UPROPERTY()
    TObjectPtr<UMingSaveGameManager> SaveManager;

    // 當前模式
    UPROPERTY()
    bool bSaveMode;

    // 當前選中的存檔槽
    UPROPERTY()
    int32 SelectedSlotIndex;

    // 當前存檔名稱
    UPROPERTY()
    FString CurrentSaveName;

    // UI 組件綁定（在藍圖中設置）
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UScrollBox> SaveSlotScrollBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UEditableTextBox> SaveNameTextBox;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SaveButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> LoadButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> DeleteButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> NewSaveButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> QuickSaveButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExportButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ImportButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> CloseButton;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SelectedSlotNameText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SelectedSlotDateText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> SelectedSlotTimeText;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UImage> SelectedSlotThumbnail;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ModeTitleText;

    // 存檔槽按鈕數組
    UPROPERTY()
    TArray<TObjectPtr<UButton>> SaveSlotButtons;

    // 按鈕點擊處理
    UFUNCTION()
    void OnSaveButtonClicked();

    UFUNCTION()
    void OnLoadButtonClicked();

    UFUNCTION()
    void OnDeleteButtonClicked();

    UFUNCTION()
    void OnNewSaveButtonClicked();

    UFUNCTION()
    void OnQuickSaveButtonClicked();

    UFUNCTION()
    void OnExportButtonClicked();

    UFUNCTION()
    void OnImportButtonClicked();

    UFUNCTION()
    void OnCloseButtonClicked();

    UFUNCTION()
    void OnSaveNameTextChanged(const FText& Text);

    UFUNCTION()
    void OnSaveNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod);

    // 存檔槽選擇處理
    UFUNCTION()
    void OnSaveSlotButtonClicked(int32 SlotIndex);

    // 保存/載入/刪除完成回調
    UFUNCTION()
    void HandleSaveCompleted(EMingSaveGameResult Result, int32 SlotIndex);

    UFUNCTION()
    void HandleLoadCompleted(EMingSaveGameResult Result, int32 SlotIndex);

    UFUNCTION()
    void HandleDeleteCompleted(bool bSuccess);

    // 更新選中槽顯示
    void UpdateSelectedSlotDisplay();

    // 創建存檔槽條目UI
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void CreateSaveSlotEntry(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo);

    // 清除存檔槽列表
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ClearSaveSlotList();
};
