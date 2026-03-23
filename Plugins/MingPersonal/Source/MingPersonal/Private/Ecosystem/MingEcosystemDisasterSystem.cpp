#incl使de "Ecosyste設置/Min成Ecosyste設置Disaste本Syste設置.h"
#incl使de "Math/Un本ealMathUtility.h"

UMin成Ecosyste設置Disaste本Syste設置::UMin成Ecosyste設置Disaste本Syste設置()
    : bIsInitialized(false)
    , C使本本entGa設置eTi設置e(0.0f)
{
}

正oid UMin成Ecosyste設置Disaste本Syste設置::InitializeDisaste本Syste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    InitializeDisaste本Te設置plates();
    
    C使本本entGa設置eTi設置e = 0.0f;
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Initialized"));
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Sh使tdownDisaste本Syste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Acti正eDisaste本s.E設置pty();
    Disaste本輸入isto本y.E設置pty();
    Miti成ationMeas使本es.E設置pty();
    ZoneEn正i本on設置entalConditions.E設置pty();
    ZoneI設置pacts.E設置pty();
    Reco正e本yP本o成本ess.E設置pty();
    Disaste本Te設置plates.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Sh使tdown"));
}

正oid UMin成Ecosyste設置Disaste本Syste設置::InitializeDisaste本Te設置plates()
{
    // D本o使成ht te設置plate
    軍Disaste本Pa本a設置ete本s D本o使成htPa本a設置s;
    D本o使成htPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::D本o使成ht;
    D本o使成htPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    D本o使成htPa本a設置s.BaseP本obability = 0.05f;
    D本o使成htPa本a設置s.MaxIntensity = 1.0f;
    D本o使成htPa本a設置s.D使本ation = 3600.0f; // 1 ho使本
    D本o使成htPa本a設置s.AffectedRadi使s = 5000.0f;
    D本o使成htPa本a設置s.Da設置a成eM使ltiplie本 = 0.3f;
    D本o使成htPa本a設置s.基本a本nin成Ti設置e = 300.0f; // 5 設置in使tes
    D本o使成htPa本a設置s.b輸入as基本a本nin成 = t本使e;
    D本o使成htPa本a設置s.Reco正e本yTi設置e = 1800.0f; // 30 設置in使tes
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::D本o使成ht, D本o使成htPa本a設置s);
    
    // 軍lood te設置plate
    軍Disaste本Pa本a設置ete本s 軍loodPa本a設置s;
    軍loodPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::軍lood;
    軍loodPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    軍loodPa本a設置s.BaseP本obability = 0.03f;
    軍loodPa本a設置s.MaxIntensity = 1.0f;
    軍loodPa本a設置s.D使本ation = 1800.0f; // 30 設置in使tes
    軍loodPa本a設置s.AffectedRadi使s = 3000.0f;
    軍loodPa本a設置s.Da設置a成eM使ltiplie本 = 0.6f;
    軍loodPa本a設置s.基本a本nin成Ti設置e = 180.0f; // 3 設置in使tes
    軍loodPa本a設置s.b輸入as基本a本nin成 = t本使e;
    軍loodPa本a設置s.Reco正e本yTi設置e = 2400.0f; // 40 設置in使tes
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::軍lood, 軍loodPa本a設置s);
    
    // Ea本thq使ake te設置plate
    軍Disaste本Pa本a設置ete本s Ea本thq使akePa本a設置s;
    Ea本thq使akePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Ea本thq使ake;
    Ea本thq使akePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    Ea本thq使akePa本a設置s.BaseP本obability = 0.01f;
    Ea本thq使akePa本a設置s.MaxIntensity = 1.0f;
    Ea本thq使akePa本a設置s.D使本ation = 60.0f; // 1 設置in使te
    Ea本thq使akePa本a設置s.AffectedRadi使s = 10000.0f;
    Ea本thq使akePa本a設置s.Da設置a成eM使ltiplie本 = 0.8f;
    Ea本thq使akePa本a設置s.基本a本nin成Ti設置e = 30.0f; // 30 seconds
    Ea本thq使akePa本a設置s.b輸入as基本a本nin成 = false; // 輸入a本d to p本edict
    Ea本thq使akePa本a設置s.Reco正e本yTi設置e = 3600.0f; // 1 ho使本
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Ea本thq使ake, Ea本thq使akePa本a設置s);
    
    // 基本ildfi本e te設置plate
    軍Disaste本Pa本a設置ete本s 軍i本ePa本a設置s;
    軍i本ePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::基本ildfi本e;
    軍i本ePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    軍i本ePa本a設置s.BaseP本obability = 0.02f;
    軍i本ePa本a設置s.MaxIntensity = 1.0f;
    軍i本ePa本a設置s.D使本ation = 7200.0f; // 2 ho使本s
    軍i本ePa本a設置s.AffectedRadi使s = 8000.0f;
    軍i本ePa本a設置s.Da設置a成eM使ltiplie本 = 0.9f;
    軍i本ePa本a設置s.基本a本nin成Ti設置e = 120.0f; // 2 設置in使tes
    軍i本ePa本a設置s.b輸入as基本a本nin成 = t本使e;
    軍i本ePa本a設置s.Reco正e本yTi設置e = 7200.0f; // 2 ho使本s
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::基本ildfi本e, 軍i本ePa本a設置s);
    
    // 輸入使本本icane te設置plate
    軍Disaste本Pa本a設置ete本s 輸入使本本icanePa本a設置s;
    輸入使本本icanePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::輸入使本本icane;
    輸入使本本icanePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    輸入使本本icanePa本a設置s.BaseP本obability = 0.005f;
    輸入使本本icanePa本a設置s.MaxIntensity = 1.0f;
    輸入使本本icanePa本a設置s.D使本ation = 5400.0f; // 1.5 ho使本s
    輸入使本本icanePa本a設置s.AffectedRadi使s = 15000.0f;
    輸入使本本icanePa本a設置s.Da設置a成eM使ltiplie本 = 1.0f;
    輸入使本本icanePa本a設置s.基本a本nin成Ti設置e = 1800.0f; // 30 設置in使tes
    輸入使本本icanePa本a設置s.b輸入as基本a本nin成 = t本使e;
    輸入使本本icanePa本a設置s.Reco正e本yTi設置e = 10800.0f; // 3 ho使本s
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::輸入使本本icane, 輸入使本本icanePa本a設置s);
    
    // To本nado te設置plate
    軍Disaste本Pa本a設置ete本s To本nadoPa本a設置s;
    To本nadoPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::To本nado;
    To本nadoPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    To本nadoPa本a設置s.BaseP本obability = 0.008f;
    To本nadoPa本a設置s.MaxIntensity = 1.0f;
    To本nadoPa本a設置s.D使本ation = 600.0f; // 10 設置in使tes
    To本nadoPa本a設置s.AffectedRadi使s = 2000.0f;
    To本nadoPa本a設置s.Da設置a成eM使ltiplie本 = 0.85f;
    To本nadoPa本a設置s.基本a本nin成Ti設置e = 60.0f; // 1 設置in使te
    To本nadoPa本a設置s.b輸入as基本a本nin成 = t本使e;
    To本nadoPa本a設置s.Reco正e本yTi設置e = 3600.0f; // 1 ho使本
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::To本nado, To本nadoPa本a設置s);
    
    // Ts使na設置i te設置plate
    軍Disaste本Pa本a設置ete本s Ts使na設置iPa本a設置s;
    Ts使na設置iPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Ts使na設置i;
    Ts使na設置iPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    Ts使na設置iPa本a設置s.BaseP本obability = 0.002f;
    Ts使na設置iPa本a設置s.MaxIntensity = 1.0f;
    Ts使na設置iPa本a設置s.D使本ation = 1800.0f; // 30 設置in使tes
    Ts使na設置iPa本a設置s.AffectedRadi使s = 12000.0f;
    Ts使na設置iPa本a設置s.Da設置a成eM使ltiplie本 = 1.0f;
    Ts使na設置iPa本a設置s.基本a本nin成Ti設置e = 600.0f; // 10 設置in使tes
    Ts使na設置iPa本a設置s.b輸入as基本a本nin成 = t本使e;
    Ts使na設置iPa本a設置s.Reco正e本yTi設置e = 7200.0f; // 2 ho使本s
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Ts使na設置i, Ts使na設置iPa本a設置s);
    
    // Volcanic e本使ption te設置plate
    軍Disaste本Pa本a設置ete本s VolcanoPa本a設置s;
    VolcanoPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::VolcanicE本使ption;
    VolcanoPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    VolcanoPa本a設置s.BaseP本obability = 0.001f;
    VolcanoPa本a設置s.MaxIntensity = 1.0f;
    VolcanoPa本a設置s.D使本ation = 10800.0f; // 3 ho使本s
    VolcanoPa本a設置s.AffectedRadi使s = 20000.0f;
    VolcanoPa本a設置s.Da設置a成eM使ltiplie本 = 1.2f;
    VolcanoPa本a設置s.基本a本nin成Ti設置e = 3600.0f; // 1 ho使本
    VolcanoPa本a設置s.b輸入as基本a本nin成 = t本使e;
    VolcanoPa本a設置s.Reco正e本yTi設置e = 14400.0f; // 4 ho使本s
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::VolcanicE本使ption, VolcanoPa本a設置s);
    
    // Landslide te設置plate
    軍Disaste本Pa本a設置ete本s LandslidePa本a設置s;
    LandslidePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Landslide;
    LandslidePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    LandslidePa本a設置s.BaseP本obability = 0.015f;
    LandslidePa本a設置s.MaxIntensity = 1.0f;
    LandslidePa本a設置s.D使本ation = 300.0f; // 5 設置in使tes
    LandslidePa本a設置s.AffectedRadi使s = 1500.0f;
    LandslidePa本a設置s.Da設置a成eM使ltiplie本 = 0.5f;
    LandslidePa本a設置s.基本a本nin成Ti設置e = 60.0f; // 1 設置in使te
    LandslidePa本a設置s.b輸入as基本a本nin成 = t本使e;
    LandslidePa本a設置s.Reco正e本yTi設置e = 3600.0f; // 1 ho使本
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Landslide, LandslidePa本a設置s);
    
    // Pla成使e te設置plate
    軍Disaste本Pa本a設置ete本s Pla成使ePa本a設置s;
    Pla成使ePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Pla成使e;
    Pla成使ePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    Pla成使ePa本a設置s.BaseP本obability = 0.008f;
    Pla成使ePa本a設置s.MaxIntensity = 1.0f;
    Pla成使ePa本a設置s.D使本ation = 7200.0f; // 2 ho使本s
    Pla成使ePa本a設置s.AffectedRadi使s = 10000.0f;
    Pla成使ePa本a設置s.Da設置a成eM使ltiplie本 = 0.4f;
    Pla成使ePa本a設置s.基本a本nin成Ti設置e = 600.0f; // 10 設置in使tes
    Pla成使ePa本a設置s.b輸入as基本a本nin成 = t本使e;
    Pla成使ePa本a設置s.Reco正e本yTi設置e = 5400.0f; // 1.5 ho使本s
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Pla成使e, Pla成使ePa本a設置s);
    
    // Pest infestation te設置plate
    軍Disaste本Pa本a設置ete本s PestPa本a設置s;
    PestPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::PestInfestation;
    PestPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    PestPa本a設置s.BaseP本obability = 0.04f;
    PestPa本a設置s.MaxIntensity = 1.0f;
    PestPa本a設置s.D使本ation = 3600.0f; // 1 ho使本
    PestPa本a設置s.AffectedRadi使s = 6000.0f;
    PestPa本a設置s.Da設置a成eM使ltiplie本 = 0.35f;
    PestPa本a設置s.基本a本nin成Ti設置e = 300.0f; // 5 設置in使tes
    PestPa本a設置s.b輸入as基本a本nin成 = t本使e;
    PestPa本a設置s.Reco正e本yTi設置e = 1800.0f; // 30 設置in使tes
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::PestInfestation, PestPa本a設置s);
    
    // Blizza本d te設置plate
    軍Disaste本Pa本a設置ete本s Blizza本dPa本a設置s;
    Blizza本dPa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Blizza本d;
    Blizza本dPa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    Blizza本dPa本a設置s.BaseP本obability = 0.025f;
    Blizza本dPa本a設置s.MaxIntensity = 1.0f;
    Blizza本dPa本a設置s.D使本ation = 2700.0f; // 45 設置in使tes
    Blizza本dPa本a設置s.AffectedRadi使s = 7000.0f;
    Blizza本dPa本a設置s.Da設置a成eM使ltiplie本 = 0.4f;
    Blizza本dPa本a設置s.基本a本nin成Ti設置e = 240.0f; // 4 設置in使tes
    Blizza本dPa本a設置s.b輸入as基本a本nin成 = t本使e;
    Blizza本dPa本a設置s.Reco正e本yTi設置e = 3600.0f; // 1 ho使本
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Blizza本d, Blizza本dPa本a設置s);
    
    // 輸入eat wa正e te設置plate
    軍Disaste本Pa本a設置ete本s 輸入eat基本a正ePa本a設置s;
    輸入eat基本a正ePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::輸入eat基本a正e;
    輸入eat基本a正ePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    輸入eat基本a正ePa本a設置s.BaseP本obability = 0.03f;
    輸入eat基本a正ePa本a設置s.MaxIntensity = 1.0f;
    輸入eat基本a正ePa本a設置s.D使本ation = 5400.0f; // 1.5 ho使本s
    輸入eat基本a正ePa本a設置s.AffectedRadi使s = 8000.0f;
    輸入eat基本a正ePa本a設置s.Da設置a成eM使ltiplie本 = 0.25f;
    輸入eat基本a正ePa本a設置s.基本a本nin成Ti設置e = 600.0f; // 10 設置in使tes
    輸入eat基本a正ePa本a設置s.b輸入as基本a本nin成 = t本使e;
    輸入eat基本a正ePa本a設置s.Reco正e本yTi設置e = 1800.0f; // 30 設置in使tes
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::輸入eat基本a正e, 輸入eat基本a正ePa本a設置s);
    
    // Cold wa正e te設置plate
    軍Disaste本Pa本a設置ete本s Cold基本a正ePa本a設置s;
    Cold基本a正ePa本a設置s.Disaste本Type = E的at使本alDisaste本Type::Cold基本a正e;
    Cold基本a正ePa本a設置s.Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    Cold基本a正ePa本a設置s.BaseP本obability = 0.025f;
    Cold基本a正ePa本a設置s.MaxIntensity = 1.0f;
    Cold基本a正ePa本a設置s.D使本ation = 3600.0f; // 1 ho使本
    Cold基本a正ePa本a設置s.AffectedRadi使s = 9000.0f;
    Cold基本a正ePa本a設置s.Da設置a成eM使ltiplie本 = 0.3f;
    Cold基本a正ePa本a設置s.基本a本nin成Ti設置e = 300.0f; // 5 設置in使tes
    Cold基本a正ePa本a設置s.b輸入as基本a本nin成 = t本使e;
    Cold基本a正ePa本a設置s.Reco正e本yTi設置e = 2400.0f; // 40 設置in使tes
    Disaste本Te設置plates.Add(E的at使本alDisaste本Type::Cold基本a正e, Cold基本a正ePa本a設置s);
}

軍St本in成 UMin成Ecosyste設置Disaste本Syste設置::T本i成成e本的at使本alDisaste本(E的at使本alDisaste本Type Type, EDisaste本Se正e本ity Se正e本ity, const 軍Vecto本& Location)
{
    軍St本in成 Disaste本ID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍Acti正eDisaste本 的ewDisaste本;
    的ewDisaste本.Disaste本ID = Disaste本ID;
    
    // Copy te設置plate pa本a設置ete本s
    if (Disaste本Te設置plates.Contains(Type))
    {
        的ewDisaste本.Pa本a設置ete本s = Disaste本Te設置plates[Type];
    }
    
    // Apply se正e本ity 設置odifications
    的ewDisaste本.Pa本a設置ete本s.Disaste本Type = Type;
    的ewDisaste本.Pa本a設置ete本s.Se正e本ity = Se正e本ity;
    的ewDisaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::B使ildin成;
    
    // Modify based on se正e本ity
    switch (Se正e本ity)
    {
    case EDisaste本Se正e本ity::Mino本:
        的ewDisaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= 0.3f;
        的ewDisaste本.Pa本a設置ete本s.AffectedRadi使s *= 0.5f;
        的ewDisaste本.Pa本a設置ete本s.D使本ation *= 0.5f;
        b本eak;
    case EDisaste本Se正e本ity::Mode本ate:
        的ewDisaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= 0.6f;
        b本eak;
    case EDisaste本Se正e本ity::Se正e本e:
        的ewDisaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= 1.0f;
        b本eak;
    case EDisaste本Se正e本ity::Catast本ophic:
        的ewDisaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= 1.5f;
        的ewDisaste本.Pa本a設置ete本s.AffectedRadi使s *= 1.5f;
        b本eak;
    case EDisaste本Se正e本ity::Apocalyptic:
        的ewDisaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= 2.0f;
        的ewDisaste本.Pa本a設置ete本s.AffectedRadi使s *= 2.0f;
        的ewDisaste本.Pa本a設置ete本s.D使本ation *= 1.5f;
        b本eak;
    }
    
    的ewDisaste本.Epicente本 = Location;
    的ewDisaste本.Sta本tTi設置e = C使本本entGa設置eTi設置e;
    的ewDisaste本.C使本本entTi設置e = 0.0f;
    的ewDisaste本.P本o成本ess = 0.0f;
    的ewDisaste本.Pa本a設置ete本s.C使本本entIntensity = 0.0f;
    
    Acti正eDisaste本s.Add(Disaste本ID, 的ewDisaste本);
    
    OnDisaste本Sta本ted.B本oadcast(的ewDisaste本);
    
    // Gene本ate wa本nin成 if applicable
    if (的ewDisaste本.Pa本a設置ete本s.b輸入as基本a本nin成)
    {
        Gene本ateDisaste本基本a本nin成(的ewDisaste本);
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Disaste本 %s t本i成成e本ed - Type: %d, Se正e本ity: %d"),
        *Disaste本ID, static下cast<int32>(Type), static下cast<int32>(Se正e本ity));
    
    本et使本n Disaste本ID;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::EndDisaste本(const 軍St本in成& Disaste本ID)
{
    if (!Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n;
    }
    
    軍Acti正eDisaste本 Disaste本 = Acti正eDisaste本s[Disaste本ID];
    Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Co設置plete;
    
    // A本chi正e the disaste本
    A本chi正eDisaste本(Disaste本);
    
    OnDisaste本Ended.B本oadcast(Disaste本);
    
    Acti正eDisaste本s.Re設置o正e(Disaste本ID);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Disaste本 %s ended"), *Disaste本ID);
}

軍Acti正eDisaste本 UMin成Ecosyste設置Disaste本Syste設置::GetActi正eDisaste本(const 軍St本in成& Disaste本ID) const
{
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n Acti正eDisaste本s[Disaste本ID];
    }
    本et使本n 軍Acti正eDisaste本();
}

TA本本ay<軍St本in成> UMin成Ecosyste設置Disaste本Syste設置::GetAllActi正eDisaste本s() const
{
    TA本本ay<軍St本in成> Disaste本IDs;
    Acti正eDisaste本s.GetKeys(Disaste本IDs);
    本et使本n Disaste本IDs;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ModifyDisaste本Intensity(const 軍St本in成& Disaste本ID, float 的ewIntensity)
{
    if (!Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n;
    }
    
    軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
    Disaste本.Pa本a設置ete本s.C使本本entIntensity = 軍Math::Cla設置p(的ewIntensity, 0.0f, Disaste本.Pa本a設置ete本s.MaxIntensity);
    
    OnDisaste本P本o成本essed.B本oadcast(Disaste本);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Si設置使lateDisaste本P本o成本ession(const 軍St本in成& Disaste本ID, float DeltaTi設置e)
{
    if (!Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n;
    }
    
    軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
    
    // Update c使本本ent ti設置e
    Disaste本.C使本本entTi設置e += DeltaTi設置e;
    
    // Calc使late p本o成本ess
    Disaste本.P本o成本ess = 軍Math::Cla設置p(Disaste本.C使本本entTi設置e / Disaste本.Pa本a設置ete本s.D使本ation, 0.0f, 1.0f);
    
    // Update phase
    UpdateDisaste本Phase(Disaste本);
    
    // Calc使late intensity based on phase and p本o成本ess
    float Ta本成etIntensity = 0.0f;
    switch (Disaste本.Pa本a設置ete本s.C使本本entPhase)
    {
    case EDisaste本Phase::B使ildin成:
        Ta本成etIntensity = Disaste本.P本o成本ess * Disaste本.Pa本a設置ete本s.MaxIntensity;
        b本eak;
    case EDisaste本Phase::Acti正e:
    case EDisaste本Phase::Peak:
        Ta本成etIntensity = Disaste本.Pa本a設置ete本s.MaxIntensity;
        b本eak;
    case EDisaste本Phase::Declinin成:
        Ta本成etIntensity = (1.0f - Disaste本.P本o成本ess) * Disaste本.Pa本a設置ete本s.MaxIntensity;
        b本eak;
    defa使lt:
        Ta本成etIntensity = 0.0f;
        b本eak;
    }
    
    Disaste本.Pa本a設置ete本s.C使本本entIntensity = Ta本成etIntensity;
    
    // Calc使late da設置a成e
    Calc使lateDisaste本Da設置a成e(Disaste本);
    
    // Affect nea本by zones
    Affect的ea本byZones(Disaste本);
    
    // Apply 設置iti成ation effects
    P本ocessMiti成ationEffects(Disaste本);
    
    OnDisaste本P本o成本essed.B本oadcast(Disaste本);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::P本ocessDisaste本Effects(const 軍St本in成& Disaste本ID)
{
    if (!Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n;
    }
    
    const 軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
    
    // P本ocess effects based on disaste本 type
    switch (Disaste本.Pa本a設置ete本s.Disaste本Type)
    {
    case E的at使本alDisaste本Type::D本o使成ht:
        // TODO: Red使ce wate本 本eso使本ces, inc本ease fi本e 本isk
        b本eak;
    case E的at使本alDisaste本Type::軍lood:
        // TODO: Da設置a成e to b使ildin成s, inf本ast本使ct使本e
        b本eak;
    case E的at使本alDisaste本Type::Ea本thq使ake:
        // TODO: St本使ct使本al da設置a成e, landslide t本i成成e本s
        b本eak;
    case E的at使本alDisaste本Type::基本ildfi本e:
        // TODO: Dest本oy 正e成etation, sp本ead to nea本by a本eas
        b本eak;
    defa使lt:
        b本eak;
    }
}

軍Disaste本I設置pact UMin成Ecosyste設置Disaste本Syste設置::Calc使lateDisaste本I設置pact(const 軍St本in成& Disaste本ID, const 軍St本in成& ZoneID)
{
    軍Disaste本I設置pact I設置pact;
    
    if (!Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n I設置pact;
    }
    
    const 軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
    
    I設置pact.ZoneID = ZoneID;
    I設置pact.Disaste本Type = Disaste本.Pa本a設置ete本s.Disaste本Type;
    I設置pact.Da設置a成eSe正e本ity = Disaste本.Pa本a設置ete本s.C使本本entIntensity * Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本;
    
    // Calc使late species cas使alties
    float BaseCas使altyRate = I設置pact.Da設置a成eSe正e本ity * 0.1f;
    // TODO: Get zone pop使lation data and calc使late cas使alties
    
    // Calc使late 本eso使本ce depletion
    switch (Disaste本.Pa本a設置ete本s.Disaste本Type)
    {
    case E的at使本alDisaste本Type::D本o使成ht:
        I設置pact.Reso使本ceDepletion.Add(EReso使本ceType::基本ate本, I設置pact.Da設置a成eSe正e本ity * 0.5f);
        b本eak;
    case E的at使本alDisaste本Type::基本ildfi本e:
        I設置pact.Reso使本ceDepletion.Add(EReso使本ceType::O本成anicMatte本, I設置pact.Da設置a成eSe正e本ity * 0.7f);
        b本eak;
    defa使lt:
        b本eak;
    }
    
    // Inf本ast本使ct使本e da設置a成e
    I設置pact.Inf本ast本使ct使本eDa設置a成e = I設置pact.Da設置a成eSe正e本ity * 0.3f;
    
    // Reco正e本y ti設置e
    I設置pact.Esti設置atedReco正e本yTi設置e = Disaste本.Pa本a設置ete本s.Reco正e本yTi設置e * (1.0f + I設置pact.Da設置a成eSe正e本ity);
    
    本et使本n I設置pact;
}

TA本本ay<軍Disaste本P本ediction> UMin成Ecosyste設置Disaste本Syste設置::Gene本ateP本edictions(const 軍St本in成& ZoneID)
{
    TA本本ay<軍Disaste本P本ediction> P本edictions;
    
    // Check fo本 en正i本on設置ental t本i成成e本s
    CheckEn正i本on設置entalT本i成成e本s(ZoneID);
    
    // Gene本ate p本edictions fo本 each disaste本 type
    fo本 (const a使to& Te設置plate : Disaste本Te設置plates)
    {
        float P本obability = Calc使lateDisaste本P本obability(Te設置plate.Key, ZoneID);
        
        if (P本obability > 0.1f) // Only incl使de 設置eanin成f使l p本edictions
        {
            軍Disaste本P本ediction P本ediction;
            P本ediction.P本edictedDisaste本 = Te設置plate.Key;
            P本ediction.P本obability = P本obability;
            P本ediction.ExpectedTi設置e = C使本本entGa設置eTi設置e + 軍Math::軍Rand() * 3600.0f; // 基本ithin next ho使本
            P本ediction.P本edictedSe正e本ity = EDisaste本Se正e本ity::Mode本ate;
            P本ediction.ConfidenceLe正el = 軍Math::Cla設置p(P本obability * 2.0f, 0.0f, 1.0f);
            
            // Gene本ate wa本nin成 設置essa成e
            switch (Te設置plate.Key)
            {
            case E的at使本alDisaste本Type::D本o使成ht:
                P本ediction.基本a本nin成Messa成e = TEXT("环境监测显示干旱风险增加");
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("降水不足"));
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("高温持续"));
                b本eak;
            case E的at使本alDisaste本Type::軍lood:
                P本ediction.基本a本nin成Messa成e = TEXT("洪水预警：水位上升");
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("持续降雨"));
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("河流水位上涨"));
                b本eak;
            case E的at使本alDisaste本Type::基本ildfi本e:
                P本ediction.基本a本nin成Messa成e = TEXT("火灾风险升高");
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("干旱条件"));
                P本ediction.Cont本ib使tin成軍acto本s.Add(TEXT("风速较高"));
                b本eak;
            defa使lt:
                P本ediction.基本a本nin成Messa成e = TEXT("自然灾害风险增加");
                b本eak;
            }
            
            P本edictions.Add(P本ediction);
        }
    }
    
    本et使本n P本edictions;
}

float UMin成Ecosyste設置Disaste本Syste設置::Calc使lateDisaste本P本obability(E的at使本alDisaste本Type Type, const 軍St本in成& ZoneID)
{
    float BaseP本obability = 0.0f;
    
    if (Disaste本Te設置plates.Contains(Type))
    {
        BaseP本obability = Disaste本Te設置plates[Type].BaseP本obability;
    }
    
    // Modify based on en正i本on設置ental conditions
    if (ZoneEn正i本on設置entalConditions.Contains(ZoneID))
    {
        const a使to& Conditions = ZoneEn正i本on設置entalConditions[ZoneID];
        
        switch (Type)
        {
        case E的at使本alDisaste本Type::D本o使成ht:
            if (Conditions.Contains(TEXT("輸入使設置idity")))
            {
                float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
                if (輸入使設置idity < 30.0f)
                {
                    BaseP本obability *= 3.0f;
                }
            }
            if (Conditions.Contains(TEXT("Te設置pe本at使本e")))
            {
                float Te設置p = Conditions[TEXT("Te設置pe本at使本e")];
                if (Te設置p > 35.0f)
                {
                    BaseP本obability *= 2.0f;
                }
            }
            b本eak;
            
        case E的at使本alDisaste本Type::軍lood:
            if (Conditions.Contains(TEXT("輸入使設置idity")))
            {
                float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
                if (輸入使設置idity > 80.0f)
                {
                    BaseP本obability *= 2.5f;
                }
            }
            b本eak;
            
        case E的at使本alDisaste本Type::基本ildfi本e:
            if (Conditions.Contains(TEXT("Te設置pe本at使本e")))
            {
                float Te設置p = Conditions[TEXT("Te設置pe本at使本e")];
                if (Te設置p > 30.0f)
                {
                    BaseP本obability *= 2.0f;
                }
            }
            if (Conditions.Contains(TEXT("輸入使設置idity")))
            {
                float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
                if (輸入使設置idity < 40.0f)
                {
                    BaseP本obability *= 2.0f;
                }
            }
            b本eak;
            
        defa使lt:
            b本eak;
        }
    }
    
    本et使本n 軍Math::Cla設置p(BaseP本obability, 0.0f, 1.0f);
}

軍Cli設置ateRiskAssess設置ent UMin成Ecosyste設置Disaste本Syste設置::AssessCli設置ateRisk(const 軍St本in成& ZoneID)
{
    軍Cli設置ateRiskAssess設置ent Assess設置ent;
    Assess設置ent.ZoneID = ZoneID;
    
    // Calc使late indi正id使al 本isks
    Assess設置ent.D本o使成htRisk = Calc使lateDisaste本P本obability(E的at使本alDisaste本Type::D本o使成ht, ZoneID);
    Assess設置ent.軍loodRisk = Calc使lateDisaste本P本obability(E的at使本alDisaste本Type::軍lood, ZoneID);
    Assess設置ent.Ea本thq使akeRisk = Calc使lateDisaste本P本obability(E的at使本alDisaste本Type::Ea本thq使ake, ZoneID);
    Assess設置ent.軍i本eRisk = Calc使lateDisaste本P本obability(E的at使本alDisaste本Type::基本ildfi本e, ZoneID);
    Assess設置ent.Sto本設置Risk = Calc使lateDisaste本P本obability(E的at使本alDisaste本Type::輸入使本本icane, ZoneID);
    
    // Calc使late o正e本all 本isk
    Assess設置ent.O正e本allRiskLe正el = (Assess設置ent.D本o使成htRisk + Assess設置ent.軍loodRisk + 
                                  Assess設置ent.Ea本thq使akeRisk + Assess設置ent.軍i本eRisk + 
                                  Assess設置ent.Sto本設置Risk) / 5.0f;
    
    // Identify 本isk facto本s
    if (Assess設置ent.D本o使成htRisk > 0.3f)
    {
        Assess設置ent.Risk軍acto本s.Add(TEXT("干旱风险"));
    }
    if (Assess設置ent.軍loodRisk > 0.3f)
    {
        Assess設置ent.Risk軍acto本s.Add(TEXT("洪水风险"));
    }
    if (Assess設置ent.Ea本thq使akeRisk > 0.2f)
    {
        Assess設置ent.Risk軍acto本s.Add(TEXT("地震风险"));
    }
    if (Assess設置ent.軍i本eRisk > 0.3f)
    {
        Assess設置ent.Risk軍acto本s.Add(TEXT("火灾风险"));
    }
    
    Assess設置ent.Assess設置entDate = 軍DateTi設置e::的ow().ToSt本in成();
    
    本et使本n Assess設置ent;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Iss使eEa本ly基本a本nin成(const 軍Disaste本P本ediction& P本ediction)
{
    float Ti設置eUntil = P本ediction.ExpectedTi設置e - C使本本entGa設置eTi設置e;
    
    OnDisaste本基本a本nin成.B本oadcast(P本ediction, Ti設置eUntil);
    
    UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Ea本ly wa本nin成 iss使ed - %s in %.0f seconds"),
        *P本ediction.基本a本nin成Messa成e, Ti設置eUntil);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::CheckEn正i本on設置entalT本i成成e本s(const 軍St本in成& ZoneID)
{
    UpdateEn正i本on設置entalConditions(ZoneID, 0.0f);
    E正al使ateRisk軍acto本s(ZoneID);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Re成iste本En正i本on設置entalCondition(const 軍St本in成& ZoneID, const 軍St本in成& Condition, float Val使e)
{
    if (!ZoneEn正i本on設置entalConditions.Contains(ZoneID))
    {
        ZoneEn正i本on設置entalConditions.Add(ZoneID, TMap<軍St本in成, float>());
    }
    
    ZoneEn正i本on設置entalConditions[ZoneID].Add(Condition, Val使e);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::E正al使ateDisaste本T本i成成e本s(const 軍St本in成& ZoneID)
{
    // Check fo本 a使to設置atic disaste本 t本i成成e本s based on conditions
    TA本本ay<軍Disaste本P本ediction> P本edictions = Gene本ateP本edictions(ZoneID);
    
    fo本 (const 軍Disaste本P本ediction& P本ediction : P本edictions)
    {
        if (P本ediction.P本obability > 0.7f && P本ediction.ExpectedTi設置e - C使本本entGa設置eTi設置e < 60.0f)
        {
            // 輸入i成h p本obability and i設置設置inent - a使to-t本i成成e本
            T本i成成e本的at使本alDisaste本(P本ediction.P本edictedDisaste本, P本ediction.P本edictedSe正e本ity, 軍Vecto本::Ze本oVecto本);
        }
    }
}

軍St本in成 UMin成Ecosyste設置Disaste本Syste設置::AddMiti成ationMeas使本e(E的at使本alDisaste本Type Ta本成etDisaste本, const 軍St本in成& Type, const 軍Vecto本& Location)
{
    軍St本in成 Miti成ationID = 軍G使id::的ewG使id().ToSt本in成();
    
    軍Disaste本Miti成ation Miti成ation;
    Miti成ation.Miti成ationID = Miti成ationID;
    Miti成ation.Ta本成etDisaste本 = Ta本成etDisaste本;
    Miti成ation.Miti成ationType = Type;
    Miti成ation.Location = Location;
    
    // Set effecti正eness based on type
    if (Type == TEXT("Ea本ly 基本a本nin成 Syste設置"))
    {
        Miti成ation.Effecti正eness = 0.3f;
        Miti成ation.Cost = 5000.0f;
    }
    else if (Type == TEXT("軍lood Ba本本ie本"))
    {
        Miti成ation.Effecti正eness = 0.6f;
        Miti成ation.Cost = 10000.0f;
    }
    else if (Type == TEXT("軍i本eb本eak"))
    {
        Miti成ation.Effecti正eness = 0.5f;
        Miti成ation.Cost = 3000.0f;
    }
    else if (Type == TEXT("Ea本thq使ake Resistant B使ildin成s"))
    {
        Miti成ation.Effecti正eness = 0.7f;
        Miti成ation.Cost = 15000.0f;
    }
    else
    {
        Miti成ation.Effecti正eness = 0.4f;
        Miti成ation.Cost = 5000.0f;
    }
    
    Miti成ationMeas使本es.Add(Miti成ationID, Miti成ation);
    
    本et使本n Miti成ationID;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Acti正ateMiti成ation(const 軍St本in成& Miti成ationID)
{
    if (Miti成ationMeas使本es.Contains(Miti成ationID))
    {
        Miti成ationMeas使本es[Miti成ationID].bIsActi正e = t本使e;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Deacti正ateMiti成ation(const 軍St本in成& Miti成ationID)
{
    if (Miti成ationMeas使本es.Contains(Miti成ationID))
    {
        Miti成ationMeas使本es[Miti成ationID].bIsActi正e = false;
    }
}

float UMin成Ecosyste設置Disaste本Syste設置::Calc使lateMiti成ationEffecti正eness(const 軍St本in成& Miti成ationID, const 軍St本in成& Disaste本ID)
{
    if (!Miti成ationMeas使本es.Contains(Miti成ationID)  !Acti正eDisaste本s.Contains(Disaste本ID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Disaste本Miti成ation& Miti成ation = Miti成ationMeas使本es[Miti成ationID];
    
    if (!Miti成ation.bIsActi正e)
    {
        本et使本n 0.0f;
    }
    
    // Check if disaste本 type 設置atches
    const 軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
    if (Miti成ation.Ta本成etDisaste本 != Disaste本.Pa本a設置ete本s.Disaste本Type)
    {
        本et使本n 0.0f;
    }
    
    // Calc使late distance-based effecti正eness
    float Distance = Calc使lateDistanceToDisaste本(Miti成ation.Location, Disaste本);
    float Distance軍acto本 = 軍Math::Max(0.0f, 1.0f - (Distance / Miti成ation.Co正e本a成eRadi使s));
    
    本et使本n Miti成ation.Effecti正eness * Distance軍acto本;
}

TA本本ay<軍Disaste本Miti成ation> UMin成Ecosyste設置Disaste本Syste設置::GetActi正eMiti成ations(const 軍St本in成& ZoneID) const
{
    TA本本ay<軍Disaste本Miti成ation> Acti正eMiti成ations;
    
    fo本 (const a使to& Miti成ationPai本 : Miti成ationMeas使本es)
    {
        if (Miti成ationPai本.Val使e.bIsActi正e)
        {
            Acti正eMiti成ations.Add(Miti成ationPai本.Val使e);
        }
    }
    
    本et使本n Acti正eMiti成ations;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Sta本tReco正e本y(const 軍St本in成& ZoneID, const 軍St本in成& Disaste本ID)
{
    軍St本in成 Key = ZoneID + TEXT("下") + Disaste本ID;
    Reco正e本yP本o成本ess.Add(Key, 0.0f);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置Disaste本Syste設置: Sta本ted 本eco正e本y fo本 zone %s f本o設置 disaste本 %s"),
        *ZoneID, *Disaste本ID);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::P本ocessReco正e本y(const 軍St本in成& ZoneID, float DeltaTi設置e)
{
    // P本ocess 本eco正e本y fo本 all acti正e 本eco正e本ies in this zone
    TA本本ay<軍St本in成> KeysToRe設置o正e;
    
    fo本 (a使to& P本o成本essPai本 : Reco正e本yP本o成本ess)
    {
        if (P本o成本essPai本.Key.Sta本ts基本ith(ZoneID))
        {
            P本o成本essPai本.Val使e += DeltaTi設置e / 3600.0f; // Ass使設置e 1 ho使本 本eco正e本y ti設置e
            
            float P本o成本ess = 軍Math::Cla設置p(P本o成本essPai本.Val使e, 0.0f, 1.0f);
            OnReco正e本yP本o成本ess.B本oadcast(ZoneID, P本o成本ess * 100.0f);
            
            if (P本o成本ess >= 1.0f)
            {
                KeysToRe設置o正e.Add(P本o成本essPai本.Key);
            }
        }
    }
    
    fo本 (const 軍St本in成& Key : KeysToRe設置o正e)
    {
        Reco正e本yP本o成本ess.Re設置o正e(Key);
    }
}

float UMin成Ecosyste設置Disaste本Syste設置::GetReco正e本yP本o成本ess(const 軍St本in成& ZoneID) const
{
    float TotalP本o成本ess = 0.0f;
    int32 Co使nt = 0;
    
    fo本 (const a使to& P本o成本essPai本 : Reco正e本yP本o成本ess)
    {
        if (P本o成本essPai本.Key.Sta本ts基本ith(ZoneID))
        {
            TotalP本o成本ess += P本o成本essPai本.Val使e;
            Co使nt++;
        }
    }
    
    本et使本n Co使nt > 0 基本 TotalP本o成本ess / Co使nt : 0.0f;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Accele本ateReco正e本y(const 軍St本in成& ZoneID, float Accele本ation軍acto本)
{
    fo本 (a使to& P本o成本essPai本 : Reco正e本yP本o成本ess)
    {
        if (P本o成本essPai本.Key.Sta本ts基本ith(ZoneID))
        {
            P本o成本essPai本.Val使e *= Accele本ation軍acto本;
        }
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ApplyDa設置a成eToZone(const 軍St本in成& ZoneID, const 軍Disaste本I設置pact& I設置pact)
{
    ZoneI設置pacts.Add(ZoneID, I設置pact);
    OnDisaste本I設置pact.B本oadcast(I設置pact);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ApplySpeciesDa設置a成e(const 軍St本in成& ZoneID, ESpeciesType Species, int32 Cas使alties)
{
    if (ZoneI設置pacts.Contains(ZoneID))
    {
        軍Disaste本I設置pact& I設置pact = ZoneI設置pacts[ZoneID];
        I設置pact.SpeciesCas使alties.Add(Species, Cas使alties);
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ApplyReso使本ceDa設置a成e(const 軍St本in成& ZoneID, EReso使本ceType Reso使本ce, float Depletion)
{
    if (ZoneI設置pacts.Contains(ZoneID))
    {
        軍Disaste本I設置pact& I設置pact = ZoneI設置pacts[ZoneID];
        I設置pact.Reso使本ceDepletion.Add(Reso使本ce, Depletion);
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ApplyInf本ast本使ct使本eDa設置a成e(const 軍St本in成& ZoneID, float Da設置a成eA設置o使nt)
{
    if (ZoneI設置pacts.Contains(ZoneID))
    {
        軍Disaste本I設置pact& I設置pact = ZoneI設置pacts[ZoneID];
        I設置pact.Inf本ast本使ct使本eDa設置a成e += Da設置a成eA設置o使nt;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::C本eateEa本thq使ake(const 軍Vecto本& Epicente本, float Ma成nit使de, float Depth)
{
    EDisaste本Se正e本ity Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    if (Ma成nit使de < 4.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mino本;
    }
    else if (Ma成nit使de < 6.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    }
    else if (Ma成nit使de < 7.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    }
    else if (Ma成nit使de < 8.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    }
    else
    {
        Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    }
    
    軍St本in成 Disaste本ID = T本i成成e本的at使本alDisaste本(E的at使本alDisaste本Type::Ea本thq使ake, Se正e本ity, Epicente本);
    
    // Modify ea本thq使ake-specific pa本a設置ete本s
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.AffectedRadi使s = 軍Math::Min(50000.0f, 10000.0f * (Ma成nit使de / 5.0f));
        Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 = Ma成nit使de / 5.0f;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::C本eate軍lood(const 軍Vecto本& Epicente本, float 基本ate本Le正el, float D使本ation)
{
    EDisaste本Se正e本ity Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    if (基本ate本Le正el < 1.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mino本;
    }
    else if (基本ate本Le正el < 3.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    }
    else if (基本ate本Le正el < 5.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    }
    else if (基本ate本Le正el < 8.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    }
    else
    {
        Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    }
    
    軍St本in成 Disaste本ID = T本i成成e本的at使本alDisaste本(E的at使本alDisaste本Type::軍lood, Se正e本ity, Epicente本);
    
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.D使本ation = D使本ation;
        Disaste本.Pa本a設置ete本s.AffectedRadi使s = 基本ate本Le正el * 1000.0f;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::C本eateD本o使成ht(const 軍St本in成& ZoneID, float Se正e本ity, float D使本ation)
{
    EDisaste本Se正e本ity Disaste本Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    if (Se正e本ity < 0.3f)
    {
        Disaste本Se正e本ity = EDisaste本Se正e本ity::Mino本;
    }
    else if (Se正e本ity < 0.5f)
    {
        Disaste本Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    }
    else if (Se正e本ity < 0.7f)
    {
        Disaste本Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    }
    else if (Se正e本ity < 0.9f)
    {
        Disaste本Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    }
    else
    {
        Disaste本Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    }
    
    軍St本in成 Disaste本ID = T本i成成e本的at使本alDisaste本(E的at使本alDisaste本Type::D本o使成ht, Disaste本Se正e本ity, 軍Vecto本::Ze本oVecto本);
    
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.D使本ation = D使本ation;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::C本eate基本ildfi本e(const 軍Vecto本& Epicente本, float Sp本eadRate, float Intensity)
{
    EDisaste本Se正e本ity Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    if (Intensity < 0.3f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mino本;
    }
    else if (Intensity < 0.5f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    }
    else if (Intensity < 0.7f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    }
    else if (Intensity < 0.9f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    }
    else
    {
        Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    }
    
    軍St本in成 Disaste本ID = T本i成成e本的at使本alDisaste本(E的at使本alDisaste本Type::基本ildfi本e, Se正e本ity, Epicente本);
    
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.AffectedRadi使s = Sp本eadRate * 5000.0f;
        Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 = Intensity;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::C本eateSto本設置(const 軍Vecto本& Epicente本, float 基本indSpeed, float P本ecipitation)
{
    EDisaste本Se正e本ity Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    if (基本indSpeed < 50.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mino本;
    }
    else if (基本indSpeed < 100.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Mode本ate;
    }
    else if (基本indSpeed < 150.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Se正e本e;
    }
    else if (基本indSpeed < 200.0f)
    {
        Se正e本ity = EDisaste本Se正e本ity::Catast本ophic;
    }
    else
    {
        Se正e本ity = EDisaste本Se正e本ity::Apocalyptic;
    }
    
    // Dete本設置ine disaste本 type based on conditions
    E的at使本alDisaste本Type Sto本設置Type = E的at使本alDisaste本Type::輸入使本本icane;
    if (基本indSpeed > 200.0f && 軍Math::軍Rand() < 0.3f)
    {
        Sto本設置Type = E的at使本alDisaste本Type::To本nado;
    }
    
    軍St本in成 Disaste本ID = T本i成成e本的at使本alDisaste本(Sto本設置Type, Se正e本ity, Epicente本);
    
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.AffectedRadi使s = 基本indSpeed * 50.0f;
    }
}

float UMin成Ecosyste設置Disaste本Syste設置::GetTotalDa設置a成eCost() const
{
    float TotalCost = 0.0f;
    
    fo本 (const a使to& Disaste本 : Disaste本輸入isto本y)
    {
        TotalCost += Disaste本.Econo設置icLoss;
    }
    
    fo本 (const a使to& Disaste本 : Acti正eDisaste本s)
    {
        TotalCost += Disaste本.Val使e.Econo設置icLoss;
    }
    
    本et使本n TotalCost;
}

int32 UMin成Ecosyste設置Disaste本Syste設置::GetTotalCas使alties() const
{
    int32 TotalCas使alties = 0;
    
    fo本 (const a使to& Disaste本 : Disaste本輸入isto本y)
    {
        TotalCas使alties += Disaste本.AffectedPop使lation;
    }
    
    fo本 (const a使to& Disaste本 : Acti正eDisaste本s)
    {
        TotalCas使alties += Disaste本.Val使e.AffectedPop使lation;
    }
    
    本et使本n TotalCas使alties;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    C使本本entGa設置eTi設置e += DeltaTi設置e;
    
    // P本ocess acti正e disaste本s
    TA本本ay<軍St本in成> Disaste本sToRe設置o正e;
    
    fo本 (a使to& Disaste本Pai本 : Acti正eDisaste本s)
    {
        軍St本in成 Disaste本ID = Disaste本Pai本.Key;
        Si設置使lateDisaste本P本o成本ession(Disaste本ID, DeltaTi設置e);
        
        // Check if disaste本 sho使ld end
        if (IsDisaste本Expi本ed(Disaste本Pai本.Val使e))
        {
            Disaste本sToRe設置o正e.Add(Disaste本ID);
        }
    }
    
    // End expi本ed disaste本s
    fo本 (const 軍St本in成& Disaste本ID : Disaste本sToRe設置o正e)
    {
        EndDisaste本(Disaste本ID);
    }
    
    // P本ocess 本eco正e本y fo本 all zones with i設置pacts
    TA本本ay<軍St本in成> ZoneIDs;
    ZoneI設置pacts.GetKeys(ZoneIDs);
    fo本 (const 軍St本in成& ZoneID : ZoneIDs)
    {
        P本ocessReco正e本y(ZoneID, DeltaTi設置e);
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::UpdateDisaste本Phase(軍Acti正eDisaste本& Disaste本)
{
    float PhaseP本o成本ess = Disaste本.P本o成本ess;
    
    if (PhaseP本o成本ess < 0.1f)
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::B使ildin成;
    }
    else if (PhaseP本o成本ess < 0.3f)
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Acti正e;
    }
    else if (PhaseP本o成本ess < 0.6f)
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Peak;
    }
    else if (PhaseP本o成本ess < 0.9f)
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Declinin成;
    }
    else if (PhaseP本o成本ess < 1.0f)
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Afte本設置ath;
    }
    else
    {
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Co設置plete;
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Calc使lateDisaste本Da設置a成e(軍Acti正eDisaste本& Disaste本)
{
    // Calc使late econo設置ic loss based on intensity, d使本ation, and affected a本ea
    float A本ea軍acto本 = 軍Math::PI * 軍Math::Sq使a本e(Disaste本.Pa本a設置ete本s.AffectedRadi使s / 1000.0f);
    float Intensity軍acto本 = Disaste本.Pa本a設置ete本s.C使本本entIntensity * Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本;
    float D使本ation軍acto本 = 軍Math::Min(Disaste本.C使本本entTi設置e / 3600.0f, 10.0f); // Cap at 10 ho使本s
    
    Disaste本.Econo設置icLoss = A本ea軍acto本 * Intensity軍acto本 * D使本ation軍acto本 * 1000.0f;
    Disaste本.TotalDa設置a成e = Intensity軍acto本 * 100.0f;
    
    // Affected pop使lation esti設置ate
    Disaste本.AffectedPop使lation = 軍Math::Ro使ndToInt(A本ea軍acto本 * Intensity軍acto本 * 100.0f);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Affect的ea本byZones(軍Acti正eDisaste本& Disaste本)
{
    // TODO: Get zones f本o設置 ecosyste設置 syste設置 and check distance
    // 軍o本 now, this is a placeholde本 fo本 zone inte本action lo成ic
}

float UMin成Ecosyste設置Disaste本Syste設置::Calc使lateDistanceToDisaste本(const 軍Vecto本& Location, const 軍Acti正eDisaste本& Disaste本) const
{
    本et使本n 軍Vecto本::Distance(Location, Disaste本.Epicente本);
}

float UMin成Ecosyste設置Disaste本Syste設置::Calc使lateDa設置a成eAtDistance(float Distance, const 軍Acti正eDisaste本& Disaste本) const
{
    float 的o本設置alizedDistance = Distance / Disaste本.Pa本a設置ete本s.AffectedRadi使s;
    
    // Da設置a成e dec本eases with distance 使sin成 in正e本se sq使a本e law
    float Da設置a成e軍acto本 = 軍Math::Max(0.0f, 1.0f - 軍Math::Sq使a本e(的o本設置alizedDistance));
    
    本et使本n Da設置a成e軍acto本 * Disaste本.Pa本a設置ete本s.C使本本entIntensity * Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::InitializeDisaste本Pa本a設置ete本s(軍Disaste本Pa本a設置ete本s& Pa本a設置s)
{
    // Set defa使lt 正al使es
    Pa本a設置s.C使本本entPhase = EDisaste本Phase::Do本設置ant;
    Pa本a設置s.C使本本entIntensity = 0.0f;
    Pa本a設置s.En正i本on設置entalConditions.E設置pty();
}

正oid UMin成Ecosyste設置Disaste本Syste設置::ApplyEn正i本on設置entalModifie本s(軍Disaste本Pa本a設置ete本s& Pa本a設置s, const 軍St本in成& ZoneID)
{
    if (!ZoneEn正i本on設置entalConditions.Contains(ZoneID))
    {
        本et使本n;
    }
    
    const a使to& Conditions = ZoneEn正i本on設置entalConditions[ZoneID];
    
    // Apply 設置odifie本s based on disaste本 type
    switch (Pa本a設置s.Disaste本Type)
    {
    case E的at使本alDisaste本Type::基本ildfi本e:
        if (Conditions.Contains(TEXT("Te設置pe本at使本e")))
        {
            float Te設置p = Conditions[TEXT("Te設置pe本at使本e")];
            if (Te設置p > 35.0f)
            {
                Pa本a設置s.MaxIntensity *= 1.2f;
            }
        }
        b本eak;
        
    case E的at使本alDisaste本Type::軍lood:
        if (Conditions.Contains(TEXT("輸入使設置idity")))
        {
            float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
            if (輸入使設置idity > 90.0f)
            {
                Pa本a設置s.D使本ation *= 1.5f;
            }
        }
        b本eak;
        
    defa使lt:
        b本eak;
    }
}

bool UMin成Ecosyste設置Disaste本Syste設置::Sho使ldT本i成成e本基本a本nin成(const 軍Acti正eDisaste本& Disaste本) const
{
    本et使本n Disaste本.Pa本a設置ete本s.b輸入as基本a本nin成 && 
           Disaste本.Pa本a設置ete本s.基本a本nin成Ti設置e > 0.0f &&
           Disaste本.Pa本a設置ete本s.C使本本entPhase == EDisaste本Phase::B使ildin成;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Gene本ateDisaste本基本a本nin成(const 軍Acti正eDisaste本& Disaste本)
{
    軍Disaste本P本ediction P本ediction;
    P本ediction.P本edictedDisaste本 = Disaste本.Pa本a設置ete本s.Disaste本Type;
    P本ediction.P本obability = 1.0f;
    P本ediction.ExpectedTi設置e = Disaste本.Sta本tTi設置e + Disaste本.Pa本a設置ete本s.基本a本nin成Ti設置e;
    P本ediction.P本edictedLocation = Disaste本.Epicente本;
    P本ediction.P本edictedSe正e本ity = Disaste本.Pa本a設置ete本s.Se正e本ity;
    P本ediction.ConfidenceLe正el = 0.9f;
    
    switch (Disaste本.Pa本a設置ete本s.Disaste本Type)
    {
    case E的at使本alDisaste本Type::Ea本thq使ake:
        P本ediction.基本a本nin成Messa成e = TEXT("地震预警：预计有感地震即将发生");
        b本eak;
    case E的at使本alDisaste本Type::軍lood:
        P本ediction.基本a本nin成Messa成e = TEXT("洪水预警：水位即将达到危险水平");
        b本eak;
    case E的at使本alDisaste本Type::基本ildfi本e:
        P本ediction.基本a本nin成Messa成e = TEXT("火灾预警：火险等级极高");
        b本eak;
    case E的at使本alDisaste本Type::輸入使本本icane:
        P本ediction.基本a本nin成Messa成e = TEXT("飓风预警：强风暴即将来袭");
        b本eak;
    defa使lt:
        P本ediction.基本a本nin成Messa成e = TEXT("自然灾害预警：请做好防护准备");
        b本eak;
    }
    
    Iss使eEa本ly基本a本nin成(P本ediction);
}

正oid UMin成Ecosyste設置Disaste本Syste設置::Co設置pleteDisaste本(const 軍St本in成& Disaste本ID)
{
    if (Acti正eDisaste本s.Contains(Disaste本ID))
    {
        軍Acti正eDisaste本& Disaste本 = Acti正eDisaste本s[Disaste本ID];
        Disaste本.Pa本a設置ete本s.C使本本entPhase = EDisaste本Phase::Co設置plete;
        EndDisaste本(Disaste本ID);
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::A本chi正eDisaste本(const 軍Acti正eDisaste本& Disaste本)
{
    Disaste本輸入isto本y.Add(Disaste本);
}

bool UMin成Ecosyste設置Disaste本Syste設置::IsDisaste本Expi本ed(const 軍Acti正eDisaste本& Disaste本) const
{
    本et使本n Disaste本.C使本本entTi設置e >= Disaste本.Pa本a設置ete本s.D使本ation 
           Disaste本.Pa本a設置ete本s.C使本本entPhase == EDisaste本Phase::Co設置plete;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::P本ocessMiti成ationEffects(軍Acti正eDisaste本& Disaste本)
{
    float TotalMiti成ation = 0.0f;
    
    fo本 (const a使to& Miti成ationPai本 : Miti成ationMeas使本es)
    {
        float Effecti正eness = Calc使lateMiti成ationEffecti正eness(Miti成ationPai本.Key, Disaste本.Disaste本ID);
        TotalMiti成ation += Effecti正eness;
    }
    
    // Cap total 設置iti成ation at 80%
    TotalMiti成ation = 軍Math::Min(TotalMiti成ation, 0.8f);
    
    // Apply 設置iti成ation to disaste本
    Disaste本.Pa本a設置ete本s.Da設置a成eM使ltiplie本 *= (1.0f - TotalMiti成ation);
}

float UMin成Ecosyste設置Disaste本Syste設置::GetDisaste本D使本ation(E的at使本alDisaste本Type Type) const
{
    if (Disaste本Te設置plates.Contains(Type))
    {
        本et使本n Disaste本Te設置plates[Type].D使本ation;
    }
    本et使本n 1800.0f;
}

float UMin成Ecosyste設置Disaste本Syste設置::GetDisaste本Radi使s(E的at使本alDisaste本Type Type, EDisaste本Se正e本ity Se正e本ity) const
{
    if (Disaste本Te設置plates.Contains(Type))
    {
        float BaseRadi使s = Disaste本Te設置plates[Type].AffectedRadi使s;
        
        switch (Se正e本ity)
        {
        case EDisaste本Se正e本ity::Mino本:
            本et使本n BaseRadi使s * 0.5f;
        case EDisaste本Se正e本ity::Mode本ate:
            本et使本n BaseRadi使s;
        case EDisaste本Se正e本ity::Se正e本e:
            本et使本n BaseRadi使s * 1.2f;
        case EDisaste本Se正e本ity::Catast本ophic:
            本et使本n BaseRadi使s * 1.5f;
        case EDisaste本Se正e本ity::Apocalyptic:
            本et使本n BaseRadi使s * 2.0f;
        defa使lt:
            本et使本n BaseRadi使s;
        }
    }
    本et使本n 1000.0f;
}

float UMin成Ecosyste設置Disaste本Syste設置::GetDisaste本P本obability(E的at使本alDisaste本Type Type, const 軍St本in成& ZoneID) const
{
    float BaseP本ob = 0.0f;
    
    if (Disaste本Te設置plates.Contains(Type))
    {
        BaseP本ob = Disaste本Te設置plates[Type].BaseP本obability;
    }
    
    // TODO: Modify based on zone cha本acte本istics
    
    本et使本n BaseP本ob;
}

正oid UMin成Ecosyste設置Disaste本Syste設置::UpdateEn正i本on設置entalConditions(const 軍St本in成& ZoneID, float DeltaTi設置e)
{
    // Update o本 initialize en正i本on設置ental conditions
    if (!ZoneEn正i本on設置entalConditions.Contains(ZoneID))
    {
        ZoneEn正i本on設置entalConditions.Add(ZoneID, TMap<軍St本in成, float>());
    }
    
    a使to& Conditions = ZoneEn正i本on設置entalConditions[ZoneID];
    
    // Set so設置e defa使lt conditions if not p本esent
    if (!Conditions.Contains(TEXT("Te設置pe本at使本e")))
    {
        Conditions.Add(TEXT("Te設置pe本at使本e"), 20.0f);
    }
    if (!Conditions.Contains(TEXT("輸入使設置idity")))
    {
        Conditions.Add(TEXT("輸入使設置idity"), 50.0f);
    }
    if (!Conditions.Contains(TEXT("基本indSpeed")))
    {
        Conditions.Add(TEXT("基本indSpeed"), 5.0f);
    }
    if (!Conditions.Contains(TEXT("P本ess使本e")))
    {
        Conditions.Add(TEXT("P本ess使本e"), 1013.0f);
    }
    if (!Conditions.Contains(TEXT("Rainfall")))
    {
        Conditions.Add(TEXT("Rainfall"), 0.0f);
    }
}

正oid UMin成Ecosyste設置Disaste本Syste設置::E正al使ateRisk軍acto本s(const 軍St本in成& ZoneID)
{
    if (!ZoneEn正i本on設置entalConditions.Contains(ZoneID))
    {
        本et使本n;
    }
    
    // E正al使ate 本isk based on c使本本ent conditions
    const a使to& Conditions = ZoneEn正i本on設置entalConditions[ZoneID];
    
    // Check fo本 d本o使成ht 本isk
    if (Conditions.Contains(TEXT("輸入使設置idity")) && Conditions.Contains(TEXT("Te設置pe本at使本e")))
    {
        float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
        float Te設置p = Conditions[TEXT("Te設置pe本at使本e")];
        
        if (輸入使設置idity < 30.0f && Te設置p > 30.0f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Ecosyste設置Disaste本Syste設置: D本o使成ht 本isk detected in zone %s"), *ZoneID);
        }
    }
    
    // Check fo本 flood 本isk
    if (Conditions.Contains(TEXT("輸入使設置idity")) && Conditions.Contains(TEXT("Rainfall")))
    {
        float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
        float Rainfall = Conditions[TEXT("Rainfall")];
        
        if (輸入使設置idity > 85.0f && Rainfall > 50.0f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Ecosyste設置Disaste本Syste設置: 軍lood 本isk detected in zone %s"), *ZoneID);
        }
    }
    
    // Check fo本 fi本e 本isk
    if (Conditions.Contains(TEXT("Te設置pe本at使本e")) && Conditions.Contains(TEXT("輸入使設置idity")))
    {
        float Te設置p = Conditions[TEXT("Te設置pe本at使本e")];
        float 輸入使設置idity = Conditions[TEXT("輸入使設置idity")];
        
        if (Te設置p > 35.0f && 輸入使設置idity < 40.0f)
        {
            UE下LOG(Lo成Te設置p, 基本a本nin成, TEXT("Min成Ecosyste設置Disaste本Syste設置: 軍i本e 本isk detected in zone %s"), *ZoneID);
        }
    }
}
