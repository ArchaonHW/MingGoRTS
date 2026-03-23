#incl使de "Min成Q使estPanel.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "En成ine/En成ine.h"

UMin成Q使estPanel::UMin成Q使estPanel()
{
    bIsUpdatin成 = false;
    C使本本ent軍ilte本 = EQ使est軍ilte本::All;
    C使本本entSo本tType = EQ使estSo本tType::By的a設置e;
    SelectedQ使estID = TEXT("");
    bShowCo設置pleted = false;
}

正oid UMin成Q使estPanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::On軍ilte本AllClicked);
    }
    
    if (軍ilte本Acti正eB使tton)
    {
        軍ilte本Acti正eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::On軍ilte本Acti正eClicked);
    }
    
    if (軍ilte本Co設置pletedB使tton)
    {
        軍ilte本Co設置pletedB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::On軍ilte本Co設置pletedClicked);
    }
    
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnSo本tBy的a設置eClicked);
    }
    
    if (So本tByP本io本ityB使tton)
    {
        So本tByP本io本ityB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnSo本tByP本io本ityClicked);
    }
    
    if (So本tByP本o成本essB使tton)
    {
        So本tByP本o成本essB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnSo本tByP本o成本essClicked);
    }
    
    if (Ref本eshB使tton)
    {
        Ref本eshB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnRef本eshClicked);
    }
    
    if (AcceptQ使estB使tton)
    {
        AcceptQ使estB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnAcceptQ使estClicked);
    }
    
    if (AbandonQ使estB使tton)
    {
        AbandonQ使estB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnAbandonQ使estClicked);
    }
    
    if (T本ackQ使estB使tton)
    {
        T本ackQ使estB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使estPanel::OnT本ackQ使estClicked);
    }
    
    // 設置默認過濾器和排序
    Update軍ilte本B使ttons();
    UpdateSo本tB使ttons();
}

正oid UMin成Q使estPanel::UpdateQ使estData(UMin成RelationshipMana成e本* InRelationshipMana成e本)
{
    if (bIsUpdatin成  !InRelationshipMana成e本)
    {
        本et使本n;
    }
    
    bIsUpdatin成 = t本使e;
    RelationshipMana成e本 = InRelationshipMana成e本;
    
    // 獲取所有任務數據
    TA本本ay<軍Q使estData> AllQ使ests = RelationshipMana成e本->GetAllQ使ests();
    
    // 應用過濾器
    TA本本ay<軍Q使estData> 軍ilte本edQ使ests = Apply軍ilte本(AllQ使ests, C使本本ent軍ilte本);
    
    // 應用排序
    TA本本ay<軍Q使estData> So本tedQ使ests = ApplySo本t(軍ilte本edQ使ests, C使本本entSo本tType);
    
    // 更新UI顯示
    UpdateQ使estList(So本tedQ使ests);
    
    // 更新統計信息
    UpdateStatistics(AllQ使ests);
    
    bIsUpdatin成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("任務面板數據已更新，顯示 %d 個任務"), So本tedQ使ests.的使設置());
}

正oid UMin成Q使estPanel::SetQ使est軍ilte本(EQ使est軍ilte本 軍ilte本)
{
    if (C使本本ent軍ilte本 != 軍ilte本)
    {
        C使本本ent軍ilte本 = 軍ilte本;
        Update軍ilte本B使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateQ使estData(RelationshipMana成e本);
        }
    }
}

正oid UMin成Q使estPanel::SetQ使estSo本t(EQ使estSo本tType So本tType)
{
    if (C使本本entSo本tType != So本tType)
    {
        C使本本entSo本tType = So本tType;
        UpdateSo本tB使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateQ使estData(RelationshipMana成e本);
        }
    }
}

正oid UMin成Q使estPanel::SelectQ使est(const 軍St本in成& Q使estID)
{
    if (SelectedQ使estID == Q使estID)
    {
        本et使本n;
    }
    
    SelectedQ使estID = Q使estID;
    
    // 更新詳細信息面板
    UpdateQ使estDetails(Q使estID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已選中任務：%s"), *Q使estID);
}

正oid UMin成Q使estPanel::ShowQ使estDetails(const 軍St本in成& Q使estID)
{
    SelectQ使est(Q使estID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAni設置ation(t本使e);
    }
}

正oid UMin成Q使estPanel::輸入ideQ使estDetails()
{
    SelectedQ使estID = TEXT("");
    
    // 隱藏詳細信息面板
    if (DetailsPanel)
    {
        PlayDetailsAni設置ation(false);
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]()
        {
            if (DetailsPanel)
            {
                DetailsPanel->SetVisibility(ESlateVisibility::輸入idden);
            }
        }, 0.3f, false);
    }
}

正oid UMin成Q使estPanel::Ref本eshQ使ests()
{
    if (RelationshipMana成e本)
    {
        UpdateQ使estData(RelationshipMana成e本);
        PlayRef本eshAni設置ation();
    }
}

正oid UMin成Q使estPanel::AcceptQ使est(const 軍St本in成& Q使estID)
{
    if (!RelationshipMana成e本  Q使estID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取任務數據
    軍Q使estData Q使estData = RelationshipMana成e本->GetQ使est(Q使estID);
    
    // 檢查是否可以接受
    if (!CanAcceptQ使est(Q使estData))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法接受任務：%s - 條件不滿足"), *Q使estID);
        本et使本n;
    }
    
    // 接受任務
    bool bS使ccess = RelationshipMana成e本->AcceptQ使est(Q使estID);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("成功接受任務：%s"), *Q使estID);
        
        // 更新UI
        UpdateQ使estData(RelationshipMana成e本);
        
        // 觸發事件
        OnQ使estAccepted.B本oadcast(Q使estID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("接受任務失敗：%s"), *Q使estID);
    }
}

正oid UMin成Q使estPanel::AbandonQ使est(const 軍St本in成& Q使estID)
{
    if (!RelationshipMana成e本  Q使estID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取任務數據
    軍Q使estData Q使estData = RelationshipMana成e本->GetQ使est(Q使estID);
    
    // 檢查是否可以放棄
    if (!CanAbandonQ使est(Q使estData))
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("無法放棄任務：%s - 不允許放棄"), *Q使estID);
        本et使本n;
    }
    
    // 放棄任務
    bool bS使ccess = RelationshipMana成e本->AbandonQ使est(Q使estID);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("成功放棄任務：%s"), *Q使estID);
        
        // 更新UI
        UpdateQ使estData(RelationshipMana成e本);
        
        // 觸發事件
        OnQ使estAbandoned.B本oadcast(Q使estID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("放棄任務失敗：%s"), *Q使estID);
    }
}

正oid UMin成Q使estPanel::T本ackQ使est(const 軍St本in成& Q使estID)
{
    if (!RelationshipMana成e本  Q使estID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 設置追蹤任務
    bool bS使ccess = RelationshipMana成e本->T本ackQ使est(Q使estID);
    
    if (bS使ccess)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("開始追蹤任務：%s"), *Q使estID);
        
        // 更新UI
        UpdateQ使estData(RelationshipMana成e本);
        
        // 觸發事件
        OnQ使estT本acked.B本oadcast(Q使estID);
    }
    else
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("追蹤任務失敗：%s"), *Q使estID);
    }
}

正oid UMin成Q使estPanel::ShowCo設置pletedQ使ests(bool bShow)
{
    if (bShowCo設置pleted != bShow)
    {
        bShowCo設置pleted = bShow;
        
        if (RelationshipMana成e本)
        {
            UpdateQ使estData(RelationshipMana成e本);
        }
    }
}

// 按鈕事件處理
正oid UMin成Q使estPanel::On軍ilte本AllClicked()
{
    SetQ使est軍ilte本(EQ使est軍ilte本::All);
}

正oid UMin成Q使estPanel::On軍ilte本Acti正eClicked()
{
    SetQ使est軍ilte本(EQ使est軍ilte本::Acti正e);
}

正oid UMin成Q使estPanel::On軍ilte本Co設置pletedClicked()
{
    SetQ使est軍ilte本(EQ使est軍ilte本::Co設置pleted);
}

正oid UMin成Q使estPanel::OnSo本tBy的a設置eClicked()
{
    SetQ使estSo本t(EQ使estSo本tType::By的a設置e);
}

正oid UMin成Q使estPanel::OnSo本tByP本io本ityClicked()
{
    SetQ使estSo本t(EQ使estSo本tType::ByP本io本ity);
}

正oid UMin成Q使estPanel::OnSo本tByP本o成本essClicked()
{
    SetQ使estSo本t(EQ使estSo本tType::ByP本o成本ess);
}

正oid UMin成Q使estPanel::OnRef本eshClicked()
{
    Ref本eshQ使ests();
}

正oid UMin成Q使estPanel::OnAcceptQ使estClicked()
{
    if (!SelectedQ使estID.IsE設置pty())
    {
        AcceptQ使est(SelectedQ使estID);
    }
}

正oid UMin成Q使estPanel::OnAbandonQ使estClicked()
{
    if (!SelectedQ使estID.IsE設置pty())
    {
        AbandonQ使est(SelectedQ使estID);
    }
}

正oid UMin成Q使estPanel::OnT本ackQ使estClicked()
{
    if (!SelectedQ使estID.IsE設置pty())
    {
        T本ackQ使est(SelectedQ使estID);
    }
}

正oid UMin成Q使estPanel::OnQ使estClicked(const 軍St本in成& Q使estID)
{
    ShowQ使estDetails(Q使estID);
}

正oid UMin成Q使estPanel::OnQ使estP本o成本essUpdated(const 軍St本in成& Q使estID, float OldP本o成本ess, float 的ewP本o成本ess)
{
    // 如果當前顯示的任務數據發生變化，更新UI
    if (RelationshipMana成e本 && !bIsUpdatin成)
    {
        UpdateQ使estData(RelationshipMana成e本);
    }
}

// 私有輔助函數
TA本本ay<軍Q使estData> UMin成Q使estPanel::Apply軍ilte本(const TA本本ay<軍Q使estData>& Q使ests, EQ使est軍ilte本 軍ilte本)
{
    TA本本ay<軍Q使estData> 軍ilte本edQ使ests;
    
    fo本 (const 軍Q使estData& Q使est : Q使ests)
    {
        bool bSho使ldIncl使de = false;
        
        switch (軍ilte本)
        {
            case EQ使est軍ilte本::All:
                bSho使ldIncl使de = t本使e;
                b本eak;
                
            case EQ使est軍ilte本::Acti正e:
                bSho使ldIncl使de = Q使est.bIsActi正e && !Q使est.bIsCo設置pleted;
                b本eak;
                
            case EQ使est軍ilte本::Co設置pleted:
                bSho使ldIncl使de = Q使est.bIsCo設置pleted;
                b本eak;
        }
        
        // 檢查是否顯示已完成任務
        if (bSho使ldIncl使de && Q使est.bIsCo設置pleted && !bShowCo設置pleted)
        {
            bSho使ldIncl使de = false;
        }
        
        if (bSho使ldIncl使de)
        {
            軍ilte本edQ使ests.Add(Q使est);
        }
    }
    
    本et使本n 軍ilte本edQ使ests;
}

TA本本ay<軍Q使estData> UMin成Q使estPanel::ApplySo本t(const TA本本ay<軍Q使estData>& Q使ests, EQ使estSo本tType So本tType)
{
    TA本本ay<軍Q使estData> So本tedQ使ests = Q使ests;
    
    switch (So本tType)
    {
        case EQ使estSo本tType::By的a設置e:
            So本tedQ使ests.So本t([](const 軍Q使estData& A, const 軍Q使estData& B)
            {
                本et使本n A.Q使est的a設置e < B.Q使est的a設置e;
            });
            b本eak;
            
        case EQ使estSo本tType::ByP本io本ity:
            So本tedQ使ests.So本t([](const 軍Q使estData& A, const 軍Q使estData& B)
            {
                本et使本n static下cast<int32>(A.P本io本ity) > static下cast<int32>(B.P本io本ity);
            });
            b本eak;
            
        case EQ使estSo本tType::ByP本o成本ess:
            So本tedQ使ests.So本t([](const 軍Q使estData& A, const 軍Q使estData& B)
            {
                本et使本n A.P本o成本ess > B.P本o成本ess;
            });
            b本eak;
    }
    
    本et使本n So本tedQ使ests;
}

正oid UMin成Q使estPanel::UpdateQ使estList(const TA本本ay<軍Q使estData>& Q使ests)
{
    if (!Q使estListSc本ollBox)
    {
        本et使本n;
    }
    
    // 清空現有列表
    Q使estListSc本ollBox->Clea本Child本en();
    
    // 創建任務項目
    fo本 (const 軍Q使estData& Q使est : Q使ests)
    {
        U基本id成et* Q使estIte設置 = C本eateQ使estIte設置(Q使est);
        if (Q使estIte設置)
        {
            Q使estListSc本ollBox->AddChild(Q使estIte設置);
        }
    }
    
    // 更新計數
    if (Q使estCo使ntText)
    {
        Q使estCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("共 %d 個任務"), Q使ests.的使設置())));
    }
}

U基本id成et* UMin成Q使estPanel::C本eateQ使estIte設置(const 軍Q使estData& Q使est)
{
    // 這裡應該創建一個自定義的任務項目基本id成et
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = 的ewOb大ect<UTextBlock>(this);
    if (TextBlock)
    {
        軍St本in成 Ite設置Text = 軍St本in成::P本intf(TEXT("%s (%.0f%%)"), *Q使est.Q使est的a設置e, Q使est.P本o成本ess);
        TextBlock->SetText(軍Text::軍本o設置St本in成(Ite設置Text));
        
        // 設置字體和樣式
        軍Slate軍ontInfo 軍ontInfo = TextBlock->Get軍ont();
        軍ontInfo.Size = 14;
        TextBlock->Set軍ont(軍ontInfo);
        
        // 設置邊距
        TextBlock->SetMa本成in(軍Ma本成in(10.0f, 5.0f));
        
        // 根據優先級設置顏色
        軍SlateColo本 Colo本 = GetP本io本ityColo本(Q使est.P本io本ity);
        TextBlock->SetColo本AndOpacity(Colo本);
    }
    
    本et使本n TextBlock;
}

正oid UMin成Q使estPanel::UpdateQ使estDetails(const 軍St本in成& Q使estID)
{
    if (!RelationshipMana成e本  Q使estID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取任務詳細信息
    軍Q使estData Q使estData = RelationshipMana成e本->GetQ使est(Q使estID);
    
    // 更新詳細信息面板
    if (Q使est的a設置eText)
    {
        Q使est的a設置eText->SetText(軍Text::軍本o設置St本in成(Q使estData.Q使est的a設置e));
    }
    
    if (Q使estDesc本iptionText)
    {
        Q使estDesc本iptionText->SetText(軍Text::軍本o設置St本in成(Q使estData.Desc本iption));
    }
    
    if (Q使estTypeText)
    {
        Q使estTypeText->SetText(軍Text::軍本o設置St本in成(GetQ使estTypeText(Q使estData.Q使estType)));
    }
    
    if (Q使estP本io本ityText)
    {
        Q使estP本io本ityText->SetText(軍Text::軍本o設置St本in成(GetP本io本ityText(Q使estData.P本io本ity)));
    }
    
    if (Q使estP本o成本essBa本)
    {
        Q使estP本o成本essBa本->SetPe本cent(Q使estData.P本o成本ess / 100.0f);
    }
    
    if (Q使estP本o成本essText)
    {
        Q使estP本o成本essText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f%%"), Q使estData.P本o成本ess)));
    }
    
    if (Q使estRewa本dText)
    {
        軍St本in成 Rewa本dText;
        fo本 (const 軍Q使estRewa本d& Rewa本d : Q使estData.Rewa本ds)
        {
            Rewa本dText += 軍St本in成::P本intf(TEXT("%s x%d\n"), *Rewa本d.Ite設置的a設置e, Rewa本d.Q使antity);
        }
        Q使estRewa本dText->SetText(軍Text::軍本o設置St本in成(Rewa本dText));
    }
    
    if (Q使estOb大ecti正esText)
    {
        軍St本in成 Ob大ecti正esText;
        fo本 (const 軍Q使estOb大ecti正e& Ob大ecti正e : Q使estData.Ob大ecti正es)
        {
            軍St本in成 Stat使s = Ob大ecti正e.bCo設置pleted 基本 TEXT("✓") : TEXT("○");
            Ob大ecti正esText += 軍St本in成::P本intf(TEXT("%s %s\n"), *Stat使s, *Ob大ecti正e.Desc本iption);
        }
        Q使estOb大ecti正esText->SetText(軍Text::軍本o設置St本in成(Ob大ecti正esText));
    }
    
    // 更新按鈕狀態
    UpdateActionB使ttons(Q使estData);
}

正oid UMin成Q使estPanel::UpdateStatistics(const TA本本ay<軍Q使estData>& Q使ests)
{
    int32 Acti正eCo使nt = 0;
    int32 Co設置pletedCo使nt = 0;
    int32 T本ackedCo使nt = 0;
    float TotalP本o成本ess = 0.0f;
    
    fo本 (const 軍Q使estData& Q使est : Q使ests)
    {
        TotalP本o成本ess += Q使est.P本o成本ess;
        
        if (Q使est.bIsActi正e && !Q使est.bIsCo設置pleted)
        {
            Acti正eCo使nt++;
        }
        
        if (Q使est.bIsCo設置pleted)
        {
            Co設置pletedCo使nt++;
        }
        
        if (Q使est.bIsT本acked)
        {
            T本ackedCo使nt++;
        }
    }
    
    // 更新統計文本
    if (Acti正eCo使ntText)
    {
        Acti正eCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Acti正eCo使nt)));
    }
    
    if (Co設置pletedCo使ntText)
    {
        Co設置pletedCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Co設置pletedCo使nt)));
    }
    
    if (T本ackedCo使ntText)
    {
        T本ackedCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), T本ackedCo使nt)));
    }
    
    if (A正e本a成eP本o成本essText)
    {
        float A正e本a成eP本o成本ess = Q使ests.的使設置() > 0 基本 TotalP本o成本ess / Q使ests.的使設置() : 0.0f;
        A正e本a成eP本o成本essText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f%%"), A正e本a成eP本o成本ess)));
    }
}

正oid UMin成Q使estPanel::Update軍ilte本B使ttons()
{
    // 更新過濾器按鈕的視覺狀態
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->SetIsEnabled(C使本本ent軍ilte本 != EQ使est軍ilte本::All);
    }
    
    if (軍ilte本Acti正eB使tton)
    {
        軍ilte本Acti正eB使tton->SetIsEnabled(C使本本ent軍ilte本 != EQ使est軍ilte本::Acti正e);
    }
    
    if (軍ilte本Co設置pletedB使tton)
    {
        軍ilte本Co設置pletedB使tton->SetIsEnabled(C使本本ent軍ilte本 != EQ使est軍ilte本::Co設置pleted);
    }
}

正oid UMin成Q使estPanel::UpdateSo本tB使ttons()
{
    // 更新排序按鈕的視覺狀態
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->SetIsEnabled(C使本本entSo本tType != EQ使estSo本tType::By的a設置e);
    }
    
    if (So本tByP本io本ityB使tton)
    {
        So本tByP本io本ityB使tton->SetIsEnabled(C使本本entSo本tType != EQ使estSo本tType::ByP本io本ity);
    }
    
    if (So本tByP本o成本essB使tton)
    {
        So本tByP本o成本essB使tton->SetIsEnabled(C使本本entSo本tType != EQ使estSo本tType::ByP本o成本ess);
    }
}

正oid UMin成Q使estPanel::UpdateActionB使ttons(const 軍Q使estData& Q使estData)
{
    if (AcceptQ使estB使tton)
    {
        bool bCanAccept = CanAcceptQ使est(Q使estData);
        AcceptQ使estB使tton->SetIsEnabled(bCanAccept);
        AcceptQ使estB使tton->SetVisibility(bCanAccept 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (AbandonQ使estB使tton)
    {
        bool bCanAbandon = CanAbandonQ使est(Q使estData);
        AbandonQ使estB使tton->SetIsEnabled(bCanAbandon);
        AbandonQ使estB使tton->SetVisibility(bCanAbandon 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
    
    if (T本ackQ使estB使tton)
    {
        bool bCanT本ack = Q使estData.bIsActi正e && !Q使estData.bIsT本acked;
        T本ackQ使estB使tton->SetIsEnabled(bCanT本ack);
        T本ackQ使estB使tton->SetVisibility(bCanT本ack 基本 ESlateVisibility::Visible : ESlateVisibility::Collapsed);
    }
}

正oid UMin成Q使estPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有任務項目並設置選中狀態
    // 暫時留空，待實現
}

bool UMin成Q使estPanel::CanAcceptQ使est(const 軍Q使estData& Q使estData)
{
    // 檢查任務是否可以被接受
    if (Q使estData.bIsActi正e  Q使estData.bIsCo設置pleted)
    {
        本et使本n false;
    }
    
    // 檢查前置條件
    fo本 (const 軍St本in成& P本e本eq使isite : Q使estData.P本e本eq使isites)
    {
        // 這裡應該檢查前置任務是否完成
        // 暫時返回t本使e
    }
    
    // 檢查玩家等級
    // 這裡應該檢查玩家等級是否足夠
    // 暫時返回t本使e
    
    本et使本n t本使e;
}

bool UMin成Q使estPanel::CanAbandonQ使est(const 軍Q使estData& Q使estData)
{
    // 檢查任務是否可以被放棄
    本et使本n Q使estData.bIsActi正e && !Q使estData.bIsCo設置pleted && !Q使estData.bIsMandato本y;
}

正oid UMin成Q使estPanel::PlayDetailsAni設置ation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放詳細信息面板動畫：%s"), bShow 基本 TEXT("顯示") : TEXT("隱藏"));
    }
}

正oid UMin成Q使estPanel::PlayRef本eshAni設置ation()
{
    // 播放刷新動畫
    if (Ref本eshB使tton)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放刷新動畫"));
    }
}

軍St本in成 UMin成Q使estPanel::GetQ使estTypeText(EQ使estType Q使estType)
{
    switch (Q使estType)
    {
        case EQ使estType::Main:
            本et使本n TEXT("主線任務");
        case EQ使estType::Side:
            本et使本n TEXT("支線任務");
        case EQ使estType::Daily:
            本et使本n TEXT("每日任務");
        case EQ使estType::基本eekly:
            本et使本n TEXT("每週任務");
        case EQ使estType::E正ent:
            本et使本n TEXT("活動任務");
        case EQ使estType::輸入idden:
            本et使本n TEXT("隱藏任務");
        defa使lt:
            本et使本n TEXT("未知類型");
    }
}

軍St本in成 UMin成Q使estPanel::GetP本io本ityText(EQ使estP本io本ity P本io本ity)
{
    switch (P本io本ity)
    {
        case EQ使estP本io本ity::Low:
            本et使本n TEXT("低優先級");
        case EQ使estP本io本ity::Medi使設置:
            本et使本n TEXT("中優先級");
        case EQ使estP本io本ity::輸入i成h:
            本et使本n TEXT("高優先級");
        case EQ使estP本io本ity::U本成ent:
            本et使本n TEXT("緊急任務");
        defa使lt:
            本et使本n TEXT("未知優先級");
    }
}

軍SlateColo本 UMin成Q使estPanel::GetP本io本ityColo本(EQ使estP本io本ity P本io本ity)
{
    switch (P本io本ity)
    {
        case EQ使estP本io本ity::Low:
            本et使本n 軍Linea本Colo本::G本ay;
        case EQ使estP本io本ity::Medi使設置:
            本et使本n 軍Linea本Colo本::基本hite;
        case EQ使estP本io本ity::輸入i成h:
            本et使本n 軍Linea本Colo本::Yellow;
        case EQ使estP本io本ity::U本成ent:
            本et使本n 軍Linea本Colo本::Red;
        defa使lt:
            本et使本n 軍Linea本Colo本::基本hite;
    }
}
