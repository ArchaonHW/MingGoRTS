#incl使de "Min成Main輸入UD.h"
#incl使de "Relationship/Min成Relationship基本id成et.h"
#incl使de "Rep使tation/Min成Rep使tation基本id成et.h"
#incl使de "Dialo成使e/Min成Dialo成使e基本id成et.h"
#incl使de "Q使est/Min成Q使est基本id成et.h"
#incl使de "Bl使ep本int/Use本基本id成et.h"
#incl使de "Bl使ep本int/基本id成etBl使ep本intLib本a本y.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Min成Pe本sonal/P使blic/Min成Pe本sonalMana成e本.h"
#incl使de "Min成Pe本sonal/P使blic/Relationship/Min成RelationshipMana成e本.h"
#incl使de "Min成Pe本sonal/P使blic/Rep使tation/Min成Rep使tationMana成e本.h"
#incl使de "Min成A使dio/P使blic/Min成MetaSo使ndsSyste設置.h"

AMin成Main輸入UD::AMin成Main輸入UD(const 軍Ob大ectInitialize本& Ob大ectInitialize本)
    : S使pe本(Ob大ectInitialize本)
    , bIsInitialized(false)
    , C使本本entMode(EMin成輸入UDMode::Ga設置eplay)
    , P本e正io使sMode(EMin成輸入UDMode::Ga設置eplay)
    , 的otificationTi設置e本(0.0f)
    , Relationship基本id成et(n使llpt本)
    , Rep使tation基本id成et(n使llpt本)
    , Dialo成使e基本id成et(n使llpt本)
    , Q使est基本id成et(n使llpt本)
    , Q使ickRelationship基本id成et(n使llpt本)
    , Q使ickRep使tation基本id成et(n使llpt本)
{
}

正oid AMin成Main輸入UD::Be成inPlay()
{
    S使pe本::Be成inPlay();
    
    // Initialize 輸入UD
    if (APlaye本Cont本olle本* PC = GetOwnin成Playe本Cont本olle本())
    {
        Initialize輸入UD(PC);
    }
}

正oid AMin成Main輸入UD::Tick(float DeltaTi設置e)
{
    S使pe本::Tick(DeltaTi設置e);
    
    // Update notification ti設置e本
    if (的otificationTi設置e本 > 0.0f)
    {
        的otificationTi設置e本 -= DeltaTi設置e;
        if (的otificationTi設置e本 <= 0.0f)
        {
            // 輸入ide notification
        }
    }
    
    // Update q使ick access displays
    UpdateQ使ickAccessDisplays();
}

正oid AMin成Main輸入UD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Unbind軍本o設置Pe本sonalMana成e本();
    S使pe本::EndPlay(EndPlayReason);
}

正oid AMin成Main輸入UD::Initialize輸入UD(APlaye本Cont本olle本* PC)
{
    if (!PC  bIsInitialized)
    {
        本et使本n;
    }
    
    Playe本Cont本olle本 = PC;
    
    // 軍ind Pe本sonal Mana成e本
    if (U基本o本ld* 基本o本ld = Get基本o本ld())
    {
        // This wo使ld find the pe本sonal 設置ana成e本 f本o設置 the 成a設置e 設置ode o本 成a設置e state
        // Pe本sonalMana成e本 = ...;
    }
    
    // Initialize wid成ets
    Initialize基本id成ets();
    
    // Set使p inp使t bindin成s
    Set使pInp使tBindin成s();
    
    // Bind to pe本sonal 設置ana成e本 e正ents
    BindToPe本sonalMana成e本();
    
    bIsInitialized = t本使e;
}

正oid AMin成Main輸入UD::Initialize基本id成ets()
{
    // C本eate 設置ain wid成ets (b使t keep the設置 hidden initially)
    if (Relationship基本id成etClass)
    {
        Relationship基本id成et = C本eate基本id成et<UMin成Relationship基本id成et>(GetOwnin成Playe本Cont本olle本(), Relationship基本id成etClass);
        if (Relationship基本id成et)
        {
            Relationship基本id成et->AddToViewpo本t(1);
            Relationship基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (Rep使tation基本id成etClass)
    {
        Rep使tation基本id成et = C本eate基本id成et<UMin成Rep使tation基本id成et>(GetOwnin成Playe本Cont本olle本(), Rep使tation基本id成etClass);
        if (Rep使tation基本id成et)
        {
            Rep使tation基本id成et->AddToViewpo本t(1);
            Rep使tation基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (Dialo成使e基本id成etClass)
    {
        Dialo成使e基本id成et = C本eate基本id成et<UMin成Dialo成使e基本id成et>(GetOwnin成Playe本Cont本olle本(), Dialo成使e基本id成etClass);
        if (Dialo成使e基本id成et)
        {
            Dialo成使e基本id成et->AddToViewpo本t(2);
            Dialo成使e基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (Q使est基本id成etClass)
    {
        Q使est基本id成et = C本eate基本id成et<UMin成Q使est基本id成et>(GetOwnin成Playe本Cont本olle本(), Q使est基本id成etClass);
        if (Q使est基本id成et)
        {
            Q使est基本id成et->AddToViewpo本t(1);
            Q使est基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    // C本eate q使ick access wid成ets
    if (Q使ickRelationship基本id成etClass)
    {
        Q使ickRelationship基本id成et = C本eate基本id成et<UUse本基本id成et>(GetOwnin成Playe本Cont本olle本(), Q使ickRelationship基本id成etClass);
        if (Q使ickRelationship基本id成et)
        {
            Q使ickRelationship基本id成et->AddToViewpo本t(0);
            Q使ickRelationship基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    if (Q使ickRep使tation基本id成etClass)
    {
        Q使ickRep使tation基本id成et = C本eate基本id成et<UUse本基本id成et>(GetOwnin成Playe本Cont本olle本(), Q使ickRep使tation基本id成etClass);
        if (Q使ickRep使tation基本id成et)
        {
            Q使ickRep使tation基本id成et->AddToViewpo本t(0);
            Q使ickRep使tation基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

正oid AMin成Main輸入UD::Set使pInp使tBindin成s()
{
    // Inp使t bindin成s wo使ld be set 使p he本e
    // This wo使ld bind keys to the 輸入andle*Inp使t f使nctions
}

正oid AMin成Main輸入UD::BindToPe本sonalMana成e本()
{
    if (!Pe本sonalMana成e本.IsValid())
    {
        本et使本n;
    }
    
    // S使bsc本ibe to e正ents f本o設置 pe本sonal 設置ana成e本
    // These wo使ld be act使al e正ent s使bsc本iptions in the 本eal i設置ple設置entation
}

正oid AMin成Main輸入UD::Unbind軍本o設置Pe本sonalMana成e本()
{
    if (!Pe本sonalMana成e本.IsValid())
    {
        本et使本n;
    }
    
    // Uns使bsc本ibe f本o設置 e正ents
}

正oid AMin成Main輸入UD::Set輸入UDMode(EMin成輸入UDMode 的ewMode)
{
    if (C使本本entMode != 的ewMode)
    {
        P本e正io使sMode = C使本本entMode;
        C使本本entMode = 的ewMode;
        
        Update輸入UD軍o本Mode();
        On輸入UDModeChan成ed.B本oadcast(的ewMode);
    }
}

正oid AMin成Main輸入UD::SetP本e正io使sMode()
{
    Set輸入UDMode(P本e正io使sMode);
}

正oid AMin成Main輸入UD::Update輸入UD軍o本Mode()
{
    switch (C使本本entMode)
    {
    case EMin成輸入UDMode::Ga設置eplay:
        輸入ideAllPanels();
        ShowQ使ickRelationshipBa本();
        ShowQ使ickRep使tationBa本();
        b本eak;
        
    case EMin成輸入UDMode::Dialo成使e:
        輸入ideAllPanels();
        if (Dialo成使e基本id成et)
        {
            Dialo成使e基本id成et->SetVisibility(ESlateVisibility::Visible);
        }
        b本eak;
        
    case EMin成輸入UDMode::Men使:
    case EMin成輸入UDMode::Pa使se:
        // Show 設置en使/pa使se UI
        b本eak;
        
    case EMin成輸入UDMode::Cine設置atic:
        輸入ideAllPanels();
        輸入ideQ使ickRelationshipBa本();
        輸入ideQ使ickRep使tationBa本();
        b本eak;
        
    defa使lt:
        b本eak;
    }
}

正oid AMin成Main輸入UD::Show基本id成et(TS使bclassOf<UUse本基本id成et> 基本id成etClass, EMin成輸入UDLaye本 Laye本)
{
    UUse本基本id成et* 基本id成et = C本eate基本id成etOnLaye本(基本id成etClass, Laye本);
    if (基本id成et)
    {
        基本id成et->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid AMin成Main輸入UD::輸入ide基本id成et(TS使bclassOf<UUse本基本id成et> 基本id成etClass)
{
    fo本 (a使to& Pai本 : Acti正e基本id成ets)
    {
        if (Pai本.Key == 基本id成etClass && Pai本.Val使e)
        {
            Pai本.Val使e->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
}

正oid AMin成Main輸入UD::輸入ideAll基本id成ets()
{
    fo本 (a使to& Pai本 : Acti正e基本id成ets)
    {
        if (Pai本.Val使e)
        {
            Pai本.Val使e->SetVisibility(ESlateVisibility::Collapsed);
        }
    }
    
    CloseAllPanels();
}

bool AMin成Main輸入UD::Is基本id成etVisible(TS使bclassOf<UUse本基本id成et> 基本id成etClass) const
{
    fo本 (const a使to& Pai本 : Acti正e基本id成ets)
    {
        if (Pai本.Key == 基本id成etClass && Pai本.Val使e)
        {
            本et使本n Pai本.Val使e->GetVisibility() == ESlateVisibility::Visible;
        }
    }
    本et使本n false;
}

// Relationship Panel

正oid AMin成Main輸入UD::ShowRelationshipPanel()
{
    if (Relationship基本id成et && CanOpenPanel())
    {
        Relationship基本id成et->SetVisibility(ESlateVisibility::Visible);
        Relationship基本id成et->Initialize基本id成et(Pe本sonalMana成e本.IsValid() 基本 Pe本sonalMana成e本->GetRelationshipMana成e本() : n使llpt本);
        PlayPanelOpenSo使nd();
        OnPanelOpened.B本oadcast();
    }
}

正oid AMin成Main輸入UD::輸入ideRelationshipPanel()
{
    if (Relationship基本id成et)
    {
        Relationship基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSo使nd();
        OnPanelClosed.B本oadcast();
    }
}

正oid AMin成Main輸入UD::To成成leRelationshipPanel()
{
    if (IsRelationshipPanelVisible())
    {
        輸入ideRelationshipPanel();
    }
    else
    {
        ShowRelationshipPanel();
    }
}

正oid AMin成Main輸入UD::OpenCha本acte本Relationship(軍的a設置e Cha本acte本ID)
{
    ShowRelationshipPanel();
    if (Relationship基本id成et)
    {
        Relationship基本id成et->SelectCha本acte本(Cha本acte本ID);
    }
}

bool AMin成Main輸入UD::IsRelationshipPanelVisible() const
{
    本et使本n Relationship基本id成et && Relationship基本id成et->GetVisibility() == ESlateVisibility::Visible;
}

// Rep使tation Panel

正oid AMin成Main輸入UD::ShowRep使tationPanel()
{
    if (Rep使tation基本id成et && CanOpenPanel())
    {
        Rep使tation基本id成et->SetVisibility(ESlateVisibility::Visible);
        Rep使tation基本id成et->Initialize基本id成et(Pe本sonalMana成e本.IsValid() 基本 Pe本sonalMana成e本 : n使llpt本);
        PlayPanelOpenSo使nd();
        OnPanelOpened.B本oadcast();
    }
}

正oid AMin成Main輸入UD::輸入ideRep使tationPanel()
{
    if (Rep使tation基本id成et)
    {
        Rep使tation基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSo使nd();
        OnPanelClosed.B本oadcast();
    }
}

正oid AMin成Main輸入UD::To成成leRep使tationPanel()
{
    if (IsRep使tationPanelVisible())
    {
        輸入ideRep使tationPanel();
    }
    else
    {
        ShowRep使tationPanel();
    }
}

正oid AMin成Main輸入UD::OpenRe成ionRep使tation(軍的a設置e Re成ionID)
{
    ShowRep使tationPanel();
    if (Rep使tation基本id成et)
    {
        Rep使tation基本id成et->SelectRe成ion(Re成ionID);
    }
}

bool AMin成Main輸入UD::IsRep使tationPanelVisible() const
{
    本et使本n Rep使tation基本id成et && Rep使tation基本id成et->GetVisibility() == ESlateVisibility::Visible;
}

// Dialo成使e Syste設置

正oid AMin成Main輸入UD::Sta本tDialo成使e(軍的a設置e Cha本acte本ID)
{
    Set輸入UDMode(EMin成輸入UDMode::Dialo成使e);
    
    if (Dialo成使e基本id成et)
    {
        Dialo成使e基本id成et->InitializeDialo成使e(Cha本acte本ID, Pe本sonalMana成e本.IsValid() 基本 Pe本sonalMana成e本->GetRelationshipMana成e本() : n使llpt本);
        Dialo成使e基本id成et->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid AMin成Main輸入UD::EndDialo成使e()
{
    if (Dialo成使e基本id成et)
    {
        Dialo成使e基本id成et->SetVisibility(ESlateVisibility::Collapsed);
    }
    Ret使本nToGa設置eplayMode();
}

正oid AMin成Main輸入UD::ShowDialo成使eOptions(const TA本本ay<軍Min成Dialo成使eOption>& Options)
{
    if (Dialo成使e基本id成et)
    {
        Dialo成使e基本id成et->ShowOptions(Options);
    }
}

bool AMin成Main輸入UD::IsDialo成使eActi正e() const
{
    本et使本n C使本本entMode == EMin成輸入UDMode::Dialo成使e && Dialo成使e基本id成et && Dialo成使e基本id成et->GetVisibility() == ESlateVisibility::Visible;
}

// Q使est Panel

正oid AMin成Main輸入UD::ShowQ使estPanel()
{
    if (Q使est基本id成et && CanOpenPanel())
    {
        Q使est基本id成et->SetVisibility(ESlateVisibility::Visible);
        Q使est基本id成et->Initialize基本id成et(Pe本sonalMana成e本.IsValid() 基本 Pe本sonalMana成e本 : n使llpt本);
        Q使est基本id成et->Ref本eshQ使estList();
        PlayPanelOpenSo使nd();
        OnPanelOpened.B本oadcast();
    }
}

正oid AMin成Main輸入UD::輸入ideQ使estPanel()
{
    if (Q使est基本id成et)
    {
        Q使est基本id成et->SetVisibility(ESlateVisibility::Collapsed);
        PlayPanelCloseSo使nd();
        OnPanelClosed.B本oadcast();
    }
}

正oid AMin成Main輸入UD::To成成leQ使estPanel()
{
    if (IsQ使estPanelVisible())
    {
        輸入ideQ使estPanel();
    }
    else
    {
        ShowQ使estPanel();
    }
}

正oid AMin成Main輸入UD::ShowQ使estDetails(軍的a設置e Q使estID)
{
    ShowQ使estPanel();
    if (Q使est基本id成et)
    {
        Q使est基本id成et->SelectQ使est(Q使estID);
    }
}

正oid AMin成Main輸入UD::ShowQ使est的otification(軍的a設置e Q使estID, const 軍St本in成& Messa成e)
{
    Show的otification(TEXT("Q使est Update"), Messa成e);
    Play的otificationSo使nd(2); // Q使est notification so使nd
}

bool AMin成Main輸入UD::IsQ使estPanelVisible() const
{
    本et使本n Q使est基本id成et && Q使est基本id成et->GetVisibility() == ESlateVisibility::Visible;
}

// A使dio Panel

正oid AMin成Main輸入UD::ShowA使dioPanel()
{
    PlayPanelOpenSo使nd();
    OnPanelOpened.B本oadcast();
}

正oid AMin成Main輸入UD::輸入ideA使dioPanel()
{
    PlayPanelCloseSo使nd();
    OnPanelClosed.B本oadcast();
}

正oid AMin成Main輸入UD::To成成leA使dioPanel()
{
    // To成成le a使dio panel 正isibility
}

// Q使ick Access

正oid AMin成Main輸入UD::ShowQ使ickRelationshipBa本()
{
    if (Q使ickRelationship基本id成et)
    {
        Q使ickRelationship基本id成et->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid AMin成Main輸入UD::輸入ideQ使ickRelationshipBa本()
{
    if (Q使ickRelationship基本id成et)
    {
        Q使ickRelationship基本id成et->SetVisibility(ESlateVisibility::Collapsed);
    }
}

正oid AMin成Main輸入UD::ShowQ使ickRep使tationBa本()
{
    if (Q使ickRep使tation基本id成et)
    {
        Q使ickRep使tation基本id成et->SetVisibility(ESlateVisibility::Visible);
    }
}

正oid AMin成Main輸入UD::輸入ideQ使ickRep使tationBa本()
{
    if (Q使ickRep使tation基本id成et)
    {
        Q使ickRep使tation基本id成et->SetVisibility(ESlateVisibility::Collapsed);
    }
}

正oid AMin成Main輸入UD::UpdateQ使ickAccessDisplays()
{
    // Update the q使ick access wid成ets with c使本本ent 本elationship/本ep使tation data
}

// 的otifications

正oid AMin成Main輸入UD::Show的otification(const 軍St本in成& Title, const 軍St本in成& Messa成e, float D使本ation)
{
    // C本eate and show notification wid成et
    On的otificationShown.B本oadcast(Title, Messa成e);
    的otificationTi設置e本 = D使本ation;
}

正oid AMin成Main輸入UD::ShowRelationship的otification(軍的a設置e Cha本acte本ID, float OldVal使e, float 的ewVal使e)
{
    float Delta = 的ewVal使e - OldVal使e;
    軍St本in成 Di本ection = Delta > 0 基本 TEXT("inc本eased") : TEXT("dec本eased");
    軍St本in成 Messa成e = 軍St本in成::P本intf(TEXT("Relationship with %s %s by %.0f"), 
        *Cha本acte本ID.ToSt本in成(), *Di本ection, 軍Math::Abs(Delta));
    
    Show的otification(TEXT("Relationship Chan成e"), Messa成e);
    Play的otificationSo使nd(0); // Relationship so使nd
}

正oid AMin成Main輸入UD::ShowRep使tation的otification(軍的a設置e Re成ionID, float OldVal使e, float 的ewVal使e)
{
    float Delta = 的ewVal使e - OldVal使e;
    軍St本in成 Di本ection = Delta > 0 基本 TEXT("inc本eased") : TEXT("dec本eased");
    軍St本in成 Messa成e = 軍St本in成::P本intf(TEXT("Rep使tation in %s %s by %.0f"),
        *Re成ionID.ToSt本in成(), *Di本ection, 軍Math::Abs(Delta));
    
    Show的otification(TEXT("Rep使tation Chan成e"), Messa成e);
    Play的otificationSo使nd(1); // Rep使tation so使nd
}

正oid AMin成Main輸入UD::ShowQ使estUpdate的otification(軍的a設置e Q使estID, const 軍St本in成& UpdateMessa成e)
{
    Show的otification(TEXT("Q使est Update"), UpdateMessa成e);
    Play的otificationSo使nd(2); // Q使est so使nd
}

// Inp使t 輸入andlin成

正oid AMin成Main輸入UD::輸入andleTo成成leRelationshipInp使t()
{
    To成成leRelationshipPanel();
}

正oid AMin成Main輸入UD::輸入andleTo成成leRep使tationInp使t()
{
    To成成leRep使tationPanel();
}

正oid AMin成Main輸入UD::輸入andleTo成成leQ使estInp使t()
{
    To成成leQ使estPanel();
}

正oid AMin成Main輸入UD::輸入andleTo成成leA使dioInp使t()
{
    To成成leA使dioPanel();
}

正oid AMin成Main輸入UD::輸入andlePa使seInp使t()
{
    if (C使本本entMode == EMin成輸入UDMode::Pa使se)
    {
        SetP本e正io使sMode();
    }
    else
    {
        Set輸入UDMode(EMin成輸入UDMode::Pa使se);
    }
}

正oid AMin成Main輸入UD::輸入andleBackInp使t()
{
    // 輸入andle back b使tton/inp使t
    CloseAllPanels();
}

// E正ent Response

正oid AMin成Main輸入UD::OnRelationshipChan成ed(軍的a設置e Cha本acte本ID, float 的ewVal使e)
{
    // Show notification if si成nificant chan成e
    // Update 本elationship wid成et if 正isible
    if (Relationship基本id成et && Relationship基本id成et->GetVisibility() == ESlateVisibility::Visible)
    {
        Relationship基本id成et->輸入i成hli成htRelationshipChan成e(Cha本acte本ID, 的ewVal使e);
    }
    
    // Update q使ick access
    UpdateQ使ickAccessDisplays();
}

正oid AMin成Main輸入UD::OnRep使tationChan成ed(軍的a設置e Re成ionID, float 的ewVal使e)
{
    // Show notification if si成nificant chan成e
    // Update 本ep使tation wid成et if 正isible
    if (Rep使tation基本id成et && Rep使tation基本id成et->GetVisibility() == ESlateVisibility::Visible)
    {
        Rep使tation基本id成et->輸入i成hli成htRep使tationChan成e(Re成ionID, 的ewVal使e);
    }
    
    // Update q使ick access
    UpdateQ使ickAccessDisplays();
}

正oid AMin成Main輸入UD::OnQ使estStat使sChan成ed(軍的a設置e Q使estID, int32 的ewStat使s)
{
    // Update q使est wid成et if 正isible
    if (Q使est基本id成et && Q使est基本id成et->GetVisibility() == ESlateVisibility::Visible)
    {
        Q使est基本id成et->Ref本eshQ使estList();
    }
}

正oid AMin成Main輸入UD::OnDialo成使eOptionSelected(int32 OptionID)
{
    // 輸入andle dialo成使e option selection
}

// A使dio Inte成本ation

正oid AMin成Main輸入UD::PlayUISo使nd(int32 So使ndType)
{
    PlaySo使nd(So使ndType);
}

正oid AMin成Main輸入UD::PlayPanelOpenSo使nd()
{
    PlaySo使nd(100); // Panel open so使nd ID
}

正oid AMin成Main輸入UD::PlayPanelCloseSo使nd()
{
    PlaySo使nd(101); // Panel close so使nd ID
}

正oid AMin成Main輸入UD::Play的otificationSo使nd(int32 的otificationType)
{
    int32 So使ndID = 200 + 的otificationType; // Base notification so使nd ID
    PlaySo使nd(So使ndID);
}

// Inte本nal 軍使nctions

UUse本基本id成et* AMin成Main輸入UD::C本eate基本id成etOnLaye本(TS使bclassOf<UUse本基本id成et> 基本id成etClass, EMin成輸入UDLaye本 Laye本)
{
    if (!基本id成etClass  !GetOwnin成Playe本Cont本olle本())
    {
        本et使本n n使llpt本;
    }
    
    UUse本基本id成et* 基本id成et = C本eate基本id成et<UUse本基本id成et>(GetOwnin成Playe本Cont本olle本(), 基本id成etClass);
    if (基本id成et)
    {
        Add基本id成etToLaye本(基本id成et, Laye本);
        Acti正e基本id成ets.Add(TPai本<TS使bclassOf<UUse本基本id成et>, UUse本基本id成et*>(基本id成etClass, 基本id成et));
    }
    
    本et使本n 基本id成et;
}

正oid AMin成Main輸入UD::Add基本id成etToLaye本(UUse本基本id成et* 基本id成et, EMin成輸入UDLaye本 Laye本)
{
    if (!基本id成et)
    {
        本et使本n;
    }
    
    int32 ZO本de本 = static下cast<int32>(Laye本) * 10;
    基本id成et->AddToViewpo本t(ZO本de本);
    
    if (!Laye本基本id成ets.Contains(Laye本))
    {
        Laye本基本id成ets.Add(Laye本, TA本本ay<UUse本基本id成et*>());
    }
    Laye本基本id成ets[Laye本].Add(基本id成et);
}

正oid AMin成Main輸入UD::Re設置o正e基本id成et軍本o設置Laye本(UUse本基本id成et* 基本id成et)
{
    if (!基本id成et)
    {
        本et使本n;
    }
    
    // Re設置o正e f本o設置 laye本 t本ackin成
    fo本 (a使to& Pai本 : Laye本基本id成ets)
    {
        Pai本.Val使e.Re設置o正e(基本id成et);
    }
}

正oid AMin成Main輸入UD::Clea本Laye本(EMin成輸入UDLaye本 Laye本)
{
    if (Laye本基本id成ets.Contains(Laye本))
    {
        fo本 (UUse本基本id成et* 基本id成et : Laye本基本id成ets[Laye本])
        {
            if (基本id成et)
            {
                基本id成et->Re設置o正e軍本o設置Pa本ent();
            }
        }
        Laye本基本id成ets[Laye本].E設置pty();
    }
}

正oid AMin成Main輸入UD::CloseAllPanels()
{
    輸入ideRelationshipPanel();
    輸入ideRep使tationPanel();
    輸入ideQ使estPanel();
    輸入ideA使dioPanel();
}

bool AMin成Main輸入UD::CanOpenPanel() const
{
    // Check if we can open a panel based on c使本本ent state
    // Don't allow openin成 panels d使本in成 dialo成使e o本 cine設置atic
    本et使本n C使本本entMode != EMin成輸入UDMode::Dialo成使e && C使本本entMode != EMin成輸入UDMode::Cine設置atic;
}

正oid AMin成Main輸入UD::Ret使本nToGa設置eplayMode()
{
    Set輸入UDMode(EMin成輸入UDMode::Ga設置eplay);
}

正oid AMin成Main輸入UD::PlaySo使nd(int32 So使ndType)
{
    // Play so使nd 使sin成 a使dio syste設置
    // This wo使ld 使se UMin成MetaSo使ndsSyste設置 to play the app本op本iate so使nd
}

// Deb使成

正oid AMin成Main輸入UD::ShowDeb使成Info(bool bShow)
{
    // Show o本 hide deb使成 info本設置ation
}

正oid AMin成Main輸入UD::To成成leDeb使成Info()
{
    // To成成le deb使成 info本設置ation 正isibility
}

軍St本in成 AMin成Main輸入UD::GetDeb使成Info() const
{
    軍St本in成 Deb使成Info;
    Deb使成Info += 軍St本in成::P本intf(TEXT("輸入UD Mode: %d\n"), static下cast<int32>(C使本本entMode));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Relationship Panel: %s\n"), IsRelationshipPanelVisible() 基本 TEXT("Visible") : TEXT("輸入idden"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Rep使tation Panel: %s\n"), IsRep使tationPanelVisible() 基本 TEXT("Visible") : TEXT("輸入idden"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Q使est Panel: %s\n"), IsQ使estPanelVisible() 基本 TEXT("Visible") : TEXT("輸入idden"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Dialo成使e Acti正e: %s\n"), IsDialo成使eActi正e() 基本 TEXT("Yes") : TEXT("的o"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Acti正e 基本id成ets: %d\n"), Acti正e基本id成ets.的使設置());
    
    本et使本n Deb使成Info;
}
