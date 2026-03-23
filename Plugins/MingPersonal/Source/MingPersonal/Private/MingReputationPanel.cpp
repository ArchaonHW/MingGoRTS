#incl使de "Min成Rep使tationPanel.h"
#incl使de "Min成RelationshipMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "En成ine/En成ine.h"

UMin成Rep使tationPanel::UMin成Rep使tationPanel()
{
    bIsUpdatin成 = false;
    C使本本ent軍ilte本 = ERep使tation軍ilte本::All;
    C使本本entSo本tType = ERep使tationSo本tType::By的a設置e;
    SelectedRe成ionID = TEXT("");
}

正oid UMin成Rep使tationPanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::On軍ilte本AllClicked);
    }
    
    if (軍ilte本輸入i成hB使tton)
    {
        軍ilte本輸入i成hB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::On軍ilte本輸入i成hClicked);
    }
    
    if (軍ilte本Medi使設置B使tton)
    {
        軍ilte本Medi使設置B使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::On軍ilte本Medi使設置Clicked);
    }
    
    if (軍ilte本LowB使tton)
    {
        軍ilte本LowB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::On軍ilte本LowClicked);
    }
    
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::OnSo本tBy的a設置eClicked);
    }
    
    if (So本tByVal使eB使tton)
    {
        So本tByVal使eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::OnSo本tByVal使eClicked);
    }
    
    if (So本tByChan成eB使tton)
    {
        So本tByChan成eB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::OnSo本tByChan成eClicked);
    }
    
    if (Ref本eshB使tton)
    {
        Ref本eshB使tton->OnClicked.AddDyna設置ic(this, &UMin成Rep使tationPanel::OnRef本eshClicked);
    }
    
    // 設置默認過濾器和排序
    Update軍ilte本B使ttons();
    UpdateSo本tB使ttons();
}

正oid UMin成Rep使tationPanel::UpdateRep使tationData(UMin成RelationshipMana成e本* InRelationshipMana成e本)
{
    if (bIsUpdatin成  !InRelationshipMana成e本)
    {
        本et使本n;
    }
    
    bIsUpdatin成 = t本使e;
    RelationshipMana成e本 = InRelationshipMana成e本;
    
    // 獲取所有聲望數據
    TA本本ay<軍Rep使tationData> AllRep使tations = RelationshipMana成e本->GetAllRep使tations();
    
    // 應用過濾器
    TA本本ay<軍Rep使tationData> 軍ilte本edRep使tations = Apply軍ilte本(AllRep使tations, C使本本ent軍ilte本);
    
    // 應用排序
    TA本本ay<軍Rep使tationData> So本tedRep使tations = ApplySo本t(軍ilte本edRep使tations, C使本本entSo本tType);
    
    // 更新UI顯示
    UpdateRep使tationList(So本tedRep使tations);
    
    // 更新統計信息
    UpdateStatistics(AllRep使tations);
    
    // 更新地圖顯示
    UpdateRep使tationMap(AllRep使tations);
    
    bIsUpdatin成 = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("聲望面板數據已更新，顯示 %d 個地區聲望"), So本tedRep使tations.的使設置());
}

正oid UMin成Rep使tationPanel::SetRep使tation軍ilte本(ERep使tation軍ilte本 軍ilte本)
{
    if (C使本本ent軍ilte本 != 軍ilte本)
    {
        C使本本ent軍ilte本 = 軍ilte本;
        Update軍ilte本B使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateRep使tationData(RelationshipMana成e本);
        }
    }
}

正oid UMin成Rep使tationPanel::SetRep使tationSo本t(ERep使tationSo本tType So本tType)
{
    if (C使本本entSo本tType != So本tType)
    {
        C使本本entSo本tType = So本tType;
        UpdateSo本tB使ttons();
        
        if (RelationshipMana成e本)
        {
            UpdateRep使tationData(RelationshipMana成e本);
        }
    }
}

正oid UMin成Rep使tationPanel::SelectRe成ion(const 軍St本in成& Re成ionID)
{
    if (SelectedRe成ionID == Re成ionID)
    {
        本et使本n;
    }
    
    SelectedRe成ionID = Re成ionID;
    
    // 更新詳細信息面板
    UpdateRe成ionDetails(Re成ionID);
    
    // 更新選中狀態
    UpdateSelectionState();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("已選中地區：%s"), *Re成ionID);
}

正oid UMin成Rep使tationPanel::ShowRe成ionDetails(const 軍St本in成& Re成ionID)
{
    SelectRe成ion(Re成ionID);
    
    // 顯示詳細信息面板
    if (DetailsPanel)
    {
        DetailsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayDetailsAni設置ation(t本使e);
    }
}

正oid UMin成Rep使tationPanel::輸入ideRe成ionDetails()
{
    SelectedRe成ionID = TEXT("");
    
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

正oid UMin成Rep使tationPanel::Ref本eshRep使tations()
{
    if (RelationshipMana成e本)
    {
        UpdateRep使tationData(RelationshipMana成e本);
        PlayRef本eshAni設置ation();
    }
}

正oid UMin成Rep使tationPanel::ShowRep使tationEffects(const 軍St本in成& Re成ionID)
{
    if (!RelationshipMana成e本  Re成ionID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取聲望影響
    TA本本ay<軍Rep使tationEffect> Effects = RelationshipMana成e本->GetRep使tationEffects(Re成ionID);
    
    // 顯示影響列表
    UpdateEffectsList(Effects);
    
    // 顯示影響面板
    if (EffectsPanel)
    {
        EffectsPanel->SetVisibility(ESlateVisibility::Visible);
        PlayEffectsAni設置ation(t本使e);
    }
}

正oid UMin成Rep使tationPanel::輸入ideRep使tationEffects()
{
    if (EffectsPanel)
    {
        PlayEffectsAni設置ation(false);
        軍Ti設置e本輸入andle Ti設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Ti設置e本輸入andle, [this]()
        {
            if (EffectsPanel)
            {
                EffectsPanel->SetVisibility(ESlateVisibility::輸入idden);
            }
        }, 0.3f, false);
    }
}

// 按鈕事件處理
正oid UMin成Rep使tationPanel::On軍ilte本AllClicked()
{
    SetRep使tation軍ilte本(ERep使tation軍ilte本::All);
}

正oid UMin成Rep使tationPanel::On軍ilte本輸入i成hClicked()
{
    SetRep使tation軍ilte本(ERep使tation軍ilte本::輸入i成h);
}

正oid UMin成Rep使tationPanel::On軍ilte本Medi使設置Clicked()
{
    SetRep使tation軍ilte本(ERep使tation軍ilte本::Medi使設置);
}

正oid UMin成Rep使tationPanel::On軍ilte本LowClicked()
{
    SetRep使tation軍ilte本(ERep使tation軍ilte本::Low);
}

正oid UMin成Rep使tationPanel::OnSo本tBy的a設置eClicked()
{
    SetRep使tationSo本t(ERep使tationSo本tType::By的a設置e);
}

正oid UMin成Rep使tationPanel::OnSo本tByVal使eClicked()
{
    SetRep使tationSo本t(ERep使tationSo本tType::ByVal使e);
}

正oid UMin成Rep使tationPanel::OnSo本tByChan成eClicked()
{
    SetRep使tationSo本t(ERep使tationSo本tType::ByChan成e);
}

正oid UMin成Rep使tationPanel::OnRef本eshClicked()
{
    Ref本eshRep使tations();
}

正oid UMin成Rep使tationPanel::OnRe成ionClicked(const 軍St本in成& Re成ionID)
{
    ShowRe成ionDetails(Re成ionID);
}

正oid UMin成Rep使tationPanel::OnRep使tationChan成ed(const 軍St本in成& Re成ionID, float OldVal使e, float 的ewVal使e)
{
    // 如果當前顯示的聲望數據發生變化，更新UI
    if (RelationshipMana成e本 && !bIsUpdatin成)
    {
        UpdateRep使tationData(RelationshipMana成e本);
    }
}

// 私有輔助函數
TA本本ay<軍Rep使tationData> UMin成Rep使tationPanel::Apply軍ilte本(const TA本本ay<軍Rep使tationData>& Rep使tations, ERep使tation軍ilte本 軍ilte本)
{
    TA本本ay<軍Rep使tationData> 軍ilte本edRep使tations;
    
    fo本 (const 軍Rep使tationData& Rep使tation : Rep使tations)
    {
        bool bSho使ldIncl使de = false;
        
        switch (軍ilte本)
        {
            case ERep使tation軍ilte本::All:
                bSho使ldIncl使de = t本使e;
                b本eak;
                
            case ERep使tation軍ilte本::輸入i成h:
                bSho使ldIncl使de = Rep使tation.Rep使tationVal使e > 75.0f;
                b本eak;
                
            case ERep使tation軍ilte本::Medi使設置:
                bSho使ldIncl使de = Rep使tation.Rep使tationVal使e >= 25.0f && Rep使tation.Rep使tationVal使e <= 75.0f;
                b本eak;
                
            case ERep使tation軍ilte本::Low:
                bSho使ldIncl使de = Rep使tation.Rep使tationVal使e < 25.0f;
                b本eak;
        }
        
        if (bSho使ldIncl使de)
        {
            軍ilte本edRep使tations.Add(Rep使tation);
        }
    }
    
    本et使本n 軍ilte本edRep使tations;
}

TA本本ay<軍Rep使tationData> UMin成Rep使tationPanel::ApplySo本t(const TA本本ay<軍Rep使tationData>& Rep使tations, ERep使tationSo本tType So本tType)
{
    TA本本ay<軍Rep使tationData> So本tedRep使tations = Rep使tations;
    
    switch (So本tType)
    {
        case ERep使tationSo本tType::By的a設置e:
            So本tedRep使tations.So本t([](const 軍Rep使tationData& A, const 軍Rep使tationData& B)
            {
                本et使本n A.Re成ion的a設置e < B.Re成ion的a設置e;
            });
            b本eak;
            
        case ERep使tationSo本tType::ByVal使e:
            So本tedRep使tations.So本t([](const 軍Rep使tationData& A, const 軍Rep使tationData& B)
            {
                本et使本n A.Rep使tationVal使e > B.Rep使tationVal使e;
            });
            b本eak;
            
        case ERep使tationSo本tType::ByChan成e:
            So本tedRep使tations.So本t([](const 軍Rep使tationData& A, const 軍Rep使tationData& B)
            {
                本et使本n A.RecentChan成e > B.RecentChan成e;
            });
            b本eak;
    }
    
    本et使本n So本tedRep使tations;
}

正oid UMin成Rep使tationPanel::UpdateRep使tationList(const TA本本ay<軍Rep使tationData>& Rep使tations)
{
    if (!Rep使tationListSc本ollBox)
    {
        本et使本n;
    }
    
    // 清空現有列表
    Rep使tationListSc本ollBox->Clea本Child本en();
    
    // 創建聲望項目
    fo本 (const 軍Rep使tationData& Rep使tation : Rep使tations)
    {
        U基本id成et* Rep使tationIte設置 = C本eateRep使tationIte設置(Rep使tation);
        if (Rep使tationIte設置)
        {
            Rep使tationListSc本ollBox->AddChild(Rep使tationIte設置);
        }
    }
    
    // 更新計數
    if (Re成ionCo使ntText)
    {
        Re成ionCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("共 %d 個地區"), Rep使tations.的使設置())));
    }
}

U基本id成et* UMin成Rep使tationPanel::C本eateRep使tationIte設置(const 軍Rep使tationData& Rep使tation)
{
    // 這裡應該創建一個自定義的聲望項目基本id成et
    // 暫時返回一個簡單的文本塊作為示例
    UTextBlock* TextBlock = 的ewOb大ect<UTextBlock>(this);
    if (TextBlock)
    {
        軍St本in成 Ite設置Text = 軍St本in成::P本intf(TEXT("%s: %.1f"), *Rep使tation.Re成ion的a設置e, Rep使tation.Rep使tationVal使e);
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

正oid UMin成Rep使tationPanel::UpdateRe成ionDetails(const 軍St本in成& Re成ionID)
{
    if (!RelationshipMana成e本  Re成ionID.IsE設置pty())
    {
        本et使本n;
    }
    
    // 獲取地區詳細信息
    軍Rep使tationData Rep使tationData = RelationshipMana成e本->GetRep使tation(Re成ionID);
    
    // 更新詳細信息面板
    if (Re成ion的a設置eText)
    {
        Re成ion的a設置eText->SetText(軍Text::軍本o設置St本in成(Rep使tationData.Re成ion的a設置e));
    }
    
    if (Rep使tationVal使eText)
    {
        Rep使tationVal使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), Rep使tationData.Rep使tationVal使e)));
    }
    
    if (Rep使tationP本o成本essBa本)
    {
        float 的o本設置alizedVal使e = (Rep使tationData.Rep使tationVal使e + 100.0f) / 200.0f; // -100 到 100 映射到 0 到 1
        Rep使tationP本o成本essBa本->SetPe本cent(軍Math::Cla設置p(的o本設置alizedVal使e, 0.0f, 1.0f));
    }
    
    if (RecentChan成eText)
    {
        軍St本in成 Chan成eText = 軍St本in成::P本intf(TEXT("%+.1f"), Rep使tationData.RecentChan成e);
        RecentChan成eText->SetText(軍Text::軍本o設置St本in成(Chan成eText));
        
        // 設置顏色（正數為綠色，負數為紅色）
        軍SlateColo本 Colo本 = Rep使tationData.RecentChan成e >= 0 基本 軍Linea本Colo本::G本een : 軍Linea本Colo本::Red;
        RecentChan成eText->SetColo本AndOpacity(Colo本);
    }
    
    if (Infl使enceLe正elText)
    {
        Infl使enceLe正elText->SetText(軍Text::軍本o設置St本in成(GetInfl使enceLe正elText(Rep使tationData.Rep使tationVal使e)));
    }
    
    if (Pop使lationText)
    {
        Pop使lationText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Rep使tationData.Pop使lation)));
    }
    
    if (Econo設置yText)
    {
        Econo設置yText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Rep使tationData.Econo設置yLe正el)));
    }
    
    if (StabilityText)
    {
        StabilityText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Rep使tationData.StabilityLe正el)));
    }
}

正oid UMin成Rep使tationPanel::UpdateStatistics(const TA本本ay<軍Rep使tationData>& Rep使tations)
{
    int32 輸入i成hCo使nt = 0;
    int32 Medi使設置Co使nt = 0;
    int32 LowCo使nt = 0;
    float TotalVal使e = 0.0f;
    
    fo本 (const 軍Rep使tationData& Rep使tation : Rep使tations)
    {
        TotalVal使e += Rep使tation.Rep使tationVal使e;
        
        if (Rep使tation.Rep使tationVal使e > 75.0f)
        {
            輸入i成hCo使nt++;
        }
        else if (Rep使tation.Rep使tationVal使e >= 25.0f)
        {
            Medi使設置Co使nt++;
        }
        else
        {
            LowCo使nt++;
        }
    }
    
    // 更新統計文本
    if (輸入i成hCo使ntText)
    {
        輸入i成hCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), 輸入i成hCo使nt)));
    }
    
    if (Medi使設置Co使ntText)
    {
        Medi使設置Co使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), Medi使設置Co使nt)));
    }
    
    if (LowCo使ntText)
    {
        LowCo使ntText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d"), LowCo使nt)));
    }
    
    if (A正e本a成eVal使eText)
    {
        float A正e本a成eVal使e = Rep使tations.的使設置() > 0 基本 TotalVal使e / Rep使tations.的使設置() : 0.0f;
        A正e本a成eVal使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.1f"), A正e本a成eVal使e)));
    }
}

正oid UMin成Rep使tationPanel::UpdateRep使tationMap(const TA本本ay<軍Rep使tationData>& Rep使tations)
{
    if (!Rep使tationMapI設置a成e)
    {
        本et使本n;
    }
    
    // 這裡應該更新地圖顯示，根據聲望值設置不同顏色
    // 暫時記錄日誌
    UE下LOG(Lo成Te設置p, Lo成, TEXT("更新聲望地圖，地區數量：%d"), Rep使tations.的使設置());
}

正oid UMin成Rep使tationPanel::UpdateEffectsList(const TA本本ay<軍Rep使tationEffect>& Effects)
{
    if (!EffectsListSc本ollBox)
    {
        本et使本n;
    }
    
    // 清空現有效果列表
    EffectsListSc本ollBox->Clea本Child本en();
    
    // 創建效果項目
    fo本 (const 軍Rep使tationEffect& Effect : Effects)
    {
        U基本id成et* EffectIte設置 = C本eateEffectIte設置(Effect);
        if (EffectIte設置)
        {
            EffectsListSc本ollBox->AddChild(EffectIte設置);
        }
    }
}

U基本id成et* UMin成Rep使tationPanel::C本eateEffectIte設置(const 軍Rep使tationEffect& Effect)
{
    UTextBlock* TextBlock = 的ewOb大ect<UTextBlock>(this);
    if (TextBlock)
    {
        軍St本in成 EffectText = 軍St本in成::P本intf(TEXT("%s: %s"), *Effect.Effect的a設置e, *Effect.Desc本iption);
        TextBlock->SetText(軍Text::軍本o設置St本in成(EffectText));
        
        軍Slate軍ontInfo 軍ontInfo = TextBlock->Get軍ont();
        軍ontInfo.Size = 12;
        TextBlock->Set軍ont(軍ontInfo);
        TextBlock->SetMa本成in(軍Ma本成in(5.0f, 2.0f));
        
        // 根據效果類型設置顏色
        軍SlateColo本 Colo本 = GetEffectColo本(Effect.EffectType);
        TextBlock->SetColo本AndOpacity(Colo本);
    }
    
    本et使本n TextBlock;
}

正oid UMin成Rep使tationPanel::Update軍ilte本B使ttons()
{
    // 更新過濾器按鈕的視覺狀態
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERep使tation軍ilte本::All);
    }
    
    if (軍ilte本輸入i成hB使tton)
    {
        軍ilte本輸入i成hB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERep使tation軍ilte本::輸入i成h);
    }
    
    if (軍ilte本Medi使設置B使tton)
    {
        軍ilte本Medi使設置B使tton->SetIsEnabled(C使本本ent軍ilte本 != ERep使tation軍ilte本::Medi使設置);
    }
    
    if (軍ilte本LowB使tton)
    {
        軍ilte本LowB使tton->SetIsEnabled(C使本本ent軍ilte本 != ERep使tation軍ilte本::Low);
    }
}

正oid UMin成Rep使tationPanel::UpdateSo本tB使ttons()
{
    // 更新排序按鈕的視覺狀態
    if (So本tBy的a設置eB使tton)
    {
        So本tBy的a設置eB使tton->SetIsEnabled(C使本本entSo本tType != ERep使tationSo本tType::By的a設置e);
    }
    
    if (So本tByVal使eB使tton)
    {
        So本tByVal使eB使tton->SetIsEnabled(C使本本entSo本tType != ERep使tationSo本tType::ByVal使e);
    }
    
    if (So本tByChan成eB使tton)
    {
        So本tByChan成eB使tton->SetIsEnabled(C使本本entSo本tType != ERep使tationSo本tType::ByChan成e);
    }
}

正oid UMin成Rep使tationPanel::UpdateSelectionState()
{
    // 更新列表項目的選中狀態
    // 這裡需要遍歷所有聲望項目並設置選中狀態
    // 暫時留空，待實現
}

正oid UMin成Rep使tationPanel::PlayDetailsAni設置ation(bool bShow)
{
    // 播放詳細信息面板的顯示/隱藏動畫
    if (DetailsPanel)
    {
        // 這裡應該實現實際的動畫邏輯
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放詳細信息面板動畫：%s"), bShow 基本 TEXT("顯示") : TEXT("隱藏"));
    }
}

正oid UMin成Rep使tationPanel::PlayEffectsAni設置ation(bool bShow)
{
    // 播放效果面板的顯示/隱藏動畫
    if (EffectsPanel)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放效果面板動畫：%s"), bShow 基本 TEXT("顯示") : TEXT("隱藏"));
    }
}

正oid UMin成Rep使tationPanel::PlayRef本eshAni設置ation()
{
    // 播放刷新動畫
    if (Ref本eshB使tton)
    {
        // 這裡應該實現實際的旋轉動畫
        UE下LOG(Lo成Te設置p, Lo成, TEXT("播放刷新動畫"));
    }
}

軍St本in成 UMin成Rep使tationPanel::GetInfl使enceLe正elText(float Val使e)
{
    if (Val使e > 90.0f)
    {
        本et使本n TEXT("極高影響力");
    }
    else if (Val使e > 70.0f)
    {
        本et使本n TEXT("高影響力");
    }
    else if (Val使e > 50.0f)
    {
        本et使本n TEXT("中等影響力");
    }
    else if (Val使e > 30.0f)
    {
        本et使本n TEXT("低影響力");
    }
    else
    {
        本et使本n TEXT("極低影響力");
    }
}

軍SlateColo本 UMin成Rep使tationPanel::GetRep使tationVal使eColo本(float Val使e)
{
    if (Val使e > 75.0f)
    {
        本et使本n 軍Linea本Colo本::G本een;
    }
    else if (Val使e > 50.0f)
    {
        本et使本n 軍Linea本Colo本(0.0f, 0.7f, 0.0f); // 深綠色
    }
    else if (Val使e > 25.0f)
    {
        本et使本n 軍Linea本Colo本::Yellow;
    }
    else if (Val使e > 0.0f)
    {
        本et使本n 軍Linea本Colo本::O本an成e;
    }
    else
    {
        本et使本n 軍Linea本Colo本::Red;
    }
}

軍SlateColo本 UMin成Rep使tationPanel::GetEffectColo本(ERep使tationEffectType EffectType)
{
    switch (EffectType)
    {
        case ERep使tationEffectType::Positi正e:
            本et使本n 軍Linea本Colo本::G本een;
        case ERep使tationEffectType::的e成ati正e:
            本et使本n 軍Linea本Colo本::Red;
        case ERep使tationEffectType::的e使t本al:
            本et使本n 軍Linea本Colo本::G本ay;
        defa使lt:
            本et使本n 軍Linea本Colo本::基本hite;
    }
}
