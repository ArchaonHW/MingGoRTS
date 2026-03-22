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
 * ‰øùÂ?/ËºâÂÖ•?åÈù¢ÁµÑ‰ª∂
 * ?ê‰?Â≠òÊ?ÊßΩÂ?Ë°®È°ØÁ§∫„ÄÅ‰?Â≠ò„ÄÅË??•„ÄÅÂà™?§Â??? */
UCLASS(ClassGroup = (UI), Blueprintable)
class MINGUI_API UMingSaveLoadWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer};

    // ?ùÂ??ñÁ?‰ª?    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void InitializeSaveLoadWidget(UMingSaveGameManager* InSaveManager};

    // Ë®≠ÁΩÆÊ®°Â?Ôºà‰?Â≠òÊ?ËºâÂÖ•Ôº?    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetMode(bool bInSaveMode};

    // ?∑Êñ∞Â≠òÊ??óË°®
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void RefreshSaveSlotList(};

    // ?∏Ê?Â≠òÊ?Êß?    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectSaveSlot(int32 SlotIndex};

    // ?≤Â??∂Â??∏‰∏≠?ÑÂ?Ê™îÊßΩ
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    int32 GetSelectedSlotIndex() const { return SelectedSlotIndex; }

    // ?∑Ë?‰øùÂ?
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteSave(};

    // ?∑Ë?ËºâÂÖ•
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteLoad(};

    // ?∑Ë??™Èô§
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExecuteDelete(};

    // Ë®≠ÁΩÆÂ≠òÊ??çÁ®±
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SetSaveName(const FString& NewName};

    // ?≤Â?Â≠òÊ??çÁ®±
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString GetSaveName() const;

    // ?µÂª∫?∞Â?Ê™îÔ?‰ΩøÁî®Á¨¨‰??ãÁ©∫ÊßΩÔ?
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void CreateNewSave(};

    // Â∞éÂá∫Â≠òÊ?
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ExportSelectedSave(};

    // Â∞éÂÖ•Â≠òÊ?
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ImportSave(};

    // ?áÊ??∞Âø´?ü‰?Â≠òÊßΩ
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectQuickSaveSlot(};

    // ?áÊ??∞Ëá™?ï‰?Â≠òÊßΩ
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void SelectAutoSaveSlot(};

    // ‰∫ã‰ª∂?ûË™ø
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

    // Á∂ÅÂ?UI‰∫ã‰ª∂
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void BindUIEvents(};

    // ?¥Êñ∞UI?Ä??    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void UpdateUIState(};

    // ?≤Â?Â≠òÊ?ÊßΩUI‰ø°ÊÅØ
    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    FSaveSlotUIInfo GetSaveSlotUIInfo(int32 SlotIndex) const;

    // Á¢∫Ë?Â∞çË©±Ê°?    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void ShowConfirmDialog(const FString& Title, const FString& Message};

    // ?ºÂ??ñÊ???    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    FString FormatPlayTime(int32 TotalSeconds) const;

    // È©óË?Ëº∏ÂÖ•
    UFUNCTION(BlueprintPure, Category = "Save/Load UI")
    bool IsValidSaveName(const FString& Name) const;

private:
    // ‰øùÂ?ÁÆ°Á??®Â???    UPROPERTY()
    TObjectPtr<UMingSaveGameManager> SaveManager;

    // ?∂Â?Ê®°Â?
    UPROPERTY()
    bool bSaveMode;

    // ?∂Â??∏‰∏≠?ÑÂ?Ê™îÊßΩ
    UPROPERTY()
    int32 SelectedSlotIndex;

    // ?∂Â?Â≠òÊ??çÁ®±
    UPROPERTY()
    FString CurrentSaveName;

    // UI ÁµÑ‰ª∂Á∂ÅÂ?ÔºàÂú®?çÂ?‰∏≠Ë®≠ÁΩÆÔ?
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

    // Â≠òÊ?ÊßΩÊ??ïÊï∏Áµ?    UPROPERTY()
    TArray<TObjectPtr<UButton>> SaveSlotButtons;

    // ?âÈ?ÈªûÊ??ïÁ?
    UFUNCTION()
    void OnSaveButtonClicked(};

    UFUNCTION()
    void OnLoadButtonClicked(};

    UFUNCTION()
    void OnDeleteButtonClicked(};

    UFUNCTION()
    void OnNewSaveButtonClicked(};

    UFUNCTION()
    void OnQuickSaveButtonClicked(};

    UFUNCTION()
    void OnExportButtonClicked(};

    UFUNCTION()
    void OnImportButtonClicked(};

    UFUNCTION()
    void OnCloseButtonClicked(};

    UFUNCTION()
    void OnSaveNameTextChanged(const FText& Text};

    UFUNCTION()
    void OnSaveNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod};

    // Â≠òÊ?ÊßΩÈÅ∏?áË???    UFUNCTION()
    void OnSaveSlotButtonClicked(int32 SlotIndex};

    // ‰øùÂ?/ËºâÂÖ•/?™Èô§ÂÆåÊ??ûË™ø
    UFUNCTION()
    void HandleSaveCompleted(EMingSaveGameResult Result, int32 SlotIndex};

    UFUNCTION()
    void HandleLoadCompleted(EMingSaveGameResult Result, int32 SlotIndex};

    UFUNCTION()
    void HandleDeleteCompleted(bool bSuccess};

    // ?¥Êñ∞?∏‰∏≠ÊßΩÈ°ØÁ§?    void UpdateSelectedSlotDisplay(};

    // ?µÂª∫Â≠òÊ?ÊßΩÊ??ÆUI
    UFUNCTION(BlueprintImplementableEvent, Category = "Save/Load UI")
    void CreateSaveSlotEntry(int32 SlotIndex, const FSaveSlotUIInfo& SlotInfo};

    // Ê∏ÖÈô§Â≠òÊ?ÊßΩÂ?Ë°?    UFUNCTION(BlueprintCallable, Category = "Save/Load UI")
    void ClearSaveSlotList(};
};

