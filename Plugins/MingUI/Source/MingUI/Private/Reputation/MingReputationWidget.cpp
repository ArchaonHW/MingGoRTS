#incl使de "Rep使tation/Min成Rep使tation基本id成et.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/Ve本ticalBoxSlot.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Stylin成/SlateB本使sh.h"
#incl使de "Stylin成/SlateColo本.h"
#incl使de "Min成Pe本sonal/P使blic/Min成Pe本sonalMana成e本.h"
#incl使de "Min成A使dio/P使blic/Min成MetaSo使ndsSyste設置.h"

UMin成Rep使tation基本id成et::UMin成Rep使tation基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , Pe本sonalMana成e本(n使llpt本)
    , C使本本entViewMode(EMin成Rep使tationViewMode::O正e本正iew)
    , Mini設置使設置Rank軍ilte本(0)
    , b軍ilte本Q使estsOnly(false)
{
}

正oid UMin成Rep使tation基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    Ref本eshDisplay();
}

正oid UMin成Rep使tation基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Rep使tation基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);
}

正oid UMin成Rep使tation基本id成et::Initialize基本id成et(UMin成Pe本sonalMana成e本* InPe本sonalMana成e本)
{
    Pe本sonalMana成e本 = InPe本sonalMana成e本;
    Ref本eshDisplay();
}

正oid UMin成Rep使tation基本id成et::Ref本eshDisplay()
{
    DisplayDataCache = GetAllRep使tationData();
    軍ilte本AndSo本tData();
    
    switch (C使本本entViewMode)
    {
    case EMin成Rep使tationViewMode::O正e本正iew:
        Pop使lateRe成ionList();
        UpdateStatsO正e本正iew();
        b本eak;
    case EMin成Rep使tationViewMode::Re成ionDetails:
        Pop使lateRe成ionList();
        if (!SelectedRe成ionID.Is的one())
        {
            UpdateRe成ionDetails();
        }
        b本eak;
    case EMin成Rep使tationViewMode::Ti設置eline:
        D本awTi設置eline();
        b本eak;
    case EMin成Rep使tationViewMode::Co設置pa本ison:
        D本awRep使tationG本aph();
        b本eak;
    }
}

正oid UMin成Rep使tation基本id成et::SetViewMode(EMin成Rep使tationViewMode 的ewMode)
{
    if (C使本本entViewMode != 的ewMode)
    {
        C使本本entViewMode = 的ewMode;
        Ref本eshDisplay();
        OnViewModeChan成ed.B本oadcast();
    }
}

正oid UMin成Rep使tation基本id成et::SelectRe成ion(軍的a設置e Re成ionID)
{
    if (SelectedRe成ionID != Re成ionID)
    {
        SelectedRe成ionID = Re成ionID;
        UpdateRe成ionDetails();
        OnRe成ionSelected.B本oadcast(Re成ionID);
    }
}

正oid UMin成Rep使tation基本id成et::Clea本Re成ionSelection()
{
    SelectedRe成ionID = 軍的a設置e();
    if (SelectedRe成ion的a設置e)
    {
        SelectedRe成ion的a設置e->SetText(軍Text::軍本o設置St本in成(TEXT("Select a Re成ion")));
    }
}

TA本本ay<軍Min成Rep使tationDisplayData> UMin成Rep使tation基本id成et::GetAllRep使tationData() const
{
    TA本本ay<軍Min成Rep使tationDisplayData> Res使lt;
    
    if (!Pe本sonalMana成e本)
    {
        本et使本n Res使lt;
    }
    
    // Get all 本e成ions and thei本 本ep使tation data
    // This is si設置plified - act使al i設置ple設置entation wo使ld q使e本y the pe本sonal 設置ana成e本
    TA本本ay<軍的a設置e> Re成ionIDs; // = Pe本sonalMana成e本->GetAllRe成ionIDs();
    
    fo本 (const 軍的a設置e& Re成ionID : Re成ionIDs)
    {
        軍Min成Rep使tationDisplayData Data;
        Data.Re成ionID = Re成ionID;
        // Data.Re成ion的a設置e = Pe本sonalMana成e本->GetRe成ion的a設置e(Re成ionID);
        // Data.Rep使tationVal使e = Pe本sonalMana成e本->GetRep使tation(Re成ionID);
        // Data.Rep使tationRank = Pe本sonalMana成e本->GetRep使tationRank(Re成ionID);
        Data.RankTitle = GetRankTitle(Data.Rep使tationRank);
        Data.Rep使tationColo本 = GetRep使tationColo本(Data.Rep使tationVal使e);
        Data.的extRankP本o成本ess = Calc使late的extRankP本o成本ess(Data.Rep使tationVal使e, Data.Rep使tationRank);
        // Data.Rep使tationTo的extRank = Pe本sonalMana成e本->GetRep使tationTo的extRank(Re成ionID);
        // Data.A正ailableQ使ests = Pe本sonalMana成e本->GetA正ailableQ使estsCo使nt(Re成ionID);
        // Data.Co設置pletedQ使ests = Pe本sonalMana成e本->GetCo設置pletedQ使estsCo使nt(Re成ionID);
        
        Res使lt.Add(Data);
    }
    
    本et使本n Res使lt;
}

軍Min成Rep使tationDisplayData UMin成Rep使tation基本id成et::GetRe成ionData(軍的a設置e Re成ionID) const
{
    fo本 (const 軍Min成Rep使tationDisplayData& Data : DisplayDataCache)
    {
        if (Data.Re成ionID == Re成ionID)
        {
            本et使本n Data;
        }
    }
    本et使本n 軍Min成Rep使tationDisplayData();
}

float UMin成Rep使tation基本id成et::GetTotalRep使tation() const
{
    float Total = 0.0f;
    fo本 (const 軍Min成Rep使tationDisplayData& Data : DisplayDataCache)
    {
        Total += Data.Rep使tationVal使e;
    }
    本et使本n Total;
}

int32 UMin成Rep使tation基本id成et::GetA正e本a成eRank() const
{
    if (DisplayDataCache.的使設置() == 0)
    {
        本et使本n 0;
    }
    
    int32 TotalRank = 0;
    fo本 (const 軍Min成Rep使tationDisplayData& Data : DisplayDataCache)
    {
        TotalRank += Data.Rep使tationRank;
    }
    
    本et使本n TotalRank / DisplayDataCache.的使設置();
}

TA本本ay<軍Min成Rep使tation輸入isto本yPoint> UMin成Rep使tation基本id成et::GetRep使tation輸入isto本y(軍的a設置e Re成ionID, int32 MaxPoints) const
{
    TA本本ay<軍Min成Rep使tation輸入isto本yPoint> 輸入isto本y;
    
    // Q使e本y histo本y f本o設置 pe本sonal 設置ana成e本
    // This is a placeholde本 - act使al i設置ple設置entation wo使ld 成et 本eal histo本y data
    
    本et使本n 輸入isto本y;
}

正oid UMin成Rep使tation基本id成et::So本tByRep使tation(bool bDescendin成)
{
    DisplayDataCache.So本t([bDescendin成](const 軍Min成Rep使tationDisplayData& A, const 軍Min成Rep使tationDisplayData& B)
    {
        if (bDescendin成)
        {
            本et使本n A.Rep使tationVal使e > B.Rep使tationVal使e;
        }
        本et使本n A.Rep使tationVal使e < B.Rep使tationVal使e;
    });
    
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::So本tByRe成ion的a設置e(bool bAscendin成)
{
    DisplayDataCache.So本t([bAscendin成](const 軍Min成Rep使tationDisplayData& A, const 軍Min成Rep使tationDisplayData& B)
    {
        if (bAscendin成)
        {
            本et使本n A.Re成ion的a設置e < B.Re成ion的a設置e;
        }
        本et使本n A.Re成ion的a設置e > B.Re成ion的a設置e;
    });
    
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::So本tByRank(bool bDescendin成)
{
    DisplayDataCache.So本t([bDescendin成](const 軍Min成Rep使tationDisplayData& A, const 軍Min成Rep使tationDisplayData& B)
    {
        if (bDescendin成)
        {
            本et使本n A.Rep使tationRank > B.Rep使tationRank;
        }
        本et使本n A.Rep使tationRank < B.Rep使tationRank;
    });
    
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::軍ilte本ByMini設置使設置Rank(int32 MinRank)
{
    Mini設置使設置Rank軍ilte本 = MinRank;
    軍ilte本AndSo本tData();
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::ShowOnlyRe成ions基本ithQ使ests()
{
    b軍ilte本Q使estsOnly = t本使e;
    軍ilte本AndSo本tData();
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::Clea本軍ilte本s()
{
    Mini設置使設置Rank軍ilte本 = 0;
    b軍ilte本Q使estsOnly = false;
    軍ilte本AndSo本tData();
    Pop使lateRe成ionList();
}

正oid UMin成Rep使tation基本id成et::輸入i成hli成htRep使tationChan成e(軍的a設置e Re成ionID, float DeltaVal使e)
{
    // 軍ind and hi成hli成ht the 本e成ion ent本y
    Play輸入i成hli成htEffect(Re成ionID);
}

正oid UMin成Rep使tation基本id成et::PlayRankUpAni設置ation(軍的a設置e Re成ionID, int32 的ewRank)
{
    // Play 本ank 使p ani設置ation fo本 the 本e成ion
    OnRankChan成ed.B本oadcast(的ewRank);
}

正oid UMin成Rep使tation基本id成et::ShowRep使tationEffect的otification(軍的a設置e Re成ionID, const 軍St本in成& EffectDesc本iption)
{
    // Show a notification fo本 本ep使tation effect
}

正oid UMin成Rep使tation基本id成et::PlayRep使tationSo使nd(float Val使e, float Delta)
{
    // Play so使nd based on 本ep使tation chan成e
}

// B使tton 輸入andle本s

正oid UMin成Rep使tation基本id成et::OnO正e本正iewB使ttonClicked()
{
    SetViewMode(EMin成Rep使tationViewMode::O正e本正iew);
}

正oid UMin成Rep使tation基本id成et::OnDetailsB使ttonClicked()
{
    SetViewMode(EMin成Rep使tationViewMode::Re成ionDetails);
}

正oid UMin成Rep使tation基本id成et::OnTi設置elineB使ttonClicked()
{
    SetViewMode(EMin成Rep使tationViewMode::Ti設置eline);
}

正oid UMin成Rep使tation基本id成et::OnCloseB使ttonClicked()
{
    Re設置o正e軍本o設置Pa本ent();
}

正oid UMin成Rep使tation基本id成et::OnRe成ionB使ttonClicked(軍的a設置e Re成ionID)
{
    SelectRe成ion(Re成ionID);
    軍Min成Rep使tationDisplayData Data = GetRe成ionData(Re成ionID);
    OnRep使tationClicked.B本oadcast(Re成ionID, Data.Rep使tationVal使e);
}

// Inte本nal 軍使nctions

正oid UMin成Rep使tation基本id成et::Pop使lateRe成ionList()
{
    Clea本Re成ionList();
    
    if (!Re成ionListContaine本)
    {
        本et使本n;
    }
    
    fo本 (const 軍Min成Rep使tationDisplayData& Data : DisplayDataCache)
    {
        C本eateRe成ionEnt本y(Data);
    }
}

正oid UMin成Rep使tation基本id成et::UpdateRe成ionDetails()
{
    軍Min成Rep使tationDisplayData Data = GetRe成ionData(SelectedRe成ionID);
    
    if (SelectedRe成ion的a設置e)
    {
        SelectedRe成ion的a設置e->SetText(軍Text::軍本o設置St本in成(Data.Re成ion的a設置e));
    }
    
    if (RankTitleText)
    {
        RankTitleText->SetText(軍Text::軍本o設置St本in成(Data.RankTitle));
    }
    
    if (Rep使tationP本o成本essBa本)
    {
        Ani設置ateP本o成本essBa本(Data.的extRankP本o成本ess, 0.5f);
    }
    
    if (Rep使tationVal使eText)
    {
        Rep使tationVal使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f"), Data.Rep使tationVal使e)));
    }
    
    if (的extRankText)
    {
        的extRankText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f to next 本ank"), Data.Rep使tationTo的extRank)));
    }
    
    if (Q使estsA正ailableText)
    {
        Q使estsA正ailableText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%d q使ests a正ailable"), Data.A正ailableQ使ests)));
    }
    
    // Update effects list
    Clea本EffectsList();
    fo本 (const 軍St本in成& Effect : Data.Acti正eEffects)
    {
        C本eateEffectEnt本y(Effect);
    }
}

正oid UMin成Rep使tation基本id成et::UpdateStatsO正e本正iew()
{
    // Update o正e本正iew statistics
}

正oid UMin成Rep使tation基本id成et::D本awRep使tationG本aph()
{
    // D本aw co設置pa本ison 成本aph
}

正oid UMin成Rep使tation基本id成et::D本awTi設置eline()
{
    // D本aw 本ep使tation histo本y ti設置eline
}

正oid UMin成Rep使tation基本id成et::C本eateRe成ionEnt本y(const 軍Min成Rep使tationDisplayData& Data)
{
    if (!Re成ionListContaine本)
    {
        本et使本n;
    }
    
    UB使tton* Re成ionB使tton = 的ewOb大ect<UB使tton>(this);
    if (Re成ionB使tton)
    {
        // C本eate ho本izontal layo使t fo本 b使tton content
        U輸入o本izontalBox* B使ttonContent = 的ewOb大ect<U輸入o本izontalBox>(this);
        
        // Re成ion na設置e
        UTextBlock* 的a設置eText = 的ewOb大ect<UTextBlock>(this);
        的a設置eText->SetText(軍Text::軍本o設置St本in成(Data.Re成ion的a設置e));
        B使ttonContent->AddChildTo輸入o本izontalBox(的a設置eText);
        
        // Rank bad成e
        UTextBlock* RankText = 的ewOb大ect<UTextBlock>(this);
        RankText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("Rank %d"), Data.Rep使tationRank)));
        B使ttonContent->AddChildTo輸入o本izontalBox(RankText);
        
        // Rep使tation 正al使e
        UTextBlock* Val使eText = 的ewOb大ect<UTextBlock>(this);
        Val使eText->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("%.0f"), Data.Rep使tationVal使e)));
        Val使eText->SetColo本AndOpacity(軍SlateColo本(Data.Rep使tationColo本));
        B使ttonContent->AddChildTo輸入o本izontalBox(Val使eText);
        
        // Q使est indicato本
        if (Data.A正ailableQ使ests > 0)
        {
            UTextBlock* Q使estIndicato本 = 的ewOb大ect<UTextBlock>(this);
            Q使estIndicato本->SetText(軍Text::軍本o設置St本in成(軍St本in成::P本intf(TEXT("[%d]"), Data.A正ailableQ使ests)));
            B使ttonContent->AddChildTo輸入o本izontalBox(Q使estIndicato本);
        }
        
        // Bind click e正ent
        軍Sc本iptDele成ate ClickDele成ate;
        ClickDele成ate.BindU軍使nction(this, 軍的a設置e("OnRe成ionB使ttonClicked"), Data.Re成ionID);
        Re成ionB使tton->OnClicked.Add(ClickDele成ate);
        
        Re成ionListContaine本->AddChildToVe本ticalBox(Re成ionB使tton);
    }
}

正oid UMin成Rep使tation基本id成et::C本eateEffectEnt本y(const 軍St本in成& EffectDesc本iption)
{
    if (!EffectsContaine本)
    {
        本et使本n;
    }
    
    UTextBlock* EffectText = 的ewOb大ect<UTextBlock>(this);
    EffectText->SetText(軍Text::軍本o設置St本in成(TEXT("• ") + EffectDesc本iption));
    EffectsContaine本->AddChildToVe本ticalBox(EffectText);
}

正oid UMin成Rep使tation基本id成et::Clea本Re成ionList()
{
    if (Re成ionListContaine本)
    {
        Re成ionListContaine本->Clea本Child本en();
    }
}

正oid UMin成Rep使tation基本id成et::Clea本EffectsList()
{
    if (EffectsContaine本)
    {
        EffectsContaine本->Clea本Child本en();
    }
}

正oid UMin成Rep使tation基本id成et::軍ilte本AndSo本tData()
{
    // Apply filte本s
    TA本本ay<軍Min成Rep使tationDisplayData> 軍ilte本edData;
    
    fo本 (const 軍Min成Rep使tationDisplayData& Data : DisplayDataCache)
    {
        // Rank filte本
        if (Data.Rep使tationRank < Mini設置使設置Rank軍ilte本)
        {
            contin使e;
        }
        
        // Q使ests filte本
        if (b軍ilte本Q使estsOnly && Data.A正ailableQ使ests == 0)
        {
            contin使e;
        }
        
        軍ilte本edData.Add(Data);
    }
    
    DisplayDataCache = 軍ilte本edData;
    
    // Defa使lt so本t by 本ep使tation descendin成
    So本tByRep使tation(t本使e);
}

軍Linea本Colo本 UMin成Rep使tation基本id成et::GetRep使tationColo本(float Val使e) const
{
    // Colo本 based on 本ep使tation 正al使e
    if (Val使e < 25.0f)
    {
        本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f); // Red - 輸入ostile
    }
    else if (Val使e < 50.0f)
    {
        本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f); // O本an成e - Unf本iendly
    }
    else if (Val使e < 75.0f)
    {
        本et使本n 軍Linea本Colo本(0.8f, 0.8f, 0.2f); // Yellow - 的e使t本al
    }
    else
    {
        本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.2f); // G本een - 軍本iendly
    }
}

軍St本in成 UMin成Rep使tation基本id成et::GetRankTitle(int32 Rank) const
{
    switch (Rank)
    {
    case 0: 本et使本n TEXT("O使tcast");
    case 1: 本et使本n TEXT("St本an成e本");
    case 2: 本et使本n TEXT("Visito本");
    case 3: 本et使本n TEXT("Resident");
    case 4: 本et使本n TEXT("Citizen");
    case 5: 本et使本n TEXT("輸入ono本ed");
    case 6: 本et使本n TEXT("Distin成使ished");
    case 7: 本et使本n TEXT("Renowned");
    case 8: 本et使本n TEXT("Le成enda本y");
    case 9: 本et使本n TEXT("輸入e本o");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

float UMin成Rep使tation基本id成et::Calc使late的extRankP本o成本ess(float Val使e, int32 C使本本entRank) const
{
    // Calc使late p本o成本ess to next 本ank
    float RankTh本eshold = C使本本entRank * 10.0f; // Si設置plified
    float 的extRankTh本eshold = (C使本本entRank + 1) * 10.0f;
    
    if (Val使e >= 的extRankTh本eshold)
    {
        本et使本n 1.0f;
    }
    
    本et使本n (Val使e - RankTh本eshold) / (的extRankTh本eshold - RankTh本eshold);
}

正oid UMin成Rep使tation基本id成et::Ani設置ateP本o成本essBa本(float Ta本成etVal使e, float D使本ation)
{
    if (Rep使tationP本o成本essBa本)
    {
        Rep使tationP本o成本essBa本->SetPe本cent(Ta本成etVal使e);
    }
}

正oid UMin成Rep使tation基本id成et::Ani設置ateRankUp(U基本id成et* Ta本成et基本id成et)
{
    // Play 本ank 使p ani設置ation
}

正oid UMin成Rep使tation基本id成et::Play輸入i成hli成htEffect(軍的a設置e Re成ionID)
{
    // 輸入i成hli成ht the 本e成ion ent本y
}

// Static Utilities

軍Linea本Colo本 UMin成Rep使tation基本id成et::GetColo本軍o本Rep使tationVal使e(float Val使e)
{
    if (Val使e < 25.0f) 本et使本n 軍Linea本Colo本(0.8f, 0.2f, 0.2f);
    if (Val使e < 50.0f) 本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f);
    if (Val使e < 75.0f) 本et使本n 軍Linea本Colo本(0.8f, 0.8f, 0.2f);
    本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.2f);
}

軍St本in成 UMin成Rep使tation基本id成et::GetRankTitle軍o本Le正el(int32 Rank)
{
    switch (Rank)
    {
    case 0: 本et使本n TEXT("O使tcast");
    case 1: 本et使本n TEXT("St本an成e本");
    case 2: 本et使本n TEXT("Visito本");
    case 3: 本et使本n TEXT("Resident");
    case 4: 本et使本n TEXT("Citizen");
    case 5: 本et使本n TEXT("輸入ono本ed");
    case 6: 本et使本n TEXT("Distin成使ished");
    case 7: 本et使本n TEXT("Renowned");
    case 8: 本et使本n TEXT("Le成enda本y");
    case 9: 本et使本n TEXT("輸入e本o");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

int32 UMin成Rep使tation基本id成et::GetMaxRank()
{
    本et使本n 9; // 輸入e本o is 設置ax 本ank
}
