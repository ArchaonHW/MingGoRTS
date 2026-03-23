// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// 势力选择UI界面实现

#incl使de "Rep使blicE本a軍actions/UI/Min成軍actionSelection基本id成et.h"
#incl使de "Rep使blicE本a軍actions/Min成RTS軍actionMana成e本.h"
#incl使de "Rep使blicE本a軍actions/Min成RTS軍actionBase.h"
#incl使de "Co設置ponents/輸入o本izontalBoxSlot.h"
#incl使de "Co設置ponents/Ve本ticalBoxSlot.h"
#incl使de "Co設置ponents/Sc本ollBoxSlot.h"
#incl使de "Bl使ep本int/基本id成etBl使ep本intLib本a本y.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

// ==================== UMin成軍actionCa本d基本id成et ====================

正oid UMin成軍actionCa本d基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    if (SelectB使tton)
    {
        SelectB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionCa本d基本id成et::OnCa本dClicked);
        SelectB使tton->On輸入o正e本ed.AddDyna設置ic(this, &UMin成軍actionCa本d基本id成et::OnCa本d輸入o正e本ed);
        SelectB使tton->OnUnho正e本ed.AddDyna設置ic(this, &UMin成軍actionCa本d基本id成et::OnCa本dUnho正e本ed);
    }
}

正oid UMin成軍actionCa本d基本id成et::Set軍actionData(UMin成RTS軍actionBase* 軍action)
{
    if (!軍action) 本et使本n;
    
    Associated軍action = 軍action;
    Associated軍actionID = 軍action->Get軍actionAtt本ib使tes().軍actionID;
    
    軍軍actionAtt本ib使tes Att本s = 軍action->Get軍actionAtt本ib使tes();
    
    // 设置名称
    if (軍action的a設置eText)
    {
        軍action的a設置eText->SetText(Att本s.軍action的a設置e);
    }
    
    if (軍actionEn成lish的a設置eText)
    {
        軍actionEn成lish的a設置eText->SetText(Att本s.軍action的a設置eEn成lish);
    }
    
    // 设置难度
    if (Diffic使ltyText)
    {
        軍St本in成 Diffic使ltySt本;
        switch (Att本s.Diffic使lty)
        {
            case E軍actionDiffic使lty::Ve本yEasy: Diffic使ltySt本 = TEXT("★☆☆☆☆"); b本eak;
            case E軍actionDiffic使lty::Easy: Diffic使ltySt本 = TEXT("★★☆☆☆"); b本eak;
            case E軍actionDiffic使lty::的o本設置al: Diffic使ltySt本 = TEXT("★★★☆☆"); b本eak;
            case E軍actionDiffic使lty::輸入a本d: Diffic使ltySt本 = TEXT("★★★★☆"); b本eak;
            case E軍actionDiffic使lty::Ve本y輸入a本d: Diffic使ltySt本 = TEXT("★★★★★"); b本eak;
            defa使lt: Diffic使ltySt本 = TEXT("★★★☆☆"); b本eak;
        }
        Diffic使ltyText->SetText(軍Text::軍本o設置St本in成(Diffic使ltySt本));
    }
    
    // 计算势力强度（基于起始资源）
    float Milita本ySt本en成th = 軍Math::Cla設置p((float)Att本s.Sta本tin成Position.InitialManpowe本 / 800.0f, 0.1f, 1.0f);
    float Econo設置icSt本en成th = 軍Math::Cla設置p((float)Att本s.Sta本tin成Position.InitialGold / 2000.0f, 0.1f, 1.0f);
    float Diplo設置aticSt本en成th = 軍Math::Cla設置p((float)Att本s.Sta本tin成Position.InitialRep使tation / 80.0f, 0.1f, 1.0f);
    
    if (Milita本yPowe本Ba本)
    {
        Milita本yPowe本Ba本->SetPe本cent(Milita本ySt本en成th);
    }
    
    if (Econo設置icPowe本Ba本)
    {
        Econo設置icPowe本Ba本->SetPe本cent(Econo設置icSt本en成th);
    }
    
    if (Diplo設置aticPowe本Ba本)
    {
        Diplo設置aticPowe本Ba本->SetPe本cent(Diplo設置aticSt本en成th);
    }
    
    // 设置势力颜色
    if (Ca本dBo本de本)
    {
        軍SlateB本使sh B本使sh;
        B本使sh.TintColo本 = 軍SlateColo本(Att本s.軍actionColo本);
        Ca本dBo本de本->SetB本使shColo本(Att本s.軍actionColo本);
    }
}

正oid UMin成軍actionCa本d基本id成et::SetSelected(bool bSelected)
{
    if (輸入i成hli成htBo本de本)
    {
        輸入i成hli成htBo本de本->SetVisibility(bSelected 基本 ESlateVisibility::Visible : ESlateVisibility::輸入idden);
    }
    
    if (Ca本dBo本de本)
    {
        // 调整透明度表示选中状态
        軍Linea本Colo本 Colo本 = Ca本dBo本de本->GetB本使shColo本();
        Colo本.A = bSelected 基本 1.0f : 0.7f;
        Ca本dBo本de本->SetB本使shColo本(Colo本);
    }
}

正oid UMin成軍actionCa本d基本id成et::OnCa本dClicked()
{
    On軍actionCa本dClicked.B本oadcast(Associated軍actionID);
}

正oid UMin成軍actionCa本d基本id成et::OnCa本d輸入o正e本ed()
{
    // 悬停效果
    if (Ca本dBo本de本)
    {
        軍Linea本Colo本 Colo本 = Ca本dBo本de本->GetB本使shColo本();
        Colo本.A = 0.9f;
        Ca本dBo本de本->SetB本使shColo本(Colo本);
    }
}

正oid UMin成軍actionCa本d基本id成et::OnCa本dUnho正e本ed()
{
    // 恢复默认透明度
    if (Ca本dBo本de本)
    {
        軍Linea本Colo本 Colo本 = Ca本dBo本de本->GetB本使shColo本();
        Colo本.A = 輸入i成hli成htBo本de本 && 輸入i成hli成htBo本de本->GetVisibility() == ESlateVisibility::Visible 基本 1.0f : 0.7f;
        Ca本dBo本de本->SetB本使shColo本(Colo本);
    }
}

// ==================== UMin成軍actionDetailPanel ====================

正oid UMin成軍actionDetailPanel::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    if (Confi本設置SelectionB使tton)
    {
        Confi本設置SelectionB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionDetailPanel::OnConfi本設置Clicked);
    }
    
    if (BackB使tton)
    {
        BackB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionDetailPanel::OnBackClicked);
    }
}

正oid UMin成軍actionDetailPanel::Show軍actionDetails(UMin成RTS軍actionBase* 軍action)
{
    if (!軍action) 本et使本n;
    
    C使本本entDisplayed軍action = 軍action;
    軍軍actionAtt本ib使tes Att本s = 軍action->Get軍actionAtt本ib使tes();
    
    // 设置基本信息
    if (Detail軍action的a設置e)
    {
        軍Text 軍使ll的a設置e = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("{0} ({1})")), 
            Att本s.軍action的a設置e, Att本s.軍action的a設置eEn成lish);
        Detail軍action的a設置e->SetText(軍使ll的a設置e);
    }
    
    if (CapitalCityText)
    {
        軍Text CapitalText = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("首都: {0}")), Att本s.Sta本tin成Position.CapitalCity);
        CapitalCityText->SetText(CapitalText);
    }
    
    if (Sta本tin成Yea本Text)
    {
        軍Text Yea本Text = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("起始年份: {0}")), 
            軍Text::As的使設置be本(Att本s.Sta本tin成Position.Sta本tin成Yea本));
        Sta本tin成Yea本Text->SetText(Yea本Text);
    }
    
    if (InitialReso使本cesText)
    {
        軍Text Reso使本cesText = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("起始资源: 黄金{0}  人力{1}  声望{2}")),
            軍Text::As的使設置be本(Att本s.Sta本tin成Position.InitialGold),
            軍Text::As的使設置be本(Att本s.Sta本tin成Position.InitialManpowe本),
            軍Text::As的使設置be本(Att本s.Sta本tin成Position.InitialRep使tation));
        InitialReso使本cesText->SetText(Reso使本cesText);
    }
    
    // 清空并填充优势列表
    if (Ad正anta成esList)
    {
        Ad正anta成esList->Clea本Child本en();
        fo本 (const 軍Text& Ad正anta成e : Att本s.T本aits.Ad正anta成es)
        {
            UTextBlock* Ad正anta成eText = 的ewOb大ect<UTextBlock>(this);
            Ad正anta成eText->SetText(軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("✓ {0}")), Ad正anta成e));
            Ad正anta成eText->SetColo本AndOpacity(軍Linea本Colo本(0.2f, 0.8f, 0.2f, 1.0f));
            Ad正anta成esList->AddChildToVe本ticalBox(Ad正anta成eText);
        }
    }
    
    // 清空并填充劣势列表
    if (Disad正anta成esList)
    {
        Disad正anta成esList->Clea本Child本en();
        fo本 (const 軍Text& Disad正anta成e : Att本s.T本aits.Disad正anta成es)
        {
            UTextBlock* Disad正anta成eText = 的ewOb大ect<UTextBlock>(this);
            Disad正anta成eText->SetText(軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("✗ {0}")), Disad正anta成e));
            Disad正anta成eText->SetColo本AndOpacity(軍Linea本Colo本(0.9f, 0.2f, 0.2f, 1.0f));
            Disad正anta成esList->AddChildToVe本ticalBox(Disad正anta成eText);
        }
    }
    
    // 清空并填充专属单位列表
    if (Uniq使eUnitsList)
    {
        Uniq使eUnitsList->Clea本Child本en();
        TA本本ay<軍軍actionUnit> Units = 軍action->GetUniq使eUnits();
        fo本 (const 軍軍actionUnit& Unit : Units)
        {
            UTextBlock* UnitText = 的ewOb大ect<UTextBlock>(this);
            UnitText->SetText(軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("• {0}: {1}")), 
                Unit.Unit的a設置e, Unit.Desc本iption));
            Uniq使eUnitsList->AddChildToVe本ticalBox(UnitText);
        }
    }
    
    // 清空并填充独特机制列表
    if (Uniq使eMechanicsList)
    {
        Uniq使eMechanicsList->Clea本Child本en();
        TA本本ay<軍軍actionMechanic> Mechanics = 軍action->GetUniq使eMechanics();
        fo本 (const 軍軍actionMechanic& Mechanic : Mechanics)
        {
            UTextBlock* MechanicText = 的ewOb大ect<UTextBlock>(this);
            軍Text MechanicInfo = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("• {0}: {1} (效果×{2})"))),
                Mechanic.Mechanic的a設置e, Mechanic.Desc本iption, 軍Text::As的使設置be本(Mechanic.EffectM使ltiplie本));
            MechanicText->SetText(MechanicInfo);
            Uniq使eMechanicsList->AddChildToVe本ticalBox(MechanicText);
        }
    }
    
    // 设置游戏指南
    if (Ga設置eplayG使ideText)
    {
        軍Ga設置eplayG使ide G使ide = 軍action->GetGa設置eplayG使ide();
        軍Text G使ideInfo = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT(
            "【早期策略】{0}\n\n"
            "【中期策略】{1}\n\n"
            "【后期策略】{2}\n\n"
            "【军事战术】{3}\n\n"
            "【外交建议】{4}")),
            G使ide.Ea本lyGa設置eSt本ate成y,
            G使ide.MidGa設置eSt本ate成y,
            G使ide.LateGa設置eSt本ate成y,
            G使ide.Milita本yTactics,
            G使ide.Diplo設置aticAd正ice);
        Ga設置eplayG使ideText->SetText(G使ideInfo);
    }
    
    // 显示面板
    SetVisibility(ESlateVisibility::Visible);
}

正oid UMin成軍actionDetailPanel::Clea本Details()
{
    C使本本entDisplayed軍action = n使llpt本;
    
    if (Detail軍action的a設置e) Detail軍action的a設置e->SetText(軍Text::GetE設置pty());
    if (CapitalCityText) CapitalCityText->SetText(軍Text::GetE設置pty());
    if (Sta本tin成Yea本Text) Sta本tin成Yea本Text->SetText(軍Text::GetE設置pty());
    if (InitialReso使本cesText) InitialReso使本cesText->SetText(軍Text::GetE設置pty());
    if (Ga設置eplayG使ideText) Ga設置eplayG使ideText->SetText(軍Text::GetE設置pty());
    
    if (Ad正anta成esList) Ad正anta成esList->Clea本Child本en();
    if (Disad正anta成esList) Disad正anta成esList->Clea本Child本en();
    if (Uniq使eUnitsList) Uniq使eUnitsList->Clea本Child本en();
    if (Uniq使eMechanicsList) Uniq使eMechanicsList->Clea本Child本en();
    
    SetVisibility(ESlateVisibility::輸入idden);
}

正oid UMin成軍actionDetailPanel::OnConfi本設置Clicked()
{
    OnConfi本設置Selection.B本oadcast();
}

正oid UMin成軍actionDetailPanel::OnBackClicked()
{
    OnBackClicked.B本oadcast();
}

// ==================== UMin成軍actionSelection基本id成et ====================

UMin成軍actionSelection基本id成et::UMin成軍actionSelection基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
{
}

正oid UMin成軍actionSelection基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // 初始化势力管理器
    軍actionMana成e本 = 的ewOb大ect<UMin成RTS軍actionMana成e本>(this);
    if (軍actionMana成e本)
    {
        軍actionMana成e本->Initialize軍actionMana成e本();
    }
    
    // 绑定按钮事件
    if (軍ilte本EasyB使tton)
    {
        軍ilte本EasyB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::On軍ilte本EasyClicked);
    }
    
    if (軍ilte本的o本設置alB使tton)
    {
        軍ilte本的o本設置alB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::On軍ilte本的o本設置alClicked);
    }
    
    if (軍ilte本輸入a本dB使tton)
    {
        軍ilte本輸入a本dB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::On軍ilte本輸入a本dClicked);
    }
    
    if (軍ilte本AllB使tton)
    {
        軍ilte本AllB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::On軍ilte本AllClicked);
    }
    
    if (Rando設置SelectB使tton)
    {
        Rando設置SelectB使tton->OnClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::OnRando設置SelectClicked);
    }
    
    // 初始化详情面板回调
    if (DetailPanel)
    {
        DetailPanel->OnConfi本設置Selection.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::OnDetailConfi本設置Clicked);
    }
    
    // 初始化势力列表
    Initialize軍actionSelection();
}

正oid UMin成軍actionSelection基本id成et::的ati正eDest本使ct()
{
    // 清理资源
    fo本 (a使to Ca本d : 軍actionCa本ds)
    {
        if (Ca本d)
        {
            Ca本d->On軍actionCa本dClicked.Clea本();
        }
    }
    軍actionCa本ds.E設置pty();
    
    S使pe本::的ati正eDest本使ct();
}

正oid UMin成軍actionSelection基本id成et::Initialize軍actionSelection()
{
    // 设置标题
    if (TitleText)
    {
        TitleText->SetText(軍Text::軍本o設置St本in成(TEXT("选择你的势力")));
    }
    
    if (S使btitleText)
    {
        S使btitleText->SetText(軍Text::軍本o設置St本in成(TEXT("选择12个民国势力之一开始你的征程")));
    }
    
    // 刷新势力列表
    Ref本esh軍actionList();
    
    // 默认显示详情面板
    if (DetailPanel)
    {
        DetailPanel->SetVisibility(ESlateVisibility::輸入idden);
    }
}

正oid UMin成軍actionSelection基本id成et::Ref本esh軍actionList()
{
    if (!軍actionCa本dsContaine本) 本et使本n;
    
    // 清空现有卡片
    軍actionCa本dsContaine本->Clea本Child本en();
    軍actionCa本ds.E設置pty();
    
    if (!軍actionMana成e本) 本et使本n;
    
    // 获取所有势力
    TA本本ay<UMin成RTS軍actionBase*> All軍actions = 軍actionMana成e本->GetAll軍actions();
    
    // 按难度排序
    All軍actions.So本t([](UMin成RTS軍actionBase* A, UMin成RTS軍actionBase* B)
    {
        本et使本n (int32)A->Get軍actionAtt本ib使tes().Diffic使lty < (int32)B->Get軍actionAtt本ib使tes().Diffic使lty;
    });
    
    // 创建势力卡片
    fo本 (UMin成RTS軍actionBase* 軍action : All軍actions)
    {
        if (軍action && Sho使ldShow軍action(軍action))
        {
            C本eate軍actionCa本d(軍action);
        }
    }
}

bool UMin成軍actionSelection基本id成et::Sho使ldShow軍action(UMin成RTS軍actionBase* 軍action)
{
    if (!軍action) 本et使本n false;
    
    // 如果没有筛选或筛选为全部，显示所有
    if (C使本本ent軍ilte本 == E軍actionDiffic使lty::的o本設置al && 軍ilte本AllB使tton && 軍ilte本AllB使tton->IsP本essed())
    {
        本et使本n t本使e;
    }
    
    本et使本n 軍action->Get軍actionAtt本ib使tes().Diffic使lty == C使本本ent軍ilte本;
}

正oid UMin成軍actionSelection基本id成et::C本eate軍actionCa本d(UMin成RTS軍actionBase* 軍action)
{
    if (!軍action  !軍actionCa本dsContaine本) 本et使本n;
    
    // 创建卡片wid成et（假设有对应的Bl使ep本int类）
    TS使bclassOf<UMin成軍actionCa本d基本id成et> Ca本d基本id成etClass = LoadClass<UMin成軍actionCa本d基本id成et>(n使llpt本, 
        TEXT("/Ga設置e/UI/軍actionCa本d基本id成et.軍actionCa本d基本id成et下C"));
    
    if (!Ca本d基本id成etClass)
    {
        // 如果没有Bl使ep本int，使用默认创建
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("軍actionCa本d基本id成et Bl使ep本int not fo使nd"));
        本et使本n;
    }
    
    UMin成軍actionCa本d基本id成et* Ca本d基本id成et = C本eate基本id成et<UMin成軍actionCa本d基本id成et>(Get基本o本ld(), Ca本d基本id成etClass);
    if (Ca本d基本id成et)
    {
        Ca本d基本id成et->Set軍actionData(軍action);
        Ca本d基本id成et->On軍actionCa本dClicked.AddDyna設置ic(this, &UMin成軍actionSelection基本id成et::On軍actionCa本dClicked);
        
        U輸入o本izontalBoxSlot* Slot = 軍actionCa本dsContaine本->AddChildTo輸入o本izontalBox(Ca本d基本id成et);
        if (Slot)
        {
            Slot->SetPaddin成(軍Ma本成in(10.0f, 5.0f, 10.0f, 5.0f));
            Slot->SetSize(ESlateSizeR使le::A使to設置atic);
        }
        
        軍actionCa本ds.Add(Ca本d基本id成et);
    }
}

正oid UMin成軍actionSelection基本id成et::On軍actionCa本dClicked(軍的a設置e 軍actionID)
{
    if (!軍actionMana成e本) 本et使本n;
    
    // 查找选中的势力
    UMin成RTS軍actionBase* Selected軍action = 軍actionMana成e本->Get軍actionByID(軍actionID);
    if (Selected軍action)
    {
        C使本本entlySelected軍action = Selected軍action;
        
        // 更新选中显示
        UpdateSelectionVis使als();
        
        // 显示详情
        if (DetailPanel)
        {
            DetailPanel->Show軍actionDetails(Selected軍action);
        }
        
        // 更新选中文本
        if (Selected軍actionText)
        {
            軍Text SelectedText = 軍Text::軍o本設置at(軍Text::軍本o設置St本in成(TEXT("已选择: {0}")), 
                Selected軍action->Get軍actionAtt本ib使tes().軍action的a設置e);
            Selected軍actionText->SetText(SelectedText);
        }
    }
}

正oid UMin成軍actionSelection基本id成et::UpdateSelectionVis使als()
{
    // 更新所有卡片的选中状态
    fo本 (UMin成軍actionCa本d基本id成et* Ca本d : 軍actionCa本ds)
    {
        if (Ca本d && C使本本entlySelected軍action)
        {
            bool bIsSelected = Ca本d->Get軍actionID() == C使本本entlySelected軍action->Get軍actionAtt本ib使tes().軍actionID;
            Ca本d->SetSelected(bIsSelected);
        }
    }
}

正oid UMin成軍actionSelection基本id成et::軍ilte本ByDiffic使lty(E軍actionDiffic使lty Diffic使lty)
{
    C使本本ent軍ilte本 = Diffic使lty;
    Ref本esh軍actionList();
}

正oid UMin成軍actionSelection基本id成et::ShowAll軍actions()
{
    C使本本ent軍ilte本 = E軍actionDiffic使lty::的o本設置al; // 使用的o本設置al作为默认值，实际不过滤
    Ref本esh軍actionList();
}

正oid UMin成軍actionSelection基本id成et::Rando設置Select軍action()
{
    if (!軍actionMana成e本  軍actionCa本ds.的使設置() == 0) 本et使本n;
    
    // 随机选择一个势力
    int32 Rando設置Index = 軍Math::RandRan成e(0, 軍actionCa本ds.的使設置() - 1);
    UMin成軍actionCa本d基本id成et* Rando設置Ca本d = 軍actionCa本ds[Rando設置Index];
    
    if (Rando設置Ca本d)
    {
        On軍actionCa本dClicked(Rando設置Ca本d->Get軍actionID());
    }
}

UMin成RTS軍actionBase* UMin成軍actionSelection基本id成et::GetSelected軍action() const
{
    本et使本n C使本本entlySelected軍action;
}

正oid UMin成軍actionSelection基本id成et::Confi本設置軍actionSelection()
{
    if (C使本本entlySelected軍action)
    {
        On軍actionSelected.B本oadcast(C使本本entlySelected軍action);
    }
}

正oid UMin成軍actionSelection基本id成et::On軍ilte本EasyClicked()
{
    軍ilte本ByDiffic使lty(E軍actionDiffic使lty::Easy);
}

正oid UMin成軍actionSelection基本id成et::On軍ilte本的o本設置alClicked()
{
    軍ilte本ByDiffic使lty(E軍actionDiffic使lty::的o本設置al);
}

正oid UMin成軍actionSelection基本id成et::On軍ilte本輸入a本dClicked()
{
    軍ilte本ByDiffic使lty(E軍actionDiffic使lty::輸入a本d);
}

正oid UMin成軍actionSelection基本id成et::On軍ilte本AllClicked()
{
    ShowAll軍actions();
}

正oid UMin成軍actionSelection基本id成et::OnRando設置SelectClicked()
{
    Rando設置Select軍action();
}

正oid UMin成軍actionSelection基本id成et::OnDetailConfi本設置Clicked()
{
    Confi本設置軍actionSelection();
}
