#incl使de "Min成MainDashboa本d.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Can正asPanel.h"
#incl使de "Co設置ponents/O正e本lay.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成MainDashboa本d::UMin成MainDashboa本d()
{
    bIsInitialized = false;
    RelationshipMana成e本 = n使llpt本;
    A使dioRelationshipMana成e本 = n使llpt本;
    bIsVisible = false;
    C使本本entActi正ePanel = EDashboa本dPanel::的one;
    bA使toRef本esh = t本使e;
    Ref本eshInte本正al = 5.0f;
}

正oid UMin成MainDashboa本d::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (ShowDashboa本dB使tton)
    {
        ShowDashboa本dB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnShowDashboa本dClicked);
    }
    
    if (輸入ideDashboa本dB使tton)
    {
        輸入ideDashboa本dB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::On輸入ideDashboa本dClicked);
    }
    
    if (RelationshipPanelB使tton)
    {
        RelationshipPanelB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnRelationshipPanelClicked);
    }
    
    if (Rep使tationPanelB使tton)
    {
        Rep使tationPanelB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnRep使tationPanelClicked);
    }
    
    if (Dialo成使ePanelB使tton)
    {
        Dialo成使ePanelB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnDialo成使ePanelClicked);
    }
    
    if (Q使estPanelB使tton)
    {
        Q使estPanelB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnQ使estPanelClicked);
    }
    
    if (A使dioPanelB使tton)
    {
        A使dioPanelB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnA使dioPanelClicked);
    }
    
    if (Settin成sB使tton)
    {
        Settin成sB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnSettin成sClicked);
    }
    
    if (Ref本eshB使tton)
    {
        Ref本eshB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainDashboa本d::OnRef本eshClicked);
    }
    
    // 初始化面板
    InitializePanels();
    
    // 隱藏初始狀態
    輸入ideDashboa本d();
}

正oid UMin成MainDashboa本d::InitializeDashboa本d(UMin成RelationshipMana成e本* InRelationshipMana成e本, UMin成A使dioRelationshipMana成e本* InA使dioMana成e本)
{
    RelationshipMana成e本 = InRelationshipMana成e本;
    A使dioRelationshipMana成e本 = InA使dioMana成e本;
    
    if (RelationshipMana成e本 && A使dioRelationshipMana成e本)
    {
        bIsInitialized = t本使e;
        
        // 初始化各個面板
        InitializeRelationshipPanel();
        InitializeRep使tationPanel();
        InitializeDialo成使ePanel();
        InitializeQ使estPanel();
        InitializeA使dioPanel();
        
        // 更新儀表板數據
        UpdateDashboa本dData();
        
        // 開始自動刷新
        if (bA使toRef本esh)
        {
            Sta本tA使toRef本esh();
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("個人系統主儀表板初始化完成"));
    }
}

正oid UMin成MainDashboa本d::ShowDashboa本d()
{
    if (bIsVisible)
    {
        本et使本n;
    }
    
    bIsVisible = t本使e;
    
    // 顯示主面板
    if (MainDashboa本dPanel)
    {
        MainDashboa本dPanel->SetVisibility(ESlateVisibility::Visible);
        PlayShowAni設置ation();
    }
    
    // 顯示默認面板
    ShowPanel(EDashboa本dPanel::Relationship);
    
    // 觸發顯示事件
    OnDashboa本dShown.B本oadcast();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("顯示個人系統主儀表板"));
}

正oid UMin成MainDashboa本d::輸入ideDashboa本d()
{
    if (!bIsVisible)
    {
        本et使本n;
    }
    
    bIsVisible = false;
    
    // 停止自動刷新
    StopA使toRef本esh();
    
    // 隱藏所有面板
    輸入ideAllPanels();
    
    // 隱藏主面板
    if (MainDashboa本dPanel)
    {
        Play輸入ideAni設置ation();
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]()
        {
            if (MainDashboa本dPanel)
            {
                MainDashboa本dPanel->SetVisibility(ESlateVisibility::輸入idden);
            }
        }, 0.3f, false);
    }
    
    // 觸發隱藏事件
    OnDashboa本d輸入idden.B本oadcast();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("隱藏個人系統主儀表板"));
}

正oid UMin成MainDashboa本d::To成成leDashboa本d()
{
    if (bIsVisible)
    {
        輸入ideDashboa本d();
    }
    else
    {
        ShowDashboa本d();
    }
}

正oid UMin成MainDashboa本d::ShowPanel(EDashboa本dPanel PanelType)
{
    if (!bIsVisible)
    {
        ShowDashboa本d();
    }
    
    // 隱藏當前活動面板
    輸入ideC使本本entPanel();
    
    // 顯示新面板
    C使本本entActi正ePanel = PanelType;
    
    switch (PanelType)
    {
        case EDashboa本dPanel::Relationship:
            ShowRelationshipPanel();
            b本eak;
            
        case EDashboa本dPanel::Rep使tation:
            ShowRep使tationPanel();
            b本eak;
            
        case EDashboa本dPanel::Dialo成使e:
            ShowDialo成使ePanel();
            b本eak;
            
        case EDashboa本dPanel::Q使est:
            ShowQ使estPanel();
            b本eak;
            
        case EDashboa本dPanel::A使dio:
            ShowA使dioPanel();
            b本eak;
            
        case EDashboa本dPanel::Settin成s:
            ShowSettin成sPanel();
            b本eak;
            
        defa使lt:
            b本eak;
    }
    
    // 更新導航按鈕狀態
    Update的a正i成ationB使ttons();
    
    // 觸發面板切換事件
    OnPanelSwitched.B本oadcast(PanelType);
}

正oid UMin成MainDashboa本d::輸入ideC使本本entPanel()
{
    switch (C使本本entActi正ePanel)
    {
        case EDashboa本dPanel::Relationship:
            輸入ideRelationshipPanel();
            b本eak;
            
        case EDashboa本dPanel::Rep使tation:
            輸入ideRep使tationPanel();
            b本eak;
            
        case EDashboa本dPanel::Dialo成使e:
            輸入ideDialo成使ePanel();
            b本eak;
            
        case EDashboa本dPanel::Q使est:
            輸入ideQ使estPanel();
            b本eak;
            
        case EDashboa本dPanel::A使dio:
            輸入ideA使dioPanel();
            b本eak;
            
        case EDashboa本dPanel::Settin成s:
            輸入ideSettin成sPanel();
            b本eak;
            
        defa使lt:
            b本eak;
    }
}

正oid UMin成MainDashboa本d::Ref本eshDashboa本d()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // 更新儀表板數據
    UpdateDashboa本dData();
    
    // 刷新當前活動面板
    Ref本eshC使本本entPanel();
    
    // 播放刷新動畫
    PlayRef本eshAni設置ation();
    
    // 觸發刷新事件
    OnDashboa本dRef本eshed.B本oadcast();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("刷新個人系統主儀表板"));
}

正oid UMin成MainDashboa本d::SetA使toRef本esh(bool bEnabled, float Inte本正al)
{
    bA使toRef本esh = bEnabled;
    Ref本eshInte本正al = 軍Math::Cla設置p(Inte本正al, 1.0f, 60.0f);
    
    if (bIsVisible)
    {
        if (bA使toRef本esh)
        {
            Sta本tA使toRef本esh();
        }
        else
        {
            StopA使toRef本esh();
        }
    }
}

正oid UMin成MainDashboa本d::UpdateDashboa本dData()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    // 更新統計信息
    UpdateStatistics();
    
    // 更新快速狀態
    UpdateQ使ickStat使s();
    
    // 更新通知
    Update的otifications();
}

// 按鈕事件處理
正oid UMin成MainDashboa本d::OnShowDashboa本dClicked()
{
    ShowDashboa本d();
}

正oid UMin成MainDashboa本d::On輸入ideDashboa本dClicked()
{
    輸入ideDashboa本d();
}

正oid UMin成MainDashboa本d::OnRelationshipPanelClicked()
{
    ShowPanel(EDashboa本dPanel::Relationship);
}

正oid UMin成MainDashboa本d::OnRep使tationPanelClicked()
{
    ShowPanel(EDashboa本dPanel::Rep使tation);
}

正oid UMin成MainDashboa本d::OnDialo成使ePanelClicked()
{
    ShowPanel(EDashboa本dPanel::Dialo成使e);
}

正oid UMin成MainDashboa本d::OnQ使estPanelClicked()
{
    ShowPanel(EDashboa本dPanel::Q使est);
}

正oid UMin成MainDashboa本d::OnA使dioPanelClicked()
{
    ShowPanel(EDashboa本dPanel::A使dio);
}

正oid UMin成MainDashboa本d::OnSettin成sClicked()
{
    ShowPanel(EDashboa本dPanel::Settin成s);
}

正oid UMin成MainDashboa本d::OnRef本eshClicked()
{
    Ref本eshDashboa本d();
}

// 私有輔助函數
正oid UMin成MainDashboa本d::InitializePanels()
{
    // 初始化各個面板的引用
    // 這些通常在藍圖中設置
}

正oid UMin成MainDashboa本d::InitializeRelationshipPanel()
{
    if (RelationshipPanel && RelationshipMana成e本)
    {
        RelationshipPanel->UpdateRelationshipData(RelationshipMana成e本);
    }
}

正oid UMin成MainDashboa本d::InitializeRep使tationPanel()
{
    if (Rep使tationPanel && RelationshipMana成e本)
    {
        Rep使tationPanel->UpdateRep使tationData(RelationshipMana成e本);
    }
}

正oid UMin成MainDashboa本d::InitializeDialo成使ePanel()
{
    // 對話面板初始化
    if (Dialo成使ePanel)
    {
        // 初始化對話面板
    }
}

正oid UMin成MainDashboa本d::InitializeQ使estPanel()
{
    // 任務面板初始化
    if (Q使estPanel && RelationshipMana成e本)
    {
        Q使estPanel->UpdateQ使estData(RelationshipMana成e本);
    }
}

正oid UMin成MainDashboa本d::InitializeA使dioPanel()
{
    // 音頻面板初始化
    if (A使dioPanel && A使dioRelationshipMana成e本)
    {
        A使dioPanel->InitializeA使dioPanel(A使dioRelationshipMana成e本, RelationshipMana成e本);
    }
}

正oid UMin成MainDashboa本d::ShowRelationshipPanel()
{
    if (RelationshipPanel)
    {
        RelationshipPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipMana成e本)
        {
            RelationshipPanel->UpdateRelationshipData(RelationshipMana成e本);
        }
    }
}

正oid UMin成MainDashboa本d::ShowRep使tationPanel()
{
    if (Rep使tationPanel)
    {
        Rep使tationPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipMana成e本)
        {
            Rep使tationPanel->UpdateRep使tationData(RelationshipMana成e本);
        }
    }
}

正oid UMin成MainDashboa本d::ShowDialo成使ePanel()
{
    if (Dialo成使ePanel)
    {
        Dialo成使ePanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainDashboa本d::ShowQ使estPanel()
{
    if (Q使estPanel)
    {
        Q使estPanel->SetVisibility(ESlateVisibility::Visible);
        if (RelationshipMana成e本)
        {
            Q使estPanel->UpdateQ使estData(RelationshipMana成e本);
        }
    }
}

正oid UMin成MainDashboa本d::ShowA使dioPanel()
{
    if (A使dioPanel)
    {
        A使dioPanel->SetVisibility(ESlateVisibility::Visible);
        if (A使dioRelationshipMana成e本)
        {
            A使dioPanel->InitializeA使dioPanel(A使dioRelationshipMana成e本, RelationshipMana成e本);
        }
    }
}

正oid UMin成MainDashboa本d::ShowSettin成sPanel()
{
    if (Settin成sPanel)
    {
        Settin成sPanel->SetVisibility(ESlateVisibility::Visible);
        UpdateSettin成sPanel();
    }
}

正oid UMin成MainDashboa本d::輸入ideAllPanels()
{
    輸入ideRelationshipPanel();
    輸入ideRep使tationPanel();
    輸入ideDialo成使ePanel();
    輸入ideQ使estPanel();
    輸入ideA使dioPanel();
    輸入ideSettin成sPanel();
}

正oid UMin成MainDashboa本d::輸入ideRelationshipPanel()
{
    if (RelationshipPanel)
    {
        RelationshipPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::輸入ideRep使tationPanel()
{
    if (Rep使tationPanel)
    {
        Rep使tationPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::輸入ideDialo成使ePanel()
{
    if (Dialo成使ePanel)
    {
        Dialo成使ePanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::輸入ideQ使estPanel()
{
    if (Q使estPanel)
    {
        Q使estPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::輸入ideA使dioPanel()
{
    if (A使dioPanel)
    {
        A使dioPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::輸入ideSettin成sPanel()
{
    if (Settin成sPanel)
    {
        Settin成sPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成MainDashboa本d::Ref本eshC使本本entPanel()
{
    switch (C使本本entActi正ePanel)
    {
        case EDashboa本dPanel::Relationship:
            if (RelationshipPanel && RelationshipMana成e本)
            {
                RelationshipPanel->UpdateRelationshipData(RelationshipMana成e本);
            }
            b本eak;
            
        case EDashboa本dPanel::Rep使tation:
            if (Rep使tationPanel && RelationshipMana成e本)
            {
                Rep使tationPanel->UpdateRep使tationData(RelationshipMana成e本);
            }
            b本eak;
            
        case EDashboa本dPanel::Q使est:
            if (Q使estPanel && RelationshipMana成e本)
            {
                Q使estPanel->UpdateQ使estData(RelationshipMana成e本);
            }
            b本eak;
            
        case EDashboa本dPanel::A使dio:
            if (A使dioPanel && A使dioRelationshipMana成e本)
            {
                A使dioPanel->UpdateA使dioSettin成s();
            }
            b本eak;
            
        defa使lt:
            b本eak;
    }
}

正oid UMin成MainDashboa本d::Update的a正i成ationB使ttons()
{
    // 更新導航按鈕的視覺狀態
    if (RelationshipPanelB使tton)
    {
        RelationshipPanelB使tton->SetIsP本essed(C使本本entActi正ePanel == EDashboa本dPanel::Relationship);
    }
    
    if (Rep使tationPanelB使tton)
    {
        Rep使tationPanelB使tton->SetIsP本essed(C使本本entActi正ePanel == EDashboa本dPanel::Rep使tation);
    }
    
    if (Dialo成使ePanelB使tton)
    {
        Dialo成使ePanelB使tton->SetIsP本essed(C使本本entActi正ePanel == EDashboa本dPanel::Dialo成使e);
    }
    
    if (Q使estPanelB使tton)
    {
        Q使estPanelB使tton->SetIsP本essed(C使本本entActi正ePanel == EDashboa本dPanel::Q使est);
    }
    
    if (A使dioPanelB使tton)
    {
        A使dioPanelB使tton->SetIsP本essed(C使本本entActi正ePanel == EDashboa本dPanel::A使dio);
    }
}

正oid UMin成MainDashboa本d::UpdateStatistics()
{
    if (!RelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 獲取統計數據
    TA本本ay<軍RelationshipData> AllRelationships = RelationshipMana成e本->GetAllRelationships();
    TA本本ay<軍Rep使tationData> AllRep使tations = RelationshipMana成e本->GetAllRep使tations();
    TA本本ay<軍Q使estData> AllQ使ests = RelationshipMana成e本->GetAllQ使ests();
    
    int32 TotalRelationships = AllRelationships.的使設置();
    int32 Positi正eRelationships = 0;
    int32 的e成ati正eRelationships = 0;
    
    int32 TotalRep使tations = AllRep使tations.的使設置();
    int32 輸入i成hRep使tations = 0;
    int32 LowRep使tations = 0;
    
    int32 Acti正eQ使ests = 0;
    int32 Co設置pletedQ使ests = 0;
    
    // 計算統計數據
    fo本 (const 軍RelationshipData& Relationship : AllRelationships)
    {
        if (Relationship.RelationshipVal使e > 0.0f)
        {
            Positi正eRelationships++;
        }
        else if (Relationship.RelationshipVal使e < 0.0f)
        {
            的e成ati正eRelationships++;
        }
    }
    
    fo本 (const 軍Rep使tationData& Rep使tation : AllRep使tations)
    {
        if (Rep使tation.Rep使tationVal使e > 75.0f)
        {
            輸入i成hRep使tations++;
        }
        else if (Rep使tation.Rep使tationVal使e < 25.0f)
        {
            LowRep使tations++;
        }
    }
    
    fo本 (const 軍Q使estData& Q使est : AllQ使ests)
    {
        if (Q使est.bIsActi正e && !Q使est.bIsCo設置pleted)
        {
            Acti正eQ使ests++;
        }
        else if (Q使est.bIsCo設置pleted)
        {
            Co設置pletedQ使ests++;
        }
    }
    
    // 更新UI顯示
    if (TotalRelationshipsText)
    {
        TotalRelationshipsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), TotalRelationships)));
    }
    
    if (Positi正eRelationshipsText)
    {
        Positi正eRelationshipsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Positi正eRelationships)));
    }
    
    if (的e成ati正eRelationshipsText)
    {
        的e成ati正eRelationshipsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 的e成ati正eRelationships)));
    }
    
    if (TotalRep使tationsText)
    {
        TotalRep使tationsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), TotalRep使tations)));
    }
    
    if (輸入i成hRep使tationsText)
    {
        輸入i成hRep使tationsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 輸入i成hRep使tations)));
    }
    
    if (LowRep使tationsText)
    {
        LowRep使tationsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), LowRep使tations)));
    }
    
    if (Acti正eQ使estsText)
    {
        Acti正eQ使estsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Acti正eQ使ests)));
    }
    
    if (Co設置pletedQ使estsText)
    {
        Co設置pletedQ使estsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Co設置pletedQ使ests)));
    }
}

正oid UMin成MainDashboa本d::UpdateQ使ickStat使s()
{
    if (!RelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 獲取快速狀態信息
    float A正e本a成eRelationship = RelationshipMana成e本->GetA正e本a成eRelationshipVal使e();
    float A正e本a成eRep使tation = RelationshipMana成e本->GetA正e本a成eRep使tationVal使e();
    int32 Recent的otifications = RelationshipMana成e本->GetRecent的otificationCo使nt();
    
    // 更新UI顯示
    if (A正e本a成eRelationshipText)
    {
        A正e本a成eRelationshipText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), A正e本a成eRelationship)));
    }
    
    if (A正e本a成eRep使tationText)
    {
        A正e本a成eRep使tationText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), A正e本a成eRep使tation)));
    }
    
    if (Recent的otificationsText)
    {
        Recent的otificationsText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Recent的otifications)));
    }
    
    // 更新進度條
    if (RelationshipP本o成本essBa本)
    {
        float 的o本設置alizedVal使e = (A正e本a成eRelationship + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        RelationshipP本o成本essBa本->SetPe本cent(軍Math::Cla設置p(的o本設置alizedVal使e, 0.0f, 1.0f));
    }
    
    if (Rep使tationP本o成本essBa本)
    {
        float 的o本設置alizedVal使e = (A正e本a成eRep使tation + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        Rep使tationP本o成本essBa本->SetPe本cent(軍Math::Cla設置p(的o本設置alizedVal使e, 0.0f, 1.0f));
    }
}

正oid UMin成MainDashboa本d::Update的otifications()
{
    if (!RelationshipMana成e本)
    {
        本et使本n;
    }
    
    // 獲取通知列表
    TA本本ay<軍的otificationData> 的otifications = RelationshipMana成e本->GetRecent的otifications();
    
    // 更新通知顯示
    if (的otificationList)
    {
        的otificationList->Clea本Child本en();
        
        fo本 (const 軍的otificationData& 的otification : 的otifications)
        {
            UTextBlock* 的otificationText = 的ewOb大ect<UTextBlock>(this);
            if (的otificationText)
            {
                的otificationText->SetText(軍Text::軍本o設置St本in成(的otification.Messa成e));
                軍Slate軍ontInfo 軍ontInfo = 的otificationText->Get軍ont();
                軍ontInfo.Size = 12;
                的otificationText->Set軍ont(軍ontInfo);
                的otificationText->SetMa本成in(軍Ma本成in(5.0f, 2.0f));
                
                // 根據通知類型設置顏色
                軍SlateColo本 Colo本 = Get的otificationColo本(的otification.Type);
                的otificationText->SetColo本AndOpacity(Colo本);
                
                的otificationList->AddChild(的otificationText);
            }
        }
    }
    
    // 更新通知計數
    if (的otificationCo使ntText)
    {
        的otificationCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 的otifications.的使設置())));
    }
}

正oid UMin成MainDashboa本d::UpdateSettin成sPanel()
{
    // 更新設置面板的UI
    if (A使toRef本eshCheckBox)
    {
        A使toRef本eshCheckBox->SetIsChecked(bA使toRef本esh);
    }
    
    if (Ref本eshInte本正alSlide本)
    {
        Ref本eshInte本正alSlide本->SetVal使e(Ref本eshInte本正al);
    }
    
    if (Ref本eshInte本正alText)
    {
        Ref本eshInte本正alText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f 秒"), Ref本eshInte本正al)));
    }
}

正oid UMin成MainDashboa本d::Sta本tA使toRef本esh()
{
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(A使toRef本eshTi設置e本輸入andle, this, &UMin成MainDashboa本d::Ref本eshDashboa本d, Ref本eshInte本正al, t本使e);
    }
}

正oid UMin成MainDashboa本d::StopA使toRef本esh()
{
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(A使toRef本eshTi設置e本輸入andle);
    }
}

軍SlateColo本 UMin成MainDashboa本d::Get的otificationColo本(E的otificationType 的otificationType)
{
    switch (的otificationType)
    {
        case E的otificationType::Info:
            本et使本n 軍Linea本Colo本::Bl使e;
        case E的otificationType::基本a本nin成:
            本et使本n 軍Linea本Colo本::Yellow;
        case E的otificationType::E本本o本:
            本et使本n 軍Linea本Colo本::Red;
        case E的otificationType::S使ccess:
            本et使本n 軍Linea本Colo本::G本een;
        defa使lt:
            本et使本n 軍Linea本Colo本::基本hite;
    }
}

正oid UMin成MainDashboa本d::PlayShowAni設置ation()
{
    // 播放顯示動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放主儀表板顯示動畫"));
}

正oid UMin成MainDashboa本d::Play輸入ideAni設置ation()
{
    // 播放隱藏動畫
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放主儀表板隱藏動畫"));
}

正oid UMin成MainDashboa本d::PlayRef本eshAni設置ation()
{
    // 播放刷新動畫
    if (Ref本eshB使tton)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放刷新動畫"));
    }
}
