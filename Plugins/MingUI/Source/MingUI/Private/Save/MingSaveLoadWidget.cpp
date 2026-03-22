#include "Save/MingSaveLoadWidget.h"
#include "Save/MingSaveGameManager.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Texture2D.h"

UMingSaveLoadWidget::UMingSaveLoadWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , SaveManager(nullptr)
    , bSaveMode(true)
    , SelectedSlotIndex(-1)
    , CurrentSaveName(TEXT(""))
{
}

void UMingSaveLoadWidget::NativeConstruct()
{
    Super::NativeConstruct();

    BindUIEvents();
    RefreshSaveSlotList();
    UpdateUIState();
}

void UMingSaveLoadWidget::NativeDestruct()
{
    // 解除事件綁定
    if (SaveManager)
    {
        SaveManager->OnSaveGameComplete.RemoveDynamic(this, &UMingSaveLoadWidget::HandleSaveCompleted);
        SaveManager->OnLoadGameComplete.RemoveDynamic(this, &UMingSaveLoadWidget::HandleLoadCompleted);
        SaveManager->OnDeleteGameComplete.RemoveDynamic(this, &UMingSaveLoadWidget::HandleDeleteCompleted);
    }

    Super::NativeDestruct();
}

void UMingSaveLoadWidget::InitializeSaveLoadWidget(UMingSaveGameManager* InSaveManager)
{
    SaveManager = InSaveManager;

    if (SaveManager)
    {
        // 綁定事件回調
        SaveManager->OnSaveGameComplete.AddDynamic(this, &UMingSaveLoadWidget::HandleSaveCompleted);
        SaveManager->OnLoadGameComplete.AddDynamic(this, &UMingSaveLoadWidget::HandleLoadCompleted);
        SaveManager->OnDeleteGameComplete.AddDynamic(this, &UMingSaveLoadWidget::HandleDeleteCompleted);
    }

    RefreshSaveSlotList();
    UpdateUIState();
}

void UMingSaveLoadWidget::SetMode(bool bInSaveMode)
{
    bSaveMode = bInSaveMode;
    UpdateUIState();

    if (ModeTitleText)
    {
        ModeTitleText->SetText(FText::FromString(bSaveMode ? TEXT("保存遊戲") : TEXT("載入遊戲")));
    }
}

void UMingSaveLoadWidget::RefreshSaveSlotList()
{
    ClearSaveSlotList();

    if (!SaveManager)
    {
        return;
    }

    // 獲取所有存檔槽信息
    TArray<FMingSaveGameSlotInfo> SlotInfos = SaveManager->GetAllSaveSlots();
    TArray<FSaveSlotUIInfo> UIInfos;

    for (const FMingSaveGameSlotInfo& Info : SlotInfos)
    {
        FSaveSlotUIInfo UIInfo;
        UIInfo.SlotIndex = Info.SlotIndex;
        UIInfo.SaveName = Info.SaveName;
        UIInfo.SaveDate = Info.SaveDateTime.ToString(TEXT("%Y-%m-%d %H:%M"));
        UIInfo.PlayTime = FormatPlayTime(Info.PlayTimeSeconds);
        UIInfo.bIsEmpty = Info.bIsEmpty;
        UIInfo.bIsAutosave = Info.bIsAutosave;
        UIInfo.Thumbnail = Info.ThumbnailImage;

        UIInfos.Add(UIInfo);

        // 創建存檔槽UI條目
        CreateSaveSlotEntry(Info.SlotIndex, UIInfo);
    }

    OnSaveSlotListRefreshed(UIInfos);
}

void UMingSaveLoadWidget::SelectSaveSlot(int32 SlotIndex)
{
    SelectedSlotIndex = SlotIndex;
    UpdateSelectedSlotDisplay();
    UpdateUIState();

    FSaveSlotUIInfo SlotInfo = GetSaveSlotUIInfo(SlotIndex);
    OnSaveSlotSelected(SlotIndex, SlotInfo);

    // 更新名稱輸入框
    if (SaveNameTextBox && !SlotInfo.bIsEmpty)
    {
        SaveNameTextBox->SetText(FText::FromString(SlotInfo.SaveName));
    }
}

void UMingSaveLoadWidget::ExecuteSave()
{
    if (!SaveManager || SelectedSlotIndex < 0)
    {
        OnErrorOccurred(TEXT("請選擇一個存檔槽"));
        return;
    }

    FString SaveName = GetSaveName();
    if (!IsValidSaveName(SaveName))
    {
        OnErrorOccurred(TEXT("請輸入有效的存檔名稱"));
        return;
    }

    EMingSaveGameResult Result = SaveManager->SaveGame(SelectedSlotIndex, SaveName, false);

    if (Result == EMingSaveGameResult::Success)
    {
        RefreshSaveSlotList();
    }
}

void UMingSaveLoadWidget::ExecuteLoad()
{
    if (!SaveManager || SelectedSlotIndex < 0)
    {
        OnErrorOccurred(TEXT("請選擇一個存檔槽"));
        return;
    }

    EMingSaveGameResult Result = SaveManager->LoadGame(SelectedSlotIndex);

    if (Result != EMingSaveGameResult::Success)
    {
        FString ErrorMsg = FString::Printf(TEXT("載入失敗: %d"), static_cast<int32>(Result));
        OnErrorOccurred(ErrorMsg);
    }
}

void UMingSaveLoadWidget::ExecuteDelete()
{
    if (!SaveManager || SelectedSlotIndex < 0)
    {
        OnErrorOccurred(TEXT("請選擇一個存檔槽"));
        return;
    }

    // 顯示確認對話框（在藍圖中實現）
    ShowConfirmDialog(TEXT("確認刪除"), TEXT("確定要刪除此存檔嗎？此操作無法撤銷。"));
}

void UMingSaveLoadWidget::SetSaveName(const FString& NewName)
{
    CurrentSaveName = NewName;

    if (SaveNameTextBox)
    {
        SaveNameTextBox->SetText(FText::FromString(NewName));
    }
}

FString UMingSaveLoadWidget::GetSaveName() const
{
    if (SaveNameTextBox)
    {
        return SaveNameTextBox->GetText().ToString();
    }
    return CurrentSaveName;
}

void UMingSaveLoadWidget::CreateNewSave()
{
    if (!SaveManager)
    {
        return;
    }

    int32 EmptySlot = SaveManager->GetNextAvailableSlot();
    if (EmptySlot >= 0)
    {
        SelectSaveSlot(EmptySlot);
        SetSaveName(FString::Printf(TEXT("存檔 %d"), EmptySlot));

        if (bSaveMode)
        {
            ExecuteSave();
        }
    }
    else
    {
        OnErrorOccurred(TEXT("沒有空的存檔槽"));
    }
}

void UMingSaveLoadWidget::ExportSelectedSave()
{
    if (!SaveManager || SelectedSlotIndex < 0)
    {
        OnErrorOccurred(TEXT("請選擇一個存檔槽"));
        return;
    }

    // 導出邏輯將在藍圖中實現（使用文件對話框）
}

void UMingSaveLoadWidget::ImportSave()
{
    if (!SaveManager)
    {
        return;
    }

    // 導入邏輯將在藍圖中實現（使用文件對話框）
}

void UMingSaveLoadWidget::SelectQuickSaveSlot()
{
    if (SaveManager)
    {
        SelectSaveSlot(SaveManager->GetQuickSaveSlot());
    }
}

void UMingSaveLoadWidget::SelectAutoSaveSlot()
{
    if (SaveManager)
    {
        SelectSaveSlot(SaveManager->GetAutoSaveSlot());
    }
}

void UMingSaveLoadWidget::BindUIEvents()
{
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnSaveButtonClicked);
    }

    if (LoadButton)
    {
        LoadButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnLoadButtonClicked);
    }

    if (DeleteButton)
    {
        DeleteButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnDeleteButtonClicked);
    }

    if (NewSaveButton)
    {
        NewSaveButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnNewSaveButtonClicked);
    }

    if (QuickSaveButton)
    {
        QuickSaveButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnQuickSaveButtonClicked);
    }

    if (ExportButton)
    {
        ExportButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnExportButtonClicked);
    }

    if (ImportButton)
    {
        ImportButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnImportButtonClicked);
    }

    if (CloseButton)
    {
        CloseButton->OnClicked.AddDynamic(this, &UMingSaveLoadWidget::OnCloseButtonClicked);
    }

    if (SaveNameTextBox)
    {
        SaveNameTextBox->OnTextChanged.AddDynamic(this, &UMingSaveLoadWidget::OnSaveNameTextChanged);
        SaveNameTextBox->OnTextCommitted.AddDynamic(this, &UMingSaveLoadWidget::OnSaveNameTextCommitted);
    }
}

void UMingSaveLoadWidget::UpdateUIState()
{
    bool bHasSelection = SelectedSlotIndex >= 0;
    bool bSlotHasData = false;

    if (SaveManager && bHasSelection)
    {
        bSlotHasData = SaveManager->DoesSaveExist(SelectedSlotIndex);
    }

    // 根據模式顯示/隱藏按鈕
    if (SaveButton)
    {
        SaveButton->SetVisibility(bSaveMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        SaveButton->SetIsEnabled(bHasSelection);
    }

    if (LoadButton)
    {
        LoadButton->SetVisibility(!bSaveMode ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        LoadButton->SetIsEnabled(bHasSelection && bSlotHasData);
    }

    if (DeleteButton)
    {
        DeleteButton->SetIsEnabled(bHasSelection && bSlotHasData);
    }

    if (ExportButton)
    {
        ExportButton->SetIsEnabled(bHasSelection && bSlotHasData);
    }
}

FSaveSlotUIInfo UMingSaveLoadWidget::GetSaveSlotUIInfo(int32 SlotIndex) const
{
    FSaveSlotUIInfo Info;

    if (SaveManager)
    {
        FMingSaveGameSlotInfo SlotInfo = SaveManager->GetSaveSlotInfo(SlotIndex);

        Info.SlotIndex = SlotInfo.SlotIndex;
        Info.SaveName = SlotInfo.SaveName;
        Info.SaveDate = SlotInfo.SaveDateTime.ToString(TEXT("%Y-%m-%d %H:%M"));
        Info.PlayTime = FormatPlayTime(SlotInfo.PlayTimeSeconds);
        Info.bIsEmpty = SlotInfo.bIsEmpty;
        Info.bIsAutosave = SlotInfo.bIsAutosave;
        Info.Thumbnail = SlotInfo.ThumbnailImage;
    }

    return Info;
}

FString UMingSaveLoadWidget::FormatPlayTime(int32 TotalSeconds) const
{
    int32 Hours = TotalSeconds / 3600;
    int32 Minutes = (TotalSeconds % 3600) / 60;

    if (Hours > 0)
    {
        return FString::Printf(TEXT("%d小時 %d分鐘"), Hours, Minutes);
    }
    else
    {
        return FString::Printf(TEXT("%d分鐘"), Minutes);
    }
}

bool UMingSaveLoadWidget::IsValidSaveName(const FString& Name) const
{
    if (Name.IsEmpty())
    {
        return false;
    }

    // 檢查無效字符
    TArray<FString> InvalidChars = { TEXT("/"), TEXT("\\"), TEXT(":"), TEXT("*"), TEXT("?"), TEXT("\""), TEXT("<"), TEXT(">"), TEXT("|") };
    for (const FString& Char : InvalidChars)
    {
        if (Name.Contains(Char))
        {
            return false;
        }
    }

    return true;
}

void UMingSaveLoadWidget::OnSaveButtonClicked()
{
    ExecuteSave();
}

void UMingSaveLoadWidget::OnLoadButtonClicked()
{
    ExecuteLoad();
}

void UMingSaveLoadWidget::OnDeleteButtonClicked()
{
    ExecuteDelete();
}

void UMingSaveLoadWidget::OnNewSaveButtonClicked()
{
    CreateNewSave();
}

void UMingSaveLoadWidget::OnQuickSaveButtonClicked()
{
    SelectQuickSaveSlot();
    if (bSaveMode && SelectedSlotIndex >= 0)
    {
        SetSaveName(TEXT("快速保存"));
        ExecuteSave();
    }
}

void UMingSaveLoadWidget::OnExportButtonClicked()
{
    ExportSelectedSave();
}

void UMingSaveLoadWidget::OnImportButtonClicked()
{
    ImportSave();
}

void UMingSaveLoadWidget::OnCloseButtonClicked()
{
    // 關閉UI
    RemoveFromParent();
}

void UMingSaveLoadWidget::OnSaveNameTextChanged(const FText& Text)
{
    CurrentSaveName = Text.ToString();
}

void UMingSaveLoadWidget::OnSaveNameTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
    CurrentSaveName = Text.ToString();

    if (CommitMethod == ETextCommit::OnEnter && bSaveMode)
    {
        ExecuteSave();
    }
}

void UMingSaveLoadWidget::OnSaveSlotButtonClicked(int32 SlotIndex)
{
    SelectSaveSlot(SlotIndex);
}

void UMingSaveLoadWidget::HandleSaveCompleted(EMingSaveGameResult Result, int32 SlotIndex)
{
    bool bSuccess = (Result == EMingSaveGameResult::Success);
    OnSaveCompleted(bSuccess, SlotIndex);

    if (bSuccess)
    {
        RefreshSaveSlotList();
    }
    else
    {
        FString ErrorMsg = FString::Printf(TEXT("保存失敗: %d"), static_cast<int32>(Result));
        OnErrorOccurred(ErrorMsg);
    }
}

void UMingSaveLoadWidget::HandleLoadCompleted(EMingSaveGameResult Result, int32 SlotIndex)
{
    bool bSuccess = (Result == EMingSaveGameResult::Success);
    OnLoadCompleted(bSuccess, SlotIndex);

    if (!bSuccess)
    {
        FString ErrorMsg = FString::Printf(TEXT("載入失敗: %d"), static_cast<int32>(Result));
        OnErrorOccurred(ErrorMsg);
    }
}

void UMingSaveLoadWidget::HandleDeleteCompleted(bool bSuccess)
{
    OnDeleteCompleted(bSuccess, SelectedSlotIndex);

    if (bSuccess)
    {
        RefreshSaveSlotList();
        SelectedSlotIndex = -1;
        UpdateUIState();
    }
}

void UMingSaveLoadWidget::UpdateSelectedSlotDisplay()
{
    if (SelectedSlotIndex < 0)
    {
        // 清空顯示
        if (SelectedSlotNameText)
        {
            SelectedSlotNameText->SetText(FText::FromString(TEXT("未選擇")));
        }
        if (SelectedSlotDateText)
        {
            SelectedSlotDateText->SetText(FText::FromString(TEXT("-")));
        }
        if (SelectedSlotTimeText)
        {
            SelectedSlotTimeText->SetText(FText::FromString(TEXT("-")));
        }
        return;
    }

    FSaveSlotUIInfo Info = GetSaveSlotUIInfo(SelectedSlotIndex);

    if (SelectedSlotNameText)
    {
        SelectedSlotNameText->SetText(FText::FromString(Info.SaveName));
    }

    if (SelectedSlotDateText)
    {
        SelectedSlotDateText->SetText(FText::FromString(Info.SaveDate));
    }

    if (SelectedSlotTimeText)
    {
        SelectedSlotTimeText->SetText(FText::FromString(Info.PlayTime));
    }

    if (SelectedSlotThumbnail)
    {
        if (Info.Thumbnail)
        {
            SelectedSlotThumbnail->SetBrushFromTexture(Info.Thumbnail);
        }
        else
        {
            // 設置默認縮略圖
            SelectedSlotThumbnail->SetBrushFromTexture(nullptr);
        }
    }
}

void UMingSaveLoadWidget::ClearSaveSlotList()
{
    if (SaveSlotScrollBox)
    {
        SaveSlotScrollBox->ClearChildren();
    }

    SaveSlotButtons.Empty();
}
