#incl使de "Ecosyste設置/Min成Ecosyste設置En正i本on設置entSyste設置.h"
#incl使de "Math/Un本ealMathUtility.h"
#incl使de "En成ine/En成ine.h"

UMin成Ecosyste設置En正i本on設置entSyste設置::UMin成Ecosyste設置En正i本on設置entSyste設置()
    : bIsInitialized(false)
    , C使本本entGa設置eTi設置e(0.0f)
    , C使本本entSeason(ESeasonType::Sp本in成)
    , SeasonP本o成本ess(0.0f)
    , DaysPe本Season(30.0f)
    , C使本本ent基本eathe本(E基本eathe本Type::Clea本)
    , 基本eathe本Chan成eTi設置e本(0.0f)
    , 基本eathe本Chan成eInte本正al(300.0f)
    , C使本本ent輸入o使本(12.0f)
    , 輸入o使本sPe本Day(24.0f)
    , C使本本entDayPhase(EDay的i成htCycle::的oon)
    , C使本本entCli設置ateZone(ECli設置ateZone::Te設置pe本ate)
    , Ta本成et基本eathe本(E基本eathe本Type::Clea本)
    , 基本eathe本T本ansitionP本o成本ess(1.0f)
    , 基本eathe本T本ansitionD使本ation(60.0f)
{
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::InitializeEn正i本on設置entSyste設置()
{
    if (bIsInitialized)
    {
        本et使本n;
    }
    
    InitializeSeasonDatabase();
    InitializeCli設置ateDatabase();
    
    C使本本entGa設置eTi設置e = 0.0f;
    C使本本entSeason = ESeasonType::Sp本in成;
    SeasonP本o成本ess = 0.0f;
    C使本本ent輸入o使本 = 6.0f;
    C使本本entDayPhase = EDay的i成htCycle::Dawn;
    C使本本ent基本eathe本 = E基本eathe本Type::Clea本;
    
    Calc使late基本eathe本Pa本a設置ete本s();
    UpdateDay的i成htCycle();
    
    bIsInitialized = t本使e;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: Initialized"));
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Sh使tdownEn正i本on設置entSyste設置()
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    Acti正eE正ents.E設置pty();
    SeasonDatabase.E設置pty();
    Cli設置ateDatabase.E設置pty();
    
    bIsInitialized = false;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: Sh使tdown"));
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::InitializeSeasonDatabase()
{
    // Sp本in成
    軍SeasonData Sp本in成Data;
    Sp本in成Data.Season = ESeasonType::Sp本in成;
    Sp本in成Data.BaseTe設置pe本at使本e = 15.0f;
    Sp本in成Data.Te設置pe本at使本eVa本iation = 8.0f;
    Sp本in成Data.A正e本a成e輸入使設置idity = 65.0f;
    Sp本in成Data.P本ecipitationP本obability = 0.4f;
    Sp本in成Data.DayLen成th = 13.0f;
    Sp本in成Data.G本owthRateM使ltiplie本 = 1.3f;
    Sp本in成Data.Reso使本ceRe成ene本ationRate = 1.2f;
    Sp本in成Data.A設置bientLi成htColo本 = 軍Linea本Colo本(0.9f, 1.0f, 0.8f, 1.0f);
    Sp本in成Data.S使nIntensity = 0.9f;
    SeasonDatabase.Add(ESeasonType::Sp本in成, Sp本in成Data);
    
    // S使設置設置e本
    軍SeasonData S使設置設置e本Data;
    S使設置設置e本Data.Season = ESeasonType::S使設置設置e本;
    S使設置設置e本Data.BaseTe設置pe本at使本e = 28.0f;
    S使設置設置e本Data.Te設置pe本at使本eVa本iation = 6.0f;
    S使設置設置e本Data.A正e本a成e輸入使設置idity = 70.0f;
    S使設置設置e本Data.P本ecipitationP本obability = 0.5f;
    S使設置設置e本Data.DayLen成th = 15.0f;
    S使設置設置e本Data.G本owthRateM使ltiplie本 = 1.5f;
    S使設置設置e本Data.Reso使本ceRe成ene本ationRate = 1.4f;
    S使設置設置e本Data.A設置bientLi成htColo本 = 軍Linea本Colo本(1.0f, 1.0f, 0.9f, 1.0f);
    S使設置設置e本Data.S使nIntensity = 1.2f;
    SeasonDatabase.Add(ESeasonType::S使設置設置e本, S使設置設置e本Data);
    
    // A使t使設置n
    軍SeasonData A使t使設置nData;
    A使t使設置nData.Season = ESeasonType::A使t使設置n;
    A使t使設置nData.BaseTe設置pe本at使本e = 18.0f;
    A使t使設置nData.Te設置pe本at使本eVa本iation = 7.0f;
    A使t使設置nData.A正e本a成e輸入使設置idity = 55.0f;
    A使t使設置nData.P本ecipitationP本obability = 0.3f;
    A使t使設置nData.DayLen成th = 11.0f;
    A使t使設置nData.G本owthRateM使ltiplie本 = 0.8f;
    A使t使設置nData.Reso使本ceRe成ene本ationRate = 0.9f;
    A使t使設置nData.A設置bientLi成htColo本 = 軍Linea本Colo本(1.0f, 0.9f, 0.7f, 1.0f);
    A使t使設置nData.S使nIntensity = 0.8f;
    SeasonDatabase.Add(ESeasonType::A使t使設置n, A使t使設置nData);
    
    // 基本inte本
    軍SeasonData 基本inte本Data;
    基本inte本Data.Season = ESeasonType::基本inte本;
    基本inte本Data.BaseTe設置pe本at使本e = 2.0f;
    基本inte本Data.Te設置pe本at使本eVa本iation = 10.0f;
    基本inte本Data.A正e本a成e輸入使設置idity = 45.0f;
    基本inte本Data.P本ecipitationP本obability = 0.2f;
    基本inte本Data.DayLen成th = 9.0f;
    基本inte本Data.G本owthRateM使ltiplie本 = 0.3f;
    基本inte本Data.Reso使本ceRe成ene本ationRate = 0.5f;
    基本inte本Data.A設置bientLi成htColo本 = 軍Linea本Colo本(0.8f, 0.85f, 0.95f, 1.0f);
    基本inte本Data.S使nIntensity = 0.6f;
    SeasonDatabase.Add(ESeasonType::基本inte本, 基本inte本Data);
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::InitializeCli設置ateDatabase()
{
    // T本opical
    軍Cli設置ateZoneData T本opicalData;
    T本opicalData.ZoneType = ECli設置ateZone::T本opical;
    T本opicalData.MinTe設置pe本at使本e = 20.0f;
    T本opicalData.MaxTe設置pe本at使本e = 35.0f;
    T本opicalData.A正e本a成e輸入使設置idity = 80.0f;
    T本opicalData.A正ailableSeasons = {ESeasonType::Sp本in成, ESeasonType::S使設置設置e本};
    T本opicalData.Co設置設置on基本eathe本Types = {E基本eathe本Type::Clea本, E基本eathe本Type::Clo使dy, E基本eathe本Type::輸入ea正yRain, E基本eathe本Type::Sto本設置};
    T本opicalData.Rainfall = 2000.0f;
    T本opicalData.的at使本alDisaste本P本obability = 0.15f;
    Cli設置ateDatabase.Add(ECli設置ateZone::T本opical, T本opicalData);
    
    // Te設置pe本ate
    軍Cli設置ateZoneData Te設置pe本ateData;
    Te設置pe本ateData.ZoneType = ECli設置ateZone::Te設置pe本ate;
    Te設置pe本ateData.MinTe設置pe本at使本e = -10.0f;
    Te設置pe本ateData.MaxTe設置pe本at使本e = 35.0f;
    Te設置pe本ateData.A正e本a成e輸入使設置idity = 60.0f;
    Te設置pe本ateData.A正ailableSeasons = {ESeasonType::Sp本in成, ESeasonType::S使設置設置e本, ESeasonType::A使t使設置n, ESeasonType::基本inte本};
    Te設置pe本ateData.Co設置設置on基本eathe本Types = {E基本eathe本Type::Clea本, E基本eathe本Type::Clo使dy, E基本eathe本Type::Li成htRain, E基本eathe本Type::輸入ea正yRain, E基本eathe本Type::Snow};
    Te設置pe本ateData.Rainfall = 800.0f;
    Te設置pe本ateData.的at使本alDisaste本P本obability = 0.1f;
    Cli設置ateDatabase.Add(ECli設置ateZone::Te設置pe本ate, Te設置pe本ateData);
    
    // A本id
    軍Cli設置ateZoneData A本idData;
    A本idData.ZoneType = ECli設置ateZone::A本id;
    A本idData.MinTe設置pe本at使本e = 5.0f;
    A本idData.MaxTe設置pe本at使本e = 45.0f;
    A本idData.A正e本a成e輸入使設置idity = 20.0f;
    A本idData.A正ailableSeasons = {ESeasonType::S使設置設置e本, ESeasonType::A使t使設置n};
    A本idData.Co設置設置on基本eathe本Types = {E基本eathe本Type::Clea本, E基本eathe本Type::Clo使dy, E基本eathe本Type::Sandsto本設置};
    A本idData.Rainfall = 100.0f;
    A本idData.的at使本alDisaste本P本obability = 0.2f;
    Cli設置ateDatabase.Add(ECli設置ateZone::A本id, A本idData);
    
    // 軍本i成id
    軍Cli設置ateZoneData 軍本i成idData;
    軍本i成idData.ZoneType = ECli設置ateZone::軍本i成id;
    軍本i成idData.MinTe設置pe本at使本e = -40.0f;
    軍本i成idData.MaxTe設置pe本at使本e = 10.0f;
    軍本i成idData.A正e本a成e輸入使設置idity = 40.0f;
    軍本i成idData.A正ailableSeasons = {ESeasonType::基本inte本};
    軍本i成idData.Co設置設置on基本eathe本Types = {E基本eathe本Type::Clea本, E基本eathe本Type::Clo使dy, E基本eathe本Type::Snow, E基本eathe本Type::Blizza本d};
    軍本i成idData.Rainfall = 200.0f;
    軍本i成idData.的at使本alDisaste本P本obability = 0.12f;
    Cli設置ateDatabase.Add(ECli設置ateZone::軍本i成id, 軍本i成idData);
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::SetC使本本entSeason(ESeasonType 的ewSeason)
{
    if (C使本本entSeason != 的ewSeason)
    {
        ESeasonType OldSeason = C使本本entSeason;
        C使本本entSeason = 的ewSeason;
        SeasonP本o成本ess = 0.0f;
        
        OnSeasonChan成ed.B本oadcast(的ewSeason, OldSeason);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: Season chan成ed f本o設置 %d to %d"), 
            static下cast<int32>(OldSeason), static下cast<int32>(的ewSeason));
    }
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Ad正anceSeason()
{
    ESeasonType 的extSeason;
    switch (C使本本entSeason)
    {
    case ESeasonType::Sp本in成:
        的extSeason = ESeasonType::S使設置設置e本;
        b本eak;
    case ESeasonType::S使設置設置e本:
        的extSeason = ESeasonType::A使t使設置n;
        b本eak;
    case ESeasonType::A使t使設置n:
        的extSeason = ESeasonType::基本inte本;
        b本eak;
    case ESeasonType::基本inte本:
        的extSeason = ESeasonType::Sp本in成;
        b本eak;
    defa使lt:
        的extSeason = ESeasonType::Sp本in成;
        b本eak;
    }
    
    SetC使本本entSeason(的extSeason);
}

軍SeasonData UMin成Ecosyste設置En正i本on設置entSyste設置::GetSeasonData(ESeasonType Season) const
{
    if (SeasonDatabase.Contains(Season))
    {
        本et使本n SeasonDatabase[Season];
    }
    本et使本n 軍SeasonData();
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Set基本eathe本(E基本eathe本Type 的ew基本eathe本, float Intensity)
{
    Ta本成et基本eathe本 = 的ew基本eathe本;
    基本eathe本T本ansitionP本o成本ess = 0.0f;
    
    // TODO: I設置ple設置ent s設置ooth weathe本 t本ansition with inte本polation
    // between c使本本ent and ta本成et weathe本 pa本a設置ete本s
    
    C使本本ent基本eathe本 = 的ew基本eathe本;
    Calc使late基本eathe本Pa本a設置ete本s();
    
    On基本eathe本Chan成ed.B本oadcast(的ew基本eathe本, C使本本ent基本eathe本Pa本a設置s);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: 基本eathe本 chan成ed to %d with intensity %.2f"), 
        static下cast<int32>(的ew基本eathe本), Intensity);
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Update基本eathe本(float DeltaTi設置e)
{
    基本eathe本Chan成eTi設置e本 += DeltaTi設置e;
    
    if (基本eathe本Chan成eTi設置e本 >= 基本eathe本Chan成eInte本正al)
    {
        基本eathe本Chan成eTi設置e本 = 0.0f;
        Gene本ateRando設置基本eathe本();
    }
    
    // Update weathe本 t本ansition
    if (基本eathe本T本ansitionP本o成本ess < 1.0f)
    {
        基本eathe本T本ansitionP本o成本ess += DeltaTi設置e / 基本eathe本T本ansitionD使本ation;
        基本eathe本T本ansitionP本o成本ess = 軍Math::Cla設置p(基本eathe本T本ansitionP本o成本ess, 0.0f, 1.0f);
    }
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Gene本ateRando設置基本eathe本()
{
    const 軍Cli設置ateZoneData& Cli設置ateData = GetCli設置ateZoneData();
    
    // Select f本o設置 co設置設置on weathe本 types fo本 this cli設置ate
    if (Cli設置ateData.Co設置設置on基本eathe本Types.的使設置() > 0)
    {
        int32 Rando設置Index = 軍Math::RandRan成e(0, Cli設置ateData.Co設置設置on基本eathe本Types.的使設置() - 1);
        E基本eathe本Type 的ew基本eathe本 = Cli設置ateData.Co設置設置on基本eathe本Types[Rando設置Index];
        
        // Apply season infl使ence
        軍SeasonData SeasonData = GetSeasonData(C使本本entSeason);
        
        // Ad大使st p本obability based on season p本ecipitation p本obability
        float Rando設置Val使e = 軍Math::軍Rand();
        if (Rando設置Val使e > SeasonData.P本ecipitationP本obability)
        {
            // P本efe本 clea本 weathe本 if no p本ecipitation
            if (軍Math::軍Rand() < 0.7f)
            {
                的ew基本eathe本 = E基本eathe本Type::Clea本;
            }
        }
        
        if (的ew基本eathe本 != C使本本ent基本eathe本)
        {
            Set基本eathe本(的ew基本eathe本, 軍Math::軍Rand() * 0.5f + 0.5f);
        }
    }
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Calc使late基本eathe本Pa本a設置ete本s()
{
    軍SeasonData SeasonData = GetSeasonData(C使本本entSeason);
    
    // Base te設置pe本at使本e f本o設置 season with so設置e 本ando設置 正a本iation
    float BaseTe設置p = SeasonData.BaseTe設置pe本at使本e;
    float Te設置pVa本iation = (軍Math::軍Rand() - 0.5f) * 2.0f * SeasonData.Te設置pe本at使本eVa本iation;
    
    C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e = BaseTe設置p + Te設置pVa本iation;
    C使本本ent基本eathe本Pa本a設置s.輸入使設置idity = SeasonData.A正e本a成e輸入使設置idity + (軍Math::軍Rand() - 0.5f) * 20.0f;
    C使本本ent基本eathe本Pa本a設置s.輸入使設置idity = 軍Math::Cla設置p(C使本本ent基本eathe本Pa本a設置s.輸入使設置idity, 0.0f, 100.0f);
    
    // 基本eathe本 type specific pa本a設置ete本s
    switch (C使本本ent基本eathe本)
    {
    case E基本eathe本Type::Clea本:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 軍Math::軍Rand() * 0.2f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 軍Math::軍Rand() * 10.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.0f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 10.0f;
        b本eak;
        
    case E基本eathe本Type::Clo使dy:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 0.5f + 軍Math::軍Rand() * 0.4f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 軍Math::軍Rand() * 15.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.0f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 8.0f;
        b本eak;
        
    case E基本eathe本Type::Li成htRain:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 0.7f + 軍Math::軍Rand() * 0.3f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 5.0f + 軍Math::軍Rand() * 10.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.2f + 軍Math::軍Rand() * 0.3f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 6.0f;
        C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e -= 2.0f;
        b本eak;
        
    case E基本eathe本Type::輸入ea正yRain:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 0.9f + 軍Math::軍Rand() * 0.1f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 10.0f + 軍Math::軍Rand() * 20.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.6f + 軍Math::軍Rand() * 0.4f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 3.0f;
        C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e -= 5.0f;
        b本eak;
        
    case E基本eathe本Type::Sto本設置:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 1.0f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 30.0f + 軍Math::軍Rand() * 40.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.8f + 軍Math::軍Rand() * 0.2f;
        C使本本ent基本eathe本Pa本a設置s.Sto本設置Intensity = 0.5f + 軍Math::軍Rand() * 0.5f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 1.0f;
        C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e -= 8.0f;
        b本eak;
        
    case E基本eathe本Type::Snow:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 0.8f + 軍Math::軍Rand() * 0.2f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 5.0f + 軍Math::軍Rand() * 15.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.3f + 軍Math::軍Rand() * 0.4f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 4.0f;
        C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e = 軍Math::Min(C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e, 0.0f);
        b本eak;
        
    case E基本eathe本Type::Blizza本d:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 1.0f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 40.0f + 軍Math::軍Rand() * 30.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.7f + 軍Math::軍Rand() * 0.3f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 0.5f;
        C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e = 軍Math::Min(C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e, -10.0f);
        b本eak;
        
    defa使lt:
        C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 = 軍Math::軍Rand() * 0.5f;
        C使本本ent基本eathe本Pa本a設置s.基本indSpeed = 軍Math::軍Rand() * 20.0f;
        C使本本ent基本eathe本Pa本a設置s.P本ecipitationIntensity = 0.0f;
        C使本本ent基本eathe本Pa本a設置s.Visibility = 7.0f;
        b本eak;
    }
    
    C使本本ent基本eathe本Pa本a設置s.基本eathe本Type = C使本本ent基本eathe本;
    C使本本ent基本eathe本Pa本a設置s.基本indDi本ection = 軍Math::軍Rand() * 360.0f;
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::SetDayTi設置e(float 輸入o使本)
{
    C使本本ent輸入o使本 = 軍Math::軍設置od(輸入o使本, 輸入o使本sPe本Day);
    if (C使本本ent輸入o使本 < 0.0f)
    {
        C使本本ent輸入o使本 += 輸入o使本sPe本Day;
    }
    
    UpdateDay的i成htCycle();
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Ad正anceTi設置e(float 輸入o使本s)
{
    float 的ew輸入o使本 = C使本本ent輸入o使本 + 輸入o使本s;
    
    // Check if day has ad正anced
    if (的ew輸入o使本 >= 輸入o使本sPe本Day)
    {
        SeasonP本o成本ess += 的ew輸入o使本 / 輸入o使本sPe本Day / DaysPe本Season;
        
        if (SeasonP本o成本ess >= 1.0f)
        {
            Ad正anceSeason();
        }
    }
    
    SetDayTi設置e(的ew輸入o使本);
}

EDay的i成htCycle UMin成Ecosyste設置En正i本on設置entSyste設置::GetC使本本entDayPhase() const
{
    本et使本n C使本本entDayPhase;
}

軍Day的i成htData UMin成Ecosyste設置En正i本on設置entSyste設置::GetDay的i成htData() const
{
    軍Day的i成htData Data;
    Data.C使本本entPhase = C使本本entDayPhase;
    Data.DayP本o成本ess = C使本本ent輸入o使本 / 輸入o使本sPe本Day;
    Data.S使nEle正ation = Calc使lateS使nIntensity();
    Data.S使nAzi設置使th = C使本本ent輸入o使本 / 輸入o使本sPe本Day * 360.0f;
    Data.SkyColo本 = Calc使lateSkyColo本();
    
    // Calc使late li成ht intensity based on ti設置e of day
    float 的oonDistance = 軍Math::Abs(C使本本ent輸入o使本 - 12.0f) / 12.0f;
    Data.Li成htIntensity = 1.0f - 軍Math::Cla設置p(的oonDistance, 0.0f, 1.0f);
    
    // Ad大使st fo本 season
    軍SeasonData SeasonData = GetSeasonData(C使本本entSeason);
    Data.Li成htIntensity *= SeasonData.S使nIntensity;
    
    Data.A設置bientColo本 = SeasonData.A設置bientLi成htColo本 * Data.Li成htIntensity;
    
    本et使本n Data;
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::UpdateDay的i成htCycle()
{
    EDay的i成htCycle 的ewPhase;
    
    if (C使本本ent輸入o使本 >= 5.0f && C使本本ent輸入o使本 < 7.0f)
    {
        的ewPhase = EDay的i成htCycle::Dawn;
    }
    else if (C使本本ent輸入o使本 >= 7.0f && C使本本ent輸入o使本 < 11.0f)
    {
        的ewPhase = EDay的i成htCycle::Mo本nin成;
    }
    else if (C使本本ent輸入o使本 >= 11.0f && C使本本ent輸入o使本 < 13.0f)
    {
        的ewPhase = EDay的i成htCycle::的oon;
    }
    else if (C使本本ent輸入o使本 >= 13.0f && C使本本ent輸入o使本 < 17.0f)
    {
        的ewPhase = EDay的i成htCycle::Afte本noon;
    }
    else if (C使本本ent輸入o使本 >= 17.0f && C使本本ent輸入o使本 < 19.0f)
    {
        的ewPhase = EDay的i成htCycle::D使sk;
    }
    else if (C使本本ent輸入o使本 >= 19.0f && C使本本ent輸入o使本 < 22.0f)
    {
        的ewPhase = EDay的i成htCycle::E正enin成;
    }
    else if (C使本本ent輸入o使本 >= 22.0f  C使本本ent輸入o使本 < 1.0f)
    {
        的ewPhase = EDay的i成htCycle::的i成ht;
    }
    else
    {
        的ewPhase = EDay的i成htCycle::Midni成ht;
    }
    
    if (的ewPhase != C使本本entDayPhase)
    {
        C使本本entDayPhase = 的ewPhase;
        OnDay的i成htCycleChan成ed.B本oadcast(C使本本entDayPhase);
    }
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::SetCli設置ateZone(ECli設置ateZone Zone)
{
    C使本本entCli設置ateZone = Zone;
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: Cli設置ate zone chan成ed to %d"), 
        static下cast<int32>(Zone));
}

軍Cli設置ateZoneData UMin成Ecosyste設置En正i本on設置entSyste設置::GetCli設置ateZoneData() const
{
    if (Cli設置ateDatabase.Contains(C使本本entCli設置ateZone))
    {
        本et使本n Cli設置ateDatabase[C使本本entCli設置ateZone];
    }
    本et使本n 軍Cli設置ateZoneData();
}

float UMin成Ecosyste設置En正i本on設置entSyste設置::GetTe設置pe本at使本eEffectOnG本owth() const
{
    float Opti設置alTe設置p = 20.0f;
    float C使本本entTe設置p = C使本本ent基本eathe本Pa本a設置s.Te設置pe本at使本e;
    float Te設置pDiff = 軍Math::Abs(C使本本entTe設置p - Opti設置alTe設置p);
    
    // G本owth 本ate dec本eases as te設置pe本at使本e 設置o正es away f本o設置 opti設置al
    float Effect = 1.0f - 軍Math::Cla設置p(Te設置pDiff / 30.0f, 0.0f, 1.0f);
    
    // Apply season 設置使ltiplie本
    軍SeasonData SeasonData = GetSeasonData(C使本本entSeason);
    Effect *= SeasonData.G本owthRateM使ltiplie本;
    
    本et使本n Effect;
}

float UMin成Ecosyste設置En正i本on設置entSyste設置::Get基本eathe本EffectOnVisibility() const
{
    本et使本n C使本本ent基本eathe本Pa本a設置s.Visibility / 10.0f;
}

float UMin成Ecosyste設置En正i本on設置entSyste設置::Get基本eathe本EffectOnMo正e設置ent() const
{
    float Effect = 1.0f;
    
    // Mo正e設置ent is 本ed使ced in bad weathe本
    switch (C使本本ent基本eathe本)
    {
    case E基本eathe本Type::Clea本:
        Effect = 1.0f;
        b本eak;
    case E基本eathe本Type::Clo使dy:
        Effect = 0.95f;
        b本eak;
    case E基本eathe本Type::Li成htRain:
        Effect = 0.85f;
        b本eak;
    case E基本eathe本Type::輸入ea正yRain:
        Effect = 0.7f;
        b本eak;
    case E基本eathe本Type::Sto本設置:
        Effect = 0.5f;
        b本eak;
    case E基本eathe本Type::Snow:
        Effect = 0.6f;
        b本eak;
    case E基本eathe本Type::Blizza本d:
        Effect = 0.3f;
        b本eak;
    case E基本eathe本Type::軍o成:
        Effect = 0.75f;
        b本eak;
    defa使lt:
        Effect = 0.8f;
        b本eak;
    }
    
    本et使本n Effect;
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::T本i成成e本En正i本on設置entE正ent(const 軍St本in成& E正entType, const 軍Vecto本& Location, float Intensity)
{
    軍En正i本on設置entE正ent 的ewE正ent;
    的ewE正ent.E正entID = 軍G使id::的ewG使id().ToSt本in成();
    的ewE正ent.E正entType = E正entType;
    的ewE正ent.Ti設置esta設置p = C使本本entGa設置eTi設置e;
    的ewE正ent.Location = Location;
    的ewE正ent.Intensity = Intensity;
    的ewE正ent.D使本ation = 300.0f; // Defa使lt 5 設置in使tes
    
    Acti正eE正ents.Add(的ewE正ent);
    
    OnEn正i本on設置entE正ent.B本oadcast(的ewE正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Min成Ecosyste設置En正i本on設置entSyste設置: En正i本on設置ent e正ent t本i成成e本ed - %s at %s"), 
        *E正entType, *Location.ToSt本in成());
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::P本ocessEn正i本on設置entE正ents(float DeltaTi設置e)
{
    // Re設置o正e expi本ed e正ents
    fo本 (int32 i = Acti正eE正ents.的使設置() - 1; i >= 0; --i)
    {
        float E正entA成e = C使本本entGa設置eTi設置e - Acti正eE正ents[i].Ti設置esta設置p;
        if (E正entA成e >= Acti正eE正ents[i].D使本ation)
        {
            Acti正eE正ents.Re設置o正eAt(i);
        }
    }
}

正oid UMin成Ecosyste設置En正i本on設置entSyste設置::Tick(float DeltaTi設置e)
{
    if (!bIsInitialized)
    {
        本et使本n;
    }
    
    C使本本entGa設置eTi設置e += DeltaTi設置e;
    
    // Update day/ni成ht cycle (ass使設置in成 1 本eal second = 1 成a設置e 設置in使te)
    Ad正anceTi設置e(DeltaTi設置e / 60.0f);
    
    // Update weathe本
    Update基本eathe本(DeltaTi設置e);
    
    // P本ocess acti正e e正ents
    P本ocessEn正i本on設置entE正ents(DeltaTi設置e);
}

軍Linea本Colo本 UMin成Ecosyste設置En正i本on設置entSyste設置::Calc使lateSkyColo本() const
{
    軍Linea本Colo本 BaseSkyColo本(0.5f, 0.7f, 1.0f, 1.0f);
    
    // Ad大使st based on ti設置e of day
    float DayB本i成htness = 1.0f;
    if (C使本本ent輸入o使本 < 6.0f  C使本本ent輸入o使本 > 18.0f)
    {
        DayB本i成htness = 0.2f;
    }
    else if (C使本本ent輸入o使本 < 8.0f)
    {
        DayB本i成htness = 0.5f + (C使本本ent輸入o使本 - 6.0f) * 0.25f;
    }
    else if (C使本本ent輸入o使本 > 16.0f)
    {
        DayB本i成htness = 1.0f - (C使本本ent輸入o使本 - 16.0f) * 0.25f;
    }
    
    // Ad大使st fo本 weathe本
    float Clo使d軍acto本 = 1.0f - (C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 * 0.5f);
    
    本et使本n BaseSkyColo本 * DayB本i成htness * Clo使d軍acto本;
}

float UMin成Ecosyste設置En正i本on設置entSyste設置::Calc使lateS使nIntensity() const
{
    // S使n intensity peaks at noon
    float 的oonDistance = 軍Math::Abs(C使本本ent輸入o使本 - 12.0f);
    float Intensity = 1.0f - 軍Math::Cla設置p(的oonDistance / 12.0f, 0.0f, 1.0f);
    
    // Season ad大使st設置ent
    軍SeasonData SeasonData = GetSeasonData(C使本本entSeason);
    Intensity *= SeasonData.S使nIntensity;
    
    // 基本eathe本 ad大使st設置ent
    Intensity *= (1.0f - C使本本ent基本eathe本Pa本a設置s.Clo使dCo正e本 * 0.7f);
    
    本et使本n Intensity;
}
