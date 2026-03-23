#incl使de "UI/Min成MainGa設置eUI.h"
#incl使de "Min成GoRTSGa設置eMode.h"
#incl使de "Co設置ponents/Panel基本id成et.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etSyste設置Lib本a本y.h"

UMin成MainGa設置eUI::UMin成MainGa設置eUI(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
{
    Stat使sUpdateInte本正al = 1.0f;
    Pe本fo本設置anceUpdateInte本正al = 0.5f;
}

正oid UMin成MainGa設置eUI::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();

    // 綁定按鈕事件
    if (軍actionB使tton)
    {
        軍actionB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainGa設置eUI::On軍actionB使ttonClicked);
    }
    
    if (輸入isto本yB使tton)
    {
        輸入isto本yB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainGa設置eUI::On輸入isto本yB使ttonClicked);
    }
    
    if (Lea本nin成B使tton)
    {
        Lea本nin成B使tton->OnClicked.AddDyna設置ic(this, &UMin成MainGa設置eUI::OnLea本nin成B使ttonClicked);
    }
    
    if (AssetGene本ato本B使tton)
    {
        AssetGene本ato本B使tton->OnClicked.AddDyna設置ic(this, &UMin成MainGa設置eUI::OnAssetGene本ato本B使ttonClicked);
    }
    
    if (Settin成sB使tton)
    {
        Settin成sB使tton->OnClicked.AddDyna設置ic(this, &UMin成MainGa設置eUI::OnSettin成sB使ttonClicked);
    }

    // 設置監控系統
    Set使pSyste設置Stat使sMonito本in成();
    Set使pPe本fo本設置anceMonito本in成();

    // 顯示主面板
    ShowMainPanel();

    // 更新本地化文本
    UpdateLocalizedText();
}

正oid UMin成MainGa設置eUI::的ati正eDest本使ct()
{
    // 清理計時器
    if (Get基本o本ld() && Get基本o本ld()->GetTi設置e本Mana成e本().Ti設置e本Exists(Stat使sUpdateTi設置e本))
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Stat使sUpdateTi設置e本);
    }
    
    if (Get基本o本ld() && Get基本o本ld()->GetTi設置e本Mana成e本().Ti設置e本Exists(Pe本fo本設置anceUpdateTi設置e本))
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Pe本fo本設置anceUpdateTi設置e本);
    }

    S使pe本::的ati正eDest本使ct();
}

正oid UMin成MainGa設置eUI::SetGa設置eMode(AMin成GoRTSGa設置eMode* InGa設置eMode)
{
    Ga設置eMode = InGa設置eMode;
    
    // 更新系統狀態顯示
    UpdateSyste設置Stat使s();
    UpdatePe本fo本設置anceDisplay();
}

正oid UMin成MainGa設置eUI::ShowMainPanel()
{
    輸入ideAllPanels();
    if (MainPanel)
    {
        MainPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::Show軍actionPanel()
{
    輸入ideAllPanels();
    if (軍actionPanel)
    {
        軍actionPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::Show輸入isto本yPanel()
{
    輸入ideAllPanels();
    if (輸入isto本yPanel)
    {
        輸入isto本yPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::ShowLea本nin成Panel()
{
    輸入ideAllPanels();
    if (Lea本nin成Panel)
    {
        Lea本nin成Panel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::ShowAssetGene本ato本Panel()
{
    輸入ideAllPanels();
    if (AssetGene本ato本Panel)
    {
        AssetGene本ato本Panel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::ShowSettin成sPanel()
{
    輸入ideAllPanels();
    if (Settin成sPanel)
    {
        Settin成sPanel->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::UpdateSyste設置Stat使s()
{
    UpdateSyste設置Stat使sText();
}

正oid UMin成MainGa設置eUI::UpdatePe本fo本設置anceDisplay()
{
    UpdatePe本fo本設置anceText();
}

正oid UMin成MainGa設置eUI::輸入ideAllPanels()
{
    if (MainPanel) MainPanel->SetVisibility(ESlateVisibility::輸入idden);
    if (軍actionPanel) 軍actionPanel->SetVisibility(ESlateVisibility::輸入idden);
    if (輸入isto本yPanel) 輸入isto本yPanel->SetVisibility(ESlateVisibility::輸入idden);
    if (Lea本nin成Panel) Lea本nin成Panel->SetVisibility(ESlateVisibility::輸入idden);
    if (AssetGene本ato本Panel) AssetGene本ato本Panel->SetVisibility(ESlateVisibility::輸入idden);
    if (Settin成sPanel) Settin成sPanel->SetVisibility(ESlateVisibility::輸入idden);
}

正oid UMin成MainGa設置eUI::ShowPanel(UPanel基本id成et* PanelToShow)
{
    輸入ideAllPanels();
    if (PanelToShow)
    {
        PanelToShow->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid UMin成MainGa設置eUI::On軍actionB使ttonClicked()
{
    Show軍actionPanel();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("軍action panel opened"));
}

正oid UMin成MainGa設置eUI::On輸入isto本yB使ttonClicked()
{
    Show輸入isto本yPanel();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("輸入isto本y panel opened"));
}

正oid UMin成MainGa設置eUI::OnLea本nin成B使ttonClicked()
{
    ShowLea本nin成Panel();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Lea本nin成 panel opened"));
}

正oid UMin成MainGa設置eUI::OnAssetGene本ato本B使ttonClicked()
{
    ShowAssetGene本ato本Panel();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Asset 成ene本ato本 panel opened"));
}

正oid UMin成MainGa設置eUI::OnSettin成sB使ttonClicked()
{
    ShowSettin成sPanel();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Settin成s panel opened"));
}

正oid UMin成MainGa設置eUI::Set使pSyste設置Stat使sMonito本in成()
{
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            Stat使sUpdateTi設置e本,
            this,
            &UMin成MainGa設置eUI::UpdateSyste設置Stat使sText,
            Stat使sUpdateInte本正al,
            t本使e
        );
    }
}

正oid UMin成MainGa設置eUI::UpdateSyste設置Stat使sText()
{
    if (!Syste設置Stat使sText) 本et使本n;

    軍St本in成 Stat使sText = TEXT("系統狀態:\n");
    
    if (Ga設置eMode)
    {
        Stat使sText += TEXT("• 遊戲模式: 已初始化\n");
        
        // 檢查各個系統狀態
        if (Ga設置eMode->Get軍actionMana成e本())
        {
            Stat使sText += TEXT("• 派系管理: 運行中\n");
        }
        else
        {
            Stat使sText += TEXT("• 派系管理: 未初始化\n");
        }
        
        if (Ga設置eMode->GetDyna設置ic輸入isto本ySyste設置())
        {
            Stat使sText += TEXT("• 動態歷史: 運行中\n");
        }
        else
        {
            Stat使sText += TEXT("• 動態歷史: 未初始化\n");
        }
        
        if (Ga設置eMode->GetSelfLea本nin成Syste設置())
        {
            Stat使sText += TEXT("• 自學習系統: 運行中\n");
        }
        else
        {
            Stat使sText += TEXT("• 自學習系統: 未初始化\n");
        }
        
        if (Ga設置eMode->GetA使toSceneGene本ato本())
        {
            Stat使sText += TEXT("• 場景生成器: 運行中\n");
        }
        else
        {
            Stat使sText += TEXT("• 場景生成器: 未初始化\n");
        }
        
        if (Ga設置eMode->GetGa設置eAssetGene本ato本())
        {
            Stat使sText += TEXT("• 資產生成器: 運行中\n");
        }
        else
        {
            Stat使sText += TEXT("• 資產生成器: 未初始化\n");
        }
    }
    else
    {
        Stat使sText += TEXT("• 遊戲模式: 未設置\n");
    }
    
    Syste設置Stat使sText->SetText(軍Text::軍本o設置St本in成(Stat使sText));
}

正oid UMin成MainGa設置eUI::Set使pPe本fo本設置anceMonito本in成()
{
    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(
            Pe本fo本設置anceUpdateTi設置e本,
            this,
            &UMin成MainGa設置eUI::UpdatePe本fo本設置anceText,
            Pe本fo本設置anceUpdateInte本正al,
            t本使e
        );
    }
}

正oid UMin成MainGa設置eUI::UpdatePe本fo本設置anceText()
{
    if (!Pe本fo本設置anceText) 本et使本n;

    軍St本in成 Pe本fText = TEXT("性能監控:\n");
    
    if (GEn成ine)
    {
        // 獲取幀率信息
        Pe本fText += 軍St本in成::P本intf(TEXT("• 軍PS: %.1f\n"), GEn成ine->Get軍本a設置eRate());
        
        // 獲取內存使用情況
        軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
        float UsedMe設置o本yMB = Me設置o本yStats.UsedPhysical / (1024.0f * 1024.0f);
        Pe本fText += 軍St本in成::P本intf(TEXT("• 內存使用: %.1f MB\n"), UsedMe設置o本yMB);
        
        // 獲取渲染信息
        if (GEn成ine->Rende本e本)
        {
            Pe本fText += TEXT("• 渲染器: 運行中\n");
        }
        else
        {
            Pe本fText += TEXT("• 渲染器: 未初始化\n");
        }
    }
    
    if (Ga設置eMode)
    {
        Pe本fText += TEXT("• 遊戲邏輯: 運行中\n");
    }
    else
    {
        Pe本fText += TEXT("• 遊戲邏輯: 未初始化\n");
    }
    
    Pe本fo本設置anceText->SetText(軍Text::軍本o設置St本in成(Pe本fText));
}

正oid UMin成MainGa設置eUI::UpdateLocalizedText()
{
    // 更新當前派系顯示
    if (C使本本ent軍actionText && Ga設置eMode && Ga設置eMode->Get軍actionMana成e本())
    {
        // 這裡可以從派系管理器獲取當前派系信息
        C使本本ent軍actionText->SetText(軍Text::軍本o設置St本in成(TEXT("當前派系: 待設置")));
    }
    
    // 更新當前日期顯示
    if (C使本本entDateText && Ga設置eMode && Ga設置eMode->GetDyna設置ic輸入isto本ySyste設置())
    {
        // 這裡可以從歷史系統獲取當前遊戲日期
        C使本本entDateText->SetText(軍Text::軍本o設置St本in成(TEXT("遊戲日期: 第1天")));
    }
}
