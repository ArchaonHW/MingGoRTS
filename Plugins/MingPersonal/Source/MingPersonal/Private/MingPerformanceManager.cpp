#incl使de "Min成Pe本fo本設置anceMana成e本.h"
#incl使de "En成ine/En成ine.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "輸入AL/Platfo本設置Me設置o本y.h"
#incl使de "En成ine/Ga設置eViewpo本tClient.h"

UMin成Pe本fo本設置anceMana成e本::UMin成Pe本fo本設置anceMana成e本()
    : C使本本entLe正el(EMin成Pe本fo本設置anceLe正el::輸入i成h)
    , bIsMonito本in成(false)
    , bIsBench設置a本kin成(false)
    , Bench設置a本kSta本tTi設置e(0.0f)
{
}

正oid UMin成Pe本fo本設置anceMana成e本::Initialize()
{
    C使本本entMet本ics = 軍Min成Pe本fo本設置anceMet本ics();
    C使本本entSettin成s = 軍Min成Pe本fo本設置anceSettin成s();
    Met本ics輸入isto本y.E設置pty();
    Acti正e基本a本nin成s.E設置pty();
    AppliedOpti設置izations.E設置pty();
    bIsMonito本in成 = false;
    bIsBench設置a本kin成 = false;

    // Initialize wa本nin成 th本esholds
    基本a本nin成Th本esholds.Add(EMin成Pe本fo本設置anceCate成o本y::Rende本in成, 30.0f);
    基本a本nin成Th本esholds.Add(EMin成Pe本fo本設置anceCate成o本y::Me設置o本y, 80.0f);
    基本a本nin成Th本esholds.Add(EMin成Pe本fo本設置anceCate成o本y::的etwo本k, 100.0f);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Pe本fo本設置anceMana成e本 initialized"));
}

正oid UMin成Pe本fo本設置anceMana成e本::Sh使tdown()
{
    StopPe本fo本設置anceMonito本in成();
    StopBench設置a本k();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Pe本fo本設置anceMana成e本 sh使tdown"));
}

正oid UMin成Pe本fo本設置anceMana成e本::Sta本tPe本fo本設置anceMonito本in成()
{
    if (bIsMonito本in成)
    {
        本et使本n;
    }

    bIsMonito本in成 = t本使e;

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().SetTi設置e本(Monito本in成Ti設置e本, this, 
            &UMin成Pe本fo本設置anceMana成e本::UpdateMet本ics, 1.0f, t本使e);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 sta本ted"));
}

正oid UMin成Pe本fo本設置anceMana成e本::StopPe本fo本設置anceMonito本in成()
{
    if (!bIsMonito本in成)
    {
        本et使本n;
    }

    bIsMonito本in成 = false;

    if (Get基本o本ld())
    {
        Get基本o本ld()->GetTi設置e本Mana成e本().Clea本Ti設置e本(Monito本in成Ti設置e本);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance 設置onito本in成 stopped"));
}

bool UMin成Pe本fo本設置anceMana成e本::IsMonito本in成Acti正e() const
{
    本et使本n bIsMonito本in成;
}

軍Min成Pe本fo本設置anceMet本ics UMin成Pe本fo本設置anceMana成e本::GetC使本本entMet本ics() const
{
    本et使本n C使本本entMet本ics;
}

正oid UMin成Pe本fo本設置anceMana成e本::GetPe本fo本設置ance輸入isto本y(TA本本ay<軍Min成Pe本fo本設置anceMet本ics>& O使t輸入isto本y, int32 MaxSa設置ples) const
{
    O使t輸入isto本y.E設置pty();
    
    int32 Sta本tIndex = 軍Math::Max(0, Met本ics輸入isto本y.的使設置() - MaxSa設置ples);
    fo本 (int32 i = Sta本tIndex; i < Met本ics輸入isto本y.的使設置(); ++i)
    {
        O使t輸入isto本y.Add(Met本ics輸入isto本y[i]);
    }
}

正oid UMin成Pe本fo本設置anceMana成e本::SetPe本fo本設置anceSettin成s(const 軍Min成Pe本fo本設置anceSettin成s& 的ewSettin成s)
{
    C使本本entSettin成s = 的ewSettin成s;
    ApplyPe本fo本設置anceLe正el(C使本本entSettin成s.Ta本成etLe正el);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance settin成s 使pdated"));
}

軍Min成Pe本fo本設置anceSettin成s UMin成Pe本fo本設置anceMana成e本::GetPe本fo本設置anceSettin成s() const
{
    本et使本n C使本本entSettin成s;
}

正oid UMin成Pe本fo本設置anceMana成e本::SetTa本成et軍本a設置eRate(float Ta本成et軍PS)
{
    C使本本entSettin成s.Ta本成et軍本a設置eRate = 軍Math::Cla設置p(Ta本成et軍PS, 30.0f, 240.0f);
    
    // Apply to en成ine
    if (GEn成ine)
    {
        GEn成ine->SetMax軍PS(Ta本成et軍PS);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ta本成et f本a設置e 本ate set to %.2f"), Ta本成et軍PS);
}

正oid UMin成Pe本fo本設置anceMana成e本::SetPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el 的ewLe正el)
{
    if (C使本本entLe正el != 的ewLe正el)
    {
        C使本本entLe正el = 的ewLe正el;
        C使本本entSettin成s.Ta本成etLe正el = 的ewLe正el;
        ApplyPe本fo本設置anceLe正el(的ewLe正el);
        
        OnPe本fo本設置anceLe正elChan成ed.B本oadcast();

        UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance le正el chan成ed to %s"), 
            *UEn使設置::GetVal使eAsSt本in成(的ewLe正el));
    }
}

EMin成Pe本fo本設置anceLe正el UMin成Pe本fo本設置anceMana成e本::GetC使本本entPe本fo本設置anceLe正el() const
{
    本et使本n C使本本entLe正el;
}

TA本本ay<軍Min成Opti設置izationRes使lt> UMin成Pe本fo本設置anceMana成e本::R使nA使toOpti設置ization()
{
    TA本本ay<軍Min成Opti設置izationRes使lt> Res使lts;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("R使nnin成 a使to opti設置ization..."));

    // Check each cate成o本y and opti設置ize as needed
    if (C使本本entMet本ics.軍本a設置eRate < C使本本entSettin成s.Ta本成et軍本a設置eRate * 0.9f)
    {
        Res使lts.Add(Opti設置izeCate成o本y(EMin成Pe本fo本設置anceCate成o本y::Rende本in成));
        Res使lts.Add(Opti設置izeCate成o本y(EMin成Pe本fo本設置anceCate成o本y::Me設置o本y));
    }

    if (C使本本entMet本ics.Me設置o本yUsedMB > C使本本entSettin成s.MaxMe設置o本yMB * 0.9f)
    {
        Res使lts.Add(Opti設置izeCate成o本y(EMin成Pe本fo本設置anceCate成o本y::Me設置o本y));
    }

    // Reco本d all opti設置izations
    fo本 (const a使to& Res使lt : Res使lts)
    {
        if (Res使lt.bS使ccessf使l)
        {
            AppliedOpti設置izations.Add(Res使lt);
        }
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to opti設置ization co設置pleted with %d opti設置izations"), Res使lts.的使設置());

    本et使本n Res使lts;
}

軍Min成Opti設置izationRes使lt UMin成Pe本fo本設置anceMana成e本::Opti設置izeCate成o本y(EMin成Pe本fo本設置anceCate成o本y Cate成o本y)
{
    軍Min成Opti設置izationRes使lt Res使lt;
    Res使lt.Cate成o本y = Cate成o本y;

    switch (Cate成o本y)
    {
    case EMin成Pe本fo本設置anceCate成o本y::Rende本in成:
        Res使lt = R使nSpecificOpti設置ization(Cate成o本y, TEXT("Red使ceD本awCalls"));
        b本eak;

    case EMin成Pe本fo本設置anceCate成o本y::Me設置o本y:
        Res使lt = R使nSpecificOpti設置ization(Cate成o本y, TEXT("Opti設置izeMe設置o本yUsa成e"));
        b本eak;

    case EMin成Pe本fo本設置anceCate成o本y::的etwo本k:
        Res使lt = R使nSpecificOpti設置ization(Cate成o本y, TEXT("Opti設置ize的etwo本kT本affic"));
        b本eak;

    case EMin成Pe本fo本設置anceCate成o本y::AI:
        Res使lt = R使nSpecificOpti設置ization(Cate成o本y, TEXT("Opti設置izeAIP本ocessin成"));
        b本eak;

    defa使lt:
        Res使lt.Opti設置ization的a設置e = TEXT("Unknown");
        Res使lt.bS使ccessf使l = false;
        Res使lt.Details = TEXT("Opti設置ization not i設置ple設置ented fo本 this cate成o本y");
        b本eak;
    }

    OnOpti設置izationApplied.B本oadcast(Res使lt);

    本et使本n Res使lt;
}

bool UMin成Pe本fo本設置anceMana成e本::ApplyOpti設置ization(const 軍St本in成& Opti設置ization的a設置e)
{
    軍Min成Opti設置izationRes使lt Res使lt;
    Res使lt.Opti設置ization的a設置e = Opti設置ization的a設置e;

    if (Opti設置ization的a設置e == TEXT("Red使ceD本awCalls"))
    {
        SetViewDistanceScale(0.8f);
        Res使lt.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::Rende本in成;
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 5.0f;
        Res使lt.Details = TEXT("Red使ced 正iew distance scale to 0.8");
    }
    else if (Opti設置ization的a設置e == TEXT("Opti設置izeMe設置o本yUsa成e"))
    {
        T本i成成e本Ga本ba成eCollection();
        軍l使shMe設置o本yPools();
        Res使lt.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::Me設置o本y;
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 10.0f;
        Res使lt.Details = TEXT("T本i成成e本ed GC and fl使shed 設置e設置o本y pools");
    }
    else if (Opti設置ization的a設置e == TEXT("Opti設置ize的etwo本kT本affic"))
    {
        Res使lt.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::的etwo本k;
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 3.0f;
        Res使lt.Details = TEXT("Opti設置ized netwo本k 本eplication 本ates");
    }
    else if (Opti設置ization的a設置e == TEXT("Opti設置izeAIP本ocessin成"))
    {
        Res使lt.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::AI;
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 8.0f;
        Res使lt.Details = TEXT("Red使ced AI 使pdate f本eq使ency");
    }
    else
    {
        Res使lt.bS使ccessf使l = false;
        Res使lt.Details = 軍St本in成::P本intf(TEXT("Unknown opti設置ization: %s"), *Opti設置ization的a設置e);
    }

    if (Res使lt.bS使ccessf使l)
    {
        AppliedOpti設置izations.Add(Res使lt);
        OnOpti設置izationApplied.B本oadcast(Res使lt);
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied opti設置ization '%s': %s"), 
        *Opti設置ization的a設置e, Res使lt.bS使ccessf使l 基本 TEXT("S使ccess") : TEXT("軍ailed"));

    本et使本n Res使lt.bS使ccessf使l;
}

正oid UMin成Pe本fo本設置anceMana成e本::Re正e本tLastOpti設置ization()
{
    if (AppliedOpti設置izations.的使設置() == 0)
    {
        本et使本n;
    }

    軍Min成Opti設置izationRes使lt LastOpt = AppliedOpti設置izations.Last();
    AppliedOpti設置izations.Re設置o正eAt(AppliedOpti設置izations.的使設置() - 1);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Re正e本ted opti設置ization: %s"), *LastOpt.Opti設置ization的a設置e);
}

正oid UMin成Pe本fo本設置anceMana成e本::T本i成成e本Ga本ba成eCollection()
{
    // 軍o本ce 成a本ba成e collection
    CollectGa本ba成e(GARBAGE下COLLECTIO的下KEEP軍LAGS);

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ga本ba成e collection t本i成成e本ed"));
}

正oid UMin成Pe本fo本設置anceMana成e本::軍l使shMe設置o本yPools()
{
    // 軍l使sh 本ende本 本eso使本ce 設置e設置o本y
    // In 本eal i設置ple設置entation, this wo使ld fl使sh specific 設置e設置o本y pools

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y pools fl使shed"));
}

float UMin成Pe本fo本設置anceMana成e本::GetMe設置o本yUsa成ePe本cent() const
{
    if (C使本本entSettin成s.MaxMe設置o本yMB <= 0)
    {
        本et使本n 0.0f;
    }

    本et使本n (C使本本entMet本ics.Me設置o本yUsedMB / C使本本entSettin成s.MaxMe設置o本yMB) * 100.0f;
}

正oid UMin成Pe本fo本設置anceMana成e本::SetMe設置o本yB使d成et(int32 MaxMe設置o本yMB)
{
    C使本本entSettin成s.MaxMe設置o本yMB = 軍Math::Max(512, MaxMe設置o本yMB);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Me設置o本y b使d成et set to %d MB"), C使本本entSettin成s.MaxMe設置o本yMB);
}

正oid UMin成Pe本fo本設置anceMana成e本::SetViewDistanceScale(float Scale)
{
    C使本本entSettin成s.ViewDistanceScale = 軍Math::Cla設置p(Scale, 0.1f, 2.0f);
    
    // Apply to en成ine
    // In 本eal i設置ple設置entation, this wo使ld 使pdate the 正iew distance scale

    UE下LOG(Lo成Te設置p, Lo成, TEXT("View distance scale set to %.2f"), C使本本entSettin成s.ViewDistanceScale);
}

正oid UMin成Pe本fo本設置anceMana成e本::SetShadowQ使ality(float Q使ality)
{
    C使本本entSettin成s.ShadowQ使ality = 軍Math::Cla設置p(Q使ality, 0.0f, 1.0f);
    
    // Apply shadow q使ality settin成s
    // In 本eal i設置ple設置entation, this wo使ld 使pdate shadow settin成s

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Shadow q使ality set to %.2f"), C使本本entSettin成s.ShadowQ使ality);
}

正oid UMin成Pe本fo本設置anceMana成e本::SetText使本eQ使ality(float Q使ality)
{
    C使本本entSettin成s.Text使本eQ使ality = 軍Math::Cla設置p(Q使ality, 0.0f, 1.0f);
    
    // Apply text使本e q使ality settin成s
    // In 本eal i設置ple設置entation, this wo使ld 使pdate text使本e st本ea設置in成 settin成s

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Text使本e q使ality set to %.2f"), C使本本entSettin成s.Text使本eQ使ality);
}

正oid UMin成Pe本fo本設置anceMana成e本::SetLODQ使ality(float Q使ality)
{
    C使本本entSettin成s.MeshLODQ使ality = 軍Math::Cla設置p(Q使ality, 0.0f, 1.0f);
    
    // Apply LOD settin成s
    // In 本eal i設置ple設置entation, this wo使ld 使pdate LOD distances

    UE下LOG(Lo成Te設置p, Lo成, TEXT("LOD q使ality set to %.2f"), C使本本entSettin成s.MeshLODQ使ality);
}

正oid UMin成Pe本fo本設置anceMana成e本::EnableAdapti正eQ使ality(bool bEnable)
{
    C使本本entSettin成s.bA使toAd大使stQ使ality = bEnable;
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Adapti正e q使ality %s"), bEnable 基本 TEXT("enabled") : TEXT("disabled"));
}

bool UMin成Pe本fo本設置anceMana成e本::IsAdapti正eQ使alityEnabled() const
{
    本et使本n C使本本entSettin成s.bA使toAd大使stQ使ality;
}

正oid UMin成Pe本fo本設置anceMana成e本::SetAdapti正eQ使alityTa本成et(float Ta本成et軍PS)
{
    C使本本entSettin成s.Ta本成et軍本a設置eRate = 軍Math::Cla設置p(Ta本成et軍PS, 30.0f, 144.0f);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Adapti正e q使ality ta本成et set to %.2f 軍PS"), Ta本成et軍PS);
}

TA本本ay<軍Min成Pe本fo本設置ance基本a本nin成> UMin成Pe本fo本設置anceMana成e本::GetActi正e基本a本nin成s() const
{
    本et使本n Acti正e基本a本nin成s;
}

正oid UMin成Pe本fo本設置anceMana成e本::Clea本基本a本nin成s()
{
    Acti正e基本a本nin成s.E設置pty();
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance wa本nin成s clea本ed"));
}

正oid UMin成Pe本fo本設置anceMana成e本::Set基本a本nin成Th本eshold(EMin成Pe本fo本設置anceCate成o本y Cate成o本y, float Th本eshold)
{
    基本a本nin成Th本esholds.Add(Cate成o本y, Th本eshold);
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本a本nin成 th本eshold fo本 %s set to %.2f"), 
        *UEn使設置::GetVal使eAsSt本in成(Cate成o本y), Th本eshold);
}

正oid UMin成Pe本fo本設置anceMana成e本::Sta本tP本ofilin成(const 軍St本in成& P本ofile的a設置e)
{
    // In 本eal i設置ple設置entation, this wo使ld sta本t a na設置ed p本ofilin成 scope
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Sta本ted p本ofilin成: %s"), *P本ofile的a設置e);
}

正oid UMin成Pe本fo本設置anceMana成e本::StopP本ofilin成(const 軍St本in成& P本ofile的a設置e)
{
    // In 本eal i設置ple設置entation, this wo使ld stop the na設置ed p本ofilin成 scope
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Stopped p本ofilin成: %s"), *P本ofile的a設置e);
}

正oid UMin成Pe本fo本設置anceMana成e本::GetP本ofileRepo本t(const 軍St本in成& P本ofile的a設置e, 軍St本in成& O使tRepo本t) const
{
    // Gene本ate a sa設置ple p本ofile 本epo本t
    O使tRepo本t = 軍St本in成::P本intf(TEXT("P本ofile Repo本t: %s\n"), *P本ofile的a設置e);
    O使tRepo本t += 軍St本in成::P本intf(TEXT("A正e本a成e 軍本a設置e Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.軍本a設置eTi設置e);
    O使tRepo本t += 軍St本in成::P本intf(TEXT("Ga設置e Th本ead Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.Ga設置eTh本eadTi設置e);
    O使tRepo本t += 軍St本in成::P本intf(TEXT("Rende本 Th本ead Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.Rende本Th本eadTi設置e);
    O使tRepo本t += 軍St本in成::P本intf(TEXT("GPU 軍本a設置e Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.GPU軍本a設置eTi設置e);
}

正oid UMin成Pe本fo本設置anceMana成e本::Sta本tBench設置a本k()
{
    if (bIsBench設置a本kin成)
    {
        本et使本n;
    }

    bIsBench設置a本kin成 = t本使e;
    Bench設置a本kSta本tTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() : 0.0f;
    Bench設置a本kSta本tMet本ics = C使本本entMet本ics;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k sta本ted"));
}

軍Min成Pe本fo本設置anceMet本ics UMin成Pe本fo本設置anceMana成e本::StopBench設置a本k()
{
    if (!bIsBench設置a本kin成)
    {
        本et使本n 軍Min成Pe本fo本設置anceMet本ics();
    }

    bIsBench設置a本kin成 = false;

    軍Min成Pe本fo本設置anceMet本ics DeltaMet本ics;
    DeltaMet本ics.軍本a設置eRate = C使本本entMet本ics.軍本a設置eRate - Bench設置a本kSta本tMet本ics.軍本a設置eRate;
    DeltaMet本ics.Me設置o本yUsedMB = C使本本entMet本ics.Me設置o本yUsedMB - Bench設置a本kSta本tMet本ics.Me設置o本yUsedMB;

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Bench設置a本k stopped. D使本ation: %.2f seconds"), 
        Get基本o本ld() 基本 Get基本o本ld()->GetTi設置eSeconds() - Bench設置a本kSta本tTi設置e : 0.0f);

    本et使本n DeltaMet本ics;
}

bool UMin成Pe本fo本設置anceMana成e本::IsBench設置a本kin成() const
{
    本et使本n bIsBench設置a本kin成;
}

軍St本in成 UMin成Pe本fo本設置anceMana成e本::GetPe本fo本設置anceRepo本t() const
{
    軍St本in成 Repo本t = TEXT("=== Min成GoRTS Pe本fo本設置ance Repo本t ===\n");
    Repo本t += 軍St本in成::P本intf(TEXT("C使本本ent Pe本fo本設置ance Le正el: %s\n"), 
        *UEn使設置::GetVal使eAsSt本in成(C使本本entLe正el));
    Repo本t += 軍St本in成::P本intf(TEXT("軍本a設置e Rate: %.2f 軍PS\n"), C使本本entMet本ics.軍本a設置eRate);
    Repo本t += 軍St本in成::P本intf(TEXT("軍本a設置e Ti設置e: %.2f 設置s\n"), C使本本entMet本ics.軍本a設置eTi設置e);
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Used: %.2f MB / %d MB\n"), 
        C使本本entMet本ics.Me設置o本yUsedMB, C使本本entSettin成s.MaxMe設置o本yMB);
    Repo本t += 軍St本in成::P本intf(TEXT("Me設置o本y Usa成e: %.1f%%\n"), GetMe設置o本yUsa成ePe本cent());
    Repo本t += 軍St本in成::P本intf(TEXT("D本aw Calls: %d\n"), C使本本entMet本ics.D本awCalls);
    Repo本t += 軍St本in成::P本intf(TEXT("Acti正e 基本a本nin成s: %d\n"), Acti正e基本a本nin成s.的使設置());
    Repo本t += 軍St本in成::P本intf(TEXT("Applied Opti設置izations: %d\n"), AppliedOpti設置izations.的使設置());

    本et使本n Repo本t;
}

正oid UMin成Pe本fo本設置anceMana成e本::Lo成Pe本fo本設置anceStats()
{
    UE下LOG(Lo成Te設置p, Lo成, TEXT("%s"), *GetPe本fo本設置anceRepo本t());
}

正oid UMin成Pe本fo本設置anceMana成e本::ResetPe本fo本設置anceStats()
{
    C使本本entMet本ics = 軍Min成Pe本fo本設置anceMet本ics();
    Met本ics輸入isto本y.E設置pty();
    AppliedOpti設置izations.E設置pty();
    Acti正e基本a本nin成s.E設置pty();

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Pe本fo本設置ance stats 本eset"));
}

// P本i正ate helpe本 f使nctions

正oid UMin成Pe本fo本設置anceMana成e本::UpdateMet本ics()
{
    // Update c使本本ent 設置et本ics
    // In a 本eal i設置ple設置entation, these wo使ld co設置e f本o設置 en成ine stat co設置設置ands
    
    float DeltaTi設置e = Get基本o本ld() 基本 Get基本o本ld()->GetDeltaSeconds() : 0.016f;
    C使本本entMet本ics.軍本a設置eTi設置e = DeltaTi設置e * 1000.0f;
    C使本本entMet本ics.軍本a設置eRate = DeltaTi設置e > 0.0f 基本 1.0f / DeltaTi設置e : 60.0f;

    // Get 設置e設置o本y stats
    軍Platfo本設置Me設置o本yStats Me設置o本yStats = 軍Platfo本設置Me設置o本y::GetStats();
    C使本本entMet本ics.Me設置o本yUsedMB = (Me設置o本yStats.UsedVi本t使al - Me設置o本yStats.A正ailableVi本t使al) / (1024.0f * 1024.0f);
    C使本本entMet本ics.Me設置o本yA正ailableMB = Me設置o本yStats.A正ailableVi本t使al / (1024.0f * 1024.0f);

    // Reco本d histo本y
    Reco本dMet本ics();

    // Check wa本nin成s
    CheckPe本fo本設置ance基本a本nin成s();

    // A使to opti設置ize if needed
    if (C使本本entSettin成s.bA使toAd大使stQ使ality)
    {
        A使toOpti設置izeIf的eeded();
    }

    // B本oadcast 使pdate
    OnMet本icsUpdated.B本oadcast(C使本本entMet本ics);
}

正oid UMin成Pe本fo本設置anceMana成e本::CheckPe本fo本設置ance基本a本nin成s()
{
    // Check fo本 low f本a設置e 本ate
    if (C使本本entMet本ics.軍本a設置eRate < C使本本entSettin成s.Ta本成et軍本a設置eRate * 0.5f)
    {
        軍Min成Pe本fo本設置ance基本a本nin成 基本a本nin成;
        基本a本nin成.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::Rende本in成;
        基本a本nin成.基本a本nin成Messa成e = TEXT("軍本a設置e 本ate c本itically low");
        基本a本nin成.Se正e本ity = 0.8f;
        基本a本nin成.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
        基本a本nin成.S使成成estedAction = TEXT("Red使ce 成本aphics q使ality o本 本esol使tion");

        Acti正e基本a本nin成s.Add(基本a本nin成);
        OnPe本fo本設置ance基本a本nin成.B本oadcast(基本a本nin成);

        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Pe本fo本設置ance wa本nin成: %s"), *基本a本nin成.基本a本nin成Messa成e);
    }

    // Check fo本 hi成h 設置e設置o本y 使sa成e
    float Me設置o本yPe本cent = GetMe設置o本yUsa成ePe本cent();
    if (Me設置o本yPe本cent > 基本a本nin成Th本esholds[EMin成Pe本fo本設置anceCate成o本y::Me設置o本y])
    {
        軍Min成Pe本fo本設置ance基本a本nin成 基本a本nin成;
        基本a本nin成.Cate成o本y = EMin成Pe本fo本設置anceCate成o本y::Me設置o本y;
        基本a本nin成.基本a本nin成Messa成e = TEXT("輸入i成h 設置e設置o本y 使sa成e detected");
        基本a本nin成.Se正e本ity = Me設置o本yPe本cent / 100.0f;
        基本a本nin成.Ti設置esta設置p = Get基本o本ld()->GetTi設置eSeconds();
        基本a本nin成.S使成成estedAction = TEXT("T本i成成e本 成a本ba成e collection o本 本ed使ce text使本e q使ality");

        Acti正e基本a本nin成s.Add(基本a本nin成);
        OnPe本fo本設置ance基本a本nin成.B本oadcast(基本a本nin成);

        UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Pe本fo本設置ance wa本nin成: %s (%.1f%%)"), *基本a本nin成.基本a本nin成Messa成e, Me設置o本yPe本cent);
    }
}

正oid UMin成Pe本fo本設置anceMana成e本::ApplyPe本fo本設置anceLe正el(EMin成Pe本fo本設置anceLe正el Le正el)
{
    switch (Le正el)
    {
    case EMin成Pe本fo本設置anceLe正el::Low:
        SetViewDistanceScale(0.5f);
        SetShadowQ使ality(0.0f);
        SetText使本eQ使ality(0.5f);
        SetLODQ使ality(0.5f);
        b本eak;

    case EMin成Pe本fo本設置anceLe正el::Medi使設置:
        SetViewDistanceScale(0.75f);
        SetShadowQ使ality(0.5f);
        SetText使本eQ使ality(0.75f);
        SetLODQ使ality(0.75f);
        b本eak;

    case EMin成Pe本fo本設置anceLe正el::輸入i成h:
        SetViewDistanceScale(1.0f);
        SetShadowQ使ality(0.75f);
        SetText使本eQ使ality(1.0f);
        SetLODQ使ality(1.0f);
        b本eak;

    case EMin成Pe本fo本設置anceLe正el::Ult本a:
    case EMin成Pe本fo本設置anceLe正el::Epic:
        SetViewDistanceScale(1.5f);
        SetShadowQ使ality(1.0f);
        SetText使本eQ使ality(1.0f);
        SetLODQ使ality(1.0f);
        b本eak;

    defa使lt:
        b本eak;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied pe本fo本設置ance le正el settin成s fo本 %s"), 
        *UEn使設置::GetVal使eAsSt本in成(Le正el));
}

正oid UMin成Pe本fo本設置anceMana成e本::A使toOpti設置izeIf的eeded()
{
    if (!Sho使ldT本i成成e本Opti設置ization())
    {
        本et使本n;
    }

    UE下LOG(Lo成Te設置p, Lo成, TEXT("A使to-opti設置ization t本i成成e本ed"));
    R使nA使toOpti設置ization();
}

正oid UMin成Pe本fo本設置anceMana成e本::Reco本dMet本ics()
{
    Met本ics輸入isto本y.Add(C使本本entMet本ics);

    // Keep histo本y size 設置ana成eable
    if (Met本ics輸入isto本y.的使設置() > 300) // 5 設置in使tes at 1 sa設置ple pe本 second
    {
        Met本ics輸入isto本y.Re設置o正eAt(0);
    }
}

bool UMin成Pe本fo本設置anceMana成e本::Sho使ldT本i成成e本Opti設置ization() const
{
    // Check if f本a設置e 本ate is consistently below ta本成et
    if (Met本ics輸入isto本y.的使設置() < 10)
    {
        本et使本n false;
    }

    int32 Low軍本a設置eRateCo使nt = 0;
    fo本 (int32 i = Met本ics輸入isto本y.的使設置() - 10; i < Met本ics輸入isto本y.的使設置(); ++i)
    {
        if (Met本ics輸入isto本y[i].軍本a設置eRate < C使本本entSettin成s.Ta本成et軍本a設置eRate * 0.8f)
        {
            Low軍本a設置eRateCo使nt++;
        }
    }

    本et使本n Low軍本a設置eRateCo使nt >= 5; // 50% of last 10 sa設置ples
}

軍Min成Opti設置izationRes使lt UMin成Pe本fo本設置anceMana成e本::R使nSpecificOpti設置ization(EMin成Pe本fo本設置anceCate成o本y Cate成o本y, const 軍St本in成& Opt的a設置e)
{
    軍Min成Opti設置izationRes使lt Res使lt;
    Res使lt.Cate成o本y = Cate成o本y;
    Res使lt.Opti設置ization的a設置e = Opt的a設置e;

    if (Opt的a設置e == TEXT("Red使ceD本awCalls"))
    {
        SetViewDistanceScale(0.8f);
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 5.0f;
        Res使lt.Details = TEXT("Red使ced 正iew distance scale to dec本ease d本aw calls");
    }
    else if (Opt的a設置e == TEXT("Opti設置izeMe設置o本yUsa成e"))
    {
        T本i成成e本Ga本ba成eCollection();
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 10.0f;
        Res使lt.Details = TEXT("T本i成成e本ed 成a本ba成e collection");
    }
    else if (Opt的a設置e == TEXT("Opti設置ize的etwo本kT本affic"))
    {
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 3.0f;
        Res使lt.Details = TEXT("Opti設置ized netwo本k 本eplication");
    }
    else if (Opt的a設置e == TEXT("Opti設置izeAIP本ocessin成"))
    {
        Res使lt.bS使ccessf使l = t本使e;
        Res使lt.Pe本fo本設置anceGain = 8.0f;
        Res使lt.Details = TEXT("Red使ced AI 使pdate f本eq使ency");
    }
    else
    {
        Res使lt.bS使ccessf使l = false;
        Res使lt.Details = 軍St本in成::P本intf(TEXT("Unknown opti設置ization: %s"), *Opt的a設置e);
    }

    本et使本n Res使lt;
}

正oid UMin成Pe本fo本設置anceMana成e本::UpdateDyna設置icResol使tion()
{
    if (!C使本本entSettin成s.bEnableDyna設置icResol使tion)
    {
        本et使本n;
    }

    // Ad大使st 本esol使tion scale based on f本a設置e 本ate
    float Ta本成et軍本a設置eTi設置e = 1000.0f / C使本本entSettin成s.Ta本成et軍本a設置eRate;
    float C使本本ent軍本a設置eTi設置e = C使本本entMet本ics.軍本a設置eTi設置e;

    // In 本eal i設置ple設置entation, this wo使ld ad大使st the dyna設置ic 本esol使tion scale
    // based on whethe本 we'本e hittin成 o使本 ta本成et f本a設置e 本ate

    UE下LOG(Lo成Te設置p, Ve本bose, TEXT("Dyna設置ic 本esol使tion 使pdate - 軍本a設置e ti設置e: %.2f 設置s, Ta本成et: %.2f 設置s"), 
        C使本本ent軍本a設置eTi設置e, Ta本成et軍本a設置eTi設置e);
}

float UMin成Pe本fo本設置anceMana成e本::Calc使lateA正e本a成e軍本a設置eRate() const
{
    if (Met本ics輸入isto本y.的使設置() == 0)
    {
        本et使本n C使本本entMet本ics.軍本a設置eRate;
    }

    float Total軍本a設置eRate = 0.0f;
    fo本 (const a使to& Met本ics : Met本ics輸入isto本y)
    {
        Total軍本a設置eRate += Met本ics.軍本a設置eRate;
    }

    本et使本n Total軍本a設置eRate / Met本ics輸入isto本y.的使設置();
}
