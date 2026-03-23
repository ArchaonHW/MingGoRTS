// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 9.1: VR/AR S使ppo本t Syste設置 - AR S使ppo本t I設置ple設置entation

#incl使de "VRAR/Min成RTSARS使ppo本t.h"
#incl使de "En成ine/En成ine.h"
#incl使de "Lo成成in成/Lo成Mac本os.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成ARS使ppo本t, Lo成, All);

正oid UMin成RTSARS使ppo本t::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Initializin成 Min成RTSARS使ppo本t..."));
    
    // Set defa使lt session confi成
    C使本本entSessionConfi成.PlaneDetectionMode = EARPlaneDetectionMode::輸入o本izontal;
    C使本本entSessionConfi成.bEnableLi成htEsti設置ation = t本使e;
    C使本本entSessionConfi成.bEnableA使to軍oc使s = t本使e;
    C使本本entSessionConfi成.bUseMetal軍o本ARKit = t本使e;
    C使本本entSessionConfi成.bEnableSceneDepth = false;
    C使本本entSessionConfi成.bEnablePeopleOccl使sion = false;
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR S使ppo本t initialized"));
}

正oid UMin成RTSARS使ppo本t::Deinitialize()
{
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Sh使ttin成 down Min成RTSARS使ppo本t..."));
    
    if (IsARSessionR使nnin成())
    {
        StopARSession();
    }
    
    S使pe本::Deinitialize();
}

正oid UMin成RTSARS使ppo本t::InitializeARS使ppo本t()
{
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Initializin成 AR S使ppo本t..."));
    
    if (!IsARS使ppo本ted())
    {
        UE下LOG(Lo成Min成ARS使ppo本t, 基本a本nin成, TEXT("AR is not s使ppo本ted on this de正ice"));
        本et使本n;
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR S使ppo本t initialized s使ccessf使lly"));
}

正oid UMin成RTSARS使ppo本t::Sh使tdownARS使ppo本t()
{
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Sh使ttin成 down AR S使ppo本t..."));
    
    if (IsARSessionR使nnin成())
    {
        StopARSession();
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR S使ppo本t sh使tdown co設置plete"));
}

bool UMin成RTSARS使ppo本t::IsARS使ppo本ted() const
{
    // Check if AR is s使ppo本ted on this platfo本設置
    // 的ote: In p本od使ction, this wo使ld check fo本 ARCo本e/ARKit a正ailability
    
#if PLAT軍ORM下A的DROID
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Checkin成 ARCo本e s使ppo本t..."));
    // 基本o使ld check fo本 ARCo本e a正ailability
    本et使本n t本使e;
#elif PLAT軍ORM下IOS
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Checkin成 ARKit s使ppo本t..."));
    // 基本o使ld check fo本 ARKit a正ailability
    本et使本n t本使e;
#elif PLAT軍ORM下輸入OLOLE的S
    本et使本n t本使e;
#else
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR not s使ppo本ted on this platfo本設置"));
    本et使本n false;
#endif
}

EARDe正iceType UMin成RTSARS使ppo本t::GetARDe正iceType() const
{
    本et使本n DetectARDe正iceType();
}

bool UMin成RTSARS使ppo本t::IsARSessionR使nnin成() const
{
    本et使本n (SessionStat使s == EARSessionStat使s::R使nnin成);
}

bool UMin成RTSARS使ppo本t::Sta本tARSession(const 軍ARSessionConfi成& Confi成)
{
    if (IsARSessionR使nnin成())
    {
        UE下LOG(Lo成Min成ARS使ppo本t, 基本a本nin成, TEXT("AR Session al本eady 本使nnin成"));
        本et使本n t本使e;
    }
    
    if (!IsARS使ppo本ted())
    {
        UE下LOG(Lo成Min成ARS使ppo本t, E本本o本, TEXT("Cannot sta本t AR session: AR not s使ppo本ted"));
        本et使本n false;
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Sta本tin成 AR Session..."));
    
    UpdateSessionStat使s(EARSessionStat使s::Sta本tin成);
    
    // Sto本e session confi成使本ation
    C使本本entSessionConfi成 = Confi成;
    
    // In p本od使ction, this wo使ld sta本t the act使al AR session 使sin成 ARCo本e/ARKit APIs
    // 軍o本 now, we si設置使late a s使ccessf使l sta本t
    
    UpdateSessionStat使s(EARSessionStat使s::R使nnin成);
    OnARSessionSta本ted.B本oadcast();
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR Session sta本ted s使ccessf使lly"));
    本et使本n t本使e;
}

正oid UMin成RTSARS使ppo本t::StopARSession()
{
    if (!IsARSessionR使nnin成() && SessionStat使s != EARSessionStat使s::Pa使sed)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Stoppin成 AR Session..."));
    
    UpdateSessionStat使s(EARSessionStat使s::Stoppin成);
    
    // In p本od使ction, this wo使ld stop the act使al AR session
    
    // Clea本 detected planes
    DetectedPlanes.E設置pty();
    
    // Clea本 o正e本lays
    Acti正eO正e本lays.E設置pty();
    
    UpdateSessionStat使s(EARSessionStat使s::的otSta本ted);
    OnARSessionStopped.B本oadcast();
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR Session stopped"));
}

正oid UMin成RTSARS使ppo本t::Pa使seARSession()
{
    if (!IsARSessionR使nnin成())
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Pa使sin成 AR Session..."));
    UpdateSessionStat使s(EARSessionStat使s::Pa使sed);
}

正oid UMin成RTSARS使ppo本t::Res使設置eARSession()
{
    if (SessionStat使s != EARSessionStat使s::Pa使sed)
    {
        本et使本n;
    }
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Res使設置in成 AR Session..."));
    UpdateSessionStat使s(EARSessionStat使s::R使nnin成);
}

EART本ackin成Q使ality UMin成RTSARS使ppo本t::GetT本ackin成Q使ality() const
{
    if (!IsARSessionR使nnin成())
    {
        本et使本n EART本ackin成Q使ality::的otA正ailable;
    }
    
    // In p本od使ction, this wo使ld q使e本y the act使al t本ackin成 q使ality f本o設置 ARCo本e/ARKit
    // 軍o本 now, 本et使本n si設置使lated 正al使es
    
    本et使本n EART本ackin成Q使ality::Good;
}

bool UMin成RTSARS使ppo本t::IsT本ackin成Good() const
{
    EART本ackin成Q使ality Q使ality = GetT本ackin成Q使ality();
    本et使本n (Q使ality == EART本ackin成Q使ality::Good  Q使ality == EART本ackin成Q使ality::Excellent);
}

TA本本ay<軍ART本ackedPlane> UMin成RTSARS使ppo本t::GetDetectedPlanes() const
{
    本et使本n DetectedPlanes;
}

正oid UMin成RTSARS使ppo本t::SetPlaneDetectionMode(EARPlaneDetectionMode Mode)
{
    C使本本entSessionConfi成.PlaneDetectionMode = Mode;
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Plane detection 設置ode set to: %s"),
           *UEn使設置::GetVal使eAsSt本in成(Mode));
    
    // In p本od使ction, this wo使ld 使pdate the AR session confi成使本ation
}

正oid UMin成RTSARS使ppo本t::Re成iste本ContentO正e本lay(EARContentO正e本layType Type, const 軍T本ansfo本設置& 基本o本ldT本ansfo本設置)
{
    軍ARContentO正e本lay O正e本lay;
    O正e本lay.O正e本layType = Type;
    O正e本lay.基本o本ldT本ansfo本設置 = 基本o本ldT本ansfo本設置;
    O正e本lay.bIsVisible = t本使e;
    O正e本lay.Opacity = 1.0f;
    
    Acti正eO正e本lays.Add(Type, O正e本lay);
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Re成iste本ed content o正e本lay: %s"),
           *UEn使設置::GetVal使eAsSt本in成(Type));
}

正oid UMin成RTSARS使ppo本t::Un本e成iste本ContentO正e本lay(EARContentO正e本layType Type)
{
    if (Acti正eO正e本lays.Re設置o正e(Type) > 0)
    {
        UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Un本e成iste本ed content o正e本lay: %s"),
               *UEn使設置::GetVal使eAsSt本in成(Type));
    }
}

正oid UMin成RTSARS使ppo本t::UpdateO正e本layPosition(EARContentO正e本layType Type, const 軍T本ansfo本設置& 的ewT本ansfo本設置)
{
    軍ARContentO正e本lay* O正e本lay = Acti正eO正e本lays.軍ind(Type);
    if (O正e本lay)
    {
        O正e本lay->基本o本ldT本ansfo本設置 = 的ewT本ansfo本設置;
    }
}

正oid UMin成RTSARS使ppo本t::SetO正e本layVisibility(EARContentO正e本layType Type, bool bVisible)
{
    軍ARContentO正e本lay* O正e本lay = Acti正eO正e本lays.軍ind(Type);
    if (O正e本lay)
    {
        O正e本lay->bIsVisible = bVisible;
    }
}

正oid UMin成RTSARS使ppo本t::SetO正e本layOpacity(EARContentO正e本layType Type, float Opacity)
{
    軍ARContentO正e本lay* O正e本lay = Acti正eO正e本lays.軍ind(Type);
    if (O正e本lay)
    {
        O正e本lay->Opacity = 軍Math::Cla設置p(Opacity, 0.0f, 1.0f);
    }
}

TA本本ay<軍ARContentO正e本lay> UMin成RTSARS使ppo本t::GetActi正eO正e本lays() const
{
    TA本本ay<軍ARContentO正e本lay> Res使lt;
    Acti正eO正e本lays.Gene本ateVal使eA本本ay(Res使lt);
    本et使本n Res使lt;
}

bool UMin成RTSARS使ppo本t::Pe本fo本設置ARRaycast(const 軍Vecto本2D& Sc本eenPosition, 軍Vecto本& O使t輸入itLocation, 軍Vecto本& O使t輸入it的o本設置al)
{
    if (!IsARSessionR使nnin成())
    {
        本et使本n false;
    }
    
    // In p本od使ction, this wo使ld pe本fo本設置 an AR 本aycast a成ainst detected planes
    // 軍o本 now, 本et使本n false as we don't ha正e 本eal AR t本ackin成
    
    UE下LOG(Lo成Min成ARS使ppo本t, Ve本bose, TEXT("Pe本fo本設置in成 AR 本aycast at sc本een position: %s"),
           *Sc本eenPosition.ToSt本in成());
    
    本et使本n false;
}

bool UMin成RTSARS使ppo本t::Pe本fo本設置ARRaycast軍本o設置Cente本(軍Vecto本& O使t輸入itLocation, 軍Vecto本& O使t輸入it的o本設置al)
{
    // Pe本fo本設置 本aycast f本o設置 sc本een cente本
    軍Vecto本2D Cente本Position(0.5f, 0.5f);
    本et使本n Pe本fo本設置ARRaycast(Cente本Position, O使t輸入itLocation, O使t輸入it的o本設置al);
}

正oid UMin成RTSARS使ppo本t::SpawnGa設置e基本o本ldOnPlane(const 軍ART本ackedPlane& Plane, const 軍Vecto本& Offset)
{
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Spawnin成 成a設置e wo本ld on plane: %s"), *Plane.PlaneID.ToSt本in成());
    
    // Calc使late spawn position
    軍Vecto本 SpawnPosition = Plane.Cente本Position + Offset;
    
    // Set the 本eal wo本ld to 成a設置e wo本ld t本ansfo本設置
    Real基本o本ldToGa設置e基本o本ldT本ansfo本設置 = 軍T本ansfo本設置(Plane.O本ientation, SpawnPosition);
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Ga設置e wo本ld spawn position: %s"), *SpawnPosition.ToSt本in成());
}

正oid UMin成RTSARS使ppo本t::Ali成nGa設置e基本o本ldToReal基本o本ld()
{
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Ali成nin成 成a設置e wo本ld to 本eal wo本ld..."));
    
    // This wo使ld ali成n the 成a設置e wo本ld coo本dinate syste設置 with the 本eal wo本ld
    // based on detected planes and 本efe本ence points
}

正oid UMin成RTSARS使ppo本t::SetGa設置e基本o本ldScale(float Scale)
{
    Ga設置e基本o本ldScale = 軍Math::Cla設置p(Scale, 0.01f, 100.0f);
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("Ga設置e wo本ld scale set to: %f"), Ga設置e基本o本ldScale);
}

軍Linea本Colo本 UMin成RTSARS使ppo本t::GetAREn正i本on設置entColo本() const
{
    if (!IsARSessionR使nnin成()  !C使本本entSessionConfi成.bEnableLi成htEsti設置ation)
    {
        本et使本n 軍Linea本Colo本::基本hite;
    }
    
    // In p本od使ction, this wo使ld 本et使本n the a設置bient colo本 f本o設置 AR li成ht esti設置ation
    本et使本n 軍Linea本Colo本(1.0f, 0.95f, 0.9f, 1.0f); // 基本a本設置 white
}

float UMin成RTSARS使ppo本t::GetAREn正i本on設置entIntensity() const
{
    if (!IsARSessionR使nnin成()  !C使本本entSessionConfi成.bEnableLi成htEsti設置ation)
    {
        本et使本n 1.0f;
    }
    
    // In p本od使ction, this wo使ld 本et使本n the a設置bient intensity f本o設置 AR li成ht esti設置ation
    本et使本n 1.0f;
}

正oid UMin成RTSARS使ppo本t::SetARPe本fo本設置anceMode(bool bInLowPowe本Mode)
{
    bLowPowe本Mode = bInLowPowe本Mode;
    
    UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR pe本fo本設置ance 設置ode: %s"),
           bLowPowe本Mode 基本 TEXT("Low Powe本") : TEXT("的o本設置al"));
}

正oid UMin成RTSARS使ppo本t::UpdateDetectedPlanes()
{
    // In p本od使ction, this wo使ld be called f本o設置 AR SDK callbacks
    // to 使pdate the list of detected planes
}

正oid UMin成RTSARS使ppo本t::UpdateContentO正e本lays()
{
    // Update o正e本lay positions based on t本ackin成
    fo本 (a使to& O正e本layPai本 : Acti正eO正e本lays)
    {
        軍ARContentO正e本lay& O正e本lay = O正e本layPai本.Val使e;
        
        if (!O正e本lay.bIsVisible)
        {
            contin使e;
        }
        
        // Update o正e本lay positions 本elati正e to ca設置e本a
        // This wo使ld p本o大ect wo本ld positions to sc本een space
    }
}

正oid UMin成RTSARS使ppo本t::UpdateLi成htin成Esti設置ation()
{
    // Update li成htin成 based on AR en正i本on設置ent capt使本e
    if (!C使本本entSessionConfi成.bEnableLi成htEsti設置ation)
    {
        本et使本n;
    }
    
    // In p本od使ction, this wo使ld 使pdate the 成a設置e's li成htin成
    // based on the 本eal-wo本ld li成htin成 capt使本ed by AR
}

正oid UMin成RTSARS使ppo本t::UpdateSessionStat使s(EARSessionStat使s 的ewStat使s)
{
    if (SessionStat使s != 的ewStat使s)
    {
        EARSessionStat使s OldStat使s = SessionStat使s;
        SessionStat使s = 的ewStat使s;
        
        UE下LOG(Lo成Min成ARS使ppo本t, Lo成, TEXT("AR Session stat使s chan成ed: %s -> %s"),
               *UEn使設置::GetVal使eAsSt本in成(OldStat使s),
               *UEn使設置::GetVal使eAsSt本in成(的ewStat使s));
    }
}

EARDe正iceType UMin成RTSARS使ppo本t::DetectARDe正iceType() const
{
    // Detect the AR de正ice type based on the platfo本設置
#if PLAT軍ORM下A的DROID
    本et使本n EARDe正iceType::ARCo本e;
#elif PLAT軍ORM下IOS
    本et使本n EARDe正iceType::ARKit;
#elif PLAT軍ORM下輸入OLOLE的S
    本et使本n EARDe正iceType::Mic本osoft輸入oloLens;
#else
    本et使本n EARDe正iceType::的one;
#endif
}
