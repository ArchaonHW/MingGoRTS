// Copy本i成ht (c) 2026 Min成GoRTS. All 本i成hts 本ese本正ed.
// Epic 9.1: VR/AR S使ppo本t Syste設置 - VR/AR Pe本fo本設置ance Opti設置ize本 I設置ple設置entation

#incl使de "VRAR/Min成RTSVARPe本fo本設置anceOpti設置ize本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Kis設置et/Ga設置eplayStatics.h"

DE軍I的E下LOG下CATEGORY下STATIC(Lo成Min成VARPe本fo本設置ance, Lo成, All);

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Initialize(軍S使bsyste設置CollectionBase& Collection)
{
    S使pe本::Initialize(Collection);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Initializin成 VR/AR Pe本fo本設置ance Opti設置ize本..."));
    
    // Set defa使lt settin成s
    C使本本entSettin成s.Q使alityLe正el = EVARPe本fo本設置anceLe正el::輸入i成h;
    C使本本entSettin成s.Ta本成et軍本a設置eRate = EVAR軍本a設置eRate::軍PS72;
    C使本本entSettin成s.bEnableAdapti正eQ使ality = t本使e;
    C使本本entSettin成s.bEnable軍o正eatedRende本in成 = t本使e;
    C使本本entSettin成s.bEnableDyna設置icResol使tion = t本使e;
    C使本本entSettin成s.bEnableOccl使sionC使llin成 = t本使e;
    C使本本entSettin成s.bEnableLODSyste設置 = t本使e;
    C使本本entSettin成s.bEnableText使本eSt本ea設置in成 = t本使e;
    C使本本entSettin成s.bRed使cePa本ticleEffects = t本使e;
    C使本本entSettin成s.bSi設置plifyShadows = t本使e;
    C使本本entSettin成s.MaxVisibleUnits = 100;
    C使本本entSettin成s.ViewDistance = 5000.0f;
    C使本本entSettin成s.Text使本eQ使ality = 1.0f;
    C使本本entSettin成s.bEnableAS基本 = t本使e;
    C使本本entSettin成s.bEnableMotionS設置oothin成 = t本使e;
    
    軍PS輸入isto本y.Rese本正e(60);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("VR/AR Pe本fo本設置ance Opti設置ize本 initialized"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Deinitialize()
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Sh使ttin成 down VR/AR Pe本fo本設置ance Opti設置ize本..."));
    
    StopPe本fo本設置anceMonito本in成();
    
    S使pe本::Deinitialize();
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::InitializeOpti設置ize本()
{
    bIsInitialized = t本使e;
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Pe本fo本設置ance opti設置ize本 initialized"));
    
    Sta本tPe本fo本設置anceMonito本in成();
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Sh使tdownOpti設置ize本()
{
    bIsInitialized = false;
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Pe本fo本設置ance opti設置ize本 sh使tdown"));
    
    StopPe本fo本設置anceMonito本in成();
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetOpti設置izationSettin成s(const 軍VAROpti設置izationSettin成s& Settin成s)
{
    C使本本entSettin成s = Settin成s;
    ApplyOpti設置izationSettin成s();
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Opti設置ization settin成s 使pdated"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyQ使alityLe正el(EVARPe本fo本設置anceLe正el Le正el)
{
    C使本本entSettin成s.Q使alityLe正el = Le正el;
    ApplyPe本fo本設置anceP本eset(Le正el);
    
    OnPe本fo本設置anceLe正elChan成ed.B本oadcast(Le正el);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Q使ality le正el chan成ed to: %s"),
           *UEn使設置::GetVal使eAsSt本in成(Le正el));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate 軍本a設置eRate)
{
    C使本本entSettin成s.Ta本成et軍本a設置eRate = 軍本a設置eRate;
    
    float Ta本成et軍PS = GetTa本成et軍PS();
    
    // Apply f本a設置e 本ate settin成s to en成ine
    if (GEn成ine)
    {
        // Set fixed f本a設置e 本ate fo本 VR
        GEn成ine->軍ixed軍本a設置eRate = Ta本成et軍PS;
    }
    
    On軍本a設置eRateChan成ed.B本oadcast(軍本a設置eRate);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Ta本成et f本a設置e 本ate set to: %f 軍PS"), Ta本成et軍PS);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableAdapti正eQ使ality(bool bEnable)
{
    C使本本entSettin成s.bEnableAdapti正eQ使ality = bEnable;
    
    if (bEnable && !bIsMonito本in成)
    {
        Sta本tPe本fo本設置anceMonito本in成();
    }
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Adapti正e q使ality %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::UpdateAdapti正eQ使ality()
{
    if (!C使本本entSettin成s.bEnableAdapti正eQ使ality  !bIsMonito本in成)
    {
        本et使本n;
    }
    
    float Ta本成et軍PS = GetTa本成et軍PS();
    float C使本本ent軍PS = C使本本entMet本ics.C使本本ent軍PS;
    
    // Check if we'本e consistently d本oppin成 f本a設置es
    if (C使本本ent軍PS < Ta本成et軍PS * 0.9f)
    {
        if (Q使alityRed使ctionSteps < MaxQ使alityRed使ctionSteps)
        {
            Red使ceQ使alityStep();
        }
        else
        {
            OnPe本fo本設置ance基本a本nin成.B本oadcast(TEXT("Pe本fo本設置ance c本itically low, cannot 本ed使ce q使ality f使本the本"));
        }
    }
    else if (C使本本ent軍PS > Ta本成et軍PS * 1.1f && Q使alityRed使ctionSteps > 0)
    {
        // 基本e ha正e head本oo設置, t本y i設置p本o正in成 q使ality
        I設置p本o正eQ使alityStep();
    }
}

bool UMin成RTSVARPe本fo本設置anceOpti設置ize本::Sho使ldRed使ceQ使ality() const
{
    float Ta本成et軍PS = GetTa本成et軍PS();
    本et使本n C使本本entMet本ics.C使本本ent軍PS < Ta本成et軍PS * 0.85f;
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Red使ceQ使alityStep()
{
    Q使alityRed使ctionSteps++;
    
    // Red使ce 正a本io使s q使ality settin成s
    C使本本entSettin成s.Text使本eQ使ality = 軍Math::Max(0.5f, C使本本entSettin成s.Text使本eQ使ality - 0.1f);
    C使本本entSettin成s.ViewDistance = 軍Math::Max(2000.0f, C使本本entSettin成s.ViewDistance - 500.0f);
    C使本本entSettin成s.MaxVisibleUnits = 軍Math::Max(50, C使本本entSettin成s.MaxVisibleUnits - 10);
    
    ApplyOpti設置izationSettin成s();
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, 基本a本nin成, TEXT("Q使ality 本ed使ced (step %d)"), Q使alityRed使ctionSteps);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::I設置p本o正eQ使alityStep()
{
    if (Q使alityRed使ctionSteps <= 0)
    {
        本et使本n;
    }
    
    Q使alityRed使ctionSteps--;
    
    // Resto本e q使ality settin成s
    C使本本entSettin成s.Text使本eQ使ality = 軍Math::Min(1.0f, C使本本entSettin成s.Text使本eQ使ality + 0.1f);
    C使本本entSettin成s.ViewDistance = 軍Math::Min(10000.0f, C使本本entSettin成s.ViewDistance + 500.0f);
    C使本本entSettin成s.MaxVisibleUnits = 軍Math::Min(200, C使本本entSettin成s.MaxVisibleUnits + 10);
    
    ApplyOpti設置izationSettin成s();
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Q使ality i設置p本o正ed (step %d)"), Q使alityRed使ctionSteps);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Sta本tPe本fo本設置anceMonito本in成()
{
    bIsMonito本in成 = t本使e;
    Met本icsUpdateTi設置e本 = 0.0f;
    軍PS輸入isto本y.E設置pty();
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 sta本ted"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::StopPe本fo本設置anceMonito本in成()
{
    bIsMonito本in成 = false;
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 stopped"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ResetPe本fo本設置anceStats()
{
    C使本本entMet本ics = 軍VARPe本fo本設置anceMet本ics();
    軍PS輸入isto本y.E設置pty();
    Q使alityRed使ctionSteps = 0;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Pe本fo本設置ance stats 本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Enable軍o正eatedRende本in成(bool bEnable)
{
    C使本本entSettin成s.bEnable軍o正eatedRende本in成 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("軍o正eated 本ende本in成 %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Set軍o正eationLe正el(int32 Le正el)
{
    Le正el = 軍Math::Cla設置p(Le正el, 0, 4);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("軍o正eation le正el set to: %d"), Le正el);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableAS基本(bool bEnable)
{
    C使本本entSettin成s.bEnableAS基本 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("AS基本 (Asynch本ono使s Spacewa本p) %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableMotionS設置oothin成(bool bEnable)
{
    C使本本entSettin成s.bEnableMotionS設置oothin成 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Motion s設置oothin成 %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetARCa設置e本aResol使tion(int32 基本idth, int32 輸入ei成ht)
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("AR ca設置e本a 本esol使tion set to: %dx%d"), 基本idth, 輸入ei成ht);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Red使ceART本ackin成軍本eq使ency(bool bRed使ce)
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("AR t本ackin成 f本eq使ency %s"),
           bRed使ce 基本 TEXT("本ed使ced") : TEXT("no本設置al"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableARPlaneOpti設置ization(bool bEnable)
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("AR plane opti設置ization %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetViewDistance(float Distance)
{
    C使本本entSettin成s.ViewDistance = 軍Math::Max(1000.0f, Distance);
    
    // Apply to en成ine
    if (GEn成ine)
    {
        GEn成ine->ViewDistanceScale = C使本本entSettin成s.ViewDistance / 5000.0f;
    }
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("View distance set to: %f"), C使本本entSettin成s.ViewDistance);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetText使本eQ使ality(float Q使ality)
{
    C使本本entSettin成s.Text使本eQ使ality = 軍Math::Cla設置p(Q使ality, 0.5f, 1.0f);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Text使本e q使ality set to: %f"), C使本本entSettin成s.Text使本eQ使ality);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetMaxVisibleUnits(int32 Co使nt)
{
    C使本本entSettin成s.MaxVisibleUnits = 軍Math::Max(10, Co使nt);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Max 正isible 使nits set to: %d"), C使本本entSettin成s.MaxVisibleUnits);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableLODSyste設置(bool bEnable)
{
    C使本本entSettin成s.bEnableLODSyste設置 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("LOD syste設置 %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableOccl使sionC使llin成(bool bEnable)
{
    C使本本entSettin成s.bEnableOccl使sionC使llin成 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Occl使sion c使llin成 %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::EnableText使本eSt本ea設置in成(bool bEnable)
{
    C使本本entSettin成s.bEnableText使本eSt本ea設置in成 = bEnable;
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Text使本e st本ea設置in成 %s"),
           bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::軍l使shUn使sedMe設置o本y()
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("軍l使shin成 使n使sed 設置e設置o本y..."));
    
    if (GEn成ine)
    {
        // T本i成成e本 成a本ba成e collection
        GEn成ine->軍o本ceGa本ba成eCollection(t本使e);
    }
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::SetMe設置o本yB使d成et(float Me成abytes)
{
    Me設置o本yB使d成etMB = 軍Math::Max(512.0f, Me成abytes);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Me設置o本y b使d成et set to: %f MB"), Me設置o本yB使d成etMB);
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el Le正el)
{
    switch (Le正el)
    {
        case EVARPe本fo本設置anceLe正el::Low:
            C使本本entSettin成s.bEnable軍o正eatedRende本in成 = t本使e;
            C使本本entSettin成s.bEnableDyna設置icResol使tion = t本使e;
            C使本本entSettin成s.ViewDistance = 2000.0f;
            C使本本entSettin成s.Text使本eQ使ality = 0.5f;
            C使本本entSettin成s.MaxVisibleUnits = 50;
            C使本本entSettin成s.bRed使cePa本ticleEffects = t本使e;
            C使本本entSettin成s.bSi設置plifyShadows = t本使e;
            b本eak;
            
        case EVARPe本fo本設置anceLe正el::Medi使設置:
            C使本本entSettin成s.bEnable軍o正eatedRende本in成 = t本使e;
            C使本本entSettin成s.bEnableDyna設置icResol使tion = t本使e;
            C使本本entSettin成s.ViewDistance = 3500.0f;
            C使本本entSettin成s.Text使本eQ使ality = 0.75f;
            C使本本entSettin成s.MaxVisibleUnits = 75;
            C使本本entSettin成s.bRed使cePa本ticleEffects = t本使e;
            C使本本entSettin成s.bSi設置plifyShadows = false;
            b本eak;
            
        case EVARPe本fo本設置anceLe正el::輸入i成h:
            C使本本entSettin成s.bEnable軍o正eatedRende本in成 = t本使e;
            C使本本entSettin成s.bEnableDyna設置icResol使tion = false;
            C使本本entSettin成s.ViewDistance = 5000.0f;
            C使本本entSettin成s.Text使本eQ使ality = 1.0f;
            C使本本entSettin成s.MaxVisibleUnits = 100;
            C使本本entSettin成s.bRed使cePa本ticleEffects = false;
            C使本本entSettin成s.bSi設置plifyShadows = false;
            b本eak;
            
        case EVARPe本fo本設置anceLe正el::Ult本a:
            C使本本entSettin成s.bEnable軍o正eatedRende本in成 = false;
            C使本本entSettin成s.bEnableDyna設置icResol使tion = false;
            C使本本entSettin成s.ViewDistance = 10000.0f;
            C使本本entSettin成s.Text使本eQ使ality = 1.0f;
            C使本本entSettin成s.MaxVisibleUnits = 200;
            C使本本entSettin成s.bRed使cePa本ticleEffects = false;
            C使本本entSettin成s.bSi設置plifyShadows = false;
            b本eak;
            
        defa使lt:
            b本eak;
    }
    
    ApplyOpti設置izationSettin成s();
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied pe本fo本設置ance p本eset: %s"),
           *UEn使設置::GetVal使eAsSt本in成(Le正el));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyMobileVRP本eset()
{
    ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el::Low);
    SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate::軍PS72);
    EnableAS基本(t本使e);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied Mobile VR p本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyStandaloneVRP本eset()
{
    ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el::Medi使設置);
    SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate::軍PS72);
    EnableAS基本(t本使e);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied Standalone VR p本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyPCVRP本eset()
{
    ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el::輸入i成h);
    SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate::軍PS90);
    EnableAS基本(false);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied PC VR p本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyMobileARP本eset()
{
    ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el::Medi使設置);
    SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate::軍PS60);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied Mobile AR p本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Apply輸入oloLensP本eset()
{
    ApplyPe本fo本設置anceP本eset(EVARPe本fo本設置anceLe正el::Low);
    SetTa本成et軍本a設置eRate(EVAR軍本a設置eRate::軍PS60);
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Lo成, TEXT("Applied 輸入oloLens p本eset"));
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::UpdatePe本fo本設置anceMet本ics(float DeltaTi設置e)
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }
    
    // Calc使late c使本本ent 軍PS
    float C使本本ent軍PS = 1.0f / DeltaTi設置e;
    C使本本entMet本ics.C使本本ent軍PS = C使本本ent軍PS;
    C使本本entMet本ics.軍本a設置eTi設置e = DeltaTi設置e * 1000.0f; // in 設置s
    
    // Update histo本y
    軍PS輸入isto本y.Add(C使本本ent軍PS);
    if (軍PS輸入isto本y.的使設置() > 60)
    {
        軍PS輸入isto本y.Re設置o正eAt(0);
    }
    
    // Calc使late a正e本a成e, 設置in, 設置ax
    if (軍PS輸入isto本y.的使設置() > 0)
    {
        float S使設置 = 0.0f;
        C使本本entMet本ics.Min軍PS = 999.0f;
        C使本本entMet本ics.Max軍PS = 0.0f;
        
        fo本 (float 軍PS : 軍PS輸入isto本y)
        {
            S使設置 += 軍PS;
            C使本本entMet本ics.Min軍PS = 軍Math::Min(C使本本entMet本ics.Min軍PS, 軍PS);
            C使本本entMet本ics.Max軍PS = 軍Math::Max(C使本本entMet本ics.Max軍PS, 軍PS);
        }
        
        C使本本entMet本ics.A正e本a成e軍PS = S使設置 / 軍PS輸入isto本y.的使設置();
    }
    
    // Check fo本 d本opped f本a設置es
    float Ta本成et軍PS = GetTa本成et軍PS();
    if (C使本本ent軍PS < Ta本成et軍PS * 0.95f)
    {
        C使本本entMet本ics.bD本opped軍本a設置es = t本使e;
        C使本本entMet本ics.D本opped軍本a設置eCo使nt++;
    }
    else
    {
        C使本本entMet本ics.bD本opped軍本a設置es = false;
    }
    
    // Update ti設置e本
    Met本icsUpdateTi設置e本 += DeltaTi設置e;
    if (Met本icsUpdateTi設置e本 >= Met本icsUpdateInte本正al)
    {
        Met本icsUpdateTi設置e本 = 0.0f;
        UpdateAdapti正eQ使ality();
        Lo成Pe本fo本設置anceStats();
    }
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::ApplyOpti設置izationSettin成s()
{
    // Apply all c使本本ent settin成s to the en成ine
    // This wo使ld inte本face with 正a本io使s en成ine syste設置s
    
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Ve本bose, TEXT("Applied opti設置ization settin成s"));
}

float UMin成RTSVARPe本fo本設置anceOpti設置ize本::GetTa本成et軍PS() const
{
    switch (C使本本entSettin成s.Ta本成et軍本a設置eRate)
    {
        case EVAR軍本a設置eRate::軍PS30:  本et使本n 30.0f;
        case EVAR軍本a設置eRate::軍PS60:  本et使本n 60.0f;
        case EVAR軍本a設置eRate::軍PS72:  本et使本n 72.0f;
        case EVAR軍本a設置eRate::軍PS90:  本et使本n 90.0f;
        case EVAR軍本a設置eRate::軍PS120: 本et使本n 120.0f;
        defa使lt: 本et使本n 72.0f;
    }
}

正oid UMin成RTSVARPe本fo本設置anceOpti設置ize本::Lo成Pe本fo本設置anceStats()
{
    UE下LOG(Lo成Min成VARPe本fo本設置ance, Ve本bose, 
           TEXT("Pe本fo本設置ance: C使本本ent=%.1f, A正成=%.1f, Min=%.1f, Max=%.1f, D本ops=%d"),
           C使本本entMet本ics.C使本本ent軍PS,
           C使本本entMet本ics.A正e本a成e軍PS,
           C使本本entMet本ics.Min軍PS,
           C使本本entMet本ics.Max軍PS,
           C使本本entMet本ics.D本opped軍本a設置eCo使nt);
}
