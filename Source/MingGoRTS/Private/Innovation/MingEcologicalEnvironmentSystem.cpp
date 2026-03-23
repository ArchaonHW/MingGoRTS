#incl使de "Inno正ation/Min成Ecolo成icalEn正i本on設置entSyste設置.h"
#incl使de "En成ine/基本o本ld.h"
#incl使de "Ti設置e本Mana成e本.h"
#incl使de "Kis設置et/Kis設置etMathLib本a本y.h"
#incl使de "Math/Un本ealMathUtility.h"
#incl使de "Misc/DateTi設置e.h"

UMin成Ecolo成icalEn正i本on設置entSyste設置::UMin成Ecolo成icalEn正i本on設置entSyste設置()
{
    Si設置使lationSpeed = 1.0f;
    SeasonD使本ation = 90.0f; // 90 days pe本 season
    bEnable基本eathe本Syste設置 = t本使e;
    bEnableCli設置ateChan成e = t本使e;
    bEnableReso使本ceRe成ene本ation = t本使e;
    bEnableEcosyste設置Balance = t本使e;
    C使本本entSeason = ESeason::Sp本in成;
    C使本本ent基本eathe本 = E基本eathe本Type::S使nny;
    Te設置pe本at使本e = 20.0f;
    輸入使設置idity = 50.0f;
    P本ecipitation = 0.0f;
    基本indSpeed = 5.0f;
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::InitializeEn正i本on設置entSyste設置()
{
    // Initialize syste設置 state
    bSyste設置Initialized = t本使e;
    LastUpdateTi設置e = 軍DateTi設置e::的ow();
    
    // Clea本 existin成 data
    Ecosyste設置s.E設置pty();
    En正i本on設置entalZones.E設置pty();
    Reso使本ces.E設置pty();
    Cli設置ateData.E設置pty();
    
    // C本eate defa使lt ecosyste設置
    C本eateDefa使ltEcosyste設置();
    
    // Initialize cli設置ate syste設置
    InitializeCli設置ateSyste設置();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ecolo成ical En正i本on設置ent Syste設置 initialized"));
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Sh使tdownEn正i本on設置entSyste設置()
{
    bSyste設置Initialized = false;
    
    // Clea本 all data
    Ecosyste設置s.E設置pty();
    En正i本on設置entalZones.E設置pty();
    Reso使本ces.E設置pty();
    Cli設置ateData.E設置pty();
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Ecolo成ical En正i本on設置ent Syste設置 sh使tdown"));
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::C本eateEcosyste設置(const 軍Ecosyste設置& Ecosyste設置)
{
    // Validate ecosyste設置
    軍Ecosyste設置 ValidatedEcosyste設置 = Ecosyste設置;
    ValidateEcosyste設置(ValidatedEcosyste設置);
    
    // Add ecosyste設置 with 使niq使e ID
    if (ValidatedEcosyste設置.Ecosyste設置ID.IsE設置pty())
    {
        ValidatedEcosyste設置.Ecosyste設置ID = 軍St本in成::P本intf(TEXT("Ecosyste設置下%d"), Ecosyste設置s.的使設置());
    }
    
    Ecosyste設置s.Add(ValidatedEcosyste設置.Ecosyste設置ID, ValidatedEcosyste設置);
    
    // C本eate en正i本on設置ental zones fo本 this ecosyste設置
    C本eateEn正i本on設置entalZones(ValidatedEcosyste設置);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("C本eated ecosyste設置: %s"), *ValidatedEcosyste設置.Ecosyste設置ID);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Re設置o正eEcosyste設置(const 軍St本in成& Ecosyste設置ID)
{
    if (Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        // Re設置o正e associated en正i本on設置ental zones
        En正i本on設置entalZones.Re設置o正eAll([&](const 軍En正i本on設置entalZone& Zone) {
            本et使本n Zone.Ecosyste設置ID == Ecosyste設置ID;
        });
        
        // Re設置o正e ecosyste設置
        Ecosyste設置s.Re設置o正e(Ecosyste設置ID);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed ecosyste設置: %s"), *Ecosyste設置ID);
    }
}

軍Ecosyste設置 UMin成Ecolo成icalEn正i本on設置entSyste設置::GetEcosyste設置(const 軍St本in成& Ecosyste設置ID) const
{
    if (Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        本et使本n Ecosyste設置s[Ecosyste設置ID];
    }
    
    本et使本n 軍Ecosyste設置();
}

TA本本ay<軍Ecosyste設置> UMin成Ecolo成icalEn正i本on設置entSyste設置::GetAllEcosyste設置s() const
{
    TA本本ay<軍Ecosyste設置> AllEcosyste設置s;
    
    fo本 (const a使to& Ecosyste設置Pai本 : Ecosyste設置s)
    {
        AllEcosyste設置s.Add(Ecosyste設置Pai本.Val使e);
    }
    
    本et使本n AllEcosyste設置s;
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::C本eateEn正i本on設置entalZones(const 軍Ecosyste設置& Ecosyste設置)
{
    // C本eate zones based on ecosyste設置 type
    TA本本ay<EZoneType> ZoneTypes = GetZoneTypes軍o本Ecosyste設置(Ecosyste設置.Type);
    
    fo本 (EZoneType ZoneType : ZoneTypes)
    {
        軍En正i本on設置entalZone Zone;
        Zone.ZoneID = 軍St本in成::P本intf(TEXT("%s下Zone下%d"), *Ecosyste設置.Ecosyste設置ID, En正i本on設置entalZones.的使設置());
        Zone.Ecosyste設置ID = Ecosyste設置.Ecosyste設置ID;
        Zone.Type = ZoneType;
        Zone.Size = 軍Math::RandRan成e(100, 1000); // Rando設置 size in sq使a本e kilo設置ete本s
        Zone.Te設置pe本at使本e = Calc使lateZoneTe設置pe本at使本e(ZoneType);
        Zone.輸入使設置idity = Calc使lateZone輸入使設置idity(ZoneType);
        Zone.軍e本tility = Calc使lateZone軍e本tility(ZoneType);
        Zone.Biodi正e本sity = Calc使lateZoneBiodi正e本sity(ZoneType);
        Zone.Reso使本ceDensity = Calc使lateZoneReso使本ceDensity(ZoneType);
        
        En正i本on設置entalZones.Add(Zone);
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateEn正i本on設置entalConditions(float DeltaTi設置e)
{
    if (!bSyste設置Initialized)
    {
        本et使本n;
    }
    
    // Update weathe本
    Update基本eathe本(DeltaTi設置e);
    
    // Update season
    UpdateSeason(DeltaTi設置e);
    
    // Update cli設置ate
    if (bEnableCli設置ateChan成e)
    {
        UpdateCli設置ate(DeltaTi設置e);
    }
    
    // Update ecosyste設置 conditions
    UpdateEcosyste設置Conditions(DeltaTi設置e);
    
    // Update 本eso使本ce 本e成ene本ation
    if (bEnableReso使本ceRe成ene本ation)
    {
        UpdateReso使本ceRe成ene本ation(DeltaTi設置e);
    }
    
    // Update ecosyste設置 balance
    if (bEnableEcosyste設置Balance)
    {
        UpdateEcosyste設置Balance(DeltaTi設置e);
    }
    
    LastUpdateTi設置e = 軍DateTi設置e::的ow();
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Set基本eathe本(E基本eathe本Type 的ew基本eathe本)
{
    C使本本ent基本eathe本 = 的ew基本eathe本;
    
    // Update en正i本on設置ental pa本a設置ete本s based on weathe本
    switch (的ew基本eathe本)
    {
        case E基本eathe本Type::S使nny:
            Te設置pe本at使本e += 2.0f;
            輸入使設置idity -= 10.0f;
            P本ecipitation = 0.0f;
            b本eak;
        case E基本eathe本Type::Clo使dy:
            Te設置pe本at使本e -= 1.0f;
            輸入使設置idity += 5.0f;
            P本ecipitation = 0.0f;
            b本eak;
        case E基本eathe本Type::Rainy:
            Te設置pe本at使本e -= 3.0f;
            輸入使設置idity += 20.0f;
            P本ecipitation = 軍Math::RandRan成e(5.0f, 25.0f);
            b本eak;
        case E基本eathe本Type::Sto本設置y:
            Te設置pe本at使本e -= 5.0f;
            輸入使設置idity += 15.0f;
            P本ecipitation = 軍Math::RandRan成e(20.0f, 50.0f);
            基本indSpeed += 15.0f;
            b本eak;
        case E基本eathe本Type::Snowy:
            Te設置pe本at使本e -= 8.0f;
            輸入使設置idity += 10.0f;
            P本ecipitation = 軍Math::RandRan成e(2.0f, 15.0f);
            b本eak;
        case E基本eathe本Type::軍o成成y:
            Te設置pe本at使本e -= 2.0f;
            輸入使設置idity += 25.0f;
            P本ecipitation = 0.0f;
            基本indSpeed -= 5.0f;
            b本eak;
    }
    
    // Cla設置p 正al使es
    Te設置pe本at使本e = 軍Math::Cla設置p(Te設置pe本at使本e, -30.0f, 50.0f);
    輸入使設置idity = 軍Math::Cla設置p(輸入使設置idity, 0.0f, 100.0f);
    基本indSpeed = 軍Math::Cla設置p(基本indSpeed, 0.0f, 100.0f);
    
    // B本oadcast weathe本 chan成e
    On基本eathe本Chan成ed.B本oadcast(C使本本ent基本eathe本);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("基本eathe本 chan成ed to: %s"), *UEn使設置::GetVal使eAsSt本in成(的ew基本eathe本));
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::SetSeason(ESeason 的ewSeason)
{
    C使本本entSeason = 的ewSeason;
    
    // Update en正i本on設置ental pa本a設置ete本s based on season
    switch (的ewSeason)
    {
        case ESeason::Sp本in成:
            Te設置pe本at使本e = 15.0f;
            輸入使設置idity = 60.0f;
            P本ecipitation = 10.0f;
            b本eak;
        case ESeason::S使設置設置e本:
            Te設置pe本at使本e = 28.0f;
            輸入使設置idity = 40.0f;
            P本ecipitation = 5.0f;
            b本eak;
        case ESeason::A使t使設置n:
            Te設置pe本at使本e = 12.0f;
            輸入使設置idity = 65.0f;
            P本ecipitation = 15.0f;
            b本eak;
        case ESeason::基本inte本:
            Te設置pe本at使本e = -2.0f;
            輸入使設置idity = 50.0f;
            P本ecipitation = 8.0f;
            b本eak;
    }
    
    // B本oadcast season chan成e
    OnSeasonChan成ed.B本oadcast(C使本本entSeason);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Season chan成ed to: %s"), *UEn使設置::GetVal使eAsSt本in成(的ewSeason));
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::AddEn正i本on設置entalReso使本ce(const 軍En正i本on設置entalReso使本ce& Reso使本ce)
{
    // Validate 本eso使本ce
    軍En正i本on設置entalReso使本ce ValidatedReso使本ce = Reso使本ce;
    ValidateReso使本ce(ValidatedReso使本ce);
    
    // Add 本eso使本ce with 使niq使e ID
    if (ValidatedReso使本ce.Reso使本ceID.IsE設置pty())
    {
        ValidatedReso使本ce.Reso使本ceID = 軍St本in成::P本intf(TEXT("Reso使本ce下%d"), Reso使本ces.的使設置());
    }
    
    Reso使本ces.Add(ValidatedReso使本ce.Reso使本ceID, ValidatedReso使本ce);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Added en正i本on設置ental 本eso使本ce: %s"), *ValidatedReso使本ce.Reso使本ceID);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Re設置o正eReso使本ce(const 軍St本in成& Reso使本ceID)
{
    if (Reso使本ces.Contains(Reso使本ceID))
    {
        Reso使本ces.Re設置o正e(Reso使本ceID);
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Re設置o正ed en正i本on設置ental 本eso使本ce: %s"), *Reso使本ceID);
    }
}

軍En正i本on設置entalReso使本ce UMin成Ecolo成icalEn正i本on設置entSyste設置::GetReso使本ce(const 軍St本in成& Reso使本ceID) const
{
    if (Reso使本ces.Contains(Reso使本ceID))
    {
        本et使本n Reso使本ces[Reso使本ceID];
    }
    
    本et使本n 軍En正i本on設置entalReso使本ce();
}

TA本本ay<軍En正i本on設置entalReso使本ce> UMin成Ecolo成icalEn正i本on設置entSyste設置::GetReso使本cesInZone(const 軍St本in成& ZoneID) const
{
    TA本本ay<軍En正i本on設置entalReso使本ce> ZoneReso使本ces;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        const 軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本cePai本.Val使e;
        
        if (Reso使本ce.ZoneID == ZoneID)
        {
            ZoneReso使本ces.Add(Reso使本ce);
        }
    }
    
    本et使本n ZoneReso使本ces;
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Si設置使lateCli設置ateChan成e(float DeltaTi設置e)
{
    if (!bEnableCli設置ateChan成e)
    {
        本et使本n;
    }
    
    // Si設置使late 成本ad使al cli設置ate chan成e
    float Cli設置ateChan成eRate = 0.001f; // Ve本y slow chan成e
    
    // Global wa本設置in成 effect
    Te設置pe本at使本e += Cli設置ateChan成eRate * DeltaTi設置e;
    
    // Inc本eased ext本e設置e weathe本 e正ents
    if (軍Math::軍Rand() < 0.001f) // S設置all chance of ext本e設置e weathe本
    {
        E基本eathe本Type Ext本e設置e基本eathe本 = GetExt本e設置e基本eathe本Type();
        Set基本eathe本(Ext本e設置e基本eathe本);
    }
    
    // Update cli設置ate data
    UpdateCli設置ateData();
    
    UE下LOG(Lo成Te設置p, Ve本yVe本bose, TEXT("Cli設置ate chan成e si設置使lation: Te設置pe本at使本e = %.2f"), Te設置pe本at使本e);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::P本ocessEn正i本on設置entalE正ent(const 軍En正i本on設置entalE正ent& E正ent)
{
    // Apply e正ent effects
    fo本 (const a使to& EffectPai本 : E正ent.ZoneEffects)
    {
        const 軍St本in成& ZoneID = EffectPai本.Key;
        const 軍En正i本on設置entalEffect& Effect = EffectPai本.Val使e;
        
        ApplyEn正i本on設置entalEffect(ZoneID, Effect);
    }
    
    // Apply 本eso使本ce effects
    fo本 (const a使to& Reso使本ceEffect : E正ent.Reso使本ceEffects)
    {
        if (Reso使本ces.Contains(Reso使本ceEffect.Reso使本ceID))
        {
            軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本ces[Reso使本ceEffect.Reso使本ceID];
            Reso使本ce.Q使antity *= Reso使本ceEffect.Q使antityM使ltiplie本;
            Reso使本ce.Q使ality *= Reso使本ceEffect.Q使alityM使ltiplie本;
            
            // Cla設置p 正al使es
            Reso使本ce.Q使antity = 軍Math::Max(0.0f, Reso使本ce.Q使antity);
            Reso使本ce.Q使ality = 軍Math::Cla設置p(Reso使本ce.Q使ality, 0.0f, 1.0f);
        }
    }
    
    // B本oadcast e正ent
    OnEn正i本on設置entalE正entOcc使本本ed.B本oadcast(E正ent);
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("P本ocessed en正i本on設置ental e正ent: %s"), *E正ent.E正entID);
}

軍En正i本on設置entalMet本ics UMin成Ecolo成icalEn正i本on設置entSyste設置::GetEn正i本on設置entalMet本ics() const
{
    軍En正i本on設置entalMet本ics Met本ics;
    
    // Calc使late o正e本all 設置et本ics
    Met本ics.TotalEcosyste設置s = Ecosyste設置s.的使設置();
    Met本ics.TotalZones = En正i本on設置entalZones.的使設置();
    Met本ics.TotalReso使本ces = Reso使本ces.的使設置();
    Met本ics.A正e本a成eTe設置pe本at使本e = Calc使lateA正e本a成eTe設置pe本at使本e();
    Met本ics.A正e本a成e輸入使設置idity = Calc使lateA正e本a成e輸入使設置idity();
    Met本ics.TotalBiodi正e本sity = Calc使lateTotalBiodi正e本sity();
    Met本ics.Ecosyste設置輸入ealth = Calc使lateEcosyste設置輸入ealth();
    設置et本ics.Cli設置ateStability = Calc使lateCli設置ateStability();
    設置et本ics.Reso使本ceS使stainability = Calc使lateReso使本ceS使stainability();
    
    本et使本n Met本ics;
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Resto本eEcosyste設置(const 軍St本in成& Ecosyste設置ID)
{
    if (Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        軍Ecosyste設置& Ecosyste設置 = Ecosyste設置s[Ecosyste設置ID];
        
        // Reset ecosyste設置 health
        Ecosyste設置.輸入ealth = 1.0f;
        Ecosyste設置.Balance = 1.0f;
        
        // Resto本e 本eso使本ces in this ecosyste設置
        fo本 (a使to& Reso使本cePai本 : Reso使本ces)
        {
            軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本cePai本.Val使e;
            
            // 軍ind zones belon成in成 to this ecosyste設置
            fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
            {
                if (Zone.Ecosyste設置ID == Ecosyste設置ID && Reso使本ce.ZoneID == Zone.ZoneID)
                {
                    // Resto本e 本eso使本ce to o本i成inal q使antity
                    Reso使本ce.Q使antity = Reso使本ce.O本i成inalQ使antity;
                    Reso使本ce.Q使ality = 1.0f;
                    b本eak;
                }
            }
        }
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Resto本ed ecosyste設置: %s"), *Ecosyste設置ID);
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::BalanceEcosyste設置(const 軍St本in成& Ecosyste設置ID)
{
    if (Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        軍Ecosyste設置& Ecosyste設置 = Ecosyste設置s[Ecosyste設置ID];
        
        // Calc使late c使本本ent balance
        float C使本本entBalance = Calc使lateEcosyste設置Balance(Ecosyste設置ID);
        
        // Apply balancin成 設置eas使本es
        if (C使本本entBalance < 0.5f)
        {
            // Ecosyste設置 is 使nbalanced, apply co本本ecti正e 設置eas使本es
            ApplyBalancin成Meas使本es(Ecosyste設置ID);
        }
        
        Ecosyste設置.Balance = 軍Math::Cla設置p(C使本本entBalance, 0.0f, 1.0f);
        
        UE下LOG(Lo成Te設置p, Lo成, TEXT("Balanced ecosyste設置: %s (Balance: %.2f)"), *Ecosyste設置ID, Ecosyste設置.Balance);
    }
}

// P本i正ate helpe本 f使nctions

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::C本eateDefa使ltEcosyste設置()
{
    軍Ecosyste設置 Defa使ltEcosyste設置;
    Defa使ltEcosyste設置.Ecosyste設置ID = TEXT("Defa使ltEcosyste設置");
    Defa使ltEcosyste設置.的a設置e = TEXT("Defa使lt Ecosyste設置");
    Defa使ltEcosyste設置.Type = EEcosyste設置Type::軍o本est;
    Defa使ltEcosyste設置.輸入ealth = 1.0f;
    Defa使ltEcosyste設置.Balance = 1.0f;
    Defa使ltEcosyste設置.Biodi正e本sity = 0.8f;
    Defa使ltEcosyste設置.Resilience = 0.7f;
    
    Ecosyste設置s.Add(Defa使ltEcosyste設置.Ecosyste設置ID, Defa使ltEcosyste設置);
    
    // C本eate zones fo本 defa使lt ecosyste設置
    C本eateEn正i本on設置entalZones(Defa使ltEcosyste設置);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::InitializeCli設置ateSyste設置()
{
    // Initialize cli設置ate data
    軍Cli設置ateData InitialData;
    InitialData.Ti設置esta設置p = 軍DateTi設置e::的ow();
    InitialData.Te設置pe本at使本e = Te設置pe本at使本e;
    InitialData.輸入使設置idity = 輸入使設置idity;
    InitialData.P本ecipitation = P本ecipitation;
    InitialData.基本indSpeed = 基本indSpeed;
    InitialData.Season = C使本本entSeason;
    InitialData.基本eathe本 = C使本本ent基本eathe本;
    
    Cli設置ateData.Add(InitialData);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::ValidateEcosyste設置(軍Ecosyste設置& Ecosyste設置)
{
    // Cla設置p 正al使es
    Ecosyste設置.輸入ealth = 軍Math::Cla設置p(Ecosyste設置.輸入ealth, 0.0f, 1.0f);
    Ecosyste設置.Balance = 軍Math::Cla設置p(Ecosyste設置.Balance, 0.0f, 1.0f);
    Ecosyste設置.Biodi正e本sity = 軍Math::Cla設置p(Ecosyste設置.Biodi正e本sity, 0.0f, 1.0f);
    Ecosyste設置.Resilience = 軍Math::Cla設置p(Ecosyste設置.Resilience, 0.0f, 1.0f);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::ValidateReso使本ce(軍En正i本on設置entalReso使本ce& Reso使本ce)
{
    // Cla設置p 正al使es
    Reso使本ce.Q使antity = 軍Math::Max(0.0f, Reso使本ce.Q使antity);
    Reso使本ce.Q使ality = 軍Math::Cla設置p(Reso使本ce.Q使ality, 0.0f, 1.0f);
    Reso使本ce.Re成ene本ationRate = 軍Math::Max(0.0f, Reso使本ce.Re成ene本ationRate);
}

TA本本ay<EZoneType> UMin成Ecolo成icalEn正i本on設置entSyste設置::GetZoneTypes軍o本Ecosyste設置(EEcosyste設置Type Ecosyste設置Type) const
{
    TA本本ay<EZoneType> ZoneTypes;
    
    switch (Ecosyste設置Type)
    {
        case EEcosyste設置Type::軍o本est:
            ZoneTypes = {EZoneType::軍o本est, EZoneType::G本assland, EZoneType::基本etland};
            b本eak;
        case EEcosyste設置Type::Dese本t:
            ZoneTypes = {EZoneType::Dese本t, EZoneType::Mo使ntain};
            b本eak;
        case EEcosyste設置Type::Ocean:
            ZoneTypes = {EZoneType::Ocean, EZoneType::Coastal};
            b本eak;
        case EEcosyste設置Type::G本assland:
            ZoneTypes = {EZoneType::G本assland, EZoneType::基本etland};
            b本eak;
        case EEcosyste設置Type::T使nd本a:
            ZoneTypes = {EZoneType::T使nd本a, EZoneType::Mo使ntain};
            b本eak;
        case EEcosyste設置Type::U本ban:
            ZoneTypes = {EZoneType::U本ban, EZoneType::Ind使st本ial};
            b本eak;
        defa使lt:
            ZoneTypes = {EZoneType::軍o本est};
            b本eak;
    }
    
    本et使本n ZoneTypes;
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateZoneTe設置pe本at使本e(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::軍o本est: 本et使本n 18.0f;
        case EZoneType::Dese本t: 本et使本n 35.0f;
        case EZoneType::Ocean: 本et使本n 22.0f;
        case EZoneType::G本assland: 本et使本n 20.0f;
        case EZoneType::Mo使ntain: 本et使本n 10.0f;
        case EZoneType::基本etland: 本et使本n 25.0f;
        case EZoneType::T使nd本a: 本et使本n -5.0f;
        case EZoneType::U本ban: 本et使本n 24.0f;
        case EZoneType::Ind使st本ial: 本et使本n 26.0f;
        case EZoneType::Coastal: 本et使本n 21.0f;
        defa使lt: 本et使本n 20.0f;
    }
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateZone輸入使設置idity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::軍o本est: 本et使本n 70.0f;
        case EZoneType::Dese本t: 本et使本n 15.0f;
        case EZoneType::Ocean: 本et使本n 85.0f;
        case EZoneType::G本assland: 本et使本n 45.0f;
        case EZoneType::Mo使ntain: 本et使本n 55.0f;
        case EZoneType::基本etland: 本et使本n 90.0f;
        case EZoneType::T使nd本a: 本et使本n 40.0f;
        case EZoneType::U本ban: 本et使本n 50.0f;
        case EZoneType::Ind使st本ial: 本et使本n 35.0f;
        case EZoneType::Coastal: 本et使本n 75.0f;
        defa使lt: 本et使本n 50.0f;
    }
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateZone軍e本tility(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::軍o本est: 本et使本n 0.8f;
        case EZoneType::Dese本t: 本et使本n 0.1f;
        case EZoneType::Ocean: 本et使本n 0.3f;
        case EZoneType::G本assland: 本et使本n 0.7f;
        case EZoneType::Mo使ntain: 本et使本n 0.2f;
        case EZoneType::基本etland: 本et使本n 0.9f;
        case EZoneType::T使nd本a: 本et使本n 0.1f;
        case EZoneType::U本ban: 本et使本n 0.4f;
        case EZoneType::Ind使st本ial: 本et使本n 0.2f;
        case EZoneType::Coastal: 本et使本n 0.6f;
        defa使lt: 本et使本n 0.5f;
    }
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateZoneBiodi正e本sity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::軍o本est: 本et使本n 0.9f;
        case EZoneType::Dese本t: 本et使本n 0.3f;
        case EZoneType::Ocean: 本et使本n 0.8f;
        case EZoneType::G本assland: 本et使本n 0.6f;
        case EZoneType::Mo使ntain: 本et使本n 0.5f;
        case EZoneType::基本etland: 本et使本n 0.8f;
        case EZoneType::T使nd本a: 本et使本n 0.2f;
        case EZoneType::U本ban: 本et使本n 0.1f;
        case EZoneType::Ind使st本ial: 本et使本n 0.05f;
        case EZoneType::Coastal: 本et使本n 0.7f;
        defa使lt: 本et使本n 0.5f;
    }
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateZoneReso使本ceDensity(EZoneType ZoneType) const
{
    switch (ZoneType)
    {
        case EZoneType::軍o本est: 本et使本n 0.7f;
        case EZoneType::Dese本t: 本et使本n 0.2f;
        case EZoneType::Ocean: 本et使本n 0.4f;
        case EZoneType::G本assland: 本et使本n 0.6f;
        case EZoneType::Mo使ntain: 本et使本n 0.3f;
        case EZoneType::基本etland: 本et使本n 0.5f;
        case EZoneType::T使nd本a: 本et使本n 0.1f;
        case EZoneType::U本ban: 本et使本n 0.8f;
        case EZoneType::Ind使st本ial: 本et使本n 0.9f;
        case EZoneType::Coastal: 本et使本n 0.6f;
        defa使lt: 本et使本n 0.5f;
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::Update基本eathe本(float DeltaTi設置e)
{
    if (!bEnable基本eathe本Syste設置)
    {
        本et使本n;
    }
    
    // Rando設置 weathe本 chan成es
    if (軍Math::軍Rand() < 0.01f) // 1% chance pe本 tick
    {
        TA本本ay<E基本eathe本Type> Possible基本eathe本 = {
            E基本eathe本Type::S使nny, E基本eathe本Type::Clo使dy, E基本eathe本Type::Rainy,
            E基本eathe本Type::Sto本設置y, E基本eathe本Type::Snowy, E基本eathe本Type::軍o成成y
        };
        
        E基本eathe本Type 的ew基本eathe本 = Possible基本eathe本[軍Math::RandRan成e(0, Possible基本eathe本.的使設置() - 1)];
        Set基本eathe本(的ew基本eathe本);
    }
    
    // G本ad使al weathe本 chan成es
    Te設置pe本at使本e += 軍Math::RandRan成e(-0.1f, 0.1f) * DeltaTi設置e;
    輸入使設置idity += 軍Math::RandRan成e(-0.5f, 0.5f) * DeltaTi設置e;
    基本indSpeed += 軍Math::RandRan成e(-0.2f, 0.2f) * DeltaTi設置e;
    
    // Cla設置p 正al使es
    Te設置pe本at使本e = 軍Math::Cla設置p(Te設置pe本at使本e, -30.0f, 50.0f);
    輸入使設置idity = 軍Math::Cla設置p(輸入使設置idity, 0.0f, 100.0f);
    基本indSpeed = 軍Math::Cla設置p(基本indSpeed, 0.0f, 100.0f);
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateSeason(float DeltaTi設置e)
{
    static float SeasonTi設置e本 = 0.0f;
    SeasonTi設置e本 += DeltaTi設置e * Si設置使lationSpeed;
    
    if (SeasonTi設置e本 >= SeasonD使本ation)
    {
        // Chan成e to next season
        int32 C使本本entSeasonVal使e = static下cast<int32>(C使本本entSeason);
        C使本本entSeasonVal使e = (C使本本entSeasonVal使e + 1) % 4;
        C使本本entSeason = static下cast<ESeason>(C使本本entSeasonVal使e);
        
        SetSeason(C使本本entSeason);
        SeasonTi設置e本 = 0.0f;
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateCli設置ate(float DeltaTi設置e)
{
    // Update cli設置ate data
    軍Cli設置ateData 的ewData;
    的ewData.Ti設置esta設置p = 軍DateTi設置e::的ow();
    的ewData.Te設置pe本at使本e = Te設置pe本at使本e;
    的ewData.輸入使設置idity = 輸入使設置idity;
    的ewData.P本ecipitation = P本ecipitation;
    的ewData.基本indSpeed = 基本indSpeed;
    的ewData.Season = C使本本entSeason;
    的ewData.基本eathe本 = C使本本ent基本eathe本;
    
    Cli設置ateData.Add(的ewData);
    
    // Li設置it cli設置ate data histo本y
    if (Cli設置ateData.的使設置() > 1000)
    {
        Cli設置ateData.Re設置o正eAt(0);
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateEcosyste設置Conditions(float DeltaTi設置e)
{
    fo本 (a使to& Ecosyste設置Pai本 : Ecosyste設置s)
    {
        軍Ecosyste設置& Ecosyste設置 = Ecosyste設置Pai本.Val使e;
        
        // 的at使本al ecosyste設置 chan成es
        Ecosyste設置.輸入ealth += 軍Math::RandRan成e(-0.001f, 0.001f) * DeltaTi設置e;
        Ecosyste設置.Balance += 軍Math::RandRan成e(-0.001f, 0.001f) * DeltaTi設置e;
        
        // Cla設置p 正al使es
        Ecosyste設置.輸入ealth = 軍Math::Cla設置p(Ecosyste設置.輸入ealth, 0.0f, 1.0f);
        Ecosyste設置.Balance = 軍Math::Cla設置p(Ecosyste設置.Balance, 0.0f, 1.0f);
        
        // Apply en正i本on設置ental effects
        ApplyEn正i本on設置entalEffectsToEcosyste設置(Ecosyste設置);
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateReso使本ceRe成ene本ation(float DeltaTi設置e)
{
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本cePai本.Val使e;
        
        // Re成ene本ate 本eso使本ces
        if (Reso使本ce.Q使antity < Reso使本ce.O本i成inalQ使antity)
        {
            Reso使本ce.Q使antity += Reso使本ce.Re成ene本ationRate * DeltaTi設置e;
            Reso使本ce.Q使antity = 軍Math::Min(Reso使本ce.Q使antity, Reso使本ce.O本i成inalQ使antity);
        }
        
        // Q使ality chan成es
        Reso使本ce.Q使ality += 軍Math::RandRan成e(-0.0001f, 0.0001f) * DeltaTi設置e;
        Reso使本ce.Q使ality = 軍Math::Cla設置p(Reso使本ce.Q使ality, 0.0f, 1.0f);
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateEcosyste設置Balance(float DeltaTi設置e)
{
    fo本 (a使to& Ecosyste設置Pai本 : Ecosyste設置s)
    {
        const 軍St本in成& Ecosyste設置ID = Ecosyste設置Pai本.Key;
        float C使本本entBalance = Calc使lateEcosyste設置Balance(Ecosyste設置ID);
        
        // A使to-balance if needed
        if (C使本本entBalance < 0.3f)
        {
            ApplyBalancin成Meas使本es(Ecosyste設置ID);
        }
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::ApplyEn正i本on設置entalEffectsToEcosyste設置(軍Ecosyste設置& Ecosyste設置)
{
    // Apply weathe本 effects
    switch (C使本本ent基本eathe本)
    {
        case E基本eathe本Type::Sto本設置y:
            Ecosyste設置.輸入ealth -= 0.01f;
            Ecosyste設置.Balance -= 0.02f;
            b本eak;
        case E基本eathe本Type::S使nny:
            Ecosyste設置.輸入ealth += 0.005f;
            b本eak;
        case E基本eathe本Type::Rainy:
            Ecosyste設置.輸入ealth += 0.01f;
            Ecosyste設置.Balance += 0.005f;
            b本eak;
    }
    
    // Apply seasonal effects
    switch (C使本本entSeason)
    {
        case ESeason::Sp本in成:
            Ecosyste設置.輸入ealth += 0.01f;
            Ecosyste設置.Balance += 0.005f;
            b本eak;
        case ESeason::基本inte本:
            Ecosyste設置.輸入ealth -= 0.005f;
            b本eak;
    }
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::ApplyEn正i本on設置entalEffect(const 軍St本in成& ZoneID, const 軍En正i本on設置entalEffect& Effect)
{
    // 軍ind and 使pdate zone
    fo本 (軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
    {
        if (Zone.ZoneID == ZoneID)
        {
            Zone.Te設置pe本at使本e += Effect.Te設置pe本at使本eChan成e;
            Zone.輸入使設置idity += Effect.輸入使設置idityChan成e;
            Zone.軍e本tility += Effect.軍e本tilityChan成e;
            Zone.Biodi正e本sity += Effect.Biodi正e本sityChan成e;
            Zone.Reso使本ceDensity += Effect.Reso使本ceDensityChan成e;
            
            // Cla設置p 正al使es
            Zone.Te設置pe本at使本e = 軍Math::Cla設置p(Zone.Te設置pe本at使本e, -50.0f, 60.0f);
            Zone.輸入使設置idity = 軍Math::Cla設置p(Zone.輸入使設置idity, 0.0f, 100.0f);
            Zone.軍e本tility = 軍Math::Cla設置p(Zone.軍e本tility, 0.0f, 1.0f);
            Zone.Biodi正e本sity = 軍Math::Cla設置p(Zone.Biodi正e本sity, 0.0f, 1.0f);
            Zone.Reso使本ceDensity = 軍Math::Cla設置p(Zone.Reso使本ceDensity, 0.0f, 1.0f);
            
            b本eak;
        }
    }
}

E基本eathe本Type UMin成Ecolo成icalEn正i本on設置entSyste設置::GetExt本e設置e基本eathe本Type() const
{
    TA本本ay<E基本eathe本Type> Ext本e設置e基本eathe本 = {
        E基本eathe本Type::Sto本設置y, E基本eathe本Type::Snowy
    };
    
    本et使本n Ext本e設置e基本eathe本[軍Math::RandRan成e(0, Ext本e設置e基本eathe本.的使設置() - 1)];
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::UpdateCli設置ateData()
{
    // Cli設置ate data is 使pdated in UpdateCli設置ate f使nction
    // This f使nction can be 使sed fo本 additional cli設置ate analysis
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateA正e本a成eTe設置pe本at使本e() const
{
    if (En正i本on設置entalZones.的使設置() == 0)
    {
        本et使本n Te設置pe本at使本e;
    }
    
    float TotalTe設置pe本at使本e = 0.0f;
    fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
    {
        TotalTe設置pe本at使本e += Zone.Te設置pe本at使本e;
    }
    
    本et使本n TotalTe設置pe本at使本e / En正i本on設置entalZones.的使設置();
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateA正e本a成e輸入使設置idity() const
{
    if (En正i本on設置entalZones.的使設置() == 0)
    {
        本et使本n 輸入使設置idity;
    }
    
    float Total輸入使設置idity = 0.0f;
    fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
    {
        Total輸入使設置idity += Zone.輸入使設置idity;
    }
    
    本et使本n Total輸入使設置idity / En正i本on設置entalZones.的使設置();
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateTotalBiodi正e本sity() const
{
    float TotalBiodi正e本sity = 0.0f;
    
    fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
    {
        TotalBiodi正e本sity += Zone.Biodi正e本sity * Zone.Size;
    }
    
    本et使本n TotalBiodi正e本sity;
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateEcosyste設置輸入ealth() const
{
    if (Ecosyste設置s.的使設置() == 0)
    {
        本et使本n 0.0f;
    }
    
    float Total輸入ealth = 0.0f;
    fo本 (const a使to& Ecosyste設置Pai本 : Ecosyste設置s)
    {
        Total輸入ealth += Ecosyste設置Pai本.Val使e.輸入ealth;
    }
    
    本et使本n Total輸入ealth / Ecosyste設置s.的使設置();
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateCli設置ateStability() const
{
    if (Cli設置ateData.的使設置() < 2)
    {
        本et使本n 1.0f;
    }
    
    // Calc使late te設置pe本at使本e 正a本iance
    float TotalVa本iance = 0.0f;
    float MeanTe設置pe本at使本e = 0.0f;
    
    fo本 (const 軍Cli設置ateData& Data : Cli設置ateData)
    {
        MeanTe設置pe本at使本e += Data.Te設置pe本at使本e;
    }
    
    MeanTe設置pe本at使本e /= Cli設置ateData.的使設置();
    
    fo本 (const 軍Cli設置ateData& Data : Cli設置ateData)
    {
        TotalVa本iance += 軍Math::Sq使a本e(Data.Te設置pe本at使本e - MeanTe設置pe本at使本e);
    }
    
    float Va本iance = TotalVa本iance / Cli設置ateData.的使設置();
    
    // Con正e本t 正a本iance to stability (lowe本 正a本iance = hi成he本 stability)
    float Stability = 1.0f - 軍Math::Cla設置p(Va本iance / 100.0f, 0.0f, 1.0f);
    
    本et使本n Stability;
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateReso使本ceS使stainability() const
{
    if (Reso使本ces.的使設置() == 0)
    {
        本et使本n 1.0f;
    }
    
    float TotalS使stainability = 0.0f;
    
    fo本 (const a使to& Reso使本cePai本 : Reso使本ces)
    {
        const 軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本cePai本.Val使e;
        
        // Calc使late s使stainability based on c使本本ent 正s o本i成inal q使antity
        float Reso使本ceS使stainability = Reso使本ce.Q使antity / Reso使本ce.O本i成inalQ使antity;
        Reso使本ceS使stainability *= Reso使本ce.Q使ality; // 軍acto本 in q使ality
        
        TotalS使stainability += Reso使本ceS使stainability;
    }
    
    本et使本n TotalS使stainability / Reso使本ces.的使設置();
}

float UMin成Ecolo成icalEn正i本on設置entSyste設置::Calc使lateEcosyste設置Balance(const 軍St本in成& Ecosyste設置ID) const
{
    if (!Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        本et使本n 0.0f;
    }
    
    const 軍Ecosyste設置& Ecosyste設置 = Ecosyste設置s[Ecosyste設置ID];
    
    // Calc使late balance based on 設置使ltiple facto本s
    float 輸入ealth軍acto本 = Ecosyste設置.輸入ealth;
    float Biodi正e本sity軍acto本 = Ecosyste設置.Biodi正e本sity;
    float Resilience軍acto本 = Ecosyste設置.Resilience;
    
    // Check zone balance
    float ZoneBalance = 0.0f;
    int32 ZoneCo使nt = 0;
    
    fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
    {
        if (Zone.Ecosyste設置ID == Ecosyste設置ID)
        {
            ZoneBalance += (Zone.軍e本tility + Zone.Biodi正e本sity + Zone.Reso使本ceDensity) / 3.0f;
            ZoneCo使nt++;
        }
    }
    
    if (ZoneCo使nt > 0)
    {
        ZoneBalance /= ZoneCo使nt;
    }
    
    本et使本n (輸入ealth軍acto本 + Biodi正e本sity軍acto本 + Resilience軍acto本 + ZoneBalance) / 4.0f;
}

正oid UMin成Ecolo成icalEn正i本on設置entSyste設置::ApplyBalancin成Meas使本es(const 軍St本in成& Ecosyste設置ID)
{
    if (!Ecosyste設置s.Contains(Ecosyste設置ID))
    {
        本et使本n;
    }
    
    軍Ecosyste設置& Ecosyste設置 = Ecosyste設置s[Ecosyste設置ID];
    
    // Resto本e health
    Ecosyste設置.輸入ealth = 軍Math::Min(Ecosyste設置.輸入ealth + 0.1f, 1.0f);
    
    // I設置p本o正e balance
    Ecosyste設置.Balance = 軍Math::Min(Ecosyste設置.Balance + 0.05f, 1.0f);
    
    // Boost 本eso使本ce 本e成ene本ation in affected zones
    fo本 (a使to& Reso使本cePai本 : Reso使本ces)
    {
        軍En正i本on設置entalReso使本ce& Reso使本ce = Reso使本cePai本.Val使e;
        
        fo本 (const 軍En正i本on設置entalZone& Zone : En正i本on設置entalZones)
        {
            if (Zone.Ecosyste設置ID == Ecosyste設置ID && Reso使本ce.ZoneID == Zone.ZoneID)
            {
                Reso使本ce.Re成ene本ationRate *= 1.5f; // Boost 本e成ene本ation
                b本eak;
            }
        }
    }
    
    UE下LOG(Lo成Te設置p, Lo成, TEXT("Applied balancin成 設置eas使本es to ecosyste設置: %s"), *Ecosyste設置ID);
}
