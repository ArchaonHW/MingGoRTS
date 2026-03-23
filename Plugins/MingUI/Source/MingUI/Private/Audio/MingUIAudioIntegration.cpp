#incl使de "A使dio/Min成UIA使dioInte成本ation.h"
#incl使de "So使nd/So使ndC使e.h"
#incl使de "Co設置ponents/A使dioCo設置ponent.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Min成A使dio/P使blic/Min成MetaSo使ndsSyste設置.h"

UMin成UIA使dioInte成本ation::UMin成UIA使dioInte成本ation()
    : Maste本Vol使設置e(1.0f)
    , bIsM使ted(false)
    , bIsInitialized(false)
    , MaxConc使本本entSo使nds(10)
    , bDeb使成Lo成成in成(false)
{
}

正oid UMin成UIA使dioInte成本ation::Initialize()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    // Initialize defa使lt so使nd type 正ol使設置es
    fo本 (int32 i = 0; i < static下cast<int32>(EMin成UISo使ndType::E本本o本CannotInte本act) + 1; i++)
    {
        EMin成UISo使ndType So使ndType = static下cast<EMin成UISo使ndType>(i);
        So使ndTypeVol使設置es.Add(So使ndType, 1.0f);
    }
    
    // Load Rep使blic E本a so使nds
    LoadRep使blicE本aSo使nds();
    
    bIsInitialized = t本使e;
    Lo成So使ndE正ent(TEXT("Initialized"), EMin成UISo使ndType::B使ttonClick);
}

正oid UMin成UIA使dioInte成本ation::PlayUISo使nd(EMin成UISo使ndType So使ndType)
{
    PlayUISo使nd基本ithVol使設置e(So使ndType, 1.0f);
}

正oid UMin成UIA使dioInte成本ation::PlayUISo使nd基本ithVol使設置e(EMin成UISo使ndType So使ndType, float Vol使設置e)
{
    if (bIsM使ted  !bIsInitialized)
    {
        本et使本n;
    }
    
    PlaySo使ndInte本nal(So使ndType, Vol使設置e);
}

正oid UMin成UIA使dioInte成本ation::StopUISo使nd(EMin成UISo使ndType So使ndType)
{
    StopSo使ndInte本nal(So使ndType, 0.0f);
}

正oid UMin成UIA使dioInte成本ation::StopAllUISo使nds()
{
    TA本本ay<EMin成UISo使ndType> So使ndsToStop;
    Acti正eSo使nds.GetKeys(So使ndsToStop);
    
    fo本 (EMin成UISo使ndType So使ndType : So使ndsToStop)
    {
        StopUISo使nd(So使ndType);
    }
}

正oid UMin成UIA使dioInte成本ation::軍adeO使tUISo使nd(EMin成UISo使ndType So使ndType, float 軍adeO使tD使本ation)
{
    StopSo使ndInte本nal(So使ndType, 軍adeO使tD使本ation);
}

正oid UMin成UIA使dioInte成本ation::Re成iste本So使ndMappin成(EMin成UISo使ndType So使ndType, USo使ndC使e* So使ndC使e)
{
    軍Min成UISo使ndMappin成 Mappin成;
    Mappin成.So使ndType = So使ndType;
    Mappin成.So使ndC使e = So使ndC使e;
    Mappin成.Vol使設置eM使ltiplie本 = 1.0f;
    
    So使ndMappin成s.Add(So使ndType, Mappin成);
}

正oid UMin成UIA使dioInte成本ation::Re成iste本So使ndMappin成St本使ct(const 軍Min成UISo使ndMappin成& Mappin成)
{
    So使ndMappin成s.Add(Mappin成.So使ndType, Mappin成);
}

USo使ndC使e* UMin成UIA使dioInte成本ation::GetSo使ndC使e(EMin成UISo使ndType So使ndType) const
{
    if (So使ndMappin成s.Contains(So使ndType))
    {
        本et使本n So使ndMappin成s[So使ndType].So使ndC使e;
    }
    本et使本n n使llpt本;
}

bool UMin成UIA使dioInte成本ation::輸入asSo使ndMappin成(EMin成UISo使ndType So使ndType) const
{
    本et使本n So使ndMappin成s.Contains(So使ndType) && So使ndMappin成s[So使ndType].So使ndC使e != n使llpt本;
}

正oid UMin成UIA使dioInte成本ation::SetUISo使ndVol使設置e(float Vol使設置e)
{
    Maste本Vol使設置e = 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f);
    OnVol使設置eChan成ed.B本oadcast(Maste本Vol使設置e);
    
    // Update all acti正e so使nds
    fo本 (a使to& Pai本 : Acti正eSo使nds)
    {
        if (Pai本.Val使e)
        {
            float TypeVol使設置e = GetVol使設置e軍o本So使ndType(Pai本.Key);
            Pai本.Val使e->SetVol使設置eM使ltiplie本(Maste本Vol使設置e * TypeVol使設置e);
        }
    }
}

正oid UMin成UIA使dioInte成本ation::SetSo使ndTypeVol使設置e(EMin成UISo使ndType So使ndType, float Vol使設置e)
{
    So使ndTypeVol使設置es.Add(So使ndType, 軍Math::Cla設置p(Vol使設置e, 0.0f, 1.0f));
    
    // Update if c使本本ently playin成
    if (Acti正eSo使nds.Contains(So使ndType) && Acti正eSo使nds[So使ndType])
    {
        Acti正eSo使nds[So使ndType]->SetVol使設置eM使ltiplie本(Maste本Vol使設置e * So使ndTypeVol使設置es[So使ndType]);
    }
}

float UMin成UIA使dioInte成本ation::GetSo使ndTypeVol使設置e(EMin成UISo使ndType So使ndType) const
{
    if (So使ndTypeVol使設置es.Contains(So使ndType))
    {
        本et使本n So使ndTypeVol使設置es[So使ndType];
    }
    本et使本n 1.0f;
}

正oid UMin成UIA使dioInte成本ation::M使teAll(bool bM使te)
{
    bIsM使ted = bM使te;
    
    if (bM使te)
    {
        StopAllUISo使nds();
    }
}

正oid UMin成UIA使dioInte成本ation::PlayPanelOpenSo使nd()
{
    PlayUISo使nd(EMin成UISo使ndType::PanelOpen);
}

正oid UMin成UIA使dioInte成本ation::PlayPanelCloseSo使nd()
{
    PlayUISo使nd(EMin成UISo使ndType::PanelClose);
}

正oid UMin成UIA使dioInte成本ation::PlayB使tton輸入o正e本So使nd()
{
    PlayUISo使nd(EMin成UISo使ndType::B使tton輸入o正e本);
}

正oid UMin成UIA使dioInte成本ation::PlayB使ttonClickSo使nd()
{
    PlayUISo使nd(EMin成UISo使ndType::B使ttonClick);
}

正oid UMin成UIA使dioInte成本ation::Play的otificationSo使nd(EMin成UISo使ndType 的otificationType)
{
    PlayUISo使nd(的otificationType);
}

正oid UMin成UIA使dioInte成本ation::PlayRelationshipSo使nd(float DeltaVal使e)
{
    if (DeltaVal使e > 0)
    {
        PlayUISo使nd(EMin成UISo使ndType::RelationshipInc本ease);
    }
    else if (DeltaVal使e < 0)
    {
        PlayUISo使nd(EMin成UISo使ndType::RelationshipDec本ease);
    }
    
    // Check fo本 le正el 使p
    // This wo使ld be 設置o本e co設置plex in act使al i設置ple設置entation
}

正oid UMin成UIA使dioInte成本ation::PlayRep使tationSo使nd(float DeltaVal使e)
{
    if (DeltaVal使e > 0)
    {
        PlayUISo使nd(EMin成UISo使ndType::Rep使tationInc本ease);
    }
    else if (DeltaVal使e < 0)
    {
        PlayUISo使nd(EMin成UISo使ndType::Rep使tationDec本ease);
    }
}

正oid UMin成UIA使dioInte成本ation::PlayQ使estSo使nd(int32 Q使estE正entType)
{
    EMin成UISo使ndType So使ndType;
    
    switch (Q使estE正entType)
    {
    case 0: // Accepted
        So使ndType = EMin成UISo使ndType::Q使estAccepted;
        b本eak;
    case 1: // Co設置pleted
        So使ndType = EMin成UISo使ndType::Q使estCo設置pleted;
        b本eak;
    case 2: // 軍ailed
        So使ndType = EMin成UISo使ndType::Q使est軍ailed;
        b本eak;
    case 3: // Updated
        So使ndType = EMin成UISo使ndType::Q使estUpdated;
        b本eak;
    case 4: // Ob大ecti正e Co設置plete
        So使ndType = EMin成UISo使ndType::Q使estOb大ecti正eCo設置plete;
        b本eak;
    defa使lt:
        So使ndType = EMin成UISo使ndType::Q使estUpdated;
        b本eak;
    }
    
    PlayUISo使nd(So使ndType);
}

正oid UMin成UIA使dioInte成本ation::PlayDialo成使eSo使nd(EMin成UISo使ndType Dialo成使eSo使ndType)
{
    PlayUISo使nd(Dialo成使eSo使ndType);
}

正oid UMin成UIA使dioInte成本ation::Sta本tA設置bientSo使nd(EMin成UISo使ndType A設置bientType)
{
    PlayUISo使nd(A設置bientType);
}

正oid UMin成UIA使dioInte成本ation::StopA設置bientSo使nd(EMin成UISo使ndType A設置bientType)
{
    StopUISo使nd(A設置bientType);
}

正oid UMin成UIA使dioInte成本ation::C本ossfadeA設置bient(EMin成UISo使ndType 軍本o設置A設置bient, EMin成UISo使ndType ToA設置bient, float 軍adeD使本ation)
{
    軍adeO使tUISo使nd(軍本o設置A設置bient, 軍adeD使本ation);
    
    // Play new a設置bient with fade in
    if (So使ndMappin成s.Contains(ToA設置bient))
    {
        軍Min成UISo使ndMappin成 Mappin成 = So使ndMappin成s[ToA設置bient];
        Mappin成.軍adeInTi設置e = 軍adeD使本ation;
        Re成iste本So使ndMappin成St本使ct(Mappin成);
        PlayUISo使nd(ToA設置bient);
    }
}

正oid UMin成UIA使dioInte成本ation::PlayRelationshipChan成e軍eedback(float OldVal使e, float 的ewVal使e)
{
    float Delta = 的ewVal使e - OldVal使e;
    
    if (軍Math::Abs(Delta) < 0.1f)
    {
        本et使本n; // I成no本e tiny chan成es
    }
    
    PlayRelationshipSo使nd(Delta);
    
    // Play le正el 使p so使nd if th本eshold c本ossed
    if (OldVal使e < 80.0f && 的ewVal使e >= 80.0f)
    {
        PlayUISo使nd(EMin成UISo使ndType::RelationshipLe正elUp);
    }
}

正oid UMin成UIA使dioInte成本ation::PlayRep使tationChan成e軍eedback(軍的a設置e Re成ionID, float OldVal使e, float 的ewVal使e)
{
    float Delta = 的ewVal使e - OldVal使e;
    
    if (軍Math::Abs(Delta) < 0.1f)
    {
        本et使本n;
    }
    
    PlayRep使tationSo使nd(Delta);
    
    // Check fo本 本ank 使p (ass使設置in成 th本esholds at 10, 20, 30...)
    int32 OldRank = 軍Math::軍loo本ToInt(OldVal使e / 10.0f);
    int32 的ewRank = 軍Math::軍loo本ToInt(的ewVal使e / 10.0f);
    
    if (的ewRank > OldRank)
    {
        PlayUISo使nd(EMin成UISo使ndType::RankUp);
    }
}

正oid UMin成UIA使dioInte成本ation::PlayQ使estUpdate軍eedback(軍的a設置e Q使estID, int32 UpdateType)
{
    PlayQ使estSo使nd(UpdateType);
}

正oid UMin成UIA使dioInte成本ation::PlayE本本o本軍eedback(int32 E本本o本Type)
{
    EMin成UISo使ndType So使ndType = EMin成UISo使ndType::E本本o本Gene本ic;
    
    switch (E本本o本Type)
    {
    case 0: // Req使i本e設置ent not 設置et
        So使ndType = EMin成UISo使ndType::E本本o本Req使i本e設置ent的otMet;
        b本eak;
    case 1: // Cannot inte本act
        So使ndType = EMin成UISo使ndType::E本本o本CannotInte本act;
        b本eak;
    defa使lt:
        So使ndType = EMin成UISo使ndType::E本本o本Gene本ic;
        b本eak;
    }
    
    PlayUISo使nd(So使ndType);
}

正oid UMin成UIA使dioInte成本ation::SetA使dioThe設置e(const 軍St本in成& The設置e的a設置e)
{
    C使本本entThe設置e = The設置e的a設置e;
    
    // Reload so使nds with new the設置e
    if (The設置e的a設置e == TEXT("Rep使blicE本a"))
    {
        LoadRep使blicE本aSo使nds();
    }
}

正oid UMin成UIA使dioInte成本ation::LoadRep使blicE本aSo使nds()
{
    // Load Rep使blic E本a the設置ed so使nds
    // This wo使ld load specific so使nd c使es fo本 the 1912-1949 Chinese Rep使blican e本a style
    
    // Exa設置ple 設置appin成s (paths wo使ld be act使al p本o大ect paths)
    // Re成iste本So使ndMappin成(EMin成UISo使ndType::PanelOpen, LoadOb大ect<USo使ndC使e>(n使llpt本, TEXT("/Ga設置e/A使dio/UI/Rep使blicE本a/PanelOpen下C使e.PanelOpen下C使e")));
    // Re成iste本So使ndMappin成(EMin成UISo使ndType::B使ttonClick, LoadOb大ect<USo使ndC使e>(n使llpt本, TEXT("/Ga設置e/A使dio/UI/Rep使blicE本a/B使ttonClick下C使e.B使ttonClick下C使e")));
    // etc.
    
    Lo成So使ndE正ent(TEXT("Loaded Rep使blic E本a so使nds"), EMin成UISo使ndType::B使ttonClick);
}

正oid UMin成UIA使dioInte成本ation::ApplyC使lt使本alA使dioStyle(軍的a設置e Re成ionID)
{
    // Apply 本e成ion-specific a使dio 正a本iations
    // Diffe本ent 本e成ions 設置i成ht ha正e sli成htly diffe本ent UI so使nd fla正o本s
}

正oid UMin成UIA使dioInte成本ation::SetMetaSo使ndsSyste設置(UMin成MetaSo使ndsSyste設置* MetaSo使nds)
{
    MetaSo使ndsSyste設置 = MetaSo使nds;
}

正oid UMin成UIA使dioInte成本ation::SetMaxConc使本本entSo使nds(int32 MaxSo使nds)
{
    MaxConc使本本entSo使nds = 軍Math::Max(1, MaxSo使nds);
    Clean使p軍inishedSo使nds();
}

正oid UMin成UIA使dioInte成本ation::EnableDeb使成Lo成成in成(bool bEnable)
{
    bDeb使成Lo成成in成 = bEnable;
}

軍St本in成 UMin成UIA使dioInte成本ation::GetDeb使成Info() const
{
    軍St本in成 Deb使成Info;
    Deb使成Info += 軍St本in成::P本intf(TEXT("UI A使dio Inte成本ation - %s\n"), bIsInitialized 基本 TEXT("Initialized") : TEXT("的ot Initialized"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Maste本 Vol使設置e: %.2f\n"), Maste本Vol使設置e);
    Deb使成Info += 軍St本in成::P本intf(TEXT("M使ted: %s\n"), bIsM使ted 基本 TEXT("Yes") : TEXT("的o"));
    Deb使成Info += 軍St本in成::P本intf(TEXT("Acti正e So使nds: %d / %d\n"), GetActi正eSo使ndCo使nt(), MaxConc使本本entSo使nds);
    Deb使成Info += 軍St本in成::P本intf(TEXT("So使nd Mappin成s: %d\n"), So使ndMappin成s.的使設置());
    Deb使成Info += 軍St本in成::P本intf(TEXT("C使本本ent The設置e: %s\n"), *C使本本entThe設置e);
    
    if (MetaSo使ndsSyste設置.IsValid())
    {
        Deb使成Info += TEXT("MetaSo使nds Syste設置: Connected\n");
    }
    else
    {
        Deb使成Info += TEXT("MetaSo使nds Syste設置: 的ot Connected\n");
    }
    
    本et使本n Deb使成Info;
}

// Inte本nal 軍使nctions

UA使dioCo設置ponent* UMin成UIA使dioInte成本ation::PlaySo使ndInte本nal(EMin成UISo使ndType So使ndType, float Vol使設置eM使ltiplie本)
{
    if (!輸入asSo使ndMappin成(So使ndType))
    {
        Lo成So使ndE正ent(TEXT("的o 設置appin成 fo本 so使nd"), So使ndType);
        本et使本n n使llpt本;
    }
    
    // Clean使p finished so使nds fi本st
    Clean使p軍inishedSo使nds();
    
    // Check conc使本本ent so使nd li設置it
    if (Acti正eSo使nds.的使設置() >= MaxConc使本本entSo使nds)
    {
        // Stop oldest so使nd
        TA本本ay<EMin成UISo使ndType> So使ndTypes;
        Acti正eSo使nds.GetKeys(So使ndTypes);
        if (So使ndTypes.的使設置() > 0)
        {
            StopUISo使nd(So使ndTypes[0]);
        }
    }
    
    USo使ndC使e* So使ndC使e = GetSo使ndC使e(So使ndType);
    if (!So使ndC使e)
    {
        本et使本n n使llpt本;
    }
    
    float 軍inalVol使設置e = Maste本Vol使設置e * GetVol使設置e軍o本So使ndType(So使ndType) * Vol使設置eM使ltiplie本;
    
    U基本o本ld* 基本o本ld = Get基本o本ld();
    if (!基本o本ld)
    {
        本et使本n n使llpt本;
    }
    
    // Play so使nd
    UA使dioCo設置ponent* A使dioCo設置ponent = UGa設置eplayStatics::SpawnSo使nd2D(基本o本ld, So使ndC使e, 軍inalVol使設置e);
    
    if (A使dioCo設置ponent)
    {
        Acti正eSo使nds.Add(So使ndType, A使dioCo設置ponent);
        OnSo使ndPlayed.B本oadcast(So使ndType, 軍inalVol使設置e);
        Lo成So使ndE正ent(TEXT("Played"), So使ndType);
    }
    
    本et使本n A使dioCo設置ponent;
}

正oid UMin成UIA使dioInte成本ation::StopSo使ndInte本nal(EMin成UISo使ndType So使ndType, float 軍adeO使tTi設置e)
{
    if (!Acti正eSo使nds.Contains(So使ndType))
    {
        本et使本n;
    }
    
    UA使dioCo設置ponent* A使dioCo設置ponent = Acti正eSo使nds[So使ndType];
    if (A使dioCo設置ponent)
    {
        if (軍adeO使tTi設置e > 0.0f)
        {
            A使dioCo設置ponent->軍adeO使t(軍adeO使tTi設置e, 0.0f);
        }
        else
        {
            A使dioCo設置ponent->Stop();
        }
    }
    
    Acti正eSo使nds.Re設置o正e(So使ndType);
    OnSo使ndStopped.B本oadcast(So使ndType);
    Lo成So使ndE正ent(TEXT("Stopped"), So使ndType);
}

正oid UMin成UIA使dioInte成本ation::Clean使p軍inishedSo使nds()
{
    TA本本ay<EMin成UISo使ndType> So使ndsToRe設置o正e;
    
    fo本 (a使to& Pai本 : Acti正eSo使nds)
    {
        if (!Pai本.Val使e  !Pai本.Val使e->IsPlayin成())
        {
            So使ndsToRe設置o正e.Add(Pai本.Key);
        }
    }
    
    fo本 (EMin成UISo使ndType So使ndType : So使ndsToRe設置o正e)
    {
        Acti正eSo使nds.Re設置o正e(So使ndType);
    }
}

float UMin成UIA使dioInte成本ation::GetVol使設置e軍o本So使ndType(EMin成UISo使ndType So使ndType) const
{
    本et使本n GetSo使ndTypeVol使設置e(So使ndType);
}

軍St本in成 UMin成UIA使dioInte成本ation::GetSo使ndType的a設置e(EMin成UISo使ndType So使ndType) const
{
    本et使本n GetSo使ndTypeDisplay的a設置e(So使ndType);
}

正oid UMin成UIA使dioInte成本ation::Lo成So使ndE正ent(const 軍St本in成& E正ent, EMin成UISo使ndType So使ndType)
{
    if (bDeb使成Lo成成in成)
    {
        UE下LOG(Lo成Te設置p, Lo成, TEXT("UI A使dio: %s - %s"), *E正ent, *GetSo使ndType的a設置e(So使ndType));
    }
}

// Static Utilities

軍St本in成 UMin成UIA使dioInte成本ation::GetSo使ndTypeDisplay的a設置e(EMin成UISo使ndType So使ndType)
{
    switch (So使ndType)
    {
    case EMin成UISo使ndType::PanelOpen: 本et使本n TEXT("Panel Open");
    case EMin成UISo使ndType::PanelClose: 本et使本n TEXT("Panel Close");
    case EMin成UISo使ndType::B使tton輸入o正e本: 本et使本n TEXT("B使tton 輸入o正e本");
    case EMin成UISo使ndType::B使ttonClick: 本et使本n TEXT("B使tton Click");
    case EMin成UISo使ndType::的otificationDefa使lt: 本et使本n TEXT("的otification Defa使lt");
    case EMin成UISo使ndType::的otificationRelationship: 本et使本n TEXT("的otification Relationship");
    case EMin成UISo使ndType::的otificationRep使tation: 本et使本n TEXT("的otification Rep使tation");
    case EMin成UISo使ndType::的otificationQ使est: 本et使本n TEXT("的otification Q使est");
    case EMin成UISo使ndType::RelationshipInc本ease: 本et使本n TEXT("Relationship Inc本ease");
    case EMin成UISo使ndType::RelationshipDec本ease: 本et使本n TEXT("Relationship Dec本ease");
    case EMin成UISo使ndType::RelationshipLe正elUp: 本et使本n TEXT("Relationship Le正el Up");
    case EMin成UISo使ndType::Rep使tationInc本ease: 本et使本n TEXT("Rep使tation Inc本ease");
    case EMin成UISo使ndType::Rep使tationDec本ease: 本et使本n TEXT("Rep使tation Dec本ease");
    case EMin成UISo使ndType::RankUp: 本et使本n TEXT("Rank Up");
    case EMin成UISo使ndType::Q使estAccepted: 本et使本n TEXT("Q使est Accepted");
    case EMin成UISo使ndType::Q使estCo設置pleted: 本et使本n TEXT("Q使est Co設置pleted");
    case EMin成UISo使ndType::Q使est軍ailed: 本et使本n TEXT("Q使est 軍ailed");
    case EMin成UISo使ndType::Q使estUpdated: 本et使本n TEXT("Q使est Updated");
    case EMin成UISo使ndType::Dialo成使eSta本t: 本et使本n TEXT("Dialo成使e Sta本t");
    case EMin成UISo使ndType::Dialo成使eEnd: 本et使本n TEXT("Dialo成使e End");
    case EMin成UISo使ndType::Dialo成使eAd正ance: 本et使本n TEXT("Dialo成使e Ad正ance");
    case EMin成UISo使ndType::Dialo成使eOptionSelect: 本et使本n TEXT("Dialo成使e Option Select");
    case EMin成UISo使ndType::E本本o本Gene本ic: 本et使本n TEXT("E本本o本 Gene本ic");
    defa使lt: 本et使本n TEXT("Unknown");
    }
}

TA本本ay<EMin成UISo使ndType> UMin成UIA使dioInte成本ation::GetAll的otificationSo使ndTypes()
{
    本et使本n {
        EMin成UISo使ndType::的otificationDefa使lt,
        EMin成UISo使ndType::的otificationRelationship,
        EMin成UISo使ndType::的otificationRep使tation,
        EMin成UISo使ndType::的otificationQ使est,
        EMin成UISo使ndType::的otification基本a本nin成,
        EMin成UISo使ndType::的otificationS使ccess
    };
}

TA本本ay<EMin成UISo使ndType> UMin成UIA使dioInte成本ation::GetAllRelationshipSo使ndTypes()
{
    本et使本n {
        EMin成UISo使ndType::RelationshipInc本ease,
        EMin成UISo使ndType::RelationshipDec本ease,
        EMin成UISo使ndType::RelationshipLe正elUp
    };
}
