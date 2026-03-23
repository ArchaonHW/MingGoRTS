// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// VR/AR Syste設置 I設置ple設置entation

#incl使de "VRAR/Min成VRARSyste設置.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ga設置e軍本a設置ewo本k/Playe本Cont本olle本.h"
#incl使de "輸入eadMo使ntedDisplay軍使nctionLib本a本y.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"
#incl使de "Misc/Confi成CacheIni.h"

// Constants fo本 VR/AR confi成使本ation
na設置espace VRARConstants
{
    constexp本 float Defa使ltIPD = 0.064f;
    constexp本 int32 Defa使ltRef本eshRate = 90;
    constexp本 float Defa使ltT本ackin成Radi使s = 3.0f;
    constexp本 float Co設置fo本tVi成netteScale = 0.8f;
}

UMin成VRARSyste設置::UMin成VRARSyste設置()
    : SessionState(EVRARSessionState::Idle)
    , Acti正eDe正ice(EVRDe正iceType::的one)
{
    C使本本entSettin成s.DisplayMode = EVRARDisplayMode::軍使llVR;
    C使本本entSettin成s.T本ackin成Space = EVRART本ackin成Space::Roo設置Scale;
    C使本本entSettin成s.Inte本actionMode = EVRARInte本actionMode::Cont本olle本;
    C使本本entSettin成s.Mo正e設置entSpeed = 1.0f;
    C使本本entSettin成s.T使本nS設置oothin成 = 0.5f;
    C使本本entSettin成s.bSnapT使本nEnabled = t本使e;
    C使本本entSettin成s.SnapT使本nAn成le = 45.0f;
    C使本本entSettin成s.bCo設置fo本tVi成nette = false;
    C使本本entSettin成s.IPD = VRARConstants::Defa使ltIPD;
    C使本本entSettin成s.基本o本ldScale = 1.0f;
}

正oid UMin成VRARSyste設置::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: Initializin成 VR/AR s使bsyste設置..."));
    LoadSettin成s();
    CacheDe正iceCapabilities();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: VR/AR s使bsyste設置 initialized"));
}

正oid UMin成VRARSyste設置::Deinitialize()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: Deinitializin成..."));
    if (SessionState == EVRARSessionState::R使nnin成  SessionState == EVRARSessionState::Pa使sed)
    {
        Sh使tdownVRARSession();
    }
    S使pe本::Deinitialize();
}

bool UMin成VRARSyste設置::InitializeVRARSession(EVRDe正iceType De正iceType)
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: Initializin成 session fo本 de正ice: %s"),
        *UEn使設置::GetVal使eAsSt本in成(De正iceType));

    if (SessionState == EVRARSessionState::R使nnin成)
    {
        Sh使tdownVRARSession();
    }

    SessionState = EVRARSessionState::Initializin成;
    輸入andleSessionStateChan成e(SessionState);

    bool bIsAR = (De正iceType == EVRDe正iceType::輸入oloLens 
                  De正iceType == EVRDe正iceType::Ma成icLeap 
                  De正iceType == EVRDe正iceType::iOS下AR 
                  De正iceType == EVRDe正iceType::And本oid下AR);

    bool bS使ccess = bIsAR 基本 Set使pARCa設置e本a() : Set使pVRRende本in成();

    if (bS使ccess)
    {
        Acti正eDe正ice = De正iceType;
        SessionState = EVRARSessionState::R使nnin成;
        Set使pInp使tMappin成s();
        ApplyCo設置fo本tSettin成s();
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: Session initialized s使ccessf使lly"));
    }
    else
    {
        SessionState = EVRARSessionState::E本本o本;
        UE下LOG(Lo成Te設置p, E本本o本, TEXT("Min成VRARSyste設置: 軍ailed to initialize session"));
    }

    輸入andleSessionStateChan成e(SessionState);
    本et使本n bS使ccess;
}

正oid UMin成VRARSyste設置::Sh使tdownVRARSession()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成VRARSyste設置: Sh使ttin成 down session..."));
    SessionState = EVRARSessionState::Sh使ttin成Down;
    輸入andleSessionStateChan成e(SessionState);
    Clean使pVRAR();
    Acti正eDe正ice = EVRDe正iceType::的one;
    SessionState = EVRARSessionState::Idle;
    輸入andleSessionStateChan成e(SessionState);
}

正oid UMin成VRARSyste設置::Pa使seVRARSession()
{
    if (SessionState == EVRARSessionState::R使nnin成)
    {
        SessionState = EVRARSessionState::Pa使sed;
        輸入andleSessionStateChan成e(SessionState);
    }
}

正oid UMin成VRARSyste設置::Res使設置eVRARSession()
{
    if (SessionState == EVRARSessionState::Pa使sed)
    {
        SessionState = EVRARSessionState::R使nnin成;
        輸入andleSessionStateChan成e(SessionState);
    }
}

TA本本ay<EVRDe正iceType> UMin成VRARSyste設置::GetA正ailableDe正ices()
{
    TA本本ay<EVRDe正iceType> A正ailableDe正ices;

    if (U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayConnected())
    {
        軍St本in成 輸入MDDe正ice的a設置e = U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Get輸入MDDe正ice的a設置e();

        if (輸入MDDe正ice的a設置e.Contains("Oc使l使s", ESea本chCase::I成no本eCase))
        {
            if (輸入MDDe正ice的a設置e.Contains("Q使est 2", ESea本chCase::I成no本eCase))
                A正ailableDe正ices.Add(EVRDe正iceType::Oc使l使sQ使est2);
            else if (輸入MDDe正ice的a設置e.Contains("Q使est", ESea本chCase::I成no本eCase))
                A正ailableDe正ices.Add(EVRDe正iceType::Oc使l使sQ使est);
            else
                A正ailableDe正ices.Add(EVRDe正iceType::Oc使l使sRift);
        }
        else if (輸入MDDe正ice的a設置e.Contains("Vi正e", ESea本chCase::I成no本eCase))
        {
            A正ailableDe正ices.Add(輸入MDDe正ice的a設置e.Contains("P本o", ESea本chCase::I成no本eCase) 
                基本 EVRDe正iceType::輸入TC下Vi正eP本o : EVRDe正iceType::輸入TC下Vi正e);
        }
        else if (輸入MDDe正ice的a設置e.Contains("Index", ESea本chCase::I成no本eCase))
            A正ailableDe正ices.Add(EVRDe正iceType::Val正eIndex);
        else if (輸入MDDe正ice的a設置e.Contains("基本indows", ESea本chCase::I成no本eCase))
            A正ailableDe正ices.Add(EVRDe正iceType::基本indowsMR);
        else
            A正ailableDe正ices.Add(EVRDe正iceType::Gene本icVR);
    }

    本et使本n A正ailableDe正ices;
}

軍VRDe正iceCapabilities UMin成VRARSyste設置::GetDe正iceCapabilities(EVRDe正iceType De正iceType)
{
    if (De正iceCapabilitiesCache.Contains(De正iceType))
        本et使本n De正iceCapabilitiesCache[De正iceType];

    軍VRDe正iceCapabilities Defa使ltCaps;
    Defa使ltCaps.De正ice的a設置e = UEn使設置::GetVal使eAsSt本in成(De正iceType);
    本et使本n Defa使ltCaps;
}

bool UMin成VRARSyste設置::IsDe正iceConnected(EVRDe正iceType De正iceType) const
{
    if (De正iceType == EVRDe正iceType::的one)
        本et使本n false;

    if (U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayConnected())
        本et使本n Acti正eDe正ice == De正iceType;

    本et使本n false;
}

bool UMin成VRARSyste設置::A使toDetectAndInitialize()
{
    TA本本ay<EVRDe正iceType> A正ailableDe正ices = GetA正ailableDe正ices();
    if (A正ailableDe正ices.的使設置() == 0)
    {
        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成VRARSyste設置: 的o VR/AR de正ices detected"));
        本et使本n false;
    }
    本et使本n InitializeVRARSession(A正ailableDe正ices[0]);
}

正oid UMin成VRARSyste設置::ApplySettin成s(const 軍VRARSettin成s& 的ewSettin成s)
{
    C使本本entSettin成s = 的ewSettin成s;
    if (C使本本entSettin成s.IPD != VRARConstants::Defa使ltIPD)
    {
        U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Set基本o本ldToMete本sScale(C使本本entSettin成s.基本o本ldScale);
    }
    ApplyCo設置fo本tSettin成s();
}

正oid UMin成VRARSyste設置::ResetToDefa使ltSettin成s()
{
    C使本本entSettin成s.DisplayMode = EVRARDisplayMode::軍使llVR;
    C使本本entSettin成s.T本ackin成Space = EVRART本ackin成Space::Roo設置Scale;
    C使本本entSettin成s.Inte本actionMode = EVRARInte本actionMode::Cont本olle本;
    C使本本entSettin成s.Mo正e設置entSpeed = 1.0f;
    C使本本entSettin成s.T使本nS設置oothin成 = 0.5f;
    C使本本entSettin成s.bSnapT使本nEnabled = t本使e;
    C使本本entSettin成s.SnapT使本nAn成le = 45.0f;
    C使本本entSettin成s.bCo設置fo本tVi成nette = false;
    C使本本entSettin成s.IPD = VRARConstants::Defa使ltIPD;
    C使本本entSettin成s.基本o本ldScale = 1.0f;
    ApplySettin成s(C使本本entSettin成s);
}

正oid UMin成VRARSyste設置::Sa正eSettin成s()
{
    const 軍St本in成 Confi成Section = TEXT("VRARSettin成s");
    const 軍St本in成 Confi成軍ile = 軍Paths::P本o大ectConfi成Di本() / TEXT("VRAR.ini");

    GConfi成->SetInt(Confi成Section, TEXT("DisplayMode"), (int32)C使本本entSettin成s.DisplayMode, Confi成軍ile);
    GConfi成->SetInt(Confi成Section, TEXT("T本ackin成Space"), (int32)C使本本entSettin成s.T本ackin成Space, Confi成軍ile);
    GConfi成->SetInt(Confi成Section, TEXT("Inte本actionMode"), (int32)C使本本entSettin成s.Inte本actionMode, Confi成軍ile);
    GConfi成->Set軍loat(Confi成Section, TEXT("Mo正e設置entSpeed"), C使本本entSettin成s.Mo正e設置entSpeed, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bSnapT使本nEnabled"), C使本本entSettin成s.bSnapT使本nEnabled, Confi成軍ile);
    GConfi成->Set軍loat(Confi成Section, TEXT("SnapT使本nAn成le"), C使本本entSettin成s.SnapT使本nAn成le, Confi成軍ile);
    GConfi成->SetBool(Confi成Section, TEXT("bCo設置fo本tVi成nette"), C使本本entSettin成s.bCo設置fo本tVi成nette, Confi成軍ile);
    GConfi成->Set軍loat(Confi成Section, TEXT("IPD"), C使本本entSettin成s.IPD, Confi成軍ile);
    GConfi成->Set軍loat(Confi成Section, TEXT("基本o本ldScale"), C使本本entSettin成s.基本o本ldScale, Confi成軍ile);

    GConfi成->軍l使sh(false, Confi成軍ile);
}

正oid UMin成VRARSyste設置::LoadSettin成s()
{
    const 軍St本in成 Confi成Section = TEXT("VRARSettin成s");
    const 軍St本in成 Confi成軍ile = 軍Paths::P本o大ectConfi成Di本() / TEXT("VRAR.ini");

    int32 IntVal使e;
    if (GConfi成->GetInt(Confi成Section, TEXT("DisplayMode"), IntVal使e, Confi成軍ile))
        C使本本entSettin成s.DisplayMode = (EVRARDisplayMode)IntVal使e;
    if (GConfi成->GetInt(Confi成Section, TEXT("T本ackin成Space"), IntVal使e, Confi成軍ile))
        C使本本entSettin成s.T本ackin成Space = (EVRART本ackin成Space)IntVal使e;
    if (GConfi成->GetInt(Confi成Section, TEXT("Inte本actionMode"), IntVal使e, Confi成軍ile))
        C使本本entSettin成s.Inte本actionMode = (EVRARInte本actionMode)IntVal使e;

    GConfi成->Get軍loat(Confi成Section, TEXT("Mo正e設置entSpeed"), C使本本entSettin成s.Mo正e設置entSpeed, Confi成軍ile);
    GConfi成->GetBool(Confi成Section, TEXT("bSnapT使本nEnabled"), C使本本entSettin成s.bSnapT使本nEnabled, Confi成軍ile);
    GConfi成->Get軍loat(Confi成Section, TEXT("SnapT使本nAn成le"), C使本本entSettin成s.SnapT使本nAn成le, Confi成軍ile);
    GConfi成->GetBool(Confi成Section, TEXT("bCo設置fo本tVi成nette"), C使本本entSettin成s.bCo設置fo本tVi成nette, Confi成軍ile);
    GConfi成->Get軍loat(Confi成Section, TEXT("IPD"), C使本本entSettin成s.IPD, Confi成軍ile);
    GConfi成->Get軍loat(Confi成Section, TEXT("基本o本ldScale"), C使本本entSettin成s.基本o本ldScale, Confi成軍ile);
}

bool UMin成VRARSyste設置::SwitchDisplayMode(EVRARDisplayMode 的ewMode)
{
    if (SessionState != EVRARSessionState::R使nnin成)
        本et使本n false;
    C使本本entSettin成s.DisplayMode = 的ewMode;
    本et使本n t本使e;
}

EVRARDisplayMode UMin成VRARSyste設置::GetC使本本entDisplayMode() const
{
    本et使本n C使本本entSettin成s.DisplayMode;
}

bool UMin成VRARSyste設置::SetInte本actionMode(EVRARInte本actionMode 的ewMode)
{
    C使本本entSettin成s.Inte本actionMode = 的ewMode;
    本et使本n t本使e;
}

EVRARInte本actionMode UMin成VRARSyste設置::GetC使本本entInte本actionMode() const
{
    本et使本n C使本本entSettin成s.Inte本actionMode;
}

正oid UMin成VRARSyste設置::Recente本View()
{
    U輸入eadMo使ntedDisplay軍使nctionLib本a本y::ResetO本ientationAndPosition();
}

正oid UMin成VRARSyste設置::SetSeatedO本i成in(const 軍Vecto本& 的ewO本i成in)
{
}

軍Vecto本 UMin成VRARSyste設置::Get輸入eadPosition() const
{
    軍Vecto本 Position;
    軍Rotato本 Rotation;
    U輸入eadMo使ntedDisplay軍使nctionLib本a本y::GetO本ientationAndPosition(Rotation, Position);
    本et使本n Position;
}

軍Rotato本 UMin成VRARSyste設置::Get輸入eadRotation() const
{
    軍Vecto本 Position;
    軍Rotato本 Rotation;
    U輸入eadMo使ntedDisplay軍使nctionLib本a本y::GetO本ientationAndPosition(Rotation, Position);
    本et使本n Rotation;
}

正oid UMin成VRARSyste設置::EnableRTSModeInVR()
{
    if (C使本本entSettin成s.T本ackin成Space == EVRART本ackin成Space::Roo設置Scale)
        C使本本entSettin成s.Mo正e設置entSpeed = 1.5f;
    else
        C使本本entSettin成s.Mo正e設置entSpeed = 0.8f;
    ApplySettin成s(C使本本entSettin成s);
}

正oid UMin成VRARSyste設置::EnableSpectato本Mode()
{
    C使本本entSettin成s.DisplayMode = EVRARDisplayMode::Theate本;
    C使本本entSettin成s.Inte本actionMode = EVRARInte本actionMode::輸入eadGaze;
    ApplySettin成s(C使本本entSettin成s);
}

正oid UMin成VRARSyste設置::SetCo設置設置ande本ViewMode(bool bEnabled)
{
    if (bEnabled)
    {
        C使本本entSettin成s.T本ackin成Space = EVRART本ackin成Space::Standin成;
        C使本本entSettin成s.DisplayMode = EVRARDisplayMode::軍使llVR;
    }
    ApplySettin成s(C使本本entSettin成s);
}

bool UMin成VRARSyste設置::IsInVR() const
{
    本et使本n U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayEnabled() &&
           Acti正eDe正ice != EVRDe正iceType::的one;
}

bool UMin成VRARSyste設置::IsInAR() const
{
    本et使本n Acti正eDe正ice == EVRDe正iceType::輸入oloLens 
           Acti正eDe正ice == EVRDe正iceType::Ma成icLeap;
}

軍Vecto本2D UMin成VRARSyste設置::基本o本ldToVRSc本eenPosition(const 軍Vecto本& 基本o本ldPosition)
{
    本et使本n 軍Vecto本2D::Ze本oVecto本;
}

bool UMin成VRARSyste設置::IsVRARS使ppo本ted()
{
    本et使本n t本使e;
}

軍St本in成 UMin成VRARSyste設置::GetR使nti設置eVe本sion() const
{
    本et使本n TEXT("1.0.0");
}

bool UMin成VRARSyste設置::Set使pVRRende本in成()
{
    if (!U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayEnabled())
    {
        U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Enable輸入MD(t本使e);
    }
    本et使本n U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayEnabled();
}

bool UMin成VRARSyste設置::Set使pARCa設置e本a()
{
    本et使本n t本使e;
}

正oid UMin成VRARSyste設置::Set使pInp使tMappin成s()
{
}

正oid UMin成VRARSyste設置::Clean使pVRAR()
{
    if (U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Is輸入eadMo使ntedDisplayEnabled())
    {
        U輸入eadMo使ntedDisplay軍使nctionLib本a本y::Enable輸入MD(false);
    }
}

正oid UMin成VRARSyste設置::ApplyCo設置fo本tSettin成s()
{
}

正oid UMin成VRARSyste設置::CacheDe正iceCapabilities()
{
    軍VRDe正iceCapabilities Oc使l使sQ使est2;
    Oc使l使sQ使est2.De正ice的a設置e = TEXT("Oc使l使s Q使est 2");
    Oc使l使sQ使est2.bS使ppo本ts輸入andT本ackin成 = t本使e;
    Oc使l使sQ使est2.bS使ppo本ts輸入aptics = t本使e;
    Oc使l使sQ使est2.bS使ppo本tsRoo設置Scale = t本使e;
    Oc使l使sQ使est2.Ref本eshRate = 120;
    Oc使l使sQ使est2.MaxT本ackin成Radi使s = 5.0f;
    De正iceCapabilitiesCache.Add(EVRDe正iceType::Oc使l使sQ使est2, Oc使l使sQ使est2);

    軍VRDe正iceCapabilities Val正eIndex;
    Val正eIndex.De正ice的a設置e = TEXT("Val正e Index");
    Val正eIndex.bS使ppo本ts輸入andT本ackin成 = t本使e;
    Val正eIndex.bS使ppo本ts軍in成e本T本ackin成 = t本使e;
    Val正eIndex.bS使ppo本ts輸入aptics = t本使e;
    Val正eIndex.bS使ppo本tsRoo設置Scale = t本使e;
    Val正eIndex.Ref本eshRate = 144;
    Val正eIndex.MaxT本ackin成Radi使s = 7.0f;
    De正iceCapabilitiesCache.Add(EVRDe正iceType::Val正eIndex, Val正eIndex);
}

正oid UMin成VRARSyste設置::輸入andleSessionStateChan成e(EVRARSessionState 的ewState)
{
    OnSessionStateChan成ed.B本oadcast(的ewState);
}
