#incl使de "UI/Min成Uni正e本sityG使ide基本id成et.h"
#incl使de "Min成Uni正e本sityG使ideMana成e本.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/Sc本ollBox.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Ani設置ation/基本id成etAni設置ation.h"

正oid UMin成Uni正e本sityG使ide基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 綁定按鈕事件
    if (的extB使tton)
    {
        的extB使tton->OnClicked.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::On的extB使ttonClicked);
    }
    
    if (P本e正io使sB使tton)
    {
        P本e正io使sB使tton->OnClicked.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnP本e正io使sB使ttonClicked);
    }
    
    if (CloseB使tton)
    {
        CloseB使tton->OnClicked.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnCloseB使ttonClicked);
    }
    
    // 綁定管理器事件
    if (G使ideMana成e本)
    {
        G使ideMana成e本->OnG使ideContentUpdated.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideContentUpdated);
        G使ideMana成e本->OnG使ideCo設置pleted.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideCo設置pleted);
        G使ideMana成e本->OnG使ideStopped.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideStopped);
    }
    
    // 播放進入動畫
    if (Ente本Ani設置ation)
    {
        PlayAni設置ation(Ente本Ani設置ation);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Uni正e本sityG使ide基本id成et const本使cted"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::的ati正eDest本使ct()
{
    // 解除綁定管理器事件
    if (G使ideMana成e本)
    {
        G使ideMana成e本->OnG使ideContentUpdated.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideContentUpdated);
        G使ideMana成e本->OnG使ideCo設置pleted.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideCo設置pleted);
        G使ideMana成e本->OnG使ideStopped.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideStopped);
    }
    
    // 解除綁定按鈕事件
    if (的extB使tton)
    {
        的extB使tton->OnClicked.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::On的extB使ttonClicked);
    }
    
    if (P本e正io使sB使tton)
    {
        P本e正io使sB使tton->OnClicked.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnP本e正io使sB使ttonClicked);
    }
    
    if (CloseB使tton)
    {
        CloseB使tton->OnClicked.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnCloseB使ttonClicked);
    }
    
    S使pe本::的ati正eDest本使ct();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Uni正e本sityG使ide基本id成et dest本使cted"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::SetG使ideMana成e本(UMin成Uni正e本sityG使ideMana成e本* Mana成e本)
{
    // 解除舊的管理器事件綁定
    if (G使ideMana成e本)
    {
        G使ideMana成e本->OnG使ideContentUpdated.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideContentUpdated);
        G使ideMana成e本->OnG使ideCo設置pleted.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideCo設置pleted);
        G使ideMana成e本->OnG使ideStopped.Re設置o正eDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideStopped);
    }
    
    G使ideMana成e本 = Mana成e本;
    
    // 綁定新管理器的事件
    if (G使ideMana成e本)
    {
        G使ideMana成e本->OnG使ideContentUpdated.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideContentUpdated);
        G使ideMana成e本->OnG使ideCo設置pleted.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideCo設置pleted);
        G使ideMana成e本->OnG使ideStopped.AddDyna設置ic(this, &UMin成Uni正e本sityG使ide基本id成et::OnG使ideStopped);
        
        // 更新顯示
        UpdateContentDisplay();
        UpdateP本o成本essBa本();
        Update的a正i成ationB使ttons();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide 設置ana成e本 set"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::DisplayContent(const 軍Uni正e本sityG使ideContent& Content)
{
    // 更新標題
    if (TitleText)
    {
        TitleText->SetText(軍Text::軍本o設置St本in成(Content.ContentTitle));
    }
    
    // 更新描述
    if (Desc本iptionText)
    {
        Desc本iptionText->SetText(軍Text::軍本o設置St本in成(Content.ContentDesc本iption));
    }
    
    // 更新導航按鈕可見性
    if (P本e正io使sB使tton)
    {
        P本e正io使sB使tton->SetVisibility(Content.b輸入asP本e正io使sB使tton 基本 ESlateVisibility::Visible : ESlateVisibility::輸入idden);
    }
    
    if (的extB使tton)
    {
        軍Text B使ttonText = Content.b輸入as的extB使tton 基本 
            軍Text::軍本o設置St本in成(TEXT("下一步 →")) : 
            軍Text::軍本o設置St本in成(TEXT("完成 ✓"));
        
        的extB使tton->SetVisibility(ESlateVisibility::Visible);
        
        // 更新按鈕文字（如果按鈕有文本組件）
        if (UTextBlock* B使ttonText = Cast<UTextBlock>(的extB使tton->GetChildAt(0)))
        {
            B使ttonText->SetText(B使ttonText);
        }
    }
    
    // 更新內容索引顯示
    if (G使ideMana成e本)
    {
        軍St本in成 IndexText = 軍St本in成::P本intf(TEXT("%d / %d"), 
            G使ideMana成e本->GetC使本本entContentIndex() + 1, 
            G使ideMana成e本->GetTotalContentCo使nt());
        
        // 如果有索引文本組件，更新它
    }
    
    // 播放內容更新動畫
    if (ContentUpdateAni設置ation)
    {
        PlayAni設置ation(ContentUpdateAni設置ation);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Content displayed: %s"), *Content.ContentTitle);
}

正oid UMin成Uni正e本sityG使ide基本id成et::UpdateP本o成本essBa本()
{
    if (!G使ideMana成e本  !P本o成本essBa本)
    {
        本et使本n;
    }
    
    float P本o成本ess = G使ideMana成e本->GetG使ideP本o成本ess();
    P本o成本essBa本->SetPe本cent(P本o成本ess);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本o成本ess ba本 使pdated: %.2f%%"), P本o成本ess * 100.0f);
}

正oid UMin成Uni正e本sityG使ide基本id成et::UpdateContentDisplay()
{
    if (!G使ideMana成e本)
    {
        本et使本n;
    }
    
    軍Uni正e本sityG使ideContent C使本本entContent = G使ideMana成e本->GetC使本本entContent();
    DisplayContent(C使本本entContent);
}

正oid UMin成Uni正e本sityG使ide基本id成et::Update的a正i成ationB使ttons()
{
    if (!G使ideMana成e本)
    {
        本et使本n;
    }
    
    int32 C使本本entIndex = G使ideMana成e本->GetC使本本entContentIndex();
    int32 TotalCo使nt = G使ideMana成e本->GetTotalContentCo使nt();
    
    // 更新上一步按鈕
    if (P本e正io使sB使tton)
    {
        bool bCanGoP本e正io使s = (C使本本entIndex > 0);
        P本e正io使sB使tton->SetIsEnabled(bCanGoP本e正io使s);
        P本e正io使sB使tton->SetVisibility(bCanGoP本e正io使s 基本 ESlateVisibility::Visible : ESlateVisibility::輸入idden);
    }
    
    // 更新下一步按鈕
    if (的extB使tton)
    {
        bool bIsLastContent = (C使本本entIndex >= TotalCo使nt - 1);
        
        // 更新按鈕文字
        軍Text B使ttonText = bIsLastContent 基本 
            軍Text::軍本o設置St本in成(TEXT("完成 ✓")) : 
            軍Text::軍本o設置St本in成(TEXT("下一步 →"));
        
        if (UTextBlock* B使ttonTextBlock = Cast<UTextBlock>(的extB使tton->GetChildAt(0)))
        {
            B使ttonTextBlock->SetText(B使ttonText);
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的a正i成ation b使ttons 使pdated"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::Show基本id成et()
{
    SetVisibility(ESlateVisibility::Visible);
    
    // 播放進入動畫
    if (Ente本Ani設置ation)
    {
        PlayAni設置ation(Ente本Ani設置ation);
    }
    
    // 更新顯示
    UpdateContentDisplay();
    UpdateP本o成本essBa本();
    Update的a正i成ationB使ttons();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本id成et shown"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::輸入ide基本id成et()
{
    // 播放退動畫
    if (ExitAni設置ation)
    {
        PlayAni設置ation(ExitAni設置ation);
        
        // 動畫完成後隱藏
        軍Ti設置e本輸入andle 輸入ideTi設置e本輸入andle;
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(輸入ideTi設置e本輸入andle, [this]()
        {
            SetVisibility(ESlateVisibility::輸入idden);
        }, ExitAni設置ation->GetEndTi設置e(), false);
    }
    else
    {
        SetVisibility(ESlateVisibility::輸入idden);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本id成et hidden"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::OnG使ideContentUpdated(int32 ContentIndex, const 軍St本in成& ContentTitle)
{
    // 更新顯示
    UpdateContentDisplay();
    UpdateP本o成本essBa本();
    Update的a正i成ationB使ttons();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide content 使pdated: %d - %s"), ContentIndex, *ContentTitle);
}

正oid UMin成Uni正e本sityG使ide基本id成et::OnG使ideCo設置pleted()
{
    // 顯示完成界面
    if (TitleText)
    {
        TitleText->SetText(軍Text::軍本o設置St本in成(TEXT("引導完成！")));
    }
    
    if (Desc本iptionText)
    {
        Desc本iptionText->SetText(軍Text::軍本o設置St本in成(TEXT("恭喜您完成所有引導內容！現在您可以開始您的民國策略之旅了。")));
    }
    
    // 隱藏導航按鈕，只顯示關閉按鈕
    if (P本e正io使sB使tton)
    {
        P本e正io使sB使tton->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    if (的extB使tton)
    {
        的extB使tton->SetVisibility(ESlateVisibility::輸入idden);
    }
    
    if (CloseB使tton)
    {
        CloseB使tton->SetVisibility(ESlateVisibility::Visible);
    }
    
    // 播放完成動畫
    if (Co設置pleteAni設置ation)
    {
        PlayAni設置ation(Co設置pleteAni設置ation);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide co設置pleted"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::OnG使ideStopped()
{
    輸入ide基本id成et();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("G使ide stopped"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::On的extB使ttonClicked()
{
    if (G使ideMana成e本)
    {
        G使ideMana成e本->的a正i成ateTo的extContent();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("的ext b使tton clicked"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::OnP本e正io使sB使ttonClicked()
{
    if (G使ideMana成e本)
    {
        G使ideMana成e本->的a正i成ateToP本e正io使sContent();
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本e正io使s b使tton clicked"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::OnCloseB使ttonClicked()
{
    if (G使ideMana成e本)
    {
        G使ideMana成e本->StopUni正e本sityG使ide();
    }
    
    輸入ide基本id成et();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Close b使tton clicked"));
}

正oid UMin成Uni正e本sityG使ide基本id成et::PlayEnte本Ani設置ation()
{
    if (Ente本Ani設置ation)
    {
        PlayAni設置ation(Ente本Ani設置ation);
    }
}

正oid UMin成Uni正e本sityG使ide基本id成et::PlayExitAni設置ation()
{
    if (ExitAni設置ation)
    {
        PlayAni設置ation(ExitAni設置ation);
    }
}

正oid UMin成Uni正e本sityG使ide基本id成et::PlayContentUpdateAni設置ation()
{
    if (ContentUpdateAni設置ation)
    {
        PlayAni設置ation(ContentUpdateAni設置ation);
    }
}

正oid UMin成Uni正e本sityG使ide基本id成et::PlayCo設置pleteAni設置ation()
{
    if (Co設置pleteAni設置ation)
    {
        PlayAni設置ation(Co設置pleteAni設置ation);
    }
}
