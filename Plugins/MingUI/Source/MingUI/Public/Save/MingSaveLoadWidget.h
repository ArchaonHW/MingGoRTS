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
 * �O??/���J??���ե�
 * 摧毀�s??��??����ܡB??�s�B?X?�B�R???X */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingSaveLoadWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer);

    // 目標數量??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void InitializeSaveLoadWidget(UMingSaveGameManager* InSaveManager);

    // �]�m��??�]??�s??���J??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetMode(bool bInSaveMode);

    // ??�s�s?X?��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void RefreshSaveSlotList();

    // 摧毀�s摧毀
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectSaveSlot(int32 SlotIndex);

    // 故事重要性?��摧毀�ɼ�
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }

    // 摧毀�O??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteSave();

    // 摧毀���J
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteLoad();

    // ???X?��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteDelete();

    // �]�m�s?X?��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetSaveName(const FString& NewName);

    // 摧毀�s?X?��
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString GetSaveName() const;

    // ??��摧毀��??�ϥβ�?X?�ż�??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void CreateNewSave();

    // �ɥX�s??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExportSelectedSave();

    // �ɤJ�s??
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ImportSave();

    // ???X?��摧毀�s��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectQuickSaveSlot();

    // ???X?��摧毀�s��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectAutoSaveSlot();

    // �ƥ�??��
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

    // �j??UI�ƥ�
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void BindUIEvents();

    // ??�sUI??X
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void UpdateUIState();

    // 摧毀�s??��UI�H��
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    FSaveSlotUIInfo GetSaveSlotUIInfo(int32 SlotIndex) const;

    // �T??���??
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void ShowConfirmDialog(const FString& Title, const FString& Message);

    // 故事重要性
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString FormatPlayTime(int32 TotalSeconds) const;

    // ��??��J
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    bool IsValidSaveName(const FString& Name) const;

private:
    // �O??��?X??X
    UPROPERTY()
    TObjectPtr<UMingSaveGameManager> SaveManager;

    // 摧毀��??
    UPROPERTY()
    bool bSaveMode;

    // ???X?��摧毀�ɼ�
    UPROPERTY()
    int32 SelectedSlotIndex;

    // 摧毀�s?X?��
    UPROPERTY()
    FString CurrentSaveName;

    // UI �ե�j??�]�b摧毀���]�m??
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

    // �s??��?X?��??
    UPROPERTY()
    TArray<TObjectPtr<UButton>> SaveSlotButtons;

    // 摧毀�I?X???
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

    // �s??�ѿ�???X
    UFUNCTION()
    void OnSaveSlotButtonClicked(int32 SlotIndex);

    // �O??/���J/??����?X?��
    UFUNCTION()
    void HandleSaveCompleted(EMingSaveGameResult Result, int32 SlotIndex);

    UFUNCTION()
    void HandleLoadCompleted(EMingSaveGameResult Result, int32 SlotIndex);

    UFUNCTION()
    void HandleDeleteCompleted(bool bSuccess);

    // ??�s??������??
    void UpdateSelectedSlotDisplay();

    // ??�ئs??��?X?UI
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void CreateSaveSlotEntry(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo);

    // �M���s??��摧毀
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ClearSaveSlotList();
};

