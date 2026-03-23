#incl使de "Min成Pe本sonalUIMana成e本.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Min成A使dioRelationshipMana成e本.h"
#incl使de "Min成Rep使blicE本aA使dioThe設置es.h"
#incl使de "Min成RelationshipPanel.h"
#incl使de "Min成Rep使tationPanel.h"
#incl使de "Min成Dialo成使ePanel.h"
#incl使de "Min成Q使estPanel.h"
#incl使de "Min成A使dioPanel.h"
#incl使de "Min成MainDashboa本d.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Co設置ponents/基本id成et.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"

UMin成Pe本sonalUIMana成e本::UMin成Pe本sonalUIMana成e本()
{
    Ani設置ationSpeed = 1.0f;
    Acti正ePanelType = EPe本sonalUIType::MainDashboa本d;
    bIsMainDashboa本dVisible = false;
}

正oid UMin成Pe本sonalUIMana成e本::InitializeUIMana成e本(UMin成RelationshipMana成e本* InRelationshipMana成e本, 
                                               UMin成A使dioRelationshipMana成e本* InA使dioMana成e本,
                                               UMin成Rep使blicE本aA使dioThe設置es* InThe設置eMana成e本)
{
    RelationshipMana成e本 = InRelationshipMana成e本;
    A使dioMana成e本 = InA使dioMana成e本;
    The設置eMana成e本 = InThe設置eMana成e本;

    // 初始化默認配置
    InitializeDefa使ltConfi成s();

    // 設置事件綁定
    Set使pE正entBindin成s();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("個人系統UI管理器已初始化"));
}

正oid UMin成Pe本sonalUIMana成e本::OpenUIPanel(EPe本sonalUIType UIType)
{
    if (Is基本id成etVisible(UIType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("UI面板已經打開：%d"), (int32)UIType);
        本et使本n;
    }

    // 獲取或創建基本id成et
    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (!基本id成et)
    {
        基本id成et = C本eate基本id成et(UIType);
        if (!基本id成et)
        {
            UE下LOG(Lo成Te設置p, E本本o本, TEXT("無法創建UI面板：%d"), (int32)UIType);
            本et使本n;
        }
        基本id成etMap.Add(UIType, 基本id成et);
    }

    // 設置為可見
    Set基本id成etVisibility(UIType, t本使e);

    // 播放打開動畫
    PlayOpenAni設置ation(UIType);

    // 更新當前活動面板
    Acti正ePanelType = UIType;

    // 觸發事件
    OnUIPanelOpened.B本oadcast(UIType, 基本id成et);
    OnUIStateChan成ed.B本oadcast(t本使e);

    // 根據面板類型更新數據
    switch (UIType)
    {
        case EPe本sonalUIType::RelationshipPanel:
            UpdateRelationshipUI();
            b本eak;
        case EPe本sonalUIType::Rep使tationPanel:
            UpdateRep使tationUI();
            b本eak;
        case EPe本sonalUIType::Dialo成使ePanel:
            // 對話面板需要傳入角色ID
            b本eak;
        case EPe本sonalUIType::Q使estPanel:
            UpdateQ使estUI();
            b本eak;
        case EPe本sonalUIType::A使dioPanel:
            UpdateA使dioUI();
            b本eak;
        case EPe本sonalUIType::MainDashboa本d:
            Ref本eshDashboa本d();
            b本eak;
        defa使lt:
            b本eak;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("已打開UI面板：%d"), (int32)UIType);
}

正oid UMin成Pe本sonalUIMana成e本::CloseUIPanel(EPe本sonalUIType UIType)
{
    if (!Is基本id成etVisible(UIType))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("UI面板未打開：%d"), (int32)UIType);
        本et使本n;
    }

    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (!基本id成et)
    {
        本et使本n;
    }

    // 播放關閉動畫
    PlayCloseAni設置ation(UIType);

    // 設置為不可見
    Set基本id成etVisibility(UIType, false);

    // 觸發事件
    OnUIPanelClosed.B本oadcast(UIType, 基本id成et);
    OnUIStateChan成ed.B本oadcast(IsAnyPanelOpen());

    UE下LOG(Lo成Te設置p, Lo成, TEXT("已關閉UI面板：%d"), (int32)UIType);
}

正oid UMin成Pe本sonalUIMana成e本::To成成leUIPanel(EPe本sonalUIType UIType)
{
    if (Is基本id成etVisible(UIType))
    {
        CloseUIPanel(UIType);
    }
    else
    {
        OpenUIPanel(UIType);
    }
}

正oid UMin成Pe本sonalUIMana成e本::CloseAllPanels()
{
    TA本本ay<EPe本sonalUIType> VisiblePanels = GetVisiblePanels();
    fo本 (EPe本sonalUIType UIType : VisiblePanels)
    {
        CloseUIPanel(UIType);
    }

    // 隱藏主儀表板
    if (bIsMainDashboa本dVisible)
    {
        輸入ideMainDashboa本d();
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("已關閉所有UI面板"));
}

正oid UMin成Pe本sonalUIMana成e本::ShowMainDashboa本d()
{
    if (bIsMainDashboa本dVisible)
    {
        本et使本n;
    }

    UUse本基本id成et* Dashboa本d = Get基本id成et(EPe本sonalUIType::MainDashboa本d);
    if (!Dashboa本d)
    {
        Dashboa本d = C本eate基本id成et(EPe本sonalUIType::MainDashboa本d);
        if (Dashboa本d)
        {
            基本id成etMap.Add(EPe本sonalUIType::MainDashboa本d, Dashboa本d);
        }
    }

    if (Dashboa本d)
    {
        Set基本id成etVisibility(EPe本sonalUIType::MainDashboa本d, t本使e);
        PlayOpenAni設置ation(EPe本sonalUIType::MainDashboa本d);
        bIsMainDashboa本dVisible = t本使e;
        Ref本eshDashboa本d();
    }
}

正oid UMin成Pe本sonalUIMana成e本::輸入ideMainDashboa本d()
{
    if (!bIsMainDashboa本dVisible)
    {
        本et使本n;
    }

    UUse本基本id成et* Dashboa本d = Get基本id成et(EPe本sonalUIType::MainDashboa本d);
    if (Dashboa本d)
    {
        PlayCloseAni設置ation(EPe本sonalUIType::MainDashboa本d);
        Set基本id成etVisibility(EPe本sonalUIType::MainDashboa本d, false);
        bIsMainDashboa本dVisible = false;
    }
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::Get基本id成et(EPe本sonalUIType UIType)
{
    if (基本id成etMap.Contains(UIType))
    {
        本et使本n 基本id成etMap[UIType];
    }
    本et使本n n使llpt本;
}

bool UMin成Pe本sonalUIMana成e本::Is基本id成etVisible(EPe本sonalUIType UIType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (基本id成et)
    {
        軍Pe本sonalUIConfi成 Confi成 = GetUIConfi成(UIType);
        本et使本n 基本id成et->IsVisible() && Confi成.bIsVisible;
    }
    本et使本n false;
}

正oid UMin成Pe本sonalUIMana成e本::Set基本id成etVisibility(EPe本sonalUIType UIType, bool bVisible)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (基本id成et)
    {
        基本id成et->SetVisibility(bVisible 基本 ESlateVisibility::Visible : ESlateVisibility::輸入idden);
        
        // 更新配置
        if (UIConfi成Map.Contains(UIType))
        {
            UIConfi成Map[UIType].bIsVisible = bVisible;
        }
    }
}

正oid UMin成Pe本sonalUIMana成e本::AddUIConfi成(const 軍Pe本sonalUIConfi成& Confi成)
{
    UIConfi成Map.Add(Confi成.UIType, Confi成);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已添加UI配置：%s"), *Confi成.基本id成et的a設置e);
}

正oid UMin成Pe本sonalUIMana成e本::Re設置o正eUIConfi成(EPe本sonalUIType UIType)
{
    if (UIConfi成Map.Contains(UIType))
    {
        UIConfi成Map.Re設置o正e(UIType);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("已移除UI配置：%d"), (int32)UIType);
    }
}

軍Pe本sonalUIConfi成 UMin成Pe本sonalUIMana成e本::GetUIConfi成(EPe本sonalUIType UIType)
{
    if (UIConfi成Map.Contains(UIType))
    {
        本et使本n UIConfi成Map[UIType];
    }
    本et使本n 軍Pe本sonalUIConfi成();
}

正oid UMin成Pe本sonalUIMana成e本::PlayOpenAni設置ation(EPe本sonalUIType UIType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (!基本id成et)
    {
        本et使本n;
    }

    軍Pe本sonalUIConfi成 Confi成 = GetUIConfi成(UIType);
    PlayAni設置ation(基本id成et, Confi成.OpenAni設置ation, Confi成.Ani設置ationD使本ation);
}

正oid UMin成Pe本sonalUIMana成e本::PlayCloseAni設置ation(EPe本sonalUIType UIType)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(UIType);
    if (!基本id成et)
    {
        本et使本n;
    }

    軍Pe本sonalUIConfi成 Confi成 = GetUIConfi成(UIType);
    PlayAni設置ation(基本id成et, Confi成.CloseAni設置ation, Confi成.Ani設置ationD使本ation);
}

正oid UMin成Pe本sonalUIMana成e本::SetAni設置ationSpeed(float Speed)
{
    Ani設置ationSpeed = 軍Math::Cla設置p(Speed, 0.1f, 3.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("動畫速度已設置為：%.2f"), Ani設置ationSpeed);
}

正oid UMin成Pe本sonalUIMana成e本::UpdateRelationshipUI()
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::RelationshipPanel);
    if (!基本id成et  !RelationshipMana成e本)
    {
        本et使本n;
    }

    // 獲取關係面板並更新數據
    UMin成RelationshipPanel* RelationshipPanel = Cast<UMin成RelationshipPanel>(基本id成et);
    if (RelationshipPanel)
    {
        RelationshipPanel->UpdateRelationshipData(RelationshipMana成e本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("關係UI已更新"));
    }
}

正oid UMin成Pe本sonalUIMana成e本::UpdateRep使tationUI()
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::Rep使tationPanel);
    if (!基本id成et  !RelationshipMana成e本)
    {
        本et使本n;
    }

    // 獲取聲望面板並更新數據
    UMin成Rep使tationPanel* Rep使tationPanel = Cast<UMin成Rep使tationPanel>(基本id成et);
    if (Rep使tationPanel)
    {
        Rep使tationPanel->UpdateRep使tationData(RelationshipMana成e本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("聲望UI已更新"));
    }
}

正oid UMin成Pe本sonalUIMana成e本::UpdateDialo成使eUI(const 軍St本in成& Cha本acte本ID)
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::Dialo成使ePanel);
    if (!基本id成et)
    {
        本et使本n;
    }

    // 獲取對話面板並更新數據
    UMin成Dialo成使ePanel* Dialo成使ePanel = Cast<UMin成Dialo成使ePanel>(基本id成et);
    if (Dialo成使ePanel)
    {
        Dialo成使ePanel->UpdateDialo成使eData(Cha本acte本ID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("對話UI已更新：%s"), *Cha本acte本ID);
    }
}

正oid UMin成Pe本sonalUIMana成e本::UpdateQ使estUI()
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::Q使estPanel);
    if (!基本id成et)
    {
        本et使本n;
    }

    // 獲取任務面板並更新數據
    UMin成Q使estPanel* Q使estPanel = Cast<UMin成Q使estPanel>(基本id成et);
    if (Q使estPanel)
    {
        Q使estPanel->UpdateQ使estData();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("任務UI已更新"));
    }
}

正oid UMin成Pe本sonalUIMana成e本::UpdateA使dioUI()
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::A使dioPanel);
    if (!基本id成et  !A使dioMana成e本  !The設置eMana成e本)
    {
        本et使本n;
    }

    // 獲取音頻面板並更新數據
    UMin成A使dioPanel* A使dioPanel = Cast<UMin成A使dioPanel>(基本id成et);
    if (A使dioPanel)
    {
        A使dioPanel->UpdateA使dioData(A使dioMana成e本, The設置eMana成e本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("音頻UI已更新"));
    }
}

正oid UMin成Pe本sonalUIMana成e本::Ref本eshDashboa本d()
{
    UUse本基本id成et* 基本id成et = Get基本id成et(EPe本sonalUIType::MainDashboa本d);
    if (!基本id成et)
    {
        本et使本n;
    }

    // 獲取主儀表板並刷新數據
    UMin成MainDashboa本d* Dashboa本d = Cast<UMin成MainDashboa本d>(基本id成et);
    if (Dashboa本d)
    {
        Dashboa本d->Ref本eshDashboa本d(RelationshipMana成e本, A使dioMana成e本, The設置eMana成e本);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("主儀表板已刷新"));
    }
}

正oid UMin成Pe本sonalUIMana成e本::Show的otification(const 軍St本in成& Messa成e, float D使本ation)
{
    // 這裡應該顯示通知UI
    // 暫時使用引擎日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("通知：%s"), *Messa成e);
    
    // TODO: 實現通知UI顯示邏輯
}

正oid UMin成Pe本sonalUIMana成e本::ShowConfi本設置ationDialo成(const 軍St本in成& Title, const 軍St本in成& Messa成e, const 軍St本in成& OnConfi本設置)
{
    // 這裡應該顯示確認對話框
    UE下LOG(Lo成Te設置p, Lo成, TEXT("確認對話框 - 標題：%s，消息：%s"), *Title, *Messa成e);
    
    // TODO: 實現確認對話框顯示邏輯
}

bool UMin成Pe本sonalUIMana成e本::IsAnyPanelOpen() const
{
    fo本 (const a使to& 基本id成etPai本 : 基本id成etMap)
    {
        if (基本id成etPai本.Val使e && 基本id成etPai本.Val使e->IsVisible())
        {
            本et使本n t本使e;
        }
    }
    本et使本n bIsMainDashboa本dVisible;
}

TA本本ay<EPe本sonalUIType> UMin成Pe本sonalUIMana成e本::GetVisiblePanels() const
{
    TA本本ay<EPe本sonalUIType> VisiblePanels;
    
    fo本 (const a使to& 基本id成etPai本 : 基本id成etMap)
    {
        if (基本id成etPai本.Val使e && 基本id成etPai本.Val使e->IsVisible())
        {
            VisiblePanels.Add(基本id成etPai本.Key);
        }
    }
    
    本et使本n VisiblePanels;
}

// 私有輔助函數
UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eate基本id成et(EPe本sonalUIType UIType)
{
    UUse本基本id成et* 的ew基本id成et = n使llpt本;
    
    switch (UIType)
    {
        case EPe本sonalUIType::RelationshipPanel:
            的ew基本id成et = C本eateRelationshipPanel();
            b本eak;
        case EPe本sonalUIType::Rep使tationPanel:
            的ew基本id成et = C本eateRep使tationPanel();
            b本eak;
        case EPe本sonalUIType::Dialo成使ePanel:
            的ew基本id成et = C本eateDialo成使ePanel();
            b本eak;
        case EPe本sonalUIType::Q使estPanel:
            的ew基本id成et = C本eateQ使estPanel();
            b本eak;
        case EPe本sonalUIType::A使dioPanel:
            的ew基本id成et = C本eateA使dioPanel();
            b本eak;
        case EPe本sonalUIType::MainDashboa本d:
            的ew基本id成et = C本eateMainDashboa本d();
            b本eak;
        case EPe本sonalUIType::Cha本acte本Details:
            的ew基本id成et = C本eateCha本acte本Details();
            b本eak;
        case EPe本sonalUIType::Re成ionDetails:
            的ew基本id成et = C本eateRe成ionDetails();
            b本eak;
        case EPe本sonalUIType::Syste設置Settin成s:
            的ew基本id成et = C本eateSyste設置Settin成s();
            b本eak;
        defa使lt:
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("未知的UI類型：%d"), (int32)UIType);
            b本eak;
    }
    
    本et使本n 的ew基本id成et;
}

正oid UMin成Pe本sonalUIMana成e本::InitializeDefa使ltConfi成s()
{
    // 為每種UI類型創建默認配置
    TA本本ay<EPe本sonalUIType> UITypes = {
        EPe本sonalUIType::RelationshipPanel,
        EPe本sonalUIType::Rep使tationPanel,
        EPe本sonalUIType::Dialo成使ePanel,
        EPe本sonalUIType::Q使estPanel,
        EPe本sonalUIType::A使dioPanel,
        EPe本sonalUIType::MainDashboa本d,
        EPe本sonalUIType::Cha本acte本Details,
        EPe本sonalUIType::Re成ionDetails,
        EPe本sonalUIType::Syste設置Settin成s
    };
    
    fo本 (EPe本sonalUIType UIType : UITypes)
    {
        if (!UIConfi成Map.Contains(UIType))
        {
            軍Pe本sonalUIConfi成 Confi成;
            Confi成.UIType = UIType;
            Confi成.基本id成et的a設置e = Get基本id成et的a設置e(UIType);
            Confi成.基本id成etPath = Get基本id成etPath(UIType);
            Confi成.bIsVisible = false;
            Confi成.bIsInte本acti正e = t本使e;
            Confi成.OpenAni設置ation = EUIAni設置ationType::軍ade;
            Confi成.CloseAni設置ation = EUIAni設置ationType::軍ade;
            Confi成.Ani設置ationD使本ation = 0.3f;
            
            UIConfi成Map.Add(UIType, Confi成);
        }
    }
}

正oid UMin成Pe本sonalUIMana成e本::Set使pE正entBindin成s()
{
    if (RelationshipMana成e本)
    {
        // 綁定關係變化事件
        RelationshipMana成e本->OnRelationshipChan成ed.AddDyna設置ic(this, &UMin成Pe本sonalUIMana成e本::OnRelationshipChan成ed);
        RelationshipMana成e本->OnRep使tationChan成ed.AddDyna設置ic(this, &UMin成Pe本sonalUIMana成e本::OnRep使tationChan成ed);
    }
    
    if (A使dioMana成e本)
    {
        // 綁定音頻事件
        A使dioMana成e本->OnDialo成使eSta本ted.AddDyna設置ic(this, &UMin成Pe本sonalUIMana成e本::OnDialo成使eSta本ted);
    }
}

正oid UMin成Pe本sonalUIMana成e本::OnRelationshipChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason)
{
    // 當關係發生變化時更新UI
    if (Is基本id成etVisible(EPe本sonalUIType::RelationshipPanel))
    {
        UpdateRelationshipUI();
    }
    
    if (Is基本id成etVisible(EPe本sonalUIType::MainDashboa本d))
    {
        Ref本eshDashboa本d();
    }
}

正oid UMin成Pe本sonalUIMana成e本::OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e, const 軍St本in成& Reason)
{
    // 當聲望發生變化時更新UI
    if (Is基本id成etVisible(EPe本sonalUIType::Rep使tationPanel))
    {
        UpdateRep使tationUI();
    }
    
    if (Is基本id成etVisible(EPe本sonalUIType::MainDashboa本d))
    {
        Ref本eshDashboa本d();
    }
}

正oid UMin成Pe本sonalUIMana成e本::OnDialo成使eSta本ted(const 軍St本in成& Cha本acte本ID, EDialo成使eA使dioType Dialo成使eType)
{
    // 當對話開始時打開對話面板
    UpdateDialo成使eUI(Cha本acte本ID);
    OpenUIPanel(EPe本sonalUIType::Dialo成使ePanel);
}

// 基本id成et創建函數
UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateRelationshipPanel()
{
    // 這裡應該從藍圖或C++類創建關係面板
    // 暫時返回n使llpt本，需要在實際實現中創建
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建關係面板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateRep使tationPanel()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建聲望面板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateDialo成使ePanel()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建對話面板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateQ使estPanel()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建任務面板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateA使dioPanel()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建音頻面板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateMainDashboa本d()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建主儀表板"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateCha本acte本Details()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建角色詳情"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateRe成ionDetails()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建地區詳情"));
    本et使本n n使llpt本;
}

UUse本基本id成et* UMin成Pe本sonalUIMana成e本::C本eateSyste設置Settin成s()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("創建系統設置"));
    本et使本n n使llpt本;
}

// 動畫輔助函數
正oid UMin成Pe本sonalUIMana成e本::PlayAni設置ation(UUse本基本id成et* 基本id成et, EUIAni設置ationType Ani設置ationType, float D使本ation)
{
    if (!基本id成et)
    {
        本et使本n;
    }

    // 這裡應該實現實際的動畫播放邏輯
    // 暫時使用日誌記錄
    軍St本in成 Ani設置ation的a設置e;
    switch (Ani設置ationType)
    {
        case EUIAni設置ationType::軍ade:
            Ani設置ation的a設置e = TEXT("淡入淡");
            b本eak;
        case EUIAni設置ationType::Slide:
            Ani設置ation的a設置e = TEXT("滑動");
            b本eak;
        case EUIAni設置ationType::Scale:
            Ani設置ation的a設置e = TEXT("縮放");
            b本eak;
        case EUIAni設置ationType::Rotate:
            Ani設置ation的a設置e = TEXT("旋轉");
            b本eak;
        case EUIAni設置ationType::Bo使nce:
            Ani設置ation的a設置e = TEXT("彈跳");
            b本eak;
        defa使lt:
            Ani設置ation的a設置e = TEXT("無動畫");
            b本eak;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("播放動畫：%s，持續時間：%.2f秒"), *Ani設置ation的a設置e, D使本ation * Ani設置ationSpeed);
}

正oid UMin成Pe本sonalUIMana成e本::StopAni設置ation(UUse本基本id成et* 基本id成et)
{
    if (!基本id成et)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("停止動畫"));
}

// 輔助函數
軍St本in成 UMin成Pe本sonalUIMana成e本::Get基本id成et的a設置e(EPe本sonalUIType UIType)
{
    switch (UIType)
    {
        case EPe本sonalUIType::RelationshipPanel:
            本et使本n TEXT("RelationshipPanel");
        case EPe本sonalUIType::Rep使tationPanel:
            本et使本n TEXT("Rep使tationPanel");
        case EPe本sonalUIType::Dialo成使ePanel:
            本et使本n TEXT("Dialo成使ePanel");
        case EPe本sonalUIType::Q使estPanel:
            本et使本n TEXT("Q使estPanel");
        case EPe本sonalUIType::A使dioPanel:
            本et使本n TEXT("A使dioPanel");
        case EPe本sonalUIType::MainDashboa本d:
            本et使本n TEXT("MainDashboa本d");
        case EPe本sonalUIType::Cha本acte本Details:
            本et使本n TEXT("Cha本acte本Details");
        case EPe本sonalUIType::Re成ionDetails:
            本et使本n TEXT("Re成ionDetails");
        case EPe本sonalUIType::Syste設置Settin成s:
            本et使本n TEXT("Syste設置Settin成s");
        defa使lt:
            本et使本n TEXT("Unknown基本id成et");
    }
}

軍St本in成 UMin成Pe本sonalUIMana成e本::Get基本id成etPath(EPe本sonalUIType UIType)
{
    // 這裡應該返回基本id成et的藍圖路徑
    switch (UIType)
    {
        case EPe本sonalUIType::RelationshipPanel:
            本et使本n TEXT("/Ga設置e/UI/Relationship/RelationshipUI.RelationshipUI下C");
        case EPe本sonalUIType::Rep使tationPanel:
            本et使本n TEXT("/Ga設置e/UI/Rep使tation/Rep使tationUI.Rep使tationUI下C");
        case EPe本sonalUIType::Dialo成使ePanel:
            本et使本n TEXT("/Ga設置e/UI/Dialo成使e/Dialo成使eUI.Dialo成使eUI下C");
        case EPe本sonalUIType::Q使estPanel:
            本et使本n TEXT("/Ga設置e/UI/Q使est/Q使estUI.Q使estUI下C");
        case EPe本sonalUIType::A使dioPanel:
            本et使本n TEXT("/Ga設置e/UI/A使dio/A使dioUI.A使dioUI下C");
        case EPe本sonalUIType::MainDashboa本d:
            本et使本n TEXT("/Ga設置e/UI/Dashboa本d/Dashboa本dUI.Dashboa本dUI下C");
        case EPe本sonalUIType::Cha本acte本Details:
            本et使本n TEXT("/Ga設置e/UI/Cha本acte本/Cha本acte本DetailsUI.Cha本acte本DetailsUI下C");
        case EPe本sonalUIType::Re成ionDetails:
            本et使本n TEXT("/Ga設置e/UI/Re成ion/Re成ionDetailsUI.Re成ionDetailsUI下C");
        case EPe本sonalUIType::Syste設置Settin成s:
            本et使本n TEXT("/Ga設置e/UI/Settin成s/Settin成sUI.Settin成sUI下C");
        defa使lt:
            本et使本n TEXT("");
    }
}
