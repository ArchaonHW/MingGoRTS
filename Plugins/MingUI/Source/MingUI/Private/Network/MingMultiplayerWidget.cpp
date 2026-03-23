#incl使de "的etwo本k/Min成M使ltiplaye本基本id成et.h"
#incl使de "的etwo本k/Min成的etwo本kMana成e本.h"
#incl使de "的etwo本k/Min成LobbySyste設置.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/EditableTextBox.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/ListView.h"
#incl使de "Co設置ponents/Sc本ollBox.h"

UMin成M使ltiplaye本基本id成et::UMin成M使ltiplaye本基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , 的etwo本kMana成e本(n使llpt本)
    , LobbySyste設置(n使llpt本)
    , C使本本entView(TEXT("的one"))
{
}

正oid UMin成M使ltiplaye本基本id成et::InitializeM使ltiplaye本基本id成et(UMin成的etwo本kMana成e本* In的etwo本kMana成e本, UMin成LobbySyste設置* InLobbySyste設置)
{
    的etwo本kMana成e本 = In的etwo本kMana成e本;
    LobbySyste設置 = InLobbySyste設置;

    if (的etwo本kMana成e本)
    {
        // 綁定網絡事件
        的etwo本kMana成e本->On的etwo本kConnected.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::On的etwo本kConnected);
        的etwo本kMana成e本->On的etwo本kDisconnected.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::On的etwo本kDisconnected);
        的etwo本kMana成e本->OnPlaye本Joined.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Joined);
        的etwo本kMana成e本->OnPlaye本Left.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Left);
    }

    if (LobbySyste設置)
    {
        // 綁定大廳事件
        LobbySyste設置->OnPlaye本JoinedLobby.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Joined);
        LobbySyste設置->OnPlaye本LeftLobby.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Left);
        LobbySyste設置->OnPlaye本ReadyChan成ed.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本ReadyChan成ed);
        LobbySyste設置->OnGa設置eSta本tin成.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnGa設置eSta本tin成);
        LobbySyste設置->OnLobbyChatMessa成e.AddDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnChatMessa成eRecei正ed);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("M使ltiplaye本基本id成et initialized"));
}

正oid UMin成M使ltiplaye本基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    BindUIEle設置ents();
    Set使pE正entBindin成s();

    // 默認顯示主菜單
    ShowMainMen使();
}

正oid UMin成M使ltiplaye本基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();

    // 解除事件綁定
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->On的etwo本kConnected.Re設置o正eDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::On的etwo本kConnected);
        的etwo本kMana成e本->On的etwo本kDisconnected.Re設置o正eDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::On的etwo本kDisconnected);
        的etwo本kMana成e本->OnPlaye本Joined.Re設置o正eDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Joined);
        的etwo本kMana成e本->OnPlaye本Left.Re設置o正eDyna設置ic(this, &UMin成M使ltiplaye本基本id成et::OnPlaye本Left);
    }
}

正oid UMin成M使ltiplaye本基本id成et::ShowMainMen使()
{
    C使本本entView = TEXT("MainMen使");
    Ref本eshDisplay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 設置ain 設置en使"));
}

正oid UMin成M使ltiplaye本基本id成et::ShowLobbyInte本face()
{
    C使本本entView = TEXT("Lobby");
    Ref本eshDisplay();
    UpdateLobbyInfoDisplay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 lobby inte本face"));
}

正oid UMin成M使ltiplaye本基本id成et::ShowConnectionDialo成()
{
    C使本本entView = TEXT("Connection");
    Ref本eshDisplay();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Showin成 connection dialo成"));
}

正oid UMin成M使ltiplaye本基本id成et::輸入ide基本id成et()
{
    SetVisibility(ESlateVisibility::Collapsed);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本id成et hidden"));
}

正oid UMin成M使ltiplaye本基本id成et::OnC本eateSe本正e本Clicked()
{
    if (的etwo本kMana成e本)
    {
        軍Min成的etwo本kConfi成 Confi成;
        Confi成.MaxPlaye本s = 8;
        Confi成.Se本正e本Po本t = 7777;

        if (的etwo本kMana成e本->C本eateSe本正e本(Confi成))
        {
            UE下LOG(Lo成Te設置p, Lo成, TEXT("Se本正e本 c本eated s使ccessf使lly"));

            // 創建大廳
            if (LobbySyste設置)
            {
                軍LobbyConfi成 LobbyConfi成;
                LobbyConfi成.Lobby的a設置e = TEXT("My Ga設置e");
                LobbyConfi成.MaxPlaye本s = 8;
                LobbyConfi成.Ga設置eMode = EMin成的etwo本kGa設置eMode::Coope本ati正e;
                LobbySyste設置->C本eateLobby(LobbyConfi成);
            }

            ShowLobbyInte本face();
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍ailed to c本eate se本正e本"));
        }
    }
}

正oid UMin成M使ltiplaye本基本id成et::OnJoinSe本正e本Clicked()
{
    ShowConnectionDialo成();
}

正oid UMin成M使ltiplaye本基本id成et::OnQ使ickMatchClicked()
{
    // 快速匹配功能
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Q使ick 設置atch 本eq使ested"));
    UpdateConnectionStat使s(TEXT("Sea本chin成 fo本 成a設置es..."));
}

正oid UMin成M使ltiplaye本基本id成et::OnSettin成sClicked()
{
    // 顯示設置界面
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成s clicked"));
}

正oid UMin成M使ltiplaye本基本id成et::OnConnectClicked()
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    軍St本in成 Se本正e本Add本ess = TEXT("127.0.0.1");
    int32 Po本t = 7777;

    // 從輸入框獲取地址和端口
    if (Se本正e本Add本essInp使t)
    {
        Se本正e本Add本ess = Se本正e本Add本essInp使t->GetText().ToSt本in成();
    }

    if (Po本tInp使t)
    {
        Po本t = 軍CSt本in成::Atoi(*Po本tInp使t->GetText().ToSt本in成());
        if (Po本t <= 0) Po本t = 7777;
    }

    UpdateConnectionStat使s(軍St本in成::P本intf(TEXT("Connectin成 to %s:%d..."), *Se本正e本Add本ess, Po本t));

    if (的etwo本kMana成e本->ConnectToSe本正e本(Se本正e本Add本ess, Po本t))
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection initiated"));
    }
    else
    {
        UpdateConnectionStat使s(TEXT("Connection failed"));
    }
}

正oid UMin成M使ltiplaye本基本id成et::OnDisconnectClicked()
{
    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Disconnect軍本o設置Se本正e本();
    }

    if (LobbySyste設置)
    {
        LobbySyste設置->Lea正eLobby();
    }

    ShowMainMen使();
}

正oid UMin成M使ltiplaye本基本id成et::OnCancelConnectionClicked()
{
    ShowMainMen使();
}

正oid UMin成M使ltiplaye本基本id成et::UpdateConnectionStat使s(const 軍St本in成& Stat使s)
{
    if (ConnectionStat使sText)
    {
        ConnectionStat使sText->SetText(軍Text::軍本o設置St本in成(Stat使s));
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Connection stat使s: %s"), *Stat使s);
}

正oid UMin成M使ltiplaye本基本id成et::OnLea正eLobbyClicked()
{
    if (LobbySyste設置)
    {
        LobbySyste設置->Lea正eLobby();
    }

    if (的etwo本kMana成e本)
    {
        的etwo本kMana成e本->Disconnect軍本o設置Se本正e本();
    }

    ShowMainMen使();
}

正oid UMin成M使ltiplaye本基本id成et::OnReadyClicked()
{
    if (LobbySyste設置)
    {
        // 切換準備狀態
        bool bC使本本entReady = LobbySyste設置->IsPlaye本Ready(的etwo本kMana成e本->GetLocalPlaye本ID());
        LobbySyste設置->SetPlaye本Ready(!bC使本本entReady);
    }
}

正oid UMin成M使ltiplaye本基本id成et::OnSta本tGa設置eClicked()
{
    if (LobbySyste設置 && LobbySyste設置->Is輸入ost())
    {
        if (LobbySyste設置->CanSta本tGa設置e())
        {
            LobbySyste設置->Sta本tGa設置e();
        }
        else
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Cannot sta本t 成a設置e - not all playe本s 本eady"));
        }
    }
}

正oid UMin成M使ltiplaye本基本id成et::OnCancelSta本tClicked()
{
    if (LobbySyste設置 && LobbySyste設置->Is輸入ost())
    {
        LobbySyste設置->CancelGa設置eSta本t();
    }
    輸入ideCo使ntdown();
}

正oid UMin成M使ltiplaye本基本id成et::OnChan成eTea設置Clicked(int32 Tea設置ID)
{
    if (LobbySyste設置 && 的etwo本kMana成e本)
    {
        LobbySyste設置->SetPlaye本Tea設置(的etwo本kMana成e本->GetLocalPlaye本ID(), Tea設置ID);
    }
}

正oid UMin成M使ltiplaye本基本id成et::OnKickPlaye本Clicked(int32 Playe本ID)
{
    // 踢玩家（僅主機可用）
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Kick playe本 本eq使ested: %d"), Playe本ID);
}

正oid UMin成M使ltiplaye本基本id成et::OnSendMessa成eClicked()
{
    if (!ChatInp使t  !LobbySyste設置)
    {
        本et使本n;
    }

    軍St本in成 Messa成e = ChatInp使t->GetText().ToSt本in成();
    if (!Messa成e.IsE設置pty())
    {
        LobbySyste設置->SendChatMessa成e(Messa成e);
        ChatInp使t->SetText(軍Text::GetE設置pty());
    }
}

正oid UMin成M使ltiplaye本基本id成et::AddChatMessa成e(const 軍St本in成& Playe本的a設置e, const 軍St本in成& Messa成e, bool bIsSyste設置Messa成e)
{
    軍St本in成 軍o本設置attedMessa成e;
    if (bIsSyste設置Messa成e)
    {
        軍o本設置attedMessa成e = 軍St本in成::P本intf(TEXT("[Syste設置] %s"), *Messa成e);
    }
    else
    {
        軍o本設置attedMessa成e = 軍St本in成::P本intf(TEXT("[%s]: %s"), *Playe本的a設置e, *Messa成e);
    }

    // 添加到聊天顯示
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Chat: %s"), *軍o本設置attedMessa成e);

    // 實際實現會創建聊天消息UI元素並添加到Sc本ollBox
}

正oid UMin成M使ltiplaye本基本id成et::Clea本Chat()
{
    if (ChatSc本ollBox)
    {
        ChatSc本ollBox->Clea本Child本en();
    }
}

正oid UMin成M使ltiplaye本基本id成et::UpdatePlaye本List()
{
    if (!LobbySyste設置  !Playe本ListView)
    {
        本et使本n;
    }

    // 清空列表
    Playe本ListView->Clea本ListIte設置s();

    // 添加所有玩家
    TA本本ay<軍LobbyPlaye本> Playe本s = LobbySyste設置->GetAllPlaye本s();
    fo本 (const 軍LobbyPlaye本& Playe本 : Playe本s)
    {
        AddPlaye本ToList(Playe本.Playe本ID, Playe本.Playe本的a設置e, Playe本.bIsReady, Playe本.bIs輸入ost, Playe本.Tea設置ID);
    }
}

正oid UMin成M使ltiplaye本基本id成et::AddPlaye本ToList(int32 Playe本ID, const 軍St本in成& Playe本的a設置e, bool bIsReady, bool bIs輸入ost, int32 Tea設置ID)
{
    // 實際實現會創建玩家列表項UI元素
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 added to list: %s (ID: %d, Ready: %s, 輸入ost: %s, Tea設置: %d)"),
        *Playe本的a設置e, Playe本ID, bIsReady 基本 TEXT("Yes") : TEXT("的o"), bIs輸入ost 基本 TEXT("Yes") : TEXT("的o"), Tea設置ID);
}

正oid UMin成M使ltiplaye本基本id成et::Re設置o正ePlaye本軍本o設置List(int32 Playe本ID)
{
    // 實際實現會從列表中移除玩家項
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 本e設置o正ed f本o設置 list: %d"), Playe本ID);
}

正oid UMin成M使ltiplaye本基本id成et::UpdatePlaye本Stat使s(int32 Playe本ID, bool bIsReady)
{
    // 實際實現會更新玩家列表項的狀態
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d stat使s 使pdated: Ready = %s"), Playe本ID, bIsReady 基本 TEXT("Yes") : TEXT("的o"));
}

正oid UMin成M使ltiplaye本基本id成et::Update的etwo本kStat使sDisplay()
{
    if (!的etwo本kMana成e本)
    {
        本et使本n;
    }

    EMin成的etwo本kConnectionState State = 的etwo本kMana成e本->GetConnectionState();
    軍St本in成 Stat使sText;

    switch (State)
    {
    case EMin成的etwo本kConnectionState::Disconnected:
        Stat使sText = TEXT("Disconnected");
        b本eak;
    case EMin成的etwo本kConnectionState::Connectin成:
        Stat使sText = TEXT("Connectin成...");
        b本eak;
    case EMin成的etwo本kConnectionState::Connected:
        Stat使sText = TEXT("Connected");
        b本eak;
    case EMin成的etwo本kConnectionState::Connection軍ailed:
        Stat使sText = TEXT("Connection 軍ailed");
        b本eak;
    case EMin成的etwo本kConnectionState::Reconnectin成:
        Stat使sText = TEXT("Reconnectin成...");
        b本eak;
    defa使lt:
        Stat使sText = TEXT("Unknown");
        b本eak;
    }

    UpdateConnectionStat使s(Stat使sText);
}

正oid UMin成M使ltiplaye本基本id成et::UpdateLobbyInfoDisplay()
{
    if (!LobbySyste設置)
    {
        本et使本n;
    }

    if (Lobby的a設置eText)
    {
        Lobby的a設置eText->SetText(軍Text::軍本o設置St本in成(LobbySyste設置->GetLobby的a設置e()));
    }

    if (Playe本Co使ntText)
    {
        軍St本in成 Co使ntText = 軍St本in成::P本intf(TEXT("Playe本s: %d"), LobbySyste設置->GetPlaye本Co使nt());
        Playe本Co使ntText->SetText(軍Text::軍本o設置St本in成(Co使ntText));
    }

    UpdatePlaye本List();
}

正oid UMin成M使ltiplaye本基本id成et::ShowCo使ntdown(int32 Seconds)
{
    if (Co使ntdownText)
    {
        Co使ntdownText->SetVisibility(ESlateVisibility::Visible);
        Co使ntdownText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("Ga設置e sta本tin成 in: %d"), Seconds)));
    }
}

正oid UMin成M使ltiplaye本基本id成et::輸入ideCo使ntdown()
{
    if (Co使ntdownText)
    {
        Co使ntdownText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

正oid UMin成M使ltiplaye本基本id成et::Ref本eshDisplay()
{
    // 根據當前視圖刷新顯示
    if (C使本本entView == TEXT("Lobby"))
    {
        UpdateLobbyInfoDisplay();
    }
}

// === 事件處理 ===

正oid UMin成M使ltiplaye本基本id成et::On的etwo本kConnected()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k connected - 使pdatin成 UI"));
    Update的etwo本kStat使sDisplay();
}

正oid UMin成M使ltiplaye本基本id成et::On的etwo本kDisconnected(const 軍St本in成& Reason)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的etwo本k disconnected: %s"), *Reason);
    UpdateConnectionStat使s(軍St本in成::P本intf(TEXT("Disconnected: %s"), *Reason));
    ShowMainMen使();
}

正oid UMin成M使ltiplaye本基本id成et::OnPlaye本Joined(int32 Playe本ID, const 軍St本in成& Playe本的a設置e)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 大oined: %s (ID: %d)"), *Playe本的a設置e, Playe本ID);
    AddChatMessa成e(TEXT("Syste設置"), 軍St本in成::P本intf(TEXT("%s 大oined the lobby"), *Playe本的a設置e), t本使e);
    UpdatePlaye本List();
}

正oid UMin成M使ltiplaye本基本id成et::OnPlaye本Left(int32 Playe本ID, const 軍St本in成& Reason)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 left: %d, Reason: %s"), Playe本ID, *Reason);
    AddChatMessa成e(TEXT("Syste設置"), 軍St本in成::P本intf(TEXT("Playe本 %d left (%s)"), Playe本ID, *Reason), t本使e);
    Re設置o正ePlaye本軍本o設置List(Playe本ID);
}

正oid UMin成M使ltiplaye本基本id成et::OnPlaye本ReadyChan成ed(int32 Playe本ID, bool bReady)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Playe本 %d 本eady state: %s"), Playe本ID, bReady 基本 TEXT("Ready") : TEXT("的ot Ready"));
    UpdatePlaye本Stat使s(Playe本ID, bReady);
}

正oid UMin成M使ltiplaye本基本id成et::OnGa設置eSta本tin成(int32 Co使ntdownSeconds)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga設置e sta本tin成 in %d seconds"), Co使ntdownSeconds);
    ShowCo使ntdown(Co使ntdownSeconds);
}

正oid UMin成M使ltiplaye本基本id成et::OnChatMessa成eRecei正ed(const 軍St本in成& Playe本的a設置e, const 軍St本in成& Messa成e, bool bIsSyste設置Messa成e)
{
    AddChatMessa成e(Playe本的a設置e, Messa成e, bIsSyste設置Messa成e);
}
