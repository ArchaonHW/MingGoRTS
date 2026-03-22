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
 * 保??/載入??面組件
 * ????存??槽??表顯示、??存、?X?、刪???X */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingSaveLoadWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer};

    // ???X?????    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void InitializeSaveLoadWidget(UMingSaveGameManager* InSaveManager};

    // 設置模??（??存??載入??    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetMode(bool bInSaveMode};

    // ??新存?X?表
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void RefreshSaveSlotList();

    // ????存????    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectSaveSlot(int32 SlotIndex};

    // ???X??X?中????檔槽
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }

    // ????保??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteSave();

    // ????載入
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteLoad();

    // ???X?除
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteDelete();

    // 設置存?X?稱
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetSaveName(const FString& NewName};

    // ????存?X?稱
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString GetSaveName() const;

    // ??建????檔??使用第?X?空槽??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void CreateNewSave();

    // 導出存??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExportSelectedSave();

    // 導入存??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ImportSave();

    // ???X?快????存槽
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectQuickSaveSlot();

    // ???X?自????存槽
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectAutoSaveSlot();

    // 事件??調
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveCompleted(bool bSuccess, int32 SlotIndex};

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnLoadCompleted(bool bSuccess, int32 SlotIndex};

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnDeleteCompleted(bool bSuccess, int32 SlotIndex};

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveSlotSelected(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo};

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnSaveSlotListRefreshed(const TArray<FSaveSlotUIInfo>& SlotInfos};

    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void OnErrorOccurred(const FString& ErrorMessage};

protected:
    // Native Construct
    virtual void NativeConstruct() override;

    // Native Destruct
    virtual void NativeDestruct() override;

    // 綁??UI事件
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void BindUIEvents();

    // ??新UI??X    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void UpdateUIState();

    // ????存??槽UI信息
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    FSaveSlotUIInfo GetSaveSlotUIInfo(int32 SlotIndex) const;

    // 確??對話??    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void ShowConfirmDialog(const FString& Title, const FString& Message};

    // ???X??X    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString FormatPlayTime(int32 TotalSeconds) const;

    // 驗??輸入
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    bool IsValidSaveName(const FString& Name) const;

private:
    // 保??管?X??X    UPROPERTY()
    TObjectPtr<UMingSaveGameManager> SaveManager;

    // ????模??
    UPROPERTY()
    bool bSaveMode;

    // ???X?中????檔槽
    UPROPERTY()
    int32 SelectedSlotIndex;

    // ????存?X?稱
    UPROPERTY()
    FString CurrentSaveName;

    // UI 組件綁??（在????中設置??
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

    // 存??槽?X?數??    UPROPERTY()
    TArray<TObjectPtr<UButton>> SaveSlotButtons;

    // ????點?X???
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
    void OnSaveNameTextChanged(const FText& Text};

    UFUNCTION()
    void OnSaveNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod};

    // 存??槽選???X    UFUNCTION()
    void OnSaveSlotButtonClicked(int32 SlotIndex};

    // 保??/載入/??除完?X?調
    UFUNCTION()
    void HandleSaveCompleted(EMingSaveGameResult Result, int32 SlotIndex};

    UFUNCTION()
    void HandleLoadCompleted(EMingSaveGameResult Result, int32 SlotIndex};

    UFUNCTION()
    void HandleDeleteCompleted(bool bSuccess};

    // ??新??中槽顯??    void UpdateSelectedSlotDisplay();

    // ??建存??槽?X?UI
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void CreateSaveSlotEntry(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo};

    // 清除存??槽????    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ClearSaveSlotList();
};

