#incl使de "Sa正e/Min成Sa正eLoad基本id成et.h"
#incl使de "Sa正e/Min成Sa正eGa設置eMana成e本.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/EditableTextBox.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/Text使本e2D.h"

UMin成Sa正eLoad基本id成et::UMin成Sa正eLoad基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , Sa正eMana成e本(n使llpt本)
    , bSa正eMode(t本使e)
    , SelectedSlotIndex(-1)
    , C使本本entSa正e的a設置e(TEXT(""))
{
}

正oid UMin成Sa正eLoad基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    BindUIE正ents();
    Ref本eshSa正eSlotList();
    UpdateUIState();
}

正oid UMin成Sa正eLoad基本id成et::的ati正eDest本使ct()
{
    // 解除事件綁定
    if (Sa正eMana成e本)
    {
        Sa正eMana成e本->OnSa正eGa設置eCo設置plete.Re設置o正eDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleSa正eCo設置pleted);
        Sa正eMana成e本->OnLoadGa設置eCo設置plete.Re設置o正eDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleLoadCo設置pleted);
        Sa正eMana成e本->OnDeleteGa設置eCo設置plete.Re設置o正eDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleDeleteCo設置pleted);
    }

    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Sa正eLoad基本id成et::InitializeSa正eLoad基本id成et(UMin成Sa正eGa設置eMana成e本* InSa正eMana成e本)
{
    Sa正eMana成e本 = InSa正eMana成e本;

    if (Sa正eMana成e本)
    {
        // 綁定事件回調
        Sa正eMana成e本->OnSa正eGa設置eCo設置plete.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleSa正eCo設置pleted);
        Sa正eMana成e本->OnLoadGa設置eCo設置plete.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleLoadCo設置pleted);
        Sa正eMana成e本->OnDeleteGa設置eCo設置plete.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::輸入andleDeleteCo設置pleted);
    }

    Ref本eshSa正eSlotList();
    UpdateUIState();
}

正oid UMin成Sa正eLoad基本id成et::SetMode(bool bInSa正eMode)
{
    bSa正eMode = bInSa正eMode;
    UpdateUIState();

    if (ModeTitleText)
    {
        ModeTitleText->SetText(軍Text::軍本o設置St本in成(bSa正eMode 基本 TEXT("保存遊戲") : TEXT("載入遊戲")));
    }
}

正oid UMin成Sa正eLoad基本id成et::Ref本eshSa正eSlotList()
{
    Clea本Sa正eSlotList();

    if (!Sa正eMana成e本)
    {
        本et使本n;
    }

    // 獲取所有存檔槽信息
    TA本本ay<軍Min成Sa正eGa設置eSlotInfo> SlotInfos = Sa正eMana成e本->GetAllSa正eSlots();
    TA本本ay<軍Sa正eSlotUIInfo> UIInfos;

    fo本 (const 軍Min成Sa正eGa設置eSlotInfo& Info : SlotInfos)
    {
        軍Sa正eSlotUIInfo UIInfo;
        UIInfo.SlotIndex = Info.SlotIndex;
        UIInfo.Sa正e的a設置e = Info.Sa正e的a設置e;
        UIInfo.Sa正eDate = Info.Sa正eDateTi設置e.ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M"));
        UIInfo.PlayTi設置e = 軍o本設置atPlayTi設置e(Info.PlayTi設置eSeconds);
        UIInfo.bIsE設置pty = Info.bIsE設置pty;
        UIInfo.bIsA使tosa正e = Info.bIsA使tosa正e;
        UIInfo.Th使設置bnail = Info.Th使設置bnailI設置a成e;

        UIInfos.Add(UIInfo);

        // 創建存檔槽UI條目
        C本eateSa正eSlotEnt本y(Info.SlotIndex, UIInfo);
    }

    OnSa正eSlotListRef本eshed(UIInfos);
}

正oid UMin成Sa正eLoad基本id成et::SelectSa正eSlot(int32 SlotIndex)
{
    SelectedSlotIndex = SlotIndex;
    UpdateSelectedSlotDisplay();
    UpdateUIState();

    軍Sa正eSlotUIInfo SlotInfo = GetSa正eSlotUIInfo(SlotIndex);
    OnSa正eSlotSelected(SlotIndex, SlotInfo);

    // 更新名稱輸入框
    if (Sa正e的a設置eTextBox && !SlotInfo.bIsE設置pty)
    {
        Sa正e的a設置eTextBox->SetText(軍Text::軍本o設置St本in成(SlotInfo.Sa正e的a設置e));
    }
}

正oid UMin成Sa正eLoad基本id成et::Exec使teSa正e()
{
    if (!Sa正eMana成e本  SelectedSlotIndex < 0)
    {
        OnE本本o本Occ使本本ed(TEXT("請選擇一個存檔槽"));
        本et使本n;
    }

    軍St本in成 Sa正e的a設置e = GetSa正e的a設置e();
    if (!IsValidSa正e的a設置e(Sa正e的a設置e))
    {
        OnE本本o本Occ使本本ed(TEXT("請輸入有效的存檔名稱"));
        本et使本n;
    }

    EMin成Sa正eGa設置eRes使lt Res使lt = Sa正eMana成e本->Sa正eGa設置e(SelectedSlotIndex, Sa正e的a設置e, false);

    if (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess)
    {
        Ref本eshSa正eSlotList();
    }
}

正oid UMin成Sa正eLoad基本id成et::Exec使teLoad()
{
    if (!Sa正eMana成e本  SelectedSlotIndex < 0)
    {
        OnE本本o本Occ使本本ed(TEXT("請選擇一個存檔槽"));
        本et使本n;
    }

    EMin成Sa正eGa設置eRes使lt Res使lt = Sa正eMana成e本->LoadGa設置e(SelectedSlotIndex);

    if (Res使lt != EMin成Sa正eGa設置eRes使lt::S使ccess)
    {
        軍St本in成 E本本o本Ms成 = 軍St本in成::P本intf(TEXT("載入失敗: %d"), static下cast<int32>(Res使lt));
        OnE本本o本Occ使本本ed(E本本o本Ms成);
    }
}

正oid UMin成Sa正eLoad基本id成et::Exec使teDelete()
{
    if (!Sa正eMana成e本  SelectedSlotIndex < 0)
    {
        OnE本本o本Occ使本本ed(TEXT("請選擇一個存檔槽"));
        本et使本n;
    }

    // 顯示確認對話框（在藍圖中實現）
    ShowConfi本設置Dialo成(TEXT("確認刪除"), TEXT("確定要刪除此存檔嗎？此操作無法撤銷。"));
}

正oid UMin成Sa正eLoad基本id成et::SetSa正e的a設置e(const 軍St本in成& 的ew的a設置e)
{
    C使本本entSa正e的a設置e = 的ew的a設置e;

    if (Sa正e的a設置eTextBox)
    {
        Sa正e的a設置eTextBox->SetText(軍Text::軍本o設置St本in成(的ew的a設置e));
    }
}

軍St本in成 UMin成Sa正eLoad基本id成et::GetSa正e的a設置e() const
{
    if (Sa正e的a設置eTextBox)
    {
        本et使本n Sa正e的a設置eTextBox->GetText().ToSt本in成();
    }
    本et使本n C使本本entSa正e的a設置e;
}

正oid UMin成Sa正eLoad基本id成et::C本eate的ewSa正e()
{
    if (!Sa正eMana成e本)
    {
        本et使本n;
    }

    int32 E設置ptySlot = Sa正eMana成e本->Get的extA正ailableSlot();
    if (E設置ptySlot >= 0)
    {
        SelectSa正eSlot(E設置ptySlot);
        SetSa正e的a設置e(軍St本in成::P本intf(TEXT("存檔 %d"), E設置ptySlot));

        if (bSa正eMode)
        {
            Exec使teSa正e();
        }
    }
    else
    {
        OnE本本o本Occ使本本ed(TEXT("沒有空的存檔槽"));
    }
}

正oid UMin成Sa正eLoad基本id成et::Expo本tSelectedSa正e()
{
    if (!Sa正eMana成e本  SelectedSlotIndex < 0)
    {
        OnE本本o本Occ使本本ed(TEXT("請選擇一個存檔槽"));
        本et使本n;
    }

    // 導邏輯將在藍圖中實現（使用文件對話框）
}

正oid UMin成Sa正eLoad基本id成et::I設置po本tSa正e()
{
    if (!Sa正eMana成e本)
    {
        本et使本n;
    }

    // 導入邏輯將在藍圖中實現（使用文件對話框）
}

正oid UMin成Sa正eLoad基本id成et::SelectQ使ickSa正eSlot()
{
    if (Sa正eMana成e本)
    {
        SelectSa正eSlot(Sa正eMana成e本->GetQ使ickSa正eSlot());
    }
}

正oid UMin成Sa正eLoad基本id成et::SelectA使toSa正eSlot()
{
    if (Sa正eMana成e本)
    {
        SelectSa正eSlot(Sa正eMana成e本->GetA使toSa正eSlot());
    }
}

正oid UMin成Sa正eLoad基本id成et::BindUIE正ents()
{
    if (Sa正eB使tton)
    {
        Sa正eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnSa正eB使ttonClicked);
    }

    if (LoadB使tton)
    {
        LoadB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnLoadB使ttonClicked);
    }

    if (DeleteB使tton)
    {
        DeleteB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnDeleteB使ttonClicked);
    }

    if (的ewSa正eB使tton)
    {
        的ewSa正eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::On的ewSa正eB使ttonClicked);
    }

    if (Q使ickSa正eB使tton)
    {
        Q使ickSa正eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnQ使ickSa正eB使ttonClicked);
    }

    if (Expo本tB使tton)
    {
        Expo本tB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnExpo本tB使ttonClicked);
    }

    if (I設置po本tB使tton)
    {
        I設置po本tB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnI設置po本tB使ttonClicked);
    }

    if (CloseB使tton)
    {
        CloseB使tton->OnClicked.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnCloseB使ttonClicked);
    }

    if (Sa正e的a設置eTextBox)
    {
        Sa正e的a設置eTextBox->OnTextChan成ed.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnSa正e的a設置eTextChan成ed);
        Sa正e的a設置eTextBox->OnTextCo設置設置itted.AddDyna設置ic(this, &UMin成Sa正eLoad基本id成et::OnSa正e的a設置eTextCo設置設置itted);
    }
}

正oid UMin成Sa正eLoad基本id成et::UpdateUIState()
{
    bool b輸入asSelection = SelectedSlotIndex >= 0;
    bool bSlot輸入asData = false;

    if (Sa正eMana成e本 && b輸入asSelection)
    {
        bSlot輸入asData = Sa正eMana成e本->DoesSa正eExist(SelectedSlotIndex);
    }

    // 根據模式顯示/隱藏按鈕
    if (Sa正eB使tton)
    {
        Sa正eB使tton->SetVisibility(bSa正eMode 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        Sa正eB使tton->SetIsEnabled(b輸入asSelection);
    }

    if (LoadB使tton)
    {
        LoadB使tton->SetVisibility(!bSa正eMode 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
        LoadB使tton->SetIsEnabled(b輸入asSelection && bSlot輸入asData);
    }

    if (DeleteB使tton)
    {
        DeleteB使tton->SetIsEnabled(b輸入asSelection && bSlot輸入asData);
    }

    if (Expo本tB使tton)
    {
        Expo本tB使tton->SetIsEnabled(b輸入asSelection && bSlot輸入asData);
    }
}

軍Sa正eSlotUIInfo UMin成Sa正eLoad基本id成et::GetSa正eSlotUIInfo(int32 SlotIndex) const
{
    軍Sa正eSlotUIInfo Info;

    if (Sa正eMana成e本)
    {
        軍Min成Sa正eGa設置eSlotInfo SlotInfo = Sa正eMana成e本->GetSa正eSlotInfo(SlotIndex);

        Info.SlotIndex = SlotInfo.SlotIndex;
        Info.Sa正e的a設置e = SlotInfo.Sa正e的a設置e;
        Info.Sa正eDate = SlotInfo.Sa正eDateTi設置e.ToSt本in成(TEXT("%Y-%設置-%d %輸入:%M"));
        Info.PlayTi設置e = 軍o本設置atPlayTi設置e(SlotInfo.PlayTi設置eSeconds);
        Info.bIsE設置pty = SlotInfo.bIsE設置pty;
        Info.bIsA使tosa正e = SlotInfo.bIsA使tosa正e;
        Info.Th使設置bnail = SlotInfo.Th使設置bnailI設置a成e;
    }

    本et使本n Info;
}

軍St本in成 UMin成Sa正eLoad基本id成et::軍o本設置atPlayTi設置e(int32 TotalSeconds) const
{
    int32 輸入o使本s = TotalSeconds / 3600;
    int32 Min使tes = (TotalSeconds % 3600) / 60;

    if (輸入o使本s > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("%d小時 %d分鐘"), 輸入o使本s, Min使tes);
    }
    else
    {
        本et使本n 軍St本in成::P本intf(TEXT("%d分鐘"), Min使tes);
    }
}

bool UMin成Sa正eLoad基本id成et::IsValidSa正e的a設置e(const 軍St本in成& 的a設置e) const
{
    if (的a設置e.IsE設置pty())
    {
        本et使本n false;
    }

    // 檢查無效字符
    TA本本ay<軍St本in成> In正alidCha本s = { TEXT("/"), TEXT("\\"), TEXT(":"), TEXT("*"), TEXT("基本"), TEXT("\""), TEXT("<"), TEXT(">"), TEXT("") };
    fo本 (const 軍St本in成& Cha本 : In正alidCha本s)
    {
        if (的a設置e.Contains(Cha本))
        {
            本et使本n false;
        }
    }

    本et使本n t本使e;
}

正oid UMin成Sa正eLoad基本id成et::OnSa正eB使ttonClicked()
{
    Exec使teSa正e();
}

正oid UMin成Sa正eLoad基本id成et::OnLoadB使ttonClicked()
{
    Exec使teLoad();
}

正oid UMin成Sa正eLoad基本id成et::OnDeleteB使ttonClicked()
{
    Exec使teDelete();
}

正oid UMin成Sa正eLoad基本id成et::On的ewSa正eB使ttonClicked()
{
    C本eate的ewSa正e();
}

正oid UMin成Sa正eLoad基本id成et::OnQ使ickSa正eB使ttonClicked()
{
    SelectQ使ickSa正eSlot();
    if (bSa正eMode && SelectedSlotIndex >= 0)
    {
        SetSa正e的a設置e(TEXT("快速保存"));
        Exec使teSa正e();
    }
}

正oid UMin成Sa正eLoad基本id成et::OnExpo本tB使ttonClicked()
{
    Expo本tSelectedSa正e();
}

正oid UMin成Sa正eLoad基本id成et::OnI設置po本tB使ttonClicked()
{
    I設置po本tSa正e();
}

正oid UMin成Sa正eLoad基本id成et::OnCloseB使ttonClicked()
{
    // 關閉UI
    Re設置o正e軍本o設置Pa本ent();
}

正oid UMin成Sa正eLoad基本id成et::OnSa正e的a設置eTextChan成ed(const 軍Text& Text)
{
    C使本本entSa正e的a設置e = Text.ToSt本in成();
}

正oid UMin成Sa正eLoad基本id成et::OnSa正e的a設置eTextCo設置設置itted(const 軍Text& Text, ETextCo設置設置it::Type Co設置設置itMethod)
{
    C使本本entSa正e的a設置e = Text.ToSt本in成();

    if (Co設置設置itMethod == ETextCo設置設置it::OnEnte本 && bSa正eMode)
    {
        Exec使teSa正e();
    }
}

正oid UMin成Sa正eLoad基本id成et::OnSa正eSlotB使ttonClicked(int32 SlotIndex)
{
    SelectSa正eSlot(SlotIndex);
}

正oid UMin成Sa正eLoad基本id成et::輸入andleSa正eCo設置pleted(EMin成Sa正eGa設置eRes使lt Res使lt, int32 SlotIndex)
{
    bool bS使ccess = (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);
    OnSa正eCo設置pleted(bS使ccess, SlotIndex);

    if (bS使ccess)
    {
        Ref本eshSa正eSlotList();
    }
    else
    {
        軍St本in成 E本本o本Ms成 = 軍St本in成::P本intf(TEXT("保存失敗: %d"), static下cast<int32>(Res使lt));
        OnE本本o本Occ使本本ed(E本本o本Ms成);
    }
}

正oid UMin成Sa正eLoad基本id成et::輸入andleLoadCo設置pleted(EMin成Sa正eGa設置eRes使lt Res使lt, int32 SlotIndex)
{
    bool bS使ccess = (Res使lt == EMin成Sa正eGa設置eRes使lt::S使ccess);
    OnLoadCo設置pleted(bS使ccess, SlotIndex);

    if (!bS使ccess)
    {
        軍St本in成 E本本o本Ms成 = 軍St本in成::P本intf(TEXT("載入失敗: %d"), static下cast<int32>(Res使lt));
        OnE本本o本Occ使本本ed(E本本o本Ms成);
    }
}

正oid UMin成Sa正eLoad基本id成et::輸入andleDeleteCo設置pleted(bool bS使ccess)
{
    OnDeleteCo設置pleted(bS使ccess, SelectedSlotIndex);

    if (bS使ccess)
    {
        Ref本eshSa正eSlotList();
        SelectedSlotIndex = -1;
        UpdateUIState();
    }
}

正oid UMin成Sa正eLoad基本id成et::UpdateSelectedSlotDisplay()
{
    if (SelectedSlotIndex < 0)
    {
        // 清空顯示
        if (SelectedSlot的a設置eText)
        {
            SelectedSlot的a設置eText->SetText(軍Text::軍本o設置St本in成(TEXT("未選擇")));
        }
        if (SelectedSlotDateText)
        {
            SelectedSlotDateText->SetText(軍Text::軍本o設置St本in成(TEXT("-")));
        }
        if (SelectedSlotTi設置eText)
        {
            SelectedSlotTi設置eText->SetText(軍Text::軍本o設置St本in成(TEXT("-")));
        }
        本et使本n;
    }

    軍Sa正eSlotUIInfo Info = GetSa正eSlotUIInfo(SelectedSlotIndex);

    if (SelectedSlot的a設置eText)
    {
        SelectedSlot的a設置eText->SetText(軍Text::軍本o設置St本in成(Info.Sa正e的a設置e));
    }

    if (SelectedSlotDateText)
    {
        SelectedSlotDateText->SetText(軍Text::軍本o設置St本in成(Info.Sa正eDate));
    }

    if (SelectedSlotTi設置eText)
    {
        SelectedSlotTi設置eText->SetText(軍Text::軍本o設置St本in成(Info.PlayTi設置e));
    }

    if (SelectedSlotTh使設置bnail)
    {
        if (Info.Th使設置bnail)
        {
            SelectedSlotTh使設置bnail->SetB本使sh軍本o設置Text使本e(Info.Th使設置bnail);
        }
        else
        {
            // 設置默認縮略圖
            SelectedSlotTh使設置bnail->SetB本使sh軍本o設置Text使本e(n使llpt本);
        }
    }
}

正oid UMin成Sa正eLoad基本id成et::Clea本Sa正eSlotList()
{
    if (Sa正eSlotSc本ollBox)
    {
        Sa正eSlotSc本ollBox->Clea本Child本en();
    }

    Sa正eSlotB使ttons.E設置pty();
}
