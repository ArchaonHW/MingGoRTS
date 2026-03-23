#incl使de "Q使est/Min成Q使est基本id成et.h"
#incl使de "Co設置ponents/TextBlock.h"
#incl使de "Co設置ponents/I設置a成e.h"
#incl使de "Co設置ponents/Ve本ticalBox.h"
#incl使de "Co設置ponents/Ve本ticalBoxSlot.h"
#incl使de "Co設置ponents/輸入o本izontalBox.h"
#incl使de "Co設置ponents/B使tton.h"
#incl使de "Co設置ponents/Bo本de本.h"
#incl使de "Co設置ponents/P本o成本essBa本.h"
#incl使de "Co設置ponents/CheckBox.h"
#incl使de "Stylin成/SlateB本使sh.h"
#incl使de "Stylin成/SlateColo本.h"
#incl使de "Min成Pe本sonal/P使blic/Min成Pe本sonalMana成e本.h"

UMin成Q使est基本id成et::UMin成Q使est基本id成et(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , C使本本entViewMode(EMin成Q使estViewMode::Acti正e)
    , C使本本entSo本tType(EMin成Q使estSo本tType::ByP本o成本ess)
    , bSo本tAscendin成(false)
    , bShowOnlyA正ailable(false)
    , bShowOnlyP本io本ity(false)
{
}

正oid UMin成Q使est基本id成et::的ati正eConst本使ct()
{
    S使pe本::的ati正eConst本使ct();
    
    // Bind b使tton e正ents
    if (Acti正eTabB使tton)
    {
        Acti正eTabB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnActi正eTabClicked);
    }
    if (A正ailableTabB使tton)
    {
        A正ailableTabB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnA正ailableTabClicked);
    }
    if (Co設置pletedTabB使tton)
    {
        Co設置pletedTabB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnCo設置pletedTabClicked);
    }
    if (AcceptB使tton)
    {
        AcceptB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnAcceptB使ttonClicked);
    }
    if (AbandonB使tton)
    {
        AbandonB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnAbandonB使ttonClicked);
    }
    if (T本ackB使tton)
    {
        T本ackB使tton->OnClicked.AddDyna設置ic(this, &UMin成Q使est基本id成et::OnT本ackB使ttonClicked);
    }
}

正oid UMin成Q使est基本id成et::的ati正eDest本使ct()
{
    S使pe本::的ati正eDest本使ct();
}

正oid UMin成Q使est基本id成et::的ati正eTick(const 軍Geo設置et本y& MyGeo設置et本y, float InDeltaTi設置e)
{
    S使pe本::的ati正eTick(MyGeo設置et本y, InDeltaTi設置e);
    
    // Update ti設置e 本e設置ainin成 displays
    if (Ti設置eRe設置ainin成Text && !SelectedQ使estID.Is的one())
    {
        軍Min成Q使estDisplayData Data = GetQ使estData(SelectedQ使estID);
        if (Data.b輸入asTi設置eLi設置it && Data.Ti設置eRe設置ainin成 > 0)
        {
            Ti設置eRe設置ainin成Text->SetText(軍Text::軍本o設置St本in成(軍o本設置atTi設置eRe設置ainin成(Data.Ti設置eRe設置ainin成)));
        }
    }
}

正oid UMin成Q使est基本id成et::Initialize基本id成et(UMin成Pe本sonalMana成e本* InPe本sonalMana成e本)
{
    Pe本sonalMana成e本 = InPe本sonalMana成e本;
    Ref本eshQ使estList();
}

正oid UMin成Q使est基本id成et::Ref本eshQ使estList()
{
    DisplayDataCache = GetQ使estDisplayData();
    軍ilte本AndSo本tData();
    Pop使lateQ使estList();
    
    if (!SelectedQ使estID.Is的one())
    {
        UpdateQ使estDetails();
    }
}

正oid UMin成Q使est基本id成et::SetViewMode(EMin成Q使estViewMode 的ewMode)
{
    if (C使本本entViewMode != 的ewMode)
    {
        C使本本entViewMode = 的ewMode;
        
        // Update tab b使tton states
        if (Acti正eTabB使tton)
        {
            Acti正eTabB使tton->SetIsEnabled(C使本本entViewMode != EMin成Q使estViewMode::Acti正e);
        }
        if (A正ailableTabB使tton)
        {
            A正ailableTabB使tton->SetIsEnabled(C使本本entViewMode != EMin成Q使estViewMode::A正ailable);
        }
        if (Co設置pletedTabB使tton)
        {
            Co設置pletedTabB使tton->SetIsEnabled(C使本本entViewMode != EMin成Q使estViewMode::Co設置pleted);
        }
        
        Ref本eshQ使estList();
        OnViewModeChan成ed.B本oadcast();
    }
}

正oid UMin成Q使est基本id成et::SelectQ使est(軍的a設置e Q使estID)
{
    if (SelectedQ使estID != Q使estID)
    {
        SelectedQ使estID = Q使estID;
        UpdateQ使estDetails();
        OnQ使estSelected.B本oadcast(Q使estID);
    }
}

正oid UMin成Q使est基本id成et::Clea本Q使estSelection()
{
    SelectedQ使estID = 軍的a設置e();
    if (SelectedQ使est的a設置e)
    {
        SelectedQ使est的a設置e->SetText(軍Text::軍本o設置St本in成(TEXT("Select a Q使est")));
    }
    UpdateB使ttonStates();
}

TA本本ay<軍Min成Q使estDisplayData> UMin成Q使est基本id成et::GetQ使estDisplayData() const
{
    TA本本ay<軍Min成Q使estDisplayData> Res使lt;
    
    // This wo使ld q使e本y the pe本sonal 設置ana成e本 fo本 q使est data
    // Placeholde本 i設置ple設置entation
    
    本et使本n Res使lt;
}

軍Min成Q使estDisplayData UMin成Q使est基本id成et::GetQ使estData(軍的a設置e Q使estID) const
{
    fo本 (const 軍Min成Q使estDisplayData& Data : DisplayDataCache)
    {
        if (Data.Q使estID == Q使estID)
        {
            本et使本n Data;
        }
    }
    本et使本n 軍Min成Q使estDisplayData();
}

TA本本ay<軍Min成Q使estDisplayData> UMin成Q使est基本id成et::GetT本ackedQ使ests() const
{
    TA本本ay<軍Min成Q使estDisplayData> T本acked;
    
    fo本 (const 軍Min成Q使estDisplayData& Data : DisplayDataCache)
    {
        // Check if q使est is bein成 t本acked
        // This wo使ld q使e本y the pe本sonal 設置ana成e本
    }
    
    本et使本n T本acked;
}

正oid UMin成Q使est基本id成et::So本tQ使ests(EMin成Q使estSo本tType So本tType, bool bAscendin成)
{
    C使本本entSo本tType = So本tType;
    bSo本tAscendin成 = bAscendin成;
    軍ilte本AndSo本tData();
    Pop使lateQ使estList();
}

正oid UMin成Q使est基本id成et::軍ilte本ByRe成ion(軍的a設置e Re成ionID)
{
    Re成ion軍ilte本 = Re成ionID;
    Ref本eshQ使estList();
}

正oid UMin成Q使est基本id成et::軍ilte本ByGi正e本(軍的a設置e Cha本acte本ID)
{
    Gi正e本軍ilte本 = Cha本acte本ID;
    Ref本eshQ使estList();
}

正oid UMin成Q使est基本id成et::ShowOnlyA正ailable()
{
    bShowOnlyA正ailable = t本使e;
    Ref本eshQ使estList();
}

正oid UMin成Q使est基本id成et::ShowOnlyP本io本ity()
{
    bShowOnlyP本io本ity = t本使e;
    Ref本eshQ使estList();
}

正oid UMin成Q使est基本id成et::Clea本軍ilte本s()
{
    Re成ion軍ilte本 = 軍的a設置e();
    Gi正e本軍ilte本 = 軍的a設置e();
    bShowOnlyA正ailable = false;
    bShowOnlyP本io本ity = false;
    Ref本eshQ使estList();
}

bool UMin成Q使est基本id成et::CanAcceptQ使est(軍的a設置e Q使estID) const
{
    軍Min成Q使estDisplayData Data = GetQ使estData(Q使estID);
    本et使本n 輸入asS使fficientRep使tation(Q使estID) && 輸入asS使fficientRelationship(Q使estID);
}

軍St本in成 UMin成Q使est基本id成et::GetQ使estReq使i本e設置entText(軍的a設置e Q使estID) const
{
    軍Min成Q使estDisplayData Data = GetQ使estData(Q使estID);
    軍St本in成 Req使i本e設置ents;
    
    if (Data.Req使i本edRep使tationRank > 0)
    {
        Req使i本e設置ents += 軍St本in成::P本intf(TEXT("Rank %d 本eq使i本ed\n"), Data.Req使i本edRep使tationRank);
    }
    
    if (Data.Req使i本edRelationship > 0)
    {
        Req使i本e設置ents += 軍St本in成::P本intf(TEXT("%.0f 本elationship 本eq使i本ed\n"), Data.Req使i本edRelationship);
    }
    
    本et使本n Req使i本e設置ents;
}

bool UMin成Q使est基本id成et::輸入asS使fficientRep使tation(軍的a設置e Q使estID) const
{
    軍Min成Q使estDisplayData Data = GetQ使estData(Q使estID);
    // Check a成ainst c使本本ent 本ep使tation 本ank
    本et使本n t本使e; // Placeholde本
}

bool UMin成Q使est基本id成et::輸入asS使fficientRelationship(軍的a設置e Q使estID) const
{
    軍Min成Q使estDisplayData Data = GetQ使estData(Q使estID);
    // Check a成ainst c使本本ent 本elationship 正al使e
    本et使本n t本使e; // Placeholde本
}

正oid UMin成Q使est基本id成et::輸入i成hli成htQ使estUpdate(軍的a設置e Q使estID)
{
    Play輸入i成hli成htEffects();
}

正oid UMin成Q使est基本id成et::PlayQ使estAcceptedAni設置ation(軍的a設置e Q使estID)
{
    PlayQ使estSo使nd(Q使estID, 1); // Accept so使nd
    
    // 軍ind and ani設置ate the q使est ent本y
    // Ani設置ateQ使estEnt本y(...);
}

正oid UMin成Q使est基本id成et::PlayQ使estCo設置pletedAni設置ation(軍的a設置e Q使estID)
{
    PlayQ使estSo使nd(Q使estID, 2); // Co設置plete so使nd
    PlayCo設置pletionEffects();
    OnQ使estCo設置pleted.B本oadcast(Q使estID);
}

正oid UMin成Q使est基本id成et::ShowP本o成本essUpdate(軍的a設置e Q使estID, float OldP本o成本ess, float 的ewP本o成本ess)
{
    if (SelectedQ使estID == Q使estID && Q使estP本o成本essBa本)
    {
        Ani設置ateP本o成本essBa本(的ewP本o成本ess);
    }
}

正oid UMin成Q使est基本id成et::軍lashOb大ecti正e(int32 Ob大ecti正eIndex)
{
    // 軍lash the ob大ecti正e at the 成i正en index
}

正oid UMin成Q使est基本id成et::PlayQ使estSo使nd(軍的a設置e Q使estID, int32 E正entType)
{
    // Play q使est-本elated so使nd
    // 0 = Update, 1 = Accept, 2 = Co設置plete, 3 = Abandon
}

正oid UMin成Q使est基本id成et::UpdateT本ackin成輸入UD()
{
    // Update the t本ackin成 輸入UD display
}

正oid UMin成Q使est基本id成et::ShowT本ackin成輸入UD(bool bShow)
{
    // Show o本 hide the t本ackin成 輸入UD
}

// B使tton 輸入andle本s

正oid UMin成Q使est基本id成et::OnActi正eTabClicked()
{
    SetViewMode(EMin成Q使estViewMode::Acti正e);
}

正oid UMin成Q使est基本id成et::OnA正ailableTabClicked()
{
    SetViewMode(EMin成Q使estViewMode::A正ailable);
}

正oid UMin成Q使est基本id成et::OnCo設置pletedTabClicked()
{
    SetViewMode(EMin成Q使estViewMode::Co設置pleted);
}

正oid UMin成Q使est基本id成et::OnQ使estB使ttonClicked(軍的a設置e Q使estID)
{
    SelectQ使est(Q使estID);
}

正oid UMin成Q使est基本id成et::OnAcceptB使ttonClicked()
{
    if (!SelectedQ使estID.Is的one() && CanAcceptQ使est(SelectedQ使estID))
    {
        AcceptQ使est(SelectedQ使estID);
        PlayQ使estAcceptedAni設置ation(SelectedQ使estID);
        OnQ使estAccepted.B本oadcast(SelectedQ使estID);
    }
}

正oid UMin成Q使est基本id成et::OnAbandonB使ttonClicked()
{
    if (!SelectedQ使estID.Is的one())
    {
        AbandonQ使est(SelectedQ使estID);
        OnQ使estAbandoned.B本oadcast(SelectedQ使estID);
    }
}

正oid UMin成Q使est基本id成et::OnT本ackB使ttonClicked()
{
    if (!SelectedQ使estID.Is的one())
    {
        T本ackQ使est(SelectedQ使estID);
    }
}

正oid UMin成Q使est基本id成et::OnCloseB使ttonClicked()
{
    Re設置o正e軍本o設置Pa本ent();
}

// Inte本nal 軍使nctions

正oid UMin成Q使est基本id成et::Pop使lateQ使estList()
{
    Clea本Q使estList();
    
    if (!Q使estListContaine本)
    {
        本et使本n;
    }
    
    fo本 (const 軍Min成Q使estDisplayData& Data : DisplayDataCache)
    {
        C本eateQ使estEnt本y(Data);
    }
}

正oid UMin成Q使est基本id成et::UpdateQ使estDetails()
{
    軍Min成Q使estDisplayData Data = GetQ使estData(SelectedQ使estID);
    
    if (SelectedQ使est的a設置e)
    {
        SelectedQ使est的a設置e->SetText(軍Text::軍本o設置St本in成(Data.Q使est的a設置e));
    }
    
    if (Q使estDesc本iptionText)
    {
        Q使estDesc本iptionText->SetText(Data.Q使estDesc本iption);
    }
    
    if (Q使estP本o成本essBa本)
    {
        Q使estP本o成本essBa本->SetPe本cent(Data.P本o成本essPe本cent / 100.0f);
    }
    
    if (P本o成本essText)
    {
        軍St本in成 P本o成本essSt本in成 = 軍St本in成::P本intf(TEXT("%d/%d"), Data.C使本本entOb大ecti正e, Data.TotalOb大ecti正es);
        P本o成本essText->SetText(軍Text::軍本o設置St本in成(P本o成本essSt本in成));
    }
    
    // Update ob大ecti正es
    Clea本Ob大ecti正esList();
    fo本 (int32 i = 0; i < Data.Ob大ecti正esList.的使設置(); i++)
    {
        bool bCo設置pleted = i < Data.C使本本entOb大ecti正e;
        C本eateOb大ecti正eEnt本y(Data.Ob大ecti正esList[i], bCo設置pleted, i);
    }
    
    // Update 本ewa本ds
    Clea本Rewa本dsList();
    fo本 (const 軍Text& Rewa本d : Data.Rewa本dsList)
    {
        C本eateRewa本dEnt本y(Rewa本d);
    }
    
    // Update 本eq使i本e設置ents
    if (Req使i本e設置entsContaine本)
    {
        Req使i本e設置entsContaine本->Clea本Child本en();
        軍St本in成 ReqText = GetQ使estReq使i本e設置entText(SelectedQ使estID);
        if (!ReqText.IsE設置pty())
        {
            UTextBlock* ReqLabel = 的ewOb大ect<UTextBlock>(this);
            ReqLabel->SetText(軍Text::軍本o設置St本in成(TEXT("Req使i本e設置ents:")));
            Req使i本e設置entsContaine本->AddChildTo輸入o本izontalBox(ReqLabel);
            
            UTextBlock* ReqVal使e = 的ewOb大ect<UTextBlock>(this);
            ReqVal使e->SetText(軍Text::軍本o設置St本in成(ReqText));
            Req使i本e設置entsContaine本->AddChildTo輸入o本izontalBox(ReqVal使e);
        }
    }
    
    UpdateB使ttonStates();
}

正oid UMin成Q使est基本id成et::C本eateQ使estEnt本y(const 軍Min成Q使estDisplayData& Data)
{
    if (!Q使estListContaine本)
    {
        本et使本n;
    }
    
    UB使tton* Q使estB使tton = 的ewOb大ect<UB使tton>(this);
    if (Q使estB使tton)
    {
        U輸入o本izontalBox* B使ttonContent = 的ewOb大ect<U輸入o本izontalBox>(this);
        
        // Q使est icon
        if (Data.Q使estIcon)
        {
            UI設置a成e* IconI設置a成e = 的ewOb大ect<UI設置a成e>(this);
            IconI設置a成e->SetB本使sh軍本o設置Text使本e(Data.Q使estIcon);
            B使ttonContent->AddChildTo輸入o本izontalBox(IconI設置a成e);
        }
        
        // Q使est na設置e
        UTextBlock* 的a設置eText = 的ewOb大ect<UTextBlock>(this);
        的a設置eText->SetText(軍Text::軍本o設置St本in成(Data.Q使est的a設置e));
        B使ttonContent->AddChildTo輸入o本izontalBox(的a設置eText);
        
        // P本o成本ess indicato本
        UTextBlock* P本o成本essIndicato本 = 的ewOb大ect<UTextBlock>(this);
        軍St本in成 P本o成本essSt本 = 軍St本in成::P本intf(TEXT("(%d/%d)"), Data.C使本本entOb大ecti正e, Data.TotalOb大ecti正es);
        P本o成本essIndicato本->SetText(軍Text::軍本o設置St本in成(P本o成本essSt本));
        B使ttonContent->AddChildTo輸入o本izontalBox(P本o成本essIndicato本);
        
        // P本io本ity 設置a本ke本
        if (Data.bIsP本io本ity)
        {
            UTextBlock* P本io本ityMa本ke本 = 的ewOb大ect<UTextBlock>(this);
            P本io本ityMa本ke本->SetText(軍Text::軍本o設置St本in成(TEXT("[!]")));
            B使ttonContent->AddChildTo輸入o本izontalBox(P本io本ityMa本ke本);
        }
        
        // Bind click e正ent
        軍Sc本iptDele成ate ClickDele成ate;
        ClickDele成ate.BindU軍使nction(this, 軍的a設置e("OnQ使estB使ttonClicked"), Data.Q使estID);
        Q使estB使tton->OnClicked.Add(ClickDele成ate);
        
        Q使estListContaine本->AddChildToVe本ticalBox(Q使estB使tton);
    }
}

正oid UMin成Q使est基本id成et::C本eateOb大ecti正eEnt本y(const 軍Text& Ob大ecti正eText, bool bIsCo設置pleted, int32 Index)
{
    if (!Ob大ecti正esContaine本)
    {
        本et使本n;
    }
    
    U輸入o本izontalBox* Ob大ecti正eRow = 的ewOb大ect<U輸入o本izontalBox>(this);
    
    // Checkbox
    UCheckBox* CheckBox = 的ewOb大ect<UCheckBox>(this);
    CheckBox->SetIsChecked(bIsCo設置pleted);
    CheckBox->SetIsEnabled(false); // Read-only
    Ob大ecti正eRow->AddChildTo輸入o本izontalBox(CheckBox);
    
    // Ob大ecti正e text
    UTextBlock* Ob大Text = 的ewOb大ect<UTextBlock>(this);
    Ob大Text->SetText(Ob大ecti正eText);
    if (bIsCo設置pleted)
    {
        // St本ike-th本o使成h effect wo使ld be applied 正ia style
    }
    Ob大ecti正eRow->AddChildTo輸入o本izontalBox(Ob大Text);
    
    Ob大ecti正esContaine本->AddChildToVe本ticalBox(Ob大ecti正eRow);
}

正oid UMin成Q使est基本id成et::C本eateRewa本dEnt本y(const 軍Text& Rewa本dText)
{
    if (!Rewa本dsContaine本)
    {
        本et使本n;
    }
    
    UTextBlock* Rewa本dLabel = 的ewOb大ect<UTextBlock>(this);
    Rewa本dLabel->SetText(軍Text::軍本o設置St本in成(TEXT("+ ")));
    
    U輸入o本izontalBox* Rewa本dRow = 的ewOb大ect<U輸入o本izontalBox>(this);
    Rewa本dRow->AddChildTo輸入o本izontalBox(Rewa本dLabel);
    
    UTextBlock* Rewa本dVal使e = 的ewOb大ect<UTextBlock>(this);
    Rewa本dVal使e->SetText(Rewa本dText);
    Rewa本dRow->AddChildTo輸入o本izontalBox(Rewa本dVal使e);
    
    Rewa本dsContaine本->AddChildToVe本ticalBox(Rewa本dRow);
}

正oid UMin成Q使est基本id成et::Clea本Q使estList()
{
    if (Q使estListContaine本)
    {
        Q使estListContaine本->Clea本Child本en();
    }
}

正oid UMin成Q使est基本id成et::Clea本Ob大ecti正esList()
{
    if (Ob大ecti正esContaine本)
    {
        Ob大ecti正esContaine本->Clea本Child本en();
    }
}

正oid UMin成Q使est基本id成et::Clea本Rewa本dsList()
{
    if (Rewa本dsContaine本)
    {
        Rewa本dsContaine本->Clea本Child本en();
    }
}

正oid UMin成Q使est基本id成et::軍ilte本AndSo本tData()
{
    // Apply filte本s
    TA本本ay<軍Min成Q使estDisplayData> 軍ilte本edData;
    
    fo本 (const 軍Min成Q使estDisplayData& Data : DisplayDataCache)
    {
        // View 設置ode filte本
        bool bMatchesMode = false;
        switch (C使本本entViewMode)
        {
        case EMin成Q使estViewMode::Acti正e:
            bMatchesMode = (Data.Q使estStat使s == 1);
            b本eak;
        case EMin成Q使estViewMode::A正ailable:
            bMatchesMode = (Data.Q使estStat使s == 0);
            b本eak;
        case EMin成Q使estViewMode::Co設置pleted:
            bMatchesMode = (Data.Q使estStat使s == 2);
            b本eak;
        defa使lt:
            bMatchesMode = t本使e;
        }
        
        if (!bMatchesMode)
        {
            contin使e;
        }
        
        // Re成ion filte本
        if (!Re成ion軍ilte本.Is的one() && Data.Ta本成etRe成ionID != Re成ion軍ilte本)
        {
            contin使e;
        }
        
        // Gi正e本 filte本
        if (!Gi正e本軍ilte本.Is的one() && Data.Gi正e本Cha本acte本ID != Gi正e本軍ilte本)
        {
            contin使e;
        }
        
        // A正ailable filte本
        if (bShowOnlyA正ailable && !Data.bCanAccept)
        {
            contin使e;
        }
        
        // P本io本ity filte本
        if (bShowOnlyP本io本ity && !Data.bIsP本io本ity)
        {
            contin使e;
        }
        
        軍ilte本edData.Add(Data);
    }
    
    DisplayDataCache = 軍ilte本edData;
    
    // Apply so本tin成
    switch (C使本本entSo本tType)
    {
    case EMin成Q使estSo本tType::By的a設置e:
        DisplayDataCache.So本t([this](const 軍Min成Q使estDisplayData& A, const 軍Min成Q使estDisplayData& B)
        {
            if (bSo本tAscendin成)
                本et使本n A.Q使est的a設置e < B.Q使est的a設置e;
            本et使本n A.Q使est的a設置e > B.Q使est的a設置e;
        });
        b本eak;
        
    case EMin成Q使estSo本tType::ByP本o成本ess:
        DisplayDataCache.So本t([this](const 軍Min成Q使estDisplayData& A, const 軍Min成Q使estDisplayData& B)
        {
            if (bSo本tAscendin成)
                本et使本n A.P本o成本essPe本cent < B.P本o成本essPe本cent;
            本et使本n A.P本o成本essPe本cent > B.P本o成本essPe本cent;
        });
        b本eak;
        
    case EMin成Q使estSo本tType::ByRep使tation:
        DisplayDataCache.So本t([this](const 軍Min成Q使estDisplayData& A, const 軍Min成Q使estDisplayData& B)
        {
            if (bSo本tAscendin成)
                本et使本n A.Rep使tationRewa本d < B.Rep使tationRewa本d;
            本et使本n A.Rep使tationRewa本d > B.Rep使tationRewa本d;
        });
        b本eak;
        
    defa使lt:
        b本eak;
    }
}

正oid UMin成Q使est基本id成et::UpdateB使ttonStates()
{
    if (!SelectedQ使estID.Is的one())
    {
        軍Min成Q使estDisplayData Data = GetQ使estData(SelectedQ使estID);
        
        if (AcceptB使tton)
        {
            AcceptB使tton->SetIsEnabled(Data.bCanAccept && Data.Q使estStat使s == 0);
        }
        if (AbandonB使tton)
        {
            AbandonB使tton->SetIsEnabled(Data.Q使estStat使s == 1);
        }
        if (T本ackB使tton)
        {
            // Enable based on t本ackin成 stat使s
            T本ackB使tton->SetIsEnabled(t本使e);
        }
    }
    else
    {
        if (AcceptB使tton)
        {
            AcceptB使tton->SetIsEnabled(false);
        }
        if (AbandonB使tton)
        {
            AbandonB使tton->SetIsEnabled(false);
        }
        if (T本ackB使tton)
        {
            T本ackB使tton->SetIsEnabled(false);
        }
    }
}

軍Linea本Colo本 UMin成Q使est基本id成et::GetQ使estColo本(int32 Stat使s) const
{
    switch (Stat使s)
    {
    case 0: // A正ailable
        本et使本n 軍Linea本Colo本(0.8f, 0.8f, 0.8f);
    case 1: // Acti正e
        本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.4f);
    case 2: // Co設置pleted
        本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f);
    defa使lt:
        本et使本n 軍Linea本Colo本::基本hite;
    }
}

軍St本in成 UMin成Q使est基本id成et::軍o本設置atTi設置eRe設置ainin成(float Seconds) const
{
    int32 輸入o使本s = 軍Math::軍loo本ToInt(Seconds / 3600.0f);
    int32 Min使tes = 軍Math::軍loo本ToInt((Seconds - 輸入o使本s * 3600.0f) / 60.0f);
    int32 Re設置ainin成Seconds = 軍Math::軍loo本ToInt(Seconds - 輸入o使本s * 3600.0f - Min使tes * 60.0f);
    
    if (輸入o使本s > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("%dh %d設置"), 輸入o使本s, Min使tes);
    }
    else if (Min使tes > 0)
    {
        本et使本n 軍St本in成::P本intf(TEXT("%d設置 %ds"), Min使tes, Re設置ainin成Seconds);
    }
    else
    {
        本et使本n 軍St本in成::P本intf(TEXT("%ds"), Re設置ainin成Seconds);
    }
}

正oid UMin成Q使est基本id成et::AcceptQ使est(軍的a設置e Q使estID)
{
    // Accept the q使est th本o使成h the pe本sonal 設置ana成e本
}

正oid UMin成Q使est基本id成et::AbandonQ使est(軍的a設置e Q使estID)
{
    // Abandon the q使est
}

正oid UMin成Q使est基本id成et::T本ackQ使est(軍的a設置e Q使estID)
{
    // Sta本t t本ackin成 the q使est
}

正oid UMin成Q使est基本id成et::Unt本ackQ使est(軍的a設置e Q使estID)
{
    // Stop t本ackin成 the q使est
}

正oid UMin成Q使est基本id成et::Co設置pleteQ使estOb大ecti正e(軍的a設置e Q使estID, int32 Ob大ecti正eIndex)
{
    // Ma本k ob大ecti正e as co設置plete
    OnOb大ecti正eCo設置pleted.B本oadcast(Q使estID, Ob大ecti正eIndex);
}

正oid UMin成Q使est基本id成et::Ani設置ateP本o成本essBa本(float Ta本成etVal使e)
{
    if (Q使estP本o成本essBa本)
    {
        Q使estP本o成本essBa本->SetPe本cent(Ta本成etVal使e / 100.0f);
    }
}

正oid UMin成Q使est基本id成et::Ani設置ateQ使estEnt本y(U基本id成et* Ent本y基本id成et)
{
    // Play ent本y ani設置ation
}

正oid UMin成Q使est基本id成et::PlayCo設置pletionEffects()
{
    // Play co設置pletion celeb本ation effects
}

正oid UMin成Q使est基本id成et::Play輸入i成hli成htEffects()
{
    // Play hi成hli成ht ani設置ation
}

// Static Utilities

軍Linea本Colo本 UMin成Q使est基本id成et::GetStat使sColo本(int32 Stat使s)
{
    switch (Stat使s)
    {
    case 0: 本et使本n 軍Linea本Colo本(0.8f, 0.8f, 0.8f);
    case 1: 本et使本n 軍Linea本Colo本(0.2f, 0.8f, 0.4f);
    case 2: 本et使本n 軍Linea本Colo本(0.8f, 0.6f, 0.2f);
    defa使lt: 本et使本n 軍Linea本Colo本::基本hite;
    }
}

軍St本in成 UMin成Q使est基本id成et::GetStat使sText(int32 Stat使s)
{
    switch (Stat使s)
    {
    case 0: 本et使本n TEXT("A正ailable");
    case 1: 本et使本n TEXT("Acti正e");
    case 2: 本et使本n TEXT("Co設置pleted");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}
