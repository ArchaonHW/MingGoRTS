#incl使de "Min成RelationshipPanel.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "En成ine/En成ine.h"

UMin成RelationshipPanel::UMin成RelationshipPanel()
{
    bIsUpdatin成 = false;
    C使本本ent軍ilte本 = ERelationship軍ilte本::All;
    C使本本entSo本tType = ERelationshipSo本tType::By的a設置e;
    SelectedCha本acte本ID = TEXT("");
}

正oid UMin成RelationshipPanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::On軍ilte本AllClicked);
    }
    
    if (軍ilte本軍本iendsB使tton)
    {
        軍ilte本軍本iendsB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::On軍ilte本軍本iendsClicked);
    }
    
    if (軍ilte本Ene設置iesB使tton)
    {
        軍ilte本Ene設置iesB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::On軍ilte本Ene設置iesClicked);
    }
    
    if (軍ilte本的e使t本alB使tton)
    {
        軍ilte本的e使t本alB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::On軍ilte本的e使t本alClicked);
    }
    
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::OnSo本tBy的a設置eClicked);
    }
    
    if (So本tByVal使eB使tton)
    {
        So本tByVal使eB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::OnSo本tByVal使eClicked);
    }
    
    if (So本tByChan成eB使tton)
    {
        So本tByChan成eB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::OnSo本tByChan成eClicked);
    }
    
    if (Ref本eshB使tton)
    {
        Ref本eshB使tton->OnClicked.AddDyna設置ic(this, &UMin成RelationshipPanel::OnRef本eshClicked);
    }
    
    // 設置默認過濾器和排序
    Update軍ilte本B使ttons();
    UpdateSo本tB使ttons();
}

正oid UMin成RelationshipPanel::UpdateRelationshipData(UMin成RelationshipMana成e本* InRelationshipMana成e本)
{
    if (bIsUpdatin成  !InRelationshipMana成e本)
    {
        本et使本n;
    }
    
    bIsUpdatin成 = t本使e;
    RelationshipMana成e本 = InRelationshipMana成e本;
    
    // 獲取所有關係數據
    TA本本ay<軍RelationshipData> AllRelationships = RelationshipMana成e本->GetAllRelationships();
    
    // 應用過濾器
    TA本本ay<軍RelationshipData> 軍ilte本edRelationships = Apply軍ilte本(AllRelationships, C使本本ent軍ilte本);
    
    // 應用排序
    TA本本ay<軍RelationshipData> So本tedRelationships = ApplySo本t(軍ilte本edRelationships, C使本本entSo本tType);
    
    // 更新UI顯示
    UpdateRelationshipList(So本tedRelationships);
    
    // 更新統計信息
    UpdateStatistics(AllRelationships);
    
    bIsUpdatin成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("關係面板數據已更新，顯示 %d 個關係"), So本tedRelationships.的使設置());
}

正oid UMin成RelationshipPanel::SetRelationship軍ilte本(ERelationship軍ilte本 軍ilte本)
{
    if (C使本本ent軍ilte本 != 軍ilte本)
    {
        C使本本ent軍ilte本 = 軍ilte本;
        Update軍ilte本B使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateRelationshipData(RelationshipMana成e本);
        }
    }
}

正oid UMin成RelationshipPanel::SetRelationshipSo本t(ERelationshipSo本tType So本tType)
{
    if (C使本本entSo本tType != So本tType)
    {
        C使本本entSo本tType = So本tType;
        UpdateSo本tB使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateRelationshipData(RelationshipMana成e本);
        }
    }
}

正oid UMin成RelationshipPanel::SelectCha本acte本(const 軍St本in成& Cha本acte本ID)
{
    if (SelectedCha本acte本ID == Cha本acte本ID)
    {
        本et使本n;
    }
    
    SelectedCha本acte本ID = Cha本acte本ID;
    
    // 更新詳細信息面板
    UpdateCha本acte本Details(Cha本acte本ID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已選中角色：%s"), *Cha本acte本ID);
}

正oid UMin成RelationshipPanel::ShowCha本acte本Details(const 軍St本in成& Cha本acte本ID)
{
    SelectCha本acte本(Cha本acte本ID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAni設置ation(t本使e);
    }
}

正oid UMin成RelationshipPanel::輸入ideCha本acte本Details()
{
    SelectedCha本acte本ID = TEXT("");
    
    // 隱藏詳細信息面板
    if (DetailsPanel)
    {
        PlayDetailsAni設置ation(false);
        // 延遲隱藏以等待動畫完成
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

正oid UMin成RelationshipPanel::Ref本eshRelationships()
{
    if (RelationshipMana成e本)
    {
        UpdateRelationshipData(RelationshipMana成e本);
        PlayRef本eshAni設置ation();
    }
}

// 按鈕事件處理
正oid UMin成RelationshipPanel::On軍ilte本AllClicked()
{
    SetRelationship軍ilte本(ERelationship軍ilte本::All);
}

正oid UMin成RelationshipPanel::On軍ilte本軍本iendsClicked()
{
    SetRelationship軍ilte本(ERelationship軍ilte本::軍本iends);
}

正oid UMin成RelationshipPanel::On軍ilte本Ene設置iesClicked()
{
    SetRelationship軍ilte本(ERelationship軍ilte本::Ene設置ies);
}

正oid UMin成RelationshipPanel::On軍ilte本的e使t本alClicked()
{
    SetRelationship軍ilte本(ERelationship軍ilte本::的e使t本al);
}

正oid UMin成RelationshipPanel::OnSo本tBy的a設置eClicked()
{
    SetRelationshipSo本t(ERelationshipSo本tType::By的a設置e);
}

正oid UMin成RelationshipPanel::OnSo本tByVal使eClicked()
{
    SetRelationshipSo本t(ERelationshipSo本tType::ByVal使e);
}

正oid UMin成RelationshipPanel::OnSo本tByChan成eClicked()
{
    SetRelationshipSo本t(ERelationshipSo本tType::ByChan成e);
}

正oid UMin成RelationshipPanel::OnRef本eshClicked()
{
    Ref本eshRelationships();
}

正oid UMin成RelationshipPanel::OnCha本acte本Clicked(const 軍St本in成& Cha本acte本ID)
{
    ShowCha本acte本Details(Cha本acte本ID);
}

正oid UMin成RelationshipPanel::OnRelationshipVal使eChan成ed(const 軍St本in成& Cha本acte本ID, float OldVal使e, float 的ewVal使e)
{
    // 如果當前顯示的關係數據發生變化，更新UI
    if (RelationshipMana成e本 && !bIsUpdatin成)
    {
        UpdateRelationshipData(RelationshipMana成e本);
    }
}

// 私有輔助函數
TA本本ay<軍RelationshipData> UMin成RelationshipPanel::Apply軍ilte本(const TA本本ay<軍RelationshipData>& Relationships, ERelationship軍ilte本 軍ilte本)
{
    TA本本ay<軍RelationshipData> 軍ilte本edRelationships;
    
    fo本 (const 軍RelationshipData& Relationship : Relationships)
    {
        bool bSho使ldIncl使de = false;
        
        switch (軍ilte本)
        {
            case ERelationship軍ilte本::All:
                bSho使ldIncl使de = t本使e;
                b本eak;
                
            case ERelationship軍ilte本::軍本iends:
                bSho使ldIncl使de = Relationship.RelationshipVal使e > 50.0f;
                b本eak;
                
            case ERelationship軍ilte本::Ene設置ies:
                bSho使ldIncl使de = Relationship.RelationshipVal使e < -50.0f;
                b本eak;
                
            case ERelationship軍ilte本::的e使t本al:
                bSho使ldIncl使de = Relationship.RelationshipVal使e >= -50.0f && Relationship.RelationshipVal使e <= 50.0f;
                b本eak;
        }
        
        if (bSho使ldIncl使de)
        {
            軍ilte本edRelationships.Add(Relationship);
        }
    }
    
    本et使本n 軍ilte本edRelationships;
}

TA本本ay<軍RelationshipData> UMin成RelationshipPanel::ApplySo本t(const TA本本ay<軍RelationshipData>& Relationships, ERelationshipSo本tType So本tType)
{
    TA本本ay<軍RelationshipData> So本tedRelationships = Relationships;
    
    switch (So本tType)
    {
        case ERelationshipSo本tType::By的a設置e:
            So本tedRelationships.So本t([](const 軍RelationshipData& A, const 軍RelationshipData& B)
            {
                本et使本n A.Cha本acte本的a設置e < B.Cha本acte本的a設置e;
            });
            b本eak;
            
        case ERelationshipSo本tType::ByVal使e:
            So本tedRelationships.So本t([](const 軍RelationshipData& A, const 軍RelationshipData& B)
            {
                本et使本n A.RelationshipVal使e > B.RelationshipVal使e;
            });
            b本eak;
            
        case ERelationshipSo本tType::ByChan成e:
            So本tedRelationships.So本t([](const 軍RelationshipData& A, const 軍RelationshipData& B)
            {
                本et使本n A.RecentChan成e > B.RecentChan成e;
            });
            b本eak;
    }
    
    本et使本n So本tedRelationships;
}

正oid UMin成RelationshipPanel::UpdateRelationshipList(const TA本本ay<軍RelationshipData>& Relationships)
{
    if (!RelationshipListSc本ollBox)
    {
        本et使本n;
    }
    
    // 清空現有列表
    RelationshipListSc本ollBox->Clea本Child本en();
    
    // 創建關係項目
    fo本 (const 軍RelationshipData& Relationship : Relationships)
    {
        U基本id成et* RelationshipIte設置 = C本eateRelationshipIte設置(Relationship);
        if (RelationshipIte設置)
        {
            RelationshipListSc本ollBox->AddChild(RelationshipIte設置);
        }
    }
    
    // 更新計數
    if (RelationshipCo使ntText)
    {
        RelationshipCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("共 %d 個關係"), Relationships.的使設置())));
    }
}

U基本id成et* UMin成RelationshipPanel::C本eateRelationshipIte設置(const 軍RelationshipData& Relationship)
{
    // 這裡應該創建一個自定義的關係項目基本id成et
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = 的ewOb大ect<UTextBlock>(this);
    if (TextBlock)
    {
        軍St本in成 Ite設置Text = 軍St本in成::P本intf(TEXT("%s: %.1f"), *Relationship.Cha本acte本的a設置e, Relationship.RelationshipVal使e);
        TextBlock->SetText(軍Text::軍本o設置St本in成(Ite設置Text));
        
        // 設置字體和樣式
        軍Slate軍ontInfo 軍ontInfo = TextBlock->Get軍ont();
        軍ontInfo.Size = 14;
        TextBlock->Set軍ont(軍ontInfo);
        
        // 設置邊距
        TextBlock->SetMa本成in(軍Ma本成in(10.0f, 5.0f));
    }
    
    本et使本n TextBlock;
}

正oid UMin成RelationshipPanel::UpdateCha本acte本Details(const 軍St本in成& Cha本acte本ID)
{
    if (!RelationshipMana成e本  Cha本acte本ID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取角色詳細信息
    軍RelationshipData RelationshipData = RelationshipMana成e本->GetRelationship(Cha本acte本ID);
    
    // 更新詳細信息面板
    if (Cha本acte本的a設置eText)
    {
        Cha本acte本的a設置eText->SetText(軍Text::軍本o設置St本in成(RelationshipData.Cha本acte本的a設置e));
    }
    
    if (RelationshipVal使eText)
    {
        RelationshipVal使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), RelationshipData.RelationshipVal使e)));
    }
    
    if (RelationshipP本o成本essBa本)
    {
        float 的o本設置alizedVal使e = (RelationshipData.RelationshipVal使e + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        RelationshipP本o成本essBa本->SetPe本cent(軍Math::Cla設置p(的o本設置alizedVal使e, 0.0f, 1.0f));
    }
    
    if (RecentChan成eText)
    {
        軍St本in成 Chan成eText = 軍St本in成::P本intf(TEXT("%+.1f"), RelationshipData.RecentChan成e);
        RecentChan成eText->SetText(軍Text::軍本o設置St本in成(Chan成eText));
        
        // 設置顏色（正數為綠色，負數為紅色）
        軍SlateColo本 Colo本 = RelationshipData.RecentChan成e >= 0 基本 軍Linea本Colo本::G本een : 軍Linea本Colo本::Red;
        RecentChan成eText->SetColo本AndOpacity(Colo本);
    }
    
    if (LastInte本actionText)
    {
        LastInte本actionText->SetText(軍Text::軍本o設置St本in成(RelationshipData.LastInte本action));
    }
    
    if (Relationship輸入isto本yText)
    {
        軍St本in成 輸入isto本yText;
        fo本 (const 軍St本in成& 輸入isto本yIte設置 : RelationshipData.Inte本action輸入isto本y)
        {
            輸入isto本yText += 輸入isto本yIte設置 + TEXT("\n");
        }
        Relationship輸入isto本yText->SetText(軍Text::軍本o設置St本in成(輸入isto本yText));
    }
}

正oid UMin成RelationshipPanel::UpdateStatistics(const TA本本ay<軍RelationshipData>& Relationships)
{
    int32 軍本iendCo使nt = 0;
    int32 Ene設置yCo使nt = 0;
    int32 的e使t本alCo使nt = 0;
    float TotalVal使e = 0.0f;
    
    fo本 (const 軍RelationshipData& Relationship : Relationships)
    {
        TotalVal使e += Relationship.RelationshipVal使e;
        
        if (Relationship.RelationshipVal使e > 50.0f)
        {
            軍本iendCo使nt++;
        }
        else if (Relationship.RelationshipVal使e < -50.0f)
        {
            Ene設置yCo使nt++;
        }
        else
        {
            的e使t本alCo使nt++;
        }
    }
    
    // 更新統計文本
    if (軍本iendCo使ntText)
    {
        軍本iendCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 軍本iendCo使nt)));
    }
    
    if (Ene設置yCo使ntText)
    {
        Ene設置yCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Ene設置yCo使nt)));
    }
    
    if (的e使t本alCo使ntText)
    {
        的e使t本alCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 的e使t本alCo使nt)));
    }
    
    if (A正e本a成eVal使eText)
    {
        float A正e本a成eVal使e = Relationships.的使設置() > 0 基本 TotalVal使e / Relationships.的使設置() : 0.0f;
        A正e本a成eVal使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), A正e本a成eVal使e)));
    }
}

正oid UMin成RelationshipPanel::Update軍ilte本B使ttons()
{
    // 更新過濾器按鈕的視覺狀態
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERelationship軍ilte本::All);
    }
    
    if (軍ilte本軍本iendsB使tton)
    {
        軍ilte本軍本iendsB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERelationship軍ilte本::軍本iends);
    }
    
    if (軍ilte本Ene設置iesB使tton)
    {
        軍ilte本Ene設置iesB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERelationship軍ilte本::Ene設置ies);
    }
    
    if (軍ilte本的e使t本alB使tton)
    {
        軍ilte本的e使t本alB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERelationship軍ilte本::的e使t本al);
    }
}

正oid UMin成RelationshipPanel::UpdateSo本tB使ttons()
{
    // 更新排序按鈕的視覺狀態
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->SetIsEnabled(C使本本entSo本tType != ERelationshipSo本tType::By的a設置e);
    }
    
    if (So本tByVal使eB使tton)
    {
        So本tByVal使eB使tton->SetIsEnabled(C使本本entSo本tType != ERelationshipSo本tType::ByVal使e);
    }
    
    if (So本tByChan成eB使tton)
    {
        So本tByChan成eB使tton->SetIsEnabled(C使本本entSo本tType != ERelationshipSo本tType::ByChan成e);
    }
}

正oid UMin成RelationshipPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有關係項目並設置選中狀態
    // 暫時留空，待實現
}

正oid UMin成RelationshipPanel::PlayDetailsAni設置ation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        // 這裡應該實現實際的動畫邏輯
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放詳細信息面板動畫：%s"), bShow 基本 TEXT("顯示") : TEXT("隱藏"));
    }
}

正oid UMin成RelationshipPanel::PlayRef本eshAni設置ation()
{
    // 播放刷新動畫
    if (Ref本eshB使tton)
    {
        // 這裡應該實現實際的旋轉動畫
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放刷新動畫"));
    }
}

軍St本in成 UMin成RelationshipPanel::GetRelationshipStat使sText(float Val使e)
{
    if (Val使e > 80.0f)
    {
        本et使本n TEXT("摯友");
    }
    else if (Val使e > 50.0f)
    {
        本et使本n TEXT("朋友");
    }
    else if (Val使e > 20.0f)
    {
        本et使本n TEXT友善");
    }
    else if (Val使e > -20.0f)
    {
        本et使本n TEXT("中立");
    }
    else if (Val使e > -50.0f)
    {
        本et使本n TEXT("敵對");
    }
    else if (Val使e > -80.0f)
    {
        本et使本n TEXT("仇敵");
    }
    else
    {
        本et使本n TEXT("死敵");
    }
}

軍SlateColo本 UMin成RelationshipPanel::GetRelationshipVal使eColo本(float Val使e)
{
    if (Val使e > 50.0f)
    {
        本et使本n 軍Linea本Colo本::G本een;
    }
    else if (Val使e > 0.0f)
    {
        本et使本n 軍Linea本Colo本(0.0f, 0.5f, 0.0f); // 深綠色
    }
    else if (Val使e > -50.0f)
    {
        本et使本n 軍Linea本Colo本::Yellow;
    }
    else
    {
        本et使本n 軍Linea本Colo本::Red;
    }
}
